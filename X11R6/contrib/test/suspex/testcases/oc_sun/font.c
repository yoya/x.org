/* $XConsortium: font.c,v 1.0 93/11/22 12:49:52 rws Exp $ */

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
Font tests:\n\n\
You should see Text, Text2D, Atext, Atext2D, EncodedText, EncodedText2D,\n\
EncodedAtext and EncodedAtext2D primitives drawn with various font indices\n\
and font groups. The resulting image is implementation dependent as the test \n\
uses font info inquired from server to setup the font table.\n\
The test also generates a text file which contains the matched font\n\
names for the patterns *oman* and *g*, for comparison\n\
";

static	PEXCoord	origin1 = {0.1, 0.1, 0.0};
static	PEXCoord2D	origin2 = {0.1, 0.2};
static	PEXCoord	origin3 = {0.1, 0.3, 0.0};
static	PEXCoord2D	origin4 = {0.1, 0.4};
static	PEXCoord	origin5 = {0.1, 0.5, 0.0};
static	PEXCoord2D	origin6 = {0.1, 0.6};
static	PEXCoord	origin7 = {0.1, 0.7, 0.0};
static	PEXCoord2D	origin8 = {0.1, 0.8};
static	PEXCoord	offset3 = {0.0, 0.0, 0.0};
static	PEXCoord2D	offset2 = {0.0, 0.0};
static	PEXVector	v1 = {1.0,0.0,0.0};
static	PEXVector	v2 = {0.0,1.0,0.0};
static	char		s1[6] = {'t', 'e', 'x', 't', '3', 'd'};
static	char		s2[6] = {'t', 'e', 'x', 't', '2', 'd'};
static	char		s3[7] = {'a', 't', 'e', 'x', 't', '3', 'd'};
static	char		s4[7] = {'a', 't', 'e', 'x', 't', '2', 'd'};
static	char		s51[1] = {'e'};
static	char		s52[4] = {'t', 'e', 'x', 't'};
static	char		s53[2] = {'3', 'd'};
static	char		s61[1] = {'e'};
static	char		s62[4] = {'t', 'e', 'x', 't'};
static	char		s63[2] = {'2', 'd'};
static	char		s71[2] = {'e', 'a'};
static	char		s72[4] = {'t', 'e', 'x', 't'};
static	char		s73[2] = {'3', 'd'};
static	char		s81[2] = {'e', 'a'};
static	char		s82[4] = {'t', 'e', 'x', 't'};
static	char		s83[2] = {'2', 'd'};

extern FILE *   suspex_open_file();
extern int      suspex_close_file();

void
outputCommands(dpy, resourceID, req_type)
    Display             *dpy;
    unsigned long       resourceID; 
    PEXOCRequestType    req_type;
{	
    PEXColor		colr;
    PEXEncodedTextData	e1[3], e2[3], e3[3], e4[3];

    PEXSetTextPrecision(dpy, resourceID, req_type, PEXStrokePrecision);
    PEXSetCharHeight(dpy, resourceID, req_type, 0.08);
    PEXText(dpy, resourceID, req_type, &origin1, &v1, &v2, 6, s1);
    PEXSetTextFontIndex(dpy, resourceID, req_type, 2);
    PEXText2D(dpy, resourceID, req_type, &origin2, 6, s2);

    PEXSetTextFontIndex(dpy, resourceID, req_type, 1);
    PEXSetATextHeight(dpy, resourceID, req_type, 0.08);
    PEXAnnotationText(dpy, resourceID, req_type, &origin3, &offset3, 7, s3);
    PEXSetTextFontIndex(dpy, resourceID, req_type, 2);
    PEXAnnotationText2D(dpy, resourceID, req_type, &origin4, &offset2, 7, s4);

    e1[0].character_set = 1;
    e1[0].character_set_width = PEXCSByte;
    e1[0].length = 1;
    e1[0].ch = s51;
    e1[1].character_set = 2;
    e1[1].character_set_width = PEXCSByte;
    e1[1].length = 4;
    e1[1].ch = s52;
    e1[2].character_set = 1;
    e1[2].character_set_width = PEXCSByte;
    e1[2].length = 2;
    e1[2].ch = s53;
    PEXEncodedText(dpy, resourceID, req_type, &origin5, &v1, &v2, 3, e1);

    PEXSetTextFontIndex(dpy, resourceID, req_type, 2);
    e2[0].character_set = 2;
    e2[0].character_set_width = PEXCSByte;
    e2[0].length = 1;
    e2[0].ch = s61;
    e2[1].character_set = 1;
    e2[1].character_set_width = PEXCSByte;
    e2[1].length = 4;
    e2[1].ch = s62;
    e2[2].character_set = 2;
    e2[2].character_set_width = PEXCSByte;
    e2[2].length = 2;
    e2[2].ch = s63;
    PEXEncodedText2D(dpy, resourceID, req_type, &origin6, 3, e2);

    PEXSetTextFontIndex(dpy, resourceID, req_type, 3);
    e3[0].character_set = 2;
    e3[0].character_set_width = PEXCSByte;
    e3[0].length = 2;
    e3[0].ch = s71;
    e3[1].character_set = 1;
    e3[1].character_set_width = PEXCSByte;
    e3[1].length = 4;
    e3[1].ch = s72;
    e3[2].character_set = 2;
    e3[2].character_set_width = PEXCSByte;
    e3[2].length = 2;
    e3[2].ch = s73;
    PEXEncodedAnnoText(dpy, resourceID, req_type, &origin7, &offset3, 3, e3);

    e4[0].character_set = 1;
    e4[0].character_set_width = PEXCSByte;
    e4[0].length = 2;
    e4[0].ch = s81;
    e4[1].character_set = 2;
    e4[1].character_set_width = PEXCSByte;
    e4[1].length = 4;
    e4[1].ch = s82;
    e4[2].character_set = 1;
    e4[2].character_set_width = PEXCSByte;
    e4[2].length = 2;
    e4[2].ch = s83;
    PEXEncodedAnnoText2D(dpy, resourceID, req_type, &origin8, &offset2, 3, e4);
}
 
void
testproc(dpy,window,x,y,w,h)
    Display		*dpy;
    Window		window;
    int			x,y,w,h;
{
    file_testproc("font.1", "various text fonts info");
    image_testproc("font", "various text fonts", test_description,
        dpy,window,x,y,w,h);
} 

void
misc_setup(dpy, window, renderer)
Display		*dpy;
Window		window;
PEXRenderer	renderer;
{
    char                *fn1, *fn2, *fn3, *fn4;
    XID                 f1, f2, f3, f4, font_table;
    PEXFont             fe1[2], fe2[2];
    PEXTextFontEntry    fentry[2];
    PEXRendererAttributes ra;
    char                **retname;
    char                *patt1 = "*oman*";
    char                *patt2 = "*g*";
    unsigned long       num_ret;
    int                 i;
    PEXFontInfo		*info;
    FILE		*fp;

    fp = suspex_open_file("font.1", "Font info");
    if (fp == (FILE *)-1) {
	printf("can't open file \"font.1\"\n");
	exit(-1);
    }
    retname = PEXListFonts(dpy, patt1, 100, &num_ret);
    for (i=0; i< num_ret; i++) {
	fprintf(fp, "font name %s\n", retname[i]);
    }

    PEXFreeFontNames(num_ret, retname);
    retname = PEXListFontsWithInfo(dpy, patt2, 100, &num_ret, &info);
    for (i=0; i< num_ret; i++) {
	fprintf(fp, "font name : %s\n", retname[i]);
	fprintf(fp, "	1st glyph %d\n", info[i].first_glyph);
	fprintf(fp, "	all exist %d\n", info[i].all_exist);
    }
    retname = PEXListFontsWithInfo(dpy, "*", 100, &num_ret, &info);
    if (num_ret < 1) {
	printf("No font support!!\n");
	exit(-1);
    }
    fn1 = fn2 = fn3 = fn4 = retname[0];
    if (num_ret > 1 ) fn2 = retname[1];
    if (num_ret > 2 ) fn3 = retname[2];
    if (num_ret > 3 ) fn4 = retname[3];
    font_table = PEXCreateLookupTable(dpy, window, PEXLUTTextFont);
    f1 = PEXLoadFont(dpy, fn1);
    PEXUnloadFont(dpy, f1);
    f1 = PEXLoadFont(dpy, fn1);
    f2 = PEXLoadFont(dpy, fn2);
    f3 = PEXLoadFont(dpy, fn3);
    f4 = PEXLoadFont(dpy, fn4);
    PEXFreeFontNames(num_ret, retname);
    PEXFreeFontInfo(num_ret, info);
    fe1[0] = f1;
    fe1[1] = f2;
    fe2[0] = f3;
    fe2[1] = f4;
    fentry[0].count = 2;
    fentry[0].fonts = fe1;
    fentry[1].count = 2;
    fentry[1].fonts = fe2;
    PEXSetTableEntries(dpy, font_table, 2, 2, PEXLUTTextFont, fentry);

    ra.text_font_table = font_table;
    PEXChangeRenderer(dpy, renderer, PEXRATextFontTable, &ra);

    suspex_close_file(fp);
}
