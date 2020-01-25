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
/* #include <sys/file.h> */

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
#include "ren.h"
#include "gcstruct.h"
#include "windowstr.h"
#include "sun.h"
#include    "colormap.h"
#include    "colormapst.h"
#include    "resource.h"

extern int	lastEventTime;
extern unsigned char *FrameBufferBase();

/*
  maintain a local pointer to the screen->devPrivate structure. When the
  server restarts, this will be used to restore the screen pointer lost
  when the screen structure is reallocated.
*/
static cfbPrivScreenPtr screens[MAXSCREENS] = {(cfbPrivScreenPtr)NULL};

unsigned char	getbyte();
unsigned short	getword();

extern int TellLostMap(), TellGainedMap();

/**** HACK HACK HACK - this should really be kept on a per screen basis */
static Bool renScreenBlanked = FALSE;

static Bool
renSaveScreen(pScreen, on)
    ScreenPtr pScreen;
    Bool on;
{
    RENAISSANCE *ren = getGpHardware(pScreen);

    if (on != SCREEN_SAVER_ON)
    {
	renScreenBlanked = FALSE;
	lastEventTime = GetTimeInMillis();
	/* Turn on Video */
	ren -> video_enable = 0xff;
    }
    else
    {
	renScreenBlanked = TRUE;
	/* Turn off video */
	ren -> video_enable = 0;
    }
    
    return TRUE;
}

static void
renUpdateColormap(pScreen, index, count, rmap, gmap,bmap)
    ScreenPtr pScreen;
    register  int	index, count;
    register u_char	*rmap, *gmap, *bmap;
{
  register u_char *gp_hardware = (u_char *) getGpHardware(pScreen);
  register i;
  Bool     toggleScreenSaver = renScreenBlanked;

  if (toggleScreenSaver)
      /* We hang in status check unless video enabled */
      renSaveScreen(pScreen, SCREEN_SAVER_OFF);

  while (count--) {
      /* Wait for color map not busy */
      while (ren_cm_busy()) ;
	
      ren_cm_set_red(index,rmap[index]);
      ren_cm_set_green(index,gmap[index]);
      ren_cm_set_blue(index,bmap[index]);

      index++;
    }

  if (toggleScreenSaver)
      renSaveScreen(pScreen, SCREEN_SAVER_ON);
}

Bool
renScreenInfo(index, argv, argc)
    int index;
    char **argv;
    int argc;
{
    cfbPrivScreenPtr thisScreen;
    renPrivPtr ren;
    int fd, gcon, gcid;

    thisScreen = (cfbPrivScreenPtr) Xalloc(sizeof(cfbPrivScreen));
    ren = (renPrivPtr) Xalloc(sizeof(renPriv));

    screenInfo.screen[index].devPrivate = (pointer)thisScreen;
    screens[index] = thisScreen;
    thisScreen->pHardwareScreen = (pointer)ren;

    ren->renDev = (RENAISSANCE *)NULL;
    ren->InstalledMap = (ColormapPtr)NULL;

    if ((fd = open(argv[2], O_RDWR)) <  0)
    {
        perror(argv[0]);
        ErrorF("%s: couldn't open %s \n", argv[0], argv[2]);
        return FALSE;
    }
    if (ioctl(fd, GCON, &gcon) < 0 || ioctl(fd, GCID, &gcid) < 0)
    {
        ErrorF("%s: couldn't GCON and GCID %s \n", argv[0], argv[2]);
        return FALSE; 
    }

    if (gcid != 10 ) /* Not a Renaissance */
    {
	ErrorF("%s: device %s not this kind of display.\n", argv[0], argv[2]);
	return FALSE;
    }

    thisScreen->fd   = fd;
    thisScreen->gcid = gcid;

    /*
     * Map the Renaissance in to our address space.  We could ask the O.S.
     * to map it in where it prefers, but this would limit the amount of
     * data we can malloc() at a later time.
     * However, we don't know how much address space it will take up until
     * we can examine its memWide and memHigh values....
     */

    ren->renDev = (RENAISSANCE *) FrameBufferBase(0);

    if (ioctl(fd, GCMAP, &ren->renDev) < 0)
    {
	perror("GCMAP:");
        ErrorF("%s: Error getting address of %s\n", argv[0], argv[1]);
        close(fd);
        return FALSE;
    }

    /* Now, calculate how much address space the frame buffer takes, and
     * bump FrameBufferBase() by that amount.
     */
    {
	int memWide, memHigh, size;

	memWide = 2048;
	memHigh = 1024;
	size = memWide * memHigh + 0x20000;
	size = 0xfff00000 & (size + 0xfffff);
	(void) FrameBufferBase(size);

	thisScreen->memHeight = memHigh;
	thisScreen->memWidth  = memWide;
    }

    /* store the screen minor number in the devPrivate structure;
     * if there are four arguments, the fourth is the screen minor number;
     */
    if (argc == 4)
	thisScreen->minor_num = strtol(argv[3], (char**)NULL, 0);
    else
	thisScreen->minor_num = 0;

    return TRUE;
}


/*-
 *-----------------------------------------------------------------------
 * renInstallColormap --
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
renInstallColormap(cmap)
    ColormapPtr	cmap;
{
    register int i;
    Entry *pent = cmap->red;
    u_char	  rmap[256], gmap[256], bmap[256];
    ColormapPtr	  renInstalledMap;
    renPrivPtr ren =
      ((renPrivPtr)((cfbPrivScreenPtr)(cmap->pScreen->devPrivate))->
	pHardwareScreen);

    renInstalledMap = ren->InstalledMap;
    if (cmap == renInstalledMap)
	return;
    if (renInstalledMap)
	WalkTree(renInstalledMap->pScreen, TellLostMap,
		 (char *) &(renInstalledMap->mid));
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
    ren->InstalledMap = cmap;
    renUpdateColormap(cmap->pScreen,
			 0, cmap->pVisual->ColormapEntries, rmap, gmap, bmap);
    WalkTree(cmap->pScreen, TellGainedMap, (char *) &(cmap->mid));
}

/*-
 *-----------------------------------------------------------------------
 * renUninstallColormap --
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
renUninstallColormap(cmap)
    ColormapPtr	cmap;
{
    renPrivPtr ren =
      (renPrivPtr)((cfbPrivScreenPtr)(cmap->pScreen->devPrivate))->
	pHardwareScreen;

     if (cmap == ren->InstalledMap) {
	Colormap defMapID = cmap->pScreen->defColormap;

	if (cmap->mid != defMapID) {
	    ColormapPtr defMap = (ColormapPtr) LookupID(defMapID, RT_COLORMAP, RC_CORE);

	    if (defMap)
		renInstallColormap(defMap);
	    else
	        ErrorF("ren: Can't find default colormap\n");
	}
    }
}

/*-
 *-----------------------------------------------------------------------
 * renListInstalledColormaps --
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
renListInstalledColormaps(pScreen, pCmapList)
    ScreenPtr	pScreen;
    Colormap	*pCmapList;
{
    *pCmapList =
      ((renPrivPtr)((cfbPrivScreenPtr)(pScreen->devPrivate))->
	pHardwareScreen)->InstalledMap->mid;
    return (1);
}


/*-
 *-----------------------------------------------------------------------
 * renStoreColors --
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
renStoreColors(pmap, ndef, pdefs)
    ColormapPtr	pmap;
    int		ndef;
    xColorItem	*pdefs;
{
    switch (pmap->class) {
    case PseudoColor:
	if (pmap ==
	    ((renPrivPtr)
	     ((cfbPrivScreenPtr)(pmap->pScreen->devPrivate))->pHardwareScreen)
	    ->InstalledMap) {
	    /* We only have a single colormap */
	    u_char	rmap[256], gmap[256], bmap[256];

	    while (ndef--) {
		register unsigned index = pdefs->pixel&0xff;

		rmap[index] = (pdefs->red) >> 8;
		gmap[index] = (pdefs->green) >> 8;
		bmap[index] = (pdefs->blue) >> 8;
	 	renUpdateColormap(pmap->pScreen,
				     index, 1, rmap, gmap, bmap);
		pdefs++;
	    }
	}
	break;
    case DirectColor:
    default:
	ErrorF("renStoreColors: bad class %d\n", pmap->class);
	break;
    }
}

/*-
 *-----------------------------------------------------------------------
 * renResolvePseudoColor --
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
renResolvePseudoColor(pRed, pGreen, pBlue, pVisual)
    CARD16	*pRed, *pGreen, *pBlue;
    VisualPtr	pVisual;
{
    *pRed &= 0xff00;
    *pGreen &= 0xff00;
    *pBlue &= 0xff00;
}

/*
 * Configure the Renaissance hardware masks;
 */

void
renMaskConfig(pScreen, foo, bar)
{

}

/*
 * Wait for the renaissance mover hardware to be not-busy and then initiate
 * the requested move operation.
 */
void
renMoveBits(pScreen, planeMask, alu,
	    sourceX, sourceY, destX, destY, width, height)
    ScreenPtr pScreen;
    u_char    planeMask;
    int       alu, sourceX, sourceY, destX, destY, width, height;
{
    u_char pMask		 = getPlanesMask(pScreen);
    register RENAISSANCE *gp_hardware = getGpHardware(pScreen);
    int       k;

    /* wait for hardware ready. test all possible planes */
    
    /* return without doing any other work if no planes are effected...
     Note: we have assured that any previous mover operation has completed.*/
    if (!planeMask) return;

    gp_hardware->source_x = sourceX;
    gp_hardware->source_y = sourceY;
    gp_hardware->dest_x = destX;
    gp_hardware->dest_y = destY;
    gp_hardware->window_width = width;
    gp_hardware->window_height = height;
    gp_hardware->rep_rule = alu | (alu<<4);
    gp_hardware->write_enable = planeMask;
    gp_hardware->wmove = 0x01;
    waitbusy(gp_hardware);
    gp_hardware->rep_rule = 0x33;
    gp_hardware->write_enable = 0xff;
}    

extern void sunMoveMouse(), sunCursorOff();

Bool
renScreenInit(index, pScreen, argc, argv)
    int index;
    ScreenPtr pScreen;
    int argc;		/* these two may NOT be changed */
    char **argv;
{
    CARD16	zero = 0, ones = ~0;
    int i;
    u_char numPlanes;
    unsigned short blockoff;
    renPrivPtr ren;
    RENAISSANCE *gp_hardware;
    cfbPrivScreenPtr pPrivScreen;

    /* always restore the devPrivate field here;
     * if it has not been allocated, this will null it out so code elsewhere
     * will be sure to allocate one;
     * If we've already allocated one, this will restore it;
     */
    pScreen->devPrivate = (pointer)screens[index];
    pPrivScreen = (cfbPrivScreenPtr)(pScreen->devPrivate);
    ren = (renPrivPtr)(pPrivScreen->pHardwareScreen);
    gp_hardware = ren->renDev;
    pPrivScreen->MoveBits = renMoveBits;
    pPrivScreen->MaskConfig = renMaskConfig;

    /* Initialize video hardware */

    ren_init(gp_hardware, CM_MAPPED_0, FOLDED);


    /* Video card initialized */
    
    gp_hardware -> write_enable = ~0;
    gp_hardware -> rep_rule = GXcopy | (GXcopy<<4);

    /*
     * Set up the color map
     */

    numPlanes = 8;
    pPrivScreen->planesMask = 0xff;

    cfbScreenInit(index, pScreen,
	((u_char *) gp_hardware) + 0x20000, 1280, 1024,
	80, numPlanes);

    pPrivScreen->MoveMouse = sunMoveMouse;
    pPrivScreen->CursorOff = sunCursorOff;

    pScreen->CreateGC = renCreateGC;
    pScreen->SaveScreen = renSaveScreen;
    pScreen->RealizeCursor = sunRealizeCursor;
    pScreen->UnrealizeCursor = sunUnrealizeCursor;
    pScreen->DisplayCursor = sunDisplayCursor;
    pScreen->SetCursorPosition = sunSetCursorPosition;
    pScreen->CursorLimits = sunCursorLimits;
    pScreen->PointerNonInterestBox = sunPointerNonInterestBox;
    pScreen->ConstrainCursor = sunConstrainCursor;
    pScreen->RecolorCursor = sunRecolorCursor;
#ifndef	STATIC_COLOR
    pScreen->InstallColormap = renInstallColormap;
    pScreen->UninstallColormap = renUninstallColormap;
    pScreen->ListInstalledColormaps = renListInstalledColormaps;
    pScreen->StoreColors = renStoreColors;
    pScreen->ResolveColor = renResolvePseudoColor;
#endif

    {
	ColormapPtr cmap = (ColormapPtr)LookupID(pScreen->defColormap, RT_COLORMAP, RC_CORE);

	if (!cmap)
	    FatalError("Can't find default colormap\n");
	ren->InstalledMap = (ColormapPtr)NULL; /* prevent reinitialization errors */
	renInstallColormap(cmap);
	if (AllocColor(cmap, &zero, &zero, &zero, &(pScreen->blackPixel), 0)
	    || AllocColor(cmap, &ones, &ones, &ones, &(pScreen->whitePixel), 0))
		FatalError("Can't alloc black & white pixels in cfbScreeninit\n");
    }

    renSaveScreen( pScreen, SCREEN_SAVER_FORCER );
    sunScreenInit (pScreen);
    return (TRUE);	/* XXX- this will be ignored */
}

Bool
renScreenClose(index, pScreen)
    int index;
    ScreenPtr pScreen;
{
    register u_char pMask	 = getPlanesMask(pScreen);
    register RENAISSANCE *gp_hardware = getGpHardware(pScreen);
    static u_char colors[2] = {0x00, 0xff};

    renScreenBlanked = FALSE;	
    renUpdateColormap(pScreen,0,2,colors,colors,colors);

    waitbusy (gp_hardware);
    gp_hardware -> write_enable = ~0;
    gp_hardware -> rep_rule = GXclear|(GXclear<<4);
    gp_hardware -> source_x = 0;
    gp_hardware -> source_y = 0;
    gp_hardware -> dest_x = 0;
    gp_hardware -> dest_y = 0;
    gp_hardware -> window_width = pScreen->width;
    gp_hardware -> window_height = pScreen->height;
    gp_hardware -> wmove = 0x01;

    cfbScreenClose(pScreen);
}

/**
 ** REN_INIT -  initializes the renaissance.
 **		PARAMETERS:
 **		colormap:	either CM_LINEAR or CM_GAMMA
 **		colormap_drive:	either  CM_MAPPED_0, - plane 0 drives rgb
 **					CM_MAPED_1,  - plane 1 drives rgb
 **					CM_UNMAPPED; - 2 drives red, 1 drives green, 0 drives blue;
 **             folded:         either FOLDED(1) or UNFOLDED(0); (true or false);
 **/

ren_init(gp_hardware,colormap_drive,folded)
RENAISSANCE *gp_hardware;
int colormap_drive, folded;
{
   int i;

   gp_hardware->reset = 0x00;
   for(i=0;i<1024;i++);

   gp_hardware->write_enable = 0xffffffff;
   gp_hardware->video_enable = 0xff;
   gp_hardware->display_enable = 0x01;
   gp_hardware->blink = 0x00;
   gp_hardware->interrupt = 0x04;
   ren_op_clear(gp_hardware);

   /************************************/
   /** Select the colormap drive mode **/
   /************************************/

   gp_hardware->drive = 0x01;
   gp_hardware->vdrive = 0x00;

#ifdef UNDEFINED
   switch (colormap_drive) {
   case CM_MAPPED_0:	gp_hardware->vdrive = 0x00;
			break;
   case CM_MAPPED_1:	gp_hardware->vdrive = 0x01;
			break;
   default:
   case CM_UNMAPPED:	gp_hardware->vdrive = 0x02;
			break;
   }

   if (folded) {
       gp_hardware->drive = 0x00;
       gp_hardware->fold  = 0x01;
   }
   else {
       gp_hardware->fold  = 0x00;
       gp_hardware->drive = 0x0f;
   }
#endif UNDEFINED
}

ren_op_clear(gp_hardware)
u_char *gp_hardware;
{
   int i;
   for(i=0; i<16; i++) {
       *(gp_hardware + 0x63c3 + i*4) = 0;
       *(gp_hardware + 0x73c3 + i*4) = 0;
    }
}
