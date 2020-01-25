/*
 * PRPQ 5799-WZQ (C) COPYRIGHT IBM CORPORATION 1987,1988
 * LICENSED MATERIALS - PROPERTY OF IBM
 * REFER TO COPYRIGHT INSTRUCTIONS FORM NUMBER G120-2083
 */
/* $Header:vgaRslvC.c 1.4$ */
/* $ACIS:vgaRslvC.c 1.4$ */
/* $Source: /ibm/acis/usr/src/X11/server/ddx/ibm/vga/RCS/vgaRslvC.c,v $ */

#ifndef lint
static char *rcsid = "$Header:vgaRslvC.c 1.4$";
#endif

/*
 * Modified From Brecon Code
 */

#include "X.h"
#include "screenint.h"
#include "rtutils.h"

void
vgaResolveColor( pRed, pGreen, pBlue, pVisual )
register unsigned short * const pRed ;
register unsigned short * const pGreen ;
register unsigned short * const pBlue ;
register VisualPtr pVisual ;
{ 
register unsigned long int tmp ;

TRACE( ( "vgaResolveColor: rgb, *rgb = x%x, x%x, x%x :: %d, %d, %d\n",
	pRed, pGreen, pBlue, *pRed, *pGreen, *pBlue ) ) ;

/* Use Top Six Bits -- ROUND To Nearest Color */
*pRed   = ( ( tmp =   *pRed + 0x100 ) > 0xFC00 ) ? 0xFC00 : ( tmp & 0xFC00 ) ;
*pGreen = ( ( tmp = *pGreen + 0x100 ) > 0xFC00 ) ? 0xFC00 : ( tmp & 0xFC00 ) ;
*pBlue  = ( ( tmp =  *pBlue + 0x100 ) > 0xFC00 ) ? 0xFC00 : ( tmp & 0xFC00 ) ;

TRACE( ( "leaving vgaResolveColor: rgb, *rgb = x%x, x%x, x%x :: %d, %d, %d\n",
	pRed, pGreen, pBlue, *pRed, *pGreen, *pBlue ) ) ;

return ;
}

void
vgaResolveToGray( pRed, pGreen, pBlue, pVisual )
register unsigned short * const pRed ;
register unsigned short * const pGreen ;
register unsigned short * const pBlue ;
register VisualPtr pVisual ;
{ 
register unsigned long int tmp ;

TRACE( ( "vgaResolveToGray: rgb, *rgb = x%x, x%x, x%x :: %d, %d, %d\n",
	pRed, pGreen, pBlue, *pRed, *pGreen, *pBlue ) ) ;

/* Convert To Resonable Approx. Of Gray */
tmp = ( ( 30 * *pRed ) + ( 59 * *pGreen ) + ( 11 * *pBlue ) ) / 100 ;

/* Use Top Six Bits -- ROUND To Nearest Color */
if ( ( tmp += 0x100 ) > 0xFC00 )
	tmp = 0xFC00 ;
else
	tmp &= 0xFC00 ;
*pRed   =
*pGreen =
*pBlue = tmp ;

TRACE( ( "leaving vgaResolveToGray: rgb, *rgb = x%x, x%x, x%x :: %d, %d, %d\n",
	pRed, pGreen, pBlue, *pRed, *pGreen, *pBlue ) ) ;

return ;
}
