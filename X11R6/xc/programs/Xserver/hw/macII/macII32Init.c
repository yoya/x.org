/*****************************************************************************
Copyright 1993 by Apple Computer, Inc, Cupertino, California
			All Rights Reserved

Permission to use, copy, modify, and distribute this software
for any purpose and without fee is hereby granted, provided
that the above copyright notice appear in all copies.

APPLE MAKES NO WARRANTY OR REPRESENTATION, EITHER EXPRESS,
OR IMPLIED, WITH RESPECT TO THIS SOFTWARE, ITS QUALITY,
PERFORMANCE, MERCHANABILITY, OR FITNESS FOR A PARTICULAR
PURPOSE. AS A RESULT, THIS SOFTWARE IS PROVIDED "AS IS,"
AND YOU THE USER ARE ASSUMING THE ENTIRE RISK AS TO ITS
QUALITY AND PERFORMANCE. IN NO EVENT WILL APPLE BE LIABLE 
FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
DAMAGES RESULTING FROM ANY DEFECT IN THE SOFTWARE.

THE WARRANTY AND REMEDIES SET FORTH ABOVE ARE EXCLUSIVE
AND IN LIEU OF ALL OTHERS, ORAL OR WRITTEN, EXPRESS OR
IMPLIED.

/************************************************************
Copyright 1987 by Sun Microsystems, Inc. Mountain View, CA.

                    All Rights Reserved

Permission  to  use,  copy,  modify,  and  distribute   this
software  and  its documentation for any purpose and without
fee is hereby granted, provided that the above copyright no-
tice  appear  in all copies and that both that copyright no-
tice and this permission notice appear in  supporting  docu-
mentation,  and  that the names of Sun or X Consortium
not be used in advertising or publicity pertaining to 
distribution  of  the software  without specific prior 
written permission. Sun and X Consortium make no 
representations about the suitability of this software for 
any purpose. It is provided "as is" without any express or 
implied warranty.

SUN DISCLAIMS ALL WARRANTIES WITH REGARD TO  THIS  SOFTWARE,
INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FIT-
NESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL SUN BE  LI-
ABLE  FOR  ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,  DATA  OR
PROFITS,  WHETHER  IN  AN  ACTION OF CONTRACT, NEGLIGENCE OR
OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION  WITH
THE USE OR PERFORMANCE OF THIS SOFTWARE.

********************************************************/

#include "X.h"
#include "Xproto.h"
#include "scrnintstr.h"
#include "colormapst.h"

#define PSZ 32
#include "../../cfb/cfb.h"
#include "mibstore.h"

extern Bool cfbCloseScreen();
extern miBSFuncRec cfbBSFuncRec;
extern Bool cfbCreateScreenResources();

Bool
macII24ScreenInit(pScreen, pbits, xsize, ysize, dpix, dpiy, width)
    register ScreenPtr pScreen;
    pointer pbits;              /* pointer to screen bitmap */
    int xsize, ysize;           /* in pixels */
    int dpix, dpiy;             /* dots per inch */
    int width;                  /* pixel width of frame buffer */
{
    if (!cfbSetupScreen(pScreen, pbits, xsize, ysize, dpix, dpiy, width))
        return FALSE;
    {
	int i, j;
	pointer oldDevPrivate;
	VisualPtr   visuals;
	DepthPtr    depths;
	int         nvisuals;
	int         ndepths;
	int         rootdepth;
	VisualID    defaultVisual;

	rootdepth = 0;
	if (!cfbInitVisuals (&visuals, &depths, &nvisuals, &ndepths, &rootdepth, &defaultVisual, 1<<31, 8))
	    return FALSE;
	for (i = 0; i < nvisuals; i++) {
#ifdef SEPARATE_24
	    if (visuals[i].nplanes == 24)
#else
	    if (visuals[i].nplanes >= 24)
#endif
	    {
		visuals[i].redMask = 0xff0000;
		visuals[i].greenMask = 0xff00;
		visuals[i].blueMask = 0xff;
		visuals[i].offsetRed = 16;
		visuals[i].offsetGreen = 8;
		visuals[i].offsetBlue = 0;
		visuals[i].nplanes = 24;
	    }
	}
	oldDevPrivate = pScreen->devPrivate;
	if (! miScreenInit(pScreen, pbits, xsize, ysize, dpix, dpiy, width,
			    rootdepth, ndepths, depths,
			    defaultVisual, nvisuals, visuals,
			    (miBSFuncPtr) 0))
	    return FALSE;
	/* overwrite miCloseScreen with our own */
	pScreen->CloseScreen = cfbCloseScreen;
	/* init backing store here so we can overwrite CloseScreen without stepping
	 * on the backing store wrapped version */
	miInitializeBackingStore (pScreen, &cfbBSFuncRec);
	pScreen->CreateScreenResources = cfbCreateScreenResources;
	pScreen->devPrivates[cfbScreenPrivateIndex].ptr = pScreen->devPrivate;
	pScreen->devPrivate = oldDevPrivate;
	return TRUE;
    }
}

Bool
macII32ScreenInit(pScreen, pbits, xsize, ysize, dpix, dpiy, width)
    register ScreenPtr pScreen;
    pointer pbits;              /* pointer to screen bitmap */
    int xsize, ysize;           /* in pixels */
    int dpix, dpiy;             /* dots per inch */
    int width;                  /* pixel width of frame buffer */
{
    if (!cfbSetupScreen(pScreen, pbits, xsize, ysize, dpix, dpiy, width))
        return FALSE;
    {
	int i, j;
	pointer oldDevPrivate;
	VisualPtr   visuals;
	DepthPtr    depths;
	int         nvisuals;
	int         ndepths;
	int         rootdepth;
	VisualID    defaultVisual;

	rootdepth = 0;
	if (!cfbInitVisuals (&visuals, &depths, &nvisuals, &ndepths, &rootdepth, &defaultVisual, 1<<31, 8)) /* hard code 32 bits-1 for sizes in InitVisuals */
	    return FALSE;
	for (i = 0; i < nvisuals; i++) {
	    if (visuals[i].nplanes == 32 && visuals[i].class == TrueColor) {
		visuals[i].redMask = 0xffe00000;
		visuals[i].greenMask = 0x1ffc00;
		visuals[i].blueMask = 0x3ff;
		visuals[i].offsetRed = 21;
		visuals[i].offsetGreen = 10;
		visuals[i].offsetBlue = 0;
	    }
	}
	oldDevPrivate = pScreen->devPrivate;
	if (! miScreenInit(pScreen, pbits, xsize, ysize, dpix, dpiy, width,
			    rootdepth, ndepths, depths,
			    defaultVisual, nvisuals, visuals,
			    (miBSFuncPtr) 0))
	    return FALSE;
	/* overwrite miCloseScreen with our own */
	pScreen->CloseScreen = cfbCloseScreen;
	/* init backing store here so we can overwrite CloseScreen without stepping
	 * on the backing store wrapped version */
	miInitializeBackingStore (pScreen, &cfbBSFuncRec);
	pScreen->CreateScreenResources = cfbCreateScreenResources;
	pScreen->devPrivates[cfbScreenPrivateIndex].ptr = pScreen->devPrivate;
	pScreen->devPrivate = oldDevPrivate;
	return TRUE;
    }
}
