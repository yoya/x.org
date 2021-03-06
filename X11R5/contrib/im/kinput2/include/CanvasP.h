/* $Id: CanvasP.h,v 1.1 1990/12/11 10:39:25 ishisone Rel $ */
/*
 * Copyright (c) 1990  Software Research Associates, Inc.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of Software Research Associates not be
 * used in advertising or publicity pertaining to distribution of the
 * software without specific, written prior permission.  Software Research
 * Associates makes no representations about the suitability of this software
 * for any purpose.  It is provided "as is" without express or implied
 * warranty.
 *
 * Author:  Makoto Ishisone, Software Research Associates, Inc., Japan
 */

#ifndef _CanvasP_h
#define _CanvasP_h

#include <X11/CoreP.h>
#include "Canvas.h"

typedef struct {
    int empty;
} CanvasClassPart;

typedef struct _CanvasClassRec {
    CoreClassPart	core_class;
    CanvasClassPart	canvas_class;
} CanvasClassRec;

extern CanvasClassRec canvasClassRec;

typedef struct {
    XtCallbackList	exposecallback;
    XtCallbackList	resizecallback;
} CanvasPart;

typedef struct _CanvasRec {
    CorePart	core;
    CanvasPart	canvas;
} CanvasRec;

#endif
