/* $XConsortium: accum_state.c,v 1.0 93/11/22 12:53:15 rws Exp $ */

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
AccumulatelateState, RenderElements Tests:\n\n\
The final image is conposed of a cyan marker and a hollow green fillareaSet.\n\
";

char *data = "PEXlib structure output command test case" ;
 
PEXStructure object, object2;
PEXRendererAttributes	values;
PEXCoord	origin = {0.2, 0.6, 0.0};

void
outputCommands(dpy, resourceID, req_type)
    Display		*dpy;
    XID			resourceID;
    PEXOCRequestType	req_type;
{	
    static int i = 0;

    switch (i) {
    case 0:
        PEXRenderElements(dpy, resourceID, object, PEXBeginning, 0,
		PEXEnd, 0);
	break;
    case 1: {
	PEXElementRef	path[2];

	path[0].structure = object;
        path[0].offset = 7;
	path[1].structure = object2;
        path[1].offset = 1;
      
	PEXAccumulateState(dpy, resourceID, 2, path);
	PEXSetSurfaceColorIndex(dpy, resourceID, req_type, 5);
	PEXRenderElements(dpy, resourceID, object2, PEXBeginning, 3,
		PEXBeginning, 3);
	
        /* don't clear image 
         */
        values.clear_image = False;
        PEXChangeRenderer(dpy, resourceID, PEXRAClearImage, &values);
	break;
    }
    case 2: {
	PEXSetMarkerColorIndex(dpy, resourceID, req_type, 6);
	PEXMarkers(dpy, resourceID, req_type, 1, &origin);
    }
    default:
	break;
    }
    i++;
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

PEXVector   	trans_vector;
PEXMatrix	matrix;
PEXVector	vector1 = {1.0, 0.0, 0.0};
PEXVector	vector2 = {0.0, 1.0, 0.0};
char		*string = "accum state test";

void
misc_setup(dpy, window, renderer)
Display         *dpy;
Window          window;
PEXRenderer     renderer;
{
    PEXColor    		color;
    extern int 			rendering_loops;

    /* two rendering loops, one to draw the entire picture,
       and the other to echo the second fill area set
     */
    rendering_loops = 3;

    point_lists[0].count = 4;
    point_lists[0].points = bow_tie;
    point_lists[1].count = 3;
    point_lists[1].points = triangle;
    
    object2 = PEXCreateStructure(dpy);
    PEXSetInteriorStyle(dpy, object2, PEXOCStore, PEXInteriorStyleHollow);
    PEXSetSurfaceColorIndex(dpy, object2, PEXOCStore, 2);
    /* draw a bow tie 
     */
    PEXFillAreaSet(dpy, object2, PEXOCStore, PEXShapeUnknown,
	True, PEXContourDisjoint, 2, point_lists);

    /* draw a text */
    PEXSetTextPrecision(dpy, object2, PEXOCStore, PEXStrokePrecision);
    PEXSetCharHeight(dpy, object2, PEXOCStore, 0.05);
    PEXText(dpy, object2, PEXOCStore, &origin, &vector1, &vector2,
		strlen(string), string);

    object = PEXCreateStructure(dpy);

    color.rgb.red = 0.6;
    color.rgb.green = 0.3;
    color.rgb.blue = 0.2;

    PEXSetSurfaceColor(dpy, object, PEXOCStore, PEXColorTypeRGB, &color);
    PEXSetInteriorStyle(dpy, object, PEXOCStore, PEXInteriorStyleSolid);

    /* draw a bow tie 
     */
    PEXFillAreaSet(dpy, object, PEXOCStore, PEXShapeUnknown,
	True, PEXContourDisjoint, 2, point_lists);

    trans_vector.x = 0.5;
    trans_vector.y = 0.1;
    trans_vector.z = 0.0;
    PEXTranslate(&trans_vector, matrix);
    PEXSetLocalTransform(dpy, object, PEXOCStore, PEXReplace, matrix);
    PEXExecuteStructure(dpy, object, PEXOCStore, object2);

    trans_vector.x = 0.2;
    trans_vector.y = 0.3;
    trans_vector.z = 0.0;
    PEXTranslate(&trans_vector, matrix);
    PEXSetLocalTransform(dpy, object, PEXOCStore, PEXReplace, matrix);
    PEXExecuteStructure(dpy, object, PEXOCStore, object2);


    /* clear image 
     */
    values.clear_image = True;
    PEXChangeRenderer(dpy, renderer, PEXRAClearImage, &values);
}

void
testproc(dpy,window,x,y,w,h)
    Display		*dpy;
    Window		window;
    int			x,y,w,h;
{
    image_testproc("accum_state", "Accumulate State", test_description,
        dpy,window,x,y,w,h);
} 
