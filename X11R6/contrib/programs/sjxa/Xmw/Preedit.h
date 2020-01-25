/* $SonyId: Preedit.h,v 1.1 1994/05/30 13:05:43 makoto Exp $ */
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
#ifndef	_XmwPreedit_H
#define	_XmwPreedit_H

#include	<X11/Intrinsic.h>
#include	<X11/Xaw/Simple.h>
#include	<Xmw/Xmw.h>
#include	<Xmw/XmwStrDefs.h>

/* Resources:

 Name		Class		RepType		Default Vaue
 ----		-----		-------		------------

*/

extern WidgetClass		xmwPreeditWidgetClass;

typedef struct _XmwPreeditClassRec	*XmwPreeditWidgetClass;
typedef struct _XmwPreeditRec		*XmwPreeditWidget;


#if NeedFunctionPrototypes
#if defined(__cplusplus)  ||  defined(c_plusplus)
extern "C" {
#endif

extern Widget	XmwCreatePreedit( Widget, char *, ArgList, Cardinal );
extern void	XmwPreeditSetHighlight( Widget, Position, Position, XmwHighlightMode );
extern Boolean	XmwPreeditReplace( Widget, Position, Position, char * );
extern Boolean	XmwPreeditReplaceWcs( Widget, Position, Position, wchar_t * );
extern void	XmwPreeditInsert( Widget, Position, char * );
extern void	XmwPreeditInsertWcs( Widget, Position, wchar_t * );
extern void	XmwPreeditSetEditable( Widget, Boolean );
extern void	XmwPreeditMoveWidget( Widget, Position, Widget );
extern void	XmwPreeditSetString( Widget, char *, XmwHighlightMode );
extern void	XmwPreeditSetStringWcs( Widget, wchar_t *, XmwHighlightMode );
extern void	XmwPreeditMapWidget( Widget );
extern void	XmwPreeditUnmapWidget( Widget );
extern void	XmwPreeditAutoReplace( Widget );
extern void	XmwPreeditSetCursorPosition( Widget, Position );

#if defined(__cplusplus)  ||  defined(c_plusplus)
}
#endif
#else	/* NeedFunctionPrototypes */
extern Widget	XmwCreatePreedit();
extern void	XmwPreeditSetHighlight();
extern Boolean	XmwPreeditReplace();
extern Boolean	XmwPreeditReplaceWcs();
extern void	XmwPreeditInsert();
extern void	XmwPreeditInsertWcs();
extern void	XmwPreeditSetEditable();
extern void	XmwPreeditMoveWidget();
extern void	XmwPreeditSetString();
extern void	XmwPreeditSetStringWcs();
extern void	XmwPreeditMapWidget();
extern void	XmwPreeditUnmapWidget();
extern void	XmwPreeditAutoReplace();
extern void	XmwPreeditSetCursorPosition();
#endif	/* NeedFunctionPrototypes */

#endif	/*_XmwPreedit_H */
