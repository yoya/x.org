#include <X11/copyright.h>

/* $Header: Simple.h,v 1.2 88/02/14 14:09:59 rws Exp $ */
/* Copyright	Massachusetts Institute of Technology	1987 */

#ifndef _Simple_h
#define _Simple_h

/****************************************************************
 *
 * Simple widgets
 *
 ****************************************************************/

/* Resources:

 Name		     Class		RepType		Default Value
 ----		     -----		-------		-------------
 background	     Background		pixel		White
 border		     BorderColor	pixel		Black
 borderWidth	     BorderWidth	int		1
 cursor		     Cursor		Cursor		None
 destroyCallback     Callback		Pointer		NULL
 height		     Height		int		0
 insensitiveBorder   Insensitive	Pixmap		Gray
 mappedWhenManaged   MappedWhenManaged	Boolean		True
 sensitive	     Sensitive		Boolean		True
 width		     Width		int		0
 x		     Position		int		0
 y		     Position		int		0

*/

#define XtNcursor		"cursor"
#define XtNinsensitiveBorder	"insensitiveBorder"

#define XtCCursor		"Cursor"
#define XtCInsensitive		"Insensitive"


typedef struct _SimpleClassRec	*SimpleWidgetClass;
typedef struct _SimpleRec	*SimpleWidget;

extern WidgetClass simpleWidgetClass;

#endif  _Simple_h
