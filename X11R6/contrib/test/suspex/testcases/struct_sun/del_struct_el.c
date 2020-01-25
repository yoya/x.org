/* $XConsortium: del_struct_el.c,v 1.0 93/11/22 12:52:00 rws Exp $ */

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
Delete Elements Tests:\n\n\
You should see a wide green dotted polyline.\n\
";

PEXCoord2D	origin = {0.5, 0.5};
int		length = 5;
char	 	*string = "PEXLIB";

PEXCoord	square[4] = {
	{0.6, 0.6, 0.0},
	{0.8, 0.6, 0.0},
	{0.8, 0.8, 0.0},
	{0.6, 0.8, 0.0}};

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
    Display		*dpy;
{
}

void
outputCommands(dpy, resourceID, req_type)
    Display             *dpy;
    XID                 resourceID;
    PEXOCRequestType    req_type;
{
    PEXSetLineColorIndex(dpy, resourceID, req_type, 3);
    PEXSetLineType(dpy, resourceID, req_type, PEXLineTypeDotted);
    PEXSetLineColorIndex(dpy, resourceID, req_type, 2);
    PEXSetLineType(dpy, resourceID, req_type, PEXLineTypeSolid);
    PEXSetLineWidth(dpy, resourceID, req_type, 4.0);
    PEXSetTextPrecision(dpy, resourceID, req_type, PEXStrokePrecision);
    PEXText2D(dpy, resourceID, req_type, &origin, length, string);
    PEXLabel(dpy, resourceID, req_type, 123);
    PEXSetSurfaceColorIndex(dpy, resourceID, req_type, 6);
    PEXSetLineType(dpy, resourceID, req_type, PEXLineTypeDashed);
    PEXFillArea(dpy, resourceID, req_type, PEXShapeConvex,
	False, 4, square);
    PEXLabel(dpy, resourceID, req_type, 123);
    PEXPolyline(dpy, resourceID, req_type, 4, points);

    /* remove the red color, solid line type  specificiation
     * leave the width to wide
     */
    PEXDeleteElements(dpy, resourceID, PEXBeginning, 3,
	PEXBeginning, 4);

    /* remove the text
     */
    PEXDeleteToLabel(dpy, resourceID, PEXBeginning, 4, 123);
 
    /* remove the FillArea and dashed line type
     */
    PEXDeleteBetweenLabels(dpy, resourceID, 123, 123);

    /* leave with a wide green dotted polyline
     */
}

void
testproc(dpy,window,x,y,w,h)
    Display             *dpy;
    Window              window;
    int                 x,y,w,h;
{
    image_testproc("del_struct_el", 
        "Structure editing - Delete Elements", test_description,
        dpy,window,x,y,w,h);
}
