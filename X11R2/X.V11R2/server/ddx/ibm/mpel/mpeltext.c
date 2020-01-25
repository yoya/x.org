/*******************************************************************
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

************************************************************************/
/***********************************************************
		Copyright IBM Corporation 1987

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

/* $Header:mpeltext.c 1.2$ */
/* $ACIS:mpeltext.c 1.2$ */
/* $Source: /ibm/acis/usr/src/X11/server/ddx/ibm/mpel/RCS/mpeltext.c,v $ */

#ifndef lint
static char *rcsid = "$Header:mpeltext.c 1.2$";
#endif

#include	"X.h"
#include	"Xmd.h"
#include	"Xproto.h"
#include	"fontstruct.h"
#include	"dixfontstr.h"
#include	"gcstruct.h"
#include	"windowstr.h"
#include	"scrnintstr.h"
#include	"pixmapstr.h"
#include	"region.h"
#include	"regionstr.h"
#include	"mfb.h"
#include	"maskbits.h"

#include	"rtutils.h"

#include	"ppcprocs.h"

#include	"mpel.h"
#include	"mpelhdwr.h"
#include	"mpelfifo.h"
#include	"mpelfont.h"

#ifdef __HIGHC__
#define	MAX(a,b)	_max(a,b)
#define	MIN(a,b)	_min(a,b)
#else
#define	MAX(a,b)	((a)>(b)?(a):(b))
#define	MIN(a,b)	((a)<(b)?(a):(b))
#endif

/***==================================================================***/

typedef struct {
	void		(*bltfun)();
	DrawablePtr	*pDraw;
	GCPtr		*pGC;
	int		 baseline;
	char		*pglyphs;
} glyphBltData;

#define	callGlyphBlt(g,x,n,ppci) \
	 ((*(g)->bltfun)((g)->pDraw,(g)->pGC,(x),\
		(g)->baseline-((WindowPtr)(g)->pDraw)->absCorner.y,(n),\
		(ppci),(g)->pglyphs))

static int
mpelClipString(gblt,pbox,n,charinfo,brect,txt)
    glyphBltData	*gblt;
    BoxPtr		 pbox;
    int			 n;
    CharInfoPtr		*charinfo;
    xRectangle		*brect;
    mpelAnnotationText	*txt;
{
    int			x,end,start,i,xCoord;

    TRACE(("mpelClipString(...)\n"));

	/*
	 * Don't even try to deal with characters clipped horizontally,
	 * just call GlyphBlt to deal with it.
	 */
    if ((pbox->y1>(brect->y))||(pbox->y2<(brect->y+brect->height-1))) {
	xCoord= txt->point.x-((WindowPtr)gblt->pDraw)->absCorner.x;
	callGlyphBlt(gblt,xCoord,n,charinfo);
	return(FALSE);
    }

	/*
	 * Find first unclipped character in txt 
	 * (left edge of character >= pbox->x1)
	 * We know we're dealing with fixed width fonts, so at most
	 * one character is partially clipped.  Draw partially clipped
	 * character with GlyphBlt, and note new starting x in txt.
	 */
    i= 0;
    x= txt->point.x;
    while ((i<n)&&(x<pbox->x1)) {
	x+= charinfo[i]->metrics.characterWidth;
	i++;
    } 
    if (i>0) {	/* Some characters clipped out */
	xCoord=		x-charinfo[i-1]->metrics.characterWidth;
	xCoord-=	((WindowPtr)gblt->pDraw)->absCorner.x;
	callGlyphBlt(gblt,xCoord,1,&charinfo[i-1]);
	txt->point.x=	x;
	brect->width-=	(x-brect->x);
	brect->x=	x;
    }
    start= 		i;

	/*
	 * Find first clipped character in (adjusted) txt
	 * (right edge of character >= pbox->x2
	 * Draw partially clipped character (at most 1) with
	 * glyphBlt, and note the new length of txt.
	 */
    while ((i<n)&&(x+charinfo[i]->metrics.characterWidth<pbox->x2)) {
	x+= charinfo[i]->metrics.characterWidth;
	i++;
    } 
    if (i<n) {	/* Didn't reach the end of the string, last char clipped */
	brect->width= x-brect->x;
	xCoord= x-((WindowPtr)gblt->pDraw)->absCorner.x;
	callGlyphBlt(gblt,xCoord,1,&charinfo[i]);
    }
    end= i;

	/*
	 * fix up txt to contain only unclipped characters
	 */
    txt->length=	end-start;
    if (txt->length==0)	{		/* no unclipped characters */
	return(FALSE);
    }

	/*
	 * fix up string
	 */
    for (i=0;i<txt->length;i++) {
	txt->string[i]= txt->string[start+i];
    }

	/*
	 * fix up background rectangle
	 */
    brect->x=	MAX(pbox->x1,brect->x);
    brect->y=	MAX(pbox->y1,brect->y);
    brect->width= MIN((brect->x+brect->width),pbox->x2)-brect->x;
    brect->height= MIN((brect->y+brect->height),pbox->y2)-brect->y;
    return(TRUE);
}

/***==================================================================***/

int
mpelImageText8(pDraw, pGC, x, y, count, chars)
    DrawablePtr pDraw;
    GCPtr	pGC;
    int		x, y;
    int		count;
    char	*chars;
{

    glyphBltData	 gblt;
    CharInfoPtr		*charinfo;
    unsigned int	 n;
    FontPtr 		 font = pGC->font;
    ExtentInfoRec	 info;
    int 		 i,nbox;
    BoxPtr 		 pbox;
    RegionPtr 		 pRegion;
    xRectangle		 backrect,tmpBackrect;
    BoxRec 		 bbox;
    int			 allIn= FALSE;
    mpelXFont 		*xFont;
    mpelRectangle 	*brect,wholeBrect,partBrect;
    mpelAnnotationText	*txt,wholeTxt,partTxt;
    int			ht;
    char		wholeStr[80],partStr[80];

    TRACE(("mpelImageText8( pDraw=0x%x, pGC=0x%x, x=%d, y=%d, count=%d, chars=0x%x)\n", pDraw, pGC, x, y, count, chars));

	/*
	 * if not touching any planes, or mode is noop, no effect, return.
	 */
    if ((!pGC->planemask)||(pGC->alu==GXnoop))
	return;
	
	/*
	 * if priv<40, font was not realized by mpelRealizeFont,
	 * if not a window, or merge mode not copy, the hardware can't 
	 * help us.
	 * (I guess we *could* write an glyph blt that uses blt and
	 *  the character as stored on the adapter....)
	 */

    if  ((((CARD32)font->devPriv[pDraw->pScreen->myNum]) < 40 )||
	 	(pDraw->type!=DRAWABLE_WINDOW)||(pGC->alu!=GXcopy))
	{
	miImageText8(pDraw, pGC, x, y, count, chars);
        return;
	}

    while (count>80) {
	x= mpelImageText8(pDraw,pGC,x,y,80,chars);
	count-= 80;
	chars= &chars[80];
    }

    xFont= (mpelXFont *)font->devPriv[pDraw->pScreen->myNum];

    x += ((WindowPtr)pDraw)->absCorner.x;
    y += ((WindowPtr)pDraw)->absCorner.y;

    if(!(charinfo = (CharInfoPtr *)ALLOCATE_LOCAL(count*sizeof(CharInfoPtr))))
	return;

    GetGlyphs(font, count, chars, Linear8Bit, &n, charinfo);

    QueryGlyphExtents(font, charinfo, n, &info);

    backrect.x=		x+info.overallLeft;
    backrect.y=		y-font->pFI->fontAscent;
    backrect.width=	info.overallRight-info.overallLeft;
    backrect.width=	max( backrect.width, info.overallWidth );
    backrect.height=	font->pFI->fontAscent+font->pFI->fontDescent;

    bbox.x1 = x + info.overallLeft;
    bbox.x2 = x + info.overallRight;
    bbox.y1 = y - info.overallAscent;
    bbox.y2 = y + info.overallDescent;

    pRegion = ((mfbPrivGC *)(pGC->devPriv))->pCompositeClip;
    nbox = pRegion->numRects;
    pbox = pRegion->rects;
    if ( nbox == 0 )
	return;

    mpelCheckCursor(bbox.x1, bbox.y1, bbox.x2-bbox.x1+1, bbox.y2-bbox.y1+1);
    mpelCheckCursor(backrect.x,backrect.y,backrect.width,backrect.height);

    MPELSetPgonInteriorColor(pGC->bgPixel);
    MPELSetTextColor(pGC->fgPixel);
    MPELSetTextFont(xFont->mpelId);
    mpelSetPlaneMask(pGC->planemask);

    mpelRemap(xFont,n,chars,wholeStr,charinfo,pGC->font->pGlyphs);

    wholeBrect.uright.x=	backrect.x+backrect.width-1;
    wholeBrect.uright.y=	1023-backrect.y;
    wholeBrect.lleft.x=		backrect.x;
    wholeBrect.lleft.y=		1023-(backrect.y+backrect.height-1);

    ht= (((fontHeight(xFont)+3)/4)*4)-fontHeight(xFont);
    wholeTxt.point.x=	x;
    wholeTxt.point.y=	(1023-y)-font->pFI->fontDescent-ht+1;
    wholeTxt.reserved=	0;
    wholeTxt.length=	n;
    wholeTxt.string=	wholeStr;

    setFontDimensions(xFont);

    for (i=0;(i<nbox)&&(!allIn);i++,pbox++) {
	switch (mpelRectIntersect(pbox,&bbox)) {
	    case rgnOUT:
		continue;
	    case rgnPART:
		bcopy(&backrect,&tmpBackrect,sizeof(xRectangle));
		bcopy(&wholeTxt,&partTxt,sizeof(mpelAnnotationText));
		bcopy(wholeStr, partStr, count);
		partTxt.string= partStr;
		gblt.bltfun=	ppcImageGlyphBlt;
		gblt.pDraw=	pDraw;
		gblt.pGC=	pGC;
		gblt.baseline=	y;
		gblt.pglyphs=	pGC->font->pGlyphs;
		if(!mpelClipString(&gblt,pbox,n,charinfo,&tmpBackrect,&partTxt))
		{
		    continue;
		}
		else {
		    partBrect.uright.x=tmpBackrect.x+tmpBackrect.width-1;
		    partBrect.uright.y=1023-tmpBackrect.y;
		    partBrect.lleft.x= tmpBackrect.x;
		    partBrect.lleft.y=1023-(tmpBackrect.y+tmpBackrect.height-1);
		    brect=	&partBrect;
		    txt=	&partTxt;
		}
		break;
	    case rgnIN:
		allIn= 	TRUE;
		brect=	&wholeBrect;
		txt=	&wholeTxt;
		break;
	}
	mpelSetALU(GXcopy);
	MPELFillRectangle(brect);
	mpelSetALU(pGC->alu);
	MPELAnnotationText(txt->length,txt);
	MPELSendData(txt->length,txt->string);
    }

    DEALLOCATE_LOCAL(partTxt);
    DEALLOCATE_LOCAL(wholeTxt);
    DEALLOCATE_LOCAL(charinfo);
    MPELResetPlaneMask();
    mpelRestoreCursor();
    return bbox.x2-((WindowPtr)pDraw)->absCorner.x;
}

/***==================================================================***/

int
mpelPolyText8(pDraw, pGC, x, y, count, chars)
    DrawablePtr pDraw;
    GCPtr	pGC;
    int		x, y;
    int		count;
    char	*chars;
{

    glyphBltData	 gblt;
    CharInfoPtr		*charinfo;
    unsigned int	 n;
    FontPtr 		 font = pGC->font;
    ExtentInfoRec	 info;
    int 		 i,nbox;
    BoxPtr 		 pbox;
    RegionPtr 		 pRegion;
    BoxRec 		 bbox;
    xRectangle		 bRect;
    int			 allIn= FALSE;
    mpelXFont 		*xFont;
    mpelAnnotationText	*txt,wholeTxt,partTxt;
    int			ht;
    char		wholeStr[80],partStr[80];

    TRACE(("mpelPolyText8( pDraw=0x%x, pGC=0x%x, x=%d, y=%d, count=%d, chars=0x%x)\n", pDraw, pGC, x, y, count, chars));

	/*
	 * if not touching any planes, or mode is noop, no effect, return.
	 */
    if ((!pGC->planemask)||(pGC->alu==GXnoop))
	return;
	
	/*
	 * if priv<40, font was not realized by mpelRealizeFont,
	 * if not a window, or merge mode not copy, the hardware can't 
	 * help us.
	 * (I guess we *could* write an glyph blt that uses blt and
	 *  the character as stored on the adapter....)
	 */

    if  ((((CARD32)font->devPriv[pDraw->pScreen->myNum]) < 40 )||
	 	(pDraw->type!=DRAWABLE_WINDOW)||(pGC->alu!=GXcopy))
	{
	miPolyText8(pDraw, pGC, x, y, count, chars);
        return;
	}

    while (count>80) {
	x= mpelPolyText8(pDraw,pGC,x,y,80,chars);
	count-= 80;
	chars= &chars[80];
    }

    xFont= (mpelXFont *)font->devPriv[pDraw->pScreen->myNum];

    x += ((WindowPtr)pDraw)->absCorner.x;
    y += ((WindowPtr)pDraw)->absCorner.y;

    if(!(charinfo = (CharInfoPtr *)ALLOCATE_LOCAL(count*sizeof(CharInfoPtr))))
	return;

    GetGlyphs(font, count, chars, Linear8Bit, &n, charinfo);

    QueryGlyphExtents(font, charinfo, n, &info);

    bbox.x1 = x + info.overallLeft;
    bbox.x2 = x + info.overallRight;
    bbox.y1 = y - info.overallAscent;
    bbox.y2 = y + info.overallDescent;


    pRegion = ((mfbPrivGC *)(pGC->devPriv))->pCompositeClip;
    nbox = pRegion->numRects;
    pbox = pRegion->rects;
    if ( nbox == 0 )
	return;

    mpelCheckCursor(bbox.x1, bbox.y1, bbox.x2-bbox.x1+1, bbox.y2-bbox.y1+1);

    MPELSetTextColor(pGC->fgPixel);
    MPELSetTextFont(xFont->mpelId);
    mpelSetALU(pGC->alu);
    mpelSetPlaneMask(pGC->planemask);

    mpelRemap(xFont,n,chars,wholeStr,charinfo,pGC->font->pGlyphs);

    ht= (((fontHeight(xFont)+3)/4)*4)-fontHeight(xFont);
    wholeTxt.point.x=	x;
    wholeTxt.point.y=	(1023-y)-font->pFI->fontDescent-ht+1;
    wholeTxt.reserved=	0;
    wholeTxt.length=	n;
    wholeTxt.string=	wholeStr;

    setFontDimensions(xFont);

    for (i=0;(i<nbox)&&(!allIn);i++,pbox++) {
	switch (mpelRectIntersect(pbox,&bbox)) {
	    case rgnOUT:
		continue;
	    case rgnPART:
		bRect.x= bbox.x1; bRect.y= bbox.y1;
		bRect.width= bbox.x2-bbox.x1-1;
		bRect.height= bbox.y2-bbox.y1-1;
		bcopy(&wholeTxt,&partTxt,sizeof(mpelAnnotationText));
		bcopy(wholeStr,partTxt.string= partStr,n);
		gblt.bltfun=	ppcPolyGlyphBlt;
		gblt.pDraw=	pDraw;
		gblt.pGC=	pGC;
		gblt.baseline=	y;
		gblt.pglyphs=	pGC->font->pGlyphs;
		if (!mpelClipString(&gblt,pbox,n,charinfo,&bRect,&partTxt))
		    continue;
		else 
		    txt= &partTxt;
		break;
	    case rgnIN:
		allIn= 	TRUE;
		txt=	&wholeTxt;
		break;
	}
	MPELAnnotationText(txt->length,txt);
	MPELSendData(txt->length,txt->string);
    }
    DEALLOCATE_LOCAL(partTxt);
    DEALLOCATE_LOCAL(wholeTxt);
    DEALLOCATE_LOCAL(charinfo);
    MPELResetPlaneMask();
    mpelRestoreCursor();
    return bbox.x2-((WindowPtr)pDraw)->absCorner.x;
}
