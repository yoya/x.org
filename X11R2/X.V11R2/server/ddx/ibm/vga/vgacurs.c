 
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
/* $Header:vgacurs.c 1.4$ */
/* $ACIS:vgacurs.c 1.4$ */
/* $Source: /ibm/acis/usr/src/X11/server/ddx/ibm/vga/RCS/vgacurs.c,v $ */

#ifndef lint
static char *rcsid = "$Header:vgacurs.c 1.4$";
#endif

/*
 * Software Cursor implementation for vga
 * Paquin 7/87
 * Modifed For vga -- Paul Shupak 10/87
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

#include "vga_video.h"
#include "vgaprocs.h"

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
int CursorChecking = 0 ;

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

/* Bytewidth == 1 */
static void
fast_CursorBlt1( source, destination, height )
register volatile char *source ;
register volatile char *destination ;
register unsigned int height ;
{
register int tmp ;
register const unsigned int notZero = ~0 ;
register const int row_jump = BYTES_PER_ROW - 1 ;

for ( ; height-- ; source += row_jump, destination += row_jump ) {
	tmp = *( (VgaMemoryPtr) ( source ++ ) ) ;
	( *( (VgaMemoryPtr) ( destination ++ ) ) = notZero ) ;
}
return ;
}

/* Bytewidth == 2 */
static void
fast_CursorBlt2( source, destination, height )
register volatile char *source ;
register volatile char *destination ;
register unsigned int height ;
{
register int tmp ;
register const unsigned int notZero = ~0 ;
register const int row_jump = BYTES_PER_ROW - 2 ;

for ( ; height-- ; source += row_jump, destination += row_jump ) {
	tmp = *( (VgaMemoryPtr) ( source ++ ) ) ;
	( *( (VgaMemoryPtr) ( destination ++ ) ) = notZero ) ;
	tmp = *( (VgaMemoryPtr) ( source ++ ) ) ;
	( *( (VgaMemoryPtr) ( destination ++ ) ) = notZero ) ;
}
return ;
}
/* Bytewidth == 3 */
static void
fast_CursorBlt3( source, destination, height )
register volatile char *source ;
register volatile char *destination ;
register unsigned int height ;
{
register int tmp ;
register const unsigned int notZero = ~0 ;
register const int row_jump = BYTES_PER_ROW - 3 ;

for ( ; height-- ; source += row_jump, destination += row_jump ) {
	tmp = *( (VgaMemoryPtr) ( source ++ ) ) ;
	( *( (VgaMemoryPtr) ( destination ++ ) ) = notZero ) ;
	tmp = *( (VgaMemoryPtr) ( source ++ ) ) ;
	( *( (VgaMemoryPtr) ( destination ++ ) ) = notZero ) ;
	tmp = *( (VgaMemoryPtr) ( source ++ ) ) ;
	( *( (VgaMemoryPtr) ( destination ++ ) ) = notZero ) ;
}
return ;
}
/* Bytewidth == 4 */
static void
fast_CursorBlt4( source, destination, height )
register volatile char *source ;
register volatile char *destination ;
register unsigned int height ;
{
register int tmp ;
register const unsigned int notZero = ~0 ;
register const int row_jump = BYTES_PER_ROW - 4 ;

for ( ; height-- ; source += row_jump, destination += row_jump ) {
	tmp = *( (VgaMemoryPtr) ( source ++ ) ) ;
	( *( (VgaMemoryPtr) ( destination ++ ) ) = notZero ) ;
	tmp = *( (VgaMemoryPtr) ( source ++ ) ) ;
	( *( (VgaMemoryPtr) ( destination ++ ) ) = notZero ) ;
	tmp = *( (VgaMemoryPtr) ( source ++ ) ) ;
	( *( (VgaMemoryPtr) ( destination ++ ) ) = notZero ) ;
	tmp = *( (VgaMemoryPtr) ( source ++ ) ) ;
	( *( (VgaMemoryPtr) ( destination ++ ) ) = notZero ) ;
}
return ;
}
/* Bytewidth == 5 */
static void
fast_CursorBlt5( source, destination, height )
register volatile char *source ;
register volatile char *destination ;
register unsigned int height ;
{
register int tmp ;
register const unsigned int notZero = ~0 ;
register const int row_jump = BYTES_PER_ROW - 5 ;

for ( ; height-- ; source += row_jump, destination += row_jump ) {
	tmp = *( (VgaMemoryPtr) ( source ++ ) ) ;
	( *( (VgaMemoryPtr) ( destination ++ ) ) = notZero ) ;
	tmp = *( (VgaMemoryPtr) ( source ++ ) ) ;
	( *( (VgaMemoryPtr) ( destination ++ ) ) = notZero ) ;
	tmp = *( (VgaMemoryPtr) ( source ++ ) ) ;
	( *( (VgaMemoryPtr) ( destination ++ ) ) = notZero ) ;
	tmp = *( (VgaMemoryPtr) ( source ++ ) ) ;
	( *( (VgaMemoryPtr) ( destination ++ ) ) = notZero ) ;
	tmp = *( (VgaMemoryPtr) ( source ++ ) ) ;
	( *( (VgaMemoryPtr) ( destination ++ ) ) = notZero ) ;
}
return ;
}

static void (*UnwoundBlitProcs[5])() = {
	fast_CursorBlt1,
	fast_CursorBlt2,
	fast_CursorBlt3,
	fast_CursorBlt4,
	fast_CursorBlt5
} ;

/* Only Does Special Case -- VERY Fast Blt
 * ( Width % 8 ) == 0 AND ( x0 % 8 ) == 0 AND ( x1 % 8 ) == 0
 */
static void vgaCursorBitBlt( x0, y0, x1, y1, w, h )
register int x0 ;
register int y0 ;
register int x1 ;
register int y1 ;
register int w, h ;
{

/* ******************** Vga Register Setup ******************** */
/* Disable SET/RESET Function */
outb( 0x3CE, 1 ) ;
outb( 0x3CF, 0 ) ;
/* Set Write Mode To 0 -- Read Mode To 0 */
outb( 0x3CE, 5 ) ;
outb( 0x3CF, 0 ) ;
/* Set Bit Mask */
outb( 0x3CE, 8 ) ;
outb( 0x3CF, 0xFF ) ;
/* Set Data Rotate Function To Data "AND" */
outb( 0x3CE, 3 ) ;
outb( 0x3CF, 0x8 ) ;
/* Set Map Mask */
outb( 0x3C4, 2 ) ;
outb( 0x3C5, ALLPLANES ) ;

/* Really Do It Here! */
(*UnwoundBlitProcs[ROW_OFFSET( w ) - 1])(
	(char *) VGABASE + ( BYTES_PER_ROW * y0 ) + ROW_OFFSET( x0 ),
	(char *) VGABASE + ( BYTES_PER_ROW * y1 ) + ROW_OFFSET( x1 ),
	h ) ;

return ;
}

static void vgaScreenDrawCursor( data, w, cursorWidth, h, cursorHeight,
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
	outb( 0x3CF, tmp2 ) ;
	/*
	 * For Each Line In The Cursor Data
	 */
	SourceRow = h ;
	dst = VGABASE + BYTE_OFFSET( x, y ) ;
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
			tmp2 = *( (VgaMemoryPtr) dst ) ;
			/* Write Pattern */
			*( (VgaMemoryPtr) dst ) =
				( *cptr << shift ) | ( cptr[1] >> ( 8 - shift ) ) ;
		}
	else /* shift == 0 */
		for ( ;
		      SourceRow-- ;
		      dst += BYTES_PER_ROW, cptr += paddedByteWidth ) {
			/* Read To Save */
			tmp2 = *( (VgaMemoryPtr) dst ) ;
			/* Write Pattern */
			*( (VgaMemoryPtr) dst ) = *cptr ;
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
outb( 0x3CF, 0xFF ) ; /* Set The Bit Mask Reg */

/*
 * For Each Line In The Source Pixmap
 */
for ( SourceRow = h, dst = VGABASE + BYTE_OFFSET( x, y ), lptr = data ;
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
		tmp1 = *( (VgaMemoryPtr) dst ) ;
		/* Write Pattern */
		cptr = lptr + ( NeedValX >> 3 ) ;
		*( (VgaMemoryPtr) dst ) = ( shift )
			? ( ( *cptr << shift ) | ( cptr[1] >> ( 8 - shift ) ) )
			: *cptr ;
		if ( ( NeedValX += 8 ) >= cursorWidth )
			NeedValX -= cursorWidth ;
	}
}

/* Do Right Edge */
if ( tmp1 = BIT_OFFSET( w ) ) { /* x Now Is Byte Aligned */
	outb( 0x3CF, 0xFF << ( 8 - tmp1 ) ) ; /* Set The Bit Mask */
	/*
	 * For Each Line In The Source Pixmap
	 */
	cptr = data + ( NeedValX >> 3 ) ;
	SourceRow = h ;
	dst = VGABASE + BYTE_OFFSET( ( x + w ), y ) ;
	if ( shift = NeedValX & 7 )
		for ( ;
		      SourceRow-- ;
		      dst += BYTES_PER_ROW, cptr += paddedByteWidth ) {
			/* Read To Save */
			tmp1 = *( (VgaMemoryPtr) dst ) ;
			/* Write Pattern */
			*( (VgaMemoryPtr) dst ) =
				( *cptr << shift ) | ( cptr[1] >> ( 8 - shift ) ) ;
		}
	else /* shift == 0 */
		while ( SourceRow-- ) {
			/* Read To Save */
			tmp1 = *( (VgaMemoryPtr) dst ) ;
			dst += BYTES_PER_ROW ;
			/* Write Pattern */
			*( (VgaMemoryPtr) dst ) = *cptr ;
			cptr += paddedByteWidth ;
		}
}

return ;
}

void vgaRemoveCursor()
{
    vgaCursorBitBlt( CURSOR_SAVE_COL, CURSOR_SAVE_ROW,
		     cursor_save_x_loc, cursor_save_y_loc,
		     cursor_save_width, cursor_save_height ) ;
    active = FALSE ;
return ;
}

int vgaShowCursor( x_loc, y_loc )
register const int x_loc, y_loc ;
{
register int x ;
register int y ;
register int width = MAX_CURSOR_WIDTH ;
register int bg_height ;
register int fg_height ;
register int hotx, hoty ;

TRACE( ( "vgaShowCursor(x=%d, y=%d)\n", x, y ) ) ;
if ( active )
	vgaRemoveCursor() ;

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
	/* Round Edges To Use FAST Blt */
	vgaCursorBitBlt( 
		   ( cursor_save_x_loc = x & ~0x7 ),
		   ( cursor_save_y_loc = y ),
		   CURSOR_SAVE_COL,
		   CURSOR_SAVE_ROW,
	   ( cursor_save_width = ( ( ( width + ( x & 0x7 ) ) + 0x7 ) & ~0x7 ) ),
		   ( cursor_save_height = ( bg_height > fg_height )
					? bg_height : fg_height ) ) ;
	/*
	 * Set The Plane-Enable
	 */
	outb( 0x3C4, 2 ) ;
	outb( 0x3C5, ALLPLANES ) ; /* Map Mask Register */
	/*
	 * Set The Vga's Alu Function
	 */
	outb( 0x3CE, 3 ) ;
	outb( 0x3CF, 0x0 ) ; /* Data Rotate Register */
	/*
	 * Put Display Into SET/RESET Write Mode
	 */
	outb( 0x3CE, 5 ) ;
	outb( 0x3CF, 0x3 ) ; /* Graphics Mode Register */

	if ( fg_height > 0 ) {
		/*
		 * Set The Color in The Set/Reset Register
		 */
		outb( 0x3CE, 0 ) ;
		outb( 0x3CF, c_fg ) ; /* Set/Reset Register */
		vgaScreenDrawCursor( (unsigned char *) cursorFG,
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
		vgaScreenDrawCursor( (unsigned char *) cursorBG,
				     width,
				     background_width,
				     bg_height,
				     background_height,
				     x, y,
				     hotx, hoty ) ;
	}
	active = TRUE ;
}

return 0 ;
}

void vgaCursorInit( index )
register int index ;
{
    TRACE( ( "vgaCursorInit(index=%d)\n", index ) ) ;

    screen_index = index ;
    rtSoftCursor( index ) = TRUE ;
    c_x = 0 ;
    c_y = 0 ;
    rtCursorBounds( index )->x1 =	MAX_COLUMN + 100 ;
    rtCursorBounds( index )->y1 =	MAX_ROW + 100 ;
    rtCursorBounds( index )->x2 =	MAX_COLUMN + 100 ;
    rtCursorBounds( index )->y2 =	MAX_ROW + 100 ;
    rtCursorShow( index )=	vgaShowCursor ;
    rtCurrentCursor( index )=	NULL ;
    active = FALSE ;
    CursorChecking = 0 ;
return ;
}

void vgaColorCursor( fg, bg )
register unsigned int fg, bg ;
{
TRACE( ( "vgaColorCursor(%d,%d)\n", fg, bg ) ) ;
if ( ( fg > 15 ) || ( bg > 15 ) ) {
	ErrorF( "vgaColorCursor: bad color value(s)(fg/bg)=(%d,%d)", fg, bg ) ;
	return ;
}
c_fg = fg ;
c_bg = bg ;
return ;
}

/***============================================================***/
/*
static void vgaStripCursor( cursorBitArray )
register unsigned long int cursorBitArray[32] ;
{
register unsigned int i ;
register unsigned long int bitMask ;
register unsigned long int *sptr ;

*/
    /* Strip The Cursor Down To Size */
    /* Remove Zero's At The Beginning */
/*
    for ( i = MAX_CURSOR_HEIGHT, sptr = curdef ;
	  i && !*sptr ;
	  i--, sptr-- ) ;
    foregroundOffsetY = MAX_CURSOR_WIDTH - i ;
*/
    /* Remove Zero's At The End */
/*
    for ( sptr = &( cursorBitArray[31] ) ;
	  !*sptr ;
	  i--, sptr-- ) ;
    foreground_height = i ;
*/
    /* Find All Used Bit Positions */
/*
    for ( bitMask = 0 ;
	  i-- ;
	  bitMask |= *sptr-- ) ;
    bitMask = ~bitMask ;
*/
    /* Leading & Trailing 0nes are Unused Bit Positions */
    /* Remove Zero's At The Beginning */
/*
    for ( i = MAX_CURSOR_WIDTH ;
	  bitMask & 0x80000000 && i-- ;
	  bitMask <<= 1 ) ;
*/
    /* Remove Zero's At The End */
/*
    for ( bitMask >>= ( foregroundOffsetX = MAX_CURSOR_WIDTH - i ) ;
	  bitMask & 0x1 && i-- ;
	  bitMask >>= 1 ) ;
    foreground_width = i ;

*/
    /* Build Copy(s) */
/*
    for ( dptr = &cursorFG[i -1] ;
	  i-- ;
	  *dptr-- = *sptr-- ) ;

    return ;
}
*/

/***============================================================***/

int
vgaDisplayCursor( pScr, pCurs )
register ScreenPtr const pScr ;
register CursorPtr const pCurs ;
{
    register const unsigned long int *curdef ;
    register const unsigned long int *sptr ;
    register unsigned long int *dptr ;
    register int i ;

    TRACE( ("vgaDisplayCursor( pScr = 0x%x, pCurs = 0x%x )\n", pScr, pCurs ) ) ;

    if ( active )
	vgaRemoveCursor() ;
    curdef = (const unsigned long int *) pCurs->devPriv[ pScr->myNum ] ;

    rtCursorHotX( pScr->myNum ) = pCurs->xhot ;
    rtCursorHotY( pScr->myNum ) = pCurs->yhot ;
    rtCurrentCursor( pScr->myNum ) = pCurs ;

    vgaColorCursor( 0, 15 ) ;

    rtCursorBounds( pScr->myNum )->x1 =	MAX_COLUMN + 100 ;
    rtCursorBounds( pScr->myNum )->y1 =	MAX_ROW    + 100 ;
    rtCursorBounds( pScr->myNum )->x2 =	MAX_COLUMN + 100 ;
    rtCursorBounds( pScr->myNum )->y2 =	MAX_ROW    + 100 ;

    /* Strip The Cursor Down To Size */
    for ( i = MAX_CURSOR_HEIGHT, sptr = &( curdef[63] ) ;
	  i && !*sptr ;
	  i--, sptr-- ) ;
    background_height = i ;
    for ( dptr = &cursorBG[i -1] ;
	  i-- ;
	  *dptr-- = *sptr-- ) ;
    for ( i = MAX_CURSOR_HEIGHT, sptr = &( curdef[31] ) ;
	  i && !*sptr ;
	  i--, sptr-- ) ;
    foreground_height = i ;
    for ( dptr = &cursorFG[i -1] ;
	  i-- ;
	  *dptr-- = *sptr-- ) ;

    vgaShowCursor( c_x, c_y ) ;

    return TRUE ;
}

int
vgaCheckCursor( x, y, lx, ly )
register const int x, y, lx, ly ;
{
/* check if the cursor is in this rectangle.  if so, remove and return TRUE
    else return FALSE */

if ( !CursorChecking && active
  && !( ( x >= ( cursor_save_x_loc + cursor_save_width ) )
     || ( y >= ( cursor_save_y_loc + cursor_save_height ) )
     || ( ( x + lx ) <= cursor_save_x_loc )
     || ( ( y + ly ) <= cursor_save_y_loc ) ) ) {
	TRACE( ( "FOUND!\n\t" ) ) ;
	CursorChecking = 1 ;
	vgaRemoveCursor() ;
	CursorChecking = 0 ;
	return cursor_not_displayed = TRUE ;
}
else
	return FALSE ;
/*NOTREACHED*/
}

void vgaReplaceCursor()
{
if ( cursor_not_displayed && !CursorChecking ) {
	CursorChecking = 1 ;
	vgaShowCursor( c_x, c_y ) ;
	CursorChecking = 0 ;
	cursor_not_displayed = FALSE ;
}

return ;
}
