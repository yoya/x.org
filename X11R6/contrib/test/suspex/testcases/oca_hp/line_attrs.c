/* $XConsortium: line_attrs.c,v 1.0 93/11/22 12:31:22 rws Exp $ */

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

#define HPPEXLineTypeCenter             0x80070001
#define HPPEXLineTypePhantom            0x80070002

#include <misc.h>

#define Y_INCREMENT   0.07

void draw_lines(Display *dpy, XID resource_id, PEXOCRequestType request_type)
    {
    PEXColorRGB color_red = {1.0, 0.0, 0.0};
    PEXCoord points[2]    = {0.1, 0.9, 0.0,
			     0.9, 0.9, 0.0};
    PEXListOfVertex  v_list;
    PEXVertexRGB v_data[2];

    v_list.count          = 2;
    v_list.vertices.rgb   = v_data;

    describe_test("    Default Color (indexed 1), default type (solid)\n");
    PEXPolyline(dpy, resource_id, request_type, 2, points);
    points[0].y -= Y_INCREMENT;
    points[1].y -= Y_INCREMENT;

    describe_test("    red,  dashed\n");
    PEXSetLineColor(dpy, resource_id, request_type, PEXColorTypeRGB, 
		   (PEXColor *) &color_red);
    PEXSetLineType (dpy, resource_id, request_type, PEXLineTypeDashed);
    PEXPolyline(dpy, resource_id, request_type, 2, points);
    points[0].y -= Y_INCREMENT;
    points[1].y -= Y_INCREMENT;

    describe_test("    Index color #3 (yellow),  dashed\n");
    PEXSetLineColorIndex(dpy, resource_id, request_type, YELLOW);
    PEXSetLineType (dpy, resource_id, request_type, PEXLineTypeDashed);
    PEXPolyline(dpy, resource_id, request_type, 2, points);
    points[0].y -= Y_INCREMENT;
    points[1].y -= Y_INCREMENT;

    describe_test("    white,  dotted\n");
    PEXSetLineColorIndex(dpy, resource_id, request_type, WHITE);
    PEXSetLineType (dpy, resource_id, request_type, PEXLineTypeDotted);
    PEXPolyline(dpy, resource_id, request_type, 2, points);
    points[0].y -= Y_INCREMENT;
    points[1].y -= Y_INCREMENT;

    describe_test("    white,  dash-dot\n");
    PEXSetLineType (dpy, resource_id, request_type, PEXLineTypeDashDot);
    PEXPolyline(dpy, resource_id, request_type, 2, points);
    points[0].y -= Y_INCREMENT;
    points[1].y -= Y_INCREMENT;

    describe_test("    white,  HP_CenterLine\n");
    PEXSetLineType (dpy, resource_id, request_type, HPPEXLineTypeCenter);
    PEXPolyline(dpy, resource_id, request_type, 2, points);
    points[0].y -= Y_INCREMENT;
    points[1].y -= Y_INCREMENT;

    describe_test("    white,  HP_Phantom\n");
    PEXSetLineType (dpy, resource_id, request_type, HPPEXLineTypePhantom);
    PEXPolyline(dpy, resource_id, request_type, 2, points);
    points[0].y -= Y_INCREMENT;
    points[1].y -= Y_INCREMENT;

    describe_test("    white,  Invalid type (should default to solid)\n");
    PEXSetLineType (dpy, resource_id, request_type, -99);
    PEXPolyline(dpy, resource_id, request_type, 2, points);
    points[0].y -= Y_INCREMENT;
    points[1].y -= Y_INCREMENT;

    describe_test("    red,  solid (Attempt to set wide line)\n");
    PEXSetLineColorIndex(dpy, resource_id, request_type, RED);
    PEXSetLineType  (dpy, resource_id, request_type, PEXLineTypeSolid);
    PEXSetLineWidth (dpy, resource_id, request_type, 2.0);
    PEXPolyline(dpy, resource_id, request_type, 2, points);
    points[0].y -= Y_INCREMENT;
    points[1].y -= Y_INCREMENT;

    describe_test("    black-white interpolated,  solid\n");
    PEXSetPolylineInterpMethod(dpy, resource_id, request_type, 
			       PEXPolylineInterpColor);
    PEXSetLineType (dpy, resource_id, request_type, PEXLineTypeSolid);
    v_data[0].point.x   = points[0].x;
    v_data[0].point.y   = points[0].y;
    v_data[0].point.z   = points[0].z;
    v_data[0].rgb.red   = 0.0;
    v_data[0].rgb.green = 0.0;
    v_data[0].rgb.blue  = 0.0;
    v_data[1].point.x   = points[1].x;
    v_data[1].point.y   = points[1].y;
    v_data[1].point.z   = points[1].z;
    v_data[1].rgb.red   = 1.0;
    v_data[1].rgb.green = 1.0;
    v_data[1].rgb.blue  = 1.0;
    PEXPolylineSetWithData(dpy, resource_id, request_type, PEXGAColor,
			   PEXColorTypeRGB, 1, &v_list);
    PEXSetPolylineInterpMethod(dpy, resource_id, request_type, 
			       PEXPolylineInterpNone);
    points[0].y -= Y_INCREMENT;
    points[1].y -= Y_INCREMENT;

    describe_test("    default line bundle entry (white, solid)\n");
    PEXSetIndividualASF(dpy, resource_id, request_type, PEXASFLineColor, PEXBundled);
    PEXSetIndividualASF(dpy, resource_id, request_type, PEXASFLineType,  PEXBundled);
    PEXSetIndividualASF(dpy, resource_id, request_type, PEXASFLineWidth, PEXBundled);
    PEXPolyline(dpy, resource_id, request_type, 2, points);
    points[0].y -= Y_INCREMENT;
    points[1].y -= Y_INCREMENT;

    describe_test("    line bundle entry 2 (yellow, dotted, width 2)\n");
    PEXSetLineBundleIndex(dpy, resource_id, request_type, 2);
    PEXPolyline(dpy, resource_id, request_type, 2, points);
    points[0].y -= Y_INCREMENT;
    points[1].y -= Y_INCREMENT;

    describe_test("    back to individual attrs (red, solid)\n");
    PEXSetIndividualASF(dpy, resource_id, request_type, PEXASFLineColor, PEXIndividual);
    PEXSetIndividualASF(dpy, resource_id, request_type, PEXASFLineType,  PEXIndividual);
    PEXSetIndividualASF(dpy, resource_id, request_type, PEXASFLineWidth, PEXIndividual);
    PEXPolyline(dpy, resource_id, request_type, 2, points);
    points[0].y -= Y_INCREMENT;
    points[1].y -= Y_INCREMENT;

    }


void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "line_attrs");
    *num_images      = 1;
    *supported_modes = IMM_MODE | STR_MODE | WKS_MODE;

    strcpy(win_title, "Line Attrs");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    draw_lines(dpy, resourceID, req_type);
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
     image_testproc("line_attrs", "Line Attributes", img_desc,
		      dpy,window, 0, 0, 
		      glob_window_width, glob_window_height);
    }
