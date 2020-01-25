#include <X11/copyright.h>

/* $Header: SimpleP.h,v 1.4 88/02/14 14:10:07 rws Exp $ */
/* Copyright	Massachusetts Institute of Technology	1987 */

#ifndef _SimpleP_h
#define _SimpleP_h

#include <X11/Simple.h>
#include <X11/CoreP.h>

typedef struct {
    Boolean	(*change_sensitive)(/* widget */);
} SimpleClassPart;

#define XtInheritChangeSensitive ((Boolean (*)())_XtInherit)

typedef struct _SimpleClassRec {
    CoreClassPart	core_class;
    SimpleClassPart	simple_class;
} SimpleClassRec;

extern SimpleClassRec simpleClassRec;

typedef struct {
    /* resources */
    Cursor	cursor;
    Pixmap	insensitive_border;

    /* private state */
} SimplePart;

typedef struct _SimpleRec {
    CorePart	core;
    SimplePart	simple;
} SimpleRec;

#endif  _SimpleP_h
