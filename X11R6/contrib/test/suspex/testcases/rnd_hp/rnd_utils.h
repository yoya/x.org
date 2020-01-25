/* $XConsortium: rnd_utils.h,v 1.0 93/11/22 12:39:00 rws Exp $ */

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

#ifndef RENDERER_TEST_UTILS_H
#define RENDERER_TEST_UTILS_H

#define EPSILON 1.0E-25
#define FP_EQUAL(val1, val2) (fabs((val1)-(val2)) < EPSILON)
#define MAX(a,b) (((a)>(b))?(a):(b))


#define NONRES_RENDERER_ATTRS_MASK (							\
									PEXRACurrentPath		|	\
									PEXRARendererState		|	\
									PEXRAHLHSRMode			|	\
									PEXRANPCSubVolume		|	\
									PEXRAViewport			|	\
									PEXRAClipList			|	\
									PEXRAPickStartPath		|	\
									PEXRABackgroundColor	|	\
									PEXRAClearImage			|	\
									PEXRAClearZ				|	\
									PEXRAEchoMode			 	\
									)

#define RES_RENDERER_ATTRS_MASK (								\
									PEXRAPipelineContext	|	\
									PEXRAHighlightIncl		|	\
									PEXRAHighlightExcl		|	\
									PEXRAInvisibilityIncl	|	\
									PEXRAInvisibilityExcl	|	\
									PEXRAPickIncl			|	\
									PEXRAPickExcl			|	\
									PEXRAMarkerBundle		|	\
									PEXRATextBundle			|	\
									PEXRALineBundle			|	\
									PEXRAInteriorBundle		|	\
									PEXRAEdgeBundle			|	\
									PEXRAViewTable			|	\
									PEXRAColorTable			|	\
									PEXRADepthCueTable		|	\
									PEXRALightTable			|	\
									PEXRAColorApproxTable	|	\
									PEXRAPatternTable		|	\
									PEXRATextFontTable		 	\
									)

#define ALL_RENDERER_ATTRS_MASK (								\
								NONRES_RENDERER_ATTRS_MASK 	|	\
								RES_RENDERER_ATTRS_MASK			\
								)

extern
int cspec_equal(PEXColorSpecifier *cspec1, 
				PEXColorSpecifier *cspec2);

extern
void dump_cspec (char *prefix, PEXColorSpecifier *cspec);

extern
int check_renderer_attrs (	
						Display *display,
						PEXRenderer rnd,
						PEXRendererAttributes *expected,
						unsigned long value_mask );

extern
void init_default_renderer_attrs (PEXRendererAttributes *rnd_attrs);

extern
void set_renderer_clear_flags (
								Display *display, 
								PEXRenderer renderer,
								int enable_clear,
								float red, float green, float blue);


#define LINE				0
#define MARKER				1
#define RECTANGLE 			2
#define FILLED_RECTANGLE 	3
#define TEXT				4

extern
void draw_primitive (
					Display *display, 
					XID target_resource,
					PEXOCRequestType oc_target,
					int prim_type, 
					float x_offset, float y_offset, float z_offset,
					float x_extent, float y_extent, float z_extent);

extern
int prompt_user (char *prompt_message, char *responses);

#endif /* RENDERER_TEST_UTILS_H */
