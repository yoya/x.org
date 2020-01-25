/* $XConsortium: lbxgfx.c,v 1.13 94/12/01 20:32:26 mor Exp $ */
/*
 * Copyright 1993 Network Computing Devices, Inc.
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
 * $NCDId: @(#)lbxgfx.c,v 1.8 1994/11/29 19:58:32 lemke Exp $
 *
 * Author:  Dave Lemke, Network Computing Devices
 */

/* various bits of DIX-level mangling */

#include <sys/types.h>
#include <stdio.h>
#define NEED_REPLIES
#define NEED_EVENTS
#include "X.h"
#include "Xproto.h"
#include "misc.h"
#include "os.h"
#include "dixstruct.h"
#include "resource.h"
#include "servermd.h"
#include "windowstr.h"
#include "scrnintstr.h"
#define _XLBX_SERVER_
#include "lbxstr.h"
#include "lbxserve.h"
#include "lbxtags.h"
#include "Xfuncproto.h"
#include "lbximage.h"

#define DrawableCache(client)	(LbxClient(client)->drawableCache)
#define GContextCache(client)	(LbxClient(client)->gcontextCache)

extern int (*ProcVector[256])();

static void
push (cache, xid)
    XID	    cache[GFX_CACHE_SIZE];
    XID	    xid;
{
    bcopy (cache, cache+1, (GFX_CACHE_SIZE - 1) * sizeof (cache[0]));
    cache[0] = xid;
}

static XID
use (cache, i)
    XID	    cache[GFX_CACHE_SIZE];
    int	    i;
{
    XID	tmp;

    tmp = cache[i];
    if (i != 0)
    {
	bcopy (cache, cache + 1, i * sizeof (cache[0]));
	cache[0] = tmp;
    }
    return tmp;
}

extern char *ConnectionInfo;

int
LbxDecodeGFXCache(client, cacheEnts, after, drawp, gcp)
    ClientPtr		client;
    CARD8		cacheEnts;
    char		*after;
    Drawable		*drawp;
    GContext		*gcp;
{
    int	skip;
    int	dcache, gcache;
    
    dcache = GFXdCacheEnt (cacheEnts);
    gcache = GFXgCacheEnt (cacheEnts);
    skip = 0;
    if (dcache == GFXCacheNone)
    {
	bcopy (after, drawp, sizeof (Drawable));
	push (DrawableCache(client), *drawp);
	after += sizeof (Drawable);
	skip += sizeof (Drawable);
    }
    else
	*drawp = use (DrawableCache(client), dcache);
    if (gcache == GFXCacheNone)
    {
	bcopy (after, gcp, sizeof (GContext));
	push (GContextCache(client), *gcp);
	skip += sizeof (GContext);
    }
    else
	*gcp = use (GContextCache(client), gcache);
    return skip;
}

int
LbxDecodeDrawableCache(client, cacheEnts, after, drawp)
    ClientPtr		client;
    CARD8		cacheEnts;
    char		*after;
    Drawable		*drawp;
{
    int	skip;
    int	dcache;
    
    dcache = GFXdCacheEnt (cacheEnts);
    skip = 0;
    if (dcache == GFXCacheNone)
    {
	bcopy (after, drawp, sizeof (Drawable));
	push (DrawableCache(client), *drawp);
	after += sizeof (Drawable);
	skip += sizeof (Drawable);
    }
    else
	*drawp = use (DrawableCache(client), dcache);
    return skip;
}

int
LbxDecodeGCCache(client, cacheEnts, after, gcp)
    ClientPtr		client;
    CARD8		cacheEnts;
    char		*after;
    GContext		*gcp;
{
    int	skip;
    int	gcache;
    
    gcache = GFXgCacheEnt (cacheEnts);
    skip = 0;
    if (gcache == GFXCacheNone)
    {
	bcopy (after, gcp, sizeof (GContext));
	push (GContextCache(client), *gcp);
	after += sizeof (GContext);
	skip += sizeof (GContext);
    }
    else
	*gcp = use (GContextCache(client), gcache);
    return skip;
}

#define GFX_GET_DRAWABLE_AND_GC(type,in,len) {\
    int	    skip;   \
    len = (stuff->length << 2) - SIZEOF(type); \
    in = ((char *) stuff) + SIZEOF(type);\
    skip = LbxDecodeGFXCache(client, stuff->cacheEnts, in, \
			     &drawable, &gc); \
    in += skip; \
    len -= skip; \
}
    
#define GFX_GET_DST_DRAWABLE_AND_GC(type,in,len) {\
    int	    skip;   \
    len = (stuff->length << 2) - SIZEOF(type); \
    in = ((char *) stuff) + SIZEOF(type);\
    skip = LbxDecodeGFXCache(client, stuff->cacheEnts, in, \
			     &dstDrawable, &gc); \
    in += skip; \
    len -= skip; \
}
    
#define GFX_GET_SRC_DST_DRAWABLE_AND_GC(type, in, len) { \
    int	    skip; \
    len = (stuff->length << 2) - SIZEOF(type); \
    in = ((char *) stuff) + SIZEOF(type); \
    skip = LbxDecodeDrawableCache(client, stuff->srcCache, in, \
				  &srcDrawable); \
    in += skip; \
    len -= skip; \
    skip = LbxDecodeGFXCache(client, stuff->cacheEnts, in, \
			     &dstDrawable, &gc); \
    in += skip; \
    len -= skip; \
}

#define GFX_GET_GC(type, in, len) { \
    int	    skip; \
    len = (stuff->length << 2) - SIZEOF(type); \
    in = ((char *) stuff) + SIZEOF(type); \
    skip = LbxDecodeGCCache(client, stuff->gcCache, in, &gc); \
    in += skip; \
    len -= skip; \
}

int
LbxDecodePoly(client, xreqtype, decode_rtn)
    register ClientPtr  client;
    CARD8		xreqtype;
    int			(*decode_rtn)();
{
    REQUEST(xLbxPolyPointReq);
    char		*in;
    xPolyPointReq	*xreq;
    int			len;
    int			retval;
    int			dcache, gcache;
    Drawable		drawable;
    GContext		gc;

    GFX_GET_DRAWABLE_AND_GC(xLbxPolyPointReq, in, len);
    if ((xreq = (xPolyPointReq *) 
	    xalloc(sizeof(xPolyPointReq) + (len << 1))) == NULL)
	return BadAlloc;
    len = (*decode_rtn)(in, in + len - stuff->padBytes, &xreq[1]);
    xreq->reqType = xreqtype;
    xreq->coordMode = 1;
    xreq->drawable = drawable;
    xreq->gc = gc;
    client->req_len = xreq->length = (sizeof(xPolyPointReq) + len) >> 2;
    client->requestBuffer = (pointer)xreq;

    retval = (*ProcVector[xreqtype])(client);
    xfree(xreq);
    return retval;
}

int
LbxDecodeFillPoly(client)
    register ClientPtr  client;
{
    REQUEST(xLbxFillPolyReq);
    char		*in;
    xFillPolyReq	*xreq;
    int			len;
    int			retval;
    Drawable		drawable;
    GContext		gc;
    
    GFX_GET_DRAWABLE_AND_GC(xLbxFillPolyReq, in, len);
    if ((xreq = (xFillPolyReq *) 
	    xalloc(sizeof(xFillPolyReq) + (len << 1))) == NULL)
	return BadAlloc;
    len = LbxDecodePoints(in, in + len - stuff->padBytes, (short *) &xreq[1]);
    xreq->reqType = X_FillPoly;
    xreq->drawable = drawable;
    xreq->gc = gc;
    xreq->shape = stuff->shape;
    xreq->coordMode = 1;
    client->req_len = xreq->length = (sizeof(xFillPolyReq) + len) >> 2;
    client->requestBuffer = (pointer)xreq;

    retval = (*ProcVector[X_FillPoly])(client);
    xfree(xreq);
    return retval;
}

/*
 * Routines for decoding line drawing requests
 */

#define DECODE_SHORT(in, val) \
    if ((*(in) & 0xf0) != 0x80) \
        (val) = (short)*(in)++; \
    else { \
	(val) = ((short)(*(in) & 0x0f) << 8) | (unsigned char)*((in) + 1); \
	if (*(in) & 0x08) (val) |= 0xf000; \
	(in) += 2; \
    }

#define DECODE_USHORT(in, val) \
    if ((*(in) & 0xf0) != 0xf0) \
	(val) = (unsigned char)*(in)++; \
    else { \
	(val) = ((short)(*(in) & 0x0f) << 8) | (unsigned char)*((in) + 1); \
	(in) += 2; \
    }

#define DECODE_ANGLE(in, val) \
    if ((*(in) & 0xf0) == 0x80) \
	(val) = (((short)(*(in)++ & 0x0f) - 4) * 45) << 7; \
    else { \
	(val) = ((short)*(in) << 8) | (unsigned char)*((in) + 1); \
	(in) += 2; \
    }

int
LbxDecodePoints(in, inend, out)
    register char  *in;
    char	   *inend;
    register short *out;
{
    char	   *start_out = (char *)out;

    while (in < inend) {
	DECODE_SHORT(in, *out);
	out++;
	DECODE_SHORT(in, *out);
	out++;
    }
    return ((char *)out - start_out);
}

int
LbxDecodeSegment(in, inend, out)
    register char  *in;
    char	   *inend;
    register short *out;
{
    register short diff;
    short	   last_x = 0;
    short	   last_y = 0;
    char	   *start_out = (char *)out;

    while (in < inend) {
	DECODE_SHORT(in, diff);
	*out = last_x + diff;
	last_x += diff;
	out++;
	DECODE_SHORT(in, diff);
	*out = last_y + diff;
	last_y += diff;
	out++;

	DECODE_SHORT(in, diff);
	*out = last_x + diff;
	out++;
	DECODE_SHORT(in, diff);
	*out = last_y + diff;
	out++;
    }
    return ((char *)out - start_out);
}

int
LbxDecodeRectangle(in, inend, out)
    register char  *in;
    char	   *inend;
    register short *out;
{
    register short diff;
    short	   last_x = 0;
    short	   last_y = 0;
    char	   *start_out = (char *)out;

    while (in < inend) {
	DECODE_SHORT(in, diff);
	*out = last_x + diff;
	last_x += diff;
	out++;
	DECODE_SHORT(in, diff);
	*out = last_y + diff;
	last_y += diff;
	out++;

	DECODE_USHORT(in, *(unsigned short *)out);
	out++;
	DECODE_USHORT(in, *(unsigned short *)out);
	out++;
    }
    return ((char *)out - start_out);
}

int
LbxDecodeArc(in, inend, out)
    register char  *in;
    char	   *inend;
    register short *out;
{
    register short diff;
    short	   last_x = 0;
    short	   last_y = 0;
    char	   *start_out = (char *)out;

    while (in < inend) {
	DECODE_SHORT(in, diff);
	*out = last_x + diff;
	last_x += diff;
	out++;
	DECODE_SHORT(in, diff);
	*out = last_y + diff;
	last_y += diff;
	out++;

	DECODE_USHORT(in, *(unsigned short *)out);
	out++;
	DECODE_USHORT(in, *(unsigned short *)out);
	out++;

	DECODE_ANGLE(in, *out);
	out++;
	DECODE_ANGLE(in, *out);
	out++;
    }
    return ((char *)out - start_out);
}

int
LbxDecodeCopyArea (client)
    ClientPtr	client;
{
    REQUEST(xLbxCopyAreaReq);
    char		*in;
    xCopyAreaReq    	req;
    int			len;
    Drawable		srcDrawable, dstDrawable;
    GContext		gc;
    
    GFX_GET_SRC_DST_DRAWABLE_AND_GC(xLbxCopyAreaReq, in, len);
    req.reqType = X_CopyArea;
    client->req_len = req.length = SIZEOF(xCopyAreaReq) >> 2;
    req.srcDrawable = srcDrawable;
    req.dstDrawable = dstDrawable;
    req.gc = gc;
    DECODE_SHORT (in, req.srcX);
    DECODE_SHORT (in, req.srcY);
    DECODE_SHORT (in, req.dstX);
    DECODE_SHORT (in, req.dstY);
    DECODE_USHORT (in, req.width);
    DECODE_USHORT (in, req.height);
    client->requestBuffer = (pointer) &req;
    return (*ProcVector[X_CopyArea])(client);
}

int
LbxDecodeCopyPlane (client)
    ClientPtr	client;
{
    REQUEST(xLbxCopyPlaneReq);
    char		*in;
    xCopyPlaneReq 	req;
    int			len;
    Drawable		srcDrawable, dstDrawable;
    GContext		gc;
    
    GFX_GET_SRC_DST_DRAWABLE_AND_GC(xLbxCopyPlaneReq, in, len);
    req.reqType = X_CopyPlane;
    client->req_len = req.length = SIZEOF(xCopyPlaneReq) >> 2;
    req.srcDrawable = srcDrawable;
    req.dstDrawable = dstDrawable;
    req.gc = gc;
    DECODE_SHORT (in, req.srcX);
    DECODE_SHORT (in, req.srcY);
    DECODE_SHORT (in, req.dstX);
    DECODE_SHORT (in, req.dstY);
    DECODE_USHORT (in, req.width);
    DECODE_USHORT (in, req.height);
    req.bitPlane = stuff->bitPlane;
    client->requestBuffer = (pointer) &req;
    return (*ProcVector[X_CopyPlane])(client);
}

int
LbxDecodePolyText (client)
    ClientPtr	client;
{
    REQUEST(xLbxPolyTextReq);
    char		*in, *pos;
    xPolyTextReq	*xreq;
    int			len;
    int			retval;
    Drawable		drawable;
    GContext		gc;
    
    GFX_GET_DRAWABLE_AND_GC(xLbxPolyTextReq, in, len);
    if ((xreq = (xPolyTextReq *) 
	    xalloc (sizeof (xPolyTextReq) + len)) == NULL)
	return BadAlloc;
    xreq->reqType = stuff->lbxReqType == X_LbxPolyText8? X_PolyText8 : X_PolyText16;
    xreq->drawable = drawable;
    xreq->gc = gc;
    pos = in;
    DECODE_SHORT(in, xreq->x);
    DECODE_SHORT(in, xreq->y);
    len -= (in - pos);
    bcopy (in, (char *) (xreq + 1), len);
    client->req_len = xreq->length = (sizeof (xPolyTextReq) + len) >> 2;
    client->requestBuffer = (pointer) xreq;
    return (*ProcVector[xreq->reqType])(client);
}

int
LbxDecodeImageText (client)
    ClientPtr	client;
{
    REQUEST(xLbxImageTextReq);
    char		*in, *pos;
    xImageTextReq	*xreq;
    int			len;
    int			retval;
    Drawable		drawable;
    GContext		gc;
    
    GFX_GET_DRAWABLE_AND_GC(xLbxImageTextReq, in, len);
    if ((xreq = (xImageTextReq *) 
	    xalloc (sizeof (xImageTextReq) + len)) == NULL)
	return BadAlloc;
    xreq->reqType = stuff->lbxReqType == X_LbxImageText8? X_ImageText8 : X_ImageText16;
    xreq->drawable = drawable;
    xreq->gc = gc;
    xreq->nChars = stuff->nChars;
    pos = in;
    DECODE_SHORT(in, xreq->x);
    DECODE_SHORT(in, xreq->y);
    bcopy (in, (char *) (xreq + 1), stuff->nChars);
    client->req_len = xreq->length = (sizeof (xImageTextReq) + stuff->nChars + 3) >> 2;
    client->requestBuffer = (pointer) xreq;
    return (*ProcVector[xreq->reqType])(client);
}


int
LbxDecodePutImage (client)
    register ClientPtr  client;
{
    REQUEST		(xLbxPutImageReq);
    char		*in;
    xPutImageReq	*xreq;
    int			len;
    int			retval;

    if (stuff->compressionMethod != LbxImageCompressFaxG42D &&
        stuff->compressionMethod != LbxImageCompressPackBits)
    {
	return BadValue;
    }

    if ((xreq = (xPutImageReq *) xalloc (stuff->xLength << 2)) == NULL)
	return BadAlloc;

    in = (char *) stuff + sz_xLbxPutImageReq;

    if (stuff->compressionMethod == LbxImageCompressFaxG42D)
    {
	len = LbxImageDecodeFaxG42D (
	    (unsigned char *) in, (unsigned char *) &xreq[1],
	    (int) ((stuff->xLength << 2) - sz_xPutImageReq),
	    (int) stuff->width,
	    ((xConnSetup *) ConnectionInfo)->bitmapBitOrder == LSBFirst);
    }
    else
    {
	len = LbxImageDecodePackBits (in, (char *) &xreq[1],
	    (int) stuff->height, (int) stuff->width);
    }

    xreq->reqType = X_PutImage;
    xreq->drawable = stuff->drawable;
    xreq->gc = stuff->gc;
    xreq->width = stuff->width;
    xreq->height = stuff->height;
    xreq->format = stuff->format;
    xreq->dstX = stuff->dstX;
    xreq->dstY = stuff->dstY;
    xreq->leftPad = stuff->leftPad;
    xreq->depth = stuff->depth;

    client->req_len = xreq->length = stuff->xLength;
    client->requestBuffer = (pointer) xreq;

    retval = (*ProcVector[X_PutImage])(client);
    xfree(xreq);
    return retval;
}



int
GetTheImage (client, stuff, depth, visual, theImage, imageBytes)
    register ClientPtr client;
    xLbxGetImageReq *stuff;
    int *depth;
    VisualID *visual;
    char **theImage;
    int *imageBytes;
{
    register DrawablePtr pDraw;
    int			lines, linesPerBuf;
    register int	height;
    long		widthBytesLine, length;
    Mask		plane;
    char		*pBuf;

    height = stuff->height;
    if ((stuff->format != XYPixmap) && (stuff->format != ZPixmap))
    {
	client->errorValue = stuff->format;
        return(BadValue);
    }

    VERIFY_DRAWABLE(pDraw, stuff->drawable, client);

    *depth = pDraw->depth;

    if(pDraw->type == DRAWABLE_WINDOW)
    {
      if( /* check for being viewable */
	 !((WindowPtr) pDraw)->realized ||
	  /* check for being on screen */
         pDraw->x + stuff->x < 0 ||
 	 pDraw->x + stuff->x + (int)stuff->width > pDraw->pScreen->width ||
         pDraw->y + stuff->y < 0 ||
         pDraw->y + stuff->y + height > pDraw->pScreen->height ||
          /* check for being inside of border */
         stuff->x < - wBorderWidth((WindowPtr)pDraw) ||
         stuff->x + (int)stuff->width >
		wBorderWidth((WindowPtr)pDraw) + (int)pDraw->width ||
         stuff->y < -wBorderWidth((WindowPtr)pDraw) ||
         stuff->y + height >
		wBorderWidth ((WindowPtr)pDraw) + (int)pDraw->height
        )
	    return(BadMatch);
	*visual = wVisual (((WindowPtr) pDraw));
    }
    else
    {
      if(stuff->x < 0 ||
         stuff->x+(int)stuff->width > pDraw->width ||
         stuff->y < 0 ||
         stuff->y+height > pDraw->height
        )
	    return(BadMatch);
	*visual = None;
    }
    if(stuff->format == ZPixmap)
    {
	widthBytesLine = PixmapBytePad(stuff->width, pDraw->depth);
	length = widthBytesLine * height;
    }
    else 
    {
	widthBytesLine = BitmapBytePad(stuff->width);
	plane = ((Mask)1) << (pDraw->depth - 1);
	/* only planes asked for */
	length = widthBytesLine * height *
		 Ones(stuff->planeMask & (plane | (plane - 1)));
    }

    *imageBytes = length;

    if (widthBytesLine == 0 || height == 0)
	linesPerBuf = 0;
    else
	linesPerBuf = height;

    if(!(pBuf = *theImage = (char *) xalloc(length + 4)))
        return (BadAlloc);

    if (linesPerBuf == 0)
    {
	/* nothing to do */
    }
    else if (stuff->format == ZPixmap)
    {
	(*pDraw->pScreen->GetImage) (pDraw,
	                                 stuff->x,
				         stuff->y,
				         stuff->width, 
				         height,
				         stuff->format,
				         stuff->planeMask,
				         (pointer) pBuf);
    }
    else /* XYPixmap */
    {
	char *bufptr = pBuf;

        for (; plane; plane >>= 1)
	{
	    if (stuff->planeMask & plane)
	    {
		(*pDraw->pScreen->GetImage) (pDraw,
	                                         stuff->x,
				                 stuff->y,
				                 stuff->width, 
				                 height,
				                 stuff->format,
				                 plane,
				                 (pointer)bufptr);

		    bufptr += (height * widthBytesLine);
            }
	}
    }

    return (client->noClientException);
}



int
LbxDecodeGetImage (client)
    register ClientPtr  client;
{
    REQUEST		(xLbxGetImageReq);
    xLbxGetImageReply	*reply = NULL;
    int			depth, uncompLen, lbxLen, n;
    VisualID		visual;
    char		*theImage;
    int			method, bytes, status;

    REQUEST_SIZE_MATCH(xLbxGetImageReq);

    status = GetTheImage (client, stuff,
	&depth, &visual, &theImage, &uncompLen);

    if (status != Success)
	return (status);

    if ((reply = (xLbxGetImageReply *) xalloc (
	sz_xLbxGetImageReply + uncompLen)) == NULL)
    {
	return (!Success);
    }

    if (!((stuff->format == ZPixmap && depth == 8) || depth == 1))
    {
	status = LBX_IMAGE_COMPRESS_UNSUPPORTED_FORMAT;
    }
    else if (depth == 1)
    {
	status = LbxImageEncodeFaxG42D ((unsigned char *) theImage,
	      (unsigned char *) reply + sz_xLbxGetImageReply,
	      uncompLen, uncompLen, (int) stuff->width,
	      ((xConnSetup *) ConnectionInfo)->bitmapBitOrder == LSBFirst,
	      &bytes);

	method = LbxImageCompressFaxG42D;
    }
    else /* depth 8 and ZPixmap format */
    {
	status = LbxImageEncodePackBits ((char *) theImage,
	      (char *) reply + sz_xLbxGetImageReply, uncompLen,
	      (int) stuff->height, (int) stuff->width, &bytes);

	method = LbxImageCompressPackBits;
    }

    reply->type = X_Reply;
    reply->depth = depth;
    reply->sequenceNumber = client->sequence;
    reply->visual = visual;

    if (status != LBX_IMAGE_COMPRESS_SUCCESS)
    {
	reply->compressionMethod = LbxImageCompressNone;
	reply->lbxLength = reply->xLength = (uncompLen + 3) >> 2;
    }
    else
    {
	reply->compressionMethod = method;
	reply->lbxLength = (bytes + 3) >> 2;
	reply->xLength = (uncompLen + 3) >> 2;
    }

    lbxLen = reply->lbxLength;

    if (client->swapped)
    {
	swaps (&reply->sequenceNumber, n);
	swapl (&reply->lbxLength, n);
	swapl (&reply->xLength, n);
	swapl (&reply->visual, n);
    }

    if (reply->compressionMethod != LbxImageCompressNone)
    {
	UncompressedWriteToClient (client,
	    sz_xLbxGetImageReply + (lbxLen << 2), reply);
    }
    else
    {
	UncompressedWriteToClient (client, sz_xLbxGetImageReply, reply);
	UncompressedWriteToClient (client, lbxLen << 2, theImage);
    }

    xfree (theImage);

    if (reply)
	xfree ((char *) reply);

    return (Success);
}
