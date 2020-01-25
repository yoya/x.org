#ifndef lint
static char rcsid[] = "$Header: Convert.c,v 1.9 88/02/26 12:31:06 swick Exp $";
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

#include	"IntrinsicI.h"

#include        <X11/Convert.h>
#include	<X11/Quarks.h>
#include	"Resource.h"

/* ||| */
#include	<stdio.h>

/* Conversion procedure hash table */

typedef struct _ConverterRec *ConverterPtr;

typedef struct _ConverterRec {
    ConverterPtr	next;
    XrmRepresentation	from, to;
    XtConverter		converter;
    XtConvertArgList    convert_args;
    Cardinal		num_args;
} ConverterRec;

#define CONVERTHASHSIZE	512
#define CONVERTHASHMASK	511
#define ProcHash(from_type, to_type) (2 * (from_type) + to_type)

typedef ConverterPtr ConverterTable[CONVERTHASHSIZE];

static ConverterTable	converterTable;


/* Data cache hash table */

typedef struct _CacheRec *CachePtr;

typedef struct _CacheRec {
    CachePtr	next;
    int		hash;
    XtConverter converter;
    XrmValue	*args;
    Cardinal	num_args;
    XrmValue	from;
    XrmValue	to;
} CacheRec;

#define CACHEHASHSIZE	256
#define CACHEHASHMASK	255
typedef CachePtr CacheHashTable[CACHEHASHSIZE];

static CacheHashTable	cacheHashTable;

void _XtAddConverter(fromType, toType, converter, convert_args, num_args)
    XrmRepresentation   fromType, toType;
    XtConverter		converter;
    XtConvertArgList    convert_args;
    Cardinal		num_args;
{
    register ConverterPtr	*pHashEntry;
    register ConverterPtr	p;

    pHashEntry = &converterTable[ProcHash(fromType, toType) & CONVERTHASHMASK];
    /* ||| Check for existing entry */
    p		    = (ConverterPtr) Xpermalloc(sizeof(ConverterRec));
    p->next	    = *pHashEntry;
    *pHashEntry     = p;
    p->from	    = fromType;
    p->to	    = toType;
    p->converter    = converter;
    p->convert_args = convert_args;
    p->num_args     = num_args;
}

void XtAddConverter(fromType, toType, converter, convert_args, num_args)
    register XrmString	fromType, toType;
    XtConverter		converter;
    XtConvertArgList    convert_args;
    Cardinal		num_args;
{
    _XtAddConverter(
	XrmStringToRepresentation(fromType), XrmStringToRepresentation(toType),
	converter, convert_args, num_args);
}

static void CacheEnter(converter, args, num_args, from, to, hash)
    register XtConverter    converter;
    register XrmValuePtr    args;
    Cardinal		    num_args;
    XrmValuePtr		    from;
    XrmValuePtr		    to;
    register int	    hash;
{
    register	CachePtr *pHashEntry;
    register	CachePtr p;
    register    Cardinal i;

    pHashEntry = &cacheHashTable[hash & CACHEHASHMASK];

    p = (CachePtr) Xpermalloc(sizeof(CacheRec));
    p->next	    = *pHashEntry;
    *pHashEntry     = p;
    p->hash	    = hash;
    p->converter    = converter;
    p->from.size    = from->size;
    p->from.addr = (caddr_t) Xpermalloc(from->size);
    XtBCopy(from->addr, p->from.addr, from->size);
    p->num_args = num_args;
    if (num_args == 0) {
	p->args = NULL;
    } else {
	p->args = (XrmValuePtr) Xpermalloc(num_args * sizeof(XrmValue));
	for (i = 0; i < num_args; i++) {
	    p->args[i].size = args[i].size;
	    p->args[i].addr = (caddr_t) Xpermalloc(args[i].size);
	    XtBCopy(args[i].addr, p->args[i].addr, args[i].size);
	}
    }
    p->to.size  = to->size;
    p->to.addr  = (caddr_t) Xpermalloc(to->size);
    XtBCopy(to->addr, p->to.addr, to->size);
}


void CacheStats()
{
    register Cardinal i;
    register CachePtr p;
    register Cardinal entries;

#define MIN(x,y)    ((x) < (y) ? (x) : (y))

    for (i = 0; i < CACHEHASHSIZE; i++) {
	p = cacheHashTable[i];
	if (p != NULL) {
	    for (entries = 0; p != NULL; p = p->next) {
		entries++;
	    }
	    (void) fprintf(stdout, "Index: %4d  Entries: %d\n", i, entries);
	    for (p = cacheHashTable[i]; p != NULL; p = p->next) {
		(void) fprintf(stdout, "    Size: %3d  '", p->from.size);
		(void) fwrite(p->from.addr, 1, (int) MIN(p->from.size, 48),
			stdout);
		(void) fprintf(stdout, "'\n");
	    }
	    (void) fprintf(stdout, "\n");
	}
    }
}

static Boolean ResourceQuarkToOffset(widget_class, name, offset)
    WidgetClass widget_class;
    XrmName     name;
    Cardinal    *offset;
{
    register WidgetClass     wc;
    register Cardinal        i;
    register XrmResourceList resources;

    for (wc = widget_class;
	wc != NULL;
	wc = wc->core_class.superclass) {
	resources = (XrmResourceList) wc->core_class.resources;
	for (i = 0; i < wc->core_class.num_resources; i++, resources++) {
	    if (resources->xrm_name == name) {
		*offset = -resources->xrm_offset - 1;
		return True;
	    }
	} /* for i in resources */
    } /* for wc in widget classes */
    (*offset) = 0;
    return False;
}


static void ComputeArgs(widget, convert_args, num_args, args)
    Widget		widget;
    XtConvertArgList    convert_args;
    Cardinal		num_args;
    XrmValuePtr		args;
{
    register Cardinal   i;
    Cardinal		offset;
    char		message[1000];

    for (i = 0; i < num_args; i++) {
	args[i].size = convert_args[i].size;
	switch (convert_args[i].address_mode) {
	case XtAddress: 
	    args[i].addr = convert_args[i].address_id;
	    break;

	case XtBaseOffset:
	    args[i].addr =
		(caddr_t) ((int) widget + (int) convert_args[i].address_id);
	    break;

	case XtImmediate:
	    args[i].addr = (caddr_t) &(convert_args[i].address_id);
	    break;

	case XtResourceString:
	    /* Convert in place for next usage */
	    convert_args[i].address_mode = XtResourceQuark;
	    convert_args[i].address_id =
	       (caddr_t) XrmStringToQuark((String) convert_args[i].address_id);
	    /* Fall through */

	case XtResourceQuark:
	    if (! ResourceQuarkToOffset(widget->core.widget_class,
		    (XrmQuark) convert_args[i].address_id, &offset)) {
		(void) sprintf(message,
		    "Cannot find resource name %s as argument to conversion",
		    XrmQuarkToString((XrmQuark) convert_args[i].address_id));
		XtWarning(message);
		offset = 0;
	    }
	    args[i].addr = (caddr_t) ((int) widget + offset);
	    break;
	} /* switch */
    } /* for */
} /* ComputeArgs */

void XtDirectConvert(converter, args, num_args, from, to)
    XtConverter     converter;
    XrmValuePtr     args;
    Cardinal	    num_args;
    register XrmValuePtr from;
    XrmValuePtr     to;
{
    register CachePtr   p;
    register int	hash;
    register Cardinal   i;

    /* Try to find cache entry for conversion */
    hash = ((int)(converter) >> 2) + from->size + *((char *) from->addr);
    if (from->size > 1) hash += ((char *) from->addr)[1];
    
    for (p = cacheHashTable[hash & CACHEHASHMASK]; p != NULL; p = p->next) {
	if ((p->hash == hash)
	 && (p->converter == converter)
	 && (p->from.size == from->size)
	 && (! XtBCmp(p->from.addr, from->addr, from->size))
         && (p->num_args == num_args)) {
	    /* Are all args the same data ? */
	    for (i = 0; i < num_args; i++) {
		if (   p->args[i].size != args[i].size
		    || XtBCmp(p->args[i].addr, args[i].addr, args[i].size)) {
		    break;
		}
	    }
	    if (i == num_args) {	 
		/* Perfect match */
		(*to) = p->to;
		return;
	    }
	}
    }

    /* Didn't find it, call converter procedure and entry result in cache */
    (*to).size = 0;
    (*to).addr = NULL;
    (*converter)(args, &num_args, from, to);
    CacheEnter(converter, args, num_args, from, to, hash);
}

void _XtConvert(widget, fromType, from, toType, to)
             Widget		widget;
    register XrmRepresentation	fromType;
	     XrmValuePtr	from;
    register XrmRepresentation	toType;
    register XrmValuePtr	to;
{
    register ConverterPtr	p;
    Cardinal		num_args;
    XrmValue		stack_args[20], *args;
    char		msg[200];

    /* Look for type converter */
    p = converterTable[ProcHash(fromType, toType) & CONVERTHASHMASK];
    for (; p != NULL; p = p->next) {
	if (fromType == p->from && toType == p->to) {
	    /* Compute actual arguments from widget and arg descriptor */
	    num_args = p->num_args;
	    if (num_args > XtNumber(stack_args)) {
		args = (XrmValuePtr) XtMalloc(num_args * sizeof (XrmValue));
	    } else {
		args = stack_args;
	    }
	    if (num_args != 0)
		ComputeArgs(widget, p->convert_args, num_args, args);
	    XtDirectConvert(p->converter, args, num_args, from, to);
	    if (args != stack_args) XtFree((char *) args);
	    return;
	}
    }

    (void) sprintf(msg,
	"No type converter registered for '%s' to '%s' conversion.",
	XrmRepresentationToString(fromType),
	XrmRepresentationToString(toType));
    XtWarning(msg);
    to->addr = NULL;
    to->size = 0;
}

void XtConvert(widget, from_type_str, from, to_type_str, to)
    Widget	widget;
    XrmString	from_type_str;
    XrmValuePtr	from;
    XrmString	to_type_str;
    XrmValuePtr	to;
{
    XrmQuark    fromType, toType;

    fromType = XrmStringToRepresentation(from_type_str);
    toType = XrmStringToRepresentation(to_type_str);
    if (fromType != toType)
	_XtConvert(widget, fromType, from, toType, to);
    else
	(*to) = *from;
}
