/* $XConsortium: attr_errors.c,v 1.0 93/11/22 12:30:35 rws Exp $ */

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

#include <misc.h>
#include <err_handler.h>

#define BAD_RESOURCE_ID 0

void print_test_description()
    {
    describe_test("\nThis test attempts to issue all attribute setting OCs\n");
    describe_test("with all probable error conditions.  X Errors such as\n");
    describe_test("BadAlloc are NOT tested.\n\n");
    describe_test("It is only supported in Rendering mode because\n");
    describe_test("both PEXOCRender and PEXOCStore must tested together\n");
    describe_test("as the output errors differ in the minor_code.\n\n");
    describe_test("In pass 1, valid renderer and structure IDs are tested,\n");
    describe_test("while in pass 2, the same OCs are sent to invalid\n");
    describe_test("renderer and structure IDs.\n\n");
    describe_test("The image files consist of the expected error output\n");
    describe_test("for the 2 passes.\n\n");
    }

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
    XSync (dpy, False);
    PEXSetBFSurfaceColor(dpy, resource_id, request_type, 
			 PEXColorTypeRGB, &bad_rgb_color1);

    XSync (dpy, False);
    PEXSetBFSurfaceColor(dpy, resource_id, request_type, 
			 PEXColorTypeRGB, &bad_rgb_color2);
    XSync (dpy, False);
    PEXSetBFSurfaceColor(dpy, resource_id, request_type, 
			 bad_color_type, &bad_rgb_color1);
    XSync (dpy, False);
    PEXSetBFSurfaceColor(dpy, resource_id, request_type, 
			 PEXColorTypeIndexed, &bad_indexed_color);
    XSync (dpy, False);

    PEXSetLineColor(dpy, resource_id, request_type, 
		    PEXColorTypeRGB, &bad_rgb_color1);
    XSync (dpy, False);
    PEXSetLineColor(dpy, resource_id, request_type, 
	            PEXColorTypeRGB, &bad_rgb_color2);
    XSync (dpy, False);
    PEXSetLineColor(dpy, resource_id, request_type, 
	            bad_color_type, &bad_rgb_color1);
    XSync (dpy, False);
    PEXSetLineColor(dpy, resource_id, request_type, 
		    PEXColorTypeIndexed, &bad_indexed_color);
    XSync (dpy, False);

    PEXSetMarkerColor(dpy, resource_id, request_type, 
		      PEXColorTypeRGB, &bad_rgb_color1);
    XSync (dpy, False);
    PEXSetMarkerColor(dpy, resource_id, request_type, 
		      PEXColorTypeRGB, &bad_rgb_color2);
    XSync (dpy, False);
    PEXSetMarkerColor(dpy, resource_id, request_type, 
		      bad_color_type, &bad_rgb_color1);
    XSync (dpy, False);
    PEXSetMarkerColor(dpy, resource_id, request_type, 
		      PEXColorTypeIndexed, &bad_indexed_color);
    XSync (dpy, False);

    PEXSetSurfaceColor(dpy, resource_id, request_type, 
		       PEXColorTypeRGB, &bad_rgb_color1);
    XSync (dpy, False);
    PEXSetSurfaceColor(dpy, resource_id, request_type, 
		       PEXColorTypeRGB, &bad_rgb_color2);
    XSync (dpy, False);
    PEXSetSurfaceColor(dpy, resource_id, request_type, 
		       bad_color_type, &bad_rgb_color1);
    XSync (dpy, False);
    PEXSetSurfaceColor(dpy, resource_id, request_type, 
		       PEXColorTypeIndexed, &bad_indexed_color);
    XSync (dpy, False);

    PEXSetSurfaceEdgeColor(dpy, resource_id, request_type, 
			   PEXColorTypeRGB, &bad_rgb_color1);
    XSync (dpy, False);
    PEXSetSurfaceEdgeColor(dpy, resource_id, request_type, 
			   PEXColorTypeRGB, &bad_rgb_color2);
    XSync (dpy, False);
    PEXSetSurfaceEdgeColor(dpy, resource_id, request_type, 
			   bad_color_type, &bad_rgb_color1);
    XSync (dpy, False);
    PEXSetSurfaceEdgeColor(dpy, resource_id, request_type, 
			   PEXColorTypeIndexed, &bad_indexed_color);
    XSync (dpy, False);

    PEXSetTextColor(dpy, resource_id, request_type, 
		    PEXColorTypeRGB, &bad_rgb_color1);
    XSync (dpy, False);
    PEXSetTextColor(dpy, resource_id, request_type, 
		    PEXColorTypeRGB, &bad_rgb_color2);
    XSync (dpy, False);
    PEXSetTextColor(dpy, resource_id, request_type, 
		    bad_color_type, &bad_rgb_color1);
    XSync (dpy, False);
    PEXSetTextColor(dpy, resource_id, request_type, 
		    PEXColorTypeIndexed, &bad_indexed_color);
    XSync (dpy, False);
    /*
    **************************************************************************
    **
    **    PEXSet<blah>ColorIndex functions
    **
    **************************************************************************
    */
    PEXSetBFSurfaceColorIndex  (dpy, resource_id, request_type, 
			        bad_color_index);
    XSync (dpy, False);

    PEXSetLineColorIndex       (dpy, resource_id, request_type, 
			        bad_color_index);
    XSync (dpy, False);

    PEXSetMarkerColorIndex     (dpy, resource_id, request_type, 
			        bad_color_index);
    XSync (dpy, False);

    PEXSetSurfaceColorIndex    (dpy, resource_id, request_type, 
			        bad_color_index);
    XSync (dpy, False);

    PEXSetSurfaceEdgeColorIndex(dpy, resource_id, request_type, 
				bad_color_index);
    XSync (dpy, False);

    PEXSetTextColorIndex       (dpy, resource_id, request_type, 
				bad_color_index);
    XSync (dpy, False);
    /*
    **************************************************************************
    **
    **    PEXSet<blah>Index functions
    **
    **************************************************************************
    */
    PEXSetEdgeBundleIndex(dpy, resource_id, request_type, 0);
    XSync (dpy, False);

    PEXSetInteriorBundleIndex(dpy, resource_id, request_type, 0);
    XSync (dpy, False);

    PEXSetLineBundleIndex(dpy, resource_id, request_type, 0);
    XSync (dpy, False);

    PEXSetMarkerBundleIndex(dpy, resource_id, request_type, 0);
    XSync (dpy, False);

    PEXSetTextBundleIndex(dpy, resource_id, request_type, 0);
    XSync (dpy, False);

    PEXSetColorApproxIndex(dpy, resource_id, request_type, 65535);
    XSync (dpy, False);

    PEXSetDepthCueIndex(dpy, resource_id, request_type, 65535);
    XSync (dpy, False);

    PEXSetTextFontIndex(dpy, resource_id, request_type, 0);
    XSync (dpy, False);

    PEXSetViewIndex(dpy, resource_id, request_type, 65535);
    XSync (dpy, False);

    /*
    **************************************************************************
    **
    **    PEXSet <Interior Style Index> functions
    **    NOTE:  These do not generate any errors except for bad renderer
    **           or structure.
    **
    **************************************************************************
    */
    if (resource_id != BAD_RESOURCE_ID)
	{
        _hppex_stderr_print( 
  	  "  PEXSetInteriorStyleIndex    does NOT generate any OC errors\n");
        _hppex_stderr_print( 
	  "  PEXBFSetInteriorStyleIndex  does NOT generate any OC errors\n");
	}
    else
	{
        PEXSetInteriorStyleIndex(dpy, resource_id, request_type, 0);
        XSync (dpy, False);
        PEXSetBFInteriorStyleIndex(dpy, resource_id, request_type, 0);
	}
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
    XSync(dpy, False);
    PEXSetLocalTransform2D(dpy, resource_id, request_type, -1, ident_mat2D);
    XSync(dpy, False);

    if (resource_id != BAD_RESOURCE_ID)
	{
        _hppex_stderr_print( 
	  "  PEXSetGlobalTransform       does NOT generate any OC errors\n");
        _hppex_stderr_print( 
	  "  PEXSetGlobalTransform2D     does NOT generate any OC errors\n");
	}
    else
	{
        PEXSetGlobalTransform  (dpy, resource_id, request_type, ident_mat);
        XSync (dpy, False);
        PEXSetGlobalTransform2D(dpy, resource_id, request_type, ident_mat2D);
        XSync (dpy, False);
	}
    /*
    **************************************************************************
    **
    **    PEXSet<SimpleValue> functions that do NOT report any  OC Errors
    **
    **************************************************************************
    */
    if (resource_id != BAD_RESOURCE_ID)
	{
        _hppex_stderr_print( 
          "  PEXSetATextStyle            does NOT generate any OC errors\n");
        _hppex_stderr_print( 
          "  PEXSetLineType              does NOT generate any OC errors\n");
        _hppex_stderr_print( 
          "  PEXSetMarkerType            does NOT generate any OC errors\n");
        _hppex_stderr_print( 
          "  PEXSetPolylineInterpMethod  does NOT generate any OC errors\n");
        _hppex_stderr_print( 
          "  PEXSetRenderingColorModel   does NOT generate any OC errors\n");
        _hppex_stderr_print( 
          "  PEXSetReflectionModel       does NOT generate any OC errors\n");
        _hppex_stderr_print( 
          "  PEXSetBFReflectionModel     does NOT generate any OC errors\n");
        _hppex_stderr_print( 
          "  PEXSetSurfaceEdgeType       does NOT generate any OC errors\n");
        _hppex_stderr_print( 
          "  PEXSetSurfaceInterpMethod   does NOT generate any OC errors\n");
        _hppex_stderr_print( 
          "  PEXSetBFSurfaceInterpMethod does NOT generate any OC errors\n");
        _hppex_stderr_print( 
          "  PEXSetATextHeight           does NOT generate any OC errors\n");
        _hppex_stderr_print( 
          "  PEXSetCharHeight            does NOT generate any OC errors\n");
        _hppex_stderr_print( 
          "  PEXSetCharExpansion         does NOT generate any OC errors\n");
        _hppex_stderr_print( 
          "  PEXSetCharSpacing           does NOT generate any OC errors\n");
        _hppex_stderr_print( 
          "  PEXSetPickID                does NOT generate any OC errors\n");
        _hppex_stderr_print( 
          "  PEXSetHLHSRID               does NOT generate any OC errors\n");
        _hppex_stderr_print( 
          "  PEXSetMarkerScale           does NOT generate any OC errors\n");
        _hppex_stderr_print( 
          "  PEXSetLineWidth             does NOT generate any OC errors\n");
        _hppex_stderr_print( 
          "  PEXSetSurfaceEdgeWidth      does NOT generate any OC errors\n");
	}
    else
	{
        PEXSetATextStyle(dpy, resource_id, request_type, PEXATextNotConnected);
        XSync (dpy, False);
        PEXSetLineType(dpy, resource_id, request_type, PEXLineTypeSolid);
        XSync (dpy, False);
        PEXSetMarkerType(dpy, resource_id, request_type, PEXMarkerAsterisk);
        XSync (dpy, False);
        PEXSetPolylineInterpMethod(dpy, resource_id, request_type, 
				   PEXPolylineInterpNone);
        XSync (dpy, False);
        PEXSetRenderingColorModel(dpy, resource_id, request_type, 
				  PEXRenderingColorModelRGB);
        XSync (dpy, False);
        PEXSetReflectionModel(dpy, resource_id, request_type, 
			      PEXReflectionNone);
        XSync (dpy, False);
        PEXSetBFReflectionModel(dpy, resource_id, request_type, 
				PEXReflectionNone);
        XSync (dpy, False);
        PEXSetSurfaceEdgeType(dpy, resource_id, request_type, 
			      PEXSurfaceEdgeSolid);
        XSync (dpy, False);
        PEXSetSurfaceInterpMethod(dpy, resource_id, request_type, 
				  PEXSurfaceInterpNone);
        XSync (dpy, False);
        PEXSetBFSurfaceInterpMethod(dpy, resource_id, request_type, 
				    PEXSurfaceInterpNone);
        XSync (dpy, False);
        PEXSetATextHeight(dpy, resource_id, request_type, 1.0);
        XSync (dpy, False);
        PEXSetCharHeight(dpy, resource_id, request_type, 1.0);
        XSync (dpy, False);
        PEXSetCharExpansion(dpy, resource_id, request_type, 1.0);
        XSync (dpy, False);
        PEXSetCharSpacing(dpy, resource_id, request_type, 1.0);
        XSync (dpy, False);
        PEXSetPickID(dpy, resource_id, request_type, 1);
        XSync (dpy, False);
        PEXSetHLHSRID(dpy, resource_id, request_type, PEXHLHSRIDEnable);
        XSync (dpy, False);
        PEXSetMarkerScale(dpy, resource_id, request_type, 1.0);
        XSync (dpy, False);
        PEXSetLineWidth(dpy, resource_id, request_type, 1.0);
        XSync (dpy, False);
        PEXSetSurfaceEdgeWidth(dpy, resource_id, request_type, 1.0);
        XSync (dpy, False);
	}

    /*
    **************************************************************************
    **
    **    PEXSet<SimpleValue> functions that do report OC Errors for 
    **    invalid enumerated types.
    **
    **************************************************************************
    */
    PEXSetFacetDistinguishFlag(dpy, resource_id, request_type, 9999);
    XSync (dpy, False);
    PEXSetFacetCullingMode(dpy, resource_id, request_type, 9999);
    XSync (dpy, False);
    PEXSetInteriorStyle(dpy, resource_id, request_type, 9999);
    XSync (dpy, False);
    PEXSetBFInteriorStyle(dpy, resource_id, request_type, 9999);
    XSync (dpy, False);
    PEXSetModelClipFlag(dpy, resource_id, request_type, 9999);
    XSync (dpy, False);
    PEXSetSurfaceEdgeFlag(dpy, resource_id, request_type, 9999);
    XSync (dpy, False);
    PEXSetATextPath(dpy, resource_id, request_type, 9999);
    XSync (dpy, False);
    PEXSetTextPath(dpy, resource_id, request_type, 9999);
    XSync (dpy, False);
    PEXSetTextPrecision(dpy, resource_id, request_type, 9999);
    XSync (dpy, False);
    /*
    **************************************************************************
    **
    **    PEXSet <Text Alignment> functions
    **
    **************************************************************************
    */
    PEXSetATextAlignment(dpy, resource_id, request_type, 9999, 9999);
    XSync (dpy, False);
    PEXSetTextAlignment (dpy, resource_id, request_type, 9999, 9999);
    XSync (dpy, False);
    /*
    **************************************************************************
    **
    **    PEXSet <Text Up Vector> functions
    **
    **************************************************************************
    */
    if (resource_id != BAD_RESOURCE_ID)
	{
        _hppex_stderr_print( 
          "  PEXSetATextUpVector         does NOT generate any OC errors\n");
        _hppex_stderr_print( 
          "  PEXSetCharUpVector          does NOT generate any OC errors\n");
	}
    else
	{
        PEXSetATextUpVector(dpy, resource_id, request_type, &valid_up_vector);
        XSync (dpy, False);
        PEXSetCharUpVector (dpy, resource_id, request_type, &valid_up_vector);
        XSync (dpy, False);
	}
    /*
    **************************************************************************
    **
    **    PEXSet <Reflection Attributes> functions
    **
    **************************************************************************
    */
    PEXSetReflectionAttributes  (dpy, resource_id, request_type, 
				 &bad_refl_attr);
    XSync (dpy, False);
    PEXSetBFReflectionAttributes(dpy, resource_id, request_type, 
				 &bad_refl_attr);
    XSync (dpy, False);
    /*
    **************************************************************************
    **
    **    PEXSet <Model Clipping> functions
    **
    **************************************************************************
    */
    if (resource_id != BAD_RESOURCE_ID)
	{
        _hppex_stderr_print( 
          "  PEXSetModelClipVolume       does NOT generate any OC errors\n");
        _hppex_stderr_print( 
          "  PEXSetModelClipVolume2D     does NOT generate any OC errors\n");
        _hppex_stderr_print( 
          "  PEXRestoreModelClipVolume   does NOT generate any OC errors\n");
	}
    else
	{
        PEXSetModelClipVolume(dpy, resource_id, request_type, 
			      PEXModelClipReplace, 0, &half_space);
        XSync (dpy, False);
        PEXSetModelClipVolume2D(dpy, resource_id, request_type, 
			        PEXModelClipReplace, 0, &half_space2D);
        XSync (dpy, False);
        PEXRestoreModelClipVolume(dpy, resource_id, request_type);
        XSync (dpy, False);
	}
    /*
    **************************************************************************
    **
    **    PEXSet <Curve / Suface Approx> functions
    **
    **************************************************************************
    */
    if (resource_id != BAD_RESOURCE_ID)
	{
        _hppex_stderr_print( 
          "  PEXSetCurveApprox           does NOT generate any OC errors\n");
        _hppex_stderr_print( 
          "  PEXSetSurfaceApprox         does NOT generate any OC errors\n");
	}
    else
	{
        PEXSetCurveApprox(dpy, resource_id, request_type, 
			  PEXApproxImpDep, 0.5);
        XSync (dpy, False);
        PEXSetSurfaceApprox(dpy, resource_id, request_type, 
			    PEXApproxImpDep, 0.5, 0.5);
        XSync (dpy, False);
	}
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
    XSync (dpy, False);
    PEXSetLightSourceState(dpy, resource_id, request_type, 
			   1, &bad_enable_list2,
			   1, &bad_disable_list2);
    XSync (dpy, False);
    /*
    **************************************************************************
    **
    **    PEXSetIndividualASF function
    **
    **************************************************************************
    */
    PEXSetIndividualASF(dpy, resource_id, request_type, 0x3, PEXIndividual);
    XSync (dpy, False);
    PEXSetIndividualASF(dpy, resource_id, request_type, 
			(PEXASFSurfaceEdgeColor*2), PEXIndividual);
    XSync (dpy, False);
    PEXSetIndividualASF(dpy, resource_id, request_type, 
			PEXASFSurfaceEdgeColor, 9999);
    XSync (dpy, False);
    /*
    **************************************************************************
    **
    **    PEXSetParaSurfCharacteristics function
    **
    **************************************************************************
    */
    PEXSetParaSurfCharacteristics(dpy, resource_id, request_type, 0, &psc_data);
    XSync (dpy, False);
    PEXSetParaSurfCharacteristics(dpy, resource_id, request_type, 6, &psc_data);
    XSync (dpy, False);
    /*
    **************************************************************************
    **
    **    PEXSet<PatternAttributes> function
    **
    **************************************************************************
    */
    if (resource_id != BAD_RESOURCE_ID)
	{
        _hppex_stderr_print( 
          "  PEXSetPatternAttributes     does NOT generate any OC errors\n");
        _hppex_stderr_print( 
          "  PEXSetPatternAttributes2D   does NOT generate any OC errors\n");
        _hppex_stderr_print( 
          "  PEXSetPatternSize           does NOT generate any OC errors\n");
	}
    else
	{
        PEXSetPatternAttributes(dpy, resource_id, request_type, 
				&ref_point, &vector1, &vector2);
        XSync (dpy, False);
        PEXSetPatternAttributes2D(dpy, resource_id, request_type, 
				&ref_point2D);
        XSync (dpy, False);
        PEXSetPatternSize(dpy, resource_id, request_type, 1.0, 1.0);
        XSync (dpy, False);
	}
    /*
    **************************************************************************
    **
    **    PEXSet<Nameset> function
    **
    **************************************************************************
    */
    if (resource_id != BAD_RESOURCE_ID)
	{
        _hppex_stderr_print( 
          "  PEXAddToNameSet             does NOT generate any OC errors\n");
        _hppex_stderr_print( 
          "  PEXRemoveFromNameSet        does NOT generate any OC errors\n");
	}
    else
	{
        PEXAddToNameSet     (dpy, resource_id, request_type, 0, &names);
        XSync (dpy, False);
        PEXRemoveFromNameSet(dpy, resource_id, request_type, 0, &names);
        XSync (dpy, False);
	}
    }



void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "attr_errors");
    *num_images      = 2;
    *supported_modes = IMM_MODE;

    strcpy(win_title, "Attr Errors Test");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    char err_file_name[80], test_name[80];

    sprintf(err_file_name, "attr_errors_%d", cur_image);
    sprintf(test_name,     "Attribute Errors #%d", cur_image);
    _hppex_set_stderr(err_file_name, test_name);
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    PEXStructure sid = PEXCreateStructure(dpy);

    switch (cur_image)
        {
	case 1:
        print_test_description();
        /*
	     Test #1 -- Renderer ID Valid
        */
        _hppex_stderr_print( "\nTest 1\n");
        _hppex_stderr_print( "\n  Renderer is Valid\n");
        issue_attrs(dpy, resourceID, PEXOCRender);

        _hppex_stderr_print( "\n  Structure is Valid\n");
        issue_attrs(dpy, sid, PEXOCStore);

	_hppex_close_stderr();
	break;

	case 2:
        /*
	     Test #2 -- Resource ID Invalid
        */
        _hppex_stderr_print( "\nTest 2\n");
        _hppex_stderr_print( "\n  Renderer is Invalid\n");
        issue_attrs(dpy, BAD_RESOURCE_ID, PEXOCRender);

        _hppex_stderr_print( "\n  Structure is Invalid\n");
        issue_attrs(dpy, BAD_RESOURCE_ID, PEXOCStore);

	_hppex_close_stderr();
	break;
	}
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
    char image_name[80], test_name[80];

    sprintf(image_name,"attr_errors_%d", cur_image);
    sprintf(test_name, "Attribute Errors #%d", cur_image);
    file_testproc(image_name, test_name);
    }
