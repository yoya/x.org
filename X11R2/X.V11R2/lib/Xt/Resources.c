#ifndef lint
static char rcsid[] = "$Header: Resources.c,v 1.40 88/02/26 12:46:13 swick Exp $";
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

/* Converted to classing toolkit on 28 August 1987 by Joel McCormack */

/* XtResourceList.c -- compile and process resource lists. */

#include <stdio.h>
#include "IntrinsicI.h"
#include "Shell.h"
#include "Resource.h"
#include "StringDefs.h"

#ifdef reverseVideoHack
static XrmName	QreverseVideo;
#endif
static XrmClass	QBoolean, QString;

static void CopyFromArg(src, dst, size)
    XtArgVal src, dst;
    register unsigned int size;
{
    if (size == sizeof(XtArgVal))
	*(XtArgVal *)dst = src;
    else if (size == sizeof(short)) 
	*(short *)dst = (short)src;
    else if (size == sizeof(char))
        *(char *)dst = (char)src;
    else if (size < sizeof(XtArgVal))
	bcopy((char *) &src, (char *) dst, (int) size);
    else
	bcopy((char *) src, (char *) dst, (int) size);

}

static void CopyToArg(src, dst, size)
    XtArgVal src, *dst;
    register unsigned int size;
{
    /* ||| Old semantics are yucky, but keep as long as NULL value */
    if (*dst != NULL) 
	dst = *(XtArgVal **)dst;

    if (size == sizeof(XtArgVal))
	*dst = *(XtArgVal *)src;
    else if (size == sizeof(short)) 
	*dst = (XtArgVal) *((short *) src);
    else if (size == sizeof(char))
        *dst = (XtArgVal) *((char *) src);
    else if (size < sizeof(XtArgVal))
	bcopy((char *) src, (char *) dst, (int) size);
    else
	bcopy((char *) src, (char *) *dst, (int) size);

}

#ifndef lint
static void PrintResourceList(list, count)
    register XtResourceList list;
    register int count;
{
    for (; --count >= 0; list++) {
        (void) printf("    name: %s, class: %s, type: %s,\n",
	    list->resource_name, list->resource_class, list->resource_type);
	(void) printf("    size: %d, offset: %x, def_type: %s, def_addr: %x\n",
	    list->resource_size, list->resource_offset,
	    list->default_type, list->default_addr);
    }
}
#endif

static Cardinal GetNamesAndClasses(w, names, classes)
    register Widget	  w;
    register XrmNameList  names;
    register XrmClassList classes;
{
    register Cardinal length, j;
    register XrmQuark t;

    for (length = 0; w != NULL; w = (Widget) w->core.parent) {
	names[length] = w->core.xrm_name;
	classes[length] = XtClass(w)->core_class.xrm_class;
	length++;
     }
    names[length] = XtApplicationName;
    classes[length] = XtApplicationClass;
    length++;
    /* They're in backwards order, flop them around */
    for (j = 0; j < length/2; j++) {
	t = names[j];
	names[j] = names[length-j-1];
	names[length-j-1] = t;
        t = classes[j];
	classes[j] = classes[length-j-1];
	classes[length-j-1] = t;
    }
    return length;
}


/* Spiffy fast compiled form of resource list.				*/
/* XtResourceLists are compiled in-place into XrmResourceLists		*/
/* All atoms are replaced by quarks, and offsets are -offset-1 to	*/
/* indicate that this list has been compiled already			*/

void  XrmCompileResourceList(resources, num_resources)
    register XtResourceList resources;
    	     Cardinal	  num_resources;
{
    register XrmResourceList xrmres;
    register Cardinal count;

    for (xrmres = (XrmResourceList) resources, count = 0;
         count < num_resources;
	 xrmres++, resources++, count++) {
    	xrmres->xrm_name	 = StringToName(resources->resource_name);
    	xrmres->xrm_class	 = StringToClass(resources->resource_class);
    	xrmres->xrm_type	 = StringToQuark(resources->resource_type);
/*	xrmres->xrm_size	 = resources->resource_size; */
        xrmres->xrm_offset	 = -resources->resource_offset - 1;
    	xrmres->xrm_default_type = StringToQuark(resources->default_type);
/*	xrmres->xrm_default_addr = resources->default_addr; */
    }

} /* XrmCompileResourceList */

static void XrmGetResources(widget, base, names, classes, length, 
	resources, num_resources, quark_args, args, num_args)
    Widget	    widget;	    /* Widget resources are associated with */
    caddr_t	    base;	    /* Base address of memory to write to   */
    register XrmNameList names;     /* Full inheritance name of widget      */
    register XrmClassList classes;  /* Full inheritance class of widget     */
    Cardinal	    length;	    /* Number of entries in names, classes  */
    XtResourceList  resources;      /* The list of resources required.      */
    Cardinal	    num_resources;  /* number of items in resource list     */
    XrmQuarkList    quark_args;     /* Arg names quarkified		    */
    ArgList	    args;	    /* ArgList to override resources	    */
    Cardinal	    num_args;       /* number of items in arg list	    */
{
    register 	ArgList		arg;
    register 	XrmName		argName;
		XrmResourceList	xrmres;
    register 	XrmResourceList	res;
    		XrmValue	value, defaultValue;
    register 	int		j;
    		int		i;
#define SEARCHTABLESIZE 100
		XrmHashTable	stackSearchList[SEARCHTABLESIZE];
    		XrmHashTable	*searchList;
		unsigned int	searchListSize;
		Bool		status;
		Boolean		found[400];
		XrmQuark	rawType;
		XrmValue	rawValue;
#ifdef reverseVideoHack
    /* ||| */
		Boolean		reverseVideo, getReverseVideo;

    reverseVideo    = FALSE;
    getReverseVideo = TRUE;
#endif

    if ((args == NULL) && (num_args != 0)) {
    	XtWarning("argument count > 0 on NULL argument list");
	num_args = 0;
    }
    if ((resources == NULL) && (num_resources != 0)) {
    	XtWarning("resource count > 0 on NULL resource list");
	return;
    }

    if (num_resources != 0) {
	/* Compile resource list if needed */
	if (((int) resources->resource_offset) >= 0) {
	    XrmCompileResourceList(resources, num_resources);
	}
	xrmres = (XrmResourceList) resources;

	/* Mark each resource as not found on arg list */
	bzero((char *) found, (int) (num_resources * sizeof(Boolean)));

	/* Copy the args into the resources, mark each as found */
	for (arg = args, i = 0; i < num_args; i++, arg++) {
	    argName = quark_args[i];
#ifdef reverseVideoHack
	    if (argName == QreverseVideo) {
		reverseVideo = (Boolean) arg->value;
		getReverseVideo = FALSE;
	    } else
#endif
		for (j = 0, res = xrmres; j < num_resources; j++, res++) {
		    if (argName == res->xrm_name) {
			CopyFromArg(
			    arg->value,
			    (XtArgVal) base - res->xrm_offset - 1,
			    res->xrm_size);
			found[j] = TRUE;
			break;
		    }
		}
	}
    }

    /* Resources name and class will go into names[length], classes[length] */
    names[length+1]   = NULLQUARK;
    classes[length+1] = NULLQUARK;

#ifdef reverseVideoHack
    if (CellsOfScreen(XtScreen(widget))) > 2) {
    	/* Color box, ignore ReverseVideo */
	reverseVideo = FALSE;
    } else if (getReverseVideo) {
	names[length] = QreverseVideo;
	classes[length] = QBoolean;
	if (XrmQGetResource(XtDefaultDB, names, classes, &rawType, &rawValue) &&
		rawType != QBoolean) {
	    _XtConvert(NULL, rawType, &rawValue, QBoolean, &value);
	} else {
	    value = rawValue;
	}
	if (value.addr)
	    reverseVideo = *((Boolean *) value.addr);
    }
    /* ||| Nothing is done w/reverseVideo now, but something should be! */
#endif

    names[length] = NULLQUARK;
    classes[length] = NULLQUARK;

    searchList = stackSearchList;
    searchListSize = SEARCHTABLESIZE;
    if (num_resources != 0) {

	/* Ask resource manager for a list of database levels that we can
	   do a single-level search on each resource */

	status = XrmQGetSearchList(XtDefaultDB, names, classes,
				   searchList, SEARCHTABLESIZE);
	if (!status) {
	    searchList = NULL;
	    do {
		searchList = (XrmHashTable*)
		    XtRealloc(searchList, (searchListSize *= 2));
		status = XrmQGetSearchList(XtDefaultDB, names, classes,
					   searchList, searchListSize);
	    } while (!status);
	}
	
	/* go to the resource manager for those resources not found yet */
	/* if it's not in the resource database use the default value   */
    
	for (res = xrmres, j = 0; j < num_resources; j++, res++) {
	    if (! found[j]) {
		if (XrmQGetSearchResource(searchList,
			res->xrm_name, res->xrm_class, &rawType, &rawValue)
			&& rawType != res->xrm_type) {
		    _XtConvert(widget,
			rawType, &rawValue, res->xrm_type, &value);
		} else {
		    value = rawValue;
		}
		if (value.addr == NULL && res->xrm_default_addr != NULL) {
		    /* Convert default value to proper type */
		    defaultValue.addr = res->xrm_default_addr;
		    if (res->xrm_default_type == QString) {
			if (defaultValue.addr == NULL) {
			    defaultValue.size = 0;
			} else {
			    defaultValue.size =
				strlen((char *) res->xrm_default_addr) + 1;
			}
		    } else {
			defaultValue.size = sizeof(caddr_t);
		    }
		    if (res->xrm_default_type != res->xrm_type)
			_XtConvert(widget, res->xrm_default_type, &defaultValue,
			    res->xrm_type, &value);
		    else
			value = defaultValue;
		}
		if (value.addr) {
		    if (res->xrm_type == QString) {
		       *((caddr_t *)(base - res->xrm_offset - 1)) = value.addr;
		    } else {
			/* Cvt procs return pointer to storage of exact size,
			   e.g. char/short/int/...  thus casts have been
			   done there and we can do a simple copy here */
			XtBCopy(value.addr, base - res->xrm_offset - 1,
			    res->xrm_size);
		    }
		} else {
		    /* didn't find a default value, initialize to NULL... */
		    XtBZero(base - res->xrm_offset - 1, res->xrm_size);
		}
	    }
	}
    }
    if (searchList != stackSearchList) XtFree(searchList);
}

static void GetResources(
	widgetClass, w, names, classes, length, quark_args, args, num_args)
    WidgetClass	  widgetClass;
    Widget	  w;
    XrmNameList	  names;
    XrmClassList  classes;
    Cardinal	  length;
    XrmQuarkList  quark_args;
    ArgList	  args;
    Cardinal	  num_args;
{
    /* First get resources for superclasses */
    if (widgetClass->core_class.superclass != NULL) {
        GetResources(widgetClass->core_class.superclass,
	    w, names, classes, length, quark_args, args, num_args);
    }
    /* Then for this class */
    XrmGetResources(w, (caddr_t) w, names, classes, length,
	widgetClass->core_class.resources,
	widgetClass->core_class.num_resources,
	quark_args, args, num_args);
} /* GetResources */

static void ConstraintGetResources(
	widgetClass, w, names, classes, length, quark_args, args, num_args)
    ConstraintWidgetClass   widgetClass;
    Widget	  w;
    XrmNameList	  names;
    XrmClassList  classes;
    Cardinal	  length;
    XrmQuarkList  quark_args;
    ArgList	  args;
    Cardinal	  num_args;
{
    /* First get resources for superclasses */
    if ((WidgetClass) widgetClass != constraintWidgetClass) {
	if (widgetClass == NULL) 
	    XtError(
		"Subclass of Constraint required in ConstraintGetResources");
        ConstraintGetResources(
	    (ConstraintWidgetClass) (widgetClass->core_class.superclass),
	    w, names, classes, length, quark_args, args, num_args);
    }
    /* Then for this class */
    XrmGetResources(w, w->core.constraints, names, classes, length,
	widgetClass->constraint_class.resources,
	widgetClass->constraint_class.num_resources,
	quark_args, args, num_args);
} /* ConstraintGetResources */



static void CacheArgs(args, num_args, quark_cache, num_quarks, pQuarks)
    ArgList	    args;
    Cardinal	    num_args;
    XrmQuarkList    quark_cache;
    Cardinal	    num_quarks;
    XrmQuarkList    *pQuarks;       /* RETURN */
{
    register XrmQuarkList   quarks;
    register Cardinal       i;

    if (num_quarks < num_args) {
	quarks = (XrmQuarkList) XtMalloc(num_args * sizeof(XrmQuark));
    } else {
	quarks = quark_cache;
    }
    *pQuarks = quarks;

    for (i = 0; i < num_args; i++, quarks++, args++) {
	*quarks = StringToQuark(args->name);
    }
}

#define FreeCache(cache, pointer) \
	  if (cache != pointer) XtFree((char *)pointer)


void XtGetResources(w, args, num_args)
    register 	Widget	  w;
    		ArgList	  args;
    		Cardinal  num_args;
{
    XrmName	    names[100];
    XrmClass	    classes[100];
    Cardinal	    length;
    XrmQuark	    quark_cache[100];
    XrmQuarkList    quark_args;

    /* Make sure xrm_class, xrm_name are valid */
    if (XtClass(w)->core_class.xrm_class == NULLQUARK) {
        XtClass(w)->core_class.xrm_class =
	    StringToClass(XtClass(w)->core_class.class_name);
    }
    w->core.xrm_name = StringToName(w->core.name);

    /* Get names, classes for widget on up */
    length = GetNamesAndClasses(w, names, classes);
   
    /* Compile arg list into quarks */
    CacheArgs(args, num_args, quark_cache, XtNumber(quark_cache), &quark_args);
    /* Get resources starting at CorePart on down to this widget */
    GetResources(XtClass(w), w, names, classes, length,
	quark_args, args, num_args);
    if (w->core.constraints != NULL) 
	ConstraintGetResources((ConstraintWidgetClass) XtClass(w->core.parent),
	    w, names, classes, length, quark_args, args, num_args);
    FreeCache(quark_cache, quark_args);
} /* XtGetResources */


void XtGetSubresources
	(w, base, name, class, resources, num_resources, args, num_args)
    Widget	  w;		  /* Widget "parent" of subobject   */
    caddr_t	  base;		  /* Base address to write to       */
    String	  name;		  /* name of subobject		    */
    String	  class;	  /* class of subobject		    */
    XtResourceList resources;	  /* resource list for subobject    */
    Cardinal	  num_resources;
    ArgList	  args;		  /* arg list to override resources */
    Cardinal	  num_args;
{
    XrmName	  names[100];
    XrmClass	  classes[100];
    Cardinal	  length;
    XrmQuark	  quark_cache[100];
    XrmQuarkList  quark_args;

    /* Get full name, class of subobject */
    length = GetNamesAndClasses(w, names, classes);
    names[length] = StringToName(name);
    classes[length] = StringToClass(class);
    length++;

    /* Compile arg list into quarks */
    CacheArgs(args, num_args, quark_cache, XtNumber(quark_cache), &quark_args);
    /* Fetch resources */
    XrmGetResources(w, base, names, classes, length,
        resources, num_resources, quark_args, args, num_args);
    FreeCache(quark_cache, quark_args);
}


void XtGetApplicationResources
	(w, base, resources, num_resources, args, num_args)
    Widget	    w;		  /* Application shell widget       */
    caddr_t	    base;	  /* Base address to write to       */
    XtResourceList  resources;	  /* resource list for subobject    */
    Cardinal	    num_resources;
    ArgList	    args;	  /* arg list to override resources */
    Cardinal	    num_args;
{
    XrmName	    names[100];
    XrmClass	    classes[100];
    Cardinal	    length;
    XrmQuark	    quark_cache[100];
    XrmQuarkList    quark_args;

    /* Get full name, class of subobject */
    length = GetNamesAndClasses(NULL, names, classes);

    /* Compile arg list into quarks */
    CacheArgs(args, num_args, quark_cache, XtNumber(quark_cache), &quark_args);
    /* Fetch resources */
    XrmGetResources(w, base, names, classes, length,
        resources, num_resources, quark_args, args, num_args);
    FreeCache(quark_cache, quark_args);
}


void XtGetSubvalues(base, resources, num_resources, args, num_args)
  caddr_t		base;		/* Base address to fetch values from */
  register XtResourceList resources;	/* The current resource values.      */
  register Cardinal	num_resources;	/* number of items in resources      */
  ArgList 		args;		/* The resource values requested     */
  Cardinal		num_args;	/* number of items in arg list       */
{
    register ArgList		arg;
    register XrmResourceList	xrmres;
    register int 		i;
    register XrmName		argName;

    if (num_resources == 0) return;

    /* Resource lists are assumed to be in compiled form already via the
       initial XtGetResources, XtGetSubresources calls */

    for (arg = args ; num_args != 0; num_args--, arg++) {
	argName = StringToName(arg->name);
	for (xrmres = (XrmResourceList) resources, i = 0;
	     i < num_resources;
	     i++, xrmres++) {
	    if (argName == xrmres->xrm_name) {
		CopyToArg(
		    (XtArgVal) base - xrmres->xrm_offset - 1,
		    &arg->value,
		    xrmres->xrm_size);
		break;
	    }
	}
    }
}

static void GetValues(widgetClass, w, args, num_args)
    WidgetClass	  widgetClass;
    Widget	  w;
    ArgList	  args;
    Cardinal	  num_args;
{
    /* First get resource values for superclass */
    if (widgetClass->core_class.superclass != NULL) {
        GetValues(widgetClass->core_class.superclass, w, args, num_args);
    }
    /* Then for this class */
    XtGetSubvalues(
	(caddr_t) w,
	widgetClass->core_class.resources,
	widgetClass->core_class.num_resources,
	args, num_args);
} /* GetValues */

static void ConstraintGetValues(widgetClass, w, args, num_args)
    ConstraintWidgetClass widgetClass;
    Widget	  w;
    ArgList	  args;
    Cardinal	  num_args;
{
    /* First get resource values for superclass */
    if (widgetClass != (ConstraintWidgetClass)constraintWidgetClass) {
        if (widgetClass == NULL)
           XtError("Subclass of Constraint required in ConstraintGetValues");
        ConstraintGetValues(
	    (ConstraintWidgetClass) (widgetClass->core_class.superclass),
            w, args, num_args);
    }
    /* Then for this class */
    XtGetSubvalues(
	w->core.constraints,
	widgetClass->constraint_class.resources,
        widgetClass->constraint_class.num_resources,
        args, num_args);
} /* ConstraintGetValues */

static void CallGetValuesHook(widgetClass, w, args, num_args)
    WidgetClass	  widgetClass;
    Widget	  w;
    ArgList	  args;
    Cardinal	  num_args;
{
    if (widgetClass->core_class.superclass != NULL) {
	CallGetValuesHook
	    (widgetClass->core_class.superclass, w, args, num_args);
    }
    if (widgetClass->core_class.get_values_hook != NULL) {
	(*(widgetClass->core_class.get_values_hook)) (w, args, &num_args);
    }
}


void XtGetValues(w, args, num_args)
    register Widget   w;
    register ArgList  args;
    register Cardinal num_args;
{
    if (num_args == 0) return;
    if ((args == NULL) && (num_args != 0)) {
	XtError("Argument count > 0 on NULL argument list in XtGetValues");
    }
    /* Get resource values starting at CorePart on down to this widget */
    GetValues(XtClass(w), w, args, num_args);
    if (w->core.constraints != NULL)
	ConstraintGetValues(
	    (ConstraintWidgetClass) XtClass(w->core.parent), w, args, num_args);
    /* Notify any class procedures that we've performed a "get_values" */
    CallGetValuesHook(XtClass(w), w, args, num_args);
} /* XtGetValues */
 
void XtSetSubvalues(base, resources, num_resources, args, num_args)
  caddr_t		base;		/* Base address to write values to   */
  register XtResourceList resources;	/* The current resource values.      */
  register Cardinal	num_resources;	/* number of items in resources      */
  ArgList 		args;		/* The resource values to set        */
  Cardinal		num_args;	/* number of items in arg list       */
{
    register ArgList		arg;
    register XrmResourceList	xrmres;
    register int 	        i;
    register XrmName		argName;

    if (num_resources == 0) return;

    /* Resource lists are assumed to be in compiled form already via the
       initial XtGetResources, XtGetSubresources calls */

    for (arg = args ; num_args != 0; num_args--, arg++) {
	argName = StringToName(arg->name);
	for (xrmres = (XrmResourceList) resources, i = 0;
	     i < num_resources;
	     i++, xrmres++) {
	    if (argName == xrmres->xrm_name) {
		CopyFromArg(arg->value,
			    (XtArgVal) base - xrmres->xrm_offset - 1,
			    xrmres->xrm_size);
		break;
	    }
	}
    }
} /* XtSetSubvalues */

static void SetValues(widgetClass, w, args, num_args)
    WidgetClass	  widgetClass;
    Widget	  w;
    ArgList	  args;
    Cardinal	  num_args;
{
    /* First set resource values for superclass */
    if (widgetClass->core_class.superclass != NULL) {
        SetValues(widgetClass->core_class.superclass, w, args, num_args);
    }
    /* Then for this class */
    XtSetSubvalues((caddr_t) w,
        widgetClass->core_class.resources,
	widgetClass->core_class.num_resources,
	args, num_args);
} /* SetValues */



static void ConstraintSetValues(widgetClass, w, args, num_args)
    ConstraintWidgetClass widgetClass;
    Widget	  w;
    ArgList	  args;
    Cardinal	  num_args;
{
    /* First set resource values for superclass */
    if (widgetClass != (ConstraintWidgetClass)constraintWidgetClass) {
	if (widgetClass == NULL)
           XtError("Subclass of Constraint required in ConstraintSetValues");
	ConstraintSetValues(
	    (ConstraintWidgetClass) (widgetClass->core_class.superclass),
	    w, args, num_args);
    }
    /* Then for this class */
    XtSetSubvalues(
	w->core.constraints,
	widgetClass->constraint_class.resources,
        widgetClass->constraint_class.num_resources,
	args, num_args);
} /* ConstraintSetValues */


static Boolean CallSetValues (class, current, request, new, args, num_args)
    WidgetClass class;
    Widget      current, request, new;
    ArgList     args;
    Cardinal    num_args;
{
    Boolean redisplay = FALSE;

    if (class->core_class.superclass != NULL)
        redisplay = CallSetValues(
	  class->core_class.superclass, current, request, new, args, num_args);
    if (class->core_class.set_values != NULL)
        redisplay |=
	    (*class->core_class.set_values) (current, request, new);
    if (class->core_class.set_values_hook != NULL)
	redisplay |= 
	    (*class->core_class.set_values_hook) (current, args, &num_args);
    return (redisplay);
}

static Boolean CallConstraintSetValues (class, current, request, new)
    ConstraintWidgetClass class;
    Widget      current, request, new;
{
    Boolean redisplay = FALSE;

    if ((WidgetClass)class != constraintWidgetClass) {
	if (class == NULL)
	 XtError("Subclass of Constraint required in CallConstraintSetValues");
	redisplay = CallConstraintSetValues(
	    (ConstraintWidgetClass) (class->core_class.superclass),
	    current, request, new);
    }
    if (class->constraint_class.set_values != NULL)
        redisplay |=
	  (*class->constraint_class.set_values) (current, request, new);
    return (redisplay);
}

void XtSetValues(w, args, num_args)
    register Widget   w;
    ArgList  args;
    Cardinal num_args;
{
    register Widget newWidget, requestWidget;
    char	    newWidgetCache[500], requestWidgetCache[500];
    char	    newConstraintCache[100], requestConstraintCache[100];
    Cardinal	    widgetSize, constraintSize;
    Dimension	    saveHeight, saveWidth;
    Boolean redisplay, resize;
    XtGeometryResult result;
    XtWidgetGeometry geometryRequest, geometryReply;
    if (num_args == 0) return;
    if ((args == NULL) && (num_args != 0)) {
        XtError("Argument count > 0 on NULL argument list in XtSetValues");
    }

    /* Allocate and copy current widget into newWidget */
    widgetSize = XtClass(w)->core_class.widget_size;
    requestWidget = (Widget) XtStackAlloc (widgetSize, requestWidgetCache);
    newWidget = (Widget) XtStackAlloc(widgetSize, newWidgetCache);
    bcopy((char *) w, (char *) requestWidget, (int) widgetSize);

    /* Set resource values starting at CorePart on down to this widget */
    SetValues(XtClass(w), requestWidget, args, num_args);
    bcopy ((char *) requestWidget, (char *) newWidget, (int) widgetSize);

    if (w->core.constraints != NULL) {
	/* Allocate and copy current constraints into newWidget */
	constraintSize = ((ConstraintWidgetClass) XtClass(w->core.parent))->
	    constraint_class.constraint_size;
	requestWidget->core.constraints =
	    (caddr_t) XtStackAlloc(constraintSize, requestConstraintCache);
	newWidget->core.constraints =
	    (caddr_t) XtStackAlloc(constraintSize, newConstraintCache);
	bcopy((char *) w->core.constraints,
	      (char *) requestWidget->core.constraints,
	      (int) constraintSize);

	/* Set constraint values starting at CorePart on down to parent */
	ConstraintSetValues(
	    (ConstraintWidgetClass) XtClass(w->core.parent),
	    requestWidget, args, num_args);
	bcopy((char *) requestWidget->core.constraints,
	      (char *) newWidget->core.constraints, (int) constraintSize);
    }

    /* If the widget is a shell widget, we have to cache old dimensions
       in order to tell whether to call the resize proc or not */

    if  (XtIsSubclass(w, shellWidgetClass)) {
	saveWidth = w->core.width;
	saveHeight = w->core.height;
    }

    /* Inform widget of changes, then inform parent of changes */
    redisplay =
       CallSetValues (XtClass(w), w, requestWidget, newWidget, args, num_args);
    if (w->core.constraints != NULL) {
	redisplay |= CallConstraintSetValues(
	    (ConstraintWidgetClass) XtClass(w->core.parent),
	    w, requestWidget, newWidget);
    }

    /* Now perform geometry request if needed */
    geometryRequest.request_mode = 0;
    if (w->core.x != newWidget->core.x)
	geometryRequest.request_mode |= CWX;
    if (w->core.y != newWidget->core.y)
	geometryRequest.request_mode |= CWY;
    if (w->core.width != newWidget->core.width)
	geometryRequest.request_mode |= CWWidth;
    if (w->core.height != newWidget->core.height)
	geometryRequest.request_mode |= CWHeight;
    if (w->core.border_width != newWidget->core.border_width)
	geometryRequest.request_mode |= CWBorderWidth;

    if  (XtIsSubclass(w, shellWidgetClass) &&
	    (w->core.width != saveWidth || w->core.height != saveHeight)) {
	resize = TRUE;
    } else resize = FALSE;

    if (geometryRequest.request_mode != 0) {
	if (XtIsSubclass(w, shellWidgetClass)) {
	    XtWarning(
		"Shell subclass did not take care of geometry in XtSetValues");
	} else {
	    geometryRequest.x = newWidget->core.x;
	    geometryRequest.y = newWidget->core.y;
	    geometryRequest.width = newWidget->core.width;
	    geometryRequest.height = newWidget->core.height;
	    geometryRequest.border_width = newWidget->core.border_width;
	    for (;;) {
		result =
		    XtMakeGeometryRequest(w, &geometryRequest, &geometryReply);
	    if (result != XtGeometryAlmost) {
		resize = (result == XtGeometryYes);
		break;
	    }
	    /* An almost reply.  Call widget and let it munge request, reply */
	    if (XtClass(w)->core_class.set_values_almost == NULL) {
		XtWarning("set_values_almost procedure should not be NULL");
		break;
	    }
	    (*(XtClass(w)->core_class.set_values_almost))
		(w, newWidget, &geometryRequest, &geometryReply);
	    if (geometryRequest.request_mode == 0)
		break;
	    } /* loop */
	}
	/* "w" contains the correct geometry fields.  Make sure not to smash
	   them when copying "new" into "w". */
	newWidget->core.x = w->core.x;
	newWidget->core.y = w->core.y;
	newWidget->core.width = w->core.width;
	newWidget->core.height = w->core.height;
	newWidget->core.border_width = w->core.border_width;
    }

    /* Copy newWidget back into w and free all dynamic storage */
    if (w->core.constraints != NULL) {
	bcopy((char *) newWidget->core.constraints,
		(char *) w->core.constraints, (int) constraintSize);
	XtStackFree((char *) newWidget->core.constraints, newConstraintCache);
	newWidget->core.constraints = w->core.constraints;
	XtStackFree((char *) requestWidget->core.constraints,
	    requestConstraintCache);
    }
    bcopy ((char *) newWidget, (char *) w, (int) widgetSize);
    XtStackFree((char *) newWidget, newWidgetCache);
    XtStackFree((char *) requestWidget, requestWidgetCache);

    /* call resize proc if we changed size */
    if (resize && XtClass(w)->core_class.resize != (XtWidgetProc) NULL)
	(*(XtClass(w)->core_class.resize))(w);
    /* Redisplay if needed */
    if (redisplay && XtIsRealized(w)) {
	XClearArea (XtDisplay(w), XtWindow(w), 0, 0, 0, 0, TRUE);
	/* ||| Should do XClearArea(...,FALSE), call expose procedure */
    }

} /* XtSetValues */
 

static Boolean initialized = FALSE;

void _XtResourceListInitialize()
{
    if (initialized) {
	XtWarning("Initializing Resource Lists twice.");
    	return;
    }
    initialized = TRUE;

#ifdef reverseVideoHack
    QreverseVideo = StringToName(XtNreverseVideo);
#endif
    QBoolean = StringToClass(XtCBoolean);
    QString = StringToClass(XtCString);
}
