/*
 *      Original Source: ./lib/Xaw/CommandP.h
 *
 * Changes
 * Modified by Fuji Xerox Co., Ltd.
 *
 *
 */
/*
* $XConsortium: CommandP.h,v 1.21 88/09/27 11:19:53 swick Exp $
*/

/***********************************************************
Copyright 1987, 1988 by Digital Equipment Corporation, Maynard, Massachusetts,
and the Massachusetts Institute of Technology, Cambridge, Massachusetts.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its
documentation for any purpose and without fee is hereby granted,
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in
supporting documentation, and that the names of Digital or MIT not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.

DIGITAL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/

/************************************************************
Copyright 1989 by Information-Technology Promotion Agency Japan (IPA),
Mitsubishi Research Institute, Inc. (MRI), and Fuji Xerox Co., Ltd. (FX)

                        All Rights Reserved

Permission to use, copy, modify and distribute this software and its
documentation for any purpose and without fee is hereby granted,
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in
supporting documentation, and that the names of IPA, MRI or FX not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.

IPA, MRI or FX DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
EVENT SHALL IPA, MRI or FX BE LIABLE FOR ANY SPECIAL, INDIRECT OR
CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
PERFORMANCE OF THIS SOFTWARE.

********************************************************/

#ifndef _StMaskedSwitchP_h
#define _StMaskedSwitchP_h

#include <St/StMaskSw.h>
#include <St/StPictrSwP.h>

typedef struct _StMaskedSwitchClass {int foo;} StMaskedSwitchClassPart;

typedef struct _StMaskedSwitchClassRec {
    CoreClassPart		core_class;
    StPrimitiveClassPart	stPrimitive_class;
    StLabelClassPart		stLabel_class;
    StSimpleSwitchClassPart	stSimpSw_class;
    StPictrialSwitchClassPart	stPicSw_class;
    StMaskedSwitchClassPart	stMaskSw_class;
} StMaskedSwitchClassRec;

extern StMaskedSwitchClassRec stMaskedSwitchClassRec;

typedef struct {
    Pixmap	  pixmap;
    int		  x;
    int		  y;
    unsigned int  width;
    unsigned int  height;
    unsigned int  depth;
} StPixmapInf;

typedef struct {
    /* resources */
    Pixmap	pixmap_for_set;
    Pixmap	pixmap_for_unset;

    /* private state */
    Boolean		pixmaps_are_mine;
    StPixmapInf		set_pixmap;
    StPixmapInf		unset_pixmap;
    XtWidgetProc	default_pixmaps;
} StMaskedSwitchPart;

typedef struct _StMaskedSwitchRec {
    CorePart			core;
    StPrimitivePart		stPrimitive;
    StLabelPart			stLabel;
    StSimpleSwitchPart		stSimpSw;
    StPictrialSwitchPart	stPicSw;
    StMaskedSwitchPart		stMaskSw;
} StMaskedSwitchRec;

#endif _StMaskedSwitchP_h
