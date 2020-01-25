#ifndef lint
static char rcsid[] = "$Header: NextEvent.c,v 1.40 88/02/26 12:43:29 swick Exp $";
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
#include <X11/Xos.h>
#include <X11/Xlib.h>
#include "IntrinsicI.h"
#include <sys/param.h>
#include "fd.h"

extern int errno;

/*
 * Private definitions
 */


typedef struct _TimerEventRec {

        struct timeval   te_timer_value;
	struct _TimerEventRec *te_next;
	Display *te_dpy;
	XtTimerCallbackProc	te_proc;
	caddr_t	te_closure;
}TimerEventRec;

static struct InputEvent {
	XtInputCallbackProc  ie_proc;
	caddr_t ie_closure;
	struct	InputEvent	*ie_next;
	struct  InputEvent	*ie_oq;
	int	ie_source;
};


/*
 * Private data
 */

static TimerEventRec *TimerQueue = NULL;

static struct InputEvent *Select_rqueue[NOFILE], *Select_wqueue[NOFILE],
  *Select_equeue[NOFILE];
static struct  InputEvent *outstanding_queue = NULL;


static struct 
{
  	xfd_set rmask;
	xfd_set wmask;
	xfd_set emask;
	int	nfds;
} composite;


  

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

static void QueueTimerEvent(ptr)
    TimerEventRec *ptr;
{
        TimerEventRec *t,**tt;
        tt = &TimerQueue;
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
 *
 * _XtWaitForSomething( ignoreTimers, ignoreInputs, block, howlong)
 * Boolean ignoreTimers;     (Don't return if a timer would fire
 *				Also implies forget timers exist)
 *
 * Boolean ignoreInputs;     (Ditto for input callbacks )
 *
 * Boolean block;	     (Okay to block)
 * TimeVal;		     (howlong to wait for if blocking and not
 *				doing Timers... Null mean forever.
 *				Maybe should mean shortest of both)
 * Returns a Boolean indicating if X input is available.
 */
Boolean
_XtwaitForSomething(ignoreTimers, ignoreInputs, block, howlong)
Boolean ignoreTimers;
Boolean ignoreInputs;
Boolean block;
unsigned long *howlong;
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
	xfd_set rmaskfd, wmaskfd, emaskfd;
	static xfd_set zero = { 0 };
	register int     nfound, i;
	Boolean ret;
	
	/* should be done only once */
	if (ConnectionNumber (toplevelDisplay) + 1 > composite.nfds)
	    composite.nfds = ConnectionNumber (toplevelDisplay) + 1;
 	if(block) {
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
		if (TimerQueue != NULL && !ignoreTimers && block) {
		    if(IS_AFTER(cur_time, TimerQueue->te_timer_value)) {
			TIMEDELTA (wait_time, TimerQueue->te_timer_value, 
				   cur_time);
			if(howlong==NULL || IS_AFTER(wait_time,max_wait_time)){
				wait_time_ptr = &wait_time;
			} else {
				wait_time_ptr = &max_wait_time;
			}
		    } else wait_time_ptr = &zero_time;
		} 
		if( !ignoreInputs ) {
			XFD_SET (ConnectionNumber (toplevelDisplay), 
				 &composite.rmask);
			rmaskfd = composite.rmask;
			wmaskfd = composite.wmask;
			emaskfd = composite.emask;
		} else {
			rmaskfd = wmaskfd = emaskfd = zero;
			XFD_SET (ConnectionNumber (toplevelDisplay), &rmaskfd);
		}
		if ((nfound = select (composite.nfds,
			      (int *) & rmaskfd, (int *) & wmaskfd,
			      (int *) & emaskfd, wait_time_ptr)) == -1) {
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
				XtError("Select failed.");
			}
		} /* timed out or input available */
		break;
	}
	
	if (nfound == 0) {
		if(howlong)
			*howlong = (unsigned long)0;  /* Timed out */
		return ( FALSE ) ;
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
	  if(XFD_ISSET( ConnectionNumber (toplevelDisplay), &rmaskfd))
	    return TRUE;
        }
	ret = FALSE;
	for (i = 0; i < composite.nfds && nfound > 0; i++) {
	    if (XFD_ISSET (i, &rmaskfd)) {
		if (i == ConnectionNumber (toplevelDisplay)) {
			ret = TRUE;
		} else {
		    Select_rqueue[i]->ie_oq = outstanding_queue;
		    outstanding_queue = Select_rqueue[i];
		    nfound--;
		}
	    }
	    if (XFD_ISSET (i, &wmaskfd)) {
		Select_wqueue[i]->ie_oq = outstanding_queue;
		outstanding_queue = Select_wqueue[i];
		nfound--;
	    }
	    if (XFD_ISSET (i, &emaskfd)) {
		Select_equeue[i]->ie_oq = outstanding_queue;
		outstanding_queue = Select_equeue[i];
		nfound--;
	    }
    }
    return ret;
}

static void
IeCallProc(ptr)
struct InputEvent *ptr;
{
	(* (ptr->ie_proc))( ptr->ie_closure, &ptr->ie_source, &ptr);
}

static void
TeCallProc(ptr)
TimerEventRec *ptr;
{
	(* (ptr->te_proc))( ptr->te_closure, &ptr);
}

/*
 * Public Routines
 */

XtIntervalId
XtAddTimeOut( interval, proc, closure)
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
	tptr->te_timer_value.tv_sec = interval/1000;
	tptr->te_timer_value.tv_usec = (interval%1000)*1000;
        (void) gettimeofday(&current_time,&timezone);
        ADD_TIME(tptr->te_timer_value,tptr->te_timer_value,current_time);
	QueueTimerEvent(tptr);
	return( (XtIntervalId) tptr);
}

void  XtRemoveTimeOut(id)
    XtIntervalId id;
{
   TimerEventRec *t, *last;

   /* find it */

   for(t = TimerQueue, last = NULL; t != NULL && t != (TimerEventRec *)id ;
       t = t->te_next) {
	   last = t;
   }
   if (t == NULL) return; /* couldn't find it */
   if(last == NULL) { /* first one on the list */
	   TimerQueue = t->te_next;
   } else last->te_next = t->te_next;

   XtFree((char*)t);
   return;
}

XtInputId
XtAddInput( source, Condition, proc, closure)
int source;
caddr_t Condition;
XtInputCallbackProc proc;
caddr_t closure;
{
	struct InputEvent *sptr;
	XtInputMask condition = (XtInputMask) Condition;
	
	if(condition == XtInputReadMask){
	    sptr = (struct InputEvent *)XtMalloc((unsigned) sizeof (*sptr));
	    sptr->ie_next = Select_rqueue[source];
	    sptr->ie_source = source;
	    Select_rqueue[source] = sptr;
	    XFD_SET(source, &composite.rmask);
	} else if(condition == XtInputWriteMask) {
	    sptr = (struct InputEvent *) XtMalloc((unsigned) sizeof (*sptr));
	    sptr->ie_next = Select_wqueue[source];
	    Select_wqueue[source] = sptr;
	    XFD_SET(source, &composite.wmask);
	} else if(condition == XtInputExceptMask) {
	    sptr = (struct InputEvent *) XtMalloc((unsigned) sizeof (*sptr));
	    sptr->ie_next = Select_equeue[source];
	    Select_equeue[source] = sptr;
	    XFD_SET(source, &composite.emask);
	} else
	  XtError("invalid condition passed to XtAddInput");
	sptr->ie_proc = proc;
	sptr->ie_closure =closure;

	if (composite.nfds < (source+1))
	    composite.nfds = source+1;
	return((XtInputId)sptr);
}

void XtRemoveInput( id )
XtInputId  id;
{
  	register struct InputEvent *sptr, *lptr;
	register source;

	source = ( (struct InputEvent *) id)->ie_source;

	if((sptr = Select_rqueue[source]) != NULL) {
		for( lptr = NULL ; sptr; sptr = sptr->ie_next ){
			if(sptr == (struct InputEvent *) id) {
				if(lptr == NULL) {
					Select_rqueue[source] = sptr->ie_next;
					XFD_CLR(source, &composite.rmask);
				} else {
					lptr->ie_next = sptr->ie_next;
				}
				XtFree((char *) sptr);
				return;
			}
			lptr = sptr;	      
		}
	}
	if((sptr = Select_wqueue[source]) != NULL) {
		for(lptr = NULL;sptr; sptr = sptr->ie_next){
			if ( sptr ==  (struct InputEvent *) id) {
				if(lptr == NULL){
					Select_wqueue[source] = sptr->ie_next;
					XFD_CLR(source, &composite.wmask);
				}else {
					lptr->ie_next = sptr->ie_next;
				}
				XtFree((char *) sptr);
				return;
			}
			lptr = sptr;
		}
	    
	}
	if((sptr = Select_equeue[source]) != NULL) {
		for(lptr = NULL;sptr; sptr = sptr->ie_next){
			if ( sptr ==  (struct InputEvent *) id) {
				if(lptr == NULL){
					Select_equeue[source] = sptr->ie_next;
					XFD_CLR(source, &composite.emask);
				}else {
					lptr->ie_next = sptr->ie_next;
				}
				XtFree((char *) sptr);
				return;
			}
			lptr = sptr;
		}
	    
	}
	XtWarning("XtRemoveInput: Input handler not found");
}

     
/*
 * XtNextEvent()
 * return next event;
 */

void XtNextEvent(event)
XEvent *event;
{
    register struct InputEvent *ie_ptr;
    register TimerEventRec *te_ptr;
    struct timeval  cur_time;
    struct timezone cur_timezone;
    register Boolean     Claims_X_is_pending = FALSE;

    if (DestroyList != NULL) {
	_XtCallCallbacks (&DestroyList, (caddr_t) NULL);
	_XtRemoveAllCallbacks (&DestroyList);
    }

    for (;;) {
	if (Claims_X_is_pending || XPending (toplevelDisplay) ) {
		XNextEvent (toplevelDisplay, event);
		return;
	}
	for (ie_ptr = outstanding_queue ; ie_ptr != NULL;
	     outstanding_queue = ie_ptr = ie_ptr->ie_oq ) {
		ie_ptr ->ie_oq = NULL;
		IeCallProc(ie_ptr);
	}
	(void) gettimeofday (&cur_time, &cur_timezone);
	if (TimerQueue!= NULL) {	/* check timeout queue */
		while(IS_AFTER (TimerQueue->te_timer_value, cur_time)) {
			te_ptr = TimerQueue;
			TimerQueue = TimerQueue->te_next;
			te_ptr->te_next = NULL;
			TeCallProc(te_ptr);
			XtFree((char*)te_ptr);
		}
	}
	XFlush(toplevelDisplay);
	Claims_X_is_pending = _XtwaitForSomething(
	    FALSE, FALSE, TRUE, (unsigned long *)NULL);
    }
}

Boolean XtPending()
{
	struct timeval cur_time;
	struct timezone curzone;
	struct InputEvent *ie_ptr;
	TimerEventRec *te_ptr;

	for(;;) {
		if(XPending(toplevelDisplay) != 0) {
			return TRUE;
		}
		for (ie_ptr = outstanding_queue ; ie_ptr != NULL;
		     outstanding_queue = ie_ptr->ie_oq ) {
			ie_ptr ->ie_oq = NULL;
			IeCallProc(ie_ptr);
		}
		(void) gettimeofday (&cur_time, &curzone);

		if (TimerQueue!= NULL) {	/* check timeout queue */
			while(IS_AFTER(TimerQueue->te_timer_value, cur_time)) {
				te_ptr = TimerQueue;
				TimerQueue = TimerQueue->te_next;
				te_ptr->te_next = NULL;
				TeCallProc(te_ptr);
				XtFree((char*)te_ptr);
			}
		}
		if(_XtwaitForSomething(
			TRUE, FALSE, FALSE, (unsigned long *)NULL))
		  continue;
		for (ie_ptr = outstanding_queue ; ie_ptr != NULL;
		     outstanding_queue = ie_ptr->ie_oq ) {
			ie_ptr ->ie_oq = NULL;
			IeCallProc(ie_ptr);
		}
		return FALSE;
	}
}

XtPeekEvent(event)
XEvent *event;
{
    xfd_set rmaskfd, wmaskfd, emaskfd;
    register int nfound, i;
    struct timeval cur_time, wait_time, *wait_time_ptr;
    struct timezone curzone;
    int Claims_X_is_pending = 0;
    register struct InputEvent *ie_ptr;
    register TimerEventRec *te_ptr;


    for(;;) {
	    if(XPending(toplevelDisplay)){
		    XPeekEvent(toplevelDisplay, event); /* Xevents */
		    return 1;
	    }
	    for (ie_ptr = outstanding_queue ; ie_ptr != NULL;
		 outstanding_queue = ie_ptr->ie_oq ) {
		    ie_ptr ->ie_oq = NULL;
		    IeCallProc(ie_ptr);
	    }
	    (void) gettimeofday (&cur_time, &curzone);
	    if (TimerQueue!= NULL) {	/* check timeout queue */
		    while(IS_AFTER (TimerQueue->te_timer_value, cur_time)) {
			    te_ptr = TimerQueue;
			    TimerQueue = TimerQueue->te_next;
			    te_ptr->te_next = NULL;
			    TeCallProc(te_ptr);
			    XtFree((char*)te_ptr);
		    }
	    }

	    /* No timers ready time to wait */
	    /* should be done only once */
	    if (ConnectionNumber (toplevelDisplay) + 1 > composite.nfds)
	      composite.nfds = ConnectionNumber (toplevelDisplay) + 1;
	    while (1) {
		    XFD_SET(ConnectionNumber (toplevelDisplay), 
			   &composite.rmask);
		    if (TimerQueue != NULL) {
			    TIMEDELTA (wait_time, TimerQueue->te_timer_value, 
				       cur_time);
			    wait_time_ptr = &wait_time;
		    } else wait_time_ptr = (struct timeval *) 0;
		    rmaskfd = composite.rmask;
		    wmaskfd = composite.wmask;
		    emaskfd = composite.emask;
		    if ((nfound = select (composite.nfds,
				   (int *) & rmaskfd, (int *) & wmaskfd,
				   (int *) & emaskfd, wait_time_ptr)) == -1) {
			    if (errno == EINTR) continue;
		    }
		    if (nfound == -1) XtError("Select failed.");
		    break;
	    }
	    if (nfound == 0)
	      continue;
	    
	    for(i = 0; i < composite.nfds && nfound > 0;i++) {
		    if(XFD_ISSET(i,&rmaskfd)) {
			    if(i == ConnectionNumber(toplevelDisplay)) {
				    Claims_X_is_pending= 1;
			    } 
			    if( Select_rqueue[i] != NULL) {
				    Select_rqueue[i] -> ie_oq = 
				      outstanding_queue;
				    outstanding_queue = Select_rqueue[i];
				    nfound--;
			    }
			    if(XFD_ISSET(i,&wmaskfd)) {
				    Select_wqueue[i] -> ie_oq = 
				      outstanding_queue;
				    outstanding_queue = Select_wqueue[i];
				    nfound--;
			    }
			    if(XFD_ISSET(i,&emaskfd)) {
				    Select_equeue[i] -> ie_oq = 
				      outstanding_queue;
				    outstanding_queue = Select_equeue[i];
				    nfound--;
			    }
		    }
	    }
	    if(Claims_X_is_pending && XPending(toplevelDisplay)) {
		    XPeekEvent(toplevelDisplay, event);
		    return(1);
	    }
	    return(0);
    }
}	
