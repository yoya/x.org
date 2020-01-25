#ifndef lint
static char rcsid[] = "$Header: Manage.c,v 1.9 88/02/26 12:42:45 swick Exp $";
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

#define MAXCHILDREN 100

void XtUnmanageChildren(children, num_children)
    WidgetList children;
    Cardinal num_children;
{
    register Widget		parent;
    register Widget		child;
    register Cardinal		num_unique_children, i;
    XtWidgetProc		change_managed;
    Widget			parent_cache[MAXCHILDREN];
    register WidgetList		unique_parents;
    register Widget		*parentP;

    if (num_children == 0) return;
    if (children[0] == NULL) {
	XtWarning("Null child passed to XtUnmanageChildren");
	return;
    }

    if (num_children <= MAXCHILDREN) {
	parentP = (Widget*) (unique_parents = parent_cache);
    } else {
	parentP = (Widget*) unique_parents = (WidgetList) XtMalloc(num_children * sizeof(Widget));
    }
    *parentP = children[0]->core.parent;

    num_unique_children = 0;
    for (i = 0; i < num_children; i++) {
	child = children[i];
	if (child == NULL) {
	    XtWarning("Null child passed to XtUnmanageChildren");
	    return;
	}
	parent = child->core.parent;
	if (! child->core.managed || parent->core.being_destroyed)
	    continue;
	if (parent != *parentP) {
	    register Widget *pP = parentP;
	    for (; pP > unique_parents && *pP != parent; pP-- ) ;
	    if (*pP != parent) *++parentP = parent;
	}
	num_unique_children++;
	child->core.managed = FALSE;
	if (child->core.mapped_when_managed) {
	    ((CompositeWidget)parent)->composite.num_mapped_children--;
	    if (XtIsRealized(child)) XtUnmapWidget(child);
	}
    }
    if (num_unique_children != 0) {
	for (; parentP >= unique_parents; parentP-- ) {
	    change_managed =
		((CompositeWidgetClass)(*parentP)->core.widget_class)->
		composite_class.change_managed;
	    if (change_managed != NULL && XtIsRealized(*parentP)) {
		(*change_managed) (*parentP);
	    }
	}
    }
    if (unique_parents != parent_cache) XtFree((char*) unique_parents);
} /* XtUnmanageChildren */


void XtUnmanageChild(child)
    Widget child;
{
    XtUnmanageChildren(&child, (Cardinal)1);
} /* XtUnmanageChild */


void XtManageChildren(children, num_children)
    WidgetList  children;
    Cardinal    num_children;
{
    register Widget		parent;
    register Widget		child;
    register Cardinal		num_unique_children, i;
    XtWidgetProc		change_managed;
    register WidgetList		unique_children;
    Widget			cache[MAXCHILDREN];
    Widget			parent_cache[MAXCHILDREN];
    register WidgetList		unique_parents;
    register Widget		*parentP;
    Boolean			parent_realized = FALSE;

    if (num_children == 0) return;
    if (children[0] == NULL) {
	XtWarning("Null child passed to XtManageChildren");
	return;
    }

    /* Construct new list of children that really need to be operated upon. */
    if (num_children <= MAXCHILDREN) {
	unique_children = cache;
	parentP = (Widget*) (unique_parents = parent_cache);
    } else {
	unique_children = (WidgetList) XtMalloc(num_children * sizeof(Widget));
	parentP = (Widget*) unique_parents = (WidgetList) XtMalloc(num_children * sizeof(Widget));
    }
    *parentP = children[0]->core.parent;
    if (!XtIsComposite(*parentP))
	XtError("Attempt to manage a child when parent is not a Composite");
    num_unique_children = 0;
    for (i = 0; i < num_children; i++) {
	child = children[i];
	if (child == NULL) {
	    XtWarning("Null child passed to XtManageChildren");
	    continue;
	}
	parent = child->core.parent;
	if ( child->core.managed ||
	     child->core.being_destroyed ||
	     parent->core.being_destroyed )
	    continue;
	if (parent != *parentP) {
	    register Widget *pP = parentP;
	    for (; pP > unique_parents && *pP != parent; pP-- ) ;
	    if (*pP != parent) {
		*++parentP = parent;
		if (!XtIsComposite(parent))
		    XtError("Attempt to manage a child when parent is not a Composite");
	    }
	}
	unique_children[num_unique_children++] = child;
	child->core.managed = TRUE;
	if (child->core.mapped_when_managed) {
	    ((CompositeWidget)parent)->composite.num_mapped_children++;
	}
    }

    if (num_unique_children != 0) {

	for (; parentP >= unique_parents; parentP--) {
	    if (XtIsRealized(*parentP)) {
	        parent_realized = TRUE;
		/* Compute geometry of new managed set of children. */
		change_managed =
		    ((CompositeWidgetClass) (*parentP)->core.widget_class)->
		    composite_class.change_managed;
		if (change_managed != NULL) (*change_managed) (*parentP);
	    }
	}
	if (parent_realized) {
	    /* Realize each child if necessary, then map if necessary */
	    for (i = 0; i < num_unique_children; i++) {
	        child = unique_children[i];
		if (XtIsRealized(child->core.parent)) {
		    if (! XtIsRealized(child)) XtRealizeWidget(child);
		    if (child->core.mapped_when_managed) XtMapWidget(child);
		}
	    }
	}
    }

    if (unique_children != cache) XtFree((char *) unique_children);
    if (unique_parents != parent_cache) XtFree((char *) unique_parents);
} /* XtManageChildren */


void XtManageChild(child)
    Widget child;
{
    XtManageChildren(&child, (Cardinal) 1);
} /* XtManageChild */


void XtSetMappedWhenManaged(widget, mapped_when_managed)
    register Widget widget;
    Boolean	    mapped_when_managed;
{
    if (widget->core.mapped_when_managed == mapped_when_managed) return;
    widget->core.mapped_when_managed = mapped_when_managed;
    if (! XtIsManaged(widget)) return;

    if (mapped_when_managed) {
	/* Didn't used to be mapped when managed.		*/
	((CompositeWidget) (widget->core.parent))->
	    composite.num_mapped_children++;
	if (XtIsRealized(widget)) XtMapWidget(widget);
    } else {
	/* Used to be mapped when managed.			*/
	((CompositeWidget) (widget->core.parent))->
	    composite.num_mapped_children--;
	if (XtIsRealized(widget)) XtUnmapWidget(widget);
    }
} /* XtSetMappedWhenManaged */


