/* $XConsortium: rnd_nresrc.c,v 1.0 93/11/22 12:39:41 rws Exp $ */

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
** Renderer non-resource attribute set/inquire test
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


#define BAD_RA_MASK (~ALL_RENDERER_ATTRS_MASK)
#define SMALLNUM 1.0E-6

#ifdef XSYSTEM
#define WIDTH 300
#define HEIGHT 500
#else
#define WIDTH 200
#define HEIGHT 200
#endif

/*
	For each Renderer attribute, this test tries each of a set of
	values described by the static arrays below.  Both valid and
	invalid values can be included.  

	Note that the error_type field in each array element indicates 
	whether the value is valid or not.  In general, invalid
	values should result in an error being reported and no
	change to the Renderer attribute settings, whereas a valid
	value should be successfully loaded into the Renderer.

	The test program carries the last valid set value forward 
	to the section for testing the next attribute.  It should
	not be the same as the default value for the attribute.

    In the case of complex attributes that contain lists, the
	test program plugs the list address into the PEXRendererAttributes
	struct.  Note that in these cases, the ordering of the valid
	values is significant.  Lists are set to the same size, grown,
	or shrunk, in order to exercise attribute storage allocation 
	in the Renderer.
*/


static struct _hlhsr_value {
	int error_type;
	int error_code;
	PEXEnumTypeIndex value;
} hlhsr_values[]={
	{ X_ERR, BadValue, 	(-1)				},
	{ X_ERR, BadValue, 	999 				},

	{ NO_ERR, 0,		 	PEXHLHSROff			},
	{ NO_ERR, 0, 			PEXHLHSRZBuffer		},
	{ NO_ERR, 0,		 	PEXHLHSRZBufferID	},
};
#define NUM_HLHSR_VALUES (sizeof(hlhsr_values)/sizeof(struct _hlhsr_value))

/*
typedef struct {
    PEXCoord            min;
    PEXCoord            max;
} PEXNPCSubVolume;
*/
static struct _npc_value {
	int error_type;
	int error_code;
	PEXNPCSubVolume value;
} npc_values[]={
	{ X_ERR, BadValue,	{ {-1.0, 0.0, 0.0}, 			{1.0, 1.0, 1.0} } 			},
	{ X_ERR, BadValue,	{ {0.0, 0.0, 0.0}, 				{1.0, 2.0, 1.0} } 			},
	{ X_ERR, BadValue,	{ {0.0, 0.0, 0.0}, 				{1.0, 1.0, 1.1} } 			},

	{ X_ERR, BadValue,	{ {1.0, 0.0, 0.0}, 				{0.0, 1.0, 1.0} } 			},
	{ X_ERR, BadValue,	{ {0.0, 1.0, 0.0}, 				{1.0, 0.0, 1.0} } 			},
	{ X_ERR, BadValue,	{ {0.0, 0.0, 1.0}, 				{1.0, 1.0, 0.0} } 			},

	{ X_ERR, BadValue,	{ {0.5, 0.0, 0.0}, 				{0.5, 1.0, 1.0} } 			},
	{ X_ERR, BadValue,	{ {0.0, 0.5, 0.0}, 				{1.0, 0.5, 1.0} } 			},

	{ NO_ERR, 0,			{ {0.0, 0.0, 0.0}, 				{1.0, 1.0, 1.0} } 			},

	{ NO_ERR, 0,			{ {0.0, 0.0, 0.0}, 				{1.1*SMALLNUM, 1.0, 1.0} } 	},
	{ NO_ERR, 0,			{ {0.0, 0.0, 0.0}, 				{1.0, 1.1*SMALLNUM, 1.0} } 	},
	{ NO_ERR, 0,			{ {0.0, 0.0, 0.0}, 				{1.0, 1.0, 1.1*SMALLNUM} }	},

	{ NO_ERR, 0,			{ {1.0-1.1*SMALLNUM, 0.0, 0.0}, {1.0, 1.0, 1.0} } 			},
	{ NO_ERR, 0,			{ {0.0, 1.0-1.1*SMALLNUM, 0.0}, {1.0, 1.0, 1.0} } 			},
	{ NO_ERR, 0,			{ {0.0, 0.0, 1.0-1.1*SMALLNUM}, {1.0, 1.0, 1.0} } 			},
};
#define NUM_NPC_VALUES (sizeof(npc_values)/sizeof(struct _npc_value))

/*
typedef struct {
    PEXDeviceCoord      min;
    PEXDeviceCoord      max;
    PEXSwitch           use_drawable;
    unsigned char       reserved[3];
} PEXViewport;
*/
static struct _viewport_value {
	int error_type;
	int error_code;
	PEXViewport value;
} viewport_values[]={
	{ X_ERR, BadValue,	{ {0, 0, 0.0},		{WIDTH, HEIGHT, 1.0},	-1,		{ 0,0,0 }	} },

	{ X_ERR, BadValue,	{ {50, 0, 0.0},		{50, HEIGHT, 1.0},		False,	{ 0,0,0 }	} },
	{ X_ERR, BadValue,	{ {0, 50, 0.0},		{WIDTH, 50, 1.0},		False,	{ 0,0,0 }	} },

	{ X_ERR, BadValue,	{ {0, 0, -1.0},		{WIDTH, HEIGHT, 1.0},	False,	{ 0,0,0 }	} },
	{ X_ERR, BadValue,	{ {0, 0, 0.0},		{WIDTH, HEIGHT, 2.0},	False,	{ 0,0,0 }	} },

	{ X_ERR, BadValue,	{ {WIDTH, 0, 0.0},	{0, HEIGHT, 1.0},		False,	{ 0,0,0 }	} },
	{ X_ERR, BadValue,	{ {0, HEIGHT, 0.0},	{WIDTH, 0, 1.0},		False,	{ 0,0,0 }	} },
	{ X_ERR, BadValue,	{ {0, 0, 1.0},		{WIDTH, HEIGHT, 0.0},	False,	{ 0,0,0 }	} },

	{ NO_ERR, 0,			{ {0, 0, 0.0},		{WIDTH, HEIGHT, 1.0},	False,	{ 0,0,0 }	} },
	{ NO_ERR, 0,			{ {0, 0, 0.0},		{WIDTH, HEIGHT, 1.0},	True,	{ 0,0,0 }	} },

	{ NO_ERR, 0,			{ {100, 100, 0.0},	{200, 200, 1.0},		False,	{ 0,0,0 }	} },
	{ NO_ERR, 0,			{ {0, 0, 0.0},		{1, 1, 1.0},			False,	{ 0,0,0 }	} },
	{ NO_ERR, 0,			{ {-20, 0, 0.0},	{WIDTH, HEIGHT, 1.0},	False,	{ 0,0,0 }	} },
	{ NO_ERR, 0,			{ {0, -30, 0.0},	{WIDTH, HEIGHT, 1.0},	False,	{ 0,0,0 }	} },

	{ NO_ERR, 0,			{ {0, 0, 0.0},		{WIDTH+100, HEIGHT,1.0},False,	{ 0,0,0 }	} },
	{ NO_ERR, 0,			{ {0, 0, 0.0},		{WIDTH, HEIGHT+100,1.0},False,	{ 0,0,0 }	} },

	{ NO_ERR, 0,			{ {0, 0, 0.0},		{WIDTH-1, HEIGHT, 1.0},	False,	{ 0,0,0 }	} },
	{ NO_ERR, 0,			{ {0, 0, 0.0},		{WIDTH, HEIGHT-1, 1.0},	False,	{ 0,0,0 }	} },
};
#define NUM_VIEWPORT_VALUES (sizeof(viewport_values)/sizeof(struct _viewport_value))

/*
typedef struct {
    short               xmin;
    short               ymin;
    short               xmax;
    short               ymax;
} PEXDeviceRect;

typedef struct {
    unsigned short      count;
    PEXDeviceRect       *rectangles;
} PEXListOfClipRect;
*/
static struct _cliplist_value {
	int error_type;
	int error_code;
	unsigned short count;
	PEXDeviceRect rectangles[4];
} cliplist_values[]={
	{ X_ERR, BadValue,	1,	{ {1,0,0,1},	{0,0,0,0},	{0,0,0,0},	{0,0,0,0}	} },
	{ X_ERR, BadValue,	1,	{ {0,1,1,0},	{0,0,0,0},	{0,0,0,0},	{0,0,0,0}	} },
	{ X_ERR, BadValue,	4,	{ {0,0,1,1},	{20,30,21,31},	{50,60,51,61},	{80,90,79,91}	} },

	{ X_ERR, BadValue,	1,	{ {21,0,20,1},	{0,0,0,0},	{0,0,0,0},	{0,0,0,0}	} },
	{ X_ERR, BadValue,	1,	{ {0,21,1,20},	{0,0,0,0},	{0,0,0,0},	{0,0,0,0}	} },

	{ NO_ERR, 0,			0,	{ {0,0,0,0},	{0,0,0,0},	{0,0,0,0},	{0,0,0,0}	} },
	{ NO_ERR, 0,			1,	{ {0,0,1,1},	{0,0,0,0},	{0,0,0,0},	{0,0,0,0}	} },
	{ NO_ERR, 0,			1,	{ {0,0,20,20},	{0,0,0,0},	{0,0,0,0},	{0,0,0,0}	} },
	{ NO_ERR, 0,			2,	{ {0,0,1,1},	{20,30,21,31},	{0,0,0,0},	{0,0,0,0}	} },
	{ NO_ERR, 0,			4,	{ {0,0,1,1},	{20,30,21,31},	{50,60,51,61},	{80,90,81,91}	} },
	{ NO_ERR, 0,			4,	{ {10,10,11,11},	{120,130,121,131},	{150,160,151,161},	{180,190,181,191}	} },
	{ NO_ERR, 0,			1,	{ {0,0,3,3},	{0,0,0,0},	{0,0,0,0},	{0,0,0,0}	} },
	{ NO_ERR, 0,			1,	{ {0,0,30,30},	{0,0,0,0},	{0,0,0,0},	{0,0,0,0}	} },
	{ NO_ERR, 0,			0,	{ {0,0,0,0},	{0,0,0,0},	{0,0,0,0},	{0,0,0,0}	} },

	{ NO_ERR, 0,			1,	{ {0,0,WIDTH,HEIGHT},	{0,0,0,0},	{0,0,0,0},	{0,0,0,0}	} },
	{ NO_ERR, 0,			1,	{ {100,100,200,200},	{0,0,0,0},	{0,0,0,0},	{0,0,0,0}	} },
	{ NO_ERR, 0,			1,	{ {0,0,WIDTH-1,HEIGHT-1},	{0,0,0,0},	{0,0,0,0},	{0,0,0,0}	} },
	{ NO_ERR, 0,			1,	{ {0,0,WIDTH+100,HEIGHT+100},	{0,0,0,0},	{0,0,0,0},	{0,0,0,0}	} },
	{ NO_ERR, 0,			1,	{ {-100,-100,WIDTH+100,HEIGHT+100},	{0,0,0,0},	{0,0,0,0},	{0,0,0,0}	} },
};
#define NUM_CLIPLIST_VALUES (sizeof(cliplist_values)/sizeof(struct _cliplist_value))

/*
typedef struct {
    PEXStructure        structure;
    unsigned long       offset;
} PEXElementRef;

typedef struct {
    unsigned long       count;
    PEXElementRef       *elements;
} PEXStructurePath;
*/
static struct _pickstartpath_value {
	int error_type;
	int error_code;
	unsigned long count;
	PEXElementRef elements[4];
} pickstartpath_values[]={
	{ NO_ERR, 0,			0,		{ {0,0},	{0,0},	{0,0},	{0,0} } },
	{ NO_ERR, 0,			1,		{ {1,0},	{0,0},	{0,0},	{0,0} } },
	{ NO_ERR, 0,			1,		{ {2,10},	{0,0},	{0,0},	{0,0} } },
	{ NO_ERR, 0,			2,		{ {3,20},	{4,30},	{0,0},	{0,0} } },
	{ NO_ERR, 0,			4,		{ {1,0},	{2,20},	{3,40},	{4,60} } },
	{ NO_ERR, 0,			4,		{ {10,0},	{20,20},	{30,40},	{40,60} } },
	{ NO_ERR, 0,			1,		{ {1,0},	{0,0},	{0,0},	{0,0} } },
	{ NO_ERR, 0,			1,		{ {2,10},	{0,0},	{0,0},	{0,0} } },
	{ NO_ERR, 0,			0,		{ {0,0},	{0,0},	{0,0},	{0,0} } },

	{ NO_ERR, 0,			1,		{ {-23493,0},	{0,0},	{0,0},	{0,0} } },
	{ NO_ERR, 0,			1,		{ {1,-23493},	{0,0},	{0,0},	{0,0} } },
};
#define NUM_PICKSTARTPATH_VALUES (sizeof(pickstartpath_values)/sizeof(struct _pickstartpath_value))

static struct _background_value {
	int error_type;
	int error_code;
	PEXColorType type;
	PEXTableIndex index;
	float red, green, blue;
} background_values[]={
	{ PEX_ERR, BadPEXColorType,	PEXColorTypeHLS,		0,		1.0, 1.0, 1.0	},

	{ X_ERR, BadValue,		PEXColorTypeIndexed,	-1,		0.0, 1.0, 1.0	},
	{ X_ERR, BadValue,		PEXColorTypeIndexed,	65535,	0.0, 1.0, 1.0	},

	{ X_ERR, BadValue,		PEXColorTypeRGB,		0,		-1.0, 1.0, 1.0	},
	{ X_ERR, BadValue,		PEXColorTypeRGB,		0,		0.0, 2.0, 1.0	},
	{ X_ERR, BadValue,		PEXColorTypeRGB,		0,		0.0, 1.0, 2.0	},

	{ NO_ERR, 0,				PEXColorTypeIndexed,	0,		1.0, 1.0, 1.0	},
	{ NO_ERR, 0,				PEXColorTypeIndexed,	1,		1.0, 1.0, 1.0	},
	{ NO_ERR, 0,				PEXColorTypeIndexed,	31,		1.0, 1.0, 1.0	},
	{ NO_ERR, 0,				PEXColorTypeIndexed,	255,	1.0, 1.0, 1.0	},
	{ NO_ERR, 0,				PEXColorTypeIndexed,	65534,	1.0, 1.0, 1.0	},

	{ NO_ERR, 0,				PEXColorTypeRGB,		0,		0.0, 0.0, 0.0	},
	{ NO_ERR, 0,				PEXColorTypeRGB,		0,		1.0, 1.0, 1.0	},
	{ NO_ERR, 0,				PEXColorTypeRGB,		0,		0.4, 0.5, 0.6	},
};
#define NUM_BACKGROUND_VALUES (sizeof(background_values)/sizeof(struct _background_value))

static struct _clearflag_value {
	int error_type;
	int error_code;
	int value;
} clearflag_values[]={
	{ X_ERR, BadValue,	-1		},

	{ NO_ERR, 0,			False	},
	{ NO_ERR, 0,			True	},
};
#define NUM_CLEARFLAG_VALUES (sizeof(clearflag_values)/sizeof(struct _clearflag_value))

static struct _echomode_value {
	int error_type;
	int error_code;
	PEXEnumTypeIndex value;
} echomode_values[]={
	{ X_ERR, BadValue,	-1				},
	{ X_ERR, BadValue,	999				},

	{ NO_ERR, 0,			PEXNoEcho		},
	{ NO_ERR, 0,			PEXEcho			},
	{ NO_ERR, 0,			PEXUnEcho		},
};
#define NUM_ECHOMODE_VALUES (sizeof(echomode_values)/sizeof(struct _echomode_value))

static struct _rndstate_value {
	int error_type;
	int error_code;
	int value;
} rndstate_values[]={
	{ X_ERR, 0,		PEXIdle			},
	{ X_ERR, 0,		PEXRendering	},
	{ X_ERR, 0,		PEXPicking		},
	{ X_ERR, 0,		-1				},
};
#define NUM_RNDSTATE_VALUES (sizeof(rndstate_values)/sizeof(struct _rndstate_value))

static struct _currentpath_value {
	int error_type;
	int error_code;
	unsigned long count;
	PEXElementRef elements[4];
} currentpath_values[]={
	{ X_ERR, 0,	0,		{ {0,0},	{0,0},	{0,0},	{0,0} } },
	{ X_ERR, 0,	1,		{ {1,0},	{0,0},	{0,0},	{0,0} } },
	{ X_ERR, 0,	1,		{ {2,10},	{0,0},	{0,0},	{0,0} } },
	{ X_ERR, 0,	2,		{ {3,20},	{4,30},	{0,0},	{0,0} } },
	{ X_ERR, 0,	4,		{ {1,0},	{2,20},	{3,40},	{4,60} } },
	{ X_ERR, 0,	4,		{ {10,0},	{20,20},	{30,40},	{40,60} } },
	{ X_ERR, 0,	1,		{ {1,0},	{0,0},	{0,0},	{0,0} } },
	{ X_ERR, 0,	1,		{ {2,10},	{0,0},	{0,0},	{0,0} } },
	{ X_ERR, 0,	0,		{ {0,0},	{0,0},	{0,0},	{0,0} } },

	{ X_ERR, 0,	1,		{ {-23493,0},	{0,0},	{0,0},	{0,0} } },
	{ X_ERR, 0,	1,		{ {1,-23493},	{0,0},	{0,0},	{0,0} } },
};
#define NUM_CURRENTPATH_VALUES (sizeof(currentpath_values)/sizeof(struct _currentpath_value))


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
	Window window;
	PEXRenderer renderer;
	PEXRendererAttributes idle_renderer_attrs, active_renderer_attrs;
	PEXRendererAttributes set_attrs, check_attrs, *base_attrs;
	XVisualInfo vis_info;
	XStandardColormap cmap_info;
	PEXColorApproxEntry capx_info;
	PEXElementRef current_path_elems;
	int screen;
	XSizeHints hints;
	PEXExtensionInfo *ext_info;

	int i, j, loop;
	int multiplicity;

    int			pexmode = 0;
	int			image_depth = 0;

#ifdef XSYSTEM
	openXSystemTesting(argc, argv, &xtc, 0, 0);
#else
    /* Handle other arguments. */
    if (parse_options (argc, argv, &pexmode, &image_depth))
	exit (1);
#endif

    _hppex_set_stderr("rnd_nresrc.stderr", "rnd_nresrc");

	describe_test("\n\n");
	describe_test("This is a test for setting and inquiry of attributes of the PEXlib Renderer\n");
	describe_test("that are not resource IDs.  It tests each attribute by setting the\n");
	describe_test("attribute to both invalid and valid values, trapping expected errors.\n");
	describe_test("Each attribute is tested with the Renderer in the following three\n");
	describe_test("states:\n\n");

	describe_test("\t1.  Idle Renderer, has never been bound to a Drawable\n");
	describe_test("\t2.  Active but quiescent Renderer, currently bound to a Drawable\n");
	describe_test("\t3.  Idle Renderer, has been bound to a Drawable in the past\n\n");

	describe_test("Note that this test assumes, regardless of Renderer dynamics,\n");
	describe_test("that PEXGetRendererAttributes returns the most recently set values.\n");
	describe_test("The test generates no images.  It succeeds if no messages starting\n");
	describe_test("with ERROR are printed to stderr.\n\n");

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

	/*
		Install the error handler for error trapping.
	*/

	_hppex_test_init_error_handler();


	/*
		Create a window.
	*/

	window = create_and_setup_window (display,
							"PEXlib Renderer Non-resource Attrs Test", 
							image_depth,
							0, 0, 
							WIDTH+1, HEIGHT+1,
							&capx_info);


	if (window == None) {

		_hppex_stderr_print( "PROCESS ERROR:  create_window failed\n");
		exit (1);
	}

	/*
		Create a Renderer.
	*/
	renderer = PEXCreateRenderer (display, window, NULL, 0);
	if (renderer == None) {

		_hppex_stderr_print( "PROCESS ERROR: PEXCreateRenderer failed\n");
		exit (1);
	}


	/*
		The following loop is executed three times, to test Renderer
		attribute control under the listed conditions.

		If the Renderer doesn't have immediate dynamics, three loops is
		overkill.
	*/

	for (loop = 0; loop < 3; loop++) {

		_hppex_stderr_print( "%s\n\n", loop_condition[loop]);

		/*
			Choose the base attribute state against which to check.
		*/
		if (loop == 1) {

			base_attrs = &active_renderer_attrs;

			PEXBeginRendering(display, window, renderer);
		}
		else {
			base_attrs = &idle_renderer_attrs;
		}

		if (check_renderer_attrs (display, renderer,
						base_attrs,
						ALL_RENDERER_ATTRS_MASK)) {

			_hppex_stderr_print( "ERROR: base attributes check failed\n");
		}

		
		/* 
			We will cumulatively modify various attrs from the base state. 
			For each attribute, both valid and invalid values are tried.

			set_attrs holds the values to be installed, which are sometimes invalid.
			check_attrs holds the expected values after the install.
		*/
		set_attrs = *base_attrs;
		check_attrs = *base_attrs;


		/* 
			HLHSR Mode 
		*/
		for (i=0; i<NUM_HLHSR_VALUES; i++) {
			set_attrs.hlhsr_mode = hlhsr_values[i].value;

			if (hlhsr_values[i].error_type == NO_ERR) {
				_hppex_test_set_expected_error (NO_ERR, 0, 0, 0, 0);
				check_attrs = set_attrs;
			}
			else
				_hppex_test_set_expected_error (hlhsr_values[i].error_type,
									hlhsr_values[i].error_code, 0, 0, 0);

			PEXChangeRenderer (display, renderer, PEXRAHLHSRMode, &set_attrs);
			XSync (display, 0);
			if (_hppex_test_check_expected_error(ERROR_CODE) ||
			   (check_renderer_attrs (display, renderer,
							&check_attrs,
							ALL_RENDERER_ATTRS_MASK))) {

				_hppex_stderr_print( 
						"\twhile setting HLHSR mode to %d\n",
						set_attrs.hlhsr_mode
						);
				_hppex_stderr_print( "\n\n");
			}
		}


		/* 
			NPC Subvolume 
		*/
		for (i=0; i<NUM_NPC_VALUES; i++) {
			set_attrs.npc_subvolume = npc_values[i].value;

			if (npc_values[i].error_type == NO_ERR) {
				_hppex_test_set_expected_error (NO_ERR, 0, 0, 0, 0);
				check_attrs = set_attrs;
			}
			else
				_hppex_test_set_expected_error (npc_values[i].error_type,
									npc_values[i].error_code, 0, 0, 0);

			PEXChangeRenderer (display, renderer, PEXRANPCSubVolume, &set_attrs);
			XSync (display, 0);
			if (_hppex_test_check_expected_error(ERROR_CODE) ||
			   (check_renderer_attrs (display, renderer,
							&check_attrs,
							ALL_RENDERER_ATTRS_MASK))) {

				_hppex_stderr_print( 
						"\twhile setting NPC subvolume to (%f, %f, %f) (%f, %f, %f)\n", 
						set_attrs.npc_subvolume.min.x,
						set_attrs.npc_subvolume.min.y,
						set_attrs.npc_subvolume.min.z,
						set_attrs.npc_subvolume.max.x,
						set_attrs.npc_subvolume.max.y,
						set_attrs.npc_subvolume.max.z
						);
				_hppex_stderr_print( "\n\n");
			}
		}


		/* 
			Viewport 
		*/
		for (i=0; i<NUM_VIEWPORT_VALUES; i++) {
			set_attrs.viewport = viewport_values[i].value;

			if (viewport_values[i].error_type == NO_ERR) {
				_hppex_test_set_expected_error (NO_ERR, 0, 0, 0, 0);
				check_attrs = set_attrs;
			}
			else
				_hppex_test_set_expected_error (viewport_values[i].error_type,
									viewport_values[i].error_code, 0, 0, 0);

			PEXChangeRenderer (display, renderer, PEXRAViewport, &set_attrs);
			XSync (display, 0);
			if (_hppex_test_check_expected_error(ERROR_CODE) ||
			   (check_renderer_attrs (display, renderer,
							&check_attrs,
							ALL_RENDERER_ATTRS_MASK))) {

				_hppex_stderr_print( 
						"\twhile setting viewport to (%d, %d, %f) (%d, %d, %f)\n", 
						set_attrs.viewport.min.x,
						set_attrs.viewport.min.y,
						set_attrs.viewport.min.z,
						set_attrs.viewport.max.x,
						set_attrs.viewport.max.y,
						set_attrs.viewport.max.z
						);
				_hppex_stderr_print( "\n\n");
			}
		}


		/* 
			Clip List 
		*/
		for (i=0; i<NUM_CLIPLIST_VALUES; i++) {
			set_attrs.clip_list.count = cliplist_values[i].count;
			set_attrs.clip_list.rectangles = cliplist_values[i].rectangles;

			if (cliplist_values[i].error_type == NO_ERR) {
				_hppex_test_set_expected_error (NO_ERR, 0, 0, 0, 0);
				check_attrs = set_attrs;
			}
			else
				_hppex_test_set_expected_error (cliplist_values[i].error_type,
									cliplist_values[i].error_code, 0, 0, 0);

			PEXChangeRenderer (display, renderer, PEXRAClipList, &set_attrs);
			XSync (display, 0);
			if (_hppex_test_check_expected_error(ERROR_CODE) ||
			   (check_renderer_attrs (display, renderer,
							&check_attrs,
							ALL_RENDERER_ATTRS_MASK))) {

				_hppex_stderr_print( 
						"\twhile setting cliplist: count = %d\n",
						set_attrs.clip_list.count);
				for (j=0; j< (int)set_attrs.clip_list.count; j++) {
					_hppex_stderr_print( "\t\trectangles[%d]: xmin = %d, xmax = %d, ymin = %d, ymax = %d\n",
										j, set_attrs.clip_list.rectangles[j].xmin,
										set_attrs.clip_list.rectangles[j].xmax,
										set_attrs.clip_list.rectangles[j].ymin,
										set_attrs.clip_list.rectangles[j].ymax);
				}
				_hppex_stderr_print( "\n\n");
			}
		}


		/* 
			PickStartPath 
		*/
		for (i=0; i<NUM_PICKSTARTPATH_VALUES; i++) {
			set_attrs.pick_start_path.count = pickstartpath_values[i].count;
			set_attrs.pick_start_path.elements = pickstartpath_values[i].elements;

			if (pickstartpath_values[i].error_type == NO_ERR) {
				_hppex_test_set_expected_error (NO_ERR, 0, 0, 0, 0);
				check_attrs = set_attrs;
			}
			else
				_hppex_test_set_expected_error (pickstartpath_values[i].error_type,
									pickstartpath_values[i].error_code, 0, 0, 0);

			PEXChangeRenderer (display, renderer, PEXRAPickStartPath, &set_attrs);
			XSync (display, 0);
			if (_hppex_test_check_expected_error(ERROR_CODE) ||
			   (check_renderer_attrs (display, renderer,
							&check_attrs,
							ALL_RENDERER_ATTRS_MASK))) {

				_hppex_stderr_print( 
						"\twhile setting pick start path: count = %d\n",
						set_attrs.pick_start_path.count);
				for (j=0; j< set_attrs.pick_start_path.count; j++) {
					_hppex_stderr_print( "\t\telements[%d]: structure = 0x%x, offset = %d\n",
									j, set_attrs.pick_start_path.elements[j].structure,
									set_attrs.pick_start_path.elements[j].offset);
				}
				_hppex_stderr_print( "\n\n");
			}
		}


		/* 
			Background Color 
		*/
		for (i=0; i<NUM_BACKGROUND_VALUES; i++) {

			if ((set_attrs.background_color.type = background_values[i].type)
				== PEXColorTypeIndexed)
				set_attrs.background_color.value.indexed.index = background_values[i].index;
			else {
				set_attrs.background_color.value.rgb.red = background_values[i].red;
				set_attrs.background_color.value.rgb.green = background_values[i].green;
				set_attrs.background_color.value.rgb.blue = background_values[i].blue;
			}

			if (background_values[i].error_type == NO_ERR) {
				_hppex_test_set_expected_error (NO_ERR, 0, 0, 0, 0);
				check_attrs = set_attrs;
			}
			else
				_hppex_test_set_expected_error (background_values[i].error_type,
									background_values[i].error_code, 0, 0, 0);

			PEXChangeRenderer (display, renderer, PEXRABackgroundColor, &set_attrs);
			XSync (display, 0);
			if (_hppex_test_check_expected_error(ERROR_CODE) ||
			   (check_renderer_attrs (display, renderer,
							&check_attrs,
							ALL_RENDERER_ATTRS_MASK))) {

				_hppex_stderr_print( 
						"\twhile setting background color:\n");
				dump_cspec ("\t\t", &(set_attrs.background_color));
				_hppex_stderr_print( "\n\n");
			}
		}


		/* 
			Clear Image 
		*/
		for (i=0; i<NUM_CLEARFLAG_VALUES; i++) {

			set_attrs.clear_image = clearflag_values[i].value;

			if (clearflag_values[i].error_type == NO_ERR) {
				_hppex_test_set_expected_error (NO_ERR, 0, 0, 0, 0);
				check_attrs = set_attrs;
			}
			else
				_hppex_test_set_expected_error (clearflag_values[i].error_type,
									clearflag_values[i].error_code, 0, 0, 0);

			PEXChangeRenderer (display, renderer, PEXRAClearImage, &set_attrs);
			XSync (display, 0);
			if (_hppex_test_check_expected_error(ERROR_CODE) ||
			   (check_renderer_attrs (display, renderer,
							&check_attrs,
							ALL_RENDERER_ATTRS_MASK))) {

				_hppex_stderr_print( 
						"\twhile setting clear_image flag to %d\n",
						set_attrs.clear_image);
				_hppex_stderr_print( "\n\n");
			}
		}


		/* 
			Clear Z 
		*/
		for (i=0; i<NUM_CLEARFLAG_VALUES; i++) {

			set_attrs.clear_z = clearflag_values[i].value;

			if (clearflag_values[i].error_type == NO_ERR) {
				_hppex_test_set_expected_error (NO_ERR, 0, 0, 0, 0);
				check_attrs = set_attrs;
			}
			else
				_hppex_test_set_expected_error (clearflag_values[i].error_type,
									clearflag_values[i].error_code, 0, 0, 0);

			PEXChangeRenderer (display, renderer, PEXRAClearZ, &set_attrs);
			XSync (display, 0);
			if (_hppex_test_check_expected_error(ERROR_CODE) ||
			   (check_renderer_attrs (display, renderer,
							&check_attrs,
							ALL_RENDERER_ATTRS_MASK))) {

				_hppex_stderr_print( 
						"\twhile setting clear_z flag to %d\n",
						set_attrs.clear_z);
				_hppex_stderr_print( "\n\n");
			}
		}


		/* 
			Echo Mode 
		*/
		for (i=0; i<NUM_ECHOMODE_VALUES; i++) {

			set_attrs.echo_mode = echomode_values[i].value;

			if (echomode_values[i].error_type == NO_ERR) {
				_hppex_test_set_expected_error (NO_ERR, 0, 0, 0, 0);
				check_attrs = set_attrs;
			}
			else
				_hppex_test_set_expected_error (echomode_values[i].error_type,
									echomode_values[i].error_code, 0, 0, 0);

			PEXChangeRenderer (display, renderer, PEXRAEchoMode, &set_attrs);
			XSync (display, 0);
			if (_hppex_test_check_expected_error(ERROR_CODE) ||
			   (check_renderer_attrs (display, renderer,
							&check_attrs,
							ALL_RENDERER_ATTRS_MASK))) {

				_hppex_stderr_print( 
						"\twhile setting echo mode to %d\n",
						set_attrs.echo_mode);
				_hppex_stderr_print( "\n\n");
			}
		}


		/* 
			Renderer State 
		*/
		for (i=0; i<NUM_RNDSTATE_VALUES; i++) {

			set_attrs.renderer_state = rndstate_values[i].value;

			if (rndstate_values[i].error_type == NO_ERR) {
				_hppex_test_set_expected_error (NO_ERR, 0, 0, 0, 0);
				check_attrs = set_attrs;
			}
			else
				/* PEXChangeRenderer should be silent about this attribute. */
				_hppex_test_set_expected_error (NO_ERR, 0, 0, 0, 0);

			PEXChangeRenderer (display, renderer, PEXRARendererState, &set_attrs);
			XSync (display, 0);
			if (_hppex_test_check_expected_error(ERROR_CODE) ||
			   (check_renderer_attrs (display, renderer,
							&check_attrs,
							ALL_RENDERER_ATTRS_MASK))) {

				_hppex_stderr_print( 
						"\twhile setting renderer state to %d\n",
						set_attrs.renderer_state);
				_hppex_stderr_print( "\n\n");
			}
		}


		/* 
			Current Path 
		*/
		for (i=0; i<NUM_CURRENTPATH_VALUES; i++) {
			set_attrs.current_path.count = currentpath_values[i].count;
			set_attrs.current_path.elements = currentpath_values[i].elements;

			if (currentpath_values[i].error_type == NO_ERR) {
				_hppex_test_set_expected_error (NO_ERR, 0, 0, 0, 0);
				check_attrs = set_attrs;
			}
			else
				/* PEXChangeRenderer should be silent about this attribute. */
				_hppex_test_set_expected_error (NO_ERR, 0, 0, 0, 0);

			PEXChangeRenderer (display, renderer, PEXRACurrentPath, &set_attrs);
			XSync (display, 0);
			if (_hppex_test_check_expected_error(ERROR_CODE) ||
			   (check_renderer_attrs (display, renderer,
							&check_attrs,
							ALL_RENDERER_ATTRS_MASK))) {

				_hppex_stderr_print( 
						"\twhile setting current path: count = %d\n",
						set_attrs.current_path.count);
				for (j=0; j< set_attrs.current_path.count; j++) {
					_hppex_stderr_print( "\t\telements[%d]: structure = 0x%x, offset = %d\n",
									j, set_attrs.current_path.elements[j].structure,
									set_attrs.current_path.elements[j].offset);
				}
				_hppex_stderr_print( "\n\n");
			}
		}



		/* 
			Now, try some boundary conditions:

			- send invalid bitmask
			- set no attributes
			- reset all attributes to the base state at once
		*/

		_hppex_test_set_expected_error (X_ERR, BadValue, 0, 0, 0);
		PEXChangeRenderer (display, renderer, BAD_RA_MASK, base_attrs);
		XSync (display, 0);
		if (_hppex_test_check_expected_error(ERROR_CODE) ||
		   check_renderer_attrs (display, renderer,
						&check_attrs,
						ALL_RENDERER_ATTRS_MASK)) {

			_hppex_stderr_print( "\twhile using bad value_mask, still changed attrs\n");
			_hppex_stderr_print( "\n\n");
		}

		_hppex_test_set_expected_error (NO_ERR, 0, 0, 0, 0);
		PEXChangeRenderer (display, renderer, 0, base_attrs);
		XSync (display, 0);
		if (_hppex_test_check_expected_error(ERROR_CODE) ||
		   check_renderer_attrs (display, renderer,
						&check_attrs,
						ALL_RENDERER_ATTRS_MASK)) {

			_hppex_stderr_print( "\twhile using zero value_mask, still changed attrs\n");
			_hppex_stderr_print( "\n\n");
		}

		_hppex_test_set_expected_error (NO_ERR, 0, 0, 0, 0);
		PEXChangeRenderer (display, renderer, NONRES_RENDERER_ATTRS_MASK, base_attrs);
		XSync (display, 0);
		if (_hppex_test_check_expected_error(ERROR_CODE) ||
		   check_renderer_attrs (display, renderer,
						base_attrs,
						ALL_RENDERER_ATTRS_MASK)) {

			_hppex_stderr_print( "\twhile using full value_mask, not all attrs changed\n");
			_hppex_stderr_print( "\n\n");
		}


		/*
			If this is the "bound" pass, unbind at end.
		*/
		if (loop == 1) {
			PEXEndRendering(display, renderer, True);
		}
	}


	/*
		Close Renderer, window, and X/PEX.
	*/
	PEXFreeRenderer(display, renderer);

	XDestroyWindow(display, window);

    _hppex_close_stderr();

#ifdef XSYSTEM
    closeXSystemTesting(&xtc);
#else
    file_testproc("rnd_nresrc.stderr", "rnd_nresrc");
	XCloseDisplay (display);
	exit(0);
#endif

} /* non-resource renderer attribute test */
