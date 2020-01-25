/* $XConsortium: dq_attrs.c,v 1.0 93/11/22 12:30:53 rws Exp $ */

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

#define Y_INCREMENT   0.2 

void draw_lines(Display *dpy, XID resource_id, PEXOCRequestType request_type)
    {
    PEXColorRGB color_red = {1.0, 0.0, 0.0};
    PEXCoord points[2]    = {0.1, 0.7, 0.0,
			     0.9, 0.7, 1.0};

    describe_test("    Default Depth Cueing\n");
    PEXPolyline(dpy, resource_id, request_type, 2, points);
    points[0].y -= Y_INCREMENT;
    points[1].y -= Y_INCREMENT;

    describe_test("    Depth cue index 2 left to right (white to dim red)\n");
    PEXSetDepthCueIndex(dpy, resource_id, request_type, 2);
    PEXPolyline(dpy, resource_id, request_type, 2, points);
    points[0].y -= Y_INCREMENT;
    points[1].y -= Y_INCREMENT;

    describe_test("    Depth cue undefined index (should be off)\n");
    PEXSetDepthCueIndex(dpy, resource_id, request_type, 9999);
    PEXPolyline(dpy, resource_id, request_type, 2, points);
    points[0].y -= Y_INCREMENT;
    points[1].y -= Y_INCREMENT;
    }

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "dq_attrs");
    *num_images      = 1;
    *supported_modes = IMM_MODE | STR_MODE | WKS_MODE;

    strcpy(win_title, "Depth Cue Attrs");
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
     image_testproc("dq_attrs", "Depth Cue Attributes", img_desc,
		      dpy,window, 0, 0, 
		      glob_window_width, glob_window_height);
    }
