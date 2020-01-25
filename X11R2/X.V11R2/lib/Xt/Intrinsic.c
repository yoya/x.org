#ifndef lint
static char rcsid[] = "$Header: Intrinsic.c,v 1.116 88/02/26 17:55:30 swick Exp $";
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

#define INTRINSIC_C

#include "IntrinsicI.h"
#include "StringDefs.h"
#include "Event.h"

static void SetAncestorSensitive();

globaldef XrmDatabase XtDefaultDB = NULL;
globaldef XrmName XtApplicationName;
globaldef XrmClass XtApplicationClass;

Boolean XtIsSubclass(widget, widgetClass)
    Widget    widget;
    WidgetClass widgetClass;
{
    register WidgetClass w;

    for (w = widget->core.widget_class; w != NULL; w = w->core_class.superclass)
	if (w == widgetClass) return (TRUE);
    return (FALSE);
} /* XtIsSubclass */

static void ComputeWindowAttributes(widget,value_mask,values)
    Widget		 widget;
    XtValueMask		 *value_mask;
    XSetWindowAttributes *values;
{
    *value_mask = CWEventMask;
    (*values).event_mask = _XtBuildEventMask(widget);
    if (widget->core.background_pixmap != UnspecifiedPixmap) {
	*value_mask |= CWBackPixmap;
	(*values).background_pixmap = widget->core.background_pixmap;
    } else {
	*value_mask |= CWBackPixel;
	(*values).background_pixel = widget->core.background_pixel;
    }
    if (widget->core.border_pixmap != UnspecifiedPixmap) {
	*value_mask |= CWBorderPixmap;
	(*values).border_pixmap = widget->core.border_pixmap;
    } else {
	*value_mask |= CWBorderPixel;
	(*values).border_pixel = widget->core.border_pixel;
    }
    if (widget->core.widget_class->core_class.expose == (XtExposeProc) NULL) {
	/* Try to avoid redisplay upon resize by making bit_gravity the same
	   as the default win_gravity */
	*value_mask |= CWBitGravity;
	(*values).bit_gravity = NorthWestGravity;
    }
} /* ComputeWindowAttributes */

static void CallChangeManaged(widget)
    register Widget		widget;
{
    register CompositeWidget	cw;
    register Cardinal		i;
    XtWidgetProc		change_managed;
    register WidgetList		children;
    int    			managed_children = 0;

    if (XtIsComposite (widget)) {
	cw = (CompositeWidget) widget;
	children = cw->composite.children;

	/* CallChangeManaged for all children */
	for (i = cw->composite.num_children; i != 0; --i) {
	    CallChangeManaged (children[i-1]);
	    if (XtIsManaged(children[i-1])) managed_children++;
	}

	change_managed = ((CompositeWidgetClass) widget->core.widget_class)->
		composite_class.change_managed;
    
	if (change_managed != NULL && managed_children != 0) {
	    (*change_managed) (widget);
	}
    }
} /* CallChangeManaged */

static void RealizeWidget(widget)
    register Widget		widget;
{
    register CompositeWidget	cw;
    XtValueMask			value_mask;
    XSetWindowAttributes	values;
    register Cardinal		left_to_map, i;
    XtRealizeProc		realize;
    register WidgetList		children;

    if (XtIsRealized(widget)) return;

    _XtBindActions(widget, &widget->core.tm,0);
    _XtInstallTranslations(widget, widget->core.tm.translations);

    ComputeWindowAttributes (widget, &value_mask, &values);
    realize = widget->core.widget_class->core_class.realize;

    if (realize == NULL) XtError("No realize class procedure defined");
    else (*realize) (widget, &value_mask, &values);

    _XtRegisterGrabs(widget,&widget->core.tm);
    _XtRegisterWindow (widget->core.window, widget);

    if (XtIsComposite (widget)) {
	cw = (CompositeWidget) widget;
	children = cw->composite.children;
	/* Realize all children */
	for (i = cw->composite.num_children; i != 0; --i) {
	    RealizeWidget (children[i-1]);
	}
	/* Map children that are managed and mapped_when_managed */
	if (cw->composite.num_children == cw->composite.num_mapped_children) {
	    XMapSubwindows (XtDisplay (widget), XtWindow (widget));
	} else {
	    for (i = 0, left_to_map = cw->composite.num_mapped_children;
                 left_to_map != 0;
                 i++) {
		if (children[i]->core.managed &&
			children[i]->core.mapped_when_managed) {
		    XtMapWidget (children[i]);
		    left_to_map--;
		}
	    }
	}
    }

    /* If this is the application's popup shell, map it */
    if (widget->core.parent == NULL) XtMapWidget (widget);
} /* RealizeWidget */

void XtRealizeWidget (widget)
    register Widget		widget;
{
    if (XtIsRealized (widget)) return;

    CallChangeManaged(widget);
    RealizeWidget(widget);
} /* XtRealizeWidget */

void XtCreateWindow(widget, window_class, visual, value_mask, attributes)
    Widget		 widget;
    unsigned int	 window_class;
    Visual		 *visual;
    Mask		 value_mask;
    XSetWindowAttributes *attributes;
{
    if (widget->core.window == None) {
	widget->core.window =
	    XCreateWindow (
		XtDisplay (widget),
		(widget->core.parent ?
		    widget->core.parent->core.window :
		    widget->core.screen->root),
		widget->core.x, widget->core.y,
		widget->core.width, widget->core.height,
		widget->core.border_width, (int) widget->core.depth,
		window_class, visual, value_mask, attributes);
    }
} /* XtCreateWindow */
			
void XtSetSensitive(widget, sensitive)
    register Widget widget;
    Boolean	    sensitive;
{
    Arg			args[1];
    register Cardinal   i;
    register WidgetList children;

    if (widget->core.sensitive == sensitive) return;

    XtSetArg(args[0], XtNsensitive, sensitive);
    XtSetValues(widget, args, XtNumber(args));

    /* If widget's ancestor_sensitive is TRUE, propagate new sensitive to
       children's ancestor_sensitive; else do nothing as children's
       ancestor_sensitive is already FALSE */
    
    if (widget->core.ancestor_sensitive && XtIsComposite (widget)) {
	children = ((CompositeWidget) widget)->composite.children;
	for (i = 0; i < ((CompositeWidget)widget)->composite.num_children; i++){
	    SetAncestorSensitive (children[i], sensitive);
	}
    }
} /* XtSetSensitive */

static void SetAncestorSensitive(widget, ancestor_sensitive)
    register Widget widget;
    Boolean	    ancestor_sensitive;
{
    Arg			args[1];
    register Cardinal   i;
    register WidgetList children;

    if (widget->core.ancestor_sensitive == ancestor_sensitive) return;

    XtSetArg(args[0], XtNancestorSensitive, ancestor_sensitive);
    XtSetValues(widget, args, XtNumber(args));

    /* If widget's sensitive is TRUE, propagate new ancestor_sensitive to
       children's ancestor_sensitive; else do nothing as children's
       ancestor_sensitive is already FALSE */
    
    if (widget->core.sensitive && XtIsComposite(widget)) {
	children = ((CompositeWidget) widget)->composite.children;
	for (i = 0; i < ((CompositeWidget)widget)->composite.num_children; i++){
	    SetAncestorSensitive (children[i], ancestor_sensitive);
	}
    }
} /* SetAncestorSensitive */

/* ---------------- XtNameToWidget ----------------- */

static Widget NameListToWidget(root, names)
    register Widget root;
    XrmNameList     names;
{
    register Cardinal   i;
    register WidgetList children;
    register XrmName    name;

    name = *names;
    if (name == NULLQUARK) return root;
    if (! XtIsComposite(root)) return NULL;

    children = ((CompositeWidget) root)->composite.children;
    for (i = 0; i < ((CompositeWidget) root)->composite.num_children; i++) {
	if (name == children[i]->core.xrm_name)
	    return NameListToWidget(children[i], &names[1]);
    }
    children = root->core.popup_list;
    for (i = 0; i < root->core.num_popups; i++) {
	if (name == children[i]->core.xrm_name)
	    return NameListToWidget(children[i], &names[1]);
    }
    return NULL;
} /* NameListToWidget */

Widget XtNameToWidget(root, name)
    Widget root;
    String name;
{
    XrmName	names[1000];
    XrmStringToNameList(name, names);
    if (names[0] == NULLQUARK) return NULL;
    return NameListToWidget(root, names);
} /* XtNameToWidget */

void _XtInherit()
{
    XtError("Unresolved inheritance operation");
}

/* Define user versions of intrinsics macros */

#undef XtDisplay

Display *XtDisplay(widget)
	Widget widget;
{
	return widget->core.screen->display;
}

#undef XtScreen

Screen *XtScreen(widget)
	Widget widget;
{
	return widget->core.screen;
}

#undef XtWindow

Window XtWindow(widget)
	Widget widget;
{
	return widget->core.window;
}

#undef XtSuperclass

WidgetClass XtSuperclass(widget)
	Widget widget;
{
	return XtClass(widget)->core_class.superclass;
}

#undef XtClass

WidgetClass XtClass(widget)
	Widget widget;
{
	return widget->core.widget_class;
}

#undef XtIsManaged

Boolean XtIsManaged(widget)
	Widget widget;
{
	return widget->core.managed;
}

#undef XtIsRealized

Boolean XtIsRealized (widget)
    Widget   widget;
{
    return (widget->core.window != NULL);
} /* XtIsRealized */

#undef XtParent

Widget XtParent(widget)
    Widget widget;
{
    return (widget->core.parent);
}


#undef XtIsSensitive

Boolean XtIsSensitive(widget)
    Widget widget;
{
    return (widget->core.sensitive && widget->core.ancestor_sensitive);
}
