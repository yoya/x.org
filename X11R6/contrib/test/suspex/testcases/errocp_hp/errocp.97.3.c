/* $XConsortium: errocp.97.3.c,v 1.0 93/11/22 12:45:06 rws Exp $ */

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

#define		BogusContoursAllOne		2

/*
 *	draw_image prototype
 */
void draw_image( Display *dpy, XID rid, PEXOCRequestType req_type );


/* 
 *	TEST: PEXSetOfFillAreaSets - Test for bad Contour Hint
 */

void draw_image( Display *dpy, XID res_id, PEXOCRequestType req_type )
{	
	/*
	 *	Data declarations
	 */
	int					 shape_hint;
	unsigned int		 facet_attributes;
	unsigned int		 vertex_attributes;
	unsigned int		 edge_attributes;
	int					 contour_hint;
	int					 contours_all_one;
	int					 color_type;
	unsigned int		 set_count;
	PEXArrayOfFacetData	 facet_data;
	unsigned int		 vertex_count;
	PEXArrayOfVertex	 vertices;
	unsigned int		 index_count;
	PEXSwitch			*edge_flags;
	PEXConnectivityData	*connectivity;

	/*
	 *	Test for Output Command Error for bad contours_all_1
	 *	parameter.
	 *
	 *		Current defined contour hints in the PEX header files
	 *		PEXSetOfFillAreaSets are:
	 *
	 *			True
	 *			False
	 *
	 */
	shape_hint			= PEXShapeUnknown;
	facet_attributes	= PEXGANone;
	vertex_attributes	= PEXGANone;
	edge_attributes		= PEXGANone;
	contour_hint		= PEXContourUnknown;
	contours_all_one	= True;
	color_type			= PEXColorTypeRGB;
	set_count			= 0;
	facet_data.rgb		= (PEXColorRGB *) NULL;
	vertex_count		= 0;
	vertices.no_data		= (PEXCoord *) NULL;
	index_count			= 0;
	edge_flags			= (PEXSwitch *) NULL;
	connectivity		= (PEXConnectivityData *) NULL;

	_hppex_stderr_print("\n==============================================\n" );
	_hppex_stderr_print("  contours_all_one = True\n" );
	contours_all_one = True;
	PEXSetOfFillAreaSets(
		dpy,
		res_id,
		req_type,
		shape_hint,
		facet_attributes,
		vertex_attributes,
		edge_attributes,
		contour_hint,
		contours_all_one,
		color_type,
		set_count,
		facet_data,
		vertex_count,
		vertices,
		index_count,
		edge_flags,
		connectivity );
	_hppex_stderr_print("==============================================\n" );

	_hppex_stderr_print("\n==============================================\n" );
	_hppex_stderr_print("  contours_all_one = False\n" );
	contours_all_one = False;
	PEXSetOfFillAreaSets(
		dpy,
		res_id,
		req_type,
		shape_hint,
		facet_attributes,
		vertex_attributes,
		edge_attributes,
		contour_hint,
		contours_all_one,
		color_type,
		set_count,
		facet_data,
		vertex_count,
		vertices,
		index_count,
		edge_flags,
		connectivity );
	_hppex_stderr_print("==============================================\n" );

	_hppex_stderr_print("\n==============================================\n" );
	_hppex_stderr_print("  contours_all_one = BogusContoursAllOne\n" );
	contours_all_one = BogusContoursAllOne;
	PEXSetOfFillAreaSets(
		dpy,
		res_id,
		req_type,
		shape_hint,
		facet_attributes,
		vertex_attributes,
		edge_attributes,
		contour_hint,
		contours_all_one,
		color_type,
		set_count,
		facet_data,
		vertex_count,
		vertices,
		index_count,
		edge_flags,
		connectivity );
	_hppex_stderr_print("==============================================\n" );

}

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "errocp.97.3");
    *num_images      = 1;
    *supported_modes = IMM_MODE | STR_MODE | WKS_MODE;

    strcpy(win_title, "errocp.97.3");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    if (glob_test_mode == IMM_MODE)
        _hppex_set_stderr("errocp.97.3.stderr_imm", "errocp.97.3");
    else
        _hppex_set_stderr("errocp.97.3.stderr_str", "errocp.97.3");
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    describe_test("\nOC Primitives Error Test (PEXSetOfFillAreaSets)\n\n");
    describe_test("   Test for bad Contour Hint \n");
    describe_test("   The stderr output will contain messages for all\n");
    describe_test("   expected errors encountered.\n\n");
    fflush(stdout);

    draw_image(dpy, resourceID, req_type);
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
    if (glob_test_mode == IMM_MODE)
        file_testproc("errocp.97.3.stderr_imm", "errocp.97.3");
    else
        file_testproc("errocp.97.3.stderr_str", "errocp.97.3");
    }
