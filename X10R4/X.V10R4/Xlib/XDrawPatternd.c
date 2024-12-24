#include <X/mit-copyright.h>

/* $Header: XDrawPatternd.c,v 10.7 86/11/06 17:47:38 jg Rel $ */
/* Copyright    Massachusetts Institute of Technology    1985	*/

#include "XlibInternal.h"
XDrawPatterned (w, vlist, vcount, width, height, pixel, altpix, pattern,
    func, planes)
	Window w;
	int func;
	int pixel, altpix, planes, height, width, vcount;
	Pattern pattern;
	Vertex *vlist;
{
	register Display *dpy;
	register XReq *req;
	int nbytes;

	GetReq(X_Draw, w);
	dpy->lastdraw = (caddr_t) req;
	req->func = func;
	req->mask = planes;
	req->params0 = vcount;
	req->paramu1 = pixel;
	req->param.b[4] = height;
	req->param.b[5] = width;
	req->params3 = DrawPatternedLine;
	req->paramu4 = altpix;
	req->params5 = pattern & 0xffff;  /* pattern string */
	req->params6 = ((pattern & 0xf0000) >> 16) + 1;  /* pattern length */
	req->params7 = (pattern & 0xfff00000) >> 20;  /* pattern multiplier */
	nbytes = vcount*psizeof (Vertex);
	PackData(dpy, vlist, nbytes);
}

