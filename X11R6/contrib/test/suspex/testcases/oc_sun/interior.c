/* $XConsortium: interior.c,v 1.0 93/11/22 12:49:58 rws Exp $ */

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
Interior Attributes + model clip/transform, HLHSR Tests:\n\n\
FillareaWithData is used in this test.\n\
(From bottom to top)\n\
1st Row: 1. default attributes - white and hollow area\n\
         2. change interior style to solid\n\
         3. change interior color to green\n\
         4. add red solid edge\n\
         5. change edge to green and dashed\n\
2st Row: 1. make edge thick and changed interior style to hatch\n\
         2. change interior color to red\n\
         3. change bf attributes (no effect since face distinguishing is OFF\n\
         4. bf attributes in effect, interior is solid and green\n\
         5. change bf color to blue, area now in blue\n\
3st Row: 1. front face again, red hatch interior, not culled\n\
         2. Invisible back-facing area got culled\n\
         3. model clipped, leaving only upper right square\n\
         4. model clipped (2D clip volume), leaving only upper right triangle\n\
         5. used bundled attributes, now a red solid with thick red solid edge\n\
4st Row: 1. interior color is green, the upper area on top of lower one\n\
         2. with HLHSR, the lower area now on top of upper one\n\
         3. area translated 0.1 unit to the right by global transformation\n\
         4. area translated 0.1 unit to the right by global transformation(2D)\n\
5st Row:\n\
         1. area translated 0.17 unit the right by global and local transform\n\
         2. area translated 0.1 unit to the right by global and local transform\n\
         3. area translated 0.17 unit the right by global and local transform\n\
";

static  PEXVector normal1 = {0.0, 0.0, 1.0};
static  PEXVector normal2 = {0.0, 0.0, -1.0};
static  PEXCoord square1[4] = {
	{0.05, 0.05, 0.0},
	{0.15, 0.05, 0.0},
	{0.15, 0.15, 0.0},
	{0.05, 0.15, 0.0}};
static  PEXCoord square2[4] = {
	{0.25, 0.05, 0.0},
	{0.35, 0.05, 0.0},
	{0.35, 0.15, 0.0},
	{0.25, 0.15, 0.0}};
static  PEXCoord square3[4] = {
	{0.45, 0.05, 0.0},
	{0.55, 0.05, 0.0},
	{0.55, 0.15, 0.0},
	{0.45, 0.15, 0.0}};
static  PEXCoord square4[4] = {
	{0.65, 0.05, 0.0},
	{0.75, 0.05, 0.0},
	{0.75, 0.15, 0.0},
	{0.65, 0.15, 0.0}};
static  PEXCoord square5[4] = {
	{0.85, 0.05, 0.0},
	{0.95, 0.05, 0.0},
	{0.95, 0.15, 0.0},
	{0.85, 0.15, 0.0}};
static  PEXCoord square6[4] = {
	{0.05, 0.25, 0.0},
	{0.15, 0.25, 0.0},
	{0.15, 0.35, 0.0},
	{0.05, 0.35, 0.0}};
static  PEXCoord square7[4] = {
	{0.25, 0.25, 0.0},
	{0.35, 0.25, 0.0},
	{0.35, 0.35, 0.0},
	{0.25, 0.35, 0.0}};
static  PEXCoord square8[4] = {
	{0.45, 0.25, 0.0},
	{0.55, 0.25, 0.0},
	{0.55, 0.35, 0.0},
	{0.45, 0.35, 0.0}};
static  PEXCoord square9[4] = {
	{0.65, 0.25, 0.0},
	{0.75, 0.25, 0.0},
	{0.75, 0.35, 0.0},
	{0.65, 0.35, 0.0}};
static  PEXCoord square10[4] = {
	{0.85, 0.25, 0.0},
	{0.95, 0.25, 0.0},
	{0.95, 0.35, 0.0},
	{0.85, 0.35, 0.0}};
static  PEXCoord square11[4] = {
	{0.05, 0.45, 0.0},
	{0.15, 0.45, 0.0},
	{0.15, 0.55, 0.0},
	{0.05, 0.55, 0.0}};
static  PEXCoord square12[4] = {
	{0.25, 0.45, 0.0},
	{0.35, 0.45, 0.0},
	{0.35, 0.55, 0.0},
	{0.25, 0.55, 0.0}};
static  PEXCoord square13[4] = {
	{0.45, 0.45, 0.0},
	{0.55, 0.45, 0.0},
	{0.55, 0.55, 0.0},
	{0.45, 0.55, 0.0}};
static  PEXCoord square14[4] = {
	{0.65, 0.45, 0.0},
	{0.75, 0.45, 0.0},
	{0.75, 0.55, 0.0},
	{0.65, 0.55, 0.0}};
static  PEXCoord square15[4] = {
	{0.85, 0.45, 0.0},
	{0.95, 0.45, 0.0},
	{0.95, 0.55, 0.0},
	{0.85, 0.55, 0.0}};
static  PEXCoord square16a[4] = {
	{0.05, 0.65, 0.7},
	{0.15, 0.65, 0.7},
	{0.15, 0.75, 0.7},
	{0.05, 0.75, 0.7}};
static  PEXCoord square16b[4] = {
	{0.10, 0.70, 0.3},
	{0.20, 0.70, 0.3},
	{0.20, 0.80, 0.3},
	{0.10, 0.80, 0.3}};
static  PEXCoord square17a[4] = {
	{0.25, 0.65, 0.7},
	{0.35, 0.65, 0.7},
	{0.35, 0.75, 0.7},
	{0.25, 0.75, 0.7}};
static  PEXCoord square17b[4] = {
	{0.30, 0.70, 0.3},
	{0.40, 0.70, 0.3},
	{0.40, 0.80, 0.3},
	{0.30, 0.80, 0.3}};
static  PEXCoord square18[4] = {
	{0.45, 0.65, 0.0},
	{0.55, 0.65, 0.0},
	{0.55, 0.75, 0.0},
	{0.45, 0.75, 0.0}};
static  PEXCoord square19[4] = {
	{0.65, 0.65, 0.0},
	{0.75, 0.65, 0.0},
	{0.75, 0.75, 0.0},
	{0.65, 0.75, 0.0}};
static  PEXCoord square20[4] = {
	{0.85, 0.65, 0.0},
	{0.95, 0.65, 0.0},
	{0.95, 0.75, 0.0},
	{0.85, 0.75, 0.0}};
static  PEXCoord square21[4] = {
	{0.05, 0.85, 0.7},
	{0.15, 0.85, 0.7},
	{0.15, 0.95, 0.7},
	{0.05, 0.95, 0.7}};
static  PEXCoord square22[4] = {
	{0.25, 0.85, 0.7},
	{0.35, 0.85, 0.7},
	{0.35, 0.95, 0.7},
	{0.25, 0.95, 0.7}};
static  PEXCoord square23[4] = {
	{0.45, 0.85, 0.7},
	{0.55, 0.85, 0.7},
	{0.55, 0.95, 0.7},
	{0.45, 0.95, 0.7}};
static  PEXVertexNormal square32[4] = {
	{{0.25, 0.55, 0.0}, {0.0, 0.0, 1.0}},
	{{0.35, 0.55, 0.0}, {0.0, 0.0, 1.0}},
	{{0.35, 0.65, 0.0}, {0.0, 0.0, 1.0}},
	{{0.25, 0.65, 0.0}, {0.0, 0.0, 1.0}}};

void
outputCommands(dpy, resourceID, req_type)
    Display             *dpy;
    unsigned long       resourceID; 
    PEXOCRequestType    req_type;
{	
    PEXArrayOfVertex 	vertices;
    PEXColor		color;
    PEXHalfSpace	hs[2];
    PEXHalfSpace2D	hs2[1];
    PEXMatrix3x3	matrix2d;
    PEXMatrix		matrix;
    PEXVector		vector;
    PEXVector2D		vector2d;

    vertices.no_data = square1;
    PEXFillAreaWithData(dpy, resourceID, req_type, PEXShapeConvex,
	False, PEXGANormal, PEXGANone, PEXColorTypeRGB,
	(PEXFacetData *)&normal1, 4, vertices);

    vertices.no_data = square2;
    PEXSetInteriorStyle(dpy, resourceID, req_type, PEXInteriorStyleSolid);
    PEXFillAreaWithData(dpy, resourceID, req_type, PEXShapeConvex,
	False, PEXGANormal, PEXGANone, PEXColorTypeRGB,
	(PEXFacetData *)&normal1, 4, vertices);

    vertices.no_data = square3;
    color.indexed.index = 3;
    PEXSetSurfaceColor(dpy, resourceID, req_type, PEXColorTypeIndexed, &color);
    PEXFillAreaWithData(dpy, resourceID, req_type, PEXShapeConvex,
	False, PEXGANormal, PEXGANone, PEXColorTypeRGB,
	(PEXFacetData *)&normal1, 4, vertices);

    vertices.no_data = square4;
    color.indexed.index = 2;
    PEXSetSurfaceEdgeColor(dpy, resourceID, req_type, PEXColorTypeIndexed, &color);
    PEXSetSurfaceEdgeFlag(dpy, resourceID, req_type, PEXOn);
    PEXFillAreaWithData(dpy, resourceID, req_type, PEXShapeConvex,
	False, PEXGANormal, PEXGANone, PEXColorTypeRGB,
	(PEXFacetData *)&normal1, 4, vertices);

    vertices.no_data = square5;
    PEXSetSurfaceEdgeColorIndex(dpy, resourceID, req_type, 5);
    PEXSetSurfaceEdgeType(dpy, resourceID, req_type, PEXSurfaceEdgeDashed);
    PEXFillAreaWithData(dpy, resourceID, req_type, PEXShapeConvex,
	False, PEXGANormal, PEXGANone, PEXColorTypeRGB,
	(PEXFacetData *)&normal1, 4, vertices);

    vertices.no_data = square6;
    PEXSetSurfaceEdgeWidth(dpy, resourceID, req_type, 3.0);
    PEXSetInteriorStyle(dpy, resourceID, req_type, PEXInteriorStyleHatch);
    PEXFillAreaWithData(dpy, resourceID, req_type, PEXShapeConvex,
	False, PEXGANormal, PEXGANone, PEXColorTypeRGB,
	(PEXFacetData *)&normal1, 4, vertices);

    vertices.no_data = square7;
    PEXSetInteriorStyleIndex(dpy, resourceID, req_type, (0x8500 | 102));
    PEXSetSurfaceColorIndex(dpy, resourceID, req_type, 2);
    PEXFillAreaWithData(dpy, resourceID, req_type, PEXShapeConvex,
	False, PEXGANormal, PEXGANone, PEXColorTypeRGB,
	(PEXFacetData *)&normal1, 4, vertices);

    vertices.no_data = square8;
    PEXSetBFInteriorStyle(dpy, resourceID, req_type, PEXInteriorStyleSolid);
    color.indexed.index = 3;
    PEXSetBFSurfaceColor(dpy, resourceID, req_type, PEXColorTypeIndexed, &color);
    PEXFillAreaWithData(dpy, resourceID, req_type, PEXShapeConvex,
	False, PEXGANormal, PEXGANone, PEXColorTypeRGB,
	(PEXFacetData *)&normal2, 4, vertices);

    vertices.no_data = square9;
    PEXSetFacetDistinguishFlag(dpy, resourceID, req_type, 1);
    PEXFillAreaWithData(dpy, resourceID, req_type, PEXShapeConvex,
	False, PEXGANormal, PEXGANone, PEXColorTypeRGB,
	(PEXFacetData *)&normal2, 4, vertices);

    vertices.no_data = square10;
    PEXSetBFInteriorStyle(dpy, resourceID, req_type, PEXInteriorStyleHatch);
    PEXSetBFInteriorStyleIndex(dpy, resourceID, req_type, (0x8500 | 103));
    PEXSetBFSurfaceColorIndex(dpy, resourceID, req_type, 4);
    PEXFillAreaWithData(dpy, resourceID, req_type, PEXShapeConvex,
	False, PEXGANormal, PEXGANone, PEXColorTypeRGB,
	(PEXFacetData *)&normal2, 4, vertices);

    vertices.no_data = square11;
    PEXSetFacetCullingMode(dpy, resourceID, req_type, PEXBackFaces);
    PEXFillAreaWithData(dpy, resourceID, req_type, PEXShapeConvex,
	False, PEXGANormal, PEXGANone, PEXColorTypeRGB,
	(PEXFacetData *)&normal1, 4, vertices);

    vertices.no_data = square12;
    PEXFillAreaWithData(dpy, resourceID, req_type, PEXShapeConvex,
	False, PEXGANormal, PEXGANone, PEXColorTypeRGB,
	(PEXFacetData *)&normal2, 4, vertices);

    vertices.no_data = square13;
    PEXSetModelClipFlag(dpy, resourceID, req_type, PEXClip);
    hs[0].point.x = 0.5;
    hs[0].point.y = 0.5;
    hs[0].point.z = 0.0;
    hs[0].vector.x = 1.0;
    hs[0].vector.y = 0.0;
    hs[0].vector.z = 0.0;
    hs[1].point.x = 0.5;
    hs[1].point.y = 0.5;
    hs[1].point.z = 0.0;
    hs[1].vector.x = 0.0;
    hs[1].vector.y = 1.0;
    hs[1].vector.z = 0.0;
    PEXSetModelClipVolume(dpy, resourceID, req_type, PEXModelClipReplace,
	2, hs);
    PEXFillAreaWithData(dpy, resourceID, req_type, PEXShapeConvex,
	False, PEXGANormal, PEXGANone, PEXColorTypeRGB,
	(PEXFacetData *)&normal1, 4, vertices);

    vertices.no_data = square14;
    hs2[0].point.x = 0.65;
    hs2[0].point.y = 0.5;
    hs2[0].vector.x = -1.0;
    hs2[0].vector.y = 1.0;
    PEXSetModelClipVolume2D(dpy, resourceID, req_type, PEXModelClipIntersection,
	1, hs2);
    PEXFillAreaWithData(dpy, resourceID, req_type, PEXShapeConvex,
	False, PEXGANormal, PEXGANone, PEXColorTypeRGB,
	(PEXFacetData *)&normal1, 4, vertices);

    vertices.no_data = square15;
    PEXSetModelClipFlag(dpy, resourceID, req_type, PEXNoClip);
    SET_INDIVIDUAL_ASF(PEXASFSurfaceColor, PEXBundled);
    SET_INDIVIDUAL_ASF(PEXASFSurfaceEdgeColor, PEXBundled);
    SET_INDIVIDUAL_ASF(PEXASFSurfaceEdgeWidth, PEXBundled);
    SET_INDIVIDUAL_ASF(PEXASFSurfaceEdgeType, PEXBundled);
    SET_INDIVIDUAL_ASF(PEXASFInteriorStyle, PEXBundled);
    SET_INDIVIDUAL_ASF(PEXASFBFInteriorStyle, PEXBundled);
    PEXSetInteriorBundleIndex(dpy, resourceID, req_type, 1);
    PEXSetEdgeBundleIndex(dpy, resourceID, req_type, 1);
    PEXFillAreaWithData(dpy, resourceID, req_type, PEXShapeConvex,
	False, PEXGANormal, PEXGANone, PEXColorTypeRGB,
	(PEXFacetData *)&normal1, 4, vertices);

    PEXSetHLHSRID(dpy, resourceID, req_type, PEXHLHSRIDDisable);
    vertices.no_data = square16a;
    color.indexed.index = 3;
    PEXSetSurfaceColor(dpy, resourceID, req_type, PEXColorTypeIndexed, &color);
    PEXSetInteriorStyle(dpy, resourceID, req_type, PEXInteriorStyleSolid);
    SET_INDIVIDUAL_ASF(PEXASFSurfaceColor, PEXIndividual);
    SET_INDIVIDUAL_ASF(PEXASFInteriorStyle, PEXIndividual);
    PEXFillAreaWithData(dpy, resourceID, req_type, PEXShapeConvex,
	False, PEXGANormal, PEXGANone, PEXColorTypeRGB,
	(PEXFacetData *)&normal1, 4, vertices);
    color.indexed.index = 5;
    PEXSetSurfaceColor(dpy, resourceID, req_type, PEXColorTypeIndexed, &color);
    vertices.no_data = square16b;
    PEXFillAreaWithData(dpy, resourceID, req_type, PEXShapeConvex,
	False, PEXGANormal, PEXGANone, PEXColorTypeRGB,
	(PEXFacetData *)&normal1, 4, vertices);

    vertices.no_data = square17a;
    PEXSetHLHSRID(dpy, resourceID, req_type, PEXHLHSRIDEnable);
    color.indexed.index = 3;
    PEXSetSurfaceColor(dpy, resourceID, req_type, PEXColorTypeIndexed, &color);
    PEXFillAreaWithData(dpy, resourceID, req_type, PEXShapeConvex,
	False, PEXGANormal, PEXGANone, PEXColorTypeRGB,
	(PEXFacetData *)&normal1, 4, vertices);
    color.indexed.index = 5;
    PEXSetSurfaceColor(dpy, resourceID, req_type, PEXColorTypeIndexed, &color);
    vertices.no_data = square17b;
    PEXFillAreaWithData(dpy, resourceID, req_type, PEXShapeConvex,
	False, PEXGANormal, PEXGANone, PEXColorTypeRGB,
	(PEXFacetData *)&normal1, 4, vertices);

    matrix[0][0] = 1.0;
    matrix[0][1] = 0.0;
    matrix[0][2] = 0.0;
    matrix[0][3] = 0.0;
    matrix[1][0] = 0.0;
    matrix[1][1] = 1.0;
    matrix[1][2] = 0.0;
    matrix[1][3] = 0.0;
    matrix[2][0] = 0.0;
    matrix[2][1] = 0.0;
    matrix[2][2] = 1.0;
    matrix[2][3] = 0.0;
    matrix[3][0] = 0.0;
    matrix[3][1] = 0.0;
    matrix[3][2] = 0.0;
    matrix[3][3] = 1.0;

    matrix2d[0][0] = 1.0;
    matrix2d[0][1] = 0.0;
    matrix2d[0][2] = 0.0;
    matrix2d[1][0] = 0.0;
    matrix2d[1][1] = 1.0;
    matrix2d[1][2] = 0.0;
    matrix2d[2][0] = 0.0;
    matrix2d[2][1] = 0.0;
    matrix2d[2][2] = 1.0;

    vector.x = 0.0;
    vector.y = 0.0;
    vector.z = 0.0;

    vector2d.x = 0.0;
    vector2d.y = 0.0;

    vertices.no_data = square18;
    vector.x = 0.1;
    PEXTranslate(&vector, matrix);
    PEXSetGlobalTransform(dpy, resourceID, req_type, matrix);
    PEXFillAreaWithData(dpy, resourceID, req_type, PEXShapeConvex,
	False, PEXGANormal, PEXGANone, PEXColorTypeRGB,
	(PEXFacetData *)&normal1, 4, vertices);

    vertices.no_data = square19;
    vector2d.x = 0.1;
    PEXTranslate2D(&vector2d, matrix2d);
    PEXSetGlobalTransform2D(dpy, resourceID, req_type, matrix2d);
    PEXFillAreaWithData(dpy, resourceID, req_type, PEXShapeConvex,
	False, PEXGANormal, PEXGANone, PEXColorTypeRGB,
	(PEXFacetData *)&normal1, 4, vertices);

    vertices.no_data = square21;
    vector.x = 0.07;
    PEXTranslate(&vector, matrix);
    PEXSetLocalTransform(dpy, resourceID, req_type, PEXPreConcatenate, 
	matrix);
    PEXFillAreaWithData(dpy, resourceID, req_type, PEXShapeConvex,
	False, PEXGANormal, PEXGANone, PEXColorTypeRGB,
	(PEXFacetData *)&normal1, 4, vertices);

    vertices.no_data = square22;
    PEXSetLocalTransform(dpy, resourceID, req_type, PEXReplace, 
	identity_matrix);
    PEXFillAreaWithData(dpy, resourceID, req_type, PEXShapeConvex,
	False, PEXGANormal, PEXGANone, PEXColorTypeRGB,
	(PEXFacetData *)&normal1, 4, vertices);

    vertices.no_data = square23;
    PEXTranslate2D(&vector2d, matrix2d);
    PEXSetLocalTransform2D(dpy, resourceID, req_type, PEXPreConcatenate,
	matrix2d);
    PEXFillAreaWithData(dpy, resourceID, req_type, PEXShapeConvex,
	False, PEXGANormal, PEXGANone, PEXColorTypeRGB,
	(PEXFacetData *)&normal1, 4, vertices);

}

void
testproc(dpy,window,x,y,w,h)
    Display		*dpy;
    Window		window;
    int			x,y,w,h;
{
    image_testproc("interior", "attrs, model clip, z-buffer", test_description,
        dpy,window,x,y,w,h);
} 

void
misc_setup(dpy, window, renderer)
Display		*dpy;
Window		window;
PEXRenderer	renderer;
{
    PEXRendererAttributes ra;

    ra.hlhsr_mode = PEXHLHSRZBufferID;
    PEXChangeRenderer(dpy, renderer, PEXRAHLHSRMode, &ra);
}
