/* $XConsortium: errocp.96.2.c,v 1.0 93/11/22 12:44:31 rws Exp $ */

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
 *	TEST: PEXQuadrilateralMesh - Test for undefined Color Type
 */

void draw_image( Display *dpy, XID res_id, PEXOCRequestType req_type )
{	
	/*
	 *	Data declarations
	 */
	int					shape_hint;
	unsigned int		facet_attributes;
	unsigned int		vertex_attributes;
	int					color_type;
	PEXArrayOfFacetData	facet_data;
	unsigned int		col_count;
	unsigned int		row_count;
	PEXArrayOfVertex	vertices;

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
	color_type			= PEXColorTypeRGB;
	facet_data.index	= (PEXColorIndexed *) NULL;
	col_count			= 0;
	row_count			= 0;
	vertices.no_data	= (PEXCoord *) NULL;

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

		PEXQuadrilateralMesh(
			dpy,
			res_id,
			req_type,
			shape_hint,
			facet_attributes,
			vertex_attributes,
			color_type,
			facet_data,
			col_count,
			row_count,
			vertices );
		_hppex_stderr_print("================================\n" );
	}
}

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "errocp.96.2");
    *num_images      = 1;
    *supported_modes = IMM_MODE | STR_MODE | WKS_MODE;

    strcpy(win_title, "errocp.96.2");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    if (glob_test_mode == IMM_MODE)
        _hppex_set_stderr("errocp.96.2.stderr_imm", "errocp.96.2");
    else
        _hppex_set_stderr("errocp.96.2.stderr_str", "errocp.96.2");
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    describe_test("\nOC Primitives Error Test (PEXQuadrilateralMesh)\n\n");
    describe_test("   Test for undefined Color Type \n");
    describe_test("   The stderr output will contain messages for all\n");
    describe_test("   expected errors encountered.\n\n");
    fflush(stdout);

    draw_image(dpy, resourceID, req_type);
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
    if (glob_test_mode == IMM_MODE)
        file_testproc("errocp.96.2.stderr_imm", "errocp.96.2");
    else
        file_testproc("errocp.96.2.stderr_str", "errocp.96.2");
    }
