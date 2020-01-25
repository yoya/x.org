/* $XConsortium: rnd_utils.c,v 1.0 93/11/22 12:40:06 rws Exp $ */

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
#include <string.h>
#include <math.h>

#include <X11/PEX5/PEXlib.h>

#include "rnd_utils.h"


int cspec_equal(PEXColorSpecifier *cspec1, 
				PEXColorSpecifier *cspec2)
{
	int match;

	match = cspec1->type == cspec2->type;

	if (match) {

		switch (cspec1->type) {

		case PEXColorTypeIndexed:
		match = match && 
				(cspec1->value.indexed.index == cspec2->value.indexed.index);
		break;

		case PEXColorTypeRGB:
		match = match && 
				FP_EQUAL(cspec1->value.rgb.red, cspec2->value.rgb.red) &&
				FP_EQUAL(cspec1->value.rgb.green, cspec2->value.rgb.green) &&
				FP_EQUAL(cspec1->value.rgb.blue, cspec2->value.rgb.blue);
		break;

		case PEXColorTypeCIE:
		match = match && 
				FP_EQUAL(cspec1->value.cie.x, cspec2->value.cie.x) &&
				FP_EQUAL(cspec1->value.cie.y, cspec2->value.cie.y) &&
				FP_EQUAL(cspec1->value.cie.z, cspec2->value.cie.z);
		break;

		case PEXColorTypeHSV:
		match = match && 
				FP_EQUAL(cspec1->value.hsv.hue, cspec2->value.hsv.hue) &&
				FP_EQUAL(cspec1->value.hsv.saturation, cspec2->value.hsv.saturation) &&
				FP_EQUAL(cspec1->value.hsv.value, cspec2->value.hsv.value);
		break;

		case PEXColorTypeHLS:
		match = match && 
				FP_EQUAL(cspec1->value.hls.hue, cspec2->value.hls.hue) &&
				FP_EQUAL(cspec1->value.hls.lightness, cspec2->value.hls.lightness) &&
				FP_EQUAL(cspec1->value.hls.saturation, cspec2->value.hls.saturation);
		break;

		case PEXColorTypeRGB8:
		match = match && 
				(cspec1->value.rgb8.red == cspec2->value.rgb8.red) &&
				(cspec1->value.rgb8.green == cspec2->value.rgb8.green) &&
				(cspec1->value.rgb8.blue == cspec2->value.rgb8.blue);
		break;

		case PEXColorTypeRGB16:
		match = match && 
				(cspec1->value.rgb16.red == cspec2->value.rgb16.red) &&
				(cspec1->value.rgb16.green == cspec2->value.rgb16.green) &&
				(cspec1->value.rgb16.blue == cspec2->value.rgb16.blue);
		break;

		default:
		match = False;
		break;
		}
	}

	return match;

} /* cspec_equal */


void dump_cspec (char *prefix, PEXColorSpecifier *cspec) 
{

	fprintf (stderr, "%s type = %d\n", 
					prefix, 
					cspec->type);


	switch (cspec->type) {

	case PEXColorTypeIndexed:
	fprintf (stderr, "%s index = %d\n", 
					prefix, 
					cspec->value.indexed.index);
	break;

	case PEXColorTypeRGB:
	fprintf (stderr, "%s rgb.red = %f, rgb.green = %f, rgb.blue = %f\n", 
					prefix, 
					cspec->value.rgb.red,
					cspec->value.rgb.green,
					cspec->value.rgb.blue);
	break;

	case PEXColorTypeCIE:
	fprintf (stderr, "%s cie.x = %f, cie.y = %f, cie.z = %f\n", 
					prefix, 
					cspec->value.cie.x,
					cspec->value.cie.y,
					cspec->value.cie.z);
	break;

	case PEXColorTypeHSV:
	fprintf (stderr, "%s hsv.hue = %f, hsv.saturation = %f, hsv.value = %f\n", 
					prefix, 
					cspec->value.hsv.hue,
					cspec->value.hsv.saturation,
					cspec->value.hsv.value);
	break;

	case PEXColorTypeHLS:
	fprintf (stderr, "%s hls.hue = %f, hls.lightness = %f, hls.saturation = %f\n", 
					prefix, 
					cspec->value.hls.hue,
					cspec->value.hls.lightness,
					cspec->value.hls.saturation);
	break;

	case PEXColorTypeRGB8:
	fprintf (stderr, "%s rgb8.red = %d, rgb8.green = %d, rgb8.blue = %d\n", 
					prefix, 
					cspec->value.rgb8.red,
					cspec->value.rgb8.green,
					cspec->value.rgb8.blue);
	break;

	case PEXColorTypeRGB16:
	fprintf (stderr, "%s rgb16.red = %d, rgb16.green = %d, rgb16.blue = %d\n", 
					prefix, 
					cspec->value.rgb16.red,
					cspec->value.rgb16.green,
					cspec->value.rgb16.blue);
	break;

	default:
	fprintf (stderr, "%s unrecognized color type\n", prefix);
	break;
	}

} /* dump_cspec */


int check_renderer_attrs (	
						Display *display,
						PEXRenderer rnd,
						PEXRendererAttributes *expected,
						unsigned long value_mask )
{
	PEXRendererAttributes *actual;
	unsigned long mask;
	int shift;
	int match, i;
	int return_val = 0;

	actual = PEXGetRendererAttributes (display, rnd, value_mask);

	shift = 0;
	mask = value_mask;
	while (mask) {

		if (mask & 1) {
			switch (shift) {

			case  0: /* Pipeline Context */

				if (actual->pipeline_context !=
					expected->pipeline_context) {

					return_val = 1;

					fprintf (stderr, "ERROR: actual Renderer attr does not match expected:\n");
					fprintf (stderr, "\tattribute:  pipeline context ID\n");
					fprintf (stderr, "\t\texpected:  0x%x\n", expected->pipeline_context);
					fprintf (stderr, "\t\tactual:  0x%x\n", actual->pipeline_context);
				}
				break;

			case  1: /* Current Path */

				match = (actual->current_path.count == expected->current_path.count);
				if (match) {
					for (i=0; i<actual->current_path.count; i++) {

						match = (match &&
								(actual->current_path.elements[i].structure ==
								expected->current_path.elements[i].structure) &&
								(actual->current_path.elements[i].offset ==
								expected->current_path.elements[i].offset));

						if (!match) break;
					}
				}

				if (!match) {

					return_val = 1;
					fprintf (stderr, "ERROR: actual Renderer attr does not match expected:\n");
					fprintf (stderr, "\tattribute:  current path\n");
					fprintf (stderr, "\t\texpected:  count = %d\n", expected->current_path.count);
					for (i=0; i<expected->current_path.count; i++) {
						fprintf (stderr, "\t\texpected:  elements[%d]: structure = 0x%x, offset = %d\n",
										i, expected->current_path.elements[i].structure,
										expected->current_path.elements[i].offset);
					}
					fprintf (stderr, "\t\tactual:  count = %d\n", actual->current_path.count);
					for (i=0; i<actual->current_path.count; i++) {
						fprintf (stderr, "\t\tactual:  elements[%d]: structure = 0x%x, offset = %d\n",
										i, actual->current_path.elements[i].structure,
										actual->current_path.elements[i].offset);
					}
				}
				break;

			case  2: /* Marker Bundle */

				if (actual->marker_bundle !=
					expected->marker_bundle) {

					return_val = 1;

					fprintf (stderr, "ERROR: actual Renderer attr does not match expected:\n");
					fprintf (stderr, "\tattribute:  marker bundle ID\n");
					fprintf (stderr, "\t\texpected:  0x%x\n", expected->marker_bundle);
					fprintf (stderr, "\t\tactual:  0x%x\n", actual->marker_bundle);
				}
				break;

			case  3: /* Text Bundle */

				if (actual->text_bundle !=
					expected->text_bundle) {

					return_val = 1;

					fprintf (stderr, "ERROR: actual Renderer attr does not match expected:\n");
					fprintf (stderr, "\tattribute:  text bundle ID\n");
					fprintf (stderr, "\t\texpected:  0x%x\n", expected->text_bundle);
					fprintf (stderr, "\t\tactual:  0x%x\n", actual->text_bundle);
				}
				break;

			case  4: /* Line Bundle */

				if (actual->line_bundle !=
					expected->line_bundle) {

					return_val = 1;

					fprintf (stderr, "ERROR: actual Renderer attr does not match expected:\n");
					fprintf (stderr, "\tattribute:  line bundle ID\n");
					fprintf (stderr, "\t\texpected:  0x%x\n", expected->line_bundle);
					fprintf (stderr, "\t\tactual:  0x%x\n", actual->line_bundle);
				}
				break;

			case  5: /* Interior Bundle */

				if (actual->interior_bundle !=
					expected->interior_bundle) {

					return_val = 1;

					fprintf (stderr, "ERROR: actual Renderer attr does not match expected:\n");
					fprintf (stderr, "\tattribute:  interior bundle ID\n");
					fprintf (stderr, "\t\texpected:  0x%x\n", expected->interior_bundle);
					fprintf (stderr, "\t\tactual:  0x%x\n", actual->interior_bundle);
				}
				break;

			case  6: /* Edge Bundle */

				if (actual->edge_bundle !=
					expected->edge_bundle) {

					return_val = 1;

					fprintf (stderr, "ERROR: actual Renderer attr does not match expected:\n");
					fprintf (stderr, "\tattribute:  edge bundle ID\n");
					fprintf (stderr, "\t\texpected:  0x%x\n", expected->edge_bundle);
					fprintf (stderr, "\t\tactual:  0x%x\n", actual->edge_bundle);
				}
				break;

			case  7: /* View Table */

				if (actual->view_table !=
					expected->view_table) {

					return_val = 1;

					fprintf (stderr, "ERROR: actual Renderer attr does not match expected:\n");
					fprintf (stderr, "\tattribute:  view table ID\n");
					fprintf (stderr, "\t\texpected:  0x%x\n", expected->view_table);
					fprintf (stderr, "\t\tactual:  0x%x\n", actual->view_table);
				}
				break;

			case  8: /* Color Table */

				if (actual->color_table !=
					expected->color_table) {

					return_val = 1;

					fprintf (stderr, "ERROR: actual Renderer attr does not match expected:\n");
					fprintf (stderr, "\tattribute:  color table ID\n");
					fprintf (stderr, "\t\texpected:  0x%x\n", expected->color_table);
					fprintf (stderr, "\t\tactual:  0x%x\n", actual->color_table);
				}
				break;

			case  9: /* Depth Cue Table */

				if (actual->depth_cue_table !=
					expected->depth_cue_table) {

					return_val = 1;

					fprintf (stderr, "ERROR: actual Renderer attr does not match expected:\n");
					fprintf (stderr, "\tattribute:  depth cue table ID\n");
					fprintf (stderr, "\t\texpected:  0x%x\n", expected->depth_cue_table);
					fprintf (stderr, "\t\tactual:  0x%x\n", actual->depth_cue_table);
				}
				break;

			case 10: /* Light Table */

				if (actual->light_table !=
					expected->light_table) {

					return_val = 1;

					fprintf (stderr, "ERROR: actual Renderer attr does not match expected:\n");
					fprintf (stderr, "\tattribute:  light table ID\n");
					fprintf (stderr, "\t\texpected:  0x%x\n", expected->light_table);
					fprintf (stderr, "\t\tactual:  0x%x\n", actual->light_table);
				}
				break;

			case 11: /* Color Approximation Table */

				if (actual->color_approx_table !=
					expected->color_approx_table) {

					return_val = 1;

					fprintf (stderr, "ERROR: actual Renderer attr does not match expected:\n");
					fprintf (stderr, "\tattribute:  color approximation table ID\n");
					fprintf (stderr, "\t\texpected:  0x%x\n", expected->color_approx_table);
					fprintf (stderr, "\t\tactual:  0x%x\n", actual->color_approx_table);
				}
				break;

			case 12: /* Pattern Table */

				if (actual->pattern_table !=
					expected->pattern_table) {

					return_val = 1;

					fprintf (stderr, "ERROR: actual Renderer attr does not match expected:\n");
					fprintf (stderr, "\tattribute:  pattern table ID\n");
					fprintf (stderr, "\t\texpected:  0x%x\n", expected->pattern_table);
					fprintf (stderr, "\t\tactual:  0x%x\n", actual->pattern_table);
				}
				break;

			case 13: /* Text Font Table */

				if (actual->text_font_table !=
					expected->text_font_table) {

					return_val = 1;

					fprintf (stderr, "ERROR: actual Renderer attr does not match expected:\n");
					fprintf (stderr, "\tattribute:  text font table ID\n");
					fprintf (stderr, "\t\texpected:  0x%x\n", expected->text_font_table);
					fprintf (stderr, "\t\tactual:  0x%x\n", actual->text_font_table);
				}
				break;

			case 14: /* Highlight Inclusion */

				if (actual->highlight_incl !=
					expected->highlight_incl) {

					return_val = 1;

					fprintf (stderr, "ERROR: actual Renderer attr does not match expected:\n");
					fprintf (stderr, "\tattribute:  highlight inclusion nameset ID\n");
					fprintf (stderr, "\t\texpected:  0x%x\n", expected->highlight_incl);
					fprintf (stderr, "\t\tactual:  0x%x\n", actual->highlight_incl);
				}
				break;

			case 15: /* Highlight Exclusion */

				if (actual->highlight_excl !=
					expected->highlight_excl) {

					return_val = 1;

					fprintf (stderr, "ERROR: actual Renderer attr does not match expected:\n");
					fprintf (stderr, "\tattribute:  highlight exclusion nameset ID\n");
					fprintf (stderr, "\t\texpected:  0x%x\n", expected->highlight_excl);
					fprintf (stderr, "\t\tactual:  0x%x\n", actual->highlight_excl);
				}
				break;

			case 16: /* Invisibility Inclusion */

				if (actual->invisibility_incl !=
					expected->invisibility_incl) {

					return_val = 1;

					fprintf (stderr, "ERROR: actual Renderer attr does not match expected:\n");
					fprintf (stderr, "\tattribute:  invisibility inclusion nameset ID\n");
					fprintf (stderr, "\t\texpected:  0x%x\n", expected->invisibility_incl);
					fprintf (stderr, "\t\tactual:  0x%x\n", actual->invisibility_incl);
				}
				break;

			case 17: /* Invisibility Exclusion */

				if (actual->invisibility_excl !=
					expected->invisibility_excl) {

					return_val = 1;

					fprintf (stderr, "ERROR: actual Renderer attr does not match expected:\n");
					fprintf (stderr, "\tattribute:  invisibility exclusion nameset ID\n");
					fprintf (stderr, "\t\texpected:  0x%x\n", expected->invisibility_excl);
					fprintf (stderr, "\t\tactual:  0x%x\n", actual->invisibility_excl);
				}
				break;

			case 18: /* Renderer State */

				if (actual->renderer_state !=
					expected->renderer_state) {

					return_val = 1;

					fprintf (stderr, "ERROR: actual Renderer attr does not match expected:\n");
					fprintf (stderr, "\tattribute:  renderer state\n");
					fprintf (stderr, "\t\texpected:  %d\n", expected->renderer_state);
					fprintf (stderr, "\t\tactual:  %d\n", actual->renderer_state);
				}
				break;

			case 19: /* HLHSR Mode */

				if (actual->hlhsr_mode !=
					expected->hlhsr_mode) {

					return_val = 1;

					fprintf (stderr, "ERROR: actual Renderer attr does not match expected:\n");
					fprintf (stderr, "\tattribute:  HLHSR mode\n");
					fprintf (stderr, "\t\texpected:  %d\n", expected->hlhsr_mode);
					fprintf (stderr, "\t\tactual:  %d\n", actual->hlhsr_mode);
				}
				break;

			case 20: /* NPC Subvolume */

				match = (FP_EQUAL(actual->npc_subvolume.min.x, expected->npc_subvolume.min.x) && 
						FP_EQUAL(actual->npc_subvolume.min.y, expected->npc_subvolume.min.y) && 
						FP_EQUAL(actual->npc_subvolume.min.z, expected->npc_subvolume.min.z) && 
						FP_EQUAL(actual->npc_subvolume.max.x, expected->npc_subvolume.max.x) && 
						FP_EQUAL(actual->npc_subvolume.max.y, expected->npc_subvolume.max.y) && 
						FP_EQUAL(actual->npc_subvolume.max.z, expected->npc_subvolume.max.z));

				if (!match) {

					return_val = 1;

					fprintf (stderr, "ERROR: actual Renderer attr does not match expected:\n");
					fprintf (stderr, "\tattribute:  NPC subvolume\n");
					fprintf (stderr, "\t\texpected:  min = (%f, %f, %f), max = (%f, %f, %f)\n", 
									expected->npc_subvolume.min.x,
									expected->npc_subvolume.min.y,
									expected->npc_subvolume.min.z,
									expected->npc_subvolume.max.x,
									expected->npc_subvolume.max.y,
									expected->npc_subvolume.max.z);
					fprintf (stderr, "\t\tactual:  min = (%f, %f, %f), max = (%f, %f, %f)\n", 
									actual->npc_subvolume.min.x,
									actual->npc_subvolume.min.y,
									actual->npc_subvolume.min.z,
									actual->npc_subvolume.max.x,
									actual->npc_subvolume.max.y,
									actual->npc_subvolume.max.z);
				}
				break;

			case 21: /* Viewport */

				match = ((actual->viewport.min.x == expected->viewport.min.x) &&
						(actual->viewport.min.y == expected->viewport.min.y) &&
						FP_EQUAL(actual->viewport.min.z, expected->viewport.min.z) &&
						(actual->viewport.max.x == expected->viewport.max.x) &&
						(actual->viewport.max.y == expected->viewport.max.y) &&
						FP_EQUAL(actual->viewport.max.z, expected->viewport.max.z) &&
						(actual->viewport.use_drawable == expected->viewport.use_drawable));

				if (!match) {

					return_val = 1;

					fprintf (stderr, "ERROR: actual Renderer attr does not match expected:\n");
					fprintf (stderr, "\tattribute:  viewport\n");
					fprintf (stderr, "\t\texpected:  use_drawable = %d, min = (%d, %d, %f), max = (%d, %d, %f)\n", 
									expected->viewport.use_drawable,
									expected->viewport.min.x,
									expected->viewport.min.y,
									expected->viewport.min.z,
									expected->viewport.max.x,
									expected->viewport.max.y,
									expected->viewport.max.z);
					fprintf (stderr, "\t\tactual:  use_drawable = %d, min = (%d, %d, %f), max = (%d, %d, %f)\n", 
									actual->viewport.use_drawable,
									actual->viewport.min.x,
									actual->viewport.min.y,
									actual->viewport.min.z,
									actual->viewport.max.x,
									actual->viewport.max.y,
									actual->viewport.max.z);
				}
				break;

			case 22: /* Clip List */

				match = (actual->clip_list.count == expected->clip_list.count);
				if (match) {
					for (i=0; i<(int)actual->clip_list.count; i++) {

						match = (match &&
								(actual->clip_list.rectangles[i].xmin ==
								expected->clip_list.rectangles[i].xmin) &&
								(actual->clip_list.rectangles[i].xmax ==
								expected->clip_list.rectangles[i].xmax) &&
								(actual->clip_list.rectangles[i].ymin ==
								expected->clip_list.rectangles[i].ymin) &&
								(actual->clip_list.rectangles[i].ymax ==
								expected->clip_list.rectangles[i].ymax));

						if (!match) break;
					}
				}

				if (!match) {

					return_val = 1;

					fprintf (stderr, "ERROR: actual Renderer attr does not match expected:\n");
					fprintf (stderr, "\tattribute:  clip list\n");
					fprintf (stderr, "\t\texpected:  count = %d\n", expected->clip_list.count);
					for (i=0; i<(int)expected->clip_list.count; i++) {
						fprintf (stderr, "\t\texpected:  rectangles[%d]: xmin = %d, xmax = %d, ymin = %d, ymax = %d\n",
										i, expected->clip_list.rectangles[i].xmin,
										expected->clip_list.rectangles[i].xmax,
										expected->clip_list.rectangles[i].ymin,
										expected->clip_list.rectangles[i].ymax);
					}
					fprintf (stderr, "\t\tactual:  count = %d\n", actual->clip_list.count);
					for (i=0; i<(int)actual->clip_list.count; i++) {
						fprintf (stderr, "\t\tactual:  rectangles[%d]: xmin = %d, xmax = %d, ymin = %d, ymax = %d\n",
										i, actual->clip_list.rectangles[i].xmin,
										actual->clip_list.rectangles[i].xmax,
										actual->clip_list.rectangles[i].ymin,
										actual->clip_list.rectangles[i].ymax);
					}
				}
				break;

			case 23: /* Pick Inclusion */

				if (actual->pick_incl !=
					expected->pick_incl) {

					return_val = 1;

					fprintf (stderr, "ERROR: actual Renderer attr does not match expected:\n");
					fprintf (stderr, "\tattribute:  pick inclusion nameset ID\n");
					fprintf (stderr, "\t\texpected:  0x%x\n", expected->pick_incl);
					fprintf (stderr, "\t\tactual:  0x%x\n", actual->pick_incl);
				}
				break;

			case 24: /* Pick Exclusion */

				if (actual->pick_excl !=
					expected->pick_excl) {

					return_val = 1;

					fprintf (stderr, "ERROR: actual Renderer attr does not match expected:\n");
					fprintf (stderr, "\tattribute:  pick exclusion nameset ID\n");
					fprintf (stderr, "\t\texpected:  0x%x\n", expected->pick_excl);
					fprintf (stderr, "\t\tactual:  0x%x\n", actual->pick_excl);
				}
				break;

			case 25: /* Pick Start Path */

				match = (actual->pick_start_path.count == expected->pick_start_path.count);
				if (match) {
					for (i=0; i<actual->pick_start_path.count; i++) {

						match = (match &&
								(actual->pick_start_path.elements[i].structure ==
								expected->pick_start_path.elements[i].structure) &&
								(actual->pick_start_path.elements[i].offset ==
								expected->pick_start_path.elements[i].offset));

						if (!match) break;
					}
				}

				if (!match) {

					return_val = 1;
					fprintf (stderr, "ERROR: actual Renderer attr does not match expected:\n");
					fprintf (stderr, "\tattribute:  pick start path\n");
					fprintf (stderr, "\t\texpected:  count = %d\n", expected->pick_start_path.count);
					for (i=0; i<expected->pick_start_path.count; i++) {
						fprintf (stderr, "\t\texpected:  elements[%d]: structure = 0x%x, offset = %d\n",
										i, expected->pick_start_path.elements[i].structure,
										expected->pick_start_path.elements[i].offset);
					}
					fprintf (stderr, "\t\tactual:  count = %d\n", actual->pick_start_path.count);
					for (i=0; i<actual->pick_start_path.count; i++) {
						fprintf (stderr, "\t\tactual:  elements[%d]: structure = 0x%x, offset = %d\n",
										i, actual->pick_start_path.elements[i].structure,
										actual->pick_start_path.elements[i].offset);
					}
				}
				break;

			case 26: /* Background Color */

				match = cspec_equal(&(actual->background_color), &(expected->background_color)); 

				if (!match) {

					return_val = 1;

					fprintf (stderr, "ERROR: actual Renderer attr does not match expected:\n");
					fprintf (stderr, "\tattribute:  background color\n");
					dump_cspec("\t\texpected:  ", &(expected->background_color));
					dump_cspec("\t\tactual:  ", &(actual->background_color));
				}
				break;

			case 27: /* Clear Image */

				if (actual->clear_image !=
					expected->clear_image) {

					return_val = 1;

					fprintf (stderr, "ERROR: actual Renderer attr does not match expected:\n");
					fprintf (stderr, "\tattribute:  clear image flag\n");
					fprintf (stderr, "\t\texpected:  0x%x\n", expected->clear_image);
					fprintf (stderr, "\t\tactual:  0x%x\n", actual->clear_image);
				}
				break;

			case 28: /* Clear Z */

				if (actual->clear_z !=
					expected->clear_z) {

					return_val = 1;

					fprintf (stderr, "ERROR: actual Renderer attr does not match expected:\n");
					fprintf (stderr, "\tattribute:  clear z flag\n");
					fprintf (stderr, "\t\texpected:  0x%x\n", expected->clear_z);
					fprintf (stderr, "\t\tactual:  0x%x\n", actual->clear_z);
				}
				break;

			case 29: /* Echo Mode */

				if (actual->echo_mode !=
					expected->echo_mode) {

					return_val = 1;

					fprintf (stderr, "ERROR: actual Renderer attr does not match expected:\n");
					fprintf (stderr, "\tattribute:  echo mode\n");
					fprintf (stderr, "\t\texpected:  %d\n", expected->echo_mode);
					fprintf (stderr, "\t\tactual:  %d\n", actual->echo_mode);
				}
				break;
			}
		}

		mask >>= 1;
		shift++;
	}

	PEXFreeRendererAttributes (actual);

	return return_val;

} /* check_renderer_attrs */

void init_default_renderer_attrs (PEXRendererAttributes *rnd_attrs)
{
	memset (rnd_attrs, 0, sizeof(PEXRendererAttributes));
	rnd_attrs->renderer_state= PEXIdle;
	rnd_attrs->clear_z = True;
	rnd_attrs->hlhsr_mode = PEXHLHSROff;
	rnd_attrs->npc_subvolume.max.x = 1.0;
	rnd_attrs->npc_subvolume.max.y = 1.0;
	rnd_attrs->npc_subvolume.max.z = 1.0;
	rnd_attrs->viewport.use_drawable = True;
	rnd_attrs->background_color.type = PEXColorTypeIndexed;
	rnd_attrs->background_color.value.indexed.index = 0;

	/* These six values are implementation-dependent. */
	rnd_attrs->viewport.min.x = 0;
	rnd_attrs->viewport.min.y = 0;
	rnd_attrs->viewport.min.z = 0.0;
	rnd_attrs->viewport.max.x = 400;
	rnd_attrs->viewport.max.y = 400;
	rnd_attrs->viewport.max.z = 1.0;

} /* init_default_renderer_attrs */


void set_renderer_clear_flags (	Display *display, 
								PEXRenderer renderer,
								int enable_clear,
								float red, float green, float blue)
{
	PEXRendererAttributes set_attrs;

	/*
		Set up Renderer to clear image and Z planes.
	*/
	set_attrs.background_color.type = PEXColorTypeRGB;
	set_attrs.background_color.value.rgb.red = red;
	set_attrs.background_color.value.rgb.green = green;
	set_attrs.background_color.value.rgb.blue = blue;

	set_attrs.clear_image = enable_clear;
	set_attrs.clear_z = enable_clear;
	set_attrs.hlhsr_mode = (enable_clear? PEXHLHSRZBuffer: PEXHLHSROff);

	PEXChangeRenderer (display, renderer, 
						PEXRAClearImage|
						PEXRABackgroundColor|
						PEXRAHLHSRMode |
						PEXRAClearZ,
						&set_attrs);

} /* set_renderer_clear_flags */


void draw_primitive (
					Display *display, 
					XID target_resource,
					PEXOCRequestType oc_target,
					int prim_type, 
					float x_offset, float y_offset, float z_offset,
					float x_extent, float y_extent, float z_extent)
{
	PEXCoord points[5];
	PEXVector base_vect, up_vect;

	points[0].x = x_offset;
	points[0].y = y_offset;
	points[0].z = z_offset;

	points[1].x = x_offset + x_extent;
	points[1].y = y_offset;
	points[1].z = z_offset + z_extent;

	points[2].x = x_offset + x_extent;
	points[2].y = y_offset + y_extent;
	points[2].z = z_offset + z_extent;

	points[3].x = x_offset;
	points[3].y = y_offset + y_extent;
	points[3].z = z_offset;

	points[4].x = x_offset;
	points[4].y = y_offset;
	points[4].z = z_offset;

	base_vect.x = 1.0;
	base_vect.y = 0.0;
	base_vect.z = 0.0;

	up_vect.x = 0.0;
	up_vect.y = 1.0;
	up_vect.z = 0.0;

	switch (prim_type) {

	case LINE:
		points[1] = points[2];
		PEXPolyline (display, target_resource, oc_target,
					2, points);
		break;

	case MARKER:
		points[1] = points[2];
		PEXMarkers (display, target_resource, oc_target,
					2, points);
		break;

	case RECTANGLE:
		PEXPolyline (display, target_resource, oc_target,
					5, points);
		break;

	case FILLED_RECTANGLE:
		PEXFillArea (display, target_resource, oc_target,
					PEXShapeUnknown, False, 5, points);
		break;

	case TEXT:
		points[1] = points[2];
		PEXText (display, target_resource, oc_target,
				points, &base_vect, &up_vect, 4, "TEXT");
		break;

	}
} /* draw_primitive */

int prompt_user (char *prompt_message, char *responses)
{
	char answer[256], *ptr;

	printf ("%s:  ", prompt_message);
	fflush (stdout);

	if (gets(answer)) {

		ptr = strchr (responses, answer[0]);
		if (ptr)
			return (ptr - responses);
		else
			return -1;
	}
	else
		return -1;

} /* prompt_user */
