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
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/graphics.h>

#include "X.h"
#define  NEED_EVENTS
#include "Xproto.h"
#include "scrnintstr.h"
#include "cursorstr.h"
#include "pixmapstr.h"
#include "inputstr.h"
#include "regionstr.h"

#include "../cfb/cfb.h"
#include "mi.h"
#include "gbx.h"
#include "gcstruct.h"
#include "sun.h"
#include    "colormap.h"
#include    "colormapst.h"
#include    "resource.h"

extern int	lastEventTime;

static GbxDisplay	*gatorbox[MAXSCREENS];

static int		fdgbx[MAXSCREENS]  = -1;

/* XXX - next line means only one gatorbox - fix this */
static ColormapPtr gbxInstalledMap;

extern int TellLostMap(), TellGainedMap();

static void
gbxUpdateColormap(index, count, rmap, gmap, bmap)
    register int		index, count;
    register u_char	*rmap, *gmap, *bmap;
{
  register GbxDisplay *gbx = gatorbox[0];  /* XXX - We need a screen pointer */
  while (count--)
    {
      /* Wait for colormap to be not busy */
      while (((u_char *)gbx)[0x6803] & 0x04);

      ((u_char *)gbx)[0x68b9] = index;
      ((u_char *)gbx)[0x69b3] = rmap[index];
      ((u_char *)gbx)[0x69b5] = gmap[index];
      ((u_char *)gbx)[0x69b7] = bmap[index];
      ((u_char *)gbx)[0x68f1] = 0xff;
      index++;
    }
}

static Bool
gbxSaveScreen(pScreen, on)
    ScreenPtr pScreen;
    Bool on;
{
    int myNum = pScreen->myNum;
    if (on != SCREEN_SAVER_ON)
    {
	lastEventTime = GetTimeInMillis();
	/* Turn on Video */
	gatorbox[myNum] -> sec_interrupt = 0x01;
    }
    else
    {
	/* Turn off Video */
	gatorbox[myNum] -> sec_interrupt = 0x00;
    }
    
    return TRUE;
}

/* The following data is cribbed from Starbase:_hp_gbox_setup_6845 via adb */

#define CRTC_DATA_LENGTH  0x0e
static unsigned char crtc_init_data[CRTC_DATA_LENGTH] = {
    0x29, 0x20, 0x23, 0x04, 0x30, 0x0b, 0x30, 0x30,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00};

Bool
gbxScreenInfo(index, argv, argc)
    int index;
    char **argv;
    int argc;
{
    unsigned char *FrameBufferBase();
    int type;
    
    if ( (fdgbx[index] = open(argv[2], O_RDWR)) <  0)
    {
        perror(argv[0]);
        ErrorF("%s: couldn't open %s \n", argv[0], argv[2]);
        return FALSE;
    }
    if (ioctl(fdgbx[index], GCON, &type) < 0 ||
        ioctl(fdgbx[index], GCID, &type) < 0 )
    {
        ErrorF("%s: couldn't GCON and GCID %s \n", argv[0], argv[2]);
        return FALSE; 
    }

    if (type != 8 ) /* Not a Gatorbox */
    {
	ErrorF("%s: device %s not this kind of display.\n", argv[0], argv[2]);
	return FALSE;
    }

    /*
     * Map the gatorbox in to our address space.  We could ask the O.S.
     * to map it in where it prefers, but this would limit the amount of
     * data we can malloc() at a later time.
     */

    gatorbox[index] = (GbxDisplay *) FrameBufferBase(0x100000);

    if (ioctl(fdgbx[index], GCMAP, &gatorbox[index]) < 0)
    {
	perror("GCMAP:");
        ErrorF("%s: Error getting address of %s\n", argv[0], argv[1]);
        close(fdgbx[index]);
        return FALSE;
    }

    return TRUE;
}

/*-
 *-----------------------------------------------------------------------
 * gbxInsthallColormap --
 *	Install given colormap.
 *
 * Results:
 *	None
 *
 * Side Effects:
 *	All clients requesting ColormapNotify are notified
 *
 *-----------------------------------------------------------------------
 */
static void
gbxInstallColormap(cmap)
    ColormapPtr	cmap;
{
    register int i;
    Entry *pent = cmap->red;
    u_char	  rmap[256], gmap[256], bmap[256];

    if (cmap == gbxInstalledMap)
	return;
    if (gbxInstalledMap)
	WalkTree(gbxInstalledMap->pScreen, TellLostMap,
		 (char *) &(gbxInstalledMap->mid));
    for (i = 0; i < cmap->pVisual->ColormapEntries; i++) {
	if (pent->fShared) {
	    rmap[i] = pent->co.shco.red->color >> 8;
	    gmap[i] = pent->co.shco.green->color >> 8;
	    bmap[i] = pent->co.shco.blue->color >> 8;
	}
	else {
	    rmap[i] = pent->co.local.red >> 8;
	    gmap[i] = pent->co.local.green >> 8;
	    bmap[i] = pent->co.local.blue >> 8;
	}
	pent++;
    }
    gbxInstalledMap = cmap;
    gbxUpdateColormap(0, 256, rmap, gmap, bmap);
    WalkTree(cmap->pScreen, TellGainedMap, (char *) &(cmap->mid));
}

/*-
 *-----------------------------------------------------------------------
 * gbxUninstallColormap --
 *	Uninstall given colormap.
 *
 * Results:
 *	None
 *
 * Side Effects:
 *	All clients requesting ColormapNotify are notified
 *
 *-----------------------------------------------------------------------
 */
static void
gbxUninstallColormap(cmap)
    ColormapPtr	cmap;
{
    if (cmap == gbxInstalledMap) {
	Colormap defMapID = cmap->pScreen->defColormap;

	if (cmap->mid != defMapID) {
	    ColormapPtr defMap = (ColormapPtr) LookupID(defMapID, RT_COLORMAP, RC_CORE);

	    if (defMap)
		gbxInstallColormap(defMap);
	    else
	        ErrorF("gbx: Can't find default colormap\n");
	}
    }
}

/*-
 *-----------------------------------------------------------------------
 * gbxListInstalledColormaps --
 *	Fills in the list with the IDs of the installed maps
 *
 * Results:
 *	Returns the number of IDs in the list
 *
 * Side Effects:
 *	None
 *
 *-----------------------------------------------------------------------
 */
static int
gbxListInstalledColormaps(pScreen, pCmapList)
    ScreenPtr	pScreen;
    Colormap	*pCmapList;
{
    *pCmapList = gbxInstalledMap->mid;
    return (1);
}


/*-
 *-----------------------------------------------------------------------
 * gbxStoreColors --
 *	Sets the pixels in pdefs into the specified map.
 *
 * Results:
 *	None
 *
 * Side Effects:
 *	None
 *
 *-----------------------------------------------------------------------
 */
static void
gbxStoreColors(pmap, ndef, pdefs)
    ColormapPtr	pmap;
    int		ndef;
    xColorItem	*pdefs;
{
    switch (pmap->class) {
    case PseudoColor:
	if (pmap == gbxInstalledMap) {
	    /* We only have a single colormap */
	    u_char	rmap[256], gmap[256], bmap[256];

	    while (ndef--) {
		register unsigned index = pdefs->pixel&0xff;

		rmap[index] = (pdefs->red) >> 8;
		gmap[index] = (pdefs->green) >> 8;
		bmap[index] = (pdefs->blue) >> 8;
	 	gbxUpdateColormap(index, 1, rmap, gmap, bmap);
		pdefs++;
	    }
	}
	break;
    case DirectColor:
    default:
	ErrorF("gbxStoreColors: bad class %d\n", pmap->class);
	break;
    }
}

/*-
 *-----------------------------------------------------------------------
 * gbxResolvePseudoColor --
 *	Adjust specified RGB values to closest values hardware can do.
 *
 * Results:
 *	Args are modified.
 *
 * Side Effects:
 *	None
 *
 *-----------------------------------------------------------------------
 */
static void
gbxResolvePseudoColor(pRed, pGreen, pBlue, pVisual)
    CARD16	*pRed, *pGreen, *pBlue;
    VisualPtr	pVisual;
{
    *pRed &= 0xff00;
    *pGreen &= 0xff00;
    *pBlue &= 0xff00;
}

Bool
gbxScreenInit(index, pScreen, argc, argv)
    int index;
    ScreenPtr pScreen;
    int argc;		/* these two may NOT be changed */
    char **argv;
{
    CARD16	zero = 0, ones = ~0;
    int i;
    GbxDisplay *gp_hardware = gatorbox[pScreen->myNum];

    gp_hardware -> interrupts    = 4;		/** fb_enable ? **/
    gp_hardware -> rep_rule      = GXcopy;
    gp_hardware -> blink1        = 0xff;
    gp_hardware -> blink2        = 0xff;

    /* program 6845 */
    {
	int i;
	for ( i = 0; i < CRTC_DATA_LENGTH; i++) {
	    gp_hardware -> crtc_address = i;
	    gp_hardware -> crtc_data = crtc_init_data[i];
	    }
	}

    gp_hardware -> write_protect = 0;	/* Enable all planes */

    gp_hardware -> sec_interrupt = 0x01; /* Enable video */

    cfbScreenInit(index, pScreen,
	gatorbox[pScreen->myNum]->frame_buffer,
	GBX_WIDTH, GBX_HEIGHT, 80);

    pScreen->SaveScreen = gbxSaveScreen;
    pScreen->RealizeCursor = sunRealizeCursor;
    pScreen->UnrealizeCursor = sunUnrealizeCursor;
    pScreen->DisplayCursor = sunDisplayCursor;
    pScreen->SetCursorPosition = sunSetCursorPosition;
    pScreen->CursorLimits = sunCursorLimits;
    pScreen->PointerNonInterestBox = sunPointerNonInterestBox;
    pScreen->ConstrainCursor = sunConstrainCursor;
    pScreen->RecolorCursor = sunRecolorCursor;

#ifndef	STATIC_COLOR
    pScreen->InstallColormap = gbxInstallColormap;
    pScreen->UninstallColormap = gbxUninstallColormap;
    pScreen->ListInstalledColormaps = gbxListInstalledColormaps;
    pScreen->StoreColors = gbxStoreColors;
    pScreen->ResolveColor = gbxResolvePseudoColor;
#endif

    {
	ColormapPtr cmap = (ColormapPtr)LookupID(pScreen->defColormap, RT_COLORMAP, RC_CORE);

	if (!cmap)
	    FatalError("Can't find default colormap\n");
	gbxInstalledMap = (ColormapPtr)NULL; /* prevent reinitialization errors */
	gbxInstallColormap(cmap);
	if (AllocColor(cmap, &ones, &ones, &ones, &(pScreen->whitePixel), 0)
	    || AllocColor(cmap, &zero, &zero, &zero, &(pScreen->blackPixel), 0))
		FatalError("Can't alloc black & white pixels in cfbScreeninit\n");
    }

    gbxSaveScreen( pScreen, SCREEN_SAVER_FORCER );
    sunScreenInit (pScreen);
    return (TRUE);	/* XXX- this will be ignored */
}

Bool
gbxScreenClose(index, pScreen)
    int index;
    ScreenPtr pScreen;
{
  return hpCloseScreen(index, pScreen);
}
