/* $XConsortium: errocp.98.4.c,v 1.0 93/11/22 12:46:10 rws Exp $ */

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

#define		U_KNOT_COUNT		5
#define		V_KNOT_COUNT		5

/*
 *	draw_image prototype
 */
void draw_image( Display *dpy, XID rid, PEXOCRequestType req_type );


/* 
 *	TEST: PEXNURBSurface test for a not non-decreasing uknot or vknot
 *		sequence. (say that 3 time real fast)
 */

void draw_image( Display *dpy, XID res_id, PEXOCRequestType req_type )
{	
	/*
	 *	Data declarations
	 */
	int					 rationality;
	int					 uorder;
	int					 vorder;
	float				 uknots[U_KNOT_COUNT];
	float				 vknots[V_KNOT_COUNT];
	unsigned int		 col_count;
	unsigned int		 row_count;
	PEXArrayOfCoord		 points;
	unsigned int		 curve_count;
	PEXListOfTrimCurve	*trim_curves;

	/*
	 *	Test for a decreasing knot vector.
	 */
	rationality		= PEXNonRational;
	uorder			= 1;
	vorder			= 1;
	col_count		= 4;	/* count + order = # of knots! */
	row_count		= 4;	/* count + order = # of knots! */
	points.point	= (PEXCoord *) NULL;
	curve_count		= 0;
	trim_curves		= (PEXListOfTrimCurve *) NULL;

	uknots[0] = vknots[0] = 1.0;
	uknots[1] = vknots[1] = 2.0;
	uknots[2] = vknots[2] = 3.0;
	uknots[3] = vknots[3] = 4.0;
	uknots[4] = vknots[4] = 5.0;

	_hppex_stderr_print("\n==============================================\n" );
	uknots[3] = 1.0;
	_hppex_stderr_print("  uknots[] = %3.2f, %3.2f, %3.2f, %3.2f, %3.2f\n", uknots[0],
				uknots[1], uknots[2], uknots[3], uknots[4] );
	PEXNURBSurface(
		dpy,
		res_id,
		req_type,
		rationality,
		uorder,
		vorder,
		uknots,
		vknots,
		col_count,
		row_count,
		points,
		curve_count,
		trim_curves );
	_hppex_stderr_print("==============================================\n" );

	_hppex_stderr_print("\n==============================================\n" );
	uknots[3] = 4.0;
	vknots[3] = 1.0;
	_hppex_stderr_print("  vknots[] = %3.2f, %3.2f, %3.2f, %3.2f, %3.2f\n", vknots[0],
				vknots[1], vknots[2], vknots[3], vknots[4] );
	PEXNURBSurface(
		dpy,
		res_id,
		req_type,
		rationality,
		uorder,
		vorder,
		uknots,
		vknots,
		col_count,
		row_count,
		points,
		curve_count,
		trim_curves );
	_hppex_stderr_print("==============================================\n" );
}

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "errocp.98.4");
    *num_images      = 1;
    *supported_modes = IMM_MODE | STR_MODE | WKS_MODE;

    strcpy(win_title, "errocp.98.4");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    if (glob_test_mode == IMM_MODE)
        _hppex_set_stderr("errocp.98.4.stderr_imm", "errocp.98.4");
    else
        _hppex_set_stderr("errocp.98.4.stderr_str", "errocp.98.4");
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    describe_test("\nOC Primitives Error Test (PEXNURBSurface)\n\n");
    describe_test("   Test for a not non-decreasing uknot or vknot sequence.\n");
    describe_test("   The stderr output will contain messages for all\n");
    describe_test("   expected errors encountered.\n\n");
    fflush(stdout);

    draw_image(dpy, resourceID, req_type);
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
    if (glob_test_mode == IMM_MODE)
        file_testproc("errocp.98.4.stderr_imm", "errocp.98.4");
    else
        file_testproc("errocp.98.4.stderr_str", "errocp.98.4");
    }
