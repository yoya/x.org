/* $SonyId: List.c,v 1.2 1994/06/03 06:37:18 makoto Exp $ */
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
#include	<stdio.h>
#include	<X11/Xlib.h>
#include	<X11/Xlibint.h>
#include	<X11/IntrinsicP.h>
#include	<X11/Xaw/XawInit.h>
#include	<X11/Xmu/Converters.h>
#include	<Xmw/ListP.h>
#include	<region.h>

static char defaultTranslations[] = "\
		<Key>Left:		PreviousItem()\n\
		<Key>Right:		NextItem()\n\
		<Key>Up:		UpItem()\n\
		<Key>Down:		DownItem()\n\
    ~a		<Key>Up:		PageUpItem()\n\
    ~a		<Key>Down:		PageDownItem()\n\
		<Motion>:		Motion()\n\
		<Btn1Down>:		Set()\n\
		<Btn1Up>:		Notify() Unset()\n\
		<EnterWindow>:		Enter()\n\
		<LeaveWindow>:		Leave()";


static int
#if NeedFunctionPrototypes
Event2Item( XmwListWidget lw, XEvent *event )
#else /* NeedFunctionPrototypes */
Event2Item( lw, event )
    XmwListWidget	lw;
    XEvent		*event;
#endif /* NeedFunctionPrototypes */
{
    int		d, w, h;
    int		item;

    if( event == NULL )
	return( -1 );

    switch( event->type ) {
      case ButtonPress:
      case ButtonRelease:
	d = (lw->list.border_width + lw->list.highlight_thickness) * 2;
	w = lw->list.dx + lw->list.font_width + lw->list.margin_width * 2 + d;
	h = lw->list.dy + lw->list.font_height + lw->list.margin_height * 2 + d;

	if( event->xbutton.x % w < lw->list.dx  ||
	    event->xbutton.x / w >= lw->list.num_rows  ||
	    event->xbutton.y % h < lw->list.dy  ||
	    event->xbutton.y / h >= lw->list.num_columns )
	    return( -1 );
	item = event->xbutton.x / w + event->xbutton.y / h * lw->list.num_rows;
	break;

      case MotionNotify:
	d = (lw->list.border_width + lw->list.highlight_thickness) * 2;
	w = lw->list.dx + lw->list.font_width + lw->list.margin_width * 2 + d;
	h = lw->list.dy + lw->list.font_height + lw->list.margin_height * 2 + d;

	if( event->xmotion.x % w < lw->list.dx  ||
	    event->xmotion.x / w >= lw->list.num_rows  ||
	    event->xmotion.y % h < lw->list.dy  ||
	    event->xmotion.y / h >= lw->list.num_columns )
	    return( -1 );
	item = event->xmotion.x / w + event->xmotion.y / h * lw->list.num_rows;
	break;

      case KeyPress:
      case KeyRelease:
	item = lw->list.selected_item;
	break;

      default:
	fprintf( stderr, "Event2Item:unknown event type[%d]\n", event->type );
	return( -1 );
    }

    if( item >= lw->list.item_count  ||
	lw->list.items[lw->list.par_item_length*item] == '\0' )
	return( -1 );
    return( item );
}


static Boolean
#if NeedFunctionPrototypes
ItemCheck( XmwListWidget lw )
#else /* NeedFunctionPrototypes */
ItemCheck( lw )
    XmwListWidget	lw;
#endif /* NeedFunctionPrototypes */
{
    int		left, right;
    int		new_item;

    left = lw->list.last_selected_item % lw->list.num_rows;
    right = lw->list.num_rows - left;
    for( new_item = lw->list.last_selected_item; left >= 0;
	 left--, new_item-- ) {
	if( lw->list.items[lw->list.par_item_length*new_item] != '\0' ) {
	    lw->list.last_selected_item = new_item;
	    return( True );
	}
    }
    if( lw->list.item_count - lw->list.last_selected_item < right )
	right = lw->list.item_count - lw->list.last_selected_item;
    for( new_item = lw->list.last_selected_item + 1; right > 1; right--,
	 new_item++ ) {
	if( lw->list.items[lw->list.par_item_length*new_item] != '\0' ) {
	    lw->list.last_selected_item = new_item;
	    return( True );
	}
    }
    return( False );
}



static void 
#if NeedFunctionPrototypes
BorderHighlight( XmwListWidget lw )
#else /* NeedFunctionPrototypes */
BorderHighlight( lw )
    XmwListWidget	lw;
#endif /* NeedFunctionPrototypes */
{
    Dimension	width, height, bw;
    int		dx, dy;
    int		ht;

    if( lw->core.width == 0  ||  lw->core.height == 0  ||
	lw->list.selected_item == -1  ||
	(!lw->list.highlight_always  &&  !lw->list.enter) )
	return;

    ht = lw->list.highlight_thickness;
    if( ht ) {
	bw = lw->list.border_width;
	width = (bw + ht + lw->list.margin_width) * 2 + lw->list.font_width;
	height = (bw + ht + lw->list.margin_height) * 2 + lw->list.font_height;
	dx = (lw->list.selected_item % lw->list.num_rows)
	   * (width + lw->list.dx) + lw->list.dx + bw + ht/2;
	dy = (lw->list.selected_item / lw->list.num_rows)
	   * (height + lw->list.dy) + lw->list.dy + bw + ht/2;
	XDrawRectangle( XtDisplay(lw), XtWindow(lw), lw->list.highlight_gc,
			dx, dy,
			lw->list.margin_width*2 + lw->list.font_width + ht,
			lw->list.margin_height*2 + lw->list.font_height + ht );
    }
}


static void 
#if NeedFunctionPrototypes
BorderUnhighlight( XmwListWidget lw, int item )
#else
BorderUnhighlight( lw, item )
    XmwListWidget	lw;
    int			item;
#endif /* NeedFunctionPrototypes */
{
    Dimension	width, height, bw;
    int		dx, dy;
    int		ht;

    if( lw->core.width == 0  ||  lw->core.height == 0  ||  item == -1 )
	return;

    ht = lw->list.highlight_thickness;
    if( ht ) {
	bw = lw->list.border_width;
	width = (bw + ht + lw->list.margin_width) * 2 + lw->list.font_width;
	height = (bw + ht + lw->list.margin_height) * 2 + lw->list.font_height;
	dx = (item % lw->list.num_rows) * (width + lw->list.dx)
	   + lw->list.dx + bw + ht/2;
	dy = (item / lw->list.num_rows) * (height + lw->list.dy)
	   + lw->list.dy + bw + ht/2;
	XDrawRectangle( XtDisplay(lw), XtWindow(lw), lw->list.unhighlight_gc,
			dx, dy,
			lw->list.margin_width*2 + lw->list.font_width + ht,
			lw->list.margin_height*2 + lw->list.font_height + ht );
    }
}



static void 
#if NeedFunctionPrototypes
SetScrollBar( XmwListWidget lw )
#else /* NeedFunctionPrototypes */
SetScrollBar( lw )
    XmwListWidget	lw;
#endif /* NeedFunctionPrototypes */
{
    Dimension	slider_size;
    float	top, shown;

    if( lw->list.v_scroll_bar ) {
	slider_size = lw->list.clip_window->core.height;
	if( slider_size > lw->core.height )
	    slider_size = lw->core.height;
	if( slider_size == 0 )
	    slider_size = 1;

	shown = (float)lw->list.clip_window->core.height /
		(float)lw->core.height;
	top = (float)-lw->core.y / (float)lw->core.height;
	XawScrollbarSetThumb( lw->list.v_scroll_bar, top, shown );
    }
    if( lw->list.h_scroll_bar ) {
	slider_size = lw->list.clip_window->core.width;
	if( slider_size > lw->core.width )
	    slider_size = lw->core.width;
	if( slider_size == 0 )
	    slider_size = 1;

	shown = (float)lw->list.clip_window->core.width / (float)lw->core.width;
	top = (float)-lw->core.x / (float)lw->core.width;
	XawScrollbarSetThumb( lw->list.h_scroll_bar, top, shown );
    }
}


static void
#if NeedFunctionPrototypes
MoveCoreWidget( XmwListWidget lw, int old_item )
#else /* NeedFunctionPrototypes */
MoveCoreWidget( lw, old_item )
    XmwListWidget	lw;
    int			old_item;
#endif /* NeedFunctionPrototypes */
{
    float			value;
    Position			x, y;
    int				item, top, bottom, left, right;
    XmwListCallbackStruct	call_value;

    x = lw->core.x;
    y = lw->core.y;

    if( lw->list.v_scroll_bar ) {
	XtVaGetValues( lw->list.v_scroll_bar, XtNtopOfThumb, &value, NULL );
	value *= (float)lw->core.height;
	item = lw->list.last_selected_item / lw->list.num_rows;
	top = ( (int)value + lw->list.base_height - 1 ) / lw->list.base_height;
	bottom = ( (int)value + lw->list.clip_window->core.height ) /
		 lw->list.base_height;
	if( item < top ) {
	    y = lw->list.last_selected_item / lw->list.num_rows *
		-lw->list.base_height;
	    if( lw->core.height + y < lw->list.clip_window->core.height )
		y = lw->list.clip_window->core.height - lw->core.height;
	}
	else if( item >= bottom ) {
	    y = ( lw->list.last_selected_item / lw->list.num_rows + 1 ) *
		-lw->list.base_height +
		lw->list.clip_window->core.height - lw->list.dy;
	}
    }

    if( lw->list.h_scroll_bar ) {
	XtVaGetValues( lw->list.h_scroll_bar, XtNtopOfThumb, &value, NULL );
	value *= lw->core.height;
	item = lw->list.last_selected_item % lw->list.num_rows;
	left = ( value + lw->list.base_width - 1 ) / lw->list.base_width;
	right = ( value + lw->list.clip_window->core.width ) /
		lw->list.base_width;
	if( item < left ) {
	    x = item * -lw->list.base_height;
	    if( lw->core.width + x < lw->list.clip_window->core.width )
		x = lw->list.clip_window->core.width - lw->core.width;
	}
	else if( item >= right ) {
	    x = ( item + 1 ) * (-lw->list.base_width) +
		lw->list.clip_window->core.width;
	}
    }

    XtMoveWidget( (Widget)lw, x, y );
    if( lw->list.last_selected_item != old_item ) {
	BorderUnhighlight( lw, old_item );
	BorderHighlight( lw );
	if( lw->list.browse_selection_callback ) {
	    call_value.reason = XmwCR_BROWSE_SELECT;
	    call_value.event = NULL;
	    call_value.item = lw->list.last_selected_item;
	    XtCallCallbackList( (Widget)lw, lw->list.browse_selection_callback,
				&call_value );
	}
    }

    SetScrollBar( lw );
}



static void
#if NeedFunctionPrototypes
ClipWindowResize( Widget w, XtPointer client_data, XtPointer call_data )
#else /* NeedFunctionPrototypes */
ClipWindowResize( w, client_data, call_data )
    Widget	w;
    XtPointer	client_data;
    XtPointer	call_data;
#endif /* NeedFunctionPrototypes */
{
    XmwListWidget		lw = (XmwListWidget)client_data;

    if( lw->core.width < lw->list.clip_window->core.width ) {
	lw->list.visible_hitem_count = lw->list.num_rows;
	XtMoveWidget( (Widget)lw, 0, lw->core.y );
    }
    else {
	lw->list.visible_hitem_count = lw->list.clip_window->core.width /
				       lw->list.base_width;
	if( lw->core.width + lw->core.x < lw->list.clip_window->core.width )
	    XtMoveWidget( (Widget)lw,
			  lw->list.clip_window->core.width - lw->core.width,
			  lw->core.y );
    }
    if( lw->core.height < lw->list.clip_window->core.height ) {
	lw->list.visible_vitem_count = lw->list.num_columns;
	XtMoveWidget( (Widget)lw, lw->core.x, 0 );
    }
    else {
	lw->list.visible_vitem_count = lw->list.clip_window->core.height /
				       lw->list.base_height;
	if( lw->core.height + lw->core.y < lw->list.clip_window->core.height )
	    XtMoveWidget( (Widget)lw,
			  lw->core.x, 
			  lw->list.clip_window->core.height - lw->core.height );
    }
    SetScrollBar( lw );
}



static void
#if NeedFunctionPrototypes
SetNormalGC( XmwListWidget lw )
#else /* NeedFunctionPrototypes */
SetNormalGC( lw )
    XmwListWidget	lw;
#endif /* NeedFunctionPrototypes */
{
    XGCValues	values;
    XtGCMask	mask;

    mask = GCForeground | GCBackground | GCGraphicsExposures
	   | GCLineWidth | GCCapStyle;
    values.foreground = lw->list.foreground;
    values.background = lw->core.background_pixel;
    values.graphics_exposures = False;
    values.line_width = lw->list.border_width;
    values.cap_style = CapProjecting;
    lw->list.foreground_gc = XtGetGC( (Widget)lw, mask, &values );

    values.line_width = lw->list.highlight_thickness;
    lw->list.highlight_gc = XtGetGC( (Widget)lw, mask, &values );
}


static void
#if NeedFunctionPrototypes
SetBackgroundGC( XmwListWidget lw )
#else /* NeedFunctionPrototypes */
SetBackgroundGC( lw )
    XmwListWidget	lw;
#endif /* NeedFunctionPrototypes */
{
    XGCValues	values;
    XtGCMask	mask;

    mask = GCForeground | GCFillStyle | GCGraphicsExposures
	   | GCLineWidth | GCCapStyle;
    values.foreground = lw->list.background;
    values.fill_style = FillSolid;
    values.graphics_exposures = False;
    values.line_width = lw->list.border_width;
    values.cap_style = CapProjecting;
    lw->list.background_gc = XtGetGC( (Widget)lw, mask, &values );

    values.foreground = lw->core.background_pixel;
    values.line_width = lw->list.highlight_thickness;
    lw->list.unhighlight_gc = XtGetGC( (Widget)lw, mask, &values );
}



static void
#if NeedFunctionPrototypes
ClassInitialize( void )
#else /* NeedFunctionPrototypes */
ClassInitialize()
#endif /* NeedFunctionPrototypes */
{
    XawInitializeWidgetSet();
    XtAddConverter( XtRString, XtRJustify, XmuCvtStringToJustify, NULL, 0 );
}


static void
#if NeedFunctionPrototypes
Resize( Widget w )
#else /* NeedFunctionPrototypes */
Resize( w )
    Widget	w;
#endif /* NeedFunctionPrototypes */
{
    XmwListWidget	lw = (XmwListWidget)w;
    Position		x, y;
    Dimension		width, height;
    XFontSetExtents	*fse;
    int			n, offset;
    XRectangle		ink, logical;
    short		row, column;

    if( lw->core.width == 0  &&  lw->core.height == 0 ) {
	for( n = offset = width = 0; n < lw->list.item_count; n++ ) {
	    if( lw->list.items[offset] != '\0' ) {
		XmbTextExtents( lw->list.font_set, &lw->list.items[offset],
				lw->list.par_item_length, &ink, &logical );
		if( width < logical.width )
		    width = logical.width;
	    }
	    offset += lw->list.par_item_length;
	}
	lw->list.font_width = width;

	fse = XExtentsOfFontSet( lw->list.font_set );
	lw->list.font_height = fse->max_logical_extent.height;
	lw->list.ascent = -(fse->max_logical_extent.y);

	width = lw->list.font_width +
		(lw->list.border_width + lw->list.highlight_thickness +
		 lw->list.margin_width) * 2; 
	lw->core.width = width * lw->list.num_rows +
			 lw->list.dx * (lw->list.num_rows + 1);
	lw->list.base_width = width + lw->list.dx;

	height = lw->list.font_height +
		(lw->list.border_width + lw->list.highlight_thickness +
		 lw->list.margin_height) * 2; 
	lw->core.height = height * lw->list.num_columns +
			  lw->list.dy * (lw->list.num_columns + 1);
	lw->list.base_height = height + lw->list.dy;
    }

    x = y = 0;
    if( lw->list.last_selected_item != -1 ) {
	row = lw->list.last_selected_item % lw->list.num_rows;
	column = lw->list.last_selected_item / lw->list.num_rows;
	if( row >= lw->list.visible_hitem_count ) {
	    x = row * -lw->list.base_width;
	    if( lw->core.width + x < lw->list.clip_window->core.width )
		x = lw->list.clip_window->core.width - lw->core.width;
	}
	if( column >= lw->list.visible_vitem_count ) {
	    y = column * -lw->list.base_height;
	    if( lw->core.height + y < lw->list.clip_window->core.height )
		y = lw->list.clip_window->core.height - lw->core.height;
	}
    }
    XtMoveWidget( w, x, y );

    SetScrollBar( lw );
}


static void
#if NeedFunctionPrototypes
Initialize( Widget request, Widget new, ArgList arg_list, Cardinal *num_args )
#else /* NeedFunctionPrototypes */
Initialize( request, new, arg_list, num_args )
    Widget	request, new;
    ArgList	arg_list;
    Cardinal	*num_args;
#endif /* NeedFunctionPrototypes */
{
    XmwListWidget	rw = (XmwListWidget)request;
    XmwListWidget	nw = (XmwListWidget)new;
    int			n;
    Arg			args[30];
    Boolean		calculate = False;
    int			vitem_count, hitem_count;
    short		row, column;

    nw->list.items = XtMalloc( nw->list.item_length + 1 );
    bcopy( rw->list.items, nw->list.items, nw->list.item_length );
    nw->list.items[nw->list.item_length] = '\0';

    if( nw->list.item_count == 0 )
	nw->list.item_count = 1;
    if( nw->list.num_rows == 0  &&  nw->list.num_columns == 0 ) {
	nw->list.num_rows = 1;
	nw->list.num_columns = nw->list.item_count;
	vitem_count = nw->list.visible_vitem_count;
	hitem_count = nw->list.visible_hitem_count;
	calculate = True;
    }
    if( nw->list.num_rows == 0 )
	nw->list.num_rows = (nw->list.item_count + nw->list.num_columns - 1) /
			       nw->list.num_columns;
    if( nw->list.num_columns == 0 )
	nw->list.num_columns = (nw->list.item_count + nw->list.num_rows - 1) /
			       nw->list.num_rows;

    if( nw->list.visible_vitem_count == 0  ||
	nw->list.visible_vitem_count > nw->list.num_columns )
	nw->list.visible_vitem_count = nw->list.num_columns;
    if( nw->list.visible_hitem_count == 0  ||
	nw->list.visible_hitem_count > nw->list.num_rows )
	nw->list.visible_hitem_count = nw->list.num_rows;

    nw->list.par_item_length = nw->list.item_length / nw->list.item_count;
    nw->list.button_press = False;
    nw->list.armed = False;
    nw->list.enter = False;
    nw->list.font_width = 0;
    nw->list.font_height = 0;
    nw->list.ascent = 0;

    SetNormalGC( nw );
    SetBackgroundGC( nw );

    nw->list.last_selected_item = nw->list.selected_item;
    if( nw->list.selected_item == -1 ) {
	for( n = 0; n < nw->list.item_count; n++ ) {
	    if( nw->list.items[nw->list.par_item_length*n] != '\0' ) {
		nw->list.last_selected_item = n;
		if( nw->list.highlight_always )
		    nw->list.selected_item = n;
		break;
	    }
	}
    }

    if( XtIsSubclass(nw->core.parent, viewportWidgetClass) ) {
	nw->list.viewport_window = nw->core.parent;
	nw->list.clip_window = XtNameToWidget( nw->core.parent, "clip" );
    }
    else {
	nw->list.clip_window = NULL;
	nw->list.viewport_window = NULL;
    }
    nw->list.v_scroll_bar = nw->list.h_scroll_bar = NULL;

    nw->core.width = nw->core.height = 0;
    Resize( new );
    if( calculate ) {
	for( row = 1; row < nw->list.item_count; row++ ) {
	    column = (nw->list.item_count + row - 1) / row;
	    if( row * nw->list.base_width > column * nw->list.base_height  ||
		row >= hitem_count )
		break;
	}
	nw->list.num_rows = row;
	nw->list.num_columns = (nw->list.item_count + row - 1) / row;
	if( vitem_count == 0  ||  vitem_count > nw->list.num_columns )
	    nw->list.visible_vitem_count = nw->list.num_columns;
	else
	    nw->list.visible_vitem_count = vitem_count;
	if( hitem_count == 0  ||  hitem_count > nw->list.num_rows )
	    nw->list.visible_hitem_count = nw->list.num_rows;
	else
	    nw->list.visible_hitem_count = hitem_count;
	nw->core.width = nw->list.base_width * nw->list.num_rows + nw->list.dx;
	nw->core.height = nw->list.base_height * nw->list.num_columns +
			  nw->list.dy;
	Resize( new );
    }

    if( !nw->list.clip_window )
	return;
    n = 0;
    XtSetArg( args[n], XtNwidth,
	      nw->list.visible_hitem_count * nw->list.base_width +
	      nw->list.dx );						n++;
    XtSetArg( args[n], XtNheight,
	      nw->list.visible_vitem_count * nw->list.base_height +
	      nw->list.dy );						n++;
    XtSetArg( args[n], XtNbackground, nw->core.background_pixel );	n++;
    XtSetArg( args[n], XtNbackgroundPixmap,
		       nw->core.background_pixmap );			n++;
    XtSetValues( nw->list.clip_window, args, n );
    XtSetValues( nw->list.viewport_window, args, n );
    Resize( new );

    if( nw->core.height > nw->list.clip_window->core.height )
	XtVaSetValues( nw->list.viewport_window, XtNallowVert, True, NULL );

    if( nw->core.width > nw->list.clip_window->core.width )
	XtVaSetValues( nw->list.viewport_window, XtNallowHoriz, True, NULL );
}


static void
#if NeedFunctionPrototypes
Realize( Widget widget, Mask *vmask, XSetWindowAttributes *attr )
#else /* NeedFunctionPrototypes */
Realize( widget, vmask, attr )
    Widget			widget;
    Mask			*vmask;
    XSetWindowAttributes	*attr;
#endif /* NeedFunctionPrototypes */
{
    XmwListWidget	w = (XmwListWidget)widget;
    int			n;
    Arg			args[10];
    Position		x;
    Dimension		width, wc, hc, bw;
    XtWidgetGeometry	request;

    (*(&simpleClassRec)->core_class.realize)( widget, vmask, attr );

    if( !w->list.clip_window )
	return;

    if( w->core.height > w->list.clip_window->core.height ) {
	w->list.v_scroll_bar = XtNameToWidget( w->list.viewport_window,
					       "vertical" );

	n = 0;
	XtSetArg( args[n], XtNborderColor, w->list.foreground );	n++;
	/* Xaw BUG */
	/*XtSetArg( args[n], XtNforeground, w->list.foreground );	n++;*/
	XtSetArg( args[n], XtNbackground, w->list.background );		n++;
	XtSetArg( args[n], XtNbackgroundPixmap,
			   w->core.background_pixmap );			n++;
	XtSetArg( args[n], XtNsensitive, w->core.sensitive );		n++;
	XtSetValues( w->list.v_scroll_bar, args, n );

	XtVaGetValues( w->list.viewport_window, XtNwidth, &width, NULL );
	XtVaGetValues( w->list.clip_window, XtNx, &x, XtNwidth, &wc,
		       XtNheight, &hc, XtNborderWidth, &bw, NULL );

	request.request_mode = CWWidth;
	request.width = x + width;
	XtMakeGeometryRequest( w->list.viewport_window, &request, NULL );
	request.width = x + wc;
	XtMakeGeometryRequest( w->list.clip_window, &request, NULL );
	/*request.width = x + w->core.width;*/
	/*XtMakeGeometryRequest( widget, &request, NULL );*/
	/*XtResizeWidget( w->list.clip_window, x + wc, hc, bw );*/
	/*XtResizeWidget( w, x + wc, hc, bw );*/
	/*XtVaSetValues( w->list.viewport_window, XtNwidth, x + width, NULL );*/
    }

    if( w->core.width > w->list.clip_window->core.width ) {
	w->list.h_scroll_bar = XtNameToWidget( w->list.viewport_window,
					       "horizontal" );

	n = 0;
	XtSetArg( args[n], XtNborderColor, w->list.foreground );	n++;
	XtSetArg( args[n], XtNforeground, w->list.foreground );		n++;
	XtSetArg( args[n], XtNbackground, w->list.background );		n++;
	XtSetArg( args[n], XtNbackgroundPixmap,
			   w->core.background_pixmap );			n++;
	XtSetArg( args[n], XtNsensitive, w->core.sensitive );		n++;
	XtSetValues( w->list.h_scroll_bar, args, n );
    }

    SetScrollBar( w );
}


static Boolean
#if NeedFunctionPrototypes
SetValues( Widget cw, Widget rw, Widget nw, ArgList args, Cardinal *num_args )
#else /* NeedFunctionPrototypes */
SetValues( cw, rw, nw, args, num_args )
    Widget	cw, rw, nw;
    ArgList	args;
    Cardinal	*num_args;
#endif /* NeedFunctionPrototypes */
{
    Boolean		flag = False;
    XmwListWidget	current = (XmwListWidget)cw;
    XmwListWidget	request = (XmwListWidget)rw;
    XmwListWidget	new = (XmwListWidget)nw;

    if( current->list.base_width != request->list.base_width )
	new->list.base_width = current->list.base_width;

    if( current->list.base_height != request->list.base_height )
	new->list.base_height = current->list.base_height;

    if( current->list.num_rows != request->list.num_rows )
	new->list.num_rows = current->list.num_rows;

    if( current->list.num_columns != request->list.num_columns )
	new->list.num_columns = current->list.num_columns;

    if( current->list.visible_vitem_count != request->list.visible_vitem_count )
	new->list.visible_vitem_count = current->list.visible_vitem_count;

    if( current->list.visible_hitem_count != request->list.visible_hitem_count )
	new->list.visible_hitem_count = current->list.visible_hitem_count;

    if( current->list.item_count != request->list.item_count )
	new->list.item_count = current->list.item_count;

    if( current->list.item_length != request->list.item_length )
	new->list.item_length = current->list.item_length;

    if( current->list.items != request->list.items )
	new->list.items = current->list.items;

    if( current->list.justify != request->list.justify )
	new->list.justify = current->list.justify;

    if( current->list.highlight_thickness != request->list.highlight_thickness )
	new->list.highlight_thickness = current->list.highlight_thickness;

    if( current->list.border_width != request->list.border_width )
	new->list.border_width = current->list.border_width;

    if( current->list.highlight_always != request->list.highlight_always )
	new->list.highlight_always = current->list.highlight_always;

    if( new->list.font_set != current->list.font_set )
	XFreeFontSet( XtDisplay(cw), current->list.font_set );

    if( new->list.foreground != current->list.foreground  ||
	new->core.background_pixel != current->core.background_pixel ) {
	XtReleaseGC( cw, current->list.foreground_gc );
	XtReleaseGC( cw, current->list.highlight_gc );
	SetNormalGC( new );
	flag = True;
    }

    if( new->list.background != current->list.background ) {
	XtReleaseGC( cw, current->list.background_gc );
	XtReleaseGC( cw, current->list.unhighlight_gc );
	SetBackgroundGC( new );
	flag = True;
    }

    if( new->list.font_set != current->list.font_set  ||
	new->list.margin_width != current->list.margin_width  ||
	new->list.margin_height != current->list.margin_height ) {
	new->core.width = new->core.height = 0;
	Resize( nw );
	flag = True;
    }

    if( (new->list.selected_item != current->list.selected_item  &&
	 new->list.selected_item == -1)  ||
	(new->list.selected_item != -1  &&
	 new->list.items[new->list.par_item_length*
			 new->list.selected_item] == '\0') ) {
	new->list.selected_item = current->list.selected_item;
    }
    else if( new->list.selected_item != current->list.selected_item )
	MoveCoreWidget( new, current->list.selected_item );

    return( flag );
}


static void
#if NeedFunctionPrototypes
Destroy( Widget w )
#else /* NeedFunctionPrototypes */
Destroy( w )
    Widget	w;
#endif /* NeedFunctionPrototypes */
{
    XmwListWidget	lw = (XmwListWidget)w;

    XtFree( lw->list.items );

    XtRemoveAllCallbacks( w, XmwNnotifyCallback );
    XtRemoveAllCallbacks( w, XmwNsetCallback );
    XtRemoveAllCallbacks( w, XmwNunsetCallback );
    XtRemoveAllCallbacks( w, XmwNbrowseSelectionCallback );

    XtReleaseGC( w, lw->list.foreground_gc );
    XtReleaseGC( w, lw->list.background_gc );
    XtReleaseGC( w, lw->list.highlight_gc );
    XtReleaseGC( w, lw->list.unhighlight_gc );

    XFreePixmap( XtDisplay(w), lw->list.pixmap );
}


static void
#if NeedFunctionPrototypes
Redisplay( Widget wi, XEvent *event, Region region )
#else /* NeedFunctionPrototypes */
Redisplay( wi, event, region )
    Widget	wi;
    XEvent	*event;
    Region	region;
#endif /* NeedFunctionPrototypes */
{
    XmwListWidget	lw = (XmwListWidget)wi;
    int			dx, dy, w, h, width, height, ww, hh, bw, ht, dw;
    int			num, sx, sy, ex, ey;
    GC			foregc, backgc;
    XPoint		points[4];
    Boolean		destroyRegion = False;
    int			n, offset;

    bw = lw->list.border_width;
    ht = lw->list.highlight_thickness;
    ww = bw + lw->list.margin_width + lw->list.highlight_thickness;
    hh = bw + lw->list.margin_height + lw->list.highlight_thickness;
    width = lw->list.font_width + ww * 2;
    height = lw->list.font_height + hh * 2;
    w = width + lw->list.dx;
    h = height + lw->list.dy;

    if( region == NULL ) {
	sx = lw->list.last_selected_item % lw->list.num_rows * w;
	sy = lw->list.last_selected_item / lw->list.num_rows * h;
	points[0].x = points[3].x = sx;
	points[0].y = points[1].y = sy;
	points[1].x = points[2].x = sx + 1;
	points[2].y = points[3].y = sy + 1;
	region = XPolygonRegion( points, 4, WindingRule );
	destroyRegion = True;
    }

    if( XtIsRealized((Widget)lw)  &&  lw->list.highlight_thickness != 0 ) {
	foregc = lw->list.armed ? lw->list.background_gc
				: lw->list.foreground_gc;
	backgc = lw->list.armed ? lw->list.foreground_gc
				: lw->list.background_gc;
	for( num = 0; num < region->numRects; num++ ) {
	    sy = region->rects[num].y1 / h;
	    ex = ( region->rects[num].x2 + ( w - 1 ) ) / w;
	    ey = ( region->rects[num].y2 + ( h - 1 ) ) / h;
	    dy = h * sy + lw->list.dy;
	    if( ex > lw->list.visible_hitem_count )
		ex = lw->list.visible_hitem_count;
	    for( ;sy < ey; sy++ ) {
		sx = region->rects[num].x1 / w;
		dx = w * sx + lw->list.dx;
		for( ; sx < ex; sx++ ) {
		    if( (n = lw->list.num_rows * sy + sx) >=
			lw->list.item_count )
			break;
		    offset = lw->list.par_item_length * n;
		    if( lw->list.items[offset] != '\0' ) {
			XFillRectangle( XtDisplay(lw), XtWindow(lw), backgc,
					dx + bw + ht, dy + bw + ht,
					width - (bw + ht) * 2,
					height - (bw + ht) * 2 );
			XDrawRectangle( XtDisplay(lw), XtWindow(lw),
					lw->list.foreground_gc,
					dx + bw/2, dy + bw/2,
					width - bw, height - bw );
			switch( lw->list.justify ) {
			  case XtJustifyLeft:
			    dw = 0;
			    break;

			  case XtJustifyCenter:
			    dw = lw->list.font_width
			       - XmbTextEscapement( lw->list.font_set,
						    &lw->list.items[offset],
						    min(lw->list.par_item_length, strlen(&lw->list.items[offset])) );
			    dw /= 2;
			    break;

			  case XtJustifyRight:
			    dw = lw->list.font_width
			       - XmbTextEscapement( lw->list.font_set,
						    &lw->list.items[offset],
						    min(lw->list.par_item_length, strlen(&lw->list.items[offset])) );
			}
			XmbDrawString( XtDisplay(lw), XtWindow(lw),
				       lw->list.font_set, foregc,
				       dx + ww + dw,
				       dy + hh + lw->list.ascent,
				       &lw->list.items[offset],
				       lw->list.par_item_length );
		    }
		    dx += w;
		}
		dy += h;
	    }
	}
	BorderHighlight( lw );

	if( destroyRegion )
	    XDestroyRegion( region );
    }
}



static void
#if NeedFunctionPrototypes
Set( Widget w, XEvent *event, String *params, Cardinal *num_params )
#else /* NeedFunctionPrototypes */
Set( w, event, params, num_params )
    Widget	w;
    XEvent	*event;
    String	*params;
    Cardinal	*num_params;
#endif /* NeedFunctionPrototypes */
{
    XmwListWidget		lw = (XmwListWidget)w;
    XmwListCallbackStruct	call_value;
    int				item;

    if( lw->list.button_press  ||  lw->list.armed )
	return;
    if( (item = Event2Item( lw, event )) == -1 )
	return;

    if( lw->list.selected_item != item ) {
	BorderUnhighlight( lw, lw->list.selected_item );
	if( lw->list.browse_selection_callback ) {
	    lw->list.selected_item = item;
	    call_value.reason = XmwCR_BROWSE_SELECT;
	    call_value.event = event;
	    call_value.item = item;
	    XtCallCallbackList( w, lw->list.browse_selection_callback,
				&call_value );
	}
    }

    lw->list.selected_item = item;
    if( item != -1 )
	lw->list.last_selected_item = item;
    lw->list.button_press = lw->list.armed = True;
    Redisplay( w, event, (Region)NULL );

    if( lw->list.set_callback ) {
	XFlush( XtDisplay(w) );
	call_value.reason = XmwCR_SET;
	call_value.event = event;
	call_value.item = item;
	XtCallCallbackList( w, lw->list.set_callback, &call_value );
    }
}


static void
#if NeedFunctionPrototypes
Notify( Widget w, XEvent *event, String *params, Cardinal *num_params )
#else /* NeedFunctionPrototypes */
Notify( w, event, params, num_params )
    Widget	w;
    XEvent	*event;
    String	*params;
    Cardinal	*num_params;
#endif /* NeedFunctionPrototypes */
{
    XmwListWidget		lw = (XmwListWidget)w;
    XmwListCallbackStruct	call_value;

    if( lw->list.armed == False )
	return;

    lw->list.armed = False;
    Redisplay( w, event, (Region)NULL );

    call_value.item = Event2Item( lw, event );

    if( lw->list.selected_item != call_value.item )
	return;

    if( lw->list.notify_callback ) {
	call_value.reason = XmwCR_NOTIFY;
	call_value.event = event;
	XtCallCallbackList( w, lw->list.notify_callback, &call_value );
    }
}


static void
#if NeedFunctionPrototypes
Unset( Widget w, XEvent *event, String *params, Cardinal *num_params )
#else /* NeedFunctionPrototypes */
Unset( w, event, params, num_params )
    Widget	w;
    XEvent	*event;
    String	*params;
    Cardinal	*num_params;
#endif /* NeedFunctionPrototypes */
{
    XmwListWidget		lw = (XmwListWidget)w;
    XmwListCallbackStruct	call_value;

    if( !lw->list.button_press )
	return;
    lw->list.button_press = False;

    if( lw->list.armed ) {
	lw->list.armed = False;
	Redisplay( w, event, (Region)NULL );
    }

    if( lw->list.unset_callback ) {
	XFlush( XtDisplay(w) );
	call_value.reason = XmwCR_UNSET;
	call_value.event = event;
	call_value.item = Event2Item( lw, event );
	XtCallCallbackList( w, lw->list.unset_callback, &call_value );
    }
}


static void
#if NeedFunctionPrototypes
Enter( Widget w, XEvent *event, String *params, Cardinal *num_params )
#else /* NeedFunctionPrototypes */
Enter( w, event, params, num_params )
    Widget	w;
    XEvent	*event;
    String	*params;
    Cardinal	*num_params;
#endif /* NeedFunctionPrototypes */
{
    XmwListWidget	lw = (XmwListWidget)w;

    lw->list.enter = True;
    if( !lw->list.highlight_always  &&  !lw->list.button_press )
	BorderHighlight( lw );
}


static void
#if NeedFunctionPrototypes
Leave( Widget w, XEvent *event, String *params, Cardinal *num_params )
#else /* NeedFunctionPrototypes */
Leave( w, event, params, num_params )
    Widget	w;
    XEvent	*event;
    String	*params;
    Cardinal	*num_params;
#endif /* NeedFunctionPrototypes */
{
    XmwListWidget	lw = (XmwListWidget)w;

    lw->list.enter = False;
    if( lw->list.armed == True ) {
	lw->list.armed = False;
	Redisplay( w, event, (Region)NULL );
    }
    if( !lw->list.highlight_always  &&  !lw->list.button_press )
	BorderUnhighlight( lw, lw->list.selected_item );
}


static void
#if NeedFunctionPrototypes
Motion( Widget w, XEvent *event, String *params, Cardinal *num_params )
#else /* NeedFunctionPrototypes */
Motion( w, event, params, num_params )
    Widget	w;
    XEvent	*event;
    String	*params;
    Cardinal	*num_params;
#endif /* NeedFunctionPrototypes */
{
    XmwListWidget		lw = (XmwListWidget)w;
    XmwListCallbackStruct	call_value;
    int				item;

    item = Event2Item( lw, event );
    if( lw->list.button_press ) {
	if( lw->list.armed == False ) {
	    if( item != lw->list.selected_item )
		return;
	    lw->list.armed = True;
	}
	else {
	    if( item == lw->list.selected_item )
		return;
	    lw->list.armed = False;
	}
	Redisplay( w, event, (Region)NULL );
    }
    else {
	if( item != lw->list.selected_item ) {
	    if( !lw->list.highlight_always ) {
		BorderUnhighlight( lw, lw->list.selected_item );
		lw->list.selected_item = item;
	    }
	    if( item != -1 ) {
		if( lw->list.highlight_always ) {
		    BorderUnhighlight( lw, lw->list.selected_item );
		    lw->list.selected_item = item;
		}
		lw->list.last_selected_item = item;
		if( lw->list.browse_selection_callback ) {
		    call_value.reason = XmwCR_BROWSE_SELECT;
		    call_value.event = event;
		    call_value.item = item;
		    XtCallCallbackList( w, lw->list.browse_selection_callback,
					&call_value );
		}
		BorderHighlight( lw );
	    }
	}
    }
}


static void
#if NeedFunctionPrototypes
PreviousItem( Widget w, XEvent *event, String *params, Cardinal *num_params )
#else /* NeedFunctionPrototypes */
PreviousItem( w, event, params, num_params )
    Widget	w;
    XEvent	*event;
    String	*params;
    Cardinal	*num_params;
#endif /* NeedFunctionPrototypes */
{
    XmwListPreviousItem( w );
}


static void
#if NeedFunctionPrototypes
NextItem( Widget w, XEvent *event, String *params, Cardinal *num_params )
#else /* NeedFunctionPrototypes */
NextItem( w, event, params, num_params )
    Widget	w;
    XEvent	*event;
    String	*params;
    Cardinal	*num_params;
#endif /* NeedFunctionPrototypes */
{
    XmwListNextItem( w );
}


static void
#if NeedFunctionPrototypes
UpItem( Widget w, XEvent *event, String *params, Cardinal *num_params )
#else /* NeedFunctionPrototypes */
UpItem( w, event, params, num_params )
    Widget	w;
    XEvent	*event;
    String	*params;
    Cardinal	*num_params;
#endif /* NeedFunctionPrototypes */
{
    XmwListUpItem( w );
}


static void
#if NeedFunctionPrototypes
DownItem( Widget w, XEvent *event, String *params, Cardinal *num_params )
#else /* NeedFunctionPrototypes */
DownItem( w, event, params, num_params )
    Widget	w;
    XEvent	*event;
    String	*params;
    Cardinal	*num_params;
#endif /* NeedFunctionPrototypes */
{
    XmwListDownItem( w );
}


static void
#if NeedFunctionPrototypes
PageUpItem( Widget w, XEvent *event, String *params, Cardinal *num_params )
#else /* NeedFunctionPrototypes */
PageUpItem( w, event, params, num_params )
    Widget	w;
    XEvent	*event;
    String	*params;
    Cardinal	*num_params;
#endif /* NeedFunctionPrototypes */
{
    XmwListPageUpItem( w );
}


static void
#if NeedFunctionPrototypes
PageDownItem( Widget w, XEvent *event, String *params, Cardinal *num_params )
#else /* NeedFunctionPrototypes */
PageDownItem( w, event, params, num_params )
    Widget	w;
    XEvent	*event;
    String	*params;
    Cardinal	*num_params;
#endif /* NeedFunctionPrototypes */
{
    XmwListPageDownItem( w );
}



static XtActionsRec	actionsList[] =
{
    { "Set",		Set		},
    { "Notify",		Notify		},
    { "Unset",		Unset		},
    { "Enter",		Enter		},
    { "Leave",		Leave		},
    { "Motion",		Motion		},
    { "PreviousItem",	PreviousItem	},
    { "NextItem",	NextItem	},
    { "UpItem",		UpItem		},
    { "DownItem",	DownItem	},
    { "PageUpItem",	PageUpItem	},
    { "PageDownItem",	PageDownItem	},
};


#define Offset(field)	XtOffsetOf(XmwListRec, field)
static XtResource	local_resources[] =
{
  {
    XmwNitemBorderWidth, XmwCItemBorderWidth,
    XtRDimension, sizeof(Dimension), Offset(list.border_width),
    XtRImmediate, (XtPointer)1 },
  {
    XmwNhighlightThickness, XtCThickness,
    XtRDimension, sizeof(Dimension), Offset(list.highlight_thickness),
    XtRImmediate, (XtPointer)2 },
  {
    XmwNmarginWidth, XmwCMarginWidth,
    XtRDimension, sizeof(Dimension), Offset(list.margin_width),
    XtRImmediate, (XtPointer)2 },
  {
    XmwNmarginHeight, XmwCMarginHeight,
    XtRDimension, sizeof(Dimension), Offset(list.margin_height),
    XtRImmediate, (XtPointer)2 },
  {
    XtNhorizDistance, XtCThickness,
    XtRInt, sizeof(int), Offset(list.dx),
    XtRImmediate, (XtPointer)4 },
  {
    XtNvertDistance, XtCThickness,
    XtRInt, sizeof(int), Offset(list.dy),
    XtRImmediate, (XtPointer)4 },
  {
    XtNjustify, XtCJustify,
    XtRJustify, sizeof(XtJustify), Offset(list.justify),
    XtRImmediate, (XtPointer)XtJustifyCenter },
  {
    XmwNitemBaseHeight, XmwCItemBaseHeight,
    XtRDimension, sizeof(Dimension), Offset(list.base_height),
    XtRImmediate, (XtPointer)0 },
  {
    XmwNitemBaseWidth, XmwCItemBaseWidth,
    XtRDimension, sizeof(Dimension), Offset(list.base_width),
    XtRImmediate, (XtPointer)0 },
  {
    XtNfontSet, XtCFontSet,
    XtRFontSet, sizeof(XFontSet), Offset(list.font_set),
    XtRString, XtDefaultFontSet },
  {
    XtNforeground, XtCForeground,
    XtRPixel, sizeof(Pixel), Offset(list.foreground),
    XtRString, XtDefaultForeground },
  {
    XmwNitemBackground, XtCBackground,
    XtRPixel, sizeof(Pixel), Offset(list.background),
    XtRString, XtDefaultBackground },
  {
    XmwNselectedItem, XmwCSelectedItem,
    XtRInt, sizeof(int), Offset(list.selected_item),
    XtRImmediate, (XtPointer)-1 },
  {
    XmwNnumRows, XmwCNumRows,
    XtRShort, sizeof(short), Offset(list.num_rows),
    XtRImmediate, (XtPointer)0 },
  {
    XmwNnumColumns, XmwCNumColumns,
    XtRShort, sizeof(short), Offset(list.num_columns),
    XtRImmediate, (XtPointer)0 },
  {
    XmwNvisibleVerticalItemCount, XmwCVisibleItemCount,
    XtRInt, sizeof(int), Offset(list.visible_vitem_count),
    XtRImmediate, (XtPointer)0 },
  {
    XmwNvisibleHorizontalItemCount, XmwCVisibleItemCount,
    XtRInt, sizeof(int), Offset(list.visible_hitem_count),
    XtRImmediate, (XtPointer)0 },
  {
    XmwNitemCount, XmwCItemCount,
    XtRInt, sizeof(int), Offset(list.item_count),
    XtRImmediate, (XtPointer)0 },
  {
    XmwNitemLength, XmwCItemLength,
    XtRInt, sizeof(int), Offset(list.item_length),
    XtRImmediate, (XtPointer)0 },
  {
    XmwNitems, XmwCItems,
    XtRString, sizeof(String), Offset(list.items),
    XtRPointer, (XtPointer)NULL },
  {
    XmwNhighlightAlways, XmwCHighlightAlways,
    XtRBoolean, sizeof(Boolean), Offset(list.highlight_always),
    XtRImmediate, (XtPointer)False },
  {
    XmwNnotifyCallback, XtCCallback,
    XtRCallback, sizeof(XtCallbackList), Offset(list.notify_callback),
    XtRPointer, (XtPointer)NULL },
  {
    XmwNsetCallback, XtCCallback,
    XtRCallback, sizeof(XtCallbackList), Offset(list.set_callback),
    XtRPointer, (XtPointer)NULL },
  {
    XmwNunsetCallback, XtCCallback,
    XtRCallback, sizeof(XtCallbackList), Offset(list.unset_callback),
    XtRPointer, (XtPointer)NULL },
  {
    XmwNbrowseSelectionCallback, XtCCallback,
    XtRCallback, sizeof(XtCallbackList), Offset(list.browse_selection_callback),
    XtRPointer, (XtPointer)NULL },
};
#undef Offset


XmwListClassRec	xmwListClassRec = 
{
  { /* core_class record	*/
    /* superclass		*/	(WidgetClass)&simpleClassRec,
    /* class_name		*/	"XmwList",
    /* widget_size		*/	sizeof(XmwListRec),
    /* class_initialize		*/	ClassInitialize,
    /* class_part_initialize	*/	NULL,
    /* class_inited		*/	False,
    /* initialize		*/	Initialize,
    /* initialize_hook		*/	NULL,
    /* realize			*/	Realize,
    /* actions			*/	actionsList,
    /* num_actions		*/	XtNumber(actionsList),
    /* resources		*/	local_resources,
    /* num_resources		*/	XtNumber(local_resources),
    /* xrm_class		*/	NULLQUARK,
    /* compress_motion		*/	False,
    /* compress_exposure	*/	XtExposeCompressMaximal,
    /* compress_enterleave	*/	True,
    /* visible_interest		*/	False,
    /* destroy			*/	Destroy,
    /* resize			*/	Resize,
    /* expose			*/	Redisplay,
    /* set_values		*/	SetValues,
    /* set_values_hook		*/	NULL,
    /* set_values_almost	*/	XtInheritSetValuesAlmost,
    /* get_values_hook		*/	NULL,
    /* accept_focus		*/	NULL,
    /* version			*/	XtVersion,
    /* callback private		*/	NULL,
    /* tm_table			*/	defaultTranslations,
    /* query_geometry		*/	XtInheritQueryGeometry,
    /* display_accelerator	*/	NULL,
    /* extension		*/	NULL
  },
  { /* simple_class record	*/
    /* change_sensitive		*/	XtInheritChangeSensitive
  },
  { /* list_class record	*/
    /* extension		*/	NULL
  }
};



externaldef(xmwcodeinputwidgetclass)
    WidgetClass	xmwListWidgetClass = (WidgetClass)&xmwListClassRec;


Widget 
#if NeedFunctionPrototypes
XmwCreateScrolledList( Widget parent, char *name, ArgList arglist,
		       int argcount )
#else /* NeedFunctionPrototypes */
XmwCreateScrolledList( parent, name, arglist, argcount )
    Widget	parent;
    char	*name;
    ArgList	arglist;
    int		argcount;
#endif /* NeedFunctionPrototypes */
{
    Widget	vw, lw;
    int		n;
    char	*ss;
    ArgList	args;

    if( name ) {
	ss = XtMalloc( strlen(name) + 3 );
	strcpy( ss, name );
	strcat( ss, "SL" );
    }
    else {
	ss = XtMalloc( strlen(name) + 3 );
	strcpy( ss, "SL" );
    }

    args = (ArgList)XtCalloc( argcount+5, sizeof(Arg) );
    for( n = 0; n < argcount; n++ ) {
	args[n].name = arglist[n].name;
	args[n].value = arglist[n].value;
    }

    XtSetArg( args[n], XtNallowHoriz, False );			n++;
    XtSetArg( args[n], XtNallowVert, False );			n++;
    XtSetArg( args[n], XtNforceBars, True );			n++;
    if( parent->core.width ) {
	XtSetArg( args[n], XtNwidth, parent->core.width );	n++;
    }
    if( parent->core.height ) {
	XtSetArg( args[n], XtNheight, parent->core.height );	n++;
    }
    vw = XtCreateManagedWidget( ss, viewportWidgetClass, parent, args, n );
    XtFree( ss );
    XtFree( (char *)args );

    lw = XtCreateWidget( name, xmwListWidgetClass, vw, arglist, argcount );

    XtAddCallback( vw, XtNreportCallback, ClipWindowResize, lw );
    return( lw );
}


Widget 
#if NeedFunctionPrototypes
XmwCreateList( Widget parent, char *name, ArgList arglist,
		       int argcount )
#else /* NeedFunctionPrototypes */
XmwCreateList( parent, name, arglist, argcount )
    Widget	parent;
    char	*name;
    ArgList	arglist;
    int		argcount;
#endif /* NeedFunctionPrototypes */
{
    return( XtCreateWidget( name, xmwListWidgetClass, parent, arglist, argcount ) );
}


Boolean
#if NeedFunctionPrototypes
XmwListSelectItem( Widget w, int item )
#else /* NeedFunctionPrototypes */
XmwListSelectItem( w, item )
    Widget	w;
    int		item;
#endif /* NeedFunctionPrototypes */
{
    XmwListWidget	lw = (XmwListWidget)w;
    int			old_item;

    if( item < 0  ||  item >= lw->list.item_count  ||
	lw->list.items[lw->list.par_item_length*item] == '\0' )
	return( False );

    old_item = lw->list.last_selected_item;
    lw->list.selected_item = lw->list.last_selected_item = item;
    MoveCoreWidget( lw, old_item );
    return( True );
}


void
#if NeedFunctionPrototypes
XmwListPreviousItem( Widget w )
#else /* NeedFunctionPrototypes */
XmwListPreviousItem( w )
    Widget	w;
#endif /* NeedFunctionPrototypes */
{
    XmwListWidget		lw = (XmwListWidget)w;
    int				new_item, old_item;

    old_item = lw->list.last_selected_item;
    for( new_item = old_item - 1; new_item >= 0; new_item-- ) {
	if( lw->list.items[lw->list.par_item_length*new_item] != '\0' )
	    break;
    }
    if( new_item >= 0 ) {
	lw->list.selected_item = lw->list.last_selected_item = new_item;
	MoveCoreWidget( lw, old_item );
    }
}


void
#if NeedFunctionPrototypes
XmwListNextItem( Widget w )
#else /* NeedFunctionPrototypes */
XmwListNextItem( w )
    Widget	w;
#endif /* NeedFunctionPrototypes */
{
    XmwListWidget	lw = (XmwListWidget)w;
    int			new_item, old_item;

    old_item = lw->list.last_selected_item;
    for( new_item = old_item + 1; new_item < lw->list.item_count; new_item++ ) {
	if( lw->list.items[lw->list.par_item_length*new_item] != '\0' )
	    break;
    }
    if( new_item < lw->list.item_count ) {
	lw->list.selected_item = lw->list.last_selected_item = new_item;
	MoveCoreWidget( lw, old_item );
    }
}


void
#if NeedFunctionPrototypes
XmwListUpItem( Widget w )
#else /* NeedFunctionPrototypes */
XmwListUpItem( w )
    Widget	w;
#endif
{
    XmwListWidget	lw = (XmwListWidget)w;
    int			old_item;

    old_item = lw->list.last_selected_item;
    lw->list.last_selected_item -= lw->list.num_rows;
    while( lw->list.last_selected_item >= 0 ) {
	if( ItemCheck( lw ) ) {
	    lw->list.selected_item = lw->list.last_selected_item;
	    MoveCoreWidget( lw, old_item );
	    return;
	}
	lw->list.last_selected_item -= lw->list.num_rows;
    }
    lw->list.last_selected_item = old_item;
}


void
#if NeedFunctionPrototypes
XmwListDownItem( Widget w )
#else /* NeedFunctionPrototypes */
XmwListDownItem( w )
    Widget	w;
#endif /* NeedFunctionPrototypes */
{
    XmwListWidget	lw = (XmwListWidget)w;
    int			old_item, last_item;

    old_item = last_item = lw->list.last_selected_item;
    lw->list.last_selected_item += lw->list.num_rows;
    while( lw->list.last_selected_item < lw->list.item_count ) {
	if( ItemCheck( lw ) ) {
	    lw->list.selected_item = lw->list.last_selected_item;
	    MoveCoreWidget( lw, old_item );
	    return;
	}
	last_item = lw->list.last_selected_item;
	lw->list.last_selected_item += lw->list.num_rows;
    }
    if( lw->list.last_selected_item / lw->list.num_rows !=
	lw->list.num_columns ) {
	lw->list.last_selected_item = lw->list.item_count;
	do {
	    lw->list.last_selected_item--;
	    if( ItemCheck( lw ) ) {
		lw->list.selected_item = lw->list.last_selected_item;
		MoveCoreWidget( lw, old_item );
		return;
	    }
	} while( lw->list.last_selected_item > last_item );
    }
    lw->list.last_selected_item = old_item;
}


void
#if NeedFunctionPrototypes
XmwListPageUpItem( Widget w )
#else /* NeedFunctionPrototypes */
XmwListPageUpItem( w )
    Widget	w;
#endif /* NeedFunctionPrototypes */
{
    XmwListWidget	lw = (XmwListWidget)w;
    float		value;
    int			maximum;
    int			top, bottom, old_item;

    if( lw->list.v_scroll_bar == NULL )
	return;

    old_item = lw->list.last_selected_item;
    XtVaGetValues( lw->list.v_scroll_bar, XtNtopOfThumb, &value, NULL );
    value *= lw->core.height;
    top = ( value + lw->list.base_height - 1 ) / lw->list.base_height;
    bottom = ( value + lw->list.clip_window->core.height ) /
	     lw->list.base_height;
    value += lw->list.base_height * ( bottom - top );
    maximum = lw->core.height - lw->list.clip_window->core.height;
    if( value > maximum )
	value = maximum;

    XawScrollbarSetThumb( lw->list.v_scroll_bar, value / (float)lw->core.height,
			  (float)lw->list.clip_window->core.height /
			  (float)lw->core.height );
    XtMoveWidget( (Widget)lw, lw->core.x, (int)-value );

    top = (( (int)value + lw->list.base_height - 1 ) / lw->list.base_height) *
	  lw->list.num_rows;
    bottom = (( (int)value + lw->list.clip_window->core.height ) /
	      lw->list.base_height + 1) * lw->list.num_rows;
    if( bottom > lw->list.item_count )
	bottom = lw->list.item_count;
    while( top < bottom ) {
	if( lw->list.items[lw->list.par_item_length*top] ) {
	    lw->list.last_selected_item = top;
	    MoveCoreWidget( lw, old_item );
	    lw->list.selected_item = lw->list.last_selected_item;
	    return;
	}
	top++;
    }
    lw->list.last_selected_item = old_item;
}


void
#if NeedFunctionPrototypes
XmwListPageDownItem( Widget w )
#else /* NeedFunctionPrototypes */
XmwListPageDownItem( w )
    Widget	w;
#endif /* NeedFunctionPrototypes */
{
    XmwListWidget	lw = (XmwListWidget)w;
    float		value;
    int			top, bottom, old_item;

    if( lw->list.v_scroll_bar == NULL )
	return;

    old_item = lw->list.last_selected_item;
    XtVaGetValues( lw->list.v_scroll_bar, XtNtopOfThumb, &value, NULL );
    value *= lw->core.height;
    top = ( value + lw->list.base_height - 1 ) / lw->list.base_height;
    bottom = ( value + lw->list.clip_window->core.height ) /
	     lw->list.base_height;
    value -= lw->list.base_height * ( bottom - top );
    if( value < 0 )
	value = 0;

    XawScrollbarSetThumb( lw->list.v_scroll_bar, value / (float)lw->core.height,
			  (float)lw->list.clip_window->core.height /
			  (float)lw->core.height );
    XtMoveWidget( (Widget)lw, lw->core.x, (int)-value );

    top = (( (int)value + lw->list.base_height - 1 ) / lw->list.base_height) *
	  lw->list.num_rows;
    bottom = (( (int)value + lw->list.clip_window->core.height ) /
	      lw->list.base_height + 1) * lw->list.num_rows;
    if( bottom > lw->list.item_count )
	bottom = lw->list.item_count;
    while( top < bottom ) {
	if( lw->list.items[lw->list.par_item_length*top] ) {
	    lw->list.last_selected_item = top;
	    MoveCoreWidget( lw, old_item );
	    lw->list.selected_item = lw->list.last_selected_item;
	    return;
	}
	top++;
    }
    lw->list.last_selected_item = old_item;
}


void
#if NeedFunctionPrototypes
XmwListTopItem( Widget w )
#else /* NeedFunctionPrototypes */
XmwListTopItem( w )
    Widget	w;
#endif /* NeedFunctionPrototypes */
{
    XmwListWidget	lw = (XmwListWidget)w;
    float		value;
    int			top;

    XtVaGetValues( lw->list.v_scroll_bar, XtNtopOfThumb, &value, NULL );
    value *= lw->core.height;
    top = ( value + lw->list.base_height - 1 ) / lw->list.base_height;

    XmwListSelectItem( w, top );
}


void
#if NeedFunctionPrototypes
XmwListEndItem( Widget w )
#else /* NeedFunctionPrototypes */
XmwListEndItem( w )
    Widget	w;
#endif /* NeedFunctionPrototypes */
{
    XmwListWidget	lw = (XmwListWidget)w;
    float		value;
    int			end;

    XtVaGetValues( lw->list.v_scroll_bar, XtNtopOfThumb, &value, NULL );
    value *= lw->core.height;
    end = ( value + lw->list.clip_window->core.height ) / lw->list.base_height
	- 1;

    XmwListSelectItem( w, end );
}
