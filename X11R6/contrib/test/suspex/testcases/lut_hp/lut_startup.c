/* $XConsortium: lut_startup.c,v 1.0 93/11/22 12:29:00 rws Exp $ */

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

/*
 * lut_startup
 *
 * Test for correct baseline functionality of lut module.
 * This test can be built with a malloc checker to verify that all allocated
 * memory is freed. This test will generate errors wherever a pattern table is
 * operated on.
 * This test has 5 basic steps:
 * 	1. Get Table info and verify the correctness of that info.
 *	2. Get predefined entries for luts and verify correctness.
 *      3. Create Luts verifying luts are indeed created as expected.
 *	4. Check the defined indices on the newly created luts to see
 *	   if they are as expected from the predefined entries.
 *	5. Delete all the LUTs
 * Aug. 20, 1992 	Ken Tidwell
 */
 
/*
 * Basic set of include files for tests
 */
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/PEX5/PEXlib.h>
#include "err_handler.h"
#include "misc.h"

Window local_window;

#define NUM_LUTS 12
#define NUM_ENTRIES (NUM_LUTS + 1)

void lut_test(Display *display,
	      Window   window)
{
    PEXLookupTable luts[NUM_ENTRIES];     /* 12 lut types, 0'th entry ignored */
    PEXTableInfo lut_info[NUM_ENTRIES];	  /* 12 lut types, 0'th entry ignored */
    PEXPointer entries [NUM_ENTRIES];	  /* 12 lut types, 0'th entry ignored */

    /* Update this array if PEX spec changes in the future */
    PEXTableInfo expected_lut_info[NUM_ENTRIES] = {
			{0, 0, 0, 0},   /* Ignored */
			{65535, 1, 1, 1},	/* LineBundle */
			{65535, 1, 1, 1},	/* MarkerBundle */
			{65535, 1, 1, 1},	/* TextBundle */
			{65535, 1, 1, 1},	/* InteriorBundle */
			{65535, 1, 1, 1},	/* EdgeBundle */
			{0, 0, 0, 0},	/* Pattern, should be error */
			{65535, 1, 1, 1},	/* TextFont */
			{65535, 8, 0, 7},	/* Color */
			{65535, 1, 0, 0},	/* View */
			{65535, 1, 1, 1},	/* Light */
			{65535, 1, 0, 0},	/* DepthCue */
			{65535, 1, 0, 0},	/* ColorApprox */
			};
    int i, j;
    int table_type;
    Status status;
    unsigned long return_count;
    PEXTableIndex *return_indices;

    PEXLineBundleEntry 		*p_LineBundle;
    PEXMarkerBundleEntry 	*p_MarkerBundle;
    PEXTextBundleEntry 		*p_TextBundle;
    PEXInteriorBundleEntry 	*p_InteriorBundle;
    PEXEdgeBundleEntry 		*p_EdgeBundle;
    PEXColorEntry 		*p_Color;
    PEXTextFontEntry 		*p_TextFont;
    PEXLightEntry 		*p_Light;
    PEXDepthCueEntry 		*p_DepthCue;
    PEXColorApproxEntry 	*p_ColorApprox;
    PEXViewEntry 		*p_View;

    int				err_mask = ERROR_CODE | REQUEST_CODE;
    
    /* For checking out View entries */
    int orientation_okay = True, mapping_okay = True;

#ifdef PATTERN_LUT_SUPPORTED
    int pattern_lut_supported = True;
#else
    int pattern_lut_supported = False;
#endif

    /**********************************************************************
     * Test begins here
     *********************************************************************/

    /*************
     * STEP 1
     ************/

    /*
     * Get table info for each kind of LUT. Pattern will fail.
     */

    for (i=1; i<NUM_ENTRIES; i++)
    {
	if (i == PEXLUTPattern && !pattern_lut_supported)
	    _hppex_test_set_expected_error (PEX_ERR, BadPEXLookupTable,
                                        	PEXRCGetTableInfo, 0, 0);

	status = PEXGetTableInfo (display, window, i, &lut_info[i]);

        XSync(display, False);

	if (i == PEXLUTPattern && !pattern_lut_supported)
        {
            XSync(display, 0);
	    _hppex_test_check_expected_error (err_mask);
        }

	if (status)
        {
	    if (i == PEXLUTPattern && !pattern_lut_supported)
	        _hppex_stderr_print("Get Info on Pattern did not fail as expected\n");
	}
	else
	{
	    if (i != PEXLUTPattern && !pattern_lut_supported)
	        _hppex_stderr_print("Get Info failed on lut %d\n", i);
	}
    }

    /*
     * Verify correctness of table info
     */

    for (i=1; i<NUM_ENTRIES; i++)
    {
        if (i != PEXLUTPattern && !pattern_lut_supported)
	{
	    if (!((lut_info[i].definable_entries == 
		   expected_lut_info[i].definable_entries) &&
		  (lut_info[i].predefined_count ==
		   expected_lut_info[i].predefined_count) &&
		  (lut_info[i].predefined_min ==
		   expected_lut_info[i].predefined_min) &&
		  (lut_info[i].predefined_max ==
		   expected_lut_info[i].predefined_max)))
	    {
		_hppex_stderr_print("Table info for LUT %d is bad\n", i);
	    }
	}
    } 

    /*************
     * STEP 2
     ************/

    /*
     * Get predefined entries for each LUT. Pattern should fail.
     */
    for (i=1; i<NUM_ENTRIES; i++)
    {
	if (i == PEXLUTPattern && !pattern_lut_supported)
	    _hppex_test_set_expected_error (PEX_ERR, BadPEXLookupTable,
                                        PEXRCGetPredefinedEntries, 0, 0);

	status = PEXGetPredefinedEntries (display, window, i, 
					  lut_info[i].predefined_min,
					  lut_info[i].predefined_count,
					  &entries[i]);
        XSync(display, False);

	if (i == PEXLUTPattern && !pattern_lut_supported)
        {
            XSync(display, 0);
	    _hppex_test_check_expected_error (err_mask);
        }

	if (status)
        {
	    if (i == PEXLUTPattern && !pattern_lut_supported)
	        _hppex_stderr_print("Get Predefs on Pattern did not fail as expected\n");
	}
	else
	{
	    if (i != PEXLUTPattern && !pattern_lut_supported)
	        _hppex_stderr_print("Get Predefs  failed on lut %d\n", i);
	}
    }
    
#ifdef _HPUX_SOURCE
    /* 
     * Examine the entries for expected values. This is a "hard coded" check.
     * This code will have to be updated to match changes in the contents 
     * of predefined entries.
     */

    p_LineBundle = (PEXLineBundleEntry *) entries [PEXLUTLineBundle];

    if ( !((p_LineBundle->type == PEXLineTypeSolid) &&
	   (p_LineBundle->interp_method == PEXPolylineInterpNone) &&
	   (p_LineBundle->curve_approx.method == PEXApproxImpDep) &&
	   (p_LineBundle->curve_approx.tolerance == 1.0) &&
	   (p_LineBundle->width == 1.0) &&
	   (p_LineBundle->color.type == PEXColorTypeIndexed) &&
	   (p_LineBundle->color.value.indexed.index == 1)))
    {
	_hppex_stderr_print("Predefined Line Bundle entry is incorrect\n");
    }


    p_MarkerBundle = (PEXMarkerBundleEntry *) entries [PEXLUTMarkerBundle];

    if ( !((p_MarkerBundle->type == PEXMarkerAsterisk) &&
	   (p_MarkerBundle->scale == 1.0) &&
	   (p_MarkerBundle->color.type == PEXColorTypeIndexed) &&
	   (p_MarkerBundle->color.value.indexed.index == 1)))
    {
	_hppex_stderr_print("Predefined Marker Bundle entry is incorrect\n");
    }


    p_TextBundle = (PEXTextBundleEntry *) entries [PEXLUTTextBundle];

    if ( !((p_TextBundle->font_index == 1) &&
	   (p_TextBundle->precision == PEXStrokePrecision) &&
	   (p_TextBundle->char_expansion == 1.0) &&
	   (p_TextBundle->char_spacing == 0.0) &&
	   (p_TextBundle->color.type == PEXColorTypeIndexed) &&
	   (p_TextBundle->color.value.indexed.index == 1)))
    {
	_hppex_stderr_print("Predefined Text Bundle entry is incorrect\n");
    }

    p_InteriorBundle = (PEXInteriorBundleEntry *) entries[PEXLUTInteriorBundle];

    if ( !((p_InteriorBundle->style == PEXInteriorStyleHollow) &&
	   (p_InteriorBundle->style_index == 0) &&
	   (p_InteriorBundle->reflection_model == PEXReflectionNone) &&
	   (p_InteriorBundle->interp_method == PEXSurfaceInterpNone) &&
	   (p_InteriorBundle->bf_style == PEXInteriorStyleHollow) &&
	   (p_InteriorBundle->bf_style_index == 0) &&
	   (p_InteriorBundle->bf_reflection_model == PEXReflectionNone) &&
	   (p_InteriorBundle->bf_interp_method == PEXSurfaceInterpNone) &&
	   (p_InteriorBundle->surface_approx.method == PEXApproxImpDep) &&
	   (p_InteriorBundle->surface_approx.u_tolerance == 1.0) &&
	   (p_InteriorBundle->surface_approx.v_tolerance == 1.0) &&
	   (p_InteriorBundle->color.type == PEXColorTypeIndexed) &&
	   (p_InteriorBundle->color.value.indexed.index == 1) &&
	   (p_InteriorBundle->reflection_attr.ambient == 1.0) &&
	   (p_InteriorBundle->reflection_attr.diffuse == 1.0) &&
	   (p_InteriorBundle->reflection_attr.specular == 1.0) &&
	   (p_InteriorBundle->reflection_attr.specular_conc == 0.0) &&
	   (p_InteriorBundle->reflection_attr.transmission == 0.0) &&
	   (p_InteriorBundle->reflection_attr.specular_color.type 
						== PEXColorTypeIndexed) &&
	   (p_InteriorBundle->reflection_attr.specular_color.value.indexed.index								 == 1 ) &&
	   (p_InteriorBundle->bf_color.type == PEXColorTypeIndexed) &&
	   (p_InteriorBundle->bf_color.value.indexed.index == 1) &&
	   (p_InteriorBundle->bf_reflection_attr.ambient == 1.0) &&
	   (p_InteriorBundle->bf_reflection_attr.diffuse == 1.0) &&
	   (p_InteriorBundle->bf_reflection_attr.specular == 1.0) &&
	   (p_InteriorBundle->bf_reflection_attr.specular_conc == 0.0) &&
	   (p_InteriorBundle->bf_reflection_attr.transmission == 0.0) &&
	   (p_InteriorBundle->bf_reflection_attr.specular_color.type 
						== PEXColorTypeIndexed) &&
        (p_InteriorBundle->bf_reflection_attr.specular_color.value.indexed.index
								== 1) ))
    {
	_hppex_stderr_print("Predefined Interior Bundle entry is incorrect\n");
    }


    p_EdgeBundle = (PEXEdgeBundleEntry *) entries [PEXLUTEdgeBundle];

    if ( !((p_EdgeBundle->edge_flag == PEXOff) &&
	   (p_EdgeBundle->type == PEXSurfaceEdgeSolid) &&
	   (p_EdgeBundle->width == 1.0) &&
	   (p_EdgeBundle->color.type == PEXColorTypeIndexed) &&
	   (p_EdgeBundle->color.value.indexed.index == 1)))
    {
	_hppex_stderr_print("Predefined Edge Bundle entry is incorrect\n");
    }

    /*
     * Won't check pattern cause it should have failed above.
     */


    p_TextFont = (PEXTextFontEntry *) entries[PEXLUTTextFont];

    /*
     * Font module should be checking that font is valid. We'll just check the
     * count.
     */
    if ( !((p_TextFont->count == 1) ))
    {
	_hppex_stderr_print("Predefined TextFont entry is incorrect\n");
    }


    /* 
     * There are 8 color entries to check out
     */
    p_Color = (PEXColorEntry *) entries [PEXLUTColor];

    /* Black */
    if ( !((p_Color->type == PEXColorTypeRGB) &&
	   (p_Color->value.rgb.red == 0.0) &&
	   (p_Color->value.rgb.green == 0.0) &&
	   (p_Color->value.rgb.blue == 0.0) ))
    {
	_hppex_stderr_print("Predefined Color entry 0 (black) is incorrect\n");
    }

    p_Color++;

    /* White */
    if ( !((p_Color->type == PEXColorTypeRGB) &&
	   (p_Color->value.rgb.red == 1.0) &&
	   (p_Color->value.rgb.green == 1.0) &&
	   (p_Color->value.rgb.blue == 1.0) ))
    {
	_hppex_stderr_print("Predefined Color entry 1 (white) is incorrect\n");
    }

    p_Color++;

    /* Red */
    if ( !((p_Color->type == PEXColorTypeRGB) &&
	   (p_Color->value.rgb.red == 1.0) &&
	   (p_Color->value.rgb.green == 0.0) &&
	   (p_Color->value.rgb.blue == 0.0) ))
    {
	_hppex_stderr_print("Predefined Color entry 2 (red) is incorrect\n");
    }

    p_Color++;

    /* Yellow */
    if ( !((p_Color->type == PEXColorTypeRGB) &&
	   (p_Color->value.rgb.red == 1.0) &&
	   (p_Color->value.rgb.green == 1.0) &&
	   (p_Color->value.rgb.blue == 0.0) ))
    {
	_hppex_stderr_print("Predefined Color entry 3 (yellow) is incorrect\n");
    }

    p_Color++;

    /* Green */
    if ( !((p_Color->type == PEXColorTypeRGB) &&
	   (p_Color->value.rgb.red == 0.0) &&
	   (p_Color->value.rgb.green == 1.0) &&
	   (p_Color->value.rgb.blue == 0.0) ))
    {
	_hppex_stderr_print("Predefined Color entry 4 (green) is incorrect\n");
    }

    p_Color++;

    /* Cyan */
    if ( !((p_Color->type == PEXColorTypeRGB) &&
	   (p_Color->value.rgb.red == 0.0) &&
	   (p_Color->value.rgb.green == 1.0) &&
	   (p_Color->value.rgb.blue == 1.0) ))
    {
	_hppex_stderr_print("Predefined Color entry 5 (cyan) is incorrect\n");
    }

    p_Color++;

    /* Blue */
    if ( !((p_Color->type == PEXColorTypeRGB) &&
	   (p_Color->value.rgb.red == 0.0) &&
	   (p_Color->value.rgb.green == 0.0) &&
	   (p_Color->value.rgb.blue == 1.0) ))
    {
	_hppex_stderr_print("Predefined Color entry 6 (blue) is incorrect\n");
    }

    p_Color++;

    /* Magenta */
    if ( !((p_Color->type == PEXColorTypeRGB) &&
	   (p_Color->value.rgb.red == 1.0) &&
	   (p_Color->value.rgb.green == 0.0) &&
	   (p_Color->value.rgb.blue == 1.0) ))
    {
	_hppex_stderr_print("Predefined Color entry 7 (magenta) is incorrect\n");
    }

    
    p_View = (PEXViewEntry *) entries [PEXLUTView];

    for (i=0; i<4; i++)
	for (j=0; j<4; j++)
	{
	    if (i == j)
	    {
		if (p_View->orientation[i][j] != 1.0) orientation_okay = False;
		if (p_View->mapping[i][j] != 1.0) mapping_okay = False;
	    }
	    else
	    {
		if (p_View->orientation[i][j] != 0.0) orientation_okay = False;
		if (p_View->mapping[i][j] != 0.0) mapping_okay = False;
	    }
	}

    if ( !((p_View->clip_flags == PEXClippingAll) &&
	   (p_View->clip_limits.min.x == 0.0) &&
	   (p_View->clip_limits.min.y == 0.0) &&
	   (p_View->clip_limits.min.z == 0.0) &&
	   (p_View->clip_limits.max.x == 1.0) &&
	   (p_View->clip_limits.max.y == 1.0) &&
	   (p_View->clip_limits.max.z == 1.0) &&
	   (orientation_okay ) &&
	   (mapping_okay ) ))
    {
	_hppex_stderr_print("Predefined View Bundle entry is incorrect\n");
    }

    
    p_Light = (PEXLightEntry *) entries [PEXLUTLight];

    if ( !((p_Light->type == PEXLightAmbient) &&
	   (p_Light->direction.x == 0.0) &&
	   (p_Light->direction.y == 0.0) &&
	   (p_Light->direction.z == 0.0) &&
	   (p_Light->point.x== 0.0) &&
	   (p_Light->point.y== 0.0) &&
	   (p_Light->point.z== 0.0) &&
	   (p_Light->concentration == 0.0) &&
	   (p_Light->spread_angle == 0.0) &&
	   (p_Light->attenuation1 == 0.0) &&
	   (p_Light->attenuation2 == 0.0) &&
	   (p_Light->color.type == PEXColorTypeRGB) &&
	   (p_Light->color.value.rgb.red == 1.0) &&
	   (p_Light->color.value.rgb.green == 1.0) &&
	   (p_Light->color.value.rgb.blue == 1.0)))
    {
	_hppex_stderr_print("Predefined Light entry is incorrect\n");
    }


    p_DepthCue = (PEXDepthCueEntry *) entries [PEXLUTDepthCue];

    if ( !((p_DepthCue->mode == PEXOff) &&
	   (p_DepthCue->front_plane == 1.0) &&
	   (p_DepthCue->back_plane == 0.0) &&
	   (p_DepthCue->front_scaling == 1.0) &&
	   (p_DepthCue->back_scaling == 0.5) &&
	   (p_DepthCue->color.type == PEXColorTypeIndexed) &&
	   (p_DepthCue->color.value.indexed.index == 0)))
    {
	_hppex_stderr_print("Predefined View entry is incorrect\n");
    }


    p_ColorApprox = (PEXColorApproxEntry *) entries [PEXLUTColorApprox];

    if ( !(
#ifndef CMAP_666
	   (p_ColorApprox->type == PEXColorSpace) &&
	   (p_ColorApprox->model == PEXColorApproxRGB) &&
	   (p_ColorApprox->max1 == 7) &&
	   (p_ColorApprox->max2 == 7) &&
	   (p_ColorApprox->max3 == 3) &&
	   (p_ColorApprox->dither == PEXOn) &&
	   (p_ColorApprox->mult1 == 32) &&
	   (p_ColorApprox->mult2 == 4) &&
	   (p_ColorApprox->mult3 == 1) &&
	   (p_ColorApprox->weight1 == 0.0) &&
	   (p_ColorApprox->weight2 == 0.0) &&
	   (p_ColorApprox->weight3 == 0.0) &&
	   (p_ColorApprox->base_pixel == 0) ))
#else
           (p_ColorApprox->type == PEXColorSpace) &&
           (p_ColorApprox->model == PEXColorApproxRGB) &&
           (p_ColorApprox->max1 == 5) &&
           (p_ColorApprox->max2 == 5) &&
           (p_ColorApprox->max3 == 5) &&
           (p_ColorApprox->dither == PEXOn) &&
           (p_ColorApprox->mult1 == 36) &&
           (p_ColorApprox->mult2 == 6) &&
           (p_ColorApprox->mult3 == 1) &&
           (p_ColorApprox->weight1 == 0.0) &&
           (p_ColorApprox->weight2 == 0.0) &&
           (p_ColorApprox->weight3 == 0.0) &&
           (p_ColorApprox->base_pixel == 40) ))
#endif
    {
	_hppex_stderr_print("Predefined Color Approx entry is incorrect\n");
    }


    /*
     * Free the entries we collected
     */

    for (i=1; i<NUM_ENTRIES; i++)
    {
	if (i == PEXLUTColor)
	    PEXFreeTableEntries (i, 8, &entries[i]);
	else if (i != PEXLUTPattern && !pattern_lut_supported)
	    PEXFreeTableEntries (i, 1, &entries[i]);
    }
#endif  /* _hpux_source */

    /*************
     * STEP 3
     ************/

    /*
     * Create one of each kind of LUT. Pattern should generate an error.
     */

    luts[PEXLUTLineBundle] = 
		PEXCreateLookupTable (display, window, PEXLUTLineBundle);

    luts[PEXLUTMarkerBundle] = 
		PEXCreateLookupTable (display, window, PEXLUTMarkerBundle);

    luts[PEXLUTTextBundle] = 
		PEXCreateLookupTable (display, window, PEXLUTTextBundle);

    luts[PEXLUTInteriorBundle] = 
		PEXCreateLookupTable (display, window, PEXLUTInteriorBundle);

    luts[PEXLUTEdgeBundle] = 
		PEXCreateLookupTable (display, window, PEXLUTEdgeBundle);

    _hppex_test_set_expected_error (PEX_ERR, BadPEXLookupTable,
                                        PEXRCCreateLookupTable, 0, 0);

    luts[PEXLUTPattern] = 
		PEXCreateLookupTable (display, window, PEXLUTPattern);

    XSync(display, False);
    _hppex_test_check_expected_error (err_mask);

    luts[PEXLUTTextFont] = 
		PEXCreateLookupTable (display, window, PEXLUTTextFont);

    luts[PEXLUTColor] = 
		PEXCreateLookupTable (display, window, PEXLUTColor);

    luts[PEXLUTView] = 
		PEXCreateLookupTable (display, window, PEXLUTView);

    luts[PEXLUTLight] = 
		PEXCreateLookupTable (display, window, PEXLUTLight);

    luts[PEXLUTDepthCue] = 
		PEXCreateLookupTable (display, window, PEXLUTDepthCue);

    luts[PEXLUTColorApprox] = 
		PEXCreateLookupTable (display, window, PEXLUTColorApprox);


    /*
     * Check that luts were created correctly.
     */

    for (i=1; i<NUM_ENTRIES; i++)
    {
	if (i != PEXLUTPattern && !pattern_lut_supported)
	{
	char *protocol_mode;
            /* 
             * In HP DHA mode, a NULL id is an indicator of an error. 
             * In protocol mode, id's are created by XAllocID, so this
             * check is not valid in protocol mode.
             */

            protocol_mode = getenv("HPPEX_CLIENT_PROTOCOL");
            if (strcmp("PEX", protocol_mode) != 0) 
            {
	        if (luts[i] == 0)
	    	   _hppex_stderr_print("Create of lut %d  did not return RID \n", i);
            }
	}
    }

    /*************
     * STEP 4
     ************/

    /*
     * Get the defined indices and see if they are as expected.
     */

    status = PEXGetDefinedIndices (display, luts[PEXLUTLineBundle], 
				   &return_count, &return_indices);

    if ( !((return_count == 1) &&
	   (return_indices[0] == 1) &&
	   (status) ))
    {
	_hppex_stderr_print("Defined LineBundle indices incorrect\n");
    }

    XFree (return_indices);


    status = PEXGetDefinedIndices (display, luts[PEXLUTMarkerBundle], 
				   &return_count, &return_indices);

    if ( !((return_count == 1) &&
	   (return_indices[0] == 1) &&
	   (status) ))
    {
	_hppex_stderr_print("Defined MarkerBundle indices incorrect\n");
    }

    XFree (return_indices);



    status = PEXGetDefinedIndices (display, luts[PEXLUTTextBundle], 
				   &return_count, &return_indices);

    if ( !((return_count == 1) &&
	   (return_indices[0] == 1) &&
	   (status) ))
    {
	_hppex_stderr_print("Defined TextBundle indices incorrect\n");
    }

    XFree (return_indices);



    status = PEXGetDefinedIndices (display, luts[PEXLUTInteriorBundle], 
				   &return_count, &return_indices);

    if ( !((return_count == 1) &&
	   (return_indices[0] == 1) &&
	   (status) ))
    {
	_hppex_stderr_print("Defined InteriorBundle indices incorrect\n");
    }

    XFree (return_indices);



    status = PEXGetDefinedIndices (display, luts[PEXLUTEdgeBundle], 
				   &return_count, &return_indices);

    if ( !((return_count == 1) &&
	   (return_indices[0] == 1) &&
	   (status) ))
    {
	_hppex_stderr_print("Defined EdgeBundle indices incorrect\n");
    }

    XFree (return_indices);



    status = PEXGetDefinedIndices (display, luts[PEXLUTTextFont], 
				   &return_count, &return_indices);

    if ( !((return_count == 1) &&
	   (return_indices[0] == 1) &&
	   (status) ))
    {
	_hppex_stderr_print("Defined TextFont indices incorrect\n");
    }

    XFree (return_indices);



    status = PEXGetDefinedIndices (display, luts[PEXLUTColor], 
				   &return_count, &return_indices);

    if ( !((return_count == 8) &&
	   (return_indices[0] == 0) &&
	   (return_indices[1] == 1) &&
	   (return_indices[2] == 2) &&
	   (return_indices[3] == 3) &&
	   (return_indices[4] == 4) &&
	   (return_indices[5] == 5) &&
	   (return_indices[6] == 6) &&
	   (return_indices[7] == 7) &&
	   (status) ))
    {
	_hppex_stderr_print("Defined Color indices incorrect\n");
    }

    XFree (return_indices);



    status = PEXGetDefinedIndices (display, luts[PEXLUTView], 
				   &return_count, &return_indices);

    if ( !((return_count == 1) &&
	   (return_indices[0] == 0) &&
	   (status) ))
    {
	_hppex_stderr_print("Defined View indices incorrect\n");
    }

    XFree (return_indices);



    status = PEXGetDefinedIndices (display, luts[PEXLUTLight], 
				   &return_count, &return_indices);

    if ( !((return_count == 1) &&
	   (return_indices[0] == 1) &&
	   (status) ))
    {
	_hppex_stderr_print("Defined Light indices incorrect\n");
    }

    XFree (return_indices);



    status = PEXGetDefinedIndices (display, luts[PEXLUTDepthCue], 
				   &return_count, &return_indices);

    if ( !((return_count == 1) &&
	   (return_indices[0] == 0) &&
	   (status) ))
    {
	_hppex_stderr_print("Defined DepthCue indices incorrect\n");
    }

    XFree (return_indices);



    status = PEXGetDefinedIndices (display, luts[PEXLUTColorApprox], 
				   &return_count, &return_indices);

    if ( !((return_count == 1) &&
	   (return_indices[0] == 0) &&
	   (status) ))
    {
	_hppex_stderr_print("Defined ColorApprox indices incorrect\n");
    }

    XFree (return_indices);



    /*
     * Get Predefined entries from each lut and see if they are correct.
     * These are real entries in the luts instead of static data.
     * Again Pattern should fail.
     */

    for (i=1; i<NUM_ENTRIES; i++)
    {
	if (i == PEXLUTPattern && !pattern_lut_supported)
	    _hppex_test_set_expected_error (PEX_ERR, BadPEXLookupTable,
                                        PEXRCGetTableEntries, 0, 0);

	status = PEXGetTableEntries (display, luts[i],
					  lut_info[i].predefined_min,
					  lut_info[i].predefined_count,
					  PEXRealizedValue,
					  &table_type,
					  &entries[i]);
        XSync(display, False);

	if (i == PEXLUTPattern && !pattern_lut_supported) 
        {
            XSync(display, 0);
	    _hppex_test_check_expected_error (err_mask);
        }

	if (status)
        {
	    if (i == PEXLUTPattern && !pattern_lut_supported)
	        _hppex_stderr_print("Get Entries on Pattern did not fail as expected\n");
	    if (i != table_type)
	        _hppex_stderr_print("Get Entries returned wrong table type\n");
	}
	else
	{
	    if (i != PEXLUTPattern && !pattern_lut_supported)
	        _hppex_stderr_print("Get Entries  failed on lut %d\n", i);
	}
	
    }
    
    /* 
     * Examine the entries for expected values. This is a "hard coded" check.
     * This code will have to be updated to match changes in the contents 
     * of predefined entries.
     */

    p_LineBundle = (PEXLineBundleEntry *) entries [PEXLUTLineBundle];

    if ( !((p_LineBundle->type == PEXLineTypeSolid) &&
	   (p_LineBundle->interp_method == PEXPolylineInterpNone) &&
	   (p_LineBundle->curve_approx.method == PEXApproxImpDep) &&
	   (p_LineBundle->curve_approx.tolerance == 1.0) &&
	   (p_LineBundle->width == 1.0) &&
	   (p_LineBundle->color.type == PEXColorTypeIndexed) &&
	   (p_LineBundle->color.value.indexed.index == 1)))
    {
	_hppex_stderr_print("Initial Line Bundle entry is incorrect\n");
    }


    p_MarkerBundle = (PEXMarkerBundleEntry *) entries [PEXLUTMarkerBundle];

    if ( !((p_MarkerBundle->type == PEXMarkerAsterisk) &&
	   (p_MarkerBundle->scale == 1.0) &&
	   (p_MarkerBundle->color.type == PEXColorTypeIndexed) &&
	   (p_MarkerBundle->color.value.indexed.index == 1)))
    {
	_hppex_stderr_print("Initial Marker Bundle entry is incorrect\n");
    }


    p_TextBundle = (PEXTextBundleEntry *) entries [PEXLUTTextBundle];

    if ( !((p_TextBundle->font_index == 1) &&
	   (p_TextBundle->precision == PEXStrokePrecision) &&
	   (p_TextBundle->char_expansion == 1.0) &&
	   (p_TextBundle->char_spacing == 0.0) &&
	   (p_TextBundle->color.type == PEXColorTypeIndexed) &&
	   (p_TextBundle->color.value.indexed.index == 1)))
    {
	_hppex_stderr_print("Initial Text Bundle entry is incorrect\n");
    }

    p_InteriorBundle = (PEXInteriorBundleEntry *) entries[PEXLUTInteriorBundle];

    if ( !((p_InteriorBundle->style == PEXInteriorStyleHollow) &&
	   (p_InteriorBundle->style_index == 0) &&
	   (p_InteriorBundle->reflection_model == PEXReflectionNone) &&
	   (p_InteriorBundle->interp_method == PEXSurfaceInterpNone) &&
	   (p_InteriorBundle->bf_style == PEXInteriorStyleHollow) &&
	   (p_InteriorBundle->bf_style_index == 0) &&
	   (p_InteriorBundle->bf_reflection_model == PEXReflectionNone) &&
	   (p_InteriorBundle->bf_interp_method == PEXSurfaceInterpNone) &&
	   (p_InteriorBundle->surface_approx.method == PEXApproxImpDep) &&
	   (p_InteriorBundle->surface_approx.u_tolerance == 1.0) &&
	   (p_InteriorBundle->surface_approx.v_tolerance == 1.0) &&
	   (p_InteriorBundle->color.type == PEXColorTypeIndexed) &&
	   (p_InteriorBundle->color.value.indexed.index == 1) &&
	   (p_InteriorBundle->reflection_attr.ambient == 1.0) &&
	   (p_InteriorBundle->reflection_attr.diffuse == 1.0) &&
	   (p_InteriorBundle->reflection_attr.specular == 1.0) &&
	   (p_InteriorBundle->reflection_attr.specular_conc == 0.0) &&
	   (p_InteriorBundle->reflection_attr.transmission == 0.0) &&
	   (p_InteriorBundle->reflection_attr.specular_color.type 
						== PEXColorTypeIndexed) &&
	   (p_InteriorBundle->reflection_attr.specular_color.value.indexed.index								 == 1 ) &&
	   (p_InteriorBundle->bf_color.type == PEXColorTypeIndexed) &&
	   (p_InteriorBundle->bf_color.value.indexed.index == 1) &&
	   (p_InteriorBundle->bf_reflection_attr.ambient == 1.0) &&
	   (p_InteriorBundle->bf_reflection_attr.diffuse == 1.0) &&
	   (p_InteriorBundle->bf_reflection_attr.specular == 1.0) &&
	   (p_InteriorBundle->bf_reflection_attr.specular_conc == 0.0) &&
	   (p_InteriorBundle->bf_reflection_attr.transmission == 0.0) &&
	   (p_InteriorBundle->bf_reflection_attr.specular_color.type 
						== PEXColorTypeIndexed) &&
        (p_InteriorBundle->bf_reflection_attr.specular_color.value.indexed.index
								== 1) ))
    {
	_hppex_stderr_print("Initial Interior Bundle entry is incorrect\n");
    }


    p_EdgeBundle = (PEXEdgeBundleEntry *) entries [PEXLUTEdgeBundle];

    if ( !((p_EdgeBundle->edge_flag == PEXOff) &&
	   (p_EdgeBundle->type == PEXSurfaceEdgeSolid) &&
	   (p_EdgeBundle->width == 1.0) &&
	   (p_EdgeBundle->color.type == PEXColorTypeIndexed) &&
	   (p_EdgeBundle->color.value.indexed.index == 1)))
    {
	_hppex_stderr_print("Initial Edge Bundle entry is incorrect\n");
    }

    /*
     * Won't check pattern cause it should have failed above.
     */


    p_TextFont = (PEXTextFontEntry *) entries[PEXLUTTextFont];

    /*
     * Font module should be checking that font is valid. We'll just check the
     * count.
     */
    if ( !((p_TextFont->count == 1) ))
    {
	_hppex_stderr_print("Initial TextFont entry is incorrect\n");
    }


    /* 
     * There are 8 color entries to check out
     */
    p_Color = (PEXColorEntry *) entries [PEXLUTColor];

    /* Black */
    if ( !((p_Color->type == PEXColorTypeRGB) &&
	   (p_Color->value.rgb.red == 0.0) &&
	   (p_Color->value.rgb.green == 0.0) &&
	   (p_Color->value.rgb.blue == 0.0) ))
    {
	_hppex_stderr_print("Initial Color entry 0 (black) is incorrect\n");
    }

    p_Color++;

    /* White */
    if ( !((p_Color->type == PEXColorTypeRGB) &&
	   (p_Color->value.rgb.red == 1.0) &&
	   (p_Color->value.rgb.green == 1.0) &&
	   (p_Color->value.rgb.blue == 1.0) ))
    {
	_hppex_stderr_print("Initial Color entry 1 (white) is incorrect\n");
    }

    p_Color++;

    /* Red */
    if ( !((p_Color->type == PEXColorTypeRGB) &&
	   (p_Color->value.rgb.red == 1.0) &&
	   (p_Color->value.rgb.green == 0.0) &&
	   (p_Color->value.rgb.blue == 0.0) ))
    {
	_hppex_stderr_print("Initial Color entry 2 (red) is incorrect\n");
    }

    p_Color++;

    /* Yellow */
    if ( !((p_Color->type == PEXColorTypeRGB) &&
	   (p_Color->value.rgb.red == 1.0) &&
	   (p_Color->value.rgb.green == 1.0) &&
	   (p_Color->value.rgb.blue == 0.0) ))
    {
	_hppex_stderr_print("Initial Color entry 3 (yellow) is incorrect\n");
    }

    p_Color++;

    /* Green */
    if ( !((p_Color->type == PEXColorTypeRGB) &&
	   (p_Color->value.rgb.red == 0.0) &&
	   (p_Color->value.rgb.green == 1.0) &&
	   (p_Color->value.rgb.blue == 0.0) ))
    {
	_hppex_stderr_print("Initial Color entry 4 (green) is incorrect\n");
    }

    p_Color++;

    /* Cyan */
    if ( !((p_Color->type == PEXColorTypeRGB) &&
	   (p_Color->value.rgb.red == 0.0) &&
	   (p_Color->value.rgb.green == 1.0) &&
	   (p_Color->value.rgb.blue == 1.0) ))
    {
	_hppex_stderr_print("Initial Color entry 5 (cyan) is incorrect\n");
    }

    p_Color++;

    /* Blue */
    if ( !((p_Color->type == PEXColorTypeRGB) &&
	   (p_Color->value.rgb.red == 0.0) &&
	   (p_Color->value.rgb.green == 0.0) &&
	   (p_Color->value.rgb.blue == 1.0) ))
    {
	_hppex_stderr_print("Initial Color entry 6 (blue) is incorrect\n");
    }

    p_Color++;

    /* Magenta */
    if ( !((p_Color->type == PEXColorTypeRGB) &&
	   (p_Color->value.rgb.red == 1.0) &&
	   (p_Color->value.rgb.green == 0.0) &&
	   (p_Color->value.rgb.blue == 1.0) ))
    {
	_hppex_stderr_print("Initial Color entry 7 (magenta) is incorrect\n");
    }

    
    p_View = (PEXViewEntry *) entries [PEXLUTView];

    for (i=0; i<4; i++)
	for (j=0; j<4; j++)
	{
	    if (i == j)
	    {
		if (p_View->orientation[i][j] != 1.0) orientation_okay = False;
		if (p_View->mapping[i][j] != 1.0) mapping_okay = False;
	    }
	    else
	    {
		if (p_View->orientation[i][j] != 0.0) orientation_okay = False;
		if (p_View->mapping[i][j] != 0.0) mapping_okay = False;
	    }
	}

    if ( !((p_View->clip_flags == PEXClippingAll) &&
	   (p_View->clip_limits.min.x == 0.0) &&
	   (p_View->clip_limits.min.y == 0.0) &&
	   (p_View->clip_limits.min.z == 0.0) &&
	   (p_View->clip_limits.max.x == 1.0) &&
	   (p_View->clip_limits.max.y == 1.0) &&
	   (p_View->clip_limits.max.z == 1.0) &&
	   (orientation_okay ) &&
	   (mapping_okay ) ))
    {
	_hppex_stderr_print("Initial View Bundle entry is incorrect\n");
    }

    
    p_Light = (PEXLightEntry *) entries [PEXLUTLight];

    if ( !((p_Light->type == PEXLightAmbient) &&
	   (p_Light->direction.x == 0.0) &&
	   (p_Light->direction.y == 0.0) &&
	   (p_Light->direction.z == 0.0) &&
	   (p_Light->point.x== 0.0) &&
	   (p_Light->point.y== 0.0) &&
	   (p_Light->point.z== 0.0) &&
	   (p_Light->concentration == 0.0) &&
	   (p_Light->spread_angle == 0.0) &&
	   (p_Light->attenuation1 == 0.0) &&
	   (p_Light->attenuation2 == 0.0) &&
	   (p_Light->color.type == PEXColorTypeRGB) &&
	   (p_Light->color.value.rgb.red == 1.0) &&
	   (p_Light->color.value.rgb.green == 1.0) &&
	   (p_Light->color.value.rgb.blue == 1.0)))
    {
	_hppex_stderr_print("Initial Light entry is incorrect\n");
    }


    p_DepthCue = (PEXDepthCueEntry *) entries [PEXLUTDepthCue];

    if ( !((p_DepthCue->mode == PEXOff) &&
	   (p_DepthCue->front_plane == 1.0) &&
	   (p_DepthCue->back_plane == 0.0) &&
	   (p_DepthCue->front_scaling == 1.0) &&
	   (p_DepthCue->back_scaling == 0.5) &&
	   (p_DepthCue->color.type == PEXColorTypeIndexed) &&
	   (p_DepthCue->color.value.indexed.index == 0)))
    {
	_hppex_stderr_print("Initial View entry is incorrect\n");
    }


    p_ColorApprox = (PEXColorApproxEntry *) entries [PEXLUTColorApprox];

    if ( !(
#ifndef CMAP_666
   	   (p_ColorApprox->type == PEXColorSpace) &&
	   (p_ColorApprox->model == PEXColorApproxRGB) &&
	   (p_ColorApprox->max1 == 7) &&
	   (p_ColorApprox->max2 == 7) &&
	   (p_ColorApprox->max3 == 3) &&
	   (p_ColorApprox->dither == PEXOn) &&
	   (p_ColorApprox->mult1 == 32) &&
	   (p_ColorApprox->mult2 == 4) &&
	   (p_ColorApprox->mult3 == 1) &&
	   (p_ColorApprox->weight1 == 0.0) &&
	   (p_ColorApprox->weight2 == 0.0) &&
	   (p_ColorApprox->weight3 == 0.0) &&
	   (p_ColorApprox->base_pixel == 0) ))
#else
           (p_ColorApprox->type == PEXColorSpace) &&
           (p_ColorApprox->model == PEXColorApproxRGB) &&
           (p_ColorApprox->max1 == 5) &&
           (p_ColorApprox->max2 == 5) &&
           (p_ColorApprox->max3 == 5) &&
           (p_ColorApprox->dither == PEXOn) &&
           (p_ColorApprox->mult1 == 36) &&
           (p_ColorApprox->mult2 == 6) &&
           (p_ColorApprox->mult3 == 1) &&
           (p_ColorApprox->weight1 == 0.0) &&
           (p_ColorApprox->weight2 == 0.0) &&
           (p_ColorApprox->weight3 == 0.0) &&
           (p_ColorApprox->base_pixel == 40) ))
#endif
    {
	_hppex_stderr_print("Initial Color Approx entry is incorrect\n");
    }


    /*
     * Free the entries we collected
     */

    for (i=1; i<NUM_ENTRIES; i++)
    {
	if (i == PEXLUTColor)
	    PEXFreeTableEntries (i, 8, &entries[i]);
	else if (i != PEXLUTPattern && !pattern_lut_supported)
	    PEXFreeTableEntries (i, 1, &entries[i]);
    }


    /*************
     * STEP 5
     ************/

    /*
     * Free the lookup tables
     */

    for (i=1; i<NUM_ENTRIES; i++)
    {
	if (i != PEXLUTPattern && !pattern_lut_supported)
	    PEXFreeLookupTable (display, luts[i]);
    }


    /**********************************************************************
     * Test ends here
     *********************************************************************/

    XSync (display, 0);
}

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "lut_startup");
    *num_images      = 1;
    *supported_modes = NON_OC_MODE;

    strcpy(win_title, "lut_startup");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    local_window = window;
    _hppex_set_stderr("lut_startup.stderr", "lut_startup");
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    describe_test( "\nPEXlib Lookup Table Startup Tests:\n\n" );
    describe_test( " Test for correct baseline functionality of lut module.\n");
    describe_test( " This test can be built with a malloc checker to verify that all allocated\n");
    describe_test( " memory is freed. This test will generate errors wherever a pattern table is\n");
    describe_test( " operated on.\n");
    describe_test( " This test has 5 basic steps:\n");
    describe_test( " 	1. Get Table info and verify the correctness of that info.\n");
    describe_test( "	2. Get predefined entries for luts and verify correctness.(HP Only)\n");
    describe_test( "      3. Create Luts verifying luts are indeed created as expected.\n");
    describe_test( "	4. Check the defined indices on the newly created luts to see\n");
    describe_test( "	   if they are as expected from the predefined entries.\n");
    describe_test( "	5. Delete all the LUTs\n\n");
    describe_test( " The test is successful if no messages are printed to stderr.\n\n");


    lut_test(dpy, local_window);
    _hppex_close_stderr();
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
    file_testproc("lut_startup.stderr", "lut_startup");
    }
