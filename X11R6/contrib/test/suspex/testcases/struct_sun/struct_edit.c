/* $XConsortium: struct_edit.c,v 1.0 93/11/22 12:52:40 rws Exp $ */

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
Structure Edit Tests:\n\n\
You should see a green dashed polyline, a blue solid polyline and a\n\
red solid polyline.\n\
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

void
setup(dpy)
    Display             *dpy;
{
}

void
outputCommands(dpy, resourceID, req_type)
    Display             *dpy;
    XID                 resourceID;
    PEXOCRequestType    req_type;
{

    /* use PEXSetElementPtr to create a green dashed polyline
     */
    PEXSetLineColorIndex(dpy, resourceID, req_type, 2);
    PEXSetLineType(dpy, resourceID, req_type, PEXLineTypeDashed);
    PEXSetElementPtr(dpy, resourceID, PEXBeginning, 1);
    PEXSetEditingMode(dpy, resourceID, PEXStructureReplace);
    PEXSetLineColorIndex(dpy, resourceID, req_type, 3);
    PEXSetElementPtr(dpy, resourceID, PEXCurrent, 1);
    PEXSetEditingMode(dpy, resourceID, PEXStructureInsert);
    PEXSetLineWidth(dpy, resourceID, req_type, 2.0);
    PEXSetElementPtr(dpy, resourceID, PEXBeginning, 1);
    PEXSetElementPtr(dpy, resourceID, PEXEnd, 0);
    PEXPolyline(dpy, resourceID, req_type, 4, points);
    PEXLabel(dpy, resourceID, req_type, 123);
    PEXPolyline(dpy, resourceID, req_type, 4, points2);
    PEXLabel(dpy, resourceID, req_type, 123);
    PEXPolyline(dpy, resourceID, req_type, 4, points3);

    /* use PEXSetElementPtrAtLabel to create a red solid polyline
     */
    PEXSetElementPtr(dpy, resourceID, PEXBeginning, 1);
    PEXSetElementPtrAtLabel(dpy, resourceID, 123, 0);
    PEXSetLineColorIndex(dpy, resourceID, req_type, 2);
    PEXSetLineType(dpy, resourceID, req_type, PEXLineTypeSolid);

    /* use PEXSetElementPtrAtLabel to create a blue solid polyline
     */
    PEXSetElementPtrAtLabel(dpy, resourceID, 123, 0);
    PEXSetLineColorIndex(dpy, resourceID, req_type, 4);
}

void
testproc(dpy,window,x,y,w,h)
    Display             *dpy;
    Window              window;
    int                 x,y,w,h;
{
    image_testproc("struct_edit", "structure editing", test_description,
        dpy,window,x,y,w,h);
}
