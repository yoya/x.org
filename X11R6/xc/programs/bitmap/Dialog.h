/* $XConsortium: Dialog.h,v 1.6 94/04/17 20:23:42 converse Exp $ */
/*

Copyright (c) 1989  X Consortium

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE X CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of the X Consortium shall
not be used in advertising or otherwise to promote the sale, use or
other dealings in this Software without prior written authorization
from the X Consortium.

*/

/*
 * Author:  Davor Matic, MIT X Consortium
 */


/*#define None   0*/
#define Yes    1<<1
#define No     1<<2
#define Maybe  1<<3  /* :-) */
#define Okay   1<<4
#define Abort  1<<5
#define Cancel 1<<6
#define Retry  1<<7

typedef struct {
  Widget top_widget, shell_widget, dialog_widget;
  unsigned long options;
} _Dialog, *Dialog;

typedef struct {
    String name;
    unsigned long flag;
} DialogButton;

extern Dialog CreateDialog();
extern unsigned long PopupDialog();
extern void PopdownDialog();
