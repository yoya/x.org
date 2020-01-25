/* @(#)awscode.h      6.1 94/06/01 20:03:48 FUJITSU LIMITED. */
/*

Copyright (c) 1985 - 1991  FUJITSU LIMITED

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

/*
 * Definitions about Code-Types / 1985 Jun. 7
 */
 
/* (1) Definitions about Code-Types Composed of 1-byte-Unit */
#define C_NONE1   0x0000    /* Code-Type is Undefined    */
#define C_ASCII   0x0100    /* (7-bit)ASCII              */
#define C_JIS8    0x0200    /* JIS8                      */
#define C_UXJIS   0x0300    /* UNIX JIS                  */
#define C_USASCI  0x0400    /* U.S.ASCII                 */
#define C_JPNKAT  0x0500    /* EBCDIC(Japanese Katakana) */
#define C_JPNENG  0x0600    /* EBCDIC(Japanese English)  */
#define C_EBCDEF  0x0700    
 
/* (2) Definitions about Code-Types Composed of 2-byte-Unit */
#define C_NONE2   0x0000    /* Code-Type is Undefined    */
#define C_SHFJIS  0x0001    /* Shift JIS                 */
#define C_JEF     0x0002    /* JEF                       */
#define C_JIS     0x0003    /* JIS                       */
 
/* (3) Check Slash ('/' in Shift-JIS Code) or Not */
#define SFTH      0x81      /* Slash('/') Code in Shift-JIS */
#define SL_SO     0x0e      
#define SL_SI     0x0f      
#define SL_SHF1   0x28      
#define SL_SHF2   0x29      
#define SL_SS2    0x8e      
#define SL_SS3    0x8f      
#define SL_SFTH   0x81      
#define SL_SFTL   0x5e      
 
#define SFTJIS(sft) \
        (*sft++ == SFTH ? \
          (*sft == SFTL ? (u->u_dirp = ++sft, 1) : 0) : 0)
