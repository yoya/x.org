/* $XConsortium: str_basic.c,v 1.0 93/11/22 12:47:03 rws Exp $ */

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
 * Basic set of include files for tests
 */
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/PEX5/PEX.h>
#include <X11/PEX5/PEXlib.h>
#include "err_handler.h"
#include "misc.h"

int struct_lists_are_equivalent(int           count,
				PEXStructure *list_1,
				PEXStructure *list_2)
{
    int i,j;
    int lists_equal = True;

    for (i = 0; i < count; i++)
    {
	for (j = 0; j < count; j++)
	{
	    if (list_1[i] == list_2[j])
		break;
	}
	if (j == count)
	{
	    lists_equal = False;
	    break;
	}
    }
    return lists_equal;
}

int path_lists_are_equivalent(int               count,
			      PEXStructurePath *list_1,
			      PEXStructurePath *list_2)
{
    int i,j,k;
    int match_count = 0;
    PEXStructurePath p1, p2;

    for (i = 0; i < count; i++)
    {
        p1 = list_1[i];
	for (j = 0; j < count; j++)
	{
            p2 = list_2[j];
	    if (p1.count == p2.count)
	    {
	        for (k = 0; k < p1.count; k++)
		{
		    if ((p1.elements[k].structure!=p2.elements[k].structure) ||
		        (p1.elements[k].offset   !=p2.elements[k].offset))
			break;
		}
		if (k == p1.count)
		{
		    match_count++;
		    j = count;
		    break;
		}
	    }
	}
    }
    return (match_count == count);
}

void struct_test(Display *display)
{
    /*
     * Test specific data
     */
    PEXStructure str_A, str_B, str_C, str_D, spare_1, spare_2;
    PEXColor color;
    PEXCoord line[8];
    Status status;

    /**********************************************************************
     * Test begins here
     *********************************************************************/

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
     * Now we have a network, let's see functionality works.
     */

    /**********************************************************************
     * Test 1
     *
     * PEXGetStructureInfo
     *********************************************************************/
    {
	unsigned long mask;
	PEXStructureInfo str_info;

	mask = 0;
	mask = PEXElementPtr | PEXNumElements | PEXLengthStructure | 
		PEXHasRefs | PEXEditMode;

	status = PEXGetStructureInfo (display, str_A, PEXIEEE_754_32, mask, 
				      &str_info);

	if (! ((status) &&
	       (str_info.element_count == 9) &&
	       (str_info.has_refs == False) &&
	       (str_info.edit_mode == PEXStructureInsert) &&
	       (str_info.element_pointer == 9) &&
	       (str_info.size == 18) ))
	{
	    _hppex_stderr_print
	       ( "Test 1: Get Info on str A returned bogus data\n");
	}

    }

    /**********************************************************************
     * Test 2
     *
     * PEXGetStructuresInNetwork
     *********************************************************************/
    {
	PEXStructure *structs;
	PEXStructure expected_structs[4];
	unsigned long count;

	expected_structs[0] = str_A;
	expected_structs[1] = str_B;
	expected_structs[2] = str_C;
	expected_structs[3] = str_D;

	PEXExecuteStructure (display, spare_1, PEXOCStore, str_C);

	structs = PEXGetStructuresInNetwork (display, str_A, PEXAll, &count);
        
        if (! (struct_lists_are_equivalent (4, structs, expected_structs)))
	{
	    _hppex_stderr_print
	       ( "Test 2a: Get Stucts in Network returned bogus data\n");
	}
        XFree (structs);
		
	structs = PEXGetStructuresInNetwork(display, str_A, PEXOrphans, &count);
        
        if (! (struct_lists_are_equivalent (2, structs, expected_structs)))
	{
	    _hppex_stderr_print
	       ( "Test 2b: Get Stucts in Network returned bogus data\n");
	}
        XFree (structs);
		

    }

    /**********************************************************************
     * Test 3
     *
     * PEXGetAncestors
     *********************************************************************/
    {
	PEXStructurePath *paths;
	PEXStructurePath expected_paths[3];
	PEXElementRef ele1[3], ele2[3], ele3[3];
	unsigned long count;

        expected_paths[0].elements = ele1;
        expected_paths[1].elements = ele2;
        expected_paths[2].elements = ele3;

        expected_paths[0].count = 3;
        expected_paths[0].elements[0].structure = str_A;
        expected_paths[0].elements[0].offset = 8;
        expected_paths[0].elements[1].structure = str_B;
        expected_paths[0].elements[1].offset = 5;
        expected_paths[0].elements[2].structure = str_D;
        expected_paths[0].elements[2].offset = 0;

        expected_paths[1].count = 3;
        expected_paths[1].elements[0].structure = spare_1;
        expected_paths[1].elements[0].offset = 1;
        expected_paths[1].elements[1].structure = str_C;
        expected_paths[1].elements[1].offset = 5;
        expected_paths[1].elements[2].structure = str_D;
        expected_paths[1].elements[2].offset = 0;

        expected_paths[2].count = 3;
        expected_paths[2].elements[0].structure = str_A;
        expected_paths[2].elements[0].offset = 4;
        expected_paths[2].elements[1].structure = str_C;
        expected_paths[2].elements[1].offset = 5;
        expected_paths[2].elements[2].structure = str_D;
        expected_paths[2].elements[2].offset = 0;

        /*
         * 3a Top part All
         */	
	paths = PEXGetAncestors (display, str_D, PEXTopPart, 0, &count);

        if (! (path_lists_are_equivalent (3, paths, expected_paths)))
	{
	    _hppex_stderr_print
	       ( "Test 3a: Get All Ancestors (top) returned bogus data\n");
	}
	PEXFreeStructurePaths (count, paths);


	/*
         * 3b Top part (roots)
         */
	paths = PEXGetAncestors (display, str_D, PEXTopPart, 1, &count);

        expected_paths[0].count = 1;
        expected_paths[0].elements[0].structure = str_A;
        expected_paths[0].elements[0].offset = 8;
        expected_paths[1].count = 1;
        expected_paths[1].elements[0].structure = spare_1;
        expected_paths[1].elements[0].offset = 1;

        expected_paths[2].count = 1;
        expected_paths[2].elements[0].structure = str_A;
        expected_paths[2].elements[0].offset = 4;

        if (! (path_lists_are_equivalent (3, paths, expected_paths)))
	{
	    _hppex_stderr_print
	       ( "Test 3b: Get Ancestors (roots) returned bogus data\n");
	}
	PEXFreeStructurePaths (count, paths);


        /*
         * 3c Bottom part All
         */	
	paths = PEXGetAncestors (display, str_D, PEXBottomPart, 0, &count);

        expected_paths[0].count = 3;
        expected_paths[0].elements[0].structure = str_A;
        expected_paths[0].elements[0].offset = 8;
        expected_paths[0].elements[1].structure = str_B;
        expected_paths[0].elements[1].offset = 5;
        expected_paths[0].elements[2].structure = str_D;
        expected_paths[0].elements[2].offset = 0;

        expected_paths[1].count = 3;
        expected_paths[1].elements[0].structure = spare_1;
        expected_paths[1].elements[0].offset = 1;
        expected_paths[1].elements[1].structure = str_C;
        expected_paths[1].elements[1].offset = 5;
        expected_paths[1].elements[2].structure = str_D;
        expected_paths[1].elements[2].offset = 0;

        expected_paths[2].count = 3;
        expected_paths[2].elements[0].structure = str_A;
        expected_paths[2].elements[0].offset = 4;
        expected_paths[2].elements[1].structure = str_C;
        expected_paths[2].elements[1].offset = 5;
        expected_paths[2].elements[2].structure = str_D;
        expected_paths[2].elements[2].offset = 0;

        if (! (path_lists_are_equivalent (3, paths, expected_paths)))
	{
	    _hppex_stderr_print
	       ( "Test 3c: Get All Ancestors (bottom) returned bogus data\n");
	}
	PEXFreeStructurePaths (count, paths);


	/*
         * 3d Bottom part (partial)
         */
	paths = PEXGetAncestors (display, str_D, PEXBottomPart, 2, &count);

        expected_paths[0].count = 2;
        expected_paths[0].elements[0].structure = str_B;
        expected_paths[0].elements[0].offset = 5;
        expected_paths[0].elements[1].structure = str_D;
        expected_paths[0].elements[1].offset = 0;

        expected_paths[1].count = 2;
        expected_paths[1].elements[0].structure = str_C;
        expected_paths[1].elements[0].offset = 5;
        expected_paths[1].elements[1].structure = str_D;
        expected_paths[1].elements[1].offset = 0;

        if (! (path_lists_are_equivalent (2, paths, expected_paths)))
	{
	    _hppex_stderr_print
	       ( "Test 3d: Get All Ancestors (bottom) returned bogus data\n");
	}
	PEXFreeStructurePaths (count, paths);
    }


    /**********************************************************************
     * Test 4
     *
     * PEXGetDescendants
     *********************************************************************/
    {
	PEXStructurePath *paths;
	PEXStructurePath expected_paths[2];
	PEXElementRef ele1[3], ele2[3];
	unsigned long count;

        expected_paths[0].elements = ele1;
        expected_paths[1].elements = ele2;

        /*
         * 4a Top part All
         */	
	paths = PEXGetDescendants (display, str_A, PEXTopPart, 0, &count);

        expected_paths[0].count = 3;
        expected_paths[0].elements[0].structure = str_A;
        expected_paths[0].elements[0].offset = 4;
        expected_paths[0].elements[1].structure = str_C;
        expected_paths[0].elements[1].offset = 5;
        expected_paths[0].elements[2].structure = str_D;
        expected_paths[0].elements[2].offset = 0;

        expected_paths[1].count = 3;
        expected_paths[1].elements[0].structure = str_A;
        expected_paths[1].elements[0].offset = 8;
        expected_paths[1].elements[1].structure = str_B;
        expected_paths[1].elements[1].offset = 5;
        expected_paths[1].elements[2].structure = str_D;
        expected_paths[1].elements[2].offset = 0;

        if (! (path_lists_are_equivalent (2, paths, expected_paths)))
	{
	    _hppex_stderr_print
	       ( "Test 4a: Get All Descendants (top) returned bogus data\n");
	}
	PEXFreeStructurePaths (count, paths);


	/*
         * 4b Top part 
         */
	paths = PEXGetDescendants (display, str_A, PEXTopPart, 2, &count);

        expected_paths[0].count = 2;
        expected_paths[0].elements[0].structure = str_A;
        expected_paths[0].elements[0].offset = 4;
        expected_paths[0].elements[1].structure = str_C;
        expected_paths[0].elements[1].offset = 5;

        expected_paths[1].count = 2;
        expected_paths[1].elements[0].structure = str_A;
        expected_paths[1].elements[0].offset = 8;
        expected_paths[1].elements[1].structure = str_B;
        expected_paths[1].elements[1].offset = 5;

        if (! (path_lists_are_equivalent (2, paths, expected_paths)))
	{
	    _hppex_stderr_print
	       ( "Test 4b: Get Descendants (top) returned bogus data\n");
	}
	PEXFreeStructurePaths (count, paths);


        /*
         * 4c Bottom part All
         */	
	paths = PEXGetDescendants (display, str_A, PEXBottomPart, 0, &count);

        expected_paths[0].count = 3;
        expected_paths[0].elements[0].structure = str_A;
        expected_paths[0].elements[0].offset = 4;
        expected_paths[0].elements[1].structure = str_C;
        expected_paths[0].elements[1].offset = 5;
        expected_paths[0].elements[2].structure = str_D;
        expected_paths[0].elements[2].offset = 0;

        expected_paths[1].count = 3;
        expected_paths[1].elements[0].structure = str_A;
        expected_paths[1].elements[0].offset = 8;
        expected_paths[1].elements[1].structure = str_B;
        expected_paths[1].elements[1].offset = 5;
        expected_paths[1].elements[2].structure = str_D;
        expected_paths[1].elements[2].offset = 0;

        if (! (path_lists_are_equivalent (2, paths, expected_paths)))
	{
	    _hppex_stderr_print
	       ( "Test 4c: Get All Descendants (bottom) returned bogus data\n");
	}
	PEXFreeStructurePaths (count, paths);


	/*
         * 4d Bottom part (partial)
         */
	paths = PEXGetDescendants (display, str_A, PEXBottomPart, 1, &count);

        expected_paths[0].count = 1;
        expected_paths[0].elements[0].structure = str_D;
        expected_paths[0].elements[0].offset = 0;

        if (! (path_lists_are_equivalent (1, paths, expected_paths)))
	{
	    _hppex_stderr_print
	       ( "Test 4d: Get All Descendants (leafs) returned bogus data\n");
	}
	PEXFreeStructurePaths (count, paths);
    }

    /**********************************************************************
     * Test 5
     *
     * PEXCopyStructure
     *********************************************************************/
     {
     PEXStructure expected_structs[4];

	/*
         * 5a Copy to an empty structure
         */
        unsigned long count;
        PEXStructure *structs;
        Status status2;
	unsigned long mask;
	PEXStructureInfo str_info1, str_info2;

        PEXSetElementPtr (display, str_A, PEXBeginning, 3);
        PEXCopyStructure (display, str_A, spare_2);

	mask = 0;
	mask = PEXElementPtr | PEXNumElements | PEXLengthStructure | 
		PEXHasRefs | PEXEditMode;

	status = PEXGetStructureInfo (display, str_A, PEXIEEE_754_32, mask, 
				      &str_info1);

	status2 = PEXGetStructureInfo (display, spare_2, PEXIEEE_754_32, mask, 
				      &str_info2);

        if (! ((status) &&
	       (status2) &&
	       (status == status2) &&
	       (str_info1.element_count == str_info2.element_count) &&
	       (str_info1.edit_mode == str_info2.edit_mode) &&
	       (str_info1.element_pointer == str_info2.element_pointer) &&
	       (str_info1.size == str_info2.size) ))
	{
	    _hppex_stderr_print( "Test 5a: Copy structure failed\n");
	}
	       
	structs = PEXGetStructuresInNetwork (display, spare_2, PEXAll, &count);
        
        expected_structs[0] = spare_2;
        expected_structs[1] = str_B;
        expected_structs[2] = str_D;
        expected_structs[3] = str_C;

        if (! (struct_lists_are_equivalent (4, structs, expected_structs)))
	{
	    _hppex_stderr_print( "Test 5a: Copy Structs corrupted hierarchy\n");
	}
	XFree (structs);

	/*
         * 5b Copy to a non-empty structure
         */
        PEXSetElementPtr (display, str_B, PEXBeginning, 3);
	PEXSetEditingMode (display, str_B, PEXStructureReplace);
        PEXCopyStructure (display, str_B, spare_2);

	mask = 0;
	mask = PEXElementPtr | PEXNumElements | PEXLengthStructure | 
		PEXHasRefs | PEXEditMode;

	status = PEXGetStructureInfo (display, str_B, PEXIEEE_754_32, mask, 
				      &str_info1);

	status2 = PEXGetStructureInfo (display, spare_2, PEXIEEE_754_32, mask, 
				      &str_info2);

        if (! ((status) &&
	       (status2) &&
	       (status == status2) &&
	       (str_info1.element_count == str_info2.element_count) &&
	       (str_info1.edit_mode == str_info2.edit_mode) &&
	       (str_info1.element_pointer == str_info2.element_pointer) &&
	       (str_info1.size == str_info2.size) ))
	{
	    _hppex_stderr_print( "Test 5b: Copy structure failed\n");
	}
	       

	structs = PEXGetStructuresInNetwork (display, spare_2, PEXAll, &count);

        expected_structs[0] = spare_2;
        expected_structs[1] = str_D;

        if (! (struct_lists_are_equivalent (2, structs, expected_structs)))
	{
	    _hppex_stderr_print( "Test 5b: Copy Structs corrupted hierarchy\n");
	}
	XFree (structs);

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

	/*
         * Test 6a: Copy Elements to an empty structure
         */

	PEXDeleteElements (display, spare_2,  PEXBeginning, 0, PEXEnd, 0);


	mask = 0;
	mask = PEXElementPtr | PEXNumElements | PEXLengthStructure | 
		PEXHasRefs | PEXEditMode;

        /*
         * Is spare_2 really empty?
         */
	status = PEXGetStructureInfo (display, spare_2, PEXIEEE_754_32, mask, 
				      &str_info);

        if (! ((status) &&
	       (str_info.has_refs == False) &&
	       (str_info.element_count == 0) &&
	       (str_info.edit_mode == PEXStructureReplace) &&
	       (str_info.element_pointer == 0) &&
	       (str_info.size == 0) ))
	{
	    _hppex_stderr_print( "Test 6a: Delete Elements failed\n");
	}
	       
 	PEXCopyElements (display, str_A, PEXEnd, -4, PEXBeginning, 3, spare_2,
			 PEXCurrent, 1);

	status = PEXGetStructureInfo (display, spare_2, PEXIEEE_754_32, mask, 
				      &str_info);

        if (! ((status) &&
	       (str_info.has_refs == False) &&
	       (str_info.element_count == 3) &&
	       (str_info.edit_mode == PEXStructureReplace) &&
	       (str_info.element_pointer == 3) &&
	       (str_info.size == 6) ))
	{
	    _hppex_stderr_print( "Test 6a: Copy Elements failed\n");
	}


	/*
	 * Test 6b. Copy elements to non-empty structure
	 */
	
	PEXSetElementPtr (display, str_A, PEXBeginning, 4);
 	PEXCopyElements (display, str_A, PEXCurrent, -2, PEXEnd, -2, spare_2,
			 PEXCurrent, -1);
	
	status = PEXGetStructureInfo (display, spare_2, PEXIEEE_754_32, mask, 
				      &str_info);

        if (! ((status) &&
	       (str_info.has_refs == False) &&
	       (str_info.element_count == 9) &&
	       (str_info.edit_mode == PEXStructureReplace) &&
	       (str_info.element_pointer == 8) &&
	       (str_info.size == 18) ))
	{
	    _hppex_stderr_print( "Test 6b: Copy Elements failed\n");
	}

	/*
         * Current and first elements. Both should be labels
	 * This verifies correct elements were copied at correct locations.
         */
	PEXGetElementInfo (display, spare_2, PEXBeginning, 1, PEXBeginning,
			   1,  PEXIEEE_754_32, &count, &ele_info);

	if (! ((count == 1) &&
	       (ele_info->type == PEXOCLabel) ))
	{
	    _hppex_stderr_print
	       ( "Test 6b: Copy Elements first label not found\n");
	}
	XFree (ele_info);

	PEXGetElementInfo (display, spare_2, PEXCurrent, 0, PEXCurrent,
			   0,  PEXIEEE_754_32, &count, &ele_info);

	if (! ((count == 1) &&
	       (ele_info->type == PEXOCLabel) ))
	{
	    _hppex_stderr_print
	       ( "Test 6b: Copy Elements second label not found\n");
	}
	XFree (ele_info);

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

        /*
         * Test 7a: Insert
         */
	PEXSetEditingMode (display, spare_2, PEXStructureInsert);
	PEXSetElementPtr (display, spare_2, PEXBeginning, 2);
        PEXSetInteriorBundleIndex (display, spare_2, PEXOCStore, 3);

	mask = 0;
	mask = PEXElementPtr | PEXNumElements | PEXEditMode;

	status = PEXGetStructureInfo (display, spare_2, PEXIEEE_754_32, mask, 
				      &str_info);

	PEXGetElementInfo (display, spare_2, PEXCurrent, 0, PEXCurrent,
			   0,  PEXIEEE_754_32, &count, &ele_info);

        if (! ((status) &&
	       (str_info.element_count == 10) &&
	       (str_info.edit_mode == PEXStructureInsert) &&
	       (str_info.element_pointer == 3) &&
	       (ele_info->type == PEXOCInteriorBundleIndex) ))
	{
	    _hppex_stderr_print( "Test 7a: Set Editing Mode/Insert failed\n");
	}
	XFree (ele_info);

	
	/*
         * Test 7b: Replace
         */
	PEXSetEditingMode (display, spare_2, PEXStructureReplace);
        PEXSetLineType (display, spare_2, PEXOCStore, PEXLineTypeDashed);

	status = PEXGetStructureInfo (display, spare_2, PEXIEEE_754_32, mask, 
				      &str_info);

	PEXGetElementInfo (display, spare_2, PEXCurrent, 0, PEXCurrent,
			   0,  PEXIEEE_754_32, &count, &ele_info);

        if (! ((status) &&
	       (str_info.element_count == 10) &&
	       (str_info.edit_mode == PEXStructureReplace) &&
	       (str_info.element_pointer == 3) &&
	       (ele_info->type == PEXOCLineType) ))
	{
	    _hppex_stderr_print( "Test 7b: Set Editing Mode/Replace failed\n");
	}
	XFree (ele_info);

	/*
         * 7c: Replace again to be sure
         */

        PEXSetLineColorIndex (display, spare_2, PEXOCStore, 4);

	status = PEXGetStructureInfo (display, spare_2, PEXIEEE_754_32, mask, 
				      &str_info);

	PEXGetElementInfo (display, spare_2, PEXCurrent, 0, PEXCurrent,
			   0,  PEXIEEE_754_32, &count, &ele_info);

        if (! ((status) &&
	       (str_info.element_count == 10) &&
	       (str_info.edit_mode == PEXStructureReplace) &&
	       (str_info.element_pointer == 3) &&
	       (ele_info->type == PEXOCLineColorIndex) ))
	{
	    _hppex_stderr_print( "Test 7c: Set Editing Mode/Replace failed\n");
	}
	XFree (ele_info);
     }

    /**********************************************************************
     * Test 8
     *
     * PEXSetElementPtr
     *********************************************************************/
     {
        unsigned long mask;
        PEXStructureInfo str_info;
         
	mask = 0;
	mask = PEXElementPtr;

        /*
         * Test 8a: Set to ele 0 directly
         */
        PEXSetElementPtr (display, spare_2, PEXBeginning, 0);

	status = PEXGetStructureInfo (display, spare_2, PEXIEEE_754_32, mask, 
				      &str_info);
        if (! ((status) &&
	       (str_info.element_pointer == 0) ))
	{
	    _hppex_stderr_print( "Test 8a: Set Element Pointer failed \n");
	}

        /*
         * Test 8b: Set to last ele directly
         */
        PEXSetElementPtr (display, spare_2, PEXEnd, 0);

	status = PEXGetStructureInfo (display, spare_2, PEXIEEE_754_32, mask, 
				      &str_info);
        if (! ((status) &&
	       (str_info.element_pointer == 10) ))
	{
	    _hppex_stderr_print( "Test 8b: Set Element Pointer failed \n");
	}

        /*
         * Test 8c: Set to ele 0 by going "out of bounds"
         */
        PEXSetElementPtr (display, spare_2, PEXEnd, -13);

	status = PEXGetStructureInfo (display, spare_2, PEXIEEE_754_32, mask, 
				      &str_info);
        if (! ((status) &&
	       (str_info.element_pointer == 0) ))
	{
	    _hppex_stderr_print( "Test 8c: Set Element Pointer failed \n");
	}

        /*
         * Test 8d: Set to last ele by going "out of bounds"
         */
        PEXSetElementPtr (display, spare_2, PEXEnd, 13);

	status = PEXGetStructureInfo (display, spare_2, PEXIEEE_754_32, mask, 
				      &str_info);
        if (! ((status) &&
	       (str_info.element_pointer == 10) ))
	{
	    _hppex_stderr_print( "Test 8d: Set Element Pointer failed \n");
	}

        /*
         * Test 8e: Set to 5th ele
         */
        PEXSetElementPtr (display, spare_2, PEXEnd, -5);

	status = PEXGetStructureInfo (display, spare_2, PEXIEEE_754_32, mask, 
				      &str_info);
        if (! ((status) &&
	       (str_info.element_pointer == 5) ))
	{
	    _hppex_stderr_print( "Test 8e: Set Element Pointer failed \n");
	}

        /*
         * Test 8f: Set to 7th ele
         */
        PEXSetElementPtr (display, spare_2, PEXCurrent, 2);

	status = PEXGetStructureInfo (display, spare_2, PEXIEEE_754_32, mask, 
				      &str_info);
        if (! ((status) &&
	       (str_info.element_pointer == 7) ))
	{
	    _hppex_stderr_print( "Test 8f: Set Element Pointer failed \n");
	}

        /*
         * Test 8g: Set to 4th ele
         */
        PEXSetElementPtr (display, spare_2, PEXCurrent, -3);

	status = PEXGetStructureInfo (display, spare_2, PEXIEEE_754_32, mask, 
				      &str_info);
        if (! ((status) &&
	       (str_info.element_pointer == 4) ))
	{
	    _hppex_stderr_print( "Test 8g: Set Element Pointer failed \n");
	}
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

        /*
         * Test 9a: Find label 1
         */
        PEXSetElementPtr (display, str_A, PEXBeginning, 0);

        PEXSetElementPtrAtLabel (display, str_A, 1, 0);

	status = PEXGetStructureInfo (display, str_A, PEXIEEE_754_32, mask, 
				      &str_info);
        if (! ((status) &&
	       (str_info.element_pointer == 3) ))
	{
	    _hppex_stderr_print
	       ( "Test 9a: Set Element Pointer At Label failed \n");
	}
	
        /*
         * Test 9b: Find label 2, offset -2
         */
        PEXSetElementPtrAtLabel (display, str_A, 2, -2);

	status = PEXGetStructureInfo (display, str_A, PEXIEEE_754_32, mask, 
				      &str_info);
        if (! ((status) &&
	       (str_info.element_pointer == 5) ))
	{
	    _hppex_stderr_print
	       ( "Test 9b: Set Element Pointer At Label failed \n");
	}
	
        /*
         * Test 9c: Find label 1, offset 1 (should fail)
         */
        _hppex_test_set_expected_error (PEX_ERR, BadPEXLabel, 
					PEXRCSetElementPointerAtLabel, 0, 0);

        PEXSetElementPtrAtLabel (display, str_A, 1, 1);

        XSync(display, 0);

	_hppex_test_check_expected_error (err_mask);


	status = PEXGetStructureInfo (display, str_A, PEXIEEE_754_32, mask, 
				      &str_info);
        if (! ((status) &&
	       (str_info.element_pointer == 5) ))
	{
	    _hppex_stderr_print
	       ( "Test 9c: Set Element Pointer At Label failed \n");
	}
	
        /*
         * Test 9d: Find label 2 (offset 1)
         */
        PEXSetElementPtrAtLabel (display, str_A, 2, 1);

	status = PEXGetStructureInfo (display, str_A, PEXIEEE_754_32, mask, 
				      &str_info);
        if (! ((status) &&
	       (str_info.element_pointer == 8) ))
	{
	    _hppex_stderr_print
	       ( "Test 9d: Set Element Pointer At Label failed \n");
	}
	
        /*
         * Test 9e: Find label 2, (should fail)
         */
        _hppex_test_set_expected_error (PEX_ERR, BadPEXLabel, 
					PEXRCSetElementPointerAtLabel, 0, 0);

        PEXSetElementPtrAtLabel (display, str_A, 2, 0);

        XSync(display, 0);
 
	_hppex_test_check_expected_error (err_mask);

	status = PEXGetStructureInfo (display, str_A, PEXIEEE_754_32, mask, 
				      &str_info);
        if (! ((status) &&
	       (str_info.element_pointer == 8) ))
	{
	    _hppex_stderr_print
	       ( "Test 9e: Set Element Pointer At Label failed \n");
	}
	
        /*
         * Test 9f: Find label 1, (should fail)
         */
        _hppex_test_set_expected_error (PEX_ERR, BadPEXLabel, 
					PEXRCSetElementPointerAtLabel, 0, 0);

        PEXSetElementPtrAtLabel (display, str_A, 1, 0);

        XSync(display, 0);

	_hppex_test_check_expected_error (err_mask);

	status = PEXGetStructureInfo (display, str_A, PEXIEEE_754_32, mask, 
				      &str_info);
        if (! ((status) &&
	       (str_info.element_pointer == 8) ))
	{
	    _hppex_stderr_print
	       ( "Test 9e: Set Element Pointer At Label failed \n");
	}
	
        /*
         * Test 9f: Find label 1, (should fail)
         */
        _hppex_test_set_expected_error (PEX_ERR, BadPEXLabel, 
					PEXRCSetElementPointerAtLabel, 0, 0);

        PEXSetElementPtr (display, str_A, PEXBeginning, 3);
        PEXSetElementPtrAtLabel (display, str_A, 1, 0);

        XSync(display, 0);

	_hppex_test_check_expected_error (err_mask);

	status = PEXGetStructureInfo (display, str_A, PEXIEEE_754_32, mask, 
				      &str_info);
        if (! ((status) &&
	       (str_info.element_pointer == 3) ))
	{
	    _hppex_stderr_print
	       ( "Test 9f: Set Element Pointer At Label failed \n");
	}
	
        /*
         * Test 9g: Find label 1 in different structure
         */
        PEXSetElementPtr (display, str_C, PEXBeginning, 0);
        PEXSetElementPtrAtLabel (display, str_C, 1, 0);

	status = PEXGetStructureInfo (display, str_C, PEXIEEE_754_32, mask, 
				      &str_info);
        if (! ((status) &&
	       (str_info.element_pointer == 1) ))
	{
	    _hppex_stderr_print
	       ( "Test 9g: Set Element Pointer At Label failed \n");
	}

        /*
         * Test 9h: Find label 1 again
         */
        PEXSetElementPtrAtLabel (display, str_C, 1, 0);

	status = PEXGetStructureInfo (display, str_C, PEXIEEE_754_32, mask, 
				      &str_info);
        if (! ((status) &&
	       (str_info.element_pointer == 4) ))
	{
	    _hppex_stderr_print
	       ( "Test 9h: Set Element Pointer At Label failed \n");
	}

        /*
         * Test 9i: Find label 1 again (should fail)
         */
        _hppex_test_set_expected_error (PEX_ERR, BadPEXLabel, 
					PEXRCSetElementPointerAtLabel, 0, 0);

        PEXSetElementPtrAtLabel (display, str_C, 1, 0);

        XSync(display, 0);
	_hppex_test_check_expected_error (err_mask);

	status = PEXGetStructureInfo (display, str_C, PEXIEEE_754_32, mask, 
				      &str_info);
        if (! ((status) &&
	       (str_info.element_pointer == 4) ))
	{
	    _hppex_stderr_print
	       ( "Test 9i: Set Element Pointer At Label failed \n");
	}
    }
    /**********************************************************************
     * Test 10
     *
     * PEXElementSearch
     *********************************************************************/
     {
	unsigned long offset;
	unsigned short incl_list[5], excl_list[5];

	/*
         * Test 10a. Search for existing element
         */

	incl_list[0] = PEXOCMarkerType;
	incl_list[1] = PEXOCCharHeight;
	incl_list[2] = PEXOCPolyline;

	excl_list[0] = PEXOCLabel;
	excl_list[1] = PEXOCExecuteStructure;

	status = PEXElementSearch (display, str_A, PEXBeginning, 0, PEXForward,
			           3, incl_list, 2, excl_list, &offset);
			  
	if (! ((status == PEXFound) &&
	       (offset == 6) ))
	{
	    _hppex_stderr_print
	       ( "Test 10a: Element Search failed\n");
	}

	/*
         * Test 10b. Search for existing element excluded
         */

	incl_list[0] = PEXOCMarkerType;
	incl_list[1] = PEXOCCharHeight;
	incl_list[2] = PEXOCPolyline;

	excl_list[0] = PEXOCCharHeight;
	excl_list[1] = PEXOCExecuteStructure;

	status = PEXElementSearch (display, str_A, PEXBeginning, 0, PEXForward,
			           3, incl_list, 2, excl_list, &offset);
			  
	if (!(status == PEXNotFound))	
	{
	    _hppex_stderr_print
	       ( "Test 10b: Element Search should not have succeeded\n");
	}
	
	/*
         * Test 10c. Search for non-existing elements
         */

	incl_list[0] = PEXOCMarkerType;
	incl_list[1] = PEXOCLineColor;
	incl_list[2] = PEXOCPolyline;

	excl_list[0] = PEXOCCharHeight;
	excl_list[1] = PEXOCExecuteStructure;

	status = PEXElementSearch (display, str_A, PEXBeginning, 0, PEXForward,
			           3, incl_list, 2, excl_list, &offset);
			  
	if (!(status == PEXNotFound))	
	{
	    _hppex_stderr_print
	       ( "Test 10c: Element Search should not have succeeded\n");
	}

	/*
         * Test 10d. Search for existing element backwards
         */

	incl_list[0] = PEXOCMarkerType;
	incl_list[1] = PEXOCCharHeight;
	incl_list[2] = PEXOCPolyline;

	excl_list[0] = PEXOCLabel;
	excl_list[1] = PEXOCExecuteStructure;

	status = PEXElementSearch (display, str_A, PEXEnd, -1, PEXBackward,
			           3, incl_list, 2, excl_list, &offset);
			  
	if (! ((status == PEXFound) &&
	       (offset == 6) ))
	{
	    _hppex_stderr_print
	       ( "Test 10d: Element Search failed\n");
	}

	/*
         * Test 10e. Search for existing element excluded backwards
         */

	incl_list[0] = PEXOCMarkerType;
	incl_list[1] = PEXOCCharHeight;
	incl_list[2] = PEXOCPolyline;

	excl_list[0] = PEXOCCharHeight;
	excl_list[1] = PEXOCExecuteStructure;

	status = PEXElementSearch (display, str_A, PEXBeginning, 7, 
				   PEXBackward,
			           3, incl_list, 2, excl_list, &offset);
			  
	if (!(status == PEXNotFound))	
	{
	    _hppex_stderr_print
	       ( "Test 10e: Element Search should not have succeeded\n");
	}
	
	/*
         * Test 10f. Search for non-existing elements
         */

	incl_list[0] = PEXOCMarkerType;
	incl_list[1] = PEXOCLineColor;
	incl_list[2] = PEXOCPolyline;

	excl_list[0] = PEXOCCharHeight;
	excl_list[1] = PEXOCExecuteStructure;

	status = PEXElementSearch (display, str_A, PEXEnd, -3, PEXBackward,
			           3, incl_list, 2, excl_list, &offset);
			  
	if (!(status == PEXNotFound))	
	{
	    _hppex_stderr_print
	       ( "Test 10f: Element Search should not have succeeded\n");
	}

	/*
         * Test 10g. Search for existing element starting on element
         */

	incl_list[0] = PEXOCMarkerType;
	incl_list[1] = PEXOCLabel;
	incl_list[2] = PEXOCPolyline;

	excl_list[0] = PEXOCCharSpacing;
	excl_list[1] = PEXOCExecuteStructure;

	status = PEXElementSearch (display, str_A, PEXBeginning, 3, PEXForward,
			           3, incl_list, 2, excl_list, &offset);
			  
	if (! ((status == PEXFound) &&
	       (offset == 3) ))
	{
	    _hppex_stderr_print( "Test 10g: Element Search failed\n");
	}

	/*
         * Test 10h. Search for existing element starting on element
         */

	incl_list[0] = PEXOCMarkerType;
	incl_list[1] = PEXOCLabel;
	incl_list[2] = PEXOCPolyline;

	excl_list[0] = PEXOCCharSpacing;
	excl_list[1] = PEXOCExecuteStructure;

	status = PEXElementSearch (display, str_A, PEXBeginning, 3, 
				   PEXBackward,
			           3, incl_list, 2, excl_list, &offset);
			  
	if (! ((status == PEXFound) &&
	       (offset == 3) ))
	{
	    _hppex_stderr_print( "Test 10h: Element Search failed\n");
	}

     }
    /**********************************************************************
     * Test 11
     *
     * PEXChangeStructureRefs
     *********************************************************************/
     {
	PEXStructurePath *path1, *path2;
	unsigned long count1, count2;

	/*
         * Empty spare_1 to prevent any structure recursion later
         */
	PEXDeleteElements (display, spare_1, PEXBeginning, 0, PEXEnd, 0);
	
	/*
         * Test 11a: Change ID to known structure
         */
	PEXChangeStructureRefs (display, str_D, spare_1);

	path1 = PEXGetDescendants (display, str_C, PEXBottomPart, 1, &count1);
	
	path2 = PEXGetDescendants (display, str_B, PEXBottomPart, 1, &count2);

	if (! ((count1 == 1) &&
	       (count2 == 1) &&
	       (path1[0].count == 1) &&
	       (path1[0].elements[0].structure == spare_1) &&
	       (path1[0].elements[0].offset == 0) &&
	       (path2[0].count == 1) &&
	       (path2[0].elements[0].structure == spare_1) &&
	       (path2[0].elements[0].offset == 0) ))
	{
	    _hppex_stderr_print( "Test 11a: Change Structure Refs failed\n");
	}
	PEXFreeStructurePaths (count1, path1);
	PEXFreeStructurePaths (count2, path2);
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

	/*
         * Test 12a: Delete an arbitrary range. 
         * We have previously deleted all elements in a structure. So, this
	 * just a sanity check that we can do any range.
         */
	PEXSetElementPtr (display, str_B, PEXBeginning, 3);
	PEXDeleteElements (display, str_B, PEXCurrent, -1, PEXEnd, -1);

	mask = PEXElementPtr | PEXNumElements | PEXEditMode;
	status = PEXGetStructureInfo (display, str_B, PEXIEEE_754_32, mask,
				      &str_info);

	status2 = PEXGetElementInfo (display, str_B, PEXBeginning, 0,
				     PEXEnd, 0, PEXIEEE_754_32, &count,
				     &ele_info);
	if (! ( (status) &&
		(status2) &&
		(str_info.element_count == 2) &&
		(str_info.element_pointer == 1) &&
		(str_info.edit_mode == PEXStructureReplace) &&
		(count == 2) &&
		(ele_info[0].type == PEXOCMarkerScale) &&
		(ele_info[1].type == PEXOCLabel) ))
	{
	    _hppex_stderr_print( "Test 12a: Delete Elements failed\n");
	}
	XFree (ele_info);
     }
    /**********************************************************************
     * Test 13
     *
     * PEXDeleteBetweenLabels
     *********************************************************************/
    {
	PEXStructureInfo str_info;
	PEXElementInfo *ele_info;
	unsigned long mask, count, err_mask;
	Status status2;

	/*
         * Test 13a: Try to delete with pointer on first label (should fail)
         */
        PEXSetElementPtr (display, str_A, PEXBeginning, 3);

	err_mask = ERROR_CODE | REQUEST_CODE;

        _hppex_test_set_expected_error (PEX_ERR, BadPEXLabel, 
					PEXRCDeleteBetweenLabels, 0, 0);
	PEXDeleteBetweenLabels (display,  str_A, 1, 2);

        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask); 

	mask = PEXElementPtr | PEXNumElements | PEXEditMode;
	status = PEXGetStructureInfo (display, str_A, PEXIEEE_754_32, mask,
				      &str_info);

	if (! ( (status) &&
		(str_info.element_count == 9) &&
		(str_info.element_pointer == 3) &&
		(str_info.edit_mode == PEXStructureInsert) ))
	{
	    _hppex_stderr_print( "Test 13a: Delete Between labels failed\n");
	}

	/*
         * Test 13b: Delete between valid labels
         */
        PEXSetElementPtr (display, str_A, PEXCurrent, -1);
	PEXDeleteBetweenLabels (display,  str_A, 1, 2);

	mask = PEXElementPtr | PEXNumElements | PEXEditMode;
	status = PEXGetStructureInfo (display, str_A, PEXIEEE_754_32, mask,
				      &str_info);

	status2 = PEXGetElementInfo (display, str_A, PEXBeginning, 3,
				     PEXBeginning, 4, PEXIEEE_754_32, &count,
				     &ele_info);
	if (! ( (status) &&
		(status2) &&
		(str_info.element_count == 6) &&
		(str_info.element_pointer == 3) &&
		(str_info.edit_mode == PEXStructureInsert) &&
		(count == 2) &&
		(ele_info[0].type == PEXOCLabel) &&
		(ele_info[1].type == PEXOCLabel) ))
	{
	    _hppex_stderr_print( "Test 13b: Delete Between Labels failed\n");
	}
	XFree (ele_info);

	/*
         * Test 13c: Delete and empth range between valid labels
         */
        PEXSetElementPtr (display, str_A, PEXCurrent, -1);
	PEXDeleteBetweenLabels (display,  str_A, 1, 2);

	mask = PEXElementPtr | PEXNumElements | PEXEditMode;
	status = PEXGetStructureInfo (display, str_A, PEXIEEE_754_32, mask,
				      &str_info);

	status2 = PEXGetElementInfo (display, str_A, PEXBeginning, 3,
				     PEXBeginning, 4, PEXIEEE_754_32, &count,
				     &ele_info);
	if (! ( (status) &&
		(status2) &&
		(str_info.element_count == 6) &&
		(str_info.element_pointer == 3) &&
		(str_info.edit_mode == PEXStructureInsert) &&
		(count == 2) &&
		(ele_info[0].type == PEXOCLabel) &&
		(ele_info[1].type == PEXOCLabel) ))
	{
	    _hppex_stderr_print( "Test 13c: Delete Between labels failed\n");
	}
	XFree (ele_info);
    }
    /**********************************************************************
     * Test 14
     *
     * PEXDeleteToLabel
     *********************************************************************/
    {
	PEXStructureInfo str_info;
	PEXElementInfo *ele_info;
	unsigned long mask, count, err_mask;
	Status status2;

	/*
	 * Test 14a: Try deleting from label (should fail)
         */
	err_mask = ERROR_CODE | REQUEST_CODE;

	PEXSetElementPtr (display, str_D, PEXBeginning, 2);

        _hppex_test_set_expected_error (PEX_ERR, BadPEXLabel, 
					PEXRCDeleteElementsToLabel, 0, 0);

	PEXDeleteToLabel (display,  str_D, PEXEnd, -1, 2);

        XSync(display, 0);
        _hppex_test_check_expected_error (err_mask); 

	mask = PEXElementPtr | PEXNumElements | PEXEditMode;
	status = PEXGetStructureInfo (display, str_D, PEXIEEE_754_32, mask,
				      &str_info);

	if (! ( (status) &&
		(str_info.element_count == 4) &&
		(str_info.element_pointer == 2) &&
		(str_info.edit_mode == PEXStructureInsert) ))
	{
	    _hppex_stderr_print( "Test 14a: Delete to Label failed\n");
	}
 
	/*
         * Test 14b: Back of one  off of label (nothing should happen)
         */
	PEXSetElementPtr (display, str_D, PEXBeginning, 1);

	PEXDeleteToLabel (display,  str_D, PEXEnd, -2, 2);

	mask = PEXElementPtr | PEXNumElements | PEXEditMode;
	status = PEXGetStructureInfo (display, str_D, PEXIEEE_754_32, mask,
				      &str_info);

	if (! ( (status) &&
		(str_info.element_count == 4) &&
		(str_info.element_pointer == 2) &&
		(str_info.edit_mode == PEXStructureInsert) ))
	{
	    _hppex_stderr_print( "Test 14b: Delete to Label failed\n");
	}
 

	/*
         * Test 14c: Delete a valid range
         */

	PEXDeleteToLabel (display,  str_D, PEXCurrent, -1, 2);

	mask = PEXElementPtr | PEXNumElements | PEXEditMode;
	status = PEXGetStructureInfo (display, str_D, PEXIEEE_754_32, mask,
				      &str_info);

	status2 = PEXGetElementInfo (display, str_D, PEXBeginning, 0,
				     PEXEnd, -1, PEXIEEE_754_32, &count,
				     &ele_info);
	if (! ( (status) &&
		(status2) &&
		(str_info.element_count == 3) &&
		(str_info.element_pointer == 1) &&
		(str_info.edit_mode == PEXStructureInsert) &&
		(count == 2) &&
		(ele_info[0].type == PEXOCLineColor) &&
		(ele_info[1].type == PEXOCLabel) ))
	{
	    _hppex_stderr_print( "Test 14c: Delete to Label failed\n");
	}
    }
    /**********************************************************************
     * Test 15
     *
     * PEXCopyElements to self
     *********************************************************************/
    {
	unsigned long mask;
	PEXStructureInfo str_info;
	PEXElementInfo *ele_info;
	unsigned long count;

	mask = PEXElementPtr | PEXNumElements | PEXEditMode;

	/*
         * Check to see if spare_2 is configured as we expect
         */
        status = PEXGetStructureInfo (display, spare_2, PEXIEEE_754_32, mask,
                                      &str_info);

        if (! ((status) &&
               (str_info.element_count == 10) &&
               (str_info.edit_mode == PEXStructureReplace) &&
               (str_info.element_pointer == 4) ))
        {
            _hppex_stderr_print
	       ( "Test 15: Get Info on spare_2 returned bogus data\n");
        }

	/*
         * Copy elements from one location in structure to another
         */
 	PEXCopyElements (display, spare_2, PEXBeginning, 3, PEXCurrent, 0, 
			 spare_2, PEXBeginning, 7);

	/*
         * See if copy was successful
         */
        status = PEXGetStructureInfo (display, spare_2, PEXIEEE_754_32, mask,
                                      &str_info);

        if (! ((status) &&
               (str_info.element_count == 12) &&
               (str_info.edit_mode == PEXStructureReplace) &&
               (str_info.element_pointer == 9) ))
        {
            _hppex_stderr_print
	       ( "Test 15: Structure info incorrect following Copy Elements\n");
        }

	PEXGetElementInfo (display, spare_2, PEXBeginning, 0, PEXEnd,
			   0,  PEXIEEE_754_32, &count, &ele_info);

	if (! ((count == 12) &&
	       (ele_info[0].type == PEXOCLabel) &&
	       (ele_info[1].type == PEXOCExecuteStructure)  &&
	       (ele_info[2].type == PEXOCLineColorIndex)  &&
	       (ele_info[3].type == PEXOCBFReflectionModel)  &&
	       (ele_info[4].type == PEXOCLabel)  &&
	       (ele_info[5].type == PEXOCExecuteStructure)  &&
	       (ele_info[6].type == PEXOCBFSurfaceColorIndex)  &&
	       (ele_info[7].type == PEXOCLineColorIndex)  &&
	       (ele_info[8].type == PEXOCBFReflectionModel)  &&
	       (ele_info[9].type == PEXOCCharHeight)  &&
	       (ele_info[10].type == PEXOCLabel)  &&
	       (ele_info[11].type == PEXOCBFSurfaceColorIndex) )) 
	{
	    _hppex_stderr_print
	       ( "Test 15: Elements not as expected following copy\n");
	}

	XFree (ele_info);
    }


    /**********************************************************************
     * Test 16
     *
     * PEXDestroyStructures
     *********************************************************************/
    {
	PEXStructurePath *path1, *path2;
	PEXStructure structs[6];
	unsigned long err_mask, mask, count1, count2;
	PEXStructureInfo str_info;

	/*
	 * Test 16a: Destroy a single structure
         */
	structs[0] = spare_1;
	PEXDestroyStructures (display, 1, structs);

	path1 = PEXGetDescendants (display, str_C, PEXBottomPart, 1, &count1);
	
	path2 = PEXGetDescendants (display, str_B, PEXBottomPart, 1, &count2);

	if (! ((count1 == 0) &&
	       (count2 == 0) ))
	{
	    _hppex_stderr_print( "Test 16a: Destroy Structures failed\n");
	}

	/*
         * Test 16b: Destroy a group of structures
         */
	structs[0] = spare_2;
	structs[1] = str_A;
	structs[2] = str_B;
	structs[3] = str_C;
	structs[4] = str_D;

	PEXDestroyStructures (display, 4, structs);
	
	err_mask = ERROR_CODE | REQUEST_CODE;
        mask = PEXElementPtr | PEXNumElements | PEXEditMode;

       _hppex_test_set_expected_error (PEX_ERR, BadPEXStructure, 
					PEXRCGetStructureInfo, 0, 0);

	status = PEXGetStructureInfo (display, str_B, PEXIEEE_754_32, mask,
				       &str_info);

       XSync(display, 0);
       _hppex_test_check_expected_error (err_mask);

	if (status)
	{
	    _hppex_stderr_print( "Test 16b: Destroy Structures failed\n");
	}
    }
}

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "str_basic");
    *num_images      = 1;
    *supported_modes = NON_OC_MODE;

    strcpy(win_title, "str_basic");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    _hppex_set_stderr("str_basic.stderr", "str_basic");
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    describe_test( "\nPEXlib Structures Tests:\n\n" );
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
    file_testproc("str_basic.stderr", "str_basic");
    }
