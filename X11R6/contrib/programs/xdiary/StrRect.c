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

#include <stdio.h>
#include <X11/StringDefs.h>
#include <X11/IntrinsicP.h>
#include "StrRect.h"
#include "StrTable.h"

/*==========================================================================*/
/*                        DrawStringInRect Routines:                        */
/*==========================================================================*/
void DrawStringInRect(
   Display* display,
   Drawable drawable,
   GC gc,
   XFontStruct* font,
   char* string,
   int length,
   XRectangle* rectangle,
   Gravity gravity,
   int xpad,
   int ypad
) {
   int ascent = font->ascent;
   int descent = font->descent;
   int string_width = XTextWidth(font, string, length);
   int string_height = ascent + descent;
   int x;
   int y;

   switch (gravity) {
   case NorthWest:
      x = rectangle->x + xpad;
      y = rectangle->y + ascent + ypad;
      break;
   case North:
      x = rectangle->x + (rectangle->width - string_width)/2;
      y = rectangle->y + ascent + ypad;
      break;
   case NorthEast:
      x = (rectangle->x + rectangle->width - 1) - string_width - xpad;
      y = rectangle->y + ascent + ypad;
      break;
   case West:
      x = rectangle->x + xpad;
      y = rectangle->y + (rectangle->height - string_height)/2 + ascent;
      break;
   case Center:
      x = rectangle->x + (rectangle->width - string_width)/2;
      y = rectangle->y + (rectangle->height - string_height)/2 + ascent;
      break;
   case East:
      x = (rectangle->x + rectangle->width - 1) - string_width - xpad;
      y = rectangle->y + (rectangle->height - string_height)/2 + ascent;
      break;
   case SouthWest:
      x = rectangle->x + xpad;
      y = (rectangle->y + rectangle->height - 1) - descent - 1 - ypad;
      break;
   case South:
      x = rectangle->x + (rectangle->width - string_width)/2;
      y = (rectangle->y + rectangle->height - 1) - descent - 1 - ypad;
      break;
   case SouthEast:
      x = (rectangle->x + rectangle->width - 1) - string_width - xpad;
      y = (rectangle->y + rectangle->height - 1) - descent - 1 - ypad;
      break;
   default:
      fprintf(stderr, "DrawStringInRect: unknown gravity\n");
      break;
   }

   XDrawString(display, drawable, gc, x, y, string, length);
}


void DrawStringsInRects(
   Display* display,
   Drawable drawable,
   GC gc,
   XFontStruct* font,
   StringTable strings,
   int* lengths,
   XRectangle* rectangles,
   int count,
   Gravity gravity,
   int xpad,
   int ypad
) {
   int ascent = font->ascent;
   int descent = font->descent;
   int string_height = ascent + descent;
   int string_width;
   int x;
   int y;
   int i;

   for (i=0; i < count; i++) {
      string_width  = XTextWidth(font, strings[i], lengths[i]);

      switch (gravity) {
      case NorthWest:
         x = rectangles[i].x + xpad;
         y = rectangles[i].y + ascent + ypad;
         break;
      case North:
         x = rectangles[i].x + (rectangles[i].width-string_width)/2;
         y = rectangles[i].y + ascent + ypad;
         break;
      case NorthEast:
         x = (rectangles[i].x + rectangles[i].width-1) - string_width - xpad;
         y = rectangles[i].y + ascent + ypad;
         break;
      case West:
         x = rectangles[i].x + xpad;
         y = rectangles[i].y + (rectangles[i].height-string_height)/2 + ascent;
         break;
      case Center:
         x = rectangles[i].x + (rectangles[i].width-string_width)/2;
         y = rectangles[i].y + (rectangles[i].height-string_height)/2 + ascent;
         break;
      case East:
         x = (rectangles[i].x + rectangles[i].width-1) - string_width - xpad;
         y = rectangles[i].y + (rectangles[i].height-string_height)/2 + ascent;
         break;
      case SouthWest:
         x = rectangles[i].x + xpad;
         y = (rectangles[i].y + rectangles[i].height-1) - descent-1 - ypad;
         break;
      case South:
         x = rectangles[i].x + (rectangles[i].width-string_width)/2;
         y = (rectangles[i].y + rectangles[i].height-1) - descent-1 - ypad;
         break;
      case SouthEast:
         x = (rectangles[i].x + rectangles[i].width-1) - string_width - xpad;
         y = (rectangles[i].y + rectangles[i].height-1) - descent-1 - ypad;
         break;
      default:
         fprintf(stderr, "DrawStringsInRects: unknown gravity\n");
         break;
      }

      XDrawString(display, drawable, gc, x, y, strings[i], lengths[i]);
   }
}
