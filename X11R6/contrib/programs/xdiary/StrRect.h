/*
 * Copyright (c) 1994  Harris Computer Systems Division
 *
 * The X Consortium, and any party obtaining a copy of these files from the X
 * Consortium, directly or indirectly, is granted, free of charge, a full and
 * unrestricted irrevocable, world-wide, paid up, royalty-free, nonexclusive
 * right and license to deal in this software and documentation files (the
 * "Software"), including without limitation the rights to use, copy, modify,
 * merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons who receive copies from any such party to do so.
 * This license includes without limitation a license to do the foregoing
 * actions under any patents of the party supplying this software to the X
 * Consortium.
 * 
 * BECAUSE THE PROGRAM IS LICENSED FREE OF CHARGE, THERE IS NO WARRANTY FOR
 * THE PROGRAM, TO THE EXTENT PERMITTED BY APPLICABLE LAW.  EXCEPT WHEN
 * OTHERWISE STATED IN WRITING THE COPYRIGHT HOLDERS AND/OR OTHER PARTIES
 * PROVIDE THE PROGRAM "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED
 * OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE ENTIRE RISK AS
 * TO THE QUALITY AND PERFORMANCE OF THE PROGRAM IS WITH YOU.  SHOULD THE
 * PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY SERVICING,
 * REPAIR OR CORRECTION.
 * 
 * IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN WRITING WILL
 * ANY COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MAY MODIFY AND/OR REDISTRIBUTE
 * THE PROGRAM AS PERMITTED ABOVE, BE LIABLE TO YOU FOR DAMAGES, INCLUDING ANY
 * GENERAL, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING OUT OF THE
 * USE OR INABILITY TO USE THE PROGRAM (INCLUDING BUT NOT LIMITED TO LOSS OF
 * DATA OR DATA BEING RENDERED INACCURATE OR LOSSES SUSTAINED BY YOU OR THIRD
 * PARTIES OR A FAILURE OF THE PROGRAM TO OPERATE WITH ANY OTHER PROGRAMS),
 * EVEN IF SUCH HOLDER OR OTHER PARTY HAS BEEN ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGES.
 */

#ifndef _DrawStringInRect_h
#define _DrawStringInRect_h

#include "Gravity.h"
#include "StrTable.h"

/*
 * The DrawStringInRect routine draws text in a specified XtRectangle at the
 * location specified by the Gravity parameter.  
 *
 * The DrawStringsInRects routine does the same thing, but for multiple
 * strings.  It is passed a pointer to an array of strings, an array of string
 * lengths, and an array of rectangles, thus making it possible for each
 * string to specify a different rectangle.
 *
 * Care must be taken to ensure that the XFontStruct passed in matches the
 * font that is specified in the GC that is passed in, otherwise the extent
 * measurements will be incorrect and the string will be drawn incorrectly.
 *
 * Absolutely no error checking is done.  The application is responsible for
 * providing a rectangle large enough for the specified string.
 *
 * You must include this file in your widget or application to use the
 * DrawStringInRect routines.
 */

extern void DrawStringInRect(
   Display*,      /* display */
   Drawable,      /* drawable */
   GC,            /* gc */
   XFontStruct*,  /* font */
   char*,         /* string */
   int,           /* length */
   XRectangle*,   /* rectangle */
   Gravity,       /* gravity */
   int,           /* x padding */
   int            /* y padding */
);

extern void DrawStringsInRects(
   Display*,      /* display */
   Drawable,      /* drawable */
   GC,            /* gc */
   XFontStruct*,  /* font */
   StringTable,   /* strings */
   int*,          /* lengths */
   XRectangle*,   /* rectangles */
   int,           /* count */
   Gravity,       /* gravity */
   int,           /* x padding */
   int            /* y padding */
);

#endif /* _DrawStringInRect_h */
