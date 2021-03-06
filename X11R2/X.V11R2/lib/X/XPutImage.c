#include "copyright.h"

/* $Header: XPutImage.c,v 11.43 88/02/14 10:34:06 rws Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include <stdio.h>
#include "Xlibint.h"
#include <errno.h>

/* assumes pad is a power of 2 */
#define ROUNDUP(nbytes, pad) (((nbytes) + ((pad) - 1)) & ~(long)((pad) - 1))

/* this is used elsewhere */
unsigned char _reverse_byte[0x100] = {
	0x00, 0x80, 0x40, 0xc0, 0x20, 0xa0, 0x60, 0xe0,
	0x10, 0x90, 0x50, 0xd0, 0x30, 0xb0, 0x70, 0xf0,
	0x08, 0x88, 0x48, 0xc8, 0x28, 0xa8, 0x68, 0xe8,
	0x18, 0x98, 0x58, 0xd8, 0x38, 0xb8, 0x78, 0xf8,
	0x04, 0x84, 0x44, 0xc4, 0x24, 0xa4, 0x64, 0xe4,
	0x14, 0x94, 0x54, 0xd4, 0x34, 0xb4, 0x74, 0xf4,
	0x0c, 0x8c, 0x4c, 0xcc, 0x2c, 0xac, 0x6c, 0xec,
	0x1c, 0x9c, 0x5c, 0xdc, 0x3c, 0xbc, 0x7c, 0xfc,
	0x02, 0x82, 0x42, 0xc2, 0x22, 0xa2, 0x62, 0xe2,
	0x12, 0x92, 0x52, 0xd2, 0x32, 0xb2, 0x72, 0xf2,
	0x0a, 0x8a, 0x4a, 0xca, 0x2a, 0xaa, 0x6a, 0xea,
	0x1a, 0x9a, 0x5a, 0xda, 0x3a, 0xba, 0x7a, 0xfa,
	0x06, 0x86, 0x46, 0xc6, 0x26, 0xa6, 0x66, 0xe6,
	0x16, 0x96, 0x56, 0xd6, 0x36, 0xb6, 0x76, 0xf6,
	0x0e, 0x8e, 0x4e, 0xce, 0x2e, 0xae, 0x6e, 0xee,
	0x1e, 0x9e, 0x5e, 0xde, 0x3e, 0xbe, 0x7e, 0xfe,
	0x01, 0x81, 0x41, 0xc1, 0x21, 0xa1, 0x61, 0xe1,
	0x11, 0x91, 0x51, 0xd1, 0x31, 0xb1, 0x71, 0xf1,
	0x09, 0x89, 0x49, 0xc9, 0x29, 0xa9, 0x69, 0xe9,
	0x19, 0x99, 0x59, 0xd9, 0x39, 0xb9, 0x79, 0xf9,
	0x05, 0x85, 0x45, 0xc5, 0x25, 0xa5, 0x65, 0xe5,
	0x15, 0x95, 0x55, 0xd5, 0x35, 0xb5, 0x75, 0xf5,
	0x0d, 0x8d, 0x4d, 0xcd, 0x2d, 0xad, 0x6d, 0xed,
	0x1d, 0x9d, 0x5d, 0xdd, 0x3d, 0xbd, 0x7d, 0xfd,
	0x03, 0x83, 0x43, 0xc3, 0x23, 0xa3, 0x63, 0xe3,
	0x13, 0x93, 0x53, 0xd3, 0x33, 0xb3, 0x73, 0xf3,
	0x0b, 0x8b, 0x4b, 0xcb, 0x2b, 0xab, 0x6b, 0xeb,
	0x1b, 0x9b, 0x5b, 0xdb, 0x3b, 0xbb, 0x7b, 0xfb,
	0x07, 0x87, 0x47, 0xc7, 0x27, 0xa7, 0x67, 0xe7,
	0x17, 0x97, 0x57, 0xd7, 0x37, 0xb7, 0x77, 0xf7,
	0x0f, 0x8f, 0x4f, 0xcf, 0x2f, 0xaf, 0x6f, 0xef,
	0x1f, 0x9f, 0x5f, 0xdf, 0x3f, 0xbf, 0x7f, 0xff
};

/* XXX the following functions are declared int instead of void because various
 * compilers and lints complain about later intialization of SwapFunc and/or
 * (swapfunc == NoSwap) when void is used.
 */

static int
NoSwap (src, dest, srclen, srcinc, destinc, height)
    register unsigned char *src, *dest;
    long srclen, srcinc, destinc;
    unsigned int height;
{
    long h = height;

    if (srcinc == destinc)
	bcopy((char *)src, (char *)dest, (int)(srcinc * (h - 1) + srclen));
    else
	for (; --h >= 0; src += srcinc, dest += destinc)
	    bcopy((char *)src, (char *)dest, (int)srclen);
}

static int
SwapTwoBytes (src, dest, srclen, srcinc, destinc, height)
    register unsigned char *src, *dest;
    long srclen, srcinc, destinc;
    unsigned int height;
{
    long length = ROUNDUP(srclen, 2);
    register long h, n;

    srcinc -= length;
    destinc -= length;
    for (h = height; --h >= 0; src += srcinc, dest += destinc) {
	if ((h == 0) && (srclen != length)) {
	    length -= 2;
	    *(dest + length + 1) = *(src + length);
	}
	for (n = length; n > 0; n -= 2, src += 2) {
	    *dest++ = *(src + 1);
	    *dest++ = *src;
	}
    }
}

static int
SwapThreeBytes (src, dest, srclen, srcinc, destinc, height)
    register unsigned char *src, *dest;
    long srclen, srcinc, destinc;
    unsigned int height;
{
    long length = ((srclen + 2) / 3) * 3;
    register long h, n;

    srcinc -= length;
    destinc -= length;
    for (h = height; --h >= 0; src += srcinc, dest += destinc) {
	if ((h == 0) && (srclen != length)) {
	    length -= 3;
	    if ((srclen - length) == 2)
		*(dest + length + 1) = *(src + length + 1);
	    *(dest + length + 2) = *(src + length);
	}
	for (n = length; n > 0; n -= 3, src += 3) {
	    *dest++ = *(src + 2);
	    *dest++ = *(src + 1);
	    *dest++ = *src;
	}
    }
}

static int
SwapFourBytes (src, dest, srclen, srcinc, destinc, height)
    register unsigned char *src, *dest;
    long srclen, srcinc, destinc;
    unsigned int height;
{
    long length = ROUNDUP(srclen, 4);
    register long h, n;

    srcinc -= length;
    destinc -= length;
    for (h = height; --h >= 0; src += srcinc, dest += destinc) {
	if ((h == 0) && (srclen != length)) {
	    length -= 4;
	    if ((srclen - length) == 3)
		*(dest + length + 1) = *(src + length + 2);
	    if (srclen & 2)
		*(dest + length + 2) = *(src + length + 1);
	    *(dest + length + 3) = *(src + length);
	}
	for (n = length; n > 0; n -= 4, src += 4) {
	    *dest++ = *(src + 3);
	    *dest++ = *(src + 2);
	    *dest++ = *(src + 1);
	    *dest++ = *src;
	}
    }
}

static int
SwapWords (src, dest, srclen, srcinc, destinc, height)
    register unsigned char *src, *dest;
    long srclen, srcinc, destinc;
    unsigned int height;
{
    long length = ROUNDUP(srclen, 4);
    register long h, n;

    srcinc -= length;
    destinc -= length;
    for (h = height; --h >= 0; src += srcinc, dest += destinc) {
	if ((h == 0) && (srclen != length)) {
	    length -= 4;
	    if ((srclen - length) == 3)
		*(dest + length) = *(src + length + 2);
	    if (srclen & 2)
		*(dest + length + 3) = *(src + length + 1);
	    *(dest + length + 2) = *(src + length);
	}
	for (n = length; n > 0; n -= 4, src += 2) {
	    *dest++ = *(src + 2);
	    *dest++ = *(src + 3);
	    *dest++ = *src++;
	    *dest++ = *src++;
	}
    }
}

/* XXX do this with a table someday, ya think? */
static int
SwapNibbles (src, dest, srclen, srcinc, destinc, height)
    register unsigned char *src, *dest;
    long srclen, srcinc, destinc;
    unsigned int height;
{
    register long h, n;
    register unsigned char c;

    srcinc -= srclen;
    destinc -= srclen;
    for (h = height; --h >= 0; src += srcinc, dest += destinc)
	for (n = srclen; --n >= 0; ) {
	    c = *src++;
	    *dest++ = ((c & 0xf) << 4) | ((c & 0xf0) >> 4);
	}
}

static int
SwapBits (src, dest, srclen, srcinc, destinc, height)
    register unsigned char *src, *dest;
    long srclen, srcinc, destinc;
    unsigned int height;
{
    register long h, n;
    register unsigned char *rev = _reverse_byte;

    srcinc -= srclen;
    destinc -= srclen;
    for (h = height; --h >= 0; src += srcinc, dest += destinc)
	for (n = srclen; --n >= 0; )
	    *dest++ = rev[*src++];
}

static int
SwapBitsAndTwoBytes (src, dest, srclen, srcinc, destinc, height)
    register unsigned char *src, *dest;
    long srclen, srcinc, destinc;
    unsigned int height;
{
    long length = ROUNDUP(srclen, 2);
    register long h, n;
    register unsigned char *rev = _reverse_byte;

    srcinc -= length;
    destinc -= length;
    for (h = height; --h >= 0; src += srcinc, dest += destinc) {
	if ((h == 0) && (srclen != length)) {
	    length -= 2;
	    *(dest + length + 1) = rev[*(src + length)];
	}
	for (n = length; n > 0; n -= 2, src += 2) {
	    *dest++ = rev[*(src + 1)];
	    *dest++ = rev[*src];
	}
    }
}

static int
SwapBitsAndFourBytes (src, dest, srclen, srcinc, destinc, height)
    register unsigned char *src, *dest;
    long srclen, srcinc, destinc;
    unsigned int height;
{
    long length = ROUNDUP(srclen, 4);
    register long h, n;
    register unsigned char *rev = _reverse_byte;

    srcinc -= length;
    destinc -= length;
    for (h = height; --h >= 0; src += srcinc, dest += destinc) {
	if ((h == 0) && (srclen != length)) {
	    length -= 4;
	    if ((srclen - length) == 3)
		*(dest + length + 1) = rev[*(src + length + 2)];
	    if (srclen & 2)
		*(dest + length + 2) = rev[*(src + length + 1)];
	    *(dest + length + 3) = rev[*(src + length)];
	}
	for (n = length; n > 0; n -= 4, src += 4) {
	    *dest++ = rev[*(src + 3)];
	    *dest++ = rev[*(src + 2)];
	    *dest++ = rev[*(src + 1)];
	    *dest++ = rev[*src];
	}
    }
}

static int
SwapBitsAndWords (src, dest, srclen, srcinc, destinc, height)
    register unsigned char *src, *dest;
    long srclen, srcinc, destinc;
    unsigned int height;
{
    long length = ROUNDUP(srclen, 4);
    register long h, n;
    register unsigned char *rev = _reverse_byte;

    srcinc -= length;
    destinc -= length;
    for (h = height; --h >= 0; src += srcinc, dest += destinc) {
	if ((h == 0) && (srclen != length)) {
	    length -= 4;
	    if ((srclen - length) == 3)
		*(dest + length) = rev[*(src + length + 2)];
	    if (srclen & 2)
		*(dest + length + 3) = rev[*(src + length + 1)];
	    *(dest + length + 2) = rev[*(src + length)];
	}
	for (n = length; n > 0; n -= 4, src += 2) {
	    *dest++ = rev[*(src + 2)];
	    *dest++ = rev[*(src + 3)];
	    *dest++ = rev[*src++];
	    *dest++ = rev[*src++];
	}
    }
}

/*

The following table gives the bit ordering within bytes (when accessed
sequentially) for a scanline containing 32 bits, with bits numbered 0 to
31, where bit 0 should be leftmost on the display.  For a given byte
labelled A-B, A is for the most significant bit of the byte, and B is
for the least significant bit.

legend:
	1   scanline-unit = 8
	2   scanline-unit = 16
	4   scanline-unit = 32
	M   byte-order = MostSignificant
	L   byte-order = LeastSignificant
	m   bit-order = MostSignificant
	l   bit-order = LeastSignificant


format	ordering

1Mm	00-07 08-15 16-23 24-31
2Mm	00-07 08-15 16-23 24-31
4Mm	00-07 08-15 16-23 24-31
1Ml	07-00 15-08 23-16 31-24
2Ml	15-08 07-00 31-24 23-16
4Ml	31-24 23-16 15-08 07-00
1Lm	00-07 08-15 16-23 24-31
2Lm	08-15 00-07 24-31 16-23
4Lm	24-31 16-23 08-15 00-07
1Ll	07-00 15-08 23-16 31-24
2Ll	07-00 15-08 23-16 31-24
4Ll	07-00 15-08 23-16 31-24


The following table gives the required conversion between any two
formats.  It is based strictly on the table above.  If you believe one,
you should believe the other.

legend:
	n   no changes
	s   reverse 8-bit units within 16-bit units
	l   reverse 8-bit units within 32-bit units
	w   reverse 16-bit units within 32-bit units
	R   reverse bits within 8-bit units
	S   s+R
	L   l+R
	W   w+R

*/

static int (*(SwapFunction[12][12]))() = {
#define n NoSwap,
#define s SwapTwoBytes,
#define l SwapFourBytes,
#define w SwapWords,
#define R SwapBits,
#define S SwapBitsAndTwoBytes,
#define L SwapBitsAndFourBytes,
#define W SwapBitsAndWords,

/*         1Mm 2Mm 4Mm 1Ml 2Ml 4Ml 1Lm 2Lm 4Lm 1Ll 2Ll 4Ll   */
/* 1Mm */ { n   n   n   R   S   L   n   s   l   R   R   R },
/* 2Mm */ { n   n   n   R   S   L   n   s   l   R   R   R },
/* 4Mm */ { n   n   n   R   S   L   n   s   l   R   R   R },
/* 1Ml */ { R   R   R   n   s   l   R   S   L   n   n   n },
/* 2Ml */ { S   S   S   s   n   w   S   R   W   s   s   s },
/* 4Ml */ { L   L   L   l   w   n   L   W   R   l   l   l },
/* 1Lm */ { n   n   n   R   S   L   n   s   l   R   R   R },
/* 2Lm */ { s   s   s   S   R   W   s   n   w   S   S   S },
/* 4Lm */ { l   l   l   L   W   R   l   w   n   L   L   L },
/* 1Ll */ { R   R   R   n   s   l   R   S   L   n   n   n },
/* 2Ll */ { R   R   R   n   s   l   R   S   L   n   n   n },
/* 4Ll */ { R   R   R   n   s   l   R   S   L   n   n   n },

#undef n
#undef s
#undef l
#undef w
#undef R
#undef S
#undef L
#undef W

};

/*
 * This macro creates a value from 0 to 11 suitable for indexing
 * into the table above.
 */
#define	ComposeIndex(bitmap_unit, bitmap_bit_order, byte_order)			\
		(((bitmap_unit == 32) ? 2 : ((bitmap_unit == 16) ? 1 : 0))	\
		     + (((bitmap_bit_order == MSBFirst) ? 0 : 3)		\
		     + ((byte_order == MSBFirst) ? 0 : 6)))

static void
SendXYImage(dpy, req, image, req_xoffset, req_yoffset)
    register Display *dpy;
    register xPutImageReq *req;
    register XImage *image;
    int req_xoffset, req_yoffset;
{
    register int j;
    long total_xoffset, bytes_per_src, bytes_per_dest, length;
    long bytes_per_line, bytes_per_src_plane, bytes_per_dest_plane;
    char *src, *dest, *buf;
    char *extra = (char *)NULL;
    register int (*swapfunc)();

    total_xoffset = image->xoffset + req_xoffset;
    req->leftPad = total_xoffset & (dpy->bitmap_unit - 1);
    total_xoffset = (total_xoffset - req->leftPad) >> 3;
    /* The protocol requires left-pad of zero on all ZPixmap, even
     * though the 1-bit case is identical to bitmap format.  This is a
     * bug in the protocol, caused because 1-bit ZPixmap was added late
     * in the game.  Hairy shifting code compensation isn't worth it,
     * just use XYPixmap format instead.
     */
    if ((req->leftPad != 0) && (req->format == ZPixmap))
	req->format = XYPixmap;
    bytes_per_dest = ROUNDUP((long)req->width + req->leftPad,
			     dpy->bitmap_pad) >> 3;
    bytes_per_dest_plane = bytes_per_dest * req->height;
    length = bytes_per_dest_plane * image->depth;
    req->length += (length + 3) >> 2;

    swapfunc = SwapFunction[ComposeIndex(image->bitmap_unit,
					 image->bitmap_bit_order,
					 image->byte_order)]
			   [ComposeIndex(dpy->bitmap_unit,
					 dpy->bitmap_bit_order,
					 dpy->byte_order)];

    src = image->data + (image->bytes_per_line * req_yoffset) + total_xoffset;

    /* when total_xoffset > 0, we have to worry about stepping off the
     * end of image->data.
     */
    if ((swapfunc == NoSwap) &&
	(image->bytes_per_line == bytes_per_dest) &&
	(((total_xoffset == 0) &&
	  ((image->depth == 1) || (image->height == req->height))) ||
	 ((image->depth == 1) &&
	  ((req_yoffset + req->height) < image->height)))) {
	Data(dpy, src, length);
	return;
    }

    length = ROUNDUP(length, 4);
    if ((dpy->bufptr + length) > dpy->bufmax)
	buf = _XAllocScratch(dpy, (unsigned long)(length));
    else
	buf = dpy->bufptr;
    bytes_per_src = ((long)req->width + req->leftPad + 7) >> 3;
    bytes_per_line = image->bytes_per_line;
    bytes_per_src_plane = bytes_per_line * image->height;
    total_xoffset &= (image->bitmap_unit - 1) >> 3;

    if ((total_xoffset > 0) &&
	(image->byte_order != image->bitmap_bit_order)) {
	char *temp;
	long bytes_per_temp_plane, temp_length;

	bytes_per_line = bytes_per_src + total_xoffset;
	src -= total_xoffset;
	bytes_per_temp_plane = bytes_per_line * req->height;
	temp_length = ROUNDUP(bytes_per_temp_plane * image->depth, 4);
	if (buf == dpy->bufptr)
	    temp = _XAllocScratch(dpy, (unsigned long)temp_length);
	else
	    extra = temp = Xmalloc(temp_length);
	swapfunc = SwapFunction[ComposeIndex(image->bitmap_unit,
					     image->bitmap_bit_order,
					     image->byte_order)]
			       [ComposeIndex(image->bitmap_unit,
					     dpy->byte_order,
					     dpy->byte_order)];
	for (dest = temp, j = image->depth;
	     --j >= 0;
	     src += bytes_per_src_plane, dest += bytes_per_temp_plane)
	    (*swapfunc)((unsigned char *)src, (unsigned char *)dest,
			bytes_per_line, (long)image->bytes_per_line,
			bytes_per_line, req->height);
	swapfunc = SwapFunction[ComposeIndex(image->bitmap_unit,
					     dpy->byte_order,
					     dpy->byte_order)]
			       [ComposeIndex(dpy->bitmap_unit,
					     dpy->bitmap_bit_order,
					     dpy->byte_order)];
	src = temp + total_xoffset;
	bytes_per_src_plane = bytes_per_temp_plane;
    }

    for (dest = buf, j = image->depth;
	 --j >= 0;
	 src += bytes_per_src_plane, dest += bytes_per_dest_plane)
	(*swapfunc)((unsigned char *)src, (unsigned char *)dest,
		    bytes_per_src, bytes_per_line,
		    bytes_per_dest, req->height);

    if (extra)
	Xfree(extra);

    if (buf == dpy->bufptr)
	dpy->bufptr += length;
    else
	_XSend(dpy, buf, length);
  }

/* XXX assumes image->bits_per_pixel == dest_bits_per_pixel */
static void
SendZImage(dpy, req, image, req_xoffset, req_yoffset,
	   dest_bits_per_pixel, dest_scanline_pad)
    register Display *dpy;
    register xPutImageReq *req;
    register XImage *image;
    int req_xoffset, req_yoffset, dest_bits_per_pixel, dest_scanline_pad;
{
    long bytes_per_src, bytes_per_dest, length;
    unsigned char *src, *dest;

    req->leftPad = 0;
    bytes_per_src = ROUNDUP((long)req->width * image->bits_per_pixel, 8) >> 3;
    bytes_per_dest = ROUNDUP((long)req->width * dest_bits_per_pixel,
			     dest_scanline_pad) >> 3;
    length = bytes_per_dest * req->height;
    req->length += (length + 3) >> 2;
    /* XXX this won't work when bits_per_pixel is 4 and req_xoffset is odd */
    src = (unsigned char *)image->data +
	  (req_yoffset * image->bytes_per_line) +
	  ((req_xoffset * image->bits_per_pixel) >> 3);

    /* when req_xoffset > 0, we have to worry about stepping off the
     * end of image->data.
     */
    if (((image->byte_order == dpy->byte_order) ||
	 (image->bits_per_pixel == 8)) &&
	((long)image->bytes_per_line == bytes_per_dest) &&
	((req_xoffset == 0) ||
	 ((req_yoffset + req->height) < image->height))) {
	Data(dpy, (char *)src, length);
	return;
    }

    length = ROUNDUP(length, 4);
    if ((dpy->bufptr + length) <= dpy->bufmax)
	dest = (unsigned char *)dpy->bufptr;
    else
	dest = (unsigned char *)_XAllocScratch(dpy, (unsigned long)(length));

    if ((image->byte_order == dpy->byte_order) ||
	(image->bits_per_pixel == 8))
	NoSwap(src, dest, bytes_per_src, (long)image->bytes_per_line,
	       bytes_per_dest, req->height);
    else if (image->bits_per_pixel == 32)
	SwapFourBytes(src, dest, bytes_per_src, (long)image->bytes_per_line,
		      bytes_per_dest, req->height);
    else if (image->bits_per_pixel == 24)
	SwapThreeBytes(src, dest, bytes_per_src, (long)image->bytes_per_line,
		       bytes_per_dest, req->height);
    else if (image->bits_per_pixel == 16)
	SwapTwoBytes(src, dest, bytes_per_src, (long)image->bytes_per_line,
		     bytes_per_dest, req->height);
    else
	SwapNibbles(src, dest, bytes_per_src, (long)image->bytes_per_line,
		    bytes_per_dest, req->height);
    if (dest == (unsigned char *)dpy->bufptr)
	dpy->bufptr += length;
    else
	_XSend(dpy, (char *)dest, length);
}

static void
PutImageRequest(dpy, d, gc, image, req_xoffset, req_yoffset, x, y,
		req_width, req_height, dest_bits_per_pixel, dest_scanline_pad)
    register Display *dpy;
    Drawable d;
    GC gc;
    register XImage *image;
    int x, y;
    unsigned int req_width, req_height;
    int req_xoffset, req_yoffset, dest_bits_per_pixel, dest_scanline_pad;
{
    register xPutImageReq *req;

    GetReq(PutImage, req);
    req->drawable = d;
    req->gc = gc->gid;
    req->dstX = x;
    req->dstY = y;
    req->width = req_width;
    req->height = req_height;
    req->depth = image->depth;
    req->format = image->format;
    if ((image->depth == 1) || (image->format != ZPixmap))
	SendXYImage(dpy, req, image, req_xoffset, req_yoffset);
    else
	SendZImage(dpy, req, image, req_xoffset, req_yoffset,
		   dest_bits_per_pixel, dest_scanline_pad);
}
	
static void
PutSubImage (dpy, d, gc, image, req_xoffset, req_yoffset, x, y,
	     req_width, req_height, dest_bits_per_pixel, dest_scanline_pad)
    register Display *dpy;
    Drawable d;
    GC gc;
    register XImage *image;
    int x, y;
    unsigned int req_width, req_height;
    int req_xoffset, req_yoffset, dest_bits_per_pixel, dest_scanline_pad;

{
    int left_pad, BytesPerRow, Available;

    if ((req_width == 0) || (req_height == 0))
	return;
    
    Available = ((65536 < dpy->max_request_size) ? (65536 << 2)
						 : (dpy->max_request_size << 2))
		- sizeof(xPutImageReq);

    if ((image->depth == 1) || (image->format != ZPixmap)) {
	left_pad = (image->xoffset + req_xoffset) & (dpy->bitmap_unit - 1);
	BytesPerRow = (ROUNDUP((long)req_width + left_pad,
			       dpy->bitmap_pad) >> 3) * image->depth;
    } else {
	left_pad = 0;
	BytesPerRow = ROUNDUP((long)req_width * dest_bits_per_pixel,
			      dest_scanline_pad) >> 3;
    }

    if ((BytesPerRow * req_height) <= Available) {
        PutImageRequest(dpy, d, gc, image, req_xoffset, req_yoffset, x, y, 
			req_width, req_height,
			dest_bits_per_pixel, dest_scanline_pad);
    } else if (req_height > 1) {
	int SubImageHeight = Available / BytesPerRow;

	if (SubImageHeight == 0)
	    SubImageHeight = 1;

	PutSubImage(dpy, d, gc, image, req_xoffset, req_yoffset, x, y, 
		    req_width, (unsigned int) SubImageHeight,
		    dest_bits_per_pixel, dest_scanline_pad);

	PutSubImage(dpy, d, gc, image, req_xoffset, 
		    req_yoffset + SubImageHeight, x, y + SubImageHeight,
		    req_width, req_height - SubImageHeight,
		    dest_bits_per_pixel, dest_scanline_pad);
    } else {
	int SubImageWidth = (((Available << 3) / dest_scanline_pad)
				* dest_scanline_pad) - left_pad;

	PutSubImage(dpy, d, gc, image, req_xoffset, req_yoffset, x, y, 
		    (unsigned int) SubImageWidth, 1,
		    dest_bits_per_pixel, dest_scanline_pad);

	PutSubImage(dpy, d, gc, image, req_xoffset + SubImageWidth, 
		    req_yoffset, x + SubImageWidth, y,
		    req_width - SubImageWidth, 1,
		    dest_bits_per_pixel, dest_scanline_pad);
    }
}

XPutImage (dpy, d, gc, image, req_xoffset, req_yoffset, x, y, req_width,
							      req_height)
    register Display *dpy;
    Drawable d;
    GC gc;
    register XImage *image;
    int x, y;
    unsigned int req_width, req_height;
    int req_xoffset, req_yoffset;

{
    long width = req_width;
    long height = req_height;
    int dest_bits_per_pixel, dest_scanline_pad;

    if (req_xoffset < 0) {
	width += req_xoffset;
	req_xoffset = 0;
    }
    if (req_yoffset < 0) {
	height += req_yoffset;
	req_yoffset = 0;
    }
    if ((req_xoffset + width) > image->width)
	width = image->width - req_xoffset;
    if ((req_yoffset + height) > image->height)
	height = image->height - req_yoffset;
    if ((width <= 0) || (height <= 0))
	return;

    if ((image->depth == 1) || (image->format != ZPixmap)) {
	dest_bits_per_pixel = 1;
	dest_scanline_pad = dpy->bitmap_pad;
    } else {
	register int n;
	register ScreenFormat *format;

	dest_bits_per_pixel = image->bits_per_pixel;
	dest_scanline_pad = image->bitmap_pad;
	for (n = dpy->nformats, format = dpy->pixmap_format; --n >= 0; format++)
	    if (format->depth == image->depth) {
		dest_bits_per_pixel = format->bits_per_pixel;
		dest_scanline_pad = format->scanline_pad;
	    }
	/* XXX until SendZImage gets hairy */
	if (dest_bits_per_pixel != image->bits_per_pixel) {
	    fputs("XPutImage: bits_per_pixel conversion unimplemented.\n",
		  stderr);
	    return;
	}
    }

    LockDisplay(dpy);
    FlushGC(dpy, gc);

    PutSubImage(dpy, d, gc, image, req_xoffset, req_yoffset, x, y,
		(unsigned int) width, (unsigned int) height,
		dest_bits_per_pixel, dest_scanline_pad);

    UnlockDisplay(dpy);
    SyncHandle();
}
