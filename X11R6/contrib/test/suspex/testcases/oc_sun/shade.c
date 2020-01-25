/* $XConsortium: shade.c,v 1.0 93/11/22 12:51:13 rws Exp $ */

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
#include "oc.h"

char test_description[] = "\
Shading Tests:\n\n\
Tested with FillareaWithData primitive.\n\
(from Bottom to Top)\n\
1st row: 1. a solid red square\n\
         2. no interpolation, same as above\n\
         3. no reflection, same as above\n\
         4. enable ambient reflection, the red square looks dimmer than before\n\
         5. back face, no lighting effect, same color as in #1\n\
2nd row: 1. also enable bf ambient reflection, red square looks dimmer\n\
         2. enable color interpolation, the square is now shaded\n\
            (top: R and B, bottom R and G)\n\
         3. back face, no color interpolation\n\
         4. also enable back face color interpolation, the square is now \n\
            shaded as in #2\n\
         5. back to front face, same as #2 \n\
3rd row: all 5 squares looks the same as the shaded square with ambient\n\
         lighting in previous cases\n\
4th row: all 4 squares looks the same as the shaded square with ambient\n\
         lighting in previous cases\n\
5th row: all 3 squares looks the same as the shaded square with ambient\n\
         lighting in previous cases\n\
";

/* #include "polygon.h" */

static  PEXVector normal1 = {0.0, 0.0, 1.0};
static  PEXVector normal2 = {0.0, 0.0, -1.0};
static  PEXVertexRGBNormal square1[4] = {
	{{0.05, 0.05, 0.0}, {1.0, 0.0, 0.0}, {0.0, 0.0, 1.0}},
	{{0.15, 0.05, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}},
	{{0.15, 0.15, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}},
	{{0.05, 0.15, 0.0}, {0.6, 0.3, 0.0}, {0.0, 0.0, 1.0}}};
static  PEXVertexRGBNormal square2[4] = {
	{{0.25, 0.05, 0.0}, {1.0, 0.0, 0.0}, {0.0, 0.0, 1.0}},
	{{0.35, 0.05, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}},
	{{0.35, 0.15, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}},
	{{0.25, 0.15, 0.0}, {0.6, 0.3, 0.0}, {0.0, 0.0, 1.0}}};
static  PEXVertexRGBNormal square3[4] = {
	{{0.45, 0.05, 0.0}, {1.0, 0.0, 0.0}, {0.0, 0.0, 1.0}},
	{{0.55, 0.05, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}},
	{{0.55, 0.15, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}},
	{{0.45, 0.15, 0.0}, {0.6, 0.3, 0.0}, {0.0, 0.0, 1.0}}};
static  PEXVertexRGBNormal square4[4] = {
	{{0.65, 0.05, 0.0}, {1.0, 0.0, 0.0}, {0.0, 0.0, 1.0}},
	{{0.75, 0.05, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}},
	{{0.75, 0.15, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}},
	{{0.65, 0.15, 0.0}, {0.6, 0.3, 0.0}, {0.0, 0.0, 1.0}}};
static  PEXVertexRGBNormal square5[4] = {
	{{0.85, 0.05, 0.0}, {1.0, 0.0, 0.0}, {0.0, 0.0, 1.0}},
	{{0.95, 0.05, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}},
	{{0.95, 0.15, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}},
	{{0.85, 0.15, 0.0}, {0.6, 0.3, 0.0}, {0.0, 0.0, 1.0}}};
static  PEXVertexRGBNormal square6[4] = {
	{{0.05, 0.25, 0.0}, {1.0, 0.0, 0.0}, {0.0, 0.0, 1.0}},
	{{0.15, 0.25, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}},
	{{0.15, 0.35, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}},
	{{0.05, 0.35, 0.0}, {0.6, 0.3, 0.0}, {0.0, 0.0, 1.0}}};
static  PEXVertexRGBNormal square7[4] = {
	{{0.25, 0.25, 0.0}, {1.0, 0.0, 0.0}, {0.0, 0.0, 1.0}},
	{{0.35, 0.25, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}},
	{{0.35, 0.35, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}},
	{{0.25, 0.35, 0.0}, {0.6, 0.3, 0.0}, {0.0, 0.0, 1.0}}};
static  PEXVertexRGBNormal square8[4] = {
	{{0.45, 0.25, 0.0}, {1.0, 0.0, 0.0}, {0.0, 0.0, 1.0}},
	{{0.55, 0.25, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}},
	{{0.55, 0.35, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}},
	{{0.45, 0.35, 0.0}, {0.6, 0.3, 0.0}, {0.0, 0.0, 1.0}}};
static  PEXVertexRGBNormal square9[4] = {
	{{0.65, 0.25, 0.0}, {1.0, 0.0, 0.0}, {0.0, 0.0, 1.0}},
	{{0.75, 0.25, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}},
	{{0.75, 0.35, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}},
	{{0.65, 0.35, 0.0}, {0.6, 0.3, 0.0}, {0.0, 0.0, 1.0}}};
static  PEXVertexRGBNormal square10[4] = {
	{{0.85, 0.25, 0.0}, {1.0, 0.0, 0.0}, {0.0, 0.0, 1.0}},
	{{0.95, 0.25, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}},
	{{0.95, 0.35, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}},
	{{0.85, 0.35, 0.0}, {0.6, 0.3, 0.0}, {0.0, 0.0, 1.0}}};
static  PEXVertexRGBNormal square11[4] = {
	{{0.05, 0.45, 0.0}, {1.0, 0.0, 0.0}, {0.0, 0.0, 1.0}},
	{{0.15, 0.45, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}},
	{{0.15, 0.55, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}},
	{{0.05, 0.55, 0.0}, {0.6, 0.3, 0.0}, {0.0, 0.0, 1.0}}};
static  PEXVertexRGBNormal square12[4] = {
	{{0.25, 0.45, 0.0}, {1.0, 0.0, 0.0}, {0.0, 0.0, 1.0}},
	{{0.35, 0.45, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}},
	{{0.35, 0.55, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}},
	{{0.25, 0.55, 0.0}, {0.6, 0.3, 0.0}, {0.0, 0.0, 1.0}}};
static  PEXVertexRGBNormal square13[4] = {
	{{0.45, 0.45, 0.0}, {1.0, 0.0, 0.0}, {0.0, 0.0, 1.0}},
	{{0.55, 0.45, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}},
	{{0.55, 0.55, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}},
	{{0.45, 0.55, 0.0}, {0.6, 0.3, 0.0}, {0.0, 0.0, 1.0}}};
static  PEXVertexRGBNormal square14[4] = {
	{{0.65, 0.45, 0.0}, {1.0, 0.0, 0.0}, {0.0, 0.0, 1.0}},
	{{0.75, 0.45, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}},
	{{0.75, 0.55, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}},
	{{0.65, 0.55, 0.0}, {0.6, 0.3, 0.0}, {0.0, 0.0, 1.0}}};
static  PEXVertexRGBNormal square15[4] = {
	{{0.85, 0.45, 0.0}, {1.0, 0.0, 0.0}, {0.0, 0.0, 1.0}},
	{{0.95, 0.45, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}},
	{{0.95, 0.55, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}},
	{{0.85, 0.55, 0.0}, {0.6, 0.3, 0.0}, {0.0, 0.0, 1.0}}};
static  PEXVertexRGBNormal square16[4] = {
	{{0.05, 0.65, 0.7}, {1.0, 0.0, 0.0}, {0.0, 0.0, 1.0}},
	{{0.15, 0.65, 0.7}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}},
	{{0.15, 0.75, 0.7}, {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}},
	{{0.05, 0.75, 0.7}, {0.6, 0.3, 0.0}, {0.0, 0.0, 1.0}}};
static  PEXVertexRGBNormal square17[4] = {
	{{0.25, 0.65, 0.7}, {1.0, 0.0, 0.0}, {0.0, 0.0, 1.0}},
	{{0.35, 0.65, 0.7}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}},
	{{0.35, 0.75, 0.7}, {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}},
	{{0.25, 0.75, 0.7}, {0.6, 0.3, 0.0}, {0.0, 0.0, 1.0}}};
static  PEXVertexRGBNormal square18[4] = {
	{{0.45, 0.65, 0.0}, {1.0, 0.0, 0.0}, {0.0, 0.0, 1.0}},
	{{0.55, 0.65, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}},
	{{0.55, 0.75, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}},
	{{0.45, 0.75, 0.0}, {0.6, 0.3, 0.0}, {0.0, 0.0, 1.0}}};
static  PEXVertexRGBNormal square19[4] = {
	{{0.65, 0.65, 0.0}, {1.0, 0.0, 0.0}, {0.0, 0.0, 1.0}},
	{{0.75, 0.65, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}},
	{{0.75, 0.75, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}},
	{{0.65, 0.75, 0.0}, {0.6, 0.3, 0.0}, {0.0, 0.0, 1.0}}};
static  PEXVertexRGBNormal square20[4] = {
	{{0.85, 0.65, 0.0}, {1.0, 0.0, 0.0}, {0.0, 0.0, 1.0}},
	{{0.95, 0.65, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}},
	{{0.95, 0.75, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}},
	{{0.85, 0.75, 0.0}, {0.6, 0.3, 0.0}, {0.0, 0.0, 1.0}}};
static  PEXVertexRGBNormal square21[4] = {
	{{0.05, 0.85, 0.7}, {1.0, 0.0, 0.0}, {0.0, 0.0, 1.0}},
	{{0.15, 0.85, 0.7}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}},
	{{0.15, 0.95, 0.7}, {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}},
	{{0.05, 0.95, 0.7}, {0.6, 0.3, 0.0}, {0.0, 0.0, 1.0}}};
static  PEXVertexRGBNormal square22[4] = {
	{{0.25, 0.85, 0.7}, {1.0, 0.0, 0.0}, {0.0, 0.0, 1.0}},
	{{0.35, 0.85, 0.7}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}},
	{{0.35, 0.95, 0.7}, {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}},
	{{0.25, 0.95, 0.7}, {0.6, 0.3, 0.0}, {0.0, 0.0, 1.0}}};
static  PEXVertexRGBNormal square23[4] = {
	{{0.45, 0.85, 0.7}, {1.0, 0.0, 0.0}, {0.0, 0.0, 1.0}},
	{{0.55, 0.85, 0.7}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}},
	{{0.55, 0.95, 0.7}, {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}},
	{{0.45, 0.95, 0.7}, {0.6, 0.3, 0.0}, {0.0, 0.0, 1.0}}};

void
outputCommands(dpy, resourceID, req_type)
    Display             *dpy;
    unsigned long       resourceID; 
    PEXOCRequestType    req_type;
{	
    PEXArrayOfVertex 	vertices;
    PEXColor		color;
    PEXReflectionAttributes	ra;
    PEXTableIndex	en[10], dis[10];
    XID			ltab;
    PEXLightEntry	entries[3];

    PEXSetInteriorStyle(dpy, resourceID, req_type, PEXInteriorStyleSolid);
    PEXSetBFInteriorStyle(dpy, resourceID, req_type, PEXInteriorStyleSolid);

    vertices.rgb_normal = square1;
    PEXFillAreaWithData(dpy, resourceID, req_type, PEXShapeConvex,
	False, PEXGANormal, PEXGANormal | PEXGAColor, PEXColorTypeRGB,
	(PEXFacetData *)&normal1, 4, vertices);

    vertices.rgb_normal = square2;
    PEXSetSurfaceInterpMethod(dpy, resourceID, req_type, PEXSurfaceInterpNone);
    PEXFillAreaWithData(dpy, resourceID, req_type, PEXShapeConvex,
	False, PEXGANormal, PEXGANormal | PEXGAColor, PEXColorTypeRGB,
	(PEXFacetData *)&normal1, 4, vertices);

    vertices.rgb_normal = square3;
    PEXSetReflectionModel(dpy, resourceID, req_type, PEXReflectionNone);
    PEXFillAreaWithData(dpy, resourceID, req_type, PEXShapeConvex,
	False, PEXGANormal, PEXGANormal | PEXGAColor, PEXColorTypeRGB,
	(PEXFacetData *)&normal1, 4, vertices);

    ltab = PEXCreateLookupTable(dpy, DefaultRootWindow(dpy), PEXLUTLight);

    entries[0].type = PEXLightAmbient;
    entries[0].color.type = PEXColorTypeRGB;
    entries[0].color.value.rgb.red = 0.0;
    entries[0].color.value.rgb.green = 0.2;
    entries[0].color.value.rgb.blue = 0.8;
    entries[1].type = PEXLightWCVector;
    entries[1].color.type = PEXColorTypeRGB;
    entries[1].color.value.rgb.red = 0.0;
    entries[1].color.value.rgb.green = 0.8;
    entries[1].color.value.rgb.blue = 0.2;
    entries[1].direction.x = 0.1;
    entries[1].direction.y = 0.1;
    entries[1].direction.z = 0.9;
    entries[2].type = PEXLightWCVector;
    entries[2].color.type = PEXColorTypeRGB;
    entries[2].color.value.rgb.red = 0.0;
    entries[2].color.value.rgb.green = 0.8;
    entries[2].color.value.rgb.blue = 0.2;
    entries[2].direction.x = 0.4;
    entries[2].direction.y = 0.1;
    entries[2].direction.z = 0.4;
    PEXSetTableEntries(dpy, ltab, 1, 3, PEXLUTLight, entries);

    ra.ambient = 0.8;
    ra.diffuse = 0.0;
    ra.specular = 0.0;
    ra.specular_conc = 0.0;
    ra.specular_color.type = PEXColorTypeRGB;

    en[0] = 1;
    en[1] = 2;
    en[2] = 3;
    PEXSetLightSourceState(dpy, resourceID, req_type, 3, en, 0 , dis);
    PEXSetFacetDistinguishFlag(dpy, resourceID, req_type, 1);

    vertices.rgb_normal = square4;
    PEXSetReflectionModel(dpy, resourceID, req_type, PEXReflectionAmbient);
    PEXSetReflectionAttributes(dpy, resourceID, req_type, &ra);
    PEXFillAreaWithData(dpy, resourceID, req_type, PEXShapeConvex,
	False, PEXGANormal, PEXGANormal | PEXGAColor, PEXColorTypeRGB,
	(PEXFacetData *)&normal1, 4, vertices);

    vertices.rgb_normal = square5;
    PEXFillAreaWithData(dpy, resourceID, req_type, PEXShapeConvex,
	False, PEXGANormal, PEXGANormal | PEXGAColor, PEXColorTypeRGB,
	(PEXFacetData *)&normal2, 4, vertices);

    vertices.rgb_normal = square6;
    PEXSetBFSurfaceInterpMethod(dpy, resourceID, req_type, PEXSurfaceInterpNone);
    PEXSetBFReflectionModel(dpy, resourceID, req_type, PEXReflectionAmbient);
    PEXSetBFReflectionAttributes(dpy, resourceID, req_type, &ra);
    PEXFillAreaWithData(dpy, resourceID, req_type, PEXShapeConvex,
	False, PEXGANormal, PEXGANormal | PEXGAColor, PEXColorTypeRGB,
	(PEXFacetData *)&normal2, 4, vertices);

    vertices.rgb_normal = square7;
    PEXSetSurfaceInterpMethod(dpy, resourceID, req_type, PEXSurfaceInterpColor);
    PEXFillAreaWithData(dpy, resourceID, req_type, PEXShapeConvex,
	False, PEXGANormal, PEXGANormal | PEXGAColor, PEXColorTypeRGB,
	(PEXFacetData *)&normal1, 4, vertices);

    vertices.rgb_normal = square8;
    PEXFillAreaWithData(dpy, resourceID, req_type, PEXShapeConvex,
	False, PEXGANormal, PEXGANormal | PEXGAColor, PEXColorTypeRGB,
	(PEXFacetData *)&normal2, 4, vertices);

    vertices.rgb_normal = square9;
    PEXSetBFSurfaceInterpMethod(dpy, resourceID, req_type, PEXSurfaceInterpColor);
    PEXFillAreaWithData(dpy, resourceID, req_type, PEXShapeConvex,
	False, PEXGANormal, PEXGANormal | PEXGAColor, PEXColorTypeRGB,
	(PEXFacetData *)&normal2, 4, vertices);

    vertices.rgb_normal = square10;
    PEXFillAreaWithData(dpy, resourceID, req_type, PEXShapeConvex,
	False, PEXGANormal, PEXGANormal | PEXGAColor, PEXColorTypeRGB,
	(PEXFacetData *)&normal1, 4, vertices);

    vertices.rgb_normal = square11;
    PEXFillAreaWithData(dpy, resourceID, req_type, PEXShapeConvex,
	False, PEXGANormal, PEXGANormal | PEXGAColor, PEXColorTypeRGB,
	(PEXFacetData *)&normal1, 4, vertices);

    vertices.rgb_normal = square12;
    PEXFillAreaWithData(dpy, resourceID, req_type, PEXShapeConvex,
	False, PEXGANormal, PEXGANormal | PEXGAColor, PEXColorTypeRGB,
	(PEXFacetData *)&normal1, 4, vertices);

    vertices.rgb_normal = square13;
    PEXFillAreaWithData(dpy, resourceID, req_type, PEXShapeConvex,
	False, PEXGANormal, PEXGANormal | PEXGAColor, PEXColorTypeRGB,
	(PEXFacetData *)&normal1, 4, vertices);

    vertices.rgb_normal = square14;
    PEXFillAreaWithData(dpy, resourceID, req_type, PEXShapeConvex,
	False, PEXGANormal, PEXGANormal | PEXGAColor, PEXColorTypeRGB,
	(PEXFacetData *)&normal1, 4, vertices);

    vertices.rgb_normal = square15;
    PEXFillAreaWithData(dpy, resourceID, req_type, PEXShapeConvex,
	False, PEXGANormal, PEXGANormal | PEXGAColor, PEXColorTypeRGB,
	(PEXFacetData *)&normal1, 4, vertices);

    vertices.rgb_normal = square16;
    PEXFillAreaWithData(dpy, resourceID, req_type, PEXShapeConvex,
	False, PEXGANormal, PEXGANormal | PEXGAColor, PEXColorTypeRGB,
	(PEXFacetData *)&normal1, 4, vertices);

    vertices.rgb_normal = square17;
    PEXFillAreaWithData(dpy, resourceID, req_type, PEXShapeConvex,
	False, PEXGANormal, PEXGANormal | PEXGAColor, PEXColorTypeRGB,
	(PEXFacetData *)&normal1, 4, vertices);

    vertices.rgb_normal = square18;
    PEXFillAreaWithData(dpy, resourceID, req_type, PEXShapeConvex,
	False, PEXGANormal, PEXGANormal | PEXGAColor, PEXColorTypeRGB,
	(PEXFacetData *)&normal1, 4, vertices);

    vertices.rgb_normal = square19;
    PEXFillAreaWithData(dpy, resourceID, req_type, PEXShapeConvex,
	False, PEXGANormal, PEXGANormal | PEXGAColor, PEXColorTypeRGB,
	(PEXFacetData *)&normal1, 4, vertices);

    vertices.rgb_normal = square21;
    PEXFillAreaWithData(dpy, resourceID, req_type, PEXShapeConvex,
	False, PEXGANormal, PEXGANormal | PEXGAColor, PEXColorTypeRGB,
	(PEXFacetData *)&normal1, 4, vertices);

    vertices.rgb_normal = square22;
    PEXFillAreaWithData(dpy, resourceID, req_type, PEXShapeConvex,
	False, PEXGANormal, PEXGANormal | PEXGAColor, PEXColorTypeRGB,
	(PEXFacetData *)&normal1, 4, vertices);

    vertices.rgb_normal = square23;
    PEXFillAreaWithData(dpy, resourceID, req_type, PEXShapeConvex,
	False, PEXGANormal, PEXGANormal | PEXGAColor, PEXColorTypeRGB,
	(PEXFacetData *)&normal1, 4, vertices);

}
 
void
testproc(dpy,window,x,y,w,h)
    Display		*dpy;
    Window		window;
    int			x,y,w,h;
{
    image_testproc("shade", "lighting and shading", test_description,
        dpy,window,x,y,w,h);
} 

void
misc_setup(dpy, window, renderer)
Display		*dpy;
Window		window;
PEXRenderer	renderer;
{
}
