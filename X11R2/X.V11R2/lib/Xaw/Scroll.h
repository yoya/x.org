/*
* $Header: Scroll.h,v 1.10 88/02/26 09:34:19 swick Exp $
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

#ifndef _Scroll_h
#define _Scroll_h

/****************************************************************
 *
 * Scrollbar Widget
 *
 ****************************************************************/


/* Parameters:

 Name		     Class		RepType		Default Value
 ----		     -----		-------		-------------
 background	     Background		pixel		White
 border		     BorderColor	pixel		Black
 borderWidth	     BorderWidth	int		1
 destroyCallback     Callback		Pointer		NULL
 foreground	     Color		pixel		Black
 height		     Height		int		length or thickness
 length		     Length		int		1
 mappedWhenManaged   MappedWhenManaged	Boolean		True
 orientation	     Orientation	XtOrientation	XtorientVertical
 scrollDCursor	     ScrollDCursor	Cursor		XC_sb_down_arrow
 scrollHCursor	     ScrollHCursor	Cursor		XC_sb_h_double_arrow
 scrollLCursor	     ScrollLCursor	Cursor		XC_sb_left_arrow
 scrollProc	     Callback		Pointer		NULL
 scrollRCursor	     ScrollRCursor	Cursor		XC_sb_right_arrow
 scrollUCursor	     ScrollUCursor	Cursor		XC_sb_up_arrow
 scrollVCursor	     ScrollVCursor	Cursor		XC_sb_v_double_arrow
 sensitive	     Sensitive		Boolean		True
 shown		     Shown		float		0.0
 thickness	     Thickness		int		14
 thumb		     Thumb		Pixmap		Grey
 thumbProc	     Callback		Pointer		NULL
 top		     Top		float		0.0
 width		     Width		int		thickness or length
 x		     Position		int		0
 y		     Position		int		0

*/

#define XtNlength		"length"
#define XtNorientation		"orientation"
#define XtNscrollProc		"scrollProc"
#define XtNscrollDCursor	"scrollDownCursor"
#define XtNscrollHCursor	"scrollHorizontalCursor"
#define XtNscrollLCursor	"scrollLeftCursor"
#define XtNscrollRCursor	"scrollRightCursor"
#define XtNscrollUCursor	"scrollUpCursor"
#define XtNscrollVCursor	"scrollVerticalCursor"
#define XtNshown		"shown"
#define XtNthickness		"thickness"
#define XtNthumb		"thumb"
#define XtNthumbProc		"thumbProc"
#define XtNtop			"top"

#define XtCShown		"Shown"
#define XtCTop			"Top"

#ifndef _XtOrientation_e
#define _XtOrientation_e

typedef enum {XtorientHorizontal, XtorientVertical} XtOrientation;
#endif _XtOrientation_e

typedef struct _ScrollbarRec	  *ScrollbarWidget;
typedef struct _ScrollbarClassRec *ScrollbarWidgetClass;

extern WidgetClass scrollbarWidgetClass;

extern void XtScrollBarSetThumb(); /* scrollBar, top, shown */
/* Widget scrollBar; */
/* float top, shown; */

#endif _Scroll_h
