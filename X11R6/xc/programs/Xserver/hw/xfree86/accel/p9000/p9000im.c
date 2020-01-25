/* $XConsortium: p9000im.c,v 1.5 95/01/05 20:38:51 kaleb Exp $ */
/* $XFree86: xc/programs/Xserver/hw/xfree86/accel/p9000/p9000im.c,v 3.4 1994/11/26 12:44:20 dawes Exp $ */
/*
 * Copyright 1992,1993 by Kevin E. Martin, Chapel Hill, North Carolina.
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of Kevin E. Martin not be used in
 * advertising or publicity pertaining to distribution of the software without
 * specific, written prior permission.  Kevin E. Martin makes no
 * representations about the suitability of this software for any purpose.
 * It is provided "as is" without express or implied warranty.
 *
 * CHRIS MASON, ERIK NYGREN, KEVIN E. MARTIN, RICKARD E. FAITH, TIAGO
 * GONS, AND CRAIG E. GROESCHEL DISCLAIM ALL WARRANTIES WITH REGARD TO
 * THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS, IN NO EVENT SHALL ANY OR ALL OF THE AUTHORS BE LIABLE
 * FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN
 * AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING
 * OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 *
 * Modified for the Mach-8 by Rickard E. Faith (faith@cs.unc.edu)
 * Modified for the Mach32 by Kevin E. Martin (martin@cs.unc.edu)
 * Rik Faith (faith@cs.unc.edu), Mon Jul  5 20:00:01 1993:
 *   Added 16-bit and some 64kb aperture optimizations.
 *   Waffled in stipple optimization by Jon Tombs (from s3/s3im.c)
 *   Added outsw code.
 * Modified for the P9000 by Chris Mason (mason@mail.csh.rit.edu)
 * */


#include "X.h"
#include "misc.h"
#include "xf86.h"
#include "xf86_HWlib.h"
#include "p9000.h"
#include "p9000reg.h"

#ifdef P9000_ACCEL

void (*p9000ImageReadFunc)(
#if NeedFunctionPrototypes
    int, int, int, int, char *, int, int, int, unsigned long
#endif
);

void (*p9000ImageWriteFunc)(
#if NeedFunctionPrototypes
    int, int, int, int, char *, int, int, int, short, unsigned long
#endif
);

static	void	p9000ImageRead(
#if NeedFunctionPrototypes
    int, int, int, int, char *, int, int, int, unsigned long 
#endif
);

static	void	p9000ImageWrite(
#if NeedFunctionPrototypes
    int, int, int, int, char *, int, int, int, short, unsigned long
#endif
);

static	void	p9000ImageReadNoMem(
#if NeedFunctionPrototypes
    int, int, int, int, char *, int, int, int, unsigned long
#endif
);
static	void	p9000ImageWriteNoMem(
#if NeedFunctionPrototypes
    int, int, int, int, char *, int, int, int, short, unsigned long
#endif
);


static unsigned long PMask;
static int screenStride;

void
p9000ImageInit()
{
    int i;

    PMask = (1UL << p9000InfoRec.depth) - 1;
    screenStride = p9000InfoRec.virtualX * p9000BytesPerPixel;

    p9000ImageReadFunc = p9000ImageRead;
    p9000ImageWriteFunc = p9000ImageWrite;
    /* p9000ImageFillFunc = p9000ImageFill; */
}

static void
#if NeedFunctionPrototypes
p9000ImageRead(
    int			x,
    int			y,
    int			w,
    int			h,
    char		*psrc,
    int			pwidth,
    int			px,
    int			py,
    unsigned long	planemask)
#else
p9000ImageRead(x, y, w, h, psrc, pwidth, px, py, planemask)
    int                 x;
    int                 y;
    int                 w;
    int                 h;
    unsigned char       *psrc;
    int                 pwidth;
    int                 px;
    int                 py;
    unsigned long	planemask;
#endif
{
    register int j;
    register unsigned char *curvm;

    if ((w == 0) || (h == 0))
        return;

    if ((planemask & PMask) != PMask) { 
        p9000ImageReadNoMem(x, y, w, h, psrc, pwidth, px, py, planemask);
        return;
    }

    psrc += pwidth * py + px * p9000BytesPerPixel;
    curvm = (unsigned char *)((int)VidBase + x * p9000BytesPerPixel);

    for (j = y; j < y+h; j++) {
        BusToMem(psrc, (void *)(curvm + j * screenStride), 
		 w * p9000BytesPerPixel);
      psrc += pwidth;
    }
}

static void
#if NeedFunctionPrototypes
p9000ImageReadNoMem(
    int			x,
    int			y,
    int			w,
    int			h,
    char		*psrc,
    int			pwidth,
    int			px,
    int			py,
    unsigned long	planemask)
#else
p9000ImageReadNoMem(x, y, w, h, psrc, pwidth, px, py, planemask)
    int                 x;
    int                 y;
    int                 w;
    int                 h;
    unsigned char       *psrc;
    int                 pwidth;
    int                 px;
    int                 py;
    unsigned long	planemask;
#endif
{
    register int i, j;
    int pix;
    short word1, word2; 

    w += px;
    psrc += pwidth * py;
    
    if (p9000BytesPerPixel == 1) {
        for (j = 0; j < h; j++) {
            for (i = px; i < w; i += 2) {
                pix = p9000Fetch((i + y * p9000InfoRec.virtualX),VidBase);
                word1 = (pix << 16);
                word1 = (pix >> 16);
                word2 = (pix >> 16);
                psrc[i] = word1;
                psrc[i+1] = word2;
            }
            psrc += pwidth;
        }

    }
}

static void
#if NeedFunctionPrototypes
p9000ImageWrite(
    int			x,
    int			y,
    int			w,
    int			h,
    char		*psrc,
    int			pwidth,
    int			px,
    int			py,
    short		alu,
    unsigned long	planemask)
#else
p9000ImageWrite(x, y, w, h, psrc, pwidth, px, py, alu, planemask)
    int			x;
    int			y;
    int			w;
    int			h;
    char		*psrc;
    int			pwidth;
    int			px;
    int			py;
    short		alu; /* pGC->alu */
    unsigned long	planemask;
#endif
{
    unsigned char *curvm;
    int byteCount;

    if ((w == 0) || (h == 0))
	return;

    if (p9000alu[alu] == IGM_D_MASK)
	return;

    if ((p9000alu[alu] != IGM_S_MASK) | ((planemask&PMask) != PMask)) {
	p9000ImageWriteNoMem(x, y, w, h, psrc, pwidth, px, py,
			      alu, planemask);
	return;
    }
       
    psrc += pwidth * py + px * p9000BytesPerPixel;
    curvm = (unsigned char *)((int)VidBase +
                            (x+y * p9000InfoRec.virtualX) * p9000BytesPerPixel);
    
    byteCount = w * p9000BytesPerPixel;
    while(h--) {
        MemToBus((void *)curvm, psrc, byteCount);
        curvm += screenStride; 
        psrc += pwidth;
    }
}

static void
#if NeedFunctionPrototypes
p9000ImageWriteNoMem(
    int			x,
    int			y,
    int			w,
    int			h,
    char		*psrc,
    int			pwidth,
    int			px,
    int			py,
    short		alu,
     unsigned long	planemask)
#else
p9000ImageWriteNoMem(x, y, w, h, psrc, pwidth, px, py, alu, planemask)
    int			x;
    int			y;
    int			w;
    int			h;
    char		*psrc;
    int			pwidth;
    int			px;
    int			py;
    short		alu; /* pGC->alu */
    unsigned long	planemask;
#endif
{
    register int i,j;
    psrc += pwidth * py;
    p9000NotBusy();
    p9000Store(RASTER,CtlBase,p9000alu[alu]);
    p9000Store(PMASK,CtlBase,planemask);


    p9000Store(DEVICE_COORD | DC_YX | DC_2,CtlBase,YX_PACK(x,y));
    p9000Store(DEVICE_COORD | DC_Y | DC_3,CtlBase,1);
    p9000Store(CMD_NEXT_PIXELS,CtlBase,w);
    
    /* ASSUMPTION: it is ok to read one byte past
       the psrc structure (for odd width). */

    for (j=0; j < h; j++) {
        for (i=0;i<(w);i+=4) {
          p9000Store(CMD_PIXEL8_SWAP,CtlBase,*((int *)(psrc + (px + i) * 
	             p9000BytesPerPixel)));
        }
        psrc += pwidth;
    }
    p9000NotBusy();

}

#endif /* P9000_ACCEL */

