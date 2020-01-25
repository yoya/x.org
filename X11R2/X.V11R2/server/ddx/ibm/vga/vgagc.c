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
		Copyright IBM Corporation 1988

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
/* $Header: vgagc.c,v 1.2 88/02/24 09:27:44 rws Exp $ */
/* $ACIS:vgagc.c 1.4$ */

#ifndef lint
static char *rcsid = "$Header: vgagc.c,v 1.2 88/02/24 09:27:44 rws Exp $";
#endif

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

#include "../../mfb/mfb.h"
#include "ppc.h"
#include "mistruct.h"

#include "rtutils.h"
#include "ppcprocs.h"
#include "vgaprocs.h"

extern void miPolyPoint() ;
extern void ppcPolyFillRect() ;
extern int ReduceRop() ;

static PixmapPtr BogusPixmap = (PixmapPtr) 1 ;

Bool
vgaCreateGC( pGC )
    register GCPtr const pGC ;
{
    register ppcPrivGC *pPriv ;
    register GCInterestPtr pQ ;

    /*TRACE(("vgaCreateGC(pGC= 0x%x)\n", pGC)) ;*/
    pGC->clientClip = NULL ;
    pGC->clientClipType = CT_NONE ;

    /* some of the output primitives aren't really necessary, since
       they will be filled in ValidateGC because of dix/CreateGC()
       setting all the change bits.  Others are necessary because although
       they depend on being a monochrome frame buffer, they don't change
    */

    pGC->FillSpans = ppcSolidFS ;
    pGC->SetSpans = ppcSetSpans ;
    pGC->PutImage = miPutImage ;
    pGC->CopyArea = ppcCopyArea ;
    pGC->CopyPlane = miCopyPlane ;
    pGC->PolyPoint = miPolyPoint ;

    pGC->Polylines = ppcScrnZeroLine ;
    pGC->PolySegment = miPolySegment ;
    pGC->PolyRectangle = miPolyRectangle ;
    pGC->PolyArc = miPolyArc ;
    pGC->FillPolygon = miFillPolygon ;
    pGC->PolyFillRect = miPolyFillRect ;
    pGC->PolyFillArc = miPolyFillArc ;
    pGC->PolyText8 = miPolyText8 ;
    pGC->ImageText8 = miImageText8 ;
    pGC->PolyText16 = miPolyText16 ;
    pGC->ImageText16 = miImageText16 ;
    pGC->ImageGlyphBlt = ppcImageGlyphBlt ;
    pGC->PolyGlyphBlt = miPolyGlyphBlt ;
    pGC->PushPixels = miPushPixels ;
    pGC->LineHelper = miMiter ;
    pGC->ChangeClip = mfbChangeClip ;
    pGC->DestroyClip = mfbDestroyClip ;

    /* mfb wants to translate before scan convesion */
    pGC->miTranslate = 1 ;

    if ( !( pPriv = (ppcPrivGC *) Xalloc( sizeof( ppcPrivGC ) ) ) )
	return FALSE ;

    pGC->devPriv = (pointer) pPriv ;
    pPriv->rop = ReduceRop( pGC->alu, pGC->fgPixel ) ;
    pPriv->fExpose = TRUE ;
    pPriv->pAbsClientRegion =(* pGC->pScreen->RegionCreate)(NULL, 1) ;
    pPriv->pCompositeClip = (*pGC->pScreen->RegionCreate)( NULL, 1 ) ;
    pPriv->freeCompClip = REPLACE_CC ;
    pPriv->lastDrawableType = -1 ;
    pPriv->lastDrawableDepth = -1 ;
    pPriv->pRotatedTile = NullPixmap ;
    pPriv->pRotatedStipple = NullPixmap ;
    pPriv->ppPixmap = &BogusPixmap ;
    pPriv->FillArea = mfbSolidInvertArea ;

    if ( !( pQ = (GCInterestPtr) Xalloc( sizeof( GCInterestRec ) ) ) ) {
	Xfree( pPriv ) ;
	return FALSE ;
    }

    /* Now link this device into the GCque */
    pGC->pNextGCInterest = pQ ;
    pGC->pLastGCInterest = pQ ;
    pQ->pNextGCInterest = (GCInterestPtr) &pGC->pNextGCInterest ;
    pQ->pLastGCInterest = (GCInterestPtr) &pGC->pNextGCInterest ;
    pQ->length = sizeof( GCInterestRec ) ;
    pQ->owner = 0 ;		/* server owns this */
    pQ->ValInterestMask = ~0 ;	/* interested in everything at validate time */
    pQ->ValidateGC = vgaValidateGC ;
    pQ->ChangeInterestMask = 0 ; /* interested in nothing at change time */
    pQ->ChangeGC = (int (*) () ) NULL ;
    pQ->CopyGCSource = (void (*) () ) NULL ;
    pQ->CopyGCDest = mfbCopyGCDest ;
    pQ->DestroyGC = ppcDestroyGC ;
    return TRUE ;
}

#define WINMOVED( pWin, pGC ) \
 ( ( pWin->absCorner.x != pGC->lastWinOrg.x ) \
|| ( pWin->absCorner.y != pGC->lastWinOrg.y ) )

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
vgaValidateGC( pGC, pQ, changes, pDrawable )
    register GCPtr const pGC ;
    GCInterestPtr const pQ ;
    Mask changes ;
    DrawablePtr const pDrawable ;
{
    register ppcPrivGCPtr devPriv ;
    register int mask ;		/* stateChanges */
    register int index ;	/* used for stepping through bitfields */
    int new_line, new_fill ;
    register WindowPtr pWin ;

    devPriv = (ppcPrivGCPtr) pGC->devPriv ;

    if ( pDrawable->type != devPriv->lastDrawableType ) {
        devPriv->lastDrawableType = pDrawable->type ;
	changes = ~0 ;
	pGC->SetSpans = ppcSetSpans ;
	if ( pDrawable->type == DRAWABLE_PIXMAP ) {
	    pGC->CopyArea = miCopyArea ;
    	    pGC->PolyFillRect = miPolyFillRect ;
	}
	else {
	    pGC->CopyArea = ppcCopyArea ;
    	    pGC->PolyFillRect = ppcPolyFillRect ;
	}
	pGC->PutImage = miPutImage ;
	pGC->CopyPlane = miCopyPlane ;
	pGC->PolyPoint = miPolyPoint ;
	pGC->PolySegment = miPolySegment ;
	pGC->PushPixels = miPushPixels ;
	pGC->FillPolygon = miFillPolygon ;
	pGC->ImageText8 = miImageText8 ;
	pGC->ImageText16 = miImageText16 ;
	pGC->PolyText8 = miPolyText8 ;
	pGC->PolyText16 = miPolyText16 ;
    }

    if ( pDrawable->type == DRAWABLE_PIXMAP ) {
	ppcValidateGC( pGC, pQ, changes, pDrawable ) ;
	return ;
    }

    if ( pDrawable->type == DRAWABLE_WINDOW )
	pWin = (WindowPtr) pDrawable ;
    else
	pWin = (WindowPtr) NULL ;

    /*
	if the client clip is different or moved OR
	the subwindowMode has changed OR
	the window's clip has changed since the last validation
	we need to recompute the composite clip
    */

    if ( ( changes & (GCClipXOrigin|GCClipYOrigin|GCClipMask|GCSubwindowMode) )
      || ( pDrawable->serialNumber != ( pGC->serialNumber & DRAWABLE_SERIAL_BITS ) ) ) {
        /* if there is a client clip (always a region, for us) AND
	        it has moved or is different OR the window has moved
           we need to (re)translate it.
        */
	if ( ( pGC->clientClipType == CT_REGION )
	  && ( ( changes & ( GCClipXOrigin | GCClipYOrigin | GCClipMask ) )
	    || ( pWin && WINMOVED( pWin, pGC ) ) ) ) {
	    /* retranslate client clip */
	    (* pGC->pScreen->RegionCopy )( devPriv->pAbsClientRegion,
			                  pGC->clientClip ) ;

	    if ( pWin ) {
		pGC->lastWinOrg.x = pWin->absCorner.x ;
		pGC->lastWinOrg.y = pWin->absCorner.y ;
		(* pGC->pScreen->TranslateRegion)(
	                       devPriv->pAbsClientRegion,
			       pGC->lastWinOrg.x + pGC->clipOrg.x,
			       pGC->lastWinOrg.y + pGC->clipOrg.y ) ;
	    }
	    else {
		pGC->lastWinOrg.x = 0 ;
		pGC->lastWinOrg.y = 0 ;
		(* pGC->pScreen->TranslateRegion)(
	           devPriv->pAbsClientRegion, pGC->clipOrg.x, pGC->clipOrg.y ) ;
	    }
	}

	    int freeTmpClip, freeCompClip ;
	    RegionPtr pregWin ;		/* clip for this window, without
					   client clip */

	    if ( pGC->subWindowMode == IncludeInferiors ) {
	        pregWin = NotClippedByChildren( pWin ) ;
		freeTmpClip = FREE_CC ;
	    }
	    else {
	        pregWin = pWin->clipList ;
		freeTmpClip = REPLACE_CC ;
	    }
	    freeCompClip = devPriv->freeCompClip ;

	    /* if there is no client clip, we can get by with
	       just keeping the pointer we got, and remembering
	       whether or not should destroy (or maybe re-use)
	       it later.  this way, we avoid unnecessary copying
	       of regions.  (this wins especially if many clients clip
	       by children and have no client clip.)
	    */
	    if ( pGC->clientClipType == CT_NONE ) {
	        if ( freeCompClip == FREE_CC )
		    (* pGC->pScreen->RegionDestroy)( devPriv->pCompositeClip ) ;
	        devPriv->pCompositeClip = pregWin ;
	        devPriv->freeCompClip = freeTmpClip ;
	    }
	    else {
		/* we need one 'real' region to put into the composite
		   clip.
			if pregWin and the current composite clip
		   are real, we can get rid of one.
			if the current composite clip is real and
		   pregWin isn't, intersect the client clip and
		   pregWin into the existing composite clip.
			if pregWin is real and the current composite
		   clip isn't, intersect pregWin with the client clip
		   and replace the composite clip with it.
			if neither is real, create a new region and
		   do the intersection into it.
		*/

		if ( ( freeTmpClip == FREE_CC ) && ( freeCompClip == FREE_CC ) )
		{
		    (* pGC->pScreen->Intersect)(
		        devPriv->pCompositeClip,
			pregWin,
			devPriv->pAbsClientRegion ) ;
		    (* pGC->pScreen->RegionDestroy)( pregWin ) ;
		}
		else if ( ( freeTmpClip == REPLACE_CC )
		       && ( freeCompClip == FREE_CC ) ) {
		    (* pGC->pScreen->Intersect)(
			devPriv->pCompositeClip,
		        pregWin,
			devPriv->pAbsClientRegion ) ;
		}
		else if ( ( freeTmpClip == FREE_CC )
		       && ( freeCompClip == REPLACE_CC ) ) {
		    (* pGC->pScreen->Intersect)(
		       pregWin,
		       pregWin,
		       devPriv->pAbsClientRegion ) ;
		    devPriv->pCompositeClip = pregWin ;
		}
		else if ( ( freeTmpClip == REPLACE_CC )
		       && ( freeCompClip == REPLACE_CC ) ) {
		    devPriv->pCompositeClip =
			(* pGC->pScreen->RegionCreate)( NULL, 1 ) ;
		    (* pGC->pScreen->Intersect)(
			devPriv->pCompositeClip,
		        pregWin,
			devPriv->pAbsClientRegion ) ;
		}
		devPriv->freeCompClip = FREE_CC ;
	    }
	 /* end of composite clip for a window */
    }

    new_line = FALSE ;
    new_fill = FALSE ;

    for ( mask = changes, index = 1 ;
          mask ;
	  index <<= 1 )
	if ( mask & index ) {
		mask &= ~ index ;

	/* this switch acculmulates a list of which procedures
	   might have to change due to changes in the GC.  in
	   some cases (e.g. changing one 16 bit tile for another)
	   we might not really need a change, but the code is
	   being paranoid.
	   this sort of batching wins if, for example, the alu
	   and the font have been changed, or any other pair
	   of items that both change the same thing.
	*/
	switch ( index ) {
	  case GCLineStyle:
	  case GCLineWidth:
	  case GCCapStyle:
	  case GCJoinStyle:
	    new_line = TRUE ;
	    break ;
	  case GCTile:
	    if ( pGC->tile == (PixmapPtr) NULL )
		break ;
	    new_fill = TRUE ;
	    break ;
	  case GCStipple:
	    if ( pGC->stipple == (PixmapPtr) NULL )
		break ;
	  case GCFillStyle:
	    new_fill = TRUE ;
	    break ;
	  case GCFunction:
	  case GCForeground:
	  case GCPlaneMask:
	  case GCBackground:
	  case GCFillRule:
	  case GCTileStipXOrigin:
	  case GCTileStipYOrigin:
	  case GCFont:
	  case GCSubwindowMode:
	  case GCGraphicsExposures:
	  case GCClipXOrigin:
	  case GCClipYOrigin:
	  case GCClipMask:
	  case GCDashOffset:
	  case GCDashList:
	  case GCArcMode:
	  default:
	    break ;
	}
    }

    /* deal with the changes we've collected .
    */

    if ( new_line || new_fill ) {
	if ( pGC->lineWidth <= 1 )
		pGC->Polylines = ppcScrnZeroLine ;
	else if ( pGC->lineStyle == LineSolid )
		pGC->Polylines = miWideLine ;
	else
	        pGC->Polylines = miWideDash ;

	switch ( pGC->joinStyle )
	{
	  case JoinMiter:
	    pGC->LineHelper = miMiter ;
	    break ;
	  case JoinRound:
	  case JoinBevel:
	    pGC->LineHelper = miNotMiter ;
	    break ;
	}
    }

    if ( new_fill ) {
	/* install a suitable fillspans */
	if ( pGC->fillStyle == FillSolid )
	    pGC->FillSpans = ppcSolidFS ;
	else if ( pGC->fillStyle == FillStippled )
	    pGC->FillSpans = ppcStippleFS ;
	else if ( pGC->fillStyle == FillOpaqueStippled )
	    pGC->FillSpans = ppcOpStippleFS ;
	else /* overload tiles to do parti-colored opaque stipples */
	    pGC->FillSpans = ppcTileFS ;

    } /* end of new_fill */

    return  ;
}
