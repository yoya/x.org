/* $XConsortium: imagelib.h,v 1.0 93/11/22 12:09:04 rws Exp $ */

/*
        NOTICE TO USER: The source code in this file is copyrighted under
        U.S. and international laws.  SunSoft, Inc., a Sun Microsystems,
        Inc. business of Mountain View, California owns the copyright. 
        Users and possessors of this source code are hereby granted a
        nonexclusive, royalty-free copyright and design patent license
        to use this code in individual and commercial software. 

        Any use of this source code must include, in the user documentation 
        and internal comments to the code, notices to the end user as follows:

(c) Copyright 1989, 1990, 1991, 1992, 1993 SunSoft, Inc., a Sun Microsystems, Inc. business.

        SUNSOFT, INC. AND SUN MICROSYSTEMS, INC. MAKE NO REPRESENTATIONS
        ABOUT THE SUITABILITY OF THIS SOURCE CODE FOR ANY PURPOSE. IT IS
        PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY OF ANY KIND. 
        SUNSOFT, INC., AND SUN MICROSYSTEMS, INC., SEVERALLY AND
        INDIVIDUALLY, DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOURCE
        CODE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
        FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL SUNSOFT, INC.
        OR SUN MICROSYSTEMS, INC., BE LIABLE FOR ANY SPECIAL, INDIRECT,
        INCIDENTAL, OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER
        RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
        OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
        OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOURCE CODE.
*/


#ifndef _IMAGELIB_H
#define _IMAGELIB_H


/*
 * imagelib.h - header file for image library functions
 */
#include "portable.h"

#define MAXCOLOR 256

typedef struct i_image {
	XImage *image;
	Unsign16 width;
	Unsign16 height;
	Unsign8 *imagedata;
} I_image;		

typedef struct i_cmap {
	Colormap cmap;
	unsigned long blackpixel;
	Unsign8 ctbl[MAXCOLOR][3];
} I_cmap;		

typedef struct {
    Unsign32 rmask;
    Unsign32 gmask;
    Unsign32 bmask;
    Unsign32 rshift;
    Unsign32 gshift;
    Unsign32 bshift;
} Pixel_info;

/*
 * external table of contents
 */
extern void i_diff_image();
extern void i_diff_image24();
extern void i_load_image();
extern void i_load_image2();
extern void i_load_image24();
extern void i_display_image();
extern void i_free_image();
extern void i_free_cmap();

#endif /* _IMAGELIB_H */
