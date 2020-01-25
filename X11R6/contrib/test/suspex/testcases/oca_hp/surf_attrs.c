/* $XConsortium: surf_attrs.c,v 1.0 93/11/22 12:32:04 rws Exp $ */

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


#include <stdio.h>
#include <string.h>

#include <X11/X.h>
#include <X11/Xlibint.h>
#include <X11/Xutil.h>
#include <X11/PEX5/PEXlib.h>

#define HPPEXHatchStyle45Degrees        0x80070001
#define HPPEXHatchStyle135Degrees       0x80070002

#include <misc.h>

#define X_START       0.06
#define Y_START       0.98
#define X_SIZE        0.1
#define Y_SIZE        0.05
#define X_INCREMENT   0.13
#define Y_INCREMENT   0.06

#define AMBIENT_LIGHT 1

#define OFFSET_HORIZONTAL                                      \
    {                                                          \
    int i;                                                     \
    for (i=0; i<5; i++)                                        \
	{                                                      \
        front_face[i].x += X_INCREMENT;                        \
        back_face[i].x  += X_INCREMENT;                        \
	}                                                      \
    }

#define OFFSET_VERTICAL                                        \
    {                                                          \
    int i;                                                     \
    for (i=0; i<5; i++)                                        \
	{                                                      \
        front_face[i].x -= X_INCREMENT*6;                      \
        back_face[i].x  -= X_INCREMENT*6;                      \
        front_face[i].y -= Y_INCREMENT*2;                      \
        back_face[i].y  -= Y_INCREMENT*2;                      \
	}                                                      \
    }

void set_bundled_attrs(Display         *dpy, 
		       XID              resource_id, 
		       PEXOCRequestType request_type)
    {
    /*
     *  Set all InteriorBundle attributes to bundled
     */
    PEXSetIndividualASF(dpy, resource_id, request_type, 
			PEXASFInteriorStyle, PEXBundled);
    PEXSetIndividualASF(dpy, resource_id, request_type, 
			PEXASFInteriorStyleIndex, PEXBundled);
    PEXSetIndividualASF(dpy, resource_id, request_type, 
			PEXASFSurfaceColor, PEXBundled);
    PEXSetIndividualASF(dpy, resource_id, request_type, 
			PEXASFReflectionAttr, PEXBundled);
    PEXSetIndividualASF(dpy, resource_id, request_type, 
			PEXASFReflectionModel, PEXBundled);
    PEXSetIndividualASF(dpy, resource_id, request_type, 
			PEXASFSurfaceInterp, PEXBundled);
    PEXSetIndividualASF(dpy, resource_id, request_type, 
			PEXASFBFInteriorStyle, PEXBundled);
    PEXSetIndividualASF(dpy, resource_id, request_type, 
			PEXASFBFInteriorStyleIndex, PEXBundled);
    PEXSetIndividualASF(dpy, resource_id, request_type, 
			PEXASFBFSurfaceColor, PEXBundled);
    PEXSetIndividualASF(dpy, resource_id, request_type, 
			PEXASFBFReflectionAttr, PEXBundled);
    PEXSetIndividualASF(dpy, resource_id, request_type, 
			PEXASFBFReflectionModel, PEXBundled);
    PEXSetIndividualASF(dpy, resource_id, request_type, 
			PEXASFBFSurfaceInterp, PEXBundled);
    PEXSetIndividualASF(dpy, resource_id, request_type, 
			PEXASFSurfaceApprox, PEXBundled);
    /*
     *  Set all EdgeBundle attributes to bundled
     */
    PEXSetIndividualASF(dpy, resource_id, request_type, 
			PEXASFSurfaceEdges, PEXBundled);
    PEXSetIndividualASF(dpy, resource_id, request_type, 
			PEXASFSurfaceEdgeType, PEXBundled);
    PEXSetIndividualASF(dpy, resource_id, request_type, 
			PEXASFSurfaceEdgeWidth, PEXBundled);
    PEXSetIndividualASF(dpy, resource_id, request_type, 
			PEXASFSurfaceEdgeColor, PEXBundled);

    }


void set_individual_attrs(Display         *dpy, 
		          XID              resource_id, 
		          PEXOCRequestType request_type)
    {
    /*
     *  Set all InteriorBundle attributes back to individual
     */
    PEXSetIndividualASF(dpy, resource_id, request_type, 
			PEXASFInteriorStyle, PEXIndividual);
    PEXSetIndividualASF(dpy, resource_id, request_type, 
			PEXASFInteriorStyleIndex, PEXIndividual);
    PEXSetIndividualASF(dpy, resource_id, request_type, 
			PEXASFSurfaceColor, PEXIndividual);
    PEXSetIndividualASF(dpy, resource_id, request_type, 
			PEXASFReflectionAttr, PEXIndividual);
    PEXSetIndividualASF(dpy, resource_id, request_type, 
			PEXASFReflectionModel, PEXIndividual);
    PEXSetIndividualASF(dpy, resource_id, request_type, 
			PEXASFSurfaceInterp, PEXIndividual);
    PEXSetIndividualASF(dpy, resource_id, request_type, 
			PEXASFBFInteriorStyle, PEXIndividual);
    PEXSetIndividualASF(dpy, resource_id, request_type, 
			PEXASFBFInteriorStyleIndex, PEXIndividual);
    PEXSetIndividualASF(dpy, resource_id, request_type, 
			PEXASFBFSurfaceColor, PEXIndividual);
    PEXSetIndividualASF(dpy, resource_id, request_type, 
			PEXASFBFReflectionAttr, PEXIndividual);
    PEXSetIndividualASF(dpy, resource_id, request_type, 
			PEXASFBFReflectionModel, PEXIndividual);
    PEXSetIndividualASF(dpy, resource_id, request_type, 
			PEXASFBFSurfaceInterp, PEXIndividual);
    PEXSetIndividualASF(dpy, resource_id, request_type, 
			PEXASFSurfaceApprox, PEXIndividual);
    /*
     *  Set all EdgeBundle attributes back to Individual
     */
    PEXSetIndividualASF(dpy, resource_id, request_type, 
			PEXASFSurfaceEdges, PEXIndividual);
    PEXSetIndividualASF(dpy, resource_id, request_type, 
			PEXASFSurfaceEdgeType, PEXIndividual);
    PEXSetIndividualASF(dpy, resource_id, request_type, 
			PEXASFSurfaceEdgeWidth, PEXIndividual);
    PEXSetIndividualASF(dpy, resource_id, request_type, 
			PEXASFSurfaceEdgeColor, PEXIndividual);

    }

void display_interior_styles(Display         *dpy, 
			     XID              resource_id, 
			     PEXOCRequestType request_type,
			     PEXCoord2D       front_face[5],
			     PEXCoord2D       back_face[5])
    {
    /* Interior Style Solid */
    PEXSetInteriorStyle(dpy, resource_id, request_type, PEXInteriorStyleSolid);
    PEXSetBFInteriorStyle(dpy, resource_id, request_type, PEXInteriorStyleSolid);
    PEXFillArea2D(dpy, resource_id, request_type, PEXShapeUnknown,
		False, 5, front_face);
    PEXFillArea2D(dpy, resource_id, request_type, PEXShapeUnknown,
		False, 5, back_face);
    OFFSET_HORIZONTAL

    /* Interior Style Hollow */
    PEXSetInteriorStyle(dpy, resource_id, request_type, PEXInteriorStyleHollow);
    PEXSetBFInteriorStyle(dpy, resource_id, request_type, PEXInteriorStyleHollow);
    PEXFillArea2D(dpy, resource_id, request_type, PEXShapeUnknown,
		False, 5, front_face);
    PEXFillArea2D(dpy, resource_id, request_type, PEXShapeUnknown,
		False, 5, back_face);
    OFFSET_HORIZONTAL

    /* Interior Style Pattern */
    PEXSetInteriorStyle(dpy, resource_id, request_type, PEXInteriorStylePattern);
    PEXSetBFInteriorStyle(dpy, resource_id, request_type, PEXInteriorStylePattern);
    PEXFillArea2D(dpy, resource_id, request_type, PEXShapeUnknown,
		False, 5, front_face);
    PEXFillArea2D(dpy, resource_id, request_type, PEXShapeUnknown,
		False, 5, back_face);
    OFFSET_HORIZONTAL

    /* Interior Style Hatch45 */
    PEXSetInteriorStyle(dpy, resource_id, request_type, PEXInteriorStyleHatch);
    PEXSetBFInteriorStyle(dpy, resource_id, request_type, PEXInteriorStyleHatch);
    PEXSetInteriorStyleIndex(dpy, resource_id, request_type, 
			     HPPEXHatchStyle45Degrees);
    PEXSetBFInteriorStyleIndex(dpy, resource_id, request_type, 
			       HPPEXHatchStyle45Degrees);
    PEXFillArea2D(dpy, resource_id, request_type, PEXShapeUnknown,
		False, 5, front_face);
    PEXFillArea2D(dpy, resource_id, request_type, PEXShapeUnknown,
		False, 5, back_face);
    OFFSET_HORIZONTAL

    /* Interior Style Hatch135 */
    PEXSetInteriorStyle(dpy, resource_id, request_type, PEXInteriorStyleHatch);
    PEXSetBFInteriorStyle(dpy, resource_id, request_type, PEXInteriorStyleHatch);
    PEXSetInteriorStyleIndex(dpy, resource_id, request_type, 
			     HPPEXHatchStyle135Degrees);
    PEXSetBFInteriorStyleIndex(dpy, resource_id, request_type, 
			       HPPEXHatchStyle135Degrees);
    PEXFillArea2D(dpy, resource_id, request_type, PEXShapeUnknown,
		False, 5, front_face);
    PEXFillArea2D(dpy, resource_id, request_type, PEXShapeUnknown,
		False, 5, back_face);
    OFFSET_HORIZONTAL

    /* Interior Style Empty */
    PEXSetInteriorStyle(dpy, resource_id, request_type, PEXInteriorStyleEmpty);
    PEXSetBFInteriorStyle(dpy, resource_id, request_type, PEXInteriorStyleEmpty);
    PEXFillArea2D(dpy, resource_id, request_type, PEXShapeUnknown,
		False, 5, front_face);
    PEXFillArea2D(dpy, resource_id, request_type, PEXShapeUnknown,
		False, 5, back_face);

    
    }

void display_edge_types(Display         *dpy, 
			     XID              resource_id, 
			     PEXOCRequestType request_type, 
			     PEXCoord2D       front_face[5],
			     PEXCoord2D       back_face[5])
    {
    /* SurfaceEdgeType Dashed */
    PEXSetSurfaceEdgeType(dpy, resource_id, request_type, PEXSurfaceEdgeDashed);
    PEXFillArea2D(dpy, resource_id, request_type, PEXShapeUnknown,
		False, 5, front_face);
    PEXFillArea2D(dpy, resource_id, request_type, PEXShapeUnknown,
		False, 5, back_face);
    OFFSET_HORIZONTAL

    /* SurfaceEdgeType Solid */
    PEXSetSurfaceEdgeType(dpy, resource_id, request_type, PEXSurfaceEdgeSolid);
    PEXFillArea2D(dpy, resource_id, request_type, PEXShapeUnknown,
		False, 5, front_face);
    PEXFillArea2D(dpy, resource_id, request_type, PEXShapeUnknown,
		False, 5, back_face);
    OFFSET_HORIZONTAL

    /* SurfaceEdgeType Dotted */
    PEXSetSurfaceEdgeType(dpy, resource_id, request_type, PEXSurfaceEdgeDotted);
    PEXFillArea2D(dpy, resource_id, request_type, PEXShapeUnknown,
		False, 5, front_face);
    PEXFillArea2D(dpy, resource_id, request_type, PEXShapeUnknown,
		False, 5, back_face);
    OFFSET_HORIZONTAL

    /* SurfaceEdgeType DashDot */
    PEXSetSurfaceEdgeType(dpy, resource_id, request_type, PEXSurfaceEdgeDashDot);
    PEXFillArea2D(dpy, resource_id, request_type, PEXShapeUnknown,
		False, 5, front_face);
    PEXFillArea2D(dpy, resource_id, request_type, PEXShapeUnknown,
		False, 5, back_face);
    OFFSET_HORIZONTAL

    /* SurfaceEdgeWidth 2.0 */
    PEXSetSurfaceEdgeWidth(dpy, resource_id, request_type, 2.0);
    PEXFillArea2D(dpy, resource_id, request_type, PEXShapeUnknown,
		False, 5, front_face);
    PEXFillArea2D(dpy, resource_id, request_type, PEXShapeUnknown,
		False, 5, back_face);
    OFFSET_HORIZONTAL

    }

void draw_surfaces(Display *dpy, XID resource_id, PEXOCRequestType request_type)
    {
    PEXColorRGB color_red    = {1.0, 0.0, 0.0};
    PEXColorRGB color_blue   = {0.0, 0.0, 1.0};
    PEXColorRGB color_cyan   = {0.0, 1.0, 1.0};
    PEXColorRGB color_yellow = {1.0, 1.0, 0.0};
    PEXCoord2D front_face[5] = {X_START,         Y_START, 
                                X_START,        (Y_START-Y_SIZE), 
			       (X_START+X_SIZE),(Y_START-Y_SIZE), 
			       (X_START+X_SIZE), Y_START,
                                X_START,         Y_START};
    PEXCoord2D back_face[5]  = {X_START,        (Y_START-Y_INCREMENT), 
			       (X_START+X_SIZE),(Y_START-Y_INCREMENT),
			       (X_START+X_SIZE),(Y_START-Y_SIZE-Y_INCREMENT), 
                                X_START,        (Y_START-Y_SIZE-Y_INCREMENT), 
                                X_START,        (Y_START-Y_INCREMENT)};

    PEXTableIndex enable_lights[1];

    describe_test("    Rows 1-12 (default, solid, hollow, pattern, hatch45, hatch135, empty\n");
    describe_test("      Attrs: Surface Color = red, Back Face color = blue, Edge Color = yellow\n");
    describe_test("      Rows 1-2 (Front/Back) Defaults (Edges off, BF Distinguish off, Cull none)\n");
				     
    /*  Default Attributes (white, hollow) */
    PEXFillArea2D(dpy, resource_id, request_type, PEXShapeUnknown,
		False, 5, front_face);
    PEXFillArea2D(dpy, resource_id, request_type, PEXShapeUnknown,
		False, 5, back_face);
    OFFSET_HORIZONTAL

    /*  Set Colors (front face -- red, back face -- blue, edges -- yellow)  */
    PEXSetSurfaceColor(dpy, resource_id, request_type, PEXColorTypeRGB, 
		       (PEXColor *) &color_red);
    PEXSetBFSurfaceColor(dpy, resource_id, request_type, PEXColorTypeRGB, 
		         (PEXColor *) &color_blue);
    PEXSetSurfaceEdgeColor(dpy, resource_id, request_type, PEXColorTypeRGB, 
		           (PEXColor *) &color_yellow);

    /*  Defaults (Edges off, BF Distinguish off, Cull none  */
    display_interior_styles(dpy, resource_id, request_type, front_face, back_face);

    /*  Set colors again using color index entry points                     */
    /*  (front face -- green, back face -- magenta, edges -- white          */

    PEXSetSurfaceColorIndex(dpy, resource_id, request_type, GREEN);
    PEXSetBFSurfaceColorIndex(dpy, resource_id, request_type, MAGENTA);
    PEXSetSurfaceEdgeColorIndex(dpy, resource_id, request_type, WHITE);

    /*  Turn on edging */
    describe_test("      Attrs: Surface Color = grn, Back Face color = magenta, Edge Color = white\n");
    describe_test("      Rows 3-4  (Front/Back) (Edges ON, BF Distinguish off, Cull none)\n");
    PEXSetSurfaceEdgeFlag(dpy, resource_id, request_type, PEXOn);
    OFFSET_VERTICAL
    OFFSET_HORIZONTAL
    display_interior_styles(dpy, resource_id, request_type, front_face, back_face);

    /*  Turn on back face distinguish */
    describe_test("      Rows 5-6  (Front/Back) (Edges ON, BF Distinguish ON, Cull none)\n");
    PEXSetFacetDistinguishFlag(dpy, resource_id, request_type, True);
    OFFSET_VERTICAL
    OFFSET_HORIZONTAL
    display_interior_styles(dpy, resource_id, request_type, front_face, back_face);

    /*  Cull Front Faces */
    describe_test("      Rows 7-8  (Front/Back) (Edges ON, BF Distinguish ON, Cull front faces)\n");
    PEXSetFacetCullingMode(dpy, resource_id, request_type, PEXFrontFaces);
    OFFSET_VERTICAL
    OFFSET_HORIZONTAL
    display_interior_styles(dpy, resource_id, request_type, front_face, back_face);

    /*  Cull Back Faces */
    describe_test("      Rows 9-10 (Front/Back) (Edges ON, BF Distinguish ON, Cull back faces)\n");
    PEXSetFacetCullingMode(dpy, resource_id, request_type, PEXBackFaces);
    OFFSET_VERTICAL
    OFFSET_HORIZONTAL
    display_interior_styles(dpy, resource_id, request_type, front_face, back_face);

    /*  Turn Culling Back Off */
    describe_test("      Rows 11-12 (Front/Back) (Edges ON, BF Distinguish ON, Cull none)\n");
    PEXSetFacetCullingMode(dpy, resource_id, request_type, PEXNone);
    OFFSET_VERTICAL
    OFFSET_HORIZONTAL
    display_interior_styles(dpy, resource_id, request_type, front_face, back_face);

    describe_test("      Rows 13-14 (Front/Back) (Edges Dashed, Solid, Dotted, DashDot, width 2.0)\n");
    OFFSET_VERTICAL
    OFFSET_HORIZONTAL
    OFFSET_HORIZONTAL
    display_edge_types(dpy, resource_id, request_type, front_face, back_face);


    OFFSET_VERTICAL
    describe_test("      Rows 15-16 (Front/Back) ASF Changes\n");
    describe_test("        Column 1 -- Blank\n");
    describe_test("        Column 2 -- Default Bundle 1 (white/hollow)\n");
    describe_test("        Column 3 -- Indiv attrs (hatch green/magenta, red edge)\n");
    describe_test("        Column 4 -- Invalid bundle (should default)\n");
    describe_test("        Column 5 -- Bundle 2 (Solid, ambient (grey, dk grey), cyan edge)\n");
    describe_test("        Column 6 -- Back to indiv attrs (column 3)\n");

    /*
     *  Set individual attrs to hatch, edge color red.
     */
    PEXSetInteriorStyle(dpy, resource_id, request_type, PEXInteriorStyleHatch);
    PEXSetBFInteriorStyle(dpy, resource_id, request_type, PEXInteriorStyleHatch);
    PEXSetInteriorStyleIndex(dpy, resource_id, request_type, 
			     HPPEXHatchStyle45Degrees);
    PEXSetBFInteriorStyleIndex(dpy, resource_id, request_type, 
			       HPPEXHatchStyle45Degrees);
    PEXSetSurfaceEdgeColorIndex(dpy, resource_id, request_type, RED);

    /*
     *  Set the light source state to a single white ambient light.
     */
    enable_lights[0] = AMBIENT_LIGHT;
    PEXSetLightSourceState (dpy, resource_id, request_type,
			    1, enable_lights, 0, NULL);


    /*
     *  Set bundled attributes and Interior and Edge bundle indices to 
     *  default (1).
     */
    set_bundled_attrs(dpy, resource_id, request_type);
    PEXSetInteriorBundleIndex(dpy, resource_id, request_type, 1);
    PEXSetEdgeBundleIndex    (dpy, resource_id, request_type, 1);
    PEXFillArea2D(dpy, resource_id, request_type, PEXShapeUnknown,
		False, 5, front_face);
    PEXFillArea2D(dpy, resource_id, request_type, PEXShapeUnknown,
		False, 5, back_face);
    OFFSET_HORIZONTAL

    /*
     *  Set back to individual attrs
     */
    set_individual_attrs(dpy, resource_id, request_type);
    PEXFillArea2D(dpy, resource_id, request_type, PEXShapeUnknown,
		False, 5, front_face);
    PEXFillArea2D(dpy, resource_id, request_type, PEXShapeUnknown,
		False, 5, back_face);
    OFFSET_HORIZONTAL

    /*
     *  Set bundled attributes and Interior and Edge bundle indices to 
     *  an undefined index (9999). Should default to 1.
     */
    set_bundled_attrs(dpy, resource_id, request_type);
    PEXSetInteriorBundleIndex(dpy, resource_id, request_type, 9999);
    PEXSetEdgeBundleIndex    (dpy, resource_id, request_type, 9999);
    PEXFillArea2D(dpy, resource_id, request_type, PEXShapeUnknown,
		False, 5, front_face);
    PEXFillArea2D(dpy, resource_id, request_type, PEXShapeUnknown,
		False, 5, back_face);
    OFFSET_HORIZONTAL

    /*
     *  Set bundled attributes and Interior and Edge bundle indices to 
     *  an undefined index 2 (our defined entries)
     */
    set_bundled_attrs(dpy, resource_id, request_type);
    PEXSetInteriorBundleIndex(dpy, resource_id, request_type, 2);
    PEXSetEdgeBundleIndex    (dpy, resource_id, request_type, 2);
    PEXFillArea2D(dpy, resource_id, request_type, PEXShapeUnknown,
		False, 5, front_face);
    PEXFillArea2D(dpy, resource_id, request_type, PEXShapeUnknown,
		False, 5, back_face);
    OFFSET_HORIZONTAL

    /*
     *  Set back to individual attrs
     */
    set_individual_attrs(dpy, resource_id, request_type);
    PEXFillArea2D(dpy, resource_id, request_type, PEXShapeUnknown,
		False, 5, front_face);
    PEXFillArea2D(dpy, resource_id, request_type, PEXShapeUnknown,
		False, 5, back_face);
    OFFSET_HORIZONTAL

    }

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "surf_attrs");
    *num_images      = 1;
    *supported_modes = IMM_MODE | STR_MODE | WKS_MODE;

    strcpy(win_title, "Surface Attrs");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    draw_surfaces(dpy, resourceID, req_type);
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
     image_testproc("surf_attrs", "Surface Attributes", img_desc,
		      dpy,window, 0, 0, 
		      glob_window_width, glob_window_height);
    }
