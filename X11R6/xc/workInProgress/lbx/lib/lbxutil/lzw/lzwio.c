/*
 * $NCDId: @(#)lzwio.c,v 1.3 1994/01/17 20:59:53 dct Exp $
 *
 * Copyright 1993 Network Computing Devices
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of NCD. not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  NCD. makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * NCD. DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL NCD.
 * BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN 
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Author:  Dale Tonogai, Network Computing Devices
 */

/* $XConsortium: lzwio.c,v 1.4 94/03/08 16:58:45 dpw Exp $ */

#include <X11/Xos.h>
#include <X11/Xfuncs.h>
#include <errno.h>
#ifdef X_NOT_STDC_ENV
extern int errno;
#endif
#include <sys/param.h>
#include <sys/uio.h>
#include "lbxbufstr.h"

#ifndef NULL
#define NULL		0
#endif

#ifndef MIN
#define MIN(_a, _b) ( ((_a) < (_b)) ? (_a) : (_b))
#endif

int
InitLzwBuffer(b, size)
    LzwBufferPtr b;
    int size;
{
    if ((b->bufbase = (char *)Xalloc(size)) == NULL)
	return -1;
    b->bufend = b->bufbase + size;
    b->bufptr = b->bufbase;
    b->bufcnt = 0;
    return 0;
}

void
FreeLzwBuffer(b)
    LzwBufferPtr b;
{
    if (b->bufbase)
	free(b->bufbase);
}

/*
 * Returns:
 *	1 if desired amount of data available in input buffer
 *	0 if eof
 *     -1 if error
 */
int
GetInputPtr(fd, inbuf, reqlen, ppkt)
    int		 fd;
    LzwBufferPtr inbuf;
    int		 reqlen;
    char	 **ppkt;
{
    int		 readbytes;
    int		 gotbytes;

    if (inbuf->bufcnt == 0)
	inbuf->bufptr = inbuf->bufbase;

    if (reqlen <= inbuf->bufcnt) {
	*ppkt = inbuf->bufptr;
	return 1;
    }

    if (reqlen > inbuf->bufend - inbuf->bufptr) {
	memmove(inbuf->bufbase, inbuf->bufptr, inbuf->bufcnt);
	inbuf->bufptr = inbuf->bufbase;
    }
    readbytes = (inbuf->bufend - inbuf->bufptr) - inbuf->bufcnt;
    gotbytes = read(fd, inbuf->bufptr + inbuf->bufcnt, readbytes);
    if (gotbytes > 0) {
	if (reqlen <= (inbuf->bufcnt += gotbytes)) {
	    *ppkt = inbuf->bufptr;
	    return 1;
	}
    }
    else
	return gotbytes;

    errno = EWOULDBLOCK;
    return -1;
}

/*
 * When LZW is started, we may well have read some data off of the
 * wire somewhere.  This sticks those bytes ahead of anything we might
 * read in the future
 */

int
StuffInput(inbuf, pkt, reqlen)
    LzwBufferPtr inbuf;
    char	 *pkt;
    int		 reqlen;
{
    int		 readbytes;
    char	 *last;
    
    last = inbuf->bufptr + inbuf->bufcnt;
    if (reqlen > inbuf->bufend - last)
    {
	memmove(inbuf->bufbase, inbuf->bufptr, inbuf->bufcnt);
	inbuf->bufptr = inbuf->bufbase;
	last = inbuf->bufptr + inbuf->bufcnt;
    }
    readbytes = MIN(reqlen, inbuf->bufend - last);
    memmove(last, pkt, readbytes);
    inbuf->bufcnt += readbytes;
    return readbytes;
}

void
FreeInput(inbuf, len)
    LzwBufferPtr inbuf;
    int		 len;
{
    inbuf->bufptr += len;
    if ((inbuf->bufcnt -= len) == 0)
        inbuf->bufptr = inbuf->bufbase;
}

/*
 * Reserve outlen bytes in the output buffer.
 */
char *
ReserveOutBuf(outbuf, outlen)
    LzwBufferPtr outbuf;
    int		 outlen;
{
    int		 left;

    left = (outbuf->bufend - outbuf->bufptr) - outbuf->bufcnt;
    if (left < outlen)
	return NULL;
    else
	return outbuf->bufptr + outbuf->bufcnt;
}

/*
 * Commit previously reserved space as real output
 */
void
CommitOutBuf(outbuf, outlen)
    LzwBufferPtr outbuf;
    int		 outlen;
{
    outbuf->bufcnt += outlen;
}

/*
 * Write out as much as possible from the output buffer.
 * Returns: >= 0 - amount left in buffer
 *	    <  0 - write error
 */
int
FlushOutBuf(fd, outbuf)
    int		 fd;
    LzwBufferPtr outbuf;
{
    int		 bytes;

    if (outbuf->bufcnt == 0)
	return 0;
    bytes = write(fd, outbuf->bufptr, outbuf->bufcnt);
    if (bytes > 0) {
	outbuf->bufptr += bytes;
	if ((outbuf->bufcnt -= bytes) == 0)
	    outbuf->bufptr = outbuf->bufbase;
	return outbuf->bufcnt;
    }
    else if (bytes == 0) {
	errno = EWOULDBLOCK;
	bytes = -1;
    }
    return bytes;
}

/*
 * Write out as much as possible from the iovec array (no more than
 * two entries allowed).
 * Returns: >= 0 - amount left in iovec[1]
 *	    <  0 - write error
 */
int
FlushIovBuf(fd, iovbuf)
    int		 fd;
    struct iovec *iovbuf;
{
    int		 bytes;
    int		 niov = 2;
    struct iovec *iov = iovbuf;

    if (iov[0].iov_len == 0) {
	++iov;
	--niov;
    }
    bytes = writev(fd, iov, niov);
    if (bytes > 0) {
	int i;
	int len;
	for (i = 0; i < niov; i++) {
	    len = MIN(bytes, iov[i].iov_len);
	    iov[i].iov_len -= len;
	    iov[i].iov_base += len;
	    if ((bytes -= len) == 0)
		break;
	}
	return iovbuf[1].iov_len;
    }
    else if (bytes == 0) {
	errno = EWOULDBLOCK;
	bytes = -1;
    }
    return bytes;
}
