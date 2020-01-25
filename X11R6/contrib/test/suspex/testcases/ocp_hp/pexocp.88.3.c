/* $XConsortium: pexocp.88.3.c,v 1.0 93/11/22 12:34:35 rws Exp $ */

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

#define		MAX_CURVE_ORDER		6

/*
 *	PEXlib NURBCurve:  Test Case 3, specifying a curve order
 *	that is not supported.
 */

draw_image(Display          *dpy,
	   XID              resourceID,
	   PEXOCRequestType req_type)
    {
	/*
	 *	Data declarations
	 */
	static PEXCoord4D control_points[9];
	static float knot_vals[] = {0,0,0,1,1,1,2,2,2,3,3,3,4,4,4,4};
	static PEXCoord pgon_points[] =
		{
			{.5,0.1,0}, {.9,0.1,0},
			{.9,0.5,0}, {.9,0.9,0},
			{.5,0.9,0}, {.1,0.9,0},
			{.1,0.5,0}, {.1,0.1,0},
			{.5,0.1,0}
		};
	int						order, count, i;
	float					tmin, tmax;
	PEXArrayOfCoord			cpts;
	PEXColor				color;

	/*
	 *	Initialize the curve points
	 */
	control_points[0].x = 0.5;
	control_points[0].y = 0.1;
	control_points[0].z = 0;
	control_points[0].w = 1;

	control_points[1].x = 0.9;
	control_points[1].y = 0.1;
	control_points[1].z = 0;
	control_points[1].w = 0.7071;

	control_points[2].x = 0.9;
	control_points[2].y = 0.5;
	control_points[2].z = 0;
	control_points[2].w = 1;

	control_points[3].x = 0.9;
	control_points[3].y = 0.9;
	control_points[3].z = 0;
	control_points[3].w = 0.7071;

	control_points[4].x = 0.5;
	control_points[4].y = 0.9;
	control_points[4].z = 0;
	control_points[4].w = 1;

	control_points[5].x = 0.1;
	control_points[5].y = 0.9;
	control_points[5].z = 0;
	control_points[5].w = 0.7071;

	control_points[6].x = 0.1;
	control_points[6].y = 0.5;
	control_points[6].z = 0;
	control_points[6].w = 1;

	control_points[7].x = 0.1;
	control_points[7].y = 0.1;
	control_points[7].z = 0;
	control_points[7].w = 0.7071;

	control_points[8].x = 0.5;
	control_points[8].y = 0.1;
	control_points[8].z = 0;
	control_points[8].w = 1;

	/*
	 * Define the curve.
	 */
	count = 9;
	cpts.point_4d = control_points;

	/*
	 * Apply weights to the x,y, and z control points coords.
	 */
	for( i = 0; i < count; i++ )
	{
		control_points[i].x *= control_points[i].w;
		control_points[i].y *= control_points[i].w;
		control_points[i].z *= control_points[i].w;
	}

	/*
	 * Use markers to show the location of the control points.
	 */
	color.rgb.red	= 1.0;
	color.rgb.green	= 0.0;
	color.rgb.blue	= 0.0;
	PEXSetMarkerScale(dpy, resourceID, req_type, 3.0);
	PEXSetMarkerColor(dpy, resourceID, req_type, PEXColorTypeRGB, &color);
	PEXMarkers(dpy, resourceID, req_type, count, pgon_points );

	/*
	 *  Use Polylines to show the control polygon and then set
	 *	the linetype and color for the NURBCurve.
	 */
	color.rgb.red	= 0.0;
	color.rgb.green	= 1.0;
	color.rgb.blue	= 0.0;
	PEXSetLineType(dpy, resourceID, req_type, PEXLineTypeDotted );
	PEXSetLineColor(dpy, resourceID, req_type, PEXColorTypeRGB, &color);
	PEXPolyline(dpy, resourceID, req_type, count, pgon_points );
	color.rgb.red	= 1.0;
	color.rgb.green	= 1.0;
	color.rgb.blue	= 1.0;
	PEXSetLineType(dpy, resourceID, req_type, PEXLineTypeSolid );
	PEXSetLineColor(dpy, resourceID, req_type, PEXColorTypeRGB, &color);

	/*
	 *	Draw the nurb curve.
	 */
	PEXSetCurveApprox( dpy, resourceID, req_type,
					   PEXApproxImpDep, 20.0 );

	order = MAX_CURVE_ORDER + 1;

	{ 
	char desc_string[80];

	sprintf( desc_string,"PEXNURBCurve order = %d\n", order );
	describe_test(desc_string);
	}

	tmin = knot_vals[order-1];
	tmax = knot_vals[count];

	PEXNURBCurve( dpy, resourceID, req_type, PEXRational,
		order, knot_vals, count, cpts, tmin, tmax );
    }


void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "pexocp.88.3");
    *num_images      = 1;
    *supported_modes = IMM_MODE | STR_MODE | WKS_MODE;

    strcpy(win_title, "pexocp.88.3");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    unsigned long         mask = PEXRAHLHSRMode;
    PEXRendererAttributes attrs;

    attrs.hlhsr_mode = PEXHLHSRZBuffer;

    PEXChangeRenderer(dpy, renderer, mask, &attrs);
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    describe_test("\nPEXlib NURBCurve with  invalid curve order.\n");
    describe_test("  Image should consist of a green dotted rectangle \n");
    describe_test("  with 8 red asterisks at the control points.\n");
    describe_test("  The NURB curve should NOT be drawn.\n\n");

    draw_image( dpy, resourceID, req_type );
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
     image_testproc("pexocp.88.3", "pexocp.88.3", img_desc,
		      dpy,window, 0, 0, 
		      glob_window_width, glob_window_height);
    }
