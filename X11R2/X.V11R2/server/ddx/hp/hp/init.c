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
/*******************************************************************************
*
* File:         init.c
* RCS:          $Revision: 1.1 $
* Description:  Multiple screen initialization
* Author:       John Howard Palevich
* Created:      April 22, 1987
* Modified:     April 29, 1987  17:41:59 (John Howard Palevich)
* Language:     C
* Package:      USER
* Status:       Experimental (Do Not Distribute)
*
* (c) Copyright 1987, Hewlett-Packard, Inc., all rights reserved.
*
*******************************************************************************/

#include "X.h"
#include "Xproto.h"
#include <servermd.h>
#include "screenint.h"
#include "input.h"
#include "cursor.h"
#include "misc.h"
#include "scrnintstr.h"

#include "screentab.h"
#include "gcstruct.h"
#include "sun.h"
#include <stdio.h>
#include <ctype.h>
#include "cfb/cfb.h" /* XXX should this really be here? */

#define MAXSTRING 120
#define MAXARG 10

extern  char    *display;		/* display number as a string */
extern int	ErrorfOn;
static char xscreens[80];

/*
 * NewRule is an array of replacement rules.  Given a replacement rule,
 *
 *   colunm	foreground pixel	background pixel
 *	0		0			0
 *	1		0			1
 *	2		1			0
 *	3		1			1
 *	4		1		      clear
 *	5		0		      clear
 */

u_char XHP_NewRule [16][6] = 
{
GXclear, GXclear,	GXclear,       GXclear, GXandInverted,GXandInverted,
GXclear, GXandInverted, GXand,	       GXnoop,  GXnoop,	      GXandInverted,
GXclear, GXnor,	        GXandReverse,  GXinvert,GXxor,	      GXandInverted,
GXclear, GXcopyInverted,GXcopy,	       GXset,   GXor,	      GXandInverted,
GXnoop,  GXand,		GXandInverted, GXclear, GXandInverted,GXnoop,
GXnoop,  GXnoop,	GXnoop,	       GXnoop,  GXnoop,	      GXnoop,
GXnoop,  GXequiv,	GXxor,	       GXinvert,GXxor,	      GXnoop,
GXnoop,  GXorInverted,	GXor,	       GXset,   GXor,	      GXnoop,
GXinvert,GXandReverse,  GXnor,	       GXclear, GXandInverted,GXxor,
GXinvert,GXxor,		GXequiv,       GXnoop,  GXnoop,	      GXxor,
GXinvert,GXinvert,	GXinvert,      GXinvert,GXxor,	      GXxor,
GXinvert,GXnand,	GXorReverse,   GXset,   GXor,	      GXxor,
GXset, 	 GXcopy,	GXcopyInverted,GXclear, GXandInverted,GXor,
GXset, 	 GXor,		GXorInverted,  GXnoop,  GXnoop,	      GXor,
GXset, 	 GXorReverse,	GXnand,	       GXinvert,GXxor,	      GXor,
GXset, 	 GXset,		GXset,	       GXset,	GXor,	      GXor
};

#define IOMAP_BASE 0xb00000
static unsigned char *iomapBase;
extern int Rtprio;

MultiScreenRec multiScreenTable[MAXSCREENS];
fbFd 	  sunFbs[MAXSCREENS];

static PixmapFormatRec	formats[] = {
    1, 1, BITMAP_SCANLINE_PAD,	/* 1-bit deep */
    8, 8, BITMAP_SCANLINE_PAD,	/* 8-bit deep */
};
#define NUMFORMATS	(sizeof formats)/(sizeof formats[0])

/*-
 *-----------------------------------------------------------------------
 * InitOutput --
 *	Initialize screenInfo for all actually accessible framebuffers.
 *	The
 *
 * Results:
 *	screenInfo init proc field set
 *
 * Side Effects:
 *	None
 *
 *-----------------------------------------------------------------------
 */

InitOutput(pScreenInfo, argc, argv)
    ScreenInfo 	  *pScreenInfo;
    int     	  argc;
    char    	  **argv;
{
    int i;
    FILE *in;
    int numScreens;
    int position[MAXSCREENS];
    static firstTime = 1;
    static int oldNumScreens;
    static ScreenTableRec *oldFoundScreens[MAXSCREENS];
    char *dispaddr, *getenv();
    ScreenTableRec *s, *FindScreen();

    if (firstTime) firstTime = 0;
    else {
	/* Re-register the screens we've already found */
	for ( i = 0; i < oldNumScreens; i++) {
	    s = oldFoundScreens[i];
	    AddScreen(s->InitScreen, argc, argv);
	    pScreenInfo->screen[i].CloseScreen = s->CloseScreen;
	}
	goto skip_search;
    }

    ErrorfOn++;

    if (pScreenInfo->numScreens == pScreenInfo->arraySize)
    {
        int oldSize = pScreenInfo->arraySize;
	pScreenInfo->arraySize += 5;
	pScreenInfo->screen = (ScreenPtr)Xrealloc(
	    pScreenInfo->screen, 
	    pScreenInfo->arraySize * sizeof(ScreenRec));
    }

    iomapBase = (unsigned char *) IOMAP_BASE;
    pScreenInfo->imageByteOrder = IMAGE_BYTE_ORDER;
    pScreenInfo->bitmapScanlineUnit = BITMAP_SCANLINE_UNIT;
    pScreenInfo->bitmapScanlinePad = BITMAP_SCANLINE_PAD;
    pScreenInfo->bitmapBitOrder = BITMAP_BIT_ORDER;

    if (dispaddr = getenv("SB_DISPLAY_ADDR"))
	iomapBase = (unsigned char *) strtol(dispaddr, (char **)NULL, 0);

    pScreenInfo->numPixmapFormats = NUMFORMATS;
    for (i=0; i< NUMFORMATS; i++)
    {
	pScreenInfo->formats[i] = formats[i];
    }

    sprintf(xscreens,"/usr/lib/X11/X%sscreens",display);

    if (NULL == (in = fopen(xscreens, "r")))
	{
	perror(xscreens);
        FatalError("Can't open screen configuration file.\n");
        }

    for ( i = 0; i < MAXSCREENS; i++ )
	position[i] = -1;

    numScreens = 0;
    for ( i = 0; i < MAXSCREENS && ! feof(in); i++ )
    {
	char *argv[MAXARG];
	int argc;
	int screenIndex;

	while (!(argc = ReadLine(in, argv)))	/** handle blank lines **/
	    ;

	if (argc == -1)				/* eof */
	    break;

	if ( argc < 0 )
	{
	    ErrorF("InitOutput: %s: line %d: Too many fields.\n",
		xscreens, i+1);
	    goto fatal_error;
	}
	
	if ( argc < 3 )
	{
	    ErrorF("InitOutput: %s: line %d: Too few fields.\n", xscreens, i+1);
	    goto fatal_error;
	}
        if ( !sscanf(argv[1],"%d", &screenIndex) )
	{
	    ErrorF("InitOutput: %s: line %d: Bad index %d.\n",
		xscreens, i+1, argv[1]);
	    goto fatal_error;
	}

	if ( screenIndex < 0 || screenIndex >= MAXSCREENS )
	{
	    ErrorF("InitOutput: %s: line %d: Screen index %d out of range.\n",
		xscreens, i+1, screenIndex);
	    goto fatal_error;
	}

	if (position[screenIndex] != -1)
	{
	    ErrorF("InitOutput: %s: line %d: Screen %d multiply defined.\n",
		xscreens, i+1, screenIndex);
	    goto fatal_error;
	}
	position[screenIndex] = i;

	if ( (s = FindScreen(argv[0])) == NULL )
	{
	    ErrorF("InitOutput: %s: line %d: Unknown screen %s.\n",
		xscreens, i+1, argv[0]);
	    goto fatal_error;
	} 
	if ( FALSE == (s->InfoScreen)(screenIndex, argv, argc) )
	{
	    ErrorF("InitOutput: %s: line %d: Couldn't find this screen.\n",
		xscreens, i+1);
	    goto fatal_error;
	}

	pScreenInfo->screen[screenIndex].CloseScreen = s->CloseScreen;

	/* Keep track of this screen for re-registration later */
	oldFoundScreens[screenIndex] = s;

	numScreens++;
    }

    if ( i >= MAXSCREENS && ! feof(in) )
    {
	ErrorF("InitOutput: %s: line %d: Too many screens.  MAXSCREENS = %d\n",
	    xscreens, i+1, MAXSCREENS);
	goto fatal_error;
    }
    fclose(in);
    
    /* Check if all the screens from 0..numScreens - 1 have been defined */

    for ( i = 0; i < numScreens; i++ )
	if ( position[i] == -1 ) {
	    ErrorF("InitOutput: %s: Screen %d not defined.\n",
		   xscreens, i);
	    goto fatal_error;
	}

    /** now that we're sure we've got valid data, do an AddScreen on each **/

    for (i=0; i<numScreens; i++) {
	s = oldFoundScreens[i];
	AddScreen(s->InitScreen, argc, argv);
    }

    /* Setup multiScreenTable */
    for ( i = 0; i < numScreens; i++ )
    {
	int left, here, right;
	here = position[i];
	multiScreenTable[here].position = i;
	multiScreenTable[here].left = ( i == 0 ? numScreens : i) - 1;
	multiScreenTable[here].right = 	( i == numScreens - 1 ) ? 0 : i + 1;
    }

    oldNumScreens = numScreens; /* Save for re-initialization time */
    
    ErrorfOn--;

  skip_search:

    sunInitCursor();

    if (Rtprio)
	if (rtprio(0,Rtprio) == -1)
	    ErrorF("rtprio failed.\n");

    return; /* What should we return? */

    fatal_error:
    fclose(in);
    ErrorF("InitOutput: Couldn't initialize screens.\n");
    exit(1);
}

int
ReadLine(in, argv)
    FILE *in;
    char **argv;
{
    int i, argc;
    static char line[MAXSTRING];
    char *s;
    int state;

    if (NULL == fgets(line, MAXSTRING, in)) return(-1);

    for ( state = argc = 0, s = line; argc < MAXARG; s++)
    {
	switch ( *s )
	{
	    case '#':
	    case '\n':
		*s = '\0';
	    case '\0':
		return (argc);
	    case ' ':
	    case '\t':
		state = 0;
		*s = '\0';
		break;
	    default:
		if ( state == 0 )
		{
		    state = 1;
		    argv[argc++] = s;
		}
		break;
	}
    }
    return(-2);
}

ScreenTableRec *FindScreen(name)
    char *name;
{
    ScreenTableRec *s;
    char *ch;

    /** downshift the names **/
    for (ch=name; *ch; ch++)
	if (isupper(*ch))
	    *ch = tolower(*ch);

    for ( s = screenTable; s->productNumber != NULL; s++)
    {
	if ( strcmp(s->productNumber, name) == 0 ||
	    strcmp(s->productNickname, name) == 0)
	{
	    return (s);
	}
    }
    return (NULL);
}

unsigned char *FrameBufferBase(size)
    long int size;
{
    unsigned char *base = iomapBase;

    /* Round size to a 4K page */
    size = (size + 4095) & 0xfffff000;
    iomapBase += size;
    return ( base );
}


/*
 * The following routine was taken from server/ddx/sun/sunInit.c
 */

/*-
 *-----------------------------------------------------------------------
 * sunScreenInit --
 *	Things which must be done for all types of frame buffers...
 *	Should be called last of all.
 *
 * Results:
 *	None.
 *
 * Side Effects:
 *      The screen's video is forced on.
 *	The graphics context for the screen is created...
 *
 *-----------------------------------------------------------------------
 */
void
sunScreenInit (pScreen)
    ScreenPtr	  pScreen;
{
    fbFd  	*fb;
    DrawablePtr	pDrawable;

    fb = &sunFbs[pScreen->myNum];

    /*
     * Prepare the GC for cursor functions on this screen.
     * Do this before setting interceptions to avoid looping when
     * putting down the cursor...
     */
    pDrawable = (DrawablePtr)
                    (((cfbPrivScreenPtr)(pScreen->devPrivate))->pDrawable);

    fb->pGC = (GC *)CreateScratchGC (pDrawable->pScreen, pDrawable->depth);

    /*
     * By setting graphicsExposures false, we prevent any expose events
     * from being generated in the CopyArea requests used by the cursor
     * routines.
     */
    fb->pGC->graphicsExposures = FALSE;

    /*
     * Preserve the "regular" functions
     */
    fb->CreateGC =	    	    	pScreen->CreateGC;
    fb->CreateWindow = 	    	    	pScreen->CreateWindow;
    fb->ChangeWindowAttributes =    	pScreen->ChangeWindowAttributes;
    fb->GetImage =	    	    	pScreen->GetImage;
    fb->GetSpans =			pScreen->GetSpans;

    /*
     * Interceptions
     */
    pScreen->CreateGC =	    	    	sunCreateGC;
    pScreen->CreateWindow = 	    	sunCreateWindow;
    pScreen->ChangeWindowAttributes = 	sunChangeWindowAttributes;
    pScreen->GetImage =	    	    	sunGetImage;
    pScreen->GetSpans =			sunGetSpans;

    /*
     * Cursor functions
     */
    pScreen->RealizeCursor = 	    	sunRealizeCursor;
    pScreen->UnrealizeCursor =	    	sunUnrealizeCursor;
    pScreen->DisplayCursor = 	    	sunDisplayCursor;
    pScreen->SetCursorPosition =    	sunSetCursorPosition;
    pScreen->CursorLimits = 	    	sunCursorLimits;
    pScreen->PointerNonInterestBox = 	sunPointerNonInterestBox;
    pScreen->ConstrainCursor = 	    	sunConstrainCursor;
    pScreen->RecolorCursor = 	    	sunRecolorCursor;

}

/*
 * Ensure video is disabled when server exits....
 */
Bool
hpCloseScreen(i, pScreen)
    int		i;
    ScreenPtr	pScreen;
{
  
    if (pScreen->SaveScreen)
    {
	return ((*pScreen->SaveScreen)(pScreen, SCREEN_SAVER_OFF));
    }
    else
	return TRUE;
}

