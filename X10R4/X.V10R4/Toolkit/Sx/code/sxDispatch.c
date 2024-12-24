/*
 *	$Source: /u1/Sx.new/code/RCS/sxDispatch.c,v $
 *	$Header: sxDispatch.c,v 1.1 86/12/03 16:09:48 swick Exp $
 */

#ifndef lint
static char *rcsid_sxDispatch_c = "$Header: sxDispatch.c,v 1.1 86/12/03 16:09:48 swick Exp $";
#endif	lint

/* 
 * sxDispatch.c --
 *
 *	This file implements the sx dispatcher, which processes
 *	events and calls clients that have expressed an interest
 *	in the events.
 *
 * Copyright (C) 1986 Regents of the University of California
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting
 * documentation.  The University of California makes no
 * representations about the suitability of this software for
 * any purpose.  It is provided "as is" without express or
 * implied warranty.
 */

#ifndef lint
static char rcsid[] = "$Header: sxDispatch.c,v 1.1 86/12/03 16:09:48 swick Exp $ SPRITE (Berkeley)";
#endif not lint

#include <sys/types.h>
#include <X/Xlib.h>
#include "sprite.h"
#include "mem.h"
#include "sx.h"

/*
 * Miscellaneous imports:
 */

extern XAssocTable *XCreateAssocTable();

/*
 * The following data structure is used to keep track of the handlers
 * declared by calls to Sx_HandlerCreate.  One such structure is allocated
 * for each call to Sx_HandlerCreate.  In order to keep users from having
 * to see the guts of a handler, it's declared here as a different type
 * and cast into and out of the user's type as needed.
 */

typedef struct Handler {
    Window w;			/* Window in which event must occur. */
    int mask;			/* Mask of events in w that are desired. */
    void (*proc)();		/* Procedure to call for each such event. */
    ClientData clientData;	/* Additional stuff to pass to proc. */
    struct Handler *nextPtr;	/* Next handler in list of those associated
				 * with this window, or NULL for end of
				 * list. */
} Handler;

/*
 * The data structure below is kept on a window-by-window basis.  There
 * is one for each window that's created by this client or used in a
 * Sx_HandlerCreate call.  These structures record the window structure
 * so that when windows get deleted, clients requested SX_DESTROYED events
 * can be notified (must keep track of structure because when parent gets
 * deleted X will automatically delete children).  The structure also
 * points to a list of handlers that have been declared for the window.
 * The addresses of WindowStuff structures are kept in the windowAssocs
 * hash table.
 */

typedef struct WindowStuff {
    Window w;				/* X's id for window. */
    int width, height;			/* Must save width and height to help
					 * in translating ExposeRegion events
					 * into ExposeWindow events. */
    Handler *firstHandler;		/* First in list of handlers
					 * associated with this window. */
    struct WindowStuff *parentPtr;	/* Points to parent, if this window's
					 * parent is known, or NULL if parent
					 * isn't known (because window wasn't
					 * created by this client). */
    struct WindowStuff *childPtr;	/* First in list of children of this
					 * window.  NULL means no children.  */
    struct WindowStuff *nextPtr;	/* Next in list of siblings with same
					 * parent.  NULL means end of list. */
} WindowStuff;

/*
 * To make it easy to locate information associated with a particular
 * window, an XAssocTable is used to keep track of all the WindowStuff
 * structures:
 */

static XAssocTable *windowTable = NULL;

/*
 * There's a potential problem if a handler is deleted while it's
 * current (i.e. it's procedure is executing), since Sx_HandleEvent
 * will need to read the handle's "nextPtr" field when the procedure
 * returns.  To handle this problem, the variable below is used to
 * hold the address of the next handler to be considered by
 * Sx_HandleEvent.  It gets updated if the handler pointed to is
 * deleted.
 */

static Handler *nextHandler;

/*
 * X doesn't do mouse-grabbing quite right (it's grabbing by a CLIENT,
 * not a window), so we do it right here.  The variable below keeps track
 * of the window that has grabbed input.
 */

static Window grabbingWindow;

/*
 * The variable below serves no purpose except to force the
 * locally-modified versions of several X library routines to
 * be included from the Sx library (even if they don't appear
 * to be needed).
 */

static Window (*dummy)() = XCreateWindow;

/*
 * Forward references to things defined later in this file:
 */

extern WindowStuff *	GetStuff();

/*
 *----------------------------------------------------------------------
 *
 * Sx_HandlerCreate --
 *
 *	This procedure sets up a handler for a particular class
 *	of events on a particular window.
 *
 * Results:
 *	The return value is a handler, which can be passed to
 *	Sx_HandlerDelete to delete the handler (the handler should
 *	be deleted before deleting w).
 *
 * Side effects:
 *	From now on, whenever an event of one of the types given
 *	by mask occurs for w and is processed by Sx_HandleEvent,
 *	proc will be called as shown below:
 *
 *	void
 *	proc(clientData, eventPtr)
 *	    ClientData clientData;
 *	    XEvent *eventPtr;
 *	{
 *	}
 *
 *	In the call to proc, clientData is the same value passed to
 *	Sx_HandlerCreate and eventPtr refers to X'es description of
 *	the event.  multiple handlers have been specified for a
 *	given event, then they are invoked in the order they were set
 *	up.
 *
 *	Another side effect of this procedure is that a call is made
 *	to XSelectInput to change the set of events selected for this
 *	window.
 *
 *----------------------------------------------------------------------
 */

Sx_EventHandler
Sx_HandlerCreate(w, mask, proc, clientData)
    Window w;			/* Window for which event was generated. */
    int mask;			/* Events for which proc should be called.
				 * May be any of the X event types, or
				 * SX_DESTROYED.  SX_DESTROYED events
				 * are generated whenever the window is
				 * destroyed.  These events only work for
				 * windows created by this client.
				 */
    void (*proc)();		/* Procedure to call for each selected event */
    ClientData clientData;	/* Arbitrary data to pass to proc. */
{
    WindowStuff *wsPtr;
    Handler *handlerPtr;
    register Handler *h2Ptr;
    int windowMask;

    wsPtr = GetStuff(w);

    if ((mask & SX_DESTROYED) && (wsPtr->parentPtr == NULL)) {
	Sx_Panic("Sx_HandlerCreate:  can't select SX_DESTROYED for \
windows not created by this client");
    }

    /*
     * Add a new handler to the end of the list of handlers for the
     * window.  While skimming through the list, compute the overall
     * event mask for the window, so we can pass this new value to
     * the X system.
     */

    handlerPtr = (Handler *) Mem_Alloc(sizeof(Handler));
    handlerPtr->w = w;
    handlerPtr->mask = mask;
    handlerPtr->proc = proc;
    handlerPtr->clientData = clientData;
    handlerPtr->nextPtr = NULL;
    windowMask = mask;
    if (wsPtr->firstHandler == NULL) {
	wsPtr->firstHandler = handlerPtr;
    } else {
	for (h2Ptr = wsPtr->firstHandler; ; h2Ptr = h2Ptr->nextPtr) {
	    windowMask |= h2Ptr->mask;
	    if (h2Ptr->nextPtr == NULL) {
		h2Ptr->nextPtr = handlerPtr;
		break;
	    }
	}
    }

    /*
     * Make sure that X knows about all the events of interest in this
     * window.
     */
    
    XSelectInput(w, windowMask);

    return (Sx_EventHandler) handlerPtr;
}

/*
 *----------------------------------------------------------------------
 *
 * Sx_HandlerDelete --
 *
 *	Remove a handler.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	After this call, the handler will never be invoked again (unless
 *	it is recreated with a call to Sx_HandlerCreate).
 *
 *----------------------------------------------------------------------
 */

void
Sx_HandlerDelete(handlerToken)
    Sx_EventHandler handlerToken;	/* Token for handler to delete;  must
					 * have been returned by previous call
					 * to Sx_HandlerCreate. */
{
    register Handler *handlerPtr = (Handler *) handlerToken;
    WindowStuff *wsPtr;
    register Handler *prevPtr;
    int mask;

    /*
     * Remove the handler from the chain associated with its
     * window, and compute the new mask of events desired from
     * the window.
     */

    wsPtr = (WindowStuff *) XLookUpAssoc(windowTable, handlerPtr->w);
    if (wsPtr == NULL) {
	return;
    }
    mask = 0;
    if (wsPtr->firstHandler == handlerPtr) {
	wsPtr->firstHandler = handlerPtr->nextPtr;
    } else for (prevPtr = wsPtr->firstHandler; prevPtr != NULL; prevPtr = prevPtr->nextPtr) {
	if (prevPtr->nextPtr == handlerPtr) {
	    prevPtr->nextPtr = handlerPtr->nextPtr;
	}
	mask |= prevPtr->mask;
    }

    /*
     * If Sx_HandleEvent is about to process this handler, tell it to
     * process the next one instead.
     */
    
    if (nextHandler == handlerPtr) {
	nextHandler = handlerPtr->nextPtr;
    }

    /*
     * Free the handler and tell X about the events we're now interested in.
     */
    
    XSelectInput(handlerPtr->w, mask);
    Mem_Free((char *) handlerPtr);
}

/*
 *----------------------------------------------------------------------
 *
 * Sx_HandleEvent --
 *
 *	This procedure processes an event, calling all the handlers
 *	that have been declared for that event.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	The only side effects are those taken by the handlers.
 *
 *----------------------------------------------------------------------
 */

void
Sx_HandleEvent(eventPtr)
    XEvent *eventPtr;		/* Event to be parceled off to handler(s). */
{
    register Handler *handlerPtr;
    register WindowStuff *wsPtr;
    register XExposeWindowEvent *eventReg;
    XKeyOrButtonEvent grabbedEvent;
    register int type;

    /*
     * First, see if window grabbing is in effect.  If so, translate the
     * event to the grabbed window's coordinates.  Ignore EnterWindow and
     * LeaveWindow events when grabbing is in effect.
     */
    
    if ((grabbingWindow != 0) && (grabbingWindow != eventPtr->window)) {
	if (eventPtr->type & (EnterWindow|LeaveWindow)) {
	    return;
	} else if (eventPtr->type
		& (ButtonPressed|ButtonReleased|MouseMoved)) {
	    int x, y;
	    grabbedEvent = *((XKeyOrButtonEvent *) eventPtr);
	    (void) XInterpretLocator(grabbingWindow, &x, &y,
		    &grabbedEvent.subwindow, grabbedEvent.location);
	    grabbedEvent.window = grabbingWindow;
	    grabbedEvent.x = x;
	    grabbedEvent.y = y;
	    eventPtr = (XEvent *) &grabbedEvent;
	}
    }

    /*
     * Locate information for the event's window.
     */

    if (windowTable == NULL) {
	return;
    }
    eventReg = (XExposeWindowEvent *) eventPtr;
    type = eventReg->type;
    wsPtr = (WindowStuff *) XLookUpAssoc(windowTable, eventReg->window);
    if (wsPtr == NULL) {
	return;
    }

    /*
     * Keep our information about the window's size up-to-date (needed
     * for ExposeRegion event translation, below).
     */
    
    if (eventReg->type == ExposeWindow) {
	wsPtr->width = eventReg->width;
	wsPtr->height = eventReg->height;
    }

    /*
     * MouseMoved events have to be translated to LeftDownMotion,
     * MiddleDownMotion, and RightDownMotion events.
     */
    
    if (type == MouseMoved) {
	if (eventReg->detail & LeftMask) {
	    type |= LeftDownMotion;
	}
	if (eventReg->detail & MiddleMask) {
	    type |= MiddleDownMotion;
	}
	if (eventReg->detail & RightMask) {
	    type |= RightDownMotion;
	}
    }

    /*
     * There's a potential interaction here with Sx_HandlerDelete.
     * Read the documentation for nextHandler.
     */

    for (handlerPtr = wsPtr->firstHandler; handlerPtr != NULL;
	    handlerPtr = nextHandler) {
	nextHandler = handlerPtr->nextPtr;
	if ((handlerPtr->mask & type) != 0) {
	    (*(handlerPtr->proc))(handlerPtr->clientData, eventReg);
	} else if ((type == ExposeRegion)
		&& (handlerPtr->mask & ExposeWindow)) {
	    XExposeEvent newEvent;

	    /*
	     * If the client requested ExposeWindow events but we
	     * get ExposeRegion events (because some other client
	     * requested them), we've got to translate.
	     */

	    newEvent.type = ExposeWindow;
	    newEvent.window = eventReg->window;
	    newEvent.detail = eventReg->detail;
	    newEvent.width = wsPtr->width;
	    newEvent.height = wsPtr->height;
	    newEvent.subwindow = eventReg->subwindow;
	    newEvent.x = newEvent.y = 0;
	    (*(handlerPtr->proc))(handlerPtr->clientData, &newEvent);
	}
    }
}

/*
 *----------------------------------------------------------------------
 *
 * Sx_GrabMouse --
 *
 *	Arrange for all future mouse-related events to be passed to
 *	a particular window, even if the mouse is over some other
 *	window.  Note:  clients should not ever call XGrabMouse
 *	directly if they use this routine.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	Until Sx_UngrabMouse is called, all mouse-related events
 *	arriving for any window other than w will be redirected
 *	to w.  If some other window had already grabbed the mouse,
 *	it is automatically un-grabbed.  Only events indicated by
 *	mask will be considered;  all others will be ignored until
 *	Sx_UngrabMouse is called.
 *
 *----------------------------------------------------------------------
 */

void
Sx_GrabMouse(w, cursor, mask)
    Window w;		/* Window to which to direct mouse events. */
    Cursor cursor;	/* New cursor to use while mouse is grabbed. */
    int mask;		/* Mask of events to be accepted during grab period. */
{
    if (grabbingWindow != 0) {
	Sx_UngrabMouse();
    }
    grabbingWindow = w;
    (void) XGrabMouse(w, cursor, mask);
}

/*
 *----------------------------------------------------------------------
 *
 * Sx_UngrabMouse --
 *
 *	Undoes the effects of a previous call to Sx_GrabMouse, allowing
 *	mouse-related events to be directed to windows in the normal
 *	fashion.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	Mouse grabbing is turned off.
 *
 *----------------------------------------------------------------------
 */

void
Sx_UngrabMouse()
{
    grabbingWindow = 0;
    XUngrabMouse();
}

/*
 *----------------------------------------------------------------------
 *
 * SxWindowCreated --
 *
 *	This procedure is called by the X library routines just after
 *	a new window is created.  This call records structural
 *	information about that window for use in handling SX_DESTROYED
 *	events.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	Information about the new window is added to windowTable.
 *
 *----------------------------------------------------------------------
 */

void
SxWindowCreated(parent, child)
    Window parent;		/* Parent of new window. */
    Window child;		/* Child of new window. */
{
    WindowStuff *parentPtr, *childPtr;

    parentPtr = GetStuff(parent);
    childPtr = GetStuff(child);
    childPtr->parentPtr = parentPtr;
    childPtr->nextPtr = parentPtr->childPtr;
    parentPtr->childPtr = childPtr;
}

/*
 *----------------------------------------------------------------------
 *
 * SxWindowDestroyed --
 *
 *	Called by the X library routines after a window is destroyed.
 *	It cleans up structures maintained here, plus calls all the
 *	clients that have selected SX_DESTROYED events.  It also
 *	assumes the destruction of all descendants of this window, and
 *	handles them in the same way.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	The WindowStuff information for this window (and all its
 *	descendants) gets deleted, and SX_DESTROYED event handlers
 *	get called.
 *
 *----------------------------------------------------------------------
 */

void
SxWindowDestroyed(w)
    Window w;			/* What was destroyed. */
				
{
    register WindowStuff *wsPtr;
    WindowStuff *siblingPtr;
    Handler *handler;
    XEvent event;

    /*
     * Get the entry for this window out of the hash table, and remove
     * it also from the list of children of its parent.
     */
    
    wsPtr = (WindowStuff *) XLookUpAssoc(windowTable, w);
    if (wsPtr == NULL) {
	return;
    }
    XDeleteAssoc(windowTable, w);
    if (wsPtr->parentPtr != NULL) {
	if (wsPtr->parentPtr->childPtr == wsPtr) {
	    wsPtr->parentPtr->childPtr = wsPtr->nextPtr;
	} else {
	    for (siblingPtr = wsPtr->parentPtr->childPtr;
		    siblingPtr != NULL; siblingPtr = siblingPtr->nextPtr) {
		if (siblingPtr->nextPtr == wsPtr) {
		    siblingPtr->nextPtr = wsPtr->nextPtr;
		    break;
		}
	    }
	}
    }

    /*
     * Call all handlers that want to know about deletions, and also
     * delete the handlers along the way.
     */

    event.type = SX_DESTROYED;
    event.window = w;
    event.pad_l1 = event.pad_l2 = 0;
    event.subwindow = 0;
    event.pad_l4 = 0;
    while (wsPtr->firstHandler != NULL) {
	handler = wsPtr->firstHandler;
	if (handler->mask & SX_DESTROYED) {
	    (*handler->proc)(handler->clientData, &event);
	}
	wsPtr->firstHandler = handler->nextPtr;
	Mem_Free((Address) handler);
    }

    /*
     * Recursively delete all of the children of this window.
     */
    
    while (wsPtr->childPtr != NULL) {
	SxWindowDestroyed(wsPtr->childPtr->w);
    }

    Mem_Free((Address) wsPtr);
}

/*
 *----------------------------------------------------------------------
 *
 * SxSubwindowsDestroyed --
 *
 *	Called as part of the XDestroySubwindows call.  It calls
 *	SxWindowDestroyed for each deleted subwindow.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	See SxWindowDestroyed.
 *
 *----------------------------------------------------------------------
 */

void
SxSubwindowsDestroyed(w)
    Window w;			/* Window whose subwindows are all dead now. */
{
    register WindowStuff * wsPtr;

    wsPtr = (WindowStuff *) XLookUpAssoc(windowTable, w);
    if (wsPtr == NULL) {
	return;
    }
    while (wsPtr->childPtr != NULL) {
	SxWindowDestroyed(wsPtr->childPtr->w);
    }
}

/*
 *----------------------------------------------------------------------
 *
 * GetStuff --
 *
 *	This procedure is called to create the dispatching association
 *	table if necessary, and also to create an entry in it for a
 *	particular window.
 *
 * Results:
 *	The return value is a pointer to the WindowStuff associated
 *	with w, which will be newly initialized if there wasn't
 *	previously an entry for w.
 *
 * Side effects:
 *	A new entry may be added to windowTable.
 *
 *----------------------------------------------------------------------
 */

static WindowStuff *
GetStuff(w)
    Window w;
{
    register WindowStuff *wsPtr;

    if (windowTable == NULL) {
	windowTable = XCreateAssocTable(32);
	dummy = dummy;		/* Just to keep lint from complaining. */
    }
    wsPtr = (WindowStuff *) XLookUpAssoc(windowTable, w);
    if (wsPtr == NULL) {
	wsPtr = (WindowStuff *) Mem_Alloc(sizeof(WindowStuff));
	wsPtr->w = w;
	wsPtr->width = wsPtr->height = 0;
	wsPtr->firstHandler = NULL;
	wsPtr->parentPtr = NULL;
	wsPtr->childPtr = NULL;
	wsPtr->nextPtr = NULL;
	XMakeAssoc(windowTable, w, (caddr_t) wsPtr);
    }
    return wsPtr;
}
