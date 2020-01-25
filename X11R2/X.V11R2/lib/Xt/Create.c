#ifndef lint
static char rcsid[] = "$Header: Create.c,v 1.35 88/02/26 12:36:43 swick Exp $";
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
#include <X11/StringDefs.h>
#include <X11/Xos.h>
#include <stdio.h>
#include "Resource.h"

extern void bcopy();
extern CallbackList _CompileCallbackList();

static void CallClassPartInit(ancestor, wc)
    WidgetClass ancestor, wc;
{
    if (ancestor->core_class.superclass != NULL) {
	CallClassPartInit(ancestor->core_class.superclass, wc);
    }
    if (ancestor->core_class.class_part_initialize != NULL) {
	(*(ancestor->core_class.class_part_initialize)) (wc);
    }
}

static void ClassInit(wc)
    WidgetClass wc;
{
    if (wc->core_class.version != XtVersion) {
	char buffer[200];

	(void) sprintf(buffer,
	   "Widget class %s version mismatch:\n  widget %d vs. intrinsics %d.",
	    wc->core_class.class_name,
	    wc->core_class.version, XtVersion);
	XtWarning(buffer);
    }

    if ((wc->core_class.superclass != NULL) 
	    && (!(wc->core_class.superclass->core_class.class_inited)))
 	ClassInit(wc->core_class.superclass);

    if (wc->core_class.class_initialize != NULL)
	(*(wc->core_class.class_initialize))();
    CallClassPartInit(wc, wc);

    wc->core_class.class_inited = TRUE;
}

static void CallInitialize (class, req_widget, new_widget, args, num_args)
    WidgetClass class;
    Widget      req_widget;
    Widget      new_widget;
    ArgList     args;
    Cardinal    num_args;
{
    if (class->core_class.superclass)
        CallInitialize (class->core_class.superclass,
	    req_widget, new_widget, args, num_args);
    if (class->core_class.initialize != NULL)
	(*class->core_class.initialize) (req_widget, new_widget);
    if (class->core_class.initialize_hook != NULL)
	(*class->core_class.initialize_hook) (new_widget, args, &num_args);
}

static void CallConstraintInitialize (class, req_widget, new_widget)
    ConstraintWidgetClass class;
    Widget	req_widget, new_widget;
{
    if (class->core_class.superclass != constraintWidgetClass)
	CallConstraintInitialize(
	    (ConstraintWidgetClass) class->core_class.superclass,
	    req_widget, new_widget);
    if (class->constraint_class.initialize != NULL)
        (*class->constraint_class.initialize) (req_widget, new_widget);
}



/* we should be able to merge _XtCreate1 and _XtCreate2 with
   changes to default resource management */
Widget _XtCreate1(name, widgetClass, parent)
    char        *name;
    WidgetClass widgetClass;
    Widget      parent;

{
    register Widget widget;

    if (! (widgetClass->core_class.class_inited))
	ClassInit(widgetClass);
    widget = (Widget) XtMalloc((unsigned)widgetClass->core_class.widget_size);
    widget->core.self = widget;
    if (name != NULL && *name != '\0')
	widget->core.name = strcpy(XtMalloc((unsigned)strlen(name)+1), name);
    else			/* all widgets should have a name entry */
	widget->core.name = "";	/* ...but save space by not duplicating this */
    widget->core.parent = parent;
    widget->core.widget_class = widgetClass;
    widget->core.tm.translations = NULL;
    widget->core.constraints = NULL;
    widget->core.being_destroyed =
	(parent != NULL ? parent->core.being_destroyed : FALSE);
/*||| garbage - set up default - need Inherit in Resource List */
    widget->core.screen = DefaultScreenOfDisplay(toplevelDisplay);
/*||| hack for setting colormap until issues resolved |||*/
    widget->core.colormap = DefaultColormapOfScreen(XtScreen(widget));

    return(widget);
}

static void _XtCreate2(widget, args, num_args, parent_constraint_class)
    Widget      widget;
    ArgList     args;
    Cardinal    num_args;
    ConstraintWidgetClass parent_constraint_class;
	/* NULL if not a subclass of Constraint */
{
    register _XtOffsetList  offsetList;
    XtCallbackList	    *pCallbacks;
    char		    widget_cache[600];
    Widget		    req_widget;
    char		    constraint_cache[100];
    char		    *req_constraints;
    Cardinal		    size;

    /* Compile any callback lists into internal form */
    for (offsetList = widget->core.widget_class->core_class.callback_private;
	 offsetList != NULL;
	 offsetList = offsetList->next) {
	 pCallbacks = (XtCallbackList *) ((int)widget - offsetList->offset - 1);
	if (*pCallbacks != NULL) {
#ifdef lint
	/* ||| GROSS!!! do the right thing after .h split!!! */
	extern CallbackList _CompileCallbackList();
#endif
	    *pCallbacks =
		(XtCallbackList) _CompileCallbackList(widget, *pCallbacks);
	}
    }

    size = XtClass(widget)->core_class.widget_size;
    req_widget = (Widget) XtStackAlloc(size, widget_cache);
    bcopy ((char *) widget, (char *) req_widget, (int) size);
    CallInitialize (XtClass(widget), req_widget, widget, args, num_args);
    if (parent_constraint_class != NULL) {
	size = parent_constraint_class->constraint_class.constraint_size;
	req_constraints = XtStackAlloc(size, constraint_cache);
	bcopy(widget->core.constraints, req_constraints, (int) size);
	req_widget->core.constraints = (caddr_t) req_constraints;
	CallConstraintInitialize(parent_constraint_class, req_widget, widget);
	XtStackFree(req_constraints, constraint_cache);
    }
    XtStackFree((char *) req_widget, widget_cache);
}


Widget XtCreateWidget(name, widgetClass, parent, args, num_args)
    String      name;
    WidgetClass widgetClass;
    Widget      parent;
    ArgList     args;
    Cardinal    num_args;
{
    register Widget	    widget;
    ConstraintWidgetClass   cwc;
    XtArgsProc		    insert_child;

    if (parent == NULL) {
	XtError("XtCreateWidget requires non-NULL parent");
    } else if (widgetClass == NULL) {
	XtError("XtCreateWidget requires non-NULL widget class");
    }
    widget = _XtCreate1(name, widgetClass, parent);
    if (XtIsSubclass(parent, constraintWidgetClass)) {
	cwc = (ConstraintWidgetClass) parent->core.widget_class;
	widget->core.constraints = 
	    (caddr_t) XtMalloc((unsigned)cwc->constraint_class.constraint_size);
    } else {
	cwc = NULL;
    }
    XtGetResources(widget, args, num_args);
/* ||| this may go away with later changes to resource management|||*/
    if (widget->core.depth == 0)
        widget->core.depth = widget->core.parent->core.depth;
    if (widget->core.screen == 0)
        widget->core.screen = widget->core.parent->core.screen;
    widget->core.ancestor_sensitive = 
	(parent->core.ancestor_sensitive & parent->core.sensitive);
    _XtCreate2(widget, args, num_args, cwc);
    if (XtIsComposite(parent)) {
	insert_child = ((CompositeWidgetClass) parent->core.widget_class)->
	    composite_class.insert_child;
	if (insert_child == NULL) {
	    XtError("NULL insert_child procedure");
	} else {
	    (*insert_child) (widget, args, &num_args);
	}
    }
    return (widget);
}

Widget XtCreateManagedWidget(name, widgetClass, parent, args, num_args)
    String      name;
    WidgetClass widgetClass;
    Widget      parent;
    ArgList     args;
    Cardinal    num_args;
{
    register Widget	    widget;

    widget = XtCreateWidget(name, widgetClass, parent, args, num_args);
    XtManageChild(widget);
    return widget;
}

Widget XtCreatePopupShell(name, widgetClass, parent, args, num_args)
    String      name;
    WidgetClass widgetClass;
    Widget      parent;
    ArgList     args;
    Cardinal    num_args;
{
    register Widget widget;

    if (parent == NULL) {
	XtError("XtCreatePopupShell requires non-NULL parent");
    } else if (widgetClass == NULL) {
	XtError("XtCreatePopupShell requires non-NULL widget class");
    }
    widget = _XtCreate1(name, widgetClass, parent);
    XtGetResources(widget, args, num_args);
    if (widget->core.depth == 0)
        widget->core.depth = DefaultDepthOfScreen(XtScreen(widget));
    widget->core.ancestor_sensitive =
	(parent->core.ancestor_sensitive & parent->core.sensitive);
    _XtCreate2(widget, args, num_args, (ConstraintWidgetClass) NULL);
    parent->core.popup_list =
	(WidgetList) XtRealloc((caddr_t) parent->core.popup_list,
               (unsigned) (parent->core.num_popups+1) * sizeof(Widget));
    parent->core.popup_list[parent->core.num_popups++] = widget;
    return(widget);
} /* XtCreatePopupShell */

Widget XtCreateApplicationShell(name, widgetClass, args, num_args)
    String      name;
    WidgetClass widgetClass;
    ArgList     args;
    Cardinal    num_args;
{
    register Widget widget;

    if (widgetClass == NULL) {
	XtError("XtCreateApplicationShell requires non-NULL widget class");
    }
    widget = _XtCreate1(name, widgetClass, (Widget)NULL);
    XtGetResources(widget, args, num_args);
    if (widget->core.depth == 0)
        widget->core.depth = DefaultDepthOfScreen(XtScreen(widget));
    widget->core.ancestor_sensitive = TRUE;
    _XtCreate2(widget, args, num_args, (ConstraintWidgetClass) NULL);

    return(widget);
} /* XtCreateApplicationShell */

