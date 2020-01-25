/* $XConsortium: pexocp.91.3.c,v 1.0 93/11/22 12:35:16 rws Exp $ */

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
#include <lut.h>

/* 
 * PEXlib FillArea 3D with Data:  Test Case 3 (Normals - Vertex and Facet)
 */

draw_image  (Display          *dpy,
	     XID              resourceID,
	     PEXOCRequestType req_type)
    {
      PEXVertexNormal			points_1[5], points_2[4];
      PEXVertexNormal		        points_3[4];
      PEXCoord				points_4[5];
      PEXArrayOfVertex			vertex_1, vertex_2, vertex_3;
      PEXArrayOfVertex			vertex_4;
      PEXFacetData			facet_3, facet_4;
      PEXColor				p_surface_color, p_edge_color;
      PEXTableIndex			lights_on[4];

      /* Set up some data. */

      vertex_1.normal = points_1;
      vertex_2.normal = points_2;
      vertex_3.normal = points_3;
      vertex_4.no_data = points_4;

      facet_4.normal.x = 0.0;
      facet_4.normal.y = -0.7071068;
      facet_4.normal.z = 0.7071068;

      points_1[0].point.x = 0.1;
      points_1[0].point.y = 0.1;
      points_1[0].point.z = 0.0;
      points_1[0].normal.x = -0.6614378;
      points_1[0].normal.y = -0.6614378;
      points_1[0].normal.z = 0.3535533;
      points_1[1].point.x = 0.3;
      points_1[1].point.y = 0.1;
      points_1[1].point.z = 0.0;
      points_1[1].normal.x = -0.6614378;
      points_1[1].normal.y = -0.6614378;
      points_1[1].normal.z = 0.3535533;
      points_1[2].point.x = 0.3;
      points_1[2].point.y = 0.3;
      points_1[2].point.z = 0.0;
      points_1[2].normal.x = 0.0;
      points_1[2].normal.y = 0.0;
      points_1[2].normal.z = 1.0;
      points_1[3].point.x = 0.1;
      points_1[3].point.y = 0.3;
      points_1[3].point.z = 0.0;
      points_1[3].normal.x = -0.6614378;
      points_1[3].normal.y = -0.6614378;
      points_1[3].normal.z = 0.3535533;
      points_1[4].point.x = 0.1;
      points_1[4].point.y = 0.1;
      points_1[4].point.z = 0.0;
      points_1[4].normal.x = -0.6614378;
      points_1[4].normal.y = -0.6614378;
      points_1[4].normal.z = 0.3535533;

      points_2[0].point.x = 0.7;
      points_2[0].point.y = 0.1;
      points_2[0].point.z = 0.0;
      points_2[0].normal.x = 0.5773502;
      points_2[0].normal.y = -0.5773502;
      points_2[0].normal.z = 0.5773502;
      points_2[1].point.x = 0.9;
      points_2[1].point.y = 0.1;
      points_2[1].point.z = 0.0;
      points_2[1].normal.x = 0.5773502;
      points_2[1].normal.y = -0.5773502;
      points_2[1].normal.z = 0.5773502;
      points_2[2].point.x = 0.9;
      points_2[2].point.y = 0.3;
      points_2[2].point.z = 0.0;
      points_2[2].normal.x = 0.5773502;
      points_2[2].normal.y = -0.5773502;
      points_2[2].normal.z = 0.5773502;
      points_2[3].point.x = 0.7;
      points_2[3].point.y = 0.3;
      points_2[3].point.z = 0.0;
      points_2[3].normal.x = 0.5773502;
      points_2[3].normal.y = -0.5773502;
      points_2[3].normal.z = 0.5773502;

      points_3[0].point.x = 0.7;
      points_3[0].point.y = 0.7;
      points_3[0].point.z = 0.0;
      points_3[0].normal.x = 0.5773502;
      points_3[0].normal.y = 0.5773502;
      points_3[0].normal.z = 0.5773502;
      points_3[1].point.x = 0.9;
      points_3[1].point.y = 0.7;
      points_3[1].point.z = 0.0;
      points_3[1].normal.x = 0.5773502;
      points_3[1].normal.y = 0.5773502;
      points_3[1].normal.z = 0.5773502;
      points_3[2].point.x = 0.9;
      points_3[2].point.y = 0.9;
      points_3[2].point.z = 0.0;
      points_3[2].normal.x = -0.6614378;
      points_3[2].normal.y = -0.6614378;
      points_3[2].normal.z = 0.3535533;
      points_3[3].point.x = 0.7;
      points_3[3].point.y = 0.9;
      points_3[3].point.z = 0.0;
      points_3[3].normal.x = 0.5773502;
      points_3[3].normal.y = 0.5773502;
      points_3[3].normal.z = 0.5773502;

      points_4[0].x = 0.1; points_4[0].y = 0.7; points_4[0].z = 0.0;
      points_4[1].x = 0.3; points_4[1].y = 0.7; points_4[1].z = 0.0;
      points_4[2].x = 0.3; points_4[2].y = 0.9; points_4[2].z = 0.0;
      points_4[3].x = 0.1; points_4[3].y = 0.9; points_4[3].z = 0.0;
      points_4[4].x = 0.1; points_4[4].y = 0.7; points_4[4].z = 0.0;

      PEXSetSurfaceEdgeFlag(dpy, resourceID, req_type, PEXOn);
      PEXSetInteriorStyle(dpy, resourceID, req_type, PEXInteriorStyleSolid);
      PEXSetSurfaceInterpMethod(dpy, resourceID, req_type, PEXSurfaceInterpColor);

      PEXSetReflectionModel( dpy, resourceID, req_type, PEXReflectionDiffuse);
      /* Turn the lights on. */
      lights_on[0] = 1;
      lights_on[1] = 2;
      PEXSetLightSourceState( dpy, resourceID, req_type, 2, lights_on, 0, ((PEXTableIndex*) NULL));

      p_edge_color.rgb.red = 1.0;
      p_edge_color.rgb.green = 0.0;
      p_edge_color.rgb.blue = 0.0;
      PEXSetSurfaceEdgeColor(dpy, resourceID, req_type, PEXColorTypeRGB, &p_edge_color);

      p_surface_color.rgb.red = 0.0;
      p_surface_color.rgb.green = 0.0;
      p_surface_color.rgb.blue = 1.0;
      PEXSetSurfaceColor(dpy, resourceID, req_type, 
			 PEXColorTypeRGB, &p_surface_color);

      PEXFillAreaWithData(dpy, resourceID, req_type, 
			  PEXShapeNonConvex, False, PEXGANone, PEXGANormal,
			  PEXColorTypeRGB, NULL, 5, vertex_1);

      PEXFillAreaWithData(dpy, resourceID, req_type, 
			  PEXShapeNonConvex, False, PEXGANone, PEXGANormal,
			  PEXColorTypeRGB, NULL, 4, vertex_3);

      p_surface_color.rgb.red = 0.0;
      p_surface_color.rgb.green = 1.0;
      p_surface_color.rgb.blue = 0.0;
      PEXSetSurfaceColor(dpy, resourceID, req_type, 
			 PEXColorTypeRGB, &p_surface_color);

      PEXFillAreaWithData(dpy, resourceID, req_type, 
			  PEXShapeNonConvex, False, PEXGANone, PEXGANormal,
			  PEXColorTypeRGB, NULL, 4, vertex_2);

      PEXFillAreaWithData(dpy, resourceID, req_type, 
			  PEXShapeNonConvex, False, PEXGANormal, PEXGANone,
			  PEXColorTypeRGB, &facet_4, 5, vertex_4);
    }


void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "pexocp.91.3");
    *num_images      = 1;
    *supported_modes = IMM_MODE | STR_MODE | WKS_MODE;

    strcpy(win_title, "pexocp.91.3");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    unsigned long         mask = PEXRAHLHSRMode;
    PEXRendererAttributes attrs;
    PEXLightEntry	  lights[2];

    attrs.hlhsr_mode = PEXHLHSRZBuffer;
    PEXChangeRenderer(dpy, renderer, mask, &attrs);

    /* Define the ambient light. */
    lights[0].type = PEXLightAmbient;
    lights[0].color.type = PEXColorTypeRGB;
    lights[0].color.value.rgb.red = .3;
    lights[0].color.value.rgb.green = .3;
    lights[0].color.value.rgb.blue = .3;

    /* Define the point light. */
    lights[1].type = PEXLightWCPoint;
    lights[1].point.x = 0.2;
    lights[1].point.y = 0.2;
    lights[1].point.z = 0.5;
    lights[1].attenuation1 = 1.0;
    lights[1].attenuation2 = 1.0;
    lights[1].color.type = PEXColorTypeRGB;
    lights[1].color.value.rgb.red = 1.0;
    lights[1].color.value.rgb.green = 1.0;
    lights[1].color.value.rgb.blue = 1.0;

    PEXSetTableEntries(dpy, lightLUT, 1, 2, PEXLUTLight, lights);
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    describe_test(" \nPEXFillAreaWithData (Normals - Vertex and Facet)\n");
    describe_test(" The image you should see consists of 4 squares in\n");
    describe_test(" each corner of the window as follows:\n");
    describe_test(" Upper left  -- solid green with red edge\n");
    describe_test(" Upper right -- soid interp    blue       light blue\n");
    describe_test("                               blue       blue\n");
    describe_test("                with red edge\n\n");
    describe_test(" Lower left  -- solid  interp  blue       light blue\n");
    describe_test("                               blue       blue \n");
    describe_test("                with red edge\n");
    describe_test(" Lower right -- solid dark green with red edge\n\n");

    draw_image(dpy, resourceID, req_type);
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
     image_testproc("pexocp.91.3", "pexocp.91.3", img_desc,
		      dpy,window, 0, 0, 
		      glob_window_width, glob_window_height);
    }
