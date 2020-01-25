/* $XConsortium: pexplc-1.c,v 1.0 93/11/22 12:38:25 rws Exp $ */

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

#include <X11/PEX5/PEXlib.h>
#include <misc.h>

/* 
 * PEXlib Pipeline Context Module:  Test Case 1  --  Initialization
 *
 */

test_image(Display         *dpy, 
	   XID              resourceID, 
	   PEXOCRequestType req_type)
    {
      PEXCoord				points[5], mpoints[5], fpoints[5];

      /* Test the initialization of the default Pipeline Context by */
      /* exercising the Renderer's pointer to it.                   */

      /* Set up some data. */

      points[0].x = 0.1; points[0].y = 0.1; points[0].z = 0.0;
      points[1].x = 0.9; points[1].y = 0.1; points[1].z = 0.0;
      points[2].x = 0.9; points[2].y = 0.9; points[2].z = 0.0;
      points[3].x = 0.1; points[3].y = 0.9; points[3].z = 0.0;
      points[4].x = 0.1; points[4].y = 0.1; points[4].z = 0.0;

      mpoints[0].x = 0.3; mpoints[0].y = 0.3; mpoints[0].z = 0.0;
      mpoints[1].x = 0.7; mpoints[1].y = 0.3; mpoints[1].z = 0.0;
      mpoints[2].x = 0.7; mpoints[2].y = 0.7; mpoints[2].z = 0.0;
      mpoints[3].x = 0.3; mpoints[3].y = 0.7; mpoints[3].z = 0.0;
      mpoints[4].x = 0.3; mpoints[4].y = 0.3; mpoints[4].z = 0.0;

      fpoints[0].x = 0.4; fpoints[0].y = 0.4; fpoints[0].z = 0.0;
      fpoints[1].x = 0.6; fpoints[1].y = 0.4; fpoints[1].z = 0.0;
      fpoints[2].x = 0.6; fpoints[2].y = 0.6; fpoints[2].z = 0.0;
      fpoints[3].x = 0.4; fpoints[3].y = 0.6; fpoints[3].z = 0.0;
      fpoints[4].x = 0.4; fpoints[4].y = 0.4; fpoints[4].z = 0.0;

      PEXPolyline(dpy, resourceID, req_type, 5, points);
      /* PEXMarkers(dpy, resourceID, req_type, 5, mpoints); */
      PEXFillArea(dpy, resourceID, req_type, PEXShapeNonConvex, False, 5, fpoints);
    }

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "pexplc-1");
    *num_images      = 1;
    *supported_modes = IMM_MODE | STR_MODE | WKS_MODE;

    strcpy(win_title, "pexplc-1");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    describe_test("\nPipeline Context Test #1\n\n");
    describe_test(" Tests the initialization of the default Pipeline Context.\n");
    describe_test(" This tests produces 1 image consisting of an outer white solid\n");
    describe_test(" polyline square and an inner white hollow fill area square.\n\n");

    test_image(dpy, resourceID, req_type);
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
     image_testproc("pexplc-1", "pexplc-1", img_desc,
		      dpy,window, 0, 0, 
		      glob_window_width, glob_window_height);
    }
