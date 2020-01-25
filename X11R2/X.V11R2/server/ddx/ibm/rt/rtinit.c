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
/* $Header:rtinit.c 6.0$ */
/* $ACIS:rtinit.c 6.0$ */
/* $Source: /vice/X11/src/server/ddx/ibm/rt/RCS/rtinit.c,v $ */

#ifndef lint
static char *rcsid = "$Header:rtinit.c 6.0$";
#endif


/* (ef) 4/11/87 -- to get definition of xEvent from Xproto.h */
#define NEED_EVENTS

#include <sys/types.h>
#include <sys/file.h>

#include "X.h"
#include "Xproto.h"
#include "misc.h"
#include "miscstruct.h"
#include "input.h"
#include "scrnintstr.h"
#include "cursorstr.h"

#include "rtinit.h"
#include "rtscreen.h"
#include "rtkeyboard.h"
#include "rtmouse.h"
#include "rtutils.h"

#include "servermd.h"

#define MOTION_BUFFER_SIZE 0
#define NUMDEVICES 2

static	int	rtOpenAllScreens=	FALSE;
static	int	rtScreensWanted=	0;
static	int	rtBadArgument=		FALSE;

static	Bool
rtFormatExists(screenInfo,newFmt)
    ScreenInfo		*screenInfo;
    PixmapFormatPtr	newFmt;
{
    PixmapFormatPtr	oldFmt;
    int			ndx;

    TRACE(("rtFormatExisits(screenInfo= 0x%x,newFmt=0x%x(%d,%d,%d))\n",
				     screenInfo,newFmt,newFmt->depth,
				     newFmt->bitsPerPixel,newFmt->scanlinePad));
    for (ndx=0;ndx<screenInfo->numPixmapFormats;ndx++) {
	oldFmt= &screenInfo->formats[ndx];
	if ((newFmt->depth==oldFmt->depth)&&
	    (newFmt->bitsPerPixel==oldFmt->bitsPerPixel)&&
	    (newFmt->scanlinePad==oldFmt->scanlinePad)) {
		return TRUE;
	}
    }
    return FALSE;
}

/***==================================================================***/

static	int
rtFindSomeScreens()
{
int	ndx;

    TRACE(("rtFindAScreen()\n"));
    if (rtNumScreens==0) {
	ndx=0;
	while (rtPossibleScreens[ndx].rt_ScreenFlag) {
	    if ((*rtPossibleScreens[ndx].rt_ProbeFunc)()) {
		rtScreens[rtNumScreens++]= &rtPossibleScreens[ndx];
		if (!rtOpenAllScreens)
		    return(TRUE);
	    }
	    ndx++;
	}
    }
    else return(TRUE);

    if ((rtOpenAllScreens)&&(rtNumScreens>0))
	return(TRUE);
    return(FALSE);
}

/***==================================================================***/

static	void
rtAddScreens(screenInfo, argc, argv)
    ScreenInfo		*screenInfo;
    int			 argc;
    char		*argv[];
{
    PixmapFormatPtr	newFmt,oldFmt;
    int			 ndx,fmtNdx;
    static int		 been_here;

    TRACE(("rtAddScreens(screenInfo= 0x%x, argc= %d, argv]=x%x)\n", 
							screenInfo,argc,argv));

    for (ndx=0;ndx<rtNumScreens;ndx++) {
	if (!been_here) {
	    if (rtScreens[ndx]->rt_Wanted) {
		ErrorF("Multiple requests for screen '%s'  -- ignored\n",
							rtScreenFlag(ndx));
		continue;
	    }
	    rtScreens[ndx]->rt_Wanted= TRUE;
	    if (ndx>0) {
		rtScreenMinX(ndx)+= rtScreenMaxX(ndx-1);
		rtScreenMaxX(ndx)+= rtScreenMaxX(ndx-1);
	    }
	}

	for (fmtNdx=0;fmtNdx<rtNumFormats(ndx);fmtNdx++) {
	    if (!rtFormatExists(screenInfo,&rtScreenFormats(ndx)[fmtNdx])) {
		newFmt= &rtScreenFormats(ndx)[fmtNdx];
		oldFmt= &screenInfo->formats[screenInfo->numPixmapFormats++];
		oldFmt->depth= 		newFmt->depth;
		oldFmt->bitsPerPixel=	newFmt->bitsPerPixel;
		oldFmt->scanlinePad=	newFmt->scanlinePad;
		if (screenInfo->numPixmapFormats>MAXFORMATS) {
		    ErrorF("WSGO!! Too many formats! Exiting\n");
		    exit(1);
		}
	    }
	}
	AddScreen(rtScreenInit(ndx),argc,argv);
    }
    been_here= TRUE;
}

/***==================================================================***/

int
InitOutput(screenInfo, argc, argv)
    ScreenInfo	*screenInfo;
    int		 argc;
    char	*argv[];
{
    static	int	been_here;
    TRACE(("InitOutput( screenInfo= 0x%x)\n",screenInfo));

    screenInfo->imageByteOrder = IMAGE_BYTE_ORDER;
    screenInfo->bitmapScanlineUnit = BITMAP_SCANLINE_UNIT;
    screenInfo->bitmapScanlinePad = BITMAP_SCANLINE_PAD;
    screenInfo->bitmapBitOrder = BITMAP_BIT_ORDER;

    screenInfo->numPixmapFormats = 1;
    screenInfo->formats[0].depth= 1;
    screenInfo->formats[0].bitsPerPixel= 1;
    screenInfo->formats[0].scanlinePad= BITMAP_SCANLINE_PAD;

    if (!been_here) {
	been_here= TRUE;
	rtScreens[0]= NULL;
    	rtNumScreens= 0;
	rtScreensWanted= 0;
	open("/dev/bus",O_RDWR);
	rtProcessCommandLine(argc,argv);
	if (rtBadArgument)		
	    rtPrintUsage(argv[0],NULL);
	else if (rtNumScreens!=rtScreensWanted) 
	    rtPrintUsage(argv[0],"Couldn't open all requested screens.");
	else if ((rtNumScreens==0)&&(!rtFindSomeScreens())) 
	    rtPrintUsage(argv[0],"Couldn't open any screens.");
    }
    rtAddScreens(screenInfo,argc,argv);
    rtInitEmulator();
}

/***==================================================================***/

static DevicePtr keyboard;
static DevicePtr mouse;

InitInput()
{
    TRACE(("InitInput()\n"));

    mouse=	AddInputDevice(rtMouseProc,	TRUE);
    keyboard=	AddInputDevice(rtKeybdProc,	TRUE);

    RegisterPointerDevice( mouse, MOTION_BUFFER_SIZE );
    RegisterKeyboardDevice( keyboard );
}

/***==================================================================***/

int
rtProcessCommandLine(argc,argv)
int	argc;
char	*argv[];
{
int	i,tmp;
char	*keybd;
extern	char *getenv();
extern	char *rtArenaFile;
static	int	been_here= FALSE;
rtPerScreenInfo	*scr;

    TRACE(("rtProcessCommandLine( argc= %d, argv= 0x%x )\n",argc,argv));

    if (been_here)	return;
    been_here= TRUE;

    for (i=1;i<argc;i++) {
	if	( strcmp( argv[i], "-pckeys" ) == 0 )	keybd= "pckeys";
	else if ( strcmp( argv[i], "-rtkeys" ) == 0 )	keybd= "rtkeys"; 
	else if ( strcmp( argv[i], "-wrapx"  ) == 0 )	rtXWrapScreen= TRUE;
	else if ( strcmp( argv[i], "-wrapy"  ) == 0 )	rtYWrapScreen= TRUE;
	else if ( strcmp( argv[i], "-wrap"  ) == 0 )	{
	    rtXWrapScreen= rtYWrapScreen= TRUE;
	}
#ifdef TRACE_X
	else if ( strcmp( argv[i], "-trace"  ) == 0 )	rtTrace= TRUE;
#endif
#ifndef RT_MUST_USE_HDWR
	else if ( strcmp( argv[i], "-nohdwr" ) == 0 )	rtUseHardware= FALSE;
#endif RT_MUST_USE_HDWR
#ifdef RT_SPECIAL_MALLOC
	else if ( strcmp( argv[i], "-malloc" ) == 0 )	{
		int lvl= atoi(argv[++i]);
		SetMallocCheckLevel(lvl);
		ErrorF("allocator check level set to %d...\n",lvl);
	}
	else if ( strcmp( argv[i], "-plumber" ) == 0 ) {
		rtSetupPlumber(argv[++i]);
	}
#endif RT_SPECIAL_MALLOC
	else if ( strcmp( argv[i], "-all" ) == 0 )	rtOpenAllScreens= TRUE;
	else {
		int found= FALSE;
		scr= &rtPossibleScreens[0];
		while ((scr->rt_ScreenFlag)&&(!found)) {
	    	    if (!strcmp(argv[i],scr->rt_ScreenFlag)) {
			rtScreensWanted++;
			if ((*(scr->rt_ProbeFunc))())
			    rtScreens[rtNumScreens++]= scr;
			else 
			    ErrorF("%s not available\n",&argv[i][1]);
			found= TRUE;
		    }
		    scr++;
		}
		if (!found) {
		    if ((tmp=rtDIXArgument(argv[i]))==0) {
			if (i<argc-1) {
			    ErrorF("Unknown argument %s\n",argv[i]);
		            rtBadArgument++;
			}
		    }
		    else i+= tmp-1;
		}
	}
    }

    if (keybd) {
	if	(strcmp(keybd,"pckeys")==0)	rtUsePCKeyboard();
	else if	(strcmp(keybd,"rtkeys")==0)	rtUseRTKeyboard();
    }
}



