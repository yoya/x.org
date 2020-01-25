/* $XConsortium: lut_lights.c,v 1.0 93/11/22 12:28:53 rws Exp $ */

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
 * lut_lights
 *
 * This is a test for lights. This test defines a group of lights of various
 * types and enables/disables them to see if lighting is working correctly.
 * The test starts with ambient lights, then directional lights, then point
 * lighta. And finally, spot lights are tested. The test basically consists
 * of 16 Fill Areas that are lit by various combinations of lights forming a
 * number of frames.
 *
 * Ken Tidwell 	Sept. 9, 1992
 */

/*
 * Basic set of include files for tests
 */
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/PEX5/PEXlib.h>
#include <math.h>
#include <misc.h>
#include <lut.h>

#define AMBIENT_BASE 1
#define	NUM_AMBIENTS 4
#define AMBIENT_GRAY  AMBIENT_BASE
#define AMBIENT_RED   AMBIENT_BASE + 1
#define AMBIENT_GREEN AMBIENT_BASE + 2 
#define AMBIENT_BLUE  AMBIENT_BASE + 3

#define DIRECT_BASE 10
#define NUM_DIRECTS 4
#define DIRECT_GRAY  DIRECT_BASE
#define DIRECT_RED   DIRECT_BASE + 1
#define DIRECT_GREEN DIRECT_BASE + 2
#define DIRECT_BLUE  DIRECT_BASE + 3

#define POINT_BASE  20
#define NUM_POINTS  5
#define POINT_1_GRAY    POINT_BASE
#define POINT_2_RED     POINT_BASE + 1 
#define POINT_3_GREEN   POINT_BASE + 2
#define POINT_4_BLUE    POINT_BASE + 3
#define POINT_5_MAGENTA POINT_BASE + 4

#define SPOT_BASE   30
#define NUM_SPOTS   17
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
   lights[1].color.value.rgb.red = 0.3;
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
   lights[2].color.value.rgb.green = 0.3;
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

   PEXSetTableEntries (display, table, SPOT_BASE, NUM_SPOTS, PEXLUTLight,
		       lights);
}


/*
 * Draw_pgons
 */
void draw_pgons(display, resourceID, req_type)
Display         *display;
XID              resourceID;
PEXOCRequestType req_type;
{
    PEXCoord corners[4];
    PEXMatrix xform;
    PEXVector translate;

    corners[0].x = -20.0; corners[0].y = -20.0; corners[0].z = 0.0;
    corners[1].x = 20.0; corners[1].y = -20.0; corners[1].z = 0.0;
    corners[2].x = 20.0; corners[2].y = 20.0; corners[2].z = 0.0;
    corners[3].x = -20.0; corners[3].y = 20.0; corners[3].z = 0.0;

    /*
     * Upper left quadrant (1)
     */

    /* Poly 1 (1,1) */
    translate.x = -80.0; translate.y = 80.0; translate.z = 0.0;
    PEXTranslate (&translate, xform);
    PEXSetLocalTransform (display, resourceID, req_type, PEXReplace, xform);
    PEXFillArea (display, resourceID, req_type, PEXShapeUnknown, True, 4, corners);

    /* Poly 2 (1,2) */
    translate.x = -30.0; translate.y = 80.0; translate.z = 0.0;
    PEXTranslate (&translate, xform);
    PEXSetLocalTransform (display, resourceID, req_type, PEXReplace, xform);
    PEXFillArea (display, resourceID, req_type, PEXShapeUnknown, True, 4, corners);

    /* Poly 3 (2,1) */
    translate.x = -80.0; translate.y = 30.0; translate.z = 0.0;
    PEXTranslate (&translate, xform);
    PEXSetLocalTransform (display, resourceID, req_type, PEXReplace, xform);
    PEXFillArea (display, resourceID, req_type, PEXShapeUnknown, True, 4, corners);

    /* Poly 4 (2,2) */
    translate.x = -30.0; translate.y = 30.0; translate.z = 0.0;
    PEXTranslate (&translate, xform);
    PEXSetLocalTransform (display, resourceID, req_type, PEXReplace, xform);
    PEXFillArea (display, resourceID, req_type, PEXShapeUnknown, True, 4, corners);


    /*
     * Upper right quadrant (2)
     */

    /* Poly 5 (1,3) */
    translate.x = 30.0; translate.y = 80.0; translate.z = 0.0;
    PEXTranslate (&translate, xform);
    PEXSetLocalTransform (display, resourceID, req_type, PEXReplace, xform);
    PEXFillArea (display, resourceID, req_type, PEXShapeUnknown, True, 4, corners);

    /* Poly 6 (1,4) */
    translate.x = 80.0; translate.y = 80.0; translate.z = 0.0;
    PEXTranslate (&translate, xform);
    PEXSetLocalTransform (display, resourceID, req_type, PEXReplace, xform);
    PEXFillArea (display, resourceID, req_type, PEXShapeUnknown, True, 4, corners);

    /* Poly 7 (2,3) */
    translate.x = 30.0; translate.y = 30.0; translate.z = 0.0;
    PEXTranslate (&translate, xform);
    PEXSetLocalTransform (display, resourceID, req_type, PEXReplace, xform);
    PEXFillArea (display, resourceID, req_type, PEXShapeUnknown, True, 4, corners);

    /* Poly 8 (2,4) */
    translate.x = 80.0; translate.y = 30.0; translate.z = 0.0;
    PEXTranslate (&translate, xform);
    PEXSetLocalTransform (display, resourceID, req_type, PEXReplace, xform);
    PEXFillArea (display, resourceID, req_type, PEXShapeUnknown, True, 4, corners);



    /*
     * Lower left quadrant (3)
     */

    /* Poly 9 (3,1) */
    translate.x = -80.0; translate.y = -30.0; translate.z = 0.0;
    PEXTranslate (&translate, xform);
    PEXSetLocalTransform (display, resourceID, req_type, PEXReplace, xform);
    PEXFillArea (display, resourceID, req_type, PEXShapeUnknown, True, 4, corners);

    /* Poly 10 (3,2) */
    translate.x = -30.0; translate.y = -30.0; translate.z = 0.0;
    PEXTranslate (&translate, xform);
    PEXSetLocalTransform (display, resourceID, req_type, PEXReplace, xform);
    PEXFillArea (display, resourceID, req_type, PEXShapeUnknown, True, 4, corners);

    /* Poly 11 (4,1) */
    translate.x = -80.0; translate.y = -80.0; translate.z = 0.0;
    PEXTranslate (&translate, xform);
    PEXSetLocalTransform (display, resourceID, req_type, PEXReplace, xform);
    PEXFillArea (display, resourceID, req_type, PEXShapeUnknown, True, 4, corners);

    /* Poly 12 (4,2) */
    translate.x = -30.0; translate.y = -80.0; translate.z = 0.0;
    PEXTranslate (&translate, xform);
    PEXSetLocalTransform (display, resourceID, req_type, PEXReplace, xform);
    PEXFillArea (display, resourceID, req_type, PEXShapeUnknown, True, 4, corners);


    /*
     * Lower right quadrant (4)

    /* Poly 13 (3,3) */
    translate.x = 30.0; translate.y = -30.0; translate.z = 0.0;
    PEXTranslate (&translate, xform);
    PEXSetLocalTransform (display, resourceID, req_type, PEXReplace, xform);
    PEXFillArea (display, resourceID, req_type, PEXShapeUnknown, True, 4, corners);

    /* Poly 14 (3,4) */
    translate.x = 80.0; translate.y = -30.0; translate.z = 0.0;
    PEXTranslate (&translate, xform);
    PEXSetLocalTransform (display, resourceID, req_type, PEXReplace, xform);
    PEXFillArea (display, resourceID, req_type, PEXShapeUnknown, True, 4, corners);

    /* Poly 15 (4,3) */
    translate.x = 30.0; translate.y = -80.0; translate.z = 0.0;
    PEXTranslate (&translate, xform);
    PEXSetLocalTransform (display, resourceID, req_type, PEXReplace, xform);
    PEXFillArea (display, resourceID, req_type, PEXShapeUnknown, True, 4, corners);

    /* Poly 16 (4,4) */
    translate.x = 30.0; translate.y = -80.0; translate.z = 0.0;
    translate.x = 80.0; translate.y = -80.0; translate.z = 0.0;
    PEXTranslate (&translate, xform);
    PEXSetLocalTransform (display, resourceID, req_type, PEXReplace, xform);
    PEXFillArea (display, resourceID, req_type, PEXShapeUnknown, True, 4, corners);
}



/*
 * pgon_attrs
 */
void pgon_attrs (display, resourceID, req_type, index)
Display         *display;
XID              resourceID;
PEXOCRequestType req_type;
int              index;
   {
   PEXSetIndividualASF (display, resourceID, req_type, 
			PEXASFInteriorStyle, PEXBundled);
   PEXSetIndividualASF (display, resourceID, req_type, 
			PEXASFSurfaceColor, PEXBundled);
   PEXSetIndividualASF (display, resourceID, req_type, 
			PEXASFReflectionModel, PEXBundled);
   PEXSetIndividualASF (display, resourceID, req_type, 
			PEXASFReflectionAttr, PEXBundled);
   PEXSetIndividualASF (display, resourceID, req_type, 
			PEXASFSurfaceInterp, PEXBundled);

   PEXSetInteriorBundleIndex (display, resourceID, req_type, index);
   }


/* 
 * define_interiors
 */
void define_interiors (display, interior_lut)
Display *display;
PEXLookupTable interior_lut;

{
    PEXInteriorBundleEntry interior_entry;

    /*
     * Set interior characteristics
     */

    interior_entry.style = PEXInteriorStyleSolid;
    interior_entry.style_index = 0;
    interior_entry.reflection_model = PEXReflectionSpecular;
    interior_entry.interp_method = PEXSurfaceInterpColor;
    interior_entry.bf_style = PEXInteriorStyleSolid;
    interior_entry.bf_style_index = 0;
    interior_entry.bf_reflection_model = PEXReflectionSpecular;
    interior_entry.bf_interp_method = PEXSurfaceInterpNone;
    interior_entry.surface_approx.method = PEXApproxDCRelative;
    interior_entry.surface_approx.u_tolerance = 1.0;
    interior_entry.surface_approx.v_tolerance = 1.0;
    interior_entry.color.type = PEXColorTypeRGB;
    interior_entry.color.value.rgb.red = 1.0;
    interior_entry.color.value.rgb.green = 1.0;
    interior_entry.color.value.rgb.blue = 1.0;
    interior_entry.reflection_attr.ambient = 1.0;
    interior_entry.reflection_attr.diffuse = 1.0;
    interior_entry.reflection_attr.specular = 1.0;
    interior_entry.reflection_attr.specular_conc = 100.0;
    interior_entry.reflection_attr.transmission = 0.0;
    interior_entry.reflection_attr.specular_color.type = PEXColorTypeRGB;
    interior_entry.reflection_attr.specular_color.value.rgb.red = 1.0;
    interior_entry.reflection_attr.specular_color.value.rgb.green = 1.0;
    interior_entry.reflection_attr.specular_color.value.rgb.blue = 1.0;
    interior_entry.bf_color.type = PEXColorTypeRGB;
    interior_entry.bf_color.value.rgb.red = 1.0;
    interior_entry.bf_color.value.rgb.green = 0.0;
    interior_entry.bf_color.value.rgb.blue = 0.0;
    interior_entry.bf_reflection_attr.specular_color.value.rgb.red = 1.0;
    interior_entry.bf_reflection_attr.specular_color.value.rgb.green = 1.0;
    interior_entry.bf_reflection_attr.specular_color.value.rgb.blue = 1.0;
    interior_entry.bf_reflection_attr.ambient = 1.0;
    interior_entry.bf_reflection_attr.diffuse = 1.0;
    interior_entry.bf_reflection_attr.specular = 1.0;
    interior_entry.bf_reflection_attr.specular_conc = 100.0;
    interior_entry.bf_reflection_attr.transmission = 1.0;
    interior_entry.bf_reflection_attr.specular_color.type = PEXColorTypeRGB;
    interior_entry.bf_reflection_attr.specular_color.value.rgb.red = 1.0;
    interior_entry.bf_reflection_attr.specular_color.value.rgb.green = 1.0;
    interior_entry.bf_reflection_attr.specular_color.value.rgb.blue = 1.0;

    PEXSetTableEntries (display, interior_lut, 2, 1, 
			PEXLUTInteriorBundle, &interior_entry);
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

void execute_test(Display        *dpy, 
		   XID              resourceID,
	           PEXOCRequestType req_type, 
		   int              cur_image)
{
    PEXTableIndex enable[NUM_AMBIENTS + NUM_DIRECTS + NUM_POINTS + 
			 NUM_SPOTS];
    PEXTableIndex disable[NUM_AMBIENTS + NUM_DIRECTS + NUM_POINTS + 
			  NUM_SPOTS];
    PEXInteriorBundleEntry interior_entry;

    switch (cur_image)
    {
    /**********************************************************************
     * Test 1
     * 
     * Enable disable ambient lights
     *********************************************************************/
    case 1:
    describe_test( "***************************************\n");
    describe_test( " Test 1       Ambient lights \n");
    describe_test( "***************************************\n\n");

    PEXSetViewIndex (dpy, resourceID, req_type, 1);

       /*
	* Setup lights
        */
       enable[0] = AMBIENT_BASE;
       PEXSetLightSourceState (dpy, resourceID, req_type,
			1, enable, 0, NULL);

        /*
        * Select interior attributes and draw pgons
        */
        pgon_attrs (dpy, resourceID, req_type, 2);
        draw_pgons (dpy, resourceID, req_type);

	describe_test( "A single gray ambient should be enabled\n");
        describe_test( "Polygons should be gray\n");

	XSync (dpy, 0);
	break;

    case 2:

        /*
         * Gray + red ambient
         */

	   PEXSetViewIndex (dpy, resourceID, req_type, 1);

           /*
	    * Setup lights
            */
           enable[0] = AMBIENT_BASE;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = AMBIENT_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
				   1, enable, 0, NULL);

	   /*
            * Select interior attributes and draw pgons
            */
	   pgon_attrs (dpy, resourceID, req_type, 2);
	   draw_pgons (dpy, resourceID, req_type);

	describe_test( "A red ambient has been added to previous ambient\n");
        describe_test( "Polygons should be reddish\n");
	describe_test( "\nHit return to continue\n");

        XSync (dpy, 0);
	break;

    case 3:
        /*
         * Gray + red + green ambient
         */
	   PEXSetViewIndex (dpy, resourceID, req_type, 1);

           /*
	    * Setup lights
            */
           enable[0] = AMBIENT_BASE;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = AMBIENT_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = AMBIENT_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
				   1, enable, 0, NULL);

	   /*
            * Select interior attributes and draw pgons
            */
	   pgon_attrs (dpy, resourceID, req_type, 2);
	   draw_pgons (dpy, resourceID, req_type);

	describe_test( "A green ambient has been added to previous ambient\n");
        describe_test( "Polygons should be yellowish\n");
	describe_test( "\nHit return to continue\n");

        XSync (dpy, 0);
	break;

    case 4:
        /*
         * Gray + red + green + blue ambient
         */
	   PEXSetViewIndex (dpy, resourceID, req_type, 1);

           /*
	    * Setup lights
            */
           enable[0] = AMBIENT_BASE;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = AMBIENT_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = AMBIENT_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = AMBIENT_BLUE;
           PEXSetLightSourceState (dpy, resourceID, req_type,
				   1, enable, 0, NULL);

	   /*
            * Select interior attributes and draw pgons
            */
	   pgon_attrs (dpy, resourceID, req_type, 2);
	   draw_pgons (dpy, resourceID, req_type);

	describe_test( "A blue ambient has been added to previous ambient\n");
        describe_test( "Polygons should be white\n");
	describe_test( "\nHit return to continue\n");

        XSync (dpy, 0);
	break;

    case 5:
        /*
         * Turn off green source
         */
	   PEXSetViewIndex (dpy, resourceID, req_type, 1);

           /*
	    * Setup lights
            */
           enable[0] = AMBIENT_BASE;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = AMBIENT_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = AMBIENT_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = AMBIENT_BLUE;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           disable[0] = AMBIENT_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
				   0, NULL, 1, disable);

	   /*
            * Select interior attributes and draw pgons
            */
	   pgon_attrs (dpy, resourceID, req_type, 2);
	   draw_pgons (dpy, resourceID, req_type);

	describe_test( "The green ambient has been turned off\n");
        describe_test( "Polygons should be magenta\n");
	describe_test( "\nHit return to continue\n");

        XSync (dpy, 0);
	break;

    case 6:
        /*
         * Turn off red source
         */
	   PEXSetViewIndex (dpy, resourceID, req_type, 1);

           /*
	    * Setup lights
            */
           enable[0] = AMBIENT_BASE;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = AMBIENT_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = AMBIENT_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = AMBIENT_BLUE;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           disable[0] = AMBIENT_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   0, NULL, 1, disable);

           disable[0] = AMBIENT_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
				   0, NULL, 1, disable);

	   /*
            * Select interior attributes and draw pgons
            */
	   pgon_attrs (dpy, resourceID, req_type, 2);
	   draw_pgons (dpy, resourceID, req_type);

	describe_test( "The red ambient has also been turned off\n");
        describe_test( "Polygons should be blue\n");
	describe_test( "\nHit return to continue\n");

        XSync (dpy, 0);
	break;

    case 7:
        /*
         * Turn off blue source
         */
           /* 
	    * Select view 
	    */
	   PEXSetViewIndex (dpy, resourceID, req_type, 1);

           /*
	    * Setup lights
            */
           enable[0] = AMBIENT_BASE;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = AMBIENT_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = AMBIENT_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = AMBIENT_BLUE;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           disable[0] = AMBIENT_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   0, NULL, 1, disable);

           disable[0] = AMBIENT_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   0, NULL, 1, disable);

           disable[0] = AMBIENT_BLUE;
           PEXSetLightSourceState (dpy, resourceID, req_type,
				   0, NULL, 1, disable);

	   /*
            * Select interior attributes and draw pgons
            */
	   pgon_attrs (dpy, resourceID, req_type, 2);
	   draw_pgons (dpy, resourceID, req_type);

	describe_test( "The blue ambient has also been turned off\n");
        describe_test( "Polygons should be gray\n");
	describe_test( "\nHit return to continue\n");

        XSync (dpy, 0);
	break;

    case 8:
        /*
         * Turn off gray source
         */
           /* 
	    * Select view 
	    */
	   PEXSetViewIndex (dpy, resourceID, req_type, 1);

           /*
	    * Setup lights
            */
           enable[0] = AMBIENT_BASE;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = AMBIENT_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = AMBIENT_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = AMBIENT_BLUE;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           disable[0] = AMBIENT_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   0, NULL, 1, disable);

           disable[0] = AMBIENT_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   0, NULL, 1, disable);

           disable[0] = AMBIENT_BLUE;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   0, NULL, 1, disable);

           disable[0] = AMBIENT_GRAY;
           PEXSetLightSourceState (dpy, resourceID, req_type,
				   0, NULL, 1, disable);

	   /*
            * Select interior attributes and draw pgons
            */
	   pgon_attrs (dpy, resourceID, req_type, 2);
	   draw_pgons (dpy, resourceID, req_type);

	describe_test( "The gray ambient has also been turned off\n");
        describe_test( "Polygons should not be lit\n");
	describe_test( "\nHit return to continue\n");

        XSync (dpy, 0);
	break;

    case 9:
        /*
         * Turn on all ambients
         */
           /* 
	    * Select view 
	    */
	   PEXSetViewIndex (dpy, resourceID, req_type, 1);

           /*
	    * Setup lights
            */
           enable[0] = AMBIENT_BASE;
           enable[1] = AMBIENT_BASE + 1;
           enable[2] = AMBIENT_BASE + 2;
           enable[3] = AMBIENT_BASE + 3;
           PEXSetLightSourceState (dpy, resourceID, req_type,
				   4, enable, 0, NULL);

	   /*
            * Select interior attributes and draw pgons
            */
	   pgon_attrs (dpy, resourceID, req_type, 2);
	   draw_pgons (dpy, resourceID, req_type);

	describe_test( "All ambients have been enabled\n");
        describe_test( "Polygons should be white\n");
	describe_test( "\nHit return to continue\n");

        XSync (dpy, 0);
	break;

    case 10:
        /*
         * Turn off all ambients
         */
           /* 
	    * Select view 
	    */
	   PEXSetViewIndex (dpy, resourceID, req_type, 1);

           /*
	    * Setup lights
            */
           enable[0] = AMBIENT_BASE;
           enable[1] = AMBIENT_BASE + 1;
           enable[2] = AMBIENT_BASE + 2;
           enable[3] = AMBIENT_BASE + 3;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   4, enable, 0, NULL);

           disable[0] = AMBIENT_BASE;
           disable[1] = AMBIENT_BASE + 1;
           disable[2] = AMBIENT_BASE + 2;
           disable[3] = AMBIENT_BASE + 3;
           PEXSetLightSourceState (dpy, resourceID, req_type,
				   0, NULL, 4, disable);

	   /*
            * Select interior attributes and draw pgons
            */
	   pgon_attrs (dpy, resourceID, req_type, 2);
	   draw_pgons (dpy, resourceID, req_type);

	describe_test( "All ambients have been disabled\n");
        describe_test( "Polygons should not lit\n");
	describe_test( "\nHit return to continue\n");

        XSync (dpy, 0);
	break;

    case 11:
        /*
         * Turn on green and gray
         */
           /* 
	    * Select view 
	    */
	   PEXSetViewIndex (dpy, resourceID, req_type, 1);

           /*
	    * Setup lights
            */
           enable[0] = AMBIENT_GRAY;
           enable[1] = AMBIENT_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
				   2, enable, 0, NULL);

	   /*
            * Select interior attributes and draw pgons
            */
	   pgon_attrs (dpy, resourceID, req_type, 2);
	   draw_pgons (dpy, resourceID, req_type);

	describe_test( "Gray and green ambients have been enabled\n");
        describe_test( "Polygons should be green\n");
	describe_test( "\nHit return to continue\n");

        XSync (dpy, 0);
	break;

    case 12:
        /*
         * Turn off green and gray, turn on red
         */
	   PEXSetViewIndex (dpy, resourceID, req_type, 1);

           /*
	    * Setup lights
            */
           enable[0] = AMBIENT_GRAY;
           enable[1] = AMBIENT_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   2, enable, 0, NULL);

	   enable[0] = AMBIENT_RED;
           disable[0] = AMBIENT_GRAY;
           disable[1] = AMBIENT_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
				   1, enable, 2, disable);

	   /*
            * Select interior attributes and draw pgons
            */
	   pgon_attrs (dpy, resourceID, req_type, 2);
	   draw_pgons (dpy, resourceID, req_type);

	describe_test( "Gray and green ambients have been disabled\n");
	describe_test( "Red ambient has been enabled\n");
        describe_test( "Polygons should be dark red\n");
	describe_test( "\nHit return to continue\n");

        XSync (dpy, 0);
	break;

    case 13:
        /*
         * Turn on blue, disable green again (should have no effect)
         */
	   PEXSetViewIndex (dpy, resourceID, req_type, 1);

           /*
	    * Setup lights
            */
           enable[0] = AMBIENT_RED;
           disable[0] = AMBIENT_GRAY;
           disable[1] = AMBIENT_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 2, disable);

	   enable[0] = AMBIENT_BLUE;
	   enable[1] = AMBIENT_RED;
           disable[0] = AMBIENT_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
				   2, enable, 1, disable);

	   /*
            * Select interior attributes and draw pgons
            */
	   pgon_attrs (dpy, resourceID, req_type, 2);
	   draw_pgons (dpy, resourceID, req_type);

	describe_test( "Blue ambient has been enabled\n");
	describe_test( "Red ambient has been enabled again (this\n");
	describe_test( "should have no effect)\n");
	describe_test( "Green ambient has been disabled again (this \n");
	describe_test( "should have no effect)\n");
        describe_test( "Polygons should be magenta\n");
	describe_test( "\nHit return to continue\n");

        XSync (dpy, 0);
	break;

	/**********************************************************************
         * Test 2
         * 
         * Enable disable directional lights
         *********************************************************************/
    case 14:
	describe_test( "***************************************\n");
	describe_test( " Test 2       Directional lights\n");
	describe_test( "***************************************\n\n");

        /*
         * Turn on gray and turnon off all ambients
         */
           /* 
	    * Select view 
	    */
	   PEXSetViewIndex (dpy, resourceID, req_type, 1);

           /*
	    * Setup lights
            */
	   enable[0] = DIRECT_GRAY;

           PEXSetLightSourceState (dpy, resourceID, req_type,
				   1, enable, 0, disable);

	   /*
            * Select interior attributes and draw pgons
            */
	   pgon_attrs (dpy, resourceID, req_type, 2);
	   draw_pgons (dpy, resourceID, req_type);

	describe_test( "All ambients have been disabled\n");
	describe_test( "Gray directional (straight on) has been enabled\n");
        describe_test( "Polygons should be gray\n");
	describe_test( "\nHit return to continue\n");

        XSync (dpy, 0);
	break;

    case 15:
        /*
         * Turn on red directional
         */
	   PEXSetViewIndex (dpy, resourceID, req_type, 1);

           /*
	    * Setup lights
            */
           enable[0] = DIRECT_GRAY;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, disable);

	   enable[0] = DIRECT_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
				   1, enable, 0, NULL);

	   /*
            * Select interior attributes and draw pgons
            */
	   pgon_attrs (dpy, resourceID, req_type, 2);
	   draw_pgons (dpy, resourceID, req_type);

	describe_test( "Red (to left) directional has been enabled\n");
        describe_test( "Polygons should be red\n");
	describe_test( "\nHit return to continue\n");

        XSync (dpy, 0);
	break;

    case 16:
        /*
         * Turn on green directional
         */
	   PEXSetViewIndex (dpy, resourceID, req_type, 1);

           /*
	    * Setup lights
            */
           enable[0] = DIRECT_GRAY;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, disable);

           enable[0] = DIRECT_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

	   enable[0] = DIRECT_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
				   1, enable, 0, NULL);

	   /*
            * Select interior attributes and draw pgons
            */
	   pgon_attrs (dpy, resourceID, req_type, 2);
	   draw_pgons (dpy, resourceID, req_type);

	describe_test( "Green (to right) directional has been enabled\n");
        describe_test( "Polygons should be yellow\n");
	describe_test( "\nHit return to continue\n");

        XSync (dpy, 0);
	break;

    case 17:
        /*
         * Turn on blue directional
         */
	   PEXSetViewIndex (dpy, resourceID, req_type, 1);

           /*
	    * Setup lights
            */
           enable[0] = DIRECT_GRAY;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, disable);

           enable[0] = DIRECT_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = DIRECT_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

	   enable[0] = DIRECT_BLUE;
           PEXSetLightSourceState (dpy, resourceID, req_type,
				   1, enable, 0, NULL);

	   /*
            * Select interior attributes and draw pgons
            */
	   pgon_attrs (dpy, resourceID, req_type, 2);
	   draw_pgons (dpy, resourceID, req_type);

	describe_test( "Blue (behind) directional has been enabled\n");
        describe_test( "Polygons should still be yellow\n");
	describe_test( "\nHit return to continue\n");

        XSync (dpy, 0);
	break;

    case 18:
        /*
         * Turn off red directional
         */
	   PEXSetViewIndex (dpy, resourceID, req_type, 1);

           /*
	    * Setup lights
            */
           enable[0] = DIRECT_GRAY;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, disable);

           enable[0] = DIRECT_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = DIRECT_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = DIRECT_BLUE;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

	   disable[0] = DIRECT_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
				   0, NULL, 1, disable);

	   /*
            * Select interior attributes and draw pgons
            */
	   pgon_attrs (dpy, resourceID, req_type, 2);
	   draw_pgons (dpy, resourceID, req_type);

	describe_test( "Red (to left) directional has been disabled\n");
        describe_test( "Polygons should be green\n");
	describe_test( "\nHit return to continue\n");

        XSync (dpy, 0);
	break;

    case 19:
        /*
         * Turn off green directional
         */
	   PEXSetViewIndex (dpy, resourceID, req_type, 1);

           /*
	    * Setup lights
            */
           enable[0] = DIRECT_GRAY;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, disable);

           enable[0] = DIRECT_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = DIRECT_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = DIRECT_BLUE;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           disable[0] = DIRECT_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   0, NULL, 1, disable);

	   disable[0] = DIRECT_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
				   0, NULL, 1, disable);

	   /*
            * Select interior attributes and draw pgons
            */
	   pgon_attrs (dpy, resourceID, req_type, 2);
	   draw_pgons (dpy, resourceID, req_type);

	describe_test( "Green (to right) directional has been disabled\n");
        describe_test( "Polygons should be gray\n");
	describe_test( "\nHit return to continue\n");

        XSync (dpy, 0);
	break;

    case 20:
        /*
         * Turn off gray directional
         */
	   PEXSetViewIndex (dpy, resourceID, req_type, 1);

           /*
	    * Setup lights
            */
           enable[0] = DIRECT_GRAY;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, disable);

           enable[0] = DIRECT_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = DIRECT_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = DIRECT_BLUE;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           disable[0] = DIRECT_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   0, NULL, 1, disable);

           disable[0] = DIRECT_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   0, NULL, 1, disable);

	   disable[0] = DIRECT_GRAY;
           PEXSetLightSourceState (dpy, resourceID, req_type,
				   0, NULL, 1, disable);

	   /*
            * Select interior attributes and draw pgons
            */
	   pgon_attrs (dpy, resourceID, req_type, 2);
	   draw_pgons (dpy, resourceID, req_type);

	describe_test( "Gray (staight on) directional has been disabled\n");
        describe_test( "Polygons should not be lit\n");
	describe_test( "\nHit return to continue\n");

        XSync (dpy, 0);
	break;

    case 21:
        /*
         * Turn off blue directional
         */
	   PEXSetViewIndex (dpy, resourceID, req_type, 1);

           /*
	    * Setup lights
            */
           enable[0] = DIRECT_GRAY;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, disable);

           enable[0] = DIRECT_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = DIRECT_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = DIRECT_BLUE;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           disable[0] = DIRECT_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   0, NULL, 1, disable);

           disable[0] = DIRECT_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   0, NULL, 1, disable);

           disable[0] = DIRECT_GRAY;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   0, NULL, 1, disable);

	   disable[0] = DIRECT_BLUE;
           PEXSetLightSourceState (dpy, resourceID, req_type,
				   0, NULL, 1, disable);

	   /*
            * Select interior attributes and draw pgons
            */
	   pgon_attrs (dpy, resourceID, req_type, 2);
	   draw_pgons (dpy, resourceID, req_type);

	describe_test( "Blue (behind) directional has been disabled\n");
        describe_test( "Polygons should still not be lit\n");
	describe_test( "\nHit return to continue\n");

        XSync (dpy, 0);
	break;

    case 22:
        /*
         * Turn on all directionals
         */
	   PEXSetViewIndex (dpy, resourceID, req_type, 1);

           /*
	    * Setup lights
            */
	   enable[0] = DIRECT_GRAY;
	   enable[1] = DIRECT_RED;
	   enable[2] = DIRECT_GREEN;
	   enable[3] = DIRECT_BLUE;

           PEXSetLightSourceState (dpy, resourceID, req_type,
				   4, enable, 0, NULL);

	   /*
            * Select interior attributes and draw pgons
            */
	   pgon_attrs (dpy, resourceID, req_type, 2);
	   draw_pgons (dpy, resourceID, req_type);

	describe_test( "All directionals have been enabled\n");
        describe_test( "Polygons should be yellow\n");
	describe_test( "\nHit return to continue\n");

        XSync (dpy, 0);
	break;

    case 23:
        /*
         * Turn off all directionals
         */
	   PEXSetViewIndex (dpy, resourceID, req_type, 1);

           /*
	    * Setup lights
            */
           enable[0] = DIRECT_GRAY;
           enable[1] = DIRECT_RED;
           enable[2] = DIRECT_GREEN;
           enable[3] = DIRECT_BLUE;

           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   4, enable, 0, NULL);

	   disable[0] = DIRECT_GRAY;
	   disable[1] = DIRECT_RED;
	   disable[2] = DIRECT_GREEN;
	   disable[3] = DIRECT_BLUE;

           PEXSetLightSourceState (dpy, resourceID, req_type,
				   0, NULL, 4, disable);

	   /*
            * Select interior attributes and draw pgons
            */
	   pgon_attrs (dpy, resourceID, req_type, 2);
	   draw_pgons (dpy, resourceID, req_type);

	describe_test( "All directionals have been disabled\n");
        describe_test( "Polygons should not be lit\n");
	describe_test( "\nHit return to continue\n");

        XSync (dpy, 0);
	break;

    case 24:
        /*
         * Turn on gray and green directionals
         */
	   PEXSetViewIndex (dpy, resourceID, req_type, 1);

           /*
	    * Setup lights
            */
	   enable[0] = DIRECT_GRAY;
	   enable[1] = DIRECT_GREEN;

           PEXSetLightSourceState (dpy, resourceID, req_type,
				   2, enable, 0, NULL);

	   /*
            * Select interior attributes and draw pgons
            */
	   pgon_attrs (dpy, resourceID, req_type, 2);
	   draw_pgons (dpy, resourceID, req_type);

	describe_test( "Gray and green directionals have been enabled\n");
        describe_test( "Polygons should be green\n");
	describe_test( "\nHit return to continue\n");

        XSync (dpy, 0);
	break;

    case 25:
        /*
         * Turn off gray and green directionals, turn on red
         */
	   PEXSetViewIndex (dpy, resourceID, req_type, 1);

           /*
	    * Setup lights
            */
           enable[0] = DIRECT_GRAY;
           enable[1] = DIRECT_GREEN;

           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   2, enable, 0, NULL);

           enable[0] = DIRECT_RED;

	   disable[0] = DIRECT_GRAY;
	   disable[1] = DIRECT_GREEN;

           PEXSetLightSourceState (dpy, resourceID, req_type,
				   1, enable, 2, disable);

	   /*
            * Select interior attributes and draw pgons
            */
	   pgon_attrs (dpy, resourceID, req_type, 2);
	   draw_pgons (dpy, resourceID, req_type);

	describe_test( "Gray and green directionals have been disabled\n");
	describe_test( "Red directional has been enabled\n");
        describe_test( "Polygons should be dark red\n");
	describe_test( "\nHit return to continue\n");

        XSync (dpy, 0);
	break;

    case 26:
        /*
         * Turn on red directionals, turn off green again
         */
	   PEXSetViewIndex (dpy, resourceID, req_type, 1);

           /*
	    * Setup lights
            */
           enable[0] = DIRECT_GRAY;
           enable[1] = DIRECT_GREEN;

           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   2, enable, 0, NULL);

           enable[0] = DIRECT_RED;

           disable[0] = DIRECT_GRAY;
           disable[1] = DIRECT_GREEN;

           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 2, disable);

           enable[0] = DIRECT_RED;

	   disable[1] = DIRECT_GREEN;

           PEXSetLightSourceState (dpy, resourceID, req_type,
				   1, enable, 1, disable);

	   /*
            * Select interior attributes and draw pgons
            */
	   pgon_attrs (dpy, resourceID, req_type, 2);
	   draw_pgons (dpy, resourceID, req_type);

	describe_test( "Green directional have been disabled again (this\n");
	describe_test( "should have no effect)\n");
	describe_test( "Red directional has been enabled again (this\n");
	describe_test( "should have no effect)\n");
        describe_test( "Polygons should still be dark red\n");
	describe_test( "\nHit return to continue\n");

        XSync (dpy, 0);
	break;

	/**********************************************************************
         * Test 3
         * 
         * Enable disable point lights
         *********************************************************************/
    case 27:
	describe_test( "***************************************\n");
	describe_test( " Test 3       Point lights\n");
	describe_test( "***************************************\n\n");

        /*
         * Turn on off directionals, turn on first point source
         */
	   PEXSetViewIndex (dpy, resourceID, req_type, 1);

           /*
	    * Setup lights
            */
           enable[0] = POINT_1_GRAY;

           PEXSetLightSourceState (dpy, resourceID, req_type,
				   1, enable, 0, NULL);

	   /*
            * Select interior attributes and draw pgons
            */
	   pgon_attrs (dpy, resourceID, req_type, 2);
	   draw_pgons (dpy, resourceID, req_type);

	describe_test( "All directionals have been disabled \n");
	describe_test( "Gray point light in upper left has been enabled\n");
        describe_test( "Polygons in upper left should be gray\n");
        describe_test( "Polygons in other quadrants should be softly lit\n");
	describe_test( "\nHit return to continue\n");

        XSync (dpy, 0);
	break;

    case 28:
        /*
         * Turn on red point
         */
	   PEXSetViewIndex (dpy, resourceID, req_type, 1);

           /*
	    * Setup lights
            */
           enable[0] = POINT_1_GRAY;

           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = POINT_2_RED;

           PEXSetLightSourceState (dpy, resourceID, req_type,
				   1, enable, 0, NULL);

	   /*
            * Select interior attributes and draw pgons
            */
	   pgon_attrs (dpy, resourceID, req_type, 2);
	   draw_pgons (dpy, resourceID, req_type);

	describe_test( "Red point light in upper right has been enabled\n");
        describe_test( "Polygons in upper right should be red\n");
        describe_test( "Polygons in lower quadrants should be softly lit\n");
	describe_test( "\nHit return to continue\n");

        XSync (dpy, 0);
	break;

    case 29:
        /*
         * Turn on green point
         */
	   PEXSetViewIndex (dpy, resourceID, req_type, 1);

           /*
	    * Setup lights
            */
           enable[0] = POINT_1_GRAY;

           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = POINT_2_RED;

           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = POINT_3_GREEN;

           PEXSetLightSourceState (dpy, resourceID, req_type,
				   1, enable, 0, NULL);

	   /*
            * Select interior attributes and draw pgons
            */
	   pgon_attrs (dpy, resourceID, req_type, 2);
	   draw_pgons (dpy, resourceID, req_type);

	describe_test( "Green point light in lower left has been enabled\n");
        describe_test( "Polygons in lower left should be green\n");
        describe_test( "Polygons in lower right should be softly lit\n");
	describe_test( "\nHit return to continue\n");

        XSync (dpy, 0);
	break;

    case 30:
        /*
         * Turn on blue point
         */
	   PEXSetViewIndex (dpy, resourceID, req_type, 1);

           /*
	    * Setup lights
            */
           enable[0] = POINT_1_GRAY;

           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = POINT_2_RED;

           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = POINT_3_GREEN;

           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = POINT_4_BLUE;

           PEXSetLightSourceState (dpy, resourceID, req_type,
				   1, enable, 0, NULL);

	   /*
            * Select interior attributes and draw pgons
            */
	   pgon_attrs (dpy, resourceID, req_type, 2);
	   draw_pgons (dpy, resourceID, req_type);

	describe_test( "Blue point light in lower right has been enabled\n");
        describe_test( "Polygons in lower right should be blue\n");
	describe_test( "\nHit return to continue\n");

        XSync (dpy, 0);
	break;

    case 31:
        /*
         * Turn on magenta point
         */
	   PEXSetViewIndex (dpy, resourceID, req_type, 1);

           /*
	    * Setup lights
            */
           enable[0] = POINT_1_GRAY;

           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = POINT_2_RED;

           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = POINT_3_GREEN;

           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = POINT_4_BLUE;

           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = POINT_5_MAGENTA;

           PEXSetLightSourceState (dpy, resourceID, req_type,
				   1, enable, 0, NULL);

	   /*
            * Select interior attributes and draw pgons
            */
	   pgon_attrs (dpy, resourceID, req_type, 2);
	   draw_pgons (dpy, resourceID, req_type);

	describe_test( "Magenta point light behind upper left has been enabled\n");
        describe_test( "(This should have no effect)\n");
        describe_test( "Polygon shading should not have changed\n");
	describe_test( "\nHit return to continue\n");

        XSync (dpy, 0);
	break;

    case 32:
        /*
         * Turn off blue point
         */
	   PEXSetViewIndex (dpy, resourceID, req_type, 1);

           /*
	    * Setup lights
            */
           enable[0] = POINT_1_GRAY;

           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = POINT_2_RED;

           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = POINT_3_GREEN;

           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = POINT_4_BLUE;

           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = POINT_5_MAGENTA;

           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           disable[0] = POINT_4_BLUE;

           PEXSetLightSourceState (dpy, resourceID, req_type,
				   0, NULL, 1, disable);

	   /*
            * Select interior attributes and draw pgons
            */
	   pgon_attrs (dpy, resourceID, req_type, 2);
	   draw_pgons (dpy, resourceID, req_type);

	describe_test( "Blue point light in lower right has been disabled\n");
        describe_test( "Polygons in lower right should be barely lit\n");
	describe_test( "\nHit return to continue\n");

        XSync (dpy, 0);
	break;

    case 33:
        /*
         * Turn off green point
         */
	   PEXSetViewIndex (dpy, resourceID, req_type, 1);

           /*
	    * Setup lights
            */
           enable[0] = POINT_1_GRAY;

           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = POINT_2_RED;

           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = POINT_3_GREEN;

           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = POINT_4_BLUE;

           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = POINT_5_MAGENTA;

           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           disable[0] = POINT_4_BLUE;

           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   0, NULL, 1, disable);

           disable[0] = POINT_3_GREEN;

           PEXSetLightSourceState (dpy, resourceID, req_type,
				   0, NULL, 1, disable);

	   /*
            * Select interior attributes and draw pgons
            */
	   pgon_attrs (dpy, resourceID, req_type, 2);
	   draw_pgons (dpy, resourceID, req_type);

	describe_test( "Green point light in lower left has been disabled\n");
        describe_test( "Polygons in lower quadrants should be barely lit\n");
	describe_test( "\nHit return to continue\n");

        XSync (dpy, 0);
	break;

    case 34:
        /*
         * Turn off red point
         */
	   PEXSetViewIndex (dpy, resourceID, req_type, 1);

           /*
	    * Setup lights
            */
           enable[0] = POINT_1_GRAY;

           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = POINT_2_RED;

           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = POINT_3_GREEN;

           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = POINT_4_BLUE;

           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = POINT_5_MAGENTA;

           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           disable[0] = POINT_4_BLUE;

           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   0, NULL, 1, disable);

           disable[0] = POINT_3_GREEN;

           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   0, NULL, 1, disable);


           disable[0] = POINT_2_RED;

           PEXSetLightSourceState (dpy, resourceID, req_type,
				   0, NULL, 1, disable);

	   /*
            * Select interior attributes and draw pgons
            */
	   pgon_attrs (dpy, resourceID, req_type, 2);
	   draw_pgons (dpy, resourceID, req_type);

	describe_test( "Red point light in upper right has been disabled\n");
        describe_test( "Only polys in upper left should be noticably lit\n");
	describe_test( "\nHit return to continue\n");

        XSync (dpy, 0);
	break;

    case 35:
        /*
         * Turn off gray point
         */
	   PEXSetViewIndex (dpy, resourceID, req_type, 1);

           /*
	    * Setup lights
            */
           enable[0] = POINT_1_GRAY;

           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = POINT_2_RED;

           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = POINT_3_GREEN;

           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = POINT_4_BLUE;

           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = POINT_5_MAGENTA;

           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           disable[0] = POINT_4_BLUE;

           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   0, NULL, 1, disable);

           disable[0] = POINT_3_GREEN;

           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   0, NULL, 1, disable);


           disable[0] = POINT_2_RED;

           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   0, NULL, 1, disable);

           disable[0] = POINT_1_GRAY;

           PEXSetLightSourceState (dpy, resourceID, req_type,
				   0, NULL, 1, disable);

	   /*
            * Select interior attributes and draw pgons
            */
	   pgon_attrs (dpy, resourceID, req_type, 2);
	   draw_pgons (dpy, resourceID, req_type);

	describe_test( "Gray point light in upper left has been disabled\n");
        describe_test( "Polygons should not be lit\n");
	describe_test( "\nHit return to continue\n");

        XSync (dpy, 0);
	break;

    case 36:
        /*
         * Turn on all visible points. turn off magenta 
         */
	   PEXSetViewIndex (dpy, resourceID, req_type, 1);

           /*
	    * Setup lights
            */
           disable[0] = POINT_5_MAGENTA;

           enable[0] = POINT_1_GRAY;
           enable[1] = POINT_2_RED;
           enable[2] = POINT_3_GREEN;
           enable[3] = POINT_4_BLUE;

           PEXSetLightSourceState (dpy, resourceID, req_type,
				   4, enable, 1, disable);

	   /*
            * Select interior attributes and draw pgons
            */
	   pgon_attrs (dpy, resourceID, req_type, 2);
	   draw_pgons (dpy, resourceID, req_type);

	describe_test( "All visible point lights has been enabled\n");
        describe_test( "All polygons should be lit as done previously\n");
	describe_test( "\nHit return to continue\n");

        XSync (dpy, 0);
	break;

    case 37:
        /*
         * Turn off all visible points
         */
	   PEXSetViewIndex (dpy, resourceID, req_type, 1);

           /*
	    * Setup lights
            */
           enable[0] = POINT_1_GRAY;
           enable[1] = POINT_2_RED;
           enable[2] = POINT_3_GREEN;
           enable[3] = POINT_4_BLUE;

           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   4, enable, 0, NULL);


           disable[0] = POINT_1_GRAY;
           disable[1] = POINT_2_RED;
           disable[2] = POINT_3_GREEN;
           disable[3] = POINT_4_BLUE;

           PEXSetLightSourceState (dpy, resourceID, req_type,
				   0, NULL, 4, disable);

	   /*
            * Select interior attributes and draw pgons
            */
	   pgon_attrs (dpy, resourceID, req_type, 2);
	   draw_pgons (dpy, resourceID, req_type);

	describe_test( "All visible point lights has been disabled\n");
        describe_test( "No polygons should be lit \n");
	describe_test( "\nHit return to continue\n");

        XSync (dpy, 0);
	break;

    case 38:
        /*
         * Turn on Quadrant 3 light
         */
	   PEXSetViewIndex (dpy, resourceID, req_type, 1);

           /*
	    * Setup lights
            */

           enable[0] = POINT_3_GREEN;

           PEXSetLightSourceState (dpy, resourceID, req_type,
				   1, enable, 0, NULL);

	   /*
            * Select interior attributes and draw pgons
            */
	   pgon_attrs (dpy, resourceID, req_type, 2);
	   draw_pgons (dpy, resourceID, req_type);

	describe_test( "The green lower left point light has been enabled\n");
        describe_test( "The lower left polygons should be green \n");
	describe_test( "\nHit return to continue\n");

        XSync (dpy, 0);
	break;

    case 39:
        /*
         * Turn off Quadrant 3 light, turn on quadrant 1 and 4
         */
	   PEXSetViewIndex (dpy, resourceID, req_type, 1);

           /*
	    * Setup lights
            */
           enable[0] = POINT_3_GREEN;

           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           disable[0] = POINT_3_GREEN;

           enable[0] = POINT_1_GRAY;
           enable[1] = POINT_4_BLUE;

           PEXSetLightSourceState (dpy, resourceID, req_type,
				   2, enable, 1, disable);

	   /*
            * Select interior attributes and draw pgons
            */
	   pgon_attrs (dpy, resourceID, req_type, 2);
	   draw_pgons (dpy, resourceID, req_type);

	describe_test( "The green lower left point light has been disabled\n");
	describe_test( "The blue lower right and the gray upper left\n");
	describe_test( "point lights have been enabled\n");
        describe_test( "The lower right and upper left polygons should be lit\n");
	describe_test( "\nHit return to continue\n");

        XSync (dpy, 0);
	break;

    case 40:
        /*
         * Turn on Quadrant 4 light, disable quadrant 2 (again)
         */
	   PEXSetViewIndex (dpy, resourceID, req_type, 1);

           /*
	    * Setup lights
            */
           enable[0] = POINT_1_GRAY;
           enable[1] = POINT_4_BLUE;

           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   2, enable, 0, NULL);


           disable[0] = POINT_2_RED;
           enable[0] = POINT_4_BLUE;

           PEXSetLightSourceState (dpy, resourceID, req_type,
				   1, enable, 1, disable);

	   /*
            * Select interior attributes and draw pgons
            */
	   pgon_attrs (dpy, resourceID, req_type, 2);
	   draw_pgons (dpy, resourceID, req_type);

	describe_test( "The red upper left point light has been disabled again\n");
	describe_test( "The blue lower right point light has been enabled again\n");
	describe_test( "No change in the image should have occurred\n");
	describe_test( "\nHit return to continue\n");

        XSync (dpy, 0);
	break;

	/**********************************************************************
         * Test 4
         * 
         *********************************************************************/
    case 41:
	describe_test( "***************************************\n");
	describe_test( " Test 4      Spot lights\n");
	describe_test( "***************************************\n\n");

        /*
         * Turn off point lights, turn on first spot
         */
	   PEXSetViewIndex (dpy, resourceID, req_type, 1);

           /*
	    * Setup lights
            */

	   enable[0] = SPOT_1_1_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
				   1, enable, 0, NULL);

	   /*
            * Select interior attributes and draw pgons
            */
	   pgon_attrs (dpy, resourceID, req_type, 2);
	   draw_pgons (dpy, resourceID, req_type);

	describe_test( "Spot (1,1) [1 red] has been enabled\n");
	describe_test( "Poly (1,1) should be red\n");
	describe_test( "\nHit return to continue\n");

        XSync (dpy, 0);
	break;

    case 42:
        /*
         * Turn on spot 2
         */
	   PEXSetViewIndex (dpy, resourceID, req_type, 1);

           /*
	    * Setup lights
            */
           enable[0] = SPOT_1_1_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

	   enable[0] = SPOT_1_2_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
				   1, enable, 0, NULL);

	   /*
            * Select interior attributes and draw pgons
            */
	   pgon_attrs (dpy, resourceID, req_type, 2);
	   draw_pgons (dpy, resourceID, req_type);

	describe_test( "Spot (1,2) [2 green] has been enabled\n");
	describe_test( "Poly (1,2) should be green\n");
	describe_test( "\nHit return to continue\n");

        XSync (dpy, 0);
	break;

    case 43:
        /*
         * Turn on spot 3
         */
	   PEXSetViewIndex (dpy, resourceID, req_type, 1);

           /*
	    * Setup lights
            */
           enable[0] = SPOT_1_1_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_1_2_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

	   enable[0] = SPOT_2_1_BLUE;
           PEXSetLightSourceState (dpy, resourceID, req_type,
				   1, enable, 0, NULL);

	   /*
            * Select interior attributes and draw pgons
            */
	   pgon_attrs (dpy, resourceID, req_type, 2);
	   draw_pgons (dpy, resourceID, req_type);

	describe_test( "Spot (2,1) [3 blue] has been enabled\n");
	describe_test( "Poly (2,1) should be blue\n");
	describe_test( "\nHit return to continue\n");

        XSync (dpy, 0);
	break;

    case 44:
        /*
         * Turn on spot 4
         */
	   PEXSetViewIndex (dpy, resourceID, req_type, 1);

           /*
	    * Setup lights
            */
           enable[0] = SPOT_1_1_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_1_2_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_2_1_BLUE;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

	   enable[0] = SPOT_2_2_MAGENTA;
           PEXSetLightSourceState (dpy, resourceID, req_type,
				   1, enable, 0, NULL);

	   /*
            * Select interior attributes and draw pgons
            */
	   pgon_attrs (dpy, resourceID, req_type, 2);
	   draw_pgons (dpy, resourceID, req_type);

	describe_test( "Spot (2,2) [4 magenta] has been enabled\n");
	describe_test( "Poly (2,2) should be magenta\n");
	describe_test( "\nHit return to continue\n");

        XSync (dpy, 0);
	break;

    case 45:
        /*
         * Turn on spot 5
         */
	   PEXSetViewIndex (dpy, resourceID, req_type, 1);

           /*
	    * Setup lights
            */
           enable[0] = SPOT_1_1_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_1_2_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_2_1_BLUE;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_2_2_MAGENTA;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

	   enable[0] = SPOT_1_3_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
				   1, enable, 0, NULL);

	   /*
            * Select interior attributes and draw pgons
            */
	   pgon_attrs (dpy, resourceID, req_type, 2);
	   draw_pgons (dpy, resourceID, req_type);

	describe_test( "Spot (1,3) [5 red] has been enabled\n");
	describe_test( "Poly (1,3) should be red\n");
	describe_test( "\nHit return to continue\n");

        XSync (dpy, 0);
	break;

    case 46:
        /*
         * Turn on spot 6
         */
	   PEXSetViewIndex (dpy, resourceID, req_type, 1);

           /*
	    * Setup lights
            */
           enable[0] = SPOT_1_1_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_1_2_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_2_1_BLUE;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_2_2_MAGENTA;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_1_3_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

	   enable[0] = SPOT_1_4_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
				   1, enable, 0, NULL);

	   /*
            * Select interior attributes and draw pgons
            */
	   pgon_attrs (dpy, resourceID, req_type, 2);
	   draw_pgons (dpy, resourceID, req_type);

	describe_test( "Spot (1,4) [6 green] has been enabled\n");
	describe_test( "Poly (1,4) should be green\n");
	describe_test( "\nHit return to continue\n");

        XSync (dpy, 0);
	break;

    case 47:
        /*
         * Turn on spot 7
         */
	   PEXSetViewIndex (dpy, resourceID, req_type, 1);

           /*
	    * Setup lights
            */
           enable[0] = SPOT_1_1_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_1_2_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_2_1_BLUE;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_2_2_MAGENTA;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_1_3_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_1_4_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

	   enable[0] = SPOT_2_3_BLUE;
           PEXSetLightSourceState (dpy, resourceID, req_type,
				   1, enable, 0, NULL);

	   /*
            * Select interior attributes and draw pgons
            */
	   pgon_attrs (dpy, resourceID, req_type, 2);
	   draw_pgons (dpy, resourceID, req_type);

	describe_test( "Spot (2,3) [7 blue] has been enabled\n");
	describe_test( "Poly (2,3) should be blue\n");
	describe_test( "\nHit return to continue\n");

        XSync (dpy, 0);
	break;

    case 48:
        /*
         * Turn on spot 8
         */
	   PEXSetViewIndex (dpy, resourceID, req_type, 1);

           /*
	    * Setup lights
            */
           enable[0] = SPOT_1_1_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_1_2_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_2_1_BLUE;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_2_2_MAGENTA;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_1_3_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_1_4_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_2_3_BLUE;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

	   enable[0] = SPOT_2_4_MAGENTA;
           PEXSetLightSourceState (dpy, resourceID, req_type,
				   1, enable, 0, NULL);

	   /*
            * Select interior attributes and draw pgons
            */
	   pgon_attrs (dpy, resourceID, req_type, 2);
	   draw_pgons (dpy, resourceID, req_type);

	describe_test( "Spot (2,4) [8 magenta] has been enabled\n");
	describe_test( "Poly (2,4) should be magenta\n");
	describe_test( "\nHit return to continue\n");

        XSync (dpy, 0);
	break;

    case 49:
        /*
         * Turn on spot 9
         */
	   PEXSetViewIndex (dpy, resourceID, req_type, 1);

           /*
	    * Setup lights
            */
           enable[0] = SPOT_1_1_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_1_2_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_2_1_BLUE;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_2_2_MAGENTA;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_1_3_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_1_4_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_2_3_BLUE;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_2_4_MAGENTA;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

	   enable[0] = SPOT_3_1_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
				   1, enable, 0, NULL);

	   /*
            * Select interior attributes and draw pgons
            */
	   pgon_attrs (dpy, resourceID, req_type, 2);
	   draw_pgons (dpy, resourceID, req_type);

	describe_test( "Spot (3,1) [9 red] has been enabled\n");
	describe_test( "Poly (3,1) should be red\n");
	describe_test( "\nHit return to continue\n");

        XSync (dpy, 0);
	break;

    case 50:
        /*
         * Turn on spot 10
         */
	   PEXSetViewIndex (dpy, resourceID, req_type, 1);

           /*
	    * Setup lights
            */
           enable[0] = SPOT_1_1_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_1_2_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_2_1_BLUE;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_2_2_MAGENTA;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_1_3_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_1_4_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_2_3_BLUE;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_2_4_MAGENTA;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_3_1_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

	   enable[0] = SPOT_3_2_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
				   1, enable, 0, NULL);

	   /*
            * Select interior attributes and draw pgons
            */
	   pgon_attrs (dpy, resourceID, req_type, 2);
	   draw_pgons (dpy, resourceID, req_type);

	describe_test( "Spot (3,2) [10 green] has been enabled\n");
	describe_test( "Poly (3,2) should be green\n");
	describe_test( "\nHit return to continue\n");

        XSync (dpy, 0);
	break;

    case 51:
        /*
         * Turn on spot 11
         */
	   PEXSetViewIndex (dpy, resourceID, req_type, 1);

           /*
	    * Setup lights
            */
           enable[0] = SPOT_1_1_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_1_2_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_2_1_BLUE;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_2_2_MAGENTA;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_1_3_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_1_4_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_2_3_BLUE;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_2_4_MAGENTA;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_3_1_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_3_2_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

	   enable[0] = SPOT_4_1_BLUE;
           PEXSetLightSourceState (dpy, resourceID, req_type,
				   1, enable, 0, NULL);

	   /*
            * Select interior attributes and draw pgons
            */
	   pgon_attrs (dpy, resourceID, req_type, 2);
	   draw_pgons (dpy, resourceID, req_type);

	describe_test( "Spot (4,1) [11 blue] has been enabled\n");
	describe_test( "Poly (4,1) should be blue\n");
	describe_test( "\nHit return to continue\n");

        XSync (dpy, 0);
	break;

    case 52:
        /*
         * Turn on spot 12
         */
	   PEXSetViewIndex (dpy, resourceID, req_type, 1);

           /*
	    * Setup lights
            */
           enable[0] = SPOT_1_1_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_1_2_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_2_1_BLUE;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_2_2_MAGENTA;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_1_3_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_1_4_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_2_3_BLUE;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_2_4_MAGENTA;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_3_1_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_3_2_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_4_1_BLUE;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

	   enable[0] = SPOT_4_2_MAGENTA;
           PEXSetLightSourceState (dpy, resourceID, req_type,
				   1, enable, 0, NULL);

	   /*
            * Select interior attributes and draw pgons
            */
	   pgon_attrs (dpy, resourceID, req_type, 2);
	   draw_pgons (dpy, resourceID, req_type);

	describe_test( "Spot (4,2) [12 magenta] has been enabled\n");
	describe_test( "Poly (4,2) should be magenta\n");
	describe_test( "\nHit return to continue\n");

        XSync (dpy, 0);
	break;

    case 53:
        /*
         * Turn on spot 13
         */
	   PEXSetViewIndex (dpy, resourceID, req_type, 1);

           /*
	    * Setup lights
            */
           enable[0] = SPOT_1_1_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_1_2_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_2_1_BLUE;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_2_2_MAGENTA;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_1_3_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_1_4_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_2_3_BLUE;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_2_4_MAGENTA;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_3_1_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_3_2_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_4_1_BLUE;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_4_2_MAGENTA;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

	   enable[0] = SPOT_3_3_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
				   1, enable, 0, NULL);

	   /*
            * Select interior attributes and draw pgons
            */
	   pgon_attrs (dpy, resourceID, req_type, 2);
	   draw_pgons (dpy, resourceID, req_type);

	describe_test( "Spot (3,3) [13 red] has been enabled\n");
	describe_test( "Poly (3,3) should be red\n");
	describe_test( "\nHit return to continue\n");

        XSync (dpy, 0);
	break;

    case 54:
        /*
         * Turn on spot 14
         */
	   PEXSetViewIndex (dpy, resourceID, req_type, 1);

           /*
	    * Setup lights
            */
           enable[0] = SPOT_1_1_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_1_2_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_2_1_BLUE;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_2_2_MAGENTA;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_1_3_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_1_4_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_2_3_BLUE;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_2_4_MAGENTA;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_3_1_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_3_2_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_4_1_BLUE;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_4_2_MAGENTA;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_3_3_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

	   enable[0] = SPOT_3_4_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
				   1, enable, 0, NULL);

	   /*
            * Select interior attributes and draw pgons
            */
	   pgon_attrs (dpy, resourceID, req_type, 2);
	   draw_pgons (dpy, resourceID, req_type);

	describe_test( "Spot (3,4) [14 green] has been enabled\n");
	describe_test( "Poly (3,4) should be green\n");
	describe_test( "\nHit return to continue\n");

        XSync (dpy, 0);
	break;

    case 55:
        /*
         * Turn on spot 15
         */
	   PEXSetViewIndex (dpy, resourceID, req_type, 1);

           /*
	    * Setup lights
            */
           enable[0] = SPOT_1_1_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_1_2_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_2_1_BLUE;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_2_2_MAGENTA;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_1_3_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_1_4_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_2_3_BLUE;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_2_4_MAGENTA;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_3_1_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_3_2_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_4_1_BLUE;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_4_2_MAGENTA;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_3_3_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_3_4_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

	   enable[0] = SPOT_4_3_BLUE;
           PEXSetLightSourceState (dpy, resourceID, req_type,
				   1, enable, 0, NULL);

	   /*
            * Select interior attributes and draw pgons
            */
	   pgon_attrs (dpy, resourceID, req_type, 2);
	   draw_pgons (dpy, resourceID, req_type);

	describe_test( "Spot (4,3) [15 blue] has been enabled\n");
	describe_test( "Poly (4,3) should be blue\n");
	describe_test( "\nHit return to continue\n");

        XSync (dpy, 0);
	break;

    case 56:
        /*
         * Turn on spot 16
         */
	   PEXSetViewIndex (dpy, resourceID, req_type, 1);

           /*
	    * Setup lights
            */
           enable[0] = SPOT_1_1_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_1_2_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_2_1_BLUE;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_2_2_MAGENTA;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_1_3_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_1_4_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_2_3_BLUE;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_2_4_MAGENTA;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_3_1_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_3_2_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_4_1_BLUE;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_4_2_MAGENTA;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_3_3_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_3_4_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_4_3_BLUE;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

	   enable[0] = SPOT_4_4_MAGENTA;
           PEXSetLightSourceState (dpy, resourceID, req_type,
				   1, enable, 0, NULL);

	   /*
            * Select interior attributes and draw pgons
            */
	   pgon_attrs (dpy, resourceID, req_type, 2);
	   draw_pgons (dpy, resourceID, req_type);

	describe_test( "Spot (4,4) [16 magenta] has been enabled\n");
	describe_test( "Poly (4,4) should NOT be lit because we have\n");
	describe_test( "exceeded 15 non-ambient lights\n");
	describe_test( "\nHit return to continue\n");

        XSync (dpy, 0);
	break;

    case 57:
        /*
         * Turn on spot 16, turn on spot 4
         */
	   PEXSetViewIndex (dpy, resourceID, req_type, 1);

           /*
	    * Setup lights
            */
           enable[0] = SPOT_1_1_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_1_2_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_2_1_BLUE;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_2_2_MAGENTA;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_1_3_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_1_4_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_2_3_BLUE;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_2_4_MAGENTA;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_3_1_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_3_2_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_4_1_BLUE;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_4_2_MAGENTA;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_3_3_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_3_4_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_4_3_BLUE;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_4_4_MAGENTA;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

	   disable[0] = SPOT_2_2_MAGENTA;
	   enable[0] = SPOT_4_4_MAGENTA;

           PEXSetLightSourceState (dpy, resourceID, req_type,
				   1, enable, 1, disable);

	   /*
            * Select interior attributes and draw pgons
            */
	   pgon_attrs (dpy, resourceID, req_type, 2);
	   draw_pgons (dpy, resourceID, req_type);

	describe_test( "Spot (4,4) [16 magenta] has been enabled\n");
	describe_test( "Spot (2,2) [4 magenta] has been disabled\n");
	describe_test( "Poly (4,4) should now be lit \n");
	describe_test( "Poly (2,2) should NOT be lit \n");
	describe_test( "\nHit return to continue\n");

        XSync (dpy, 0);
	break;

    case 58:
        /*
         * Turn off all spots
         */
	   PEXSetViewIndex (dpy, resourceID, req_type, 1);

           /*
	    * Setup lights
            */
           enable[0] = SPOT_1_1_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_1_2_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_2_1_BLUE;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_2_2_MAGENTA;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_1_3_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_1_4_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_2_3_BLUE;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_2_4_MAGENTA;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_3_1_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_3_2_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_4_1_BLUE;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_4_2_MAGENTA;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_3_3_RED;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_3_4_GREEN;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_4_3_BLUE;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           enable[0] = SPOT_4_4_MAGENTA;
           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 0, NULL);

           disable[0] = SPOT_2_2_MAGENTA;
           enable[0] = SPOT_4_4_MAGENTA;

           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   1, enable, 1, disable);


	   disable[0] = SPOT_BASE;
	   disable[1] = SPOT_BASE + 1;
	   disable[2] = SPOT_BASE + 2;
	   disable[3] = SPOT_BASE + 3;
	   disable[4] = SPOT_BASE + 4;
	   disable[5] = SPOT_BASE + 5;
	   disable[6] = SPOT_BASE + 6;
	   disable[7] = SPOT_BASE + 7;
	   disable[8] = SPOT_BASE + 8;
	   disable[9] = SPOT_BASE + 9;
	   disable[10] = SPOT_BASE + 10;
	   disable[11] = SPOT_BASE + 11;
	   disable[12] = SPOT_BASE + 12;
	   disable[13] = SPOT_BASE + 13;
	   disable[14] = SPOT_BASE + 14;
	   disable[15] = SPOT_BASE + 15;
	   disable[16] = SPOT_BASE + 16;

           PEXSetLightSourceState (dpy, resourceID, req_type,
				   0, disable, 17, disable);

	   /*
            * Select interior attributes and draw pgons
            */
	   pgon_attrs (dpy, resourceID, req_type, 2);
	   draw_pgons (dpy, resourceID, req_type);

	describe_test( "All spots have been disabled\n");
	describe_test( "No polys should be lit\n");
	describe_test( "\nHit return to continue\n");

        XSync (dpy, 0);
	break;

    case 59:
        /*
         * Turn on all spots
         */
	   PEXSetViewIndex (dpy, resourceID, req_type, 1);

           /*
	    * Setup lights
            */

	   enable[0] = SPOT_BASE + 14;
	   enable[1] = SPOT_BASE + 1;
	   enable[2] = SPOT_BASE + 2;
	   enable[3] = SPOT_BASE + 3;
	   enable[4] = SPOT_BASE + 4;
	   enable[5] = SPOT_BASE + 5;
	   enable[6] = SPOT_BASE + 6;
	   enable[7] = SPOT_BASE + 7;
	   enable[8] = SPOT_BASE + 8;
	   enable[9] = SPOT_BASE + 9;
	   enable[10] = SPOT_BASE + 10;
	   enable[11] = SPOT_BASE + 11;
	   enable[12] = SPOT_BASE + 12;
	   enable[13] = SPOT_BASE + 13;
	   enable[14] = SPOT_BASE + 15;
	   enable[15] = SPOT_BASE + 16;
	   enable[16] = SPOT_BASE;

           PEXSetLightSourceState (dpy, resourceID, req_type,
				   17, enable, 0, NULL);

	   /*
            * Select interior attributes and draw pgons
            */
	   pgon_attrs (dpy, resourceID, req_type, 2);
	   draw_pgons (dpy, resourceID, req_type);

	describe_test( "All spots have been enabled\n");
	describe_test( "All polys should be lit except (1,1)\n");
	describe_test( "\nHit return to continue\n");

        XSync (dpy, 0);
	break;

    case 60:
        /*
         * Turn off magenta and red spots 
         */
	   PEXSetViewIndex (dpy, resourceID, req_type, 1);

           /*
	    * Setup lights
            */
           enable[0] = SPOT_BASE + 14;
           enable[1] = SPOT_BASE + 1;
           enable[2] = SPOT_BASE + 2;
           enable[3] = SPOT_BASE + 3;
           enable[4] = SPOT_BASE + 4;
           enable[5] = SPOT_BASE + 5;
           enable[6] = SPOT_BASE + 6;
           enable[7] = SPOT_BASE + 7;
           enable[8] = SPOT_BASE + 8;
           enable[9] = SPOT_BASE + 9;
           enable[10] = SPOT_BASE + 10;
           enable[11] = SPOT_BASE + 11;
           enable[12] = SPOT_BASE + 12;
           enable[13] = SPOT_BASE + 13;
           enable[14] = SPOT_BASE + 15;
           enable[15] = SPOT_BASE + 16;
           enable[16] = SPOT_BASE;

           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   17, enable, 0, NULL);

	   disable[0] = SPOT_BASE;
	   disable[1] = SPOT_BASE + 3;
	   disable[2] = SPOT_BASE + 4;
	   disable[3] = SPOT_BASE + 7;
	   disable[4] = SPOT_BASE + 8;
	   disable[5] = SPOT_BASE + 11;
	   disable[6] = SPOT_BASE + 12;
	   disable[7] = SPOT_BASE + 15;

           PEXSetLightSourceState (dpy, resourceID, req_type,
				   0, NULL, 8, disable);

	   /*
            * Select interior attributes and draw pgons
            */
	   pgon_attrs (dpy, resourceID, req_type, 2);
	   draw_pgons (dpy, resourceID, req_type);

	describe_test( "All red and magenta spots have been disabled\n");
	describe_test( "Only green and blue polys should be lit \n");
	describe_test( "\nHit return to continue\n");

        XSync (dpy, 0);
	break;

    case 61:
        /*
         * Turn on magenta and red spots turn off green and blue spots
         */
	   PEXSetViewIndex (dpy, resourceID, req_type, 1);

           /*
	    * Setup lights
            */
           enable[0] = SPOT_BASE + 14;
           enable[1] = SPOT_BASE + 1;
           enable[2] = SPOT_BASE + 2;
           enable[3] = SPOT_BASE + 3;
           enable[4] = SPOT_BASE + 4;
           enable[5] = SPOT_BASE + 5;
           enable[6] = SPOT_BASE + 6;
           enable[7] = SPOT_BASE + 7;
           enable[8] = SPOT_BASE + 8;
           enable[9] = SPOT_BASE + 9;
           enable[10] = SPOT_BASE + 10;
           enable[11] = SPOT_BASE + 11;
           enable[12] = SPOT_BASE + 12;
           enable[13] = SPOT_BASE + 13;
           enable[14] = SPOT_BASE + 15;
           enable[15] = SPOT_BASE + 16;
           enable[16] = SPOT_BASE;

           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   17, enable, 0, NULL);

           disable[0] = SPOT_BASE;
           disable[1] = SPOT_BASE + 3;
           disable[2] = SPOT_BASE + 4;
           disable[3] = SPOT_BASE + 7;
           disable[4] = SPOT_BASE + 8;
           disable[5] = SPOT_BASE + 11;
           disable[6] = SPOT_BASE + 12;
           disable[7] = SPOT_BASE + 15;

           PEXSetLightSourceState (dpy, resourceID, req_type,
                                   0, NULL, 8, disable);


	   enable[0] = SPOT_BASE;
	   enable[1] = SPOT_BASE + 3;
	   enable[2] = SPOT_BASE + 4;
	   enable[3] = SPOT_BASE + 7;
	   enable[4] = SPOT_BASE + 8;
	   enable[5] = SPOT_BASE + 11;
	   enable[6] = SPOT_BASE + 12;
	   enable[7] = SPOT_BASE + 15;

	   disable[0] = SPOT_BASE + 1;
	   disable[1] = SPOT_BASE + 2;
	   disable[2] = SPOT_BASE + 5;
	   disable[3] = SPOT_BASE + 6;
	   disable[4] = SPOT_BASE + 9;
	   disable[5] = SPOT_BASE + 10;
	   disable[6] = SPOT_BASE + 13;
	   disable[7] = SPOT_BASE + 14;

           PEXSetLightSourceState (dpy, resourceID, req_type,
				   8, enable, 8, disable);

	   /*
            * Select interior attributes and draw pgons
            */
	   pgon_attrs (dpy, resourceID, req_type, 2);
	   draw_pgons (dpy, resourceID, req_type);

	describe_test( "All red and magenta spots have been enabled\n");
	describe_test( "All green and blue spots have been disabled\n");
	describe_test( "Only red and magenta polys should be lit \n");
	describe_test( "\nHit return to continue\n");

        XSync (dpy, 0);
	break;
    }
}

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "lut_lights");
    *num_images      = 61;
    *supported_modes = IMM_MODE | STR_MODE | WKS_MODE;

    strcpy(win_title, "lut_lights");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    if (cur_image == 1)
	{
        define_views (dpy, viewLUT);
        define_interiors (dpy, interiorLUT);
        define_lights (dpy, lightLUT);
	}
    }


void testproc(Display *dpy, Window window, int cur_image)
    {
    char image_name[80], test_name[80];

    sprintf(image_name,"lut_lights_%d", cur_image);
    sprintf(test_name, "lut_lights_#%d", cur_image);
    image_testproc(image_name, test_name, img_desc,
		      dpy,window, 0, 0, 
		      glob_window_width, glob_window_height);
    }
