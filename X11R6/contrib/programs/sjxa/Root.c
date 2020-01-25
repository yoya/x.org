/* $SonyId: Root.c,v 1.1 1994/05/30 13:03:58 makoto Exp $ */
/******************************************************************

Copyright (c) 1992, 1993, 1994  Sony Corporation

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
#include	<X11/Intrinsic.h>
#include	<X11/StringDefs.h>
#include	<X11/Shell.h>
#include	<Xmw/TextOut.h>
#include	"sjxa.h"
#include	"resource.h"
#include	"SjBuffer.h"

#define	ON	1
#define	OFF	0

int		preedit_nothing_count = 0;
static int	status_nothing_count = 0;

extern Buffer	*buffer, root_window;
extern Widget	TopLevel, RootText, RootLabel, Box[], RootTextField;
extern Sjxa	resources;


void	RootWindowMap( b, style )
    Buffer		*b;
    unsigned long	 style;
{
    if( style & XIMPreeditNothing ) {
	preedit_nothing_count++;
	if( preedit_nothing_count == 1 ) {
	    buffer_initialize( &root_window );
	    XtVaSetValues( RootText, XmwNdisplayCaret, True, NULL );
	    XmwTextOutSetString( RootText, "", XmwHIGHLIGHT_NORMAL );
	}
    }

    if( style & XIMStatusNothing ) {
	status_nothing_count++;
	set_label( b, b->conversion_mode );
	XtMapWidget( Box[0] );
	XtMapWidget( Box[1] );
    }
    if( style == (XIMPreeditNothing | XIMStatusNothing)  ||
	resources.auto_raise )
	XMapRaised( XtDisplay(TopLevel), XtWindow(TopLevel) );
}


void	RootWindowUnmap( style )
unsigned long	style;
{
    int		state;

    if( style & XIMPreeditNothing ) {
	preedit_nothing_count--;
	if( preedit_nothing_count == 0 ) {
	    XmwTextOutSetString( RootText, NULL, XmwHIGHLIGHT_NORMAL );
	    XtVaSetValues( RootText, XmwNdisplayCaret, False, NULL );
	}
    }
    if( style & XIMStatusNothing ) {
	status_nothing_count--;
	if( status_nothing_count == 0 ) {
	    if( buffer->state & (INTERMEDIATE | CONVERSION) ) {
		XtUnmapWidget( Box[0] );
		XtUnmapWidget( Box[1] );
	    }
	    else if( buffer->state & SELECT ) {
		XmwTextOutSetString( RootTextField, NULL, XmwHIGHLIGHT_NORMAL );
		XtUnmapWidget( RootTextField );
		XtMapWidget( RootLabel );
	    }
	    XtVaSetValues( RootLabel, XtNlabel, resources.sjxa, NULL );
	}
    }

    if( preedit_nothing_count == 0  &&  status_nothing_count == 0 ) {
	XtVaGetValues( TopLevel, XtNinitialState, &state, NULL );
	if( state == IconicState )
	    XIconifyWindow( XtDisplay(TopLevel), XtWindow(TopLevel),
			    DefaultScreen(XtDisplay(TopLevel)) );
    }
}


void	MoveWidget( w )
Widget	w;
{
    int			x, y;
    Position		newx, newy;
    unsigned int	width, height;
    Display		*display = XtDisplay(w);
    int			screen = DefaultScreen(display);
    Window		child;
    XSizeHints		hints;
    long		supplied;
    Dimension		theight, pwidth, pheight;

    XtVaGetValues( TopLevel, XtNheight, &theight, NULL );
    XGetWMNormalHints( XtDisplay(w), XtWindow(w), &hints, &supplied );
    XtVaGetValues( w, XtNwidth, &pwidth, XtNheight, &pheight, NULL );
    hints.flags |= PWinGravity;

    XTranslateCoordinates( display, XtWindow(TopLevel),
			   DefaultRootWindow(display), 0, (int)theight,
			   &x, &y, &child );
    newx = x;
    newy = y;
    hints.win_gravity = NorthWestGravity;

    width = DisplayWidth( display, screen );
    height = DisplayHeight( display, screen );
    if( x + pwidth > width ) {
	newx -= x + pwidth - width;
	hints.win_gravity = NorthEastGravity;
    }
    if( y + pheight > height ) {
	newy -= pheight + theight;
	hints.win_gravity = (hints.win_gravity == NorthWestGravity) ?
					  SouthWestGravity : SouthEastGravity;
    }
    XSetWMNormalHints( display, XtWindow(w), &hints );
    XtMoveWidget( w, newx, newy );
    XtMapWidget( w );
}
