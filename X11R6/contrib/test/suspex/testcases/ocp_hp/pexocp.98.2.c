/* $XConsortium: pexocp.98.2.c,v 1.0 93/11/22 12:37:44 rws Exp $ */

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
#include <lut.h>

typedef struct {
	int					u_order;
	int					v_order;
	int					num_trim_loops;
	int					trim_rationality[3];
	int					trim_visibility[3];
	int					trim_order[3];
	PEXSurfaceApprox	approx;
	int					psc_type;
	int					interior_style;
	int					surface_edge_flag;
	char				*message;
	char				*description;
} _attr_value;

static _attr_value attr_values[]={
	{ 	
		2, 2, 3, 
		{ PEXNonRational, PEXNonRational, PEXNonRational },
		{ PEXOn, PEXOn, PEXOn },
		{ 3, 3, 2 },
		{ PEXApproxImpDep, 0, 0.0, 0.0 },
		PEXPSCNone, PEXInteriorStyleSolid, PEXOn,
		"uord = 2, vord = 2, num_trim = 3, orders 3, 3, 2, NonRational, ImpDep approx, Solid, with Edges",
		"Order 2 in u and v, but smooth trim order, should trim flat surfaces."
	},
	{ 	
		2, 3, 3, 
		{ PEXNonRational, PEXNonRational, PEXNonRational },
		{ PEXOn, PEXOn, PEXOn },
		{ 3, 3, 2 },
		{ PEXApproxImpDep, 0, 0.0, 0.0 },
		PEXPSCNone, PEXInteriorStyleSolid, PEXOn,
		"uord = 2, vord = 3, num_trim = 3, orders 3, 3, 2, NonRational, ImpDep approx, Solid, with Edges",
		"Order 2 in u, 3 in v, should trim correctly."
	},
	{ 	
		3, 3, 3,
		{ PEXNonRational, PEXNonRational, PEXNonRational },
		{ PEXOn, PEXOn, PEXOn },
		{ 3, 3, 2 },
		{ PEXApproxImpDep, 0, 0.0, 0.0 },
		PEXPSCNone, PEXInteriorStyleSolid, PEXOn,
		"uord = 3, vord = 3, num_trim = 3, orders 3, 3, 3, NonRational, ImpDep approx, Solid, with Edges",
		"Order 3 in u and v, anomalies in v but still smooth trimming."
	},
	{ 	
		3, 4, 3,
		{ PEXNonRational, PEXNonRational, PEXNonRational },
		{ PEXOn, PEXOn, PEXOn },
		{ 3, 3, 2 },
		{ PEXApproxImpDep, 0, 0.0, 0.0 },
		PEXPSCNone, PEXInteriorStyleSolid, PEXOn,
		"uord = 3, vord = 4, num_trim = 3, orders 3, 3, 2, NonRational, ImpDep approx, Solid, with Edges",
		"Order 3 in u, 4 in v, should look pretty good."
	},
	{ 	
		3, 4, 3,
		{ PEXNonRational, PEXNonRational, PEXNonRational },
		{ PEXOn, PEXOn, PEXOn },
		{ 3, 2, 3 },
		{ PEXApproxImpDep, 0, 0.0, 0.0 },
		PEXPSCNone, PEXInteriorStyleSolid, PEXOn,
		"uord = 3, vord = 4, num_trim = 3, orders 3, 2, 3, NonRational, ImpDep approx, Solid, with Edges",
		"Middle trim loop order 2 (should be rectangle), outer loop is order 3 (curvy)."
	},
	{ 	
		3, 4, 3,
		{ PEXNonRational, PEXNonRational, PEXNonRational },
		{ PEXOn, PEXOn, PEXOn },
		{ 2, 2, 2 },
		{ PEXApproxImpDep, 0, 0.0, 0.0 },
		PEXPSCNone, PEXInteriorStyleSolid, PEXOn,
		"uord = 3, vord = 4, num_trim = 3, orders 2, 2, 2, NonRational, ImpDep approx, Solid, with Edges",
		"All trim loops are order 2, all rectangular."
	},
	{ 	
		3, 4, 1,
		{ PEXNonRational, PEXNonRational, PEXNonRational },
		{ PEXOn, PEXOn, PEXOn },
		{ 3, 3, 2 },
		{ PEXApproxImpDep, 0, 0.0, 0.0 },
		PEXPSCNone, PEXInteriorStyleSolid, PEXOn,
		"uord = 3, vord = 4, num_trim = 1, order 3, NonRational, ImpDep approx, Solid, with Edges",
		"One trim loop, just the donut hole."
	},
	{ 	
		3, 4, 0,
		{ PEXNonRational, PEXNonRational, PEXNonRational },
		{ PEXOn, PEXOn, PEXOn },
		{ 3, 3, 2 },
		{ PEXApproxImpDep, 0, 0.0, 0.0 },
		PEXPSCNone, PEXInteriorStyleSolid, PEXOn,
		"uord = 3, vord = 4, num_trim = 0, NonRational, ImpDep approx, Solid, with Edges",
		"No trim loops, the whole surface should be visible."
	},
	{ 	
		3, 4, 3,
		{ PEXNonRational, PEXNonRational, PEXNonRational },
		{ PEXOn, PEXOn, PEXOn },
		{ 3, 3, 2 },
		{ PEXApproxImpDep, 0, 0.0, 0.0 },
		PEXPSCImpDep, PEXInteriorStyleHollow, PEXOn,
		"uord = 3, vord = 4, num_trim = 3, orders 3, 3, 2, NonRational, ImpDep approx, Hollow, with Edges, all visible",
		"Hollow with visibility on all trim loops, all should be in edge color."
	},
	{ 	
		3, 4, 3,
		{ PEXNonRational, PEXNonRational, PEXNonRational },
		{ PEXOff, PEXOn, PEXOff },
		{ 3, 3, 2 },
		{ PEXApproxImpDep, 0, 0.0, 0.0 },
		PEXPSCNone, PEXInteriorStyleHollow, PEXOn,
		"uord = 3, vord = 4, num_trim = 3, orders 3, 3, 2, NonRational, ImpDep approx, Hollow, with Edges, some visible",
		"Hollow with visibility off for inner and outer loops, they should be in interior color."
	},
	{ 	
		3, 4, 3,
		{ PEXNonRational, PEXNonRational, PEXNonRational },
		{ PEXOn, PEXOn, PEXOn },
		{ 3, 3, 2 },
		{ PEXApproxImpDep, 0, 0.0, 0.0 },
		PEXPSCNone, PEXInteriorStyleSolid, PEXOff,
		"uord = 3, vord = 4, num_trim = 3, orders 3, 3, 2, NonRational, ImpDep approx, Solid, PSCNone, no Edges",
		"No edging or PSC, should be trimmed green surface."
	},
	{ 	
		3, 4, 3,
		{ PEXNonRational, PEXNonRational, PEXNonRational },
		{ PEXOn, PEXOn, PEXOn },
		{ 3, 3, 2 },
		{ PEXApproxImpDep, 0, 0.0, 0.0 },
		PEXPSCImpDep, PEXInteriorStyleSolid, PEXOff,
		"uord = 3, vord = 4, num_trim = 3, orders 3, 3, 2, NonRational, ImpDep approx, Solid, PSCImpDep, no Edges",
		"No edging, PSCNone, same as last if interior edging not supported."
	},
	{ 	
		3, 4, 3,
		{ PEXNonRational, PEXNonRational, PEXNonRational },
		{ PEXOn, PEXOn, PEXOn },
		{ 3, 3, 2 },
		{ PEXApproxDCRelative, 0, 0.0, 0.0 },
		PEXPSCNone, PEXInteriorStyleSolid, PEXOn,
		"uord = 3, vord = 4, num_trim = 3, orders 3, 3, 2, NonRational, DCRelative approx 0.0,0.0, Solid, with Edges",
		"Low surface approximation, trim curve should adapt."
	},
	{ 	
		3, 4, 3,
		{ PEXNonRational, PEXNonRational, PEXNonRational },
		{ PEXOn, PEXOn, PEXOn },
		{ 3, 3, 2 },
		{ PEXApproxDCRelative, 0, 0.5, 0.5 },
		PEXPSCNone, PEXInteriorStyleSolid, PEXOn,
		"uord = 3, vord = 4, num_trim = 3, orders 3, 3, 2, NonRational, DCRelative approx 0.5,0.5, Solid, with Edges",
		"Medium surface approximation, trim curve should adapt."
	},
	{ 	
		3, 4, 3,
		{ PEXNonRational, PEXNonRational, PEXNonRational },
		{ PEXOn, PEXOn, PEXOn },
		{ 3, 3, 2 },
		{ PEXApproxDCRelative, 0, 0.9, 0.9 },
		PEXPSCNone, PEXInteriorStyleSolid, PEXOn,
		"uord = 3, vord = 4, num_trim = 3, orders 3, 3, 2, NonRational, DCRelative approx 0.9,0.9, Solid, with Edges",
		"Pretty good surface approximation, trim curve should adapt."
	},
	{ 	
		3, 4, 3,
		{ PEXRational, PEXNonRational, PEXNonRational },
		{ PEXOn, PEXOn, PEXOn },
		{ 3, 3, 2 },
		{ PEXApproxDCRelative, 0, 0.9, 0.9 },
		PEXPSCNone, PEXInteriorStyleSolid, PEXOn,
		"uord = 3, vord = 4, num_trim = 3, orders 3, 3, 2, one Rational, DCRelative approx 0.9,0.9, Solid, with Edges",
		"Looks same as last but inner loop uses rational control points."
	},
	{ 	
		3, 4, 3,
		{ PEXRational, PEXRational, PEXRational },
		{ PEXOn, PEXOn, PEXOn },
		{ 3, 3, 2 },
		{ PEXApproxDCRelative, 0, 0.9, 0.9 },
		PEXPSCNone, PEXInteriorStyleSolid, PEXOn,
		"uord = 3, vord = 4, num_trim = 3, orders 3, 3, 2, all Rational, DCRelative approx 0.9,0.9, Solid, with Edges",
		"Looks same as last but all loops use rational control points."
	},
};
#define NUM_ATTR_SETS (sizeof(attr_values)/sizeof(_attr_value))

/* The surface knots and control points. */
#define NUM_COLS 3
#define NUM_ROWS 4
static PEXCoord	surf_points[NUM_COLS*NUM_ROWS]={
	{ 0.0, 0.5, 0.0},
	{ 0.0, 0.6, 0.5},
	{ 0.0, 0.5, 1.0},

	{ 0.3, 0.5, 0.0},
	{ 0.3, 1.0, 0.5},
	{ 0.3, 0.5, 1.0},

	{ 0.6, 0.4, 0.0},
	{ 0.6, 0.5, 0.5},
	{ 0.6, 0.4, 1.0},

	{ 0.9, 0.5, 0.0},
	{ 0.9, 0.3, 0.5},
	{ 0.9, 0.2, 1.0},
};

static float uknots[][NUM_COLS+6]={
	/* order 2 */ { 0.0, 0.0, 0.5, 1.0, 1.0, -1.0, -1.0, -1.0, -1.0, },
	/* order 3 */ { 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, -1.0, -1.0, -1.0, },
	/* order 4 */ { 0.0, 0.0, 0.0, 0.5, 1.0, 1.0, 1.0, -1.0, -1.0, },
	/* order 5 */ { 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, -1.0, },
	/* order 6 */ { 0.0, 0.0, 0.0, 0.0, 0.5, 1.0, 1.0, 1.0, 1.0, },
};

static float vknots[][NUM_ROWS+6]={
	/* order 2 */ { 0.0, 0.0, 0.33, 0.66, 1.0, 1.0, -1.0, -1.0, -1.0, -1.0, },
	/* order 3 */ { 0.0, 0.0, 0.0, 0.5, 1.0, 1.0, 1.0, -1.0, -1.0, -1.0, },
	/* order 4 */ { 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, -1.0, -1.0, },
	/* order 5 */ { 0.0, 0.0, 0.0, 0.0, 0.5, 1.0, 1.0, 1.0, 1.0, -1.0, },
	/* order 6 */ { 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, },
};


/* Trimming Curves, from inner-most to outer-most. */
static	float		t0_knots_2[] = {0,0,1,1,2,2,2.5,3,3,4,4 };
static	float		t0_knots_3[] = {0,0,0,1,1,2,2,3,3,4,4,4 };
static	PEXCoord2D	t0_pts[] = {
    {0.5,0.32},
	{0.68,0.32},
	{0.68,0.5},
	{0.68,0.68},
	{0.5,0.68},
    {0.32,0.68},
	{0.32,0.5},
	{0.32,0.32},
	{0.5,0.32}
};
static	PEXCoord	t0_pts_rational[] = {
    {0.5,0.32,1.0},
	{0.68,0.32,1.0},
	{0.68,0.5,1.0},
	{0.68,0.68,1.0},
	{0.5,0.68,1.0},
    {0.32,0.68,1.0},
	{0.32,0.5,1.0},
	{0.32,0.32,1.0},
	{0.5,0.32,1.0}
};

static	float		t1_knots_2[] = {0,0,1,1,2,2,2.5,3,3,4,4 };
static	float		t1_knots_3[] = {0,0,0,1,1,2,2,3,3,4,4,4 };
static	PEXCoord2D	t1_pts[] = {
	{0.5,0.25},
	{0.25,0.25},
	{0.25,0.5},
    {0.25,0.75},
	{0.5,0.75},
	{0.75,0.75},
	{0.75,0.5},
	{0.75,0.25},
    {0.5,0.25},
};
static	PEXCoord	t1_pts_rational[] = {
	{0.5,0.25,1.0},
	{0.25,0.25,1.0},
	{0.25,0.5,1.0},
    {0.25,0.75,1.0},
	{0.5,0.75,1.0},
	{0.75,0.75,1.0},
	{0.75,0.5,1.0},
	{0.75,0.25,1.0},
    {0.5,0.25,1.0},
};

static	float		t2_knots_2[] = { 0, 0, 1, 2, 3, 4, 4 };
static	float		t2_knots_3[] = { 0, 0, 0, 1, 3, 4, 4, 4 };
static	PEXCoord2D	t2_pts[] = {
	{0.0, 0.0}, 
	{1.0, 0.0}, 
	{1.0, 1.0}, 
	{0.0, 1.0}, 
	{0.0, 0.0}
};
static	PEXCoord	t2_pts_rational[] = {
	{0.0, 0.0,1.0}, 
	{1.0, 0.0,1.0}, 
	{1.0, 1.0,1.0}, 
	{0.0, 1.0,1.0}, 
	{0.0, 0.0,1.0}
};

void draw_image( Display *dpy, XID res_id, PEXOCRequestType req_type, int attr_index )
{
    int					uorder, vorder;
    unsigned int		num_u_points = 3, num_v_points = 4;
    PEXArrayOfCoord		grid;
    PEXColor			color;
    PEXPSCData			psc;
    unsigned long		num_trim_loops;
    PEXListOfTrimCurve	trim_loops[3];
    PEXTrimCurve		trim_curves[3];
	float				*uknot_list, *vknot_list;

	/* Build the trimming loops.  Here, each loop is a single curve. */
	num_trim_loops = 0;

	if (attr_values[attr_index].num_trim_loops == 1) {
		num_trim_loops = 1;
		trim_loops[0].count = 1;
		trim_loops[0].curves = &trim_curves[0];

		trim_curves[0].rationality = attr_values[attr_index].trim_rationality[0];
		trim_curves[0].visibility = attr_values[attr_index].trim_visibility[0];
		trim_curves[0].order = attr_values[attr_index].trim_order[0];
		trim_curves[0].tmin = 0;
		trim_curves[0].tmax = 4;
		if (attr_values[attr_index].trim_order[0] == 2) {
			trim_curves[0].knots.count = sizeof(t0_knots_2)/sizeof(t0_knots_2[0]);
			trim_curves[0].knots.floats = t0_knots_2;
		}
		else if (attr_values[attr_index].trim_order[0] == 3) {
			trim_curves[0].knots.count = sizeof(t0_knots_3)/sizeof(t0_knots_3[0]);
			trim_curves[0].knots.floats = t0_knots_3;
		}
		trim_curves[0].count = sizeof(t0_pts)/sizeof(t0_pts[0]);
		if (attr_values[attr_index].trim_rationality[0] == PEXNonRational)
			trim_curves[0].control_points.point_2d = t0_pts;
		else
			trim_curves[0].control_points.point = t0_pts_rational;
		trim_curves[0].approx_method = PEXApproxImpDep;
		trim_curves[0].tolerance = 0.0;
	}
	else if (attr_values[attr_index].num_trim_loops == 3) {
		
		num_trim_loops = 3;
		trim_loops[0].count = 1;
		trim_loops[0].curves = &trim_curves[0];
		trim_loops[1].count = 1;
		trim_loops[1].curves = &trim_curves[1];
		trim_loops[2].count = 1;
		trim_loops[2].curves = &trim_curves[2];

		trim_curves[0].rationality = attr_values[attr_index].trim_rationality[0];
		trim_curves[0].visibility = attr_values[attr_index].trim_visibility[0];
		trim_curves[0].order = attr_values[attr_index].trim_order[0];
		trim_curves[0].tmin = 0;
		trim_curves[0].tmax = 4;
		if (attr_values[attr_index].trim_order[0] == 2) {
			trim_curves[0].knots.count = sizeof(t0_knots_2)/sizeof(t0_knots_2[0]);
			trim_curves[0].knots.floats = t0_knots_2;
		}
		else if (attr_values[attr_index].trim_order[0] == 3) {
			trim_curves[0].knots.count = sizeof(t0_knots_3)/sizeof(t0_knots_3[0]);
			trim_curves[0].knots.floats = t0_knots_3;
		}
		trim_curves[0].count = sizeof(t0_pts)/sizeof(t0_pts[0]);
		if (attr_values[attr_index].trim_rationality[0] == PEXNonRational)
			trim_curves[0].control_points.point_2d = t0_pts;
		else
			trim_curves[0].control_points.point = t0_pts_rational;
		trim_curves[0].approx_method = PEXApproxImpDep;
		trim_curves[0].tolerance = 0.0;
		
		trim_curves[1].rationality = attr_values[attr_index].trim_rationality[1];
		trim_curves[1].visibility = attr_values[attr_index].trim_visibility[1];
		trim_curves[1].order = attr_values[attr_index].trim_order[1];
		trim_curves[1].tmin = 0;
		trim_curves[1].tmax = 4;
		if (attr_values[attr_index].trim_order[1] == 2) {
			trim_curves[1].knots.count = sizeof(t1_knots_2)/sizeof(t1_knots_2[0]);
			trim_curves[1].knots.floats = t1_knots_2;
		}
		else if (attr_values[attr_index].trim_order[1] == 3) {
			trim_curves[1].knots.count = sizeof(t1_knots_3)/sizeof(t1_knots_3[0]);
			trim_curves[1].knots.floats = t1_knots_3;
		}
		trim_curves[1].count = sizeof(t1_pts)/sizeof(t1_pts[0]);
		if (attr_values[attr_index].trim_rationality[1] == PEXNonRational)
			trim_curves[1].control_points.point_2d = t1_pts;
		else
			trim_curves[1].control_points.point = t1_pts_rational;
		trim_curves[1].approx_method = PEXApproxImpDep;
		trim_curves[1].tolerance = 0.0;
		
		trim_curves[2].rationality = attr_values[attr_index].trim_rationality[2];
		trim_curves[2].visibility = attr_values[attr_index].trim_visibility[2];
		trim_curves[2].order = attr_values[attr_index].trim_order[2];
		trim_curves[2].tmin = 0;
		trim_curves[2].tmax = 4;
		if (attr_values[attr_index].trim_order[2] == 2) {
			trim_curves[2].knots.count = sizeof(t2_knots_2)/sizeof(t2_knots_2[0]);
			trim_curves[2].knots.floats = t2_knots_2;
		}
		else if (attr_values[attr_index].trim_order[2] == 3) {
			trim_curves[2].knots.count = sizeof(t2_knots_3)/sizeof(t2_knots_3[0]);
			trim_curves[2].knots.floats = t2_knots_3;
		}
		trim_curves[2].count = sizeof(t2_pts)/sizeof(t2_pts[0]);
		if (attr_values[attr_index].trim_rationality[2] == PEXNonRational)
			trim_curves[2].control_points.point_2d = t2_pts;
		else
			trim_curves[2].control_points.point = t2_pts_rational;
		trim_curves[2].approx_method = PEXApproxImpDep;
		trim_curves[2].tolerance = 0.0;
	}

	uorder = attr_values[attr_index].u_order;
	vorder = attr_values[attr_index].v_order;
	uknot_list = uknots[uorder-2];
	vknot_list = vknots[vorder-2];

	PEXSetViewIndex( dpy, res_id, req_type, (unsigned int) 1 );

	/*
		Set interior attributes.
	*/
	PEXSetInteriorStyle( dpy, res_id, req_type, attr_values[attr_index].interior_style );
	color.rgb.red = 0.0;
	color.rgb.green = 1.0;
	color.rgb.blue = 0.0;
	PEXSetSurfaceColor (dpy, res_id, req_type, PEXColorTypeRGB, &color);
	PEXSetSurfaceEdgeFlag( dpy, res_id, req_type, attr_values[attr_index].surface_edge_flag );
	color.rgb.red = 1.0;
	color.rgb.green = 0.0;
	color.rgb.blue = 0.0;
	PEXSetSurfaceEdgeColor (dpy, res_id, req_type, PEXColorTypeRGB, &color);

	/*
	 *	Set the parametric surface characteristics
	 */
	psc.imp_dep.length = 0;
	psc.imp_dep.data = NULL;
	PEXSetParaSurfCharacteristics( dpy, res_id, req_type, 
									attr_values[attr_index].psc_type, &psc );

	/*
	 *	Set the surface approximation criteria
	 */
	PEXSetSurfaceApprox( dpy, res_id, req_type, 
						attr_values[attr_index].approx.method, 
						attr_values[attr_index].approx.u_tolerance,
						attr_values[attr_index].approx.v_tolerance );

	/* Create the surface. */
	grid.point = surf_points;
	PEXNURBSurface( dpy, res_id, req_type, PEXNonRational,
		        uorder, vorder, uknot_list, vknot_list,
		        num_u_points, num_v_points, grid,
		        num_trim_loops, trim_loops );
}

static void set_view( dpy, view_table )
    Display		*dpy;
    PEXLookupTable	view_table;
{
    PEXViewEntry	view;
    PEXCoord2D		window[2];
    double		view_plane, front_plane, back_plane;
    PEXCoord		prp;
    PEXNPCSubVolume	viewport;

    /* The view orientation parameters. */
    static PEXCoord	view_ref_pt = {0.5, 0.5, 0.5};
    static PEXVector	view_plane_normal = {0.5643, 0.4775, 0.6735};
    static PEXVector	view_up_vec = {0,1,0};

    /* Compute the view orientation transform. */
    PEXViewOrientationMatrix( &view_ref_pt, &view_plane_normal,
			      &view_up_vec, view.orientation );
    
    /* The view mapping parameters. */
    prp.x = 0; prp.y = 0; prp.z = 10;
    window[0].x = -0.78; window[1].x = 0.7;
    window[0].y = -0.78; window[1].y = 0.7;
    front_plane = 1; view_plane = 0; back_plane = -1;
    viewport.min.x = 0; viewport.max.x = 1;
    viewport.min.y = 0; viewport.max.y = 1;
    viewport.min.z = 0; viewport.max.z = 1;
    
    /* Compute the view mapping transform. */
    PEXViewMappingMatrix( window, &viewport, False, &prp,
			  view_plane, back_plane, front_plane,
			  view.mapping );
    
    /* The view clipping parameters. */
    view.clip_flags = PEXClippingAll;
    view.clip_limits = viewport;
    
    /* Set view 1. */
    PEXSetTableEntries( dpy, view_table, 1, 1, PEXLUTView,
		        (PEXPointer) &view );
}

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "pexocp.98.2");
    *num_images      = NUM_ATTR_SETS;
    *supported_modes = IMM_MODE | STR_MODE | WKS_MODE;

    strcpy(win_title, "pexocp.98.2");
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
        set_view(dpy, viewLUT);
	}
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    char desc_msg[80];

    if (cur_image == 1)
	{
        describe_test ("\n\n");
        describe_test ("This is a mainline test for NURB surfaces with\n");
	describe_test ("trimming curves.\n");
        describe_test ("Surface approximation criteria, parametric surface\n");
        describe_test ("criteria, and interior attributes are varied to\n");
        describe_test ("make sure that they correctly affect the spline\n");
        describe_test ("surface rendering. Several orders of surface spline\n");
	describe_test ("are tested.  Several orders of trim curve are also\n");
	describe_test ("tried, as are individual control over loop \n");
	describe_test ("visibility (edging) and rationality.  The test \n");
	describe_test ("describes each image as it is generated.\n\n");
	}
  
    sprintf (desc_msg, "  %s\n", attr_values[cur_image-1].description);
    describe_test(desc_msg);

    draw_image( dpy, resourceID, req_type, cur_image-1 );
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
    char image_name[80], test_name[80];

    sprintf(image_name,"pexocp.98.2_%d", cur_image);
    sprintf(test_name, "pexocp.98.2_#%d", cur_image);
     image_testproc(image_name, test_name, img_desc,
		      dpy,window, 0, 0, 
		      glob_window_width, glob_window_height);
    }
