/* $XConsortium: cell.c,v 1.0 93/11/22 12:49:12 rws Exp $ */

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
#include "oc.h"


char test_description[] = "\
Cell Array Tests:\n\n\
Top:    CellArray2D\n\
Bottom: CellArray, ExtendedCellArray\n\
";

static PEXCoord points[] = {{0.1, 0.1, 0.0}, {0.4, 0.1, 0.0},
                        {0.1, 0.4, 0.0}};
 
static PEXCoord2D points2[] = {{0.5, 0.1}, {0.9, 0.4}};
 
static PEXCoord points3[] = {{0.1, 0.5, 0.0}, {0.6, 0.5, 0.0},
                        {0.1, 0.9, 0.0}};
 
 
PEXMatrix matrix;
PEXVector vector;

outputCommands(dpy, resourceID, req_type)
    Display	*dpy;
    unsigned long	resourceID;
    PEXOCRequestType	req_type;
		
{
    PEXTableIndex	c1[6], c2[6];
    PEXArrayOfColor	c3;
    PEXColorRGB		rgb[3];

    c1[0] = 1;
    c1[1] = 2;
    c1[2] = 3;
    c1[3] = 4;
    c1[4] = 5;
    c1[5] = 6;
    PEXCellArray(dpy, resourceID, req_type, &points[0], &points[1],
		 &points[2], 2, 3, c1);

    c2[0] = 6;
    c2[1] = 5;
    c2[2] = 4;
    c2[3] = 3;
    c2[4] = 2;
    c2[5] = 1;
    PEXCellArray2D(dpy, resourceID, req_type, &points2[0], &points2[1],
		 3, 2, c2);

    rgb[0].red = 1.0;
    rgb[0].green = 0.0;
    rgb[0].blue = 0.0;
    rgb[1].red = 0.0;
    rgb[1].green = 1.0;
    rgb[1].blue = 0.0;
    rgb[2].red = 0.0;
    rgb[2].green = 0.0;
    rgb[2].blue = 1.0;
    c3.rgb = rgb;
    PEXExtendedCellArray(dpy, resourceID, req_type, &points3[0],
		&points3[1], &points3[2], 1, 3, PEXColorTypeRGB, c3);
}

void
testproc(dpy,window,x,y,w,h)
Display             *dpy;
Window              window;
int                 x,y,w,h;
{
    image_testproc("cell", "cell arrays", test_description, dpy,window,x,y,w,h);
}

void
misc_setup(dpy, window, renderer)
Display         *dpy;
Window          window;
PEXRenderer     renderer;
{
}
