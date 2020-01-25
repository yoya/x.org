/* $XConsortium: SunPEXproto.h,v 3.1 93/11/22 12:08:03 rws Exp $ */

/*
 * Copyright (c) 1992 by Sun Microsystems, Inc.
 * All Rights Reserved
 */

#ifndef SUNPEX_PROTO_H
#define SUNPEX_PROTO_H 1

/* In the following typedefs, comments appear that say "LISTof Foo( numItems )",
 * "CLIST of Foo()", and "SINGLE Foo()".   These are used when the protocol
 * specifies that a request or reply contains a variable length list of
 * (possibly variable types of) objects.
 *
 * A LISTof list is one for which we have already been given the length.
 * The items in the list are of type "Foo". The number of items in the list * appears parenthetically after the type.  ("numItems" in our example.)
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
 * the parentheses.  (number of bytes of padding = n?(3-((n-1)%4):0 , where * n is the parenthetical value.)
 */
 
#include "X11/PEX5/PEX.h"
#include "X11/PEX5/PEXproto.h"
#include "X11/PEX5/SunPEX.h"

typedef struct {
    pexCoord3D	center;
    pexVector3D	dir[2];
    PEXFLOAT	radius;
} pexSunGdp3DCircle;

typedef struct {
    pexCoord2D	center;
    PEXFLOAT	radius;
} pexSunGdp2DCircle;

typedef struct {
    pexCoord3D	center;
    PEXFLOAT	radius;
} pexSunGdp3DAnnotCircle;

typedef struct {
    pexCoord2D	center;
    PEXFLOAT	radius;
} pexSunGdp2DAnnotCircle;

typedef struct {
    pexCoord3D	center;
    pexVector3D	dir[2];
    PEXFLOAT	radius;
    PEXFLOAT	start_angle;
    PEXFLOAT	end_angle;
} pexSunGdp3DCircArc;
	
typedef struct {
    pexCoord2D	center;
    PEXFLOAT	radius;
    PEXFLOAT	start_angle;
    PEXFLOAT	end_angle;
} pexSunGdp2DCircArc;
	
typedef struct {
    pexCoord3D	center;
    PEXFLOAT	radius;
    PEXFLOAT	start_angle;
    PEXFLOAT	end_angle;
} pexSunGdp3DAnnotCircArc;

typedef struct {
    pexCoord2D	center;
    PEXFLOAT	radius;
    PEXFLOAT	start_angle;
    PEXFLOAT	end_angle;
} pexSunGdp2DAnnotCircArc;

typedef struct {
    pexCoord3D	center;
    pexVector3D	dir[2];
    PEXFLOAT	radius;
    PEXFLOAT	start_angle;
    PEXFLOAT	end_angle;
    INT32	close_type;
} pexSunGdp3DCircArcClose;

typedef struct {
    pexCoord2D	center;
    PEXFLOAT	radius;
    PEXFLOAT	start_angle;
    PEXFLOAT	end_angle;
    INT32	close_type;
} pexSunGdp2DCircArcClose;

typedef struct {
    pexCoord3D	center;
    PEXFLOAT	radius;
    PEXFLOAT	start_angle;
    PEXFLOAT	end_angle;
    INT32	close_type;
} pexSunGdp3DAnnotCircArcClose;

typedef struct {
    pexCoord2D	center;
    PEXFLOAT	radius;
    PEXFLOAT	start_angle;
    PEXFLOAT	end_angle;
    INT32	close_type;
} pexSunGdp2DAnnotCircArcClose;

typedef struct {
    pexCoord3D	center;
    pexVector3D	major_axis;
    pexVector3D	minor_axis;
} pexSunGdp3DEllipse;

typedef struct {
    pexCoord2D	center;
    pexVector2D	major_axis;
    pexVector3D	minor_axis;
} pexSunGdp2DEllipse;

typedef struct {
    pexCoord3D	center;
    pexVector3D	major_axis;
    pexVector3D	minor_axis;
    PEXFLOAT	start_angle;
    PEXFLOAT	end_angle;
} pexSunGdp3DEllpArc;

typedef struct {
    pexCoord2D	center;
    pexVector2D	major_axis;
    pexVector2D	minor_axis;
    PEXFLOAT	start_angle;
    PEXFLOAT	end_angle;
} pexSunGdp2DEllpArc;

typedef struct {
    pexCoord3D	center;
    pexVector3D	major_axis;
    pexVector3D	minor_axis;
    PEXFLOAT	start_angle;
    PEXFLOAT	end_angle;
    INT32	close_type;
} pexSunGdp3DEllpArcClose;

typedef struct {
    pexCoord2D	center;
    pexVector2D	major_axis;
    pexVector2D	minor_axis;
    PEXFLOAT	start_angle;
    PEXFLOAT	end_angle;
    INT32	close_type;
} pexSunGdp2DEllpArcClose;

typedef struct {
    pexCoord3D	center;
    pexVector2D	major_axis;
    pexVector2D	minor_axis;
} pexSunGdp3DAnnotEllipse;

typedef struct {
    pexCoord2D	center;
    pexVector2D	major_axis;
    pexVector2D	minor_axis;
} pexSunGdp2DAnnotEllipse;

typedef struct {
    pexCoord3D	center;
    pexVector2D	major_axis;
    pexVector2D	minor_axis;
    PEXFLOAT	start_angle;
    PEXFLOAT	end_angle;
} pexSunGdp3DAnnotEllpArc;

typedef struct {
    pexCoord2D	center;
    pexVector2D	major_axis;
    pexVector2D	minor_axis;
    PEXFLOAT	start_angle;
    PEXFLOAT	end_angle;
} pexSunGdp2DAnnotEllpArc;

typedef struct {
    pexCoord3D	center;
    pexVector2D	major_axis;
    pexVector2D	minor_axis;
    PEXFLOAT	start_angle;
    PEXFLOAT	end_angle;
    INT32	close_type;
} pexSunGdp3DAnnotEllpArcClose;

typedef struct {
    pexCoord2D	center;
    pexVector2D	major_axis;
    pexVector2D	minor_axis;
    PEXFLOAT	start_angle;
    PEXFLOAT	end_angle;
    INT32	close_type;
} pexSunGdp2DAnnotEllpArcClose;

typedef struct {
    pexCoord3D	center;
    PEXFLOAT	dx;
    PEXFLOAT	dy;
} pexSunGdp3DRectGrid;

typedef struct {
    pexCoord2D	center;
    PEXFLOAT	dx;
    PEXFLOAT	dy;
} pexSunGdp2DRectGrid;

typedef struct {
    pexCoord3D	center;
    PEXFLOAT	angle;
    PEXFLOAT	dist;
} pexSunGdp3DRadGrid;

typedef struct {
    pexCoord2D	center;
    PEXFLOAT	angle;
    PEXFLOAT	dist;
} pexSunGdp2DRadGrid;

typedef struct {
    pexColorType	colorType B32;
    pexBitmask		facetAttribs B32;  
    pexBitmask		vertexAttribs B32;
    pexBitmask		flags B32;
    CARD32		numFacets B32;
    CARD32		numVertices B32;
    /* LISTof pexOptData( numFacets, facetAttribs, colorType ) */
    /* LISTof pexVertex( numVertices, vertexAttribs, colorType ) */
} pexSunGdp3DTriList;

typedef struct {
    pexColorSpecifier	colorspec;
    /* SINGLE COLOR() */
} pexSunGseHighlightColor;

typedef struct {
    PEXFLOAT	slant_angle;
} pexSunGseTextSlantAngle;

typedef struct {
    PEXFLOAT	slant_angle;
} pexSunGseAnnotTextSlantAngle;

typedef struct {
    CARD32	prim;
    INT32	blend_eq;
    INT32	filter_width;
    INT32	filter_shape;
} pexSunGseStrokeAAlias;

typedef struct {
    PEXFLOAT	scale;
} pexSunGseTextStrokeWidth;

typedef struct {
    CARD32	prim;
    INT32	type;
} pexSunGseEndcap;

typedef struct {
    CARD32	prim;
    INT32	type;
} pexSunGseStrokeJoin;

typedef struct {
    INT32	flag;
} pexSunGseSilhouettedEdge;

typedef struct {
    PEXFLOAT	value;
} pexSunGseTranspCoef;

typedef struct {
    CARD8               reqType;
    CARD8               opcode;
    CARD16              length B16;     /* 2 + n */
    CARD32              escapeID B32;
    /* 4n bytes of additional escape data to skip */
} pexSunEscapeReq, pexSunEscapeWithReplyReq;

typedef struct {
    BYTE        type;                   /* X_Reply */
    CARD8       what;                   /* unused */
    CARD16      sequenceNumber  B16;
    CARD32      length B32;
    CARD32      escapeID B32;
    CARD8       escape_specific[20];
    /* more escape specific data, treat as */
    /* LISTof CARD8( length ) */
} pexSunEscapeWithReplyReply;

typedef struct {
    XID         	id;		/* drawable id */
    CARD32              buffer_mode B32;
} pexSunEscSetBufferMode;

typedef struct {
    XID         	id B32;		/* drawable id */
} pexSunEscSwapBuffer;

typedef struct {
    XID                 id;             /* drawable id */
    INT32               draw_buffer B32;
} pexSunEscSetDrawBuffer;

typedef struct {
    XID                 id B32;         /* drawable id */
} pexSunEscGetSwapBufferContent;

typedef struct {
   BYTE        type;                   /* X_Reply */
   CARD8       what;                   /* unused */
   CARD16      sequenceNumber  B16;
   CARD32      length B32;
   CARD32      escapeID B32;
   CARD32      content B32;
   CARD8       unused[16];
} pexSunEscGetSwapBufferContentReply;

typedef struct {
   pexEnumTypeIndex	fpFormat B16;
   CARD16		unused B16;
   CARD32		marker_id;
   CARD32		num_lists;
   /* LISTof LISTof COORD2D */
} pexSunEscDefineMarkerType;

typedef struct {
   pexEnumTypeIndex	fpFormat B16;
   CARD16		unused B16;
   CARD32		marker_id;
} pexSunEscGetMarkerDescr;

typedef struct {
   BYTE        type;                   /* X_Reply */
   CARD8       what;                   /* unused */
   CARD16      sequenceNumber  B16;
   CARD32      length B32;
   CARD32      escapeID B32;
   CARD8       unused[20];
   CARD32      num_lists;
   /* LISTof LISTof COORD2D */
} pexSunEscGetMarkerDescrReply;

typedef struct {
   pexEnumTypeIndex	fpFormat B16;
   CARD16		unused B16;
   XID			id B32;		/* renderer id */
   CARD32		itemMask;	
   /* LISTofVALUE */
} pexSunEscChangeExtRendAttr;
   
typedef struct {
   pexEnumTypeIndex	fpFormat B16;
   CARD16		unused B16;
   XID			id B32;		/* renderer id */
   CARD32		itemMask;	
} pexSunEscGetExtRendAttr;

typedef struct {
   BYTE        type;                   /* X_Reply */
   CARD8       what;                   /* unused */
   CARD16      sequenceNumber  B16;
   CARD32      length B32;
   CARD32      escapeID B32;
   CARD8       unused[20];
   /* LISTofVALUE */
} pexSunEscGetExtRendAttrReply;

typedef struct {
   XID			id B32;
} pexSunEscGetExtRendAttrDyn;

typedef struct {
   BYTE        type;                   /* X_Reply */
   CARD8       what;                   /* unused */
   CARD16      sequenceNumber  B16;
   CARD32      length B32;
   CARD32      escapeID B32;
   CARD32      itemMask;
   CARD8       unused[16];
} pexSunEscGetExtRendAttrDynReply;

#endif /* SUNPEX_PROTO_H */
