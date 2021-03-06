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

/* $XConsortium: gc.c,v 1.111 88/09/06 15:41:05 jim Exp $ */

#include "X.h"
#include "Xmd.h"
#include "Xproto.h"
#include "misc.h"
#include "resource.h"
#include "gcstruct.h"
#include "pixmapstr.h"
#include "dixfontstr.h"
#include "scrnintstr.h"
#include "region.h"

#include "dix.h"

extern XID clientErrorValue;

#ifdef DEBUG
extern void NotImplemented();
#endif

/* written by drewry august 1986 */

void
ValidateGC(pDraw, pGC)
    DrawablePtr	pDraw;
    GC		*pGC;
{
    GCInterestPtr	pQ, pQInit;

    pQ = pGC->pNextGCInterest;
    pQInit = (GCInterestPtr) &pGC->pNextGCInterest;
    if (pGC->serialNumber != pDraw->serialNumber)
        pGC->stateChanges |= GC_CALL_VALIDATE_BIT;
    do
    {
	if (pQ->ValInterestMask & pGC->stateChanges)
	    (* pQ->ValidateGC) (pGC, pQ, pGC->stateChanges, pDraw);
	pQ = pQ->pNextGCInterest;
    }
    while(pQ != pQInit);
    pGC->stateChanges = 0;
    pGC->serialNumber = pDraw->serialNumber;
}



/* Publically defined entry to ChangeGC.  Just calls DoChangeGC and tells
 * it that all of the entries are constants or IDs */
ChangeGC(pGC, mask, pval)
    register GC 	*pGC;
    register BITS32	mask;
    XID			*pval;
{
    return (DoChangeGC(pGC, mask, pval, 0));
}
/* DoChangeGC(pGC, mask, pval, fPointer)
   mask is a set of bits indicating which values to change.
   pval contains an appropriate value for each mask.
   fPointer is true if the values for tiles, stipples, fonts or clipmasks
   are pointers instead of IDs.  
   if there is an error, the value is marked as changed 
   anyway, which is probably wrong, but infrequent.

NOTE:
	all values sent over the protocol for ChangeGC requests are
32 bits long
*/

int
DoChangeGC(pGC, mask, pval, fPointer)
    register GC 	*pGC;
    register BITS32	mask;
    XID			*pval;
    int			fPointer;
{
    register BITS32 	index;
    register int 	error = 0;
    PixmapPtr 		pPixmap;
    BITS32		maskQ;
    GCInterestPtr	pQ, pQInit;

    pGC->serialNumber |= GC_CHANGE_SERIAL_BIT;

    maskQ = mask;	/* save these for when we walk the GCque */
    while (mask && !error) 
    {
	index = (BITS32) lowbit (mask);
	mask &= ~index;
	pGC->stateChanges |= index;
	switch (index)
	{
	    case GCFunction:
		if (((CARD8)*pval >= GXclear) && ((CARD8)*pval <= GXset))
		    pGC->alu = (CARD8)*pval;
		else
		{
		    clientErrorValue = (CARD8)*pval;
		    error = BadValue;
		}
		pval++;
		break;
	    case GCPlaneMask:
		pGC->planemask = *pval++;
		break;
	    case GCForeground:
		pGC->fgPixel = *pval++;
		break;
	    case GCBackground:
		pGC->bgPixel = *pval++;
		break;
	    case GCLineWidth:		/* ??? line width is a CARD16 */
		pGC->lineWidth = (CARD16)*pval;
                pval++;
		break;
	    case GCLineStyle:
		if (((CARD8)*pval >= LineSolid) 
		    && ((CARD8)*pval <= LineDoubleDash))
		    pGC->lineStyle = (CARD8)*pval;
		else
		{
		    clientErrorValue = (CARD8)*pval;
		    error = BadValue;
		}
		pval++;
		break;
	    case GCCapStyle:
		if (((CARD8)*pval >= CapNotLast) 
		    && ((CARD8)*pval <= CapProjecting))
		    pGC->capStyle = (CARD8)*pval;
		else
		{
		    clientErrorValue = (CARD8)*pval;
		    error = BadValue;
		}
		pval++;
		break;
	    case GCJoinStyle:
		if (((CARD8)*pval >= JoinMiter) && ((CARD8)*pval <= JoinBevel))
		    pGC->joinStyle = (CARD8)*pval;
		else
		{
		    clientErrorValue = (CARD8)*pval;
		    error = BadValue;
		}
		pval++;
		break;
	    case GCFillStyle:
		if (((CARD8)*pval >= FillSolid) 
		    && ((CARD8)*pval <= FillOpaqueStippled))
		    pGC->fillStyle = (CARD8)*pval;
		else
		{
		    clientErrorValue = (CARD8)*pval;
		    error = BadValue;
		}
		pval++;
		break;
	    case GCFillRule:
		if (((CARD8)*pval >= EvenOddRule) && 
		    ((CARD8)*pval <= WindingRule))
		    pGC->fillRule = (CARD8)*pval;
		else
		{
		    clientErrorValue = (CARD8)*pval;
		    error = BadValue;
		}
		pval++;
		break;
	    case GCTile:
		if(fPointer)
		    pPixmap = (PixmapPtr) *pval;
		else
		    pPixmap = (PixmapPtr)LookupID((CARD32)*pval, 
					      RT_PIXMAP, RC_CORE);
		if (pPixmap)
		{
		    if ((pPixmap->drawable.depth != pGC->depth) ||
			(pPixmap->drawable.pScreen != pGC->pScreen))
		    {
			error = BadMatch;
		    }
		    else
		    {
			pPixmap->refcnt++;
			(* pGC->pScreen->DestroyPixmap)(pGC->tile);
			pGC->tile = pPixmap;
		    }
		}
		else
		{
		    clientErrorValue = (CARD32)*pval;
		    error = BadPixmap;
		}
		pval++;
		break;
	    case GCStipple:
		if(fPointer)
		    pPixmap = (PixmapPtr) *pval;
		else
		    pPixmap = (PixmapPtr)LookupID((CARD32)*pval, 
					      RT_PIXMAP, RC_CORE);
		if (pPixmap)
		{
		    if ((pPixmap->drawable.depth != 1) ||
			(pPixmap->drawable.pScreen != pGC->pScreen))
		    {
			error = BadMatch;
		    }
		    else
		    {
			pPixmap->refcnt++;
			(* pGC->pScreen->DestroyPixmap)(pGC->stipple);
			pGC->stipple = pPixmap;
		    }
		}
		else
		{
		    clientErrorValue = (CARD32)*pval;
		    error = BadPixmap;
		}
		pval++;
		break;
	    case GCTileStipXOrigin:
		pGC->patOrg.x = (INT16)*pval;
                pval++;
		break;
	    case GCTileStipYOrigin:
		pGC->patOrg.y = (INT16)*pval;
		pval++;
		break;
	    case GCFont:
              {
		FontPtr	pFont;


		if(fPointer)
		    pFont = (FontPtr) *pval;
		else
		    pFont = (FontPtr)LookupID((CARD32)*pval, RT_FONT, RC_CORE);

		if (pFont)
		{
		    pFont->refcnt++;
		    if (pGC->font)
    		        CloseFont( pGC->font);
		    pGC->font = pFont;
		 }
		else
		{
		    clientErrorValue = *pval;
		    error = BadFont;
		}
		pval++;
		break;
	      }
	    case GCSubwindowMode:
		if (((CARD8)*pval == ClipByChildren) ||
		    ((CARD8)*pval == IncludeInferiors))
		    pGC->subWindowMode = (CARD8)*pval;
		else
		{
		    clientErrorValue = (CARD8)*pval;
		    error = BadValue;
		}
		pval++;
		break;
	    case GCGraphicsExposures:
		if ((Bool)*pval == xFalse)
		    pGC->graphicsExposures = FALSE;
		else if ((Bool)*pval == xTrue)
		    pGC->graphicsExposures = TRUE;
		else
		{
		    clientErrorValue = (Bool)*pval;
		    error = BadValue;
		}
		pval++;
		break;
	    case GCClipXOrigin:
		pGC->clipOrg.x = (INT16)(*pval);
		pval++;
		break;
	    case GCClipYOrigin:
		pGC->clipOrg.y = (INT16)(*pval);
		pval++;
		break;
	    case GCClipMask:
	      {
		Pixmap pid;
		int	clipType;

		pid = (Pixmap) *pval;
		if (pid == None)
		{
		    clipType = CT_NONE;
		}
		else
		{
		    if(fPointer)
			pPixmap = (PixmapPtr) *pval;
		    else
		        pPixmap = (PixmapPtr)LookupID(pid, RT_PIXMAP, RC_CORE);
		    if (pPixmap)
  		    {
			if ((pPixmap->drawable.depth != 1) ||
			    (pPixmap->drawable.pScreen != pGC->pScreen))
			{
			    error = BadMatch;
			}
			else
			{
			    clipType = CT_PIXMAP;
			    pPixmap->refcnt++;
			}
		    }
		    else
		    {
			clientErrorValue = pid;
			error = BadPixmap;
		    }
		}
		pval++;
		if(error == Success)
		{
		    (*pGC->ChangeClip)(pGC, clipType, pPixmap, 0);
		}
		break;
	      }
	    case GCDashOffset:
		pGC->dashOffset = (CARD16)*pval;
		pval++;
		break;
	    case GCDashList:
		if ((CARD8) (*pval) != 0)
 		{
		    xfree(pGC->dash);
		    pGC->numInDashList = 2;
		    pGC->dash = (unsigned char *)xalloc(2 * sizeof(unsigned char));
		    pGC->dash[0] = (CARD8)(*pval);
		    pGC->dash[1] = (CARD8)(*pval);
		}
 		else
		{
		   clientErrorValue = (CARD8)*pval;
		   error = BadValue;
		}
		pval++;
		break;
	    case GCArcMode:
		if (((CARD8)*pval >= ArcChord) 
		    && ((CARD8)*pval <= ArcPieSlice))
		    pGC->arcMode = (CARD8)*pval;
		else
		{
		    clientErrorValue = (CARD8)*pval;
		    error = BadValue;
		}
		pval++;
		break;
	    default:
		clientErrorValue = maskQ;
		error = BadValue;
		pval++;
		break;
	}
    }
    pQ = pGC->pNextGCInterest;
    pQInit = (GCInterestPtr) &pGC->pNextGCInterest;
    do
    {
	/* I assume that if you've set a change interest mask, you've set a
	 * changeGC function */
	if(pQ->ChangeInterestMask & maskQ)
	    (*pQ->ChangeGC)(pGC, pQ, maskQ);
	pQ = pQ->pNextGCInterest;
    }
    while(pQ != pQInit);
    return error;
}

/* CreateGC(pDrawable, mask, pval, pStatus)
   creates a default GC for the given drawable, using mask to fill
   in any non-default values.
   Returns a pointer to the new GC on success, NULL otherwise.
   returns status of non-default fields in pStatus
BUG:
   should check for failure to create default tile

*/

GC *
CreateGC(pDrawable, mask, pval, pStatus)
    DrawablePtr	pDrawable;
    BITS32	mask;
    XID		*pval;
    int		*pStatus;
{
    register GC *pGC;
    extern FontPtr defaultFont;
#ifdef DEBUG
    void 	(**j)();
#endif /* DEBUG */

    pGC = (GC *)xalloc(sizeof(GC));
    if (!pGC)
	return (GC *)NULL;

#ifdef DEBUG
    for(j = &pGC->FillSpans;
        j < &pGC->PushPixels;
        j++ )
        *j = NotImplemented;
#endif /* DEBUG */

    pGC->pScreen = pDrawable->pScreen;
    pGC->depth = pDrawable->depth;
    pGC->alu = GXcopy; /* dst <- src */
    pGC->planemask = ~0;
    pGC->serialNumber = GC_CHANGE_SERIAL_BIT;

    pGC->fgPixel = 0;
    pGC->bgPixel = 1;
    pGC->lineWidth = 0;
    pGC->lineStyle = LineSolid;
    pGC->capStyle = CapButt;
    pGC->joinStyle = JoinMiter;
    pGC->fillStyle = FillSolid;
    pGC->fillRule = EvenOddRule;
    pGC->arcMode = ArcPieSlice;
    pGC->font = defaultFont;
    if ( pGC->font)  /* necessary, because open of default font could fail */
	pGC->font->refcnt++;
    pGC->tile = NullPixmap;

    /* use the default stipple */
    pGC->stipple = pGC->pScreen->PixmapPerDepth[0];
    pGC->stipple->refcnt++;
    pGC->patOrg.x = 0;
    pGC->patOrg.y = 0;
    pGC->subWindowMode = ClipByChildren;
    pGC->graphicsExposures = TRUE;
    pGC->clipOrg.x = 0;
    pGC->clipOrg.y = 0;
    pGC->clientClipType = CT_NONE;
    pGC->clientClip = (pointer)NULL;
    pGC->numInDashList = 2;
    pGC->dash = (unsigned char *)xalloc(2 * sizeof(unsigned char));
    pGC->dash[0] = 4;
    pGC->dash[1] = 4;
    pGC->dashOffset = 0;

    /* if the client hasn't provided a tile, build one and fill it with
       the foreground pixel
    */
    if(!(mask & GCTile))
    {
	XID		tmpval[3];
	PixmapPtr 	pTile;
	GCPtr		pgcScratch;
	xRectangle	rect;
	short		w, h;

	w = 16; /* XXX arbitrary */
	h = 16;
	(*pGC->pScreen->QueryBestSize)(TileShape, &w, &h);
	pTile = (PixmapPtr)
		(*pGC->pScreen->CreatePixmap)(pDrawable->pScreen,
					      w, h, pGC->depth);
	tmpval[0] = GXcopy;
	tmpval[1] = (mask & GCForeground) ? 
	    /* blech */
	    pval[(mask & GCFunction) + ((mask & GCPlaneMask) == GCPlaneMask)] :
	    pGC->fgPixel;
	tmpval[2] = FillSolid;
	pgcScratch = GetScratchGC(pGC->depth, pGC->pScreen);
	ChangeGC(pgcScratch, GCFunction | GCForeground | GCFillStyle, 
		 tmpval);
	ValidateGC((DrawablePtr)pTile, pgcScratch);
	rect.x = 0;
	rect.y = 0;
	rect.width = w;
	rect.height = h;
	(*pgcScratch->PolyFillRect)(pTile, pgcScratch, 1, &rect);
	/* Always remember to free the scratch graphics context after use. */
	FreeScratchGC(pgcScratch);

	pGC->tile = pTile;
    }
    pGC->stateChanges = (1 << GCLastBit+1) - 1;
    (*pGC->pScreen->CreateGC)(pGC);
    if(mask)
        *pStatus = ChangeGC(pGC, mask, pval);
    else
	*pStatus = Success;

    return (pGC);
}


int
CopyGC(pgcSrc, pgcDst, mask)
    register GC		*pgcSrc;
    register GC		*pgcDst;
    register BITS32	mask;
{
    register BITS32	index;
    BITS32		maskQ;
    GCInterestPtr	pQ, pQInit;
    int i;
    int 		error = 0;

    pgcDst->serialNumber |= GC_CHANGE_SERIAL_BIT;
    pgcDst->stateChanges |= mask;
    maskQ = mask;
    while (mask)
    {
	index = (BITS32) lowbit (mask);
	mask &= ~index;
	switch (index)
	{
	    case GCFunction:
		pgcDst->alu = pgcSrc->alu;
		break;
	    case GCPlaneMask:
		pgcDst->planemask = pgcSrc->planemask;
		break;
	    case GCForeground:
		pgcDst->fgPixel = pgcSrc->fgPixel;
		break;
	    case GCBackground:
		pgcDst->bgPixel = pgcSrc->bgPixel;
		break;
	    case GCLineWidth:
		pgcDst->lineWidth = pgcSrc->lineWidth;
		break;
	    case GCLineStyle:
		pgcDst->lineStyle = pgcSrc->lineStyle;
		break;
	    case GCCapStyle:
		pgcDst->capStyle = pgcSrc->capStyle;
		break;
	    case GCJoinStyle:
		pgcDst->joinStyle = pgcSrc->joinStyle;
		break;
	    case GCFillStyle:
		pgcDst->fillStyle = pgcSrc->fillStyle;
		break;
	    case GCFillRule:
		pgcDst->fillRule = pgcSrc->fillRule;
		break;
	    case GCTile:
		{
		    if (pgcDst->tile == pgcSrc->tile)
			break;
		    (* pgcDst->pScreen->DestroyPixmap)(pgcDst->tile);
		    pgcDst->tile = pgcSrc->tile;
		    if (IS_VALID_PIXMAP(pgcDst->tile))
		       pgcDst->tile->refcnt ++;
		    break;
		}
	    case GCStipple:
		{
		    if (pgcDst->stipple == pgcSrc->stipple)
			break;
		    (* pgcDst->pScreen->DestroyPixmap)(pgcDst->stipple);
		    pgcDst->stipple = pgcSrc->stipple;
		    if (IS_VALID_PIXMAP(pgcDst->stipple))
    		        pgcDst->stipple->refcnt ++;
		    break;
		}
	    case GCTileStipXOrigin:
		pgcDst->patOrg.x = pgcSrc->patOrg.x;
		break;
	    case GCTileStipYOrigin:
		pgcDst->patOrg.y = pgcSrc->patOrg.y;
		break;
	    case GCFont:
		if (pgcDst->font == pgcSrc->font)
		    break;
		if (pgcDst->font)
		    CloseFont(pgcDst->font);
		if ((pgcDst->font = pgcSrc->font) != NullFont)
		    (pgcDst->font)->refcnt++;
		break;
	    case GCSubwindowMode:
		pgcDst->subWindowMode = pgcSrc->subWindowMode;
		break;
	    case GCGraphicsExposures:
		pgcDst->graphicsExposures = pgcSrc->graphicsExposures;
		break;
	    case GCClipXOrigin:
		pgcDst->clipOrg.x = pgcSrc->clipOrg.x;
		break;
	    case GCClipYOrigin:
		pgcDst->clipOrg.y = pgcSrc->clipOrg.y;
		break;
	    case GCClipMask:
		(* pgcDst->CopyClip)(pgcDst, pgcSrc);
		break;
	    case GCDashOffset:
		pgcDst->dashOffset = pgcSrc->dashOffset;
		break;
	    case GCDashList:
		xfree(pgcDst->dash);
		pgcDst->dash = (unsigned char *)
				xalloc(pgcSrc->numInDashList * sizeof(unsigned char));
		pgcDst->numInDashList = pgcSrc->numInDashList;
		for (i=0; i<pgcSrc->numInDashList; i++)
		    pgcDst->dash[i] = pgcSrc->dash[i];
		break;
	    case GCArcMode:
		pgcDst->arcMode = pgcSrc->arcMode;
		break;
	    default:
		clientErrorValue = maskQ;
		error = BadValue;
		break;
	}
    }
    pQ = pgcSrc->pNextGCInterest;
    pQInit = (GCInterestPtr) &pgcSrc->pNextGCInterest;
    do
    {
	if(pQ->CopyGCSource)
	    (*pQ->CopyGCSource)(pgcSrc, pQ, maskQ, pgcDst);
	pQ = pQ->pNextGCInterest;
    }
    while(pQ != pQInit);
    pQ = pgcDst->pNextGCInterest;
    pQInit = (GCInterestPtr) &pgcDst->pNextGCInterest;
    do
    {
	if(pQ->CopyGCDest)
	    (*pQ->CopyGCDest)(pgcDst, pQ, maskQ, pgcSrc);
	pQ = pQ->pNextGCInterest;
    }
    while(pQ != pQInit);
    return error;
}

/*****************
 * FreeGC 
 *   does the diX part of freeing the characteristics in the GC 
 ***************/

/*ARGSUSED*/
int
FreeGC(pGC, gid)
    GC *pGC;
    GContext gid;
{
    GCInterestPtr	pQ, pQInit, pQnext;

    CloseFont(pGC->font);
    (* pGC->DestroyClip)(pGC);

    (* pGC->pScreen->DestroyPixmap)(pGC->tile);
    (* pGC->pScreen->DestroyPixmap)(pGC->stipple);

    pQ = pGC->pNextGCInterest;
    pQInit = (GCInterestPtr) &pGC->pNextGCInterest;
    do
    {
        pQnext = pQ->pNextGCInterest;
	if(pQ->DestroyGC)
	    (*pQ->DestroyGC) (pGC, pQ);
	pQ = pQnext;
    }
    while(pQ != pQInit);
    xfree(pGC->dash);
    xfree(pGC);
    return(Success);
}

void
SetGCMask(pGC, selectMask, newDataMask)
    GCPtr pGC;
    Mask selectMask;
    Mask newDataMask;
{
    pGC->stateChanges = (~selectMask & pGC->stateChanges) |
		        (selectMask & newDataMask);
    if (selectMask & newDataMask)
        pGC->serialNumber |= GC_CHANGE_SERIAL_BIT;        
}



/* CreateScratchGC(pScreen, depth)
    like CreateGC, but doesn't do the default tile or stipple,
since we can't create them without already having a GC.  any code
using the tile or stipple has to set them explicitly anyway,
since the state of the scratch gc is unknown.  This is OK
because ChangeGC() has to be able to deal with NULL tiles and
stipples anyway (in case the CreateGC() call has provided a 
value for them -- we can't set the default tile until the
client-supplied attributes are installed, since the fgPixel
is what fills the default tile.  (maybe this comment should
go with CreateGC() or ChangeGC().)
*/

GC *
CreateScratchGC(pScreen, depth)
    ScreenPtr pScreen;
    int depth;
{
    register GC *pGC;
    extern FontPtr defaultFont;
#ifdef DEBUG
    void 	(**j)();
#endif /* DEBUG */

    pGC = (GC *)xalloc(sizeof(GC));
    if (!pGC)
	return (GC *)NULL;

#ifdef DEBUG
    for(j = &pGC->FillSpans;
        j < &pGC->PushPixels;
        j++ )
        *j = NotImplemented;
#endif /* DEBUG */

    pGC->pScreen = pScreen;
    pGC->depth = depth;
    pGC->alu = GXcopy; /* dst <- src */
    pGC->planemask = ~0;
    pGC->serialNumber = 0;

    pGC->fgPixel = 0;
    pGC->bgPixel = 1;
    pGC->lineWidth = 0;
    pGC->lineStyle = LineSolid;
    pGC->capStyle = CapButt;
    pGC->joinStyle = JoinMiter;
    pGC->fillStyle = FillSolid;
    pGC->fillRule = EvenOddRule;
    pGC->arcMode = ArcPieSlice;
    pGC->font = defaultFont;
    if ( pGC->font)  /* necessary, because open of default font could fail */
	pGC->font->refcnt++;
    pGC->tile = NullPixmap;
    pGC->stipple = NullPixmap;
    pGC->patOrg.x = 0;
    pGC->patOrg.y = 0;
    pGC->subWindowMode = ClipByChildren;
    pGC->graphicsExposures = TRUE;
    pGC->clipOrg.x = 0;
    pGC->clipOrg.y = 0;
    pGC->clientClipType = CT_NONE;
    pGC->numInDashList = 2;
    pGC->dash = (unsigned char *)xalloc(2 * sizeof(unsigned char));
    pGC->dash[0] = 4;
    pGC->dash[1] = 4;
    pGC->dashOffset = 0;

    pGC->stateChanges = (1 << GCLastBit+1) - 1;
    (*pScreen->CreateGC)(pGC);
    return pGC;
}


FreeGCperDepth(screenNum)
    int screenNum;
{
    register int i;
    register ScreenPtr pScreen;
    GCPtr *ppGC;

    pScreen = &screenInfo.screen[screenNum];
    ppGC = (GCPtr *) pScreen->GCperDepth;

    /* do depth 1 seperately because it's not included in list */
    (void)FreeGC(ppGC[0], (GContext)0);

    for (i = 0; i < pScreen-> numDepths; i++)
    {
	(void)FreeGC(ppGC[i+1], (GContext)0);
    }
}


CreateGCperDepthArray(screenNum)
    int screenNum;
{
    register int i;
    register ScreenPtr pScreen;
    DepthPtr pDepth;

    pScreen = &screenInfo.screen[screenNum];
    pScreen->rgf = 0;
    /* do depth 1 seperately because it's not included in list */
    pScreen->GCperDepth[0] = CreateScratchGC(pScreen, 1);
    (pScreen->GCperDepth[0])->graphicsExposures = FALSE;

    pDepth = pScreen->allowedDepths;
    for (i=0; i<pScreen->numDepths; i++, pDepth++)
    {
	pScreen->GCperDepth[i+1] = CreateScratchGC(pScreen,
						   pDepth->depth);
	(pScreen->GCperDepth[i+1])->graphicsExposures = FALSE;
    }
}

CreateDefaultStipple(screenNum)
    int screenNum;
{
    register ScreenPtr pScreen;
    XID tmpval[3];
    xRectangle rect;
    short w, h;
    GCPtr pgcScratch;

    pScreen = &screenInfo.screen[screenNum];

    w = 16;
    h = 16;
    (* pScreen->QueryBestSize)(StippleShape, &w, &h);
    pScreen->PixmapPerDepth[0] = 
		(*pScreen->CreatePixmap)(pScreen, w, h, 1);

    /* fill stipple with 1 */
    tmpval[0] = GXcopy; tmpval[1] = 1; tmpval[2] = FillSolid;
    pgcScratch = GetScratchGC(1, pScreen);
    ChangeGC(pgcScratch, GCFunction | GCForeground | GCFillStyle, tmpval);
    ValidateGC((DrawablePtr)pScreen->PixmapPerDepth[0], pgcScratch);
    rect.x = 0;
    rect.y = 0;
    rect.width = w;
    rect.height = h;
    (*pgcScratch->PolyFillRect)(pScreen->PixmapPerDepth[0], 
				pgcScratch, 1, &rect);
    FreeScratchGC(pgcScratch);
}

FreeDefaultStipple(screenNum)
    int screenNum;
{
    ScreenPtr pScreen = &screenInfo.screen[screenNum];
    (*pScreen->DestroyPixmap)(pScreen->PixmapPerDepth[0]);
}


SetDashes(pGC, offset, ndash, pdash)
register GCPtr pGC;
unsigned offset;
register unsigned ndash;
register unsigned char *pdash;
{
    register long i;
    register unsigned char *p;
    GCInterestPtr	pQ, pQInit;
    BITS32 maskQ = 0;

    i = ndash;
    p = pdash;
    while (i--)
    {
	if (!*p++)
	{
	    /* dash segment must be > 0 */
	    clientErrorValue = 0;
	    return BadValue;
	}
    }

    pGC->serialNumber |= GC_CHANGE_SERIAL_BIT;
    if (offset != pGC->dashOffset)
    {
	pGC->dashOffset = offset;
	pGC->stateChanges |= GCDashOffset;
	maskQ |= GCDashOffset;
    }

    p = (unsigned char *)xalloc(ndash * sizeof(unsigned char));
    xfree(pGC->dash);
    pGC->dash = p;
    pGC->numInDashList = ndash;
    while(ndash--)
	*p++ = *pdash++;
    pGC->stateChanges |= GCDashList;
    maskQ |= GCDashList;

    pQ = pGC->pNextGCInterest;
    pQInit = (GCInterestPtr) &pGC->pNextGCInterest;
    do
    {
	if(pQ->ChangeInterestMask & maskQ)
	    (*pQ->ChangeGC)(pGC, pQ, maskQ);
	pQ = pQ->pNextGCInterest;
    }
    while(pQ != pQInit);
    return Success;
}


int
SetClipRects(pGC, xOrigin, yOrigin, nrects, prects, ordering)
    GCPtr		pGC;
    int			xOrigin, yOrigin;
    int			nrects;
    xRectangle		*prects;
    int			ordering;
{
    register xRectangle	*prectP, *prectN;
    register int	i;
    int			newct, size;
    xRectangle 		*prectsNew;
    GCInterestPtr	pQ, pQInit;

    switch(ordering)
    {
      case Unsorted:
	  newct = CT_UNSORTED;
	  break;
      case YSorted:
	  if(nrects > 1)
	  {
	      for(i = 1, prectP = prects, prectN = prects + 1;
		  i < nrects;
		  i++, prectP++, prectN++)
		  if(prectN->y < prectP->y)
		      return(BadMatch);
	  }
	  newct = CT_YSORTED;
	  break;
      case YXSorted:
	  if(nrects > 1)
	  {
	      for(i = 1, prectP = prects, prectN = prects + 1;
		  i < nrects;
		  i++, prectP++, prectN++)
		  if((prectN->y < prectP->y) ||
		      ( (prectN->y == prectP->y) &&
		        (prectN->x < prectP->x) ) )
		      return(BadMatch);
	  }
	  newct = CT_YXSORTED;
	  break;
      case YXBanded:
	  if(nrects > 1)
	  {
	      for(i = 1, prectP = prects, prectN = prects + 1;
		  i < nrects;
		  i++, prectP++, prectN++)
		  if((prectN->y < prectP->y) ||
		      ( (prectN->y == prectP->y) &&
		        (  (prectN->x < prectP->x)   ||
		           (prectN->height != prectP->height) ) ) )
		      return(BadMatch);
	  }
	  newct = CT_YXBANDED;
	  break;
    }

    pGC->serialNumber |= GC_CHANGE_SERIAL_BIT;
    pGC->clipOrg.x = xOrigin;
    pGC->stateChanges |= GCClipXOrigin;
		 
    pGC->clipOrg.y = yOrigin;
    pGC->stateChanges |= GCClipYOrigin;

    size = nrects * sizeof(xRectangle);
    prectsNew = (xRectangle *) xalloc(size);
    bcopy((char *)prects, (char *)prectsNew, size);
    (*pGC->ChangeClip)(pGC, newct, prectsNew, nrects);
    pQ = pGC->pNextGCInterest;
    pQInit = (GCInterestPtr) &pGC->pNextGCInterest;
    do
    {
	if(pQ->ChangeInterestMask & (GCClipXOrigin|GCClipYOrigin|GCClipMask))
	    (*pQ->ChangeGC)(pGC, pQ, (GCClipXOrigin|GCClipYOrigin|GCClipMask));
	pQ = pQ->pNextGCInterest;
    }
    while(pQ != pQInit);
    return Success;

}


/*
   sets reasonable defaults 
   if we can get a pre-allocated one, use it and mark it as used.
   if we can't, create one out of whole cloth (The Velveteen GC -- if
   you use it often enough it will become real.)
*/
GCPtr
GetScratchGC(depth, pScreen)
    register int depth;
    register ScreenPtr pScreen;
{
    register int i;
    register GCPtr pGC = (GCPtr)NULL;;

    for (i=0; i<=pScreen->numDepths; i++)
        if ( pScreen->GCperDepth[i]->depth == depth &&
	     !(pScreen->rgf & 1 << (i+1))
	   )
	{
	    pScreen->rgf |= 1 << (i+1);
            pGC = (pScreen->GCperDepth[i]);

	    pGC->alu = GXcopy;
	    pGC->planemask = ~0;
	    pGC->serialNumber = 0;
	    pGC->fgPixel = 0;
	    pGC->bgPixel = 1;
	    pGC->lineWidth = 0;
	    pGC->lineStyle = LineSolid;
	    pGC->capStyle = CapButt;
	    pGC->joinStyle = JoinMiter;
	    pGC->fillStyle = FillSolid;
	    pGC->fillRule = EvenOddRule;
	    pGC->arcMode = ArcChord;
	    pGC->patOrg.x = 0;
	    pGC->patOrg.y = 0;
	    pGC->subWindowMode = ClipByChildren;
	    pGC->graphicsExposures = FALSE;
	    pGC->clipOrg.x = 0;
	    pGC->clipOrg.y = 0;
	    /* can't change clip type, because we might drop storage */
	    pGC->stateChanges = (1 << GCLastBit+1) - 1;
	    return pGC;
	}
    /* if we make it this far, need to roll our own */
    pGC =  CreateScratchGC(pScreen, depth);
    pGC->graphicsExposures = FALSE;
    return pGC;
}

/*
   if the gc to free is in the table of pre-existing ones,
mark it as available.
   if not, free it for real
*/
void
FreeScratchGC(pGC)
    register GCPtr pGC;
{
    register ScreenPtr pScreen = pGC->pScreen;
    register int i;

    for (i=0; i<=pScreen->numDepths; i++)
    {
        if ( pScreen->GCperDepth[i] == pGC)
	{
	    pScreen->rgf &= ~(1<<i+1);
	    return;
	}
    }
    (void)FreeGC(pGC, (GContext)0);
}


