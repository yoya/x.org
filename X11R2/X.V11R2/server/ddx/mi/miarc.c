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
/* $Header: miarc.c,v 1.49 88/02/02 13:42:01 rws Exp $ */
/* Author: Todd Newman */
#include "X.h"
#include "Xprotostr.h"
#include "misc.h"
#include "gcstruct.h"
#include "scrnintstr.h"
#include "pixmapstr.h"
#include "windowstr.h"
#include "mifpoly.h"

extern double sqrt(), cos(), sin(), atan();
/* these are from our <math.h>, but I'm told some systems don't have
 * math.h and that they're not in all versions of math.h.  Psi! */
#define M_PI	3.14159265358979323846
#define M_PI_2	1.57079632679489661923


/* This contains the information needed to draw one arc of a polyarc.
 * An array of them can be allocated if the polyarc has more than 1 part */
typedef struct
{
    int		cpt;
    DDXPointPtr	pPts;
} POLYARCINFO;

#define GCValsFunction		0
#define GCValsForeground 	1
#define GCValsBackground 	2
#define GCValsLineWidth 	3
#define GCValsCapStyle 		4
#define GCValsJoinStyle		5
#define GCValsMask		(GCFunction | GCForeground | GCBackground | \
				 GCLineWidth | GCCapStyle | GCJoinStyle)
static CARD32 gcvals[]= {GXcopy, 1, 0, 0, 0, 0};

/* MIPOLYARC -- Public entry for the polyarc call.
 * Strategy: Similar in many ways to that for wide lines.
 * In general, we will each arc segment as a polyline. (We can make the arc
 * arbitrarily smooth by increasing the number of segments and shortening
 * the length of each segment.)
 * If there's only 1 arc, or if the arc is draw with zero width lines, we 
 * don't have to worry about the rasterop or join styles.   
 * Otherwise, we set up pDrawTo and pGCTo according to the rasterop, then
 * draw using pGCTo and pDrawTo.  If the raster-op was "tricky," that is,
 * if it involves the destination, then we use PushPixels to move the bits
 * from the scratch drawable to pDraw. (See the wide line code for a
 * fuller explanation of this.)
 */
void
miPolyArc(pDraw, pGC, narcs, parcs)
    DrawablePtr	pDraw;
    GCPtr	pGC;
    int		narcs;
    xArc	*parcs;
{
    register int		i;
    register DDXPointPtr	ppt;	/* Points for the current arc */
    register DDXPointPtr	pAllPts; /* Points for all arcs so far */
    register int		cpt,	/* count of points in current arc */
    				cptAll;	/* count of points in all arcs */
    DDXPointPtr			pPts;	/* Points for the current arc */
    int				xMin, xMax, yMin, yMax, yOrg, xOrg, dx, dy,
    				ifirst, count, width;
    Bool			fTricky;
    DrawablePtr			pDrawTo;
    GCPtr			pGCTo;
    register POLYARCINFO	*polyarcs;

    width = pGC->lineWidth;
    if(width == 0 || narcs == 1)
    {
	/* don't have to worry about overlap or joinstyles */
        for(i = 0; i < narcs; i++)
	{
	    pPts = (DDXPointPtr)NULL;
	    if( cpt = miGetArcPts(&parcs[i], 0, &pPts))
	    {
		(*pGC->Polylines)(pDraw, pGC, CoordModeOrigin, cpt, pPts);
		Xfree((pointer)pPts);
	    }
	}
    }
    else 
    {
	polyarcs = (POLYARCINFO *)ALLOCATE_LOCAL(narcs * sizeof(POLYARCINFO));
	if(!polyarcs)
	    return;

	xMin = yMin = MAXSHORT;
	xMax = yMax = MINSHORT;

	/* Get all points for all the arcs. */
	for(i = 0; i < narcs; i++)
	{
	    pPts = (DDXPointPtr) NULL;
	    cpt = miGetArcPts(&parcs[i], 0, &pPts);
	    polyarcs[i].cpt = cpt;
	    polyarcs[i].pPts = pPts;
	    for(ppt = pPts, count = cpt; --count >= 0; ppt++)
	    {
		xMin = min(xMin, ppt->x);
		yMin = min(yMin, ppt->y);
		xMax = max(xMax, ppt->x);
		yMax = max(yMax, ppt->y);
	    }
	}
	/* Set up pDrawTo and pGCTo based on the rasterop */
	switch(pGC->alu)
	{
	  case GXclear:		/* 0 */
	  case GXcopy:		/* src */
	  case GXcopyInverted:	/* NOT src */
	  case GXset:		/* 1 */
	    fTricky = FALSE;
	    pDrawTo = pDraw;
	    pGCTo = pGC;
	    break;
	  default:
	    fTricky = TRUE;

	    pGCTo = GetScratchGC(1, pDraw->pScreen);
	    gcvals[GCValsLineWidth] = pGC->lineWidth;
	    gcvals[GCValsCapStyle] = pGC->capStyle;
	    gcvals[GCValsJoinStyle] = pGC->joinStyle;
	    DoChangeGC(pGCTo, GCValsMask, gcvals, 0);
    
    	    xOrg = xMin - (width + 1)/2;
	    yOrg = yMin - (width + 1)/2;
	    dx = xMax - xMin + width + 1;
	    dy = yMax - yMin + width + 1;
	    for(i = 0; i < narcs; i++)
	    {
		for(ppt = polyarcs[i].pPts, count = polyarcs[i].cpt;
		    --count >= 0;
		    ppt++)
		{
		    ppt->x -= xOrg;
		    ppt->y -= yOrg;
		}
	    }
	    if (pGC->miTranslate && (pDraw->type == DRAWABLE_WINDOW))
	    {
		xOrg += ((WindowPtr)pDraw)->absCorner.x;
		yOrg += ((WindowPtr)pDraw)->absCorner.y;
	    }

	    /* allocate a 1 bit deep pixmap of the appropriate size, and
	     * validate it */
	    pDrawTo = (DrawablePtr)(*pDraw->pScreen->CreatePixmap)
					(pDraw->pScreen, dx, dy, 1, XYBitmap);
	    ValidateGC(pDrawTo, pGCTo);
	    miClearDrawable(pDrawTo, pGCTo);
	}

	ifirst = 0;

	if (polyarcs[0].cpt)
	{
	    for (i = narcs - 1;
		 (i >= 0) &&
		 (cpt = polyarcs[i].cpt) &&
		 PtEqual(polyarcs[ifirst].pPts[0], polyarcs[i].pPts[cpt - 1]);
		 ifirst = i, i--) ;
	}

	cptAll = 0;
	pAllPts = (DDXPointPtr) NULL;

	for(i = ifirst, count = narcs; --count >= 0; i++)
	{
	    if (i == narcs)
		i = 0;	/* wrap */

	    cpt = polyarcs[i].cpt;
	    pPts = polyarcs[i].pPts;

	    if((cpt > 0) && (cptAll > 0) && PtEqual(pPts[0], pAllPts[cptAll-1]))
	    {
		cpt--;
		pAllPts = (DDXPointPtr)
		   Xrealloc((pointer)pAllPts,
			    (cptAll + cpt) * sizeof(DDXPointRec));
		bcopy((char *)&pPts[1],
		      (char *)&pAllPts[cptAll], 
		      cpt * sizeof(DDXPointRec));
		Xfree((pointer)pPts);
		cptAll += cpt;
	    }
	    else
	    {
		/* Flush what we have so far and start collecting again */
		if(cptAll > 0)
		{
		    (*pGC->Polylines)(pDrawTo, pGCTo, CoordModeOrigin,
				      cptAll, pAllPts);
		    Xfree((pointer)pAllPts);
		    if(fTricky)
		    {
		        (*pGC->PushPixels)(pGC, pDrawTo, pDraw, dx, dy,
					    xOrg, yOrg);
		        miClearDrawable((DrawablePtr)pDrawTo, pGCTo);
		    }
		}
		cptAll = cpt;
		pAllPts = pPts;
	    }
	}

	if (cptAll > 0)
	{
	    (*pGC->Polylines)(pDrawTo,pGCTo, CoordModeOrigin, cptAll, pAllPts);
	    Xfree((pointer)pAllPts);
	    if(fTricky)
		(*pGC->PushPixels)(pGC, pDrawTo, pDraw, dx, dy, xOrg, yOrg);
	}
	if(fTricky)
	{
	    (*pGCTo->pScreen->DestroyPixmap)((PixmapPtr)pDrawTo);
	    FreeScratchGC(pGCTo);
	}
	DEALLOCATE_LOCAL(polyarcs);
    }
}

/* 360 degrees * 64 sub-degree positions */
#define FULLCIRCLE 64 * 360

/* MIPOLYFILLARC -- The public entry for the PolyFillArc request.
 * Since we don't have to worry about overlapping segments, we can just
 * fill each arc as it comes.  As above, we convert the arc into a set of
 * line segments and then fill the resulting polygon.
 */
void
miPolyFillArc(pDraw, pGC, narcs, parcs)
    DrawablePtr	pDraw;
    GCPtr	pGC;
    int		narcs;
    xArc	*parcs;
{
    int	i, cpt;
    DDXPointPtr pPts;

    for(i = 0; i < narcs; i++)
    {
	/* We do this test every time because a full circle PieSlice isn't
	 * really a slice, but a full pie, and the Chord code (below) should
	 * handle it better */
        if(pGC->arcMode == ArcPieSlice && parcs[i].angle2 < FULLCIRCLE)
	{
	    pPts = (DDXPointPtr)Xalloc(sizeof(DDXPointRec));
	    if(cpt = miGetArcPts(&parcs[i], 1, &pPts))
	    {
		pPts[0].x = parcs[i].x + parcs[i].width/2;
		pPts[0].y = parcs[i].y + parcs[i].height/2;
		(*pGC->FillPolygon)(pDraw, pGC, Convex,
			            CoordModeOrigin, cpt + 1, pPts);
		Xfree((pointer) pPts);
	    }
	}
        else /* Chord */
	{
	    pPts = (DDXPointPtr)NULL;
	    if(cpt = miGetArcPts(&parcs[i], 0, &pPts))
	    {
		(*pGC->FillPolygon)(pDraw, pGC, Convex,
		                    CoordModeOrigin, cpt, pPts);
		Xfree((pointer) pPts);
	    }
	}
    }
}

/* MIGETARCPTS -- Converts an arc into a set of line segments -- a helper
 * routine for arc and line (round cap) code.
 * Returns the number of points in the arc.  Note that it takes a pointer
 * to a pointer to where it should put the points and an index (cpt).
 * This procedure allocates the space necessary to fit the arc points.
 * Sometimes it's convenient for those points to be at the end of an existing
 * array. (For example, if we want to leave a spare point to make sectors
 * instead of segments.)  So we pass in the Xalloc()ed chunk that contains the
 * array and an index saying where we should start stashing the points.
 * If there isn't an array already, we just pass in a null pointer and 
 * count on Xrealloc() to handle the null pointer correctly.
 */
int
miGetArcPts(parc, cpt, ppPts)
    xArc	*parc;	/* points to an arc */
    int		cpt;	/* number of points already in arc list */
    DDXPointPtr	*ppPts; /* pointer to pointer to arc-list -- modified */
{
    double 	st,	/* Start Theta, start angle */
                et,	/* End Theta, offset from start theta */
		dt,	/* Delta Theta, angle to sweep ellipse */
		cdt,	/* Cos Delta Theta, actually 2 cos(dt) */
    		x0, y0,	/* the recurrence formula needs two points to start */
		x1, y1,
		x2, y2, /* this will be the new point generated */
		xc, yc; /* the center point */
    int		count, i;
    DDXPointPtr	poly;

    /* The spec says that positive angles indicate counterclockwise motion.
     * Given our coordinate system (with 0,0 in the upper left corner), 
     * the screen appears flipped in Y.  The easiest fix is to negate the
     * angles given */
    
    /* Throw out multiples of 360 degrees. */
    i = -parc->angle1;
    if(i < 0)
    {
	while (i < -FULLCIRCLE)
	    i += FULLCIRCLE;
    }
    else
    {
	while(i > FULLCIRCLE)
	    i -= FULLCIRCLE;
    }
    st = (double ) i;
    st *= ((double)M_PI) / (64 * 180); 	/* convert to degrees, then to rads */

    i = -parc->angle2;
    /* If it's more than one full rotation, make it exactly 1 rotation */
    if(i > FULLCIRCLE || i < -FULLCIRCLE)
    {
	i = FULLCIRCLE;
    }
    et = (double) i;
    et *= ((double)M_PI) / (64 * 180); 	/* convert to degrees, then to rads */

    /* Try to get a delta theta that is within 1/2 pixel.  Then adjust it
     * so that it divides evenly into the total.
     * I'm just using cdt 'cause I'm lazy.
     */
    cdt = max(parc->width, parc->height)/2;
    if(cdt <= 0)
	return 0;
    dt =  ( (double)2.0 / sqrt(cdt));
    count = et/dt;
    count = abs(count) + 1;
    dt = et/count;	
    count++;

    cdt = 2 * cos(dt);

    poly = (DDXPointPtr) Xrealloc((pointer)*ppPts,
				  (count + cpt) * sizeof(DDXPointRec));
    *ppPts = poly;

    xc = parc->width/2.0;		/* store half width and half height */
    yc = parc->height/2.0;
    

    x0 = xc * cos(st);
    y0 = yc * sin(st);
    x1 = xc * cos(st + dt);
    y1 = yc * sin(st + dt);
    xc += parc->x;		/* by adding initial point, these become */
    yc += parc->y;		/* the center point */
    poly[cpt].x = ROUNDTOINT(xc + x0);
    poly[cpt].y = ROUNDTOINT(yc + y0);
    poly[cpt + 1].x = ROUNDTOINT(xc + x1);
    poly[cpt + 1].y = ROUNDTOINT(yc + y1);

    for(i = 2; i < count; i++)
    {
	x2 = cdt * x1 - x0;
	y2 = cdt * y1 - y0;
	poly[cpt + i].x = ROUNDTOINT(xc + x2);
	poly[cpt + i].y = ROUNDTOINT(yc + y2);

	x0 = x1; y0 = y1;
	x1 = x2; y1 = y2;
    }
    /* adjust the last point */
    poly[cpt +i -1].x = ROUNDTOINT(cos(st + et) * parc->width/2.0 + xc);
    poly[cpt +i -1].y = ROUNDTOINT(sin(st + et) * parc->height/2.0 + yc);

    return(count);
}
