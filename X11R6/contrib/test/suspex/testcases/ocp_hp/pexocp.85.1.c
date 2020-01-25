/* $XConsortium: pexocp.85.1.c,v 1.0 93/11/22 12:33:20 rws Exp $ */

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
 * PEXlib Polyline 3D:  Test Case 1 (using RGB colors)
 */

draw_polylines(Display          *dpy,
	       XID              resourceID,
	       PEXOCRequestType req_type)
    {
      PEXCoord				points_1[5], points_2[5], points_3[5];
      PEXCoord				points_4[2], points_5[2];
      PEXColor				p_line_color;

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
      points_1[4].x = 0.1;
      points_1[4].y = 0.1;
      points_1[4].z = 0.0;

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
      points_2[4].x = 0.25;
      points_2[4].y = 0.25;
      points_2[4].z = 0.0;

      points_3[0].x = 0.4;
      points_3[0].y = 0.4;
      points_3[0].z = 0.5;
      points_3[1].x = 0.6;
      points_3[1].y = 0.4;
      points_3[1].z = 0.5;
      points_3[2].x = 0.6;
      points_3[2].y = 0.6;
      points_3[2].z = 0.5;
      points_3[3].x = 0.4;
      points_3[3].y = 0.6;
      points_3[3].z = 0.5;
      points_3[4].x = 0.4;
      points_3[4].y = 0.4;
      points_3[4].z = 0.5;

      points_4[0].x = 0.1;
      points_4[0].y = 0.1;
      points_4[0].z = 0.0;
      points_4[1].x = 0.9;
      points_4[1].y = 0.9;
      points_4[1].z = 0.0;

      points_5[0].x = 0.1;
      points_5[0].y = 0.9;
      points_5[0].z = 0.0;
      points_5[1].x = 0.9;
      points_5[1].y = 0.1;
      points_5[1].z = 0.0;


      p_line_color.rgb.red = 1.0;
      p_line_color.rgb.green = 0.0;
      p_line_color.rgb.blue = 0.0;
      PEXSetLineColor(dpy, resourceID, req_type, PEXColorTypeRGB, &p_line_color);
      PEXPolyline(dpy, resourceID, req_type, 5, points_1);

      p_line_color.rgb.red = 0.0;
      p_line_color.rgb.green = 1.0;
      p_line_color.rgb.blue = 0.0;
      PEXSetLineColor(dpy, resourceID, req_type, PEXColorTypeRGB, &p_line_color);
      PEXPolyline(dpy, resourceID, req_type, 5, points_2);
    
      p_line_color.rgb.red = 0.0;
      p_line_color.rgb.green = 0.0;
      p_line_color.rgb.blue = 1.0;
      PEXSetSurfaceColor(dpy, resourceID, req_type, PEXColorTypeRGB, &p_line_color);
      PEXSetInteriorStyle(dpy, resourceID, req_type, PEXInteriorStyleSolid);
      PEXFillArea(dpy, resourceID, req_type, PEXShapeNonConvex, False, 5, points_3);

      p_line_color.rgb.red = 1.0;
      p_line_color.rgb.green = 1.0;
      p_line_color.rgb.blue = 1.0;
      PEXSetLineColor(dpy, resourceID, req_type, PEXColorTypeRGB, &p_line_color);
      PEXPolyline(dpy, resourceID, req_type, 2, points_4);
      PEXPolyline(dpy, resourceID, req_type, 2, points_5);
    }

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "pexocp.85.1");
    *num_images      = 1;
    *supported_modes = IMM_MODE | STR_MODE | WKS_MODE;

    strcpy(win_title, "pexocp.85.1");
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
    describe_test( "PEXPolyline Mainline Test:\n" );
    describe_test( "\n" );
    describe_test( "  The image that should be visible consists of an outer rectangle made\n" );
    describe_test( "  up of red polylines, a middle rectangle made up of green polylines,\n" );
    describe_test( "  two diagonal white polylines passing through the corners of all\n" );
    describe_test( "  of the rectangles, and a small blue fill area in the middle placed\n" );
    describe_test( "  on top of the white diagonals.\n" );
    describe_test( "\n" );


    draw_polylines(dpy, resourceID, req_type);
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
     image_testproc("pexocp.85.1", "pexocp.85.1", img_desc,
		      dpy,window, 0, 0, 
		      glob_window_width, glob_window_height);
    }
