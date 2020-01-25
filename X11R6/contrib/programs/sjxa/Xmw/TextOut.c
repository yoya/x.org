/* $SonyId: TextOut.c,v 1.2 1994/06/03 06:37:18 makoto Exp $ */
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
#include	<stdlib.h>
#include	<ctype.h>
#ifdef		SVR4
#include	<widec.h>
#endif
#include	<X11/Xlib.h>
#include	<X11/Xlibint.h>
#include	<X11/keysym.h>
#include	<X11/IntrinsicP.h>
#include	<X11/StringDefs.h>
#include	<X11/Xaw/XawInit.h>
#include	<Xmw/TextOutP.h>
#include	<Xmw/XmwStrDefs.h>
#include	"SjString.h"


#define MAXINT		2147483647
#define ALLOC_SIZE	32


void
#if NeedFunctionPrototypes
XmwCvtStringToValueWcs( XrmValuePtr args, Cardinal *num_args,
			XrmValue *from_val, XrmValue *to_val )
#else /* NeedFunctionPrototypes */
XmwCvtStringToValueWcs( args, num_args, from_val, to_val )
    XrmValuePtr	args;
    Cardinal	*num_args;
    XrmValue	*from_val, *to_val;
#endif /* NeedFunctionPrototypes */
{
    char		*in_str = (char *)(from_val->addr);
    int			len;
    static wchar_t	*wcs;

    to_val->size = sizeof(wchar_t *);
    to_val->addr = (XPointer)&wcs;

    len = mbstowcs( NULL, in_str, strlen(in_str) );
    wcs = (wchar_t *)XtMalloc( sizeof(wchar_t) * (len + 1) );
    mbstowcs( wcs, in_str, strlen(in_str) );
}


static void
#if NeedFunctionPrototypes
ValidateString( XmwTextOutWidget w, char *value )
#else /* NeedFunctionPrototypes */
ValidateString( w, value )
    XmwTextOutWidget	w;
    char		*value;
#endif /* NeedFunctionPrototypes */
{
    int		len = strlen(value);
    wchar_t	*ws = (wchar_t *)XtMalloc(sizeof(wchar_t)*(unsigned)(len + 1));

    len = mbs2wcs( ws, value, len );
    if( len != -1 ) {
	XtFree( (char *)TextValueWcs(w) );
	w->text_out.string_length = len;
	w->text_out.alloc_size = sizeof(wchar_t) * ( len + ALLOC_SIZE );
	TextValueWcs(w) = wscpy( (wchar_t *)XtMalloc( w->text_out.alloc_size ),
				 ws );
	XtFree( (char *)ws );
    }
    TextValue(w) = NULL;
}


static Boolean
#if NeedFunctionPrototypes
GetXYFromPos( XmwTextOutWidget w, Position position, Position *x, Position *y )
#else /* NeedFunctionPrototypes */
GetXYFromPos( w, position, x, y )
    XmwTextOutWidget	 w;
    Position		 position;
    Position		*x, *y;
#endif /* NeedFunctionPrototypes */
{
    wchar_t	*value = TextValueWcs(w);

    *x = w->text_out.cursor_width + w->text_out.left_margin;
    *y = w->text_out.ascent + w->text_out.top_margin;

    if( position > w->text_out.string_length )
	return( False );

    *x += XwcTextEscapement( TextFontSet(w), value, position );
    if( w->text_out.attachment == XmwCURSOR_LEFT )
	*x -= XwcTextEscapement( TextFontSet(w), value,
				 w->text_out.left_cursor_position );
    else
	*x += w->core.width - w->text_out.cursor_width
	    - XwcTextEscapement( TextFontSet(w), value,
				 w->text_out.right_cursor_position );
    return( True );
}


static void	PaintCursor( w )
    XmwTextOutWidget	w;
{
    Position	x, y;

    GetXYFromPos( w, TextCursorPosition(w), &x, &y );
    x -= w->text_out.cursor_width;
    y -= w->text_out.cursor_height;
    XCopyArea( XtDisplay(w), w->text_out.cursor, XtWindow(w),
	       w->text_out.xor_gc, 0, 0,
	       w->text_out.cursor_width, w->text_out.cursor_height, x, y );
    w->text_out.cursor_on = !w->text_out.cursor_on;
}


static void	PaintCaret( w )
    XmwTextOutWidget	w;
{
    Position	x, y;

    GetXYFromPos( w, TextCursorPosition(w), &x, &y );
    x -= w->text_out.cursor_width -
	((w->text_out.cursor_width - CARET_WIDTH) >> 1);
    y -= CARET_HEIGHT - w->text_out.descent;
    XCopyArea( XtDisplay(w), w->text_out.dest_cursor, XtWindow(w),
	       w->text_out.xor_gc, 0, 0,
	       CARET_WIDTH, CARET_HEIGHT, x, y );
    w->text_out.caret_on = !w->text_out.caret_on;
}


static void
#if NeedFunctionPrototypes
LoadGCs( XmwTextOutWidget w, Pixel background, Pixel foreground )
#else /* NeedFunctionPrototypes */
LoadGCs( w, background, foreground )
    XmwTextOutWidget	w;
    Pixel		background, foreground;
#endif /* NeedFunctionPrototypes */
{
    Display		*display = XtDisplay(w);
    Window		window = XtWindow(w);
    XGCValues		values;
    unsigned long	mask;

    mask = 0;
    values.function = GXcopy;		mask |= GCFunction;
    values.foreground = foreground;	mask |= GCForeground;
    values.background = background;	mask |= GCBackground;
    values.fill_style = FillSolid;	mask |= GCFillStyle;
    values.line_width = (w->text_out.ascent > 19) ? 2 : 1;
					mask |= GCLineWidth;
    values.graphics_exposures = False;	mask |= GCGraphicsExposures;
    if( w->text_out.gc != NULL )
	XChangeGC( display, w->text_out.gc, mask, &values );
    else
	w->text_out.gc = XCreateGC( display, window, mask, &values );

    values.foreground = background;	mask |= GCForeground;
    values.background = foreground;	mask |= GCBackground;
    if( w->text_out.inv_gc != NULL )
	XChangeGC( display, w->text_out.inv_gc, mask, &values );
    else
	w->text_out.inv_gc = XCreateGC( display, window, mask, &values );

    values.function = GXxor;
    values.graphics_exposures = FALSE;
    values.foreground = foreground ^ background;
    values.background = 0;
    if( w->text_out.xor_gc != NULL )
	XtReleaseGC( (Widget)w, w->text_out.xor_gc );
    w->text_out.xor_gc = XtGetGC( (Widget)w, mask, &values );
}


static void
#if NeedFunctionPrototypes
MakeIBeamCursor( XmwTextOutWidget w, int line_width )
#else /* NeedFunctionPrototypes */
MakeIBeamCursor( w, line_width )
    XmwTextOutWidget	w;
    int			line_width;
#endif /* NeedFunctionPrototypes */
{
    Screen	*screen = XtScreen(w);
    Display	*display = XtDisplay(w);
    Window	window = XtWindow(w);
    XSegment	segments[3];
    XtGCMask	mask;
    XGCValues	values;
    GC		gc;

    mask = GCForeground | GCBackground |GCLineWidth;
    values.foreground = 0;
    values.background = 0;
    values.line_width = line_width;
    gc = XCreateGC( display, window, mask, &values );

    w->text_out.cursor = XCreatePixmap( display, window,
					w->text_out.cursor_width,
					w->text_out.cursor_height,
					screen->root_depth );

    XFillRectangle( display, w->text_out.cursor, gc, 0, 0,
		    w->text_out.cursor_width,
		    w->text_out.cursor_height );
    XSetForeground( display, gc,
		   w->text_out.foreground ^ w->core.background_pixel );

    segments[0].x1 = 0;
    segments[0].y1 = line_width - 1;
    segments[0].x2 = w->text_out.cursor_width;
    segments[0].y2 = line_width - 1;

    segments[1].x1 = 0;
    segments[1].y1 = w->text_out.cursor_height - 1;
    segments[1].x2 = w->text_out.cursor_width;
    segments[1].y2 = w->text_out.cursor_height - 1;

    segments[2].x1 = w->text_out.cursor_width / 2;
    segments[2].y1 = line_width;
    segments[2].x2 = w->text_out.cursor_width / 2;
    segments[2].y2 = w->text_out.cursor_height - 1;

    XDrawSegments( display, w->text_out.cursor, gc, segments, 3 );

    XFreeGC( display, gc );
}


static void
#if NeedFunctionPrototypes
MakeDestCursor( XmwTextOutWidget w )
#else /* NeedFunctionPrototypes */
MakeDestCursor( w )
    XmwTextOutWidget	w;
#endif /* NeedFunctionPrototypes */
{
    Display	*dpy = XtDisplay(w);
    Screen	*screen = XtScreen(w);

    w->text_out.dest_cursor = XCreatePixmapFromBitmapData( dpy, XtWindow(w),
			(char*)caretBits, CARET_WIDTH, CARET_HEIGHT,
			w->core.background_pixel ^ w->text_out.foreground, 0,
			screen->root_depth );
}


static void
#if NeedFunctionPrototypes
MakeCursors( XmwTextOutWidget w )
#else /* NeedFunctionPrototypes */
MakeCursors( w )
    XmwTextOutWidget	w;
#endif /* NeedFunctionPrototypes */
{
    int		line_width = 1;

    if( !XtIsRealized((Widget)w) )	return;

    w->text_out.cursor_width = 5;
    w->text_out.cursor_height = w->text_out.ascent;

    MakeDestCursor( w );

    if( w->text_out.cursor_height > 19 ) {
	w->text_out.cursor_width++;
	line_width = 2;
    }

    MakeIBeamCursor( w, line_width );
}



static void
#if NeedFunctionPrototypes
ClassInitialize( void )
#else /* NeedFunctionPrototypes */
ClassInitialize()
#endif /* NeedFunctionPrototypes */
{
    XawInitializeWidgetSet();
    XtAddConverter( XtRString, XmwRValueWcs, XmwCvtStringToValueWcs, NULL, 0 );
}


static void
#if NeedFunctionPrototypes
Destroy( Widget w )
#else /* NeedFunctionPrototypes */
Destroy( w )
    Widget	w;
#endif /* NeedFunctionPrototypes */
{
    XmwTextOutWidget	pw = (XmwTextOutWidget)w;
    Display		*display = XtDisplay(w);

    if( pw->text_out.timer_id )
	XtRemoveTimeOut( pw->text_out.timer_id );
    XFreeGC( display, pw->text_out.gc );
    XFreeGC( display, pw->text_out.inv_gc );
    XtReleaseGC( w, pw->text_out.xor_gc );
    XtFree( (char *)pw->text_out.highlight.list );
    XtFree( pw->text_out.value );
}


static Boolean
#if NeedFunctionPrototypes
SetValues( Widget current, Widget request, Widget new, ArgList args,
	   Cardinal *num_args )
#else /* NeedFunctionPrototypes */
SetValues( current, request, new, args, num_args )
    Widget	current, request, new;
    ArgList	args;
    Cardinal	*num_args;
#endif /* NeedFunctionPrototypes */
{
    XmwTextOutWidget	  cw = (XmwTextOutWidget)current;
    XmwTextOutWidget	  rw = (XmwTextOutWidget)request;
    XmwTextOutWidget	  nw = (XmwTextOutWidget)new;
    Boolean		  changed = False;
    Display		 *d = XtDisplay( request );
    XFontStruct		**fs;
    char		**fn;
    int			  n;
    XFontSetExtents	 *fse;

    if( nw->text_out.left_margin != cw->text_out.left_margin )
	nw->text_out.left_margin = cw->text_out.left_margin;
    if( nw->text_out.right_margin != cw->text_out.right_margin )
	nw->text_out.right_margin = cw->text_out.right_margin;
    if( nw->text_out.top_margin != cw->text_out.top_margin )
	nw->text_out.top_margin = cw->text_out.top_margin;
    if( nw->text_out.bottom_margin != cw->text_out.bottom_margin )
	nw->text_out.bottom_margin = cw->text_out.bottom_margin;

    if( nw->text_out.font_set != cw->text_out.font_set ) {
	if( nw->text_out.font_set != NULL )
	    XFreeFontSet( d, cw->text_out.font_set );
	n = XFontsOfFontSet( TextFontSet(rw), &fs, &fn );
	nw->text_out.ascent = 0;
	nw->text_out.descent = 0;
	while( --n >= 0 ) {
	    if( nw->text_out.ascent < fs[n]->ascent )
		nw->text_out.ascent = fs[n]->ascent;
	    if( nw->text_out.descent < fs[n]->descent )
		nw->text_out.descent = fs[n]->descent;
	}
	fse = XExtentsOfFontSet( TextFontSet(nw) );
	nw->text_out.max_extent = fse->max_logical_extent.width;
	cw->core.height =
	nw->core.height = nw->text_out.ascent + nw->text_out.descent;
	MakeCursors( nw );
	changed = True;
    }

    if( wscmp( nw->text_out.wcs_value, cw->text_out.wcs_value ) ) {
	XtFree( (char *)TextValueWcs(cw) );
	if( nw->text_out.wcs_value == NULL ) {
	    nw->text_out.string_length = 0;
	    nw->text_out.alloc_size = sizeof(wchar_t) * ALLOC_SIZE;
	    TextValueWcs(nw) = (wchar_t *)XtMalloc( nw->text_out.alloc_size );
	    TextValueWcs(nw)[0] = '\0';
	}
	else {
	    nw->text_out.string_length = wslen( TextValueWcs(nw) );
	    nw->text_out.alloc_size = sizeof(wchar_t) *
				  ( nw->text_out.string_length + ALLOC_SIZE );
	    TextValueWcs(nw) =
		wscpy( (wchar_t *)XtMalloc( nw->text_out.alloc_size ),
		       TextValueWcs(rw) );
	}

	nw->text_out.highlight.number = 1;
	nw->text_out.highlight.list[0].position = 0;

	XmwTextOutSetCursorPosition( new, nw->text_out.string_length );

	changed = True;
	nw->text_out.value = NULL;
    }
    else if( nw->text_out.value != NULL ) {
	ValidateString( nw, nw->text_out.value );
	nw->text_out.highlight.number = 1;
	nw->text_out.highlight.list[0].position = 0;

	XmwTextOutSetCursorPosition( new, nw->text_out.string_length );

	changed = True;
	nw->text_out.value = NULL;
    }

    if( nw->text_out.cursor_position != cw->text_out.cursor_position )
	changed = True;

    if( nw->text_out.display_caret != cw->text_out.display_caret ) {
	if( nw->text_out.display_caret ) {
	    if( nw->text_out.enter )
		PaintCursor( nw );
	    else
		PaintCaret( nw );
	}
	else {
	    if( nw->text_out.cursor_on )
		PaintCursor( nw );
	    if( nw->text_out.caret_on )
		PaintCaret( nw );
	}
    }

    nw->text_out.columns = nw->core.width / nw->text_out.max_extent;

    return( changed );
}


static void
#if NeedFunctionPrototypes
GetValuesHook( Widget w, ArgList args, Cardinal *num_args )
#else
GetValuesHook(w, args, num_args)
    Widget	 w;
    ArgList	 args;
    Cardinal	*num_args;
#endif
{
    int			 n, len;
    char		*mbs;

    for( n = 0; n < *num_args ; n++ ) {
	if( strcmp( args[n].name, XtNvalue ) == 0 ) {
	    len = wslen(TextValueWcs(w)) * MB_CUR_MAX;
	    mbs = XtMalloc(len + 1);
	    if( mbs == NULL )
		*((char **)args[n].value) = NULL;
	    else {
		len = wcstombs( mbs, TextValueWcs(w), len );
		if( len != -1 )
		    *((char **)args[n].value) = mbs;
		else
		    *((char **)args[n].value) = NULL;
	    }
	}
    }
}


static int	FindHighlight( w, position )
XmwTextOutWidget	w;
Position		position;
{
    HighlightRec	*l = w->text_out.highlight.list;
    int			ii;

    for( ii = w->text_out.highlight.number - 1; ii >= 0; ii-- )
        if( position >= l[ii].position )
            return( ii );
    return( 0 );
}


static void	InsertHighlight( w, position, mode )
XmwTextOutWidget	w;
Position		position;
XmwHighlightMode	mode;
{
    HighlightRec	*l = w->text_out.highlight.list;
    int			ii, jj;

    ii = FindHighlight( w, position );

    if( l[ii].position == position ) {
	l[ii].mode = mode;
	if( ii > 0  && l[ii-1].mode == mode ) {
	    for( jj = ii, w->text_out.highlight.number--;
		jj < w->text_out.highlight.number; jj++ )
		l[jj] = l[jj+1];
	    ii--;
	}
	if( ii + 1 != w->text_out.highlight.number  &&  l[ii+1].mode == mode ) {
	    w->text_out.highlight.number--;
	    for( ii++; ii < w->text_out.highlight.number; ii++ )
		l[ii] = l[ii+1];
	}
    }
    else  if( l[ii].mode != mode ) {
	if( ii + 1 != w->text_out.highlight.number  &&  l[ii+1].mode == mode ) {
	    l[ii+1].position = position;
	}
	else {
	    if( w->text_out.highlight.number == w->text_out.highlight.maximum ) {
		w->text_out.highlight.maximum++;
		l = w->text_out.highlight.list = (HighlightRec *)XtRealloc( (char *)l, sizeof(HighlightRec) * w->text_out.highlight.maximum );
	    }
	    for( jj = w->text_out.highlight.number; jj > ii; jj-- )
		l[jj] = l[jj-1];
	    w->text_out.highlight.number++;
	    l[ii+1].position = position;
	    l[ii+1].mode = mode;
	}
    }

    for( ii = 0; ii < w->text_out.highlight.number; ii++ ) {
	if( w->text_out.highlight.list[ii].position > w->text_out.string_length ) {
	    w->text_out.highlight.number = ii;
	    break;
	}
    }
}


static void	TextOutAreaSetHighlight( w, left, right, mode )
XmwTextOutWidget	w;
Position		left, right;
XmwHighlightMode	mode;
{
    XmwHighlightMode	rmode;
    int			l, r;

    if( left >= right )
	return;

    rmode = w->text_out.highlight.list[FindHighlight(w, right)].mode;
    InsertHighlight( w, left, mode );
    l = FindHighlight( w, left );
    r = FindHighlight( w, right - 1 );
    while( l < r ) {
	InsertHighlight( w, w->text_out.highlight.list[r].position, mode );
	r = FindHighlight( w, right );
	if( w->text_out.highlight.list[r].mode == mode )
	    r--;
    }
    InsertHighlight( w, right, rmode );
}


static void	RedisplayText( w, start, end )
    XmwTextOutWidget	w;
    Position		start, end;
{
    Display		*display = XtDisplay(w);
    wchar_t		*value = w->text_out.wcs_value;
    HighlightRec	*list = w->text_out.highlight.list;
    int			x, y, width;
    int			ii;
    Position		_end;
    GC			gc;

    if( !XtIsRealized((Widget)w) )	return;

    if( w->text_out.display_caret ) {
	if( w->text_out.cursor_on )
	    PaintCursor( w );
	if( w->text_out.caret_on )
	    PaintCaret( w );
    }

    XSelectInput( display, w->core.window,
		  w->text_out.event_mask & ~ExposureMask );

    width = XwcTextEscapement( TextFontSet(w), value,
			       w->text_out.string_length )
	  + w->text_out.cursor_width / 2;
    if( width < w->core.width )
	XFillRectangle( display, XtWindow(w), w->text_out.inv_gc, width, 0,
			w->core.width, w->core.height );

    ii = 0;
    while( start != end  &&  ii < w->text_out.highlight.number ) {
	if( list[ii].position <= start  &&
	    (w->text_out.highlight.number == ii + 1  ||
	     start < list[ii+1].position) ) {
	    x = XwcTextEscapement( TextFontSet(w), value, start )
	      + w->text_out.left_margin;
	    y = w->text_out.ascent + w->text_out.top_margin;
	    if( w->text_out.attachment == XmwCURSOR_LEFT )
		x += (w->text_out.cursor_width >> 1)
		   - XwcTextEscapement( TextFontSet(w), value,
					w->text_out.left_cursor_position );
	    else
		x += w->core.width - (w->text_out.cursor_width >> 1)
		   - XwcTextEscapement( TextFontSet(w), value,
					w->text_out.right_cursor_position );

	    _end = end;
	    if( ii+1 < w->text_out.highlight.number  &&
		_end > list[ii+1].position )
		_end = list[ii+1].position;

	    switch( list[ii].mode ) {
	      case XmwHIGHLIGHT_NORMAL:
	      case XmwHIGHLIGHT_SECONDARY_SELECTED:
		gc = w->text_out.gc;
		break;

	      case XmwHIGHLIGHT_SELECTED:
		gc = w->text_out.inv_gc;
		break;

	      default:
		gc = w->text_out.gc;
		break;
	    }

	    XwcDrawImageString( display, XtWindow(w), TextFontSet(w),
				gc, x, y, value + start, _end - start );

	    if( list[ii].mode == XmwHIGHLIGHT_SECONDARY_SELECTED )
		XDrawLine( display, XtWindow(w), gc,
			  x, y + w->text_out.descent - 1,
			  x + XwcTextEscapement( TextFontSet(w), &value[start],
						 _end - start ),
			  y + w->text_out.descent - 1 );

	    start = _end;
	}
	else
	    ii++;
    }

    if( w->text_out.display_caret ) {
	if( w->text_out.enter )
	    PaintCursor( w );
	else
	    PaintCaret( w );
    }
    XSelectInput( display, XtWindow(w), w->text_out.event_mask );
}


static void	SetCursorPosition( w, position )
    XmwTextOutWidget	w;
    Position		position;
{
    int		n;
    Boolean	redraw = False;
    wchar_t	*value = w->text_out.wcs_value;
    Dimension	width;

    if( position < 0 )
	position = 0;

    if( position > w->text_out.string_length )
	position = w->text_out.string_length;

    if( XwcTextEscapement( TextFontSet(w), value, w->text_out.string_length ) <
	w->core.width ) {
	w->text_out.left_cursor_position = 0;
	w->text_out.right_cursor_position = w->text_out.string_length;
	w->text_out.attachment = XmwCURSOR_LEFT;

	XFillRectangle( XtDisplay(w), XtWindow(w), w->text_out.inv_gc,
			0, 0, w->text_out.cursor_width / 2, w->core.height );
    }
    else if( position < w->text_out.left_cursor_position ) {
	width = w->core.width;
	for( n = position + 1;
	     width > XwcTextEscapement( TextFontSet(w), &value[position],
					n - position ); n++ );
	w->text_out.right_cursor_position = n - 1;
	w->text_out.left_cursor_position = position;
	w->text_out.attachment = XmwCURSOR_LEFT;
	redraw = True;
    }
    else if( position > w->text_out.right_cursor_position ) {
	width = (w->text_out.cursor_width >> 1)
		+ XwcTextEscapement( TextFontSet(w), value, position )
		- w->core.width;
	for( n = 1; XwcTextEscapement( TextFontSet(w), value, n ) < width;
	     n++ );
	w->text_out.left_cursor_position = n;
	w->text_out.right_cursor_position = position;
	w->text_out.attachment = XmwCURSOR_RIGHT;
	redraw = True;
    }
    else if( w->text_out.attachment == XmwCURSOR_RIGHT  &&
	     w->text_out.string_length < w->text_out.right_cursor_position ) {
	width = (w->text_out.cursor_width >> 1)
	      + XwcTextEscapement( TextFontSet(w), value,
				   w->text_out.string_length )
	      - w->core.width;
	for( n = 1; XwcTextEscapement( TextFontSet(w), value, n ) < width;
	     n++ );
	w->text_out.left_cursor_position = n;
	w->text_out.right_cursor_position = w->text_out.string_length;
	redraw = True;
    }

    if( w->text_out.cursor_on )
	PaintCursor( w );
    if( w->text_out.caret_on )
	PaintCaret( w );

    TextCursorPosition(w) = position;

    if( redraw )
	XClearArea( XtDisplay(w), XtWindow(w), 0, 0, 0, 0, True );

    if( w->text_out.display_caret )
	if( w->text_out.enter )
	    PaintCursor( w );
	else
	    PaintCaret( w );
}


static Boolean
#if NeedFunctionPrototypes
ReplaceText( XmwTextOutWidget w, XEvent *event, Position replace_prev,
	     Position replace_next, wchar_t *insert, int insert_length )
#else
ReplaceText( w, event, replace_prev, replace_next, insert, insert_length )
    XmwTextOutWidget	w;
    XEvent		*event;
    Position		replace_prev, replace_next;
    wchar_t		*insert;
    int			insert_length;
#endif
{
    int			replace_length, ii, jj;
    wchar_t		*src, *dst;
    int			delta = 0;
    Position		cursorPos;
    wchar_t		*orig_insert;
    XmwHighlightMode	highlight_mode;
    HighlightRec	*l;
    int			number;

    highlight_mode = w->text_out.highlight.list[FindHighlight( w, replace_prev )].mode;
    orig_insert = insert;
    replace_length = replace_next - replace_prev;

    if( (w->text_out.string_length + insert_length - replace_length) - 
       w->text_out.max_length > 0) { 
	if( w->text_out.verify_bell )
	    XBell( XtDisplay( (Widget)w), 0 );
	if( insert != orig_insert )
	    XtFree( (char *)insert );
	return( False );
    }

    if( w->text_out.string_length + insert_length - replace_length >=
        (w->text_out.alloc_size / sizeof(wchar_t)) ) {
	w->text_out.alloc_size += max( insert_length + 1, ALLOC_SIZE );
	TextValueWcs(w) = (wchar_t *)XtRealloc( (char *)TextValueWcs(w),
						w->text_out.alloc_size );
    }

    delta = insert_length - replace_length;

    if( replace_length > insert_length ) {
	for( src = TextValueWcs(w) + replace_next, dst = src + delta,
	    ii = (w->text_out.string_length + 1) - replace_next;
	    ii > 0; ++src, ++dst, --ii )
	    *dst = *src;
	l = w->text_out.highlight.list;
	for( ii = w->text_out.highlight.number - 1; ii >= 0; ii-- )
	    if( replace_next > l[ii].position )
		break;
	jj = ii;
	jj++;
	while( jj < w->text_out.highlight.number ) {
	    l[jj].position += delta;
	    jj++;
	}
	if( (ii + 1) < w->text_out.highlight.number ) {
	    number = 0;
	    while( l[ii].position >= l[ii+number+1].position  &&
		   (ii+number+1) < w->text_out.highlight.number )
		number++;
	    if( number > 0 ) {
		jj = ii;
		while( jj < w->text_out.highlight.number ) {
		    l[jj] = l[jj+number];
		    jj++;
		}
		w->text_out.highlight.number -= number;
		if( ii > 0  &&  l[ii].mode == l[ii-1].mode ) {
		    while( ii < w->text_out.highlight.number ) {
			l[ii] = l[ii+1];
			ii++;
		    }
		    w->text_out.highlight.number--;
		}
	    }
	}
    }
    else if( replace_length < insert_length ) {
	for( src = TextValueWcs(w) + w->text_out.string_length,
	     dst = src + delta,
	     ii = (w->text_out.string_length + 1) - replace_next;
	     ii > 0; src--, dst--, ii-- )
	    *dst = *src;
	l = w->text_out.highlight.list;
	for( ii = w->text_out.highlight.number - 1; ii > 0; ii-- )
	    if( replace_next > l[ii].position )
		break;
	ii++;
	while( ii < w->text_out.highlight.number ) {
	    l[ii].position += delta;
	    ii++;
	}
	TextOutAreaSetHighlight( w, replace_prev, replace_prev + insert_length,
				 highlight_mode );
    }

    if( insert_length != 0 ) {
	for( src = insert, dst = TextValueWcs(w) + replace_prev,
	    ii = insert_length;
	    ii > 0; ++src, ++dst, --ii )
	    *dst = *src;
    }

    w->text_out.string_length += delta;
    cursorPos = replace_next + delta;
    SetCursorPosition( w, cursorPos );

    if( replace_length == insert_length )
	replace_next = replace_prev + replace_length;
    else
	replace_next = w->text_out.string_length;
    RedisplayText( w, 0, replace_next );

    if( insert != orig_insert )
	XtFree( (char *)insert );
    return( True );
}


static void
#if NeedFunctionPrototypes
Initialize( Widget request, Widget new, ArgList args, Cardinal *num_args )
#else /* NeedFunctionPrototypes */
Initialize( request, new, args, num_args )
    Widget	 request, new;
    ArgList	 args;
    Cardinal	*num_args;
#endif /* NeedFunctionPrototypes */
{
    XmwTextOutWidget	  rw = (XmwTextOutWidget)request;
    XmwTextOutWidget	  nw = (XmwTextOutWidget)new;
    XFontStruct		**fs;
    char		**fn;
    int			  n;
    wchar_t		 *wcs;
    XFontSetExtents	 *fse;

    if( TextCursorPosition(rw) < 0 )
	TextCursorPosition(nw) = 0;

    n = XFontsOfFontSet( TextFontSet(nw), &fs, &fn );
    nw->text_out.ascent = 0;
    nw->text_out.descent = 0;
    while( --n >= 0 ) {
	if( nw->text_out.ascent < fs[n]->ascent )
	    nw->text_out.ascent = fs[n]->ascent;
	if( nw->text_out.descent < fs[n]->descent )
	    nw->text_out.descent = fs[n]->descent;
    }
    if( nw->core.height < nw->text_out.ascent + nw->text_out.descent )
	nw->core.height = nw->text_out.ascent + nw->text_out.descent
			+ nw->text_out.top_margin + nw->text_out.bottom_margin;

    fse = XExtentsOfFontSet( TextFontSet(nw) );
    nw->text_out.max_extent = fse->max_logical_extent.width;
    if( nw->core.width <= 0 ) {
	if( nw->text_out.columns > 0 )
	    nw->core.width = nw->text_out.max_extent * nw->text_out.columns
			   + nw->text_out.left_margin
			   + nw->text_out.right_margin;
	else {
	    nw->core.width = nw->core.parent->core.width;
	    nw->text_out.columns = nw->core.width / nw->text_out.max_extent;
	    if( nw->text_out.columns == 0 ) {
		nw->core.width = nw->text_out.max_extent * 20
			       + nw->text_out.left_margin
			       + nw->text_out.right_margin;
		nw->text_out.columns = 20;
	    }
	}
    }
    nw->text_out.old_width = nw->core.width;
    nw->text_out.old_height = nw->core.height;

    nw->text_out.cursor_on = nw->text_out.caret_on = False;

    nw->text_out.gc = NULL;
    nw->text_out.inv_gc = NULL;
    nw->text_out.xor_gc = NULL;

    if( TextValueWcs(nw) == NULL ) {
	if( TextValue(nw) == NULL ) {
	    nw->text_out.string_length = 0;
	    nw->text_out.alloc_size = sizeof(wchar_t) * ALLOC_SIZE;
	    TextValueWcs(nw) = (wchar_t *)XtMalloc( nw->text_out.alloc_size );
	    TextValueWcs(nw)[0] = '\0';
	}
	else
	    ValidateString( nw, TextValue(nw) );
    }
    else {
	wcs = TextValueWcs(nw);
	nw->text_out.string_length = wslen( wcs );
	nw->text_out.alloc_size = sizeof(wchar_t) *
				  ( nw->text_out.string_length + ALLOC_SIZE );
	TextValueWcs(nw) = wscpy( (wchar_t *)XtMalloc( nw->text_out.alloc_size ),
				 wcs );
	TextValue(nw) = NULL;
    }

    if( TextMaxLength(nw) < MAXINT ) {
	if( nw->text_out.string_length > TextMaxLength(nw) ) {
	    nw->text_out.string_length = TextMaxLength(nw);
	    TextValueWcs(nw)[nw->text_out.string_length] = '\0';
	}
    }

    nw->text_out.cursor_height = nw->text_out.cursor_width = 0;
    nw->text_out.cursor = None;

    nw->text_out.highlight.number = nw->text_out.highlight.maximum = 1;
    nw->text_out.highlight.list
	= (HighlightRec *)XtMalloc( (unsigned)sizeof(HighlightRec) );
    nw->text_out.highlight.list[0].position = 0;
    nw->text_out.highlight.list[0].mode = XmwHIGHLIGHT_SECONDARY_SELECTED;

    nw->text_out.timer_id = '\0';

    nw->text_out.cursor_width = 5;

    TextCursorPosition(nw) = 0;
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
    XmwTextOutWidget		pw = (XmwTextOutWidget)widget;

    pw->text_out.event_mask = attr->event_mask;

    attr->border_pixel = pw->text_out.foreground;
    *vmask |= CWBitGravity;
    attr->bit_gravity = NorthWestGravity;
    XtCreateWindow( widget, (unsigned)InputOutput, (Visual *)CopyFromParent,
		    *vmask, attr );

    LoadGCs( pw, pw->core.background_pixel, pw->text_out.foreground );
    MakeCursors( pw );
}


static void
#if NeedFunctionPrototypes
Resize( Widget w )
#else /* NeedFunctionPrototypes */
Resize( w )
    Widget	w;
#endif /* NeedFunctionPrototypes */
{
    XmwTextOutWidget	tw = (XmwTextOutWidget)w;

    if( tw->text_out.old_width < tw->core.width  &&
	tw->text_out.right_margin > 0 )
	XClearArea( XtDisplay(w), XtWindow(w),
		    tw->core.width - tw->text_out.right_margin, 0, 0, 0,
		    False );
    if( tw->text_out.old_height < tw->core.height  &&
	tw->text_out.bottom_margin > 0 )
	XClearArea( XtDisplay(w), XtWindow(w),
		    0, tw->core.height - tw->text_out.bottom_margin, 0, 0,
		    False );

    tw->text_out.old_width = tw->core.width;
    tw->text_out.old_height = tw->core.height;

    tw->text_out.columns = tw->core.width / tw->text_out.max_extent;

    RedisplayText( w, TextCursorPosition(tw), tw->text_out.string_length );
}


static void
#if NeedFunctionPrototypes
Redisplay( Widget w, XEvent *event, Region region )
#else /* NeedFunctionPrototypes */
Redisplay( w, event, region )
    Widget	w;
    XEvent	*event;
    Region	region;
#endif /* NeedFunctionPrototypes */
{
    XmwTextOutWidget	pw = (XmwTextOutWidget)w;

    if( !pw->core.sensitive )
	return;
    RedisplayText( pw, 0, pw->text_out.string_length );
}



static void
#if NeedFunctionPrototypes
Enter( Widget w, XEvent *event, String *params, Cardinal *num_params )
#else
Enter( w, event, params, num_params )
    Widget	 w;
    XEvent	*event;
    String	*params;
    Cardinal	*num_params;
#endif
{
    XmwTextOutWidget	tw = (XmwTextOutWidget)w;

    tw->text_out.enter = True;
}


static void
#if NeedFunctionPrototypes
Leave( Widget w, XEvent *event, String *params, Cardinal *num_params )
#else
Leave( w, event, params, num_params )
    Widget	 w;
    XEvent	*event;
    String	*params;
    Cardinal	*num_params;
#endif
{
    XmwTextOutWidget	tw = (XmwTextOutWidget)w;

    tw->text_out.enter = False;
}


static XtActionsRec	actionsList[] = {
    { "Enter",		Enter	},
    { "Leave",		Leave	}};


#define Offset(field)	XtOffsetOf(XmwTextOutRec, field)
static XtResource	local_resources[] =
{
    {
	XtNvalue, XtCValue,
	XtRString, sizeof(String), Offset(text_out.value),
	XtRString, NULL },
    {
	XmwNvalueWcs, XmwCValueWcs,
	XmwRValueWcs, sizeof(wchar_t *), Offset(text_out.wcs_value),
	XtRString, NULL },
    {
	XmwNcursorPosition, XmwCCursorPosition,
	XtRPosition, sizeof(Position), Offset(text_out.cursor_position),
	XtRImmediate, (XtPointer)0 },
    {
	XmwNdisplayCaret, XmwCOutput,
	XtRBoolean, sizeof(Boolean), Offset(text_out.display_caret),
	XtRImmediate, (XtPointer)True },
    {
	XtNfontSet, XtCFontSet,
	XtRFontSet, sizeof(XFontSet), Offset(text_out.font_set),
	XtRString, XtDefaultFontSet },
    {
	XtNforeground, XtCForeground,
	XtRPixel, sizeof(Pixel), Offset(text_out.foreground),
	XtRString, XtDefaultForeground },
    {
	XmwNcolumns, XmwCColumns,
	XtRShort, sizeof(short), Offset(text_out.columns),
	XtRImmediate, (XtPointer)0 },
    {
	XmwNmaxLength, XmwCMaxLength,
	XtRInt, sizeof(int), Offset(text_out.max_length),
	XtRImmediate, (XtPointer)MAXINT },
    {
	XmwNverifyBell, XmwCVerifyBell,
	XtRBoolean, sizeof(Boolean), Offset(text_out.verify_bell),
	XtRImmediate, (XtPointer)True },
    {
	XmwNbottomMargin, XtCMargin,
	XtRPosition, sizeof(Position), Offset(text_out.bottom_margin),
	XtRImmediate, (XtPointer)2 },
    {
	XmwNtopMargin, XtCMargin,
	XtRPosition, sizeof(Position), Offset(text_out.top_margin),
	XtRImmediate, (XtPointer)2 },
    {
	XmwNleftMargin, XtCMargin,
	XtRPosition, sizeof(Position), Offset(text_out.left_margin),
	XtRImmediate, (XtPointer)2 },
    {
	XmwNrightMargin, XtCMargin,
	XtRPosition, sizeof(Position), Offset(text_out.right_margin),
	XtRImmediate, (XtPointer)2 }
};
#undef Offset


static char defaultTranslations[] = "\
	<EnterWindow>:	Enter()\n\
	<LeaveWindow>:	Leave()";


XmwTextOutClassRec	xmwTextOutClassRec = 
{
  { /* core_class record     */
    /* superclass	     */ (WidgetClass) &widgetClassRec,
    /* class_name	     */ "XmwTextOut",
    /* widget_size	     */ sizeof(XmwTextOutRec),
    /* class_initialize	     */ ClassInitialize,
    /* class_part_initialize */ NULL,
    /* class_inited          */ False,
    /* initialize	     */ Initialize,
    /* initialize_hook       */ NULL,
    /* realize		     */ Realize,
    /* actions               */ actionsList,
    /* num_actions    	     */ XtNumber(actionsList),
    /* resources	     */ local_resources,
    /* num_resources         */ XtNumber(local_resources),
    /* xrm_class	     */ NULLQUARK,
    /* compress_motion       */ True,
    /* compress_exposure     */ XtExposeCompressMaximal,
    /* compress_enterleave   */ True,
    /* visible_interest      */ False,
    /* destroy               */ Destroy,
    /* resize                */ Resize,
    /* expose                */ Redisplay,
    /* set_values	     */ SetValues,
    /* set_values_hook       */ NULL,
    /* set_values_almost     */ XtInheritSetValuesAlmost,
    /* get_values_hook       */ GetValuesHook,
    /* accept_focus	     */ NULL,
    /* version               */ XtVersion,
    /* callback private      */ NULL,
    /* tm_table              */ defaultTranslations,
    /* query_geometry        */ XtInheritQueryGeometry,
    /* display_accelerator   */ NULL,
    /* extension             */ NULL
  },
  { /* simple_class record   */
    /* cahnge_sensitive      */	XtInheritChangeSensitive
  },
  { /* text_out_class record */
    /* extension	     */ NULL
  },

};

WidgetClass	xmwTextOutClass = (WidgetClass)&xmwTextOutClassRec;


Widget
#if NeedFunctionPrototypes
XmwCreateTextOut( Widget parent, char *name, ArgList arglist,
		  Cardinal argcount )
#else /* NeedFunctionPrototypes */
XmwCreateTextOut( parent, name, arglist, argcount )
    Widget	parent;
    char	*name;
    ArgList	arglist;
    Cardinal	argcount;
#endif /* NeedFunctionPrototypes */
{
    return( XtCreateManagedWidget( name, xmwTextOutClass, parent, arglist,
	    argcount ) );
}


void
#if NeedFunctionPrototypes
XmwTextOutSetHighlight( Widget w, Position left, Position right,
			XmwHighlightMode mode )
#else /* NeedFunctionPrototypes */
XmwTextOutSetHighlight( w, left, right, mode )
    Widget		w;
    Position		left, right;
    XmwHighlightMode	mode;
#endif /* NeedFunctionPrototypes */
{
    XmwTextOutWidget	pw = (XmwTextOutWidget)w;

    if( left >= right  ||  right <= 0 )	return;

    if( left < 0 )	left = 0;

    if( right > pw->text_out.string_length )
	right = pw->text_out.string_length;

    TextOutAreaSetHighlight( pw, left, right, mode );

    RedisplayText( pw, left, right );
}


Boolean	
#if NeedFunctionPrototypes
XmwTextOutReplace( Widget w, Position from_pos, Position to_pos, char *value )
#else /* NeedFunctionPrototypes */
XmwTextOutReplace( w, from_pos, to_pos, value )
    Widget	w;
    Position	from_pos, to_pos;
    char	*value;
#endif /* NeedFunctionPrototypes */
{
    XmwTextOutWidget	pw = (XmwTextOutWidget)w;
    Boolean		result;
    int			len;
    wchar_t		*wcs;

    if( from_pos < 0 )
	from_pos = 0;

    if( to_pos >= pw->text_out.string_length )
	to_pos = pw->text_out.string_length;

    if( value == NULL )
	value = "";
    len = strlen( value );
    wcs = (wchar_t *)XtMalloc( sizeof(wchar_t) * ( len + 1 ) );
    len = mbstowcs( wcs, value, len );

    result = ReplaceText( pw, NULL, from_pos, to_pos, wcs, len );
    XtFree( (char *)wcs );
    return( result );
}


Boolean
#if NeedFunctionPrototypes
XmwTextOutReplaceWcs( Widget w, Position from_pos, Position to_pos,
		      wchar_t *value )
#else /* NeedFunctionPrototypes */
XmwTextOutReplaceWcs( w, from_pos, to_pos, value )
    Widget	w;
    Position	from_pos, to_pos;
    wchar_t	*value;
#endif /* NeedFunctionPrototypes */
{
    XmwTextOutWidget	pw = (XmwTextOutWidget)w;
    Boolean		result;
    wchar_t		*wcs;

    if( from_pos < 0 )
	from_pos = 0;

    if( to_pos >= pw->text_out.string_length )
	to_pos = pw->text_out.string_length;

    if( value == NULL ) {
	wcs = (wchar_t *)XtMalloc( sizeof(wchar_t) );
	wcs[0] = '\0';
    }
    else
	wcs = value;

    result = ReplaceText( pw, NULL, from_pos, to_pos, wcs, wslen(wcs) );
    if( value == NULL )
	XtFree( (char *)wcs );
    return( result );
}


void
#if NeedFunctionPrototypes
XmwTextOutInsert( Widget w, Position position, char *value )
#else /* NeedFunctionPrototypes */
XmwTextOutInsert( w, position, value )
    Widget	w;
    Position	position;
    char	*value;
#endif /* NeedFunctionPrototypes */
{
    XmwTextOutReplace( w, position, position, value );
}


void
#if NeedFunctionPrototypes
XmwTextOutInsertWcs( Widget w, Position position, wchar_t *value )
#else /* NeedFunctionPrototypes */
XmwTextOutInsertWcs( w, position, value )
    Widget	w;
    Position	position;
    wchar_t	*value;
#endif /* NeedFunctionPrototypes */
{
    XmwTextOutReplaceWcs( w, position, position, value );
}


void
#if NeedFunctionPrototypes
XmwTextOutSetString( Widget w, char *value, XmwHighlightMode mode )
#else /* NeedFunctionPrototypes */
XmwTextOutSetString( w, value, mode )
    Widget		w;
    char		*value;
    XmwHighlightMode	mode;
#endif /* NeedFunctionPrototypes */
{
    XmwTextOutWidget	pw = (XmwTextOutWidget)w;

    if( value == NULL )
	value = "";
    ValidateString( pw, value );
    pw->text_out.highlight.number = 1;
    pw->text_out.highlight.list[0].position = 0;
    pw->text_out.highlight.list[0].mode = mode;

    XmwTextOutSetCursorPosition( (Widget)pw, pw->text_out.string_length );
    RedisplayText( pw, 0, pw->text_out.string_length );
}


void
#if NeedFunctionPrototypes
XmwTextOutSetStringWcs( Widget w, wchar_t *value, XmwHighlightMode mode )
#else /* NeedFunctionPrototypes */
XmwTextOutSetStringWcs( w, value, mode )
    Widget		w;
    wchar_t		*value;
    XmwHighlightMode	mode;
#endif /* NeedFunctionPrototypes */
{
    XmwTextOutWidget	pw = (XmwTextOutWidget)w;

    XtFree( (char *)TextValueWcs(pw) );
    if( value == NULL ) {
	pw->text_out.string_length = 0;
	pw->text_out.alloc_size = sizeof(wchar_t) * ALLOC_SIZE;
	TextValueWcs(pw) = (wchar_t *)XtMalloc( pw->text_out.alloc_size );
	TextValueWcs(pw)[0] = '\0';
    }
    else {
	pw->text_out.string_length = wslen( value );
	pw->text_out.alloc_size = sizeof(wchar_t) *
				  ( pw->text_out.string_length + ALLOC_SIZE );
	TextValueWcs(pw) =
	    wscpy( (wchar_t *)XtMalloc( pw->text_out.alloc_size ), value );
    }

    pw->text_out.highlight.number = 1;
    pw->text_out.highlight.list[0].position = 0;
    pw->text_out.highlight.list[0].mode = mode;

    XmwTextOutSetCursorPosition( w, pw->text_out.string_length );
    RedisplayText( pw, 0, pw->text_out.string_length );
}


void
#if NeedFunctionPrototypes
XmwTextOutSetCursorPosition( Widget w, Position position )
#else /* NeedFunctionPrototypes */
XmwTextOutSetCursorPosition( w, position )
    Widget	w;
    Position	position;
#endif /* NeedFunctionPrototypes */
{
    SetCursorPosition( (XmwTextOutWidget)w, position );
}


char	*
#if NeedFunctionPrototypes
XmwTextOutGetString( Widget w )
#else
XmwTextOutGetString( w )
    Widget	w;
#endif
{
    int			len = wslen(TextValueWcs(w)) * MB_CUR_MAX;
    char		*mbs = XtMalloc(len + 1);

    if( mbs == NULL )
	return( NULL );
    len = wcstombs( mbs, TextValueWcs(w), len );
    return( (len != -1) ? mbs : NULL );
}


wchar_t	*
#if NeedFunctionPrototypes
XmwTextOutGetStringWcs( Widget w )
#else
XmwTextOutGetStringWcs( w )
    Widget	w;
#endif
{
    int			len = wslen(TextValueWcs(w));
    wchar_t		*wcs = (wchar_t *)XtMalloc((len + 1) * MB_CUR_MAX);

    if( wcs == NULL )
	return( NULL );
    wsncpy( wcs, TextValueWcs(w), len );
    wcs[len] = '\0';
    return( wcs );
}


Position
#if NeedFunctionPrototypes
XmwTextOutGetLastPosition( Widget w )
#else
XmwTextOutGetLastPosition( w )
    Widget	w;
#endif
{
    return( wslen(TextValueWcs(w)) );
}
