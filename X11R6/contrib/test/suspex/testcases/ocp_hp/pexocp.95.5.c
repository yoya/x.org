/* $XConsortium: pexocp.95.5.c,v 1.0 93/11/22 12:36:57 rws Exp $ */

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

#define		VERTEX_COUNT		5

/*
 *	draw_image prototype
 */
void draw_image( Display *dpy, XID rid, PEXOCRequestType req_type );

/* 
 *	TEST: Boundary test for PEXTriangleStrip with fewer
 *	than three vertices.
 */

void draw_image( Display *dpy, XID res_id, PEXOCRequestType req_type )
{	
	/*
	 *	Data declarations
	 */
	unsigned int			facet_attributes;
	unsigned int			vertex_attributes;
	int						color_type;
	PEXArrayOfFacetData		facet_data;
	unsigned int			count;
	PEXArrayOfVertex		vertices;
	PEXCoord				vertex_list[VERTEX_COUNT];

	facet_attributes		= PEXGANone;
	vertex_attributes		= PEXGANone;
	color_type				= PEXColorTypeRGB;
	facet_data.index		= (PEXColorIndexed *) NULL;
	count					= 2;
	vertices.no_data		= vertex_list;

	/*
	 *	Set up the vertex points
	 */
	vertex_list[0].x = 0.1; vertex_list[0].y = 0.8; vertex_list[0].z = 0.0;

	vertex_list[1].x = 0.3; vertex_list[1].y = 0.2; vertex_list[1].z = 1.0;

	vertex_list[2].x = 0.5; vertex_list[2].y = 0.8; vertex_list[2].z = 0.1;

	vertex_list[3].x = 0.7; vertex_list[3].y = 0.2; vertex_list[3].z = 1.0;

	vertex_list[4].x = 0.9; vertex_list[4].y = 0.8; vertex_list[4].z = 0.0;

	PEXTriangleStrip(
		dpy,
		res_id,
		req_type,
		facet_attributes,
		vertex_attributes,
		color_type,
		facet_data,
		count,
		vertices );
}


void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "pexocp.95.5");
    *num_images      = 1;
    *supported_modes = IMM_MODE | STR_MODE | WKS_MODE;

    strcpy(win_title, "pexocp.95.5");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    unsigned long         mask = PEXRAHLHSRMode;
    PEXRendererAttributes attrs;

    attrs.hlhsr_mode = PEXHLHSRZBuffer;
    PEXChangeRenderer(dpy, renderer, mask, &attrs);

    if (glob_test_mode != IMM_MODE)
        _hppex_set_stderr("pexocp.95.5.stderr", "pexocp.95.5");
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    if (glob_test_mode == IMM_MODE)
	{
        describe_test( "\nPEXTriangleStrip Boundry Condition Test:\n" );
        describe_test( "\n" );
        describe_test( "  Triangle Strip with fewer than 3 vertices. \n");
        describe_test( "  A blank image should be created.\n\n");
	
        draw_image( dpy, resourceID, req_type );
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
        describe_test( "  1 element was added to the structure.\n" );
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
	 *	Dump the results of the queries to stder
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
     image_testproc("pexocp.95.5", "pexocp.95.5", img_desc,
		      dpy,window, 0, 0, 
		      glob_window_width, glob_window_height);
    if (glob_test_mode != IMM_MODE)
	{
	file_testproc("pexocp.95.5.stderr", "pexocp.95.5");
	}
    }
