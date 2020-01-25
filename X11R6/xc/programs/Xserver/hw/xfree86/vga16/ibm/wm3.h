/* $XConsortium: wm3.h,v 1.1 94/03/28 21:40:31 dpw Exp $ */
#include "vgaReg.h"

/* Do call in Write Mode 3.
 * We take care of the possibility that two passes are needed.
 */
#define DO_WM3(pgc,call) \
   { int _tp, _fg, _bg, _alu; \
	_fg = pgc->fgPixel; _bg = pgc->bgPixel; \
	_tp = wm3_set_regs(pgc); \
        (call); \
	if ( _tp ) { \
           _alu = pgc->alu; \
	   pgc->alu = GXinvert; \
	   _tp = wm3_set_regs(pgc); \
	   (call); \
           pgc->alu = _alu; \
	} \
	pgc->fgPixel = _fg; pgc->bgPixel = _bg; \
    }

#define WM3_SET_INK(ink) \
    SetVideoGraphics(Set_ResetIndex, ink)

/* GJA -- Move a long word to screen memory.
 * The reads into 'dummy' are here to load the VGA latches.
 * This is a RMW operation except for trivial cases.
 * Notice that we ignore the operation.
 */
#define UPDRW(destp,src) \
	{ volatile char *_dtmp = (volatile char *)(destp); \
	  int _stmp = (src); \
	  volatile int dummy; /* Bit bucket. */ \
	  dummy = *_dtmp; *_dtmp = _stmp; _dtmp++; _stmp >>= 8; \
	  dummy = *_dtmp; *_dtmp = _stmp; _dtmp++; _stmp >>= 8; \
	  dummy = *_dtmp; *_dtmp = _stmp; _dtmp++; _stmp >>= 8; \
	  dummy = *_dtmp; *_dtmp = _stmp; }

#define UPDRWB(destp,src) \
	{ volatile int dummy; /* Bit bucket. */ \
	  dummy = *(destp); *(destp) = (src); }
