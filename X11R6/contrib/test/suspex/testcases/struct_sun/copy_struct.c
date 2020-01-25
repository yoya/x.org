/* $XConsortium: copy_struct.c,v 1.0 93/11/22 12:52:05 rws Exp $ */


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
CopyStructure Tests:\n\n\
The image consists of a thin red dashed polyline and\n\
2 thin green dashed polyline.\n\
";

PEXCoord 	points[4] = {
	{0.1, 0.1, 0.0},
	{0.2, 0.1, 0.0},
	{0.3, 0.2, 0.0},
	{0.4, 0.2, 0.0}};

PEXCoord 	points2[4] = {
	{0.5, 0.5, 0.0},
	{0.7, 0.3, 0.0},
	{0.8, 0.7, 0.0},
	{0.9, 0.6, 0.0}};

PEXCoord 	points3[4] = {
	{0.2, 0.3, 0.0},
	{0.3, 0.3, 0.0},
	{0.3, 0.4, 0.0},
	{0.4, 0.4, 0.0}};

PEXStructure 	src_structure;
PEXStructure 	src_structure2;

void
setup(dpy)
    Display		*dpy;
{
    PEXColor		color;

    src_structure = PEXCreateStructure(dpy);

    /* this structure stores two green dashed polylines 
     */
    PEXSetLineColorIndex(dpy, src_structure, PEXOCStore, 3);
    PEXSetLineType(dpy, src_structure, PEXOCStore, PEXLineTypeDashed);
    PEXPolyline(dpy, src_structure, PEXOCStore, 4, points);
    PEXLabel(dpy, src_structure, PEXOCStore, 123);
    PEXPolyline(dpy, src_structure, PEXOCStore, 4, points2);


    src_structure2 = PEXCreateStructure(dpy);

    /* this structure stores a red dotted polyline
     */
    color.rgb.red = 1.0;
    color.rgb.green = 0.0;
    color.rgb.blue = 0.0;

    PEXSetLineType(dpy, src_structure2, PEXOCStore, PEXLineTypeDotted);
    PEXSetLineColor(dpy, src_structure2, PEXOCStore, 
		PEXColorTypeRGB, &color);
    PEXLabel(dpy, src_structure2, PEXOCStore, 123);
    PEXPolyline(dpy, src_structure2, PEXOCStore, 4, points3);

}

void
outputCommands(dpy, resourceID, req_type)
    Display             *dpy;
    XID                 resourceID;
    PEXOCRequestType    req_type;
{
    PEXSetLineColorIndex(dpy, resourceID, req_type, 6);
    PEXSetLineType(dpy, resourceID, req_type, PEXLineTypeSolid);
    PEXSetLineWidth(dpy, resourceID, req_type, 4.0);

    /* copy will overwrite what's originally in the structure
     * so make sure polylines drawn are not wide lines
     */
    PEXCopyStructure(dpy, src_structure, resourceID);
    
    /* copy the green line, NOT dotted though
     */
    PEXCopyElements(dpy, src_structure2, PEXBeginning, 2,
	PEXEnd, 0, resourceID, PEXEnd, 0);
}

void
testproc(dpy,window,x,y,w,h)
    Display             *dpy;
    Window              window;
    int                 x,y,w,h;
{
    image_testproc("copy_struct", "copy structure", test_description,
        dpy,window,x,y,w,h);
}
