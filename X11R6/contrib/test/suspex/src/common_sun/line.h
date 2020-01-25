/* $XConsortium: line.h,v 1.0 93/11/22 12:25:52 rws Exp $ */

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
#ifndef _LINE_H
#define _LINE_H

static PEXCoord points[] = {{0.2, 0.2, 0.0}, {0.4, 0.7, 0.0},
                        {0.6, 0.3, 0.0}, {0.8, 0.8, 0.0}};

#define SET_LINE_COLOR(_colorType,_colr)	\
    PEXSetLineColor(dpy, resourceID, req_type, (_colorType), (_colr));

#define SET_LINE_COLOR_INDEX(_colorIndex)	\
    PEXSetLineColorIndex(dpy, resourceID, req_type, (_colorIndex));

#define SET_LINE_WIDTH(_width)	\
    PEXSetLineWidth(dpy, resourceID, req_type, (_width));

#define SET_LINE_TYPE(_type)	\
    PEXSetLineType(dpy, resourceID, req_type, (_type));

#define SET_LINE_BUNDLE_INDEX(_index)	\
    PEXSetLineBundleIndex(dpy, resourceID, req_type, (_index));

#define SET_LINE_INTERP_METHOD(_method)	\
    PEXSetPolylineInterpMethod(dpy, resourceID, req_type, (_method));

#define DRAW_Polyline \
    PEXPolyline(dpy, resourceID, req_type, 4, points);

#define POLYLINE(_count,_data) \
    PEXPolyline(dpy, resourceID, req_type, (_count), (_data));

#define POLYLINE2D(_count,_data) \
    PEXPolyline2D(dpy, resourceID, req_type, (_count), (_data));

#define POLYLINESETWITHDATA(_vattrs,_colorType,_count,_vdata) \
    PEXPolylineSetWithData(dpy, resourceID, req_type, (_vattrs), (_colorType), \
	(_count), (_vdata));

#define TRANSLATE(_x,_y) \
{                       \
    vector.x = _x;      \
    vector.y = _y;      \
    PEXTranslate(&vector, matrix); \
}

#define SET_LOCAL_TRANSFORM(comp_type, mat)     \
    PEXSetLocalTransform(dpy, resourceID, req_type, comp_type, mat)
 
#define SET_GLOBAL_TRANSFORM(mat)       \
    PEXSetGlobalTransform(dpy, resourceID, req_type, mat)
 

#endif /* _LINE_H */
