/* Combined Purdue/PurduePlus patches, level 2.0, 1/17/89 */
/***********************************************************

Copyright (c) 1987  X Consortium

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
X CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of the X Consortium shall not be
used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization from the X Consortium.


Copyright 1987 by Digital Equipment Corporation, Maynard, Massachusetts.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the name of Digital not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.  

DIGITAL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

Copyright 1992, 1993 Data General Corporation;
Copyright 1992, 1993 OMRON Corporation  

Permission to use, copy, modify, distribute, and sell this software and its
documentation for any purpose is hereby granted without fee, provided that the
above copyright notice appear in all copies and that both that copyright
notice and this permission notice appear in supporting documentation, and that
neither the name OMRON or DATA GENERAL be used in advertising or publicity
pertaining to distribution of the software without specific, written prior
permission of the party whose name is to be used.  Neither OMRON or 
DATA GENERAL make any representation about the suitability of this software
for any purpose.  It is provided "as is" without express or implied warranty.  

OMRON AND DATA GENERAL EACH DISCLAIM ALL WARRANTIES WITH REGARD TO THIS
SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS,
IN NO EVENT SHALL OMRON OR DATA GENERAL BE LIABLE FOR ANY SPECIAL, INDIRECT
OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
OF THIS SOFTWARE.

******************************************************************/
/* $XConsortium: mfbfillsp.c,v 1.5 94/04/17 21:16:12 dpw Exp $ */
#include "X.h"
#include "Xmd.h"
#include "gcstruct.h"
#include "window.h"
#include "pixmapstr.h"
#include "scrnintstr.h"
#include "windowstr.h"
#include "mfb.h"
#include "maskbits.h"

#include "mergerop.h"

#include "servermd.h"

#include "mtxlock.h"

#ifdef XTHREADS
#ifndef TRANSLATE_COORDS
#undef MAYBE_COME_UP_FOR_AIR
#define MAYBE_COME_UP_FOR_AIR()
#endif
#define MTX_MAYBE_COME_UP_FOR_AIR() MAYBE_COME_UP_FOR_AIR()
#else /* XTHREADS */
#define MTX_MAYBE_COME_UP_FOR_AIR() /* nothing */
#endif /* XTHREADS */

/* scanline filling for monochrome frame buffer
   written by drewry, oct 1986

   these routines all clip.  they assume that anything that has called
them has already translated the points (i.e. pGC->miTranslate is
non-zero, which is howit gets set in mfbCreateGC().)

    MTX: If TRANSLATE_COORDS is defined, these routines assume that
	anything that has called them has NOT already translated the
	points (i.e. pGC->miTranslate is 0).  This translation now occurs
	in miClipSpans().

   the number of new scnalines created by clipping ==
MaxRectsPerBand * nSpans.

    FillSolid is overloaded to be used for OpaqueStipple as well,
if fgPixel == bgPixel.  


    FillTiled is overloaded to be used for OpaqueStipple, if
fgPixel != bgPixel.  based on the fill style, it uses
{RotatedPixmap, gc.alu} or {RotatedPixmap, PrivGC.ropOpStip}
*/


void
mfbBlackSolidFS(pDrawable, pGC, nInit, pptInit, pwidthInit, fSorted)
    DrawablePtr pDrawable;
    GCPtr	pGC;
    int		nInit;		/* number of spans to fill */
    DDXPointPtr pptInit;	/* pointer to list of start points */
    int		*pwidthInit;	/* pointer to list of n widths */
    int 	fSorted;
{
				/* next three parameters are post-clip */
    int n;			/* number of spans to fill */
    register DDXPointPtr ppt;	/* pointer to list of start points */
    register int *pwidth;	/* pointer to list of n widths */
    PixelType *addrlBase;	/* pointer to start of bitmap */
    int nlwidth;		/* width in longwords of bitmap */
    register PixelType *addrl;/* pointer to current longword in bitmap */
    register int nlmiddle;
    register PixelType startmask;
    register PixelType endmask;
    int *pwidthFree;		/* copies of the pointers to free */
    DDXPointPtr pptFree;
    mfbPrivGC *devPriv;         /* pointer to private GC */

    if (!(pGC->planemask & 1))
	return;

    MTX_MAYBE_COME_UP_FOR_AIR();

    devPriv = ((mfbPrivGC *)(pGC->devPrivates[mfbGCPrivateIndex].ptr));
    n = nInit * miFindMaxBand(devPriv->pCompositeClip);

    pwidthFree = (int *)ALLOCATE_LOCAL(n * sizeof(int));
    pptFree = (DDXPointRec *)ALLOCATE_LOCAL(n * sizeof(DDXPointRec));
    if(!pptFree || !pwidthFree)
    {
	if (pptFree) DEALLOCATE_LOCAL(pptFree);
	if (pwidthFree) DEALLOCATE_LOCAL(pwidthFree);
	return;
    }
    pwidth = pwidthFree;
    ppt = pptFree;
    n = miClipSpans(devPriv->pCompositeClip, pptInit, pwidthInit, nInit,
		    ppt, pwidth, fSorted
#if defined(XTHREADS) && defined(TRANSLATE_COORDS)
			, pDrawable->x, pDrawable->y
#endif
		    );

    mfbGetPixelWidthAndPointer(pDrawable, nlwidth, addrlBase);

    while (n--)
    {
        addrl = mfbScanline(addrlBase, ppt->x, ppt->y, nlwidth);

	if (*pwidth)
	{
	    if ( ((ppt->x & PIM) + *pwidth) < PPW)
	    {
		/* all bits inside same longword */
		maskpartialbits(ppt->x, *pwidth, startmask);
		    *addrl &= ~startmask;
	    }
	    else
	    {
		maskbits(ppt->x, *pwidth, startmask, endmask, nlmiddle);
		if (startmask)
		    *addrl++ &= ~startmask;
		Duff (nlmiddle, *addrl++ = 0x0);
		if (endmask)
		    *addrl &= ~endmask;
	    }
	}
	pwidth++;
	ppt++;
    }
    DEALLOCATE_LOCAL(pptFree);
    DEALLOCATE_LOCAL(pwidthFree);
}



void
mfbWhiteSolidFS(pDrawable, pGC, nInit, pptInit, pwidthInit, fSorted)
    DrawablePtr pDrawable;
    GCPtr	pGC;
    int		nInit;		/* number of spans to fill */
    DDXPointPtr pptInit;	/* pointer to list of start points */
    int		*pwidthInit;	/* pointer to list of n widths */
    int 	fSorted;
{
				/* next three parameters are post-clip */
    int n;			/* number of spans to fill */
    register DDXPointPtr ppt;	/* pointer to list of start points */
    register int *pwidth;	/* pointer to list of n widths */
    PixelType *addrlBase;	/* pointer to start of bitmap */
    int nlwidth;		/* width in longwords of bitmap */
    register PixelType *addrl;/* pointer to current longword in bitmap */
    register int nlmiddle;
    register PixelType startmask;
    register PixelType endmask;
    int *pwidthFree;		/* copies of the pointers to free */
    DDXPointPtr pptFree;
    mfbPrivGC *devPriv;         /* pointer to private GC */

    if (!(pGC->planemask & 1))
	return;

    MTX_MAYBE_COME_UP_FOR_AIR();

    devPriv = ((mfbPrivGC *)(pGC->devPrivates[mfbGCPrivateIndex].ptr));
    n = nInit * miFindMaxBand(devPriv->pCompositeClip);

    pwidthFree = (int *)ALLOCATE_LOCAL(n * sizeof(int));
    pptFree = (DDXPointRec *)ALLOCATE_LOCAL(n * sizeof(DDXPointRec));
    if(!pptFree || !pwidthFree)
    {
	if (pptFree) DEALLOCATE_LOCAL(pptFree);
	if (pwidthFree) DEALLOCATE_LOCAL(pwidthFree);
	return;
    }
    pwidth = pwidthFree;
    ppt = pptFree;
    n = miClipSpans(devPriv->pCompositeClip, pptInit, pwidthInit, nInit,
		    ppt, pwidth, fSorted
#if defined(XTHREADS) && defined(TRANSLATE_COORDS)
			, pDrawable->x, pDrawable->y
#endif
		    );

    mfbGetPixelWidthAndPointer(pDrawable, nlwidth, addrlBase);

    while (n--)
    {
        addrl = mfbScanline(addrlBase, ppt->x, ppt->y, nlwidth);

	if (*pwidth)
	{
	    if ( ((ppt->x & PIM) + *pwidth) < PPW)
	    {
		/* all bits inside same longword */
		maskpartialbits(ppt->x, *pwidth, startmask);
		*addrl |= startmask;
	    }
	    else
	    {
		maskbits(ppt->x, *pwidth, startmask, endmask, nlmiddle);
		if (startmask)
		    *addrl++ |= startmask;
		Duff (nlmiddle, *addrl++ = ~0);
		if (endmask)
		    *addrl |= endmask;
	    }
	}
	pwidth++;
	ppt++;
    }
    DEALLOCATE_LOCAL(pptFree);
    DEALLOCATE_LOCAL(pwidthFree);
}



void
mfbInvertSolidFS(pDrawable, pGC, nInit, pptInit, pwidthInit, fSorted)
    DrawablePtr pDrawable;
    GCPtr	pGC;
    int		nInit;		/* number of spans to fill */
    DDXPointPtr pptInit;	/* pointer to list of start points */
    int		*pwidthInit;	/* pointer to list of n widths */
    int 	fSorted;
{
				/* next three parameters are post-clip */
    int n;			/* number of spans to fill */
    register DDXPointPtr ppt;	/* pointer to list of start points */
    register int *pwidth;	/* pointer to list of n widths */
    PixelType *addrlBase;	/* pointer to start of bitmap */
    int nlwidth;		/* width in longwords of bitmap */
    register PixelType *addrl;/* pointer to current longword in bitmap */
    register int nlmiddle;
    register PixelType startmask;
    register PixelType endmask;
    int *pwidthFree;		/* copies of the pointers to free */
    DDXPointPtr pptFree;
    mfbPrivGC *devPriv;         /* pointer to private GC */

    if (!(pGC->planemask & 1))
	return;

    MTX_MAYBE_COME_UP_FOR_AIR();

    devPriv = ((mfbPrivGC *)(pGC->devPrivates[mfbGCPrivateIndex].ptr));
    n = nInit * miFindMaxBand(devPriv->pCompositeClip);

    pwidthFree = (int *)ALLOCATE_LOCAL(n * sizeof(int));
    pptFree = (DDXPointRec *)ALLOCATE_LOCAL(n * sizeof(DDXPointRec));
    if(!pptFree || !pwidthFree)
    {
	if (pptFree) DEALLOCATE_LOCAL(pptFree);
	if (pwidthFree) DEALLOCATE_LOCAL(pwidthFree);
	return;
    }
    pwidth = pwidthFree;
    ppt = pptFree;
    n = miClipSpans(devPriv->pCompositeClip, pptInit, pwidthInit, nInit,
		    ppt, pwidth, fSorted
#if defined(XTHREADS) && defined(TRANSLATE_COORDS)
			, pDrawable->x, pDrawable->y
#endif
		    );

    mfbGetPixelWidthAndPointer(pDrawable, nlwidth, addrlBase);

    while (n--)
    {
        addrl = mfbScanline(addrlBase, ppt->x, ppt->y, nlwidth);

	if (*pwidth)
	{
	    if ( ((ppt->x & PIM) + *pwidth) < PPW)
	    {
		/* all bits inside same longword */
		maskpartialbits(ppt->x, *pwidth, startmask);
		*addrl ^= startmask;
	    }
	    else
	    {
		maskbits(ppt->x, *pwidth, startmask, endmask, nlmiddle);
		if (startmask)
		    *addrl++ ^= startmask;
		Duff (nlmiddle, *addrl++ ^= ~0);
		if (endmask)
		    *addrl ^= endmask;
	    }
	}
	pwidth++;
	ppt++;
    }
    DEALLOCATE_LOCAL(pptFree);
    DEALLOCATE_LOCAL(pwidthFree);
}


void 
mfbWhiteStippleFS(pDrawable, pGC, nInit, pptInit, pwidthInit, fSorted)
    DrawablePtr pDrawable;
    GC *pGC;
    int nInit;			/* number of spans to fill */
    DDXPointPtr pptInit;	/* pointer to list of start points */
    int *pwidthInit;		/* pointer to list of n widths */
    int fSorted;
{
				/* next three parameters are post-clip */
    int n;			/* number of spans to fill */
    register DDXPointPtr ppt;	/* pointer to list of start points */
    register int *pwidth;	/* pointer to list of n widths */
    PixelType *addrlBase;	/* pointer to start of bitmap */
    int nlwidth;		/* width in longwords of bitmap */
    register PixelType *addrl;/* pointer to current longword in bitmap */
    register PixelType src;
    register int nlmiddle;
    register PixelType startmask;
    register PixelType endmask;
    PixmapPtr pStipple;
    PixelType *psrc;
    int tileHeight;
    int *pwidthFree;		/* copies of the pointers to free */
    DDXPointPtr pptFree;
    mfbPrivGC *devPriv;         /* pointer to private GC */

    if (!(pGC->planemask & 1))
	return;

    MTX_MAYBE_COME_UP_FOR_AIR();

    devPriv = ((mfbPrivGC *)(pGC->devPrivates[mfbGCPrivateIndex].ptr));
    n = nInit * miFindMaxBand(devPriv->pCompositeClip);

    pwidthFree = (int *)ALLOCATE_LOCAL(n * sizeof(int));
    pptFree = (DDXPointRec *)ALLOCATE_LOCAL(n * sizeof(DDXPointRec));
    if(!pptFree || !pwidthFree)
    {
	if (pptFree) DEALLOCATE_LOCAL(pptFree);
	if (pwidthFree) DEALLOCATE_LOCAL(pwidthFree);
	return;
    }
    pwidth = pwidthFree;
    ppt = pptFree;
    n = miClipSpans(devPriv->pCompositeClip, pptInit, pwidthInit, nInit, 
		    ppt, pwidth, fSorted
#if defined(XTHREADS) && defined(TRANSLATE_COORDS)
			, pDrawable->x, pDrawable->y
#endif
		    );

    mfbGetPixelWidthAndPointer(pDrawable, nlwidth, addrlBase);

    pStipple = devPriv->pRotatedPixmap;
    tileHeight = pStipple->drawable.height;
    psrc = (PixelType *)(pStipple->devPrivate.ptr);

    while (n--)
    {
        addrl = mfbScanline(addrlBase, ppt->x, ppt->y, nlwidth);
	src = psrc[ppt->y % tileHeight];

        /* all bits inside same longword */
        if ( ((ppt->x & PIM) + *pwidth) < PPW)
        {
	    maskpartialbits(ppt->x, *pwidth, startmask);
	    *addrl |= (src & startmask);
        }
        else
        {
	    maskbits(ppt->x, *pwidth, startmask, endmask, nlmiddle);
	    if (startmask)
		*addrl++ |= (src & startmask);
	    Duff (nlmiddle, *addrl++ |= src);
	    if (endmask)
		*addrl |= (src & endmask);
        }
	pwidth++;
	ppt++;
    }
    DEALLOCATE_LOCAL(pptFree);
    DEALLOCATE_LOCAL(pwidthFree);
}


void 
mfbBlackStippleFS(pDrawable, pGC, nInit, pptInit, pwidthInit, fSorted)
    DrawablePtr pDrawable;
    GC *pGC;
    int nInit;			/* number of spans to fill */
    DDXPointPtr pptInit;	/* pointer to list of start points */
    int *pwidthInit;		/* pointer to list of n widths */
    int fSorted;
{
				/* next three parameters are post-clip */
    int n;			/* number of spans to fill */
    register DDXPointPtr ppt;	/* pointer to list of start points */
    register int *pwidth;	/* pointer to list of n widths */
    PixelType *addrlBase;	/* pointer to start of bitmap */
    int nlwidth;		/* width in longwords of bitmap */
    register PixelType *addrl;	/* pointer to current longword in bitmap */
    register PixelType src;
    register int nlmiddle;
    register PixelType startmask;
    register PixelType endmask;
    PixmapPtr pStipple;
    PixelType *psrc;
    int tileHeight;
    int *pwidthFree;		/* copies of the pointers to free */
    DDXPointPtr pptFree;
    mfbPrivGC *devPriv;         /* pointer to private GC */

    if (!(pGC->planemask & 1))
	return;

    MTX_MAYBE_COME_UP_FOR_AIR();

    devPriv = ((mfbPrivGC *)(pGC->devPrivates[mfbGCPrivateIndex].ptr));
    n = nInit * miFindMaxBand(devPriv->pCompositeClip);

    pwidthFree = (int *)ALLOCATE_LOCAL(n * sizeof(int));
    pptFree = (DDXPointRec *)ALLOCATE_LOCAL(n * sizeof(DDXPointRec));
    if(!pptFree || !pwidthFree)
    {
	if (pptFree) DEALLOCATE_LOCAL(pptFree);
	if (pwidthFree) DEALLOCATE_LOCAL(pwidthFree);
	return;
    }
    pwidth = pwidthFree;
    ppt = pptFree;
    n = miClipSpans(devPriv->pCompositeClip, pptInit, pwidthInit, nInit, 
		    ppt, pwidth, fSorted
#if defined(XTHREADS) && defined(TRANSLATE_COORDS)
			, pDrawable->x, pDrawable->y
#endif
);

    mfbGetPixelWidthAndPointer(pDrawable, nlwidth, addrlBase);

    pStipple = devPriv->pRotatedPixmap;
    tileHeight = pStipple->drawable.height;
    psrc = (PixelType *)(pStipple->devPrivate.ptr);

    while (n--)
    {
        addrl = mfbScanline(addrlBase, ppt->x, ppt->y, nlwidth);
	src = psrc[ppt->y % tileHeight];

        /* all bits inside same longword */
        if ( ((ppt->x & PIM) + *pwidth) < PPW)
        {
	    maskpartialbits(ppt->x, *pwidth, startmask);
	    *addrl &= ~(src & startmask);
        }
        else
        {
	    maskbits(ppt->x, *pwidth, startmask, endmask, nlmiddle);
	    if (startmask)
		*addrl++ &= ~(src & startmask);
	    Duff (nlmiddle, *addrl++ &= ~src);
	    if (endmask)
		*addrl &= ~(src & endmask);
        }
	pwidth++;
	ppt++;
    }
    DEALLOCATE_LOCAL(pptFree);
    DEALLOCATE_LOCAL(pwidthFree);
}


void 
mfbInvertStippleFS(pDrawable, pGC, nInit, pptInit, pwidthInit, fSorted)
    DrawablePtr pDrawable;
    GC *pGC;
    int nInit;			/* number of spans to fill */
    DDXPointPtr pptInit;	/* pointer to list of start points */
    int *pwidthInit;		/* pointer to list of n widths */
    int fSorted;
{
				/* next three parameters are post-clip */
    int n;			/* number of spans to fill */
    register DDXPointPtr ppt;	/* pointer to list of start points */
    register int *pwidth;	/* pointer to list of n widths */
    PixelType *addrlBase;	/* pointer to start of bitmap */
    int nlwidth;		/* width in longwords of bitmap */
    register PixelType *addrl;	/* pointer to current longword in bitmap */
    register PixelType src;
    register int nlmiddle;
    register PixelType startmask;
    register PixelType endmask;
    PixmapPtr pStipple;
    PixelType *psrc;
    int tileHeight;
    int *pwidthFree;		/* copies of the pointers to free */
    DDXPointPtr pptFree;
    mfbPrivGC *devPriv;         /* pointer to private GC */

    if (!(pGC->planemask & 1))
	return;

    MTX_MAYBE_COME_UP_FOR_AIR();

    devPriv = ((mfbPrivGC *)(pGC->devPrivates[mfbGCPrivateIndex].ptr));
    n = nInit * miFindMaxBand(devPriv->pCompositeClip);

    pwidthFree = (int *)ALLOCATE_LOCAL(n * sizeof(int));
    pptFree = (DDXPointRec *)ALLOCATE_LOCAL(n * sizeof(DDXPointRec));
    if(!pptFree || !pwidthFree)
    {
	if (pptFree) DEALLOCATE_LOCAL(pptFree);
	if (pwidthFree) DEALLOCATE_LOCAL(pwidthFree);
	return;
    }
    pwidth = pwidthFree;
    ppt = pptFree;
    n = miClipSpans(devPriv->pCompositeClip, pptInit, pwidthInit, nInit, 
		    ppt, pwidth, fSorted
#if defined(XTHREADS) && defined(TRANSLATE_COORDS)
			, pDrawable->x, pDrawable->y
#endif
);

    mfbGetPixelWidthAndPointer(pDrawable, nlwidth, addrlBase);

    pStipple = devPriv->pRotatedPixmap;
    tileHeight = pStipple->drawable.height;
    psrc = (PixelType *)(pStipple->devPrivate.ptr);

    while (n--)
    {
        addrl = mfbScanline(addrlBase, ppt->x, ppt->y, nlwidth);
	src = psrc[ppt->y % tileHeight];

        /* all bits inside same longword */
        if ( ((ppt->x & PIM) + *pwidth) < PPW)
        {
	    maskpartialbits(ppt->x, *pwidth, startmask);
	    *addrl ^= (src & startmask);
        }
        else
        {
	    maskbits(ppt->x, *pwidth, startmask, endmask, nlmiddle);
	    if (startmask)
		*addrl++ ^= (src & startmask);
	    Duff(nlmiddle, *addrl++ ^= src);
	    if (endmask)
		*addrl ^= (src & endmask);
        }
	pwidth++;
	ppt++;
    }
    DEALLOCATE_LOCAL(pptFree);
    DEALLOCATE_LOCAL(pwidthFree);
}


/* this works with tiles of width == PPW */
#define FILLSPANPPW(ROP) \
    while (n--) \
    { \
	if (*pwidth) \
	{ \
            addrl = mfbScanline(addrlBase, ppt->x, ppt->y, nlwidth); \
	    src = psrc[ppt->y % tileHeight]; \
            if ( ((ppt->x & PIM) + *pwidth) < PPW) \
            { \
	        maskpartialbits(ppt->x, *pwidth, startmask); \
	        *addrl = (*addrl & ~startmask) | \
		         (ROP(src, *addrl) & startmask); \
            } \
            else \
            { \
	        maskbits(ppt->x, *pwidth, startmask, endmask, nlmiddle); \
	        if (startmask) \
	        { \
	            *addrl = (*addrl & ~startmask) | \
			     (ROP(src, *addrl) & startmask); \
		    addrl++; \
	        } \
	        while (nlmiddle--) \
	        { \
		    *addrl = ROP(src, *addrl); \
		    addrl++; \
	        } \
	        if (endmask) \
	            *addrl = (*addrl & ~endmask) | \
			     (ROP(src, *addrl) & endmask); \
            } \
	} \
	pwidth++; \
	ppt++; \
    }



void
mfbTileFS(pDrawable, pGC, nInit, pptInit, pwidthInit, fSorted)
    DrawablePtr pDrawable;
    GC *pGC;
    int nInit;			/* number of spans to fill */
    DDXPointPtr pptInit;	/* pointer to list of start points */
    int *pwidthInit;		/* pointer to list of n widths */
    int fSorted;
{
				/* next three parameters are post-clip */
    int n;			/* number of spans to fill */
    register DDXPointPtr ppt;	/* pointer to list of start points */
    register int *pwidth;	/* pointer to list of n widths */
    PixelType *addrlBase;	/* pointer to start of bitmap */
    int nlwidth;		/* width in longwords of bitmap */
    register PixelType *addrl;	/* pointer to current longword in bitmap */
    register PixelType src;
    register int nlmiddle;
    register PixelType startmask;
    register PixelType endmask;
    PixmapPtr pTile;
    PixelType *psrc;
    int tileHeight;
    int rop;
    int *pwidthFree;		/* copies of the pointers to free */
    DDXPointPtr pptFree;
    unsigned long   flip;
    mfbPrivGC *devPriv;         /* pointer to private GC */


    if (!(pGC->planemask & 1))
	return;

    MTX_MAYBE_COME_UP_FOR_AIR();

    devPriv = ((mfbPrivGC *)(pGC->devPrivates[mfbGCPrivateIndex].ptr));
    n = nInit * miFindMaxBand(devPriv->pCompositeClip);

    pwidthFree = (int *)ALLOCATE_LOCAL(n * sizeof(int));
    pptFree = (DDXPointRec *)ALLOCATE_LOCAL(n * sizeof(DDXPointRec));
    if(!pptFree || !pwidthFree)
    {
	if (pptFree) DEALLOCATE_LOCAL(pptFree);
	if (pwidthFree) DEALLOCATE_LOCAL(pwidthFree);
	return;
    }
    pwidth = pwidthFree;
    ppt = pptFree;
    n = miClipSpans(devPriv->pCompositeClip, pptInit, pwidthInit, nInit, 
		    ppt, pwidth, fSorted
#if defined(XTHREADS) && defined(TRANSLATE_COORDS)
			, pDrawable->x, pDrawable->y
#endif
		    );

    mfbGetPixelWidthAndPointer(pDrawable, nlwidth, addrlBase);

    pTile = devPriv->pRotatedPixmap;
    tileHeight = pTile->drawable.height;
    psrc = (PixelType *)(pTile->devPrivate.ptr);
    if (pGC->fillStyle == FillTiled)
	rop = pGC->alu;
    else
	rop = devPriv->ropOpStip;

    flip = 0;
    switch(rop)
    {
      case GXcopyInverted:  /* for opaque stipples */
	flip = ~0;
      case GXcopy:
	{

#define DoMaskCopyRop(src,dst,mask)	((dst) & ~(mask) | (src) & (mask))

	    while (n--)
	    {
	    	if (*pwidth)
	    	{
            	    addrl = mfbScanline(addrlBase, ppt->x, ppt->y, nlwidth);
	    	    src = psrc[ppt->y % tileHeight] ^ flip;
            	    if ( ((ppt->x & PIM) + *pwidth) < PPW)
            	    {
	            	maskpartialbits(ppt->x, *pwidth, startmask);
			*addrl = DoMaskCopyRop (src, *addrl, startmask);
            	    }
            	    else
            	    {
	            	maskbits(ppt->x, *pwidth, startmask, endmask, nlmiddle);
	            	if (startmask)
	            	{
			    *addrl = DoMaskCopyRop (src, *addrl, startmask);
		    	    addrl++;
	            	}
	            	while (nlmiddle--)
	            	{
			    *addrl = src;
		    	    addrl++;
	            	}
	            	if (endmask)
			    *addrl = DoMaskCopyRop (src, *addrl, endmask);
            	    }
	    	}
	    	pwidth++;
	    	ppt++;
	    }
	}
	break;
      default:
	{
	    register DeclareMergeRop ();

	    InitializeMergeRop(rop,~0);
	    while (n--)
	    {
	    	if (*pwidth)
	    	{
            	    addrl = mfbScanline(addrlBase, ppt->x, ppt->y, nlwidth);
	    	    src = psrc[ppt->y % tileHeight];
            	    if ( ((ppt->x & PIM) + *pwidth) < PPW)
            	    {
	            	maskpartialbits(ppt->x, *pwidth, startmask);
			*addrl = DoMaskMergeRop (src, *addrl, startmask);
            	    }
            	    else
            	    {
	            	maskbits(ppt->x, *pwidth, startmask, endmask, nlmiddle);
	            	if (startmask)
	            	{
			    *addrl = DoMaskMergeRop (src, *addrl, startmask);
		    	    addrl++;
	            	}
	            	while (nlmiddle--)
	            	{
			    *addrl = DoMergeRop (src, *addrl);
		    	    addrl++;
	            	}
	            	if (endmask)
			    *addrl = DoMaskMergeRop (src, *addrl, endmask);
            	    }
	    	}
	    	pwidth++;
	    	ppt++;
	    }
	}
	break;
    }
    DEALLOCATE_LOCAL(pptFree);
    DEALLOCATE_LOCAL(pwidthFree);
}


/* Fill spans with tiles that aren't PPW bits wide */
void
mfbUnnaturalTileFS(pDrawable, pGC, nInit, pptInit, pwidthInit, fSorted)
    DrawablePtr pDrawable;
    GC		*pGC;
    int		nInit;		/* number of spans to fill */
    DDXPointPtr pptInit;	/* pointer to list of start points */
    int *pwidthInit;		/* pointer to list of n widths */
    int fSorted;
{
    int		iline;		/* first line of tile to use */
				/* next three parameters are post-clip */
    int n;			/* number of spans to fill */
    register DDXPointPtr ppt;	/* pointer to list of start points */
    register int *pwidth;	/* pointer to list of n widths */
    PixelType *addrlBase;	/* pointer to start of bitmap */
    int		 nlwidth;	/* width in longwords of bitmap */
    register PixelType *pdst;/* pointer to current word in bitmap */
    register PixelType *psrc;/* pointer to current word in tile */
    register int nlMiddle;
    register int rop, nstart;
    PixelType startmask;
    PixmapPtr	pTile;		/* pointer to tile we want to fill with */
    int		w, width, x, xSrc, ySrc, srcStartOver, nend;
    int 	tlwidth, rem, tileWidth, tileHeight, endinc;
    PixelType      endmask, *psrcT;
    int *pwidthFree;		/* copies of the pointers to free */
    DDXPointPtr pptFree;
    mfbPrivGC *devPriv;         /* pointer to private GC */

    if (!(pGC->planemask & 1))
	return;

    MTX_MAYBE_COME_UP_FOR_AIR();

    devPriv = ((mfbPrivGC *)(pGC->devPrivates[mfbGCPrivateIndex].ptr));
    n = nInit * miFindMaxBand(devPriv->pCompositeClip);

    pwidthFree = (int *)ALLOCATE_LOCAL(n * sizeof(int));
    pptFree = (DDXPointRec *)ALLOCATE_LOCAL(n * sizeof(DDXPointRec));
    if(!pptFree || !pwidthFree)
    {
	if (pptFree) DEALLOCATE_LOCAL(pptFree);
	if (pwidthFree) DEALLOCATE_LOCAL(pwidthFree);
	return;
    }
    pwidth = pwidthFree;
    ppt = pptFree;
    n = miClipSpans(devPriv->pCompositeClip, pptInit, pwidthInit, nInit, 
		    ppt, pwidth, fSorted
#if defined(XTHREADS) && defined(TRANSLATE_COORDS)
			, pDrawable->x, pDrawable->y
#endif
		    );

    if (pGC->fillStyle == FillTiled)
    {
	pTile = pGC->tile.pixmap;
	tlwidth = pTile->devKind / PGSZB;
	rop = pGC->alu;
    }
    else
    {
	pTile = pGC->stipple;
	tlwidth = pTile->devKind / PGSZB;
	rop = devPriv->ropOpStip;
    }

    xSrc = pDrawable->x;
    ySrc = pDrawable->y;

    mfbGetPixelWidthAndPointer(pDrawable, nlwidth, addrlBase);

    tileWidth = pTile->drawable.width;
    tileHeight = pTile->drawable.height;

    /* this replaces rotating the tile. Instead we just adjust the offset
     * at which we start grabbing bits from the tile.
     * Ensure that ppt->x - xSrc >= 0 and ppt->y - ySrc >= 0,
     * so that iline and rem always stay within the tile bounds.
     */
    xSrc += (pGC->patOrg.x % tileWidth) - tileWidth;
    ySrc += (pGC->patOrg.y % tileHeight) - tileHeight;

    while (n--)
    {
	iline = (ppt->y - ySrc) % tileHeight;
        pdst = mfbScanline(addrlBase, ppt->x, ppt->y, nlwidth);
        psrcT = (PixelType *) pTile->devPrivate.ptr + (iline * tlwidth);
	x = ppt->x;

	if (*pwidth)
	{
	    width = *pwidth;
	    while(width > 0)
	    {
		psrc = psrcT;
	        w = min(tileWidth, width);
		if((rem = (x - xSrc)  % tileWidth) != 0)
		{
		    /* if we're in the middle of the tile, get
		       as many bits as will finish the span, or
		       as many as will get to the left edge of the tile,
		       or a longword worth, starting at the appropriate
		       offset in the tile.
		    */
		    w = min(min(tileWidth - rem, width), BITMAP_SCANLINE_UNIT);
		    endinc = rem / BITMAP_SCANLINE_UNIT;
		    getandputrop((psrc+endinc), (rem&PIM), (x & PIM), w, pdst, rop);
		    if((x & PIM) + w >= PPW)
			pdst++;
		}
		else if(((x & PIM) + w) < PPW)
		{
		    /* doing < PPW bits is easy, and worth special-casing */
		    putbitsrop(*psrc, x & PIM, w, pdst, rop);
		}
		else
		{
		    /* start at the left edge of the tile,
		       and put down as much as we can
		    */
		    maskbits(x, w, startmask, endmask, nlMiddle);

	            if (startmask)
		        nstart = PPW - (x & PIM);
	            else
		        nstart = 0;
	            if (endmask)
	                nend = (x + w)  & PIM;
	            else
		        nend = 0;

	            srcStartOver = nstart > PLST;

		    if(startmask)
		    {
			putbitsrop(*psrc, (x & PIM), nstart, pdst, rop);
			pdst++;
			if(srcStartOver)
			    psrc++;
		    }
		     
		    while(nlMiddle--)
		    {
			    getandputrop0(psrc, nstart, PPW, pdst, rop);
			    pdst++;
			    psrc++;
		    }
		    if(endmask)
		    {
			getandputrop0(psrc, nstart, nend, pdst, rop);
		    }
		 }
		 x += w;
		 width -= w;
	    }
	}
	ppt++;
	pwidth++;
    }
    DEALLOCATE_LOCAL(pptFree);
    DEALLOCATE_LOCAL(pwidthFree);
}


/* Fill spans with stipples that aren't PPW bits wide */
void
mfbUnnaturalStippleFS(pDrawable, pGC, nInit, pptInit, pwidthInit, fSorted)
    DrawablePtr pDrawable;
    GC		*pGC;
    int		nInit;		/* number of spans to fill */
    DDXPointPtr pptInit;	/* pointer to list of start points */
    int *pwidthInit;		/* pointer to list of n widths */
    int fSorted;
{
				/* next three parameters are post-clip */
    int n;			/* number of spans to fill */
    register DDXPointPtr ppt;	/* pointer to list of start points */
    register int *pwidth;	/* pointer to list of n widths */
    int		iline;		/* first line of tile to use */
    PixelType		*addrlBase;	/* pointer to start of bitmap */
    int		 nlwidth;	/* width in longwords of bitmap */
    register PixelType *pdst;		/* pointer to current word in bitmap */
    register PixelType *psrc;		/* pointer to current word in tile */
    register int nlMiddle;
    register int rop, nstart;
    PixelType startmask;
    PixmapPtr	pTile;		/* pointer to tile we want to fill with */
    int		w, width,  x, xSrc, ySrc, srcStartOver, nend;
    PixelType 	endmask, *psrcT;
    int 	tlwidth, rem, tileWidth, endinc;
    int		tileHeight;
    int *pwidthFree;		/* copies of the pointers to free */
    DDXPointPtr pptFree;
    mfbPrivGC *devPriv;         /* pointer to private GC */

    if (!(pGC->planemask & 1))
	return;

    MTX_MAYBE_COME_UP_FOR_AIR();

    devPriv = ((mfbPrivGC *)(pGC->devPrivates[mfbGCPrivateIndex].ptr));
    n = nInit * miFindMaxBand(devPriv->pCompositeClip);

    pwidthFree = (int *)ALLOCATE_LOCAL(n * sizeof(int));
    pptFree = (DDXPointRec *)ALLOCATE_LOCAL(n * sizeof(DDXPointRec));
    if(!pptFree || !pwidthFree)
    {
	if (pptFree) DEALLOCATE_LOCAL(pptFree);
	if (pwidthFree) DEALLOCATE_LOCAL(pwidthFree);
	return;
    }
    pwidth = pwidthFree;
    ppt = pptFree;
    n = miClipSpans(devPriv->pCompositeClip, pptInit, pwidthInit, nInit, 
		    ppt, pwidth, fSorted
#if defined(XTHREADS) && defined(TRANSLATE_COORDS)
			, pDrawable->x, pDrawable->y
#endif
		    );

    pTile = pGC->stipple;
    rop = devPriv->rop;
    tlwidth = pTile->devKind / PGSZB;
    xSrc = pDrawable->x;
    ySrc = pDrawable->y;
    mfbGetPixelWidthAndPointer(pDrawable, nlwidth, addrlBase);

    tileWidth = pTile->drawable.width;
    tileHeight = pTile->drawable.height;

    /* this replaces rotating the stipple.  Instead, we just adjust the offset
     * at which we start grabbing bits from the stipple.
     * Ensure that ppt->x - xSrc >= 0 and ppt->y - ySrc >= 0,
     * so that iline and rem always stay within the tile bounds.
     */
    xSrc += (pGC->patOrg.x % tileWidth) - tileWidth;
    ySrc += (pGC->patOrg.y % tileHeight) - tileHeight;
    while (n--)
    {
	iline = (ppt->y - ySrc) % tileHeight;
        pdst = mfbScanline(addrlBase, ppt->x, ppt->y, nlwidth);
        psrcT = (PixelType *) pTile->devPrivate.ptr + (iline * tlwidth);
	x = ppt->x;

	if (*pwidth)
	{
	    width = *pwidth;
	    while(width > 0)
	    {
		psrc = psrcT;
	        w = min(tileWidth, width);
		if((rem = (x - xSrc) % tileWidth) != 0)
		{
		    /* if we're in the middle of the tile, get
		       as many bits as will finish the span, or
		       as many as will get to the left edge of the tile,
		       or a longword worth, starting at the appropriate
		       offset in the tile.
		    */
		    w = min(min(tileWidth - rem, width), BITMAP_SCANLINE_UNIT);
		    endinc = rem / BITMAP_SCANLINE_UNIT;
		    getandputrrop((psrc + endinc), (rem & PIM), (x & PIM),
				 w, pdst, rop)
		    if((x & PIM) + w >= PPW)
			pdst++;
		}

		else if(((x & PIM) + w) < PPW)
		{
		    /* doing < PPW bits is easy, and worth special-casing */
		    putbitsrrop(*psrc, x & PIM, w, pdst, rop);
		}
		else
		{
		    /* start at the left edge of the tile,
		       and put down as much as we can
		    */
		    maskbits(x, w, startmask, endmask, nlMiddle);

	            if (startmask)
		        nstart = PPW - (x & PIM);
	            else
		        nstart = 0;
	            if (endmask)
	                nend = (x + w)  & PIM;
	            else
		        nend = 0;

	            srcStartOver = nstart > PLST;

		    if(startmask)
		    {
			putbitsrrop(*psrc, (x & PIM), nstart, pdst, rop);
			pdst++;
			if(srcStartOver)
			    psrc++;
		    }
		     
		    while(nlMiddle--)
		    {
			    getandputrrop0(psrc, nstart, PPW, pdst, rop);
			    pdst++;
			    psrc++;
		    }
		    if(endmask)
		    {
			getandputrrop0(psrc, nstart, nend, pdst, rop);
		    }
		 }
		 x += w;
		 width -= w;
	    }
	}
	ppt++;
	pwidth++;
    }
    DEALLOCATE_LOCAL(pptFree);
    DEALLOCATE_LOCAL(pwidthFree);
}
