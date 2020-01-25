/* $XConsortium: edgeAttr.c,v 1.0 93/11/22 12:49:17 rws Exp $ */

/*
        NOTICE TO USER: The source code in this file is copyrighted under
        U.S. and international laws.  SunSoft, Inc., a Sun Microsystems,
        Inc. business of Mountain View, California owns the copyright.
        Users and possessors of this source code are hereby granted a
        nonexclusive, royalty-free copyright and design patent license
        to use this code in individual and commercial software.

        Any use of this source code must include, in the user documentation
        and internal comments to the code, notices to the end user as follows:

(c) Copyright 1992, 1993 SunSoft, Inc., a Sun Microsystems, Inc. business.
 
        SUNSOFT, INC. AND SUN MICROSYSTEMS, INC. MAKE NO REPRESENTATIONS
        ABOUT THE SUITABILITY OF THIS SOURCE CODE FOR ANY PURPOSE. IT IS
        PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY OF ANY KIND.
        SUNSOFT, INC., AND SUN MICROSYSTEMS, INC., SEVERALLY AND
        INDIVIDUALLY, DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOURCE
        CODE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
        FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL SUNSOFT, INC.
        OR SUN MICROSYSTEMS, INC., BE LIABLE FOR ANY SPECIAL, INDIRECT,
        INCIDENTAL, OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER
        RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
        OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
        OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOURCE CODE.
*/ 

#include <stdio.h>
#include <X11/PEX5/PEXlib.h>
#include "surface.h"
#include "misc.h"

char test_description[] = "\
Edge Attibutes Tests:\n\n\
Fill area are rendered with empty interior and different edges attributes.\n\
Upper: First 4 edges in blue - solid, dash, dot, dashdot.\n\
       Next is wide dashdot edges in red (bundle attributes)\n\
Lower: First one rendered with default edge attributes (edge flag off)\n\
       is invisible. Next 5 are in green with increasing widths from 0-4.\n\
       Last one is specified with unsupported type and should rendered as\n\
       default.\n\
";

void
outputCommands(dpy, resourceID, req_type)
    Display		*dpy;
    XID			resourceID;
    PEXOCRequestType	req_type;
{	
    PEXColor		colr;
    int			i;

    static		PEXCoord pts1[] = { 
	0.0, 0.4, 0.0,
	0.1, 0.2, 0.0,
	0.0, 0.1, 0.0,
    };

    static		PEXCoord pts2[] = { 
	0.1, 0.8, 0.0,
	0.2, 0.6, 0.0,
	0.1, 0.5, 0.0,
    };

    static int		edgeType[] = {
	PEXSurfaceEdgeSolid,
	PEXSurfaceEdgeDashed,
	PEXSurfaceEdgeDotted,
	PEXSurfaceEdgeDashDot,
    };

    SET_INTERIOR_STYLE(PEXInteriorStyleEmpty)

/* default edge attrs */
    FILLAREA(PEXShapeConvex, False, 3, pts1);
    TRANSLATE_VERTEX(x, 0.12, 3, pts1)

/* enable edge flag */
    SET_EDGE_FLAG(PEXOn);

/* fillarea specified ignore edge flag */
    FILLAREA(PEXShapeConvex, True, 3, pts1);
    TRANSLATE_VERTEX(x, 0.12, 3, pts1)

/* edge color index */
    SET_EDGE_COLOR_INDEX(5);

/* various edge width */
    for (i=0; i < 5; ++i ) {
        SET_EDGE_WIDTH((double) i);
	FILLAREA(PEXShapeConvex, False, 3, pts1);
	TRANSLATE_VERTEX(x, 0.12, 3, pts1)
    }

/* edge width = 0.0, type = -1, color index = 128 */
    SET_EDGE_WIDTH(0.0);
    SET_EDGE_TYPE(-1);
    SET_EDGE_COLOR_INDEX(128);
    FILLAREA(PEXShapeConvex, False, 3, pts1);

/* edge colour */
    colr.rgb.red = 0.0;
    colr.rgb.green = 0.0;
    colr.rgb.blue = 1.0;
    SET_EDGE_COLOR(PEXColorTypeRGB,&colr);

/* various edge types */
    SET_EDGE_WIDTH(2.0);
    for (i=0; i < sizeof(edgeType)/sizeof(int); ++i ) {
    	SET_EDGE_TYPE(edgeType[i])
	FILLAREA(PEXShapeConvex, False, 3, pts2);
	TRANSLATE_VERTEX(x, 0.2, 3, pts2)
    }

/* edge bundle */
    SET_EDGE_BUNDLE_INDEX(1);
    SET_INDIVIDUAL_ASF(PEXASFSurfaceEdgeType,PEXIndividual);
    SET_INDIVIDUAL_ASF(PEXASFSurfaceEdgeWidth,PEXBundled);
    SET_INDIVIDUAL_ASF(PEXASFSurfaceEdgeColor,PEXBundled);
    FILLAREA(PEXShapeConvex, False, 3, pts2);
}
 
void
testproc(dpy,window,x,y,w,h)
    Display		*dpy;
    Window		window;
    int			x,y,w,h;
{
    image_testproc("edgeAttrs","edge attributes", test_description,
		dpy,window,x,y,w,h);
} 

void
misc_setup(dpy, window, renderer)
Display		*dpy;
Window		window;
PEXRenderer	renderer;
{
}
