/* $XConsortium: pexocp.96.4.c,v 1.0 93/11/22 12:37:20 rws Exp $ */

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
#include <math.h>
#include <misc.h>
#include <lut.h>

#define NUM_ROWS			9
#define NUM_COLS			15
#define NUM_ROW_POINTS		(NUM_ROWS + 1)
#define NUM_COL_POINTS		(NUM_COLS + 1)
#define	NUM_VERTS			(NUM_ROW_POINTS)*(NUM_COL_POINTS)
#define	NUM_FACETS			(NUM_ROWS * NUM_COLS)
#define	DELTA_Z				0.10
#define Y_START				0.95
#define	Y_END				0.70
#define	X_START				0.0
#define	X_END				0.85
#define	Z_START				0.0
#define X0					((X_END-X_START)/2.0)
#define Y0					0.6
#define	Z_FLAT				0.2
#define	DOWN				0.3

/*
 *	prototypes
 */
void draw_image(
		Display				*dpy,
		XID					 rid,
		PEXOCRequestType	 req_type,
		PEXLookupTable		 light_lut,
		PEXLookupTable		 view_lut );

void set_view(
	Display			*dpy,
	PEXLookupTable	view_table );

void build_axes(
	Display				*dpy,
	XID					 res_id,
	PEXOCRequestType	 req_type,
	PEXCoord			*origin,
	double				 length );


/* 
 * PEXlib Quadrilateral Mesh: Mainline test for all permutations of
 *		vertex data formats.
 *
 *		PEXGAColor
 *		PEXGANormal
 *		PEXGAColor | PEXGANormal
 */

void build_axes(
	Display				*dpy,
	XID					 res_id,
	PEXOCRequestType	 req_type,
	PEXCoord			*origin,
	double				 length )
{
    PEXVertexRGB	x_axis[2], y_axis[2], z_axis[2];
    PEXListOfVertex	axes[3];

    /*
	 *	Set the vertex coordinates and colors.
	 */
    axes[0].count			= 2;
    axes[0].vertices.rgb	= x_axis;

    x_axis[0].point			= *origin;

    x_axis[1].point.x		= origin->x + length;
    x_axis[1].point.y		= origin->y;
    x_axis[1].point.z		= origin->z;

	x_axis[0].rgb.red		= 1.0;
	x_axis[0].rgb.green		= 1.0;
	x_axis[0].rgb.blue		= 1.0;

	x_axis[1].rgb.red		= 1.0;
	x_axis[1].rgb.green		= 1.0;
	x_axis[1].rgb.blue		= 0.0;

    
    axes[1].count			= 2;
    axes[1].vertices.rgb	= y_axis;
    y_axis[0].point			= *origin;
    y_axis[1].point.x		= origin->x;
    y_axis[1].point.y		= origin->y + length;
    y_axis[1].point.z		= origin->z;

	y_axis[0].rgb.red		= 1.0;
	y_axis[0].rgb.green		= 1.0;
	y_axis[0].rgb.blue		= 1.0;

	y_axis[1].rgb.red		= 1.0;
	y_axis[1].rgb.green		= 1.0;
	y_axis[1].rgb.blue		= 0.0;
    
    axes[2].count			= 2;
    axes[2].vertices.rgb	= z_axis;
    z_axis[0].point			= *origin;
    z_axis[1].point.x		= origin->x;
    z_axis[1].point.y		= origin->y;
    z_axis[1].point.z		= origin->z + length;

	z_axis[0].rgb.red		= 1.0;
	z_axis[0].rgb.green		= 1.0;
	z_axis[0].rgb.blue		= 1.0;

	z_axis[1].rgb.red		= 1.0;
	z_axis[1].rgb.green		= 1.0;
	z_axis[1].rgb.blue		= 0.0;

	PEXSetPolylineInterpMethod( dpy, res_id, req_type, PEXPolylineInterpColor );
    PEXPolylineSetWithData( dpy, res_id, req_type, PEXGAColor,
			    PEXColorTypeRGB, 3, axes );
}


void set_view(
	Display			*dpy,
	PEXLookupTable	view_table )
{
    PEXViewEntry	view;
    PEXCoord2D		window[2];
    double			view_plane, front_plane, back_plane;
    PEXCoord		prp;
    PEXNPCSubVolume	viewport;

    /*
	 *	The view orientation parameters.
	 */
    static PEXCoord		view_ref_pt			= {  0.5, 0.5, 0.5 };
    static PEXVector	view_plane_normal	= {  0.0, 0.0,   1 };
    static PEXVector	view_up_vec			= {    0,   1,   0 };

    /*
	 *	Compute the view orientation transform.
	 */
    PEXViewOrientationMatrix( &view_ref_pt, &view_plane_normal,
				    &view_up_vec, view.orientation );
    
    /*
	 *	The view mapping parameters.
	 */
    prp.x = 0.0;
	prp.y = 0.0;
	prp.z =   5;

    window[0].x = -0.8; window[1].x = 0.8;
    window[0].y = -0.9; window[1].y = 0.7;

    front_plane = 3; view_plane = 0; back_plane = -1;

    viewport.min.x = 0; viewport.max.x = 1;
    viewport.min.y = 0; viewport.max.y = 1;
    viewport.min.z = 0; viewport.max.z = 1;
    
    /*
	 *	Compute the view mapping transform.
	 */
    PEXViewMappingMatrix( window, &viewport, True, &prp,
			        view_plane, back_plane, front_plane,
			        view.mapping );
    
    /*
	 *	The view clipping parameters.
	 */
    view.clip_flags		= PEXClipXY;
    view.clip_limits	= viewport;
    
    /*
	 *	Set view 1.
	 */
    PEXSetTableEntries( dpy, view_table, 1, 1, PEXLUTView,
		       (PEXPointer) &view );
}


void draw_image(
		Display				*dpy,
		XID					 res_id,
		PEXOCRequestType	 req_type,
		PEXLookupTable		 light_lut,
		PEXLookupTable		 view_lut )
{	
	/*
	 *	Data declarations
	 */
	PEXTableIndex			lights_on[4];
	unsigned int			color_index;
	PEXCoord				origin;

	PEXColorIndexedNormal	facet_index_normal[NUM_FACETS];

	PEXCoord				pts[NUM_VERTS];
	PEXVertexIndexedNormal	pts_index_normal[NUM_VERTS];

	PEXArrayOfVertex		vertices;
	PEXArrayOfFacetData		facet_data;
	unsigned int			facet_attributes, vertex_attributes;

	PEXReflectionAttributes	refl_attributes;

	unsigned int			col_count, row_count;
	int						i, j, index;
	double					x, dx, y, dy;
	float					norm_angle;

	/*
	 *	Generate the points for the surface.
	 */
	dx = (X_END-X_START)/(NUM_COLS);
	dy = (Y_START-Y_END)/NUM_ROWS;

	y = Y_START;
	for ( i = 0; i < NUM_ROW_POINTS; i++ )
	{
		x = X_START;
		for ( j = 0; j < NUM_COL_POINTS; j++ )
		{
			index = i * NUM_COL_POINTS + j;
			pts[index].x = x + dx;
			pts[index].y = y;
			pts[index].z = (x - X0) * (x - X0) + (y - Y0) * (y - Y0);
			x += dx;
		}
		y -= dy;
	}
    vertices.no_data	= pts;
  

	/*
	 *	Set the view index
	 */
/*
    PEXSetViewIndex( dpy, res_id, req_type, 1 );
*/

	/*
	 *	Build the axes.
	 */
	 origin.x = origin.y = origin.z = 0;
	 build_axes( dpy, res_id, req_type, &origin, 1.0 );


	/*
	 *	Turn on the lights
	 */
	lights_on[0] = 1;
	lights_on[1] = 2;
	lights_on[2] = 3;
	PEXSetLightSourceState( dpy, res_id, req_type, 3, lights_on, 0,
		((PEXTableIndex *) NULL ));

	/*
	 *	Set the reflection model
	 */
	PEXSetReflectionModel( dpy, res_id, req_type, PEXReflectionSpecular );

	/*
	 *  Set the reflection attributes
	 */
	refl_attributes.ambient                         = 1.0;
	refl_attributes.diffuse                         = 1.0;
	refl_attributes.specular                        = 1.0;
	refl_attributes.specular_conc                   = 5.0;
	refl_attributes.transmission                    = 0.0;
	refl_attributes.specular_color.type             = PEXColorTypeRGB;
	refl_attributes.specular_color.value.rgb.red    = 0.0;
	refl_attributes.specular_color.value.rgb.green  = 0.0;
	refl_attributes.specular_color.value.rgb.blue   = 1.0;
	PEXSetReflectionAttributes( dpy, res_id, req_type, &refl_attributes );

    /*
	 *	Distinguish between front and back faces.
	 */
    PEXSetFacetDistinguishFlag( dpy, res_id, req_type, PEXOff );

	/*
	 *	No facet culling
	 */
	PEXSetFacetCullingMode( dpy, res_id, req_type, PEXNone );

	/*
	 *	Hollow interiors for front/back facets
	 */
    PEXSetInteriorStyle( dpy, res_id, req_type, PEXInteriorStyleSolid );
    PEXSetBFInteriorStyle( dpy, res_id, req_type, PEXInteriorStyleSolid );

    /*
	 *	Color index 1 for interior color for front/back facets
	 */
	color_index = 1;
    PEXSetSurfaceColorIndex( dpy, res_id, req_type, color_index );
    PEXSetBFSurfaceColorIndex( dpy, res_id, req_type, color_index );

	/*
	 *	Goraud interior shading
	 */
	PEXSetSurfaceInterpMethod( dpy, res_id, req_type, PEXSurfaceInterpColor );


    /*
	 *	Render the quadrilateral mesh with no optional data.
	 *
	 *		Facet Color   :	None
	 *		Facet Nomral  :	None
	 *
	 *		Vertex Color  :	None
	 *		Vertex Normal :	None
	 */
	facet_data.index	= (PEXColorIndexed *) NULL;

    facet_attributes	= PEXGANone;
    vertex_attributes	= PEXGANone;

	col_count = NUM_COL_POINTS;
	row_count = NUM_ROW_POINTS;

    PEXQuadrilateralMesh(
		dpy, res_id, req_type,
		PEXShapeUnknown,
		facet_attributes,
		vertex_attributes,
		PEXColorTypeIndexed,
		facet_data,
		col_count,
		row_count,
		vertices );

	/*
	 *	Bump the y points down a bit
	 */
	for ( i = 0; i < NUM_ROW_POINTS; i++ )
	{
		for ( j = 0; j < NUM_COL_POINTS; j++ )
		{
			index			= i * NUM_COL_POINTS + j;
			pts[index].y	-= DOWN;
			pts[index].z	= Z_FLAT;
		}
	}
	vertices.no_data	= pts;

	/*
	 *	Add color and normals per facet The quad mesh will be
	 *	"flattened" by setting all of the z values to a
	 *	constant, and then normals per facet will be added
	 *	so the flat surface will have shading effects.
	 */
	norm_angle = M_PI*2/NUM_COLS;
	for ( i = 0; i < NUM_ROWS; i++ )
	{
		for ( j = 0; j < NUM_COLS; j++ )
		{
			index									= i * NUM_COLS + j;
			facet_index_normal[index].index.index	= (i + j) % 8;
			facet_index_normal[index].normal.x		= -(cos( norm_angle * j));
			facet_index_normal[index].normal.y		= 0.0;
			facet_index_normal[index].normal.z		= (sin( norm_angle * j));

		}
	}
	facet_data.index_normal	= facet_index_normal;

    /*
	 *	Render the quadrilateral mesh with color
	 *	and normals per facet.
	 *
	 *		Facet Color   :	Indexed Color
	 *		Facet Normal  :	Yes
	 *
	 *		Vertex Color  :	None
	 *		Vertex Normal :	None
	 */

	facet_attributes	= PEXGAColor | PEXGANormal;

    PEXQuadrilateralMesh(
		dpy, res_id, req_type,
		PEXShapeUnknown,
		facet_attributes,
		vertex_attributes,
		PEXColorTypeIndexed,
		facet_data,
		col_count,
		row_count,
		vertices );

	/*
	 *  Bump the y points down a bit and add color and
	 *	normals per vertex.
	 */
	norm_angle = M_PI*2/NUM_COL_POINTS;
	for ( i = 0; i < NUM_ROW_POINTS; i++ )
	{
		for ( j = 0; j < NUM_COL_POINTS; j++ )
		{
			index			= i * NUM_COL_POINTS + j;
			pts[index].y	-= DOWN;
			pts[index].z	= Z_FLAT;

			pts_index_normal[index].point	= pts[index];

			pts_index_normal[index].index.index	= (i + j) % 8;

			pts_index_normal[index].normal.x = -(cos( norm_angle * j ));
			pts_index_normal[index].normal.y = 0.0;
			pts_index_normal[index].normal.z = (sin( norm_angle * j ));
		}
	}
	vertices.index_normal = pts_index_normal;

    /*
	 *	Render the quadrilateral mesh with color and
	 *	normals per facet.
	 *
	 *		Facet Color   :	Yes, direct RGB
	 *		Facet Normal  :	Yes
	 *
	 *		Vertex Color  :	None
	 *		Vertex Normal :	None
	 */

    vertex_attributes	= PEXGAColor | PEXGANormal;

    PEXQuadrilateralMesh(
		dpy, res_id, req_type,
		PEXShapeUnknown,
		facet_attributes,
		vertex_attributes,
		PEXColorTypeIndexed,
		facet_data,
		col_count,
		row_count,
		vertices );


} /* draw_image */

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
{
    strcpy(test_name, "pexocp.96.4");
    *num_images      = 1;
    *supported_modes = IMM_MODE | STR_MODE | WKS_MODE;

    strcpy(win_title, "pexocp.96.4");
}

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
{
    unsigned long         mask = PEXRAHLHSRMode;
    PEXRendererAttributes attrs;
    PEXLightEntry			lights[3];

    attrs.hlhsr_mode = PEXHLHSRZBuffer;

    PEXChangeRenderer(dpy, renderer, mask, &attrs);
    set_view( dpy, viewLUT );
	/*
	 *	Define the Ambient light (weak)
	 */
	lights[0].type					= PEXLightAmbient;
	lights[0].color.type			= PEXColorTypeRGB;
	lights[0].color.value.rgb.red	= 0.5;
	lights[0].color.value.rgb.green	= 0.5;
	lights[0].color.value.rgb.blue	= 0.5;

	/*
	 *	Define the Directional light
	 */
	lights[1].type					= PEXLightWCVector;
	lights[1].direction.x			=  0.7071;
	lights[1].direction.y			=  0.0;
	lights[1].direction.z			= -0.7071;
	lights[1].color.type			= PEXColorTypeRGB;
	lights[1].color.value.rgb.red	= 1.0;
	lights[1].color.value.rgb.green	= 1.0;
	lights[1].color.value.rgb.blue	= 1.0;

	/*
	 *	Define the Point light
	 */
	lights[2].type					= PEXLightWCPoint;
	lights[2].point.x				= 0.5;
	lights[2].point.y				= 0.5;
	lights[2].point.z				= 1.0;
	lights[2].attenuation1			= 1.0;
	lights[2].attenuation2			= 1.0;
	lights[2].concentration			= 1.0;
	lights[2].spread_angle			= 0.01;
	lights[2].color.type			= PEXColorTypeRGB;
	lights[2].color.value.rgb.red	= 1.0;
	lights[2].color.value.rgb.green	= 1.0;
	lights[2].color.value.rgb.blue	= 1.0;

	PEXSetTableEntries( dpy, lightLUT, 1, 3, PEXLUTLight, lights );
}

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
{
    describe_test( "\nPEXQuadrilateralMesh Mainline Test:\n\n" );
    describe_test( "The top image is a flat white quad mesh with no\n");
    describe_test( "optional data\n\n");
    describe_test( "The middle image is normals per facet and indexed color\n");
    describe_test( "per facet cycling through basic color indices 0-7\n\n");
    describe_test( "The bottom image is normals and indexed color per facet\n");
    describe_test( "plus normals and indexed color per vertex.  On\n");
    describe_test( "the bottom 2 images, normals are artifically rotated\n");
    describe_test( "to produce the 'S' shaped banding.\n\n");

    draw_image (dpy, resourceID, req_type, lightLUT, viewLUT);
}

void testproc(Display *dpy, Window window, int cur_image)
{
     image_testproc("pexocp.96.4", "pexocp.96.4", img_desc,
		      dpy,window, 0, 0, 
		      glob_window_width, glob_window_height);
}
