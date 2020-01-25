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
/* $Header:vgaColorImg.c 1.4$ */
/* $ACIS:vgaColorImg.c 1.4$ */
/* $Source: /ibm/acis/usr/src/X11/server/ddx/ibm/vga/RCS/vgaColorImg.c,v $ */

#ifndef lint
static char *rcsid = "$Header:vgaColorImg.c 1.4$";
#endif

#include "vga_video.h"
#include "X.h"

#undef TRUE
#undef FALSE
#define TRUE 1
#define FALSE 0

extern void vgaFillSolid() ;
/* Declared in "vgacurs.c" */
extern int CursorChecking ;
extern int vgaCheckCursor() ;
extern void vgaReplaceCursor() ;

void vgaDrawColorImage( x, y, w, h, data, alu, planes )
int x, y ;
register int w, h ;
register const unsigned char *data ;
const int alu, planes ;
{
register unsigned long int tmp ;
register const unsigned char *src ;
register volatile unsigned char *dst ;
register int Pixel_Count ;
register unsigned int Mask ;
register unsigned int InitialMask ;
register const unsigned char *DataPtr ;
register volatile unsigned char *StartByte ;
register int RowIncrement ;
unsigned int invert_source_data = FALSE ;
int cursor_saved ;

{
	unsigned int invert_existing_data = FALSE ;
	unsigned int data_rotate_value = 0 ;

	switch ( alu ) {
		case GXclear:		/* 0x0 Zero 0 */
		case GXinvert:		/* 0xa NOT dst */
		case GXset:		/* 0xf 1 */
			vgaFillSolid( 0xF, alu, planes, x, y, w, h ) ;
		case GXnoop:		/* 0x5 dst */
			return ;
		case GXnor:		/* 0x8 NOT src AND NOT dst */
			invert_existing_data = TRUE ;
		case GXandInverted:	/* 0x4 NOT src AND dst */
			invert_source_data = TRUE ;
		case GXand:		/* 0x1 src AND dst */
			data_rotate_value = 0x08 ;
			break ;
		case GXequiv:		/* 0x9 NOT src XOR dst */
			invert_source_data = TRUE ;
		case GXxor:		/* 0x6 src XOR dst */
			data_rotate_value = 0x18 ;
			break ;
		case GXandReverse:	/* 0x2 src AND NOT dst */
			invert_existing_data = TRUE ;
			data_rotate_value = 0x08 ;
			break ;
		case GXnand:		/* 0xe NOT src OR NOT dst */
			invert_source_data = TRUE ;
			invert_existing_data = TRUE ;
		case GXorReverse:	/* 0xb src OR NOT dst */
			data_rotate_value = 0x10 ;
			break ;
		case GXorInverted:	/* 0xd NOT src OR dst */
			invert_source_data = TRUE ;
		case GXor:		/* 0x7 src OR dst */
			data_rotate_value = 0x10 ;
			break ;
		case GXcopyInverted:	/* 0xc NOT src */
			invert_source_data = TRUE ;
		case GXcopy:		/* 0x3 src */
		default:
			break ;
	}
	cursor_saved = !CursorChecking && vgaCheckCursor( x, y, w, h ) ;
	if ( invert_existing_data )
		vgaFillSolid( 0xF, GXinvert, planes, x, y, w, h ) ;
	/* Setup VGA Registers */
	outb( 0x3C4, 2 ) ;
	outb( 0x3C5, planes ) ;			/* Set Write Enable Planes */
	outb( 0x3CE, 3 ) ;
	outb( 0x3CF, data_rotate_value ) ;	/* Set Raster Op */
	outb( 0x3CE, 5 ) ;
	outb( 0x3CF, 2 ) ;			/* Set Write Mode 2 */
	/* Point To Bit Mask Register */
	outb( 0x3CE, 8 ) ;
	
}

StartByte = (volatile char *)
	( VGABASE + ( y * BYTES_PER_ROW ) + ROW_OFFSET( x ) ) ;
InitialMask = 0x80 >> BIT_OFFSET( x ) ;
RowIncrement = w + 3 & ~3 ;

for ( DataPtr = data ;
      h-- ;
      DataPtr += RowIncrement, StartByte += BYTES_PER_ROW ) {
	for ( dst = StartByte, src = DataPtr,
	      Pixel_Count = w, Mask = InitialMask ;
	      Pixel_Count-- ;
	      src++ ) {
		outb( 0x3CF, Mask ) ;
		/* Read To Load vga Data Latches */
		tmp = *( (VgaMemoryPtr) dst ) ;
		tmp = *src ;
		if ( invert_source_data )
			*( (VgaMemoryPtr) dst ) = ~tmp ;
		else
			*( (VgaMemoryPtr) dst ) = tmp ;
		if ( Mask & 1 ) {
			Mask = 0x80 ;
			dst++ ;
		}
		else
			Mask >>= 1 ;
	}
}
if ( cursor_saved )
	vgaReplaceCursor() ;

return ;
}

static unsigned long int read8Z( screen_ptr )
register volatile char *screen_ptr ;
{
register unsigned int i ;
register unsigned int j ;

/* Read One Byte At A Time to get
 *	i ==	[ Plane 3 ] [ Plane 2 ] [ Plane 1 ] [ Plane 0 ]
 * into a single register
 */
outb( 0x3CF, 3 ) ;
i = *( (VgaMemoryPtr) screen_ptr ) << 8 ;
outb( 0x3CF, 2 ) ;
i |= *( (VgaMemoryPtr) screen_ptr ) ;
i <<= 8 ;
outb( 0x3CF, 1 ) ;
i |= *( (VgaMemoryPtr) screen_ptr ) ;
i <<= 8 ;
outb( 0x3CF, 0 ) ;
i |= *( (VgaMemoryPtr) screen_ptr ) ;

/* Push Bits To Get
 * j ==	[Pixel 7][Pixel 6][Pixel 5][Pixel 4][Pixel 3][Pixel 2][Pixel 1][Pixel 0]
 * into one register
 */

j = ( i & 0x1 ) << 4 ;
j |= ( i >>= 1 ) & 0x1 ;
j <<= 4 ;
j |= ( i >>= 1 ) & 0x1 ;
j <<= 4 ;
j |= ( i >>= 1 ) & 0x1 ;
j <<= 4 ;
j |= ( i >>= 1 ) & 0x1 ;
j <<= 4 ;
j |= ( i >>= 1 ) & 0x1 ;
j <<= 4 ;
j |= ( i >>= 1 ) & 0x1 ;
j <<= 4 ;
j |= ( i >>= 1 ) & 0x1 ;

j |= ( i & 0x2 ) << 28 ;
j |= ( ( i >>= 1 ) & 0x2 ) << 24 ;
j |= ( ( i >>= 1 ) & 0x2 ) << 20 ;
j |= ( ( i >>= 1 ) & 0x2 ) << 16 ;
j |= ( ( i >>= 1 ) & 0x2 ) << 12 ;
j |= ( ( i >>= 1 ) & 0x2 ) <<  8 ;
j |= ( ( i >>= 1 ) & 0x2 ) <<  4 ;
j |= ( i >>= 1 ) & 0x2 ;

j |= ( i & 0x4 ) << 28 ;
j |= ( ( i >>= 1 ) & 0x4 ) << 24 ;
j |= ( ( i >>= 1 ) & 0x4 ) << 20 ;
j |= ( ( i >>= 1 ) & 0x4 ) << 16 ;
j |= ( ( i >>= 1 ) & 0x4 ) << 12 ;
j |= ( ( i >>= 1 ) & 0x4 ) <<  8 ;
j |= ( ( i >>= 1 ) & 0x4 ) <<  4 ;
j |= ( i >>= 1 ) & 0x4 ;

j |= ( i & 0x8 ) << 28 ;
j |= ( ( i >>= 1 ) & 0x8 ) << 24 ;
j |= ( ( i >>= 1 ) & 0x8 ) << 20 ;
j |= ( ( i >>= 1 ) & 0x8 ) << 16 ;
j |= ( ( i >>= 1 ) & 0x8 ) << 12 ;
j |= ( ( i >>= 1 ) & 0x8 ) <<  8 ;
j |= ( ( i >>= 1 ) & 0x8 ) <<  4 ;
j |= ( i >>= 1 ) & 0x8 ;

return j ;
}

void vgaReadColorImage( x, y, lx, ly, data )
int x, y ;
register int lx, ly ;
register unsigned char *data ;
{
register volatile unsigned char *src ;
register volatile unsigned char *s1ptr ;
register unsigned long int tmp ;
register int dx ;
register int skip ;
register int center_width ;
register int ignore ;
int pad ;
int cursor_saved ;

cursor_saved = !CursorChecking && vgaCheckCursor( x, y, lx, ly ) ;

/* Setup VGA Registers */
outb( 0x3CE, 5 ) ;
outb( 0x3CF, inb( 0x3CF ) & ~0x8 ) ;
outb( 0x3CE, 4 ) ;

skip = BIT_OFFSET( x ) ;
ignore = BIT_OFFSET( x + lx ) ;
pad = ( skip - ignore ) & 0x3 ;
src = (volatile unsigned char *) VGABASE + BYTE_OFFSET( x, y ) ;
center_width = ROW_OFFSET( x + lx ) - ROW_OFFSET( ( x + 0x7 ) & ~0x7 ) ;

#define SINGLE_STEP 	*data++ = tmp & 0xF ; tmp >>= 4

if ( center_width < 0 )
	for ( ;
	      ly-- ;
	      src += BYTES_PER_ROW ) {
		tmp = read8Z( src ) >> skip ;
		for ( dx = ignore - skip ; dx-- ; ) {
			SINGLE_STEP ;
		}
		for ( dx = pad ; dx-- ; )
			*data++ = 0 ;
	}
else
	for ( ;
	      ly-- ;
	      src += BYTES_PER_ROW ) {
		s1ptr = src ;
		tmp = read8Z( s1ptr++ ) ;
		if ( dx = skip ) {
			tmp >>= ( dx << 2 ) ;
			center_width++ ;
		}
	BranchPoint:
		switch ( dx ) {
		LoopTop:
		case 0x0: SINGLE_STEP ;
		case 0x1: SINGLE_STEP ;
		case 0x2: SINGLE_STEP ;
		case 0x3: SINGLE_STEP ;
		case 0x4: SINGLE_STEP ;
		case 0x5: SINGLE_STEP ;
		case 0x6: SINGLE_STEP ;
		case 0x7: *data++ = tmp & 0xF ;

			/* Fall Through To End Of Inner Loop */
			if ( center_width > 0 ) {
				tmp = read8Z( s1ptr++ ) ;
				center_width-- ;
				goto LoopTop ;
			}
			else if ( ( center_width == 0 )
			       && ( dx = ( - ignore ) & 07 ) ) {
				tmp = read8Z( s1ptr++ ) ;
				center_width-- ;
				goto BranchPoint ;
			}
			else if ( dx = pad )
				for ( dx = pad ; dx-- ; )
					*data++ = 0 ;
		}
	}

if ( cursor_saved )
	vgaReplaceCursor() ;

return ;
}
