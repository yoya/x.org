/*
 * PRPQ 5799-WZQ (C) COPYRIGHT IBM CORPORATION 1987,1988
 * LICENSED MATERIALS - PROPERTY OF IBM
 * REFER TO COPYRIGHT INSTRUCTIONS FORM NUMBER G120-2083
 */
/* $Header:vgainit.c 1.3$ */
/* $ACIS:vgainit.c 1.3$ */
/* $Source: /ibm/acis/usr/src/X11/server/ddx/ibm/vga/RCS/vgainit.c,v $ */

#ifndef lint
static char *rcsid = "$Header:vgainit.c 1.3$";
#endif

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

#define NEED_EVENTS

#include "X.h"
#include "Xproto.h"
#include "misc.h"
#include "input.h"
#include "scrnintstr.h"
#include "servermd.h"

#include "rtinit.h"
#include "rtkeyboard.h"
#include "rtmouse.h"
#include "rtutils.h"

#define MOTION_BUFFER_SIZE 0
#define NUMFORMATS 2
#define NUMDEVICES 2

extern void vgaScreenInit() ;

PixmapFormatRec	formats[NUMFORMATS] ;

int
InitOutput( screenInfo, argc, argv )
    ScreenInfo * const screenInfo ;
    const int argc ;
    char * const argv[] ;
{
    register int i ;

    rtTrace = 1 ;
    TRACE( ( "InitOutput( screenInfo= 0x%x)\n", screenInfo ) ) ;

	/* for ppc only!!!!!!! */
	formats[0].depth = 1 ;
	formats[0].bitsPerPixel = 1 ;
	formats[0].scanlinePad = 32 ;
	formats[1].depth = 4 ;
	formats[1].bitsPerPixel = 8 ;
	formats[1].scanlinePad = 32 ;

    rtProcessCommandLine( argc, argv ) ;
    screenInfo->imageByteOrder = IMAGE_BYTE_ORDER ;
    screenInfo->bitmapScanlineUnit = BITMAP_SCANLINE_UNIT ;
    screenInfo->bitmapScanlinePad = BITMAP_SCANLINE_PAD ;
    screenInfo->bitmapBitOrder = BITMAP_BIT_ORDER ;

    for ( i = screenInfo->numPixmapFormats = NUMFORMATS ; i-- ; ) {
	screenInfo->formats[i].depth = formats[i].depth ;
	screenInfo->formats[i].bitsPerPixel = formats[i].bitsPerPixel ;
	screenInfo->formats[i].scanlinePad = formats[i].scanlinePad ;
    }

    AddScreen( vgaScreenInit, argc, argv ) ;
    return 0 ;
}

static DevicePtr keyboard ;
static DevicePtr mouse ;

int InitInput()
{
    TRACE(("InitInput()\n")) ;

    mouse =	AddInputDevice( rtMouseProc, TRUE ) ;
    keyboard =	AddInputDevice( rtKeybdProc, TRUE ) ;
    RegisterPointerDevice( mouse, MOTION_BUFFER_SIZE ) ;
    RegisterKeyboardDevice( keyboard ) ;
    return 0 ;
}

int
rtProcessCommandLine( argc, argv )
const int argc ;
const char * const argv[] ;
{
register int i ;
register char *keybd ;

extern	char *getenv() ;

    TRACE(("rtProcessCommandLine( argc= %d, argv= 0x%x )\n",argc,argv)) ;

    keybd = getenv( "X11_KEYBOARD" ) ;

    for ( i = 1 ; i < argc ; i++ ) {
	if ( !strcmp( argv[i], "-pckeys" ) )
		keybd = "pckeys" ;
	else if ( !strcmp( argv[i], "-rtkeys" ) )
		keybd = "rtkeys" ;
	else if ( !strcmp( argv[i], "-trace"  ) )
		rtTrace = TRUE ;
    }

    if ( keybd ) {
	if ( !strcmp( keybd, "pckeys" ) )
		rtUsePCKeyboard() ;
	else if	( !strcmp( keybd, "rtkeys" ) )
		rtUseRTKeyboard() ;
    }
return 0 ;
}
