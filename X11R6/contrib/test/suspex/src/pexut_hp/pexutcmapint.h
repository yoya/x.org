/* $XConsortium: pexutcmapint.h,v 1.0 93/11/22 12:23:54 rws Exp $ */

#ifndef PEXUTCMAP_IH /* { */
#define PEXUTCMAP_IH

/******************************************************************************/
/*                                                                            */
/*  (c) Copyright Hewlett-Packard Company, 1992, Fort Collins, Colorado       */
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

/******************************************************************************/
/*                                                                            */
/* $Source: /xsrc/xc/unsupported/test/suspex/src/pexut_hp/RCS/pexutcmapint.h,v $                      	      */
/* $Date: 93/11/22 12:23:54 $                                                 */
/* $Revision: 1.0 $                                                   */
/*                                                                            */
/* Description:                                                               */
/*   Internal header file for PEXUt cmap/visual utilities,                    */
/*   not to be included by application programs.                              */
/*                                                                            */
/* Notes:                                                                     */
/*                                                                            */
/******************************************************************************/


#ifndef NeedFunctionPrototypes
#if defined(FUNCPROTO) || defined(__STDC__) || \
    defined(__cplusplus) || defined(c_plusplus)
#define NeedFunctionPrototypes  1
#else
#define NeedFunctionPrototypes  0
#endif /* FUNCPROTO, __STDC__, __cplusplus, c_plusplus */
#endif /* NeedFunctionPrototypes */


#ifdef __cplusplus   /* do not leave open across includes */
extern "C" {         /* for C++ V2.0 */
#endif


/*
    This structure type is used to describe standard colormap properties
    that should searched as part of an interoperability convention.
    The ordering of entries in an array of these structures is the search
    order of the properties.

    The fields are used as follows:

	is_standard		True if the colormap property is a current
				standard, i.e. supported by XGetRGBColormaps.
				False otherwise.

	have_atom		True if the Atom in the property_atom field
				is valid, False if only the property_name
				string is valid.

	property_atom		Atom value for the property name, which may
				be predefined or may have been fetched by
				an earlier call to XInternAtom.

	property_name		the property name as a NULL-terminated
				character string.
*/
	
typedef struct {
	int	is_standard;
	int	have_atom;
	Atom	property_atom;
	char	*property_name;
} interop_property_type;


/*
    This structure type is the entry type for the SERVER_OVERLAY_VISUALS
    property.  The property contains an entry for each Visual on the screen
    of the root window where it is attached.
    
    The fields are used as follows:

	visualid		identifier of the Visual.

	transparent_type	specifies the mechanism that controls
				transparency in the Visual: one of None,
				TransparentPixel, or TransparentMask.

	value			pixel value that is transparent, if
				the transparent_type is TransparentPixel.

	layer			specifies the layer of the Visual.  Image
				layer is 0, overlay layer is 1.
*/
	
typedef struct {
	VisualID	visualid;
	int		transparent_type;
	int		value;
	int		layer;
} overlay_visuals_type;



/*
    This structure type is used internally by the PEXUt code to record
    information about each Visual that is considered.  The central
    creator and user of the structure type is PEXUtFindVisual; it passes
    pointers to this structure type to several subsidiary routines.
    
    The fields are used as follows:

	unmet_hard_criteria	a mask of the hard criteria that are not
				met by the Visual; if this is mask is non-zero,
				the Visual is not a candidate for selection.

	unmet_soft_criteria	a mask of the soft criteria that are not
				met by the Visual; this mask is used in
				choosing among several Visuals that are
				candidates.

	property_ptr		a pointer to the entry for the Visual in the 
				first standard colormap property (in priority)
				that contains an entry for the Visual; if 
				properties were not searched then this pointer
				is NULL.

	property_atom		the atom for the property whose entry is
				addressed by property_ptr.

	visual_ptr		a pointer to the description of the Visual.

	order_rating,		independent integers that represent the
	soft_rating,		relative rank of the Visual in the respective
	color_rating		category:  order of appearance in a target
	class_rating		list or colormap property; number of soft
				criteria that the Visual meets; level of
				color resolution; preference for Visual class.  
				These fields are combined to form an overall 
				rating in order to choose among several candidate Visuals.
*/

typedef struct {
	unsigned int		unmet_hard_criteria;
	unsigned int		unmet_soft_criteria;
	XStandardColormap	*property_ptr;
	Atom			property_atom;
	XVisualInfo		*visual_ptr;
	unsigned int		order_rating;
	unsigned int		soft_rating;
	unsigned int		color_rating;
	unsigned int		class_rating;
} visual_info_type;


/*
    MAX_ORDER_RATING is the maximum value of the order rating, i.e. the rating 
    for each candidate visual that is based on its relative order in a
    target list or standard colormap property.  The value range should
    map to a range compatible with the ORDER_SHIFT,
    such that the range and the shift do not exceed 32 bits when combined.

    Similarly, MAX_SOFT_RATING is the maximum value for the rating based
    on how well a Visual meets the soft criteria.  It must be compatible
    with SOFT_SHIFT.  And so on for the other factors used in rating Visuals.
*/

#define MAX_CLASS_RATING  0x1
#define CLASS_SHIFT       0
#define MAX_COLOR_RATING  0xFFFF
#define COLOR_SHIFT       1
#define MAX_ORDER_RATING  0x1F
#define ORDER_SHIFT       17
#define MAX_SOFT_RATING   0x1F
#define SOFT_SHIFT        22


/*
    No specifications are provided for these internal utilities.
    For the most part, their names describe their functionality.
*/

extern
int pexut_get_standard_cmap_property (
#if NeedFunctionPrototypes
		Display				*display,
		int				screen,
		interop_property_type		*property_info,
		Atom				*property_atom_return,
		int				*property_count_return,
		XStandardColormap		**property_data_return
#endif
		);
		
extern
int pexut_get_overlay_visuals_property (
#if NeedFunctionPrototypes
		Display				*display,
		int				screen,
		int				*property_count_return,
		overlay_visuals_type		**property_data_return
#endif
		);
		
extern
int pexut_evaluate_color_criteria (
#if NeedFunctionPrototypes
		Display				*display,
		PEXUtVisualCriteria		*criteria,
		XVisualInfo			*vis_info,
		visual_info_type		*p_info
#endif
		);

extern
int pexut_evaluate_double_buffering_capability (
#if NeedFunctionPrototypes
		Display				*display,
		PEXUtVisualCriteria		*criteria,
		XVisualInfo			*vis_info,
		visual_info_type		*p_info
#endif
		);

extern
int pexut_evaluate_color_approx_type (
#if NeedFunctionPrototypes
		Display				*display,
		PEXUtVisualCriteria		*criteria,
		XVisualInfo			*vis_info,
		visual_info_type		*p_info
#endif
		);

extern
unsigned int pexut_compute_overall_rating (
#if NeedFunctionPrototypes
		visual_info_type		*p_info
#endif
		);

extern
int pexut_create_temporary_window (
#if NeedFunctionPrototypes
		Display				*display,
		XVisualInfo			*vis_info,
		Window				*window_return
#endif
		);

extern
int pexut_destroy_temporary_window (
#if NeedFunctionPrototypes
		Display				*display,
		XVisualInfo			*vis_info,
		Window				window
#endif
		);

extern
int pexut_verify_color_approx_type (
#if NeedFunctionPrototypes
		Display				*display,
		Window				window,
		PEXEnumTypeIndex		color_approx_type
#endif
		);

extern
int pexut_synthesize_cmap_from_visual (
#if NeedFunctionPrototypes
		XVisualInfo			*vis_info,
		XStandardColormap		*cmap_info_return
#endif
		);

extern
int pexut_load_color_approx_from_std_cmap (
#if NeedFunctionPrototypes
		int				color_approx_type,
		XVisualInfo			*vis_info,
		XStandardColormap		*cmap_info,
		PEXColorApproxEntry		*capx_info_return
#endif
		);

extern
int pexut_match_color_approx_entry (
#if NeedFunctionPrototypes
		PEXColorApproxEntry		*entry1, 
		PEXColorApproxEntry		*entry2
#endif
		);

extern
int pexut_create_one_channel_map (
#if NeedFunctionPrototypes
		Display				*display,
		XVisualInfo			*vis_info,
		PEXColorApproxEntry		*capx_info,
		Colormap			*colormap_id_return
#endif
		);

extern
int pexut_create_three_channel_map (
#if NeedFunctionPrototypes
		Display				*display,
		XVisualInfo			*vis_info,
		PEXColorApproxEntry		*capx_info,
		Colormap			*colormap_id_return
#endif
		);

extern
int pexut_create_read_only_map (
#if NeedFunctionPrototypes
		Display				*display,
		XVisualInfo			*vis_info,
		PEXColorApproxEntry		*capx_info,
		Colormap			*colormap_id_return
#endif
		);

extern
int pexut_count_bits_in_mask (
#if NeedFunctionPrototypes
		unsigned int mask
#endif
		);

#ifdef __cplusplus
}                    /* for C++ V2.0 */
#endif

#endif /* } PEXUTCMAP_IH */
