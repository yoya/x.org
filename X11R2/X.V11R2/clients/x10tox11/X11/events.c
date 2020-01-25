/*
 * Copyright 1987 by Tektronix Inc, Beaverton, Oregon.
 *
 *                         All Rights Reserved
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted,
 * provided that the above copyright notice appear in all copies and that
 * both that copyright notice and this permission notice appear in
 * supporting documentation, and that the name of Tektronix not be used
 * in advertising or publicity pertaining to distribution of the software
 * without specific, written prior permission.
 *
 *
 * TEKTRONIX DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
 * ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
 * TEKTRONIX BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
 * ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
 * ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 */
/* 
 * $Header: events.c,v 4.1 88/02/14 17:03:40 toddb Exp $ 
 * $Locker:  $ 
 */
#include "X11/Xlib.h"
#include "X11/keysym.h"
#include "resource.h"
#include "lk201keys.h"

extern u_char	LockToggleMode;
extern u_char	LockStateOn;

#define	EventOK			1
#define	EventNotSelected	2
#define	EventWindowMissing	3
#define	EventSubWindowMissing	4
#define	EventSelectedByChild	5
#define	EventLockToggleMode	6
#define	EventUnImplemented	7
#define	EventHasNoX10Equiv	8
#define	EventModeField		9
#define	EventNoChange		10

/*
 * Standard local macro for looking up an event window.
 */
#define	GetEventWindowOrReturn(client, win, wp, interestMask)		\
	wp = WinLookup(client, win, False);				\
	if (wp == NULL)							\
		return (EventWindowMissing);				\
	if ((wp->mask & (interestMask)) == 0)				\
		return (EventNotSelected);


CheckX11events(client, reqcode)
	int	client;
	int	reqcode;
{
	Display *dpy = clist[ client ]->dpy;

	if (QLength(dpy)) {
		debug(reqcode, ("...\n"));
		X11events( ConnectionNumber(dpy) );
		debug(reqcode, (" ..."));
	}
}

static WinInfoptr LookupEventAncestor(wp, mask)
	WinInfoptr	wp;
	int		mask;
{
	WinInfoptr	parent;

	/*
	 * Figure out the which ancestor is interested in this event.
	 */
	for (parent = wp; parent; parent = parent->parent)
		if (parent->mask & mask)
			break;;
	return(parent);
}

static GenerateNoExposeEvent(client, noexpev, rep)
	int	client;
	XNoExposeEvent	*noexpev;
	X10Rep	*rep;
{
	WinInfoptr	wp, parent;
	Window		eventWindow, subWindow;

	GetEventWindowOrReturn(client, noexpev->drawable, wp, X10ExposeCopy);

	parent = LookupEventAncestor(wp, X10ExposeCopy);
	if (parent == NULL)
		return (EventNotSelected);
	if (parent == wp) {
		eventWindow = wp->id;
		subWindow = 0;
	} else {
		eventWindow = parent->id;
		subWindow = wp->id;
	}

	rep->code = X10ExposeCopy;
	rep->param.l[0] = eventWindow;
	rep->param.l[3] = subWindow;
	return (EventOK);
}

static GenerateGraphicsExposeEvent(client, grexpev, rep)
	int	client;
	XGraphicsExposeEvent	*grexpev;
	X10Rep	*rep;
{
	WinInfoptr	wp, parent;
	Window		eventWindow, subWindow;

	GetEventWindowOrReturn(client, grexpev->drawable, wp, X10ExposeCopy);

	parent = LookupEventAncestor(wp, X10ExposeCopy);
	if (parent == NULL)
		return (EventNotSelected);
	if (parent == wp) {
		eventWindow = wp->id;
		subWindow = 0;
	} else {
		eventWindow = parent->id;
		subWindow = wp->id;
	}

	rep->code = X10ExposeRegion;
	rep->param.l[0] = eventWindow;
	rep->param.s[3] = X10ExposeCopy;
	rep->param.s[4] = grexpev->width;
	rep->param.s[5] = grexpev->height;
	rep->param.l[3] = subWindow;
	rep->param.s[9] = grexpev->x;
	rep->param.s[8] = grexpev->y;

	/*
	 * At the end of a expose sequence send
	 * an ExposeCopy.
	 */
	if (grexpev->count == 0) {
		XEvent	pe;

		pe.xnoexpose.type = NoExpose;
		pe.xnoexpose.display = grexpev->display;
		pe.xnoexpose.drawable = grexpev->drawable;
		pe.xnoexpose.major_code = grexpev->major_code;
		pe.xnoexpose.minor_code = grexpev->minor_code;
		UnGetXEvent(client, &pe);
	}
	return (EventOK);
}

static GenerateConfigureExposeEvent(client, configev, rep)
	int	client;
	XConfigureEvent	*configev;
	X10Rep	*rep;
{
	WinInfoptr	wp;
	Window		eventWindow = configev->window,
			subWindow;

	GetEventWindowOrReturn(client, eventWindow, wp,
		X10ExposeRegion | X10ExposeWindow);

	if (configev->window == configev->event)
		subWindow = 0;
	else {
		subWindow = configev->window;
		wp = WinLookup(client, subWindow, False);
		if (wp == NULL)
			return (EventSubWindowMissing);
		if (wp->mask & X10ExposeWindow)
			return (EventSelectedByChild);
	}

	wp->x = configev->x;
	wp->y = configev->y;
	if (! wp->configured
	    && wp->width == configev->width
	    && wp->height == configev->height)
		return (EventNoChange);

	wp->width = configev->width;
	wp->height = configev->height;
	wp->configured--;

	rep->code = X10ExposeWindow;
	rep->param.l[0] = eventWindow;
	rep->param.s[3] = 0; /* detail */
	rep->param.s[4] = wp->width;
	rep->param.s[5] = wp->height;
	rep->param.l[3] = subWindow;
	rep->param.s[9] = wp->x; /* x */
	rep->param.s[8] = wp->y; /* y */

	return (EventOK);
}

static GenerateUnmapEvent(client, unmapev, rep)
	int	client;
	XUnmapEvent	*unmapev;
	X10Rep	*rep;
{
	WinInfoptr	wp;
	Window		eventWindow = unmapev->window,
			subWindow;

	GetEventWindowOrReturn(client, eventWindow, wp, X10UnmapWindow);

	if (unmapev->window == unmapev->event) {
		subWindow = 0;
		wp->mapped = False;
	} 
	else {
		subWindow = unmapev->window;
		wp = WinLookup(client, subWindow, False);
		if (wp == NULL)
			return (EventSubWindowMissing);
		wp->mapped = False;

		if (wp->mask & X10UnmapWindow)
			return (EventSelectedByChild);
	}

	rep->code = X10UnmapWindow;
	rep->param.l[0] = eventWindow;
	rep->param.l[3] = subWindow;
	return (EventOK);
}

/*
 * the UnmapTransparent request will sometimes gobble up unmap events...
 * those events must still be processed.
 */
ProcessUnmapEvent(client, unmapev)
	int	client;
	XUnmapEvent	*unmapev;
{
	X10Rep	dummy;

	(void) GenerateUnmapEvent(client, unmapev, &dummy);
}

static AllExposuresAfterMapEvent(dpy, ev, wp)
	Display		*dpy;
	XEvent		*ev;
	WinInfoptr	wp;
{
	if (ev->type == Expose) {
		if (ev->xexpose.window == wp->id)
			return (True);
	}
	return (False);
}

static GenerateMapExposeEvent(client, mapev, rep)
	int	client;
	XMapEvent	*mapev;
	X10Rep	*rep;
{
	WinInfoptr	wp, parent;
	Window		eventWindow = mapev->window,
			subWindow;
	Display		*dpy;
	XEvent		next;
	int		found;

	GetEventWindowOrReturn(client, eventWindow, wp, X10ExposeWindow);

	if (mapev->window == mapev->event) {
		subWindow = 0;
		if (wp->mapped)
			return (EventAlreadyMapped);
		wp->mapped = True;
	} 
	else {
		subWindow = mapev->window;
		wp = WinLookup(client, subWindow, False);
		if (wp == NULL)
			return (EventSubWindowMissing);
		if (wp->mapped)
			return (EventAlreadyMapped);
		wp->mapped = True;
		if (wp->mask & X10ExposeWindow)
			return (EventSelectedByChild);
	}

	/*
	 * Don't generate any event if An ancestor of ours is not
	 * mapped.
	 */
	for (parent = wp; parent; parent = parent->parent)
		if (! parent->mapped)
			return(EventParentNotMapped);

	/*
	 * If there are exposures in the queue for the same
	 * window, then delete them.  The protocol says that
	 * a MapWindow request causes a single exposeWindow,
	 * and no ExposeRegions.
	 */
	dpy = clist[ client ]->dpy;
	found = 0;
	XSync(dpy, False); /* awful */
	while (XCheckIfEvent(dpy, &next, AllExposuresAfterMapEvent, wp)) {
		ShowX11AndX10Event(client, &next, NULL, NULL);
	}

	rep->code = X10ExposeWindow;
	rep->param.l[0] = eventWindow;
	rep->param.s[3] = 0; /* detail */
	rep->param.s[4] = wp->width;
	rep->param.s[5] = wp->height;
	rep->param.l[3] = subWindow;
	rep->param.s[9] = 0; /* x */
	rep->param.s[8] = 0; /* y */

	return (EventOK);
}

static GenerateExposeEvent(client, expev, rep)
	int	client;
	XExposeEvent	*expev;
	X10Rep	*rep;
{
	WinInfoptr		wp, parent;
	Window			eventWindow, subWindow;
	long			code;
	int			mask;
	XRectangle		bounds;

	wp = WinLookup(client, expev->window, False);
	if (wp == NULL)
		return (EventWindowMissing);

	parent = LookupEventAncestor(wp, X10ExposeRegion | X10ExposeWindow);
	if (parent == NULL)
		return (EventNotSelected);
	if (parent == wp) {
		mask = wp->mask;
		eventWindow = wp->id;
		subWindow = 0;
	} else {
		mask = parent->mask;
		eventWindow = parent->id;
		subWindow = wp->id;
	}

	/*
	 * Must be one of these, if we have gotten here...
	 * Discover which kind of event to generate
	 */
	if ((mask & (X10ExposeWindow | X10ExposeRegion)) == X10ExposeWindow)
		code = X10ExposeWindow;
	else if ((mask & X10ExposeWindow)
	    && wp->width == expev->width
	    && wp->height == expev->height
	    && wp->x == 0
	    && wp->y == 0)
		code = X10ExposeWindow;
	else
		code = X10ExposeRegion;

	if (code == X10ExposeWindow && expev->count)
		return(EventNotSelected);

	/*
	 * Set the bounds for these exposure events.
	 */
	if (code == X10ExposeWindow) {
		bounds.width = wp->width;
		bounds.height = wp->height;
		bounds.x = 0;
		bounds.y = 0;
	} else {
		bounds.width = expev->width;
		bounds.height = expev->height;
		bounds.x = expev->x;
		bounds.y = expev->y;
	}

	rep->code = code;
	rep->param.l[0] = eventWindow;
	rep->param.s[3] = 0;	/* always 0 */
	rep->param.l[3] = subWindow;
	rep->param.s[4] = bounds.width;
	rep->param.s[5] = bounds.height;
	rep->param.s[9] = bounds.x;
	rep->param.s[8] = bounds.y;
	return(True);
}

static TranslateEvent(client, ev, rep)
	int	client;
	XEvent	*ev;
	X10Rep	*rep;
{
	int	keycode;
	WinInfoptr	wp;
	int	event = EventOK;
	
	switch (ev->type) {
	case KeyPress:
	case KeyRelease:
	case ButtonPress:
	case ButtonRelease:
		rep->param.s[2] = ev->xmotion.time / 10;
	case MotionNotify:
	case EnterNotify:
	case LeaveNotify:
		rep->param.l[0] = ev->xmotion.window;
		rep->param.s[4] = ev->xmotion.x;
		rep->param.s[5] = ev->xmotion.y;
		rep->param.l[3] = ev->xmotion.subwindow;
		rep->param.l[4] = (ev->xmotion.x_root << 16)
			| ev->xmotion.y_root;

		switch (ev->type) {
		case MotionNotify:
			rep->param.s[3] =
				X11toX10KeyButState(ev->xmotion.state);
			rep->code = X10MouseMoved;
			break;

		case KeyPress:
		case KeyRelease:
			rep->param.s[3] =
				X11toX10KeyButState(ev->xkey.state);
			keycode = X11toX10Keycode(ev->xkey.display,
					(KeyCode)ev->xkey.keycode);
			rep->param.s[3] |= keycode;
			if (ev->type == KeyPress)
				rep->code = X10KeyPressed;
			else
				rep->code = X10KeyReleased;
			if (keycode == KEY_LOCK && LockToggleMode)
				event = EventLockToggleMode;
			break;

		case ButtonPress:
		case ButtonRelease:
			rep->param.s[3] =
				X11toX10KeyButState(ev->xbutton.state);
			if (ev->type == ButtonRelease)
				rep->code = X10ButtonReleased;
			else
				rep->code = X10ButtonPressed;
			switch(ev->xbutton.button) {
			case Button1:
				rep->param.s[3] |= 2;
				break;
			case Button2:
				rep->param.s[3] |= 1;
				break;
			case Button3:
			case Button4:
			case Button5:
				break;
			}
			break;

		case EnterNotify:
		case LeaveNotify:
			if (ev->xcrossing.mode != NotifyNormal) {
				event = EventModeField;
				break;
			}
			rep->param.s[3] =
				X11toX10KeyButState(ev->xcrossing.state);
			if (ev->type == EnterNotify)
				rep->code = X10EnterWindow;
			else
				rep->code = X10LeaveWindow;
			switch (ev->xcrossing.detail) {
			case NotifyInferior:
				rep->param.s[3] |= 1;
				break;
			case NotifyVirtual:
				rep->param.s[3] |= 2;
				break;
			case NotifyAncestor:
			case NotifyNonlinear:
			case NotifyNonlinearVirtual:
				break;
			}
		}

		break;

	case ClientMessage:
	case FocusIn:
	case FocusOut:
	case KeymapNotify:
	case VisibilityNotify:
	case CreateNotify:
	case CirculateRequest:
	case ConfigureRequest:
	case MapRequest:
	case ResizeRequest:
	case PropertyNotify:
	case ColormapNotify:
		event = EventHasNoX10Equiv;
		break;

	case ConfigureNotify:
		event = GenerateConfigureExposeEvent(client,
				&ev->xconfigure, rep);
		break;

	case MapNotify:
		event = GenerateMapExposeEvent(client, &ev->xmap, rep);
		break;

	case NoExpose:
		event = GenerateNoExposeEvent(client, &ev->xnoexpose, rep);
		break;

	case Expose:
		event = GenerateExposeEvent(client, &ev->xexpose, rep);
		break;

	case GraphicsExpose:
		event = GenerateGraphicsExposeEvent(client,
			&ev->xgraphicsexpose, rep);
		break;

	case UnmapNotify:
		event = GenerateUnmapEvent(client,
			&ev->xunmap, rep);
		break;

	case DestroyNotify:
		event = EventHasNoX10Equiv;
		/*
		 * We only look it up when the event delivered goes
		 * to the window that was destroyed... that way we
		 * only look it up once.
		 */
		if (ev->xdestroywindow.event != ev->xdestroywindow.window)
			break;
		wp = WinLookup(client, ev->xdestroywindow.window, False);
		if (wp != NULL) {
			FreeWinInfo(client, wp);
			ShowWinTree(wp->parent, 0);
		}
		break;
	case ReparentNotify:
		/* XXX should break out in cold sweat here */
	case GravityNotify:
		/* XXX should reset window size */
	case CirculateNotify:
		/* XXX should reset window size if necessary */
	case SelectionClear:
	case SelectionNotify:
	case SelectionRequest:
		/* XXX should do something with our properties */
		event = EventHasNoX10Equiv;
		break;
	default:
		event = EventUnknown;
		break;
	}

	return(event);
}

static UnGetXEvent(client, ev)
	int	client;
	XEvent	*ev;
{
	Clientptr	cp = clist[ client ];
	/*
	 * we really only need one for graphics exposures.
	 */
	if (cp->buffer.type != 0)
		Error("event buffer overflow\n");
	bcopy(ev, &cp->buffer, sizeof(XEvent));
	if (cp->buffer.type == 0)
		Error("funny event type\n");
}

static GetNextXEvent(client, ev, forceIt)
	int	client;
	XEvent	*ev;
	int	forceIt;
{
	Clientptr	cp = clist[ client ];
	Display 	*dpy;

	if (cp->buffer.type != 0) {
		bcopy(&cp->buffer, ev, sizeof(XEvent));
		cp->buffer.type = 0;
		return (True);
	}

	dpy = cp->dpy;
	if (!forceIt && QLength(dpy) == 0)
		return (False);
	XNextEvent(dpy, ev);
	if (clist[ client ] == NULL)	/* we were shut down */
		return (False);
	return (True);
}

X11events(serverfd)
	int	serverfd;
{
	int	client,
		eventcnt = 0,
		qLength = 0,
		status;
	Display *dpy;
	XEvent	pe;
	X10Rep	*rep, *replies;

	client = (int)clist[ serverfd ];
	if (client < 0 || client >= FileDescriptors)
		Error("X11events: server %d: client # %d?\n",
			serverfd, client);
	dpy = clist[ client ]->dpy;
	if (dpy == NULL)
		return;

	if (! GetNextXEvent(client, &pe, True))
		return;

	qLength = QLength(dpy) + 1;
	rep = replies = (X10Rep *)StaticAlloc(sizeof(X10Rep) * (++qLength));
	debug(XSequenceDebug, ("Events: Last seq #%d\n",
		dpy->last_request_read));

	do {
		status = TranslateEvent(client, &pe, rep);
		ShowX11AndX10Event(client,
			&pe,
			status == EventOK ? rep : NULL,
			&status);

		if (status == EventOK) {
			if (++eventcnt == qLength)
				replies = (X10Rep *)
				    StaticAlloc(sizeof(X10Rep) * (++qLength));
			rep = replies + eventcnt;
		}

	} while (GetNextXEvent(client, &pe, False));

	if (eventcnt)
		sendevents(client, replies, eventcnt);
}

unsigned int X10toX11EventMask(mask)
unsigned int	mask;
{
	unsigned int	X11mask = 0;

	if (X10KeyPressed & mask)	X11mask |= KeyPressMask;
	if (X10KeyReleased & mask)	X11mask |= KeyReleaseMask;
	if (X10ButtonPressed & mask)	X11mask |= ButtonPressMask;
	if (X10ButtonReleased & mask)	X11mask |= ButtonReleaseMask;
	if (X10EnterWindow & mask)	X11mask |= EnterWindowMask;
	if (X10LeaveWindow & mask)	X11mask |= LeaveWindowMask;
	if (X10MouseMoved & mask)	X11mask |= PointerMotionMask;
	if (X10RightDownMotion & mask)	X11mask |= Button3MotionMask;
	if (X10MiddleDownMotion & mask)	X11mask |= Button2MotionMask;
	if (X10LeftDownMotion & mask)	X11mask |= Button1MotionMask;
	if (X10ExposeWindow & mask)	X11mask |= ExposureMask;
	if (X10ExposeRegion & mask)	X11mask |= ExposureMask;
	if (X10ExposeCopy & mask)	X11mask |= 0; /* XXX done in gc */
	if (X10FocusChange & mask)	X11mask |= FocusChangeMask;
	if (X10UnmapWindow & mask)	X11mask |= StructureNotifyMask;

	X11mask |= OwnerGrabButtonMask; /* always add this */

	/*
	 * We need this for map and configureNotify events.
	 */
	X11mask |= (StructureNotifyMask | SubstructureNotifyMask);
	return (X11mask);
}

unsigned int X11toX10EventMask(mask)
unsigned int	mask;
{
	unsigned int	X10mask = 0;

	/* XXX missing GraphicsExposure --> ExposeCopy */
	if (KeyPressMask & mask)		X10mask |= X10KeyPressed;
	if (KeyReleaseMask & mask)		X10mask |= X10KeyReleased;
	if (ButtonPressMask & mask)		X10mask |= X10ButtonPressed;
	if (ButtonReleaseMask & mask)		X10mask |= X10ButtonReleased;
	if (EnterWindowMask & mask)		X10mask |= X10EnterWindow;
	if (LeaveWindowMask & mask)		X10mask |= X10LeaveWindow;
	if (PointerMotionMask & mask)		X10mask |= X10MouseMoved;
	if (PointerMotionHintMask & mask)	X10mask |= X10MouseMoved;
	if (Button1MotionMask & mask)		X10mask |= X10LeftDownMotion;
	if (Button2MotionMask & mask)		X10mask |= X10MiddleDownMotion;
	if (Button3MotionMask & mask)		X10mask |= X10RightDownMotion;
	if (Button4MotionMask & mask)		/* X10mask |= 0 */;
	if (Button5MotionMask & mask)		/* X10mask |= 0 */;
	if (ButtonMotionMask & mask)		/* X10mask |= 0 */;
	if (KeymapStateMask & mask)		/* X10mask |= 0 */;
	if (ExposureMask & mask)		X10mask |= X10ExposeWindow;
	if (VisibilityChangeMask & mask)	/* X10mask |= 0 */;
	if (StructureNotifyMask & mask)		X10mask |= X10UnmapWindow;
	if (ResizeRedirectMask & mask)		/* X10mask |= 0 */;
	if (SubstructureNotifyMask & mask)	X10mask |= X10UnmapWindow;
	if (SubstructureRedirectMask & mask)	/* X10mask |= 0 */;
	if (FocusChangeMask & mask)		X10mask |= X10FocusChange;
	if (PropertyChangeMask & mask)		/* X10mask |= 0 */;
	if (ColormapChangeMask & mask)		/* X10mask |= 0 */;
	if (OwnerGrabButtonMask & mask)		/* X10mask |= 0 */;

	return (X10mask);
}

unsigned int X11toX10KeyButState(state)
unsigned int	state;
{
	unsigned int	x10state = 0;

	if (LockToggleMode) {
		if (LockStateOn)	x10state |= X10ShiftLockMask;
	} 
	else {
		if (state & LockMask)	x10state |= X10ShiftLockMask;
	}

	if (state & ShiftMask)		x10state |= X10ShiftMask;
	if (state & ControlMask)	x10state |= X10ControlMask;
	if (state & Mod1Mask)		x10state |= X10MetaMask;
	if (state & Mod2Mask)		x10state |= 0;
	if (state & Mod3Mask)		x10state |= 0;
	if (state & Mod4Mask)		x10state |= 0;
	if (state & Mod5Mask)		x10state |= 0;
	if (state & Button1Mask)	x10state |= X10LeftMask;
	if (state & Button2Mask)	x10state |= X10MiddleMask;
	if (state & Button3Mask)	x10state |= X10RightMask;
	if (state & Button4Mask)	x10state |= 0;
	if (state & Button5Mask)	x10state |= 0;

	return(x10state);
}

unsigned int X10ButtonMaskToX11KeyMask(mask)
unsigned int	mask;
{
	unsigned int	x11mask = 0;

	if (mask & X10ShiftMask)	x11mask |= ShiftMask;
	if (mask & X10ShiftLockMask)	x11mask |= LockMask;
	if (mask & X10ControlMask)	x11mask |= ControlMask;
	if (mask & X10MetaMask)		x11mask |= Mod1Mask;

	return(x11mask);
}

/*
* Called for GrabButton and UngrabButton: mask must have exactly
* one of LeftMask, MiddleMask or RightMask set.
*/
unsigned int X10ButtonMaskToX11Button(mask)
unsigned int	mask;
{
	unsigned int	button;

	switch(mask & (X10LeftMask|X10MiddleMask|X10RightMask)) {
	case X10LeftMask:       
		button = Button1; 
		break;
	case X10MiddleMask:     
		button = Button2; 
		break;
	case X10RightMask:      
		button = Button3; 
		break;
	default:		
		button = -1; 
		break;
	}
	return (button);
}

#define MAP_LENGTH	0x10000
unsigned char X11KeysymToX10KeycodeMap[ MAP_LENGTH ];
/*
* We must translate an X11 keycode --> X11 keysym --> X10 keycode,
* which just happens to be an lk201 keycode.
 */
InitKeycodeMap()
{
	X11KeysymToX10KeycodeMap[ XK_F1 ] = KEY_F1;
	X11KeysymToX10KeycodeMap[ XK_F2 ] = KEY_F2;
	X11KeysymToX10KeycodeMap[ XK_F3 ] = KEY_F3;
	X11KeysymToX10KeycodeMap[ XK_F4 ] = KEY_F4;
	X11KeysymToX10KeycodeMap[ XK_F5 ] = KEY_F5;
	X11KeysymToX10KeycodeMap[ XK_F6 ] = KEY_F6;
	X11KeysymToX10KeycodeMap[ XK_F7 ] = KEY_F7;
	X11KeysymToX10KeycodeMap[ XK_F8 ] = KEY_F8;
	X11KeysymToX10KeycodeMap[ XK_F9 ] = KEY_F9;
	X11KeysymToX10KeycodeMap[ XK_F10 ] = KEY_F10;
	X11KeysymToX10KeycodeMap[ XK_Escape ] = KEY_ESC;
	X11KeysymToX10KeycodeMap[ XK_BackSpace ] = KEY_F12;
	X11KeysymToX10KeycodeMap[ XK_Linefeed ] = KEY_LF;
	X11KeysymToX10KeycodeMap[ XK_F14 ] = KEY_F14;
	X11KeysymToX10KeycodeMap[ XK_Help ] = KEY_HELP;
	X11KeysymToX10KeycodeMap[ XK_Menu ] = KEY_MENU;
	X11KeysymToX10KeycodeMap[ XK_F17 ] = KEY_F17;
	X11KeysymToX10KeycodeMap[ XK_F18 ] = KEY_F18;
	X11KeysymToX10KeycodeMap[ XK_F19 ] = KEY_F19;
	X11KeysymToX10KeycodeMap[ XK_F20 ] = KEY_F20;
	X11KeysymToX10KeycodeMap[ XK_Find ] = KEY_FIND;
	X11KeysymToX10KeycodeMap[ XK_Insert ] = KEY_INSERT_HERE;
	X11KeysymToX10KeycodeMap[ XK_Delete ] = KEY_REMOVE;
	X11KeysymToX10KeycodeMap[ XK_Select ] = KEY_SELECT;
	X11KeysymToX10KeycodeMap[ XK_Prior ] = KEY_PREV_SCREEN;
	X11KeysymToX10KeycodeMap[ XK_Next ] = KEY_NEXT_SCREEN;
	X11KeysymToX10KeycodeMap[ XK_KP_0 ] = KEY_KP_0;
	X11KeysymToX10KeycodeMap[ XK_KP_Decimal ] = KEY_KP_PERIOD;
	X11KeysymToX10KeycodeMap[ XK_KP_Enter ] = KEY_KP_ENTER;
	X11KeysymToX10KeycodeMap[ XK_KP_1 ] = KEY_KP_1;
	X11KeysymToX10KeycodeMap[ XK_KP_2 ] = KEY_KP_2;
	X11KeysymToX10KeycodeMap[ XK_KP_3 ] = KEY_KP_3;
	X11KeysymToX10KeycodeMap[ XK_KP_4 ] = KEY_KP_4;
	X11KeysymToX10KeycodeMap[ XK_KP_5 ] = KEY_KP_5;
	X11KeysymToX10KeycodeMap[ XK_KP_6 ] = KEY_KP_6;
	X11KeysymToX10KeycodeMap[ XK_KP_Separator ] = KEY_KP_COMMA;
	X11KeysymToX10KeycodeMap[ XK_KP_7 ] = KEY_KP_7;
	X11KeysymToX10KeycodeMap[ XK_KP_8 ] = KEY_KP_8;
	X11KeysymToX10KeycodeMap[ XK_KP_9 ] = KEY_KP_9;

	X11KeysymToX10KeycodeMap[ XK_KP_Subtract ] = KEY_KP_HYPHEN;
	X11KeysymToX10KeycodeMap[ XK_hyphen ] = KEY_UBAR;

	X11KeysymToX10KeycodeMap[ XK_KP_F1 ] = KEY_KP_PF1;
	X11KeysymToX10KeycodeMap[ XK_KP_F2 ] = KEY_KP_PF2;
	X11KeysymToX10KeycodeMap[ XK_KP_F3 ] = KEY_KP_PF3;
	X11KeysymToX10KeycodeMap[ XK_KP_F4 ] = KEY_KP_PF4;
	X11KeysymToX10KeycodeMap[ XK_Left ] = KEY_LEFT;
	X11KeysymToX10KeycodeMap[ XK_Right ] = KEY_RIGHT;
	X11KeysymToX10KeycodeMap[ XK_Down ] = KEY_DOWN;
	X11KeysymToX10KeycodeMap[ XK_Up ] = KEY_UP;
	X11KeysymToX10KeycodeMap[ XK_Shift_L ] = KEY_SHIFT;
	X11KeysymToX10KeycodeMap[ XK_Control_L ] = KEY_CTRL;
	X11KeysymToX10KeycodeMap[ XK_Caps_Lock ] = KEY_LOCK;
	X11KeysymToX10KeycodeMap[ XK_Multi_key ] = KEY_COMPOSE;
	X11KeysymToX10KeycodeMap[ XK_Delete ] = KEY_DELETE;
	X11KeysymToX10KeycodeMap[ XK_Return ] = KEY_RETURN;
	X11KeysymToX10KeycodeMap[ XK_Tab ] = KEY_TAB;
	X11KeysymToX10KeycodeMap[ XK_quoteleft ] = KEY_TILDE;
	X11KeysymToX10KeycodeMap[ XK_1 ] = KEY_TR_1;                 
	X11KeysymToX10KeycodeMap[ XK_q ] = KEY_Q;
	X11KeysymToX10KeycodeMap[ XK_a ] = KEY_A;
	X11KeysymToX10KeycodeMap[ XK_z ] = KEY_Z;
	X11KeysymToX10KeycodeMap[ XK_2 ] = KEY_TR_2;
	X11KeysymToX10KeycodeMap[ XK_w ] = KEY_W;
	X11KeysymToX10KeycodeMap[ XK_s ] = KEY_S;
	X11KeysymToX10KeycodeMap[ XK_x ] = KEY_X;
	X11KeysymToX10KeycodeMap[ XK_less ] = KEY_LANGLE_RANGLE;
	X11KeysymToX10KeycodeMap[ XK_3 ] = KEY_TR_3;
	X11KeysymToX10KeycodeMap[ XK_e ] = KEY_E;
	X11KeysymToX10KeycodeMap[ XK_d ] = KEY_D;
	X11KeysymToX10KeycodeMap[ XK_c ] = KEY_C;
	X11KeysymToX10KeycodeMap[ XK_4 ] = KEY_TR_4;
	X11KeysymToX10KeycodeMap[ XK_r ] = KEY_R;
	X11KeysymToX10KeycodeMap[ XK_f ] = KEY_F;
	X11KeysymToX10KeycodeMap[ XK_v ] = KEY_V;
	X11KeysymToX10KeycodeMap[ XK_space ] = KEY_SPACE;
	X11KeysymToX10KeycodeMap[ XK_5 ] = KEY_TR_5;
	X11KeysymToX10KeycodeMap[ XK_t ] = KEY_T;
	X11KeysymToX10KeycodeMap[ XK_g ] = KEY_G;
	X11KeysymToX10KeycodeMap[ XK_b ] = KEY_B;
	X11KeysymToX10KeycodeMap[ XK_6 ] = KEY_TR_6;
	X11KeysymToX10KeycodeMap[ XK_y ] = KEY_Y;
	X11KeysymToX10KeycodeMap[ XK_h ] = KEY_H;
	X11KeysymToX10KeycodeMap[ XK_n ] = KEY_N;
	X11KeysymToX10KeycodeMap[ XK_7 ] = KEY_TR_7;
	X11KeysymToX10KeycodeMap[ XK_u ] = KEY_U;
	X11KeysymToX10KeycodeMap[ XK_j ] = KEY_J;
	X11KeysymToX10KeycodeMap[ XK_m ] = KEY_M;
	X11KeysymToX10KeycodeMap[ XK_8 ] = KEY_TR_8;
	X11KeysymToX10KeycodeMap[ XK_i ] = KEY_I;
	X11KeysymToX10KeycodeMap[ XK_k ] = KEY_K;
	X11KeysymToX10KeycodeMap[ XK_comma ] = KEY_COMMA;
	X11KeysymToX10KeycodeMap[ XK_9 ] = KEY_TR_9;
	X11KeysymToX10KeycodeMap[ XK_o ] = KEY_O;
	X11KeysymToX10KeycodeMap[ XK_l ] = KEY_L;
	X11KeysymToX10KeycodeMap[ XK_period ] = KEY_PERIOD;
	X11KeysymToX10KeycodeMap[ XK_0 ] = KEY_TR_0;
	X11KeysymToX10KeycodeMap[ XK_p ] = KEY_P;
	X11KeysymToX10KeycodeMap[ XK_semicolon ] = KEY_SEMICOLON;
	X11KeysymToX10KeycodeMap[ XK_slash ] = KEY_QMARK;   
	X11KeysymToX10KeycodeMap[ XK_equal ] = KEY_PLUS;
	X11KeysymToX10KeycodeMap[ XK_bracketright ] = KEY_RBRACE;
	X11KeysymToX10KeycodeMap[ XK_backslash ] = KEY_VBAR;
	X11KeysymToX10KeycodeMap[ XK_minus ] = KEY_UBAR;
	X11KeysymToX10KeycodeMap[ XK_bracketleft ] = KEY_LBRACE;
	X11KeysymToX10KeycodeMap[ XK_quoteright ] = KEY_QUOTE;
}

X11toX10Keycode(dpy, x11keyode)
	Display	*dpy;
	KeyCode x11keyode;
{
	unsigned short 	keysym;

	/* always pick column 0 and let the x10 client interpret */
	keysym = (unsigned short) XKeycodeToKeysym(dpy, x11keyode, 0);
	return ( X11KeysymToX10KeycodeMap[ keysym ] );
}
