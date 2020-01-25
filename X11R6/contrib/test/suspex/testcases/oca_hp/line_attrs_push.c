/* $XConsortium: line_attrs_push.c,v 1.0 93/11/22 12:31:28 rws Exp $ */

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

#define Y_INCREMENT   0.1

#define LINE_WIDTH_1  2.0
#define LINE_WIDTH_2  4.0

#include <misc.h>

PEXCoord points[2]    = {0.1, 0.9, 0.0,
			 0.9, 0.9, 0.0};

void setup_attrs1(Display *dpy, XID resource_id, PEXOCRequestType request_type)
    {
    PEXSetLineColorIndex (dpy, resource_id, request_type, RED);
    PEXSetLineType       (dpy, resource_id, request_type, PEXLineTypeDashed);
    PEXSetLineWidth      (dpy, resource_id, request_type, LINE_WIDTH_1);
    PEXSetLineBundleIndex(dpy, resource_id, request_type, 1);
    PEXSetPolylineInterpMethod(dpy, resource_id, request_type, 
			       PEXPolylineInterpNone);
    }

void setup_attrs2(Display *dpy, XID resource_id, PEXOCRequestType request_type)
    {
    PEXSetLineColorIndex (dpy, resource_id, request_type, GREEN);
    PEXSetLineType       (dpy, resource_id, request_type, PEXLineTypeSolid);
    PEXSetLineWidth      (dpy, resource_id, request_type, LINE_WIDTH_2);
    PEXSetLineBundleIndex(dpy, resource_id, request_type, 2);
    PEXSetPolylineInterpMethod(dpy, resource_id, request_type, 
			       PEXPolylineInterpColor);
    }

void draw_lines(Display *dpy, XID resource_id, PEXOCRequestType request_type)
    {
    PEXListOfVertex  v_list;
    PEXVertexRGB v_data[2];

    v_list.count          = 2;
    v_list.vertices.rgb   = v_data;

    PEXPolyline(dpy, resource_id, request_type, 2, points);
    points[0].y -= Y_INCREMENT;
    points[1].y -= Y_INCREMENT;

    v_data[0].point.x   = points[0].x;
    v_data[0].point.y   = points[0].y;
    v_data[0].point.z   = points[0].z;
    v_data[0].rgb.red   = 0.0;
    v_data[0].rgb.green = 0.0;
    v_data[0].rgb.blue  = 1.0;
    v_data[1].point.x   = points[1].x;
    v_data[1].point.y   = points[1].y;
    v_data[1].point.z   = points[1].z;
    v_data[1].rgb.red   = 1.0;
    v_data[1].rgb.green = 1.0;
    v_data[1].rgb.blue  = 1.0;
    PEXPolylineSetWithData(dpy, resource_id, request_type, PEXGAColor,
			   PEXColorTypeRGB, 1, &v_list);
    points[0].y -= Y_INCREMENT;
    points[1].y -= Y_INCREMENT;

    PEXSetIndividualASF(dpy, resource_id, request_type, PEXASFLineColor, 
			PEXBundled);
    PEXSetIndividualASF(dpy, resource_id, request_type, PEXASFLineType,  
			PEXBundled);
    PEXSetIndividualASF(dpy, resource_id, request_type, PEXASFLineWidth, 
			PEXBundled);
    PEXPolyline(dpy, resource_id, request_type, 2, points);
    points[0].y -= Y_INCREMENT;
    points[1].y -= Y_INCREMENT;
    PEXSetIndividualASF(dpy, resource_id, request_type, PEXASFLineColor, 
			PEXIndividual);
    PEXSetIndividualASF(dpy, resource_id, request_type, PEXASFLineType,  
			PEXIndividual);
    PEXSetIndividualASF(dpy, resource_id, request_type, PEXASFLineWidth, 
			PEXIndividual);
    }

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "line_attrs_push");
    *num_images      = 1;
    *supported_modes = IMM_MODE | STR_MODE | WKS_MODE;

    strcpy(win_title, "Line Attrs Push/Pop");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    PEXStructure sid;

    sid = PEXCreateStructure(dpy);
    if (sid == None)
        {
        _hppex_stderr_print("PROCESS ERROR:  create_structure failed\n");
	exit (1);
        }
    
    describe_test("\nImage 1\n");
    describe_test("    red dashed\n");
    describe_test("    with data (blue dashed)\n");
    describe_test("    bundle index 1 (white solid)\n");
    describe_test("  Execute Structure\n");
    describe_test("    green solid\n");
    describe_test("    with data (blue to white interpolated)\n");
    describe_test("    bundle index 2 (yellow, dotted, width 2.0)\n");
    describe_test("  After pop_state\n");
    describe_test("    red dashed\n");
    describe_test("    with data (blue dashed)\n");
    describe_test("    bundle index 1 (white solid)\n");
    
    setup_attrs1(dpy, resourceID, req_type);
    draw_lines (dpy, resourceID, req_type);
    
    setup_attrs2(dpy, sid, PEXOCStore);
    draw_lines(dpy, sid, PEXOCStore);
    PEXExecuteStructure (dpy, resourceID, req_type, sid);

    draw_lines(dpy, resourceID, req_type);
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
     image_testproc("line_attrs_push", "Line Attrs Push/Pop", img_desc,
		      dpy,window, 0, 0, 
		      glob_window_width, glob_window_height);
    }
