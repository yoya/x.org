/* $XConsortium: errocp.98.5.c,v 1.0 93/11/22 12:46:16 rws Exp $ */

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
#define		ROW_COUNT		4
#define		COL_COUNT		4
#define		POINT_COUNT		(ROW_COUNT * COL_COUNT)

/*
 *	draw_image prototype
 */
void draw_image( Display *dpy, XID rid, PEXOCRequestType req_type );


/* 
 *	TEST: PEXNURBSurface test for tmin and tmax being inconsistent
 *		with the knot vector.
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
	PEXListOfTrimCurve	*trim_curves;
	PEXCoord4D			 rational_ctlpts[POINT_COUNT];

	int				 indx;

	/*
	 *	Test for rational control point being less than or equal
	 *	to zero.
	 */
	rationality		= PEXRational;
	uorder			= 1;
	vorder			= 1;
	col_count		= 4;	/* count + order = # of knots! */
	row_count		= 4;	/* count + order = # of knots! */
	curve_count		= 0;
	trim_curves		= (PEXListOfTrimCurve *) NULL;

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
	/*
	 *	Now, throw in another monkey wrench.
	 */
	rational_ctlpts[3].w = -1.0;
	_hppex_stderr_print("rational_ctlpts[3].w = %4.2f\n", rational_ctlpts[3].w );

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
    strcpy(test_name, "errocp.98.5");
    *num_images      = 1;
    *supported_modes = IMM_MODE | STR_MODE | WKS_MODE;

    strcpy(win_title, "errocp.98.5");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    if (glob_test_mode == IMM_MODE)
        _hppex_set_stderr("errocp.98.5.stderr_imm", "errocp.98.5");
    else
        _hppex_set_stderr("errocp.98.5.stderr_str", "errocp.98.5");
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    describe_test("\nOC Primitives Error Test (PEXNURBSurface)\n\n");
    describe_test("   Test for tmin and tmax being inconsistent with the knot vector.\n");
    describe_test("   The stderr output will contain messages for all\n");
    describe_test("   expected errors encountered.\n\n");
    fflush(stdout);

    draw_image(dpy, resourceID, req_type);
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
    if (glob_test_mode == IMM_MODE)
        file_testproc("errocp.98.5.stderr_imm", "errocp.98.5");
    else
        file_testproc("errocp.98.5.stderr_str", "errocp.98.5");
    }
