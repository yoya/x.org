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
 * $Header: window.c,v 4.0 88/01/27 16:54:48 toddb Exp $ 
 * $Locker:  $ 
 */
#include "X11/Xlib.h"
#include "resource.h"

/*
 * Allocate a GC for an existing window.
 */
static Bool AllocWinGC(dpy, wp)
	Display		*dpy;
	WinInfoptr	wp;
{
	XGCValues	gcv;
	GC		gc;

	gc = XCreateGC(dpy,
		wp->gcWin,
		(u_long)0,
		(XGCValues *)NULL);
	if (gc == NULL)
		return(False);

	wp->gc = gc;

	/*
	 * By default in X10, the root window is created with a ClipMode
	 * of ClipModeDrawThru.  Other windows are created with mode
	 * ClipModeClipped which is equivalent to the X11 subwindow-mode
	 * of ClipByChildren.  So this is the only place where we
	 * must do something special to this window's gc.
	 */ 
	if (wp->id == DefaultRootWindow(dpy))
		SetClipMode(dpy, wp, True);
	return(True);
}

/*
 * This routine recurses UP the tree looking for the parent of the
 * current window.  When it has found a parent that will adopt all
 * the children found en route, it unwinds, updating the tree in
 * the process.
 */
static WinInfoptr FindForiegnWinTree(client, win, winNeeded)
	int	client;
	Window	win, winNeeded;
{
	Clientptr	cp = clist[ client ];
	WinInfoptr	parentwp, wp, FindWinTree();
	Window	root, parent, *children, nchildren;
	Status	successful;
	Bool	updated;

	successful = XQueryTree(clist[ client ]->dpy,
		win,
		&root,
		&parent,
		&children,
		&nchildren);

	if (! successful) {
		if (winNeeded == win)
			return(NULL); /* it really doesn't exist */
		Error("Orphaned window=0x%x!\n", win);
	}
	if (nchildren == 0 && winNeeded != win)
		Error("0 children for window=0x%x, ancestor of 0x%x?\n",
			win, winNeeded);

	parentwp = FindWinTree(cp->win, parent);
	if (parentwp == NULL) {
		parentwp = FindForiegnWinTree(client, parent, winNeeded);
		wp = FindWinTree(cp->win, win); /* must return non-null */
	} else {
		wp = AddWin(client, win, parentwp, False, False);
		wp->foriegn = True;
	}
	UpdateWinTree(client, wp, nchildren, children);
	if (nchildren)
		free(children);
	return(wp);
}

static WinInfoptr FindWinTree(wp, win)
	WinInfoptr	wp;
	Window		win;
{
	WinInfoptr	child;

	if (wp == NULL)
		return(NULL);

	while (wp) {
		if (win == wp->id)
			return(wp);
		child = FindWinTree(wp->child, win);
		if (child)
			return(child);
		wp = wp->sibling;
	}
	return(NULL);
}

/*
 * This routine is called ONLY if you know that your window is transparent
 * and you want the opaque ancestor.  All windows searched are marked
 * as having a transparent descendent.
 */
static Window FindGCWin(wp)
	WinInfoptr	wp;
{
	if (wp == NULL)
		Error("FindGCWin: wp==NULL\n");
	wp->hasTransparency = True;
	while (wp->transparent) {
		wp = wp->parent;
		wp->hasTransparency = True;
	}
	return (wp->id);
}

static Bool FillInWinInfo(dpy, wp)
	Display		*dpy;
	WinInfoptr	wp;
{
	XWindowAttributes	att;

	/*
	 * The window may have disappeared.
	 * It will be cleaned up later.
	 */
	if (! XGetWindowAttributes(dpy, wp->id, &att))
		return(False);
	wp->transparent = (att.class == InputOnly);
	wp->x = att.x;
	wp->y = att.y;
	wp->width = att.width;
	wp->height = att.height;
	wp->borderWidth = att.border_width;
	wp->gcWin = (wp->transparent ? FindGCWin(wp->parent) : wp->id);
	/* XXX don't know tile or mask */
	return(True);
}
	
/*
 * Called by those needing window info.
 */
WinInfoptr WinLookup(client, w, GCneeded)
	int	client;
	Window	w;
	Bool	GCneeded;
{
	Clientptr	cp = clist[ client ];
	WinInfoptr	wp;

	wp = FindWinTree(cp->win, w);
	if (wp == NULL)
		wp = FindForiegnWinTree(client, w, w);
	if (wp == NULL)
		return(NULL);
	if (wp->gc || ! GCneeded)
		return(wp);

	/*
	 * We don't know about this window, but we need a gc...
	 */
	if (wp->foriegn && !FillInWinInfo(cp->dpy, wp)) {
		/*
		 * The window may have disappeared.
		 * It will be cleaned up later.
		 */
		return(NULL);
	}

	/*
	 * We only allocate a gc when the window is referenced and needs
	 * a gc.
	 */
	if (! AllocWinGC(cp->dpy, wp))
		return(NULL);
	return(wp);
}

static void FreeWinTree(dpy, wp)
	Display		*dpy;
	WinInfoptr	wp;
{
	WinInfoptr	next;

	if (wp == NULL)
		return;
	while (wp) {
		next = wp->sibling;
		FreeWinTree(dpy, wp->child);
		if (wp->gc)
			XFreeGC(dpy, wp->gc);
		if (wp->savedPixmap)
			XFreePixmap(dpy, wp->savedPixmap);
		free(wp);
		wp = next;
	}
}

static void DeleteWinInfo(winfo)
	WinInfoptr	winfo;
{
	WinInfoptr wp, parentwp = winfo->parent;

	/*
	 * don't bother if this is the top.
	 */
	if (parentwp == NULL)
		return;

	/*
	 * Fix up the pointers in the tree.
	 */
	if (parentwp->child == winfo)
		parentwp->child = winfo->sibling;
	else {
		for (wp = parentwp->child; wp; wp = wp->sibling)
			if (wp->sibling == winfo) {
				wp->sibling = winfo->sibling;
				break;
			}
	}
	/*
	 * Isolate its reference pointers.
	 */
	winfo->sibling = NULL;
}

FreeWinInfo(client, winfo)
	int	client;
	WinInfoptr	winfo;
{
	Clientptr cp = clist[ client ];

	DeleteWinInfo(winfo);
	FreeWinTree(cp->dpy, winfo);
}

WinInfoptr AddWin(client, win, parent, isTransparent, GCneeded)
	int		client;
	Window		win;
	WinInfoptr	parent;
	Bool		isTransparent,
			GCneeded;
{
	Clientptr cp = clist[ client ];
	WinInfoptr wp, wpnew;

	wpnew = (WinInfoptr)Xalloc(sizeof(WinInfo));
	if (wpnew == NULL)
		return(NULL);

	bzero(wpnew, sizeof(WinInfo));

	wpnew->gcWin = (isTransparent ? FindGCWin(parent) : win);
	if (GCneeded && ! AllocWinGC(cp->dpy, wpnew)) {
		free(wpnew);;
		return(NULL);
	}
	wpnew->parent = parent;
	wpnew->id = win;
	wpnew->transparent = isTransparent;

	/*
	 * Insert this window in the tree of windows for this client.
	 */
	if (parent == NULL)
		wp = cp->win;
	else
		wp = parent;
	
	if (wp) {
		if (wp->child == NULL)
			wp->child = wpnew;
		else {
			wp = wp->child;
			while (wp->sibling)
				wp = wp->sibling;
			wp->sibling = wpnew;
		}
	} else
		cp->win = wpnew;

	return(wpnew);
}

MarkMapped(client, win, mapped)
	int	client;
	Window	win;
	int	mapped;
{
	WinInfoptr wp = WinLookup(client, win, False);
	if (wp == NULL)
		return(False);
	wp->mapped = mapped;
	if (mapped)
		RaiseWindow(wp);
	else if (win == DefaultRootWindow(clist[ client ]->dpy))
		wp->mapped = True;
	return(True);
}

MarkMappedTree(client, win, mapped)
	int	client;
	Window	win;
	int	mapped;
{
	WinInfoptr wp = WinLookup(client, win, False);
	if (wp == NULL)
		return(False);
	MarkMappedWinTree(wp, mapped);
	if (!mapped && win == DefaultRootWindow(clist[ client ]->dpy))
		wp->mapped = True;
	return(True);
}

MarkMappedWinTree(wp, mapped)
	WinInfoptr wp;
	int	mapped;
{
	if (wp == NULL)
		return;
	while (wp) {
		if (mapped)
			RaiseWindow(wp);
		wp->mapped = mapped;
		MarkMappedWinTree(wp->child);
		wp = wp->sibling;
	}
}

/*
 * Raise a window with respect to its siblings.  We do this
 * for the sake of transparent windows so that we may know what sibling
 * windows are above others in stacking order.  From this we can discover
 * which siblings occlude the transparent window and set the clip mask in
 * the gc accordingly.
 */
RaiseWindow(wp)
	WinInfoptr wp;
{
	WinInfoptr first, next;

	if (wp->parent == NULL)
		return;	/* doesn't matter... we don't own the root */

	first = wp->parent->child;
	if (first == wp)
		return; /* nothing more to do */

	next = first;
	while (next->sibling != NULL) {
		if (next->sibling == wp) {
			next->sibling = wp->sibling;
			wp->sibling = first;
			wp->parent->child = wp;
			return;
		}
		next = next->sibling;
	}
	Error("RaiseWindow: wp missing!\n");
}

/*
 * Lower a window with respect to its siblings.
 */
LowerWindow(wp)
	WinInfoptr wp;
{
	WinInfoptr next;

	if (wp->parent == NULL)
		return;	/* doesn't matter... we don't own the root */

	if (wp->sibling == NULL)
		return; /* nothing more to do */

	/*
	 * find it in the list, and remove it...
	 */
	next = wp->parent->child;
	if (next == wp) {
		wp->parent->child = wp->sibling;
		next = wp->sibling;
	} else {
		while (next->sibling != NULL && next->sibling != wp)
			next = next->sibling;
		if (next->sibling == NULL)
			Error("LowerWindow: wp missing!\n");
		next->sibling = wp->sibling;
	}

	/*
	 * Find the end of the list and stick it on.
	 */
	while (next->sibling != NULL)
		next = next->sibling;
	next->sibling = wp;
	wp->sibling = NULL;
}

/*
 * Lower the highest mapped child window with respect to its siblings.
 */
LowerHighestChild(parent)
	WinInfoptr parent;
{
	WinInfoptr next, first, prev;
	Bool	foundNextMapped;

	if (parent->child == NULL)
		return; /* nothing more to do */

	/*
	 * find the window just before the first mapped child...
	 */
	next = parent->child;
	if (next->mapped) {
		first = next;
		prev = NULL;
	} else {
		while (next->sibling != NULL && ! next->sibling->mapped)
			next = next->sibling;
		prev = next;
		first = next->sibling;
	}
	if (first->sibling == NULL)
		return;	/* already last on the list */

	/*
	 * Find the end of the list and stick it on.
	 */
	foundNextMapped = False;
	next = first;
	while (next->sibling != NULL) {
		next = next->sibling;
		if (next->mapped)
			foundNextMapped = True;
	}
	if (! foundNextMapped)
		return;	/* only one window mapped */
	if (prev == NULL)
		parent->child = first->sibling;
	else
		prev->sibling = first->sibling;
	next->sibling = first;
	first->sibling = NULL;
}

/*
 * Raise the lowest mapped child window with respect to its siblings.
 */
RaiseLowestChild(parent)
	WinInfoptr parent;
{
	WinInfoptr next, last, prev;
	int	mappedCount;

	if (parent->child == NULL)
		return; /* nothing more to do */

	/*
	 * find the window just before the last mapped child...
	 */
	next = parent->child;
	mappedCount = 0;
	while (next != NULL) {
		if (next->mapped)
			mappedCount++;
		if (next->sibling->mapped)
			prev = next;
		next = next->sibling;
	}
	if (mappedCount <= 1)
		return;	/* only one mapped child */
	/* if mappedCount > 1, then we know prev is non-null */

	last = prev->sibling;
	prev->sibling = last->sibling;
	parent->child = last;
}

/*
 * modify a transparent drawable, x and y so the drawing actually
 * happens correctly on an InputOutput window.
 */
ChangeXYAndClipAndDrawable(dpy, wp, dptr, xptr, yptr)
	Display		*dpy;
	WinInfoptr	wp;
	Drawable	*dptr;
	int		*xptr, *yptr;	/* x, y */
{
	Drawable	drawble;
	XRectangle	clip;
	WinInfoptr	next;

	/*
	 * First, find a drawble that is suitable for graphic requests.
	 * As we move upward, modify x and y to continue to point 
	 * to the same place.
	 */
	clip.x = 0;
	clip.y = 0;
	for (next = wp; next; next = next->parent) {
		clip.x += wp->x;
		clip.y += wp->y;
		if (! next->transparent)
			break;
	}
	if (next == NULL)
		Error("ChangeXYAndClipAndWindow: missing parent drawable!\n");
	*dptr = next->id;
	*xptr += clip.x;
	*yptr += clip.y;

	/*
	 * now set the clipping rectangle.
	 */
	clip.width = wp->width;
	clip.height = wp->height;

	if (wp->gc != NULL)
		XSetClipRectangles(
			dpy,
			wp->gc,
			0, 0,			/* x, y clip origin */
			&clip,			/* clip rectangle */
			1,			/* number of rectangles */
			Unsorted);		/* rectangle ordering */
}

void UpdateWinTree(client, parent, nchildren, children)
	int		client;
	WinInfoptr	parent;
	int		nchildren,
			*children;
{
	Display		*dpy = clist[ client ]->dpy;
	WinInfoptr	wp, new;
	int		i;
	Window		child;

	/*
	 * children are returned from a QueryTree in bottom-to-top order.
	 */
	for (i = nchildren - 1; i >= 0; i--) {
		child = children[ i ];
		/*
		 * See if we already know about this one.
		 */
		wp = FindWinTree(parent, child);
		if (wp != NULL) {
			/*
			 * XXX is this worth fixing?
			 */
			if (wp->parent != parent)
				Error("win %x has moved (reparented?)\n",
					wp->id);
			continue;
		}
		new = AddWin(client, child, parent, False, False);
		new->foriegn = True;
	}
}

/*
 * Change the tile mode of a window to either relative or absolute.
 */
void SetTileMode(dpy, wp, relative)
	Display		*dpy;
	WinInfoptr	wp;
	int		relative;
{
	XGCValues	gcv;

	if (relative) {
		gcv.ts_x_origin = - wp->x;
		gcv.ts_y_origin = - wp->y;
		wp->tileModeRelative = True;
	} else {
		gcv.ts_x_origin = 0;
		gcv.ts_y_origin = 0;
		wp->tileModeRelative = False;
	}
	XChangeGC(dpy,
		wp->gc,
		GCTileStipXOrigin|GCTileStipYOrigin,
		&gcv);
}

void SetClipMode(dpy, wp, drawThrough)
	Display		*dpy;
	WinInfoptr	wp;
	int		drawThrough;
{
	XGCValues	gcv;

	if (drawThrough == wp->drawThrough)
		return;

	if (drawThrough)
		gcv.subwindow_mode = IncludeInferiors;
	else
		gcv.subwindow_mode = ClipByChildren;
	wp->drawThrough = drawThrough;
	XChangeGC(dpy,
		wp->gc,
		GCSubwindowMode,
		&gcv);
}
