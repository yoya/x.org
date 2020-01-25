/* $XConsortium: marker_attrs.c,v 1.0 93/11/22 12:31:34 rws Exp $ */

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

#include <stdio.h>
#include <string.h>

#include <X11/X.h>
#include <X11/Xlibint.h>
#include <X11/Xutil.h>
#include <X11/PEX5/PEXlib.h>

#include <misc.h>

#define Y_INCREMENT         0.10
#define X_INCREMENT         0.10
#define NUM_MARKER_TYPES    5


void draw_markers(Display *dpy, XID resource_id, PEXOCRequestType request_type)
    {
    PEXColor color_red;
    PEXCoord points[2];
    int      marker_type;

    color_red.rgb.red   = 1.0;
    color_red.rgb.green = 0.0;
    color_red.rgb.blue  = 0.0;

    points[0].x = 0.1;
    points[0].y = 0.9;
    points[0].z = 0.0;
    points[1].x = 0.9;
    points[1].y = 0.9;
    points[1].z = 0.0;

    describe_test("    Default Color (indexed 1), default type (asterisk), default scale\n");
    PEXMarkers(dpy, resource_id, request_type, 1, points);
    points[0].y -= Y_INCREMENT;
    points[1].y -= Y_INCREMENT;

    describe_test("    red,  default type (asterisk), scale = 5.0\n");
    PEXSetMarkerScale(dpy, resource_id, request_type, 5.0);
    PEXSetMarkerColor(dpy, resource_id, request_type, PEXColorTypeRGB, &color_red);
    PEXMarkers(dpy, resource_id, request_type, 1, points);
    points[0].y -= Y_INCREMENT;
    points[1].y -= Y_INCREMENT;

    describe_test("    Index color #3 (yellow),  default type (asterisk), scale = 5.0\n");
    PEXSetMarkerColorIndex(dpy, resource_id, request_type, YELLOW);
    PEXMarkers(dpy, resource_id, request_type, 1, points);
    points[0].y -= Y_INCREMENT;
    points[1].y -= Y_INCREMENT;

    describe_test("    white, invalid type (should default to asterisk), scale = 5.0\n");
    PEXSetMarkerColorIndex(dpy, resource_id, request_type, WHITE);
    PEXSetMarkerType(dpy, resource_id, request_type, -999);
    PEXMarkers(dpy, resource_id, request_type, 1, points);
    points[0].y -= Y_INCREMENT;
    points[1].y -= Y_INCREMENT;

#ifdef LATER
	Get marker types via enum info
#endif
    describe_test("    red,  5 std types (.,+,*,O,X), scale = 5.0\n");
    PEXSetMarkerColorIndex(dpy, resource_id, request_type, RED);
    PEXSetMarkerScale(dpy, resource_id, request_type, 5.0);
    for (marker_type = 1; marker_type <= NUM_MARKER_TYPES; marker_type++)
	{
        PEXSetMarkerType(dpy, resource_id, request_type, marker_type);
        PEXMarkers(dpy, resource_id, request_type, 1, points);
        points[0].x += X_INCREMENT;
        points[1].x += X_INCREMENT;
	}
    points[0].y -= Y_INCREMENT;
    points[1].y -= Y_INCREMENT;

    describe_test("    default marker bundle entry (white, asterisk)\n");
    PEXSetIndividualASF(dpy, resource_id, request_type, PEXASFMarkerColor, PEXBundled);
    PEXSetIndividualASF(dpy, resource_id, request_type, PEXASFMarkerScale, PEXBundled);
    PEXSetIndividualASF(dpy, resource_id, request_type, PEXASFMarkerType,  PEXBundled);
    PEXMarkers(dpy, resource_id, request_type, 1, points);
    points[0].y -= Y_INCREMENT;
    points[1].y -= Y_INCREMENT;

    describe_test("    marker bundle entry 2 (yellow +, scale 10.0)\n");
    PEXSetMarkerBundleIndex(dpy, resource_id, request_type, 2);
    PEXMarkers(dpy, resource_id, request_type, 1, points);
    points[0].y -= Y_INCREMENT;
    points[1].y -= Y_INCREMENT;

    describe_test("    back to individual attrs (Red, X, Scale 5.0)\n");
    PEXSetIndividualASF(dpy, resource_id, request_type, PEXASFMarkerColor, PEXIndividual);
    PEXSetIndividualASF(dpy, resource_id, request_type, PEXASFMarkerScale, PEXIndividual);
    PEXSetIndividualASF(dpy, resource_id, request_type, PEXASFMarkerType,  PEXIndividual);
    PEXMarkers(dpy, resource_id, request_type, 1, points);
    points[0].y -= Y_INCREMENT;
    points[1].y -= Y_INCREMENT;

    }

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "marker_attrs");
    *num_images      = 1;
    *supported_modes = IMM_MODE | STR_MODE | WKS_MODE;

    strcpy(win_title, "Marker Attrs");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    draw_markers(dpy, resourceID, req_type);
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
     image_testproc("marker_attrs", "Marker Attributes", img_desc,
		      dpy,window, 0, 0, 
		      glob_window_width, glob_window_height);
    }
