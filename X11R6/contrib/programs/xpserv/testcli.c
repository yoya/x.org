/*
Copyright (c) 1994  Matthew Reynolds and Qualcomm Inc.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
QUALCOMM INC., OR MATTHEW REYNOLDS BE LIABLE FOR ANY CLAIM, 
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE 
OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of the Qualcomm Inc., or Matthew 
Reynolds shall not be used in advertising or otherwise to promote the sale, 
use or other dealings in this Software without their prior written 
authorization.

*/

#include <stdio.h>
#include <math.h>
#include <values.h>
#include <stdlib.h>
#include <X11/Xlib.h>

void main( int argc, char *argv[]) 
{
    Display			*pdisplay, *display;
    GC				gc, gc2, gc3;
    Window			window, window2;
    int				i, x, y, screen, j, index;
    XColor			color, exact;
    XGCValues			gcv;
    Colormap			cmap;
    XPoint			points[ 20];
    XEvent			event;
    XSetWindowAttributes	attr;
    Font			font;
    XImage			*image;
    XColor			vector[ 32];
    Pixmap			pixmap;

    if ( argc < 2) {
	display = XOpenDisplay( "unix:0.0");
    } else {
	display = XOpenDisplay( argv[ 1]);
    }
    if ( display == NULL) {
	fprintf( stderr, "Graphics XOpenDisplay failed\n");
	exit( -1);
    } else {
	fprintf( stderr, "Graphics XOpenDisplay succeeded\n");
    }
    pdisplay = XOpenDisplay( getenv( "PRINTSERVER"));
    if ( pdisplay == NULL) {
	fprintf( stderr, "Printer XOpenDisplay failed\n");
	exit( -1);
    } else {
	fprintf( stderr, "Printer XOpenDisplay succeeded\n");
    }
    cmap = DefaultColormap( pdisplay, DefaultScreen( pdisplay));
    screen = DefaultScreen( pdisplay);
    window = XCreateSimpleWindow( pdisplay,
				  DefaultRootWindow( pdisplay),
				  10, 10, 600, 1200, 1, 1, 0);
    XSetPrintParams( pdisplay, False, 2, 2, 612, 792);
    XOpenPrintWindow( pdisplay, window, "printfile");
    color.red = 255 << 8;
    color.green = 0;
    color.blue = 0;
    XAllocColor( pdisplay, cmap, &color);
    font = XLoadFont( pdisplay, 
	    "-adobe-courier-bold-o-normal--14-140-75-75-m-90-iso8859-1");
    gcv.foreground = color.pixel;
    gcv.font = font;
    gc = XCreateGC( pdisplay, window, GCForeground | GCFont, &gcv);
    XDrawLine( pdisplay, window, gc, 10, 10, 20, 20);
    XDrawPoint( pdisplay, window, gc, 20, 20);
    XDrawString( pdisplay, window, gc, 20, 20, "this is a test", 14);
    XDrawString( pdisplay, window, gc, 20, 100, "this is a test", 14);
    XDrawString( pdisplay, window, gc, 20, 200, "this is a test", 14);
    XDrawImageString( pdisplay, window, gc, 20, 300, "this is a test", 14);
    for ( i = 0; i < 100; i++) {
	x = 100+100*cos(( i*M_PI)/50); 
	y = 100+100*sin(( i*M_PI)/50);
	XDrawLine( pdisplay, window, gc, 100, 100, x, y);
    }
    XDrawArc( pdisplay, window, gc, 0, 0, 200, 200, 0, 360*64);
    XAllocNamedColor( pdisplay, cmap, "Blue", &color, &exact);
    gcv.foreground = color.pixel;
    gcv.line_width = 4;
    gc2 = XCreateGC( pdisplay, window , GCForeground, &gcv);
    XDrawRectangle( pdisplay, window, gc2, 200, 200, 300, 200);
    XFillRectangle( pdisplay, window, gc2, 100, 100, 200, 100);
    points[ 0].x = 310;
    points[ 0].y = 320;
    points[ 1].x = 410;
    points[ 1].y = 220;
    points[ 2].x = 250;
    points[ 2].y = 320;
    points[ 3].x = 300;
    points[ 3].y = 210;
    points[ 4] = points[ 0];
    XFillPolygon( pdisplay, window, gc2, points, 5, Complex, CoordModeOrigin);
    XDrawLines( pdisplay, window, gc2, points, 5, CoordModeOrigin);
    XCopyArea( pdisplay, window, window, gc2, 0, 0, 300, 300, 0, 300);
    for ( i = 0; i < 32; i++) {
	vector[ i].red = i << 11;
	vector[ i].blue = i << 11;
	vector[ i].green = i << 11;
	XAllocColor( pdisplay, cmap, &vector[ i]);
    }
    XStoreColors( pdisplay, cmap, vector, 32);
    image = XCreateImage( pdisplay, DefaultVisual( pdisplay, screen),
			  DefaultDepth( pdisplay, screen),
			  ZPixmap, 0, NULL, 128, 128, 8, 0);
    image->data = ( char *) malloc( image->height*image->bytes_per_line);
    for ( i = 0; i < image->height; i++) {
	for ( j = 0; j < image->bytes_per_line; j++) {
	    index = 16 + 16*sin((( double) i)/12)*cos((( double) j)/12);
	    if ( index < 0) {
		index = 0;
	    } else if ( index >= 32) {
		index = 31;
	    }
	    image->data[ i*image->bytes_per_line + j] = vector[ index].pixel;
	}
    }
    XPutImage( pdisplay, window, gc, image, 0, 0, 64, 64, 128, 128);
#if 0
    for (i = 0; i < 500; i++) {
	y = 100+ 100*sin( i/100.0);
	XDrawPoint( pdisplay, window, gc, i + 20, y);
    }
#endif

/* create a pixmap and copy it from the real display to the print display */

    pixmap = XCreatePixmap( display, DefaultRootWindow( display), 128, 128,
			    DefaultDepth( display, DefaultScreen( display)));
    gc3 = XCreateGC( display, DefaultRootWindow( display), 0, NULL);
    XPutImage( display, pixmap, gc3, image, 0, 0, 0, 0, 128, 128);
    XCopyArea( pdisplay, pixmap, window, gc2, 0, 0, 128, 128, 200, 300);
    XClosePrintWindow( pdisplay, window);
    XCloseDisplay( pdisplay);
    window = XCreateSimpleWindow( display, DefaultRootWindow( display),
				  10, 10, 400, 200, 1, 1, 0);
    attr.event_mask = ButtonPressMask | ExposureMask;
    XChangeWindowAttributes( display, window, CWEventMask, &attr);
    XMapWindow( display, window);
    do {
	XNextEvent( display, &event);
	if ( event.type == ButtonPress) {
	    XCloseDisplay( display);
	    exit( 0);
	}
    } while ( 1);
}
