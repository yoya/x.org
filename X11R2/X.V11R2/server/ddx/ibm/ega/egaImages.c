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
/* $Header:egaImages.c 1.1$ */
/* $ACIS:egaImages.c 1.1$ */
/* Author: Todd Newman  (aided and abetted by Mr. Drewry) */

#include "X.h"
#include "Xprotostr.h"

#include "misc.h"
#include "gcstruct.h"
#include "pixmapstr.h"
#include "windowstr.h"
#include "scrnintstr.h"
#include "mi.h"
#include "servermd.h"

/* MIGETPLANE -- gets a bitmap representing one plane of pDraw
 * A helper used for CopyPlane and XY format GetImage
 * No clever strategy here, we grab a scanline at a time, pull out the
 * bits and then stuff them in a 1 bit deep map.
 */
static
unsigned long *
miGetPlane( pDraw, planeNum, sx, sy, w, h, result )
	DrawablePtr	pDraw ;
	int		planeNum ;	/* number of the bitPlane */
	int		sx, sy, w, h ;
	unsigned long	*result ;
{
	int		i, j, k, depth, width, bitsPerPixel, widthInBytes ;
	DDXPointRec 	pt ;
	unsigned int 	*pline ;
	unsigned int	bit ;
	unsigned char	*pCharsOut ;
	CARD16		*pShortsOut ;
	CARD32		*pLongsOut ;

	depth = pDraw->depth ;
	if ( pDraw->type != DRAWABLE_PIXMAP ) {
		sx += ( (WindowPtr) pDraw )->absCorner.x ;
		sy += ( (WindowPtr) pDraw )->absCorner.y ;
	}
	widthInBytes = PixmapBytePad( w, 1 ) ;
	if ( !result )
		result = (unsigned long *) Xalloc( h * widthInBytes ) ;
	bitsPerPixel = GetBitsPerPixel( depth ) ;
	bzero( result, h * widthInBytes ) ;
	if ( BITMAP_SCANLINE_UNIT == 8 )
		pCharsOut = (unsigned char *) result ;
	else if ( BITMAP_SCANLINE_UNIT == 16 )
		pShortsOut = (CARD16 *) result ;
	else if ( BITMAP_SCANLINE_UNIT == 32 )
		pLongsOut = (CARD32 *) result ;
	if ( bitsPerPixel == 1 )
		pCharsOut = (unsigned char *) result ;
	for ( i = sy ; i < sy + h ; i++ ) {
		if ( bitsPerPixel == 1 ) {
			pt.x = sx ;
			pt.y = i ;
			width = w ;
			pline = (* pDraw->pScreen->GetSpans )
				( pDraw, width, &pt, &width, 1 ) ;
			bcopy( pline, pCharsOut, ( w + 7 ) / 8 ) ;
			pCharsOut += widthInBytes ;
			Xfree( pline ) ;
		}
		else {
			k = 0 ;
			for ( j = 0 ; j < w ; j++ ) {
				pt.x = sx + j ;
				pt.y = i ;
				width = 1 ;
				/* Fetch the next pixel */
				pline = (* pDraw->pScreen->GetSpans )
				    ( pDraw, width, &pt, &width, 1 ) ;
				bit = (unsigned int)
					( ( *pline >> planeNum ) & 1 ) ;
				/* Now insert that bit into a bitmap
				   in XY format */
				if ( BITMAP_BIT_ORDER == LSBFirst )
					bit <<= k ;
				else
					bit <<= ( ( BITMAP_SCANLINE_UNIT - 1 ) - k ) ;
				if ( BITMAP_SCANLINE_UNIT == 8 ) {
					*pCharsOut |= (unsigned char) bit ;
					k++ ;
					if ( k == 8 ) {
					    pCharsOut++ ;
					    k = 0 ;
					}
				}
				else if ( BITMAP_SCANLINE_UNIT == 16 ) {
					*pShortsOut |= (CARD16) bit ;
					k++ ;
					if ( k == 16 ) {
					    pShortsOut++ ;
					    k = 0 ;
					}
				}
				if ( BITMAP_SCANLINE_UNIT == 32 ) {
					*pLongsOut |= (CARD32) bit ;
					k++ ;
					if ( k == 32 ) {
					    pLongsOut++ ;
					    k = 0 ;
					}
				}
				Xfree( pline ) ;
			}
	
		}
	}
	return result ;

}

/* GETBITSPERPIXEL -- Find out how many bits per pixel are supported at
 * this depth -- another helper function
 */
static
int
GetBitsPerPixel( depth )
	register int depth ;
{
	register int i ;

	for ( i = 0 ; i < screenInfo.numPixmapFormats ; i++ )
		if ( screenInfo.formats[i].depth == depth )
			return screenInfo.formats[i].bitsPerPixel ;
	return 1 ;
}

/* MICOPYPLANE -- public entry for the CopyPlane request.
 * strategy:
 * First build up a bitmap out of the bits requested
 * build a source clip
 * Use the bitmap we've built up as a Stipple for the destination
 */
void
miCopyPlane( pSrcDrawable, pDstDrawable,
	     pGC, srcx, srcy, width, height, dstx, dsty, bitPlane )
DrawablePtr 	pSrcDrawable ;
DrawablePtr	pDstDrawable ;
GCPtr		pGC ;
int 		srcx, srcy ;
int 		width, height ;
int 		dstx, dsty ;
unsigned long	bitPlane ;
{
	unsigned long	*ptile ;
	BoxRec 		box ;
	RegionPtr	prgnSrc ;

	/* clip the left and top edges of the source */
	if ( srcx < 0 ) {
		width += srcx ;
		srcx = 0 ;
	}
	if ( srcy < 0 ) {
		height += srcy ;
		srcy = 0 ;
	}

	/* incorporate the source clip */

	if ( pSrcDrawable->type != DRAWABLE_PIXMAP ) {
		box.x1 = ((WindowPtr)pSrcDrawable)->absCorner.x ;
		box.y1 = ((WindowPtr)pSrcDrawable)->absCorner.y ;
		box.x2 = box.x1 + width ;
		box.y2 = box.y1 + height ;
		prgnSrc = (*pGC->pScreen->RegionCreate)(&box, 1) ;
	(*pGC->pScreen->Intersect)
		(prgnSrc, prgnSrc, ((WindowPtr)pSrcDrawable)->clipList) ;
	(*pGC->pScreen->TranslateRegion)(prgnSrc,
			- ( (WindowPtr) pSrcDrawable )->absCorner.x,
			- ( (WindowPtr) pSrcDrawable )->absCorner.y ) ;
	}
	else {
		box.x1 = 0 ;
		box.y1 = 0 ;
		box.x2 = ( (PixmapPtr) pSrcDrawable )->width ;
		box.y2 = ( (PixmapPtr) pSrcDrawable )->height ;
		prgnSrc = (* pGC->pScreen->RegionCreate )( &box, 1 ) ;
	}

	/* note that we convert the plane mask bitPlane into a plane number */
	ptile = miGetPlane( pSrcDrawable, ffs( bitPlane ) - 1, srcx, srcy,
   			    width, height, (unsigned long *) NULL ) ;
	miOpqStipDrawable( pDstDrawable, pGC, prgnSrc, ptile, 0,
			   width, height, dstx, dsty ) ;
	miHandleExposures( pSrcDrawable, pDstDrawable, pGC, srcx, srcy,
			   width, height, dstx, dsty ) ;
	Xfree( ptile ) ;
	(* pGC->pScreen->RegionDestroy )( prgnSrc ) ;
	return ;
}

/* EGAGETIMAGE -- public entry for the GetImage Request
 * We're getting the image into a memory buffer. While we have to use GetSpans
 * to read a line from the device (since we don't know what that looks like),
 * we can just write into the destination buffer
 *
 * two different strategies are used, depending on whether we're getting the
 * image in Z format or XY format
 * Z format:
 * Line at a time, GetSpans a line and bcopy it to the destination
 * buffer, except that if the planemask is not all ones, we create a
 * temporary pixmap and do a SetSpans into it (to get bits turned off)
 * and then another GetSpans to get stuff back (because pixmaps are
 * opaque, and we are passed in the memory to write into).  This is
 * completely ugly and slow but works, but the interfaces just aren't
 * designed for this case.  Life is hard.
 * XY format:
 * get the single plane specified in planemask
 */
void
egaGetImage( pDraw, sx, sy, w, h, format, planeMask, pdstLine )
	DrawablePtr 	pDraw ;
	int		sx, sy, w, h ;
	unsigned int 	format ;
	unsigned long 	planeMask ;
	pointer		pdstLine ;
{
	int		depth, i, linelength, width, srcx, srcy ;
	DDXPointRec	pt ;
	unsigned long	*pbits ;
	long		gcv[2] ;
	PixmapPtr	pPixmap = (PixmapPtr) NULL ;
	GCPtr		pGC ;
	unsigned long * pDst = (unsigned long *) pdstLine ;

	if ( pDraw->type != DRAWABLE_WINDOW ) {
		miGetImage( pDraw, sx, sy, w, h, format, planeMask, pdstLine ) ;
		return ;
	}

	depth = pDraw->depth ;
	if ( format != ZPixmap ) {
		miGetPlane( pDraw, ffs( planeMask ) - 1, sx, sy, w, h, pDst ) ;
	}
	else {
		if ( ( ( ( 1 << pDraw->depth ) - 1 ) & planeMask )
		      != ( 1 << pDraw->depth ) - 1 ) {
			pGC = GetScratchGC( depth, pDraw->pScreen ) ;
			pPixmap = (PixmapPtr) (* pDraw->pScreen->CreatePixmap )
					   ( pDraw->pScreen, w, h, depth ) ;
			gcv[0] = GXcopy ;
			gcv[1] = planeMask ;
			DoChangeGC( pGC, GCPlaneMask | GCFunction, gcv, 0 ) ;
			ValidateGC( pPixmap, pGC ) ;
		}

		linelength = PixmapBytePad( w, depth ) ;
		srcx = sx ;
		srcy = sy ;
		if ( pDraw->type == DRAWABLE_WINDOW ) {
			srcx += ( (WindowPtr) pDraw )->absCorner.x ;
			srcy += ( (WindowPtr) pDraw )->absCorner.y ;
		}
		for ( i = 0 ; i < h ; i++ ) {
			pt.x = srcx ;
			pt.y = srcy + i ;
			width = w ;
			pbits = (unsigned long *)
		(* pDraw->pScreen->GetSpans)( pDraw, w, &pt, &width, 1 ) ;
			if ( pPixmap ) {
			   pt.x = 0 ;
			   pt.y = 0 ;
			   width = w ;
			   (* pGC->SetSpans )( pPixmap, pGC, pbits, &pt,
					       &width, 1, TRUE ) ;
			   Xfree( pbits ) ;
			   pbits = (unsigned long *)
			  (* pDraw->pScreen->GetSpans )( pPixmap, w, &pt,
							 &width, 1 ) ;
			}
			bcopy( pbits, (char *) pDst, linelength ) ;
			pDst += linelength / sizeof( long ) ;
			Xfree( pbits ) ;
		}
		if ( pPixmap ) {
			(* pGC->pScreen->DestroyPixmap )( pPixmap ) ;
			FreeScratchGC( pGC ) ;
		}
	}
	return ;
}

/* MIPUTIMAGE -- public entry for the PutImage request
 * Here we benefit from knowing the format of the bits pointed to by pImage,
 * even if we don't know how pDraw represents them.
 * Three different strategies are used depending on the format
 * XYBitmap Format:
 * 	we just use the Opaque Stipple helper function to cover the destination
 * 	Note that this covers all the planes of the drawable with the
 *	foreground color (masked with the GC planemask) where there are 1 bits
 *	and the background color (masked with the GC planemask) where there are
 *	0 bits
 * XYPixmap format:
 *	what we're called with is a series of XYBitmaps, but we only want
 *	each XYPixmap to update 1 plane, instead of updating all of them.
 * 	we set the foreground color to be all 1s and the background to all 0s
 *	then for each plane, we set the plane mask to only effect that one
 *	plane and recursive call ourself with the format set to XYBitmap
 *	(This clever idea courtesy of RGD.)
 * ZPixmap format:
 *	This part is simple, just call SetSpans
 */
void
egaPutImage( pDraw, pGC, depth, x, y, w, h, leftPad, format, pImage )
	DrawablePtr	pDraw ;
	GCPtr		pGC ;
	int 		depth, x, y, w, h, leftPad ;
	unsigned int	format ;
	unsigned char	*pImage ;
{
	int		i ;

	switch ( format ) {
	case XYBitmap: {
		BoxRec		box ;
		RegionPtr	prgnSrc ;

		box.x1 = 0 ;
		box.y1 = 0 ;
		box.x2 = w ;
		box.y2 = h ;
		prgnSrc = (* pGC->pScreen->RegionCreate )( &box, 1 ) ;

		miOpqStipDrawable( pDraw, pGC, prgnSrc, pImage, leftPad,
				   ( w - leftPad ), h, x, y ) ;
		(* pGC->pScreen->RegionDestroy )( prgnSrc ) ;
		break ;
	}

	case XYPixmap: {
		unsigned long gcv[3] ;
		unsigned long oldFg, oldBg ;
		unsigned long oldPlanemask ;

		depth = pGC->depth ;
		oldPlanemask = pGC->planemask ;
		oldFg = pGC->fgPixel ;
		oldBg = pGC->bgPixel ;
		gcv[0] = ~0 ;
		gcv[1] = 0 ;
		DoChangeGC( pGC, GCForeground | GCBackground, gcv, 0 ) ;

		for ( i = 1 << ( depth - 1 ) ; i > 0 ; i >>= 1 )
			if ( i & oldPlanemask ) {
				gcv[0] = i ;
				DoChangeGC( pGC, GCPlaneMask, gcv, 0 ) ;
				ValidateGC( pDraw, pGC ) ;
				(* pGC->PutImage )( pDraw, pGC, 1, x, y,
						    w, h, leftPad, XYBitmap,
						    pImage ) ;
				pImage += h * PixmapBytePad( w, 1 ) ;
			}
		gcv[0] = oldPlanemask ;
		gcv[1] = oldFg ;
		gcv[2] = oldBg ;
		DoChangeGC( pGC, GCPlaneMask | GCForeground | GCBackground,
			    gcv, 0 ) ;
		break ;
	}

	case ZPixmap: {
		DDXPointPtr pptFirst, ppt ;
		int *pwidthFirst, *pwidth ;

		ppt = pptFirst = (DDXPointPtr)
			ALLOCATE_LOCAL( h * sizeof( DDXPointRec ) ) ;
		pwidth = pwidthFirst = (int *)
			ALLOCATE_LOCAL( h * sizeof( int ) ) ;
		if ( !ppt || !pwidth ) {
		   if ( ppt )
			   DEALLOCATE_LOCAL( ppt ) ;
		   else if ( pwidth )
			   DEALLOCATE_LOCAL( pwidthFirst ) ;
		   return ;
		}
		if ( ( pDraw->type == DRAWABLE_WINDOW )
		  && ( pGC->miTranslate ) ) {
			x += ( (WindowPtr) pDraw )->absCorner.x ;
			y += ( (WindowPtr) pDraw )->absCorner.y ;
		}

		for ( i = 0 ; i < h ; i++ ) {
			ppt->x = x ;
			ppt->y = y + i ;
			ppt++ ;
			*pwidth++ = w ;
		}

		(* pGC->SetSpans )( pDraw, pGC, pImage, pptFirst,
				    pwidthFirst, h, TRUE ) ;
		DEALLOCATE_LOCAL( pptFirst ) ;
		DEALLOCATE_LOCAL( pwidthFirst ) ;
		break ;
	}
	default :
		ErrorF( "Unknown Format" ) ;
		break ;
	}
	return ;
}
