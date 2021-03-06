/* $XConsortium: vgaBitBlt.c,v 1.2 94/10/12 21:06:18 kaleb Exp $ */
/* $XFree86: xc/programs/Xserver/hw/xfree86/vga16/ibm/vgaBitBlt.c,v 3.0 1994/05/04 15:03:44 dawes Exp $ */
/* GJA -- span move routines */

#include "X.h"
#include "OScompiler.h"
#include "vgaReg.h"
#include "vgaVideo.h"

#include "windowstr.h" /* GJA -- for pWin */
#include "scrnintstr.h" /* GJA -- for pWin */
#include "pixmapstr.h" /* GJA -- for pWin */
#include "ppc.h" /* GJA -- for pWin */

/* NOTE: It seems that there is no way to program the VGA to copy just
 * a part of a byte in the smarter modes. Therefore we copy the boundaries
 * plane by plane.
 */
#define WORDSZ 8
 /* The fast blit code requires WORDSZ = 8 for its read-modify write cycle.
  * Therefore, we do not fully implement the other options.
  */
#define HIGHPLANEMASK 0x08
#define HIGHPLANEINDEX 3

/* Of course, we want the following anyway:
 * (Yes, they're identical now.)
 */
#define SMEM(x,y) ( VIDBASE(pWin) + (y) * BYTES_PER_LINE(pWin) + (x) )
#define DMEM(x,y) ( VIDBASE(pWin) + (y) * BYTES_PER_LINE(pWin) + (x) )

#define WORD8 unsigned char
#define LW8 BYTES_PER_LINE(pWin) /* Line width */
#define WSHIFT8 0x3
#define WMASK8 0x07
/* NOTE: lmask[8] matters. It must be different from lmask[0] */
unsigned char lmasktab[] = {
	0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE, 0xFF
} ;
unsigned char rmasktab[] = {
	0xFF, 0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x01, 0x00
} ;

#define LMASK8(n) lmasktab[n]
#define RMASK8(n) rmasktab[n]
#define SWAPB8(x) (x)

#if (WORDSZ == 8)

#define WORD WORD8
#define LW LW8
#define WSHIFT WSHIFT8
#define WMASK WMASK8

#define LMASK(n) LMASK8(n)
#define RMASK(n) RMASK8(n)
#define SWAPB(x) SWAPB8(x)

#endif /* WORDSZ == 8 */

#define DO_ALU(dst,src,mask,alu) {\
	int _ndst, _odst; _odst = dst; \
	switch ( alu ) { \
	case GXclear: \
		_ndst = 0;			break; \
	case GXand: \
		_ndst = src & _odst;		break; \
	case GXandReverse: \
		_ndst = src & ~ _odst;		break; \
	case GXcopy: \
		_ndst = src;			break; \
	case GXandInverted: \
		_ndst = ~ src & _odst;		break; \
	case GXnoop: \
		_ndst = _odst;			break; \
	case GXxor: \
		_ndst = src ^ _odst;		break; \
	case GXor: \
		_ndst = src | _odst;		break; \
	case GXnor: \
		_ndst = ~ src & ~ _odst;	break; \
	case GXequiv: \
		_ndst = ~ src ^ _odst;		break; \
	case GXinvert: \
		_ndst = ~ _odst;		break; \
	case GXorReverse: \
		_ndst = src | ~ _odst;		break; \
	case GXcopyInverted: \
		_ndst = ~ src;			break; \
	case GXorInverted: \
		_ndst = ~ src | _odst;		break; \
	case GXnand: \
		_ndst = ~ src | ~ _odst;	break; \
	case GXset: \
		_ndst = ~0;			break; \
	} \
	dst = (_odst & ~(mask)) | (_ndst & (mask)); \
	}


void vgaBitBlt(pWin,alu,readplanes,writeplanes,x0,y0,x1,y1,w,h)
WindowPtr pWin; /* GJA */
int alu;
int readplanes; /* unused */
int writeplanes; /* planes */
int x0, y0, x1, y1, w, h;
{
    int plane, bit;
    extern int xf86VTSema;

    if ( !w || !h ) return;

    if ( ! xf86VTSema ) {
        offBitBlt(pWin,alu,readplanes,writeplanes,x0,y0,x1,y1,w,h);
        return;
    }

    /* 0x7, not WMASK: it is hardware dependant */
    if ( ((x0 - x1) & 0x7) || (alu != GXcopy) ) {
	/* Use slow copy */
	SetVideoGraphics(Enb_Set_ResetIndex, 0); /* All from CPU */
	SetVideoGraphics(Bit_MaskIndex, 0xFF); /* All bits */
	SetVideoGraphics(Graphics_ModeIndex, 0); /* Write mode 0 */
	SetVideoGraphics(Data_RotateIndex, 0); /* Don't rotate, replace */

	for ( plane = HIGHPLANEMASK, bit = HIGHPLANEINDEX ;
		plane ; plane >>= 1, bit-- )
	{

		if ( writeplanes & plane) {
			SetVideoGraphics(Read_Map_SelectIndex, bit);
			SetVideoSequencer(Mask_MapIndex, plane);

			shift(pWin,x0,x1,y0,y1,w,h,alu);
		}
	}
    } else {
        aligned_blit(pWin,x0,x1,y0,y1,w,h,alu,writeplanes);
    }
}

/* Copy a span a number of places to the right.
 */
shift(pWin,x0,x1,y0,y1,w,h,alu)
WindowPtr pWin; /* GJA */
int x0;  /* left edge of source */
int x1;  /* left edge of target */
int y0;
int y1;
int w; /* length of source, and of target */
int h;
int alu;
{
  if ( ((x1 & WMASK) + w) <= WORDSZ ) {
     shift_thin_rect(pWin,x0,x1,y0,y1,w,h,alu);
  } else if ( x1 > x0 ) { /* Shift right: start right */
     int l1 = x1 & WMASK, r1 = (x1 + w) & WMASK;

     if ( r1 ) /* right edge */
        shift_thin_rect(pWin,x0+w-r1,x1+w-r1,y0,y1,r1,h,alu);
     shift_center(pWin,x0,x1,y0,y1,w,h,alu);
     if ( l1 ) /* left edge */
        shift_thin_rect(pWin,x0,x1,y0,y1,(WORDSZ-l1),h,alu);
  } else {
     int l1 = x1 & WMASK, r1 = (x1 + w) & WMASK;

     if ( l1 ) /* left edge */
        shift_thin_rect(pWin,x0,x1,y0,y1,(WORDSZ-l1),h,alu);
     shift_center(pWin,x0,x1,y0,y1,w,h,alu);
     if ( r1 ) /* right edge */
        shift_thin_rect(pWin,x0+w-r1,x1+w-r1,y0,y1,r1,h,alu);
  }
}

/* The whole rectangle is so thin that it fits in one byte written */
shift_thin_rect(pWin,x0,x1,y0,y1,w,h,alu)
WindowPtr pWin; /* GJA */
int x0;  /* left edge of source */
int x1;  /* left edge of target */
int y0;
int y1;
int w; /* length of source, and of target */
int h;
int alu;
{
  int l0 = x0 & WMASK; /* Left edge of source, as bit */
  int l1 = x1 & WMASK; /* Left edge of target, as bit */
  int L0 = x0 >> WSHIFT; /* Left edge of source, as byte */
  int L1 = x1 >> WSHIFT; /* Left edge of target, as byte */
  int pad;
  int htmp;
  int mask;
  int tmp;
  int bs;
  
  volatile unsigned char *sp, *dp;

  mask = RMASK(l1) & LMASK(l1+w);
  bs = (x1 - x0) & WMASK;

  if ( y1 > y0 ) { /* Move down, start at the bottom */
    pad = - BYTES_PER_LINE(pWin);
    sp = SMEM(L0,y0+h-1);
    dp = DMEM(L1,y1+h-1);
  } else { /* Move up, start at the top */
    pad = BYTES_PER_LINE(pWin);
    sp = SMEM(L0,y0);
    dp = DMEM(L1,y1);
  }
  VSETW(dp); VSETR(sp);

  if ( l0+w > WORDSZ ) {
    /* Need two bytes */
    for ( htmp = h ; htmp ; htmp-- ) {
      tmp = (sp[0] << (WORDSZ - bs));
      VINCR(sp);
      tmp |= (sp[0] >> bs);
      VDECR(sp);
      VPUSHR();
      DO_ALU(dp[0],tmp,mask,alu);
      ADDW(dp,pad);
      VPOPR();
      ADDR(sp,pad);
    }
  } else if ( l0 <= l1 ) {
    /* Need one byte, shifted right */
    for ( htmp = h ; htmp ; htmp-- ) {
      tmp = (sp[0] >> bs);
      VPUSHR();
      DO_ALU(dp[0],tmp,mask,alu);
      ADDW(dp,pad);
      VPOPR();
      ADDR(sp,pad);
    }
  } else {
    /* Need one byte, shifted left */
    for ( htmp = h ; htmp ; htmp-- ) {
      tmp = (sp[0] << (WORDSZ - bs));
      VPUSHR();
      DO_ALU(dp[0],tmp,mask,alu);
      ADDW(dp,pad);
      VPOPR();
      ADDR(sp,pad);
    }
  }
}

shift_center(pWin,x0,x1,y0,y1,w,h,alu)
WindowPtr pWin; /* GJA */
int x0;  /* left edge of source */
int x1;  /* left edge of target */
int y0;
int y1;
int w; /* length of source, and of target */
int h;
int alu;
{
  int l0 = x0 & WMASK; /* Left edge of source, as bit */
  int l1 = x1 & WMASK; /* Left edge of target, as bit */
  int r0 = (x0 + w) & WMASK; /* Right edge of source, as bit */
  int r1 = (x1 + w) & WMASK; /* Right edge of target, as bit */
  int L0 = x0 >> WSHIFT; /* Left edge of source, as byte */
  int L1 = x1 >> WSHIFT; /* Left edge of target, as byte */
  int pad;
  int htmp, wtmp; /* Temporaries for indices over height and width */
  volatile unsigned char tmp; /* Temporary result of the shifts */
  int bs;
  int rem; /* Remaining bits; temporary in loop */
  int bytecnt;
  
  volatile unsigned char *sp, *dp;

  bs = (x1 - x0) & WMASK;

  if ( l1 ) {
     bytecnt = (w - (WORDSZ - l1) - r1) >> WSHIFT;
     sp = SMEM( ((x0 + (WORDSZ - l1)) >> WSHIFT), y0);
     dp = DMEM( ((x1 + (WORDSZ - l1)) >> WSHIFT), y1);
  } else {
     bytecnt = (w - r1) >> WSHIFT;
     sp = SMEM( (x0 >> WSHIFT), y0);
     dp = DMEM( (x1 >> WSHIFT), y1);
  }

  if ( y1 > y0 ) { /* Move down, start at the bottom */
    if ( x1 > x0 ) { /* Move right, start right */
       pad = - BYTES_PER_LINE(pWin) + bytecnt;
       sp += BYTES_PER_LINE(pWin) * (h - 1) + bytecnt - 1;
       dp += BYTES_PER_LINE(pWin) * (h - 1) + bytecnt - 1;
    } else { /* Move left, start left */
       pad = - BYTES_PER_LINE(pWin) - bytecnt;
       sp += BYTES_PER_LINE(pWin) * (h - 1);
       dp += BYTES_PER_LINE(pWin) * (h - 1);
    }
  } else { /* Move up, start at the top */
    if ( x1 > x0 ) { /* Move right, start right */
       pad = BYTES_PER_LINE(pWin) + bytecnt;
       sp += bytecnt - 1;
       dp += bytecnt - 1;
    } else { /* Move left, start left */
       pad = BYTES_PER_LINE(pWin) - bytecnt;
       sp += 0;
       dp += 0;
    }
  }
  VSETW(dp); VSETR(sp);

  if ( x1 > x0 ) { /* Move right, start right */
    if ( bs == 0 ) { /* No shift. Need one byte only */
      for ( htmp = h ; htmp ; htmp-- ) {
        for ( wtmp = bytecnt ; wtmp ; wtmp-- ) {
          tmp = sp[0];
          VPUSHR();
          DO_ALU(dp[0],tmp,~0,alu); 
	  VDECW(dp);
	  VPOPR();
          VDECR(sp);
        }
	VPUSHR();
        ADDW(dp,pad);
        VPOPR();
        ADDR(sp,pad);
      } 
    } else {
      for ( htmp = h ; htmp ; htmp-- ) {
	if ( bytecnt ) {
	   VINCR(sp);
   	   rem = sp[0];
	   VDECR(sp);
           for ( wtmp = bytecnt ; wtmp ; wtmp-- ) {
             tmp = (rem >> bs);
             rem = sp[0];
             tmp |= (rem << (WORDSZ - bs)) ;
	     VPUSHR();
             DO_ALU(dp[0],tmp,~0,alu); 
	     VDECW(dp);
	     VPOPR();
             VDECR(sp);
           }
        }
	VPUSHR();
        ADDW(dp,pad);
        VPOPR();
        ADDR(sp,pad);
      } 
    }
  } else { /* x1 <= x0 */ /* Move left, start left */
    if ( bs == 0 ) { /* No shift. Need one byte only */
      for ( htmp = h ; htmp ; htmp-- ) {
        for ( wtmp = bytecnt ; wtmp ; wtmp-- ) {
          tmp = sp[0];
	  VPUSHR();
          DO_ALU(dp[0],tmp,~0,alu); 
	  VINCW(dp);
          VPOPR();
          VINCR(sp);
        }
	VPUSHR();
        ADDW(dp,pad);
        VPOPR();
        ADDR(sp,pad);
      } 
    } else {
      for ( htmp = h ; htmp ; htmp-- ) {
        if ( bytecnt ) {
          rem = sp[0];
          for ( wtmp = bytecnt ; wtmp ; wtmp-- ) {
            tmp = (rem << (WORDSZ - bs));
	    VINCR(sp);
	    rem = sp[0];
	    VDECR(sp);
            tmp |= (rem >> bs);
	    VPUSHR();
            DO_ALU(dp[0],tmp,~0,alu); 
	    VINCW(dp);
	    VPOPR();
            VINCR(sp);
          }
        }
	VPUSHR();
        ADDW(dp,pad);
        VPOPR();
        ADDR(sp,pad);
      } 
    }
  }
}

/* Copy a rectangle.
 */
aligned_blit(pWin,x0,x1,y0,y1,w,h,alu,planes)
WindowPtr pWin; /* GJA */
int x0;  /* left edge of source */
int x1;  /* left edge of target */
int y0;
int y1;
int w; /* length of source, and of target */
int h;
int alu;
int planes;
{
  int plane, bit;

  if ( ((x1 & WMASK) + w) <= WORDSZ ) {
	SetVideoGraphics(Enb_Set_ResetIndex, 0); /* All from CPU */
	SetVideoGraphics(Bit_MaskIndex, 0xFF); /* All bits */
	SetVideoGraphics(Graphics_ModeIndex, 0); /* Write mode 0 */
	SetVideoGraphics(Data_RotateIndex, 0); /* Don't rotate, replace */

	for ( plane = HIGHPLANEMASK, bit = HIGHPLANEINDEX;
		plane ; plane >>= 1, bit-- )
	{
		if ( planes & plane) {
			SetVideoGraphics(Read_Map_SelectIndex, bit);
			SetVideoSequencer(Mask_MapIndex, plane);

     			shift_thin_rect(pWin,x0,x1,y0,y1,w,h,alu);
		}
	}
  } else if ( x1 > x0 ) { /* Shift right: start right */
     int l1 = x1 & WMASK, r1 = (x1 + w) & WMASK;

     if ( r1 ) { /* right edge */
	SetVideoGraphics(Enb_Set_ResetIndex, 0); /* All from CPU */
	SetVideoGraphics(Bit_MaskIndex, 0xFF); /* All bits */
	SetVideoGraphics(Graphics_ModeIndex, 0); /* Write mode 0 */
	SetVideoGraphics(Data_RotateIndex, 0); /* Don't rotate, replace */

	for ( plane = HIGHPLANEMASK, bit = HIGHPLANEINDEX;
		plane ; plane >>= 1, bit-- )
	{
		if ( planes & plane) {
			SetVideoGraphics(Read_Map_SelectIndex, bit);
			SetVideoSequencer(Mask_MapIndex, plane);

		        shift_thin_rect(pWin,x0+w-r1,x1+w-r1,y0,y1,r1,h,alu);
		}
	}
     }

     /* Center */
     SetVideoGraphics(Graphics_ModeIndex, 1); /* Write mode 1 */
     SetVideoSequencer(Mask_MapIndex, planes);

     aligned_blit_center(pWin,x0,x1,y0,y1,w,h,alu);

     if ( l1 ) { /* left edge */
	SetVideoGraphics(Enb_Set_ResetIndex, 0); /* All from CPU */
	SetVideoGraphics(Bit_MaskIndex, 0xFF); /* All bits */
	SetVideoGraphics(Graphics_ModeIndex, 0); /* Write mode 0 */
	SetVideoGraphics(Data_RotateIndex, 0); /* Don't rotate, replace */

	for ( plane = HIGHPLANEMASK, bit = HIGHPLANEINDEX;
		plane ; plane >>= 1, bit-- )
	{
		if ( planes & plane) {
			SetVideoGraphics(Read_Map_SelectIndex, bit);
			SetVideoSequencer(Mask_MapIndex, plane);

        		shift_thin_rect(pWin,x0,x1,y0,y1,(WORDSZ-l1),h,alu);
		}
	}
     }
  } else {
     int l1 = x1 & WMASK, r1 = (x1 + w) & WMASK;

     if ( l1 ) { /* left edge */
	SetVideoGraphics(Enb_Set_ResetIndex, 0); /* All from CPU */
	SetVideoGraphics(Bit_MaskIndex, 0xFF); /* All bits */
	SetVideoGraphics(Graphics_ModeIndex, 0); /* Write mode 0 */
	SetVideoGraphics(Data_RotateIndex, 0); /* Don't rotate, replace */

	for ( plane = HIGHPLANEMASK, bit = HIGHPLANEINDEX;
		plane ; plane >>= 1, bit-- )
	{
		if ( planes & plane) {
			SetVideoGraphics(Read_Map_SelectIndex, bit);
			SetVideoSequencer(Mask_MapIndex, plane);

        		shift_thin_rect(pWin,x0,x1,y0,y1,(WORDSZ-l1),h,alu);
		}
	}
     }

     /* Center */
     SetVideoGraphics(Graphics_ModeIndex, 1); /* Write mode 1 */
     SetVideoSequencer(Mask_MapIndex, planes);

     aligned_blit_center(pWin,x0,x1,y0,y1,w,h,alu);

     if ( r1 ) { /* right edge */
	SetVideoGraphics(Enb_Set_ResetIndex, 0); /* All from CPU */
	SetVideoGraphics(Bit_MaskIndex, 0xFF); /* All bits */
	SetVideoGraphics(Graphics_ModeIndex, 0); /* Write mode 0 */
	SetVideoGraphics(Data_RotateIndex, 0); /* Don't rotate, replace */

	for ( plane = HIGHPLANEMASK, bit = HIGHPLANEINDEX ;
		plane ; plane >>= 1, bit-- )
	{
		if ( planes & plane) {
			SetVideoGraphics(Read_Map_SelectIndex, bit);
			SetVideoSequencer(Mask_MapIndex, plane);

        		shift_thin_rect(pWin,x0+w-r1,x1+w-r1,y0,y1,r1,h,alu);
		}
	}
     }
  }
}

aligned_blit_center(pWin,x0,x1,y0,y1,w,h,alu)
WindowPtr pWin; /* GJA */
int x0;  /* left edge of source */
int x1;  /* left edge of target */
int y0;
int y1;
int w; /* length of source, and of target */
int h;
int alu;
{
  int l0 = x0 & WMASK; /* Left edge of source, as bit */
  int l1 = x1 & WMASK; /* Left edge of target, as bit */
  int r0 = (x0 + w) & WMASK; /* Right edge of source, as bit */
  int r1 = (x1 + w) & WMASK; /* Right edge of target, as bit */
  int L0 = x0 >> WSHIFT; /* Left edge of source, as byte */
  int L1 = x1 >> WSHIFT; /* Left edge of target, as byte */
  int pad;
  int htmp, wtmp; /* Temporaries for indices over height and width */
  volatile unsigned char tmp; /* Temporary result of the shifts */
  int bs;
  int bytecnt;
  
  volatile unsigned char *sp, *dp;

  bs = (x1 - x0) & WMASK;

  if ( l1 ) {
     bytecnt = (w - (WORDSZ - l1) - r1) >> WSHIFT;
     sp = SMEM( ((x0 + (WORDSZ - l1)) >> WSHIFT), y0);
     dp = DMEM( ((x1 + (WORDSZ - l1)) >> WSHIFT), y1);
  } else {
     bytecnt = (w - r1) >> WSHIFT;
     sp = SMEM( (x0 >> WSHIFT), y0);
     dp = DMEM( (x1 >> WSHIFT), y1);
  }

  if ( y1 > y0 ) { /* Move down, start at the bottom */
    if ( x1 > x0 ) { /* Move right, start right */
       pad = - BYTES_PER_LINE(pWin) + bytecnt;
       sp += BYTES_PER_LINE(pWin) * (h - 1) + bytecnt - 1;
       dp += BYTES_PER_LINE(pWin) * (h - 1) + bytecnt - 1;
    } else { /* Move left, start left */
       pad = - BYTES_PER_LINE(pWin) - bytecnt;
       sp += BYTES_PER_LINE(pWin) * (h - 1);
       dp += BYTES_PER_LINE(pWin) * (h - 1);
    }
  } else { /* Move up, start at the top */
    if ( x1 > x0 ) { /* Move right, start right */
       pad = BYTES_PER_LINE(pWin) + bytecnt;
       sp += bytecnt - 1;
       dp += bytecnt - 1;
    } else { /* Move left, start left */
       pad = BYTES_PER_LINE(pWin) - bytecnt;
       sp += 0;
       dp += 0;
    }
  }
  VSETW(dp); VSETR(sp);

  if ( x1 > x0 ) { /* Move right, start right */
      for ( htmp = h ; htmp ; htmp-- ) {
        for ( wtmp = bytecnt ; wtmp ; wtmp-- ) {
          tmp = sp[0];
	  VPUSHR();
	  dp[0] = tmp;
	  VDECW(dp);
	  VPOPR();
          VDECR(sp);
        }
	VPUSHR();
        ADDW(dp,pad);
        VPOPR();
        ADDR(sp,pad);
      } 
  } else { /* x1 <= x0 */ /* Move left, start left */
      for ( htmp = h ; htmp ; htmp-- ) {
        for ( wtmp = bytecnt ; wtmp ; wtmp-- ) {
          tmp = sp[0];
	  VPUSHR();
          dp[0] = tmp;
	  VINCW(dp);
	  VPOPR();
          VINCR(sp);
        }
	VPUSHR();
        ADDW(dp,pad);
        VPOPR();
        ADDR(sp,pad);
      } 
  }
}
