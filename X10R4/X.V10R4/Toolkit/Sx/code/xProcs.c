/*
 *	$Source: /u1/Sx.new/code/RCS/xProcs.c,v $
 *	$Header: xProcs.c,v 1.1 86/12/03 16:11:21 swick Exp $
 */

#ifndef lint
static char *rcsid_xProcs_c = "$Header: xProcs.c,v 1.1 86/12/03 16:11:21 swick Exp $";
#endif	lint

/* 
 * xProcs.c --
 *
 *	This file contains slightly-modified versions of several
 *	of the X library procedures.  They are modified to call
 *	Sx routines that keep track of window creations and
 *	deletions.  It is important that they are all together
 *	in one file:  this guarantees that they all get loaded from
 *	here.  If they're in separate files, some might not appear
 *	to be necessary when the Sx library is scanned, but might
 *	be needed by other X procedures not included here.  The
 *	result is that the original X procedures would get grabbed
 *	from the X Library.
 *
 */

#ifndef lint
static char rcsid[] = "$Header: xProcs.c,v 1.1 86/12/03 16:11:21 swick Exp $ SPRITE (Berkeley)";
#endif not lint

#include <X/mit-copyright.h>
#include "XlibInternal.h"
#include "sxInt.h"

int XCreateTransparencies (parent, defs, ndefs)
	Window parent;
	TransparentFrame defs[];
	int ndefs;
{
	register Display *dpy;
	register int i;
	register TransparentFrame *frame = defs;
	int nresult = 0;

	for (i=0;i<ndefs;i++) {
	    register XReq *req;
	    GetReq(X_CreateTransparency, parent);
	    req->param.s[0] = frame->height;
	    req->param.s[1] = frame->width;
	    req->param.s[2] = frame->x;
	    req->param.s[3] = (frame++)->y;
	    }

	/* Reset request number to its old value, so that
	    error packets are processed correctly.  */
	dpy->request -= ndefs;

	frame = defs;
	for (i=0;i<ndefs;i++) {
	    XRep rep;
	    /* Increment request number so error packets
	       are processed correctly. */
	    dpy->request++;
	    if (!_XReply(dpy, &rep)) {
	    	frame->self = NULL;
		frame++;
	    } else {
		frame->self = rep.param.l[0];
		frame++;
		SxWindowCreated(parent, rep.param.l[0]);
		nresult++;
		}
	    }
	return (nresult);
}

Window XCreateTransparency (parent, x, y, width, height)
	int x, y, width, height;
	Window parent;
{
	register Display *dpy;
	register XReq *req;
	XRep rep;

	GetReq(X_CreateTransparency, parent);
	req->param.s[0] = height;
	req->param.s[1] = width;
	req->param.s[2] = x;
	req->param.s[3] = y;
	if (!_XReply(dpy, &rep)) {
	    return(NULL);
	}
	SxWindowCreated(parent, rep.param.l[0]);
	return (rep.param.l[0]);
}

int XCreateWindowBatch(defs, ndefs)
	BatchFrame defs[];
	int ndefs;
{
	register Display *dpy;
	register int i;
	register BatchFrame *frame = defs;
	int nresult = 0;

	/*
	 * Issue requests.
	 */
	for (i=0; i < ndefs; i++) {
	    register XReq *req;
	    if (frame->type == IsOpaque) {
		GetReq(X_CreateWindow, frame->parent);
		req->param.s[0] = frame->height;
		req->param.s[1] = frame->width;
		req->param.s[2] = frame->x;
		req->param.s[3] = frame->y;
		req->param.l[2] = frame->border;
		req->param.l[3] = frame->background;
		req->func = (frame++)->bdrwidth;
	    }
	    else {
		GetReq(X_CreateTransparency, frame->parent);
		req->param.s[0] = frame->height;
		req->param.s[1] = frame->width;
		req->param.s[2] = frame->x;
		req->param.s[3] = (frame++)->y;
	    }
	}
	
	/*
	 * Reset request number to its old value, so that
	 * error packets are processed correctly.
	 */
	dpy->request -= ndefs;

	/*
	 * Retrieve replies.
	 */
	frame = defs;
	for (i=0;i<ndefs;i++) {
	    XRep rep;
	    /*
	     * Increment request number so error packets
	     * are processed correctly.
	     */
	    dpy->request++;
	    if (!_XReply(dpy, &rep)) {
		(frame++)->self = NULL;
	    } else {
		SxWindowCreated(frame->parent, rep.param.l[0]);
		(frame++)->self = rep.param.l[0];
		nresult++;
	    }
	}
	return (nresult);
}

Window XCreateWindow (parent, x, y, width, height, bdr_width, border, bgnd)
	int x, y, width, height, bdr_width;
	Window parent;
	Pixmap border, bgnd;
{
	register Display *dpy;
	register XReq *req;
	XRep rep;

	GetReq(X_CreateWindow, parent);
	req->param.s[0] = height;
	req->param.s[1] = width;
	req->param.s[2] = x;
	req->param.s[3] = y;
	req->param.l[2] = border;
	req->param.l[3] = bgnd;
	req->func = bdr_width;
	if (!_XReply(dpy, &rep)) {
	    return(NULL);
	}
	SxWindowCreated(parent, rep.param.l[0]);
	return (rep.param.l[0]);
}

int XCreateWindows (parent, defs, ndefs)
	Window parent;
	OpaqueFrame defs[];
	int ndefs;
{
	register Display *dpy;
	register int i;
	register OpaqueFrame *frame = defs;
	int nresult = 0;

	for (i=0;i<ndefs;i++) {
	    register XReq *req;
	    GetReq(X_CreateWindow, parent);
	    req->param.s[0] = frame->height;
	    req->param.s[1] = frame->width;
	    req->param.s[2] = frame->x;
	    req->param.s[3] = frame->y;
	    req->param.l[2] = frame->border;
	    req->param.l[3] = frame->background;
	    req->func = (frame++)->bdrwidth;
	    }
	
	/* Reset request number to its old value, so that
	    error packets are processed correctly.  */
	dpy->request -= ndefs;

	frame = defs;
	for (i=0;i<ndefs;i++) {
	    XRep rep;
	    /* Increment request number so error packets
	       are processed correctly. */
	    dpy->request++;
	    if (!_XReply(dpy, &rep)) {
	    	frame->self = NULL;
		frame++;
	    } else {
		SxWindowCreated(parent, rep.param.l[0]);
		(frame++)->self = rep.param.l[0];
		nresult++;
		}
	    }
	return (nresult);
}

XDestroySubwindows (w)
	Window w;
{
	register Display *dpy;
	register XReq *req;

	SxSubwindowsDestroyed(w);
	GetReq(X_DestroySubwindows, w);
}

XDestroyWindow (w)
	Window w;
{
	register Display *dpy;
	register XReq *req;

	SxWindowDestroyed(w);
	GetReq(X_DestroyWindow, w);
}
