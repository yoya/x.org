/*
 * misprite.c
 *
 * machine independent software sprite routines
 */

/*

Copyright (c) 1989  X Consortium

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

*/

# include   "X.h"
# include   "Xproto.h"
# include   "misc.h"
# include   "pixmapstr.h"
# include   "input.h"
# include   "mi.h"
# include   "cursorstr.h"
# include   "font.h"
# include   "scrnintstr.h"
# include   "colormapst.h"
# include   "windowstr.h"
# include   "gcstruct.h"
# include   "mipointer.h"
# include   "mispritest.h"
# include   "dixfontstr.h"
# include   "fontstruct.h"

/*
 * screen wrappers
 */

static int  miSpriteScreenIndex;
static unsigned long miSpriteGeneration = 0;

static Bool	    miSpriteCloseScreen();
static void	    miSpriteGetImage();
static void	    miSpriteGetSpans();
static Bool	    miSpriteCreateGC();
#ifndef XTHREADS
static void	    miSpriteSourceValidate();
static void	    miSpriteBlockHandler();
#endif /* XTHREADS */
static void	    miSpriteInstallColormap();
static void	    miSpriteStoreColors();

static void	    miSpritePaintWindowBackground();
static void	    miSpritePaintWindowBorder();
static void	    miSpriteCopyWindow();
static void	    miSpriteClearToBackground();

static void	    miSpriteSaveDoomedAreas();
static RegionPtr    miSpriteRestoreAreas();
static void	    miSpriteComputeSaved();

#ifndef XTHREADS
#define SCREEN_PROLOGUE(pScreen, field)\
  ((pScreen)->field = \
   ((miSpriteScreenPtr) (pScreen)->devPrivates[miSpriteScreenIndex].ptr)->field)

#define SCREEN_EPILOGUE(pScreen, field, wrapper)\
    ((pScreen)->field = wrapper)
#endif /* XTHREADS */

/*
 * GC func wrappers
 */

static int  miSpriteGCIndex;

static void miSpriteValidateGC (),  miSpriteCopyGC ();
static void miSpriteDestroyGC(),    miSpriteChangeGC();
static void miSpriteChangeClip(),   miSpriteDestroyClip();
static void miSpriteCopyClip();

static GCFuncs	miSpriteGCFuncs = {
    miSpriteValidateGC,
    miSpriteChangeGC,
    miSpriteCopyGC,
    miSpriteDestroyGC,
    miSpriteChangeClip,
    miSpriteDestroyClip,
    miSpriteCopyClip,
};

#define GC_FUNC_PROLOGUE(pGC)					\
    miSpriteGCPtr   pGCPriv =					\
	(miSpriteGCPtr) (pGC)->devPrivates[miSpriteGCIndex].ptr;\
    (pGC)->funcs = pGCPriv->wrapFuncs;				\
    if (pGCPriv->wrapOps)					\
	(pGC)->ops = pGCPriv->wrapOps;

#define GC_FUNC_EPILOGUE(pGC)					\
    pGCPriv->wrapFuncs = (pGC)->funcs;				\
    (pGC)->funcs = &miSpriteGCFuncs;				\
    if (pGCPriv->wrapOps)					\
    {								\
	pGCPriv->wrapOps = (pGC)->ops;				\
	(pGC)->ops = &miSpriteGCOps;				\
    }

/*
 * GC op wrappers
 */

static void	    miSpriteFillSpans(),	miSpriteSetSpans();
static void	    miSpritePutImage();
static RegionPtr    miSpriteCopyArea(),		miSpriteCopyPlane();
static void	    miSpritePolyPoint(),	miSpritePolylines();
static void	    miSpritePolySegment(),	miSpritePolyRectangle();
static void	    miSpritePolyArc(),		miSpriteFillPolygon();
static void	    miSpritePolyFillRect(),	miSpritePolyFillArc();
static int	    miSpritePolyText8(),	miSpritePolyText16();
static void	    miSpriteImageText8(),	miSpriteImageText16();
static void	    miSpriteImageGlyphBlt(),	miSpritePolyGlyphBlt();
static void	    miSpritePushPixels();
#ifdef NEED_LINEHELPER
static void	    miSpriteLineHelper();
#endif

static GCOps miSpriteGCOps = {
    miSpriteFillSpans,	    miSpriteSetSpans,	    miSpritePutImage,	
    miSpriteCopyArea,	    miSpriteCopyPlane,	    miSpritePolyPoint,
    miSpritePolylines,	    miSpritePolySegment,    miSpritePolyRectangle,
    miSpritePolyArc,	    miSpriteFillPolygon,    miSpritePolyFillRect,
    miSpritePolyFillArc,    miSpritePolyText8,	    miSpritePolyText16,
    miSpriteImageText8,	    miSpriteImageText16,    miSpriteImageGlyphBlt,
    miSpritePolyGlyphBlt,   miSpritePushPixels
#ifdef NEED_LINEHELPER
    , miSpriteLineHelper
#endif
};

/*
 * testing only -- remove cursor for every draw.  Eventually,
 * each draw operation will perform a bounding box check against
 * the saved cursor area
 */

#define GC_SETUP_CHEAP(pDrawable)				    \
    miSpriteScreenPtr	pScreenPriv = (miSpriteScreenPtr)	    \
	(pDrawable)->pScreen->devPrivates[miSpriteScreenIndex].ptr; \

#define GC_SETUP(pDrawable, pGC)				    \
    GC_SETUP_CHEAP(pDrawable)					    \
    miSpriteGCPtr	pGCPrivate = (miSpriteGCPtr)		    \
	(pGC)->devPrivates[miSpriteGCIndex].ptr;		    \
    GCFuncs *oldFuncs = (pGC)->funcs;				    \

#ifndef XTHREADS
#define GC_SETUP_AND_CHECK(pDrawable, pGC)			    \
    GC_SETUP(pDrawable, pGC);					    \
    if (GC_CHECK((WindowPtr)pDrawable))				    \
	miSpriteRemoveCursor (pDrawable->pScreen);
#endif /* XTHREADS */
    
#ifndef XTHREADS
#define GC_CHECK(pWin)						    \
    (pScreenPriv->isUp &&					    \
        (pScreenPriv->pCacheWin == pWin ?			    \
	    pScreenPriv->isInCacheWin : (			    \
	    ((int) (pScreenPriv->pCacheWin = (pWin))) ,		    \
	    (pScreenPriv->isInCacheWin =			    \
		(pWin)->drawable.x < pScreenPriv->saved.x2 &&	    \
		pScreenPriv->saved.x1 < (pWin)->drawable.x +	    \
				    (int) (pWin)->drawable.width && \
		(pWin)->drawable.y < pScreenPriv->saved.y2 &&	    \
		pScreenPriv->saved.y1 < (pWin)->drawable.y +	    \
				    (int) (pWin)->drawable.height &&\
		RECT_IN_REGION((pWin)->drawable.pScreen, &(pWin)->borderClip, \
			&pScreenPriv->saved) != rgnOUT))))
#else /* XTHREADS */
#define GC_CHECK(pWin)						    \
	    ( (pWin)->drawable.type == DRAWABLE_WINDOW &&	    \
		(pWin)->drawable.x < pScreenPriv->saved.x2 &&	    \
		pScreenPriv->saved.x1 < (pWin)->drawable.x +	    \
				    (int) (pWin)->drawable.width && \
		(pWin)->drawable.y < pScreenPriv->saved.y2 &&	    \
		pScreenPriv->saved.y1 < (pWin)->drawable.y +	    \
				    (int) (pWin)->drawable.height &&\
		RECT_IN_REGION((pWin)->drawable.pScreen, &(pWin)->borderClip, \
			&pScreenPriv->saved) != rgnOUT)
#endif /* XTHREADS */

#define GC_OP_PROLOGUE(pGC) { \
    (pGC)->funcs = pGCPrivate->wrapFuncs; \
    (pGC)->ops = pGCPrivate->wrapOps; \
    }

#ifndef XTHREADS
#define GC_OP_EPILOGUE(pGC) { \
    (pGC)->funcs = oldFuncs; \
    (pGC)->ops = &miSpriteGCOps; \
    }
#else /* XTHREADS */
#define GC_OP_EPILOGUE(pGC) { \
    pGCPrivate->wrapOps = (pGC)->ops; \
    (pGC)->funcs = oldFuncs; \
    (pGC)->ops = &miSpriteGCOps; \
    }
#endif /* XTHREADS */

/*
 * pointer-sprite method table
 */

static Bool miSpriteRealizeCursor (),	miSpriteUnrealizeCursor ();
static void miSpriteSetCursor (),	miSpriteMoveCursor ();

miPointerSpriteFuncRec miSpritePointerFuncs = {
    miSpriteRealizeCursor,
    miSpriteUnrealizeCursor,
    miSpriteSetCursor,
    miSpriteMoveCursor,
};

/*
 * other misc functions
 */

static void miSpriteRemoveCursor (),	miSpriteRestoreCursor();

#ifdef XTHREADS
/*
 * pointer lock/unlock functions
 * 
 * miPointerLock   -- Lock the pointer data.
 *		      Return TRUE if successfully locked the pointer data.
 *		      Return FALSE if already locked the pointer data by
 *		      the same thread.
 *	
 * miPointerUnlock -- Unlock to the pointer data.
 *
 */
extern Bool miPointerLock ();	/* mipointer.c */
extern void miPointerUnlock();  /* mipointer.c */
#endif /* XTHREADS */

/*
 * miSpriteInitialize -- called from device-dependent screen
 * initialization proc after all of the function pointers have
 * been stored in the screen structure.
 */

Bool
miSpriteInitialize (pScreen, cursorFuncs, screenFuncs)
    ScreenPtr		    pScreen;
    miSpriteCursorFuncPtr   cursorFuncs;
    miPointerScreenFuncPtr  screenFuncs;
{
    miSpriteScreenPtr	pPriv;
    VisualPtr		pVisual;
#ifdef XTHREADS
    int		        i;
#endif /* XTHREADS */
    
    if (miSpriteGeneration != serverGeneration)
    {
	miSpriteScreenIndex = AllocateScreenPrivateIndex ();
	if (miSpriteScreenIndex < 0)
	    return FALSE;
	miSpriteGeneration = serverGeneration;
	miSpriteGCIndex = AllocateGCPrivateIndex ();
    }
    if (!AllocateGCPrivate(pScreen, miSpriteGCIndex, sizeof(miSpriteGCRec)))
	return FALSE;
    pPriv = (miSpriteScreenPtr) xalloc (sizeof (miSpriteScreenRec));
    if (!pPriv)
	return FALSE;
    if (!miPointerInitialize (pScreen, &miSpritePointerFuncs, screenFuncs,TRUE))
    {
	xfree ((pointer) pPriv);
	return FALSE;
    }
    for (pVisual = pScreen->visuals;
	 pVisual->vid != pScreen->rootVisual;
	 pVisual++)
	;
    pPriv->pVisual = pVisual;
    pPriv->CloseScreen = pScreen->CloseScreen;
    pPriv->GetImage = pScreen->GetImage;
    pPriv->GetSpans = pScreen->GetSpans;
#ifndef XTHREADS
    pPriv->SourceValidate = pScreen->SourceValidate;
#endif /* XTHREADS */
    pPriv->CreateGC = pScreen->CreateGC;
#ifndef XTHREADS
    pPriv->BlockHandler = pScreen->BlockHandler;
#endif /* XTHREADS */
    pPriv->InstallColormap = pScreen->InstallColormap;
    pPriv->StoreColors = pScreen->StoreColors;

    pPriv->PaintWindowBackground = pScreen->PaintWindowBackground;
    pPriv->PaintWindowBorder = pScreen->PaintWindowBorder;
    pPriv->CopyWindow = pScreen->CopyWindow;
    pPriv->ClearToBackground = pScreen->ClearToBackground;

    pPriv->SaveDoomedAreas = pScreen->SaveDoomedAreas;
    pPriv->RestoreAreas = pScreen->RestoreAreas;

    pPriv->pCursor = NULL;
    pPriv->x = 0;
    pPriv->y = 0;
    pPriv->isUp = FALSE;
#ifndef XTHREADS
    pPriv->shouldBeUp = FALSE;
    pPriv->pCacheWin = NullWindow;
    pPriv->isInCacheWin = FALSE;
#endif /* XTHREADS */
    pPriv->checkPixels = TRUE;
    pPriv->pInstalledMap = NULL;
    pPriv->pColormap = NULL;
    pPriv->funcs = cursorFuncs;
    pPriv->colors[SOURCE_COLOR].red = 0;
    pPriv->colors[SOURCE_COLOR].green = 0;
    pPriv->colors[SOURCE_COLOR].blue = 0;
    pPriv->colors[MASK_COLOR].red = 0;
    pPriv->colors[MASK_COLOR].green = 0;
    pPriv->colors[MASK_COLOR].blue = 0;
    pScreen->devPrivates[miSpriteScreenIndex].ptr = (pointer) pPriv;
    pScreen->CloseScreen = miSpriteCloseScreen;
    pScreen->GetImage = miSpriteGetImage;
    pScreen->GetSpans = miSpriteGetSpans;
    pScreen->CreateGC = miSpriteCreateGC;
#ifndef XTHREADS
    pScreen->SourceValidate = miSpriteSourceValidate;
    pScreen->BlockHandler = miSpriteBlockHandler;
#endif /* XTHREADS */
    pScreen->InstallColormap = miSpriteInstallColormap;
    pScreen->StoreColors = miSpriteStoreColors;

    pScreen->PaintWindowBackground = miSpritePaintWindowBackground;
    pScreen->PaintWindowBorder = miSpritePaintWindowBorder;
    pScreen->CopyWindow = miSpriteCopyWindow;
    pScreen->ClearToBackground = miSpriteClearToBackground;

    pScreen->SaveDoomedAreas = miSpriteSaveDoomedAreas;
    pScreen->RestoreAreas = miSpriteRestoreAreas;

    return TRUE;
}

/*
 * Screen wrappers
 */

/*
 * CloseScreen wrapper -- unwrap everything, free the private data
 * and call the wrapped function
 */

static Bool
miSpriteCloseScreen (i, pScreen)
    ScreenPtr	pScreen;
{
    miSpriteScreenPtr   pScreenPriv;

    pScreenPriv = (miSpriteScreenPtr) pScreen->devPrivates[miSpriteScreenIndex].ptr;

    pScreen->CloseScreen = pScreenPriv->CloseScreen;
    pScreen->GetImage = pScreenPriv->GetImage;
    pScreen->GetSpans = pScreenPriv->GetSpans;
    pScreen->CreateGC = pScreenPriv->CreateGC;
#ifndef XTHREADS
    pScreen->SourceValidate = pScreenPriv->SourceValidate;
    pScreen->BlockHandler = pScreenPriv->BlockHandler;
#endif /* XTHREADS */
    pScreen->InstallColormap = pScreenPriv->InstallColormap;
    pScreen->StoreColors = pScreenPriv->StoreColors;

    pScreen->PaintWindowBackground = pScreenPriv->PaintWindowBackground;
    pScreen->PaintWindowBorder = pScreenPriv->PaintWindowBorder;
    pScreen->CopyWindow = pScreenPriv->CopyWindow;
    pScreen->ClearToBackground = pScreenPriv->ClearToBackground;

    pScreen->SaveDoomedAreas = pScreenPriv->SaveDoomedAreas;
    pScreen->RestoreAreas = pScreenPriv->RestoreAreas;

    xfree ((pointer) pScreenPriv);

    return (*pScreen->CloseScreen) (i, pScreen);
}

static void
miSpriteGetImage (pDrawable, sx, sy, w, h, format, planemask, pdstLine)
    DrawablePtr	    pDrawable;
    int		    sx, sy, w, h;
    unsigned int    format;
    unsigned long   planemask;
    char	    *pdstLine;
{
    ScreenPtr	    pScreen = pDrawable->pScreen;
    miSpriteScreenPtr    pScreenPriv;
#ifdef XTHREADS
    Bool	      drawCursor = FALSE;
#endif /* XTHREADS */
    
#ifndef XTHREADS
    SCREEN_PROLOGUE (pScreen, GetImage);
#endif /* XTHREADS */

    pScreenPriv = (miSpriteScreenPtr) pScreen->devPrivates[miSpriteScreenIndex].ptr;

    if (pDrawable->type == DRAWABLE_WINDOW &&
#ifndef XTHREADS
        pScreenPriv->isUp &&
#endif /* XTHREADS */
	ORG_OVERLAP(&pScreenPriv->saved,pDrawable->x,pDrawable->y, sx, sy, w, h))
    {
#ifdef XTHREADS
      if(miPointerLock())
      {
#endif /* XTHREADS */
	miSpriteRemoveCursor (pScreen);
#ifdef XTHREADS
	drawCursor = TRUE;
      }
#endif /* XTHREADS */
    }

#ifndef XTHREADS
    (*pScreen->GetImage)
#else /* XTHREADS */
    (*pScreenPriv->GetImage)
#endif /* XTHREADS */
	(pDrawable, sx, sy, w, h, format, planemask, pdstLine);

#ifndef XTHREADS
    SCREEN_EPILOGUE (pScreen, GetImage, miSpriteGetImage);
#else /* XTHREADS */
    if(drawCursor)
    {
	miSpriteRestoreCursor (pScreen);
	miPointerUnlock();
    }
#endif /* XTHREADS */
}

static void
miSpriteGetSpans (pDrawable, wMax, ppt, pwidth, nspans, pdstStart)
    DrawablePtr	pDrawable;
    int		wMax;
    DDXPointPtr	ppt;
    int		*pwidth;
    int		nspans;
    char	*pdstStart;
{
    ScreenPtr		    pScreen = pDrawable->pScreen;
    miSpriteScreenPtr	    pScreenPriv;
#ifdef XTHREADS
    Bool		    drawCursor = FALSE;
#endif /* XTHREADS */
    
#ifndef XTHREADS
    SCREEN_PROLOGUE (pScreen, GetSpans);
#endif /* XTHREADS */

    pScreenPriv = (miSpriteScreenPtr) pScreen->devPrivates[miSpriteScreenIndex].ptr;

    if ((pDrawable->type == DRAWABLE_WINDOW)
#ifndef XTHREADS
	&& pScreenPriv->isUp
#endif /* XTHREADS */
	)
    {
	register DDXPointPtr    pts;
	register int    	*widths;
	register int    	nPts;
	register int    	xorg,
				yorg;

	xorg = pDrawable->x;
	yorg = pDrawable->y;

	for (pts = ppt, widths = pwidth, nPts = nspans;
	     nPts--;
	     pts++, widths++)
 	{
	    if (SPN_OVERLAP(&pScreenPriv->saved,pts->y+yorg,
			     pts->x+xorg,*widths))
	    {
#ifndef XTHREADS
		miSpriteRemoveCursor (pScreen);
#else /* XTHREADS */
	    	if(miPointerLock())
		{
		    miSpriteRemoveCursor (pScreen);
		    drawCursor = TRUE;
		}
#endif /* XTHREADS */
		break;
	    }
	}
    }

#ifndef XTHREADS
    (*pScreen->GetSpans)
#else /* XTHREADS */
    (*pScreenPriv->GetSpans)
#endif /* XTHREADS */
	(pDrawable, wMax, ppt, pwidth, nspans, pdstStart);

#ifndef XTHREADS
    SCREEN_EPILOGUE (pScreen, GetSpans, miSpriteGetSpans);
#else /* XTHREADS */
    if(drawCursor)
    {
	miSpriteRestoreCursor (pScreen);
	miPointerUnlock();
    }
#endif /* XTHREADS */
}

#ifndef XTHREADS
static void
miSpriteSourceValidate (pDrawable, x, y, width, height)
    DrawablePtr	pDrawable;
    int		x, y, width, height;
{
    ScreenPtr		    pScreen = pDrawable->pScreen;
    miSpriteScreenPtr	    pScreenPriv;
    
    SCREEN_PROLOGUE (pScreen, SourceValidate);

    pScreenPriv = (miSpriteScreenPtr) pScreen->devPrivates[miSpriteScreenIndex].ptr;

    if (pDrawable->type == DRAWABLE_WINDOW && pScreenPriv->isUp &&
	ORG_OVERLAP(&pScreenPriv->saved, pDrawable->x, pDrawable->y,
		    x, y, width, height))
    {
	miSpriteRemoveCursor (pScreen);
    }

    if (pScreen->SourceValidate)
	(*pScreen->SourceValidate) (pDrawable, x, y, width, height);

    SCREEN_EPILOGUE (pScreen, SourceValidate, miSpriteSourceValidate);
}
#endif /* XTHREADS */

static Bool
miSpriteCreateGC (pGC)
    GCPtr   pGC;
{
    ScreenPtr	    pScreen = pGC->pScreen;
    Bool	    ret;
    miSpriteGCPtr   pPriv;

#ifdef XTHREADS
    miSpriteScreenPtr  pScreenPriv;
#else /* XTHREADS */
    SCREEN_PROLOGUE (pScreen, CreateGC);
#endif /* XTHREADS */
    
    pPriv = (miSpriteGCPtr)pGC->devPrivates[miSpriteGCIndex].ptr;

#ifdef XTHREADS
    pScreenPriv = (miSpriteScreenPtr) pScreen->devPrivates[miSpriteScreenIndex].ptr;
#endif /* XTHREADS */

    ret =
#ifndef XTHREADS
	(*pScreen->CreateGC)
#else /* XTHREADS */
	(*pScreenPriv->CreateGC)
#endif /* XTHREADS */
	    (pGC);

    pPriv->wrapOps = NULL;
    pPriv->wrapFuncs = pGC->funcs;
    pGC->funcs = &miSpriteGCFuncs;

#ifndef XTHREADS
    SCREEN_EPILOGUE (pScreen, CreateGC, miSpriteCreateGC);
#endif /* XTHREADS */

    return ret;
}

#ifndef XTHREADS
static void
miSpriteBlockHandler (i, blockData, pTimeout, pReadmask)
    int	i;
    pointer	blockData;
    OSTimePtr	pTimeout;
    pointer	pReadmask;
{
    ScreenPtr		pScreen = screenInfo.screens[i];
    miSpriteScreenPtr	pPriv;

    pPriv = (miSpriteScreenPtr) pScreen->devPrivates[miSpriteScreenIndex].ptr;

    SCREEN_PROLOGUE(pScreen, BlockHandler);
    
    (*pScreen->BlockHandler) (i, blockData, pTimeout, pReadmask);

    SCREEN_EPILOGUE(pScreen, BlockHandler, miSpriteBlockHandler);

    if (!pPriv->isUp && pPriv->shouldBeUp)
	miSpriteRestoreCursor (pScreen);
}
#endif /* XTHREADS */

static void
miSpriteInstallColormap (pMap)
    ColormapPtr	pMap;
{
    ScreenPtr		pScreen = pMap->pScreen;
    miSpriteScreenPtr	pPriv;
#ifdef XTHREADS
    Bool		lock;
#endif /* XTHREADS */

    pPriv = (miSpriteScreenPtr) pScreen->devPrivates[miSpriteScreenIndex].ptr;

#ifndef XTHREADS
    SCREEN_PROLOGUE(pScreen, InstallColormap);
#endif /* XTHREADS */
    
#ifndef XTHREADS
    (*pScreen->InstallColormap)
#else /* XTHREADS */
    (*pPriv->InstallColormap)
#endif /* XTHREADS */
	(pMap);

#ifndef XTHREADS
    SCREEN_EPILOGUE(pScreen, InstallColormap, miSpriteInstallColormap);
#endif /* XTHREADS */

    pPriv->pInstalledMap = pMap;
    if (pPriv->pColormap != pMap)
    {
#ifdef XTHREADS
	lock = miPointerLock();
#endif /* XTHREADS */
    	pPriv->checkPixels = TRUE;
	if (pPriv->isUp)
	{
	    miSpriteRemoveCursor (pScreen);
#ifdef XTHREADS
	    if(lock)
		miPointerUnlock();
#endif /* XTHREADS */
	}
    }
}

static void
miSpriteStoreColors (pMap, ndef, pdef)
    ColormapPtr	pMap;
    int		ndef;
    xColorItem	*pdef;
{
    ScreenPtr		pScreen = pMap->pScreen;
    miSpriteScreenPtr	pPriv;
    int			i;
    int			updated;
    VisualPtr		pVisual;
#ifdef XTHREADS
    Bool		lock;
#endif /* XTHREADS */

    pPriv = (miSpriteScreenPtr) pScreen->devPrivates[miSpriteScreenIndex].ptr;

#ifndef XTHREADS
    SCREEN_PROLOGUE(pScreen, StoreColors);
    
    (*pScreen->StoreColors)
#else /* XTHREADS */
    (*pPriv->StoreColors)
    (pMap, ndef, pdef);
#endif /* XTHREADS */
	(pMap, ndef, pdef);

#ifndef XTHREADS
    SCREEN_EPILOGUE(pScreen, StoreColors, miSpriteStoreColors);
#endif /* XTHREADS */

    if (pPriv->pColormap == pMap)
    {
	updated = 0;
	pVisual = pMap->pVisual;
	if (pVisual->class == DirectColor)
	{
	    /* Direct color - match on any of the subfields */

#define MaskMatch(a,b,mask) ((a) & (pVisual->mask) == (b) & (pVisual->mask))

#define UpdateDAC(plane,dac,mask) {\
    if (MaskMatch (pPriv->colors[plane].pixel,pdef[i].pixel,mask)) {\
	pPriv->colors[plane].dac = pdef[i].dac; \
	updated = 1; \
    } \
}

#define CheckDirect(plane) \
	    UpdateDAC(plane,red,redMask) \
	    UpdateDAC(plane,green,greenMask) \
	    UpdateDAC(plane,blue,blueMask)

	    for (i = 0; i < ndef; i++)
	    {
		CheckDirect (SOURCE_COLOR)
		CheckDirect (MASK_COLOR)
	    }
	}
	else
	{
	    /* PseudoColor/GrayScale - match on exact pixel */
	    for (i = 0; i < ndef; i++)
	    {
	    	if (pdef[i].pixel == pPriv->colors[SOURCE_COLOR].pixel)
	    	{
		    pPriv->colors[SOURCE_COLOR] = pdef[i];
		    if (++updated == 2)
		    	break;
	    	}
	    	if (pdef[i].pixel == pPriv->colors[MASK_COLOR].pixel)
	    	{
		    pPriv->colors[MASK_COLOR] = pdef[i];
		    if (++updated == 2)
		    	break;
	    	}
	    }
	}
    	if (updated)
    	{
#ifdef XTHREADS
	    lock = miPointerLock();
#endif /* XTHREADS */
	    pPriv->checkPixels = TRUE;
	    if (pPriv->isUp)
	    {
	    	miSpriteRemoveCursor (pScreen);
	    }
#ifdef XTHREADS
	    if (lock)
		miPointerUnlock();
#endif /* XTHREADS */
    	}
    }
}

static void
miSpriteFindColors (pScreen)
    ScreenPtr	pScreen;
{
    miSpriteScreenPtr	pScreenPriv = (miSpriteScreenPtr)
			    pScreen->devPrivates[miSpriteScreenIndex].ptr;
    CursorPtr		pCursor;
    xColorItem		*sourceColor, *maskColor;

    pCursor = pScreenPriv->pCursor;
    sourceColor = &pScreenPriv->colors[SOURCE_COLOR];
    maskColor = &pScreenPriv->colors[MASK_COLOR];
    if (!(pCursor->foreRed == sourceColor->red &&
	  pCursor->foreGreen == sourceColor->green &&
          pCursor->foreBlue == sourceColor->blue &&
	  pCursor->backRed == maskColor->red &&
	  pCursor->backGreen == maskColor->green &&
	  pCursor->backBlue == maskColor->blue) ||
	 pScreenPriv->pColormap != pScreenPriv->pInstalledMap)
    {
	pScreenPriv->pColormap = pScreenPriv->pInstalledMap;
	sourceColor->red = pCursor->foreRed;
	sourceColor->green = pCursor->foreGreen;
	sourceColor->blue = pCursor->foreBlue;
	FakeAllocColor (pScreenPriv->pColormap, sourceColor);
	maskColor->red = pCursor->backRed;
	maskColor->green = pCursor->backGreen;
	maskColor->blue = pCursor->backBlue;
	FakeAllocColor (pScreenPriv->pColormap, maskColor);
	/* "free" the pixels right away, don't let this confuse you */
	FakeFreeColor(pScreenPriv->pColormap, sourceColor->pixel);
	FakeFreeColor(pScreenPriv->pColormap, maskColor->pixel);
    }
    pScreenPriv->checkPixels = FALSE;
}

/*
 * BackingStore wrappers
 */

static void
miSpriteSaveDoomedAreas (pWin, pObscured, dx, dy)
    WindowPtr	pWin;
    RegionPtr	pObscured;
    int		dx, dy;
{
    ScreenPtr		pScreen;
    miSpriteScreenPtr   pScreenPriv;
    BoxRec		cursorBox;
#ifdef XTHREADS
    Bool		drawCursor = FALSE;
#endif /* XTHREADS */

    pScreen = pWin->drawable.pScreen;
    
#ifndef XTHREADS
    SCREEN_PROLOGUE (pScreen, SaveDoomedAreas);
#endif /* XTHREADS */

    pScreenPriv = (miSpriteScreenPtr) pScreen->devPrivates[miSpriteScreenIndex].ptr;

#ifndef XTHREADS
    if (pScreenPriv->isUp)
#endif /* XTHREADS */
    {
	cursorBox = pScreenPriv->saved;

	if (dx || dy)
 	{
	    cursorBox.x1 += dx;
	    cursorBox.y1 += dy;
	    cursorBox.x2 += dx;
	    cursorBox.y2 += dy;
	}
	if (RECT_IN_REGION(pScreen, pObscured, &cursorBox) != rgnOUT)
	{
#ifdef XTHREADS
	    if (miPointerLock())
#endif /* XTHREADS */
	    {
		miSpriteRemoveCursor (pScreen);
#ifdef XTHREADS
		drawCursor = TRUE;
#endif /* XTHREADS */
	    }
	}
    }

#ifndef XTHREADS
    (*pScreen->SaveDoomedAreas)
	(pWin, pObscured, dx, dy);
#else /* XTHREADS */
    (*pScreenPriv->SaveDoomedAreas)
	(pWin, pObscured, dx, dy);
#endif /* XTHREADS */

#ifndef XTHREADS
    SCREEN_EPILOGUE (pScreen, SaveDoomedAreas, miSpriteSaveDoomedAreas);
#else /* XTHREADS */
    if (drawCursor)
    {
	miSpriteRestoreCursor (pScreen);
	miPointerUnlock();
    }
#endif /* XTHREADS */
}

static RegionPtr
miSpriteRestoreAreas (pWin, prgnExposed)
    WindowPtr	pWin;
    RegionPtr	prgnExposed;
{
    ScreenPtr		pScreen;
    miSpriteScreenPtr   pScreenPriv;
    RegionPtr		result;
#ifdef XTHREADS
    Bool		drawCursor = FALSE;
#endif /* XTHREADS */

    pScreen = pWin->drawable.pScreen;
    
#ifndef XTHREADS
    SCREEN_PROLOGUE (pScreen, RestoreAreas);
#endif /* XTHREADS */

    pScreenPriv = (miSpriteScreenPtr) pScreen->devPrivates[miSpriteScreenIndex].ptr;
#ifndef XTHREADS
    if (pScreenPriv->isUp)
#endif /* XTHREADS */
    {
	if (RECT_IN_REGION(pScreen, prgnExposed, &pScreenPriv->saved) != rgnOUT)
#ifdef XTHREADS
	if(miPointerLock())
	{
#endif /* XTHREADS */
	    miSpriteRemoveCursor (pScreen);
#ifdef XTHREADS
	    drawCursor = TRUE;
        }
#endif /* XTHREADS */
    }

    result =
#ifndef XTHREADS
	(*pScreen->RestoreAreas)
#else /* XTHREADS */
	(*pScreenPriv->RestoreAreas)
#endif /* XTHREADS */
	    (pWin, prgnExposed);

#ifndef XTHREADS
    SCREEN_EPILOGUE (pScreen, RestoreAreas, miSpriteRestoreAreas);
#else /* XTHREADS */
    if(drawCursor)
    {
	miSpriteRestoreCursor (pScreen);
	miPointerUnlock();
    }
#endif /* XTHREADS */
    return result;
}

/*
 * Window wrappers
 */

static void
miSpritePaintWindowBackground (pWin, pRegion, what)
    WindowPtr	pWin;
    RegionPtr	pRegion;
    int		what;
{
    ScreenPtr	    pScreen;
    miSpriteScreenPtr    pScreenPriv;
#ifdef XTHREADS
    Bool		drawCursor = FALSE;
#endif /* XTHREADS */

    pScreen = pWin->drawable.pScreen;

#ifndef XTHREADS
    SCREEN_PROLOGUE (pScreen, PaintWindowBackground);
#endif /* XTHREADS */

    pScreenPriv = (miSpriteScreenPtr) pScreen->devPrivates[miSpriteScreenIndex].ptr;
#ifndef XTHREADS
    if (pScreenPriv->isUp)
#endif /* XTHREADS */
    {
	/*
	 * If the cursor is on the same screen as the window, check the
	 * region to paint for the cursor and remove it as necessary
	 */
	if (RECT_IN_REGION(pScreen, pRegion, &pScreenPriv->saved) != rgnOUT)
#ifdef XTHREADS
	if(miPointerLock())
	{
#endif /* XTHREADS */
	    miSpriteRemoveCursor (pScreen);
#ifdef XTHREADS
	    drawCursor = TRUE;
        }
#endif /* XTHREADS */
    }

#ifndef XTHREADS
    (*pScreen->PaintWindowBackground)
	(pWin, pRegion, what);
#else /* XTHREADS */
    (*pScreenPriv->PaintWindowBackground)
	(pWin, pRegion, what);
#endif /* XTHREADS */

#ifndef XTHREADS
    SCREEN_EPILOGUE (pScreen, PaintWindowBackground, miSpritePaintWindowBackground);
#else /* XTHREADS */
    if (drawCursor)
    {
	miSpriteRestoreCursor (pScreen);
	miPointerUnlock();
    }
#endif /* XTHREADS */
}

static void
miSpritePaintWindowBorder (pWin, pRegion, what)
    WindowPtr	pWin;
    RegionPtr	pRegion;
    int		what;
{
    ScreenPtr	    pScreen;
    miSpriteScreenPtr    pScreenPriv;
#ifdef XTHREADS
    Bool		drawCursor = FALSE;
#endif /* XTHREADS */

    pScreen = pWin->drawable.pScreen;

#ifndef XTHREADS
    SCREEN_PROLOGUE (pScreen, PaintWindowBorder);
#endif /* XTHREADS */

    pScreenPriv = (miSpriteScreenPtr) pScreen->devPrivates[miSpriteScreenIndex].ptr;
#ifndef XTHREADS
    if (pScreenPriv->isUp)
#endif /* XTHREADS */
    {
	/*
	 * If the cursor is on the same screen as the window, check the
	 * region to paint for the cursor and remove it as necessary
	 */
	if (RECT_IN_REGION(pScreen, pRegion, &pScreenPriv->saved) != rgnOUT)
#ifdef XTHREADS
	if(miPointerLock())
	{
#endif /* XTHREADS */
	    miSpriteRemoveCursor (pScreen);
#ifdef XTHREADS
	    drawCursor = TRUE;
        }
#endif /* XTHREADS */
    }

#ifndef XTHREADS
    (*pScreen->PaintWindowBorder)
	(pWin, pRegion, what);
#else /* XTHREADS */
    (*pScreenPriv->PaintWindowBorder)
	(pWin, pRegion, what);
#endif /* XTHREADS */

#ifndef XTHREADS
    SCREEN_EPILOGUE (pScreen, PaintWindowBorder, miSpritePaintWindowBorder);
#else /* XTHREADS */
    if(drawCursor)
    {
	miSpriteRestoreCursor (pScreen);
	miPointerUnlock();
    }
#endif /* XTHREADS */
}

static void
miSpriteCopyWindow (pWin, ptOldOrg, pRegion)
    WindowPtr	pWin;
    DDXPointRec	ptOldOrg;
    RegionPtr	pRegion;
{
    ScreenPtr	    pScreen;
    miSpriteScreenPtr    pScreenPriv;
    BoxRec	    cursorBox;
    int		    dx, dy;
#ifdef XTHREADS
    Bool	    drawCursor = FALSE;
#endif /* XTHREADS */

    pScreen = pWin->drawable.pScreen;

#ifndef XTHREADS
    SCREEN_PROLOGUE (pScreen, CopyWindow);
#endif /* XTHREADS */

    pScreenPriv = (miSpriteScreenPtr) pScreen->devPrivates[miSpriteScreenIndex].ptr;
#ifndef XTHREADS
    if (pScreenPriv->isUp)
#endif /* XTHREADS */
    {
	/*
	 * check both the source and the destination areas.  The given
	 * region is source relative, so offset the cursor box by
	 * the delta position
	 */
	cursorBox = pScreenPriv->saved;
	dx = pWin->drawable.x - ptOldOrg.x;
	dy = pWin->drawable.y - ptOldOrg.y;
	cursorBox.x1 -= dx;
	cursorBox.x2 -= dx;
	cursorBox.y1 -= dy;
	cursorBox.y2 -= dy;
	if (RECT_IN_REGION(pScreen, pRegion, &pScreenPriv->saved) != rgnOUT ||
	    RECT_IN_REGION(pScreen, pRegion, &cursorBox) != rgnOUT)
#ifdef XTHREADS
	if(miPointerLock())
	{
#endif /* XTHREADS */
	    miSpriteRemoveCursor (pScreen);
#ifdef XTHREADS
	    drawCursor = TRUE;
        }
#endif /* XTHREADS */
    }

#ifndef XTHREADS
    (*pScreen->CopyWindow)
	(pWin, ptOldOrg, pRegion);
#else /* XTHREADS */
    (*pScreenPriv->CopyWindow)
	(pWin, ptOldOrg, pRegion);
#endif /* XTHREADS */

#ifndef XTHREADS
    SCREEN_EPILOGUE (pScreen, CopyWindow, miSpriteCopyWindow);
#else /* XTHREADS */
    if(drawCursor)
    {
	miSpriteRestoreCursor (pScreen);
	miPointerUnlock();
    }
#endif /* XTHREADS */
}

static void
miSpriteClearToBackground (pWin, x, y, w, h, generateExposures)
    WindowPtr pWin;
    short x,y;
    unsigned short w,h;
    Bool generateExposures;
{
    ScreenPtr		pScreen;
    miSpriteScreenPtr	pScreenPriv;
    int			realw, realh;
#ifdef XTHREADS
    Bool		drawCursor = FALSE;
#endif /* XTHREADS */

    pScreen = pWin->drawable.pScreen;

#ifndef XTHREADS
    SCREEN_PROLOGUE (pScreen, ClearToBackground);
#endif /* XTHREADS */

    pScreenPriv = (miSpriteScreenPtr) pScreen->devPrivates[miSpriteScreenIndex].ptr;
    if (GC_CHECK(pWin))
    {
	if (!(realw = w))
	    realw = (int) pWin->drawable.width - x;
	if (!(realh = h))
	    realh = (int) pWin->drawable.height - y;
	if (ORG_OVERLAP(&pScreenPriv->saved, pWin->drawable.x, pWin->drawable.y,
			x, y, realw, realh))
	{
#ifdef XTHREADS
	    if(miPointerLock())
	    {
#endif /* XTHREADS */
		miSpriteRemoveCursor (pScreen);
#ifdef XTHREADS
		drawCursor = TRUE;
            }
#endif /* XTHREADS */
	}
    }

#ifndef XTHREADS
    (*pScreen->ClearToBackground)
	(pWin, x, y, w, h, generateExposures);
#else /* XTHREADS */
    (*pScreenPriv->ClearToBackground)
	(pWin, x, y, w, h, generateExposures);
#endif /* XTHREADS */

#ifndef XTHREADS
    SCREEN_EPILOGUE (pScreen, ClearToBackground, miSpriteClearToBackground);
#else /* XTHREADS */
    if(drawCursor)
    {
	miSpriteRestoreCursor (pScreen);
	miPointerUnlock();
    }
#endif /* XTHREADS */
}

/*
 * GC Func wrappers
 */

static void
miSpriteValidateGC (pGC, changes, pDrawable)
    GCPtr	pGC;
    Mask	changes;
    DrawablePtr	pDrawable;
{
#ifdef XTHREADS
    GCOps          *oldOps  = pGC->ops;
#endif /* XTHREADS */
    GC_FUNC_PROLOGUE (pGC);

    (*pGC->funcs->ValidateGC) (pGC, changes, pDrawable);
    
    pGCPriv->wrapOps = NULL;
    if ((pDrawable->type == DRAWABLE_WINDOW)
#ifndef XTHREADS
    && (((WindowPtr) pDrawable)->viewable)
#endif /* XTHREADS */
	)
    {
#ifdef XTHREADS
	if (((WindowPtr) pDrawable)->viewable)
#endif /* XTHREADS */
	{
	    WindowPtr   pWin;
	    RegionPtr   pRegion;

	    pWin = (WindowPtr) pDrawable;
	    pRegion = &pWin->clipList;
	    if (pGC->subWindowMode == IncludeInferiors)
		pRegion = &pWin->borderClip;
	    if (REGION_NOTEMPTY(pDrawable->pScreen, pRegion))
		pGCPriv->wrapOps = pGC->ops;
        }
    }
#ifdef XTHREADS
    else if (pDrawable->type == DRAWABLE_PIXMAP)
	pGCPriv->wrapOps = pGC->ops;
#endif /* XTHREADS */

    GC_FUNC_EPILOGUE (pGC);
}

static void
miSpriteChangeGC (pGC, mask)
    GCPtr	    pGC;
    unsigned long   mask;
{
    GC_FUNC_PROLOGUE (pGC);

    (*pGC->funcs->ChangeGC) (pGC, mask);
    
    GC_FUNC_EPILOGUE (pGC);
}

static void
miSpriteCopyGC (pGCSrc, mask, pGCDst)
    GCPtr	    pGCSrc, pGCDst;
    unsigned long   mask;
{
    GC_FUNC_PROLOGUE (pGCDst);

    (*pGCDst->funcs->CopyGC) (pGCSrc, mask, pGCDst);
    
    GC_FUNC_EPILOGUE (pGCDst);
}

static void
miSpriteDestroyGC (pGC)
    GCPtr   pGC;
{
    GC_FUNC_PROLOGUE (pGC);

    (*pGC->funcs->DestroyGC) (pGC);
    
#ifndef XTHREADS
    GC_FUNC_EPILOGUE (pGC);
#endif /* XTHREADS */
}

static void
miSpriteChangeClip (pGC, type, pvalue, nrects)
    GCPtr   pGC;
    int		type;
    pointer	pvalue;
    int		nrects;
{
    GC_FUNC_PROLOGUE (pGC);

    (*pGC->funcs->ChangeClip) (pGC, type, pvalue, nrects);

    GC_FUNC_EPILOGUE (pGC);
}

static void
miSpriteCopyClip(pgcDst, pgcSrc)
    GCPtr pgcDst, pgcSrc;
{
    GC_FUNC_PROLOGUE (pgcDst);

    (* pgcDst->funcs->CopyClip)(pgcDst, pgcSrc);

    GC_FUNC_EPILOGUE (pgcDst);
}

static void
miSpriteDestroyClip(pGC)
    GCPtr	pGC;
{
    GC_FUNC_PROLOGUE (pGC);

    (* pGC->funcs->DestroyClip)(pGC);

    GC_FUNC_EPILOGUE (pGC);
}

/*
 * GC Op wrappers
 */

static void
miSpriteFillSpans(pDrawable, pGC, nInit, pptInit, pwidthInit, fSorted)
    DrawablePtr pDrawable;
    GCPtr	pGC;
    int		nInit;			/* number of spans to fill */
    DDXPointPtr pptInit;		/* pointer to list of start points */
    int		*pwidthInit;		/* pointer to list of n widths */
    int 	fSorted;
{
#ifdef XTHREADS
    Bool	drawCursor = FALSE;
#endif /* XTHREADS */
    GC_SETUP(pDrawable, pGC);

    if (GC_CHECK((WindowPtr) pDrawable))
    {
	register DDXPointPtr    pts;
	register int    	*widths;
	register int    	nPts;

	for (pts = pptInit, widths = pwidthInit, nPts = nInit;
	     nPts--;
	     pts++, widths++)
 	{
	    if (SPN_OVERLAP(&pScreenPriv->saved,pts->y,pts->x,*widths))
	    {
#ifdef XTHREADS
		if (miPointerLock())
		{
#endif /* XTHREADS */
		     miSpriteRemoveCursor (pDrawable->pScreen);
#ifdef XTHREADS
		     drawCursor = TRUE;
		}
#endif /* XTHREADS */
		break;
	    }
	}
    }

    GC_OP_PROLOGUE (pGC);

    (*pGC->ops->FillSpans) (pDrawable, pGC, nInit, pptInit, pwidthInit, fSorted);

#ifdef XTHREADS
    if(drawCursor)
    {
	miSpriteRestoreCursor (pDrawable->pScreen);
	miPointerUnlock();
    }
#endif /* XTHREADS */

    GC_OP_EPILOGUE (pGC);
}

static void
miSpriteSetSpans(pDrawable, pGC, psrc, ppt, pwidth, nspans, fSorted)
    DrawablePtr		pDrawable;
    GCPtr		pGC;
    char		*psrc;
    register DDXPointPtr ppt;
    int			*pwidth;
    int			nspans;
    int			fSorted;
{
#ifdef XTHREADS
    Bool	drawCursor = FALSE;
#endif /* XTHREADS */
    GC_SETUP(pDrawable, pGC);

    if (GC_CHECK((WindowPtr) pDrawable))
    {
	register DDXPointPtr    pts;
	register int    	*widths;
	register int    	nPts;

	for (pts = ppt, widths = pwidth, nPts = nspans;
	     nPts--;
	     pts++, widths++)
 	{
	    if (SPN_OVERLAP(&pScreenPriv->saved,pts->y,pts->x,*widths))
	    {
#ifdef XTHREADS
		if(miPointerLock())
#endif /* XTHREADS */
		{
		     miSpriteRemoveCursor (pDrawable->pScreen);
#ifdef XTHREADS
		     drawCursor = TRUE;
#endif /* XTHREADS */
		}
		break;
	    }
	}
    }

    GC_OP_PROLOGUE (pGC);

    (*pGC->ops->SetSpans) (pDrawable, pGC, psrc, ppt, pwidth, nspans, fSorted);

#ifdef XTHREADS
    if(drawCursor)
    {
	miSpriteRestoreCursor (pDrawable->pScreen);
	miPointerUnlock();
    }
#endif /* XTHREADS */

    GC_OP_EPILOGUE (pGC);
}

static void
miSpritePutImage(pDrawable, pGC, depth, x, y, w, h, leftPad, format, pBits)
    DrawablePtr	  pDrawable;
    GCPtr   	  pGC;
    int		  depth;
    int	    	  x;
    int	    	  y;
    int	    	  w;
    int	    	  h;
    int	    	  format;
    char    	  *pBits;
{
#ifdef XTHREADS
    Bool	drawCursor = FALSE;
#endif /* XTHREADS */
    GC_SETUP(pDrawable, pGC);

    if (GC_CHECK((WindowPtr) pDrawable))
    {
	if (ORG_OVERLAP(&pScreenPriv->saved,pDrawable->x,pDrawable->y,
			x,y,w,h))
 	{
#ifdef XTHREADS
	    if(miPointerLock())
	    {
#endif /* XTHREADS */
		miSpriteRemoveCursor (pDrawable->pScreen);
#ifdef XTHREADS
		drawCursor = TRUE;
	    }
#endif /* XTHREADS */
	}
    }

    GC_OP_PROLOGUE (pGC);

    (*pGC->ops->PutImage) (pDrawable, pGC, depth, x, y, w, h, leftPad, format, pBits);

#ifdef XTHREADS
    if(drawCursor)
    {
	miSpriteRestoreCursor (pDrawable->pScreen);
	miPointerUnlock();
    }
#endif /* XTHREADS */

    GC_OP_EPILOGUE (pGC);
}

static RegionPtr
miSpriteCopyArea (pSrc, pDst, pGC, srcx, srcy, w, h, dstx, dsty)
    DrawablePtr	  pSrc;
    DrawablePtr	  pDst;
    GCPtr   	  pGC;
    int	    	  srcx;
    int	    	  srcy;
    int	    	  w;
    int	    	  h;
    int	    	  dstx;
    int	    	  dsty;
{
    RegionPtr rgn;
#ifdef XTHREADS
    Bool      drawCursor = FALSE;
#endif /* XTHREADS */

    GC_SETUP(pDst, pGC);

#ifndef XTHREADS
    /* check destination/source overlap. */
    if (GC_CHECK((WindowPtr) pDst) &&
	 (ORG_OVERLAP(&pScreenPriv->saved,pDst->x,pDst->y,dstx,dsty,w,h) ||
	  ((pDst == pSrc) &&
	   ORG_OVERLAP(&pScreenPriv->saved,pSrc->x,pSrc->y,srcx,srcy,w,h))))
#else /* XTHREADS */
    if ((pDst->type == DRAWABLE_WINDOW && GC_CHECK((WindowPtr) pDst) &&
	 ORG_OVERLAP(&pScreenPriv->saved,pDst->x,pDst->y,dstx,dsty,w,h)) ||
	(pSrc->type == DRAWABLE_WINDOW &&
	 ORG_OVERLAP(&pScreenPriv->saved,pSrc->x,pSrc->y,srcx,srcy,w,h)))
#endif /* XTHREADS */
    {
#ifdef XTHREADS
	if(miPointerLock())
	{
#endif /* XTHREADS */
	    miSpriteRemoveCursor (pDst->pScreen);
#ifdef XTHREADS
	    drawCursor = TRUE;
	}
#endif /* XTHREADS */
    }
 
    GC_OP_PROLOGUE (pGC);

    rgn = (*pGC->ops->CopyArea) (pSrc, pDst, pGC, srcx, srcy, w, h,
				 dstx, dsty);

#ifdef XTHREADS
    if(drawCursor)
    {
	miSpriteRestoreCursor (pDst->pScreen);
	miPointerUnlock();
    }
#endif /* XTHREADS */

    GC_OP_EPILOGUE (pGC);

    return rgn;
}

static RegionPtr
miSpriteCopyPlane (pSrc, pDst, pGC, srcx, srcy, w, h, dstx, dsty, plane)
    DrawablePtr	  pSrc;
    DrawablePtr	  pDst;
    register GCPtr pGC;
    int     	  srcx,
		  srcy;
    int     	  w,
		  h;
    int     	  dstx,
		  dsty;
    unsigned long  plane;
{
    RegionPtr rgn;
#ifdef XTHREADS
    Bool	drawCursor = FALSE;
#endif /* XTHREADS */

    GC_SETUP(pDst, pGC);

    /*
     * check destination/source for overlap.
     */
#ifndef XTHREADS
    if (GC_CHECK((WindowPtr) pDst) &&
	(ORG_OVERLAP(&pScreenPriv->saved,pDst->x,pDst->y,dstx,dsty,w,h) ||
	 ((pDst == pSrc) &&
	  ORG_OVERLAP(&pScreenPriv->saved,pSrc->x,pSrc->y,srcx,srcy,w,h))))
#else /* XTHREADS */
    if ((pDst->type == DRAWABLE_WINDOW && GC_CHECK((WindowPtr) pDst) &&
	 ORG_OVERLAP(&pScreenPriv->saved,pDst->x,pDst->y,dstx,dsty,w,h)) ||
	(pSrc->type == DRAWABLE_WINDOW &&
	 ORG_OVERLAP(&pScreenPriv->saved,pSrc->x,pSrc->y,srcx,srcy,w,h)))
#endif /* XTHREADS */
    {
#ifdef XTHREADS
	if(miPointerLock())
	{
#endif /* XTHREADS */
	    miSpriteRemoveCursor (pDst->pScreen);
#ifdef XTHREADS
	    drawCursor = TRUE;
	}
#endif /* XTHREADS */
    }

    GC_OP_PROLOGUE (pGC);

    rgn = (*pGC->ops->CopyPlane) (pSrc, pDst, pGC, srcx, srcy, w, h,
				  dstx, dsty, plane);

#ifdef XTHREADS
    if(drawCursor)
    {
	miSpriteRestoreCursor (pDst->pScreen);
	miPointerUnlock();
    }
#endif /* XTHREADS */

    GC_OP_EPILOGUE (pGC);

    return rgn;
}

static void
miSpritePolyPoint (pDrawable, pGC, mode, npt, pptInit)
    DrawablePtr pDrawable;
    GCPtr	pGC;
    int		mode;		/* Origin or Previous */
    int		npt;
    xPoint 	*pptInit;
{
#ifdef XTHREADS
    Bool	drawCursor = FALSE;
#endif /* XTHREADS */
    xPoint	t;
    int		n;
    BoxRec	cursor;
    register xPoint *pts;

    GC_SETUP (pDrawable, pGC);

    if (npt && GC_CHECK((WindowPtr) pDrawable))
    {
	cursor.x1 = pScreenPriv->saved.x1 - pDrawable->x;
	cursor.y1 = pScreenPriv->saved.y1 - pDrawable->y;
	cursor.x2 = pScreenPriv->saved.x2 - pDrawable->x;
	cursor.y2 = pScreenPriv->saved.y2 - pDrawable->y;

	if (mode == CoordModePrevious)
	{
	    t.x = 0;
	    t.y = 0;
	    for (pts = pptInit, n = npt; n--; pts++)
	    {
		t.x += pts->x;
		t.y += pts->y;
		if (cursor.x1 <= t.x && t.x <= cursor.x2 &&
		    cursor.y1 <= t.y && t.y <= cursor.y2)
		{
#ifdef XTHREADS
		    if(miPointerLock())
		    {
#endif /* XTHREADS */
			miSpriteRemoveCursor (pDrawable->pScreen);
#ifdef XTHREADS
			drawCursor = TRUE;
		    }
#endif /* XTHREADS */
		    break;
		}
	    }
	}
	else
	{
	    for (pts = pptInit, n = npt; n--; pts++)
	    {
		if (cursor.x1 <= pts->x && pts->x <= cursor.x2 &&
		    cursor.y1 <= pts->y && pts->y <= cursor.y2)
		{
#ifdef XTHREADS
		    if(miPointerLock())
		    {
#endif /* XTHREADS */
			miSpriteRemoveCursor (pDrawable->pScreen);
#ifdef XTHREADS
			drawCursor = TRUE;
		    }
#endif /* XTHREADS */
		    break;
		}
	    }
	}
    }

    GC_OP_PROLOGUE (pGC);

    (*pGC->ops->PolyPoint) (pDrawable, pGC, mode, npt, pptInit);

#ifdef XTHREADS
    if(drawCursor)
    {
	miSpriteRestoreCursor (pDrawable->pScreen);
	miPointerUnlock();
    }

#endif /* XTHREADS */
    GC_OP_EPILOGUE (pGC);
}

static void
miSpritePolylines (pDrawable, pGC, mode, npt, pptInit)
    DrawablePtr	  pDrawable;
    GCPtr   	  pGC;
    int	    	  mode;
    int	    	  npt;
    DDXPointPtr	  pptInit;
{
#ifdef XTHREADS
    Bool drawCursor = FALSE;
#endif /* XTHREADS */
    BoxPtr  cursor;
    register DDXPointPtr pts;
    int	    n;
    int	    x, y, x1, y1, x2, y2;
    int	    lw;
    int	    extra;

    GC_SETUP (pDrawable, pGC);

    if (npt && GC_CHECK((WindowPtr) pDrawable))
    {
	cursor = &pScreenPriv->saved;
	lw = pGC->lineWidth;
	x = pptInit->x + pDrawable->x;
	y = pptInit->y + pDrawable->y;

	if (npt == 1)
	{
	    extra = lw >> 1;
	    if (LINE_OVERLAP(cursor, x, y, x, y, extra))
	    {
#ifdef XTHREADS
		if(miPointerLock())
		{
#endif /* XTHREADS */
		    miSpriteRemoveCursor (pDrawable->pScreen);
#ifdef XTHREADS
		    drawCursor = TRUE;
		}
#endif /* XTHREADS */
	    }
	}
	else
	{
	    extra = lw >> 1;
	    /*
	     * mitered joins can project quite a way from
	     * the line end; the 11 degree miter limit limits
	     * this extension to 10.43 * lw / 2, rounded up
	     * and converted to int yields 6 * lw
	     */
	    if (pGC->joinStyle == JoinMiter)
		extra = 6 * lw;
	    else if (pGC->capStyle == CapProjecting)
		extra = lw;
	    for (pts = pptInit + 1, n = npt - 1; n--; pts++)
	    {
		x1 = x;
		y1 = y;
		if (mode == CoordModeOrigin)
		{
		    x2 = pDrawable->x + pts->x;
		    y2 = pDrawable->y + pts->y;
		}
		else
		{
		    x2 = x + pts->x;
		    y2 = y + pts->y;
		}
		x = x2;
		y = y2;
		LINE_SORT(x1, y1, x2, y2);
		if (LINE_OVERLAP(cursor, x1, y1, x2, y2, extra))
		{
#ifdef XTHREADS
		    if(miPointerLock())
		    {
#endif /* XTHREADS */
		        miSpriteRemoveCursor (pDrawable->pScreen);
#ifdef XTHREADS
			drawCursor = TRUE;
		    }
#endif /* XTHREADS */
		    break;
		}
	    }
	}
    }
    GC_OP_PROLOGUE (pGC);

    (*pGC->ops->Polylines) (pDrawable, pGC, mode, npt, pptInit);

#ifdef XTHREADS
    if(drawCursor)
    {
	miSpriteRestoreCursor (pDrawable->pScreen);
	miPointerUnlock();
    }
#endif /* XTHREADS */

    GC_OP_EPILOGUE (pGC);
}

static void
miSpritePolySegment(pDrawable, pGC, nseg, pSegs)
    DrawablePtr pDrawable;
    GCPtr 	pGC;
    int		nseg;
    xSegment	*pSegs;
{
#ifdef XTHREADS
    Bool	drawCursor = FALSE;
#endif /* XTHREADS */
    int	    n;
    register xSegment *segs;
    BoxPtr  cursor;
    int	    x1, y1, x2, y2;
    int	    extra;

    GC_SETUP(pDrawable, pGC);

    if (nseg && GC_CHECK((WindowPtr) pDrawable))
    {
	cursor = &pScreenPriv->saved;
	extra = pGC->lineWidth >> 1;
	if (pGC->capStyle == CapProjecting)
	    extra = pGC->lineWidth;
	for (segs = pSegs, n = nseg; n--; segs++)
	{
	    x1 = segs->x1 + pDrawable->x;
	    y1 = segs->y1 + pDrawable->y;
	    x2 = segs->x2 + pDrawable->x;
	    y2 = segs->y2 + pDrawable->y;
	    LINE_SORT(x1, y1, x2, y2);
	    if (LINE_OVERLAP(cursor, x1, y1, x2, y2, extra))
	    {
#ifdef XTHREADS
		if(miPointerLock())
		{
#endif /* XTHREADS */
		    miSpriteRemoveCursor (pDrawable->pScreen);
#ifdef XTHREADS
		    drawCursor = TRUE;
		}
#endif /* XTHREADS */
		break;
	    }
	}
    }

    GC_OP_PROLOGUE (pGC);

    (*pGC->ops->PolySegment) (pDrawable, pGC, nseg, pSegs);

#ifdef XTHREADS
    if(drawCursor)
    {
	miSpriteRestoreCursor (pDrawable->pScreen);
	miPointerUnlock();
    }
#endif /* XTHREADS */

    GC_OP_EPILOGUE (pGC);
}

static void
miSpritePolyRectangle(pDrawable, pGC, nrects, pRects)
    DrawablePtr	pDrawable;
    GCPtr	pGC;
    int		nrects;
    xRectangle	*pRects;
{
#ifdef XTHREADS
    Bool	drawCursor = FALSE;
#endif /* XTHREADS */
    register xRectangle *rects;
    BoxPtr  cursor;
    int	    lw;
    int	    n;
    int     x1, y1, x2, y2;
    
    GC_SETUP (pDrawable, pGC);

    if (GC_CHECK((WindowPtr) pDrawable))
    {
	lw = pGC->lineWidth >> 1;
	cursor = &pScreenPriv->saved;
	for (rects = pRects, n = nrects; n--; rects++)
	{
	    x1 = rects->x + pDrawable->x;
	    y1 = rects->y + pDrawable->y;
	    x2 = x1 + (int)rects->width;
	    y2 = y1 + (int)rects->height;
	    if (LINE_OVERLAP(cursor, x1, y1, x2, y1, lw) ||
		LINE_OVERLAP(cursor, x2, y1, x2, y2, lw) ||
		LINE_OVERLAP(cursor, x1, y2, x2, y2, lw) ||
		LINE_OVERLAP(cursor, x1, y1, x1, y2, lw))
	    {
#ifdef XTHREADS
		if(miPointerLock())
		{
#endif /* XTHREADS */
		    miSpriteRemoveCursor (pDrawable->pScreen);
#ifdef XTHREADS
		    drawCursor = TRUE;
		}
#endif /* XTHREADS */
		break;
	    }
	}
    }

    GC_OP_PROLOGUE (pGC);

    (*pGC->ops->PolyRectangle) (pDrawable, pGC, nrects, pRects);

#ifdef XTHREADS
    if(drawCursor)
    {
	miSpriteRestoreCursor (pDrawable->pScreen);
	miPointerUnlock();
    }
#endif /* XTHREADS */

    GC_OP_EPILOGUE (pGC);
}

static void
miSpritePolyArc(pDrawable, pGC, narcs, parcs)
    DrawablePtr	pDrawable;
    register GCPtr	pGC;
    int		narcs;
    xArc	*parcs;
{
#ifdef XTHREADS
    Bool    drawCursor = FALSE;
#endif /* XTHREADS */
    BoxPtr  cursor;
    int	    lw;
    int	    n;
    register xArc *arcs;
    
    GC_SETUP (pDrawable, pGC);

    if (GC_CHECK((WindowPtr) pDrawable))
    {
	lw = pGC->lineWidth >> 1;
	cursor = &pScreenPriv->saved;
	for (arcs = parcs, n = narcs; n--; arcs++)
	{
	    if (ORG_OVERLAP (cursor, pDrawable->x, pDrawable->y,
			     arcs->x - lw, arcs->y - lw,
			     (int) arcs->width + pGC->lineWidth,
 			     (int) arcs->height + pGC->lineWidth))
	    {
#ifdef XTHREADS
		if(miPointerLock())
		{
#endif /* XTHREADS */
		    miSpriteRemoveCursor (pDrawable->pScreen);
#ifdef XTHREADS
		    drawCursor = TRUE;
		}
#endif /* XTHREADS */
		break;
	    }
	}
    }

    GC_OP_PROLOGUE (pGC);

    (*pGC->ops->PolyArc) (pDrawable, pGC, narcs, parcs);

#ifdef XTHREADS
    if(drawCursor)
    {
	miSpriteRestoreCursor (pDrawable->pScreen);
	miPointerUnlock();
    }
#endif /* XTHREADS */

    GC_OP_EPILOGUE (pGC);
}

static void
miSpriteFillPolygon(pDrawable, pGC, shape, mode, count, pPts)
    register DrawablePtr pDrawable;
    register GCPtr	pGC;
    int			shape, mode;
    int			count;
    DDXPointPtr		pPts;
{
#ifdef XTHREADS
    Bool drawCursor = FALSE;
#endif /* XTHREADS */
    int x, y, minx, miny, maxx, maxy;
    register DDXPointPtr pts;
    int n;

    GC_SETUP (pDrawable, pGC);

    if (count && GC_CHECK((WindowPtr) pDrawable))
    {
	x = pDrawable->x;
	y = pDrawable->y;
	pts = pPts;
	minx = maxx = pts->x;
	miny = maxy = pts->y;
	pts++;
	n = count - 1;

	if (mode == CoordModeOrigin)
	{
	    for (; n--; pts++)
	    {
		if (pts->x < minx)
		    minx = pts->x;
		else if (pts->x > maxx)
		    maxx = pts->x;
		if (pts->y < miny)
		    miny = pts->y;
		else if (pts->y > maxy)
		    maxy = pts->y;
	    }
	    minx += x;
	    miny += y;
	    maxx += x;
	    maxy += y;
	}
	else
	{
	    x += minx;
	    y += miny;
	    minx = maxx = x;
	    miny = maxy = y;
	    for (; n--; pts++)
	    {
		x += pts->x;
		y += pts->y;
		if (x < minx)
		    minx = x;
		else if (x > maxx)
		    maxx = x;
		if (y < miny)
		    miny = y;
		else if (y > maxy)
		    maxy = y;
	    }
	}
	if (BOX_OVERLAP(&pScreenPriv->saved,minx,miny,maxx,maxy))
#ifdef XTHREADS
	    if(miPointerLock())
	    {
#endif /* XTHREADS */
		miSpriteRemoveCursor (pDrawable->pScreen);
#ifdef XTHREADS
		drawCursor = TRUE;
	    }
#endif /* XTHREADS */
    }

    GC_OP_PROLOGUE (pGC);

    (*pGC->ops->FillPolygon) (pDrawable, pGC, shape, mode, count, pPts);

#ifdef XTHREADS
    if(drawCursor)
    {
	miSpriteRestoreCursor (pDrawable->pScreen);
	miPointerUnlock();
    }
#endif /* XTHREADS */

    GC_OP_EPILOGUE (pGC);
}

static void
miSpritePolyFillRect(pDrawable, pGC, nrectFill, prectInit)
    DrawablePtr pDrawable;
    GCPtr	pGC;
    int		nrectFill; 	/* number of rectangles to fill */
    xRectangle	*prectInit;  	/* Pointer to first rectangle to fill */
{
#ifdef XTHREADS
    Bool drawCursor = FALSE;
#endif /* XTHREADS */
    GC_SETUP(pDrawable, pGC);

    if (GC_CHECK((WindowPtr) pDrawable))
    {
	register int	    nRect;
	register xRectangle *pRect;
	register int	    xorg, yorg;

	xorg = pDrawable->x;
	yorg = pDrawable->y;

	for (nRect = nrectFill, pRect = prectInit; nRect--; pRect++) {
	    if (ORGRECT_OVERLAP(&pScreenPriv->saved,xorg,yorg,pRect)){
#ifdef XTHREADS
		if(miPointerLock())
		{
#endif /* XTHREADS */
		    miSpriteRemoveCursor (pDrawable->pScreen);
#ifdef XTHREADS
		    drawCursor = TRUE;
		}
#endif /* XTHREADS */
		break;
	    }
	}
    }

    GC_OP_PROLOGUE (pGC);

    (*pGC->ops->PolyFillRect) (pDrawable, pGC, nrectFill, prectInit);

#ifdef XTHREADS
    if(drawCursor)
    {
	miSpriteRestoreCursor (pDrawable->pScreen);
	miPointerUnlock();
    }
#endif /* XTHREADS */

    GC_OP_EPILOGUE (pGC);
}

static void
miSpritePolyFillArc(pDrawable, pGC, narcs, parcs)
    DrawablePtr	pDrawable;
    GCPtr	pGC;
    int		narcs;
    xArc	*parcs;
{
#ifdef XTHREADS
    Bool drawCursor = FALSE;
#endif /* XTHREADS */

    GC_SETUP(pDrawable, pGC);

    if (GC_CHECK((WindowPtr) pDrawable))
    {
	register int	n;
	BoxPtr		cursor;
	register xArc *arcs;

	cursor = &pScreenPriv->saved;

	for (arcs = parcs, n = narcs; n--; arcs++)
	{
	    if (ORG_OVERLAP(cursor, pDrawable->x, pDrawable->y,
			    arcs->x, arcs->y,
 			    (int) arcs->width, (int) arcs->height))
	    {
#ifdef XTHREADS
		if(miPointerLock())
		{
#endif /* XTHREADS */
		    miSpriteRemoveCursor (pDrawable->pScreen);
#ifdef XTHREADS
		    drawCursor = TRUE;
		}
#endif /* XTHREADS */
		break;
	    }
	}
    }

    GC_OP_PROLOGUE (pGC);

    (*pGC->ops->PolyFillArc) (pDrawable, pGC, narcs, parcs);

#ifdef XTHREADS
    if(drawCursor)
    {
	miSpriteRestoreCursor (pDrawable->pScreen);
	miPointerUnlock();
    }
#endif /* XTHREADS */

    GC_OP_EPILOGUE (pGC);
}

/*
 * general Poly/Image text function.  Extract glyph information,
 * compute bounding box and remove cursor if it is overlapped.
 */

static Bool
miSpriteTextOverlap (pDraw, font, x, y, n, charinfo, imageblt, w, cursorBox)
    DrawablePtr   pDraw;
    FontPtr	  font;
    int		  x, y;
    unsigned long n;
    CharInfoPtr   *charinfo;
    Bool	  imageblt;
    unsigned int  w;
    BoxPtr	  cursorBox;
{
    ExtentInfoRec extents;

    x += pDraw->x;
    y += pDraw->y;

    if (FONTMINBOUNDS(font,characterWidth) >= 0)
    {
	/* compute an approximate (but covering) bounding box */
	if (!imageblt || (charinfo[0]->metrics.leftSideBearing < 0))
	    extents.overallLeft = charinfo[0]->metrics.leftSideBearing;
	else
	    extents.overallLeft = 0;
	if (w)
	    extents.overallRight = w - charinfo[n-1]->metrics.characterWidth;
	else
	    extents.overallRight = FONTMAXBOUNDS(font,characterWidth)
				    * (n - 1);
	if (imageblt && (charinfo[n-1]->metrics.characterWidth >
			 charinfo[n-1]->metrics.rightSideBearing))
	    extents.overallRight += charinfo[n-1]->metrics.characterWidth;
	else
	    extents.overallRight += charinfo[n-1]->metrics.rightSideBearing;
	if (imageblt && FONTASCENT(font) > FONTMAXBOUNDS(font,ascent))
	    extents.overallAscent = FONTASCENT(font);
	else
	    extents.overallAscent = FONTMAXBOUNDS(font, ascent);
	if (imageblt && FONTDESCENT(font) > FONTMAXBOUNDS(font,descent))
	    extents.overallDescent = FONTDESCENT(font);
	else
	    extents.overallDescent = FONTMAXBOUNDS(font,descent);
	if (!BOX_OVERLAP(cursorBox,
			 x + extents.overallLeft,
			 y - extents.overallAscent,
			 x + extents.overallRight,
			 y + extents.overallDescent))
	    return FALSE;
	else if (imageblt && w)
	    return TRUE;
	/* if it does overlap, fall through and compute exactly, because
	 * taking down the cursor is expensive enough to make this worth it
	 */
    }
    QueryGlyphExtents(font, charinfo, n, &extents);
    if (imageblt)
    {
	if (extents.overallWidth > extents.overallRight)
	    extents.overallRight = extents.overallWidth;
	if (extents.overallWidth < extents.overallLeft)
	    extents.overallLeft = extents.overallWidth;
	if (extents.overallLeft > 0)
	    extents.overallLeft = 0;
	if (extents.fontAscent > extents.overallAscent)
	    extents.overallAscent = extents.fontAscent;
	if (extents.fontDescent > extents.overallDescent)
	    extents.overallDescent = extents.fontDescent;
    }
    return (BOX_OVERLAP(cursorBox,
			x + extents.overallLeft,
			y - extents.overallAscent,
			x + extents.overallRight,
			y + extents.overallDescent));
}

/*
 * values for textType:
 */
#define TT_POLY8   0
#define TT_IMAGE8  1
#define TT_POLY16  2
#define TT_IMAGE16 3

static int 
miSpriteText (pDraw, pGC, x, y, count, chars, fontEncoding, textType, cursorBox)
    DrawablePtr	    pDraw;
    GCPtr	    pGC;
    int		    x,
		    y;
    unsigned long    count;
    char	    *chars;
    FontEncoding    fontEncoding;
    Bool	    textType;
    BoxPtr	    cursorBox;
{
#ifdef XTHREADS
    Bool	drawCursor = FALSE;
#endif /* XTHREADS */
    CharInfoPtr *charinfo;
    register CharInfoPtr *info;
    unsigned long n, i;
    int		  w;
    void   	  (*drawFunc)();

    Bool imageblt;

    imageblt = (textType == TT_IMAGE8) || (textType == TT_IMAGE16);

    charinfo = (CharInfoPtr *) ALLOCATE_LOCAL(count * sizeof(CharInfoPtr));
    if (!charinfo)
	return x;

    GetGlyphs(pGC->font, count, (unsigned char *)chars,
	      fontEncoding, &n, charinfo);
    w = 0;
    if (!imageblt)
	for (info = charinfo, i = n; i--; info++)
	    w += (*info)->metrics.characterWidth;

    if (n != 0) {
	if (miSpriteTextOverlap(pDraw, pGC->font, x, y, n, charinfo, imageblt, w, cursorBox))
#ifdef XTHREADS
	    if(miPointerLock())
	    {
#endif /* XTHREADS */
		miSpriteRemoveCursor (pDraw->pScreen);
#ifdef XTHREADS
		drawCursor = TRUE;
	    }
#endif /* XTHREADS */

#ifdef AVOID_GLYPHBLT
	/*
	 * On displays like Apollos, which do not optimize the GlyphBlt functions because they
	 * convert fonts to their internal form in RealizeFont and optimize text directly, we
	 * want to invoke the text functions here, not the GlyphBlt functions.
	 */
	switch (textType)
	{
	case TT_POLY8:
	    drawFunc = (void (*)())pGC->ops->PolyText8;
	    break;
	case TT_IMAGE8:
	    drawFunc = pGC->ops->ImageText8;
	    break;
	case TT_POLY16:
	    drawFunc = (void (*)())pGC->ops->PolyText16;
	    break;
	case TT_IMAGE16:
	    drawFunc = pGC->ops->ImageText16;
	    break;
	}
	(*drawFunc) (pDraw, pGC, x, y, (int) count, chars);
#else /* don't AVOID_GLYPHBLT */
	/*
	 * On the other hand, if the device does use GlyphBlt ultimately to do text, we
	 * don't want to slow it down by invoking the text functions and having them call
	 * GetGlyphs all over again, so we go directly to the GlyphBlt functions here.
	 */
	drawFunc = imageblt ? pGC->ops->ImageGlyphBlt : pGC->ops->PolyGlyphBlt;
	(*drawFunc) (pDraw, pGC, x, y, n, charinfo, FONTGLYPHS(pGC->font));
#endif /* AVOID_GLYPHBLT */

#ifdef XTHREADS
	if(drawCursor)
	{
	    miSpriteRestoreCursor (pDraw->pScreen);
	    miPointerUnlock();
	}
#endif /* XTHREADS */
    }
    DEALLOCATE_LOCAL(charinfo);
    return x + w;
}

static int
miSpritePolyText8(pDrawable, pGC, x, y, count, chars)
    DrawablePtr pDrawable;
    GCPtr	pGC;
    int		x, y;
    int 	count;
    char	*chars;
{
    int	ret;

    GC_SETUP (pDrawable, pGC);

    GC_OP_PROLOGUE (pGC);

    if (GC_CHECK((WindowPtr) pDrawable))
	ret = miSpriteText (pDrawable, pGC, x, y, (unsigned long)count, chars,
			    Linear8Bit, TT_POLY8, &pScreenPriv->saved);
    else
	ret = (*pGC->ops->PolyText8) (pDrawable, pGC, x, y, count, chars);

    GC_OP_EPILOGUE (pGC);
    return ret;
}

static int
miSpritePolyText16(pDrawable, pGC, x, y, count, chars)
    DrawablePtr pDrawable;
    GCPtr	pGC;
    int		x, y;
    int		count;
    unsigned short *chars;
{
    int	ret;

    GC_SETUP(pDrawable, pGC);

    GC_OP_PROLOGUE (pGC);

    if (GC_CHECK((WindowPtr) pDrawable))
	ret = miSpriteText (pDrawable, pGC, x, y, (unsigned long)count,
			    (char *)chars,
			    FONTLASTROW(pGC->font) == 0 ?
			    Linear16Bit : TwoD16Bit, TT_POLY16, &pScreenPriv->saved);
    else
	ret = (*pGC->ops->PolyText16) (pDrawable, pGC, x, y, count, chars);

    GC_OP_EPILOGUE (pGC);
    return ret;
}

static void
miSpriteImageText8(pDrawable, pGC, x, y, count, chars)
    DrawablePtr pDrawable;
    GCPtr	pGC;
    int		x, y;
    int		count;
    char	*chars;
{
    GC_SETUP(pDrawable, pGC);

    GC_OP_PROLOGUE (pGC);

    if (GC_CHECK((WindowPtr) pDrawable))
	(void) miSpriteText (pDrawable, pGC, x, y, (unsigned long)count,
			     chars, Linear8Bit, TT_IMAGE8, &pScreenPriv->saved);
    else
	(*pGC->ops->ImageText8) (pDrawable, pGC, x, y, count, chars);

    GC_OP_EPILOGUE (pGC);
}

static void
miSpriteImageText16(pDrawable, pGC, x, y, count, chars)
    DrawablePtr pDrawable;
    GCPtr	pGC;
    int		x, y;
    int		count;
    unsigned short *chars;
{
    GC_SETUP(pDrawable, pGC);

    GC_OP_PROLOGUE (pGC);

    if (GC_CHECK((WindowPtr) pDrawable))
	(void) miSpriteText (pDrawable, pGC, x, y, (unsigned long)count,
			     (char *)chars,
			    FONTLASTROW(pGC->font) == 0 ?
			    Linear16Bit : TwoD16Bit, TT_IMAGE16, &pScreenPriv->saved);
    else
	(*pGC->ops->ImageText16) (pDrawable, pGC, x, y, count, chars);

    GC_OP_EPILOGUE (pGC);
}

static void
miSpriteImageGlyphBlt(pDrawable, pGC, x, y, nglyph, ppci, pglyphBase)
    DrawablePtr pDrawable;
    GCPtr 	pGC;
    int 	x, y;
    unsigned int nglyph;
    CharInfoPtr *ppci;		/* array of character info */
    pointer 	pglyphBase;	/* start of array of glyphs */
{
#ifdef XTHREADS
    Bool	drawCursor = FALSE;
#endif /* XTHREADS */
    GC_SETUP(pDrawable, pGC);

    GC_OP_PROLOGUE (pGC);

    if (GC_CHECK((WindowPtr) pDrawable) &&
	miSpriteTextOverlap (pDrawable, pGC->font, x, y, nglyph, ppci, TRUE, 0, &pScreenPriv->saved))
    {
#ifdef XTHREADS
	if(miPointerLock())
	{
#endif /* XTHREADS */
	    miSpriteRemoveCursor (pDrawable->pScreen);
#ifdef XTHREADS
	    drawCursor = TRUE;
	}
#endif /* XTHREADS */
    }

    (*pGC->ops->ImageGlyphBlt) (pDrawable, pGC, x, y, nglyph, ppci, pglyphBase);

#ifdef XTHREADS
    if(drawCursor)
    {
	miSpriteRestoreCursor (pDrawable->pScreen);
	miPointerUnlock();
    }
#endif /* XTHREADS */

    GC_OP_EPILOGUE (pGC);
}

static void
miSpritePolyGlyphBlt(pDrawable, pGC, x, y, nglyph, ppci, pglyphBase)
    DrawablePtr pDrawable;
    GCPtr	pGC;
    int 	x, y;
    unsigned int nglyph;
    CharInfoPtr *ppci;		/* array of character info */
    pointer	pglyphBase;	/* start of array of glyphs */
{
#ifdef XTHREADS
    Bool	drawCursor = FALSE;
#endif /* XTHREADS */
    GC_SETUP (pDrawable, pGC);

    GC_OP_PROLOGUE (pGC);

    if (GC_CHECK((WindowPtr) pDrawable) &&
	miSpriteTextOverlap (pDrawable, pGC->font, x, y, nglyph, ppci, FALSE, 0, &pScreenPriv->saved))
    {
#ifdef XTHREADS
	if(miPointerLock())
	{
#endif /* XTHREADS */
	    miSpriteRemoveCursor(pDrawable->pScreen);
#ifdef XTHREADS
	    drawCursor = TRUE;
	}
#endif /* XTHREADS */
    }

    (*pGC->ops->PolyGlyphBlt) (pDrawable, pGC, x, y, nglyph, ppci, pglyphBase);

#ifdef XTHREADS
    if(drawCursor)
    {
	miSpriteRestoreCursor (pDrawable->pScreen);
	miPointerUnlock();
    }
#endif /* XTHREADS */

    GC_OP_EPILOGUE (pGC);
}

static void
miSpritePushPixels(pGC, pBitMap, pDrawable, w, h, x, y)
    GCPtr	pGC;
    PixmapPtr	pBitMap;
    DrawablePtr pDrawable;
    int		w, h, x, y;
{
#ifdef XTHREADS
    Bool drawCursor = FALSE;
#endif /* XTHREADS */
    GC_SETUP(pDrawable, pGC);

    if (GC_CHECK((WindowPtr) pDrawable) &&
	ORG_OVERLAP(&pScreenPriv->saved,pDrawable->x,pDrawable->y,x,y,w,h))
    {
#ifdef XTHREADS
	if(miPointerLock())
	{
#endif /* XTHREADS */
	    miSpriteRemoveCursor (pDrawable->pScreen);
#ifdef XTHREADS
	    drawCursor = TRUE;
	}
#endif /* XTHREADS */
    }

    GC_OP_PROLOGUE (pGC);

    (*pGC->ops->PushPixels) (pGC, pBitMap, pDrawable, w, h, x, y);

#ifdef XTHREADS
    if(drawCursor)
    {
	miSpriteRestoreCursor (pDrawable->pScreen);
	miPointerUnlock();
    }
#endif /* XTHREADS */

    GC_OP_EPILOGUE (pGC);
}

#ifdef NEED_LINEHELPER
/*
 * I don't expect this routine will ever be called, as the GC
 * will have been unwrapped for the line drawing
 */

static void
miSpriteLineHelper()
{
    FatalError("miSpriteLineHelper called\n");
}
#endif

/*
 * miPointer interface routines
 */

#define SPRITE_PAD  8

#ifdef XTHREADS
void
miSpriteCalculateCursorRegion (pScreen, pRegion)
    ScreenPtr   pScreen;
    RegionPtr   pRegion;
{
    miSpriteScreenPtr pScreenPriv;
    CursorPtr pCursor;
    int x1, y1, x2, y2;
    int current_x, current_y;
    int pad;

    pScreenPriv = (miSpriteScreenPtr) 
			pScreen->devPrivates[miSpriteScreenIndex].ptr;
    pCursor = pScreenPriv->pCursor;

    /* 
     * Calculate the cursor lock region. 
     * This region should also include the cursor backing store,
     * which is why the padding is used in the calculation.
     */
    pad = SPRITE_PAD;
    x1 = -(pCursor->bits->xhot + pad);
    y1 = -(pCursor->bits->yhot + pad);
    x2 = x1 + pCursor->bits->width + (pad * 2);
    y2 = y1 + pCursor->bits->height + (pad * 2);

    /*
     * Get new location of the current cursor.
     */
    miPointerPosition ( &current_x, &current_y );

    if (pScreenPriv->x < current_x)
    {
        pRegion->extents.x1 = max (0, (pScreenPriv->x + x1));
        pRegion->extents.x2 = current_x + x2;
    }
    else
    {
        pRegion->extents.x1 = max (0, (current_x + x1));
        pRegion->extents.x2 = pScreenPriv->x + x2;
    }

    if (pScreenPriv->y < current_y)
    {
        pRegion->extents.y1 = max (0, (pScreenPriv->y + y1));
        pRegion->extents.y2 = current_y + y2;
    }
    else
    {
        pRegion->extents.y1 = max (0, (current_y  + y1));
        pRegion->extents.y2 = pScreenPriv->y + y2;
    }

    pRegion->data = NULL;
}
#endif /* XTHREADS */

static Bool
miSpriteRealizeCursor (pScreen, pCursor)
    ScreenPtr	pScreen;
    CursorPtr	pCursor;
{
    miSpriteScreenPtr	pScreenPriv;
#ifdef XTHREADS
    int		        my = pScreen->myNum;
#endif /* XTHREADS */

    pScreenPriv = (miSpriteScreenPtr) pScreen->devPrivates[miSpriteScreenIndex].ptr;
    if (pCursor == pScreenPriv->pCursor)
	pScreenPriv->checkPixels = TRUE;
    return (*pScreenPriv->funcs->RealizeCursor) (pScreen, pCursor);
}

static Bool
miSpriteUnrealizeCursor (pScreen, pCursor)
    ScreenPtr	pScreen;
    CursorPtr	pCursor;
{
    miSpriteScreenPtr	pScreenPriv;

    pScreenPriv = (miSpriteScreenPtr) pScreen->devPrivates[miSpriteScreenIndex].ptr;
    return (*pScreenPriv->funcs->UnrealizeCursor) (pScreen, pCursor);
}

static void
miSpriteSetCursor (pScreen, pCursor, x, y)
    ScreenPtr	pScreen;
    CursorPtr	pCursor;
{
    miSpriteScreenPtr	pScreenPriv;

    pScreenPriv = (miSpriteScreenPtr) pScreen->devPrivates[miSpriteScreenIndex].ptr;
#ifndef XTHREADS
    pScreenPriv->shouldBeUp = TRUE;
#endif /* XTHREADS */

    if (pScreenPriv->x == x &&
	pScreenPriv->y == y &&
	pScreenPriv->pCursor == pCursor &&
	!pScreenPriv->checkPixels)
    {
	return;
    }
    if (!pCursor)
    {
#ifndef XTHREADS
    	pScreenPriv->shouldBeUp = FALSE;
#endif /* XTHREADS */
    	if (pScreenPriv->isUp)
	    miSpriteRemoveCursor (pScreen);
	pScreenPriv->pCursor = 0;
	return;
    }
    pScreenPriv->x = x;
    pScreenPriv->y = y;
#ifndef XTHREADS
    pScreenPriv->pCacheWin = NullWindow;
#endif /* XTHREADS */
    if (pScreenPriv->checkPixels || pScreenPriv->pCursor != pCursor)
    {
	pScreenPriv->pCursor = pCursor;
	miSpriteFindColors (pScreen);
    }
    if (pScreenPriv->isUp) {
	int	sx, sy;
	/*
	 * check to see if the old saved region
	 * encloses the new sprite, in which case we use
	 * the flicker-free MoveCursor primitive.
	 */
	sx = pScreenPriv->x - (int)pCursor->bits->xhot;
	sy = pScreenPriv->y - (int)pCursor->bits->yhot;

#ifdef XTHREADS
#ifdef USE_MOVECURSOR
	if (sx + (int) pCursor->bits->width >= pScreenPriv->saved.x1 &&
	    sx < pScreenPriv->saved.x2 &&
	    sy + (int) pCursor->bits->height >= pScreenPriv->saved.y1 &&
	    sy < pScreenPriv->saved.y2 &&
	    (int) pCursor->bits->width + (2 * SPRITE_PAD) ==
		pScreenPriv->saved.x2 - pScreenPriv->saved.x1 &&
	    (int) pCursor->bits->height + (2 * SPRITE_PAD) ==
		pScreenPriv->saved.y2 - pScreenPriv->saved.y1
	    )
	{
	    pScreenPriv->isUp = FALSE;
	    if (!(sx >= pScreenPriv->saved.x1 &&
	      	  sx + (int)pCursor->bits->width < pScreenPriv->saved.x2 &&
	      	  sy >= pScreenPriv->saved.y1 &&
	      	  sy + (int)pCursor->bits->height < pScreenPriv->saved.y2))
	    {
		int oldx1, oldy1, dx, dy;

		oldx1 = pScreenPriv->saved.x1;
		oldy1 = pScreenPriv->saved.y1;
		dx = oldx1 - (sx - SPRITE_PAD);
		dy = oldy1 - (sy - SPRITE_PAD);
		pScreenPriv->saved.x1 -= dx;
		pScreenPriv->saved.y1 -= dy;
		pScreenPriv->saved.x2 -= dx;
		pScreenPriv->saved.y2 -= dy;
		(void) (*pScreenPriv->funcs->ChangeSave) (pScreen,
				pScreenPriv->saved.x1,
 				pScreenPriv->saved.y1,
				pScreenPriv->saved.x2 - pScreenPriv->saved.x1,
				pScreenPriv->saved.y2 - pScreenPriv->saved.y1,
				dx, dy);
	    }
	    (void) (*pScreenPriv->funcs->MoveCursor) (pScreen, pCursor,
				  pScreenPriv->saved.x1,
 				  pScreenPriv->saved.y1,
				  pScreenPriv->saved.x2 - pScreenPriv->saved.x1,
				  pScreenPriv->saved.y2 - pScreenPriv->saved.y1,
				  sx - pScreenPriv->saved.x1,
				  sy - pScreenPriv->saved.y1,
				  pScreenPriv->colors[SOURCE_COLOR].pixel,
				  pScreenPriv->colors[MASK_COLOR].pixel);
	    pScreenPriv->isUp = TRUE;
	}
	else
#endif
#endif /* XTHREADS */
	{
	    miSpriteRemoveCursor (pScreen);
	}
    }
    if (!pScreenPriv->isUp && pScreenPriv->pCursor)
	miSpriteRestoreCursor (pScreen);
}

static void
miSpriteMoveCursor (pScreen, x, y)
    ScreenPtr	pScreen;
    int		x, y;
{
    miSpriteScreenPtr	pScreenPriv;

    pScreenPriv = (miSpriteScreenPtr) pScreen->devPrivates[miSpriteScreenIndex].ptr;
    miSpriteSetCursor (pScreen, pScreenPriv->pCursor, x, y);
}

/*
 * undraw/draw cursor
 */

static void
miSpriteRemoveCursor (pScreen)
    ScreenPtr	pScreen;
{
    miSpriteScreenPtr   pScreenPriv;

    pScreenPriv = (miSpriteScreenPtr) pScreen->devPrivates[miSpriteScreenIndex].ptr;
    pScreenPriv->isUp = FALSE;
#ifndef XTHREADS
    pScreenPriv->pCacheWin = NullWindow;
#endif /* XTHREADS */
    if (!(*pScreenPriv->funcs->RestoreUnderCursor) (pScreen,
			 pScreenPriv->saved.x1,
			 pScreenPriv->saved.y1,
			 pScreenPriv->saved.x2 - pScreenPriv->saved.x1,
			 pScreenPriv->saved.y2 - pScreenPriv->saved.y1))
    {
	pScreenPriv->isUp = TRUE;
    }
}

/*
 * Called from the block handler, restores the cursor
 * before waiting for something to do.
 */

static void
miSpriteRestoreCursor (pScreen)
    ScreenPtr	pScreen;
{
    miSpriteScreenPtr   pScreenPriv;
    int			x, y;
    CursorPtr		pCursor;

    miSpriteComputeSaved (pScreen);
    pScreenPriv = (miSpriteScreenPtr) pScreen->devPrivates[miSpriteScreenIndex].ptr;
    pCursor = pScreenPriv->pCursor;
    x = pScreenPriv->x - (int)pCursor->bits->xhot;
    y = pScreenPriv->y - (int)pCursor->bits->yhot;
    if ((*pScreenPriv->funcs->SaveUnderCursor) (pScreen,
			      pScreenPriv->saved.x1,
			      pScreenPriv->saved.y1,
			      pScreenPriv->saved.x2 - pScreenPriv->saved.x1,
			      pScreenPriv->saved.y2 - pScreenPriv->saved.y1))
    {
	if (pScreenPriv->checkPixels)
	    miSpriteFindColors (pScreen);
	if ((*pScreenPriv->funcs->PutUpCursor) (pScreen, pCursor, x, y,
				  pScreenPriv->colors[SOURCE_COLOR].pixel,
				  pScreenPriv->colors[MASK_COLOR].pixel))
	    pScreenPriv->isUp = TRUE;
    }
}

/*
 * compute the desired area of the screen to save
 */

static void
miSpriteComputeSaved (pScreen)
    ScreenPtr	pScreen;
{
    miSpriteScreenPtr   pScreenPriv;
    int		    x, y, w, h;
    int		    wpad, hpad;
    CursorPtr	    pCursor;

    pScreenPriv = (miSpriteScreenPtr) pScreen->devPrivates[miSpriteScreenIndex].ptr;
    pCursor = pScreenPriv->pCursor;
    x = pScreenPriv->x - (int)pCursor->bits->xhot;
    y = pScreenPriv->y - (int)pCursor->bits->yhot;
    w = pCursor->bits->width;
    h = pCursor->bits->height;
    wpad = SPRITE_PAD;
    hpad = SPRITE_PAD;
    pScreenPriv->saved.x1 = x - wpad;
    pScreenPriv->saved.y1 = y - hpad;
    pScreenPriv->saved.x2 = pScreenPriv->saved.x1 + w + wpad * 2;
    pScreenPriv->saved.y2 = pScreenPriv->saved.y1 + h + hpad * 2;
}
