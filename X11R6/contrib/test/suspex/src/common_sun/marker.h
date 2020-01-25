/* $XConsortium: marker.h,v 1.0 93/11/22 12:26:04 rws Exp $ */

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
#ifndef _MARKER_H
#define _MARKER_H

#define SET_MARKER_COLOR(_colorType,_colr)	\
    PEXSetMarkerColor(dpy, resourceID, req_type, (_colorType), (_colr));

#define SET_MARKER_COLOR_INDEX(_colorIndex)	\
    PEXSetMarkerColorIndex(dpy, resourceID, req_type, (_colorIndex));

#define SET_MARKER_SCALE(_scale)	\
    PEXSetMarkerScale(dpy, resourceID, req_type, (_scale));

#define SET_MARKER_TYPE(_type)	\
    PEXSetMarkerType(dpy, resourceID, req_type, (_type));

#define SET_MARKER_BUNDLE_INDEX(_index)	\
    PEXSetMarkerBundleIndex(dpy, resourceID, req_type, (_index));

#define MARKER(_count,_data) \
    PEXMarkers(dpy, resourceID, req_type, (_count), (_data));

#define MARKER2D(_count,_data) \
    PEXMarkers2D(dpy, resourceID, req_type, (_count), (_data));

#endif /* _MARKER_H */
