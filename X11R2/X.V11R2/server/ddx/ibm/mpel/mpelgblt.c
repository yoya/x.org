/* Image Glyph Blt in terms of screen->rectFill and screen->glyphFill
 * from ppc
 *
 *  Tom Paquin 9/87
 *
 */

#include	"X.h"
#include	"Xmd.h"
#include	"Xproto.h"
#include	"fontstruct.h"
#include	"dixfontstr.h"
#include	"gcstruct.h"
#include	"windowstr.h"
#include	"scrnintstr.h"
#include	"pixmapstr.h"
#include	"regionstr.h"
#include	"mfb.h"
#include	"ppc.h"
#include	"maskbits.h"
#include	"rtutils.h"
#include	"ppcutils.h"

#include "os.h"
#include "mpel.h"
#include "mpelhdwr.h"
#include "mpelfifo.h"

#define MOVE _move
#define then


void
mpelImageGlyphBlt(pDrawable, pGC, x, y, nglyph, ppci, pglyphBase)
    DrawablePtr pDrawable;
    GC 		*pGC;
    int 	x, y;
    unsigned int nglyph;
    CharInfoPtr *ppci;		/* array of character info */
    unsigned char *pglyphBase;	/* start of array of glyphs */
{
    register unsigned char	*pglyph;
    ExtentInfoRec 		info;	
    BoxRec 			bbox;	
    xRectangle 			backrect;
    CharInfoPtr  		bigchar, pci;
    int 			fg, bg, alu, pm, x0,y0, ax,ay,zx,zy,zw,zh,
 		   		w, h, widthGlyph, nbox, i;
    BoxPtr 			pbox;
    short 			merge;
    RegionPtr 			pRegion;

    ScreenPtr			pScreen;
    void			(*rectFunc)(), (*glyphFunc)() ;

    if (pDrawable->type == DRAWABLE_PIXMAP)
	{
	miImageGlyphBlt(pDrawable, pGC, x, y, nglyph, ppci, pglyphBase);
	return;
	}

    pScreen = pGC->pScreen;

    rectFunc = (((ppcScrnPriv *)(pScreen->devPrivate))->solidFill);
    glyphFunc = (((ppcScrnPriv *)(pScreen->devPrivate))->glyphFill);

    QueryGlyphExtents(pGC->font, ppci, nglyph, &info);

    x += ((WindowPtr)pDrawable)->absCorner.x;
    y += ((WindowPtr)pDrawable)->absCorner.y;

    backrect.x = x + info.overallLeft;
    backrect.y = y - pGC->font->pFI->fontAscent;
    backrect.width = info.overallRight - info.overallLeft;
    backrect.width = max( backrect.width, info.overallWidth );
    backrect.height = pGC->font->pFI->fontAscent + 
		      pGC->font->pFI->fontDescent;

    bbox.x1 = x + info.overallLeft;
    bbox.x2 = x + info.overallRight;
    bbox.y1 = y - info.overallAscent;
    bbox.y2 = y + info.overallDescent;

    mpelCheckCursor( backrect.x, backrect.y, backrect.width, backrect.height );
    mpelCheckCursor( bbox.x1, bbox.y1, bbox.x2-bbox.x1, bbox.y2-bbox.y1 );

    pRegion = ((mfbPrivGC *)(pGC->devPriv))->pCompositeClip; 
    pbox = pRegion->rects;
    nbox = pRegion->numRects;
    if ( nbox == 0 )
	return;

    alu = pGC->alu;
    pm =  pGC->planemask;
    fg =  pGC->fgPixel;
    bg =  pGC->bgPixel;
    for (i=0; i<nbox; i++, pbox++) 
	{
	ax = MAX(pbox->x1, backrect.x);
	ay = MAX(pbox->y1, backrect.y);
	zx = MIN(pbox->x2, backrect.x + backrect.width);
	zy = MIN(pbox->y2, backrect.y + backrect.height);
	zw = zx - ax;
	zh = zy - ay;
	if ((zw>=0) && (zh>=0)) (*rectFunc)(bg, alu, pm, ax, ay, zw, zh);
	}

    switch ((*pScreen->RectIn)( pRegion, &bbox))
    {
      case rgnOUT:
	break;
      case rgnIN:

        while(nglyph--)
        {
	    pci = *ppci++;
	    pglyph = pglyphBase + pci->byteOffset;
	    w = GLYPHWIDTHPIXELS(pci);
	    h = GLYPHHEIGHTPIXELS(pci);
	    x0 = x + pci->metrics.leftSideBearing;
	    y0 = y - pci->metrics.ascent;
	    (*glyphFunc)(pglyph, x0, y0, w, h, fg, alu, pm);
	    x += pci->metrics.characterWidth;	/* update character origin */
        } /* while nglyph-- */
	break;
      case rgnPART:
      {
	int nbox;
	BoxPtr pbox;
	int i;
	RegionPtr prgnClip;

	BoxRec cbox;
	int glx, gly;
	    mpelRectangle	scissor;
	    short		*block;
	    int			size;

        while(nglyph--)
        {

	    pci = *ppci++;
	    pglyph = pglyphBase + pci->byteOffset;
	    w = GLYPHWIDTHPIXELS(pci);
	    h = GLYPHHEIGHTPIXELS(pci);
	    cbox.x1 = glx = x + pci->metrics.leftSideBearing;
	    cbox.y1 = gly = y - pci->metrics.ascent;
	    cbox.x2 = cbox.x1 + w;
	    cbox.y2 = cbox.y1 + h;

	    switch ((*pScreen->RectIn)( pRegion,  &cbox))

	    {
	      case rgnOUT:
		break;
	      case rgnIN:
	    	(*glyphFunc)(pglyph, glx, gly, w, h, fg, alu, pm);
		break;
	      case rgnPART:
		prgnClip = (*pScreen->RegionCreate)(&cbox,pRegion->numRects);
		(*pScreen->Intersect) (prgnClip, prgnClip, pRegion);
		pbox = prgnClip->rects;
		nbox = prgnClip->numRects;
		if ( nbox == 0 )
		    break;

		mpelSetPlaneMask(pm);

		size = ( h * ((w+7)/8));
		block = MPELAllocBLTImmedWColorExpansion( size );

		mpelStageMonoByteData(block, pglyph, glx, gly, w, h, fg, alu);

		for(i = 0; i < nbox; i++, pbox++)
			{
			scissor.uright.x= 	pbox->x2 -1;
			scissor.uright.y= 	1023-pbox->y1;
			scissor.lleft.x=  	pbox->x1;
			scissor.lleft.y=	1023-pbox->y2 +1;
			MPELSetScissorBoundaries(&scissor);

			MPELBLTImmedWColorExpansion(size, block);
			}
		DEALLOCATE_LOCAL(block);

	    	scissor.uright.x= 	1023;
	    	scissor.uright.y= 	1023;
	    	scissor.lleft.x=  	0;
	    	scissor.lleft.y=	0;
	    	MPELSetScissorBoundaries(&scissor);
		NoOp();
		break;

      	    default:
		break;
	    }

	    /* update character origin */
	    x += pci->metrics.characterWidth;
        } /* while nglyph-- */

      } /* case rgnPART */
      default:
	break;
    }/* switch rgnIN */
mpelSetPlaneMask(0xff);
mpelRestoreCursor();
}


mpelStageMonoByteData (retblock, data, x,y,w,h, fg, alu)
mpelBLTImmedWColorExpansion *retblock;
unsigned char *data;
int x, y, w, h, fg, alu;
{
	unsigned char *srcPtr, *dstPtr;
	int i,j;
	int cleanBytes, widthBytes;
	unsigned char lastbyteMask ;
	int PaddedWidthInBits;

	widthBytes = (w+7)/8;

	if (i = (w%8))
		{
		lastbyteMask = ((~0) << (8 - i));
		PaddedWidthInBits = (w & ~7) + 8;
		}
	   else {
		lastbyteMask = ~0;
		PaddedWidthInBits = w;
		}

	/* copy image TO adapter buffer area */
	srcPtr = (unsigned char *) data;

	dstPtr = (unsigned char *) retblock->string;

	cleanBytes = widthBytes-1;
	for (j=0; j<h; j++) 
		{
		if (cleanBytes)
			{
			MOVE(srcPtr, dstPtr, cleanBytes);
			srcPtr += cleanBytes;
			dstPtr += cleanBytes;
			}

		/* mask last byte */
		*dstPtr++ = *srcPtr++ & lastbyteMask; 
		}

	retblock->color		= fg;
	retblock->alu		= alu;
	retblock->dest.lleft.x	= x;
	retblock->dest.lleft.y	= 1023 - (y + h - 1);
	retblock->dest.uright.x	= x + PaddedWidthInBits - 1;
	retblock->dest.uright.y	= 1023 - y;
}
