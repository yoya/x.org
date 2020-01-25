/* $XConsortium: atextAttr.c,v 1.0 93/11/22 12:49:06 rws Exp $ */


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
#include "text.h"
#include "marker.h"
#include "line.h"
#include "misc.h"

#define STR_PEX " PEX"
#define STR_3DTEXT "3D atext"
#define STR_BUNDLE "BUNDLE"
#define STRLEN 16

char test_description[] = "\
Annotation Text Attributes Tests:\n\n\
Top:      Red 2D Annotation text showing various kinds of alignment: \n\
          Normal, Center-Half, Right-Bottom, Left-Top, Right-Base, Left-Cap.\n\
          Markers are drawn on the text point for each text primitive.\n\
Center:   1. Red 2D Annotation text rendered using bundled attributes.\n\
             (color and spacing)\n\
          2. Annotation text drawn in blue with white dashed line connecting\n\
	     annotation point and offset.\n\
          3. 2D Annotation text rendered with 2 different up vectors: \n\
             (0,-1),(1,2)\n\
Mid-Left: 2D Annotation text drawn in 4 different paths: \n\
          Top, Down, Right, Left\n\
Bottom:   1. 2D Annotation text drawn with char height: -0.02, 0, 0.04.\n\
          2. 2D Annotation text drawn with char expansion: -0.4, 0, 1.2.\n\
          3. 2D Annotation text drawn with char spacing: -0.5, 0, 1.0.\n\
";

void
outputCommands(dpy, resourceID, req_type)
    Display		*dpy;
    XID			resourceID;
    PEXOCRequestType	req_type;
{	
    PEXColor		colr;
    int			i,j,k;

    static		PEXCoord2D pt1 = { 0.025, 0.05 };
    static		PEXCoord pt2 = { 0.5, 0.45, 0.0 };
    static		PEXCoord2D offset1 = { 0.0, 0.0 };
    static		PEXCoord offset2 = { 0.1, 0.05, 0.0 };

    static		double height[] = {0.04, 0.0, -0.02};
    static		char str_height[][STRLEN] = 
				{"HT=0.04","HT=0.0","HT=-0.02"};

    static		double exp[] = {1.2, 0.0, -0.4};
    static		char str_exp[][STRLEN] = 
				{"EX=1.2", "EX=0.0", "EX=-0.6"};

    static		double space[] = {1.0, 0.0, -0.5};
    static		char str_space[][STRLEN] = 
				{"SP=1.0", "SP=0.0", "SP=-0.5"};

    static		PEXVector2D upvec1[] = {1.0,2.0};
    static		PEXVector2D upvec2[] = {0.0,-1.0};
    static		PEXVector2D upvec3[] = {0.0,1.0};
    static		char str_upvec[][STRLEN] = {"UV=(1,2)", "UV=(0,-1)"};

    static		int path[] = 
			{PEXPathRight, PEXPathLeft, PEXPathUp, PEXPathDown};

    static		char str_align[][STRLEN] = 
				{ "L-Cap", "L-Top", "Normal",
				"R-Base", "R-Bottom","C-Half"};

    static		int align[][2] = { 
				{PEXHAlignLeft, PEXVAlignCap}, 
				{PEXHAlignLeft, PEXVAlignTop}, 
				{PEXHAlignNormal, PEXVAlignNormal}, 
				{PEXHAlignRight, PEXVAlignBase}, 
				{PEXHAlignRight, PEXVAlignBottom}, 
				{PEXHAlignCenter, PEXVAlignHalf}, 
			};

    colr.rgb.red = 0.0;
    colr.rgb.green = 0.0;
    colr.rgb.blue = 1.0;

/* use stroke precision */
    SET_TEXT_PRECISION(PEXStrokePrecision)

/* atext height */
    for (i=0; i < sizeof(height)/sizeof(double); i++) {
    	SET_ATEXT_HEIGHT(height[i])
    	ANNOTATION_TEXT2D(&pt1,&offset1, sizeof(str_height[i])-1, str_height[i])
    	pt1.y += 0.08;
    }

    SET_ATEXT_HEIGHT(0.035)

/* char expansion */
    pt1.x = 0.35;
    pt1.y = 0.05;
    for (i=0; i < sizeof(exp)/sizeof(double); i++) {
    	SET_CHAR_EXPANSION(exp[i])
    	ANNOTATION_TEXT2D(&pt1, &offset1, sizeof(str_exp[i]) - 1, str_exp[i])
    	pt1.y += 0.08;
    }

    SET_CHAR_EXPANSION(1.0)

/* char spacing */
    pt1.x = 0.7;
    pt1.y = 0.05;
    for (i=0; i < sizeof(space)/sizeof(double); i++) {
    	SET_CHAR_SPACING(space[i])
    	ANNOTATION_TEXT2D(&pt1, &offset1, sizeof(str_space[i])-1, str_space[i])
    	pt1.y += 0.08;
    }
    SET_CHAR_SPACING(0.0)

/* atext path */
    pt1.x = 0.15;
    pt1.y = 0.45;
    for (i=0; i < sizeof(path)/sizeof(int); i++) {
    	SET_ATEXT_PATH(path[i])
    	ANNOTATION_TEXT2D(&pt1, &offset1, sizeof(STR_PEX) - 1, STR_PEX)
    }
    SET_ATEXT_PATH(PEXPathRight)

/* atext up vector */
    pt1.x = 0.375;
    pt1.y = 0.3;
    SET_ATEXT_UP_VECTOR(upvec1)
    ANNOTATION_TEXT2D(&pt1, &offset1, sizeof(str_upvec[0]) - 1, str_upvec[0])

    pt1.x = 0.55;
    pt1.y = 0.4;
    SET_ATEXT_UP_VECTOR(upvec2)
    ANNOTATION_TEXT2D(&pt1, &offset1, sizeof(str_upvec[1]) - 1, str_upvec[1])

    SET_ATEXT_UP_VECTOR(upvec3)

/* atext alignment and color index */
    pt1.x = 0.05;
    pt1.y = 0.7;
    SET_MARKER_TYPE(PEXMarkerX);
    SET_MARKER_SCALE(0.5);
    SET_TEXT_COLOR_INDEX(2);
    for (i=0,k=0; i < 2; i++) {
    	for (j=0; j < 3; j++,k++) {
    	    SET_ATEXT_ALIGNMENT(align[k][0], align[k][1])
    	    ANNOTATION_TEXT2D(&pt1, &offset1, sizeof(str_align[k]) - 1, 
		str_align[k])
	    MARKER2D(1, &pt1)
	    pt1.y += 0.1;
	}
	pt1.x += 0.4;
	pt1.y = 0.7;
    }

    SET_ATEXT_ALIGNMENT(PEXHAlignNormal, PEXVAlignNormal)

/* 3D atext, line style, atext style and text color */
    SET_TEXT_COLOR(PEXColorTypeRGB,&colr)
    SET_ATEXT_STYLE(PEXATextConnected)
    SET_LINE_TYPE(PEXLineTypeDashed)
    ANNOTATION_TEXT(&pt2, &offset2, sizeof(STR_3DTEXT) - 1, STR_3DTEXT)

/* text bundle */
    SET_TEXT_BUNDLE_INDEX(1)
    SET_INDIVIDUAL_ASF(PEXASFCharExpansion,PEXIndividual)
    SET_INDIVIDUAL_ASF(PEXASFCharSpacing,PEXBundled)
    SET_INDIVIDUAL_ASF(PEXASFTextColor,PEXBundled)
    pt1.x = 0.4;
    pt1.y = 0.6;
    ANNOTATION_TEXT2D(&pt1, &offset1, sizeof(STR_BUNDLE) - 1, STR_BUNDLE)
}
 
void
testproc(dpy,window,x,y,w,h)
    Display		*dpy;
    Window		window;
    int			x,y,w,h;
{
    image_testproc("atextAttrs","annotation text attributes", test_description,
	dpy,window,x,y,w,h);
} 

void
misc_setup(dpy, window, renderer)
Display		*dpy;
Window		window;
PEXRenderer	renderer;
{
}
