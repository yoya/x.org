/* $XConsortium: pexocp.87.4.c,v 1.0 93/11/22 12:34:18 rws Exp $ */

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

#include <X11/PEX5/PEXlib.h>
#include <misc.h>
#include <err_handler.h>
#include <stdio.h>

/* 
 *	TEST: Boundary test for PEXPolylineSetWithData for a polyline with fewer
 *	than two verticies.
 */

draw_polylines(Display          *dpy,
	            XID              resourceID,
	            PEXOCRequestType req_type)
    {
	PEXListOfVertex		polyline_list_1[2], polyline_list_2[2];
	PEXCoord			points_3[5];
	PEXVertexRGB		polyline_1[5], polyline_2[5];
	PEXVertexRGB		polyline_3[2], polyline_4[2];
	PEXColor			p_line_color;
	
	/*
	 *	Set up some data.
	 */
	polyline_list_1[0].count = 5;
	polyline_list_1[0].vertices.rgb = polyline_1;
	polyline_list_1[1].count = 5;
	polyline_list_1[1].vertices.rgb = polyline_2;

	polyline_list_2[0].count = 1;
	polyline_list_2[0].vertices.rgb = polyline_3;
	polyline_list_2[1].count = 2;
	polyline_list_2[1].vertices.rgb = polyline_4;
	
	polyline_1[0].point.x = 0.1;
	polyline_1[0].point.y = 0.1;
	polyline_1[0].point.z = 0.0;
	
	polyline_1[0].rgb.red = 1.0;
	polyline_1[0].rgb.green = 1.0;
	polyline_1[0].rgb.blue = 1.0;
	
	polyline_1[1].point.x = 0.9;
	polyline_1[1].point.y = 0.1;
	polyline_1[1].point.z = 0.0;
	
	polyline_1[1].rgb.red = 0.0;
	polyline_1[1].rgb.green = 0.0;
	polyline_1[1].rgb.blue = 0.0;
	
	polyline_1[2].point.x = 0.9;
	polyline_1[2].point.y = 0.9;
	polyline_1[2].point.z = 0.0;
	
	polyline_1[2].rgb.red = 1.0;
	polyline_1[2].rgb.green = 0.0;
	polyline_1[2].rgb.blue = 0.0;
	
	polyline_1[3].point.x = 0.1;
	polyline_1[3].point.y = 0.9;
	polyline_1[3].point.z = 0.0;
	
	polyline_1[3].rgb.red = 0.0;
	polyline_1[3].rgb.green = 1.0;
	polyline_1[3].rgb.blue = 0.0;
	
	polyline_1[4].point.x = 0.1;
	polyline_1[4].point.y = 0.1;
	polyline_1[4].point.z = 0.0;
	
	polyline_1[4].rgb.red = 0.0;
	polyline_1[4].rgb.green = 0.0;
	polyline_1[4].rgb.blue = 1.0;
	
	polyline_2[0].point.x = 0.25;
	polyline_2[0].point.y = 0.25;
	polyline_2[0].point.z = 0.0;
	
	polyline_2[0].rgb.red = 1.0;
	polyline_2[0].rgb.green = 1.0;
	polyline_2[0].rgb.blue = 1.0;
	
	polyline_2[1].point.x = 0.75;
	polyline_2[1].point.y = 0.25;
	polyline_2[1].point.z = 0.0;
	
	polyline_2[1].rgb.red = 0.0;
	polyline_2[1].rgb.green = 1.0;
	polyline_2[1].rgb.blue = 1.0;
	
	polyline_2[2].point.x = 0.75;
	polyline_2[2].point.y = 0.75;
	polyline_2[2].point.z = 0.0;
	
	polyline_2[2].rgb.red = 1.0;
	polyline_2[2].rgb.green = 0.0;
	polyline_2[2].rgb.blue = 1.0;
	
	polyline_2[3].point.x = 0.25;
	polyline_2[3].point.y = 0.75;
	polyline_2[3].point.z = 0.0;
	
	polyline_2[3].rgb.red = 1.0;
	polyline_2[3].rgb.green = 1.0;
	polyline_2[3].rgb.blue = 0.0;
	
	polyline_2[4].point.x = 0.25;
	polyline_2[4].point.y = 0.25;
	polyline_2[4].point.z = 0.0;
	
	polyline_2[4].rgb.red = 1.0;
	polyline_2[4].rgb.green = 1.0;
	polyline_2[4].rgb.blue = 1.0;
	
	points_3[0].x = 0.4;
	points_3[0].y = 0.4;
	points_3[0].z = 0.5;
	
	points_3[1].x = 0.6;
	points_3[1].y = 0.4;
	points_3[1].z = 0.5;
	
	points_3[2].x = 0.6;
	points_3[2].y = 0.6;
	points_3[2].z = 0.5;
	
	points_3[3].x = 0.4;
	points_3[3].y = 0.6;
	points_3[3].z = 0.5;
	
	points_3[4].x = 0.4;
	points_3[4].y = 0.4;
	points_3[4].z = 0.5;
	
	polyline_3[0].point.x = 0.1;
	polyline_3[0].point.y = 0.1;
	polyline_3[0].point.z = 0.0;
	
	polyline_3[0].rgb.red = 1.0;
	polyline_3[0].rgb.green = 0.0;
	polyline_3[0].rgb.blue = 0.0;
	
	polyline_4[0].point.x = 0.9;
	polyline_4[0].point.y = 0.1;
	polyline_4[0].point.z = 0.0;
	
	polyline_4[0].rgb.red = 0.0;
	polyline_4[0].rgb.green = 0.0;
	polyline_4[0].rgb.blue = 1.0;
	
	polyline_4[1].point.x = 0.1;
	polyline_4[1].point.y = 0.9;
	polyline_4[1].point.z = 0.0;
	
	polyline_4[1].rgb.red = 1.0;
	polyline_4[1].rgb.green = 1.0;
	polyline_4[1].rgb.blue = 1.0;

	p_line_color.rgb.red = 0.0;
	p_line_color.rgb.green = 0.0;
	p_line_color.rgb.blue = 1.0;

	PEXSetSurfaceColor(dpy, resourceID, req_type, PEXColorTypeRGB, &p_line_color);
	PEXSetInteriorStyle(dpy, resourceID, req_type, PEXInteriorStyleSolid);
	PEXFillArea(dpy, resourceID, req_type, PEXShapeNonConvex, False, 5, points_3);

	PEXSetPolylineInterpMethod(dpy, resourceID, req_type, PEXPolylineInterpColor);
	PEXPolylineSetWithData(dpy, resourceID, req_type, PEXGAColor, PEXColorTypeRGB,
		2, polyline_list_1);
	PEXSetPolylineInterpMethod(dpy, resourceID, req_type, PEXPolylineInterpNone);
	PEXPolylineSetWithData(dpy, resourceID, req_type, PEXGAColor, PEXColorTypeRGB,
		2, polyline_list_2);

    }


void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "pexocp.87.4");
    *num_images      = 1;
    *supported_modes = IMM_MODE | STR_MODE | WKS_MODE;

    strcpy(win_title, "pexocp.87.4");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    unsigned long         mask = PEXRAHLHSRMode;
    PEXRendererAttributes attrs;

    attrs.hlhsr_mode = PEXHLHSRZBuffer;
    PEXChangeRenderer(dpy, renderer, mask, &attrs);

    if (glob_test_mode != IMM_MODE)
        _hppex_set_stderr("pexocp.87.4.stderr", "pexocp.87.4");
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    if (glob_test_mode == IMM_MODE)
	{
        describe_test( "PEXPolylineWithData Boundry Condition Test:\n" );
        describe_test( "\n" );
        describe_test( "  A degenerate lower left to upper right diagonal\n");
        describe_test( "  line is included in the same basic image for \n" );
        describe_test( "  the pexocp.87.1-3 tests.  It should NOT be drawn.\n" );
        describe_test( "\n" );
	
        draw_polylines( dpy, resourceID, req_type );
	}
    else
	{
	unsigned long    value_mask;
	int              float_format = PEXIEEE_754_32;
	PEXStructureInfo before_info, after_info;

        describe_test( "\n" );
        describe_test( "  Even though a degenerate primitive exists, it should still\n" );
        describe_test( "  be stored in a structure.  Therefore, the before and after\n" );
        describe_test( "  numbers (returned by a structure query) should show that\n" );
        describe_test( "  7 elements (6 good & 1 degenerate) were added to the structure.\n" );
        describe_test( "  The image should be identical to immediate mode.\n" );
        describe_test( "\n" );

        value_mask = PEXElementPtr		|
                     PEXNumElements		|
                     PEXLengthStructure	        |
                     PEXHasRefs			|
                     PEXEditMode;

        PEXGetStructureInfo( dpy,
                             resourceID,
                             float_format,
                             value_mask,
                            &before_info );

        draw_polylines( dpy, resourceID, PEXOCStore );

        /*
         *	Query the structure to see what is in it.
         */
        PEXGetStructureInfo( dpy,
                             resourceID,
                             float_format,
                             value_mask,
                             &after_info );

        /*
	 *	Dump the results of the queries to stderr
         */
        _hppex_stderr_print( "Structure Info Field        Before        After\n" );
        _hppex_stderr_print( "====================   ===========   ==========\n" );
        _hppex_stderr_print( "Element Count          %10d   %10d\n",
                 before_info.element_count, after_info.element_count );
        _hppex_stderr_print( "Structure Size         %10d   %10d\n",
                 before_info.size, after_info.size );
        _hppex_stderr_print( "Has References         %10d   %10d\n",
                 before_info.has_refs, after_info.has_refs );
        _hppex_stderr_print( "Edit Mode              %10d   %10d\n",
                 before_info.edit_mode, after_info.edit_mode );
        _hppex_stderr_print( "Element Pointer        %10d   %10d\n",
                 before_info.element_pointer, after_info.element_pointer );
        _hppex_stderr_print( "====================   ===========   ==========\n" );

	_hppex_close_stderr();
	}
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
     image_testproc("pexocp.87.4", "pexocp.87.4", img_desc,
		      dpy,window, 0, 0, 
		      glob_window_width, glob_window_height);
    if (glob_test_mode != IMM_MODE)
	{
	file_testproc("pexocp.87.4.stderr", "pexocp.87.4");
	}
    }
