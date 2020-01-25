/* $XConsortium: pexutcmapint.c,v 1.0 93/11/22 12:23:48 rws Exp $ */

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
/* $Source: /xsrc/xc/unsupported/test/suspex/src/pexut_hp/RCS/pexutcmapint.c,v $                         */
/* $Date: 93/11/22 12:23:48 $                                                 */
/* $Revision: 1.0 $                                                   */
/*                                                                            */
/* Description:                                                               */
/*   Internal routine implementation file for PEXUt colormap/visual utilities.*/
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

#ifndef MBX_HEADER_FILE_NOT_INSTALLED /* [ */
#include <X11/extensions/multibuf.h>
#endif /* !MBX_HEADER_FILE_NOT_INSTALLED ] */
#include "pexutdbint.h"


int pexut_get_standard_cmap_property (
		display,
		screen,
		property_info,
		property_atom_return,
		property_count_return,
		property_data_return
		)

		Display				*display;
		int				screen;
		interop_property_type		*property_info;
		Atom				*property_atom_return;
		int				*property_count_return;
		XStandardColormap		**property_data_return;
{
	Atom	    		actual_type;
	int			actual_format;
	unsigned long   	item_count;
	unsigned long   	item_count_return, bytes_unread;
	int			result;


	if ( !property_info->is_standard) {

	    if (property_info->have_atom)
		*property_atom_return = property_info->property_atom;
	    else {
		/*
		    Get the atom from the X server.  If it doesn't exist,
		    neither does the property.  This should not be considered
		    a fatal error.
		*/

		*property_atom_return = XInternAtom(display, 
						    property_info->property_name, 
						    True);
		if (*property_atom_return == None) {
		    *property_count_return = 0;
		    return PEXUtSuccess;
		}
	    }


	    /* 
		Property atom exists - fetch the property.
	    */

	    bytes_unread = 0;
	    item_count = sizeof(XStandardColormap)/4;
	    do {
		item_count += (bytes_unread+3)/4;
		result = XGetWindowProperty (display, 
					RootWindow(display, screen),
					*property_atom_return,
					0, item_count, False, 
					*property_atom_return,
					&actual_type, &actual_format, 
					&item_count_return, &bytes_unread,
					(unsigned char **) 
					    property_data_return);

	    } while ((result == Success) && (bytes_unread > 0));

	    if (result != Success)
		return PEXUtXFailure;

	    *property_count_return = 
		item_count_return/(sizeof(XStandardColormap)/4);

	    return PEXUtSuccess;
	}
	else {

	    /*
		Note that XGetRGBColormaps returns a zero status even
		if nothing is wrong but the property doesn't exist.
	    */

	    *property_atom_return = property_info->property_atom;

	    result = XGetRGBColormaps (display, 
					RootWindow(display, screen),
					property_data_return, 
					property_count_return, 
					*property_atom_return);
	    if (!result) 
		*property_count_return = 0;
	    return PEXUtSuccess;
	}
	
} /* pexut_get_standard_cmap_property */
		

int pexut_get_overlay_visuals_property (
		display,
		screen,
		property_count_return,
		property_data_return
		)

		Display				*display;
		int				screen;
		int				*property_count_return;
		overlay_visuals_type		**property_data_return;
{
	Atom			property_atom;
	Atom	    		actual_type;
	int			actual_format;
	unsigned long   	item_count;
	unsigned long   	item_count_return, bytes_unread;
	int			result;

	/*
	    Get the atom from the X server.  If it doesn't exist,
	    neither does the property.
	*/

	property_atom = XInternAtom(display, "SERVER_OVERLAY_VISUALS", True);
	if (property_atom == None) {
	    return PEXUtXFailure;
	}


	/* 
	    Property atom exists - fetch the property.
	*/

	bytes_unread = 0;
	item_count = sizeof(overlay_visuals_type)/4;
	do {
	    item_count += (bytes_unread+3)/4;
	    result = XGetWindowProperty (display, 
				    RootWindow(display, screen),
				    property_atom,
				    0, item_count, False, 
				    property_atom,
				    &actual_type, &actual_format, 
				    &item_count_return, &bytes_unread,
				    (unsigned char **) property_data_return);

	} while ((result == Success) && (bytes_unread > 0));

	if (result != Success)
	    return PEXUtXFailure;

	*property_count_return = item_count_return/(sizeof(overlay_visuals_type)/4);

	return PEXUtSuccess;

} /* pexut_get_overlay_visuals_property */
		

int pexut_evaluate_color_criteria (
		display,
		criteria,
		vis_info,
		p_info
		)

		Display				*display;
		PEXUtVisualCriteria		*criteria;
		XVisualInfo			*vis_info;
		visual_info_type		*p_info;

/*
    Algorithm:

	Section I:	Compute the color resolution numbers.
			If we have a property entry, use it to do this,
			otherwise use the Visual description.

	Section II:	Mark the information block as criteria are tested
			for satisfaction.  Also, keep numbers for the
			degree to which the available colors surpass the
			specified minima.

	Section III:	Compute an overall color rating based on to what
			degree the colors exceed the minima.  If no color
			criteria were specified, this rating comes out zero.

			If sharable_colormap was a criterion, Visuals that
			are not named in properties lose their color rating.
			Visuals that were named in a property, but whose
			ramps do not meet the criteria, also lose.
*/
{
	PEXEnumTypeIndex	color_approx_type;
	int			avail_reds, avail_greens, avail_blues;
	int			avail_colors;
	int			colors_excess, 
				total_excess, 
				reds_excess, 
				greens_excess, 
				blues_excess;
	unsigned long		temp;
	XStandardColormap	*cmap_info;


    /*
    -----------------------------------------------------------------------
    Section I:		Compute the color resolution numbers.
			If we have a property entry, use it to do this,
			otherwise use the Visual description.
    -----------------------------------------------------------------------
    */

	if (PEXUtColorApproxType &
	    (criteria->hard_criteria_mask|criteria->soft_criteria_mask))
	    color_approx_type = criteria->color_approx_type;
	else
	    color_approx_type = PEXColorSpace;

	if ((p_info->property_ptr != NULL) &&
	    (color_approx_type == PEXColorSpace)) {

	    cmap_info = p_info->property_ptr;

	    avail_reds = cmap_info->red_max + 1;
	    avail_greens = cmap_info->green_max + 1;
	    avail_blues = cmap_info->blue_max + 1;
	    avail_colors = avail_reds * avail_greens * avail_blues;
	}
	else {
	    switch (vis_info->class) {

	    case PseudoColor:
	    case StaticColor:
	    case GrayScale:
	    case StaticGray:

		avail_colors = vis_info->colormap_size;
		avail_reds = vis_info->colormap_size;
		avail_greens = vis_info->colormap_size;
		avail_blues = vis_info->colormap_size;
		break;

	    case DirectColor:
	    case TrueColor:

		avail_reds = 1;
		for (temp = vis_info->red_mask; temp > 0; temp >>= 1) {
		    if (temp & 1)
			avail_reds *= 2;
		}

		avail_greens = 1;
		for (temp = vis_info->green_mask; temp > 0; temp >>= 1) {
		    if (temp & 1)
			avail_greens *= 2;
		}

		avail_blues = 1;
		for (temp = vis_info->blue_mask; temp > 0; temp >>= 1) {
		    if (temp & 1)
			avail_blues *= 2;
		}
		avail_colors = avail_reds * avail_greens * avail_blues;
		break;
	    }
	}
	

    /*
    -----------------------------------------------------------------------
    Section II:		Mark the information block as criteria are tested
			for satisfaction.  Also, keep numbers for the
			degree to which the available colors surpass the
			specified minima.
    -----------------------------------------------------------------------
    */

	if (PEXUtMinColors &
	    (criteria->hard_criteria_mask | criteria->soft_criteria_mask)) {

	    if (avail_colors < criteria->min_colors) {
		p_info->unmet_hard_criteria |= 
		    (criteria->hard_criteria_mask & PEXUtMinColors);
		p_info->unmet_soft_criteria |= 
		    (criteria->soft_criteria_mask & PEXUtMinColors);
		colors_excess = criteria->min_colors - avail_colors;
	    }
	    else
		colors_excess = avail_colors - criteria->min_colors;
	}
	else
	    colors_excess = avail_colors;

	if (PEXUtMinRed &
	    (criteria->hard_criteria_mask | criteria->soft_criteria_mask)) {

	    if (avail_reds < criteria->min_red) {
		p_info->unmet_hard_criteria |= 
		    (criteria->hard_criteria_mask & PEXUtMinRed);
		p_info->unmet_soft_criteria |= 
		    (criteria->soft_criteria_mask & PEXUtMinRed);
		reds_excess = criteria->min_red - avail_reds;
	    }
	    else
		reds_excess =  avail_reds - criteria->min_red;
	}
	else
	    reds_excess = 0;

	if (PEXUtMinGreen &
	    (criteria->hard_criteria_mask | criteria->soft_criteria_mask)) {

	    if (avail_greens < criteria->min_green) {
		p_info->unmet_hard_criteria |= 
		    (criteria->hard_criteria_mask & PEXUtMinGreen);
		p_info->unmet_soft_criteria |= 
		    (criteria->soft_criteria_mask & PEXUtMinGreen);
		greens_excess = criteria->min_green - avail_greens;
	    }
	    else
		greens_excess = avail_greens - criteria->min_green;
	}
	else
	    greens_excess = 0;

	if (PEXUtMinBlue &
	    (criteria->hard_criteria_mask | criteria->soft_criteria_mask)) {

	    if (avail_blues < criteria->min_blue) {
		p_info->unmet_hard_criteria |= 
		    (criteria->hard_criteria_mask & PEXUtMinBlue);
		p_info->unmet_soft_criteria |= 
		    (criteria->soft_criteria_mask & PEXUtMinBlue);
		blues_excess = criteria->min_blue- avail_blues;
	    }
	    else
		blues_excess = avail_blues - criteria->min_blue;
	}
	else
	    blues_excess = 0;

    /*
    -----------------------------------------------------------------------
    Section III:	Compute an overall color rating based on to what
			degree the colors exceed the minima.  If no color
			criteria were specified, this rating comes out zero.

			If sharable_colormap was a criterion, Visuals that
			are not named in properties lose their color rating.
			Visuals that were named in a property, but whose
			ramps do not meet the criteria, also lose.
    -----------------------------------------------------------------------
    */

	total_excess = colors_excess + reds_excess + 
			greens_excess + blues_excess;

	if (total_excess >= 0x10000) 		/* 24 bits */
	    p_info->color_rating = MAX_COLOR_RATING;
	else if (total_excess >= 0x1000) 	/* 16 bits */
	    p_info->color_rating = MAX_COLOR_RATING - 1;
	else if (total_excess >= 0x100) 	/* 12 bits */
	    p_info->color_rating = MAX_COLOR_RATING - 2;
	else 
	    p_info->color_rating = total_excess;

	/*
	    If caller asked for colormap to NOT be sharable, criteria are
	    met, since any Visual supports private colormaps.
	*/

	if  ((PEXUtSharableColormap &
	    (criteria->hard_criteria_mask | criteria->soft_criteria_mask)) &&
	    (criteria->sharable_colormap)) {

	    if ((colors_excess < 0) ||
		(reds_excess < 0) ||
		(greens_excess < 0) ||
		(blues_excess < 0) ||
		(p_info->property_ptr == NULL)) {

		p_info->unmet_hard_criteria |= 
		    (criteria->hard_criteria_mask & PEXUtSharableColormap);
		p_info->unmet_soft_criteria |= 
		    (criteria->soft_criteria_mask & PEXUtSharableColormap);
		p_info->color_rating = 0;
	    }
	}

	return PEXUtSuccess;

} /* pexut_evaluate_color_criteria */


static Bool pexut_is_pex_db_supported(
		dpy, 
		win, 
		db_x )

		Display             *dpy;
		Window              win;
		int                 db_x;
{
    PEXExtensionInfo    *pexinfo;
    XWindowAttributes   wattrs;
    unsigned long       count;
    PEXRenderingTarget  *target;
    
    /*
    ** Verify the PEX server is major version 5.
    */
    pexinfo = PEXGetExtensionInfo( dpy );
    if ( pexinfo == (PEXExtensionInfo *)NULL )
        return( False );
    if ( pexinfo->major_version != 5 )
        return( False );

    /*
    ** Get the window attributes for use in later inquiries.
    */
    if ( ! XGetWindowAttributes( dpy, win, &wattrs ) )
        return( False );

    /*
    ** Try MBX.
    **
    ** Verify rendering to MBX buffers is supported.
    ** Verify the MBX extension is supported and that it is major version 3.
    ** Verify the MBX extension supports at least 2 buffers for this drawable.
    */
    {
        int first_event, first_error, num, dc;
	XmbufBufferInfo  *buf_info, *dc_info;
        
        if ( ( ( pexinfo->minor_version >= 1 ) &&
	       PEXMatchRenderingTargets( dpy, win, wattrs.depth,
                                         PEXBufferDrawable, wattrs.visual, 1,
                                         &count, &target ) &&
               ( count == 1 ) ) ) {
            if ( XmbufQueryExtension( dpy, &first_event, &first_error ) ) {
		if ( XmbufGetScreenInfo( dpy, win, &num, &buf_info,
					 &dc, &dc_info ) ) {
		    if ( buf_info->max_buffers >= 2 )
			return( True );
		}
            }
        }
    }
    
    /*
    ** MBX not available, so try double-buffer escapes.
    **
    ** The escapes are not usable if db_x;
    ** the escapes are not available on pre-5.1 servers (unless server is Sun).
    ** Verify the double-buffer escape is supported.
    */
    if ( ! db_x ) {
        
        if ( ( pexinfo->minor_version == 1 ) ||
	     (strncmp( pexinfo->vendor_name, "SunPEX 2", 8 ) == 0 ) ) {

            int                     enumtypes, i;
            unsigned long           *enum_counts;
            PEXEnumTypeDesc         *enum_info;

            enumtypes = PEXETEscape;
            if ( PEXGetEnumTypeInfo( dpy, win, 1, &enumtypes, PEXETIndex,
                                     &enum_counts, &enum_info ) ) {
                for ( i = 0;  i < *enum_counts;  i++ ) {
                    if ( (short)enum_info[i].index ==
						 (short)ES_ESCAPE_ET_DBLBUFFER )
                        return( True );         /* assume rest of E&S escapes */
                }
            }
        }
    }
    
    /*
    ** Pixmaps are not considered support for double-buffering; rather, pixmaps
    ** are a mechanism for doing smooth animation when double-buffering is not
    ** supported.
    **
    ** However, return True if rendering to pixmaps is supported
    ** this is acceptable because the PEXUtDB utilities will double-buffer
    ** using pixmaps if rendering to pixmaps is supported.
    */
    if ( PEXMatchRenderingTargets( dpy, win, wattrs.depth, PEXPixmapDrawable,
                                   wattrs.visual, 1, &count, &target ) &&
         ( count == 1 ) ) {
	return( True );
    }
    

    /*
    ** No double-buffering is supported.
    */
    return( False );

} /* pexut_is_pex_db_supported */

int pexut_evaluate_double_buffering_capability (
		display,
		criteria,
		vis_info,
		p_info
		)

		Display				*display;
		PEXUtVisualCriteria		*criteria;
		XVisualInfo			*vis_info;
		visual_info_type		*p_info;
{
	int			actual_capabilities;
	Window			window;
	int			db_x;
	int			result;


	if  (PEXUtDoubleBufferingCapability &
	    (criteria->hard_criteria_mask | criteria->soft_criteria_mask)) {

	    /*
		Create a temporary unmapped window in the visual.
	    */
	    pexut_create_temporary_window (display, vis_info, &window);


	    /*
		Determine if the window's visual supports the desired
		capabilities.
	    */

	    db_x = (criteria->double_buffering_capability 
			== PEXUtDbufferPEXAndX);
	    result = pexut_is_pex_db_supported (display, window, db_x);
	    if (result)
		actual_capabilities = db_x ? 	PEXUtDbufferPEXAndX: 
						PEXUtDbufferPEX;
	    else
		actual_capabilities = PEXUtDbufferNone;

	    /*
		Destroy the temporary window.
	    */
	    pexut_destroy_temporary_window (display, vis_info, window);

	    if (actual_capabilities != criteria->double_buffering_capability) {
		p_info->unmet_hard_criteria |= 
			(criteria->hard_criteria_mask & 
			PEXUtDoubleBufferingCapability);
		p_info->unmet_soft_criteria |= 
			(criteria->soft_criteria_mask & 
			PEXUtDoubleBufferingCapability);
	    }
	}

	return PEXUtSuccess;

} /* pexut_evaluate_double_buffering_capability */


int pexut_evaluate_color_approx_type (
		display,
		criteria,
		vis_info,
		p_info
		)

		Display				*display;
		PEXUtVisualCriteria		*criteria;
		XVisualInfo			*vis_info;
		visual_info_type		*p_info;
{
	Window			window;
	int			found;
	int			result;


	if (PEXUtColorApproxType &
	    (criteria->hard_criteria_mask | criteria->soft_criteria_mask)) {

	    /*
		Create a temporary unmapped window in the visual.
	    */
	    pexut_create_temporary_window (display, vis_info, &window);

	    /*
		Determine support for the color approx type 
		using GetEnumTypeInfo.
	    */
	    result = pexut_verify_color_approx_type (display, window, 
						criteria->color_approx_type);

	    /*
		Destroy the temporary window.
	    */
	    pexut_destroy_temporary_window (display, vis_info, window);

	    if (result == PEXUtSuccess)
		found = True;
	    else if (result == PEXUtCriteriaFailure)
		found = False;
	    else
		return result;

	    if (!found) {
		p_info->unmet_hard_criteria |= 
		    (criteria->hard_criteria_mask & PEXUtColorApproxType);
		p_info->unmet_soft_criteria |= 
		    (criteria->soft_criteria_mask & PEXUtColorApproxType);
	    }
	}

	return PEXUtSuccess;

} /* pexut_evaluate_color_approx_type */




unsigned int pexut_compute_overall_rating (
		p_info
		)

		visual_info_type	*p_info;
{
	unsigned int rating;

	rating = (p_info->order_rating << ORDER_SHIFT) +
		 (p_info->soft_rating << SOFT_SHIFT) +
		 (p_info->class_rating << CLASS_SHIFT) +
		 (p_info->color_rating << COLOR_SHIFT);

	return rating;

} /* pexut_compute_overall_rating */


int pexut_create_temporary_window (
		display,
		vis_info,
		window_return
		)

		Display				*display;
		XVisualInfo			*vis_info;
		Window				*window_return;
{
	unsigned long 		window_mask;
	XSetWindowAttributes 	window_attrs;
	Colormap		cmap_id;


	/*
	    Create a window using override-redirect.  Do not map it.
	    If the visual is the default, use the root window.
	    Otherwise, create a colormap to use.
	*/

	if (vis_info->visual == DefaultVisual (display, vis_info->screen)) {

	    *window_return = RootWindow (display, vis_info->screen);
	    return PEXUtSuccess;
	}
	else {
	    cmap_id = XCreateColormap (display, 
					RootWindow(display, vis_info->screen),
					vis_info->visual, AllocNone );
	    if (cmap_id == None)
		return PEXUtXFailure;

	    window_attrs.colormap = cmap_id;
	    window_mask = CWColormap;

	    window_attrs.override_redirect = True;
	    window_mask |= CWOverrideRedirect;

	    window_attrs.background_pixel = 0;
	    window_mask |= CWBackPixel;

	    window_attrs.border_pixel = 0;
	    window_mask |= CWBorderPixel;

	    *window_return = XCreateWindow (display, 
				    RootWindow (display, vis_info->screen),
				    10, 10, 1, 1, 0,
				    vis_info->depth, 
				    InputOutput,
				    vis_info->visual,
				    window_mask,
				    &(window_attrs));

	    if (*window_return == None)
		return PEXUtXFailure;
	    else
		return PEXUtSuccess;
	}
	
} /* pexut_create_temporary_window */

int pexut_destroy_temporary_window (
		display,
		vis_info,
		window
		)

		Display				*display;
		XVisualInfo			*vis_info;
		Window				window;
{
	XWindowAttributes	window_attrs;

	/*
	    If this window is not in the default Visual, 
	    the Colormap and Window need to be freed.
	*/

	if (vis_info->visual != DefaultVisual (display, vis_info->screen)) {

	    if (XGetWindowAttributes (display, window, &window_attrs))
		XFreeColormap (display, window_attrs.colormap);

	    XDestroyWindow (display, window);
	}

        return PEXUtSuccess;

} /* pexut_destroy_temporary_window */


int pexut_verify_color_approx_type
#if NeedFunctionPrototypes
		(
		Display				*display,
		Window				window,
		PEXEnumTypeIndex		color_approx_type
		)
#else
		(
		display,
		window,
		color_approx_type
		)

		Display				*display;
		Window				window;
		PEXEnumTypeIndex		color_approx_type;
#endif
{
	int			enum_types[1];
	unsigned long 		*count;
	PEXEnumTypeDesc		*enum_data;
	int			enum_index;
	int			found;

	/*
	    Verify that specified color approx type is supported
	    using PEXGetEnumTypeInfo.  If not, return failure.
	*/

	enum_types[0] = PEXETColorApproxType;
	if (!PEXGetEnumTypeInfo (display, window, 1, enum_types, PEXETIndex, 
				&count, &enum_data)) {

	    return PEXUtPEXFailure;
	}

	found = False;
	for (enum_index = 0; enum_index < *count; enum_index++) {
	    if (enum_data[enum_index].index == color_approx_type) {
		found = True;
		break;
	    }
	}
	PEXFreeEnumInfo (1, count, enum_data);

	if (found)
	    return PEXUtSuccess;
	else
	    return PEXUtCriteriaFailure;

} /* pexut_verify_color_approx_type */


int pexut_synthesize_cmap_from_visual (
		vis_info,
		cmap_info_return
		)

		XVisualInfo			*vis_info;
		XStandardColormap		*cmap_info_return;
{
	int 		num_reds, red_shift, red_mult;
	int 		num_greens, green_shift, green_mult;
	int 		num_blues, blue_shift, blue_mult;
	int 		base_pixel;

	unsigned long 	temp;
	int		depth;
	int		trial_value, overrun;


	switch (vis_info->class) {

	case PseudoColor:
	case StaticColor:
	    /* 
		Divide the number of planes into thirds, and then favor
		equal numbers of red and green planes at the expense
		of blue planes.  Favor green at the expense of red in tight
		situations.  This yields 121 in depth 4, 332 in depth 8,
		444 in depth 12, and 888 in depth 24.
	    */

	    depth = vis_info->depth;
	    trial_value = depth / 3;
	    overrun = depth % 3;
	    if (overrun)
		trial_value += 1;
	    num_greens = (1 << trial_value);
	    num_reds = (1 << trial_value);
	    if ((num_reds * num_greens) >= vis_info->colormap_size)
		num_reds = (1 << (trial_value-1));
	    num_blues = vis_info->colormap_size / (num_reds * num_greens);
	    red_mult = num_greens * num_blues;
	    green_mult = num_blues;
	    blue_mult = 1;
	    base_pixel = 0;
	    break;

	case GrayScale:
	case StaticGray:

	    /*
		PEXColorRange is advantageous in gray Visuals 
		since it uses less cells to achieve the same effect.
		Here, use the entire colormap for gray levels.
	    */

	    num_reds = vis_info->colormap_size;
	    num_greens = vis_info->colormap_size;
	    num_blues = vis_info->colormap_size;
	    red_mult = 1;
	    green_mult = 1;
	    blue_mult = 1;
	    base_pixel = 0;
	    break;

	case DirectColor:
	case TrueColor:

	    /* 
		Determine the maximum number of color values for each of
		red, green, and blue, and the corresponding shifts.
	    */
	    num_reds = 1;
	    red_shift = 0;
	    for (temp = vis_info->red_mask; temp > 0; temp >>= 1) {
		if (temp & 1)
		    num_reds *= 2;
		else
		    red_shift++;
	    }
	    red_mult = 1 << red_shift;

	    num_greens = 1;
	    green_shift = 0;
	    for (temp = vis_info->green_mask; temp > 0; temp >>= 1) {
		if (temp & 1)
		    num_greens *= 2;
		else
		    green_shift++;
	    }
	    green_mult = 1 << green_shift;

	    num_blues = 1;
	    blue_shift = 0;
	    for (temp = vis_info->blue_mask; temp > 0; temp >>= 1) {
		if (temp & 1)
		    num_blues *= 2;
		else
		    blue_shift++;
	    }
	    blue_mult = 1 << blue_shift;

	    base_pixel = 0;
	    break;
	}

	cmap_info_return->colormap = None;
	cmap_info_return->visualid = vis_info->visualid;
	cmap_info_return->killid = None;

	cmap_info_return->red_max = num_reds - 1;
	cmap_info_return->green_max = num_greens - 1;
	cmap_info_return->blue_max = num_blues - 1;
	cmap_info_return->red_mult = red_mult;
	cmap_info_return->green_mult = green_mult;
	cmap_info_return->blue_mult = blue_mult;
	cmap_info_return->base_pixel = base_pixel;

	return PEXUtSuccess;

} /* pexut_synthesize_cmap_from_visual */


int pexut_load_color_approx_from_std_cmap (
		color_approx_type,
		vis_info,
		cmap_info,
		capx_info_return
		)

		int				color_approx_type;
		XVisualInfo			*vis_info;
		XStandardColormap		*cmap_info;
		PEXColorApproxEntry		*capx_info_return;
{
	/*
	    If color approx type was not specified, decide it
	    based on the Visual class.
	*/

	if (color_approx_type == -1) {

	    if ((vis_info->class == GrayScale) ||
		(vis_info->class == StaticGray))
		color_approx_type = PEXColorRange;
	    else
		color_approx_type = PEXColorSpace;
	}


	/*
	    Transfer information from the standard colormap
	    property structure into the color approximation entry.
	*/

	if (color_approx_type == PEXColorRange) {

	    capx_info_return->type = PEXColorRange;
	    capx_info_return->model = PEXColorApproxRGB;
	    capx_info_return->dither = PEXOn;
	    capx_info_return->base_pixel = 0;
	    capx_info_return->max1 = vis_info->colormap_size-1;
	    capx_info_return->max2 = 0;
	    capx_info_return->max3 = 0;
	    capx_info_return->weight1 = 0.299;
	    capx_info_return->weight2 = 0.587;
	    capx_info_return->weight3 = 0.114;
	    capx_info_return->mult1 = 1;
	    capx_info_return->mult2 = 0;
	    capx_info_return->mult3 = 0;
	}
	else {

	    capx_info_return->type = PEXColorSpace;
	    capx_info_return->model = PEXColorApproxRGB;
	    capx_info_return->dither = PEXOn;
	    capx_info_return->base_pixel = cmap_info->base_pixel;
	    capx_info_return->max1 = cmap_info->red_max;
	    capx_info_return->max2 = cmap_info->green_max;
	    capx_info_return->max3 = cmap_info->blue_max;
	    capx_info_return->weight1 = 0.0;
	    capx_info_return->weight2 = 0.0;
	    capx_info_return->weight3 = 0.0;
	    capx_info_return->mult1 = cmap_info->red_mult;
	    capx_info_return->mult2 = cmap_info->green_mult;
	    capx_info_return->mult3 = cmap_info->blue_mult;
	}

	return PEXUtSuccess;

} /* pexut_load_color_approx_from_std_cmap */


int pexut_match_color_approx_entry (
		entry1,
		entry2
		)

		PEXColorApproxEntry		*entry1;
		PEXColorApproxEntry		*entry2;
{
	int match = 1;


	match &= (entry1->type == entry2->type);
	match &= (entry1->model == entry2->model);
	match &= (entry1->base_pixel == entry2->base_pixel);
	match &= (entry1->max1 == entry2->max1);

	if (entry1->type == PEXColorSpace) {
	    match &= (entry1->mult1 == entry2->mult1);
	    match &= (entry1->max2 == entry2->max2);
	    match &= (entry1->mult2 == entry2->mult2);
	    match &= (entry1->max3 == entry2->max3);
	    match &= (entry1->mult3 == entry2->mult3);
	}
	else if (entry1->type == PEXColorRange) {
	    /*
		Issue:  how flexible are implementations on the
		values of mults or of weights?
		We'll be lax and pass everything.
	    */
	    ;
	}

	if (match)
	    return True;
	else
	    return False;

} /* pexut_match_color_approx_entry */


int pexut_create_one_channel_map (
		display,
		vis_info,
		capx_info,
		colormap_id_return
		)

		Display				*display;
		XVisualInfo			*vis_info;
		PEXColorApproxEntry		*capx_info;
		Colormap			*colormap_id_return;

/*
    Algorithm:

	Section I:	Create a Colormap and capture the default values.
			Compute the number of cells in the color 
			approximation region.
	
	Section II:	Allocate all the cells in the Colormap read-write.
			Load the color approximation region with the
			correct colors.
*/
{
	Colormap	cmap_id;
	int		num_reds, num_greens, num_blues, num_colors;
	unsigned long	*pixels, pixel_value;
	XColor		*colors, *p_color;
	int		i, j, k;
	int		result;


    /*
    -----------------------------------------------------------------------
    Section I:		Create a Colormap and capture the default values.
			Compute the number of cells in the color 
			approximation region.
    -----------------------------------------------------------------------
    */

	/*
	    Create the colormap with AllocNone.
	*/

	cmap_id = XCreateColormap ( display, 
				    RootWindow (display, vis_info->screen),
				    vis_info->visual, AllocNone );
	if (cmap_id == None)
	    return PEXUtXFailure;


	/*
	    If color approx type is PEXColorRange, compute the number of 
	    cells from max1, typical for GrayScale.

	    If color approx type is PEXColorSpace, compute the number of 
	    cells from all three maxes, typical for PseudoColor.
	*/

	if (capx_info->type == PEXColorRange) {

	    num_colors = capx_info->max1 + 1;
	}
	else { /* assume PEXColorSpace */

	    num_reds = capx_info->max1 + 1;
	    num_greens = capx_info->max2 + 1;
	    num_blues = capx_info->max3 + 1;
	    num_colors = num_reds * num_greens * num_blues;
	}

	if (capx_info->base_pixel >= vis_info->colormap_size) {
	    XFreeColormap (display, cmap_id);
	    return PEXUtXFailure;
	}
	if (capx_info->base_pixel + num_colors > vis_info->colormap_size)
	    num_colors = vis_info->colormap_size - capx_info->base_pixel;


	/*
	    Query the default colors, or the default Colormap if we're in
	    that Visual; we will preserve the colors in cells outside the ramp.
	*/

	colors = (XColor *) malloc (vis_info->colormap_size * sizeof(XColor));

	if (colors == NULL) {
	    XFreeColormap (display, cmap_id);
	    return PEXUtXFailure;
	}

	for (pixel_value = 0, p_color = colors; 
	    pixel_value < vis_info->colormap_size;
	    pixel_value++, p_color++) {

	    p_color->pixel = pixel_value;
	}

	if (vis_info->visualid == 
	    XVisualIDFromVisual(DefaultVisual(display, vis_info->screen)))
	    XQueryColors (display, DefaultColormap(display, vis_info->screen), 
			colors, vis_info->colormap_size);
	else
	    XQueryColors (display, cmap_id, 
			colors, vis_info->colormap_size);


    /*
    -----------------------------------------------------------------------
    Section II:		Allocate all the cells in the Colormap read-write.
			Load the color approximation region with the
			correct colors.
    -----------------------------------------------------------------------
    */

	/*
	    Allocate all cells read/write using XAllocColorCells.
	*/

	pixels = (unsigned long *) 
		    malloc (vis_info->colormap_size * sizeof(unsigned long));
	if (pixels == NULL) {
	    XFreeColormap (display, cmap_id);
	    return PEXUtXFailure;
	}

	result = XAllocColorCells ( display, cmap_id, True,
				    (unsigned long *) NULL, 0, 
				    pixels, vis_info->colormap_size);
	free (pixels);
	if (! result) {
	    XFreeColormap (display, cmap_id);
	    return PEXUtXFailure;
	}


	/*
	    Load the colors into the cells.  Even if the visual
	    is PseudoColor, if the color approx type is PEXColorRange,
	    we will load a ramp of gray values (since we have no idea
	    what the colors should be).  Similarly, even for GrayScale,
	    PEXColorSpace will load a color space sampling.
	*/

	for (pixel_value = 0, p_color = colors;
	    pixel_value < capx_info->base_pixel;
	    pixel_value++, p_color++) {

	    p_color->flags = DoRed | DoGreen | DoBlue;
	}

	if (capx_info->type == PEXColorRange) {

	    pixel_value = capx_info->base_pixel;
	    p_color = colors + capx_info->base_pixel;
	    for (i = 0; i < num_colors; i++) {

		p_color->flags = DoRed | DoGreen | DoBlue;
		p_color->pixel = pixel_value++;
		p_color->red =
		p_color->green =
		p_color->blue = i * 65535 / (num_colors - 1);
		p_color++;
	    }
	}
	else { /* assume PEXColorSpace */

	    pixel_value = capx_info->base_pixel;
	    p_color = colors + capx_info->base_pixel;

	    /*
		The following logic can be coded more generally 
		but hasn't been, yet.
	    */

	    if (capx_info->mult1 > capx_info->mult3) {
		if (capx_info->mult2 > capx_info->mult1) {

		    /* 
			Set up GBR ramp, as on Sun.
		    */
		    for (i = 0; (i < num_greens) && 
			(pixel_value < vis_info->colormap_size); i++) {
			for (j = 0; (j < num_blues) && 
			    (pixel_value < vis_info->colormap_size); j++) {
			    for (k = 0; (k < num_reds) && 
				(pixel_value < vis_info->colormap_size); k++) {

				p_color->flags = DoRed | DoGreen | DoBlue;
				p_color->pixel = pixel_value++;
				p_color->red = (unsigned short)(k * 65535) / capx_info->max1;
				p_color->green = (unsigned short)(i * 65535) / capx_info->max2;
				p_color->blue = (unsigned short)(j * 65535) / capx_info->max3;
				p_color++;

				if (pixel_value == vis_info->colormap_size) 
				    break;
			    }
			}
		    }
		}
		else {

		    /* 
			Set up RGB ramp, as on HP, DEC, Kubota Pacific, 
			Oki, some NCD.
		    */
		    for (i = 0; (i < num_reds) && 
			(pixel_value < vis_info->colormap_size); i++) {
			for (j = 0; (j < num_greens) && 
			    (pixel_value < vis_info->colormap_size); j++) {
			    for (k = 0; (k < num_blues) && 
				(pixel_value < vis_info->colormap_size); k++) {

				p_color->flags = DoRed | DoGreen | DoBlue;
				p_color->pixel = pixel_value++;
				p_color->red = (unsigned short)(i * 65535) / capx_info->max1;
				p_color->green = (unsigned short)(j * 65535) / capx_info->max2;
				p_color->blue = (unsigned short)(k * 65535) / capx_info->max3;
				p_color++;

				if (pixel_value == vis_info->colormap_size) 
				    break;
			    }
			}
		    }
		}
	    }
	    else {

		/* 
		    Set up BGR ramp, as on IBM, SHOgraphics, Tektronix, 
		    some NCD.
		*/
		for (i = 0; (i < num_blues) && 
			(pixel_value < vis_info->colormap_size); i++) {
		    for (j = 0; (j < num_greens) && 
			    (pixel_value < vis_info->colormap_size); j++) {
			for (k = 0; (k < num_reds) && 
				(pixel_value < vis_info->colormap_size); k++) {

			    p_color->flags = DoRed | DoGreen | DoBlue;
			    p_color->pixel = pixel_value++;
			    p_color->red = (unsigned short)(k * 65535) / capx_info->max1;
			    p_color->green = (unsigned short)(j * 65535) / capx_info->max2;
			    p_color->blue = (unsigned short)(i * 65535) / capx_info->max3;
			    p_color++;

			    if (pixel_value == vis_info->colormap_size) break;
			}
		    }
		}
	    }
	}

	for (pixel_value = capx_info->base_pixel + num_colors, 
	    p_color = colors;
	    pixel_value < vis_info->colormap_size;
	    pixel_value++, p_color++) {

	    p_color->flags = DoRed | DoGreen | DoBlue;
	}

	XStoreColors (display, cmap_id, colors, vis_info->colormap_size);
	free (colors);


	/*
	    Optional:  could free cells outside the ramp at this
	    point, and/or reallocate cells as read-only so 
	    XAllocColor might work.  At the very least, should
	    make sure that black and white can be found.  However,
	    currently leaving implementation of policy to the caller.
	    See PEXUtCopyColormapAsReadOnly().
	*/

	*colormap_id_return = cmap_id;
	return PEXUtSuccess;

} /* pexut_create_one_channel_map */


int pexut_create_three_channel_map (
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
	Colormap	cmap_id;
	int		num_colors;
	int		num_reds, num_greens, num_blues;
	int		red_shift, green_shift, blue_shift;
	int		vis_red_shift, vis_green_shift, vis_blue_shift;
	unsigned long	temp;
	unsigned long	*pixels;
	XColor		*colors, *p_color;
	int		i;
	int		result;


	/*
	    Create the colormap with AllocNone.
	*/

	cmap_id = XCreateColormap ( display, 
				    RootWindow (display, vis_info->screen),
				    vis_info->visual, AllocNone );
	if (cmap_id == None)
	    return PEXUtXFailure;


	/*
	    Compute the color shifts for r, g, b.
	    Making an major assumption that the number of levels of
	    each component are all powers of two.
	    Compute the number of cells to write as the maximum
	    count for any of the three channels.
	*/

	num_reds = capx_info->max1 + 1;
	for (red_shift = 0, temp = capx_info->mult1;
	    temp > 1; red_shift++, temp >>= 1);
	vis_red_shift = 0;
	temp = vis_info->red_mask;
	while (!(temp & 0x1)) {
	    vis_red_shift++;
	    temp >>= 1;
	}

	num_greens = capx_info->max2 + 1;
	for (green_shift = 0, temp = capx_info->mult2;
	    temp > 1; green_shift++, temp >>= 1);
	vis_green_shift = 0;
	temp = vis_info->green_mask;
	while (!(temp & 0x1)) {
	    vis_green_shift++;
	    temp >>= 1;
	}

	num_blues = capx_info->max3 + 1;
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
	    XFreeColormap (display, cmap_id);
	    return PEXUtXFailure;
	}
	

	num_colors = num_reds;
	if (num_greens > num_colors) num_colors = num_greens;
	if (num_blues > num_colors) num_colors = num_blues;

	if (capx_info->base_pixel >= vis_info->colormap_size) {
	    XFreeColormap (display, cmap_id);
	    return PEXUtXFailure;
	}
	if (capx_info->base_pixel + num_colors > vis_info->colormap_size)
	    num_colors = vis_info->colormap_size - capx_info->base_pixel;


	/*
	    Allocate all cells read/write using XAllocColorCells.
	*/

	pixels = (unsigned long *) 
		    malloc (vis_info->colormap_size * sizeof(unsigned long));
	if (pixels == NULL) {
	    XFreeColormap (display, cmap_id);
	    return PEXUtXFailure;
	}

	result = XAllocColorCells ( display, cmap_id, True,
				    (unsigned long *) NULL, 0, 
				    pixels, vis_info->colormap_size);
	free (pixels);
	if (! result) {
	    XFreeColormap (display, cmap_id);
	    return PEXUtXFailure;
	}


	/*
	    Load the colors into the cells.
	*/

	colors = (XColor *) malloc (num_colors * sizeof(XColor));

	if (colors == NULL) {
	    XFreeColormap (display, cmap_id);
	    return PEXUtXFailure;
	}

	for (i = 0, p_color = colors; i < num_colors; i++, p_color++) {

	    p_color->flags = 0;
	    p_color->pixel = capx_info->base_pixel;

	    if (i < num_reds) {
		p_color->flags |= DoRed;
		p_color->pixel |= (i << red_shift);
		p_color->red = (unsigned short)(i * 65535) / capx_info->max1;
	    }

	    if (i < num_greens) {
		p_color->flags |= DoGreen;
		p_color->pixel |= (i << green_shift);
		p_color->green = (unsigned short)(i * 65535) / capx_info->max2;
	    }

	    if (i < num_blues) {
		p_color->flags |= DoBlue;
		p_color->pixel |= (i << blue_shift);
		p_color->blue = (unsigned short)(i * 65535) / capx_info->max3;
	    }
	}

	XStoreColors (display, cmap_id, colors, num_colors);
	free (colors);

	/*
	    Optional:  could free cells outside the ramp at this
	    point, and/or reallocate cells as read-only so 
	    XAllocColor might work.  At the very least, should
	    make sure that black and white can be found.  However,
	    currently leaving implementation of policy to the caller.
	    See PEXUtCopyColormapAsReadOnly().
	*/


	*colormap_id_return = cmap_id;
	return PEXUtSuccess;

} /* pexut_create_three_channel_map */


int pexut_create_read_only_map (
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
	/*
	    Create the colormap with AllocNone.
	*/

	*colormap_id_return = XCreateColormap ( display, 
				    RootWindow (display, vis_info->screen),
				    vis_info->visual, AllocNone );

	/*
	    Optional:  could verify that the ramp is actually represented
	    in the colormap, and return an error if not.  Left as an exercise.
        */


	if (*colormap_id_return == None)
	    return PEXUtXFailure;
	else
	    return PEXUtSuccess;

} /* pexut_create_read_only_map */


int pexut_count_bits_in_mask (
		mask
		)

		unsigned int mask;
{
    int count = 0;

    while (mask) {
	if (mask & 0x1) count++;
	mask >>= 1;
    }

    return count;

} /* pexut_count_bits_in_mask */

