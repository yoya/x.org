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

/* $Header:mpelstip.c 5.0$ */
/* $ACIS:mpelstip.c 5.0$ */
/* $Source: /vice/X11/src/server/ddx/ibm/mpel/RCS/mpelstip.c,v $ */

#ifndef lint
static char *rcsid = "$Header:mpelstip.c 5.0$";
#endif

#include "Xmd.h"
#include "misc.h"
#include "pixmapstr.h"

#include "rtutils.h"

#include "mpelhdwr.h"
#include "mpelfifo.h"

/***==================================================================***/

/*
 * mpelFillStipple(pStipple, fg, alu, planes,x,y,w,h,xSrc,ySrc)
 */

mpelFillStipple(pStipple,fg,alu,planes,x,y,w,h,xSrc,ySrc)
    PixmapPtr	pStipple;
    int		fg,alu,planes,x,y,w,h,xSrc,ySrc;
{
    int		sw, sh;
    unsigned char *bitmap;

    /* rotate image if needed */

    /* $$$$$$$$$$$$$$$$$$$$$$$$$$$
       $$$$$$$$$$$$$$$$$$$$$$$$$$$ */

    TRACE(("mpelFillStipple(0x%x,%d,%d,0x%x,%d,%d,%d,%d,0x%x,0x%x)\n",
				pStipple,fg,alu,planes,x,y,w,h,xSrc,ySrc));

    bitmap = pStipple->devPrivate;
    sw = pStipple->width;
    sh = pStipple->height;
    mpelCheckCursor(x,y,w,h);
    XStipple(bitmap, sw, sh, x, y, w, h, fg, alu, planes);
    mpelRestoreCursor();
}

/***==================================================================***/

static
XStipple(bitmap, stipWidth, stipHeight, x, y, w, h, fg, alu, mask)
    char	*bitmap;
    int		stipWidth, stipHeight, x, y, w, h, fg, alu, mask;
{
    CARD8	*srcPtr, *dstPtr,*tmpPtr;
    int		i,j;
    int		MainPadBytes, ExcessPadBytes;
    int		stipWidthInPaddedBytes, stipWidthInBytes, ExcessWidthInBytes;
    int		xDestination, yDestination;
    int		ExcessHeight, ExcessWidth, bltw, blth;
    int		widthleft, heightleft;
    int 	Mainmask, Excessmask;
    int		PaddedWidthInBits;
    mpelSrcBLTVPMWColorExpansion	blt;

    TRACE(("XStipple(0x%x,%d,%d,%d,%d,%d,%d,%d,%d,0x%x)\n",
			bitmap,stipWidth,stipHeight,x,y,w,h,fg,alu,mask));

    /* copy image to shared area */

    stipWidthInPaddedBytes = ((stipWidth+31)/32)<<2;
    stipWidthInBytes = (stipWidth+7)/8;

    /* check for bitmap > 64K bytes and printf-it */
    if ( ( stipWidthInPaddedBytes * stipHeight ) > ( MPEL_BLIT_STAGE_SIZE/2 ) )
	ErrorF("Stipple too big");

    MainPadBytes = stipWidthInPaddedBytes - stipWidthInBytes;

    ExcessWidth = w % stipWidth;		/* pad width to multiple of 8 */
    if (i=(ExcessWidth%8)) {
	Excessmask = 8 - i;
	ExcessWidth = (ExcessWidth & ~7) + 8;
    } 
    else
	Excessmask = 0;

    ExcessHeight = h % stipHeight;


    ExcessWidthInBytes = (ExcessWidth+7)/8;
    ExcessPadBytes = stipWidthInPaddedBytes - ExcessWidthInBytes;

    if (i = (stipWidth%8)) {
	Mainmask = 8 - i;
	PaddedWidthInBits = (stipWidth & ~7) + 8;
    }
    else {
	Mainmask = 0;
	PaddedWidthInBits = stipWidth;
    }

    mpelSetPlaneMask(mask);

    /* copy image TO adapter buffer area */
    srcPtr = (unsigned char *) bitmap;
    dstPtr = (unsigned char *) MPEL_BLIT_STAGE;

    for (j=0; j<stipHeight; j++) {
	for (i=0; i<stipWidthInBytes-1; i++) 
	    *dstPtr++ = *srcPtr++;

	/* mask last byte */
	if (Mainmask) 
	     *dstPtr = *srcPtr & ((~0) << Mainmask);
	else *dstPtr = *srcPtr;

	srcPtr++; dstPtr++;

	/* skip over padding */
	srcPtr += MainPadBytes;
    }

    /* copy image for rightmost (possibly clipped region */
    if (ExcessWidthInBytes) {
	srcPtr = (unsigned char *) bitmap;
	dstPtr = ((unsigned char *) MPEL_BLIT_STAGE)+MPEL_BLIT_STAGE_SIZE/2;

	for (j=0; j<stipHeight; j++) {
	    for (i=0; i<ExcessWidthInBytes-1; i++) 
		*dstPtr++ = *srcPtr++;

	    /* mask last byte */
	    if (Excessmask) 
		 *dstPtr = *srcPtr & ((~0) << Excessmask);
	    else *dstPtr = *srcPtr;
	}
	srcPtr++; dstPtr++;

	/* skip over padding */
	srcPtr += ExcessPadBytes;
    }

    /* start the blits */
    xDestination = x; yDestination = y;
    widthleft = w; heightleft = h;

    while (heightleft) {
	if (heightleft < stipHeight) {
	    blth = ExcessHeight;
	    heightleft = 0;
	}
	else  {
	    blth = stipHeight;
	    heightleft -= stipHeight;
	}

	while (widthleft) {
	    if (widthleft < stipWidth) {
		tmpPtr= ((CARD8 *)MPEL_BLIT_STAGE)+(MPEL_BLIT_STAGE_SIZE/2);
		bltw= ExcessWidth;
		/* blit use BANK3 */
		blt.srcaddr=		mpelAddr(tmpPtr);
		blt.dest.lleft.x=	xDestination;
		blt.dest.lleft.y=	1023-(yDestination+blth-1);
		blt.dest.uright.x=	xDestination+bltw-1;
		blt.dest.uright.y=	1023-yDestination;
		blt.color=		fg;
		blt.alu=		alu+1;
		MPELSrcBLTVPMWColorExpansion(&blt);
		/* xDestination = x; */
		widthleft = 0;
		break;
	    }
	    else {
		bltw= PaddedWidthInBits;
		blt.srcaddr=		mpelAddr(MPEL_BLIT_STAGE);
		blt.dest.lleft.x=	xDestination;
		blt.dest.lleft.y=	1023-(yDestination+blth-1);
		blt.dest.uright.x=	xDestination+bltw-1;
		blt.dest.uright.y=	1023-yDestination;
		blt.color=		fg;
		blt.alu=		alu+1;
		MPELSrcBLTVPMWColorExpansion(&blt);

		xDestination += stipWidth;
		widthleft -= stipWidth;
	    }
	}
	xDestination = x;
	yDestination += stipHeight;
	widthleft = w;
    }
    /* do a SourceBlt VPM with Color Expansion (p. 159) */

    MPELWaitFifo();

    MPELResetPlaneMask();
}

