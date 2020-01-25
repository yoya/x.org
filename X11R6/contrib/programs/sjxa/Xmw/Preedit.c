/* $SonyId: Preedit.c,v 1.2 1994/06/03 06:37:18 makoto Exp $ */
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
#include	<stdlib.h>
#include	<ctype.h>
#ifdef	SVR4
#include	<widec.h>
#endif
#include	<X11/Xlib.h>
#include	<X11/Xlibint.h>
#include	<X11/keysym.h>
#include	<X11/IntrinsicP.h>
#include	<X11/StringDefs.h>
#include	<Xmw/PreeditP.h>
#include	"SjString.h"

#define MAXINT		2147483647
#define ALLOC_SIZE	32


static XtResource	local_resources[] =
{
    {
	XmwNtopLine, XmwCTopLine, XtRInt,
	sizeof(int),
	XtOffset(XmwPreeditWidget, preedit.top_line),
	XtRInt, 0 },
    {
	XmwNfocusWindow, XmwCFocusWindow, XtRWindow,
	sizeof(Window),
	XtOffset(XmwPreeditWidget, preedit.focus_window),
	XtRWindow, NULL },
    {
	XmwNlineSpacing, XmwCLineSpacing, XtRDimension,
	sizeof(Dimension),
	XtOffset(XmwPreeditWidget, preedit.line_spacing),
	XtRInt, 0 },
    {
	XmwNareaHeight, XmwCAreaHeight, XtRDimension,
	sizeof(Dimension),
	XtOffset(XmwPreeditWidget, preedit.height),
	XtRInt, 0 },
    {
	XmwNareaWidth, XmwCAreaWidth, XtRDimension,
	sizeof(Dimension),
	XtOffset(XmwPreeditWidget, preedit.width),
	XtRInt, 0 },
    {
	XmwNareaX, XmwCAreaX, XtRPosition,
	sizeof(Position),
	XtOffset(XmwPreeditWidget, preedit.x),
	XtRInt, 0 },
    {
	XmwNareaY, XmwCAreaY, XtRPosition,
	sizeof(Position),
	XtOffset(XmwPreeditWidget, preedit.y),
	XtRInt, 0 },
    {
	XmwNareaNeededWidth, XmwCAreaNeededWidth, XtRDimension,
	sizeof(Dimension),
	XtOffset(XmwPreeditWidget, preedit.area_needed_width),
	XtRInt, 0 },
    {
	XmwNareaNeededHeight, XmwCAreaNeededHeight, XtRDimension,
	sizeof(Dimension),
	XtOffset(XmwPreeditWidget, preedit.area_needed_height),
	XtRInt, 0 },
    {
	XmwNspotLocationX, XmwCSpotLocationX, XtRPosition,
	sizeof(Position),
	XtOffset(XmwPreeditWidget, core.x),
	XtRImmediate, 0 },
    {
	XmwNspotLocationY, XmwCSpotLocationY, XtRPosition,
	sizeof(Position),
	XtOffset(XmwPreeditWidget, core.y),
	XtRImmediate, 0 },
    {
	XtNvalue, XtCValue, XtRString,
	sizeof(String),
	XtOffset(XmwPreeditWidget, preedit.value),
	XtRString, NULL },
    {
	XmwNvalueWcs, XmwCValueWcs, XmwRValueWcs,
	sizeof(wchar_t *),
	XtOffset(XmwPreeditWidget, preedit.wcs_value),
	XtRString, NULL },
    {
	XmwNcursorPosition, XmwCCursorPosition, XtRPosition,
	sizeof(Position),
	XtOffset(XmwPreeditWidget, preedit.cursor_position),
	XtRImmediate, (caddr_t)0 },
    {
	XmwNdisplayCaret, XmwCOutput, XtRBoolean,
	sizeof(Boolean),
	XtOffset(XmwPreeditWidget, preedit.display_caret),
	XtRImmediate, (caddr_t)True },
    {
	XmwNcolumns, XmwCColumns, XtRShort,
	sizeof(short),
	XtOffset(XmwPreeditWidget, preedit.columns),
	XtRImmediate, (caddr_t)1 },
    {
	XmwNblinkRate, XmwCBlinkRate, XtRInt,
	sizeof(int),
	XtOffset(XmwPreeditWidget, preedit.blink_rate),
	XtRImmediate, (caddr_t)0 },
    {
	XtNfontSet, XtCFontSet, XtRFontSet,
	sizeof(XFontSet),
	XtOffset(XmwPreeditWidget, preedit.font_set),
	XtRString, XtDefaultFontSet },
    {
	XtNforeground, XtCForeground, XtRPixel,
	sizeof(Pixel),
	XtOffset(XmwPreeditWidget, preedit.foreground),
	XtRString, XtDefaultForeground },
    {
	XmwNfocus, XmwCFocus, XtRBoolean,
	sizeof(Boolean),
	XtOffset(XmwPreeditWidget, preedit.has_focus),
	XtRImmediate, (caddr_t)False },
    {
	XmwNmaxLength, XmwCMaxLength, XtRInt,
	sizeof(int),
	XtOffset(XmwPreeditWidget, preedit.max_length),
	XtRImmediate, (caddr_t)MAXINT },
    {
	XmwNverifyBell, XmwCVerifyBell, XtRBoolean,
	sizeof(Boolean),
	XtOffset(XmwPreeditWidget, preedit.verify_bell),
	XtRImmediate, (caddr_t)True },
};


static void
#if NeedFunctionPrototypes
ValidateString( XmwPreeditWidget w, char *value )
#else /* NeedFunctionPrototypes */
ValidateString( w, value )
    XmwPreeditWidget	w;
    char		*value;
#endif /* NeedFunctionPrototypes */
{
    int		len = strlen(value);
    wchar_t	*ws = (wchar_t *)XtMalloc(sizeof(wchar_t)*(unsigned)(len + 1));

    len = mbstowcs( ws, value, len );
    if( len != -1 ) {
	XtFree( (char *)TextValueWcs(w) );
	ws[len] = '\0';
	w->preedit.string_length = len;
	w->preedit.alloc_size = sizeof(wchar_t) * ( len + ALLOC_SIZE );
	TextValueWcs(w) = wscpy( (wchar_t *)XtMalloc( w->preedit.alloc_size ),
				 ws );
	XtFree( (char *)ws );
    }
    TextValue(w) = NULL;
}


static void
#if NeedFunctionPrototypes
CalculateLine( XmwPreeditWidget w, Position position )
#else /* NeedFunctionPrototypes */
CalculateLine( w, position )
    XmwPreeditWidget	w;
    Position		position;
#endif /* NeedFunctionPrototypes */
{
    int		line;
    Position	*p = w->preedit.line.position;
    Dimension	width;
    wchar_t	*value = w->preedit.wcs_value;
    int		ii;

    if( w->preedit.string_length == 0 ) {
	w->preedit.line.number = 1;
	w->preedit.line.position[0] = 0;
	w->preedit.line.position[1] = 0;
	return;
    }
    if( w->preedit.max_lines == 1 ) {
	w->preedit.line.number = 1;
	w->preedit.line.position[1] = w->preedit.string_length;
	return;
    }

    for( line = w->preedit.line.number - 1; line > 0; line-- )
	if( position >= p[line] )
	    break;

    if( line == 0  &&  w->preedit.width < (w->core.x - w->preedit.x) ) {
	p[1] = w->preedit.string_length;
	w->preedit.line.number = 1;
	return;
    }
    do {
	width = (line == 0) ? (w->preedit.width - (w->core.x - w->preedit.x))
	    : w->preedit.width;
	width -= w->preedit.cursor_width;
	if( line + 1 >= w->preedit.line.maximum ) {
	    w->preedit.line.maximum = line + 2;
	    p = w->preedit.line.position
	      = (Position *)XtRealloc( (char *)p, (Cardinal)((line + 2) *
						  sizeof(Position)) );
	}
	ii = position - p[line];
	while( XwcTextEscapement( TextFontSet(w), value + p[line], ii )
	       <= width ) {
	    p[line+1] = p[line] + ii;
	    if( (p[line+1]) == w->preedit.string_length )
		break;
	    ii++;
	}
	line++;
	position = p[line];
	w->preedit.line.number = line;
    } while( position < w->preedit.string_length );
}


static int
#if NeedFunctionPrototypes
FindLine( XmwPreeditWidget w, Position position )
#else /* NeedFunctionPrototypes */
FindLine( w, position )
    XmwPreeditWidget	w;
    Position		position;
#endif /* NeedFunctionPrototypes */
{
    Position	*p = w->preedit.line.position;
    int		ii;

    if( position >= w->preedit.string_length )
	return( w->preedit.line.number - 1 );

    for( ii = 1; ii <= w->preedit.line.number; ii++ )
        if( position < p[ii] )
            break;

    return( ii - 1 );
}


static Boolean
#if NeedFunctionPrototypes
GetWXYFromPos( XmwPreeditWidget w, Position position, Window *window,
	       Position *x, Position *y )
#else /* NeedFunctionPrototypes */
GetWXYFromPos( w, position, window, x, y )
    XmwPreeditWidget	w;
    Position		position;
    Window		*window;
    Position		*x, *y;
#endif /* NeedFunctionPrototypes */
{
    int		line = FindLine( w, position );
    wchar_t	*value = TextValueWcs(w);
    Position	p;

    *window = w->core.window;
    *x = w->preedit.cursor_width;
    *y = w->preedit.ascent;

    if( position > w->preedit.string_length )
	return( False );

    p = w->preedit.line.position[line];
    if( line == 0 ) {
	*window = w->core.window;
	*x += XwcTextEscapement( TextFontSet(w), value, position );

	if( w->preedit.max_lines == 1 ) {
	    if( w->preedit.attachment == XmwCURSOR_LEFT )
		*x -= XwcTextEscapement( TextFontSet(w), value,
					w->preedit.left_cursor_position );
	    else
		*x += w->core.width - w->preedit.cursor_width
		    - XwcTextEscapement( TextFontSet(w), value,
					 w->preedit.right_cursor_position );
	}
    }
    else if( line == w->preedit.top_line ) {
	*window = w->core.window;
	*x += XwcTextEscapement( TextFontSet(w), &value[p], position - p )
	      - w->core.x + w->preedit.x;
    }
    else if( line + 1 == w->preedit.line.number  &&
	     w->preedit.line.number <= w->preedit.top_line +
				       w->preedit.max_lines ) {
	*window = w->preedit.window3;
	*x += XwcTextEscapement( TextFontSet(w), &value[p], position - p );
    }
    else {
	*window = w->preedit.window2;
	*x += XwcTextEscapement( TextFontSet(w), &value[p], position - p );
	*y += w->preedit.line_spacing * ( line - 1 - w->preedit.top_line );
    }
    return( True );
}


static void	PaintCursor( w )
XmwPreeditWidget	w;
{
    Position		x, y;
    Window		window;

    if( !XtIsRealized((Widget)w)  ||  !w->preedit.display_caret )
	return;

    GetWXYFromPos( w, TextCursorPosition(w), &window, &x, &y );
    x -= w->preedit.cursor_width;
    y -= w->preedit.cursor_height;
    XCopyArea( XtDisplay(w), w->preedit.cursor, window,
	      w->preedit.xor_gc, 0, 0,
	      w->preedit.cursor_width, w->preedit.cursor_height, x, y );
    w->preedit.blink_on = !w->preedit.blink_on;
}


static void	PaintDestination( w )
XmwPreeditWidget	w;
{
    Position	x, y;
    Window	window;

    if( !XtIsRealized( (Widget)w )  ||  !w->preedit.display_caret )
	return;

    GetWXYFromPos( w, TextCursorPosition(w), &window, &x, &y );
    x -= w->preedit.cursor_width -
	((w->preedit.cursor_width - CARET_WIDTH) >> 1);
    y -= CARET_HEIGHT - w->preedit.descent;
    XCopyArea( XtDisplay(w), w->preedit.dest_cursor, window,
	      w->preedit.xor_gc, 0, 0,
	      CARET_WIDTH, CARET_HEIGHT, x, y );
    w->preedit.dest_on = !w->preedit.dest_on;
}


static void
#if NeedFunctionPrototypes
DrawInsertionPoint( XmwPreeditWidget w, Boolean turn_on )
#else
DrawInsertionPoint( w, turn_on )
    XmwPreeditWidget	w;
    Boolean		turn_on;
#endif
{
    if( w->preedit.cursor_on == turn_on  ||  !XtIsRealized((Widget)w) )
        return;
    w->preedit.cursor_on = turn_on & w->preedit.display_caret;
    if( !turn_on  &&  w->preedit.blink_on )
	PaintCursor( w );
    if( turn_on  &&  w->preedit.blink_rate == 0 )
	PaintCursor( w );
}


static void	HandleTimer( closure, id )
caddr_t		closure;
XtIntervalId	id;
{
    XmwPreeditWidget	w = (XmwPreeditWidget)closure;

    if( TextBlinkRate(w) != 0 )
        w->preedit.timer_id =
	    XtAppAddTimeOut( XtWidgetToApplicationContext((Widget)w),
			    (unsigned long)TextBlinkRate(w),
			    (XtTimerCallbackProc)HandleTimer,
			    (caddr_t)closure );
    if( w->preedit.has_focus  &&  XtIsSensitive((Widget)w)  &&
	w->preedit.cursor_on )
        PaintCursor(w);
}


static void
#if NeedFunctionPrototypes
ChangeBlinkBehavior( XmwPreeditWidget w, Boolean turn_on )
#else
ChangeBlinkBehavior( w, turn_on )
    XmwPreeditWidget	w;
    Boolean		turn_on;
#endif
{
    if( turn_on ) {
        if( TextBlinkRate(w) != 0  &&  w->preedit.timer_id == None )
            w->preedit.timer_id =
                XtAppAddTimeOut( XtWidgetToApplicationContext((Widget)w),
			        (unsigned long)TextBlinkRate(w),
                                (XtTimerCallbackProc)HandleTimer,
                                (caddr_t)w );
	else if( w->preedit.blink_rate == 0 )
	    DrawInsertionPoint( w, True );
    }
    else {
        if( w->preedit.timer_id )
            XtRemoveTimeOut( w->preedit.timer_id );
        w->preedit.timer_id = None;
	if( w->preedit.blink_on )
	    DrawInsertionPoint( w, False );
    }
}


static void
#if NeedFunctionPrototypes
LoadGCs( XmwPreeditWidget w, Pixel background, Pixel foreground )
#else /* NeedFunctionPrototypes */
LoadGCs( w, background, foreground )
    XmwPreeditWidget	w;
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
    values.line_width = (w->preedit.ascent > 19) ? 2 : 1;
					mask |= GCLineWidth;
    values.graphics_exposures = False;	mask |= GCGraphicsExposures;
    if( w->preedit.gc != NULL )
	XChangeGC( display, w->preedit.gc, mask, &values );
    else
	w->preedit.gc = XCreateGC( display, window, mask, &values );

    values.foreground = background;	mask |= GCForeground;
    values.background = foreground;	mask |= GCBackground;
    if( w->preedit.inv_gc != NULL )
	XChangeGC( display, w->preedit.inv_gc, mask, &values );
    else
	w->preedit.inv_gc = XCreateGC( display, window, mask, &values );

    values.function = GXxor;
    values.graphics_exposures = FALSE;
    values.foreground = foreground ^ background;
    values.background = 0;
    if( w->preedit.xor_gc != NULL )
	XtReleaseGC( (Widget)w, w->preedit.xor_gc );
    w->preedit.xor_gc = XtGetGC( (Widget)w, mask, &values );
}


static void
#if NeedFunctionPrototypes
MakeIBeamCursor( XmwPreeditWidget w, Pixmap *cursor, int line_width )
#else /* NeedFunctionPrototypes */
MakeIBeamCursor( w, cursor, line_width )
    XmwPreeditWidget	w;
    Pixmap		*cursor;
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

    *cursor = XCreatePixmap( display, window,
			    w->preedit.cursor_width,
			    w->preedit.cursor_height,
			    screen->root_depth );

    XFillRectangle( display, *cursor, gc, 0, 0,
		   w->preedit.cursor_width,
		   w->preedit.cursor_height );
    XSetForeground( display, gc,
		   w->preedit.foreground ^ w->core.background_pixel );

    segments[0].x1 = 0;
    segments[0].y1 = line_width - 1;
    segments[0].x2 = w->preedit.cursor_width;
    segments[0].y2 = line_width - 1;

    segments[1].x1 = 0;
    segments[1].y1 = w->preedit.cursor_height - 1;
    segments[1].x2 = w->preedit.cursor_width;
    segments[1].y2 = w->preedit.cursor_height - 1;

    segments[2].x1 = w->preedit.cursor_width / 2;
    segments[2].y1 = line_width;
    segments[2].x2 = w->preedit.cursor_width / 2;
    segments[2].y2 = w->preedit.cursor_height - 1;

    XDrawSegments( display, *cursor, gc, segments, 3 );

    XFreeGC( display, gc );
}


static void
#if NeedFunctionPrototypes
MakeDestCursor( XmwPreeditWidget w )
#else /* NeedFunctionPrototypes */
MakeDestCursor( w )
    XmwPreeditWidget	w;
#endif /* NeedFunctionPrototypes */
{
    Screen	*screen = XtScreen(w);

    w->preedit.dest_cursor = XCreatePixmapFromBitmapData( XtDisplay(w),
			XtWindow(w),
			(char*)caretBits, CARET_WIDTH, CARET_HEIGHT,
			w->core.background_pixel ^ w->preedit.foreground, 0,
			screen->root_depth );
}


static void
#if NeedFunctionPrototypes
MakeCursors( XmwPreeditWidget w )
#else /* NeedFunctionPrototypes */
MakeCursors( w )
    XmwPreeditWidget	w;
#endif /* NeedFunctionPrototypes */
{
    int		line_width = 1;

    if( !XtIsRealized((Widget)w) )	return;

    w->preedit.cursor_width = 5;
    w->preedit.cursor_height = w->preedit.ascent;

    MakeDestCursor( w );

    if( w->preedit.cursor_height > 19 ) {
	w->preedit.cursor_width++;
	line_width = 2;
    }

    MakeIBeamCursor( w, &(w->preedit.cursor), line_width );
}



static void
#if NeedFunctionPrototypes
ClassInitialize( void )
#else /* NeedFunctionPrototypes */
ClassInitialize()
#endif /* NeedFunctionPrototypes */
{
}


static void
#if NeedFunctionPrototypes
Destroy( Widget w )
#else /* NeedFunctionPrototypes */
Destroy( w )
    Widget	w;
#endif /* NeedFunctionPrototypes */
{
    XmwPreeditWidget	pw = (XmwPreeditWidget)w;
    Display		*display = XtDisplay(w);

    if( pw->preedit.timer_id )
	XtRemoveTimeOut( pw->preedit.timer_id );
    XFreeGC( display, pw->preedit.gc );
    XFreeGC( display, pw->preedit.inv_gc );
    XtReleaseGC( w, pw->preedit.xor_gc );
    XDestroyWindow( display, pw->preedit.window2 );
    XDestroyWindow( display, pw->preedit.window3 );
    XtFree( (char *)pw->preedit.highlight.list );
    XtFree( (char *)pw->preedit.line.position );
    XtFree( pw->preedit.value );
}


static int	FindHighlight( w, position )
XmwPreeditWidget	w;
Position		position;
{
    HighlightRec	*l = w->preedit.highlight.list;
    int			ii;

    for( ii = w->preedit.highlight.number - 1; ii >= 0; ii-- )
        if( position >= l[ii].position )
            return( ii );
    return( 0 );
}


static void	InsertHighlight( w, position, mode )
XmwPreeditWidget	w;
Position		position;
XmwHighlightMode	mode;
{
    HighlightRec	*l = w->preedit.highlight.list;
    int			ii, jj;

    ii = FindHighlight( w, position );

    if( l[ii].position == position ) {
	l[ii].mode = mode;
	if( ii > 0  && l[ii-1].mode == mode ) {
	    for( jj = ii, w->preedit.highlight.number--;
		jj < w->preedit.highlight.number; jj++ )
		l[jj] = l[jj+1];
	    ii--;
	}
	if( ii + 1 != w->preedit.highlight.number  &&  l[ii+1].mode == mode ) {
	    w->preedit.highlight.number--;
	    for( ii++; ii < w->preedit.highlight.number; ii++ )
		l[ii] = l[ii+1];
	}
    }
    else  if( l[ii].mode != mode ) {
	if( ii + 1 != w->preedit.highlight.number  &&  l[ii+1].mode == mode ) {
	    l[ii+1].position = position;
	}
	else {
	    if( w->preedit.highlight.number == w->preedit.highlight.maximum ) {
		w->preedit.highlight.maximum++;
		l = w->preedit.highlight.list = (HighlightRec *)XtRealloc( (char *)l, sizeof(HighlightRec) * w->preedit.highlight.maximum );
	    }
	    for( jj = w->preedit.highlight.number; jj > ii; jj-- )
		l[jj] = l[jj-1];
	    w->preedit.highlight.number++;
	    l[ii+1].position = position;
	    l[ii+1].mode = mode;
	}
    }

    for( ii = 0; ii < w->preedit.highlight.number; ii++ ) {
	if( w->preedit.highlight.list[ii].position > w->preedit.string_length ) {
	    w->preedit.highlight.number = ii;
	    break;
	}
    }
}


static void	PreeditAreaSetHighlight( w, left, right, mode )
XmwPreeditWidget	w;
Position		left, right;
XmwHighlightMode	mode;
{
    XmwHighlightMode	rmode;
    int			l, r;

    if( left >= right )
	return;

    rmode = w->preedit.highlight.list[FindHighlight(w, right)].mode;
    InsertHighlight( w, left, mode );
    l = FindHighlight( w, left );
    r = FindHighlight( w, right - 1 );
    while( l < r ) {
	InsertHighlight( w, w->preedit.highlight.list[r].position, mode );
	r = FindHighlight( w, right );
	if( w->preedit.highlight.list[r].mode == mode )
	    r--;
    }
    InsertHighlight( w, right, rmode );
}


static void	RedisplayText( w, start, end )
XmwPreeditWidget	w;
Position		start, end;
{
    Display		*display = XtDisplay(w);
    wchar_t		*value = w->preedit.wcs_value;
    HighlightRec	*list = w->preedit.highlight.list;
    Position		*position = w->preedit.line.position;
    int			x, y, width, height;
    int			ii;
    int			sline = FindLine( w, start );
    int			eline = FindLine( w, end );
    Position		p, _end;
    Window		win;
    GC			gc;
    int			top_line = w->preedit.top_line;

    if( !XtIsRealized((Widget)w) )	return;
    if( sline < w->preedit.top_line ) {
	sline = w->preedit.top_line;
	start = w->preedit.line.position[sline];
    }
    if( eline >= w->preedit.top_line + w->preedit.max_lines ) {
	eline = w->preedit.top_line + w->preedit.max_lines;
	end = w->preedit.line.position[eline];
    }
    if( sline > eline )
	return;

    DrawInsertionPoint( w, False );

    XSelectInput( display, w->core.window,
		  w->preedit.event_mask & ~ExposureMask );
    gc = w->preedit.inv_gc;
    if( sline == top_line ) {
	if( w->preedit.line.number == 1 )
	    width = XwcTextEscapement( TextFontSet(w), value, 
				       w->preedit.string_length );
	else
	    width = w->preedit.width - ( w->core.x - w->preedit.x )
		    - w->preedit.cursor_width;
	w->core.width = width + w->preedit.cursor_width;
	if( w->core.width > w->preedit.width - ( w->core.x - w->preedit.x ) )
	    w->core.width = w->preedit.width - ( w->core.x - w->preedit.x );
	if( w->core.width > w->preedit.width )
	    w->core.width = w->preedit.width;

	XMoveResizeWindow( display, XtWindow(w), w->core.x, w->core.y,
			   w->core.width, w->core.height );
	XFillRectangle( display, w->core.window, gc,
		        width + (w->preedit.cursor_width / 2), 0,
		        w->preedit.cursor_width, w->core.height );
    }
    if( eline != w->preedit.top_line  &&
	(eline + 1) == w->preedit.line.number  &&
	w->preedit.line.number <= w->preedit.top_line + w->preedit.max_lines ) {
	p = position[eline];
	width = XwcTextEscapement( TextFontSet(w), &value[p],
				   w->preedit.string_length - p );
	w->preedit.width3 = width + w->preedit.cursor_width;
	y = w->preedit.y3;
	w->preedit.y3 = w->core.y + w->preedit.line_spacing *
			( eline - w->preedit.top_line )
			+ w->core.border_width *
			((eline == w->preedit.top_line + 1) ? 1 : 2);
	if( y != w->preedit.y3  &&  start == end )
	    start = position[sline];

	if( w->preedit.y3 <= (w->preedit.y + w->preedit.height) ) {
	    height = w->preedit.height3;
	    XMoveResizeWindow( display, w->preedit.window3, w->preedit.x3,
			       w->preedit.y3, w->preedit.width3, height );
	    XFillRectangle( display, w->preedit.window3, gc,
			    width + (w->preedit.cursor_width / 2), 0,
			    w->preedit.cursor_width, height );
	    XMapWindow( display, w->preedit.window3 );
	}
	else
	    XUnmapWindow( display, w->preedit.window3 );
    }
    if( (sline == w->preedit.top_line  &&
	 (eline + 1) == w->preedit.line.number  &&  (eline - sline >= 2))  ||
	(sline != eline  &&
	 (sline == w->preedit.top_line  ||
	 (eline + 1) == w->preedit.line.number)  &&
	 w->preedit.line.number >= 3)  ||
	(w->preedit.max_lines >= 3  &&  w->preedit.line.number >= 3  &&
	 w->preedit.y3 < w->preedit.y2 + w->preedit.height2) ) {
	if( w->preedit.line.number - w->preedit.top_line >
	    w->preedit.max_lines )
	    w->preedit.height2 = w->core.height * (w->preedit.max_lines - 1);
	else
	    w->preedit.height2 = w->core.height *
		(min(w->preedit.max_lines, w->preedit.line.number) - 2);
	if( w->preedit.y2 <= (w->preedit.y + w->preedit.height) ) {
	    if( w->preedit.y2 + w->preedit.height2 >
		w->preedit.y + w->preedit.height )
		w->preedit.height2 = w->preedit.y + w->preedit.height
				     - w->preedit.y2;
	    if( w->preedit.height2 > 0 ) {
		XMoveResizeWindow( display, w->preedit.window2,
				   w->preedit.x2, w->preedit.y2,
				   w->preedit.width2, w->preedit.height2 );
		XMapWindow( display, w->preedit.window2 );
	    }
	    else
		XUnmapWindow( display, w->preedit.window2 );
	}
	else
	    XUnmapWindow( display, w->preedit.window2 );
    }

    if( w->preedit.line.number > w->preedit.top_line + w->preedit.max_lines )
	XUnmapWindow( display, w->preedit.window3 );
    if( min( w->preedit.line.number, w->preedit.max_lines ) == 1 )  {
	XUnmapWindow( display, w->preedit.window3 );
	XUnmapWindow( display, w->preedit.window2 );
    }
    if( w->preedit.line.number == 2 )
	XUnmapWindow( display, w->preedit.window2 );
    if( w->preedit.max_lines == 2  &&
	w->preedit.top_line + w->preedit.max_lines == w->preedit.line.number )
	XUnmapWindow( display, w->preedit.window2 );

    ii = 0;
    while( start != end  &&  ii < w->preedit.highlight.number ) {
	if( list[ii].position <= start  &&
	    (w->preedit.highlight.number == ii + 1  ||
	     start < list[ii+1].position) ) {
	    sline = FindLine( w, start );
	    x = w->preedit.cursor_width >> 1;
	    y = w->preedit.ascent;
	    if( sline == 0 ) {
		win = w->core.window;
		if( w->preedit.max_lines == 1 ) {
		    start = list[ii].position;
		    if( w->preedit.attachment == XmwCURSOR_LEFT )
			x -= XwcTextEscapement( TextFontSet(w), value,
					    w->preedit.left_cursor_position );
		    else
			x = w->core.width - (w->preedit.cursor_width >> 1)
			    - XwcTextEscapement( TextFontSet(w), value,
					     w->preedit.right_cursor_position );
		    x += XwcTextEscapement( TextFontSet(w), value, start );
		}
		else
		    x += XwcTextEscapement( TextFontSet(w), value, start );
	    }
	    else if( sline == w->preedit.top_line ) {
		win = w->core.window;
		x += XwcTextEscapement( TextFontSet(w), &value[position[sline]],
				       start - position[sline] )
		     - w->core.x + w->preedit.x;
	    }
	    else if( sline + 1 == w->preedit.line.number  &&
		     w->preedit.line.number <= w->preedit.top_line +
					       w->preedit.max_lines ) {
		win = w->preedit.window3;
		x += XwcTextEscapement( TextFontSet(w),
				       &value[position[sline]],
				       start - position[sline] );
	    }
	    else {
		win = w->preedit.window2;
		x += XwcTextEscapement( TextFontSet(w),
				       &value[position[sline]],
				       start - position[sline] );
		y += w->preedit.line_spacing *
		     ( sline - 1 - w->preedit.top_line );
	    }

	    _end = end;
	    if( ii+1 < w->preedit.highlight.number  &&
		_end > list[ii+1].position )
		_end = list[ii+1].position;
	    if( sline+1 < w->preedit.line.number  &&  _end > position[sline+1] )
		_end = position[sline+1];

	    switch( list[ii].mode ) {
	      case XmwHIGHLIGHT_NORMAL:
	      case XmwHIGHLIGHT_SECONDARY_SELECTED:
		gc = w->preedit.gc;
		break;

	      case XmwHIGHLIGHT_SELECTED:
		gc = w->preedit.inv_gc;
		break;

	      default:
		gc = w->preedit.gc;
		break;
	    }

	    XwcDrawImageString( display, win, w->preedit.font_set,
				gc, x, y, value + start, _end - start );

	    if( list[ii].mode == XmwHIGHLIGHT_SECONDARY_SELECTED )
		XDrawLine( display, win, gc,
			  x, y + w->preedit.descent - 1,
			  x + XwcTextEscapement( TextFontSet(w), value + start,
						 _end - start ),
			  y + w->preedit.descent - 1 );

	    start = _end;
	}
	else
	    ii++;
    }

    if( w->preedit.has_focus )
	DrawInsertionPoint( w, True );
    else
	PaintDestination( w );
    XSelectInput( display, w->core.window, w->preedit.event_mask );
}


static void
#if NeedFunctionPrototypes
SetCursorPosition( XmwPreeditWidget w, Position position )
#else
SetCursorPosition( w, position )
    XmwPreeditWidget	w;
    Position		position;
#endif
{
    Boolean	cursor_on = w->preedit.cursor_on;
    Position	*p = w->preedit.line.position;
    int		old_line, new_line;
    int		n;
    Boolean	redraw = False;
    wchar_t	*value = w->preedit.wcs_value;
    Dimension	width;

    if( position < 0 )
	position = 0;

    if( position > w->preedit.string_length )
	position = w->preedit.string_length;

    if( w->preedit.max_lines == 1 ) {
	if( XwcTextEscapement( TextFontSet(w), value,
			       w->preedit.string_length ) <
	    w->preedit.width - ( w->core.x - w->preedit.x ) ) {
	    w->preedit.left_cursor_position = 0;
	    w->preedit.right_cursor_position = w->preedit.string_length;
	    w->preedit.attachment = XmwCURSOR_LEFT;

	    XFillRectangle( XtDisplay(w), XtWindow(w), w->preedit.inv_gc,
			    0, 0, w->preedit.cursor_width / 2, w->core.height );
	}
	else if( position < w->preedit.left_cursor_position ) {
	    width = w->preedit.width - ( w->core.x - w->preedit.x );
	    for( n = position + 1;
		 width > XwcTextEscapement( TextFontSet(w), &value[position],
					    n - position ); n++ );
	    w->preedit.right_cursor_position = n - 1;
	    w->preedit.left_cursor_position = position;
	    w->preedit.attachment = XmwCURSOR_LEFT;
	    redraw = True;
	}
	else if( position > w->preedit.right_cursor_position ) {
	    width = (w->preedit.cursor_width >> 1)
		    + XwcTextEscapement( TextFontSet(w), value, position )
		    - (w->preedit.width - ( w->core.x - w->preedit.x ));
	    for( n = 1; XwcTextEscapement( TextFontSet(w), value, n ) < width;
		 n++ );
	    w->preedit.left_cursor_position = n;
	    w->preedit.right_cursor_position = position;
	    w->preedit.attachment = XmwCURSOR_RIGHT;
	    redraw = True;
	}
	else if( w->preedit.attachment == XmwCURSOR_RIGHT  &&
		 w->preedit.string_length < w->preedit.right_cursor_position ) {
	    width = (w->preedit.cursor_width >> 1)
		    + XwcTextEscapement( TextFontSet(w), value,
					 w->preedit.string_length )
		    - (w->preedit.width - ( w->core.x - w->preedit.x ));
	    for( n = 1; XwcTextEscapement( TextFontSet(w), value, n ) < width;
		 n++ );
	    w->preedit.left_cursor_position = n;
	    w->preedit.right_cursor_position = w->preedit.string_length;
	    redraw = True;
	}
    }

    old_line = FindLine( w, TextCursorPosition(w) );
    new_line = FindLine( w, position );
    if( TextCursorPosition(w) < position ) {
	for( n = 1; n <= w->preedit.line.number; n++ ) {
	    if( p[n] == TextCursorPosition(w) ) {
		old_line--;
		break;
	    }
	}
    }

    if( w->preedit.cursor_on )
	DrawInsertionPoint( w, False );

    TextCursorPosition(w) = position;

    if( old_line > new_line ) {
	if( w->preedit.top_line > new_line ) {
	    w->preedit.top_line = new_line;
	    redraw = True;
	}
    }
    else if( old_line < new_line ) {
	if( w->preedit.top_line + w->preedit.max_lines <= new_line ) {
	    w->preedit.top_line = new_line + 1 - w->preedit.max_lines;
	    redraw = True;
	}
    }
    else {
	if( w->preedit.top_line + w->preedit.max_lines >
	    w->preedit.line.number  &&
	    w->preedit.max_lines <= w->preedit.line.number ) {
	    w->preedit.top_line = w->preedit.line.number - w->preedit.max_lines;
	    redraw = True;
	}
    }
    if( w->preedit.top_line > 0  &&  new_line == w->preedit.top_line ) {
	w->preedit.top_line--;
	redraw = True;
    }

    if( redraw ) {
	XClearArea( XtDisplay(w), XtWindow(w), 0, 0, 0, 0, True );
	XClearWindow( XtDisplay(w), w->preedit.window2 );
	XClearWindow( XtDisplay(w), w->preedit.window3 );
    }

    if( cursor_on )
	DrawInsertionPoint( w, True );
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
    XmwPreeditWidget	cw = (XmwPreeditWidget)current;
    XmwPreeditWidget	rw = (XmwPreeditWidget)request;
    XmwPreeditWidget	nw = (XmwPreeditWidget)new;
    Boolean		changed = False;
    Display		*d = XtDisplay( request );
    XFontStruct		**fs;
    char		**fn;
    int			n;
    Dimension		height;

    if( cw->core.width != rw->core.width )
	nw->core.width = cw->core.width;
    if( cw->core.height != rw->core.height )
	nw->core.height = cw->core.height;

    if( nw->preedit.font_set != cw->preedit.font_set ) {
	if( nw->preedit.font_set != NULL )
	    XFreeFontSet( d, cw->preedit.font_set );
	n = XFontsOfFontSet( TextFontSet(rw), &fs, &fn );
	nw->preedit.ascent = 0;
	nw->preedit.descent = 0;
	while( --n >= 0 ) {
	    if( nw->preedit.ascent < fs[n]->ascent )
		nw->preedit.ascent = fs[n]->ascent;
	    if( nw->preedit.descent < fs[n]->descent )
		nw->preedit.descent = fs[n]->descent;
	}
	MakeCursors( nw );
	XClearWindow( d, XtWindow(cw) );
	XClearWindow( d, nw->preedit.window2 );
	XClearWindow( d, nw->preedit.window3 );
	if( rw->preedit.line_spacing == cw->preedit.line_spacing )
	    nw->preedit.line_spacing = nw->preedit.ascent + nw->preedit.descent;
	changed = True;
    }

    if( rw->preedit.focus_window != cw->preedit.focus_window ) {
	XReparentWindow( XtDisplay(request), rw->core.window,
			rw->preedit.focus_window, rw->core.x, rw->core.y );
	XReparentWindow( XtDisplay(request), rw->preedit.window2,
			rw->preedit.focus_window,
			rw->preedit.x2, rw->preedit.y2 );
	XReparentWindow( XtDisplay(request), rw->preedit.window3,
			rw->preedit.focus_window,
			rw->preedit.x3, rw->preedit.y3 );
    }

    if( rw->core.x != cw->core.x  ||  rw->core.y != cw->core.y ) {
	cw->core.x = nw->core.x;
	cw->core.y = nw->core.y;
	nw->preedit.top_line = 0;
	nw->preedit.max_lines =
		(nw->preedit.height -
		 (nw->core.y - nw->preedit.descent - nw->preedit.y))
		/ nw->preedit.line_spacing;
	if( nw->preedit.max_lines == 0 )
	    nw->preedit.max_lines = 1;
	nw->preedit.y2 = nw->core.y + nw->core.height + nw->core.border_width;
	CalculateLine( nw, 0 );
	changed = True;
    }

    if( rw->preedit.x != cw->preedit.x  ||  rw->preedit.y != cw->preedit.y  ||
       rw->preedit.width != cw->preedit.width  ||
       rw->preedit.height != cw->preedit.height  ||
       rw->preedit.line_spacing != cw->preedit.line_spacing ) {
	nw->preedit.x2 = nw->preedit.x3 = rw->preedit.x;
	nw->preedit.width2 = nw->preedit.width;
	if( nw->preedit.line_spacing <= 0 )
	    nw->preedit.line_spacing = nw->preedit.ascent + nw->preedit.descent;
	cw->core.height = nw->core.height = nw->preedit.height3
			= nw->preedit.line_spacing;
	nw->preedit.max_lines =
		(nw->preedit.height -
		 (nw->core.y - nw->preedit.descent - nw->preedit.y))
		/ nw->preedit.line_spacing;
	if( nw->preedit.max_lines == 0 )
	    nw->preedit.max_lines = 1;
	nw->preedit.y2 = nw->core.y + nw->preedit.line_spacing +
			 nw->core.border_width;
	CalculateLine( nw, 0 );
	nw->preedit.area_needed_width = nw->preedit.width;
	nw->preedit.area_needed_height = nw->preedit.height;
    SetCursorPosition( nw, 0 );
    SetCursorPosition( nw, nw->preedit.string_length );
    SetCursorPosition( nw, TextCursorPosition(cw) );
	if( nw->preedit.width < cw->preedit.width )
	    XClearWindow( XtDisplay(nw), nw->preedit.window2 );
	changed = True;
    }

    if( nw->preedit.area_needed_width <= 0 )
	nw->preedit.area_needed_width = nw->preedit.width;
    height = nw->preedit.ascent + nw->preedit.descent;
    if( nw->preedit.area_needed_height < height )
	nw->preedit.area_needed_height = height;

    if( rw->preedit.foreground != cw->preedit.foreground  ||
       rw->core.background_pixel != cw->core.background_pixel ) {
	LoadGCs( nw, nw->preedit.foreground, nw->core.background_pixel );
	changed = True;
    }

    if( wscmp( nw->preedit.wcs_value, cw->preedit.wcs_value ) ) {
	ChangeBlinkBehavior( nw, False );

	XtFree( (char *)TextValueWcs(cw) );
	if( nw->preedit.wcs_value == NULL ) {
	    nw->preedit.string_length = 0;
	    nw->preedit.alloc_size = sizeof(wchar_t) * ALLOC_SIZE;
	    TextValueWcs(nw) = (wchar_t *)XtMalloc( nw->preedit.alloc_size );
	    TextValueWcs(nw)[0] = '\0';
	}
	else {
	    nw->preedit.string_length = wslen( TextValueWcs(nw) );
	    nw->preedit.alloc_size = sizeof(wchar_t) *
				  ( nw->preedit.string_length + ALLOC_SIZE );
	    TextValueWcs(nw) =
		wscpy( (wchar_t *)XtMalloc( nw->preedit.alloc_size ),
		       TextValueWcs(rw) );
	}

	CalculateLine( nw, 0 );
	nw->preedit.highlight.number = 1;
	nw->preedit.highlight.list[0].position = 0;

	XmwPreeditSetCursorPosition( new, nw->preedit.string_length );
	if( nw->core.sensitive  &&  nw->preedit.has_focus  &&
	    nw->preedit.display_caret )
	    ChangeBlinkBehavior( nw, True );

	changed = True;
	nw->preedit.value = NULL;
    }
    else if( nw->preedit.value != NULL ) {
	ChangeBlinkBehavior( nw, False );

	ValidateString( nw, nw->preedit.value );
	CalculateLine( nw, 0 );
	nw->preedit.highlight.number = 1;
	nw->preedit.highlight.list[0].position = 0;

	XmwPreeditSetCursorPosition( new, nw->preedit.string_length );
	if( nw->core.sensitive  &&  nw->preedit.has_focus  &&
	    nw->preedit.display_caret )
	    ChangeBlinkBehavior( nw, True );

	changed = True;
	nw->preedit.value = NULL;
    }

    if( nw->preedit.cursor_position != cw->preedit.cursor_position )
	changed = True;

    if( nw->preedit.display_caret != cw->preedit.display_caret ) {
	if( nw->preedit.display_caret ) {
	    ChangeBlinkBehavior( nw, True );
	}
	else {
	    nw->preedit.display_caret = True;
	    ChangeBlinkBehavior( nw, False );
	    nw->preedit.display_caret = False;
	}
	nw->preedit.cursor_on = nw->preedit.display_caret;
    }

    if( nw->preedit.has_focus != cw->preedit.has_focus ) {
	if( nw->preedit.has_focus ) {
	    PaintDestination( nw );
	    DrawInsertionPoint( nw, True );
	    if( nw->core.sensitive )
		ChangeBlinkBehavior( nw, True );
	}
	else {
	    DrawInsertionPoint( nw, False );
	    PaintDestination( nw );
	    if( nw->core.sensitive )
		ChangeBlinkBehavior( nw, False );
	}
    }

    return( changed );
}


static Boolean	ReplaceText( w, event, replace_prev, replace_next, insert,
			     insert_length )
XmwPreeditWidget	w;
XEvent			*event;
Position		replace_prev, replace_next;
wchar_t			*insert;
int			insert_length;
{
    int			replace_length, ii, jj;
    wchar_t		*src, *dst;
    int			delta = 0;
    Position		cursorPos;
    wchar_t		*orig_insert;
    XmwHighlightMode	highlight_mode;
    HighlightRec	*l;
    int			number;

    if( !TextEditable(w) ) {
	if( w->preedit.verify_bell )
	    XBell( XtDisplay((Widget)w), 0 );
	return( False );
    }

    if( w->preedit.cursor_on )
	DrawInsertionPoint( w, False );

    highlight_mode = w->preedit.highlight.list[FindHighlight( w, replace_prev )].mode;
    orig_insert = insert;
    replace_length = replace_next - replace_prev;

    if( (w->preedit.string_length + insert_length - replace_length) - 
       w->preedit.max_length > 0) { 
	if( w->preedit.verify_bell )
	    XBell( XtDisplay( (Widget)w), 0 );
	if( insert != orig_insert )
	    XtFree( (char *)insert );
	return( False );
    }

    if( w->preedit.string_length + insert_length - replace_length >=
        (w->preedit.alloc_size / sizeof(wchar_t)) ) {
	w->preedit.alloc_size += max( insert_length + 1, ALLOC_SIZE );
	TextValueWcs(w) = (wchar_t *)XtRealloc( (char *)TextValueWcs(w),
						w->preedit.alloc_size );
    }

    delta = insert_length - replace_length;

    if( replace_length > insert_length ) {
	for( src = TextValueWcs(w) + replace_next, dst = src + delta,
	    ii = (w->preedit.string_length + 1) - replace_next;
	    ii > 0; ++src, ++dst, --ii )
	    *dst = *src;
	l = w->preedit.highlight.list;
	for( ii = w->preedit.highlight.number - 1; ii >= 0; ii-- )
	    if( replace_next > l[ii].position )
		break;
	jj = ii;
	jj++;
	while( jj < w->preedit.highlight.number ) {
	    l[jj].position += delta;
	    jj++;
	}
	if( (ii + 1) < w->preedit.highlight.number ) {
	    number = 0;
	    while( l[ii].position >= l[ii+number+1].position  &&
		   (ii+number+1) < w->preedit.highlight.number )
		number++;
	    if( number > 0 ) {
		jj = ii;
		while( jj < w->preedit.highlight.number ) {
		    l[jj] = l[jj+number];
		    jj++;
		}
		w->preedit.highlight.number -= number;
		if( ii > 0  &&  l[ii].mode == l[ii-1].mode ) {
		    while( ii < w->preedit.highlight.number ) {
			l[ii] = l[ii+1];
			ii++;
		    }
		    w->preedit.highlight.number--;
		}
	    }
	}
    }
    else if( replace_length < insert_length ) {
	for( src = TextValueWcs(w) + w->preedit.string_length,
	     dst = src + delta,
	     ii = (w->preedit.string_length + 1) - replace_next;
	     ii > 0; src--, dst--, ii-- )
	    *dst = *src;
	l = w->preedit.highlight.list;
	for( ii = w->preedit.highlight.number - 1; ii > 0; ii-- )
	    if( replace_next > l[ii].position )
		break;
	ii++;
	while( ii < w->preedit.highlight.number ) {
	    l[ii].position += delta;
	    ii++;
	}
	PreeditAreaSetHighlight( w, replace_prev, replace_prev + insert_length,
				 highlight_mode );
    }

    if( insert_length != 0 ) {
	for( src = insert, dst = TextValueWcs(w) + replace_prev,
	    ii = insert_length;
	    ii > 0; ++src, ++dst, --ii )
	    *dst = *src;
    }

    w->preedit.string_length += delta;
    cursorPos = replace_next + delta;
    CalculateLine( w, replace_prev );
    SetCursorPosition( w, cursorPos );

    if( replace_length == insert_length )
	replace_next = replace_prev + replace_length;
    else
	replace_next = w->preedit.string_length;
    if( FindLine(w, replace_next) + 1 == w->preedit.line.number )
	replace_prev = w->preedit.line.position[FindLine(w, replace_next)];
    RedisplayText( w, replace_prev, replace_next );

    if( insert != orig_insert )
	XtFree( (char *)insert );
    return( True );
}


static void
#if NeedFunctionPrototypes
Initialize( Widget request, Widget new, ArgList args, Cardinal *num_args )
#else /* NeedFunctionPrototypes */
Initialize( request, new, args, num_args )
    Widget	request, new;
    ArgList	args;
    Cardinal	*num_args;
#endif /* NeedFunctionPrototypes */
{
    XmwPreeditWidget	rw = (XmwPreeditWidget)request;
    XmwPreeditWidget	nw = (XmwPreeditWidget)new;
    XFontStruct		**fs;
    char		**fn;
    int			n;
    wchar_t		*wcs;

    if( TextCursorPosition(rw) < 0 ) {
	XtWarning( "Invalid cursor position, must be >= 0." );
	TextCursorPosition(nw) = 0;
    }

    if( TextColumns(rw) <= 0 ) {
	XtWarning( "Invalid columns, must be > 0." );
	TextColumns(nw) = 1;
    }

    nw->core.sensitive = False;

    nw->preedit.x2 = nw->preedit.x3 = nw->preedit.x;
    nw->preedit.y3 = 0;
    nw->preedit.width2 = nw->preedit.width;
    nw->preedit.window2 = None;
    nw->preedit.window3 = None;

    n = XFontsOfFontSet( TextFontSet(nw), &fs, &fn );
    nw->preedit.ascent = 0;
    nw->preedit.descent = 0;
    while( --n >= 0 ) {
	if( nw->preedit.ascent < fs[n]->ascent )
	    nw->preedit.ascent = fs[n]->ascent;
	if( nw->preedit.descent < fs[n]->descent )
	    nw->preedit.descent = fs[n]->descent;
    }
    if( nw->preedit.line_spacing == 0 )
	nw->preedit.line_spacing = nw->preedit.ascent + nw->preedit.descent;
    nw->core.height = nw->preedit.height3 = nw->preedit.line_spacing;
    if( nw->core.height > nw->preedit.line_spacing )
	nw->core.height = nw->preedit.line_spacing;
    nw->preedit.y2 = nw->core.y + nw->core.height + nw->core.border_width;

    if( nw->preedit.area_needed_width <= 0 )
	nw->preedit.area_needed_width = nw->preedit.width;
    nw->preedit.area_needed_height = nw->preedit.ascent + nw->preedit.descent;

    nw->preedit.blink_on = False;
    nw->preedit.cursor_on = nw->preedit.display_caret;
    nw->preedit.dest_on = False;

    nw->preedit.gc = NULL;
    nw->preedit.inv_gc = NULL;
    nw->preedit.xor_gc = NULL;

    if( TextValueWcs(nw) == NULL ) {
	if( TextValue(nw) == NULL ) {
	    nw->preedit.string_length = 0;
	    nw->preedit.alloc_size = sizeof(wchar_t) * ALLOC_SIZE;
	    TextValueWcs(nw) = (wchar_t *)XtMalloc( nw->preedit.alloc_size );
	    TextValueWcs(nw)[0] = '\0';
	}
	else
	    ValidateString( nw, TextValue(nw) );
    }
    else {
	wcs = TextValueWcs(nw);
	nw->preedit.string_length = wslen( wcs );
	nw->preedit.alloc_size = sizeof(wchar_t) *
				  ( nw->preedit.string_length + ALLOC_SIZE );
	TextValueWcs(nw) = wscpy( (wchar_t *)XtMalloc( nw->preedit.alloc_size ),
				 wcs );
	TextValue(nw) = NULL;
    }

    if( TextMaxLength(nw) < MAXINT ) {
	if( nw->preedit.string_length > TextMaxLength(nw) ) {
	    nw->preedit.string_length = TextMaxLength(nw);
	    TextValueWcs(nw)[nw->preedit.string_length] = '\0';
	}
    }

    nw->preedit.cursor_height = nw->preedit.cursor_width = 0;

    nw->preedit.highlight.number = nw->preedit.highlight.maximum = 1;
    nw->preedit.highlight.list
	= (HighlightRec *)XtMalloc( (unsigned)sizeof(HighlightRec) );
    nw->preedit.highlight.list[0].position = 0;
    nw->preedit.highlight.list[0].mode = XmwHIGHLIGHT_SECONDARY_SELECTED;

    nw->preedit.line.number = 1;
    nw->preedit.line.maximum = 2;
    nw->preedit.line.position
	= (Position *)XtMalloc( (unsigned)sizeof(Position) * 2 );
    nw->preedit.line.position[0] = 0;
    nw->preedit.line.position[1] = 0;

    nw->preedit.timer_id = None;

    nw->preedit.cursor_width = 5;

    nw->core.width = 5;

    TextCursorPosition(nw) = 0;
    TextColumns(nw) = 1;
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
    XmwPreeditWidget		pw = (XmwPreeditWidget)widget;
    Display			*d = XtDisplay(widget);
    XSetWindowAttributes	at;

    attr->border_pixel = pw->preedit.foreground;
    *vmask |= CWBitGravity | CWEventMask;
    attr->bit_gravity = NorthWestGravity;
    attr->event_mask |= KeyPressMask;
    pw->preedit.event_mask = attr->event_mask;

    pw->core.window
	= XCreateWindow( d,
			pw->preedit.focus_window,
			pw->core.x, widget->core.y,
			pw->core.width, widget->core.height,
			pw->core.border_width,
			pw->core.depth, InputOutput,
			CopyFromParent, *vmask, attr );
    pw->preedit.window2
	= XCreateSimpleWindow( d,
			      pw->preedit.focus_window,
			      0, 0, 1, 1, pw->core.border_width,
			      pw->preedit.foreground,
			      pw->core.background_pixel );
    pw->preedit.window3
	= XCreateSimpleWindow( d,
			      pw->preedit.focus_window,
			      0, 0, 1, 1, pw->core.border_width,
			      pw->preedit.foreground,
			      pw->core.background_pixel );
    at.bit_gravity = NorthWestGravity;
    at.event_mask = KeyPressMask;
    XChangeWindowAttributes( d, pw->preedit.window2, CWBitGravity | CWEventMask,
			     &at );
    XChangeWindowAttributes( d, pw->preedit.window3, CWBitGravity | CWEventMask,
			     &at );

    LoadGCs( pw, pw->core.background_pixel, pw->preedit.foreground );
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
    XmwPreeditWidget	pw = (XmwPreeditWidget)w;

    pw->core.height = pw->preedit.height3;
    RedisplayText( w, TextCursorPosition(pw), pw->preedit.string_length );
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
    XmwPreeditWidget	pw = (XmwPreeditWidget)w;
    Position		x, y;
    Window		window;

    if( !pw->core.sensitive )
	return;

    if( pw->preedit.cursor_on  ||  pw->preedit.dest_on ) {
	GetWXYFromPos( pw, TextCursorPosition(pw), &window, &x, &y );
	x -= pw->preedit.cursor_width;
	y -= pw->preedit.cursor_height;
	XFillRectangle( XtDisplay(w), window, pw->preedit.inv_gc,
		       x, y, pw->preedit.cursor_width, pw->core.height );
	pw->preedit.cursor_on = pw->preedit.blink_on = False;
	pw->preedit.dest_on = False;
    }
    RedisplayText( pw, 0, pw->preedit.string_length );
}


XmwPreeditClassRec	xmwPreeditClassRec = 
{
  {
    /* superclass	     */ (WidgetClass) &widgetClassRec,
    /* class_name	     */ "XmwPreedit",
    /* widget_size	     */ sizeof(XmwPreeditRec),
    /* class_initialize	     */ ClassInitialize,
    /* class_part_initialize */ NULL,
    /* class_inited          */ False,
    /* initialize	     */ Initialize,
    /* initialize_hook       */ NULL,
    /* realize		     */ Realize,
    /* actions               */ NULL,
    /* num_actions    	     */ 0,
    /* resources	     */ local_resources,
    /* num_resources         */ XtNumber(local_resources),
    /* xrm_class	     */ NULLQUARK,
    /* compress_motion       */ FALSE,
    /* compress_exposure     */ TRUE,
    /* compress_enterleave   */ FALSE,
    /* visible_interest      */ FALSE,
    /* destroy               */ Destroy,
    /* resize                */ Resize,
    /* expose                */ Redisplay,
    /* set_values	     */ SetValues,
    /* set_values_hook       */ NULL,
    /* set_values_almost     */ XtInheritSetValuesAlmost,
    /* get_values_hook       */ NULL,
    /* accept_focus	     */ NULL,
    /* version               */ XtVersion,
    /* callback private      */ NULL,
    /* tm_table              */ NULL,
    /* query_geometry        */ XtInheritQueryGeometry,
    /* display_accelerator   */ NULL,
    /* extension             */ NULL,
   },
   {
    /* extension	     */ NULL,
   },

};

WidgetClass	xmwPreeditClass = (WidgetClass)&xmwPreeditClassRec;


Widget
#if NeedFunctionPrototypes
XmwCreatePreedit( Widget parent, char *name, ArgList arglist,
			  Cardinal argcount )
#else /* NeedFunctionPrototypes */
XmwCreatePreedit( parent, name, arglist, argcount )
    Widget	parent;
    char	*name;
    ArgList	arglist;
    Cardinal	argcount;
#endif /* NeedFunctionPrototypes */
{
    Widget	w;

    w = XtCreateManagedWidget( name, xmwPreeditClass, parent, arglist,
			       argcount );
    XtAddGrab( w, False, False );
    return( w );
}


void
#if NeedFunctionPrototypes
XmwPreeditSetHighlight( Widget w, Position left, Position right,
			XmwHighlightMode mode )
#else /* NeedFunctionPrototypes */
XmwPreeditSetHighlight( w, left, right, mode )
    Widget		w;
    Position		left, right;
    XmwHighlightMode	mode;
#endif /* NeedFunctionPrototypes */
{
    XmwPreeditWidget	pw = (XmwPreeditWidget)w;

    if( left >= right  ||  right <= 0 )	return;

    if( left < 0 )	left = 0;

    if( right > pw->preedit.string_length )
	right = pw->preedit.string_length;

    PreeditAreaSetHighlight( pw, left, right, mode );

    RedisplayText( pw, left, right );
}


Boolean	
#if NeedFunctionPrototypes
XmwPreeditReplace( Widget w, Position from_pos, Position to_pos,
		   char *value )
#else /* NeedFunctionPrototypes */
XmwPreeditReplace( w, from_pos, to_pos, value )
    Widget	w;
    Position	from_pos, to_pos;
    char	*value;
#endif /* NeedFunctionPrototypes */
{
    XmwPreeditWidget	pw = (XmwPreeditWidget)w;
    Boolean		save_editable = TextEditable(pw);
    Boolean		result;
    int			len;
    wchar_t		*wcs;

    if( from_pos < 0 )
	from_pos = 0;

    if( to_pos >= pw->preedit.string_length )
	to_pos = pw->preedit.string_length;

    if( value == NULL )
	value = "";
    len = strlen( value );
    wcs = (wchar_t *)XtMalloc( sizeof(wchar_t) * ( len + 1 ) );
    len = mbstowcs( wcs, value, len );

    TextEditable(pw) = True;
    result = ReplaceText( pw, NULL, from_pos, to_pos, wcs, len );
    TextEditable(pw) = save_editable;
    XtFree( (char *)wcs );
    return( result );
}


Boolean
#if NeedFunctionPrototypes
XmwPreeditReplaceWcs( Widget w, Position from_pos, Position to_pos,
		      wchar_t *value )
#else /* NeedFunctionPrototypes */
XmwPreeditReplaceWcs( w, from_pos, to_pos, value )
    Widget	w;
    Position	from_pos, to_pos;
    wchar_t	*value;
#endif /* NeedFunctionPrototypes */
{
    XmwPreeditWidget	pw = (XmwPreeditWidget)w;
    Boolean		save_editable = TextEditable(pw);
    Boolean		result;
    wchar_t		*wcs;

    if( from_pos < 0 )
	from_pos = 0;

    if( to_pos >= pw->preedit.string_length )
	to_pos = pw->preedit.string_length;

    if( value == NULL ) {
	wcs = (wchar_t *)XtMalloc( sizeof(wchar_t) );
	wcs[0] = '\0';
    }
    else
	wcs = value;

    TextEditable(pw) = True;
    result = ReplaceText( pw, NULL, from_pos, to_pos, wcs, wslen(wcs) );
    TextEditable(pw) = save_editable;
    if( value == NULL )
	XtFree( (char *)wcs );
    return( result );
}


void
#if NeedFunctionPrototypes
XmwPreeditInsert( Widget w, Position position, char *value )
#else /* NeedFunctionPrototypes */
XmwPreeditInsert( w, position, value )
    Widget	w;
    Position	position;
    char	*value;
#endif /* NeedFunctionPrototypes */
{
    XmwPreeditReplace( w, position, position, value );
}


void
#if NeedFunctionPrototypes
XmwPreeditInsertWcs( Widget w, Position position, wchar_t *value )
#else /* NeedFunctionPrototypes */
XmwPreeditInsertWcs( w, position, value )
    Widget	w;
    Position	position;
    wchar_t	*value;
#endif /* NeedFunctionPrototypes */
{
    XmwPreeditReplaceWcs( w, position, position, value );
}


void
#if NeedFunctionPrototypes
XmwPreeditSetEditable( Widget w, Boolean editable )
#else /* NeedFunctionPrototypes */
XmwPreeditSetEditable( w, editable )
    Widget	w;
    Boolean	editable;
#endif /* NeedFunctionPrototypes */
{
    XmwPreeditWidget	pw = (XmwPreeditWidget)w;

    TextEditable(pw) = editable;
}


void
#if NeedFunctionPrototypes
XmwPreeditMoveWidget( Widget w, Position position, Widget sh )
#else /* NeedFunctionPrototypes */
XmwPreeditMoveWidget( w, position, sh )
    Widget	w;
    Position	position;
    Widget	sh;
#endif /* NeedFunctionPrototypes */
{
    XmwPreeditWidget	pw = (XmwPreeditWidget)w;
    Window		win;
    Position		x, y, newx, newy;
    int			xx, yy;
    unsigned int	width, height, border, depth;
    Display		*display = XtDisplay(w);
    int			screen = DefaultScreen(display);
    Window		root, child;
    XSizeHints		hints;
    long		supplied;
    Dimension		wi, he;

    XGetWMNormalHints( XtDisplay(sh), XtWindow(sh), &hints, &supplied );
    XtVaGetValues( sh, XtNwidth, &wi, XtNheight, &he, NULL );
    hints.flags |= PWinGravity;

    GetWXYFromPos( pw, position, &win, &x, &y );
    y += pw->preedit.descent;
    XGetGeometry( display, win, &root, &xx, &yy, &width, &height,
		  &border, &depth );
    XTranslateCoordinates( display, win, root, (int)x, (int)y,
			   &xx, &yy, &child );
    newx = xx;
    newy = yy;
    hints.win_gravity = NorthWestGravity;

    width = DisplayWidth( display, screen );
    height = DisplayHeight( display, screen );
    if( xx + wi > width ) {
	newx -= xx + wi - width;
	hints.win_gravity = NorthEastGravity;
    }
    if( yy + he > height ) {
	newy -= pw->preedit.descent + pw->preedit.ascent + he;
	hints.win_gravity = (hints.win_gravity == NorthWestGravity) ?
					  SouthWestGravity : SouthEastGravity;
    }
    XSetWMNormalHints( XtDisplay(sh), XtWindow(sh), &hints );
    XtMoveWidget( sh, newx, newy );
    XtMapWidget( sh );
}


void
#if NeedFunctionPrototypes
XmwPreeditSetString( Widget w, char *value, XmwHighlightMode mode )
#else /* NeedFunctionPrototypes */
XmwPreeditSetString( w, value, mode )
    Widget		w;
    char		*value;
    XmwHighlightMode	mode;
#endif /* NeedFunctionPrototypes */
{
    XmwPreeditWidget	pw = (XmwPreeditWidget)w;

    ChangeBlinkBehavior( pw, False );

    if( value == NULL )
	value = "";
    ValidateString( pw, value );
    CalculateLine( pw, 0 );
    pw->preedit.top_line = 0;
    pw->preedit.highlight.number = 1;
    pw->preedit.highlight.list[0].position = 0;
    pw->preedit.highlight.list[0].mode = mode;

    XmwPreeditSetCursorPosition( (Widget)pw, pw->preedit.string_length );
    RedisplayText( pw, 0, pw->preedit.string_length );
    if( pw->core.sensitive  &&  pw->preedit.has_focus  &&
       pw->preedit.display_caret )
        ChangeBlinkBehavior( pw, True );
}


void
#if NeedFunctionPrototypes
XmwPreeditSetStringWcs( Widget w, wchar_t *value, XmwHighlightMode mode )
#else /* NeedFunctionPrototypes */
XmwPreeditSetStringWcs( w, value, mode )
    Widget		w;
    wchar_t		*value;
    XmwHighlightMode	mode;
#endif /* NeedFunctionPrototypes */
{
    XmwPreeditWidget	pw = (XmwPreeditWidget)w;

    ChangeBlinkBehavior( pw, False );

    XtFree( (char *)TextValueWcs(pw) );
    if( value == NULL ) {
	pw->preedit.string_length = 0;
	pw->preedit.alloc_size = sizeof(wchar_t) * ALLOC_SIZE;
	TextValueWcs(pw) = (wchar_t *)XtMalloc( pw->preedit.alloc_size );
	TextValueWcs(pw)[0] = '\0';
    }
    else {
	pw->preedit.string_length = wslen( value );
	pw->preedit.alloc_size = sizeof(wchar_t) *
				  ( pw->preedit.string_length + ALLOC_SIZE );
	TextValueWcs(pw) =
	    wscpy( (wchar_t *)XtMalloc( pw->preedit.alloc_size ), value );
    }

    CalculateLine( pw, 0 );
    pw->preedit.highlight.number = 1;
    pw->preedit.highlight.list[0].position = 0;
    pw->preedit.highlight.list[0].mode = mode;

    XmwPreeditSetCursorPosition( w, pw->preedit.string_length );
    RedisplayText( pw, 0, pw->preedit.string_length );
    if( pw->core.sensitive  &&  pw->preedit.has_focus  &&
	pw->preedit.display_caret )
        ChangeBlinkBehavior( pw, True );
}


void
#if NeedFunctionPrototypes
XmwPreeditMapWidget( Widget w )
#else /* NeedFunctionPrototypes */
XmwPreeditMapWidget( w )
    Widget	w;
#endif /* NeedFunctionPrototypes */
{
    XmwPreeditWidget	pw = (XmwPreeditWidget)w;

    pw->preedit.top_line = 0;
    pw->preedit.max_lines = (pw->preedit.height -
			     (pw->core.y - pw->preedit.descent - pw->preedit.y))
			    / pw->preedit.line_spacing;
    if( pw->preedit.max_lines == 0 )
	pw->preedit.max_lines = 1;

    pw->core.width = pw->preedit.cursor_width;
    pw->core.sensitive = True;
    pw->preedit.display_caret = True;
    TextEditable(pw) = True;
    XmwPreeditSetString( w, "", XmwHIGHLIGHT_SECONDARY_SELECTED );

    pw->preedit.cursor_on = pw->preedit.blink_on = pw->preedit.dest_on = False;
    XMoveResizeWindow( XtDisplay(pw), pw->core.window,
		      pw->core.x, pw->core.y, pw->core.width, pw->core.height );

    XMapWindow( XtDisplay(pw), pw->core.window );
    ChangeBlinkBehavior( pw, True );
}


void
#if NeedFunctionPrototypes
XmwPreeditUnmapWidget( Widget w )
#else /* NeedFunctionPrototypes */
XmwPreeditUnmapWidget( w )
    Widget	w;
#endif /* NeedFunctionPrototypes */
{
    XmwPreeditWidget	pw = (XmwPreeditWidget)w;

    pw->core.sensitive = False;
    pw->preedit.has_focus = False;
    ChangeBlinkBehavior( pw, False );

    XUnmapWindow( XtDisplay(pw), pw->core.window );
    XUnmapWindow( XtDisplay(pw), pw->preedit.window2 );
    XUnmapWindow( XtDisplay(pw), pw->preedit.window3 );
}


void
#if NeedFunctionPrototypes
XmwPreeditAutoReplace( Widget w )
#else /* NeedFunctionPrototypes */
XmwPreeditAutoReplace( w )
    Widget	w;
#endif /* NeedFunctionPrototypes */
{
    XmwPreeditWidget	pw = (XmwPreeditWidget)w;

    if( pw->core.sensitive  &&  pw->preedit.has_focus )
        ChangeBlinkBehavior( pw, False );

    XUnmapWindow( XtDisplay(pw), pw->core.window );
    XUnmapWindow( XtDisplay(pw), pw->preedit.window2 );
    XUnmapWindow( XtDisplay(pw), pw->preedit.window3 );
    XSync( XtDisplay(pw), False );

    pw->core.width = pw->preedit.cursor_width;
    pw->core.x -= pw->preedit.x;
    pw->core.x += XwcTextEscapement( TextFontSet(pw), TextValueWcs(pw),
				     pw->preedit.string_length );
    pw->core.y += (pw->core.x / pw->preedit.width) * pw->preedit.line_spacing;
    pw->core.x %= pw->preedit.width;
    pw->core.x += pw->preedit.x;

    XmwPreeditSetString( w, "", XmwHIGHLIGHT_SECONDARY_SELECTED );

    XMoveResizeWindow( XtDisplay(pw), pw->core.window,
		       pw->core.x, pw->core.y,
		       pw->core.width, pw->core.height );
    XSync( XtDisplay(pw), False );

    XMapWindow( XtDisplay(pw), pw->core.window );
    if( pw->core.sensitive  &&  pw->preedit.has_focus  &&
       pw->preedit.display_caret )
        ChangeBlinkBehavior( pw, True );
}


#if NeedFunctionPrototypes
void	XmwPreeditSetCursorPosition( Widget w, Position position )
#else /* NeedFunctionPrototypes */
void	XmwPreeditSetCursorPosition( w, position )
    Widget	w;
    Position	position;
#endif /* NeedFunctionPrototypes */
{
    SetCursorPosition( (XmwPreeditWidget)w, position );
}

