#include <X11/copyright.h>

/* $Header: AsciiText.h,v 1.6 88/02/25 12:12:17 swick Exp $ */


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


#ifndef _AsciiText_h
#define _AsciiText_h

/****************************************************************
 *
 * AsciiText widgets
 *
 ****************************************************************/

/* Resources:

 Name		     Class		RepType		Default Value
 ----		     -----		-------		-------------
 background	     Background		pixel		White
 border		     BorderColor	pixel		Black
 borderWidth	     BorderWidth	int		1
 destroyCallback     Callback		Pointer		NULL
 displayPosition     TextPosition	int		0
 editType	     EditType		XtTextEditType	XttextRead
 file		     File		String		NULL
 height		     Height		int		font height
 insertPosition	     TextPosition	int		0
 leftMargin	     Margin		int		2
 mappedWhenManaged   MappedWhenManaged	Boolean		True
 selectTypes	     SelectTypes	Pointer		(internal)
 selection	     Selection		Pointer		empty selection
 sensitive	     Sensitive		Boolean		True
 string		     String		String		NULL
 textOptions	     TextOptions	int		0
 width		     Width		int		100
 x		     Position		int		0
 y		     Position		int		0

*/


#include <X11/Text.h>		/* AsciiText is a subclass of Text */

#define XtNstring		"string"
#define XtNfile			"file"

#define XtCString		"String"
#define XtCFile			"File"

typedef struct _AsciiStringClassRec	*AsciiStringWidgetClass;
typedef struct _AsciiStringRec		*AsciiStringWidget;

extern WidgetClass asciiStringWidgetClass;

typedef struct _AsciiDiskClassRec	*AsciiDiskWidgetClass;
typedef struct _AsciiDiskRec		*AsciiDiskWidget;

extern WidgetClass asciiDiskWidgetClass;

#endif  _AsciiText_h
