/* $XConsortium: pexocp.88.1.c,v 1.0 93/11/22 12:34:24 rws Exp $ */

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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/PEX5/PEXlib.h>

#include <misc.h>

typedef struct {
	int				subframe;
	PEXCurveApprox	approx;
	int				line_style;
	char			*message;
} _attr_value;

static _attr_value attr_values[]={
	{ 1, { PEXApproxImpDep, 0, 0.0 }, PEXLineTypeSolid, "Imp Dep approx, various Line types" },
	{ 2, { PEXApproxImpDep, 0, 0.0 }, PEXLineTypeDashed, "" },
	{ 3, { PEXApproxImpDep, 0, 0.0 }, PEXLineTypeDotted, "" },
	{ 4, { PEXApproxImpDep, 0, 0.0 }, PEXLineTypeDashDot, "Imp Dep approx, various Line types" },

	{ 1, { PEXApproxDCRelative, 0, 0.0 }, PEXLineTypeSolid, "DCRelative approx" },
	{ 2, { PEXApproxDCRelative, 0, 0.5 }, PEXLineTypeSolid, "" },
	{ 3, { PEXApproxDCRelative, 0, 0.9 }, PEXLineTypeSolid, "" },
	{ 4, { PEXApproxDCRelative, 0, 1.0 }, PEXLineTypeSolid, "DCRelative approx" },

	{ 1, { PEXApproxNPCRelative, 0, 0.0 }, PEXLineTypeSolid, "NPCRelative approx" },
	{ 2, { PEXApproxNPCRelative, 0, 0.5 }, PEXLineTypeSolid, "" },
	{ 3, { PEXApproxNPCRelative, 0, 0.9 }, PEXLineTypeSolid, "" },
	{ 4, { PEXApproxNPCRelative, 0, 1.0 }, PEXLineTypeSolid, "NPCRelative approx" },

	{ 1, { PEXApproxWCRelative, 0, 0.0 }, PEXLineTypeSolid, "DCRelative approx" },
	{ 2, { PEXApproxWCRelative, 0, 0.5 }, PEXLineTypeSolid, "" },
	{ 3, { PEXApproxWCRelative, 0, 0.9 }, PEXLineTypeSolid, "" },
	{ 4, { PEXApproxWCRelative, 0, 1.0 }, PEXLineTypeSolid, "DCRelative approx" },
};
#define NUM_ATTR_SETS (sizeof(attr_values)/sizeof(_attr_value))

typedef struct {
	float xscale, yscale, zscale;
	float xtrans, ytrans, ztrans;
} _subframe;

static _subframe subframe_data[]={
	{ 0.5, 0.5, 0.5, 0.0, 0.0, 0.0 },
	{ 0.5, 0.5, 0.5, 0.0, 0.5, 0.0 },
	{ 0.5, 0.5, 0.5, 0.5, 0.0, 0.0 },
	{ 0.5, 0.5, 0.5, 0.5, 0.5, 0.0 },
};
#define NUM_SUBFRAMES_IN_IMAGE 4


draw_image(Display           *dpy,
	     XID              resourceID,
	     PEXOCRequestType req_type,
	     int              attr_index)
    {
	/*
	 *	Data declarations
	 */
	static PEXCoord control_points[6];
	static float knots_2ord[] = { 1,1,2,3,4,5,6,6 };
	static float knots_3ord[] = { 1,1,1,2,3,4,5,5,5 };
	static float knots_6ord[] = { 1,1,1,1,1,1,2,2,2,2,2,2 };
	int						order, count, i;
	float					tmin, tmax;
	PEXArrayOfCoord			cpts;
	PEXColor				color;
	int						subframe;
	PEXMatrix				xform;

	/*
	 *	Set up the points.
	 */
	control_points[0].x = 0.10;	control_points[0].y = 0.80;	control_points[0].z = 0;
	control_points[1].x = 0.35;	control_points[1].y = 0.90;	control_points[1].z = 0;
	control_points[2].x = 0.60;	control_points[2].y = 0.90;	control_points[2].z = 0;
	control_points[3].x = 0.40;	control_points[3].y = 0.70;	control_points[3].z = 0;
	control_points[4].x = 0.70;	control_points[4].y = 0.70;	control_points[4].z = 0;
	control_points[5].x = 0.90;	control_points[5].y = 0.88;	control_points[5].z = 0;

	/*
	 *	Set up the markers to show in red.
	 */
	color.rgb.red	= 1.0;
	color.rgb.green	= 0.0;
	color.rgb.blue	= 0.0;
	PEXSetMarkerScale( dpy, resourceID, req_type, 3.0);
	PEXSetMarkerColor( dpy, resourceID, req_type, PEXColorTypeRGB, &color);

	/*
	 *	Set the translation, curve approximation, and line type
	 */
	subframe = attr_values[attr_index].subframe-1;
	PEXIdentityMatrix (xform);
	xform[0][0] = subframe_data[subframe].xscale;
	xform[1][1] = subframe_data[subframe].yscale;
	xform[2][2] = subframe_data[subframe].zscale;
	xform[0][3] = subframe_data[subframe].xtrans;
	xform[1][3] = subframe_data[subframe].ytrans;
	xform[2][3] = subframe_data[subframe].ztrans;
	PEXSetGlobalTransform (dpy, resourceID, req_type, xform );
	PEXSetCurveApprox( dpy, resourceID, req_type, 
						attr_values[attr_index].approx.method,  
						attr_values[attr_index].approx.tolerance ); 
	PEXSetLineType (dpy, resourceID, req_type, attr_values[attr_index].line_style);

	/*
	 * 2nd order curve
	 */
	order		= 2;
	count		= 6;
	cpts.point	= control_points;
	tmin		= knots_2ord[order-1];
	tmax		= knots_2ord[(count+order)-order];
	PEXNURBCurve( dpy, resourceID, req_type, PEXNonRational,
				  order, knots_2ord, count, cpts, tmin, tmax );
	PEXMarkers(dpy, resourceID, req_type, count, control_points );

	/*
	 *	Draw the 3rd order nurb curve.
	 */
	order		= 3;
	tmin		= knots_3ord[order-1];
	tmax		= knots_3ord[(count+order)-order];

	/*
	 *	Shift the control points down a bit
	 */
	for( i = 0; i < count; i++ )
		control_points[i].y -= 0.35;
	PEXNURBCurve( dpy, resourceID, req_type, PEXNonRational,
				  order, knots_3ord, count, cpts, tmin, tmax );
	PEXMarkers(dpy, resourceID, req_type, count, control_points );

	/*
	 *	Draw the 6th order nurb curve.
	 */
	order		= 6;
	tmin		= knots_6ord[order-1];
	tmax		= knots_6ord[(count+order)-order];

	/*
	 *	Shift the control points down a bit
	 */
	for( i = 0; i < count; i++ )
		control_points[i].y -= 0.3;
	PEXNURBCurve( dpy, resourceID, req_type, PEXNonRational,
				  order, knots_6ord, count, cpts, tmin, tmax );
	PEXMarkers(dpy, resourceID, req_type, count, control_points );
    }

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "pexocp.88.1");
    *num_images      = NUM_ATTR_SETS / NUM_SUBFRAMES_IN_IMAGE;
    *supported_modes = IMM_MODE | STR_MODE | WKS_MODE;

    strcpy(win_title, "pexocp.88.1");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    unsigned long         mask = PEXRAHLHSRMode;
    PEXRendererAttributes attrs;

    if (cur_image == 1)
	{
        attrs.hlhsr_mode = PEXHLHSRZBuffer;

        PEXChangeRenderer(dpy, renderer, mask, &attrs);
	}
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    int first_attr = (cur_image-1) * NUM_SUBFRAMES_IN_IMAGE;
    int attr_index;
    char type_msg[80];

    if (cur_image == 1)
	{
        describe_test ("\n\n");
        describe_test ("This test is the mainline test for NURB curve primitives and\n");
        describe_test ("curve approximation criteria.  \n");
        describe_test ("Each image consists of four subframes.  The subframe ordering is\n");
        describe_test ("lower left, upper left, lower right, upper right.\n");
        describe_test ("Some set of attributes is varied across the subframes of an image\n");
        describe_test("(the set is announced as the image is generated).\n\n");
        describe_test ("Within each subframe, three curve orders (2,3,6) are drawn with the\n");
        describe_test ("same control point data.\n\n");
	}
  
    for (attr_index = first_attr; attr_index<(first_attr+NUM_SUBFRAMES_IN_IMAGE); attr_index++)
	{
        if (attr_values[attr_index].subframe == 1)
	    {
	    sprintf (type_msg, "\t%s\n", attr_values[attr_index].message);
            describe_test(type_msg);
	    }

        draw_image( dpy, resourceID, req_type, attr_index );
	}
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
    char image_name[80], test_name[80];

    sprintf(image_name,"pexocp.88.1_%d", cur_image);
    sprintf(test_name, "pexocp.88.1_#%d", cur_image);
     image_testproc(image_name, test_name, img_desc,
		      dpy,window, 0, 0, 
		      glob_window_width, glob_window_height);
    }
