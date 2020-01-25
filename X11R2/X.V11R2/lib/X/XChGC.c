#include "copyright.h"

/* $Header: XChGC.c,v 11.7 88/01/30 14:09:24 rws Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

XChangeGC (dpy, gc, valuemask, values)
    register Display *dpy;
    GC gc;
    unsigned long valuemask;
    XGCValues *values;
{
    LockDisplay(dpy);

    valuemask &= (1L << (GCLastBit + 1)) - 1;
    if (valuemask) _XUpdateGCCache (gc, valuemask, values);

    /* if any Resource ID changed, must flush */
    if (valuemask & (GCFont | GCTile | GCStipple)) _XFlushGCCache(dpy, gc);
    UnlockDisplay(dpy);
    SyncHandle();
}

