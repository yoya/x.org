/* $XConsortium: lbxlzw.h,v 1.4 94/12/01 20:07:54 mor Exp $ */
/*
 * $NCDId: @(#)lzw.h,v 1.2 1994/01/17 20:59:50 dct Exp $
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

#ifndef _LZW_H_
#define _LZW_H_

#define LZW_STRCOMP_OPT		"lzw"
#define LZW_STRCOMP_OPT_LEN	3

#define LZW_PACKET_HDRLEN	2
#define LZW_MAX_DATALEN		0xfff
#define LZW_MAX_PLAIN		270
#define LZW_MAX_OUTLEN		(LZW_MAX_PLAIN << 1)

#define LZW_COMPRESS_FLAG	0x80
#define LZW_DATALEN_MASK	0x0f

#define LZW_PUT_PKTHDR(p, len, compflag) \
    { \
	(p)[0] = ((unsigned)(len)) >> 8 | ((compflag) ? LZW_COMPRESS_FLAG : 0);\
	(p)[1] = (len) & 0xff; \
    }

#define LZW_GET_DATALEN(p) \
	((((unsigned)((p)[0] & LZW_DATALEN_MASK)) << 8) | (unsigned)(p)[1])

#define LZW_COMPRESSED(p) ((p)[0] & LZW_COMPRESS_FLAG)

#endif /* _LZW_H_ */
