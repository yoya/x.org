/*
 *	$Source: /u1/Sx.new/code/RCS/byte.h,v $
 *	$Header: byte.h,v 1.1 86/12/03 16:11:35 swick Exp $
 */

/*
 * byte.h --
 *
 *	This is a dummy version of byte.h, which uses standard Unix
 *	routines instead of the Sprite library routines.  It's here
 *	so Sx can be used independently of the Sprite library.
 *
 * Copyright (C) 1986 Regents of the University of California.
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appears in all copies.  The University of California
 * makes no representations about the suitability of this
 * software for any purpose.  It is provided "as is" without
 * express or implied warranty.
 *
 *
 * $Header: byte.h,v 1.1 86/12/03 16:11:35 swick Exp $ SPRITE (Berkeley)
 */

#ifndef _BYTE
#define _BYTE

#define Byte_Copy(numBytes, src, dst)  bcopy(src, dst, numBytes)

#endif _BYTE
