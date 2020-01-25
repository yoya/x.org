/* $XConsortium: shapetest.c,v 1.5 94/04/17 20:46:03 gildea Exp $ */
/*

Copyright (c) 1989  X Consortium

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE X CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of the X Consortium shall
not be used in advertising or otherwise to promote the sale, use or
other dealings in this Software without prior written authorization
from the X Consortium.

*/

#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xproto.h>
#include <X11/Xutil.h>
#include <X11/extensions/shape.h>

Display *dpy;

StartConnectionToServer(argc, argv)
int     argc;
char    *argv[];
{
    char *display;

    display = NULL;
    for(--argc, ++argv; argc; --argc, ++argv)
    {
	if ((*argv)[0] == '-') {
	    switch((*argv)[1]) {
	    case 'd':
		display = argv[1];
		++argv; --argc;
		break;
	    }
	}
    }
    if (!(dpy = XOpenDisplay(display)))
    {
       perror("Cannot open display\n");
       exit(0);
   }
}

XRectangle  rects[] = { 0,0, 100, 100, 10, 10, 100, 100 };

main(argc, argv)
    int argc;
    char **argv;

{
	Window  w;
	GC gc;
	char *windowName = "Test of Shape Extension";
	XSetWindowAttributes xswa;
	unsigned long	mask;
	XEvent pe;
	XColor screen_def_blue, exact_def_blue;
	XColor screen_def_red, exact_def_red;

	/*_Xdebug = 1;*/   /* turn on synchronization */

	StartConnectionToServer(argc, argv);

	xswa.event_mask = ExposureMask;
	xswa.background_pixel = BlackPixel (dpy, DefaultScreen (dpy));
	mask = CWEventMask | CWBackPixel;
	w = XCreateWindow(dpy, RootWindow(dpy, DefaultScreen(dpy)),
		100, 100, 340, 340, 0, 
		CopyFromParent, CopyFromParent,	CopyFromParent,
		mask, &xswa);

	XChangeProperty(dpy,
	    w, XA_WM_NAME, XA_STRING, 8, PropModeReplace,
	    (unsigned char *)windowName, strlen(windowName));

	XShapeCombineRectangles (dpy, w, ShapeBounding, 0, 0, 
		          rects, sizeof (rects) / sizeof (rects[0]),
			  ShapeSet, Unsorted);

	XMapWindow(dpy, w);

	gc = XCreateGC(dpy, w, 0, 0);
	XAllocNamedColor(dpy, DefaultColormap(dpy, DefaultScreen(dpy)), "blue",
	       &screen_def_blue,  &exact_def_blue);
	XAllocNamedColor(dpy, DefaultColormap(dpy, DefaultScreen(dpy)), "red",
	       &screen_def_red,  &exact_def_red);
	XSetForeground(dpy, gc, screen_def_red.pixel);
	XSetBackground(dpy, gc, screen_def_blue.pixel);

	while (1) {
	    XNextEvent(dpy, &pe);
	}
}
