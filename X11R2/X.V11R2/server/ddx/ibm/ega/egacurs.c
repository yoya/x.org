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
/* $Header:egacurs.c 1.2$ */
/* $ACIS:egacurs.c 1.2$ */
/* $Source: /ibm/acis/usr/src/X11/server/ddx/ibm/ega/RCS/egacurs.c,v $ */

#ifndef lint
static char *rcsid = "$Header:egacurs.c 1.2$";
#endif

/*
 * Software Cursor implementation for ega
 *
 * Depends on owning some off-screen memory in SAVELOC, etc #defined areas
 */

#include "X.h"
#include "Xmd.h"

#include "miscstruct.h"
#include "scrnintstr.h"
#include "pixmapstr.h"
#include "cursorstr.h"
#include <machinecons/xio.h>
#include "rtio.h"
#include "rtcursor.h"
#include "rtscreen.h"
#include "rtutils.h"

#include "ega_video.h"
#include "egaprocs.h"

#undef TRUE
#undef FALSE
#define TRUE 1
#define FALSE 0

/* CONSTANTS FOR NOW */
#define MAX_CURSOR_WIDTH 32
#define MAX_CURSOR_HEIGHT 32
#define CURSOR_SAVE_ROW ( MAX_OFFSCREEN_ROW - MAX_CURSOR_HEIGHT )
#define CURSOR_SAVE_COL 0

/* Global Semaphore For Low Level ( e.g. Screen Drawing ) Routines */
int egaCursorChecking = 0 ;

/* Static Cursor Variables */
static int cursor_save_x_loc ;
static int cursor_save_y_loc ;
static int cursor_save_width ;
static int cursor_save_height ;

static short c_x ;
static short c_y ;
static unsigned int c_fg = 15 ;
static unsigned int c_bg = 0 ;
static int active = FALSE ;
static int cursor_not_displayed = FALSE ;
static int screen_index ;

static unsigned long int cursorFG[ MAX_CURSOR_HEIGHT ] ;
static int foreground_width  = MAX_CURSOR_WIDTH ;	/* width */
static int foreground_height = MAX_CURSOR_HEIGHT ;	/* height */

static unsigned long int cursorBG[ MAX_CURSOR_HEIGHT ] ;
static int background_width  = MAX_CURSOR_WIDTH ;	/* width */
static int background_height = MAX_CURSOR_HEIGHT ;	/* height */

static void egaScreenDrawCursor( data, w, cursorWidth, h, cursorHeight,
				 x, y, xSrc, ySrc )
register const unsigned char *data ;
register int w ;
register int const cursorWidth;
int const h ;
int const cursorHeight ;
int x ;
int const y ;
int const xSrc ;
int const ySrc ;
{
register unsigned int Maskdata ;
register const unsigned char *cptr ;
register int shift ;
register unsigned SourceRow ;
register int tmp1 ;
register int tmp2 ;
register volatile unsigned char *dst ;
register const unsigned char *lptr ;
register unsigned int paddedByteWidth ;
register int NeedValX ;
register int SavNeedX ;
int xshift ;

/* Figure Bit Offsets & Source Address */
paddedByteWidth = ( ( cursorWidth + 31 ) & ~31 ) >> 3 ;
if ( ( xshift = ( x - xSrc ) ) < 0 )
	xshift += cursorWidth ;
else if ( xshift >= cursorWidth )
	xshift -= cursorWidth ;
if ( ( tmp1 = ( y - ySrc ) ) < 0 ) /* Now Use Tmp1 as Y-Shift */
	tmp1 += cursorHeight ;
else if ( tmp1 >= cursorHeight ) /* Now Use Tmp1 as Y-Shift */
	tmp1 -= cursorHeight ;
if ( tmp1 )
	data += ( tmp1 * paddedByteWidth ) ;

outb( 0x3CE, 8 ) ; /* Point At The Bit Mask Reg */

/* Do Left Edge */
if ( tmp1 = x & 07 ) {

	tmp2 = ( (unsigned) 0xFF ) >> tmp1 ;
	/* Catch The Cases Where The Entire Region Is Within One Byte */
	if ( ( w -= 8 - tmp1 ) < 0 ) {
		tmp2 &= 0xFF << -w ;
		w = 0 ;
	}
	Maskdata = tmp2 ; /* Set The Bit Mask Reg */
	/*
	 * For Each Line In The Cursor Data
	 */
	SourceRow = h ;
	dst = EGABASE + BYTE_OFFSET( x, y ) ;
	if ( ( NeedValX = xshift - tmp1 ) < 0 ) {
		NeedValX += cursorWidth ;
		cptr = data + ( NeedValX >> 3 ) - paddedByteWidth ;
	}
	else
		cptr = data + ( NeedValX >> 3 ) ;
	if ( shift = NeedValX & 7 )
		for ( ;
		      SourceRow-- ;
		      dst += BYTES_PER_ROW, cptr += paddedByteWidth ) {
			/* Read To Save */
			tmp2 = *( (EgaMemoryPtr) dst ) ;
			/* Write Pattern */
			outb( 0x3CF, /* Set The Bit Mask Reg */
	( Maskdata & ( ( *cptr << shift ) | ( cptr[1] >> ( 8 - shift ) ) ) ) ) ;
			*( (EgaMemoryPtr) dst ) = Maskdata ;
		}
	else /* shift == 0 */
		for ( ;
		      SourceRow-- ;
		      dst += BYTES_PER_ROW, cptr += paddedByteWidth ) {
			/* Read To Save */
			tmp2 = *( (EgaMemoryPtr) dst ) ;
			/* Write Pattern */
			outb( 0x3CF, Maskdata & *cptr ) ; /* Set The Bit Mask Reg */
			*( (EgaMemoryPtr) dst ) = Maskdata ;
		}
	x = ( x + 7 ) & ~07 ;
	if ( ( SavNeedX = NeedValX + 8 ) >= cursorWidth )
		SavNeedX -= cursorWidth ;
}
else {
	SavNeedX = xshift ;
}
shift = SavNeedX & 0x7 ;

/* Fill The Center Of The Box */
Maskdata = 0xFF ; /* Set The Bit Mask Reg */

/*
 * For Each Line In The Source Pixmap
 */
for ( SourceRow = h, dst = EGABASE + BYTE_OFFSET( x, y ), lptr = data ;
      SourceRow-- ;
      dst += BYTES_PER_ROW - ROW_OFFSET( w ),
      lptr += paddedByteWidth ) {
	/*
	 * For Each Byte Across The Pattern In The X Dimension
	 */
	for ( tmp2 = ROW_OFFSET( w ), NeedValX = SavNeedX ;
	      tmp2-- ;
	      dst++ ) {
		/* Read To Save */
		tmp1 = *( (EgaMemoryPtr) dst ) ;
		/* Write Pattern */
		cptr = lptr + ( NeedValX >> 3 ) ;
		/* Write Pattern */
		outb( 0x3CF, /* Set The Bit Mask Reg */
			( shift )
			? ( ( *cptr << shift ) | ( cptr[1] >> ( 8 - shift ) ) )
			: *cptr ) ;
		*( (EgaMemoryPtr) dst ) = Maskdata ;
		if ( ( NeedValX += 8 ) >= cursorWidth )
			NeedValX -= cursorWidth ;
	}
}

/* Do Right Edge */
if ( tmp1 = BIT_OFFSET( w ) ) { /* x Now Is Byte Aligned */
	Maskdata = ( 0xFF << ( 8 - tmp1 ) ) ; /* Set The Bit Mask */
	/*
	 * For Each Line In The Source Pixmap
	 */
	cptr = data + ( NeedValX >> 3 ) ;
	SourceRow = h ;
	dst = EGABASE + BYTE_OFFSET( ( x + w ), y ) ;
	if ( shift = NeedValX & 7 )
		for ( ;
		      SourceRow-- ;
		      dst += BYTES_PER_ROW, cptr += paddedByteWidth ) {
			/* Read To Save */
			tmp1 = *( (EgaMemoryPtr) dst ) ;
			/* Write Pattern */
			outb( 0x3CF, /* Set The Bit Mask Reg */
	( Maskdata & ( ( *cptr << shift ) | ( cptr[1] >> ( 8 - shift ) ) ) ) ) ;
			*( (EgaMemoryPtr) dst ) = Maskdata ;
		}
	else /* shift == 0 */
		while ( SourceRow-- ) {
			/* Read To Save */
			tmp1 = *( (EgaMemoryPtr) dst ) ;
			dst += BYTES_PER_ROW ;
			/* Write Pattern */
			outb( 0x3CF, Maskdata & *cptr ) ; /* Set The Bit Mask Reg */
			*( (EgaMemoryPtr) dst ) = Maskdata ;
			cptr += paddedByteWidth ;
		}
}

return ;
}

void egaRemoveCursor()
{
   egaCursorChecking++ ; /* Prevent Multiple Cursors */
   egaBitBlt( GXcopy, ALLPLANES, ALLPLANES,
	      CURSOR_SAVE_COL, CURSOR_SAVE_ROW,
	      cursor_save_x_loc, cursor_save_y_loc,
	      cursor_save_width, cursor_save_height ) ;
   egaCursorChecking-- ;
   active = FALSE ;
return ;
}

int egaShowCursor( x_loc, y_loc )
register const int x_loc, y_loc ;
{
register int x ;
register int y ;
register int width = MAX_CURSOR_WIDTH ;
register int bg_height ;
register int fg_height ;
register int hotx, hoty ;

TRACE( ( "egaShowCursor(x=%d, y=%d)\n", x, y ) ) ;
if ( active )
	egaRemoveCursor() ;

bg_height = background_height ;
fg_height = foreground_height ;

x = ( hotx = ( ( c_x = x_loc ) - rtCursorHotX( screen_index ) ) ) ;
y = ( hoty = ( ( c_y = y_loc ) - rtCursorHotY( screen_index ) ) ) ;

/* Validate The Arguments */
if ( x < 0 ) {
	width += x ;
	x = 0 ;
}
if ( x + width > ( MAX_COLUMN + 1 ) )
	width = ( MAX_COLUMN + 1 ) - x ;

if ( y < 0 ) {
	bg_height += y ;
	fg_height += y ;
	y = 0 ;
}
if ( y + bg_height > ( MAX_ROW + 1 ) )
	bg_height = ( MAX_ROW + 1 ) - y ;
if ( y + fg_height > ( MAX_ROW + 1 ) )
	fg_height = ( MAX_ROW + 1 ) - y ;

/* Check If The Cursor Is On-Screen */
if ( ( width > 0 )
  && ( ( bg_height > 0 ) || ( fg_height > 0 ) )
  && ( x <= MAX_COLUMN )
  && ( y <= MAX_ROW ) ) {
	egaCursorChecking++ ; /* Prevent Multiple Cursors */
	/* Round Edges To Use FAST Blt */
	egaBitBlt( GXcopy, ALLPLANES, ALLPLANES,
		   ( cursor_save_x_loc = x & ~0x7 ),
		   ( cursor_save_y_loc = y ),
		   CURSOR_SAVE_COL,
		   CURSOR_SAVE_ROW,
	   ( cursor_save_width = ( ( ( width + ( x & 0x7 ) ) + 0x7 ) & ~0x7 ) ),
		   ( cursor_save_height = ( bg_height > fg_height )
					? bg_height : fg_height ) ) ;
	--egaCursorChecking ; /* Prevent Multiple Cursors */
	/*
	 * Set The Plane-Enable
	 */
	outb( 0x3C4, 2 ) ;
	outb( 0x3C5, ALLPLANES ) ; /* Map Mask Register */
	/*
	 * Set The Planes in The Enable-Set/Reset Register
	 */
	outb( 0x3CE, 1 ) ;
	outb( 0x3CF, ALLPLANES ) ;
	/*
	 * Set The Ega's Alu Function
	 */
	outb( 0x3CE, 3 ) ;
	outb( 0x3CF, 0x0 ) ; /* Data Rotate Register */
	/*
	 * Put Display Into SET/RESET Write Mode
	 */
/* ******************** EGAFIX ******************** */
/* Need To Use Mode 0
 * Method Exchange The vga's method of writing
 * the bit-mask and the data
 * i.e. Write The Pattern into The Bit-Mask Register
 * Write What would have been the mask as data
 */
	outb( 0x3CE, 5 ) ;
	outb( 0x3CF, 0x0 ) ; /* Graphics Mode Register */

	if ( fg_height > 0 ) {
		/*
		 * Set The Color in The Set/Reset Register
		 */
		outb( 0x3CE, 0 ) ;
		outb( 0x3CF, c_fg ) ; /* Set/Reset Register */
		egaScreenDrawCursor( (unsigned char *) cursorFG,
				     width,
				     foreground_width,
				     fg_height,
				     foreground_height,
				     x, y,
				     hotx, hoty ) ;
	}
	if ( bg_height > 0 ) {
		/*
		 * Set The Color in The Set/Reset Register
		 */
		outb( 0x3CE, 0 ) ;
		outb( 0x3CF, c_bg ) ; /* Set/Reset Register */
		egaScreenDrawCursor( (unsigned char *) cursorBG,
				     width,
				     background_width,
				     bg_height,
				     background_height,
				     x, y,
				     hotx, hoty ) ;
	}
	/* Disable Set/Reset Register */
	outb( 0x3CE, 1 ) ;
	outb( 0x3CF, 0 ) ;

	active = TRUE ;
}

return 0 ;
}

void egaCursorInit( index )
register int index ;
{
    TRACE( ( "egaCursorInit(index=%d)\n", index ) ) ;

    screen_index = index ;
    rtSoftCursor( index ) = TRUE ;
    c_x = 0 ;
    c_y = 0 ;
    rtCursorBounds( index )->x1 =	MAX_COLUMN + 100 ;
    rtCursorBounds( index )->y1 =	MAX_ROW + 100 ;
    rtCursorBounds( index )->x2 =	MAX_COLUMN + 100 ;
    rtCursorBounds( index )->y2 =	MAX_ROW + 100 ;
    rtCursorShow( index )=	egaShowCursor ;
    rtCurrentCursor( index )=	NULL ;
    active = FALSE ;
    egaCursorChecking = 0 ;
return ;
}

void egaColorCursor( fg, bg )
register unsigned int fg, bg ;
{
TRACE( ( "egaColorCursor(%d,%d)\n", fg, bg ) ) ;
if ( ( fg > 15 ) || ( bg > 15 ) ) {
	ErrorF( "egaColorCursor: bad color value(s)(fg/bg)=(%d,%d)", fg, bg ) ;
	return ;
}
c_fg = fg ;
c_bg = bg ;
return ;
}

/***============================================================***/
/***============================================================***/

int
egaDisplayCursor( pScr, pCurs )
register ScreenPtr const pScr ;
register CursorPtr const pCurs ;
{
    register const unsigned long int *curdef ;
    register const unsigned long int *sptr ;
    register unsigned long int *dptr ;
    register int i ;

    TRACE( ("egaDisplayCursor( pScr = 0x%x, pCurs = 0x%x )\n", pScr, pCurs ) ) ;

    if ( active )
	egaRemoveCursor() ;
    curdef = (const unsigned long int *) pCurs->devPriv[ pScr->myNum ] ;

    rtCursorHotX( pScr->myNum ) = pCurs->xhot ;
    rtCursorHotY( pScr->myNum ) = pCurs->yhot ;
    rtCurrentCursor( pScr->myNum ) = pCurs ;

    /*
    egaFindColor( &fg, pCurs->foreRed, pCurs->foreGreen, pCurs->foreBlue ) ;
    egaFindColor( &bg, pCurs->backRed, pCurs->backGreen, pCurs->backBlue ) ;
    */
    egaColorCursor( 0, 15 ) ;

    rtCursorBounds( pScr->myNum )->x1 =	MAX_COLUMN + 100 ;
    rtCursorBounds( pScr->myNum )->y1 =	MAX_ROW    + 100 ;
    rtCursorBounds( pScr->myNum )->x2 =	MAX_COLUMN + 100 ;
    rtCursorBounds( pScr->myNum )->y2 =	MAX_ROW    + 100 ;

    /* Strip The Cursor Down To Size */
    for ( i = MAX_CURSOR_HEIGHT, sptr = &( curdef[63] ) ;
	  i && !*sptr ;
	  i--, sptr-- ) ;
    foreground_height = i ;
    for ( dptr = &cursorFG[i -1] ;
	  i-- ;
	  *dptr-- = *sptr-- ) ;
    for ( i = MAX_CURSOR_HEIGHT, sptr = &( curdef[31] ) ;
	  i && !*sptr ;
	  i--, sptr-- ) ;
    background_height = i ;
    for ( dptr = &cursorBG[i -1] ;
	  i-- ;
	  *dptr-- = *sptr-- ) ;

    egaShowCursor( c_x, c_y ) ;

    return TRUE ;
}

int
egaCheckCursor( x, y, lx, ly )
register const int x, y, lx, ly ;
{
/* check if the cursor is in this rectangle.  if so, remove and return TRUE
    else return FALSE */

if ( !egaCursorChecking && active
  && !( ( x >= ( cursor_save_x_loc + cursor_save_width ) )
     || ( y >= ( cursor_save_y_loc + cursor_save_height ) )
     || ( ( x + lx ) <= cursor_save_x_loc )
     || ( ( y + ly ) <= cursor_save_y_loc ) ) ) {
	TRACE( ( "FOUND!\n\t" ) ) ;
	egaCursorChecking = 1 ;
	egaRemoveCursor() ;
	egaCursorChecking = 0 ;
	return cursor_not_displayed = TRUE ;
}
else
	return FALSE ;
/*NOTREACHED*/
}

void egaReplaceCursor()
{
if ( cursor_not_displayed && !egaCursorChecking ) {
	egaCursorChecking = 1 ;
	egaShowCursor( c_x, c_y ) ;
	egaCursorChecking = 0 ;
	cursor_not_displayed = FALSE ;
}

return ;
}
