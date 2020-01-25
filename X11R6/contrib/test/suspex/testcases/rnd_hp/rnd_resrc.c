/* $XConsortium: rnd_resrc.c,v 1.0 93/11/22 12:39:47 rws Exp $ */

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
** Renderer resource attribute set/inquire test
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


#ifdef XSYSTEm
#define WIDTH 300
#define HEIGHT 500
#else
#define WIDTH 200
#define HEIGHT 200
#endif

#define BAD_RESOURCE_ID 0x010101
#define NUM_RENDERERS 3

static PEXRenderer renderers[NUM_RENDERERS];
static PEXRendererAttributes idle_renderer_attrs, 
							active_renderer_attrs, 
							check_attrs;

static int create_and_bind_renderers (	Display *display, 
										Window window, 
										int loop)
{
	int i;

	for (i=0; i<NUM_RENDERERS; i++) {

		renderers[i] = PEXCreateRenderer (display, window, NULL, 0);
		if (renderers[i] == None) {

			_hppex_stderr_print( "ERROR: PEXCreateRenderer failed for renderer %d\n", i);
			return 1;
		}

		if (loop == 1) {
			PEXBeginRendering(display, window, renderers[i]);
			check_attrs = active_renderer_attrs;
		}
		else
			check_attrs = idle_renderer_attrs;
	}

	return 0;
} /* create_and_bind_renderers */


static char * loop_condition[]={
	"Testing with Idle Renderer, never bound to a window",
	"Testing with Active Renderer",
	"Testing with Idle Renderer, has been bound in the past",
};


main(argc, argv)
int argc;
char **argv;
{
	Display *display;
	Window window, extra_window;
	PEXStructure structure;
	PEXRenderer renderer;
	PEXRendererAttributes set_attrs;
	Colormap extra_colormap;
	PEXExtensionInfo *ext_info;

	XVisualInfo vis_info, *visuals, vis_template;
	XStandardColormap cmap_info;
	PEXColorApproxEntry capx_info;
	PEXElementRef current_path_elems;
	int screen, num_visuals;
	XSizeHints hints;
	int extra_target_found;

	int i, j, loop;

    int			pexmode = 0;
	int			image_depth = 0;

#ifdef XSYSTEM
	openXSystemTesting(argc, argv, &xtc, 0, 0);
#else
    /* Handle other arguments. */
    if (parse_options (argc, argv, &pexmode, &image_depth))
	exit (1);
#endif

    _hppex_set_stderr("rnd_resrc.stderr", "rnd_resrc");

	describe_test("\n\n");
	describe_test("This is a test of setting and inquire of attributes of the PEXlib Renderer\n");
	describe_test("that are resource IDs.  In addition to testing that the IDs can be set\n");
	describe_test("and changed, the test attempts to make sure that resource reference\n");
	describe_test("count occurs properly: e.g., a resource persists after it is freed\n");
	describe_test("when it has a Renderer reference, but is freed when the Renderer reference\n");
	describe_test("is removed, etc.  Several other conditions are also tested that are described\n");
	describe_test("in commentary in the code.\n\n");

	describe_test("Each attribute is tested with the Renderer in the following three states:\n");

	describe_test("\t1.  Idle Renderer, has never been bound to a Drawable\n");
	describe_test("\t2.  Active but quiescent Renderer, currently bound to a Drawable\n");
	describe_test("\t3.  Idle Renderer, has been bound to a Drawable in the past\n\n");

	describe_test("Note that this test assumes, regardless of Renderer dynamics,\n");
	describe_test("that PEXGetRendererAttributes returns the most recently set values.\n");
	describe_test("It also assumes that inquiring a resource ID attribute for a freed\n");
	describe_test("resource will return AlreadyFreed even when the Renderer is active.\n");
	describe_test("This test binds several Renderers to the same window.  It generates no\n");
	describe_test("images.  It succeeds if no messages starting with ERROR are printed to stderr.\n\n");

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
		Set up structures describing base state Renderer attributes.
	*/
	init_default_renderer_attrs (&idle_renderer_attrs);

	active_renderer_attrs = idle_renderer_attrs;
	active_renderer_attrs.renderer_state = PEXRendering;
	active_renderer_attrs.current_path.count = 1;
	active_renderer_attrs.current_path.elements = &current_path_elems;
	active_renderer_attrs.current_path.elements[0].structure = None;
	active_renderer_attrs.current_path.elements[0].offset = 0;

	/*
		Initialize X and PEX, and create a window and a Renderer.
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

	_hppex_test_init_error_handler();

	/*
		Create a window.
	*/

	window = create_and_setup_window (display,
							"PEXlib Renderer Resource Attrs Test", 
							image_depth,
							0, 0, 
							WIDTH, HEIGHT,
							&capx_info);

	if (window == None) {

		_hppex_stderr_print( "PROCESS ERROR:  create_window failed\n");
		exit (1);
	}

	{
		XWindowAttributes wattrs;

		XGetWindowAttributes (display, window, &wattrs);
		vis_template.visualid = XVisualIDFromVisual(wattrs.visual);
		if (visuals = XGetVisualInfo (display, VisualIDMask, 
									&vis_template, &num_visuals)) {
			vis_info = visuals[0];
			XFree (visuals);
		}
	}


	/*
		Now see if there is another visual on the screen that is
		also supported by PEX.  If so, create a window in that
		visual to verify that the Renderer makes sure
		that Lookup Tables are compatible with itself.
	*/
	extra_target_found = False;
	vis_template.screen = screen;
	if (visuals = XGetVisualInfo (display, VisualScreenMask, 
								&vis_template, &num_visuals)) {

		if (num_visuals > 1) {

			for (i=0; i<num_visuals; i++) {

				if (visuals[i].visualid != vis_info.visualid) {

					unsigned long target_count;
					PEXRenderingTarget *target_info;

					PEXMatchRenderingTargets (display, RootWindow(display, screen),
												visuals[i].depth, PEXAnyDrawable,
												visuals[i].visual, 1, 
												&target_count, &target_info);
					if (target_count == 1) {

						XSetWindowAttributes 	attrs;

						if (extra_colormap = XCreateColormap (display, RootWindow(display, screen),
															visuals[i].visual, AllocAll)) {
							attrs.colormap = extra_colormap;
							attrs.background_pixel = 0;
							attrs.border_pixel = 1;

							hints.x = 800;
							hints.y = 20;
							hints.width = 40;
							hints.height = 40;
							hints.flags = (USSize|USPosition);

							if (extra_window = XCreateWindow (display, RootWindow(display,screen),
														 hints.x, hints.y, hints.width, hints.height, 1,
														 visuals[i].depth, InputOutput,
														 visuals[i].visual,
														 (CWBackPixel|CWBorderPixel|CWColormap),&attrs)) {
								XSetNormalHints(display, extra_window, &hints);
								XStoreName (display, extra_window, "Extra Window");
								XMapWindow (display, extra_window);
								XSync (display,0);

								extra_target_found = True;
							}
						}

						if (target_info) XFree (target_info);
					}
				}
			}
		}
	}


	/*
		Create a Structure resource to use in checking the
		case of an valid ID for an inappropriate resource.
	*/
	structure = PEXCreateStructure (display);
	if (structure == None) {
		
		_hppex_stderr_print( "PROCESS ERROR:  Structure creation failed\n");
		exit (1);
	}


	/*
		The following loop is executed three times, to test Renderer
		attribute control under the following situations:

		1.  Idle Renderer, has never been bound to a window
		2.  Quiescent Renderer, currently bound to a window
		3.  Idle Renderer, has been bound to a window in the past

		For each Renderer attribute that is a resource ID,
		each time through the loop, three Renderers are created.
		They are used as follows:

		renderers[0] is used to check that the Renderer gets
		a reference count to the resource that causes it to 
		persist after the resource is freed until the
		Renderer is freed

		renderers[1] is used to check that when a Renderer is
		freed, the reference resource is not freed if it
		has references from elsewhere

		renderers[2] is used to check that replacing a
		reference to one resource with a reference to another
		causes the correct reference count operations,
		and that a redundant change does not result in
		loss of a resource

		Note that all three of these Renderers are bound to the
		same window.
	*/

	for (loop = 0; loop < 3; loop++) {

		_hppex_stderr_print( "%s\n\n", loop_condition[loop]);

		/* 
			====================================================================
			Pipeline Context 
			====================================================================
		*/
		{
			PEXPipelineContext plc[3], pipeline_context;
			PEXPCAttributes *plc_attrs;
			unsigned long pc_value_mask[3];


			if (create_and_bind_renderers (display, window, loop)) {

				_hppex_stderr_print( "\twhile creating renderers for pipeline context testing\n");
				continue;
			}

			set_attrs = check_attrs;
			pc_value_mask[0] = 0;
			pc_value_mask[1] = 0;
			pc_value_mask[2] = 0;
			plc[0] = PEXCreatePipelineContext (display, pc_value_mask, NULL);
			plc[1] = PEXCreatePipelineContext (display, pc_value_mask, NULL);
			plc[2] = PEXCreatePipelineContext (display, pc_value_mask, NULL);
			if ((plc[0] == None) ||
				(plc[1] == None) ||
				(plc[2] == None)) {

				_hppex_stderr_print( "ERROR: failure while creating pipeline contexts\n");
				continue;
			}

			/* 
				First try invalid values 
			*/
			renderer = renderers[0];

			set_attrs.pipeline_context = BAD_RESOURCE_ID;
			_hppex_test_set_expected_error (PEX_ERR, BadPEXPipelineContext, 0, 0, 0);
			PEXChangeRenderer (display, renderer, PEXRAPipelineContext, &set_attrs);
			XSync (display, 0);
			if (_hppex_test_check_expected_error(ERROR_CODE) ||
			   (check_renderer_attrs (display, renderer, 
									&check_attrs, ALL_RENDERER_ATTRS_MASK))) {
				_hppex_stderr_print( "\twhile setting pipeline context to a bad ID\n");
				_hppex_stderr_print( "\n\n");
			}

			set_attrs.pipeline_context = structure;
			_hppex_test_set_expected_error (PEX_ERR, BadPEXPipelineContext, 0, 0, 0);
			PEXChangeRenderer (display, renderer, PEXRAPipelineContext, &set_attrs);
			XSync (display, 0);
			if (_hppex_test_check_expected_error(ERROR_CODE) ||
			   (check_renderer_attrs (display, renderer, 
									&check_attrs, ALL_RENDERER_ATTRS_MASK))) {
				_hppex_stderr_print( "\twhile setting pipeline context to ID of another resource type\n");
				_hppex_stderr_print( "\n\n");
			}

			/*
				Try setting and freeing a resource, should remain due to renderer reference.
			*/
			renderer = renderers[0];
			pipeline_context = plc[0];

			set_attrs.pipeline_context = pipeline_context;
			check_attrs = set_attrs;
			_hppex_test_set_expected_error (NO_ERR, 0, 0, 0, 0);
			PEXChangeRenderer (display, renderer, PEXRAPipelineContext, &set_attrs);
			XSync (display, 0);
			if (_hppex_test_check_expected_error(ERROR_CODE) ||
			   (check_renderer_attrs (display, renderer, 
									&check_attrs, ALL_RENDERER_ATTRS_MASK))) {
				_hppex_stderr_print( "\twhile setting pipeline context to a valid ID %x\n", pipeline_context);
				_hppex_stderr_print( "\n\n");
			}

			PEXFreePipelineContext (display, pipeline_context);
			_hppex_test_set_expected_error (PEX_ERR, BadPEXPipelineContext, 0, 0, 0);
			PEXSetPCAttributeMask(pc_value_mask, PEXPCMarkerType);
			plc_attrs = PEXGetPipelineContext (display, pipeline_context, pc_value_mask);
			XSync (display, 0);
			if (_hppex_test_check_expected_error(ERROR_CODE)) {
				_hppex_stderr_print( "\twhile inquiring a pipeline context that has been freed\n");
				_hppex_stderr_print( "\n\n");
			}
			PEXFreePCAttributes (plc_attrs);

			check_attrs.pipeline_context = PEXAlreadyFreed;
			if (check_renderer_attrs (display, renderer, &check_attrs, PEXRAPipelineContext)) {
				_hppex_stderr_print( "\twhile inquiring ID of a freed pipeline context, expected PEXAlreadyFreed\n");
				_hppex_stderr_print( "\n\n");
			}
			PEXFreeRenderer(display, renderer);


			/*
				Try freeing a renderer that has a reference, make sure resource remains.
			*/
			renderer = renderers[1];
			pipeline_context = plc[1];

			set_attrs.pipeline_context = pipeline_context;
			check_attrs = set_attrs;
			_hppex_test_set_expected_error (NO_ERR, 0, 0, 0, 0);
			PEXChangeRenderer (display, renderer, PEXRAPipelineContext, &set_attrs);
			XSync (display, 0);
			if (_hppex_test_check_expected_error(ERROR_CODE) ||
			   (check_renderer_attrs (display, renderer, 
									&check_attrs, ALL_RENDERER_ATTRS_MASK))) {
				_hppex_stderr_print( "\twhile setting pipeline context to a valid ID %x\n", pipeline_context);
				_hppex_stderr_print( "\n\n");
			}

			_hppex_test_set_expected_error (NO_ERR, 0, 0, 0, 0);
			PEXSetPCAttributeMask(pc_value_mask, PEXPCMarkerType);
			plc_attrs = PEXGetPipelineContext (display, pipeline_context, pc_value_mask);
			XSync (display, 0);
			if (_hppex_test_check_expected_error(ERROR_CODE)) {
				_hppex_stderr_print( "\twhile inquiring a valid pipeline context %x that has a renderer reference\n", pipeline_context);
				_hppex_stderr_print( "\n\n");
			}
			PEXFreePCAttributes (plc_attrs);

			PEXFreeRenderer(display, renderer);
			_hppex_test_set_expected_error (NO_ERR, 0, 0, 0, 0);
			PEXSetPCAttributeMask(pc_value_mask, PEXPCMarkerType);
			plc_attrs = PEXGetPipelineContext (display, pipeline_context, pc_value_mask);
			XSync (display, 0);
			if (_hppex_test_check_expected_error(ERROR_CODE)) {
				_hppex_stderr_print( "%s %x\n%s\n",
						"\twhile inquiring a valid pipeline context", pipeline_context,
						"\tthat has lost a renderer reference via PEXFreeRenderer\n");
				_hppex_stderr_print( "\n\n");
			}

			/*
				Try replacing a resource ID with another one, for each of:

				a different resource
				the null resource
				the same resource
			*/
			renderer = renderers[2];

			set_attrs.pipeline_context = plc[1];
			check_attrs = set_attrs;
			_hppex_test_set_expected_error (NO_ERR, 0, 0, 0, 0);
			PEXChangeRenderer (display, renderer, PEXRAPipelineContext, &set_attrs);
			XSync (display, 0);
			if (_hppex_test_check_expected_error(ERROR_CODE) ||
			   (check_renderer_attrs (display, renderer, 
									&check_attrs, ALL_RENDERER_ATTRS_MASK))) {
				_hppex_stderr_print( "\twhile setting pipeline context to a valid ID %x\n", plc[1]);
				_hppex_stderr_print( "\n\n");
			}

			PEXChangeRenderer (display, renderer, PEXRAPipelineContext, &set_attrs);
			_hppex_test_set_expected_error (NO_ERR, 0, 0, 0, 0);
			PEXSetPCAttributeMask(pc_value_mask, PEXPCMarkerType);
			plc_attrs = PEXGetPipelineContext (display, plc[1], pc_value_mask);
			XSync (display, 0);
			if (_hppex_test_check_expected_error(ERROR_CODE) ||
			   (check_renderer_attrs (display, renderer, 
									&check_attrs, ALL_RENDERER_ATTRS_MASK))) {
				_hppex_stderr_print( "\twhile redundantly setting pipeline context to a valid ID %x\n", plc[1]);
				_hppex_stderr_print( "\n\n");
			}
			PEXFreePCAttributes (plc_attrs);

			PEXFreePipelineContext (display, plc[1]);
			set_attrs.pipeline_context = plc[2];
			check_attrs = set_attrs;
			_hppex_test_set_expected_error (NO_ERR, 0, 0, 0, 0);
			PEXChangeRenderer (display, renderer, PEXRAPipelineContext, &set_attrs);
			XSync (display, 0);
			if (_hppex_test_check_expected_error(ERROR_CODE) ||
			   (check_renderer_attrs (display, renderer, 
									&check_attrs, ALL_RENDERER_ATTRS_MASK))) {
				_hppex_stderr_print( "\twhile replacing one pipeline context ID %x with another %x\n", plc[1], plc[2]);
				_hppex_stderr_print( "\n\n");
			}

			_hppex_test_set_expected_error (PEX_ERR, BadPEXPipelineContext, 0, 0, 0);
			PEXSetPCAttributeMask(pc_value_mask, PEXPCMarkerType);
			plc_attrs = PEXGetPipelineContext (display, plc[1], pc_value_mask);
			XSync (display, 0);
			if (_hppex_test_check_expected_error(ERROR_CODE)) {
				_hppex_stderr_print( "%s %x\n%s\n",
						"\twhile inquiring a freed pipeline context", plc[1],
						"\tthat has lost its renderer reference via PEXChangeRenderer\n");
				_hppex_stderr_print( "\n\n");
			}
			PEXFreePCAttributes (plc_attrs);

			set_attrs.pipeline_context = None;
			check_attrs = set_attrs;
			_hppex_test_set_expected_error (NO_ERR, 0, 0, 0, 0);
			PEXChangeRenderer (display, renderer, PEXRAPipelineContext, &set_attrs);
			XSync (display, 0);
			if (_hppex_test_check_expected_error(ERROR_CODE) ||
			   (check_renderer_attrs (display, renderer, 
									&check_attrs, ALL_RENDERER_ATTRS_MASK))) {
				_hppex_stderr_print( "\twhile setting pipeline context to None\n");
				_hppex_stderr_print( "\n\n");
			}

			PEXFreePipelineContext (display, plc[2]);
			PEXFreeRenderer (display, renderer);
		}
		


		/* 
			====================================================================
			Filter Namesets 
			====================================================================
		*/

#define NUM_FILTER_NAMESET_TYPES 6

		for (i=0; i<NUM_FILTER_NAMESET_TYPES; i++) {

			PEXNameSet nms[3], nameset;
			PEXNameSet *check_attrs_nms, *set_attrs_nms;
			PEXName *nms_attrs;
			unsigned long names_count;
			unsigned long RA_bit;
			char *nms_string;
			

			switch (i) {

			case 0:
				check_attrs_nms = &(check_attrs.highlight_incl);
				set_attrs_nms = &(set_attrs.highlight_incl);
				RA_bit = PEXRAHighlightIncl;
				nms_string = "highlight inclusion";
				break;

			case 1:
				check_attrs_nms = &(check_attrs.highlight_excl);
				set_attrs_nms = &(set_attrs.highlight_excl);
				RA_bit = PEXRAHighlightExcl;
				nms_string = "highlight exclusion";
				break;

			case 2:
				check_attrs_nms = &(check_attrs.invisibility_incl);
				set_attrs_nms = &(set_attrs.invisibility_incl);
				RA_bit = PEXRAInvisibilityIncl;
				nms_string = "invisibility inclusion";
				break;

			case 3:
				check_attrs_nms = &(check_attrs.invisibility_excl);
				set_attrs_nms = &(set_attrs.invisibility_excl);
				RA_bit = PEXRAInvisibilityExcl;
				nms_string = "invisibility exclusion";
				break;

			case 4:
				check_attrs_nms = &(check_attrs.pick_incl);
				set_attrs_nms = &(set_attrs.pick_incl);
				RA_bit = PEXRAPickIncl;
				nms_string = "pick inclusion";
				break;

			case 5:
				check_attrs_nms = &(check_attrs.pick_excl);
				set_attrs_nms = &(set_attrs.pick_excl);
				RA_bit = PEXRAPickExcl;
				nms_string = "pick exclusion";
				break;

			default:
				_hppex_stderr_print( "PROCESS ERROR: unrecognized nameset type\n");
				exit (1);
			}

			if (create_and_bind_renderers (display, window, loop)) {

				_hppex_stderr_print( "\twhile creating renderers for %s test\n", nms_string);
				continue;
			}

			set_attrs = check_attrs;

			nms[0] = PEXCreateNameSet (display);
			nms[1] = PEXCreateNameSet (display);
			nms[2] = PEXCreateNameSet (display);
			if ((nms[0] == None) ||
				(nms[1] == None) ||
				(nms[2] == None)) {

				_hppex_stderr_print( "ERROR: failure while creating namesets for %s test\n", nms_string);
				continue;
			}

			/* 
				First try invalid values 
			*/
			renderer = renderers[0];

			*set_attrs_nms = BAD_RESOURCE_ID;
			_hppex_test_set_expected_error (PEX_ERR, BadPEXNameSet, 0, 0, 0);
			PEXChangeRenderer (display, renderer, RA_bit, &set_attrs);
			XSync (display, 0);
			if (_hppex_test_check_expected_error(ERROR_CODE) ||
			   (check_renderer_attrs (display, renderer, 
									&check_attrs, ALL_RENDERER_ATTRS_MASK))) {
				_hppex_stderr_print( "\twhile setting %s to a bad ID\n", nms_string);
				_hppex_stderr_print( "\n\n");
			}

			*set_attrs_nms = structure;
			_hppex_test_set_expected_error (PEX_ERR, BadPEXNameSet, 0, 0, 0);
			PEXChangeRenderer (display, renderer, RA_bit, &set_attrs);
			XSync (display, 0);
			if (_hppex_test_check_expected_error(ERROR_CODE) ||
			   (check_renderer_attrs (display, renderer, 
									&check_attrs, ALL_RENDERER_ATTRS_MASK))) {
				_hppex_stderr_print( "\twhile setting %s to ID of another resource type\n", nms_string);
				_hppex_stderr_print( "\n\n");
			}

			/*
				Try setting and freeing a resource, should remain due to renderer reference.
			*/
			renderer = renderers[0];
			nameset = nms[0];

			*set_attrs_nms = nameset;
			check_attrs = set_attrs;
			_hppex_test_set_expected_error (NO_ERR, 0, 0, 0, 0);
			PEXChangeRenderer (display, renderer, RA_bit, &set_attrs);
			XSync (display, 0);
			if (_hppex_test_check_expected_error(ERROR_CODE) ||
			   (check_renderer_attrs (display, renderer, 
									&check_attrs, ALL_RENDERER_ATTRS_MASK))) {
				_hppex_stderr_print( "\twhile setting %s to a valid ID %x\n", nms_string, nameset);
				_hppex_stderr_print( "\n\n");
			}

			PEXFreeNameSet (display, nameset);
			_hppex_test_set_expected_error (PEX_ERR, BadPEXNameSet, 0, 0, 0);
			nms_attrs = NULL;
			PEXGetNameSet (display, nameset, &names_count, &nms_attrs);
			XSync (display, 0);
			if (_hppex_test_check_expected_error(ERROR_CODE)) {
				_hppex_stderr_print( "\twhile inquiring a nameset that has been freed\n");
				_hppex_stderr_print( "\n\n");
			}
			if (nms_attrs) XFree (nms_attrs);

			*check_attrs_nms = PEXAlreadyFreed;
			if (check_renderer_attrs (display, renderer, &check_attrs, RA_bit)) {
				_hppex_stderr_print( "\twhile inquiring ID of a freed %s, expected PEXAlreadyFreed\n", nms_string);
				_hppex_stderr_print( "\n\n");
			}
			PEXFreeRenderer(display, renderer);


			/*
				Try freeing a renderer that has a reference, make sure resource remains.
			*/
			renderer = renderers[1];
			nameset = nms[1];

			*set_attrs_nms = nameset;
			check_attrs = set_attrs;
			_hppex_test_set_expected_error (NO_ERR, 0, 0, 0, 0);
			PEXChangeRenderer (display, renderer, RA_bit, &set_attrs);
			XSync (display, 0);
			if (_hppex_test_check_expected_error(ERROR_CODE) ||
			   (check_renderer_attrs (display, renderer, 
									&check_attrs, ALL_RENDERER_ATTRS_MASK))) {
				_hppex_stderr_print( "\twhile setting %s to a valid ID %x\n", nms_string, nameset);
				_hppex_stderr_print( "\n\n");
			}

			_hppex_test_set_expected_error (NO_ERR, 0, 0, 0, 0);
			nms_attrs = NULL;
			PEXGetNameSet (display, nameset, &names_count, &nms_attrs);
			XSync (display, 0);
			if (_hppex_test_check_expected_error(ERROR_CODE)) {
				_hppex_stderr_print( "\twhile inquiring a valid nameset %x that has a renderer reference\n", nameset);
				_hppex_stderr_print( "\n\n");
			}
			if (nms_attrs) XFree (nms_attrs);

			PEXFreeRenderer(display, renderer);
			_hppex_test_set_expected_error (NO_ERR, 0, 0, 0, 0);
			nms_attrs = NULL;
			PEXGetNameSet (display, nameset, &names_count, &nms_attrs);
			XSync (display, 0);
			if (_hppex_test_check_expected_error(ERROR_CODE)) {
				_hppex_stderr_print( "%s %x\n%s\n",
						"\twhile inquiring a valid nameset", nameset,
						"\tthat has lost a renderer reference via PEXFreeRenderer\n");
				_hppex_stderr_print( "\n\n");
			}

			/*
				Try replacing a resource ID with another one, for each of:

				a different resource
				the null resource
				the same resource
			*/
			renderer = renderers[2];

			*set_attrs_nms = nms[1];
			check_attrs = set_attrs;
			_hppex_test_set_expected_error (NO_ERR, 0, 0, 0, 0);
			PEXChangeRenderer (display, renderer, RA_bit, &set_attrs);
			XSync (display, 0);
			if (_hppex_test_check_expected_error(ERROR_CODE) ||
			   (check_renderer_attrs (display, renderer, 
									&check_attrs, ALL_RENDERER_ATTRS_MASK))) {
				_hppex_stderr_print( "\twhile setting %s to a valid ID %x\n", nms_string, nms[1]);
				_hppex_stderr_print( "\n\n");
			}

			PEXChangeRenderer (display, renderer, RA_bit, &set_attrs);
			_hppex_test_set_expected_error (NO_ERR, 0, 0, 0, 0);
			nms_attrs = NULL;
			PEXGetNameSet (display, nms[1], &names_count, &nms_attrs);
			XSync (display, 0);
			if (_hppex_test_check_expected_error(ERROR_CODE) ||
			   (check_renderer_attrs (display, renderer, 
									&check_attrs, ALL_RENDERER_ATTRS_MASK))) {
				_hppex_stderr_print( "\twhile redundantly setting %s to a valid ID %x\n", nms_string, nms[1]);
				_hppex_stderr_print( "\n\n");
			}
			if (nms_attrs) XFree (nms_attrs);

			PEXFreeNameSet (display, nms[1]);
			*set_attrs_nms = nms[2];
			check_attrs = set_attrs;
			_hppex_test_set_expected_error (NO_ERR, 0, 0, 0, 0);
			PEXChangeRenderer (display, renderer, RA_bit, &set_attrs);
			XSync (display, 0);
			if (_hppex_test_check_expected_error(ERROR_CODE) ||
			   (check_renderer_attrs (display, renderer, 
									&check_attrs, ALL_RENDERER_ATTRS_MASK))) {
				_hppex_stderr_print( "\twhile replacing one %s ID %x with another %x\n", nms_string, nms[1], nms[2]);
				_hppex_stderr_print( "\n\n");
			}

			_hppex_test_set_expected_error (PEX_ERR, BadPEXNameSet, 0, 0, 0);
			nms_attrs = NULL;
			PEXGetNameSet (display, nms[1], &names_count, &nms_attrs);
			XSync (display, 0);
			if (_hppex_test_check_expected_error(ERROR_CODE)) {
				_hppex_stderr_print( "%s %x\n%s\n",
						"\twhile inquiring a freed %s", nms_string, nms[1],
						"\tthat has lost its renderer reference via PEXChangeRenderer\n");
				_hppex_stderr_print( "\n\n");
			}
			if (nms_attrs) XFree (nms_attrs);

			*set_attrs_nms = None;
			check_attrs = set_attrs;
			_hppex_test_set_expected_error (NO_ERR, 0, 0, 0, 0);
			PEXChangeRenderer (display, renderer, RA_bit, &set_attrs);
			XSync (display, 0);
			if (_hppex_test_check_expected_error(ERROR_CODE) ||
			   (check_renderer_attrs (display, renderer, 
									&check_attrs, ALL_RENDERER_ATTRS_MASK))) {
				_hppex_stderr_print( "\twhile setting %s to None\n", nms_string);
				_hppex_stderr_print( "\n\n");
			}

			PEXFreeNameSet (display, nms[2]);
			PEXFreeRenderer (display, renderer);
		}



		/* 
			====================================================================
			Lookup Tables
			====================================================================
		*/

#define NUM_LOOKUP_TABLE_TYPES 12

		for (i=0; i<NUM_LOOKUP_TABLE_TYPES; i++) {

			PEXLookupTable lut[3], lookup_table, extra_lut;
			PEXLookupTable *check_attrs_lut, *set_attrs_lut;
			PEXTableIndex *lut_attrs;
			unsigned long index_count;
			unsigned long RA_bit;
			int lut_type;
			char *lut_string;
			

			switch (i+1) {

			case PEXLUTLineBundle:
				check_attrs_lut = &(check_attrs.line_bundle);
				set_attrs_lut = &(set_attrs.line_bundle);
				RA_bit = PEXRALineBundle;
				lut_type = PEXLUTLineBundle;
				lut_string = "line bundle";
				break;

			case PEXLUTMarkerBundle:
				check_attrs_lut = &(check_attrs.marker_bundle);
				set_attrs_lut = &(set_attrs.marker_bundle);
				RA_bit = PEXRAMarkerBundle;
				lut_type = PEXLUTMarkerBundle;
				lut_string = "marker bundle";
				break;

			case PEXLUTTextBundle:
				check_attrs_lut = &(check_attrs.text_bundle);
				set_attrs_lut = &(set_attrs.text_bundle);
				RA_bit = PEXRATextBundle;
				lut_type = PEXLUTTextBundle;
				lut_string = "text bundle";
				break;

			case PEXLUTInteriorBundle:
				check_attrs_lut = &(check_attrs.interior_bundle);
				set_attrs_lut = &(set_attrs.interior_bundle);
				RA_bit = PEXRAInteriorBundle;
				lut_type = PEXLUTInteriorBundle;
				lut_string = "interior bundle";
				break;

			case PEXLUTEdgeBundle:
				check_attrs_lut = &(check_attrs.edge_bundle);
				set_attrs_lut = &(set_attrs.edge_bundle);
				RA_bit = PEXRAEdgeBundle;
				lut_type = PEXLUTEdgeBundle;
				lut_string = "edge bundle";
				break;

			case PEXLUTPattern:
				/* Not currently testing Pattern table */
				continue;
				
			case PEXLUTTextFont:
				check_attrs_lut = &(check_attrs.text_font_table);
				set_attrs_lut = &(set_attrs.text_font_table);
				RA_bit = PEXRATextFontTable;
				lut_type = PEXLUTTextFont;
				lut_string = "text font table";
				break;

			case PEXLUTColor:
				check_attrs_lut = &(check_attrs.color_table);
				set_attrs_lut = &(set_attrs.color_table);
				RA_bit = PEXRAColorTable;
				lut_type = PEXLUTColor;
				lut_string = "color table";
				break;

			case PEXLUTView:
				check_attrs_lut = &(check_attrs.view_table);
				set_attrs_lut = &(set_attrs.view_table);
				RA_bit = PEXRAViewTable;
				lut_type = PEXLUTView;
				lut_string = "view table";
				break;

			case PEXLUTLight:
				check_attrs_lut = &(check_attrs.light_table);
				set_attrs_lut = &(set_attrs.light_table);
				RA_bit = PEXRALightTable;
				lut_type = PEXLUTLight;
				lut_string = "light table";
				break;

			case PEXLUTDepthCue:
				check_attrs_lut = &(check_attrs.depth_cue_table);
				set_attrs_lut = &(set_attrs.depth_cue_table);
				RA_bit = PEXRADepthCueTable;
				lut_type = PEXLUTDepthCue;
				lut_string = "depth cue table";
				break;

			case PEXLUTColorApprox:
				check_attrs_lut = &(check_attrs.color_approx_table);
				set_attrs_lut = &(set_attrs.color_approx_table);
				RA_bit = PEXRAColorApproxTable;
				lut_type = PEXLUTColorApprox;
				lut_string = "color approximation table";
				break;

			default:
				_hppex_stderr_print( "PROCESS ERROR: unrecognized lookup table type\n");
				exit (1);
			}


			if (create_and_bind_renderers (display, window, loop)) {

				_hppex_stderr_print( "\twhile creating renderers for %s test\n", lut_string);
				continue;
			}

			set_attrs = check_attrs;

			lut[0] = PEXCreateLookupTable (display, window, lut_type);
			lut[1] = PEXCreateLookupTable (display, window, lut_type);
			lut[2] = PEXCreateLookupTable (display, window, lut_type);
			if ((lut[0] == None) ||
				(lut[1] == None) ||
				(lut[2] == None)) {

				_hppex_stderr_print( "ERROR: failure while creating LUTs for %s test\n", lut_string);
				continue;
			}

			if (extra_target_found) {
				extra_lut = PEXCreateLookupTable (display, extra_window, lut_type);
				if (extra_lut == None) {
					_hppex_stderr_print( "ERROR: failure while creating extra LUT for %s test\n", lut_string);
					continue;
				}
			}

			/* 
				First try invalid values 
			*/
			renderer = renderers[0];

			*set_attrs_lut = BAD_RESOURCE_ID;
			_hppex_test_set_expected_error (PEX_ERR, BadPEXLookupTable, 0, 0, 0);
			PEXChangeRenderer (display, renderer, RA_bit, &set_attrs);
			XSync (display, 0);
			if (_hppex_test_check_expected_error(ERROR_CODE) ||
			   (check_renderer_attrs (display, renderer, 
									&check_attrs, ALL_RENDERER_ATTRS_MASK))) {
				_hppex_stderr_print( "\twhile setting %s to a bad ID\n", lut_string);
				_hppex_stderr_print( "\n\n");
			}

			*set_attrs_lut = structure;
			_hppex_test_set_expected_error (PEX_ERR, BadPEXLookupTable, 0, 0, 0);
			PEXChangeRenderer (display, renderer, RA_bit, &set_attrs);
			XSync (display, 0);
			if (_hppex_test_check_expected_error(ERROR_CODE) ||
			   (check_renderer_attrs (display, renderer, 
									&check_attrs, ALL_RENDERER_ATTRS_MASK))) {
				_hppex_stderr_print( "\twhile setting %s to ID of another resource type\n", lut_string);
				_hppex_stderr_print( "\n\n");
			}

			if (extra_target_found) {
				*set_attrs_lut = extra_lut;
				_hppex_test_set_expected_error (X_ERR, BadMatch, 0, 0, 0);
				PEXChangeRenderer (display, renderer, RA_bit, &set_attrs);
				XSync (display, 0);
				if (_hppex_test_check_expected_error(ERROR_CODE) ||
				   (check_renderer_attrs (display, renderer, 
										&check_attrs, ALL_RENDERER_ATTRS_MASK))) {
					_hppex_stderr_print( "\twhile setting %s to LUT created for a different target\n", lut_string);
				_hppex_stderr_print( "\n\n");
			}
				PEXFreeLookupTable (display, extra_lut);
			}


			/*
				Try setting and freeing a resource, should remain due to renderer reference.
			*/
			renderer = renderers[0];
			lookup_table = lut[0];

			*set_attrs_lut = lookup_table;
			check_attrs = set_attrs;
			_hppex_test_set_expected_error (NO_ERR, 0, 0, 0, 0);
			PEXChangeRenderer (display, renderer, RA_bit, &set_attrs);
			XSync (display, 0);
			if (_hppex_test_check_expected_error(ERROR_CODE) ||
			   (check_renderer_attrs (display, renderer, 
									&check_attrs, ALL_RENDERER_ATTRS_MASK))) {
				_hppex_stderr_print( "\twhile setting %s to a valid ID %x\n", lut_string, lookup_table);
				_hppex_stderr_print( "\n\n");
			}

			PEXFreeLookupTable (display, lookup_table);
			_hppex_test_set_expected_error (PEX_ERR, BadPEXLookupTable, 0, 0, 0);
			lut_attrs = NULL;
			PEXGetDefinedIndices (display, lookup_table, &index_count, &lut_attrs);
			XSync (display, 0);
			if (_hppex_test_check_expected_error(ERROR_CODE)) {
				_hppex_stderr_print( "\twhile inquiring a lookup table that has been freed\n");
				_hppex_stderr_print( "\n\n");
			}
			if (lut_attrs) XFree (lut_attrs);

			*check_attrs_lut = PEXAlreadyFreed;
			if (check_renderer_attrs (display, renderer, &check_attrs, RA_bit)) {
				_hppex_stderr_print( "\twhile inquiring ID of a freed %s, expected PEXAlreadyFreed\n", lut_string);
				_hppex_stderr_print( "\n\n");
			}
			PEXFreeRenderer(display, renderer);


			/*
				Try freeing a renderer that has a reference, make sure resource remains.
			*/
			renderer = renderers[1];
			lookup_table = lut[1];

			*set_attrs_lut = lookup_table;
			check_attrs = set_attrs;
			_hppex_test_set_expected_error (NO_ERR, 0, 0, 0, 0);
			PEXChangeRenderer (display, renderer, RA_bit, &set_attrs);
			XSync (display, 0);
			if (_hppex_test_check_expected_error(ERROR_CODE) ||
			   (check_renderer_attrs (display, renderer, 
									&check_attrs, ALL_RENDERER_ATTRS_MASK))) {
				_hppex_stderr_print( "\twhile setting %s to a valid ID %x\n", lut_string, lookup_table);
				_hppex_stderr_print( "\n\n");
			}

			_hppex_test_set_expected_error (NO_ERR, 0, 0, 0, 0);
			lut_attrs = NULL;
			PEXGetDefinedIndices (display, lookup_table, &index_count, &lut_attrs);
			XSync (display, 0);
			if (_hppex_test_check_expected_error(ERROR_CODE)) {
				_hppex_stderr_print( "\twhile inquiring a valid lookup table %x that has a renderer reference\n", lookup_table);
				_hppex_stderr_print( "\n\n");
			}
			if (lut_attrs) XFree (lut_attrs);

			PEXFreeRenderer(display, renderer);
			_hppex_test_set_expected_error (NO_ERR, 0, 0, 0, 0);
			lut_attrs = NULL;
			PEXGetDefinedIndices (display, lookup_table, &index_count, &lut_attrs);
			XSync (display, 0);
			if (_hppex_test_check_expected_error(ERROR_CODE)) {
				_hppex_stderr_print( "%s %x\n%s\n",
						"\twhile inquiring a valid lookup table", lookup_table,
						"\tthat has lost a renderer reference via PEXFreeRenderer\n");
				_hppex_stderr_print( "\n\n");
			}

			/*
				Try replacing a resource ID with another one, for each of:

				a different resource
				the null resource
				the same resource
			*/
			renderer = renderers[2];

			*set_attrs_lut = lut[1];
			check_attrs = set_attrs;
			_hppex_test_set_expected_error (NO_ERR, 0, 0, 0, 0);
			PEXChangeRenderer (display, renderer, RA_bit, &set_attrs);
			XSync (display, 0);
			if (_hppex_test_check_expected_error(ERROR_CODE) ||
			   (check_renderer_attrs (display, renderer, 
									&check_attrs, ALL_RENDERER_ATTRS_MASK))) {
				_hppex_stderr_print( "\twhile setting %s to a valid ID %x\n", lut_string, lut[1]);
				_hppex_stderr_print( "\n\n");
			}

			PEXChangeRenderer (display, renderer, RA_bit, &set_attrs);
			_hppex_test_set_expected_error (NO_ERR, 0, 0, 0, 0);
			lut_attrs = NULL;
			PEXGetDefinedIndices (display, lut[1], &index_count, &lut_attrs);
			XSync (display, 0);
			if (_hppex_test_check_expected_error(ERROR_CODE) ||
			   (check_renderer_attrs (display, renderer, 
									&check_attrs, ALL_RENDERER_ATTRS_MASK))) {
				_hppex_stderr_print( "\twhile redundantly setting %s to a valid ID %x\n", lut_string, lut[1]);
				_hppex_stderr_print( "\n\n");
			}
			if (lut_attrs) XFree (lut_attrs);

			PEXFreeLookupTable (display, lut[1]);
			*set_attrs_lut = lut[2];
			check_attrs = set_attrs;
			_hppex_test_set_expected_error (NO_ERR, 0, 0, 0, 0);
			PEXChangeRenderer (display, renderer, RA_bit, &set_attrs);
			XSync (display, 0);
			if (_hppex_test_check_expected_error(ERROR_CODE) ||
			   (check_renderer_attrs (display, renderer, 
									&check_attrs, ALL_RENDERER_ATTRS_MASK))) {
				_hppex_stderr_print( "\twhile replacing one %s ID %x with another %x\n", lut_string, lut[1], lut[2]);
				_hppex_stderr_print( "\n\n");
			}

			_hppex_test_set_expected_error (PEX_ERR, BadPEXLookupTable, 0, 0, 0);
			lut_attrs = NULL;
			PEXGetDefinedIndices (display, lut[1], &index_count, &lut_attrs);
			XSync (display, 0);
			if (_hppex_test_check_expected_error(ERROR_CODE)) {
				_hppex_stderr_print( "%s %x\n%s\n",
						"\twhile inquiring a freed %s", lut_string, lut[1],
						"\tthat has lost its renderer reference via PEXChangeRenderer\n");
				_hppex_stderr_print( "\n\n");
			}
			if (lut_attrs) XFree (lut_attrs);

			*set_attrs_lut = None;
			check_attrs = set_attrs;
			_hppex_test_set_expected_error (NO_ERR, 0, 0, 0, 0);
			PEXChangeRenderer (display, renderer, RA_bit, &set_attrs);
			XSync (display, 0);
			if (_hppex_test_check_expected_error(ERROR_CODE) ||
			   (check_renderer_attrs (display, renderer, 
									&check_attrs, ALL_RENDERER_ATTRS_MASK))) {
				_hppex_stderr_print( "\twhile setting %s to None\n", lut_string);
				_hppex_stderr_print( "\n\n");
			}

			PEXFreeLookupTable (display, lut[2]);
			PEXFreeRenderer (display, renderer);
		}

	} /* loop on Renderer states */


	/*
		Close window and X/PEX.
	*/

	XDestroyWindow(display, window);
	if (extra_target_found) {
		XDestroyWindow(display, extra_window);
		XFreeColormap (display, extra_colormap);
	}

	
    _hppex_close_stderr();

#ifdef XSYSTEM
    closeXSystemTesting(&xtc);
#else
    file_testproc("rnd_resrc.stderr", "rnd_resrc");
	XCloseDisplay (display);
	exit(0);
#endif

} /* resource renderer attribute test */
