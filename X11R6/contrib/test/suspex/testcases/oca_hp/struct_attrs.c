/* $XConsortium: struct_attrs.c,v 1.0 93/11/22 12:31:58 rws Exp $ */

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

#define Y_INCREMENT        0.07
#define X_INCREMENT        0.15
#define DUMMY_DATA_SIZE 1024
#define DUMMY_GSE_ID       1

void struct_attrs(Display *dpy, XID resource_id, PEXOCRequestType request_type)
    {
    char buf[DUMMY_DATA_SIZE];
    PEXCoord points[2]    = {0.1, 0.9, 0.0,
			     0.9, 0.9, 0.0};

    describe_test("    PEXApplicationData (Red Line)\n");
    PEXApplicationData(dpy, resource_id, request_type, 
		       sizeof(buf), buf);
    PEXSetLineColorIndex(dpy, resource_id, request_type, RED);
    PEXPolyline(dpy, resource_id, request_type, 2, points);
    points[0].y -= Y_INCREMENT;
    points[1].y -= Y_INCREMENT;

    describe_test("    PEXGSE (White Line)\n");
    PEXGSE(dpy, resource_id, request_type, DUMMY_GSE_ID, sizeof(buf), buf);
    PEXSetLineColorIndex(dpy, resource_id, request_type, WHITE);
    PEXPolyline(dpy, resource_id, request_type, 2, points);
    points[0].y -= Y_INCREMENT;
    points[1].y -= Y_INCREMENT;

    describe_test("    PEXNoop (Blue Line)\n");
    PEXNoop(dpy, resource_id, request_type);
    PEXSetLineColorIndex(dpy, resource_id, request_type, BLUE);
    PEXPolyline(dpy, resource_id, request_type, 2, points);
    points[0].y -= Y_INCREMENT;
    points[1].y -= Y_INCREMENT;

    }

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "struct_attrs");
    *num_images      = 1;
    *supported_modes = IMM_MODE | STR_MODE | WKS_MODE;

    strcpy(win_title, "Struct Attrs");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    struct_attrs(dpy, resourceID, req_type);
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
     image_testproc("struct_attrs", "Struct Attributes", img_desc,
		      dpy,window, 0, 0, 
		      glob_window_width, glob_window_height);
    }
