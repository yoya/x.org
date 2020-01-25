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
#if	RCSID && IN_MAIN
static char *trcsid="$Header: catseye.c,v 1.3 88/02/27 10:01:30 rws Exp $";
#endif	RCSID

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

/*
 * $Log:	catseye.c,v $
 * Revision 1.3  88/02/27  10:01:30  rws
 * get screen save right (again)
 * 
 * Revision 1.2  88/02/26  13:57:36  rws
 * screen saver fix
 * 
 * Revision 1.1  87/12/16  13:22:34  hp
 * Initial revision
 * 
 * Revision 1.1  87/12/14  14:35:47  hp
 * Initial revision
 * 
 * Revision 1.2  87/12/03 16:03:14 GMT  peter
 * HP server source tree restructuring...
 * 
 * Revision 1.1  87/12/03  11:17:34  11:17:34  peter ()
 * Initial revision
 * 
 *
 */

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
#include "catseye.h"
#include "gcstruct.h"
#include "windowstr.h"
#include "sun.h"
#include    "colormap.h"
#include    "colormapst.h"
#include    "resource.h"

extern int 	CatseyeBrainDamage;
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
static Bool catseyeScreenBlanked = FALSE;

static Bool
catseyeSaveScreen(pScreen, on)
    ScreenPtr pScreen;
    Bool on;
{
    CATSEYE *catseye = getGpHardware(pScreen);

    if (on != SCREEN_SAVER_ON)
    {
	catseyeScreenBlanked = FALSE;
	lastEventTime = GetTimeInMillis();
	/* Turn on Video */
	catseye -> nblank = 0xff;
    }
    else
    {
	catseyeScreenBlanked = TRUE;
	/* Turn off video */
	catseye -> nblank = 0x00;
    }
    
    return TRUE;
}

static void
catseyeUpdateColormap(pScreen, index, count, rmap, gmap,bmap)
    ScreenPtr pScreen;
    register  int	index, count;
    register u_char	*rmap, *gmap, *bmap;
{
  register CATSEYE *gp_hardware = getGpHardware(pScreen);
  register i;
  Bool     toggleScreenSaver = catseyeScreenBlanked;

  if (((cfbPrivScreenPtr)(pScreen->devPrivate))->planesMask == 0x01)
      return;

  if (toggleScreenSaver)
      /* We hang in status check unless video enabled */
      catseyeSaveScreen(pScreen, SCREEN_SAVER_OFF);

  while (count--)
    {
      /* Wait for color map not busy */
      while (gp_hardware -> colormap_status & 0x04)
	for (i=0; i<20; i++);
	
      gp_hardware->plane_mask = 0xff;

      gp_hardware->red_data = rmap[index];
      gp_hardware->green_data = gmap[index];;
      gp_hardware->blue_data = bmap[index];;
      gp_hardware->color_index = ~index;
      gp_hardware->colormap_writestrobe = 0xff;

      /*
       * Not sure just why we have to do this, but it sure doesn't work
       * without it. Delay loop slowed and delay after last status added.
       * Seemed to be needed for faster 350 processor. HACK!
       */

      while (gp_hardware -> colormap_status & 0x04)
	for (i=0; i<100; i++);
	
      for (i=0; i<1000; i++);

      gp_hardware -> red_data = 0;
      gp_hardware -> green_data = 0;
      gp_hardware -> blue_data = 0;
      gp_hardware -> color_index = 0;

      index++;
    }

  if (toggleScreenSaver)
      catseyeSaveScreen(pScreen, SCREEN_SAVER_ON);
}

/* The following data is cribbed from Starbase:_hp_gbox_setup_6845 via adb */

#define CRTC_DATA_LENGTH  0x0e
static unsigned char crtc_init_data[CRTC_DATA_LENGTH] = {
    0x29, 0x20, 0x23, 0x04, 0x30, 0x0b, 0x30, 0x30,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00};

Bool
catseyeScreenInfo(index, argv, argc)
    int index;
    char **argv;
    int argc;
{
    cfbPrivScreenPtr thisScreen;
    catseyePrivPtr catseye;
    int fd, gcon, gcid;

    thisScreen = (cfbPrivScreenPtr) Xalloc(sizeof(cfbPrivScreen));
    catseye = (catseyePrivPtr) Xalloc(sizeof(catseyePriv));

    screenInfo.screen[index].devPrivate = (pointer)thisScreen;
    screens[index] = thisScreen;
    thisScreen->pHardwareScreen = (pointer)catseye;

    catseye->catseyeDev = (CATSEYE *)NULL;
    catseye->InstalledMap = (ColormapPtr)NULL;

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

    catseye->catseyeDev = (CATSEYE *) FrameBufferBase(0);

    if (ioctl(fd, GCMAP, &catseye->catseyeDev) < 0)
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
      CATSEYE *tc;
      tc = catseye->catseyeDev;
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
 * catseyeInstallColormap --
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
catseyeInstallColormap(cmap)
    ColormapPtr	cmap;
{
    register int i;
    Entry *pent = cmap->red;
    u_char	  rmap[256], gmap[256], bmap[256];
    ColormapPtr	  catseyeInstalledMap;
    catseyePrivPtr catseye =
      ((catseyePrivPtr)((cfbPrivScreenPtr)(cmap->pScreen->devPrivate))->
	pHardwareScreen);

    catseyeInstalledMap = catseye->InstalledMap;
    if (cmap == catseyeInstalledMap)
	return;
    if (catseyeInstalledMap)
	WalkTree(catseyeInstalledMap->pScreen, TellLostMap,
		 (char *) &(catseyeInstalledMap->mid));
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
    catseye->InstalledMap = cmap;
    catseyeUpdateColormap(cmap->pScreen,
			 0, cmap->pVisual->ColormapEntries, rmap, gmap, bmap);
    WalkTree(cmap->pScreen, TellGainedMap, (char *) &(cmap->mid));
}

/*-
 *-----------------------------------------------------------------------
 * catseyeUninstallColormap --
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
catseyeUninstallColormap(cmap)
    ColormapPtr	cmap;
{
    catseyePrivPtr catseye =
      (catseyePrivPtr)((cfbPrivScreenPtr)(cmap->pScreen->devPrivate))->
	pHardwareScreen;

     if (cmap == catseye->InstalledMap) {
	Colormap defMapID = cmap->pScreen->defColormap;

	if (cmap->mid != defMapID) {
	    ColormapPtr defMap = (ColormapPtr) LookupID(defMapID, RT_COLORMAP, RC_CORE);

	    if (defMap)
		catseyeInstallColormap(defMap);
	    else
	        ErrorF("catseye: Can't find default colormap\n");
	}
    }
}

/*-
 *-----------------------------------------------------------------------
 * catseyeListInstalledColormaps --
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
catseyeListInstalledColormaps(pScreen, pCmapList)
    ScreenPtr	pScreen;
    Colormap	*pCmapList;
{
    *pCmapList =
      ((catseyePrivPtr)((cfbPrivScreenPtr)(pScreen->devPrivate))->
	pHardwareScreen)->InstalledMap->mid;
    return (1);
}


/*-
 *-----------------------------------------------------------------------
 * catseyeStoreColors --
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
catseyeStoreColors(pmap, ndef, pdefs)
    ColormapPtr	pmap;
    int		ndef;
    xColorItem	*pdefs;
{
    switch (pmap->class) {
    case PseudoColor:
	if (pmap ==
	    ((catseyePrivPtr)
	     ((cfbPrivScreenPtr)(pmap->pScreen->devPrivate))->pHardwareScreen)
	    ->InstalledMap) {
	    /* We only have a single colormap */
	    u_char	rmap[256], gmap[256], bmap[256];

	    while (ndef--) {
		register unsigned index = pdefs->pixel&0xff;

		rmap[index] = (pdefs->red) >> 8;
		gmap[index] = (pdefs->green) >> 8;
		bmap[index] = (pdefs->blue) >> 8;
	 	catseyeUpdateColormap(pmap->pScreen,
				     index, 1, rmap, gmap, bmap);
		pdefs++;
	    }
	}
	break;
    case DirectColor:
    default:
	ErrorF("catseyeStoreColors: bad class %d\n", pmap->class);
	break;
    }
}

/*-
 *-----------------------------------------------------------------------
 * catseyeResolvePseudoColor --
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
catseyeResolvePseudoColor(pRed, pGreen, pBlue, pVisual)
    CARD16	*pRed, *pGreen, *pBlue;
    VisualPtr	pVisual;
{
    *pRed &= 0xff00;
    *pGreen &= 0xff00;
    *pBlue &= 0xff00;
}

/*
 * Configure the catseye hardware masks;
 */

void
catseyeMaskConfig(pScreen, writeEnableMask, replacementRule)
    register ScreenPtr pScreen;
    register int writeEnableMask, replacementRule;
{
    register CATSEYE *gp_hardware = getGpHardware(pScreen);

    waitbusy(writeEnableMask);
    gp_hardware -> write_enable = writeEnableMask;
    gp_hardware -> pixel_write_replacement_rule = replacementRule;
}

/*
 * Wait for the topcat mover hardware to be not-busy and then initiate
 * the requested move operation.
 */
void
catseyeMoveBits(pScreen, planeMask, alu,
	    sourceX, sourceY, destX, destY, width, height)
    ScreenPtr pScreen;
    u_char    planeMask;
    int       alu, sourceX, sourceY, destX, destY, width, height;
{
    u_char pMask		 = getPlanesMask(pScreen);
    register CATSEYE *gp_hardware = getGpHardware(pScreen);
    int       k;

    /* wait for hardware ready. test all possible planes */
    while ((gp_hardware -> move_active) & pMask)
      for (k=0; k<100; k++); /* delay to let catseye mover work */
    
    /* return without doing any other work if no planes are effected...
     Note: we have assured that any previous mover operation has completed.*/
    if ((planeMask &= pMask) == 0) return;

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

Bool
catseyeScreenInit(index, pScreen, argc, argv)
    int index;
    ScreenPtr pScreen;
    int argc;		/* these two may NOT be changed */
    char **argv;
{
    CARD16	zero = 0, ones = ~0;
    int i;
    u_char numPlanes;
    unsigned short blockoff;
    catseyePrivPtr catseye;
    CATSEYE *gp_hardware;
    cfbPrivScreenPtr pPrivScreen;

    /* always restore the devPrivate field here;
     * if it has not been allocated, this will null it out so code elsewhere
     * will be sure to allocate one;
     * If we've already allocated one, this will restore it;
     */
    pScreen->devPrivate = (pointer)screens[index];
    pPrivScreen = (cfbPrivScreenPtr)(pScreen->devPrivate);
    catseye = (catseyePrivPtr)(pPrivScreen->pHardwareScreen);
    gp_hardware = catseye->catseyeDev;
    pPrivScreen->MoveBits = catseyeMoveBits;
    pPrivScreen->MaskConfig = catseyeMaskConfig;

    /* Initialize video card */
    blockoff = getword(gp_hardware, RAI0);
    while (blockoff != 0) blockoff = processblk(gp_hardware, blockoff);

    /* Video card initialized */
    
    gp_hardware -> write_enable = ~0;
    gp_hardware -> pixel_write_replacement_rule = GXcopy;
    gp_hardware -> frame_buf_write_enable = ~0;

    /*
     * Set up the color map
     */

    numPlanes = CatseyeNumPlanes(gp_hardware, &pPrivScreen->planesMask);
    cfbScreenInit(index, pScreen,
	((u_char *) gp_hardware) + 0x10000,
	(gp_hardware->t_dispwide << 8)+gp_hardware->b_dispwide,
	(gp_hardware->t_disphigh << 8)+gp_hardware->b_disphigh,
	80, numPlanes);

    pScreen->CreateGC = catseyeCreateGC;
    pScreen->SaveScreen = catseyeSaveScreen;
    pScreen->RealizeCursor = sunRealizeCursor;
    pScreen->UnrealizeCursor = sunUnrealizeCursor;
    pScreen->DisplayCursor = sunDisplayCursor;
    pScreen->SetCursorPosition = sunSetCursorPosition;
    pScreen->CursorLimits = sunCursorLimits;
    pScreen->PointerNonInterestBox = sunPointerNonInterestBox;
    pScreen->ConstrainCursor = sunConstrainCursor;
    pScreen->RecolorCursor = sunRecolorCursor;
#ifndef	STATIC_COLOR
    pScreen->InstallColormap = catseyeInstallColormap;
    pScreen->UninstallColormap = catseyeUninstallColormap;
    pScreen->ListInstalledColormaps = catseyeListInstalledColormaps;
    pScreen->StoreColors = catseyeStoreColors;
    pScreen->ResolveColor = catseyeResolvePseudoColor;
#endif

    if (numPlanes == 1)	{
	pScreen->blackPixel = 0;
	pScreen->whitePixel = 1;
    }
    else {
	ColormapPtr cmap = (ColormapPtr)LookupID(pScreen->defColormap, RT_COLORMAP, RC_CORE);

	if (!cmap)
	    FatalError("Can't find default colormap\n");
	catseye->InstalledMap = (ColormapPtr)NULL; /* prevent reinitialization errors */
	catseyeInstallColormap(cmap);
	if (AllocColor(cmap, &zero, &zero, &zero, &(pScreen->blackPixel), 0)
	    || AllocColor(cmap, &ones, &ones, &ones, &(pScreen->whitePixel), 0))
	    FatalError("Can't alloc black & white pixels in cfbScreeninit\n");
    }

    catseyeSaveScreen( pScreen, SCREEN_SAVER_OFF );
    sunScreenInit (pScreen);
    return (TRUE);	/* XXX- this will be ignored */
}

Bool
catseyeScreenClose(index, pScreen)
    int index;
    ScreenPtr pScreen;
{
    register u_char pMask	 = getPlanesMask(pScreen);
    register CATSEYE *gp_hardware = getGpHardware(pScreen);
    static u_char colors[2] = {0x00, 0xff};

    catseyeScreenBlanked = FALSE;	
    catseyeUpdateColormap(pScreen,0,2,colors,colors,colors);

    waitbusy (pMask);
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

CatseyeNumPlanes(catseye, pMask)
     CATSEYE *catseye;
     u_char *pMask;
{
  u_char *base;
  u_char sample0, sample1, rwBits;
  int numPlanes;
  base = ((u_char *) catseye) + 0x10000;
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

  if (CatseyeBrainDamage) {
      *pMask = 0x01;
      numPlanes = 1;
  }

  return numPlanes;
}

/*
 * following code borrowed from xcelinit.c authored by Tim Connors at HPL
 */

processblk(catseye, blkoff)
        CATSEYE *catseye;
	unsigned short	blkoff;
{
	unsigned char	cntlbyte, wordcnt;

	cntlbyte	= getbyte(catseye, blkoff);
	wordcnt		= getbyte(catseye, blkoff+2);
	switch (cntlbyte & 0x7f) {
		case 0x00:

				do_copblk(catseye, blkoff);
				break;

		case 0x40:
				ErrorF("catseye: WARNING! constant fill not implemented\n");

				break;

		case 0x01:
				ErrorF("catseye: WARNING! test bit not implemented\n");

				break;

		default  :
				ErrorF("catseye: WARNING! unknown controlbyte\n");

				break;
	}
	if (cntlbyte & 0x80)
		return 0;
	else
		return blkoff + 2 + 2 + 4 + 4*(wordcnt+1);
}

do_copblk(catseye, blkoff)
        CATSEYE *catseye;
	unsigned short	blkoff;
{
	unsigned char	cntlbyte, wordcnt;
	unsigned short	destoff, source;
	int		i;

	cntlbyte	= getbyte(catseye, blkoff);
	wordcnt		= getbyte(catseye, blkoff+2);
	destoff		= getword(catseye, blkoff+4);

	source = blkoff + 2 + 2 + 4;
	for ( i=0; i < wordcnt+1; i++) {
		putword(catseye, getword(catseye, source), destoff);
		source	+= 4;
		destoff	+= 2;
	}
}

putword(catseye, word, destoff)
        CATSEYE *catseye;
	unsigned short	word;
	unsigned short	destoff;
{
	int addr;

	addr = (int) catseye + destoff;
	*(short*)addr = word;
}

unsigned char getbyte(catseye, offset)
        CATSEYE *catseye;
	int offset;
{
	return *(char*)((int) catseye + offset);
}

unsigned short getword(catseye, offset)
        CATSEYE *catseye;
	int offset;
{
	unsigned short	value;
	char		*d1, *d2;

	d1 = (char*)&value;
	d2 = d1+1;
	*d1 = *(char*)((int) catseye + offset);
	*d2 = *(char*)((int) catseye + offset + 2);
	return value;
}
