/* $XConsortium: pfline.c,v 1.0 93/11/22 12:50:56 rws Exp $ */

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

char test_description[] = "\
Encoded OC Tests:\n\n\
Primitives sent as encoded OCs. Also test Noop, Label and ApplicationData\n\
Left:  CellArray 2D\n\
Right: red polyline\n\
";

PEXMatrix matrix;
PEXVector vector;
 
void
outputCommands(dpy, resourceID, req_type)
    Display		*dpy;
    XID			resourceID;
    PEXOCRequestType	req_type;
{	

    char		*enc_data;
    unsigned long	oc_count;
    PEXOCData		oc_data[4];
    long		length;
    PEXTableIndex	c1[6];
    PEXCoord		linepts[4];

    long                llabel = 2; 
    char                appdata[10]; 
	 
    PEXNoop(dpy, resourceID, req_type); 
    PEXLabel(dpy, resourceID, req_type, llabel); 
    PEXApplicationData(dpy, resourceID, req_type, 10, appdata);
    oc_count = 3;

    oc_data[0].oc_type = PEXOCCellArray2D;
    oc_data[0].data.CellArray2D.point1.x = 0.1;
    oc_data[0].data.CellArray2D.point1.y = 0.1;
    oc_data[0].data.CellArray2D.point2.x = 0.4;
    oc_data[0].data.CellArray2D.point2.y = 0.4;
    oc_data[0].data.CellArray2D.col_count = 2;
    oc_data[0].data.CellArray2D.row_count = 3;
    c1[0] = 1;
    c1[1] = 2;
    c1[2] = 3;
    c1[3] = 4;
    c1[4] = 5;
    c1[5] = 6;
    oc_data[0].data.CellArray2D.color_indices = c1;

    oc_data[1].oc_type = PEXOCLineColor;
    oc_data[1].data.SetLineColor.color_type = PEXColorTypeRGB;
    oc_data[1].data.SetLineColor.color.rgb.red = 1.0;
    oc_data[1].data.SetLineColor.color.rgb.green = 0.0;
    oc_data[1].data.SetLineColor.color.rgb.blue = 0.0;

    oc_data[2].oc_type = PEXOCPolyline;
    oc_data[2].data.Polyline.count = 4;
    linepts[0].x = 0.5;
    linepts[0].y = 0.1;
    linepts[0].z = 0.0;
    linepts[1].x = 0.6;
    linepts[1].y = 0.3;
    linepts[1].z = 0.0;
    linepts[2].x = 0.7;
    linepts[2].y = 0.1;
    linepts[2].z = 0.0;
    linepts[3].x = 0.8;
    linepts[3].y = 0.4;
    linepts[3].z = 0.0;
    oc_data[2].data.Polyline.points = linepts;
    enc_data = PEXEncodeOCs(PEXIEEE_754_32, oc_count, oc_data, 
		(unsigned long *)&length);
    PEXSendOCs(dpy, resourceID, req_type, PEXIEEE_754_32, oc_count,
	length, enc_data);
}
 
void
testproc(dpy,window,x,y,w,h)
    Display		*dpy;
    Window		window;
    int			x,y,w,h;
{
    image_testproc("pfline", "preformatted OCs", test_description,
        dpy,window,x,y,w,h);
} 

void
misc_setup(dpy, window, renderer)
Display		*dpy;
Window		window;
PEXRenderer	renderer;
{
}
