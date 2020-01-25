/* $XConsortium: geometry.h,v 1.0 93/11/22 12:25:46 rws Exp $ */

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
#ifndef _GEOMETRY_H
#define _GEOMETRY_H

/* front facing */
static	PEXCoord pyramid_pts[][3] = {
	{{0.0,0.95,0.0}, {0.15,0.85,0.5}, {0.3,0.95,0.0}},
	{{0.3,0.75,0.0}, {0.3,0.95,0.0}, {0.15,0.85,0.5}},
	{{0.3,0.75,0.0}, {0.15,0.85,0.5}, {0.0,0.75,0.0}},
	{{0.0,0.95,0.0}, {0.0,0.75,0.0}, {0.15,0.85,0.5}},
};

/* front facing */
static	PEXCoord pyramid_pts_quadmesh[][4] = {
        {{0.0,0.95,0.0}, {0.3,0.95,0.0}, {0.15,0.85,0.5}, {0.15,0.85,0.5}},
	{{0.3,0.95,0.0}, {0.3,0.75,0.0}, {0.15,0.85,0.5}, {0.15,0.85,0.5}},
	{{0.15,0.85,0.5}, {0.15,0.85,0.5}, {0.0,0.75,0.0}, {0.3,0.75,0.0}}, 
	{{0.0,0.75,0.0}, {0.0,0.95,0.0}, {0.15,0.85,0.5}, {0.15,0.85,0.5}},
};

static  PEXVector pyramid_fnorm[] =  {
	{0.0, 0.9806, 0.1961 },
	{0.9285, 0.0, 0.3714 },
	{0.0, -0.9806, 0.1961 },
	{-0.9285, 0.0, 0.3714 },
};

#endif /* _GEOMETRY_H */
