/* $XConsortium: Form.h,v 1.16 88/10/23 13:39:30 swick Exp $ */


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

#ifndef _Form_h
#define _Form_h

#include <X11/Constraint.h>

/***********************************************************************
 *
 * Form Widget
 *
 ***********************************************************************/

/* Parameters:

 Name		     Class		RepType		Default Value
 ----		     -----		-------		-------------
 background	     Background		Pixel		XtDefaultBackground
 border		     BorderColor	Pixel		XtDefaultForeground
 borderWidth	     BorderWidth	Dimension	1
 defaultDistance     Thickness		int		4
 destroyCallback     Callback		Pointer		NULL
 height		     Height		Dimension	computed at realize
 mappedWhenManaged   MappedWhenManaged	Boolean		True
 sensitive	     Sensitive		Boolean		True
 width		     Width		Dimension	computed at realize
 x		     Position		Position	0
 y		     Position		Position	0

*/

/* Constraint parameters:

 Name		     Class		RepType		Default Value
 ----		     -----		-------		-------------
 bottom		     Edge		XtEdgeType	XtRubber
 fromHoriz	     Widget		Widget		(left edge of form)
 fromVert	     Widget		Widget		(top of form)
 horizDistance	     Thickness		int		defaultDistance
 left		     Edge		XtEdgeType	XtRubber
 resizable	     Boolean		Boolean		False
 right		     Edge		XtEdgeType	XtRubber
 top		     Edge		XtEdgeType	XtRubber
 vertDistance	     Thickness		int		defaultDistance

*/



#define XtNdefaultDistance	"defaultDistance"
#define XtNtop			"top"
#define XtNbottom		"bottom"
#define XtNleft			"left"
#define XtNright		"right"
#define XtNfromHoriz		"fromHoriz"
#define XtNfromVert		"fromVert"
#define XtNhorizDistance	"horizDistance"
#define XtNvertDistance		"vertDistance"
#define XtNresizable		"resizable"

#define XtCEdge			"Edge"
#define XtCWidget		"Widget"

#define XtRWidget		"Widget"

#ifndef _XtEdgeType_e
#define _XtEdgeType_e
typedef enum {
    XtChainTop,			/* Keep this edge a constant distance from
				   the top of the form */
    XtChainBottom,		/* Keep this edge a constant distance from
				   the bottom of the form */
    XtChainLeft,		/* Keep this edge a constant distance from
				   the left of the form */
    XtChainRight,		/* Keep this edge a constant distance from
				   the right of the form */
    XtRubber			/* Keep this edge a proportional distance
				   from the edges of the form*/
} XtEdgeType;
#endif  _XtEdgeType_e

typedef struct _FormClassRec	*FormWidgetClass;
typedef struct _FormRec		*FormWidget;

extern WidgetClass formWidgetClass;
 
#endif _Form_h

