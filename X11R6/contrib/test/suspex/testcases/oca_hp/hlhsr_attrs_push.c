/* $XConsortium: hlhsr_attrs_push.c,v 1.0 93/11/22 12:31:10 rws Exp $ */

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

#define X_SIZE        0.12
#define Y_SIZE        0.12
#define X_FRONT       (0.5 - X_SIZE)
#define Y_FRONT       0.95
#define X_BACK        (X_FRONT + X_SIZE/2.0)
#define Y_BACK        (Y_FRONT - Y_SIZE/2.0)

PEXCoord front_poly[5] = {X_FRONT,         Y_FRONT,         0.5,
                          X_FRONT,        (Y_FRONT-Y_SIZE), 0.5,
     	                 (X_FRONT+X_SIZE),(Y_FRONT-Y_SIZE), 0.5, 
			 (X_FRONT+X_SIZE), Y_FRONT,         0.5,
                          X_FRONT,         Y_FRONT,         0.5};
PEXCoord back_poly[5]  = {X_BACK ,         Y_BACK,          0.0,
			 (X_BACK +X_SIZE), Y_BACK,          0.0,
			 (X_BACK +X_SIZE),(Y_BACK-Y_SIZE),  0.0,
                          X_BACK ,        (Y_BACK-Y_SIZE),  0.0,
                          X_BACK ,         Y_BACK,          0.0};


#define OFFSET_VERTICAL                                        \
    {                                                          \
    int i;                                                     \
    for (i=0; i<5; i++)                                        \
	{                                                      \
        front_poly[i].y -= Y_SIZE*2.5;                         \
        back_poly[i].y  -= Y_SIZE*2.5;                         \
	}                                                      \
    }

void setup_attrs1(Display *dpy, XID resource_id, PEXOCRequestType request_type)
    {
    PEXSetHLHSRID(dpy, resource_id, request_type, PEXHLHSRIDEnable);
    }

void setup_attrs2(Display *dpy, XID resource_id, PEXOCRequestType request_type)
    {
    PEXSetHLHSRID(dpy, resource_id, request_type, PEXHLHSRIDDisable);
    }

void draw_2_polygons(Display         *dpy, 
		     XID              resource_id, 
		     PEXOCRequestType request_type)
    {
    PEXSetInteriorStyle(dpy, resource_id, request_type, PEXInteriorStyleSolid);
    PEXSetSurfaceColorIndex(dpy, resource_id, request_type, RED);
    PEXFillArea(dpy, resource_id, request_type, PEXShapeUnknown,
		False, 5, front_poly);
    PEXSetSurfaceColorIndex(dpy, resource_id, request_type, WHITE);
    PEXFillArea(dpy, resource_id, request_type, PEXShapeUnknown,
		False, 5, back_poly);
    OFFSET_VERTICAL;
    }

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "hlhsr_attrs_push");
    *num_images      = 1;
    *supported_modes = IMM_MODE | STR_MODE | WKS_MODE;

    strcpy(win_title, "HLHSR Clip Push/Pop");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    PEXRendererAttributes rdr_attrs;
    unsigned long         value_mask;

    rdr_attrs.hlhsr_mode = PEXHLHSRZBufferID;
    value_mask = PEXRAHLHSRMode;
    PEXChangeRenderer (dpy, renderer, value_mask, &rdr_attrs);
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
    describe_test("  Immediate mode\n");
    describe_test("    Zbuffer ID = enable\n");
    describe_test("  Execute Structure\n");
    describe_test("    Zbuffer ID = disable\n");
    describe_test("  After pop_state\n");
    describe_test("    Zbuffer ID = enable\n");

    setup_attrs1(dpy, resourceID, req_type);
    draw_2_polygons (dpy, resourceID, req_type);

    setup_attrs2(dpy, sid, PEXOCStore);
    draw_2_polygons(dpy, sid, PEXOCStore);
    PEXExecuteStructure (dpy, resourceID, req_type, sid);

    draw_2_polygons(dpy, resourceID, req_type);
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
     image_testproc("hlhsr_attrs_push", "HLHSR Attributes Push/Pop", img_desc,
		      dpy,window, 0, 0, 
		      glob_window_width, glob_window_height);
    }
