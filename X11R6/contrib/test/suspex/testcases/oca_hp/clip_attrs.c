/* $XConsortium: clip_attrs.c,v 1.0 93/11/22 12:30:41 rws Exp $ */

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

extern void malloc_logging (int enabled);
extern void check_all_free (void);

#define X_INCREMENT   0.02
#define Y_INCREMENT   0.02

#define OFFSET_LINES                               \
    {                                              \
    line1[0].y += Y_INCREMENT;                     \
    line1[1].y += Y_INCREMENT;                     \
    line2[0].x += X_INCREMENT;                     \
    line2[1].x += X_INCREMENT;                     \
    }


void clip_lines(Display *dpy, XID resource_id, PEXOCRequestType request_type)
    {
    PEXHalfSpace halfspaces[4] = { 0.2,  0.0,  0.0,  1.0,  0.0,  0.0,
				   0.0,  0.2,  0.0,  0.0,  1.0,  0.0,
				   0.8,  0.0,  0.0, -1.0,  0.0,  0.0,
				   0.0,  0.8,  0.0,  0.0, -1.0,  0.0 };
    PEXHalfSpace2D halfspaces2D[2] = { 0.3,  0.0,  1.0,  0.0,
				       0.0,  0.3,  0.0,  1.0}; 
    PEXCoord line1[2] = {-1.0,  0.5, -1.0, 2.0, 0.5, 2.0};
    PEXCoord line2[2] = { 0.5, -1.0, -1.0, 0.5, 2.0, 2.0};

    describe_test("    XY Model Clipped, white\n");
    PEXSetModelClipFlag(dpy, resource_id, request_type, PEXClip);
    PEXSetModelClipVolume(dpy, resource_id, request_type, 
			  PEXModelClipReplace, 4, halfspaces);
    PEXSetLineColorIndex(dpy, resource_id, request_type, WHITE);
    PEXPolyline(dpy, resource_id, request_type, 2, line1);
    PEXPolyline(dpy, resource_id, request_type, 2, line2);
    OFFSET_LINES;

    describe_test("    Model Clip Disabled, red\n");
    PEXSetModelClipFlag(dpy, resource_id, request_type, PEXNoClip);
    PEXSetLineColorIndex(dpy, resource_id, request_type, RED);
    PEXPolyline(dpy, resource_id, request_type, 2, line1);
    PEXPolyline(dpy, resource_id, request_type, 2, line2);
    OFFSET_LINES;

    describe_test("    Model Clip Re-enabled, yellow\n");
    PEXSetModelClipFlag(dpy, resource_id, request_type, PEXClip);
    PEXSetLineColorIndex(dpy, resource_id, request_type, YELLOW);
    PEXPolyline(dpy, resource_id, request_type, 2, line1);
    PEXPolyline(dpy, resource_id, request_type, 2, line2);
    OFFSET_LINES;

    describe_test("    Model Clip2D Intersection, blue\n");
    PEXSetModelClipVolume2D(dpy, resource_id, request_type, 
			  PEXModelClipIntersection, 2, halfspaces2D);
    PEXSetLineColorIndex(dpy, resource_id, request_type, BLUE);
    PEXPolyline(dpy, resource_id, request_type, 2, line1);
    PEXPolyline(dpy, resource_id, request_type, 2, line2);
    OFFSET_LINES;

    describe_test("    Restore Model Clip Volume, cyan\n");
    PEXRestoreModelClipVolume(dpy, resource_id, request_type);
    PEXSetLineColorIndex(dpy, resource_id, request_type, CYAN);
    PEXPolyline(dpy, resource_id, request_type, 2, line1);
    PEXPolyline(dpy, resource_id, request_type, 2, line2);
    OFFSET_LINES;

    }

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "clip_attrs");
    *num_images      = 1;
    *supported_modes = IMM_MODE | STR_MODE | WKS_MODE;

    strcpy(win_title, "Model Clip Attrs");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    clip_lines(dpy, resourceID, req_type);
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
     image_testproc("clip_attrs", "Model Clip Attributes", img_desc,
		      dpy,window, 0, 0, 
		      glob_window_width, glob_window_height);
    }
