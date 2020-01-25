/* $XConsortium: shareDraw.c,v 1.0 93/11/22 12:54:35 rws Exp $ */

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
#include "line.h"
#include "misc.h"
 
char test_description[] = "\
Share Drawable Tests:\n\n\
The image consists of a thick red solid line and a thick blue dotted line\n\
which are rendered using different renderers.\n\
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
	0.0, 0.3, 0.0,
	0.1, 0.2, 0.0,
	0.0, 0.1, 0.0,
    };

    SET_LINE_TYPE(PEXLineTypeDotted)
    SET_LINE_COLOR_INDEX(4);
    SET_LINE_WIDTH(3.0);
    POLYLINE(3, pts1);
}

void
outputCommands2(dpy, resourceID, req_type)
    Display		*dpy;
    XID			resourceID;
    PEXOCRequestType	req_type;
{	
    PEXColor		colr;
    int			i;

    static		PEXCoord pts1[] = { 
	0.0, 0.6, 0.0,
	0.1, 0.5, 0.0,
	0.0, 0.4, 0.0,
    };

    SET_LINE_BUNDLE_INDEX(1);
    SET_INDIVIDUAL_ASF(PEXASFLineType,PEXIndividual);
    SET_INDIVIDUAL_ASF(PEXASFLineWidth,PEXBundled);
    SET_INDIVIDUAL_ASF(PEXASFLineColor,PEXBundled);
    POLYLINE(3, pts1);
}
 
void
testproc(dpy,window,x,y,w,h)
    Display		*dpy;
    Window		window;
    int			x,y,w,h;
{
    image_testproc("shareDraw", "sharing drawable between renderers",
	test_description, dpy,window,x,y,w,h);
} 

void
misc_setup(dpy, window, renderer)
Display         *dpy;
Window          window;
PEXRenderer     renderer;
{
    PEXRenderer		renderer2;
    extern PEXRenderer	setup_renderer();

    renderer2 = setup_renderer(dpy, window);
    PEXBeginRendering(dpy, window, renderer2);
    outputCommands2(dpy, (XID)renderer2, PEXOCRender);
    PEXEndRendering(dpy, renderer2, True);
}
