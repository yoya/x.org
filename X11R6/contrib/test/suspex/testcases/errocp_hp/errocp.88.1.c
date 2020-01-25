/* $XConsortium: errocp.88.1.c,v 1.0 93/11/22 12:41:33 rws Exp $ */

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

#define		BogusRationality		2

/*
 *	draw_image prototype
 */
void draw_image( Display *dpy, XID rid, PEXOCRequestType req_type );


/* 
 *	TEST: PEXNURBCurve test for rationality parameter being
 *		undefined.
 */

void draw_image( Display *dpy, XID res_id, PEXOCRequestType req_type )
{	
	/*
	 *	Data declarations
	 */
	int				 rationality;
	int				 order;
	float			*knots;
	unsigned int	 count;
	PEXArrayOfCoord	 points;
	double			 tmin;
	double			 tmax;

	/*
	 *	Test for type parameter not being one of:
	 *
	 *		#define PEXRational                     0
	 *		#define PEXNonRational                  1
	 *
	 */
	rationality		= PEXNonRational;
	order			= 1;
	knots			= (float *) NULL;
	count			= 0;
	points.point	= (PEXCoord *) NULL;
	tmin			= 0;
	tmax			= 0;

	_hppex_stderr_print("\n==============================================\n" );
	rationality = BogusRationality;
	_hppex_stderr_print("  rationality = BogusRationality = 2\n" );
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
    strcpy(test_name, "errocp.88.1");
    *num_images      = 1;
    *supported_modes = IMM_MODE | STR_MODE | WKS_MODE;

    strcpy(win_title, "errocp.88.1");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    if (glob_test_mode == IMM_MODE)
        _hppex_set_stderr("errocp.88.1.stderr_imm", "errocp.88.1");
    else
        _hppex_set_stderr("errocp.88.1.stderr_str", "errocp.88.1");
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    describe_test("\nOC Primitives Error Test (PEXNURBCurve)\n\n");
    describe_test("   Test for rationality parameter being undefined \n");
    describe_test("   The stderr output will contain messages for all\n");
    describe_test("   expected errors encountered.\n\n");
    fflush(stdout);

    draw_image(dpy, resourceID, req_type);
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
    if (glob_test_mode == IMM_MODE)
        file_testproc("errocp.88.1.stderr_imm", "errocp.88.1");
    else
        file_testproc("errocp.88.1.stderr_str", "errocp.88.1");
    }
