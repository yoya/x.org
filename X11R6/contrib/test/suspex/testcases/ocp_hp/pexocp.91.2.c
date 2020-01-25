/* $XConsortium: pexocp.91.2.c,v 1.0 93/11/22 12:35:10 rws Exp $ */

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
 * PEXlib FillArea 3D with Data:  Test Case 2 (RGB Data - Vertex and Facet)
 */

draw_image  (Display          *dpy,
	     XID              resourceID,
	     PEXOCRequestType req_type)
    {
      PEXVertexRGB			points_1[5], points_2[4];
      PEXCoord			        points_3[4], points_4[5];
      PEXArrayOfVertex			vertex_1, vertex_2, vertex_3;
      PEXArrayOfVertex			vertex_4;
      PEXFacetData			facet_3, facet_4;
      PEXColor				p_surface_color, p_edge_color;

      /* Set up some data. */

      vertex_1.rgb = points_1;
      vertex_2.rgb = points_2;
      vertex_3.no_data = points_3;
      vertex_4.no_data = points_4;

      facet_3.rgb.red = 1.0;
      facet_3.rgb.green = 0.0;
      facet_3.rgb.blue = 0.0;

      facet_4.rgb.red = 0.0;
      facet_4.rgb.green = 0.0;
      facet_4.rgb.blue = 1.0;

      points_1[0].point.x = 0.1; 
      points_1[0].point.y = 0.1; 
      points_1[0].point.z = 0.0;
      points_1[0].rgb.red = 1.0;
      points_1[0].rgb.green = 1.0;
      points_1[0].rgb.blue = 1.0;
      points_1[1].point.x = 0.3;
      points_1[1].point.y = 0.1;
      points_1[1].point.z = 0.0;
      points_1[1].rgb.red = 1.0;
      points_1[1].rgb.green = 0.0;
      points_1[1].rgb.blue = 0.0;
      points_1[2].point.x = 0.3;
      points_1[2].point.y = 0.3;
      points_1[2].point.z = 0.0;
      points_1[2].rgb.red = 0.0;
      points_1[2].rgb.green = 1.0;
      points_1[2].rgb.blue = 0.0;
      points_1[3].point.x = 0.1;
      points_1[3].point.y = 0.3;
      points_1[3].point.z = 0.0;
      points_1[3].rgb.red = 0.0;
      points_1[3].rgb.green = 0.0;
      points_1[3].rgb.blue = 1.0;
      points_1[4].point.x = 0.1;
      points_1[4].point.y = 0.1;
      points_1[4].point.z = 0.0;
      points_1[4].rgb.red = 1.0;
      points_1[4].rgb.green = 1.0;
      points_1[4].rgb.blue = 1.0;

      points_2[0].point.x = 0.7;
      points_2[0].point.y = 0.1;
      points_2[0].point.z = 0.0;
      points_2[0].rgb.red = 1.0;
      points_2[0].rgb.green = 1.0;
      points_2[0].rgb.blue = 1.0;
      points_2[1].point.x = 0.9;
      points_2[1].point.y = 0.1;
      points_2[1].point.z = 0.0;
      points_2[1].rgb.red = 1.0;
      points_2[1].rgb.green = 0.0;
      points_2[1].rgb.blue = 0.0;
      points_2[2].point.x = 0.9;
      points_2[2].point.y = 0.3;
      points_2[2].point.z = 0.0;
      points_2[2].rgb.red = 0.0;
      points_2[2].rgb.green = 1.0;
      points_2[2].rgb.blue = 0.0;
      points_2[3].point.x = 0.7;
      points_2[3].point.y = 0.3;
      points_2[3].point.z = 0.0;
      points_2[3].rgb.red = 0.0;
      points_2[3].rgb.green = 0.0;
      points_2[3].rgb.blue = 1.0;

      points_3[0].x = 0.7; points_3[0].y = 0.7; points_3[0].z = 0.0;
      points_3[1].x = 0.9; points_3[1].y = 0.7; points_3[1].z = 0.0;
      points_3[2].x = 0.9; points_3[2].y = 0.9; points_3[2].z = 0.0;
      points_3[3].x = 0.7; points_3[3].y = 0.9; points_3[3].z = 0.0;

      points_4[0].x = 0.1; points_4[0].y = 0.7; points_4[0].z = 0.0;
      points_4[1].x = 0.3; points_4[1].y = 0.7; points_4[1].z = 0.0;
      points_4[2].x = 0.3; points_4[2].y = 0.9; points_4[2].z = 0.0;
      points_4[3].x = 0.1; points_4[3].y = 0.9; points_4[3].z = 0.0;
      points_4[4].x = 0.1; points_4[4].y = 0.7; points_4[4].z = 0.0;

      PEXSetSurfaceEdgeFlag(dpy, resourceID, req_type, PEXOn);
      PEXSetSurfaceInterpMethod(dpy, resourceID, req_type, PEXSurfaceInterpColor);

      p_surface_color.rgb.red = 1.0;
      p_surface_color.rgb.green = 0.0;
      p_surface_color.rgb.blue = 1.0;
      PEXSetSurfaceColor(dpy, resourceID, req_type, PEXColorTypeRGB, &p_surface_color);
      PEXSetInteriorStyle(dpy, resourceID, req_type, PEXInteriorStyleHollow);

      /* Color data should overresourceIDe surface color. */
      PEXFillAreaWithData(dpy, resourceID, req_type, PEXShapeNonConvex, True, PEXGANone, PEXGAColor,
			  PEXColorTypeRGB, NULL, 5, vertex_1);

      PEXSetInteriorStyle(dpy, resourceID, req_type, PEXInteriorStyleSolid);
      p_edge_color.rgb.red = 1.0;
      p_edge_color.rgb.green = 0.0;
      p_edge_color.rgb.blue = 0.0;
      PEXSetSurfaceEdgeColor(dpy, resourceID, req_type, PEXColorTypeRGB, &p_edge_color);
      p_surface_color.rgb.red = 0.0;
      p_surface_color.rgb.green = 0.0;
      p_surface_color.rgb.blue = 1.0;
      PEXSetSurfaceColor(dpy, resourceID, req_type, PEXColorTypeRGB, &p_surface_color);

      /* Color data should overresourceIDe surface color. */
      PEXFillAreaWithData(dpy, resourceID, req_type, PEXShapeNonConvex, False, PEXGANone, PEXGAColor,
			  PEXColorTypeRGB, NULL, 4, vertex_2);

      PEXFillAreaWithData(dpy, resourceID, req_type, PEXShapeNonConvex, True, PEXGAColor, PEXGANone,
			  PEXColorTypeRGB, &facet_3, 4, vertex_3);

      p_edge_color.rgb.red = 0.0;
      p_edge_color.rgb.green = 1.0;
      p_edge_color.rgb.blue = 0.0;
      PEXSetSurfaceEdgeColor(dpy, resourceID, req_type, PEXColorTypeRGB, &p_edge_color);
      PEXFillAreaWithData(dpy, resourceID, req_type, PEXShapeNonConvex, False, PEXGAColor, PEXGANone,
			  PEXColorTypeRGB, &facet_4, 5, vertex_4);
    }

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "pexocp.91.2");
    *num_images      = 1;
    *supported_modes = IMM_MODE | STR_MODE | WKS_MODE;

    strcpy(win_title, "pexocp.91.2");
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
    describe_test(" \nPEXFillAreaWithData (RGB Data - Vertex and Facet)\n");
    describe_test(" The image you should see consists of 4 squares in\n");
    describe_test(" each corner of the window as follows:\n");
    describe_test(" Upper left  -- solid blue with green edging\n");
    describe_test(" Upper right -- solid red with no edges\n");
    describe_test(" Lower left  -- hollow interp  blue       green\n");
    describe_test("                               white      red  \n");
    describe_test(" Lower right -- solid  interp  blue       green\n");
    describe_test("                               white      red  \n");
    describe_test("                with red edge\n\n");

    draw_image(dpy, resourceID, req_type);
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
     image_testproc("pexocp.91.2", "pexocp.91.2", img_desc,
		      dpy,window, 0, 0, 
		      glob_window_width, glob_window_height);
    }
