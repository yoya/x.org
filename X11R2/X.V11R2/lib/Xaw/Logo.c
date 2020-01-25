#ifndef lint
static char rcsid[] = "$Header: Logo.c,v 1.2 88/02/26 12:13:44 swick Exp $";
#endif

/*
Copyright 1988 by the Massachusetts Institute of Technology

Permission to use, copy, modify, and distribute this
software and its documentation for any purpose and without
fee is hereby granted, provided that the above copyright
notice appear in all copies and that both that copyright
notice and this permission notice appear in supporting
documentation, and that the name of M.I.T. not be used in
advertising or publicity pertaining to distribution of the
software without specific, written prior permission.
M.I.T. makes no representations about the suitability of
this software for any purpose.  It is provided "as is"
without express or implied warranty.
*/

#include <X11/Xlib.h>
#include <X11/StringDefs.h>
#include <X11/IntrinsicP.h>
#include <X11/LogoP.h>

static XtResource resources[] = {
    {XtNwidth, XtCWidth, XtRInt, sizeof(int),
	XtOffset(Widget,core.width), XtRString, "100"},
    {XtNheight, XtCHeight, XtRInt, sizeof(int),
	XtOffset(Widget,core.height), XtRString, "100"},
    {XtNforeground, XtCForeground, XtRPixel, sizeof(Pixel),
        XtOffset(LogoWidget,logo.fgpixel), XtRString, "Black"},
    {XtNreverseVideo, XtCReverseVideo, XtRBoolean, sizeof (Boolean),
	XtOffset(LogoWidget,logo.reverse_video), XtRString, "FALSE"},
};

static void Initialize(), Realize(), Destroy(), Redisplay();
static Boolean SetValues();

LogoClassRec logoClassRec = {
    { /* core fields */
    /* superclass		*/	&widgetClassRec,
    /* class_name		*/	"Logo",
    /* widget_size		*/	sizeof(LogoRec),
    /* class_initialize		*/	NULL,
    /* class_part_initialize	*/	NULL,
    /* class_inited		*/	FALSE,
    /* initialize		*/	Initialize,
    /* initialize_hook		*/	NULL,
    /* realize			*/	Realize,
    /* actions			*/	NULL,
    /* num_actions		*/	0,
    /* resources		*/	resources,
    /* resource_count		*/	XtNumber(resources),
    /* xrm_class		*/	NULL,
    /* compress_motion		*/	TRUE,
    /* compress_exposure	*/	TRUE,
    /* compress_enterleave	*/	TRUE,
    /* visible_interest		*/	FALSE,
    /* destroy			*/	Destroy,
    /* resize			*/	NULL,
    /* expose			*/	Redisplay,
    /* set_values		*/	SetValues,
    /* set_values_hook		*/	NULL,
    /* set_values_almost	*/	XtInheritSetValuesAlmost,
    /* get_values_hook		*/	NULL,
    /* accept_focus		*/	NULL,
    /* version			*/	XtVersion,
    /* callback_private		*/	NULL,
    /* tm_table			*/	NULL,
    /* query_geometry		*/	NULL,
    }
};

WidgetClass logoWidgetClass = (WidgetClass) &logoClassRec;

/* ARGSUSED */
static void Initialize (request, new)
    Widget request, new;
{
    LogoWidget w = (LogoWidget)new;
    XGCValues  gcv;

    if (w->logo.reverse_video) {
	Pixel fg = w->logo.fgpixel;
	Pixel bg = w->core.background_pixel;

	if (w->core.border_pixel == fg)
	    w->core.border_pixel = bg;
	w->logo.fgpixel = bg;
	w->core.background_pixel = fg;
    }

    gcv.foreground = w->logo.fgpixel;
    w->logo.foreGC = XtGetGC((Widget)w, GCForeground, &gcv);
    gcv.foreground = w->core.background_pixel;
    w->logo.backGC = XtGetGC((Widget)w, GCForeground, &gcv);
}

static void Realize (gw, valueMask, attrs)
    Widget gw;
    XtValueMask *valueMask;
    XSetWindowAttributes *attrs;
{
    XtCreateWindow( gw, InputOutput, (Visual *)CopyFromParent,
		    *valueMask, attrs);
}

static void Destroy (gw)
    Widget gw;
{
    LogoWidget w = (LogoWidget) gw;
    XtDestroyGC (w->logo.foreGC);
    XtDestroyGC (w->logo.backGC);
}

/* ARGSUSED */
static void Redisplay (gw, event, region)
    Widget gw;
    XEvent *event;		/* unused */
    Region region;		/* unused */
{
    LogoWidget w = (LogoWidget) gw;
    register Display *dpy = XtDisplay(w);
    register Window win = XtWindow(w);

    XDrawLogo(XtDisplay(w), XtWindow(w), w->logo.foreGC, w->logo.backGC,
	      0, 0, w->core.width, w->core.height);
}

/* ARGSUSED */
static Boolean SetValues (gcurrent, grequest, gnew)
    Widget gcurrent, grequest, gnew;
{
    LogoWidget current = (LogoWidget) gcurrent;
    LogoWidget new = (LogoWidget) gnew;
    Boolean redisplay = FALSE;
    XGCValues	gcv;

    if ((new->logo.fgpixel != current->logo.fgpixel) ||
	(new->core.background_pixel != current->core.background_pixel)) {
	XtDestroyGC (current->logo.foreGC);
	XtDestroyGC (current->logo.backGC);
	gcv.foreground = new->logo.fgpixel;
	new->logo.foreGC = XtGetGC(gcurrent, GCForeground, &gcv);
	gcv.foreground = new->core.background_pixel;
	new->logo.backGC = XtGetGC(gcurrent, GCForeground, &gcv);
	redisplay = TRUE;
    }
   
   return (redisplay);
}
