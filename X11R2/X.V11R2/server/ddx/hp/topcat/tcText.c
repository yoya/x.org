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
 *  file: topcatText.c
 *
 *
 *  ******************************************************************
 *  *  (c) Copyright Hewlett-Packard Company, 1987.  All rights are  *
 *  *  reserved.  Copying or other reproduction of this program      *
 *  *  except for archival purposes is prohibited without prior      *
 *  *  written consent of Hewlett-Packard Company.		     *
 *  ******************************************************************
 *
 *  ImageText and PolyText routines for the topcat displays
 *
 *		Hewlett Packard -- Corvallis Workstation Operation
 *		Project -- port of X11 to HP9000
 *		Harry Phinney -- MTS
 *
 *
 */

#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/graphics.h>

#include "X.h"
#include "Xproto.h"
#include "scrnintstr.h"
#include "cursorstr.h"
#include "pixmapstr.h"
#include "regionstr.h"
#include "dixfontstr.h"
#include "fontstruct.h"
#include "gcstruct.h"
#include "windowstr.h"

#include "../cfb/cfb.h"
#include "mi.h"
#include "topcat.h"
#include "../cfb/hpbuf.h"
#include "hpFonts.h"

extern unsigned char *FrameBufferBase();
extern u_char XHP_NewRule[16][6]; /* array of replacement rules */


/************************************************************************
 *  Routine:    tcImageOptText
 *              Render text strings to the display with "optimized" fonts.
 *              Uses the Topcat block mover to place the character if the
 *              character has been "optimized" (stored in offscreen memory).
 *              If it's not in offscreen then we call pGC->ImageGlyphBlt.
 *              It is assumed that these are *terminal fonts*, drawn left
 *              to right, and have the most-used characters stored in
 *              offscreen memory. The characters must be stored in increasing
 *		order in the chunks, e.g. char 78 *must* be further down the
 *		list of chunks than char 57, though char 63 doesn't have to 
 *		be stored in offscreen. No chunk can have holes - the chars
 *		must be contiguous within a chunk. In the example above, if
 *		char 63 isn't stored, 57 and 78 *must* be in different chunks.
 *
 *		If pGC->bgPixel is -1, then assume we are being called by
 *		PolyText, and make the glyph background a no-op (transparent).
 *		This allows PolyText to be fast iff FillStyle == Solid.
 *
 *  Inputs: pDraw points to the drawable we're to print to
 *          pGC points to the GC we're to use
 *          x, y is the starting location for the string in the drawable
 *          count is the number of characters we're to put out
 *          chars points to an array of the characters we're to put out(!)
 *          encoding is Linear8Bit, Linear16Bit, etc.
 *
 *  Returns: nothing
 *
 *  Side Effects: none
 *
 */
void
tcImageOptText(pDraw, pGC, dstx, dsty, count, chars /*, encoding */)
    DrawablePtr pDraw;
    GCPtr       pGC;
    int         dstx, dsty;
    int         count;
    u_char      *chars;
    /* FontEncoding encoding; */
{
    FontEncoding encoding = Linear8Bit; /* should be passed in ... */
    register TOPCAT *hardware = getGpHardware(pDraw->pScreen);
    int XHP_bits = hardware->bits;
    unsigned long screenPlanes = getPlanesMask(pDraw->pScreen);
    unsigned long zmask = pGC->planemask & screenPlanes;
    int fore = pGC->fgPixel;
    int back = pGC->bgPixel;

    FontPtr pfont = pGC->font;
    FontInfoPtr pFI = pfont->pFI;
    CharInfoPtr pCI = pfont->pCI;
    unsigned int firstCol = pFI->firstCol;
    unsigned int firstRow = pFI->firstRow;
    unsigned int chDefault = pFI->chDefault;

    hpFontRec *pfrec = (hpFontRec *)(pfont->devPriv[pDraw->pScreen->myNum]);
    hpChunk *pChunk;
    int numChunks = pfrec->NumChunks;
    hpCharRange *pRange;

    RegionPtr pRegion = (RegionPtr)((cfbPrivGC *)pGC->devPriv)->pCompositeClip;
    register BoxPtr pBox = pRegion->rects;
    int nbox = pRegion->numRects;

    BoxRec cursorBox;

    register int i, x, y, dx, dy, h, w;
    int srcx, srcy;
    int found = 0; /* flag for in/out of offscreen */

   /*
    * some stupid applications send a request to output a zero length
    * string, so I have to check for it.
    */
   if(!count) return;

   /*
    * absolutize the coordinates 
    */
    dstx += ((WindowPtr)pDraw)->absCorner.x;
    dsty += ((WindowPtr)pDraw)->absCorner.y;

    /*
     * adjust dsty from char baseline to top edge
     * because we assume terminal fonts, all ascents are the same
     */
    dsty -= pFI->maxbounds.metrics.ascent;

    /*
     * Check to see if the cursor overlaps the rectangle we're trying
     * to write into. If it does, then turn it off.
     */
    if(hpCursorLoc(pDraw->pScreen, &cursorBox)) {
      w = GLWIDTHPIXELS(pCI + *chars);
      h = GLHEIGHTPIXELS(pCI + *chars);
      if((cursorBox.x1 <= (dstx+(w*count))) && (dstx <= cursorBox.x2) &&
	  (cursorBox.y1 <= (dsty+h)) && (dsty <= cursorBox.y2)) {
	hpCursorOff(pDraw->pScreen);
      }
    }

    waitbusy(screenPlanes, hardware);
    if(back == -1) {
      /*
       * handle special case of PolyText, so PolyText can be fast if
       * it uses a solid fill
       */
      hardware->write_enable = zmask & fore;
      hardware->window_move_replacement_rule = XHP_NewRule[pGC->alu][4];
      hardware->write_enable = zmask & ~fore;
      hardware->window_move_replacement_rule = XHP_NewRule[pGC->alu][5];
    }
    else {
      /*
       * Set registers for normal ImageText case.  Note that pGC->alu
       * is ignored, and GXcopy is forced.
       */
      hardware->write_enable = zmask & ~fore & ~back;
      hardware->window_move_replacement_rule = XHP_NewRule[GXcopy] [0];
      hardware->write_enable = zmask & ~fore & back;
      hardware->window_move_replacement_rule = XHP_NewRule[GXcopy] [1];
      hardware->write_enable = zmask & fore & ~back;
      hardware->window_move_replacement_rule = XHP_NewRule[GXcopy] [2];
      hardware->write_enable = zmask & fore & back;
      hardware->window_move_replacement_rule = XHP_NewRule[GXcopy] [3];
    }

    hardware->write_enable = zmask;
    hardware->pixel_write_replacement_rule = GXcopy;


    switch(encoding) {
      case Linear8Bit: 
      case TwoD8Bit: {

	while(nbox--) { /* for each clip box */

	  /*
	   * because we assume terminal fonts, we just need the w & h 
	   * of any random character from the font (since their all the same)
	   */
          w = GLWIDTHPIXELS(pCI + *chars);
	  h = GLHEIGHTPIXELS(pCI + *chars);
	  y = dsty;
	  dy = 0;

	  /*
	   * clip the height only once since all chars are same height&ascent
	   */
	  if (y < pBox->y1) {
	    dy = pBox->y1 - y;
	    h -= dy;
	    y = pBox->y1;
	  }
	  if(y+h > pBox->y2) {
	    h -= y + h - pBox->y2;
	  }

	  /* 
	   * if any part is in the y-band of the box, write it
	   */
	  if(h > 0) {
	    register slen = count;
	    register u_char *str = chars;
	    register unsigned char st = *str++ - firstCol;
	    x = dstx;

	    /*
	     * while the character is to the left of the clipping
	     * rectangle, go to the next character
	     */
	    while(x+w <= pBox->x1) {
	      if (--slen == 0) break;
	      x += w;
	      st = *str++ - firstCol;
	    }

	    /*
	     * clip the first "in" character to the edge
	     */
	    if((dx = pBox->x1 - x) < 0) dx = 0;
	    if(x+dx >= pBox->x2) slen = 0;

	    /*
	     * in the clip box, put out each character
	     */
	    while(slen--) { /* for each char left in the string */

	      if(!pCI[st].exists) {
		/*
		 * check for non-existent glyphs & replace w/the default
		 */
		st = chDefault;
	      }

	      /*
	       * find the char's location in offscreen (hopefully)
	       */
	      found = 0; /* flag for in/out of offscreen */

	      for(i = 0, pRange = pfrec->pRange; i < numChunks; i++, pRange++) {
		if(st <= pRange->endChar) {
		  if(st >= pRange->startChar) {
		    found = 1; /* we found it */
		    pChunk = pfrec->ppChunk[i]; 
		    srcx = pChunk->x + w * (st - pRange->startChar);
		    srcy = pChunk->y;
		    break;
		  }
		  else {
		    /*
		     * it's not in any of the chunks
		     */
		    break;
		  }
		}
	      }

	      if(!found) {
		/*
		 * the char wasn't in any chunk, so call ImageGlyphBlt
		 */
		int oldTranslate = pGC->miTranslate;
		pGC->miTranslate = 0; /* we already translated it */
		if(back == -1) {
		  (void)(*pGC->PolyGlyphBlt)(pDraw, pGC, x, 
				        dsty + pFI->maxbounds.metrics.ascent,
				        1, &pCI, pfont->pGlyphs);
      		  hardware->write_enable = zmask & fore;
      		  hardware->window_move_replacement_rule = 
						XHP_NewRule[pGC->alu][4];
      		  hardware->write_enable = zmask & ~fore;
      		  hardware->window_move_replacement_rule =
						XHP_NewRule[pGC->alu][5];
		}
		else {
		  (*pGC->ImageGlyphBlt)(pDraw, pGC, x, 
				        dsty + pFI->maxbounds.metrics.ascent,
				        1, &pCI, pfont->pGlyphs);
		  hardware->write_enable = zmask & ~fore & ~back;
		  hardware->window_move_replacement_rule = 
						XHP_NewRule[GXcopy] [0];
		  hardware->write_enable = zmask & ~fore & back;
		  hardware->window_move_replacement_rule = 
						XHP_NewRule[GXcopy] [1];
		  hardware->write_enable = zmask & fore & ~back;
		  hardware->window_move_replacement_rule = 
						XHP_NewRule[GXcopy] [2];
		  hardware->write_enable = zmask & fore & back;
		  hardware->window_move_replacement_rule = 
						XHP_NewRule[GXcopy] [3];
		}
    		hardware->write_enable = zmask;
    		hardware->pixel_write_replacement_rule = GXcopy;

	      }
	      else {
		/*
		 * use the block mover to place the glyph
		 */

		if(x+w > pBox->x2) {
                  /* clip to right edge */
		  w = pBox->x2 - x;
		}

		waitbusy(screenPlanes, hardware);
		hardware->source_x = (srcx + dx) << XHP_bits;
		hardware->source_y = srcy + dy;
		hardware->dest_x = (x + dx) << XHP_bits;
		hardware->dest_y = y;
		hardware->window_width = (w - dx) << XHP_bits;
		hardware->window_height = h;
		hardware->start_move = zmask;
	      }
	      x += w; /* move to start pos of next char on screen */
	      dx = 0;
	      if(x >= pBox->x2) slen = 0; /* no more within pBox */
	      st = *str++ - firstCol;
	    }
	  }
	  pBox++;
	}
      
	
      }
      case Linear16Bit: {
      }
      case TwoD16Bit: {
      }
    }
}

#include "salloc.h"

/************************************************************************
 *  Routine:    tcImageVarText
 *              Render text strings to the display with "optimized" fonts
 *		in the case where the font is variable-width, and varible
 *		ascent, etc.
 *              Uses the Topcat block mover to place the character if the
 *              character has been "optimized" (stored in offscreen memory).
 *              If it's not in offscreen then we call pGC->ImageGlyphBlt.
 *              It is assumed that these characters are drawn left
 *              to right, and have the most-used characters stored in
 *              offscreen memory. The characters must be stored in increasing
 *		order in the chunks, e.g. char 78 *must* be further down the
 *		list of chunks than char 57, though char 63 doesn't have to 
 *		be stored in offscreen. No chunk can have holes - the chars
 *		must be contiguous within a chunk. In the example above, if
 *		char 63 isn't stored, 57 and 78 *must* be in different chunks.
 *
 *		If pGC->bgPixel is -1, then assume we are being called by
 *		PolyText, and make the glyph background a no-op (transparent).
 *		This allows PolyText to be fast iff FillStyle == Solid.
 *
 *  Inputs: pDraw points to the drawable we're to print to
 *          pGC points to the GC we're to use
 *          x, y is the starting location for the string in the drawable
 *          count is the number of characters we're to put out
 *          chars points to an array of the characters we're to put out
 *          encoding is Linear8Bit, Linear16Bit, etc.
 *
 *  Returns: nothing
 *
 *  Side Effects: none
 *
 */
void
tcImageVarText(pDraw, pGC, dstx, dsty, count, chars /*, encoding */)
    DrawablePtr pDraw;
    GCPtr       pGC;
    int         dstx, dsty;
    int         count;
    u_char      *chars;
    /* FontEncoding encoding; */
{
    FontEncoding encoding = Linear8Bit; /* should be passed in ... */
    register TOPCAT *hardware = getGpHardware(pDraw->pScreen);
    int XHP_bits = hardware->bits;
    unsigned long screenPlanes = getPlanesMask(pDraw->pScreen);
    unsigned long zmask = pGC->planemask & screenPlanes;
    int fore = pGC->fgPixel;
    int back = pGC->bgPixel;

    FontPtr pfont = pGC->font;
    FontInfoPtr pFI = pfont->pFI;
    CharInfoPtr pMaxBounds = &pFI->maxbounds;
    CharInfoPtr pMinBounds = &pFI->minbounds;
    unsigned int firstCol = pFI->firstCol;
    unsigned int firstRow = pFI->firstRow;
    unsigned int chDefault = pFI->chDefault;

    unsigned int maxWidth = pMaxBounds->metrics.rightSideBearing -
			    pMinBounds->metrics.leftSideBearing;
    unsigned int maxHeight = pMaxBounds->metrics.ascent + 
			     pMaxBounds->metrics.descent;
    int miny, maxy, minx, maxx;

    CharInfoPtr pCI = pfont->pCI;
    CharInfoPtr *ppCI;
    ExtentInfoRec info;

    hpFontRec *pfrec = (hpFontRec *)(pfont->devPriv[pDraw->pScreen->myNum]);
    hpChunk *pChunk;
    int numChunks = pfrec->NumChunks;
    hpCharRange *pRange;

    RegionPtr pRegion = (RegionPtr)((cfbPrivGC *)pGC->devPriv)->pCompositeClip;
    register BoxPtr pBox = pRegion->rects;
    int nbox = pRegion->numRects;

    BoxRec cursorBox;

    register int i, x, y, dx, dy, h, w, oldAlu, oldFS;
    CARD32 oldFG;
    long gcvals[3];
    xRectangle backrect;
    int srcx, srcy;
    int found = 0; /* flag for in/out of offscreen */


   if(!count) return;

    if(back != -1) {
      int n;
      /*
       * Fill the background rectangle to the background color
       */

      /*
       * build the array of CharInfo struct pointers for glyphs in the string
       */
#if SAOK
      SALLOC(count*sizeof(CharInfoPtr)); ppCI = (CharInfoPtr *)SADDR;
#else
      if(!(ppCI = (CharInfoPtr *)ALLOCATE_LOCAL(count*sizeof(CharInfoPtr))))
	return;
#endif
      GetGlyphs(pfont, count, chars, Linear8Bit, &n, ppCI);
      QueryGlyphExtents(pGC->font, ppCI, n, &info);
#if !SAOK
      DEALLOCATE_LOCAL(ppCI);
#endif
      backrect.x = dstx;
      backrect.y = dsty - pGC->font->pFI->fontAscent;
      backrect.width = info.overallWidth;
      backrect.height = pGC->font->pFI->fontAscent +
			pGC->font->pFI->fontDescent;

      oldAlu = pGC->alu;
      oldFG = pGC->fgPixel;
      oldFS = pGC->fillStyle;
      /* fill in the background */
      pGC->alu = (long) GXcopy;
      pGC->fgPixel = (long) pGC->bgPixel;
      pGC->fillStyle = (long) FillSolid;
      tcPolyFillSolidRect(pDraw, pGC, 1, &backrect);
      /*
       * put the GC back except for alu
       * in an ImageText, the effective alu is GXcopy
       */
      pGC->fgPixel = oldFG;
      pGC->fillStyle = oldFS;
    }

   /*
    * absolutize the coordinates 
    */
    dstx += ((WindowPtr)pDraw)->absCorner.x;
    dsty += ((WindowPtr)pDraw)->absCorner.y;

    miny = dsty - pMaxBounds->metrics.ascent;
    maxy = dsty + pMaxBounds->metrics.descent;
    minx = dstx + pMinBounds->metrics.leftSideBearing; /* is this right? */
    maxx = dstx + count * maxWidth;

    /*
     * Check to see if the cursor overlaps the rectangle we're trying
     * to write into. If it does, then turn it off.
     */
    if(hpCursorLoc(pDraw->pScreen, &cursorBox)) {
      if((cursorBox.x1 <= maxx) && (minx <= cursorBox.x2) &&
	 (cursorBox.y1 <= maxy) && (miny <= cursorBox.y2)) {
	hpCursorOff(pDraw->pScreen);
      }
    }

    waitbusy(screenPlanes, hardware);
    /*
     * set a PolyText type of replacement rule.
     * we've cleared the background rectangle for the string in the case
     * of an Imagetext call.
     */
    hardware->write_enable = zmask & fore;
    hardware->window_move_replacement_rule = XHP_NewRule[pGC->alu][4];
    hardware->write_enable = zmask & ~fore;
    hardware->window_move_replacement_rule = XHP_NewRule[pGC->alu][5];

    hardware->write_enable = zmask;
    hardware->pixel_write_replacement_rule = GXcopy;


    switch(encoding) {
      case Linear8Bit: 
      case TwoD8Bit: {

	while(nbox--) { /* for each clip box */

	  /*
	   * check to see if any characters may be in the clip box
	   * if any part may be in the box, write it
	   */
          if((pBox->x1 <= maxx) && (minx <= pBox->x2) &&
	     (pBox->y1 <= maxy) && (miny <= pBox->y2)) {

	    register int slen = count;
	    register u_char *str = chars;
	    register unsigned char st = *str++ - firstCol;
	    CharInfoPtr pci = pCI + st;

	    x = dstx;

	    if(!pci->exists) {
	      pci = pCI + chDefault;
	      st = chDefault;
	    }

	    /*
	     * while the character is to the left of the clipping
	     * rectangle, go to the next character
	     * XXX this can blow if chDefault doesn't exist
	     */
	    while((x+(w = GLYPHWIDTHPIXELS(pci)) +
		   pci->metrics.leftSideBearing) <= pBox->x1) {
	      if (--slen == 0) break;
	      x += w;
	      st = *str++ - firstCol;
	      pci = pCI + st;
	      if(!pci->exists) {
	        pci = pCI + chDefault;
	        st = chDefault;
	      }
	    }

	    /*
	     * clip the first "in" character to the edge
	     */
	    if((dx = pBox->x1 - (x+pci->metrics.leftSideBearing)) < 0) dx = 0;
	    if((x+dx+pci->metrics.leftSideBearing) > pBox->x2) slen = 0;

	    /*
	     * in the clip box, put out each character
	     */
	    while(slen--) { /* for each char left in the string */

	      pci = &pCI[st];
	      if(!pci->exists) {
		/*
		 * check for non-existent glyphs & replace w/the default
		 */
		st = chDefault;
	        if(!pci->exists) {
		  /*
		   * if the default doesn't exist, skip the char
		   */
	          dx = 0;
	          st = *str++ - firstCol;
		  continue;
		}
	      }

	      /*
	       * find the char's location in offscreen (hopefully)
	       */
	      found = 0; /* flag for in/out of offscreen */

	      for(i = 0, pRange = pfrec->pRange; i < numChunks; i++, pRange++) {
		if(st <= pRange->endChar) {
		  if(st >= pRange->startChar) {
		    found = 1; /* we found it */
		    pChunk = pfrec->ppChunk[i]; 
		    srcx = pChunk->x + maxWidth * (st - pRange->startChar);
		    srcy = pChunk->y;
		    break;
		  }
		  else {
		    /*
		     * it's not in any of the chunks
		     */
		    break;
		  }
		}
	      }

	      if(!found) {
		/*
		 * the char wasn't in any chunk, so call ImageGlyphBlt
		 */
		int oldTranslate = pGC->miTranslate;
		pGC->miTranslate = 0; /* we already translated it */
		if(back == -1) {
		  (void)(*pGC->PolyGlyphBlt)(pDraw, pGC, x, dsty,
				        1, &pci, pfont->pGlyphs);
		}
		else {
		  (*pGC->ImageGlyphBlt)(pDraw, pGC, x, dsty,
				        1, &pci, pfont->pGlyphs);
		}
      		hardware->write_enable = zmask & fore;
      		hardware->window_move_replacement_rule = 
						XHP_NewRule[pGC->alu][4];
      		hardware->write_enable = zmask & ~fore;
      		hardware->window_move_replacement_rule =
						XHP_NewRule[pGC->alu][5];
    		hardware->write_enable = zmask;
    		hardware->pixel_write_replacement_rule = GXcopy;

		pGC->miTranslate = oldTranslate; /* put it back */
	      }
	      else {
		/*
		 * use the block mover to place the glyph
		 */

		w = GLWIDTHPIXELS(pci);
		if(x+w+pci->metrics.leftSideBearing > pBox->x2) {
                  /* clip to right edge */
		  w = pBox->x2 - (x + pci->metrics.leftSideBearing);
		}

		/*
		 * clip the height of the glyph
		 */
		y = dsty - pci->metrics.ascent;
		dy = 0;
		h = GLHEIGHTPIXELS(pci);
		if(y < pBox->y1) {
		  dy = pBox->y1 - y;
		  h -= dy;
		  y = pBox->y1;
		}
		if(y+h > pBox->y2) {
		  h -= y + h -pBox->y2;
		}

		if(h > 0) {
		  waitbusy(screenPlanes, hardware);
		  hardware->source_x = (srcx + dx) << XHP_bits;
		  hardware->source_y = srcy + dy;
		  hardware->dest_x = (x + dx + 
				      pci->metrics.leftSideBearing) << XHP_bits;
		  hardware->dest_y = y;
		  hardware->window_width = (w - dx) << XHP_bits;
		  hardware->window_height = h;
		  hardware->start_move = zmask;
		}
	      }
	      /* move to start pos of next char on screen */
	      x += pci->metrics.characterWidth;
	      dx = 0;
	      if(x >= pBox->x2) slen = 0; /* no more within pBox */
	      st = *str++ - firstCol;
	    }
	  }
	  pBox++;
	}
      }
      case Linear16Bit: {
      }
      case TwoD16Bit: {
      }
    }
    if(back != -1) {
      /* put the alu back right */ 
      pGC->alu = oldAlu; 
    }
}

int
tcPolyOptText(pDraw, pGC, dstx, dsty, count, chars /*, encoding */)
    DrawablePtr pDraw;
    GCPtr       pGC;
    int         dstx, dsty;
    int         count;
    u_char      *chars;
    /* FontEncoding encoding; */
{
    if((pGC->alu == GXcopy) && (pGC->fillStyle == FillSolid)) {
      /*
       * it looks enough like an imagetext that we can use tcImage???Text
       */
      register FontPtr pFont = pGC->font;
      int oldBackground;
      CharInfoPtr pci = pFont->pCI;
      unsigned char *thischar = chars;

      /*
       * set background to -1 to tell tcImage???Text that the background
       * should be transparent (no-op)
       */
      oldBackground = pGC->bgPixel;
      pGC->bgPixel = -1;
      (* pGC->ImageText8)(pDraw, pGC, dstx, dsty, count, chars);
      pGC->bgPixel = oldBackground;

      while(count--) {
	dstx += pci[*thischar].metrics.characterWidth;
	thischar++;
      }
      return dstx;
    }
    else {
      FontPtr pfont = pGC->font;
      FontInfoPtr pFI = pfont->pFI;
      CharInfoPtr pMaxBounds = &pFI->maxbounds;
      CharInfoPtr pMinBounds = &pFI->minbounds;
      int x = dstx + ((WindowPtr)pDraw)->absCorner.x;
      int y = dsty + ((WindowPtr)pDraw)->absCorner.y;

      unsigned int maxWidth = pMaxBounds->metrics.rightSideBearing -
			      pMinBounds->metrics.leftSideBearing;
      int miny = y - pMaxBounds->metrics.ascent;
      int maxy = y + pMaxBounds->metrics.descent;
      int minx = x + pMinBounds->metrics.leftSideBearing; /* is this right? */
      int maxx = x + count * maxWidth;
      BoxRec cursorBox;

      /*
       * Check to see if the cursor overlaps the rectangle we're trying
       * to write into. If it does, then turn it off.
       */
      if(hpCursorLoc(pDraw->pScreen, &cursorBox)) {
        if((cursorBox.x1 <= maxx) && (minx <= cursorBox.x2) &&
	   (cursorBox.y1 <= maxy) && (miny <= cursorBox.y2)) {
	  hpCursorOff(pDraw->pScreen);
        }
      }

      return miPolyText8(pDraw, pGC, dstx, dsty, count, chars);
    }
}


/************************************************************************
 *  Routine:    tcImageText8
 *              Used to output text in the case that drawable type is a window
 *              and the font glyphs are not in offscreen memory
 *		Calls tcImageGlyphBlt to actually put the glyphs on the screen
 *		
 *
 *  Inputs: pDraw points to the window we're to print to
 *          pGC points to the GC we're to use
 *          dstx, dsty is the starting location for the string in the window
 *          count is the number of characters we're to put out
 *          chars points to an array of the characters we're to put out(!)
 *
 *  Returns: nothing (it's a void)
 *
 *  Side Effects: none
 *
 */
void
tcImageText8(pDraw, pGC, dstx, dsty, count, chars)
    DrawablePtr pDraw;
    GCPtr       pGC;
    int         dstx, dsty;
    int         count;
    char        *chars;
{
    CharInfoPtr *ppCI;
    unsigned int n;
    FontPtr pFont = pGC->font;

    /*
     * build the array of CharInfo struct pointers for glyphs in the string
     */
#if SAOK
    SALLOC(count*sizeof(CharInfoPtr)); ppCI = (CharInfoPtr *)SADDR;
#else
    if(!(ppCI = (CharInfoPtr *)ALLOCATE_LOCAL(count*sizeof(CharInfoPtr))))
      return;
#endif
    GetGlyphs(pFont, count, chars, Linear8Bit, &n, ppCI);

    if(n) {
      tcImageGlyphBlt(pDraw, pGC, dstx, dsty, n, ppCI, pFont->pGlyphs);
    }
#if !SAOK
    DEALLOCATE_LOCAL(ppCI);
#endif
    return;
}
