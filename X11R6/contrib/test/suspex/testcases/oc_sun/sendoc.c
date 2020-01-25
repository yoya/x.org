/* $XConsortium: sendoc.c,v 1.0 93/11/22 12:51:08 rws Exp $ */

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
SendOC Tests:\n\n\
Test EncodedOCs and SendOCs\n\
You should see a wide green dashed polyline\n\
";

int fp = PEXIEEE_754_32;

void
outputCommands(dpy, resourceID, req_type)
    Display		*dpy;
    XID			resourceID;
    PEXOCRequestType	req_type;
{	
    char *oc;
    PEXOCData oc_data[4];
    unsigned long  wc;

    oc_data[0].oc_type = PEXOCLineColor;
    oc_data[0].data.SetLineColor.color_type = PEXColorTypeRGB;
    oc_data[0].data.SetLineColor.color.rgb.red = 0.0;
    oc_data[0].data.SetLineColor.color.rgb.green = 1.0;
    oc_data[0].data.SetLineColor.color.rgb.blue = 0.0;

    oc_data[1].oc_type = PEXOCLineWidth;
    oc_data[1].data.SetLineWidth.width = 2.0;

    oc_data[2].oc_type = PEXOCLineType;
    oc_data[2].data.SetLineType.line_type = PEXLineTypeDashed;

    oc_data[3].oc_type = PEXOCPolyline;
    oc_data[3].data.Polyline.count = 4;
    oc_data[3].data.Polyline.points = points;

    oc = PEXEncodeOCs(fp, 4, oc_data, &wc);
    PEXSendOCs(dpy, resourceID, req_type, fp, 4, wc, oc);
    XFree(oc);
}
 
void
testproc(dpy,window,x,y,w,h)
    Display		*dpy;
    Window		window;
    int			x,y,w,h;
{
    image_testproc("sendoc", "send OCs", test_description, dpy,window,x,y,w,h);
} 

void
misc_setup(dpy, window, renderer)
Display		*dpy;
Window		window;
PEXRenderer	renderer;
{
}
