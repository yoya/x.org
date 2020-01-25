/* $XConsortium: pexutcmap.c,v 1.0 93/11/22 12:23:22 rws Exp $ */

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
/* $Source: /xsrc/xc/unsupported/test/suspex/src/pexut_hp/RCS/pexutcmap.c,v $                            */
/* $Date: 93/11/22 12:23:22 $                                                 */
/* $Revision: 1.0 $                                                   */
/*                                                                            */
/* Description:                                                               */
/*   Main interface implementation file for PEXUt colormap/visual utilities.  */
/*                                                                            */
/* Notes:                                                                     */
/*                                                                            */
/******************************************************************************/


#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/PEX5/PEXlib.h>
#include "pexutext.h"

#include "pexutcmap.h"
#include "pexutcmapint.h"
#include "pexutcmaphp.h"


/*
    This structure describes each of the standard colormap 
    properties to be searched when choosing a Visual, in
    priority order.  Changing the interoperability convention
    should be a simple matter of adding or shuffling these entries.
*/

static interop_property_type interop_properties[]={
	{ True, True, XA_RGB_BEST_MAP, "" },
	{ True, True, XA_RGB_DEFAULT_MAP, "" },
};

#define NUM_INTEROP_PROPERTIES \
	(sizeof(interop_properties)/sizeof(interop_property_type))




int PEXUtFindVisual (
		display,
		screen,
		criteria,
		vis_info_return,
		cmap_info_return,
		capx_info_return,
		unmet_criteria_return,
		std_prop_atom_return
		)

		Display				*display;
		int				screen;
		PEXUtVisualCriteria		*criteria;
		XVisualInfo			*vis_info_return;
		XStandardColormap		*cmap_info_return;
		PEXColorApproxEntry		*capx_info_return;
		unsigned int			*unmet_criteria_return;
		Atom				*std_prop_atom_return;

/*
    Algorithm:

	Section I:    	Initialize and process input criteria masks.

	Section II:   	Fetch information from the server.

	Section III:  	Build information blocks about all the Visuals
			that are supported by PEX.

	Section IV:	Evaluate candidates (Visuals that meet hard criteria).

	Section V:	Copy or generate return information.
*/
{
	PEXUtVisualCriteria	local_criteria;

	PEXExtensionInfo	*ext_info;
	int			target_depth;
	unsigned long		target_count = 0;
	PEXRenderingTarget	*targets = NULL;

	XVisualInfo       	vis_template;
	unsigned int      	vis_mask;
	int			visual_count = 0;
	XVisualInfo		*visuals = NULL;

	int			property_counts[NUM_INTEROP_PROPERTIES];
	Atom			property_atoms[NUM_INTEROP_PROPERTIES];
	XStandardColormap	*property_data[NUM_INTEROP_PROPERTIES];

	int			overlay_count = 0;
	overlay_visuals_type	*overlay_data = NULL;

	int			visual_info_count = 0;
	visual_info_type	*visual_info = NULL;
	visual_info_type	*p_info; 

	int			met_hard_count;
	int			min_unmet_criteria_count;
	int			soft_criteria_count;
	int			supported_visual_count;
	visual_info_type	*p_chosen_info, *p_min_unmet;

	int			visual_index;
	int			target_index;
	int			prop_index;
	int			entry_index;

	int			result;
	int			color_approx_type;


	/*
	    This macro is used to clean up in case of an early exit.
	*/
#define FREE_ALLOCATED_RESOURCES {					\
	    int i;							\
	    if (target_count) XFree (targets);				\
	    if (visual_count > 0) XFree (visuals);			\
	    for (i=0; i<NUM_INTEROP_PROPERTIES; i++)			\
		if (property_counts[i] > 0) XFree (property_data[i]);	\
	    if (overlay_count > 0) XFree (overlay_data);		\
	    if (visual_info_count > 0) free (visual_info);		\
	}


    /*
    -----------------------------------------------------------------------
    Section I:    	Initialize and process input criteria masks.
    -----------------------------------------------------------------------
    */

	/*
	    Initialize property data arrays to empty.
	*/

	for (prop_index=0; prop_index<NUM_INTEROP_PROPERTIES; prop_index++) {

	    property_counts[prop_index] = 0;
	    property_data[prop_index] = NULL;
	    property_atoms[prop_index] = None;
	}


	/*
	    Copy the caller's criteria structure and clean up the masks
	    (eliminate unsupported bits and overlaps).
	*/

	local_criteria = *criteria;
	local_criteria.hard_criteria_mask &= PEXUtAllCriteria;
	local_criteria.soft_criteria_mask &= PEXUtAllCriteria;
	local_criteria.soft_criteria_mask &= ~(local_criteria.hard_criteria_mask);

    /*
    -----------------------------------------------------------------------
    Section II:   	Fetch information from the server.
    -----------------------------------------------------------------------
    */

	/*
	    Get PEX extension info.
	*/

	if (NULL == (ext_info = PEXGetExtensionInfo (display)))
	    return PEXUtPEXFailure;


	/*
	    Get list of supported targets from PEX.
	    If depth is specified as a hard criterion, use it as a filter.
	*/

	if ((ext_info->major_version == 5) &&
	    (ext_info->minor_version >= 1)) {

	    if (local_criteria.hard_criteria_mask & PEXUtDepth)
		target_depth = local_criteria.depth;
	    else
		target_depth = 0;

	    if (! PEXMatchRenderingTargets (display, 
					RootWindow(display, screen),
					target_depth, PEXWindowDrawable, NULL,
					32, &target_count, &targets)) {
		FREE_ALLOCATED_RESOURCES
		return PEXUtPEXFailure;
	    }

	    if (target_count == 0) {
		FREE_ALLOCATED_RESOURCES
		*unmet_criteria_return = local_criteria.hard_criteria_mask
					| local_criteria.soft_criteria_mask;
		return PEXUtCriteriaFailure;
	    }
	}


	/*
	    Get list of visuals for the screen.
	    If depth and/or visual class are specified as hard criteria, 
	    use them as filters.
	*/

	vis_template.screen = screen;
	vis_mask = VisualScreenMask;

	if (local_criteria.hard_criteria_mask & PEXUtDepth) {
	    vis_template.depth = local_criteria.depth;
	    vis_mask |= VisualDepthMask;
	}

	if (local_criteria.hard_criteria_mask & PEXUtVisualClass) {
	    vis_template.class = local_criteria.visual_class;
	    vis_mask |= VisualClassMask;
	}

	if (NULL == (visuals = XGetVisualInfo (display, vis_mask, &vis_template,
						&visual_count))) {
	    FREE_ALLOCATED_RESOURCES
	    return PEXUtXFailure;
	}


	/*
	    For each property in the list of properties specified by
	    the interoperability conventions, get the value of the property.
	*/
	if ((PEXUtStandardColormapProperty|PEXUtSharableColormap) &
	    (local_criteria.hard_criteria_mask | 
	    local_criteria.soft_criteria_mask)) {

	    for (prop_index=0; 
		prop_index<NUM_INTEROP_PROPERTIES; 
		prop_index++) {

		result = pexut_get_standard_cmap_property (display, screen, 
					&(interop_properties[prop_index]),
					&(property_atoms[prop_index]), 
					&(property_counts[prop_index]), 
					&(property_data[prop_index]));
		if (result != PEXUtSuccess) {
		    FREE_ALLOCATED_RESOURCES
		    return PEXUtXFailure;
		}
	    }
	}


	/*
	    Get the value of the SERVER_OVERLAY_VISUALS property, if supported.
	*/

	if (PEXUtLayer &
	    (local_criteria.hard_criteria_mask | 
	    local_criteria.soft_criteria_mask)) {

	    result = pexut_get_overlay_visuals_property (display, screen, 
						&overlay_count, &overlay_data);
	    if (result != PEXUtSuccess) {
		FREE_ALLOCATED_RESOURCES
		return PEXUtXFailure;
	    }
	}
	

    /*
    -----------------------------------------------------------------------
    Section III:  	Build information blocks about all the Visuals
			that are supported by PEX.
    -----------------------------------------------------------------------
    */

	/*
	    Allocate information blocks for all visuals.  (Could filter here
	    and only allocate space for visuals listed in target data, but
	    might take longer than just allocating space for all visuals.)
	*/

	visual_info_count = visual_count;
	visual_info = (visual_info_type *) 
			malloc (visual_info_count * sizeof(visual_info_type));

	if (visual_info == NULL) {
	    visual_info_count = 0;
	    FREE_ALLOCATED_RESOURCES
	    return PEXUtAllocFailure;
	}


	/*
	    For the remaining visuals, build an information structure
	    for each one.  Keep track of how well each one meets the
	    hard and soft criteria.  

	    Count the number that meet all the hard criteria.
	*/

	met_hard_count = 0;
	p_chosen_info = NULL;

	min_unmet_criteria_count = 999;
	p_min_unmet = NULL;
	soft_criteria_count = 
		pexut_count_bits_in_mask (local_criteria.soft_criteria_mask);

	p_info = visual_info;
	supported_visual_count = 0;
	for (visual_index=0; visual_index<visual_count; visual_index++) {

	    int found;
	    int supported;
	    unsigned int order_rating;
	    float temp;
	    int unmet_criteria_count;


	    /*
		If we have target data, determine support for the Visual from 
		that data.  Assign a rating based on the relative position in 
		the target data.

		Otherwise, assume support.
	    */

	    order_rating = 0;

	    if (target_count) {
		supported = False;
		for (target_index=0; 
		    target_index<target_count; 
		    target_index++) {

		    if (visuals[visual_index].visualid == 
			XVisualIDFromVisual (targets[target_index].visual)) {
			supported = True;
			break;
		    }
		}
	    }
	    else
		supported = True;

	    if (supported) {

		p_info->unmet_hard_criteria = 0;
		p_info->unmet_soft_criteria = 0;
		p_info->order_rating = order_rating;


		/*
		    Search the colormap properties for the first match with 
		    the visual.  The property data is used in evaluating color 
		    criteria as well as the standard colormap property 
		    criterion.

		    If we get property data, and we did not have target data 
		    from which to give an order rating, then give a rating 
		    depending on the relative position of the property entry, 
		    including the possibility of a search
		    process that involves more than one property.
		*/

		p_info->property_ptr = NULL;
		p_info->property_atom = None;

		if ((PEXUtStandardColormapProperty|PEXUtSharableColormap) &
		    (local_criteria.hard_criteria_mask | 
		    local_criteria.soft_criteria_mask)) {

		    found = False;
		    for (prop_index=0; 
			prop_index<NUM_INTEROP_PROPERTIES; 
			prop_index++) {

			for (entry_index=0; 
			    entry_index < property_counts[prop_index]; 
			    entry_index++) {

			    if (property_data[prop_index][entry_index].visualid
				== visuals[visual_index].visualid) {

				found = True;
				p_info->property_ptr = 
				    &(property_data[prop_index][entry_index]);
				p_info->property_atom = 
				    property_atoms[prop_index];
				break;
			    }
			}
			if (found) break;
		    }

		    if ((PEXUtStandardColormapProperty &
			(local_criteria.hard_criteria_mask | 
			local_criteria.soft_criteria_mask)) &&
		    (found != local_criteria.standard_colormap_property)) {
			p_info->unmet_hard_criteria |= 
			    (local_criteria.hard_criteria_mask 
			    & PEXUtStandardColormapProperty);
			p_info->unmet_soft_criteria |= 
			    (local_criteria.soft_criteria_mask 
			    & PEXUtStandardColormapProperty);
		    }
		}

		/*
		    Depth is not currently used for any rating.
		*/

		if (PEXUtDepth &
		    (local_criteria.hard_criteria_mask | 
		    local_criteria.soft_criteria_mask)) {

		    if (visuals[visual_index].depth != local_criteria.depth) {
			p_info->unmet_hard_criteria |= 
			    (local_criteria.hard_criteria_mask & PEXUtDepth);
			p_info->unmet_soft_criteria |= 
			    (local_criteria.soft_criteria_mask & PEXUtDepth);
		    }
		}

		/*
		    Color criteria are complicated, since they interact with 
		    property information.  This procedure not only evaluates 
		    whether or not criteria are met, but also assigns a rating 
		    based on available color resolution and the 
		    sharable_colormap criterion.
		*/

		result = pexut_evaluate_color_criteria (display, 
		    &local_criteria, &(visuals[visual_index]), p_info);
		if (result != PEXUtSuccess) {
		    FREE_ALLOCATED_RESOURCES
		    return result;
		}

		if (PEXUtVisualClass &
		    (local_criteria.hard_criteria_mask | 
		    local_criteria.soft_criteria_mask)) {

		    if (visuals[visual_index].class != 
			local_criteria.visual_class) {

			p_info->unmet_hard_criteria |= 
			    (local_criteria.hard_criteria_mask 
			    & PEXUtVisualClass);
			p_info->unmet_soft_criteria |= 
			    (local_criteria.soft_criteria_mask 
			    & PEXUtVisualClass);
		    }
		}

		/*
		    The Visual class is always a factor in rating:
		    static Visuals are preferred over dynamic Visuals,
		    all else being equal.
		*/

		if ((visuals[visual_index].class == TrueColor) ||
		    (visuals[visual_index].class == StaticColor) ||
		    (visuals[visual_index].class == StaticGray))
		    p_info->class_rating = MAX_CLASS_RATING;
		else
		    p_info->class_rating = 0;

		/*
		    Layer is not currently used for any rating.
		*/

		if (PEXUtLayer &
		    (local_criteria.hard_criteria_mask | 
		    local_criteria.soft_criteria_mask)) {

		    found = False;
		    for (prop_index=0; 
			prop_index < overlay_count; 
			prop_index++) {

			if (overlay_data[prop_index].visualid == 
				visuals[visual_index].visualid) {
			    found = True;
			    break;
			}
		    }

		    if ((!found) || 
			(((overlay_data[prop_index].layer == 1)? 
				PEXUtOverlay:PEXUtImage)
			    != local_criteria.layer)) {
			p_info->unmet_hard_criteria |= 
			    (local_criteria.hard_criteria_mask & PEXUtLayer);
			p_info->unmet_soft_criteria |= 
			    (local_criteria.soft_criteria_mask & PEXUtLayer);
		    }
		}

		/*
		    Determining double-buffering capability is a complicated 
		    affair.  This procedure attempts to assess whether 
		    criteria are met and assign a rating.
		*/

		result = pexut_evaluate_double_buffering_capability (display, 
			    &local_criteria, &(visuals[visual_index]), p_info);
		if (result != PEXUtSuccess) {
		    FREE_ALLOCATED_RESOURCES
		    return result;
		}

		/*
		    Determining support for color approximation types is 
		    straightforward but is done in a subprocedure in order 
		    to centralize it.  This procedure attempts to assess 
		    whether criteria are met and assign a rating.
		*/

		result = pexut_evaluate_color_approx_type (display, 
			    &local_criteria, &(visuals[visual_index]), p_info);
		if (result != PEXUtSuccess) {
		    FREE_ALLOCATED_RESOURCES
		    return result;
		}


		/*
		    At this point, all the criteria have been evaluated for 
		    this Visual.  If it met all the hard criteria, it is a 
		    candidate for success, and we give a rating based on how 
		    many soft criteria have also been met.  We keep a pointer 
		    to the last Visual that met all hard criteria, so we
		    can avoid some work if there turns out to be only one.

		    If not, we still keep track of the Visual that came 
		    closest to meeting all the hard criteria.
		*/

		if (p_info->unmet_hard_criteria == 0) {
		    met_hard_count++;

		    p_info->visual_ptr = &(visuals[visual_index]);
		    p_chosen_info = p_info;

		    if (soft_criteria_count) {
			unmet_criteria_count = 
			 pexut_count_bits_in_mask (p_info->unmet_soft_criteria);
			p_info->soft_rating = (int) 
						((((float) (soft_criteria_count 
						      - unmet_criteria_count))
						 /((float) soft_criteria_count))
						* MAX_SOFT_RATING);
		    }
		    else
			p_info->soft_rating = 0;
		}
		else {
		    unmet_criteria_count = 
			pexut_count_bits_in_mask (p_info->unmet_hard_criteria);

		    if (unmet_criteria_count < min_unmet_criteria_count) {
			min_unmet_criteria_count = unmet_criteria_count;
			p_min_unmet = p_info;
		    }
		}

		p_info++;
		supported_visual_count++;
	    }
	}


    /*
    -----------------------------------------------------------------------
    Section IV:	Evaluate candidates (Visuals that meet hard criteria).
    -----------------------------------------------------------------------
    */

	/*
	    If no Visual has met the hard criteria, we have failed.
	    We will return the criteria information for the Visual that 
	    came closest to being accepted.
	*/

	if (met_hard_count == 0) {

	    *unmet_criteria_return = p_min_unmet->unmet_hard_criteria |
				     p_min_unmet->unmet_soft_criteria;
	    FREE_ALLOCATED_RESOURCES
	    return PEXUtCriteriaFailure;
	}


	/*
	    If more than one Visual has met the hard criteria, make a pass 
	    to choose the best one.  
	*/

	if (met_hard_count > 1) {

	    unsigned int overall_rating;
	    unsigned int max_rating = 0;

	    p_info = visual_info;
	    p_chosen_info = p_info;
	    for (visual_index=0; 
		visual_index<supported_visual_count; 
		visual_index++) {

		if (p_info->unmet_hard_criteria == 0) {

		    overall_rating = pexut_compute_overall_rating (p_info);
		    if (overall_rating > max_rating) {
			max_rating = overall_rating;
			p_chosen_info = p_info;
		    }
		}

		p_info++;
	    }
	}


    /*
    -----------------------------------------------------------------------
    Section V:	Copy or generate return information.
    -----------------------------------------------------------------------
    */

	/*
	    If we have chosen a Visual, and we've got standard colormap 
	    information, and the caller requested it, we'll return that 
	    and the atom for the property.

	    If we have chosen a Visual, but we don't have standard colormap
	    information yet, then synthesize the info from the visual.
	    This is just an "educated" guess at what kind of color ramp
	    might be supported by PEX on the visual.
	*/

	if (((PEXUtStandardColormapProperty|PEXUtSharableColormap) &
	    (local_criteria.hard_criteria_mask | 
		local_criteria.soft_criteria_mask)) &&
	    (local_criteria.standard_colormap_property || 
		local_criteria.sharable_colormap) &&
	    (p_chosen_info->property_ptr != NULL)) {

	    *cmap_info_return = *(p_chosen_info->property_ptr);
	    *std_prop_atom_return = p_chosen_info->property_atom;
	    color_approx_type = PEXColorSpace;
	}
	else {
	    pexut_synthesize_cmap_from_visual (p_chosen_info->visual_ptr, 
						cmap_info_return);
	    *std_prop_atom_return = None;
	    color_approx_type = -1;
	}

	*vis_info_return = *(p_chosen_info->visual_ptr);

	if (((PEXUtColorApproxType) &
	    (local_criteria.hard_criteria_mask | 
		local_criteria.soft_criteria_mask)) &&
	    (!(PEXUtColorApproxType & p_chosen_info->unmet_soft_criteria)))
	    color_approx_type = local_criteria.color_approx_type;

	pexut_load_color_approx_from_std_cmap (color_approx_type, 
					vis_info_return, cmap_info_return, 
					capx_info_return);

	*unmet_criteria_return = p_chosen_info->unmet_soft_criteria;
	FREE_ALLOCATED_RESOURCES

	if (*unmet_criteria_return)
	    return PEXUtQualifiedSuccess;
	else
	    return PEXUtSuccess;

#undef FREE_ALLOCATED_RESOURCES

} /* PEXUtFindVisual */


int PEXUtGetStandardColormapInfo (
		display,
		vis_info,
		cmap_info_return,
		capx_info_return,
		std_prop_atom_return
		)

		Display				*display;
		XVisualInfo			*vis_info;
		XStandardColormap		*cmap_info_return;
		PEXColorApproxEntry		*capx_info_return;
		Atom				*std_prop_atom_return;
{

	int			entry_count;
	Atom			property_atom;
	XStandardColormap	*property_data;

	int			prop_index;
	int			entry_index;
	int			result;


	/*
	    For each property in the list of properties specified by
	    the interoperability conventions, get the value of the 
	    property, and search it for the specified visual.
	*/

	for (prop_index=0; prop_index<NUM_INTEROP_PROPERTIES; prop_index++) {

	    result = pexut_get_standard_cmap_property (display, 
					    vis_info->screen, 
					    &(interop_properties[prop_index]),
					    &property_atom,
					    &entry_count,
					    &property_data);

	    if (result != PEXUtSuccess) {
		return result;
	    }

	    for (entry_index=0; entry_index < entry_count; entry_index++) {

		if (property_data[entry_index].visualid == vis_info->visualid) {

			*cmap_info_return = property_data[entry_index];
			pexut_load_color_approx_from_std_cmap (PEXColorSpace, 
							    vis_info, 
							    cmap_info_return, 
							    capx_info_return);
			XFree (property_data);
			*std_prop_atom_return = property_atom;
			return PEXUtSuccess;
		}
	    }

	    if (entry_count > 0)
		XFree (property_data);
	}

	/*
	    If the visual was not found in any property, synthesize colormap 
	    info from the visual.  In other words, make an educated guess.
	*/

	pexut_synthesize_cmap_from_visual (vis_info, cmap_info_return);
	pexut_load_color_approx_from_std_cmap (-1, vis_info, 
						cmap_info_return, 
						capx_info_return);
	*std_prop_atom_return = None;
	return PEXUtSuccess;

} /* PEXUtGetStandardColormapInfo */


int PEXUtVerifyColorApproximation (
		display,
		capx_info,
		vis_info
		)

		Display				*display;
		PEXColorApproxEntry		*capx_info;
		XVisualInfo			*vis_info;
{
	int			result;
	int			return_value;
	Window			window;
	int			inquiry_supported;

	/*
	    Create a temporary unmapped window in the visual.
	*/
	pexut_create_temporary_window (display, vis_info, &window);


	/*
	    Verify that specified approx type is supported
	    using PEXGetEnumTypeInfo.  If not, return failure.
	*/

	result = pexut_verify_color_approx_type (display, window, 
						capx_info->type);
	if (result != PEXUtSuccess) {
	    pexut_destroy_temporary_window (display, vis_info, window);
	    return result;
	}

	inquiry_supported = False;

	{
	    int			enum_types[1];
	    unsigned long 	*count;
	    PEXEnumTypeDesc	*enum_data;
	    int			enum_index;

	    /*
		Verify that the QueryColorApprox escape is supported
		using PEXGetEnumTypeInfo.  If not, we just can't verify
		support.
	    */

	    enum_types[0] = PEXETEscape;
	    if (!PEXGetEnumTypeInfo (display, window, 1, enum_types, 
				    PEXETIndex, &count, &enum_data)) {

		pexut_destroy_temporary_window (display, vis_info, window);
		return PEXUtPEXFailure;
	    }
	    else {

		for (enum_index = 0; enum_index < *count; enum_index++) {
		    if ((short) enum_data[enum_index].index == 
			(short) PEXETEscapeQueryColorApprox) {
			inquiry_supported = True;
			break;
		    }
		}
		PEXFreeEnumInfo (1, count, enum_data);
	    }
	}

	if (inquiry_supported) {

	    PEXEscapeQueryColorApproxData	query;
	    PEXEscapeQueryColorApproxReplyData	*reply;
	    unsigned long 			reply_length;

	    query.drawable = window;
	    query.capx = *capx_info;

	    reply = (PEXEscapeQueryColorApproxReplyData *)
		    PEXEscapeWithReply (display, PEXEscapeQueryColorApprox, 
					sizeof(query), ((char *) &query), 
					&reply_length);

	    if (reply != NULL) {

		if (reply->capx_is_supported)
		    return_value = PEXUtSuccess;
		else
		    return_value = PEXUtCriteriaFailure;

		XFree ((char *) reply);
	    }
	    else {
		pexut_destroy_temporary_window (display, vis_info, window);
		return PEXUtPEXFailure;
	    }
	}
	else {
	    /* Can't determine support or non-support. */
	    return_value = PEXUtQualifiedSuccess;
	}


	/*
	    Destroy the temporary window.
	*/

	pexut_destroy_temporary_window (display, vis_info, window);


	return return_value;
	
} /* PEXUtVerifyColorApproximation */


int PEXUtCreateWriteableColormap (
		display,
		vis_info,
		capx_info,
		colormap_id_return
		)

		Display				*display;
		XVisualInfo			*vis_info;
		PEXColorApproxEntry		*capx_info;
		Colormap			*colormap_id_return;
{
	int 	result;

	result = PEXUtXFailure;
	switch (vis_info->class) {

	case PseudoColor:	
	case GrayScale:	
	    result = pexut_create_one_channel_map (display, vis_info, 
						capx_info, colormap_id_return);
	    break;

	case DirectColor:	
	    result = pexut_create_three_channel_map (display, vis_info, 
						capx_info, colormap_id_return);
	    break;

	case StaticColor:
	case TrueColor:
	case StaticGray:
	    result = pexut_create_read_only_map (display, vis_info, 
						capx_info, colormap_id_return);
	    break;
	}

	return result;
	
} /* PEXUtCreateWriteableColormap */


int PEXUtModifyColormapForVendorDependencies (
		display,
		colormap_id,
		vis_info,
		capx_info
		)

		Display				*display;
		Colormap			colormap_id;
		XVisualInfo			*vis_info;
		PEXColorApproxEntry		*capx_info;
{
	PEXExtensionInfo	*ext_info;
	int			result;

	/*
	    Get PEX extension info.
	*/

	if (NULL == (ext_info = PEXGetExtensionInfo (display)))
	    return PEXUtPEXFailure;

	result = PEXUtUnmodifiedResource;
	if (!strcmp(ext_info->vendor_name, "Hewlett-Packard Company")) {
	    result = pexut_modify_colormap_for_HP_dependencies (
					display, colormap_id, 
					vis_info, capx_info);
	}
	/*
	else if (!strcmp(ext_info->vendor_name, "vendor_2")) {
	    result = pexut_modify_colormap_for_vendor_2_dependencies (
					display, colormap_id, 
					vis_info, capx_info);
	}
	*/

	return result;

} /* PEXUtModifyColormapForVendorDependencies */


int PEXUtCopyColormapAsReadOnly (
		display,
		source_cmap_id,
		vis_info,
		capx_info,
		cmap_id_return
		)

		Display				*display;
		Colormap			source_cmap_id;
		XVisualInfo			*vis_info;
		PEXColorApproxEntry		*capx_info;
		Colormap			*cmap_id_return;

/*
    NOTE:  	There is an assumption in this code that if XAllocColor does 
		not find an existing cell that matches the RGB, that it
		allocates the lowest unallocated cell available, i.e. it
		works from pixel zero towards higher values.

			
    Algorithm:

	Section I:	Allocate a new Colormap and allocate all the cells.

	Section II:	Compute the number of cells in the color approximation
			region.  Fetch all the cells of the source Colormap.

	Section III:	Free and reallocate as read-only the regions above
			and below the color approximation region if it is
			partial.  The color approximation region must be left
			read-write because XAllocColor might find matches in
			the cells outside the region and so would leave
			unallocated cells inside the region.  Later XAllocColor
			calls can then corrupt the color approximation ramp.

			Freeing and reallocating the color approximation 
			region first doesn't work either because then 
			XAllocColor calls for the lower region find matches 
			within the color ramp, and so leave holes in the 
			lower region that is often already in use by other 
			clients.  Later XAllocColor calls then write these
			cells and cause color flashing in the human interface.

	Section IV:	Free and allocate the entire Colormap as read-only
			if the color approximation region consumes the entire
			Colormap.  This is safe because no unallocated cells
			will be left.  If the color approximation region
			was partial, then write the region with the source
			colors.
*/
{
	Colormap	cmap_id;
	int		num_reds, num_greens, num_blues, num_colors;
	int		red_shift, green_shift, blue_shift;
	int		vis_red_shift, vis_green_shift, vis_blue_shift;
	unsigned long	temp;
	unsigned long	*pixels, pixel_value, *p_pixel;
	XColor		*source_colors, *p_color, screen_def;
	int		result;


	/*
	    Nothing to do for read-only Visuals.
	*/

	if ((vis_info->class == StaticGray) ||
	    (vis_info->class == StaticColor) ||
	    (vis_info->class == TrueColor)) {

	    *cmap_id_return = source_cmap_id;
	    return PEXUtSuccess;
	}

    /*
    -----------------------------------------------------------------------
    Section I:	Allocate a new Colormap and allocate all the cells.
    -----------------------------------------------------------------------
    */

	/*
	    Allocate new colormap with AllocNone.
	    Allocate all the cells as read/write using XAllocColorCells.
	*/

	cmap_id = XCreateColormap ( display, 
				    RootWindow (display, vis_info->screen),
				    vis_info->visual, AllocNone );
	if (cmap_id == None)
	    return PEXUtXFailure;

	pixels = (unsigned long *) 
		    malloc (vis_info->colormap_size * sizeof(unsigned long));
	if (pixels == NULL) {
	    XFreeColormap (display, cmap_id);
	    return PEXUtAllocFailure;
	}

	result = XAllocColorCells ( display, cmap_id, True,
				    (unsigned long *) NULL, 0, 
				    pixels, vis_info->colormap_size);
	if (! result) {
	    free (pixels);
	    XFreeColormap (display, cmap_id);
	    return PEXUtXFailure;
	}


    /*
    -----------------------------------------------------------------------
    Section II:	Compute the number of cells in the color approximation
		region.  Fetch all the cells of the source Colormap.
    -----------------------------------------------------------------------
    */
	/*
	    Compute the number of cells in the ramp region.
	*/

	if ((vis_info->class == GrayScale) ||
	    (vis_info->class == PseudoColor)) {

	    if (capx_info->type == PEXColorRange) {

		num_colors = capx_info->max1 + 1;
	    }
	    else { /* assume PEXColorSpace */

		num_reds = capx_info->max1 + 1;
		num_greens = capx_info->max2 + 1;
		num_blues = capx_info->max3 + 1;
		num_colors = num_reds * num_greens * num_blues;
	    }
	}
	else { /* assume DirectColor  */

	    num_reds = capx_info->max1 + 1;
	    num_greens = capx_info->max2 + 1;
	    num_blues = capx_info->max3 + 1;
	    num_colors = num_reds;
	    if (num_greens > num_colors) num_colors = num_greens;
	    if (num_blues > num_colors) num_colors = num_blues;

	    for (red_shift = 0, temp = capx_info->mult1;
		temp > 1; red_shift++, temp >>= 1);
	    vis_red_shift = 0;
	    temp = vis_info->red_mask;
	    while (!(temp & 0x1)) {
		vis_red_shift++;
		temp >>= 1;
	    }

	    for (green_shift = 0, temp = capx_info->mult2;
		temp > 1; green_shift++, temp >>= 1);
	    vis_green_shift = 0;
	    temp = vis_info->green_mask;
	    while (!(temp & 0x1)) {
		vis_green_shift++;
		temp >>= 1;
	    }

	    for (blue_shift = 0, temp = capx_info->mult3;
		temp > 1; blue_shift++, temp >>= 1);
	    vis_blue_shift = 0;
	    temp = vis_info->blue_mask;
	    while (!(temp & 0x1)) {
		vis_blue_shift++;
		temp >>= 1;
	    }

	    if ((red_shift != vis_red_shift) ||
		(green_shift != vis_green_shift) ||
		(blue_shift != vis_blue_shift)) {

		free (pixels);
		XFreeColormap (display, cmap_id);
		return PEXUtXFailure;
	    }
	}

	if (capx_info->base_pixel >= vis_info->colormap_size) {
	    free (pixels);
	    XFreeColormap (display, cmap_id);
	    return PEXUtXFailure;
	}
	if (capx_info->base_pixel + num_colors > vis_info->colormap_size)
	    num_colors = vis_info->colormap_size - capx_info->base_pixel;


	/*
	    Fetch all the cells in the source colormap.
	*/

	if ((source_colors = (XColor *) malloc 
		(vis_info->colormap_size * sizeof(XColor))) == NULL) {

	    free (pixels);
	    XFreeColormap (display, cmap_id);
	    return PEXUtAllocFailure;
	}

	for (pixel_value = 0, p_color = source_colors; 
	    pixel_value < vis_info->colormap_size;
	    pixel_value++, p_color++) {

	    if (vis_info->class == DirectColor) {

		p_color->pixel = (pixel_value << red_shift);
		p_color->pixel |= (pixel_value << green_shift);
		p_color->pixel |= (pixel_value << blue_shift);
	    }
	    else {
		p_color->pixel = pixel_value;
	    }
	    p_color->flags = DoRed | DoGreen | DoBlue;
	}
	XQueryColors (display, source_cmap_id, source_colors, 
			vis_info->colormap_size);


    /*
    -----------------------------------------------------------------------
    Section III:	Free and reallocate as read-only the regions above
			and below the color approximation region if it is
			partial.  The color approximation region must be left
			read-write because XAllocColor might find matches in
			the cells outside the region and so would leave
			unallocated cells inside the region.  Later XAllocColor
			calls can then corrupt the color approximation ramp.

			Freeing and reallocating the color approximation 
			region first doesn't work either because then 
			XAllocColor calls for the lower region find matches 
			within the color ramp, and so leave holes in the 
			lower region that is often already in use by other 
			clients.  Later XAllocColor calls then write these
			cells and cause color flashing in the human interface.
    -----------------------------------------------------------------------
    */

	/*
	    Free cells above the ramp region and allocate those colors.
	*/

	if ((capx_info->base_pixel + num_colors) < vis_info->colormap_size) {

	    for (pixel_value = (capx_info->base_pixel + num_colors), 
		p_pixel = pixels;
		pixel_value < vis_info->colormap_size;
		pixel_value++, p_pixel++)  {

		if (vis_info->class == DirectColor) {

		    *p_pixel = (pixel_value << red_shift);
		    *p_pixel |= (pixel_value << green_shift);
		    *p_pixel |= (pixel_value << blue_shift);
		}
		else {
		    *p_pixel = pixel_value;
		}
	    }

	    XFreeColors (display, cmap_id, pixels, 
			(vis_info->colormap_size - 
			    (capx_info->base_pixel + num_colors)), 
			0);

	    for (pixel_value = (capx_info->base_pixel + num_colors), 
		p_color = source_colors + (capx_info->base_pixel + num_colors);
		pixel_value < vis_info->colormap_size;
		pixel_value++, p_color++) {

		screen_def = *p_color;
		if (!XAllocColor (display, cmap_id, &screen_def)) {
		    free (pixels);
		    free (source_colors);
		    XFreeColormap (display, cmap_id);
		    return PEXUtXFailure;
		}
	    }
	}

	/*
	    Free cells below the ramp region and allocate those colors.
	*/

	if (capx_info->base_pixel > 0) {

	    for (pixel_value = 0, p_pixel = pixels;
		pixel_value < capx_info->base_pixel;
		pixel_value++, p_pixel++)  {

		if (vis_info->class == DirectColor) {

		    *p_pixel = (pixel_value << red_shift);
		    *p_pixel |= (pixel_value << green_shift);
		    *p_pixel |= (pixel_value << blue_shift);
		}
		else {
		    *p_pixel = pixel_value;
		}
	    }

	    XFreeColors (display, cmap_id, pixels, 
			capx_info->base_pixel, 0);

	    for (pixel_value = 0,
		p_color = source_colors;
		pixel_value < capx_info->base_pixel;
		pixel_value++, p_color++) {

		screen_def = *p_color;
		if (!XAllocColor (display, cmap_id, &screen_def)) {
		    free (pixels);
		    free (source_colors);
		    XFreeColormap (display, cmap_id);
		    return PEXUtXFailure;
		}
	    }
	}


    /*
    -----------------------------------------------------------------------
    Section IV:		Free and allocate the entire Colormap as read-only
			if the color approximation region consumes the entire
			Colormap.  This is safe because no unallocated cells
			will be left.  If the color approximation region
			was partial, then write the region with the source
			colors.
    -----------------------------------------------------------------------
    */
	/*
	    If the ramp consumes the entire map,
	    free cells in the ramp region and allocate those colors read-only.
	*/

	if (num_colors == vis_info->colormap_size) {

	    for (pixel_value = capx_info->base_pixel, p_pixel = pixels;
		pixel_value < (capx_info->base_pixel + num_colors);
		pixel_value++, p_pixel++)  {

		if (vis_info->class == DirectColor) {

		    *p_pixel = (pixel_value << red_shift);
		    *p_pixel |= (pixel_value << green_shift);
		    *p_pixel |= (pixel_value << blue_shift);
		}
		else {
		    *p_pixel = pixel_value;
		}
	    }

	    XFreeColors (display, cmap_id, pixels, 
			    num_colors, 0);

	    for (pixel_value = capx_info->base_pixel,
		p_color = source_colors + capx_info->base_pixel;
		pixel_value < (capx_info->base_pixel + num_colors);
		pixel_value++, p_color++) {

		screen_def = *p_color;
		if (!XAllocColor (display, cmap_id, &screen_def)) {
		    free (pixels);
		    free (source_colors);
		    XFreeColormap (display, cmap_id);
		    return PEXUtXFailure;
		}
	    }
	}
	else {
	    /*
		Write out the colors in the region to the new colormap.
	    */

	    XStoreColors (display, cmap_id, 
			    source_colors + capx_info->base_pixel, 
			    num_colors);
	}


	/*
	    Free the allocated storage.
	    Caller must free the source colormap.
	*/

	free (pixels);
	free (source_colors);

	*cmap_id_return = cmap_id;
	return PEXUtSuccess;

} /* PEXUtCopyColormapAsReadOnly */


int PEXUtCreateColormap (
		display,
		vis_info,
		capx_info,
		colormap_id_return
		)

		Display				*display;
		XVisualInfo			*vis_info;
		PEXColorApproxEntry		*capx_info;
		Colormap			*colormap_id_return;
{
	int		result;
	Colormap	writeable_cmap_id;
	Colormap	readonly_cmap_id;
    

	result = PEXUtCreateWriteableColormap (display, vis_info,
				      capx_info, &writeable_cmap_id);
	if (result != PEXUtSuccess)
	    return result;

	result = PEXUtModifyColormapForVendorDependencies (
				    display, writeable_cmap_id,
				    vis_info, capx_info);

	if ((result != PEXUtUnmodifiedResource) &&
	    (result != PEXUtModifiedResource))
	    return result;

	result = PEXUtCopyColormapAsReadOnly (display, writeable_cmap_id,
					    vis_info, capx_info,
					    &readonly_cmap_id);
	if (result != PEXUtSuccess)
	    return result;

	if (readonly_cmap_id != writeable_cmap_id)
	    XFreeColormap (display, writeable_cmap_id);
	*colormap_id_return = readonly_cmap_id;
	return PEXUtSuccess;

} /* PEXUtCreateColormap */


int PEXUtCreateWindow (
		display,
		screen,
		window_info,
		vis_info,
		window_return,
		background_color_return
		)

		Display				*display;
		int				screen;
		PEXUtWindowSpecification	*window_info;
		XVisualInfo			*vis_info;
		Window				*window_return;
		XColor				*background_color_return;
{
	unsigned long 		window_mask;
	XSetWindowAttributes 	window_attrs;
	int			result;

	unsigned long		pixel_value;
	XColor			screen_def;
	XColor			exact_def;
	int			need_color;


	/*
	    Check the window specification.
	*/

	if (( !(window_info->attr_mask & CWColormap)) ||
	    (window_info->attrs.colormap == None))
	    return PEXUtXFailure;


	/*
	    Look up colors for background and border.
	*/

	window_mask = window_info->attr_mask;
	window_attrs = window_info->attrs;

	if ( !(window_info->attr_mask & CWBackPixel)) {

	    need_color = True;
	    if (XParseColor (display, window_attrs.colormap, 
			    window_info->background_color_name, &exact_def)) {

		screen_def = exact_def;

		if (XAllocColor (display, window_attrs.colormap, &screen_def)) {
		    pixel_value = screen_def.pixel;
		    *background_color_return = screen_def;
		    need_color = False;
		}
	    }

	    if (need_color) {

		if (XAllocNamedColor (display, window_attrs.colormap, "Black", 
					&screen_def, &exact_def)) {
		    pixel_value = screen_def.pixel;
		    *background_color_return = screen_def;
		}
		else {
		    pixel_value = 0;
		    background_color_return->pixel = pixel_value;
		    background_color_return->red = 0;
		    background_color_return->green = 0;
		    background_color_return->blue = 0;
		}
	    }
	    window_attrs.background_pixel = pixel_value;
	    window_mask |= CWBackPixel;
	}

	if ( !(window_info->attr_mask & CWBorderPixel)) {

	    need_color = True;
	    if (XParseColor (display, window_attrs.colormap, 
			    window_info->border_color_name, &exact_def)) {

		screen_def = exact_def;

		if (XAllocColor (display, window_attrs.colormap, &screen_def)) {
		    pixel_value = screen_def.pixel;
		    need_color = False;
		}
	    }

	    if (need_color) {

		if (XAllocNamedColor (display, window_attrs.colormap, "White", 
					&screen_def, &exact_def))
		    pixel_value = screen_def.pixel;
		else
		    pixel_value = 0;
	    }
	    window_attrs.border_pixel = pixel_value;
	    window_mask |= CWBorderPixel;
	}


	/*
	    Create the window.
	*/

	*window_return = XCreateWindow (display, 
				window_info->parent,
				window_info->size_hints.x,
				window_info->size_hints.y,
				window_info->size_hints.width,
				window_info->size_hints.height,
				window_info->border_width,
				vis_info->depth, 
				InputOutput,
				vis_info->visual,
				window_mask,
				&window_attrs);

	if (*window_return == None)
	    return PEXUtXFailure;

	XSetNormalHints (display, *window_return, &(window_info->size_hints));
	XStoreName (display, *window_return, window_info->title);
	XSync (display,0);

	return PEXUtSuccess;

} /* PEXUtCreateWindow */


int PEXUtCreateWindowAndColormap (
		display,
		screen,
		criteria,
		window_info,
		window_return,
		vis_info_return,
		cmap_info_return,
		capx_info_return,
		unmet_criteria_return,
		std_prop_atom_return,
		background_color_return
		)

		Display				*display;
		int				screen;
		PEXUtVisualCriteria		*criteria;
		PEXUtWindowSpecification	*window_info;
		Window				*window_return;
		XVisualInfo			*vis_info_return;
		XStandardColormap		*cmap_info_return;
		PEXColorApproxEntry		*capx_info_return;
		unsigned int			*unmet_criteria_return;
		Atom				*std_prop_atom_return;
		XColor				*background_color_return;
{
	Colormap			cmap_id_from_property;
	Colormap			created_cmap_id;
	PEXUtWindowSpecification	local_window_info;
	int 				result;
	

	/*
	    Find visual.
	*/

	result = PEXUtFindVisual (	display, screen, criteria,
					vis_info_return,
					cmap_info_return,
					capx_info_return,
					unmet_criteria_return,
					std_prop_atom_return );

	if ((result != PEXUtSuccess) && (result != PEXUtQualifiedSuccess))
	    return result;

	cmap_id_from_property = cmap_info_return->colormap;


	/*
	    Verify color approximation.
	*/

	result = PEXUtVerifyColorApproximation (display, 
						capx_info_return, 
						vis_info_return);
	if ((result != PEXUtSuccess) && (result != PEXUtQualifiedSuccess))
	    return result;


	/*
	    If sharing, and the default Visual was chosen but there is
	    no Colormap ID from a property, then use the default Colormap.
	*/

	if ((PEXUtSharableColormap &
	    (criteria->hard_criteria_mask | criteria->soft_criteria_mask)) &&
	    (vis_info_return->visual == DefaultVisual(display, screen))) {

	    if (cmap_id_from_property == None) {
		cmap_id_from_property = DefaultColormap(display, screen);
		cmap_info_return->colormap = cmap_id_from_property;
	    }
	}

	    
	/*
	    If no colormap ID in returned info, or not sharing,
	    create a colormap and modify it for vendor dependencies.
	    Then reallocate the same colors as read-only, so
	    XAllocColor has a chance of working.
	*/

	if ((cmap_id_from_property == None) ||
	    (!(PEXUtSharableColormap &
	    (criteria->hard_criteria_mask | criteria->soft_criteria_mask)))) {

	    result = PEXUtCreateColormap (display, vis_info_return,
					  capx_info_return, &created_cmap_id);
	    if (result != PEXUtSuccess)
		return result;

	    cmap_info_return->colormap = created_cmap_id;
	}


	/*
	    Load a local copy of the window info with an augmented set of items
	    and create the window.
	*/

	local_window_info = *window_info;
	local_window_info.attrs.colormap = cmap_info_return->colormap;
	local_window_info.attr_mask |= CWColormap;
	local_window_info.attr_mask &= ~(CWBackPixel|CWBorderPixel);

	result = PEXUtCreateWindow (display, screen, &local_window_info, 
				   vis_info_return, window_return, 
				    background_color_return);

	if (result == PEXUtSuccess) {
	    XMapWindow (display, *window_return);
	    XSync (display, 0);
	}

	return result;

} /* PEXUtCreateWindowAndColormap */
