/* $XConsortium: faset.c,v 1.0 93/11/22 12:49:47 rws Exp $ */

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

char test_description[] = "\
FillareaSet Tests:\n\n\
Top:    A FillareaSet primitive consists of a nested square and another square.\n\
        Default interior style(hollow) and color (white).\n\
Bottom: A 2D FillareaSet primitive that looks the same as the Top one.\n\
";

static PEXListOfCoord c[3];
static PEXListOfCoord2D c2[3];
static PEXCoord pl1[] = {{0.1, 0.1, 0.0}, {0.4, 0.1, 0.0},
			 {0.4, 0.4, 0.0}, {0.1, 0.4, 0.0}};
static PEXCoord pl2[] = {{0.2, 0.2, 0.0}, {0.3, 0.2, 0.0},
			 {0.3, 0.3, 0.0}, {0.2, 0.3, 0.0}};
static PEXCoord pl3[] = {{0.6, 0.2, 0.0}, {0.7, 0.2, 0.0},
			 {0.7, 0.3, 0.0}, {0.6, 0.3, 0.0}};

static PEXCoord2D pl4[] = {{0.1, 0.6}, {0.4, 0.6},
			 {0.4, 0.9}, {0.1, 0.9}};
static PEXCoord2D pl5[] = {{0.2, 0.7}, {0.3, 0.7},
			 {0.3, 0.8}, {0.2, 0.8}};
static PEXCoord2D pl6[] = {{0.6, 0.7}, {0.7, 0.7},
			 {0.7, 0.8}, {0.6, 0.8}};

void
outputCommands(dpy, resourceID, req_type)
    Display             *dpy;
    unsigned long       resourceID; 
    PEXOCRequestType    req_type;
{	
    c[0].count = 4;
    c[0].points = pl1;
    c[1].count = 4;
    c[1].points = pl2;
    c[2].count = 4;
    c[2].points = pl3;
    PEXFillAreaSet(dpy, resourceID, req_type, PEXShapeUnknown, True,
	PEXContourUnknown, 3, c);
    c2[0].count = 4;
    c2[0].points = pl4;
    c2[1].count = 4;
    c2[1].points = pl5;
    c2[2].count = 4;
    c2[2].points = pl6;
    PEXFillAreaSet2D(dpy, resourceID, req_type, PEXShapeUnknown, True,
	PEXContourUnknown, 3, c2);
}
 
void
testproc(dpy,window,x,y,w,h)
    Display		*dpy;
    Window		window;
    int			x,y,w,h;
{
    image_testproc("faset", "fill area set", test_description,
        dpy,window,x,y,w,h);
} 

void
misc_setup(dpy, window, renderer)
Display		*dpy;
Window		window;
PEXRenderer	renderer;
{
}
