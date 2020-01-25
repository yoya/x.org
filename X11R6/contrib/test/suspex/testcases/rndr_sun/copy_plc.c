/* $XConsortium: copy_plc.c,v 1.0 93/11/22 12:53:55 rws Exp $ */

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
#include "surface.h"
#include "line.h"
 
char test_description[] = "\
Copy Pipeline Contexts Tests:\n\n\
This test creates 2 PC, one has line type and line color attributes modified\n\
and then copy to the other. The one that get copied is used by a renderer\n\
to draw a line, which should be rendered in red and dashed.\n\
"; 

void
outputCommands(dpy, resourceID, req_type)
    Display		*dpy;
    XID			resourceID;
    PEXOCRequestType	req_type;
{	
 
    static              PEXCoord pts[] = {
        {0.3, 0.3, 0.5},
        {0.3, 0.7, 0.5},
    };

    POLYLINE(2,pts);
}
 
void
testproc(dpy,window,x,y,w,h)
    Display		*dpy;
    Window		window;
    int			x,y,w,h;
{
    image_testproc("copy_plc", "Copy Pipeline Context", test_description,
        dpy,window,x,y,w,h);
} 

void
misc_setup(dpy, window, renderer)
Display		*dpy;
Window		window;
PEXRenderer	renderer;
{

    unsigned long                       item_mask = 0;
    unsigned long                       valuemask[3];
    PEXPipelineContext                  plc1,plc2;
    PEXPCAttributes                     pc_values;
    PEXRendererAttributes               rndAttrs;
    PEXColorSpecifier colorRed ;

    colorRed.type = PEXColorTypeRGB;
    colorRed.value.rgb.red = 1.0;
    colorRed.value.rgb.green = 0.0;
    colorRed.value.rgb.blue = 0.0;

    valuemask[0] = 0;
    valuemask[1] = 0;
    valuemask[2] = 0;

    plc1 = PEXCreatePipelineContext(dpy, valuemask, &pc_values);
    plc2 = PEXCreatePipelineContext(dpy, valuemask, &pc_values);

    PEXSetPCAttributeMask(valuemask, PEXPCLineColor);
    PEXSetPCAttributeMask(valuemask, PEXPCLineType);

    pc_values.line_type = PEXLineTypeDashed;
    pc_values.line_color = colorRed;
    PEXChangePipelineContext(dpy, plc1, valuemask, &pc_values);

    PEXSetPCAttributeMask(valuemask, PEXPCLineType);
    PEXCopyPipelineContext(dpy, valuemask, plc1, plc2);

    item_mask = PEXRAPipelineContext;
    rndAttrs.pipeline_context = plc2;
    PEXChangeRenderer(dpy, renderer, item_mask, &rndAttrs);
}
