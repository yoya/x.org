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
#include "topcat.h"
#include "gcstruct.h"
#include "windowstr.h"
#include "sun.h"
#include    "colormap.h"
#include    "colormapst.h"
#include    "resource.h"

extern int 	TopcatBrainDamage;
extern int	lastEventTime;
extern unsigned char *FrameBufferBase();
extern Bool	hpRealizeFont(), tcCreateWindow(), tcChangeWindowAttributes();
extern Bool	hpUnrealizeFont();
extern void	hpGetByteImage();

extern u_char XHP_NewRule[16][6];

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
static Bool topcatScreenBlanked = FALSE;

static Bool
topcatSaveScreen(pScreen, on)
    ScreenPtr pScreen;
    Bool on;
{
    TOPCAT *topcat = getGpHardware(pScreen);

    if (on != SCREEN_SAVER_ON)
    {
	lastEventTime = GetTimeInMillis();
	/* Turn on Video */
	topcat -> nblank = 0xff;
	topcatScreenBlanked = FALSE;
    }
    else
    {
	/* Turn off video */
	topcat -> nblank = 0x00;
	topcatScreenBlanked = TRUE;
    }
    
    return TRUE;
}

static void
topcatUpdateColormap(pScreen, index, count, rmap, gmap,bmap)
    ScreenPtr pScreen;
    register  int	index, count;
    register u_char	*rmap, *gmap, *bmap;
{
  register TOPCAT *gp_hardware = getGpHardware(pScreen);
  register i;
  Bool     toggleScreenSaver = topcatScreenBlanked;

  if (((cfbPrivScreenPtr)(pScreen->devPrivate))->planesMask == 0x01)
      return;

  if (toggleScreenSaver)
      /* We hang in status check unless video enabled */
      topcatSaveScreen(pScreen, SCREEN_SAVER_OFF);

  while (count--)
    {
      unsigned short id;

      /* Wait for color map not busy */
      while (gp_hardware -> colormap_status & 0x04)
	for (i=0; i<20; i++);
	
      gp_hardware->plane_mask = 0xff;

      gp_hardware->red_data = rmap[index];
      id = gp_hardware->id_reset;
      gp_hardware->green_data = gmap[index];;
      id = gp_hardware->id_reset;
      gp_hardware->blue_data = bmap[index];;
      id = gp_hardware->id_reset;
      gp_hardware->color_index = ~index;
      id = gp_hardware->id_reset;
      gp_hardware->colormap_writestrobe = 0xff;
      id = gp_hardware->id_reset;

      /*
       * Not sure just why we have to do this, but it sure doesn't work
       * without it. Delay loop slowed and delay after last status added.
       * Seemed to be needed for faster 350 processor. HACK!
       */

      while (gp_hardware -> colormap_status & 0x04) {
        id = gp_hardware->id_reset;
	for (i=0; i<100; i++);
      }
	
      for (i=0; i<1000; i++);

      gp_hardware -> red_data = 0;
      id = gp_hardware->id_reset;
      gp_hardware -> green_data = 0;
      id = gp_hardware->id_reset;
      gp_hardware -> blue_data = 0;
      id = gp_hardware->id_reset;
      gp_hardware -> color_index = 0;
      id = gp_hardware->id_reset;

      index++;
    }

  if (toggleScreenSaver)
      topcatSaveScreen(pScreen, SCREEN_SAVER_ON);
}

/* The following data is cribbed from Starbase:_hp_gbox_setup_6845 via adb */

#define CRTC_DATA_LENGTH  0x0e
static unsigned char crtc_init_data[CRTC_DATA_LENGTH] = {
    0x29, 0x20, 0x23, 0x04, 0x30, 0x0b, 0x30, 0x30,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00};

Bool
topcatScreenInfo(index, argv, argc)
    int index;
    char **argv;
    int argc;
{
    cfbPrivScreenPtr thisScreen;
    topcatPrivPtr topcat;
    int fd, gcon, gcid;

    thisScreen = (cfbPrivScreenPtr) Xalloc(sizeof(cfbPrivScreen));
    topcat = (topcatPrivPtr) Xalloc(sizeof(topcatPriv));

    screenInfo.screen[index].devPrivate = (pointer)thisScreen;
    screens[index] = thisScreen;
    thisScreen->pHardwareScreen = (pointer)topcat;

    topcat->topcatDev = (TOPCAT *)NULL;
    topcat->InstalledMap = (ColormapPtr)NULL;

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

    if (gcid != 9 ) /* Not a Topcat */
    {
	ErrorF("%s: device %s not this kind of display.\n", argv[0], argv[2]);
	return FALSE;
    }

    thisScreen->fd   = fd;
    thisScreen->gcid = gcid;

    /*
     * Map the topcat in to our address space.  We could ask the O.S.
     * to map it in where it prefers, but this would limit the amount of
     * data we can malloc() at a later time.
     * However, we don't know how much address space it will take up until
     * we can examine its memWide and memHigh values....
     */

    topcat->topcatDev = (TOPCAT *) FrameBufferBase(0);

    if (ioctl(fd, GCMAP, &topcat->topcatDev) < 0)
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
      TOPCAT *tc;
      tc = topcat->topcatDev;
      memWide = (tc->t_memwide << 8) | tc->b_memwide;
      memHigh = (tc->t_memhigh << 8) | tc->b_memhigh;
      size = memWide * memHigh + 0x10000;
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
 * topcatInstallColormap --
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
topcatInstallColormap(cmap)
    ColormapPtr	cmap;
{
    register int i;
    Entry *pent = cmap->red;
    u_char	  rmap[256], gmap[256], bmap[256];
    ColormapPtr	  topcatInstalledMap;
    topcatPrivPtr topcat =
      ((topcatPrivPtr)((cfbPrivScreenPtr)(cmap->pScreen->devPrivate))->
	pHardwareScreen);

    topcatInstalledMap = topcat->InstalledMap;
    if (cmap == topcatInstalledMap)
	return;
    if (topcatInstalledMap)
	WalkTree(topcatInstalledMap->pScreen, TellLostMap,
		 (char *) &(topcatInstalledMap->mid));
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
    topcat->InstalledMap = cmap;
    topcatUpdateColormap(cmap->pScreen,
			 0, cmap->pVisual->ColormapEntries, rmap, gmap, bmap);
    WalkTree(cmap->pScreen, TellGainedMap, (char *) &(cmap->mid));
}

/*-
 *-----------------------------------------------------------------------
 * topcatUninstallColormap --
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
topcatUninstallColormap(cmap)
    ColormapPtr	cmap;
{
    topcatPrivPtr topcat =
      (topcatPrivPtr)((cfbPrivScreenPtr)(cmap->pScreen->devPrivate))->
	pHardwareScreen;

     if (cmap == topcat->InstalledMap) {
	Colormap defMapID = cmap->pScreen->defColormap;

	if (cmap->mid != defMapID) {
	    ColormapPtr defMap = (ColormapPtr) LookupID(defMapID, RT_COLORMAP, RC_CORE);

	    if (defMap)
		topcatInstallColormap(defMap);
	    else
	        ErrorF("topcat: Can't find default colormap\n");
	}
    }
}

/*-
 *-----------------------------------------------------------------------
 * topcatListInstalledColormaps --
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
topcatListInstalledColormaps(pScreen, pCmapList)
    ScreenPtr	pScreen;
    Colormap	*pCmapList;
{
    *pCmapList =
      ((topcatPrivPtr)((cfbPrivScreenPtr)(pScreen->devPrivate))->
	pHardwareScreen)->InstalledMap->mid;
    return (1);
}


/*-
 *-----------------------------------------------------------------------
 * topcatStoreColors --
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
topcatStoreColors(pmap, ndef, pdefs)
    ColormapPtr	pmap;
    int		ndef;
    xColorItem	*pdefs;
{
    switch (pmap->class) {
    case PseudoColor:
	if (pmap ==
	    ((topcatPrivPtr)
	     ((cfbPrivScreenPtr)(pmap->pScreen->devPrivate))->pHardwareScreen)
	    ->InstalledMap) {
	    /* We only have a single colormap */
	    u_char	rmap[256], gmap[256], bmap[256];

	    while (ndef--) {
		register unsigned index = pdefs->pixel&0xff;

		rmap[index] = (pdefs->red) >> 8;
		gmap[index] = (pdefs->green) >> 8;
		bmap[index] = (pdefs->blue) >> 8;
	 	topcatUpdateColormap(pmap->pScreen,
				     index, 1, rmap, gmap, bmap);
		pdefs++;
	    }
	}
	break;
    case DirectColor:
    default:
	ErrorF("topcatStoreColors: bad class %d\n", pmap->class);
	break;
    }
}

/*-
 *-----------------------------------------------------------------------
 * topcatResolvePseudoColor --
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
topcatResolvePseudoColor(pRed, pGreen, pBlue, pVisual)
    CARD16	*pRed, *pGreen, *pBlue;
    VisualPtr	pVisual;
{
  register int mask;

  if (pVisual->nplanes!=1) mask = 0xFF00;       /* 8 bit color */
  else mask = 0x8000;   /* monochrome */

  *pRed &= mask; *pGreen &= mask; *pBlue &= mask;
}

/*
 * Configure the topcat hardware masks;
 */

void
topcatMaskConfig(pScreen, writeEnableMask, replacementRule)
    register ScreenPtr pScreen;
    register int writeEnableMask, replacementRule;
{
    register TOPCAT *gp_hardware = getGpHardware(pScreen);
    register unsigned int planes = getPlanesMask(pScreen);

    waitbusy(planes, gp_hardware); /* wait for all planes to quiet */
    gp_hardware -> write_enable = writeEnableMask;
    gp_hardware -> frame_buf_write_enable = writeEnableMask;
    gp_hardware -> pixel_write_replacement_rule = replacementRule;
}

/*
 * Wait for the topcat mover hardware to be not-busy and then initiate
 * the requested move operation.
 */
void
topcatMoveBits(pScreen, planeMask, alu,
	    sourceX, sourceY, destX, destY, width, height)
    ScreenPtr pScreen;
    u_char    planeMask;
    int       alu, sourceX, sourceY, destX, destY, width, height;
{
    u_char pMask		 = getPlanesMask(pScreen);
    register TOPCAT *gp_hardware = getGpHardware(pScreen);
    int       k;

    /* wait for hardware ready. test all possible planes */
    while ((gp_hardware -> move_active) & pMask)
      for (k=0; k<100; k++); /* delay to let topcat mover work */
    
    /* return without doing any other work if no planes are effected...
     Note: we have assured that any previous mover operation has completed.*/
    if ((planeMask &= pMask) == 0) return;

    gp_hardware->write_enable = planeMask;
    gp_hardware -> window_move_replacement_rule = alu;
    gp_hardware -> source_x = sourceX;
    gp_hardware -> source_y = sourceY;
    gp_hardware -> dest_x = destX;
    gp_hardware -> dest_y = destY;
    gp_hardware -> window_width = width;
    gp_hardware -> window_height = height;
    /* start move on all GC enabled planes supported by hardware */  
    gp_hardware -> start_move = planeMask;
}    

extern Bool
  hpRealizeCursor(), hpUnrealizeCursor(), hpDisplayCursor(),
  hpSetCursorPosition();
extern void hpInitCursor(), hpCursorLimits(), hpPointerNonInterestBox(),
  hpConstrainCursor(), hpRecolorCursor(), hpRemoveCursor();

Bool
topcatScreenInit(index, pScreen, argc, argv)
    int index;
    ScreenPtr pScreen;
    int argc;		/* these two may NOT be changed */
    char **argv;
{
    CARD16	zero = 0, ones = ~0;
    int i;
    u_char numPlanes;
    unsigned short blockoff;
    topcatPrivPtr topcat;
    TOPCAT *gp_hardware;
    cfbPrivScreenPtr pPrivScreen;

    /* always restore the devPrivate field here;
     * if it has not been allocated, this will null it out so code elsewhere
     * will be sure to allocate one;
     * If we've already allocated one, this will restore it;
     */
    pScreen->devPrivate = (pointer)screens[index];
    pPrivScreen = (cfbPrivScreenPtr)(pScreen->devPrivate);
    topcat = (topcatPrivPtr)(pPrivScreen->pHardwareScreen);
    gp_hardware = topcat->topcatDev;
    pPrivScreen->MoveBits = topcatMoveBits;
    pPrivScreen->MaskConfig = topcatMaskConfig;

    /* Initialize video card */
    /*
    blockoff = getword(gp_hardware, RAI0);
    while (blockoff != 0) blockoff = processblk(gp_hardware, blockoff);
    */

    /* Video card initialized */
    
    gp_hardware -> write_enable = ~0;
    gp_hardware -> pixel_write_replacement_rule = GXcopy;
    gp_hardware -> frame_buf_write_enable = ~0;

    /*
     * Set up the color map
     */

    numPlanes = TopcatNumPlanes(gp_hardware, &pPrivScreen->planesMask);
    cfbScreenInit(index, pScreen,
	((u_char *) gp_hardware) + 0x10000,
	(gp_hardware->t_dispwide << 8)+gp_hardware->b_dispwide,
	(gp_hardware->t_disphigh << 8)+gp_hardware->b_disphigh,
	80, numPlanes);

    hpInitCursor(pScreen);	/* init cursor stuff */

    pScreen->ChangeWindowAttributes = tcChangeWindowAttributes;
    pScreen->CreateWindow = tcCreateWindow;

    pScreen->CreateGC = topcatCreateGC;
    pScreen->SaveScreen = topcatSaveScreen;
    pScreen->RealizeCursor = hpRealizeCursor;
    pScreen->UnrealizeCursor = hpUnrealizeCursor;
    pScreen->DisplayCursor = hpDisplayCursor;
    pScreen->SetCursorPosition = hpSetCursorPosition;
    pScreen->CursorLimits = hpCursorLimits;
    pScreen->PointerNonInterestBox = hpPointerNonInterestBox;
    pScreen->ConstrainCursor = hpConstrainCursor;
    pScreen->RecolorCursor = hpRecolorCursor;
    pScreen->RealizeFont = hpRealizeFont;
    pScreen->UnrealizeFont = hpUnrealizeFont;
    pScreen->GetImage = hpGetByteImage;
#ifndef	STATIC_COLOR
    pScreen->InstallColormap = topcatInstallColormap;
    pScreen->UninstallColormap = topcatUninstallColormap;
    pScreen->ListInstalledColormaps = topcatListInstalledColormaps;
    pScreen->StoreColors = topcatStoreColors;
    pScreen->ResolveColor = topcatResolvePseudoColor;
#endif

    if (numPlanes == 1)	{
	pScreen->blackPixel = 0;
	pScreen->whitePixel = 1;
    }
    else {
	ColormapPtr cmap = (ColormapPtr)LookupID(pScreen->defColormap, RT_COLORMAP, RC_CORE);

	if (!cmap)
	    FatalError("Can't find default colormap\n");
	topcat->InstalledMap = (ColormapPtr)NULL; /* prevent reinitialization errors */
	topcatInstallColormap(cmap);
	if (AllocColor(cmap, &zero, &zero, &zero, &(pScreen->blackPixel), 0)
	    || AllocColor(cmap, &ones, &ones, &ones, &(pScreen->whitePixel), 0))
	    FatalError("Can't alloc black & white pixels in cfbScreeninit\n");
    }

    topcatSaveScreen( pScreen, SCREEN_SAVER_OFF );
/*     sunScreenInit (pScreen); */
    return (TRUE);	/* XXX- this will be ignored */
}

Bool
topcatScreenClose(index, pScreen)
    int index;
    ScreenPtr pScreen;
{
    register u_char pMask	 = getPlanesMask(pScreen);
    register TOPCAT *gp_hardware = getGpHardware(pScreen);
    static u_char colors[2] = {0x00, 0xff};

    topcatScreenBlanked = FALSE;	
    topcatUpdateColormap(pScreen,0,2,colors,colors,colors);

    waitbusy (pMask, gp_hardware);
    gp_hardware -> nblank = 0xff;
    gp_hardware -> write_enable = ~0;
    gp_hardware -> window_move_replacement_rule = GXclear;
    gp_hardware -> write_enable = ~0;
    gp_hardware -> pixel_write_replacement_rule = GXclear;
    gp_hardware -> source_x = 0;
    gp_hardware -> source_y = 0;
    gp_hardware -> dest_x = 0;
    gp_hardware -> dest_y = 0;
    gp_hardware -> window_width = pScreen->width;
    gp_hardware -> window_height = pScreen->height;
    gp_hardware -> start_move = ~0;

    cfbScreenClose(pScreen);
}

TopcatNumPlanes(topcat, pMask)
     TOPCAT *topcat;
     u_char *pMask;
{
  u_char *base;
  u_char sample0, sample1, rwBits;
  int numPlanes;
  base = ((u_char *) topcat) + 0x10000;
  *base = 0;
  sample0 = *base;
  *base = 0xff;
  sample1 = *base;
  rwBits = sample0 ^ sample1;
  switch(rwBits)
    {
  case 0x00: numPlanes = 0; break;
  case 0x01: numPlanes = 1; break;
  case 0x03: numPlanes = 2; break;
  case 0x07: numPlanes = 3; break;
  case 0x0f: numPlanes = 4; break;
  case 0x1f: numPlanes = 5; break;
  case 0x3f: numPlanes = 6; break;
  case 0x7f: numPlanes = 7; break;
  case 0xff: numPlanes = 8; break;
    default: numPlanes = -1;
  }
  *pMask = rwBits;

  if (TopcatBrainDamage) {
      *pMask = 0x01;
      numPlanes = 1;
  }

  return numPlanes;
}


/*
 * tcPaintBlockClipped
 *   This routine paints a solid-colored block clipped to the composite
 * clipping region described in the devPriv of the GC.
 * Basically it programs the replacement rule registers to place a 
 * rectangle "width" pixels wide by "height" pixels tall
 * with the appropriate color (fgPixel) and alu starting at x, y.
 *  ~jra
 *  ~hp
 */
void
tcPaintBlockClipped( dst,pGC, x, y, width, height)
DrawablePtr dst;
GC *pGC;
int x, y, width, height;
{ 
	ScreenPtr pScreen = dst->pScreen;
	register TOPCAT *gp_hardware = getGpHardware( pScreen );
    	u_char pMask	 = getPlanesMask(pScreen);
	int zmask = pMask & pGC->planemask;
	int srcpix = pGC->fgPixel;
	int k;
	int XHP_bits = gp_hardware->bits;
	int alu = pGC->alu;
	RegionPtr pRegion = (RegionPtr)((cfbPrivGC *)pGC->devPriv)->pCompositeClip;
	register BoxPtr pBox = pRegion->rects;
	int nbox = pRegion->numRects;

	while(nbox--) {
	  /*
	   * for each clipping rectangle, put out the portion of the
	   * block contained in the clipping rect
	   */
	  int clippedWidth = width;
	  int clippedHeight = height;
	  int startx = x;
	  int starty = y;
	  int dx, dy;

	  /*
	   * clip the rectangle height
	   */
	  if(y < pBox->y1) {
	    dy = pBox->y1 - y;
	    clippedHeight -= dy;
	    if(clippedHeight <= 0) {
	      pBox++;
	      continue;
	    }
	    starty = pBox->y1;
	  }
	  if(starty+clippedHeight > pBox->y2) {
	    clippedHeight = pBox->y2 - starty;
	    if(clippedHeight <= 0) {
	      pBox++;
	      continue;
	    }
	  }
	  /*
	   * clip the rectangle width
	   */
	  if(x < pBox->x1) {
	    dx = pBox->x1 - x;
	    clippedWidth -= dx;
	    if(clippedWidth <= 0) {
	      pBox++;
	      continue;
	    }
	    startx = pBox->x1;
	  }
	  if(startx+clippedWidth > pBox->x2) {
	    clippedWidth = pBox->x2 - startx;
	    if(clippedWidth <= 0) {
	      pBox++;
	      continue;
	    }
	  }

	  /*
	   * put out the block part in this clip box
	   */

	  if(clippedWidth && clippedHeight) {
	    waitbusy(pMask, gp_hardware);
    
	    gp_hardware -> write_enable = zmask & srcpix;
            gp_hardware -> window_move_replacement_rule = XHP_NewRule[alu][3];
            gp_hardware -> write_enable = zmask & ~srcpix;
            gp_hardware -> window_move_replacement_rule = XHP_NewRule[alu][0];
            gp_hardware -> write_enable = zmask;
            gp_hardware -> pixel_write_replacement_rule = GXcopy;

	    gp_hardware -> source_x = startx;
	    gp_hardware -> source_y = starty;
	    gp_hardware -> dest_x = startx;
	    gp_hardware -> dest_y = starty;
	    gp_hardware -> window_width = clippedWidth;
	    gp_hardware -> window_height = clippedHeight;

	    gp_hardware -> start_move = zmask;
	  }

	  pBox++;
      }
}
/*
 * tcPolyFillSolidRect -- A fast fill routine for filling rectangles
 * with a solid color on a Topcat
 * This is the pGC->PolyFillRect function when FillStyle == Solid
 */
void
tcPolyFillSolidRect(pDrawable, pGC, nrectFill, prectInit)
    DrawablePtr	pDrawable;
    GCPtr	pGC;
    int		nrectFill; 	/* number of rectangles to fill */
    xRectangle	*prectInit;  	/* Pointer to first rectangle to fill */
{
    register int i;
    register xRectangle *prect; 
    int			xorg;
    int			yorg;

    xorg = ((WindowPtr)pDrawable)->absCorner.x;
    yorg = ((WindowPtr)pDrawable)->absCorner.y;
    prect = prectInit;

    for (i = 0; i<nrectFill; i++, prect++)
    {
        prect->x += xorg;
	prect->y += yorg;
    }

    prect = prectInit;
    while(nrectFill--)
    {
	tcPaintBlockClipped(pDrawable, pGC, prect->x, prect->y, 
				prect->width, prect->height);
	prect++;
    }
}
