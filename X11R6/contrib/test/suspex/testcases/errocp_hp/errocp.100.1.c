/* $XConsortium: errocp.100.1.c,v 1.0 93/11/22 12:41:10 rws Exp $ */

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

/*
 *	draw_image prototype
 */
void draw_image( Display *dpy, XID rid, PEXOCRequestType req_type );


/* 
 *	TEST: PEXCellArray2D - Test for row or column count < 1
 */

void draw_image( Display *dpy, XID res_id, PEXOCRequestType req_type )
{	
	/*
	 *	Data declarations
	 */
	PEXCoord2D		*point1;
	PEXCoord2D		*point2;
	unsigned int	 col_count;
	unsigned int	 row_count;
	PEXTableIndex	*color_indices;

	/*
	 *	Test for Output Command Error for row_count or
	 *	col_count being less than or equal to zero.
	 */
	point1			= (PEXCoord2D *) NULL;
	point2			= (PEXCoord2D *) NULL;
	col_count		= 1;
	row_count		= 1;
	color_indices	= (PEXTableIndex *) NULL;

	_hppex_stderr_print("\n==============================================\n" );
	_hppex_stderr_print("  row_count = 0\n" );
	row_count = 0;
	col_count = 1;
	PEXCellArray2D(
		dpy,
		res_id,
		req_type,
		point1,
		point2,
		col_count,
		row_count,
		color_indices );
	_hppex_stderr_print("==============================================\n" );

	_hppex_stderr_print("\n==============================================\n" );
	_hppex_stderr_print("  col_count = 0\n" );
	col_count = 0;
	row_count = 1;
	PEXCellArray2D(
		dpy,
		res_id,
		req_type,
		point1,
		point2,
		col_count,
		row_count,
		color_indices );
	_hppex_stderr_print("==============================================\n" );

}

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "errocp.100.1");
    *num_images      = 1;
    *supported_modes = IMM_MODE | STR_MODE | WKS_MODE;

    strcpy(win_title, "errocp.100.1");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    if (glob_test_mode == IMM_MODE)
        _hppex_set_stderr("errocp.100.1.stderr_imm", "errocp.100.1");
    else
        _hppex_set_stderr("errocp.100.1.stderr_str", "errocp.100.1");
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    describe_test("\nOC Primitives Error Test (CellArray2D)\n\n");
    describe_test("   Test for row or column count less than one\n");
    describe_test("   The stderr output will contain messages for all\n");
    describe_test("   expected errors encountered.\n\n");
    fflush(stdout);

    draw_image(dpy, resourceID, req_type);
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
    if (glob_test_mode == IMM_MODE)
        file_testproc("errocp.100.1.stderr_imm", "errocp.100.1");
    else
        file_testproc("errocp.100.1.stderr_str", "errocp.100.1");
    }
