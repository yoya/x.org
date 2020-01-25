/***********************************************************
		Copyright IBM Corporation 1988

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
/* $Header:egaRslvC.c 1.2$ */
/* $ACIS:egaRslvC.c 1.2$ */
/* $Source: /ibm/acis/usr/src/X11/server/ddx/ibm/ega/RCS/egaRslvC.c,v $ */

#ifndef lint
static char *rcsid = "$Header:egaRslvC.c 1.2$";
#endif

#include "X.h"
#include "screenint.h"
#include "rtutils.h"

void
egaResolveColor( pRed, pGreen, pBlue, pVisual )
register unsigned short * const pRed ;
register unsigned short * const pGreen ;
register unsigned short * const pBlue ;
register VisualPtr pVisual ;
{ 
register unsigned long int tmp ;

TRACE( ( "egaResolveColor: rgb, *rgb = x%x, x%x, x%x :: %d, %d, %d\n",
	pRed, pGreen, pBlue, *pRed, *pGreen, *pBlue ) ) ;

/* Use Top Two Bits -- ROUND To Nearest Color */
*pRed   = ( ( tmp =   *pRed + 0x2000 ) > 0xC000 ) ? 0xC000 : ( tmp & 0xC000 ) ;
*pGreen = ( ( tmp = *pGreen + 0x2000 ) > 0xC000 ) ? 0xC000 : ( tmp & 0xC000 ) ;
*pBlue  = ( ( tmp =  *pBlue + 0x2000 ) > 0xC000 ) ? 0xC000 : ( tmp & 0xC000 ) ;

TRACE( ( "leaving egaResolveColor: rgb, *rgb = x%x, x%x, x%x :: %d, %d, %d\n",
	pRed, pGreen, pBlue, *pRed, *pGreen, *pBlue ) ) ;

return ;
}
