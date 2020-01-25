#include "copyright.h"

/* $Header: XDrArc.c,v 11.11 87/12/08 13:28:22 newman Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

/* Note to future maintainers:  XDrawArc does NOT batch successive PolyArc
   requests into a single request like XDrawLine, XDrawPoint, etc.
   We don't do this because X_PolyArc applies the GC's join-style if
   the last point in one arc coincides with the first point in another.
   The client wouldn't expect this and would have no easy way to defeat it. */
   
#include "Xlibint.h"

XDrawArc(dpy, d, gc, x, y, width, height, angle1, angle2)
    register Display *dpy;
    Drawable d;
    GC gc;
    int x, y; /* INT16 */
    unsigned int width, height; /* CARD16 */
    int angle1, angle2; /* INT16 */
{
    register xArc *arc;
    register xPolyArcReq *req;

    LockDisplay(dpy);
    FlushGC(dpy, gc);
    GetReqExtra (PolyArc, sizeof(xArc), req);

    req->drawable = d;
    req->gc = gc->gid;

    arc = (xArc *) (req + 1);
    arc->x = x;
    arc->y = y;
    arc->width = width;
    arc->height = height;
    arc->angle1 = angle1;
    arc->angle2 = angle2;

    UnlockDisplay(dpy);
    SyncHandle();
}
