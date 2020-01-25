/* $XConsortium: clipList.c,v 1.0 93/11/22 12:53:43 rws Exp $ */

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
#include "misc.h"
 
char test_description[] = "\
Clip Lists Tests:\n\n\
You should see a small red square on upper left, a small red rectangle on \n\
lower left and a red area covering the right half of the screen.\n\
This image is created by specifying 3 clip areas on a large fillarea that\n\
covers the entire viewport.\n\
"; 

void
outputCommands(dpy, resourceID, req_type)
    Display		*dpy;
    XID			resourceID;
    PEXOCRequestType	req_type;
{	
    PEXColor		colr;
    int			i;

    static		PEXCoord pts1[] = { 
	0.0, 0.0, 0.0,
	1.0, 0.0, 0.0,
	1.0, 1.0, 0.0,
	0.0, 1.0, 0.0,
    };

    SET_INTERIOR_STYLE(PEXInteriorStyleSolid)
    SET_SURFACE_COLOR_INDEX(2)
    FILLAREA(PEXShapeConvex, False, 4, pts1);
}

void
testproc(dpy,window,x,y,w,h)
    Display		*dpy;
    Window		window;
    int			w,h;
{
    image_testproc("clipList", "renderer clip list", test_description,
	dpy, window, x,y,w,h);
} 


void
misc_setup(dpy, window, renderer)
Display		*dpy;
Window		window;
PEXRenderer	renderer;
{
    PEXRendererAttributes attrs;
    PEXListOfClipRect  	cliplist;
    static PEXDeviceRect rects[] = {
	{100,0,200,200},
	{10,10,50,30},
	{10,160,30,180},
    };

    cliplist.count = 3;
    cliplist.rectangles = rects;

    attrs.clip_list = cliplist;
    PEXChangeRenderer(dpy, renderer, PEXRAClipList, &attrs);
}
