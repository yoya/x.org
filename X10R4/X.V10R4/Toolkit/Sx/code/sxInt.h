/*
 *	$Source: /u1/Sx.new/code/RCS/sxInt.h,v $
 *	$Header: sxInt.h,v 1.1 86/12/03 16:12:24 swick Exp $
 */

/*
 * sxInt.h --
 *
 *	This file contains declarations for things used internally by
 *	several files in the sx module, but not exported outside the
 *	module.
 *
 * Copyright (C) 1986 Regents of the University of California.
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting
 * documentation.  The University of California makes no
 * representations about the suitability of this software for
 * any purpose.  It is provided "as is" without express or
 * implied warranty.
 *
 *
 * $Header: sxInt.h,v 1.1 86/12/03 16:12:24 swick Exp $ SPRITE (Berkeley)
 */

#ifndef _SXINT
#define _SXINT

#ifndef BlackPixel
#include <X/Xlib.h>
#endif

/*
 * The defines below determine the amount of extra space required on
 * each side of a shadowed box, as drawn by SxDrawShadow.  This space
 * includes both the box border and the shadow.
 */

#define SHADOW_BORDER 2
#define SHADOW_WIDTH 6
#define SHADOW_LEFT 2
#define SHADOW_TOP 2
#define SHADOW_RIGHT 8
#define SHADOW_BOTTOM 8

/*
 * Procedures exported by one sx module and used by another:
 */

extern void		SxFlashWait();
extern void		SxDrawShadow();
extern void		SxSubwindowsDestroyed();
extern void		SxWindowCreated();
extern void		SxWindowDestroyed();

#endif _SXINT
