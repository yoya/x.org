/* LINTLIBRARY */
#ifndef lint
static char rcsid[] = "$Header: Core.c,v 1.13 88/02/26 12:32:11 swick Exp $";
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

#define CORE
#include "IntrinsicI.h"
#include "StringDefs.h"
#include "Event.h"
#include "Resource.h"
/******************************************************************
 *
 * Core Resources
 *
 ******************************************************************/
/* UnspecifiedPixmap is a magic value used by ComputeWindowAttributes
   and Shell.Realize to know whether or not a background or border
   pixmap was defined */
static Pixmap defNone = UnspecifiedPixmap;
static int zero = 0;
static int one  = 1;
static Boolean true = TRUE;

static XtResource resources[] = {
/* |||
    {XtNscreen, XtCScreen, XtRPointer, sizeof(int),
      XtOffset(Widget,core.screen), XtRPointer, (caddr_t) &null}, 
    {XtNcolormap, XtCColormap, XtRPointer, sizeof(Colormap),
      XtOffset(Widget,core.colormap), XtRPointer, (caddr_t) &null},
*/
    {XtNancestorSensitive, XtCSensitive, XtRBoolean, sizeof(Boolean),
         XtOffset(Widget,core.ancestor_sensitive), XtRBoolean, (caddr_t) &true},
    {XtNx, XtCPosition, XtRInt, sizeof(int),
         XtOffset(Widget,core.x), XtRInt, (caddr_t) &zero},
    {XtNy, XtCPosition, XtRInt, sizeof(int),
         XtOffset(Widget,core.y), XtRInt, (caddr_t) &zero},
    {XtNwidth, XtCWidth, XtRInt, sizeof(int),
         XtOffset(Widget,core.width), XtRInt, (caddr_t) &zero},
    {XtNheight, XtCHeight, XtRInt, sizeof(int),
         XtOffset(Widget,core.height), XtRInt, (caddr_t) &zero},
/* default depth should be "InheritFromParent", and we should have a */
/* string to depth type converter, but it needs the widget to be able */
/* to find the parent's depth... right now, we kludge it and check for */
/* a depth of 0 in XtCreateWidget. Gross. */
    {XtNdepth, XtCDepth, XtRInt,sizeof(int),
         XtOffset(Widget,core.depth), XtRInt, (caddr_t) &zero},
    {XtNbackground, XtCBackground, XtRPixel,sizeof(Pixel),
         XtOffset(Widget,core.background_pixel),
         XtRDefaultColor,(caddr_t)&one},
    {XtNbackgroundPixmap, XtCPixmap, XtRPixmap, sizeof(Pixmap),
         XtOffset(Widget,core.background_pixmap), XtRPixmap,(caddr_t)&defNone},
    {XtNborderWidth, XtCBorderWidth, XtRInt, sizeof(int),
         XtOffset(Widget,core.border_width), XtRInt, (caddr_t) &one},
    {XtNborderColor, XtCBorderColor, XtRPixel,sizeof(Pixel),
         XtOffset(Widget,core.border_pixel), XtRDefaultColor,(caddr_t)&zero},
    {XtNborderPixmap, XtCPixmap, XtRPixmap, sizeof(Pixmap),
         XtOffset(Widget,core.border_pixmap), XtRPixmap, (caddr_t)&defNone},
    {XtNsensitive, XtCSensitive, XtRBoolean, sizeof(Boolean),
         XtOffset(Widget,core.sensitive), XtRBoolean, (caddr_t) &true},
    {XtNmappedWhenManaged, XtCMappedWhenManaged, XtRBoolean, sizeof(Boolean),
         XtOffset(Widget,core.mapped_when_managed), XtRBoolean, (caddr_t)&true},
    {XtNdestroyCallback, XtCCallback, XtRCallback,sizeof(caddr_t),
         XtOffset(Widget,core.destroy_callbacks), XtRCallback, (caddr_t)NULL},
    {XtNtranslations, XtCTranslations, XtRTranslationTable,
        sizeof(XtTranslations), XtOffset(Widget,core.tm.translations),
        XtRTranslationTable, (caddr_t)NULL},
    };

static void CoreClassPartInitialize();
void CoreInitialize();
static void CoreDestroy();
static void CoreRealize ();
static Boolean CoreSetValues ();
static void CoreSetValuesAlmost();

globaldef WidgetClassRec widgetClassRec = {
  {
    /* superclass	  */	NULL,
    /* class_name	  */	"Core",
    /* widget_size	  */	sizeof(WidgetRec),
    /* class_initialize   */    NULL,
    /* class_part_initialize*/	CoreClassPartInitialize,
    /* class_inited       */	FALSE,
    /* initialize	  */	CoreInitialize,
    /* initialize_hook    */	NULL,		
    /* realize		  */	CoreRealize,
    /* actions		  */	NULL,
    /* num_actions	  */	0,
    /* resources	  */	resources,
    /* num_resources	  */	XtNumber(resources),
    /* xrm_class	  */	NULLQUARK,
    /* compress_motion	  */	FALSE,
    /* compress_exposure  */	TRUE,
    /* compress_enterleave*/ 	FALSE,
    /* visible_interest	  */	FALSE,
    /* destroy		  */	CoreDestroy,
    /* resize		  */	NULL,
    /* expose		  */	NULL,
    /* set_values	  */	CoreSetValues,
    /* set_values_hook    */	NULL,			
    /* set_values_almost  */	CoreSetValuesAlmost,  
    /* get_values_hook    */	NULL,			
    /* accept_focus	  */	NULL,
    /* version		  */	XtVersion,
    /* callback_offsets   */    NULL,
    /* tm_table           */    NULL,
  },
};

globaldef WidgetClass widgetClass = &widgetClassRec;

/*
 * Start of core inheritance routines.
 */

int _XtInheritTranslations = NULL;

static void ConstructCallbackOffsets(widgetClass)
    WidgetClass widgetClass;
{
    register Cardinal i;
    register XrmResourceList resourceList;
    register _XtOffsetList newItem;
    XrmQuark xtQCallback = XrmStringToQuark(XtRCallback);

    if (widgetClass->core_class.superclass != NULL)
	widgetClass->core_class.callback_private = 
	    widgetClass->core_class.superclass->core_class.callback_private;
    for (i = widgetClass->core_class.num_resources,
	 resourceList = (XrmResourceList) widgetClass->core_class.resources;
         i != 0; i--)
	if (resourceList[i-1].xrm_type == xtQCallback) {
	    newItem = XtNew(XtOffsetRec);
	    newItem->next   = widgetClass->core_class.callback_private;
	    widgetClass->core_class.callback_private = newItem;
	    newItem->offset = resourceList[i-1].xrm_offset;
	    newItem->name   = resourceList[i-1].xrm_name;
     }
}

static void CoreClassPartInitialize(wc)
    register WidgetClass wc;
{
    register WidgetClass super = wc->core_class.superclass;
    extern void  XrmCompileResourceList();
    extern Opaque _CompileActionTable();

    /* We don't need to check for null super since we'll get to core
       eventually, and it had better define them!  */

    if (wc->core_class.realize == XtInheritRealize) {
	wc->core_class.realize = super->core_class.realize;
    }

    if (wc->core_class.resize == XtInheritResize) {
	wc->core_class.resize = super->core_class.resize;
    }

    if (wc->core_class.expose == XtInheritExpose) {
	wc->core_class.expose = super->core_class.expose;
    }

    if (wc->core_class.set_values_almost == XtInheritSetValuesAlmost) {
	wc->core_class.set_values_almost = super->core_class.set_values_almost;
    }

    if (wc->core_class.accept_focus == XtInheritAcceptFocus) {
	wc->core_class.accept_focus = super->core_class.accept_focus;
    }

    if (wc->core_class.query_geometry == XtInheritQueryGeometry) {
	wc->core_class.query_geometry = super->core_class.query_geometry;
    }

    if (wc->core_class.resources != NULL) {
	XrmCompileResourceList(wc->core_class.resources,
		wc->core_class.num_resources);
    }

    ConstructCallbackOffsets(wc);

    if (wc->core_class.tm_table == (char *) XtInheritTranslations) {
	wc->core_class.tm_table =
		wc->core_class.superclass->core_class.tm_table;
    } else if (wc->core_class.tm_table != NULL) {
	wc->core_class.tm_table =
	      (String) _ParseTranslationTable(wc->core_class.tm_table);
    }

    if (wc->core_class.actions != NULL) {
	/* Compile the action table into a more efficient form */
        wc->core_class.actions = (XtActionList) _CompileActionTable(
	    wc->core_class.actions, wc->core_class.num_actions);
    }
}

/* ARGSUSED */
void CoreInitialize(requested_widget, new_widget)
    Widget   requested_widget;
    register Widget new_widget;
{
    new_widget->core.window = (Window) NULL;
    new_widget->core.managed = FALSE;
    new_widget->core.visible = TRUE;
    new_widget->core.event_table = NULL;
    new_widget->core.popup_list = NULL;
    new_widget->core.num_popups = 0;
    new_widget->core.tm.proc_table = NULL;
    new_widget->core.tm.current_state = NULL;
    new_widget->core.tm.lastEventTime = 0;
    if (new_widget->core.tm.translations == NULL)
     new_widget->core.tm.translations =
	(XtTranslations)new_widget->core.widget_class->core_class.tm_table;


/* ||| verify valid screen ?*/

/* ||| check for valid depth and colormap for this screen */

}

static void CoreRealize(widget, value_mask, attributes)
    Widget		 widget;
    Mask		 *value_mask;
    XSetWindowAttributes *attributes;
{
    XtCreateWindow(widget, (unsigned int) InputOutput,
	(Visual *) CopyFromParent, *value_mask, attributes);
} /* CoreRealize */

static void CoreDestroy (widget)
    register Widget    widget;
{
    register XtEventRec *event, *next;
    int i;

    if (*widget->core.name != '\0') /* special case; we didn't copy this */
	XtFree((char *) (widget->core.name));
    if (widget->core.background_pixmap > UnspecifiedPixmap)
	XFreePixmap(XtDisplay(widget), widget->core.background_pixmap);
    if (widget->core.border_pixmap > UnspecifiedPixmap)
	XFreePixmap(XtDisplay(widget), widget->core.border_pixmap);
    event = widget->core.event_table;
    while (event != NULL) {
	next = event->next;
	XtFree((char *) event);
	event = next;
    }
    XtFree((char *) widget->core.tm.proc_table);
    _XtUnregisterWindow(widget->core.window, widget);
    if (widget->core.constraints != NULL)
	XtFree((char *) widget->core.constraints);
    for (i = 0; i < widget->core.num_popups; i++)
        XDestroyWindow(XtDisplay(widget->core.popup_list[i]),
		       widget->core.popup_list[i]->core.window);
    if (widget->core.popup_list != NULL) XtFree(widget->core.popup_list);
    XtFree((char *) widget);
} /* CoreDestroy */

/* ARGSUSED */
static Boolean CoreSetValues(old, reference, new)
    Widget old, reference, new;
{
    Boolean redisplay;
    Mask    window_mask;
    XSetWindowAttributes attributes;

    redisplay = FALSE;

    /* Check everything that depends upon window being realized */
    if (XtIsRealized(old)) {
	window_mask = 0;
	/* Check window attributes */
	if (old->core.background_pixel != new->core.background_pixel) {
	   window_mask |= CWBackPixel;
	   redisplay = TRUE;
	}
	if (old->core.background_pixmap != new->core.background_pixmap) {
	   window_mask |= CWBackPixmap;
	   redisplay = TRUE;
	}
	if (old->core.border_pixel != new->core.border_pixel)
	   window_mask |= CWBorderPixel;
	if (old->core.border_pixmap != new->core.border_pixmap)
	   window_mask |= CWBorderPixmap;
	if (old->core.depth != new->core.depth) {
	   XtWarning("Can't change widget depth");
	   new->core.depth = old->core.depth;
	}
	if (window_mask != 0) {
	    /* Actually change X window attributes */
	    attributes.background_pixmap = new->core.background_pixmap;
	    attributes.background_pixel  = new->core.background_pixel;
	    attributes.border_pixmap     = new->core.border_pixmap;
	    attributes.border_pixel      = new->core.border_pixel;
	    XChangeWindowAttributes(
		XtDisplay(new), XtWindow(new), window_mask, &attributes);
	}
	if (old->core.mapped_when_managed != new->core.mapped_when_managed)
	    XtSetMappedWhenManaged(new, new->core.mapped_when_managed);

	/* Translation table and state */
	if (old->core.tm.translations != new->core.tm.translations) {
	    XtFree((char *)old->core.tm.proc_table);
	    /* ||| _XtUninstallTranslations(old, old->core.tm.translations); */
	    new->core.tm.proc_table = NULL;
	    _XtBindActions(new, &new->core.tm, 0);
	    _XtInstallTranslations(new, new->core.tm.translations);
	}
    } /* if realized */

    return redisplay;
} /* CoreSetValues */

/*ARGSUSED*/
static void CoreSetValuesAlmost(old, new, request, reply)
    Widget		old;
    Widget		new;
    XtWidgetGeometry    *request;
    XtWidgetGeometry    *reply;
{
    *request = *reply;
}

