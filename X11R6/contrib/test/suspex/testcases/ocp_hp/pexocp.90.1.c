/* $XConsortium: pexocp.90.1.c,v 1.0 93/11/22 12:34:53 rws Exp $ */

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
 * PEXlib FillArea 2D:  Test Case 1 (using RGB colors)
 */

draw_image  (Display          *dpy,
	     XID              resourceID,
	     PEXOCRequestType req_type)
    {
      PEXCoord2D			points_1[5], points_2[4], points_3[4];
      PEXCoord2D			points_4[5], points_6[5];
      PEXCoord				points_5[8];
      PEXColor				p_surface_color, p_edge_color;

      /* Set up some data. */

      points_1[0].x = 0.1; points_1[0].y = 0.1;
      points_1[1].x = 0.3; points_1[1].y = 0.1;
      points_1[2].x = 0.3; points_1[2].y = 0.3;
      points_1[3].x = 0.1; points_1[3].y = 0.3;
      points_1[4].x = 0.1; points_1[4].y = 0.1;

      points_2[0].x = 0.7; points_2[0].y = 0.1;
      points_2[1].x = 0.9; points_2[1].y = 0.1;
      points_2[2].x = 0.9; points_2[2].y = 0.3;
      points_2[3].x = 0.7; points_2[3].y = 0.3;

      points_3[0].x = 0.7; points_3[0].y = 0.7;
      points_3[1].x = 0.9; points_3[1].y = 0.7;
      points_3[2].x = 0.9; points_3[2].y = 0.9;
      points_3[3].x = 0.7; points_3[3].y = 0.9;

      points_4[0].x = 0.1; points_4[0].y = 0.7;
      points_4[1].x = 0.3; points_4[1].y = 0.7;
      points_4[2].x = 0.3; points_4[2].y = 0.9;
      points_4[3].x = 0.1; points_4[3].y = 0.9;
      points_4[4].x = 0.1; points_4[4].y = 0.7;

      points_5[0].x = 0.45; points_5[0].y = 0.45; points_5[0].z = 0.50; 
      points_5[1].x = 0.50; points_5[1].y = 0.25; points_5[1].z = 0.5;
      points_5[2].x = 0.55; points_5[2].y = 0.45; points_5[2].z = 0.50; 
      points_5[3].x = 0.75; points_5[3].y = 0.50; points_5[3].z = 0.5;
      points_5[4].x = 0.55; points_5[4].y = 0.55; points_5[4].z = 0.5;
      points_5[5].x = 0.50; points_5[5].y = 0.75; points_5[5].z = 0.5;
      points_5[6].x = 0.45; points_5[6].y = 0.55; points_5[6].z = 0.5;
      points_5[7].x = 0.25; points_5[7].y = 0.50; points_5[7].z = 0.5;

      points_6[0].x = 0.4; points_6[0].y = 0.4; 
      points_6[1].x = 0.6; points_6[1].y = 0.4; 
      points_6[2].x = 0.6; points_6[2].y = 0.6;
      points_6[3].x = 0.4; points_6[3].y = 0.6; 
      points_6[4].x = 0.4; points_6[4].y = 0.4; 

      PEXSetSurfaceEdgeFlag(dpy, resourceID, req_type, PEXOn);

      p_surface_color.rgb.red = 1.0;
      p_surface_color.rgb.green = 0.0;
      p_surface_color.rgb.blue = 1.0;
      PEXSetSurfaceColor(dpy, resourceID, req_type, PEXColorTypeRGB, &p_surface_color);
      PEXSetInteriorStyle(dpy, resourceID, req_type, PEXInteriorStyleHollow);
      PEXFillArea2D(dpy, resourceID, req_type, PEXShapeNonConvex, True, 5, points_1);

      p_surface_color.rgb.red = 0.0;
      p_surface_color.rgb.green = 0.0;
      p_surface_color.rgb.blue = 1.0;
      PEXSetSurfaceColor(dpy, resourceID, req_type, PEXColorTypeRGB, &p_surface_color);
      PEXFillArea2D(dpy, resourceID, req_type, PEXShapeNonConvex, False, 4, points_2);
      PEXSetInteriorStyle(dpy, resourceID, req_type, PEXInteriorStyleSolid);
      PEXFillArea2D(dpy, resourceID, req_type, PEXShapeNonConvex, True, 4, points_3);

      PEXFillArea2D(dpy, resourceID, req_type, PEXShapeNonConvex, False, 5, points_4);

      p_edge_color.rgb.red = 1.0;
      p_edge_color.rgb.green = 0.0;
      p_edge_color.rgb.blue = 0.0;
      PEXSetSurfaceEdgeColor(dpy, resourceID, req_type, PEXColorTypeRGB, &p_edge_color);
      p_surface_color.rgb.red = 0.0;
      p_surface_color.rgb.green = 1.0;
      p_surface_color.rgb.blue = 0.0;
      PEXSetSurfaceColor(dpy, resourceID, req_type, PEXColorTypeRGB, &p_surface_color);
      PEXFillArea(dpy, resourceID, req_type, PEXShapeNonConvex, False, 8, points_5);

      p_edge_color.rgb.red = 1.0;
      p_edge_color.rgb.green = 0.0;
      p_edge_color.rgb.blue = 1.0;
      PEXSetSurfaceEdgeColor(dpy, resourceID, req_type, PEXColorTypeRGB, &p_edge_color);
      p_surface_color.rgb.red = 1.0;
      p_surface_color.rgb.green = 1.0;
      p_surface_color.rgb.blue = 1.0;
      PEXSetSurfaceColor(dpy, resourceID, req_type, PEXColorTypeRGB, &p_surface_color);
      PEXFillArea2D(dpy, resourceID, req_type, PEXShapeNonConvex, False, 5, points_6);
    }


void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "pexocp.90.1");
    *num_images      = 1;
    *supported_modes = IMM_MODE | STR_MODE | WKS_MODE;

    strcpy(win_title, "pexocp.90.1");
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
    describe_test(" \nPEXFillArea2D with RGB Colors\n");
    describe_test(" The image you should see consists of 2 solid blue squares\n");
    describe_test(" in the upper left / right corners.  The left square\n");
    describe_test(" is edged in white and the right is not edged.\n");
    describe_test(" There are 2 hollow squares in the lower left / right\n");
    describe_test(" corners that are magenta and white respectively.\n");
    describe_test(" In the center of the image, a solid green star with red\n");
    describe_test(" edges lies on top of a white solid square with magenta\n");
    describe_test(" edges.\n\n");

    draw_image(dpy, resourceID, req_type);
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
     image_testproc("pexocp.90.1", "pexocp.90.1", img_desc,
		      dpy,window, 0, 0, 
		      glob_window_width, glob_window_height);
    }
