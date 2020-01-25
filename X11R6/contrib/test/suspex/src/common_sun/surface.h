/* $XConsortium: surface.h,v 1.0 93/11/22 12:26:26 rws Exp $ */

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
#ifndef _SURFACE_H
#define _SURFACE_H


/* Surface Primitives */

#define FILLAREA(_shape, _ignoreEdge, _count, _data) \
    PEXFillArea(dpy, resourceID, req_type, (_shape), 	\
        (_ignoreEdge), (_count), (_data));

#define FILLAREA2D(_shape, _ignoreEdge, _count, _data) \
    PEXFillArea2D(dpy, resourceID, req_type, (_shape), 	\
        (_ignoreEdge), (_count), (_data));

#define FILLAREA_SET(_shape, _ignoreEdge, _contour, _count, _data) \
    PEXFillAreaSet(dpy, resourceID, req_type, (_shape), 	\
        (_ignoreEdge), (_contour), (_count), (_data));

#define FILLAREA_SET2D(_shape, _ignoreEdge, _contour, _count, _data) \
    PEXFillAreaSet2D(dpy, resourceID, req_type, (_shape), 	\
        (_ignoreEdge), (_contour), (_count), (_data));

#define FILLAREA_WITH_DATA(_shape, _ignore, _fattrs, _vattrs, _ctype, _fdata, _count, _vdata) \
    PEXFillAreaWithData(dpy, resourceID, req_type, (_shape), (_ignore),\
 	(_fattrs), (_vattrs), (_ctype), (_fdata), (_count), (_vdata));

#define FILLAREA_SET_WITH_DATA(_shape, _ignore, _contour, _fattrs, _vattrs, _ctype, _count,  _fdata, _vdata) \
    PEXFillAreaSetWithData(dpy, resourceID, req_type, (_shape), \
	(_ignore), (_contour), (_fattrs), (_vattrs), (_ctype), \
	(_count), (_fdata), (_vdata));

#define SETOF_FILLAREA_SETS(_shape, _fattrs, _vattrs, _eattrs, _contour, _allone, _ctype, _scount, _fdata, _vcount, _vdata, _icount, _eflags, _cdata) \
    PEXSetOfFillAreaSets(dpy, resourceID, req_type, (_shape), \
	(_fattrs), (_vattrs), (_eattrs), (_contour), (_allone), \
	(_ctype), (_scount), (_fdata), (_vcount), (_vdata), (_icount), \
	(_eflags), (_cdata));

#define TRIANGLE_STRIP(_fattrs, _vattrs, _ctype, _fdata, _count, _vdata) \
    PEXTriangleStrip(dpy, resourceID, req_type, \
 	(_fattrs), (_vattrs), (_ctype), (_fdata), (_count), (_vdata));

#define QUADRILATERAL_MESH(_shape, _fattrs, _vattrs, _ctype, _fdata, _ccount, _rcount, _vdata) \
    PEXQuadrilateralMesh(dpy, resourceID, req_type, (_shape), (_fattrs), \
	(_vattrs), (_ctype), (_fdata), (_ccount), (_rcount), (_vdata));


/* Front surface attributes */

#define SET_INTERIOR_STYLE(_style)	\
    PEXSetInteriorStyle(dpy, resourceID, req_type, (_style));

#define SET_INTERIOR_STYLE_INDEX(_style)	\
    PEXSetInteriorStyleIndex(dpy, resourceID, req_type, (_style));

#define SET_SURFACE_COLOR(_colorType,_colr)	\
    PEXSetSurfaceColor(dpy, resourceID, req_type, (_colorType), (_colr));

#define SET_SURFACE_COLOR_INDEX(_colorIndex)	\
    PEXSetSurfaceColorIndex(dpy, resourceID, req_type, (_colorIndex));

#define SET_SURFACE_INTERP_METHOD(_method)	\
    PEXSetSurfaceInterpMethod(dpy, resourceID, req_type, (_method)); 


/* Back surface attributes */

#define SET_BF_INTERIOR_STYLE(_style)	\
    PEXSetBFInteriorStyle(dpy, resourceID, req_type, (_style));

#define SET_BF_INTERIOR_STYLE_INDEX(_style)	\
    PEXSetBFInteriorStyleIndex(dpy, resourceID, req_type, (_style));

#define SET_BF_SURFACE_COLOR(_colorType,_colr)	\
    PEXSetBFSurfaceColor(dpy, resourceID, req_type, (_colorType), (_colr));

#define SET_BF_SURFACE_COLOR_INDEX(_colorIndex)	\
    PEXSetBFSurfaceColorIndex(dpy, resourceID, req_type, (_colorIndex));

#define SET_BF_SURFACE_INTERP_METHOD(_method)	\
    PEXSetBFSurfaceInterpMethod(dpy, resourceID, req_type, (_method)); 


/* Misc surface attributes */

#define SET_INTERIOR_BUNDLE_INDEX(_index)	\
    PEXSetInteriorBundleIndex(dpy, resourceID, req_type, (_index));

#define SET_FACET_CULLING_MODE(_mode)	\
    PEXSetFacetCullingMode(dpy, resourceID, req_type, (_method));

#define SET_FACET_DISTINGUISH_FLAG(_flag)	\
    PEXSetFacetDistinguishFlag(dpy, resourceID, req_type, (_flag));

#define SET_SURFACE_APPROX(_method, _utolerance, _vtolerance)	\
    PEXSetSurfaceApprox(dpy, resourceID, req_type, (_method), \
	(_utolerance), (_vtolerance));


/* Edge attributes */

#define SET_EDGE_COLOR(_colorType,_colr)	\
    PEXSetSurfaceEdgeColor(dpy, resourceID, req_type, (_colorType), (_colr));

#define SET_EDGE_COLOR_INDEX(_colorIndex)	\
    PEXSetSurfaceEdgeColorIndex(dpy, resourceID, req_type, (_colorIndex));

#define SET_EDGE_WIDTH(_width)	\
    PEXSetSurfaceEdgeWidth(dpy, resourceID, req_type, (_width));

#define SET_EDGE_TYPE(_type)	\
    PEXSetSurfaceEdgeType(dpy, resourceID, req_type, (_type));

#define SET_EDGE_FLAG(_flag)	\
    PEXSetSurfaceEdgeFlag(dpy, resourceID, req_type, (_flag));

#define SET_EDGE_BUNDLE_INDEX(_index)	\
    PEXSetEdgeBundleIndex(dpy, resourceID, req_type, (_index));

#endif /* _SURFACE_H */
