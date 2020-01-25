/* $SonyId: StsAreaP.h,v 1.1 1994/05/30 13:05:43 makoto Exp $ */
/******************************************************************
Copyright (c) 1993, 1994  Sony Corporation

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL SONY CORPORATION BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of Sony Corporation shall
not be used in advertising or otherwise to promote the sale, use or ot
her dealings in this Software without prior written authorization from
Sony Corporation.
******************************************************************/
#ifndef	_XmwStatusAreaP_H
#define	_XmwStatusAreaP_H

#include	<Xmw/StsArea.h>
#include	<X11/IntrinsicP.h>
#include	<X11/CoreP.h>
#include	<X11/ConstrainP.h>

typedef struct	_XmwStatusAreaClassPart {
    XtPointer	extension;
} XmwStatusAreaClassPart;

typedef struct	_XmwStatusAreaClassRec {
    CoreClassPart		core_class;
    CompositeClassPart		composite_class;
    ConstraintClassPart		constraint_class;
    XmwStatusAreaClassPart	status_area_class;
} XmwStatusAreaClassRec;

extern XmwStatusAreaClassRec	xmwStatusAreaClassRec;


typedef struct	_XmwStatusAreaPart
{
    Dimension           margin_width;
    Dimension           margin_height;
    Dimension		spacing;

    Window		parent_window;
    Cursor		cursor;
    Dimension		needed_width;
    Dimension		needed_height;
} XmwStatusAreaPart;


typedef struct	_XmwStatusAreaWidgetRec {
    CorePart		core;
    CompositePart	composite;
    ConstraintPart	constraint;
    XmwStatusAreaPart	status_area;
} XmwStatusAreaWidgetRec;

#endif	/* _XmwStatusAreaP_H */
