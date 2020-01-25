/*
* $Header: Grip.h,v 1.6 88/02/26 09:17:54 swick Exp $
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

/*
 *  Grip.h - Public Definitions for Grip widget (used by VPane Widget)
 *
 */

#ifndef _XtGrip_h
#define _XtGrip_h

/***************************************************************************
 *
 * Grip Widget 
 *
 **************************************************************************/

/* Parameters:

 Name		     Class		RepType		Default Value
 ----		     -----		-------		-------------
 background	     Background		pixel		Black
 border		     BorderColor	pixel		Black
 borderWidth	     BorderWidth	int		0
 callback	     Callback		Pointer		GripAction
 destroyCallback     Callback		Pointer		NULL
 height		     Height		int		6
 mappedWhenManaged   MappedWhenManaged	Boolean		True
 sensitive	     Sensitive		Boolean		True
 width		     Width		int		6
 x		     Position		int		0
 y		     Position		int		0

*/


#define XtNgripTranslations	"gripTranslations"

typedef struct {
  XEvent *event;		/* the event causing the GripAction */
  String *params;		/* the TranslationTable params */
  Cardinal num_params;		/* count of params */
} GripCallDataRec, *GripCallData;

/* Class Record Constant */

extern WidgetClass gripWidgetClass;

typedef struct _GripClassRec *GripWidgetClass;
typedef struct _GripRec      *GripWidget;

#endif _XtGrip_h
