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
/* edit.h */
#define MODBACK 1
#define MODINV 2
#define MODCOLOR 3

#define NOCONSTRAINT 0
#define XCONSTRAINT 1
#define YCONSTRAINT 2

#define WHITE 1
#define GREY 2

#define WMENU 100
#define WEDIT 514 
#define WBASE 12
#define HEDIT WEDIT

#define point 0
#define line 1
#define recta 2
#define cercle 3
#define transl 4 
#define bez 5
#define rotate 6 
#define fil 7
#define metric 8
#define write 9
#define cutap 10
#define resize 11
