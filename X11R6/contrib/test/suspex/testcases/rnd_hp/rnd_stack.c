/* $XConsortium: rnd_stack.c,v 1.0 93/11/22 12:39:54 rws Exp $ */

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
** Renderer attribute stack test
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


/* structure type used for test scripts below */
typedef struct {
	int 				opcode;
	char				*message;
	PEXEnumTypeIndex 	line_type;
	unsigned long		client_structure_id;
	int					hierarchy;
	int					mixture;
	int					expected_error_type;
	int					expected_error_code;
	int					traverse_to_bottom;
	int					is_bottom;
} pex_operation;

/* opcodes */
#define BEGIN_RENDERING 	0
#define END_RENDERING 		1
#define BEGIN_STRUCTURE 	2
#define END_STRUCTURE 		3
#define RENDER_NETWORK 		4
#define RENDER_ELEMENTS 	5
#define ACCUMULATE_STATE 	6
#define DRAW_LINE 			7
#define LINE_TYPE 			8
#define OC_SET 				9
#define EXECUTE_STRUCTURE	10
#define END_SEQUENCE		-1

/* structure hierarchy levels */
#define LEVEL_0		0
#define LEVEL_1		1
#define LEVEL_2		2

/* these control building of structures from OC set */
#define NESTED			0
#define FLAT			1
#define MAX_HIERARCHY	2

#define	MIXED			0
#define	ATTRS_ONLY		1
#define	PRIMS_ONLY		2
#define	MAX_MIXTURE		3

/* the main sequence of attrs, prims, and push/pops */
static pex_operation oc_set[]={
	{ LINE_TYPE,	
		NULL, PEXLineTypeSolid, 0, 0, 0, NO_ERR, 0, 0, False },
	{ DRAW_LINE,
		"solid", 0, 0, 0, 0, NO_ERR, 0, 0, False },
	{ BEGIN_STRUCTURE,	0, LEVEL_1,
		NULL, 0, 0, NO_ERR, 0, 0, False },
	{ LINE_TYPE,	
		NULL, PEXLineTypeDashed, 0, 0, 0, NO_ERR, 0, 0, False },
	{ DRAW_LINE,
		"dashed", 0, 0, 0, 0, NO_ERR, 0, 0, False },
	{ BEGIN_STRUCTURE, 0,	LEVEL_2,
		NULL, 0, 0, NO_ERR, 0, 0, False },
	{ LINE_TYPE,	
		NULL, PEXLineTypeDotted, 0, 0, 0, NO_ERR, 0, 0, False },
	{ DRAW_LINE,
		"dotted", 0, 0, 0, 0, NO_ERR, 0, 0, True },
	{ END_STRUCTURE,
		NULL, 0, 0, 0, 0, NO_ERR, 0, 0, False },
	{ DRAW_LINE,
		"dashed", 0, 0, 0, 0, NO_ERR, 0, 0, False },
	{ END_STRUCTURE,
		NULL, 0, 0, 0, 0, NO_ERR, 0, 0, False },
	{ DRAW_LINE,
		"solid", 0, 0, 0, 0, NO_ERR, 0, 0, False },
	{ END_SEQUENCE,
		NULL, 0, 0, 0, 0, NO_ERR, 0, 0, False },
};

/* push/pop hierarchy not expected to be deeper than this */
#define MAX_DEPTH	5

/* this is total count of prims drawn in a traversal of the OC set */
#define NUM_STR_PRIMS	5


static pex_operation test_scripts[]={

	{ BEGIN_RENDERING,
		"Immediate mode sequence--standard image",
		0, 0, 0, 0, NO_ERR, 0, 0, False },
	{ OC_SET,
		NULL, 0, 0, 0, 0, NO_ERR, 0, False, False },
	{ END_RENDERING,
		NULL, 0, 0, 0, 0, NO_ERR, 0, 0, False },
	{ END_SEQUENCE,
		NULL, 0, 0, 0, 0, NO_ERR, 0, 0, False },

	{ BEGIN_RENDERING,
		"Immediate mode with EndRendering while stack non-empty--standard image",
		0, 0, 0, 0, NO_ERR, 0, 0, False },
	{ OC_SET,
		NULL, 0, 0, 0, 0, NO_ERR, 0, True, False },
	{ END_RENDERING,
		NULL, 0, 0, 0, 0, NO_ERR, 0, 0, False },
	{ END_SEQUENCE,
		NULL, 0, 0, 0, 0, NO_ERR, 0, 0, False },

	{ BEGIN_RENDERING,
		"Stack underflow in immediate mode--standard image",
		0, 0, 0, 0, NO_ERR, 0, 0, False },
	{ OC_SET,
		NULL, 0, 0, 0, 0, NO_ERR, 0, False, False },
	{ END_STRUCTURE,
		NULL, 0, 0, 0, 0, PEX_ERR, BadPEXRendererState, 0, False },
	{ END_RENDERING,
		NULL, 0, 0, 0, 0, NO_ERR, 0, 0, False },
	{ END_SEQUENCE,
		NULL, 0, 0, 0, 0, NO_ERR, 0, 0, False },

	{ BEGIN_RENDERING,
		"Immediate mode with extra level of push/pop--standard image",
		0, 0, 0, 0, NO_ERR, 0, 0, False },
	{ BEGIN_STRUCTURE,	0, LEVEL_0,
		NULL, 0, 0, NO_ERR, 0, 0, False },
	{ OC_SET,
		NULL, 0, 0, 0, 0, NO_ERR, 0, False, False },
	{ END_STRUCTURE,
		NULL, 0, 0, 0, 0, NO_ERR, 0, 0, False },
	{ END_RENDERING,
		NULL, 0, 0, 0, 0, NO_ERR, 0, 0, False },
	{ END_SEQUENCE,
		NULL, 0, 0, 0, 0, NO_ERR, 0, 0, False },

	{ BEGIN_RENDERING,
		"Immediate mode sequence sent to Idle Renderer--no primitives should be drawn",
		0, 0, 0, 0, NO_ERR, 0, 0, False },
	{ END_RENDERING,
		NULL, 0, 0, 0, 0, NO_ERR, 0, 0, False },
	{ OC_SET,
		NULL, 0, 0, 0, 0, NO_ERR, 0, False, False },
	{ END_SEQUENCE,
		NULL, 0, 0, 0, 0, NO_ERR, 0, 0, False },

	{ RENDER_NETWORK,
		"Traveral mode sequence (RenderNetwork)--standard image but no description printed",
		0, 0, NESTED, MIXED, NO_ERR, 0, 0, False },
	{ END_SEQUENCE,
		NULL, 0, 0, 0, 0, NO_ERR, 0, 0, False },

	{ BEGIN_RENDERING,
		"Mixed mode sequence (ExecuteStructure)--extra prims before and after traversal",
		0, 0, 0, 0, NO_ERR, 0, 0, False },
	{ LINE_TYPE,	
		NULL, PEXLineTypeSolid, 0, 0, 0, NO_ERR, 0, 0, False },
	{ DRAW_LINE,
		"solid", 0, 0, 0, 0, NO_ERR, 0, 0, False },
	{ LINE_TYPE,	
		NULL, PEXLineTypeDashed, 0, 0, 0, NO_ERR, 0, 0, False },
	{ DRAW_LINE,
		"dashed", 0, 0, 0, 0, NO_ERR, 0, 0, False },
	{ EXECUTE_STRUCTURE,
		"(execute structure, standard image)", 0, 0, NESTED, MIXED, NO_ERR, 0, 0, False },
	{ DRAW_LINE,
		"dashed", 0, 0, 0, 0, NO_ERR, 0, 0, False },
	{ LINE_TYPE,	
		NULL, PEXLineTypeSolid, 0, 0, 0, NO_ERR, 0, 0, False },
	{ DRAW_LINE,
		"solid", 0, 0, 0, 0, NO_ERR, 0, 0, False },
	{ END_RENDERING,
		NULL, 0, 0, 0, 0, NO_ERR, 0, 0, False },
	{ END_SEQUENCE,
		NULL, 0, 0, 0, 0, NO_ERR, 0, 0, False },

	{ BEGIN_RENDERING,
		"AccumulateState on mixed path, extra prims before and after traversal",
		0, 0, 0, 0, NO_ERR, 0, 0, False },
	{ LINE_TYPE,	
		NULL, PEXLineTypeDashed, 0, 0, 0, NO_ERR, 0, 0, False },
	{ DRAW_LINE,
		"dashed", 0, 0, 0, 0, NO_ERR, 0, 0, False },
	{ LINE_TYPE,	
		NULL, PEXLineTypeSolid, 0, 0, 0, NO_ERR, 0, 0, False },
	{ DRAW_LINE,
		"solid", 0, 0, 0, 0, NO_ERR, 0, 0, False },
	{ ACCUMULATE_STATE,
		"(accumulate state, attribute setting but no prims)", 
		0, 0, NESTED, MIXED, NO_ERR, 0, 0, False },
	{ DRAW_LINE,
		"dotted", 0, 0, 0, 0, NO_ERR, 0, 0, False },
	{ LINE_TYPE,	
		NULL, PEXLineTypeSolid, 0, 0, 0, NO_ERR, 0, 0, False },
	{ DRAW_LINE,
		"solid", 0, 0, 0, 0, NO_ERR, 0, 0, False },
	{ END_STRUCTURE,
		NULL, 0, 0, 0, 0, PEX_ERR, BadPEXRendererState, 0, False },
	{ END_RENDERING,
		NULL, 0, 0, 0, 0, NO_ERR, 0, 0, False },
	{ END_SEQUENCE,
		NULL, 0, 0, 0, 0, NO_ERR, 0, 0, False },

	{ BEGIN_RENDERING,
		"AccumulateState on attr-only path, extra prims before and after traversal",
		0, 0, 0, 0, NO_ERR, 0, 0, False },
	{ LINE_TYPE,	
		NULL, PEXLineTypeDashed, 0, 0, 0, NO_ERR, 0, 0, False },
	{ DRAW_LINE,
		"dashed", 0, 0, 0, 0, NO_ERR, 0, 0, False },
	{ LINE_TYPE,	
		NULL, PEXLineTypeSolid, 0, 0, 0, NO_ERR, 0, 0, False },
	{ DRAW_LINE,
		"solid", 0, 0, 0, 0, NO_ERR, 0, 0, False },
	{ ACCUMULATE_STATE,
		"(accumulate state, attribute setting but no prims)", 
		0, 0, NESTED, ATTRS_ONLY, NO_ERR, 0, 0, False },
	{ DRAW_LINE,
		"dotted", 0, 0, 0, 0, NO_ERR, 0, 0, False },
	{ LINE_TYPE,	
		NULL, PEXLineTypeSolid, 0, 0, 0, NO_ERR, 0, 0, False },
	{ DRAW_LINE,
		"solid", 0, 0, 0, 0, NO_ERR, 0, 0, False },
	{ END_RENDERING,
		NULL, 0, 0, 0, 0, NO_ERR, 0, 0, False },
	{ END_SEQUENCE,
		NULL, 0, 0, 0, 0, NO_ERR, 0, 0, False },

	{ BEGIN_RENDERING,
		"AccumulateState on primitive-only path, extra prims before and after traversal",
		0, 0, 0, 0, NO_ERR, 0, 0, False },
	{ LINE_TYPE,	
		NULL, PEXLineTypeSolid, 0, 0, 0, NO_ERR, 0, 0, False },
	{ DRAW_LINE,
		"solid", 0, 0, 0, 0, NO_ERR, 0, 0, False },
	{ LINE_TYPE,	
		NULL, PEXLineTypeDashed, 0, 0, 0, NO_ERR, 0, 0, False },
	{ DRAW_LINE,
		"dashed", 0, 0, 0, 0, NO_ERR, 0, 0, False },
	{ ACCUMULATE_STATE,
		"(accumulate state, no attribute changes and no prims)", 
		0, 0, NESTED, PRIMS_ONLY, NO_ERR, 0, 0, False },
	{ DRAW_LINE,
		"dashed", 0, 0, 0, 0, NO_ERR, 0, 0, False },
	{ LINE_TYPE,	
		NULL, PEXLineTypeSolid, 0, 0, 0, NO_ERR, 0, 0, False },
	{ DRAW_LINE,
		"solid", 0, 0, 0, 0, NO_ERR, 0, 0, False },
	{ END_RENDERING,
		NULL, 0, 0, 0, 0, NO_ERR, 0, 0, False },
	{ END_SEQUENCE,
		NULL, 0, 0, 0, 0, NO_ERR, 0, 0, False },

	{ BEGIN_RENDERING,
		"RenderElements on flat, mixed path, extra prims before and after range traversal",
		0, 0, 0, 0, NO_ERR, 0, 0, False },
	{ LINE_TYPE,	
		NULL, PEXLineTypeDashed, 0, 0, 0, NO_ERR, 0, 0, False },
	{ DRAW_LINE,
		"dashed", 0, 0, 0, 0, NO_ERR, 0, 0, False },
	{ LINE_TYPE,	
		NULL, PEXLineTypeSolid, 0, 0, 0, NO_ERR, 0, 0, False },
	{ DRAW_LINE,
		"solid", 0, 0, 0, 0, NO_ERR, 0, 0, False },
	{ RENDER_ELEMENTS,
		"(render elements, no pops so last attribute remains in effect)", 
		0, 0, FLAT, MIXED, NO_ERR, 0, 0, False },
	{ DRAW_LINE,
		"dotted", 0, 0, 0, 0, NO_ERR, 0, 0, False },
	{ LINE_TYPE,	
		NULL, PEXLineTypeSolid, 0, 0, 0, NO_ERR, 0, 0, False },
	{ DRAW_LINE,
		"solid", 0, 0, 0, 0, NO_ERR, 0, 0, False },
	{ END_STRUCTURE,
		NULL, 0, 0, 0, 0, PEX_ERR, BadPEXRendererState, 0, False },
	{ END_RENDERING,
		NULL, 0, 0, 0, 0, NO_ERR, 0, 0, False },
	{ END_SEQUENCE,
		NULL, 0, 0, 0, 0, NO_ERR, 0, 0, False },

	{ BEGIN_RENDERING,
		"RenderElements on flat, attr-only path, extra prims before and after range traversal",
		0, 0, 0, 0, NO_ERR, 0, 0, False },
	{ LINE_TYPE,	
		NULL, PEXLineTypeDashed, 0, 0, 0, NO_ERR, 0, 0, False },
	{ DRAW_LINE,
		"dashed", 0, 0, 0, 0, NO_ERR, 0, 0, False },
	{ LINE_TYPE,	
		NULL, PEXLineTypeSolid, 0, 0, 0, NO_ERR, 0, 0, False },
	{ DRAW_LINE,
		"solid", 0, 0, 0, 0, NO_ERR, 0, 0, False },
	{ RENDER_ELEMENTS,
		"(render elements, no prims, no pops so last attribute remains in effect)", 
		0, 0, FLAT, ATTRS_ONLY, NO_ERR, 0, 0, False },
	{ DRAW_LINE,
		"dotted", 0, 0, 0, 0, NO_ERR, 0, 0, False },
	{ LINE_TYPE,	
		NULL, PEXLineTypeSolid, 0, 0, 0, NO_ERR, 0, 0, False },
	{ DRAW_LINE,
		"solid", 0, 0, 0, 0, NO_ERR, 0, 0, False },
	{ END_RENDERING,
		NULL, 0, 0, 0, 0, NO_ERR, 0, 0, False },
	{ END_SEQUENCE,
		NULL, 0, 0, 0, 0, NO_ERR, 0, 0, False },

	{ BEGIN_RENDERING,
		"RenderElements on flat, primitive-only path, extra prims before and after range traversal",
		0, 0, 0, 0, NO_ERR, 0, 0, False },
	{ LINE_TYPE,	
		NULL, PEXLineTypeSolid, 0, 0, 0, NO_ERR, 0, 0, False },
	{ DRAW_LINE,
		"solid", 0, 0, 0, 0, NO_ERR, 0, 0, False },
	{ LINE_TYPE,	
		NULL, PEXLineTypeDashed, 0, 0, 0, NO_ERR, 0, 0, False },
	{ DRAW_LINE,
		"dashed", 0, 0, 0, 0, NO_ERR, 0, 0, False },
	{ RENDER_ELEMENTS,
		"(render elements, no attrs so prims have last imm. mode attribute)", 
		0, 0, FLAT, PRIMS_ONLY, NO_ERR, 0, 0, False },
	{ DRAW_LINE,
		"dashed", 0, 0, 0, 0, NO_ERR, 0, 0, False },
	{ LINE_TYPE,	
		NULL, PEXLineTypeSolid, 0, 0, 0, NO_ERR, 0, 0, False },
	{ DRAW_LINE,
		"solid", 0, 0, 0, 0, NO_ERR, 0, 0, False },
	{ END_RENDERING,
		NULL, 0, 0, 0, 0, NO_ERR, 0, 0, False },
	{ END_SEQUENCE,
		NULL, 0, 0, 0, 0, NO_ERR, 0, 0, False },

	{ END_SEQUENCE,
		NULL, 0, 0, 0, 0, NO_ERR, 0, 0, False },
};

static char *image_msgs[]={
		"ERROR: Immediate mode sequence",
		"ERROR: Immediate mode with EndRendering while stack non-empty",
		"ERROR: Stack underflow in immediate mode",
		"ERROR: Immediate mode with extra level of push/pop",
		"ERROR: Immediate mode sequence sent to Idle Renderer",
		"ERROR: Traveral mode sequence (RenderNetwork)",
		"ERROR: Mixed mode sequence (ExecuteStructure)",
		"ERROR: AccumulateState on mixed path",
		"ERROR: AccumulateState on attr-only path",
		"ERROR: AccumulateState on primitive-only path",
		"ERROR: RenderElements on flat, mixed path",
		"ERROR: RenderElements on flat, attr-only path",
		"ERROR: RenderElements on flat, primitive-only path",
};


static PEXStructure structure_ids[MAX_HIERARCHY][MAX_MIXTURE];
static PEXStructure extra_structure_ids[MAX_HIERARCHY][MAX_MIXTURE][MAX_DEPTH];
static extra_str_counts[MAX_HIERARCHY][MAX_MIXTURE];

void build_structures (Display *display,
						PEXCoord *points,
						float y_increment)
{
	int hierarchy, mixture, depth;
	pex_operation *script;
	PEXStructure str_id_stack[MAX_DEPTH];
	PEXStructure current_structure;

	float initial_y_offset;
	float y_offset;

	initial_y_offset = points[0].y;

	depth = 0;
	for (hierarchy = 0; hierarchy < MAX_HIERARCHY; hierarchy++) {
		for (mixture = 0; mixture < MAX_MIXTURE; mixture++) {

			current_structure = PEXCreateStructure (display);
			structure_ids[hierarchy][mixture] = current_structure;
			extra_str_counts[hierarchy][mixture] = 0;

			y_offset = initial_y_offset;

			script = oc_set;
			while (script->opcode != END_SEQUENCE) {

				switch (script->opcode) {

				case LINE_TYPE:
				if (mixture != PRIMS_ONLY)
					PEXSetLineType (display, current_structure, PEXOCStore,
									script->line_type);
				break;

				case DRAW_LINE:
				if (mixture != ATTRS_ONLY) {
					points[0].y = y_offset;
					points[1].y = y_offset;
					y_offset += y_increment;
					PEXPolyline (display, current_structure, PEXOCStore,
									2, points);
				}
				break;

				case BEGIN_STRUCTURE:
				if (hierarchy != FLAT) {
					PEXStructure new_structure;

					str_id_stack[depth++] = current_structure;
					new_structure = PEXCreateStructure (display);
					extra_structure_ids[hierarchy][mixture][
									(extra_str_counts[hierarchy][mixture])++]
						= new_structure;
					PEXExecuteStructure (display, current_structure, PEXOCStore, new_structure);
					current_structure = new_structure;
				}
				break;

				case END_STRUCTURE:
				if (hierarchy != FLAT)
					current_structure = str_id_stack[--depth];
				break;
				} /* switch */

				script++;

			} /* while */
		} /* mixture */
	} /* hierarchy */

} /* build_structures */

void free_structures (Display *display)
{
	int hierarchy, mixture, index;

	for (hierarchy = 0; hierarchy < MAX_HIERARCHY; hierarchy++) {
		for (mixture = 0; mixture < MAX_MIXTURE; mixture++) {
			PEXDestroyStructures (display, 1, 
									&(structure_ids[hierarchy][mixture]));
			for (index = 0; 
				index < extra_str_counts[hierarchy][mixture]; 
				index++)

				PEXDestroyStructures (display, 1, 
					&(extra_structure_ids[hierarchy][mixture][index]));
		}
	}


} /* free_structures */


void build_path ( 	int hierarchy, 
					int mixture,
					PEXStructurePath *path)
{
	int index, extra_str_index, done;
	pex_operation *script;

	index = 0;
	path->elements[index].structure = structure_ids[hierarchy][mixture];
	path->elements[index].offset = 0;

	extra_str_index = 0;
	done = False;

	script = oc_set;
	while (!done) {

		switch (script->opcode) {

		case LINE_TYPE:
		if (mixture != PRIMS_ONLY)
			path->elements[index].offset++;
		break;

		case DRAW_LINE:
		if (mixture != ATTRS_ONLY)
			path->elements[index].offset++;
		break;

		case BEGIN_STRUCTURE:
		if (hierarchy != FLAT) {
			path->elements[index].offset++;
			index++;
			path->elements[index].structure = 
				extra_structure_ids[hierarchy][mixture][extra_str_index++];
			path->elements[index].offset = 0;
		}
		break;

		case END_STRUCTURE:
		case END_SEQUENCE:
		done = True;
		break;

		} /* switch */

		script++;

	} /* while */

	path->count = index + 1;

} /* build_path */


void interpret_script (	Display *display, 
						Window window,
						PEXRenderer renderer, 
						pex_operation **p_script,
						PEXCoord *points,
						float y_increment)
{
	int depth;
	pex_operation *script, *operation_stack[3];
	PEXStructurePath local_path;
	PEXElementRef elements[MAX_DEPTH];
	float y_offset;
	PEXVector trans_vector;
	PEXMatrix trans_matrix;

	local_path.elements = elements;
	script = *p_script;

	trans_vector.x = 0.0;
	trans_vector.z = 0.0;
	y_offset = points[0].y;

	depth = 0;
	while (True) {

#ifndef XSYSTEM
		if (glob_verbose_mode)
#endif
			if (script->message != NULL)
				printf ("  %s\n", script->message);

		_hppex_test_set_expected_error (script->expected_error_type, 
							script->expected_error_code, 0, 0, 0);

		switch (script->opcode) {

		case BEGIN_RENDERING:
		PEXBeginRendering (display, window, renderer);
		break;

		case END_RENDERING:
		PEXEndRendering (display, renderer, True);
		break;

		case BEGIN_STRUCTURE:
		PEXBeginStructure (display, renderer, script->client_structure_id);
		break;

		case END_STRUCTURE:
		PEXEndStructure (display, renderer);
		break;

		case RENDER_NETWORK:
		PEXRenderNetwork (display, window, renderer,
					structure_ids[script->hierarchy][script->mixture]);
		break;

		case RENDER_ELEMENTS:
		trans_vector.y = y_offset - y_increment;
		PEXTranslate (&trans_vector, trans_matrix);
		PEXSetLocalTransform (display, renderer, PEXOCRender,
							PEXReplace, trans_matrix);
		PEXRenderElements (display, renderer,
					structure_ids[script->hierarchy][script->mixture],
					PEXBeginning, 0, PEXEnd, 0);
		y_offset += NUM_STR_PRIMS * y_increment;
		trans_vector.y = 0.0;
		PEXTranslate (&trans_vector, trans_matrix);
		PEXSetLocalTransform (display, renderer, PEXOCRender,
							PEXReplace, trans_matrix);
		break;

		case ACCUMULATE_STATE:
		build_path (script->hierarchy, script->mixture, &local_path);
		trans_vector.y = y_offset - y_increment;
		PEXTranslate (&trans_vector, trans_matrix);
		PEXSetLocalTransform (display, renderer, PEXOCRender,
							PEXReplace, trans_matrix);
		PEXAccumulateState (display, renderer,
					local_path.count, local_path.elements);
		y_offset += NUM_STR_PRIMS * y_increment;
		trans_vector.y = 0.0;
		PEXTranslate (&trans_vector, trans_matrix);
		PEXSetLocalTransform (display, renderer, PEXOCRender,
							PEXReplace, trans_matrix);
		PEXSetGlobalTransform (display, renderer, PEXOCRender,
							trans_matrix);
		break;

		case LINE_TYPE:
		PEXSetLineType (display, renderer, PEXOCRender,
							script->line_type);
		break;

		case DRAW_LINE:
		points[0].y = y_offset;
		points[1].y = y_offset;
		y_offset += y_increment;
		PEXPolyline (display, renderer, PEXOCRender,
							2, points);
		break;

		case OC_SET:
		operation_stack[depth++] = script;
		script = oc_set;
		break;

		case EXECUTE_STRUCTURE:
		trans_vector.y = y_offset - y_increment;
		PEXTranslate (&trans_vector, trans_matrix);
		PEXSetLocalTransform (display, renderer, PEXOCRender,
							PEXReplace, trans_matrix);
		PEXExecuteStructure (display, renderer, PEXOCRender,
					structure_ids[script->hierarchy][script->mixture]);
		y_offset += NUM_STR_PRIMS * y_increment;
		trans_vector.y = 0.0;
		PEXTranslate (&trans_vector, trans_matrix);
		PEXSetLocalTransform (display, renderer, PEXOCRender,
							PEXReplace, trans_matrix);
		break;

		case END_SEQUENCE:
		if (depth > 0) {
			script = operation_stack[--depth];
			break;
		}
		else {
			*p_script = script;
			return;
		}

		} /* switch */

		XSync (display, 0);
		_hppex_test_check_expected_error(ERROR_CODE);

		script++;

	} /* while */

} /* interpret_script */


main(argc, argv)
int argc;
char **argv;
{
	Display *display;
	Window window;
	PEXRenderer renderer;
	PEXRendererAttributes set_attrs;
	unsigned long value_mask;
	PEXExtensionInfo *ext_info;

	XVisualInfo vis_info;
	XStandardColormap cmap_info;
	PEXColorApproxEntry capx_info;
	PEXElementRef current_path_elems;
	int screen;
	XSizeHints hints;

	int i, j, loop, subframe;
	int need_to_clear, need_to_capture;
	int image_number = 0;
	pex_operation *script;

	float x_offset, y_offset, z_offset;
	float x_increment, y_increment, z_increment;
	PEXCoord points[2];

    int			pexmode = 0;
	int			image_depth = 0;

#ifdef XSYSTEM
    int XSystemValues[20];
#endif

    /*  Handle window geometry string if present */
    parse_window_geometry(argc, argv);


#ifndef XSYSTEM
    /* Handle other arguments. */
    if (parse_options (argc, argv, &pexmode, &image_depth))
	exit (1);
#endif

#ifdef XSYSTEM
	openXSystemTesting(argc, argv, &xtc, 0, 0);
#endif

    _hppex_set_stderr("rnd_stack.stderr", "rnd_stack");

	describe_test("\n\n");
	describe_test("This is a test of attribute inheritance and push/pop in the PEXLib Renderer,\n");
	describe_test("for all of immediate mode, structure mode, and mixed mode.  This includes\n");
	describe_test("exercising the PEXBeginStructure/PEXEndStructure entrypoints, as well as\n");
	describe_test("PEXRenderElements and PEXAccumulateState.  A couple of the cases also test\n");
	describe_test("attribute stack underflow to make sure the proper error is reported\n\n");

	describe_test("This test generates images.  It operates by repeating basically the same\n");
	describe_test("sequence of Line Style and Polyline calls under various conditions.\n");
	describe_test("The particular conditions for each image are printed as it is generated.\n");
	describe_test("For normal and AccumulateState traversals, a nested structure hierarchy is\n");
	describe_test("traversed; for RenderElements, the same elements are put into a flat structure.\n\n");

#ifdef XSYSTEM
	if (xtc.interactiveFramePrompt)
#else
	if (glob_verbose_mode && getenv("PAUSE"))
#endif
		prompt_user ("Hit return to continue with the next page of instructions", "");

	describe_test("IT IS VERY IMPORTANT to know that the primitives are rendered progressively\n");
	describe_test("from the bottom of the window towards the top.  The standard appearance of\n");
	describe_test("each primitive under normal attribute inheritance is printed out during \n");
	describe_test("immediate mode (but cannot be during traversal, of course).  So, if the\n");
	describe_test("printed description is in the sequence solid, dashed, dotted, look for that\n");
	describe_test("sequence from the bottom up, not the top down.\n\n");

	describe_test("The test starts out with several occurrences of the standard sequence of\n");
	describe_test("attributes and primitives, so you can get familiar with them.  The conditions\n"); 
	describe_test("message may describe how the picture should be different from this standard.\n");
	describe_test("The AccumulateState and RenderElements images are more weird and require some\n");
	describe_test("close attention to understand.  Make sure you understand how the conditions \n");
	describe_test("affect the image before declaring it to be incorrect.\n\n");

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
							"PEXlib Renderer Attr Stack Test", 
							image_depth,
							glob_window_x, glob_window_y, 
							glob_window_width, glob_window_height,
							&capx_info);


	if (window == None) {

		_hppex_stderr_print( "PROCESS ERROR:  create_window failed\n");
		exit (1);
	}

#ifdef XSYSTEM
    XSystemValues[0] = WINDOW_SMART_HW_CONFIG;
    XSystemValues[1] = window;
    XSystemValues[2] = -1;
    defineXSystemTestControlParm(&xtc, ARCHIVE_MODE, XSystemValues);

    XSystemValues[0] = WINDOW_SMART_EXTENT;
    XSystemValues[1] = window;
    defineXSystemTestControlParm(&xtc, ARCHIVE_EXTENT, XSystemValues);
#endif

	/*
		Create a Renderer.
	*/

	setup_LUT (display, window, &capx_info);

	renderer = setup_renderer (display, window);
	if (renderer == None) {

		_hppex_stderr_print( "PROCESS ERROR: PEXCreateRenderer failed\n");
		exit (1);
	}

	/*
		Set up Renderer to clear image and Z planes.
	*/
	set_attrs.clear_image = True;
	set_attrs.clear_z = True;
	set_attrs.background_color.type = PEXColorTypeRGB;
	set_attrs.background_color.value.rgb.red = 0.0;
	set_attrs.background_color.value.rgb.green = 0.0;
	set_attrs.background_color.value.rgb.blue = 0.0;
	set_attrs.hlhsr_mode = PEXHLHSRZBuffer;
	value_mask = PEXRAClearImage|
				PEXRABackgroundColor|
				PEXRAHLHSRMode |
				PEXRAClearZ;
	PEXChangeRenderer (display, renderer, value_mask, &set_attrs);


	subframe = 0;
	need_to_capture = False;

	points[0].x = 0.2;
	points[0].z = 0.0;
	points[1].x = 0.8;
	points[1].z = 0.0;
	x_increment = 0.0;
	y_increment = 0.1;
	z_increment = 0.0;

	/*
		Build PEX Structures for traversal testing.
	*/
	points[0].y = 0.1;
	points[1].y = 0.1;
	build_structures (display, points, y_increment);

	script = test_scripts;
	while (script->opcode != END_SEQUENCE) {

		points[0].y = 0.1;
		points[1].y = 0.1;
		interpret_script ( display, window, renderer, 
							&script, points, y_increment );

		/* capture image */
		XSync (display, 0);
#ifdef XSYSTEM
		XSystemCheck(image_msgs[image_number], &xtc);
		printf ("\n");
#else
		{
			char image_name[80], test_name[80];
			
			sprintf(image_name,"rnd_stack_%d", image_number);
			sprintf(test_name, "rnd_stack #%d", image_number);
			 image_testproc(image_name, test_name, img_desc,
							  display, window, 0, 0,
							  glob_window_width, glob_window_height);
		}
#endif
		image_number++;

		script++;
#ifndef XSYSTEM
		if (glob_verbose_mode)
			printf ("\n\n");
#endif
	}


	/*
		Free all Structures that were allocated.
	*/
	free_structures(display);
		
	/*
		Close Renderer, window, and X/PEX.
	*/
	PEXFreeRenderer(display, renderer);

	XDestroyWindow(display, window);

    _hppex_close_stderr();

#ifdef XSYSTEM
    closeXSystemTesting(&xtc);
#else
    file_testproc("rnd_stack.stderr", "rnd_stack");
	XCloseDisplay (display);
	exit(0);
#endif

} /* renderer attribute stack test */


