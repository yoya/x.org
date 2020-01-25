/* $XConsortium: struct_ref2.c,v 1.0 93/11/22 12:52:57 rws Exp $ */

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
Structure Reference Edit Tests:\n\n\
You should see 2 which polylines.\n\
";

PEXCoord 	line_pts[4] = {
	{0.1, 0.1, 0.0},
	{0.2, 0.1, 0.0},
	{0.3, 0.2, 0.0},
	{0.4, 0.2, 0.0}};

PEXVector vector = {0.5, 0.5, 0.0};

PEXMatrix transform;
PEXStructure line, object, nothing;

void
setup(dpy)
    Display             *dpy;
{
    /* create the line structure
     */
    line = PEXCreateStructure(dpy);
    PEXPolyline(dpy, line, PEXOCStore, 4, line_pts);

    /* create the nothing structure
     */
    nothing = PEXCreateStructure(dpy);

    /* object executes nothing
     */
    object = PEXCreateStructure(dpy);
    PEXExecuteStructure(dpy, object, PEXOCStore, line);
}

void
outputCommands(dpy, resourceID, req_type)
    Display             *dpy;
    XID                 resourceID;
    PEXOCRequestType    req_type;
{
    PEXExecuteStructure(dpy, resourceID, req_type, object);
    PEXExecuteStructure(dpy, resourceID, req_type, object);
    PEXExecuteStructure(dpy, resourceID, req_type, object);
    PEXTranslate(&vector, transform);
    PEXSetLocalTransform(dpy, resourceID, req_type, PEXReplace, transform);
    PEXExecuteStructure(dpy, resourceID, req_type, object);

    /* remove the first 2 execute structures elements
     */
    PEXDeleteElements(dpy, resourceID, PEXBeginning, 1,
	PEXBeginning, 2);

}

void
testproc(dpy,window,x,y,w,h)
    Display             *dpy;
    Window              window;
    int                 x,y,w,h;
{
    image_testproc("struct_ref2", "structure reference edit", test_description,
        dpy,window,x,y,w,h);
}
