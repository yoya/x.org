/***********************************************************
Copyright 1987, 1988 by Digital Equipment Corporation, Maynard, Massachusetts,
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

******************************************************************/

#include <errno.h>
#include <sys/types.h>

#include "X.h"
#include	"gcstruct.h"
#include "windowstr.h"

#include "Ultrix2.0inc.h"
#include "qduser.h"
#include "qdreg.h"
#include	"qd.h"
#include "tl.h"
#include "tlsg.h"
#include "tltemplabels.h"

extern int errno;
extern int Vaxstar;
extern	int	Nplanes;
extern	int	Nchannels;

/*
 *  tlputimage
 *
 *  put a rectangle of Z-mode data into the framebuffer.
 */
int
tlputimage( pwin, pgc, x, y, w, h, pcolor)
    WindowPtr		pwin;
    GCPtr		pgc;
    register int	x, y, w, h;
    register unsigned char *pcolor;
{
    register struct adder * adder = Adder;

    RegionPtr pSaveGCclip = ((QDPrivGCPtr)pgc->devPriv)->pCompositeClip;
    register BoxPtr pclip = pSaveGCclip->rects;	/* step through GC clip */
    register int nclip	  = pSaveGCclip->numRects;	/* number of clips */

    INVALID_SHADOW;	/* XXX */
    SETTRANSLATEPOINT( pwin->absCorner.x, pwin->absCorner.y);
    ptbsetup(x, y, w, h);
    write_ID( adder, LU_FUNCTION_R3, umtable[pgc->alu]);
    adder->rasterop_mode = (DST_WRITE_ENABLE | DST_INDEX_ENABLE | NORMAL);

    for ( ; nclip > 0; nclip--, pclip++)
    {
	adder->x_clip_min = pclip->x1;
	adder->y_clip_min = pclip->y1;
	adder->x_clip_max = pclip->x2;
	adder->y_clip_max = pclip->y2;

#if	NPLANES==24
	write_ID( adder, RED_UPDATE, RED(pgc->planemask));
	write_ID( adder, GREEN_UPDATE, 0x0);
	write_ID( adder, BLUE_UPDATE, 0x0);
	  ptbimage( x, y, w, h, pcolor+0, ZRED );
	write_ID( adder, RED_UPDATE, 0x0);
	write_ID( adder, GREEN_UPDATE, GREEN(pgc->planemask));
	write_ID( adder, BLUE_UPDATE, 0x0);
	  ptbimage( x, y, w, h, pcolor+1, ZGREEN);
	write_ID( adder, RED_UPDATE, 0x0);
	write_ID( adder, GREEN_UPDATE, 0x0);
	write_ID( adder, BLUE_UPDATE, BLUE(pgc->planemask));
	  ptbimage( x, y, w, h, pcolor+2, ZBLUE);
#else	/* NPLANES==8 */
	write_ID( adder, CS_UPDATE_MASK, GREEN(pgc->planemask));
	  /* this should be MUCH faster */
	  ptbzblock( x, y, w, h, pcolor, ZGREEN);
#endif
    }
    poll_status( adder, ADDRESS_COMPLETE );
    dmafxns.enable();
}

/* send all red/a-g/a-b formatted data to screen.  no clipping, no trans */
tlspadump( x, y, w, h, ppixel )
int		  x, y, w, h;
unsigned char	* ppixel;
{
    register struct adder *     adder = Adder;

    INVALID_SHADOW;	/* XXX */
    ptbsetup(x, y, w, h);
    write_ID(adder, LU_FUNCTION_R3, LF_S | FULL_SRC_RESOLUTION); /* source */
    adder->rasterop_mode = DST_WRITE_ENABLE|NORMAL;	/* no translation */
    /* no clipping */
    adder->x_clip_min = 0;
    adder->y_clip_min = 0;
    adder->x_clip_max = 1024;
    adder->y_clip_max = 2048;
#if	NPLANES==24
    write_ID(adder, RED_UPDATE, 0xff);
    write_ID(adder, GREEN_UPDATE, 0x00);
    write_ID(adder, BLUE_UPDATE, 0x00);
    ptbzblock( 0, y, w, h, ppixel, ZRED	);
    write_ID(adder, RED_UPDATE, 0x00);
    write_ID(adder, GREEN_UPDATE, 0xff);
    write_ID(adder, BLUE_UPDATE, 0x00);
    ptbzblock( 0, y, w, h, ppixel + w*h, ZGREEN	);
    write_ID(adder, RED_UPDATE, 0x00);
    write_ID(adder, GREEN_UPDATE, 0x00);
    write_ID(adder, BLUE_UPDATE, 0xff);
    ptbzblock( 0, y, w, h, ppixel + 2*w*h, ZBLUE );
#else	/* NPLANES==8 */
    write_ID(adder, CS_UPDATE_MASK, 0xff);
    ptbzblock( 0, y, w, h, ppixel, ZGREEN );
#endif
    poll_status( adder, ADDRESS_COMPLETE );
    dmafxns.enable();
}

/* XXX - is src_1 supposed to be translated? */

void
tlspaca( ppix, pwin, pgc, srcx, srcy, w, h, dstx, dsty )
PixmapPtr	ppix;
WindowPtr	pwin;
GCPtr		pgc;
int		srcx, srcy, w, h, dstx, dsty;
{
    register struct adder *     adder = Adder;

    RegionPtr pSaveGCclip = ((QDPrivGCPtr)pgc->devPriv)->pCompositeClip;
    register BoxPtr pclip = pSaveGCclip->rects;	/* step through GC clip */
    register int nclip	  = pSaveGCclip->numRects;	/* number of clips */
    int		igreen = ppix->width * ppix->height;
    register int	iy;	/* index of current inc past dsty */
    int		pixoffy;	/* y address of offscreen pixmap */
    /* data points to first byte (in red block) */

    INVALID_SHADOW;	/* XXX */

    /* put pixmap offscreen and do bitblt 
       remember to nuke the copy if the original gets drawn to
    */
    if (ppix->width <= 1024 && ppix->height <= DragonPix)
    {
	if (! tlConfirmPixmap( ppix->devPrivate, ppix, &pixoffy ))
	    FatalError( "tlspaca: could not store pixmap off-screen\n");
	tlbitblt( pgc, dstx+pwin->absCorner.x, dsty+pwin->absCorner.y,
		  w, h, srcx, srcy+pixoffy);
	return;
    }

    SETTRANSLATEPOINT( pwin->absCorner.x, pwin->absCorner.y);

    dmafxns.flush( TRUE );
    /* hope that we can do it all in one block...*/
    if (srcx == 0 && w == ppix->width)
        ptbsetup(dstx, dsty, w, h);

    write_ID( adder, LU_FUNCTION_R3, umtable[pgc->alu]);
    adder->rasterop_mode = (DST_WRITE_ENABLE | DST_INDEX_ENABLE | NORMAL);

    for ( ; nclip > 0; nclip--, pclip++)
    {
	register unsigned char * data = ((QDPixPtr) ppix->devPrivate)->data
					+ srcy * ppix->width + srcx;

	adder->x_clip_min = pclip->x1;
	adder->y_clip_min = pclip->y1;
	adder->x_clip_max = pclip->x2;
	adder->y_clip_max = pclip->y2;

	if (srcx == 0 && w == ppix->width)
	{	/* can do continuous height block */
#if	NPLANES==24
	    write_ID( adder, RED_UPDATE, RED(pgc->planemask));
	    write_ID( adder, GREEN_UPDATE, 0x0);
	    write_ID( adder, BLUE_UPDATE, 0x0);
	      ptbzblock( dstx, dsty, w, h, data, ZRED );
	    write_ID( adder, RED_UPDATE, 0x0);
	    write_ID( adder, GREEN_UPDATE, GREEN(pgc->planemask));
	    write_ID( adder, BLUE_UPDATE, 0x0);
	      ptbzblock( dstx, dsty, w, h, data + igreen, ZGREEN);
	    write_ID( adder, RED_UPDATE, 0x0);
	    write_ID( adder, GREEN_UPDATE, 0x0);
	    write_ID( adder, BLUE_UPDATE, BLUE(pgc->planemask));
	      ptbzblock( dstx, dsty, w, h, data + 2*igreen, ZBLUE);
#else	/* NPLANES==8 */
	    write_ID( adder, CS_UPDATE_MASK, GREEN(pgc->planemask));
	      ptbzblock( dstx, dsty, w, h, data, ZGREEN);
#endif
	} else
	{	/* must do individual lines */
	    for (iy = 0; iy < h; iy++)
	    {
		/* re-setup adder */
		ptbsetup(dstx, dsty+iy, w, 1);
#if	NPLANES==24
		write_ID( adder, RED_UPDATE, RED(pgc->planemask));
		write_ID( adder, GREEN_UPDATE, 0x0);
		write_ID( adder, BLUE_UPDATE, 0x0);
		  ptbzblock( dstx, dsty + iy, w, 1, data, ZRED );
		write_ID( adder, RED_UPDATE, 0x0);
		write_ID( adder, GREEN_UPDATE, GREEN(pgc->planemask));
		write_ID( adder, BLUE_UPDATE, 0x0);
		  ptbzblock( dstx, dsty + iy, w, 1, data + igreen, ZGREEN);
		write_ID( adder, RED_UPDATE, 0x0);
		write_ID( adder, GREEN_UPDATE, 0x0);
		write_ID( adder, BLUE_UPDATE, BLUE(pgc->planemask));
		  ptbzblock( dstx, dsty + iy, w, 1, data + 2*igreen, ZBLUE);
#else	/* NPLANES==8 */
		write_ID( adder, CS_UPDATE_MASK, GREEN(pgc->planemask));
		  ptbzblock( dstx, dsty + iy, w, 1,
			data, ZGREEN);
#endif
		data += ppix->width;
	    }
	}
    }
    poll_status( adder, ADDRESS_COMPLETE );
    dmafxns.enable();
}

/* this sets up everything for a PTB transfer, EXCEPT:	*
 *	alu, rasterop_mode, planemask, and clipping.	*/
static
ptbsetup(x, y, w, h)
int		x, y, w, h;
{
    register struct adder	*adder = Adder;
    register unsigned short	*p;

    /*
     * system hangs in following WAITDMADONE, 
     * before first xterm comes up, if these three lines are
     * omitted: WHY?            XX
     */
    Need_dma(1);
    *p++ = TEMPLATE_DONE;
    Confirm_dma ();

    dmafxns.flush ( TRUE);      /* spin until all dragon FIFOs are empty */

    adder->source_1_dx = 1;
    adder->source_1_dy = 1;
    adder->destination_x = x;
    adder->destination_y = y;
    adder->fast_dest_dx = w;
    adder->fast_dest_dy = 0;
    adder->slow_dest_dx = 0;
    adder->slow_dest_dy = h;

    /* Set ALL viper registers.
     * Set up viper registers for PTBZ transfer.
     * Actual data flow is observed to be very different from that
     * described in manual:  bytes from the DMA chip go into the viper SOURCE
     * register rather than FOREGROUND or BACKGROUND.
     */
#if	NPLANES==24
    write_ID(adder, RED_UPDATE, 0xff);
    write_ID(adder, GREEN_UPDATE, 0xff);
    write_ID(adder, BLUE_UPDATE, 0xff);
#else	/* NPLANES==8 */
    write_ID(adder, CS_UPDATE_MASK, 0xff);
#endif
    write_ID( adder, FOREGROUND_COLOR, 0xffff );
    write_ID( adder, BACKGROUND_COLOR, 0x0000 );
    write_ID( adder, MASK_1, 0xffff );    /* necessary??? */
}

/* This data is assumed to be contiguous.  no stepping.		*
 *  use direct mapped i/o with the driver's services for	*
 *  dma from user space.					*
 *  could use template for things less than 8K			*
 * Who cares?  This is fast enough.				*
 */
static
ptbzblock( x, y, w, h, ppixel, zblock)
    int         x,y,w,h;
    unsigned char *ppixel;
    int         zblock;         /* code indicating which block of planes */
{
    register struct adder *     adder = Adder;
    register struct dga *       dga = Dga;

    /* make sure that (request_enable & status) is non-zero */
    adder->request_enable = TX_READY;
    adder->command = PTBZ | zblock;
    dga->csr = GLOBAL_IE | DMA_IE | CURS_ENB | PTOB_ENB | BYTE_DMA;

    if ( write( fd_qdss, ppixel, w*h) == -1)
	FatalError("tlSetPixelArray: DMA write failed, errno=%d\n",errno);

    WAITBYTCNTZERO(dga);
    WAITDMADONE(dga);       /* mode bit 1 is NOT TO BE TOUCHED  until
                               fifo is empty */
    poll_status( adder, ADDRESS_COMPLETE); 
    dmafxns.enable ();  /* Re-enable DMA */
}

/* put RGB data into the framebuffer.	stepping by NPLANES / 8	*
 *	This is EXTREMELY slow.  Sorry.				*
 */
static
ptbimage( x, y, w, h, pcolor, zblock)
    register int	x, y, w, h;
    register unsigned char *	pcolor;
    register int	zblock;
{
    register int		width;
    register struct adder * adder = Adder;

    poll_status( adder, ADDRESS_COMPLETE); 
    adder->command = (PTBZ | zblock);
    while (h--)
    {
	for (width = w; width > 0; width--)
	{
	    poll_status( adder, TX_READY);
	    adder->id_data = *pcolor;
	    pcolor += NPLANES/8;
	}
    }
}

#ifdef	undef
/*
 * THIS IS BROKEN, BUT IT IS NOT USED   XX
 *
 * Transfer a rectangular full-depth image to the frame buffer.
 * Will not work if the rectangle is more than 8K pixels.
 *
 *  the dma buffer has
 *  JMPT_SETVIPER24
 *  rop | FULL_SRC_RESOLUTION
 *  JMPT_INITPTOBZ
 *  x, y, width, height
 *  PTBZ | count
 *  pixvals
 *  DONE
 */
ntlSetPixelArray( zblock, x, y, width, height, ppix)
    int         zblock;         /* code indicating which block of planes */
    int         x,y;
    register int width;
    register int height;
    register unsigned char *ppix;
{
    register unsigned short *p;
    register int npix;

    INVALID_SHADOW;	/* XXX */
    SETTRANSLATEPOINT(0, 0);

    npix = width * height;


    Need_dma( 7);
    *p++ = JMPT_SETRGBPLANEMASK;
    *p++ = (zblock & ZRED) ? 0xff : 0;
    *p++ = (zblock & ZGREEN) ? 0xff : 0;
    *p++ = (zblock & ZBLUE) ? 0xff : 0;
    *p++ = JMPT_SETVIPER24;     /* sets foreground and background registers */
    *p++ = umtable[ GXcopy] | FULL_SRC_RESOLUTION;
    *p++ = JMPT_RESETCLIP;
    Confirm_dma ();

    Need_dma( 6 /*7*/ + npix + 1);
    *p++ = JMPT_INITZBLOCKPTOB;
    *p++ = x & 0x3fff;
    *p++ = y & 0x3fff;
    *p++ = width & 0x3fff;
    *p++ = height & 0x3fff;

    /* DGA magic bit pattern for PTB */
    *p++ = 0x6000 |(0x1fff & -npix);
    while (npix--)              /* unpack pixels into shorts */
        *p++ = *ppix++;

    *p++ = TEMPLATE_DONE;
    Confirm_dma ();
}
/* following is conditional code for the vaxstar.   oh well */
    }else{
        /* Do pbtz by programmed i/o for now; must rewrite this using
		dma/fifo later */

	register int i;
	register unsigned char * pbuf = ppixel;

	adder->request_enable = 0;
	adder->command = PTBZ | zblock;

	for (i = w*h; --i >= 0; ) {
	    while ( ! (adder->status & TX_READY) )
		;
	    adder->id_data = *pbuf++;
	}
    }

#endif
