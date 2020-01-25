/* $XConsortium: errocp.94.3.c,v 1.0 93/11/22 12:43:45 rws Exp $ */

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

#define		BogusContourHint		4

/*
 *	draw_image prototype
 */
void draw_image( Display *dpy, XID rid, PEXOCRequestType req_type );


/* 
 *	TEST: PEXFillAreaSetWithData - Test for bad Contour Hint
 */

void draw_image( Display *dpy, XID res_id, PEXOCRequestType req_type )
{	
	/*
	 *	Data declarations
	 */
	int					 shape_hint;
	int					 ignore_edges;
	int					 contour_hint;
	unsigned int		 facet_attributes;
	unsigned int		 vertex_attributes;
	int					 color_type;
	unsigned int		 count;
	PEXFacetData		*facet_data;
	PEXListOfVertex		*vertex_lists;

	/*
	 *	Test for Output Command Error for bad contour_hint
	 *	parameter.
	 *
	 *		Current defined contour hints in the PEX header files
	 *		PEXFillAreaSetWithData are:
	 *
	 *			#define PEXContourDisjoint       0
	 *			#define PEXContourNested         1
	 *			#define PEXContourIntersecting   2
	 *			#define PEXContourUnknown        3
	 *
	 */
	shape_hint			= PEXShapeUnknown;
	ignore_edges		= True;
	contour_hint		= PEXContourUnknown;
	vertex_attributes	= PEXGANone;
	facet_attributes	= PEXGANone;
	color_type			= PEXColorTypeRGB;
	count				= 0;
	vertex_lists		= (PEXListOfVertex *) NULL;

	_hppex_stderr_print("\n==============================================\n" );
	_hppex_stderr_print("  contour_hint = PEXContourDisjoint\n" );
	contour_hint = PEXContourDisjoint;
	PEXFillAreaSetWithData(
		dpy,
		res_id,
		req_type,
		shape_hint,
		ignore_edges, 
		contour_hint,
		facet_attributes,
		vertex_attributes,
		color_type,
		count,
		facet_data,
		vertex_lists );
	_hppex_stderr_print("==============================================\n" );

	_hppex_stderr_print("\n==============================================\n" );
	_hppex_stderr_print("  contour_hint = PEXContourNested\n" );
	contour_hint = PEXContourNested;
	PEXFillAreaSetWithData(
		dpy,
		res_id,
		req_type,
		shape_hint,
		ignore_edges, 
		contour_hint,
		facet_attributes,
		vertex_attributes,
		color_type,
		count,
		facet_data,
		vertex_lists );
	_hppex_stderr_print("==============================================\n" );

	_hppex_stderr_print("\n==============================================\n" );
	_hppex_stderr_print("  contour_hint = PEXContourIntersecting\n" );
	contour_hint = PEXContourIntersecting;
	PEXFillAreaSetWithData(
		dpy,
		res_id,
		req_type,
		shape_hint,
		ignore_edges, 
		contour_hint,
		facet_attributes,
		vertex_attributes,
		color_type,
		count,
		facet_data,
		vertex_lists );
	_hppex_stderr_print("==============================================\n" );

	_hppex_stderr_print("\n==============================================\n" );
	_hppex_stderr_print("  contour_hint = PEXContourUnknown\n" );
	contour_hint = PEXContourUnknown;
	PEXFillAreaSetWithData(
		dpy,
		res_id,
		req_type,
		shape_hint,
		ignore_edges, 
		contour_hint,
		facet_attributes,
		vertex_attributes,
		color_type,
		count,
		facet_data,
		vertex_lists );
	_hppex_stderr_print("==============================================\n" );

	_hppex_stderr_print("\n==============================================\n" );
	_hppex_stderr_print("  contour_hint = BogusContourHint\n" );
	contour_hint = BogusContourHint;
	PEXFillAreaSetWithData(
		dpy,
		res_id,
		req_type,
		shape_hint,
		ignore_edges, 
		contour_hint,
		facet_attributes,
		vertex_attributes,
		color_type,
		count,
		facet_data,
		vertex_lists );
	_hppex_stderr_print("==============================================\n" );

}

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "errocp.94.3");
    *num_images      = 1;
    *supported_modes = IMM_MODE | STR_MODE | WKS_MODE;

    strcpy(win_title, "errocp.94.3");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    if (glob_test_mode == IMM_MODE)
        _hppex_set_stderr("errocp.94.3.stderr_imm", "errocp.94.3");
    else
        _hppex_set_stderr("errocp.94.3.stderr_str", "errocp.94.3");
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    describe_test("\nOC Primitives Error Test (PEXFillAreaSetWithData)\n\n");
    describe_test("   Test for bad Contour Hint \n");
    describe_test("   The stderr output will contain messages for all\n");
    describe_test("   expected errors encountered.\n\n");
    fflush(stdout);

    draw_image(dpy, resourceID, req_type);
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
    if (glob_test_mode == IMM_MODE)
        file_testproc("errocp.94.3.stderr_imm", "errocp.94.3");
    else
        file_testproc("errocp.94.3.stderr_str", "errocp.94.3");
    }
