/*
 *	$Source: /u1/Sx.new/code/RCS/mem.h,v $
 *	$Header: mem.h,v 1.1 86/12/03 16:11:59 swick Exp $
 */

/*
 * mem.h --
 *
 *	This is a dummy version of mem.h, which uses standard Unix
 *	routines instead of the Sprite library routines.  It's here
 *	so Sx can be used independently of the Sprite library.
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
 * $Header: mem.h,v 1.1 86/12/03 16:11:59 swick Exp $ SPRITE (Berkeley)
 */

#ifndef _MEM
#define _MEM

#ifndef _SPRITE
#include "sprite.h"
#endif _SPRITE

extern char *malloc();

#define Mem_Free free
#define Mem_Alloc malloc

#endif _MEM
