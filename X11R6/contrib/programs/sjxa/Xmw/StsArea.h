/* $SonyId: StsArea.h,v 1.1 1994/05/30 13:05:43 makoto Exp $ */
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
#ifndef	_XmwStatusArea_H
#define	_XmwStatusArea_H

#include	<Xmw/Xmw.h>

/* Resources:

 Name			Class		Reptype		Default Value
 ----			-----		-------		-------------
 marginWidth		MarginWidth	Dimension	5
 marginHeight		MarginHeight	Dimension	5
 spacing		Spacing		Dimension	10
 parentWindow		ParentWindow	Window		NULL
 cursor			Cursor		Cursor		0
 areaNeededWidth	Width		Dimension	0
 areaNeededHeight	Height		Dimension	0
*/

extern WidgetClass			xmwStatusAreaWidgetClass;

typedef struct _XmwStatusAreaClassRec	*XmwStatusAreaWidgetClass;
typedef struct _XmwStatusAreaWidgetRec	*XmwStatusAreaWidget;


#if	NeedFunctionPrototypes

#if defined(__cplusplus)  ||  defined(c_plusplus)
extern "C" {
#endif

extern Widget	XmwCreateManagedStatusArea( Widget p, char *name, ArgList al, Cardinal ac );

#if defined(__cplusplus)  ||  defined(c_plusplus)
}
#endif
#else	/* NeedFunctionPrototypes */
extern Widget	XmwCreateManagedStatusArea();
#endif	/* NeedFunctionPrototypes */

#ifndef	XmwIsStatusArea
#define	XmwIsStatusArea(w)	XtIsSubclass((w), xmwStatusAreaWidgetClass)
#endif	/* XmwIsStatusArea */

#endif	/*_XmwStatusArea_H */
