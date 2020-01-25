/* $XConsortium: ESproto.h,v 3.2 94/04/17 20:53:50 rws Exp $ */

/*
 * Copyright (c) 1992 by Sun Microsystems, Inc.
 * All Rights Reserved
 */

/***********************************************************

Copyright (c) 1989, 1990  X Consortium

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
X CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of the X Consortium shall not be
used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization from the X Consortium.

Copyright 1989, 1990 by Sun Microsystems, Inc. 

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the name of Sun Microsystems,
not be used in advertising or publicity pertaining to distribution of 
the software without specific, written prior permission.  

SUN MICROSYSTEMS DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, 
INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT 
SHALL SUN MICROSYSTEMS BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL 
DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/

/* Definitions for the PEX used by server and c bindings */

/*
 * This packet-construction scheme makes the following assumptions:
 *
 * 1. The compiler is able to generate code which addresses one- and two-byte
 * quantities.  In the worst case, this would be done with bit-fields.  If 
 * bit-fields are used it may be necessary to reorder the request fields in
 * this file, depending on the order in which the machine assigns bit fields
 * to machine words.  There may also be a problem with sign extension, as K+R 
 * specify that bitfields are always unsigned.
 *
 * 2. 2- and 4-byte fields in packet structures must be ordered by hand such 
 * that they are naturally-aligned, so that no compiler will ever insert 
 * padding bytes.
 *
 * 3. All packets are hand-padded to a multiple of 4 bytes, for the same reason.
 */

#ifndef ESPROTO_H
#define ESPROTO_H

/* In the following typedefs, comments appear that say "LISTof Foo( numItems )",
 * "CLIST of Foo()", and "SINGLE Foo()".   These are used when the protocol 
 * specifies that a request or reply contains a variable length list of 
 * (possibly variable types of) objects.
 *
 * A LISTof list is one for which we have already been given the length.
 * The items in the list are of type "Foo". The number of items in the list
 * appears parenthetically after the type.  ("numItems" in our example.)
 * Any other information needed to parse the list is also passed in the
 * parentheses. (E.g., "tableType" in a list of table entries.)
 *
 * A CLISTof list is the same, except that the first 4 bytes of the list
 * indicate the number of items in the list.  The length may need to be
 * byte-swapped.
 *
 * A SINGLE item of an indeterminate length is indicated in the same
 * manner.  (E.g., a "SINGLE TableEntry()".) Any other information
 * needed to parse the item is also passed in the parentheses.
 * (E.g., "itemMask" in a set of pipeline context attributes.)
 *
 * If no information is given in the parentheses, then the size is
 * implicit.
 *
 * Variable length padding is noted with a comment, with the number
 * of bytes of padding required as calculated from the value in
 * the parentheses.  (number of bytes of padding = n?(3-((n-1)%4):0 , where
 * n is the parenthetical value.)
 */

#include "X11/PEX5/ESpex.h"
#include "X11/PEX5/ESprotost.h"
#include "X11/PEX5/PEXproto.h"

/*****************************************************************
 * Output Commands
 *****************************************************************/

typedef struct {
    pexElementInfo	head;
    esColorSpecifier	colorSpec;
} esMarkerColor;

typedef esMarkerColor esTextColor;
typedef esMarkerColor esLineColor;
typedef esMarkerColor esSurfaceColor;
typedef esMarkerColor esBfSurfaceColor;
typedef esMarkerColor esSurfaceEdgeColor;

typedef struct {
    pexElementInfo	head;
    esReflectionAttr	reflectionAttr;
} esSurfaceReflAttr;
typedef esSurfaceReflAttr esBfSurfaceReflAttr;

typedef struct {
    pexElementInfo	head;
    CARD16		numEnable B16;
    CARD16		numDisable B16;
    CARD32              enable B32;
    CARD32              disable B32;
} esLightState;

typedef struct {
    pexElementInfo	head;
    /* LISTof esName() */
} esAddToNameSet;
typedef esAddToNameSet esRemoveFromNameSet;

typedef struct {
    pexElementInfo	head;
    CARD16		numElements B16;
    CARD16		unused B16;
    CARD32              data[1];
} esApplicationData;

typedef struct {
    pexElementInfo	head;
    pexCoord3D		origin;
    pexVector3D		vector1;
    pexVector3D		vector2;
    pexString            string;
} esText;

typedef struct {
    pexElementInfo	head;
    pexCoord2D		origin;
    pexString           string;
} esText2D;

typedef struct {
    pexElementInfo	head;
    pexCoord3D		origin;
    pexCoord3D		offset;
    pexString           string;
} esAnnotationText;

typedef struct {
    pexElementInfo	head;
    pexCoord2D		origin;
    pexCoord2D		offset;
    pexString           string;
} esAnnotationText2D;

typedef struct _esSphere
{
    pexElementInfo	head;
    CARD32		gdpId B32;
    CARD32		numPoints B32;
    CARD32		numBytes B32;
    pexColorType	colorType B16;
    CARD16		attr B16;
    CARD32              n;
    /* SINGLE sphere (radius (opt), color (opt), Coord3D ) */
} esSphere;

typedef struct _esUserCircle3
{
    pexElementInfo	head;
    CARD32		gdpId B32;
    CARD32		numPoints B32;
    CARD32		numBytes B32;
    pexCoord3D          center;
    pexCoord3D          dir[2];
    PEXFLOAT               radius;
} esUserCircle3;

typedef struct _esCircle3
{
    esUserCircle3       userdata;
    pexFillArea         fillareadata;  /* Added by ddpex StoreElement */
    pexCoord3D          vecs[1];
} esCircle3;

typedef struct _esUserArc3
{
    pexElementInfo	head;
    CARD32		gdpId B32;
    CARD32		numPoints B32;
    CARD32		numBytes B32;
    pexCoord3D          center;
    pexCoord3D          dir[2];
    PEXFLOAT               radius;
    PEXFLOAT               st_ang;    /* Starting angle, in radians */
    PEXFLOAT               end_ang;   /* Ending angle, in radians. */
} esUserArc3;

typedef struct _esArc3
{
    esUserArc3          userdata;
    pexPolyline         polylinedata;  /* Added by ddpex StoreElement */
    pexCoord3D          vecs[1];
} esArc3;

typedef struct _esPoints3D
{
    pexElementInfo	head;
    CARD32		gdpId B32;
    CARD32		numPoints B32;
    CARD32		numBytes B32;
    CARD32              data1;
    CARD32              data2;
} esPoints3D;

typedef struct _esSphereRadius
{
    pexElementInfo	head;
    CARD32		gseId B32;
    CARD16		numElements B16;
    CARD16		unused B16;
    PEXFLOAT               radius;
} esSphereRadius;

typedef struct _esSpherePrecision
{
    pexElementInfo	head;
    CARD32		gseId B32;
    CARD16		numElements B16;
    CARD16		unused B16;
    CARD32              precision;
} esSpherePrecision;

typedef struct _esFillAreaTolerance
{
    pexElementInfo	head;
    CARD32		gseId B32;
    CARD16		numElements B16;
    CARD16		unused B16;
    PEXFLOAT               tolerance;
} esFillareaTolerance;

typedef struct _esFrontBackDistinguish_
{
    pexElementInfo	head;
    CARD32		gseId B32;
    CARD16		numElements B16;
    CARD16		unused B16;
    CARD32              value;
} esFrontBackDistinguish;

typedef struct _esPolylineQuality
{
    pexElementInfo	head;
    CARD32		gseId B32;
    CARD16		numElements B16;
    CARD16		unused B16;
    CARD32              quality;
} esPolylineQuality;

typedef struct _esLinePatternMask
{
    pexElementInfo	head;
    CARD32		gseId B32;
    CARD16		numElements B16;
    CARD16		unused B16;
    CARD32              length;
    CARD32              pattern;
} esLinePatternMask;


typedef struct _esEdgePatternMask
{
    pexElementInfo	head;
    CARD32		gseId B32;
    CARD16		numElements B16;
    CARD16		unused B16;
    CARD32              length;
    CARD32              pattern;
} esEdgePatternMask;

typedef struct _esTransparent
{
    pexElementInfo	head;
    CARD32		gseId B32;
    CARD16		numElements B16;
    CARD16		unused B16;
    PEXFLOAT               transparent;
} esTransparent;

typedef struct _esHighlightingMode
{
    pexElementInfo	head;
    CARD32		gseId B32;
    CARD16		numElements B16;
    CARD16		unused B16;
    CARD32              mode;
} esHighlightingMode;

typedef struct _esHighlightingColor
{
    pexElementInfo	head;
    CARD32		gseId B32;
    CARD16		numElements B16;
    CARD16		unused1 B16;
    esColorSpecifier   colorSpec;
} esHighlightingColor;

typedef struct _esHighlightingColorNode
{
    pexElementInfo	head;
    CARD32		gseId B32;
    CARD16		numElements B16;
    CARD16		unused1 B16;
    /* note this is the phigs pgcolr definition */
    CARD32              type;
    union {
	CARD32          ind;
	struct {
	    PEXFLOAT       x;
	    PEXFLOAT       y;
	    PEXFLOAT       z;
	} general;
    } val;
} esHighlightingColorNode;

typedef struct _esInformation
{
    pexElementInfo	head;
    CARD32		gseId B32;
    CARD16		numElements B16;
    CARD16		unused B16;
    CARD32              type;
    union {
	INT32  unused;
	INT32  id;
    } rec;
} esInformation;

typedef struct _esCylinder
{
    pexElementInfo	head;
    CARD32		gdpId B32;
    CARD32		numPoints B32;
    CARD32		numBytes B32;
    pexColorType	colorType B16;
    CARD16		attr B16;
    CARD32              m;
    /* LISTof connected cylinders */
} esCylinder;

typedef struct _esCylinderRadius
{
    pexElementInfo	head;
    CARD32		gseId B32;
    CARD16		numElements B16;
    CARD16		unused B16;
    PEXFLOAT               radius;
} esCylinderRadius;

typedef struct _esCylinderPrecision
{
    pexElementInfo	head;
    CARD32		gseId B32;
    CARD16		numElements B16;
    CARD16		unused B16;
    CARD32              precision;
} esCylinderPrecision;

typedef struct _esStereoViewIndices
{
    pexElementInfo	head;
    CARD32		gseId B32;
    CARD16		numElements B16;
    CARD16		unused B16;
    CARD32              mono;
    CARD32              left;
    CARD32              right;
} esStereoViewIndices;

typedef struct {
    BYTE		type;			/* X_Reply */
    CARD8		what;			/* unused */
    CARD16		sequenceNumber	B16;
    CARD32		length B32;		/* 0 */
    pexDynamicType	viewRep;
    pexDynamicType	markerBundle;
    pexDynamicType	textBundle;
    pexDynamicType	lineBundle;
    pexDynamicType	interiorBundle;
    pexDynamicType	edgeBundle;
    pexDynamicType	colorTable;
    pexDynamicType	patternTable;
    pexDynamicType	wksTransform;
    pexDynamicType	highlightFilter;
    pexDynamicType	invisibilityFilter;
    pexDynamicType	HlhsrMode;
    pexDynamicType	structureModify;
    pexDynamicType	postStructure;
    pexDynamicType	unpostStructure;
    pexDynamicType	deleteStructure;
    pexDynamicType	referenceModify;
    pexDynamicType	bufferModify;
    pexDynamicType	lightTable;
    pexDynamicType	depthCueTable;
    pexDynamicType	colorApproxTable;
    pexDynamicType	dataMappingTable;
    CARD8		pad[2];
    } espexGetDynamicsReply;

typedef pexMarkerBundleIndex pexesDataMapMethodIndex;
typedef pexMarkerBundleIndex pexesBfDataMapMethodIndex;

typedef struct {
    BYTE	type;			/* X_Reply */
    CARD8	what;			/* unused */
    CARD16	sequenceNumber	B16;
    CARD32	length B32;		/* 4 + 76*fp/4 */
    CARD16	dmapUpdate B16;		/* Pending, NotPending */ 
    BYTE	pad[22];
    /* SINGLE pexesDataMappingMethod() 	requested */
    /* SINGLE pexesDataMappingMethod() 	current */
    } pexGetDmapRepReply;

typedef struct {
    CARD8 		reqType;
    CARD8 		opcode;
    CARD16 		length B16;	/* 3 */
    pexEnumTypeIndex	fpFormat B16;
    pexTableIndex	index B16;
    pexPhigsWks		wks B32;
} pexGetDmapRepReq;

typedef struct {
    CARD8		reqType;
    CARD8		opcode;
    CARD16		length B16;	/* 43 */
    pexEnumTypeIndex	fpFormat B16;
    CARD16		unused B16;
    pexPhigsWks		wks B32;
    pexTableIndex	index B16;
    /* Data mapping method */
} pexSetDmapRepReq;

typedef struct {
    pexElementInfo      head;
    esDataMappingRecord	rec;
} pexesDataMappingMethod;
typedef pexesDataMappingMethod pexesBfDataMappingMethod;

/***************************************************************************/
 /* The following are almost duplicates from PEX.h only these have the */
 /* added fields for data mapping */

typedef struct {
    pexElementInfo	head;
    CARD16		shape B16;
    pexSwitch		ignoreEdges;
    CARD8		unused;
    pexColorType	colorType B16;
    pexBitmaskShort	facetAttribs B16;
    pexBitmaskShort	vertexAttribs B16;
    CARD16		unused2 B16;
    CARD32		fdataCount B32;
    CARD32		vdataCount B32;
    /* SINGLE Facet( facetAttribs, vertexAttribs, colorType ) */
} pexesExtFillArea;

typedef struct {
    pexElementInfo	head;
    CARD16		shape B16;
    pexSwitch		ignoreEdges;
    CARD8		contourHint;
    pexColorType	colorType B16;
    pexBitmaskShort	facetAttribs B16;
    pexBitmaskShort	vertexAttribs B16;
    CARD16		unused2 B16;
    CARD32		numLists B32;
    CARD32		fdataCount B32;
    CARD32		vdataCount B32;
    /* pexOptData( facetAttribs ) */
    /* LISTof CLISTof  pexVertex( numLists, vertexAttribs, colorType ) */
} pexesExtFillAreaSet;

typedef struct {
    pexElementInfo	head;
    pexColorType	colorType B16;
    pexBitmaskShort	facetAttribs B16;
    pexBitmaskShort	vertexAttribs B16;
    CARD16		unused B16;
    CARD32		numVertices B32;
    CARD32		fdataCount B32;
    CARD32		vdataCount B32;
    /* number of OptData is numVert - 2 */
    /* LISTof pexOptData( facetAttribs, colorType ) */
    /* LISTof pexVertex( numVertices, vertexAttribs, colorType ) */
} pexesTriangleStrip;

typedef struct {
    pexElementInfo	head;
    pexColorType	colorType B16;
    CARD16		mPts B16;
    CARD16		nPts B16;
    pexBitmaskShort	facetAttribs B16;
    pexBitmaskShort	vertexAttribs B16;
    CARD16		shape B16;
    CARD32		fdataCount B32;
    CARD32		vdataCount B32;
    /* actually, there are (mPts-1)*(nPts-1) opt data entries */
    /* LISTof pexOptData( facetAttribs, colorType ) */
    /* LISTof pexVertex( mPts, nPts, vertexAttribs, colorType ) */
} pexesQuadrilateralMesh;

typedef struct {
    pexElementInfo	head;
    CARD16		shape B16;
    pexColorType	colorType B16;
    CARD16		FAS_Attributes B16;
    CARD16		vertexAttributes B16;
    CARD16		edgeAttributes B16;
    CARD8		contourHint;
    pexSwitch		contourCountsFlag;
    CARD16		numFAS B16;
    CARD16		numVertices B16;
    CARD16		numEdges B16;
    CARD16		numContours B16;
    CARD32		fdataCount B32;
    CARD32		vdataCount B32;
    /* LISTof OPT_DATA( numFAS ) */
    /* LISTof pexVertex( numVertices ) */
    /* LISTof CARD8( numEdges ) */
    /* pad( numEdges ) */
    /* LISTof CLISTof CLISTof CARD16( numFAS, numContours, numEdges ) */
    /* pad */
} pexesSOFAS;


/* Escape to request double buffering */

typedef struct {
    Drawable    	drawable;
    unsigned long 	bufferMode;
} esEscapeDblBuffer;

/* Escape to swap buffers */

typedef struct {
    Drawable 		drawable;
} esEscapeSwapBuffer;

/* Escape to inquire the update action at swap. */

typedef struct {
    Drawable	drawable;
} esEscapeSwapBufferContent;

typedef struct {
    unsigned char	type;
    unsigned char	unused;
    unsigned short	sequenceNum;
    unsigned long	length;
    unsigned long	escapeId;
    unsigned long	content;
} esEscapeSwapBufferContentReply;

#endif /* ESPROTO_H */
