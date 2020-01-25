/*

Copyright (c) 1986, 1987 by Hewlett-Packard Company
Copyright (c) 1986, 1987 by the Massachusetts Institute of Technology

Permission to use, copy, modify, and distribute this
software and its documentation for any purpose and without
fee is hereby granted, provided that the above copyright
notice appear in all copies and that both that copyright
notice and this permission notice appear in supporting
documentation, and that the name of M.I.T. not be used in
advertising or publicity pertaining to distribution of the
software without specific, written prior permission.

HEWLETT-PACKARD MAKES NO WARRANTY OF ANY KIND WITH REGARD
TO THIS SOFWARE, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
PURPOSE.  Hewlett-Packard shall not be liable for errors 
contained herein or direct, indirect, special, incidental or 
consequential damages in connection with the furnishing, 
performance, or use of this material.

This software is not subject to any license of the American
Telephone and Telegraph Company or of the Regents of the
University of California.

*/
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
#include "X.h"
#include "Xmd.h"
#include "Xproto.h"
#include "dixfontstr.h"
#include "fontstruct.h"
#include "gcstruct.h"
#include "windowstr.h"
#include "pixmapstr.h"
#include "scrnintstr.h"
#include "region.h"

#include "../cfb/cfb.h"
#include "mistruct.h"

#include "ren.h"

#include "../cfb/cfbmskbits.h"
extern cfbXRotatePixmap();
extern cfbYRotatePixmap();
extern void mfbPushPixels();
extern void hpfbCopyArea();
extern void hpfbPolyFillRect();

void renValidateGC();

Bool
renCreateGC(pGC)
    register GCPtr pGC;
{
    GCInterestPtr pQ;

    switch (pGC->depth) {
    case 1:
	return (mfbCreateGC(pGC));
    case PSZ:
	break;
    default:
	ErrorF("renCreateGC: unsupported depth: %d\n", pGC->depth);
	return FALSE;
    }
    pGC->clientClip = NULL;
    pGC->clientClipType = CT_NONE;

    /*
     * some of the output primitives aren't really necessary, since they
     * will be filled in ValidateGC because of dix/CreateGC() setting all
     * the change bits.  Others are necessary because although they depend
     * on being a color frame buffer, they don't change 
     */

    pGC->FillSpans = cfbSolidFS;
    pGC->SetSpans = cfbSetSpans;
    pGC->PutImage = miPutImage;
    pGC->CopyArea = hpfbCopyArea;
    pGC->CopyPlane = miCopyPlane;
    pGC->PolyPoint = miPolyPoint;

#ifdef notdef
    pGC->Polylines = miNotMiter;    /* Doesn't work for 0-width lines */
#else
    pGC->Polylines = miZeroLine;
#endif notdef
    pGC->PolySegment = miPolySegment;
    pGC->PolyRectangle = miPolyRectangle;
    pGC->PolyArc = miPolyArc;
    pGC->FillPolygon = miFillPolygon;
    pGC->PolyFillRect = hpfbPolyFillRect;
    pGC->PolyFillArc = miPolyFillArc;
    pGC->PolyText8 = miPolyText8;
    pGC->ImageText8 = miImageText8;
    pGC->PolyText16 = miPolyText16;
    pGC->ImageText16 = miImageText16;
    pGC->ImageGlyphBlt = miImageGlyphBlt;
    pGC->PolyGlyphBlt = miPolyGlyphBlt;
#ifdef	notdef
    pGC->PushPixels = miPushPixels;	/* miPushPixels is garbage */
#else
    pGC->PushPixels = mfbPushPixels;	/* but mfbPushPixels isn't depth
					 * dependent */
#endif
    pGC->LineHelper = miMiter;
    pGC->ChangeClip = cfbChangeClip;
    pGC->DestroyClip = cfbDestroyClip;
    pGC->CopyClip = cfbCopyClip;

    /* cfb wants to translate before scan convesion */
    pGC->miTranslate = 1;

    {
	cfbPrivGC  *pPriv;

	pPriv = (cfbPrivGC *) Xalloc(sizeof(cfbPrivGC));
	if (!pPriv)
	    return FALSE;
	else {
	    pPriv->rop = pGC->alu;
	    pPriv->fExpose = TRUE;
	    pGC->devPriv = (pointer) pPriv;
	    pPriv->pRotatedTile = NullPixmap;
	    pPriv->pRotatedStipple = NullPixmap;
	    pPriv->pAbsClientRegion = (*pGC->pScreen->RegionCreate) (NULL, 1);
	    pPriv->pCompositeClip = (*pGC->pScreen->RegionCreate) (NULL, 1);
	    pPriv->freeCompClip = REPLACE_CC;
	}
    }
    pQ = (GCInterestPtr) Xalloc(sizeof(GCInterestRec));
    if (!pQ) {
	Xfree(pGC->devPriv);
	return FALSE;
    }

    /* Now link this device into the GCque */
    pGC->pNextGCInterest = pQ;
    pGC->pLastGCInterest = pQ;
    pQ->pNextGCInterest = (GCInterestPtr) & pGC->pNextGCInterest;
    pQ->pLastGCInterest = (GCInterestPtr) & pGC->pNextGCInterest;
    pQ->length = sizeof(GCInterestRec);
    pQ->owner = 0;		/* server owns this */
    pQ->ValInterestMask = ~0;	/* interested in everything at validate
				 * time */
    pQ->ValidateGC = renValidateGC;
    pQ->ChangeInterestMask = 0;	/* interested in nothing at change time */
    pQ->ChangeGC = (int (*) ()) NULL;
    pQ->CopyGCSource = (void (*) ()) NULL;
    pQ->CopyGCDest = cfbCopyGCDest;
    pQ->DestroyGC = cfbDestroyGC;
    return TRUE;
}


#define WINMOVED(pWin, pGC) \
((pWin->absCorner.x != pGC->lastWinOrg.x) || \
 (pWin->absCorner.y != pGC->lastWinOrg.y))

/* Clipping conventions
	if the drawable is a window
	    CT_REGION ==> pCompositeClip really is the composite
	    CT_other ==> pCompositeClip is the window clip region
	if the drawable is a pixmap
	    CT_REGION ==> pCompositeClip is the translated client region
		clipped to the pixmap boundary
	    CT_other ==> pCompositeClip is the pixmap bounding box
*/

void
renValidateGC(pGC, pQ, changes, pDrawable)
    register GC *pGC;
    GCInterestPtr	*pQ;
    Mask changes;
    DrawablePtr pDrawable;
{
    WindowPtr   pWin;
    int         mask;		/* stateChanges */
    int         index;		/* used for stepping through bitfields */
    int         xrot, yrot;	/* rotations for tile and stipple pattern */
    Bool        fRotate = FALSE;/* True if rotated pixmaps are needed */
    int         new_line, new_text, new_fillspans, new_fillrects;
    /* flags for changing the proc vector */
    cfbPrivGCPtr devPriv;

    switch (pGC->depth) {
    case PSZ:
	break;
    case 1:
	if (pDrawable->type == DRAWABLE_PIXMAP) {
	    mfbValidateGC(pGC, pQ, changes, pDrawable);
	    return;
	}
	/* WARNING - FALL THROUGH */
    default:
	ErrorF("cfbCreateGC: unsupported depth: %d\n", pGC->depth);
	return;
    }
    if (pDrawable->type == DRAWABLE_WINDOW)
	pWin = (WindowPtr) pDrawable;
    else
	pWin = (WindowPtr) NULL;

    devPriv = ((cfbPrivGCPtr) (pGC->devPriv));

    /*
     * if the client clip is different or moved OR the subwindowMode has
     * changed OR the window's clip has changed since the last validation
     * we need to recompute the composite clip 
     */

    if ((changes & (GCClipXOrigin | GCClipYOrigin | GCClipMask)) ||
	(changes & GCSubwindowMode) ||
	(pDrawable->serialNumber != (pGC->serialNumber & DRAWABLE_SERIAL_BITS))
	) {

	/*
	 * if there is a client clip (always a region, for us) AND it has
	 * moved or is different OR the window has moved we need to
	 * (re)translate it. 
	 */
	if ((pGC->clientClipType == CT_REGION) &&
	    ((changes & (GCClipXOrigin | GCClipYOrigin | GCClipMask)) ||
	     (pWin && WINMOVED(pWin, pGC))
	     )
	    ) {
	    /* retranslate client clip */
	    (*pGC->pScreen->RegionCopy) (devPriv->pAbsClientRegion,
					 pGC->clientClip);

	    if (pWin) {
		pGC->lastWinOrg.x = pWin->absCorner.x;
		pGC->lastWinOrg.y = pWin->absCorner.y;
		(*pGC->pScreen->TranslateRegion) (
					       devPriv->pAbsClientRegion,
				      pGC->lastWinOrg.x + pGC->clipOrg.x,
				     pGC->lastWinOrg.y + pGC->clipOrg.y);
	    }
	    else {
		pGC->lastWinOrg.x = 0;
		pGC->lastWinOrg.y = 0;
		(*pGC->pScreen->TranslateRegion) (
						  devPriv->pAbsClientRegion, pGC->clipOrg.x, pGC->clipOrg.y);
	    }
	}

	if (pWin) {
	    RegionPtr   pregWin;
	    int         freeTmpClip, freeCompClip;

	    if (pGC->subWindowMode == IncludeInferiors) {
		pregWin = NotClippedByChildren(pWin);
		freeTmpClip = FREE_CC;
	    }
	    else {
		pregWin = pWin->clipList;
		freeTmpClip = REPLACE_CC;
	    }
	    freeCompClip = devPriv->freeCompClip;

	    /*
	     * if there is no client clip, we can get by with just keeping
	     * the pointer we got, and remembering whether or not should
	     * destroy (or maybe re-use) it later.  this way, we avoid
	     * unnecessary copying of regions.  (this wins especially if
	     * many clients clip by children and have no client clip.) 
	     */
	    if (pGC->clientClipType == CT_NONE) {
		if (freeCompClip == FREE_CC) {
		    (*pGC->pScreen->RegionDestroy) (devPriv->pCompositeClip);
		}
		devPriv->pCompositeClip = pregWin;
		devPriv->freeCompClip = freeTmpClip;
	    }
	    else {
		/*
		 * we need one 'real' region to put into the composite
		 * clip. if pregWin the current composite clip are real,
		 * we can get rid of one. if pregWin is real and the
		 * current composite clip isn't, use pregWin for the
		 * composite clip. if the current composite clip is real
		 * and pregWin isn't, use the current composite clip. if
		 * neither is real, create a new region. 
		 */

		if ((freeTmpClip == FREE_CC) && (freeCompClip == FREE_CC)) {
		    (*pGC->pScreen->Intersect) (
						devPriv->pCompositeClip,
						pregWin,
					      devPriv->pAbsClientRegion);
		    (*pGC->pScreen->RegionDestroy) (pregWin);
		}
		else if ((freeTmpClip == REPLACE_CC) &&
			 (freeCompClip == FREE_CC)) {
		    devPriv->pCompositeClip = pregWin;
		    (*pGC->pScreen->Intersect) (
						devPriv->pCompositeClip,
						devPriv->pCompositeClip,
					      devPriv->pAbsClientRegion);
		}
		else if ((freeTmpClip == FREE_CC) &&
			 (freeCompClip == REPLACE_CC)) {
		    (*pGC->pScreen->Intersect) (
						devPriv->pCompositeClip,
						pregWin,
					      devPriv->pAbsClientRegion);
		}
		else if ((freeTmpClip == REPLACE_CC) &&
			 (freeCompClip == REPLACE_CC)) {
		    devPriv->pCompositeClip =
			(*pGC->pScreen->RegionCreate) (NULL, 1);
		    (*pGC->pScreen->Intersect) (
						devPriv->pCompositeClip,
						pregWin,
					      devPriv->pAbsClientRegion);
		}
	    }
	}			/* end of composite clip for a window */
	else {
	    BoxRec      pixbounds;

	    pixbounds.x1 = 0;
	    pixbounds.y1 = 0;
	    pixbounds.x2 = ((PixmapPtr) pDrawable)->width;
	    pixbounds.y2 = ((PixmapPtr) pDrawable)->height;

	    if (devPriv->freeCompClip == FREE_CC)
		(*pGC->pScreen->RegionReset) (
				    devPriv->pCompositeClip, &pixbounds);
	    else {
		devPriv->freeCompClip = FREE_CC;
		devPriv->pCompositeClip =
		    (*pGC->pScreen->RegionCreate) (&pixbounds, 1);
	    }

	    if (pGC->clientClipType == CT_REGION)
		(*pGC->pScreen->Intersect) (
					    devPriv->pCompositeClip,
					    devPriv->pCompositeClip,
					    devPriv->pAbsClientRegion);
	}			/* end of composute clip for pixmap */
    }

/*
    if (pWin) {

	*
	 * rotate tile patterns so that pattern can be combined in word by
	 * word, but the pattern seems to begin aligned with the window 
	 *
	xrot = pWin->absCorner.x;
	yrot = pWin->absCorner.y;
    }
    else {
*/
	yrot = 0;
	xrot = 0;
/*
    }
*/


    new_line = FALSE;
    new_text = FALSE;
    new_fillspans = FALSE;
    new_fillrects = FALSE;

    mask = changes;
    while (mask) {
	index = ffs(mask) - 1;
	mask &= ~(index = (1 << index));

	/*
	 * this switch acculmulates a list of which procedures might have
	 * to change due to changes in the GC.  in some cases (e.g.
	 * changing one 16 bit tile for another) we might not really need
	 * a change, but the code is being paranoid. this sort of batching
	 * wins if, for example, the alu and the font have been changed,
	 * or any other pair of items that both change the same thing. 
	 */
	switch (index) {
	case GCFunction:
	case GCForeground:
	    new_text = TRUE;
	    break;
	case GCPlaneMask:
	    break;
	case GCBackground:
	    new_fillspans = TRUE;
	    break;
	case GCLineStyle:
	    break;
	case GCLineWidth:
	case GCCapStyle:
	case GCJoinStyle:
	    new_line = TRUE;
	    break;
	case GCFillStyle:
	    new_text = TRUE;
	    new_fillspans = TRUE;
	    new_line = TRUE;
	    break;
	case GCFillRule:
	    break;
	case GCTile:
	    if (pGC->tile == (PixmapPtr) NULL)
		break;
	    cfbPadPixmap(pGC->tile);
	    fRotate = TRUE;
	    new_fillspans = TRUE;
	    new_fillrects = TRUE;
	    break;

	case GCStipple:
	    if (pGC->stipple == (PixmapPtr) NULL)
		break;
	    cfbPadPixmap(pGC->stipple);
	    fRotate = TRUE;
	    new_fillspans = TRUE;
	    new_fillrects = TRUE;
	    break;

	case GCTileStipXOrigin:
	    fRotate = TRUE;
	    break;

	case GCTileStipYOrigin:
	    fRotate = TRUE;
	    break;

	case GCFont:
	    new_text = TRUE;
	    break;
	case GCSubwindowMode:
	    break;
	case GCGraphicsExposures:
	    break;
	case GCClipXOrigin:
	    break;
	case GCClipYOrigin:
	    break;
	case GCClipMask:
	    break;
	case GCDashOffset:
	    break;
	case GCDashList:
	    break;
	case GCArcMode:
	    break;
	default:
	    break;
	}
    }

    /*
     * If the drawable has changed,  check its depth & ensure suitable
     * entries are in the proc vector. 
     */
    if (pDrawable->serialNumber != (pGC->serialNumber & (DRAWABLE_SERIAL_BITS))) {
	new_fillspans = TRUE;	/* deal with FillSpans later */
	new_fillrects = TRUE;
	pGC->SetSpans = cfbSetSpans;
    }

    /* deal with the changes we've collected */

    if (new_line)
    {
	if (pGC->lineStyle == LineSolid)
	{
	    if(pGC->lineWidth == 0)
		pGC->Polylines = miZeroLine;
	    else
		pGC->Polylines = miWideLine;
	}
	else
	    pGC->Polylines = miWideDash;

	switch(pGC->joinStyle)
	{
	  case JoinMiter:
	    pGC->LineHelper = miMiter;
	    break;
	  case JoinRound:
	  case JoinBevel:
	    pGC->LineHelper = miNotMiter;
	    break;
	}
    }

    if (new_text && pGC->font) {
	pGC->PolyGlyphBlt = miPolyGlyphBlt;
	pGC->ImageGlyphBlt = miImageGlyphBlt;
	pGC->PolyGlyphBlt = miPolyGlyphBlt;
	pGC->ImageGlyphBlt = miImageGlyphBlt;
    }

    if (new_fillspans) {
	switch (pGC->fillStyle) {
	case FillSolid:
	    pGC->FillSpans = cfbSolidFS;
	    break;
	case FillTiled:
	    pGC->FillSpans = cfbUnnaturalTileFS;
	    if (!pGC->tile)
		FatalError("cfbValidateGC: tile mode & no tile\n");
	    if (((DrawablePtr)pGC->tile)->depth != pGC->depth)
		FatalError("cfbValidateGC: tile wrong depth\n");
	    break;
	case FillStippled:
	    pGC->FillSpans = cfbUnnaturalStippleFS;
	    if (!pGC->stipple)
		FatalError("cfbValidateGC: stipple mode & no stipple\n");
	    if (((DrawablePtr)pGC->stipple)->depth != 1)
		FatalError("cfbValidateGC: stipple wrong depth\n");
	    break;
	case FillOpaqueStippled:
	    if (pGC->fgPixel == pGC->bgPixel)
		pGC->FillSpans = cfbSolidFS;
	    else {
		pGC->FillSpans = cfbUnnaturalStippleFS;
		if (!pGC->stipple)
		    FatalError("cfbValidateGC: stipple mode & no stipple\n");
		if (((DrawablePtr)pGC->stipple)->depth != 1)
		    FatalError("cfbValidateGC: stipple wrong depth\n");
	    }
	    break;
	default:
	    FatalError("cfbValidateGC: illegal fillStyle\n");
	}
    } /* end of new_fillspans */

    if (xrot || yrot || fRotate) {
	/*
	 * First destroy any previously-rotated tile/stipple
	 */
	if (devPriv->pRotatedTile) {
	    cfbDestroyPixmap(devPriv->pRotatedTile);
	    devPriv->pRotatedTile = (PixmapPtr)NULL;
	}
	if (devPriv->pRotatedStipple) {
	    cfbDestroyPixmap(devPriv->pRotatedStipple);
	    devPriv->pRotatedStipple = (PixmapPtr)NULL;
	}
	if (pGC->tile &&
	    (devPriv->pRotatedTile = cfbCopyPixmap(pGC->tile))
		== (PixmapPtr) NULL)
	    FatalError("cfbValidateGC: cannot rotate tile\n");
	if (pGC->stipple && 
	    (devPriv->pRotatedStipple = cfbCopyPixmap(pGC->stipple))
		== (PixmapPtr) NULL)
	    FatalError("cfbValidateGC: cannot rotate stipple\n");
	/*
	 * If we've gotten here, we're probably going to rotate the tile
	 * and/or stipple, so we have to add the pattern origin into
	 * the rotation factor, even if it hasn't changed.
	 */
	xrot += pGC->patOrg.x;
	yrot += pGC->patOrg.y;
	if (xrot) {
	    if (pGC->tile && devPriv->pRotatedTile)
		cfbXRotatePixmap(devPriv->pRotatedTile, xrot);
	    if (pGC->stipple && devPriv->pRotatedStipple)
		cfbXRotatePixmap(devPriv->pRotatedStipple, xrot);
	}
	if (yrot) {
	    if (pGC->tile && devPriv->pRotatedTile)
		cfbYRotatePixmap(devPriv->pRotatedTile, yrot);
	    if (pGC->stipple && devPriv->pRotatedStipple)
		cfbYRotatePixmap(devPriv->pRotatedStipple, yrot);
	}
    }

    if (new_fillrects) {
	PixmapPtr pTile;  
	pGC->PolyFillRect = hpfbPolyFillRect; /* fast until proven otherwise */
	if (((pDrawable->type == DRAWABLE_PIXMAP) &&
	     (((PixmapPtr)(pDrawable))->devKind != PIXMAP_FRAME_BUFFER)) &&
	    (pDrawable->type != DRAWABLE_WINDOW)) {
	  pGC->PolyFillRect = miPolyFillRect;
	  return;
	}

	if (pGC->fillStyle == FillTiled)
		pTile = ((cfbPrivGC *)(pGC->devPriv))->pRotatedTile;
	else
		pTile = ((cfbPrivGC *)(pGC->devPriv))->pRotatedStipple;
	if (pTile->devKind != PIXMAP_FRAME_BUFFER) {
	    pGC->PolyFillRect = miPolyFillRect;
	    return;
	}

	if (pDrawable->pScreen != pTile->drawable.pScreen)
	    pGC->PolyFillRect = miPolyFillRect;
    } /* end of new_fillrects */
}


