/* $XConsortium: errocp.89.1.c,v 1.0 93/11/22 12:42:07 rws Exp $ */

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

#define		BogusShapeHint		4

/*
 *	draw_image prototype
 */
void draw_image( Display *dpy, XID rid, PEXOCRequestType req_type );


/* 
 *	TEST: PEXFillArea - Test for undefined Shape Hint
 */

void draw_image( Display *dpy, XID res_id, PEXOCRequestType req_type )
{	
	/*
	 *	Data declarations
	 */
	int					 shape_hint;
	int					 ignore_edges;
	unsigned int		 count;
	PEXCoord			*points;


	/*
	 *	Test for Output Command Error for undefined shape
	 *	hint.
	 *
	 *		Current defined shape hints in the PEX header files
	 *		PEXFillArea are:
	 *
	 *		#define PEXShapeComplex     0
	 *		#define PEXShapeNonConvex   1
	 *		#define PEXShapeConvex      2
	 *		#define PEXShapeUnknown     3
	 */
	ignore_edges	= True;
	count			= 0;
	points			= (PEXCoord *) NULL;


	_hppex_stderr_print("\n==============================================\n" );
	_hppex_stderr_print("  shape_hint = PEXShapeComplex\n" );
	shape_hint = PEXShapeComplex;
	PEXFillArea(
		dpy,
		res_id,
		req_type,
		shape_hint,
		ignore_edges,
		count,
		points);
	_hppex_stderr_print("==============================================\n" );


	_hppex_stderr_print("\n==============================================\n" );
	_hppex_stderr_print("  shape_hint = PEXShapeNonConvex\n" );
	shape_hint = PEXShapeNonConvex;
	PEXFillArea(
		dpy,
		res_id,
		req_type,
		shape_hint,
		ignore_edges,
		count,
		points);
	_hppex_stderr_print("==============================================\n" );


	_hppex_stderr_print("\n==============================================\n" );
	_hppex_stderr_print("  shape_hint = PEXShapeConvex\n" );
	shape_hint = PEXShapeConvex;
	PEXFillArea(
		dpy,
		res_id,
		req_type,
		shape_hint,
		ignore_edges,
		count,
		points);
	_hppex_stderr_print("==============================================\n" );


	_hppex_stderr_print("\n==============================================\n" );
	_hppex_stderr_print("  shape_hint = PEXShapeUnknown\n" );
	shape_hint = PEXShapeUnknown;
	PEXFillArea(
		dpy,
		res_id,
		req_type,
		shape_hint,
		ignore_edges,
		count,
		points);
	_hppex_stderr_print("==============================================\n" );

	_hppex_stderr_print("\n==============================================\n" );
	_hppex_stderr_print("  shape_hint = BogusShapeHint\n" );
	shape_hint = BogusShapeHint;
	PEXFillArea(
		dpy,
		res_id,
		req_type,
		shape_hint,
		ignore_edges,
		count,
		points);
	_hppex_stderr_print("==============================================\n" );

}

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "errocp.89.1");
    *num_images      = 1;
    *supported_modes = IMM_MODE | STR_MODE | WKS_MODE;

    strcpy(win_title, "errocp.89.1");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    if (glob_test_mode == IMM_MODE)
        _hppex_set_stderr("errocp.89.1.stderr_imm", "errocp.89.1");
    else
        _hppex_set_stderr("errocp.89.1.stderr_str", "errocp.89.1");
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    describe_test("\nOC Primitives Error Test (PEXFillArea)\n\n");
    describe_test("   Test for undefined Shape Hint \n");
    describe_test("   The stderr output will contain messages for all\n");
    describe_test("   expected errors encountered.\n\n");
    fflush(stdout);

    draw_image(dpy, resourceID, req_type);
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
    if (glob_test_mode == IMM_MODE)
        file_testproc("errocp.89.1.stderr_imm", "errocp.89.1");
    else
        file_testproc("errocp.89.1.stderr_str", "errocp.89.1");
    }
