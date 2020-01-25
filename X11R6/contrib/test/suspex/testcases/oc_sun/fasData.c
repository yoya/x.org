/* $XConsortium: fasData.c,v 1.0 93/11/22 12:49:41 rws Exp $ */

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

char test_description[] = "\
All seven pyramids in Top and Middle row should looks the same.\n\
Each pyramid, consists of 4 FillareaSetWithData (color in green),\n\
has apex pointing towards viewer.\n\
white ambient and vector light(from NE direction) in effect.\n\
Top:    The first one has no option data. The second one with facet normal.\n\
        The next one with facet color and normal.\n\
Middle: The first one specified with vertex normal.\n\
        The second one specified with vertex normal and edge.\n\
        The third one specified with vertex color and normal.\n\
        The next one specified with vertex color and normal and edge.\n\
Bottom: The first is shaded (vertex color: top is green, middle is white, \n\
        left is red, right is blue) and has solid interior, \n\
        There is also a white edge.\n\
        The next is back-facing nested rectangles and has hatch interior \n\
        and facet color of red.\n\
	The next is 2 blue triangles with white edge on the left side of the \n\
        first, white edge on the left and right side of the second.\n\
	The last is a magenta triangle with edge of the right and left side.\n\
";


#define PYRAMID(fattr,vattr,colr) \
    for (i=0; i<4; i++) \
        FILLAREA_SET_WITH_DATA(PEXShapeConvex, False, PEXContourDisjoint, \
		fattr, vattr, colr, 1, &fdata[i], &vdata[i]) 

#define  TRANSLATE_PYRAMID(which, offset) \
    for (i=0; i<4; i++) \
        TRANSLATE_VERTEX(which, offset, 3, pyramid_pts[i]) 

void
outputCommands(dpy, resourceID, req_type)
    Display		*dpy;
    XID			resourceID;
    PEXOCRequestType	req_type;
{	
    PEXColor		colr;
    int			i,j;
    PEXListOfVertex	vdata[4];
    PEXFacetData	fdata[4];
    PEXVertexNormal	vertexNormal[12];
    PEXVertexNormalEdge	vertexNormalEdge[12];
    PEXVertexIndexedNormal	vertexIndexedNormal[12];
    PEXVertexIndexedNormalEdge	vertexIndexedNormalEdge[12];
    static PEXVertexRGB vertexRGB[] = {
	{{0.2,0.1,0.0},{1.0,1.0,1.0}},
	{{0.4,0.0,0.0},{0.0,0.0,1.0}},
	{{0.2,0.2,0.0},{0.0,1.0,0.0}},
	{{0.0,0.0,0.0},{1.0,0.0,0.0}},
    };
    static PEXCoord	vertex[][4] = {
	{{0.45, 0.05, 0.0}, {0.45,0.25,0.0}, {0.6,0.25,0.0}, {0.6,0.05,0.0} },
	{{0.5, 0.1, 0.0}, {0.5, 0.2, 0.0}, {0.55, 0.2, 0.0}, {0.55, 0.1,0.0} },
    };
    static PEXVertexRGBEdge	vertexRGBEdge[] = {
	{{0.95, 0.05, 0.0},{1.0,0.0,1.0},PEXOn}, 
	{{0.9,0.15,0.0},{1.0,0.0,1.0}, PEXOn}, 
	{{0.85,0.05,0.0},{1.0,0.0,1.0},PEXOff}, 
    };
    static PEXVertexEdge vertexEdge[][3] = {
	{{{0.75, 0.05, 0.0},PEXOn}, 
	 {{0.7,0.15,0.0}, PEXOff}, 
	 {{0.65,0.05,0.0},PEXOff}}, 
	{{{0.85, 0.05, 0.0},PEXOn}, 
	 {{0.8,0.15,0.0}, PEXOn}, 
	 {{0.75,0.05,0.0},PEXOff}}, 
    };

    colr.rgb.red = 0.0;
    colr.rgb.green = 1.0;
    colr.rgb.blue = 0.0;

    SET_EDGE_FLAG(PEXOn);
    SET_INTERIOR_STYLE(PEXInteriorStyleSolid)
    SET_BF_INTERIOR_STYLE(PEXInteriorStyleHatch)
    SET_FACET_DISTINGUISH_FLAG(PEXOn)

/* back facing, facet index color, vertex none, ignore edge */
    for (i=0; i<2; i++) {
    	vdata[i].count = 4;
    	vdata[i].vertices.no_data = vertex[i];
    }
    fdata[0].index.index = 2;
    FILLAREA_SET_WITH_DATA(PEXShapeConvex, True, PEXContourNested, 
	PEXGAColor, PEXGANone, PEXColorTypeIndexed, 2, &fdata[0], &vdata[0]) 

/* facet none, vertex edge */
    for (i=0; i<2; i++) {
    	vdata[i].count = 3;
    	vdata[i].vertices.edge = vertexEdge[i];
    }
    SET_SURFACE_COLOR_INDEX(4);
    FILLAREA_SET_WITH_DATA(PEXShapeConvex, False, PEXContourUnknown, 
	PEXGANone, PEXGAEdges, PEXColorTypeIndexed, 2, &fdata[0], &vdata[0]) 

/* facet none, vertex edge RGB color */
    vdata[0].count = 3;
    vdata[0].vertices.rgb_edge = vertexRGBEdge;
    FILLAREA_SET_WITH_DATA(PEXShapeConvex, False, PEXContourUnknown, 
	PEXGANone, PEXGAEdges | PEXGAColor, PEXColorTypeRGB, 1, 
        &fdata[0], &vdata[0]) 

/* facet none, vertex RGB color */
    SET_SURFACE_INTERP_METHOD(PEXSurfaceInterpColor)
    vdata[0].count = 4;
    vdata[0].vertices.rgb = vertexRGB;
    FILLAREA_SET_WITH_DATA(PEXShapeConvex, False, PEXContourUnknown,
	PEXGANone, PEXGAColor, PEXColorTypeRGB, 1, &fdata[0], &vdata[0]) 
    SET_SURFACE_INTERP_METHOD(PEXSurfaceInterpNone)

/* test with vector light */
    SETUP_VECTOR_LIGHT
    SET_SURFACE_COLOR_INDEX(3);

    for (i=0; i<4; i++) {
        vdata[i].count = 3;
        vdata[i].vertices.no_data = pyramid_pts[i];
    }

/* facet none, vertex none */
    PYRAMID(PEXGANone, PEXGANone, PEXColorTypeRGB) 
    TRANSLATE_PYRAMID(x, 0.325)

/* facet normal, vertex none */
    for (i=0; i<4; i++) 
        fdata[i].normal = pyramid_fnorm[i];
    PYRAMID(PEXGANormal, PEXGANone, PEXColorTypeRGB) 

    SET_SURFACE_COLOR_INDEX(1);

    TRANSLATE_PYRAMID(x, 0.325)

/* facet RGB color & normal, vertex none */
    for (i=0; i<4; i++) {
        fdata[i].rgb_normal.rgb = colr.rgb;
        fdata[i].rgb_normal.normal = pyramid_fnorm[i];
    }
    PYRAMID(PEXGANormal | PEXGAColor, PEXGANone, PEXColorTypeRGB) 
    TRANSLATE_PYRAMID(x, -0.65)
    TRANSLATE_PYRAMID(y, -0.25)

    SET_SURFACE_COLOR(PEXColorTypeRGB,&colr);

/* facet none, vertex normal */
    for (i=0; i<4; i++) {
        vdata[i].vertices.normal = vertexNormal + i * 3;
        for (j=0; j<3; j++) {
            vdata[i].vertices.normal[j].point = pyramid_pts[i][j];
            vdata[i].vertices.normal[j].normal = pyramid_fnorm[i];
        }
    }
    PYRAMID(PEXGANone , PEXGANormal, PEXColorTypeRGB)
    TRANSLATE_PYRAMID(x, 0.325)

/* facet none, vertex normal edge */
    for (i=0; i<4; i++) {
        vdata[i].vertices.normal_edge = vertexNormalEdge + i * 3;
        for (j=0; j<3; j++) {
            vdata[i].vertices.normal_edge[j].point = pyramid_pts[i][j];
            vdata[i].vertices.normal_edge[j].normal = pyramid_fnorm[i];
            vdata[i].vertices.normal_edge[j].edge = PEXOn;
        }
    }
    PYRAMID(PEXGANone , PEXGANormal | PEXGAEdges, PEXColorTypeRGB)
    TRANSLATE_PYRAMID(x, 0.325)

    SET_SURFACE_COLOR_INDEX(1);

/* facet none, vertex index colour & normal */
    for (i=0; i<4; i++) {
 	vdata[i].vertices.index_normal = vertexIndexedNormal + i * 3;
	for (j=0; j<3; j++) {
	    vdata[i].vertices.index_normal[j].point = pyramid_pts[i][j];
	    vdata[i].vertices.index_normal[j].index.index = 3;
	    vdata[i].vertices.index_normal[j].normal = pyramid_fnorm[i];
	}
    }
    PYRAMID(PEXGANone, PEXGANormal|PEXGAColor, PEXColorTypeIndexed) 
    TRANSLATE_PYRAMID(x, -0.65)
    TRANSLATE_PYRAMID(y, -0.25)

/* facet none, vertex index colour, normal and edge*/
    for (i=0; i<4; i++) {
 	vdata[i].vertices.index_normal_edge = vertexIndexedNormalEdge + i * 3;
	for (j=0; j<3; j++) {
	    vdata[i].vertices.index_normal_edge[j].point = pyramid_pts[i][j];
	    vdata[i].vertices.index_normal_edge[j].index.index = 3;
	    vdata[i].vertices.index_normal_edge[j].normal = pyramid_fnorm[i];
	    vdata[i].vertices.index_normal_edge[j].edge = PEXOn;
	}
    }
    PYRAMID(PEXGANone, PEXGANormal|PEXGAColor|PEXGAEdges, PEXColorTypeIndexed) 
}
 
void
testproc(dpy,window,x,y,w,h)
    Display		*dpy;
    Window		window;
    int			x,y,w,h;
{
    image_testproc("fasData", "fill area set with data", test_description,
		dpy,window,x,y,w,h);
} 

void
misc_setup(dpy, window, renderer)
Display		*dpy;
Window		window;
PEXRenderer	renderer;
{
}
