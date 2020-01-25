/* $XConsortium: get_lines_info.c,v 1.0 93/11/22 12:52:23 rws Exp $ */

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
Get Element Info Tests:\n\n\
This test generates a text file which contains a list\n\
of element type and length of each elements inquried.\n\
";

PEXCoord 	points[4] = {
	{0.1, 0.1, 0.0},
	{0.2, 0.1, 0.0},
	{0.3, 0.2, 0.0},
	{0.4, 0.2, 0.0}};

PEXCoord2D 	points2[4] = {
	{0.5, 0.1},
	{0.6, 0.1},
	{0.7, 0.2},
	{0.8, 0.2}};

static PEXVertexRGB ptscolrgb1[] = {
        {{0.0, 0.5, 0.0},       {1.0, 0.0, 0.0}},
        {{0.05, 0.6, 0.0},      {0.0, 1.0, 0.0}},
        {{0.1, 0.4, 0.0},       {0.0, 0.0, 1.0}},
        {{0.125, 0.75, 0.0},    {1.0, 1.0, 1.0}}
};

PEXListOfVertex     vList1[2], vList2[1], vList3[2];
PEXArrayOfVertex    vArray1,vArray2,vArray3,vArray4,vArray5;

PEXStructure 	src_structure;
PEXStructure 	src_structure2;

extern FILE *	suspex_open_file();
extern int	suspex_print_el_info();
extern int	suspex_close_file();

void
setup(dpy)
    Display		*dpy;
{
    PEXColor		color;
    int			float_format = PEXIEEE_754_32;
    int			count;
    PEXElementInfo	*info;
    FILE		*fp;

    src_structure = PEXCreateStructure(dpy);

    /* this structure uses every polyline primitives and attributes 
     */
    PEXSetLineColorIndex(dpy, src_structure, PEXOCStore, 3);
    PEXSetLineType(dpy, src_structure, PEXOCStore, PEXLineTypeDashed);
    PEXSetLineWidth(dpy, src_structure, PEXOCStore, 4.0);
    PEXPolyline(dpy, src_structure, PEXOCStore, 4, points);

    color.rgb.red = 0.0;
    color.rgb.green = 0.5;
    color.rgb.blue = 0.5;
    PEXSetLineColor(dpy, src_structure, PEXOCStore,
	PEXColorTypeRGB, &color);
    PEXPolyline2D(dpy, src_structure, PEXOCStore, 4, points2);

    vArray3.rgb = ptscolrgb1;
    vList2[0].count = 4;
    vList2[0].vertices = vArray3;


    PEXSetLineBundleIndex(dpy, src_structure, PEXOCStore, 2);
    PEXPolylineSetWithData(dpy, src_structure, PEXOCStore,
	PEXGAColor, PEXColorTypeRGB, 1, vList2);

    if (PEXGetElementInfo(dpy, src_structure, PEXBeginning,
	    0, PEXEnd, 0, float_format, &count, &info)) {
	fp = suspex_open_file("get_lines_info",
	   "Element Info of Lines Primitives and Attributes");
	if (fp == (FILE *)-1)
	    return;
	suspex_print_el_info(fp, count, info);
	suspex_close_file(fp);
    }
}

void
outputCommands(dpy, resourceID, req_type)
    Display             *dpy;
    XID                 resourceID;
    PEXOCRequestType    req_type;
{
}

void
testproc(dpy,window,x,y,w,h)
    Display             *dpy;
    Window              window;
    int                 x,y,w,h;
{
    file_testproc("get_lines_info", 
	   "Element Info of Lines Primitives and Attributes");
}
