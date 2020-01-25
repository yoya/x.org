/***********************************************************
Copyright 1987 by Digital Equipment Corporation, Maynard, Massachusetts,
and the Massachusetts Institute of Technology, Cambridge, Massachusetts.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the names of Digital or MIT not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.  

DIGITAL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/
/***********************************************************
		Copyright IBM Corporation 1987

                      All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the name of IBM not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.  

IBM DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
IBM BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/

/* $Header: aedcparea.c,v 1.2 88/02/05 15:04:12 rws Exp $ */
/* $ACIS:aedcparea.c 6.0$ */

#ifndef lint
static char *rcsid = "$Header: aedcparea.c,v 1.2 88/02/05 15:04:12 rws Exp $";
#endif

#include "X.h"
#include "Xprotostr.h"

#include "miscstruct.h"
#include "regionstr.h"
#include "gcstruct.h"
#include "windowstr.h"
#include "pixmapstr.h"
#include "scrnintstr.h"

#include "mfb.h"
#include "maskbits.h"
#include "rtutils.h"
#include "xaed.h"
#include <stdio.h>


/* CopyArea and CopyPlane for a monchrome frame buffer


    clip the source rectangle to the source's available bits.  (this
avoids copying unnecessary pieces that will just get exposed anyway.)
this becomes the new shape of the destination.
    clip the destination region to the composite clip in the
GC.  this requires translating the destination region to (dstx, dsty).
    build a list of source points, one for each rectangle in the
destination.  this is a simple translation.
    go do the multiple rectangle copies
    do graphics exposures
*/

void aedCopyAreaFromPix();

#ifdef __HIGHC__
#define MOVE _move
#else
#define MOVE bcopy
#endif __HIGHC__

void aedCopyArea(pSrcDrawable, pDstDrawable,
	    pGC, srcx, srcy, width, height, dstx, dsty)
register DrawablePtr pSrcDrawable;
register DrawablePtr pDstDrawable;
GC *pGC;
int srcx, srcy;
int width, height;
int dstx, dsty;
{
    BoxRec srcBox;
    RegionPtr prgnSrcClip;	/* may be a new region, or just a copy */
    int realSrcClip = 0;	/* non-0 if we've created a src clip */

    RegionPtr prgnDst;
    register BoxPtr pbox;
    int i;
    register int dx;
    register int dy;
    xRectangle origSource;
    DDXPointRec origDest;
    int nbox;
    BoxPtr pboxTmp, pboxNext, pboxBase, pboxNew1, pboxNew2;
				/* temporaries for shuffling rectangles */

    if (!(pGC->planemask & 1))
	return;

    /*
     * If the destination drawable is not a window then call the mi version.
     * If the source is also a window do it here otherwise call the Pixmap to
     *     screen version.
     */
    if ( pDstDrawable->type != DRAWABLE_WINDOW )
    {
	miCopyArea(pSrcDrawable, pDstDrawable,
	    pGC, srcx, srcy, width, height, dstx, dsty);
	return;
    }
    else if ( pSrcDrawable->type != DRAWABLE_WINDOW )
    {
	aedCopyAreaFromPix(pSrcDrawable, pDstDrawable,
	    pGC, srcx, srcy, width, height, dstx, dsty);
	return;
    }
    TRACE(("aedCopyArea(pSrcDrawable=0x%x, pDstDrawable=0x%x, pGC=0x%x, srcx=%d, srcy=%d, width=%d, height=%d, dstx=%d, dsty=%d)\n", pSrcDrawable, pDstDrawable, pGC, srcx, srcy, width, height,dstx,dsty));

    origSource.x = srcx;
    origSource.y = srcy;
    origSource.width = width;
    origSource.height = height;
    origDest.x = dstx;
    origDest.y = dsty;

    /*
       clip the left and top edges of the source
    */
    if (srcx < 0)
    {
        width += srcx;
        srcx = 0;
    }
    if (srcy < 0)
    {
        height += srcy;
        srcy = 0;
    }

    /* clip the source */

    srcx += ((WindowPtr)pSrcDrawable)->absCorner.x;
    srcy += ((WindowPtr)pSrcDrawable)->absCorner.y;
    prgnSrcClip = ((WindowPtr)pSrcDrawable)->clipList;

    srcBox.x1 = srcx;
    srcBox.y1 = srcy;
    srcBox.x2 = srcx + width;
    srcBox.y2 = srcy + height;

    prgnDst = miRegionCreate(&srcBox, 1);
    miIntersect(prgnDst, prgnDst, prgnSrcClip);

    if (!((WindowPtr)pDstDrawable)->realized)
	return;
    dstx += ((WindowPtr)pDstDrawable)->absCorner.x;
    dsty += ((WindowPtr)pDstDrawable)->absCorner.y;

    dx = srcx - dstx;
    dy = srcy - dsty;

    /* clip the shape of the dst to the destination composite clip */
    miTranslateRegion(prgnDst, -dx, -dy);
    miIntersect(prgnDst,
		prgnDst,
		((mfbPrivGC *)(pGC->devPriv))->pCompositeClip);

    pbox = prgnDst->rects;
    nbox = prgnDst->numRects;

    pboxNew1 = 0;
    pboxNew2 = 0;
    if ( (pbox->y1 + dy) < pbox->y1) 
    {
        /* walk source bottom to top */

	if (nbox > 1)
	{
	    /* keep ordering in each band, reverse order of bands */
	    pboxNew1 = (BoxPtr)ALLOCATE_LOCAL(sizeof(BoxRec) * nbox);
	    if(!pboxNew1)
	    {
	        DEALLOCATE_LOCAL(pboxNew1);
	        return;
	    }
	    pboxBase = pboxNext = pbox+nbox-1;
	    while (pboxBase >= pbox)
	    {
	        while ((pboxNext >= pbox) && 
		       (pboxBase->y1 == pboxNext->y1))
		    pboxNext--;
	        pboxTmp = pboxNext+1;
	        while (pboxTmp <= pboxBase)
	        {
		    *pboxNew1++ = *pboxTmp++;
	        }
	        pboxBase = pboxNext;
	    }
	    pboxNew1 -= nbox;
	    pbox = pboxNew1;
        }
    }

    if ( (pbox->x1 + dx) < pbox->x1)
    {
	/* walk source right to left */

	if (nbox > 1)
	{
	    /* reverse order of rects in each band */
	    pboxNew2 = (BoxPtr)ALLOCATE_LOCAL(sizeof(BoxRec) * nbox);
	    pboxBase = pboxNext = pbox;
	    if(!pboxNew2)
	    {
	        DEALLOCATE_LOCAL(pboxNew2);
	        return;
	    }
	    while (pboxBase < pbox+nbox)
	    {
	        while ((pboxNext < pbox+nbox) &&
		       (pboxNext->y1 == pboxBase->y1))
		    pboxNext++;
	        pboxTmp = pboxNext;
	        while (pboxTmp != pboxBase)
	        {
		    *pboxNew2++ = *--pboxTmp;
	        }
	        pboxBase = pboxNext;
	    }
	    pboxNew2 -= nbox;
	    pbox = pboxNew2;
	}
    }

/*
    pbox = prgnDst->rects;
    nbox = prgnDst->numRects;
*/

    vforce();
    vikint[1] = 3;			/* copy area command */
    vikint[8] = mergexlate[pGC->alu];	/* merge mode */
    vikint[9] = vikint[10] = 0;		/* area colors */
    for (i=0; i<nbox; i++, pbox++)
    {
	vikint[2] = pbox->x1 + dx;	/* src x */
	vikint[3] = pbox->y1 + dy;	/* src y */
	vikint[4] = pbox->x1;		/* dest x */
	vikint[5] = pbox->y1;		/* dest y */
	vikint[6] = pbox->x2 - pbox->x1;/* area width */
	vikint[7] = pbox->y2 - pbox->y1;/* area height */
	vikwait();
	command(10);
    }

    /* free up stuff */
    if (pboxNew1)
    {
	DEALLOCATE_LOCAL(pboxNew1);
    }
    if (pboxNew2)
    {
	DEALLOCATE_LOCAL(pboxNew2);
    }


    if (((mfbPrivGC *)(pGC->devPriv))->fExpose)
        miHandleExposures(pSrcDrawable, pDstDrawable, pGC,
		          origSource.x, origSource.y,
		          origSource.width, origSource.height,
		          origDest.x, origDest.y);
		
    miRegionDestroy(prgnDst);
    if (realSrcClip)
	miRegionDestroy(prgnSrcClip);
}

void aedCopyAreaFromPix(pSrcDrawable, pDstDrawable,
	    pGC, srcx, srcy, width, height, dstx, dsty)
register DrawablePtr pSrcDrawable;
register DrawablePtr pDstDrawable;
GC *pGC;
int srcx, srcy;
int width, height;
int dstx, dsty;
{

    BoxRec srcBox;
    RegionPtr prgnSrcClip;	/* may be a new region, or just a copy */
    int realSrcClip = 0;	/* non-0 if we've created a src clip */

    RegionPtr prgnDst;
    DDXPointPtr pptSrc;
    register DDXPointPtr ppt;
    register BoxPtr pbox;
    int i;
    register int dx;
    register int dy;
    xRectangle origSource;
    DDXPointRec origDest;
    BoxRec box;
    int srcWidth, srcHeight, srclen;
    short *bits;
    int skip, j, nbox;


    TRACE(("aedCopyAreaFromPix(pSrcDrawable=0x%x, pDstDrawable=0x%x, pGC=0x%x, srcx=%d, srcy=%d, width=%d, height=%d, dstx=%d, dsty=%d)\n", pSrcDrawable, pDstDrawable, pGC, srcx, srcy, width, height,dstx,dsty));

    vforce();

    srcWidth = ((PixmapPtr)pSrcDrawable)->width;
    srcHeight = ((PixmapPtr)pSrcDrawable)->height;
    srclen = ( srcWidth + 15 ) / 16;
    if ( ( srcHeight * srclen ) > 2000 )
    {
	miCopyArea(pSrcDrawable, pDstDrawable,
	    pGC, srcx, srcy, width, height, dstx, dsty);
	return;
    }

    origSource.x = srcx;
    origSource.y = srcy;
    origSource.width = width;
    origSource.height = height;
    origDest.x = dstx;
    origDest.y = dsty;

    /*
       clip the left and top edges of the source
    */
    if (srcx < 0)
    {
        width += srcx;
        srcx = 0;
    }
    if (srcy < 0)
    {
        height += srcy;
        srcy = 0;
    }

    /* clip the source */



    box.x1 = 0;
    box.y1 = 0;
    box.x2 = ((PixmapPtr)pSrcDrawable)->width;
    box.y2 = ((PixmapPtr)pSrcDrawable)->height;

    prgnSrcClip = (*pGC->pScreen->RegionCreate)(&box, 1);
    realSrcClip = 1;

    srcBox.x1 = srcx;
    srcBox.y1 = srcy;
    srcBox.x2 = srcx + width;
    srcBox.y2 = srcy + height;

    prgnDst = (*pGC->pScreen->RegionCreate)(&srcBox, 1);
    (*pGC->pScreen->Intersect)(prgnDst, prgnDst, prgnSrcClip);

    if (!((WindowPtr)pDstDrawable)->realized)
    {
	miSendNoExpose(pGC);
	(*pGC->pScreen->RegionDestroy)(prgnDst);
	if (realSrcClip)
	    (*pGC->pScreen->RegionDestroy)(prgnSrcClip);
	return;
    }
    dstx += ((WindowPtr)pDstDrawable)->absCorner.x;
    dsty += ((WindowPtr)pDstDrawable)->absCorner.y;

    dx = srcx - dstx;
    dy = srcy - dsty;

    /* clip the shape of the dst to the destination composite clip */
    (*pGC->pScreen->TranslateRegion)(prgnDst, -dx, -dy);
    (*pGC->pScreen->Intersect)(prgnDst,
		prgnDst,
		((mfbPrivGC *)(pGC->devPriv))->pCompositeClip);

    if (!prgnDst->numRects)
    {
	if (((mfbPrivGC *)(pGC->devPriv))->fExpose)
	    miHandleExposures(pSrcDrawable, pDstDrawable, pGC,
			      origSource.x, origSource.y,
			      origSource.width, origSource.height,
			      origDest.x, origDest.y);
	(*pGC->pScreen->RegionDestroy)(prgnDst);
	if (realSrcClip)
	    (*pGC->pScreen->RegionDestroy)(prgnSrcClip);
	return;
    }
    if(!(pptSrc = (DDXPointPtr)ALLOCATE_LOCAL( prgnDst->numRects *
        sizeof(DDXPointRec))))
    {
	(*pGC->pScreen->RegionDestroy)(prgnDst);
	if (realSrcClip)
	    (*pGC->pScreen->RegionDestroy)(prgnSrcClip);
	return;
    }
    pbox = prgnDst->rects;
    ppt = pptSrc;
    for (i=0; i<prgnDst->numRects; i++, pbox++, ppt++)
    {
	ppt->x = pbox->x1 + dx;
	ppt->y = pbox->y1 + dy;
    }

    nbox = prgnDst->numRects;
    pbox = prgnDst->rects;
    ppt = pptSrc;
    if ( nbox )
    {
	clear(2);
/* computed above now!
	srcWidth = ((PixmapPtr)pSrcDrawable)->width;
	srcHeight = ((PixmapPtr)pSrcDrawable)->height;
	srclen = ( srcWidth + 15 ) / 16;
*/
	bits = ((PixmapPtr)pSrcDrawable)->devPrivate;
	skip = (((srcWidth-1) & 0x1f) < 16);
	vikint[ORMERGE] = mergexlate[pGC->alu];
	vikint[ORXPOSN] = dstx;
	vikint[ORYPOSN] = dsty;
	vikint[ORCLIPLX] = pbox->x1;
	vikint[ORCLIPLY] = pbox->y1;
	vikint[ORCLIPHX] = pbox->x2-1;
	vikint[ORCLIPHY] = pbox->y2-1;
	pbox++;
	nbox--;

	vikint[vikoff++] = 9;		/* draw image order */
	vikint[vikoff++] = srcWidth;
	vikint[vikoff++] = srcHeight;
	if ( skip )
	{
            for(j=0; j<srcHeight; j++)
            {
        	MOVE(bits, vikint+vikoff, srclen<<1 );
        	bits = bits + srclen + 1; /* skip the last 16 pad bits */
        	vikoff += srclen;
            }
	}
	else
	{
            bcopy(bits, vikint+vikoff, ( ( srclen<<1 ) * srcHeight ) );
            vikoff += ( srclen * srcHeight );
	}

	vforce();

	vikint[VIKCMD]      = 2;            /* reprocess orders */
	for(i=0; i<nbox; i++, pbox++)
	{
            vikint[ORCLIPLX] = pbox->x1;
            vikint[ORCLIPLY] = pbox->y1;
            vikint[ORCLIPHX] = pbox->x2-1;
            vikint[ORCLIPHY] = pbox->y2-1;
            command(ORDATA);
	}
    }
    clear(2);

    if (((mfbPrivGC *)(pGC->devPriv))->fExpose)
        miHandleExposures(pSrcDrawable, pDstDrawable, pGC,
		          origSource.x, origSource.y,
		          origSource.width, origSource.height,
		          origDest.x, origDest.y);
		
    DEALLOCATE_LOCAL(pptSrc);
    (*pGC->pScreen->RegionDestroy)(prgnDst);
    if (realSrcClip)
	(*pGC->pScreen->RegionDestroy)(prgnSrcClip);
}
