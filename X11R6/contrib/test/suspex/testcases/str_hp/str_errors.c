/* $XConsortium: str_errors.c,v 1.0 93/11/22 12:47:09 rws Exp $ */

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
#include <X11/PEX5/PEX.h>
#include <X11/PEX5/PEXlib.h>
#include "err_handler.h"
#include "misc.h"
#include "lut.h"

void struct_test(Display *display)
{
    /*
     * Test specific data
     */
    PEXStructure str_A, str_B, str_C, str_D, spare_1, spare_2;
    PEXLookupTable lut;
    PEXColor color;
    PEXCoord line[8];
    Status status;

    /*
     * Create some structures to work with
     */
    str_A = PEXCreateStructure (display);
    str_B = PEXCreateStructure (display);
    str_C = PEXCreateStructure (display);
    str_D = PEXCreateStructure (display);
    
    spare_1 = PEXCreateStructure (display);
    spare_2 = PEXCreateStructure (display);

    /* 
     * Just another resource to use in trying to confuse things
     */
    lut = colorLUT;

    /*
     * Fill out structures and build up network
     */
   
    /*
     * str_D
     */
    color.rgb.red = .3;
    color.rgb.green = .3;
    color.rgb.blue = .3;

    PEXSetLineColor (display, str_D, PEXOCStore, PEXColorTypeRGB, &color);
    PEXSetLineType (display, str_D, PEXOCStore, PEXLineTypeDashed);
    PEXLabel (display, str_D, PEXOCStore, 2);
    PEXSetLineWidth (display, str_D, PEXOCStore, 1.0);


    /*
     * str_C
     */
    PEXLabel (display, str_C, PEXOCStore, 1);
    PEXSetInteriorBundleIndex (display, str_C, PEXOCStore, 2);
    PEXSetSurfaceColorIndex (display, str_C, PEXOCStore, 3);
    PEXLabel (display, str_C, PEXOCStore, 1);
    PEXExecuteStructure (display, str_C, PEXOCStore, str_D);
    PEXSetInteriorStyle (display, str_C, PEXOCStore, PEXInteriorStyleSolid);


    /*
     * str_B
     */
    PEXSetMarkerScale (display, str_B, PEXOCStore, 3.0);
    PEXLabel (display, str_B, PEXOCStore, 1);
    PEXSetMarkerType (display, str_B, PEXOCStore, PEXMarkerCross);
    color.rgb.red = .5;
    color.rgb.green = .5;
    color.rgb.blue = .5;
    PEXSetMarkerColor (display, str_B, PEXOCStore, PEXColorTypeRGB, &color);
    PEXExecuteStructure (display, str_B, PEXOCStore, str_D);
    PEXLabel (display, str_B, PEXOCStore, 2);
 
    /*
     * str_A
     */
    PEXSetBFInteriorStyle (display, str_A, PEXOCStore, PEXInteriorStyleSolid);
    PEXSetBFReflectionModel (display, str_A, PEXOCStore, PEXReflectionAmbient);
    PEXLabel (display, str_A, PEXOCStore, 1);
    PEXExecuteStructure (display, str_A, PEXOCStore, str_C);
    PEXSetBFSurfaceColorIndex (display, str_A, PEXOCStore, 1);
    PEXSetCharHeight (display, str_A, PEXOCStore, .25);
    PEXLabel (display, str_A, PEXOCStore, 2);
    PEXExecuteStructure (display, str_A, PEXOCStore, str_B);
    PEXSetCharSpacing (display, str_A, PEXOCStore, .25);

    /*
     * Now we have a network, let's see what functionality works.
     */

    /**********************************************************************
     * Test 1
     *
     * PEXGetStructureInfo
     *********************************************************************/
    {
	unsigned long mask;
	unsigned long err_mask;
	PEXStructureInfo str_info;

	err_mask = ERROR_CODE | REQUEST_CODE;

	mask = PEXElementPtr | PEXNumElements | PEXLengthStructure | 
		PEXHasRefs | PEXEditMode;

       /*
        * Try bogus resource ID
        */
        _hppex_test_set_expected_error (PEX_ERR, BadPEXStructure, 
					PEXRCGetStructureInfo, 0, 0);
	status = PEXGetStructureInfo (display, 54353, PEXIEEE_754_32, mask, 
				      &str_info);

        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask);


        /*
         * Try non-structure resource
         */
        _hppex_test_set_expected_error (PEX_ERR, BadPEXStructure,
                                        PEXRCGetStructureInfo, 0, 0);
        status = PEXGetStructureInfo (display, lut, PEXIEEE_754_32, mask,
                                      &str_info);

        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask);



        /*
         * Try Bad Floating point format
         */
        _hppex_test_set_expected_error (PEX_ERR, BadPEXFloatingPointFormat,
                                        PEXRCGetStructureInfo, 0, 0);
	status = PEXGetStructureInfo (display, str_A, PEXDEC_F_Floating, mask, 
				      &str_info);
        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask);


	/*
         * Try bad bit in mask
         */
	mask |= 32;
        _hppex_test_set_expected_error (X_ERR, BadValue,
                                        PEXRCGetStructureInfo, 0, 0);
	status = PEXGetStructureInfo (display, str_A, PEXIEEE_754_32, mask, 
				      &str_info);
        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask);

    }

    /**********************************************************************
     * Test 2
     *
     * PEXGetStructuresInNetwork
     *********************************************************************/
    {
	PEXStructure *structs;
	unsigned long count;
	unsigned long err_mask = ERROR_CODE | REQUEST_CODE;

	PEXExecuteStructure (display, spare_1, PEXOCStore, str_C);

       /*
        * Try bogus resource ID
        */
        _hppex_test_set_expected_error (PEX_ERR, BadPEXStructure,
                                        PEXRCGetStructuresInNetwork, 0, 0);
	structs = PEXGetStructuresInNetwork (display, 12897, PEXAll, &count);

        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask);
        
       /*
        * Try non-structure ID
        */
        _hppex_test_set_expected_error (PEX_ERR, BadPEXStructure,
                                        PEXRCGetStructuresInNetwork, 0, 0);
	structs = PEXGetStructuresInNetwork (display, lut, PEXAll, &count);
 
        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask);
        
       /*
        * Try bogus "which"
        */
        _hppex_test_set_expected_error (X_ERR, BadValue,
                                        PEXRCGetStructuresInNetwork, 0, 0);
	structs = PEXGetStructuresInNetwork (display, str_A, 2, &count);

        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask);
        

    }

    /**********************************************************************
     * Test 3
     *
     * PEXGetAncestors
     *********************************************************************/
    {
	PEXStructurePath *paths;
	unsigned long count;
	unsigned long err_mask = ERROR_CODE | REQUEST_CODE;

       /*
        * Try bogus resource ID
        */
        _hppex_test_set_expected_error (PEX_ERR, BadPEXStructure,
                                        PEXRCGetAncestors, 0, 0);
	paths = PEXGetAncestors (display, 3487234, PEXTopPart, 0, &count);

        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask);

       /*
        * Try non-structure ID
        */
        _hppex_test_set_expected_error (PEX_ERR, BadPEXStructure,
                                        PEXRCGetAncestors, 0, 0);
	paths = PEXGetAncestors (display, lut, PEXTopPart, 0, &count);

        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask);

       /*
        * Try bogus "which"
        */
        _hppex_test_set_expected_error (X_ERR, BadValue,
                                        PEXRCGetAncestors, 0, 0);
	paths = PEXGetAncestors (display, str_D, 3, 0, &count);
 
        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask);


    }


    /**********************************************************************
     * Test 4
     *
     * PEXGetDescendants
     *********************************************************************/
    {
	PEXStructurePath *paths;
	unsigned long count;
	unsigned long err_mask = ERROR_CODE | REQUEST_CODE;

       /*
        * Try bogus resource ID
        */
        _hppex_test_set_expected_error (PEX_ERR, BadPEXStructure,
                                        PEXRCGetDescendants, 0, 0);
	paths = PEXGetDescendants (display, 98346, PEXTopPart, 0, &count);

        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask);

       /*
        * Try non-structure ID
        */
        _hppex_test_set_expected_error (PEX_ERR, BadPEXStructure,
                                        PEXRCGetDescendants, 0, 0);
	paths = PEXGetDescendants (display, lut, PEXTopPart, 0, &count);

        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask);

       /*
        * Try bogus "which"
        */
        _hppex_test_set_expected_error (X_ERR, BadValue,
                                        PEXRCGetDescendants, 0, 0);
	paths = PEXGetDescendants (display, str_A, 5, 0, &count);

        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask);

    }

    /**********************************************************************
     * Test 5
     *
     * PEXCopyStructure
     *********************************************************************/
     {
        unsigned long count;
        PEXStructure *structs;
        Status status2;
	unsigned long mask;
	PEXStructureInfo str_info1, str_info2;
	unsigned long err_mask = ERROR_CODE | REQUEST_CODE;

        PEXSetElementPtr (display, str_A, PEXBeginning, 3);

       /*
        * Try bogus src resource ID
        */
        _hppex_test_set_expected_error (PEX_ERR, BadPEXStructure,
                                        PEXRCCopyStructure, 0, 0);
        PEXCopyStructure (display, 43985, spare_2);

        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask);


       /*
        * Try bogus dest resource ID
        */
        _hppex_test_set_expected_error (PEX_ERR, BadPEXStructure,
                                        PEXRCCopyStructure, 0, 0);
        PEXCopyStructure (display, str_A, 3746);

        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask);


       /*
        * Try src non-structure ID
        */
        _hppex_test_set_expected_error (PEX_ERR, BadPEXStructure,
                                        PEXRCCopyStructure, 0, 0);
        PEXCopyStructure (display, lut, spare_2);

        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask);


       /*
        * Try dest non-structure ID
        */
        _hppex_test_set_expected_error (PEX_ERR, BadPEXStructure,
                                        PEXRCCopyStructure, 0, 0);
        PEXCopyStructure (display, str_A, lut);
        
        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask);

    }
    /**********************************************************************
     * Test 6
     *
     * PEXCopyElements
     *********************************************************************/
     {
	unsigned long mask;
	PEXStructureInfo str_info;
	PEXElementInfo *ele_info;
	unsigned long count;
	unsigned long err_mask = ERROR_CODE | REQUEST_CODE;

       /*
        * Try bogus src resource ID
        */
        _hppex_test_set_expected_error (PEX_ERR, BadPEXStructure,
                                        PEXRCCopyElements, 0, 0);
 	PEXCopyElements (display, 4582, PEXEnd, -4, PEXBeginning, 3, spare_2,
			 PEXCurrent, 1);

        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask);

       /*
        * Try bogus dest resource ID
        */
        _hppex_test_set_expected_error (PEX_ERR, BadPEXStructure,
                                        PEXRCCopyElements, 0, 0);
 	PEXCopyElements (display, str_A, PEXEnd, -4, PEXBeginning, 3, 8347,
			 PEXCurrent, 1);

        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask);

       /*
        * Try src non-structure ID
        */
        _hppex_test_set_expected_error (PEX_ERR, BadPEXStructure,
                                        PEXRCCopyElements, 0, 0);
 	PEXCopyElements (display, lut, PEXEnd, -4, PEXBeginning, 3, spare_2,
			 PEXCurrent, 1);

        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask);

       /*
        * Try dest non-structure ID
        */
        _hppex_test_set_expected_error (PEX_ERR, BadPEXStructure,
                                        PEXRCCopyElements, 0, 0);
 	PEXCopyElements (display, str_A, PEXEnd, -4, PEXBeginning, 3, lut,
			 PEXCurrent, 1);

        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask);


       /*
        * Try bogus src  1st "whence"
        */
        _hppex_test_set_expected_error (X_ERR, BadValue,
                                        PEXRCCopyElements, 0, 0);
 	PEXCopyElements (display, str_A, 3, -4, PEXBeginning, 3, spare_2,
			 PEXCurrent, 1);

        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask);

       /*
        * Try bogus src 2nd "whence"
        */
        _hppex_test_set_expected_error (X_ERR, BadValue,
                                        PEXRCCopyElements, 0, 0);
 	PEXCopyElements (display, str_A, PEXEnd, -4, 6, 3, spare_2,
			 PEXCurrent, 1);

        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask);

       /*
        * Try bogus src "whence"
        */
        _hppex_test_set_expected_error (X_ERR, BadValue,
                                        PEXRCCopyElements, 0, 0);
 	PEXCopyElements (display, str_A, PEXEnd, -4, PEXBeginning, 3, spare_2,
			 7, 1);

        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask);

     }
    /**********************************************************************
     * Test 7
     *
     * PEXSetEditingMode
     *********************************************************************/
     {
	unsigned long mask;
	PEXStructureInfo str_info;
	PEXElementInfo *ele_info;
	unsigned long count;
	unsigned long err_mask = ERROR_CODE | REQUEST_CODE;

       /*
        * Try bogus resource ID
        */
        _hppex_test_set_expected_error (PEX_ERR, BadPEXStructure,
                                        PEXRCSetEditingMode, 0, 0);
	PEXSetEditingMode (display, 8975, PEXStructureInsert);

        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask);

       /*
        * Try non-structure ID
        */
        _hppex_test_set_expected_error (PEX_ERR, BadPEXStructure,
                                        PEXRCSetEditingMode, 0, 0);
	PEXSetEditingMode (display, lut, PEXStructureInsert);

        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask);

       /*
        * Try bogus "mode"
        */
        _hppex_test_set_expected_error (X_ERR, BadValue,
                                        PEXRCSetEditingMode, 0, 0);
	PEXSetEditingMode (display, spare_2, 2);

        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask);

     }

    /**********************************************************************
     * Test 8
     *
     * PEXSetElementPtr
     *********************************************************************/
     {
        unsigned long mask;
        PEXStructureInfo str_info;
	unsigned long err_mask = ERROR_CODE | REQUEST_CODE;

	mask = PEXElementPtr;

       /*
        * Try bogus resource ID
        */
        _hppex_test_set_expected_error (PEX_ERR, BadPEXStructure,
                                        PEXRCSetElementPointer, 0, 0);
        PEXSetElementPtr (display, 9876, PEXBeginning, 0);


        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask);

       /*
        * Try non-structure ID
        */
        _hppex_test_set_expected_error (PEX_ERR, BadPEXStructure,
                                        PEXRCSetElementPointer, 0, 0);
        PEXSetElementPtr (display, lut, PEXBeginning, 0);

        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask);


       /*
        * Try bogus "whence"
        */
        _hppex_test_set_expected_error (X_ERR, BadValue,
                                        PEXRCSetElementPointer, 0, 0);
        PEXSetElementPtr (display, spare_2, 4, 0);

        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask);

     }
    /**********************************************************************
     * Test 9
     *
     * PEXSetElementPtrAtLabel 
     *********************************************************************/
    {
        unsigned long mask, err_mask;
        PEXStructureInfo str_info;
         
	mask = PEXElementPtr;
	err_mask = ERROR_CODE | REQUEST_CODE;

        PEXSetElementPtr (display, str_A, PEXBeginning, 0);

       /*
        * Try bogus resource ID
        */
        _hppex_test_set_expected_error (PEX_ERR, BadPEXStructure,
                                        PEXRCSetElementPointerAtLabel, 0, 0);
        PEXSetElementPtrAtLabel (display, 98623, 1, 0);

        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask);


       /*
        * Try non-structure ID
        */
        _hppex_test_set_expected_error (PEX_ERR, BadPEXStructure,
                                        PEXRCSetElementPointerAtLabel, 0, 0);
        PEXSetElementPtrAtLabel (display, lut, 1, 0);

        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask);

        /*
         * Try to find label 1, offset 1 (should fail)
         */
        PEXSetElementPtr (display, str_A, PEXBeginning, 5);

        _hppex_test_set_expected_error (PEX_ERR, BadPEXLabel, 
					PEXRCSetElementPointerAtLabel, 0, 0);
        PEXSetElementPtrAtLabel (display, str_A, 1, 1);

        XSync(display, 0);
	_hppex_test_check_expected_error (err_mask);


    }
    /**********************************************************************
     * Test 10
     *
     * PEXElementSearch
     *********************************************************************/
     {
	unsigned long offset;
	unsigned short incl_list[5], excl_list[5];
	unsigned long err_mask = ERROR_CODE | REQUEST_CODE;

	incl_list[0] = PEXOCMarkerType;
	incl_list[1] = PEXOCCharHeight;
	incl_list[2] = PEXOCPolyline;

	excl_list[0] = PEXOCLabel;
	excl_list[1] = PEXOCExecuteStructure;

       /*
        * Try bogus resource ID
        */
        _hppex_test_set_expected_error (PEX_ERR, BadPEXStructure,
                                        PEXRCElementSearch, 0, 0);
	status = PEXElementSearch (display, 18264, PEXBeginning, 0, PEXForward,
			           3, incl_list, 2, excl_list, &offset);

        XSync(display, 0);
	_hppex_test_check_expected_error (err_mask);

       /*
        * Try non-structure ID
        */
        _hppex_test_set_expected_error (PEX_ERR, BadPEXStructure,
                                        PEXRCElementSearch, 0, 0);
	status = PEXElementSearch (display, lut, PEXBeginning, 0, PEXForward,
			           3, incl_list, 2, excl_list, &offset);

        XSync(display, 0);
	_hppex_test_check_expected_error (err_mask);
			  
       /*
        * Try bogus "whence"
        */
        _hppex_test_set_expected_error (X_ERR, BadValue,
                                        PEXRCElementSearch, 0, 0);
	status = PEXElementSearch (display, str_A, 21, 0, PEXForward,
			           3, incl_list, 2, excl_list, &offset);

        XSync(display, 0);
	_hppex_test_check_expected_error (err_mask);
			  
       /*
        * Try bogus "direction"
        */
        _hppex_test_set_expected_error (X_ERR, BadValue,
                                        PEXRCElementSearch, 0, 0);
	status = PEXElementSearch (display, str_A, PEXBeginning, 0, 3,
			           3, incl_list, 2, excl_list, &offset);

        XSync(display, 0);
	_hppex_test_check_expected_error (err_mask);
			  

     }
    /**********************************************************************
     * Test 11
     *
     * PEXChangeStructureRefs
     *********************************************************************/
     {
	PEXStructurePath *path1, *path2;
	unsigned long count1, count2;
	unsigned long err_mask = ERROR_CODE | REQUEST_CODE;

       /*
        * Try bogus old resource ID
        */
        _hppex_test_set_expected_error (PEX_ERR, BadPEXStructure,
                                        PEXRCChangeStructureRefs, 0, 0);
	PEXChangeStructureRefs (display, 32, spare_1);
        XSync(display, 0);
	_hppex_test_check_expected_error (err_mask);

       /*
        * Try bogus new resource ID
        */
        _hppex_test_set_expected_error (PEX_ERR, BadPEXStructure,
                                        PEXRCChangeStructureRefs, 0, 0);
	PEXChangeStructureRefs (display, str_D, 456);

        XSync(display, 0);
	_hppex_test_check_expected_error (err_mask);

       /*
        * Try non-structure old resource ID
        */
        _hppex_test_set_expected_error (PEX_ERR, BadPEXStructure,
                                        PEXRCChangeStructureRefs, 0, 0);
	PEXChangeStructureRefs (display, lut, spare_1);

        XSync(display, 0);
	_hppex_test_check_expected_error (err_mask);

       /*
        * Try non-structure new resource ID
        */
        _hppex_test_set_expected_error (PEX_ERR, BadPEXStructure,
                                        PEXRCChangeStructureRefs, 0, 0);
	PEXChangeStructureRefs (display, str_D, lut);

        XSync(display, 0);
	_hppex_test_check_expected_error (err_mask);

     }
    /**********************************************************************
     * Test 12
     *
     * PEXDeleteElements
     *********************************************************************/
     {
	PEXStructureInfo str_info;
	PEXElementInfo *ele_info;
	unsigned long mask, count;
	Status status2;
	unsigned long err_mask = ERROR_CODE | REQUEST_CODE;

	PEXSetElementPtr (display, str_B, PEXBeginning, 3);

       /*
        * Try bogus resource ID
        */
        _hppex_test_set_expected_error (PEX_ERR, BadPEXStructure,
                                        PEXRCDeleteElements, 0, 0);
	PEXDeleteElements (display, 3476823, PEXCurrent, -1, PEXEnd, -1);

        XSync(display, 0);
	_hppex_test_check_expected_error (err_mask);


       /*
        * Try non-structure old resource ID
        */
        _hppex_test_set_expected_error (PEX_ERR, BadPEXStructure,
                                        PEXRCDeleteElements, 0, 0);
	PEXDeleteElements (display, lut, PEXCurrent, -1, PEXEnd, -1);

        XSync(display, 0);
	_hppex_test_check_expected_error (err_mask);


       /*
        * Try bogus "whence1"
        */
        _hppex_test_set_expected_error (X_ERR, BadValue,
                                        PEXRCDeleteElements, 0, 0);
	PEXDeleteElements (display, str_B, 5, -1, PEXEnd, -1);

        XSync(display, 0);
	_hppex_test_check_expected_error (err_mask);

       /*
        * Try bogus "whence"
        */
        _hppex_test_set_expected_error (X_ERR, BadValue,
                                        PEXRCDeleteElements, 0, 0);
	PEXDeleteElements (display, str_B, PEXCurrent, -1, 7, -1);
        XSync(display, 0);
	_hppex_test_check_expected_error (err_mask);

     }
    /**********************************************************************
     * Test 13
     *
     * PEXDeleteBetweenLabels
     *********************************************************************/
    {
	PEXStructureInfo str_info;
	PEXElementInfo *ele_info;
	unsigned long mask, count;
	Status status2;
	unsigned long err_mask = ERROR_CODE | REQUEST_CODE;

       /*
        * Try bogus resource ID
        */
        _hppex_test_set_expected_error (PEX_ERR, BadPEXStructure,
                                        PEXRCDeleteBetweenLabels, 0, 0);
	PEXDeleteBetweenLabels (display,  9874, 1, 2);

        XSync(display, 0);
	_hppex_test_check_expected_error (err_mask);

       /*
        * Try non-structure resource ID
        */
        _hppex_test_set_expected_error (PEX_ERR, BadPEXStructure,
                                        PEXRCDeleteBetweenLabels, 0, 0);
	PEXDeleteBetweenLabels (display,  lut, 1, 2);

        XSync(display, 0);
	_hppex_test_check_expected_error (err_mask);

	/*
         * Try to delete with pointer on first label (should fail)
         */
        PEXSetElementPtr (display, str_A, PEXBeginning, 3);


        _hppex_test_set_expected_error (PEX_ERR, BadPEXLabel, 
					PEXRCDeleteBetweenLabels, 0, 0);
	PEXDeleteBetweenLabels (display,  str_A, 1, 2);

        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask); 


	/*
         * Try Delete between valid first label and non-existent 2nd
         */
        PEXSetElementPtr (display, str_A, PEXCurrent, -1);

        _hppex_test_set_expected_error (PEX_ERR, BadPEXLabel, 
					PEXRCDeleteBetweenLabels, 0, 0);
	PEXDeleteBetweenLabels (display,  str_A, 1, 4);

        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask); 

    }
    /**********************************************************************
     * Test 14
     *
     * PEXDeleteToLabel
     *********************************************************************/
    {
	PEXStructureInfo str_info;
	PEXElementInfo *ele_info;
	unsigned long mask, count;
	Status status2;
	unsigned long err_mask = ERROR_CODE | REQUEST_CODE;

       /*
        * Try bogus resource ID
        */
        _hppex_test_set_expected_error (PEX_ERR, BadPEXStructure,
                                        PEXRCDeleteElementsToLabel, 0, 0);
	PEXDeleteToLabel (display,  83746, PEXEnd, -1, 2);

        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask); 

       /*
        * Try non-structure resource ID
        */
        _hppex_test_set_expected_error (PEX_ERR, BadPEXStructure,
                                        PEXRCDeleteElementsToLabel, 0, 0);
	PEXDeleteToLabel (display,  lut, PEXEnd, -1, 2);

        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask); 

       /*
        * Try bogus "whence"
        */
        _hppex_test_set_expected_error (X_ERR, BadValue,
                                        PEXRCDeleteElementsToLabel, 0, 0);
	PEXDeleteToLabel (display,  str_D, 45, -1, 2);


        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask); 

	/*
	 * Try deleting from label (should fail)
         */
	PEXSetElementPtr (display, str_D, PEXBeginning, 2);

        _hppex_test_set_expected_error (PEX_ERR, BadPEXLabel, 
					PEXRCDeleteElementsToLabel, 0, 0);
	PEXDeleteToLabel (display,  str_D, PEXEnd, -1, 2);

        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask); 

    }
    /**********************************************************************
     * Test 15
     *
     * PEXGetElementInfo
     *********************************************************************/
    {
	unsigned long mask;
	PEXStructureInfo str_info;
	PEXElementInfo *ele_info;
	unsigned long count;
	unsigned long err_mask = ERROR_CODE | REQUEST_CODE;

	mask = PEXElementPtr | PEXNumElements | PEXEditMode;

       /*
        * Try bogus resource ID
        */
        _hppex_test_set_expected_error (PEX_ERR, BadPEXStructure,
                                        PEXRCGetElementInfo, 0, 0);
        PEXGetElementInfo (display, 15234, PEXBeginning, 0, PEXEnd,
                           0,  PEXIEEE_754_32, &count, &ele_info);

        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask); 


       /*
        * Try non-structure resource ID
        */
        _hppex_test_set_expected_error (PEX_ERR, BadPEXStructure,
                                        PEXRCGetElementInfo, 0, 0);
        PEXGetElementInfo (display, lut, PEXBeginning, 0, PEXEnd,
                           0,  PEXIEEE_754_32, &count, &ele_info);

        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask); 


       /*
        * Try bogus "fp format"
        */
        _hppex_test_set_expected_error (PEX_ERR, BadPEXFloatingPointFormat,
                                        PEXRCGetElementInfo, 0, 0);
        PEXGetElementInfo (display, spare_2, PEXBeginning, 0, PEXEnd,
                           0,  PEXDEC_D_Floating, &count, &ele_info);

        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask); 


       /*
        * Try bogus "whence1"
        */
        _hppex_test_set_expected_error (X_ERR, BadValue,
                                        PEXRCGetElementInfo, 0, 0);
        PEXGetElementInfo (display, spare_2, 32, 0, PEXEnd,
                           0,  PEXIEEE_754_32, &count, &ele_info);

        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask); 


       /*
        * Try bogus "whence2"
        */
        _hppex_test_set_expected_error (X_ERR, BadValue,
                                        PEXRCGetElementInfo, 0, 0);
        PEXGetElementInfo (display, spare_2, PEXBeginning, 0, -76,
                           0,  PEXIEEE_754_32, &count, &ele_info);


        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask); 

    }


    /**********************************************************************
     * Test 16
     *
     * PEXDestroyStructures
     *********************************************************************/
    {
	PEXStructurePath *path1, *path2;
	PEXStructure structs[6];
	unsigned long mask, count1, count2;
	PEXStructureInfo str_info;
	unsigned long err_mask = ERROR_CODE | REQUEST_CODE;

       /*
        * Try bogus resource ID
        */
        _hppex_test_set_expected_error (PEX_ERR, BadPEXStructure,
                                        PEXRCDestroyStructures, 0, 0);
	structs[0] = 42;
	PEXDestroyStructures (display, 1, structs);

        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask); 

       /*
        * Try non_structure resource ID
        */
        _hppex_test_set_expected_error (PEX_ERR, BadPEXStructure,
                                        PEXRCDestroyStructures, 0, 0);
	structs[0] = lut;
	PEXDestroyStructures (display, 1, structs);

        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask); 

    }
}


void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "str_errors");
    *num_images      = 1;
    *supported_modes = NON_OC_MODE;

    strcpy(win_title, "str_errors");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    _hppex_set_stderr("str_errors.stderr", "str_errors");
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    describe_test( "\nPEXlib Structures Errors Tests:\n\n" );
    describe_test( "This test contains the following sub-tests:\n");
    describe_test( "  Test  1 PEXGetStructureInfo\n");
    describe_test( "  Test  2 PEXGetStructuresInNetwork\n");
    describe_test( "  Test  3 PEXGetAncestors\n");
    describe_test( "  Test  4 PEXGetDescendants\n");
    describe_test( "  Test  5 PEXCopyStructure\n");
    describe_test( "  Test  6 PEXCopyElements\n");
    describe_test( "  Test  7 PEXSetEditingMode\n");
    describe_test( "  Test  8 PEXSetElementPtr\n");
    describe_test( "  Test  9 PEXSetElementPtrAtLabel\n");
    describe_test( "  Test 10 PEXElementSearch\n");
    describe_test( "  Test 11 PEXChangeStructureRefs\n");
    describe_test( "  Test 12 PEXDeleteElements\n");
    describe_test( "  Test 13 PEXDeleteBetweenLabels\n");
    describe_test( "  Test 14 PEXDeleteToLabel\n");
    describe_test( "  Test 15 PEXCopyElements to self\n");
    describe_test( "  Test 16 PEXDestroyStructures\n\n");
    describe_test( "This test is successful if no error messages are printed to stderr.\n\n");

    struct_test(dpy);
    _hppex_close_stderr();
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
    file_testproc("str_errors.stderr", "str_errors");
    }
