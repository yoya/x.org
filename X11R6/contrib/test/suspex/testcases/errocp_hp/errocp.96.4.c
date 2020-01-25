/* $XConsortium: errocp.96.4.c,v 1.0 93/11/22 12:44:43 rws Exp $ */

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

#define		BogusFacetAttribute		(PEXGAColor | PEXGANormal | PEXGAEdges)<<1

/*
 *	draw_image prototype
 */
void draw_image( Display *dpy, XID rid, PEXOCRequestType req_type );


/* 
 *	TEST: PEXQuadrilateralMesh - Test for undefined bit(s) in facet
 *			Attributes bitmask
 */

void draw_image( Display *dpy, XID res_id, PEXOCRequestType req_type )
{	
	/*
	 *	Data declarations
	 */
	int					shape_hint;
	unsigned int		facet_attributes;
	unsigned int		vertex_attributes;
	int					color_type;
	PEXArrayOfFacetData	facet_data;
	unsigned int		col_count;
	unsigned int		row_count;
	PEXArrayOfVertex	vertices;

	/*
	 *	Test for Output Command Error for undefined bit set in
	 *	the facet attribute bitmask.
	 *
	 *		Current defined bits in the facet attribute
	 *		bitmask for PEXQuadrilateralMesh are:
	 *
	 *		#define PEXGANone      0          No optional data
	 *		#define PEXGAColor     (1L<<0)    Color per facet
	 *		#define PEXGANormal    (1L<<1)    Normal per facet
	 *
	 *		Undefined bits are:
	 *
	 *		#define PEXGAEdges     (1L<<2)    Illegal
	 *
	 */
	shape_hint			= PEXShapeUnknown;
	facet_attributes	= PEXGANone;
	vertex_attributes	= PEXGANone;
	color_type			= PEXColorTypeRGB;
	facet_data.index	= (PEXColorIndexed *) NULL;
	col_count			= 0;
	row_count			= 0;
	vertices.no_data	= (PEXCoord *) NULL;

	_hppex_stderr_print("\n==============================================\n" );
	facet_attributes = PEXGANone;
	_hppex_stderr_print("  facet_attributes = PEXGANone;\n" );
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
	_hppex_stderr_print("==============================================\n" );

	_hppex_stderr_print("\n==============================================\n" );
	facet_attributes = PEXGAColor;
	_hppex_stderr_print("  facet_attributes = PEXGAColor;\n" );
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
	_hppex_stderr_print("==============================================\n" );

	_hppex_stderr_print("\n==============================================\n" );
	facet_attributes = PEXGANormal;
	_hppex_stderr_print("  facet_attributes = PEXGANormal;\n" );
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
	_hppex_stderr_print("==============================================\n" );

	_hppex_stderr_print("\n==============================================\n" );
	facet_attributes = PEXGAEdges;
	_hppex_stderr_print("  facet_attributes = PEXGAEdges;\n" );
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
	_hppex_stderr_print("==============================================\n" );

	_hppex_stderr_print("\n==============================================\n" );
	facet_attributes = PEXGAColor | PEXGANormal;
	_hppex_stderr_print("  facet_attributes = PEXGAColor | PEXGANormal;\n" );
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
	_hppex_stderr_print("==============================================\n" );

	_hppex_stderr_print("\n==============================================\n" );
	facet_attributes = PEXGAColor | PEXGAEdges;
	_hppex_stderr_print("  facet_attributes = PEXGAColor | PEXGAEdges;\n" );
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
	_hppex_stderr_print("==============================================\n" );

	_hppex_stderr_print("\n==============================================\n" );
	facet_attributes = PEXGANormal | PEXGAEdges;
	_hppex_stderr_print("  facet_attributes = PEXGANormal | PEXGAEdges;\n" );
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
	_hppex_stderr_print("==============================================\n" );

	_hppex_stderr_print("\n==============================================\n" );
	facet_attributes = PEXGAColor | PEXGANormal | PEXGAEdges;
	_hppex_stderr_print("  facet_attributes = PEXGAColor | PEXGANormal | PEXGAEdges;\n" );
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
	_hppex_stderr_print("==============================================\n" );

	_hppex_stderr_print("\n==============================================\n" );
	facet_attributes = BogusFacetAttribute;
	_hppex_stderr_print("  facet_attributes = BogusFacetAttribute;\n" );
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
	_hppex_stderr_print("==============================================\n" );

}

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "errocp.96.4");
    *num_images      = 1;
    *supported_modes = IMM_MODE | STR_MODE | WKS_MODE;

    strcpy(win_title, "errocp.96.4");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    if (glob_test_mode == IMM_MODE)
        _hppex_set_stderr("errocp.96.4.stderr_imm", "errocp.96.4");
    else
        _hppex_set_stderr("errocp.96.4.stderr_str", "errocp.96.4");
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    describe_test("\nOC Primitives Error Test (PEXQuadrilateralMesh)\n\n");
    describe_test("   Test for undefined bit(s) in facet Attributes bitmask\n");
    describe_test("   The stderr output will contain messages for all\n");
    describe_test("   expected errors encountered.\n\n");
    fflush(stdout);

    draw_image(dpy, resourceID, req_type);
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
    if (glob_test_mode == IMM_MODE)
        file_testproc("errocp.96.4.stderr_imm", "errocp.96.4");
    else
        file_testproc("errocp.96.4.stderr_str", "errocp.96.4");
    }
