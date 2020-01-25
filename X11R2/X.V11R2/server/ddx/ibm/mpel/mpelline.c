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

/* $Header:mpelline.c 1.2$ */
/* $ACIS:mpelline.c 1.2$ */
/* $Source: /ibm/acis/usr/src/X11/server/ddx/ibm/mpel/RCS/mpelline.c,v $ */

#ifndef lint
static char *rcsid = "$Header:mpelline.c 1.2$";
#endif

#include "X.h"
#include "gc.h"
#include "gcstruct.h"
#include "regionstr.h"
#include "pixmapstr.h"
#include "windowstr.h"
#include "mistruct.h"
#include "mpelfifo.h"
#include "mfb.h"
#include "ppcutils.h"
#include "rtutils.h"

/***==================================================================***/

void
mpelZeroSolidLine(dst, pGC, mode, n, pPoint)
    DrawablePtr dst;
    GCPtr pGC;
    int mode;
    int n;
    DDXPointPtr pPoint;
{
    int  i;
    DDXPointPtr ppt;
    RegionPtr pRegion;
    int nbox;
    BoxPtr pbox;
    int xorg, yorg, nptTmp;
    int minx, maxx, miny, maxy;
    mpelRectangle scissor;
    
    TRACE(("mpelZeroSolidLine(0x%x,0x%x,%d,%d,0x%x)\n",dst,pGC,mode,n,pPoint));

    pRegion = ((mfbPrivGC *)(pGC->devPriv))->pCompositeClip;
    nbox = pRegion->numRects;
    pbox = pRegion->rects;
    if( nbox == 0 )
	return;
    
    xorg = ((WindowPtr)dst)->absCorner.x;
    yorg = ((WindowPtr)dst)->absCorner.y;

    /* translate the point list */
    ppt = pPoint;
    nptTmp = n;

    minx= miny= 1024;
    maxx= maxy= 0;

    if (mode == CoordModeOrigin)
    {
	while(nptTmp--)
	{
	    ppt->x += xorg;
	    ppt->y += yorg;
	    minx = MIN(minx,ppt->x);
	    miny = MIN(miny,ppt->y);
	    maxx = MAX(maxx,ppt->x);
	    maxy = MAX(maxy,ppt->y);
	    ppt->y = 1023-(ppt->y);
	    ppt++;
	}
    }
    else
    {
	ppt->x += xorg;
	ppt->y += yorg;
	nptTmp--;
	while(nptTmp--)
	{
	    ppt++;
	    ppt->x += (ppt-1)->x;
	    ppt->y += (ppt-1)->y;
	    minx = MIN(minx,ppt->x);
	    miny = MIN(miny,ppt->y);
	    maxx = MAX(maxx,ppt->x);
	    maxy = MAX(maxy,ppt->y);
	    (ppt-1)->y = 1023 - ((ppt-1)->y);
	}
	ppt->y = 1023 - (ppt->y);
    }

    mpelCheckCursor(minx, miny, maxx-minx+1, maxy-miny+1);

    mpelSetALU(pGC->alu);
    mpelSetPlaneMask(pGC->planemask);
    MPELSetLineType(1);
    MPELSetPolylineColor(pGC->fgPixel);

    pbox = (RegionPtr)pRegion->rects;
    for (i=0;i<nbox;i++,pbox++) {
	scissor.uright.x= 	pbox->x2-1;
	scissor.uright.y= 	1023-pbox->y1;
	scissor.lleft.x=  	pbox->x1;
	scissor.lleft.y=	1023-(pbox->y2-1);
	MPELSetScissor(&scissor);
    	MPELPolyline(n,pPoint);
    }

    MPELResetScissor(&scissor);
    MPELResetPlaneMask();

    mpelRestoreCursor();
}

/*****************************************************************
 * mpelPolySegment
 *
 *    For each segment, draws a line between (x1, y1) and (x2, y2).  The
 *    lines are drawn in the order listed.
 *
 *****************************************************************/


void
mpelPolySegment(pDraw, pGC, nseg, pSegs)
    DrawablePtr pDraw;
    GCPtr 	pGC;
    int		nseg;
    xSegment	*pSegs;
{
    int i;
    RegionPtr pRegion;
    int xorg, yorg, nbox;
    BoxPtr pbox;
    int ntmp;
    DDXPointPtr ppt;
    int minx, miny, maxx, maxy;
    mpelRectangle scissor;

    TRACE(("mpelPolySegment(0x%x,0x%x,%d,0x%x)\n", pDraw, pGC, nseg, pSegs));

    pRegion = ((mfbPrivGC *)(pGC->devPriv))->pCompositeClip;
    nbox = pRegion->numRects;
    pbox = pRegion->rects;
    if( nbox == 0 )
	return;
    
    xorg = ((WindowPtr)pDraw)->absCorner.x;
    yorg = ((WindowPtr)pDraw)->absCorner.y;

    ntmp = nseg * 2;
    ppt = (DDXPointPtr)pSegs;

    minx= miny= 1024;
    maxx= maxy= 0;

    while ( ntmp-- )
    {
	ppt->x += xorg;
	ppt->y += yorg;
	minx = MIN(minx,ppt->x);
	miny = MIN(miny,ppt->y);
	maxx = MAX(maxx,ppt->x);
	maxy = MAX(maxy,ppt->y);
	ppt->y = 1023-(ppt->y);
	ppt++;
    }

    mpelCheckCursor(minx, miny, maxx-minx+1, maxy-miny+1);

    mpelSetALU(pGC->alu);
    mpelSetPlaneMask(pGC->planemask);
    MPELSetLineType(1);
    MPELSetPolylineColor(pGC->fgPixel);

    for (i=0;i<nbox;i++,pbox++) {
	scissor.uright.x= 	pbox->x2-1;
	scissor.uright.y= 	1023-pbox->y1;
	scissor.lleft.x=  	pbox->x1;
	scissor.lleft.y=	1023-(pbox->y2-1);
	MPELSetScissor(&scissor);
    	MPELMultiline( nseg*2, pSegs );
    }

    MPELResetScissor();
    MPELResetPlaneMask(0xff);

    mpelRestoreCursor();

}
