#ifndef lint
static char rcsid[] = "$Header: Event.c,v 1.63 88/02/26 20:25:28 swick Exp $";
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

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "IntrinsicI.h"
#include "Event.h"
#include <X11/StringDefs.h>
#include <X11/Shell.h>
extern void bzero();

EventMask _XtBuildEventMask(widget)
    Widget widget;
{
    _XtEventTable ev;
    EventMask	mask = 0;

    for (ev = widget->core.event_table; ev != NULL; ev = ev->next)
	if (ev->select) mask |= ev->mask;
    if (widget->core.widget_class->core_class.expose != NULL)
	mask |= ExposureMask;
    if (widget->core.widget_class->core_class.visible_interest) 
	mask |= VisibilityChangeMask;

    return mask;
}

static void RemoveEventHandler(widget, eventMask, other, proc, closure, raw)
    Widget	widget;
    EventMask   eventMask;
    Boolean	other;
    XtEventHandler proc;
    caddr_t	closure;
    Boolean	raw;
{
    XtEventRec *p, **pp;
    EventMask oldMask = _XtBuildEventMask(widget);

    pp = &widget->core.event_table;
    p = *pp;

    /* find it */
    while (p != NULL && (p->proc != proc || p->closure != closure)) {
	pp = &p->next;
	p = *pp;
    }
    if (p == NULL) return; /* couldn't find it */
    if (raw) p->raw = FALSE; else p->select = FALSE;
    if (p->raw || p->select) return;

    /* un-register it */
    p->mask &= ~eventMask;
    if (other) p->non_filter = FALSE;

    if (p->mask == 0 && !p->non_filter) {
	/* delete it entirely */
	*pp = p->next;
	XtFree((char *)p);
    }

    /* reset select mask if realized */
    if (XtIsRealized(widget)) {
	EventMask mask = _XtBuildEventMask(widget);

	if (oldMask != mask)
	    XSelectInput(XtDisplay(widget), XtWindow(widget), mask);
    }
}

static void AddEventHandler(widget, eventMask, other, proc, closure, raw)
    Widget	    widget;
    EventMask   eventMask;
    Boolean         other;
    XtEventHandler  proc;
    caddr_t	closure;
    Boolean	raw;
{
   register XtEventRec *p,**pp;
   EventMask oldMask;

   if (eventMask == 0 && other == FALSE) return;

   if (XtIsRealized(widget) && ! raw) oldMask = _XtBuildEventMask(widget);

   pp = & widget->core.event_table;
   p = *pp;
   while (p != NULL && (p->proc != proc || p->closure != closure)) {
         pp = &p->next;
         p = *pp;
   }

   if (p == NULL) {
	/* new proc to add to list */
	p = XtNew(XtEventRec);
	p->proc = proc;
	p->closure = closure;
	p->mask = eventMask;
	p->non_filter = other;
	p->select = ! raw;
	p->raw = raw;

	p->next = widget->core.event_table;
	widget->core.event_table = p;

    } else {
	/* update existing proc */
	p->mask |= eventMask;
	p->non_filter = p->non_filter || other;
	p->select |= ! raw;
	p->raw |= raw;
    }

    if (XtIsRealized(widget) && ! raw) {
	EventMask mask = _XtBuildEventMask(widget);

	if (oldMask != mask)
	    XSelectInput(XtDisplay(widget), XtWindow(widget), mask);
    }

}


void XtRemoveEventHandler(widget, eventMask, other, proc, closure)
    Widget	widget;
    EventMask   eventMask;
    Boolean	other;
    XtEventHandler proc;
    caddr_t	closure;
{
    RemoveEventHandler(widget, eventMask, other, proc, closure, FALSE);
}


void XtAddEventHandler(widget, eventMask, other, proc, closure)
    Widget	    widget;
    EventMask   eventMask;
    Boolean         other;
    XtEventHandler  proc;
    caddr_t	closure;
{
    AddEventHandler(widget, eventMask, other, proc, closure, FALSE);
}


void XtRemoveRawEventHandler(widget, eventMask, other, proc, closure)
    Widget	widget;
    EventMask   eventMask;
    Boolean	other;
    XtEventHandler proc;
    caddr_t	closure;
{
    RemoveEventHandler(widget, eventMask, other, proc, closure, TRUE);
}


void XtAddRawEventHandler(widget, eventMask, other, proc, closure)
    Widget	    widget;
    EventMask   eventMask;
    Boolean         other;
    XtEventHandler  proc;
    caddr_t	closure;
{
    AddEventHandler(widget, eventMask, other, proc, closure, FALSE);
}


typedef struct _HashRec *HashPtr;

typedef struct _HashRec {
    Display	*display;
    Window	window;
    Widget	widget;
    HashPtr	next;
} HashRec;

typedef struct {
    unsigned int	size;
    unsigned int	count;
    HashPtr		entries[1];
} HashTableRec, *HashTable;

static HashTable table = NULL;

static void ExpandTable();

void _XtRegisterWindow(window, widget)
    Window window;
    Widget widget;
{
    register HashPtr hp, *hpp;

    if ((table->count + (table->count / 4)) >= table->size) ExpandTable();

    hpp = &table->entries[(unsigned int)window & (table->size-1)];
    hp = *hpp;

    while (hp != NULL) {
        if (hp->window == window) {
	    if (hp->widget != hp->widget)
		XtWarning("Attempt to change already registered window.\n");
	    return;
	}
        hpp = &hp->next;
	hp = *hpp;
    }

    hp = *hpp = XtNew(HashRec);
    hp->window = window;
    hp->widget = widget;
    hp->next = NULL;
    table->count++;
}


void _XtUnregisterWindow(window, widget)
    Window window;
    Widget widget;
{
    HashPtr hp, *hpp;

    hpp = &table->entries[(unsigned int)window  & (table->size-1)];
    hp = *hpp;

    while (hp != NULL) {
        if (hp->window == window) {
	    if (hp->widget != widget) {
		XtWarning("Unregister-window does not match widget.\n");
                return;
                }
             else /* found entry to delete */
                  (*hpp) = hp->next;
                  XtFree((char*)hp);
                  table->count--;
                  return;
	}
        hpp = &hp->next;
	hp = *hpp;
    }
    
}

static void ExpandTable()
{
    HashTable	oldTable = table;
    unsigned int i;

    i = oldTable->size * 2;
    table = (HashTable) XtMalloc(
	(unsigned) sizeof(HashTableRec)+i*sizeof(HashRec));
    table->size = i;
    table->count = oldTable->count;
    for (i = 0; i<oldTable->size; i++) {
	HashPtr hp;
	hp = oldTable->entries[i];
	while (hp != NULL) {
	    HashPtr temp = hp;
	    _XtRegisterWindow(hp->window, hp->widget);
	    hp = hp->next;
	    XtFree((char *) temp);
	}
    }
    XtFree((char *)oldTable);
}


/*ARGSUSED*/
Widget XtWindowToWidget(display, window)
    Display *display;
    Window window;
{
    register HashPtr hp;

/* display is ignored for now; will be used when multiple displays
   are supported */

    for (
        hp = table->entries[(unsigned int)window & (table->size-1)];
        hp != NULL;
	hp = hp->next)
	if (hp->window == window) return hp->widget;

    return NULL;
}

static void InitializeHash()
{
    int size = sizeof(HashTableRec)+1024*sizeof(HashPtr);

    table = (HashTable) XtMalloc((unsigned) size);
    bzero((char *) table, size);

    table->size = 1024;
    table->count = 0;
}


XtAddExposureToRegion(event, region)
    XEvent   *event;
    Region   region;
{
    XRectangle rect;

    switch (event->type) {
	case Expose:
		rect.x = event->xexpose.x;
		rect.y = event->xexpose.y;
		rect.width = event->xexpose.width;
		rect.height = event->xexpose.height;
		break;
	case GraphicsExpose:
		rect.x = event->xgraphicsexpose.x;
		rect.y = event->xgraphicsexpose.y;
		rect.width = event->xgraphicsexpose.width;
		rect.height = event->xgraphicsexpose.height;
		break;
	default:
		return;
    }

    XUnionRectWithRegion(&rect, region, region);
}


/* %%% Multiple display support will require keeping an exposeRegion
 *     for each open display
 */

static Region nullRegion;
static Region exposeRegion;

static void DispatchEvent(event, widget, mask)
    register XEvent    *event;
    Widget    widget;
    unsigned long mask;
{
    XtEventRec *p;   
    XtEventHandler proc[100];
    caddr_t closure[100];
    int numprocs, i;
    XEvent nextEvent;

    if (mask == ExposureMask) {
	XtExposeProc expose = widget->core.widget_class->core_class.expose;
	if (expose != NULL) {
	    if (!widget->core.widget_class->core_class.compress_exposure) {
		(*expose)(widget, event, (Region)NULL);
	    }
	    else {
		XtAddExposureToRegion(event, exposeRegion);
		if (event->xexpose.count == 0) {
		    /* Patch event to have the new bounding box.  Unless
		       someone's goofed, it can only be an Expose event */
		    XRectangle rect;
		    XClipBox(exposeRegion, &rect);
		    event->xexpose.x = rect.x;
		    event->xexpose.y = rect.y;
		    event->xexpose.width = rect.width;
		    event->xexpose.height = rect.height;
		    (*expose)(widget, event, exposeRegion);
		    XIntersectRegion( nullRegion, exposeRegion, exposeRegion );
		}
	    }
	}
    }

    if (mask == EnterWindowMask &&
	    widget->core.widget_class->core_class.compress_enterleave) {
	if (XPending(event->xcrossing.display)) {
	    XPeekEvent(event->xcrossing.display, &nextEvent);
	    if (nextEvent.type == LeaveNotify &&
		    event->xcrossing.window == nextEvent.xcrossing.window &&
		    event->xcrossing.subwindow == nextEvent.xcrossing.subwindow) {
		/* skip the enter/leave pair */
		XNextEvent(event->xcrossing.display, &nextEvent);
		return;
	    }
	}
    }

    if (event->type == MotionNotify &&
	    widget->core.widget_class->core_class.compress_motion) {
	while (XPending(event->xmotion.display)) {
	    XPeekEvent(event->xmotion.display, &nextEvent);
	    if (nextEvent.type == MotionNotify &&
		    event->xmotion.window == nextEvent.xmotion.window &&
		    event->xmotion.subwindow == nextEvent.xmotion.subwindow) {
		/* replace the current event with the next one */
		XNextEvent(event->xmotion.display, event);
	    } else break;
	}
    }

    if (mask == VisibilityChangeMask &&
            ! widget->core.widget_class->core_class.visible_interest) return;

    /* Have to copy the procs into an array, because calling one of them */
    /* might call XtRemoveEventHandler, which would break our linked list.*/

    numprocs = 0;

    for (p=widget->core.event_table; p != NULL; p = p->next) {
	if ((mask & p->mask) != 0 || (mask == 0 && p->non_filter)) {
	    proc[numprocs] = p->proc;
	    closure[numprocs++] = p->closure;
	}
    }

    for (i=0 ; i<numprocs ; i++) (*(proc[i]))(widget, closure[i], event);
}


typedef enum {pass, ignore, remap} GrabType;

static void ConvertTypeToMask (eventType, mask, grabType)
    int		eventType;
    EventMask   *mask;
    GrabType    *grabType;
{

static struct {
    EventMask   mask;
    GrabType    grabType;
} masks[] = {
    {0,				pass},      /* shouldn't see 0  */
    {0,				pass},      /* shouldn't see 1  */
    {KeyPressMask,		remap},     /* KeyPress		*/
    {KeyReleaseMask,		remap},     /* KeyRelease       */
    {ButtonPressMask,		remap},     /* ButtonPress      */
    {ButtonReleaseMask,		remap},     /* ButtonRelease    */
    {PointerMotionMask
     | Button1MotionMask
     | Button2MotionMask
     | Button3MotionMask
     | Button4MotionMask
     | Button5MotionMask
     | ButtonMotionMask,	ignore},    /* MotionNotify	*/
    {EnterWindowMask,		ignore},    /* EnterNotify	*/
    {LeaveWindowMask,		ignore},    /* LeaveNotify	*/
    {FocusChangeMask,		pass},      /* FocusIn		*/
    {FocusChangeMask,		pass},      /* FocusOut		*/
    {KeymapStateMask,		pass},      /* KeymapNotify	*/
    {ExposureMask,		pass},      /* Expose		*/
    {0,				pass},      /* GraphicsExpose   */
    {0,				pass},      /* NoExpose		*/
    {VisibilityChangeMask,      pass},      /* VisibilityNotify */
    {SubstructureNotifyMask,    pass},      /* CreateNotify	*/
    {StructureNotifyMask
     | SubstructureNotifyMask,  pass},      /* DestroyNotify	*/
    {StructureNotifyMask
     | SubstructureNotifyMask,  pass},      /* UnmapNotify	*/
    {StructureNotifyMask
     | SubstructureNotifyMask,  pass},      /* MapNotify	*/
    {SubstructureRedirectMask,  pass},      /* MapRequest	*/
    {StructureNotifyMask
     | SubstructureNotifyMask,  pass},      /* ReparentNotify   */
    {StructureNotifyMask
     | SubstructureNotifyMask,  pass},      /* ConfigureNotify  */
    {SubstructureRedirectMask,  pass},      /* ConfigureRequest */
    {StructureNotifyMask
     | SubstructureNotifyMask,  pass},      /* GravityNotify	*/
    {ResizeRedirectMask,	pass},      /* ResizeRequest	*/
    {StructureNotifyMask
     | SubstructureNotifyMask,  pass},      /* CirculateNotify	*/
    {SubstructureRedirectMask,  pass},      /* CirculateRequest */
    {PropertyChangeMask,	pass},      /* PropertyNotify   */
    {0,				pass},      /* SelectionClear   */
    {0,				pass},      /* SelectionRequest */
    {0,				pass},      /* SelectionNotify  */
    {ColormapChangeMask,	pass},      /* ColormapNotify   */
    {0,				pass},      /* ClientMessage	*/
    {0,				pass},      /* MappingNotify ???*/
  };

    eventType &= 0x7f;	/* Events sent with XSendEvent have high bit set. */
    (*mask)      = masks[eventType].mask;
    (*grabType)  = masks[eventType].grabType;
};


static GrabRec* OnGrabList (widget)
    register Widget widget;

{
    register GrabRec* gl;
    for (; widget != NULL; widget = (Widget)widget->core.parent)
	for (gl = grabList; gl != NULL; gl = gl->next) {
	    if (gl->widget == widget) return (gl);
	    if (gl->exclusive) break;
	}
    return (NULL);
}

static Boolean OnlyKeyboardGrabs()
{
    register GrabRec* gl;
    for (gl = grabList; gl != NULL; gl = gl->next) {
	if (gl->keyboard_focus != (Widget)NULL) return False;
    }
    return True;
}


void XtDispatchEvent (event)
    XEvent  *event;
{
    register    Widget widget;
    EventMask   mask;
    GrabType    grabType;
    register    GrabList gl;

#define ShouldDispatch \
    (widget->core.sensitive && widget->core.ancestor_sensitive)

    widget = XtWindowToWidget (event->xany.display, event->xany.window);

    /* Lint complains about &grabType not matching the declaration.
       Don't bother trying to fix it, it won't work */

    ConvertTypeToMask(event->xany.type, &mask, &grabType);

    if (widget == NULL) {
	if (grabType != remap) return;
	/* event occurred in a non-widget window, but we've promised
	   also to dispatch it to the nearest spring_loaded widget */
	for (gl = grabList; gl != NULL; gl = gl->next) {
	    if (gl->spring_loaded) {
		DispatchEvent(event, gl->widget, mask);
		return;
	    }
	}
    }

    if (grabType == pass) {
	DispatchEvent(event, widget, mask);

    } else if (grabList == NULL) {
	if ShouldDispatch {
	    DispatchEvent(event, widget, mask);
	}

    } else if (grabType == remap) {
        if ((mask & (ButtonPressMask|ButtonReleaseMask) && OnlyKeyboardGrabs)
	    || (gl = OnGrabList(widget))) {
	    if (mask & (KeyPressMask|KeyReleaseMask) &&
		 gl->keyboard_focus != NULL) {
		/* replace with focus widget iff source widget
		   is not a child of the focus widget */
		register Widget w;
		for (w = widget; w != gl->keyboard_focus && w != gl->widget;)
		     w = XtParent(w);
		if (w != gl->keyboard_focus)
		    widget = gl->keyboard_focus;
	    }
	    if ShouldDispatch {
		DispatchEvent(event, widget, mask);
	    }
	}
	/* Also dispatch to nearest accessible spring_loaded */
        /* Dispatching the event may invoke a proc that popdowns all
           the popups and un-registers all the grabs from the grabList,
           so check for something still on the grabList. */
 	gl = grabList;
	while (gl!= NULL) {
	    if (gl->spring_loaded) {
		if (gl->widget != widget) {
		    DispatchEvent(event, gl->widget, mask);
		}
		break;
	    }
	    if (gl->exclusive) {
		break;
	    }
	    gl = gl->next;
	} 
    } else if (OnlyKeyboardGrabs || OnGrabList(widget)) {
	if ShouldDispatch {
	    DispatchEvent(event, widget, mask);
	}
    }
}

static void RemoveGrab();

/* ARGSUSED */
static void GrabDestroyCallback(widget, closure, call_data)
    Widget  widget;
    caddr_t closure;		/* Widget or NULL */
    caddr_t call_data;
{
    /* Remove widget from grab list if it destroyed */
    if (closure != NULL)	/* removing a keyboard grab on some parent? */
	RemoveGrab( (Widget)closure, True );
    else
	RemoveGrab( widget, False );
}

static void AddGrab(widget, exclusive, spring_loaded, keyboard_focus)
    Widget  widget;
    Boolean exclusive;
    Boolean spring_loaded;
    Widget  keyboard_focus;
{
    register GrabList    gl;

    if (freeGrabs == NULL)
	gl	      = /* XtNew(GrabRec) */
			(GrabRec*)Xpermalloc((unsigned int)sizeof(GrabRec));
    else {
	gl = freeGrabs;
	freeGrabs = freeGrabs->next;
    }
    gl->next	      = grabList;
    grabList	      = gl;
    gl->widget        = widget;
    gl->exclusive     = exclusive;
    gl->spring_loaded = spring_loaded;
    gl->keyboard_focus= keyboard_focus;

    XtAddCallback(
		  (keyboard_focus != NULL) ? keyboard_focus : widget,
		  XtNdestroyCallback, GrabDestroyCallback,
		  (caddr_t)(keyboard_focus != NULL) ? widget : NULL
		 );
}

/* add a grab record to the list, or replace the focus widget in an
   existing grab record.  Returns True if the action was not a no-op.
 */
static Boolean InsertKeyboardGrab(widget, keyboard_focus)
    Widget  widget;
    Widget  keyboard_focus;
{
    register GrabRec *gl;
    GrabRec* ge;
    Boolean found = False;

    if (grabList == NULL) {
	AddGrab( widget, False, False, keyboard_focus );
	return;
    }
    /* look for a keyboard grab entry for the same parent; if none,
       reposition this entry after any non-focus entries for the same tree */
    for (; widget != NULL && !found; widget = widget->core.parent) {
	for (gl = grabList; gl != NULL; gl = gl->next)
	    if (gl->widget == widget) {
		found = True;
		break;
	    }
    }
    if (found)
	if (gl->keyboard_focus != NULL) 
	    if (gl->keyboard_focus == keyboard_focus)
		return False;
	    else
		gl->keyboard_focus = keyboard_focus;
	else {
	    AddGrab( widget, False, False, keyboard_focus );
	    ge = grabList;
	    grabList = grabList->next;
	    ge->next = gl->next;
	    gl->next = ge;
	}
    else { /* insert a new grab at end of list */
	AddGrab( widget, False, False, keyboard_focus );
	ge = grabList;
	grabList = grabList->next;
	ge->next = NULL;
	if (grabList == NULL)
	    grabList = ge;
	else {
	    for (gl = grabList; gl->next != NULL; gl = gl->next);
	    gl->next = ge;
	}
    }
    return True;
}

void XtAddGrab(widget, exclusive, spring_loaded)
    Widget  widget;
    Boolean exclusive;
    Boolean spring_loaded;
{
    if (spring_loaded && !exclusive) {
	XtWarning("XtAddGrab requires exclusive grab if spring_loaded is TRUE");
	exclusive = TRUE;
    }

    AddGrab(widget, exclusive, spring_loaded, (Widget)NULL);
}


static void RemoveGrab(widget, keyboard_focus)
    Widget widget;
    Boolean keyboard_focus;
{
    register GrabList gl, prev, next;
    Widget focus_widget;

    for (prev = NULL, gl = grabList; gl != NULL; prev = gl, gl = gl->next) {
	if (gl->widget == widget)
	    if (!keyboard_focus || gl->keyboard_focus != NULL)
		break;
    }

    if (gl == NULL) {
	if (!keyboard_focus)
	    XtWarning("XtRemoveGrab asked to remove a widget not on the grab list");
	return;
    }

    focus_widget = gl->keyboard_focus;
    while (gl) {
	if (gl->widget == widget) {
	    if (prev)
		prev->next = next = gl->next;
	    else
		grabList = next = gl->next;
	    gl->next = freeGrabs;
	    freeGrabs = gl;
	    gl = next;
	}
	else {
	    prev = gl;
	    gl = gl->next;
	}
	if (keyboard_focus) break; /* don't remove all grabs (yet) */
    }
    XtRemoveCallback(
		     keyboard_focus ? focus_widget : widget,
		     XtNdestroyCallback, GrabDestroyCallback,
		     (caddr_t)(keyboard_focus ? widget : NULL)
		    );
}


void XtRemoveGrab(widget)
    Widget  widget;
{
    RemoveGrab(widget, False);
}


void XtMainLoop()
{
    XEvent event;

    for (;;) {
    	XtNextEvent(&event);
	XtDispatchEvent(&event);
    }
}


void _XtEventInitialize()
{
    grabList = NULL;
    freeGrabs = NULL;
    DestroyList = NULL;
    nullRegion = XCreateRegion();
    exposeRegion = XCreateRegion();
    InitializeHash();
}


/* ARGSUSED */
static void ForwardEvent(widget, client_data, event)
    Widget widget;
    caddr_t client_data;
    XEvent *event;
{
    /* this shouldn't have been necessary, as the keyboard grab will cause
       needed, however, a protocol bug causes us to believe we lost the
       focus during a grab */
    EventMask mask;
    GrabType grabType;

    if ShouldDispatch {
	ConvertTypeToMask(event->xany.type, &mask, &grabType);
	DispatchEvent(event, (Widget)client_data, mask);
    }
}


/* ARGSUSED */
static void HandleFocus(widget, client_data, event)
    Widget widget;
    caddr_t client_data;	/* child who wants focus */
    XEvent *event;
{
    enum {add, remove} which;
    Widget descendant = (Widget)client_data;
    switch( event->type ) {
      case EnterNotify:
      case LeaveNotify:
		if (!event->xcrossing.focus ||
/* a protocol bug causes us to not receive notification that we've lost
   the focus when the pointer leaves during a grab
		    event->xcrossing.mode != NotifyNormal ||
*/
		    event->xcrossing.detail == NotifyInferior)
		    return;
		if (event->type == EnterNotify) which = add;
		else				which = remove;
		break;
      case FocusIn:
      case FocusOut:
		if ((event->xfocus.mode != NotifyNormal &&
		     event->xfocus.mode != NotifyWhileGrabbed) ||
		    event->xfocus.detail == NotifyInferior)
		    return;
		if (event->type == FocusIn) which = add;
		else			   which = remove;
    }
    if (which == add)
	InsertKeyboardGrab( widget, descendant );
    else
	RemoveGrab( widget, True );

    SendFocusNotify( descendant, (which == add) ? FocusIn : FocusOut );
}


static void AddForwardingHandler(widget, descendant)
    Widget widget, descendant;
{
    Window win, root, child;
    int root_x, root_y, win_x, win_y;
    unsigned int mask;
    EventMask eventMask;

    eventMask = _XtBuildEventMask(descendant);
    eventMask &= KeyPressMask | KeyReleaseMask;
    if (eventMask != 0) {
	XtAddEventHandler
	    (
	     widget,
	     eventMask,
	     False,
	     ForwardEvent,
	     (caddr_t)descendant
	    );
	if (widget != XtParent(descendant) ||
	    !XtIsSubclass(widget, shellWidgetClass)) {
	    /* is the pointer already inside? */
	    XQueryPointer( XtDisplay(widget), XtWindow(widget),
			   &root, &child, &root_x, &root_y,
			   &win_x, &win_y, &mask );
	    if (win_x >= 0 && win_x < widget->core.width &&
		win_y >= 0 && win_y < widget->core.height) {
		if (InsertKeyboardGrab( widget, descendant ))
		    SendFocusNotify( descendant, FocusIn );
	    }
	}
    }
}

/* ARGSUSED */
static void QueryEventMask(widget, client_data, event)
    Widget widget;		/* child who gets focus */
    caddr_t client_data;	/* ancestor giving it */
    XEvent *event;
{
    if (event->type == MapNotify) {
	AddForwardingHandler((Widget)client_data, widget);
	XtRemoveEventHandler( widget, XtAllEvents, True,
			      QueryEventMask, client_data );
    }
}


void XtSetKeyboardFocus(widget, descendant)
    Widget widget;
    Widget descendant;
{
    if (descendant == (Widget)None) {
	XtRemoveEventHandler(widget, XtAllEvents, True, HandleFocus, NULL);
	RemoveGrab( widget, True );
    }
    else {
	XtAddEventHandler
	    (
	     widget,
	     /* shells are always occluded by their children */
	     FocusChangeMask |
		 ((widget == XtParent(descendant) &&
		   XtIsSubclass(widget, shellWidgetClass))
		      ? 0 : EnterWindowMask | LeaveWindowMask),
	     False,
	     HandleFocus,
	     (caddr_t)descendant
	    );
	if (XtIsRealized(descendant)) {	/* are his translations installed? */
	    AddForwardingHandler(widget, descendant);
	}
	else {			/* we'll have to wait 'till later */
	    XtAddEventHandler
		(
		 descendant,
		 StructureNotifyMask,
		 False,
		 QueryEventMask,
		 (caddr_t)widget
		);
	}
    }
}


static SendFocusNotify(child, type)
    Widget child;
    int type;
{
    XEvent event;
    EventMask mask;
    GrabType grabType;

    if (_XtBuildEventMask(child) && FocusChangeMask) {
	event.xfocus.serial = LastKnownRequestProcessed(XtDisplay(child));
	event.xfocus.send_event = True;
	event.xfocus.display = XtDisplay(child);
	event.xfocus.window = XtWindow(child);
	event.xfocus.type = type;
	event.xfocus.mode = NotifyNormal;
	event.xfocus.detail = NotifyAncestor;
	ConvertTypeToMask(type, &mask, &grabType);
	DispatchEvent(&event, child, mask);
    }
}
