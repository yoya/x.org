/* $XConsortium: renderer.c,v 1.0 93/11/22 12:22:22 rws Exp $ */

/******************************************************************************/
/*                                                                            */
/*  (c) Copyright Hewlett-Packard Company, 1993, Fort Collins, Colorado       */
/*                                                                            */
/*                            All Rights Reserved                             */
/*                                                                            */
/*  Permission to use, copy, modify, and distribute this software and its     */
/*  documentation for any purpose and without fee is hereby granted,          */
/*  provided that the above copyright notices appear in all copies and that   */
/*  both the copyright notices and this permission notice appear in           */
/*  supporting documentation, and that the name of Hewlett-Packard not be     */
/*  used in advertising or publicity pertaining to distribution of the        */
/*  software without specific, written prior permission.                      */
/*                                                                            */
/*  HEWLETT-PACKARD MAKES NO WARRANTY OF ANY KIND WITH REGARD TO THIS         */
/*  SOFTWARE, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF        */
/*  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  Hewlett-Packard    */
/*  shall not be liable for errors contained herein or direct, indirect,      */
/*  special, incidental or consequential damages in connection with the       */
/*  furnishing, performance or use of this software.                          */
/*                                                                            */
/******************************************************************************/

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
/*
	PEXRAPatternTable |
*/
	PEXRATextFontTable |
	PEXRAColorApproxTable | 
	PEXRAColorTable |
	PEXRAViewTable |
	PEXRABackgroundColor | 
	PEXRAClearImage;

    attrs.color_approx_table = colorApproxLUT;
    attrs.color_table = colorLUT;
    attrs.depth_cue_table = dcueLUT;
    attrs.light_table = lightLUT;
    attrs.text_font_table = fontLUT;
/*
    attrs.pattern_table = patternLUT;
*/
    attrs.line_bundle = lineLUT;
    attrs.text_bundle = textLUT;
    attrs.edge_bundle = edgeLUT;
    attrs.interior_bundle = interiorLUT;
    attrs.marker_bundle = markerLUT;
    attrs.view_table = viewLUT;

    attrs.background_color.type = PEXColorTypeRGB;
    attrs.background_color.value.rgb.red = 0.0;
    attrs.background_color.value.rgb.green = 0.0;
    attrs.background_color.value.rgb.blue = 0.0;
    attrs.clear_image = True;

    /* create the renderer */
    renderer = PEXCreateRenderer(dpy, window, mask, &attrs);

    return  (renderer);
}
