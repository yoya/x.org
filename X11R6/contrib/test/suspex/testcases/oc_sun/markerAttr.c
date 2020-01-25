/* $XConsortium: markerAttr.c,v 1.0 93/11/22 12:50:33 rws Exp $ */

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
#include "marker.h"
#include "misc.h"

char test_description[] = "\
Marker Attributes Tests:\n\n\
Top:    First 5 markers in blue - dot, cross, asterisk, circle and X.\n\
        The last one is an large X in red (bundled attributes).\n\
Middle: Same as top row.\n\
Bottom: 7 markers - First is a default marker (white asterisk)\n\
        Next five are asterisks in increasing size from 0 - 4\n\
        Last one is specified with unsupported type, and should rendered\n\
        as default.\n\
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
	0.05, 0.1, 0.0,
    };

    static		PEXCoord2D pts2[] = { 
	0.1, 0.5,
	0.1, 0.7,
    };

    static int		markerType[] = {
	PEXMarkerDot,
	PEXMarkerCross,
	PEXMarkerAsterisk,
	PEXMarkerCircle,
	PEXMarkerX,
    };

    colr.rgb.red = 0.0;
    colr.rgb.green = 0.0;
    colr.rgb.blue = 1.0;

/* default marker attrs */

    MARKER(1, pts1)

/* marker colour index */
    SET_MARKER_COLOR_INDEX(3)

/* various marker scale */
    for (i=0; i < 5; ++i ) {
	TRANSLATE_VERTEX(x, 0.15, 1, pts1)
        SET_MARKER_SCALE((double)i)
	MARKER(1, pts1)
    }

/* type = -1, color index = 128 */

    SET_MARKER_TYPE(-1);
    SET_MARKER_COLOR_INDEX(128);
    TRANSLATE_VERTEX(x, 0.15, 1, pts1)
    MARKER(1, pts1);

/* marker colour */

    SET_MARKER_SCALE(1.0);
    SET_MARKER_COLOR(PEXColorTypeRGB,&colr);

/* various marker types with 2D lines */

    for (i=0; i < sizeof(markerType)/sizeof(int); ++i ) {
    	SET_MARKER_TYPE(markerType[i])
	MARKER2D(2, pts2)
	TRANSLATE_VERTEX(x, 0.1, 2, pts2)
    }

/* marker bundle */
    SET_MARKER_BUNDLE_INDEX(1)
    SET_INDIVIDUAL_ASF(PEXASFMarkerType,PEXIndividual)
    SET_INDIVIDUAL_ASF(PEXASFMarkerScale,PEXBundled)
    SET_INDIVIDUAL_ASF(PEXASFMarkerColor,PEXBundled)
    MARKER2D(2, pts2)
}
 
void
testproc(dpy,window,x,y,w,h)
    Display		*dpy;
    Window		window;
    int			x,y,w,h;
{
    image_testproc("markerAttrs","marker attributes", test_description,
		dpy,window,x,y,w,h);
} 

void
misc_setup(dpy, window, renderer)
Display		*dpy;
Window		window;
PEXRenderer	renderer;
{
}
