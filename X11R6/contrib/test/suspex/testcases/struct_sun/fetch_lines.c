/* $XConsortium: fetch_lines.c,v 1.0 93/11/22 12:52:17 rws Exp $ */

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
Fetch Elements Tests:\n\n\
You should see 2 thick dashed polyline. One in green, the other bluish green.\n\
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


PEXStructure 	src_structure;
PEXStructure 	src_structure2;

void
setup(dpy)
    Display		*dpy;
{
    PEXColor		color;

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
}

void
outputCommands(dpy, resourceID, req_type)
    Display             *dpy;
    XID                 resourceID;
    PEXOCRequestType    req_type;
{
    char 		*ocs;
    unsigned long    	oc_count, oc_length;
    int			fp = PEXIEEE_754_32;
    extern PEXRenderer  renderer;

#if 0
    if (PEXFetchElements(dpy, src_structure, PEXBeginning,
	    0, PEXEnd, 0, fp, &oc_count, &oc_length, &ocs))
	PEXSendOCs(dpy, resourceID, req_type, fp, oc_count,
		oc_length, ocs);
#endif
    PEXFetchElementsAndSend(dpy, src_structure, 
	PEXBeginning, 0, PEXEnd, 4, dpy, renderer, PEXOCRender);

    PEXFetchElementsAndSend(dpy, src_structure, 
	PEXBeginning, 0, PEXEnd, 0, dpy, resourceID, req_type);
}

void
testproc(dpy,window,x,y,w,h)
    Display             *dpy;
    Window              window;
    int                 x,y,w,h;
{
    image_testproc("fetch_lines", "fetch lines", test_description,
        dpy,window,x,y,w,h);
}
