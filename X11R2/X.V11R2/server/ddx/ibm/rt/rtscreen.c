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

/* $Header:rtscreen.c 6.0$ */
/* $ACIS:rtscreen.c 6.0$ */
/* $Source: /vice/X11/src/server/ddx/ibm/rt/RCS/rtscreen.c,v $ */

#ifndef lint
static char *rcsid = "$Header:rtscreen.c 6.0$";
#endif

#include <stdio.h>
#include <sys/types.h>
#include <sys/file.h>
#include <machinecons/xio.h>

#include "X.h"
#include "Xproto.h"
#include "misc.h"
#include "miscstruct.h"
#include "input.h"
#include "scrnintstr.h"
#include "cursorstr.h"

#include "rtdecls.h"

extern	Bool	apa16ScreenInit();
extern	Bool	apa16Probe();
extern	void	apa16HideCursor();

extern	Bool	aedScreenInit();
extern	Bool	aedProbe();
extern	void	aedHideCursor();

extern	Bool	mpelScreenInit();
extern	Bool	mpelProbe();
extern	void	mpelHideCursor();
extern	PixmapFormatRec	mpelFormats[];

extern	Bool	ibm8514ScreenInit();
extern	Bool	ibm8514Probe();
extern	void	ibm8514RemoveCursor();
extern	PixmapFormatRec  ibm8514Formats[];

extern	Bool	vgaScreenInit();
extern	Bool	vgaProbe();
extern	void	vgaRemoveCursor();
extern	PixmapFormatRec  vgaFormats[];

extern	Bool	egaScreenInit();
extern	Bool	egaProbe();
extern	void	egaRemoveCursor();
extern	PixmapFormatRec  egaFormats[];

rtPerScreenInfo	rtPossibleScreens[] = {
#ifdef RT_8514
	{ { 0, 0, 1023, 767 },
	  1,	ibm8514Formats,
	  ibm8514ScreenInit,	ibm8514Probe,	ibm8514RemoveCursor,
	  "-8514",	"/dev/ibm8514",	"/dev/ms8514" },
#endif /* RT_8514 */
#ifdef RT_VGA
	{ { 0, 0, 639, 479 },		/* If Found VGA is First Screen */
	  1,	vgaFormats,
	  vgaScreenInit,	vgaProbe,	vgaRemoveCursor,
	  "-vga",	"/dev/vga",	"/dev/msvga"},
#endif /* RT_VGA */
#ifdef RT_MPEL
	{ { 0, 0, 1023, 1023 },
	  1,	mpelFormats,
	  mpelScreenInit,	mpelProbe,	mpelHideCursor,
	  "-mpel",	"/dev/mpel",	"/dev/msmpel" },
#endif /* RT_MPEL */
#ifdef RT_EGA
	{ { 0, 0, 639, 349 },		/* If Found EGA is Last Screen */
	  1,	egaFormats,
	  egaScreenInit,	egaProbe,	egaRemoveCursor,
	  "-ega",	"/dev/ega",	"/dev/msega"},
#endif /* RT_EGA */
#ifdef RT_APA16
	{ { 0, 0, 1023, 767 },
	  0,	NULL,
	  apa16ScreenInit,	apa16Probe,	apa16HideCursor,
	  "-apa16",	"/dev/apa16",	"/dev/msapa16" },
#endif /* RT_APA16 */
#ifdef RT_AED
	{ { 0, 0, 1023, 799 },
	  0,	NULL,
	  aedScreenInit,	aedProbe,	aedHideCursor,
	  "-aed",	"/dev/aed",	"/dev/msaed"},
#endif /* RT_AED */
	{ { 0, 0, 0, 0}, NULL, NULL, NULL, NULL }
};

rtPerScreenInfo	*rtScreens[MAXSCREENS];
int		 rtNumScreens=	0;
int		 rtXWrapScreen= FALSE;
int		 rtYWrapScreen= FALSE;
int		 rtCurrentScreen= 0;
int		 rtUseHardware= TRUE;
int		 rtEmulatorFD=	-1;

/***==================================================================***/

rtSaveScreen( pScreen, on )
    ScreenPtr	pScreen;
    int		on;
{
    TRACE(("rtSaveScreen( pScreen= 0x%x, on= %d )\n",pScreen,on));

    if ( on == SCREEN_SAVER_FORCER ) {
	lastEventTime = GetTimeInMillis();
	return TRUE;
    }
    else
	return FALSE;
}

/***==================================================================***/

void
rtInitEmulator()
{
static int been_here= 0;
int	emulator= E_XINPUT;
struct	x_screen_size bounds;
int	scr;
int	miny,maxy;

    TRACE(("rtInitEmulator()"));
    if (rtNumScreens<=0) {
	ErrorF("WSGO! No open screens in rtInitEmulator.\nExiting.\n");
	exit(1);
    }
    if (!been_here) {
	been_here= 1;
	rtEmulatorFD= rtScreenFD(0);
	if (rtEmulatorFD<0) {
	    ErrorF("WSGO! ScreenFD 0 not opened!\nExiting.\n");
	    exit(1);
	}
	ioctl(rtEmulatorFD,EISETD,&emulator);
	ioctl(rtEmulatorFD,QIOCHIDECUR);
	ioctl(rtEmulatorFD,QIOCADDR,&rtXaddr);
	rtXaddr->mbox.bottom= rtXaddr->mbox.top= 
	rtXaddr->mbox.left= rtXaddr->mbox.right= 10000;
	rtQueue= (XEventQueue *)(&rtXaddr->ibuff);

	miny= rtScreenMinY(0);
	maxy= rtScreenMaxY(0);
	for (scr=1;scr<rtNumScreens;scr++) {
	    if (rtScreenMinY(scr)<miny)	miny= rtScreenMinY(scr);
	    if (rtScreenMaxY(scr)>maxy)	maxy= rtScreenMaxY(scr);
	}

	/*
	 * If WrapScreen is defined, let the cursor leave the bounds of all
	 * the screens, so we can wrap it.  50 is an arbitrary choice.
	 */
	bounds.x_x_min= rtScreenMinX(0)-(rtXWrapScreen?1:0);
	bounds.x_x_max= rtScreenMaxX(rtNumScreens-1)+(rtXWrapScreen?1:0);
	bounds.x_y_min= miny-(rtYWrapScreen?1:0);
	bounds.x_y_max= maxy+(rtYWrapScreen?1:0);
	if (ioctl(rtEmulatorFD,QIOCSETSIZE,&bounds)==-1) {
	    perror("couldn't set bounds:");
	    ErrorF("Exiting\n");
	    exit(1);
	}
    }
    return;
}
