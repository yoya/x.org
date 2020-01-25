/* @(#)slcetoa.c      6.1 94/06/01 20:03:48 FUJITSU LIMITED. */
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

#include <sys/types.h>
#include "slcode.h"

#ifndef SLC_NF
#define SLC_NF     0xff
#endif
#ifndef SLCNF
#define SLCNF      0xffff
#endif

/*
 * 0byte is JAPANESE EBCDIC KANA
 * 1byte is JAPANESE EBCDIC ENGLISH
 * 2byte is USASCII
 */

extern unsigned char slceatb[][3];

int slcetoa(incode, ebctype)
     register unsigned char incode;
     register int ebctype;
     {
     register int type;
     register unsigned char (*ea)[3] = slceatb;

     if(ebctype == C_USASCI)
          type = 2;
     else if(ebctype == C_JPNKAT)
          type = 0;
     else
          type = 1;
     if(ea[(unsigned)incode & 0377][type] == SLC_NF)
          return(-1);
     else
          return(ea[(unsigned)incode & 0377][type] & 0xff);
     }
