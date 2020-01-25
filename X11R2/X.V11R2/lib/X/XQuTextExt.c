#include "copyright.h"

/* $Header: XQuTextExt.c,v 11.10 88/01/30 13:49:25 rws Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986, 1987	*/

#define NEED_REPLIES
#include "Xlibint.h"

XQueryTextExtents (dpy, fid, string, nchars, dir, font_ascent, font_descent,
                   overall)
    register Display *dpy;
    Font fid;
    register char *string;
    register int nchars;
    int *dir;
    int *font_ascent, *font_descent;
    register XCharStruct *overall;
{
    register int i;
    register char *ptr;
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
    for (ptr = buf, i = nchars; --i >= 0;) {
	*ptr++ = 0;
	*ptr++ = *string++;
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
