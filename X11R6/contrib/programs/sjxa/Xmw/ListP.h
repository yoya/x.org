/* $SonyId: ListP.h,v 1.1 1994/05/30 13:05:43 makoto Exp $ */
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
#ifndef	_XmwListP_H
#define	_XmwListP_H

#include	<Xmw/List.h>
#include	<X11/Xaw/SimpleP.h>
#include	<Xmw/ViewP.h>
#include	<X11/Xaw/ScrollbarP.h>

/* List class structure */
typedef struct	_XmwListClassPart
{
    XtPointer	extension;
} XmwListClassPart;


/* Full class record declaration for XmwList class */
typedef struct	_XmwListClassRec {
    CoreClassPart		core_class;
    SimpleClassPart		simple_class;
    XmwListClassPart		list_class;
} XmwListClassRec;


/* XmwList instance record */
typedef struct	_XmwListPart
{
    Pixel		foreground;		/* XtNforeground	      */
    Dimension		border_width;		/* XmwNitemBorderWidth	      */
    int			dx;			/* XtNhorizDistance	      */
    int			dy;			/* XtNvertDistance	      */
    Dimension		highlight_thickness;	/* XtNhighlightThickness      */
    Dimension		base_width;		/* XmwNitemBaseWidth	      */
    Dimension		base_height;		/* XmwNitemBaseHeight	      */
    XFontSet		font_set;		/* XmwNfontSet		      */
    Dimension		margin_width;		/* XtNmarginWidth	      */
    Dimension		margin_height;		/* XtNmarginHeight	      */
    Pixel		background;		/* XmwNitemBackground	      */
    int			selected_item;		/* XmwNselectedItem	      */
    int			visible_vitem_count;/* XmwNvisibleVerticalItemCount   */
    int			visible_hitem_count;/* XmwNvisibleHorizontalItemCount */
    short		num_rows;		/* XmwNnumRows		      */
    short		num_columns;		/* XmwNnumColumns	      */
    int			item_count;		/* XmwNitemCount	      */
    int			item_length;		/* XmwNitemLength	      */
    String		items;			/* XmwNitems		      */
    unsigned char	source_cursor_icon_type;/* XmwNsourceCursorIconType   */
    XtJustify		justify;		/* XtNjustify		      */
    Boolean		highlight_always;	/* XmwNhighlightAlways	      */

    XtCallbackList	notify_callback;
    XtCallbackList	set_callback;
    XtCallbackList	unset_callback;
    XtCallbackList	browse_selection_callback;

    Pixmap		pixmap;

    GC			foreground_gc;
    GC			background_gc;
    GC			highlight_gc;
    GC			unhighlight_gc;

    int			last_selected_item;

    int			par_item_length;
    Dimension		font_width;
    Dimension		font_height;
    short		ascent;
    Boolean		button_press;
    Boolean		armed;
    Boolean		enter;

    Widget		viewport_window;
    Widget		clip_window;
    Widget		h_scroll_bar;
    Widget		v_scroll_bar;
} XmwListPart;


/* Full instance record declaration */
typedef struct	_XmwListRec {
    CorePart	core;
    SimplePart	simple;
    XmwListPart	list;
} XmwListRec;


/* Macros for instance fields */


#endif	/* _XmwListP_H */
