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
/* $Header: vgascrinit.c,v 1.2 88/02/10 10:19:59 rws Exp $ */
/* $ACIS:vgascrinit.c 1.5$ */

#ifndef lint
static char *rcsid = "$Header: vgascrinit.c,v 1.2 88/02/10 10:19:59 rws Exp $";
#endif

#include <stdio.h>
#include <sys/types.h>
#include <sys/file.h>

#include "X.h"
#include "Xproto.h"	/* for xColorItem */
#include "Xmd.h"
#include "scrnintstr.h"
#include "pixmapstr.h"
#include "resource.h"
#include "colormap.h"
#include "../../mfb/mfb.h"
#include "mistruct.h"
#include "dix.h"
#include "rtutils.h"
#include "ppc.h"
#include "ppcprocs.h"
#include "vgaprocs.h"
#include "vga_video.h"
#include "servermd.h"

extern mfbFreeVisual() ;

Colormap    vgaDefaultColormap ;

extern int vgaNumberOfPlanes ;
extern int vgaDisplayTubeType ;

static DepthRec depths[] = {
/* depth	numVid	vids */
    1,		0,	NULL,
    4,		1,	NULL,
} ;

static void
vgaFreeVisual( p, id )
register pointer const p ;
register int id ;
{
    /*$$$XXX are visuals the same (no allocs inside a priv or whatever?*/
    Xfree( p ) ;
    return ;
}

/* dts * (inch/dot) * (25.4 mm / inch) = mm */
Bool
vgaInitScreen( index, pScreen, pbits, xsize, ysize, dpix, dpiy )
    register int index ;
    register ScreenPtr const pScreen ;
    pointer pbits ;		/* pointer to screen bitmap */
    int xsize, ysize ;		/* in pixels */
    int dpix, dpiy ;		/* dots per inch */
{
    register PixmapPtr pPixmap ;
    register ppcScrnPrivPtr devPriv ;
    VisualPtr	pVisual ;
    long	*pVids ;

    TRACE( ( "vgaInitScreen(index= %d, pScreen= 0x%x, pbits= 0x%x, xsize= %d, \
ysize= %d, dpix= %d, dpiy = %d)\n", index, pScreen, pbits, xsize, ysize, dpix, dpiy ) ) ;

    pScreen->myNum = index ;
    pScreen->width = xsize ;
    pScreen->height = ysize ;
    pScreen->mmWidth = ( xsize * 254 ) / ( dpix * 10 ) ;
    pScreen->mmHeight = ( ysize * 254 ) / ( dpiy * 10 ) ;
    pScreen->numDepths = 2 ;	/* 1 or 4 == 2 depths ? 3 w/ depth == 8 ? */
    pScreen->allowedDepths = depths ;

    pScreen->rootDepth = vgaNumberOfPlanes ;
    pScreen->rootVisual = FakeClientID( 0 ) ;

    pScreen->defColormap = (Colormap) FakeClientID( 0 ) ;
    pScreen->minInstalledCmaps = 1 ;
    pScreen->maxInstalledCmaps = 1 ;
    pScreen->whitePixel = 15 ;
    pScreen->blackPixel = 0 ;
    pScreen->backingStoreSupport = NotUseful ;
    pScreen->saveUnderSupport = NotUseful ;

    /* cursmin and cursmax are device specific */

    pScreen->numVisuals = 1 ;
    pScreen->visuals = pVisual = (VisualPtr) Xalloc( sizeof( VisualRec ) ) ;

    pScreen->devPrivate =
	(pointer) ( devPriv = 
		(ppcScrnPrivPtr) Xalloc( sizeof( ppcScrnPriv ) ) ) ;
    devPriv->blit = vgaBitBlt ;
    devPriv->solidFill = vgaFillSolid ;
#ifdef VGA_SPECIFIC_ROUTINES
    devPriv->tileFill = vgaTileRect ;
#else
    devPriv->tileFill = ppcTileRect ;
#endif
    devPriv->stipFill = vgaFillStipple ;
/*
    devPriv->opstipFill = vgaFillOpaqueStipple ;
*/
    devPriv->opstipFill = ppcOpaqueStipple ;
    devPriv->imageFill = vgaDrawColorImage ;
    devPriv->imageRead = vgaReadColorImage ;
    devPriv->lineHorz = vgaHorzLine ;
    devPriv->lineVert = vgaVertLine ;
    devPriv->lineBres = vgaBresLine ;
    devPriv->setColor = vgaSetColor ;
    devPriv->RecolorCursor = NoopDDA;
    devPriv->monoFill = NoopDDA;
    devPriv->glyphFill = vgaDrawGlyph ;

    pPixmap = (PixmapPtr) pScreen->devPrivate ;

    pPixmap->drawable.pScreen = pScreen ;
    pPixmap->drawable.serialNumber = 0 ;
    pPixmap->drawable.type = DRAWABLE_PIXMAP ;
    pPixmap->height = ysize ;
    pPixmap->width = xsize ;
    pPixmap->drawable.depth = vgaNumberOfPlanes ;
    pPixmap->devKind = PixmapBytePad( xsize, vgaNumberOfPlanes ) ;
    pPixmap->refcnt = 1 ;
    pPixmap->devPrivate = NULL ;

    /* anything that mfb doesn't know about is assumed to be done
       elsewhere.  (we put in no-op only for things that we KNOW
       are really no-op.
    */
    pScreen->CreateWindow = ppcCreateWindow ;
    pScreen->DestroyWindow = ppcDestroyWindow ;
    pScreen->PositionWindow = ppcPositionWindow ;
    pScreen->ChangeWindowAttributes = ppcChangeWindowAttributes ;
    pScreen->RealizeWindow = mfbMapWindow ;
    pScreen->UnrealizeWindow = mfbUnmapWindow ;

    pScreen->RealizeFont = mfbRealizeFont ;
    pScreen->UnrealizeFont = mfbUnrealizeFont ;
    pScreen->GetImage = miGetImage ;
    pScreen->GetSpans = ppcGetSpans ;
    pScreen->CreateGC = vgaCreateGC ;
    pScreen->CreatePixmap = ppcCreatePixmap ;
    pScreen->DestroyPixmap = mfbDestroyPixmap ;
    pScreen->ValidateTree = miValidateTree ;

    pScreen->CreateColormap = NoopDDA ;
    pScreen->DestroyColormap = NoopDDA ;
    pScreen->InstallColormap = ppcInstallColormap ;
    pScreen->UninstallColormap = ppcUninstallColormap ;
    pScreen->ListInstalledColormaps = ppcListInstalledColormaps ;
    pScreen->StoreColors = ppcStoreColors ;

    pScreen->RegionCreate = miRegionCreate ;
    pScreen->RegionCopy = miRegionCopy ;
    pScreen->RegionDestroy = miRegionDestroy ;
    pScreen->Intersect = miIntersect ;
    pScreen->Inverse = miInverse ;
    pScreen->Union = miUnion ;
    pScreen->Subtract = miSubtract ;
    pScreen->RegionReset = miRegionReset ;
    pScreen->TranslateRegion = miTranslateRegion ;
    pScreen->RectIn = miRectIn ;
    pScreen->PointInRegion = miPointInRegion ;
    pScreen->WindowExposures = miWindowExposures ;
    pScreen->RegionNotEmpty = miRegionNotEmpty ;
    pScreen->RegionEmpty = miRegionEmpty ;
    pScreen->RegionExtents = miRegionExtents ;

    pScreen->BlockHandler = NoopDDA ;
    pScreen->WakeupHandler = NoopDDA ;
    pScreen->blockData = (pointer) 0 ;
    pScreen->wakeupData = (pointer) 0 ;

    pVisual->class = ( vgaDisplayTubeType == COLOR_TUBE )
	? PseudoColor : GrayScale ;

    pScreen->ResolveColor = ppcResolveColor ;
    pVisual->redMask = 0 ;
    pVisual->greenMask = 0 ;
    pVisual->blueMask = 0 ;
    pVisual->bitsPerRGBValue = 6 ;
    pVisual->ColormapEntries = 1 << vgaNumberOfPlanes ;
    pVisual->vid = pScreen->rootVisual ;
    pVisual->screen = index ;

    depths[1].vids = pVids = (long *) Xalloc( sizeof( long ) ) ;
    pVids[0] = pScreen->rootVisual ;	/* our one and only visual */
    AddResource( pScreen->rootVisual, RT_VISUALID,
		 pVisual, vgaFreeVisual, RC_CORE ) ;

    /* we MIGHT return 0 if we had been keeping track of potential
       allocation failures.  one day someone will clean this up.
    */
    return 1 ;
}
