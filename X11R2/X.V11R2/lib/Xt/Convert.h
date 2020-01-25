/* $Header: Convert.h,v 1.3 88/02/26 12:31:31 swick Exp $ */
/*
 *	sccsid:	%W%	%G%
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

#ifndef _Convert_h
#define _Convert_h


/****************************************************************
 *
 * Resource Conversions
 *
 ****************************************************************/

typedef enum {
/* address mode     	parameter representation    */
/* ------------     	------------------------    */
    XtAddress,      	/* address		    */
    XtBaseOffset,   	/* offset		    */
    XtImmediate,    	/* constant		    */
    XtResourceString,	/* resource name string     */
    XtResourceQuark	/* resource name quark      */
} XtAddressMode;

typedef struct {
    XtAddressMode   address_mode;
    caddr_t	    address_id;
    Cardinal	    size;
} XtConvertArgRec, *XtConvertArgList;

typedef (*XtConverter)();
    /* XrmValue		    *args;	    */
    /* Cardinal		    *num_args;       */
    /* XrmValue		    *from;	    */
    /* Xrmvalue		    *to;	    */

extern void _XtAddConverter();
    /* XrmRepresentation    from_type;      */
    /* XrmRepresentation    to_type;	    */
    /* XtTypeConverter      converter;      */
    /* XtConvertArgList     convert_args;   */
    /* Cardinal		    num_args;       */

extern void XtAddConverter();
    /* String		    from_type;      */
    /* String		    to_type;	    */
    /* XtTypeConverter      converter;      */
    /* XtConvertArgList     convert_args;   */
    /* Cardinal		    num_args;   */

extern void _XtConvert();
    /* Widget		    widget;	    */
    /* XrmRepresentation    from_type       */
    /* XrmValue		    *from;	    */
    /* XrmRepresentation    to_type;	    */
    /* XrmValue		    *to;	    */  /* RETURN */

extern void XtConvert();
    /* Widget		    widget;	    */
    /* String		    from_type;	    */
    /* XrmValue		    *from;	    */
    /* String		    to_type;	    */
    /* XrmValue		    *to;	    */  /* RETURN */


extern void XtDirectConvert();
    /* XtConverter	    converter;      */
    /* XrmValuePtr	    args;	    */
    /* Cardinal		    num_args;       */
    /* XrmValuePtr	    from;	    */
    /* XrmValue		    *to;	    */  /* RETURN */


/****************************************************************
 *
 * Utility procedures
 *
 ****************************************************************/

extern void LowerCase(); /* source, dest */
    /* char *source;    */
    /* char *dest;      */  /* RETURN */


#endif _Convert_h
/* DON'T ADD STUFF AFTER THIS #endif */
