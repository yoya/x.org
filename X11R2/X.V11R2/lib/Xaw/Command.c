#ifndef lint
static char rcsid[] = "$Header: Command.c,v 1.34 88/02/26 10:34:47 swick Exp $";
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
 * Command.c - Command button widget
 *
 */

#define XtStrlen(s)	((s) ? strlen(s) : 0)

  /* The following are defined for the reader's convenience.  Any
     Xt..Field macro in this code just refers to some field in
     one of the substructures of the WidgetRec.  */

#include <stdio.h>
#include <X11/Xos.h>
#include <ctype.h>
#include <X11/StringDefs.h>
#include <X11/IntrinsicP.h>
#include <X11/Misc.h>
#include <X11/CommandP.h>
#include "CommandI.h"

/****************************************************************
 *
 * Full class record constant
 *
 ****************************************************************/

/* Private Data */

static char defaultTranslations[] =
    "<Btn1Down>:	set() \n\
     <Btn1Up>:		notify() unset() \n\
     <EnterWindow>:	highlight() \n\
     <LeaveWindow>:	unset(NoRedisplay) unhighlight()";

static int defHighlight = 2;

#define offset(field) XtOffset(CommandWidget, field)
static XtResource resources[] = { 

   {XtNcallback, XtCCallback, XtRCallback, sizeof(caddr_t), 
      offset(command.callbacks), XtRCallback, (caddr_t)NULL},
   {XtNcursor, XtCCursor, XtRCursor, sizeof(Cursor),
      offset(simple.cursor), XtRString, "opendot"},
   {XtNhighlightThickness, XtCThickness, XtRInt, sizeof(Dimension),
      offset(command.highlight_thickness), XtRInt, (caddr_t)&defHighlight},
};
#undef offset

static XtActionsRec actionsList[] =
{
  {"set",		Set},
  {"notify",		Notify},
  {"highlight",		Highlight},
  {"unset",		Unset},
  {"unhighlight",	Unhighlight},
};

  /* ...ClassData must be initialized at compile time.  Must
     initialize all substructures.  (Actually, last two here
     need not be initialized since not used.)
  */
CommandClassRec commandClassRec = {
  {
    (WidgetClass) &labelClassRec,	/* superclass		  */	
    "Command",				/* class_name		  */
    sizeof(CommandRec),			/* size			  */
    NULL,				/* class_initialize	  */
    NULL,				/* class_part_initialize  */
    FALSE,				/* class_inited		  */
    Initialize,				/* initialize		  */
    NULL,				/* initialize_hook	  */
    XtInheritRealize,			/* realize		  */
    actionsList,			/* actions		  */
    XtNumber(actionsList),		/* num_actions		  */
    resources,				/* resources		  */
    XtNumber(resources),		/* resource_count	  */
    NULLQUARK,				/* xrm_class		  */
    FALSE,				/* compress_motion	  */
    TRUE,				/* compress_exposure	  */
    TRUE,				/* compress_enterleave    */
    FALSE,				/* visible_interest	  */
    Destroy,				/* destroy		  */
    XtInheritResize,			/* resize		  */
    Redisplay,				/* expose		  */
    SetValues,				/* set_values		  */
    NULL,				/* set_values_hook	  */
    XtInheritSetValuesAlmost,		/* set_values_almost	  */
    NULL,				/* get_values_hook	  */
    NULL,				/* accept_focus		  */
    XtVersion,				/* version		  */
    NULL,				/* callback_private	  */
    defaultTranslations,		/* tm_table		  */
    NULL,				/* query_geometry	  */
  },  /* CoreClass fields initialization */
  {
    0,                                     /* field not used    */
  },  /* LabelClass fields initialization */
  {
    0,                                     /* field not used    */
  },  /* CommandClass fields initialization */
};

  /* for public consumption */
WidgetClass commandWidgetClass = (WidgetClass) &commandClassRec;

/****************************************************************
 *
 * Private Procedures
 *
 ****************************************************************/

static void Get_inverseGC(cbw)
    CommandWidget cbw;
{
    XGCValues	values;

    /* Set up a GC for inverse (set) state */

    values.foreground   = ComWforeground;
    values.font		= ComWfont->fid;
    values.fill_style   = FillSolid;

    ComWinverseGC = XtGetGC((Widget)cbw,
    	(unsigned) GCForeground | GCFont | GCFillStyle, &values);
}

static void Get_inverseTextGC(cbw)
    CommandWidget cbw;
{
    XGCValues	values;

    /* Set up a GC for inverse (set) state */

    values.foreground   = ComWbackground; /* default is White */
    values.font		= ComWfont->fid;
    values.fill_style   = FillSolid;

    ComWinverseTextGC = XtGetGC((Widget)cbw,
    	(unsigned) GCForeground | GCFont | GCFillStyle, &values);
}

static void Get_highlightGC(cbw)
    CommandWidget cbw;
{
    XGCValues	values;
    
    /* Set up a GC for highlighted state.  It has a thicker
       line width for the highlight border */

    values.foreground   = ComWforeground;
    values.line_width   = ComWhighlightThickness > 1
			  ? ComWhighlightThickness : 0;

    ComWhighlightGC = XtGetGC((Widget)cbw,
    	(unsigned) GCForeground | GCLineWidth, &values);
}


/* ARGSUSED */
static void Initialize(request, new)
 Widget request, new;
{
    CommandWidget cbw = (CommandWidget) new;

    Get_inverseGC(cbw);
    Get_inverseTextGC(cbw);
    Get_highlightGC(cbw);

      /* init flags for state */
    ComWset = FALSE;
    ComWhighlighted = FALSE;  
    ComWdisplayHighlighted = FALSE;
    ComWdisplaySet = FALSE;

} 

/***************************
*
*  Action Procedures
*
***************************/

/* ARGSUSED */
static void Set(w,event,params,num_params)
     Widget w;
     XEvent *event;
     String *params;		/* unused */
     Cardinal *num_params;	/* unused */
{
  CommandWidget cbw = (CommandWidget)w;
  ComWset = TRUE;
  Redisplay(w, event, NULL);
}

/* ARGSUSED */
static void Unset(w,event,params,num_params)
     Widget w;
     XEvent *event;
     String *params;		/* unused */
     Cardinal *num_params;
{
  CommandWidget cbw = (CommandWidget)w;
  ComWset = FALSE;
  if (*num_params == 0)
      Redisplay(w, event, NULL);
}

/* ARGSUSED */
static void Highlight(w,event,params,num_params)
     Widget w;
     XEvent *event;
     String *params;		/* unused */
     Cardinal *num_params;	/* unused */
{
  CommandWidget cbw = (CommandWidget)w;
  ComWhighlighted = TRUE;
  Redisplay(w, event, NULL);
}

/* ARGSUSED */
static void Unhighlight(w,event,params,num_params)
     Widget w;
     XEvent *event;
     String *params;		/* unused */
     Cardinal *num_params;	/* unused */
{
  CommandWidget cbw = (CommandWidget)w;
  ComWhighlighted = FALSE;
  Redisplay(w, event, NULL);
}

/* ARGSUSED */
static void Notify(w,event,params,num_params)
     Widget w;
     XEvent *event;
     String *params;		/* unused */
     Cardinal *num_params;	/* unused */
{
  CommandWidget cbw = (CommandWidget)w;
  if (ComWset)
      XtCallCallbacks(w, XtNcallback, NULL);
}
/*
 * Repaint the widget window
 */

/************************
*
*  REDISPLAY (DRAW)
*
************************/

/* ARGSUSED */
static void Redisplay(w, event, region)
    Widget w;
    XEvent *event;		/* unused */
    Region region;		/* unused */
{
   CommandWidget cbw = (CommandWidget) w;
   Boolean very_thick = ComWhighlightThickness > Min(ComWwidth,ComWheight)/2;

   /* Here's the scoop:  If the command button button is normal,
      you show the text.  If the command button is highlighted but 
      not set, you draw a thick border and show the text.
      If the command button is set, draw the button and text
      in inverse. */

   /* Note that Redisplay must remember the state of its last
      draw to determine whether to erase the window before
      redrawing to avoid flicker.  If the state is the same,
      the window just needs to redraw (even on an expose). */

   if ((!ComWhighlighted && ComWdisplayHighlighted) ||
       (!ComWset && ComWdisplaySet))
     XClearWindow(XtDisplay(w),XtWindow(w));
     /* Don't clear the window if the button's in a set condition;
	instead, fill it with black to avoid flicker. (Must fill
	with black in case it was an expose */
   else if (ComWset)
     XFillRectangle(XtDisplay(w),XtWindow(w), ComWinverseGC,
		    0,0,ComWwidth,ComWheight);

   if (ComWhighlighted && ComWhighlightThickness > 0) {
       if (very_thick)
	   XFillRectangle(XtDisplay(w),XtWindow(w), ComWinverseGC,
			  0,0,ComWwidth,ComWheight);
       else {
	   /* wide lines are centered on the path, so indent it */
	   int offset = ComWhighlightThickness/2;
	   XDrawRectangle(XtDisplay(w),XtWindow(w), ComWhighlightGC,
			  offset, offset,
			  ComWwidth - ComWhighlightThickness,
			  ComWheight - ComWhighlightThickness);
       }
   }

     /* draw the string:  there are three different "styles" for it,
	all in separate GCs */
   XDrawString(XtDisplay(w),XtWindow(w),
	       (ComWset || (ComWhighlighted && very_thick)
		  ?  ComWinverseTextGC : 
		    (ComWsensitive ? ComWnormalGC : ComWgrayGC)),
		ComWlabelX, ComWlabelY, ComWlabel, (int) ComWlabelLen);

   ComWdisplayHighlighted = ComWhighlighted;
   ComWdisplaySet = ComWset;
}


/* ARGSUSED */
static void Destroy(w)
    Widget w;
{
  /* must free GCs and pixmaps */
}


/*
 * Set specified arguments into widget
 */
/* ARGSUSED */
static Boolean SetValues (current, request, new)
    Widget current, request, new;
{
    CommandWidget cbw = (CommandWidget) current;
    CommandWidget newcbw = (CommandWidget) new;

    if (XtCField(newcbw,sensitive) != ComWsensitive &&
	!XtCField(newcbw,sensitive)) {  /* about to become insensitive? */
	XtCBField(newcbw,set) = FALSE;	/* yes, then anticipate unset */
	XtCBField(newcbw,highlighted) = FALSE;
    }

     if (XtLField(newcbw,foreground) != ComWforeground)
       {
         XtDestroyGC(ComWinverseGC);
	 Get_inverseGC(newcbw);
         XtDestroyGC(ComWhighlightGC);
	 Get_highlightGC(newcbw);
       }
    else 
      {
	if (XtCField(newcbw,background_pixel) != ComWbackground ||
	     XtLField(newcbw,font) != ComWfont) {
	     XtDestroyGC(ComWinverseTextGC);
	     Get_inverseTextGC(newcbw);
	     }
	if (XtCBField(newcbw,highlight_thickness) != ComWhighlightThickness) {
	    XtDestroyGC(ComWhighlightGC);
	    Get_highlightGC(newcbw);
	}
      }
     
    /*  NEED TO RESET PROC AND CLOSURE */

     /* ACTIONS */
    /* Change Label to remove ClearWindow and Redisplay */
    /* Change Label to change GCs if foreground, etc */

    return (XtCField(newcbw, sensitive) != ComWsensitive ||
	    XtCBField(newcbw, set) != ComWset ||
	    XtCBField(newcbw, highlighted) != ComWhighlighted);
}
