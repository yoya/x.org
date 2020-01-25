/* $XConsortium: errocp.88.5.c,v 1.0 93/11/22 12:41:55 rws Exp $ */

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

#define		KNOT_COUNT		5

/*
 *	draw_image prototype
 */
void draw_image( Display *dpy, XID rid, PEXOCRequestType req_type );


/* 
 *	TEST: PEXNURBCurve test for tmin and tmax being inconsistent
 *		with the knot vector.
 */

void draw_image( Display *dpy, XID res_id, PEXOCRequestType req_type )
{	
	/*
	 *	Data declarations
	 */
	int				 rationality;
	int				 order;
	float			 knots[KNOT_COUNT];
	unsigned int	 count;
	PEXArrayOfCoord	 points;
	double			 tmin;
	double			 tmax;

	/*
	 *	Test for tmin and/or tmax not being consistent with
	 *	the knot vector.
	 *
	 *		tmin, tmax and the knots must meet the criteria
	 *		of:
	 *
	 *      1.  tmin must be less than tmax
	 *             tmin < tmax
	 *
	 *      2.  tmin must be greater than or equal to the order'th knot value
	 *             tmin >= knot_vector[order-1]
	 *
	 *      3.  tmax must be less than or equal to the k+1-order'th knot value
	 *          where k is the number of knots (control points + order)
	 *             tmax <= knot_vector[(order+control_pts)+1-(order-1)]
	 *                -or-
	 *             tmax <= knot_vector[control_pts]
	 *
	 *	Three PEXNURBCurve calls will be made to excercise all three
	 *	cases
	 */
	rationality		= PEXNonRational;
	order			= 1;
	knots[0]		= 1.0;
	knots[1]		= 2.0;
	knots[2]		= 3.0;
	knots[3]		= 4.0;
	knots[4]		= 5.0;
	count			= KNOT_COUNT - order; /* # knots = control pts. + order */
	points.point	= (PEXCoord *) NULL;
	tmin			= 0;
	tmax			= 0;

	_hppex_stderr_print("\n==============================================\n" );
	tmin = 3.0;
	tmax = 2.0;
	_hppex_stderr_print("Error 1: tmin = %3.2f > tmax = %3.2f\n", tmin, tmax );
	PEXNURBCurve(
		dpy,
		res_id,
		req_type,
		rationality,
		order,
		knots,
		count,
		points,
		tmin,
		tmax );
	_hppex_stderr_print("==============================================\n" );

	_hppex_stderr_print("\n==============================================\n" );
	tmin = 0.0;
	tmax = 1.0;
	_hppex_stderr_print("Error 2: tmin = %3.2f < knots[order-1] = %3.2f\n", tmin,
		knots[order-1] );
	PEXNURBCurve(
		dpy,
		res_id,
		req_type,
		rationality,
		order,
		knots,
		count,
		points,
		tmin,
		tmax );
	_hppex_stderr_print("==============================================\n" );

	_hppex_stderr_print("\n==============================================\n" );
	tmin =  1.0;
	tmax = 10.0;
	_hppex_stderr_print("Error 3: tmax = %3.2f > knot_vector[control_pts] = %3.2f\n",
		tmax, knots[count] );
	PEXNURBCurve(
		dpy,
		res_id,
		req_type,
		rationality,
		order,
		knots,
		count,
		points,
		tmin,
		tmax );
	_hppex_stderr_print("==============================================\n" );
}

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "errocp.88.5");
    *num_images      = 1;
    *supported_modes = IMM_MODE | STR_MODE | WKS_MODE;

    strcpy(win_title, "errocp.88.5");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    if (glob_test_mode == IMM_MODE)
        _hppex_set_stderr("errocp.88.5.stderr_imm", "errocp.88.5");
    else
        _hppex_set_stderr("errocp.88.5.stderr_str", "errocp.88.5");
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    describe_test("\nOC Primitives Error Test (PEXNURBCurve)\n\n");
    describe_test("   Test for tmin and tmax being inconsistent with the knot vector. \n");
    describe_test("   The stderr output will contain messages for all\n");
    describe_test("   expected errors encountered.\n\n");
    fflush(stdout);

    draw_image(dpy, resourceID, req_type);
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
    if (glob_test_mode == IMM_MODE)
        file_testproc("errocp.88.5.stderr_imm", "errocp.88.5");
    else
        file_testproc("errocp.88.5.stderr_str", "errocp.88.5");
    }
