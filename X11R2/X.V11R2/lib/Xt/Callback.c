#ifndef lint
static char rcsid[] = "$Header: Callback.c,v 1.4 88/02/26 12:28:36 swick Exp $";
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
#include "Resource.h"

extern CallbackList _CompileCallbackList();


typedef struct _CallbackRec {
    CallbackList  next;
    Widget	    widget;
    XtCallbackProc  callback;
    caddr_t	    closure;
} CallbackRec;

static CallbackList *FetchCallbackList (widget, name)
    Widget  widget;
    String  name;
{
    register _XtOffsetList  offsets;
    register XrmQuark       quark;

    quark = StringToQuark(name);
    for (offsets = widget->core.widget_class->core_class.callback_private;
	 offsets != NULL;
	 offsets = offsets->next) {
	if (quark == offsets->name) {
	    return((CallbackList *) ((char *) widget - offsets->offset - 1));
	}
    }
    return(NULL);
} /* FetchCallbackList */


void _XtAddCallback(widget, callbacks, callback, closure)
    Widget		    widget;
    register CallbackList   *callbacks;
    XtCallbackProc	    callback;
    caddr_t		    closure;
{
    register CallbackRec *new;

    new = XtNew(CallbackRec);
    new->next = NULL;
    new->widget = widget;
    new->closure = closure;
    new->callback = callback; 

    for ( ; *callbacks != NULL; callbacks = &(*callbacks)->next) {};
    *callbacks = new;
} /* _XtAddCallback */

void XtAddCallback(widget, name, callback, closure)
    Widget	    widget;
    String	    name;
    XtCallbackProc  callback;
    caddr_t	    closure;
{
    CallbackList *callbacks;

    callbacks = FetchCallbackList(widget,name);
    if (callbacks == NULL) {
       XtWarning("Cannot find callback list in XtAddCallbacks");
       return;
    }
    _XtAddCallback(widget, callbacks, callback, closure);
} /* XtAddCallbacks */

/* ARGSUSED */
static void AddCallbacks(widget, callbacks, newcallbacks)
    Widget		    widget;
    register CallbackList   *callbacks;
    CallbackList	    newcallbacks;
{
    for ( ; *callbacks != NULL; callbacks = &(*callbacks)->next) {};
    *callbacks = newcallbacks;
} /* AddCallback */

void XtAddCallbacks(widget, name, xtcallbacks)
    Widget	    widget;
    String	    name;
    XtCallbackList     xtcallbacks;
{
    CallbackList *callbacks;

    callbacks = FetchCallbackList(widget, name);
    if (callbacks == NULL) {
       XtWarning("Cannot find callback list in XtAddCallbacks");
       return;
    }
    AddCallbacks(widget, callbacks, _CompileCallbackList(widget, xtcallbacks));
} /* XtAddCallbacks */

void RemoveCallback (widget, callbacks, callback, closure)
    Widget		    widget;
    register CallbackList   *callbacks;
    XtCallbackProc	    callback;
    caddr_t		    closure;

{
    register CallbackList cl;

    for (cl = *callbacks; cl != NULL; (cl = *(callbacks = &cl->next))) {
	if ((cl->widget == widget) && (cl->closure == closure)
                            && (cl->callback == callback)) {
	    *callbacks = cl->next;
	    XtFree ((char *)cl);
	    return;
	}
    }
} /* RemoveCallback */

void XtRemoveCallback (widget, name, callback, closure)
    Widget	    widget;
    String	    name;
    XtCallbackProc  callback;
    caddr_t	    closure;
{

    CallbackList *callbacks;

    callbacks = FetchCallbackList(widget, name);
    if (callbacks == NULL) {
	XtWarning("Cannot find callback list in XtRemoveCallback");
	return;
    }
    RemoveCallback(widget, callbacks, callback, closure);
} /* XtRemoveCallback */


void XtRemoveCallbacks (widget, name, xtcallbacks)
    Widget	    widget;
    String	    name;
    register XtCallbackList  xtcallbacks;
{

    CallbackList *callbacks;

    callbacks = FetchCallbackList(widget, name);
    if (callbacks == NULL) {
	XtWarning("Cannot find callback list in XtRemoveCallbacks");
	return;
    }

    for (; xtcallbacks->callback != NULL; xtcallbacks++) {
	RemoveCallback(
	    widget, callbacks, xtcallbacks->callback,
	    (caddr_t) xtcallbacks->closure);
    }
} /* XtRemoveCallbacks */


void _XtRemoveAllCallbacks (callbacks)
    CallbackList *callbacks;
{
    register CallbackList cl, next;
    
    cl = *callbacks;
    while (cl != NULL) {
	next = cl->next;
	XtFree((char *)cl);
	cl = next;
    }
    (*callbacks) = NULL;
} /* _XtRemoveAllCallbacks */

void XtRemoveAllCallbacks(widget, name)
    Widget widget;
    String name;
{
    CallbackList *callbacks;

    callbacks = FetchCallbackList(widget, name);
    if (callbacks == NULL) {
	XtWarning("Cannot find callback list in XtRemoveAllCallbacks");
	return;
    }
    _XtRemoveAllCallbacks(callbacks);
} /* XtRemoveAllCallbacks */



#define CALLBACK_CACHE_SIZE	30

void _XtCallCallbacks (callbacks, call_data)
    CallbackList *callbacks;
    caddr_t       call_data;
{
    register CallbackRec *cl;
    CallbackRec		 stack_cache [CALLBACK_CACHE_SIZE];
    CallbackList	 head;
    register Cardinal    i;

    if ((*callbacks) == NULL ) return;

    /* Get length of callback list */
    for (i = 0, cl = *callbacks; cl != NULL; i++, cl = cl->next) {};

    /* If lots of entries allocate an array, otherwise use stack_cache */
    if (i > CALLBACK_CACHE_SIZE) {
	head = (CallbackList) XtMalloc((unsigned) (i * sizeof(CallbackRec)));
    } else {
	head = stack_cache;
    }

    /* Copy callback list into array to insulate us from callbacks that change
       the callback list */
    for (i = 0, cl = *callbacks; cl != NULL; i++, cl = cl->next) {
	head[i] = *cl;
    }

    /* Execute each callback in the array */
    for (cl = head; i != 0; cl++, i--) {
	(*(cl->callback)) (cl->widget, cl->closure, call_data);
    }

    /* If temporary array allocated, free it */
    if (head != stack_cache) {
	XtFree((char *)head);
    }
} /* _XtCallCallbacks */


CallbackList _CompileCallbackList(widget, xtcallbacks)
    Widget		    widget;
    register XtCallbackList xtcallbacks;
{
    /* Turn a public XtCallbackList into a private CallbackList */

    register CallbackList   new, *pLast;
    CallbackList	    head;

    pLast = &head;
    for (; xtcallbacks->callback != NULL; xtcallbacks++) {
	new		= XtNew(CallbackRec);
	*pLast		= new;
	pLast		= &(new->next);
	new->widget     = widget;
	new->callback   = xtcallbacks->callback;
	new->closure    = (caddr_t) xtcallbacks->closure;
    };
    *pLast = NULL;

    return(head);
} /* _CompileCallbackList */

void XtCallCallbacks(widget, name, call_data)
    Widget   widget;
    String   name;
    caddr_t  call_data;
{
    CallbackList *callbacks;

    callbacks = FetchCallbackList(widget, name);
    if (callbacks == NULL) {
	XtWarning("Cannot find callback list in XtCallCallbacks");
	return;
    }
    _XtCallCallbacks(callbacks, call_data);
} /* XtCallCallbacks */

/* ||| What is this doing here? */
static void OverrideCallback(callbacks, callback)
    CallbackList	    *callbacks;
    register XtCallbackProc callback;
{
    register CallbackList cl;

    cl = *callbacks;
    while (cl != NULL) {
	cl->callback = callback;
	cl = cl->next;
    }
} /* OverrideCallback */

/* ||| What is this doing here? */
void XtOverrideCallback(widget, callback_name, callback)
     Widget		widget;
     String		callback_name;
     XtCallbackProc	callback;
{
    CallbackList *callbacks;
    callbacks = FetchCallbackList(widget,callback_name);
    if (callbacks == NULL) {
	XtWarning("Cannot find callback list in XtOverrideCallback");
	return;
    }
    OverrideCallback(callbacks, callback);
} /* XtOverrideCallback */

/* ||| What is this doing here? */
extern XtCallbackStatus XtHasCallbacks(widget, callback_name)
     Widget		widget;
     String		callback_name;
{
    CallbackList *callbacks;
    callbacks = FetchCallbackList(widget, callback_name);
    if (callbacks == NULL) {
	return (XtCallbackNoList);
    }    
    if (*callbacks == NULL) return (XtCallbackHasNone);
    return (XtCallbackHasSome);
} /* XtHasCallbacks */
