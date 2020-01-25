/* $XConsortium: lineAttr.c,v 1.0 93/11/22 12:50:16 rws Exp $ */

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
Line Attributes Tests:\n\n\
Top:    horizontal line rendered with default attributes\n\
Mid:    4 blue lines of solid, dashed dotted, and dashdot respectively,\n\
        then a red wide dashdot rendered using bundled attributes.\n\
Bottom: 10 lines. The first is rendered with color index 0 and is invisible\n\
        the next 8 lines are rendered with increasing line width and color \n\
        index from 1 - 8. The next line is specified with unsupported \n\
        attributes and should rendered as a default white solid line.\n\
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

    static		PEXCoord2D pts2[] = { 
	0.0, 0.5,
	0.05, 0.6,
	0.1, 0.4,
	0.125, 0.75,
    };

    static		PEXCoord pts3[] = { 
	0.1, 0.8, 0.0, 0.3, 0.8, 0.0,
    };

    static int		lineType[] = {
	PEXLineTypeSolid,
	PEXLineTypeDashed,
	PEXLineTypeDotted,
	PEXLineTypeDashDot,
    };

    colr.rgb.red = 0.0;
    colr.rgb.green = 0.0;
    colr.rgb.blue = 1.0;

/* default line attrs */

    POLYLINE(2, pts3);

/* combinations of various line colour index and various line width */

    for (i=0; i < 9; ++i ) {
    	SET_LINE_COLOR_INDEX((unsigned int)i);
        SET_LINE_WIDTH((double) i);
	POLYLINE(3, pts1);
	TRANSLATE_VERTEX(x, 0.1, 3, pts1)
    }

/* line width = 0.0, type = -1, color index = 128 */

    SET_LINE_WIDTH(0.0);
    SET_LINE_TYPE(-1);
    SET_LINE_COLOR_INDEX(128);
    POLYLINE(3, pts1);

/* line colour */

    SET_LINE_COLOR(PEXColorTypeRGB,&colr);

/* various line types with 2D lines */

    SET_LINE_WIDTH(2.0);
    for (i=0; i < sizeof(lineType)/sizeof(int); ++i ) {
    	SET_LINE_TYPE(lineType[i])
	POLYLINE2D(4, pts2)
	TRANSLATE_VERTEX(x, 0.2, 4, pts2)
    }

/* line bundle */
    SET_LINE_BUNDLE_INDEX(1);
    SET_INDIVIDUAL_ASF(PEXASFLineType,PEXIndividual);
    SET_INDIVIDUAL_ASF(PEXASFLineWidth,PEXBundled);
    SET_INDIVIDUAL_ASF(PEXASFLineColor,PEXBundled);
    POLYLINE2D(4, pts2);
}
 
void
testproc(dpy,window,x,y,w,h)
    Display		*dpy;
    Window		window;
    int			x,y,w,h;
{
    image_testproc("lineAttrs", "line colr index,width,type", 
	test_description, dpy,window,x,y,w,h);
} 

void
misc_setup(dpy, window, renderer)
Display		*dpy;
Window		window;
PEXRenderer	renderer;
{
}
