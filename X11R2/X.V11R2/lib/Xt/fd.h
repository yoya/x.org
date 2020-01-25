/*
* $Header: fd.h,v 1.7 88/02/26 12:50:11 swick Exp $
*/

/***********************************************************
Copyright 1987, 1988 by Digital Equipment Corporation, Maynard, Massachusetts,
and the Massachusetts Institute of Technology, Cambridge, Massachusetts.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the names of Digital or MIT not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.  

DIGITAL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/

#ifndef __fd_set
#define __fd_set
#ifndef NBBY
#define	NBBY	8		/* number of bits in a byte */
#endif
/*
 * Select uses bit masks of file descriptors in longs.
 * These macros manipulate such bit fields (the filesystem macros use chars).
 * FD_SETSIZE may be defined by the user, but the default here
 * should be >= NOFILE (param.h).
 */
#ifndef	FD_SETSIZE
#define	FD_SETSIZE	256
#endif

typedef long	xfd_mask;
#define NXFDBITS (sizeof(xfd_mask) * NBBY)	/* bits per mask */
#define	countfdmasks(x, y) (((x)+((y)-1))/(y))

typedef	struct xfd_set {
	xfd_mask fds_bits[countfdmasks(FD_SETSIZE, NXFDBITS)];
} xfd_set;

#define	XFD_SET(n, p)	((p)->fds_bits[(n)/NXFDBITS] |= (1 << ((n) % NXFDBITS)))
#define	XFD_CLR(n, p)	((p)->fds_bits[(n)/NXFDBITS] &= ~(1 << ((n) % NXFDBITS)))
#define	XFD_ISSET(n, p)	((p)->fds_bits[(n)/NXFDBITS] & (1 << ((n) % NXFDBITS)))

#endif
