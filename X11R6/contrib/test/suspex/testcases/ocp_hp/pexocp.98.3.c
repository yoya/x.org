/* $XConsortium: pexocp.98.3.c,v 1.0 93/11/22 12:37:50 rws Exp $ */

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
	int					rationality;
	PEXSurfaceApprox	approx;
	int					psc_type;
	int					interior_style;
	int					surface_edge_flag;
	char				*message;
	char				*description;
} _attr_value;

static _attr_value attr_values[]={
	{ 	
		3, 7, PEXNonRational,
		{ PEXApproxWCRelative, 0, 0.95, 0.95 },
		PEXPSCNone, PEXInteriorStyleSolid, PEXOn,
		"uord = 3, vord = 7, NonRational ctl, WCRelative approx 0.95,0.95, PSCNone, Solid, with Edges",
		"Empty Window and no error messges."
	},
};
#define NUM_ATTR_SETS (sizeof(attr_values)/sizeof(_attr_value))

#define NUM_COLS 3
#define NUM_ROWS 7

static PEXCoord	surf_points[NUM_COLS*NUM_ROWS]={
	{ 0.0,  0.5,  0.0}, 
	{ 0.0,  0.6,  0.5}, 
	{ 0.0,  0.5,  1.0},

	{ 0.15, 0.5,  0.0},
	{ 0.15, 0.5,  0.5},
	{ 0.15, 0.5,  1.0},

	{ 0.3,  0.5,  0.0}, 
	{ 0.3,  1.0,  0.5}, 
	{ 0.3,  0.5,  1.0},

	{ 0.45, 0.5,  0.0},
	{ 0.45, 1.0,  0.5},
	{ 0.45, 0.5,  1.0},

	{ 0.6,  0.4,  0.0}, 
	{ 0.6,  0.5,  0.5}, 
	{ 0.6,  0.4,  1.0},

	{ 0.75, 0.5,  0.0},
	{ 0.75, 0.5,  0.5},
	{ 0.75, 0.5,  1.0},

	{ 0.9,  0.5,  0.0}, 
	{ 0.9,  0.3,  0.5}, 
	{ 0.9,  0.2,  1.0},

};
static PEXCoord4D	surf_points_rational[NUM_COLS*NUM_ROWS]={
	{ 0.0,  0.5,  0.0, 1.0 }, 
	{ 0.0,  0.6,  0.5, 1.0 }, 
	{ 0.0,  0.5,  1.0, 1.0 },

	{ 0.15, 0.5,  0.0, 1.0 },
	{ 0.15, 0.5,  0.5, 1.0 },
	{ 0.15, 0.5,  1.0, 1.0 },

	{ 0.3,  0.5,  0.0, 1.0 }, 
	{ 0.3,  1.0,  0.5, 1.0 }, 
	{ 0.3,  0.5,  1.0, 1.0 },

	{ 0.45, 0.5,  0.0, 1.0 },
	{ 0.45, 1.0,  0.5, 1.0 },
	{ 0.45, 0.5,  1.0, 1.0 },

	{ 0.6,  0.4,  0.0, 1.0 }, 
	{ 0.6,  0.5,  0.5, 1.0 }, 
	{ 0.6,  0.4,  1.0, 1.0 },

	{ 0.75, 0.5,  0.0, 1.0 },
	{ 0.75, 0.5,  0.5, 1.0 },
	{ 0.75, 0.5,  1.0, 1.0 },

	{ 0.9,  0.5,  0.0, 1.0 }, 
	{ 0.9,  0.3,  0.5, 1.0 }, 
	{ 0.9,  0.2,  1.0, 1.0 },
};

static float uknots[]={
	 0.0,  0.0,  0.0,  1.0,
	 1.0,  1.0, -1.0, -1.0,
	-1.0 };

static float vknots[]={
	0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 1.0, 1.0,
	1.0, 1.0, 1.0, 1.0,
	1.0, 1.0, 1.0, 1.0,
	1.0, 1.0, 1.0, 1.0 };

void draw_image( Display *dpy, XID res_id, PEXOCRequestType req_type, int attr_index )
{	
	/*
	 *	Data declarations
	 */
	int							 uorder,
								 vorder,

								 col_count,
								 row_count;
	float						*uknot_list,
								*vknot_list;
	PEXArrayOfCoord				 grid;
	PEXColor					 color;
	PEXPSCData					 psc;



	/*
	 *	Assign surface parameters
	 */
	if (attr_values[attr_index].rationality == PEXNonRational)
		grid.point	= surf_points;
	else
		grid.point_4d	= surf_points_rational;

	col_count	= NUM_COLS;
	row_count	= NUM_ROWS;

	uorder		= attr_values[0].u_order;
	vorder		= attr_values[0].v_order;

	uknot_list = uknots;
	vknot_list = vknots;

	PEXSetViewIndex( dpy, res_id, req_type, (unsigned int) 1 );

	/*
		Set interior attributes.
	*/
	PEXSetInteriorStyle( dpy, res_id, req_type, attr_values[0].interior_style );

	color.rgb.red	= 0.0;
	color.rgb.green	= 1.0;
	color.rgb.blue	= 0.0;
	PEXSetSurfaceColor (dpy, res_id, req_type, PEXColorTypeRGB, &color);

	PEXSetSurfaceEdgeFlag( dpy, res_id, req_type, attr_values[0].surface_edge_flag );

	color.rgb.red	= 1.0;
	color.rgb.green	= 0.0;
	color.rgb.blue	= 0.0;
	PEXSetSurfaceEdgeColor (dpy, res_id, req_type, PEXColorTypeRGB, &color);

	/*
	 *	Set the parametric surface characteristics
	 */
	psc.imp_dep.length	= 0;
	psc.imp_dep.data	= NULL;
	PEXSetParaSurfCharacteristics( dpy, res_id, req_type, 
									attr_values[0].psc_type, &psc );

	/*
	 *	Set the surface approximation criteria
	 */
	PEXSetSurfaceApprox( dpy, res_id, req_type, 
						attr_values[0].approx.method, 
						attr_values[0].approx.u_tolerance,
						attr_values[0].approx.v_tolerance );

	/*
	 *	Create the surface with no trimming curves
	 */
	PEXNURBSurface( dpy, res_id, req_type, 
					attr_values[0].rationality, 
					uorder, vorder, uknot_list, vknot_list,
					col_count, row_count, grid, 
					0, (PEXListOfTrimCurve *) NULL );
}

static void
set_view( dpy, view_table )
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
    strcpy(test_name, "pexocp.98.3");
    *num_images      = NUM_ATTR_SETS;
    *supported_modes = IMM_MODE | STR_MODE | WKS_MODE;

    strcpy(win_title, "pexocp.98.3");
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
        describe_test ("This is a boundary condition test for NURB surfaces without trimming curves.\n");
        describe_test ("The specified order in the v direction is 7 which exceeds the maximum supported\n" );
        describe_test ("curve order of 6.  The result should be the entire output command being\n" );
        describe_test ("silently ignored by a renderer.  That is, there should be no pixels in the\n" );
        describe_test ("in the window and there should be no error output.\n\n" );
	}
  
    sprintf (desc_msg, "  %s\n", attr_values[cur_image-1].description);
    describe_test(desc_msg);

    draw_image( dpy, resourceID, req_type, cur_image-1 );
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
    char image_name[80], test_name[80];

    sprintf(image_name,"pexocp.98.3_%d", cur_image);
    sprintf(test_name, "pexocp.98.3_#%d", cur_image);
     image_testproc(image_name, test_name, img_desc,
		      dpy,window, 0, 0, 
		      glob_window_width, glob_window_height);
    }
