/*
* $Header: Logo.h,v 1.1 88/02/23 10:32:19 rws Exp $
*/

/*
Copyright 1988 by the Massachusetts Institute of Technology

Permission to use, copy, modify, and distribute this
software and its documentation for any purpose and without
fee is hereby granted, provided that the above copyright
notice appear in all copies and that both that copyright
notice and this permission notice appear in supporting
documentation, and that the name of M.I.T. not be used in
advertising or publicity pertaining to distribution of the
software without specific, written prior permission.
M.I.T. makes no representations about the suitability of
this software for any purpose.  It is provided "as is"
without express or implied warranty.
*/

#ifndef _XtLogo_h
#define _XtLogo_h

/* Parameters:

 Name		     Class		RepType		Default Value
 ----		     -----		-------		-------------
 background	     Background		pixel		White
 border		     BorderColor	pixel		Black
 borderWidth	     BorderWidth	int		1
 destroyCallback     Callback		Pointer		NULL
 foreground	     Foreground		Pixel		Black
 height		     Height		int		100
 mappedWhenManaged   MappedWhenManaged	Boolean		True
 reverseVideo	     ReverseVideo	Boolean		False
 width		     Width		int		100
 x		     Position		int		0
 y		     Position		int		0

*/


typedef struct _LogoRec *LogoWidget;
typedef struct _LogoClassRec *LogoWidgetClass;

extern WidgetClass logoWidgetClass;

#endif _XtLogo_h
