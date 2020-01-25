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

/* $Header:mpelscrinit.c 1.3$ */
/* $ACIS:mpelscrinit.c 1.3$ */
/* $Source: /ibm/acis/usr/src/X11/server/ddx/ibm/mpel/RCS/mpelscrinit.c,v $ */

#ifndef lint
static char *rcsid = "$Header:mpelscrinit.c 1.3$";
#endif

#include "X.h"
#include "Xproto.h"	/* for xColorItem */
#include "Xmd.h"
#include "servermd.h"
#include "scrnintstr.h"
#include "pixmapstr.h"
#include "resource.h"
#include "colormap.h"
#include "mfb.h"
#include "mistruct.h"
#include "dix.h"

#include "rtutils.h"

#include "ppc.h"
#include "ppcprocs.h"

#include "mpelprocs.h"

Colormap    mpelDefaultColormap;

static DepthRec depths[2] ;

/***==================================================================***/

static void
mpelFreeVisual(p, id)
    pointer p;
    int id;
{
    /*$$$XXX are visuals the same (no allocs inside a priv or whatever?*/
    Xfree(p);
}

/***==================================================================***/

/* dts * (inch/dot) * (25.4 mm / inch) = mm */
Bool
mpelInitScreen(index, pScreen, pbits, xsize, ysize, dpix, dpiy)
    int index;
    register ScreenPtr pScreen;
    pointer pbits;		/* pointer to screen bitmap */
    int xsize, ysize;		/* in pixels */
    int dpix, dpiy;		/* dots per inch */
{
    VisualPtr	pVisual;
    long	*pVids;
    register PixmapPtr pPixmap;
    ppcScrnPrivPtr devPriv;

    TRACE(("mpelInitScreen(%d,0x%x,0x%x,%d,%d,%d,%d)\n", 
			index, pScreen, pbits, xsize, ysize, dpix, dpiy));

    pScreen->myNum = index;
    pScreen->width = xsize;
    pScreen->height = ysize;
    pScreen->mmWidth = (xsize * 254) / (dpix * 10);
    pScreen->mmHeight = (ysize * 254) / (dpiy * 10);
    pScreen->numDepths = 1; 
    depths[0].depth = 8;
    depths[0].numVids = 1;
    depths[0].vids = 0;
    pScreen->allowedDepths = depths;

    pScreen->rootDepth = 8;
    pScreen->rootVisual = FakeClientID(0);

    pScreen->defColormap = (Colormap) FakeClientID(0);
    pScreen->minInstalledCmaps = 1;
    pScreen->maxInstalledCmaps = 1;
    pScreen->whitePixel = 1;
    pScreen->blackPixel = 0;
    pScreen->backingStoreSupport = NotUseful;
    pScreen->saveUnderSupport = NotUseful;

    /* cursmin and cursmax are device specific */

    pScreen->numVisuals = 1;
    pScreen->visuals = pVisual = (VisualPtr) Xalloc(sizeof (VisualRec));

    pScreen->devPrivate = devPriv = (ppcScrnPrivPtr)Xalloc(sizeof(ppcScrnPriv));
    devPriv->blit = mpelBitBlt;
    devPriv->solidFill = mpelFillSolid;
    devPriv->tileFill = mpelTile;
    devPriv->stipFill = ppcStipple;
    devPriv->opstipFill = ppcOpaqueStipple;
    devPriv->lineBres = NoopDDA;
    devPriv->imageFill = mpelDrawColorImage;
    devPriv->imageRead = mpelReadColorImage;
    devPriv->lineHorz = NoopDDA;
    devPriv->lineVert = NoopDDA;
    devPriv->setColor = mpelSetColor;
    devPriv->RecolorCursor = NoopDDA;
    devPriv->monoFill = mpelDrawMonoImage;
    devPriv->glyphFill = mpelDrawMonoByteImage;
    devPriv->planeRead = mpelGetPlane;

    pPixmap = (PixmapPtr) pScreen->devPrivate;

    pPixmap->drawable.type = DRAWABLE_PIXMAP;
    pPixmap->drawable.depth = 8;
    pPixmap->drawable.pScreen = pScreen;
    pPixmap->drawable.serialNumber = 0;
    pPixmap->width = xsize;
    pPixmap->height = ysize;
    pPixmap->refcnt = 1;
    pPixmap->devPrivate = NULL;
    pPixmap->devKind = PixmapBytePad(xsize, 8);

    /* anything that mfb doesn't know about is assumed to be done
       elsewhere.  (we put in no-op only for things that we KNOW
       are really no-op.
    */
    pScreen->CreateWindow = ppcCreateWindow;
    pScreen->DestroyWindow = ppcDestroyWindow;
    pScreen->PositionWindow = ppcPositionWindow;
    pScreen->ChangeWindowAttributes = ppcChangeWindowAttributes;
    pScreen->RealizeWindow = mfbMapWindow;
    pScreen->UnrealizeWindow = mfbUnmapWindow;

    pScreen->RealizeFont = mpelRealizeFont;
    pScreen->UnrealizeFont = mpelUnrealizeFont;
    pScreen->GetImage = ppcGetImage;
    pScreen->GetSpans = ppcGetSpans;
    pScreen->CreateGC = mpelCreateGC;
    pScreen->CreatePixmap = ppcCreatePixmap;
    pScreen->DestroyPixmap = mfbDestroyPixmap;
    pScreen->ValidateTree = miValidateTree;

    pScreen->CreateColormap = NoopDDA;
    pScreen->DestroyColormap = NoopDDA;
    pScreen->InstallColormap = mpelInstallColormap;
    pScreen->UninstallColormap = mpelUninstallColormap;
    pScreen->ListInstalledColormaps = ppcListInstalledColormaps;
    pScreen->StoreColors = ppcStoreColors;
    pScreen->ResolveColor = ppcResolveColor;

    pScreen->RegionCreate = miRegionCreate;
    pScreen->RegionCopy = miRegionCopy;
    pScreen->RegionDestroy = miRegionDestroy;
    pScreen->Intersect = miIntersect;
    pScreen->Inverse = miInverse;
    pScreen->Union = miUnion;
    pScreen->Subtract = miSubtract;
    pScreen->RegionReset = miRegionReset;
    pScreen->TranslateRegion = miTranslateRegion;
    pScreen->RectIn = miRectIn;
    pScreen->PointInRegion = miPointInRegion;
    pScreen->WindowExposures = miWindowExposures;
    pScreen->RegionNotEmpty = miRegionNotEmpty;
    pScreen->RegionEmpty = miRegionEmpty;
    pScreen->RegionExtents = miRegionExtents;

    pScreen->BlockHandler = NoopDDA;
    pScreen->WakeupHandler = NoopDDA;
    pScreen->blockData = (pointer)0;
    pScreen->wakeupData = (pointer)0;

    pVisual->vid = pScreen->rootVisual;
    pVisual->screen = index;
    pVisual->class = PseudoColor;
    pVisual->redMask = 0;
    pVisual->greenMask = 0; /* no use to PseudoColor */
    pVisual->blueMask = 0;
    pVisual->bitsPerRGBValue = 4; 
    pVisual->ColormapEntries = 256;

    depths[0].vids = pVids = (long *) Xalloc(sizeof (long));
    pVids[0] = pScreen->rootVisual;	/* our one and only visual */
    AddResource(
	pScreen->rootVisual, RT_VISUALID, pVisual, mpelFreeVisual, RC_CORE);

    return 1;
}

