#include "copyright.h"

/* $Header: XFreePix.c,v 11.5 87/09/11 08:03:47 toddb Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

XFreePixmap(dpy, pixmap)
    register Display *dpy;
    Pixmap pixmap;

{   
    register xResourceReq *req;    
    LockDisplay(dpy);
    GetResReq(FreePixmap, pixmap, req);
    UnlockDisplay(dpy);
    SyncHandle();
}

