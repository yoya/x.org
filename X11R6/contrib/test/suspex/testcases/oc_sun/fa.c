/* $XConsortium: fa.c,v 1.0 93/11/22 12:49:29 rws Exp $ */

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
#include "line.h"

char test_description[] = "\
FillArea Tests:\n\n\
Center: A Red Fillarea (bundled color and interior style attributes) \n\
        with wide red edge\n\
Bottom: A green Fillarea with wide red edge, and a 2D red fillarea\n\
";

static PEXCoord square[] = {{0.2, 0.2, 0.0}, {0.3, 0.2, 0.0},
				{0.3, 0.3, 0.0}, {0.2, 0.3, 0.0}};
static int square_np = sizeof(square) / sizeof(PEXCoord);

static PEXCoord2D square2[] = {{0.8, 0.2}, {0.9, 0.2},
				{0.9, 0.3}, {0.8, 0.3}};
static int square_np2 = sizeof(square2) / sizeof(PEXCoord2D);

PEXMatrix matrix;
PEXVector vector;

void
outputCommands(dpy, resourceID, req_type)
    Display		*dpy;
    unsigned long	resourceID;
    PEXOCRequestType	req_type;
{	
    PEXColor		colr;

    SET_INTERIOR_STYLE(PEXInteriorStyleSolid);
    SET_SURFACE_COLOR_INDEX(2);
    FILLAREA2D(PEXShapeConvex, False, square_np2, square2);
    colr.rgb.red = 0.0;
    colr.rgb.green = 1.0;
    colr.rgb.blue = 0.0;
    SET_SURFACE_COLOR(PEXColorTypeRGB, &colr);
    SET_EDGE_FLAG(PEXOn);
    SET_EDGE_TYPE(PEXSurfaceEdgeSolid);
    SET_EDGE_WIDTH(10.0);
    colr.rgb.red = 1.0;
    colr.rgb.green = 0.0;
    colr.rgb.blue = 0.0;
    SET_EDGE_COLOR(PEXColorTypeRGB, &colr);
    FILLAREA(PEXShapeConvex, False, square_np, square);
    SET_INDIVIDUAL_ASF(PEXASFSurfaceColor, PEXBundled);
    SET_INDIVIDUAL_ASF(PEXASFInteriorStyle, PEXBundled);
    SET_INDIVIDUAL_ASF(PEXASFSurfaceEdges, PEXBundled);
    SET_INTERIOR_BUNDLE_INDEX(1);
    SET_EDGE_BUNDLE_INDEX(1);
    TRANSLATE(0.2,0.3);
    SET_LOCAL_TRANSFORM(PEXPreConcatenate, matrix);
    FILLAREA(PEXShapeConvex, False, square_np, square);
}
 
void
testproc(dpy,window,x,y,w,h)
    Display             *dpy;
    Window              window;
    int                 x,y,w,h;
{
    image_testproc("fa", "fill area", test_description, dpy,window,x,y,w,h);
}

void
misc_setup(dpy, window, renderer)
Display		*dpy;
Window		window;
PEXRenderer	renderer;
{
}

