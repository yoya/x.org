/*
 * $NCDOr: init.c,v 1.1 1993/11/17 23:13:01 keithp Exp keithp $
 *
 * Copyright 1992 Network Computing Devices
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of NCD. not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  NCD. makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * NCD. DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL NCD.
 * BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN 
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Author:  Keith Packard, Network Computing Devices
 */

 /* $XConsortium: init.c,v 1.4 94/02/17 15:27:11 dpw Exp $ */

/* Hook up to an X server and set up a multiplexing LBX encoded connection */

#include <X11/Xlib.h>

int  lbxMajor, lbxMinor;

int
InitMux (dpy_name, requestp, eventp, errorp, sequencep)
    char    *dpy_name;
    int	    *requestp, *eventp, *errorp;
    int	    *sequencep;
{
    Display *dpy;
    int	    fd;

    dpy = XOpenDisplay (dpy_name);
    if (!dpy)
	return -1;
    
    if (!XLbxQueryExtension (dpy, requestp, eventp, errorp))
	return -1;
    if (!XLbxQueryVersion (dpy, &lbxMajor, &lbxMinor))
	return -1;
    /* yuck.  Guess at the request number; 1 XFreeGC per screen and 1 XSync */
#if R5Xlib
    /*  XXX This works for R5 Xlib.  The else block works for R6 Xlib.
     *  R6 Xlib calls shutdown() on the socket when you close the display;
     *  that apparently renders the dup'ed fd invalid.
     */
    *sequencep = NextRequest (dpy) + XScreenCount (dpy);
    fd = dup (ConnectionNumber (dpy));
    XCloseDisplay (dpy);
#else /* R6Xlib */
    *sequencep = NextRequest (dpy)  - 1;
    fd = ConnectionNumber (dpy);
#endif
    return fd;
}
