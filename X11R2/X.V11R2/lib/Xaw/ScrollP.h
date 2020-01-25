/*
 * $Header: ScrollP.h,v 1.10 88/02/26 10:04:12 swick Exp $
 */


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

#ifndef _ScrollP_h
#define _ScrollP_h

#include <X11/Scroll.h>
#include <X11/CoreP.h>

typedef struct {
     /* public */
    Pixel	  foreground;	/* thumb foreground color */
    XtOrientation orientation;	/* horizontal or vertical */
    XtCallbackList scrollProc;	/* smooth scroll */
    XtCallbackList thumbProc;	/* jump scroll */
    Pixmap	  thumb;	/* thumb color */
    Cursor	  upCursor;	/* scroll up cursor */
    Cursor	  downCursor;	/* scroll down cursor */
    Cursor	  leftCursor;	/* scroll left cursor */
    Cursor	  rightCursor;	/* scroll right cursor */
    Cursor	  verCursor;	/* scroll vertical cursor */
    Cursor	  horCursor;	/* scroll horizontal cursor */
    float	  top;		/* What percent is above the win's top */
    float	  shown;	/* What percent is shown in the win */
    int		  length;	/* either height or width */
    int		  thickness;	/* either width or height */
     /* private */
    Cursor	  inactiveCursor; /* The normal cursor for scrollbar */
    char	  direction;	/* a scroll has started; which direction */
    GC		  gc;		/* a (shared) gc */
    int		  topLoc;	/* Pixel that corresponds to top */
    int		  shownLength;	/* Num pixels corresponding to shown */
} ScrollbarPart;


typedef struct _ScrollbarRec {
    CorePart		core;
    ScrollbarPart	scrollbar;
} ScrollbarRec;


typedef struct {int empty;} ScrollbarClassPart;


typedef struct _ScrollbarClassRec {
    CoreClassPart		core_class;
    ScrollbarClassPart		scrollbar_class;
} ScrollbarClassRec;

#endif _ScrollP_h
