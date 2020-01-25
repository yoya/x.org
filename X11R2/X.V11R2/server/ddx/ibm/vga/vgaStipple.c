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
/* $Header:vgaStipple.c 1.5$ */
/* $ACIS:vgaStipple.c 1.5$ */
/* $Source: /ibm/acis/usr/src/X11/server/ddx/ibm/vga/RCS/vgaStipple.c,v $ */

#ifndef lint
static char *rcsid = "$Header:vgaStipple.c 1.5$";
#endif

#include "vga_video.h"
#include "X.h"
#include "pixmapstr.h"

#undef TRUE
#undef FALSE
#define TRUE 1
#define FALSE 0

static unsigned char getbits( x, patternWidth, lineptr )
register const int x, patternWidth ;
register const unsigned char * const lineptr ;
{
register unsigned char bits ;
register const unsigned char *cptr ;
register shift ;
register wrap ;

cptr = lineptr + ( x >> 3 ) ;
bits = *cptr ;
if ( shift = x & 7 )
	bits = ( bits << shift ) | ( cptr[1] >> ( 8 - shift ) ) ;
if ( ( wrap = x + 8 - patternWidth ) > 0 ) {
	bits &= 0xFF << wrap ;
	bits |= *lineptr >> ( 8 - wrap ) ;
}

return bits ;
}

extern void vgaFillSolid() ;
/* Declared in "vgacurs.c" */
extern int CursorChecking ;
extern int vgaCheckCursor() ;
extern void vgaReplaceCursor() ;

void vgaFillStipple( pStipple, fg, alu, planes, x, y, w, h, xSrc, ySrc )
PixmapPtr const pStipple ;
int fg ;
const int alu, planes ;
register int x, y, w, h ;
const int xSrc, ySrc ;
{
register unsigned bitPattern ;
register volatile unsigned char *xDst ;
register unsigned DestinationRow ;
register unsigned int SourceRow ;
register int tmp1 ;
register int tmp2 ;
register volatile unsigned char *dst ;
register const unsigned char *mastersrc ;
register unsigned int height ;
register int NeedValX ;
register unsigned int paddedByteWidth ;
int xshift ;
int yshift ;
unsigned int width ;
int SavNeedX ;
unsigned int data_rotate_value = 0 ;
int cursor_saved ;

/* Test The Raster-Op */
{
	unsigned int invert_existing_data = FALSE ;

	switch ( alu ) {
		case GXclear:		/* 0x0 Zero 0 */
			fg = 0 ;
			break ;
		case GXinvert:		/* 0xa NOT dst */
			data_rotate_value = 0x18 ;
		case GXset:		/* 0xf 1 */
			fg = ALLPLANES ;
			break ;
		case GXnoop:		/* 0x5 dst */
			return ;
		case GXnor:		/* 0x8 NOT src AND NOT dst */
			invert_existing_data = TRUE ;
		case GXandInverted:	/* 0x4 NOT src AND dst */
			fg = ~fg ;
		case GXand:		/* 0x1 src AND dst */
			data_rotate_value = 0x8 ;
		case GXcopy:		/* 0x3 src */
			break ;
		case GXequiv:		/* 0x9 NOT src XOR dst */
			fg = ~fg ;
		case GXxor:		/* 0x6 src XOR dst */
			data_rotate_value = 0x18 ;
			break ;
		case GXandReverse:	/* 0x2 src AND NOT dst */
			invert_existing_data = TRUE ;
			data_rotate_value = 0x8 ;
			break ;
		case GXorReverse:	/* 0xb src OR NOT dst */
			invert_existing_data = TRUE ;
			data_rotate_value = 0x10 ;
			break ;
		case GXnand:		/* 0xe NOT src OR NOT dst */
			invert_existing_data = TRUE ;
		case GXorInverted:	/* 0xd NOT src OR dst */
			fg = ~fg ;
		case GXor:		/* 0x7 src OR dst */
			data_rotate_value = 0x10 ;
			break ;
		case GXcopyInverted:	/* 0xc NOT src */
			fg = ~fg ;
		default:
			break ;
	}
	if ( invert_existing_data )
		vgaFillStipple( pStipple, 0xF, GXinvert, planes,
				x, y, w, h, xSrc, ySrc ) ;
}
/* Check If Cursor Is In The Way */
cursor_saved = !CursorChecking && vgaCheckCursor( x, y, w, h ) ;

/* Setup VGA Registers */
/*
 * Put Display Into SET-AND (i.e. Write Mode 3 )
 */
outb( 0x3CE, 5 ) ;
outb( 0x3CF, 0x3 ) ; /* Graphics Mode Register */
/*
 * Set The Color in The Set/Reset Register
 */
outb( 0x3CE, 0 ) ;
outb( 0x3CF, fg ) ; /* Set/Reset Register */
/*
 * Set The Plane-Enable
 */
outb( 0x3C4, 2 ) ;
outb( 0x3C5, planes ) ; /* Map Mask Register */
/*
 * Set The Vga's Alu Function
 */
outb( 0x3CE, 3 ) ;
outb( 0x3CF, data_rotate_value ) ; /* Data Rotate Register */

/* Point At The Bit Mask Reg */
outb( 0x3CE, 8 ) ;

/* Figure Bit Offsets & Source Address */
mastersrc = (const unsigned char *) pStipple->devPrivate ;
width = pStipple->width ;
paddedByteWidth = ( ( width + 31 ) & ~31 ) >> 3 ;

if ( ( xshift = ( x - xSrc ) % width ) < 0 )
	xshift += width ;

height = pStipple->height ;
if ( ( yshift = ( y - ySrc ) % height ) < 0 )
	yshift += height ;

/* Do Left Edge */
if ( tmp1 = x & 07 ) {
	tmp2 = ( (unsigned) 0xFF ) >> tmp1 ;
	/* Catch The Cases Where The Entire Region Is Within One Byte */
	if ( ( w -= 8 - tmp1 ) < 0 ) {
		tmp2 &= 0xFF << -w ;
		w = 0 ;
	}
	outb( 0x3CF, tmp2 ) ; /* Set The Bit Mask Reg */
	if ( ( NeedValX = xshift - tmp1 ) < 0 )
		NeedValX += width ;
	/*
	 * For Each Line In The Source Pixmap
	 */
	SourceRow = 0, dst = VGABASE + BYTE_OFFSET( x, y ) ;
	if ( h > height )
		for ( ;
		      SourceRow < height ;
		      SourceRow++, dst += BYTES_PER_ROW ) {

			if ( ( tmp1 = ( SourceRow + yshift ) ) >= height )
				tmp1 -= height ;
			bitPattern = getbits( NeedValX, width,
					      mastersrc + ( tmp1 * paddedByteWidth ) ) ;
			/*
			 * For Each Time Pattern Repeats In The Y Dimension
			 */
			for ( xDst = dst, DestinationRow = /* y + */ SourceRow ;
			      DestinationRow < /* y + */ h ;
			      xDst += height * BYTES_PER_ROW,
			      DestinationRow += height ) {
				/* Read To Save */
				tmp2 = *( (VgaMemoryPtr) xDst) ;
				/* Write Pattern */
				*( (VgaMemoryPtr) xDst ) =
					bitPattern ;
			}
		}
	else
		for ( ;
		      SourceRow < h ;
		      SourceRow++, dst += BYTES_PER_ROW ) {

			if ( ( tmp1 = ( SourceRow + yshift ) ) >= height )
				tmp1 -= height ;
			bitPattern = getbits( NeedValX, width,
					      mastersrc + ( tmp1 * paddedByteWidth ) ) ;
			/* Read To Save */
			tmp2 = *( (VgaMemoryPtr) dst ) ;
			/* Write Pattern */
			*( (VgaMemoryPtr) dst ) = bitPattern ;
		}
	x = ( x + 7 ) & ~07 ;
	if ( ( NeedValX += 8 ) >= width )
		NeedValX -= width ;
}
else {
	NeedValX = xshift ;
}
/* Fill The Center Of The Box */
outb( 0x3CF, 0xFF ) ; /* Set The Bit Mask Reg */
/*
 * For Each Line In The Source Pixmap
 */
SavNeedX = NeedValX ;

SourceRow = 0, dst = VGABASE + BYTE_OFFSET( x, y ) ;
if ( h > height )
	for ( ;
	      SourceRow < height ;
	      SourceRow++, dst += BYTES_PER_ROW - ROW_OFFSET( w ) ) {
		/*
		 * For Each Byte Across The Pattern In The X Dimension
		 */
		tmp2 = ROW_OFFSET( w ), NeedValX = SavNeedX ;
		for ( ; tmp2-- ; dst++ ) {

			if ( ( tmp1 = ( SourceRow + yshift ) ) >= height )
				tmp1 -= height ;
			bitPattern = getbits( NeedValX, width,
					      mastersrc + ( tmp1 * paddedByteWidth ) ) ;
			/*
			 * For Each Time Pattern Repeats In The Y Dimension
			 */
			for ( xDst = dst, DestinationRow = /* y + */ SourceRow ;
			      DestinationRow < /* y + */ h ;
			      xDst += ( height * BYTES_PER_ROW ),
			      DestinationRow += height ) {
				/* Read To Save */
				tmp1 = *( (VgaMemoryPtr) xDst ) ;
				/* Write Pattern */
				*( (VgaMemoryPtr) xDst ) = bitPattern ;
			}
			if ( ( NeedValX += 8 ) >= width )
				NeedValX -= width ;
		}
	}
else
	for ( ;
	      SourceRow < h ;
	      SourceRow++, dst += BYTES_PER_ROW - ROW_OFFSET( w ) ) {
		/*
		 * For Each Byte Across The Pattern In The X Dimension
		 */
		tmp2 = ROW_OFFSET( w ), NeedValX = SavNeedX ;
		for ( ; tmp2-- ; dst++ ) {
			if ( ( tmp1 = ( SourceRow + yshift ) ) >= height )
				tmp1 -= height ;
			bitPattern = getbits( NeedValX, width,
					      mastersrc + ( tmp1 * paddedByteWidth ) ) ;
			/* Read To Save */
			tmp1 = *( (VgaMemoryPtr) dst ) ;
			/* Write Pattern */
			*( (VgaMemoryPtr) dst ) = bitPattern ;
			if ( ( NeedValX += 8 ) >= width )
				NeedValX -= width ;
		}
	}

/* Do Right Edge */
if ( tmp1 = BIT_OFFSET( w ) ) { /* x Now Is Byte Aligned */
	outb( 0x3CF, 0xFF << ( 8 - tmp1 ) ) ; /* Set The Bit Mask */
	/*
	 * For Each Line In The Source Pixmap
	 */
	SourceRow = 0, dst = VGABASE + BYTE_OFFSET( ( x + w ), y ) ;
	if ( h > height )
		for ( ;
		      SourceRow < height ;
		      SourceRow++, dst += BYTES_PER_ROW ) {

			if ( ( tmp1 = ( SourceRow + yshift ) ) >= height )
				tmp1 -= height ;
			bitPattern = getbits( NeedValX, width,
					      mastersrc + ( tmp1 * paddedByteWidth ) ) ;
			/*
			 * For Each Time Pattern Repeats In The Y Dimension
			 */
			for ( xDst = dst, DestinationRow = /* y + */ SourceRow ;
			      DestinationRow < /* y + */ h ;
			      xDst += ( height * BYTES_PER_ROW ),
			      DestinationRow += height ) {
				/* Read To Save */
				tmp1 = *( (VgaMemoryPtr) xDst ) ;
				/* Write Pattern */
				*( (VgaMemoryPtr) xDst ) = bitPattern ;
			}
		}
	else
		for ( ;
		      SourceRow < h ;
		      SourceRow++, dst += BYTES_PER_ROW ) {

			if ( ( tmp1 = ( SourceRow + yshift ) ) >= height )
				tmp1 -= height ;
			bitPattern = getbits( NeedValX, width,
					      mastersrc + ( tmp1 * paddedByteWidth ) ) ;
			/* Read To Save */
			tmp1 = *( (VgaMemoryPtr) dst ) ;
			/* Write Pattern */
			*( (VgaMemoryPtr) dst ) = bitPattern ;
		}
}
/* Replace Cursor If Nessesary */
if ( cursor_saved )
	vgaReplaceCursor() ;

return ;
}
