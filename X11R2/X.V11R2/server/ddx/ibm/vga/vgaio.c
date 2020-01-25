/***********************************************************
		Copyright IBM Corporation 1988

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

/* $Header:vgaio.c 1.5$ */
/* $ACIS:vgaio.c 1.5$ */
/* $Source: /ibm/acis/usr/src/X11/server/ddx/ibm/vga/RCS/vgaio.c,v $ */

#ifndef lint
static char *rcsid = "$Header:vgaio.c 1.5$";
#endif

#include <stdio.h>
#include <sys/types.h>
#include <sys/file.h>
#include <machinecons/xio.h>
#include <machinecons/buf_emul.h>

#include "vga_video.h"

#include "X.h"
#include "Xproto.h"
#include "scrnintstr.h"
#include "miscstruct.h"
#include "misc.h"
#include "colormap.h"
#include "colormapst.h"
#include "cursorstr.h"
#include "resource.h"

#include "ppcprocs.h"
#include "vgaprocs.h"

#include "rtcursor.h"
#include "rtinit.h"
#include "rtio.h"
#include "rtscreen.h"
#include "rtutils.h"

extern	int		open() ;
extern	int		close() ;

extern	void		AllocColor() ;
extern	void		ErrorF() ;
extern	pointer		LookupID() ;

extern	void		miRecolorCursor() ;

extern	int		vgaScreenInitHW() ;
extern	Bool		vgaInitScreen() ;
extern	void		vgaCursorInit() ;
extern	void		vgaCloseHW() ;

#define SERVER_ID 0

#ifdef DEBUG
/* Include Helpful Procedures For Debugging */
extern int inbyte(), outbyte() ;
static int (*randomjunk[])() = { inbyte, outbyte } ;
#endif

/* Global Variable */
xColorItem	screenWhite, screenBlack ;
int 		vgaNumberOfPlanes  = 0 ;
int		vgaDisplayTubeType = 0 ;

static
void getBW( pCmap )
register ColormapPtr const pCmap ;
{
unsigned short r, g, b ;
int pix ;

r  = g  = b  = 0 ;
AllocColor( pCmap, &r, &g, &b, &pix, SERVER_ID ) ;
TRACE( ( "getBW: black is pixel number %d\n", pix ) ) ;

r  = g  = b  = 0xffff ;
AllocColor( pCmap, &r, &g, &b, &pix, SERVER_ID ) ;
TRACE( ( "getBW: white is pixel number %d\n", pix ) ) ;
return ;
}

PixmapFormatRec	vgaFormats[] = { { 4, 8, 32 } } ;

int vgaCheckDisplay( fd )
register int fd ; /* Device File Decriptor */
{
static unsigned long int infoword ;

return ( ioctl( fd, BUFDISPINFO, &infoword ) < 0 ) ? -1 : ( infoword & 3 ) ;
}

int
vgaProbe()
{
register int fd ;

    TRACE( ( "vgaProbe()\n" ) ) ;
    if ( ( fd = open( "/dev/vga", O_RDWR | O_NDELAY ) ) < 0 ) {
	return FALSE ;
    }
    if ( ( vgaDisplayTubeType = vgaCheckDisplay( fd ) ) <= 0 ) {
	(void) close( fd ) ;
	return FALSE ;
    }
    (void) close( fd ) ;
    ErrorF( ( vgaDisplayTubeType == COLOR_TUBE )
      ? "Found a vga with color display.\n" : 
	"Found a vga with gray scale display.\n" ) ;

    return TRUE ;
}

static int HardwareReady = 0 ;

Bool
vgaScreenClose( index, pScreen )
register const int index;
register ScreenPtr const pScreen ;
{
    TRACE( ( "vgaScreenClose( index= %d, pScreen= 0x%x )\n", index, pScreen ));
    if ( HardwareReady ) {
	vgaCloseHW( index ) ;
	HardwareReady = 0 ;
    }
    if ( pScreen->devPrivate )
	Xfree( pScreen->devPrivate ) ;

    return TRUE ;
}

Bool
vgaScreenInit( index, pScreen, argc, argv )
register const int index ;
register ScreenPtr const pScreen ;
register int const argc ;		/* these two may NOT be changed */
register char * const * const argv ;
{
    static int been_here = 0 ;
    ColormapPtr pColormap ;
    register int retval ;

    TRACE(
	( "vgaScreenInit( index= %d, pScreen= 0x%x, argc= %d, argv= 0x%x )\n",
		index, pScreen, argc, argv ) ) ;

    if ( !been_here ) {
/*	vgaInitFontCache() ;	MAYBE SOMEDAY */
	been_here = TRUE ;
    }

    if ( !HardwareReady ) {
	vgaNumberOfPlanes  = vgaScreenInitHW( index ) ;
	HardwareReady      = 1 ;
    }

    retval = vgaInitScreen( index, pScreen, 0x80000000, 640, 480, 80, 80 ) ;

    pScreen->CloseScreen =		vgaScreenClose ;
    pScreen->SaveScreen =		rtSaveScreen ;
    pScreen->RealizeCursor =		ppcRealizeCursor ;
    pScreen->UnrealizeCursor =		ppcUnrealizeCursor ;
    pScreen->DisplayCursor =		vgaDisplayCursor ;
    pScreen->SetCursorPosition =	rtSetCursorPosition ;
    pScreen->CursorLimits =		rtCursorLimits ;
    pScreen->PointerNonInterestBox =	rtPointerNonInterestBox ;
    pScreen->ConstrainCursor =		rtConstrainCursor ;
    pScreen->RecolorCursor =		miRecolorCursor ;
    pScreen->QueryBestSize =		rtQueryBestSize ;
    pScreen->CreateGC =			vgaCreateGC ;
    pScreen->CreateWindow =		ppcCreateWindow ;
    pScreen->ChangeWindowAttributes =	ppcChangeWindowAttributes ;

    CreateColormap( pScreen->defColormap, pScreen,
		    LookupID( pScreen->rootVisual, RT_VISUALID, RC_CORE ),
		    &pColormap, AllocNone, 0 ) ;
    ppcDefineDefaultColormapColors( 
			LookupID( pScreen->rootVisual, RT_VISUALID, RC_CORE ),
			pColormap ) ;
    pColormap->pScreen  = pScreen ;
    ppcInstallColormap( pColormap ) ;
    getBW( pColormap ) ;

    vgaCursorInit( index ) ;
    rtScreen( index ) = pScreen ;

    return retval ;
}
