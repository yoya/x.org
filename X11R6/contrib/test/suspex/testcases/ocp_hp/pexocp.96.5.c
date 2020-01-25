/* $XConsortium: pexocp.96.5.c,v 1.0 93/11/22 12:37:26 rws Exp $ */

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

#define		ROW_COUNT		3
#define		COL_COUNT		3

/*
 *	draw_image prototype
 */
void draw_image( Display *dpy, XID rid, PEXOCRequestType req_type );


/* 
 *	TEST: Boundary test for PEXQuadrilateralMesh with fewer
 *	than three vertices.
 */

void draw_image( Display *dpy, XID res_id, PEXOCRequestType req_type )
{	
	/*
	 *	Data declarations
	 */
	int						shape_hint;
	unsigned int			facet_attributes;
	unsigned int			vertex_attributes;
	int						color_type;
	PEXArrayOfFacetData		facet_data;
	unsigned int			col_count;
	unsigned int			row_count;
	PEXArrayOfVertex		vertices;

	PEXCoord				vertex_list[ROW_COUNT*COL_COUNT];
	float					x, y;
	int						row, col, indx;

	shape_hint				= PEXShapeUnknown;
	facet_attributes		= PEXGANone;
	vertex_attributes		= PEXGANone;
	color_type				= PEXColorTypeRGB;
	facet_data.index		= (PEXColorIndexed *) NULL;
	vertices.no_data		= vertex_list;

	/*
	 *	Set up the vertex points
	 */
	y = 0.8;
	for( row = 0; row < ROW_COUNT; row++ )
	{
		x = 0.1;
		for( col = 0; col < COL_COUNT; col++ )
		{
			indx = row * COL_COUNT + col;
			vertex_list[indx].x = x;
			vertex_list[indx].y = y;
			vertex_list[indx].z = 0.0;
			x += 0.4;
		}
		y -= 0.3;
	}

	/*
	 *	Set the col_count to less than 2.
	 */
	row_count	= ROW_COUNT;
	col_count	= 1;

	PEXQuadrilateralMesh(
		dpy,
		res_id,
		req_type,
		shape_hint,
		facet_attributes,
		vertex_attributes,
		color_type,
		facet_data,
		col_count,
		row_count,
		vertices );

	/*
	 *	Set the row_count to less than 2.
	 */
	col_count	= COL_COUNT;
	row_count	= 1;

	PEXQuadrilateralMesh(
		dpy,
		res_id,
		req_type,
		shape_hint,
		facet_attributes,
		vertex_attributes,
		color_type,
		facet_data,
		col_count,
		row_count,
		vertices );

}

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "pexocp.96.5");
    *num_images      = 1;
    *supported_modes = IMM_MODE | STR_MODE | WKS_MODE;

    strcpy(win_title, "pexocp.96.5");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    unsigned long         mask = PEXRAHLHSRMode;
    PEXRendererAttributes attrs;

    attrs.hlhsr_mode = PEXHLHSRZBuffer;
    PEXChangeRenderer(dpy, renderer, mask, &attrs);

    if (glob_test_mode != IMM_MODE)
        _hppex_set_stderr("pexocp.96.5.stderr", "pexocp.96.5");
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    if (glob_test_mode == IMM_MODE)
	{
        describe_test( "\nPEXQuadilateralMesh Boundry Condition Test:\n\n" );
        describe_test( "  Quad mesh with m and n  < 2. \n");
        describe_test( "  A blank image should be created.\n\n");
	
        draw_image( dpy, resourceID, req_type );
	}
    else
	{
	unsigned long    value_mask;
	int              float_format = PEXIEEE_754_32;
	PEXStructureInfo before_info, after_info;

        describe_test( "\n" );
        describe_test( "\nPEXQuadilateralMesh Boundry Condition Test:\n\n" );
        describe_test( "  Quad mesh with m and n  < 2. \n");
        describe_test( "  Even though a degenerate primitive exists, it should still\n" );
        describe_test( "  be stored in a structure.  Therefore, the before and after\n" );
        describe_test( "  numbers (returned by a structure query) should show that\n" );
        describe_test( "  2 elements was added to the structure.\n" );
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
     image_testproc("pexocp.96.5", "pexocp.96.5", img_desc,
		      dpy,window, 0, 0, 
		      glob_window_width, glob_window_height);
    if (glob_test_mode != IMM_MODE)
	{
	file_testproc("pexocp.96.5.stderr", "pexocp.96.5");
	}
    }
