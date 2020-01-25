/* $XConsortium: main.c,v 1.0 93/11/22 12:47:27 rws Exp $ */

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
#include "client_utils.h"
#include <XSystemTest.h>

/*
  this test verifies pick apertures, using pick one.  A small aperture
  is selected, with primitives rendered on each side of the aperture,
  as well as within.
  This is done for both NPC and DC apertures.
*/

PEXName pick_name[] = {1};

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

XSystemTestingControlStructure xtc;

void main(int argc, char *argv[])
{
    Display *display;
    Window window;
    PEXRenderer rdr;
    int status, undetect;
    PEXPickPath *path;
    PEXExtensionInfo *info;
    int err_num;
    char err_string[160];

    openXSystemTesting(argc, argv, &xtc, 0, 0);

    display = xtc.display;

    err_num = PEXInitialize(display, &info, 160, err_string);

    {
	const int screen = DefaultScreen(display);
	XSizeHints hints;
	XVisualInfo vis_info;
	XStandardColormap cmap_info;
	PEXColorApproxEntry color_approx_entry;
	const int min_num_colors = 1 << 8;

	hints.x = 0;
	hints.y = 0;
	hints.width = 1280;
	hints.height = 1024;
	hints.flags = (USSize | USPosition);

	HPPEXCreateWindowAndColormap (display, screen, min_num_colors,
				      True,  &hints, "HP-PEXlib Window",
				      &window, &vis_info, &cmap_info,
				      &color_approx_entry);
    }

    {
	PEXRendererAttributes rdr_attrs;
	
	rdr_attrs.clear_image = True;

	rdr_attrs.pick_incl = PEXCreateNameSet(display);
	PEXChangeNameSet(display, rdr_attrs.pick_incl, PEXNSReplace,
			 1, pick_name); 

	rdr = PEXCreateRenderer(display, window,
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
	PEXBeginPickOne(display, window, rdr, 1, PEXPickLast,
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
	PEXPDDCHitBox box = {{500,600}, 3.0};
	PEXCoord coord[2] = {{496 / 1024.0, 596 / 1024.0, -0.01},
			     {504 / 1024.0, 604 / 1024.0, 1.01}};
	PEXPickRecord pickRecord;

	pickRecord.box = box;
	PEXBeginPickOne(display, window, rdr, 1, PEXPickLast,
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

#ifdef BBA
    _bA_dump();
#endif

    closeXSystemTesting(&xtc);
}
