/* $XConsortium: lut_edit.c,v 1.0 93/11/22 12:28:35 rws Exp $ */

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
 * lut_edit
 *
 * This test checks out the various aspects of editing a lookup table. It
 * tests out entry setting, entry deletion, and entry inquiry.
 *
 * The basic mechanisms for all LUTs are the same. So, the mechanisms of 
 * deletion and insertion are first checked out on a LineBundle LUT. The
 * remainder of this test just sets a few entries for each type of LUT and
 * then inquires the LUTs. These tests are designed to check the set/realized
 * semantics of entry setting, check the 2 entry inquiry entry points, and check
 * that all entry fields are set correctly and returned on inquiry. 
 * 
 * This test does not test Pattern LUTs which are unsupported. And, it does
 * not test Color Approx LUTs which are covered by the Color sub module testing.
 *
 * Ken Tidwell 
 */

/*
 * Basic set of include files for tests
 */
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
    PEXLookupTable luts[NUM_ENTRIES];     /* 12 lut types, 0'th entry ignored */
    PEXPointer entries [NUM_ENTRIES];     /* 12 lut types, 0'th entry ignored */

    int i, j;
    int table_type, entry_type;
    Status status;
    unsigned long return_count;
    PEXTableIndex *return_indices;
    unsigned long num_fonts;
    char **font_names;
    PEXFont font_id, *p_font;
     

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
	    luts[i] = PEXCreateLookupTable (display, window, i);
    }

    /*********
     * Step 1	Set entries sparsely in table and check them out
     ********/

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
     * Set entries sparsely
     */
    PEXSetTableEntries (display, luts[PEXLUTLineBundle], 511, 3, 
			PEXLUTLineBundle, p_LineBundle);
     
    PEXSetTableEntries (display, luts[PEXLUTLineBundle], 619, 2, 
			PEXLUTLineBundle, p_LineBundle);

    PEXSetTableEntries (display, luts[PEXLUTLineBundle], 65530, 2, 
			PEXLUTLineBundle, p_LineBundle);

    /*
     * See if defined indices are what we expect.
     */
    status = PEXGetDefinedIndices (display, luts[PEXLUTLineBundle], 
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
	_hppex_stderr_print
	     ( "ERROR: sparse LineBundle not set up as expected\n");
    }

    XFree (return_indices);

    /*
     * Delete a couple of entries. Are defined indices still as expected?
     */
    PEXDeleteTableEntries (display, luts[PEXLUTLineBundle], 511, 2);

    status = PEXGetDefinedIndices (display, luts[PEXLUTLineBundle], 
				   &return_count, &return_indices);


    if (! ( (status) &&
	    (return_count == 6) &&
	    (return_indices[0] == 1) &&
	    (return_indices[1] == 513) &&
	    (return_indices[2] == 619) &&
	    (return_indices[3] == 620) &&
	    (return_indices[4] == 65530) &&
	    (return_indices[5] == 65531)
	    ))
    {
	_hppex_stderr_print
	     ( "ERROR: LineBundle not set up as expected following delete\n");
    }

    XFree (return_indices);


    /*
     * Get back some of the entries and see if they are correct. We try to
     * get our deleted entries, some never set entries and some of the defined
     * entries. We also check the set vs. realized values in a few cases.
     */

    /*
     * Get a real entry
     */

    p_LineBundle_tmp = (PEXLineBundleEntry*) PEXGetTableEntry (display, 
					 luts[PEXLUTLineBundle], 1, 
		                         PEXRealizedValue, &entry_type, 
					 &table_type);

    if (!( (entry_type == PEXDefinedEntry) &&
	   (table_type == PEXLUTLineBundle) &&
    	   (p_LineBundle_tmp->type == PEXLineTypeSolid) &&
    	   (p_LineBundle_tmp->interp_method == PEXPolylineInterpNone) &&
    	   (p_LineBundle_tmp->curve_approx.method == PEXApproxImpDep) &&
    	   (p_LineBundle_tmp->curve_approx.tolerance == 1.0) &&
    	   (p_LineBundle_tmp->width == 1.0) &&
    	   (p_LineBundle_tmp->color.type == PEXColorTypeIndexed) &&
    	   (p_LineBundle_tmp->color.value.indexed.index == 1) ))
    {
	_hppex_stderr_print( "ERROR: LineBundle Get Entry 1 failed\n");
    }

    PEXFreeTableEntries (PEXLUTLineBundle, 1, p_LineBundle_tmp);


    /*
     * Get a deleted entry
     */
    p_LineBundle_tmp = (PEXLineBundleEntry*) PEXGetTableEntry (display, 
					 luts[PEXLUTLineBundle], 511, 
		                         PEXRealizedValue, &entry_type, 
					 &table_type);

    if (!( (entry_type == PEXDefaultEntry) &&
	   (table_type == PEXLUTLineBundle) &&
    	   (p_LineBundle_tmp->type == PEXLineTypeSolid) &&
    	   (p_LineBundle_tmp->interp_method == PEXPolylineInterpNone) &&
    	   (p_LineBundle_tmp->curve_approx.method == PEXApproxImpDep) &&
    	   (p_LineBundle_tmp->curve_approx.tolerance == 1.0) &&
    	   (p_LineBundle_tmp->width == 1.0) &&
    	   (p_LineBundle_tmp->color.type == PEXColorTypeIndexed) &&
    	   (p_LineBundle_tmp->color.value.indexed.index == 1) ))
    {
	_hppex_stderr_print( "ERROR: LineBundle Get Entry 2 failed\n");
    }

    PEXFreeTableEntries (PEXLUTLineBundle, 1, p_LineBundle_tmp);

    /*
     * Get a "virgin" entry
     */
    p_LineBundle_tmp = (PEXLineBundleEntry*) PEXGetTableEntry (display, 
					 luts[PEXLUTLineBundle], 900, 
		                         PEXRealizedValue, &entry_type, 
					 &table_type);

    if (!( (entry_type == PEXDefaultEntry) &&
	   (table_type == PEXLUTLineBundle) &&
    	   (p_LineBundle_tmp->type == PEXLineTypeSolid) &&
    	   (p_LineBundle_tmp->interp_method == PEXPolylineInterpNone) &&
    	   (p_LineBundle_tmp->curve_approx.method == PEXApproxImpDep) &&
    	   (p_LineBundle_tmp->curve_approx.tolerance == 1.0) &&
    	   (p_LineBundle_tmp->width == 1.0) &&
    	   (p_LineBundle_tmp->color.type == PEXColorTypeIndexed) &&
    	   (p_LineBundle_tmp->color.value.indexed.index == 1) ))
    {
	_hppex_stderr_print( "ERROR: LineBundle Get Entry 3 failed\n");
    }

    PEXFreeTableEntries (PEXLUTLineBundle, 1, p_LineBundle_tmp);

    /*
     * Get set values from an entry
     */
    p_LineBundle_tmp = (PEXLineBundleEntry*) PEXGetTableEntry (display, 
					 luts[PEXLUTLineBundle], 513, 
		                         PEXSetValue, &entry_type, 
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
	_hppex_stderr_print( "ERROR: LineBundle Get Entry 4 failed\n");
    }

    PEXFreeTableEntries (PEXLUTLineBundle, 1, p_LineBundle_tmp);

    /*
     * Get some entries. Some defined and some not. Check 'em out.
     */

    status = PEXGetTableEntries (display, luts[PEXLUTLineBundle], 618, 4,  
	                         PEXSetValue, &table_type, 
				 (PEXPointer *) &p_LineBundle);

    if (! status)
    {
	_hppex_stderr_print
	     ( "ERROR: Final Get LineBundle Table Entries failed\n");
    }

    if (! (
    	   (p_LineBundle[0].type == PEXLineTypeSolid) &&
    	   (p_LineBundle[0].interp_method == PEXPolylineInterpNone) &&
    	   (p_LineBundle[0].curve_approx.method == PEXApproxImpDep) &&
    	   (p_LineBundle[0].curve_approx.tolerance == 1.0) &&
    	   (p_LineBundle[0].width == 1.0) &&
    	   (p_LineBundle[0].color.type == PEXColorTypeIndexed) &&
    	   (p_LineBundle[0].color.value.indexed.index == 1) ))
    {
	_hppex_stderr_print
	     ("ERROR: Final LineBundle Table Entry 1 is incorrect\n");
    }


    if (! (
    	   (p_LineBundle[1].type == PEXLineTypeDashed) &&
    	   (p_LineBundle[1].interp_method == PEXPolylineInterpNone) &&
    	   (p_LineBundle[1].curve_approx.method == PEXApproxImpDep) &&
    	   (p_LineBundle[1].curve_approx.tolerance == 1.0) &&
    	   (p_LineBundle[1].width == 1.0) &&
    	   (p_LineBundle[1].color.type == PEXColorTypeIndexed) &&
    	   (p_LineBundle[1].color.value.indexed.index == 1) ))
    {
	_hppex_stderr_print
	    ("ERROR: Final LineBundle Table Entry 2 is incorrect\n");
    }


    if (! (
    	   (p_LineBundle[2].type == PEXLineTypeSolid) &&
    	   (p_LineBundle[2].interp_method == PEXPolylineInterpNone) &&
    	   (p_LineBundle[2].curve_approx.method == PEXApproxImpDep) &&
    	   (p_LineBundle[2].curve_approx.tolerance == 1.0) &&
    	   (p_LineBundle[2].width == 2.0) &&
    	   (p_LineBundle[2].color.type == PEXColorTypeIndexed) &&
    	   (p_LineBundle[2].color.value.indexed.index == 2) ))
    {
	_hppex_stderr_print
	    ("ERROR: Final LineBundle Table Entry 3 is incorrect\n");
    }


    if (! (
    	   (p_LineBundle[3].type == PEXLineTypeSolid) &&
    	   (p_LineBundle[3].interp_method == PEXPolylineInterpNone) &&
    	   (p_LineBundle[3].curve_approx.method == PEXApproxImpDep) &&
    	   (p_LineBundle[3].curve_approx.tolerance == 1.0) &&
    	   (p_LineBundle[3].width == 1.0) &&
    	   (p_LineBundle[3].color.type == PEXColorTypeIndexed) &&
    	   (p_LineBundle[3].color.value.indexed.index == 1) ))
    {
	_hppex_stderr_print
	    ("ERROR: Final LineBundle Table Entry 4 is incorrect\n");
    }

    PEXFreeTableEntries (PEXLUTLineBundle, 4, p_LineBundle);
 
    /*
     * Check realized value
     */
    status = PEXGetTableEntries (display, luts[PEXLUTLineBundle], 620, 1,  
	                         PEXRealizedValue, &table_type, 
				 (PEXPointer *) &p_LineBundle);

    if (! (
    	   (p_LineBundle[0].type == PEXLineTypeSolid) &&
    	   (p_LineBundle[0].interp_method == PEXPolylineInterpNone) &&
    	   (p_LineBundle[0].curve_approx.method == PEXApproxImpDep) &&
    	   (p_LineBundle[0].curve_approx.tolerance == 1.0) &&
    	   (p_LineBundle[0].width == 1.0) &&
    	   (p_LineBundle[0].color.type == PEXColorTypeIndexed) &&
    	   (p_LineBundle[0].color.value.indexed.index == 2) ))
    {
	_hppex_stderr_print
	    ("ERROR: Realized LineBundle Table Entry is incorrect\n");
    }

    PEXFreeTableEntries (PEXLUTLineBundle, 1, p_LineBundle);
    
    /*
     * At this point we have checked out the deletion mechanism and the
     * Get Defined Indices mechanism. There is no need to do any more of
     * that. The remainder of this test will concentrate on making sure
     * that all fields of lut entries are set correctly and are inquired
     * correctly. We will also verify set/realized fields in the entries.
     * Since Get Entry and Get Entries use the same support routine to collect
     * the entry data, we do not exhaustive test each entry point. Rather,
     * we use both API routines to cover the interesting cases for entry
     * retrieval.
     */



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
    
    PEXSetTableEntries (display, luts[PEXLUTMarkerBundle], 511, 2, 
			PEXLUTMarkerBundle, p_MarkerBundle);

    free (p_MarkerBundle);
     
    /*
     * Get the realized values of a defined entry
     */
    p_MarkerBundle_tmp = (PEXMarkerBundleEntry*) PEXGetTableEntry(display, 
					 luts[PEXLUTMarkerBundle], 
					 512, PEXRealizedValue, &entry_type, 
					 &table_type);

    if (!((p_MarkerBundle_tmp->type == PEXMarkerCross) &&
	  (p_MarkerBundle_tmp->scale == 1.0) &&
    	  (p_MarkerBundle_tmp->color.type == PEXColorTypeIndexed) &&
    	  (p_MarkerBundle_tmp->color.value.indexed.index = 2) &&
	  (entry_type == PEXDefinedEntry) &&
	  (table_type == PEXLUTMarkerBundle) ))
    {
	_hppex_stderr_print
	    ( "Get defined Marker Bundle Entry returned incorrect data\n");
    }

    PEXFreeTableEntries (PEXLUTMarkerBundle, 1, p_MarkerBundle_tmp);

    /*
     * Get the set values of an undefined entry
     */
    p_MarkerBundle_tmp = (PEXMarkerBundleEntry*)PEXGetTableEntry(display, 
					luts[PEXLUTMarkerBundle], 
					 710, PEXSetValue, &entry_type, 
					 &table_type);

    if (!((p_MarkerBundle_tmp->type == PEXMarkerAsterisk) &&
	  (p_MarkerBundle_tmp->scale == 1.0) &&
    	  (p_MarkerBundle_tmp->color.type == PEXColorTypeIndexed) &&
    	  (p_MarkerBundle_tmp->color.value.indexed.index = 1) &&
	  (entry_type == PEXDefaultEntry) &&
	  (table_type == PEXLUTMarkerBundle) ))
    {
	_hppex_stderr_print
	    ( "Get undefined Marker Bundle Entry returned incorrect data\n");
    }

    PEXFreeTableEntries (PEXLUTMarkerBundle, 1, p_MarkerBundle_tmp);
    
    /*
     * Get a couple entries at once
     */
    status = PEXGetTableEntries (display, luts[PEXLUTMarkerBundle], 510, 3,  
	                         PEXSetValue, &table_type, 
				 (PEXPointer *) &p_MarkerBundle_tmp);

    if (!((p_MarkerBundle_tmp[0].type == PEXMarkerAsterisk) &&
	  (p_MarkerBundle_tmp[0].scale == 1.0) &&
    	  (p_MarkerBundle_tmp[0].color.type == PEXColorTypeIndexed) &&
    	  (p_MarkerBundle_tmp[0].color.value.indexed.index == 1) &&
	  (table_type == PEXLUTMarkerBundle) &&
    	  (p_MarkerBundle_tmp[1].type == PEXMarkerDot) &&
    	  (p_MarkerBundle_tmp[1].scale == 0.5) &&
    	  (p_MarkerBundle_tmp[1].color.type == PEXColorTypeRGB) &&
    	  (p_MarkerBundle_tmp[1].color.value.rgb.red == 1.0) &&
    	  (p_MarkerBundle_tmp[1].color.value.rgb.green == 0.5) &&
    	  (p_MarkerBundle_tmp[1].color.value.rgb.blue == 0.5)  && 
    	  (p_MarkerBundle_tmp[2].type == PEXMarkerCross) &&
    	  (p_MarkerBundle_tmp[2].scale == 0.25) &&
    	  (p_MarkerBundle_tmp[2].color.type == PEXColorTypeIndexed) &&
    	  (p_MarkerBundle_tmp[2].color.value.indexed.index == 2) ))
    {
	_hppex_stderr_print
	    ( "Get Marker Bundle Entries returned incorrect data\n");
    }

    PEXFreeTableEntries (PEXLUTMarkerBundle, 3, p_MarkerBundle_tmp);



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
    
    PEXSetTableEntries (display, luts[PEXLUTTextBundle], 1023, 2, 
			PEXLUTTextBundle, p_TextBundle);

    free (p_TextBundle);
     
    /*
     * Get the realized values of a defined entry
     */
    p_TextBundle_tmp = (PEXTextBundleEntry*) PEXGetTableEntry(display, 
					luts[PEXLUTTextBundle], 
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
	_hppex_stderr_print
	    ( "Get defined Text Bundle Entry returned incorrect data\n");
    }

    PEXFreeTableEntries (PEXLUTTextBundle, 1, p_TextBundle_tmp);

    /*
     * Get the set values of an undefined entry
     */
    p_TextBundle_tmp = (PEXTextBundleEntry*) PEXGetTableEntry(display, 
					luts[PEXLUTTextBundle], 
					 10, PEXSetValue, &entry_type, 
					 &table_type);

    if (!((p_TextBundle_tmp->font_index == 1) &&
          (p_TextBundle_tmp->precision == PEXStrokePrecision) &&
          (p_TextBundle_tmp->char_expansion == 1.0) &&
          (p_TextBundle_tmp->char_spacing == 0.0) &&
          (p_TextBundle_tmp->color.type == PEXColorTypeIndexed) &&
          (p_TextBundle_tmp->color.value.indexed.index == 1) &&
	  (entry_type == PEXDefaultEntry) &&
	  (table_type == PEXLUTTextBundle) ))
    {
	_hppex_stderr_print
	    ( "Get undefined Text Bundle Entry returned incorrect data\n");
    }

    PEXFreeTableEntries (PEXLUTTextBundle, 1, p_TextBundle_tmp);
    
    /*
     * Get a couple entries at once
     */
    status = PEXGetTableEntries (display, luts[PEXLUTTextBundle], 1023, 3,  
	                         PEXSetValue, &table_type, 
				 (PEXPointer *) &p_TextBundle_tmp);

    if (!((table_type == PEXLUTTextBundle) &&
          (p_TextBundle_tmp[0].font_index == 1) &&
          (p_TextBundle_tmp[0].precision == PEXStrokePrecision) &&
          (p_TextBundle_tmp[0].char_expansion == 1.0) &&
          (p_TextBundle_tmp[0].char_spacing == 0.5) &&
          (p_TextBundle_tmp[0].color.type == PEXColorTypeRGB) &&
          (p_TextBundle_tmp[0].color.value.rgb.red == 1.0) &&
          (p_TextBundle_tmp[0].color.value.rgb.green == 0.5) &&
          (p_TextBundle_tmp[0].color.value.rgb.blue == 0.5) &&

    	  (p_TextBundle_tmp[1].font_index == 0) &&
          (p_TextBundle_tmp[1].precision == PEXCharPrecision) &&
          (p_TextBundle_tmp[1].char_expansion == 2.0) &&
          (p_TextBundle_tmp[1].char_spacing == 0.25) &&
          (p_TextBundle_tmp[1].color.type == PEXColorTypeIndexed) &&
          (p_TextBundle_tmp[1].color.value.indexed.index == 2) &&

          (p_TextBundle_tmp[2].font_index == 1) &&
          (p_TextBundle_tmp[2].precision == PEXStrokePrecision) &&
          (p_TextBundle_tmp[2].char_expansion == 1.0) &&
          (p_TextBundle_tmp[2].char_spacing == 0.0) &&
          (p_TextBundle_tmp[2].color.type == PEXColorTypeIndexed) &&
          (p_TextBundle_tmp[2].color.value.indexed.index == 1) ))
    {
	_hppex_stderr_print
	    ( "Get Text Bundle Entries returned incorrect data\n");
    }

    PEXFreeTableEntries (PEXLUTTextBundle, 3, p_TextBundle_tmp);


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

    PEXSetTableEntries (display, luts[PEXLUTInteriorBundle], 255, 2, 
			PEXLUTInteriorBundle, p_InteriorBundle);

    free (p_InteriorBundle);
     
    /*
     * Get the realized values of a defined entry
     */
    p_InteriorBundle_tmp = (PEXInteriorBundleEntry*) PEXGetTableEntry(display, 
					luts[PEXLUTInteriorBundle],
					 255, PEXRealizedValue, &entry_type, 
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
	_hppex_stderr_print
	    ( "Defined Interior Bundle Entry returned incorrect data\n");
    }

    PEXFreeTableEntries (PEXLUTInteriorBundle, 1, p_InteriorBundle_tmp);


    /*
     * Get the set values of an undefined entry
     */
    p_InteriorBundle_tmp = (PEXInteriorBundleEntry*) PEXGetTableEntry(display, 
					luts[PEXLUTInteriorBundle],
					 700, PEXSetValue, &entry_type, 
					 &table_type);

    if (!((entry_type == PEXDefaultEntry) &&
	  (table_type == PEXLUTInteriorBundle) &&
	  (p_InteriorBundle_tmp->style == PEXInteriorStyleHollow) &&
          (p_InteriorBundle_tmp->style_index == 0) &&
          (p_InteriorBundle_tmp->reflection_model == PEXReflectionNone) &&
          (p_InteriorBundle_tmp->interp_method == PEXSurfaceInterpNone) &&
          (p_InteriorBundle_tmp->bf_style == PEXInteriorStyleHollow) &&
          (p_InteriorBundle_tmp->bf_style_index == 0) &&
          (p_InteriorBundle_tmp->bf_reflection_model == PEXReflectionNone) &&
          (p_InteriorBundle_tmp->bf_interp_method == PEXSurfaceInterpNone) &&
          (p_InteriorBundle_tmp->surface_approx.method == 
							PEXApproxImpDep) &&
          (p_InteriorBundle_tmp->surface_approx.u_tolerance == 1.0) &&
          (p_InteriorBundle_tmp->surface_approx.v_tolerance == 1.0) &&
          (p_InteriorBundle_tmp->color.type == PEXColorTypeIndexed) &&
          (p_InteriorBundle_tmp->color.value.indexed.index == 1) &&
          (p_InteriorBundle_tmp->reflection_attr.ambient == 1.0) &&
          (p_InteriorBundle_tmp->reflection_attr.diffuse == 1.0) &&
          (p_InteriorBundle_tmp->reflection_attr.specular == 1.0) &&
          (p_InteriorBundle_tmp->reflection_attr.specular_conc == 0.0) &&
          (p_InteriorBundle_tmp->reflection_attr.transmission == 0.0) &&
          (p_InteriorBundle_tmp->reflection_attr.specular_color.type == 
							PEXColorTypeIndexed) &&
          (p_InteriorBundle_tmp->reflection_attr.specular_color.value.indexed.index == 1.0) &&
          (p_InteriorBundle_tmp->bf_color.type == PEXColorTypeIndexed) &&
          (p_InteriorBundle_tmp->bf_color.value.indexed.index == 1) &&
          (p_InteriorBundle_tmp->bf_reflection_attr.ambient == 1.0) &&
          (p_InteriorBundle_tmp->bf_reflection_attr.diffuse == 1.0) &&
          (p_InteriorBundle_tmp->bf_reflection_attr.specular == 1.0) &&
          (p_InteriorBundle_tmp->bf_reflection_attr.specular_conc == 0.0) &&
          (p_InteriorBundle_tmp->bf_reflection_attr.transmission == 0.0) &&
          (p_InteriorBundle_tmp->bf_reflection_attr.specular_color.type == 
							PEXColorTypeIndexed) &&
          (p_InteriorBundle_tmp->bf_reflection_attr.specular_color.value.indexed.index == 1.0) ))
    {
	_hppex_stderr_print
	    ( "Undefined Interior Bundle Entry returned incorrect data\n");
    }

    PEXFreeTableEntries (PEXLUTInteriorBundle, 1, p_InteriorBundle_tmp);
    
    /*
     * Get a couple entries at once
     */
    status = PEXGetTableEntries (display, luts[PEXLUTInteriorBundle], 255, 3,  
	                         PEXSetValue, &table_type, 
				 (PEXPointer *) &p_InteriorBundle_tmp);

    if (!((table_type == PEXLUTInteriorBundle) &&
          (p_InteriorBundle_tmp[0].style == PEXInteriorStyleSolid) &&
          (p_InteriorBundle_tmp[0].style_index == 2) &&
          (p_InteriorBundle_tmp[0].reflection_model == PEXReflectionDiffuse) &&
          (p_InteriorBundle_tmp[0].interp_method == PEXSurfaceInterpColor) &&
          (p_InteriorBundle_tmp[0].bf_style == PEXInteriorStyleSolid) &&
          (p_InteriorBundle_tmp[0].bf_style_index == 1) &&
          (p_InteriorBundle_tmp[0].bf_reflection_model == PEXReflectionDiffuse) &&
          (p_InteriorBundle_tmp[0].bf_interp_method == PEXSurfaceInterpNone) &&
          (p_InteriorBundle_tmp[0].surface_approx.method == PEXApproxNPCRelative) &&
          (p_InteriorBundle_tmp[0].surface_approx.u_tolerance == 1.0) &&
          (p_InteriorBundle_tmp[0].surface_approx.v_tolerance == 0.5) &&
          (p_InteriorBundle_tmp[0].color.type == PEXColorTypeRGB) &&
          (p_InteriorBundle_tmp[0].color.value.rgb.red == 1.0) &&
          (p_InteriorBundle_tmp[0].color.value.rgb.green == 0.5) &&
          (p_InteriorBundle_tmp[0].color.value.rgb.blue == 0.5) &&
          (p_InteriorBundle_tmp[0].reflection_attr.ambient == 1.5) &&
          (p_InteriorBundle_tmp[0].reflection_attr.diffuse == 1.5) &&
          (p_InteriorBundle_tmp[0].reflection_attr.specular == 1.25) &&
          (p_InteriorBundle_tmp[0].reflection_attr.specular_conc == 65535.0) &&
          (p_InteriorBundle_tmp[0].reflection_attr.transmission == 1.5) &&
          (p_InteriorBundle_tmp[0].reflection_attr.specular_color.type == PEXColorTypeRGB) &&
          (p_InteriorBundle_tmp[0].reflection_attr.specular_color.value.rgb.red == 1.0) &&
          (p_InteriorBundle_tmp[0].reflection_attr.specular_color.value.rgb.green == 0.5) &&
          (p_InteriorBundle_tmp[0].reflection_attr.specular_color.value.rgb.blue == 0.5) &&
          (p_InteriorBundle_tmp[0].bf_color.type == PEXColorTypeIndexed) &&
          (p_InteriorBundle_tmp[0].bf_color.value.indexed.index == 2) &&
          (p_InteriorBundle_tmp[0].bf_reflection_attr.ambient == 1.5) &&
          (p_InteriorBundle_tmp[0].bf_reflection_attr.diffuse == 1.25) &&
          (p_InteriorBundle_tmp[0].bf_reflection_attr.specular == 1.5) &&
          (p_InteriorBundle_tmp[0].bf_reflection_attr.specular_conc == 40000.0) &&
          (p_InteriorBundle_tmp[0].bf_reflection_attr.transmission == 1.25) &&
          (p_InteriorBundle_tmp[0].bf_reflection_attr.specular_color.type == PEXColorTypeRGB) &&
          (p_InteriorBundle_tmp[0].bf_reflection_attr.specular_color.value.rgb.red == 1.0) &&
          (p_InteriorBundle_tmp[0].bf_reflection_attr.specular_color.value.rgb.green == 0.5) &&
          (p_InteriorBundle_tmp[0].bf_reflection_attr.specular_color.value.rgb.blue == 0.5) &&

          (p_InteriorBundle_tmp[1].style == PEXInteriorStyleSolid) &&
          (p_InteriorBundle_tmp[1].style_index == 3) &&
          (p_InteriorBundle_tmp[1].reflection_model == PEXReflectionSpecular) &&
          (p_InteriorBundle_tmp[1].interp_method == PEXSurfaceInterpColor) &&
          (p_InteriorBundle_tmp[1].bf_style == PEXInteriorStyleSolid) &&
          (p_InteriorBundle_tmp[1].bf_style_index == 2) &&
          (p_InteriorBundle_tmp[1].bf_reflection_model == PEXReflectionAmbient) &&
          (p_InteriorBundle_tmp[1].bf_interp_method == PEXSurfaceInterpNone) &&
          (p_InteriorBundle_tmp[1].surface_approx.method == PEXApproxWCRelative) &&
          (p_InteriorBundle_tmp[1].surface_approx.u_tolerance == 1.0) &&
          (p_InteriorBundle_tmp[1].surface_approx.v_tolerance == 0.5) &&
          (p_InteriorBundle_tmp[1].color.type == PEXColorTypeIndexed) &&
          (p_InteriorBundle_tmp[1].color.value.indexed.index == 2) &&
          (p_InteriorBundle_tmp[1].reflection_attr.ambient == 0.5) &&
          (p_InteriorBundle_tmp[1].reflection_attr.diffuse == 0.5) &&
          (p_InteriorBundle_tmp[1].reflection_attr.specular == 0.25) &&
          (p_InteriorBundle_tmp[1].reflection_attr.specular_conc == 10.0) &&
          (p_InteriorBundle_tmp[1].reflection_attr.transmission == 0.5) &&
          (p_InteriorBundle_tmp[1].reflection_attr.specular_color.type == PEXColorTypeRGB) &&
          (p_InteriorBundle_tmp[1].reflection_attr.specular_color.value.rgb.red == 1.0) &&
          (p_InteriorBundle_tmp[1].reflection_attr.specular_color.value.rgb.green == 0.5) &&
          (p_InteriorBundle_tmp[1].reflection_attr.specular_color.value.rgb.blue == 0.5) &&
          (p_InteriorBundle_tmp[1].bf_color.type == PEXColorTypeRGB) &&
          (p_InteriorBundle_tmp[1].bf_color.value.rgb.red == 1.0) &&
          (p_InteriorBundle_tmp[1].bf_color.value.rgb.green == 0.25) &&
          (p_InteriorBundle_tmp[1].bf_color.value.rgb.blue == 0.5) &&
          (p_InteriorBundle_tmp[1].bf_reflection_attr.ambient == 0.5) &&
          (p_InteriorBundle_tmp[1].bf_reflection_attr.diffuse == 0.25) &&
          (p_InteriorBundle_tmp[1].bf_reflection_attr.specular == 0.5) &&
          (p_InteriorBundle_tmp[1].bf_reflection_attr.specular_conc == 100.0) &&
          (p_InteriorBundle_tmp[1].bf_reflection_attr.transmission == 0.25) &&
          (p_InteriorBundle_tmp[1].bf_reflection_attr.specular_color.type == PEXColorTypeRGB) &&
          (p_InteriorBundle_tmp[1].bf_reflection_attr.specular_color.value.rgb.red == 1.0) &&
          (p_InteriorBundle_tmp[1].bf_reflection_attr.specular_color.value.rgb.green == 0.5) &&
          (p_InteriorBundle_tmp[1].bf_reflection_attr.specular_color.value.rgb.blue == 0.5) &&

	  (p_InteriorBundle_tmp[2].style == PEXInteriorStyleHollow) &&
          (p_InteriorBundle_tmp[2].style_index == 0) &&
          (p_InteriorBundle_tmp[2].reflection_model == PEXReflectionNone) &&
          (p_InteriorBundle_tmp[2].interp_method == PEXSurfaceInterpNone) &&
          (p_InteriorBundle_tmp[2].bf_style == PEXInteriorStyleHollow) &&
          (p_InteriorBundle_tmp[2].bf_style_index == 0) &&
          (p_InteriorBundle_tmp[2].bf_reflection_model == PEXReflectionNone) &&
          (p_InteriorBundle_tmp[2].bf_interp_method == PEXSurfaceInterpNone) &&
          (p_InteriorBundle_tmp[2].surface_approx.method == 
							PEXApproxImpDep) &&
          (p_InteriorBundle_tmp[2].surface_approx.u_tolerance == 1.0) &&
          (p_InteriorBundle_tmp[2].surface_approx.v_tolerance == 1.0) &&
          (p_InteriorBundle_tmp[2].color.type == PEXColorTypeIndexed) &&
          (p_InteriorBundle_tmp[2].color.value.indexed.index == 1) &&
          (p_InteriorBundle_tmp[2].reflection_attr.ambient == 1.0) &&
          (p_InteriorBundle_tmp[2].reflection_attr.diffuse == 1.0) &&
          (p_InteriorBundle_tmp[2].reflection_attr.specular == 1.0) &&
          (p_InteriorBundle_tmp[2].reflection_attr.specular_conc == 0.0) &&
          (p_InteriorBundle_tmp[2].reflection_attr.transmission == 0.0) &&
          (p_InteriorBundle_tmp[2].reflection_attr.specular_color.type == 
							PEXColorTypeIndexed) &&
          (p_InteriorBundle_tmp[2].reflection_attr.specular_color.value.indexed.index == 1.0) &&
          (p_InteriorBundle_tmp[2].bf_color.type == PEXColorTypeIndexed) &&
          (p_InteriorBundle_tmp[2].bf_color.value.indexed.index == 1) &&
          (p_InteriorBundle_tmp[2].bf_reflection_attr.ambient == 1.0) &&
          (p_InteriorBundle_tmp[2].bf_reflection_attr.diffuse == 1.0) &&
          (p_InteriorBundle_tmp[2].bf_reflection_attr.specular == 1.0) &&
          (p_InteriorBundle_tmp[2].bf_reflection_attr.specular_conc == 0.0) &&
          (p_InteriorBundle_tmp[2].bf_reflection_attr.transmission == 0.0) &&
          (p_InteriorBundle_tmp[2].bf_reflection_attr.specular_color.type == 
							PEXColorTypeIndexed) &&
          (p_InteriorBundle_tmp[2].bf_reflection_attr.specular_color.value.indexed.index == 1.0) ))
    {
	_hppex_stderr_print
	    ( "Get Interior Bundle Entries returned incorrect data\n");
    }

    PEXFreeTableEntries (PEXLUTInteriorBundle, 3, p_InteriorBundle_tmp);



    /*
     * Check out Edges
     */


    /*
     * Set a couple entries
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

    PEXSetTableEntries (display, luts[PEXLUTEdgeBundle], 1100, 2, 
			PEXLUTEdgeBundle, p_EdgeBundle);

    free (p_EdgeBundle);
     
    /*
     * Get the realized values of a defined entry
     */
    p_EdgeBundle_tmp = (PEXEdgeBundleEntry*)PEXGetTableEntry(display, 
					luts[PEXLUTEdgeBundle], 
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
	_hppex_stderr_print
	    ( "Defined Edge Bundle Entry returned incorrect data\n");
    }

    PEXFreeTableEntries (PEXLUTEdgeBundle, 1, p_EdgeBundle_tmp);

    /*
     * Get the set values of an undefined entry
     */
    p_EdgeBundle_tmp = (PEXEdgeBundleEntry*) PEXGetTableEntry(display, 
					luts[PEXLUTEdgeBundle], 
					 410, PEXSetValue, &entry_type, 
					 &table_type);

    if (!((entry_type == PEXDefaultEntry) &&
	  (table_type == PEXLUTEdgeBundle) &&
	  (p_EdgeBundle_tmp->edge_flag == PEXOff) &&
          (p_EdgeBundle_tmp->type == PEXSurfaceEdgeSolid) &&
          (p_EdgeBundle_tmp->width == 1.0) &&
          (p_EdgeBundle_tmp->color.type == PEXColorTypeIndexed) &&
          (p_EdgeBundle_tmp->color.value.indexed.index == 1)))
    {
	_hppex_stderr_print
	    ( "Undefined Edge Bundle Entry returned incorrect data\n");
    }

    PEXFreeTableEntries (PEXLUTEdgeBundle, 1, p_EdgeBundle_tmp);
    

    /*
     * Get a couple entries at once
     */
    status = PEXGetTableEntries (display, luts[PEXLUTEdgeBundle], 1099, 3,  
	                         PEXSetValue, &table_type, 
				 (PEXPointer *) &p_EdgeBundle_tmp);

    if (!((p_EdgeBundle_tmp[0].edge_flag == PEXOff) &&
          (p_EdgeBundle_tmp[0].type == PEXSurfaceEdgeSolid) &&
          (p_EdgeBundle_tmp[0].width == 1.0) &&
          (p_EdgeBundle_tmp[0].color.type == PEXColorTypeIndexed) &&
          (p_EdgeBundle_tmp[0].color.value.indexed.index == 1) &&

          (p_EdgeBundle_tmp[1].edge_flag == PEXOn) &&
          (p_EdgeBundle_tmp[1].type == PEXSurfaceEdgeSolid) &&
          (p_EdgeBundle_tmp[1].width == 0.5) &&
          (p_EdgeBundle_tmp[1].color.type == PEXColorTypeRGB) &&
          (p_EdgeBundle_tmp[1].color.value.rgb.red == 1.0) &&
          (p_EdgeBundle_tmp[1].color.value.rgb.green == 0.5) &&
          (p_EdgeBundle_tmp[1].color.value.rgb.blue == 0.5) &&

          (p_EdgeBundle_tmp[2].edge_flag == PEXOn) &&
          (p_EdgeBundle_tmp[2].type == PEXSurfaceEdgeSolid) &&
          (p_EdgeBundle_tmp[2].width == 1.0) &&
          (p_EdgeBundle_tmp[2].color.type == PEXColorTypeIndexed) &&
          (p_EdgeBundle_tmp[2].color.value.indexed.index == 2) ))
    {
	_hppex_stderr_print
	    ( "Get Edge Bundle Entries returned incorrect data\n");
    }

    PEXFreeTableEntries (PEXLUTEdgeBundle, 3, p_EdgeBundle_tmp);


    /*
     * Skip Pattern since it is unsupported.
     */


    /*
     * Check out fonts. This not an exhaustive test. That is left to the
     * font module. This test just goes through the motions to make sure
     * nothing dies unexpectedly and thing look okay as far as the 
     * mechanism goes. It does not check out the fonts.
     */

    /* Get a font */
    font_names = PEXListFonts (display, "*", 1, &num_fonts);
    font_id = PEXLoadFont (display, font_names[0]);
    PEXFreeFontNames (1, font_names);
    
    p_TextFont = (PEXTextFontEntry *) 
				malloc((2*sizeof(PEXTextFontEntry)) +
                                       (2*sizeof(PEXFont)));
    p_TextFont_tmp = p_TextFont;
    p_font = (PEXFont *) (p_TextFont + 2);

    p_TextFont_tmp->count = 1;
    *p_font = font_id;
    p_TextFont_tmp->fonts = p_font;

    p_TextFont_tmp++; p_font++;
    p_TextFont_tmp->count = 1;
    *p_font = font_id;
    p_TextFont_tmp->fonts = p_font;

    PEXSetTableEntries (display, luts[PEXLUTTextFont], 255, 2, 
			PEXLUTTextFont, p_TextFont);

    /*
     * Get defined entry
     */

    p_TextFont_tmp = (PEXTextFontEntry*)PEXGetTableEntry(display, 
					luts[PEXLUTTextFont], 
					 255, PEXSetValue, &entry_type, 
					 &table_type);

    if (!((entry_type == PEXDefinedEntry) &&
	  (table_type == PEXLUTTextFont) &&
          (p_TextFont_tmp->count == 1) &&
          (p_TextFont_tmp->fonts[0] == font_id) )) 
    {
	_hppex_stderr_print( "Defined Text Font entry is incorrect\n");
    }

    PEXFreeTableEntries (PEXLUTTextFont, 1, p_TextFont_tmp);

    /*
     * Get default entry
     */

    p_TextFont_tmp = (PEXTextFontEntry*) PEXGetTableEntry(display, 
					luts[PEXLUTTextFont], 
					 253, PEXSetValue, &entry_type, 
					 &table_type);

    if (!((entry_type == PEXDefaultEntry) &&
	  (table_type == PEXLUTTextFont) &&
          (p_TextFont_tmp->count == 1) &&
          (p_TextFont_tmp->fonts[0] != 0) ))  /* Anything non-zero is fine */
    {
	_hppex_stderr_print( "Undefined Text Font entry is incorrect\n");
    }

    PEXFreeTableEntries (PEXLUTTextFont, 1, p_TextFont_tmp);


    /*
     * Get Entries
     */
    status = PEXGetTableEntries (display, luts[PEXLUTTextFont], 254, 3,  
	                         PEXRealizedValue, &table_type, 
				 (PEXPointer *) &p_TextFont_tmp);

    if (!((status) &&
	  (table_type == PEXLUTTextFont) &&
          (p_TextFont_tmp[0].count == 1) &&
          (p_TextFont_tmp[0].fonts[0] != 0) &&
          (p_TextFont_tmp[1].count == 1) &&
          (p_TextFont_tmp[1].fonts[0] == font_id) &&
          (p_TextFont_tmp[2].count == 1) &&
          (p_TextFont_tmp[2].fonts[0] == font_id)))
    {
	_hppex_stderr_print( "Get Text Font entries are incorrect\n");
    }

    PEXFreeTableEntries (PEXLUTTextFont, 3, p_TextFont_tmp);



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

    PEXSetTableEntries (display, luts[PEXLUTColor], 1400, 2, 
			PEXLUTColor, p_Color);

    free (p_Color);
     
    /*
     * Get the realized values of a defined entry
     */
    p_Color_tmp = (PEXColorEntry*) PEXGetTableEntry(display, luts[PEXLUTColor], 
				 	1401, PEXRealizedValue, &entry_type, 
					 &table_type);

    if (!((entry_type == PEXDefinedEntry) &&
	  (table_type == PEXLUTColor) &&
          (p_Color_tmp->type == PEXColorTypeRGB) &&
          (p_Color_tmp->value.rgb.red == 0.5) &&
          (p_Color_tmp->value.rgb.green == 0.25) &&
          (p_Color_tmp->value.rgb.blue == 1.0) ))
    {
	_hppex_stderr_print( "Defined Color Entry returned incorrect data\n");
    }

    PEXFreeTableEntries (PEXLUTColor, 1, p_Color_tmp);

    /*
     * Get the set values of an undefined entry
     */
    p_Color_tmp =  (PEXColorEntry*)PEXGetTableEntry(display, luts[PEXLUTColor], 
					 200, PEXSetValue, &entry_type, 
					 &table_type);

    if (!((entry_type == PEXDefaultEntry) &&
	  (table_type == PEXLUTColor) &&
          (p_Color_tmp->type == PEXColorTypeRGB) &&
          (p_Color_tmp->value.rgb.red == 1.0) &&
          (p_Color_tmp->value.rgb.green == 1.0) &&
          (p_Color_tmp->value.rgb.blue == 1.0) ))
    {
	_hppex_stderr_print( "Undefined Color Entry returned incorrect data\n");
    }

    PEXFreeTableEntries (PEXLUTColor, 1, p_Color_tmp);
    

    /*
     * Get a couple entries at once
     */
    status = PEXGetTableEntries (display, luts[PEXLUTColor], 1399, 3,  
	                         PEXSetValue, &table_type, 
				 (PEXPointer *) &p_Color_tmp);

    if (!((p_Color_tmp[0].type == PEXColorTypeRGB) &&
          (p_Color_tmp[0].value.rgb.red == 1.0) &&
          (p_Color_tmp[0].value.rgb.green == 1.0) &&
          (p_Color_tmp[0].value.rgb.blue == 1.0) &&

          (p_Color_tmp[1].type == PEXColorTypeRGB) &&
          (p_Color_tmp[1].value.rgb.red == 0.25) &&
          (p_Color_tmp[1].value.rgb.green == 0.5) &&
          (p_Color_tmp[1].value.rgb.blue == 0.25) &&

          (p_Color_tmp[2].type == PEXColorTypeRGB) &&
          (p_Color_tmp[2].value.rgb.red == 0.5) &&
          (p_Color_tmp[2].value.rgb.green == 0.25) &&
          (p_Color_tmp[2].value.rgb.blue == 1.0) ))
    {
	_hppex_stderr_print( "Get Color Entries returned incorrect data\n");
    }

    PEXFreeTableEntries (PEXLUTColor, 3, p_Color_tmp);

    

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

    PEXSetTableEntries (display, luts[PEXLUTView], 16000, 2, 
			PEXLUTView, p_View);

    free (p_View);
     
    /*
     * Get the realized values of a defined entry
     */
    p_View_tmp =  (PEXViewEntry*) PEXGetTableEntry(display, luts[PEXLUTView], 
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
	_hppex_stderr_print( "Defined View Entry returned incorrect data\n");
    }

    for (i=0; i<4; i++)
        for (j=0; j<4; j++)
        {
	    if (!((p_View_tmp->orientation[i][j] == 2.0) &&
	          (p_View_tmp->mapping[i][j] == 1.0) ))
	    {
	        _hppex_stderr_print
		    ( "Defined View Entry arrays returned incorrect data\n");
		break;
	    }
	}

    PEXFreeTableEntries (PEXLUTView, 1, p_View_tmp);

    /*
     * Get the set values of an undefined entry
     */
    p_View_tmp = (PEXViewEntry*) PEXGetTableEntry(display, luts[PEXLUTView], 
					 610, PEXSetValue, &entry_type, 
					 &table_type);

    if (!((entry_type == PEXDefaultEntry) &&
	  (table_type == PEXLUTView) &&
    	  (p_View_tmp->clip_flags == PEXClippingAll) &&
	  (p_View_tmp->clip_limits.min.x == 0.0) &&
    	  (p_View_tmp->clip_limits.min.y == 0.0) &&
    	  (p_View_tmp->clip_limits.min.z == 0.0) &&
    	  (p_View_tmp->clip_limits.max.x == 1.0) &&
    	  (p_View_tmp->clip_limits.max.y == 1.0) &&
    	  (p_View_tmp->clip_limits.max.z == 1.0) ))
    {
	_hppex_stderr_print( "Defined View Entry returned incorrect data\n");
    }

    for (i=0; i<4; i++)
        for (j=0; j<4; j++)
        {
   	    if (i == j)
	    {
	        if (!((p_View_tmp->orientation[i][j] == 1.0) &&
	              (p_View_tmp->mapping[i][j] == 1.0) ))
	        {
	           _hppex_stderr_print
		       ( "Undefined View Entry arrays returned incorrect data\n");
		   break;
	        }
	    }
	    else
 	    {
	        if (!((p_View_tmp->orientation[i][j] == 0.0) &&
	              (p_View_tmp->mapping[i][j] == 0.0) ))
	        {
	            _hppex_stderr_print
			( "Undefined View Entry arrays returned incorrect data\n");
		   break;
	        }
	    }
	}

    PEXFreeTableEntries (PEXLUTView, 1, p_View_tmp);
    

    /*
     * Get a couple entries at once
     */
    status = PEXGetTableEntries (display, luts[PEXLUTView], 16000, 3,  
	                         PEXSetValue, &table_type, 
				 (PEXPointer *) &p_View_tmp);

    if (!((p_View_tmp->clip_flags == PEXClipXY | PEXClipBack) &&
	  (p_View_tmp[0].clip_limits.min.x == 0.25) &&
    	  (p_View_tmp[0].clip_limits.min.y == 0.25) &&
    	  (p_View_tmp[0].clip_limits.min.z == 0.25) &&
    	  (p_View_tmp[0].clip_limits.max.x == 0.5) &&
    	  (p_View_tmp[0].clip_limits.max.y == 0.5) &&
    	  (p_View_tmp[0].clip_limits.max.z == 0.5) &&

    	  (p_View_tmp[1].clip_flags == PEXClipXY | PEXClipFront) &&
	  (p_View_tmp[1].clip_limits.min.x == 0.0) &&
    	  (p_View_tmp[1].clip_limits.min.y == 0.0) &&
    	  (p_View_tmp[1].clip_limits.min.z == 0.0) &&
    	  (p_View_tmp[1].clip_limits.max.x == 0.5) &&
    	  (p_View_tmp[1].clip_limits.max.y == 0.5) &&
    	  (p_View_tmp[1].clip_limits.max.z == 0.5) &&

    	  (p_View_tmp[2].clip_flags == PEXClippingAll) &&
	  (p_View_tmp[2].clip_limits.min.x == 0.0) &&
    	  (p_View_tmp[2].clip_limits.min.y == 0.0) &&
    	  (p_View_tmp[2].clip_limits.min.z == 0.0) &&
    	  (p_View_tmp[2].clip_limits.max.x == 1.0) &&
    	  (p_View_tmp[2].clip_limits.max.y == 1.0) &&
    	  (p_View_tmp[2].clip_limits.max.z == 1.0) ))
    {
	_hppex_stderr_print
	    ( "Get View Entries returned incorrect data\n");
    }

    for (i=0; i<4; i++)
        for (j=0; j<4; j++)
        {
	    if (!((p_View_tmp[0].orientation[i][j] == 2.0) &&
	          (p_View_tmp[0].mapping[i][j] == 1.0) ))
	    {
	        _hppex_stderr_print
		    ( "Get View Entries first arrays returned incorrect data\n");
		break;
	    }
	}

    for (i=0; i<4; i++)
        for (j=0; j<4; j++)
        {
	    if (!((p_View_tmp[1].orientation[i][j] == 4.0) &&
	          (p_View_tmp[1].mapping[i][j] == 1.0) ))
	    {
	        _hppex_stderr_print
		    ( "Get View Entries second arrays returned incorrect data\n");
		break;
	    }
	}

    for (i=0; i<4; i++)
        for (j=0; j<4; j++)
        {
   	    if (i == j)
	    {
	        if (!((p_View_tmp[2].orientation[i][j] == 1.0) &&
	              (p_View_tmp[2].mapping[i][j] == 1.0) ))
	        {
	           _hppex_stderr_print
		   ( "Get View Entries third arrays returned incorrect data\n");
		   break;
	        }
	    }
	    else
 	    {
	        if (!((p_View_tmp[2].orientation[i][j] == 0.0) &&
	              (p_View_tmp[2].mapping[i][j] == 0.0) ))
	        {
	           _hppex_stderr_print
		   ( "Get View Entries third arrays returned incorrect data\n");
		   break;
	        }
	    }
	}

    PEXFreeTableEntries (PEXLUTView, 3, p_View_tmp);



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

    PEXSetTableEntries (display, luts[PEXLUTLight], 1022, 4, 
			PEXLUTLight, p_Light);

    free (p_Light);
     
    /*
     * Get the realized values of a defined entry
     */
    p_Light_tmp = (PEXLightEntry*)PEXGetTableEntry(display, luts[PEXLUTLight], 
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
	_hppex_stderr_print( "Defined Light Entry returned incorrect data\n");
    }

    PEXFreeTableEntries (PEXLUTEdgeBundle, 1, p_EdgeBundle_tmp);

    /*
     * Get the set values of an undefined entry
     */
    p_Light_tmp = (PEXLightEntry*)PEXGetTableEntry(display, luts[PEXLUTLight], 
					 410, PEXSetValue, &entry_type, 
					 &table_type);

    if (!((entry_type == PEXDefaultEntry) &&
	  (table_type == PEXLUTLight) &&
          (p_Light_tmp->type == PEXLightAmbient) &&
          (p_Light_tmp->color.type == PEXColorTypeRGB) &&
          (p_Light_tmp->color.value.rgb.red == 1.0) &&
          (p_Light_tmp->color.value.rgb.green == 1.0) &&
          (p_Light_tmp->color.value.rgb.blue == 1.0)))
    {
	_hppex_stderr_print( "Undefined Light Entry returned incorrect data\n");
    }

    PEXFreeTableEntries (PEXLUTLight, 1, p_Light_tmp);
    

    /*
     * Get a1l the entries at once
     */
    status = PEXGetTableEntries (display, luts[PEXLUTLight], 1021, 5,  
	                         PEXSetValue, &table_type, 
				 (PEXPointer *) &p_Light_tmp);

    if (!((p_Light_tmp[0].type == PEXLightAmbient) &&
          (p_Light_tmp[0].color.type == PEXColorTypeRGB) &&
          (p_Light_tmp[0].color.value.rgb.red == 1.0) &&
          (p_Light_tmp[0].color.value.rgb.green == 1.0) &&
          (p_Light_tmp[0].color.value.rgb.blue == 1.0) &&

          (p_Light_tmp[1].type == PEXLightAmbient) &&
          (p_Light_tmp[1].color.type == PEXColorTypeIndexed) &&
          (p_Light_tmp[1].color.value.indexed.index == 4) &&

          (p_Light_tmp[2].type == PEXLightWCVector) &&
          (p_Light_tmp[2].direction.x == 1.0) &&
          (p_Light_tmp[2].direction.y == 1.0) &&
          (p_Light_tmp[2].direction.z == 1.0) &&
          (p_Light_tmp[2].color.type == PEXColorTypeRGB) &&
          (p_Light_tmp[2].color.value.rgb.red == 1.0) &&
          (p_Light_tmp[2].color.value.rgb.green == 0.5) &&
          (p_Light_tmp[2].color.value.rgb.blue == 0.5) &&

          (p_Light_tmp[3].type == PEXLightWCPoint) &&
          (p_Light_tmp[3].point.x == 2.0) &&
          (p_Light_tmp[3].point.y == 4.0) &&
          (p_Light_tmp[3].point.z == 8.0) &&
          (p_Light_tmp[3].attenuation1 == 0.5) &&
          (p_Light_tmp[3].attenuation2 == 0.5) &&
          (p_Light_tmp[3].color.type == PEXColorTypeRGB) &&
          (p_Light_tmp[3].color.value.rgb.red == 0.5) &&
          (p_Light_tmp[3].color.value.rgb.green == 1.0) &&
          (p_Light_tmp[3].color.value.rgb.blue == 0.5) &&

          (p_Light_tmp[4].type == PEXLightWCSpot) &&
          (p_Light_tmp[4].direction.x == 0.5) &&
          (p_Light_tmp[4].direction.y == 0.5) &&
          (p_Light_tmp[4].direction.z == 0.5) &&
          (p_Light_tmp[4].point.x == 2.0) &&
          (p_Light_tmp[4].point.y == 2.0) &&
          (p_Light_tmp[4].point.z == 2.0) &&
          (p_Light_tmp[4].concentration == 24000.0) &&
          (p_Light_tmp[4].spread_angle == -2.0) &&
          (p_Light_tmp[4].attenuation1 == -1.0) &&
          (p_Light_tmp[4].attenuation2 == 1.0) &&
          (p_Light_tmp[4].color.type == PEXColorTypeRGB) &&
          (p_Light_tmp[4].color.value.rgb.red == 0.5) &&
          (p_Light_tmp[4].color.value.rgb.green == 0.5) &&
          (p_Light_tmp[4].color.value.rgb.blue == 1.0) ))
    {
	_hppex_stderr_print( "Get Light Entries returned incorrect data\n");
    }

    PEXFreeTableEntries (PEXLUTLight, 5, p_Light_tmp);


    /*
     * Depth Cue entry
     */

    /*
     * Set a couple entries
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

    PEXSetTableEntries (display, luts[PEXLUTDepthCue], 1100, 2, 
			PEXLUTDepthCue, p_DepthCue);

    free (p_DepthCue);
     
    /*
     * Get the realized values of a defined entry
     */
    p_DepthCue_tmp = (PEXDepthCueEntry *)PEXGetTableEntry(display, 
					luts[PEXLUTDepthCue], 
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
	_hppex_stderr_print
	    ( "Defined Depth Cue Entry returned incorrect data\n");
    }

    PEXFreeTableEntries (PEXLUTDepthCue, 1, p_DepthCue_tmp);

    /*
     * Get the set values of an undefined entry
     */
    p_DepthCue_tmp = (PEXDepthCueEntry *)PEXGetTableEntry(display, 
					luts[PEXLUTDepthCue], 
					 410, PEXSetValue, &entry_type, 
					 &table_type);

    if (!((entry_type == PEXDefaultEntry) &&
	  (table_type == PEXLUTDepthCue) &&
          (p_DepthCue_tmp->mode == PEXOff) &&
          (p_DepthCue_tmp->front_plane == 1.0) &&
          (p_DepthCue_tmp->back_plane == 0.0) &&
          (p_DepthCue_tmp->front_scaling == 1.0) &&
          (p_DepthCue_tmp->back_scaling == 0.5) &&
          (p_DepthCue_tmp->color.type == PEXColorTypeIndexed) &&
          (p_DepthCue_tmp->color.value.indexed.index == 0)))
    {
	_hppex_stderr_print
	    ( "Undefined Depth Cue Entry returned incorrect data\n");
    }

    PEXFreeTableEntries (PEXLUTDepthCue, 1, p_DepthCue_tmp);
    

    /*
     * Get a couple entries at once
     */
    status = PEXGetTableEntries (display, luts[PEXLUTDepthCue], 1099, 3,  
	                         PEXSetValue, &table_type, 
				 (PEXPointer *) &p_DepthCue_tmp);

    if (!((table_type == PEXLUTDepthCue) &&
          (p_DepthCue_tmp[0].mode == PEXOff) &&
          (p_DepthCue_tmp[0].front_plane == 1.0) &&
          (p_DepthCue_tmp[0].back_plane == 0.0) &&
          (p_DepthCue_tmp[0].front_scaling == 1.0) &&
          (p_DepthCue_tmp[0].back_scaling == 0.5) &&
          (p_DepthCue_tmp[0].color.type == PEXColorTypeIndexed) &&
          (p_DepthCue_tmp[0].color.value.indexed.index == 0) &&

          (p_DepthCue_tmp[1].mode == PEXOn) &&
          (p_DepthCue_tmp[1].front_plane == .5) &&
          (p_DepthCue_tmp[1].back_plane == .25) &&
          (p_DepthCue_tmp[1].front_scaling == 1.0) &&
          (p_DepthCue_tmp[1].back_scaling == 0.25) &&
          (p_DepthCue_tmp[1].color.type == PEXColorTypeRGB) &&
          (p_DepthCue_tmp[1].color.value.rgb.red == 1.0) &&
          (p_DepthCue_tmp[1].color.value.rgb.green == 0.5) &&
          (p_DepthCue_tmp[1].color.value.rgb.blue == 0.5) &&

          (p_DepthCue_tmp[2].mode == PEXOn) &&
          (p_DepthCue_tmp[2].front_plane == .5) &&
          (p_DepthCue_tmp[2].back_plane == .25) &&
          (p_DepthCue_tmp[2].front_scaling == 1.0) &&
          (p_DepthCue_tmp[2].back_scaling == 0.25) &&
          (p_DepthCue_tmp[2].color.type == PEXColorTypeIndexed) &&
          (p_DepthCue_tmp[2].color.value.indexed.index == 3)))
    {
	_hppex_stderr_print
	     ( "Get Edge Bundle Entries returned incorrect data\n");
    }

    PEXFreeTableEntries (PEXLUTDepthCue, 3, p_DepthCue_tmp);



    /*
     * Color Approx. Since, the color module checks this
     * functionality exhaustively and it is configuration dependent, no test is
     * added here.
     */

    /**********************************************************************
     * Test ends here
     *********************************************************************/
}

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "lut_edit");
    *num_images      = 1;
    *supported_modes = NON_OC_MODE;

    strcpy(win_title, "lut_edit");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    local_window = window;
    _hppex_set_stderr("lut_edit.stderr", "lut_edit");
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    describe_test( "\nPEXlib Lookup Table Edit Tests:\n\n" );
    describe_test( "This test checks out the various aspects of editing a lookup table. It\n");
    describe_test( "tests out entry setting, entry deletion, and entry inquiry.\n");
    describe_test( "\n");
    describe_test( "The basic mechanisms for all LUTs are the same. So, the mechanisms of \n");
    describe_test( "deletion and insertion are first checked out on a LineBundle LUT. The\n");
    describe_test( "remainder of this test just sets a few entries for each type of LUT and\n");
    describe_test( "then inquires the LUTs. These tests are designed to check the set/realized\n");
    describe_test( "semantics of entry setting, check the 2 entry inquiry entry points, and check\n");
    describe_test( "that all entry fields are set correctly and returned on inquiry. \n");
    describe_test( "\n");
    describe_test( "This test does not test Pattern LUTs which are not HP supported, and it does\n");
    describe_test( "not test Color Approx LUTs which are covered by the Color sub module testing.\n\n");
    describe_test( "The test is successful if no error messages are printed to stderr.\n\n");

    lut_test(dpy, local_window);
    _hppex_close_stderr();
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
    file_testproc("lut_edit.stderr", "lut_edit");
    }
