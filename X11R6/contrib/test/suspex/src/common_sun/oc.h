/* $XConsortium: oc.h,v 1.0 93/11/22 12:26:15 rws Exp $ */

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
#ifndef _OC_H
#define _OC_H


#define SET_SURFACE_EDGE_COLOR_INDEX(_index)    \
    PEXSetSurfaceEdgeColorIndex(dpy, resourceID, req_type, _index);

#define SET_SURFACE_EDGE_COLOR(_colorType,_colr)	\
    PEXSetSurfaceEdgeColor(dpy, resourceID, req_type, _colorType, _colr);

#define SET_SURFACE_EDGE_TYPE(_type)	\
    PEXSetSurfaceEdgeType(dpy, resourceID, req_type, _type);

#define SET_SURFACE_EDGE_FLAG(_flag)	\
    PEXSetSurfaceEdgeFlag(dpy, resourceID, req_type, _flag);

#define SET_SURFACE_EDGE_WIDTH(_width)	\
    PEXSetSurfaceEdgeWidth(dpy, resourceID, req_type, _width);

#define SET_EDGE_BUNDLE_INDEX(_index)	\
    PEXSetEdgeBundleIndex(dpy, resourceID, req_type, _index);

#define SET_INTERIOR_STYLE(_type)    \
    PEXSetInteriorStyle(dpy, resourceID, req_type, _type);

#define SET_INTERIOR_STYLE_INDEX(_index)    \
    PEXSetInteriorStyleIndex(dpy, resourceID, req_type, _index);

#define SET_SURFACE_COLOR_INDEX(_index)    \
    PEXSetSurfaceColorIndex(dpy, resourceID, req_type, _index);

#define SET_SURFACE_COLOR(_colorType,_colr)	\
    PEXSetSurfaceColor(dpy, resourceID, req_type, _colorType, _colr);

#define SET_INTERIOR_BUNDLE_INDEX(_index)	\
    PEXSetInteriorBundleIndex(dpy, resourceID, req_type, _index);

#define SET_LINE_COLOR_INDEX(_index)    \
    PEXSetLineColorIndex(dpy, resourceID, req_type, _index);

#define SET_LINE_COLOR(_colorType,_colr)	\
    PEXSetLineColor(dpy, resourceID, req_type, _colorType, _colr);

#define SET_LINE_WIDTH(_width)	\
    PEXSetLineWidth(dpy, resourceID, req_type, _width);

#define SET_LINE_TYPE(_type)	\
    PEXSetLineType(dpy, resourceID, req_type, _type);

#define SET_LINE_BUNDLE_INDEX(_index)	\
    PEXSetLineBundleIndex(dpy, resourceID, req_type, _index);

#define DRAW_Polyline2D(_np, _points) \
    PEXPolyline2D(dpy, resourceID, req_type, (_np), (_points))

#define DRAW_Polyline(_np, _points) \
    PEXPolyline(dpy, resourceID, req_type, (_np), (_points))

#define SET_INDIVIDUAL_ASF(_asf,_flag) \
    PEXSetIndividualASF(dpy, resourceID, req_type, (_asf), (_flag))

#define TRANSLATE(_x,_y) \
{			\
    vector.x = _x;	\
    vector.y = _y;	\
    PEXTranslate(&vector, matrix); \
}


#define SCALE(_x,_y,_z) \
{			\
    vector.x = _x;	\
    vector.y = _y;	\
    vector.z = _z;	\
    PEXScale(&vector, matrix); \
}


#define SET_LOCAL_TRANSFORM(comp_type, mat)	\
    PEXSetLocalTransform(dpy, resourceID, req_type, comp_type, mat)

#define SET_GLOBAL_TRANSFORM(mat)	\
    PEXSetGlobalTransform(dpy, resourceID, req_type, mat)

PEXMatrix identity_matrix = { 
	{1.0, 0.0, 0.0, 0.0},	
	{0.0, 1.0, 0.0, 0.0},	
	{0.0, 0.0, 1.0, 0.0},	
	{0.0, 0.0, 0.0, 1.0}} ;

#endif /* _OC_H */
