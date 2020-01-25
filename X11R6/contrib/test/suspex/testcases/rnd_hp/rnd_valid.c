/* $XConsortium: rnd_valid.c,v 1.0 93/11/22 12:40:12 rws Exp $ */

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
** Renderer/Structure/OC API state validation test
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


#ifdef XSYSTEM
#define WIDTH 256
#define HEIGHT 256
#else
#define WIDTH 200
#define HEIGHT 200
#endif

#define HORIZONTAL_SPACING 20
#define VERTICAL_SPACING 40

#define NUM_COLUMNS 10
#define NUM_ROWS 10
#define X_INCREMENT (1.0/NUM_COLUMNS)
#define Y_INCREMENT (1.0/NUM_ROWS)

#define NUM_CONNECTIONS 2
#define NUM_WINDOWS 3
#define NUM_RENDERERS 2
#define NUM_STRUCTURES 3

#define NUM_ITERATIONS (NUM_COLUMNS *		\
						NUM_ROWS *			\
						NUM_CONNECTIONS *	\
						NUM_WINDOWS *		\
						NUM_RENDERERS *		\
						NUM_STRUCTURES/2)

/* possible targets */
#define RENDERER 0
#define STRUCTURE 1
#define NUM_TARGETS 2

/* possible operations (OCs) */
#define ATTR 0
#define PRIM 1
#define NUM_OPERATIONS 2

#define NUM_COLORS 7

#define SEED_RANDOM 		{ srand48(1L); }
#define RANDOM_CONNECTION	((int) (drand48() * NUM_CONNECTIONS))
#define RANDOM_OPERATION	((int) (drand48() * NUM_OPERATIONS))
#define RANDOM_TARGET		((int) (drand48() * NUM_TARGETS))
#define RANDOM_RENDERER		((int) (drand48() * NUM_RENDERERS))
#define RANDOM_WINDOW		((int) (drand48() * NUM_WINDOWS))
#define RANDOM_STRUCTURE	((int) (drand48() * NUM_STRUCTURES))


typedef struct {
	int color_index;
} attr_state;

typedef struct {
	int row, col;
} prim_state;

typedef struct {
	attr_state attr;
	prim_state prim;
	int elem_count;
} attr_prim_state;



main(argc, argv)
int argc;
char **argv;
{
	Display *display, *test_display;
	Display *displays[NUM_CONNECTIONS];
	Window window;
	Window windows[NUM_CONNECTIONS][NUM_WINDOWS];
	PEXRenderer renderer;
	PEXRenderer renderers[NUM_CONNECTIONS][NUM_RENDERERS];
	PEXLookupTable color_luts[NUM_CONNECTIONS][NUM_RENDERERS];
	PEXPipelineContext pipeline_contexts[NUM_CONNECTIONS][NUM_RENDERERS];
	PEXStructure structure;
	PEXStructure structures[NUM_CONNECTIONS][NUM_STRUCTURES];
	PEXExtensionInfo *ext_info;

	attr_state renderer_states[NUM_CONNECTIONS][NUM_RENDERERS];
	prim_state window_states[NUM_CONNECTIONS][NUM_WINDOWS];
	attr_prim_state structure_states[NUM_CONNECTIONS][NUM_STRUCTURES];
	PEXStructureInfo struct_info;

	XVisualInfo vis_info;
	XVisualInfo vis_infos[NUM_CONNECTIONS];
	XStandardColormap cmap_info;
	XStandardColormap cmap_infos[NUM_CONNECTIONS];
	PEXColorApproxEntry capx_info;
	PEXColorApproxEntry capx_infos[NUM_CONNECTIONS];

	int screen;
	XSizeHints hints;
	char title[256];

	PEXUtWindowSpecification  window_info;
	PEXUtVisualCriteria       criteria;
	Atom                      prop_atom;
	XColor                    returned_background;
	XEvent                    event;
	unsigned int              unmet;
	int                       status;

	int conn_index, window_index, resource_index, color_index, extra_index;
	int operation, target, iterations;
	int row, col;
	int image_number = 0;
	int need_to_capture;

	XID target_resource;
	PEXOCRequestType oc_target;
	attr_state *attr_ptr;
	prim_state *prim_ptr;

	PEXPCAttributes plc_attrs;
	unsigned long plc_mask[3];
	PEXColor color;
	char *display_string;

	char image_msg[256];
	char image_name[80], test_name[80];

#ifdef XSYSTEM
    int XSystemValues[20];
#endif

    int			pexmode = 0;
	int			image_depth = 0;

#ifdef XSYSTEM
	openXSystemTesting(argc, argv, &xtc, 0, 0);
	test_display = xtc.display;
	display_string = DisplayString(test_display);
#else
    /* Handle other arguments. */
    if (parse_options (argc, argv, &pexmode, &image_depth))
		exit (1);
	display_string = getenv("DISPLAY");
#endif

    _hppex_set_stderr("rnd_valid.stderr", "rnd_valid");

	describe_test ("\n\n");
	describe_test("This test stresses state switching and validation.  Several connections\n");
	describe_test("to the X server are created, and several Windows, Renderers, and Structures\n");
	describe_test("are created on each connection.  Then a pseudo-random number stream is\n");
	describe_test("used to repeatedly choose a connection, target, and OC (attribute or primitive).\n\n");
	
	describe_test("Images are generated, which are not easily verified by an operator to be\n");
	describe_test("right or wrong.  The most important thing to look for is that approximately\n");
	describe_test("an equal number of OCS (to the extent one would expect of a random number\n");
	describe_test("stream) are sent to each target.  This is a system-level stress test.\n\n");

	describe_test("Images are captured in two sets:  First, all the immediate mode rendering\n");
	describe_test("(which occurred when a renderer/window combination was randomly chosen)\n");
	describe_test("is captured.  Then, all the windows are cleared and the Structures\n");
	describe_test("that have been also receiving OCs are traversed to show their contents.\n");
	describe_test("If there are more Structures per connection than Windows, this may take more\n");
	describe_test("than one image.  The number of connections, Windows, Renderers, and Structures\n");
	describe_test("are compile-time constants that should not be varied once this test is archived.\n\n");

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

	for (conn_index = 0; conn_index < NUM_CONNECTIONS; conn_index++) {

		/*
			Initialize X and PEX.
		*/
		if (NULL == (displays[conn_index] = XOpenDisplay (display_string))) {

			_hppex_stderr_print( 
					"PROCESS ERROR:  XOpenDisplay failed on connection %d to %s\n", 
					conn_index, display_string);
			exit (1);
		}
		display = displays[conn_index];

		if (PEXInitialize (display, &ext_info, 0, NULL))
		{
			_hppex_stderr_print( 
					"PROCESS ERROR:  PEXInitialize failed on connection %d to %s\n", 
					conn_index, display_string);
			exit (1);
		}

#ifdef XSYSTEM
	    if (conn_index == 0) {
			/*
				Make sure all banks are cleared at the start of this test.
			*/

			_hppex_tst_init_ALL_banks (display);
		}
#endif

		/*
			Create first window.
		*/

		sprintf (title, "Validation [%d][0]", conn_index);
		screen = DefaultScreen(display);
		hints.x = HORIZONTAL_SPACING + 
				conn_index*2*(WIDTH+HORIZONTAL_SPACING);
		hints.y = VERTICAL_SPACING;
		hints.width = WIDTH;
		hints.height = HEIGHT;
		hints.flags = (USSize|USPosition);

		window_info.attr_mask             = CWEventMask;
		window_info.attrs.event_mask      = ExposureMask;
		window_info.title                 = title;
		window_info.size_hints            = hints;
		window_info.parent                = RootWindow (display, screen);
		window_info.border_width          = 0;
		window_info.background_color_name = "black";
		window_info.border_color_name     = "white";
		
		if (image_depth == 0)
		{
			char *depth;

			if (depth = getenv("TEST_IMAGE_DEPTH"))
				{
				image_depth = atoi(depth);
				}
		}

		if (image_depth == 0)
		{
			criteria.hard_criteria_mask = PEXUtMinColors;
			criteria.soft_criteria_mask = PEXUtDepth | 
						  PEXUtStandardColormapProperty;
			criteria.depth = 24;
		}
		else
		{
			criteria.hard_criteria_mask = PEXUtMinColors | PEXUtDepth;
			criteria.soft_criteria_mask = PEXUtStandardColormapProperty;
			criteria.depth = image_depth;
		}

		criteria.min_colors = 2;
		criteria.standard_colormap_property = True;

		status = PEXUtCreateWindowAndColormap (display, screen,
						   	&criteria,
						   	&window_info, 
							&(windows[conn_index][0]),
							&(vis_infos[conn_index]), 
							&(cmap_infos[conn_index]), 
							&(capx_infos[conn_index]),
						   	&unmet, &prop_atom,
						   	&returned_background);

		if ((status != PEXUtSuccess) &&
			(status != PEXUtQualifiedSuccess)) {
			fprintf (stderr,
				"PROCESS ERROR:  First window creation on connection %d failed\n", conn_index);
			exit (1);
		}

		window = windows[conn_index][0];
		XSetWindowColormap(display, window, cmap_infos[conn_index].colormap);
#ifdef XSYSTEM
		XInstallColormap(display, cmap_infos[conn_index].colormap);
#endif
		XMapWindow(display, window);
		XFlush(display);

		/* Wait for the first exposure */
		XSelectInput( display, window, ExposureMask);
		XNextEvent(display, &event);

#ifdef XSYSTEM
		XSystemValues[0] = WINDOW_SMART_HW_CONFIG;
		XSystemValues[1] = windows[conn_index][0];
		XSystemValues[2] = -1;
		defineXSystemTestControlParm(&xtc, ARCHIVE_MODE, XSystemValues);
#endif

		window_states[conn_index][0].col = 0;
		window_states[conn_index][0].row = 0;

		/*
			Create more windows using same colormap and visual.
		*/
		hints.x += WIDTH + HORIZONTAL_SPACING;

		window_info.attr_mask             = CWColormap;
		window_info.attrs.colormap = cmap_infos[conn_index].colormap;

		for (window_index=1; window_index<NUM_WINDOWS; window_index++) {

			sprintf (title, "Validation [%d][%d]", conn_index, window_index);
			window_info.size_hints = hints;

			status = PEXUtCreateWindow ( display, screen,
								&window_info,
								&(vis_infos[conn_index]),
								&(windows[conn_index][window_index]),
								&returned_background);

			if ((status != PEXUtSuccess) &&
				(status != PEXUtQualifiedSuccess)) {

				_hppex_stderr_print( 
					"PROCESS ERROR:  Window creation #%d on connection %d failed\n", 
					window_index, conn_index);
				exit (1);
			}

			XMapWindow(display, windows[conn_index][window_index]);
			XFlush(display);

#ifdef XSYSTEM
			XSetWindowColormap(display, windows[conn_index][window_index], 
								cmap_infos[conn_index].colormap);
			XInstallColormap(display, cmap_infos[conn_index].colormap);
			XSync (display, 0);
#endif

			window_states[conn_index][window_index].col = 0;
			window_states[conn_index][window_index].row = 0;

			hints.y += HEIGHT + VERTICAL_SPACING;
		}


		for (resource_index=0; resource_index<NUM_RENDERERS; resource_index++) {

			PEXRendererAttributes set_attrs;

			/*
				Create a Renderer.
			*/
			window = windows[conn_index][0];
			renderers[conn_index][resource_index] = PEXCreateRenderer (display, window, 0, NULL);
			if (renderers[conn_index][resource_index] == None) {

				_hppex_stderr_print( 
					"PROCESS ERROR: PEXCreateRenderer %d on connection %d failed\n", 
					resource_index, conn_index);
				exit (1);
			}

			/*
				Create a color LUT and a Pipeline Context 
				and attach them to the Renderer.
			*/
			color_luts[conn_index][resource_index] = 
				PEXCreateLookupTable (display, window, PEXLUTColor);

			if (color_luts[conn_index][resource_index] == None) {

				_hppex_stderr_print( 
					"PROCESS ERROR: PEXCreateLookupTable %d on connection %d failed\n", 
					resource_index, conn_index);
				exit (1);
			}

			plc_mask[0] = 0;
			plc_mask[1] = 0;
			plc_mask[2] = 0;
			pipeline_contexts[conn_index][resource_index] = 
				PEXCreatePipelineContext (display, plc_mask, &plc_attrs);

			if (pipeline_contexts[conn_index][resource_index] == None) {

				_hppex_stderr_print( 
					"PROCESS ERROR: PEXCreatePipelineContext %d on connection %d failed\n", 
					resource_index, conn_index);
				exit (1);
			}

			set_attrs.color_table = color_luts[conn_index][resource_index];
			set_attrs.pipeline_context = pipeline_contexts[conn_index][resource_index];
			PEXChangeRenderer (display, 
				renderers[conn_index][resource_index],
				PEXRAColorTable | PEXRAPipelineContext, &set_attrs);

			renderer_states[conn_index][resource_index].color_index = 1;

			PEXSetPCAttributeMask (plc_mask, PEXPCSurfaceColor);
			plc_attrs.surface_color.type = PEXColorTypeIndexed;
			plc_attrs.surface_color.value.indexed.index = 1;
			PEXSetPCAttributeMask (plc_mask, PEXPCInteriorStyle);
			plc_attrs.interior_style = PEXInteriorStyleSolid;
			PEXSetPCAttributeMask (plc_mask, PEXPCSurfaceEdgeFlag);
			plc_attrs.surface_edges = PEXOn;
			PEXSetPCAttributeMask (plc_mask, PEXPCSurfaceEdgeColor);
			plc_attrs.surface_edge_color.type = PEXColorTypeIndexed;
			plc_attrs.surface_edge_color.value.indexed.index = 1;
			PEXChangePipelineContext (display, 
				pipeline_contexts[conn_index][resource_index],
				plc_mask, &plc_attrs);
		}

		for (resource_index=0; resource_index<NUM_STRUCTURES; resource_index++) {

			structures[conn_index][resource_index] = PEXCreateStructure (display);
			if (structures[conn_index][resource_index] == None) {

				_hppex_stderr_print( 
					"PROCESS ERROR: PEXCreateStructure %d on connection %d failed\n", 
					resource_index, conn_index);
				exit (1);
			}

			structure_states[conn_index][resource_index].prim.col = 0;
			structure_states[conn_index][resource_index].prim.row = 0;
			structure_states[conn_index][resource_index].attr.color_index = 1;

			color.indexed.index = 1;
			PEXSetSurfaceColor (display, 
								structures[conn_index][resource_index], 
								PEXOCStore,
								PEXColorTypeIndexed, &color);
			PEXSetSurfaceEdgeFlag (display, 
								structures[conn_index][resource_index], 
								PEXOCStore, PEXOn);
			PEXSetSurfaceEdgeColor (display, 
								structures[conn_index][resource_index], 
								PEXOCStore,
								PEXColorTypeIndexed, &color);
			PEXSetInteriorStyle (display,
								structures[conn_index][resource_index], 
								PEXOCStore,
								PEXInteriorStyleSolid);
			structure_states[conn_index][resource_index].elem_count = 4;
		}
	} /* for connections */


	for (conn_index = 0; conn_index < NUM_CONNECTIONS; conn_index++) {
		for (window_index=0; window_index<NUM_WINDOWS; window_index++) {
			for (resource_index=0; resource_index<NUM_RENDERERS; resource_index++) {

				display = displays[conn_index];
				window = windows[conn_index][window_index];
				renderer = renderers[conn_index][resource_index];

				set_renderer_clear_flags (display, renderer, True, 0.0, 0.0, 0.0);
				PEXBeginRendering (display, window, renderer);
				PEXEndRendering (display, renderer, True);
				set_renderer_clear_flags (display, renderer, False, 0.0, 0.0, 0.0);
			}
		}
	}


	for (iterations=0; iterations<NUM_ITERATIONS; iterations++) {

		conn_index = RANDOM_CONNECTION;
		operation = RANDOM_OPERATION;
		target = RANDOM_TARGET;
		if (target == RENDERER) {

			resource_index = RANDOM_RENDERER;
			window_index = RANDOM_WINDOW;

			attr_ptr = &(renderer_states[conn_index][resource_index]);
			prim_ptr = &(window_states[conn_index][window_index]);
			target_resource = renderers[conn_index][resource_index];
			oc_target = PEXOCRender;

			if (operation == ATTR) {

				color_index = ((attr_ptr->color_index + 1) % NUM_COLORS) + 1;

				plc_mask[0] = 0;
				plc_mask[1] = 0;
				plc_mask[2] = 0;
				PEXSetPCAttributeMask (plc_mask, PEXPCSurfaceColor);
				plc_attrs.surface_color.type = PEXColorTypeIndexed;
				plc_attrs.surface_color.value.indexed.index = color_index;
				PEXSetPCAttributeMask (plc_mask, PEXPCSurfaceEdgeFlag);
				plc_attrs.surface_edges = PEXOn;
				PEXSetPCAttributeMask (plc_mask, PEXPCSurfaceEdgeColor);
				plc_attrs.surface_edge_color.type = PEXColorTypeIndexed;
				plc_attrs.surface_edge_color.value.indexed.index = color_index;
				PEXChangePipelineContext (displays[conn_index], 
					pipeline_contexts[conn_index][resource_index],
					plc_mask, &plc_attrs);

				attr_ptr->color_index = color_index;
			}

			PEXBeginRendering (displays[conn_index], 
								windows[conn_index][window_index], 
								renderers[conn_index][resource_index]);
		}
		else {
			resource_index = RANDOM_STRUCTURE;

			attr_ptr = &(structure_states[conn_index][resource_index].attr);
			prim_ptr = &(structure_states[conn_index][resource_index].prim);
			target_resource = structures[conn_index][resource_index];
			oc_target = PEXOCStore;

			if (operation == ATTR) {

				color_index = ((attr_ptr->color_index + 1) % NUM_COLORS) + 1;
				color.indexed.index = color_index;
				PEXSetSurfaceColor (displays[conn_index], target_resource, oc_target,
									PEXColorTypeIndexed, &color);
				PEXSetSurfaceEdgeFlag (displays[conn_index], target_resource, oc_target, PEXOn);
				PEXSetSurfaceEdgeColor (displays[conn_index], target_resource, oc_target,
									PEXColorTypeIndexed, &color);
				attr_ptr->color_index = color_index;
				structure_states[conn_index][resource_index].elem_count += 3;
			}
			else
				structure_states[conn_index][resource_index].elem_count += 1;
		}

		if (operation == PRIM) {

			row = prim_ptr->row;
			col = prim_ptr->col;
			if (++col >= NUM_COLUMNS) {
				if (row++ >= NUM_ROWS)
					row = 0;
				col = 0;
			}

			draw_primitive (displays[conn_index], target_resource, oc_target,
							FILLED_RECTANGLE,
							col * X_INCREMENT,
							row * Y_INCREMENT,
							0.0,
							X_INCREMENT,
							Y_INCREMENT,
							0.0);
			prim_ptr->row = row;
			prim_ptr->col = col;
		}

		if (target == RENDERER) {
			PEXEndRendering (	displays[conn_index], 
								renderers[conn_index][resource_index],
								True);
		}

	} /* for iterations */

	/* capture images from all windows */
	_hppex_stderr_print( "Capturing images from immediate mode rendering\n\n");
	for (conn_index = 0; conn_index < NUM_CONNECTIONS; conn_index++) {
		XSync (displays[conn_index], 0);
	}
#ifdef XSYSTEM
#ifdef IMAGE_PER_WINDOW
	for (conn_index = 0; conn_index < NUM_CONNECTIONS; conn_index++) {
		XSync (displays[conn_index], 0);
		for (extra_index=0; extra_index<NUM_WINDOWS; extra_index++) {

			XSystemValues[0] = WINDOW_SMART_EXTENT;
			XSystemValues[1] = windows[conn_index][extra_index];
			defineXSystemTestControlParm(&xtc, ARCHIVE_EXTENT, XSystemValues);

			sprintf (image_msg, "ERROR: Immediate mode, conn %d, window %d",
					conn_index, extra_index);
			XSystemCheck(image_msg, &xtc);
		}
	}
	printf ("\n");
#else
	XSystemValues[0] = FULL_SCREEN;
	defineXSystemTestControlParm(&xtc, ARCHIVE_EXTENT, XSystemValues);

	XSystemCheck("ERROR: Immediate mode", &xtc);
	printf ("\n");
#endif /* IMAGE_PER_WINDOW */
#else
	for (conn_index = 0; conn_index < NUM_CONNECTIONS; conn_index++) {
		XSync (displays[conn_index], 0);
		for (extra_index=0; extra_index<NUM_WINDOWS; extra_index++) {

			sprintf(image_name,"rnd_valid_%d", image_number);
			sprintf(test_name, "rnd_valid #%d", image_number);
			 image_testproc(image_name, test_name, img_desc,
							  display, 
								windows[conn_index][extra_index],
								0, 0, WIDTH, HEIGHT);
			image_number++;
		}
	}
#endif


	/*
		Now dump all the structures to appropriate windows.
	*/
	need_to_capture = False;
	_hppex_stderr_print( "Capturing images from traversal mode rendering\n\n");
	for (resource_index = 0; resource_index<NUM_STRUCTURES; resource_index++) {

		window_index = resource_index % NUM_WINDOWS;

		if ((window_index < resource_index) && (window_index == 0)) {
			/* 
				Must have wrapped around because there are more structures
				than windows, so capture the current set of windows.
			*/
			for (conn_index = 0; conn_index < NUM_CONNECTIONS; conn_index++) {
				XSync (displays[conn_index], 0);
			}
#ifdef XSYSTEM
#ifdef IMAGE_PER_WINDOW
			for (conn_index = 0; conn_index < NUM_CONNECTIONS; conn_index++) {
				for (extra_index=0; extra_index<NUM_WINDOWS; extra_index++) {

					XSystemValues[0] = WINDOW_SMART_EXTENT;
					XSystemValues[1] = windows[conn_index][extra_index];
					defineXSystemTestControlParm(&xtc, ARCHIVE_EXTENT, XSystemValues);

					sprintf (image_msg, "ERROR: Traversal mode, conn %d, window %d",
							conn_index, extra_index);
					XSystemCheck(image_msg, &xtc);
				}
			}
			printf ("\n");
#else
			XSystemValues[0] = FULL_SCREEN;
			defineXSystemTestControlParm(&xtc, ARCHIVE_EXTENT, XSystemValues);

			XSystemCheck("Traversal mode", &xtc);
			printf ("\n");
#endif /* IMAGE_PER_WINDOW */
#else
			for (conn_index = 0; conn_index < NUM_CONNECTIONS; conn_index++) {
				for (extra_index=0; extra_index<NUM_WINDOWS; extra_index++) {

					sprintf(image_name,"rnd_valid_%d", image_number);
					sprintf(test_name, "rnd_valid #%d", image_number);
					 image_testproc(image_name, test_name, img_desc,
									  display, 
										windows[conn_index][extra_index],
										0, 0, WIDTH, HEIGHT);

					image_number++;
				}
			}
#endif
			need_to_capture = False;
		}

		for (conn_index = 0; conn_index<NUM_CONNECTIONS; conn_index++) {

			if (PEXGetStructureInfo (displays[conn_index],
								structures[conn_index][resource_index],
								PEXIEEE_754_32, PEXNumElements,
								&struct_info)) {
				if (structure_states[conn_index][resource_index].elem_count !=
					struct_info.element_count) {

					_hppex_stderr_print( "ERROR: structure %d, connection %d elem count mismatch\n",
							resource_index, conn_index);
					_hppex_stderr_print( "\texpected elem count: %d\n",
						structure_states[conn_index][resource_index].elem_count);
					_hppex_stderr_print( "\tactual elem count: %d\n", struct_info.element_count);
				}
			}

			set_renderer_clear_flags (displays[conn_index], renderers[conn_index][0], True, 0.0, 0.0, 0.0);
			PEXRenderNetwork (displays[conn_index], windows[conn_index][window_index],
							renderers[conn_index][0], structures[conn_index][resource_index]);
			XSync (displays[conn_index], 0);
			need_to_capture = True;
		}
	}

	if (need_to_capture) {
		/* capture images */
#ifdef XSYSTEM
#ifdef IMAGE_PER_WINDOW
		for (conn_index = 0; conn_index < NUM_CONNECTIONS; conn_index++) {
			for (extra_index=0; extra_index<NUM_WINDOWS; extra_index++) {

				XSystemValues[0] = WINDOW_SMART_EXTENT;
				XSystemValues[1] = windows[conn_index][extra_index];
				defineXSystemTestControlParm(&xtc, ARCHIVE_EXTENT, XSystemValues);

				sprintf (image_msg, "ERROR: Traversal mode, conn %d, window %d",
						conn_index, extra_index);
				XSystemCheck(image_msg, &xtc);
			}
		}
		printf ("\n");
#else
		XSystemValues[0] = FULL_SCREEN;
		defineXSystemTestControlParm(&xtc, ARCHIVE_EXTENT, XSystemValues);

		XSystemCheck("ERROR: Traversal mode", &xtc);
		printf ("\n");
#endif /* IMAGE_PER_WINDOW */
#else
		for (conn_index = 0; conn_index < NUM_CONNECTIONS; conn_index++) {
			for (extra_index=0; extra_index<NUM_WINDOWS; extra_index++) {

				sprintf(image_name,"rnd_valid_%d", image_number);
				sprintf(test_name, "rnd_valid #%d", image_number);
				 image_testproc(image_name, test_name, img_desc,
								  display, 
									windows[conn_index][extra_index],
									0, 0, WIDTH, HEIGHT);
				image_number++;
			}
		}
#endif
	}


	/*
		Close Structures, Renderers, windows, and connections.
	*/
	for (conn_index = 0; conn_index<NUM_CONNECTIONS; conn_index++) {

		for (resource_index=0; resource_index < NUM_STRUCTURES; resource_index++)
			PEXDestroyStructures(displays[conn_index], 1, &(structures[conn_index][resource_index]));

		for (resource_index=0; resource_index < NUM_RENDERERS; resource_index++) {
			PEXFreeRenderer(displays[conn_index], renderers[conn_index][resource_index]);
			PEXFreeLookupTable(displays[conn_index], color_luts[conn_index][resource_index]);
			PEXFreePipelineContext(displays[conn_index], pipeline_contexts[conn_index][resource_index]);
		}

		for (window_index=0; window_index < NUM_WINDOWS; window_index++)
			XDestroyWindow(displays[conn_index], windows[conn_index][window_index]);

		XCloseDisplay (displays[conn_index]);
	}

    _hppex_close_stderr();

#ifdef XSYSTEM
    closeXSystemTesting(&xtc);
#else
    file_testproc("rnd_valid.stderr", "rnd_valid");
	exit(0);
#endif

} /* renderer state validation test */
