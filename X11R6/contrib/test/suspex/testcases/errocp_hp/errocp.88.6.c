/* $XConsortium: errocp.88.6.c,v 1.0 93/11/22 12:42:01 rws Exp $ */

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
	PEXCoord4D		 rational_ctlpts[10];

	int				 indx;

	/*
	 *	Test for rational control point being less than or equal
	 *	to zero.
	 */
	rationality		= PEXRational;
	order			= 1;
	knots[0]		= 1.0;
	knots[1]		= 2.0;
	knots[2]		= 3.0;
	knots[3]		= 4.0;
	knots[4]		= 5.0;
	count			= KNOT_COUNT - order; /* # knots = control pts. + order */
	tmin			= knots[order-1];
	tmax			= knots[count];

	points.point_4d	= rational_ctlpts;

	/*
	 *	Give all of the control points "reasonable" values.
	 */
	for( indx = 0; indx < count; indx++ )
	{
		rational_ctlpts[indx].x = (float) indx;
		rational_ctlpts[indx].y = (float) indx;
		rational_ctlpts[indx].z = (float) indx;
		rational_ctlpts[indx].w = 1.0;
	}

	_hppex_stderr_print("\n==============================================\n" );
	/*
	 *	Now, throw in the monkey wrench.
	 */
	rational_ctlpts[3].w = 0.0;
	_hppex_stderr_print("rational_ctlpts[3].w = %4.2f\n", rational_ctlpts[3].w );

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
	/*
	 *	Now, throw in another monkey wrench.
	 */
	rational_ctlpts[3].w = -1.0;
	_hppex_stderr_print("rational_ctlpts[3].w = %4.2f\n", rational_ctlpts[3].w );

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
    strcpy(test_name, "errocp.88.6");
    *num_images      = 1;
    *supported_modes = IMM_MODE | STR_MODE | WKS_MODE;

    strcpy(win_title, "errocp.88.6");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    if (glob_test_mode == IMM_MODE)
        _hppex_set_stderr("errocp.88.6.stderr_imm", "errocp.88.6");
    else
        _hppex_set_stderr("errocp.88.6.stderr_str", "errocp.88.6");
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    describe_test("\nOC Primitives Error Test (PEXNURBCurve)\n\n");
    describe_test("   test for tmin and tmax being inconsistent with the knot vector.\n");
    describe_test("   The stderr output will contain messages for all\n");
    describe_test("   expected errors encountered.\n\n");
    fflush(stdout);

    draw_image(dpy, resourceID, req_type);
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
    if (glob_test_mode == IMM_MODE)
        file_testproc("errocp.88.6.stderr_imm", "errocp.88.6");
    else
        file_testproc("errocp.88.6.stderr_str", "errocp.88.6");
    }
