#ifndef lint
static char rcsid[] = "$Header: Grip.c,v 1.14 88/02/26 12:12:02 swick Exp $";
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


/*
 * Grip.c - Grip composite Widget (Used by VPane Widget)
 *
 */


#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
#include <X11/GripP.h>

static int defDim = 8;
static int defBorder = 0;

static XtResource resources[] = {
   {XtNwidth, XtCWidth, XtRInt, sizeof(int),
      XtOffset(GripWidget, core.width), XtRInt, (caddr_t)&defDim},
   {XtNheight, XtCHeight, XtRInt, sizeof(int),
      XtOffset(GripWidget, core.height), XtRInt, (caddr_t)&defDim},
   {XtNbackground, XtCBackground, XtRPixel, sizeof(Pixel),
      XtOffset(GripWidget, core.background_pixel), XtRString, "Black"},
   {XtNborderWidth, XtCBorderWidth, XtRInt, sizeof(int),
      XtOffset(GripWidget, core.border_width), XtRInt, (caddr_t)&defBorder},
   {XtNcallback, XtCCallback, XtRCallback, sizeof(caddr_t), 
      XtOffset(GripWidget, grip.grip_action), XtRCallback, NULL},
};

void GripAction( /* Widget, XEvent*, String*, Cardinal */ );

static XtActionsRec actionsList[] =
{
  {"GripAction",      GripAction},
};


GripClassRec gripClassRec = {
   {
/* core class fields */
    /* superclass         */   (WidgetClass) &widgetClassRec,
    /* class name         */   "Grip",
    /* size               */   sizeof(GripRec),
    /* class initialize   */   NULL,
    /* class_part_init    */   NULL,
    /* class_inited       */   FALSE,
    /* initialize         */   NULL,
    /* initialize_hook    */   NULL,
    /* realize            */   XtInheritRealize,
    /* actions            */   actionsList,
    /* num_actions        */   XtNumber(actionsList),
    /* resourses          */   resources,
    /* resource_count     */   XtNumber(resources),
    /* xrm_class          */   NULLQUARK,
    /* compress_motion    */   TRUE,
    /* compress_exposure  */   TRUE,
    /* compress_enterleave*/   TRUE,
    /* visible_interest   */   FALSE,
    /* destroy            */   NULL,
    /* resize             */   NULL,
    /* expose             */   NULL,
    /* set_values         */   NULL,
    /* set_values_hook    */   NULL,
    /* set_values_almost  */   XtInheritSetValuesAlmost,
    /* get_values_hook    */   NULL,
    /* accept_focus       */   NULL,
    /* version            */   XtVersion,
    /* callback_private   */   NULL,
    /* tm_table           */   NULL,
    /* query_geometry     */   NULL,
   }, {
    /* empty              */   0  /* make C compiler happy */
   }
};

WidgetClass gripWidgetClass = (WidgetClass) &gripClassRec;

static void GripAction( widget, event, params, num_params )
    Widget widget;
    XEvent *event;
    String *params;
    Cardinal *num_params;
{
    GripCallDataRec call_data;

    call_data.event = event;
    call_data.params = params;
    call_data.num_params = *num_params;

    XtCallCallbacks( widget, XtNcallback, (caddr_t)&call_data );
}
