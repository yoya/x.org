/* $XConsortium: render_elements.c,v 1.0 93/11/22 12:54:29 rws Exp $ */

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
RenderElements Tests:\n\n\
You should see a reddish fillareaSet which consists of a bowtie and a\n\
triangle.\n\
"; 

char *data = "PEXlib structure output command test case" ;
 
PEXStructure object;

void
outputCommands(dpy, resourceID, req_type)
    Display		*dpy;
    XID			resourceID;
    PEXOCRequestType	req_type;
{	
    PEXRenderElements(dpy, resourceID, object, PEXBeginning, 0,
	PEXEnd, 0);
}
 

PEXListOfCoord point_lists[2];
PEXCoord	bow_tie[4] = {
			{0.2, 0.4, 0.5},
			{0.2, 0.2, 0.5},
			{0.4, 0.4, 0.5},
			{0.4, 0.2, 0.5}};

PEXCoord	triangle[3] = {
			{0.4, 0.3, 0.0},
			{0.3, 0.1, 0.0},
			{0.5, 0.1, 0.0}};

void
misc_setup(dpy, window, renderer)
Display         *dpy;
Window          window;
PEXRenderer     renderer;
{
    PEXColor    color;

    object = PEXCreateStructure(dpy);

    color.rgb.red = 0.6;
    color.rgb.green = 0.3;
    color.rgb.blue = 0.2;

    PEXSetSurfaceColor(dpy, object, PEXOCStore, PEXColorTypeRGB, &color);
    PEXSetInteriorStyle(dpy, object, PEXOCStore, PEXInteriorStyleSolid);

    /* draw a bow tie 
     */
    point_lists[0].count = 4;
    point_lists[0].points = bow_tie;
    point_lists[1].count = 3;
    point_lists[1].points = triangle;
    
    PEXFillAreaSet(dpy, object, PEXOCStore, PEXShapeUnknown,
	True, PEXContourDisjoint, 2, point_lists);
}

void
testproc(dpy,window,x,y,w,h)
    Display		*dpy;
    Window		window;
    int			x,y,w,h;
{
    image_testproc("render_elements", "Render Elements", test_description,
        dpy,window,x,y,w,h);
} 
