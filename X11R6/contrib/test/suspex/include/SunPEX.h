/* $XConsortium: SunPEX.h,v 3.1 93/11/22 12:07:57 rws Exp $ */

/*
 * Copyright (c) 1992 by Sun Microsystems, Inc.
 * All Rights Reserved
 */

#ifndef SUNPEX_H
#define SUNPEX_H 1

/*
 * This file contains definition for Sun's GDP/GSE/ESCAPE ids and
 * the associated template data structures.
 */

#define	PEXSunVendorId	5

/* Sun GDP 3D Ids*/
#define PEXSunGDP3DIdCircle        	(0x80000000 |(PEXSunVendorId<<16)| 2)
#define PEXSunGDP3DIdCircArc       	(0x80000000 |(PEXSunVendorId<<16)| 3)
#define PEXSunGDP3DIdCircArcClose  	(0x80000000 |(PEXSunVendorId<<16)| 4)
#define PEXSunGDP3DIdAnnotCircle   	(0x80000000 |(PEXSunVendorId<<16)| 5)
#define PEXSunGDP3DIdAnnotCircArc  	(0x80000000 |(PEXSunVendorId<<16)| 6)
#define PEXSunGDP3DIdAnnotCircArcClose	(0x80000000 |(PEXSunVendorId<<16)| 7)
#define PEXSunGDP3DIdEllipse       	(0x80000000 |(PEXSunVendorId<<16)| 8)
#define PEXSunGDP3DIdEllpArc       	(0x80000000 |(PEXSunVendorId<<16)| 9)
#define PEXSunGDP3DIdEllpArcClose  	(0x80000000 |(PEXSunVendorId<<16)| 10)
#define PEXSunGDP3DIdAnnotEllipse  	(0x80000000 |(PEXSunVendorId<<16)| 11)
#define PEXSunGDP3DIdAnnotEllpArc  	(0x80000000 |(PEXSunVendorId<<16)| 12)
#define PEXSunGDP3DIdAnnotEllpArcClose 	(0x80000000 |(PEXSunVendorId<<16)| 13)
#define PEXSunGDP3DIdImage         	(0x80000000 |(PEXSunVendorId<<16)| 14)
#define PEXSunGDP3DIdRectGrid      	(0x80000000 |(PEXSunVendorId<<16)| 15)
#define PEXSunGDP3DIdRadialGrid    	(0x80000000 |(PEXSunVendorId<<16)| 16)
#define PEXSunGDP3DIdTriangleList 	(0x80000000 |(PEXSunVendorId<<16)| 17)

/* Sun GDP 3D enumerated types */
#define PEXETSunGDP3DCircle            	(0x8000 |(PEXSunVendorId<<8)| 2)
#define PEXETSunGDP3DCircArc           	(0x8000 |(PEXSunVendorId<<8)| 3)
#define PEXETSunGDP3DCircArcClose      	(0x8000 |(PEXSunVendorId<<8)| 4)
#define PEXETSunGDP3DAnnotCircle       	(0x8000 |(PEXSunVendorId<<8)| 5)
#define PEXETSunGDP3DAnnotCircArc      	(0x8000 |(PEXSunVendorId<<8)| 6)
#define PEXETSunGDP3DAnnotCircArcClose 	(0x8000 |(PEXSunVendorId<<8)| 7)
#define PEXETSunGDP3DEllipse           	(0x8000 |(PEXSunVendorId<<8)| 8)
#define PEXETSunGDP3DEllpArc           	(0x8000 |(PEXSunVendorId<<8)| 9)
#define PEXETSunGDP2DEllpArcClose      	(0x8000 |(PEXSunVendorId<<8)| 10)
#define PEXETSunGDP3DAnnotEllipse      	(0x8000 |(PEXSunVendorId<<8)| 11)
#define PEXETSunGDP3DAnnotEllpArc      	(0x8000 |(PEXSunVendorId<<8)| 12)
#define PEXETSunGDP3DAnnotEllpArcClose 	(0x8000 |(PEXSunVendorId<<8)| 13)
#define PEXETSunGDP3DImage 	       	(0x8000 |(PEXSunVendorId<<8)| 14)
#define PEXETSunGDP3DRectGrid 	       	(0x8000 |(PEXSunVendorId<<8)| 15)
#define PEXETSunGDP3DRadialGrid        	(0x8000 |(PEXSunVendorId<<8)| 16)
#define PEXETSunGDP3DTriangleList 	(0x8000 |(PEXSunVendorId<<8)| 17)

/* Sun GDP 3D memonics */
#define PEXETMSun3DCircle		"SunCircle"
#define PEXETMSun3DCircArc		"SunCircularArc"
#define PEXETMSun3DCircArcClose		"SunCircularArcClose"
#define PEXETMSun3DAnnotCircle		"SunAnnotationCircle"
#define PEXETMSun3DAnnotCircArc		"SunAnnotationCircularArc"
#define PEXETMSun3DAnnotCircArcClose	"SunAnnotationCircularArcClose"
#define PEXETMSun3DEllipse		"SunEllipse"
#define PEXETMSun3DEllpArc		"SunEllipticalArc"
#define PEXETMSun3DEllpArcClose		"SunEllipticalArcClose"
#define PEXETMSun3DAnnotEllipse		"SunAnnotationEllipse"
#define PEXETMSun3DAnnotEllpArc		"SunAnnotationEllipticalArc"
#define PEXETMSun3DAnnotEllpArcClose	"SunAnnotationEllipticalArcClose"
#define PEXETMSun3DImage		"SunImage"
#define PEXETMSun3DRectGrid		"SunRectangularGrid"
#define PEXETMSun3DRadialGrid		"SunRadialGrid"
#define PEXETMSun3DTriangleList		"SunTriangleList"

/* Sun GDP 2D Ids*/
#define PEXSunGDP2DIdCircle            	(0x80000000 |(PEXSunVendorId<<16)| 2)
#define PEXSunGDP2DIdCircArc           	(0x80000000 |(PEXSunVendorId<<16)| 3)
#define PEXSunGDP2DIdCircArcClose      	(0x80000000 |(PEXSunVendorId<<16)| 4)
#define PEXSunGDP2DIdAnnotCircle       	(0x80000000 |(PEXSunVendorId<<16)| 5)
#define PEXSunGDP2DIdAnnotCircArc      	(0x80000000 |(PEXSunVendorId<<16)| 6)
#define PEXSunGDP2DIdAnnotCircArcClose 	(0x80000000 |(PEXSunVendorId<<16)| 7)
#define PEXSunGDP2DIdEllipse           	(0x80000000 |(PEXSunVendorId<<16)| 8)
#define PEXSunGDP2DIdEllpArc           	(0x80000000 |(PEXSunVendorId<<16)| 9)
#define PEXSunGDP2DIdEllpArcClose      	(0x80000000 |(PEXSunVendorId<<16)| 10)
#define PEXSunGDP2DIdAnnotEllipse      	(0x80000000 |(PEXSunVendorId<<16)| 11)
#define PEXSunGDP2DIdAnnotEllpArc      	(0x80000000 |(PEXSunVendorId<<16)| 12)
#define PEXSunGDP2DIdAnnotEllpArcClose 	(0x80000000 |(PEXSunVendorId<<16)| 13)
#define PEXSunGDP2DIdImage 	       	(0x80000000 |(PEXSunVendorId<<16)| 14)
#define PEXSunGDP2DIdRectGrid 	       	(0x80000000 |(PEXSunVendorId<<16)| 15)
#define PEXSunGDP2DIdRadialGrid        	(0x80000000 |(PEXSunVendorId<<16)| 16)

/* Sun GDP 2D enumerated types */
#define PEXETSunGDP2DCircle            	(0x8000 |(PEXSunVendorId<<8)| 2)
#define PEXETSunGDP2DCircArc           	(0x8000 |(PEXSunVendorId<<8)| 3)
#define PEXETSunGDP2DCircArcClose      	(0x8000 |(PEXSunVendorId<<8)| 4)
#define PEXETSunGDP2DAnnotCircle       	(0x8000 |(PEXSunVendorId<<8)| 5)
#define PEXETSunGDP2DAnnotCircArc      	(0x8000 |(PEXSunVendorId<<8)| 6)
#define PEXETSunGDP2DAnnotCircArcClose 	(0x8000 |(PEXSunVendorId<<8)| 7)
#define PEXETSunGDP2DEllipse           	(0x8000 |(PEXSunVendorId<<8)| 8)
#define PEXETSunGDP2DEllpArc           	(0x8000 |(PEXSunVendorId<<8)| 9)
#define PEXETSunGDP2DEllpArcClose      	(0x8000 |(PEXSunVendorId<<8)| 10)
#define PEXETSunGDP2DAnnotEllipse      	(0x8000 |(PEXSunVendorId<<8)| 11)
#define PEXETSunGDP2DAnnotEllpArc      	(0x8000 |(PEXSunVendorId<<8)| 12)
#define PEXETSunGDP2DAnnotEllpArcClose 	(0x8000 |(PEXSunVendorId<<8)| 13)
#define PEXETSunGDP2DImage 	       	(0x8000 |(PEXSunVendorId<<8)| 14)
#define PEXETSunGDP2DRectGrid 	       	(0x8000 |(PEXSunVendorId<<8)| 15)
#define PEXETSunGDP2DRadialGrid        	(0x8000 |(PEXSunVendorId<<8)| 16)

/* Sun GDP 2D memonics */
#define PEXETMSun2DCircle		"SunCircle"
#define PEXETMSun2DCircArc		"SunCircularArc"
#define PEXETMSun2DCircArcClose		"SunCircularArcClose"
#define PEXETMSun2DAnnotCircle		"SunAnnotationCircle"
#define PEXETMSun2DAnnotCircArc		"SunAnnotationCircularArc"
#define PEXETMSun2DAnnotCircArcClose	"SunAnnotationCircularArcClose"
#define PEXETMSun2DEllipse		"SunEllipse"
#define PEXETMSun2DEllpArc		"SunEllipticalArc"
#define PEXETMSun2DEllpArcClose		"SunEllipticalArcClose"
#define PEXETMSun2DAnnotEllipse		"SunAnnotationEllipse"
#define PEXETMSun2DAnnotEllpArc		"SunAnnotationEllipticalArc"
#define PEXETMSun2DAnnotEllpArcClose	"SunAnnotationEllipticalArcClose"
#define PEXETMSun2DImage		"SunImage"
#define PEXETMSun2DRectGrid		"SunRectangularGrid"
#define PEXETMSun2DRadialGrid		"SunRadialGrid"

/* Sun GSE Ids */
#define PEXSunGSEIdSetHighlightColor		\
				(0x80000000 | (PEXSunVendorId << 16) | 1)
#define PEXSunGSEIdSetTextSlantAngle		\
				(0x80000000 | (PEXSunVendorId << 16) | 4)
#define PEXSunGSEIdSetAnnotTextSlantAngle	\
                                (0x80000000 | (PEXSunVendorId << 16) | 5)
#define PEXSunGSEIdSetStrokeAAliasParams  	\
                                (0x80000000 | (PEXSunVendorId << 16) | 7)
#define PEXSunGSEIdSetTextStrokeWidth		\
                                (0x80000000 | (PEXSunVendorId << 16) | 8)
#define PEXSunGSEIdSetEndcap   			\
                                (0x80000000 | (PEXSunVendorId << 16) | 9)
#define PEXSunGSEIdSetStrokeJoin    		\
                                (0x80000000 | (PEXSunVendorId << 16) | 10)
#define PEXSunGSEIdSetSilhouetteEdgeFlag    	\
                                (0x80000000 | (PEXSunVendorId << 16) | 11)
#define PEXSunGSEIdSetSurfTranspCoef    	\
                                (0x80000000 | (PEXSunVendorId << 16) | 12)

/* Sun GSE enumerated types */
#define PEXETSunGSESetHighlightColor 	  (0x8000 | (PEXSunVendorId << 8) | 1)
#define PEXETSunGSESetTextSlantAngle 	  (0x8000 | (PEXSunVendorId << 8) | 4)
#define PEXETSunGSESetAnnotTextSlantAngle (0x8000 | (PEXSunVendorId << 8) | 5)
#define PEXETSunGSESetStrokeAAliasParams  (0x8000 | (PEXSunVendorId << 8) | 7)
#define PEXETSunGSESetTextStrokeWidth 	  (0x8000 | (PEXSunVendorId << 8) | 8)
#define PEXETSunGSESetEndcap   		  (0x8000 | (PEXSunVendorId << 8) | 9)
#define PEXETSunGSESetStrokeJoin    	  (0x8000 | (PEXSunVendorId << 8) | 10)
#define PEXETSunGSESetSilhouetteEdgeFlag  (0x8000 | (PEXSunVendorId << 8) | 11)
#define PEXETSunGSESetSurfTranspCoef      (0x8000 | (PEXSunVendorId << 8) | 12)

/* Sun GSE mnemonics */
#define PEXETMSunGSESetHighlightColor	   "SunSetHighlightColor"
#define PEXETMSunGSESetTextSlantAngle	   "SunSetTextSlantAngle"
#define PEXETMSunGSESetAnnotTextSlantAngle "SunSetAnnotationTextSlantAngle"
#define PEXETMSunGSESetStrokeAAliasParams  "SunSetStrokeAntiAliasParameters"
#define PEXETMSunGSESetTextStrokeWidth 	   "SunSetTextStrokeWidth"
#define PEXETMSunGSESetEndcap 		   "SunSetEndcap"
#define PEXETMSunGSESetStrokeJoin 	   "SunSetStrokeJoin"
#define PEXETMSunGSESetSilhouetteEdgeFlag  "SunSetSilhouetteEdgeFlag"
#define PEXETMSunGSESetSurfTranspCoef      "SunSetSurfaceTransparencyCoefficient"

/* Sun ESCAPE Ids */
#define PEXSunEscIdDefineMarkerType    	(0x80000000 |(PEXSunVendorId<<16)| 5)
#define PEXSunEscIdGetMarkerDescr    	(0x80000000 |(PEXSunVendorId<<16)| 6)
#define PEXSunEscIdChangeExtRendAttr	(0x80000000 |(PEXSunVendorId<<16)| 7)
#define PEXSunEscIdGetExtRendAttr	(0x80000000 |(PEXSunVendorId<<16)| 8)
#define PEXSunEscIdGetExtRendAttrDyn	(0x80000000 |(PEXSunVendorId<<16)| 9)

/* Sun ESCAPE enumerated types */
#define PEXETSunEscDefineMarkerType    	(0x8000 |(PEXSunVendorId<<8)| 5)
#define PEXETSunEscGetMarkerDescr    	(0x8000 |(PEXSunVendorId<<8)| 6)
#define PEXETSunEscChangeExtRendAttr	(0x8000 |(PEXSunVendorId<<8)| 7)
#define PEXETSunEscGetExtRendAttr	(0x8000 |(PEXSunVendorId<<8)| 8)
#define PEXETSunEscGetExtRendAttrDyn	(0x8000 |(PEXSunVendorId<<16)| 9)

/* Sun ESCAPE mnemonics */
#define PEXETMSunEscDefineMarkerType   	 "SunDefineMarkerType"
#define PEXETMSunEscGetMarkerDescr     	 "SunGetMarkerDescription"
#define PEXETMSunEscChangeExtRendAttr  	 "SunChangeExtendedRendererAttributes"
#define PEXETMSunEscGetExtRendAttr     	 "SunGetExtendedRendererAttributes"
#define PEXETMSunEscGetExtRendAttrDyn    "SunGetExtendedRendererAttributeDynamics"

/* Sun line types */
#define PEXSunLineTypeLongDash 		(0x8000 | (PEXSunVendorId << 8) | 0)
#define PEXSunLineTypeDotDashDot 	(0x8000 | (PEXSunVendorId << 8) | 1)
#define PEXSunLineTypeCenter		(0x8000 | (PEXSunVendorId << 8) | 2)
#define PEXSunLineTypePhantom		(0x8000 | (PEXSunVendorId << 8) | 3)

/* Sun line type mnemonics */
#define PEXETMSunLineTypeLongDash	"SunLongDash"
#define PEXETMSunLineTypeDotDashDot	"SunDotDashDot"
#define PEXETMSunLineTypeCenter		"SunCenter"
#define PEXETMSunLineTypePhantom	"SunPhantom"

/* Sun maker types */
#define PEXSunMarkerTypeSquare 		(0x8000 | (PEXSunVendorId << 8) | 0)
#define PEXSunMarkerTypeBowtieNE 	(0x8000 | (PEXSunVendorId << 8) | 1)
#define PEXSunMarkerTypeBowtieNW 	(0x8000 | (PEXSunVendorId << 8) | 2)

/* Sun maker type mnemonics */
#define PEXETMSunMarkerTypeSquare 	"SunSquare"
#define PEXETMSunMarkerTypeBowtieNE 	"SunBowtieNE"
#define PEXETMSunMarkerTypeBowtieNW 	"SunBowtieNW"

/* Sun hatch styles */
#define PEXSunHatchStyleHoriz 		(0x8000 | (PEXSunVendorId << 8) | 1)
#define PEXSunHatchStyleHorizDbl 	(0x8000 | (PEXSunVendorId << 8) | 51)
#define PEXSunHatchStyleHorizTrans 	(0x8000 | (PEXSunVendorId << 8) | 101)
#define PEXSunHatchStyleHorizDblTrans 	(0x8000 | (PEXSunVendorId << 8) | 151)
#define PEXSunHatchStyleVert           	(0x8000 | (PEXSunVendorId << 8) | 2)
#define PEXSunHatchStyleVertDbl       	(0x8000 | (PEXSunVendorId << 8) | 52)
#define PEXSunHatchStyleVertTrans      	(0x8000 | (PEXSunVendorId << 8) | 102)
#define PEXSunHatchStyleVertDblTrans    (0x8000 | (PEXSunVendorId << 8) | 152)
#define PEXSunHatchStyleDiag45          (0x8000 | (PEXSunVendorId << 8) | 3)
#define PEXSunHatchStyleDiag45Dbl       (0x8000 | (PEXSunVendorId << 8) | 53)
#define PEXSunHatchStyleDiag45Trans     (0x8000 | (PEXSunVendorId << 8) | 103)
#define PEXSunHatchStyleDiag45DblTrans  (0x8000 | (PEXSunVendorId << 8) | 153)
#define PEXSunHatchStyleDiag135         (0x8000 | (PEXSunVendorId << 8) | 4)
#define PEXSunHatchStyleDiag135Dbl      (0x8000 | (PEXSunVendorId << 8) | 54)
#define PEXSunHatchStyleDiag135Trans    (0x8000 | (PEXSunVendorId << 8) | 104)
#define PEXSunHatchStyleDiag135DblTrans (0x8000 | (PEXSunVendorId << 8) | 154)
#define PEXSunHatchStyleGridR           (0x8000 | (PEXSunVendorId << 8) | 5)
#define PEXSunHatchStyleGridRDbl        (0x8000 | (PEXSunVendorId << 8) | 55) 
#define PEXSunHatchStyleGridRTrans 	(0x8000 | (PEXSunVendorId << 8) | 105)
#define PEXSunHatchStyleGridRDblTrans 	(0x8000 | (PEXSunVendorId << 8) | 155)
#define PEXSunHatchStyleGridD           (0x8000 | (PEXSunVendorId << 8) | 6)
#define PEXSunHatchStyleGridDDbl        (0x8000 | (PEXSunVendorId << 8) | 56) 
#define PEXSunHatchStyleGridDTrans      (0x8000 | (PEXSunVendorId << 8) | 106)
#define PEXSunHatchStyleGridDDblTrans   (0x8000 | (PEXSunVendorId << 8) | 156)

/* Sun hatch style mnemonics */
#define PEXETMSunHatchStyleHoriz 	   "SunHorizontal"
#define PEXETMSunHatchStyleHorizDbl 	   "SunHorizontalDouble"
#define PEXETMSunHatchStyleHorizTrans 	   "SunHorizontalTransparent"
#define PEXETMSunHatchStyleHorizDblTrans   "SunHorizontalDoubleTransparent"
#define PEXETMSunHatchStyleVert            "SunVertical"
#define PEXETMSunHatchStyleVertDbl         "SunVerticalDouble"
#define PEXETMSunHatchStyleVertTrans       "SunVerticalTransparent"
#define PEXETMSunHatchStyleVertDblTrans    "SunVerticalDoubleTransparent"
#define PEXETMSunHatchStyleDiag45          "SunDiagonal45"
#define PEXETMSunHatchStyleDiag45Dbl       "SunDiagonal45Double"
#define PEXETMSunHatchStyleDiag45Trans     "SunDiagonal45Transparent"
#define PEXETMSunHatchStyleDiag45DblTrans  "SunDiagonal45DoubleTransparent"
#define PEXETMSunHatchStyleDiag135         "SunDiagonal135"
#define PEXETMSunHatchStyleDiag135Dbl      "SunDiagonal135Double"
#define PEXETMSunHatchStyleDiag135Trans    "SunDiagonal135Transparent"
#define PEXETMSunHatchStyleDiag135DblTrans "SunDiagonal135DoubleTransparent"
#define PEXETMSunHatchStyleGridR           "SunGridRectagular"
#define PEXETMSunHatchStyleGridRDbl        "SunGridRectagularDouble"
#define PEXETMSunHatchStyleGridRTrans 	   "SunGridRectagularTransparent"
#define PEXETMSunHatchStyleGridRDblTrans   "SunGridRectagularDoubleTransparent"
#define PEXETMSunHatchStyleGridD           "SunGridDiagonal"
#define PEXETMSunHatchStyleGridDDbl        "SunGridDiagonalDouble"
#define PEXETMSunHatchStyleGridDTrans      "SunGridDiagonalTransparent"
#define PEXETMSunHatchStyleGridDDblTrans   "SunGridDiagonalDoubleTransparent"

/* Arc close types */
#define PEXSunArcClosePie			0
#define PEXSunArcCloseChord			1

/* Anti-alias primitive types */
#define PEXSunAAliasPrimLine			0x00000001
#define PEXSunAAliasPrimMarker			0x00000002
#define PEXSunAAliasPrimText			0x00000004
#define PEXSunAAliasPrimHollowSurf		0x00000008
#define PEXSunAAliasPrimEdge			0x00000010
#define PEXSunAAliasPrimAll			0xffffffff

/* Anti-alias blend equations */
#define PEXSunAAliasBlendNone			0
#define PEXSunAAliasBlendArbitBG		1
#define PEXSunAAliasBlendConstBG		2
#define PEXSunAAliasBlendAddToBG		3

#define PEXSunAAliasFiltShapeGaussian		0

/* Endcap primitive types */
#define PEXSunEndcapPrimLine			0x00000001
#define PEXSunEndcapPrimText			0x00000002
#define PEXSunEndcapPrimEdge			0x00000004
#define PEXSunAAliasPrimAll			0xffffffff

/* Endcap types */
#define PEXSunEndcapTypeButt  			0
#define PEXSunEndcapTypeSquare   		1
#define PEXSunEndcapTypeRound  			2

/* Join primitive types */
#define PEXSunJoinPrimLine  			0x00000001
#define PEXSunJoinPrimText  			0x00000002
#define PEXSunJoinPrimEdge  			0x00000004
#define PEXSunJoinPrimAll			0xffffffff

/* Join styles */
#define PEXSunJoinTypeMitred  			0
#define PEXSunJoinTypeRound  			1
#define PEXSunJoinTypeBeveled  			2
#define PEXSunJoinTypeAny  			3

/* Triangle-list mutually exclusive flags */
#define PEXSunTlistFlagTriRestart		0x00
#define PEXSunTlistFlagTriStrip			0x01
#define PEXSunTlistFlagTriStar			0x02
#define PEXSunTlistFlagUseVtxFlags		0x03

/* Triangle-list non-mutually exclusive flags */
#define PEXSunTlistFlagEdgesOn			0x04
#define PEXSunTlistFlagPlanar			0x08
#define PEXSunTlistFlagFNNotConsistent		0x00
#define PEXSunTlistFlagFNConsistent		0x10
#define PEXSunTlistFlagFNConsistentCW		0x20
#define PEXSunTlistFlagFNConsistentCCW		0x30


/* Extended renderer attribute bitmasks */
#define PEXRDSunAAliasMode			0x00000001
#define PEXRDSunTranspMode			0x00000002
#define PEXRDSunStereoMode			0x00000004
#define PEXRDSunSilhouetteEdgeMode		0x00000008

/* Extended renderer dynamics bitmasks */
#define PEXDynSunAAliasMode			0x00000001
#define PEXDynSunTranspMode			0x00000002
#define PEXDynSunStereoMode			0x00000004
#define PEXDynSunSilhouetteEdgeMode		0x00000008

/* AntiAlias Modes */
#define PEXSunAAliasNone			0
#define PEXSunAAliasBlended			1

/* Transparency Modes */
#define PEXSunTranspNone			0
#define PEXSunTranspSreenDoor			1
#define PEXSunTranspBlended			2

/* Stereo Modes */
#define PEXSunStereoOff				0
#define PEXSunStereoOn				1

/* Silhouette Edge Modes */
#define PEXSunSilhouetteModeOff			0
#define PEXSunSilhouetteModeOn			1

#endif /* SUNPEX_H */
