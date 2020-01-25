/* $XConsortium: gfx.c,v 1.13 94/12/01 20:55:35 mor Exp $ */
/*
 * Copyright 1994 Network Computing Devices, Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software and
 * its documentation for any purpose is hereby granted without fee, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name Network Computing Devices, Inc. not be
 * used in advertising or publicity pertaining to distribution of this
 * software without specific, written prior permission.
 *
 * THIS SOFTWARE IS PROVIDED `AS-IS'.  NETWORK COMPUTING DEVICES, INC.,
 * DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING WITHOUT
 * LIMITATION ALL IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE, OR NONINFRINGEMENT.  IN NO EVENT SHALL NETWORK
 * COMPUTING DEVICES, INC., BE LIABLE FOR ANY DAMAGES WHATSOEVER, INCLUDING
 * SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES, INCLUDING LOSS OF USE, DATA,
 * OR PROFITS, EVEN IF ADVISED OF THE POSSIBILITY THEREOF, AND REGARDLESS OF
 * WHETHER IN AN ACTION IN CONTRACT, TORT OR NEGLIGENCE, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * $NCDId: @(#)gfx.c,v 1.9 1994/11/29 19:56:39 lemke Exp $
 */
/*
 * graphics acceleration
 */

#include	<stdio.h>
#define NEED_REPLIES
#define NEED_EVENTS
#include	<X11/X.h>	/* for KeymapNotify */
#include	<X11/Xproto.h>
#include	"assert.h"
#include	"misc.h"
#include	"lbxdata.h"
#include	"util.h"
#include	"lbx.h"		/* gets dixstruct.h */
#include	"resource.h"
#include	"wire.h"
#define _XLBX_SERVER_
#include	"lbxstr.h"	/* gets dixstruct.h */
#include        "lbximage.h"

static int  pad[4] = {0, 3, 2, 1};

/*
 * Routines for re-encoding line, rectangle and arc requests
 */

#define REENCODE_SHORT(val, out) \
    if ((val) >= (short)0x0800 || (val) < (short)0xf800) \
	return 0; \
    if ((val) <= -113 || (val) >= 128) \
	*(out)++ = 0x80 | (((val) >> 8) & 0x0f); \
    *(out)++ = (val) & 0xff;

#define REENCODE_USHORT(val, out) \
    if ((unsigned short)(val) >= 0x1000) \
	return 0; \
    if ((val) >= 240) \
	*(out)++ = 0xf0 | ((val) >> 8); \
    *(out)++ = (val) & 0xff;

#define REENCODE_ANGLE(val, out) \
    if ((val) < ((-360) << 6) || (val) > (360 << 6)) \
	return 0; \
    if (((val) & 0x007f) == 0) { \
	short angle = (val) >> 7; \
	if (angle % 45) { \
	    *(out)++ = (val) >> 8; \
	    *(out)++ = (val) & 0xff; \
	} \
	else \
	    *(out)++ = 0x80 | ((angle / 45) + 4); \
    }

#define COPY_AND_SWAPS(oval, ival) \
    { \
	register int n; \
	(oval) = (ival); \
	if (client->swapped) \
	    swaps(&(oval), n); \
    }

#define SWAP_REENCODE_SHORT(ival, out) { \
    register int    _n; \
    short	    _val; \
    _val = (ival); \
    if (client->swapped) swaps (&_val, _n); \
    REENCODE_SHORT(_val, out); \
}

#define SWAP_REENCODE_USHORT(ival, out) { \
    register int    _n; \
    unsigned short  _val; \
    _val = (ival); \
    if (client->swapped) swaps (&_val, _n); \
    REENCODE_USHORT(_val, out); \
}

int         gfx_gc_hit;
int         gfx_gc_miss;
int         gfx_draw_hit;
int         gfx_draw_miss;
int         gfx_total;
int         gfx_bail;

static void
push(cache, xid)
    XID         cache[GFX_CACHE_SIZE];
    XID         xid;
{
    bcopy(cache, cache + 1, (GFX_CACHE_SIZE - 1) * sizeof(cache[0]));
    cache[0] = xid;
}

static void
use(cache, i)
    XID         cache[GFX_CACHE_SIZE];
    int         i;
{
    XID         tmp;

    if (i == 0)
	return;
    tmp = cache[i];
    bcopy(cache, cache + 1, i * sizeof(cache[0]));
    cache[0] = tmp;
}

static int
match(cache, xid)
    XID         cache[GFX_CACHE_SIZE];
    XID         xid;
{
    int         j;

    for (j = 0; j < GFX_CACHE_SIZE; j++)
	if (cache[j] == xid)
	    return j;
    return GFXCacheNone;
}

#define GFX_REQ_PAD	8

#define GFX_SRC_DST_REQ_PAD	12

#define GFX_CACHE_DECLARE   int		_dcache, _gcache; \
			    Drawable	_drawable; \
			    GContext	_gcontext; \

#define GFX_SRC_DST_CACHE_DECLARE	int _srcDcache, _dstDcache, _gcache; \
					Drawable _srcDrawable, _dstDrawable; \
					GContext _gcontext;

#define GFX_SETUP_DRAWABLE_AND_GC(after) {\
    _drawable = stuff->drawable; \
    _gcontext = stuff->gc; \
    if (client->swapped) { \
	register int _n; \
	swapl(&_drawable, _n); \
	swapl(&_gcontext, _n); \
    } \
    _dcache = match (LBXDrawableCache(client), _drawable); \
    if (_dcache == GFXCacheNone) \
    { \
	bcopy (&stuff->drawable, after, 4); \
	after += 4; \
    } \
    _gcache = match (LBXGContextCache(client), _gcontext); \
    if (_gcache == GFXCacheNone) \
    { \
	bcopy (&stuff->gc, after, 4); \
	after += 4; \
    } \
}

#define GFX_SETUP_SRC_DST_DRAWABLE_AND_GC(after) {\
    Drawable	tmpDrawableCache[GFX_CACHE_SIZE]; \
    bcopy (LBXDrawableCache(client), tmpDrawableCache, sizeof (LBXDrawableCache(client))); \
    _srcDrawable = stuff->srcDrawable; \
    _dstDrawable = stuff->dstDrawable; \
    _gcontext = stuff->gc; \
    if (client->swapped) { \
	register int _n; \
	swapl (&_srcDrawable, _n); \
	swapl (&_dstDrawable, _n); \
	swapl (&_gcontext, _n); \
    } \
    _srcDcache = match (LBXDrawableCache(client), _srcDrawable); \
    if (_srcDcache == GFXCacheNone) \
    { \
	bcopy (&stuff->srcDrawable, after, 4); \
	after += 4; \
	push (tmpDrawableCache, _srcDrawable); \
    } else \
	use (tmpDrawableCache, _srcDcache); \
    _dstDcache = match (tmpDrawableCache, _dstDrawable); \
    if (_dstDcache == GFXCacheNone) \
    { \
	bcopy (&stuff->dstDrawable, after, 4); \
	after += 4; \
    } \
    _gcache = match (LBXGContextCache(client), _gcontext); \
    if (_gcache == GFXCacheNone) \
    { \
	bcopy (&stuff->gc, after, 4); \
	after += 4; \
    } \
}

#define GFX_SET_DRAWABLE_AND_GC {\
    if (_dcache == GFXCacheNone) { \
	gfx_draw_miss++; \
	push (LBXDrawableCache(client), _drawable); \
    } else { \
	gfx_draw_hit++; \
	use (LBXDrawableCache(client), _dcache); \
    } \
    if (_gcache == GFXCacheNone) { \
	gfx_gc_miss++; \
	push (LBXGContextCache(client), _gcontext);\
    } else { \
	gfx_gc_hit++; \
	use (LBXGContextCache(client), _gcache); \
    } \
    newreq->cacheEnts = GFXCacheEnts (_dcache, _gcache); \
    gfx_total++; \
}
#define GFX_SET_SRC_DST_DRAWABLE_AND_GC {\
    if (_srcDcache == GFXCacheNone) { \
	gfx_draw_miss++; \
	push (LBXDrawableCache(client), _srcDrawable); \
    } else { \
	gfx_draw_hit++; \
	use (LBXDrawableCache(client), _srcDcache); \
    } \
    newreq->srcCache = GFXCacheEnts (_srcDcache, 0); \
    if (_dstDcache == GFXCacheNone) { \
	gfx_draw_miss++; \
	push (LBXDrawableCache(client), _dstDrawable); \
    } else { \
	gfx_draw_hit++; \
	use (LBXDrawableCache(client), _dstDcache); \
    } \
    if (_gcache == GFXCacheNone) { \
	gfx_gc_miss++; \
	push (LBXGContextCache(client), _gcontext);\
    } else { \
	gfx_gc_hit++; \
	use (LBXGContextCache(client), _gcache); \
    } \
    newreq->cacheEnts = GFXCacheEnts (_dstDcache, _gcache); \
    gfx_total++; \
}

static int
reencode_poly(client, lbxreq, reencode_rtn)
    ClientPtr   client;
    CARD8       lbxreq;
    int         (*reencode_rtn) ();
{
    REQUEST(xPolyPointReq);
    XServerPtr  server = client->server;
    int         len = client->req_len << 2;
    int         space;
    xLbxPolyPointReq *newreq = NULL;
    char       *after;
    int         bytes;

    GFX_CACHE_DECLARE

	space = len - sz_xPolyPointReq + sz_xLbxPolyPointReq + GFX_REQ_PAD + 3;
    newreq = (xLbxPolyPointReq *) xalloc(space);
    if (newreq == NULL)
	goto bail;
    after = ((char *) newreq) + sz_xLbxPolyPointReq;
    GFX_SETUP_DRAWABLE_AND_GC(after);
    bytes = (*reencode_rtn) (client, (short *) &stuff[1],
			     (char *) after,
			     (len - sz_xPolyPointReq) >> 2);
    if (bytes == 0)
	goto bail;
    FinishLBXRequest(client, REQ_PASSTHROUGH);
    newreq->reqType = server->lbxReq;
    newreq->lbxReqType = lbxreq;
    GFX_SET_DRAWABLE_AND_GC;
    len = after - ((char *) newreq) + bytes;
    newreq->padBytes = pad[len % 4];
    bzero(((char *) newreq) + len, newreq->padBytes);
    len += newreq->padBytes;
    newreq->length = len >> 2;
    WriteReqToServer(client, len, (char *) newreq);
    xfree(newreq);
    return Success;
bail:
    if (newreq)
	xfree(newreq);
    return ProcStandardRequest(client);

}


/* ARGSUSED */
static int
reencode_points_origin(client, in, out, count)
    ClientPtr   client;
    register short *in;
    register char *out;
    int         count;
{
    register short diff;
    int         i;
    short       inval;
    short       last_x = 0;
    short       last_y = 0;
    char       *start_out = out;

    for (i = 0; i < count; i++, in += 2) {
	COPY_AND_SWAPS(inval, in[0]);
	diff = inval - last_x;
	last_x = inval;
	REENCODE_SHORT(diff, out);
	COPY_AND_SWAPS(inval, in[1]);
	diff = inval - last_y;
	last_y = inval;
	REENCODE_SHORT(diff, out);
    }
    return out - start_out;
}

/* ARGSUSED */
static int
reencode_points_previous(client, in, out, count)
    ClientPtr   client;
    register short *in;
    register char *out;
    int         count;
{
    int         i;
    short       inval;
    char       *start_out = out;

    for (i = 0; i < count; i++, in += 2) {
	COPY_AND_SWAPS(inval, in[0]);
	REENCODE_SHORT(inval, out);
	COPY_AND_SWAPS(inval, in[1]);
	REENCODE_SHORT(inval, out);
    }
    return out - start_out;
}

/* ARGSUSED */
static int
reencode_segment(client, in, out, count)
    ClientPtr   client;
    register short *in;
    register char *out;
    int         count;
{
    register short diff;
    int         i;
    short       inval;
    short       last_x = 0;
    short       last_y = 0;
    char       *start_out = out;

    count >>= 1;
    for (i = 0; i < count; i++, in += 4) {
	/* reencode (x1, y1) */
	COPY_AND_SWAPS(inval, in[0]);
	diff = inval - last_x;
	last_x = inval;
	REENCODE_SHORT(diff, out);
	COPY_AND_SWAPS(inval, in[1]);
	diff = inval - last_y;
	last_y = inval;
	REENCODE_SHORT(diff, out);

	/* reencode (x2, y2) */
	COPY_AND_SWAPS(inval, in[2]);
	diff = inval - last_x;
	REENCODE_SHORT(diff, out);
	COPY_AND_SWAPS(inval, in[3]);
	diff = inval - last_y;
	REENCODE_SHORT(diff, out);
    }
    return out - start_out;
}

/* ARGSUSED */
static int
reencode_rectangle(client, in, out, count)
    ClientPtr   client;
    register short *in;
    register char *out;
    int         count;
{
    register short diff;
    int         i;
    short       inval;
    short       last_x = 0;
    short       last_y = 0;
    char       *start_out = out;

    count >>= 1;
    for (i = 0; i < count; i++, in += 4) {
	/* reencode (x, y) origin */
	COPY_AND_SWAPS(inval, in[0]);
	diff = inval - last_x;
	last_x = inval;
	REENCODE_SHORT(diff, out);
	COPY_AND_SWAPS(inval, in[1]);
	diff = inval - last_y;
	last_y = inval;
	REENCODE_SHORT(diff, out);

	/* reencode (width, height) */
	COPY_AND_SWAPS(inval, in[2]);
	REENCODE_USHORT(inval, out);
	COPY_AND_SWAPS(inval, in[3]);
	REENCODE_USHORT(inval, out);
    }
    return out - start_out;
}

/* ARGSUSED */
static int
reencode_arc(client, in, out, count)
    ClientPtr   client;
    register short *in;
    register char *out;
    int         count;
{
    register short diff;
    int         i;
    short       inval;
    short       last_x = 0;
    short       last_y = 0;
    char       *start_out = out;

    count = count / 3;
    for (i = 0; i < count; i++, in += 6) {
	/* reencode (x, y) origin */
	COPY_AND_SWAPS(inval, in[0]);
	diff = inval - last_x;
	last_x = inval;
	REENCODE_SHORT(diff, out);
	COPY_AND_SWAPS(inval, in[1]);
	diff = inval - last_y;
	last_y = inval;
	REENCODE_SHORT(diff, out);

	/* reencode (width, height) */
	COPY_AND_SWAPS(inval, in[2]);
	REENCODE_USHORT(inval, out);
	COPY_AND_SWAPS(inval, in[3]);
	REENCODE_USHORT(inval, out);

	/* reencode (angle1, angle2) */
	COPY_AND_SWAPS(inval, in[4]);
	REENCODE_ANGLE(inval, out);
	COPY_AND_SWAPS(inval, in[5]);
	REENCODE_ANGLE(inval, out);
    }
    return out - start_out;
}

int
ProcLBXPolyPoint(client)
    ClientPtr   client;
{
    REQUEST(xPolyPointReq);
    int         (*reencode_rtn) ();

    reencode_rtn = (stuff->coordMode) ? reencode_points_previous :
	reencode_points_origin;
    return reencode_poly(client, X_LbxPolyPoint, reencode_rtn);
}

int
ProcLBXPolyLine(client)
    ClientPtr   client;
{
    REQUEST(xPolyLineReq);
    int         (*reencode_rtn) ();

    reencode_rtn = (stuff->coordMode) ? reencode_points_previous :
	reencode_points_origin;
    return reencode_poly(client, X_LbxPolyLine, reencode_rtn);
}

int
ProcLBXPolySegment(client)
    ClientPtr   client;
{
    return reencode_poly(client, X_LbxPolySegment, reencode_segment);
}

int
ProcLBXPolyRectangle(client)
    ClientPtr   client;
{
    return reencode_poly(client, X_LbxPolyRectangle, reencode_rectangle);
}

int
ProcLBXPolyArc(client)
    ClientPtr   client;
{
    return reencode_poly(client, X_LbxPolyArc, reencode_arc);
}

int
ProcLBXFillPoly(client)
    ClientPtr   client;
{
    REQUEST(xFillPolyReq);
    XServerPtr  server = client->server;
    int         len = client->req_len << 2;
    xLbxFillPolyReq *newreq = NULL;
    char       *after;
    int         bytes;
    int         space;
    int         (*reencode_rtn) ();

    GFX_CACHE_DECLARE;

    reencode_rtn = (stuff->coordMode) ? reencode_points_previous :
	reencode_points_origin;

    space = len - sz_xFillPolyReq + sz_xLbxPolyPointReq + GFX_REQ_PAD + 3;
    newreq = (xLbxFillPolyReq *) xalloc(space);
    if (newreq == NULL)
	goto bail;
    after = ((char *) newreq) + sz_xLbxFillPolyReq;
    GFX_SETUP_DRAWABLE_AND_GC(after);
    bytes = (*reencode_rtn) (client, (short *) &stuff[1], after,
			     (len - sz_xFillPolyReq) >> 2);
    if (bytes == 0)
	goto bail;

    FinishLBXRequest(client, REQ_PASSTHROUGH);
    newreq->reqType = server->lbxReq;
    newreq->lbxReqType = X_LbxFillPoly;
    newreq->shape = stuff->shape;
    GFX_SET_DRAWABLE_AND_GC;
    len = after - ((char *) newreq) + bytes;
    newreq->padBytes = pad[len % 4];
    bzero(((char *) newreq) + len, newreq->padBytes);
    len += newreq->padBytes;
    newreq->length = len >> 2;
    WriteReqToServer(client, len, (char *) newreq);
    xfree(newreq);
    return Success;
bail:
    if (newreq)
	xfree(newreq);
    return ProcStandardRequest(client);
}

int
ProcLBXPolyFillRectangle(client)
    ClientPtr   client;
{
    return reencode_poly(client, X_LbxPolyFillRectangle, reencode_rectangle);
}

int
ProcLBXPolyFillArc(client)
    ClientPtr   client;
{
    return reencode_poly(client, X_LbxPolyFillArc, reencode_arc);
}

static int
reencode_copy(client, in, out)
    ClientPtr   client;
    register xCopyAreaReq *in;
    register char *out;
{
    char       *start_out = out;

    SWAP_REENCODE_SHORT(in->srcX, out);
    SWAP_REENCODE_SHORT(in->srcY, out);
    SWAP_REENCODE_SHORT(in->dstX, out);
    SWAP_REENCODE_SHORT(in->dstY, out);
    SWAP_REENCODE_USHORT(in->width, out);
    SWAP_REENCODE_USHORT(in->height, out);
    return out - start_out;
}

int
ProcLBXCopyArea(client)
    ClientPtr   client;
{
    REQUEST(xCopyAreaReq);
    XServerPtr  server = client->server;
    int         len;
    xLbxCopyAreaReq *newreq = NULL;
    unsigned long space[(SIZEOF(xLbxCopyAreaReq) + GFX_SRC_DST_REQ_PAD + 7 * sizeof(unsigned long) - 1) / sizeof(unsigned long)];
    char       *after;
    int         extra;
    int         bytes;

    GFX_SRC_DST_CACHE_DECLARE;

    newreq = (xLbxCopyAreaReq *) space;
    after = ((char *) newreq) + sz_xLbxCopyAreaReq;
    GFX_SETUP_SRC_DST_DRAWABLE_AND_GC(after);
    bytes = reencode_copy(client, stuff, after);
    if (bytes == 0)
	goto bail;
    FinishLBXRequest(client, REQ_PASSTHROUGH);
    newreq->reqType = server->lbxReq;
    newreq->lbxReqType = X_LbxCopyArea;
    GFX_SET_SRC_DST_DRAWABLE_AND_GC;
    len = ((after + bytes) - ((char *) newreq));
    extra = pad[len & 3];
    bzero(((char *) newreq) + len, extra);
    len += extra;
    newreq->length = len >> 2;
    WriteReqToServer(client, len, (char *) newreq);
    return Success;
bail:
    return ProcStandardRequest(client);
}

int
ProcLBXCopyPlane(client)
    ClientPtr   client;
{
    REQUEST(xCopyPlaneReq);
    XServerPtr  server = client->server;
    int         len;
    xLbxCopyPlaneReq *newreq = NULL;
    unsigned long space[(SIZEOF(xLbxCopyPlaneReq) + GFX_SRC_DST_REQ_PAD + 7 * sizeof(unsigned long) - 1) / sizeof(unsigned long)];
    char       *after;
    int         bytes;
    int         extra;

    GFX_SRC_DST_CACHE_DECLARE;

    newreq = (xLbxCopyPlaneReq *) space;
    after = ((char *) newreq) + sz_xLbxCopyPlaneReq;
    GFX_SETUP_SRC_DST_DRAWABLE_AND_GC(after);
    bytes = reencode_copy(client, stuff, after);
    if (bytes == 0)
	goto bail;
    FinishLBXRequest(client, REQ_PASSTHROUGH);
    newreq->reqType = server->lbxReq;
    newreq->bitPlane = stuff->bitPlane;
    newreq->lbxReqType = X_LbxCopyPlane;
    GFX_SET_SRC_DST_DRAWABLE_AND_GC;
    len = ((after + bytes) - ((char *) newreq));
    extra = pad[len & 3];
    bzero(((char *) newreq) + len, extra);
    len += extra;
    newreq->length = len >> 2;
    WriteReqToServer(client, len, (char *) newreq);
    return Success;
bail:
    return ProcStandardRequest(client);
}

reencode_text_pos(client, in, out)
    ClientPtr   client;
    xPolyTextReq *in;
    char       *out;
{
    char       *start_out;

    start_out = out;
    SWAP_REENCODE_SHORT(in->x, out);
    SWAP_REENCODE_SHORT(in->y, out);
    return out - start_out;
}

int
ProcLBXPolyText(client)
    ClientPtr   client;
{
    REQUEST(xPolyTextReq);
    XServerPtr  server = client->server;
    int         len = client->req_len << 2;
    xLbxPolyTextReq *newreq = NULL;
    int         space;
    char       *after;
    int         bytes;
    int         extra;

    GFX_CACHE_DECLARE;

    space = len - sz_xPolyTextReq + sz_xLbxPolyTextReq + GFX_REQ_PAD + 4 + 3;
    newreq = (xLbxPolyTextReq *) xalloc(space);
    if (newreq == NULL)
	goto bail;
    after = ((char *) newreq) + sz_xLbxPolyTextReq;
    GFX_SETUP_DRAWABLE_AND_GC(after);
    /* Reencode the text position */
    bytes = reencode_text_pos(client, stuff, after);
    if (bytes == 0)
	goto bail;
    /* copy the text elements */
    bcopy((char *) &stuff[1], after + bytes, len - sz_xPolyTextReq);
    bytes += len - sz_xPolyTextReq;
    FinishLBXRequest(client, REQ_PASSTHROUGH);
    newreq->reqType = server->lbxReq;
    newreq->lbxReqType = stuff->reqType == X_PolyText8 ? X_LbxPolyText8 : X_LbxPolyText16;
    GFX_SET_DRAWABLE_AND_GC;
    len = ((after + bytes) - ((char *) newreq));
    extra = pad[len & 3];
    bzero(((char *) newreq) + len, extra);
    len += extra;
    newreq->length = len >> 2;
    WriteReqToServer(client, len, (char *) newreq);
    xfree(newreq);
    return Success;
bail:
    if (newreq)
	xfree(newreq);
    return ProcStandardRequest(client);
}

int
ProcLBXImageText(client)
    ClientPtr   client;
{
    REQUEST(xImageTextReq);
    XServerPtr  server = client->server;
    int         len = client->req_len << 2;
    xLbxImageTextReq *newreq = NULL;
    int         space;
    char       *after;
    int         bytes;
    int         extra;

    GFX_CACHE_DECLARE;

    space = len - sz_xImageTextReq + sz_xLbxImageTextReq + GFX_REQ_PAD + 4 + 3;
    newreq = (xLbxImageTextReq *) xalloc(space);
    if (newreq == NULL)
	goto bail;
    after = ((char *) newreq) + sz_xLbxImageTextReq;
    GFX_SETUP_DRAWABLE_AND_GC(after);
    /* Reencode the text position */
    bytes = reencode_text_pos(client, (xPolyTextReq *) stuff, after);
    if (bytes == 0)
	goto bail;
    /* copy the text elements */
    bcopy((char *) &stuff[1], after + bytes, stuff->nChars);
    FinishLBXRequest(client, REQ_PASSTHROUGH);
    newreq->reqType = server->lbxReq;
    newreq->lbxReqType = stuff->reqType == X_ImageText8 ? X_LbxImageText8 : X_LbxImageText16;
    newreq->nChars = stuff->nChars;
    GFX_SET_DRAWABLE_AND_GC;
    len = ((after + bytes + stuff->nChars) - ((char *) newreq));
    extra = pad[len & 3];
    bzero(((char *) newreq) + len, extra);
    len += extra;
    newreq->length = len >> 2;
    WriteReqToServer(client, len, (char *) newreq);
    xfree(newreq);
    return Success;
bail:
    if (newreq)
	xfree(newreq);
    return ProcStandardRequest(client);
}

static void
SwapXPutImage(req)
    xPutImageReq *req;
{
    char        n;

    swaps(&req->length, n);
    swapl(&req->drawable, n);
    swapl(&req->gc, n);
    swaps(&req->width, n);
    swaps(&req->height, n);
    swaps(&req->dstX, n);
    swaps(&req->dstY, n);
}


int
ProcLBXPutImage(client)
    ClientPtr   client;
{
    REQUEST(xPutImageReq);
    XServerPtr  server = client->server;
    xLbxPutImageReq *newreq = NULL;
    int         len,
                method,
                compBytes,
                status;
    float       percentCompression;

    if (client->swapped)
	SwapXPutImage(stuff);

    len = stuff->length << 2;

    if (!((stuff->format == ZPixmap && stuff->depth == 8) ||
	  stuff->depth == 1)) {
	status = LBX_IMAGE_COMPRESS_UNSUPPORTED_FORMAT;
    } else if ((newreq = (xLbxPutImageReq *) xalloc(len)) == NULL) {
	status = LBX_IMAGE_COMPRESS_BAD_MALLOC;
    } else {
	if (stuff->depth == 1) {
	    status = LbxImageEncodeFaxG42D((unsigned char *) &stuff[1],
			       (unsigned char *) newreq + sz_xLbxPutImageReq,
					   len - sz_xLbxPutImageReq,
					   len - sz_xPutImageReq,
					   (int) stuff->width,
					   LBXBitmapBitOrder(client) == LSBFirst,
					   &compBytes);

	    method = LbxImageCompressFaxG42D;
	} else {		/* depth 8 and ZPixmap format */
	    status = LbxImageEncodePackBits((char *) &stuff[1],
					(char *) newreq + sz_xLbxPutImageReq,
					    len - sz_xLbxPutImageReq,
			(int) stuff->height, (int) stuff->width, &compBytes);

	    method = LbxImageCompressPackBits;
	}
    }

#ifdef LBX_STATS
    fprintf(stderr, "PutImage (xid=%x, w=%d, h=%d, x=%d, y=%d): ",
	    stuff->drawable, stuff->width, stuff->height,
	    stuff->dstX, stuff->dstY);
#endif

    if (status != LBX_IMAGE_COMPRESS_SUCCESS) {

#ifdef LBX_STATS
	fprintf(stderr, "image not compressed - ");

	if (status == LBX_IMAGE_COMPRESS_UNSUPPORTED_FORMAT)
	    fprintf(stderr, "unsupported format\n");
	else if (status == LBX_IMAGE_COMPRESS_NOT_WORTH_IT)
	    fprintf(stderr, "not worth it\n");
	else if (status == LBX_IMAGE_COMPRESS_BAD_MALLOC)
	    fprintf(stderr, "bad malloc\n");
#endif

	if (newreq)
	    xfree(newreq);

	if (client->swapped)	/* Swap it back */
	    SwapXPutImage(stuff);

	return ProcStandardRequest(client);
    }
    if (client->swapped) {
	/*
	 * Swap the length back because FinishLBXRequest will swap it.
	 */

	char        n;

	swaps(&stuff->length, n);
    }
    FinishLBXRequest(client, REQ_PASSTHROUGH);

    newreq->reqType = server->lbxReq;
    newreq->lbxReqType = X_LbxPutImage;
    newreq->compressionMethod = method;
    newreq->drawable = stuff->drawable;
    newreq->gc = stuff->gc;
    newreq->width = stuff->width;
    newreq->height = stuff->height;
    newreq->dstX = stuff->dstX;
    newreq->dstY = stuff->dstY;
    newreq->format = stuff->format;
    newreq->leftPad = stuff->leftPad;
    newreq->depth = stuff->depth;

    len = sz_xLbxPutImageReq + compBytes;
    newreq->padBytes = pad[len % 4];
    len += newreq->padBytes;
    newreq->lbxLength = len >> 2;
    newreq->xLength = stuff->length;

#ifdef LBX_STATS
    percentCompression = 100.0 * (1.0 -
				  ((float) (compBytes + pad[compBytes % 4]) /
			  (float) ((stuff->length << 2) - sz_xPutImageReq)));

    fprintf(stderr, "%f percent compression ", percentCompression);
    if (method == LbxImageCompressFaxG42D)
	fprintf(stderr, "(FAX G42D)\n");
    else if (method == LbxImageCompressPackBits)
	fprintf(stderr, "(PackBits)\n");
#endif

    if (client->swapped)
	SwapPutImage(newreq);

    WriteToServerUncompressed(client, len, (char *) newreq);

    xfree(newreq);
    return Success;
}


static int
get_image_req(client, data)
    ClientPtr   client;
    char       *data;
{
    ReplyStuffPtr nr;
    xGetImageReq *req;

    req = (xGetImageReq *) data;
    nr = NewReply(client);

    if (nr) {
	if (client->swapped) {
	    char        n;

	    swapl(&req->drawable, n);
	    swaps(&req->x, n);
	    swaps(&req->y, n);
	    swaps(&req->width, n);
	    swaps(&req->height, n);
	    swapl(&req->planeMask, n);
	}
	nr->sequenceNumber = LBXSequenceNumber(client);
	nr->request = X_LbxGetImage;
	nr->lbx_req = TRUE;
	nr->extension = client->server->lbxReq;
	nr->request_info.lbxgetimage.width = req->width;
	nr->request_info.lbxgetimage.height = req->height;

#ifdef LBX_STATS
	nr->request_info.lbxgetimage.drawable = req->drawable;
	nr->request_info.lbxgetimage.x = req->x;
	nr->request_info.lbxgetimage.y = req->y;
#endif

	/*
	 * this expects a reply.  since we write the data here, we have to be
	 * sure the seq number is in sync first
	 */

	ForceSequenceUpdate(client);

	SendGetImage(client, req->drawable, req->x, req->y,
		     req->width, req->height, req->planeMask, req->format);

	return REQ_REPLACE;
    } else
	return REQ_NOCHANGE;
}

int
ProcLBXGetImage(client)
    ClientPtr   client;
{
    int         yank;

    yank = get_image_req(client, client->requestBuffer);
    return FinishLBXRequest(client, yank);
}


Bool
GetLbxImageReply(client, data)
    ClientPtr   client;
    char       *data;
{
    xLbxGetImageReply *rep;
    xGetImageReply reply;
    int         len;
    pointer     imageData;
    ReplyStuffPtr nr;
    int         freeIt = 1;

    rep = (xLbxGetImageReply *) data;

    nr = GetReply(client);
    assert(nr);

    if (client->swapped) {
	char        n;

	swapl(&rep->xLength, n);
	swapl(&rep->visual, n);
    }
    if ((imageData = (pointer) xalloc(rep->xLength << 2)) == NULL)
	return FALSE;

    if (rep->compressionMethod == LbxImageCompressFaxG42D) {
	LbxImageDecodeFaxG42D(
		      (unsigned char *) &rep[1], (unsigned char *) imageData,
			      (int) ((rep->xLength << 2) - sz_xPutImageReq),
			      (int) nr->request_info.lbxgetimage.width,
			      LBXBitmapBitOrder(client) == LSBFirst);
    } else if (rep->compressionMethod == LbxImageCompressPackBits) {
	LbxImageDecodePackBits((char *) &rep[1], (char *) imageData,
			       (int) nr->request_info.lbxgetimage.height,
			       (int) nr->request_info.lbxgetimage.width);
    } else if (rep->compressionMethod == LbxImageCompressNone) {
	xfree((char *) imageData);
	imageData = (pointer) &rep[1];
	freeIt = 0;
    }
    reply.type = X_Reply;
    reply.depth = rep->depth;
    reply.sequenceNumber = rep->sequenceNumber;
    reply.length = rep->xLength;
    reply.visual = rep->visual;

    if (client->swapped)
	SwapGetImageReply(&reply);

#ifdef LBX_STATS
    fprintf (stderr, "GetImage (xid=%x, w=%d, h=%d, x=%d, y=%d): ",
			nr->request_info.lbxgetimage.drawable,
			nr->request_info.lbxgetimage.width,
			nr->request_info.lbxgetimage.height,
			nr->request_info.lbxgetimage.x,
			nr->request_info.lbxgetimage.y);
    if (rep->compressionMethod == LbxImageCompressNone) {
	fprintf(stderr, "image not compressed\n");
    } else {
	float       percentCompression = 100.0 * (1.0 -
			    ((float) rep->lbxLength / (float) rep->xLength));

	fprintf(stderr, "%f percent compression ", percentCompression);
	if (rep->compressionMethod == LbxImageCompressFaxG42D)
	    fprintf(stderr, "(FAX G42D)\n");
	else if (rep->compressionMethod == LbxImageCompressPackBits)
	    fprintf(stderr, "(PackBits)\n");
    }
#endif

    WriteToClient(client, sizeof(xGetImageReply), &reply);
    WriteToClientPad(client, rep->xLength << 2, imageData);

    if (freeIt)
	xfree((char *) imageData);

    return TRUE;
}
