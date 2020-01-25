/*
* $Header: VPaned.h,v 1.14 88/02/26 10:14:19 swick Exp $
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

#ifndef _XtVPaned_h
#define _XtVPaned_h

#include <X11/Constraint.h>

/****************************************************************
 *
 * Vertical Paned Widget (SubClass of CompositeClass)
 *
 ****************************************************************/

/* Parameters:

 Name		     Class		RepType		Default Value
 ----		     -----		-------		-------------
 allowResize	     Boolean		Boolean		False
 background	     Background		pixel		White
 border		     BorderColor	pixel		Black
 borderWidth	     BorderWidth	int		1
 destroyCallback     Callback		Pointer		NULL
 foreground	     Foreground		Pixel		Black
 height		     Height		int		0
 gripIndent	     GripIndent		int		16
 mappedWhenManaged   MappedWhenManaged	Boolean		True
 max		     Max		int		65535
 min		     Min		int		1
 refigureMode	     Boolean		Boolean		On
 sensitive	     Sensitive		Boolean		True
 skipAdjust	     Boolean		Boolean		False
 width		     Width		int		0
 x		     Position		int		0
 y		     Position		int		0

*/


/* New Fields */
#define XtNallowResize		"allowResize"
#define XtNforeground		"foreground"
#define XtNgripIndent		"gripIndent"
#define XtNrefigureMode		"refigureMode"
#define XtNposition		"position"
#define XtNmin			"min"
#define XtNmax			"max"
#define XtNskipAdjust		"skipAdjust"

/* Class record constant */
extern WidgetClass vPanedWidgetClass;

typedef struct _VPanedClassRec	*VPanedWidgetClass;
typedef struct _VPanedRec	*VPanedWidget;

/* Public Procedures */

extern void XtPanedSetMinMax( /* panedWidget, min, max */ );
    /* Widget panedWidget;	*/
    /* int    min, max;		*/

extern void XtPanedRefigureMode( /* widget, mode */ );
    /* Widget widget;		*/
    /* Boolean  mode;		*/

extern void XtPanedGetMinMax( /* panedWidget, min, max */ );
    /* Widget panedWidget;	*/
    /* int    *min, *max;	*/ /* RETURN */

extern int XtPanedGetNumSub( /* w */ );
    /* Widget w;		*/

#endif _XtVPaned_h
/* DON'T ADD STUFF AFTER THIS #endif */
