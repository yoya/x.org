/* $XConsortium: pexocp.87.1.c,v 1.0 93/11/22 12:33:55 rws Exp $ */

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
 * PEXlib Polyline 3D with data:  Test Case 1 (using RGB colors)
 */

draw_polylines(Display          *dpy,
	       XID              resourceID,
	       PEXOCRequestType req_type)
    {
      PEXListOfVertex			polyline_list_1[2], polyline_list_2[2];
      PEXCoord				points_3[5];
      PEXVertexRGB			polyline_1[5], polyline_2[5];
      PEXVertexRGB			polyline_3[2], polyline_4[2];
      PEXColor				p_line_color;

      /* Set up some data. */
      polyline_list_1[0].count = 5;
      polyline_list_1[0].vertices.rgb = polyline_1;
      polyline_list_1[1].count = 5;
      polyline_list_1[1].vertices.rgb = polyline_2;

      polyline_list_2[0].count = 2;
      polyline_list_2[0].vertices.rgb = polyline_3;
      polyline_list_2[1].count = 2;
      polyline_list_2[1].vertices.rgb = polyline_4;

      polyline_1[0].point.x = 0.1;
      polyline_1[0].point.y = 0.1;
      polyline_1[0].point.z = 0.0;
      polyline_1[0].rgb.red = 1.0;
      polyline_1[0].rgb.green = 1.0;
      polyline_1[0].rgb.blue = 1.0;
      polyline_1[1].point.x = 0.9;
      polyline_1[1].point.y = 0.1;
      polyline_1[1].point.z = 0.0;
      polyline_1[1].rgb.red = 0.0;
      polyline_1[1].rgb.green = 0.0;
      polyline_1[1].rgb.blue = 0.0;
      polyline_1[2].point.x = 0.9;
      polyline_1[2].point.y = 0.9;
      polyline_1[2].point.z = 0.0;
      polyline_1[2].rgb.red = 1.0;
      polyline_1[2].rgb.green = 0.0;
      polyline_1[2].rgb.blue = 0.0;
      polyline_1[3].point.x = 0.1;
      polyline_1[3].point.y = 0.9;
      polyline_1[3].point.z = 0.0;
      polyline_1[3].rgb.red = 0.0;
      polyline_1[3].rgb.green = 1.0;
      polyline_1[3].rgb.blue = 0.0;
      polyline_1[4].point.x = 0.1;
      polyline_1[4].point.y = 0.1;
      polyline_1[4].point.z = 0.0;
      polyline_1[4].rgb.red = 0.0;
      polyline_1[4].rgb.green = 0.0;
      polyline_1[4].rgb.blue = 1.0;

      polyline_2[0].point.x = 0.25;
      polyline_2[0].point.y = 0.25;
      polyline_2[0].point.z = 0.0;
      polyline_2[0].rgb.red = 1.0;
      polyline_2[0].rgb.green = 1.0;
      polyline_2[0].rgb.blue = 1.0;
      polyline_2[1].point.x = 0.75;
      polyline_2[1].point.y = 0.25;
      polyline_2[1].point.z = 0.0;
      polyline_2[1].rgb.red = 0.0;
      polyline_2[1].rgb.green = 1.0;
      polyline_2[1].rgb.blue = 1.0;
      polyline_2[2].point.x = 0.75;
      polyline_2[2].point.y = 0.75;
      polyline_2[2].point.z = 0.0;
      polyline_2[2].rgb.red = 1.0;
      polyline_2[2].rgb.green = 0.0;
      polyline_2[2].rgb.blue = 1.0;
      polyline_2[3].point.x = 0.25;
      polyline_2[3].point.y = 0.75;
      polyline_2[3].point.z = 0.0;
      polyline_2[3].rgb.red = 1.0;
      polyline_2[3].rgb.green = 1.0;
      polyline_2[3].rgb.blue = 0.0;
      polyline_2[4].point.x = 0.25;
      polyline_2[4].point.y = 0.25;
      polyline_2[4].point.z = 0.0;
      polyline_2[4].rgb.red = 1.0;
      polyline_2[4].rgb.green = 1.0;
      polyline_2[4].rgb.blue = 1.0;

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

      polyline_3[0].point.x = 0.1;
      polyline_3[0].point.y = 0.1;
      polyline_3[0].point.z = 0.0;
      polyline_3[0].rgb.red = 1.0;
      polyline_3[0].rgb.green = 0.0;
      polyline_3[0].rgb.blue = 0.0;
      polyline_3[1].point.x = 0.9;
      polyline_3[1].point.y = 0.9;
      polyline_3[1].point.z = 0.0;
      polyline_3[1].rgb.red = 1.0;
      polyline_3[1].rgb.green = 1.0;
      polyline_3[1].rgb.blue = 1.0;

      polyline_4[0].point.x = 0.9;
      polyline_4[0].point.y = 0.1;
      polyline_4[0].point.z = 0.0;
      polyline_4[0].rgb.red = 0.0;
      polyline_4[0].rgb.green = 0.0;
      polyline_4[0].rgb.blue = 1.0;
      polyline_4[1].point.x = 0.1;
      polyline_4[1].point.y = 0.9;
      polyline_4[1].point.z = 0.0;
      polyline_4[1].rgb.red = 1.0;
      polyline_4[1].rgb.green = 1.0;
      polyline_4[1].rgb.blue = 1.0;

      p_line_color.rgb.red = 0.0;
      p_line_color.rgb.green = 0.0;
      p_line_color.rgb.blue = 1.0;
      PEXSetSurfaceColor(dpy, resourceID, req_type, PEXColorTypeRGB, &p_line_color);
      PEXSetInteriorStyle(dpy, resourceID, req_type, PEXInteriorStyleSolid);
      PEXFillArea(dpy, resourceID, req_type, PEXShapeNonConvex, False, 5, points_3);

      PEXSetPolylineInterpMethod(dpy, resourceID, req_type, PEXPolylineInterpColor);
      PEXPolylineSetWithData(dpy, resourceID, req_type, PEXGAColor, PEXColorTypeRGB, 2, polyline_list_1);
      PEXSetPolylineInterpMethod(dpy, resourceID, req_type, PEXPolylineInterpNone);
      PEXPolylineSetWithData(dpy, resourceID, req_type, PEXGAColor, PEXColorTypeRGB, 2, polyline_list_2);
    }


void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "pexocp.87.1");
    *num_images      = 1;
    *supported_modes = IMM_MODE | STR_MODE | WKS_MODE;

    strcpy(win_title, "pexocp.87.1");
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
    describe_test( "PEXPolylineSetWithData Mainline Test:\n" );
    describe_test( "\n" );
    describe_test( "  The image that should be visible consists of an outer rectangle made\n" );
    describe_test( "  up of polylines with interpolated color, a second middle rectangle made\n" );
    describe_test( "  up of polylines with interpolated color, two diagonal polylines (red and\n" );
    describe_test( "  blue) passing through the corners of all of the rectangles, and a small\n" );
    describe_test( "  blue fill area in the middle placed on top of the diagonals.\n" );
    describe_test( "                                                      \n");
    describe_test( "  Green                                    Red        \n");
    describe_test( "                                                      \n");
    describe_test( "       Yellow                       Magenta           \n");
    describe_test( "                                                      \n");
    describe_test( "       White                        Cyan              \n");
    describe_test( "                                                      \n");
    describe_test( "  Blue / White                           Black        \n");
    describe_test( "\n" );

    draw_polylines(dpy, resourceID, req_type);
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
     image_testproc("pexocp.87.1", "pexocp.87.1", img_desc,
		      dpy,window, 0, 0, 
		      glob_window_width, glob_window_height);
    }
