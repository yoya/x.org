/*
* $Header: IntrinsicI.h,v 1.20 88/02/26 12:41:21 swick Exp $
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

#ifndef _XtintrinsicI_h
#define _XtintrinsicI_h

#include "IntrinsicP.h"

/****************************************************************
 *
 * Byte utilities
 *
 ****************************************************************/


extern void bcopy();
#define XtBCopy(src, dst, size)				    \
    if (size == sizeof(int) &&				    \
	!((unsigned int) src & 0x3) &&			    \
	!((unsigned int) dst & 0x3))			    \
	*((int *) (dst)) = *((int *) (src));		    \
    else if (size == sizeof(char))			    \
	*((char *) (dst)) = *((char *) (src));		    \
    else						    \
	bcopy((char *) (src), (char *) (dst), (int) (size));

extern void bzero();
#define XtBZero(dst, size)				    \
    if (size == sizeof(int))				    \
	*((int *) (dst)) = 0;				    \
    else						    \
	bzero((char *) (dst), (int) (size));

extern int bcmp();
#define XtBCmp(b1, b2, size)				    \
    (size == sizeof(int) ?				    \
	*((int *) (b1)) != *((int *) (b2))		    \
    :   bcmp((char *) (b1), (char *) (b2), (int) (size))    \
    )



/****************************************************************
 *
 * Stack cache allocation/free
 *
 ****************************************************************/

#define XtStackAlloc(size, stack_cache_array)     \
    (size <= sizeof(stack_cache_array)		  \
    ?  stack_cache_array			  \
    :  XtMalloc((unsigned) size))

#define XtStackFree(pointer, stack_cache_array) \
    if ((pointer) != (stack_cache_array)) XtFree(pointer)



/****************************************************************
 *
 * Callbacks
 *
 ****************************************************************/

typedef struct _XtOffsetRec {
     struct _XtOffsetRec *next;
     XrmQuark       name;
     int            offset;
} XtOffsetRec, *_XtOffsetList;

extern void _XtRemoveAllCallbacks (); /* callbackList */
    /* CallbackList	*callbackList;	*/

extern void _XtCallCallbacks (); /* callbacks, call_data */
    /* CallbackList	*callbacks;	*/
    /* Opaque		callData;	*/

extern void _XtAddCallback(); /* widget, callbacks, callback, closure */
    /* Widget           widget; */
    /* CallbackList     *callbacks; */
    /* XtCallbackProc   callback; */
    /* Opaque           closure; */

typedef struct _CallbackRec *CallbackList;

CallbackList DestroyList;

/****************************************************************
 *
 * Next Event
 *
 ****************************************************************/

Display *toplevelDisplay;

extern Boolean _XtwaitForSomething(); /* ignoreTimers, ignoreInputs, block, howlong */
    /* Boolean ignoreTimers; */
    /* Boolean ignoreInputs; */
    /* Boolean block; */
    /* unsigned long *howlong; */

/****************************************************************
 *
 * Resources
 *
 ****************************************************************/

#define StringToQuark(string) XrmStringToQuark(string)
#define StringToName(string) XrmStringToName(string)
#define StringToClass(string) XrmStringToClass(string)

extern void XtGetResources();
    /* Widget       widget;             */
    /* ArgList	    args;		*/
    /* int	    num_args;		*/

#ifndef INTRINSIC_C
globalref XrmDatabase XtDefaultDB;
globalref XrmName XtApplicationName;
globalref XrmClass XtApplicationClass;
#endif

extern void _XtInstallTranslations(); /* widget, stateTable */
    /* Widget widget; */
    /* XtTranslations stateTable; */

extern XtTranslations _ParseTranslationTable();	/* source */
    /* String source; */

extern void _XtBindActions(); /* widget, stateTable */
    /* Widget widget; */
    /* XtTranslations stateTable; */

extern void _XtTranslateInitialize();

extern void _XtConvertInitialize();

extern void _XtConvert();

extern void _XtRegisterGrabs(); /* widget */
    /* Widget widget; */

extern void _XtPopup(); /* widget, grab_kind, spring_loaded */
    /* Widget      widget; */
    /* XtGrabKind  grab_kind; */
    /* Boolean     spring_loaded; */

#endif _XtintrinsicI_h
/* DON'T ADD STUFF AFTER THIS #endif */
