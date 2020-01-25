/* $XConsortium: startoc.c,v 1.0 93/11/22 12:51:25 rws Exp $ */

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
StartOCs, FinishOCs, CopyBytesToOC Tests:\n\n\
You should see a green dashed line and white encoded text.\n\
";

int fp = PEXIEEE_754_32;
#define NUMWORDS(_size) (((unsigned int)((_size) + 3)) >> 2)

PEXCoord2D pt1 = { 0.025, 0.05 };
PEXCoord pt2 = { 0.65, 0.35, 0.0 };
PEXVector vec1 = { 1.0, 0.0, 0.0 };
PEXVector vec2 = { 0.0, 1.0, 0.0 };

PEXVector upvec1[] = {1.0,2.0};
PEXVector upvec2[] = {0.0,-1.0};
PEXVector upvec3[] = {0.0,1.0};

PEXEncodedTextData encoded_text[2];

char *text_string[2] = {"SusPEXing", "PEXlib"};

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
    PEXStartOCs(dpy, resourceID, req_type, fp, 4, NUMWORDS(wc));
    PEXCopyBytesToOC(dpy, wc, oc);
    PEXFinishOCs(dpy);
    XFree(oc);

    oc_data[0].oc_type = PEXOCCharHeight;
    oc_data[0].data.SetCharHeight.height = 0.02;
    oc_data[1].oc_type = PEXOCTextPrecision;
    oc_data[1].data.SetTextPrecision.precision = PEXStrokePrecision;
    oc_data[2].oc_type = PEXOCText;
    oc_data[2].data.EncodedText.origin = pt2;
    oc_data[2].data.EncodedText.vector1 = vec1;
    oc_data[2].data.EncodedText.vector2 = vec2;
    oc_data[2].data.EncodedText.count = 2;
    oc_data[2].data.EncodedText.encoded_text = encoded_text;
    encoded_text[0].character_set = 0;
    encoded_text[0].character_set_width = PEXCSByte;
    encoded_text[0].encoding_state = 0;
    encoded_text[0].length = strlen(text_string[0]);
    encoded_text[0].ch = text_string[0];
    encoded_text[1].character_set = 0;
    encoded_text[1].character_set_width = PEXCSByte;
    encoded_text[1].encoding_state = 0;
    encoded_text[1].length = strlen(text_string[1]);
    encoded_text[1].ch = text_string[1];
    oc = PEXEncodeOCs(fp, 3, oc_data, &wc);
    PEXStartOCs(dpy, resourceID, req_type, fp, 3, NUMWORDS(wc));
    PEXCopyBytesToOC(dpy, wc, oc);
    PEXFinishOCs(dpy);
    XFree(oc);
}
 
void
testproc(dpy,window,x,y,w,h)
    Display		*dpy;
    Window		window;
    int			x,y,w,h;
{
    image_testproc("startoc", "start OCs", test_description,
	    dpy,window,x,y,w,h);
} 

void
misc_setup(dpy, window, renderer)
Display		*dpy;
Window		window;
PEXRenderer	renderer;
{
}
