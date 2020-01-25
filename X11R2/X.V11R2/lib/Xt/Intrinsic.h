/*
* $Header: Intrinsic.h,v 1.76 88/02/26 12:40:57 swick Exp $
*/

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

#ifndef _XtIntrinsic_h
#define _XtIntrinsic_h
#include	<X11/Xlib.h>
#include	<X11/Xresource.h>
#include	<sys/types.h>

#ifndef NULL
#define NULL 0
#endif

#define globalref extern
#define globaldef

#ifndef FALSE
#define FALSE 0
#define TRUE  1
#endif

#define XtNumber(arr)		((Cardinal) (sizeof(arr) / sizeof(arr[0])))
#define XtOffset(type,field)    ((unsigned int)&(((type)NULL)->field))

typedef char *String;
typedef struct _WidgetRec *Widget;
typedef Widget *WidgetList;
typedef struct _WidgetClassRec *WidgetClass;
typedef struct _CompositeRec *CompositeWidget;
typedef struct _XtActionsRec *XtActionList;
typedef struct _XtResource *XtResourceList;
typedef unsigned int   Cardinal;
typedef char	Boolean;
typedef unsigned long	*Opaque;
typedef struct _TranslationData	*XtTranslations;
typedef struct _XtCallbackRec*    XtCallbackList;
typedef unsigned long   XtValueMask;
typedef unsigned long   XtIntervalId;
typedef unsigned long   XtInputId;
typedef unsigned int    XtGeometryMask;
typedef unsigned long   XtGCMask;   /* Mask of values that are used by widget*/
typedef unsigned long   Pixel;	    /* Index into colormap	        */
typedef int		Position;   /* Offset from 0 coordinate	        */
typedef unsigned int	Dimension;  /* Size in pixels		        */

/****************************************************************
 *
 * System Dependent Definitions
 *
 *
 * XtArgVal ought to be a union of caddr_t, char *, long, int *, and proc *
 * but casting to union types is not really supported.
 *
 * So the typedef for XtArgVal should be chosen such that
 *
 *      sizeof (XtArgVal) >=	sizeof(caddr_t)
 *				sizeof(char *)
 *				sizeof(long)
 *				sizeof(int *)
 *				sizeof(proc *)
 *
 * ArgLists rely heavily on the above typedef.
 *
 ****************************************************************/
typedef long XtArgVal;

#include <X11/Core.h>
#include <X11/Composite.h>
#include <X11/Constraint.h>

extern Boolean XtIsSubclass ();
    /* Widget       widget;	    */
    /* WidgetClass  widgetClass;    */

#define XtIsComposite(widget)	XtIsSubclass((widget), compositeWidgetClass)
#define XtMapWidget(widget)	XMapWindow(XtDisplay(widget), XtWindow(widget))
#define XtUnmapWidget(widget)	\
		XUnmapWindow(XtDisplay(widget), XtWindow(widget))

extern Widget XtCreateWidget ();
    /* String	    name;	    */
    /* WidgetClass  widget_class;    */
    /* Widget       parent;	    */
    /* ArgList      args;	    */
    /* Cardinal     num_args;       */

extern Widget XtCreateManagedWidget ();
    /* String	    name;	    */
    /* WidgetClass  widget_class;    */
    /* Widget       parent;	    */
    /* ArgList      args;	    */
    /* Cardinal     num_args;       */

extern Widget XtCreateApplicationShell ();
    /* String	    name;	    */
    /* WidgetClass  widget_class;    */
    /* ArgList      args;	    */
    /* Cardinal     num_args;       */

extern void XtRealizeWidget ();
    /* Widget    widget      */

extern Boolean XtIsRealized ();
    /* Widget    widget; */

extern void XtDestroyWidget ();
    /* Widget widget */

extern void XtSetSensitive ();
    /* Widget    widget;    */
    /* Boolean   sensitive; */

extern void XtSetMappedWhenManaged ();
    /* Widget    widget;    */
    /* Boolean   mappedWhenManaged; */

extern Widget XtNameToWidget(); /* root, name */
    /* Widget root; */
    /* String name; */

extern Widget XtWindowToWidget(); /* window */
    /* Display *display; */
    /* Window window; */

/**********************************************************
 *
 * Composite widget Procedures
 *
 **********************************************************/

extern void XtManageChildren ();
    /* WidgetList children; */
    /* Cardinal   num_children; */

extern void XtManageChild ();
    /* Widget    child; */

extern void XtUnmanageChildren ();
    /* WidgetList children; */
    /* Cardinal   num_children; */

extern void XtUnmanageChild ();
    /* Widget child; */

/*************************************************************
 *
 *  Callbacks
 *
 **************************************************************/

typedef void (*XtCallbackProc)();
    /* Widget widget; */
    /* caddr_t closure;  data the application registered */
    /* caddr_t callData; widget instance specific data passed to application*/

typedef struct _XtCallbackRec {
    XtCallbackProc  callback;
    caddr_t	    closure;
} XtCallbackRec;

extern void XtAddCallback ();
    /* Widget		widget;		*/
    /* String		callback_name;  */
    /* XtCallbackProc   callback;       */
    /* caddr_t		closure;	*/

extern void XtRemoveCallback ();
    /* Widget		widget;		*/
    /* String		callback_name;  */
    /* XtCallbackProc   callback;       */
    /* caddr_t		closure;	*/

extern void XtAddCallbacks ();
    /* Widget		widget;		*/
    /* String		callback_name;  */
    /* XtCallbackList   callbacks;       */

extern void XtRemoveCallbacks ();
    /* Widget		widget;		*/
    /* String		callback_name;  */
    /* XtCallbackList   callbacks;       */

extern void XtRemoveAllCallbacks ();
    /* Widget		widget;		*/
    /* String		callback_name;  */

extern void XtCallCallbacks ();
    /* Widget		widget;		*/
    /* String		callback_name;   */
    /* caddr_t		callData;	*/

/* These utilities are here on Leo's request. We should think about them */
/* and decide if they really belong in the intrinsics, or a level above */

/* In particular, this procedure is nasty... */
extern void XtOverrideCallback(); /* widget, callcallback */
    /* Widget		widget; */
    /* String		callback_name; */
    /* XtCallbackProc	callback; */

typedef enum {
	XtCallbackNoList,
	XtCallbackHasNone,
	XtCallbackHasSome
} XtCallbackStatus;

extern XtCallbackStatus XtHasCallbacks(); /* widget, callback_name */
    /* Widget		widget; */
    /* String		callback_name; */

/****************************************************************
 *
 * Toolkit initialization
 *
 ****************************************************************/

extern Widget XtInitialize();
    /* String    	    name;       */
    /* String		    class;      */
    /* XrmOptionsDescRec    options;    */
    /* Cardinal             num_options;  */
    /* Cardinal		    *argc; */ /* returns count of args not processed */
    /* char		    **argv;     */

/****************************************************************
 *
 * Memory Management
 *
 ****************************************************************/

#define XtNew(type) ((type *) XtMalloc((unsigned) sizeof(type)))

extern char *XtMalloc(); /* size */
    /* Cardinal size; */

extern char *XtCalloc(); /* num, size */
    /* Cardinal num, size; */

extern char *XtRealloc(); /* ptr, num */
    /* char     *ptr; */
    /* Cardinal num; */

extern void XtFree(); /* ptr */
	/* char  *ptr */


/****************************************************************
 *
 * Arg lists
 *
 ****************************************************************/

typedef struct {
    String	name;
    XtArgVal	value;
} Arg, *ArgList;

#define XtSetArg(arg, n, d) \
    ( (arg).name = (n), (arg).value = (XtArgVal)(d) )

extern ArgList XtMergeArgLists(); /* args1, num_args1, args2, num_args2 */
    /* ArgList  args1;      */
    /* Cardinal num_args1;  */
    /* ArgList  args2;      */
    /* Cardinal num_args2;  */

/****************************************************************
 *
 * Event Management
 *
 ****************************************************************/

typedef void (*XtEventHandler)(); /* widget, closure, event */
    /* Widget  widget   */
    /* caddr_t closure  */
    /* XEvent  *event;  */

typedef unsigned long EventMask;
#define XtAllEvents ((EventMask) -1L)

extern void XtAddEventHandler(); /* widget, eventMask, nonmaskable, proc, closure */
    /* Widget		widget      */
    /* EventMask        eventMask;  */
    /* Boolean          nonmaskable; */
    /* XtEventHandler   proc;       */
    /* caddr_t		closure ;   */


extern void XtRemoveEventHandler(); /* widget, eventMask, nonmaskable, proc, closure */
    /* Widget		widget      */
    /* EventMask        eventMask;  */
    /* Boolean          nonmaskable;*/
    /* XtEventHandler   proc;       */
    /* caddr_t		closure ;   */


extern void XtAddRawEventHandler(); /* widget, eventMask, nonmaskable, proc, closure */
    /* Widget		widget      */
    /* EventMask        eventMask;  */
    /* Boolean          nonmaskable; */
    /* XtEventHandler   proc;       */
    /* caddr_t		closure ;   */


extern void XtRemoveRawEventHandler(); /* widget, eventMask, nonmaskable, proc, closure */
    /* Widget		widget      */
    /* EventMask        eventMask;  */
    /* Boolean          nonmaskable;*/
    /* XtEventHandler   proc;       */
    /* caddr_t		closure ;   */


extern void XtAddGrab();
    /* Widget   widget;		    */
    /* Boolean  exclusive;	    */
    /* Boolean  spring_loaded;      */

extern void XtRemoveGrab();
    /* Widget   widget;		    */

extern void XtDispatchEvent(); /* event */
    /* XEvent	*event; */

extern void XtMainLoop();

/****************************************************************
 *
 * Event Gathering Routines
 *
 ****************************************************************/

typedef unsigned long	XtInputMask;

#define XtInputNoneMask		0L
#define XtInputReadMask		(1L<<0)
#define XtInputWriteMask	(1L<<1)
#define XtInputExceptMask	(1L<<2)

extern Atom XtHasInput;
extern Atom XtTimerExpired;

typedef void (*XtTimerCallbackProc)();
    /* opaque closure         */
    /* XtIntervalId   *id      */

extern XtIntervalId XtAddTimeOut();
    /* unsigned long interval */
    /* XtTimerCallbackProc  proc  */
    /* Opaque   closure       */

extern void XtRemoveTimeOut();
    /* XtIntervalId timer;      */

typedef void (* XtInputCallbackProc)();
    /* Opaque closure;          */
    /* int    *source;           */
    /* XtInputId  *id;           */

extern XtInputId XtAddInput(); /* source, condition, proc, closure */
    /* int source;		*/
    /* Opaque condition;        */
    /* XtInputCallbackProc proc;*/
    /* Opaque closure;          */

extern void XtRemoveInput(); /* id */
    /* XtInputid id;		*/

extern void XtNextEvent(); /* event */
    /* XtEvent *event;		*/

extern XtPeekEvent(); /* event */
    /* XtEvent *event;		*/

extern Boolean XtPending ();


/****************************************************************
 *
 * Geometry Management
 *
 ****************************************************************/

#define XtSMDontChange	5 /* don't change the stacking order stack_mode */

typedef struct {
    XtGeometryMask request_mode;
    Position x, y;
    Dimension width, height, border_width;
    Widget sibling;
    int stack_mode;	/* Above, Below, TopIf, BottomIf, Opposite */
} XtWidgetGeometry;

typedef enum  {
    XtGeometryYes,        /* Request accepted. */
    XtGeometryNo,         /* Request denied. */
    XtGeometryAlmost,     /* Request denied, but willing to take replyBox. */
    XtGeometryDone,	  /* Request accepted and done. */
} XtGeometryResult;

extern XtGeometryResult XtMakeGeometryRequest();
    /*  widget, request, reply		    */
    /* Widget	widget; 		    */
    /* XtWidgetGeometry    *request;	    */
    /* XtWidgetGeometry	 *reply; *//* RETURN */

extern XtGeometryResult XtMakeResizeRequest ();
    /* Widget    widget;	*/
    /* Dimension width, height; */
    /* Dimension *replyWidth, *replyHeight; */

extern void XtTransformCoords();
    /* register Widget w;	*/
    /* Position x, y;		*/
    /* register Position *rootx, *rooty; */

extern XtGeometryResult XtQueryGeometry();
    /* Widget	widget;			*/
    /* XtWidgetGeometry *intended;	*/
    /* XtWidgetGeometry *reply_return;	*/


/****************************************************************
 *
 * Widget resource initialization, reading, writing
 *
 ****************************************************************/

typedef struct _XtResource {
    String     resource_name;	/* Resource name			    */
    String     resource_class;	/* Resource class			    */
    String     resource_type;	/* Representation type desired		    */
    Cardinal    resource_size;	/* Size in bytes of representation	    */
    Cardinal    resource_offset;/* Offset from base to put resource value   */
    String     default_type;	/* representation type of specified default */
    caddr_t     default_addr;   /* Address of default resource		    */
} XtResource;

extern void XtGetSubresources();
    /* Widget		widget;		*/
    /* caddr_t		base;		*/
    /* String		name;		*/
    /* String		class;		*/
    /* XtResourceList   resources;      */
    /* Cardinal		num_resources;  */
    /* ArgList		args;		*/
    /* Cardinal		num_args;       */

extern void XtSetValues(); 
    /* Widget           widget;         */
    /* ArgList		args;		*/
    /* Cardinal		num_args;       */

extern void XtGetValues();
    /* Widget           widget;         */
    /* ArgList		args;		*/
    /* Cardinal 	num_args;       */

extern void XtSetSubvalues(); 
    /* caddr_t          base;		*/
    /* XtResourceList   resources;      */
    /* Cardinal		num_resources;  */
    /* ArgList		args;		*/
    /* Cardinal		num_args;       */

extern void XtGetSubvalues(); 
    /* caddr_t          base;		*/
    /* XtResourceList   resources;      */
    /* Cardinal		num_resources;  */
    /* ArgList		args;		*/
    /* Cardinal		num_args;       */


/****************************************************************
 *
 * Translation Management
 *
 ****************************************************************/


typedef void (*XtActionProc)();
    /* Widget widget; */
    /* XEvent *event; */
    /* String *params; */
    /* Cardinal *num_params; */

typedef struct _XtActionsRec{
    char    *string;
    XtActionProc proc;
} XtActionsRec;

extern XtTranslations XtParseTranslationTable(); /* source */
    /* String *source; */

extern void XtFreeTranslations(); /* stateTable */
    /* XtTranslations stateTable; */

extern void XtOverrideTranslations(); /* widget, new */
    /* Widget widget; */
    /* XtTranslations new; */

extern void XtAugmentTranslations(); /* widget, new */
    /* Widget widget; */
    /* XtTranslations new; */

extern void XtAddActions(); /* action, num_actions */
   /* XtActionList action */
   /* Cardinal num_actions */

/*************************************************************
 *
 * Error Handling
 *
 ************************************************************/

extern void XtError();  /* message */
    /* String message */

extern void XtWarning();  /* message */
    /* String message */

extern void XtSetErrorHandler(); /* errorProc */
  /* (*errorProc)(String); */

extern void XtSetWarningHandler(); /* errorProc */
  /* (*errorProc)(String); */

/*************************************************************
 *
 * popup Handling
 *
 ************************************************************/

extern Widget XtCreatePopupShell();

    /* String	    name;	    */
    /* WidgetClass  widgetClass;    */
    /* Widget       parent;	    */
    /* ArgList      args;	    */
    /* Cardinal     num_args;       */

typedef enum {XtGrabNone, XtGrabNonexclusive, XtGrabExclusive} XtGrabKind;

extern void XtPopup();
    /* Widget       widget;	    */
    /* XtGrabKind   grab_kind;      */

extern void XtCallbackNone();
    /* Widget       widget;	    */
    /* caddr_t      closure;	    */
    /* caddr_t      call_data;      */

extern void XtCallbackNonexclusive();
    /* Widget       widget;	    */
    /* caddr_t      closure;	    */
    /* caddr_t      call_data;      */

extern void XtCallbackExclusive();
    /* Widget       widget;	    */
    /* caddr_t      closure;	    */
    /* caddr_t      call_data;      */

extern void XtPopdown();
    /* Widget       widget;	    */

typedef struct {
    Widget  shell_widget;
    Widget  enable_widget;
} XtPopdownIDRec, *XtPopdownID;
    
extern void XtCallbackPopdown();
    /* Widget       widget;	    */
    /* caddr_t      closure;	    */
    /* caddr_t      call_data;      */

/****************************************************************
 *
 * Graphic Context Management
 *****************************************************************/

extern GC XtGetGC(); /* widget, valueMask, values */
    /* Widget    widget */
    /* XtGCMask valueMask; */
    /* XGCValues *values; */

extern void XtDestroyGC (); /* gc */
    /* GC gc; */

/*************************************************************
 *
 * Misc
 *
 ************************************************************/

extern Cursor XtGetCursor(); /* dpy, num */
    /* Display *dpy */
    /* int num */

/*************************************************************
 *
 * Information routines
 *
 ************************************************************/

#ifndef _XtIntrinsicP_h

/* We're not included from the private file, so define these */

extern Display *XtDisplay();
    /*	Widget widget;		*/

extern Screen *XtScreen();
    /*	Widget widget;		*/

extern Window XtWindow();
    /*	Widget widget;		*/

extern WidgetClass XtSuperclass();
    /*	Widget widget;		*/

extern WidgetClass XtClass();
    /*	Widget widget;		*/

extern Boolean XtIsManaged();
    /*	Widget widget;		*/

extern Boolean XtIsRealized();
    /*	Widget widget;		*/

extern Widget XtParent();
    /*  Widget widget;		*/

#endif _XtIntrinsicP_h

#endif _XtIntrinsic_h
/* DON'T ADD STUFF AFTER THIS #endif */
