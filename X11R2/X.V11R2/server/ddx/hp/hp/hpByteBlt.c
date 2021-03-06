/*

Copyright (c) 1986, 1987 by Hewlett-Packard Company
Copyright (c) 1986, 1987 by the Massachusetts Institute of Technology

Permission to use, copy, modify, and distribute this
software and its documentation for any purpose and without
fee is hereby granted, provided that the above copyright
notice appear in all copies and that both that copyright
notice and this permission notice appear in supporting
documentation, and that the name of M.I.T. not be used in
advertising or publicity pertaining to distribution of the
software without specific, written prior permission.

HEWLETT-PACKARD MAKES NO WARRANTY OF ANY KIND WITH REGARD
TO THIS SOFWARE, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
PURPOSE.  Hewlett-Packard shall not be liable for errors 
contained herein or direct, indirect, special, incidental or 
consequential damages in connection with the furnishing, 
performance, or use of this material.

This software is not subject to any license of the American
Telephone and Telegraph Company or of the Regents of the
University of California.

*/
/***********************************************************************
 *  file: hpByteBlt.c
 *
 *  Byte-Per-Pixel 68000 (and hopefully Spectrum) bit/byte order
 *  image transfer routines. Specifically:
 *	hpGetByteImage
 *
 *		Hewlett Packard -- Corvallis Workstation Operation
 *		Project -- port of X11 to HP9000
 *		Harry Phinney -- MTS
 *
 *
 */

#include "X.h"
#include "Xprotostr.h"

#include "misc.h"
#include "gcstruct.h"
#include "pixmapstr.h"
#include "windowstr.h"
#include "scrnintstr.h"
#include "mi.h"
#include "regionstr.h"
#include "Xmd.h"
#include "servermd.h"
#include "cfb/cfb.h"


/* hpGetPlane -- gets a bitmap representing one plane of pDraw
 * A helper used for XY format GetImage 
 * No clever strategy here, we grab a scanline at a time, pull out the
 * bits and then stuff them in a 1 bit deep map.
 * This is a significantly modified version of miGetPlane.  The arguments
 * are different, as here we pass in the start address within the source 
 * pixmap, instead of the sx and sy within the pixmap, and we pass in the
 * stride that will get us to the next line - either the pixmap width or
 * the screen width depending on whether the pixmap is in framebuffer or
 * main memory.
 */
static
unsigned long	*
hpGetPlane(pDraw, planeNum, startAddr, w, h, stride, result)
    DrawablePtr		pDraw;
    int			planeNum;	/* number of the bitPlane */
    unsigned char *	startAddr;
    int			w, h;
    int			stride; 	/* stride to get to next line */
    unsigned long	*result;
{
    register int 	i, j;
    int			k, widthInBytes;
    register CARD32	bits;
    CARD8		*pCharsOut;
    register unsigned char	*pixAddr;
    register unsigned char	pixel;

    if(pDraw->depth != 8)
	FatalError("hpGetPlane: invalid depth\n");
    widthInBytes = PixmapBytePad(w, 1);
    if(!result)
        result = (unsigned long *)Xalloc(h * widthInBytes);

    for(i = 0; i < h; i++)
    {
    	pCharsOut = ((unsigned char *)result) + i * widthInBytes;
	pixAddr = startAddr + i * (stride);
	k = 0;
	bits = 0;
	for(j = 0; j < w; j++)
	{
	    pixel = *pixAddr++;
            if(BITMAP_BIT_ORDER == LSBFirst) {
	        bits = (bits << 1) | ((pixel >> planeNum) & 1);
	    }
	    else {
		bits |= ((pixel >> planeNum) & 1) << ((BITMAP_SCANLINE_UNIT - 1)
			  - k);
	    }
	    k++;
	    if(BITMAP_SCANLINE_UNIT == k) {
		switch (BITMAP_SCANLINE_UNIT) {
		    case 8: *pCharsOut++ = (CARD8)bits; break;
		    case 16: *(CARD16 *)pCharsOut = (CARD16) bits;
			     pCharsOut += sizeof(CARD16);
			     break;
		    case 32: *(CARD32 *)pCharsOut = bits;
			     pCharsOut += sizeof(CARD32);
			     break;
		}
		k = bits = 0;
	      }
	}
	if(k) {
	    switch (BITMAP_SCANLINE_UNIT) {
		case 8: *pCharsOut = (CARD8)bits; break;
		case 16: *(CARD16 *)pCharsOut =(CARD16) bits; break;
		case 32: *(CARD32 *)pCharsOut = bits; break;
		}
	}
    }
    return(result);    
}

/* hpGetByteImage -- public entry for the GetImage Request
 * We're getting the image into a memory buffer.
 * Since we know what an hp byte-deep framebuffer looks like, we can do
 * this much faster than miGetImage.
 *
 * two different strategies are used, depending on whether we're getting the
 * image in Z format or XY format
 * Z format:
 *   For each row
 *	For each column
 *	  Read the pixel into pdst
 *   If planeMask is not all ones  (yech)
 *	For each row
 *	  For each column
 *	    Mask the unwanted bits 
 *
 * XY format:
 *   Call hpGetBytePlane (see above in this file)
 *   
 *
 */
void
hpGetByteImage(pDraw, sx, sy, w, h, format, planeMask, pdstLine)
    DrawablePtr 	pDraw;
    int			sx, sy, w, h;
    unsigned int 	format;
    unsigned long 	planeMask;
    pointer             pdstLine;
{
    int			depth, i, linelength, srcx, srcy;
    GCPtr		pGC;
    unsigned long *     pDst = (unsigned long *)pdstLine;

    register unsigned char *pByteDst;
    register unsigned char *pSrcByte;
    unsigned char *pSrc;
    int widthSrc;

    depth = pDraw->depth;
    if(depth != 8) { /* we only work on byte-deep sources */
	miGetImage(pDraw, sx, sy, w, h, format, planeMask, pdstLine);
	return;
    }
	
    linelength = PixmapBytePad(w, depth);
    planeMask &= getPlanesMask(pDraw->pScreen);
    srcx = sx;
    srcy = sy;
    if(pDraw->type == DRAWABLE_WINDOW)
    {
	srcx += ((WindowPtr)pDraw)->absCorner.x;
	srcy += ((WindowPtr)pDraw)->absCorner.y;
        widthSrc = (int)
            ((cfbPrivScreenPtr)(pDraw->pScreen->devPrivate))->stride;
        pSrc = (unsigned char *)
            (((cfbPrivScreenPtr)(pDraw->pScreen->devPrivate))->bits) +
	       srcx + srcy * widthSrc;
    }
    else { /* we're getting from a pixmap */
	if(((PixmapPtr)pDraw)->devKind == PIXMAP_FRAME_BUFFER) {
	  cfbPrivPixmapPtr pPrivPix = (cfbPrivPixmapPtr)((PixmapPtr)
				       pDraw)->devPrivate;
          widthSrc = (int) ((cfbPrivScreenPtr)
				 (pDraw->pScreen->devPrivate))->stride;
	  srcx += ((hpChunk *)pPrivPix->pChunk)->x;
	  srcy += ((hpChunk *)pPrivPix->pChunk)->y;
        }
	else { /* main memory pixmap */
          widthSrc = (int) (((cfbPrivPixmapPtr)(((PixmapPtr)pDraw)->
				devPrivate))->stride);
         }
         pSrc = (unsigned char *) (((cfbPrivPixmapPtr)(((PixmapPtr)
				    pDraw)->devPrivate))->bits) +
				    srcx + srcy * widthSrc;
    }
    if(format == ZPixmap) 
    {
	WAIT_READY_TO_RENDER(pDraw->pScreen);

	for(i = 0; i < h; i++) { /* for each row */
	    register j;
	    pSrcByte = pSrc + i * widthSrc;
	    pByteDst = ((unsigned char *)pdstLine) + i * linelength; 

	    for(j = 0; j < w; j++) { /* for each pixel in row */
		*pByteDst++ = *pSrcByte++ & planeMask;
	    }
	}
    }
    else
    {
        (void) hpGetPlane(pDraw, ffs(planeMask) - 1, pSrc, w, h, 
			  widthSrc, pdstLine);
    }
}


/* hpPutByteImage -- public entry for the PutImage Request
 *  This is here to try to make XY pixmap puts work reasonably on
 *  our byte-deep framebuffers.
 */
void
hpPutByteImage(pDraw, pGC, depth, dstx, dsty, w, h, srcOffset, format, pImage)
    DrawablePtr         pDraw;
    GCPtr               pGC;
    int                 depth, dstx, dsty, w, h, srcOffset;
    unsigned int        format;
    unsigned char       *pImage;
{
    unsigned long planeMask = pGC->planemask;
    int i;
    unsigned int planeSize, srcStride, widthDst, garbageBits, bits;
    CARD8 *psrc, *psrcLine, *psrcBase, *pdstBase, *pdstLine, *pdst;
    RegionPtr pRegion;
    unsigned int alu;
    register BoxPtr pBox;
    unsigned int numBoxes;
    CARD8 srcByte;
    unsigned int rows;
    unsigned int pixels;
    
    planeMask &= getPlanesMask(pDraw->pScreen);

    if((format == ZPixmap) || ((pDraw->type == DRAWABLE_PIXMAP) &&
	(((PixmapPtr)(pDraw))->devKind == PIXMAP_HOST_MEMORY))) {
      /*
       * mi seems to work for ZPixmaps
       * and my code doesn't work for main-memory pixmaps since
       * I rely on the hardware plane-enable
       */
      miPutImage(pDraw, pGC, depth, dstx, dsty, w, h, srcOffset, 
		 format, pImage);
      return;
    }

    alu = pGC->alu;
    pRegion = ((cfbPrivGC *)(pGC->devPriv))->pCompositeClip;
    pBox = pRegion->rects;
    numBoxes = pRegion->numRects;

    if(pDraw->type == DRAWABLE_WINDOW) {
      dstx += ((WindowPtr)(pDraw))->absCorner.x;
      dsty += ((WindowPtr)(pDraw))->absCorner.y;
      widthDst = (unsigned int)
                 ((cfbPrivScreenPtr)(pDraw->pScreen->devPrivate))->stride;
      pdstBase = (CARD8 *)
                (((cfbPrivScreenPtr)(pDraw->pScreen->devPrivate))->bits); 
    }
    else
    {
        widthDst = (unsigned int)
            (((cfbPrivPixmapPtr)(((PixmapPtr)pDraw)->devPrivate))->stride);
        pdstBase = (CARD8 *)
            (((cfbPrivPixmapPtr)(((PixmapPtr)pDraw)->devPrivate))->bits); 
    }

    srcStride = PixmapBytePad(w, 1);
    planeSize = h * srcStride;

    while(numBoxes--) {
      int clippedWidth = w - srcOffset;
      int clippedHeight = h;
      int startx = dstx;
      int starty = dsty;
      int dx = 0, dy = 0;

      psrcBase = (CARD8 *)pImage;

      /*
       * clip the height
       */
      if(dsty < pBox->y1) {
	dy = pBox->y1 - dsty;
	clippedHeight -= dy;
	if(clippedHeight <= 0) {
	  pBox++;
	  continue;
	}
        starty = pBox->y1;
      }
      if(starty+clippedHeight > pBox->y2) {
        clippedHeight = pBox->y2 - starty;
        if(clippedHeight <= 0) {
          pBox++;
          continue;
        }
      }
      /*
       * clip the width
       */
      if(dstx < pBox->x1) {
        dx = pBox->x1 - dstx;
        clippedWidth -= dx;
        if(clippedWidth <= 0) {
          pBox++;
          continue;
        }
        startx = pBox->x1;
      }
      if(startx+clippedWidth > pBox->x2) {
        clippedWidth = pBox->x2 - startx;
        if(clippedWidth <= 0) {
          pBox++;
          continue;
        }
      }

      if(!clippedWidth || !clippedHeight) {
	pBox++;
	continue;
      }

      /*
       * get address of the first destination pixel
       */
      pdstBase += startx + starty * widthDst;
      /*
       * get address of the first source bytes with useful bits
       */
      psrcBase += srcStride * dy + (srcOffset + dx) / 8;
      garbageBits = (srcOffset + dx) % 8;

      if(depth == 1) {
	/*
	 * XYBitmap code
	 */
	unsigned int fore = pGC->fgPixel;
	unsigned int back = pGC->bgPixel;

	rows = clippedHeight;
	pixels = clippedWidth;
	/*
	 * write enable the one plane we want to alter
	 */
        SET_REGISTERS_FOR_WRITING(pDraw->pScreen, planeMask, alu);

        psrcLine = psrcBase;
	pdstLine = pdstBase;
	while(rows--) {
	  psrc = psrcLine;
	  pdst = pdstLine;
	  srcByte = *psrc++ << garbageBits;
	  bits = 8 - garbageBits;

	  while(pixels--) {
	    if(srcByte & 0x80)  *pdst++ = fore;
	    else  *pdst++ = back;
	    if(--bits) {
	      srcByte <<= 1;
	    }
	    else {
	      bits = 8;
	      srcByte = *psrc++;
	    }
	  }
	  pixels = clippedWidth;
	  psrcLine += srcStride;
	  pdstLine += widthDst;
	}
	
      }
      else {
        for(i = 1 << (depth - 1); i > 0; i >>= 1, psrcBase += planeSize) {
          if(i & planeMask) {
	    rows = clippedHeight;
	    pixels = clippedWidth;

	    /*
	     * write enable the one plane we want to alter
	     */
            SET_REGISTERS_FOR_WRITING(pDraw->pScreen, i, alu);

            psrcLine = psrcBase;
	    pdstLine = pdstBase;
	    while(rows--) {
	      psrc = psrcLine;
	      pdst = pdstLine;
	      srcByte = *psrc++ << garbageBits;
	      bits = 8 - garbageBits;

	      while(pixels--) {
	        if(srcByte & 0x80)  *pdst++ = 0xff;
	        else  *pdst++ = 0x00;
	        if(--bits) {
	          srcByte <<= 1;
	        }
	        else {
		  bits = 8;
		  srcByte = *psrc++;
	        }
	      }
	      pixels = clippedWidth;
	      psrcLine += srcStride;
	      pdstLine += widthDst;
	    }
	  }
        }
      }
    }
    SET_REGISTERS_FOR_WRITING(pDraw->pScreen, 0xff, alu);
}

