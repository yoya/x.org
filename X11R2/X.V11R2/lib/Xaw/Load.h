/*
* $Header: Load.h,v 1.6 88/02/26 09:32:04 swick Exp $
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

#ifndef _XtLoad_h
#define _XtLoad_h

/***********************************************************************
 *
 * Load Widget
 *
 ***********************************************************************/

/* Parameters:

 Name		     Class		RepType		Default Value
 ----		     -----		-------		-------------
 background	     Background		pixel		White
 border		     BorderColor	pixel		Black
 borderWidth	     BorderWidth	int		1
 destroyCallback     Callback		Pointer		NULL
 font		     Font		XFontStruct*	fixed
 foreground	     Foreground		Pixel		Black
 getLoadProc	     Callback		Callback	(internal)
 height		     Height		int		120
 label		     Label		String		(empty string)
 mappedWhenManaged   MappedWhenManaged	Boolean		True
 minScale	     Scale		int		1
 reverseVideo	     ReverseVideo	Boolean		False
 scale		     Scale		int		1
 update		     Interval		int		5 (seconds)
 width		     Width		int		120
 x		     Position		int		0
 y		     Position		int		0

*/


#define XtNupdate		"update"
#define XtNscale		"scale"
#define XtNvmunix		"vmunix"
#define XtNminScale		"minScale"
#define XtNgetLoadProc		"getLoadProc"
 
#define XtCScale		"Scale"

typedef struct _LoadRec *LoadWidget;  /* completely defined in LoadPrivate.h */
typedef struct _LoadClassRec *LoadWidgetClass;    /* completely defined in LoadPrivate.h */

extern WidgetClass loadWidgetClass;

#endif _XtLoad_h
/* DON'T ADD STUFF AFTER THIS #endif */
