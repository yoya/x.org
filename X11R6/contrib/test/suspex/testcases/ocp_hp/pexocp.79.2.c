/* $XConsortium: pexocp.79.2.c,v 1.0 93/11/22 12:33:08 rws Exp $ */

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
 * PEXlib Markers 3D:  Test Case 2 (using RGB colors) - test for pop clipping
 */

draw_markers(Display          *dpy,
	     XID              resourceID,
	     PEXOCRequestType req_type)
    {
      PEXCoord	points_1[1], points_2[1], points_3[1], points_4[1], points_5[1];
      PEXColor	p_marker_color;

      /* Set up some data. */

      points_1[0].x = 0.1;
      points_1[0].y = 0.2;
      points_1[0].z = 0.0;

      points_2[0].x = 0.3;
      points_2[0].y = 0.15;
      points_2[0].z = 0.0;

      points_3[0].x = 0.5;
      points_3[0].y = 0.1;
      points_3[0].z = 0.0;

      points_4[0].x = 0.7;
      points_4[0].y = 0.05;
      points_4[0].z = 0.0;

      points_5[0].x = 0.9;
      points_5[0].y = -0.05;
      points_5[0].z = 0.0;

      p_marker_color.rgb.red = 1.0;
      p_marker_color.rgb.green = 0.0;
      p_marker_color.rgb.blue = 1.0;
      PEXSetMarkerColor(dpy, resourceID, req_type, PEXColorTypeRGB, &p_marker_color);
      PEXSetMarkerType(dpy, resourceID, req_type, PEXMarkerCross);
      PEXSetMarkerScale(dpy, resourceID, req_type, 10.0);
      PEXMarkers(dpy, resourceID, req_type, 1, points_1);
      PEXMarkers(dpy, resourceID, req_type, 1, points_2);
      PEXMarkers(dpy, resourceID, req_type, 1, points_3);
      PEXMarkers(dpy, resourceID, req_type, 1, points_4);
      PEXMarkers(dpy, resourceID, req_type, 1, points_5);
    }

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "pexocp.79.2");
    *num_images      = 1;
    *supported_modes = IMM_MODE | STR_MODE | WKS_MODE;

    strcpy(win_title, "pexocp.79.2");
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
    describe_test("PEXMarkers Mainline Test:\n" );
    describe_test("\n" );
    describe_test("  The image that you should see on the screen consists of\n");
    describe_test("  three magenta (PEXMarkerCross) markers in the lower right\n");
    describe_test("  corner with the fourth and fifth markers clipped since\n");
    describe_test("  their extent or point is outside of the clipping volume\n");
    describe_test("\n");

    draw_markers(dpy, resourceID, req_type);
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
     image_testproc("pexocp.79.2", "pexocp.79.2", img_desc,
		      dpy,window, 0, 0, 
		      glob_window_width, glob_window_height);
    }
