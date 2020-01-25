/* $XConsortium: shmtest.c,v 1.6 94/04/17 20:46:03 rws Exp $ */
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
#ifdef dirty
#undef dirty
#endif
#include <X11/Xlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/extensions/XShm.h>

main(argc, argv)
    int argc;
    char **argv;
{
    char *display;
    Display *dpy;
    int screen;
    XGCValues gcv;
    GC gc;
    XEvent event;
    register XExposeEvent *expose = (XExposeEvent *)&event;
    register XMotionEvent *motion = (XMotionEvent *)&event;
    int x, y, curx, cury, maxx, maxy;
    XImage *image;
    XShmSegmentInfo shminfo;
    Window putwin, pixwin;
    Pixmap pix;
    int i;
    char c;
    int majorv, minorv;
    Bool sharedPixmaps;
    int Completion;
    Bool ok, update;
    int busy;

    display = NULL;
    for(--argc, ++argv; argc; --argc, ++argv) {
	if ((*argv)[0] == '-') {
	    switch((*argv)[1]) {
	    case 'd':
		display = argv[1];
		++argv; --argc;
		break;
	    }
	}
    }
    dpy = XOpenDisplay(display);
    if (!dpy)
	exit(1);
    if (!XShmQueryVersion(dpy, &majorv, &minorv, &sharedPixmaps))
	exit(1);
    Completion = XShmGetEventBase(dpy) + ShmCompletion;
    screen = DefaultScreen(dpy);
    putwin = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), 0, 0, 200, 200,
				 2, BlackPixel(dpy, screen),
				 BlackPixel(dpy, screen));
    XStoreName(dpy, putwin, "ShmPutImage");
    XSelectInput(dpy, putwin,
		 ButtonPressMask|ButtonReleaseMask|
		 ButtonMotionMask|KeyPressMask|ExposureMask);
    if (sharedPixmaps)
    {
	pixwin = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), 0, 0,
				     200, 200,
				     2, BlackPixel(dpy, screen),
				     BlackPixel(dpy, screen));
	XStoreName(dpy, pixwin, "ShmCopyArea");
	XSelectInput(dpy, pixwin,
		     ButtonPressMask|ButtonReleaseMask|
		     ButtonMotionMask|KeyPressMask|ExposureMask);
    }
    image = XShmCreateImage(dpy, 0, DisplayPlanes(dpy, screen), ZPixmap,
			    0, &shminfo, 200, 200);
    shminfo.shmid = shmget(IPC_PRIVATE,
			   image->bytes_per_line * image->height,
			   IPC_CREAT|0777);
    if (shminfo.shmid < 0)
    {
	perror("shmget");
	exit(1);
    }
    shminfo.shmaddr = (char *)shmat(shminfo.shmid, 0, 0);
    if (shminfo.shmaddr == ((char *)-1))
    {
	perror("shmat");
	exit(1);
    }
    image->data = shminfo.shmaddr;
    shminfo.readOnly = False;
    XShmAttach(dpy, &shminfo);
    if (sharedPixmaps)
	pix = XShmCreatePixmap(dpy, DefaultRootWindow(dpy), shminfo.shmaddr,
			       &shminfo, image->width, image->height,
			       image->depth);
    curx = cury = x = y = 0;
    maxx = DisplayWidth(dpy, screen) - image->width;
    maxy = DisplayHeight(dpy, screen) - image->height;
    XShmGetImage(dpy, DefaultRootWindow(dpy), image, x, y, AllPlanes);
    XMapWindow(dpy, putwin);
    if (sharedPixmaps)
	XMapWindow(dpy, pixwin);
    gcv.graphics_exposures = False;
    gc = XCreateGC(dpy, putwin, GCGraphicsExposures, &gcv);
    ok = update = False;
    busy = 0;
    while (1) {
	XNextEvent(dpy, &event);
	switch(event.type) {
	case ButtonPress:
	    ok = True;
	case MotionNotify:
	    x = motion->x_root;
	    y = motion->y_root;
	    if (x > maxx)
		x = maxx;
	    if (y > maxy)
		y = maxy;
	    if (XEventsQueued(dpy, QueuedAfterReading))
	    {
		XPeekEvent(dpy, &event);
		if (event.xany.type == MotionNotify)
		    break;
	    }
	    update = ok;
	    break;
	case ButtonRelease:
	    ok = False;
	    break;
	case KeyPress:
	    i = XLookupString(&event, &c, 1, NULL, NULL);
	    if ((i == 1) && ((c == 'q') || (c == 'Q') || (c == '\03'))) {
		XCloseDisplay(dpy);
		return;
	    }
	    break;
	case Expose:
	    if ((expose->x < image->width) &&
		(expose->y < image->height)) {
		if ((image->width - expose->x) < expose->width)
		    expose->width = image->width - expose->x;
		if ((image->height - expose->y) < expose->height)
		    expose->height = image->height - expose->y;
		if (expose->window == putwin) {
		    XShmPutImage(dpy, expose->window, gc, image,
				 expose->x, expose->y, expose->x, expose->y,
				 expose->width, expose->height, True);
		    busy++;
		}
		else if (sharedPixmaps && (expose->window == pixwin))
		    XCopyArea(dpy, pix, expose->window, gc,
			      expose->x, expose->y,
			      expose->width, expose->height,
			      expose->x, expose->y);
	    }
	    break;
	default:
	    if (event.xany.type == Completion)
		busy--;
	}
	if (update && !busy && ((curx != x) || (cury != y))) {
	    update = False;
	    curx = x;
	    cury = y;
	    XShmGetImage(dpy, DefaultRootWindow(dpy), image,
			 x, y, AllPlanes);
	    XShmPutImage(dpy, putwin, gc, image, 0, 0, 0, 0,
			 image->width, image->height, True);
	    busy++;
	    if (sharedPixmaps)
		XCopyArea(dpy, pix, pixwin, gc, 0, 0,
			  image->width, image->height, 0, 0);
	}
    }
}
