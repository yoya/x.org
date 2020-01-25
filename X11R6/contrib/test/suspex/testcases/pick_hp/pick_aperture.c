/* $XConsortium: pick_aperture.c,v 1.0 93/11/22 12:47:39 rws Exp $ */

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
#include <X11/Xlib.h>
#include <X11/PEX5/PEXlib.h>
#include <X11/Xutil.h>
#include <misc.h>

/*
  this test verifies pick apertures, using pick one.  A small aperture
  is selected, with primitives rendered on each side of the aperture,
  as well as within.
  This is done for both NPC and DC apertures.
*/

PEXName pick_name[] = {1};
Window  local_win;

void check_borders(Display *display, PEXRenderer rdr, PEXCoord *coord)
{
    PEXCoord temp[2];
    
    PEXAddToNameSet(display, rdr, PEXOCRender, 1, pick_name);

    PEXPolyline(display, rdr, PEXOCRender, 2, coord);

    temp[0] = coord[0];

    temp[1] = coord[1];
    temp[1].x = temp[0].x;
    PEXPolyline(display, rdr, PEXOCRender, 2, temp);

    temp[1] = coord[1];
    temp[1].y = temp[0].y;
    PEXPolyline(display, rdr, PEXOCRender, 2, temp);

    temp[1] = coord[1];
    temp[1].z = temp[0].z;
    PEXPolyline(display, rdr, PEXOCRender, 2, temp);

    temp[1] = coord[1];

    temp[0] = coord[0];
    temp[0].x = temp[1].x;
    PEXPolyline(display, rdr, PEXOCRender, 2, temp);

    temp[0] = coord[0];
    temp[0].y = temp[1].y;
    PEXPolyline(display, rdr, PEXOCRender, 2, temp);

    temp[0] = coord[0];
    temp[0].z = temp[1].z;
    PEXPolyline(display, rdr, PEXOCRender, 2, temp);
}

test_pick(Display         *display, 
	  XID              rdr, 
	  PEXOCRequestType req_type)
{
    int status, undetect;
    PEXPickPath *path;

    {
	PEXRendererAttributes rdr_attrs;
	
	rdr_attrs.clear_image = True;

	rdr_attrs.pick_incl = PEXCreateNameSet(display);
	PEXChangeNameSet(display, rdr_attrs.pick_incl, PEXNSReplace,
			 1, pick_name); 

	PEXChangeRenderer(display, rdr,
			  PEXRAClearImage | PEXRAPickIncl,
			  &rdr_attrs);

	PEXFreeNameSet(display, rdr_attrs.pick_incl);
    }

    {
	PEXPDNPCHitVolume vol = {{0.3 - 0.01, 0.4 - 0.01 ,0.5 - 0.01},
				 {0.3 + 0.01, 0.4 + 0.01 ,0.5 + 0.01}};
	PEXCoord coord[2] = {{0.3 - 0.02, 0.4 - 0.02 ,0.5 - 0.02},
			     {0.3 + 0.02, 0.4 + 0.02 ,0.5 + 0.02}};
	PEXPickRecord pickRecord;

	pickRecord.volume = vol;
	PEXBeginPickOne(display, local_win, rdr, 1, PEXPickLast,
			PEXPickDeviceNPCHitVolume, &pickRecord);

	check_borders(display, rdr, coord);

	path = PEXEndPickOne(display, rdr, &status, &undetect);

	if (!(status == PEXPick && undetect == False &&
	      path->count == 1 &&
	      path->elements->sid == 1 &&
	      path->elements->offset == 2)) 
	    fprintf(stderr, "NPC aperture failed\n");

	PEXFreePickPaths(1, path);
    }

    {
	PEXPDDCHitBox box;
	PEXCoord coord[2];
	PEXPickRecord pickRecord;

	box.position.x = glob_window_width  / 2;
	box.position.y = glob_window_height / 2;
	box.distance   = 3.0;

	coord[0].x = 0.48; coord[0].y = 0.48; coord[0].z = -0.01;
	coord[1].x = 0.52; coord[1].y = 0.52; coord[1].z =  1.01;

	pickRecord.box = box;
	PEXBeginPickOne(display, local_win, rdr, 1, PEXPickLast,
			PEXPickDeviceDCHitBox, &pickRecord);

	check_borders(display, rdr, coord);

	path = PEXEndPickOne(display, rdr, &status, &undetect);

	if (!(status == PEXPick && undetect == False &&
	      path->count == 1 &&
	      path->elements->sid == 1 &&
	      path->elements->offset == 2)) 
	    fprintf(stderr, "DC aperture failed\n");

	PEXFreePickPaths(1, path);
    }
}

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "pick_aperture");
    *num_images      = 1;
    *supported_modes = NON_OC_MODE;

    strcpy(win_title, "pick_aperture");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    _hppex_set_stderr("pick_aperture.stderr", "pick_aperture");
    local_win = window;
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    describe_test("\nPick Test \n\n");
    describe_test(" this test verifies pick apertures, using pick one.  A small aperture\n");
    describe_test(" is selected, with primitives rendered on each side of the aperture,\n");
    describe_test(" as well as within.\n");
    describe_test(" This is done for both NPC and DC apertures.\n\n");
    describe_test(" This test is successful if no stderr output is printed.\n\n");

    test_pick(dpy, resourceID, req_type);
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
    file_testproc("pick_aperture.stderr", "pick_aperture");
    }
