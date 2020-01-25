/* $XConsortium: vgaStipple.c,v 1.2 94/10/12 21:06:18 kaleb Exp $ */
/* $XFree86: xc/programs/Xserver/hw/xfree86/vga16/ibm/vgaStipple.c,v 3.0 1994/05/04 15:03:51 dawes Exp $ */
/*
 * Copyright IBM Corporation 1987,1988,1989
 *
 * All Rights Reserved
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted,
 * provided that the above copyright notice appear in all copies and that 
 * both that copyright notice and this permission notice appear in
 * supporting documentation, and that the name of IBM not be
 * used in advertising or publicity pertaining to distribution of the
 * software without specific, written prior permission.
 *
 * IBM DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
 * ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
 * IBM BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
 * ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
 * ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 *
*/


#include "X.h"
#include "pixmapstr.h"
#include "OScompiler.h"
#include "vgaVideo.h"
#include "vgaReg.h"

#include "windowstr.h" /* GJA -- for pWin */
#include "scrnintstr.h" /* GJA -- for pWin */
#include "pixmapstr.h" /* GJA -- for pWin */
#include "ppc.h" /* GJA -- for pWin */

static unsigned char
getbits( x, patternWidth, lineptr )
register const int x ;
register const unsigned int patternWidth ;
register const unsigned char * const lineptr ;
{
register unsigned char bits ;
register const unsigned char *cptr ;
register shift ;
register wrap ;

cptr = lineptr + ( x >> 3 ) ;
bits = *cptr ;
if ( shift = x & 7 )
	bits = SCRLEFT8( bits, shift ) | SCRRIGHT8( cptr[1], ( 8 - shift ) ) ;
if ( ( wrap = x + 8 - patternWidth ) > 0 ) {
	bits &= SCRLEFT8( 0xFF, wrap ) ;
	bits |= SCRRIGHT8( *lineptr, ( 8 - wrap ) ) ;
}

/* GJA -- Handle extraction of 8 bits from < 8 bits wide stipple.
 * I duplicated case 4,5,6,7 to give the compiler a chance to optimize.
 */
switch (patternWidth) {
case 1:	/* Not really useful. */
	bits &= ~SCRRIGHT8(0xFF,1);
	bits |= SCRRIGHT8(bits,1); 
	bits |= SCRRIGHT8(bits,2);
	bits |= SCRRIGHT8(bits,4);
	break;
case 2:
	bits &= ~SCRRIGHT8(0xFF,2);
	bits |= SCRRIGHT8(bits,2); bits |= SCRRIGHT8(bits,4); break;
case 3:
	bits &= ~SCRRIGHT8(0xFF,3);
	bits |= (SCRRIGHT8(bits,3) | SCRRIGHT8(bits,6)); break;
case 4:
	bits = (bits & ~SCRRIGHT8(0xFF,4)) | SCRRIGHT8(bits,4); break;
case 5:
	bits = (bits & ~SCRRIGHT8(0xFF,5)) | SCRRIGHT8(bits,5); break;
case 6:
	bits = (bits & ~SCRRIGHT8(0xFF,6)) | SCRRIGHT8(bits,6); break;
case 7:
	bits = (bits & ~SCRRIGHT8(0xFF,7)) | SCRRIGHT8(bits,7); break;
default:
	;
	/* Do nothing, of course */
}
return bits ;
}

extern void vgaFillSolid() ;

/* GJA --
 * Basically, in the code below, we will draw a stipple in the usual
 * three parts: left edge, center and right edge.
 * For efficiency reasons, the center will be drawn byte aligned, so that
 * we will have to shuffle the bits in the left and right edges.
 * The hard cases will be stipples with width < 8: In order to get 8
 * bits from those, we will need a loop. One single 'if' will never do.
 * This is taken care of above.
 */
static void
DoMonoSingle( pWin, w, x, y, mastersrc, h, width, paddedByteWidth, height,
	      xshift, yshift )
WindowPtr pWin; /* GJA */
int w, x, y ;
register const unsigned char *mastersrc ;
int h ;
register unsigned int width ;
register unsigned int paddedByteWidth ;
unsigned int height ;
int xshift ;
int yshift ;
{
register volatile unsigned char *xDst ;
register VideoAdapterObject tmp2 ;
register int NeedValX ;
register int counter ;
register int tmp1 ;
unsigned int rowCounter ;
int byte_cnt ;

/* Do Left Edge */
if ( tmp1 = x & 07 ) {
	tmp2 = SCRRIGHT8( ( (unsigned) 0xFF ), tmp1 ) ;
	/* Catch The Cases Where The Entire Region Is Within One Byte */
	if ( ( w -= 8 - tmp1 ) < 0 ) {
		tmp2 &= SCRLEFT8( (unsigned) 0xFF, -w ) ;
		w = 0 ;
	}
	SetVideoGraphics( Bit_MaskIndex, tmp2 ) ; /* Set The Bit Mask */
	/*
	 * For Each Line In The Source Pixmap
	 */
	xDst = SCREENADDRESS( pWin, x, y ); VSETRW(xDst);
	for ( tmp1 = yshift, rowCounter = h;
	      rowCounter ;
	      rowCounter-- , tmp1++ ) {

		if ( tmp1 >= height )
			tmp1 -= height ;
		/* Read To Save */
		tmp2 = *( (VgaMemoryPtr) xDst) ;
		/* Write Pattern */
		*( (VgaMemoryPtr) xDst ) =
			getbits( xshift /* GJA */, width,
				 mastersrc
				 + ( tmp1 * paddedByteWidth ) ) >> (x & 07) ;
 		xDst += BYTES_PER_LINE(pWin); VCHECKRWO(xDst);
	}
	NeedValX = (xshift + 8 - (x & 07)) % width;
	x = ( x + 7 ) & ~07 ;
}
else {
	NeedValX = xshift ;
}

if ( byte_cnt = ROW_OFFSET( w ) ) { /* Fill The Center Of The Box */
	int SavNeedX = NeedValX ;

	SetVideoGraphics( Bit_MaskIndex, 0xFF ) ; /* Set The Bit Mask */
	/*
	 * For Each Line In The Source Pixmap
	 */
	xDst = SCREENADDRESS( pWin, x, y ); VSETRW(xDst);
	for ( tmp1 = yshift, rowCounter = h;
	      rowCounter ;
	      rowCounter-- , tmp1++ ) {
		register const unsigned char *l_ptr ;
		if ( tmp1 >= height )
			tmp1 -= height ;
		l_ptr = mastersrc + ( tmp1 * paddedByteWidth ) ;
		/*
		 * For Each Byte Across The Pattern In X
		 */
		for ( counter = byte_cnt, NeedValX = SavNeedX ;
		      counter-- ; ) {
			/* Read To Save */
			tmp2 = *( (VgaMemoryPtr) xDst) ;
			/* Write Pattern */
			*( (VgaMemoryPtr) xDst ) =
				getbits( NeedValX, width, l_ptr ) ;
			/* GJA -- The '%' is there since width could be < 8 */
			NeedValX = (NeedValX + 8) % width;
			xDst++; VCHECKRWO(xDst);
		}
		xDst += BYTES_PER_LINE(pWin) - byte_cnt; VCHECKRWO(xDst);
	}
}

/* Do Right Edge */
if ( tmp1 = BIT_OFFSET( w ) ) { /* x Now Is Byte Aligned */
	/* Set The Bit Mask */
	SetVideoGraphics( Bit_MaskIndex, SCRLEFT8( 0xFF, ( 8 - tmp1 ) ) ) ;
	/*
	 * For Each Line In The Source Pixmap
	 */
	xDst = SCREENADDRESS( pWin, ( x + w ), y ); VSETRW(xDst);
	for ( tmp1 = yshift, rowCounter = h;
	      rowCounter ;
	      rowCounter-- , tmp1++ ) {
		if ( tmp1 >= height )
			tmp1 -= height ;
		/* Read To Save */
		tmp2 = *( (VgaMemoryPtr) xDst) ;
		/* Write Pattern */
		*( (VgaMemoryPtr) xDst ) =
			getbits( NeedValX, width,
				 mastersrc
				 + ( tmp1 * paddedByteWidth ) ) ;
		xDst += BYTES_PER_LINE(pWin) ; VCHECKRWO(xDst);
	}
}

return ;
}

static void
DoMonoMany( pWin, w, x, y, mastersrc, h, width, paddedByteWidth, height,
	    xshift, yshift )
WindowPtr pWin; /* GJA */
int w, x, y ;
register const unsigned char *mastersrc ;
int h ;
register unsigned int width ;
register unsigned int paddedByteWidth ;
unsigned int height ;
int xshift ;
int yshift ;
{
register volatile unsigned char *xDst ;
register VideoAdapterObject tmp2 ;
register int NeedValX ;
register int byte_cnt ;
register int tmp1 ;
unsigned DestinationRow ;
unsigned int SourceRow ;
volatile unsigned char *dst ;
int scr_incr = ( height * BYTES_PER_LINE(pWin) ) ;

/* Do Left Edge */
if ( tmp1 = x & 07 ) {
	tmp2 = SCRRIGHT8( ( (unsigned) 0xFF ), tmp1 ) ;
	/* Catch The Cases Where The Entire Region Is Within One Byte */
	if ( ( w -= 8 - tmp1 ) < 0 ) {
		tmp2 &= SCRLEFT8( (unsigned) 0xFF, -w ) ;
		w = 0 ;
	}
	SetVideoGraphics( Bit_MaskIndex, tmp2 ) ; /* Set The Bit Mask */
	/*
	 * For Each Line In The Source Pixmap
	 */
	for ( tmp1 = yshift, SourceRow = 0, dst = SCREENADDRESS( pWin, x, y ) ;
	      SourceRow < height ;
	      tmp1++, SourceRow++, dst += BYTES_PER_LINE(pWin) ) {
		register unsigned bitPattern ;

		if ( tmp1 >= height )
			tmp1 -= height ;
		/*
		 * For Each Time Pattern Repeats In The Y Dimension
		 */
		xDst = dst; VSETRW(xDst);
		for ( DestinationRow = SourceRow,
		      bitPattern = getbits( xshift, width,
					    mastersrc
					+ ( tmp1 * paddedByteWidth ) ) ;
		      DestinationRow < h ;
		      DestinationRow += height ) {
			/* Read To Save */
			tmp2 = *( (VgaMemoryPtr) xDst ) ;
			/* Write Pattern */
			*( (VgaMemoryPtr) xDst ) = bitPattern >> (x & 07);
		        xDst += scr_incr; VCHECKRWO(xDst);
		}
	}
	NeedValX = (xshift + 8 - (x & 07)) % width;
	x = ( x + 7 ) & ~07 ;
}
else {
	NeedValX = xshift ;
}

if ( byte_cnt = ROW_OFFSET( w ) ) { /* Fill The Center Of The Box */
	int SavNeedX = NeedValX ;

	SetVideoGraphics( Bit_MaskIndex, 0xFF ) ; /* Set The Bit Mask */
	/*
	 * For Each Line In The Source Pixmap
	 */
	for ( tmp1 = yshift, SourceRow = 0, dst = SCREENADDRESS( pWin, x, y ) ;
	      SourceRow < height ;
	      tmp1++, SourceRow++, dst += BYTES_PER_LINE(pWin) - byte_cnt ) {
		register const unsigned char *l_ptr ;
		if ( tmp1 >= height )
			tmp1 -= height ;
		l_ptr = mastersrc + ( tmp1 * paddedByteWidth ) ;
		/*
		 * For Each Byte Across The Pattern In X
		 */
		for ( tmp2 = byte_cnt, NeedValX = SavNeedX ;
		      tmp2-- ;
		      dst++ ) {
			register unsigned bitPattern ;
			register VideoAdapterObject tmp3 ;
			/*
			 * For Each Time Pattern Repeats In Y
			 */
			xDst = dst; VSETRW(xDst);
			for ( DestinationRow = SourceRow,
			      bitPattern = getbits( NeedValX, width, l_ptr ) ;
			      DestinationRow < h ;
			      DestinationRow += height ) {
				/* Read To Save */
				tmp3 = *( (VgaMemoryPtr) xDst) ;
				/* Write Pattern */
				*( (VgaMemoryPtr) xDst ) = bitPattern ;
			        xDst += scr_incr; VCHECKRWO(xDst);
			}
			NeedValX = (NeedValX + 8) % width;
		}
	}
}

/* Do Right Edge */
if ( tmp1 = BIT_OFFSET( w ) ) { /* x Now Is Byte Aligned */
	/* Set The Bit Mask */
	SetVideoGraphics( Bit_MaskIndex, SCRLEFT8( 0xFF, ( 8 - tmp1 ) ) ) ;
	/*
	 * For Each Line In The Source Pixmap
	 */
	for ( tmp1 = yshift, SourceRow = 0,
	      dst = SCREENADDRESS( pWin, ( x + w ), y ) ;
	      SourceRow < height ;
	      tmp1++, SourceRow++, dst += BYTES_PER_LINE(pWin) ) {
		register unsigned bitPattern ;
		if ( tmp1 >= height )
			tmp1 -= height ;
		/*
		 * For Each Time Pattern Repeats In The Y Dimension
		 */
		xDst = dst; VSETRW(xDst);
		for ( DestinationRow = SourceRow,
		      bitPattern = getbits( NeedValX, width,
					    mastersrc
					+ ( tmp1 * paddedByteWidth ) ) ;
		      DestinationRow < h ;
		      DestinationRow += height ) {
			/* Read To Save */
			tmp2 = *( (VgaMemoryPtr) xDst) ;
			/* Write Pattern */
			*( (VgaMemoryPtr) xDst ) = bitPattern ;
		        xDst += scr_incr; VCHECKRWO(xDst);
		}
	}
}

return ;
}

static void
vgaSetMonoRegisters( plane_mask, desiredState )
register unsigned long int plane_mask ;
register unsigned long int desiredState ;
{
/* Setup VGA Registers */
/*
 * Set The Plane-Enable
 */
SetVideoSequencer( Mask_MapIndex, plane_mask ) ;
SetVideoGraphics( Enb_Set_ResetIndex, plane_mask ) ;
/*
 * Put Display Into SET-AND (i.e. Write Mode 3 )
 */
SetVideoGraphics( Graphics_ModeIndex, VGA_WRITE_MODE_3 ) ;
/*
 * Set The Color in The Set/Reset Register
 */
SetVideoGraphics( Set_ResetIndex, desiredState & VGA_ALLPLANES ) ;
/*
 * Set The Vga's Alu Function
 */
SetVideoGraphics( Data_RotateIndex, desiredState >> 8 ) ;

return ;
}

static unsigned long
vgaCalcMonoMode( rasterOp, color )
int rasterOp ;
register unsigned long int color ;
{
#define DO_RECURSE 0x10000
register unsigned int data_rotate_value = VGA_COPY_MODE << 8 ;
register unsigned int invert_existing_data = 0 ;

/* Test The Raster-Op */
switch ( rasterOp ) {
	case GXclear:		/* 0x0 Zero 0 */
		color = 0 ;
		break ;
	case GXinvert:		/* 0xa NOT dst */
		data_rotate_value = VGA_XOR_MODE << 8 ;
	case GXset:		/* 0xf 1 */
		color = VGA_ALLPLANES ;
		break ;
	case GXnor:		/* 0x8 NOT src AND NOT dst */
		invert_existing_data = DO_RECURSE ;
	case GXandInverted:	/* 0x4 NOT src AND dst */
		color = ~color ;
	case GXand:		/* 0x1 src AND dst */
		data_rotate_value = VGA_AND_MODE << 8 ;
	case GXcopy:		/* 0x3 src */
		break ;
	case GXequiv:		/* 0x9 NOT src XOR dst */
		color = ~color ;
	case GXxor:		/* 0x6 src XOR dst */
		data_rotate_value = VGA_XOR_MODE << 8 ;
		break ;
	case GXandReverse:	/* 0x2 src AND NOT dst */
		invert_existing_data = DO_RECURSE ;
		data_rotate_value = VGA_AND_MODE << 8 ;
		break ;
	case GXorReverse:	/* 0xb src OR NOT dst */
		invert_existing_data = DO_RECURSE ;
		data_rotate_value = VGA_OR_MODE << 8 ;
		break ;
	case GXnand:		/* 0xe NOT src OR NOT dst */
		invert_existing_data = DO_RECURSE ;
	case GXorInverted:	/* 0xd NOT src OR dst */
		color = ~color ;
	case GXor:		/* 0x7 src OR dst */
		data_rotate_value = VGA_OR_MODE << 8 ;
		break ;
	case GXcopyInverted:	/* 0xc NOT src */
		color = ~color ;
		break ;
	case GXnoop:		/* 0x5 dst */
		; /* Shouldn't Get Here !! */
}

return ( color & VGA_ALLPLANES ) | data_rotate_value | invert_existing_data ;
}

void
vgaDrawMonoImage( pWin, data, x, y, w, h, fg, alu, planes )
WindowPtr pWin; /* GJA */
unsigned char *data;
int x, y, w, h ;
unsigned long int fg ;
int alu ;
unsigned long int planes;
{
unsigned long regState ;

if ( !xf86VTSema ) {
	offDrawMonoImage( pWin, data, x, y, w, h, fg, alu, planes );
	return;
}

if ( ( alu == GXnoop ) || !( planes &= VGA_ALLPLANES ) )
	return ;

if ( ( regState = vgaCalcMonoMode( alu, fg ) ) & DO_RECURSE ) {
	vgaDrawMonoImage( pWin, data, x, y, w, h,
			  VGA_ALLPLANES, GXinvert, planes ) ;
	regState &= ~DO_RECURSE ;
}


vgaSetMonoRegisters( planes, regState ) ;

DoMonoSingle( pWin, w, x, y, (const unsigned char *) data, h,
	      w, ( ( w + 31 ) & ~31 ) >> 3, h, 0, 0 ) ;


return ;
}

void
vgaFillStipple( pWin, pStipple, fg, alu, planes, x, y, w, h, xSrc, ySrc )
WindowPtr pWin; /* GJA */
register PixmapPtr const pStipple ;
unsigned long int fg ;
const int alu ;
unsigned long int planes ;
int x, y, w, h ;
const int xSrc, ySrc ;
{
unsigned int width ;
unsigned int height ;
int xshift ;
int yshift ;
unsigned long regState ;

if ( !xf86VTSema ) {
	offFillStipple( pWin, pStipple, fg, alu, planes,
			x, y, w, h, xSrc, ySrc );
	return;
}

if ( ( alu == GXnoop ) || !( planes &= VGA_ALLPLANES ) )
	return ;

if ( ( regState = vgaCalcMonoMode( alu, fg ) ) & DO_RECURSE ) {
	vgaFillStipple( pWin, pStipple, VGA_ALLPLANES, GXinvert, planes,
			x, y, w, h, xSrc, ySrc ) ;
	regState &= ~DO_RECURSE ;
}


vgaSetMonoRegisters( planes, regState ) ;

/* Figure Bit Offsets & Source Address */
width = pStipple->drawable.width ;
if ( ( xshift = ( x - xSrc ) ) < 0 )
	xshift = width - ( ( - xshift ) % width ) ;
else
	xshift %= width ;
if ( xshift == width ) xshift = 0;

height = pStipple->drawable.height ;
if ( ( yshift = ( y - ySrc ) ) < 0 )
	yshift = height - ( ( - yshift ) % height ) ;
else
	yshift %= height ;
if ( yshift == height ) yshift = 0;

	(* ( (h > height) ? DoMonoMany : DoMonoSingle ) ) (
			pWin, w, x, y,
			(const unsigned char *) pStipple->devPrivate.ptr,
			h,
			width,
			( ( width + 31 ) & ~31 ) >> 3,
			height,
			xshift, yshift
			) ;

return ;
}
