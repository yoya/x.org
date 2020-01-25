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
 * $Header: proto3.c,v 4.1 88/02/14 17:05:21 toddb Exp $ 
 * $Locker:  $ 
 */
#include "X11/Xlib.h"
#include "X11/Xatom.h"
#include "resource.h"

extern u_char	Xstatus;
extern u_char	LockToggleMode;

X10ProcRaiseWindow(client, req)
	int	client;
	X10Req	*req;
{
	GetWinInfoOrReturnBadWindow(client, req->windowId, wp);

	XRaiseWindow(clist[ client ]->dpy, req->windowId);
	RaiseWindow(wp);
	ShowWinTree(wp->parent, 0);
}

X10ProcLowerWindow(client, req)
	int	client;
	X10Req	*req;
{
	GetWinInfoOrReturnBadWindow(client, req->windowId, wp);

	XLowerWindow(clist[ client ]->dpy, req->windowId);
	LowerWindow(wp);
	ShowWinTree(wp->parent, 0);
}

X10ProcCircWindow(client, req, up)
	int	client;
	X10Req	*req;
	int	up;
{
	GetWinInfoOrReturnBadWindow(client, req->windowId, wp);

	XCirculateSubwindows(clist[ client ]->dpy, req->windowId,
		up ? RaiseLowest : LowerHighest);
	if (up)
		RaiseLowestChild(wp);
	else
		LowerHighestChild(wp);
	ShowWinTree(wp->parent, 0);
}

X10ProcMoveWindow(client, req)
	int	client;
	X10Req	*req;
{
	GetWinInfoOrReturnBadWindow(client, req->windowId, wp);

	wp->x = req->param.s[0];
	wp->y = req->param.s[1];

	XMoveWindow(clist[ client ]->dpy, req->windowId,
		req->param.s[0], req->param.s[1]);
}

X10ProcChangeWindow(client, req)
	int	client;
	X10Req	*req;
{
	GetWinInfoOrReturnBadWindow(client, req->windowId, wp);

	if (wp->mapped) {
		if (wp->width != req->param.s[1]
		 || wp->height != req->param.s[0])
			wp->configured++;
	}
	wp->width = req->param.s[1];
	wp->height = req->param.s[0];

	XResizeWindow(clist[ client ]->dpy,
		req->windowId,
		(unsigned int) req->param.s[1],
		(unsigned int) req->param.s[0]);
}

X10ProcConfigureWindow(client, req)
	int	client;
	X10Req	*req;
{
	GetWinInfoOrReturnBadWindow(client, req->windowId, wp);

	if (wp->mapped) {
		if (wp->width != req->param.s[1]
		 || wp->height != req->param.s[0])
			wp->configured++;
	}
	wp->width = req->param.s[1];
	wp->height = req->param.s[0];
	wp->x = req->param.s[2];
	wp->y = req->param.s[3];

	XMoveResizeWindow(clist[ client ]->dpy,
		req->windowId,
		req->param.s[2],		/* x */
		req->param.s[3],		/* y */
		(unsigned int) req->param.s[1],	/* width */
		(unsigned int) req->param.s[0]);/* height */
}

X10ProcQueryColor(client, req, rep)
	int	client;
	X10Req	*req;
	X10Rep	*rep;
{
	Display		*dpy = clist[ client ]->dpy;
	XColor	pixel;
	int	result;

	/*
	 * Translate pixel value into rgb value.
	 */
	pixel.pixel = req->param.u[0];
	result = XQueryColors(dpy,
		DefaultColormap(dpy, DefaultScreen(dpy)),
		&pixel,
		1);
	if (result == 0) {
		Xstatus = X10BadValue;
		return(False);
	}
	rep->param.u[0] = pixel.red;
	rep->param.u[1] = pixel.green;
	rep->param.u[2] = pixel.blue;
	return (True);
}

/*ARGSUSED*/
X10ProcLookupColor(client, req, rep, name)
	int	client;
	X10Req	*req;
	X10Rep	*rep;
	char	*name;
{
	Display		*dpy = clist[ client ]->dpy;
	XColor	def, scr;
	int	result, nameLength = req->param.s[0];
	char	*buffer;

	buffer = StaticAlloc(nameLength + 1);
	bcopy(name, buffer, nameLength);
	buffer[ nameLength ] = '\0';

	debug(X10_LookupColor, ("(%s) ", buffer));
	/*
	 * Translate pixel value into rgb value.
	 */
	result = XLookupColor(dpy,
		DefaultColormap(dpy, DefaultScreen(dpy)),
		buffer, &def, &scr);
	if (result == 0) {
		Xstatus = X10BadColor;
		return(False);
	}
	rep->param.u[0] = def.red;
	rep->param.u[1] = def.green;
	rep->param.u[2] = def.blue;
	rep->param.u[3] = scr.red;
	rep->param.u[4] = scr.green;
	rep->param.u[5] = scr.blue;
	return (True);
}

X10ProcGetColor(client, req, rep)
	int	client;
	X10Req	*req;
	X10Rep	*rep;
{
	XColor	color;
	Display	*dpy = clist[ client ]->dpy;
	Status	status;

	color.red = req->param.u[0];
	color.green = req->param.u[1];
	color.blue = req->param.u[2];

	status = XAllocColor(dpy,
			     DefaultColormap(dpy,
			     DefaultScreen(dpy)),
			     &color);

	rep->param.u[0] = color.pixel;
	if (status == 0) {
		Xstatus = X10BadAlloc;
		return(False);
	}
	return(True);
}

X10ProcGetColorCells(client, req, rep, x10pixels)
	int	client;
	X10Req	*req;
	X10Rep	*rep;
	short	**x10pixels;
{
	Display	*dpy = clist[ client ]->dpy;
	unsigned int	ncolors = req->param.s[0],
			nplanes = req->param.s[1];
	unsigned long	*pixels,
			*planemasks,
			planemask;
	int	successful, i, class;

	/*
	 * XXX the default colormap must be of class PseudoColor.
	 * In which case, the masks we get back
	 * are exactly one bit apiece, and by or'ing them all
	 * we get the ``planemask'' that X10 knows.
	 */
	class = DefaultVisual(dpy, DefaultScreen(dpy))->class;
	if (class != PseudoColor) {
		debug(X10_GetColorCells, ("visual->class=%s\n",
			class == StaticGray ? "StaticGray" :
			  class == GrayScale ? "GrayScale" :
			    class == StaticColor ? "StaticColor" :
			      class == TrueColor ? "TrueColor" :
			        class == DirectColor ? "DirectColor" : "???"));
		Xstatus = BadAlloc;
		return(False);
	}
	pixels = (unsigned long *)StaticAlloc(
		sizeof(long)*(ncolors + nplanes) + sizeof(short)*ncolors);
	planemasks = pixels + ncolors;
	*x10pixels = (short *)(planemasks + nplanes);
	if (ncolors == 0)
		ncolors = DisplayCells(dpy, DefaultScreen(dpy));
	
	successful = XAllocColorCells(dpy, 
		DefaultColormap(dpy, DefaultScreen(dpy)),
		req->func,		/* continuous ? */
		planemasks,
		nplanes,
		pixels,
		ncolors);
	if (!successful) {
		Xstatus = BadAlloc;
		return(False);
	}
	for (i=planemask=0; i<nplanes; i++)
		planemask |= planemasks[ i ];
	for (i=0; i<ncolors; i++)
		(*x10pixels)[i] = pixels[i];
	rep->param.u[0] = planemask;
	return(True);
}

X10ProcFreeColors(client, req, x10pixels)
	int	client;
	X10Req	*req;
	short	*x10pixels;
{
	Display		*dpy = clist[ client ]->dpy;
	unsigned long	*pixels;
	int		npixels = req->param.s[0],
			i;

	pixels = (unsigned long *)StaticAlloc(sizeof(long)*npixels);
	for (i=0; i<npixels; i++)
		pixels[i] = x10pixels[i];
	XFreeColors(dpy,
		DefaultColormap(dpy, DefaultScreen(dpy)),
		pixels,
		npixels,
		req->mask);
}

X10ProcStoreColors(client, req, x10colors)
	int	client;
	X10Req	*req;
	X10ColorDef *x10colors;
{
	Display	*dpy = clist[ client ]->dpy;
	int	ncolors = req->param.s[0];
	int	i;
	XColor	*colors;

	colors = (XColor *)StaticAlloc(sizeof(XColor)*ncolors);
	/*
	 * XXX should we set the flags??
	 */
	for (i=0; i<ncolors; i++) {
		colors[i].pixel = x10colors[i].pixel;
		colors[i].red = x10colors[i].red;
		colors[i].green = x10colors[i].green;
		colors[i].blue = x10colors[i].blue;
	}
	XStoreColors(dpy, 
		DefaultColormap(dpy, DefaultScreen(dpy)),
		colors,
		ncolors);
}

X10ProcMouseControl(client, req)
	int	client;
	X10Req	*req;
{
	XChangePointerControl(clist[ client ]->dpy,
		True, True, req->param.s[0], 1, req->param.s[1]);
}


X10ProcFeepControl(client, req)
	int	client;
	X10Req	*req;
{
	XKeyboardControl	kc;

	kc.bell_percent = (req->func * 100) / 7;
	XChangeKeyboardControl(clist[ client ]->dpy, KBBellPercent, &kc);
}

X10ProcFeep(client, req)
	int	client;
	X10Req	*req;
{
	XBell(clist[ client ]->dpy, (int)((req->func * 100) / 7));
}

X10ProcShiftLock(client, req)
	int	client;
	X10Req	*req;
{
	switch (req->func) {
	case 0:	/* LockUpDownMode */
		LockToggleMode = False;
		break;
	case 1:	/* LockToggleMode */
		LockToggleMode = True;
		break;
	default:
		Xstatus = X10BadValue;
		break;
	}
}

X10ProcQueryShape(client, req, rep)
	int	client;
	X10Req	*req;
	X10Rep	*rep;
{
	Display	*dpy = clist[ client ]->dpy;
	unsigned int	width, height;
	int		class;

	switch (req->func) {
	case 0:	class = CursorShape; break;
	case 1:	class = TileShape; break;
	case 2:	rep->param.s[0] =  rep->param.s[0] = 1; return;
	}

	XQueryBestSize(dpy,
		class,
		DefaultRootWindow(dpy),
		(unsigned int) req->param.s[1],
		(unsigned int) req->param.s[0],
		&width,
		&height);

	rep->param.s[1] = width;
	rep->param.s[0] = height;
}

X10ProcChangeBackground(client, req)
	int	client;
	X10Req	*req;
{
	Pixmap	p = req->param.l[0];
	Window	w = req->windowId;
	GetWinInfoOrReturnBadWindow(client, w, wp);

	if (wp->transparent) {
		Xstatus = X10BadWindow;
		return;
	}

	/*
	 * XXX Note that if the X10 TileMode is TileModeRelative, we
	 * loose that distiction here, because X11 does not support
	 * a window background of ParentRelative plus having a uniq
	 * pixmap.
	 */
	if (p == 0)
		p = ParentRelative;
	else {
		PixmapInfoptr	pp = PixmapLookup(client, p);

		if (pp == NULL) {
			Xstatus = X10BadPixmap;
			return;
		}
	}
	wp->tile = p;

	XSetWindowBackgroundPixmap(clist[ client ]->dpy, w, p);
}

X10ProcChangeBorder(client, req)
	int	client;
	X10Req	*req;
{
	GetWinInfoOrReturnBadWindow(client, req->windowId, wp);

	XSetWindowBorderPixmap(clist[ client ]->dpy,
		req->windowId,
		(Pixmap) req->param.l[0]);
}

/*ARGSUSED*/
X10ProcTileMode(client, req)
	int	client;
	X10Req	*req;
{
	Display	*dpy = clist[ client ]->dpy;
	XSetWindowAttributes	swa;
	GC		gc;
	XGCValues	gcv;
	GetWinInfoOrReturnBadDrawable(client, req->windowId, wp);

	gc = wp->gc;

	/*
	 * XXX In X10, there were four combinations of tiles and modes
	 * that were possible:
	 *	Tile		Mode			X11
	 *	----		----			---
	 *	use parent's	Absolute		can't
	 *	use parent's	Relative to parent	can
	 *	uniqe tile	Absolute		can
	 *	uniqe tile	Relative to parent	can't
	 * In X11, we really can't do a complete job for the window
	 * background, but we can for X10 StippleFill and DrawFilled
	 * requests
	 */
	switch (req->func) {
	case 0: /* TileModeAbsolute */
		SetTileMode(dpy, wp, False);

		if (wp->tile == ParentRelative) {
			if (wp->parent->tile == ParentRelative) {
				Notice("Can't set tile mode to absolute!\n");
				break;
			} else
				wp->tile = wp->parent->tile;
		}
		swa.background_pixmap = wp->tile;
		XChangeWindowAttributes(dpy, req->windowId, CWBackPixmap, &swa);
		break;
		
	case 1: /* TileModeRelative */
		/*
		 * XXX For window-background tiles: We could look up
		 * the parent here
		 * and see if the tile was different than the childs...
		 * but then we would have to copy the tile into another
		 * tile changing the origin.  This is too much trouble for
		 * the value right now.  If the parent is the root, we can
		 * never be sure if we know what its tile is, because we can't
		 * query the tile.
		 * So for now, either the tile is already ParentRelative
		 * (so we do nothing), or the tile is unique (which we 
		 * can't handle, so we still do nothing.
		 */
		SetTileMode(dpy, wp, True);
		break;
		
	default:
		Xstatus = X10BadValue;
		return;
	}
}

X10ProcClipMode(client, req)
	int	client;
	X10Req	*req;
{
	Display	*dpy = clist[ client ]->dpy;
	GC		gc;
	XGCValues	gcv;
	GetWinInfoOrReturnBadDrawable(client, req->windowId, wp);

	gc = wp->gc;

	if (req->func == 0)
		SetClipMode(dpy, wp, False);
	else if (req->func == 1)
		SetClipMode(dpy, wp, True);
	else {
		Xstatus = X10BadValue;
		return;
	}
}

X10ProcSetIconWindow(client, req)
	int	client;
	X10Req	*req;
{
	Window	icon = req->param.l[0];

	XChangeProperty(clist[ client ]->dpy,
		req->windowId,
		XA_WM_ICON_NAME,
		XA_WINDOW,
		32,
		PropModeReplace,
		&icon,
		1);
}

X10ProcInterpretLocator(client, x, y, req, rep)
	int	client, x, y;
	X10Req	*req;
	X10Rep	*rep;
{
	Window	child;
	int	rel_x, rel_y;

	XTranslateCoordinates(clist[ client ]->dpy,
		req->windowId,
		req->windowId,
		x, y,
		&rel_x, &rel_y,
		&child);
	rep->param.l[0] = child;
	rep->param.s[2] = rel_x;
	rep->param.s[3] = rel_y;
}

X10ProcWarpMouse(client, req)
	int	client;
	X10Req	*req;
{
	XWarpPointer(clist[ client ]->dpy,
		req->param.l[1],	/* src window */
		req->windowId,		/* dest window */
		req->param.s[6],	/* src x */
		req->param.s[7],	/* src y */
		req->param.s[5],	/* src width */
		req->param.s[4],	/* src height */
		req->param.s[0],	/* dest x */
		req->param.s[1]);	/* dest y */
}

X10ProcFocusKeyboard(client, req)
	int	client;
	X10Req	*req;
{
	XSetInputFocus(clist[ client ]->dpy,
		req->windowId,
		PointerRoot,
		CurrentTime);
}

X10ProcQueryTree(client, req, rep, nchildren, children)
	int	client;
	X10Req	*req;
	X10Rep	*rep;
	int	*nchildren;
	Window	**children;
{
	Window	root, parent, win = req->windowId;
	int	successful;
	GetWinInfoOrReturnBadWindow(client, win, wp);

	successful = XQueryTree(clist[ client ]->dpy,
		win,
		&root,
		&parent,
		children,
		nchildren);
	if (! successful) {
		Xstatus = X10BadWindow;
		return(False);
	}
	UpdateWinTree(client, wp, *nchildren, *children);
	ShowWinTree(wp->parent, 0);
	rep->param.l[0] = parent;
	rep->param.l[1] = *nchildren;
	return(True);
}

X10ProcCopyArea(client, req)
	int	client;
	X10Req	*req;
{
	Clientptr	cp = clist[ client ];
	GC		gc;
	Display		*dpy = cp->dpy;
	XGCValues	gcv;
	int		x = req->param.s[6],
			y = req->param.s[7];
	Drawable	d = req->windowId;
	GetWinInfoOrReturnBadDrawable(client, d, wp);

	gc = wp->gc;

	gcv.function = X10toX11function(req->func);
	gcv.plane_mask = req->mask;
	XChangeGC(dpy, gc, GCFunction|GCPlaneMask, &gcv);

	CheckTransparentDrawableAndXY(dpy, wp, &d, &x, &y);
	XCopyArea(dpy,
		d,			/* source drawable */
		d,			/* dest drawable */
		gc,			/* gc */
		req->param.s[2],	/* src x */
		req->param.s[3],	/* src y */
		req->param.s[1],	/* width */
		req->param.s[0],	/* height */
		x,			/* dest x */
		y);			/* dest y */
}
