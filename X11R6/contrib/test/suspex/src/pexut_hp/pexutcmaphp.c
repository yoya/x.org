/* $XConsortium: pexutcmaphp.c,v 1.0 93/11/22 12:23:35 rws Exp $ */

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
/* $Source: /xsrc/xc/unsupported/test/suspex/src/pexut_hp/RCS/pexutcmaphp.c,v $                          */
/* $Date: 93/11/22 12:23:35 $                                                 */
/* $Revision: 1.0 $                                                   */
/*                                                                            */
/* Description:                                                               */
/*   Implementation file of Hewlett-Packard-specific routines                 */
/*   for PEXUt colormap/visual utilities.                                     */
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

#include "pexutcmap.h"
#include "pexutcmapint.h"
#include "pexutcmaphp.h"


static short hp_crx_666_dithertable[256] = {
/*   0: */   0,   1,   2,   3,   4,   5,   6,   7,
/*   8: */   8,   9,  10,  11,  12,  13,  14,  15,
/*  16: */  16,  17,  18,  19,  20,  21,  22,  23,
/*  24: */  24,  25,  26,  27,  28,  29,  30,  31,
/*  32: */  32,  33,  34,  35,  36,  37,  38,  39,
/*  40: */ 255, 251, 191, 187, 127, 123, 253, 249,
/*  48: */ 189, 185, 125, 121, 239, 235, 175, 171,
/*  56: */ 111, 107, 237, 233, 173, 169, 109, 105,
/*  64: */ 223, 219, 159, 155,  95,  91, 221, 217,
/*  72: */ 157, 153,  93,  89, 254, 250, 190, 186,
/*  80: */ 126, 122, 252, 248, 188, 184, 124, 120,
/*  88: */ 238, 234, 174, 170, 110, 106, 236, 232,
/*  96: */ 172, 168, 108, 104, 222, 218, 158, 154,
/* 104: */  94,  90, 220, 216, 156, 152,  92,  88,
/* 112: */ 247, 243, 183, 179, 119, 115, 245, 241,
/* 120: */ 181, 177, 117, 113, 231, 227, 167, 163,
/* 128: */ 103,  99, 229, 225, 165, 161, 101,  97,
/* 136: */ 215, 211, 151, 147,  87,  83, 213, 209,
/* 144: */ 149, 145,  85,  81, 246, 242, 182, 178,
/* 152: */ 118, 114, 244, 240, 180, 176, 116, 112,
/* 160: */ 230, 226, 166, 162, 102,  98, 228, 224,
/* 168: */ 164, 160, 100,  96, 214, 210, 150, 146,
/* 176: */  86,  82, 212, 208, 148, 144,  84,  80,
/* 184: */ 207, 203, 143, 139,  79,  75, 205, 201,
/* 192: */ 141, 137,  77,  73, 199, 195, 135, 131,
/* 200: */  71,  67, 197, 193, 133, 129,  69,  65,
/* 208: */  63,  59,  55,  51,  47,  43,  61,  57,
/* 216: */  53,  49,  45,  41, 206, 202, 142, 138,
/* 224: */  78,  74, 204, 200, 140, 136,  76,  72,
/* 232: */ 198, 194, 134, 130,  70,  66, 196, 192,
/* 240: */ 132, 128,  68,  64,  62,  58,  54,  50,
/* 248: */  46,  42,  60,  56,  52,  48,  44,  40,
};

static short hp_crx_884_dithertable[256] = {
/*   0: */ 255, 251, 223, 219, 253, 249, 221, 217,
/*   8: */ 239, 235, 207, 203, 237, 233, 205, 201,
/*  16: */ 127, 123,  95,  91, 125, 121,  93,  89,
/*  24: */ 111, 107,  79,  75, 109, 105,  77,  73,
/*  32: */ 254, 250, 222, 218, 252, 248, 220, 216,
/*  40: */ 238, 234, 206, 202, 236, 232, 204, 200,
/*  48: */ 126, 122,  94,  90, 124, 120,  92,  88,
/*  56: */ 110, 106,  78,  74, 108, 104,  76,  72,
/*  64: */ 247, 243, 215, 211, 245, 241, 213, 209,
/*  72: */ 231, 227, 199, 195, 229, 225, 197, 193,
/*  80: */ 119, 115,  87,  83, 117, 113,  85,  81,
/*  88: */ 103,  99,  71,  67, 101,  97,  69,  65,
/*  96: */ 246, 242, 214, 210, 244, 240, 212, 208,
/* 104: */ 230, 226, 198, 194, 228, 224, 196, 192,
/* 112: */ 118, 114,  86,  82, 116, 112,  84,  80,
/* 120: */ 102,  98,  70,  66, 100,  96,  68,  64,
/* 128: */ 191, 187, 159, 155, 189, 185, 157, 153,
/* 136: */ 175, 171, 143, 139, 173, 169, 141, 137,
/* 144: */  63,  59,  31,  27,  61,  57,  29,  25,
/* 152: */  47,  43,  15,  11,  45,  41,  13,   9,
/* 160: */ 190, 186, 158, 154, 188, 184, 156, 152,
/* 168: */ 174, 170, 142, 138, 172, 168, 140, 136,
/* 176: */  62,  58,  30,  26,  60,  56,  28,  24,
/* 184: */  46,  42,  14,  10,  44,  40,  12,   8,
/* 192: */ 183, 179, 151, 147, 181, 177, 149, 145,
/* 200: */ 167, 163, 135, 131, 165, 161, 133, 129,
/* 208: */  55,  51,  23,  19,  53,  49,  21,  17,
/* 216: */  39,  35,   7,   3,  37,  33,   5,   1,
/* 224: */ 182, 178, 150, 146, 180, 176, 148, 144,
/* 232: */ 166, 162, 134, 130, 164, 160, 132, 128,
/* 240: */  54,  50,  22,  18,  52,  48,  20,  16,
/* 248: */  38,  34,   6,   2,  36,  32,   4,   0,
};


/* 
    This is the value type for the _HP_RGB_SHADING_MAP property.
    There may be one entry per Visual.
*/

typedef struct {
	VisualID	visualid;
	int		interpolation_ramp;
} hp_rgb_shading_map_type;

/*
    Interesting values for the interpolation_ramp field.
*/

#define HPPEX_INTERP_RAMP_884_CRX     0x011 
#define HPPEX_INTERP_RAMP_666_CRX     0x012


int pexut_modify_colormap_for_HP_dependencies (
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
	int				num_colors;
	unsigned long			pixel_value;
	XColor				*new_colors, *source_colors, *p_color;
	short				*translation_table;
	int				base_pixel;

	int				ramp_index;
	hp_rgb_shading_map_type		*shading_map_data;
	int				shading_map_count;
	int				i;

	Atom				property_atom;
	Atom	    			actual_type;
	int				actual_format;
	unsigned long   		item_count;
	unsigned long   		item_count_return, bytes_unread;
	int				result;


	/*
	    If using X protocol, the remote server's rendering behavior
	    does not affect the local rendering, so ignore the property.
	    Otherwise, try to fetch it.
	*/

	if (!strcmp("X",getenv("HPPEX_CLIENT_PROTOCOL")))
	    return PEXUtUnmodifiedResource;


	/*
	    Currently nothing to do on any HP platform for PEXColorRange.
	*/
	
	if (capx_info->type != PEXColorSpace)
	    return PEXUtUnmodifiedResource;


	/*
	    Determine if the _HP_RGB_SHADING_MAP property is defined on
	    the server, and if so get its value.
	*/

	ramp_index = 0;


	/*
	    Get the atom from the X server.  If it doesn't exist,
	    neither does the property.  In this case, succeed silently.
	*/

	property_atom = XInternAtom(display, "_HP_RGB_SHADING_MAP", True);
	if (property_atom == None) {
	    return PEXUtUnmodifiedResource;
	}


	/* 
	    Property atom exists - fetch the property.
	*/

	bytes_unread = 0;
	item_count = sizeof(hp_rgb_shading_map_type)/4;
	do {
	    item_count += (bytes_unread+3)/4;
	    result = XGetWindowProperty (display, 
				    RootWindow(display, vis_info->screen),
				    property_atom,
				    0, item_count, False, 
				    property_atom,
				    &actual_type, &actual_format, 
				    &item_count_return, &bytes_unread,
				    (unsigned char **) &shading_map_data);

	} while ((result == Success) && (bytes_unread > 0));

	if (result != Success)
	    return PEXUtUnmodifiedResource;

	/*
	    Search for an entry for the specified visual in the returned data.
	*/

	ramp_index = 0;
	shading_map_count = item_count_return/(sizeof(hp_rgb_shading_map_type)/4);
	for (i=0; i<shading_map_count; i++) {

	    if (vis_info->visualid == shading_map_data[i].visualid) {

		ramp_index = shading_map_data[i].interpolation_ramp;
		break;
	    }
	}

	if (item_count_return > 0)
	    XFree (shading_map_data);
	if (ramp_index == 0) 
	    return PEXUtUnmodifiedResource;


	/*
	    See if this is one of the interesting values for 
	    the interpolation ramp, if not interesting quit now.
	*/

	if ((ramp_index == HPPEX_INTERP_RAMP_884_CRX) &&
	    (capx_info->max1 == 7) && (capx_info->mult1 == 32) &&
	    (capx_info->max2 == 7) && (capx_info->mult2 == 4) &&
	    (capx_info->max3 == 3) && (capx_info->mult3 == 1) &&
	    (capx_info->base_pixel == 0))
	{

	    translation_table = hp_crx_884_dithertable;
	    base_pixel = 0;
	    num_colors = 256;
	}
	else if ((ramp_index == HPPEX_INTERP_RAMP_666_CRX) && 
	    (capx_info->max1 == 5) && (capx_info->mult1 == 36) &&
	    (capx_info->max2 == 5) && (capx_info->mult2 == 6) &&
	    (capx_info->max3 == 5) && (capx_info->mult3 == 1) &&
	    (capx_info->base_pixel == 40))
	{
	    translation_table = hp_crx_666_dithertable;
	    base_pixel = 40;
	    num_colors = 216;
	}
	else
	    return PEXUtUnmodifiedResource;


	/*
	    Allocate storage and fetch the current colormap contents.
	*/

	if ((source_colors = (XColor *) 
			malloc (num_colors * sizeof(XColor))) == NULL) {

	    return PEXUtAllocFailure;
	}

	for (pixel_value = base_pixel, p_color = source_colors; 
	    pixel_value < base_pixel + num_colors; 
	    pixel_value++, p_color++) {

	    p_color->pixel = pixel_value;
	}
	XQueryColors (display, colormap_id, source_colors, num_colors);


	/*
	    Allocate storage for a new set of colors for the ramp.
	*/

	if ((new_colors = (XColor *) 
			malloc (num_colors * sizeof(XColor))) == NULL) {

	    free (source_colors);
	    return PEXUtAllocFailure;
	}

	
	/*
	    Copy colors from the original ramp, but in the shuffled order.
	*/

	p_color = new_colors;
	for (pixel_value = base_pixel; 
	    pixel_value < base_pixel + num_colors; 
	    pixel_value++) {

	    p_color->flags = DoRed | DoGreen | DoBlue;
	    p_color->pixel = translation_table[pixel_value];
	    p_color->red = source_colors[pixel_value - base_pixel].red;
	    p_color->green = source_colors[pixel_value - base_pixel].green;
	    p_color->blue = source_colors[pixel_value - base_pixel].blue;
	    p_color++;
	}


	/*
	    Load the color map.
	*/

	XStoreColors (display, colormap_id, new_colors, num_colors);


	/*
	    Free the temporary storage.
	*/

	free (new_colors);
	free (source_colors);

	return PEXUtModifiedResource;

} /* pexut_modify_colormap_for_HP_dependencies */
