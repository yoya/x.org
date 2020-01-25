/* $XConsortium: sofas.c,v 1.0 93/11/22 12:51:19 rws Exp $ */

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
#include "geometry.h"
#include "misc.h"

#define PYRAMID(fattr,vattr,colr) \
    for (i=0; i<4; i++) \
        SETOF_FILLAREA_SETS(PEXShapeConvex, fattr, vattr, PEXGANone, \
		PEXContourDisjoint, True, colr, 1, fdata[i], 3, vdata[i], 3, \
		NULL, &connect)

#define  TRANSLATE_PYRAMID(which, offset) \
    for (i=0; i<4; i++) \
        TRANSLATE_VERTEX(which, offset, 3, pyramid_pts[i]) 

char test_description[] = "\
SOFAS Tests:\n\n\
All five pyramids in Top and Middle row should looks the same.\n\
Each pyramid, consists of 4 sofas, has apex pointing towards viewer.\n\
white ambient and vector light(from NE direction).\n\
Top:    The first one has no option data. The second with facet normal.\n\
        The third one has facet color and normal data.\n\
Middle: The first one specified with vertex normal.\n\
	The third one specified with vertex color and normal.\n\
Bottom: On the left is 2 sets of 2 fillareas, back-facing with hatch \n\
        interior, specified with no optional data and is shaded\n\
        The next one is a 1 set of nested rectangles in red, white edges on\n\
        1 side of the inner contour and 3 sides of the outer contour.\n\
        The next one is 1 set of 1 fillarea in a shape of a triangle,\n\
        specified with facet color(green) and white edge on 1 side.\n\
";

void
outputCommands(dpy, resourceID, req_type)
    Display		*dpy;
    XID			resourceID;
    PEXOCRequestType	req_type;
{	
    PEXColor		colr;
    int			i,j;
    PEXArrayOfVertex	vdata[4];
    PEXArrayOfFacetData	fdata[4];
    PEXVertexNormal	vertexNormal[12];
    PEXVertexIndexedNormal	vertexIndexedNormal[12];
    PEXColorRGBNormal 	rgbNormal[4];
    PEXColorIndexed 	index;

    static unsigned short defData[] = {0,1,2};
    static PEXListOfUShort defList = {3, defData};

    static unsigned short sqrData1[] = {0,1,2,3};
    static unsigned short sqrData2[] = {4,5,6,7};
    static PEXListOfUShort sqrsList[] = { {4, sqrData1}, {4,sqrData2} };

    static unsigned short triData0[] = {0,1,2};
    static PEXListOfUShort triList0[] = { {3, triData0} };

    static unsigned short triData11[] = {0,1,2};
    static unsigned short triData12[] = {1,2,3};
    static PEXListOfUShort triList1[] = { {3, triData11}, {3,triData12} };

    static unsigned short triData21[] = {4,5,6};
    static unsigned short triData22[] = {5,6,7};
    static PEXListOfUShort triList2[] = { {3, triData21}, {3,triData22} };

    static PEXConnectivityData connect;
    static PEXConnectivityData connect_arr[] = { {2,triList1},{2,triList2} };

    static PEXSwitch sqrsEdata[] = { PEXOn, PEXOn, PEXOn, PEXOff, 
                                 PEXOn, PEXOff, PEXOff, PEXOff, };
    static PEXSwitch triEdata[] = { PEXOn, PEXOff, PEXOff };

    static PEXVertexRGB trisVertexRGB[] = {
	{{0.3,0.3,0.0},{1.0,1.0,1.0}},
	{{0.2,0.2,0.0},{0.0,0.0,1.0}},
	{{0.1,0.3,0.0},{0.0,1.0,0.0}},
	{{0.0,0.2,0.0},{1.0,0.0,0.0}},

	{{0.3,0.15,0.0},{1.0,0.0,0.0}},
	{{0.2,0.05,0.0},{0.0,1.0,0.0}},
	{{0.1,0.15,0.0},{0.0,0.0,1.0}},
	{{0.0,0.05,0.0},{1.0,1.0,1.0}},
    };
    static PEXCoord	sqrsVertex[] = {
	{0.45, 0.05, 0.0}, {0.6,0.05,0.0}, {0.6,0.25,0.0}, {0.45,0.25,0.0},
	{0.5, 0.1, 0.0}, {0.55, 0.1, 0.0}, {0.55, 0.2, 0.0}, {0.5, 0.2,0.0},
    };
    static PEXCoord	triVertex[] = {
	{0.8, 0.1, 0.0}, {0.7,0.2,0.0}, {0.6,0.1,0.0} ,
    };

    colr.rgb.red = 0.0;
    colr.rgb.green = 1.0;
    colr.rgb.blue = 0.0;

    SET_EDGE_FLAG(PEXOn);
    SET_INTERIOR_STYLE(PEXInteriorStyleSolid)
    SET_BF_SURFACE_INTERP_METHOD(PEXSurfaceInterpColor)
    SET_BF_INTERIOR_STYLE(PEXInteriorStyleHatch)
    SET_FACET_DISTINGUISH_FLAG(PEXOn)

/* back facing, facet none, vertex RGB color, edges none, 
   2 sets of 2 fillareas */

    vdata[0].rgb = trisVertexRGB;
    SETOF_FILLAREA_SETS(PEXShapeConvex, PEXGANone, PEXGAColor, PEXGANone, 
		PEXContourUnknown, False, PEXColorTypeRGB, 2, fdata[0], 
		8, vdata[0], 12, NULL, connect_arr)

/* facet none, vertex none, edges */

    vdata[0].no_data = triVertex;
    connect.count = 1;
    connect.lists = triList0;
    SET_SURFACE_COLOR(PEXColorTypeRGB, &colr);
    SETOF_FILLAREA_SETS(PEXShapeConvex, PEXGANone, PEXGANone, PEXGAEdges, 
		PEXContourUnknown, True, PEXColorTypeRGB, 1, fdata[0], 
		3, vdata[0], 3, triEdata, &connect)

/* facet index color, vertex none, edges, 1 set of 2 fillareas */

    connect.count = 2;
    connect.lists = sqrsList;
    vdata[0].no_data = sqrsVertex;
    fdata[0].index = &index;
    fdata[0].index[0].index = 2;
    SETOF_FILLAREA_SETS(PEXShapeConvex, PEXGAColor, PEXGANone, PEXGAEdges, 
		PEXContourNested, False, PEXColorTypeIndexed, 1, fdata[0], 
		8, vdata[0], 8, sqrsEdata, &connect)

/* test with vector light */
    SETUP_VECTOR_LIGHT
    SET_SURFACE_COLOR_INDEX(3);

    for (i=0; i<4; i++) 
        vdata[i].no_data = pyramid_pts[i];

    connect.count = 1;
    connect.lists = &defList;

/* facet none, vertex none, edge none */
    PYRAMID(PEXGANone, PEXGANone, PEXColorTypeRGB)
    TRANSLATE_PYRAMID(x, 0.325)

/* facet normal, vertex none, edge none */
    for (i=0; i<4; i++) 
        fdata[i].normal = &pyramid_fnorm[i];
    PYRAMID(PEXGANormal, PEXGANone, PEXColorTypeRGB)

    SET_SURFACE_COLOR_INDEX(1)

    TRANSLATE_PYRAMID(x, 0.325)

/* facet RGB color & normal, vertex none, edge none */
    for (i=0; i<4; i++) {
        fdata[i].rgb_normal = &rgbNormal[i];
        fdata[i].rgb_normal[0].rgb = colr.rgb;
        fdata[i].rgb_normal[0].normal = pyramid_fnorm[i];
    }
    PYRAMID(PEXGANormal | PEXGAColor, PEXGANone, PEXColorTypeRGB) 
    TRANSLATE_PYRAMID(x, -0.65)
    TRANSLATE_PYRAMID(y, -0.25)

    SET_SURFACE_COLOR(PEXColorTypeRGB,&colr);

/* facet none, vertex normal, edge none */
    for (i=0; i<4; i++) {
        vdata[i].normal = vertexNormal + i * 3;
        for (j=0; j<3; j++) {
            vdata[i].normal[j].point = pyramid_pts[i][j];
            vdata[i].normal[j].normal = pyramid_fnorm[i];
        }
    }
    PYRAMID(PEXGANone , PEXGANormal, PEXColorTypeRGB)
    TRANSLATE_PYRAMID(x, 0.325)

    SET_SURFACE_COLOR_INDEX(1);

/* facet none, vertex index colour & normal, edge none */
    for (i=0; i<4; i++) {
 	vdata[i].index_normal = vertexIndexedNormal + i * 3;
	for (j=0; j<3; j++) {
	    vdata[i].index_normal[j].point = pyramid_pts[i][j];
	    vdata[i].index_normal[j].index.index = 3;
	    vdata[i].index_normal[j].normal = pyramid_fnorm[i];
	}
    }
    PYRAMID(PEXGANone, PEXGANormal|PEXGAColor, PEXColorTypeIndexed) 
}
 
void
testproc(dpy,window,x,y,w,h)
    Display		*dpy;
    Window		window;
    int			x,y,w,h;
{
    image_testproc("sofas", "set of fill area set with data", test_description,
		dpy,window,x,y,w,h);
} 

void
misc_setup(dpy, window, renderer)
Display		*dpy;
Window		window;
PEXRenderer	renderer;
{
}
