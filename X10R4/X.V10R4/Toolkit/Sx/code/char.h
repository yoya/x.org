/*
 *	$Source: /u1/Sx.new/code/RCS/char.h,v $
 *	$Header: char.h,v 1.1 86/12/03 16:11:41 swick Exp $
 */

/*
 * char.h --
 *
 *	This is a dummy version of char.h, which uses standard Unix
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
 * $Header: char.h,v 1.1 86/12/03 16:11:41 swick Exp $ SPRITE (Berkeley)
 */

#ifndef _CHAR
#define _CHAR

#include <ctype.h>

#define Char_IsSpace isspace
#define Char_IsPrint isprint

#endif _CHAR
