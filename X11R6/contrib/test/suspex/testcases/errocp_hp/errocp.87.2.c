/* $XConsortium: errocp.87.2.c,v 1.0 93/11/22 12:41:27 rws Exp $ */

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

#define		BogusVertexAttribute		0b10011

/*
 *	draw_image prototype
 */
void draw_image( Display *dpy, XID rid, PEXOCRequestType req_type );


/* 
 *	TEST: PEXPolylineSetWithData - Undefined bit(s) in Vertex Attributes Mask
 */

void draw_image( Display *dpy, XID res_id, PEXOCRequestType req_type )
{	
	/*
	 *	Data declarations
	 */
	unsigned int	vertex_attributes	= PEXGANone;
	int				color_type;
	unsigned int	count				= 0;
	PEXListOfVertex	*vertex_lists		= NULL;

	/*
	 *	Test for Output Command Error for undefined bit set in
	 *	the vertex attribute bitmask.
	 *
	 *		Current defined bits in the vertex attribute
	 *		bitmask for PEXPolylineSetWithData are:
	 *
	 *		#define PEXGANone      0          No optional data
	 *		#define PEXGAColor     (1L<<0)    Color per vertex
	 *
	 *		Undefined bits are:
	 *
	 *		#define PEXGANormal    (1L<<1)    Illegal
	 *		#define PEXGAEdges     (1L<<2)    Illegal
	 *		Any bits other than 0 or 1        Illegal
	 */
	color_type = PEXColorTypeRGB;

	_hppex_stderr_print("\n==============================================\n" );
	vertex_attributes = PEXGANone;
	_hppex_stderr_print("  vertex_attributes = PEXGANone;\n" );
	PEXPolylineSetWithData(
		dpy,
		res_id,
		req_type,
		vertex_attributes,
		color_type,
		count,
		vertex_lists );
	_hppex_stderr_print("==============================================\n" );

	_hppex_stderr_print("\n==============================================\n" );
	vertex_attributes = PEXGAColor;
	_hppex_stderr_print("  vertex_attributes = PEXGAColor;\n" );
	PEXPolylineSetWithData(
		dpy,
		res_id,
		req_type,
		vertex_attributes,
		color_type,
		count,
		vertex_lists );
	_hppex_stderr_print("==============================================\n" );

	_hppex_stderr_print("\n==============================================\n" );
	vertex_attributes = PEXGANormal;
	_hppex_stderr_print("  vertex_attributes = PEXGANormal;\n" );
	PEXPolylineSetWithData(
		dpy,
		res_id,
		req_type,
		vertex_attributes,
		color_type,
		count,
		vertex_lists );
	_hppex_stderr_print("==============================================\n" );

	_hppex_stderr_print("\n==============================================\n" );
	vertex_attributes = PEXGAEdges;
	_hppex_stderr_print("  vertex_attributes = PEXGAEdges;\n" );
	PEXPolylineSetWithData(
		dpy,
		res_id,
		req_type,
		vertex_attributes,
		color_type,
		count,
		vertex_lists );
	_hppex_stderr_print("==============================================\n" );

	_hppex_stderr_print("\n==============================================\n" );
	vertex_attributes = PEXGAColor | PEXGANormal;
	_hppex_stderr_print("  vertex_attributes = PEXGAColor | PEXGANormal;\n" );
	PEXPolylineSetWithData(
		dpy,
		res_id,
		req_type,
		vertex_attributes,
		color_type,
		count,
		vertex_lists );
	_hppex_stderr_print("==============================================\n" );

	_hppex_stderr_print("\n==============================================\n" );
	vertex_attributes = PEXGAColor | PEXGAEdges;
	_hppex_stderr_print("  vertex_attributes = PEXGAColor | PEXGAEdges;\n" );
	PEXPolylineSetWithData(
		dpy,
		res_id,
		req_type,
		vertex_attributes,
		color_type,
		count,
		vertex_lists );
	_hppex_stderr_print("==============================================\n" );

	_hppex_stderr_print("\n==============================================\n" );
	vertex_attributes = PEXGANormal | PEXGAEdges;
	_hppex_stderr_print("  vertex_attributes = PEXGANormal | PEXGAEdges;\n" );
	PEXPolylineSetWithData(
		dpy,
		res_id,
		req_type,
		vertex_attributes,
		color_type,
		count,
		vertex_lists );
	_hppex_stderr_print("==============================================\n" );

	_hppex_stderr_print("\n==============================================\n" );
	vertex_attributes = PEXGAColor | PEXGANormal | PEXGAEdges;
	_hppex_stderr_print("  vertex_attributes = PEXGAColor | PEXGANormal | PEXGAEdges;\n" );
	PEXPolylineSetWithData(
		dpy,
		res_id,
		req_type,
		vertex_attributes,
		color_type,
		count,
		vertex_lists );
	_hppex_stderr_print("==============================================\n" );

}

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "errocp.87.2");
    *num_images      = 1;
    *supported_modes = IMM_MODE | STR_MODE | WKS_MODE;

    strcpy(win_title, "errocp.87.2");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    if (glob_test_mode == IMM_MODE)
        _hppex_set_stderr("errocp.87.2.stderr_imm", "errocp.87.2");
    else
        _hppex_set_stderr("errocp.87.2.stderr_str", "errocp.87.2");
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    describe_test("\nOC Primitives Error Test (PEXPolylineSetWithData)\n\n");
    describe_test("   Undefined bit(s) in Vertex Attributes Mask\n");
    describe_test("   The stderr output will contain messages for all\n");
    describe_test("   expected errors encountered.\n\n");
    fflush(stdout);

    draw_image(dpy, resourceID, req_type);
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
    if (glob_test_mode == IMM_MODE)
        file_testproc("errocp.87.2.stderr_imm", "errocp.87.2");
    else
        file_testproc("errocp.87.2.stderr_str", "errocp.87.2");
    }
