/*                            xmove
 *                            -----
 *             A Pseudoserver For Client Mobility
 *
 *   Copyright (c) 1994         Ethan Solomita
 *
 *   The X Consortium, and any party obtaining a copy of these files from
 *   the X Consortium, directly or indirectly, is granted, free of charge, a
 *   full and unrestricted irrevocable, world-wide, paid up, royalty-free,
 *   nonexclusive right and license to deal in this software and
 *   documentation files (the "Software"), including without limitation the
 *   rights to use, copy, modify, merge, publish, distribute, sublicense,
 *   and/or sell copies of the Software, and to permit persons who receive
 *   copies from any such party to do so.  This license includes without
 *   limitation a license to do the foregoing actions under any patents of
 *   the party supplying this software to the X Consortium.
 */
/*
 * File: common.c
 *
 * Description: Standard header file
 *
 */

#ifndef COMMON_H
#define COMMON_H

#include    <ctype.h>
#include    <sys/types.h>
#include    <stdio.h>

#include    <string.h>

typedef		int				Bool;
typedef		caddr_t				Pointer;
typedef		void				(*VoidCallback)();	/* ptr to void function */
typedef		int				(*IntCallback)();	/* ptr to int function */

#define		Global				/* Global */

#ifndef True
#define		True				1
#define		False				0
#endif

#define		Streq(a,b) 			(strcmp(a,b) == 0)
#define		Strneq(a,b,n)		(strncmp(a,b,n) == 0)
#define		abs(x)				(((x) < 0) ? (-(x)) : (x))
#ifndef MAX
#define		MAX(n1, n2)			(((n1) < (n2)) ? (n2) :	 (n1))
#endif
#ifndef MIN
#define		MIN(n1, n2)			(((n1) > (n2)) ? (n2) :	 (n1))
#endif
#define		Tmalloc(type)		(type *)xmovemalloc(sizeof(type))
#define		Tcalloc(n, type)	(type *)xmovecalloc(n, sizeof(type))
#define		Tfree(ptr)			free((char *)(ptr))

/* optimize ILong/IShort/IByte/IBool/ISetLong/ISetShort/ISetByte/ISetBool.
   Store xmove's native endianism, and if it matches the current client's,
   just copy the image and don't do any conversion. */

extern Bool littleEndian, XmoveLittleEndian;

/*#define ILong(l)  ((littleEndian == XmoveLittleEndian) ? \
		   (*(unsigned long *)(l)) : ilong(l))
#define IShort(s) ((littleEndian == XmoveLittleEndian) ? \
		   (*(unsigned short *)(s)) : ishort(s))
*/
#define IByte(b)  ((unsigned short)*(b))
#define IBool(b)  (*(b) ? True : False)

/*#define ISetLong(buf, l)  ((littleEndian == XmoveLittleEndian) ? \
			   (*(unsigned long *)(buf) = (l)) : isetlong(buf, l))
#define ISetShort(buf, s) ((littleEndian == XmoveLittleEndian) ? \
			   (*(unsigned short *)(buf) = (s)) : isetshort(buf, s))
*/
#define ISetByte(buf, b)  (*(buf) = (b))
#define ISetBool(buf, b)  (*(buf) = (b))
			    


#ifdef	__STDC__
#define		P(args)				args
#else
#define		P(args)				()
#endif

#ifdef SVR4
#define bcopy(src, dst, n) ((void) memmove(dst, src, n))
#define bzero(src, n) ((void) memset(src, 0, n))
#endif

#define RGB2BW(r, g, b) (r*.3 + g*.59 + b*.11)
#include "externs.h"

/* ROUNDUP(): this redefinition requires pad be a power of 2 */
/*            it uses no multiplications or divides.         */

#define ROUNDUP(nbytes, pad) (((nbytes) & ((pad)-1)) ? (((nbytes) & ~((pad)-1)) + (pad)) : (nbytes))
#define ROUNDUP2(nbytes) (((nbytes) & 1) ? ((nbytes) + 1) : (nbytes))
#define ROUNDUP4(nbytes) (((nbytes) & 3) ? (((nbytes) & ~(3)) + 4) : (nbytes))
#define ROUNDUP8(nbytes) (((nbytes) & 7) ? (((nbytes) & ~(7)) + 8) : (nbytes))

/*#define ROUNDUP(nbytes, pad) (((((nbytes) - 1) + (pad)) / (pad)) * (pad))*/
#define XMOVEGetImageFromPixmap(f, sn, s, p, x, y, w, h) \
     XMOVEGetImage(f, sn, s, p, x, y, w, h, 0xFFFFFFFF, XYPixmap)

#endif  /* COMMON_H */
