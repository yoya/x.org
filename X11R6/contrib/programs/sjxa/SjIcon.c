/* $SonyId: SjIcon.c,v 1.1 1994/05/30 13:03:58 makoto Exp $ */
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
#include	<X11/Shell.h>
#include	<Xmw/Xmw.h>
#include	"sjxa.h"
#include	"bitmaps/sjxa.xbm"

#define	ICON_SIZE	50

typedef enum {
    XmwICON_PIXMAP,
    XmwICON_WINDOW
} XmwIconMode;

static Pixmap		icon;

static XmwIconMode	icon_mode = XmwICON_WINDOW;
unsigned long		default_foreground, default_background;
static Pixmap		pixmap = None;
static GC		gc;
static Colormap		colormap;
static Window		icon_window;

extern Widget		TopLevel;


static void	GetIconColor( cm, pixel )
Colormap	cm;
Pixel		*pixel;
{
    XColor	xcolor;

    if( cm == None ) {
	*pixel = XmwICON_DEFAULT_COLOR;
	return;
    }
    xcolor.pixel = *pixel;
    XQueryColor( XtDisplay(TopLevel), cm, &xcolor );
    if( XAllocColor( XtDisplay(TopLevel), colormap, &xcolor ) )
	*pixel = xcolor.pixel;
    else
	*pixel = XmwICON_DEFAULT_COLOR;
}


void	MakeIconPixmap( w, sub )
Widget	w, sub;
{
    Display		*dsp = XtDisplay( w );
    Window		win;
    XIconSize		*size;
    int			count, scope, width_diff, height_diff;
    int			ii, jj, kk;
    Dimension		width, height;

    width = height = ICON_SIZE;
    if( XGetIconSizes( dsp, DefaultRootWindow( dsp ), &size, &count ) ) {
	width_diff = height_diff = ICON_SIZE;
	for( ii = 0, scope = count; ii < count; ii++ ) {
	    if( width >= size[ii].min_width  &&
	       width <= size[ii].max_width  &&
	       height >= size[ii].min_height  &&
	       height <= size[ii].max_height ) {
		jj = (width - size[ii].min_width) % size[ii].width_inc;
		kk = (height - size[ii].min_height) % size[ii].height_inc;
		if( jj == 0  &&  kk == 0 )
		    break;
		if( ((width - jj) * (height - kk)) > ((width - width_diff) * (height - height_diff)) ) {
		    width_diff = jj;
		    height_diff = kk;
		    scope = count;
		}
	    }
	}
	if( ii == count ) {
	    if( scope != count ) {
		width -= width_diff;
		height -= height_diff;
	    }
	    else {
		icon_mode = XmwICON_PIXMAP;
	    }
	}
	XFree( size );
    }

    win = XtWindow( w );
    icon = XCreateBitmapFromData( dsp, win, sjxa_bits, sjxa_width,
				  sjxa_height );

    XtVaGetValues( sub, XtNforeground, &default_foreground,
		   XtNbackground, &default_background,
		   XtNcolormap, &colormap, NULL );
    if( icon_mode == XmwICON_WINDOW ) {
	XWindowAttributes	swa;
	Colormap		cmap;
	unsigned int		depth = DefaultDepthOfScreen(XtScreen(w));

	pixmap = XCreatePixmap( dsp, win, width, height, depth );
	win = XCreateSimpleWindow( dsp, DefaultRootWindow( dsp ), 0, 0,
		  width, height, 0, default_foreground, default_background );
	XSelectInput( dsp, win, StructureNotifyMask );
	XtVaSetValues( w, XtNiconWindow, win, NULL );

	cmap = colormap;
	XGetWindowAttributes( dsp, win, &swa );
	colormap = swa.colormap;
	GetIconColor( cmap, &default_foreground );
	GetIconColor( cmap, &default_background );

	gc = XCreateGC( dsp, win, 0L, NULL );
	XSetForeground( dsp, gc, default_foreground );
	XSetBackground( dsp, gc, default_background );
	XCopyPlane( dsp, icon, pixmap, gc, 0, 0,
		    ICON_SIZE, ICON_SIZE, 0, 0, 1 );
	XSetWindowBackgroundPixmap( dsp, win, pixmap );
	XClearWindow( dsp, win );
    }
    else
	XtVaSetValues( w, XtNiconPixmap, icon, NULL );
    XtVaGetValues( TopLevel, XtNiconWindow, &icon_window, NULL );
}


Boolean	IconFilterEvent( event )
XEvent	*event;
{
    Display	*display = event->xany.display;

    if( event->xany.window != icon_window )
	return( False );
    switch( event->type ) {
      case MapNotify:
	if( icon_mode == XmwICON_WINDOW ) {
	    XSetForeground( display, gc, default_foreground );
	    XSetBackground( display, gc, default_background );

	    if( pixmap != None ) {
		if( icon_window != None ) {
		    XCopyPlane( display, icon, pixmap, gc, 0, 0,
				ICON_SIZE, ICON_SIZE, 0, 0, 1 );
		    XSetWindowBackgroundPixmap( display, icon_window, pixmap );
		    XClearWindow( display, icon_window );
		}
	    }
	}
	break;
    }
    return( True );
}
