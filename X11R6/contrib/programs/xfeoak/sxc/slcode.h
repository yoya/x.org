/* @(#)slcode.h      6.1 94/06/01 20:03:48 FUJITSU LIMITED. */
/*

Copyright (c) 1991  FUJITSU LIMITED

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
IN NO EVENT SHALL THE FUJITSU LIMITED BE LIABLE FOR ANY CLAIM, DAMAGES
OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of the FUJITSU LIMITED shall
not be used in advertising or otherwise to promote the sale, use or
other dealings in this Software without prior written authorization
from the FUJITSU LIMITED.

  Author: Takashi Fujiwara     FUJITSU LIMITED 
                               fujiwara@a80.tech.yk.fujitsu.co.jp

*/

#ifndef slcode
#define slcode
#include "awscode.h"

#ifndef NULL
#define NULL        0
#endif
#define SS2         0x8e
#define SS3         0x8f
#define SHF1        0x28
#define SHF2        0x29

#define SLC_NF      0xff
#define SLCNF       0xffff
#define SLCFOREVER  ;;
#define SLCOFF      0
#define SLCON       1
#define SLCFALSE    0
#define SLCTRUE     1
#define SLCMAXBUF   1000

#define SLCKATAKANA 0
#define SLCHIRAGANA 1

#define SLCNTYPE    0
#define SLCHTYPE    1

#define SLCXNOCNV   0
#define SLCXCNV     2

#define SLCNOREFENV 0
#define SLCREFENV   4

#define SLCBARNO    0
#define SLCBARYES   1

#define slkana(a)   ((a) >= 0x80 && (a) <= 0xff)

#define sljgaiji(a) (*(a) < 0xa1 || *(a) > 0xfe || *(a + 1) < 0xa1 ||\
         *(a + 1) > 0xfe || (*(a) * 256 + *(a + 1) == 0x4040))

#define slugaiji(a) (*(a) < 0xa1 || *(a) > 0xfe || *(a + 1) < 0xa1 ||\
                    *(a + 1) > 0xfe)
#endif
