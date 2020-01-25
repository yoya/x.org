/*    Copyright 1989-94 Daniel Dardailler.
Permission to use, copy, modify, distribute, and sell this software
and its documentation for any purpose is hereby granted without fee,
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in
supporting documentation, and that the name of Daniel Dardailler not
be used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.  Daniel
Dardailler makes no representations about the suitability of this
software for any purpose.  It is provided "as is" without express or
implied warranty.  
*/
/* clientimage.h */
#ifndef _XLIB_H_
#include <X11/Xlib.h>
#endif

#define VIDAND		1	/*	 S .  D	-> D */
#define VIDSTR		3	/*	 S	-> D */
#define VIDXOR		6	/*	 S ^  D -> D */
#define VIDOR		7       /*	 S +  D -> D */

typedef XImage ClientImage ;
