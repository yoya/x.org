/************************************************************
Copyright 1987 by Sun Microsystems, Inc. Mountain View, CA.

                    All Rights Reserved

Permission  to  use,  copy,  modify,  and  distribute   this
software  and  its documentation for any purpose and without
fee is hereby granted, provided that the above copyright no-
tice  appear  in all copies and that both that copyright no-
tice and this permission notice appear in  supporting  docu-
mentation,  and  that the names of Sun or MIT not be used in
advertising or publicity pertaining to distribution  of  the
software  without specific prior written permission. Sun and
M.I.T. make no representations about the suitability of this
software for any purpose. It is provided "as is" without any
express or implied warranty.

SUN DISCLAIMS ALL WARRANTIES WITH REGARD TO  THIS  SOFTWARE,
INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FIT-
NESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL SUN BE  LI-
ABLE  FOR  ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,  DATA  OR
PROFITS,  WHETHER  IN  AN  ACTION OF CONTRACT, NEGLIGENCE OR
OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION  WITH
THE USE OR PERFORMANCE OF THIS SOFTWARE.

********************************************************/

/* $Header: xprkbd.c,v 1.4 88/02/09 11:24:24 jim Exp $ */
#include <stdio.h>
#include "X11/Xlib.h"
#include "X11/Xutil.h"
#include "X11/Xatom.h"

char *ProgramName;
Display *dpy;
int scr;

static void usage ()
{
    fprintf (stderr, "usage:  %s [-display host:dpy]\n\n", ProgramName);
    exit (1);
}


StartConnectionToServer(argc, argv)
int	argc;
char	*argv[];
{
    char *display;
    int i;

    display = NULL;
    for(i = 1; i < argc; i++)
    {
	if (strcmp (argv[i], "-display") == 0 || 
	    strcmp (argv[i], "-d") == 0) {
	    if (++i >= argc) usage ();
	    display = argv[i];
	    continue;
	} else 
	    usage ();
    }
    if (!(dpy = XOpenDisplay(display)))
    {
       perror("Cannot open display\n");
       exit(1);
   }
}


PrintKeyboardMapping()
{
    int         i;

    for (i = dpy->min_keycode; i <= dpy->max_keycode; i++) {
	int         j;
	/* Next line merely to ensure fields in dpy initialized */
	KeySym	junk = XKeycodeToKeysym(dpy, dpy->min_keycode, 0);

	printf("0x%x\t", i);
	for (j = 0; j < dpy->keysyms_per_keycode; j++) {
	    KeySym      ks = XKeycodeToKeysym(dpy, i, j);

	    if (ks != NoSymbol)
		printf("0x%x\t%s\t", ks, XKeysymToString(ks));
	}
	printf("\n");
    }
}

main(argc, argv)
    int argc;
    char **argv;
{
	ProgramName = argv[0];

	StartConnectionToServer(argc, argv);

	PrintKeyboardMapping();

	XCloseDisplay (dpy);
	exit (0);
}

