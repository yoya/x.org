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

/* $Header:mpelimg.c 1.3$ */
/* $ACIS:mpelimg.c 1.3$ */
/* $Source: /ibm/acis/usr/src/X11/server/ddx/ibm/mpel/RCS/mpelimg.c,v $ */

#ifndef lint
static char *rcsid = "$Header:mpelimg.c 1.3$";
#endif

#include "X.h"
#include "Xproto.h"
#include "misc.h"
#include "scrnintstr.h"
#include "colormapst.h"
#include "pixmapstr.h"
#include "regionstr.h"
#include "windowstr.h"

#include "servermd.h"

#include "rtutils.h"

#include "ppc.h"

#include "mpelhdwr.h"
#include "mpelfifo.h"

#define MOVE _move

/***==================================================================***/

/*
 * mpelDrawColorImage(x,y,w,h,data,alu,planes)
 */

mpelDrawColorImage(x,y,w,h,data,alu,mask)
    int		x,y,w,h;
    CARD8	*data;
    int		alu,mask;
{
    CARD8	*ptr, *daptr;
    int		j;
    int		skip;
    int		nh, ny, maxrows, rowsleft, done;
    mpelSrcBLTVPM	blt;

    TRACE(("mpelDrawColorImage(%d,%d,%d,%d,0x%x,0x%x,0x%x)\n",
						x,y,w,h,data,alu,mask));

    /* copy image TO adapter buffer area */
    ptr = data;
    daptr = (unsigned char *) MPEL_BLIT_STAGE;
    skip = ((4 - (w&3)) & 3);

    maxrows = MPEL_BLIT_STAGE_SIZE / w;
    mpelCheckCursor(x,y,w,h);
    mpelSetPlaneMask(mask);

    if (maxrows >= h) {
	for (j=0; j<h; j++) {
	    MOVE(ptr, daptr, w);
	    daptr += w;
	    ptr += (w+skip);
	}	

	blt.srcaddr=		mpelAddr(MPEL_BLIT_STAGE);
	blt.dest.lleft.x=	x;
	blt.dest.lleft.y=	1023-(y+h-1);
	blt.dest.uright.x=	x+w-1;
	blt.dest.uright.y=	1023-y;
	blt.bpixel=		0x0008;
	blt.alu=		alu+1;
	MPELSrcBLTVPM(&blt);
	MPELNoOp(2,0);
	MPELWaitFifo();
    } 
    else {
	/* do multiple blits */
	ny = y; 
	nh = maxrows; 
	rowsleft = h; 
	done = FALSE;
	while(TRUE) {
	    for (j=0; j<nh; j++) {
	    	MOVE(ptr, daptr, w);
		daptr += w;
	    	ptr += (w+skip);
	    }	

	    blt.srcaddr=	mpelAddr(MPEL_BLIT_STAGE);
	    blt.dest.lleft.x=	x;
	    blt.dest.lleft.y=	1023-(ny+nh-1);
	    blt.dest.uright.x=	x+w-1;
	    blt.dest.uright.y=	1023-ny;
	    blt.bpixel=		0x0008;
	    blt.alu=		alu+1;
	    MPELSrcBLTVPM(&blt);
	    MPELNoOp(2,0);

	    if (done) {
		MPELWaitFifo();
		break;
	    }

	    /* update values */
	    daptr = (unsigned char *) MPEL_BLIT_STAGE;
	    ny += maxrows;
	    rowsleft -= nh;
	    if (rowsleft <= maxrows) {
		nh = rowsleft; 
		done = TRUE;
	    }
	    MPELWaitFifo();
	} /* end while */
    } /* end else */
    MPELResetPlaneMask();
    mpelRestoreCursor();
}

/*
 * mpelReadColorImage(x,y,lx,ly,data)
 * 
 * BUG: the first 32-bit word is broken !!!
 */

mpelReadColorImage(x,y,w,h,data)
    int		x,y,w,h;
    CARD8	*data;
{
    CARD8	*ptr, *daptr;
    int 	j;
    int 	skip;
    int 	nh, ny, maxrows, rowsleft;
    mpelVPMBLTDestination	blt;

    TRACE(("mpelReadColorImage(%d,%d,%d,%d,0x%x)\n",x,y,w,h,data));

    mpelCheckCursor(x,y,w,h);

    /* copy image TO adapter buffer area */
    daptr = data;
    ptr = (unsigned char *) MPEL_BLIT_STAGE;

    maxrows = MPEL_BLIT_STAGE_SIZE / w;
    skip = ((4 - (w&3)) & 3);

    if (maxrows >= h) {
	/* do a DestBlt VPM */
	blt.destaddr=		mpelAddr(MPEL_BLIT_STAGE);
	blt.source.lleft.x=	x;
	blt.source.lleft.y=	1023-(y+h-1);
	blt.source.uright.x=	x+w-1;
	blt.source.uright.y=	1023-y;
  	blt.comp=		 0x0001;	/* no color compression */
	MPELVPMBLTDest(&blt);
	MPELNoOp(2,0);
	MPELWaitFifo();
	for (j=0; j<h; j++) {
		MOVE(ptr, daptr, w);
		ptr += w;
		daptr += (w+skip);
	}	
    } 
    else {
	/* do multiple blits */
	ny = y;
	nh = maxrows;
	rowsleft = h;

	while(rowsleft>0) {
	    /* do a DestBlt VPM */
	    blt.destaddr=		mpelAddr(MPEL_BLIT_STAGE);
	    blt.source.lleft.x=		x;
	    blt.source.lleft.y=		1023-(ny+nh-1);
	    blt.source.uright.x=	x+w-1;
	    blt.source.uright.y=	1023-ny;
	    blt.comp=			0x0001;
	    MPELVPMBLTDest(&blt);
	    MPELNoOp(2,0);
	    MPELWaitFifo();
	    for (j=0; j<nh; j++) {
		MOVE(ptr, daptr, w);
		ptr += w;
		daptr += (w+skip);
	    }

	    /* update values */
	    ptr = (unsigned char *) MPEL_BLIT_STAGE;
	    ny += maxrows;
	    rowsleft -= nh;
	    if (rowsleft <= maxrows) {
		nh = rowsleft; 
	    }
	}
    } /* end ELSE */

    mpelRestoreCursor();
}

/* Was MIGETPLANE -- gets a bitmap representing one plane of pDraw
 * A helper used for CopyPlane and XY format GetImage 
 * No clever strategy here, we grab a scanline at a time, pull out the
 * bits and then stuff them in a 1 bit deep map.
 */
unsigned long	*
mpelGetPlane(planeNum, sx, sy, w, h, result)
    int			planeNum;	/* number of the bitPlane */
    int			sx, sy, w, h;
    unsigned long	*result;
{
    int			i, j, k;
    register unsigned char *pp;
    unsigned char 	*pline;
    unsigned int	bit;
#if  (BITMAP_SCANLINE_UNIT == 8)
    register unsigned char	tmpCharOut;
    unsigned char		*pCharsOut;
#endif
#if  (BITMAP_SCANLINE_UNIT == 16)
    register CARD16		tmpShortOut;
    CARD16			*pShortsOut;
#endif
#if  (BITMAP_SCANLINE_UNIT == 32)
    register CARD32		tmpLongOut;
    CARD32			*pLongsOut;
#endif

TRACE(("mpelGetPlane(%d, %d, %d, %d, %d, 0x%X)\n", planeNum, sx, sy, w, h, result));
	pline = (unsigned char *)ALLOCATE_LOCAL(w);

#if  (BITMAP_SCANLINE_UNIT == 8)
	pCharsOut = (unsigned char *) result;
#endif
#if  (BITMAP_SCANLINE_UNIT == 16)
	pShortsOut = (CARD16 *) result;
#endif
#if  (BITMAP_SCANLINE_UNIT == 32)
	pLongsOut = (CARD32 *) result;
#endif
    for(i = sy; i < sy + h; i++)
    {
	{
#if  (BITMAP_SCANLINE_UNIT ==  8)
	tmpCharOut = 0;
#endif
#if  (BITMAP_SCANLINE_UNIT ==  16)
	tmpShortOut = 0;
#endif
#if  (BITMAP_SCANLINE_UNIT ==  32)
	tmpLongOut = 0;
#endif
	    k = 0;
		/* Fetch the next line */
		mpelReadColorImage(sx,i,w,1,pline);
		for(j=0,pp=pline;j<w;j++,pp++){

		bit = (unsigned int) ((*pp >> planeNum) & 1);

		/* Now insert that bit into a bitmap in XY format */
#if (BITMAP_BIT_ORDER == LSBFIRST)
		    bit >> = k;
#else
		    bit <<= ((BITMAP_SCANLINE_UNIT - 1) - k);
#endif

#if (BITMAP_SCANLINE_UNIT == 8)
		{
		    tmpCharOut |= (unsigned char) bit;
		    k++;
		    if (k == 8)
		    {
		        *pCharsOut++ = tmpCharOut;
			tmpCharOut = 0;
		        k = 0;
		    }
		}
#endif
#if  (BITMAP_SCANLINE_UNIT == 16)
		{
		    tmpShortOut |= (CARD16) bit;
		    k++;
		    if (k == 16)
		    {
		        *pShortsOut++ = tmpShortout;
			tmpShortOut = 0;
		        k = 0;
		    }
		}
#endif
#if  (BITMAP_SCANLINE_UNIT == 32)
		{
		    tmpLongOut |= (CARD32) bit;
		    k++;
		    if (k == 32)
		    {
		        *pLongsOut++ = tmpLongOut;
			tmpLongOut = 0;
		        k = 0;
		    }
		}
#endif
		}
	    if(k!=0){
#if  (BITMAP_SCANLINE_UNIT == 32)
			  *pLongsOut++ = tmpLongOut;
#endif
#if  (BITMAP_SCANLINE_UNIT == 16)
			  *pShortsOut++ = tmpShortOut;
#endif
#if  (BITMAP_SCANLINE_UNIT == 8)
			  *pCharsOut++ = tmpCharOut;
#endif
	   }
	}
    }

    DEALLOCATE_LOCAL(pline);

    return(result);    
}
