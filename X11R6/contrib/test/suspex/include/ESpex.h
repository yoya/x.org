/* $XConsortium: ESpex.h,v 3.2 94/04/17 20:53:50 rws Exp $ */

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


#ifndef ESPEX_H
#define ESPEX_H

#include "X11/PEX5/PEX.h"

/* Escape Type */
#define PEXTimedBufferSwap      -1

/* Escape to request double buffering */
#define ES_ESCAPE_ET_DBLBUFFER		0x8401
#define ES_ESCAPE_ETM_DBLBUFFER         "ES_ESCAPE_DBLBUFFER"
#define ES_ESCAPE_DBLBUFFER		0x80040001

#define ES_RENDERER_SINGLEBUFFER	0
#define ES_RENDERER_DBLBUFFER		1

/* Escape to swap buffers */
#define ES_ESCAPE_ET_SWAPBUFFER         0x8402
#define ES_ESCAPE_ETM_SWAPBUFFER        "ES_ESCAPE_SWAPBUFFER"
#define ES_ESCAPE_SWAPBUFFER		0x80040002

/* Escape to inquire the update action at swap */
#define ES_ESCAPE_ET_SWAPBUFFERCONTENT  0x8403
#define ES_ESCAPE_ETM_SWAPBUFFERCONTENT "ES_ESCAPE_SWAPBUFFERCONTENT"
#define ES_ESCAPE_SWAPBUFFERCONTENT     0x80040003

#define ES_DB_SWAP_CONTENT_UNDEFINED	0
#define ES_DB_SWAP_CONTENT_CLEAR_TO_BACKGROUND	1
#define ES_DB_SWAP_CONTENT_UNCHANGED	2
#define ES_DB_SWAP_CONTENT_FRONTBUFFER	3


/* These defines must match those in esgdp.h */
#define ESHlhsrHQAlgorithm      0xFFFF8402
#define ESHlhsrModeMultipassST  0xFFFF8403
#define ESHlhsrModeMultipassSTL 0xFFFF8404
#define ESHlhsrModeMultipassSL  0xFFFF8405
#define ESHlhsrModeMultipassSLT 0xFFFF8406
#define ESHlhsrModeMultipassT   0xFFFF8407
#define ESHlhsrModeMultipassTS  0xFFFF8408
#define ESHlhsrModeMultipassTSL 0xFFFF8409
#define ESHlhsrModeMultipassTL  0xFFFF840A
#define ESHlhsrModeMultipassTLS 0xFFFF840B
#define ESHlhsrModeMultipassLS  0xFFFF840C
#define ESHlhsrModeMultipassLST 0xFFFF840D
#define ESHlhsrModeMultipassLT  0xFFFF840E
#define ESHlhsrModeMultipassLTS 0xFFFF840F
#define ESHlhsrModeZBufferId    0xFFFF8410

/* The following constant is used in es_ddInfo.h and should be incremented
   every time we add another HLHSR mode. */
#define ESNumOfHlhsrs           17

/* PromptEchoType */
#define ESEchoItemInPrimitive  -1


/* Pipeline Context */
#define ESPEXPCSphereRadius             65
#define ESPEXPCSpherePrecision          66
#define ESPEXPCCylinderRadius           67
#define ESPEXPCCylinderPrecision        68
#define ESPEXPCStereoViewIndices        69
#define ESPEXPCFillareaTolerance        70
#define ESPEXPCFrontBackDistinguish     71
#define ESPEXPCPolylineQuality          72
#define ESPEXPCLinePatternMask          73
#define ESPEXPCEdgePatternMask          74
#define ESPEXPCTransparent              75
#define ESPEXPCHighlightingMode         76
#define ESPEXPCHighlightingColour       77
#define	ESPEXPCDataMappingMethod	78
#define	ESPEXPCBfDataMappingMethod	79
#define ESPEXPCDataMappingIndex		80
#define ESPEXPCBfDataMappingIndex	81


/* GDP and GSE output sub-cmds */
/* These defines must match those in esgdp.h */
#define ESGseSphereRadius		0x80040001
#define ESGseSphereDivisions	        0x80040002
#define ESGseCylinderRadius		0x80040003
#define ESGseCylinderDivisions	        0x80040004
#define ESGseStereoViewIndices	        0x80040005
#define ESGseFillareaTolerance   	0x80040006
#define ESGseFrontBackDistinguish       0x80040007
#define ESGsePolylineQuality            0x80040008
#define ESGseLinePatternMask            0x80040009
#define ESGseEdgePatternMask            0x8004000A
#define ESGseInformation                0x8004000B
#define ESGseTransparent                0x8004000C
#define ESGseHighlightingMode           0x8004000D
#define ESGseHighlightingColour         0x8004000E
/* The following constant is used in es_ddInfo.h and should be incremented
   every time we add another GSE. */
#define ESNumOfGSEs                      14

/* These defines must match those in esgdp.h */
/* GDP3 defines to match those in esgdp.h */
#define ESGdpSphere			0x80040001
#define ESGdpSphereRadius		0x80040002
#define ESGdpSphereColour		0x80040003
#define ESGdpSphereRadiusColour   	0x80040004
#define ESGdpCylinder		        0x80040005
#define ESGdpCylinderRadius		0x80040006
#define ESGdpCylinderColour		0x80040007
#define ESGdpCylinderRadiusColour	0x80040008
#define ESGdpPointList          	0x80040009
#define ESGdpCircle3            	0x8004000A
#define ESGdpArc3               	0x8004000B
/* The following constant is used in es_ddInfo.h and should be incremented
   every time we add another GDP. */
#define ESNumOfGDP3s                     11


/* marker define constants need to agree with esphigs.h */
#define ESMarkerOctagon 0xFFFF8406
#define ESMarkerInv_Triangle 0xFFFF8405
#define ESMarkerSquare 0xFFFF8404
#define ESMarkerTriangle 0xFFFF8403
#define ESMarkerFast_dot 0xFFFF8402
#define ESMarkerDiamond 0xFFFF8401
#define ESMarkerDefStar 0xFFFF8400

/* line type defines need to agree with esphigs.h */
#define ESLineMask 0xFFFF8401


#define ESNumCircleChords                51

#define ESFrontWin                       0
#define ESBackWin                        1

#define ESSmooth                         0
#define ESJaggy                          1

/* these values must be the same as those in esgdp.h */
#define ESInformationMatrix              0
#define ESInformationId                  1

#define ESHighlightingColor              0
#define ESHighlightingBlink              1

/* When this define is used as a PEX line type or edge type it 
   tells the structure walker to use the currently defined
   GSE Line Pattern Mask or Edge Pattern Mask. */
#define ES_PLINE_MASK                   0xFFFF8401

/* Masks for setting Asf's */
/* This undefines the ending shift value and redefines it */
#undef	PEXMaxAsfShift

#define	ESDataMappingAsf		(1L<<30)
#define	ESBfDataMappingAsf		(1L<<31)
#define PEXMaxAsfShift	31

/* Enum Types */
#define	ESETDataMapMethod			28

/* Table Type */
#define ESDataMappingLUT	(0x8400 | (PEXMaxTableType + 1)) /* E&S 0 */
#define ESMaxTableType		(PEXMaxTableType + 1)

/* Geometric attributes (Vertex, Facet) */
#define	ESPEXGAData	0x0008

/* Enumerated Type Descriptors */

/* SurfaceInterpMethod */
#define ESSurfaceInterpData		0xFFFF8403

/* Data Mapping Methods */
#define ESDataMapMethodColor		1
#define ESDataMapMethodSingleU		2
#define ESDataMapMethodSingleNU		3
#define ESDataMapMethodBiU		4
#define ESDataMapMethodBiNU		5
#define ESDataMapMethodTexture          6


/* Update this when types are added */
#define ESNumOfMapMeths			6

/* Data Mapping Selectors */
#define ESDMSourceSelColor		1
#define ESDMSourceSelFColor		2
#define ESDMSourceSelFData		3
#define ESDMSourceSelVColor		4
#define ESDMSourceSelVData		5

/* Data Mapping Texture types */
#define ESTextureMapTypeMipMap          0
#define ESTextureMapTypeBump            1
#define ESTextureMapTypeDisplacement    2
#define ESTextureMapTypeProcedural      3
#define ESTextureMapTypeComposite       4
#define ESTextureMapTypeShadow          5
#define ESTextureMapTypeEnvironment     6
 
/* Texture Interpolation Methods */
#define ESTextureInterpPoint            0
#define ESTextureInterpBilinear         1
#define ESTextureInterpMipmapPoint      2
#define ESTextureInterpMipmapBilinear   3
#define ESTextureInterpMipmapTrilinear  4

/* Texture Boundary handling */
#define ESTextureClampSpecifiedValue    0
#define ESTextureClampIntrinsicColour   1
#define ESTextureClampBoundaryColour    2
#define ESTextureWrap                   3
#define ESTextureBorder                 4
 
/* Render component values */
#define ESRenderCompIntrinsicColour     0
#define ESRenderCompPostLightingColour  1
#define ESRenderCompDepthCueColour      2
#define ESRenderCompAlpha               3
 
/* Texture operations */
#define ESTextureOpNone                 0
#define ESTextureOpReplace              1
#define ESTextureOpModulate             2
#define ESTextureOpEnvironment          3
#define ESTextureOpDecal                4
#define ESTextureOpBlendReplace         5
#define ESTextureOpBlend                6
 
/* Renderer Bitmasks */
#undef	PEXMaxRDShift

#define	ESRDDataMappingTable		(1L<<23)
#define	PEXMaxRDShift	23

/* Renderer Dynamics Bitmasks */
/*	tables		      */
#define	ESDynDataMappingTableContents		(1L<<28)

/* Phigs Workstation Attribute Bitmasks */
#define	ESPWDataMappingTable	34

/* Indices for GetDynamics */
#define	ESPWDDataMappingTable		21

/* Output Commands */
#define	PEXOCesDataMappingMethod	(0x8400 | (PEXMaxOC + 1)) /* E&S 0 */
#define PEXOCesBfDataMappingMethod	(0x8400 | (PEXMaxOC + 2)) /* E&S 1 */
#define PEXOCesDataMapMethodIndex	(0x8400 | (PEXMaxOC + 3)) /* E&S 2 */
#define PEXOCesBfDataMapMethodIndex	(0x8400 | (PEXMaxOC + 4)) /* E&S 3 */
#define PEXOCesExtFillAreaSet		(0x8400 | (PEXMaxOC + 5)) /* E&S 4 */
#define PEXOCesTriangleStrip		(0x8400 | (PEXMaxOC + 6)) /* E&S 5 */
#define PEXOCesQuadrilateralMesh	(0x8400 | (PEXMaxOC + 7)) /* E&S 6 */
#define PEXOCesSOFAS			(0x8400 | (PEXMaxOC + 8)) /* E&S 7 */
#define ESMaxOC				(PEXMaxOC + 8)

#endif /* ESpex.h */

