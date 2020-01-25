#ifndef lint
static char rcsid[] = "$Header: Composite.c,v 1.4 88/02/26 12:29:10 swick Exp $";
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


#define COMPOSITE
#include "IntrinsicI.h"
#include "Resource.h"
#include "StringDefs.h"

static void CompositeClassPartInitialize();
static void CompositeInitialize();
static void CompositeInsertChild();
static void CompositeDeleteChild();
static void CompositeDestroy();

globaldef CompositeClassRec compositeClassRec = {
  { /******* CorePart *******/
    /* superclass	    */	&widgetClassRec,
    /* class_name	    */	"Composite",
    /* widget_size	    */	sizeof(CompositeRec),
    /* class_initialize     */  NULL,
    /* class_part_initialize*/	CompositeClassPartInitialize,
    /* class_inited	    */	FALSE,
    /* initialize	    */	CompositeInitialize,
    /* initialize_hook      */	NULL,		
    /* realize		    */	XtInheritRealize,
    /* actions		    */	NULL,
    /* num_actions	    */	0,
    /* resources	    */	NULL,
    /* num_resources	    */	0,
    /* xrm_class	    */	NULLQUARK,
    /* compress_motion      */	FALSE,
    /* compress_exposure    */	TRUE,
    /* compress_enterleave  */  FALSE,
    /* visible_interest     */	FALSE,
    /* destroy		    */	CompositeDestroy,
    /* resize		    */	NULL,
    /* expose		    */	NULL,
    /* set_values	    */	NULL,
    /* set_values_hook      */	NULL,			
    /* set_values_almost    */	XtInheritSetValuesAlmost,  
    /* get_values_hook      */	NULL,			
    /* accept_focus	    */	NULL,
    /* version		    */	XtVersion,
    /* callback_offsets     */  NULL,
    /* tm_table		    */  NULL,
  },
  { /**** CompositePart *****/
    /* geometry_handler     */  NULL,
    /* change_managed       */  NULL,
    /* insert_child	    */  CompositeInsertChild,
    /* delete_child	    */  CompositeDeleteChild,
    /* move_focus_to_next   */  NULL,
    /* move_focus_to_prev   */  NULL
    }
};

globaldef WidgetClass compositeWidgetClass = (WidgetClass) &compositeClassRec;


static void CompositeClassPartInitialize(widgetClass)
	WidgetClass widgetClass;
{
    register CompositeWidgetClass wc = (CompositeWidgetClass) widgetClass;
    register CompositeWidgetClass super =
	    (CompositeWidgetClass) wc->core_class.superclass;

    /* We don't need to check for null super since we'll get to composite
       eventually, and it had better define them!  */

    if (wc->composite_class.geometry_manager == XtInheritGeometryManager) {
	wc->composite_class.geometry_manager =
		super->composite_class.geometry_manager;
    }

    if (wc->composite_class.change_managed == XtInheritChangeManaged) {
	wc->composite_class.change_managed =
		super->composite_class.change_managed;
    }

    if (wc->composite_class.insert_child == XtInheritInsertChild) {
	wc->composite_class.insert_child = super->composite_class.insert_child;
    }

    if (wc->composite_class.delete_child == XtInheritDeleteChild) {
	wc->composite_class.delete_child = super->composite_class.delete_child;
    }

    if (wc->composite_class.move_focus_to_next == 
	    XtInheritMoveFocusToNext) {
	wc->composite_class.move_focus_to_next =
		super->composite_class.move_focus_to_next;
    }

    if (wc->composite_class.move_focus_to_prev ==
	    XtInheritMoveFocusToPrev) {
	wc->composite_class.move_focus_to_prev =
		super->composite_class.move_focus_to_prev;
    }
}

static void CompositeDestroy(w)
    CompositeWidget	w;
{
    XtFree((char *) w->composite.children);
}

/*ARGSUSED*/
static void CompositeInsertChild(w, args, num_argsP)
    Widget	w;
    ArgList	args;
    Cardinal	*num_argsP;
{
    register Cardinal	     position;
    register Cardinal        i;
    register CompositeWidget cw;
    register WidgetList      children;

    cw = (CompositeWidget) w->core.parent;
    children = cw->composite.children;

    if (cw->composite.insert_position != NULL)
	position = (*(cw->composite.insert_position))(w);
    else
	position = cw->composite.num_children;

    if (cw->composite.num_children == cw->composite.num_slots) {
	/* Allocate more space */
	cw->composite.num_slots +=  (cw->composite.num_slots / 2) + 2;
	cw->composite.children = children = 
	    (WidgetList) XtRealloc((caddr_t) children,
	    (unsigned) (cw->composite.num_slots) * sizeof(Widget));
    }
    /* Ripple children up one space from "position" */
    for (i = cw->composite.num_children; i > position; i--) {
	children[i] = children[i-1];
    }
    children[position] = w;
    cw->composite.num_children++;
}

static void CompositeDeleteChild(w)
    Widget	w;
{
    register Cardinal	     position;
    register Cardinal	     i;
    register CompositeWidget cw;

    cw = (CompositeWidget) w->core.parent;

    for (position = 0; position < cw->composite.num_children; position++) {
        if (cw->composite.children[position] == w) {
	    break;
	}
    }
    if (position == cw->composite.num_children) return;

    /* Ripple children down one space from "position" */
    cw->composite.num_children--;
    for (i = position; i < cw->composite.num_children; i++) {
        cw->composite.children[i] = cw->composite.children[i+1];
    }
}

/* ARGSUSED */
static void CompositeInitialize(requested_widget, new_widget)
    Widget   new_widget, requested_widget;
{
    register CompositeWidget cw;

    cw = (CompositeWidget) new_widget;
    cw->composite.insert_position = NULL;
    cw->composite.num_children = 0;
    cw->composite.num_mapped_children = 0;
    cw->composite.children = NULL;
    cw->composite.num_slots = 0;
}
