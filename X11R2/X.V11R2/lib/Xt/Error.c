#ifndef lint
static char rcsid[] = "$Header: Error.c,v 1.14 88/02/26 12:37:53 swick Exp $";
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

#include <X11/Xlib.h>
#include <stdio.h>
#include "IntrinsicI.h"

static void _XtError (message)
    String message;
{
    extern void exit();
    (void) fprintf(stderr, "X Toolkit Error: %s\n", message);
    exit(1);
}

static void _XtWarning (message)
    String message;
{ (void) fprintf(stderr, "X Toolkit Warning: %s\n", message); }

static void (*errorFunction)() = _XtError;
static void (*warningFunction)() = _XtWarning;

void XtError(message) 
String message; 
{ 
    (*errorFunction)(message); 
}

void XtWarning(message) 
String message; 
{ 
    (*warningFunction)(message);
}

void XtSetErrorHandler(handler)
    register void (*handler)();
{
    if (handler != NULL) errorFunction = handler;
    else errorFunction = _XtError;
}

void XtSetWarningHandler(handler)
    register void (*handler)();
{
    if (handler != NULL) warningFunction = handler;
    else warningFunction = _XtWarning;
}


