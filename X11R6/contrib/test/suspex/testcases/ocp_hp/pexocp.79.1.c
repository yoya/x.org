/* $XConsortium: pexocp.79.1.c,v 1.0 93/11/22 12:33:02 rws Exp $ */

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

#include <X11/PEX5/PEXlib.h>
#include <misc.h>

/*
 * PEXlib Markers 3D:  Test Case 1 (using RGB colors)
 */

draw_markers(Display          *dpy,
	     XID              resourceID,
	     PEXOCRequestType req_type)
    {
    PEXCoord	points_1[4], points_2[4], points_3[1], points_4[5];
    PEXColor	p_line_color;

    /* Set up some data. */

    points_1[0].x = 0.1;
    points_1[0].y = 0.1;
    points_1[0].z = 0.0;
    points_1[1].x = 0.9;
    points_1[1].y = 0.1;
    points_1[1].z = 0.0;
    points_1[2].x = 0.9;
    points_1[2].y = 0.9;
    points_1[2].z = 0.0;
    points_1[3].x = 0.1;
    points_1[3].y = 0.9;
    points_1[3].z = 0.0;

    points_2[0].x = 0.25;
    points_2[0].y = 0.25;
    points_2[0].z = 0.0;
    points_2[1].x = 0.75;
    points_2[1].y = 0.25;
    points_2[1].z = 0.0;
    points_2[2].x = 0.75;
    points_2[2].y = 0.75;
    points_2[2].z = 0.0;
    points_2[3].x = 0.25;
    points_2[3].y = 0.75;
    points_2[3].z = 0.0;

    points_3[0].x = 0.5;
    points_3[0].y = 0.5;
    points_3[0].z = 0.0;

    points_4[0].x = 0.4;
    points_4[0].y = 0.4;
    points_4[0].z = 0.5;
    points_4[1].x = 0.6;
    points_4[1].y = 0.4;
    points_4[1].z = 0.5;
    points_4[2].x = 0.6;
    points_4[2].y = 0.6;
    points_4[2].z = 0.5;
    points_4[3].x = 0.4;
    points_4[3].y = 0.6;
    points_4[3].z = 0.5;
    points_4[4].x = 0.4;
    points_4[4].y = 0.4;
    points_4[4].z = 0.5;


    PEXSetMarkerScale(dpy, resourceID, req_type, 3.0);
    p_line_color.rgb.red = 1.0;
    p_line_color.rgb.green = 0.0;
    p_line_color.rgb.blue = 0.0;
    PEXSetMarkerColor(dpy, resourceID, req_type, 
		      PEXColorTypeRGB, &p_line_color);
    PEXMarkers(dpy, resourceID, req_type, 4, points_1);

    PEXSetMarkerScale(dpy, resourceID, req_type, 6.0);
    PEXSetMarkerType(dpy, resourceID, req_type, PEXMarkerCircle);
    p_line_color.rgb.red = 0.0;
    p_line_color.rgb.green = 1.0;
    p_line_color.rgb.blue = 0.0;
    PEXSetMarkerColor(dpy, resourceID, req_type, 
		      PEXColorTypeRGB, &p_line_color);
    PEXMarkers(dpy, resourceID, req_type, 4, points_2);

    p_line_color.rgb.red = 0.0;
    p_line_color.rgb.green = 0.0;
    p_line_color.rgb.blue = 1.0;
    PEXSetSurfaceColor(dpy, resourceID, req_type, 
		       PEXColorTypeRGB, &p_line_color);
    PEXSetInteriorStyle(dpy, resourceID, req_type, 
			PEXInteriorStyleSolid);
    PEXFillArea(dpy, resourceID, req_type, 
		PEXShapeNonConvex, False, 5, points_4);

    PEXSetMarkerScale(dpy, resourceID, req_type, 50.0);
    PEXSetMarkerType(dpy, resourceID, req_type, PEXMarkerCross);
    p_line_color.rgb.red = 1.0;
    p_line_color.rgb.green = 0.0;
    p_line_color.rgb.blue = 1.0;
    PEXSetMarkerColor(dpy, resourceID, req_type, 
		      PEXColorTypeRGB, &p_line_color);
    PEXMarkers(dpy, resourceID, req_type, 1, points_3);
    }

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "pexocp.79.1");
    *num_images      = 1;
    *supported_modes = IMM_MODE | STR_MODE | WKS_MODE;

    strcpy(win_title, "pexocp.79.1");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    unsigned long         mask = PEXRAHLHSRMode;
    PEXRendererAttributes attrs;

    attrs.hlhsr_mode = PEXHLHSRZBuffer;

    PEXChangeRenderer(dpy, renderer, mask, &attrs);
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    describe_test( "PEXMarkers Mainline Test:\n" );
    describe_test( "\n" );
    describe_test( "  The image that you should see on the screen consists of\n" );
    describe_test ( "  four red (PEXMarkerAsterisk) markers in each of the window corners\n" );
    describe_test ( "  followed by four green (PEXMarkerCircle) markers in each corner and\n" );
    describe_test ( "  a larger magenta (PEXMarkerCross) marker in the center with a blue Fill\n" );
    describe_test ( "  Area place on top to obscure part of the marker.\n\n" );

    draw_markers(dpy, resourceID, req_type);
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
     image_testproc("pexocp.79.1", "pexocp.79.1", img_desc,
		      dpy,window, 0, 0, 
		      glob_window_width, glob_window_height);
    }
