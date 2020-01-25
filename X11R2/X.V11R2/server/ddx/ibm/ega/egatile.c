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
/* $Header:egatile.c 1.2$ */
/* $ACIS:egatile.c 1.2$ */
/* $Source: /ibm/acis/usr/src/X11/server/ddx/ibm/ega/RCS/egatile.c,v $ */

#ifndef lint
static char *rcsid = "$Header:egatile.c 1.2$" ;
#endif

/* ega Tile
 * Uses private imageFill a bunch of times
 */

#include "X.h"
#include "gcstruct.h"
#include "scrnintstr.h"
#include "pixmapstr.h"
#include "pixmap.h"
#include "rtutils.h"
#include "misc.h"
#include "colormap.h"
#include "ppc.h"
#include "ppcprocs.h"

extern void egaFillSolid( ) ;
extern void ErrorF( ) ;
extern void ppcRotZ8mapLeft( ) ;
extern void ppcRotZ8mapUp( ) ;
extern void mfbDestroyPixmap( ) ;
extern void egaDrawColorImage( ) ;
extern void ppcClipZ8Pixmap( ) ;
extern void egaBitBlt( ) ;

/* Declared in "egacurs.c" */
extern int egaCursorChecking ;
extern int egaCheckCursor() ;
extern void egaReplaceCursor() ;

void egaTileRect( pTile, alu, planes, x, y, w, h, xSrc, ySrc )
register PixmapPtr pTile ;
const int alu, planes ;
register int x, y, w, h ;
{
register int	htarget, vtarget ;
register int 	tlx, tly ;
int		depth ;
register PixmapPtr 	pTmpTile ;
ScreenPtr	pScreen ;
int		TileCopied ;
int		cursor_saved ;

TRACE( ( "egaTileRect(pTile = x%x, alu= x%x, planes=x%02x, x=%d, y=%d, w=%d, h=%d, xSrc=%d, ySrc=%d\n",
		pTile, alu, planes, x, y, w, h, xSrc, ySrc ) ) ;

switch ( alu ) {
	case GXclear:		/* 0x0 Zero 0 */
	case GXinvert:		/* 0xa NOT dst */
	case GXset:		/* 0xf 1 */
		egaFillSolid( 0xF, alu, planes, x, y, w, h ) ;
	case GXnoop:		/* 0x5 dst */
		return ;
	default:
		break ;
}

tlx = pTile->width ;
tly = pTile->height ;
pScreen = pTile->drawable.pScreen ;
depth   = pTile->drawable.depth ;

if ( ( x - xSrc ) % tlx ) {
	if ( !( pTmpTile = ppcCreatePixmap( pScreen, tlx, tly, depth ) ) ) {
		ErrorF( "Failed To Create Temporary Pixmap in egaTileRect\n" ) ;
		return ;
	}
	ppcRotZ8mapLeft( pTile, pTmpTile, ( ( x - xSrc ) % tlx ) ) ;
	if ( ( y - ySrc ) % tly ) {
		if ( !( pTile = ppcCreatePixmap( pScreen, tlx, tly, depth ) ) ) {
			ErrorF( "Failed To Create Pixmap \"pTile\" in egaTileRect\n" ) ;
			return ;
		}
		ppcRotZ8mapUp( pTmpTile, pTile, ( ( y - ySrc ) % tly ) ) ;
		mfbDestroyPixmap( pTmpTile ) ;
	}
	else
		pTile = pTmpTile ;
	TileCopied = 1 ;
}
else if ( ( y - ySrc ) % tly ) {
	if ( !( pTmpTile = ppcCreatePixmap( pScreen, tlx, tly, depth ) ) ) {
		ErrorF( "Failed To Create Pixmap \"pTmpTile\" in egaTileRect\n" ) ;
		return ;
	}
	ppcRotZ8mapUp( pTile, pTmpTile, ( ( y - ySrc ) % tly ) ) ;
	pTile = pTmpTile ;
	TileCopied = 1 ;
}
else {
	TileCopied = 0 ;
}

cursor_saved = !egaCursorChecking && egaCheckCursor( x, y, w, h ) ;
/* Incr. Semaphore Now To Avoid Checking In Each Low Level Call */
egaCursorChecking++ ;

switch ( alu ) {
	case GXcopyInverted:	/* 0xc NOT src */
	case GXcopy:		/* 0x3 src */
		/* Special Case Code */
		vtarget = ( h > tly ) ? tly : h ;
		if ( w >= tlx ) {
			/* Note: Drawing with Height Less Than
			   The Actual Height Of The Image Will
			   Work Correctly */
			egaDrawColorImage( x, y,
					   htarget = tlx,
					   vtarget,
					   pTile->devPrivate,
					   alu, planes ) ;
		}
		else {
			if ( !( pTmpTile = ppcCreatePixmap( pScreen,
							    htarget = w,
					   		    vtarget,
							    depth ) ) ) {
				ErrorF( "Failed To Create Short Pixmap in egaTileRect\n" ) ;
				if ( !--egaCursorChecking && cursor_saved )
					egaReplaceCursor() ;
				return ;
			}
			ppcClipZ8Pixmap( pTile, pTmpTile ) ;
			egaDrawColorImage( x, y,
					   htarget,
					   vtarget,
					   pTmpTile->devPrivate,
					   alu, planes ) ;
			mfbDestroyPixmap( pTmpTile ) ;
		}
		for ( ;
		      htarget <= ( w >> 1 ) ;
		      htarget <<= 1 ) {
			egaBitBlt( GXcopy, planes, planes,
				   x, y,
				   x + htarget, y,
				   htarget, vtarget ) ;
		}
		if ( w - htarget )
			egaBitBlt( GXcopy, planes, planes,
				   x, y,
				   x + htarget, y,
				   w - htarget, vtarget ) ;
		for ( ;
		      vtarget <= ( h >> 1 ) ;
		      vtarget <<= 1 ) {
			egaBitBlt( GXcopy, planes, planes,
				   x, y,
				   x, y + vtarget,
				   w, vtarget ) ;
		}
		if ( h - vtarget )
			egaBitBlt( GXcopy, planes, planes,
				   x, y,
				   x, y + vtarget,
				   w, h - vtarget ) ;
		break ;
	case GXnor:		/* 0x8 NOT src AND NOT dst */
	case GXandReverse:	/* 0x2 src AND NOT dst */
	case GXorReverse:	/* 0xb src OR NOT dst */
	case GXnand:		/* 0xe NOT src OR NOT dst */
	case GXandInverted:	/* 0x4 NOT src AND dst */
	case GXand:		/* 0x1 src AND dst */
	case GXequiv:		/* 0x9 NOT src XOR dst */
	case GXxor:		/* 0x6 src XOR dst */
	case GXorInverted:	/* 0xd NOT src OR dst */
	case GXor:		/* 0x7 src OR dst */
	default:
		{
		register int	hcount, vcount ;
		register int	savevcount ;
		register int	savehcount ;
		char 		*data ;
		int		Rtlx, Btly ;
	
		savevcount = h / tly ;
		savehcount = w / tlx ;
	
		/* FIRST DO COMPLETELY VISIBLE PORTIONS */
		for ( data = (char *) pTile->devPrivate,
		      vcount = savevcount,
		      vtarget = y ;
		      vcount-- ;
		      vtarget += tly ) {
			for ( hcount = savehcount,
			      htarget = x ;
			      hcount-- ;
			      htarget += tlx ) {
				egaDrawColorImage( htarget, vtarget,
						   tlx, tly, data,
						   alu, planes ) ;
			}
		}
		
		/* NOW DO RIGHT HAND SIDE */
		if ( Rtlx = w % tlx ) {
			if ( !( pTmpTile = ppcCreatePixmap( pScreen, Rtlx, tly, depth ) ) ) {
				ErrorF( "Failed To Create Right Pixmap in egaTileRect\n" ) ;
				if ( !--egaCursorChecking && cursor_saved )
					egaReplaceCursor() ;
				return ;
			}
			ppcClipZ8Pixmap( pTile, pTmpTile ) ;
		
			for ( data = (char *) pTmpTile->devPrivate,
			      vcount = savevcount,
			      htarget = x + savehcount * tlx,
			      vtarget = y ;
			      vcount-- ;
			      vtarget += tly ) {
				egaDrawColorImage( htarget, vtarget,
						   Rtlx, tly, data,
						   alu, planes ) ;
			}
			mfbDestroyPixmap( pTmpTile ) ;
		}
		
		/* NOW DO BOTTOM */
		if ( Btly = h % tly ) {
			for ( data = (char *) pTile->devPrivate,
			      hcount = savehcount,
			      htarget = x,
			      vtarget = y + savevcount * tly ;
			      hcount-- ;
			      htarget += tlx ) {
				egaDrawColorImage( htarget, vtarget,
						   tlx, Btly, data,
						   alu, planes ) ;
			}
			if ( Rtlx ) {
			/* NOW DO BOTTOM RIGHT CORNER */
				if ( !( pTmpTile = ppcCreatePixmap( pScreen, Rtlx, Btly, depth ) ) ) {
					ErrorF( "Failed To Create Corner Pixmap in egaTileRect\n" ) ;
					if ( !--egaCursorChecking && cursor_saved )
						egaReplaceCursor() ;
					return ;
				}
				ppcClipZ8Pixmap( pTile, pTmpTile ) ;
				egaDrawColorImage( ( x + savehcount * tlx ),
						   ( y + savevcount * tly ),
						   Rtlx, Btly, pTmpTile->devPrivate,
						   alu, planes ) ;
				mfbDestroyPixmap( pTmpTile ) ;
			}
		}
	}
	break ;
}

if ( TileCopied )
	mfbDestroyPixmap( pTile ) ;

if ( !--egaCursorChecking && cursor_saved )
	egaReplaceCursor() ;

return ;
}
