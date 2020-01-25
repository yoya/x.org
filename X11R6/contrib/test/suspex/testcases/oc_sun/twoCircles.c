/* $XConsortium: twoCircles.c,v 1.0 93/11/22 12:51:42 rws Exp $ */

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
#include <X11/PEX5/PEXlib.h>
#include <stdio.h>
#include <math.h>
/* Depending on the OS version, math.h may not have defined M_PI, so ... */
#ifndef M_PI
#define  M_PI    3.14159265358979323846
#endif	/* !M_PI */

#define NUM_CIRCLE_SEGMENTS     40

#define CIRCLE(x_center,y_center,radius,num_segments,points) \
{								\
    double delta, angle;					\
    int i;							\
    angle = 0.0;						\
    delta = 2 * M_PI / num_segments;				\
    for ( i = 0; i < num_segments; i++, angle += delta ) {	\
        points[i].x = x_center + radius * cos( angle );		\
        points[i].y = y_center + radius * sin( angle );		\
	points[i].z = 0;					\
    }								\
}

#define SET_COLOR(r,g,b,c) { 					\
        (c).rgb.red = (r); \
        (c).rgb.green = (g); \
        (c).rgb.blue = (b);}

char test_description[] = "\
FillareaSet Tests:\n\n\
You should see a donut shaped FillareaSet in white with a green edge\n\
";
 

void
outputCommands(dpy, resourceID, req_type)
    Display		*dpy;
    XID			resourceID;
    PEXOCRequestType	req_type;
{	
    PEXColor            color;
    PEXCoord            large[NUM_CIRCLE_SEGMENTS];
    PEXCoord            small[NUM_CIRCLE_SEGMENTS];
    PEXListOfCoord      contours[2];
    
    /* Build the contour list -- 2 contours. */
    contours[0].count = NUM_CIRCLE_SEGMENTS;
    contours[0].points = large;
    contours[1].count = NUM_CIRCLE_SEGMENTS;
    contours[1].points = small;
    /* Set the interior style and color. */
    PEXSetInteriorStyle( dpy, resourceID, req_type,
                         PEXInteriorStyleSolid );

    SET_COLOR( 1, 1, 1, color ); /* white */
    PEXSetSurfaceColor( dpy, resourceID, req_type,
                        PEXColorTypeRGB, &color );

    /* Set the interior style and color. */
    /* Turn edges on and set their color. */
    PEXSetSurfaceEdgeFlag( dpy, resourceID, req_type, PEXOn );
    SET_COLOR( 0, 1, 0, color ); /* green */
    PEXSetSurfaceEdgeColor( dpy, resourceID, req_type,
                            PEXColorTypeRGB, &color );
         
    /* Generate the two CIRCLEs fully overlapping. */
    CIRCLE( 0.15, 0.5, 0.1, NUM_CIRCLE_SEGMENTS, large );
    CIRCLE( 0.15, 0.5, 0.04, NUM_CIRCLE_SEGMENTS, small );
    PEXFillAreaSet( dpy, resourceID, req_type, PEXShapeConvex,
                    False, PEXContourNested, 2, contours );
}
 
void
testproc(dpy,window,x,y,w,h)
    Display		*dpy;
    Window		window;
    int			x,y,w,h;
{
    image_testproc("twoCircles", "CIRCLE drawn w/ fill area set", 
	test_description, dpy,window,x,y,w,h);
} 

void
misc_setup(dpy, window, renderer)
Display		*dpy;
Window		window;
PEXRenderer	renderer;
{
}
