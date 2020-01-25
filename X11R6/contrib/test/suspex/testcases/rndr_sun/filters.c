/* $XConsortium: filters.c,v 1.0 93/11/22 12:54:06 rws Exp $ */

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
Highlight/Invisibility Filter Tests:\n\n\
(From Bottom to Top)\n\
1st - 3rd row: red line\n\
4th row: invisible line\n\
5th row: red line\n\
6th row: highlighted line \n\
7th row: red line\n\
8th row: highlighted line\n\
9th row: red line\n\
"; 

static PEXCoord points1[] = {{0.1, 0.1, 0.0}, {0.5, 0.1, 0.0}};
static PEXCoord points2[] = {{0.1, 0.2, 0.0}, {0.5, 0.2, 0.0}};
static PEXCoord points3[] = {{0.1, 0.3, 0.0}, {0.5, 0.3, 0.0}};
static PEXCoord points4[] = {{0.1, 0.4, 0.0}, {0.5, 0.4, 0.0}};
static PEXCoord points5[] = {{0.1, 0.5, 0.0}, {0.5, 0.5, 0.0}};
static PEXCoord points6[] = {{0.1, 0.6, 0.0}, {0.5, 0.6, 0.0}};
static PEXCoord points7[] = {{0.1, 0.7, 0.0}, {0.5, 0.7, 0.0}};
static PEXCoord points8[] = {{0.1, 0.8, 0.0}, {0.5, 0.8, 0.0}};
static PEXCoord points9[] = {{0.1, 0.9, 0.0}, {0.5, 0.9, 0.0}};

void
outputCommands(dpy, resourceID, req_type)
    Display		*dpy;
    unsigned long	resourceID;
    PEXOCRequestType	req_type;
{	
    PEXColor		colr;
    PEXName		names[10];

    PEXSetLineColorIndex(dpy, resourceID, req_type, 2);
    /* Draw 3 line segments */
    names[0] = 1;
    PEXAddToNameSet(dpy, resourceID, req_type, 1, names);
    DRAW_Polyline(2,points1);
    names[0] = 21;
    PEXAddToNameSet(dpy, resourceID, req_type, 1, names);
    DRAW_Polyline(2,points2);
    names[0] = 1;
    names[1] = 21;
    PEXRemoveFromNameSet(dpy, resourceID, req_type, 2, names);
    DRAW_Polyline(2,points3);

    /* Don't draw 1 line segment */
    names[0] = 11;
    PEXAddToNameSet(dpy, resourceID, req_type, 1, names);
    DRAW_Polyline(2,points4);

    /* Draw 1 line segment */
    names[0] = 21;
    PEXAddToNameSet(dpy, resourceID, req_type, 1, names);
    DRAW_Polyline(2,points5);

    /* Draw 1 highlighted line segment */
    names[0] = 11;
    PEXRemoveFromNameSet(dpy, resourceID, req_type, 1, names);
    names[0] = 31;
    PEXAddToNameSet(dpy, resourceID, req_type, 1, names);
    DRAW_Polyline(2,points6);

    /* Draw 1 UN-highlighted line segment */
    names[0] = 41;
    PEXAddToNameSet(dpy, resourceID, req_type, 1, names);
    DRAW_Polyline(2,points7);

    /* Draw 1 highlighted line segment */
    names[0] = 41;
    PEXRemoveFromNameSet(dpy, resourceID, req_type, 1, names);
    DRAW_Polyline(2,points8);

    /* Draw 1 UN-highlighted line segment */
    names[0] = 31;
    PEXRemoveFromNameSet(dpy, resourceID, req_type, 1, names);
    DRAW_Polyline(2,points9);
}
 
void
testproc(dpy,window,x,y,w,h)
    Display		*dpy;
    Window		window;
    int			x,y,w,h;
{
    image_testproc("filters", "filters", test_description,
        dpy,window,x,y,w,h);
} 

void
misc_setup(dpy, window, renderer)
Display		*dpy;
Window		window;
PEXRenderer	renderer;
{
    XID                 	id1, id2, id3, id4;
    PEXName             	names[2];
    PEXRendererAttributes	ra;

    id1 = PEXCreateNameSet(dpy);
    names[0] = 11;
    names[1] = 12;
    PEXChangeNameSet(dpy, id1, PEXNSAdd, 2, names);
    id2 = PEXCreateNameSet(dpy);
    names[0] = 21;
    names[1] = 22;
    PEXChangeNameSet(dpy, id2, PEXNSAdd, 2, names);
    id3 = PEXCreateNameSet(dpy);
    names[0] = 31;
    names[1] = 32;
    PEXChangeNameSet(dpy, id3, PEXNSAdd, 2, names);
    id4 = PEXCreateNameSet(dpy);
    names[0] = 41;
    names[1] = 42;
    PEXChangeNameSet(dpy, id4, PEXNSAdd, 2, names);

    ra.invisibility_incl = id1;
    ra.invisibility_excl = id2;
    ra.highlight_incl = id3;
    ra.highlight_excl = id4;
    PEXChangeRenderer(dpy, renderer, 
	PEXRAInvisibilityIncl | PEXRAInvisibilityExcl | 
	PEXRAHighlightIncl | PEXRAHighlightExcl,
	&ra);
}

