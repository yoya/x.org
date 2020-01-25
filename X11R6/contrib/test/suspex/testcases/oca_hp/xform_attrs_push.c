/* $XConsortium: xform_attrs_push.c,v 1.0 93/11/22 12:32:21 rws Exp $ */

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


void setup_attrs1(Display *dpy, XID resource_id, PEXOCRequestType request_type)
    {
    PEXMatrix    xform3d;
    PEXVector    xlate_vect = {0.25, 0.6, 0.0};
    PEXVector    scale_vect = {0.5,  0.5, 1.0};

    PEXTranslate(&xlate_vect, xform3d);
    PEXSetGlobalTransform(dpy, resource_id, request_type, xform3d);

    PEXScale(&scale_vect, xform3d);
    PEXSetLocalTransform(dpy, resource_id, request_type,
			 PEXReplace, xform3d);
    }

void setup_attrs2(Display *dpy, XID resource_id, PEXOCRequestType request_type)
    {
    PEXMatrix    xform3d;
    PEXVector    xlate_vect = {0.0, 0.9, 0.0};
    PEXVector    scale_vect = {1.0, 1.0, 1.0};

    PEXTranslate(&xlate_vect, xform3d);
    PEXSetGlobalTransform(dpy, resource_id, request_type, xform3d);

    PEXScale(&scale_vect, xform3d);
    PEXSetLocalTransform(dpy, resource_id, request_type,
			 PEXReplace, xform3d);
    }

void draw_line(Display *dpy, XID resource_id, PEXOCRequestType request_type)
    {
    PEXCoord points[2] = {0.1, 0.0, 0.0,
			  0.9, 0.0, 0.0};

    PEXPolyline(dpy, resource_id, request_type, 2, points);
    }



void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "xform_attrs_push");
    *num_images      = 1;
    *supported_modes = IMM_MODE | STR_MODE | WKS_MODE;

    strcpy(win_title, "Xform Attrs Push/Pop");
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
    describe_test("    Global Translate y=0.6, local scale 0.5\n");
    describe_test("  Execute Structure\n");
    describe_test("    Global Translate y=0.9, local scale 1.0\n");
    describe_test("  After pop_state\n");
    describe_test("    Global Translate y=0.6, local scale 0.5\n");
    describe_test("    (should overrite bottom line with red dashed)\n");
	
    setup_attrs1(dpy, resourceID, req_type);
    draw_line (dpy, resourceID, req_type);
    
    setup_attrs2(dpy, sid, PEXOCStore);
    draw_line(dpy, sid, PEXOCStore);
    PEXExecuteStructure (dpy, resourceID, req_type, sid);
    
    PEXSetLineType (dpy, resourceID, req_type, PEXLineTypeDashed);
    PEXSetLineColorIndex(dpy, resourceID, req_type, RED);
    draw_line(dpy, resourceID, req_type);
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
    image_testproc("xform_attrs_push", "Xform Attrs Push/Pop", img_desc,
		      dpy,window, 0, 0, 
		      glob_window_width, glob_window_height);
    }
