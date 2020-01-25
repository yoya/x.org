/* $XConsortium: misc.h,v 1.0 93/11/22 12:26:09 rws Exp $ */

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
#ifndef _MISC_H
#define _MISC_H


#define SET_INDIVIDUAL_ASF(_attr,_asf)	\
    PEXSetIndividualASF(dpy, resourceID, req_type, (_attr), (_asf));

#define SET_LIGHT_SOURCE_STATE(_ecount, _enable, _dcount, _disable)	\
    PEXSetLightSourceState(dpy, resourceID, req_type, \
	(_ecount),(_enable),(_dcount),(_disable));

#define SET_REFLECTION_ATTRIBUTES(_attributes)	\
    PEXSetReflectionAttributes(dpy, resourceID, req_type, (_attributes));

#define SET_REFLECTION_MODEL(_model)	\
    PEXSetReflectionModel(dpy, resourceID, req_type, (_model));

#define SET_DEPTH_CUE_INDEX(_index)	\
    PEXSetDepthCueIndex(dpy, resourceID, req_type, (_index));

#define TRANSLATE_VERTEX(_which, _offset, _count, _vertex) { \
    int _i; \
    for (_i=0; _i< (_count); ++_i) \
	(_vertex)[_i]._which += (_offset); \
    }

#define SETUP_VECTOR_LIGHT \
{ 									\
	static PEXReflectionAttributes reflattr = {0.3,1.0,0.0,0.0,0.0};\
	static PEXTableIndex enable[] = {1,2};				\
									\
	SET_REFLECTION_MODEL(PEXReflectionDiffuse)			\
	SET_LIGHT_SOURCE_STATE(2,enable,0,NULL)				\
	SET_REFLECTION_ATTRIBUTES(&reflattr)				\
}

#endif /* _MISC_H */
