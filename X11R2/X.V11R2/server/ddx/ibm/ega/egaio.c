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

/* $Header:egaio.c 1.2$ */
/* $ACIS:egaio.c 1.2$ */
/* $Source: /ibm/acis/usr/src/X11/server/ddx/ibm/ega/RCS/egaio.c,v $ */

#ifndef lint
static char *rcsid = "$Header:egaio.c 1.2$";
#endif


#include <stdio.h>
#include <sys/types.h>
#include <sys/file.h>
#include <machinecons/xio.h>
#include <machinecons/buf_emul.h>

#include "ega_video.h"

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
#include "egaprocs.h"

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

extern	int		egaScreenInitHW() ;
extern	Bool		egaInitScreen() ;
extern	void		egaCursorInit() ;
extern	void		egaCloseHW() ;

#define SERVER_ID 0

#ifdef DEBUG
/* Include Helpful Procedures For Debugging */
extern int rt_inbyte(), rt_outbyte() ;
static int (*randomjunk[])() = { rt_inbyte, rt_outbyte } ;
#endif

/* Global Variable */
xColorItem	screenWhite, screenBlack ;
int 		egaNumberOfPlanes  = 0 ;
int		egaSwitchSettings = 0 ;

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

PixmapFormatRec	egaFormats[] = { { 4, 8, 32 } } ;

int egaCheckDisplay( fd )
register int fd ; /* Device File Decriptor */
{
static unsigned long int infoword ;

return ( ioctl( fd, BUFDISPINFO, &infoword ) < 0 ) ? -1 : ( infoword & 0xF ) ;
}

int
egaProbe()
{
register int fd ;

    TRACE( ( "egaProbe()\n" ) ) ;
    if ( ( fd = open( "/dev/ega", O_RDWR | O_NDELAY ) ) < 0 ) {
	return FALSE ;
    }
    if ( ( egaSwitchSettings = egaCheckDisplay( fd ) ) <= 0 ) {
	(void) close( fd ) ;
	return FALSE ;
    }
    (void) close( fd ) ;
    ErrorF( "Found an ega\n" ) ;

    return TRUE ;
}

static int HardwareReady = 0 ;

Bool
egaScreenClose( index, pScreen )
register const int index;
register ScreenPtr const pScreen ;
{
    TRACE( ( "egaScreenClose( index= %d, pScreen= 0x%x )\n", index, pScreen ));
    if ( HardwareReady ) {
	egaCloseHW( index ) ;
	HardwareReady = 0 ;
    }
    if ( pScreen->devPrivate )
	Xfree( pScreen->devPrivate ) ;

    return TRUE ;
}

Bool
egaScreenInit( index, pScreen, argc, argv )
register const int index ;
register ScreenPtr const pScreen ;
register int const argc ;		/* these two may NOT be changed */
register char * const * const argv ;
{
    ColormapPtr pColormap ;
    register int retval ;

    TRACE(
	( "egaScreenInit( index= %d, pScreen= 0x%x, argc= %d, argv= 0x%x )\n",
		index, pScreen, argc, argv ) ) ;

    if ( !HardwareReady ) {
	egaNumberOfPlanes  = egaScreenInitHW( index ) ;
	HardwareReady      = 1 ;
    }

    retval = egaInitScreen( index, pScreen, 0x80000000, 640, 350, 80, 60 ) ;

    pScreen->CloseScreen =		egaScreenClose ;
    pScreen->SaveScreen =		rtSaveScreen ;
    pScreen->RealizeCursor =		ppcRealizeCursor ;
    pScreen->UnrealizeCursor =		ppcUnrealizeCursor ;
    pScreen->DisplayCursor =		egaDisplayCursor ;
    pScreen->SetCursorPosition =	rtSetCursorPosition ;
    pScreen->CursorLimits =		rtCursorLimits ;
    pScreen->PointerNonInterestBox =	rtPointerNonInterestBox ;
    pScreen->ConstrainCursor =		rtConstrainCursor ;
    pScreen->RecolorCursor =		miRecolorCursor ;
    pScreen->QueryBestSize =		rtQueryBestSize ;
    pScreen->CreateGC =			egaCreateGC ;
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

    egaCursorInit( index ) ;
    rtScreen( index ) = pScreen ;

    return retval ;
}
