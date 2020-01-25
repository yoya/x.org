/* $SonyId: TextOut.h,v 1.1 1994/05/30 13:05:43 makoto Exp $ */
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
#ifndef	_XmwTextOut_h
#define	_XmwTextOut_h
#include	<X11/Intrinsic.h>
#include	<X11/Xaw/Simple.h>
#include	<Xmw/Xmw.h>
#include	<Xmw/XmwStrDefs.h>

/* Resources:

 Name		     Class		RepType		Default Value
 ----		     -----		-------		-------------
 topLine	     TopLine		int		0
 value		     Value		String		NULL
 valueWcs	     ValueWcs		ValueWcs	""
 cursorPosition	     CursorPosition	Position	0
 displayCaret	     Output		Boolean		True
 columns	     Columns		short		dynamic
 blinkRate	     BlinkRate		int		0
 fontSet	     FontSet		XFontSet	XtDefaultFontSet
 foreground	     Foreground		Pixel		XtDefaultForeground
 focus		     Focus		Boolean		False
 maxLength	     MaxLength		int		MAXINT
 verifyBell	     VerifyBell		Boolean		True
*/

extern WidgetClass		xmwTextOutWidgetClass;

typedef struct _XmwTextOutClassRec	*XmwTextOutWidgetClass;
typedef struct _XmwTextOutRec		*XmwTextOutWidget;

#if NeedFunctionPrototypes
#if defined(__cplusplus)  ||  defined(c_plusplus)
extern "C" {
#endif
extern Widget XmwCreateTextOut( Widget parent, char *name, ArgList arglist, Cardinal argcount );
extern void XmwTextOutSetHighlight( Widget w, Position left, Position right, XmwHighlightMode mode );
extern Boolean XmwTextOutReplace( Widget w, Position from_pos, Position to_pos, char *value );
extern Boolean XmwTextOutReplaceWcs( Widget w, Position from_pos, Position to_pos, wchar_t *value );
extern void XmwTextOutInsert( Widget w, Position position, char *value );
extern void XmwTextOutInsertWcs( Widget w, Position position, wchar_t *value );
extern void XmwTextOutSetEditable( Widget w, int editable );
extern void XmwTextOutSetString( Widget w, char *value, XmwHighlightMode mode );
extern void XmwTextOutSetStringWcs( Widget w, wchar_t *value, XmwHighlightMode mode );
extern void XmwTextOutSetCursorPosition( Widget w, Position position );
extern char *XmwTextOutGetString( Widget w );
extern wchar_t *XmwTextOutGetStringWcs( Widget w );
extern Position XmwTextOutGetLastPosition( Widget w );
#if defined(__cplusplus)  ||  defined(c_plusplus)
}
#endif
#else	/* NeedFunctionPrototypes */
extern Widget XmwCreateTextOut();
extern void XmwTextOutSetHighlight();
extern Boolean XmwTextOutReplace();
extern Boolean XmwTextOutReplaceWcs();
extern void XmwTextOutInsert();
extern void XmwTextOutInsertWcs();
extern void XmwTextOutSetEditable();
extern void XmwTextOutSetString();
extern void XmwTextOutSetStringWcs();
extern void XmwTextOutSetCursorPosition();
extern char *XmwTextOutGetString();
extern wchar_t *XmwTextOutGetStringWcs();
extern Position XmwTextOutGetLastPosition();
#endif	/* NeedFunctionPrototypes */

#endif	/*_XmwTextOut_h */
