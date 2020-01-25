/* $XConsortium: errocp.98.9.c,v 1.0 93/11/22 12:46:39 rws Exp $ */

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
 *	TEST: PEXNURBSurface test for not non-decreasing knot sequence
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
	 *	Test for decreasing knot sequence error.
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


	_hppex_stderr_print("\n==============================================\n" );
	/*
	 *	Now, throw in the monkey wrench by sticking in a value in
	 *	the knot vector that makes it a decreasing sequence.
	 */
	trim_curves.curves->knots.floats[3] = 1.0;
	_hppex_stderr_print("   trim_curves.curves->knots.floats[0] = %3.2f\n",
		trim_curves.curves->knots.floats[0] );
	_hppex_stderr_print("   trim_curves.curves->knots.floats[1] = %3.2f\n",
		trim_curves.curves->knots.floats[1] );
	_hppex_stderr_print("   trim_curves.curves->knots.floats[2] = %3.2f\n",
		trim_curves.curves->knots.floats[2] );
	_hppex_stderr_print("   trim_curves.curves->knots.floats[3] = %3.2f\n",
		trim_curves.curves->knots.floats[3] );
	_hppex_stderr_print("   trim_curves.curves->knots.floats[4] = %3.2f\n",
		trim_curves.curves->knots.floats[4] );
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
    strcpy(test_name, "errocp.98.9");
    *num_images      = 1;
    *supported_modes = IMM_MODE | STR_MODE | WKS_MODE;

    strcpy(win_title, "errocp.98.9");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    if (glob_test_mode == IMM_MODE)
        _hppex_set_stderr("errocp.98.9.stderr_imm", "errocp.98.9");
    else
        _hppex_set_stderr("errocp.98.9.stderr_str", "errocp.98.9");
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    describe_test("\nOC Primitives Error Test (PEXNURBSurface)\n\n");
    describe_test("   Test for not non-decreasing knot sequence \n");
    describe_test("   The stderr output will contain messages for all\n");
    describe_test("   expected errors encountered.\n\n");
    fflush(stdout);

    draw_image(dpy, resourceID, req_type);
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
    if (glob_test_mode == IMM_MODE)
        file_testproc("errocp.98.9.stderr_imm", "errocp.98.9");
    else
        file_testproc("errocp.98.9.stderr_str", "errocp.98.9");
    }
