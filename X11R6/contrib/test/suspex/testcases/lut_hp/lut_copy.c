/* $XConsortium: lut_copy.c,v 1.0 93/11/22 12:28:28 rws Exp $ */

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

#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/PEX5/PEXlib.h>
#include <misc.h>

#define NUM_LUTS 12
#define NUM_ENTRIES (NUM_LUTS + 1)

Window local_window;

void lut_test(Display *display,
	      Window   window)
{
    /*************************************************************************
     * Test specific data
     ************************************************************************/
    PEXLookupTable src_luts[NUM_ENTRIES]; /* 12 lut types, 0'th entry ignored */
    PEXLookupTable dest_luts[NUM_ENTRIES];/* 12 lut types, 0'th entry ignored */
    PEXPointer entries [NUM_ENTRIES];     /* 12 lut types, 0'th entry ignored */

    int i, j;
    int table_type, entry_type;
    Status status;
    unsigned long return_count;
    PEXTableIndex *return_indices;
    unsigned long num_fonts;
    char **font_names;
    PEXFont font_id1, font_id2, *p_font;
     

    PEXLineBundleEntry          *p_LineBundle, *p_LineBundle_tmp;
    PEXMarkerBundleEntry        *p_MarkerBundle, *p_MarkerBundle_tmp;
    PEXTextBundleEntry          *p_TextBundle, *p_TextBundle_tmp;
    PEXInteriorBundleEntry      *p_InteriorBundle, *p_InteriorBundle_tmp;
    PEXEdgeBundleEntry          *p_EdgeBundle, *p_EdgeBundle_tmp;
    PEXColorEntry               *p_Color, *p_Color_tmp;
    PEXTextFontEntry            *p_TextFont, *p_TextFont_tmp;
    PEXLightEntry               *p_Light, *p_Light_tmp;
    PEXDepthCueEntry            *p_DepthCue, *p_DepthCue_tmp;
    PEXColorApproxEntry         *p_ColorApprox, *p_ColorApprox_tmp;
    PEXViewEntry                *p_View, *p_View_tmp;

    /**********************************************************************
     * Test begins here
     *********************************************************************/

    /*
     * Create a full set of LUTS to play with during this test.
     * No error checking here since other tests will check this out.
     */

    for (i=1; i<NUM_ENTRIES; i++)
    {
	if (i != PEXLUTPattern)
	{
	    src_luts[i] = PEXCreateLookupTable (display, window, i);
	    dest_luts[i] = PEXCreateLookupTable (display, window, i);
	}
    }


    /*
     *Try LineBundle copy
     */


    /*
     * Allocate a buffer for setting entries
     */
    p_LineBundle = (PEXLineBundleEntry *) malloc(3*sizeof(PEXLineBundleEntry));
    p_LineBundle_tmp = p_LineBundle;
    
    /*
     * Fill out buffer
     */
    p_LineBundle_tmp->type = PEXLineTypeDashed;
    p_LineBundle_tmp->interp_method = PEXPolylineInterpNone;
    p_LineBundle_tmp->curve_approx.method = PEXApproxImpDep;
    p_LineBundle_tmp->curve_approx.tolerance = 1.0;
    p_LineBundle_tmp->width = 1.0;
    p_LineBundle_tmp->color.type = PEXColorTypeIndexed;
    p_LineBundle_tmp->color.value.indexed.index = 1;

    p_LineBundle_tmp++;
    p_LineBundle_tmp->type = PEXLineTypeSolid;
    p_LineBundle_tmp->interp_method = PEXPolylineInterpNone;
    p_LineBundle_tmp->curve_approx.method = PEXApproxImpDep;
    p_LineBundle_tmp->curve_approx.tolerance = 1.0;
    p_LineBundle_tmp->width = 2.0;
    p_LineBundle_tmp->color.type = PEXColorTypeIndexed;
    p_LineBundle_tmp->color.value.indexed.index = 2;

    p_LineBundle_tmp++;
    p_LineBundle_tmp->type = PEXLineTypeDotted;
    p_LineBundle_tmp->interp_method = PEXPolylineInterpNone;
    p_LineBundle_tmp->curve_approx.method = PEXApproxImpDep;
    p_LineBundle_tmp->curve_approx.tolerance = 1.0;
    p_LineBundle_tmp->width = 3.0;
    p_LineBundle_tmp->color.type = PEXColorTypeIndexed;
    p_LineBundle_tmp->color.value.indexed.index = 3;

    /*
     * Set source entries sparsely
     */
    PEXSetTableEntries (display, src_luts[PEXLUTLineBundle], 511, 3, 
			PEXLUTLineBundle, p_LineBundle);
     
    PEXSetTableEntries (display, src_luts[PEXLUTLineBundle], 619, 2, 
			PEXLUTLineBundle, p_LineBundle);

    PEXSetTableEntries (display, src_luts[PEXLUTLineBundle], 65530, 2, 
			PEXLUTLineBundle, p_LineBundle);

    /*
     * Set source entries sparsely
     */
    PEXSetTableEntries (display, dest_luts[PEXLUTLineBundle], 813, 2, 
			PEXLUTLineBundle, p_LineBundle);
     
    PEXSetTableEntries (display, dest_luts[PEXLUTLineBundle], 4000, 3, 
			PEXLUTLineBundle, p_LineBundle);

    PEXSetTableEntries (display, dest_luts[PEXLUTLineBundle], 62530, 2, 
			PEXLUTLineBundle, p_LineBundle);

    /*
     * Copy src lut to dest
     */
    PEXCopyLookupTable (display, src_luts[PEXLUTLineBundle], 
			dest_luts[PEXLUTLineBundle]);

    /*
     * See if defined indices are what we expect.
     */
    status = PEXGetDefinedIndices (display, dest_luts[PEXLUTLineBundle], 
				   &return_count, &return_indices);

    if (! ( (status) &&
	    (return_count == 8) &&
	    (return_indices[0] == 1) &&
	    (return_indices[1] == 511) &&
	    (return_indices[2] == 512) &&
	    (return_indices[3] == 513) &&
	    (return_indices[4] == 619) &&
	    (return_indices[5] == 620) &&
	    (return_indices[6] == 65530) &&
	    (return_indices[7] == 65531)
	    ))
    {
	_hppex_stderr_print( "ERROR: destination Line Bundle topology not as expected\n");
    }

    XFree (return_indices);


    /*
     * Get an entry to see if it copied over correctly
     */

    p_LineBundle_tmp = (PEXLineBundleEntry *) PEXGetTableEntry (display, 
					 dest_luts[PEXLUTLineBundle], 
					 513, PEXSetValue, &entry_type, 
					 &table_type);

    if (!( (entry_type == PEXDefinedEntry) &&
	   (table_type == PEXLUTLineBundle) &&
    	   (p_LineBundle_tmp->type == PEXLineTypeDotted) &&
    	   (p_LineBundle_tmp->interp_method == PEXPolylineInterpNone) &&
    	   (p_LineBundle_tmp->curve_approx.method == PEXApproxImpDep) &&
    	   (p_LineBundle_tmp->curve_approx.tolerance == 1.0) &&
    	   (p_LineBundle_tmp->width == 3.0) &&
    	   (p_LineBundle_tmp->color.type == PEXColorTypeIndexed) &&
    	   (p_LineBundle_tmp->color.value.indexed.index == 3) ))
    {
	_hppex_stderr_print( "ERROR: Dest LineBundle entry is incorrect\n");
    }

    PEXFreeTableEntries (PEXLUTLineBundle, 1, p_LineBundle_tmp);


    /*
     * Check out Markers
     */


    /*
     * Set a couple entries
     */
    p_MarkerBundle = (PEXMarkerBundleEntry *) 
				malloc(2*sizeof(PEXMarkerBundleEntry));
    p_MarkerBundle_tmp = p_MarkerBundle;
    
    p_MarkerBundle_tmp->type = PEXMarkerDot;
    p_MarkerBundle_tmp->scale = 0.5;
    p_MarkerBundle_tmp->color.type = PEXColorTypeRGB;
    p_MarkerBundle_tmp->color.value.rgb.red = 1.0;
    p_MarkerBundle_tmp->color.value.rgb.green = 0.5;
    p_MarkerBundle_tmp->color.value.rgb.blue = 0.5;

    p_MarkerBundle_tmp++;
    p_MarkerBundle_tmp->type = PEXMarkerCross;
    p_MarkerBundle_tmp->scale = 0.25;
    p_MarkerBundle_tmp->color.type = PEXColorTypeIndexed;
    p_MarkerBundle_tmp->color.value.indexed.index = 2;
    
    /*
     * Set src entries sparsely
     */
    PEXSetTableEntries (display, src_luts[PEXLUTMarkerBundle], 1111, 2, 
			PEXLUTMarkerBundle, p_MarkerBundle);

    PEXSetTableEntries (display, src_luts[PEXLUTMarkerBundle], 811, 1, 
			PEXLUTMarkerBundle, p_MarkerBundle);

    /*
     * Set dest entries sparsely
     */
    PEXSetTableEntries (display, dest_luts[PEXLUTMarkerBundle], 1131, 2, 
			PEXLUTMarkerBundle, p_MarkerBundle);

    PEXSetTableEntries (display, dest_luts[PEXLUTMarkerBundle], 11, 1, 
			PEXLUTMarkerBundle, p_MarkerBundle);

    PEXSetTableEntries (display, dest_luts[PEXLUTMarkerBundle], 6100, 1, 
			PEXLUTMarkerBundle, p_MarkerBundle);

    free (p_MarkerBundle);
     
    /*
     * Copy src lut to dest
     */
    PEXCopyLookupTable (display, src_luts[PEXLUTMarkerBundle], 
			dest_luts[PEXLUTMarkerBundle]);

    /*
     * See if defined indices are what we expect.
     */
    status = PEXGetDefinedIndices (display, dest_luts[PEXLUTMarkerBundle], 
				   &return_count, &return_indices);

    if (! ( (status) &&
	    (return_count == 4) &&
	    (return_indices[0] == 1) &&
	    (return_indices[1] == 811) &&
	    (return_indices[2] == 1111) &&
	    (return_indices[3] == 1112) ))
    {
	_hppex_stderr_print( "ERROR: destination Marker Bundle topology not as expected\n");
    }

    XFree (return_indices);

    /*
     * Get an entry to see if it copied correctly
     */
    p_MarkerBundle_tmp = (PEXMarkerBundleEntry *) PEXGetTableEntry(display, 
					  dest_luts[PEXLUTMarkerBundle], 
					 1112, PEXRealizedValue, &entry_type, 
					 &table_type);

    if (!((p_MarkerBundle_tmp->type == PEXMarkerCross) &&
	  (p_MarkerBundle_tmp->scale == 1.0) &&
    	  (p_MarkerBundle_tmp->color.type == PEXColorTypeIndexed) &&
    	  (p_MarkerBundle_tmp->color.value.indexed.index = 2) &&
	  (entry_type == PEXDefinedEntry) &&
	  (table_type == PEXLUTMarkerBundle) ))
    {
	_hppex_stderr_print( "ERROR: Destination MarkerBundle entry has incorrect data\n");
    }

    PEXFreeTableEntries (PEXLUTMarkerBundle, 1, p_MarkerBundle_tmp);


    /*
     * Check out Text
     */


    /*
     * Set a couple entries
     */
    p_TextBundle = (PEXTextBundleEntry *) 
				malloc(2*sizeof(PEXTextBundleEntry));
    p_TextBundle_tmp = p_TextBundle;
    
    p_TextBundle_tmp->font_index = 1;
    p_TextBundle_tmp->precision = PEXStrokePrecision;
    p_TextBundle_tmp->char_expansion = 1.0;
    p_TextBundle_tmp->char_spacing = 0.5;
    p_TextBundle_tmp->color.type = PEXColorTypeRGB;
    p_TextBundle_tmp->color.value.rgb.red = 1.0;
    p_TextBundle_tmp->color.value.rgb.green = 0.5;
    p_TextBundle_tmp->color.value.rgb.blue = 0.5;

    p_TextBundle_tmp++;
    p_TextBundle_tmp->font_index = 0;
    p_TextBundle_tmp->precision = PEXCharPrecision;
    p_TextBundle_tmp->char_expansion = 2.0;
    p_TextBundle_tmp->char_spacing = 0.25;
    p_TextBundle_tmp->color.type = PEXColorTypeIndexed;
    p_TextBundle_tmp->color.value.indexed.index = 2;
    
    /*
     * Set some src lut entries sparsely
     */
    PEXSetTableEntries (display, src_luts[PEXLUTTextBundle], 1023, 2, 
			PEXLUTTextBundle, p_TextBundle);

    PEXSetTableEntries (display, src_luts[PEXLUTTextBundle], 16023, 1, 
			PEXLUTTextBundle, p_TextBundle);

    /*
     * Set some dest lut entries sparsely
     */
    PEXSetTableEntries (display, dest_luts[PEXLUTTextBundle], 1024, 2, 
			PEXLUTTextBundle, p_TextBundle);

    PEXSetTableEntries (display, dest_luts[PEXLUTTextBundle], 6023, 1, 
			PEXLUTTextBundle, p_TextBundle);

    PEXSetTableEntries (display, dest_luts[PEXLUTTextBundle], 2316, 2, 
			PEXLUTTextBundle, p_TextBundle);

    free (p_TextBundle);
     
    /*
     * Copy src lut to dest
     */
    PEXCopyLookupTable (display, src_luts[PEXLUTTextBundle], 
			dest_luts[PEXLUTTextBundle]);

    /*
     * See if defined indices are what we expect.
     */
    status = PEXGetDefinedIndices (display, dest_luts[PEXLUTTextBundle], 
				   &return_count, &return_indices);

    if (! ( (status) &&
	    (return_count == 4) &&
	    (return_indices[0] == 1) &&
	    (return_indices[1] == 1023) &&
	    (return_indices[2] == 1024) &&
	    (return_indices[3] == 16023)))
    {
	_hppex_stderr_print( "ERROR: destination Text Bundle topology not as expected\n");
    }

    XFree (return_indices);

    /*
     * Get an entry to see if it copied correctly
     */
    p_TextBundle_tmp = (PEXTextBundleEntry *) PEXGetTableEntry(display, 
					 dest_luts[PEXLUTTextBundle], 
					 1024, PEXRealizedValue, &entry_type, 
					 &table_type);

    if (!((p_TextBundle_tmp->font_index == 0) &&
          (p_TextBundle_tmp->precision == PEXStrokePrecision) &&
          (p_TextBundle_tmp->char_expansion == 2.0) &&
          (p_TextBundle_tmp->char_spacing == 0.25) &&
          (p_TextBundle_tmp->color.type == PEXColorTypeIndexed) &&
          (p_TextBundle_tmp->color.value.indexed.index == 2) &&
	  (entry_type == PEXDefinedEntry) &&
	  (table_type == PEXLUTTextBundle) ))
    {
	_hppex_stderr_print( "ERROR: Text Bundle dest entry has incorrect data\n");
    }

    PEXFreeTableEntries (PEXLUTTextBundle, 1, p_TextBundle_tmp);



    /*
     * Check out Interior Bundle
     */


    /*
     * Set a couple entries
     */
    p_InteriorBundle = (PEXInteriorBundleEntry *) 
				malloc(2*sizeof(PEXInteriorBundleEntry));
    p_InteriorBundle_tmp = p_InteriorBundle;
    
    p_InteriorBundle_tmp->style = PEXInteriorStyleSolid;
    p_InteriorBundle_tmp->style_index = 2;
    p_InteriorBundle_tmp->reflection_model = PEXReflectionDiffuse;
    p_InteriorBundle_tmp->interp_method = PEXSurfaceInterpColor;
    p_InteriorBundle_tmp->bf_style = PEXInteriorStyleSolid;
    p_InteriorBundle_tmp->bf_style_index = 1;
    p_InteriorBundle_tmp->bf_reflection_model = PEXReflectionDiffuse;
    p_InteriorBundle_tmp->bf_interp_method = PEXSurfaceInterpNone;
    p_InteriorBundle_tmp->surface_approx.method = PEXApproxNPCRelative;
    p_InteriorBundle_tmp->surface_approx.u_tolerance = 1.0;
    p_InteriorBundle_tmp->surface_approx.v_tolerance = 0.5;
    p_InteriorBundle_tmp->color.type = PEXColorTypeRGB;
    p_InteriorBundle_tmp->color.value.rgb.red = 1.0;
    p_InteriorBundle_tmp->color.value.rgb.green = 0.5;
    p_InteriorBundle_tmp->color.value.rgb.blue = 0.5;
    p_InteriorBundle_tmp->reflection_attr.ambient = 1.5;
    p_InteriorBundle_tmp->reflection_attr.diffuse = 1.5;
    p_InteriorBundle_tmp->reflection_attr.specular = 1.25;
    p_InteriorBundle_tmp->reflection_attr.specular_conc = 65535.0;
    p_InteriorBundle_tmp->reflection_attr.transmission = 1.5;
    p_InteriorBundle_tmp->reflection_attr.specular_color.type = PEXColorTypeRGB;
    p_InteriorBundle_tmp->reflection_attr.specular_color.value.rgb.red = 1.0;
    p_InteriorBundle_tmp->reflection_attr.specular_color.value.rgb.green = 0.5;
    p_InteriorBundle_tmp->reflection_attr.specular_color.value.rgb.blue = 0.5;
    p_InteriorBundle_tmp->bf_color.type = PEXColorTypeIndexed;
    p_InteriorBundle_tmp->bf_color.value.indexed.index = 2;
    p_InteriorBundle_tmp->bf_reflection_attr.ambient = 1.5;
    p_InteriorBundle_tmp->bf_reflection_attr.diffuse = 1.25;
    p_InteriorBundle_tmp->bf_reflection_attr.specular = 1.5;
    p_InteriorBundle_tmp->bf_reflection_attr.specular_conc = 40000.0;
    p_InteriorBundle_tmp->bf_reflection_attr.transmission = 1.25;
    p_InteriorBundle_tmp->bf_reflection_attr.specular_color.type = PEXColorTypeRGB;
    p_InteriorBundle_tmp->bf_reflection_attr.specular_color.value.rgb.red = 1.0;
    p_InteriorBundle_tmp->bf_reflection_attr.specular_color.value.rgb.green = 0.5;
    p_InteriorBundle_tmp->bf_reflection_attr.specular_color.value.rgb.blue = 0.5;

    p_InteriorBundle_tmp++;
    p_InteriorBundle_tmp->style = PEXInteriorStyleSolid;
    p_InteriorBundle_tmp->style_index = 3;
    p_InteriorBundle_tmp->reflection_model = PEXReflectionSpecular;
    p_InteriorBundle_tmp->interp_method = PEXSurfaceInterpColor;
    p_InteriorBundle_tmp->bf_style = PEXInteriorStyleSolid;
    p_InteriorBundle_tmp->bf_style_index = 2;
    p_InteriorBundle_tmp->bf_reflection_model = PEXReflectionAmbient;
    p_InteriorBundle_tmp->bf_interp_method = PEXSurfaceInterpNone;
    p_InteriorBundle_tmp->surface_approx.method = PEXApproxWCRelative;
    p_InteriorBundle_tmp->surface_approx.u_tolerance = 1.0;
    p_InteriorBundle_tmp->surface_approx.v_tolerance = 0.5;
    p_InteriorBundle_tmp->color.type = PEXColorTypeIndexed;
    p_InteriorBundle_tmp->color.value.indexed.index = 2;
    p_InteriorBundle_tmp->reflection_attr.ambient = 0.5;
    p_InteriorBundle_tmp->reflection_attr.diffuse = 0.5;
    p_InteriorBundle_tmp->reflection_attr.specular = 0.25;
    p_InteriorBundle_tmp->reflection_attr.specular_conc = 10.0;
    p_InteriorBundle_tmp->reflection_attr.transmission = 0.5;
    p_InteriorBundle_tmp->reflection_attr.specular_color.type = PEXColorTypeRGB;
    p_InteriorBundle_tmp->reflection_attr.specular_color.value.rgb.red = 1.0;
    p_InteriorBundle_tmp->reflection_attr.specular_color.value.rgb.green = 0.5;
    p_InteriorBundle_tmp->reflection_attr.specular_color.value.rgb.blue = 0.5;
    p_InteriorBundle_tmp->bf_color.type = PEXColorTypeRGB;
    p_InteriorBundle_tmp->bf_color.value.rgb.red = 1.0;
    p_InteriorBundle_tmp->bf_color.value.rgb.green = 0.25;
    p_InteriorBundle_tmp->bf_color.value.rgb.blue = 0.5;
    p_InteriorBundle_tmp->bf_reflection_attr.ambient = 0.5;
    p_InteriorBundle_tmp->bf_reflection_attr.diffuse = 0.25;
    p_InteriorBundle_tmp->bf_reflection_attr.specular = 0.5;
    p_InteriorBundle_tmp->bf_reflection_attr.specular_conc = 100.0;
    p_InteriorBundle_tmp->bf_reflection_attr.transmission = 0.25;
    p_InteriorBundle_tmp->bf_reflection_attr.specular_color.type = PEXColorTypeRGB;
    p_InteriorBundle_tmp->bf_reflection_attr.specular_color.value.rgb.red = 1.0;
    p_InteriorBundle_tmp->bf_reflection_attr.specular_color.value.rgb.green = 0.5;
    p_InteriorBundle_tmp->bf_reflection_attr.specular_color.value.rgb.blue = 0.5;

    /*
     * Set sparse entries in the src
     */
    PEXSetTableEntries (display, src_luts[PEXLUTInteriorBundle], 255, 1, 
			PEXLUTInteriorBundle, p_InteriorBundle);

    PEXSetTableEntries (display, src_luts[PEXLUTInteriorBundle], 555, 2, 
			PEXLUTInteriorBundle, p_InteriorBundle);

    PEXSetTableEntries (display, src_luts[PEXLUTInteriorBundle], 17111, 2, 
			PEXLUTInteriorBundle, p_InteriorBundle);

    /*
     * Set sparse entries in the dest
     */
    PEXSetTableEntries (display, dest_luts[PEXLUTInteriorBundle], 1301, 1, 
			PEXLUTInteriorBundle, p_InteriorBundle);

    PEXSetTableEntries (display, dest_luts[PEXLUTInteriorBundle], 331, 2, 
			PEXLUTInteriorBundle, p_InteriorBundle);

    free (p_InteriorBundle);
     
    /*
     * Copy the src lut to the dest
     */
    PEXCopyLookupTable (display, src_luts[PEXLUTInteriorBundle],
			dest_luts[PEXLUTInteriorBundle]);

    /*
     * See if defined indices are what we expect.
     */
    status = PEXGetDefinedIndices (display, dest_luts[PEXLUTInteriorBundle], 
				   &return_count, &return_indices);

    if (! ( (status) &&
	    (return_count == 6) &&
	    (return_indices[0] == 1) &&
	    (return_indices[1] == 255) &&
	    (return_indices[2] == 555) &&
	    (return_indices[3] == 556) &&
	    (return_indices[4] == 17111) &&
	    (return_indices[5] == 17112)))
    {
	_hppex_stderr_print( "ERROR: destination Interior Bundle topology not as expected\n");
    }

    XFree (return_indices);

    /*
     * Get an entry to see if it copied correctly
     */
    p_InteriorBundle_tmp = (PEXInteriorBundleEntry *) PEXGetTableEntry(display, 
					 dest_luts[PEXLUTInteriorBundle],
					 555, PEXRealizedValue, &entry_type, 
					 &table_type);

    if (!((entry_type == PEXDefinedEntry) &&
	  (table_type == PEXLUTInteriorBundle) &&
	  (p_InteriorBundle_tmp->style == PEXInteriorStyleSolid) &&
          (p_InteriorBundle_tmp->style_index == 2) &&
          (p_InteriorBundle_tmp->reflection_model == PEXReflectionDiffuse) &&
          (p_InteriorBundle_tmp->interp_method == PEXSurfaceInterpColor) &&
          (p_InteriorBundle_tmp->bf_style == PEXInteriorStyleSolid) &&
          (p_InteriorBundle_tmp->bf_style_index == 1) &&
          (p_InteriorBundle_tmp->bf_reflection_model == PEXReflectionDiffuse) &&
          (p_InteriorBundle_tmp->bf_interp_method == PEXSurfaceInterpNone) &&
          (p_InteriorBundle_tmp->surface_approx.method == 
							PEXApproxNPCRelative) &&
          (p_InteriorBundle_tmp->surface_approx.u_tolerance == 1.0) &&
          (p_InteriorBundle_tmp->surface_approx.v_tolerance == 0.5) &&
          (p_InteriorBundle_tmp->color.type == PEXColorTypeRGB) &&
          (p_InteriorBundle_tmp->color.value.rgb.red == 1.0) &&
          (p_InteriorBundle_tmp->color.value.rgb.green == 0.5) &&
          (p_InteriorBundle_tmp->color.value.rgb.blue == 0.5) &&
          (p_InteriorBundle_tmp->reflection_attr.ambient == 1.0) &&
          (p_InteriorBundle_tmp->reflection_attr.diffuse == 1.0) &&
          (p_InteriorBundle_tmp->reflection_attr.specular == 1.0) &&
          (p_InteriorBundle_tmp->reflection_attr.specular_conc == 16383.0) &&
          (p_InteriorBundle_tmp->reflection_attr.transmission == 1.0) &&
          (p_InteriorBundle_tmp->reflection_attr.specular_color.type == 
							PEXColorTypeRGB) &&
          (p_InteriorBundle_tmp->reflection_attr.specular_color.value.rgb.red ==
								 1.0) &&
          (p_InteriorBundle_tmp->reflection_attr.specular_color.value.rgb.green
								  == 0.5) &&
          (p_InteriorBundle_tmp->reflection_attr.specular_color.value.rgb.blue 
								  == 0.5) &&
          (p_InteriorBundle_tmp->bf_color.type == PEXColorTypeIndexed) &&
          (p_InteriorBundle_tmp->bf_color.value.indexed.index == 2) &&
          (p_InteriorBundle_tmp->bf_reflection_attr.ambient == 1.0) &&
          (p_InteriorBundle_tmp->bf_reflection_attr.diffuse == 1.0) &&
          (p_InteriorBundle_tmp->bf_reflection_attr.specular == 1.0) &&
          (p_InteriorBundle_tmp->bf_reflection_attr.specular_conc == 16383.0) &&
          (p_InteriorBundle_tmp->bf_reflection_attr.transmission ==1.0) &&
          (p_InteriorBundle_tmp->bf_reflection_attr.specular_color.type == 
							PEXColorTypeRGB) &&
          (p_InteriorBundle_tmp->bf_reflection_attr.specular_color.value.rgb.red
								 == 1.0) &&
          (p_InteriorBundle_tmp->bf_reflection_attr.specular_color.value.rgb.green == 0.5) &&
          (p_InteriorBundle_tmp->bf_reflection_attr.specular_color.value.rgb.blue == 0.5) ))
    {
	_hppex_stderr_print( "ERROR: Dest Interior Bundle Entry has incorrect data\n");
    }

    PEXFreeTableEntries (PEXLUTInteriorBundle, 1, p_InteriorBundle_tmp);


    /*
     * Check out Edges
     */


    /*
     * Setup a couple entries
     */
    p_EdgeBundle = (PEXEdgeBundleEntry *) 
				malloc(2*sizeof(PEXEdgeBundleEntry));
    p_EdgeBundle_tmp = p_EdgeBundle;
    
    p_EdgeBundle_tmp->edge_flag = PEXOn;
    p_EdgeBundle_tmp->type = PEXSurfaceEdgeSolid;
    p_EdgeBundle_tmp->width = 0.5;
    p_EdgeBundle_tmp->color.type = PEXColorTypeRGB;
    p_EdgeBundle_tmp->color.value.rgb.red = 1.0;
    p_EdgeBundle_tmp->color.value.rgb.green = 0.5;
    p_EdgeBundle_tmp->color.value.rgb.blue = 0.5;

    p_EdgeBundle_tmp++;
    p_EdgeBundle_tmp->edge_flag = PEXOn;
    p_EdgeBundle_tmp->type = PEXSurfaceEdgeSolid;
    p_EdgeBundle_tmp->width = 1.0;
    p_EdgeBundle_tmp->color.type = PEXColorTypeIndexed;
    p_EdgeBundle_tmp->color.value.indexed.index = 2;

    /*
     * Set some sparse entries in the src lut
     */
    PEXSetTableEntries (display, src_luts[PEXLUTEdgeBundle], 1100, 2, 
			PEXLUTEdgeBundle, p_EdgeBundle);

    PEXSetTableEntries (display, src_luts[PEXLUTEdgeBundle], 801, 1, 
			PEXLUTEdgeBundle, p_EdgeBundle);


    /*
     * Set some sparse entries in the dest lut
     */
    PEXSetTableEntries (display, dest_luts[PEXLUTEdgeBundle], 11111, 2, 
			PEXLUTEdgeBundle, p_EdgeBundle);

    PEXSetTableEntries (display, dest_luts[PEXLUTEdgeBundle], 13, 2, 
			PEXLUTEdgeBundle, p_EdgeBundle);

    free (p_EdgeBundle);
     
    /*
     * Copy the src lut to dest
     */
    PEXCopyLookupTable (display, src_luts[PEXLUTEdgeBundle],
			dest_luts[PEXLUTEdgeBundle]);

    /*
     * See if defined indices are what we expect.
     */
    status = PEXGetDefinedIndices (display, dest_luts[PEXLUTEdgeBundle], 
				   &return_count, &return_indices);

    if (! ( (status) &&
	    (return_count == 4) &&
	    (return_indices[0] == 1) &&
	    (return_indices[1] == 801) &&
	    (return_indices[2] == 1100) &&
	    (return_indices[3] == 1101)))
    {
	_hppex_stderr_print( "ERROR: destination Edge Bundle topology not as expected\n");
    }

    XFree (return_indices);

    /*
     * Get an entry to see if it copied correctly
     */
    p_EdgeBundle_tmp = (PEXEdgeBundleEntry *) PEXGetTableEntry(display, 
					 dest_luts[PEXLUTEdgeBundle], 
					 1100, PEXRealizedValue, &entry_type, 
					 &table_type);

    if (!((entry_type == PEXDefinedEntry) &&
	  (table_type == PEXLUTEdgeBundle) &&
	  (p_EdgeBundle_tmp->edge_flag == PEXOn) &&
          (p_EdgeBundle_tmp->type == PEXSurfaceEdgeSolid) &&
          (p_EdgeBundle_tmp->width == 1.0) &&
          (p_EdgeBundle_tmp->color.type == PEXColorTypeRGB) &&
          (p_EdgeBundle_tmp->color.value.rgb.red == 1.0) &&
          (p_EdgeBundle_tmp->color.value.rgb.green == 0.5) &&
          (p_EdgeBundle_tmp->color.value.rgb.blue == 0.5) ))
    {
	_hppex_stderr_print( "ERROR: Dest Edge Bundle Entry has incorrect data\n");
    }

    PEXFreeTableEntries (PEXLUTEdgeBundle, 1, p_EdgeBundle_tmp);


    /*
     * Skip Pattern since it is unsupported.
     */

    /*
     * Check out fonts. 
     */

    /* Get a font */
    font_names = PEXListFonts (display, "*", 2, &num_fonts);
    font_id1 = PEXLoadFont (display, font_names[0]);
    font_id2 = PEXLoadFont (display, font_names[1]);
    PEXFreeFontNames (2, font_names);
   
    p_TextFont = (PEXTextFontEntry *) 
				malloc((2*sizeof(PEXTextFontEntry)) +
                                       (2*sizeof(PEXFont)));
    p_TextFont_tmp = p_TextFont;
    p_font = (PEXFont *) (p_TextFont + 2);

    p_TextFont_tmp->count = 1;
    *p_font = font_id1;
    p_TextFont_tmp->fonts = p_font;

    p_TextFont_tmp++; p_font++;
    p_TextFont_tmp->count = 1;
    *p_font = font_id2;
    p_TextFont_tmp->fonts = p_font;

    /*
     * Set some src sparse entries
     */
    PEXSetTableEntries (display, src_luts[PEXLUTTextFont], 255, 2, 
			PEXLUTTextFont, p_TextFont);

    PEXSetTableEntries (display, src_luts[PEXLUTTextFont], 12044, 1, 
			PEXLUTTextFont, p_TextFont);

    /*
     * Set some dest sparse entries
     */
    PEXSetTableEntries (display, dest_luts[PEXLUTTextFont], 900, 1, 
			PEXLUTTextFont, p_TextFont);

    PEXSetTableEntries (display, dest_luts[PEXLUTTextFont], 1801, 2, 
			PEXLUTTextFont, p_TextFont);

  
    /*
     * Do copy
     */
    PEXCopyLookupTable (display, src_luts[PEXLUTTextFont],
			dest_luts[PEXLUTTextFont]);

    /*
     * See if defined indices are what we expect.
     */
    status = PEXGetDefinedIndices (display, dest_luts[PEXLUTTextFont], 
				   &return_count, &return_indices);

    if (! ( (status) &&
	    (return_count == 4) &&
	    (return_indices[0] == 1) &&
	    (return_indices[1] == 255) &&
	    (return_indices[2] == 256) &&
	    (return_indices[3] == 12044) ))
    {
	_hppex_stderr_print( "ERROR: destination Text Font topology not as expected\n");
    }

    XFree (return_indices);

    /*
     * Get an entry to see if it copied correctly
     */

    p_TextFont_tmp = (PEXTextFontEntry *) PEXGetTableEntry(display, 
					 dest_luts[PEXLUTTextFont], 
					 12044, PEXSetValue, &entry_type, 
					 &table_type);

    if (!((entry_type == PEXDefinedEntry) &&
	  (table_type == PEXLUTTextFont) &&
          (p_TextFont_tmp->count == 1) &&
          (p_TextFont_tmp->fonts[0] == font_id1) )) 
    {
	_hppex_stderr_print( "ERROR: Dest Text Font entry is incorrect\n");
    }

    PEXFreeTableEntries (PEXLUTTextFont, 1, p_TextFont_tmp);


    /*
     * Check out color
     */

    /*
     * Set a couple entries
     */
    p_Color = (PEXColorEntry *) 
				malloc(2*sizeof(PEXColorEntry));
    p_Color_tmp = p_Color;
    
    p_Color_tmp->type = PEXColorTypeRGB;
    p_Color_tmp->value.rgb.red = 0.25;
    p_Color_tmp->value.rgb.green= 0.5;
    p_Color_tmp->value.rgb.blue = 0.25;

    p_Color_tmp++;
    p_Color_tmp->type = PEXColorTypeRGB;
    p_Color_tmp->value.rgb.red = 0.5;
    p_Color_tmp->value.rgb.green= 0.25;
    p_Color_tmp->value.rgb.blue = 1.0;

    /*
     * Set some sparse src entries
     */
    PEXSetTableEntries (display, src_luts[PEXLUTColor], 1400, 2, 
			PEXLUTColor, p_Color);

    PEXSetTableEntries (display, src_luts[PEXLUTColor], 2459, 1, 
			PEXLUTColor, p_Color);

    PEXSetTableEntries (display, src_luts[PEXLUTColor], 13000, 2, 
			PEXLUTColor, p_Color);

    /*
     * Set some sparse dest entries
     */
    PEXSetTableEntries (display, dest_luts[PEXLUTColor], 15000, 2, 
			PEXLUTColor, p_Color);

    PEXSetTableEntries (display, dest_luts[PEXLUTColor], 400, 2, 
			PEXLUTColor, p_Color);

    free (p_Color);
     

    /*
     * Copy src to dest
     */

    PEXCopyLookupTable (display, src_luts[PEXLUTColor],
			dest_luts[PEXLUTColor]);

    /*
     * See if defined indices are what we expect.
     */
    status = PEXGetDefinedIndices (display, dest_luts[PEXLUTColor], 
				   &return_count, &return_indices);

    if (! ( (status) &&
	    (return_count == 13) &&
	    (return_indices[0] == 0) &&
	    (return_indices[1] == 1) &&
	    (return_indices[2] == 2) &&
	    (return_indices[3] == 3) &&
	    (return_indices[4] == 4) &&
	    (return_indices[5] == 5) &&
	    (return_indices[6] == 6) &&
	    (return_indices[7] == 7) &&
	    (return_indices[8] == 1400) &&
	    (return_indices[9] == 1401) &&
	    (return_indices[10] == 2459) &&
	    (return_indices[11] == 13000) &&
	    (return_indices[12] == 13001) ))
    {
	_hppex_stderr_print( "ERROR: destination Color topology not as expected\n");
    }

    XFree (return_indices);

    /*
     * Get an entry to see if it copied correctly
     */

    p_Color_tmp = (PEXColorEntry *) PEXGetTableEntry(display, 
					dest_luts[PEXLUTColor], 
				 	1401, PEXRealizedValue, &entry_type, 
					 &table_type);

    if (!((entry_type == PEXDefinedEntry) &&
	  (table_type == PEXLUTColor) &&
          (p_Color_tmp->type == PEXColorTypeRGB) &&
          (p_Color_tmp->value.rgb.red == 0.5) &&
          (p_Color_tmp->value.rgb.green == 0.25) &&
          (p_Color_tmp->value.rgb.blue == 1.0) ))
    {
	_hppex_stderr_print( "ERROR: Dest Color Entry has incorrect data\n");
    }

    PEXFreeTableEntries (PEXLUTColor, 1, p_Color_tmp);


    /*
     * Check out View table
     */

    /*
     * Set a couple entries
     */
    p_View = (PEXViewEntry *) 
				malloc(2*sizeof(PEXViewEntry));
    p_View_tmp = p_View;
    
    p_View_tmp->clip_flags = (PEXClipXY | PEXClipBack);
    p_View_tmp->clip_limits.min.x = 0.25;
    p_View_tmp->clip_limits.min.y = 0.25;
    p_View_tmp->clip_limits.min.z = 0.25;
    p_View_tmp->clip_limits.max.x = 0.5;
    p_View_tmp->clip_limits.max.y = 0.5;
    p_View_tmp->clip_limits.max.z = 0.5;

    for (i=0; i<4; i++)
        for (j=0; j<4; j++)
        {
	    p_View_tmp->orientation[i][j] = 2.0;
	    p_View_tmp->mapping[i][j] = 1.0;
	}


    p_View_tmp++;
    p_View_tmp->clip_flags = (PEXClipXY | PEXClipFront);
    p_View_tmp->clip_limits.min.x = 0.0;
    p_View_tmp->clip_limits.min.y = 0.0;
    p_View_tmp->clip_limits.min.z = 0.0;
    p_View_tmp->clip_limits.max.x = 0.5;
    p_View_tmp->clip_limits.max.y = 0.5;
    p_View_tmp->clip_limits.max.z = 0.5;

    for (i=0; i<4; i++)
        for (j=0; j<4; j++)
        {
	    p_View_tmp->orientation[i][j] = 4.0;
	    p_View_tmp->mapping[i][j] = 1.0;
	}

    /*
     * Set a few entries in the source
     */
    PEXSetTableEntries (display, src_luts[PEXLUTView], 16000, 2, 
			PEXLUTView, p_View);

    /*
     * Let the dest just have the default entry (ie. do nothing)
     */

    free (p_View);
     
    /*
     * Copy src to dest
     */
    PEXCopyLookupTable (display, src_luts[PEXLUTView],
			dest_luts[PEXLUTView]);
    /*
     * See if defined indices are what we expect.
     */
    status = PEXGetDefinedIndices (display, dest_luts[PEXLUTView], 
				   &return_count, &return_indices);

    if (! ( (status) &&
	    (return_count == 3) &&
	    (return_indices[0] == 0) &&
	    (return_indices[1] == 16000) &&
	    (return_indices[2] == 16001) ))
    {
	_hppex_stderr_print( "ERROR: Dest View topology not as expected\n");
    }

    XFree (return_indices);

    /*
     * Get a destination entry to see if it copied correctly
     */
    p_View_tmp = (PEXViewEntry *) PEXGetTableEntry(display, 
					 dest_luts[PEXLUTView], 
					 16000, PEXRealizedValue, &entry_type, 
					 &table_type);

    if (!((entry_type == PEXDefinedEntry) &&
	  (table_type == PEXLUTView) &&
    	  (p_View_tmp->clip_flags == PEXClipXY | PEXClipBack) &&
	  (p_View_tmp->clip_limits.min.x == 0.25) &&
    	  (p_View_tmp->clip_limits.min.y == 0.25) &&
    	  (p_View_tmp->clip_limits.min.z == 0.25) &&
    	  (p_View_tmp->clip_limits.max.x == 0.5) &&
    	  (p_View_tmp->clip_limits.max.y == 0.5) &&
    	  (p_View_tmp->clip_limits.max.z == 0.5) ))
    {
	_hppex_stderr_print( "ERROR: Dest View Entry has incorrect data\n");
    }

    for (i=0; i<4; i++)
        for (j=0; j<4; j++)
        {
	    if (!((p_View_tmp->orientation[i][j] == 2.0) &&
	          (p_View_tmp->mapping[i][j] == 1.0) ))
	    {
	        _hppex_stderr_print( "ERROR: Dest View Entry arrays have incorrect data\n");
		break;
	    }
	}

    PEXFreeTableEntries (PEXLUTView, 1, p_View_tmp);

    /*
     * Time to try out some Light table entries
     */

    /*
     * Set a couple entries
     */
    p_Light = (PEXLightEntry *) malloc(4*sizeof(PEXLightEntry));
    p_Light_tmp = p_Light;
    
    p_Light_tmp->type = PEXLightAmbient;
    p_Light_tmp->color.type = PEXColorTypeIndexed;
    p_Light_tmp->color.value.indexed.index = 4;

    p_Light_tmp++;
    p_Light_tmp->type = PEXLightWCVector;
    p_Light_tmp->direction.x = 1.0;
    p_Light_tmp->direction.y = 1.0;
    p_Light_tmp->direction.z = 1.0;
    p_Light_tmp->color.type = PEXColorTypeRGB;
    p_Light_tmp->color.value.rgb.red = 1.0;
    p_Light_tmp->color.value.rgb.green = 0.5;
    p_Light_tmp->color.value.rgb.blue = 0.5;

    p_Light_tmp++;
    p_Light_tmp->type = PEXLightWCPoint;
    p_Light_tmp->point.x = 2.0;
    p_Light_tmp->point.y = 4.0;
    p_Light_tmp->point.z = 8.0;
    p_Light_tmp->attenuation1 = 0.5;
    p_Light_tmp->attenuation2 = 0.5;
    p_Light_tmp->color.type = PEXColorTypeRGB;
    p_Light_tmp->color.value.rgb.red = 0.5;
    p_Light_tmp->color.value.rgb.green = 1.0;
    p_Light_tmp->color.value.rgb.blue = 0.5;

    p_Light_tmp++;
    p_Light_tmp->type = PEXLightWCSpot;
    p_Light_tmp->direction.x = 0.5;
    p_Light_tmp->direction.y = 0.5;
    p_Light_tmp->direction.z = 0.5;
    p_Light_tmp->point.x = 2.0;
    p_Light_tmp->point.y = 2.0;
    p_Light_tmp->point.z = 2.0;
    p_Light_tmp->concentration = 24000.0;
    p_Light_tmp->spread_angle = -2.0;
    p_Light_tmp->attenuation1 = -1.0;
    p_Light_tmp->attenuation2 = 1.0;
    p_Light_tmp->color.type = PEXColorTypeRGB;
    p_Light_tmp->color.value.rgb.red = 0.5;
    p_Light_tmp->color.value.rgb.green = 0.5;
    p_Light_tmp->color.value.rgb.blue = 1.0;

    /*
     * Set Some src light entries
     */
    PEXSetTableEntries (display, src_luts[PEXLUTLight], 1022, 4, 
			PEXLUTLight, p_Light);

    PEXSetTableEntries (display, src_luts[PEXLUTLight], 5400, 3, 
			PEXLUTLight, p_Light);


    /*
     * Set Some dest light entries
     */
    PEXSetTableEntries (display, dest_luts[PEXLUTLight], 102, 4, 
			PEXLUTLight, p_Light);

    PEXSetTableEntries (display, dest_luts[PEXLUTLight], 61111, 2, 
			PEXLUTLight, p_Light);

    PEXSetTableEntries (display, dest_luts[PEXLUTLight], 31020, 1, 
			PEXLUTLight, p_Light);

    free (p_Light);
     
    /*
     * Copy src to dest
     */
    PEXCopyLookupTable (display, src_luts[PEXLUTLight],
			dest_luts[PEXLUTLight]);
    /*
     * See if defined indices are what we expect.
     */
    status = PEXGetDefinedIndices (display, dest_luts[PEXLUTLight], 
				   &return_count, &return_indices);

    if (! ( (status) &&
	    (return_count == 8) &&
	    (return_indices[0] == 1) &&
	    (return_indices[1] == 1022) &&
	    (return_indices[2] == 1023) &&
	    (return_indices[3] == 1024) &&
	    (return_indices[4] == 1025) &&
	    (return_indices[5] == 5400) &&
	    (return_indices[6] == 5401) &&
	    (return_indices[7] == 5402) ))
    {
	_hppex_stderr_print( "ERROR: Dest Light topology not as expected\n");
    }

    XFree (return_indices);

    /*
     * Get a destination entry to see if it copied correctly
     */
    p_Light_tmp = (PEXLightEntry *) PEXGetTableEntry(display, 
					 dest_luts[PEXLUTLight], 
					 1025, PEXRealizedValue, &entry_type, 
					 &table_type);

    if (!((entry_type == PEXDefinedEntry) &&
	  (table_type == PEXLUTLight) &&
          (p_Light_tmp->type == PEXLightWCSpot) &&
          (p_Light_tmp->direction.x == 0.5) &&
          (p_Light_tmp->direction.y == 0.5) &&
          (p_Light_tmp->direction.z == 0.5) &&
          (p_Light_tmp->point.x == 2.0) &&
          (p_Light_tmp->point.y == 2.0) &&
          (p_Light_tmp->point.z == 2.0) &&
          (p_Light_tmp->concentration == 16383.0) &&
          (p_Light_tmp->spread_angle == 0.0) &&
          (p_Light_tmp->attenuation1 == 0.0) &&
          (p_Light_tmp->attenuation2 == 1.0) &&
          (p_Light_tmp->color.type == PEXColorTypeRGB) &&
          (p_Light_tmp->color.value.rgb.red == 0.5) &&
          (p_Light_tmp->color.value.rgb.green == 0.5) &&
          (p_Light_tmp->color.value.rgb.blue == 1.0))) 

    {
	_hppex_stderr_print( "ERROR: Dest Light Entry returned incorrect data\n");
    }

    PEXFreeTableEntries (PEXLUTLight, 1, p_Light_tmp);

    /*
     * Depth Cue entry
     */

    /*
     * Setup a couple entries
     */
    p_DepthCue = (PEXDepthCueEntry *) 
				malloc(2*sizeof(PEXDepthCueEntry));
    p_DepthCue_tmp = p_DepthCue;
    
    p_DepthCue_tmp->mode = PEXOn;
    p_DepthCue_tmp->front_plane = .5;
    p_DepthCue_tmp->back_plane = .25;
    p_DepthCue_tmp->front_scaling = 1.0;
    p_DepthCue_tmp->back_scaling = 0.25;
    p_DepthCue_tmp->color.type = PEXColorTypeRGB;
    p_DepthCue_tmp->color.value.rgb.red = 1.0;
    p_DepthCue_tmp->color.value.rgb.green = 0.5;
    p_DepthCue_tmp->color.value.rgb.blue = 0.5;

    p_DepthCue_tmp++;
    p_DepthCue_tmp->mode = PEXOn;
    p_DepthCue_tmp->front_plane = .5;
    p_DepthCue_tmp->back_plane = .25;
    p_DepthCue_tmp->front_scaling = 1.0;
    p_DepthCue_tmp->back_scaling = 0.25;
    p_DepthCue_tmp->color.type = PEXColorTypeIndexed;
    p_DepthCue_tmp->color.value.indexed.index = 3;

    /* 
     * Set some sparse entries in the src
     */
    PEXSetTableEntries (display, src_luts[PEXLUTDepthCue], 1100, 2, 
			PEXLUTDepthCue, p_DepthCue);

    PEXSetTableEntries (display, src_luts[PEXLUTDepthCue], 21100, 1, 
			PEXLUTDepthCue, p_DepthCue);

    /* 
     * Set some sparse entries in the dest
     */
    PEXSetTableEntries (display, dest_luts[PEXLUTDepthCue], 105, 1, 
			PEXLUTDepthCue, p_DepthCue);

    PEXSetTableEntries (display, dest_luts[PEXLUTDepthCue], 11300, 2, 
			PEXLUTDepthCue, p_DepthCue);

    free (p_DepthCue);
     
    /*
     * Copy src to dest
     */
    PEXCopyLookupTable (display, src_luts[PEXLUTDepthCue],
			dest_luts[PEXLUTDepthCue]);

    /*
     * See if defined indices are what we expect.
     */
    status = PEXGetDefinedIndices (display, dest_luts[PEXLUTDepthCue], 
				   &return_count, &return_indices);

    if (! ( (status) &&
	    (return_count == 4) &&
	    (return_indices[0] == 0) &&
	    (return_indices[1] == 1100) &&
	    (return_indices[2] == 1101) &&
	    (return_indices[3] == 21100) ))
    {
	_hppex_stderr_print( "ERROR: Dest Depth Cue topology not as expected\n");
    }

    XFree (return_indices);

    /*
     * Get an entry to see if it copied correctly
     */
    p_DepthCue_tmp = (PEXDepthCueEntry *) PEXGetTableEntry(display, 
					 dest_luts[PEXLUTDepthCue], 
					 1100, PEXRealizedValue, &entry_type, 
					 &table_type);

    if (!((entry_type == PEXDefinedEntry) &&
	  (table_type == PEXLUTDepthCue) &&
          (p_DepthCue_tmp->mode == PEXOn) &&
          (p_DepthCue_tmp->front_plane == .5) &&
          (p_DepthCue_tmp->back_plane == .25) &&
          (p_DepthCue_tmp->front_scaling == 1.0) &&
          (p_DepthCue_tmp->back_scaling == 0.25) &&
          (p_DepthCue_tmp->color.type == PEXColorTypeRGB) &&
          (p_DepthCue_tmp->color.value.rgb.red == 1.0) &&
          (p_DepthCue_tmp->color.value.rgb.green == 0.5) &&
          (p_DepthCue_tmp->color.value.rgb.blue == 0.5) ))

    {
	_hppex_stderr_print( "Dest Depth Cue Entry has incorrect data\n");
    }

    PEXFreeTableEntries (PEXLUTDepthCue, 1, p_DepthCue_tmp);

    /*
     * Color Approx. Since, the color module checks this
     * is configuration dependent all we will do is created 2 LUTs, delete
     * the default entry from the destination and see if the the copy
     * restores the default entry.
     */

     PEXDeleteTableEntries (display, dest_luts[PEXLUTColorApprox], 0, 1);

    /*
     * See that the lut is empty
     */
    status = PEXGetDefinedIndices (display, dest_luts[PEXLUTColorApprox], 
				   &return_count, &return_indices);

    if (! ( (status) &&
	    (return_count == 0) ))
    {
	_hppex_stderr_print( "ERROR: Dest Color Approx not empty as expected\n");
    }

    XFree (return_indices);

    /*
     * Copy the src to dest
     */
    PEXCopyLookupTable (display, src_luts[PEXLUTColorApprox],
			dest_luts[PEXLUTColorApprox]);

    /*
     * See if src entry was copied
     */
    status = PEXGetDefinedIndices (display, dest_luts[PEXLUTColorApprox], 
				   &return_count, &return_indices);

    if (! ( (status) &&
	    (return_count == 1) &&
	    (return_indices[0] == 0) ))
    {
	_hppex_stderr_print( "ERROR: Dest Color Approx topology not as expected\n");
    }

    XFree (return_indices);
}


void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "lut_copy");
    *num_images      = 1;
    *supported_modes = NON_OC_MODE;

    strcpy(win_title, "lut_copy");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    local_window = window;
    _hppex_set_stderr("lut_copy.stderr", "lut_copy");
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    describe_test( "\nPEXlib Lookup Table Copy Tests:\n\n" );
    describe_test( "This test exercises PEXCopyLookupTable\n");
    describe_test( "using sparse entries for all lookup table\n");
    describe_test( "types.  The test is successful if no error\n");
    describe_test( "messages are printed to stderr.\n\n");

    lut_test(dpy, local_window);
    _hppex_close_stderr();
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
    file_testproc("lut_copy.stderr", "lut_copy");
    }
