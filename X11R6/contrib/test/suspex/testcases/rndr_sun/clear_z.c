/* $XConsortium: clear_z.c,v 1.0 93/11/22 12:53:32 rws Exp $ */

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
Clear Z Tests:\n\n\
The final image composed of a red triangle which lies on top of both the \n\
green and blue triangles.\n\
";



char *data = "PEXlib structure output command test case" ;
 
PEXCoord	triangle3[3] = {
			{0.5, 0.6, 0.0},
			{0.3, 0.1, 0.0},
			{0.7, 0.1, 0.0}};

PEXCoord	triangle1[3] = {
			{0.4, 0.5, 0.5},
			{0.3, 0.3, 0.5},
			{0.5, 0.3, 0.5}};

PEXCoord	triangle2[3] = {
			{0.5, 0.4, 0.2},
			{0.4, 0.2, 0.2},
			{0.6, 0.2, 0.2}};

PEXCoord	*triangle[3];
PEXRendererAttributes	values;
Bool		clearI[3] = {True, False, False};

void
outputCommands(dpy, resourceID, req_type)
    Display		*dpy;
    XID			resourceID;
    PEXOCRequestType	req_type;
{	
    static int i = 0;

    PEXSetInteriorStyle(dpy, resourceID, req_type, PEXInteriorStyleSolid);
    PEXSetSurfaceColorIndex(dpy, resourceID, req_type, i+2);
    PEXFillArea(dpy, resourceID, req_type, PEXShapeConvex,
	False, 3, triangle[i]);
    i++;
    values.clear_z = False;
    PEXChangeRenderer(dpy, resourceID, PEXRAClearZ, &values);
}
 

void
misc_setup(dpy, window, renderer)
Display         *dpy;
Window          window;
PEXRenderer     renderer;
{
    extern int rendering_loops;
    rendering_loops = 3;
    triangle[0] = triangle1;
    triangle[1] = triangle2;
    triangle[2] = triangle3;
    values.hlhsr_mode = PEXHLHSRZBuffer;
    PEXChangeRenderer(dpy, renderer, PEXRAHLHSRMode, &values);
}

void
testproc(dpy,window,x,y,w,h)
    Display		*dpy;
    Window		window;
    int			x,y,w,h;
{
    image_testproc("clear_z", "Clear Z Buffer", test_description,
        dpy,window,x,y,w,h);
} 
