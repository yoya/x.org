/* $SonyId: List.h,v 1.1 1994/05/30 13:05:43 makoto Exp $ */
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
#ifndef _XmwList_H
#define _XmwList_H

#include	<X11/Xaw/Simple.h>
#include	<X11/Xaw/Form.h>
#include	<Xmw/Xmw.h>
#include	<Xmw/XmwStrDefs.h>

/* Resources:

 Name			    Class	     RepType	    Default Value
 ----			    -----	     -------	    -------------
 notifyCallback		    Callback	     XtCallbackList NULL
 setCallback		    Callback	     XtCallbackList NULL
 unsetCallback		    Callback	     XtCallbackList NULL
 browseSelectionCallback    Callback	     XtCallbackList NULL
 fontSet		    FontSet	     XFontSet	    -*-fixed-*-*-*-*-16-*-*-*-*-*-*-*
 foreground		    Foreground	     Pixel	    XtDefaultForeground
 highlightAlways	    HighlightAlways  Boolean	    False;
 highlightThickness	    Thickness	     Dimension	    2
 horizDistance		    Thickness	     Int	    4
 itemArmColor		    ArmColor	     Pixel	    dynamic
 itemBaseHeight		    ItemBaseHeight   Dimension	    0
 itemBaseWidth		    ItemBaseWidth    Dimension	    0
 itemBackground		    Background	     Pixel	    XtDefaultBackground
 itemBorderWidth	    ItemBorderWidth  Dimension	    1
 itemCount		    ItemCount	     int	    0
 itemLength		    ItemLength	     int	    0
 items			    Items	     String	    NULL
 justify		    Justify	     Justify	    center
 marginHeight		    MarginHeight     Dimension	    2
 marginWidth		    MarginWidth	     Dimension	    2
 numColumns		    NumColumns	     short	    dynamic
 numRows		    NumRows	     short	    dynamic
 selectedItem		    SelectedItem     int	    dynamic
 vertDistance		    Thickness	     Int	    4
 visibleHorizontalItemCount VisibleItemCount int	    dynamic
 visibleVerticalItemCount   VisibleItemCount int	    dynamic
*/

extern WidgetClass	xmwListWidgetClass;

typedef struct _XmwListClassRec	*XmwListWidgetClass;
typedef struct _XmwListRec	*XmwListWidget;

typedef struct {
    int			reason;
    XEvent		*event;
    int			item;
} XmwListCallbackStruct, *XmwListCallback;

#if NeedFunctionPrototypes
#if defined(__cplusplus)  ||  defined(c_plusplus)
extern "C" {
#endif
extern Widget	XmwCreateList( Widget parent, char *name, ArgList arglist, int argcount );
extern Widget	XmwCreateScrolledList( Widget parent, char *name, ArgList arglist, int argcount );
extern Boolean	XmwListSelectItem( Widget w, int item );
extern void	XmwListPreviousItem( Widget w );
extern void	XmwListNextItem( Widget w );
extern void	XmwListUpItem( Widget w );
extern void	XmwListDownItem( Widget w );
extern void	XmwListPageUpItem( Widget w );
extern void	XmwListPageDownItem( Widget w );
extern void	XmwListTopItem( Widget w );
extern void	XmwListEndItem( Widget w );
extern void	XmwCvtStringToSourceCursorIconType( XrmValue *args, Cardinal *num_args, XrmValue *from_val, XrmValue *to_val );
#if defined(__cplusplus)  ||  defined(c_plusplus)
}
#endif
#else /* NeedFunctionPrototypes */
extern Widget	XmwCreateList();
extern Widget	XmwCreateScrolledList();
extern void	XmwListPreviousItem();
extern void	XmwListNextItem();
extern void	XmwListUpItem();
extern void	XmwListDownItem();
extern Boolean	XmwListSelectItem();
extern void	XmwListPageUpItem();
extern void	XmwListPageDownItem();
extern void	XmwListTopItem();
extern void	XmwListEndItem();
extern void	XmwCvtStringToSourceCursorIconType();
#endif /* NeedFunctionPrototypes */

#endif /* _XmwList_H */
