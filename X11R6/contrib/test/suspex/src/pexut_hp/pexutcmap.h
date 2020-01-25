/* $XConsortium: pexutcmap.h,v 1.0 93/11/22 12:23:29 rws Exp $ */

#ifndef PEXUTCMAP_H /* { */
#define PEXUTCMAP_H

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
/* $Source: /xsrc/xc/unsupported/test/suspex/src/pexut_hp/RCS/pexutcmap.h,v $                     	      */
/* $Date: 93/11/22 12:23:29 $                                                 */
/* $Revision: 1.0 $                                                   */
/*                                                                            */
/* Description:                                                               */
/*   Interface header file for PEXUt colormap/visual utilities.               */
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
	The following structure type is used to specify criteria for
	selecting a Visual.  The fields are used as follows:

	hard_criteria_mask	a mask of bits for "hard" criteria; that is,
				for criteria that MUST be met by the Visual;
				the supported bits are defined below.

	soft_criteria_mask	a mask of bits for "soft" criteria, i.e.
				criteria that are not required to be met but
				are "preferred"; supported bits are defined
				below.

	depth			the criterion value for PEXUtDepth; allowed
				value is any reasonable Visual depth.
	
	min_colors		criterion value for PEXUtMinColors; specifies
				the minimum number of distinct colors the
				Visual must support.  Values less than two
				are illogical, as even monochrome systems have
				two colors.  The way in which the available 
				number of colors is computed depends
				on the Visual class.  When a standard colormap
				property entry is used, the number of available
				colors is based on the ramp described in the
				property.

	min_red			criterion value for (PEXUtMinRed,PEXUtMinGreen,
	min_green		PEXUtMinBlue) respectively; specify the
	min_blue		minimum number of (red,green,blue) levels the 
				Visual must support.  Allowed values are 
				integer greater than one.

	visual_class		criterion value for PEXUtVisualClass; specifies
				the X Visual class (one of StaticColor, 
				PseudoColor, StaticGray, GrayScale, 
				TrueColor, DirectColor).

	layer			criterion value for PEXUtLayer; one of
				PEXUtOverlay or PEXUtImage.

	standard_colormap_property 	criterion value for 
				PEXUtStandardColormapProperty; one of True
				or False.  If True, the Visual must be named
				in an entry in a standard colormap property
				in order to satisfy the criterion.

	sharable_colormap	criterion value for PEXUtSharableColormap;
				one of True or False.  If True, the Visual
				must be named in a standard colormap property,
				and furthermore, such Visuals will be favored
				over Visuals that have higher color resolution.

	double_buffering_capability	criterion value for
				PEXUtDoubleBufferingCapability; one of
				PEXUtDbufferNone, PEXUtDbufferPEX, or
				PEXUtDbufferPEXAndX.  Note that specifying
				PEXUtDbufferNone "hard" may cause the "best"
				Visual for PEX rendering to be rejected, since
				it is likely to be double-buffering-capable.

	color_approx_type	criterion value for PEXUtColorApproxType;
				one of PEXColorSpace or PEXColorRange.
				Typically this criterion is only specified if
				it is PEXColorRange, since most applications
				need PEXColorSpace.  If specified, the 
				Visual must be capable of the specifed
				approximation type.
*/


typedef struct {
	unsigned int		hard_criteria_mask;
	unsigned int		soft_criteria_mask;
	unsigned int		depth;
	int			min_colors;
	int			min_red;
	int			min_green;
	int			min_blue;
	int			visual_class;
	int			layer;
	int			standard_colormap_property;
	int			sharable_colormap;
	int			double_buffering_capability;
	PEXEnumTypeIndex	color_approx_type;
} PEXUtVisualCriteria;

/*
	Mask bits for the criteria.
*/

#define PEXUtDepth			(1L<<0)
#define PEXUtMinColors			(1L<<1)
#define PEXUtMinRed			(1L<<2)
#define PEXUtMinGreen			(1L<<3)
#define PEXUtMinBlue			(1L<<4)
#define PEXUtVisualClass		(1L<<5)
#define PEXUtLayer			(1L<<6)
#define PEXUtStandardColormapProperty	(1L<<7)
#define PEXUtSharableColormap		(1L<<8)
#define PEXUtDoubleBufferingCapability	(1L<<9)
#define PEXUtColorApproxType		(1L<<10)
#define PEXUtAllCriteria		(   				\
					PEXUtDepth 			|\
					PEXUtMinColors 			|\
					PEXUtMinRed 			|\
					PEXUtMinGreen 			|\
					PEXUtMinBlue 			|\
					PEXUtVisualClass 		|\
					PEXUtLayer 			|\
					PEXUtStandardColormapProperty 	|\
					PEXUtSharableColormap 		|\
					PEXUtDoubleBufferingCapability 	|\
					PEXUtColorApproxType 		\
					)


/*
	Values for the layer criterion.
*/

#define PEXUtOverlay			1
#define PEXUtImage			2


/*
	Values for the double_buffering_capability crierion.
*/

#define PEXUtDbufferNone		0
#define PEXUtDbufferPEX			1
#define PEXUtDbufferPEXAndX		2


/*
	Values for the integer error codes 
	returned by the various utilities.
*/

#define PEXUtModifiedResource		1
#define PEXUtUnmodifiedResource		0
#define PEXUtQualifiedSuccess		1
#define PEXUtSuccess			0
#define PEXUtCriteriaFailure		-1
#define PEXUtXFailure			-2
#define PEXUtPEXFailure			-3
#define PEXUtAllocFailure		-4



/*
	The following structure type is used to specify window size,
	location, title, and other attributes needed for window
	creation.  The fields are used as follows:

	attr_mask		mask indicating which attribute values in
				the attrs substructure are to be used;
				basically the same attribute mask that
				would be passed into XCreateWindow, i.e. the
				bits are CWColormap, CWBorderPixel, etc.

	attrs			attribute values corresponding to attr_mask
				bits.  Used the same as in XCreateWindow,
				with the following restrictions:

				PEXUtCreateWindow requires the colormap
				field to be set, and CWColormap to be present
				in attr_mask.  PEXUtCreateWindowAndColormap
				ignores any colormap since it finds or
				creates one.

				Background_pixel and border_pixel are
				signficant to PEXUtCreateWindow, but if not
				present, that utility will look up color values
				from the Colormap for the window.
				PEXUtCreateWindowAndColormap always looks up
				colors so these fields are ignored.

				Background_pixmap and border_pixmap are
				effective, but may not be compatible with a
				Colormap that is appropriate for PEX.

				Backing store fields should be used with
				caution, since many PEX implementations
				do not support backing store for PEX rendering.

	title			a character string to be assigned as
				the title of the window.

	size_hints		a structure specfying the size and position
				of the window, both used in a call to
				XCreateWindow and passed on to the window
				manager via XSetNormalHints.  Fields x, y,
				width, and height should be specified.

	parent			identifier of the parent window.

	border_width		width of the window border in pixels.

	background_color_name	a character string naming a color in the
				X color database.  This name will be looked
				up to derive a background color.  If 
				CWBackgroundPixel is specified to 
				PEXUtCreateWindow, that pixel value will be
				used instead.  PEXUtCreateWindowAndColormap	
				always looks up the color.

	border_color_name	similar to background_color_name except it
				specifies the border color of the Window.
*/

typedef struct {
	unsigned long		attr_mask;
	XSetWindowAttributes	attrs;
	char			*title;
	XSizeHints		size_hints;
	Window			parent;
	unsigned int		border_width;
	char			*background_color_name;
	char			*border_color_name;
} PEXUtWindowSpecification;



/******************************************************************************
 *
 * Function:
 *   PEXUtFindVisual -- choose a Visual for PEX and return color setup info
 *                                                                            
 * Description:                                                               
 *   This function attempts to find the "best" Visual supported by a PEX server
 *   that meets a set of criteria specified by the caller.  If it successfully
 *   finds a Visual, it returns colormap and color approximation information
 *   that is predicted to be compatible with the Visual and the PEX 
 *   implementation.  However, in some cases the compatibility is not known in
 *   advance.  PEXUtVerifyColorApproximation (q.v.) can be used to check for
 *   support, but may also be unable to determine server support for a 
 *   particular color approximation setup.  The choice of what is "best" is
 *   a policy decision that cannot be controlled by the caller; it is determined
 *   by the ordering of Renderer targets and by PEX interoperability convention.
 *   
 *   Criteria for Visual selection are specified in the PEXUtVisualCriteria
 *   structure.  Criteria can be "hard" or "soft".  Hard criteria must be met
 *   in order for a Visual to be considered as a candidate; if no Visual meets
 *   all hard criteria, the function returns a failure status.  Soft criteria
 *   are considered when choosing among several candidate Visuals that meet
 *   all the hard criteria.  There is an implicit hard criterion that PEX
 *   supports the Visual as a target (this can only be checked on 5.1 and later
 *   servers).  There is an implicit soft criterion of significant weight:
 *   when several Visuals are qualifed by hard criteria, the utility prefers
 *   the Visual that appears earlier in the target list returned 
 *   PEXMatchRenderingTargets (5.1 and later), or earlier in any other list
 *   established by interoperability convention.  (Currently, an earlier
 *   appearance in one of the standard colormap properties that may be 
 *   searched increases the likelihood of selection.)
 *   
 *   Please see the on-line manual page for a complete description of how
 *   to use the criteria structure.
 *   
 * Returns:                                                                   
 *   The function returns a value of PEXUtSuccess if it is completely 
 *   successful in finding a Visual that meets all hard and soft criteria.
 *   PEXUtQualifiedSuccess is returned if the chosen Visual meets all hard
 *   criteria but not all soft criteria.
 *   
 *   A negative return value indicates failure:
 *   
 *   	PEXUtCriteriaFailure means that no Visual was found that meets all
 *	the hard criteria.
 *   
 *   	PEXUtXFailure means that an error was encountered in an Xlib call.
 *   
 *   	PEXUtPEXFailure means that an error occurred during a PEXlib call.
 *   
 *   	PEXUtAllocFailure means that the utility ran out of memory.
 *   
 * On Entry:                                                                 
 *   display		indicates the X connection to use; it is assumed that
 *			PEXInitialize has already been called.
 *                                                                            
 *   screen		is the screen number for which to choose a Visual.
 *                                                                            
 *   criteria		is a pointer to a structure describing the criteria
 *			to be used in Visual selection.
 *                                                                            
 * On Exit:                                                                 
 *   vis_info_return	points to storage allocated by the caller that has 
 *			been written with a description of the chosen Visual.
 *                                                                            
 *   cmap_info_return	points to storage allocated by the caller that has 
 *			been written with a description of an RGB color map
 *			that might be appropriate for PEX rendering.  If a
 *			standard colormap property was used to guide selection,
 *			this is a copy of the entry from the property that
 *			guided the choice, including any Colormap ID that was
 *			present.  Otherwise, it is a description "synthesized"
 *			from the description of the Visual.
 *                                                                            
 *   capx_info_return	points to storage allocated by the caller that has 
 *			been written with a PEX color approximation entry
 *			that is compatible with the Visual and the criteria.
 *			If no color approximation type was specified as a
 *			a criterion, this is typically a PEXColorSpace setup
 *			that matches the cmap_info_return data.  If a color
 *			approximation type was specified, this is an entry
 *			of the appropriate type.
 *                                                                            
 *   unmet_criteria_return	
 *			points to storage allocated by the caller that has 
 *			been written with a combined mask of all hard and
 *			soft criteria that were not met by the chosen Visual.
 *			This parameter is valid when successful or when 
 *			PEXUtCriteriaFailure is returned; it may not valid 
 *			for failure returns.
 *                                                                            
 *   std_prop_atom_return
 *			points to storage allocated by the caller that has 
 *			been written with the Atom for the name of the
 *			standard colormap property that was used in Visual
 *			selection, or None if no property was used.
 *                                                                            
 * Restrictions:                                                              
 *   This function assumes that PEXInitialize has been called for the         
 *   specified display argument.                                              
 *                                                                            
 *   The ability of this procedure to verify that criteria are met depends
 *   on information available from the PEX server, including its support for
 *   various interoperability conventions.  In general, if a hard criterion
 *   cannot be determined to have been satisfied or not, it is considered to
 *   have not been met; if a soft criterion cannot be verified, the utility 
 *   acts as though it had been met but still returns the corresponding bit
 *   in unmet_criteria_return.
 *                                                                            
 ******************************************************************************/

extern int PEXUtFindVisual (
#if NeedFunctionPrototypes
		Display				*display,
		int				screen,
		PEXUtVisualCriteria		*criteria,
		XVisualInfo			*vis_info_return,
		XStandardColormap		*cmap_info_return,
		PEXColorApproxEntry		*capx_info_return,
		unsigned int			*unmet_criteria_return,
		Atom				*std_prop_atom_return
#endif
		);


/******************************************************************************
 *
 * Function:
 *   PEXUtGetStandardColormap -- retrieve or generate PEX color information
 *                               for a specified Visual
 *                                                                            
 * Description:                                                               
 *   This function is intended to be used by applications that choose a
 *   Visual by means other than PEXUtFindVisual, but that wish to make use
 *   of PEXUt functions for initializing Colormaps and/or creating Windows.
 *   
 *   Given a Visual chosen by the caller, and presumed to support PEX,
 *   this function attempts to fetch color map and PEX color approximation
 *   information using standard colormap properties as prescribed by PEX
 *   interoperability conventions.  If it cannot find any such information,
 *   it "synthesizes" the information from the Visual description.
 *   
 * Returns:                                                                   
 *   The function returns a value of PEXUtSuccess if it succeeds in
 *   finding or generating the color information.
 *   
 *   A negative return value indicates failure:
 *   
 *   	PEXUtXFailure means that an error was encountered in an Xlib call.
 *   
 *   	PEXUtAllocFailure means that the utility ran out of memory.
 *   
 * On Entry:                                                                 
 *   display		indicates the X connection to use; it is assumed that
 *			PEXInitialize has already been called.
 *                                                                            
 *   vis_info		is a pointer to a structure describing the Visual.
 *                                                                            
 * On Exit:                                                                 
 *   cmap_info_return	points to storage allocated by the caller that has 
 *			been written with a description of an RGB color map
 *			that might be appropriate for PEX rendering.  If a
 *			standard colormap property entry was found,
 *			this is a copy of the entry from the property,
 *			including any shared Colormap ID that was
 *			present.  Otherwise, it is a description "synthesized"
 *			from the description of the Visual.
 *                                                                            
 *   capx_info_return	points to storage allocated by the caller that has 
 *			been written with a PEX color approximation entry
 *			that is compatible with the Visual.  This is typically 
 *			a PEXColorSpace setup that matches the cmap_info_return
 *			data.  If a color approximation type was specified, 
 *			this is an entry of the appropriate type.
 *                                                                            
 *   std_prop_atom_return
 *			points to storage allocated by the caller that has 
 *			been written with the Atom for the name of the
 *			standard colormap property that was found, if any,
 *			or None if no property entry was found.
 *                                                                            
 ******************************************************************************/

extern int PEXUtGetStandardColormapInfo (
#if NeedFunctionPrototypes
		Display				*display,
		XVisualInfo			*vis_info,
		XStandardColormap		*cmap_info_return,
		PEXColorApproxEntry		*capx_info_return,
		Atom				*std_prop_atom_return
#endif
		);


/******************************************************************************
 *
 * Function:
 *   PEXUtVerifyColorApproximation -- determine PEX server support for a
 *                                    particular color approximation entry
 *                                                                            
 * Description:                                                               
 *   This function attempts to verify that a specified PEX color approximation
 *   table entry is supported by the PEX server on a specified Visual.
 *   It uses information about color approximation provided according to
 *   interoperability conventions to make this determination.  (PEX 5.1 has
 *   no inquiries that are sufficient to make the determination.)
 *   
 * Returns:                                                                   
 *   The function returns a value of PEXUtSuccess if it is completely 
 *   successful in determining that the color approximation entry is
 *   supported.  PEXUtQualifiedSuccess is returned if support/non-support
 *   cannot be determined.
 *   
 *   A negative return value indicates failure:
 *   
 *   	PEXUtCriteriaFailure means that the utility was able to determine
 *	that the approximation entry is definitely not supported.
 *   
 *   	PEXUtXFailure means that an error was encountered in an Xlib call.
 *   
 *   	PEXUtPEXFailure means that an error occurred during a PEXlib call.
 *   
 *   	PEXUtAllocFailure means that the utility ran out of memory.
 *   
 * On Entry:                                                                 
 *   display		indicates the X connection to use; it is assumed that
 *			PEXInitialize has already been called.
 *                                                                            
 *   capx_info		is a pointer to the PEXColorApproxEntry of interest.
 *                                                                            
 *   vis_info		is a pointer to a structure describing the Visual.
 *                                                                            
 * Restrictions:                                                              
 *   This function assumes that PEXInitialize has been called for the         
 *   specified display argument.                                              
 *                                                                            
 *   The ability of this procedure to verify that criteria are met depends
 *   on information available from the PEX server, including its support for
 *   various interoperability conventions.  If support cannot be determined,
 *   PEXUtQualifiedSuccess is returned so that the application can choose
 *   to go on to attempt a call to PEXSetTableEntries.  The application should
 *   be prepared for an error to be returned during that call in case the
 *   color approximation entry is, in fact, not supported.
 *                                                                            
 ******************************************************************************/

extern int PEXUtVerifyColorApproximation (
#if NeedFunctionPrototypes
		Display				*display,
		PEXColorApproxEntry		*capx_info,
		XVisualInfo			*vis_info
#endif
		);


/******************************************************************************
 *
 * Function:
 *   PEXUtCreateWriteableColormap -- create a read-write Colormap and 
 *				     initialize it for use with a PEX 
 *				     color approximation entry
 *                                                                            
 * Description:                                                               
 *   This function creates a private X Colormap.  The region of the Colormap
 *   that is described in a specified PEX color approximation entry is 
 *   initialized according to the entry; cells outside the region are left
 *   with their default values, in order to reduce color flashing of X clients
 *   that are already using those cells when the new Colormap is installed.
 *   In the case of a PEXColorRange entry, the region is initialized to a
 *   gray ramp.  All cells in the Colormap are read-write for the caller.
 *   
 *   This function should be used by applications that plan to modify cells
 *   in the Colormap, either inside or outside the color approximation region.
 *   For example, if a PEXColorRange entry is to be used, it may be that the
 *   application needs to replace the gray ramp with another set of colors.
 *   If the application only needs to set up a Colormap to match a PEXColorSpace
 *   entry and will not be changing the Colormap contents, it is recommended
 *   that PEXUtCreateColormap be used instead, because it improves the chances
 *   of XAllocColor succeeding on the Colormap.
 *   
 *   Note that some implementations of PEX may require additional adjustments
 *   to the Colormap contents for some configurations.  It is recommended that
 *   PEXUtModifyColormapForVendorDependencies be called after all changes to
 *   the color approximation region are completed.
 *   
 * Returns:                                                                   
 *   The function returns a value of PEXUtSuccess if the Colormap is
 *   successfully allocated and initialized.
 *   
 *   A negative return value indicates failure:
 *   
 *   	PEXUtXFailure means that an error was encountered in an Xlib call.
 *   
 *   	PEXUtAllocFailure means that the utility ran out of memory.
 *   
 * On Entry:                                                                 
 *   display		indicates the X connection to use.
 *   
 *   vis_info		is a pointer to a structure describing the Visual.
 *                                                                            
 *   capx_info		is a pointer to the PEXColorApproxEntry of interest.
 *                                                                            
 * On Exit:                                                                 
 *   colormap_id_return	points to storage allocated by the caller that has 
 *			been written with the new Colormap ID.
 *                                                                            
 ******************************************************************************/

extern int PEXUtCreateWriteableColormap (
#if NeedFunctionPrototypes
		Display				*display,
		XVisualInfo			*vis_info,
		PEXColorApproxEntry		*capx_info,
		Colormap			*colormap_id_return
#endif
		);


/******************************************************************************
 *
 * Function:
 *   PEXUtModifyColormapForVendorDependencies -- adjust Colormap contents for
 *                           vendor, device, and configuration idiosyncrasies
 *                                                                            
 * Description:                                                               
 *   This procedure determines if the Visual, PEX color approximation entry, 
 *   and vendor-dependent server and device form a combination that requires
 *   adjustment to the Colormap contents after it has been initialized
 *   according to the standard and interoperability conventions.  Code for
 *   this procedure is expected to be composed of sections contributed by
 *   vendors that have such combinations, so that interoperability is 
 *   well-supported.  If the combination requires adjustment, this procedure
 *   directly modifies the Colormap.  Therefore, the Colormap cells must be
 *   writeable for this client on entry.
 *   
 *   It is recommended that this utility always be called after initialization
 *   of a Colormap to match a color approximation entry, in order to support
 *   interoperability of applications.
 *   
 * Returns:                                                                   
 *   The function returns a value of PEXUtUnmodifiedResource if it did not
 *   need to change the Colormap in any way.  It returns PEXUtModifiedResource
 *   if some adjustments were made.
 *   
 *   A negative return value indicates failure:
 *   
 *   	PEXUtXFailure means that an error was encountered in an Xlib call.
 *   
 *   	PEXUtPEXFailure means that an error occurred during a PEXlib call.
 *   
 *   	PEXUtAllocFailure means that the utility ran out of memory.
 *   
 * On Entry:                                                                 
 *   display		indicates the X connection to use; it is assumed that
 *			PEXInitialize has already been called.
 *                                                                            
 *   colormap_id	is the identifier of the Colormap to adjust; all cells
 *   			in the color approximation region are expected to be
 *   			writeable.
 *                                                                            
 *   vis_info 		a pointer to a structure describing the Visual.
 *                                                                            
 *   capx_info		is a pointer to the PEXColorApproxEntry of interest.
 *                                                                            
 * On Exit:                                                                 
 *   The contents of the Colormap may have been altered.
 *                                                                            
 * Restrictions:                                                              
 *   This function assumes that PEXInitialize has been called for the         
 *   specified display argument.                                              
 *                                                                            
 *   This function may be a no-op for some vendors' servers.  In such cases
 *   it always returns PEXUtUnmodifiedResource.
 *                                                                            
 ******************************************************************************/

extern int PEXUtModifyColormapForVendorDependencies (
#if NeedFunctionPrototypes
		Display				*display,
		Colormap			colormap_id,
		XVisualInfo			*vis_info,
		PEXColorApproxEntry		*capx_info
#endif
		);


/******************************************************************************
 *
 * Function:
 *   PEXUtCopyColormapAsReadOnly -- create a new Colormap that is a copy of
 *                               an existing one, but usable with XAllocColor
 *                                                                            
 * Description:                                                               
 *   This utility creates a Colormap that is a copy of an existing one that
 *   is assumed to have been initialized for use with a specified PEX color
 *   approximation entry.  As many cells as practical are read-only or 
 *   unallocated in the new Colormap, to improve the chances that XAllocColor
 *   will work with the Colormap.  However, the integrity of the color
 *   approximation region is protected to ensure that no unallocated cells
 *   exist inside the region, since a later XAllocColor could "corrupt" the
 *   region.
 *   
 *   The source Colormap is not freed by this utility.  The caller may choose
 *   to free it if only the new copy will be used.
 *   
 * Returns:                                                                   
 *   The function returns a value of PEXUtSuccess if the new Colormap is
 *   successfully created and initialized.
 *   
 *   A negative return value indicates failure:
 *   
 *   	PEXUtXFailure means that an error was encountered in an Xlib call.
 *   
 *   	PEXUtAllocFailure means that the utility ran out of memory.
 *   
 * On Entry:                                                                 
 *   display		indicates the X connection to use; it is assumed that
 *			PEXInitialize has already been called.
 *                                                                            
 *   source_cmap_id	is the identifier of the source Colormap to adjust.
 *   			It is expected to have been initialized to match
 *   			the color approximation entry, and any vendor-dependent
 *   			adjustments to have already been made.
 *                                                                            
 *   vis_info 		a pointer to a structure describing the Visual.
 *                                                                            
 *   capx_info		is a pointer to the PEXColorApproxEntry for which the
 *   			source Colormap is initialized.
 *                                                                            
 * On Exit:                                                                 
 *   cmap_id_return	points to storage allocated by the caller that has 
 *			been written with the new Colormap ID.
 *                                                                            
 ******************************************************************************/

extern int PEXUtCopyColormapAsReadOnly (
#if NeedFunctionPrototypes
		Display				*display,
		Colormap			source_cmap_id,
		XVisualInfo			*vis_info,
		PEXColorApproxEntry		*capx_info,
		Colormap			*cmap_id_return
#endif
		);


/******************************************************************************
 *
 * Function:
 *   PEXUtCreateColormap -- create a Colormap and initialize it for
 *   		            use with a PEX color approximation entry
 *                                                                            
 * Description:                                                               
 *   This function creates a private X Colormap.  The region of the Colormap
 *   that is described in a specified PEX color approximation entry is 
 *   initialized according to the entry; cells outside the region are left
 *   with their default values, in order to reduce color flashing of X clients
 *   that are already using those cells when the new Colormap is installed.
 *   In the case of a PEXColorRange entry, the region is initialized to a
 *   gray ramp.  As many cells in the Colormap are left read-only or unallocated
 *   as possible, while preserving the integrity of the color approximation
 *   region.  This allows XAllocColor to be used with some success on the
 *   Colormap, although some colors still will not be found by that routine.
 *   
 *   This function is approximately equivalent to the following sequence 
 *   of calls:
 *   
 *   	PEXUtCreateWriteableColormap (...,&writeable_cmap_id);
 *   	PEXUtModifyColormapForVendorDependencies (...,writeable_cmap_id,...).
 *   	PEXUtCopyColormapAsReadOnly (..,writeable_cmap_id,..,&readonly_cmap_id).
 *   	XFreeColormap (...,writeable_cmap_id);
 *   
 * Returns:                                                                   
 *   The function returns a value of PEXUtSuccess if the Colormap is
 *   successfully allocated and initialized.
 *   
 *   A negative return value indicates failure:
 *   
 *   	PEXUtXFailure means that an error was encountered in an Xlib call.
 *   
 *   	PEXUtAllocFailure means that the utility ran out of memory.
 *   
 * On Entry:                                                                 
 *   display		indicates the X connection to use.
 *   
 *   vis_info		is a pointer to a structure describing the Visual.
 *                                                                            
 *   capx_info		is a pointer to the PEXColorApproxEntry of interest.
 *                                                                            
 * On Exit:                                                                 
 *   colormap_id_return	points to storage allocated by the caller that has 
 *			been written with the new Colormap ID.
 *                                                                            
 ******************************************************************************/

extern int PEXUtCreateColormap (
#if NeedFunctionPrototypes
		Display				*display,
		XVisualInfo			*vis_info,
		PEXColorApproxEntry		*capx_info,
		Colormap			*colormap_id_return
#endif
		);


/******************************************************************************
 *
 * Function:
 *   PEXUtCreateWindow -- create a Window using specifed attributes
 *                                                                            
 * Description:                                                               
 *   This function creates a Window in the specified Visual using a
 *   specified set of attributes.  The window is not mapped; the application
 *   must take that action when appropriate.  It is intended for use by
 *   applications that already have chosen a Visual and created a Colormap,
 *   and only need window creation.  A typical use might be to create 
 *   additional windows after PEXUtCreateWindowAndColormap has been used to
 *   create the first one; the windows would all share the same Colormap.
 *   
 *   The attributes are specified in a PEXUtWindowSpecification structure.
 *   For the most part, they are specified and used in the same way as the
 *   corresponding parameters of XCreateWindow.  However, several restrictions
 *   are imposed.  Please see the on-line manual page for a complete 
 *   description of how to use the structure.
 *   
 * Returns:                                                                   
 *   The function returns a value of PEXUtSuccess if it is successful
 *   in creating the Window.
 *   
 *   A negative return value indicates failure:
 *   
 *   	PEXUtXFailure means that an error was encountered in an Xlib call.
 *   
 *   	PEXUtAllocFailure means that the utility ran out of memory.
 *   
 * On Entry:                                                                 
 *   display		indicates the X connection to use; it is assumed that
 *			PEXInitialize has already been called.
 *                                                                            
 *   screen		is the screen number on which to creation a Window.
 *                                                                            
 *   window_info	is a pointer to a structure describing the attributes
 *			to be used in Window creation.
 *                                                                            
 *   vis_info		is a pointer to a structure describing the Visual.
 *                                                                            
 * On Exit:                                                                 
 *   window_return	points to storage allocated by the caller that has 
 *			been written with the identifier of the created Window.
 *                                                                            
 *   background_color_return	
 *			points to storage allocated by the caller that has 
 *			been written with an XColor structure describing
 *			the color that was chosen for the window background.
 *			(The background color is specified in the
 *			window_info structure as a color name, which is 
 *			looked up in the Colormap specified in 
 *			window_info->attr.colormap.  This allows a newly
 *			created Colormap to be used for the Window.
 *                                                                            
 ******************************************************************************/

extern int PEXUtCreateWindow (
#if NeedFunctionPrototypes
		Display				*display,
		int				screen,
		PEXUtWindowSpecification	*window_info,
		XVisualInfo			*vis_info,
		Window				*window_return,
		XColor				*background_color_return
#endif
		);


/******************************************************************************
 *
 * Function:
 *   PEXUtCreateWindowAndColormap -- choose a Visual, create a Colormap,
 *   				     and create a Window that is ready for
 *   				     PEX rendering
 *                                                                            
 * Description:                                                               
 *   This function attempts to find the "best" Visual supported by a PEX server
 *   that meets a set of criteria specified by the caller.  If it successfully
 *   finds a Visual, it determines a color approximation entry to use based
 *   on the criteria and the selected Visual.  It creates a Colormap in 
 *   the Visual and initializes it to support the color approximation entry,
 *   unless a standard colormap property was found that contains a Colormap ID
 *   AND the sharable_colormap criterion was specified, in which case the.
 *   shared colormap is used.  Then it creates a Window in the Visual 
 *   using the chosen Colormap.
 *                                                                            
 *   The choice of what is the "best" Visual is a policy decision that
 *   cannot be controlled by the caller; it is determined by the ordering 
 *   of Renderer targets and by PEX interoperability convention.
 *   
 *   Criteria for Visual selection are specified in the PEXUtVisualCriteria
 *   structure.  Criteria can be "hard" or "soft".  Hard criteria must be met
 *   in order for a Visual to be considered as a candidate; if no Visual meets
 *   all hard criteria, the function returns a failure status.  Soft criteria
 *   are considered when choosing among several candidate Visuals that meet
 *   all the hard criteria.  There is an implicit hard criterion that PEX
 *   supports the Visual as a target (this can only be checked on 5.1 and later
 *   servers).  There is an implicit soft criterion of significant weight:
 *   when several Visuals are qualifed by hard criteria, the utility prefers
 *   the Visual that appears earlier in the target list returned 
 *   PEXMatchRenderingTargets (5.1 and later), or earlier in any other list
 *   established by interoperability convention.  (Currently, an earlier
 *   appearance in one of the standard colormap properties that may be 
 *   searched increases the likelihood of selection.)
 *   
 *   Window attributes are specified in a PEXUtWindowSpecification structure.
 *   For the most part, they are specified and used in the same way as the
 *   corresponding parameters of XCreateWindow.  However, several restrictions
 *   are imposed.  The Window has been mapped when the function returns.
 *   
 *   Any created Colormap has as many cells left read-only or unallocated as
 *   possible, while still preserving the integrity of the color approximation
 *   region.  This allows XAllocColor to be used with the Colormap.  Typically,
 *   shared Colormaps also allow use of this function.
 *   
 *   Please see the on-line manual page for a complete description of how
 *   to use the criteria and window attributes structures.
 *   
 * Returns:                                                                   
 *   The function returns a value of PEXUtSuccess if it is completely 
 *   successful in finding a Visual that meets all hard and soft criteria,
 *   and in creating the Colormap and Window.  PEXUtQualifiedSuccess is 
 *   returned if the chosen Visual meets all hard criteria but not all 
 *   soft criteria.
 *   
 *   A negative return value indicates failure:
 *   
 *   	PEXUtCriteriaFailure means that no Visual was found that meets all
 *	the hard criteria.
 *   
 *   	PEXUtXFailure means that an error was encountered in an Xlib call.
 *   
 *   	PEXUtPEXFailure means that an error occurred during a PEXlib call.
 *   
 *   	PEXUtAllocFailure means that the utility ran out of memory.
 *   
 * On Entry:                                                                 
 *   display		indicates the X connection to use; it is assumed that
 *			PEXInitialize has already been called.
 *                                                                            
 *   screen		is the screen number for which to choose a Visual.
 *                                                                            
 *   criteria		is a pointer to a structure describing the criteria
 *			to be used in Visual selection.
 *                                                                            
 *   window_info	is a pointer to a structure describing the attributes
 *			to be used in Window creation.
 *                                                                            
 * On Exit:                                                                 
 *   window_return	points to storage allocated by the caller that has 
 *			been written with the identifier of the created Window.
 *                                                                            
 *   vis_info_return	points to storage allocated by the caller that has 
 *			been written with a description of the chosen Visual.
 *                                                                            
 *   cmap_info_return	points to storage allocated by the caller that has 
 *			been written with a description of an RGB color map
 *			that might be appropriate for PEX rendering.  If a
 *			standard colormap property was used to guide selection,
 *			this is a copy of the entry from the property that
 *			guided the choice.  Otherwise, it is a description 
 *			"synthesized" from the description of the Visual.
 *			The identifier of the Colormap attached to the Window 
 *			is in cmap_info_return->colormap.
 *                                                                            
 *   capx_info_return	points to storage allocated by the caller that has 
 *			been written with a PEX color approximation entry
 *			that is compatible with the Visual and the criteria,
 *			and has been used to initialize any new Colormap.
 *			If no color approximation type was specified as a
 *			a criterion, this is typically a PEXColorSpace setup
 *			that matches the cmap_info_return data.  If a color
 *			approximation type was specified, this is an entry
 *			of the appropriate type.
 *                                                                            
 *   unmet_criteria_return	
 *			points to storage allocated by the caller that has 
 *			been written with a combined mask of all hard and
 *			soft criteria that were not met by the chosen Visual.
 *			This parameter is valid when successful or when 
 *			PEXUtCriteriaFailure is returned; it may not valid 
 *			for failure returns.
 *                                                                            
 *   std_prop_atom_return
 *			points to storage allocated by the caller that has 
 *			been written with the Atom for the name of the
 *			standard colormap property that was used in Visual
 *			selection, or None if no property was used.
 *                                                                            
 *   background_color_return	
 *			points to storage allocated by the caller that has 
 *			been written with an XColor structure describing
 *			the color that was chosen for the window background.
 *			(The background color is specified in the
 *			window_info structure as a color name, which is 
 *			looked up in the Colormap specified in 
 *			window_info->attr.colormap.  This allows a newly
 *			created Colormap to be used for the Window.
 *                                                                            
 * Restrictions:                                                              
 *   This function assumes that PEXInitialize has been called for the         
 *   specified display argument.                                              
 *                                                                            
 *   The ability of this procedure to verify that criteria are met depends
 *   on information available from the PEX server, including its support for
 *   various interoperability conventions.  In general, if a hard criterion
 *   cannot be determined to have been satisfied or not, it is considered to
 *   have not been met; if a soft criterion cannot be verified, the utility 
 *   acts as though it had been met but still returns the corresponding bit
 *   in unmet_criteria_return.
 *                                                                            
 ******************************************************************************/

extern int PEXUtCreateWindowAndColormap (
#if NeedFunctionPrototypes
		Display				*display,
		int				screen,
		PEXUtVisualCriteria		*criteria,
		PEXUtWindowSpecification	*window_info,
		Window				*window_return,
		XVisualInfo			*vis_info_return,
		XStandardColormap		*cmap_info_return,
		PEXColorApproxEntry		*capx_info_return,
		unsigned int			*unmet_criteria_return,
		Atom				*std_prop_atom_return,
		XColor				*background_color_return
#endif
		);

#ifdef __cplusplus
}                    /* for C++ V2.0 */
#endif

#endif /* } PEXUTCMAP_H */
