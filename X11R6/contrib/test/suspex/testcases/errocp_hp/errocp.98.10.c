/* $XConsortium: errocp.98.10.c,v 1.0 93/11/22 12:45:53 rws Exp $ */

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

#define		KNOT_COUNT				5
#define		ROW_COUNT				4
#define		COL_COUNT				4
#define		POINT_COUNT				(ROW_COUNT * COL_COUNT)
#define		TRIM_LOOP_COUNT			2
#define		TRIM_CURVE_SEG_COUNT	5
#define		TRIM_CURVE_POINT_COUNT	20

/*
 *	draw_image prototype
 */
void draw_image( Display *dpy, XID rid, PEXOCRequestType req_type );


/* 
 *	TEST: PEXNURBSurface test for trim curve parameter ranges that
 *	are inconsisten with the knot vector.
 */

void draw_image( Display *dpy, XID res_id, PEXOCRequestType req_type )
{	
	/*
	 *	Data declarations
	 */
	int					 rationality;
	int					 uorder;
	int					 vorder;
	float				 uknots[KNOT_COUNT];
	float				 vknots[KNOT_COUNT];
	unsigned int		 col_count;
	unsigned int		 row_count;
	PEXArrayOfCoord		 points;
	unsigned int		 curve_count;
	PEXListOfTrimCurve	 trim_curves;
	PEXCoord4D			 rational_ctlpts[POINT_COUNT];
	PEXTrimCurve		 trimming_curves;
	PEXCoord			 trim_seg_ctl_pts[TRIM_CURVE_POINT_COUNT];
	PEXListOfFloat		 knot_list;
	float				 knots[KNOT_COUNT];

	int					 indx;

	/*
	 *	Test for inconsistent parameter ranges.
	 */
	rationality		= PEXRational;
	uorder			= 1;
	vorder			= 1;
	col_count		= 4;	/* count + order = # of knots! */
	row_count		= 4;	/* count + order = # of knots! */
	curve_count		= 1;

	/*
	 *	Set up knot vector properly to get by the knot
	 *	sequence error check.
	 */
	uknots[0] = vknots[0] = 1.0;
	uknots[1] = vknots[1] = 2.0;
	uknots[2] = vknots[2] = 3.0;
	uknots[3] = vknots[3] = 4.0;
	uknots[4] = vknots[4] = 5.0;

	/*
	 *	Give all of the control points "reasonable" values.
	 */
	points.point_4d	= rational_ctlpts;
	for( indx = 0; indx < col_count * row_count; indx++ )
	{
		points.point_4d[indx].x = (float) indx;
		points.point_4d[indx].y = (float) indx;
		points.point_4d[indx].z = (float) indx;
		points.point_4d[indx].w = 1.0;
	}

	/*
	 *	Set up the trimming curve
	 */
	trim_curves.count							= 1;
	trim_curves.curves							= &trimming_curves;
	trim_curves.curves->visibility				= PEXOn;
	trim_curves.curves->order					= 2;
	trim_curves.curves->rationality				= PEXNonRational;
	trim_curves.curves->approx_method			= PEXApproxImpDep;
	trim_curves.curves->tolerance				= 1.0;
	trim_curves.curves->tmin					= 0.0;
	trim_curves.curves->tmax					= 0.0;
	trim_curves.curves->knots					= knot_list;
	trim_curves.curves->knots.count				= KNOT_COUNT;
	trim_curves.curves->knots.floats			= knots;
	trim_curves.curves->knots.floats[0]			= 1.0;
	trim_curves.curves->knots.floats[1]			= 2.0;
	trim_curves.curves->knots.floats[2]			= 3.0;
	trim_curves.curves->knots.floats[3]			= 4.0;
	trim_curves.curves->knots.floats[4]			= 5.0;
	trim_curves.curves->count					= 3;
	trim_curves.curves->control_points.point	= trim_seg_ctl_pts;

	for( indx = 0; indx < TRIM_CURVE_POINT_COUNT; indx++ )
	{
		trim_curves.curves->control_points.point[indx].x	= 1.0;
		trim_curves.curves->control_points.point[indx].y	= 1.0;
		trim_curves.curves->control_points.point[indx].z	= 1.0;
	}

	trim_curves.curves->tmin =
		trim_curves.curves->knots.floats[(trim_curves.curves->order)-1];
	trim_curves.curves->tmax =
		trim_curves.curves->knots.floats[trim_curves.curves->count];

	/*
	 *	Now, throw in the monkey wrench by sticking in values for
	 *	the parameter ranges which are inconsistent with the knot
	 *	vector.
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
	 *	Three PEXNURBSurface calls will be made to excercise all three
	 *	cases.
	 */

	_hppex_stderr_print("\n==============================================\n" );
	/* 
	 *	tmin < tmax                       =   4.0
	 *	tmin >= knot_vector[order-1]      >=  2.0
	 *	tmax <= knot_vector[control_pts]  <=  4.0
	 *
	 *	so, set tmin to be greater than tmax but not violate the
	 *	remaining two conditions.
	 *
	 *		tmin = 3.0  > tmax                     = 2.0  bad
	 *		tmin = 3.0 >= knot_vector[order-1]     = 2.0  OK
	 *		tmax = 2.0 <= knot_vector[control_pts] = 4.0  OK
	 */
	trim_curves.curves->tmin = 3.0;
	trim_curves.curves->tmax = 2.0;
	_hppex_stderr_print("   tmin = %3.2f > tmax = %3.2f\n",
		trim_curves.curves->tmin, trim_curves.curves->tmax );
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
		&trim_curves );
	_hppex_stderr_print("==============================================\n" );

	_hppex_stderr_print("\n==============================================\n" );
	/*
	 *	tmin < tmax                        =  4.0
	 *	tmin >= knot_vector[order-1]      >=  2.0
	 *	tmax <= knot_vector[control_pts]  <=  4.0
	 *
	 *	so, set tmin to be less than knot_vector[order-1]
	 *	but not violate the remaining two conditions.
	 *
	 *		tmin = 1.0  < tmax                     = 4.0  OK
	 *		tmin = 1.0 >= knot_vector[order-1]     = 2.0  bad
	 *		tmax = 4.0 <= knot_vector[control_pts] = 4.0  OK
	 */
	trim_curves.curves->tmin = 1.0;
	trim_curves.curves->tmax = 4.0;
	_hppex_stderr_print("   tmin = %3.2f < knot_vector[order-1] = %3.2f\n",
		trim_curves.curves->tmin,
		trim_curves.curves->knots.floats[trim_curves.curves->order-1] );
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
		&trim_curves );
	_hppex_stderr_print("==============================================\n" );

	_hppex_stderr_print("\n==============================================\n" );
	/*
	 *	tmin < tmax                        =  4.0
	 *	tmin >= knot_vector[order-1]      >=  2.0
	 *	tmax <= knot_vector[control_pts]  <=  4.0
	 *
	 *	so, set tmin to be less than knot_vector[order-1]
	 *	but not violate the remaining two conditions.
	 *
	 *		tmin = 1.0  < tmax                     = 5.0  OK
	 *		tmin = 2.0 >= knot_vector[order-1]     = 2.0  OK
	 *		tmax = 5.0 <= knot_vector[control_pts] = 4.0  bad
	 */
	trim_curves.curves->tmin = 1.0;
	trim_curves.curves->tmax = 5.0;
	_hppex_stderr_print("   tmax = %3.2f > knot_vector[control_pts] = %3.2f\n",
		trim_curves.curves->tmax,
		trim_curves.curves->knots.floats[trim_curves.curves->count] );
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
		&trim_curves );
	_hppex_stderr_print("==============================================\n" );
}

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "errocp.98.10");
    *num_images      = 1;
    *supported_modes = IMM_MODE | STR_MODE | WKS_MODE;

    strcpy(win_title, "errocp.98.10");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    if (glob_test_mode == IMM_MODE)
        _hppex_set_stderr("errocp.98.10.stderr_imm", "errocp.98.10");
    else
        _hppex_set_stderr("errocp.98.10.stderr_str", "errocp.98.10");
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    describe_test("\nOC Primitives Error Test (PEXNURBSurface)\n\n");
    describe_test("   Test for trim curve parameter ranges that are inconsistent with the knot vector.\n");
    describe_test("   The stderr output will contain messages for all\n");
    describe_test("   expected errors encountered.\n\n");
    fflush(stdout);

    draw_image(dpy, resourceID, req_type);
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
    if (glob_test_mode == IMM_MODE)
        file_testproc("errocp.98.10.stderr_imm", "errocp.98.10");
    else
        file_testproc("errocp.98.10.stderr_str", "errocp.98.10");
    }
