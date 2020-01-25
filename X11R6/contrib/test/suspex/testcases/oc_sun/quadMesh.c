/* $XConsortium: quadMesh.c,v 1.0 93/11/22 12:51:02 rws Exp $ */

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
Quad Mesh Tests:\n\n\
All five pyramids in Top and Middle row should looks the same.\n\
Each pyramid, consists of 4 quadmesh, has apex pointing towards viewer.\n\
white ambient and vector light(from NE direction).\n\
Top:    The first one has no option data. The second with facet normal.\n\
Middle: The first one specified with facet color and normal.\n\
	The second one specified with vertex normal.\n\
	The third one specified with vertex color and normal.\n\
Bottom: 3 quadmesh each contains 2 quads.\n\
        The first is back facing with hatch interior, facet color of R and G.\n\
        The next two are specified with vertex color.\n\
        The first one is shaded - \n\
            G and R at top, W and B at bottom for first quad\n\
            w and B at top, R and G at bottom for second quad\n\
        The second one should be colored in red and blue.\n\
";

#define PYRAMID(fattr,vattr,ctype) \
    for (i=0; i<4; i++) \
        QUADRILATERAL_MESH(PEXShapeUnknown, fattr, vattr, ctype, fdata[i], \
		2, 2, vdata[i]) 

#define  TRANSLATE_PYRAMID(which, offset) \
    for (i=0; i<4; i++) \
        TRANSLATE_VERTEX(which, offset, 4, pyramid_pts_quadmesh[i]) 

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
    PEXVertexNormal	vertexNormal[16];
    PEXColorRGBNormal	rgbNormal[4];
    PEXColorIndexed	index[2];
    PEXVertexIndexedNormal	vertexIndexedNormal[16];
    static PEXVertexRGB vertexRGB[] = {
	{{0.4,0.15,0.0},{1.0,1.0,0.0}},
	{{0.3,0.15,0.0},{1.0,0.0,0.0}},
	{{0.4,0.25,0.0},{0.0,0.0,1.0}},
	{{0.3,0.25,0.0},{1.0,1.0,1.0}},
	{{0.4,0.35,0.0},{1.0,0.0,0.0}},
	{{0.3,0.35,0.0},{0.0,1.0,0.0}},
    };
    static PEXCoord	vertex[] = {
	{0.25,0.15,0.0}, {0.15,0.15,0.0}, {0.05,0.25,0.0}, 
	{0.25,0.05,0.0}, {0.15,0.05,0.0}, {0.05,0.15,0.0}, 
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
    QUADRILATERAL_MESH(PEXShapeUnknown, PEXGAColor, PEXGANone, 
	PEXColorTypeIndexed, fdata[0], 3, 2, vdata[0])

/* facet none, vertex RGB color, interpolated */
    vdata[0].rgb = vertexRGB;
    QUADRILATERAL_MESH(PEXShapeUnknown, PEXGANone, PEXGAColor, 
	PEXColorTypeRGB, fdata[0], 2, 3, vdata[0])

/* facet none, vertex RGB color */
    SET_SURFACE_INTERP_METHOD(PEXSurfaceInterpNone)
    TRANSLATE_VERTEX(point.x, 0.2, 6, vertexRGB)
    QUADRILATERAL_MESH(PEXShapeUnknown, PEXGANone, PEXGAColor, 
	PEXColorTypeRGB, fdata[0], 2, 3, vdata[0])

/* test with vector light */
    SETUP_VECTOR_LIGHT
    SET_SURFACE_COLOR_INDEX(3);

    for (i=0; i<4; i++) 
        vdata[i].no_data = pyramid_pts_quadmesh[i];

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
        vdata[i].normal = vertexNormal + i * 4;
        for (j=0; j<4; j++) {
            vdata[i].normal[j].point = pyramid_pts_quadmesh[i][j];
            vdata[i].normal[j].normal = pyramid_fnorm[i];
        }
    }
    PYRAMID(PEXGANone , PEXGANormal, PEXColorTypeRGB)
    TRANSLATE_PYRAMID(x, 0.325)

    SET_SURFACE_COLOR_INDEX(1);

/* facet none, vertex index colour & normal */
    for (i=0; i<4; i++) {
 	vdata[i].index_normal = vertexIndexedNormal + i * 4;
	for (j=0; j<4; j++) {
	    vdata[i].index_normal[j].point = pyramid_pts_quadmesh[i][j];
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
    image_testproc("quadMesh", "Quad mesh test", test_description, 
	dpy,window,x,y,w,h);
} 

void
misc_setup(dpy, window, renderer)
Display		*dpy;
Window		window;
PEXRenderer	renderer;
{
}
