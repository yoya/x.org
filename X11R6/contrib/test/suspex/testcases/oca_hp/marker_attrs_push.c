/* $XConsortium: marker_attrs_push.c,v 1.0 93/11/22 12:31:39 rws Exp $ */

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

#define Y_INCREMENT   0.15


PEXCoord points[1]    = {0.47, 0.9, 0.0};

void setup_attrs1(Display *dpy, XID resource_id, PEXOCRequestType request_type)
    {
    PEXSetMarkerColorIndex  (dpy, resource_id, request_type, RED);
    PEXSetMarkerScale       (dpy, resource_id, request_type, 3.0);
    PEXSetMarkerType        (dpy, resource_id, request_type, PEXMarkerX);
    PEXSetMarkerBundleIndex (dpy, resource_id, request_type, 1);
    }

void setup_attrs2(Display *dpy, XID resource_id, PEXOCRequestType request_type)
    {
    PEXSetMarkerColorIndex  (dpy, resource_id, request_type, GREEN);
    PEXSetMarkerScale       (dpy, resource_id, request_type, 6.0);
    PEXSetMarkerType        (dpy, resource_id, request_type, PEXMarkerCircle);
    PEXSetMarkerBundleIndex (dpy, resource_id, request_type, 2);
    }

void draw_markers(Display *dpy, XID resource_id, PEXOCRequestType request_type)
    {
    PEXMarkers(dpy, resource_id, request_type, 1, points);
    points[0].y -= Y_INCREMENT;

    PEXSetIndividualASF(dpy, resource_id, request_type, PEXASFMarkerColor, 
			PEXBundled);
    PEXSetIndividualASF(dpy, resource_id, request_type, PEXASFMarkerScale, 
			PEXBundled);
    PEXSetIndividualASF(dpy, resource_id, request_type, PEXASFMarkerType,  
			PEXBundled);
    PEXMarkers(dpy, resource_id, request_type, 1, points);
    points[0].y -= Y_INCREMENT;
    PEXSetIndividualASF(dpy, resource_id, request_type, PEXASFMarkerColor, 
			PEXIndividual);
    PEXSetIndividualASF(dpy, resource_id, request_type, PEXASFMarkerScale, 
			PEXIndividual);
    PEXSetIndividualASF(dpy, resource_id, request_type, PEXASFMarkerType,  
			PEXIndividual);
    }

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "marker_attrs_push");
    *num_images      = 1;
    *supported_modes = IMM_MODE | STR_MODE | WKS_MODE;

    strcpy(win_title, "Model Attrs Push/Pop");
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
    describe_test("    Red X Scale 3.0\n");
    describe_test("    Default bundle (asterisk scale 1.0)\n");
    describe_test("  Execute Structure\n");
    describe_test("    Green Circle Scale 6.0\n");
    describe_test("    Bundle index 2 (Yellow Cross scale 10.0)\n");
    describe_test("  After pop_state\n");
    describe_test("    Red X Scale 3.0\n");
    describe_test("    Default bundle (asterisk scale 1.0)\n");
 
    setup_attrs1 (dpy, resourceID, req_type);
    draw_markers (dpy, resourceID, req_type);

    setup_attrs2(dpy, sid, PEXOCStore);
    draw_markers(dpy, sid, PEXOCStore);
    PEXExecuteStructure (dpy, resourceID, req_type, sid);

    draw_markers(dpy, resourceID, req_type);
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
     image_testproc("marker_attrs_push", "Model Attrs Push/Pop", img_desc,
		      dpy,window, 0, 0, 
		      glob_window_width, glob_window_height);
    }
