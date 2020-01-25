/*

Copyright (c) 1986, 1987 by Hewlett-Packard Company
Copyright (c) 1986, 1987 by the Massachusetts Institute of Technology

Permission to use, copy, modify, and distribute this
software and its documentation for any purpose and without
fee is hereby granted, provided that the above copyright
notice appear in all copies and that both that copyright
notice and this permission notice appear in supporting
documentation, and that the name of M.I.T. not be used in
advertising or publicity pertaining to distribution of the
software without specific, written prior permission.

HEWLETT-PACKARD MAKES NO WARRANTY OF ANY KIND WITH REGARD
TO THIS SOFWARE, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
PURPOSE.  Hewlett-Packard shall not be liable for errors 
contained herein or direct, indirect, special, incidental or 
consequential damages in connection with the furnishing, 
performance, or use of this material.

This software is not subject to any license of the American
Telephone and Telegraph Company or of the Regents of the
University of California.

*/

#include	"X.h"
#include        "Xmd.h"
#include	"keysym.h"
#include        "hpKeyDef.h"
#include	"sun.h"
#include	"input.h"

/* This file was composed from the X10 hil_keymap.h by
 * Jack Palevich, HP-Labs
 */

static KeySym USASCIIMap[] = {

 /* code values in comments at line end are actual value reported on HIL.
    REMEMBER, there is an offset of MIN_KEYCODE applied to this table! */
	/* Extend Char Right -- a.k.a. Kanji? */
	XK_Meta_R,	NoSymbol,		/* 0x02 */
	XK_Meta_L,	NoSymbol,		/* 0x03 */
	XK_Shift_R,	NoSymbol,		/* 0x4 */
	XK_Shift_L,	NoSymbol,		/* 0x5 */
	XK_Control_L,   NoSymbol,		/* 0x6 */
	XK_Break,	XK_Reset,		/* 0x7 */
	XK_KP_4,	NoSymbol,		/* 0x8 */
	XK_KP_8,	NoSymbol,		/* 0x9 */
	XK_KP_5,	NoSymbol,		/* 0xa */
	XK_KP_9,	NoSymbol,		/* 0xb */
	XK_KP_6,	NoSymbol,		/* 0xc */
	XK_KP_7,	NoSymbol,		/* 0xd */
	XK_KP_Separator,	NoSymbol,	/* 0xe */
	XK_KP_Enter,	NoSymbol,		/* 0xf */
	XK_KP_1,	NoSymbol,		/* 0x10 */
	XK_KP_Divide,	NoSymbol,		/* 0x11 */
	XK_KP_2,	NoSymbol,		/* 0x12 */
	XK_KP_Add,	NoSymbol,		/* 0x13 */
	XK_KP_3,	NoSymbol,		/* 0x14 */
	XK_KP_Multiply,	NoSymbol,		/* 0x15 */
	XK_KP_0,	NoSymbol,		/* 0x16 */
	XK_KP_Subtract,	NoSymbol,		/* 0x17 */
	XK_B,		NoSymbol,		/* 0x18 */
	XK_V,		NoSymbol,		/* 0x19 */
	XK_C,		NoSymbol,		/* 0x1a */
	XK_X,		NoSymbol,		/* 0x1b */
	XK_Z,		NoSymbol,		/* 0x1c */
/* Was Kanji Left.... */
	XK_Kanji,	NoSymbol,		/* 0x1d */
	NoSymbol,	NoSymbol,		/* 0x1e */
	XK_Escape,	XK_Delete,		/* 0x1f */
	XK_KP_6,	NoSymbol,		/* 0x20 */
	XK_KP_F2,	NoSymbol,		/* 0x21 */
	XK_KP_3,	NoSymbol,		/* 0x22 */
	XK_KP_F3,	NoSymbol,		/* 0x23 */
	XK_KP_Decimal,	NoSymbol,		/* 0x24 */
	XK_KP_F1,	NoSymbol,		/* 0x25 */
	XK_KP_Tab,	XK_KP_BackTab,		/* 0x26 */
	XK_KP_F4,	NoSymbol,		/* 0x27 */
	XK_H,		NoSymbol,		/* 0x28 */
	XK_G,		NoSymbol,		/* 0x29 */
	XK_F,		NoSymbol,		/* 0x2a */
	XK_D,		NoSymbol,		/* 0x2b */
	XK_S,		NoSymbol,		/* 0x2c */
	XK_A,		NoSymbol,		/* 0x2d */
	NoSymbol,	NoSymbol,		/* 0x2e */
	XK_Caps_Lock,	NoSymbol,		/* 0x2f */
	XK_U,		NoSymbol,		/* 0x30 */
	XK_Y,		NoSymbol,		/* 0x31 */
	XK_T,		NoSymbol,		/* 0x32 */
	XK_R,		NoSymbol,		/* 0x33 */
	XK_E,		NoSymbol,		/* 0x34 */
	XK_W,		NoSymbol,		/* 0x35 */
	XK_Q,		NoSymbol,		/* 0x36 */
	XK_Tab,		XK_BackTab,		/* 0x37 */
	XK_7,		XK_ampersand,		/* 0x38 */
	XK_6,		XK_asciicircum,		/* 0x39 */
	XK_5,		XK_percent,		/* 0x3a */
	XK_4,		XK_dollar,		/* 0x3b */
	XK_3,		XK_numbersign,		/* 0x3c */
	XK_2,		XK_at,			/* 0x3d */
	XK_1,		XK_exclam,		/* 0x3e */
	XK_quoteleft,	XK_asciitilde,		/* 0x3f */
/* Was Mouse-L */
	NoSymbol,	NoSymbol,		/* 0x40 */
/* Was Mouse-M */
	NoSymbol,	NoSymbol,		/* 0x41 */
/* Was Mouse-R */
	NoSymbol,	NoSymbol,		/* 0x42 */
/* Was 4 button puck */
	NoSymbol,	NoSymbol,		/* 0x43 */
	NoSymbol,	NoSymbol,		/* 0x44 */
	NoSymbol,	NoSymbol,		/* 0x45 */
	NoSymbol,	NoSymbol,		/* 0x46 */
	NoSymbol,	NoSymbol,		/* 0x47 */
	XK_Menu,	NoSymbol,		/* 0x48 */
	XK_F4,		NoSymbol,		/* 0x49 */
	XK_F3,		NoSymbol,		/* 0x4a */
	XK_F2,		NoSymbol,		/* 0x4b */
	XK_F1,		NoSymbol,		/* 0x4c */
	NoSymbol,	NoSymbol,		/* 0x4d */
/* Was 'Stop' */
	XK_Cancel,	NoSymbol,		/* 0x4e */
/* Was 'Enter' */
	XK_Execute,	XK_Print,		/* 0x4f */
	XK_System,	XK_User,		/* 0x50 */
	XK_F5,		NoSymbol,		/* 0x51 */
	XK_F6,		NoSymbol,		/* 0x52 */
	XK_F7,		NoSymbol,		/* 0x53 */
	XK_F8,		NoSymbol,		/* 0x54 */
	NoSymbol,	NoSymbol,		/* 0x55 */
	XK_ClearLine,	NoSymbol,		/* 0x56 */
/* Was 'Clear Display' */
	XK_Clear,	NoSymbol,		/* 0x57 */
	XK_8,		XK_asterisk,		/* 0x58 */
	XK_9,		XK_parenleft,		/* 0x59 */
	XK_0,		XK_parenright,		/* 0x5a */
	XK_minus,	XK_underscore,		/* 0x5b */
	XK_equal,	XK_plus,		/* 0x5c */
	XK_BackSpace,	NoSymbol,		/* 0x5d */
	XK_InsertLine,	NoSymbol,		/* 0x5e */
	XK_DeleteLine,	NoSymbol,		/* 0x5f */
	
	XK_I,		NoSymbol,		/* 0x60 */
	XK_O,		NoSymbol,		/* 0x61 */
	XK_P,		NoSymbol,		/* 0x62 */
	XK_bracketleft,	XK_braceleft,		/* 0x63 */
	XK_bracketright,	XK_braceright,	/* 0x64 */
	XK_backslash,	XK_bar,			/* 0x65 */

	/* HP special, might also be Insert */
	XK_InsertChar,	NoSymbol,		/* 0x66 */
	XK_DeleteChar,	NoSymbol,		/* 0x67 */
	XK_J,		NoSymbol,		/* 0x68 */
	XK_K,		NoSymbol,		/* 0x69 */
	XK_L,		NoSymbol,		/* 0x6a */
	XK_semicolon,	XK_colon,		/* 0x6b */
	XK_quoteright,	XK_quotedbl,		/* 0x6c */
	XK_Return,	NoSymbol,		/* 0x6d */
	XK_Home,	NoSymbol,		/* 0x6e */
	/* Prev */
	XK_Prior,	NoSymbol,		/* 0x6f */

	XK_M,		NoSymbol,		/* 0x70 */
	XK_comma,	XK_less,		/* 0x71 */
	XK_period,	XK_greater,		/* 0x72 */
	XK_slash,	XK_question,		/* 0x73 */
	NoSymbol,	NoSymbol,		/* 0x74 */
	XK_Select,	NoSymbol,		/* 0x75 */
	NoSymbol,	NoSymbol,		/* 0x76 */
	XK_Next,	NoSymbol,		/* 0x77 */
	XK_N,		NoSymbol,		/* 0x78 */
	/* "Space, the final frontier..." */
	XK_space,	NoSymbol,		/* 0x79 */
	NoSymbol,	NoSymbol,		/* 0x7a */
	/* Kanji Right */
	XK_Kanji,	NoSymbol,		/* 0x7b */

	XK_Left,	NoSymbol,		/* 0x7c */
	XK_Down,	NoSymbol,		/* 0x7d */
	XK_Up,		NoSymbol,		/* 0x7e */
	XK_Right,	NoSymbol		/* 0x7f */
};


KeySymsRec hpKeySyms[] = {
    /*	map	     minKeyCode              maxKC           width */
    USASCIIMap, (MIN_KEYCODE + 0x02), (MIN_KEYCODE + 0x80),	2,
};

#define	cT	(ControlMask)
#define	sH	(ShiftMask)
#define	lK	(LockMask)
#define	mT	(Mod1Mask)

static CARD8 type0modmap[MAP_LENGTH] = {
/* shift table values up by 8. This offset is necessary to reserve codes
   for mouse buttons. Note last 8 entries of table are commented out to
   preserve length of table.
   Note: '#define MIN_KEYCODE 8' in ddx/hp/sun.h */
    0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  mT, mT, sH, sH, cT, 0,  0,  0,  0,  0,  0,  0,  0,  0, /* 00-0f */
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* 10-1f */
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  lK,/* 20-2f */
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* 30-3f */
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* 40-4f */
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* 50-5f */
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* 60-6f */
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* 70-7f */
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* 80-8f */
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* 90-9f */
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* a0-af */
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* b0-bf */
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* c0-cf */
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* d0-df */
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* e0-ef */
    0,  0,  0,  0,  0,  0,  0,  0,/*0,  0,  0,  0,  0,  0,  0,  0, /* f0-ff */
};

CARD8 *hpMapRec[] = {
      type0modmap,
};






