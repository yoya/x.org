#include "copyright.h"

/* $Header: XQuTextE16.c,v 11.8 88/01/30 13:50:43 rws Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986, 1987	*/

#define NEED_REPLIES
#include "Xlibint.h"

XQueryTextExtents16 (dpy, fid, string, nchars, dir, font_ascent, font_descent,
                     overall)
    register Display *dpy;
    Font fid;
    XChar2b *string;
    register int nchars;
    int *dir;
    int *font_ascent, *font_descent;
    register XCharStruct *overall;
{
    register long i;
    register unsigned char *ptr;
    char *buf;
    xQueryTextExtentsReply rep;
    long nbytes;
    register xQueryTextExtentsReq *req;

    LockDisplay(dpy);
    GetReq(QueryTextExtents, req);
    req->fid = fid;
    nbytes = nchars << 1;
    req->length += (nbytes + 3)>>2;
    req->oddLength = nchars & 1;
    buf = _XAllocScratch (dpy, (unsigned long)nbytes);
    for (ptr = (unsigned char *)buf, i = nchars; --i >= 0; string++) {
	*ptr++ = string->byte1;
	*ptr++ = string->byte2;
    }
    Data (dpy, buf, nbytes);
    if (!_XReply (dpy, (xReply *)&rep, 0, xTrue))
	return (0);
    *dir = rep.drawDirection;
    *font_ascent = rep.fontAscent;
    *font_descent = rep.fontDescent;
    overall->ascent = rep.overallAscent;
    overall->descent = rep.overallDescent;
    overall->width  = rep.overallWidth;
    overall->lbearing = rep.overallLeft;
    overall->rbearing = rep.overallRight;
    UnlockDisplay(dpy);
    SyncHandle();
    return (1);
}

