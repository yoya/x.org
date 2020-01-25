/* $XConsortium: errocp.97.4.c,v 1.0 93/11/22 12:45:12 rws Exp $ */

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

#define		BogusColorType		7

/*
 *	draw_image prototype
 */
void draw_image( Display *dpy, XID rid, PEXOCRequestType req_type );


/* 
 *	TEST:  PEXSetOfFillAreaSets - Test for undefined Color Type
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
	 *	Test for Output Command Error for undefined color type.
	 *
	 *		Currently PEX defined color types are:
	 *			#define PEXColorTypeIndexed  0
	 *			#define PEXColorTypeRGB      1
	 *			#define PEXColorTypeCIE      2
	 *			#define PEXColorTypeHSV      3
	 *			#define PEXColorTypeHLS      4
	 *			#define PEXColorTypeRGB8     5
	 *			#define PEXColorTypeRGB16    6
	 *
	 *		of which only PEXColorTypeIndexed and PEXColorTypeRGB
	 *		are supported in the first release of HP-PEX.  Only
	 *		these two should result in no error message.  The rest
	 *		should generate an error.
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

	for(
		color_type = PEXColorTypeIndexed;
		color_type <= BogusColorType;
		color_type++ )
	{
		_hppex_stderr_print("\n================================\n" );
		switch( color_type )
		{
			case( PEXColorTypeIndexed ) :
				_hppex_stderr_print("color_type = PEXColorTypeIndexed\n" );
				break;
			case( PEXColorTypeRGB ) :
				_hppex_stderr_print("color_type = PEXColorTypeRGB\n" );
				break;
			case( PEXColorTypeCIE ) :
				_hppex_stderr_print("color_type = PEXColorTypeCIE\n" );
				break;
			case( PEXColorTypeHSV ) :
				_hppex_stderr_print("color_type = PEXColorTypeHSV\n" );
				break;
			case( PEXColorTypeHLS ) :
				_hppex_stderr_print("color_type = PEXColorTypeHLS\n" );
				break;
			case( PEXColorTypeRGB8 ) :
				_hppex_stderr_print("color_type = PEXColorTypeRGB8\n" );
				break;
			case( PEXColorTypeRGB16 ) :
				_hppex_stderr_print("color_type = PEXColorType16\n" );
				break;
			case( BogusColorType ) :
				_hppex_stderr_print("color_type = BogusColorType\n" );
				break;
		}

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
		_hppex_stderr_print("================================\n" );
	}
}

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "errocp.97.4");
    *num_images      = 1;
    *supported_modes = IMM_MODE | STR_MODE | WKS_MODE;

    strcpy(win_title, "errocp.97.4");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    if (glob_test_mode == IMM_MODE)
        _hppex_set_stderr("errocp.97.4.stderr_imm", "errocp.97.4");
    else
        _hppex_set_stderr("errocp.97.4.stderr_str", "errocp.97.4");
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    describe_test("\nOC Primitives Error Test (PEXSetOfFillAreaSets)\n\n");
    describe_test("   Test for undefined Color Type \n");
    describe_test("   The stderr output will contain messages for all\n");
    describe_test("   expected errors encountered.\n\n");
    fflush(stdout);

    draw_image(dpy, resourceID, req_type);
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
    if (glob_test_mode == IMM_MODE)
        file_testproc("errocp.97.4.stderr_imm", "errocp.97.4");
    else
        file_testproc("errocp.97.4.stderr_str", "errocp.97.4");
    }
