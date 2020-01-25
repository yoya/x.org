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

/* $Header:mpelxmorg.c 5.1$ */
/* $ACIS:mpelxmorg.c 5.1$ */
/* $Source: /vice/X11/src/server/ddx/ibm/mpel/RCS/mpelxmorg.c,v $ */

#include "Xmd.h"
#include "misc.h"
#include "os.h"

static unsigned char endmask[] = 
	{ 0xff,
	  0x80,
	  0xc0,
	  0xe0,
	  0xf0,
	  0xf8,
	  0xfc,
	  0xfe };

	/*
	 * Transmogrify: To change or alter greatly and often with
	 *	grotesque or humorous effect.
	 * (Websters Ninth New Collegiate Dictionary)
	 */

mpelTransmogrify(w, h, src, dst)
    int 	w,h;
    CARD8	*src;
    CARD16	*dst;
{
    CARD8	c, *ptr;
    CARD16	shrt, shrt2;
    int i,j, srcWidthBytes, width4, height4, hmod4, row, col, maskindex;

    width4 = ((w+3)/4) ;
    height4 = ((h+3)/4) ;
    srcWidthBytes = ((w+7)/8) ;

    if (maskindex = (w%8))
	{
	ptr = src + srcWidthBytes - 1;
	for (i=0; i<h; i++)
		{
		c = *ptr & endmask[maskindex];
		*ptr = c;
		ptr += srcWidthBytes;
		}
	}

    if (width4 & 0x01)
	/* ODD */
	{
	row = height4-1;
	if (hmod4 = (h%4))
		{
		/* BOTTOM ROW */
		for (col = 0; col<width4-1; col+=2)
			{
			ptr = src + (4*row)*srcWidthBytes + col/2;
			shrt = shrt2 = 0;
			for (i=0; i< hmod4; i++)
				{
				shrt  |= (((unsigned short)((*ptr) & 0xf0)) 
						<< (8-(i*4))) ;
				shrt2 |= (((unsigned short)((*ptr) & 0x0f)) 
						<< (12-(i*4))) ;
				ptr += srcWidthBytes;
				}
			*dst++ = shrt;
			*dst++ = shrt2;
			}
		ptr = src + (4*row)*srcWidthBytes + col/2;
		shrt = 0;
		for(i=0; i<hmod4; i++)
			{
			shrt  |= (((*ptr) & 0xf0) << (8-(i*4))) ;
			ptr += srcWidthBytes;
			}
		*dst++ = shrt;
		row--;
		}

	for (; row >=0; row-- )
		{
		for (col = 0; col < ( width4 - 1) ; col+=2)
			{
			ptr = src + (4*row)*srcWidthBytes + col/2;
			shrt  = ((*ptr) & 0xf0) << 8 ;
			shrt2 = ((*ptr) & 0x0f) << 12 ;
			ptr += srcWidthBytes;
			shrt  |= ((*ptr) & 0xf0) << 4 ;
			shrt2 |= ((*ptr) & 0x0f) << 8 ;
			ptr += srcWidthBytes;
			shrt  |= ((*ptr) & 0xf0) << 0 ;
			shrt2 |= ((*ptr) & 0x0f) << 4 ;
			ptr += srcWidthBytes;
			shrt  |= ((*ptr) & 0xf0) >> 4 ;
			shrt2 |= ((*ptr) & 0x0f)      ;
			*dst++ = shrt;
			*dst++ = shrt2;
			}
		shrt = 0;
		ptr = src + (4*row)*srcWidthBytes + col/2;
		shrt   = ((*ptr) & 0xf0) << 8 ;
		ptr += srcWidthBytes;
		shrt  |= ((*ptr) & 0xf0) << 4 ;
		ptr += srcWidthBytes;
		shrt  |= ((*ptr) & 0xf0) << 0 ;
		ptr += srcWidthBytes;
		shrt  |= ((*ptr) & 0xf0) >> 4 ;
		*dst++ = shrt;
		}
	}
   else /* EVEN */
	{
	row = height4-1;
	if (hmod4 = (h%4))
		{
		/* BOTTOM ROW */
		for (col = 0; col<width4; col+=2)
			{
			ptr = src + (4*row)*srcWidthBytes + col/2;
			shrt = shrt2 = 0;
			for (i=0; i< hmod4; i++)
				{
				shrt  |= (((*ptr) & 0xf0) << (8-(i*4))) ;
				shrt2 |= (((*ptr) & 0x0f) << (12-(i*4))) ;
				ptr += srcWidthBytes;
				}
			*dst++ = shrt;
			*dst++ = shrt2;
			}
		row--;
		}

	for (; row >=0; row-- )
		for (col = 0; col<width4; col+=2)
			{
			ptr = src + (4*row)*srcWidthBytes + col/2;
			shrt  = ((*ptr) & 0xf0) << 8 ;
			shrt2 = ((*ptr) & 0x0f) << 12 ;
			ptr += srcWidthBytes;
			shrt  |= ((*ptr) & 0xf0) << 4 ;
			shrt2 |= ((*ptr) & 0x0f) << 8 ;
			ptr += srcWidthBytes;
			shrt  |= ((*ptr) & 0xf0) << 0 ;
			shrt2 |= ((*ptr) & 0x0f) << 4 ;
			ptr += srcWidthBytes;
			shrt  |= ((*ptr) & 0xf0) >> 4 ;
			shrt2 |= ((*ptr) & 0x0f)      ;
			*dst++ = shrt;
			*dst++ = shrt2;
			}
	}
}
