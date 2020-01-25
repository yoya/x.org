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
#include <sys/iomap.h>

#include "X.h"
#define  NEED_EVENTS
#include "Xproto.h"
#include "scrnintstr.h"
#include "cursorstr.h"
#include "pixmapstr.h"
#include "inputstr.h"
#include "windowstr.h"
#include "regionstr.h"

#include "sun.h"
#include "mi.h"
#include "mob.h"
#include "gcstruct.h"


/*
  maintain a local pointer to the screen->devPrivate structure. When the
  server restarts, this will be used to restore the screen pointer lost
  when the screen structure is reallocated.
*/
static mfbPrivScreenPtr screens[MAXSCREENS] = {(mfbPrivScreenPtr)NULL};

extern void miRecolorCursor();
extern int		lastEventTime;

static MobDisplay	*moberly[MAXSCREENS];

static int		fdmob[MAXSCREENS]  = -1;

static Bool
mobSaveScreen(pScreen, on)
    ScreenPtr pScreen;
    int on;
{
    int myNum = pScreen->myNum;
    if (on != SCREEN_SAVER_ON)
    {
	lastEventTime = GetTimeInMillis();
	/* Turn on Sync */
	moberly[pScreen->myNum]->video_control = MOB_SYNC;
	/* Allow Sync to stabilize */
	{ /* Insert high-res sleep routine here */
	  int i;
	  for ( i = 0; i < 10000; i++) i = i;
        }
	/* Turn on Video */
	moberly[pScreen->myNum]->video_control = MOB_VIDEO;

    }
    else
    {
	moberly[pScreen->myNum]->video_control = MOB_SYNC;
    }

    return TRUE;
}

void
mobResolveColor(pred, pgreen, pblue, pVisual)
    unsigned short	*pred, *pgreen, *pblue;
    VisualPtr		pVisual;
{
    /* Gets intensity from RGB.  If intensity is >= half, pick white, else
     * pick black.  This may well be more trouble than it's worth. */
    *pred = *pgreen = *pblue = 
        (((39L * *pred +
           50L * *pgreen +
           11L * *pblue) >> 8) >= (((1<<8)-1)*50)) ? ~0 : 0;
}

void
mobCreateColormap(pmap)
    ColormapPtr	pmap;
{
    int	red, green, blue, pix;

    /* this is a monochrome colormap, it only has two entries, just fill
     * them in by hand.  If it were a more complex static map, it would be
     * worth writing a for loop or three to initialize it */
    red = green = blue = 0;
    AllocColor(pmap, &red, &green, &blue, &pix, 0);
    red = green = blue = ~0;
    AllocColor(pmap, &red, &green, &blue, &pix, 0);

}

void
mobDestroyColormap(pmap)
    ColormapPtr	pmap;
{
}

/* Moberly CRTC values from Horizon hpux-moberly.s */

#define	NUM_MOB_REGISTERS	18

static unsigned char mob_register_init[NUM_MOB_REGISTERS] = {
    31, 24, 25, 3, 65, 11, 64, 64, 0, 15, 0, 0, 0, 0, 0, 0, 0, 0};

Bool
mobScreenInfo(index, argv, argc)
    int index;
    char **argv;
    int argc;
{
    unsigned char *FrameBufferBase();
    if ((fdmob[index] = open(argv[2], O_RDWR)) <  0)
    {
        ErrorF("%s: couldn't open %s \n", argv[0], argv[2]);
        return FALSE; 
    }

    /*
     * Map the Moberly in to our address space.  We could ask the O.S.
     * to map it in where it prefers, but this would limit the amount of
     * data we can malloc() at a later time.
     *
     */

    screens[index] = (mfbPrivScreenPtr)(screenInfo.screen[index].devPrivate);
    moberly[index] = (MobDisplay *) FrameBufferBase(0x20000);
    if (ioctl(fdmob[index], (int) IOMAPMAP, &moberly[index]) < 0)
    {
	perror("IOMAPMAP:");
        ErrorF("%s: Error getting address of %s\n", argv[0], argv[1]);
        close(fdmob[index]);
        return FALSE;
    }

    return TRUE;
}

extern void sunMoveMouse(), sunCursorOff();

Bool
mobScreenInit(index, pScreen, argc, argv)
    int index;
    ScreenPtr pScreen;
    int argc;		/* these two may NOT be changed */
    char **argv;
{
    register PixmapPtr *pPixmap;
    Bool retval;
    ColormapPtr		pColormap;
    
    /* always restore the devPrivate field here;
     * if it has not been allocated, this will null it out so code elsewhere
     * will be sure to allocate one;
     * If we've already allocated one, this will restore it;
     */
    pScreen->devPrivate = (pointer)screens[index];

    /* Program CTRC registers */
    {
	int i;
	for( i = 0; i < NUM_MOB_REGISTERS; i++) {
	    moberly[pScreen->myNum]->reg_addr = i;
	    moberly[pScreen->myNum]->reg_data = mob_register_init[i];
	    }
	}

    if (!mfbScreenInit(index, pScreen,
	((unsigned char *) moberly[pScreen->myNum]) + 256,
	WIDTH, HEIGHT, 80))
	return (FALSE);

    ((mfbPrivScreenPtr)(pScreen->devPrivate))->MoveBits = 0;

    ((mfbPrivScreenPtr)(pScreen->devPrivate))->MoveMouse = sunMoveMouse;
    ((mfbPrivScreenPtr)(pScreen->devPrivate))->CursorOff = sunCursorOff;

    pScreen->SaveScreen = mobSaveScreen;
    pScreen->RealizeCursor = sunRealizeCursor;
    pScreen->UnrealizeCursor = sunUnrealizeCursor;
    pScreen->DisplayCursor = sunDisplayCursor;
    pScreen->SetCursorPosition = sunSetCursorPosition;
    pScreen->CursorLimits = sunCursorLimits;
    pScreen->PointerNonInterestBox = sunPointerNonInterestBox;
    pScreen->ConstrainCursor = sunConstrainCursor;
    pScreen->RecolorCursor = miRecolorCursor;
    pScreen->ResolveColor = mobResolveColor;
    pScreen->CreateColormap = mobCreateColormap;
    pScreen->DestroyColormap = mobDestroyColormap;
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

    if (CreateColormap(pScreen->defColormap, pScreen,
		   LookupID(pScreen->rootVisual, RT_VISUALID, RC_CORE),
		   &pColormap, AllocNone, 0) != Success
	|| pColormap == NULL)
	    FatalError("Can't create colormap in mobScreenInit()\n");

    /* Initialize the soft cursor stuff */
    sunScreenInit (pScreen);

    /* Turn on Sync */
    moberly[pScreen->myNum]->video_control = MOB_SYNC;
    /* Allow Sync to stabilize */
    { /* Insert high-res sleep routine here */
        int i;
        for ( i = 0; i < 10000; i++) i = i;
        }
    /* Turn on Video */
    moberly[pScreen->myNum]->video_control = MOB_VIDEO;
    return(TRUE);
}

Bool
mobScreenClose(index, pScreen)
    int index;
    ScreenPtr pScreen;
{
  return hpCloseScreen(index, pScreen);
}
