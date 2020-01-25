/* $XConsortium: hlhsr_z.c,v 1.0 93/11/22 12:54:12 rws Exp $ */

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
#include "surface.h"
#include "line.h"
 
char test_description[] = "\
HLHSR (ZBufferID) Tests:\n\n\
The image is composed of a red square in the center,\n\
a while line on the right side of the square (hidden left half is removed)\n\
and a white trapezoid (which is really a triangle with hidden upper half\n\
removed).\n\
"; 

PEXMatrix matrix;
PEXVector vector;
 
void
outputCommands(dpy, resourceID, req_type)
    Display		*dpy;
    XID			resourceID;
    PEXOCRequestType	req_type;
{	
    PEXColor            colr;
    int                 i;
 
    static              PEXCoord fa1[] = {
        {0.3, 0.3, 0.5},
        {0.3, 0.7, 0.5},
        {0.7, 0.7, 0.5},
        {0.7, 0.3, 0.5}
    };
    static              PEXCoord fa2[] = {
        {0.4, 0.4, 0.7},
        {0.5, 0.5, 0.3},
        {0.6, 0.4, 0.7}
    };
    static              PEXCoord line[] = {
        {0.4, 0.6, 0.3},
        {0.6, 0.6, 0.7}
    };
       
    PEXSetHLHSRID(dpy, resourceID, req_type, PEXHLHSRIDEnable);
    SET_INTERIOR_STYLE(PEXInteriorStyleSolid)
    POLYLINE(2,line);
    FILLAREA(PEXShapeConvex, False, 3, fa2);
    SET_SURFACE_COLOR_INDEX(2)
    FILLAREA(PEXShapeConvex, False, 4, fa1);
}
 
void
testproc(dpy,window,x,y,w,h)
    Display		*dpy;
    Window		window;
    int			x,y,w,h;
{
    image_testproc("hlhsr_z", "HLHSR w/ Z_BufferID", test_description,
        dpy,window,x,y,w,h);
} 

void
misc_setup(dpy, window, renderer)
Display		*dpy;
Window		window;
PEXRenderer	renderer;
{
    PEXRendererAttributes values;
    values.hlhsr_mode = PEXHLHSRZBufferID;
    PEXChangeRenderer(dpy, renderer, PEXRAHLHSRMode, &values);
}
