/* $XConsortium: idle_renderer.c,v 1.0 93/11/22 12:31:16 rws Exp $ */

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
#include <unistd.h>
#include <string.h>

#include <X11/X.h>
#include <X11/Xlibint.h>
#include <X11/Xutil.h>
#include <X11/PEX5/PEXlib.h>

#include <misc.h>
#include <err_handler.h>

void issue_attrs(Display *dpy, XID resource_id, PEXOCRequestType request_type)
    {
    PEXColor bad_rgb_color1, bad_rgb_color2;
    PEXColor bad_indexed_color;
    PEXMatrix    ident_mat;
    PEXMatrix3x3 ident_mat2D;
    PEXVector2D valid_up_vector = {0.0, 1.0};
    PEXReflectionAttributes bad_refl_attr;
    PEXHalfSpace   half_space;
    PEXHalfSpace2D half_space2D;
    PEXTableIndex  bad_enable_list1  = 0;
    PEXTableIndex  bad_disable_list1 = 1;
    PEXTableIndex  bad_enable_list2  = 2;
    PEXTableIndex  bad_disable_list2 = 2;

    int      bad_color_type  = 9999;
    int      bad_color_index = 65535;

    PEXPSCData psc_data;

    PEXCoord ref_point;
    PEXVector vector1, vector2;
    PEXCoord2D ref_point2D;

    PEXName names;

    bad_rgb_color1.rgb.red   = -0.1;
    bad_rgb_color1.rgb.green =  0.0;
    bad_rgb_color1.rgb.blue  =  0.0;

    bad_rgb_color2.rgb.red   =  0.0;
    bad_rgb_color2.rgb.green =  1.1;
    bad_rgb_color2.rgb.blue  =  0.0;

    bad_indexed_color.indexed.index = 65535;

    PEXIdentityMatrix(ident_mat);

    bad_refl_attr.specular_color.type = bad_color_type;


    /*
    **************************************************************************
    **
    **    PEXSet<blah>Color functions
    **
    **************************************************************************
    */
    PEXSetBFSurfaceColor(dpy, resource_id, request_type, 
			 PEXColorTypeRGB, &bad_rgb_color1);
    PEXSetBFSurfaceColor(dpy, resource_id, request_type, 
			 PEXColorTypeRGB, &bad_rgb_color2);
    PEXSetBFSurfaceColor(dpy, resource_id, request_type, 
			 bad_color_type, &bad_rgb_color1);
    PEXSetBFSurfaceColor(dpy, resource_id, request_type, 
			 PEXColorTypeIndexed, &bad_indexed_color);

    PEXSetLineColor(dpy, resource_id, request_type, 
		    PEXColorTypeRGB, &bad_rgb_color1);
    PEXSetLineColor(dpy, resource_id, request_type, 
	            PEXColorTypeRGB, &bad_rgb_color2);
    PEXSetLineColor(dpy, resource_id, request_type, 
	            bad_color_type, &bad_rgb_color1);
    PEXSetLineColor(dpy, resource_id, request_type, 
		    PEXColorTypeIndexed, &bad_indexed_color);

    PEXSetMarkerColor(dpy, resource_id, request_type, 
		      PEXColorTypeRGB, &bad_rgb_color1);
    PEXSetMarkerColor(dpy, resource_id, request_type, 
		      PEXColorTypeRGB, &bad_rgb_color2);
    PEXSetMarkerColor(dpy, resource_id, request_type, 
		      bad_color_type, &bad_rgb_color1);
    PEXSetMarkerColor(dpy, resource_id, request_type, 
		      PEXColorTypeIndexed, &bad_indexed_color);

    PEXSetSurfaceColor(dpy, resource_id, request_type, 
		       PEXColorTypeRGB, &bad_rgb_color1);
    PEXSetSurfaceColor(dpy, resource_id, request_type, 
		       PEXColorTypeRGB, &bad_rgb_color2);
    PEXSetSurfaceColor(dpy, resource_id, request_type, 
		       bad_color_type, &bad_rgb_color1);
    PEXSetSurfaceColor(dpy, resource_id, request_type, 
		       PEXColorTypeIndexed, &bad_indexed_color);

    PEXSetSurfaceEdgeColor(dpy, resource_id, request_type, 
			   PEXColorTypeRGB, &bad_rgb_color1);
    PEXSetSurfaceEdgeColor(dpy, resource_id, request_type, 
			   PEXColorTypeRGB, &bad_rgb_color2);
    PEXSetSurfaceEdgeColor(dpy, resource_id, request_type, 
			   bad_color_type, &bad_rgb_color1);
    PEXSetSurfaceEdgeColor(dpy, resource_id, request_type, 
			   PEXColorTypeIndexed, &bad_indexed_color);

    PEXSetTextColor(dpy, resource_id, request_type, 
		    PEXColorTypeRGB, &bad_rgb_color1);
    PEXSetTextColor(dpy, resource_id, request_type, 
		    PEXColorTypeRGB, &bad_rgb_color2);
    PEXSetTextColor(dpy, resource_id, request_type, 
		    bad_color_type, &bad_rgb_color1);
    PEXSetTextColor(dpy, resource_id, request_type, 
		    PEXColorTypeIndexed, &bad_indexed_color);
    /*
    **************************************************************************
    **
    **    PEXSet<blah>ColorIndex functions
    **
    **************************************************************************
    */
    PEXSetBFSurfaceColorIndex  (dpy, resource_id, request_type, 
			        bad_color_index);

    PEXSetLineColorIndex       (dpy, resource_id, request_type, 
			        bad_color_index);

    PEXSetMarkerColorIndex     (dpy, resource_id, request_type, 
			        bad_color_index);

    PEXSetSurfaceColorIndex    (dpy, resource_id, request_type, 
			        bad_color_index);

    PEXSetSurfaceEdgeColorIndex(dpy, resource_id, request_type, 
				bad_color_index);

    PEXSetTextColorIndex       (dpy, resource_id, request_type, 
				bad_color_index);
    /*
    **************************************************************************
    **
    **    PEXSet<blah>Index functions
    **
    **************************************************************************
    */
    PEXSetEdgeBundleIndex(dpy, resource_id, request_type, 0);

    PEXSetInteriorBundleIndex(dpy, resource_id, request_type, 0);

    PEXSetLineBundleIndex(dpy, resource_id, request_type, 0);

    PEXSetMarkerBundleIndex(dpy, resource_id, request_type, 0);

    PEXSetTextBundleIndex(dpy, resource_id, request_type, 0);

    PEXSetColorApproxIndex(dpy, resource_id, request_type, 65535);

    PEXSetDepthCueIndex(dpy, resource_id, request_type, 65535);

    PEXSetTextFontIndex(dpy, resource_id, request_type, 0);

    PEXSetViewIndex(dpy, resource_id, request_type, 65535);

    /*
    **************************************************************************
    **
    **    PEXSet <Interior Style Index> functions
    **
    **************************************************************************
    */
    PEXSetInteriorStyleIndex(dpy, resource_id, request_type, 0);
    PEXSetBFInteriorStyleIndex(dpy, resource_id, request_type, 0);

    /*
    **************************************************************************
    **
    **    PEXSetGlobal/LocalTransform functions
    **    NOTE:  The PEXSetGlobalTransform functions do not generate any 
    **           errors other than bad renderer or structure.
    **
    **************************************************************************
    */
    PEXSetLocalTransform  (dpy, resource_id, request_type, -1, ident_mat);
    PEXSetLocalTransform2D(dpy, resource_id, request_type, -1, ident_mat2D);
    PEXSetGlobalTransform  (dpy, resource_id, request_type, ident_mat);
    PEXSetGlobalTransform2D(dpy, resource_id, request_type, ident_mat2D);
	 
    /*
    **************************************************************************
    **
    **    PEXSet<SimpleValue> functions that do NOT report any  OC Errors
    **
    **************************************************************************
    */
    PEXSetATextStyle(dpy, resource_id, request_type, PEXATextNotConnected);
    PEXSetLineType(dpy, resource_id, request_type, PEXLineTypeSolid);
    PEXSetMarkerType(dpy, resource_id, request_type, PEXMarkerAsterisk);
    PEXSetPolylineInterpMethod(dpy, resource_id, request_type, 
			       PEXPolylineInterpNone);
    PEXSetRenderingColorModel(dpy, resource_id, request_type, 
		              PEXRenderingColorModelRGB);
    PEXSetReflectionModel(dpy, resource_id, request_type, 
			  PEXReflectionNone);
    PEXSetBFReflectionModel(dpy, resource_id, request_type, 
                            PEXReflectionNone);
    PEXSetSurfaceEdgeType(dpy, resource_id, request_type, 
			  PEXSurfaceEdgeSolid);
    PEXSetSurfaceInterpMethod(dpy, resource_id, request_type, 
		              PEXSurfaceInterpNone);
    PEXSetBFSurfaceInterpMethod(dpy, resource_id, request_type, 
				PEXSurfaceInterpNone);
    PEXSetATextHeight(dpy, resource_id, request_type, 1.0);
    PEXSetCharHeight(dpy, resource_id, request_type, 1.0);
    PEXSetCharExpansion(dpy, resource_id, request_type, 1.0);
    PEXSetCharSpacing(dpy, resource_id, request_type, 1.0);
    PEXSetPickID(dpy, resource_id, request_type, 1);
    PEXSetHLHSRID(dpy, resource_id, request_type, PEXHLHSRIDEnable);
    PEXSetMarkerScale(dpy, resource_id, request_type, 1.0);
    PEXSetLineWidth(dpy, resource_id, request_type, 1.0);
    PEXSetSurfaceEdgeWidth(dpy, resource_id, request_type, 1.0);

    /*
    **************************************************************************
    **
    **    PEXSet<SimpleValue> functions that do report OC Errors for 
    **    invalid enumerated types.
    **
    **************************************************************************
    */
    PEXSetFacetDistinguishFlag(dpy, resource_id, request_type, 9999);
    PEXSetFacetCullingMode(dpy, resource_id, request_type, 9999);
    PEXSetInteriorStyle(dpy, resource_id, request_type, 9999);
    PEXSetBFInteriorStyle(dpy, resource_id, request_type, 9999);
    PEXSetModelClipFlag(dpy, resource_id, request_type, 9999);
    PEXSetSurfaceEdgeFlag(dpy, resource_id, request_type, 9999);
    PEXSetATextPath(dpy, resource_id, request_type, 9999);
    PEXSetTextPath(dpy, resource_id, request_type, 9999);
    PEXSetTextPrecision(dpy, resource_id, request_type, 9999);
    /*
    **************************************************************************
    **
    **    PEXSet <Text Alignment> functions
    **
    **************************************************************************
    */
    PEXSetATextAlignment(dpy, resource_id, request_type, 9999, 9999);
    PEXSetTextAlignment (dpy, resource_id, request_type, 9999, 9999);
    /*
    **************************************************************************
    **
    **    PEXSet <Text Up Vector> functions
    **
    **************************************************************************
    */
    PEXSetATextUpVector(dpy, resource_id, request_type, &valid_up_vector);
    PEXSetCharUpVector (dpy, resource_id, request_type, &valid_up_vector);
	 
    /*
    **************************************************************************
    **
    **    PEXSet <Reflection Attributes> functions
    **
    **************************************************************************
    */
    PEXSetReflectionAttributes  (dpy, resource_id, request_type, 
				 &bad_refl_attr);
    PEXSetBFReflectionAttributes(dpy, resource_id, request_type, 
				 &bad_refl_attr);
    /*
    **************************************************************************
    **
    **    PEXSet <Model Clipping> functions
    **
    **************************************************************************
    */
    PEXSetModelClipVolume(dpy, resource_id, request_type, 
			  PEXModelClipReplace, 0, &half_space);
    PEXSetModelClipVolume2D(dpy, resource_id, request_type, 
			    PEXModelClipReplace, 0, &half_space2D);
    PEXRestoreModelClipVolume(dpy, resource_id, request_type);

    /*
    **************************************************************************
    **
    **    PEXSet <Curve / Suface Approx> functions
    **
    **************************************************************************
    */
    PEXSetCurveApprox(dpy, resource_id, request_type, PEXApproxImpDep, 0.5);
    PEXSetSurfaceApprox(dpy, resource_id, request_type, 
			PEXApproxImpDep, 0.5, 0.5);
	 
    /*
    **************************************************************************
    **
    **    PEXSetLightSourceState function
    **
    **************************************************************************
    */
    PEXSetLightSourceState(dpy, resource_id, request_type, 
			   1, &bad_enable_list1,
			   1, &bad_disable_list1);
    PEXSetLightSourceState(dpy, resource_id, request_type, 
			   1, &bad_enable_list2,
			   1, &bad_disable_list2);
    /*
    **************************************************************************
    **
    **    PEXSetIndividualASF function
    **
    **************************************************************************
    */
    PEXSetIndividualASF(dpy, resource_id, request_type, 0x3, PEXIndividual);
    PEXSetIndividualASF(dpy, resource_id, request_type, 
			(PEXASFSurfaceEdgeColor*2), PEXIndividual);
    PEXSetIndividualASF(dpy, resource_id, request_type, 
			PEXASFSurfaceEdgeColor, 9999);
    /*
    **************************************************************************
    **
    **    PEXSetParaSurfCharacteristics function
    **
    **************************************************************************
    */
    PEXSetParaSurfCharacteristics(dpy, resource_id, request_type, 0, &psc_data);
    PEXSetParaSurfCharacteristics(dpy, resource_id, request_type, 6, &psc_data);
    /*
    **************************************************************************
    **
    **    PEXSet<PatternAttributes> function
    **
    **************************************************************************
    */
    PEXSetPatternAttributes(dpy, resource_id, request_type, 
                            &ref_point, &vector1, &vector2);
    PEXSetPatternAttributes2D(dpy, resource_id, request_type, 
                            &ref_point2D);
    PEXSetPatternSize(dpy, resource_id, request_type, 1.0, 1.0);
	 
    /*
    **************************************************************************
    **
    **    PEXSet<Nameset> function
    **
    **************************************************************************
    */
    PEXAddToNameSet     (dpy, resource_id, request_type, 0, &names);
    PEXRemoveFromNameSet(dpy, resource_id, request_type, 0, &names);
    
    }


void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "idle_renderer");
    *num_images      = 1;
    *supported_modes = IMM_MODE;
    strcpy(win_title, "Idle Renderer Attributes");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    _hppex_set_stderr("idle_renderer.stderr", 
		      "Idle Renderer Attributes");
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    /*****  Force idle renderer *******/
    PEXEndRendering(dpy, resourceID, True);

    describe_test("\nImage 1\n");
    describe_test("  Immediate Mode, Renderer Idle\n");
    describe_test("  This test executes every OC Attribute with a \n");
    describe_test("  condition that would generate an error message if \n");
    describe_test("  executed under an active renderer.  However, \n");
    describe_test("  since we are testing with an idle renderer,  \n");
    describe_test("  NO error messages should be generated and all\n");
    describe_test("  OC Attributes should be silently ignored.  \n\n");

    issue_attrs(dpy, resourceID, req_type);
    _hppex_close_stderr();
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
    file_testproc("idle_renderer.stderr", "Idle Renderer Attributes");
    image_testproc("idle_renderer.image1", "Idle Renderer Attributes", img_desc,
		      dpy,window, 0, 0, 
		      glob_window_width, glob_window_height);
    }
