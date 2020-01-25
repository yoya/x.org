
/* $XConsortium: xform_attrs.c,v 1.0 93/11/22 12:32:16 rws Exp $ */


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

#define Y_DECREMENT         0.10


void display_xforms(Display *dpy, XID resource_id, PEXOCRequestType request_type)
    {
    PEXMatrix3x3 xform2d;
    PEXMatrix    xform3d;
    PEXVector    xlate_vect = {0.0, 0.9, 0.0};
    PEXVector    scale_vect = {0.5, 0.5, 0.5};
    PEXCoord     points[2] = {0.1, 0.0, 0.0, 0.9, 0.0, 0.0};

    describe_test("    Set Global Transform (translate)\n");
    describe_test("      white line should be: x=0.1-0.9, y=0.9\n");
    PEXIdentityMatrix(xform3d);
    PEXTranslate(&xlate_vect, xform3d);
    PEXSetGlobalTransform(dpy, resource_id, request_type, xform3d);
    PEXPolyline(dpy, resource_id, request_type, 2, points);

    describe_test("    Set Global Transform 2D (translate, y=0.8)\n");
    describe_test("      white line should be: x=0.1-0.9, y=0.8\n");
    PEXIdentityMatrix2D(xform2d);
    xlate_vect.y -= Y_DECREMENT; 
    PEXTranslate2D( (PEXVector2D *) &xlate_vect, xform2d);
    PEXSetGlobalTransform2D(dpy, resource_id, request_type, xform2d);
    PEXPolyline(dpy, resource_id, request_type, 2, points);

    describe_test("    Set Local Transform2D (translate, y=0.7, PEXReplace)\n");
    describe_test("      white line should be: x=0.1-0.9, y=0.7\n");
    PEXIdentityMatrix(xform3d);
    PEXSetGlobalTransform(dpy, resource_id, request_type, xform3d);
    PEXIdentityMatrix2D(xform2d);
    xlate_vect.y -= Y_DECREMENT; 
    PEXTranslate2D( (PEXVector2D *) &xlate_vect, xform2d);
    PEXSetLocalTransform2D(dpy, resource_id, request_type,
			   PEXReplace, xform2d);
    PEXPolyline(dpy, resource_id, request_type, 2, points);

    describe_test("    Set Local Transform (scale, translate, PEXPostConcatenate)\n");
    describe_test("      red line should be: x=0.55-0.95, y=0.5\n");
    PEXIdentityMatrix(xform3d);
    PEXScale(&scale_vect, xform3d);
    PEXSetLocalTransform(dpy, resource_id, request_type,
			 PEXReplace, xform3d);
    PEXIdentityMatrix(xform3d);
    xlate_vect.x = 0.5; 
    xlate_vect.y = 0.5; 
    PEXTranslate(&xlate_vect, xform3d);
    PEXSetLocalTransform(dpy, resource_id, request_type,
			 PEXPostConcatenate, xform3d);
    PEXSetLineColorIndex(dpy, resource_id, request_type, RED);
    PEXPolyline(dpy, resource_id, request_type, 2, points);

    describe_test("    Set Local Transform (scale, translate, PEXPreConcatenate)\n");
    describe_test("      yellow line should be: x=0.25-0.75, y=0.25\n");
    PEXIdentityMatrix(xform3d);
    PEXScale(&scale_vect, xform3d);
    PEXSetLocalTransform(dpy, resource_id, request_type,
			 PEXReplace, xform3d);
    PEXIdentityMatrix(xform3d);
    PEXTranslate(&xlate_vect, xform3d);
    PEXSetLocalTransform(dpy, resource_id, request_type,
			 PEXPreConcatenate, xform3d);
    PEXSetLineColorIndex(dpy, resource_id, request_type, YELLOW);
    PEXPolyline(dpy, resource_id, request_type, 2, points);

    }

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "xform_attrs");
    *num_images      = 1;
    *supported_modes = IMM_MODE | STR_MODE | WKS_MODE;

    strcpy(win_title, "Xform Attrs");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    display_xforms(dpy, resourceID, req_type);
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
    image_testproc("xform_attrs", "Transofrmation Attributes", img_desc,
		      dpy,window, 0, 0, 
		      glob_window_width, glob_window_height);
    }
