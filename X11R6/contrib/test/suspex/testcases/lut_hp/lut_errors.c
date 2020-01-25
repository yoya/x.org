/* $XConsortium: lut_errors.c,v 1.0 93/11/22 12:28:41 rws Exp $ */

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
 * lut_errors
 *
 * This test tests that we get the expected errors for bogus parameters
 *
 * Oct 29, 1992 	Ken Tidwell
 */
 
/*
 * Basic set of include files for tests
 */
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/PEX5/PEXlib.h>
#include "misc.h"
#include "err_handler.h"

Window local_window;

#define NUM_LUTS 12
#define NUM_ENTRIES (NUM_LUTS + 1)

void lut_test(Display *display,
	      Window   window)
{
    char *protocol_mode;
 
    /*************************************************************************
     * Test specific data
     ************************************************************************/
    PEXLookupTable luts[NUM_ENTRIES];     /* 12 lut types, 0'th entry ignored */
    PEXTableInfo lut_info[NUM_ENTRIES];	  /* 12 lut types, 0'th entry ignored */
    PEXPointer entries [NUM_ENTRIES];	  /* 12 lut types, 0'th entry ignored */
    PEXLookupTable color_lut, line_lut;
    PEXStructure str;
    PEXTableInfo table_info;
    PEXPointer predef_entries; 

    int i, j;
    int table_type;
    Status status;
    unsigned long return_count;
    PEXTableIndex *return_indices;

    PEXLineBundleEntry 		LineBundle;
    PEXMarkerBundleEntry 	MarkerBundle;
    PEXTextBundleEntry 		TextBundle;
    PEXInteriorBundleEntry 	InteriorBundle;
    PEXEdgeBundleEntry 		EdgeBundle;
    PEXColorEntry 		Color;
    PEXTextFontEntry 		TextFont;
    PEXLightEntry 		Light;
    PEXDepthCueEntry 		DepthCue;
    PEXColorApproxEntry 	ColorApprox;
    PEXViewEntry 		View;
    PEXPatternEntry             Pattern;

    int				err_mask = ERROR_CODE | REQUEST_CODE;

    /**********************************************************************
     * Test begins here
     *********************************************************************/

    /**********************************************************************
     * Test 1
     *
     * Create Lookup Table
     *********************************************************************/
    {
	PEXLookupTable lut;

	/*
         * Try bogus drawable
         */
	_hppex_stderr_print("Begin Test 1\n");
	_hppex_test_set_expected_error (X_ERR, BadDrawable,
                                      	PEXRCCreateLookupTable, 0, 0);
	lut = PEXCreateLookupTable (display, 254, PEXLUTColor);

        XSync(display, 0);
	_hppex_test_check_expected_error (err_mask);


	/*
         * Try bogus LUT type
         */
	_hppex_test_set_expected_error (X_ERR, BadValue,
                                      	PEXRCCreateLookupTable, 0, 0);
	lut = PEXCreateLookupTable (display, window, 17);

        XSync(display, 0); 
	_hppex_test_check_expected_error (err_mask);

	/*
         * Try unsuppored  LUT type
         */
	_hppex_test_set_expected_error (PEX_ERR, BadPEXLookupTable,
                                      	PEXRCCreateLookupTable, 0, 0);
	lut = PEXCreateLookupTable (display, window, PEXLUTPattern);

        XSync(display, 0);
	_hppex_test_check_expected_error (err_mask);
    }
    /**********************************************************************
     * Test 2
     *
     * Copy Lookup Table
     *********************************************************************/
    {

	color_lut = PEXCreateLookupTable (display, window, PEXLUTColor);
	line_lut = PEXCreateLookupTable (display, window, PEXLUTLineBundle);
	str = PEXCreateStructure (display);

	/*
         * Try bogus src ID
         */
	_hppex_stderr_print("Begin Test 2\n");
	_hppex_test_set_expected_error (PEX_ERR, BadPEXLookupTable,
                                      	PEXRCCopyLookupTable, 0, 0);
	PEXCopyLookupTable (display, 34786, line_lut);

        XSync(display, 0);
	_hppex_test_check_expected_error (err_mask);

	/*
         * Try bogus dest ID
         */
	_hppex_test_set_expected_error (PEX_ERR, BadPEXLookupTable,
                                      	PEXRCCopyLookupTable, 0, 0);
	PEXCopyLookupTable (display, color_lut, 374628);

        XSync(display, 0);
	_hppex_test_check_expected_error (err_mask);

	/*
         * Try non-lut src ID
         */
	_hppex_test_set_expected_error (PEX_ERR, BadPEXLookupTable,
                                      	PEXRCCopyLookupTable, 0, 0);
	PEXCopyLookupTable (display, color_lut, str);

        XSync(display, 0);
	_hppex_test_check_expected_error (err_mask);

	/*
         * Try non-lut dest ID
         */
	_hppex_test_set_expected_error (PEX_ERR, BadPEXLookupTable,
                                      	PEXRCCopyLookupTable, 0, 0);
	PEXCopyLookupTable (display, str, line_lut);
        XSync(display, 0);
	_hppex_test_check_expected_error (err_mask);

	/*
         * Try copy of different lut types
         */
	_hppex_test_set_expected_error (X_ERR, BadMatch,
                                      	PEXRCCopyLookupTable, 0, 0);
	PEXCopyLookupTable (display, color_lut, line_lut);
        XSync(display, 0);
	_hppex_test_check_expected_error (err_mask);
    }
    /**********************************************************************
     * Test 3
     *
     * Get Table Info
     *********************************************************************/
    {
	/*
         * Try bogus table type
         */
	_hppex_stderr_print("Begin Test 3\n");
	_hppex_test_set_expected_error (X_ERR, BadValue,
                                      	PEXRCGetTableInfo, 0, 0);
	status = PEXGetTableInfo (display, window, 42, &table_info);
        XSync(display, 0); 
	_hppex_test_check_expected_error (err_mask);

	/*
         * Try unsupported table type
         */
	_hppex_test_set_expected_error (PEX_ERR, BadPEXLookupTable,
                                      	PEXRCGetTableInfo, 0, 0);
	status = PEXGetTableInfo (display, window, PEXLUTPattern, &table_info);
        XSync(display, 0);
	_hppex_test_check_expected_error (err_mask);

	/*
   	 * Try a bogus drawable
	 */
	_hppex_test_set_expected_error (X_ERR, BadDrawable,
                                      	PEXRCGetTableInfo, 0, 0);
	status = PEXGetTableInfo (display, 34, PEXLUTColor, &table_info);
        XSync(display, 0);
	_hppex_test_check_expected_error (err_mask);

    }
    /**********************************************************************
     * Test 4
     *
     * Get Predefined Entries
     *********************************************************************/
    {
	/*
         * Try bogus table type
         */
	_hppex_stderr_print("Begin Test 4\n");
	_hppex_test_set_expected_error (X_ERR, BadValue,
                                      	PEXRCGetPredefinedEntries, 0, 0);
	status = PEXGetPredefinedEntries (display, window, 15, 
					  1,
					  1,
					  &predef_entries);
        XSync(display, 0);
	_hppex_test_check_expected_error (err_mask);

	/*
         * Try unsupported table type
         */
	_hppex_test_set_expected_error (PEX_ERR, BadPEXLookupTable,
                                      	PEXRCGetPredefinedEntries, 0, 0);
	status = PEXGetPredefinedEntries (display, window, PEXLUTPattern, 
					  1,
					  1,
					  &predef_entries);
        XSync(display, 0);
	_hppex_test_check_expected_error (err_mask);

	/*
   	 * Try a bogus drawable
	 */
	_hppex_test_set_expected_error (X_ERR, BadDrawable,
                                      	PEXRCGetPredefinedEntries, 0, 0);
	status = PEXGetPredefinedEntries (display, 312, PEXLUTColor, 
					  0,
					  8,
					  &predef_entries);

        XSync(display, 0);
	_hppex_test_check_expected_error (err_mask);

	/*
 	 * Try a bogus start
	 */
	_hppex_test_set_expected_error (X_ERR, BadValue,
                                      	PEXRCGetPredefinedEntries, 0, 0);
	status = PEXGetPredefinedEntries (display, window, PEXLUTLineBundle, 
					  0,
					  1,
					  &predef_entries);

        XSync(display, 0);
	_hppex_test_check_expected_error (err_mask);

	/*
	 * Try a bogus count
	 */
	_hppex_test_set_expected_error (X_ERR, BadValue,
                                      	PEXRCGetPredefinedEntries, 0, 0);
	status = PEXGetPredefinedEntries (display, window, PEXLUTColor, 
					  2,
					  9,
					  &predef_entries);

        XSync(display, 0);
	_hppex_test_check_expected_error (err_mask);
    }
    /**********************************************************************
     * Test 5
     *
     * Get Defined Indices
     *********************************************************************/
    {
	/*
         * Try bogus table ID
         */
	_hppex_stderr_print("Begin Test 5\n");
	_hppex_test_set_expected_error (PEX_ERR, BadPEXLookupTable,
                                      	PEXRCGetDefinedIndices, 0, 0);
	status = PEXGetDefinedIndices (display, 32314, 
				       &return_count, &return_indices);

        XSync(display, 0);
	_hppex_test_check_expected_error (err_mask);

	/*
         * Try non-lut ID
         */
	_hppex_test_set_expected_error (PEX_ERR, BadPEXLookupTable,
                                      	PEXRCGetDefinedIndices, 0, 0);
	status = PEXGetDefinedIndices (display, str, 
				       &return_count, &return_indices);
        XSync(display, 0);
	_hppex_test_check_expected_error (err_mask);
    }
    /**********************************************************************
     * Test 6
     *
     * Get Table Entry
     *********************************************************************/
    {
	int status_return, table_type_return;

	/*
         * Try bogus table type
         */
	_hppex_stderr_print("Begin Test 6\n");
	_hppex_test_set_expected_error (PEX_ERR, BadPEXLookupTable,
                                      	PEXRCGetTableEntry, 0, 0);
	predef_entries = PEXGetTableEntry (display, 34098, 1, PEXSetValue,
					   &status_return, &table_type_return);
        XSync(display, 0);
	_hppex_test_check_expected_error (err_mask);

	/*
         * Try non-lut ID
         */
	_hppex_test_set_expected_error (PEX_ERR, BadPEXLookupTable,
                                      	PEXRCGetTableEntry, 0, 0);
	predef_entries = PEXGetTableEntry (display, str, 1, PEXSetValue,
					   &status_return, &table_type_return);
        XSync(display, 0);
	_hppex_test_check_expected_error (err_mask);

	/*
         * Try bad index
         */
	_hppex_test_set_expected_error (X_ERR, BadValue,
                                      	PEXRCGetTableEntry, 0, 0);
	predef_entries = PEXGetTableEntry (display, line_lut, 0, PEXSetValue,
					   &status_return, &table_type_return);
        XSync(display, 0);
	_hppex_test_check_expected_error (err_mask);

	/*
	 * Try bad value type
 	 */
	_hppex_test_set_expected_error (X_ERR, BadValue,
                                      	PEXRCGetTableEntry, 0, 0);
	predef_entries = PEXGetTableEntry (display, color_lut, 1, 4,
					   &status_return, &table_type_return);
        XSync(display, 0);
	_hppex_test_check_expected_error (err_mask);
    }
    /**********************************************************************
     * Test 7
     *
     * Get Table Entries
     *********************************************************************/
    {
        int table_type_return;

        /*
         * Try bogus table type
         */
	_hppex_stderr_print("Begin Test 7\n");
        _hppex_test_set_expected_error (PEX_ERR, BadPEXLookupTable,
                                        PEXRCGetTableEntries, 0, 0);
        status = PEXGetTableEntries (display, 34098, 1, 1, PEXSetValue,
                                     &table_type_return, &predef_entries);
        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask);

        /*
         * Try non-lut ID
         */
        _hppex_test_set_expected_error (PEX_ERR, BadPEXLookupTable,
                                        PEXRCGetTableEntries, 0, 0);
        status = PEXGetTableEntries (display, str, 1, 1, PEXSetValue,
                                     &table_type_return, &predef_entries);
        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask);

        /*
         * Try bad start
         */
        _hppex_test_set_expected_error (X_ERR, BadValue,
                                        PEXRCGetTableEntries, 0, 0);
        status = PEXGetTableEntries (display, line_lut, 0, 1, PEXSetValue,
                                     &table_type_return, &predef_entries);
        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask);

        /*
         * Try bad count
         */

        /* Protocol mode does not test for this condition, since it doesn't
         * do error checking.
         */

     
        _hppex_test_set_expected_error (X_ERR, BadValue,
                                        PEXRCGetTableEntries, 0, 0);

       /* Protocol does not do error checking and converts this to 0. */

        status = PEXGetTableEntries (display, line_lut, 1, 65536, PEXSetValue,
                                     &table_type_return, &predef_entries);
        XSync(display, 0);

        if (strcmp("PEX", protocol_mode) != 0) 
           _hppex_test_check_expected_error (err_mask);

        /*
         * Try bad value type
         */
        _hppex_test_set_expected_error (X_ERR, BadValue,
                                        PEXRCGetTableEntries, 0, 0);
        status = PEXGetTableEntries (display, line_lut, 1, 1, 4,
                                     &table_type_return, &predef_entries);
        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask);

    }
    /**********************************************************************
     * Test 8
     *
     * Delete Table Entries
     *********************************************************************/
    {
        /*
         * Try bogus table ID
         */
	_hppex_stderr_print("Begin Test 8\n");
        _hppex_test_set_expected_error (PEX_ERR, BadPEXLookupTable,
                                        PEXRCDeleteTableEntries, 0, 0);
	PEXDeleteTableEntries (display, 738246, 1, 2);
        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask);

        /*
         * Try non-lut ID
         */
        _hppex_test_set_expected_error (PEX_ERR, BadPEXLookupTable,
                                        PEXRCDeleteTableEntries, 0, 0);
	PEXDeleteTableEntries (display, str, 1, 2);
        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask);

        /*
         * Try bad start
         */
        _hppex_test_set_expected_error (X_ERR, BadValue,
                                        PEXRCDeleteTableEntries, 0, 0);
	PEXDeleteTableEntries (display, line_lut, 0, 1);
        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask);

        /*
         * Try bad count
         */
        _hppex_test_set_expected_error (X_ERR, BadValue,
                                        PEXRCDeleteTableEntries, 0, 0);
	PEXDeleteTableEntries (display, color_lut, 1, 65535);
        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask);
    }
    /**********************************************************************
     * Test 9
     *
     * Free Lookup Table 
     *********************************************************************/
    {
        /*
         * Try bogus table ID
         */
	_hppex_stderr_print("Begin Test 9\n");
        _hppex_test_set_expected_error (PEX_ERR, BadPEXLookupTable,
                                        PEXRCFreeLookupTable, 0, 0);
	PEXFreeLookupTable (display, 837246);
        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask);

        /*
         * Try non-lut ID
         */
        _hppex_test_set_expected_error (PEX_ERR, BadPEXLookupTable,
                                        PEXRCFreeLookupTable, 0, 0);
	PEXFreeLookupTable (display, str);
        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask);
    }
    /**********************************************************************
     * Test 10
     *
     * Set Table Entries
     *********************************************************************/
    {
	LineBundle.type = PEXLineTypeSolid;
	LineBundle.interp_method = PEXPolylineInterpNone;
	LineBundle.curve_approx.method = PEXApproxImpDep;
	LineBundle.curve_approx.tolerance = 1.0;
	LineBundle.width = 1.0;
	LineBundle.color.type = PEXColorTypeIndexed;
	LineBundle.color.value.indexed.index = 2;

        /*
         * Try bogus table ID
         */
	_hppex_stderr_print("Begin Test 10\n");
        _hppex_test_set_expected_error (PEX_ERR, BadPEXLookupTable,
                                        PEXRCSetTableEntries, 0, 0);
	PEXSetTableEntries (display, 56478, 1, 1, PEXLUTLineBundle, 
			    &LineBundle);

        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask);


        /*
         * Try non-lut ID
         */
        _hppex_test_set_expected_error (PEX_ERR, BadPEXLookupTable,
                                        PEXRCSetTableEntries, 0, 0);
	PEXSetTableEntries (display, str, 1, 1, PEXLUTLineBundle, 
			    &LineBundle);
        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask);


        /*
         * Try bogus table type
         */
        _hppex_test_set_expected_error (PEX_ERR, BadPEXLookupTable,
                                        PEXRCSetTableEntries, 0, 0);
	PEXSetTableEntries (display, line_lut, 1, 1, 16, 
			    &LineBundle);
        XSync(display, 0); 

       /* This test doesn't work in protocol mode because the server
        * doesn't receive a table type in protocol mode.
        */

        if (strcmp("PEX", protocol_mode) != 0)
           _hppex_test_check_expected_error (err_mask);


        /*
         * Try unsupported table type
         */
        _hppex_test_set_expected_error (PEX_ERR, BadPEXLookupTable,
                                        PEXRCSetTableEntries, 0, 0);
	PEXSetTableEntries (display, line_lut, 1, 1, PEXLUTPattern, 
			    &Pattern);
        XSync(display, 0);
       
       /* This test will fail in protocol mode because the server
        * doesn't get a table type in the request.
        */

        if (strcmp("PEX", protocol_mode) != 0)
           _hppex_test_check_expected_error (err_mask);

	/*
 	 * Try bogus start
         */
        _hppex_test_set_expected_error (X_ERR, BadValue,
                                        PEXRCSetTableEntries, 0, 0);
	PEXSetTableEntries (display, line_lut, 0, 1, PEXLUTLineBundle, 
			    &LineBundle);
        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask);

	/*
	 * Try bogus count
	 */
        _hppex_test_set_expected_error (X_ERR, BadValue,
                                        PEXRCSetTableEntries, 0, 0);
	PEXSetTableEntries (display, line_lut, 65534, 3, PEXLUTLineBundle, 
			    &LineBundle);
        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask);

	/*
	 * Try bogus data in entry
	 */
	LineBundle.interp_method = 4;
        _hppex_test_set_expected_error (X_ERR, BadValue,
                                        PEXRCSetTableEntries, 0, 0);
	PEXSetTableEntries (display, line_lut, 1, 1, PEXLUTLineBundle, 
			    &LineBundle);

        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask);

	/*
	 * Try bogus color type
	 */
	LineBundle.interp_method = PEXPolylineInterpNone;
	LineBundle.color.type = PEXColorTypeHSV;
	LineBundle.color.value.hsv.hue = 0.8;
	LineBundle.color.value.hsv.saturation = 0.9;
	LineBundle.color.value.hsv.value = 0.5;
        _hppex_test_set_expected_error (PEX_ERR, BadPEXColorType,
                                        PEXRCSetTableEntries, 0, 0);
	PEXSetTableEntries (display, line_lut, 1, 1, PEXLUTLineBundle, 
			    &LineBundle);
        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask);
    }


    /**********************************************************************
     * Test ends here
     *********************************************************************/
}

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "lut_errors");
    *num_images      = 1;
    *supported_modes = NON_OC_MODE;

    strcpy(win_title, "lut_errors");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    local_window = window;
    _hppex_set_stderr("lut_errors.stderr", "lut_errors");
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    describe_test( "\nPEXlib Lookup Table Errors Tests:\n\n" );
    describe_test( "This test contains the following sub-tests:\n");
    describe_test( "  Test  1 PEXCreateLookupTable\n");
    describe_test( "  Test  2 PEXCopyLookupTable\n");
    describe_test( "  Test  3 PEXGetTableInfo\n");
    describe_test( "  Test  4 PEXGetPredefinedEntries\n");
    describe_test( "  Test  5 PEXGetDefinedIndices\n");
    describe_test( "  Test  6 PEXGetTableEntry\n");
    describe_test( "  Test  7 PEXGetTableEntries\n");
    describe_test( "  Test  8 PEXDeleteTableEntries\n");
    describe_test( "  Test  9 PEXFreeLookupTable\n");
    describe_test( "  Test 10 PEXSetTableEntries\n\n");
    describe_test( "This test is successful if no error messages are printed to stderr.\n\n");

    lut_test(dpy, local_window);
    _hppex_close_stderr();
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
    file_testproc("lut_errors.stderr", "lut_errors");
    }
