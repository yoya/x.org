/*
Copyright 1987 by Digital Equipment Corporation, Maynard, Massachusetts,
and the Massachusetts Institute of Technology, Cambridge, Massachusetts.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the names of Digital or MIT not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.  

DIGITAL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

*/
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

/* $Header:aedfont.c 6.0$ */
/* $ACIS:aedfont.c 6.0$ */
/* $Source: /vice/X11/src/server/ddx/ibm/aed/RCS/aedfont.c,v $ */

#ifndef lint
static char *rcsid = "$Header:aedfont.c 6.0$";
#endif

#include "X.h"
#include "Xmd.h"
#include "Xproto.h"
#include "fontstruct.h"
#include "dixfontstr.h"
#include "scrnintstr.h"

#include "mfb.h"
#include "xaed.h"
#include "rtutils.h"

static FontPtr aedRealizedFonts[256] = { 
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

static int curfont = 1;

/*
 * Attempt to allocate a font number for use on the aed.
 */
Bool
aedRealizeFont( pscr, pFont)
    ScreenPtr	pscr;
    FontPtr	pFont;
{
    /*
     * pGC->font is now known to be valid
     */
    int			index = pscr->myNum;
    int fontnum;
    int w, h;
    int wrapped;
    TRACE(("aedRealizeFont( pscr=0x%x, pFont=0x%x)\n", pscr, pFont));
    w = GLYPHWIDTHBYTESPADDED((&(pFont->pFI->maxbounds)));
    h = GLYPHHEIGHTPIXELS((&(pFont->pFI->maxbounds)));
    if ( ( pFont->pFI->lastRow == 0 ) && ( pFont->pFI->lastCol < 256 ) && ( (w*h) < 2000 ) )
    {
	wrapped = 0;
	while ( aedRealizedFonts[curfont] != 0 )
	{
	    curfont++;
	    if ( curfont == 256 )
	    {
		curfont = 1;
		wrapped ++ ;
		if ( wrapped > 1 )
		    break;
	    }
	}
	if ( wrapped > 1 )
	{
	    ErrorF("using more that 256 fonts!!!!\n");
	    fontnum = 0;
	}
	else
	{
	    fontnum = curfont;
	    curfont++;
	}

	if (fontnum != 0)
	{
	    aedRealizedFonts[fontnum] = pFont;
	}
	pFont->devPriv[ index] = (pointer)fontnum;
    }
    else
	pFont->devPriv[ index] = (pointer)0;
    TRACE(("realized font #%d\n",(int)pFont->devPriv[index]));
    return(TRUE);
}

/*
 * tell the aed that the font is not around any more and then update our data
 * data structures.
 */
Bool
aedUnrealizeFont( pscr, pFont)
    ScreenPtr	pscr;
    FontPtr	pFont;
{
    int i;
    int fontnum;
    vforce();
    TRACE(("aedUnrealizeFont( pscr=0x%x, pFont=0x%x)\n", pscr, pFont));
    if ( ( fontnum = (int)pFont->devPriv[ pscr->myNum ] ) != 0 )
    {
	for ( i = 0 ; i < 256; i++ )
	{
	    vikint[1] = 6;	/* delete character */
	    vikint[2] = fontnum;
	    vikint[3] = i;
	    command(3);
	}
	vikint[1] = 5;	/* delete font */
	vikint[2] = fontnum;
	command(2);
	aedRealizedFonts[fontnum] = (FontPtr)NULL;
    }
    TRACE(("unrealized font #%d\n",(int)pFont->devPriv[pscr->myNum]));
    clear(2);
    return(TRUE);
}

#define vikcmd ((volatile unsigned short *)(VIKROOT + 0x4002))
aedCheckFault()
{
    short fontid;
    char charid;
    volatile unsigned short *vikptr;
    FontPtr pFont;
    CharInfoPtr ci;
    int nglyphs;
    int h,w;
    unsigned char *pglyph;
    int widthGlyph, widthShorts, oddWidth;
    int len;
    char *pdst;
    int i, j;
    volatile unsigned short *pvikcmd = vikcmd;

    TRACE(("aedCheckFault()\n"));

    vikwait();
    while( *pvikcmd != 0 )
    {
	switch( *pvikcmd )
	{
	    case 1:
		fontid = *(pvikcmd+1);
		if ( aedRealizedFonts[fontid] != 0 )
		{
		    TRACE(("Font fault on font #%d\n",fontid));
		    *(pvikcmd) = 1;	/* font available */
		    vikptr = pvikcmd+1;
		    for ( i=0; i<256; i++ )
			*vikptr++ = 0;
		    *semaphore = 0x0101;
		}
		else
		{
		    ErrorF("Font fault on unknown font\n");
		    *(pvikcmd) = 0;	/* font available */
		    *semaphore = 0x0101;
		}
		break;
	    case 2:
		fontid = *(pvikcmd+1);
		charid = (char)(*(pvikcmd+2));
		if ( aedRealizedFonts[fontid] != 0 )
		{
		    TRACE(("Char fault on font #%d, character #%d\n",fontid,charid));
		    pFont = aedRealizedFonts[fontid];
		    GetGlyphs(pFont, 1, &charid, Linear8Bit, &nglyphs, &ci);
		    if( nglyphs == 1 )
		    {
			pglyph = ((char *)pFont->pGlyphs)+ci->byteOffset;
			vikptr = pvikcmd;
			w = GLYPHWIDTHPIXELS((ci));
			h = GLYPHHEIGHTPIXELS((ci));
			widthGlyph = GLYPHWIDTHBYTESPADDED((ci));
			widthShorts = widthGlyph >> 1;
			oddWidth = widthGlyph != ( widthShorts << 1 );
			len = h*((w+15)/16) + 9;
			*vikptr++ = 1;	/* char available */
			*vikptr++ = len;
			*vikptr++ = 6;	/* move relative order */
			*vikptr++ = ci->metrics.leftSideBearing;
			*vikptr++ = -ci->metrics.ascent;
			*vikptr++ = 9;	/* draw image */
			*vikptr++ = w;
			*vikptr++ = h;
			pdst = vikptr;	 
			for ( i=0; i<h; i++ )
			{
			    for( j=0; j < widthGlyph; j++ )
				*pdst++ = *pglyph++;
			    vikptr = vikptr + widthShorts;
			    if ( oddWidth )
			    {
				vikptr++;
				*pdst++ = (char)0;
			    }
			}
			*vikptr++ = 6;	/* move relative */
			*vikptr++ = ci->metrics.characterWidth - ci->metrics.leftSideBearing;
			*vikptr++ = ci->metrics.ascent;
			*semaphore = 0x0101;
		    }
		    else
		    {
			*(pvikcmd) = 0;
			*semaphore = 0x0101;
			ErrorF("char fault on unknown character\n");
		    }
		}
		else
		{
		    *(pvikcmd) = 0;
		    *semaphore = 0x0101;
		    ErrorF("char fault on unknown font\n");
		}
		break;
		
	    default:
		{}
	}
    vikwait();
    }
}
