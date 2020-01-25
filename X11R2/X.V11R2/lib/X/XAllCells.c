#include "copyright.h"

/* $Header: XAllCells.c,v 11.15 88/02/07 11:10:59 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/
#define NEED_REPLIES

#include "Xlibint.h"

Status XAllocColorCells(dpy, cmap, contig, masks, nplanes, pixels, ncolors)
register Display *dpy;
Colormap cmap;
Bool contig;
unsigned int ncolors; /* CARD16 */
unsigned int nplanes; /* CARD16 */
unsigned long masks[]; /* LISTofCARD32 */ /* RETURN */
unsigned long pixels[]; /* LISTofCARD32 */ /* RETURN */
{

    Status status;
    xAllocColorCellsReply rep;
    register xAllocColorCellsReq *req;
    LockDisplay(dpy);
    GetReq(AllocColorCells, req);

    req->cmap = cmap;
    req->colors = ncolors;
    req->planes = nplanes;
    req->contiguous = contig;

    status = _XReply(dpy, (xReply *)&rep, 0, xFalse);

    if (status) {
	_XRead (dpy, (char *) pixels, 4L * (long) (rep.nPixels));
	_XRead (dpy, (char *) masks, 4L * (long) (rep.nMasks));
    }

    UnlockDisplay(dpy);
    SyncHandle();
    return(status);
}
