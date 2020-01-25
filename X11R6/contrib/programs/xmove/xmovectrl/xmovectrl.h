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
#define		max(n1, n2)			(((n1) < (n2)) ? (n2) :	 (n1))
#define		min(n1, n2)			(((n1) > (n2)) ? (n2) :	 (n1))
#define		Tmalloc(type)		(type *)xmovemalloc(sizeof(type))
#define		Tcalloc(n, type)	(type *)xmovecalloc(n, sizeof(type))
#define		Tfree(ptr)			free((char *)(ptr))

#ifdef	__STDC__
#define		P(args)				args
#else
#define		P(args)				()
#endif

#ifdef SVR4
#define bcopy(src, dst, n) ((void) memmove(dst, src, n))
#define bzero(src, n) ((void) memset(src, 0, n))
#endif

#include "externs.h"


Global int pad P((long n ));
Global void ISetLong P((unsigned char buf [], unsigned long value));
Global void ISetShort P((unsigned char buf [], unsigned short value));
Global void ISetByte P((unsigned char buf [], unsigned char value));
Global void ISetBool P((unsigned char buf [], Bool value));
Global unsigned long ILong P((unsigned char buf []));
Global unsigned short IShort P((unsigned char buf []));
Global unsigned short IByte P((unsigned char buf []));
Global Bool IBool P((unsigned char buf []));
