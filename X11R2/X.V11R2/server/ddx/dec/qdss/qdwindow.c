/***********************************************************
Copyright 1987, 1988 by Digital Equipment Corporation, Maynard, Massachusetts,
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
#include "windowstr.h"
#include "qd.h"
#include "gcstruct.h"
#include "scrnintstr.h"
#include "regionstr.h"
#include "mi.h"

extern WindowRec WindowTable[];

Bool
qdDestroyWindow(pWin)
WindowPtr pWin;
{
}

Bool
qdChangeWindowAttributes()
{
}

Bool
qdMapWindow(pWindow)
WindowPtr pWindow;
{
}

Bool
qdUnmapWindow(pWindow, x, y)
WindowPtr pWindow;
int x, y;
{
}

Bool
qdPositionWindow(pWindow)
WindowPtr pWindow;
{
}

extern	int	Nplanes;

/*
 * DDX CopyWindow is required to translate prgnSrc by
 * pWin->absCorner - ptOldOrg .
 *
 * This change appears to have been made by MIT.  -dwm
 */
void
qdCopyWindow(pWin, ptOldOrg, prgnSrc)
    WindowPtr pWin;
    DDXPointRec ptOldOrg;
    RegionPtr prgnSrc;
{
    GCPtr pcopyGC;
    GC cgc;		/* helps in changing pcopyGC */
    WindowPtr pwinRoot;
    ScreenPtr pScreen;
    int               dx, dy;
 
    dx = ptOldOrg.x - pWin->absCorner.x;
    dy = ptOldOrg.y - pWin->absCorner.y;
    (* pWin->drawable.pScreen->TranslateRegion)(prgnSrc, -dx, -dy);
    pScreen = pWin->drawable.pScreen;
    pwinRoot = &WindowTable[pScreen->myNum];

    pcopyGC = GetScratchGC( Nplanes, pScreen);
    cgc.alu = GXcopy;
    cgc.subWindowMode = IncludeInferiors;
    cgc.stateChanges = GCFunction|GCSubwindowMode;
    QDChangeGCHelper( pcopyGC, &cgc);
    /*
     * assume planemask and client clip are reasonable
     */
    ValidateGC( pWin, pcopyGC);
    qdCopyArea(
		pwinRoot, pwinRoot, pcopyGC,
                ptOldOrg.x, ptOldOrg.y,
		pWin->clientWinSize.width + (pWin->borderWidth<<1),
		pWin->clientWinSize.height + (pWin->borderWidth<<1),
		pWin->absCorner.x, pWin->absCorner.y);

    FreeScratchGC( pcopyGC);
}

void
qdSaveDoomedAreas(pWin)
WindowPtr pWin;
{
}

Bool
qdCreateWindow(pWin)
WindowPtr pWin;
{
    pWin->ClearToBackground = miClearToBackground;
    pWin->PaintWindowBackground = miPaintWindow;
    pWin->PaintWindowBorder = miPaintWindow;
    pWin->CopyWindow = qdCopyWindow;
    pWin->backStorage = (BackingStorePtr)NULL;
    pWin->backingStore = NotUseful;
    pWin->devBackingStore = (pointer)NULL/*miInitBackingStore(pWin)*/;
    return TRUE;
}
