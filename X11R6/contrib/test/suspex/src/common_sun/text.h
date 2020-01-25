/* $XConsortium: text.h,v 1.0 93/11/22 12:26:32 rws Exp $ */

/*
        NOTICE TO USER: The source code in this file is copyrighted under
        U.S. and international laws.  SunSoft, Inc., a Sun Microsystems,
        Inc. business of Mountain View, California owns the copyright. 
        Users and possessors of this source code are hereby granted a
        nonexclusive, royalty-free copyright and design patent license
        to use this code in individual and commercial software. 

        Any use of this source code must include, in the user documentation 
        and internal comments to the code, notices to the end user as follows:

(c) Copyright 1992, 1993 SunSoft, Inc., a Sun Microsystems, Inc. business.

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
#ifndef _TEXT_H
#define _TEXT_H

/* shared */
#define SET_TEXT_COLOR(_colorType,_colr)	\
    PEXSetTextColor(dpy, resourceID, req_type, (_colorType), (_colr));

#define SET_TEXT_COLOR_INDEX(_colorIndex)	\
    PEXSetTextColorIndex(dpy, resourceID, req_type, (_colorIndex));

#define SET_TEXT_BUNDLE_INDEX(_index)	\
    PEXSetTextBundleIndex(dpy, resourceID, req_type, (_index));

#define SET_CHAR_EXPANSION(_expansion)	\
    PEXSetCharExpansion(dpy, resourceID, req_type, (_expansion));

#define SET_CHAR_HEIGHT(_height)	\
    PEXSetCharHeight(dpy, resourceID, req_type, (_height));

#define SET_CHAR_SPACING(_spacing)	\
    PEXSetCharSpacing(dpy, resourceID, req_type, (_spacing));

/* Text */
#define SET_TEXT_ALIGNMENT(_halign,_valign)	\
    PEXSetTextAlignment(dpy, resourceID, req_type, (_halign),(_valign));

#define SET_TEXT_PATH(_path)	\
    PEXSetTextPath(dpy, resourceID, req_type, (_path));

#define SET_TEXT_FONT_INDEX(_index)	\
    PEXSetTextFontIndex(dpy, resourceID, req_type, (_index));

#define SET_TEXT_PRECISION(_precision)	\
    PEXSetTextPrecision(dpy, resourceID, req_type, (_precision));

#define SET_CHAR_UP_VECTOR(_vector)	\
    PEXSetCharUpVector(dpy, resourceID, req_type, (_vector));

#define ENCTEXT(_origin,_vector1,_vector2,_count,_encodedText) \
    PEXEncodedText(dpy, resourceID, req_type, (_origin),(_vector1), \
	(_vector2), (_count),(_encodedText));

#define ENCTEXT2D(_origin,_count,_encodedText) \
    PEXEncodedText2D(dpy, resourceID, req_type, (_origin), \
	(_count),(_encodedText));

#define TEXT(_origin,_vector1,_vector2,_length,_string) \
    PEXText(dpy, resourceID, req_type, (_origin),(_vector1),(_vector2), \
	(_length),(_string));

#define TEXT2D(_origin,_length,_string) \
    PEXText2D(dpy, resourceID, req_type, (_origin),(_length),(_string));

/* AText */
#define SET_ATEXT_ALIGNMENT(_halign,_valign)	\
    PEXSetATextAlignment(dpy, resourceID, req_type, (_halign),(_valign));

#define SET_ATEXT_PATH(_path)	\
    PEXSetATextPath(dpy, resourceID, req_type, (_path));

#define SET_ATEXT_HEIGHT(_height)	\
    PEXSetATextHeight(dpy, resourceID, req_type, (_height));

#define SET_ATEXT_STYLE(_style)	\
    PEXSetATextStyle(dpy, resourceID, req_type, (_style));

#define SET_ATEXT_UP_VECTOR(_vector)	\
    PEXSetATextUpVector(dpy, resourceID, req_type, (_vector));

#define ANNOTATION_TEXT(_origin,_offset,_length,_string) \
    PEXAnnotationText(dpy, resourceID, req_type, \
	(_origin),(_offset),(_length),(_string));

#define ANNOTATION_TEXT2D(_origin,_offset,_length,_string) \
    PEXAnnotationText2D(dpy, resourceID, req_type, \
	(_origin),(_offset),(_length),(_string));

#endif /* _TEXT_H */
