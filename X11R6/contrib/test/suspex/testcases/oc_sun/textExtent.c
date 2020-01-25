/* $XConsortium: textExtent.c,v 1.0 93/11/22 12:48:54 rws Exp $ */

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
#include "surface.h"


#define STR_PEX " PEX"
#define STRLEN 16
#define CHAR_HEIGHT 0.035

char test_description[] = "\
Query (Encoded) Text Extent Tests:\n\n\
Left: 2D text drawn in 4 different paths: Top, Down, Right, Left\n\
Right: Encoded 2D text drawn with char spacing: -0.5, 0, 1.0.\n\
Polylines are drawn around the text extent of each text primitives\n\
";


void
outputCommands(dpy, resourceID, req_type)
    Display		*dpy;
    XID			resourceID;
    PEXOCRequestType	req_type;
{	
    PEXColor		colr;
    int			i,j,k;

    static		PEXCoord2D pt; 
    static		int path[] = 
			{PEXPathRight, PEXPathLeft, PEXPathUp, PEXPathDown};
    static		double space[] = {1.0, 0.0, -0.5};

    PEXTextExtent	*extent;
    PEXCoord2D		box[4];
    PEXStringData	str;
    PEXEncodedTextData  et[3];
    PEXListOfEncodedText etlist;
    static  char  s61[1] = {'e'};
    static  char  s62[4] = {'t', 'e', 'x', 't'};
    static  char  s63[2] = {'2', 'd'};

/* use stroke precision */
    SET_TEXT_PRECISION(PEXStrokePrecision)
    SET_CHAR_HEIGHT(CHAR_HEIGHT)

/* text path */
    pt.x = 0.15;
    pt.y = 0.45;

    str.length = strlen(STR_PEX);
    str.ch = STR_PEX;

    for (i=0; i < sizeof(path)/sizeof(int); i++) {
    	SET_TEXT_PATH(path[i])
    	TEXT2D(&pt, sizeof(STR_PEX) - 1, STR_PEX)

        if (extent = PEXQueryTextExtents(dpy, resourceID, 0, path[i], 1.0,
            0.0, CHAR_HEIGHT, PEXHAlignNormal, PEXVAlignNormal, 1, &str)) {
            box[0].x = box[3].x = pt.x + extent->lower_left.x;
            box[0].y = box[1].y = pt.y + extent->lower_left.y;
            box[1].x = box[2].x = pt.x + extent->upper_right.x;
            box[2].y = box[3].y = pt.y + extent->upper_right.y;
	    PEXPolyline2D(dpy, resourceID, req_type, 4, box);
            XFree(extent);
	}
    }

    SET_TEXT_PATH(PEXPathRight)
    PEXSetTextFontIndex(dpy, resourceID, req_type, 2);

    et[0].character_set = 2;
    et[0].character_set_width = PEXCSByte;
    et[0].length = 1;
    et[0].ch = s61;
    et[1].character_set = 1;
    et[1].character_set_width = PEXCSByte;
    et[1].length = 4;
    et[1].ch = s62;
    et[2].character_set = 2;
    et[2].character_set_width = PEXCSByte;
    et[2].length = 2;
    et[2].ch = s63;

    etlist.count = 3;
    etlist.encoded_text = et;

/* char spacing */
    pt.x = 0.5;
    pt.y = 0.35;
    for (i=0; i < sizeof(space)/sizeof(double); i++) {
    	SET_CHAR_SPACING(space[i])
        PEXEncodedText2D(dpy, resourceID, req_type, &pt, 3, et);
    	pt.y += 0.08;
        if (extent = PEXQueryEncodedTextExtents(dpy, resourceID, 2, 
	    PEXPathRight, 1.0, space[i], CHAR_HEIGHT, PEXHAlignNormal, 
            PEXVAlignNormal, 1, &etlist)) {
            box[0].x = box[3].x = pt.x + extent->lower_left.x;
            box[0].y = box[1].y = pt.y + extent->lower_left.y;
            box[1].x = box[2].x = pt.x + extent->upper_right.x;
            box[2].y = box[3].y = pt.y + extent->upper_right.y;
	    PEXPolyline2D(dpy, resourceID, req_type, 4, box);
            XFree(extent);
	}
    }
}
 
void
testproc(dpy,window,x,y,w,h)
    Display		*dpy;
    Window		window;
    int			x,y,w,h;
{
    image_testproc("textExtents", "text extents", test_description,
	dpy,window,x,y,w,h);
} 

void
misc_setup(dpy, window, renderer)
Display         *dpy;
Window          window;
PEXRenderer     renderer;
{
    char                *fn1, *fn2;
    XID                 f1, f2, font_table;
    PEXFont             fe1[2];
    PEXTextFontEntry    fentry[1];
    PEXRendererAttributes ra;
    char                **retname;
    unsigned long       num_ret;
    PEXFontInfo         *info;

    retname = PEXListFontsWithInfo(dpy, "*", 100, &num_ret, &info);
    if (num_ret < 1) {
        printf("No font support!!\n");
        exit(-1);
    }   
    fn1 = fn2 = retname[0];
    if (num_ret > 1 ) fn2 = retname[1];
    font_table = PEXCreateLookupTable(dpy, window, PEXLUTTextFont);
    f1 = PEXLoadFont(dpy, fn1);
    f2 = PEXLoadFont(dpy, fn2);
    PEXFreeFontNames(num_ret, retname);
    PEXFreeFontInfo(num_ret, info);
    fe1[0] = f1;
    fe1[1] = f2;
    fentry[0].count = 2;
    fentry[0].fonts = fe1;
    PEXSetTableEntries(dpy, font_table, 2, 1, PEXLUTTextFont, fentry);
 
    ra.text_font_table = font_table;
    PEXChangeRenderer(dpy, renderer, PEXRATextFontTable, &ra);
}
