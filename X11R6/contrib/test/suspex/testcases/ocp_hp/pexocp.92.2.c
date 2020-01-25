/* $XConsortium: pexocp.92.2.c,v 1.0 93/11/22 12:35:52 rws Exp $ */

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
 * PEXlib FillAreaSet 3D:  Test Case 2 (fewer than 3 vertices)
 */

draw_image  (Display          *dpy,
	     XID              resourceID,
	     PEXOCRequestType req_type)
    {
      PEXCoord				points_1[5], points_2[3], points_3[2];
      PEXCoord				points_4[1], points_5[8], points_6[5];
      PEXListOfCoord			point_list_1[4], point_list_2[2];
      PEXColor				p_surface_color, p_edge_color;

      /* Set up some data. */

      points_1[0].x = 0.1; points_1[0].y = 0.1; points_1[0].z = 0.0;
      points_1[1].x = 0.3; points_1[1].y = 0.1; points_1[1].z = 0.0;
      points_1[2].x = 0.3; points_1[2].y = 0.3; points_1[2].z = 0.0;
      points_1[3].x = 0.1; points_1[3].y = 0.3; points_1[3].z = 0.0;
      points_1[4].x = 0.1; points_1[4].y = 0.1; points_1[4].z = 0.0;

      points_2[0].x = 0.7; points_2[0].y = 0.1; points_2[0].z = 0.0;
      points_2[1].x = 0.9; points_2[1].y = 0.1; points_2[1].z = 0.0;
      points_2[2].x = 0.9; points_2[2].y = 0.3; points_2[2].z = 0.0;

      points_3[0].x = 0.7; points_3[0].y = 0.7; points_3[0].z = 0.0;
      points_3[1].x = 0.9; points_3[1].y = 0.7; points_3[1].z = 0.0;

      points_4[0].x = 0.1; points_4[0].y = 0.7; points_4[0].z = 0.0;

      points_5[0].x = 0.45; points_5[0].y = 0.45; points_5[0].z = 0.5;
      points_5[1].x = 0.50; points_5[1].y = 0.25; points_5[1].z = 0.5;
      points_5[2].x = 0.55; points_5[2].y = 0.45; points_5[2].z = 0.5;
      points_5[3].x = 0.75; points_5[3].y = 0.50; points_5[3].z = 0.5;
      points_5[4].x = 0.55; points_5[4].y = 0.55; points_5[4].z = 0.5;
      points_5[5].x = 0.50; points_5[5].y = 0.75; points_5[5].z = 0.5;
      points_5[6].x = 0.45; points_5[6].y = 0.55; points_5[6].z = 0.5;
      points_5[7].x = 0.25; points_5[7].y = 0.50; points_5[7].z = 0.5;

      /* Now, hook up the points into lists. */
      point_list_1[0].count = 5;
      point_list_1[0].points = points_1;
      point_list_1[1].count = 3;
      point_list_1[1].points = points_2;
      point_list_1[2].count = 2;
      point_list_1[2].points = points_3;
      point_list_1[3].count = 1;
      point_list_1[3].points = points_4;

      point_list_2[0].count = 8;
      point_list_2[0].points = points_5;
      point_list_2[1].count = 0;
      point_list_2[1].points = NULL;

      PEXSetSurfaceEdgeFlag(dpy, resourceID, req_type, PEXOn);

      p_surface_color.rgb.red = 0.0;
      p_surface_color.rgb.green = 0.0;
      p_surface_color.rgb.blue = 1.0;
      PEXSetSurfaceColor(dpy, resourceID, req_type, 
			 PEXColorTypeRGB, &p_surface_color);
      PEXSetInteriorStyle(dpy, resourceID, req_type, PEXInteriorStyleSolid);
      PEXFillAreaSet(dpy, resourceID, req_type, 
		     PEXShapeNonConvex, False, PEXContourDisjoint, 
		     4, point_list_1);

      p_edge_color.rgb.red = 1.0;
      p_edge_color.rgb.green = 0.0;
      p_edge_color.rgb.blue = 0.0;
      PEXSetSurfaceEdgeColor(dpy, resourceID, req_type, 
			     PEXColorTypeRGB, &p_edge_color);
      p_surface_color.rgb.red = 0.0;
      p_surface_color.rgb.green = 1.0;
      p_surface_color.rgb.blue = 0.0;
      PEXSetSurfaceColor(dpy, resourceID, req_type, 
			 PEXColorTypeRGB, &p_surface_color);
      PEXFillAreaSet(dpy, resourceID, req_type, 
		     PEXShapeNonConvex, False, PEXContourIntersecting, 
		     2, point_list_2);
    }

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "pexocp.92.2");
    *num_images      = 1;
    *supported_modes = IMM_MODE | STR_MODE | WKS_MODE;

    strcpy(win_title, "pexocp.92.2");
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
    describe_test(" \nPEXFillAreaSet with < 2 points\n");
    describe_test(" The test should create a blank image.\n\n");

    draw_image(dpy, resourceID, req_type);
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
     image_testproc("pexocp.92.2", "pexocp.92.2", img_desc,
		      dpy,window, 0, 0, 
		      glob_window_width, glob_window_height);
    }
