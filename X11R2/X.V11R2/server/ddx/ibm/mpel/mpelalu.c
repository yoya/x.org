/***********************************************************
		Copyright IBM Corporation 1987

                      All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the name of IBM not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.  

IBM DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
IBM BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/

/* $Header:mpelalu.c 5.1$ */
/* $ACIS:mpelalu.c 5.1$ */
/* $Source: /vice/X11/src/server/ddx/ibm/mpel/RCS/mpelalu.c,v $ */

#ifndef lint
static char *rcsid = "$Header:mpelalu.c 5.1$";
#endif

#include "Xmd.h"

#include "rtutils.h"

#include "mpelfifo.h"

/*
 * XMask(xmask)
 *
 * Megapel mask works as follows:
 * | pl0|pl1....pl7|00000000|
 *      16-bit integer
 */

CARD16	mpelMaskXlate[256];

mpelInitPlaneMask()
{
    int	i;

    TRACE(("mpelInitPlaneMask()\n"));

    for(i=0; i<256; i++)
    {
	mpelMaskXlate[i] = (CARD16) (((CARD8) ~i) << 8);
	/* TESTING NEW CODE
	mpelMaskXlate[i] = ~0;
	if (i & 0x01) mpelMaskXlate[i] &= 0x7f00;
	if (i & 0x02) mpelMaskXlate[i] &= 0xbf00;
	if (i & 0x04) mpelMaskXlate[i] &= 0xdf00;
	if (i & 0x08) mpelMaskXlate[i] &= 0xef00;
	if (i & 0x10) mpelMaskXlate[i] &= 0xf700;
	if (i & 0x20) mpelMaskXlate[i] &= 0xfb00;
	if (i & 0x40) mpelMaskXlate[i] &= 0xfd00;
	if (i & 0x80) mpelMaskXlate[i] &= 0xfe00;
	***/
    }
}

