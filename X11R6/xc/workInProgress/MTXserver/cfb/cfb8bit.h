/*
 * cfb8bit.h
 *
 * Defines which are only useful to 8 bit color frame buffers
 *
 * That doesn't seem to be true any more.  Some of the macros in here 
 * are used for depths other than 8.  Perhaps the file should be
 * renamed.  dpw
 */

/*

Copyright (c) 1989  X Consortium

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
X CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of the X Consortium shall not be
used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization from the X Consortium.
*/

/* $XConsortium: cfb8bit.h,v 1.3 94/04/17 21:16:18 rob Exp $ */

#include "servermd.h"

#if (BITMAP_BIT_ORDER == MSBFirst)
#define GetBitGroup(x)		(((PixelGroup) (x)) >> (PGSZ - PGSZB))
#define NextBitGroup(x)		((x) <<= PGSZB)
#define NextSomeBits(x,n)	((x) <<= (n))
#else
#define GetBitGroup(x)		((x) & PGSZBMSK)
#define NextBitGroup(x)		((x) >>= PGSZB)
#define NextSomeBits(x,n)	((x) >>= (n))
#endif

#define RotBitsLeft(x,k)    ((x) = BitLeft (x,k) | \
				   BitRight (x, PGSZ-(k)))

#if defined(__GNUC__) && defined(mc68020)
#undef RotBitsLeft
#define RotBitsLeft(x,k)	asm("rol%.l %2,%0" \
				: "=d" (x) \
 				: "0" (x), "dI" (k))
#endif

#ifndef XTHREADS
#define MTX_STIPPLE(_a) _a
#define MTX_STIPPLE_CHANGE(_a) /* nothing */
#else /* XTHREADS */
#define MTX_STIPPLE(_a) pstipple->_a
#define MTX_STIPPLE_CHANGE(_a) pstipple->change = (_a)
#endif /* XTHREADS */


#if PSZ == 8

#define GetPixelGroup(x)						\
    (MTX_STIPPLE(cfb8StippleXor[GetBitGroup(x)]))
#define RRopPixels(dst,x)						\
    (DoRRop(dst,MTX_STIPPLE(cfb8StippleAnd[x]), 			\
            MTX_STIPPLE(cfb8StippleXor[x])))
#define MaskRRopPixels(dst,x,mask)					\
    (DoMaskRRop(dst,MTX_STIPPLE(cfb8StippleAnd[x]),			\
                MTX_STIPPLE(cfb8StippleXor[x]), mask))


#define RRopPixelGroup(dst,x)	(RRopPixels(dst,GetBitGroup(x)))

#define NUM_MASKS	(1<<PPW) /* XXX goes in cfbmskbits.h? */

/*
 * Outch!  None of this stuff exists in the MTX universe.
 */
#ifndef XTHREADS 
extern int		cfb8StippleMode, cfb8StippleAlu, cfb8StippleRRop;
extern PixelGroup	cfb8StippleFg, cfb8StippleBg, cfb8StipplePm;
extern PixelGroup	cfb8StippleAnd[NUM_MASKS], cfb8StippleXor[NUM_MASKS];
#endif /* !XTHREADS */

extern PixelGroup	cfb8StippleMasks[NUM_MASKS];

#define cfb8PixelMasks	cfb8StippleMasks
#define cfb8Pixels	cfb8StippleXor

#ifdef XTHREADS
#define UpdateStipple   \
    pstipple = (cfbGetGCPrivate(pGC))->stipple; \
    if(pstipple->change == TRUE) \
    { \
        if (pGC->fillStyle == FillStippled) \
        { \
            if((pGC->alu != pstipple->cfb8StippleAlu)             || \
               (((pGC->fgPixel) & PMSK) != pstipple->cfb8StippleFg)   || \
               (((pGC->planemask) & PMSK) != pstipple->cfb8StipplePm) || \
               (FillStippled != pstipple->cfb8StippleMode)) \
            { \
                cfb8SetStipple( pGC->alu, pGC->fgPixel, pGC->planemask,  \
                                pstipple); \
            } \
        } else { \
            if((pGC->alu != pstipple->cfb8StippleAlu)              || \
               (((pGC->fgPixel) & PMSK) != pstipple->cfb8StippleFg )   || \
               (((pGC->bgPixel) & PMSK) != pstipple->cfb8StippleBg )   || \
               (((pGC->planemask) & PMSK) != pstipple->cfb8StipplePm ) || \
               (FillOpaqueStippled != pstipple->cfb8StippleMode )) \
            { \
                cfb8SetOpaqueStipple( pGC->alu, pGC->fgPixel, pGC->bgPixel, \
                                      pGC->planemask, pstipple );\
            }\
        } \
        pstipple->change = FALSE; \
    }
#endif /* XTHREADS */

#define cfb8CheckPixels(fg, bg) \
    (FillOpaqueStippled == MTX_STIPPLE(cfb8StippleMode) && \
     GXcopy == MTX_STIPPLE(cfb8StippleAlu) && \
     ((fg) & PMSK) == MTX_STIPPLE(cfb8StippleFg) && \
     ((bg) & PMSK) == MTX_STIPPLE(cfb8StippleBg) && \
     PMSK == MTX_STIPPLE(cfb8StipplePm))

#ifndef XTHREADS
#define cfb8CheckOpaqueStipple(alu,fg,bg,pm) \
    ((FillOpaqueStippled == MTX_STIPPLE(cfb8StippleMode) && \
      (alu) == MTX_STIPPLE(cfb8StippleAlu) && \
      ((fg) & PMSK) == MTX_STIPPLE(cfb8StippleFg) && \
      ((bg) & PMSK) == MTX_STIPPLE(cfb8StippleBg) && \
      ((pm) & PMSK) == MTX_STIPPLE(cfb8StipplePm)) ? 0 : cfb8SetOpaqueStipple(alu,fg,bg,pm))
#else
#define cfb8CheckOpaqueStipple(alu,fg,bg,pm,pstipple) \
    ((FillOpaqueStippled == MTX_STIPPLE(cfb8StippleMode) && \
      (alu) == MTX_STIPPLE(cfb8StippleAlu) && \
      ((fg) & PMSK) == MTX_STIPPLE(cfb8StippleFg) && \
      ((bg) & PMSK) == MTX_STIPPLE(cfb8StippleBg) && \
      ((pm) & PMSK) == MTX_STIPPLE(cfb8StipplePm)) ? 0 : cfb8SetOpaqueStipple(alu,fg,bg,pm,pstipple))
#endif /* !XTHREADS */

#ifndef XTHREADS
#define cfb8CheckStipple(alu,fg,pm) \
    ((FillStippled == MTX_STIPPLE(cfb8StippleMode) && \
      (alu) == MTX_STIPPLE(cfb8StippleAlu) && \
      ((fg) & PMSK) == MTX_STIPPLE(cfb8StippleFg) && \
      ((pm) & PMSK) == MTX_STIPPLE(cfb8StipplePm)) ? 0 : cfb8SetStipple(alu,fg,pm))
#else
#define cfb8CheckStipple(alu,fg,pm,pstipple) \
    ((FillStippled == MTX_STIPPLE(cfb8StippleMode) && \
      (alu) == MTX_STIPPLE(cfb8StippleAlu) && \
      ((fg) & PMSK) == MTX_STIPPLE(cfb8StippleFg) && \
      ((pm) & PMSK) == MTX_STIPPLE(cfb8StipplePm)) ? 0 : cfb8SetStipple(alu,fg,pm,pstipple))
#endif /* !XTHREADS */

#ifndef XTHREADS
#define cfb8SetPixels(fg,bg) cfb8SetOpaqueStipple(GXcopy,fg,bg,PMSK)
#else
#define cfb8SetPixels(fg,bg,pstipple) cfb8SetOpaqueStipple(GXcopy,fg,bg,PMSK,pstipple)
#endif /* !XTHREADS */

/*
 * These macros are shared between the unnatural spans code
 * and the unnatural rectangle code.  No reasonable person
 * would attempt to use them anyplace else.
 */

#define NextUnnaturalStippleWord \
	if (bitsLeft >= MFB_PPW) \
	{ \
	    inputBits = *srcTemp++; \
	    bitsLeft -= MFB_PPW; \
	    partBitsLeft = MFB_PPW; \
	} \
	else \
	{ \
	    inputBits = 0; \
	    if (bitsLeft) \
		inputBits = *srcTemp & ~cfb8BitLenMasks[bitsLeft]; \
	    srcTemp = srcStart; \
	    partBitsLeft = bitsLeft; \
	    bitsLeft = bitsWhole; \
	}

#define NextUnnaturalStippleBits \
    if (partBitsLeft >= PPW) { \
	bits = GetBitGroup (inputBits); \
	NextBitGroup (inputBits); \
	partBitsLeft -= PPW; \
    } else { \
	bits = GetBitGroup (inputBits); \
	nextPartBits = PPW - partBitsLeft; \
	NextUnnaturalStippleWord \
	if (partBitsLeft < nextPartBits) { \
	    if (partBitsLeft) {\
	    	bits |= BitRight (GetBitGroup (inputBits), \
				  PPW - nextPartBits) & PPWMSK;\
	    	nextPartBits -= partBitsLeft; \
	    } \
	    NextUnnaturalStippleWord \
	} \
	bits |= BitRight (GetBitGroup (inputBits), \
			  PPW - nextPartBits) & PPWMSK; \
	NextSomeBits (inputBits, nextPartBits); \
	partBitsLeft -= nextPartBits; \
    }

#define NextUnnaturalStippleBitsFast \
    if (partBitsLeft >= PPW) { \
	bits = GetBitGroup(inputBits); \
	NextBitGroup(inputBits); \
	partBitsLeft -= PPW; \
    } else { \
	bits = GetBitGroup (inputBits); \
	nextPartBits = PPW - partBitsLeft; \
	inputBits = *srcTemp++; \
	bits |= BitRight (GetBitGroup (inputBits), \
		          partBitsLeft) & PPWMSK; \
	NextSomeBits (inputBits, nextPartBits); \
	partBitsLeft =  MFB_PPW - nextPartBits; \
    }

/*
 * WriteBitGroup takes the destination address, a pixel
 * value (which must be 8 bits duplicated 4 time with PFILL)
 * and the PPW bits to write, which must be in the low order
 * bits of the register (probably from GetBitGroup) and writes
 * the appropriate locations in memory with the pixel value.  This
 * is a copy-mode only operation.
 */

#define RRopBitGroup(dst,bits)					\
    {								\
    register PixelGroup    _bitsTmp = (bits);			\
    *(dst) = RRopPixels(*(dst),bits);				\
    }

#define MaskRRopBitGroup(dst,bits,mask)				\
    {								\
    register PixelGroup   _bitsTmp = (bits);			\
    *(dst) = MaskRRopPixels(*(dst),bits,mask);			\
    }
#endif /* PSZ == 8 */

#if !defined(AVOID_MEMORY_READ) && PSZ == 8

#define WriteBitGroup(dst,pixel,bits)				\
    {								\
    register PixelGroup _maskTmp = cfb8PixelMasks[(bits)];   \
    *(dst) = (*(dst) & ~_maskTmp) | ((pixel) & _maskTmp);	\
    }

#define SwitchBitGroup(dst,pixel,bits)				\
    {								\
    register PixelGroup _maskTmp = cfb8PixelMasks[(bits)];   \
    register PixelGroup _pixTmp = ((pixel) & _maskTmp);	\
    _maskTmp = ~_maskTmp;					\
    SwitchBitsLoop (*(dst) = (*(dst) & _maskTmp) | _pixTmp;)	\
    }
    
#else /* AVOID_MEMORY_READ */

#if PGSZ == 32
#if (BITMAP_BIT_ORDER == MSBFirst)
#define SinglePixel0	3
#define SinglePixel1	2
#define SinglePixel2	1
#define SinglePixel3	0
#define DoublePixel0	1
#define DoublePixel1	0
#else
#define SinglePixel0	0
#define SinglePixel1	1
#define SinglePixel2	2
#define SinglePixel3	3
#define DoublePixel0	0
#define DoublePixel1	1
#endif
#define QuadPixel0	0
#else /* PGSZ == 64 */
#if (BITMAP_BIT_ORDER == MSBFirst)
#define SinglePixel0	7
#define SinglePixel1	6
#define SinglePixel2	5
#define SinglePixel3	4
#define SinglePixel4	3
#define SinglePixel5	2
#define SinglePixel6	1
#define SinglePixel7	0
#define DoublePixel0	3
#define DoublePixel1	2
#define DoublePixel2	1
#define DoublePixel3	0
#define QuadPixel0	1
#define QuadPixel1	0
#else
#define SinglePixel0	0
#define SinglePixel1	1
#define SinglePixel2	2
#define SinglePixel3	3
#define SinglePixel4	4
#define SinglePixel5	5
#define SinglePixel6	6
#define SinglePixel7	7
#define DoublePixel0	0
#define DoublePixel1	1
#define DoublePixel2	2
#define DoublePixel3	3
#define QuadPixel0	0
#define QuadPixel1	1
#endif
#define OctaPixel0	0
#endif /* PGSZ == 64 */

#if PSZ == 8

#if PGSZ == 32
#define WriteBitGroup(dst,pixel,bits) \
	switch (bits) {			\
	case 0:				\
	    break;			\
	case 1:				\
	    ((CARD8 *) (dst))[SinglePixel0] = (pixel);	\
	    break;			\
	case 2:				\
	    ((CARD8 *) (dst))[SinglePixel1] = (pixel);	\
	    break;			\
	case 3:				\
	    ((CARD16 *) (dst))[DoublePixel0] = (pixel);	\
	    break;			\
	case 4:				\
	    ((CARD8 *) (dst))[SinglePixel2] = (pixel);	\
	    break;			\
	case 5:				\
	    ((CARD8 *) (dst))[SinglePixel0] = (pixel);	\
	    ((CARD8 *) (dst))[SinglePixel2] = (pixel);	\
	    break;			\
	case 6:				\
	    ((CARD8 *) (dst))[SinglePixel1] = (pixel);	\
	    ((CARD8 *) (dst))[SinglePixel2] = (pixel);	\
	    break;			\
	case 7:				\
	    ((CARD16 *) (dst))[DoublePixel0] = (pixel);	\
	    ((CARD8 *) (dst))[SinglePixel2] = (pixel);	\
	    break;			\
	case 8:				\
	    ((CARD8 *) (dst))[SinglePixel3] = (pixel);	\
	    break;			\
	case 9:				\
	    ((CARD8 *) (dst))[SinglePixel0] = (pixel);	\
	    ((CARD8 *) (dst))[SinglePixel3] = (pixel);	\
	    break;			\
	case 10:			\
	    ((CARD8 *) (dst))[SinglePixel1] = (pixel);	\
	    ((CARD8 *) (dst))[SinglePixel3] = (pixel);	\
	    break;			\
	case 11:			\
	    ((CARD16 *) (dst))[DoublePixel0] = (pixel);	\
	    ((CARD8 *) (dst))[SinglePixel3] = (pixel);	\
	    break;			\
	case 12:			\
	    ((CARD16 *) (dst))[DoublePixel1] = (pixel);	\
	    break;			\
	case 13:			\
	    ((CARD8 *) (dst))[SinglePixel0] = (pixel);	\
	    ((CARD16 *) (dst))[DoublePixel1] = (pixel);	\
	    break;			\
	case 14:			\
	    ((CARD8 *) (dst))[SinglePixel1] = (pixel);	\
	    ((CARD16 *) (dst))[DoublePixel1] = (pixel);	\
	    break;			\
	case 15:			\
	    ((CARD32 *) (dst))[0] = (pixel);	\
	    break;			\
	}
#else /* PGSZ == 64 */
#define WriteBitGroup(dst,pixel,bits) 				\
    if ( bits == 0xff )						\
	((PixelGroup *) (dst))[OctaPixel0] = (pixel);		\
    else {							\
	switch (bits & 0x0f) {					\
	    case 0:						\
	        break;						\
	    case 1:						\
	        ((CARD8 *) (dst))[SinglePixel0] = (pixel);	\
	        break;						\
	    case 2:						\
	        ((CARD8 *) (dst))[SinglePixel1] = (pixel);	\
	        break;						\
	    case 3:						\
	        ((CARD16 *) (dst))[DoublePixel0] = (pixel);	\
	        break;						\
	    case 4:						\
	        ((CARD8 *) (dst))[SinglePixel2] = (pixel);	\
	        break;						\
	    case 5:						\
	        ((CARD8 *) (dst))[SinglePixel0] = (pixel);	\
	        ((CARD8 *) (dst))[SinglePixel2] = (pixel);	\
	        break;						\
	    case 6:						\
	        ((CARD8 *) (dst))[SinglePixel1] = (pixel);	\
	        ((CARD8 *) (dst))[SinglePixel2] = (pixel);	\
	        break;						\
	    case 7:						\
	        ((CARD16 *) (dst))[DoublePixel0] = (pixel);	\
	        ((CARD8 *) (dst))[SinglePixel2] = (pixel);	\
	        break;						\
	    case 8:						\
	        ((CARD8 *) (dst))[SinglePixel3] = (pixel);	\
	        break;						\
	    case 9:						\
	        ((CARD8 *) (dst))[SinglePixel0] = (pixel);	\
	        ((CARD8 *) (dst))[SinglePixel3] = (pixel);	\
	        break;						\
	    case 10:						\
	        ((CARD8 *) (dst))[SinglePixel1] = (pixel);	\
	        ((CARD8 *) (dst))[SinglePixel3] = (pixel);	\
	        break;						\
	    case 11:						\
	        ((CARD16 *) (dst))[DoublePixel0] = (pixel);	\
	        ((CARD8 *) (dst))[SinglePixel3] = (pixel);	\
	        break;						\
	    case 12:						\
	        ((CARD16 *) (dst))[DoublePixel1] = (pixel);	\
	        break;						\
	    case 13:						\
	        ((CARD8 *) (dst))[SinglePixel0] = (pixel);	\
	        ((CARD16 *) (dst))[DoublePixel1] = (pixel);	\
	        break;						\
	    case 14:						\
	        ((CARD8 *) (dst))[SinglePixel1] = (pixel);	\
	        ((CARD16 *) (dst))[DoublePixel1] = (pixel);	\
	        break;						\
	    case 15:						\
	        ((CARD32 *) (dst))[QuadPixel0] = (pixel);	\
	        break;						\
	}							\
	switch ((bits & 0xf0) >> 4) {				\
	    case 0:						\
	        break;						\
	    case 1:						\
	        ((CARD8 *) (dst))[SinglePixel4] = (pixel);	\
	        break;						\
	    case 2:						\
	        ((CARD8 *) (dst))[SinglePixel5] = (pixel);	\
	        break;						\
	    case 3:						\
	        ((CARD16 *) (dst))[DoublePixel2] = (pixel);	\
	        break;						\
	    case 4:						\
	        ((CARD8 *) (dst))[SinglePixel6] = (pixel);	\
	        break;						\
	    case 5:						\
	        ((CARD8 *) (dst))[SinglePixel4] = (pixel);	\
	        ((CARD8 *) (dst))[SinglePixel6] = (pixel);	\
	        break;						\
	    case 6:						\
	        ((CARD8 *) (dst))[SinglePixel5] = (pixel);	\
	        ((CARD8 *) (dst))[SinglePixel7] = (pixel);	\
	        break;						\
	    case 7:						\
	        ((CARD16 *) (dst))[DoublePixel2] = (pixel);	\
	        ((CARD8 *) (dst))[SinglePixel6] = (pixel);	\
	        break;						\
	    case 8:						\
	        ((CARD8 *) (dst))[SinglePixel7] = (pixel);	\
	        break;						\
	    case 9:						\
	        ((CARD8 *) (dst))[SinglePixel4] = (pixel);	\
	        ((CARD8 *) (dst))[SinglePixel7] = (pixel);	\
	        break;						\
	    case 10:						\
	        ((CARD8 *) (dst))[SinglePixel5] = (pixel);	\
	        ((CARD8 *) (dst))[SinglePixel7] = (pixel);	\
	        break;						\
	    case 11:						\
	        ((CARD16 *) (dst))[DoublePixel2] = (pixel);	\
	        ((CARD8 *) (dst))[SinglePixel7] = (pixel);	\
	        break;						\
	    case 12:						\
	        ((CARD16 *) (dst))[DoublePixel3] = (pixel);	\
	        break;						\
	    case 13:						\
	        ((CARD8 *) (dst))[SinglePixel4] = (pixel);	\
	        ((CARD16 *) (dst))[DoublePixel3] = (pixel);	\
	        break;						\
	    case 14:						\
	        ((CARD8 *) (dst))[SinglePixel5] = (pixel);	\
	        ((CARD16 *) (dst))[DoublePixel3] = (pixel);	\
	        break;						\
	    case 15:						\
	        ((CARD32 *) (dst))[QuadPixel1] = (pixel);	\
	        break;						\
	}							\
    }
#endif /* PGSZ == 64 */

#if PGSZ == 32
#define SwitchBitGroup(dst,pixel,bits) { \
	switch (bits) { \
	case 0: \
       	    break; \
	case 1: \
	    SwitchBitsLoop (((CARD8 *) (dst))[SinglePixel0] = (pixel);) \
	    break; \
	case 2: \
	    SwitchBitsLoop (((CARD8 *) (dst))[SinglePixel1] = (pixel);) \
	    break; \
	case 3: \
	    SwitchBitsLoop (((CARD16 *) (dst))[DoublePixel0] = (pixel);) \
	    break; \
	case 4: \
	    SwitchBitsLoop (((CARD8 *) (dst))[SinglePixel2] = (pixel);) \
	    break; \
	case 5: \
	    SwitchBitsLoop (((CARD8 *) (dst))[SinglePixel0] = (pixel); \
		     ((CARD8 *) (dst))[SinglePixel2] = (pixel);) \
	    break; \
	case 6: \
	    SwitchBitsLoop (((CARD8 *) (dst))[SinglePixel1] = (pixel); \
		     ((CARD8 *) (dst))[SinglePixel2] = (pixel);) \
	    break; \
	case 7: \
	    SwitchBitsLoop (((CARD16 *) (dst))[DoublePixel0] = (pixel); \
		     ((CARD8 *) (dst))[SinglePixel2] = (pixel);) \
	    break; \
	case 8: \
	    SwitchBitsLoop (((CARD8 *) (dst))[SinglePixel3] = (pixel);) \
	    break; \
	case 9: \
	    SwitchBitsLoop (((CARD8 *) (dst))[SinglePixel0] = (pixel); \
		     ((CARD8 *) (dst))[SinglePixel3] = (pixel);) \
	    break; \
	case 10: \
	    SwitchBitsLoop (((CARD8 *) (dst))[SinglePixel1] = (pixel); \
		     ((CARD8 *) (dst))[SinglePixel3] = (pixel);) \
	    break; \
	case 11: \
	    SwitchBitsLoop (((CARD16 *) (dst))[DoublePixel0] = (pixel); \
		     ((CARD8 *) (dst))[SinglePixel3] = (pixel);) \
	    break; \
	case 12: \
	    SwitchBitsLoop (((CARD16 *) (dst))[DoublePixel1] = (pixel);) \
	    break; \
	case 13: \
	    SwitchBitsLoop (((CARD8 *) (dst))[SinglePixel0] = (pixel); \
		     ((CARD16 *) (dst))[DoublePixel1] = (pixel);) \
	    break; \
	case 14: \
	    SwitchBitsLoop (((CARD8 *) (dst))[SinglePixel1] = (pixel); \
		     ((CARD16 *) (dst))[DoublePixel1] = (pixel);) \
	    break; \
	case 15: \
	    SwitchBitsLoop (((CARD32 *) (dst))[0] = (pixel);) \
	    break; \
	} \
}
#else /* PGSZ == 64 */
#define SwitchBitGroup(dst,pixel,bits) { 				   \
    if ( bits == 0xff )							   \
	SwitchBitsLoop (((PixelGroup *) (dst))[OctaPixel0] = (pixel);)	   \
    else {								   \
	switch (bits & 0x0f) {	 					   \
	    case 0: 							   \
       	        break; 							   \
	    case 1: 							   \
	        SwitchBitsLoop (((CARD8 *) (dst))[SinglePixel0] = (pixel);) \
	        break; 							   \
	    case 2: 							   \
	        SwitchBitsLoop (((CARD8 *) (dst))[SinglePixel1] = (pixel);) \
	        break; 							   \
	    case 3: 							   \
	        SwitchBitsLoop (((CARD16 *) (dst))[DoublePixel0] = (pixel);)\
	        break; 							   \
	    case 4: 							   \
	        SwitchBitsLoop (((CARD8 *) (dst))[SinglePixel2] = (pixel);) \
	        break; 							   \
	    case 5: 							   \
	        SwitchBitsLoop (((CARD8 *) (dst))[SinglePixel0] = (pixel);  \
		         	((CARD8 *) (dst))[SinglePixel2] = (pixel);) \
	        break; 							   \
	    case 6: 							   \
	        SwitchBitsLoop (((CARD8 *) (dst))[SinglePixel1] = (pixel);  \
		         	((CARD8 *) (dst))[SinglePixel2] = (pixel);) \
	        break; 							   \
	    case 7: 							   \
	        SwitchBitsLoop (((CARD16 *) (dst))[DoublePixel0] = (pixel); \
		         	((CARD8 *) (dst))[SinglePixel2] = (pixel);) \
	        break; 							   \
	    case 8: 							   \
	        SwitchBitsLoop (((CARD8 *) (dst))[SinglePixel3] = (pixel);) \
	        break; 							   \
	    case 9: 							   \
	        SwitchBitsLoop (((CARD8 *) (dst))[SinglePixel0] = (pixel);  \
		         	((CARD8 *) (dst))[SinglePixel3] = (pixel);) \
	        break; 							   \
	    case 10: 							   \
	        SwitchBitsLoop (((CARD8 *) (dst))[SinglePixel1] = (pixel);  \
		         	((CARD8 *) (dst))[SinglePixel3] = (pixel);) \
	        break; 							   \
	    case 11: 							   \
	        SwitchBitsLoop (((CARD16 *) (dst))[DoublePixel0] = (pixel); \
		         	((CARD8 *) (dst))[SinglePixel3] = (pixel);) \
	        break; 							   \
	    case 12: 							   \
	        SwitchBitsLoop (((CARD16 *) (dst))[DoublePixel1] = (pixel);)\
	        break; 							   \
	    case 13: 							   \
	        SwitchBitsLoop (((CARD8 *) (dst))[SinglePixel0] = (pixel);  \
		         	((CARD16 *) (dst))[DoublePixel1] = (pixel);)\
	        break; 							   \
	    case 14: 							   \
	        SwitchBitsLoop (((CARD8 *) (dst))[SinglePixel1] = (pixel);  \
		         	((CARD16 *) (dst))[DoublePixel1] = (pixel);)\
	        break; 							   \
	    case 15: 							   \
	        SwitchBitsLoop (((CARD32 *) (dst))[QuadPixel0] = (pixel);)    \
	        break; 							   \
	}								   \
	switch ((bits & 0xf0) >> 4) {					   \
	    case 0: 							   \
       	        break; 							   \
	    case 1: 							   \
	        SwitchBitsLoop (((CARD8 *) (dst))[SinglePixel4] = (pixel);) \
	        break; 							   \
	    case 2: 							   \
	        SwitchBitsLoop (((CARD8 *) (dst))[SinglePixel5] = (pixel);) \
	        break; 							   \
	    case 3: 							   \
	        SwitchBitsLoop (((CARD16 *) (dst))[DoublePixel2] = (pixel);)\
	        break; 							   \
	    case 4: 							   \
	        SwitchBitsLoop (((CARD8 *) (dst))[SinglePixel6] = (pixel);) \
	        break; 							   \
	    case 5: 							   \
	        SwitchBitsLoop (((CARD8 *) (dst))[SinglePixel4] = (pixel);  \
		         	((CARD8 *) (dst))[SinglePixel6] = (pixel);) \
	        break; 							   \
	    case 6: 							   \
	        SwitchBitsLoop (((CARD8 *) (dst))[SinglePixel5] = (pixel);  \
		         	((CARD8 *) (dst))[SinglePixel6] = (pixel);) \
	        break; 							   \
	    case 7: 							   \
	        SwitchBitsLoop (((CARD16 *) (dst))[DoublePixel2] = (pixel); \
		         	((CARD8 *) (dst))[SinglePixel6] = (pixel);) \
	        break; 							   \
	    case 8: 							   \
	        SwitchBitsLoop (((CARD8 *) (dst))[SinglePixel7] = (pixel);) \
	        break; 							   \
	    case 9: 							   \
	        SwitchBitsLoop (((CARD8 *) (dst))[SinglePixel4] = (pixel);  \
		         	((CARD8 *) (dst))[SinglePixel7] = (pixel);) \
	        break; 							   \
	    case 10: 							   \
	        SwitchBitsLoop (((CARD8 *) (dst))[SinglePixel5] = (pixel);  \
		         	((CARD8 *) (dst))[SinglePixel7] = (pixel);) \
	        break; 							   \
	    case 11: 							   \
	        SwitchBitsLoop (((CARD16 *) (dst))[DoublePixel2] = (pixel); \
		         	((CARD8 *) (dst))[SinglePixel7] = (pixel);) \
	        break; 							   \
	    case 12: 							   \
	        SwitchBitsLoop (((CARD16 *) (dst))[DoublePixel3] = (pixel);)\
	        break; 							   \
	    case 13: 							   \
	        SwitchBitsLoop (((CARD8 *) (dst))[SinglePixel4] = (pixel);  \
		         	((CARD16 *) (dst))[DoublePixel3] = (pixel);)\
	        break; 							   \
	    case 14: 							   \
	        SwitchBitsLoop (((CARD8 *) (dst))[SinglePixel5] = (pixel);  \
		         	((CARD16 *) (dst))[DoublePixel3] = (pixel);)\
	        break; 							   \
	    case 15: 							   \
	        SwitchBitsLoop (((CARD32 *) (dst))[QuadPixel1] = (pixel);) \
	        break; 							   \
	} 								   \
}
#endif /* PGSZ == 64 */
#endif /* PSZ == 8 */

#if PSZ == 16

#if PGSZ == 32
#define WriteBitGroup(dst,pixel,bits) \
	switch (bits) {			\
	case 0:				\
	    break;			\
	case 1:				\
	    ((CARD16 *) (dst))[SinglePixel0] = (pixel);	\
	    break;			\
	case 2:				\
	    ((CARD16 *) (dst))[SinglePixel1] = (pixel);	\
	    break;			\
	case 3:				\
	    ((CARD32 *) (dst))[DoublePixel0] = (pixel);	\
	    break;			\
	case 4:				\
	    ((CARD16 *) (dst))[SinglePixel2] = (pixel);	\
	    break;			\
	case 5:				\
	    ((CARD16 *) (dst))[SinglePixel0] = (pixel);	\
	    ((CARD16 *) (dst))[SinglePixel2] = (pixel);	\
	    break;			\
	case 6:				\
	    ((CARD16 *) (dst))[SinglePixel1] = (pixel);	\
	    ((CARD16 *) (dst))[SinglePixel2] = (pixel);	\
	    break;			\
	case 7:				\
	    ((CARD32 *) (dst))[DoublePixel0] = (pixel);	\
	    ((CARD16 *) (dst))[SinglePixel2] = (pixel);	\
	    break;			\
	case 8:				\
	    ((CARD16 *) (dst))[SinglePixel3] = (pixel);	\
	    break;			\
	case 9:				\
	    ((CARD16 *) (dst))[SinglePixel0] = (pixel);	\
	    ((CARD16 *) (dst))[SinglePixel3] = (pixel);	\
	    break;			\
	case 10:			\
	    ((CARD16 *) (dst))[SinglePixel1] = (pixel);	\
	    ((CARD16 *) (dst))[SinglePixel3] = (pixel);	\
	    break;			\
	case 11:			\
	    ((CARD32 *) (dst))[DoublePixel0] = (pixel);	\
	    ((CARD16 *) (dst))[SinglePixel3] = (pixel);	\
	    break;			\
	case 12:			\
	    ((CARD32 *) (dst))[DoublePixel1] = (pixel);	\
	    break;			\
	case 13:			\
	    ((CARD16 *) (dst))[SinglePixel0] = (pixel);	\
	    ((CARD32 *) (dst))[DoublePixel1] = (pixel);	\
	    break;			\
	case 14:			\
	    ((CARD16 *) (dst))[SinglePixel1] = (pixel);	\
	    ((CARD32 *) (dst))[DoublePixel1] = (pixel);	\
	    break;			\
	case 15:			\
	    ((CARD32 *) (dst))[DoublePixel0] = (pixel);	\
	    ((CARD32 *) (dst))[DoublePixel1] = (pixel);	\
	    break;			\
	}
#else /* PGSZ == 64 */
#define WriteBitGroup(dst,pixel,bits) \
    if ( bits == 0xff )	{						\
	((PixelGroup *) (dst))[QuadPixel0] = (pixel);			\
	((PixelGroup *) (dst))[QuadPixel1] = (pixel);			\
    }									\
    else {								\
	switch (bits & 0x0f) {	 					\
	case 0:				\
	    break;			\
	case 1:				\
	    ((CARD16 *) (dst))[SinglePixel0] = (pixel);	\
	    break;			\
	case 2:				\
	    ((CARD16 *) (dst))[SinglePixel1] = (pixel);	\
	    break;			\
	case 3:				\
	    ((CARD32 *) (dst))[DoublePixel0] = (pixel);	\
	    break;			\
	case 4:				\
	    ((CARD16 *) (dst))[SinglePixel2] = (pixel);	\
	    break;			\
	case 5:				\
	    ((CARD16 *) (dst))[SinglePixel0] = (pixel);	\
	    ((CARD16 *) (dst))[SinglePixel2] = (pixel);	\
	    break;			\
	case 6:				\
	    ((CARD16 *) (dst))[SinglePixel1] = (pixel);	\
	    ((CARD16 *) (dst))[SinglePixel2] = (pixel);	\
	    break;			\
	case 7:				\
	    ((CARD32 *) (dst))[DoublePixel0] = (pixel);	\
	    ((CARD16 *) (dst))[SinglePixel2] = (pixel);	\
	    break;			\
	case 8:				\
	    ((CARD16 *) (dst))[SinglePixel3] = (pixel);	\
	    break;			\
	case 9:				\
	    ((CARD16 *) (dst))[SinglePixel0] = (pixel);	\
	    ((CARD16 *) (dst))[SinglePixel3] = (pixel);	\
	    break;			\
	case 10:			\
	    ((CARD16 *) (dst))[SinglePixel1] = (pixel);	\
	    ((CARD16 *) (dst))[SinglePixel3] = (pixel);	\
	    break;			\
	case 11:			\
	    ((CARD32 *) (dst))[DoublePixel0] = (pixel);	\
	    ((CARD16 *) (dst))[SinglePixel3] = (pixel);	\
	    break;			\
	case 12:			\
	    ((CARD32 *) (dst))[DoublePixel1] = (pixel);	\
	    break;			\
	case 13:			\
	    ((CARD16 *) (dst))[SinglePixel0] = (pixel);	\
	    ((CARD32 *) (dst))[DoublePixel1] = (pixel);	\
	    break;			\
	case 14:			\
	    ((CARD16 *) (dst))[SinglePixel1] = (pixel);	\
	    ((CARD32 *) (dst))[DoublePixel1] = (pixel);	\
	    break;			\
	case 15:			\
	    ((CARD32 *) (dst))[DoublePixel0] = (pixel);	\
	    ((CARD32 *) (dst))[DoublePixel1] = (pixel);	\
	    break;			\
	}				\
	switch ((bits & 0xf0) >> 4) {			\
	case 0:				\
	    break;			\
	case 1:				\
	    ((CARD16 *) (dst))[SinglePixel4] = (pixel);	\
	    break;			\
	case 2:				\
	    ((CARD16 *) (dst))[SinglePixel5] = (pixel);	\
	    break;			\
	case 3:				\
	    ((CARD32 *) (dst))[DoublePixel2] = (pixel);	\
	    break;			\
	case 4:				\
	    ((CARD16 *) (dst))[SinglePixel6] = (pixel);	\
	    break;			\
	case 5:				\
	    ((CARD16 *) (dst))[SinglePixel4] = (pixel);	\
	    ((CARD16 *) (dst))[SinglePixel6] = (pixel);	\
	    break;			\
	case 6:				\
	    ((CARD16 *) (dst))[SinglePixel5] = (pixel);	\
	    ((CARD16 *) (dst))[SinglePixel6] = (pixel);	\
	    break;			\
	case 7:				\
	    ((CARD32 *) (dst))[DoublePixel2] = (pixel);	\
	    ((CARD16 *) (dst))[SinglePixel6] = (pixel);	\
	    break;			\
	case 8:				\
	    ((CARD16 *) (dst))[SinglePixel7] = (pixel);	\
	    break;			\
	case 9:				\
	    ((CARD16 *) (dst))[SinglePixel4] = (pixel);	\
	    ((CARD16 *) (dst))[SinglePixel7] = (pixel);	\
	    break;			\
	case 10:			\
	    ((CARD16 *) (dst))[SinglePixel5] = (pixel);	\
	    ((CARD16 *) (dst))[SinglePixel7] = (pixel);	\
	    break;			\
	case 11:			\
	    ((CARD32 *) (dst))[DoublePixel2] = (pixel);	\
	    ((CARD16 *) (dst))[SinglePixel7] = (pixel);	\
	    break;			\
	case 12:			\
	    ((CARD32 *) (dst))[DoublePixel3] = (pixel);	\
	    break;			\
	case 13:			\
	    ((CARD16 *) (dst))[SinglePixel4] = (pixel);	\
	    ((CARD32 *) (dst))[DoublePixel3] = (pixel);	\
	    break;			\
	case 14:			\
	    ((CARD16 *) (dst))[SinglePixel5] = (pixel);	\
	    ((CARD32 *) (dst))[DoublePixel3] = (pixel);	\
	    break;			\
	case 15:			\
	    ((CARD32 *) (dst))[DoublePixel2] = (pixel);	\
	    ((CARD32 *) (dst))[DoublePixel3] = (pixel);	\
	    break;			\
	}				\
    }
#endif /* PGSZ */

#if PGSZ == 32
#define SwitchBitGroup(dst,pixel,bits) { \
	switch (bits) { \
	case 0: \
       	    break; \
	case 1: \
	    SwitchBitsLoop (((CARD16 *) (dst))[SinglePixel0] = (pixel);) \
	    break; \
	case 2: \
	    SwitchBitsLoop (((CARD16 *) (dst))[SinglePixel1] = (pixel);) \
	    break; \
	case 3: \
	    SwitchBitsLoop (((CARD32 *) (dst))[DoublePixel0] = (pixel);) \
	    break; \
	case 4: \
	    SwitchBitsLoop (((CARD16 *) (dst))[SinglePixel2] = (pixel);) \
	    break; \
	case 5: \
	    SwitchBitsLoop (((CARD16 *) (dst))[SinglePixel0] = (pixel); \
		     ((CARD16 *) (dst))[SinglePixel2] = (pixel);) \
	    break; \
	case 6: \
	    SwitchBitsLoop (((CARD16 *) (dst))[SinglePixel1] = (pixel); \
		     ((CARD16 *) (dst))[SinglePixel2] = (pixel);) \
	    break; \
	case 7: \
	    SwitchBitsLoop (((CARD32 *) (dst))[DoublePixel0] = (pixel); \
		     ((CARD16 *) (dst))[SinglePixel2] = (pixel);) \
	    break; \
	case 8: \
	    SwitchBitsLoop (((CARD16 *) (dst))[SinglePixel3] = (pixel);) \
	    break; \
	case 9: \
	    SwitchBitsLoop (((CARD16 *) (dst))[SinglePixel0] = (pixel); \
		     ((CARD16 *) (dst))[SinglePixel3] = (pixel);) \
	    break; \
	case 10: \
	    SwitchBitsLoop (((CARD16 *) (dst))[SinglePixel1] = (pixel); \
		     ((CARD16 *) (dst))[SinglePixel3] = (pixel);) \
	    break; \
	case 11: \
	    SwitchBitsLoop (((CARD32 *) (dst))[DoublePixel0] = (pixel); \
		     ((CARD16 *) (dst))[SinglePixel3] = (pixel);) \
	    break; \
	case 12: \
	    SwitchBitsLoop (((CARD32 *) (dst))[DoublePixel1] = (pixel);) \
	    break; \
	case 13: \
	    SwitchBitsLoop (((CARD16 *) (dst))[SinglePixel0] = (pixel); \
		     ((CARD32 *) (dst))[DoublePixel1] = (pixel);) \
	    break; \
	case 14: \
	    SwitchBitsLoop (((CARD16 *) (dst))[SinglePixel1] = (pixel); \
		     ((CARD32 *) (dst))[DoublePixel1] = (pixel);) \
	    break; \
	case 15: \
	    SwitchBitsLoop (((CARD32 *) (dst))[DoublePixel0] = (pixel); \
			    ((CARD32 *) (dst))[DoublePixel1] = (pixel);) \
	    break; \
	} \
}
#else /* PGSZ == 64 */
#define SwitchBitGroup(dst,pixel,bits) { \
	cfb cannot hack 64-bit SwitchBitGroup psz=PSZ
#endif /* PGSZ */

#endif /* PSZ == 16 */


#if PSZ == 32

#if PGSZ == 32
#define WriteBitGroup(dst,pixel,bits) \
	switch (bits) {			\
	case 0:				\
	    break;			\
	case 1:				\
	    ((CARD32 *) (dst))[SinglePixel0] = (pixel);	\
	    break;			\
	case 2:				\
	    ((CARD32 *) (dst))[SinglePixel1] = (pixel);	\
	    break;			\
	case 3:				\
	    ((CARD32 *) (dst))[SinglePixel0] = (pixel);	\
	    ((CARD32 *) (dst))[SinglePixel1] = (pixel);	\
	    break;			\
	case 4:				\
	    ((CARD32 *) (dst))[SinglePixel2] = (pixel);	\
	    break;			\
	case 5:				\
	    ((CARD32 *) (dst))[SinglePixel0] = (pixel);	\
	    ((CARD32 *) (dst))[SinglePixel2] = (pixel);	\
	    break;			\
	case 6:				\
	    ((CARD32 *) (dst))[SinglePixel1] = (pixel);	\
	    ((CARD32 *) (dst))[SinglePixel2] = (pixel);	\
	    break;			\
	case 7:				\
	    ((CARD32 *) (dst))[SinglePixel0] = (pixel);	\
	    ((CARD32 *) (dst))[SinglePixel1] = (pixel);	\
	    ((CARD32 *) (dst))[SinglePixel2] = (pixel);	\
	    break;			\
	case 8:				\
	    ((CARD32 *) (dst))[SinglePixel3] = (pixel);	\
	    break;			\
	case 9:				\
	    ((CARD32 *) (dst))[SinglePixel0] = (pixel);	\
	    ((CARD32 *) (dst))[SinglePixel3] = (pixel);	\
	    break;			\
	case 10:			\
	    ((CARD32 *) (dst))[SinglePixel1] = (pixel);	\
	    ((CARD32 *) (dst))[SinglePixel3] = (pixel);	\
	    break;			\
	case 11:			\
	    ((CARD32 *) (dst))[SinglePixel0] = (pixel);	\
	    ((CARD32 *) (dst))[SinglePixel1] = (pixel);	\
	    ((CARD32 *) (dst))[SinglePixel3] = (pixel);	\
	    break;			\
	case 12:			\
	    ((CARD32 *) (dst))[SinglePixel2] = (pixel);	\
	    ((CARD32 *) (dst))[SinglePixel3] = (pixel);	\
	    break;			\
	case 13:			\
	    ((CARD32 *) (dst))[SinglePixel0] = (pixel);	\
	    ((CARD32 *) (dst))[SinglePixel2] = (pixel);	\
	    ((CARD32 *) (dst))[SinglePixel3] = (pixel);	\
	    break;			\
	case 14:			\
	    ((CARD32 *) (dst))[SinglePixel1] = (pixel);	\
	    ((CARD32 *) (dst))[SinglePixel2] = (pixel);	\
	    ((CARD32 *) (dst))[SinglePixel3] = (pixel);	\
	    break;			\
	case 15:			\
	    ((CARD32 *) (dst))[SinglePixel0] = (pixel);	\
	    ((CARD32 *) (dst))[SinglePixel1] = (pixel);	\
	    ((CARD32 *) (dst))[SinglePixel2] = (pixel);	\
	    ((CARD32 *) (dst))[SinglePixel3] = (pixel);	\
	    break;			\
	}
#else /* PGSZ == 64 */
#define WriteBitGroup(dst,pixel,bits) \
    if ( bits == 0xff )	 {				   \
	((PixelGroup *) (dst))[DoublePixel0] = (pixel);	   \
	((PixelGroup *) (dst))[DoublePixel1] = (pixel);	   \
	((PixelGroup *) (dst))[DoublePixel2] = (pixel);	   \
	((PixelGroup *) (dst))[DoublePixel3] = (pixel);	   \
    }							   \
    else {						   \
	switch (bits & 0x0f) {	 			   \
	case 0:				\
	    break;			\
	case 1:				\
	    ((CARD32 *) (dst))[SinglePixel0] = (pixel);	\
	    break;			\
	case 2:				\
	    ((CARD32 *) (dst))[SinglePixel1] = (pixel);	\
	    break;			\
	case 3:				\
	    ((CARD32 *) (dst))[SinglePixel0] = (pixel);	\
	    ((CARD32 *) (dst))[SinglePixel1] = (pixel);	\
	    break;			\
	case 4:				\
	    ((CARD32 *) (dst))[SinglePixel2] = (pixel);	\
	    break;			\
	case 5:				\
	    ((CARD32 *) (dst))[SinglePixel0] = (pixel);	\
	    ((CARD32 *) (dst))[SinglePixel2] = (pixel);	\
	    break;			\
	case 6:				\
	    ((CARD32 *) (dst))[SinglePixel1] = (pixel);	\
	    ((CARD32 *) (dst))[SinglePixel2] = (pixel);	\
	    break;			\
	case 7:				\
	    ((CARD32 *) (dst))[SinglePixel0] = (pixel);	\
	    ((CARD32 *) (dst))[SinglePixel1] = (pixel);	\
	    ((CARD32 *) (dst))[SinglePixel2] = (pixel);	\
	    break;			\
	case 8:				\
	    ((CARD32 *) (dst))[SinglePixel3] = (pixel);	\
	    break;			\
	case 9:				\
	    ((CARD32 *) (dst))[SinglePixel0] = (pixel);	\
	    ((CARD32 *) (dst))[SinglePixel3] = (pixel);	\
	    break;			\
	case 10:			\
	    ((CARD32 *) (dst))[SinglePixel1] = (pixel);	\
	    ((CARD32 *) (dst))[SinglePixel3] = (pixel);	\
	    break;			\
	case 11:			\
	    ((CARD32 *) (dst))[SinglePixel0] = (pixel);	\
	    ((CARD32 *) (dst))[SinglePixel1] = (pixel);	\
	    ((CARD32 *) (dst))[SinglePixel3] = (pixel);	\
	    break;			\
	case 12:			\
	    ((CARD32 *) (dst))[SinglePixel2] = (pixel);	\
	    ((CARD32 *) (dst))[SinglePixel3] = (pixel);	\
	    break;			\
	case 13:			\
	    ((CARD32 *) (dst))[SinglePixel0] = (pixel);	\
	    ((CARD32 *) (dst))[SinglePixel2] = (pixel);	\
	    ((CARD32 *) (dst))[SinglePixel3] = (pixel);	\
	    break;			\
	case 14:			\
	    ((CARD32 *) (dst))[SinglePixel1] = (pixel);	\
	    ((CARD32 *) (dst))[SinglePixel2] = (pixel);	\
	    ((CARD32 *) (dst))[SinglePixel3] = (pixel);	\
	    break;			\
	case 15:			\
	    ((CARD32 *) (dst))[SinglePixel0] = (pixel);	\
	    ((CARD32 *) (dst))[SinglePixel1] = (pixel);	\
	    ((CARD32 *) (dst))[SinglePixel2] = (pixel);	\
	    ((CARD32 *) (dst))[SinglePixel3] = (pixel);	\
	    break;			\
	}				\
	switch ((bits & 0xf0) >> 4) {	\
	case 0:				\
	    break;			\
	case 1:				\
	    ((CARD32 *) (dst))[SinglePixel4] = (pixel);	\
	    break;			\
	case 2:				\
	    ((CARD32 *) (dst))[SinglePixel5] = (pixel);	\
	    break;			\
	case 3:				\
	    ((CARD32 *) (dst))[SinglePixel4] = (pixel);	\
	    ((CARD32 *) (dst))[SinglePixel5] = (pixel);	\
	    break;			\
	case 4:				\
	    ((CARD32 *) (dst))[SinglePixel6] = (pixel);	\
	    break;			\
	case 5:				\
	    ((CARD32 *) (dst))[SinglePixel4] = (pixel);	\
	    ((CARD32 *) (dst))[SinglePixel6] = (pixel);	\
	    break;			\
	case 6:				\
	    ((CARD32 *) (dst))[SinglePixel5] = (pixel);	\
	    ((CARD32 *) (dst))[SinglePixel6] = (pixel);	\
	    break;			\
	case 7:				\
	    ((CARD32 *) (dst))[SinglePixel4] = (pixel);	\
	    ((CARD32 *) (dst))[SinglePixel5] = (pixel);	\
	    ((CARD32 *) (dst))[SinglePixel6] = (pixel);	\
	    break;			\
	case 8:				\
	    ((CARD32 *) (dst))[SinglePixel7] = (pixel);	\
	    break;			\
	case 9:				\
	    ((CARD32 *) (dst))[SinglePixel4] = (pixel);	\
	    ((CARD32 *) (dst))[SinglePixel7] = (pixel);	\
	    break;			\
	case 10:			\
	    ((CARD32 *) (dst))[SinglePixel5] = (pixel);	\
	    ((CARD32 *) (dst))[SinglePixel7] = (pixel);	\
	    break;			\
	case 11:			\
	    ((CARD32 *) (dst))[SinglePixel4] = (pixel);	\
	    ((CARD32 *) (dst))[SinglePixel5] = (pixel);	\
	    ((CARD32 *) (dst))[SinglePixel7] = (pixel);	\
	    break;			\
	case 12:			\
	    ((CARD32 *) (dst))[SinglePixel6] = (pixel);	\
	    ((CARD32 *) (dst))[SinglePixel7] = (pixel);	\
	    break;			\
	case 13:			\
	    ((CARD32 *) (dst))[SinglePixel4] = (pixel);	\
	    ((CARD32 *) (dst))[SinglePixel6] = (pixel);	\
	    ((CARD32 *) (dst))[SinglePixel7] = (pixel);	\
	    break;			\
	case 14:			\
	    ((CARD32 *) (dst))[SinglePixel5] = (pixel);	\
	    ((CARD32 *) (dst))[SinglePixel6] = (pixel);	\
	    ((CARD32 *) (dst))[SinglePixel7] = (pixel);	\
	    break;			\
	case 15:			\
	    ((CARD32 *) (dst))[SinglePixel4] = (pixel);	\
	    ((CARD32 *) (dst))[SinglePixel5] = (pixel);	\
	    ((CARD32 *) (dst))[SinglePixel6] = (pixel);	\
	    ((CARD32 *) (dst))[SinglePixel7] = (pixel);	\
	    break;			\
	}				\
    }
#endif /* PGSZ */

#if PGSZ == 32
#define SwitchBitGroup(dst,pixel,bits) { \
	switch (bits) { \
	case 0: \
       	    break; \
	case 1: \
	    SwitchBitsLoop (((CARD32 *) (dst))[SinglePixel0] = (pixel);) \
	    break; \
	case 2: \
	    SwitchBitsLoop (((CARD32 *) (dst))[SinglePixel1] = (pixel);) \
	    break; \
	case 3: \
	    SwitchBitsLoop (((CARD32 *) (dst))[SinglePixel0] = (pixel); \
			    ((CARD32 *) (dst))[SinglePixel1] = (pixel);) \
	    break; \
	case 4: \
	    SwitchBitsLoop (((CARD32 *) (dst))[SinglePixel2] = (pixel);) \
	    break; \
	case 5: \
	    SwitchBitsLoop (((CARD32 *) (dst))[SinglePixel0] = (pixel); \
			    ((CARD32 *) (dst))[SinglePixel2] = (pixel);) \
	    break; \
	case 6: \
	    SwitchBitsLoop (((CARD32 *) (dst))[SinglePixel1] = (pixel); \
			    ((CARD32 *) (dst))[SinglePixel2] = (pixel);) \
	    break; \
	case 7: \
	    SwitchBitsLoop (((CARD32 *) (dst))[SinglePixel0] = (pixel); \
			    ((CARD32 *) (dst))[SinglePixel1] = (pixel); \
			    ((CARD32 *) (dst))[SinglePixel2] = (pixel);) \
	    break; \
	case 8: \
	    SwitchBitsLoop (((CARD32 *) (dst))[SinglePixel3] = (pixel);) \
	    break; \
	case 9: \
	    SwitchBitsLoop (((CARD32 *) (dst))[SinglePixel0] = (pixel); \
			    ((CARD32 *) (dst))[SinglePixel3] = (pixel);) \
	    break; \
	case 10: \
	    SwitchBitsLoop (((CARD32 *) (dst))[SinglePixel1] = (pixel); \
			    ((CARD32 *) (dst))[SinglePixel3] = (pixel);) \
	    break; \
	case 11: \
	    SwitchBitsLoop (((CARD32 *) (dst))[SinglePixel0] = (pixel); \
			    ((CARD32 *) (dst))[SinglePixel1] = (pixel); \
			    ((CARD32 *) (dst))[SinglePixel3] = (pixel);) \
	    break; \
	case 12: \
	    SwitchBitsLoop (((CARD32 *) (dst))[SinglePixel2] = (pixel); \
			    ((CARD32 *) (dst))[SinglePixel3] = (pixel);) \
	    break; \
	case 13: \
	    SwitchBitsLoop (((CARD32 *) (dst))[SinglePixel0] = (pixel); \
			    ((CARD32 *) (dst))[SinglePixel2] = (pixel); \
			    ((CARD32 *) (dst))[SinglePixel3] = (pixel);) \
	    break; \
	case 14: \
	    SwitchBitsLoop (((CARD32 *) (dst))[SinglePixel1] = (pixel); \
			    ((CARD32 *) (dst))[SinglePixel2] = (pixel); \
			    ((CARD32 *) (dst))[SinglePixel3] = (pixel);) \
	    break; \
	case 15: \
	    SwitchBitsLoop (((CARD32 *) (dst))[SinglePixel0] = (pixel); \
			    ((CARD32 *) (dst))[SinglePixel1] = (pixel); \
			    ((CARD32 *) (dst))[SinglePixel2] = (pixel); \
			    ((CARD32 *) (dst))[SinglePixel3] = (pixel);) \
	    break; \
	} \
}
#else /* PGSZ == 64 */
#define SwitchBitGroup(dst,pixel,bits) { \
	cfb cannot hack 64-bit SwitchBitGroup psz=PSZ
#endif /* PGSZ */

#endif /* PSZ == 32 */
#endif /* AVOID_MEMORY_READ */

extern PixelGroup cfb8BitLenMasks[PGSZ];
