/* $XConsortium: misc_attrs.c,v 1.0 93/11/22 12:31:45 rws Exp $ */

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

#define Y_INCREMENT   0.07
#define X_INCREMENT   0.15


#define OFFSET_HORIZONTAL                                      \
    {                                                          \
    int i;                                                     \
    for (i=0; i<5; i++)                                        \
	{                                                      \
        fill_points[i].x += X_INCREMENT;                        \
	}                                                      \
    }

void misc_attrs(Display *dpy, XID resource_id, PEXOCRequestType request_type)
    {
    PEXCoord points[2]    = {0.1, 0.9, 0.0,
			     0.9, 0.9, 0.0};
    PEXCoord2D fill_points[5] =  {0.1, 0.2, 0.2, 0.2,
				  0.2, 0.3, 0.1, 0.3,
				  0.1, 0.2};
    PEXCoord ref_point;
    PEXVector vector1, vector2;
    PEXCoord2D ref_point2D;

    describe_test("    PEXRenderingColorModelRGB (Red Line)\n");
    PEXSetRenderingColorModel(dpy, resource_id, request_type, 
			      PEXRenderingColorModelRGB);
    PEXSetLineColorIndex(dpy, resource_id, request_type, RED);
    PEXPolyline(dpy, resource_id, request_type, 2, points);
    points[0].y -= Y_INCREMENT;
    points[1].y -= Y_INCREMENT;

    describe_test("    PEXRenderingColorModelCIE (Yellow Line)\n");
    PEXSetRenderingColorModel(dpy, resource_id, request_type, 
			      PEXRenderingColorModelCIE);
    PEXSetLineColorIndex(dpy, resource_id, request_type, YELLOW);
    PEXPolyline(dpy, resource_id, request_type, 2, points);
    points[0].y -= Y_INCREMENT;
    points[1].y -= Y_INCREMENT;

    describe_test("    PEXRenderingColorModelHSV (Blue Line)\n");
    PEXSetRenderingColorModel(dpy, resource_id, request_type, 
			      PEXRenderingColorModelHSV);
    PEXSetLineColorIndex(dpy, resource_id, request_type, BLUE);
    PEXPolyline(dpy, resource_id, request_type, 2, points);
    points[0].y -= Y_INCREMENT;
    points[1].y -= Y_INCREMENT;
    
    describe_test("    PEXRenderingColorModelHLS (Green Line)\n");
    PEXSetRenderingColorModel(dpy, resource_id, request_type, 
			      PEXRenderingColorModelHLS);
    PEXSetLineColorIndex(dpy, resource_id, request_type, GREEN);
    PEXPolyline(dpy, resource_id, request_type, 2, points);
    points[0].y -= Y_INCREMENT;
    points[1].y -= Y_INCREMENT;
    
    describe_test("    Invalid Color Model (Cyan Line)\n");
    PEXSetRenderingColorModel(dpy, resource_id, request_type, 
			      -99);
    PEXSetLineColorIndex(dpy, resource_id, request_type, CYAN);
    PEXPolyline(dpy, resource_id, request_type, 2, points);
    points[0].y -= Y_INCREMENT;
    points[1].y -= Y_INCREMENT;

    describe_test("    SetInteriorStyle to Pattern (should default to hollow)\n");
    PEXSetInteriorStyle(dpy, resource_id, request_type, 
			PEXInteriorStylePattern);

    describe_test("    SetPatternAttributes (green hollow square)\n");
    PEXSetPatternAttributes(dpy, resource_id, request_type, 
			    &ref_point, &vector1, &vector2);
    PEXSetSurfaceColorIndex(dpy, resource_id, request_type, GREEN);
    PEXFillArea2D(dpy, resource_id, request_type, PEXShapeUnknown,
		False, 5, fill_points);
    OFFSET_HORIZONTAL;
			  
    describe_test("    SetPatternAttributes2D (white hollow square)\n");
    PEXSetPatternAttributes2D(dpy, resource_id, request_type, 
			    &ref_point2D);
    PEXSetSurfaceColorIndex(dpy, resource_id, request_type, WHITE);
    PEXFillArea2D(dpy, resource_id, request_type, PEXShapeUnknown,
		False, 5, fill_points);
    OFFSET_HORIZONTAL;
			  
    describe_test("    SetPatternSize (red hollow square)\n");
    PEXSetPatternSize(dpy, resource_id, request_type, 1.0, 1.0);
    PEXSetSurfaceColorIndex(dpy, resource_id, request_type, RED);
    PEXFillArea2D(dpy, resource_id, request_type, PEXShapeUnknown,
		False, 5, fill_points);
    OFFSET_HORIZONTAL;
    }

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "misc_attrs");
    *num_images      = 1;
    *supported_modes = IMM_MODE | STR_MODE | WKS_MODE;

    strcpy(win_title, "Misc Attrs");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    misc_attrs(dpy, resourceID, req_type);
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
     image_testproc("misc_attrs", "Misc Attributes", img_desc,
		      dpy,window, 0, 0, 
		      glob_window_width, glob_window_height);
    }
