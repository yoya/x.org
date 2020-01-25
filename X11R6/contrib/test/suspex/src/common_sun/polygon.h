/* $XConsortium: polygon.h,v 1.0 93/11/22 12:26:21 rws Exp $ */

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
#ifndef _POLYGON_H
#define _POLYGON_H


static PEXCoord square[] = {{0.2, 0.2, 0.0}, {0.3, 0.2, 0.0},
                        {0.3, 0.3, 0.0}, {0.2, 0.3, 0.0}};
static int square_np = sizeof(square) / sizeof(PEXCoord);
 

static  PEXColorRGB red_square_facet_rgb = {1.0, 0.0, 0.0};
	 
static  PEXVertexRGB square_vertex_rgb[4] = {
		 {{0.4, 0.4, 0.0}, {1.0, 0.0, 0.0}},
		 {{0.5, 0.4, 0.0}, {0.0, 1.0, 0.0}},
		 {{0.5, 0.5, 0.0}, {0.0, 0.0, 1.0}},
		 {{0.4, 0.5, 0.0}, {0.0, 0.0, 0.0}}};

#define DRAW_SOLID_POLYGON 					\
{								\
	PEXSetInteriorStyle(dpy, renderer, req_type,	\
		PEXInteriorStyleSolid);				\
}

#define DRAW_SHADED_POLYGON 					\
    PEXSetSurfaceInterpMethod(dpy, renderer, req_type,		\
	PEXSurfaceInterpColor);
	

#define DRAW_SQUARE_FillArea \
    PEXFillArea(dpy, renderer, req_type, PEXShapeConvex, 	\
        True, square_np, square);
 

#define DRAW_SQUARE_FillAreaWithData \
{								\
    PEXArrayOfVertex vertices;					\
    vertices.rgb = square_vertex_rgb;				\
    PEXFillAreaWithData(dpy, renderer, req_type, PEXShapeConvex, \
        True, PEXGAColor, PEXGAColor, PEXColorTypeRGB,		\
        &red_square_facet_rgb, square_np, vertices);		\
}


#endif /* _POLYGON_H */
