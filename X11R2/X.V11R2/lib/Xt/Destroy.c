#ifndef lint
static char rcsid[] = "$Header: Destroy.c,v 1.8 88/02/26 12:37:27 swick Exp $";
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

#include "IntrinsicI.h"

static void Recursive(widget, proc)
    Widget       widget;
    XtWidgetProc proc;
{
    register int i;
    CompositeWidget cw;

    /* Recurse down normal children */
    if (XtIsComposite(widget)) {
	cw = (CompositeWidget) widget;
	for (i = 0; i < cw->composite.num_children; i++) {
	    Recursive(cw->composite.children[i], proc);
	}
    } 

    /* Recurse down popup children */
    for (i = 0; i < widget->core.num_popups; i++) {
	Recursive(widget->core.popup_list[i], proc);
    }

    /* Finally, apply procedure to this widget */
    (*proc) (widget);  
} /* Recursive */

static void Phase1Destroy (widget)
    Widget    widget;
{
    widget->core.being_destroyed = TRUE;
} /* Phase1Destroy */

static void Phase2Callbacks(widget)
    Widget    widget;
{
    _XtCallCallbacks(
	&(CallbackList)(widget->core.destroy_callbacks), (caddr_t) NULL);
} /* Phase2Callbacks */

static void Phase2Destroy(widget)
    register Widget widget;
{
    register WidgetClass	    class;
    register ConstraintWidgetClass  cwClass;

    /* Call constraint destroy procedures */
    if (widget->core.parent != NULL && widget->core.constraints != NULL) {
	cwClass = (ConstraintWidgetClass)widget->core.parent->core.widget_class;
	for (;;) {
	    if (cwClass->constraint_class.destroy != NULL)
		(*(cwClass->constraint_class.destroy)) (widget);
            if (cwClass == (ConstraintWidgetClass)constraintWidgetClass) break;
            cwClass = (ConstraintWidgetClass) cwClass->core_class.superclass;
	}
    }

    /* Call widget destroy procedures */
    for (class = widget->core.widget_class;
	 class != NULL; 
	 class = class->core_class.superclass) {
	if ((class->core_class.destroy) != NULL)
	    (*(class->core_class.destroy))(widget);
    }
} /* Phase2Destroy */

/*ARGSUSED*/
static void XtPhase2Destroy (widget, closure, call_data)
    register Widget widget;
    caddr_t	    closure;
    caddr_t	    call_data;
{
    Display	    *display;
    Window	    window;
    XtWidgetProc    delete_child;

    if (widget->core.parent != NULL &&
	XtIsComposite(widget->core.parent)) {
	XtUnmanageChild(widget);
	delete_child =
	    (((CompositeWidgetClass) widget->core.parent->core.widget_class)
		->composite_class.delete_child);
	if (delete_child == NULL) {
	    XtWarning("NULL delete_child procedure");
	} else {
	    (*delete_child) (widget);
	}
    }
    display = XtDisplay(widget); /* widget is freed in Phase2Destroy */
    window = widget->core.window;
    Recursive(widget, Phase2Callbacks);
    Recursive(widget, Phase2Destroy);
    if (window != NULL) XDestroyWindow(display,window);
} /* XtPhase2Destroy */


void XtDestroyWidget (widget)
    Widget    widget;
{
    if (widget->core.being_destroyed) return;
    Recursive(widget, Phase1Destroy);
    _XtAddCallback(widget, &DestroyList, XtPhase2Destroy, (caddr_t) NULL);
} /* XtDestroyWidget */


