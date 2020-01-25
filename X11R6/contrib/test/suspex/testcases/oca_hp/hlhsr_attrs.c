/* $XConsortium: hlhsr_attrs.c,v 1.0 93/11/22 12:31:05 rws Exp $ */

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


#define OFFSET_VERTICAL                                        \
    {                                                          \
    int i;                                                     \
    for (i=0; i<5; i++)                                        \
	{                                                      \
        front_poly[i].y -= Y_SIZE*2.0;                         \
        back_poly[i].y  -= Y_SIZE*2.0;                         \
	}                                                      \
    }

void draw_2_polygons(Display         *dpy, 
		     XID              resource_id, 
		     PEXOCRequestType request_type,
		     PEXCoord         front_poly[5],
		     PEXCoord         back_poly[5])
    {
    PEXSetInteriorStyle(dpy, resource_id, request_type, PEXInteriorStyleSolid);
    PEXSetSurfaceColorIndex(dpy, resource_id, request_type, RED);
    PEXFillArea(dpy, resource_id, request_type, PEXShapeUnknown,
		False, 5, front_poly);
    PEXSetSurfaceColorIndex(dpy, resource_id, request_type, WHITE);
    PEXFillArea(dpy, resource_id, request_type, PEXShapeUnknown,
		False, 5, back_poly);
    }

void draw_hlhsr_image(Display         *dpy, 
		      XID              resource_id, 
		      PEXOCRequestType request_type)
    {
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

    draw_2_polygons(dpy, resource_id, request_type, front_poly, back_poly);

    OFFSET_VERTICAL;
    PEXSetHLHSRID(dpy, resource_id, request_type, PEXHLHSRIDEnable);
    draw_2_polygons(dpy, resource_id, request_type, front_poly, back_poly);

    OFFSET_VERTICAL;
    PEXSetHLHSRID(dpy, resource_id, request_type, PEXHLHSRIDDisable);
    draw_2_polygons(dpy, resource_id, request_type, front_poly, back_poly);

    OFFSET_VERTICAL;
    PEXSetHLHSRID(dpy, resource_id, request_type, PEXHLHSRIDEnable);
    draw_2_polygons(dpy, resource_id, request_type, front_poly, back_poly);
    }


void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "hlhsr_attrs");
    *num_images      = 3;
    *supported_modes = IMM_MODE | STR_MODE | WKS_MODE;

    strcpy(win_title, "HLHSR Attrs");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    PEXRendererAttributes rdr_attrs;
    unsigned long         value_mask;

    switch (cur_image)
	{
	case 1:
        rdr_attrs.hlhsr_mode = PEXHLHSRZBuffer;
        value_mask = PEXRAHLHSRMode;
        PEXChangeRenderer (dpy, renderer, value_mask, &rdr_attrs);
        describe_test("\nImage 1\n");
        describe_test("    HLHSR Mode ZBuffer, HLHSR ID default, on, off, on\n");
	break;

	case 2:
        rdr_attrs.hlhsr_mode = PEXHLHSROff;
        value_mask = PEXRAHLHSRMode;
        PEXChangeRenderer (dpy, renderer, value_mask, &rdr_attrs);
        describe_test("\nImage 2\n");
        describe_test("    HLHSR Mode off, HLHSR ID default, on, off, on\n");
	break;

	case 3:
        rdr_attrs.hlhsr_mode = PEXHLHSRZBufferID;
        value_mask = PEXRAHLHSRMode;
        PEXChangeRenderer (dpy, renderer, value_mask, &rdr_attrs);
        describe_test("\nImage 3\n");
        describe_test("    HLHSR Mode ZBufferID, HLHSR ID default, on, off, on\n");
	break;

	default:
	_hppex_stderr_print("Invalid current image passed to misc_setup\n");
        }
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    draw_hlhsr_image(dpy, resourceID, req_type);
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
    char image_name[80], test_name[80];

    sprintf(image_name,"hlhsr_attrs_%d", cur_image);
    sprintf(test_name, "HLHSR attributes #%d", cur_image);
    image_testproc(image_name, test_name, img_desc,
		      dpy,window, 0, 0, 
		      glob_window_width, glob_window_height);
    }
