/* $XConsortium: lineSet.c,v 1.0 93/11/22 12:50:21 rws Exp $ */

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
PolylineSetWithData Tests:\n\n\
Top:    3 polylinSet each contains 2 set of lines with 3 and 2 vertices\n\
        respectively, and also specified with vertex color.\n\
        The first one is shaded, the last one rendered with bundled line type\n\
        dashdot, the others are dashes.\n\
Middle: 5 polylineSet each contains 1 set of lines(4 vertices) specified with\n\
        vertex color of red, green, blue and white. The first one is rendered\n\
        with default attributes. The next four is wide, shaded and drawn with\n\
        different line types of solid, dot, dashdot and dash.\n\
Bottom: 3 polylineSet each contains 1 set of lines(3 vertices) \n\
        in white, blue and green\n\
";

#define    TRANSLATE_VLIST1(_which, _offset) \
    TRANSLATE_VERTEX(_which, (_offset), 2, pts1) \
    TRANSLATE_VERTEX(_which, (_offset), 3, pts2)

#define    TRANSLATE_VLIST2(_which, _offset) \
    TRANSLATE_VERTEX(point._which, (_offset), 4, ptscolrgb1) \

#define    TRANSLATE_VLIST3(_which, _offset) \
    TRANSLATE_VERTEX(point._which, (_offset), 3, ptscolind1) \
    TRANSLATE_VERTEX(point._which, (_offset), 2, ptscolind2) 

void
outputCommands(dpy, resourceID, req_type)
    Display		*dpy;
    XID			resourceID;
    PEXOCRequestType	req_type;
{	
    PEXColor		colr;
    int			i;

    static int          lineType[] = {
        PEXLineTypeSolid,
        PEXLineTypeDotted,
        PEXLineTypeDashDot,
        PEXLineTypeDashed,
    };

    static PEXCoord 	pts1[] = { 
	0.1, 0.1, 0.0,
	0.2, 0.2, 0.0,
    };

    static PEXCoord 	pts2[] = { 
	0.125, 0.1, 0.0,
	0.3, 0.1, 0.0,
	0.225, 0.2, 0.0,
    };

    static PEXVertexRGB ptscolrgb1[] = { 
        {{0.0, 0.5, 0.0},	{1.0, 0.0, 0.0}},
        {{0.05, 0.6, 0.0},	{0.0, 1.0, 0.0}},
        {{0.1, 0.4, 0.0},	{0.0, 0.0, 1.0}},
        {{0.125, 0.75, 0.0},	{1.0, 1.0, 1.0}}
    };

    static PEXVertexIndexed ptscolind1[] = { 
        {{0.0, 0.8, 0.0},	{3}},
        {{0.1, 0.9, 0.0},	{4}},
        {{0.2, 0.8, 0.0},	{5}},
    };

    static PEXVertexIndexed ptscolind2[] = { 
        {{0.1, 0.8, 0.0},	{1}},
        {{0.1, 0.9, 0.0},	{2}},
    };

    PEXListOfVertex 	vList1[2], vList2[1], vList3[2];
    PEXArrayOfVertex 	vArray1,vArray2,vArray3,vArray4,vArray5;

    colr.rgb.red = 0.0;
    colr.rgb.green = 0.0;
    colr.rgb.blue = 1.0;
 
    vArray1.no_data = pts1;
    vArray2.no_data = pts2;
    vArray3.rgb = ptscolrgb1;
    vArray4.index = ptscolind1;
    vArray5.index = ptscolind2;

    vList1[0].count = 2;
    vList1[0].vertices = vArray1;
    vList1[1].count = 3;
    vList1[1].vertices = vArray2;

    vList2[0].count = 4;
    vList2[0].vertices = vArray3;

    vList3[0].count = 3;
    vList3[0].vertices = vArray4;
    vList3[1].count = 2;
    vList3[1].vertices = vArray5;

/* default line attrs */
    POLYLINESETWITHDATA(PEXGANone, PEXColorTypeRGB, 2, vList1)

/* line color */
    TRANSLATE_VLIST1(x, 0.2)
    SET_LINE_COLOR(PEXColorTypeRGB,&colr)
    POLYLINESETWITHDATA(PEXGANone, PEXColorTypeRGB, 2, vList1)

/* line color index */
    TRANSLATE_VLIST1(x, 0.2)
    SET_LINE_COLOR_INDEX(3)
    POLYLINESETWITHDATA(PEXGANone, PEXColorTypeRGB, 2, vList1)

/* vertex color overrides line color settings */
    POLYLINESETWITHDATA(PEXGAColor, PEXColorTypeRGB, 1, vList2)

/* various line types, non-default width, RGB color interp  */

    SET_LINE_WIDTH(5.0)
    SET_LINE_INTERP_METHOD(PEXPolylineInterpColor)
    for (i=0; i < sizeof(lineType)/sizeof(int); ++i ) {
        SET_LINE_TYPE(lineType[i])
	TRANSLATE_VLIST2(x, 0.2)
        POLYLINESETWITHDATA(PEXGAColor, PEXColorTypeRGB, 1, vList2)
    }

/* index color interp */

    POLYLINESETWITHDATA(PEXGAColor, PEXColorTypeIndexed, 2, vList3)

/* no interp */

    TRANSLATE_VLIST3(x, 0.3)
    SET_LINE_INTERP_METHOD(PEXPolylineInterpNone)
    POLYLINESETWITHDATA(PEXGAColor, PEXColorTypeIndexed, 2, vList3)

/* line bundle */
    SET_LINE_BUNDLE_INDEX(1);
    SET_INDIVIDUAL_ASF(PEXASFLineType,PEXBundled);
    SET_INDIVIDUAL_ASF(PEXASFLineWidth,PEXIndividual);
    TRANSLATE_VLIST3(x, 0.3)
    POLYLINESETWITHDATA(PEXGAColor, PEXColorTypeIndexed, 2, vList3)
}
 
void
testproc(dpy,window,x,y,w,h)
    Display		*dpy;
    Window		window;
    int			x,y,w,h;
{
    image_testproc("lineSet", "lineSet colr index,width,type", test_description,
	dpy,window,x,y,w,h);
} 

void
misc_setup(dpy, window, renderer)
Display		*dpy;
Window		window;
PEXRenderer	renderer;
{
}
