/* $XConsortium: encTextAttr.c,v 1.0 93/11/22 12:49:23 rws Exp $ */

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
#include "misc.h"

#define STR_PEX " PEX"
#define STR_3DTEXT "3D text"
#define STR_PEXLIB "PEXLib"
#define STR_BUNDLE "BUNDLE"
#define STRLEN 16

char test_description[] = "\
Encoded Text Tests:\n\n\
Top:      Red 2D encoded text showing various kinds of alignment: Normal, \n\
          Center-Half Right-Bottom, Left-Top, Right-Base and Left-Cap. \n\
          Markers drawn on the text point for each text primitive .\n\
Center:   1.Red 2D encoded text rendered using bundled attributes\n\
            (color and spacing)\n\
          2.Multiple-encoded text drawn in blue \n\
          3. 2D Encoded Text drawn with 2 different up vectors: (0,-1),(1,2)\n\
Mid-Left: 2D Encoded Text drawn in 4 different paths: Top, Down, Right, Left\n\
Bottom:   1. 2D Encoded Text drawn with char height: -0.02, 0, 0.04.\n\
          2. 2D Encoded Text drawn with char expansion: -0.4, 0, 1.2.\n\
          3. 2D Encoded Text drawn with char spacing: -0.5, 0, 1.0.\n\
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
    static		PEXCoord pt2 = { 0.65, 0.35, 0.0 };

    static		PEXVector vec1 = { 0.0, 1.0, 0.0 };
    static		PEXVector vec2 = { 1.0, 0.0, 0.0 };

    static		double height[] = {0.04, 0.0, -0.02};
    static		char str_height[][STRLEN] = 
				{"HT=0.04","HT=0.0","HT=-0.02"};

    static		double exp[] = {1.2, 0.0, -0.4};
    static		char str_exp[][STRLEN] = 
				{"EX=1.2", "EX=0.0", "EX=-0.6"};

    static		double space[] = {1.0, 0.0, -0.5};
    static		char str_space[][STRLEN] = 
				{"SP=1.0", "SP=0.0", "SP=-0.5"};

    static		PEXVector upvec1[] = {1.0,2.0};
    static		PEXVector upvec2[] = {0.0,-1.0};
    static		PEXVector upvec3[] = {0.0,1.0};
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

    PEXEncodedTextData  encoded_texts[2];
    PEXEncodedTextData  *encoded_text = encoded_texts;

    encoded_text->character_set = 0;
    encoded_text->character_set_width = PEXCSByte;
    encoded_text->encoding_state;

    colr.rgb.red = 0.0;
    colr.rgb.green = 0.0;
    colr.rgb.blue = 1.0;

/* use stroke precision */
    SET_TEXT_PRECISION(PEXStrokePrecision)

/* char height */
    for (i=0; i < sizeof(height)/sizeof(double); i++) {
    	SET_CHAR_HEIGHT(height[i])
	encoded_text->length = strlen(str_height[i]);
	encoded_text->ch = str_height[i];
    	ENCTEXT2D(&pt1, 1, encoded_text)
    	pt1.y += 0.08;
    }

    SET_CHAR_HEIGHT(0.035)

/* char expansion */
    pt1.x = 0.35;
    pt1.y = 0.05;
    for (i=0; i < sizeof(exp)/sizeof(double); i++) {
    	SET_CHAR_EXPANSION(exp[i])
	encoded_text->length = strlen(str_exp[i]);
	encoded_text->ch = str_exp[i];
    	ENCTEXT2D(&pt1, 1, encoded_text)
    	pt1.y += 0.08;
    }

    SET_CHAR_EXPANSION(1.0)

/* char spacing */
    pt1.x = 0.7;
    pt1.y = 0.05;
    for (i=0; i < sizeof(space)/sizeof(double); i++) {
    	SET_CHAR_SPACING(space[i])
	encoded_text->length = strlen(str_space[i]);
	encoded_text->ch = str_space[i];
    	ENCTEXT2D(&pt1, 1, encoded_text)
    	pt1.y += 0.08;
    }
    SET_CHAR_SPACING(0.0)

/* text path */
    pt1.x = 0.15;
    pt1.y = 0.45;
    for (i=0; i < sizeof(path)/sizeof(int); i++) {
    	SET_TEXT_PATH(path[i])
	encoded_text->length = 4;
	encoded_text->ch = STR_PEX;
    	ENCTEXT2D(&pt1, 1, encoded_text)
    }
    SET_TEXT_PATH(PEXPathRight)

/* up vector */
    pt1.x = 0.375;
    pt1.y = 0.3;
    SET_CHAR_UP_VECTOR((PEXVector2D *)upvec1)
    encoded_text->length = strlen(str_upvec[0]);
    encoded_text->ch = str_upvec[0];
    ENCTEXT2D(&pt1, 1, encoded_text)

    pt1.x = 0.55;
    pt1.y = 0.4;
    SET_CHAR_UP_VECTOR((PEXVector2D *)upvec2)
    encoded_text->length = strlen(str_upvec[1]);
    encoded_text->ch = str_upvec[1];
    ENCTEXT2D(&pt1, 1, encoded_text)

    SET_CHAR_UP_VECTOR((PEXVector2D *)upvec3)

/* alignment and color index */
    pt1.x = 0.05;
    pt1.y = 0.7;
    SET_MARKER_TYPE(PEXMarkerX);
    SET_MARKER_SCALE(0.5);
    SET_TEXT_COLOR_INDEX(2);
    for (i=0,k=0; i < 2; i++) {
    	for (j=0; j < 3; j++,k++) {
    	    SET_TEXT_ALIGNMENT(align[k][0], align[k][1])
    	    encoded_text->length = strlen(str_align[k]);
    	    encoded_text->ch = str_align[k];
    	    ENCTEXT2D(&pt1, 1, encoded_text)
	    MARKER(1, (PEXCoord *)&pt1)
	    pt1.y += 0.1;
	}
	pt1.x += 0.4;
	pt1.y = 0.7;
    }

    SET_TEXT_ALIGNMENT(PEXHAlignNormal, PEXVAlignNormal)

/* 3D text and text color */
    SET_TEXT_FONT_INDEX(1)
    SET_TEXT_COLOR(PEXColorTypeRGB, &colr);
    encoded_texts[0].length = 7;
    encoded_texts[0].ch = STR_3DTEXT;
    encoded_texts[1].character_set = 0;
    encoded_texts[1].character_set_width = PEXCSByte;
    encoded_texts[1].encoding_state;
    encoded_texts[1].length = 6;
    encoded_texts[1].ch = STR_PEXLIB;
    ENCTEXT(&pt2, &vec1, &vec2, 2, encoded_texts);

/* text bundle */
    SET_TEXT_BUNDLE_INDEX(1)
    SET_INDIVIDUAL_ASF(PEXASFCharExpansion,PEXIndividual)
    SET_INDIVIDUAL_ASF(PEXASFCharSpacing,PEXBundled)
    SET_INDIVIDUAL_ASF(PEXASFTextColor,PEXBundled)
    pt1.x = 0.4;
    pt1.y = 0.6;
    encoded_text->length = 6;
    encoded_text->ch = STR_BUNDLE;
    ENCTEXT2D(&pt1, 1, encoded_text)

}
 
void
testproc(dpy,window,x,y,w,h)
    Display		*dpy;
    Window		window;
    int			x,y,w,h;
{
    image_testproc("encTextAttrs", "enc text attributes", test_description,
		dpy,window,x,y,w,h);
} 

void
misc_setup(dpy, window, renderer)
Display		*dpy;
Window		window;
PEXRenderer	renderer;
{
}
