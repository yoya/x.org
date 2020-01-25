/* $XConsortium: rnd_dynam.c,v 1.0 93/11/22 12:39:29 rws Exp $ */

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
** Renderer attributes dynamics test (actually source for two tests)
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


#define SMALLNUM 1.0E-6

#ifdef XSYSTEM
#define WIDTH 512
#define HEIGHT 512
#else
#define WIDTH 200
#define HEIGHT 200
#endif


/*
	Renderer attribute dynamics test

	For each Renderer attribute, dynamics are tested
	by the algorithm described below.

	In order to only implement the attribute-setting and
	drawing code once per attribute type, this algorithm is
	actually implemented in a loop that is controlled by
	an array of flags (see loop_control, below).  The
	steps accomplished by each pass through the loop are
	labelled here.

	{
		PASS 0
		set attribute to value 1 (while Renderer is idle)
		bind (to a window)
		draw something with value 1 (image A)

		PASS 1
		set attribute to value 2 (while Renderer is active)
		draw something (image A)
		- if the attribute has immediate dynamics,
		  this will be done with the value 2;
		  if it has deferred dynamics, drawing
		  will be done with value 1
		unbind

		PASS 2
		bind
		draw something (image B)
		- even deferred attributes will use value 2 here
		unbind
	}

	Note that "attributes" includes all the specified Renderer
	attributes, and LUT and Nameset contents as well.

	Note also that some Renderer attributes require two archived images
	to demonstrate, others can use a subframe per setting, and yet others are
	demonstrated by drawing several primitives in one sub-frame of 
	one image.  When sub-frames can be used, the window is typically divided 
	into four sub-frames which are then archived together as an image.

	For XSystem, text bundle and text font table dynamics are done in a 
	separate test with the same organization.  This allows only
	that test to be rearchived if text rendering suffers pixelitis 
	due to floating point libraries or for other reasons.
    There are not two separate tests for susPEX.
*/

/*
	This array controls the behavior of the several loops,
	in order to implement the algorithm above.
*/
static struct _loop_info {
	int set_value;
	int value_index;
	int bind;
	int unbind;
	int capture_image;
} loop_control[3]={
	{ True, 	0, 	True, 	False,	False },
	{ True, 	1, 	False, 	True,	True },
	{ False, 	0, 	True, 	True,	True },
};

/*
	These arrays describe the viewports for full-window and subframe images.
*/
static PEXViewport fullframe_data[]={
	{ {0, 0, 0.0},				{WIDTH, HEIGHT, 1.0},		True,	{ 0,0,0 }	},
};

static PEXViewport subframe_data[]={
	{ {0, 0, 0.0},				{WIDTH/2, HEIGHT/2, 1.0},	False,	{ 0,0,0 }	},
	{ {0, HEIGHT/2, 0.0},		{WIDTH/2, HEIGHT, 1.0},		False,	{ 0,0,0 }	},
	{ {WIDTH/2, 0, 0.0},		{WIDTH, HEIGHT/2, 1.0},		False,	{ 0,0,0 }	},
	{ {WIDTH/2, HEIGHT/2, 0.0},	{WIDTH, HEIGHT, 1.0},		False,	{ 0,0,0 }	},
};
#define NUM_SUBFRAMES_PER_IMAGE	4

/*
	These constants and strings are for capturing and printing out
	the Renderer dynamics.
*/
#define TABLES 0
#define NAMES 1
#define ATTRS 2
#define NO_ENTRY 3

#define RDABackgroundColor			0x01
#define RDAClearImage				0x02
#define RDAClearZ					0x04
#define RDAPipelineContext			0x08
#define RDAPipelineContextContents	0x10

static char *dyn_names[]={
	"immediate",
	"deferred"
};



/*
	These constants determine the order in which the
	attributes are tested.  The ordering is important
	because tests of later attributes assume that earlier
	ones work correctly.

	HLHSR mode is first because once a Z buffer has been 
	allocated, it remains associated with the Renderer.
	Clearing functionality is tested next because it is also
	heavily used.  Viewport setting is also relied on for
	subframe operation.

	The attr_control array describes variations in the
	testing algorithm to test each particular attribute.
	The first few attributes require an image per setting.
	Attributes starting with NPC subvolume are tested
	using subframes.  Attributes starting with ?? can be
	tested in single subframes, allowing several attributes
	to be tested per archived image.

	FIRST_ATTR and LAST_ATTR determine the attributes that
	are tested.
*/

#define HLHSR_MODE					0
#define CLEAR_IMAGE					1
#define CLEAR_Z						2
#define BACKGROUND_COLOR			3
#define VIEWPORT					4
#define CLIPLIST					5

#define NPC_SUBVOLUME				6
#define VIEW_LUT					7
#define VIEW_LUT_CONTENTS			8

#define ECHO_MODE					9
#define COLOR_LUT					10
#define COLOR_LUT_CONTENTS			11
#define DEPTH_CUE_LUT				12
#define DEPTH_CUE_LUT_CONTENTS		13
#define COLOR_APPROX_LUT			14
#define COLOR_APPROX_LUT_CONTENTS	15
#define LIGHT_LUT					16
#define LIGHT_LUT_CONTENTS			17

#define LINE_LUT					18
#define LINE_LUT_CONTENTS			19
#define MARKER_LUT					20
#define MARKER_LUT_CONTENTS			21
#define INTERIOR_LUT				22
#define INTERIOR_LUT_CONTENTS		23
#define EDGE_LUT					24
#define EDGE_LUT_CONTENTS			25

#define PIPELINE_CONTEXT			26
#define PIPELINE_CONTEXT_ATTR		27
#define PIPELINE_CONTEXT_ASF		28

#define PLC_NAMESET					29
#define PLC_NAMESET_CONTENTS		30

#define HIGHLIGHT_INCL				31
#define HIGHLIGHT_INCL_CONTENTS		32
#define HIGHLIGHT_EXCL				33
#define HIGHLIGHT_EXCL_CONTENTS		34
#define INVISIBILITY_INCL			35
#define INVISIBILITY_INCL_CONTENTS	36
#define INVISIBILITY_EXCL			37
#define INVISIBILITY_EXCL_CONTENTS	38

#define TEXT_LUT					39
#define TEXT_LUT_CONTENTS			40
#define TEXT_FONT_LUT				41
#define TEXT_FONT_LUT_CONTENTS		42

/* these are not currently tested */
#define PICK_INCL					43
#define PICK_INCL_CONTENTS			44
#define PICK_EXCL					45
#define PICK_EXCL_CONTENTS			46


/*
	This test source can be conditionally compiled into
	two separate tests.  The text cases are separated out
	for XSystem because they are more likely to need rearchiving.
*/
#ifdef XSYSTEM
#ifdef TEXT_TEST
#define FIRST_ATTR					TEXT_LUT
#define LAST_ATTR 					TEXT_FONT_LUT_CONTENTS
#else
#define FIRST_ATTR					HLHSR_MODE
#define LAST_ATTR 					INVISIBILITY_EXCL_CONTENTS
#endif
#else
#define FIRST_ATTR					HLHSR_MODE
#define LAST_ATTR 					TEXT_FONT_LUT_CONTENTS
#endif



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
	int dyn_index;
	unsigned int dyn_bit;
	char *imm_message_0;
	char *imm_message_1;
	char *imm_message_2;
	char *def_message_0;
	char *def_message_1;
	char *def_message_2;
} attr_control[]={
	/* HLHSR_MODE */				{ True, False, False, True,
									NOT_A_RESOURCE, NULL, PEXRAHLHSRMode, 0,
									"HLHSR Mode", ATTRS, PEXRDAHLHSRMode,
									"First image: Red rect at z=1.0 should obscure green rect at z=0.0 that was drawn later",
									"Second image: identical to first",
									"",
									"First image: Red rect at z=1.0 should lie under green rect at z=0.0 that was drawn later",
									"Second image: Red rect at z=1.0 should obscure green rect at z=0.0 that was drawn later",
									"" },

	/* CLEAR_IMAGE */				{ True,	False, False, True,
									NOT_A_RESOURCE, NULL, PEXRAClearImage, 0,
									"Clear Image Flag", NO_ENTRY, RDAClearImage,
									"", "", "",
									"First image: Red rectangle", 
									"Second image: Cleared to 50 percent gray", 
									"" },

	/* CLEAR_Z */					{ True, False, False, True,
									NOT_A_RESOURCE, NULL, PEXRAClearZ, 0,
									"Clear Z Flag", NO_ENTRY, RDAClearZ,
									"", "", "",
									"First image: Gray rect at z=0.5",
									"Second image: Red rect cuts through z=0.5",
									"If Z-buffer was cleared, should see all of red rect" },
	/* BACKGROUND_COLOR */			{ True, False, False, True,
									NOT_A_RESOURCE, NULL, PEXRABackgroundColor, 0,
									"Background Color", NO_ENTRY, RDABackgroundColor,
									"", "", "",
									"First image: Cleared to red",
									"Second image: Cleared to green",
									"" },

	/* VIEWPORT */					{ True, False, False, False,
									NOT_A_RESOURCE, NULL, PEXRAViewport, 0,
									"Viewport", ATTRS, PEXRDAViewport,
									"First image: outline rect of full-window viewport",
									"and of modified viewport (upper right quarter)",
									"Second image: only the modified viewport outlined",
									"First image: outline rect of full-window viewport only",
									"Second image: modified viewport outlined (upper right quarter)",
									"" },
	/* CLIPLIST */					{ True, False, False, False,
									NOT_A_RESOURCE, NULL, PEXRAClipList, 0,
									"Clip List", ATTRS, PEXRDAClipList,
									"First image: whole outline rect (actually drawn in",
									"two passes with two clip lists)",
									"Second image: only second clip list, lower left and upper right",
									"First image: only first clip list, upper left and lower right",
									"Second image: only second clip list, lower left and upper right",
									"" },

	/* NPC_SUBVOLUME */				{ False, True, False, False,
									NOT_A_RESOURCE, NULL, PEXRANPCSubVolume, 0,
									"NPC Subvolume", ATTRS, PEXRDANPCSubVolume,
									"First frame: outline of first subvol (whole viewport)",
									"Second frame: outline of second subvol (lower half)",
									"Third frame: same as second frame",
									"First frame: outline of first subvol (whole viewport)",
									"Second frame: same as first frame",
									"Third frame: outline of second subvol (lower half)" },
	/* VIEW_LUT */					{ False, True, False, False,
									RESOURCE_ID, NULL, PEXRAViewTable, PEXLUTView,
									"View LUT ID", TABLES, PEXRDTViewTable,
									"First frame: outline rectangle with horizontal orientation",
									"Second frame: view orientation has change, long axis is vertical",
									"Third frame: same as second frame",
									"First frame: outline rectangle with horizontal orientation",
									"Second frame: same as first frame",
									"Third frame: view orientation has change, long axis is vertical" },
	/* VIEW_LUT_CONTENTS */			{ False, True, False, False,
									RESOURCE_CONTENTS, NULL, PEXRAViewTable, PEXLUTView,
									"View LUT Contents", TABLES, PEXRDTViewTableContents,
									"First frame: outline rectangle with horizontal orientation",
									"Second frame: view orientation has change, long axis is vertical",
									"Third frame: same as second frame",
									"First frame: outline rectangle with horizontal orientation",
									"Second frame: same as first frame",
									"Third frame: view orientation has change, long axis is vertical" },

	/* ECHO_MODE */					{ False, False, True, False,
									NOT_A_RESOURCE, NULL, PEXRAEchoMode, 0,
									"Echo Mode", ATTRS, PEXRDAEchoMode,
									"Bottom prim: drawn in line color (red)",
									"Middle prim: drawn in echo color",
									"Top prim: drawn in echo color",
									"Bottom prim: drawn in line color (red)",
									"Middle prim: drawn in line color (red)",
									"Top prim: drawn in echo color" },

	/* COLOR_LUT */					{ False, False, False, False,
									RESOURCE_ID, NULL, PEXRAColorTable, PEXLUTColor,
									"Color LUT ID", TABLES, PEXRDTColorTable,
									"Bottom prim: drawn in indexed color (magenta)",
									"Middle prim: drawn in same index, changed entry (green)",
									"Top prim: drawn in same index, changed entry (green)",
									"Bottom prim: drawn in indexed color (magenta)",
									"Middle prim: drawn in same index, unchanged entry (magenta)",
									"Top prim: drawn in same index, changed entry (green)" },
	/* COLOR_LUT_CONTENTS */		{ False, False, False, False,
									RESOURCE_CONTENTS, NULL, PEXRAColorTable, PEXLUTColor,
									"Color LUT Contents", TABLES, PEXRDTColorTableContents,
									"Bottom prim: drawn in indexed color (magenta)",
									"Middle prim: drawn in same index, changed entry (green)",
									"Top prim: drawn in same index, changed entry (green)",
									"Bottom prim: drawn in indexed color (magenta)",
									"Middle prim: drawn in same index, unchanged entry (magenta)",
									"Top prim: drawn in same index, changed entry (green)" },
	/* DEPTH_CUE_LUT */				{ False, False, False, False,
									RESOURCE_ID, NULL, PEXRADepthCueTable, PEXLUTDepthCue,
									"Depth Cue LUT ID", TABLES, PEXRDTDepthCueTable,
									"Bottom prim: drawn in first depth cue color (yellow)",
									"Middle prim: drawn in second depth cue color (green)",
									"Top prim: drawn in second depth cue color (green)",
									"Bottom prim: drawn in first depth cue color (yellow)",
									"Middle prim: drawn in first depth cue color (yellow)",
									"Top prim: drawn in second depth cue color (green)" },
	/* DEPTH_CUE_LUT_CONTENTS */	{ False, False, True, False,
									RESOURCE_CONTENTS, NULL, PEXRADepthCueTable, PEXLUTDepthCue,
									"Depth Cue LUT Contents", TABLES, PEXRDTDepthCueTableContents,
									"Bottom prim: drawn in first depth cue color (yellow)",
									"Middle prim: drawn in second depth cue color (green)",
									"Top prim: drawn in second depth cue color (green)",
									"Bottom prim: drawn in first depth cue color (yellow)",
									"Middle prim: drawn in first depth cue color (yellow)",
									"Top prim: drawn in second depth cue color (green)" },

	/* COLOR_APPROX_LUT */			{ False, False, False, False,
									RESOURCE_ID, NULL, PEXRAColorApproxTable, PEXLUTColorApprox,
									"Color Approximation LUT ID", TABLES, PEXRDTColorApproxTable,
									"Bottom prim: drawn in ColorSpace (red)",
									"Middle prim: drawn in ColorRange",
									"Top prim: drawn in ColorRange",
									"Bottom prim: drawn in ColorSpace (red)",
									"Middle prim: drawn in ColorSpace (red)",
									"Top prim: drawn in ColorRange" },
	/* COLOR_APPROX_LUT_CONTENTS */	{ False, False, False, False,
									RESOURCE_CONTENTS, NULL, PEXRAColorApproxTable, PEXLUTColorApprox,
									"Color Approximation LUT Contents", TABLES, PEXRDTColorApproxContents,
									"Bottom prim: drawn in ColorSpace (red)",
									"Middle prim: drawn in ColorRange",
									"Top prim: drawn in ColorRange",
									"Bottom prim: drawn in ColorSpace (red)",
									"Middle prim: drawn in ColorSpace (red)",
									"Top prim: drawn in ColorRange" },
	/* LIGHT_LUT */					{ False, False, False, False,
									RESOURCE_ID, NULL, PEXRALightTable, PEXLUTLight,
									"Light LUT ID", TABLES, PEXRDTLightTable,
									"Bottom prim: neutral rect lit by first ambient color (red)",
									"Middle prim: neutral rect lit by second ambient color (green)",
									"Top prim: neutral rect lit by second ambient color (green)",
									"Bottom prim: neutral rect lit by first ambient color (red)",
									"Middle prim: neutral rect lit by first ambient color (red)",
									"Top prim: neutral rect lit by second ambient color (green)" },
	/* LIGHT_LUT_CONTENTS */		{ False, False, True, False,
									RESOURCE_CONTENTS, NULL, PEXRALightTable, PEXLUTLight,
									"Light LUT Contents", TABLES, PEXRDTLightTableContents,
									"Bottom prim: neutral rect lit by first ambient color (red)",
									"Middle prim: neutral rect lit by second ambient color (green)",
									"Top prim: neutral rect lit by second ambient color (green)",
									"Bottom prim: neutral rect lit by first ambient color (red)",
									"Middle prim: neutral rect lit by first ambient color (red)",
									"Top prim: neutral rect lit by second ambient color (green)" },

	/* LINE_LUT */					{ False, False, False, False,
									RESOURCE_ID, NULL, PEXRALineBundle, PEXLUTLineBundle,
									"Line Bundle LUT ID", TABLES, PEXRDTLineBundle,
									"Bottom prim: drawn in first line type (solid)",
									"Middle prim: drawn in second line type (dashed)",
									"Top prim: drawn in second line type (dashed)",
									"Bottom prim: drawn in first line type (solid)",
									"Middle prim: drawn in first line type (solid)",
									"Top prim: drawn in second line type (dashed)" },
	/* LINE_LUT_CONTENTS */			{ False, False, False, False,
									RESOURCE_CONTENTS, NULL, PEXRALineBundle, PEXLUTLineBundle,
									"Line Bundle LUT Contents", TABLES, PEXRDTLineBundleContents,
									"Bottom prim: drawn in first line type (solid)",
									"Middle prim: drawn in second line type (dashed)",
									"Top prim: drawn in second line type (dashed)",
									"Bottom prim: drawn in first line type (solid)",
									"Middle prim: drawn in first line type (solid)",
									"Top prim: drawn in second line type (dashed)" },
	/* MARKER_LUT */				{ False, False, False, False,
									RESOURCE_ID, NULL, PEXRAMarkerBundle, PEXLUTMarkerBundle,
									"Marker Bundle LUT ID", TABLES, PEXRDTMarkerBundle,
									"Bottom prim: drawn in first marker type (cross)",
									"Middle prim: drawn in second marker type (circle)",
									"Top prim: drawn in second marker type (circle)",
									"Bottom prim: drawn in first marker type (cross)",
									"Middle prim: drawn in first marker type (cross)",
									"Top prim: drawn in second marker type (circle)" },
	/* MARKER_LUT_CONTENTS */		{ False, False, True, False,
									RESOURCE_CONTENTS, NULL, PEXRAMarkerBundle, PEXLUTMarkerBundle,
									"Marker Bundle LUT Contents", TABLES, PEXRDTMarkerBundleContents,
									"Bottom prim: drawn in first marker type (cross)",
									"Middle prim: drawn in second marker type (circle)",
									"Top prim: drawn in second marker type (circle)",
									"Bottom prim: drawn in first marker type (cross)",
									"Middle prim: drawn in first marker type (cross)",
									"Top prim: drawn in second marker type (circle)" },

	/* INTERIOR_LUT */				{ False, False, False, False,
									RESOURCE_ID, NULL, PEXRAInteriorBundle, PEXLUTInteriorBundle,
									"Interior Bundle LUT ID", TABLES, PEXRDTInteriorBundle,
									"Bottom prim: drawn in first interior style (hollow)",
									"Middle prim: drawn in second interior style (solid)",
									"Top prim: drawn in second interior style (solid)",
									"Bottom prim: drawn in first interior style (hollow)",
									"Middle prim: drawn in first interior style (hollow)",
									"Top prim: drawn in second interior style (solid)" },
	/* INTERIOR_LUT_CONTENTS */		{ False, False, False, False,
									RESOURCE_CONTENTS, NULL, PEXRAInteriorBundle, PEXLUTInteriorBundle,
									"Interior Bundle LUT Contents", TABLES, PEXRDTInteriorBundleContents,
									"Bottom prim: drawn in first interior style (hollow)",
									"Middle prim: drawn in second interior style (solid)",
									"Top prim: drawn in second interior style (solid)",
									"Bottom prim: drawn in first interior style (hollow)",
									"Middle prim: drawn in first interior style (hollow)",
									"Top prim: drawn in second interior style (solid)" },
	/* EDGE_LUT */					{ False, False, False, False,
									RESOURCE_ID, NULL, PEXRAEdgeBundle, PEXLUTEdgeBundle,
									"Edge Bundle LUT ID", TABLES, PEXRDTEdgeBundle,
									"Bottom prim: drawn in first edge color (yellow)",
									"Middle prim: drawn in second edge color (green)",
									"Top prim: drawn in second edge color (green)",
									"Bottom prim: drawn in first edge color (yellow)",
									"Middle prim: drawn in first edge color (yellow)",
									"Top prim: drawn in second edge color (green)" },
	/* EDGE_LUT_CONTENTS */			{ False, False, True, False,
									RESOURCE_CONTENTS, NULL, PEXRAEdgeBundle, PEXLUTEdgeBundle,
									"Edge Bundle LUT Contents", TABLES, PEXRDTEdgeBundleContents,
									"Bottom prim: drawn in first edge color (yellow)",
									"Middle prim: drawn in second edge color (green)",
									"Top prim: drawn in second edge color (green)",
									"Bottom prim: drawn in first edge color (yellow)",
									"Middle prim: drawn in first edge color (yellow)",
									"Top prim: drawn in second edge color (green)" },

	/* PIPELINE_CONTEXT */			{ False, False, False, False,
									RESOURCE_ID, NULL, PEXRAPipelineContext, 0,
									"Pipeline Context ID", NO_ENTRY, RDAPipelineContext,
									"", "", "",
									"Bottom prim: drawn in first line type (solid)",
									"Middle prim: drawn in first line type (solid)",
									"Top prim: drawn in second line type (dashed)" },
	/* PIPELINE_CONTEXT_ATTR */		{ False, False, False, False,
									RESOURCE_CONTENTS, NULL, PEXRAPipelineContext, 0,
									"Pipeline Context Contents (Attribute)", NO_ENTRY, RDAPipelineContextContents,
									"", "", "",
									"Bottom prim: drawn in first line type (solid)",
									"Middle prim: drawn in first line type (solid)",
									"Top prim: drawn in second line type (dashed)" },
	/* PIPELINE_CONTEXT_ASF */		{ False, False, True, False,
									RESOURCE_CONTENTS, NULL, PEXRAPipelineContext, 0,
									"Pipeline Context Contents (ASF)", NO_ENTRY, RDAPipelineContextContents,
									"", "", "",
									"Bottom prim: drawn in first line type (solid)",
									"Middle prim: drawn in first line type (solid)",
									"Top prim: drawn in second line type (dashed)" },

	/* PLC_NAMESET */				{ False, False, False, False,
									RESOURCE_ID, NULL, PEXRAPipelineContext, 0,
									"Pipeline Context Nameset ID", NO_ENTRY, RDAPipelineContextContents,
									"", "", "",
									"Bottom prim: not drawn because passes invis filter",
									"Middle prim: not drawn because passes invis filter",
									"Top prim: drawn because invis exclusion rejects" },
	/* PLC_NAMESET_CONTENTS */		{ False, False, True, False,
									RESOURCE_CONTENTS, NULL, PEXRAPipelineContext, 0,
									"Pipeline Context Nameset Contents", NO_ENTRY, RDAPipelineContextContents,
									"", "", "",
									"Bottom prim: not drawn because passes invis filter",
									"Middle prim: not drawn because passes invis filter",
									"Top prim: drawn because invis exclusion rejects" },

	/* HIGHLIGHT_INCL */			{ False, False, False, False,
									RESOURCE_ID, NULL, PEXRAHighlightIncl, 0,
									"Highlight Inclusion Nameset ID", NAMES, PEXRDNHighlightNameSet,
									"Bottom prim: not highlighted",
									"Middle prim: highlighted",
									"Top prim: highlighted",
									"Bottom prim: not highlighted",
									"Middle prim: not highlighted",
									"Top prim: highlighted" },
	/* HIGHLIGHT_INCL_CONTENTS */	{ False, False, False, False,
									RESOURCE_CONTENTS, NULL, PEXRAHighlightIncl, 0,
									"Highlight Inclusion Nameset Contents", NAMES, PEXRDNHighlightNameSetContents,
									"Bottom prim: not highlighted",
									"Middle prim: highlighted",
									"Top prim: highlighted",
									"Bottom prim: not highlighted",
									"Middle prim: not highlighted",
									"Top prim: highlighted" },
	/* HIGHLIGHT_EXCL */			{ False, False, False, False,
									RESOURCE_ID, NULL, PEXRAHighlightExcl, 0,
									"Highlight Exclusion Nameset ID", NAMES, PEXRDNHighlightNameSet,
									"Bottom prim: highlighted",
									"Middle prim: not highlighted",
									"Top prim: not highlighted",
									"Bottom prim: highlighted",
									"Middle prim: highlighted",
									"Top prim: not highlighted" },
	/* HIGHLIGHT_EXCL_CONTENTS */	{ False, False, True, False,
									RESOURCE_CONTENTS, NULL, PEXRAHighlightExcl, 0,
									"Highlight Exclusion Nameset Contents", NAMES, PEXRDNHighlightNameSetContents,
									"Bottom prim: highlighted",
									"Middle prim: not highlighted",
									"Top prim: not highlighted",
									"Bottom prim: highlighted",
									"Middle prim: highlighted",
									"Top prim: not highlighted" },

	/* INVISIBILITY_INCL */			{ False, False, False, False,
									RESOURCE_ID, NULL, PEXRAInvisibilityIncl, 0,
									"Invisibility Inclusion Nameset ID", NAMES, PEXRDNInvisibilityNameSet,
									"Bottom prim: not invisible",
									"Middle prim: invisible",
									"Top prim: invisible",
									"Bottom prim: not invisible",
									"Middle prim: not invisible",
									"Top prim: invisible" },
	/* INVISIBILITY_INCL_CONTENTS */{ False, False, False, False,
									RESOURCE_CONTENTS, NULL, PEXRAInvisibilityIncl, 0,
									"Invisibility Inclusion Nameset Contents", NAMES, PEXRDNInvisibilityNameSetContents,
									"Bottom prim: not invisible",
									"Middle prim: invisible",
									"Top prim: invisible",
									"Bottom prim: not invisible",
									"Middle prim: not invisible",
									"Top prim: invisible" },
	/* INVISIBILITY_EXCL */			{ False, False, False, False,
									RESOURCE_ID, NULL, PEXRAInvisibilityExcl, 0,
									"Invisibility Exclusion Nameset ID", NAMES, PEXRDNInvisibilityNameSet,
									"Bottom prim: invisible",
									"Middle prim: not invisible",
									"Top prim: not invisible",
									"Bottom prim: invisible",
									"Middle prim: invisible",
									"Top prim: not invisible" },
	/* INVISIBILITY_EXCL_CONTENTS */{ False, False, True, False,
									RESOURCE_CONTENTS, NULL, PEXRAInvisibilityExcl, 0,
									"Invisibility Exclusion Nameset Contents", NAMES, PEXRDNInvisibilityNameSetContents,
									"Bottom prim: invisible",
									"Middle prim: not invisible",
									"Top prim: not invisible",
									"Bottom prim: invisible",
									"Middle prim: invisible",
									"Top prim: not invisible" },

	/* TEXT_LUT */					{ False, False, False, False,
									RESOURCE_ID, NULL, PEXRATextBundle, PEXLUTTextBundle,
									"Text Bundle LUT ID", TABLES, PEXRDTTextBundle,
									"Bottom prim: first char spacing (0.0)",
									"Middle prim: second char spacing (1.0)",
									"Top prim: second char spacing (1.0)",
									"Bottom prim: first char spacing (0.0)",
									"Middle prim: first char spacing (0.0)",
									"Top prim: second char spacing (1.0)" },
	/* TEXT_LUT_CONTENTS */			{ False, False, False, False,
									RESOURCE_CONTENTS, NULL, PEXRATextBundle, PEXLUTTextBundle,
									"Text Bundle LUT Contents", TABLES, PEXRDTTextBundleContents,
									"Bottom prim: first char spacing (0.0)",
									"Middle prim: second char spacing (1.0)",
									"Top prim: second char spacing (1.0)",
									"Bottom prim: first char spacing (0.0)",
									"Middle prim: first char spacing (0.0)",
									"Top prim: second char spacing (1.0)" },
	/* TEXT_FONT_LUT */				{ False, False, False, False,
									RESOURCE_ID, NULL, PEXRATextFontTable, PEXLUTTextFont,
									"Text Font LUT ID", TABLES, PEXRDTTextFontTable,
									"Bottom prim: first font",
									"Middle prim: second font",
									"Top prim: second font",
									"Bottom prim: first font",
									"Middle prim: first font",
									"Top prim: second font" },
	/* TEXT_FONT_LUT_CONTENTS */	{ False, False, True, False,
									RESOURCE_CONTENTS, NULL, PEXRATextFontTable, PEXLUTTextFont,
									"Text Font LUT Contents", TABLES, PEXRDTTextFontTableContents,
									"Bottom prim: first font",
									"Middle prim: second font",
									"Top prim: second font",
									"Bottom prim: first font",
									"Middle prim: first font",
									"Top prim: second font" },

	/* PICK_INCL */					{ False, False, False, False,
									RESOURCE_ID, NULL, PEXRAPickIncl, 0,
									"Pick Inclusion Nameset ID", NAMES, PEXRDNPickNameSet,
									"Bottom prim: not pickable",
									"Middle prim: pickable",
									"Top prim: pickable" },
	/* PICK_INCL_CONTENTS */		{ False, False, False, False,
									RESOURCE_CONTENTS, NULL, PEXRAPickIncl, 0,
									"Pick Inclusion Nameset Contents", NAMES, PEXRDNPickNameSetContents,
									"Bottom prim: not pickable",
									"Middle prim: pickable",
									"Top prim: pickable" },
	/* PICK_EXCL */					{ False, False, False, False,
									RESOURCE_ID, NULL, PEXRAPickExcl, 0,
									"Pick Exclusion Nameset ID", NAMES, PEXRDNPickNameSet,
									"Bottom prim: pickable",
									"Middle prim: not pickable",
									"Top prim: not pickable" },
	/* PICK_EXCL_CONTENTS */		{ False, False, True, False,
									RESOURCE_CONTENTS, NULL, PEXRAPickExcl, 0,
									"Pick Exclusion Nameset Contents", NAMES, PEXRDNPickNameSetContents,
									"Bottom prim: pickable",
									"Middle prim: not pickable",
									"Top prim: not pickable" },
};


static struct _hlhsr_value {
	PEXEnumTypeIndex value;
	unsigned long ID;
} hlhsrmode_values[]={
	{ PEXHLHSROff,		0	},
	{ PEXHLHSRZBuffer,	0	},
};

/* This struct is used for several attributes. */
typedef struct {
	int clear_image;
	int clear_z;
	float red, green, blue;
	int hlhsr_mode;
} clear_value;

static clear_value clearimage_values[]={
	{ False,	False,	0.5, 0.5, 0.5,	PEXHLHSROff },
	{ True,		False,	0.5, 0.5, 0.5,	PEXHLHSROff },
};

static clear_value clearz_values[]={
	{ False,	False,	0.5, 0.5, 0.5,	PEXHLHSRZBuffer },
	{ False,	True,	0.5, 0.5, 0.5,	PEXHLHSRZBuffer },
};

static clear_value background_values[]={
	{ True,		False,	1.0, 0.0, 0.0,	PEXHLHSROff },
	{ True,		False,	0.0, 1.0, 0.0,	PEXHLHSROff },
};

/*
typedef struct {
    PEXDeviceCoord      min;
    PEXDeviceCoord      max;
    PEXSwitch           use_drawable;
    unsigned char       reserved[3];
} PEXViewport;
*/
static struct _viewport_value {
	PEXViewport value;
} viewport_values[]={
	{ { {0, 0, 0.0},				{WIDTH, HEIGHT, 1.0},	False,	{ 0,0,0 }	} },
	{ { {WIDTH/2, HEIGHT/2, 0.0},	{WIDTH, HEIGHT, 1.0},	False,	{ 0,0,0 }	} },
};

/*
typedef struct {
    PEXCoord            min;
    PEXCoord            max;
} PEXNPCSubVolume;
*/
static struct _npc_value {
	PEXNPCSubVolume value;
} npc_values[]={
	{ { {0.0, 0.0, 0.0}, 				{1.0, 1.0, 1.0} }		},
	{ { {0.0, 0.0, 0.0}, 				{1.0, 0.5, 1.0} }		},
};

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
	unsigned short count;
	PEXDeviceRect rectangles[2];
} cliplist_values[]={
	{ 2,	{ {0,0,WIDTH/2,HEIGHT/2},	{WIDTH/2,HEIGHT/2,WIDTH,HEIGHT}	} },
	{ 2,	{ {0,HEIGHT/2,WIDTH/2,HEIGHT},	{WIDTH/2,0,WIDTH,HEIGHT/2}	} },
};

static struct _view_value {
	PEXCoord		vrp;
	PEXVector		vpn;
	PEXVector  		vup;
	PEXCoord2D		frame[2];
	PEXNPCSubVolume	viewport;
	int				perspective;
	PEXCoord		prp;
	double			view_plane;
	double			front_plane;
	double			back_plane;
} view_values[]={
	{ { 0.5, 0.5, 0.0 }, { 0.0, 0.0, 1.0 },	{ 0.0, 1.0, 0.0 },
	  { { -0.5, -0.5 }, { 0.5, 0.5 } },
	  { {0.0, 0.0, 0.0},	{1.0, 1.0, 1.0} },
	  False, { 0.0, 0.0, 2.0 }, 1.0, 1.0, 0.0 },
	{ { 0.5, 0.5, 0.0 }, { 0.0, 0.0, 1.0 },	{ 1.0, 0.0, 0.0 },
	  { { -0.5, -0.5 }, { 0.5, 0.5 } },
	  { {0.0, 0.0, 0.0},	{1.0, 1.0, 1.0} },
	  False, { 0.0, 0.0, 2.0 }, 1.0, 1.0, 0.0 },
};

static struct _echomode_value {
	PEXEnumTypeIndex value;
} echomode_values[]={
	{ PEXNoEcho		},
	{ PEXEcho		},
};

static struct _color_value {
	float red, green, blue;
} color_values[]={
	{ 1.0, 0.0, 1.0 },
	{ 0.0, 1.0, 0.0 },
};

/*
typedef struct {
    PEXEnumTypeIndex    method;
    unsigned short      reserved;
    float               tolerance;
} PEXCurveApprox;

typedef struct {
    PEXEnumTypeIndex    type;
    PEXEnumTypeIndex    interp_method;
    PEXCurveApprox      curve_approx;
    float               width;
    PEXColorSpecifier   color;
} PEXLineBundleEntry;
*/
static struct _line_value {
	PEXEnumTypeIndex	type;
	PEXEnumTypeIndex	interp_method;
	float 				red, green, blue;
} line_values[]={
	{ PEXLineTypeSolid, PEXPolylineInterpNone, 1.0, 0.0, 1.0 },
	{ PEXLineTypeDashed, PEXPolylineInterpNone, 0.0, 1.0, 0.0 },
};
static unsigned long line_asfs[]={
	PEXASFLineType,
};
#define NUM_LINE_ASFS (sizeof(line_asfs)/sizeof(unsigned long))

/*
typedef struct {
    PEXEnumTypeIndex    type;
    short               reserved;
    float               scale;
    PEXColorSpecifier   color;
} PEXMarkerBundleEntry;
*/
static struct _marker_value {
	PEXEnumTypeIndex	type;
	float 				red, green, blue;
} marker_values[]={
	{ PEXMarkerCross, 1.0, 0.0, 1.0 },
	{ PEXMarkerCircle, 0.0, 1.0, 0.0 },
};
static unsigned long marker_asfs[]={
	PEXASFMarkerType,
};
#define NUM_MARKER_ASFS (sizeof(marker_asfs)/sizeof(unsigned long))

/*
typedef struct {
    PEXEnumTypeIndex    style;
    PEXTypeOrTableIndex style_index;
    PEXEnumTypeIndex    reflection_model;
    PEXEnumTypeIndex    interp_method;
    PEXEnumTypeIndex    bf_style;
    PEXTypeOrTableIndex bf_style_index;
    PEXEnumTypeIndex    bf_reflection_model;
    PEXEnumTypeIndex    bf_interp_method;
    PEXSurfaceApprox    surface_approx;
    PEXColorSpecifier   color;
    PEXReflectionAttributes   reflection_attr;
    PEXColorSpecifier   bf_color;
    PEXReflectionAttributes   bf_reflection_attr;
} PEXInteriorBundleEntry;
*/
static struct _interior_value {
	PEXEnumTypeIndex	style;
	float 				red, green, blue;
} interior_values[]={
	{ PEXInteriorStyleHollow, 1.0, 0.0, 1.0 },
	{ PEXInteriorStyleSolid, 0.0, 1.0, 0.0 },
};
static unsigned long interior_asfs[]={
	PEXASFInteriorStyle,
};
#define NUM_INTERIOR_ASFS (sizeof(interior_asfs)/sizeof(unsigned long))

/*
typedef struct {
    PEXSwitch           edge_flag;
    unsigned char       reserved;
    PEXEnumTypeIndex    type;
    float               width;
    PEXColorSpecifier   color;
} PEXEdgeBundleEntry;
*/
static struct _edge_value {
    PEXSwitch           edge_flag;
	PEXEnumTypeIndex	type;
	float 				red, green, blue;
} edge_values[]={
	{ PEXOn, PEXSurfaceEdgeSolid, 1.0, 1.0, 0.0 },
	{ PEXOn, PEXSurfaceEdgeDashed, 0.0, 1.0, 0.0 },
};
static unsigned long edge_asfs[]={
	PEXASFSurfaceEdges,
	PEXASFSurfaceEdgeColor,
};
#define NUM_EDGE_ASFS (sizeof(edge_asfs)/sizeof(unsigned long))

/*
typedef struct {
    PEXSwitch           mode;
    unsigned char       reserved[3];
    float               front_plane;
    float               back_plane;
    float               front_scaling;
    float               back_scaling;
    PEXColorSpecifier   color;
} PEXDepthCueEntry;
*/
static struct _depthcue_value {
    PEXSwitch           mode;
    float               front_plane;
    float               back_plane;
    float               front_scaling;
    float               back_scaling;
	float 				red, green, blue;
} depthcue_values[]={
	{ PEXOn, 1.0, 0.0, 	1.0, 0.0,	1.0, 1.0, 0.0 },
	{ PEXOn, 1.0, 0.0, 	1.0, 0.0,	0.0, 1.0, 0.0 },
};

/*
typedef struct {
    PEXEnumTypeIndex    type;
    unsigned short      reserved;
    PEXVector           direction;
    PEXCoord            point;
    float               concentration;
    float               spread_angle;
    float               attenuation1;
    float               attenuation2;
    PEXColorSpecifier   color;
} PEXLightEntry;
*/
static struct _light_value {
    PEXEnumTypeIndex    type;
	float 				red, green, blue;
} light_values[]={
	{ PEXLightAmbient, 1.0, 0.0, 0.0 },
	{ PEXLightAmbient, 0.0, 1.0, 0.0 },
};

/*
typedef struct {
    PEXEnumTypeIndex    type;
    PEXEnumTypeIndex    model;
    unsigned short      max1;
    unsigned short      max2;
    unsigned short      max3;
    PEXSwitch           dither;
    unsigned char       reserved;
    unsigned long       mult1;
    unsigned long       mult2;
    unsigned long       mult3;
    float               weight1;
    float               weight2;
    float               weight3;
    unsigned long       base_pixel;
} PEXColorApproxEntry;

	This data is not current used.  Instead, the first color approx
	setting is just a copy of what was used in creating the window,
	which is expected to be a ColorSpace entry.
	The second setting is a ColorRange entry constructed such that
	the surface color maps to the last entry in the ramp (typically white).
*/
static PEXColorApproxEntry colorapprox_values[]={
	{ PEXColorSpace, PEXColorApproxRGB, 7, 7, 3, PEXOn, 0,
		1, 0, 0, 0.59, 0.11, 0.30, 0 },
	{ PEXColorRange, PEXColorApproxRGB, 7, 0, 0, PEXOn, 0,
		1, 0, 0, 0.59, 0.11, 0.30, 0 },
};

typedef struct {
	int		nms_count;
	PEXName	nms_value;
	int		incl_count;
	PEXName	incl_value;
	int		excl_count;
	PEXName	excl_value;
} nameset_value;

static nameset_value incl_values[]={
	{ 1, 1,		0, 0,	0, 0 },
	{ 1, 1,		1, 1,	0, 0 },
};

static nameset_value excl_values[]={
	{ 1, 1,		1, 1,	0, 0 },
	{ 1, 1,		1, 1,	1, 1 },
};

static nameset_value nms_values[]={
	{ 1, 1,		1, 1,	1, 2 },
	{ 1, 2,		1, 1,	1, 2 },
};

static struct _plcattr_value {
	PEXEnumTypeIndex	line_type;
	PEXBitmask			asf_enables;
	PEXBitmask			asf_values;
} plcattr_values[]={
	{ PEXLineTypeSolid, 	PEXASFLineType,	PEXASFLineType },
	{ PEXLineTypeDashed,  	PEXASFLineType,	0 },
};


/*
typedef struct {
    PEXTableIndex       font_index;
    PEXEnumTypeIndex    precision;
    float               char_expansion;
    float               char_spacing;
    PEXColorSpecifier   color;
} PEXTextBundleEntry;
*/
static struct _text_value {
    float               char_spacing;
	float 				red, green, blue;
} text_values[]={
	{ 0.0, 1.0, 0.0, 1.0 },
	{ 1.0, 0.0, 1.0, 0.0 },
};
static unsigned long text_asfs[]={
	PEXASFCharSpacing,
};
#define NUM_TEXT_ASFS (sizeof(text_asfs)/sizeof(unsigned long))

/*
typedef struct {
    unsigned short      count;
    PEXFont             *fonts;
} PEXTextFontEntry;
*/
static struct _textfont_value {
	char *font_name;
} textfont_values[2];
/*
={
	"-hp-PEX stick-medium-r-normal-normal-0-0-0-0-m-0-hp-roman8",
	"-hp-PEX polygonal sans serif-bold-r-normal-normal-0-0-0-0-p-0-hp-roman8",
};
*/


#define PRIMATTR_LINE_COLOR			(1L << 0)
#define PRIMATTR_LINE_TYPE			(1L << 1)
#define PRIMATTR_LINE_INTERP		(1L << 2)
#define PRIMATTR_MARKER_COLOR		(1L << 3)
#define PRIMATTR_MARKER_TYPE		(1L << 4)
#define PRIMATTR_MARKER_SIZE		(1L << 5)
#define PRIMATTR_INTERIOR_STYLE		(1L << 6)
#define PRIMATTR_INTERIOR_COLOR		(1L << 7)
#define PRIMATTR_INTERIOR_INTERP	(1L << 8)
#define PRIMATTR_REFLECTION_MODEL	(1L << 9)
#define PRIMATTR_REFLECTION_ATTRS	(1L << 10)
#define PRIMATTR_EDGE_FLAG			(1L << 11)
#define PRIMATTR_EDGE_COLOR			(1L << 12)
#define PRIMATTR_EDGE_TYPE			(1L << 13)
#define PRIMATTR_TEXT_COLOR			(1L << 14)
#define PRIMATTR_CHAR_HEIGHT		(1L << 15)
#define PRIMATTR_CHAR_SPACING		(1L << 16)
#define PRIMATTR_NAMESET			(1L << 17)
#define PRIMATTR_MAX_SHIFT			(1L << 18)
#define PRIMATTR_ALL				(PRIMATTR_MAX_SHIFT - 1)

void set_primattrs (Display *display, 
					PEXRenderer renderer, 
					unsigned int mask)
{
	PEXColor color;

	color.rgb.red = 1.0;
	color.rgb.green = 0.0;
	color.rgb.blue = 0.0;

	if (mask & PRIMATTR_LINE_COLOR)
		PEXSetLineColor (display, renderer, PEXOCRender,
						PEXColorTypeRGB, &color);
	if (mask & PRIMATTR_LINE_TYPE)
		PEXSetLineType (display, renderer, PEXOCRender, 
						PEXLineTypeSolid);
	if (mask & PRIMATTR_LINE_INTERP)
		PEXSetPolylineInterpMethod (display, renderer, PEXOCRender, 
						PEXPolylineInterpNone);

	if (mask & PRIMATTR_MARKER_COLOR)
		PEXSetMarkerColor (display, renderer, PEXOCRender,
						PEXColorTypeRGB, &color);
	if (mask & PRIMATTR_MARKER_TYPE)
		PEXSetMarkerType (display, renderer, PEXOCRender, 
						PEXMarkerAsterisk);
	if (mask & PRIMATTR_MARKER_SIZE)
		PEXSetMarkerScale (display, renderer, PEXOCRender, 
						6.0);

	if (mask & PRIMATTR_INTERIOR_COLOR)
		PEXSetSurfaceColor (display, renderer, PEXOCRender, 
							PEXColorTypeRGB, &color);
	if (mask & PRIMATTR_INTERIOR_STYLE)
		PEXSetInteriorStyle (display, renderer, PEXOCRender, 
							PEXInteriorStyleSolid);
	if (mask & PRIMATTR_INTERIOR_INTERP)
		PEXSetSurfaceInterpMethod (display, renderer, PEXOCRender, 
							PEXSurfaceInterpNone);
	if (mask & PRIMATTR_REFLECTION_MODEL)
		PEXSetReflectionModel (display, renderer, PEXOCRender, 
							PEXReflectionNone);
	if (mask & PRIMATTR_REFLECTION_ATTRS) {
		PEXReflectionAttributes refl_attrs;

		refl_attrs.ambient = 1.0;
		refl_attrs.diffuse = 1.0;
		refl_attrs.specular = 1.0;
		refl_attrs.specular_conc = 5.0;
		refl_attrs.transmission = 0.0;
		refl_attrs.specular_color.type = PEXColorTypeRGB;
		refl_attrs.specular_color.value.rgb.red = 1.0;
		refl_attrs.specular_color.value.rgb.green = 1.0;
		refl_attrs.specular_color.value.rgb.blue = 1.0;

		PEXSetReflectionAttributes (display, renderer, PEXOCRender, 
								&refl_attrs);
	}

	if (mask & PRIMATTR_EDGE_FLAG)
		PEXSetSurfaceEdgeFlag (display, renderer, PEXOCRender,
						PEXOff);
	if (mask & PRIMATTR_EDGE_COLOR)
		PEXSetSurfaceEdgeColor (display, renderer, PEXOCRender,
						PEXColorTypeRGB, &color);
	if (mask & PRIMATTR_EDGE_TYPE)
		PEXSetSurfaceEdgeType (display, renderer, PEXOCRender, 
						PEXSurfaceEdgeSolid);

	if (mask & PRIMATTR_TEXT_COLOR)
		PEXSetTextColor (display, renderer, PEXOCRender,
						PEXColorTypeRGB, &color);
	if (mask & PRIMATTR_CHAR_HEIGHT)
		PEXSetCharHeight (display, renderer, PEXOCRender, 
						0.1);
	if (mask & PRIMATTR_CHAR_SPACING)
		PEXSetCharSpacing (display, renderer, PEXOCRender, 
						0.0);

	if (mask & PRIMATTR_NAMESET) {

		PEXName name;

		name = 1;
		PEXAddToNameSet (display, renderer, PEXOCRender, 
						1, &name);
	}
}


static int get_two_fonts (Display *display)
{
    int					i, first_font_index;
    unsigned long		num_fonts;
    char				**font_names;
    PEXExtensionInfo	*pex_info;
	PEXFont				font_id;

    pex_info = PEXGetExtensionInfo( display );

    /* Get the font names and store the first two in the 
		textfont_values table. */
	if ( font_names = PEXListFonts( display, "*", 32, &num_fonts ) ) {
		if (num_fonts < 2) {
			_hppex_stderr_print ( "\t%s\n\t%s\n",
				"WARNING:  Only one text font found,",
				"images may not be distinguishable.\n");
		}

		/* search forward to the first loadable font */
		first_font_index = -1;
		for ( i = 0; i < num_fonts; i++ ) {

			if ((font_id = PEXLoadFont (display, font_names[i])) != None) {

				first_font_index = i;

				if ((textfont_values[0].font_name = 
						malloc (strlen(font_names[first_font_index]) + 1)) 
					== NULL) {
					_hppex_stderr_print ("PROCESS ERROR:  Font name allocation.\n");
					return 1;
				}
				strcpy (textfont_values[0].font_name, font_names[first_font_index]);

				PEXUnloadFont (display, font_id);
				break;
			}
		}
		if (first_font_index == -1) {
			_hppex_stderr_print ("PROCESS ERROR:  No loadable fonts.\n");
			return 1;
		}

		for ( i = (num_fonts-1); i > first_font_index; i-- ) {

			if ((font_id = PEXLoadFont (display, font_names[i])) != None) {

				if (i == first_font_index) {
					_hppex_stderr_print ( "\t%s\n\t%s\n",
						"WARNING:  Only one loadable text font found,",
						"images may not be distinguishable.\n");
				}

				if ((textfont_values[1].font_name = 
						malloc (strlen(font_names[i]) + 1)) 
					== NULL) {
					_hppex_stderr_print ("PROCESS ERROR:  Font name allocation.\n");
					return 1;
				}
				strcpy (textfont_values[1].font_name, font_names[i]);

				PEXUnloadFont (display, font_id);
				break;
			}
		}

		PEXFreeFontNames(  num_fonts, font_names );
	}

    return 0;
}



main(argc, argv)
int argc;
char **argv;
{
	Display *display;
	Window window;
	PEXRenderer renderer;
	PEXRendererAttributes idle_renderer_attrs, set_attrs;
	PEXPCAttributes plc_attrs;
	PEXColor color;
	PEXExtensionInfo *ext_info;

	XVisualInfo vis_info;
	XStandardColormap cmap_info;
	PEXColorApproxEntry capx_info;
	PEXElementRef current_path_elems;
	int screen;
	XSizeHints hints;

	int i, j, loop, attr, subframe, value_index;
	int need_to_clear, need_to_capture;
	clear_value *clear_data;
	float x_offset, y_offset, z_offset;
	float x_increment, y_increment, z_increment;
	unsigned int value_mask, primattr_mask;
	int image_number = 0;
	int dynamic_index;

	PEXLookupTable lut[2], aux_lut;
	PEXNameSet nms[2], aux_nms[2];
	PEXPipelineContext plc[2];
	PEXFont font[2];
	unsigned long rnd_dynamics[4];

	char image_msg[256];

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

    _hppex_set_stderr("rnd_dynam.stderr", "rnd_dynam");

	describe_test("\n\n");
	describe_test("This is a test of PEXlib Renderer attribute dynamics, that is,\n");
	describe_test("whether attribute changes take effect if made while the Renderer is\n");
	describe_test("active.  There are many attributes, some of which are references to\n");
	describe_test("other resources (Lookup Tables, NameSets, and the Pipeline Context).\n");
	describe_test("For those that are resources, dynamics for both a change in the resource\n");
	describe_test("ID (i.e., which resource is in use), and a change in the contents of the\n");
	describe_test("resource, are tested.\n\n");

	describe_test("This test generates images.  For some attributes that cannot be constrained\n");
	describe_test("to the viewport, such as the background color, two window images are captured\n");
	describe_test("per attribute.  For other attributes, the window is divided into four subframes,\n");
	describe_test("order as follows:  0 (lower left), 1 (upper left), 2 (lower right),\n");
	describe_test("                   3 (upper right).\n"); 
	describe_test("Finally, for attributes that can be demonstrated by primitives within one\n");
	describe_test("subframe, primitives are rendered from the bottom of the subframe to the top.\n");
	describe_test("This allows up to four different attributes to be tested in one window image.\n\n");

	describe_test("For each attribute, a description is printed of what the image should look like\n");
	describe_test("if the dynamics are immediate.  The subframe image for a resource ID change and\n");
	describe_test("a resource content change should look identical if they have the same dynamics,\n");
	describe_test("and are drawn in paired subframes (0,1 or 2,3)\n\n");

#ifdef XSYSTEM
	if (xtc.interactiveFramePrompt)
#else
	if (glob_verbose_mode && getenv("PAUSE"))
#endif
		prompt_user ("Return to continue with the test", "");

#ifndef XSYSTEM
    if (glob_verbose_mode)
	    fflush (stdout);
#endif


	/*
		Set up structures describing expected Renderer attributes.
	*/
	init_default_renderer_attrs (&idle_renderer_attrs);
	idle_renderer_attrs.viewport = fullframe_data[0];


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
		Create a window.
	*/

	window = create_and_setup_window (display,
							"PEXlib Renderer Attr Dynamics Test", 
							image_depth,
							0, 0, 
							WIDTH, HEIGHT,
							&capx_info);

	if (window == None) {

		_hppex_stderr_print( "PROCESS ERROR:  create_window failed\n");
		exit (1);
	}

#ifdef XSYSTEM
/*
    XSystemValues[0] = WINDOW_SMART_HW_CONFIG;
    XSystemValues[1] = window;
    XSystemValues[2] = -1;
    defineXSystemTestControlParm(&xtc, ARCHIVE_MODE, XSystemValues);
*/

    XSystemValues[0] = WINDOW_SMART_EXTENT;
    XSystemValues[1] = window;
    defineXSystemTestControlParm(&xtc, ARCHIVE_EXTENT, XSystemValues);
#endif


	/*
		Load the textfont_values table with two 
		(hopefully visually distinguishable) fonts.
	*/

	if (get_two_fonts (display))
		exit (1);


	/*
		Create a Renderer.
	*/
	renderer = PEXCreateRenderer (display, window, 0, NULL);
	if (renderer == None) {

		_hppex_stderr_print( "PROCESS ERROR: PEXCreateRenderer failed\n");
		exit (1);
	}

	/*
		Inquire the Renderer dynamics.  The appropriate bits will be
		printed out as each attribute is announced.  We build up a
		bogus fourth word for attributes that don't have inquirable dynamics.
	*/
	if (0 == PEXGetRendererDynamics (display, renderer, 
					&(rnd_dynamics[TABLES]), &(rnd_dynamics[NAMES]), &(rnd_dynamics[ATTRS]))) {
		_hppex_stderr_print( "PROCESS ERROR: PEXGetRendererDynamics failed\n");
		exit (1);
	}

	rnd_dynamics[NO_ENTRY]= RDABackgroundColor 	|
							RDAClearImage 		|
							RDAClearZ 			|
							RDAPipelineContext 	|
							RDAPipelineContextContents;

	/*
		This code loads addresses within the set_attrs structure into
		the attr_control array, for use in modifying resource ID attributes.
	*/
	attr_control[VIEW_LUT].resource_id_attr = &(set_attrs.view_table);
	attr_control[VIEW_LUT_CONTENTS].resource_id_attr = &(set_attrs.view_table);
	attr_control[COLOR_LUT].resource_id_attr = &(set_attrs.color_table);
	attr_control[COLOR_LUT_CONTENTS].resource_id_attr = &(set_attrs.color_table);
	attr_control[DEPTH_CUE_LUT].resource_id_attr = &(set_attrs.depth_cue_table);
	attr_control[DEPTH_CUE_LUT_CONTENTS].resource_id_attr = &(set_attrs.depth_cue_table);
	attr_control[COLOR_APPROX_LUT].resource_id_attr = &(set_attrs.color_approx_table);
	attr_control[COLOR_APPROX_LUT_CONTENTS].resource_id_attr = &(set_attrs.color_approx_table);
	attr_control[LIGHT_LUT].resource_id_attr = &(set_attrs.light_table);
	attr_control[LIGHT_LUT_CONTENTS].resource_id_attr = &(set_attrs.light_table);
	attr_control[LINE_LUT].resource_id_attr = &(set_attrs.line_bundle);
	attr_control[LINE_LUT_CONTENTS].resource_id_attr = &(set_attrs.line_bundle);
	attr_control[MARKER_LUT].resource_id_attr = &(set_attrs.marker_bundle);
	attr_control[MARKER_LUT_CONTENTS].resource_id_attr = &(set_attrs.marker_bundle);
	attr_control[INTERIOR_LUT].resource_id_attr = &(set_attrs.interior_bundle);
	attr_control[INTERIOR_LUT_CONTENTS].resource_id_attr = &(set_attrs.interior_bundle);
	attr_control[EDGE_LUT].resource_id_attr = &(set_attrs.edge_bundle);
	attr_control[EDGE_LUT_CONTENTS].resource_id_attr = &(set_attrs.edge_bundle);

	attr_control[HIGHLIGHT_INCL].resource_id_attr = &(set_attrs.highlight_incl);
	attr_control[HIGHLIGHT_INCL_CONTENTS].resource_id_attr = &(set_attrs.highlight_incl);
	attr_control[HIGHLIGHT_EXCL].resource_id_attr = &(set_attrs.highlight_excl);
	attr_control[HIGHLIGHT_EXCL_CONTENTS].resource_id_attr = &(set_attrs.highlight_excl);
	attr_control[INVISIBILITY_INCL].resource_id_attr = &(set_attrs.invisibility_incl);
	attr_control[INVISIBILITY_INCL_CONTENTS].resource_id_attr = &(set_attrs.invisibility_incl);
	attr_control[INVISIBILITY_EXCL].resource_id_attr = &(set_attrs.invisibility_excl);
	attr_control[INVISIBILITY_EXCL_CONTENTS].resource_id_attr = &(set_attrs.invisibility_excl);
	attr_control[PICK_INCL].resource_id_attr = &(set_attrs.pick_incl);
	attr_control[PICK_INCL_CONTENTS].resource_id_attr = &(set_attrs.pick_incl);
	attr_control[PICK_EXCL].resource_id_attr = &(set_attrs.pick_excl);
	attr_control[PICK_EXCL_CONTENTS].resource_id_attr = &(set_attrs.pick_excl);

	attr_control[PIPELINE_CONTEXT].resource_id_attr = &(set_attrs.pipeline_context);
	attr_control[PIPELINE_CONTEXT_ATTR].resource_id_attr = &(set_attrs.pipeline_context);
	attr_control[PIPELINE_CONTEXT_ASF].resource_id_attr = &(set_attrs.pipeline_context);
	attr_control[PLC_NAMESET].resource_id_attr = &(set_attrs.pipeline_context);
	attr_control[PLC_NAMESET_CONTENTS].resource_id_attr = &(set_attrs.pipeline_context);

	attr_control[TEXT_LUT].resource_id_attr = &(set_attrs.text_bundle);
	attr_control[TEXT_LUT_CONTENTS].resource_id_attr = &(set_attrs.text_bundle);
	attr_control[TEXT_FONT_LUT].resource_id_attr = &(set_attrs.text_font_table);
	attr_control[TEXT_FONT_LUT_CONTENTS].resource_id_attr = &(set_attrs.text_font_table);

	subframe = 0;
	need_to_clear = True;
	need_to_capture = False;

	x_offset = 0.2;
	y_offset = 0.2;
	z_offset = 0.5;
	x_increment = 0.0;
	y_increment = 0.2;
	z_increment = 0.0;

	/* For each attribute to be tested */
	for (attr = FIRST_ATTR; attr <= LAST_ATTR; attr++) {

		dynamic_index = (rnd_dynamics[attr_control[attr].dyn_index] & attr_control[attr].dyn_bit)? 1:0;

		sprintf (image_msg, "ERROR: %s Attribute dynamics", 
				attr_control[attr].attr_name);

#ifndef XSYSTEM
		if (glob_verbose_mode) {
#endif
			printf ("Attribute %d: %s   Dynamics: %s\n",
					attr, attr_control[attr].attr_name,
					dyn_names[dynamic_index]);

			if (dynamic_index)
				printf ("\t%s\n\t%s\n\t%s\n", 
						attr_control[attr].def_message_0,
						attr_control[attr].def_message_1,
						attr_control[attr].def_message_2);
			else
				printf ("\t%s\n\t%s\n\t%s\n", 
						attr_control[attr].imm_message_0,
						attr_control[attr].imm_message_1,
						attr_control[attr].imm_message_2);

			if ((attr == TEXT_FONT_LUT) ||
				(attr == TEXT_FONT_LUT_CONTENTS)) {
				printf ("\tfirst font is \"%s\"\n", 
							textfont_values[0].font_name);
				printf ("\tsecond font is \"%s\"\n", 
							textfont_values[1].font_name);
			}
#ifndef XSYSTEM
		}
#endif

		if ((attr == COLOR_APPROX_LUT) ||
			(attr == COLOR_APPROX_LUT_CONTENTS)) {

			int					enum_types[1];
			unsigned long 		*count;
			PEXEnumTypeDesc		*enum_data;
			int					enum_index;
			int					found;

			/*
				Verify that PEXColorRange is supported
				using PEXGetEnumTypeInfo.  If not, skip this attribute.
			*/

			enum_types[0] = PEXETColorApproxType;
			if (!PEXGetEnumTypeInfo (display, window, 1, 
						enum_types, PEXETIndex, 
						&count, &enum_data)) {

				_hppex_stderr_print ("PEXGetEnumTypeInfo failure!\n");
				exit (1);
			}

			found = False;
			for (enum_index = 0; enum_index < *count; enum_index++) {
				if ((short) enum_data[enum_index].index == 
					(short) PEXColorRange) {
				found = True;
				break;
				}
			}
			PEXFreeEnumInfo (1, count, enum_data);

			if (!found) {
#ifndef XSYSTEM
				if (glob_verbose_mode) 
#endif
				_hppex_stderr_print ( "%s %s\n",
					"Skipping color approx because PEXColorRange",
					"not supported.");

				continue;
			}
		}

		PEXChangeRenderer (display, renderer, 
							ALL_RENDERER_ATTRS_MASK, &idle_renderer_attrs);

		need_to_clear = (need_to_clear || 
						attr_control[attr].image_per_setting ||
						attr_control[attr].subframe_per_setting);

		/*
			Create resources that will be needed for testing
			this attribute.  Also set a flag describing the nature
			of the attribute, and various values and addresses.
			These allow the attribute-setting code inside the inner
			loop to be collapsed to a relatively small number of cases.

			See the balancing code to free the resources at the end of
			the loop on attributes.  Note that references to resources
			from the Renderer are eliminated by the blanket ChangeRenderer
			call to set all attributes, above.

			Some attributes don't have any pre-loop resource setup to do.
		*/
		switch (attr) {

		case VIEW_LUT:
		case COLOR_LUT:
		case DEPTH_CUE_LUT:
		case COLOR_APPROX_LUT:
		case LIGHT_LUT:
		case LINE_LUT:
		case MARKER_LUT:
		case INTERIOR_LUT:
		case EDGE_LUT:
		case TEXT_LUT:
		case TEXT_FONT_LUT:
			lut[0] = PEXCreateLookupTable (display, window, attr_control[attr].lut_type);
			lut[1] = PEXCreateLookupTable (display, window, attr_control[attr].lut_type);
			break;

		case VIEW_LUT_CONTENTS:
		case COLOR_LUT_CONTENTS:
		case DEPTH_CUE_LUT_CONTENTS:
		case COLOR_APPROX_LUT_CONTENTS:
		case LIGHT_LUT_CONTENTS:
		case LINE_LUT_CONTENTS:
		case MARKER_LUT_CONTENTS:
		case INTERIOR_LUT_CONTENTS:
		case EDGE_LUT_CONTENTS:
		case TEXT_LUT_CONTENTS:
		case TEXT_FONT_LUT_CONTENTS:
			lut[0] = PEXCreateLookupTable (display, window, attr_control[attr].lut_type);
			*(attr_control[attr].resource_id_attr) = lut[0];
			PEXChangeRenderer (display, renderer, attr_control[attr].resource_RA_bit, &set_attrs);
			break;

		case HIGHLIGHT_INCL:
		case HIGHLIGHT_INCL_CONTENTS:
			nms[0] = PEXCreateNameSet (display);
			if (attr == HIGHLIGHT_INCL) {
				nms[1] = PEXCreateNameSet (display);
			}
			else {
				set_attrs.highlight_incl = nms[0];
				PEXChangeRenderer (display, renderer, PEXRAHighlightIncl, &set_attrs);
			}

			aux_nms[0] = PEXCreateNameSet (display);
			PEXChangeNameSet (display, aux_nms[0], PEXNSReplace, 
								incl_values[0].excl_count,
								&(incl_values[0].excl_value));
			set_attrs.highlight_excl = aux_nms[0];
			PEXChangeRenderer (display, renderer, PEXRAHighlightExcl, &set_attrs);
			break;

		case HIGHLIGHT_EXCL:
		case HIGHLIGHT_EXCL_CONTENTS:
			nms[0] = PEXCreateNameSet (display);
			if (attr == HIGHLIGHT_EXCL) {
				nms[1] = PEXCreateNameSet (display);
			}
			else {
				set_attrs.highlight_excl = nms[0];
				PEXChangeRenderer (display, renderer, PEXRAHighlightExcl, &set_attrs);
			}

			aux_nms[0] = PEXCreateNameSet (display);
			PEXChangeNameSet (display, aux_nms[0], PEXNSReplace, 
								excl_values[0].incl_count,
								&(excl_values[0].incl_value));
			set_attrs.highlight_incl = aux_nms[0];
			PEXChangeRenderer (display, renderer, PEXRAHighlightIncl, &set_attrs);
			break;

		case INVISIBILITY_INCL:
		case INVISIBILITY_INCL_CONTENTS:
			nms[0] = PEXCreateNameSet (display);
			if (attr == INVISIBILITY_INCL) {
				nms[1] = PEXCreateNameSet (display);
			}
			else {
				set_attrs.invisibility_incl = nms[0];
				PEXChangeRenderer (display, renderer, PEXRAInvisibilityIncl, &set_attrs);
			}

			aux_nms[0] = PEXCreateNameSet (display);
			PEXChangeNameSet (display, aux_nms[0], PEXNSReplace, 
								incl_values[0].excl_count,
								&(incl_values[0].excl_value));
			set_attrs.invisibility_excl = aux_nms[0];
			PEXChangeRenderer (display, renderer, PEXRAInvisibilityExcl, &set_attrs);
			break;

		case INVISIBILITY_EXCL:
		case INVISIBILITY_EXCL_CONTENTS:
			nms[0] = PEXCreateNameSet (display);
			if (attr == INVISIBILITY_EXCL) {
				nms[1] = PEXCreateNameSet (display);
			}
			else {
				set_attrs.invisibility_excl = nms[0];
				PEXChangeRenderer (display, renderer, PEXRAInvisibilityExcl, &set_attrs);
			}

			aux_nms[0] = PEXCreateNameSet (display);
			PEXChangeNameSet (display, aux_nms[0], PEXNSReplace, 
								excl_values[0].incl_count,
								&(excl_values[0].incl_value));
			set_attrs.invisibility_incl = aux_nms[0];
			PEXChangeRenderer (display, renderer, PEXRAInvisibilityIncl, &set_attrs);
			break;

		case PICK_INCL:
		case PICK_INCL_CONTENTS:
			nms[0] = PEXCreateNameSet (display);
			if (attr == PICK_INCL) {
				nms[1] = PEXCreateNameSet (display);
			}
			else {
				set_attrs.pick_incl = nms[0];
				PEXChangeRenderer (display, renderer, PEXRAPickIncl, &set_attrs);
			}

			aux_nms[0] = PEXCreateNameSet (display);
			PEXChangeNameSet (display, aux_nms[0], PEXNSReplace, 
								incl_values[0].excl_count,
								&(incl_values[0].excl_value));
			set_attrs.pick_excl = aux_nms[0];
			PEXChangeRenderer (display, renderer, PEXRAPickExcl, &set_attrs);
			break;

		case PICK_EXCL:
		case PICK_EXCL_CONTENTS:
			nms[0] = PEXCreateNameSet (display);
			if (attr == PICK_EXCL) {
				nms[1] = PEXCreateNameSet (display);
			}
			else {
				set_attrs.pick_excl = nms[0];
				PEXChangeRenderer (display, renderer, PEXRAPickExcl, &set_attrs);
			}

			aux_nms[0] = PEXCreateNameSet (display);
			PEXChangeNameSet (display, aux_nms[0], PEXNSReplace, 
								excl_values[0].incl_count,
								&(excl_values[0].incl_value));
			set_attrs.pick_incl = aux_nms[0];
			PEXChangeRenderer (display, renderer, PEXRAPickIncl, &set_attrs);
			break;

		case PIPELINE_CONTEXT:
		case PIPELINE_CONTEXT_ATTR:
		case PIPELINE_CONTEXT_ASF:
			{
				unsigned long plc_mask[3];

				plc_mask[0] = 0;
				plc_mask[1] = 0;
				plc_mask[2] = 0;
				plc[0] = PEXCreatePipelineContext (display, plc_mask, NULL);

				if (attr == PIPELINE_CONTEXT) {
					plc[1] = PEXCreatePipelineContext (display, plc_mask, NULL);
				}
				else if ((attr == PIPELINE_CONTEXT_ATTR) ||
						(attr == PIPELINE_CONTEXT_ASF)) {

					set_attrs.pipeline_context = plc[0];
					PEXChangeRenderer (display, renderer, PEXRAPipelineContext, &set_attrs);

					if (attr == PIPELINE_CONTEXT_ASF) {
						PEXLineBundleEntry line_entry, *tmp_entry;

						aux_lut = PEXCreateLookupTable (display, window, PEXLUTLineBundle);
						set_attrs.line_bundle = aux_lut;
						PEXChangeRenderer (display, renderer, PEXRALineBundle, &set_attrs);

						PEXGetPredefinedEntries (display, window, 
							PEXLUTLineBundle, 1, 1, 
							((PEXPointer *) &tmp_entry));
						line_entry = *tmp_entry;
						PEXFreeTableEntries (PEXLUTLineBundle, 1, tmp_entry);

						line_entry.type = plcattr_values[1].line_type;
						line_entry.interp_method = PEXPolylineInterpNone;
						line_entry.color.type = PEXColorTypeRGB;
						line_entry.color.value.rgb.red = 1.0;
						line_entry.color.value.rgb.green = 0.0;
						line_entry.color.value.rgb.blue = 1.0;

						PEXSetTableEntries (display, aux_lut, 1, 1, 
								PEXLUTLineBundle, ((PEXPointer) &line_entry));
					}
				}
			}
			break;

		case PLC_NAMESET:
		case PLC_NAMESET_CONTENTS:
			{
				unsigned long plc_mask[3];

				plc_mask[0] = 0;
				plc_mask[1] = 0;
				plc_mask[2] = 0;
				plc[0] = PEXCreatePipelineContext (display, plc_mask, NULL);
				set_attrs.pipeline_context = plc[0];

				aux_nms[0] = PEXCreateNameSet (display);
				PEXChangeNameSet (display, aux_nms[0], PEXNSReplace, 
									nms_values[0].incl_count,
									&(nms_values[0].incl_value));
				set_attrs.invisibility_incl = aux_nms[0];

				aux_nms[1] = PEXCreateNameSet (display);
				PEXChangeNameSet (display, aux_nms[1], PEXNSReplace, 
									nms_values[0].excl_count,
									&(nms_values[0].excl_value));
				set_attrs.invisibility_excl = aux_nms[1];

				PEXChangeRenderer (display, renderer, 
									PEXRAPipelineContext |
									PEXRAInvisibilityIncl |
									PEXRAInvisibilityExcl, 
									&set_attrs);

				nms[0] = PEXCreateNameSet (display);
				if (attr == PLC_NAMESET) {
					nms[1] = PEXCreateNameSet (display);
				}
				else if (attr == PLC_NAMESET_CONTENTS) {
					plc_attrs.name_set = nms[0];
					PEXSetPCAttributeMask (plc_mask, PEXPCNameSet);
					PEXChangePipelineContext (display, plc[0], plc_mask, &plc_attrs);
				}
			}
			break;
		}

		/*
			This code disables the setting of certain default primitive attributes,
			which is done after a bind (rather than loading them into a Pipeline Context
			associated with the Renderer.  The attributes that are overridden are typically
			set by the primitive-drawing section, to which this code is coupled.
		*/
		primattr_mask = PRIMATTR_ALL;
		switch (attr) {

		case COLOR_LUT:
		case COLOR_LUT_CONTENTS:
			primattr_mask &= ~PRIMATTR_LINE_COLOR;
			break;

		case DEPTH_CUE_LUT:
		case DEPTH_CUE_LUT_CONTENTS:
			primattr_mask &= ~PRIMATTR_INTERIOR_COLOR;
			break;

		case COLOR_APPROX_LUT:
		case COLOR_APPROX_LUT_CONTENTS:
			primattr_mask &= ~PRIMATTR_INTERIOR_COLOR;
			primattr_mask &= ~PRIMATTR_EDGE_FLAG;
			primattr_mask &= ~PRIMATTR_EDGE_COLOR;
			break;

		case LIGHT_LUT:
		case LIGHT_LUT_CONTENTS:
			primattr_mask &= ~PRIMATTR_INTERIOR_COLOR;
			primattr_mask &= ~PRIMATTR_INTERIOR_INTERP;
			primattr_mask &= ~PRIMATTR_REFLECTION_MODEL;
			break;

		case LINE_LUT:
		case LINE_LUT_CONTENTS:
			primattr_mask &= ~PRIMATTR_LINE_TYPE;
			break;

		case MARKER_LUT:
		case MARKER_LUT_CONTENTS:
			primattr_mask &= ~PRIMATTR_MARKER_TYPE;
			break;

		case INTERIOR_LUT:
		case INTERIOR_LUT_CONTENTS:
			primattr_mask &= ~PRIMATTR_INTERIOR_STYLE;
			break;

		case EDGE_LUT:
		case EDGE_LUT_CONTENTS:
			primattr_mask &= ~PRIMATTR_EDGE_FLAG;
			primattr_mask &= ~PRIMATTR_EDGE_COLOR;
			primattr_mask &= ~PRIMATTR_INTERIOR_STYLE;
			break;

		case PIPELINE_CONTEXT:
		case PIPELINE_CONTEXT_ATTR:
		case PIPELINE_CONTEXT_ASF:
			primattr_mask &= ~PRIMATTR_LINE_TYPE;
			break;

		case PLC_NAMESET:
		case PLC_NAMESET_CONTENTS:
			primattr_mask &= ~PRIMATTR_NAMESET;
			break;

		case TEXT_LUT:
		case TEXT_LUT_CONTENTS:
			primattr_mask &= ~PRIMATTR_CHAR_SPACING;
			break;

		}


		for (loop = 0; loop < 3; loop++) {

			if (!attr_control[attr].image_per_setting) {

				set_attrs.viewport = subframe_data[subframe];
				PEXChangeRenderer (display, renderer, PEXRAViewport, &set_attrs);
			}

			if (need_to_clear) {
				set_attrs.clear_image = True;
				set_attrs.clear_z = True;
				set_attrs.background_color.type = PEXColorTypeRGB;
				set_attrs.background_color.value.rgb.red = 0.0;
				set_attrs.background_color.value.rgb.green = 0.0;
				set_attrs.background_color.value.rgb.blue = 0.0;
				value_mask = PEXRAClearImage|
							PEXRABackgroundColor|
							PEXRAClearZ;
				if (attr != HLHSR_MODE) {
					set_attrs.hlhsr_mode = PEXHLHSRZBuffer;
					value_mask |= PEXRAHLHSRMode;
				}
				PEXChangeRenderer (display, renderer, value_mask, &set_attrs);
				PEXBeginRendering (display, window, renderer);
				PEXEndRendering (display, renderer, True);

				set_attrs.clear_image = False;
				set_attrs.clear_z = False;
				value_mask = PEXRAClearImage|
							PEXRAClearZ;
				PEXChangeRenderer (display, renderer, value_mask, &set_attrs);

				need_to_clear = False;
			}


			if (loop_control[loop].set_value) {
				/*
					Set attribute to first or second value.
				*/

				value_index = loop_control[loop].value_index;
				switch (attr) {

				case CLEAR_Z:
				case CLEAR_IMAGE:
				case BACKGROUND_COLOR:
					if (attr == CLEAR_IMAGE)
						clear_data = clearimage_values + value_index;
					else if (attr == CLEAR_Z)
						clear_data = clearz_values + value_index;
					else if (attr == BACKGROUND_COLOR)
						clear_data = background_values + value_index;
					set_attrs.clear_image = clear_data->clear_image;
					set_attrs.clear_z = clear_data->clear_z;
					set_attrs.background_color.type = PEXColorTypeRGB;
					set_attrs.background_color.value.rgb.red = clear_data->red;
					set_attrs.background_color.value.rgb.green = clear_data->green;
					set_attrs.background_color.value.rgb.blue = clear_data->blue;
					set_attrs.hlhsr_mode = clear_data->hlhsr_mode;
					PEXChangeRenderer (display, renderer, 
										PEXRAClearImage |
										PEXRAClearZ |
										PEXRABackgroundColor |
										PEXRAHLHSRMode, &set_attrs);
					break;

				case HLHSR_MODE:
					set_attrs.hlhsr_mode = hlhsrmode_values[value_index].value;
					PEXChangeRenderer (display, renderer, PEXRAHLHSRMode, &set_attrs);
					break;

				case VIEWPORT:
					set_attrs.viewport = viewport_values[value_index].value;
					PEXChangeRenderer (display, renderer, PEXRAViewport, &set_attrs);
					break;

				case CLIPLIST:
					set_attrs.clip_list.count = cliplist_values[value_index].count;
					set_attrs.clip_list.rectangles = cliplist_values[value_index].rectangles;
					PEXChangeRenderer (display, renderer, PEXRAClipList, &set_attrs);
					break;

				case NPC_SUBVOLUME:
					set_attrs.npc_subvolume = npc_values[value_index].value;
					PEXChangeRenderer (display, renderer, PEXRANPCSubVolume, &set_attrs);
					break;

				case ECHO_MODE:
					set_attrs.echo_mode = echomode_values[value_index].value;
					PEXChangeRenderer (display, renderer, PEXRAEchoMode, &set_attrs);
					break;

				case VIEW_LUT:
				case VIEW_LUT_CONTENTS:
				case COLOR_LUT:
				case COLOR_LUT_CONTENTS:
				case DEPTH_CUE_LUT:
				case DEPTH_CUE_LUT_CONTENTS:
				case COLOR_APPROX_LUT:
				case COLOR_APPROX_LUT_CONTENTS:
				case LIGHT_LUT:
				case LIGHT_LUT_CONTENTS:
				case LINE_LUT:
				case LINE_LUT_CONTENTS:
				case MARKER_LUT:
				case MARKER_LUT_CONTENTS:
				case INTERIOR_LUT:
				case INTERIOR_LUT_CONTENTS:
				case EDGE_LUT:
				case EDGE_LUT_CONTENTS:
				case TEXT_LUT:
				case TEXT_LUT_CONTENTS:
				case TEXT_FONT_LUT:
				case TEXT_FONT_LUT_CONTENTS:
					{
						int lut_index;

						if (attr_control[attr].resource_attr_type == RESOURCE_ID)
							lut_index = value_index;
						else if (attr_control[attr].resource_attr_type == RESOURCE_CONTENTS)
							lut_index = 0;

						switch (attr) {

						case VIEW_LUT:
						case VIEW_LUT_CONTENTS:
						{
							PEXViewEntry view_entry;
							int result;

							view_entry.clip_flags = 0;
							view_entry.clip_limits.max.x = 1.0;
							view_entry.clip_limits.max.y = 1.0;
							view_entry.clip_limits.max.z = 1.0;
							result = PEXViewOrientationMatrix (
								&(view_values[value_index].vrp),
								&(view_values[value_index].vpn),
								&(view_values[value_index].vup),
								view_entry.orientation);
							result = PEXViewMappingMatrix (
								view_values[value_index].frame,
								&(view_values[value_index].viewport),
								view_values[value_index].perspective,
								&(view_values[value_index].prp),
								view_values[value_index].view_plane,
								view_values[value_index].back_plane,
								view_values[value_index].front_plane,
								view_entry.mapping);

							PEXSetTableEntries (display, lut[lut_index], 0, 1, 
										PEXLUTView, ((PEXPointer) &view_entry));
						}
						break;

						case COLOR_LUT:
						case COLOR_LUT_CONTENTS:	
						{
							PEXColorSpecifier color_entry;

							color_entry.type = PEXColorTypeRGB;
							color_entry.value.rgb.red = color_values[value_index].red;
							color_entry.value.rgb.green = color_values[value_index].green;
							color_entry.value.rgb.blue = color_values[value_index].blue;
							PEXSetTableEntries (display, lut[lut_index], 1, 1, 
												PEXLUTColor, ((PEXPointer) &color_entry));

						}
						break;

						case DEPTH_CUE_LUT:
						case DEPTH_CUE_LUT_CONTENTS:	
						{
							PEXDepthCueEntry depthcue_entry;

							depthcue_entry.mode = depthcue_values[value_index].mode;
							depthcue_entry.front_plane = depthcue_values[value_index].front_plane;
							depthcue_entry.back_plane = depthcue_values[value_index].back_plane;
							depthcue_entry.front_scaling = depthcue_values[value_index].front_scaling;
							depthcue_entry.back_scaling = depthcue_values[value_index].back_scaling;
							depthcue_entry.color.type = PEXColorTypeRGB;
							depthcue_entry.color.value.rgb.red = depthcue_values[value_index].red;
							depthcue_entry.color.value.rgb.green = depthcue_values[value_index].green;
							depthcue_entry.color.value.rgb.blue = depthcue_values[value_index].blue;
							PEXSetTableEntries (display, lut[lut_index], 0, 1, 
												PEXLUTDepthCue, ((PEXPointer) &depthcue_entry));
						}
						break;

						case COLOR_APPROX_LUT:
						case COLOR_APPROX_LUT_CONTENTS:
						{
							PEXColorApproxEntry colorapprox_entry, tmp_entry;

							tmp_entry = capx_info;

							if (tmp_entry.type != PEXColorSpace) {
								_hppex_stderr_print( "%s %s\n",
										"PROCESS ERROR:  Color Approximation entry in use",
										"is not a ColorSpace entry");
								continue;
							}

							if (value_index == 0) {
								colorapprox_entry = tmp_entry;
							}
							else if (value_index == 1) {
								int num_colors;

								/*
									Build a ColorRange entry such that a pure red RGB
									maps to the last entry in the ColorSpace ramp.
								*/
									
								if ((vis_info.class == DirectColor) ||
									(vis_info.class == TrueColor)) {

									num_colors = MAX((int)(tmp_entry.max1 + 1),
													(int)(tmp_entry.max2 + 1));
									num_colors = MAX(num_colors,
													(int) (tmp_entry.max3 + 1));
								}
								else
									num_colors = (tmp_entry.max1 + 1) *
												(tmp_entry.max2 + 1) *
												(tmp_entry.max3 + 1);

								colorapprox_entry.type = PEXColorRange;
								colorapprox_entry.model = PEXColorApproxRGB;
								colorapprox_entry.max1 = num_colors - 1;
								colorapprox_entry.max2 = 0;
								colorapprox_entry.max3 = 0;
								colorapprox_entry.dither = PEXOn;
								colorapprox_entry.mult1 = 1.0;
								colorapprox_entry.mult2 = 0.0;
								colorapprox_entry.mult3 = 0.0;
								colorapprox_entry.weight1 = 1.0;
								colorapprox_entry.weight2 = 0.0;
								colorapprox_entry.weight3 = 0.0;
								colorapprox_entry.base_pixel = tmp_entry.base_pixel;
							}
								
							PEXSetTableEntries (display, lut[lut_index], 0, 1, 
												PEXLUTColorApprox, ((PEXPointer) &colorapprox_entry));
						}
						break;

						case LIGHT_LUT:
						case LIGHT_LUT_CONTENTS:
						{
							PEXLightEntry light_entry;

							light_entry.type = light_values[value_index].type;
							light_entry.color.type = PEXColorTypeRGB;
							light_entry.color.value.rgb.red = light_values[value_index].red;
							light_entry.color.value.rgb.green = light_values[value_index].green;
							light_entry.color.value.rgb.blue = light_values[value_index].blue;
							PEXSetTableEntries (display, lut[lut_index], 1, 1, 
												PEXLUTLight, ((PEXPointer) &light_entry));
						}
						break;

						case LINE_LUT:
						case LINE_LUT_CONTENTS:
						{
							PEXLineBundleEntry line_entry, *tmp_entry;

							PEXGetPredefinedEntries (display, window, PEXLUTLineBundle, 1, 1, 
													((PEXPointer *) &tmp_entry));
							line_entry = *tmp_entry;
							PEXFreeTableEntries (PEXLUTLineBundle, 1, tmp_entry);

							line_entry.type = line_values[value_index].type;
							line_entry.interp_method = line_values[value_index].interp_method;
							line_entry.color.type = PEXColorTypeRGB;
							line_entry.color.value.rgb.red = line_values[value_index].red;
							line_entry.color.value.rgb.green = line_values[value_index].green;
							line_entry.color.value.rgb.blue = line_values[value_index].blue;
							PEXSetTableEntries (display, lut[lut_index], 1, 1, 
												PEXLUTLineBundle, ((PEXPointer) &line_entry));
						}
						break;

						case MARKER_LUT:
						case MARKER_LUT_CONTENTS:
						{
							PEXMarkerBundleEntry marker_entry;

							marker_entry.type = marker_values[value_index].type;
							marker_entry.color.type = PEXColorTypeRGB;
							marker_entry.color.value.rgb.red = marker_values[value_index].red;
							marker_entry.color.value.rgb.green = marker_values[value_index].green;
							marker_entry.color.value.rgb.blue = marker_values[value_index].blue;
							PEXSetTableEntries (display, lut[lut_index], 1, 1, 
												PEXLUTMarkerBundle, ((PEXPointer) &marker_entry));
						}
						break;

						case INTERIOR_LUT:
						case INTERIOR_LUT_CONTENTS:
						{
							PEXInteriorBundleEntry interior_entry, *tmp_entry;
							PEXGetPredefinedEntries (display, window, PEXLUTInteriorBundle, 1, 1, 
													((PEXPointer *) &tmp_entry));
							interior_entry = *tmp_entry;
							PEXFreeTableEntries (PEXLUTInteriorBundle, 1, tmp_entry);

							interior_entry.style = interior_values[value_index].style;
							interior_entry.color.type = PEXColorTypeRGB;
							interior_entry.color.value.rgb.red = interior_values[value_index].red;
							interior_entry.color.value.rgb.green = interior_values[value_index].green;
							interior_entry.color.value.rgb.blue = interior_values[value_index].blue;

							/* 
								All of the following field settings are a workaround 
								for a defect in OCA/LUT init.
								Currently the predefined entry that is fetched is bad.
							*/
							interior_entry.reflection_model = PEXReflectionNone;
							interior_entry.interp_method = PEXSurfaceInterpNone;
							interior_entry.bf_style = PEXInteriorStyleSolid;
							interior_entry.bf_reflection_model = PEXReflectionNone;
							interior_entry.bf_interp_method = PEXSurfaceInterpNone;
							interior_entry.surface_approx.method = PEXApproxImpDep;

							PEXSetTableEntries (display, lut[lut_index], 1, 1, 
												PEXLUTInteriorBundle, ((PEXPointer) &interior_entry));
						}
						break;

						case EDGE_LUT:
						case EDGE_LUT_CONTENTS:
						{
							PEXEdgeBundleEntry edge_entry, *tmp_entry;

							PEXGetPredefinedEntries (display, window, PEXLUTEdgeBundle, 1, 1, 
													((PEXPointer *) &tmp_entry));
							edge_entry = *tmp_entry;
							PEXFreeTableEntries (PEXLUTEdgeBundle, 1, tmp_entry);

							edge_entry.edge_flag = edge_values[value_index].edge_flag;
							edge_entry.type = edge_values[value_index].type;
							edge_entry.color.type = PEXColorTypeRGB;
							edge_entry.color.value.rgb.red = edge_values[value_index].red;
							edge_entry.color.value.rgb.green = edge_values[value_index].green;
							edge_entry.color.value.rgb.blue = edge_values[value_index].blue;

							PEXSetTableEntries (display, lut[lut_index], 1, 1, 
												PEXLUTEdgeBundle, ((PEXPointer) &edge_entry));
						}
						break;

						case TEXT_LUT:
						case TEXT_LUT_CONTENTS:
						{
							PEXTextBundleEntry text_entry, *tmp_entry;

							PEXGetPredefinedEntries (display, window, PEXLUTTextBundle, 1, 1, 
													((PEXPointer *) &tmp_entry));
							text_entry = *tmp_entry;
							PEXFreeTableEntries (PEXLUTTextBundle, 1, tmp_entry);

							text_entry.char_spacing = text_values[value_index].char_spacing;
							text_entry.color.type = PEXColorTypeRGB;
							text_entry.color.value.rgb.red = text_values[value_index].red;
							text_entry.color.value.rgb.green = text_values[value_index].green;
							text_entry.color.value.rgb.blue = text_values[value_index].blue;

							PEXSetTableEntries (display, lut[lut_index], 1, 1, 
												PEXLUTTextBundle, ((PEXPointer) &text_entry));
						}
						break;

						case TEXT_FONT_LUT:
						case TEXT_FONT_LUT_CONTENTS:
						{
							PEXTextFontEntry textfont_entry;

							font[value_index] = PEXLoadFont(display, 
													textfont_values[value_index].font_name);

							textfont_entry.count = 1;
							textfont_entry.fonts = &(font[value_index]);

							PEXSetTableEntries (display, lut[lut_index], 1, 1, 
												PEXLUTTextFont, ((PEXPointer) &textfont_entry));
						}
						break;

						} /* switch on attr to set contents of resource */

						if (attr_control[attr].resource_attr_type == RESOURCE_ID) {
							*(attr_control[attr].resource_id_attr) = lut[lut_index];
							PEXChangeRenderer (display, renderer, 
											attr_control[attr].resource_RA_bit, &set_attrs);
						}
					}
					break;

				case HIGHLIGHT_INCL:
				case HIGHLIGHT_INCL_CONTENTS:
				case HIGHLIGHT_EXCL:
				case HIGHLIGHT_EXCL_CONTENTS:
				case INVISIBILITY_INCL:
				case INVISIBILITY_INCL_CONTENTS:
				case INVISIBILITY_EXCL:
				case INVISIBILITY_EXCL_CONTENTS:
				case PICK_INCL:
				case PICK_INCL_CONTENTS:
				case PICK_EXCL:
				case PICK_EXCL_CONTENTS:
					{
						int nms_index;

						if (attr_control[attr].resource_attr_type == RESOURCE_ID)
							nms_index = value_index;
						else if (attr_control[attr].resource_attr_type == RESOURCE_CONTENTS)
							nms_index = 0;

						switch (attr) {

						case HIGHLIGHT_INCL:
						case HIGHLIGHT_INCL_CONTENTS:
						case INVISIBILITY_INCL:
						case INVISIBILITY_INCL_CONTENTS:
						case PICK_INCL:
						case PICK_INCL_CONTENTS:
							PEXChangeNameSet (display, nms[nms_index], PEXNSReplace, 
												incl_values[value_index].incl_count,
												&(incl_values[value_index].incl_value));
						break;

						case HIGHLIGHT_EXCL:
						case HIGHLIGHT_EXCL_CONTENTS:
						case INVISIBILITY_EXCL:
						case INVISIBILITY_EXCL_CONTENTS:
						case PICK_EXCL:
						case PICK_EXCL_CONTENTS:
							PEXChangeNameSet (display, nms[nms_index], PEXNSReplace, 
												excl_values[value_index].excl_count,
												&(excl_values[value_index].excl_value));
						break;

						} /* switch on attr to set resource contents */

						if (attr_control[attr].resource_attr_type == RESOURCE_ID) {
							*(attr_control[attr].resource_id_attr) = nms[nms_index];
							PEXChangeRenderer (display, renderer, 
											attr_control[attr].resource_RA_bit, &set_attrs);
						}
					}
					break;

				case PIPELINE_CONTEXT:
				case PIPELINE_CONTEXT_ATTR:
				case PIPELINE_CONTEXT_ASF:
					{
						int plc_index;
						unsigned long plc_mask[3];

						plc_mask[0] = 0;
						plc_mask[1] = 0;
						plc_mask[2] = 0;

						if (attr_control[attr].resource_attr_type == RESOURCE_ID)
							plc_index = value_index;
						else if (attr_control[attr].resource_attr_type == RESOURCE_CONTENTS)
							plc_index = 0;

						if ((attr == PIPELINE_CONTEXT) ||
							(attr == PIPELINE_CONTEXT_ATTR)) {

							plc_attrs.line_type = plcattr_values[value_index].line_type;
							PEXSetPCAttributeMask (plc_mask, PEXPCLineType);
							PEXChangePipelineContext (display, plc[plc_index], plc_mask, &plc_attrs);
						}
						else if (attr == PIPELINE_CONTEXT_ASF) {
							plc_attrs.asf_enables = plcattr_values[value_index].asf_enables;
							plc_attrs.asf_values = plcattr_values[value_index].asf_values;
							PEXSetPCAttributeMask (plc_mask, PEXPCASFValues);
							PEXChangePipelineContext (display, plc[plc_index], plc_mask, &plc_attrs);
						}

						if (attr_control[attr].resource_attr_type == RESOURCE_ID) {
							*(attr_control[attr].resource_id_attr) = plc[plc_index];
							PEXChangeRenderer (display, renderer, 
											attr_control[attr].resource_RA_bit, &set_attrs);
						}
					}
					break;

				case PLC_NAMESET:
				case PLC_NAMESET_CONTENTS:
					{
						int nms_index;
						unsigned long plc_mask[3];

						if (attr_control[attr].resource_attr_type == RESOURCE_ID)
							nms_index = value_index;
						else if (attr_control[attr].resource_attr_type == RESOURCE_CONTENTS)
							nms_index = 0;

						PEXChangeNameSet (display, nms[nms_index], PEXNSReplace, 
											nms_values[value_index].nms_count,
											&(nms_values[value_index].nms_value));

						if (attr_control[attr].resource_attr_type == RESOURCE_ID) {
							plc_attrs.name_set = nms[nms_index];
							PEXSetPCAttributeMask (plc_mask, PEXPCNameSet);
							PEXChangePipelineContext (display, plc[0], plc_mask, &plc_attrs);
						}
					}
					break;

				} /* big switch on attr to set attribute values */
			}


			if (loop_control[loop].bind) {

				/*
					Bind Renderer to the window.
				*/
				PEXBeginRendering(display, window, renderer);

				set_primattrs (display, renderer, primattr_mask);
			}


			/*
				Draw something.
			*/
			switch (attr) {

			case CLEAR_IMAGE:
				if (loop == 0) {
					/* 
						Draw a filled rectangle in first image,
						it should be cleared in the second.
					*/
					draw_primitive (display, renderer, PEXOCRender,
									FILLED_RECTANGLE,
									0.2, 0.2, 0.0,
									0.6, 0.6, 0.0);
				}
				break;

			case HLHSR_MODE:
				/*
					Draw two overlapping rectangles,
					front before back.  If HLHSR is working
					they will be drawn correctly, otherwise
					they will overlap the wrong way.
				*/
				color.rgb.red = 1.0;
				color.rgb.green = 0.0;
				color.rgb.blue = 0.0;
				PEXSetSurfaceColor (display, renderer, PEXOCRender,
								PEXColorTypeRGB, &color);
				draw_primitive (display, renderer, PEXOCRender,
								FILLED_RECTANGLE,
								0.2, 0.2, 1.0,
								0.4, 0.4, 0.0);
				color.rgb.red = 0.0;
				color.rgb.green = 1.0;
				color.rgb.blue = 0.0;
				PEXSetSurfaceColor (display, renderer, PEXOCRender,
								PEXColorTypeRGB, &color);
				draw_primitive (display, renderer, PEXOCRender,
								FILLED_RECTANGLE,
								0.4, 0.4, 0.0,
								0.4, 0.4, 0.0);
				break;

			case CLEAR_Z:
				/*
					First image, preset z-buffer to 0.5
					by drawing a rectangle.
				*/
				if (loop == 0) {
					color.rgb.red = 0.5;
					color.rgb.green = 0.5;
					color.rgb.blue = 0.5;
					PEXSetSurfaceColor (display, renderer, PEXOCRender,
									PEXColorTypeRGB, &color);
					draw_primitive (display, renderer, PEXOCRender,
									FILLED_RECTANGLE,
									0.0, 0.0, 0.5,
									1.0, 1.0, 0.0);
				}

				/*
					Second image, draw a rectangle
					that cuts through z=0.5.  If Z was
					cleared, all will be visible;
					if not, only the right half.
				*/
				if (loop == 2) {
					color.rgb.red = 1.0;
					color.rgb.green = 0.0;
					color.rgb.blue = 0.0;
					PEXSetSurfaceColor (display, renderer, PEXOCRender,
									PEXColorTypeRGB, &color);
					draw_primitive (display, renderer,PEXOCRender,
									FILLED_RECTANGLE,
									0.2, 0.2, 0.2,
									0.6, 0.6, 0.6);
				}
				break;

			case BACKGROUND_COLOR:
				/*
					No need to draw anything.  Second image 
					should be cleared to different color than first.
				*/
				break;

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

			case CLIPLIST:
				/*
					Draw a hollow rectangle centered in the window.
					The second cliplist is different than the first,
					so different parts of the rectangle will be
					visible, if cliplist is working (requires driver support).
					If dynamics are immediate, first image will show
					whole rectangle.

					The third image is not drawn; it shows that clearing
					also respects the clip list.
				*/
				if (loop < 2) {
					draw_primitive (display, renderer, PEXOCRender,
									RECTANGLE,
									0.2, 0.2, 0.0,
									0.6, 0.6, 0.0);
				}
				break;

			case NPC_SUBVOLUME:
				/*
					Draw a hollow rectangle at the NPC limits;
					shows NPC limits as NPC is maximized in
					the (square) viewport.
				*/
				draw_primitive (display, renderer, PEXOCRender,
								RECTANGLE,
								0.0, 0.0, 0.0,
								1.0, 1.0, 0.0);
				break;


			case VIEW_LUT:
			case VIEW_LUT_CONTENTS:
				/*
					Draw a hollow rectangle.  The orientation
					of the second image is different than the first.
				*/
				draw_primitive (display, renderer, PEXOCRender,
								RECTANGLE,
								0.2, 0.4, 0.0,
								0.6, 0.2, 0.0);
				break;

			case ECHO_MODE:
				/*
					Draw a line at the current y_offset.
					If the attribute has immediate dynamics,
					the second line should be different than the first.
					In any case, the third line should be different
					than the first.
				*/
				draw_primitive (display, renderer, PEXOCRender,
								LINE,
								x_offset, y_offset, z_offset,
								0.6, 0.0, 0.0);
				break;

			case COLOR_LUT:
			case COLOR_LUT_CONTENTS:
				color.indexed.index = 1;
				PEXSetLineColor (display, renderer, PEXOCRender,
								PEXColorTypeIndexed, &color);
				draw_primitive (display, renderer, PEXOCRender,
								LINE,
								x_offset, y_offset, z_offset,
								0.6, 0.0, 0.0);
				break;

			case DEPTH_CUE_LUT:
			case DEPTH_CUE_LUT_CONTENTS:
				color.rgb.red = 0.5;
				color.rgb.green = 0.5;
				color.rgb.blue = 0.5;
				PEXSetSurfaceColor (display, renderer, PEXOCRender,
								PEXColorTypeRGB, &color);
				draw_primitive (display, renderer, PEXOCRender,
								FILLED_RECTANGLE,
								x_offset, y_offset, 0.2,
								0.6, 0.15, 0.0);
				break;

			case COLOR_APPROX_LUT:
			case COLOR_APPROX_LUT_CONTENTS:
				color.rgb.red = 1.0;
				color.rgb.green = 0.0;
				color.rgb.blue = 0.0;
				PEXSetSurfaceColor (display, renderer, PEXOCRender,
								PEXColorTypeRGB, &color);
				PEXSetSurfaceEdgeFlag (display, renderer, PEXOCRender,
								PEXOn);
				color.rgb.red = 0.0;
				color.rgb.green = 1.0;
				color.rgb.blue = 1.0;
				PEXSetSurfaceEdgeColor (display, renderer, PEXOCRender,
								PEXColorTypeRGB, &color);
				draw_primitive (display, renderer, PEXOCRender,
								FILLED_RECTANGLE,
								x_offset, y_offset, 0.2,
								0.6, 0.15, 0.0);
				break;

			case LIGHT_LUT:
			case LIGHT_LUT_CONTENTS:
				{
					PEXTableIndex enable_list = 1;

					color.rgb.red = 0.5;
					color.rgb.green = 0.5;
					color.rgb.blue = 0.5;
					PEXSetSurfaceColor (display, renderer, PEXOCRender,
									PEXColorTypeRGB, &color);
					PEXSetSurfaceInterpMethod (display, renderer, PEXOCRender, 
										PEXSurfaceInterpColor);
					PEXSetReflectionModel (display, renderer, PEXOCRender, 
										PEXReflectionAmbient);
					PEXSetLightSourceState (display, renderer, PEXOCRender,
											1, &enable_list, 0, NULL);
					draw_primitive (display, renderer, PEXOCRender,
									FILLED_RECTANGLE,
									x_offset, y_offset, z_offset,
									0.6, 0.15, 0.0);
				}
				break;

			case LINE_LUT:
			case LINE_LUT_CONTENTS:
				for (i=0; i<NUM_LINE_ASFS; i++)
					PEXSetIndividualASF (display, renderer, PEXOCRender,
										line_asfs[i], PEXBundled);
				draw_primitive (display, renderer, PEXOCRender,
								LINE,
								x_offset, y_offset, z_offset,
								0.6, 0.0, 0.0);
				break;

			case MARKER_LUT:
			case MARKER_LUT_CONTENTS:
				for (i=0; i<NUM_MARKER_ASFS; i++)
					PEXSetIndividualASF (display, renderer, PEXOCRender,
										marker_asfs[i], PEXBundled);
				draw_primitive (display, renderer, PEXOCRender,
								MARKER,
								x_offset, y_offset, z_offset,
								0.6, 0.0, 0.0);
				break;

			case INTERIOR_LUT:
			case INTERIOR_LUT_CONTENTS:
				for (i=0; i<NUM_INTERIOR_ASFS; i++)
					PEXSetIndividualASF (display, renderer, PEXOCRender,
										interior_asfs[i], PEXBundled);
				draw_primitive (display, renderer, PEXOCRender,
								FILLED_RECTANGLE,
								x_offset, y_offset, z_offset,
								0.6, 0.15, 0.0);
				break;

			case EDGE_LUT:
			case EDGE_LUT_CONTENTS:
				PEXSetInteriorStyle (display, renderer, PEXOCRender,
									PEXInteriorStyleEmpty);
				for (i=0; i<NUM_EDGE_ASFS; i++)
					PEXSetIndividualASF (display, renderer, PEXOCRender,
										edge_asfs[i], PEXBundled);
				draw_primitive (display, renderer, PEXOCRender,
								FILLED_RECTANGLE,
								x_offset, y_offset, z_offset,
								0.6, 0.15, 0.0);
				break;

			case HIGHLIGHT_INCL:
			case HIGHLIGHT_INCL_CONTENTS:
			case HIGHLIGHT_EXCL:
			case HIGHLIGHT_EXCL_CONTENTS:
			case INVISIBILITY_INCL:
			case INVISIBILITY_INCL_CONTENTS:
			case INVISIBILITY_EXCL:
			case INVISIBILITY_EXCL_CONTENTS:
				draw_primitive (display, renderer, PEXOCRender,
								LINE,
								x_offset, y_offset, z_offset,
								0.6, 0.0, 0.0);
				break;

			case PICK_INCL:
			case PICK_INCL_CONTENTS:
			case PICK_EXCL:
			case PICK_EXCL_CONTENTS:
				draw_primitive (display, renderer, PEXOCRender,
								LINE,
								x_offset, y_offset, z_offset,
								0.6, 0.0, 0.0);
				break;

			case PIPELINE_CONTEXT_ASF:
				PEXSetLineType (display, renderer, PEXOCRender,
						plcattr_values[0].line_type);
				/* fall through */
			case PIPELINE_CONTEXT:
			case PIPELINE_CONTEXT_ATTR:
			case PLC_NAMESET:
			case PLC_NAMESET_CONTENTS:
				draw_primitive (display, renderer, PEXOCRender,
								LINE,
								x_offset, y_offset, z_offset,
								0.6, 0.0, 0.0);
				break;

			case TEXT_LUT:
			case TEXT_LUT_CONTENTS:
				for (i=0; i<NUM_TEXT_ASFS; i++)
					PEXSetIndividualASF (display, renderer, PEXOCRender,
										text_asfs[i], PEXBundled);
				draw_primitive (display, renderer, PEXOCRender,
								TEXT,
								x_offset, y_offset, z_offset,
								0.0, 0.0, 0.0);
				break;

			case TEXT_FONT_LUT:
			case TEXT_FONT_LUT_CONTENTS:
				draw_primitive (display, renderer, PEXOCRender,
								TEXT,
								x_offset, y_offset, z_offset,
								0.0, 0.0, 0.0);
				break;

			}


			if (attr_control[attr].image_per_setting) {
				need_to_capture = loop_control[loop].capture_image;
			}
			else if (attr_control[attr].subframe_per_setting) {
				if (subframe == NUM_SUBFRAMES_PER_IMAGE) {
					need_to_capture = True;
				}
				else {
					subframe++;
					need_to_capture = False;
				}
			}
			else {
				x_offset += x_increment;
				y_offset += y_increment;
				z_offset += z_increment;
				need_to_capture = False;
			}

			if (need_to_capture) {
				XSync (display, 0);
#ifdef XSYSTEM
				XSystemCheck(image_msg, &xtc);
				printf ("\n");
#else
				{
					char image_name[80], test_name[80];
					
					sprintf(image_name,"rnd_dynam_%d", image_number);
					sprintf(test_name, "rnd_dynam #%d", image_number);
					 image_testproc(image_name, test_name, img_desc,
									  display, window, 0, 0,
									  WIDTH, HEIGHT);
				}
#endif
				image_number++;
				need_to_capture = False;
				need_to_clear = !attr_control[attr].suppress_clear_after_image;
				subframe = 0;
				x_offset = 0.2;
				y_offset = 0.2;
				z_offset = 0.5;
			}

			if (loop_control[loop].unbind) {
				/*
					Unbind Renderer from the window.
				*/
				PEXEndRendering(display, renderer, True);
			}
		}

		if (!attr_control[attr].image_per_setting) {
			if (attr_control[attr].subframe_per_setting ||
				attr_control[attr].last_subframe_in_image ||
				(subframe == NUM_SUBFRAMES_PER_IMAGE)) {
					XSync (display, 0);
#ifdef XSYSTEM
					XSystemCheck(image_msg, &xtc);
					printf ("\n");
#else
					{
						char image_name[80], test_name[80];
						
						sprintf(image_name,"rnd_dynam_%d", image_number);
						sprintf(test_name, "rnd_dynam #%d", image_number);
						 image_testproc(image_name, test_name, img_desc,
										  display, window, 0, 0,
										  WIDTH, HEIGHT);
					}
#endif
					image_number++;
					need_to_clear = !attr_control[attr].suppress_clear_after_image;
					subframe = 0;
			}
			else
				subframe++;

			x_offset = 0.2;
			y_offset = 0.2;
			z_offset = 0.5;
		}


		/*
			Release any resources that were allocated for this attribute.
			See the matching code at the beginning of the attribute loop	
			for the allocations.
		*/
		switch (attr) {

		case VIEW_LUT:
		case COLOR_LUT:
		case DEPTH_CUE_LUT:
		case LIGHT_LUT:
		case COLOR_APPROX_LUT:
		case LINE_LUT:
		case MARKER_LUT:
		case INTERIOR_LUT:
		case EDGE_LUT:
		case TEXT_LUT:
			PEXFreeLookupTable (display, lut[1]);
			/* fall through */
		case VIEW_LUT_CONTENTS:
		case COLOR_LUT_CONTENTS:
		case DEPTH_CUE_LUT_CONTENTS:
		case LIGHT_LUT_CONTENTS:
		case COLOR_APPROX_LUT_CONTENTS:
		case LINE_LUT_CONTENTS:
		case MARKER_LUT_CONTENTS:
		case INTERIOR_LUT_CONTENTS:
		case EDGE_LUT_CONTENTS:
		case TEXT_LUT_CONTENTS:
			PEXFreeLookupTable (display, lut[0]);
			break;

		case TEXT_FONT_LUT:
			PEXFreeLookupTable (display, lut[1]);
			/* fall through */
		case TEXT_FONT_LUT_CONTENTS:
			PEXFreeLookupTable (display, lut[0]);

			PEXUnloadFont (display, font[0]);
			PEXUnloadFont (display, font[1]);
			break;

		case HIGHLIGHT_INCL:
		case HIGHLIGHT_EXCL:
		case INVISIBILITY_INCL:
		case INVISIBILITY_EXCL:
		case PICK_INCL:
		case PICK_EXCL:
			PEXFreeNameSet (display, nms[1]);
			/* fall through */
		case HIGHLIGHT_INCL_CONTENTS:
		case HIGHLIGHT_EXCL_CONTENTS:
		case INVISIBILITY_INCL_CONTENTS:
		case INVISIBILITY_EXCL_CONTENTS:
		case PICK_INCL_CONTENTS:
		case PICK_EXCL_CONTENTS:
			PEXFreeNameSet (display, nms[0]);
			PEXFreeNameSet (display, aux_nms[0]);
			break;

		case PIPELINE_CONTEXT:
			PEXFreePipelineContext (display, plc[1]);
		case PIPELINE_CONTEXT_ATTR:
			PEXFreePipelineContext (display, plc[0]);
			break;

		case PIPELINE_CONTEXT_ASF:
			PEXFreePipelineContext (display, plc[0]);
			PEXFreeLookupTable (display, aux_lut);
			break;

		case PLC_NAMESET:
			PEXFreeNameSet (display, nms[1]);
		case PLC_NAMESET_CONTENTS:
			PEXFreeNameSet (display, nms[0]);
			PEXFreePipelineContext (display, plc[0]);
			break;

		} /* switch on attr to release resources */

	} /* for each attribute */

		
	/*
		Close Renderer, window, and X/PEX.
	*/
	PEXFreeRenderer(display, renderer);

	XDestroyWindow(display, window);

	
    _hppex_close_stderr();

#ifdef XSYSTEM
    closeXSystemTesting(&xtc);
#else
    file_testproc("rnd_dynam.stderr", "rnd_dynam");
	XCloseDisplay (display);
	exit(0);
#endif

} /* renderer attribute dynamics test */
