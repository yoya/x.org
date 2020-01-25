/* $XConsortium: renderer.c,v 1.0 93/11/22 12:25:41 rws Exp $ */

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
#include <X11/Xlib.h>
#include <X11/PEX5/PEXlib.h>
#include <stdio.h>
#include "lut.h"

PEXRenderer
setup_renderer(dpy, window)
    Display		*dpy;
    Window		window;
{
    unsigned long	mask;
    PEXRenderer		renderer;
    PEXRendererAttributes attrs;

    mask = 
	PEXRALineBundle |
	PEXRAMarkerBundle |
	PEXRATextBundle |
	PEXRAInteriorBundle |
	PEXRAEdgeBundle |
	PEXRALightTable |
	PEXRADepthCueTable |
	PEXRAPatternTable |
	PEXRATextFontTable |
	PEXRAColorApproxTable | 
	PEXRAColorTable;

    attrs.color_approx_table = colorApproxLUT;
    attrs.color_table = colorLUT;
    attrs.depth_cue_table = dcueLUT;
    attrs.light_table = lightLUT;
    attrs.text_font_table = fontLUT;
    attrs.pattern_table = patternLUT;
    attrs.line_bundle = lineLUT;
    attrs.text_bundle = textLUT;
    attrs.edge_bundle = edgeLUT;
    attrs.interior_bundle = interiorLUT;
    attrs.marker_bundle = markerLUT;

    /* create the renderer */
    renderer = PEXCreateRenderer(dpy, window, mask, &attrs);

    return  (renderer);
}
