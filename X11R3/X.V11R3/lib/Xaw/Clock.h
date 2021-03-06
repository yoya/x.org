/*
* $XConsortium: Clock.h,v 1.23 88/10/23 13:25:38 swick Exp $
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

#ifndef _XtClock_h
#define _XtClock_h

/***********************************************************************
 *
 * Clock Widget
 *
 ***********************************************************************/

/* Parameters:

 Name		     Class		RepType		Default Value
 ----		     -----		-------		-------------
 analog		     Boolean		Boolean		True
 background	     Background		Pixel		white
 backingStore	     BackingStore	BackingStore	default
 border		     BorderColor	Pixel		Black
 borderWidth	     BorderWidth	Dimension	1
 chime		     Boolean		Boolean		False
 destroyCallback     Callback		Pointer		NULL
 font		     Font		XFontStruct*	fixed
 foreground	     Foreground		Pixel		black
 hand		     Foreground		Pixel		black
 height		     Height		Dimension	164
 highlight	     Foreground		Pixel		black
 mappedWhenManaged   MappedWhenManaged	Boolean		True
 padding	     Margin		int		8
 reverseVideo	     ReverseVideo	Boolean		False
 update		     Interval		int		60 (seconds)
 width		     Width		Dimension	164
 x		     Position		Position	0
 y		     Position		Position	0

*/


#include <X11/Xmu.h>

/* Resource names used to the clock widget */

#define XtNupdate		"update"
		/* Int: how often to update hands? */
#define XtNhand			"hands"
		/* color of hands */
#define XtNhighlight		"highlight"
		/* color of hand outline */
#define XtNanalog		"analog"
		/* Boolean: digital if FALSE */
#define XtNbackingStore		"backingStore"
		/* when server should use backing store */
#define XtNchime		"chime"
		/* Boolean:  */
#define XtNpadding		"padding"
		/* Int: amount of space around outside of clock */

typedef struct _ClockRec *ClockWidget;  /* completely defined in ClockPrivate.h */
typedef struct _ClockClassRec *ClockWidgetClass;    /* completely defined in ClockPrivate.h */

extern WidgetClass clockWidgetClass;

#endif _XtClock_h
/* DON'T ADD STUFF AFTER THIS #endif */
