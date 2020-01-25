/* $XConsortium: lut_dynamics.c,v 1.0 93/11/22 12:54:17 rws Exp $ */

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
#include "misc.h"


char test_description[] = "\
Lut Dynamics Tests:\n\n\
You should see a dotted line above a dashdot line.\n\
";

static	PEXCoord	pts[] = {0.3, 0.3, 0.0, 0.7, 0.3, 0.0};

XID	line_table ;

/* This test case should be expanded to include more tests
   for other lookup tables. Currently, it only tests the
   line lut. 
 */

void
outputCommands(dpy, resourceID, req_type)
    Display             *dpy;
    unsigned long       resourceID; 
    PEXOCRequestType    req_type;
{	
    int			type;
    PEXPointer		entries;

    PEXSetLineBundleIndex(dpy, resourceID, req_type, 2);
    PEXSetIndividualASF(dpy, resourceID, req_type, PEXASFLineType,PEXBundled);
    PEXPolyline(dpy, resourceID, req_type, 2, pts);
    TRANSLATE_VERTEX(y, 0.2, 2, pts)
    PEXGetTableEntries(dpy, line_table, 3, 1, PEXSetValue, &type, &entries);
    PEXSetTableEntries(dpy, line_table, 2, 1, type, entries);
    PEXPolyline(dpy, resourceID, req_type, 2, pts);
    PEXFreeTableEntries(type, 1, entries);
}

 
void
testproc(dpy,window,x,y,w,h)
    Display		*dpy;
    Window		window;
    int			x,y,w,h;
{
    image_testproc("lut_dynamics", "renderer LUT attributes dynamics", 
	test_description, dpy,window,x,y,w,h);
} 

void
misc_setup(dpy, window, renderer)
Display		*dpy;
Window		window;
PEXRenderer	renderer;
{
    PEXRendererAttributes ra;
    int                 i;
    PEXLineBundleEntry  lineEntry[2] = {
        PEXLineTypeDashDot, PEXPolylineInterpColor,
        {PEXApproxImpDep, 0, 0.0}, 5.0,
    };
    PEXColorSpecifier colorRed;

    colorRed.type = PEXColorTypeRGB;
    colorRed.value.rgb.red = 1.0;
    colorRed.value.rgb.green = 0.0;
    colorRed.value.rgb.blue = 0.0;
    lineEntry[0].color = colorRed;
    memcpy((void*)&lineEntry[1], (void*)&lineEntry[0], sizeof(PEXLineBundleEntry));
    lineEntry[1].type = PEXLineTypeDotted;

    line_table = PEXCreateLookupTable(dpy, window, PEXLUTLineBundle);
    PEXSetTableEntries(dpy, line_table, 2, 2, PEXLUTLineBundle, lineEntry);

    ra.line_bundle = line_table;
    PEXChangeRenderer(dpy, renderer, PEXRALineBundle, &ra);
}
