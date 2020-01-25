/*
 * $XConsortium: xmbufinfo.c,v 1.4 94/04/17 20:46:05 rws Exp $
 *
Copyright (c) 1989  X Consortium

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
 */

#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xos.h>
#include <X11/extensions/multibuf.h>

char *ProgramName;

static void usage ()
{
    fprintf (stderr, "usage:  %s [-display dpy]\n", ProgramName);
    exit (1);
}

main (argc, argv)
    int argc;
    char **argv;
{
    char *displayname = NULL;
    Display *dpy;
    int i;    
    int event_base, error_base;

    ProgramName = argv[0];
    for (i = 1; i < argc; i++) {
	char *arg = argv[i];

	if (arg[0] == '-') {
	    switch (arg[1]) {
	      case 'd':			/* -display dpy */
		if (++i >= argc) usage ();
		displayname = argv[i];
		continue;
	    }
	}
	usage ();
    }

    dpy = XOpenDisplay (displayname);
    if (!dpy) {
	fprintf (stderr, "%s:  unable to open display \"%s\"\n",
		 ProgramName, XDisplayName(displayname));
	exit (1);
    }

    if (!XmbufQueryExtension (dpy, &event_base, &error_base)) {
	fprintf (stderr, 
	 "%s:  multibuffering extension not supported on server \"%s\"\n",
		 ProgramName, DisplayString(dpy));
	XCloseDisplay(dpy);
	exit (1);
    }

    printf ("Multibuffer information for server \"%s\":\n",
	    DisplayString(dpy));
    printf ("  First event number:  %d\n", event_base);
    printf ("  First error number:  %d\n", error_base);

    for (i = 0; i < ScreenCount(dpy); i++) {
	int j;
	int nmono, nstereo;
	XmbufBufferInfo *mono_info = NULL, *stereo_info = NULL;
	
	if (!XmbufGetScreenInfo (dpy, RootWindow (dpy, i), &nmono, &mono_info,
				 &nstereo, &stereo_info)) {
	    fprintf (stderr,
		     "%s:  unable to get multibuffer info for screen %d\n",
		     ProgramName, i);
	    continue;
	}

	printf ("  Screen %d:\n", i);
	printf ("    Number of mono types:  %d\n", nmono);
	for (j = 0; j < nmono; j++) {
	    printf ("      Visual id, max buffers, depth:  0x%lx, %d, %d\n",
		    mono_info[j].visualid, mono_info[j].max_buffers,
		    mono_info[j].depth);
	}
	printf ("    Number of stereo types:  %d\n", nstereo);
	for (j = 0; j < nstereo; j++) {
	    printf ("      Visual id, max buffers, depth:  0x%lx, %d, %d\n",
		   stereo_info[j].visualid, stereo_info[j].max_buffers,
		   stereo_info[j].depth);
	}
	if (mono_info) XFree ((char *) mono_info);
	if (stereo_info) XFree ((char *) stereo_info);
    }
    XCloseDisplay (dpy);
    exit (0);
}
