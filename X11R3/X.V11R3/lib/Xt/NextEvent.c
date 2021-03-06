#ifndef lint
static char Xrcsid[] = "$XConsortium: NextEvent.c,v 1.59 89/01/18 17:04:01 swick Exp $";
/* $oHeader: NextEvent.c,v 1.4 88/09/01 11:43:27 asente Exp $ */
#endif lint

/***********************************************************
Copyright 1987, 1988 by Digital Equipment Corporation, Maynard, Massachusetts,
and the Massachusetts Institute of Technology, Cambridge, Massachusetts.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the names of Digital or MIT not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.  

DIGITAL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/

#include <stdio.h>
#include <errno.h>
#include <X11/Xlib.h>
#include "IntrinsicI.h"

extern int errno;

/*
 * Private routines
 */
#define ADD_TIME(dest, src1, src2) { \
	if(((dest).tv_usec = (src1).tv_usec + (src2).tv_usec) >= 1000000) {\
	      (dest).tv_usec -= 1000000;\
	      (dest).tv_sec = (src1).tv_sec + (src2).tv_sec + 1 ; \
	} else { (dest).tv_sec = (src1).tv_sec + (src2).tv_sec ; \
	   if(((dest).tv_sec >= 1) && (((dest).tv_usec <0))) { \
	    (dest).tv_sec --;(dest).tv_usec += 1000000; } } }


#define TIMEDELTA(dest, src1, src2) { \
	if(((dest).tv_usec = (src1).tv_usec - (src2).tv_usec) < 0) {\
	      (dest).tv_usec += 1000000;\
	      (dest).tv_sec = (src1).tv_sec - (src2).tv_sec - 1;\
	} else 	(dest).tv_sec = (src1).tv_sec - (src2).tv_sec;  }

#define IS_AFTER(t1, t2) (((t2).tv_sec > (t1).tv_sec) \
	|| (((t2).tv_sec == (t1).tv_sec)&& ((t2).tv_usec > (t1).tv_usec)))

static void QueueTimerEvent(app, ptr)
    XtAppContext app;
    TimerEventRec *ptr;
{
        TimerEventRec *t,**tt;
        tt = &app->timerQueue;
        t  = *tt;
        while (t != NULL &&
                IS_AFTER(t->te_timer_value, ptr->te_timer_value)) {
          tt = &t->te_next;
          t  = *tt;
         }
         ptr->te_next = t;
         *tt = ptr;
}

/* 
 * Ultrix routine to block in the toolkit.  This should be the only call to 
 * select..
 *
 * This routine returns when there is something to be done
 *
 * Before calling this with ignoreInputs==False, app->outstandingQueue should
 * be checked; this routine will not verify that an alternate input source
 * has not already been enqueued.
 *
 *
 * _XtWaitForSomething( ignoreTimers, ignoreInputs, ignoreEvents,
 *			block, howlong, appContext)
 * Boolean ignoreTimers;     (Don't return if a timer would fire
 *				Also implies forget timers exist)
 *
 * Boolean ignoreInputs;     (Ditto for input callbacks )
 *
 * Boolean ignoreEvents;     (Ditto for X events)
 *
 * Boolean block;	     (Okay to block)
 * TimeVal howlong;	     (howlong to wait for if blocking and not
 *				doing Timers... Null mean forever.
 *				Maybe should mean shortest of both)
 * XtAppContext app;	     (Displays to check wait on)
 * Returns display for which input is available, if any
 * and if ignoreEvents==False, else returns -1
 *
 * if ignoring everything && block=True && howlong=NULL, you'll have
 * lots of time for coffee; better not try it!  In fact, it probably
 * makes little sense to do this regardless of the value of howlong
 * (bottom line is, we don't bother checking here).
 */
int _XtwaitForSomething(ignoreTimers, ignoreInputs, ignoreEvents,
			block, howlong, app)
	Boolean ignoreTimers;
	Boolean ignoreInputs;
	Boolean ignoreEvents;
	Boolean block;
	unsigned long *howlong;
	XtAppContext app;
{
	struct timezone cur_timezone;
	struct timeval  cur_time;
	struct timeval  start_time;
	struct timeval  wait_time;
	struct timeval  new_time;
	struct timeval  time_spent;
	struct timeval	max_wait_time;
	static struct timeval  zero_time = { 0 , 0};
	register struct timeval *wait_time_ptr;
	Fd_set rmaskfd, wmaskfd, emaskfd;
	static Fd_set zero = { 0 };
	int nfound, i, d;
	int ret;
	
 	if (block) {
		(void) gettimeofday (&cur_time, &cur_timezone);
		start_time = cur_time;
		if(howlong == NULL) { /* special case for ever */
			wait_time_ptr = 0;
		} else { /* block until at most */
			max_wait_time.tv_sec = *howlong/1000;
			max_wait_time.tv_usec = (*howlong %1000)*1000;
			wait_time_ptr = &max_wait_time;
		}
	} else {  /* don't block */
		max_wait_time = zero_time;
		wait_time_ptr = &max_wait_time;
	}
	while (1) {
		if (app->timerQueue != NULL && !ignoreTimers && block) {
		    if(IS_AFTER(cur_time, app->timerQueue->te_timer_value)) {
			TIMEDELTA (wait_time, app->timerQueue->te_timer_value, 
				   cur_time);
			if(howlong==NULL || IS_AFTER(wait_time,max_wait_time)){
				wait_time_ptr = &wait_time;
			} else {
				wait_time_ptr = &max_wait_time;
			}
		    } else wait_time_ptr = &zero_time;
		} 
		if( !ignoreInputs ) {
			rmaskfd = app->fds.rmask;
			wmaskfd = app->fds.wmask;
			emaskfd = app->fds.emask;
		} else {
			rmaskfd = zero;
			wmaskfd = zero;
			emaskfd = zero;
		}
		if (!ignoreEvents) {
		    for (d = 0; d < app->count; d++) {
			FD_SET (ConnectionNumber(app->list[d]), &rmaskfd);
		    }
		}
		nfound = select (app->fds.nfds, (int *) &rmaskfd,
			(int *) &wmaskfd, (int *) &emaskfd, wait_time_ptr);
		if (nfound == -1) {
			/*
			 *  interrupt occured recalculate time value and select
			 *  again.
			 */
			if (errno == EINTR) {
			    errno = 0;  /* errno is not self reseting */
			    if(block && wait_time_ptr != NULL) {
				(void) gettimeofday (&new_time, &cur_timezone);
				TIMEDELTA(time_spent, new_time, cur_time);
				cur_time = new_time;
				if(IS_AFTER(time_spent, *wait_time_ptr)) {
					TIMEDELTA(wait_time, *wait_time_ptr,
						  time_spent);
					wait_time_ptr = &wait_time;
					continue;
				} else {
					/* time is up anyway */
					nfound = 0;
				}
			    }
			} else {
				XtAppErrorMsg(app, "communicationError","select",
                                        "XtToolkitError","Select failed",
                                         (String *)NULL, (Cardinal *)NULL);
			}
		} /* timed out or input available */
		break;
	}
	
	if (nfound == 0) {
		if(howlong) *howlong = (unsigned long)0;  /* Timed out */
		return -1;
	}
	if(block && wait_time_ptr != NULL) { /* adjust howlong */
		(void) gettimeofday (&new_time, &cur_timezone);
		TIMEDELTA(time_spent, new_time, start_time);
		if(howlong != NULL) {
		   if(*howlong <=
		    (time_spent.tv_sec*1000+time_spent.tv_usec/1000)){
			*howlong = (unsigned long)0;  /* Timed out */
		   } else *howlong -= 
		       (time_spent.tv_sec*1000+time_spent.tv_usec/1000);
	        }
	}
	if(ignoreInputs) {
	    if (ignoreEvents) return -1; /* then only doing timers */
	    for (d = 0; d < app->count; d++) {
		if (FD_ISSET(ConnectionNumber(app->list[d]), &rmaskfd)) {
		    return d;
		}
	    }
        }
	ret = -1;
	for (i = 0; i < app->fds.nfds && nfound > 0; i++) {
	    if (FD_ISSET (i, &rmaskfd)) {
		nfound--;
		if (!ignoreEvents) {
		    for (d = 0; d < app->count; d++) {
			if (i == ConnectionNumber(app->list[d])) {
			    if (ret == -1) ret = d;
			    goto ENDILOOP;
			}
		    }
		}

		app->selectRqueue[i]->ie_oq = app->outstandingQueue;
		app->outstandingQueue = app->selectRqueue[i];
	    }
	    if (FD_ISSET (i, &wmaskfd)) {
		app->selectWqueue[i]->ie_oq = app->outstandingQueue;
		app->outstandingQueue = app->selectWqueue[i];
		nfound--;
	    }
	    if (FD_ISSET (i, &emaskfd)) {
		app->selectEqueue[i]->ie_oq = app->outstandingQueue;
		app->outstandingQueue = app->selectEqueue[i];
		nfound--;
	    }
ENDILOOP:   ;
	}
	return ret;
}

static void IeCallProc(ptr)
	InputEvent *ptr;
{
	(* (ptr->ie_proc))( ptr->ie_closure, &ptr->ie_source, &ptr);
}

static void TeCallProc(ptr)
	TimerEventRec *ptr;
{
	(* (ptr->te_proc))( ptr->te_closure, &ptr);
}

/*
 * Public Routines
 */

XtIntervalId XtAddTimeOut(interval, proc, closure)
	unsigned long interval;
	XtTimerCallbackProc proc;
	caddr_t closure;
{
	return XtAppAddTimeOut(_XtDefaultAppContext(), 
		interval, proc, closure); 
}


XtIntervalId XtAppAddTimeOut(app, interval, proc, closure)
	XtAppContext app;
	unsigned long interval;
	XtTimerCallbackProc proc;
	caddr_t closure;
{
	TimerEventRec *tptr;
        struct timeval current_time;
	struct timezone timezone;

	tptr = (TimerEventRec *)XtMalloc((unsigned) sizeof(TimerEventRec));
	tptr->te_next = NULL;
	tptr->te_closure = closure;
	tptr->te_proc = proc;
	tptr->app = app;
	tptr->te_timer_value.tv_sec = interval/1000;
	tptr->te_timer_value.tv_usec = (interval%1000)*1000;
        (void) gettimeofday(&current_time,&timezone);
        ADD_TIME(tptr->te_timer_value,tptr->te_timer_value,current_time);
	QueueTimerEvent(app, tptr);
	return( (XtIntervalId) tptr);
}

void  XtRemoveTimeOut(id)
    XtIntervalId id;
{
   TimerEventRec *t, *last, *tid = (TimerEventRec *) id;

   /* find it */

   for(t = tid->app->timerQueue, last = NULL;
	   t != NULL && t != (TimerEventRec *)id;
	   t = t->te_next) last = t;

   if (t == NULL) return; /* couldn't find it */
   if(last == NULL) { /* first one on the list */
    tid->app->timerQueue = t->te_next;
   } else last->te_next = t->te_next;

   XtFree((char*)t);
   return;
}

XtWorkProcId XtAddWorkProc(proc, closure)
	XtWorkProc proc;
	caddr_t closure;
{
	return XtAppAddWorkProc(_XtDefaultAppContext(), proc, closure);
}

XtWorkProcId XtAppAddWorkProc(app, proc, closure)
	XtAppContext app;
	XtWorkProc proc;
	caddr_t closure;
{
	WorkProcRec *wptr;

	wptr = XtNew(WorkProcRec);
	wptr->next = app->workQueue;
	wptr->closure = closure;
	wptr->proc = proc;
	wptr->app = app;
	app->workQueue = wptr;

	return (XtWorkProcId) wptr;
}

void  XtRemoveWorkProc(id)
	XtWorkProcId id;
{
	WorkProcRec *wid= (WorkProcRec *) id, *w, *last;

	/* find it */
	for(w = wid->app->workQueue, last = NULL; w != NULL && w != wid; w = w->next) last = w;

	if (w == NULL) return; /* couldn't find it */

	if(last == NULL) wid->app->workQueue = w->next;
	else last->next = w->next;

	XtFree((char *) w);
}

XtInputId XtAddInput( source, Condition, proc, closure)
	int source;
	caddr_t Condition;
	XtInputCallbackProc proc;
	caddr_t closure;
{
	return XtAppAddInput(_XtDefaultAppContext(),
		source, Condition, proc, closure);
}

XtInputId XtAppAddInput(app, source, Condition, proc, closure)
	XtAppContext app;
	int source;
	caddr_t Condition;
	XtInputCallbackProc proc;
	caddr_t closure;
{
	InputEvent *sptr;
	XtInputMask condition = (XtInputMask) Condition;
	
	sptr = XtNew(InputEvent);
	if(condition == XtInputReadMask){
	    sptr->ie_next = app->selectRqueue[source];
	    app->selectRqueue[source] = sptr;
	    FD_SET(source, &app->fds.rmask);
	} else if(condition == XtInputWriteMask) {
	    sptr->ie_next = app->selectWqueue[source];
	    app->selectWqueue[source] = sptr;
	    FD_SET(source, &app->fds.wmask);
	} else if(condition == XtInputExceptMask) {
	    sptr->ie_next = app->selectEqueue[source];
	    app->selectEqueue[source] = sptr;
	    FD_SET(source, &app->fds.emask);
	} else
	  XtAppErrorMsg(app, "invalidParameter","xtAddInput","XtToolkitError",
                  "invalid condition passed to XtAddInput",
                   (String *)NULL, (Cardinal *)NULL);
	sptr->ie_proc = proc;
	sptr->ie_closure =closure;
	sptr->app = app;
	sptr->ie_oq = NULL;
	sptr->ie_source = source;
	
	if (app->fds.nfds < (source+1)) app->fds.nfds = source+1;
	app->fds.count++;
	return((XtInputId)sptr);

}

void XtRemoveInput( id )
	XtInputId  id;
{
  	register InputEvent *sptr, *lptr, *iid = (InputEvent *) id;
	XtAppContext app = iid->app;
	register source;
	
	source = iid->ie_source;
	app->fds.count--;

	sptr = app->outstandingQueue;
	lptr = NULL;
	for (; sptr != NULL; sptr = sptr->ie_oq) {
	    if (sptr == iid) {
		if (lptr == NULL) app->outstandingQueue = sptr->ie_oq;
		else lptr->ie_oq = sptr->ie_oq;
	    }
	    lptr = sptr;
	}

	if((sptr = app->selectRqueue[source]) != NULL) {
		for( lptr = NULL ; sptr; sptr = sptr->ie_next ){
			if(sptr == (InputEvent *) id) {
				if(lptr == NULL) {
					app->selectRqueue[source] = sptr->ie_next;
					FD_CLR(source, &app->fds.rmask);
				} else {
					lptr->ie_next = sptr->ie_next;
				}
				XtFree((char *) sptr);
				return;
			}
			lptr = sptr;	      
		}
	}
	if((sptr = app->selectWqueue[source]) != NULL) {
		for(lptr = NULL;sptr; sptr = sptr->ie_next){
			if ( sptr ==  (InputEvent *) id) {
				if(lptr == NULL){
					app->selectWqueue[source] = sptr->ie_next;
					FD_CLR(source, &app->fds.wmask);
				}else {
					lptr->ie_next = sptr->ie_next;
				}
				XtFree((char *) sptr);
				return;
			}
			lptr = sptr;
		}
	    
	}
	if((sptr = app->selectEqueue[source]) != NULL) {
		for(lptr = NULL;sptr; sptr = sptr->ie_next){
			if ( sptr ==  (InputEvent *) id) {
				if(lptr == NULL){
					app->selectEqueue[source] = sptr->ie_next;
					FD_CLR(source, &app->fds.emask);
				}else {
					lptr->ie_next = sptr->ie_next;
				}
				XtFree((char *) sptr);
				return;
			}
			lptr = sptr;
		}
	    
	}
	XtAppWarningMsg(app, "invalidProcedure","inputHandler","XtToolkitError",
                   "XtRemoveInput: Input handler not found",
		   (String *)NULL, (Cardinal *)NULL);
	app->fds.count++;	/* Didn't remove it after all */
}

/* Do alternate input and timer callbacks if there are any */

static void DoOtherSources(app)
	XtAppContext app;
{
	register TimerEventRec *te_ptr;
	register InputEvent *ie_ptr;
	struct timeval  cur_time;
	struct timezone cur_timezone;

#define DrainQueue() \
	for (ie_ptr = app->outstandingQueue; ie_ptr != NULL;) { \
	    app->outstandingQueue = ie_ptr->ie_oq;		\
	    ie_ptr ->ie_oq = NULL;				\
	    IeCallProc(ie_ptr);					\
	    ie_ptr = app->outstandingQueue;			\
	}
/*enddef*/
	DrainQueue();
	if (app->fds.count > 0) {
	    /* Call _XtwaitForSomething to get input queued up */
	    (void) _XtwaitForSomething(TRUE, FALSE, TRUE, FALSE,
		(unsigned long *)NULL, app);
	    DrainQueue();
	}
	if (app->timerQueue != NULL) {	/* check timeout queue */
	    (void) gettimeofday (&cur_time, &cur_timezone);
	    while(IS_AFTER (app->timerQueue->te_timer_value, cur_time)) {
		te_ptr = app->timerQueue;
		app->timerQueue = te_ptr->te_next;
		te_ptr->te_next = NULL;
               if (te_ptr->te_proc != 0)
		  TeCallProc(te_ptr);
		XtFree((char*)te_ptr);
              if (app->timerQueue == NULL) break;
	    }
	}
#undef DrainQueue
}

/* If there are any work procs, call them.  Return whether we did so */

static Boolean CallWorkProc(app)
	XtAppContext app;
{
	register WorkProcRec *w = app->workQueue;
	Boolean delete;

	if (w == NULL) return FALSE;

	app->workQueue = w->next;

	delete = (*(w->proc)) (w->closure);

	if (delete) XtFree((char *) w);
	else {
	    w->next = app->workQueue;
	    app->workQueue = w;
	}
	return TRUE;
}

/*
 * XtNextEvent()
 * return next event;
 */

void XtNextEvent(event)
	XEvent *event;
{
	XtAppNextEvent(_XtDefaultAppContext(), event);
}

static void _RefreshMapping(event)
    XEvent *event;
{
    XtPerDisplay pd = _XtGetPerDisplay(event->xmapping.display);
    if (pd != NULL) {
	XtFree((char *) pd->keysyms);
	pd->keysyms = NULL;
	XtFree((char *) pd->modKeysyms);
	pd->modKeysyms = NULL;
	XtFree((char *) pd->modsToKeysyms);
	pd->modsToKeysyms = NULL;
	_XtBuildKeysymTable(event->xmapping.display,pd);
	pd ->modsToKeysyms =_XtBuildModsToKeysymTable(
	    event->xmapping.display,pd);
    }
    XRefreshKeyboardMapping(event);
}

void XtAppNextEvent(app, event)
	XtAppContext app;
	XEvent *event;
{
    int i, d;

    for (;;) {
	for (i = 1; i <= app->count; i++) {
	    d = (i + app->last) % app->count;
	    if (d == 0) DoOtherSources(app);
	    if (XEventsQueued(app->list[d], QueuedAfterFlush) > 0) {
		XNextEvent(app->list[d], event);
		app->last = d;
		if (event->xany.type == MappingNotify)
		    _RefreshMapping(event);
		return;
	    }
	}

	/* We're ready to wait...if there is a work proc, call it */
	if (CallWorkProc(app)) continue;

	d = _XtwaitForSomething(FALSE, FALSE, FALSE, TRUE,
				(unsigned long *) NULL, app);

	if (d != -1) {
	    XNextEvent (app->list[d], event);
	    app->last = d;;
	    if (event->xany.type == MappingNotify)
		_RefreshMapping(event);
	    return;
	} 

    } /* for */
}
    
void XtProcessEvent(mask)
	XtInputMask mask;
{
	void XtAppProcessEvent();
	XtAppProcessEvent(_XtDefaultAppContext(), mask);
}

void XtAppProcessEvent(app, mask)
	XtAppContext app;
	XtInputMask mask;
{
	InputEvent *ie_ptr;
	TimerEventRec *te_ptr;
	int i, d;
	XEvent event;
	struct timeval cur_time;
	struct timezone curzone;

	if (mask == 0) return;

	for (;;) {
	    if (mask & XtIMTimer && app->timerQueue != NULL) {
		(void) gettimeofday (&cur_time, &curzone);
		if (IS_AFTER(app->timerQueue->te_timer_value, cur_time)) {
		    te_ptr = app->timerQueue;
		    app->timerQueue = app->timerQueue->te_next;
		    te_ptr->te_next = NULL;
                    if (te_ptr->te_proc != 0)
		        TeCallProc(te_ptr);
		    XtFree((char*)te_ptr);
		    return;
		}
	    }
    
	    if (mask & XtIMAlternateInput) {
		if (app->fds.count > 0 && app->outstandingQueue == NULL) {
		    /* Call _XtwaitForSomething to get input queued up */
		    (void) _XtwaitForSomething(TRUE, FALSE, TRUE, FALSE,
			    (unsigned long *)NULL, app);
		}
		if (app->outstandingQueue != NULL) {
		    ie_ptr = app->outstandingQueue;
		    app->outstandingQueue = ie_ptr->ie_oq;
		    ie_ptr->ie_oq = NULL;
		    IeCallProc(ie_ptr);
		    return;
		}
	    }
    
	    if (mask & XtIMXEvent) {
		for (i = 1; i <= app->count; i++) {
		    d = (i + app->last) % app->count;
		    if (XEventsQueued(app->list[d], QueuedAfterFlush) ) {
			XNextEvent(app->list[d], &event);
			app->last = d;
			if (event.xany.type == MappingNotify)
			    XRefreshKeyboardMapping(&event);
			XtDispatchEvent(&event);
			return;
		    }
		}
	    }

	    /* Nothing to do...wait for something */

	    if (CallWorkProc(app)) continue;

	    d = _XtwaitForSomething(
				    (mask & XtIMTimer ? FALSE : TRUE),
				    (mask & XtIMAlternateInput ? FALSE : TRUE),
				    (mask & XtIMXEvent ? FALSE : TRUE),
				    TRUE,
				    (unsigned long *) NULL, app);
	    
	    if (mask & XtIMXEvent && d != -1) {
		XNextEvent(app->list[d], &event);
		app->last = d;
		if (event.xany.type == MappingNotify) {
		    XRefreshKeyboardMapping(&event);
		}
		XtDispatchEvent(&event);
		return;
	    } 
	
	}    
}

XtInputMask XtPending()
{
	return XtAppPending(_XtDefaultAppContext());
}

XtInputMask XtAppPending(app)
	XtAppContext app;
{
	struct timeval cur_time;
	struct timezone curzone;
	int d;
	XtInputMask ret = 0;

/*
 * Check for pending X events
 */
	for (d = 0; d < app->count; d++) {
	    if (XEventsQueued(app->list[d], QueuedAfterFlush)) {
		ret |= XtIMXEvent;
		break;
	    }
	}

/*
 * Check for pending alternate input
 */
	if (app->timerQueue != NULL) {	/* check timeout queue */ 
	    (void) gettimeofday (&cur_time, &curzone);
	    if ((IS_AFTER(app->timerQueue->te_timer_value, cur_time))  &&
                (app->timerQueue->te_proc != 0)) {
		ret |= XtIMTimer;
	    }
	}

	if (app->outstandingQueue != NULL) ret |= XtIMAlternateInput;
	else {
	    /* This won't cause a wait, but will enqueue any input */

	    if(_XtwaitForSomething(TRUE, FALSE, FALSE, FALSE, (unsigned long *) NULL,
		    app) != -1) ret |= XtIMXEvent;
	    if (app->outstandingQueue != NULL) ret |= XtIMAlternateInput;
	}
	return ret;
}

/* Peek at alternate input and timer callbacks if there are any */

Boolean PeekOtherSources(app)
	XtAppContext app;
{
	struct timeval  cur_time;
	struct timezone cur_timezone;

	if (app->outstandingQueue != NULL) return TRUE;

	if (app->fds.count > 0) {
	    /* Call _XtwaitForSomething to get input queued up */
	    (void) _XtwaitForSomething(TRUE, FALSE, TRUE, FALSE,
		    (unsigned long *)NULL, app);
	    if (app->outstandingQueue != NULL) return TRUE;
	}

	if (app->timerQueue != NULL) {	/* check timeout queue */
	    (void) gettimeofday (&cur_time, &cur_timezone);
	    if (IS_AFTER (app->timerQueue->te_timer_value, cur_time)) return TRUE;
	}

	return FALSE;
}

Boolean XtPeekEvent(event)
	XEvent *event;
{
	return XtAppPeekEvent(_XtDefaultAppContext(), event);
}

Boolean XtAppPeekEvent(app, event)
	XtAppContext app;
	XEvent *event;
{
	int i, d;
	Boolean foundCall = FALSE;

	for (;;) {
	    for (i = 1; i <= app->count; i++) {
		d = (i + app->last) % app->count;
		if (d == 0) foundCall = PeekOtherSources(app);
		if (XEventsQueued(app->list[d], QueuedAfterFlush) ) {
		    XPeekEvent(app->list[d], event);
		    app->last = (d == 0 ? app->count : d) - 1;
		    return TRUE;
		}
	    }

	    if (foundCall) {
		event->xany.type = 0;
		event->xany.display = NULL;
		event->xany.window = NULL;
		return FALSE;
	    }

	    d = _XtwaitForSomething(FALSE, FALSE, FALSE, TRUE,
		    (unsigned long *) NULL, app);

	    if (d != -1) {
		XPeekEvent(app->list[d], event);
		app->last = (d == 0 ? app->count : d) - 1;
		return TRUE;
	    }
	    event->xany.type = 0;	/* Something else must be ready */
	    event->xany.display = NULL;
	    event->xany.window = NULL;
	    return FALSE;

	    /* Greater than 0 is number of dpys with input -- loop around */
	}
}	
