/* $XConsortium: intrAttr.c,v 1.0 93/11/22 12:50:04 rws Exp $ */

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
#include <X11/PEX5/PEXlib.h>
#include <stdio.h>
#include "surface.h"
#include "misc.h"

char test_description[] = "\
Interior Attributes Tests:\n\n\
Note: fa=fill area; fas=fill area set\n\
Top row:    fa2D, depth cued fa, nested fas2D, self-intersecting fas\n\
Mid row:    test front face attributes\n\
            4fa - hatched in blue, empty/white dot edge,\n\
                  hollow green/white dot edge, solid yellow (bundled attrs)\n\
Bottom row: test back face attributes\n\
            4fa - hatched in red, empty with white dashdot edge,\n\
                  hollow blue/white dashdot edge, solid blue (bundled attrs)\n\
";

void
outputCommands(dpy, resourceID, req_type)
    Display		*dpy;
    XID			resourceID;
    PEXOCRequestType	req_type;
{	
    PEXColor		colr;
    int			i,j;

    static		PEXCoord2D pts0[] = { 
	0.05, 0.7,
	0.15, 0.8,
	0.25, 0.7,
    };
    static		PEXCoord pts1[] = { 
	0.05, 0.5, 1.0,
	0.15, 0.6, 0.0,
	0.25, 0.5, 1.0,
    };
    static              PEXCoord pts2[] = {
        0.45, 0.7, 0.0,
        0.45, 0.8, 0.0,
        0.65, 0.8, 0.0,
        0.65, 0.7, 0.0,
    };
    static              PEXCoord pts3[] = {
        0.45, 0.75, 0.0,
        0.55, 0.8, 0.0,
        0.65, 0.75, 0.0,
        0.55, 0.7, 0.0,
    };
    static		PEXCoord2D pts4[] = { 
	0.75, 0.9,
	0.95, 0.9,
	0.95, 0.7,
	0.65, 0.7,
	0.65, 0.85,
	0.85, 0.85,
	0.85, 0.75,
	0.75, 0.75,
    };
    static		PEXCoord pts5[] = { 
	0.25, 0.7, 1.0,
	0.35, 0.8, 0.0,
	0.45, 0.7, 1.0,
    };
    static		PEXListOfCoord2D fas1[] = { {8, pts4} };
    static		PEXListOfCoord fas2[] = { {4, pts2},{4, pts3}, };

    colr.rgb.red = 0.0;
    colr.rgb.green = 0.0;
    colr.rgb.blue = 1.0;

/* Testing front face attributes */

    /* default attrs */
    FILLAREA2D(PEXShapeConvex, False, 3, pts0);

    /* depth cued, solid */
    SET_DEPTH_CUE_INDEX(1)
    SET_INTERIOR_STYLE(PEXInteriorStyleSolid)
    FILLAREA(PEXShapeConvex, False, 3, pts5);
    SET_DEPTH_CUE_INDEX(0)

    /* fill area set, solid */ 
    FILLAREA_SET(PEXShapeConvex, False, PEXContourNested, 2, fas2);

    /* fill area set 2D, hatch */
    SET_INTERIOR_STYLE(PEXInteriorStyleHatch)
    FILLAREA_SET2D(PEXShapeComplex, False, PEXContourDisjoint, 1, fas1);

    /* interior style HATCH, surface colour */
    SET_INTERIOR_STYLE(PEXInteriorStyleHatch)
    SET_SURFACE_COLOR(PEXColorTypeRGB,&colr);
    SET_INTERIOR_STYLE_INDEX(1)
    FILLAREA(PEXShapeConvex, False, 3, pts1);
    TRANSLATE_VERTEX(x, 0.2, 3, pts1);

    /* interior styles EMPTY, surface color index,
    surface edge DOTTED, surface edge color index  */
    SET_EDGE_TYPE(PEXSurfaceEdgeDotted);
    SET_EDGE_COLOR_INDEX(1);
    SET_EDGE_FLAG(PEXOn);
    SET_SURFACE_COLOR_INDEX(3);
    SET_INTERIOR_STYLE(PEXInteriorStyleEmpty)
    FILLAREA(PEXShapeConvex, False, 3, pts1);
    TRANSLATE_VERTEX(x, 0.2, 3, pts1);

    /* interior styles HOLLOW, surface edge DOTTED, edge color index */
    SET_INTERIOR_STYLE(PEXInteriorStyleHollow)
    FILLAREA(PEXShapeConvex, False, 3, pts1);
    TRANSLATE_VERTEX(x, 0.2, 3, pts1);
    SET_EDGE_FLAG(PEXOff);

    /* interior bundle */
    SET_INTERIOR_BUNDLE_INDEX(1);
    SET_INDIVIDUAL_ASF(PEXASFInteriorStyle,PEXBundled);
    SET_INDIVIDUAL_ASF(PEXASFInteriorStyleIndex,PEXBundled);
    SET_INDIVIDUAL_ASF(PEXASFSurfaceColor,PEXIndividual);
    FILLAREA(PEXShapeConvex, False, 3, pts1);


/* Testing back face attributes */
    SET_FACET_DISTINGUISH_FLAG(PEXOn)

    colr.rgb.red = 1.0;
    colr.rgb.green = 0.0;
    colr.rgb.blue = 0.0;

    /* interior style HATCH, surface colour */
    TRANSLATE_VERTEX(x, -0.6, 3, pts1);
    TRANSLATE_VERTEX(y, -0.2, 3, pts1);
    SET_BF_INTERIOR_STYLE(PEXInteriorStyleHatch)
    SET_BF_SURFACE_COLOR(PEXColorTypeRGB,&colr);
    SET_BF_INTERIOR_STYLE_INDEX(1)
    FILLAREA(PEXShapeConvex, False, 3, pts1);
    TRANSLATE_VERTEX(x, 0.2, 3, pts1);

    /* interior styles EMPTY, surface color index,
    surface edge DASHED, edge color index  */
    SET_EDGE_TYPE(PEXSurfaceEdgeDashDot);
    SET_EDGE_FLAG(PEXOn);
    SET_BF_SURFACE_COLOR_INDEX(4);
    SET_BF_INTERIOR_STYLE(PEXInteriorStyleEmpty)
    FILLAREA(PEXShapeConvex, False, 3, pts1);
    TRANSLATE_VERTEX(x, 0.2, 3, pts1);

    /* interior styles HOLLOW, surface edge DASHDOT, surface edge color index  */
    SET_BF_INTERIOR_STYLE(PEXInteriorStyleHollow)
    FILLAREA(PEXShapeConvex, False, 3, pts1);
    TRANSLATE_VERTEX(x, 0.2, 3, pts1);
    SET_EDGE_FLAG(PEXOff);

    /* interior bundle */
    SET_INTERIOR_BUNDLE_INDEX(1);
    SET_INDIVIDUAL_ASF(PEXASFBFInteriorStyle,PEXBundled);
    SET_INDIVIDUAL_ASF(PEXASFBFInteriorStyleIndex,PEXBundled);
    SET_INDIVIDUAL_ASF(PEXASFBFSurfaceColor,PEXIndividual);
    FILLAREA(PEXShapeConvex, False, 3, pts1);

}
 
void
testproc(dpy,window,x,y,w,h)
    Display		*dpy;
    Window		window;
    int			x,y,w,h;
{
    image_testproc("intrAttr",
	"interior attributes with fillarea and fillareaset", test_description,
	dpy,window,x,y,w,h);
} 

void
misc_setup(dpy, window, renderer)
Display		*dpy;
Window		window;
PEXRenderer	renderer;
{
}
