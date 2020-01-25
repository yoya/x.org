/* $XConsortium: Bt485.h,v 1.1 95/01/26 15:31:41 kaleb Exp $ */
/* $XFree86: xc/programs/Xserver/hw/xfree86/accel/agx/Bt485.h,v 3.1 1994/09/07 15:47:44 dawes Exp $ */
/*
 * Copyright 1993 by David Wexelblat <dwex@goblin.org>
 * Copyright 1994 by Henry A. Worth  <haw30@eng.amdahl.com>
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of David Wexelblat not be used in
 * advertising or publicity pertaining to distribution of the software without
 * specific, written prior permission.  David Wexelblat makes no representations
 * about the suitability of this software for any purpose.  It is provided
 * "as is" without express or implied warranty.
 *
 * DAVID WEXELBLAT AND HENRY A. WORTH DISCLAIM ALL WARRANTIES WITH 
 * REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL THE AUTHORS BE 
 * LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR 
 * ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, 
 * WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 * Modified for the AGX by Henry A. Worth
 *
 */

#include "compiler.h"
#include <X11/Xfuncproto.h>

/* Directly Accessable RamDac Registers */

#define BT485_WRITE_ADDR		0x00
#define BT485_RAMDAC_DATA		0x01	
#define BT485_PIXEL_MASK		0x02
#define BT485_READ_ADDR			0x03
#define BT485_CURS_WR_ADDR		0x04
#define BT485_CURS_DATA			0x05
#define BT485_COMMAND_REG_0		0x06
#define BT485_CURS_RD_ADDR		0x07
#define BT485_COMMAND_REG_1		0x08
#define BT485_COMMAND_REG_2		0x09
#define BT485_STATUS_REG		0x0A
#define BT485_CURS_RAM_DATA		0x0B
#define BT485_CURS_X_LOW		0x0C
#define BT485_CURS_X_HIGH		0x0D
#define BT485_CURS_Y_LOW		0x0E
#define BT485_CURS_Y_HIGH		0x0F

/* Indirectly Accessable RamDac Registers */

#define BT485_COMMAND_REG_3		0x01

_XFUNCPROTOBEGIN

extern void xf86OutBt485IndReg(
#if NeedFunctionPrototypes
	unsigned char,
	unsigned char,
	unsigned char
#endif
);

extern unsigned char xf86InBt485IndReg(
#if NeedFunctionPrototypes
	unsigned char
#endif
);

extern unsigned char xf86InBt485StatReg(
#if NeedFunctionPrototypes
	void
#endif
);

extern void xf86Bt485HWSave(
#if NeedFunctionPrototypes
        union xf86RamDacSave *
#endif
);

extern void xf86Bt485HWRestore(
#if NeedFunctionPrototypes
        union xf86RamDacSave *
#endif
);

extern void xf86Bt485Init(
#if NeedFunctionPrototypes
        void
#endif
);

_XFUNCPROTOEND
