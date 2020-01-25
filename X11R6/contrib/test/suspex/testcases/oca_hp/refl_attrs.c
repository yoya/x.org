/* $XConsortium: refl_attrs.c,v 1.0 93/11/22 12:31:51 rws Exp $ */

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

/*
 * refl_attrs
 *
 * Ken Tidwell 	Oct. 9, 1992
 */

/*
 * Basic set of include files for tests
 */
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/PEX5/PEXlib.h>
#include <math.h>

#include <lut.h>
#include <misc.h>

#define NORMAL_NORMALS 1
#define INVERTED_NORMALS -1

#define AMBIENT_BASE 1
#define	NUM_AMBIENTS 4
#define AMBIENT_GRAY  AMBIENT_BASE
#define AMBIENT_RED   AMBIENT_BASE + 1
#define AMBIENT_GREEN AMBIENT_BASE + 2 
#define AMBIENT_BLUE  AMBIENT_BASE + 3

#define DIRECT_BASE 10
#define NUM_DIRECTS 6
#define DIRECT_GRAY  DIRECT_BASE
#define DIRECT_RED   DIRECT_BASE + 1
#define DIRECT_GREEN DIRECT_BASE + 2
#define DIRECT_BLUE  DIRECT_BASE + 3
#define DIRECT_SPEC  DIRECT_BASE + 4
#define DIRECT_MODEL DIRECT_BASE + 5

#define POINT_BASE  20
#define NUM_POINTS  5
#define POINT_1_GRAY    POINT_BASE
#define POINT_2_RED     POINT_BASE + 1 
#define POINT_3_GREEN   POINT_BASE + 2
#define POINT_4_BLUE    POINT_BASE + 3
#define POINT_5_MAGENTA POINT_BASE + 4

#define SPOT_BASE   30
#define NUM_SPOTS   18
#define SPOT_1_1_RED	 SPOT_BASE
#define SPOT_1_2_GREEN	 SPOT_BASE + 1
#define SPOT_2_1_BLUE	 SPOT_BASE + 2
#define SPOT_2_2_MAGENTA SPOT_BASE + 3

#define SPOT_1_3_RED	 SPOT_BASE + 4
#define SPOT_1_4_GREEN	 SPOT_BASE + 5
#define SPOT_2_3_BLUE	 SPOT_BASE + 6
#define SPOT_2_4_MAGENTA SPOT_BASE + 7

#define SPOT_3_1_RED	 SPOT_BASE + 8
#define SPOT_3_2_GREEN	 SPOT_BASE + 9
#define SPOT_4_1_BLUE	 SPOT_BASE + 10
#define SPOT_4_2_MAGENTA SPOT_BASE + 11

#define SPOT_3_3_RED	 SPOT_BASE + 12
#define SPOT_3_4_GREEN	 SPOT_BASE + 13
#define SPOT_4_3_BLUE	 SPOT_BASE + 14
#define SPOT_4_4_MAGENTA SPOT_BASE + 15

#define SPOT_BEHIND	 SPOT_BASE + 16
#define SPOT_SPECULAR_1  SPOT_BASE + 17

/*
 * Various entries into the interior LUT
 */
#define FF_AMBIENT_ONLY 2
#define FF_DIFFUSE_ONLY 3
#define BF_AMBIENT_ONLY 4
#define BF_DIFFUSE_ONLY 5
#define FF_AMBIENT_ASF  6
#define BF_AMBIENT_ASF  7
#define OPAQUE		8
#define SEMI_OPAQUE	9
#define TRANSPARENT	10

PEXCoord ff_pgon[4] = {{-20.0, -20.0, 0.0},
    		       {20.0, -20.0, 0.0},
    		       {20.0, 20.0, 0.0},
                       {-20.0, 20.0, 0.0}};

PEXCoord bf_pgon[4] = {
                       {-20.0, 20.0, 0.0},
    		       {20.0, 20.0, 0.0},
    		       {20.0, -20.0, 0.0},
		       {-20.0, -20.0, 0.0}};

PEXCoord ff_pgon_1[4] = {{-20.0, -20.0, 0.0},
    		       {0.0, -20.0, 0.0},
    		       {0.0, 0.0, 0.0},
                       {-20.0, 0.0, 0.0}};

PEXCoord ff_pgon_2[4] = {{-20.0, 0.0, 0.0},
    		       {0.0, 0.0, 0.0},
    		       {0.0, 20.0, 0.0},
                       {-20.0, 20.0, 0.0}};

PEXCoord ff_pgon_3[4] = {{0.0, -20.0, 0.0},
    		       {20.0, -20.0, 0.0},
    		       {20.0, 0.0, 0.0},
                       {0.0, 0.0, 0.0}};

PEXCoord ff_pgon_4[4] = {{0.0, 0.0, 0.0},
    		       {20.0, 0.0, 0.0},
    		       {20.0, 20.0, 0.0},
                       {0.0, 20.0, 0.0}};

#define NUM_LONG_FACETS 60
#define NUM_LAT_FACETS  30
#define NUM_LONG_POINTS (NUM_LONG_FACETS +1)
#define NUM_LAT_POINTS (NUM_LAT_FACETS +1)
#define RADIUS 20.0

/*
 * Normalize
 */
normalize (vector)
PEXVector *vector;
{
    float r;

    r = vector->x * vector->x +
	vector->y * vector->y +
	vector->z * vector->z;

    if (r > 0)
    {
	r = 1.0 / sqrt (r);
	vector->x *= r;
	vector->y *= r;
	vector->z *= r;
    }
}

/*
 * build_sphere
 */
build_sphere (display, renderer, location, req_type,  normal_type, facet_data_flag)
Display *display;
XID renderer;
PEXVector *location;
PEXOCRequestType req_type;
int normal_type;
unsigned int facet_data_flag;
{
    PEXColor color;
    PEXVertexNormal verts[NUM_LONG_POINTS * NUM_LAT_POINTS];
    PEXArrayOfVertex sphere;
    PEXMatrix transform;
    PEXArrayOfFacetData dummy;
    PEXVector tmp_normal, facet_normals[NUM_LONG_FACETS * NUM_LAT_FACETS];
    unsigned int vertex_data_flag;
    double theta = 0, phi = 0;
    int i, j, index, index1, index2, index3, index4;

    for (i = 0; i<NUM_LAT_POINTS; i++)
    {
	for (j = 0; j < NUM_LONG_FACETS; j++)
	{
	    index = i * NUM_LONG_POINTS + j;

	    verts[index].point.x = RADIUS * sin (phi) * sin (theta);
	    verts[index].point.y = RADIUS * cos (phi);
	    verts[index].point.z = RADIUS * sin (phi) * cos (theta);

	    /* Normal is just unit vector to the point */
	    verts[index].normal.x = verts[index].point.x / RADIUS * normal_type;
	    verts[index].normal.y = verts[index].point.y / RADIUS * normal_type;
	    verts[index].normal.z = verts[index].point.z / RADIUS * normal_type;

	    theta += (2 * M_PI)/NUM_LONG_FACETS;
	}
	phi += M_PI / NUM_LAT_FACETS;
	
	/* Close the circle */
	index = i * NUM_LONG_POINTS + NUM_LONG_FACETS;
	verts[index].point = verts[index - NUM_LONG_FACETS].point;
	verts[index].normal = verts[index - NUM_LONG_FACETS].normal;
    }

    /*
     * Compute facet normals using sum of vertex normals
     */
    for (i = 0; i<NUM_LAT_FACETS; i++)
    {
	for (j = 0; j < NUM_LONG_FACETS; j++)
	{
	    tmp_normal.x = tmp_normal.y = tmp_normal.z = 0.0;

	    index = i * NUM_LONG_FACETS + j;

	    index1 = i * NUM_LONG_POINTS + j;
	    index2 = (i+1) * NUM_LONG_POINTS + j;
	    index3 = (i+1) * NUM_LONG_POINTS + (j+1);
	    index4 = i * NUM_LONG_POINTS + (j+1);

	    tmp_normal.x += verts[index1].normal.x;
	    tmp_normal.x += verts[index2].normal.x;
	    tmp_normal.x += verts[index3].normal.x;
	    tmp_normal.x += verts[index4].normal.x;

	    tmp_normal.y += verts[index1].normal.y;
	    tmp_normal.y += verts[index2].normal.y;
	    tmp_normal.y += verts[index3].normal.y;
	    tmp_normal.y += verts[index4].normal.y;

	    tmp_normal.z += verts[index1].normal.z;
	    tmp_normal.z += verts[index2].normal.z;
	    tmp_normal.z += verts[index3].normal.z;
	    tmp_normal.z += verts[index4].normal.z;

	    normalize(&tmp_normal);

	    facet_normals[index].x = tmp_normal.x; /* normal_type; */
	    facet_normals[index].y = tmp_normal.y; /* normal_type; */
	    facet_normals[index].z = tmp_normal.z; /* normal_type; */
	}
    }

    dummy.normal = facet_normals;

    PEXTranslate (location, transform);
    PEXSetLocalTransform (display, renderer, req_type, PEXReplace, 
			  transform);
	
    color.rgb.red =  1.0;
    color.rgb.green = 0.3;
    color.rgb.blue = 0.0;

    PEXSetSurfaceColor (display, renderer, req_type, PEXColorTypeRGB, 
			&color);

    color.rgb.red = 0.3;
    color.rgb.green = 0.3;
    color.rgb.blue = 0.8;

    PEXSetBFSurfaceColor (display, renderer, req_type, PEXColorTypeRGB, 
			&color);

    vertex_data_flag = PEXGANormal;
    sphere.normal = verts;

    PEXQuadrilateralMesh (display, renderer, req_type, PEXShapeConvex,
			facet_data_flag, vertex_data_flag, PEXColorTypeRGB,
			dummy,NUM_LONG_POINTS, NUM_LAT_POINTS, sphere);
}



/*
 * build_white_sphere
 */
build_white_sphere (display, renderer, location, req_type,  normal_type, facet_data_flag)
Display *display;
XID renderer;
PEXVector *location;
PEXOCRequestType req_type;
int normal_type;
unsigned int facet_data_flag;
{
    PEXColor color;
    PEXVertexNormal verts[NUM_LONG_POINTS * NUM_LAT_POINTS];
    PEXArrayOfVertex sphere;
    PEXMatrix transform;
    PEXArrayOfFacetData dummy;
    PEXVector tmp_normal, facet_normals[NUM_LONG_FACETS * NUM_LAT_FACETS];
    unsigned int vertex_data_flag;
    double theta = 0, phi = 0;
    int i, j, index, index1, index2, index3, index4;

    for (i = 0; i<NUM_LAT_POINTS; i++)
    {
	for (j = 0; j < NUM_LONG_FACETS; j++)
	{
	    index = i * NUM_LONG_POINTS + j;

	    verts[index].point.x = RADIUS * sin (phi) * sin (theta);
	    verts[index].point.y = RADIUS * cos (phi);
	    verts[index].point.z = RADIUS * sin (phi) * cos (theta);

	    /* Normal is just unit vector to the point */
	    verts[index].normal.x = verts[index].point.x / RADIUS * normal_type;
	    verts[index].normal.y = verts[index].point.y / RADIUS * normal_type;
	    verts[index].normal.z = verts[index].point.z / RADIUS * normal_type;

	    theta += (2 * M_PI)/NUM_LONG_FACETS;
	}
	phi += M_PI / NUM_LAT_FACETS;
	
	/* Close the circle */
	index = i * NUM_LONG_POINTS + NUM_LONG_FACETS;
	verts[index].point = verts[index - NUM_LONG_FACETS].point;
	verts[index].normal = verts[index - NUM_LONG_FACETS].normal;
    }

    /*
     * Compute facet normals using sum of vertex normals
     */
    for (i = 0; i<NUM_LAT_FACETS; i++)
    {
	for (j = 0; j < NUM_LONG_FACETS; j++)
	{
	    tmp_normal.x = tmp_normal.y = tmp_normal.z = 0.0;

	    index = i * NUM_LONG_FACETS + j;

	    index1 = i * NUM_LONG_POINTS + j;
	    index2 = (i+1) * NUM_LONG_POINTS + j;
	    index3 = (i+1) * NUM_LONG_POINTS + (j+1);
	    index4 = i * NUM_LONG_POINTS + (j+1);

	    tmp_normal.x += verts[index1].normal.x;
	    tmp_normal.x += verts[index2].normal.x;
	    tmp_normal.x += verts[index3].normal.x;
	    tmp_normal.x += verts[index4].normal.x;

	    tmp_normal.y += verts[index1].normal.y;
	    tmp_normal.y += verts[index2].normal.y;
	    tmp_normal.y += verts[index3].normal.y;
	    tmp_normal.y += verts[index4].normal.y;

	    tmp_normal.z += verts[index1].normal.z;
	    tmp_normal.z += verts[index2].normal.z;
	    tmp_normal.z += verts[index3].normal.z;
	    tmp_normal.z += verts[index4].normal.z;

	    normalize(&tmp_normal);

	    facet_normals[index].x = tmp_normal.x; /* normal_type; */
	    facet_normals[index].y = tmp_normal.y; /* normal_type; */
	    facet_normals[index].z = tmp_normal.z; /* normal_type; */
	}
    }

    dummy.normal = facet_normals;

    PEXTranslate (location, transform);
    PEXSetLocalTransform (display, renderer, req_type, PEXReplace, 
			  transform);
	
    color.rgb.red =  0.5;
    color.rgb.green = 0.5;
    color.rgb.blue = 0.5;

    PEXSetSurfaceColor (display, renderer, req_type, PEXColorTypeRGB, 
			&color);

    color.rgb.red = 0.5;
    color.rgb.green = 0.5;
    color.rgb.blue = 0.5;

    PEXSetBFSurfaceColor (display, renderer, req_type, PEXColorTypeRGB, 
			&color);

    vertex_data_flag = PEXGANormal;
    sphere.normal = verts;

    PEXQuadrilateralMesh (display, renderer, req_type, PEXShapeConvex,
			facet_data_flag, vertex_data_flag, PEXColorTypeRGB,
			dummy,NUM_LONG_POINTS, NUM_LAT_POINTS, sphere);
}

/*
 * define_lights
 */
void define_lights(display, table)
Display *display;
PEXLookupTable table;
{
   PEXLightEntry lights[20];

   /*
    * Setup Ambient lights
    */

   /* White */
   lights[0].type = PEXLightAmbient;
   lights[0].color.type = PEXColorTypeRGB;
   lights[0].color.value.rgb.red = 0.3;
   lights[0].color.value.rgb.green = 0.3;
   lights[0].color.value.rgb.blue = 0.3;

   /* Red */
   lights[1].type = PEXLightAmbient;
   lights[1].color.type = PEXColorTypeRGB;
   lights[1].color.value.rgb.red = 1.0;
   lights[1].color.value.rgb.green = 0.0;
   lights[1].color.value.rgb.blue = 0.0;

   /* Green */
   lights[2].type = PEXLightAmbient;
   lights[2].color.type = PEXColorTypeRGB;
   lights[2].color.value.rgb.red = 0.0;
   lights[2].color.value.rgb.green = 0.3;
   lights[2].color.value.rgb.blue = 0.0;

   /* Blue */
   lights[3].type = PEXLightAmbient;
   lights[3].color.type = PEXColorTypeRGB;
   lights[3].color.value.rgb.red = 0.0;
   lights[3].color.value.rgb.green = 0.0;
   lights[3].color.value.rgb.blue = 0.3;

   PEXSetTableEntries (display, table, AMBIENT_BASE, NUM_AMBIENTS, PEXLUTLight,
		       lights);


   /*
    * Setup directional lights
    */

   /* White  straight on*/
   lights[0].type = PEXLightWCVector;
   lights[0].direction.x = 0.0;
   lights[0].direction.y = 0.0;
   lights[0].direction.z = -1.0;
   lights[0].color.type = PEXColorTypeRGB;
   lights[0].color.value.rgb.red = 0.1;
   lights[0].color.value.rgb.green = 0.1;
   lights[0].color.value.rgb.blue = 0.1;

   /* Red  left side 45 deg.*/
   lights[1].type = PEXLightWCVector;
   lights[1].direction.x = cos (M_PI_4);
   lights[1].direction.y = 0.0;
   lights[1].direction.z = -cos (M_PI_4);
   lights[1].color.type = PEXColorTypeRGB;
   lights[1].color.value.rgb.red = 0.3;
   lights[1].color.value.rgb.green = 0.0;
   lights[1].color.value.rgb.blue = 0.0;

   /* Green  right side 45 deg.*/
   lights[2].type = PEXLightWCVector;
   lights[2].direction.x = -cos (M_PI_4);
   lights[2].direction.y = 0.0;
   lights[2].direction.z = -cos (M_PI_4);
   lights[2].color.type = PEXColorTypeRGB;
   lights[2].color.value.rgb.red = 0.0;
   lights[2].color.value.rgb.green = 0.6;
   lights[2].color.value.rgb.blue = 0.0;

   /* Blue  behind*/
   lights[3].type = PEXLightWCVector;
   lights[3].direction.x = 0.0;
   lights[3].direction.y = 0.0;
   lights[3].direction.z = 1.0;
   lights[3].color.type = PEXColorTypeRGB;
   lights[3].color.value.rgb.red = 0.0;
   lights[3].color.value.rgb.green = 0.0;
   lights[3].color.value.rgb.blue = 0.3;

   /* Direct right side 45 deg.*/
   lights[4].type = PEXLightWCVector;
   lights[4].direction.x = -cos (M_PI_4);
   lights[4].direction.y = 0.0;
   lights[4].direction.z = -cos (M_PI_4);
   lights[4].color.type = PEXColorTypeRGB;
   lights[4].color.value.rgb.red = 1.0;
   lights[4].color.value.rgb.green = 1.0;
   lights[4].color.value.rgb.blue = 1.0;

   /* Direct right side 45 deg.*/
   lights[5].type = PEXLightWCVector;
   lights[5].direction.x = -cos (M_PI_4);
   lights[5].direction.y = 0.0;
   lights[5].direction.z = -cos (M_PI_4);
   lights[5].color.type = PEXColorTypeRGB;
   lights[5].color.value.rgb.red = 0.5;
   lights[5].color.value.rgb.green = 0.5;
   lights[5].color.value.rgb.blue = 0.5;

   PEXSetTableEntries (display, table, DIRECT_BASE, NUM_DIRECTS, PEXLUTLight,
		       lights);

   /*
    * Setup point lights
    */
  
   /* Quadrant 1 white */
   lights[0].type = PEXLightWCPoint;
   lights[0].point.x = -55.0;
   lights[0].point.y = 55.0;
   lights[0].point.z = 20.0;
   lights[0].attenuation1 = 1.0;
   lights[0].attenuation2 = 0.1;
   lights[0].color.type = PEXColorTypeRGB;
   lights[0].color.value.rgb.red = 1.0;
   lights[0].color.value.rgb.green = 1.0;
   lights[0].color.value.rgb.blue = 1.0;

   /* Quadrant 2 red */
   lights[1].type = PEXLightWCPoint;
   lights[1].point.x = 55.0;
   lights[1].point.y = 55.0;
   lights[1].point.z = 20.0;
   lights[1].attenuation1 = 1.0;
   lights[1].attenuation2 = 0.1;
   lights[1].color.type = PEXColorTypeRGB;
   lights[1].color.value.rgb.red = 1.0;
   lights[1].color.value.rgb.green = 0.0;
   lights[1].color.value.rgb.blue = 0.0;

   /* Quadrant 3 green */
   lights[2].type = PEXLightWCPoint;
   lights[2].point.x = -55.0;
   lights[2].point.y = -55.0;
   lights[2].point.z = 20.0;
   lights[2].attenuation1 = 1.0;
   lights[2].attenuation2 = 0.1;
   lights[2].color.type = PEXColorTypeRGB;
   lights[2].color.value.rgb.red = 0.0;
   lights[2].color.value.rgb.green = 1.0;
   lights[2].color.value.rgb.blue = 0.0;

   /* Quadrant 4 blue */
   lights[3].type = PEXLightWCPoint;
   lights[3].point.x = 55.0;
   lights[3].point.y = -55.0;
   lights[3].point.z = 20.0;
   lights[3].attenuation1 = 1.0;
   lights[3].attenuation2 = 0.1;
   lights[3].color.type = PEXColorTypeRGB;
   lights[3].color.value.rgb.red = 0.0;
   lights[3].color.value.rgb.green = 0.0;
   lights[3].color.value.rgb.blue = 1.0;

   /* Behind Magenta*/
   lights[4].type = PEXLightWCPoint;
   lights[4].point.x = -55.0;
   lights[4].point.y = 55.0;
   lights[4].point.z = -20.0;
   lights[4].attenuation1 = 1.0;
   lights[4].attenuation2 = 0.1;
   lights[4].color.type = PEXColorTypeRGB;
   lights[4].color.value.rgb.red = 1.0;
   lights[4].color.value.rgb.green = 0.0;
   lights[4].color.value.rgb.blue = 1.0;

   PEXSetTableEntries (display, table, POINT_BASE, NUM_POINTS, PEXLUTLight,
		       lights);

   /*
    * Setup Spot lights (numbering matches polygons)
    */

   /*
    * Quadrant 1
    */

   /* Light 1 (1,1) Red */
   lights[0].type = PEXLightWCSpot;
   lights[0].direction.x = 0.0;
   lights[0].direction.y = 0.0;
   lights[0].direction.z = -1.0;
   lights[0].point.x = -80.0;
   lights[0].point.y = 80.0;
   lights[0].point.z = 20.0;
   lights[0].concentration = 0.0;
   lights[0].spread_angle = 0.970; /* ~55 deg. to encompass pgon */
   lights[0].attenuation1 = 1.0;
   lights[0].attenuation2 = 0.01;
   lights[0].color.type = PEXColorTypeRGB;
   lights[0].color.value.rgb.red = 0.9;
   lights[0].color.value.rgb.green = 0.0;
   lights[0].color.value.rgb.blue = 0.0;

   /* Light 2 (1,2) Green */
   lights[1].type = PEXLightWCSpot;
   lights[1].direction.x = 0.0;
   lights[1].direction.y = 0.0;
   lights[1].direction.z = -1.0;
   lights[1].point.x = -30.0;
   lights[1].point.y = 80.0;
   lights[1].point.z = 20.0;
   lights[1].concentration = 0.0;
   lights[1].spread_angle = 0.97; /* ~55 deg. to encompass pgon */
   lights[1].attenuation1 = 1.0;
   lights[1].attenuation2 = 0.01;
   lights[1].color.type = PEXColorTypeRGB;
   lights[1].color.value.rgb.red = 0.0;
   lights[1].color.value.rgb.green = 0.9;
   lights[1].color.value.rgb.blue = 0.0;

   /* Light 3 (2,1) Blue */
   lights[2].type = PEXLightWCSpot;
   lights[2].direction.x = 0.0;
   lights[2].direction.y = 0.0;
   lights[2].direction.z = -1.0;
   lights[2].point.x = -80.0;
   lights[2].point.y = 30.0;
   lights[2].point.z = 20.0;
   lights[2].concentration = 0.0;
   lights[2].spread_angle = 0.97; /* ~55 deg. to encompass pgon */
   lights[2].attenuation1 = 1.0;
   lights[2].attenuation2 = 0.01;
   lights[2].color.type = PEXColorTypeRGB;
   lights[2].color.value.rgb.red = 0.0;
   lights[2].color.value.rgb.green = 0.0;
   lights[2].color.value.rgb.blue = 0.9;

   /* Light 4 (2,2) Magenta */
   lights[3].type = PEXLightWCSpot;
   lights[3].direction.x = 0.0;
   lights[3].direction.y = 0.0;
   lights[3].direction.z = -1.0;
   lights[3].point.x = -30.0;
   lights[3].point.y = 30.0;
   lights[3].point.z = 20.0;
   lights[3].concentration = 0.0;
   lights[3].spread_angle = 0.97; /* ~55 deg. to encompass pgon */
   lights[3].attenuation1 = 1.0;
   lights[3].attenuation2 = 0.01;
   lights[3].color.type = PEXColorTypeRGB;
   lights[3].color.value.rgb.red = 0.9;
   lights[3].color.value.rgb.green = 0.0;
   lights[3].color.value.rgb.blue = 0.9;

   /*
    * Quadrant 2
    */

   /* Light 5 (1,3) Red */
   lights[4].type = PEXLightWCSpot;
   lights[4].direction.x = 0.0;
   lights[4].direction.y = 0.0;
   lights[4].direction.z = -1.0;
   lights[4].point.x = 30.0;
   lights[4].point.y = 80.0;
   lights[4].point.z = 20.0;
   lights[4].concentration = 0.0;
   lights[4].spread_angle = 0.97; /* ~55 deg. to encompass pgon */
   lights[4].attenuation1 = 1.0;
   lights[4].attenuation2 = 0.01;
   lights[4].color.type = PEXColorTypeRGB;
   lights[4].color.value.rgb.red = 0.9;
   lights[4].color.value.rgb.green = 0.0;
   lights[4].color.value.rgb.blue = 0.0;

   /* Light 6 (1,4) Green */
   lights[5].type = PEXLightWCSpot;
   lights[5].direction.x = 0.0;
   lights[5].direction.y = 0.0;
   lights[5].direction.z = -1.0;
   lights[5].point.x = 80.0;
   lights[5].point.y = 80.0;
   lights[5].point.z = 20.0;
   lights[5].concentration = 0.0;
   lights[5].spread_angle = 0.97; /* ~55 deg. to encompass pgon */
   lights[5].attenuation1 = 1.0;
   lights[5].attenuation2 = 0.01;
   lights[5].color.type = PEXColorTypeRGB;
   lights[5].color.value.rgb.red = 0.0;
   lights[5].color.value.rgb.green = 0.9;
   lights[5].color.value.rgb.blue = 0.0;

   /* Light 7 (2,3) Blue */
   lights[6].type = PEXLightWCSpot;
   lights[6].direction.x = 0.0;
   lights[6].direction.y = 0.0;
   lights[6].direction.z = -1.0;
   lights[6].point.x = 30.0;
   lights[6].point.y = 30.0;
   lights[6].point.z = 20.0;
   lights[6].concentration = 0.0;
   lights[6].spread_angle = 0.97; /* ~55 deg. to encompass pgon */
   lights[6].attenuation1 = 1.0;
   lights[6].attenuation2 = 0.01;
   lights[6].color.type = PEXColorTypeRGB;
   lights[6].color.value.rgb.red = 0.0;
   lights[6].color.value.rgb.green = 0.0;
   lights[6].color.value.rgb.blue = 0.9;

   /* Light 8 (2,4) Magenta */
   lights[7].type = PEXLightWCSpot;
   lights[7].direction.x = 0.0;
   lights[7].direction.y = 0.0;
   lights[7].direction.z = -1.0;
   lights[7].point.x = 80.0;
   lights[7].point.y = 30.0;
   lights[7].point.z = 20.0;
   lights[7].concentration = 0.0;
   lights[7].spread_angle = 0.97; /* ~55 deg. to encompass pgon */
   lights[7].attenuation1 = 1.0;
   lights[7].attenuation2 = 0.01;
   lights[7].color.type = PEXColorTypeRGB;
   lights[7].color.value.rgb.red = 0.9;
   lights[7].color.value.rgb.green = 0.0;
   lights[7].color.value.rgb.blue = 0.9;

   /*
    * Quadrant 3
    */

   /* Light 9 (3,1) Red */
   lights[8].type = PEXLightWCSpot;
   lights[8].direction.x = 0.0;
   lights[8].direction.y = 0.0;
   lights[8].direction.z = -1.0;
   lights[8].point.x = -80.0;
   lights[8].point.y = -30.0;
   lights[8].point.z = 20.0;
   lights[8].concentration = 0.0;
   lights[8].spread_angle = 0.97; /* ~55 deg. to encompass pgon */
   lights[8].attenuation1 = 1.0;
   lights[8].attenuation2 = 0.01;
   lights[8].color.type = PEXColorTypeRGB;
   lights[8].color.value.rgb.red = 0.9;
   lights[8].color.value.rgb.green = 0.0;
   lights[8].color.value.rgb.blue = 0.0;

   /* Light 10 (3,2) Green */
   lights[9].type = PEXLightWCSpot;
   lights[9].direction.x = 0.0;
   lights[9].direction.y = 0.0;
   lights[9].direction.z = -1.0;
   lights[9].point.x = -30.0;
   lights[9].point.y = -30.0;
   lights[9].point.z = 20.0;
   lights[9].concentration = 0.0;
   lights[9].spread_angle = 0.97; /* ~55 deg. to encompass pgon */
   lights[9].attenuation1 = 1.0;
   lights[9].attenuation2 = 0.01;
   lights[9].color.type = PEXColorTypeRGB;
   lights[9].color.value.rgb.red = 0.0;
   lights[9].color.value.rgb.green = 0.9;
   lights[9].color.value.rgb.blue = 0.0;

   /* Light 11 (4,1) Blue */
   lights[10].type = PEXLightWCSpot;
   lights[10].direction.x = 0.0;
   lights[10].direction.y = 0.0;
   lights[10].direction.z = -1.0;
   lights[10].point.x = -80.0;
   lights[10].point.y = -80.0;
   lights[10].point.z = 20.0;
   lights[10].concentration = 0.0;
   lights[10].spread_angle = 0.97; /* ~55 deg. to encompass pgon */
   lights[10].attenuation1 = 1.0;
   lights[10].attenuation2 = 0.01;
   lights[10].color.type = PEXColorTypeRGB;
   lights[10].color.value.rgb.red = 0.0;
   lights[10].color.value.rgb.green = 0.0;
   lights[10].color.value.rgb.blue = 0.9;

   /* Light 12 (4,2) Magenta */
   lights[11].type = PEXLightWCSpot;
   lights[11].direction.x = 0.0;
   lights[11].direction.y = 0.0;
   lights[11].direction.z = -1.0;
   lights[11].point.x = -30.0;
   lights[11].point.y = -80.0;
   lights[11].point.z = 20.0;
   lights[11].concentration = 0.0;
   lights[11].spread_angle = 0.97; /* ~55 deg. to encompass pgon */
   lights[11].attenuation1 = 1.0;
   lights[11].attenuation2 = 0.01;
   lights[11].color.type = PEXColorTypeRGB;
   lights[11].color.value.rgb.red = 0.9;
   lights[11].color.value.rgb.green = 0.0;
   lights[11].color.value.rgb.blue = 0.9;

   /*
    * Quadrant 4
    */

   /* Light 13 (3,3) Red */
   lights[12].type = PEXLightWCSpot;
   lights[12].direction.x = 0.0;
   lights[12].direction.y = 0.0;
   lights[12].direction.z = -1.0;
   lights[12].point.x = 30.0;
   lights[12].point.y = -30.0;
   lights[12].point.z = 20.0;
   lights[12].concentration = 0.0;
   lights[12].spread_angle = 0.97; /* ~55 deg. to encompass pgon */
   lights[12].attenuation1 = 1.0;
   lights[12].attenuation2 = 0.01;
   lights[12].color.type = PEXColorTypeRGB;
   lights[12].color.value.rgb.red = 0.9;
   lights[12].color.value.rgb.green = 0.0;
   lights[12].color.value.rgb.blue = 0.0;

   /* Light 14 (3,4) Green */
   lights[13].type = PEXLightWCSpot;
   lights[13].direction.x = 0.0;
   lights[13].direction.y = 0.0;
   lights[13].direction.z = -1.0;
   lights[13].point.x = 80.0;
   lights[13].point.y = -30.0;
   lights[13].point.z = 20.0;
   lights[13].concentration = 0.0;
   lights[13].spread_angle = 0.97; /* ~55 deg. to encompass pgon */
   lights[13].attenuation1 = 1.0;
   lights[13].attenuation2 = 0.01;
   lights[13].color.type = PEXColorTypeRGB;
   lights[13].color.value.rgb.red = 0.0;
   lights[13].color.value.rgb.green = 0.9;
   lights[13].color.value.rgb.blue = 0.0;

   /* Light 15 (4,3) Blue */
   lights[14].type = PEXLightWCSpot;
   lights[14].direction.x = 0.0;
   lights[14].direction.y = 0.0;
   lights[14].direction.z = -1.0;
   lights[14].point.x = 30.0;
   lights[14].point.y = -80.0;
   lights[14].point.z = 20.0;
   lights[14].concentration = 0.0;
   lights[14].spread_angle = 0.97; /* ~55 deg. to encompass pgon */
   lights[14].attenuation1 = 1.0;
   lights[14].attenuation2 = 0.01;
   lights[14].color.type = PEXColorTypeRGB;
   lights[14].color.value.rgb.red = 0.0;
   lights[14].color.value.rgb.green = 0.0;
   lights[14].color.value.rgb.blue = 0.9;

   /* Light 16 (4,4) Magenta */
   lights[15].type = PEXLightWCSpot;
   lights[15].direction.x = 0.0;
   lights[15].direction.y = 0.0;
   lights[15].direction.z = -1.0;
   lights[15].point.x = 80.0;
   lights[15].point.y = -80.0;
   lights[15].point.z = 20.0;
   lights[15].concentration = 0.0;
   lights[15].spread_angle = 0.97; /* ~55 deg. to encompass pgon */
   lights[15].attenuation1 = 1.0;
   lights[15].attenuation2 = 0.01;
   lights[15].color.type = PEXColorTypeRGB;
   lights[15].color.value.rgb.red = 0.9;
   lights[15].color.value.rgb.green = 0.0;
   lights[15].color.value.rgb.blue = 0.9;

   /* Light 17 behind 16  Cyan */
   lights[16].type = PEXLightWCSpot;
   lights[16].direction.x = 0.0;
   lights[16].direction.y = 0.0;
   lights[16].direction.z = -1.0;
   lights[16].point.x = 80.0;
   lights[16].point.y = -80.0;
   lights[16].point.z = -20.0;
   lights[16].concentration = 0.0;
   lights[16].spread_angle = 0.97; /* ~55 deg. to encompass pgon */
   lights[16].attenuation1 = 1.0;
   lights[16].attenuation2 = 0.01;
   lights[16].color.type = PEXColorTypeRGB;
   lights[16].color.value.rgb.red = 0.0;
   lights[16].color.value.rgb.green = 0.9;
   lights[16].color.value.rgb.blue = 0.9;

   /* Light 18 Positioned to give specular highlight in location (1,1) */

/* Shows lighting overflow!!!
   lights[17].type = PEXLightWCSpot;
   lights[17].direction.x = 0.0;
   lights[17].direction.y = -0.5;
   lights[17].direction.z = -0.866;
   lights[17].point.x = -80.0;
   lights[17].point.y = 103.47;
   lights[17].point.z = 19.70;
   lights[17].concentration = 10.0;
   lights[17].spread_angle = 0.97; 
   lights[17].attenuation1 = 1.0;
   lights[17].attenuation2 = 0.01;
   lights[17].color.type = PEXColorTypeRGB;
   lights[17].color.value.rgb.red = 0.7;
   lights[17].color.value.rgb.green = 0.7;
   lights[17].color.value.rgb.blue = 0.7;
*/

   lights[17].type = PEXLightWCSpot;
   lights[17].direction.x = 0.0;
   lights[17].direction.y = -0.866;
   lights[17].direction.z = 0.5;
   lights[17].point.x = -80.0;
   lights[17].point.y = 114.64;
   lights[17].point.z =  0.0;
   lights[17].concentration = 10.0;
   lights[17].spread_angle = 0.97; /* ~55 deg. to encompass pgon */
   lights[17].attenuation1 = 1.0;
   lights[17].attenuation2 = 0.01;
   lights[17].color.type = PEXColorTypeRGB;
   lights[17].color.value.rgb.red = 0.0;
   lights[17].color.value.rgb.green = 0.0;
   lights[17].color.value.rgb.blue = 1.0;


   PEXSetTableEntries (display, table, SPOT_BASE, NUM_SPOTS, PEXLUTLight,
		       lights);
}


/*
 * Draw_pgons
 */
void draw_pgon_1_1(display, rid, req, pgon)
Display *display;
PEXRenderer rid;
PEXOCRequestType req;
PEXCoord *pgon;
{
    PEXMatrix xform;
    PEXVector translate;

    /*
     * Upper left quadrant (1)
     */

    /* Poly 1 (1,1) */
    translate.x = -80.0; translate.y = 80.0; translate.z = 0.0;
    PEXTranslate (&translate, xform);
    PEXSetLocalTransform (display, rid, req, PEXReplace, xform);
    PEXFillArea (display, rid, req, PEXShapeUnknown, True, 4, pgon);
}

void draw_pgon_1_2(display, rid, req, pgon)
Display *display;
PEXRenderer rid;
PEXOCRequestType req;
PEXCoord *pgon;
{
    PEXMatrix xform;
    PEXVector translate;

    /* Poly 2 (1,2) */
    translate.x = -30.0; translate.y = 80.0; translate.z = 0.0;
    PEXTranslate (&translate, xform);
    PEXSetLocalTransform (display, rid, req, PEXReplace, xform);
    PEXFillArea (display, rid, req, PEXShapeUnknown, True, 4, pgon);

}

void draw_pgon_2_1(display, rid, req, pgon)
Display *display;
PEXRenderer rid;
PEXOCRequestType req;
PEXCoord *pgon;
{
    PEXMatrix xform;
    PEXVector translate;


    /* Poly 3 (2,1) */
    translate.x = -80.0; translate.y = 30.0; translate.z = 0.0;
    PEXTranslate (&translate, xform);
    PEXSetLocalTransform (display, rid, req, PEXReplace, xform);
    PEXFillArea (display, rid, req, PEXShapeUnknown, True, 4, pgon);

}

void draw_pgon_2_2(display, rid, req, pgon)
Display *display;
PEXRenderer rid;
PEXOCRequestType req;
PEXCoord *pgon;
{
    PEXMatrix xform;
    PEXVector translate;


    /* Poly 4 (2,2) */
    translate.x = -30.0; translate.y = 30.0; translate.z = 0.0;
    PEXTranslate (&translate, xform);
    PEXSetLocalTransform (display, rid, req, PEXReplace, xform);
    PEXFillArea (display, rid, req, PEXShapeUnknown, True, 4, pgon);
}

void draw_pgon_1_3(display, rid, req, pgon)
Display *display;
PEXRenderer rid;
PEXOCRequestType req;
PEXCoord *pgon;
{
    PEXMatrix xform;
    PEXVector translate;

    /* Poly 5 (1,3) */
    translate.x = 30.0; translate.y = 80.0; translate.z = 0.0;
    PEXTranslate (&translate, xform);
    PEXSetLocalTransform (display, rid, req, PEXReplace, xform);
    PEXFillArea (display, rid, req, PEXShapeUnknown, True, 4, pgon);
}

void draw_pgon_1_4(display, rid, req, pgon)
Display *display;
PEXRenderer rid;
PEXOCRequestType req;
PEXCoord *pgon;
{
    PEXMatrix xform;
    PEXVector translate;

    /* Poly 6 (1,4) */
    translate.x = 80.0; translate.y = 80.0; translate.z = 0.0;
    PEXTranslate (&translate, xform);
    PEXSetLocalTransform (display, rid, req, PEXReplace, xform);
    PEXFillArea (display, rid, req, PEXShapeUnknown, True, 4, pgon);
}

void draw_pgon_2_3(display, rid, req, pgon)
Display *display;
PEXRenderer rid;
PEXOCRequestType req;
PEXCoord *pgon;
{
    PEXMatrix xform;
    PEXVector translate;

    /* Poly 7 (2,3) */
    translate.x = 30.0; translate.y = 30.0; translate.z = 0.0;
    PEXTranslate (&translate, xform);
    PEXSetLocalTransform (display, rid, req, PEXReplace, xform);
    PEXFillArea (display, rid, req, PEXShapeUnknown, True, 4, pgon);
}

void draw_pgon_2_4(display, rid, req, pgon)
Display *display;
PEXRenderer rid;
PEXOCRequestType req;
PEXCoord *pgon;
{
    PEXMatrix xform;
    PEXVector translate;

    /* Poly 8 (2,4) */
    translate.x = 80.0; translate.y = 30.0; translate.z = 0.0;
    PEXTranslate (&translate, xform);
    PEXSetLocalTransform (display, rid, req, PEXReplace, xform);
    PEXFillArea (display, rid, req, PEXShapeUnknown, True, 4, pgon);
}

void draw_pgon_3_1(display, rid, req, pgon)
Display *display;
PEXRenderer rid;
PEXOCRequestType req;
PEXCoord *pgon;
{
    PEXMatrix xform;
    PEXVector translate;

    /* Poly 9 (3,1) */
    translate.x = -80.0; translate.y = -30.0; translate.z = 0.0;
    PEXTranslate (&translate, xform);
    PEXSetLocalTransform (display, rid, req, PEXReplace, xform);
    PEXFillArea (display, rid, req, PEXShapeUnknown, True, 4, pgon);
}

void draw_pgon_3_2(display, rid, req, pgon)
Display *display;
PEXRenderer rid;
PEXOCRequestType req;
PEXCoord *pgon;
{
    PEXMatrix xform;
    PEXVector translate;

    /* Poly 10 (3,2) */
    translate.x = -30.0; translate.y = -30.0; translate.z = 0.0;
    PEXTranslate (&translate, xform);
    PEXSetLocalTransform (display, rid, req, PEXReplace, xform);
    PEXFillArea (display, rid, req, PEXShapeUnknown, True, 4, pgon);
}

void draw_pgon_4_1(display, rid, req, pgon)
Display *display;
PEXRenderer rid;
PEXOCRequestType req;
PEXCoord *pgon;
{
    PEXMatrix xform;
    PEXVector translate;

    /* Poly 11 (4,1) */
    translate.x = -80.0; translate.y = -80.0; translate.z = 0.0;
    PEXTranslate (&translate, xform);
    PEXSetLocalTransform (display, rid, req, PEXReplace, xform);
    PEXFillArea (display, rid, req, PEXShapeUnknown, True, 4, pgon);
}

void draw_pgon_4_2(display, rid, req, pgon)
Display *display;
PEXRenderer rid;
PEXOCRequestType req;
PEXCoord *pgon;
{
    PEXMatrix xform;
    PEXVector translate;

    /* Poly 12 (4,2) */
    translate.x = -30.0; translate.y = -80.0; translate.z = 0.0;
    PEXTranslate (&translate, xform);
    PEXSetLocalTransform (display, rid, req, PEXReplace, xform);
    PEXFillArea (display, rid, req, PEXShapeUnknown, True, 4, pgon);
}

void draw_pgon_3_3(display, rid, req, pgon)
Display *display;
PEXRenderer rid;
PEXOCRequestType req;
PEXCoord *pgon;
{
    PEXMatrix xform;
    PEXVector translate;

    /* Poly 13 (3,3) */
    translate.x = 30.0; translate.y = -30.0; translate.z = 0.0;
    PEXTranslate (&translate, xform);
    PEXSetLocalTransform (display, rid, req, PEXReplace, xform);
    PEXFillArea (display, rid, req, PEXShapeUnknown, True, 4, pgon);
}

void draw_pgon_3_4(display, rid, req, pgon)
Display *display;
PEXRenderer rid;
PEXOCRequestType req;
PEXCoord *pgon;
{
    PEXMatrix xform;
    PEXVector translate;

    /* Poly 14 (3,4) */
    translate.x = 80.0; translate.y = -30.0; translate.z = 0.0;
    PEXTranslate (&translate, xform);
    PEXSetLocalTransform (display, rid, req, PEXReplace, xform);
    PEXFillArea (display, rid, req, PEXShapeUnknown, True, 4, pgon);
}

void draw_pgon_4_3(display, rid, req, pgon)
Display *display;
PEXRenderer rid;
PEXOCRequestType req;
PEXCoord *pgon;
{
    PEXMatrix xform;
    PEXVector translate;

    /* Poly 15 (4,3) */
    translate.x = 30.0; translate.y = -80.0; translate.z = 0.0;
    PEXTranslate (&translate, xform);
    PEXSetLocalTransform (display, rid, req, PEXReplace, xform);
    PEXFillArea (display, rid, req, PEXShapeUnknown, True, 4, pgon);
}

void draw_pgon_4_4(display, rid, req, pgon)
Display *display;
PEXRenderer rid;
PEXOCRequestType req;
PEXCoord *pgon;
{
    PEXMatrix xform;
    PEXVector translate;

    /* Poly 16 (4,4) */
    translate.x = 30.0; translate.y = -80.0; translate.z = 0.0;
    translate.x = 80.0; translate.y = -80.0; translate.z = 0.0;
    PEXTranslate (&translate, xform);
    PEXSetLocalTransform (display, rid, req, PEXReplace, xform);
    PEXFillArea (display, rid, req, PEXShapeUnknown, True, 4, pgon);
}

/* 
 * define_interiors
 */
void define_interiors (display, interior_lut)
Display *display;
PEXLookupTable interior_lut;

{
    PEXInteriorBundleEntry interior_entries[9];

    /*
     * Set interior characteristics
     */

    /* FF_AMBIENT_ONLY */
    interior_entries[0].style = PEXInteriorStyleSolid;
    interior_entries[0].style_index = 0;
    interior_entries[0].reflection_model = PEXReflectionAmbient;
    interior_entries[0].interp_method = PEXSurfaceInterpColor;
    interior_entries[0].bf_style = PEXInteriorStyleSolid;
    interior_entries[0].bf_style_index = 0;
    interior_entries[0].bf_reflection_model = PEXReflectionSpecular;
    interior_entries[0].bf_interp_method = PEXSurfaceInterpNone;
    interior_entries[0].surface_approx.method = PEXApproxDCRelative;
    interior_entries[0].surface_approx.u_tolerance = 1.0;
    interior_entries[0].surface_approx.v_tolerance = 1.0;
    interior_entries[0].color.type = PEXColorTypeRGB;
    interior_entries[0].color.value.rgb.red = 1.0;
    interior_entries[0].color.value.rgb.green = 1.0;
    interior_entries[0].color.value.rgb.blue = 1.0;
    interior_entries[0].reflection_attr.ambient = 1.0;
    interior_entries[0].reflection_attr.diffuse = 0.0;
    interior_entries[0].reflection_attr.specular = 0.0;
    interior_entries[0].reflection_attr.specular_conc = 0.0;
    interior_entries[0].reflection_attr.transmission = 0.0;
    interior_entries[0].reflection_attr.specular_color.type = PEXColorTypeRGB;
    interior_entries[0].reflection_attr.specular_color.value.rgb.red = 0.0;
    interior_entries[0].reflection_attr.specular_color.value.rgb.green = 0.0;
    interior_entries[0].reflection_attr.specular_color.value.rgb.blue = 0.0;
    interior_entries[0].bf_color.type = PEXColorTypeRGB;
    interior_entries[0].bf_color.value.rgb.red = 1.0;
    interior_entries[0].bf_color.value.rgb.green = 1.0;
    interior_entries[0].bf_color.value.rgb.blue = 1.0;
    interior_entries[0].bf_reflection_attr.specular_color.value.rgb.red = 0.0;
    interior_entries[0].bf_reflection_attr.specular_color.value.rgb.green = 0.0;
    interior_entries[0].bf_reflection_attr.specular_color.value.rgb.blue = 0.0;
    interior_entries[0].bf_reflection_attr.ambient = 1.0;
    interior_entries[0].bf_reflection_attr.diffuse = 1.0;
    interior_entries[0].bf_reflection_attr.specular = 1.0;
    interior_entries[0].bf_reflection_attr.specular_conc = 100.0;
    interior_entries[0].bf_reflection_attr.transmission = 0.0;
    interior_entries[0].bf_reflection_attr.specular_color.type = PEXColorTypeRGB;
    interior_entries[0].bf_reflection_attr.specular_color.value.rgb.red = 1.0;
    interior_entries[0].bf_reflection_attr.specular_color.value.rgb.green = 1.0;
    interior_entries[0].bf_reflection_attr.specular_color.value.rgb.blue = 1.0;

    /* FF_DIFFUSE_ONLY */
    interior_entries[1].style = PEXInteriorStyleSolid;
    interior_entries[1].style_index = 0;
    interior_entries[1].reflection_model = PEXReflectionDiffuse;
    interior_entries[1].interp_method = PEXSurfaceInterpColor;
    interior_entries[1].bf_style = PEXInteriorStyleSolid;
    interior_entries[1].bf_style_index = 0;
    interior_entries[1].bf_reflection_model = PEXReflectionSpecular;
    interior_entries[1].bf_interp_method = PEXSurfaceInterpNone;
    interior_entries[1].surface_approx.method = PEXApproxDCRelative;
    interior_entries[1].surface_approx.u_tolerance = 1.0;
    interior_entries[1].surface_approx.v_tolerance = 1.0;
    interior_entries[1].color.type = PEXColorTypeRGB;
    interior_entries[1].color.value.rgb.red = 1.0;
    interior_entries[1].color.value.rgb.green = 1.0;
    interior_entries[1].color.value.rgb.blue = 1.0;
    interior_entries[1].reflection_attr.ambient = 0.0;
    interior_entries[1].reflection_attr.diffuse = 1.0;
    interior_entries[1].reflection_attr.specular = 0.0;
    interior_entries[1].reflection_attr.specular_conc = 0.0;
    interior_entries[1].reflection_attr.transmission = 0.0;
    interior_entries[1].reflection_attr.specular_color.type = PEXColorTypeRGB;
    interior_entries[1].reflection_attr.specular_color.value.rgb.red = 0.0;
    interior_entries[1].reflection_attr.specular_color.value.rgb.green = 0.0;
    interior_entries[1].reflection_attr.specular_color.value.rgb.blue = 0.0;
    interior_entries[1].bf_color.type = PEXColorTypeRGB;
    interior_entries[1].bf_color.value.rgb.red = 1.0;
    interior_entries[1].bf_color.value.rgb.green = 1.0;
    interior_entries[1].bf_color.value.rgb.blue = 1.0;
    interior_entries[1].bf_reflection_attr.specular_color.value.rgb.red = 0.0;
    interior_entries[1].bf_reflection_attr.specular_color.value.rgb.green = 0.0;
    interior_entries[1].bf_reflection_attr.specular_color.value.rgb.blue = 0.0;
    interior_entries[1].bf_reflection_attr.ambient = 1.0;
    interior_entries[1].bf_reflection_attr.diffuse = 1.0;
    interior_entries[1].bf_reflection_attr.specular = 1.0;
    interior_entries[1].bf_reflection_attr.specular_conc = 100.0;
    interior_entries[1].bf_reflection_attr.transmission = 0.0;
    interior_entries[1].bf_reflection_attr.specular_color.type = PEXColorTypeRGB;
    interior_entries[1].bf_reflection_attr.specular_color.value.rgb.red = 1.0;
    interior_entries[1].bf_reflection_attr.specular_color.value.rgb.green = 1.0;
    interior_entries[1].bf_reflection_attr.specular_color.value.rgb.blue = 1.0;

    /* BF_AMBIENT_ONLY */
    interior_entries[2].style = PEXInteriorStyleSolid;
    interior_entries[2].style_index = 0;
    interior_entries[2].reflection_model = PEXReflectionSpecular;
    interior_entries[2].interp_method = PEXSurfaceInterpNone;
    interior_entries[2].bf_style = PEXInteriorStyleSolid;
    interior_entries[2].bf_style_index = 0;
    interior_entries[2].bf_reflection_model = PEXReflectionAmbient;
    interior_entries[2].bf_interp_method = PEXSurfaceInterpColor;
    interior_entries[2].surface_approx.method = PEXApproxDCRelative;
    interior_entries[2].surface_approx.u_tolerance = 1.0;
    interior_entries[2].surface_approx.v_tolerance = 1.0;
    interior_entries[2].color.type = PEXColorTypeRGB;
    interior_entries[2].color.value.rgb.red = 1.0;
    interior_entries[2].color.value.rgb.green = 1.0;
    interior_entries[2].color.value.rgb.blue = 1.0;
    interior_entries[2].reflection_attr.ambient = 0.5;
    interior_entries[2].reflection_attr.diffuse = 0.0;
    interior_entries[2].reflection_attr.specular = 0.0;
    interior_entries[2].reflection_attr.specular_conc = 10.0;
    interior_entries[2].reflection_attr.transmission = 0.0;
    interior_entries[2].reflection_attr.specular_color.type = PEXColorTypeRGB;
    interior_entries[2].reflection_attr.specular_color.value.rgb.red = 0.0;
    interior_entries[2].reflection_attr.specular_color.value.rgb.green = 0.0;
    interior_entries[2].reflection_attr.specular_color.value.rgb.blue = 0.0;
    interior_entries[2].bf_color.type = PEXColorTypeRGB;
    interior_entries[2].bf_color.value.rgb.red = 1.0;
    interior_entries[2].bf_color.value.rgb.green = 1.0;
    interior_entries[2].bf_color.value.rgb.blue = 1.0;
    interior_entries[2].bf_reflection_attr.specular_color.value.rgb.red = 0.0;
    interior_entries[2].bf_reflection_attr.specular_color.value.rgb.green = 0.0;
    interior_entries[2].bf_reflection_attr.specular_color.value.rgb.blue = 0.0;
    interior_entries[2].bf_reflection_attr.ambient = 1.0;
    interior_entries[2].bf_reflection_attr.diffuse = 0.0;
    interior_entries[2].bf_reflection_attr.specular = 0.0;
    interior_entries[2].bf_reflection_attr.specular_conc = 100.0;
    interior_entries[2].bf_reflection_attr.transmission = 0.0;
    interior_entries[2].bf_reflection_attr.specular_color.type = PEXColorTypeRGB;
    interior_entries[2].bf_reflection_attr.specular_color.value.rgb.red = 1.0;
    interior_entries[2].bf_reflection_attr.specular_color.value.rgb.green = 1.0;
    interior_entries[2].bf_reflection_attr.specular_color.value.rgb.blue = 1.0;

    /* BF_DIFFUSE_ONLY */
    interior_entries[3].style = PEXInteriorStyleSolid;
    interior_entries[3].style_index = 0;
    interior_entries[3].reflection_model = PEXReflectionSpecular;
    interior_entries[3].interp_method = PEXSurfaceInterpNone;
    interior_entries[3].bf_style = PEXInteriorStyleSolid;
    interior_entries[3].bf_style_index = 0;
    interior_entries[3].bf_reflection_model = PEXReflectionDiffuse;
    interior_entries[3].bf_interp_method = PEXSurfaceInterpColor;
    interior_entries[3].surface_approx.method = PEXApproxDCRelative;
    interior_entries[3].surface_approx.u_tolerance = 1.0;
    interior_entries[3].surface_approx.v_tolerance = 1.0;
    interior_entries[3].color.type = PEXColorTypeRGB;
    interior_entries[3].color.value.rgb.red = 1.0;
    interior_entries[3].color.value.rgb.green = 0.0;
    interior_entries[3].color.value.rgb.blue = 0.0;
    interior_entries[3].reflection_attr.ambient = 1.0;
    interior_entries[3].reflection_attr.diffuse = 1.0;
    interior_entries[3].reflection_attr.specular = 1.0;
    interior_entries[3].reflection_attr.specular_conc = 10.0;
    interior_entries[3].reflection_attr.transmission = 0.0;
    interior_entries[3].reflection_attr.specular_color.type = PEXColorTypeRGB;
    interior_entries[3].reflection_attr.specular_color.value.rgb.red = 0.0;
    interior_entries[3].reflection_attr.specular_color.value.rgb.green = 0.0;
    interior_entries[3].reflection_attr.specular_color.value.rgb.blue = 0.0;
    interior_entries[3].bf_color.type = PEXColorTypeRGB;
    interior_entries[3].bf_color.value.rgb.red = 1.0;
    interior_entries[3].bf_color.value.rgb.green = 1.0;
    interior_entries[3].bf_color.value.rgb.blue = 1.0;
    interior_entries[3].bf_reflection_attr.specular_color.value.rgb.red = 0.0;
    interior_entries[3].bf_reflection_attr.specular_color.value.rgb.green = 0.0;
    interior_entries[3].bf_reflection_attr.specular_color.value.rgb.blue = 0.0;
    interior_entries[3].bf_reflection_attr.ambient = 0.0;
    interior_entries[3].bf_reflection_attr.diffuse = 1.0;
    interior_entries[3].bf_reflection_attr.specular = 0.0;
    interior_entries[3].bf_reflection_attr.specular_conc = 100.0;
    interior_entries[3].bf_reflection_attr.transmission = 0.0;
    interior_entries[3].bf_reflection_attr.specular_color.type = PEXColorTypeRGB;
    interior_entries[3].bf_reflection_attr.specular_color.value.rgb.red = 1.0;
    interior_entries[3].bf_reflection_attr.specular_color.value.rgb.green = 1.0;
    interior_entries[3].bf_reflection_attr.specular_color.value.rgb.blue = 1.0;

    /* FF_AMBIENT_ASF */
    interior_entries[4].style = PEXInteriorStyleSolid;
    interior_entries[4].style_index = 0;
    interior_entries[4].reflection_model = PEXReflectionAmbient;
    interior_entries[4].interp_method = PEXSurfaceInterpNone;
    interior_entries[4].bf_style = PEXInteriorStyleSolid;
    interior_entries[4].bf_style_index = 0;
    interior_entries[4].bf_reflection_model = PEXReflectionAmbient;
    interior_entries[4].bf_interp_method = PEXSurfaceInterpColor;
    interior_entries[4].surface_approx.method = PEXApproxDCRelative;
    interior_entries[4].surface_approx.u_tolerance = 1.0;
    interior_entries[4].surface_approx.v_tolerance = 1.0;
    interior_entries[4].color.type = PEXColorTypeRGB;
    interior_entries[4].color.value.rgb.red = 1.0;
    interior_entries[4].color.value.rgb.green = 0.0;
    interior_entries[4].color.value.rgb.blue = 0.0;
    interior_entries[4].reflection_attr.ambient = 1.0;
    interior_entries[4].reflection_attr.diffuse = 0.0;
    interior_entries[4].reflection_attr.specular = 0.0;
    interior_entries[4].reflection_attr.specular_conc = 10.0;
    interior_entries[4].reflection_attr.transmission = 0.0;
    interior_entries[4].reflection_attr.specular_color.type = PEXColorTypeRGB;
    interior_entries[4].reflection_attr.specular_color.value.rgb.red = 0.0;
    interior_entries[4].reflection_attr.specular_color.value.rgb.green = 0.0;
    interior_entries[4].reflection_attr.specular_color.value.rgb.blue = 0.0;
    interior_entries[4].bf_color.type = PEXColorTypeRGB;
    interior_entries[4].bf_color.value.rgb.red = 1.0;
    interior_entries[4].bf_color.value.rgb.green = 1.0;
    interior_entries[4].bf_color.value.rgb.blue = 1.0;
    interior_entries[4].bf_reflection_attr.specular_color.value.rgb.red = 0.0;
    interior_entries[4].bf_reflection_attr.specular_color.value.rgb.green = 0.0;
    interior_entries[4].bf_reflection_attr.specular_color.value.rgb.blue = 0.0;
    interior_entries[4].bf_reflection_attr.ambient = 1.0;
    interior_entries[4].bf_reflection_attr.diffuse = 0.0;
    interior_entries[4].bf_reflection_attr.specular = 0.0;
    interior_entries[4].bf_reflection_attr.specular_conc = 100.0;
    interior_entries[4].bf_reflection_attr.transmission = 0.0;
    interior_entries[4].bf_reflection_attr.specular_color.type = PEXColorTypeRGB;

    /* BF_AMBIENT_ASF */
    interior_entries[5].style = PEXInteriorStyleSolid;
    interior_entries[5].style_index = 0;
    interior_entries[5].reflection_model = PEXReflectionSpecular;
    interior_entries[5].interp_method = PEXSurfaceInterpColor;
    interior_entries[5].bf_style = PEXInteriorStyleSolid;
    interior_entries[5].bf_style_index = 0;
    interior_entries[5].bf_reflection_model = PEXReflectionSpecular;
    interior_entries[5].bf_interp_method = PEXSurfaceInterpColor;
    interior_entries[5].surface_approx.method = PEXApproxDCRelative;
    interior_entries[5].surface_approx.u_tolerance = 1.0;
    interior_entries[5].surface_approx.v_tolerance = 1.0;
    interior_entries[5].color.type = PEXColorTypeRGB;
    interior_entries[5].color.value.rgb.red = 1.0;
    interior_entries[5].color.value.rgb.green = 1.0;
    interior_entries[5].color.value.rgb.blue = 1.0;
    interior_entries[5].reflection_attr.ambient = 1.0;
    interior_entries[5].reflection_attr.diffuse = 1.0;
    interior_entries[5].reflection_attr.specular = 1.0;
    interior_entries[5].reflection_attr.specular_conc = 10.0;
    interior_entries[5].reflection_attr.transmission = 0.0;
    interior_entries[5].reflection_attr.specular_color.type = PEXColorTypeRGB;
    interior_entries[5].reflection_attr.specular_color.value.rgb.red = 1.0;
    interior_entries[5].reflection_attr.specular_color.value.rgb.green = 0.0;
    interior_entries[5].reflection_attr.specular_color.value.rgb.blue = 1.0;
    interior_entries[5].bf_color.type = PEXColorTypeRGB;
    interior_entries[5].bf_color.value.rgb.red = 1.0;
    interior_entries[5].bf_color.value.rgb.green = 1.0;
    interior_entries[5].bf_color.value.rgb.blue = 1.0;
    interior_entries[5].bf_reflection_attr.specular_color.type = PEXColorTypeRGB;
    interior_entries[5].bf_reflection_attr.specular_color.value.rgb.red = 1.0;
    interior_entries[5].bf_reflection_attr.specular_color.value.rgb.green = 0.0;
    interior_entries[5].bf_reflection_attr.specular_color.value.rgb.blue = 1.0;
    interior_entries[5].bf_reflection_attr.ambient = 1.0;
    interior_entries[5].bf_reflection_attr.diffuse = 1.0;
    interior_entries[5].bf_reflection_attr.specular = 1.0;
    interior_entries[5].bf_reflection_attr.specular_conc = 10.0;
    interior_entries[5].bf_reflection_attr.transmission = 0.0;

    /* OPAQUE */
    interior_entries[6].style = PEXInteriorStyleSolid;
    interior_entries[6].style_index = 0;
    interior_entries[6].reflection_model = PEXReflectionSpecular;
    interior_entries[6].interp_method = PEXSurfaceInterpNone;
    interior_entries[6].bf_style = PEXInteriorStyleSolid;
    interior_entries[6].bf_style_index = 0;
    interior_entries[6].bf_reflection_model = PEXReflectionSpecular;
    interior_entries[6].bf_interp_method = PEXSurfaceInterpNone;
    interior_entries[6].surface_approx.method = PEXApproxDCRelative;
    interior_entries[6].surface_approx.u_tolerance = 1.0;
    interior_entries[6].surface_approx.v_tolerance = 1.0;
    interior_entries[6].color.type = PEXColorTypeRGB;
    interior_entries[6].color.value.rgb.red = 1.0;
    interior_entries[6].color.value.rgb.green = 1.0;
    interior_entries[6].color.value.rgb.blue = 1.0;
    interior_entries[6].reflection_attr.ambient = 1.0;
    interior_entries[6].reflection_attr.diffuse = 1.0;
    interior_entries[6].reflection_attr.specular = 1.0;
    interior_entries[6].reflection_attr.specular_conc = 10.0;
    interior_entries[6].reflection_attr.transmission = 0.0;
    interior_entries[6].reflection_attr.specular_color.type = PEXColorTypeRGB;
    interior_entries[6].reflection_attr.specular_color.value.rgb.red = 1.0;
    interior_entries[6].reflection_attr.specular_color.value.rgb.green = 0.0;
    interior_entries[6].reflection_attr.specular_color.value.rgb.blue = 1.0;
    interior_entries[6].bf_color.type = PEXColorTypeRGB;
    interior_entries[6].bf_color.value.rgb.red = 1.0;
    interior_entries[6].bf_color.value.rgb.green = 1.0;
    interior_entries[6].bf_color.value.rgb.blue = 1.0;
    interior_entries[6].bf_reflection_attr.specular_color.type = PEXColorTypeRGB;
    interior_entries[6].bf_reflection_attr.specular_color.value.rgb.red = 1.0;
    interior_entries[6].bf_reflection_attr.specular_color.value.rgb.green = 0.0;
    interior_entries[6].bf_reflection_attr.specular_color.value.rgb.blue = 1.0;
    interior_entries[6].bf_reflection_attr.ambient = 1.0;
    interior_entries[6].bf_reflection_attr.diffuse = 1.0;
    interior_entries[6].bf_reflection_attr.specular = 1.0;
    interior_entries[6].bf_reflection_attr.specular_conc = 10.0;
    interior_entries[6].bf_reflection_attr.transmission = 0.0;

    /* SEMI_OPAQUE */
    interior_entries[7].style = PEXInteriorStyleSolid;
    interior_entries[7].style_index = 0;
    interior_entries[7].reflection_model = PEXReflectionSpecular;
    interior_entries[7].interp_method = PEXSurfaceInterpColor;
    interior_entries[7].bf_style = PEXInteriorStyleSolid;
    interior_entries[7].bf_style_index = 0;
    interior_entries[7].bf_reflection_model = PEXReflectionSpecular;
    interior_entries[7].bf_interp_method = PEXSurfaceInterpColor;
    interior_entries[7].surface_approx.method = PEXApproxDCRelative;
    interior_entries[7].surface_approx.u_tolerance = 1.0;
    interior_entries[7].surface_approx.v_tolerance = 1.0;
    interior_entries[7].color.type = PEXColorTypeRGB;
    interior_entries[7].color.value.rgb.red = 1.0;
    interior_entries[7].color.value.rgb.green = 1.0;
    interior_entries[7].color.value.rgb.blue = 1.0;
    interior_entries[7].reflection_attr.ambient = 1.0;
    interior_entries[7].reflection_attr.diffuse = 1.0;
    interior_entries[7].reflection_attr.specular = 1.0;
    interior_entries[7].reflection_attr.specular_conc = 10.0;
    interior_entries[7].reflection_attr.transmission = 0.5;
    interior_entries[7].reflection_attr.specular_color.type = PEXColorTypeRGB;
    interior_entries[7].reflection_attr.specular_color.value.rgb.red = 1.0;
    interior_entries[7].reflection_attr.specular_color.value.rgb.green = 0.0;
    interior_entries[7].reflection_attr.specular_color.value.rgb.blue = 1.0;
    interior_entries[7].bf_color.type = PEXColorTypeRGB;
    interior_entries[7].bf_color.value.rgb.red = 1.0;
    interior_entries[7].bf_color.value.rgb.green = 1.0;
    interior_entries[7].bf_color.value.rgb.blue = 1.0;
    interior_entries[7].bf_reflection_attr.specular_color.type = PEXColorTypeRGB;
    interior_entries[7].bf_reflection_attr.specular_color.value.rgb.red = 1.0;
    interior_entries[7].bf_reflection_attr.specular_color.value.rgb.green = 0.0;
    interior_entries[7].bf_reflection_attr.specular_color.value.rgb.blue = 1.0;
    interior_entries[7].bf_reflection_attr.ambient = 1.0;
    interior_entries[7].bf_reflection_attr.diffuse = 1.0;
    interior_entries[7].bf_reflection_attr.specular = 1.0;
    interior_entries[7].bf_reflection_attr.specular_conc = 10.0;
    interior_entries[7].bf_reflection_attr.transmission = 0.5;

    /* TRANSPARENT */
    interior_entries[8].style = PEXInteriorStyleSolid;
    interior_entries[8].style_index = 0;
    interior_entries[8].reflection_model = PEXReflectionSpecular;
    interior_entries[8].interp_method = PEXSurfaceInterpColor;
    interior_entries[8].bf_style = PEXInteriorStyleSolid;
    interior_entries[8].bf_style_index = 0;
    interior_entries[8].bf_reflection_model = PEXReflectionSpecular;
    interior_entries[8].bf_interp_method = PEXSurfaceInterpColor;
    interior_entries[8].surface_approx.method = PEXApproxDCRelative;
    interior_entries[8].surface_approx.u_tolerance = 1.0;
    interior_entries[8].surface_approx.v_tolerance = 1.0;
    interior_entries[8].color.type = PEXColorTypeRGB;
    interior_entries[8].color.value.rgb.red = 1.0;
    interior_entries[8].color.value.rgb.green = 1.0;
    interior_entries[8].color.value.rgb.blue = 1.0;
    interior_entries[8].reflection_attr.ambient = 1.0;
    interior_entries[8].reflection_attr.diffuse = 1.0;
    interior_entries[8].reflection_attr.specular = 1.0;
    interior_entries[8].reflection_attr.specular_conc = 10.0;
    interior_entries[8].reflection_attr.transmission = 0.9;
    interior_entries[8].reflection_attr.specular_color.type = PEXColorTypeRGB;
    interior_entries[8].reflection_attr.specular_color.value.rgb.red = 1.0;
    interior_entries[8].reflection_attr.specular_color.value.rgb.green = 0.0;
    interior_entries[8].reflection_attr.specular_color.value.rgb.blue = 1.0;
    interior_entries[8].bf_color.type = PEXColorTypeRGB;
    interior_entries[8].bf_color.value.rgb.red = 1.0;
    interior_entries[8].bf_color.value.rgb.green = 1.0;
    interior_entries[8].bf_color.value.rgb.blue = 1.0;
    interior_entries[8].bf_reflection_attr.specular_color.type = PEXColorTypeRGB;
    interior_entries[8].bf_reflection_attr.specular_color.value.rgb.red = 1.0;
    interior_entries[8].bf_reflection_attr.specular_color.value.rgb.green = 0.0;
    interior_entries[8].bf_reflection_attr.specular_color.value.rgb.blue = 1.0;
    interior_entries[8].bf_reflection_attr.ambient = 1.0;
    interior_entries[8].bf_reflection_attr.diffuse = 1.0;
    interior_entries[8].bf_reflection_attr.specular = 1.0;
    interior_entries[8].bf_reflection_attr.specular_conc = 10.0;
    interior_entries[8].bf_reflection_attr.transmission = 0.9;

    PEXSetTableEntries (display, interior_lut, 2, 9, 
			PEXLUTInteriorBundle, interior_entries);
}


/*
 * define_view
 */
void define_views (display, view_lut)
Display *display;
PEXLookupTable view_lut;
{
    PEXViewEntry view_entry;
    PEXCoord vrp, prp;
    PEXVector vpn, vup;
    PEXCoord2D frame[2];
    PEXNPCSubVolume viewport;
	    
    /*
     * Compute view matrices for test and setup view entry
     */ 
    vrp.x = 0.0; vrp.y = 0.0; vrp.z = 100.0;
    vpn.x = 0.0; vpn.y = 0.0; vpn.z = 1.0;
    vup.x = 0.0; vup.y = 1.0; vup.z = 0.0;

    PEXViewOrientationMatrix (&vrp, &vpn, &vup, view_entry.orientation);

    frame[0].x = -110.0; frame[0].y = -110.0; 
    frame[1].x = 110.0; frame[1].y = 110.0; 
	
    viewport.min.x = 0.0; viewport.min.y = 0.0; viewport.min.z = 0.0;
    viewport.max.x = 1.0; viewport.max.y = 1.0; viewport.max.z = 1.0;


    prp.x = 0.0; prp.y = 0.0; prp.z = -1.0;

    PEXViewMappingMatrix (frame, &viewport, False, &prp, -20.0, -110.0, 
			  -10.0, view_entry.mapping);

    view_entry.clip_flags = PEXClipXY | PEXClipBack | PEXClipFront;
    view_entry.clip_limits.min.x = 0.0; 
    view_entry.clip_limits.min.y = 0.0;
    view_entry.clip_limits.min.z = 0.0;
    view_entry.clip_limits.max.x = 1.0; 
    view_entry.clip_limits.max.y = 1.0;
    view_entry.clip_limits.max.z = 1.0;

    PEXSetTableEntries (display, view_lut, 1, 1, PEXLUTView, 
			&view_entry);
}


Test_1 (display, target, req_type)
Display *display;
XID target;
PEXOCRequestType req_type;
{
    PEXReflectionAttributes refl_attrs;
    PEXTableIndex enable[NUM_AMBIENTS + NUM_DIRECTS + NUM_POINTS + NUM_SPOTS];
    PEXTableIndex disable[NUM_AMBIENTS + NUM_DIRECTS + NUM_POINTS + NUM_SPOTS];
    PEXColor color;

   /* 
    * Select view 
    */
   PEXSetViewIndex (display, target, req_type, 1);

   /*
    * Setup lights
    */
   enable[0] = AMBIENT_RED;
   enable[2] = DIRECT_BLUE;
   enable[1] = DIRECT_GREEN;
   PEXSetLightSourceState (display, target, req_type,
			   3, enable, 0, NULL);


   /*
    * Select interior attributes and draw pgons
    * No Face distinquishing by default
    */
   PEXSetInteriorStyle (display, target, req_type,
			PEXInteriorStyleSolid);
   PEXSetBFInteriorStyle (display, target, req_type,
			  PEXInteriorStyleSolid);
   PEXSetReflectionModel (display, target, req_type, 
			      PEXReflectionDiffuse);
   PEXSetBFReflectionModel (display, target, req_type, 
			  PEXReflectionDiffuse);
   color.rgb.red = 1.0;
   color.rgb.green = 1.0;
   color.rgb.blue = 1.0;

   PEXSetSurfaceColor (display, target, req_type,
			PEXColorTypeRGB, &color);

   PEXSetBFSurfaceColor (display, target, req_type,
			PEXColorTypeRGB, &color);


   refl_attrs.ambient = 1.0;
   refl_attrs.diffuse = 0.0;
   refl_attrs.specular = 0.0;
   refl_attrs.transmission = 0.0;
   refl_attrs.specular_color.type  = PEXColorTypeRGB;
   refl_attrs.specular_color.value = color;

   PEXSetReflectionAttributes (display, target, req_type, 
			       &refl_attrs);
   
   draw_pgon_1_1 (display, target, req_type, ff_pgon);

   refl_attrs.ambient = 0.0;
   refl_attrs.diffuse = 1.0;
   PEXSetReflectionAttributes (display, target, req_type, 
			       &refl_attrs);

   draw_pgon_1_2 (display, target, req_type, ff_pgon);

   /*
    * BF Attrs should be ignored
    */
   refl_attrs.ambient = 1.0;
   refl_attrs.diffuse = 0.0;
   PEXSetBFReflectionAttributes (display, target, req_type,
				 &refl_attrs);

   refl_attrs.ambient = 1.0;
   refl_attrs.diffuse = 0.0;
   PEXSetReflectionAttributes (display, target, req_type, 
			       &refl_attrs);

   draw_pgon_2_1 (display, target, req_type, bf_pgon);

   refl_attrs.ambient = 0.0;
   refl_attrs.diffuse = 1.0;
   PEXSetReflectionAttributes (display, target, req_type,
				 &refl_attrs);

   draw_pgon_2_2 (display, target, req_type, bf_pgon);

   /* 
    * Jumble everything up before going to bundles
    */

   PEXSetInteriorStyle (display, target, req_type,
			PEXInteriorStyleHollow);
   PEXSetBFInteriorStyle (display, target, req_type,
			  PEXInteriorStyleHollow);
   PEXSetReflectionModel (display, target, req_type,
			  PEXReflectionNone);
   PEXSetBFReflectionModel (display, target, req_type,
			  PEXReflectionNone);

   refl_attrs.ambient = 0.0;
   refl_attrs.diffuse = 0.0;
   refl_attrs.specular = 0.0;
   refl_attrs.transmission = 0.0;

   PEXSetReflectionAttributes (display, target, req_type,
			       &refl_attrs);
   PEXSetBFReflectionAttributes (display, target, req_type,
			       &refl_attrs);

   /*
    * Use interior bundles now.
    */
   PEXSetIndividualASF (display, target, req_type, 
			PEXASFInteriorStyle, PEXBundled);
   PEXSetIndividualASF (display, target, req_type, 
			PEXASFSurfaceColor, PEXBundled);
   PEXSetIndividualASF (display, target, req_type, 
			PEXASFSurfaceInterp, PEXBundled);
   PEXSetIndividualASF (display, target, req_type, 
			PEXASFReflectionModel, PEXBundled);
   PEXSetIndividualASF (display, target, req_type, 
			PEXASFReflectionAttr, PEXBundled);

   PEXSetIndividualASF (display, target, req_type, 
			PEXASFBFInteriorStyle, PEXBundled);
   PEXSetIndividualASF (display, target, req_type, 
			PEXASFBFSurfaceColor, PEXBundled);
   PEXSetIndividualASF (display, target, req_type, 
			PEXASFBFSurfaceInterp, PEXBundled);
   PEXSetIndividualASF (display, target, req_type, 
			PEXASFBFReflectionModel, PEXBundled);
   PEXSetIndividualASF (display, target, req_type, 
			PEXASFBFReflectionAttr, PEXBundled);

   PEXSetInteriorBundleIndex (display, target, req_type,
			      FF_AMBIENT_ONLY);

   draw_pgon_3_1 (display, target, req_type, ff_pgon);

   PEXSetInteriorBundleIndex (display, target, req_type,
			      FF_DIFFUSE_ONLY);

   draw_pgon_3_2 (display, target, req_type, ff_pgon);

   PEXSetInteriorBundleIndex (display, target, req_type,
			      FF_AMBIENT_ONLY);

   draw_pgon_4_1 (display, target, req_type, bf_pgon);

   PEXSetInteriorBundleIndex (display, target, req_type,
			      FF_DIFFUSE_ONLY);

   draw_pgon_4_2 (display, target, req_type, bf_pgon);


   /*
    * Now draw with face distinquishing on
    */

   PEXSetFacetDistinguishFlag (display, target, req_type, True);
   PEXSetIndividualASF (display, target, req_type,
			PEXASFInteriorStyle, PEXIndividual);
   PEXSetIndividualASF (display, target, req_type,
			PEXASFSurfaceColor, PEXIndividual);
   PEXSetIndividualASF (display, target, req_type,
			PEXASFSurfaceInterp, PEXIndividual);
   PEXSetIndividualASF (display, target, req_type,
			PEXASFReflectionModel, PEXIndividual);
   PEXSetIndividualASF (display, target, req_type,
			PEXASFReflectionAttr, PEXIndividual);

   PEXSetIndividualASF (display, target, req_type,
			PEXASFBFInteriorStyle, PEXIndividual);
   PEXSetIndividualASF (display, target, req_type,
			PEXASFBFSurfaceColor, PEXIndividual);
   PEXSetIndividualASF (display, target, req_type,
			PEXASFBFSurfaceInterp, PEXIndividual);
   PEXSetIndividualASF (display, target, req_type,
			PEXASFBFReflectionModel, PEXIndividual);
   PEXSetIndividualASF (display, target, req_type,
			PEXASFBFReflectionAttr, PEXIndividual);

   PEXSetInteriorStyle (display, target, req_type,
			PEXInteriorStyleSolid);
   PEXSetBFInteriorStyle (display, target, req_type,
			  PEXInteriorStyleSolid);
   PEXSetReflectionModel (display, target, req_type,
			  PEXReflectionDiffuse);
   PEXSetBFReflectionModel (display, target, req_type,
			  PEXReflectionDiffuse);


   refl_attrs.ambient = 1.0;
   refl_attrs.diffuse = 0.0;

   PEXSetReflectionAttributes (display, target, req_type,
			       &refl_attrs);

   draw_pgon_1_3 (display, target, req_type, ff_pgon);

   refl_attrs.ambient = 0.0;
   refl_attrs.diffuse = 1.0;
   PEXSetReflectionAttributes (display, target, req_type,
			       &refl_attrs);

   draw_pgon_1_4 (display, target, req_type, ff_pgon);

   /*
    * BF Attrs should now be be used
    */
   refl_attrs.ambient = 1.0;
   refl_attrs.diffuse = 0.0;
   PEXSetBFReflectionAttributes (display, target, req_type,
			       &refl_attrs);

   /*
    * FF Attrs should not be looked at
    */
   refl_attrs.ambient = 0.0;
   refl_attrs.diffuse = 0.0;
   PEXSetReflectionAttributes (display, target, req_type,
			       &refl_attrs);

   draw_pgon_2_3 (display, target, req_type, bf_pgon);

   refl_attrs.ambient = 0.0;
   refl_attrs.diffuse = 1.0;
   PEXSetBFReflectionAttributes (display, target, req_type,
				 &refl_attrs);

   draw_pgon_2_4 (display, target, req_type, bf_pgon);

   /*
    * Jumble everything up before going to bundles
    */

   PEXSetInteriorStyle (display, target, req_type,
			PEXInteriorStyleHollow);
   PEXSetBFInteriorStyle (display, target, req_type,
			  PEXInteriorStyleHollow);
   PEXSetReflectionModel (display, target, req_type,
			  PEXReflectionNone);
   PEXSetBFReflectionModel (display, target, req_type,
			  PEXReflectionNone);

   refl_attrs.ambient = 0.0;
   refl_attrs.diffuse = 0.0;
   refl_attrs.specular = 0.0;
   refl_attrs.transmission = 0.0;

   PEXSetReflectionAttributes (display, target, req_type,
			       &refl_attrs);
   PEXSetBFReflectionAttributes (display, target, req_type,
			       &refl_attrs);

   /*
    * Use interior bundles now.
    */
   PEXSetIndividualASF (display, target, req_type,
			PEXASFInteriorStyle, PEXBundled);
   PEXSetIndividualASF (display, target, req_type,
			PEXASFSurfaceColor, PEXBundled);
   PEXSetIndividualASF (display, target, req_type,
			PEXASFSurfaceInterp, PEXBundled);
   PEXSetIndividualASF (display, target, req_type,
			PEXASFReflectionModel, PEXBundled);
   PEXSetIndividualASF (display, target, req_type,
			PEXASFReflectionAttr, PEXBundled);

   PEXSetIndividualASF (display, target, req_type,
			PEXASFBFInteriorStyle, PEXBundled);
   PEXSetIndividualASF (display, target, req_type,
			PEXASFBFSurfaceColor, PEXBundled);
   PEXSetIndividualASF (display, target, req_type,
			PEXASFBFSurfaceInterp, PEXBundled);
   PEXSetIndividualASF (display, target, req_type,
			PEXASFBFReflectionModel, PEXBundled);
   PEXSetIndividualASF (display, target, req_type,
			PEXASFBFReflectionAttr, PEXBundled);

   PEXSetInteriorBundleIndex (display, target, req_type,
			      FF_AMBIENT_ONLY);

   draw_pgon_3_3 (display, target, req_type, ff_pgon);

   PEXSetInteriorBundleIndex (display, target, req_type,
			      FF_DIFFUSE_ONLY);

   draw_pgon_3_4 (display, target, req_type, ff_pgon);

   PEXSetInteriorBundleIndex (display, target, req_type,
			      BF_AMBIENT_ONLY);

   draw_pgon_4_3 (display, target, req_type, bf_pgon);

   PEXSetInteriorBundleIndex (display, target, req_type,
			      BF_DIFFUSE_ONLY);

   draw_pgon_4_4 (display, target, req_type, bf_pgon);
   XSync (display,0);
}


Test_2 (display, target, req_type)
Display *display;
XID target;
PEXOCRequestType req_type;
{
    PEXReflectionAttributes refl_attrs;
    PEXTableIndex enable[NUM_AMBIENTS + NUM_DIRECTS + NUM_POINTS + NUM_SPOTS];
    PEXTableIndex disable[NUM_AMBIENTS + NUM_DIRECTS + NUM_POINTS + NUM_SPOTS];
    PEXColor color;

   /*
    * Select view
    */
   PEXSetViewIndex (display, target, req_type, 1);

   /*
    * Turn face distinquishing on
    */

   PEXSetFacetDistinguishFlag (display, target, req_type, True);

   /*
    * Setup lights
    */
   enable[0] = AMBIENT_RED;
   enable[1] = DIRECT_GREEN;
   enable[2] = DIRECT_BLUE;
   PEXSetLightSourceState (display, target, req_type,
			   3, enable, 0, NULL);


   /*
    * Select Individual interior attributes 
    */
   PEXSetInteriorStyle (display, target, req_type,
			PEXInteriorStyleSolid);
   PEXSetBFInteriorStyle (display, target, req_type,
			  PEXInteriorStyleSolid);

   PEXSetReflectionModel (display, target, req_type,
			  PEXReflectionDiffuse);
   PEXSetBFReflectionModel (display, target, req_type,
			  PEXReflectionDiffuse);
   color.rgb.red = 1.0;
   color.rgb.green = 1.0;
   color.rgb.blue = 1.0;

   PEXSetSurfaceColor (display, target, req_type, 
			PEXColorTypeRGB, &color);

   PEXSetBFSurfaceColor (display, target, req_type, 
			PEXColorTypeRGB, &color);

   refl_attrs.ambient = 0.0;
   refl_attrs.diffuse = 1.0;
   refl_attrs.specular_color.type  = PEXColorTypeRGB;
   refl_attrs.specular_color.value = color;
   PEXSetReflectionAttributes (display, target, req_type,
			       &refl_attrs);

   refl_attrs.ambient = 0.0;
   refl_attrs.diffuse = 1.0;
   PEXSetBFReflectionAttributes (display, target, req_type,
			       &refl_attrs);
   
   PEXSetInteriorBundleIndex (display, target, req_type,
			      FF_AMBIENT_ASF);

   /* Draw diffuse green FF poly */
   draw_pgon_1_1 (display, target, req_type, ff_pgon); 

   /* Draw diffuse green BF poly */
   draw_pgon_1_3 (display, target, req_type, bf_pgon); 

   /* Draw invisible ff poly  & bf poly*/
   PEXSetIndividualASF (display, target, req_type,
			PEXASFReflectionModel, PEXBundled);
   PEXSetIndividualASF (display, target, req_type,
			PEXASFBFReflectionModel, PEXBundled);

   draw_pgon_1_2 (display, target, req_type, ff_pgon); 
   draw_pgon_1_4 (display, target, req_type, bf_pgon);

   /* Draw red ff bf poly by taking refl attrs from bundle */
   PEXSetIndividualASF (display, target, req_type,
			PEXASFReflectionAttr, PEXBundled);
   PEXSetIndividualASF (display, target, req_type,
			PEXASFBFReflectionAttr, PEXBundled);

   draw_pgon_2_1 (display, target, req_type, ff_pgon); 
   draw_pgon_2_3 (display, target, req_type, bf_pgon);

   /* Toggle model back to Individual should get red poly because
    * model still includes ambient. But, attr (ambient only) hasn't
    * changed.
    */
   PEXSetIndividualASF (display, target, req_type,
			PEXASFReflectionModel, PEXIndividual);
   PEXSetIndividualASF (display, target, req_type,
			PEXASFBFReflectionModel, PEXIndividual);

   draw_pgon_2_2 (display, target, req_type, ff_pgon); 
   draw_pgon_2_4 (display, target, req_type, bf_pgon); 

   /* Toggle attr back, should get green ff poly bf poly */
   PEXSetIndividualASF (display, target, req_type,
			PEXASFReflectionAttr, PEXIndividual);
   PEXSetIndividualASF (display, target, req_type,
			PEXASFBFReflectionAttr, PEXIndividual);

   draw_pgon_3_1 (display, target, req_type, ff_pgon); 
   draw_pgon_3_3 (display, target, req_type, bf_pgon); 

   /* Bang on the ASFs again no change should occur */
   PEXSetIndividualASF (display, target, req_type,
			PEXASFReflectionAttr, PEXIndividual);
   PEXSetIndividualASF (display, target, req_type,
			PEXASFReflectionModel, PEXIndividual);
   PEXSetIndividualASF (display, target, req_type,
			PEXASFBFReflectionAttr, PEXIndividual);
   PEXSetIndividualASF (display, target, req_type,
			PEXASFBFReflectionModel, PEXIndividual);

   draw_pgon_3_2 (display, target, req_type, ff_pgon); 
   draw_pgon_3_4 (display, target, req_type, bf_pgon); 

   /* Bang on the ASFs as bundled */
   PEXSetIndividualASF (display, target, req_type,
			PEXASFReflectionAttr, PEXBundled);
   PEXSetIndividualASF (display, target, req_type,
			PEXASFReflectionModel, PEXBundled);
   PEXSetIndividualASF (display, target, req_type,
			PEXASFReflectionAttr, PEXBundled);
   PEXSetIndividualASF (display, target, req_type,
			PEXASFReflectionModel, PEXBundled);

   PEXSetIndividualASF (display, target, req_type,
			PEXASFBFReflectionAttr, PEXBundled);
   PEXSetIndividualASF (display, target, req_type,
			PEXASFBFReflectionModel, PEXBundled);
   PEXSetIndividualASF (display, target, req_type,
			PEXASFBFReflectionAttr, PEXBundled);
   PEXSetIndividualASF (display, target, req_type,
			PEXASFBFReflectionModel, PEXBundled);

   draw_pgon_4_1 (display, target, req_type, ff_pgon); 
   draw_pgon_4_3 (display, target, req_type, bf_pgon); 
}



Test_3 (display, target, req_type)
Display *display;
XID target;
PEXOCRequestType req_type;
{
    PEXReflectionAttributes refl_attrs;
    PEXTableIndex enable[NUM_AMBIENTS + NUM_DIRECTS + NUM_POINTS + NUM_SPOTS];
    PEXTableIndex disable[NUM_AMBIENTS + NUM_DIRECTS + NUM_POINTS + NUM_SPOTS];
    PEXVector location;

   /*
    * Select view
    */
   PEXSetViewIndex (display, target, req_type, 1);

   /*
    * Setup lights
    */
   enable[0] = AMBIENT_GRAY;
   enable[1] = DIRECT_SPEC;
   PEXSetLightSourceState (display, target, req_type,
			   2, enable, 0, disable);


   /*
    * Select Individual interior attributes
    */
   PEXSetInteriorStyle (display, target, req_type,
			PEXInteriorStyleSolid);

   PEXSetReflectionModel (display, target, req_type,
			  PEXReflectionSpecular);

   PEXSetBFReflectionModel (display, target, req_type,
			  PEXReflectionNone);

   refl_attrs.ambient = 1.0;
   refl_attrs.diffuse = 1.0;
   refl_attrs.specular = 1.0;
   refl_attrs.specular_conc = 10.0;
   refl_attrs.specular_color.type = PEXColorTypeRGB;
   refl_attrs.specular_color.value.rgb.red = 0.0;
   refl_attrs.specular_color.value.rgb.green = 1.0;
   refl_attrs.specular_color.value.rgb.blue = 0.0;

   PEXSetReflectionAttributes (display, target, req_type,
			       &refl_attrs);

   /*
    * Should get large green specular highlight 
    */
   location.x = -80.0; location.y = 80.0; location.z = 0.0;
   build_white_sphere (display, target, &location, req_type, NORMAL_NORMALS, PEXGANormal);

   refl_attrs.ambient = 1.0;
   refl_attrs.diffuse = 1.0;
   refl_attrs.specular = 1.0;
   refl_attrs.specular_conc = 100.0;
   refl_attrs.specular_color.type = PEXColorTypeIndexed;
   refl_attrs.specular_color.value.indexed.index = 6;
/*
   refl_attrs.specular_color.type = PEXColorTypeRGB;
   refl_attrs.specular_color.value.rgb.red = 0.0;
   refl_attrs.specular_color.value.rgb.green = 0.0;
   refl_attrs.specular_color.value.rgb.blue = 1.0;

*/
   PEXSetReflectionAttributes (display, target, req_type,
			       &refl_attrs);

   /*
    * Should get small blue specular highlight 
    */
   location.x = -30.0; location.y = 80.0; location.z = 0.0;
   build_white_sphere (display, target, &location, req_type, NORMAL_NORMALS, PEXGANormal);

   /*
    * Get specular from lut (should be wide and magenta )
    */
   PEXSetIndividualASF (display, target, req_type, 
			PEXASFReflectionAttr, PEXBundled);

   PEXSetInteriorBundleIndex (display, target, req_type, 
				BF_AMBIENT_ASF);
   
   location.x = -80.0; location.y = 30.0; location.z = 0.0;
   build_white_sphere (display, target, &location, req_type, NORMAL_NORMALS, PEXGANormal);

   /*
    * Toggle back to individual, should get small blue highlight
    */
   PEXSetIndividualASF (display, target, req_type, 
			PEXASFReflectionAttr, PEXIndividual);

   location.x = -30.0; location.y = 30.0; location.z = 0.0;
   build_white_sphere (display, target, &location, req_type, NORMAL_NORMALS, PEXGANormal);

   /*
    * Toggle back to individual again no change
    */
   PEXSetIndividualASF (display, target, req_type, 
			PEXASFReflectionAttr, PEXIndividual);

   location.x = -80.0; location.y = -30.0; location.z = 0.0;
   build_white_sphere (display, target, &location, req_type, NORMAL_NORMALS, PEXGANormal);

   /*
    * Toggle back to bundled, should get large magenta hightlight
    */
   PEXSetIndividualASF (display, target, req_type, 
			PEXASFReflectionAttr, PEXBundled);

   location.x = -30.0; location.y = -30.0; location.z = 0.0;
   build_white_sphere (display, target, &location, req_type, NORMAL_NORMALS, PEXGANormal);

   /*
    * Set ASF to bundled again, should be no change
    */
   PEXSetIndividualASF (display, target, req_type, 
			PEXASFReflectionAttr, PEXBundled);

   location.x = -80.0; location.y = -80.0; location.z = 0.0;
   build_white_sphere (display, target, &location, req_type, NORMAL_NORMALS, PEXGANormal);



   /*
    * Try steps above on backfacing quads 
    */


   PEXSetIndividualASF (display, target, req_type, 
			PEXASFBFReflectionAttr, PEXIndividual);

   PEXSetFacetDistinguishFlag (display, target, req_type, True);


   PEXSetBFInteriorStyle (display, target, req_type,
			PEXInteriorStyleSolid);

   PEXSetBFReflectionModel (display, target, req_type,
			  PEXReflectionSpecular);

   refl_attrs.ambient = 1.0;
   refl_attrs.diffuse = 1.0;
   refl_attrs.specular = 1.0;
   refl_attrs.specular_conc = 10.0;
   refl_attrs.specular_color.type = PEXColorTypeRGB;
   refl_attrs.specular_color.value.rgb.red = 1.0;
   refl_attrs.specular_color.value.rgb.green = 1.0;
   refl_attrs.specular_color.value.rgb.blue = 0.0;

   PEXSetBFReflectionAttributes (display, target, req_type,
			       &refl_attrs);

   /*
    * Should get large yellow specular highlight 
    */
   location.x = 30.0; location.y = 80.0; location.z = 0.0;
   build_white_sphere (display, target, &location, req_type, INVERTED_NORMALS, PEXGANormal);

   refl_attrs.ambient = 1.0;
   refl_attrs.diffuse = 1.0;
   refl_attrs.specular = 1.0;
   refl_attrs.specular_conc = 100.0;
   refl_attrs.specular_color.type = PEXColorTypeIndexed;
   refl_attrs.specular_color.value.indexed.index = 2;

   PEXSetBFReflectionAttributes (display, target, req_type,
			       &refl_attrs);

   /*
    * Should get small red specular highlight 
    */
   location.x = 80.0; location.y = 80.0; location.z = 0.0;
   build_white_sphere (display, target, &location, req_type, INVERTED_NORMALS, PEXGANormal);

   /*
    * Get specular from lut (should be wide and magenta )
    */
   PEXSetIndividualASF (display, target, req_type, 
			PEXASFBFReflectionAttr, PEXBundled);

   PEXSetInteriorBundleIndex (display, target, req_type, 
				BF_AMBIENT_ASF);
   
   location.x = 30.0; location.y = 30.0; location.z = 0.0;
   build_white_sphere (display, target, &location, req_type, INVERTED_NORMALS, PEXGANormal);

   /*
    * Toggle back to individual, should get small red highlight
    */
   PEXSetIndividualASF (display, target, req_type, 
			PEXASFBFReflectionAttr, PEXIndividual);

   location.x = 80.0; location.y = 30.0; location.z = 0.0;
   build_white_sphere (display, target, &location, req_type, INVERTED_NORMALS, PEXGANormal);

   /*
    * Toggle back to individual again no change
    */
   PEXSetIndividualASF (display, target, req_type, 
			PEXASFBFReflectionAttr, PEXIndividual);

   location.x = 30.0; location.y = -30.0; location.z = 0.0;
   build_white_sphere (display, target, &location, req_type, INVERTED_NORMALS, PEXGANormal);

   /*
    * Toggle back to bundled, should get large magenta hightlight
    */
   PEXSetIndividualASF (display, target, req_type, 
			PEXASFBFReflectionAttr, PEXBundled);

   location.x = 80.0; location.y = -30.0; location.z = 0.0;
   build_white_sphere (display, target, &location, req_type, INVERTED_NORMALS, PEXGANormal);

   /*
    * Set ASF to bundled again, should be no change
    */
   PEXSetIndividualASF (display, target, req_type, 
			PEXASFBFReflectionAttr, PEXBundled);

   location.x = 30.0; location.y = -80.0; location.z = 0.0;
   build_white_sphere (display, target, &location, req_type, INVERTED_NORMALS, PEXGANormal);
}


Test_4 (display, target, req_type)
Display *display;
XID target;
PEXOCRequestType req_type;
{
    PEXReflectionAttributes refl_attrs;
    PEXTableIndex enable[NUM_AMBIENTS + NUM_DIRECTS + NUM_POINTS + NUM_SPOTS];
    PEXTableIndex disable[NUM_AMBIENTS + NUM_DIRECTS + NUM_POINTS + NUM_SPOTS];
    PEXVector location;

   /*
    * Select view
    */
   PEXSetViewIndex (display, target, req_type, 1);

   /*
    * Setup lights
    */
   enable[0] = AMBIENT_GRAY;
   enable[1] = DIRECT_MODEL;
   PEXSetLightSourceState (display, target, req_type,
			   2, enable, 0, disable);


   /*
    * Setup to draw ambient
    */
   PEXSetInteriorStyle (display, target, req_type,
			PEXInteriorStyleSolid);

   PEXSetReflectionModel (display, target, req_type,
			  PEXReflectionAmbient);

   refl_attrs.ambient = 1.0;
   refl_attrs.diffuse = 1.0;
   refl_attrs.specular = 1.0;
   refl_attrs.specular_conc = 10.0;
   refl_attrs.specular_color.type = PEXColorTypeRGB;
   refl_attrs.specular_color.value.rgb.red = 1.0;
   refl_attrs.specular_color.value.rgb.green = 1.0;
   refl_attrs.specular_color.value.rgb.blue = 1.0;

   PEXSetReflectionAttributes (display, target, req_type,
			       &refl_attrs);
   location.x = -80.0; location.y = 80.0; location.z = 0.0;
   build_sphere (display, target, &location, req_type,  NORMAL_NORMALS, PEXGANormal);

   /* 
    * Draw diffuse
    */
   PEXSetReflectionModel (display, target, req_type,
			  PEXReflectionDiffuse);
   location.x = -80.0; location.y = 30.0; location.z = 0.0;
   build_sphere (display, target, &location, req_type,  NORMAL_NORMALS, PEXGANormal);

   /*
    * Draw specular
    */
   PEXSetReflectionModel (display, target, req_type,
			  PEXReflectionSpecular);
   location.x = -80.0; location.y = -30.0; location.z = 0.0;
   build_sphere (display, target, &location, req_type,  NORMAL_NORMALS, PEXGANormal);

   /* 
    * Use model from luts 
    */

   PEXSetInteriorBundleIndex (display, target, req_type, 
			      FF_AMBIENT_ONLY);
   
   PEXSetIndividualASF (display, target, req_type, 
			PEXASFReflectionModel, PEXBundled);

   /*
    * Ambient
    */
   location.x = -30.0; location.y = 80.0; location.z = 0.0;
   build_sphere (display, target, &location, req_type,  NORMAL_NORMALS, PEXGANormal);

   /*
    * Diffuse
    */
   PEXSetInteriorBundleIndex (display, target, req_type, 
			      FF_DIFFUSE_ONLY);

   location.x = -30.0; location.y = 30.0; location.z = 0.0;
   build_sphere (display, target, &location, req_type,  NORMAL_NORMALS, PEXGANormal);

   /*
    * Specular
    */
   PEXSetInteriorBundleIndex (display, target, req_type, 
			      BF_AMBIENT_ASF);

   location.x = -30.0; location.y = -30.0; location.z = 0.0;
   build_sphere (display, target, &location, req_type,  NORMAL_NORMALS, PEXGANormal);



   /*
    * Draw backfacing gray spheres
    */
   PEXSetFacetDistinguishFlag (display, target, req_type, True);

   /*
    * Setup to draw ambient
    */
   PEXSetBFInteriorStyle (display, target, req_type,
			PEXInteriorStyleSolid);

   PEXSetBFReflectionModel (display, target, req_type,
			  PEXReflectionAmbient);

   refl_attrs.ambient = 1.0;
   refl_attrs.diffuse = 1.0;
   refl_attrs.specular = 1.0;
   refl_attrs.specular_conc = 10.0;
   refl_attrs.specular_color.type = PEXColorTypeRGB;
   refl_attrs.specular_color.value.rgb.red = 1.0;
   refl_attrs.specular_color.value.rgb.green = 1.0;
   refl_attrs.specular_color.value.rgb.blue = 1.0;

   PEXSetBFReflectionAttributes (display, target, req_type,
			       &refl_attrs);
   location.x = 30.0; location.y = 80.0; location.z = 0.0;
   build_sphere (display, target, &location, req_type,  INVERTED_NORMALS, PEXGANormal);

   /*
    * Draw diffuse
    */
   PEXSetBFReflectionModel (display, target, req_type,
			  PEXReflectionDiffuse);
   location.x = 30.0; location.y = 30.0; location.z = 0.0;
   build_sphere (display, target, &location, req_type,  INVERTED_NORMALS, PEXGANormal);

   /*
    * Draw specular
    */
   PEXSetBFReflectionModel (display, target, req_type,
			  PEXReflectionSpecular);
   location.x = 30.0; location.y = -30.0; location.z = 0.0;
   build_sphere (display, target, &location, req_type,  INVERTED_NORMALS, PEXGANormal);

   /*
    * Use model from luts
    */

   PEXSetInteriorBundleIndex (display, target, req_type,
			      BF_AMBIENT_ONLY);

   PEXSetIndividualASF (display, target, req_type,
			PEXASFBFReflectionModel, PEXBundled);

   /*
    * Ambient
    */
   location.x = 80.0; location.y = 80.0; location.z = 0.0;
   build_sphere (display, target, &location, req_type,  INVERTED_NORMALS, PEXGANormal);

   /*
    * Diffuse
    */
   PEXSetInteriorBundleIndex (display, target, req_type,
			      BF_DIFFUSE_ONLY);

   location.x = 80.0; location.y = 30.0; location.z = 0.0;
   build_sphere (display, target, &location, req_type,  INVERTED_NORMALS, PEXGANormal);

   /*
    * Specular
    */
   PEXSetInteriorBundleIndex (display, target, req_type,
			      BF_AMBIENT_ASF);

   location.x = 80.0; location.y = -30.0; location.z = 0.0;
   build_sphere (display, target, &location, req_type,  INVERTED_NORMALS, PEXGANormal);
}


Test_5 (display, target, req_type)
Display *display;
XID target;
PEXOCRequestType req_type;
{
    PEXReflectionAttributes refl_attrs;
    PEXTableIndex enable[NUM_AMBIENTS + NUM_DIRECTS + NUM_POINTS + NUM_SPOTS];
    PEXTableIndex disable[NUM_AMBIENTS + NUM_DIRECTS + NUM_POINTS + NUM_SPOTS];
    PEXColor color;

   /*
    * Select view
    */
   PEXSetViewIndex (display, target, req_type, 1);

   /*
    * Setup lights
    */
   enable[0] = AMBIENT_GRAY;
   enable[1] = DIRECT_MODEL;
   PEXSetLightSourceState (display, target, req_type,
			   2, enable, 0, disable);

   /*
    * Setup to draw ambient
    */
   PEXSetInteriorStyle (display, target, req_type,
			PEXInteriorStyleSolid);

   PEXSetReflectionModel (display, target, req_type,
			  PEXReflectionDiffuse);
   color.rgb.red = 0.0;
   color.rgb.green = 1.0;
   color.rgb.blue = 0.0;

   PEXSetSurfaceColor (display, target, req_type,
			PEXColorTypeRGB, &color);

   /*
    * Completely opaque
    */
   refl_attrs.ambient = 1.0;
   refl_attrs.diffuse = 1.0;
   refl_attrs.specular = 1.0;
   refl_attrs.specular_conc = 10.0;
   refl_attrs.specular_color.type = PEXColorTypeRGB;
   refl_attrs.specular_color.value.rgb.red = 1.0;
   refl_attrs.specular_color.value.rgb.green = 1.0;
   refl_attrs.specular_color.value.rgb.blue = 1.0;
   refl_attrs.transmission = 0.0;

   PEXSetReflectionAttributes (display, target, req_type,
			       &refl_attrs);

   draw_pgon_1_1 (display, target, req_type, ff_pgon);

   /*
    * Halfway opaque
    */
   refl_attrs.transmission = 0.5;

   PEXSetReflectionAttributes (display, target, req_type,
			       &refl_attrs);
   draw_pgon_2_1 (display, target, req_type, ff_pgon);

   /*
    * Mostly transparent
    */
   refl_attrs.transmission = 0.9;

   PEXSetReflectionAttributes (display, target, req_type,
			       &refl_attrs);

   draw_pgon_3_1 (display, target, req_type, ff_pgon);

   /*
    * Pull transmission from LUT
    */

   /*
    * Draw opaque
    */
   PEXSetInteriorBundleIndex (display, target, req_type,
			      OPAQUE);

   PEXSetIndividualASF (display, target, req_type,
			PEXASFReflectionAttr, PEXBundled);

   draw_pgon_1_2 (display, target, req_type, ff_pgon);

   /*
    * Draw semi-opaque
    */
   PEXSetInteriorBundleIndex (display, target, req_type,
			      SEMI_OPAQUE);

   draw_pgon_2_2 (display, target, req_type, ff_pgon);

   /*
    * Draw semi-opaque
    */
   PEXSetInteriorBundleIndex (display, target, req_type,
			      TRANSPARENT);

   draw_pgon_3_2 (display, target, req_type, ff_pgon);


   /*
    * Do backfaces like we did front faces
    */

   PEXSetFacetDistinguishFlag (display, target, req_type, True);

   /*
    * Setup to draw ambient
    */
   PEXSetBFInteriorStyle (display, target, req_type,
			PEXInteriorStyleSolid);

   PEXSetBFReflectionModel (display, target, req_type,
			  PEXReflectionDiffuse);
   color.rgb.red = 1.0;
   color.rgb.green = 0.0;
   color.rgb.blue = 0.0;

   PEXSetBFSurfaceColor (display, target, req_type,
			PEXColorTypeRGB, &color);

   /*
    * Completely opaque
    */
   refl_attrs.ambient = 1.0;
   refl_attrs.diffuse = 1.0;
   refl_attrs.specular = 1.0;
   refl_attrs.specular_conc = 10.0;
   refl_attrs.specular_color.type = PEXColorTypeRGB;
   refl_attrs.specular_color.value.rgb.red = 1.0;
   refl_attrs.specular_color.value.rgb.green = 1.0;
   refl_attrs.specular_color.value.rgb.blue = 1.0;
   refl_attrs.transmission = 0.0;

   PEXSetBFReflectionAttributes (display, target, req_type,
			       &refl_attrs);

   draw_pgon_1_3 (display, target, req_type, bf_pgon);

   /*
    * Halfway opaque
    */
   refl_attrs.transmission = 0.5;

   PEXSetBFReflectionAttributes (display, target, req_type,
			       &refl_attrs);
   draw_pgon_2_3 (display, target, req_type, bf_pgon);

   /*
    * Mostly transparent
    */
   refl_attrs.transmission = 0.9;

   PEXSetBFReflectionAttributes (display, target, req_type,
			       &refl_attrs);

   draw_pgon_3_3 (display, target, req_type, bf_pgon);

   /*
    * Pull transmission from LUT
    */

   /*
    * Draw opaque
    */
   PEXSetInteriorBundleIndex (display, target, req_type,
			      OPAQUE);

   PEXSetIndividualASF (display, target, req_type,
			PEXASFBFReflectionAttr, PEXBundled);

   draw_pgon_1_4 (display, target, req_type, bf_pgon);

   /*
    * Draw semi-opaque
    */
   PEXSetInteriorBundleIndex (display, target, req_type,
			      SEMI_OPAQUE);

   draw_pgon_2_4 (display, target, req_type, bf_pgon);

   /*
    * Draw semi-opaque
    */
   PEXSetInteriorBundleIndex (display, target, req_type,
			      TRANSPARENT);

   draw_pgon_3_4 (display, target, req_type, bf_pgon);
}



Test_6 (display, target, req_type)
Display *display;
XID target;
PEXOCRequestType req_type;
{
    PEXReflectionAttributes refl_attrs;
    PEXTableIndex enable[NUM_AMBIENTS + NUM_DIRECTS + NUM_POINTS + NUM_SPOTS];
    PEXTableIndex disable[NUM_AMBIENTS + NUM_DIRECTS + NUM_POINTS + NUM_SPOTS];
    PEXVector location;

   /*
    * Select view
    */
   PEXSetViewIndex (display, target, req_type, 1);

   /*
    * Setup lights
    */
   enable[0] = AMBIENT_GRAY;
   enable[1] = DIRECT_MODEL;
   PEXSetLightSourceState (display, target, req_type,
			   2, enable, 0, disable);

   /*
    * Setup attributes
    */
   PEXSetInteriorStyle (display, target, req_type,
			PEXInteriorStyleSolid);

   PEXSetReflectionModel (display, target, req_type,
			  PEXReflectionSpecular);

   refl_attrs.ambient = 1.0;
   refl_attrs.diffuse = 1.0;
   refl_attrs.specular = 1.0;
   refl_attrs.specular_conc = 10.0;
   refl_attrs.specular_color.type = PEXColorTypeRGB;
   refl_attrs.specular_color.value.rgb.red = 1.0;
   refl_attrs.specular_color.value.rgb.green = 1.0;
   refl_attrs.specular_color.value.rgb.blue = 1.0;
   refl_attrs.transmission = 0.0;

   /*
    * Surface Interp None
    */
   PEXSetReflectionAttributes (display, target, req_type,
			       &refl_attrs);

   PEXSetSurfaceInterpMethod (display, target, req_type,
			      PEXSurfaceInterpNone);

   location.x = -80.0; location.y = 80.0; location.z = 0.0;
   build_sphere (display, target, &location, req_type, NORMAL_NORMALS, PEXGANormal);

   /*
    * Surface Interp Color
    */
   PEXSetSurfaceInterpMethod (display, target, req_type,
			      PEXSurfaceInterpColor);

   location.x = -30.0; location.y = 80.0; location.z = 0.0;
   build_sphere (display, target, &location, req_type, NORMAL_NORMALS, PEXGANormal);

   /*
    * Now load from lut use opaque entry
    */
   PEXSetInteriorBundleIndex (display, target, req_type, OPAQUE);

   PEXSetIndividualASF (display, target, req_type,
			PEXASFSurfaceInterp, PEXBundled);

   location.x = -80.0; location.y = 30.0; location.z = 0.0;
   build_sphere (display, target, &location, req_type, NORMAL_NORMALS, PEXGANormal);

   PEXSetIndividualASF (display, target, req_type,
			PEXASFSurfaceInterp, PEXIndividual);

   location.x = -30.0; location.y = 30.0; location.z = 0.0;
   build_sphere (display, target, &location, req_type, NORMAL_NORMALS, PEXGANormal);


   /*
    * Now try back facing interpolation
    */

   PEXSetFacetDistinguishFlag (display, target, req_type, True);

   /*
    * Setup attributes
    */
   PEXSetBFInteriorStyle (display, target, req_type,
			PEXInteriorStyleSolid);

   PEXSetBFReflectionModel (display, target, req_type,
			  PEXReflectionSpecular);

   refl_attrs.ambient = 1.0;
   refl_attrs.diffuse = 1.0;
   refl_attrs.specular = 1.0;
   refl_attrs.specular_conc = 10.0;
   refl_attrs.specular_color.type = PEXColorTypeRGB;
   refl_attrs.specular_color.value.rgb.red = 1.0;
   refl_attrs.specular_color.value.rgb.green = 1.0;
   refl_attrs.transmission = 0.0;

   /*
    * Surface Interp None
    */
   PEXSetBFReflectionAttributes (display, target, req_type,
			       &refl_attrs);

   PEXSetBFSurfaceInterpMethod (display, target, req_type,
			      PEXSurfaceInterpNone);

   location.x = 30.0; location.y = 80.0; location.z = 0.0;
   build_sphere (display, target, &location, req_type, INVERTED_NORMALS, PEXGANormal);

   /*
    * Surface Interp Color
    */
   PEXSetBFSurfaceInterpMethod (display, target, req_type,
			      PEXSurfaceInterpColor);

   location.x = 80.0; location.y = 80.0; location.z = 0.0;
   build_sphere (display, target, &location, req_type, INVERTED_NORMALS, PEXGANormal);

   /*
    * Now load from lut use opaque entry
    */
   PEXSetInteriorBundleIndex (display, target, req_type, OPAQUE);

   PEXSetIndividualASF (display, target, req_type,
			PEXASFBFSurfaceInterp, PEXBundled);

   location.x = 30.0; location.y = 30.0; location.z = 0.0;
   build_sphere (display, target, &location, req_type, INVERTED_NORMALS, PEXGANormal);

   PEXSetIndividualASF (display, target, req_type,
			PEXASFBFSurfaceInterp, PEXIndividual);

   location.x = 80.0; location.y = 30.0; location.z = 0.0;
   build_sphere (display, target, &location, req_type, INVERTED_NORMALS, PEXGANormal);

}


Test_7 (display, target, req_type)
Display *display;
XID target;
PEXOCRequestType req_type;
{
    PEXReflectionAttributes refl_attrs;
    PEXTableIndex enable[NUM_AMBIENTS + NUM_DIRECTS + NUM_POINTS + NUM_SPOTS];
    PEXTableIndex disable[NUM_AMBIENTS + NUM_DIRECTS + NUM_POINTS + NUM_SPOTS];
    PEXVector location;

   /*
    * Select view
    */
   PEXSetViewIndex (display, target, req_type, 1);

   /*
    * Setup lights
    */
   enable[0] = AMBIENT_GRAY;
   enable[1] = DIRECT_MODEL;
   PEXSetLightSourceState (display, target, req_type,
			   2, enable, 0, disable);

   /*
    * Setup attributes
    */
   PEXSetFacetDistinguishFlag (display, target, req_type, True);

   PEXSetInteriorStyle (display, target, req_type,
			PEXInteriorStyleSolid);

   PEXSetBFInteriorStyle (display, target, req_type,
			PEXInteriorStyleSolid);

   /*
    * Front
    */
   location.x = -80.0; location.y = 80.0; location.z = 0.0;
   build_sphere (display, target, &location, req_type, NORMAL_NORMALS, PEXGANormal);


   /*
    * Back
    */
   location.x = -30.0; location.y = 80.0; location.z = 0.0;
   build_sphere (display, target, &location, req_type, INVERTED_NORMALS, PEXGANormal);
}	   

Test_8 (display, target, req_type)
Display *display;
XID target;
PEXOCRequestType req_type;
{
    PEXReflectionAttributes refl_attrs;
    PEXTableIndex enable[NUM_AMBIENTS + NUM_DIRECTS + NUM_POINTS + NUM_SPOTS];
    PEXTableIndex disable[NUM_AMBIENTS + NUM_DIRECTS + NUM_POINTS + NUM_SPOTS];
    PEXVector location;

   /*
    * Select view
    */
   PEXSetViewIndex (display, target, req_type, 1);

   /*
    * Setup lights
    */
   enable[0] = AMBIENT_GRAY;
   enable[1] = DIRECT_MODEL;
   PEXSetLightSourceState (display, target, req_type,
			   2, enable, 0, disable);

   /*
    * Setup attributes
    */
   PEXSetFacetDistinguishFlag (display, target, req_type, True);

   PEXSetInteriorStyle (display, target, req_type,
			PEXInteriorStyleSolid);

   PEXSetBFInteriorStyle (display, target, req_type,
			PEXInteriorStyleSolid);

   /*
    * Cull Back faces - result should be no change - red sphere.
    */

   PEXSetFacetCullingMode( display, target, req_type, PEXBackFaces );
   location.x = -80.0; location.y = 80.0; location.z = 0.0;
   build_sphere (display, target, &location, req_type, NORMAL_NORMALS, PEXGANone);

   /*
    * Cull Front faces - result should be nothing drawn.
    */

   PEXSetFacetCullingMode( display, target, req_type, PEXFrontFaces );
   location.x = -30.0; location.y = 80.0; location.z = 0.0;
   build_sphere (display, target, &location, req_type, NORMAL_NORMALS, PEXGANone);
}	   

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "refl_attrs");
    *num_images      = 8;
    *supported_modes = IMM_MODE | STR_MODE | WKS_MODE;

    strcpy(win_title, "Reflection Attrs");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    XSynchronize(dpy, True);

    switch (cur_image)
	{
	case 1:
	    {
	    int item_mask=0;
	    PEXRendererAttributes rndAttrs;

	    item_mask |= PEXRAClearImage;
            rndAttrs.clear_image = True;

	    item_mask |= PEXRAClearZ;
            rndAttrs.clear_z = True;

	    item_mask |= PEXRAHLHSRMode;
            rndAttrs.hlhsr_mode = PEXHLHSRZBuffer;
            
	    PEXChangeRenderer (dpy, renderer, item_mask, &rndAttrs);
	    /*
             * Define views
             */
            define_views (dpy, viewLUT);
    
            /*
             * Define the interior attributes
             */
	    define_interiors (dpy, interiorLUT);
    
	    /*
             * Define all the light sources to be used
             */
            define_lights (dpy, lightLUT);
	    break;
	    }

	case 7:
	    {
	    PEXPipelineContext plc;
	    PEXRendererAttributes rndAttrs;
	    unsigned long plc_mask[3] = {0.0, 0.0, 0.0};
	    int item_mask = 0;

            item_mask |= PEXRAPipelineContext;
            plc = PEXCreatePipelineContext (dpy, plc_mask, NULL);
            rndAttrs.pipeline_context = plc;

	    PEXChangeRenderer (dpy, renderer, item_mask, &rndAttrs);
	    break;
	    }
	}
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    switch (cur_image)
	{
        case 1: 
	    {
	    /******************************************************************
             * Test 1
             * 
             * Ambient & Diffuse attrs
             *****************************************************************/

	    describe_test("***************************************\n");
	    describe_test(" Test 1       Ambient & Diffuse attrs\n");
	    describe_test("***************************************\n\n");

	    Test_1 (dpy, resourceID, req_type);

	    describe_test("All squares in left column should be red\n");
	    describe_test(
		    "All squares in 2nd column & rows 1 & 3 should be green\n");
	    describe_test(
		    "All squares in 2nd column & rows 2 & 4 should be green\n");
	    describe_test("All squares in 3rd column should be red\n");
	    describe_test("All squares in 4th column should be green\n");
	    break;
	    }

        case 2: 
	    {
            /******************************************************************
             * Test 2
             *
             * Ambient & Diffuse attrs via asfs
             *****************************************************************/
    
            describe_test("***************************************\n");
            describe_test(" Test 2       Ambient & Diffuse via asfs\n");
            describe_test("***************************************\n\n");
    
	        Test_2 (dpy, resourceID, req_type);
    
	    describe_test("Row 1: green, blank, green, blank\n");
	    describe_test("Row 2: red, red, red, red\n");
	    describe_test("Row 3: green, green, green, green\n");
	    describe_test("Row 4: red, blank, red, blank\n");
	    break;
	    }
    
        case 3: 
	    {
            /******************************************************************
             * Test 3
             *
             * Specular
             *****************************************************************/
    
            describe_test("***************************************\n");
            describe_test(" Test 3       Specular\n");
            describe_test("***************************************\n\n");
    
	        Test_3(dpy, resourceID, req_type);
    
            describe_test("Spheres should all be gray\n");
            describe_test("Specular highlights should be as follows:\n");
            describe_test(
		"Row 1: large green, small blue, large yellow, small red\n");
            describe_test(
	        "Row 2: large magenta, small blue, large magenta, small red\n");
            describe_test(
	        "Row 3: small blue, large magenta, small red, large magenta\n");
            describe_test(
		"Row 4: large magenta, blank, large magenta, blank\n");
	    break;
	    }

        case 4: 
	    {
            /******************************************************************
             * Test 4
             *
             * Reflection model
             *****************************************************************/
    
            describe_test("***************************************\n");
            describe_test(" Test 4       Reflection Model\n");
            describe_test("***************************************\n\n");
    
	        Test_4(dpy, resourceID, req_type);
    
	    describe_test("First 2 columns should have reddish spheres\n");
	    describe_test("Last 2 columns should have blueish spheres\n");
	    describe_test("Each column should 3 spheres that are shaded\n");
	    describe_test("with ambient, diffuse, and specular models\n");
	    describe_test("respectively.\n");
	    break;
	    }
    
        case 5: 
	    {
            /******************************************************************
             * Test 5
             *
             * Transmission
             *****************************************************************/
    
            describe_test("***************************************\n");
            describe_test(" Test 5       Transmission\n");
            describe_test("***************************************\n\n");
    
	        Test_5(dpy, resourceID, req_type);
    
            describe_test("First 2 columns should be green\n");
            describe_test("Last 2 columns should be red\n");
            describe_test("Row 1: Opaque rectangles\n");
            describe_test("Row 2: Semi-opaque rectangles\n");
            describe_test("Row 3: Mostly-transparent rectangles\n");
	    break;
	    }
    

        case 6: 
	    {
            /******************************************************************
             * Test 6
             *
             * Surface Interpolation
             *****************************************************************/
    
            describe_test("***************************************\n");
            describe_test(" Test 6       Surface Interpolation \n");
            describe_test("***************************************\n\n");
    
	        Test_6(dpy, resourceID, req_type);
    
	    describe_test("4 spheres on left should be red\n");
	    describe_test("4 spheres on right should be blue\n");
	    describe_test(
		     "1st and 3rd columns should have faceted spheres\n");
	    describe_test(
		     "2nd and 4th columns should have smooth spheres\n");
	    break;
	    }

        case 7: 
	    {
            /******************************************************************
             * Test 7
             *
             * Default pipeline context
             *****************************************************************/
    
            describe_test("***************************************\n");
            describe_test(" Test 7       Default Pipeline Context \n");
            describe_test("***************************************\n\n");
    
	        Test_7(dpy, resourceID, req_type);
    
	    describe_test("Should see a solid red and a solid blue circle\n");
	    describe_test("Defaults are basically no shading\n");
	    break;
	    }
    
        case 8: 
	    {
            /******************************************************************
             * Test 8
             *
             * FF and BF Culling on QuadMesh
             *****************************************************************/
	    describe_test("***************************************\n");
	    describe_test(" Test 8       Quad Mesh Culling\n");
            describe_test("***************************************\n\n");

	    Test_8(dpy, resourceID, req_type);

            describe_test("Should see one solid red circle\n");
            describe_test("Should see blue ring due to culling\n");
	    break;
	    }
	}
    }


void testproc(Display *dpy, Window window, int cur_image)
    {
    char image_name[80], test_name[80];

    sprintf(image_name,"refl_attrs_%d", cur_image);
    sprintf(test_name, "Reflection Attributes #%d", cur_image);
     image_testproc(image_name, test_name, img_desc,
		      dpy,window, 0, 0, 
		      glob_window_width, glob_window_height);
    }
