/* $XConsortium: rnd_covrg.c,v 1.0 93/11/22 12:39:23 rws Exp $ */

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


/*
******************************************************************************
** Renderer entrypoint coverage test
******************************************************************************
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/PEX5/PEXlib.h>

#ifdef XSYSTEM
#include <XSystemTest.h>
#endif

#include <pexutcmap.h>
#include <misc.h>
#include <common_hp.h>
#include <err_handler.h>

#include "rnd_utils.h"


#ifdef XSYSTEM
XSystemTestingControlStructure xtc;
#endif

main(argc, argv)
int argc;
char **argv;
{
	Display *display;
	Window window;
	PEXRenderer renderer;
	PEXRendererAttributes *renderer_attrs, my_renderer_attrs;
	PEXStructure structure, substructure;
	PEXElementRef path[2];
	int path_length;
	unsigned long table_dynamics, name_set_dynamics, attr_dynamics;
	long client_structure;
	PEXPickRecord pick_aperture;
	int pick_status, more_hits, nd_hits;
	unsigned long pick_path_count;
	PEXPickPath *pick_paths;
	PEXExtensionInfo *ext_info;

	XVisualInfo vis_info;
	XStandardColormap cmap_info;
	PEXColorApproxEntry capx_info;

    int			pexmode = 0;
	int			image_depth = 0;

#ifndef XSYSTEM
    /* Handle other arguments. */
    if (parse_options (argc, argv, &pexmode, &image_depth))
	exit (1);
#endif

#ifdef XSYSTEM
	openXSystemTesting(argc, argv, &xtc, 0, 0);
#endif

    _hppex_set_stderr("rnd_covrg.stderr", "rnd_covrg");

	describe_test("\n\n");
	describe_test ("This is an entrypoint coverage test for the PEXlib Renderer.\n");
	describe_test ("Each Renderer entrypoint is called with reasonable mainline\n");
	describe_test ("values.\n\n");

	describe_test ("The test generates no images.  It succeeds if no messages\n");
	describe_test ("starting with ERROR are printed to stderr.\n\n");

#ifdef XSYSTEM
	if (xtc.interactiveFramePrompt)
#else
	if (glob_verbose_mode && getenv("PAUSE"))
#endif
		prompt_user ("Hit return to continue with the test", "");

#ifndef XSYSTEM
    if (glob_verbose_mode)
	    fflush (stdout);
#endif


	/*
		Initialize X and PEX.
	*/

#ifdef XSYSTEM
    display = xtc.display;
#else
	if (NULL == (display = XOpenDisplay (NULL))) {

		_hppex_stderr_print( "PROCESS ERROR:  XOpenDisplay failed on %s\n", getenv("DISPLAY"));
		exit (1);
	}
#endif

	if (PEXInitialize (display, &ext_info, 0, NULL))
	{
		_hppex_stderr_print( "PROCESS ERROR:  PEXInitialize failed on %s\n", getenv("DISPLAY"));
		exit (1);
	}

	/*
		Create a window.
	*/
	window = create_and_setup_window (display,
							"PEXlib Renderer Coverage Test", 
							image_depth,
							0, 0, 
							200, 200,
							&capx_info);

	if (window == None) {

		_hppex_stderr_print( "PROCESS ERROR:  create_window failed\n");
		exit (1);
	}


	/*
		Create a simple structure network to use in traversals.
	*/
	structure = PEXCreateStructure (display);
	substructure = PEXCreateStructure (display);
	if ((structure == None) || (substructure == None)) {

		_hppex_stderr_print( "PROCESS ERROR:  structure creation failed\n");
		exit (1);
	}
	PEXLabel (display, structure, PEXOCStore, 100);
	PEXLabel (display, structure, PEXOCStore, 200);
	PEXExecuteStructure (display, structure, PEXOCStore, substructure);
	PEXLabel (display, structure, PEXOCStore, 600);
	PEXLabel (display, structure, PEXOCStore, 700);

	PEXLabel (display, substructure, PEXOCStore, 300);
	PEXLabel (display, substructure, PEXOCStore, 400);
	PEXLabel (display, substructure, PEXOCStore, 500);

	path[0].structure = structure;
	path[0].offset = 3;
	path[1].structure = substructure;
	path[1].offset = 3;
	path_length = 2;

	client_structure = 52;


	/*
		Create a Renderer.
	*/
	renderer = PEXCreateRenderer (display, window, NULL, 0);
	if (renderer == None) {

		_hppex_stderr_print( "ERROR: PEXCreateRenderer failed, test aborted\n");
		exit (1);
	}

	/*
		Call the Renderer attribute routines.
	*/
	renderer_attrs = PEXGetRendererAttributes (display, renderer, ALL_RENDERER_ATTRS_MASK);
	if (NULL == renderer_attrs) {

		_hppex_stderr_print( "ERROR: PEXGetRendererAttributes failed\n");
	}
	PEXChangeRenderer (display, renderer, ALL_RENDERER_ATTRS_MASK, renderer_attrs);
	PEXFreeRendererAttributes (renderer_attrs);

	if (0 == PEXGetRendererDynamics (display, renderer, 
							&table_dynamics, &name_set_dynamics, &attr_dynamics)) {
		_hppex_stderr_print( "ERROR: PEXGetRendererDynamics failed\n");
	}
	

	/*
		Start an immediate mode drawing sequence and call all the Renderer 
		entrypoints that are normally called in such a sequence.
	*/
	PEXBeginRendering (display, window, renderer);
	PEXBeginStructure (display, renderer, client_structure);
	PEXEndStructure (display, renderer);
	PEXAccumulateState (display, renderer, path_length, path);
	PEXRenderElements (display, renderer, structure, PEXBeginning, 0, PEXEnd, 0);
	PEXEndRendering (display, renderer, True);

	/*
		Start a drawing traversal of a structure.
	*/
	PEXRenderNetwork (display, window, renderer, structure);

	/*
		Set up stuff needed for picking entrypoints.
	*/
	pick_aperture.volume.min.x = 0.0;
	pick_aperture.volume.min.y = 0.0;
	pick_aperture.volume.min.z = 0.0;
	pick_aperture.volume.max.x = 0.0;
	pick_aperture.volume.max.y = 0.0;
	pick_aperture.volume.max.z = 0.0;

	/*
		Exercise the two types of immediate mode picking.
	*/
	PEXBeginPickAll (display, window, renderer, client_structure,
					PEXPickAllAll, True, 5, 
					PEXPickDeviceNPCHitVolume, &pick_aperture);
	pick_paths = PEXEndPickAll (display, renderer, 
								&pick_status, &more_hits, &pick_path_count);
	if (pick_status == PEXPick) 
		PEXFreePickPaths (pick_path_count, pick_paths);
	else 
		/* Make sure this gets called at least once. */
		PEXFreePickPaths (0, pick_paths);

	PEXBeginPickOne (display, window, renderer, client_structure,
					PEXPickLast,
					PEXPickDeviceNPCHitVolume, &pick_aperture);
	pick_paths = PEXEndPickOne (display, renderer, 
								&pick_status, &nd_hits);
	if (pick_status == PEXPick) 
		PEXFreePickPaths (1, pick_paths);

	/*
		Exercise the two types of traversal picking.
	*/
	path[0].structure = structure;
	path[0].offset = 1;
	my_renderer_attrs.pick_start_path.count = 1;
	my_renderer_attrs.pick_start_path.elements = path;
	PEXChangeRenderer (display, renderer, PEXRAPickStartPath, &my_renderer_attrs);

	pick_paths = PEXPickAll (display, window, renderer,
							PEXPickAllAll, 5, 
							PEXPickDeviceNPCHitVolume, &pick_aperture,
							&pick_status, &more_hits, &pick_path_count);
	if (pick_status == PEXPick) 
		PEXFreePickPaths (pick_path_count, pick_paths);

	pick_paths = PEXPickOne (display, window, renderer, structure,
							PEXPickLast,
							PEXPickDeviceNPCHitVolume, &pick_aperture,
							&pick_status, &nd_hits);
	if (pick_status == PEXPick) 
		PEXFreePickPaths (1, pick_paths);


	/* 
		Free the Renderer, window, and connection.
	*/
	PEXFreeRenderer (display, renderer);

	XDestroyWindow (display, window);

    _hppex_close_stderr();

#ifdef XSYSTEM
    closeXSystemTesting(&xtc);
#else
    file_testproc("rnd_covrg.stderr", "rnd_covrg");
	XCloseDisplay (display);
	exit(0);
#endif

} /* renderer coverage test */
