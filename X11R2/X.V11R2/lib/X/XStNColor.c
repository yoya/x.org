#include "copyright.h"

/* $Header: XStNColor.c,v 11.9 87/10/30 00:00:23 newman Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

XStoreNamedColor(dpy, cmap, name, pixel, flags)
register Display *dpy;
Colormap cmap;
unsigned long pixel; /* CARD32 */
char *name; /* STRING8 */
int flags;  /* DoRed, DoGreen, DoBlue */
{
    unsigned int nbytes;
    register xStoreNamedColorReq *req;

    LockDisplay(dpy);
    GetReq(StoreNamedColor, req);

    req->cmap = cmap;
    req->flags = flags;
    req->pixel = pixel;
    req->nbytes = nbytes = name ? strlen(name) : 0;
    req->length += (nbytes + 3) >> 2; /* round up to multiple of 4 */
    Data(dpy, name, (long)nbytes);
    UnlockDisplay(dpy);
    SyncHandle();
}


