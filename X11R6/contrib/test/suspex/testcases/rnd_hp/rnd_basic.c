/* $XConsortium: rnd_basic.c,v 1.0 93/11/22 12:39:17 rws Exp $ */

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
** Renderer resource basic functional test
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

#include "rnd_utils.h"


#ifdef XSYSTEM
XSystemTestingControlStructure xtc;
#endif



typedef struct {
	int	create_windows;
	int num_windows;
	int	initialize_pex;
	int create_renderers;
	int num_renderers;
	int check_renderer_attributes;
	int bind_renderers;
	int unbind_renderers;
	int free_renderers;
	int destroy_windows;
} test_script;

static test_script scripts[]={
	/* 
		create	#	init	create	#	check	bind	unbind	free	destroy
		windows		PEX		renderers	attrs	rnds	rnds	rnds	windows
	*/
	{ 	False, 	0, 	False,	False, 	0, 	False, 	False, 	False, 	False, 	False },
	{ 	True, 	1, 	True, 	False,	0, 	False, 	False,	False, 	False, 	True },
	{ 	True, 	1, 	True, 	True,	1, 	True, 	False,	False,  True,	True },
	{ 	True, 	1, 	True, 	True,	1, 	True, 	True,	True,	True,	True },
	{ 	True, 	2, 	True, 	True,	2, 	False, 	False,	False,	True,	True },
	{ 	True, 	2, 	True, 	True,	2, 	True, 	True,	True,	True,	True },
};

#define NUM_SCRIPTS (sizeof(scripts)/sizeof(test_script))
#define MAX_MULTIPLICITY 2

static dump_script (int loop)
{

	_hppex_stderr_print( "------------------------------\n");
	_hppex_stderr_print( "script number = %d\n", loop);
	_hppex_stderr_print( "create windows = %d, num_windows = %d, destroy_windows = %d\n",
						scripts[loop].create_windows,
						scripts[loop].num_windows,
						scripts[loop].destroy_windows);
	_hppex_stderr_print( "initialize_pex = %d\n",
						scripts[loop].initialize_pex);
	_hppex_stderr_print( "create renderers = %d, num_renderers = %d, free_renderers = %d\n",
						scripts[loop].create_renderers,
						scripts[loop].num_renderers,
						scripts[loop].free_renderers);
	_hppex_stderr_print( "check_renderer_attributes = %d\n",
						scripts[loop].check_renderer_attributes);
	_hppex_stderr_print( "bind renderers = %d, unbind_renderers = %d\n",
						scripts[loop].bind_renderers,
						scripts[loop].unbind_renderers);
	_hppex_stderr_print( "\n");
}

static Window 		windows[MAX_MULTIPLICITY];
static PEXRenderer 	renderers[MAX_MULTIPLICITY];

main(argc, argv)
int argc;
char **argv;
{
	Display *display, *test_display;
	Window window;
	PEXRenderer renderer;
	PEXRendererAttributes default_renderer_attrs, active_renderer_attrs;
	XVisualInfo vis_info;
	XStandardColormap cmap_info;
	PEXColorApproxEntry capx_info;
	PEXElementRef current_path_elems;
	PEXExtensionInfo *ext_info;

	int i, loop;
	int multiplicity;
	char *display_string;

    int			pexmode = 0;
	int			image_depth = 0;

#ifndef XSYSTEM
    /* Handle other arguments. */
    if (parse_options (argc, argv, &pexmode, &image_depth))
	exit (1);
#endif

#ifdef XSYSTEM
	openXSystemTesting(argc, argv, &xtc, 0, 0);
    test_display = xtc.display;
	display_string = DisplayString(test_display);
#else
	display_string = getenv("DISPLAY");
#endif

    _hppex_set_stderr("rnd_basic.stderr", "rnd_basic");

	describe_test("\n\n");
	describe_test("This is a test of the fundamental operations of the PEXlib Renderer:\n");
	describe_test("initializing PEXlib, creating and freeing Renderer resources,\n");
	describe_test("and binding and unbinding Renderers to windows.  It should be\n");
	describe_test("the first test run in the Renderer test set.\n\n");

	describe_test("This test generates no images.  It succeeds if no messages\n");
	describe_test("starting with ERROR are printed to stderr.  Windows will \n");
	describe_test("appear and disappear during its operation but their appearance\n");
	describe_test("is not important.\n\n");

	describe_test("The test interprets a progressive set of scripts that start\n");
	describe_test("out by simply trying XOpenDisplay/XCloseDisplay, then creating \n");
	describe_test("Windows, then creating Renderers, etc.  The operations performed\n");
	describe_test("by each script are printed to stderr and will become part of the\n");
	describe_test("archive.\n\n");

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
		Set up structures describing expected Renderer attributes.
	*/
	init_default_renderer_attrs (&default_renderer_attrs);

	active_renderer_attrs = default_renderer_attrs;
	active_renderer_attrs.renderer_state = PEXRendering;
	active_renderer_attrs.current_path.count = 1;
	active_renderer_attrs.current_path.elements = &current_path_elems;
	active_renderer_attrs.current_path.elements[0].structure = None;
	active_renderer_attrs.current_path.elements[0].offset = 0;


	/*
		The following loop is executed several times, each time
		including more calls to Renderer functions, as controlled
		by the local scripts defined above.
	*/

	for (loop = 0; loop < NUM_SCRIPTS; loop++) {

		/* Always print out what's going on. */
		dump_script (loop);

		if (NULL == (display = XOpenDisplay (display_string))) {

			_hppex_stderr_print( "PROCESS ERROR:  XOpenDisplay failed on %s\n", display_string);
			continue;
		}

		if (scripts[loop].initialize_pex) {

			if (PEXInitialize (display, &ext_info, 0, NULL))
			{
				_hppex_stderr_print( "PROCESS ERROR:  PEXInitialize failed on %s\n", display_string);
				continue;
			}
		}

		if (scripts[loop].create_windows) {

			for (i = 0; i < scripts[loop].num_windows; i++) {

				windows[i] = create_and_setup_window (display,
										"PEXlib Renderer Basic Test", 
										image_depth,
										0, 0, 200, 200,
										&capx_info);

				if (None == windows[i]) {
					_hppex_stderr_print( "PROCESS ERROR:  create_window failed on window %d\n", i); 
					continue;
				}


			}
		}

		if (scripts[loop].create_renderers) {

			for (i = 0; i < scripts[loop].num_renderers; i++) {

				window = windows[i % scripts[loop].num_windows];
				if (None == (renderers[i] = PEXCreateRenderer (display, window, 0, NULL))) {

					_hppex_stderr_print( "ERROR: PEXCreateRenderer failed on renderer %d using window %d\n",
										i, (i % scripts[loop].num_windows));
					continue;
				}

				if (scripts[loop].check_renderer_attributes)
					if (check_renderer_attrs (display, renderers[i], 
									&default_renderer_attrs, 
									ALL_RENDERER_ATTRS_MASK)) {
						_hppex_stderr_print( "ERROR: attributes check after create failed on renderer %d\n", i);
						continue;
					}
			}

			multiplicity = MAX(scripts[loop].num_windows,
								scripts[loop].num_renderers);

			for (i = 0; i < multiplicity; i++) {

				window = windows[i % scripts[loop].num_windows];
				renderer = renderers[i % scripts[loop].num_renderers];

				if (scripts[loop].bind_renderers) {

					PEXBeginRendering(display, window, renderer);

					if (scripts[loop].check_renderer_attributes)
						if (check_renderer_attrs (display, renderers[i], 
										  &active_renderer_attrs, 
										ALL_RENDERER_ATTRS_MASK)) {
							_hppex_stderr_print( "ERROR: attributes check after bind failed on renderer %d\n", i);
							continue;
						}
				}

				if (scripts[loop].unbind_renderers) {

					PEXEndRendering(display, renderer, True);

					if (scripts[loop].check_renderer_attributes)
						if (check_renderer_attrs (display, renderers[i], 
										  &default_renderer_attrs, 
										ALL_RENDERER_ATTRS_MASK)) {
							_hppex_stderr_print( "ERROR: attributes check after unbind failed on renderer %d\n", i);
							continue;
						}
				}
			}

			if (scripts[loop].free_renderers) {

				for (i = 0; i < scripts[loop].num_renderers; i++)

					PEXFreeRenderer(display, renderers[i]);
			}
		}

		if (scripts[loop].destroy_windows) {

			for (i = 0; i < scripts[loop].num_windows; i++)

				XDestroyWindow(display, windows[i]);
		}

		XCloseDisplay (display);
	
	}


    _hppex_close_stderr();

#ifdef XSYSTEM
    closeXSystemTesting(&xtc);
#else
    file_testproc("rnd_basic.stderr", "rnd_basic");
	exit(0);
#endif

} /* basic renderer test */
