/* $XConsortium: triStrip.c,v 1.0 93/11/22 12:51:37 rws Exp $ */

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
        TRIANGLE_STRIP(fattr, vattr, colr, fdata[i], 3, vdata[i]) 

#define  TRANSLATE_PYRAMID(which, offset) \
    for (i=0; i<4; i++) \
        TRANSLATE_VERTEX(which, offset, 3, pyramid_pts[i]) 

char test_description[] = "\
Triangle Strip Tests:\n\n\
All five pyramids in Top and Middle row should looks the same.\n\
Each pyramid is made up of 4 triangles with apex point towards viewer.\n\
white ambient and vector light(from NE direction).\n\
Top:    The first one has no option data. The second with facet normal.\n\
Middle: The first one specified with facet color and normal.\n\
	The second one specified with vertex normal.\n\
	The third one specified with vertex color and normal.\n\
Bottom: 3 triangle strips each contains 2 triangles.\n\
        The first is back facing with hatch interior, facet color of R and G.\n\
        The next two are square in shape and specified with vertex color.\n\
        The first one is shaded (R and G at top, B and R at bottom).\n\
        The second one should be colored in red and blue.\n\
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
    PEXColorRGBNormal	rgbNormal[4];
    PEXColorIndexed	index[2];
    PEXVertexIndexedNormal	vertexIndexedNormal[12];
    static PEXVertexRGB vertexRGB[] = {
	{{0.4,0.25,0.0},{1.0,0.0,0.0}},
	{{0.4,0.05,0.0},{0.0,0.0,1.0}},
	{{0.6,0.25,0.0},{0.0,1.0,0.0}},
	{{0.6,0.05,0.0},{1.0,0.0,0.0}},
    };
    static PEXCoord	vertex[] = {
	{0.05,0.05,0.0}, {0.15,0.15,0.0}, {0.25,0.05,0.0}, {0.35,0.15,0.0},
    };

    colr.rgb.red = 0.0;
    colr.rgb.green = 1.0;
    colr.rgb.blue = 0.0;

    SET_EDGE_FLAG(PEXOn);
    SET_INTERIOR_STYLE(PEXInteriorStyleSolid)
    SET_BF_INTERIOR_STYLE(PEXInteriorStyleHatch)
    SET_SURFACE_INTERP_METHOD(PEXSurfaceInterpColor)
    SET_FACET_DISTINGUISH_FLAG(PEXOn)

/* back facing, facet index color, vertex none */
    vdata[0].no_data = vertex;
    index[0].index = 2;
    index[1].index = 3;
    fdata[0].index = index;
    TRIANGLE_STRIP(PEXGAColor, PEXGANone, PEXColorTypeIndexed, 
	fdata[0], 4, vdata[0]) 

/* facet none, vertex RGB color */
    SET_SURFACE_INTERP_METHOD(PEXSurfaceInterpColor)
    vdata[0].rgb = vertexRGB;
    TRIANGLE_STRIP(PEXGANone, PEXGAColor, PEXColorTypeRGB, 
	fdata[0], 4, vdata[0]) 

    SET_SURFACE_INTERP_METHOD(PEXSurfaceInterpNone)
    TRANSLATE_VERTEX(point.x, 0.2, 4, vertexRGB)
    TRIANGLE_STRIP(PEXGANone, PEXGAColor, PEXColorTypeRGB, 
	fdata[0], 4, vdata[0]) 

/* test with vector light */
    SETUP_VECTOR_LIGHT
    SET_SURFACE_COLOR_INDEX(3);

    for (i=0; i<4; i++) 
        vdata[i].no_data = pyramid_pts[i];

/* facet none, vertex none */
    PYRAMID(PEXGANone, PEXGANone, PEXColorTypeRGB) 
    TRANSLATE_PYRAMID(x, 0.325)

/* facet normal, vertex none */
    for (i=0; i<4; i++) 
        fdata[i].normal = &pyramid_fnorm[i];
    PYRAMID(PEXGANormal, PEXGANone, PEXColorTypeRGB) 

    SET_SURFACE_COLOR_INDEX(1);
    TRANSLATE_PYRAMID(x, -0.325)
    TRANSLATE_PYRAMID(y, -0.325)

/* facet RGB color & normal, vertex none */
    for (i=0; i<4; i++) {
        fdata[i].rgb_normal = rgbNormal + i;
        fdata[i].rgb_normal[0].rgb = colr.rgb;
        fdata[i].rgb_normal[0].normal = pyramid_fnorm[i];
    }
    PYRAMID(PEXGANormal | PEXGAColor, PEXGANone, PEXColorTypeRGB) 
    TRANSLATE_PYRAMID(x, 0.325)

    SET_SURFACE_COLOR(PEXColorTypeRGB,&colr);

/* facet none, vertex normal */
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

/* facet none, vertex index colour & normal */
    for (i=0; i<4; i++) {
 	vdata[i].index_normal = vertexIndexedNormal + i * 3;
	for (j=0; j<3; j++) {
	    vdata[i].index_normal[j].point = pyramid_pts[i][j];
	    vdata[i].index_normal[j].index.index = 3;
	    vdata[i].index_normal[j].normal = pyramid_fnorm[i];
	}
    }
    PYRAMID(PEXGANone, PEXGANormal | PEXGAColor, PEXColorTypeIndexed) 

}
 
void
testproc(dpy,window,x,y,w,h)
    Display		*dpy;
    Window		window;
    int			x,y,w,h;
{
    image_testproc("triStrip", "triangle strip with data", test_description,
	dpy,window,x,y,w,h); 
} 

void
misc_setup(dpy, window, renderer)
Display		*dpy;
Window		window;
PEXRenderer	renderer;
{
}
