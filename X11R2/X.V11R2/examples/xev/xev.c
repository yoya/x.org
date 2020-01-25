/*
 * This is a simple program to determine keycode and keysym names
 * 
 * Compile and link with:
 * 
 *                 cc -o xev xev.c -lX11
 */

#include <stdio.h>
#include <X11/Xos.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#define XK_LATIN1
#include <X11/keysymdef.h>

typedef unsigned long Pixel;

char *ProgramName;
Display *dpy;
int screen;

usage ()
{
    fprintf (stderr, 
	     "usage:  %s [-display host:dpy] [-geometry geom]\n", ProgramName);
    exit (1);
}

main (argc, argv)
    int argc;
    char **argv;
{
    char *displayname = NULL;
    char *geom = NULL;
    int i;
    XSizeHints hints;
    Window w;
    Pixel fore, back, border;
    int done;

    ProgramName = argv[0];
    for (i = 1; i < argc; i++) {
	char *arg = argv[i];

	if (arg[0] == '-') {
	    switch (arg[1]) {
	      case 'd':			/* -display host:dpy */
		if (++i >= argc) usage ();
		displayname = argv[i];
		continue;
	      case 'g':			/* -geometry geom */
		if (++i >= argc) usage ();
		geom = argv[i];
		continue;
	      default:
		usage ();
		/* doesn't return */
	    }
	} else 
	  usage ();
    }

    dpy = XOpenDisplay (displayname);
    if (!dpy) {
	fprintf (stderr, "%s:  unable to open display '%s'\n",
		 ProgramName, XDisplayName (displayname));
	exit (1);
    }

    set_sizehints (&hints, 10, 10, 400, 200, 100, 100, geom);

    screen = DefaultScreen (dpy);
    fore = WhitePixel (dpy, screen);
    back = BlackPixel (dpy, screen);
    border = WhitePixel (dpy, screen);

    w = XCreateSimpleWindow (dpy, RootWindow (dpy, screen), hints.x, hints.y,
			     hints.width, hints.height, 2, border, back);

    XSetStandardProperties (dpy, w, "Event Tester", NULL, (Pixmap) 0,
			    argv, argc, &hints);

    XSelectInput (dpy, w, (KeyPressMask | KeyReleaseMask |
			   ButtonPressMask | ButtonReleaseMask));
    XMapWindow (dpy, w);

    for (done = 0; !done; ) {
	XEvent event;
	XKeyEvent *kep;
	XButtonEvent *bep;
	XMappingEvent *mep;
	KeySym ks;
	char *ksname;

	XNextEvent (dpy, &event);

	switch (event.type) {
	  case KeyPress:
	    kep = (XKeyEvent *) &event;
	    printf ("%s\n\t(%d,%d) root:(%d,%d), state 0x%x, ",
		    "KeyPress",
		    kep->x, kep->y, kep->x_root, kep->y_root,
		    kep->state);
	    printf ("keycode %d = 0x%x,\n", kep->keycode, kep->keycode);
	    ks = XLookupKeysym (kep, 0);
	    ksname = XKeysymToString (ks);
	    printf ("\tkeysym %d = 0x%x (%s)\n",
		    ks, ks, ksname ? ksname : "?");
	    if (ks == XK_q) done = 1;
	    continue;
	  case KeyRelease:
	    kep = (XKeyEvent *) &event;
	    printf ("%s\n\t(%d,%d) root:(%d,%d), state 0x%x, ",
		    "KeyRelease",
		    kep->x, kep->y, kep->x_root, kep->y_root,
		    kep->state);
	    printf ("keycode %d = 0x%x,\n", kep->keycode, kep->keycode);
	    ks = XLookupKeysym (kep, 0);
	    ksname = XKeysymToString (ks);
	    printf ("\tkeysym %d = 0x%x (%s)\n",
		    ks, ks, ksname ? ksname : "?");
	    continue;
	  case ButtonPress:
	    bep = (XButtonEvent *) &event;
	    printf ("%s\n\t(%d,%d) root:(%d,%d), state 0x%x, ",
		    "ButtonPress",
		    bep->x, bep->y, bep->x_root, bep->y_root,
		    bep->state);
	    printf ("button %d = 0x%x\n", bep->button, bep->button);

	    continue;
	  case ButtonRelease:
	    bep = (XButtonEvent *) &event;
	    printf ("%s\n\t(%d,%d) root:(%d,%d), state 0x%x, ",
		    "ButtonRelease",
		    bep->x, bep->y, bep->x_root, bep->y_root,
		    bep->state);
	    printf ("button %d = 0x%x\n", bep->button, bep->button);
	    continue;
	  case MappingNotify:
	    mep = (XMappingEvent *) &event;
	    printf ("%s\n\trequest %d, first_keycode %d, count %d\n",
		    "MappingNotify", mep->request, mep->first_keycode,
		    mep->count);
	    XRefreshKeyboardMapping (&event);
	    continue;
	  default:
	    printf ("Unhandled event type %d\n", event.type);
	    continue;
	}
    }

    XCloseDisplay (dpy);
    exit (0);
}

set_sizehints (hintp, min_width, min_height,
	       defwidth, defheight, defx, defy, geom)
    XSizeHints *hintp;
    int min_width, min_height, defwidth, defheight, defx, defy;
    char *geom;
{
    int geom_result;

    /* set the size hints, algorithm from xlib xbiff */

    hintp->width = hintp->min_width = min_width;
    hintp->height = hintp->min_height = min_height;
    hintp->flags = PMinSize;
    hintp->x = hintp->y = 0;
    geom_result = NoValue;
    if (geom != NULL) {
        geom_result = XParseGeometry (geom, &hintp->x, &hintp->y,
				      &hintp->width, &hintp->height);
	if ((geom_result & WidthValue) && (geom_result & HeightValue)) {
#define max(a,b) ((a) > (b) ? (a) : (b))
	    hintp->width = max (hintp->width, hintp->min_width);
	    hintp->height = max (hintp->height, hintp->min_height);
	    hintp->flags |= USSize;
	}
	if ((geom_result & XValue) && (geom_result & YValue)) {
	    hintp->flags += USPosition;
	}
    }
    if (!(hintp->flags & USSize)) {
	hintp->width = defwidth;
	hintp->height = defheight;
	hintp->flags |= PSize;
    }
    if (!(hintp->flags & USPosition)) {
	hintp->x = defx;
	hintp->y = defy;
	hintp->flags |= PPosition;
    }

    if (geom_result & XNegative) {
	hintp->x = DisplayWidth (dpy, DefaultScreen (dpy)) + hintp->x -
		    hintp->width;
    }
    if (geom_result & YNegative) {
	hintp->y = DisplayHeight (dpy, DefaultScreen (dpy)) + hintp->y -
		    hintp->height;
    }
    return;
}

