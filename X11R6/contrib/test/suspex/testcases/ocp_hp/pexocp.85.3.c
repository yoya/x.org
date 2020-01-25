/* $XConsortium: pexocp.85.3.c,v 1.0 93/11/22 12:33:31 rws Exp $ */

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
#include <stdio.h>
#include <misc.h>
#include <err_handler.h>


/*
 *	draw_image prototype
 */
void draw_image( Display *dpy, XID res_id, PEXOCRequestType req_type );

/* 
 *	TEST: Boundary test for PEXPolyline for a polyline with fewer
 *	than two verticies.
 */
void draw_image( Display *dpy, XID res_id, PEXOCRequestType req_type )
{	
	/*
	 *	Data declarations
	 */
	unsigned int		count;
	PEXCoord			vertex_lists;

	vertex_lists.x	= 1.0;
	vertex_lists.y	= 2.0;
	vertex_lists.z	= 3.0;

	count			= 1;

	PEXPolyline(
		dpy,
		res_id,
		req_type,
		count,
		&vertex_lists );
}

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "pexocp.85.3");
    *num_images      = 1;
    *supported_modes = IMM_MODE | STR_MODE | WKS_MODE;

    strcpy(win_title, "pexocp.85.3");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    unsigned long         mask = PEXRAHLHSRMode;
    PEXRendererAttributes attrs;

    attrs.hlhsr_mode = PEXHLHSRZBuffer;
    PEXChangeRenderer(dpy, renderer, mask, &attrs);

    if (glob_test_mode != IMM_MODE)
        _hppex_set_stderr("pexocp.85.3.stderr", "pexocp.85.3");
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    if (glob_test_mode == IMM_MODE)
	{
        describe_test( "PEXPolyline Boundry Condition Test:\n" );
        describe_test( "\n" );
        describe_test( "  This is a degenerate polyline (< 2 points)\n" );
        describe_test( "  should be ignored by a renderer.  Therefore, the\n" );
        describe_test( "  window should be empty.\n" );
        describe_test( "\n" );
	
        draw_image( dpy, resourceID, req_type );
	}
    else
	{
	unsigned long    value_mask;
	int              float_format = PEXIEEE_754_32;
	PEXStructureInfo before_info, after_info;

        describe_test( "\n" );
        describe_test( "  Even though the primitive is degenerate, it should still\n" );
        describe_test( "  be stored in a structure.  Therefore, the before and after\n" );
        describe_test( "  numbers (returned by a structure query) should show that\n" );
        describe_test( "  the primitive was indeed added to the structure.  There still\n" );
        describe_test( "  should be no visible output in the window on the screen.\n" );
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

        draw_image( dpy, resourceID, PEXOCStore );

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
        _hppex_stderr_print("Structure Info Field        Before        After\n" );
        _hppex_stderr_print("====================   ===========   ==========\n" );
        _hppex_stderr_print("Element Count          %10d   %10d\n",
                            before_info.element_count, after_info.element_count );
        _hppex_stderr_print("Structure Size         %10d   %10d\n",
                            before_info.size, after_info.size );
        _hppex_stderr_print("Has References         %10d   %10d\n",
                            before_info.has_refs, after_info.has_refs );
        _hppex_stderr_print("Edit Mode              %10d   %10d\n",
                            before_info.edit_mode, after_info.edit_mode );
        _hppex_stderr_print("Element Pointer        %10d   %10d\n",
                            before_info.element_pointer, after_info.element_pointer );
        _hppex_stderr_print("====================   ===========   ==========\n" );

	_hppex_close_stderr();
	}
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
     image_testproc("pexocp.85.3", "pexocp.85.3", img_desc,
		      dpy,window, 0, 0, 
		      glob_window_width, glob_window_height);
    if (glob_test_mode != IMM_MODE)
	{
	file_testproc("pexocp.85.3.stderr", "pexocp.85.3");
	}
    }
