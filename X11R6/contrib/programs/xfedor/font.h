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
/* font.h */
/* parties communes a tr_font et tr_extrafont */
/* HMENU deja defined */

#include "bitmaps/left"
#include "bitmaps/right"

#define LEFT 1
#define RIGHT 2
     
#define WSCROLL 35
#define HSCROLL 25
#define HFONT  (HMENU + HSCROLL)
#define HFONTMAX (HFONT + 128)
#define WFONT 896		/* 56*16, 28*32, 14*64, 7*128 */

#define WLIFT  (WFONT - 2*WSCROLL)
