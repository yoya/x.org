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

/* $Header:mpelpoly.c 1.2$ */
/* $ACIS:mpelpoly.c 1.2$ */
/* $Source: /ibm/acis/usr/src/X11/server/ddx/ibm/mpel/RCS/mpelpoly.c,v $ */

#ifndef lint
static char *rcsid = "$Header:mpelpoly.c 1.2$";
#endif

/* 
 * solid polygon fill
 */

#include 	"X.h"
#include        "Xmd.h"
#include        "Xproto.h"
#include        "gcstruct.h"
#include        "windowstr.h"
#include        "scrnintstr.h"
#include	"region.h"
#include        "regionstr.h"
#include        "mfb.h"

#include        "ppcutils.h"

#include        "rtutils.h"

#include	"mpelhdwr.h"
#include	"mpelfifo.h"

/***==================================================================***/

mpelFillPolygon(pDraw, pGC, shape, mode, count, pPts)
    DrawablePtr		pDraw;
    GCPtr		pGC;
    int			shape, mode;
    int			count;
    DDXPointPtr		pPts;
{
    RegionPtr		pCC;
    int			nbox;
    int			i;
    BoxRec		*pbox,bbox;
    mpelRectangle	scissor;
    DDXPointPtr		ppt;
    int			scissorOn,allIn;


    TRACE(("mpelFillPolygon( pDraw=x%x, pGC=x%x, count=%d, pPts=x%x )\n",
						pDraw, pGC, count, pPts));

    if (count == 0) return;

    if ((pDraw->type != DRAWABLE_WINDOW)||(pGC->fillStyle!=FillSolid)) {
	ErrorF("WSGO! bad call to mpelFillPolygon\n");
	miFillPolygon(pDraw, pGC, shape, mode, count, pPts);
	return;
    }

    if (mode == CoordModePrevious) {
	ppt = &pPts[1];
	for (i = 1; i<count; i++) {
	    ppt->x += (ppt-1)->x;
	    ppt->y += (ppt-1)->y;
	    ppt++;
	}
    }

    pCC = ((mfbPrivGC *)pGC->devPriv)->pCompositeClip;
    nbox= pCC->numRects;
    if (nbox==0)	return;

    bbox.x1= bbox.y1= 1024;
    bbox.x2= bbox.y2= 0;

    for (i=0;i<count;i++) {
	pPts[i].x+= ((WindowPtr)pDraw)->absCorner.x;
	pPts[i].y+= ((WindowPtr)pDraw)->absCorner.y;
	bbox.x1= MIN(bbox.x1,pPts[i].x);
	bbox.y1= MIN(bbox.y1,pPts[i].y);
	bbox.x2= MAX(bbox.x2,pPts[i].x);
	bbox.y2= MAX(bbox.y2,pPts[i].y);
	pPts[i].y= 1023-pPts[i].y;
    }

    mpelCheckCursor(bbox.x1, bbox.y1, bbox.x2-bbox.x1, bbox.y2-bbox.y1);

    MPELSetPgonEdgeColor(pGC->fgPixel);
    MPELSetPgonEdgeFlag(0);
    MPELSetPgonInteriorColor(pGC->fgPixel);
    MPELSetPgonInteriorStyle(2);
    mpelSetALU(pGC->alu);
    mpelSetPlaneMask(pGC->planemask);

    allIn= scissorOn= FALSE;

    pbox = (RegionPtr)pCC->rects;
    for (i=0;(i<nbox)&&(!allIn);i++,pbox++) {
	switch (mpelRectIntersect(pbox,&bbox)) {
	    case rgnOUT:	continue;
	    case rgnPART:	scissor.uright.x= 	pbox->x2-1;
				scissor.uright.y= 	1023-pbox->y1;
				scissor.lleft.x=  	pbox->x1;
				scissor.lleft.y=	1023-(pbox->y2-1);
				MPELSetScissor(&scissor);
				scissorOn= TRUE;
				break;
	    case rgnIN:		allIn= TRUE;
				if (scissorOn)  {
				    MPELResetScissor();
				    scissorOn= FALSE;
				}
				break;
	}
	MPELBeginPgon();
    	MPELPolyline(count,pPts);
    	MPELEndPgon();
    }
    if (scissorOn) {
	MPELResetScissor();
    }

    MPELResetPlaneMask();
    mpelRestoreCursor();
}

