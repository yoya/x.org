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
 * $Header: proto1.c,v 4.1 88/02/14 17:04:33 toddb Exp $ 
 * $Locker:  $ 
 */
#include "X11/Xlib.h"
#include "X11/Xatom.h"
#include "X11/Xutil.h"
#include "resource.h"

extern u_char	Xstatus;
extern u_char	GrabServerAllowed;
extern u_char	GrabMouseAllowed;
extern u_char	ServerGrabbed;
extern u_char	DoSync;

X10ProcSetUp(client, rep)
	int	client;
	X10Rep	*rep;
{
	Display *dpy = clist[ client ]->dpy;

	rep->param.l[0] = DefaultRootWindow(dpy);
	rep->param.s[2] = X10_PROTOCOL;
	rep->param.s[3] = XDEV_X10TRANSLATOR;
	rep->param.s[4] = XDefaultDepth(dpy, DefaultScreen(dpy));
	rep->param.u[5] = XDisplayCells(dpy, DefaultScreen(dpy));
	/*
	 * This is also used as a synchronization mechanism in
	 * X10... so we do the same here.
	 */
	XSync(dpy, False);
}

X10ProcQueryWindow(client, req, rep)
	int	client;
	X10Req	*req;
	X10Rep	*rep;
{
	XWindowAttributes	att;
	Clientptr cp = clist[ client ];
	Display *dpy = cp->dpy;
	Atom	ActualType;
	int	ActualFormat, BytesAfter, nitems;
	unsigned int	x10mask, x11mask;
	char	*prop;
	Window	w = req->windowId;
	GetWinInfoOrReturnBadWindow(client, w, wp);

	if (! XGetWindowAttributes(dpy, w, &att)) {
		Xstatus = X10BadWindow;
		return(False);
	}
	x11mask = att.your_event_mask;
	x10mask = X11toX10EventMask(x11mask);
	if (debugged(X10_QueryWindow))
		ShowEventMask(x10mask, x11mask);
	rep->param.l[4] = wp->mask = x10mask;
	rep->param.s[0] = wp->height = att.height;
	rep->param.s[1] = wp->width = att.width;
	rep->param.s[2] = att.x - att.border_width;
	rep->param.s[3] = att.y - att.border_width;
	rep->param.s[4] = att.border_width;
	wp->borderWidth = att.border_width;

	switch(att.map_state) {
	case IsUnmapped:
		rep->param.b[10] = 0;
		wp->mapped = False;
		break;
	case IsUnviewable:
		rep->param.b[10] = 2;
		wp->mapped = True;
		break;
	case IsViewable:
		rep->param.b[10] = 1;
		wp->mapped = True;
		break;
	}

	switch(att.class) {
	case InputOutput:
		wp->transparent = False;
		rep->param.b[11] = 1;
		if (XGetWindowProperty(dpy, w, XA_WM_ICON_NAME,
		     0, 1, False,
		     (Atom)AnyPropertyType, &ActualType, &ActualFormat, &nitems,
		     &BytesAfter, &prop) == 0) {
			if (ActualType == XA_BITMAP
			|| ActualType == XA_PIXMAP)
				rep->param.b[11] = 2;
			if (prop)
				free (prop);
		}
		break;
	case InputOnly:
		wp->transparent = True;
		rep->param.b[11] = 0;
		rep->param.l[3] = 0;
		break;
	}

	return(True);
}

X10ProcCreateWindow(client, req, rep)
	int	client;
	X10Req	*req;
	X10Rep	*rep;
{
	XSetWindowAttributes xswa;
	Clientptr	cp = clist[ client ];
	Display		*dpy = cp->dpy;
	u_long		valuemask;
	XID		win;
	XSizeHints	sh;
	int		depth;
	unsigned int	width = req->param.s[1],
			height = req->param.s[0],
			border_width,
			class = (req->code == X10_CreateTransparency)
				? InputOnly : InputOutput;
	WinInfoptr	newp;
	GetWinInfoOrReturnBadWindow(client, req->windowId, wp);

	valuemask = 0;
	if (class == InputOnly) {
		border_width = 0;
		depth = 0;
	} else {
		/*
		 * X10 backgrounds are always either inheirited from the
		 * parent or an explicit pixmap.
		 */
		valuemask |= CWBackPixmap;
		if (req->param.l[3])
			xswa.background_pixmap = req->param.l[3];
		else
			xswa.background_pixmap = wp->tile;

		/*
		 * XXX X10 spec does not say what the border_pixmap
		 * should be of it is specified as 0 and the
		 * border width is > 0.  Assume CopyFromParent (default).
		 */
		if (req->param.l[2]) {
			valuemask |= CWBorderPixmap;
			xswa.border_pixmap = req->param.l[2];
		}

		border_width = req->func;
		depth = DefaultDepth(dpy, DefaultScreen(dpy));
	}

	/*
	 * We roll our own window prompting.
	 */
	xswa.override_redirect = True;
	valuemask |= CWOverrideRedirect;

	win = XCreateWindow(dpy,
		req->windowId,				/* parent */
		req->param.s[2],			/* x */
		req->param.s[3],			/* y */
		width,					/* width */
		height,					/* height */
		border_width,				/* border width */
		depth,					/* depth */
		class,					/* class */
		DefaultVisual(dpy, 0),			/* visual */
		valuemask,				/* flags */
		&xswa);					/* attributes */
	rep->param.l[0] = win;
	/* no errors detectable! XXX */

	newp = AddWin(client, win, wp, class == InputOnly, class == InputOnly);
	if (newp == NULL) {
		XDestroyWindow(dpy, win);
		Xstatus = X10BadAlloc;
	}

	newp->x = sh.x = req->param.s[2];
	newp->y = sh.y = req->param.s[3];
	newp->height = sh.height = height;
	newp->width = sh.width = width;
	newp->tile = xswa.background_pixmap;
	sh.flags = (USSize | USPosition | PPosition | PSize);
	XSetNormalHints(dpy, win, &sh);

	/*
	 * Tile mode of Transparent window is TileModeRelative.
	 */
	if (class == InputOnly)
		SetTileMode(dpy, newp, True);

	/*
	 * Force exposure events on all windows... weed them out later.
	 */
	XSelectInput(dpy, win, ExposureMask | StructureNotifyMask);

	ShowWinTree(cp->win, 0);
	return (True);
}

#ifdef	notdef	
X10ProcMapWindow(client, req)
	int	client;
	X10Req	*req;
{
	if (! MarkMapped(client, req->windowId, True))
		Xstatus = X10BadWindow;
	else
		XMapRaised(clist[ client ]->dpy, req->windowId);
}

X10ProcMapSubwindows(client, req)
	int	client;
	X10Req	*req;
{
	if (! MarkMappedTree(client, req->windowId, True))
		Xstatus = X10BadWindow;
	else
		XMapSubwindows(clist[ client ]->dpy, req->windowId);
}

X10ProcUnmapWindow(client, req)
	int	client;
	X10Req	*req;
{
	Window	w = req->windowId;

	if (! MarkMapped(client, w, False)) {
		Xstatus = X10BadWindow;
		return;
	}
	XUnmapWindow(clist[ client ]->dpy, w);
}
#endif

X10ProcMapWindow(client, req)
	int	client;
	X10Req	*req;
{
	Window	w = req->windowId;
	GetWinInfoOrReturnBadWindow(client, w, wp);

	XMapRaised(clist[ client ]->dpy, w);
}

X10ProcMapSubwindows(client, req)
	int	client;
	X10Req	*req;
{
	Window	w = req->windowId;
	GetWinInfoOrReturnBadWindow(client, w, wp);

	XMapRaised(clist[ client ]->dpy, w);
	XMapSubwindows(clist[ client ]->dpy, w);
}

X10ProcUnmapWindow(client, req)
	int	client;
	X10Req	*req;
{
	Window	w = req->windowId;
	GetWinInfoOrReturnBadWindow(client, w, wp);

	XUnmapWindow(clist[ client ]->dpy, w);
}

AnyExposeOrUnmapEvent(dpy, ev, junk)
	Display	*dpy;
	XEvent	*ev;
	int	junk;
{
	return (ev->type == Expose || ev->type == UnmapNotify);
}

X10ProcUnmapTransparent(client, req)
	int	client;
	X10Req	*req;
{
	XSetWindowAttributes xswa;
	Display	*dpy = clist[ client ]->dpy;
	Window	w = req->windowId;
	XEvent	event;
	int	count;
	GetWinInfoOrReturnBadWindow(client, w, wp);

	/*
	 * The closest that we can come here is to slurp up all the
	 * expose events.  We can do a better job later (without the
	 * GrabServer) when the sequence number appears in the Xlib event
	 * structure.
	 *
	 * Truly awful:
	 *	grab the server (so nobody moves a window, etc.)
	 *	process all events up to this point
	 *	unmap the window in question
	 *	if there's a save-under for this that thinks its been
	 *		restored, restore it.
	 *	suck up ALL exposure events
	 *	let go of the server
	 */
	if (!ServerGrabbed && GrabServerAllowed)
		XGrabServer(dpy);
	if (!DoSync)
		XSync(dpy, False);
	CheckX11events(client, req->code);

	XUnmapWindow(dpy, w);
	if (!wp->transparent && wp->savedPixmap) {
		PixmapInfoptr	pp = PixmapLookup(client, wp->savedPixmap);
		WinInfoptr	next;
		int		x = 0, y = 0;
		unsigned long	valuemask;
		XGCValues	gcv;
		Bool		drawThrough = wp->drawThrough;

		/*
		 * Find the root-relative x and y.
		 */
		for (next = wp; next; next = next->parent) {
			x += next->x;
			y += next->y;
		}

		/*
		 * Fix up the GC for the copy... Be sure SubwindowMode is set
		 */
		valuemask = GCFunction | GCPlaneMask | GCGraphicsExposures;
		gcv.function = GXcopy;
		gcv.plane_mask = ~0;
		gcv.graphics_exposures = False;
		XChangeGC(dpy, wp->gc, valuemask, &gcv);
		SetClipMode(dpy, wp, True);

		/*
		 * do the copy.
		 */
		XCopyArea(dpy,
			wp->savedPixmap,        /* source drawable */
			DefaultRootWindow(dpy),	/* destination drawable */
			wp->gc,
			0, 0,			/* source x,y */
			wp->width, wp->height,
			x, y);			/* dest x,y */

		/*
		 * Clean up.
		 */
		if (pp == NULL)
			XFreePixmap(dpy, wp->savedPixmap);
		else
			pp->savedPixmap = False;
		wp->savedPixmap = 0;
		SetClipMode(dpy, wp, drawThrough);
		gcv.graphics_exposures = True;
		XChangeGC(dpy, wp->gc, GCGraphicsExposures, &gcv);
	}

	if (!DoSync)
		XSync(dpy, False);
	if (!ServerGrabbed && GrabServerAllowed) {
		XUngrabServer(dpy);
		if (!DoSync)
			XFlush(dpy);
	}

	count = 0;
	while (XCheckIfEvent (dpy, &event, AnyExposeOrUnmapEvent, 0)) {
		if (debugged(event.type) && count == 0)
			bugout("...\n");
		ShowX11AndX10Event(client, &event, NULL, NULL);
		if (event.type == UnmapNotify)
			ProcessUnmapEvent(client, &event.xunmap);
		count++;
	}

	/*
	 * put the window back the way it was
	 */
	xswa.background_pixmap = wp->tile;
	XChangeWindowAttributes (dpy, w, CWBackPixmap, &xswa);
}

X10ProcUnmapSubwindows(client, req)
	int	client;
	X10Req	*req;
{
	if (! MarkMappedTree(client, req->windowId, False))
		Xstatus = X10BadWindow;
	else
		XUnmapSubwindows(clist[ client ]->dpy, req->windowId);
}

X10ProcGrabButton(client, req)
	int	client;
	X10Req	*req;
{
	unsigned int	button,
			x10EventMask = req->param.l[1],
			x11EventMask,
			x10ButtonMask = req->mask,
			x11ButtonMask;
	Cursor		cursor = req->param.l[0];
	Clientptr	cp = clist[ client ];
	Display		*dpy = cp->dpy;
	GetWinInfoOrReturnBadWindow(client, req->windowId, wp);

	button = X10ButtonMaskToX11Button(x10ButtonMask);
	x11ButtonMask = X10ButtonMaskToX11KeyMask(x10ButtonMask);
	x11EventMask = X10toX11EventMask(x10EventMask);

	if (debugged(X10_GrabButton)) {
		ShowEventMask(x10EventMask, x11EventMask);
		ShowButtonMask(x10ButtonMask, x11ButtonMask);
	}

	if (button > Button3) {
		Xstatus = X10BadValue;
		return(False);
	}

	XGrabButton(dpy,
		button,				/* button */
		x11ButtonMask,			/* modifiers */
		req->windowId,
		True,				/* owner events */
		x11EventMask,
		GrabModeAsync, GrabModeAsync,	/* pointer/keyboard mode */
		None,				/* confine-to window */
		cursor == 0 ? None : cursor);

	/* XXX are X11 pointer/button grabs exclusive? */
	wp->grabmask = wp->mask;
	wp->mask = x11ButtonMask;
	cp->grabwin = req->windowId;

	return(True);
}

X10ProcUngrabButton(client, req)
	int	client;
	X10Req	*req;
{
	Clientptr	cp = clist[ client ];
	Display		*dpy = cp->dpy;
	WinInfoptr	wp;

	/* XXX are X11 pointer/button grabs exclusive? */
	wp = WinLookup(client, cp->grabwin, False);
	cp->grabwin = 0;
	if (wp == NULL)
		return;
	wp->mask = wp->grabmask;

	XUngrabButton(dpy,
		(unsigned int)AnyButton,
		(unsigned int)AnyModifier,
		cp->grabwin);
}

X10ProcGrabMouse(client, req)
	int	client;
	X10Req	*req;
{
	int	status,
		result = False,
		mask = req->param.l[1];
	Cursor	cursor = req->param.l[0];
	Clientptr	cp = clist[ client ];
	Display *dpy = cp->dpy;
	GetWinInfoOrReturnBadWindow(client, req->windowId, wp);

	if (! GrabMouseAllowed)
		return(True);
	/*
	 * Only these bits are significant in the X10 GrabMouse request.
	 */
	mask =    X10ButtonPressed
		| X10ButtonReleased
		| X10EnterWindow
		| X10LeaveWindow
		| X10MouseMoved
		| X10RightDownMotion
		| X10MiddleDownMotion
		| X10LeftDownMotion;

	mask = (req->param.l[1] & mask) | (wp->mask & ~mask);

	/*
	 * Force on expose events... we will weed them out later.
	 */
	status = XGrabPointer(dpy,
			req->windowId,
			True,
			X10toX11EventMask(mask) | ExposureMask,
			GrabModeAsync, GrabModeAsync,
			None,
			cursor == 0 ? None : cursor,
			CurrentTime);
	switch (status) {
	case AlreadyGrabbed:
		debug(X10_GrabMouse, ("AlreadyGrabbed "));
		Xstatus = X10BadGrab;
		break;
	case GrabNotViewable:
		debug(X10_GrabMouse, ("GrabNotViewable "));
		Xstatus = X10BadGrab;
		break;
	case GrabFrozen:
		debug(X10_GrabMouse, ("GrabFrozen "));
		Xstatus = X10BadGrab;
		break;
	case GrabInvalidTime:
		Error("Invalid time on GrabPointer()");
		break;
	case GrabSuccess:
		result = True;
		wp->grabmask = wp->mask;
		wp->mask = mask;
		cp->grabwin = req->windowId;
		break;
	default:
		Error("unknow GrabPointer status=%d", status);
		break;
	}
	return(result);
}

X10ProcUngrabMouse(client, req)
	int	client;
	X10Req	*req;
{
	Clientptr	cp = clist[ client ];
	Display		*dpy = cp->dpy;
	WinInfoptr	wp;

	/* XXX are X11 pointer/button grabs exclusive? */
	wp = WinLookup(client, cp->grabwin, False);
	cp->grabwin = 0;
	if (wp == NULL)
		return;
	wp->mask = wp->grabmask;

	XUngrabPointer(dpy, CurrentTime);
}

X10ProcGrabServer(client)
	int	client;
{
	if (GrabServerAllowed) {
		XGrabServer(clist[ client ]->dpy);
		ServerGrabbed = True;
	}
}

X10ProcUngrabServer(client)
	int	client;
{
	if (GrabServerAllowed) {
		XUngrabServer(clist[ client ]->dpy);
		ServerGrabbed = False;
	}
}

X10ProcQueryMouse(client, req, rep)
	int	client;
	X10Req	*req;
	X10Rep	*rep;
{
	Window	root, child;
	int	rootx, rooty, winx, winy;
	unsigned	mask;
	Display *dpy = clist[ client ]->dpy;
	Bool	result;

	result = XQueryPointer(dpy,
			req->windowId,
			&root,
			&child,
			&rootx,
			&rooty,
			&winx,
			&winy,
			&mask);

	if (! result) {
		Xstatus = X10BadWindow;
		return (False);
	}
	rep->param.l[0] = child;
	rep->param.s[2] = winx;
	rep->param.s[3] = winy;
	rep->param.s[4] = X11toX10KeyButState(mask);
	return (True);
}

X10ProcStoreName(client, req, command)
	int	client;
	X10Req	*req;
	unsigned char	*command;
{
	unsigned char	name[ BUFSIZ ];

	sprintf(name, "(X10) %s", command);
	XChangeProperty(clist[ client ]->dpy, 
	    req->windowId, XA_WM_NAME, XA_STRING, 8, PropModeReplace,
	    name, strlen(name));
	XChangeProperty(clist[ client ]->dpy, 
	    req->windowId, XA_WM_COMMAND, XA_STRING, 8, PropModeReplace,
	    command, strlen(command));
}

char *X10ProcFetchName(client, req, rep)
	int	client;
	X10Req	*req;
	X10Rep	*rep;
{
	char	*name;
	int	result, type, format, nitems, bytesafter;

	result = XGetWindowProperty(clist[ client ]->dpy, 
	    req->windowId, XA_WM_COMMAND, 0, BUFSIZ-1,
	    False, XA_STRING, &type, &format, &nitems, &bytesafter, &name);

	if (result == Success) {
		rep->param.s[0] = nitems;
		return(name);
	} else {
		rep->param.s[0] = 0;
		switch(result) {
		default:
			Notice("unknown XGetWindowProperty() return=%d",
				result);
			break;
		case BadMatch:
			break;
		case BadWindow:
			Xstatus = X10BadWindow;
			break;
		}
		return(NULL);
	}
}

X10ProcSetResizeHint(client, req)
	int	client;
	X10Req	*req;
{
	XSizeHints	sh;
	Display *dpy = clist[ client ]->dpy;
	int	result;

	result = XGetNormalHints(dpy, req->windowId, &sh);
	switch (result) {
	case BadWindow:
		Xstatus = X10BadWindow;
		return;
	default:
		break;
	}
	sh.min_height = req->param.s[0];
	sh.height_inc = req->param.s[1];
	sh.min_width = req->param.s[2];
	sh.width_inc = req->param.s[3];
	sh.flags |= (PMinSize | PResizeInc);
	(void) XSetNormalHints(dpy, req->windowId, &sh);
}

X10ProcGetResizeHint(client, req, rep)
	int	client;
	X10Req	*req;
	X10Rep	*rep;
{
	XSizeHints	sh;
	Display *dpy = clist[ client ]->dpy;
	int	result;

	result = XGetNormalHints(dpy, req->windowId, &sh);
	switch (result) {
	case BadWindow:
		Xstatus = X10BadWindow;
		return;
	default:
		rep->param.s[0] = sh.min_height;
		rep->param.s[1] = sh.height_inc;
		rep->param.s[2] = sh.min_width;
		rep->param.s[3] = sh.width_inc;
		break;
	}
}

X10ProcSelectInput(client, req)
	int	client;
	X10Req	*req;
{
	unsigned int	x10mask = req->param.l[0];
	unsigned int	x11mask = X10toX11EventMask(x10mask);
	GetWinInfoOrReturnBadWindow(client, req->windowId, wp);

	if (x10mask & X10ExposeRegion)
		x10mask |= X10ExposeWindow; /* protocol says so */
	if (debugged(X10_SelectInput))
		ShowEventMask(x10mask, x11mask);
	wp->mask = x10mask;

	/*
	 * Force selection of exposures for all windows and weed them
	 * out later.
	 */
	XSelectInput(clist[ client ]->dpy,
			req->windowId,
			x11mask | StructureNotifyMask | ExposureMask);
}

X10ProcClear(client, req)
	int	client;
	X10Req	*req;
{
	XClearWindow(clist[ client ]->dpy, req->windowId);
}

X10ProcDefineCursor(client, req)
	int	client;
	X10Req	*req;
{
	XDefineCursor(clist[ client ]->dpy,
		req->windowId,
		(Cursor) req->param.l[0]);
}

