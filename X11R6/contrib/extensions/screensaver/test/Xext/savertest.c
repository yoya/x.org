/*
 * $XConsortium: savertest.c,v 1.5 94/04/17 20:59:39 eswu Exp $
 *
Copyright (c) 1992  X Consortium

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
X CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of the X Consortium shall not be
used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization from the X Consortium.
 *
 * Author:  Keith Packard, MIT X Consortium
 */

#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xproto.h>
#include <X11/Xutil.h>
#include <X11/extensions/scrnsaver.h>

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

fatal(s)
{
    printf ("fatal: %s\n", s);
    exit (1);
}

int ignoreError (dpy, error)
    Display *dpy;
    XErrorEvent	*error;
{
    printf ("ignoring error\n");
}

char *stateNames[] = { "Off", "On", "Cycle", "Disable" };
char *kindNames[] = { "Blanked", "Internal", "External" };

main(argc, argv)
    int argc;
    char **argv;

{
    XEvent pe;
    XScreenSaverNotifyEvent *se;
    Window root, saver;
    int	screen;
    int majorVersion, minorVersion;
    int error_base, event_base;
    XScreenSaverInfo	*info;
    XSetWindowAttributes    attr;
    unsigned long	mask;
    GC			gc;

    /*_Xdebug = 1;*/   /* turn on synchronization */

    StartConnectionToServer(argc, argv);

    if (!XScreenSaverQueryExtension (dpy, &event_base, &error_base))
	fatal ("QueryExtension failed");

    printf ("event_base %d, error_base %d\n", event_base, error_base);

    if (!XScreenSaverQueryVersion (dpy, &majorVersion, &minorVersion))
	fatal ("QueryVersion failed");

    printf ("majorVersion: %d, minorVersion: %d\n", majorVersion, minorVersion);
    
    root = DefaultRootWindow (dpy);

    info = XScreenSaverAllocInfo ();
    if (!XScreenSaverQueryInfo (dpy, root, info))
	fatal ("QueryInfo");
    printf ("window: 0x%x\n", info->window);
    printf ("state: %s\n", stateNames[info->state]);
    printf ("kind: %s\n", kindNames[info->kind]);
    printf ("til_or_since: %d\n", info->til_or_since);
    printf ("idle: %d\n", info->idle);
    printf ("eventMask: 0x%x\n", info->eventMask);
    saver = info->window;
    XFree (info);
    XScreenSaverSelectInput (dpy, root, ScreenSaverNotifyMask|ScreenSaverCycleMask);
    screen = DefaultScreen(dpy);
    attr.background_pixel = BlackPixel (dpy, screen);
    mask = CWBackPixel;
    XScreenSaverSetAttributes (dpy, root, 0, 0, 
	DisplayWidth(dpy, screen), DisplayHeight (dpy, screen), 0,
	CopyFromParent, CopyFromParent, CopyFromParent, mask, &attr);
    gc = DefaultGC (dpy, screen);
    XSetForeground (dpy, gc, WhitePixel(dpy, screen));
    while (1) {
	XNextEvent(dpy, &pe);
	if (pe.type == event_base)
	{
	    se = (XScreenSaverNotifyEvent *) &pe;
	    printf ("ScreenSaverNotifyEvent\n");
	    printf ("serial: %d\n", se->serial);
	    printf ("send_event: %d\n", se->send_event);
	    printf ("window: %x\n", se->window);
	    printf ("root: %x\n", se->root);
	    printf ("state: %s\n", stateNames[se->state]);
	    printf ("kind: %s\n", kindNames[se->kind]);
	    printf ("forced: %d\n", se->forced);
	    printf ("time: %d\n", se->time);
	    if (se->state == ScreenSaverOn && 
		se->kind == ScreenSaverExternal)
	    {
/*		XSetErrorHandler (ignoreError); */
		XDrawString (dpy, saver, gc, 100, 100, "Screen Saver Test", 17);
		XSync (dpy, False);
/*		XSetErrorHandler (NULL); */
	    }
	}
	else
	{
	    printf ("unexpected event %d\n", pe.type);
	}
    }
}
