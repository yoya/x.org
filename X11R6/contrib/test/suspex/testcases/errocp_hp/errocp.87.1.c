/* $XConsortium: errocp.87.1.c,v 1.0 93/11/22 12:41:21 rws Exp $ */

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
 * TEST: PEXPolylineSetWithData - Unsupported/Undefined Color Type
 */

void draw_image( Display *dpy, XID res_id, PEXOCRequestType req_type )
{	
	/*
	 *	Data declarations
	 */
	unsigned int	vertex_attributes	= PEXGANone;
	int				color_type;
	unsigned int	count				= 0;
	PEXListOfVertex	*vertex_lists		= NULL;

	/*
	 *	Test for Output Command Error for unsupported or undefined
	 *	color type.
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
	vertex_attributes = PEXGANone;

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

		PEXPolylineSetWithData(
			dpy,
			res_id,
			req_type,
			vertex_attributes,
			color_type,
			count,
			vertex_lists );

		_hppex_stderr_print("================================\n" );
	}
}

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "errocp.87.1");
    *num_images      = 1;
    *supported_modes = IMM_MODE | STR_MODE | WKS_MODE;

    strcpy(win_title, "errocp.87.1");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    if (glob_test_mode == IMM_MODE)
        _hppex_set_stderr("errocp.87.1.stderr_imm", "errocp.87.1");
    else
        _hppex_set_stderr("errocp.87.1.stderr_str", "errocp.87.1");
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    describe_test("\nOC Primitives Error Test (PolylineSetWithData)\n\n");
    describe_test("   Unsupported/Undefined Color Type\n");
    describe_test("   The stderr output will contain messages for all\n");
    describe_test("   expected errors encountered.\n\n");
    fflush(stdout);

    draw_image(dpy, resourceID, req_type);
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
    if (glob_test_mode == IMM_MODE)
        file_testproc("errocp.87.1.stderr_imm", "errocp.87.1");
    else
        file_testproc("errocp.87.1.stderr_str", "errocp.87.1");
    }
