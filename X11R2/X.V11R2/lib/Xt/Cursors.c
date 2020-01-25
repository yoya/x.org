#ifndef lint
static char rcsid[] = "$Header: Cursors.c,v 1.15 88/02/26 12:37:09 swick Exp $";
#endif lint

/***********************************************************
Copyright 1987, 1988 by Digital Equipment Corporation, Maynard, Massachusetts,
and the Massachusetts Institute of Technology, Cambridge, Massachusetts.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the names of Digital or MIT not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.  

DIGITAL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/

#include <X11/Intrinsic.h>

static Cursor 	*cursorarray = NULL;
static int 	numcursors = 0;

static Boolean initialized = FALSE;


void _XtCursorsInitialize()
{
    if (initialized) {
	XtWarning("Initializing Cursors twice.");
    	return;
    }
    initialized = TRUE;
    numcursors = 1;
    cursorarray = XtNew(Cursor);
    *cursorarray = NULL;
}

Cursor XtGetCursor(dpy, num)
    Display *dpy;
    int num;
{
    if (num >= numcursors) {
	cursorarray = (Cursor *) XtRealloc((char *)cursorarray,
					(unsigned) (num + 1) * sizeof(Cursor));
	for (; numcursors <= num ; numcursors++)
	    cursorarray[numcursors] = NULL;
    }
    if (cursorarray[num] == NULL)
	cursorarray[num] = XCreateFontCursor(dpy, (unsigned) num);
    return cursorarray[num];
}
