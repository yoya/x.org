#include "copyright.h"

/* $Header: XIfEvent.c,v 11.9 88/02/03 20:44:51 rws Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#define NEED_EVENTS
#include "Xlibint.h"

extern _XQEvent *_qfree;

/* 
 * Flush output and (wait for and) return the next event matching the
 * predicate in the queue.
 */

XIfEvent (dpy, event, predicate, arg)
	register Display *dpy;
	Bool (*predicate)();		/* function to call */
	register XEvent *event;
	char *arg;
{
	register _XQEvent *qelt, *prev;
	
        LockDisplay(dpy);
	prev = NULL;
	while (1) {
	    for (qelt = prev ? prev->next : dpy->head;
		 qelt;
		 prev = qelt, qelt = qelt->next) {
		if ((*predicate)(dpy, &qelt->event, arg)) {
		    *event = qelt->event;
		    if (prev) {
			if ((prev->next = qelt->next) == NULL)
			    dpy->tail = prev;
		    } else {
			if ((dpy->head = qelt->next) == NULL)
			dpy->tail = NULL;
		    }
		    qelt->next = _qfree;
		    _qfree = qelt;
		    dpy->qlen--;
		    UnlockDisplay(dpy);
		    return;
		}
	    }
	    _XReadEvents(dpy);
	}
}
