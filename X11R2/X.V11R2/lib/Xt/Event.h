/* $Header: Event.h,v 1.6 88/02/26 12:38:57 swick Exp $ */

/* 
 * Event.h - exported types and functions for toolkit event handler
 * 
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

#ifndef _Event_h_
#define _Event_h_

#include "IntrinsicI.h"

typedef struct _XtEventRec *_XtEventTable;
typedef struct _GrabRec  *GrabList;

extern void _XtEventInitialize();

extern void _XtRegisterWindow(); /* window, widget */
    /* Window window; */
    /* Widget widget; */

extern void _XtUnregisterWindow(); /* window, widget */
    /* Window window; */
    /* Widget widget; */

typedef struct _XtEventRec {
     _XtEventTable	next;
     EventMask		mask;
     Boolean		non_filter;
     Boolean		select;
     Boolean		raw;
     XtEventHandler	proc;
     caddr_t		closure;
}XtEventRec;

typedef struct _GrabRec {
    GrabList next;
    Widget   widget;
    Boolean  exclusive;
    Boolean  spring_loaded;
    Widget   keyboard_focus;
}GrabRec;

GrabRec *grabList;
GrabRec *freeGrabs;

extern EventMask _XtBuildEventMask(); /* widget */
    /* Widget widget; */

#endif
