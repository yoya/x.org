/* $XConsortium: rnd_trans.c,v 1.0 93/11/22 12:40:00 rws Exp $ */

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
** Renderer resource state transition test
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

static PEXViewport fullframe_data[]={
	{ {0, 0, 0.0},				{WIDTH, HEIGHT, 1.0},		True,	{ 0,0,0 }	},
};

#define NUM_WINDOWS 3
#define BAD_WINDOW_ID 52


#define IDLE		0
#define	RENDERING	1
#define PICK_ONE	2
#define PICK_ALL	3

/*
	This table describes expected values when
	various state transitions are attempted.
*/
static struct _state_value {
	int initial_set_state;
	int initial_expected_state;
	int begin_expected_error_type;
	int begin_expected_error_code;
	int begin_expected_state;
	int end_expected_error_type;
	int end_expected_error_code;
	int end_expected_state;
} state_values[]={

	{ IDLE, PEXIdle,
		NO_ERR, 0, PEXRendering,
		NO_ERR, 0, PEXIdle },
	{ RENDERING, PEXRendering,
		PEX_ERR, BadPEXRendererState, PEXRendering,
		NO_ERR, 0, PEXIdle },
	{ PICK_ONE, PEXPicking,
		PEX_ERR, BadPEXRendererState, PEXRendering,
		PEX_ERR, BadPEXRendererState, PEXPicking },
	{ PICK_ALL, PEXPicking,
		PEX_ERR, BadPEXRendererState, PEXRendering,
		PEX_ERR, BadPEXRendererState, PEXPicking },
};


#define VIEWPORT					0
#define PIPELINE_CONTEXT_ATTR		1
#define NUM_ATTRS 					2

#define NOT_A_RESOURCE		0
#define RESOURCE_ID			1
#define RESOURCE_CONTENTS	2

static struct _attr_info {
	int image_per_setting;
	int subframe_per_setting;
	int last_subframe_in_image;
	int suppress_clear_after_image;
	int resource_attr_type;
	XID *resource_id_attr;
	int resource_RA_bit;
	int lut_type;
	char *attr_name;
} attr_control[]={

	/* VIEWPORT */					{ True, False, False, False,
									NOT_A_RESOURCE, NULL, PEXRAViewport, 0,
									"Viewport" },
	/* PIPELINE_CONTEXT */			{ False, False, False, False,
									RESOURCE_ID, NULL, PEXRAPipelineContext, 0,
									"Pipeline Context ID" },
};

static struct _viewport_value {
	PEXViewport value;
	char *message;
} viewport_values[]={
	{ { {0, 0, 0.0},				{WIDTH, HEIGHT, 1.0},	False,	{ 0,0,0 }	},
	"First viewport value: Full-window viewport" },
	{ { {WIDTH/2, HEIGHT/2, 0.0},	{WIDTH, HEIGHT, 1.0},	False,	{ 0,0,0 }	},
	"Second viewport value: Viewport in upper-right quarter of window" },
};

static struct _plcattr_value {
	PEXEnumTypeIndex	line_type;
	PEXBitmask			asf_enables;
	PEXBitmask			asf_values;
	char *message;
} plcattr_values[]={
	{ PEXLineTypeSolid, 	PEXASFLineType,	PEXASFLineType,
	"First PLC value: Solid line type" },
	{ PEXLineTypeDashed,  	PEXASFLineType,	0,
	"Second PLC value: Dashed line type" },
};


main(argc, argv)
int argc;
char **argv;
{
	Display *display;
	Window window;
	Window windows[NUM_WINDOWS];
	PEXRenderer renderer;
	PEXRendererAttributes set_attrs, expected_attrs;
	unsigned long value_mask;
	PEXExtensionInfo *ext_info;

	char title[256];

	PEXUtWindowSpecification  window_info;
	PEXUtVisualCriteria       criteria;
	Atom                      prop_atom;
	XColor                    returned_background;
	XEvent                    event;
	unsigned int              unmet;
	int                       status;
	XVisualInfo 				vis_info;
	XStandardColormap 			cmap_info;
	PEXColorApproxEntry 		capx_info;
	int 						screen;
	XSizeHints                	hints;

	PEXElementRef current_path_elems;
	PEXStructure structure, substructure;
	long client_structure;
	PEXPickRecord pick_aperture;
	int pick_status, more_hits, nd_hits;
	unsigned long pick_path_count;
	PEXPickPath *pick_paths;

    XVisualInfo       	*visuals;
    int     		  	num_visuals;
    XVisualInfo       	vis_templ;
    unsigned int      	vis_mask;

	int i, j;
	int value_index, trans, state, attr;
	int image_number = 0;

	PEXCoord points[2];
	PEXPipelineContext pipeline_context;
	PEXPCAttributes plc_attrs;

	char *image_desc, image_msg[256];
	char image_name[80], test_name[80];

#ifdef XSYSTEM
    int XSystemValues[20];
#endif

    int			pexmode = 0;
	int			image_depth = 0;

#ifdef XSYSTEM
	openXSystemTesting(argc, argv, &xtc, 0, 0);
#else
    /* Handle other arguments. */
    if (parse_options (argc, argv, &pexmode, &image_depth))
	exit (1);
#endif

    _hppex_set_stderr("rnd_trans.stderr", "rnd_trans");

	describe_test("\n\n");
	describe_test("This is a test of PEXlib Renderer state transitions, in two parts.\n");

	describe_test("The first part tests binding the Renderer to multiple windows in succession,\n");
	describe_test("handling a PEXBegin* call when already active, and handling an inappropriate\n");
	describe_test("PEXEnd* for the current state.  Most of these conditions result in\n");
	describe_test("PEXBadRendererState errors, which are trapped and checked.  This part of the\n");
	describe_test("test does not generate images.  It succeeds if no messages starting with ERROR\n");
	describe_test("are printed to stderr.\n\n");

	describe_test("The second part of the test makes sure that if a Renderer attribute is changed,\n");
	describe_test("the change is effective on each of several windows that the Renderer might\n");
	describe_test("be bound to.  Actually, only a representative Pipeline Context attribute\n");
	describe_test("(line style), and a representative non-resource attribute (viewport), are tested\n\n");

	describe_test("The second part of the test generates images.  To demonstrate the viewport\n");
	describe_test("change, a rectangle is drawn that outlines the NPC subvolume.  To demonstrate\n");
	describe_test("the Pipeline Context change, a horizontal line is drawn.\n\n");

	describe_test("A third part of the test makes sure that primitives are not drawn when the\n");
	describe_test("Renderer is Idle.  This also generates one image.\n\n");

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
		Initialize X and PEX, and create two windows and a Renderer.
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

	strcpy (title, "Rnd Transition #0");
	screen = DefaultScreen(display);
	hints.x      = 100;
	hints.y      = 100;
	hints.width  = WIDTH;
	hints.height = HEIGHT;
	hints.flags  = (USSize|USPosition);

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
					   &window_info, &(windows[0]),
					   &vis_info, &cmap_info, &capx_info,
					   &unmet, &prop_atom,
					   &returned_background);

	if ((status != PEXUtSuccess) &&
		(status != PEXUtQualifiedSuccess)) {
	    fprintf (stderr,
		    "PROCESS ERROR:  First window creation failed\n");
		exit (1);
	}

	window = windows[0];
	XSetWindowColormap(display, window, cmap_info.colormap);
#ifdef XSYSTEM
	XInstallColormap(display, cmap_info.colormap);
#endif
	XMapWindow(display, window);
	XFlush(display);

	/* Wait for the first exposure */
	XSelectInput( display, window, ExposureMask);
	XNextEvent(display, &event);

#ifdef XSYSTEM
    XSystemValues[0] = WINDOW_SMART_HW_CONFIG;
    XSystemValues[1] = windows[0];
    XSystemValues[2] = -1;
    defineXSystemTestControlParm(&xtc, ARCHIVE_MODE, XSystemValues);
#endif

	/*
		Create more windows (assume to be using same 
		colormap and visual).
	*/
	hints.x += 300;

	window_info.attr_mask             = CWColormap;
	window_info.attrs.colormap = cmap_info.colormap;

	for (i=1; i<NUM_WINDOWS; i++) {

		sprintf (title, "Rnd Transition #%d", i);

		window_info.size_hints = hints;
		status = PEXUtCreateWindow ( display, screen,
							&window_info,
							&vis_info,
							&(windows[i]),
							&returned_background);

		if ((status != PEXUtSuccess) &&
			(status != PEXUtQualifiedSuccess)) {

			_hppex_stderr_print( "PROCESS ERROR:  Window creation #%d failed\n", i);
			exit (1);
		}

		XMapWindow(display, windows[i]);
		XFlush(display);

		hints.y += HEIGHT + 50;
	}



	/*
		Create a Renderer.
	*/
	window = windows[0];
	renderer = PEXCreateRenderer (display, window, 0, NULL);
	if (renderer == None) {

		_hppex_stderr_print( "PROCESS ERROR: PEXCreateRenderer failed\n");
		exit (1);
	}

	/*
		Set up Renderer to clear image and Z planes.
	*/
	set_renderer_clear_flags (display, renderer, True, 0.0, 0.0, 0.0);


	/*
		Try a bad drawable ID in BeginRendering.
	*/
	_hppex_stderr_print( "\n\n");
	_hppex_stderr_print( "About to try PEXBeginRendering with a bad Window ID.\n");
	_hppex_test_set_expected_error (X_ERR, BadDrawable, 0, 0, 0);
	PEXBeginRendering (display, BAD_WINDOW_ID, renderer);
	XSync (display, 0);
	if (_hppex_test_check_expected_error (ERROR_CODE)) {
		_hppex_stderr_print( "\twhile calling PEXBeginRendering with a bad drawable\n");
	}
	_hppex_stderr_print( "\n\n");


	
	/*
		If more than one visual is supported on the screen,
		try BeginRendering with a drawable for a different target than
		the Renderer was created for.
	*/
    vis_templ.screen = screen;
    vis_mask = VisualScreenMask;

    if ((NULL != (visuals = XGetVisualInfo(display, vis_mask, &vis_templ, &num_visuals))) &&
		(num_visuals > 1)) {

		int i;
		PEXColorApproxEntry dummy_capx_info;
		XStandardColormap dummy_cmap_info;
		Window dummy_window;
		Colormap cmap_id;
		Atom dummy_prop_atom;

		for (i=0; i<num_visuals; i++)
			if (visuals[i].visualid != vis_info.visualid)
				break;

		status = PEXUtGetStandardColormapInfo (display,  
							&(visuals[i]),
							&dummy_cmap_info,
							&dummy_capx_info,
							&dummy_prop_atom);

		status = PEXUtCreateColormap (display, 
							&(visuals[i]),  
							&dummy_capx_info, 
							&cmap_id);

		hints.x = 700;
		hints.y = 100;
		hints.width = 200;
		hints.height = 100;
		hints.flags = (USSize|USPosition);

		window_info.attr_mask = CWColormap;
		window_info.attrs.colormap = cmap_id;
		window_info.size_hints = hints;
		window_info.title = "Rnd Transition Extra";

		status = PEXUtCreateWindow ( display, screen,
							&window_info,
							&(visuals[i]),
							&dummy_window,
							&returned_background);

		if ((status != PEXUtSuccess) &&
			(status != PEXUtQualifiedSuccess)) {

			_hppex_stderr_print( "PROCESS ERROR:  Extra window creation failed\n");
			exit (1);
		}

		XMapWindow(display, dummy_window);
		XFlush(display);

		_hppex_test_set_expected_error (X_ERR, BadMatch, 0, 0, 0);
		PEXBeginRendering (display, dummy_window, renderer);
		XSync (display, 0);
		if (_hppex_test_check_expected_error (ERROR_CODE)) {
			_hppex_stderr_print( "\twhile calling PEXBeginRendering with an inappropriate target \n");
			_hppex_stderr_print( "\n\n");
		}
		PEXEndRendering (display, renderer, False);

		XDestroyWindow(display, dummy_window);
		XFreeColormap (display, cmap_id);
		XFree (visuals);
	}

	
	/*
		Make sure Renderer can be bound to more than one window, more than
		one time in succession.
	*/
	for (i = 0; i<2; i++) {

		_hppex_test_set_expected_error (NO_ERR, 0, 0, 0, 0);

		for (j=0; j<NUM_WINDOWS; j++) {
			PEXBeginRendering (display, windows[j], renderer);
			XSync (display, 0);
			if (_hppex_test_check_expected_error (ERROR_CODE)) {
				_hppex_stderr_print( "\twhile calling PEXBeginRendering with window %d\n", j);
				_hppex_stderr_print( "\n\n");
			}
			PEXEndRendering (display, renderer, True);
			XSync (display, 0);
			if (_hppex_test_check_expected_error (ERROR_CODE)) {
				_hppex_stderr_print( "\twhile calling PEXEndRendering with window %d\n", j);
				_hppex_stderr_print( "\n\n");
			}
		}
	}


	/*
		Make sure that BeginRendering succeeds regardless of current Renderer state,
		and that EndRendering is a no-op except when in Rendering state.
	*/

	window = windows[0];
	client_structure = 52;
	pick_aperture.volume.min.x = 0.0;
	pick_aperture.volume.min.y = 0.0;
	pick_aperture.volume.min.z = 0.0;
	pick_aperture.volume.max.x = 0.0;
	pick_aperture.volume.max.y = 0.0;
	pick_aperture.volume.max.z = 0.0;

	for (trans = 0; trans < 2; trans++) {
		for (state = 0; state < 4; state++) {

			_hppex_test_set_expected_error (NO_ERR, 0, 0, 0, 0);
			expected_attrs.renderer_state = state_values[state].initial_expected_state;

			switch (state_values[state].initial_set_state) {

			case IDLE:
			break;

			case RENDERING:
			PEXBeginRendering (display, window, renderer);
			break;

			case PICK_ONE:
			PEXBeginPickOne (display, window, renderer, client_structure,
							PEXPickLast,
							PEXPickDeviceNPCHitVolume, &pick_aperture);
			break;

			case PICK_ALL:
			PEXBeginPickAll (display, window, renderer, client_structure,
							PEXPickAllAll, True, 5, 
							PEXPickDeviceNPCHitVolume, &pick_aperture);
			break;
			}

			XSync (display, 0);
			if (_hppex_test_check_expected_error(ERROR_CODE) ||
				(check_renderer_attrs (display, renderer,
								&expected_attrs, PEXRARendererState))) {
				_hppex_stderr_print( "\twhile attempting to enter state %d\n", 
									state_values[state].initial_expected_state);
				_hppex_stderr_print( "\n\n");
			}

			switch (trans) {

			case 0: 
				/* Do BeginRendering while in the initial state */
				_hppex_test_set_expected_error (state_values[state].begin_expected_error_type,
									state_values[state].begin_expected_error_code, 0, 0, 0);
				expected_attrs.renderer_state = state_values[state].begin_expected_state;

				PEXBeginRendering (display, window, renderer);

				XSync (display, 0);
				if (_hppex_test_check_expected_error(ERROR_CODE) ||
					(check_renderer_attrs (display, renderer,
									&expected_attrs, PEXRARendererState))) {
					_hppex_stderr_print( "\twhile attempting to do BeginRendering from state %d\n", 
										state_values[state].initial_expected_state);
					_hppex_stderr_print( "\n\n");
				}
				PEXEndRendering (display, renderer, True);
				break;

			case 1: 
				/* Do EndRendering while in the initial state */
				_hppex_test_set_expected_error (state_values[state].end_expected_error_type,
									state_values[state].end_expected_error_code, 0, 0, 0);
				expected_attrs.renderer_state = state_values[state].end_expected_state;

				PEXEndRendering (display, renderer, True);

				XSync (display, 0);
				if (_hppex_test_check_expected_error(ERROR_CODE) ||
					(check_renderer_attrs (display, renderer,
									&expected_attrs, PEXRARendererState))) {
					_hppex_stderr_print( "\twhile attempting to do EndRendering from state %d\n", 
										state_values[state].initial_expected_state);
					_hppex_stderr_print( "\n\n");
				}

				_hppex_test_set_expected_error (PEX_ERR, BadPEXRendererState, 0, 0, 0);
				PEXBeginRendering (display, window, renderer);
				PEXEndRendering (display, renderer, True);
				XSync (display,0);
				break;
			}
		}
	}
	_hppex_test_set_expected_error (NO_ERR, 0, 0, 0, 0);


	/*
		For a variety of attributes, make sure that installation of a new
		attribute value is done for each of several windows.
	*/

	/* clear all the windows */
	for (i=0; i < NUM_WINDOWS; i++) {
		PEXBeginRendering (display, windows[i], renderer);
		PEXEndRendering (display, renderer, True);
	}

	for (attr=0; attr<NUM_ATTRS; attr++) {
		unsigned long plc_mask[3];
		float y_offset;

		set_attrs.viewport = fullframe_data[0];
		PEXChangeRenderer (display, renderer, PEXRAViewport, &set_attrs);

		/* create resources if necessary */
		switch (attr) {

		case VIEWPORT:
#ifndef XSYSTEM
			if (glob_verbose_mode)
#endif
			printf ("Testing Renderer viewport attribute change\n");
			break;

		case PIPELINE_CONTEXT_ATTR:
#ifndef XSYSTEM
			if (glob_verbose_mode)
#endif
			printf ("Testing pipeline context attribute change\n");

			plc_mask[0] = 0;
			plc_mask[1] = 0;
			plc_mask[2] = 0;
			pipeline_context = PEXCreatePipelineContext (display, plc_mask, NULL);
			set_attrs.pipeline_context = pipeline_context;
			PEXChangeRenderer (display, renderer, PEXRAPipelineContext, &set_attrs);
			break;
		}

		/* for each value for the attribute, set the value while the Renderer is not bound */
		y_offset = 0.2;
		for (value_index=0; value_index < 2; value_index++) {

			switch (attr) {

			case VIEWPORT:
				set_attrs.viewport = viewport_values[value_index].value;
				image_desc = viewport_values[value_index].message;
				PEXChangeRenderer (display, renderer, PEXRAViewport, &set_attrs);
				break;

			case PIPELINE_CONTEXT_ATTR:
				plc_attrs.line_type = plcattr_values[value_index].line_type;
				image_desc = plcattr_values[value_index].message;
				PEXSetPCAttributeMask (plc_mask, PEXPCLineType);
				PEXChangePipelineContext (display, pipeline_context, plc_mask, &plc_attrs);
				break;
			}
#ifndef XSYSTEM
			if (glob_verbose_mode)
#endif
			printf ("%s\n", image_desc);

			for (i=0; i<NUM_WINDOWS; i++) {

#ifndef XSYSTEM
			if (glob_verbose_mode)
#endif
				printf ("Demonstrating attribute change on window %d\n", i);

				PEXBeginRendering (display, windows[i], renderer);

				switch (attr) {

				case VIEWPORT:
					/*
						Draw a hollow rectangle at the NPC limits;
						shows size of viewport assuming NPC refitted
						as expected.
					*/
					draw_primitive (display, renderer, PEXOCRender,
									RECTANGLE,
									0.0, 0.0, 0.0,
									1.0, 1.0, 0.0);
					break;

				case PIPELINE_CONTEXT_ATTR:
					draw_primitive (display, renderer, PEXOCRender,
									LINE,
									0.2, y_offset, 0.0,
									0.6, 0.0, 0.0);
					break;
				}

				PEXEndRendering (display, renderer, True);
			}
			y_offset += 0.2;

			/* capture image of each window */
			XSync (display, 0);
			for (i=0; i<NUM_WINDOWS; i++) {
#ifdef XSYSTEM
				strcpy (image_msg, "ERROR: ");
				strcat (image_msg, image_desc);

				XSystemValues[0] = WINDOW_SMART_EXTENT;
				XSystemValues[1] = windows[i];
				defineXSystemTestControlParm(&xtc, ARCHIVE_EXTENT, XSystemValues);

				XSystemCheck(image_msg, &xtc);
#else
				sprintf(image_name,"rnd_trans_%d", image_number);
				sprintf(test_name, "rnd_trans #%d", image_number);
				 image_testproc(image_name, test_name, img_desc,
								  display, windows[i], 0, 0,
								  WIDTH, HEIGHT);
#endif
				image_number++;
			}
#ifdef XSYSTEM
			printf ("\n");
#endif
		}

		switch (attr) {

		case PIPELINE_CONTEXT_ATTR:
			PEXFreePipelineContext (display, pipeline_context);
			set_attrs.pipeline_context = None;
			PEXChangeRenderer (display, renderer, PEXRAPipelineContext, &set_attrs);
			break;
		}
	}


	/*
		Check whether a typical OC behaves correctly across
		Renderer state transitions.
	*/

#ifndef XSYSTEM
	if (glob_verbose_mode) {
#endif
		printf ("Demonstrating that primitives are disabled when Renderer is Idle\n");
		printf ("Image should only contain two horizontal lines, in the center\n");
#ifndef XSYSTEM
	}
#endif

	window = windows[0];
	points[0].x = 0.2;
	points[1].x = 0.8;
	points[0].z = 0.0;
	points[1].z = 0.0;

	/* clear the window */
	PEXBeginRendering (display, window, renderer);
	PEXEndRendering (display, renderer, True);
	set_renderer_clear_flags (display, renderer, False, 0.0, 0.0, 0.0);
	
	points[0].y = 0.1;
	points[1].y = 0.1;
	PEXPolyline (display, renderer, PEXOCRender, 2, points);
	points[0].y = 0.2;
	points[1].y = 0.2;
	PEXPolyline (display, renderer, PEXOCRender, 2, points);

	PEXBeginRendering (display, window, renderer);

	points[0].y = 0.3;
	points[1].y = 0.3;
	PEXPolyline (display, renderer, PEXOCRender, 2, points);
	points[0].y = 0.4;
	points[1].y = 0.4;
	PEXPolyline (display, renderer, PEXOCRender, 2, points);

	PEXEndRendering (display, renderer, True);

	points[0].y = 0.5;
	points[1].y = 0.5;
	PEXPolyline (display, renderer, PEXOCRender, 2, points);
	points[0].y = 0.6;
	points[1].y = 0.6;
	PEXPolyline (display, renderer, PEXOCRender, 2, points);
	
	/* capture image of window */
	XSync (display, 0);
#ifdef XSYSTEM
    XSystemValues[0] = WINDOW_SMART_EXTENT;
    XSystemValues[1] = window;
    defineXSystemTestControlParm(&xtc, ARCHIVE_EXTENT, XSystemValues);

	XSystemCheck("ERROR: OC stream across Renderer state changes", &xtc);
	printf ("\n");
#else
	sprintf(image_name,"rnd_trans_%d", image_number);
	sprintf(test_name, "rnd_trans #%d", image_number);
	 image_testproc(image_name, test_name, img_desc,
					  display, window, 0, 0,
					  WIDTH, HEIGHT);
#endif
	image_number++;

	/*
		Close Renderer, windows, and X/PEX.
	*/
	PEXFreeRenderer(display, renderer);
	for (i=0; i < NUM_WINDOWS; i++)
		XDestroyWindow(display, windows[i]);

    _hppex_close_stderr();

#ifdef XSYSTEM
    closeXSystemTesting(&xtc);
#else
    file_testproc("rnd_trans.stderr", "rnd_trans");
	XCloseDisplay (display);
	exit(0);
#endif

} /* renderer state transition test */
