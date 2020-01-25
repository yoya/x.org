/* @(#)XimSwap.c      6.1 94/06/01 20:04:28 FUJITSU LIMITED. */
/*

Copyright (c) 1992, 1993, 1994  FUJITSU LIMITED

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
          Riki Kawaguchi       FUJITSU LIMITED
	  Toshimitu Terazono   FUJITSU LIMITED

*/

/*
 *  xfeoak : Input Server (kana-kanji covertion IM Server)
 */

#include <X11/Xlibint.h>
#include "XimProto.h"
#include "Xfeoak.h"

Static CARD16
_XimCard16(val)
    CARD16	 val;
{
    return val;
}

Static CARD16
_XimSwapCard16(val)
    CARD16	 val;
{
    return (val << 8) & 0xff00 |
           (val >> 8) & 0xff;
}

Static CARD32
_XimCard32(val)
    CARD32	val;
{
    return val;
}

Static CARD32
_XimSwapCard32(val)
    CARD32	val;
{
    return (val << 24) & 0xff000000 |
	   (val <<  8) & 0xff0000   |
	   (val >>  8) & 0xff00     |
	   (val >> 24) & 0xff;
}

void
XimSetEndian()
{
    CARD8		endian = current_connect->byte_order;
    static CARD16	tesr_card = 1;

    if ((endian == BIGENDIAN    && *((CARD8 *)&tesr_card) == 0)
     || (endian == LITTLEENDIAN && *((CARD8 *)&tesr_card) != 0)) {
	current_connect->Card16 = _XimCard16;
	current_connect->Card32  = _XimCard32;
    } else {
	current_connect->Card16 = _XimSwapCard16;
	current_connect->Card32  = _XimSwapCard32;
    }
    return;
}
