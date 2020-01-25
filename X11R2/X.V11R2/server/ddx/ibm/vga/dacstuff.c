/*
 * PRPQ 5799-WZQ (C) COPYRIGHT IBM CORPORATION 1987,1988
 * LICENSED MATERIALS - PROPERTY OF IBM
 * REFER TO COPYRIGHT INSTRUCTIONS FORM NUMBER G120-2083
 */
/* $Header:dacstuff.c 1.3$ */
/* $ACIS:dacstuff.c 1.3$ */
/* $Source: /ibm/acis/usr/src/X11/server/ddx/ibm/vga/RCS/dacstuff.c,v $ */

#ifndef lint
static char *rcsid = "$Header:dacstuff.c 1.3$";
#endif

/*
 * REGISTER USAGE
 * 0x3C7 -- PEL ADDRESS
 * 0x3C8 -- PEL ADDRESS
 * 0x3C9 -- PEL DATA
 */

#include "vga_video.h"

void save_dac( tablePtr )
DAC_TABLE *tablePtr ;
{
register int i ;
register unsigned char *cptr ;

/* Point PEL Address Register To First Entry */
outb( 0x3C7, 0x0 ) ;
/* Now Read All 256 Colors -- 768 Entries R-G-B */
for ( i = 768, cptr = (unsigned char *) *tablePtr ; --i ; )
	*cptr++ = inb( 0x03C9 ) ;

return ;
}

void vgaSetColor( color, r, g, b )
int color ;
short r, b, g ;
{
register unsigned char *cptr ;

outb( 0x3C8, color ) ; /* Point PEL Address Register To First Entry */
outb( 0x3C9, r >> 10 ) ;
outb( 0x3C9, g >> 10 ) ;
outb( 0x3C9, b >> 10 ) ;

return ;
}

void restore_dac( tablePtr )
DAC_TABLE *tablePtr ;
{
register int i ;
register unsigned char *cptr ;

/* Point PEL Address Register To First Entry */
outb( 0x3C8, 0x0 ) ;
/* Now Write All 256 Colors -- 768 Entries R-G-B */
for ( i = 768, cptr = (unsigned char *) *tablePtr ; --i ; )
	outb( 0x03C9, *cptr++ ) ;

return ;
}
