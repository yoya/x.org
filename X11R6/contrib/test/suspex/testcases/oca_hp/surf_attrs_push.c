/* $XConsortium: surf_attrs_push.c,v 1.0 93/11/22 12:32:10 rws Exp $ */

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

#define X_START       0.35
#define Y_START       0.95
#define X_SIZE        0.1
#define Y_SIZE        0.1 
#define X_INCREMENT   0.2
#define Y_INCREMENT   0.15

#define AMBIENT_LIGHT 1
#define WIDTH_1       2.0
#define WIDTH_2       4.0

#define OFFSET_VERTICAL                                        \
    {                                                          \
    int i;                                                     \
    for (i=0; i<5; i++)                                        \
	{                                                      \
        front_face[i].y -= Y_INCREMENT;                        \
        back_face[i].y  -= Y_INCREMENT;                        \
	}                                                      \
    }


PEXCoord2D front_face[5] = {X_START        , Y_START, 
                            X_START        ,(Y_START-Y_SIZE), 
			   (X_START+X_SIZE),(Y_START-Y_SIZE), 
			   (X_START+X_SIZE), Y_START,
                            X_START        , Y_START};
PEXCoord2D back_face[5]  = {(X_START+X_INCREMENT)      , Y_START, 
			   (X_START+X_SIZE+X_INCREMENT), Y_START,
			   (X_START+X_SIZE+X_INCREMENT),(Y_START-Y_SIZE), 
                           (X_START+X_INCREMENT)       ,(Y_START-Y_SIZE), 
                           (X_START+X_INCREMENT)       , Y_START};

void setup_attrs1(Display *dpy, XID resource_id, PEXOCRequestType request_type)
    {
    PEXTableIndex enable_lights[1] = {AMBIENT_LIGHT};

    PEXSetBFSurfaceColorIndex  (dpy, resource_id, request_type, BLUE);
    PEXSetSurfaceColorIndex    (dpy, resource_id, request_type, RED);
    PEXSetSurfaceEdgeColorIndex(dpy, resource_id, request_type, YELLOW);
    PEXSetSurfaceEdgeFlag      (dpy, resource_id, request_type, PEXOn);
    PEXSetSurfaceEdgeType      (dpy, resource_id, request_type, 
				PEXSurfaceEdgeDashed);
    PEXSetBFInteriorStyle      (dpy, resource_id, request_type, 
				PEXInteriorStyleHatch);
    PEXSetBFInteriorStyleIndex (dpy, resource_id, request_type, 
			        HPPEXHatchStyle135Degrees);
    PEXSetInteriorStyle        (dpy, resource_id, request_type, 
				PEXInteriorStyleHatch);
    PEXSetInteriorStyleIndex   (dpy, resource_id, request_type, 
			        HPPEXHatchStyle45Degrees);
    PEXSetFacetCullingMode     (dpy, resource_id, request_type, PEXNone);
    PEXSetFacetDistinguishFlag (dpy, resource_id, request_type, True);
    PEXSetSurfaceEdgeWidth     (dpy, resource_id, request_type, WIDTH_1);
    PEXSetInteriorBundleIndex  (dpy, resource_id, request_type, 1);
    PEXSetEdgeBundleIndex      (dpy, resource_id, request_type, 1);
    PEXSetBFReflectionModel    (dpy, resource_id, request_type, 
				PEXReflectionNone);
    PEXSetReflectionModel      (dpy, resource_id, request_type, 
				PEXReflectionNone);
    PEXSetLightSourceState     (dpy, resource_id, request_type,
				    1, enable_lights, 0, NULL);
    }

void setup_attrs2(Display *dpy, XID resource_id, PEXOCRequestType request_type)
    {
    PEXSetBFSurfaceColorIndex  (dpy, resource_id, request_type, CYAN);
    PEXSetSurfaceColorIndex    (dpy, resource_id, request_type, GREEN);
    PEXSetSurfaceEdgeColorIndex(dpy, resource_id, request_type, WHITE);
    PEXSetSurfaceEdgeFlag      (dpy, resource_id, request_type, PEXOff);
    PEXSetSurfaceEdgeType      (dpy, resource_id, request_type, 
				PEXSurfaceEdgeSolid);
    PEXSetBFInteriorStyle      (dpy, resource_id, request_type, 
				PEXInteriorStyleSolid);
    PEXSetBFInteriorStyleIndex (dpy, resource_id, request_type, 
			        HPPEXHatchStyle45Degrees);
    PEXSetInteriorStyle        (dpy, resource_id, request_type, 
				PEXInteriorStyleSolid);
    PEXSetInteriorStyleIndex   (dpy, resource_id, request_type, 
			        HPPEXHatchStyle135Degrees);
    PEXSetFacetCullingMode     (dpy, resource_id, request_type, PEXBackFaces);
    PEXSetFacetDistinguishFlag (dpy, resource_id, request_type, False);
    PEXSetSurfaceEdgeWidth     (dpy, resource_id, request_type, WIDTH_2);
    PEXSetInteriorBundleIndex  (dpy, resource_id, request_type, 2);
    PEXSetEdgeBundleIndex      (dpy, resource_id, request_type, 2);
    PEXSetBFReflectionModel    (dpy, resource_id, request_type, 
				PEXReflectionAmbient);
    PEXSetReflectionModel      (dpy, resource_id, request_type, 
				PEXReflectionAmbient);
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

void draw_areas(Display *dpy, XID resource_id, PEXOCRequestType request_type)
    {
    set_individual_attrs(dpy, resource_id, request_type);
    PEXFillArea2D(dpy, resource_id, request_type, PEXShapeUnknown,
		False, 5, front_face);
    PEXFillArea2D(dpy, resource_id, request_type, PEXShapeUnknown,
		False, 5, back_face);
    OFFSET_VERTICAL

    set_bundled_attrs(dpy, resource_id, request_type);
    PEXFillArea2D(dpy, resource_id, request_type, PEXShapeUnknown,
		False, 5, front_face);
    PEXFillArea2D(dpy, resource_id, request_type, PEXShapeUnknown,
		False, 5, back_face);
    OFFSET_VERTICAL
    set_individual_attrs(dpy, resource_id, request_type);
    }

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "surf_attrs_push");
    *num_images      = 1;
    *supported_modes = IMM_MODE | STR_MODE | WKS_MODE;

    strcpy(win_title, "Surf Attrs Push/Pop");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    PEXStructure sid;

    sid = PEXCreateStructure(dpy);
    if (sid == None)
        {
        _hppex_stderr_print("PROCESS ERROR:  create_structure failed\n");
	exit (1);
        }
    
    describe_test("\nImage 1\n");
    describe_test("    Row 1 - Front, red  hatch  45, yellow edge\n");
    describe_test("            Back,  blue hatch 135, yellow edge\n");
    describe_test("    Row 2 - Default interior/edge bundle entries\n");
    describe_test("  Execute Structure\n");
    describe_test("    Row 3 - Front, green, solid, no edges\n");
    describe_test("            Back,  CULLED\n");
    describe_test("    Row 4 - Interior/edge bundle entry  ambient grey, cyan edge\n");
    describe_test("            Back,  CULLED\n");
    describe_test("  After pop_state\n");
    describe_test("    Row 5 - Front, red  hatch  45, yellow edge\n");
    describe_test("            Back,  blue hatch 135, yellow edge\n");
    describe_test("    Row 6 - Default interior/edge bundle entries\n");

    setup_attrs1(dpy, resourceID, req_type);
    draw_areas  (dpy, resourceID, req_type);
    
    setup_attrs2(dpy, sid, PEXOCStore);
    draw_areas(dpy, sid, PEXOCStore);
    PEXExecuteStructure (dpy, resourceID, req_type, sid);
    draw_areas(dpy, resourceID, req_type);
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
     image_testproc("surf_attrs_push", "Surf Attrs Push/Pop", img_desc,
		      dpy,window, 0, 0, 
		      glob_window_width, glob_window_height);
    }
