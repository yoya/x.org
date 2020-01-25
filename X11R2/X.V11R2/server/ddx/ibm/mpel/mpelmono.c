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

/* $Header:mpelmono.c 5.0$ */
/* $ACIS:mpelmono.c 5.0$ */
/* $Source: /vice/X11/src/server/ddx/ibm/mpel/RCS/mpelmono.c,v $ */

#ifndef lint
static char *rcsid = "$Header:mpelmono.c 5.0$";
#endif

/*
 * Megapel bitmap fill with solid foreground and transparent background
 *
 * Paquin 10/87
 *
 */

#include "X.h"
#include "Xmd.h"
#include "os.h"

#include "rtutils.h"

#include "mpel.h"
#include "mpelhdwr.h"
#include "mpelfifo.h"

#ifdef __HIGHC__
#define MOVE _move
#else
ERROR!! No equivalent to _move for pcc
#endif

/***==================================================================***/

mpelUnnaturalMonoByteImage(data, x,y,w,h, fg, alu, planes )
    unsigned char	*data;
    int			x, y, w, h, fg, alu, planes;
{
    int 	totwidthBytes, linesPerBlock, bytes, bytesPerBlock;

    TRACE(("mpelUnnaturalMonoByteImage(0x%x,%d,%d,%d,%d,%d,%d,0x%x)\n",
						data,x,y,w,h,fg,alu,planes));

    totwidthBytes = ((w+7)/8);
    linesPerBlock = MPEL_BLIT_STAGE_SIZE / totwidthBytes;

    /* the following *should* be impossible */
    if ( (linesPerBlock*totwidthBytes) > MPEL_BLIT_STAGE_SIZE )
	{
	    ErrorF("Fatal MonoByte: too big (internal)\n");
	    return;
	}

    bytes = totwidthBytes * h;
    bytesPerBlock = totwidthBytes * linesPerBlock;

    while (bytes>0)
	{
	    mpelDrawMonoByteImage(data,x,y,w,linesPerBlock,fg,alu,planes);
	    y += linesPerBlock;
	    data += bytesPerBlock;
	    bytes -= bytesPerBlock;
	}
}

/***==================================================================***/

mpelUnnaturalMonoImage(data, x,y,w,h, fg, alu, planes )
    unsigned char	*data;
    int			x, y, w, h, fg, alu, planes;
{
    int		 	totwidthBytes, linesPerBlock, bytes, bytesPerBlock;

    TRACE(("mpelUnnaturalMonoImage(0x%x,%d,%d,%d,%d,%d,%d,0x%x)\n",
						data,x,y,w,h,fg,alu,planes));

    totwidthBytes = ((w+31)/32)<<2;
    linesPerBlock = MPEL_BLIT_STAGE_SIZE / totwidthBytes;

    /* the following *should* be impossible */
    if ( (linesPerBlock*totwidthBytes) > MPEL_BLIT_STAGE_SIZE )
    	{
	    ErrorF("Fatal Mono: too big (internal)\n");
 	    return;
	}

    bytes = totwidthBytes * h;
    bytesPerBlock = totwidthBytes * linesPerBlock;

    while (bytes>0)
	{
	    mpelDrawMonoImage(data, x,y, w,linesPerBlock, fg, alu, planes);
	    y += linesPerBlock;
	    data += bytesPerBlock;
	    bytes -= bytesPerBlock;
	}
}

/***==================================================================***/

mpelDrawMonoImage(data, x,y,w,h, fg, alu, planes )
    unsigned char	*data;
    int			x, y, w, h, fg, alu, planes;
{
    unsigned char 			*srcPtr, *dstPtr;
    int 				i,j;
    int 				cleanBytes, PadBytes;
    int 				totwidthBytes, realwidthBytes ;
    int 				lastbyteMask ;
    int 				PaddedWidthInBits;
    mpelSrcBLTVPMWColorExpansion 	block;

    TRACE(("mpelDrawMonoImage(0x%x,%d,%d,%d,%d,%d,%d,0x%x)\n",
						data,x,y,w,h,fg,alu,planes));

    totwidthBytes = ((w+31)/32)<<2;
    realwidthBytes = (w+7)/8;
    if ((realwidthBytes*h) > MPEL_BLIT_STAGE_SIZE )
    	{
	    mpelUnnaturalMonoImage(data, x,y,w,h, fg, alu, planes );
	    return;
    	}

    MPELWaitFifo();
    mpelCheckCursor(x,y,w,h);

    PadBytes = totwidthBytes - realwidthBytes;

    if (i = (w%8))
	{
	    lastbyteMask = ((~0) << (8 - i));
	    PaddedWidthInBits = (w & ~7) + 8;
	}
	else {
	    lastbyteMask = ~0;
	    PaddedWidthInBits = w;
	}

    mpelSetPlaneMask(planes);


    /* copy image TO adapter buffer area */
    srcPtr = (unsigned char *) data;

    dstPtr = (unsigned char *) MPEL_BLIT_STAGE;

    cleanBytes = realwidthBytes-1;
    for (j=0; j<h; j++) 
	{
	    if (cleanBytes)
		{
		    MOVE(srcPtr, dstPtr, cleanBytes);
		    srcPtr += cleanBytes;
		    dstPtr += cleanBytes;
		}

	    /* mask last byte */
	    *dstPtr++ = *srcPtr++ & lastbyteMask; 

	    srcPtr += PadBytes;
	}

    block.color		= fg;
    block.srcaddr     	= mpelAddr(MPEL_BLIT_STAGE);
    block.dest.lleft.x	= x;
    block.dest.lleft.y	= 1023 - (y + h - 1);
    block.dest.uright.x	= x + PaddedWidthInBits - 1;
    block.dest.uright.y	= 1023 - y;
    block.alu		= alu+1;

    MPELSrcBLTVPMWColorExpansionROP(&block);

    MPELWaitFifo();

    MPELResetPlaneMask();
    mpelRestoreCursor();
}

/***==================================================================***/

mpelDrawMonoByteImage(data, x,y,w,h, fg, alu, planes )
    unsigned char	*data;
    int			x, y, w, h, fg, alu, planes;
{
    unsigned char	*srcPtr, *dstPtr;
    int			i,j;
    int			cleanBytes, widthBytes;
    unsigned char	lastbyteMask ;
    int			PaddedWidthInBits;
    mpelSrcBLTVPMWColorExpansion block;

    TRACE(("mpelDrawMonoByteImage(0x%x,%d,%d,%d,%d,%d,%d,0x%x)\n",
						data,x,y,w,h,fg,alu,planes));
    widthBytes = (w+7)/8;
    if ((widthBytes*h) > MPEL_BLIT_STAGE_SIZE )
	{
	    mpelUnnaturalMonoByteImage(data, x,y,w,h, fg, alu, planes );
	    return;
	}

    MPELWaitFifo();
    mpelCheckCursor(x,y,w,h);

    if (i = (w%8))
	{
	    lastbyteMask = ((~0) << (8 - i));
	    PaddedWidthInBits = (w & ~7) + 8;
	}
   else {
	    lastbyteMask = ~0;
	    PaddedWidthInBits = w;
	}

    mpelSetPlaneMask(planes);

    /* copy image TO adapter buffer area */
    srcPtr = (unsigned char *) data;

    dstPtr = (unsigned char *) MPEL_BLIT_STAGE;

    cleanBytes = widthBytes-1;
    for (j=0; j<h; j++) 
	{
	    if (cleanBytes)
		{
		    MOVE(srcPtr, dstPtr, cleanBytes);
		    srcPtr += cleanBytes;
		    dstPtr += cleanBytes;
		}

	    /* mask last byte */
	    *dstPtr++ = *srcPtr++ & lastbyteMask; 
	}

    block.color   		= fg;
    block.srcaddr     	= mpelAddr(MPEL_BLIT_STAGE);
    block.dest.lleft.x	= x;
    block.dest.lleft.y	= 1023 - (y + h - 1);
    block.dest.uright.x	= x + PaddedWidthInBits - 1;
    block.dest.uright.y	= 1023 - y;
    block.alu		= alu+1;

    MPELSrcBLTVPMWColorExpansionROP(&block);

    MPELWaitFifo();

    MPELResetPlaneMask();
    mpelRestoreCursor();
}
