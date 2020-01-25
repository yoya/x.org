/* $XConsortium: pexocp.102.1.c,v 1.0 93/11/22 12:32:51 rws Exp $ */

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

/*
 *	draw_image prototype
 */
void draw_image( Display *dpy, XID rid, PEXOCRequestType req_type );

/* 
 * PEXlib your_favorite_test_name_here: Description
 */

void draw_image( Display *dpy, XID res_id, PEXOCRequestType req_type )
{	
	/*
	 *	Your test guts here ...
	 */
	float			float_value;
	int				indx;
	long			gdp_id;
	char			data[256];
	unsigned int	count;
	unsigned long	length;
	PEXCoord		points[10];

	/*
	 *	Make a "fake" set of data for a GDP
	 */
	strcpy( data, "This is just for the GDP testing" );
	gdp_id	= 216;
	count	= 10;
	length	= strlen( data );
	for( indx = 0; indx < count; indx++ )
	{
		float_value		= (float) indx / (float) count;

		points[indx].x	= float_value;
		points[indx].y	= 1 - float_value;
		points[indx].z	= float_value/(1 - float_value);
	}

	PEXGDP(
		dpy, res_id, req_type,
		gdp_id,
		count,
		points,
		length,
		data );
}

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "pexocp.102.1");
    *num_images      = 1;
    *supported_modes = IMM_MODE | STR_MODE | WKS_MODE;

    strcpy(win_title, "pexocp.102.1");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    describe_test("\nPEXGDP Mainline Test\n\n");
    describe_test("This should be a no-op, and the screen blank.\n");

    draw_image(dpy, resourceID, req_type);
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
     image_testproc("pexocp.102.1", "pexocp.102.1", img_desc,
		      dpy,window, 0, 0, 
		      glob_window_width, glob_window_height);
    }
