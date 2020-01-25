/* $Header: Cascade.c,v 1.8 88/02/26 10:32:55 swick Exp $ */


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

#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/ShellP.h>
#include <X11/Cascade.h>
#include <X11/CascadeP.h>

static char defaultTranslations[] =
       "<Btn1Up>:      cascade() \n\
        Button1<EnterWindow>: cascade1()";

static void PopdownCascade();
static void TMPopdownCascade();

static XtActionsRec actionsList[] = {
 {"cascade1", PopdownCascade},
 {"cascade",  TMPopdownCascade},
};

static void InsertChild(widget, args, num_args)
Widget widget;
ArgList args;
Cardinal num_args;
{
   CompositeWidget parent;
  if(!XtIsComposite(widget->core.parent)) {
    XtError("Can't Inherit from composite if not a subclass of composite");
    return;
  }
    parent = (CompositeWidget)widget->core.parent;
    parent->composite.num_children = 1;
    parent->composite.num_mapped_children = 0;
    parent->composite.children = (WidgetList) XtMalloc (sizeof(Widget));
    (parent->composite.children)[0]= widget;
    XtManageChild(widget);
    widget->core.mapped_when_managed = TRUE;
    return;
}

#define Offset(x)	(XtOffset(PopupWidget, x))

static XtResource resources[] = {
   { XtNsaveUnder, XtCSaveUnder, XtRBoolean, sizeof(Boolean),
       Offset(shell.save_under), XtRString, "TRUE"},
   { XtNtransient, XtCTransient, XtRBoolean, sizeof(Boolean),
       Offset(wm.transient), XtRString, "TRUE"},
};

PopupClassRec popupClassRec = {
  { /* core_class */
    /* superclass         */    (WidgetClass) &transientShellClassRec,
    /* class_name         */    "Popup",
    /* size               */    sizeof(PopupRec),
    /* Class Initializer  */	NULL,
    /* Class Part Init    */    NULL,
    /* Class init'ed ?    */	FALSE,
    /* initialize         */    NULL,
    /* initialize_hook    */    NULL,
    /* realize            */    XtInheritRealize,
    /* actions            */    actionsList,
    /* num_actions        */    XtNumber(actionsList),
    /* resources          */    resources,
    /* resource_count     */	XtNumber(resources),
    /* xrm_class          */    NULLQUARK,
    /* compress_motion    */    FALSE,
    /* compress_exposure  */    TRUE,
    /* compress_enterleave*/    FALSE,
    /* visible_interest   */    FALSE,
    /* destroy            */    NULL,
    /* resize             */    XtInheritResize,
    /* expose             */    XtInheritExpose,
    /* set_values         */    NULL,
    /* set_values_hook    */    NULL,
    /* set_values_almost  */    NULL,
    /* get_values_hook    */    NULL,
    /* accept_focus       */    NULL,
    /* version            */    XtVersion,
    /* callback_private   */    NULL,
    /* reserved_private   */    NULL,
    /* query_geometry     */    NULL,
  },
  { /* composite_class    */
    /* geometry_manager   */    XtInheritGeometryManager,
    /* change_managed     */    XtInheritChangeManaged,
    /* insert_child	  */	InsertChild,
    /* delete_child	  */	XtInheritDeleteChild,
    /* move_focus_to_next */    NULL,
    /* move_focus_to_prev */    NULL
  }
};

WidgetClass popupWidgetClass = (WidgetClass) (&popupClassRec);

static void PopdownCascade (widget,event,params,num_params)
   Widget   widget;
   XEvent *event;
   String *params;
   Cardinal *num_params;
{
   Widget w;
   w = widget;
   while (w->core.popup_list != NULL) {
      XtPopdown(w->core.popup_list[0]);
      w = w->core.popup_list[0]; 
   }

}
static void TMPopdownCascade(widget,event,params,num_params)
    Widget widget;
    XEvent *event;
    String *params;
    Cardinal *num_params;
{
   PopdownCascade(widget,event,params,num_params);
   XtPopdown(widget);
}
