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
/* This file contains code from XGetImage.c copyrighted by MIT */

/* $XConsortium: XGetImage.c,v 11.28 91/12/23 09:31:46 rws Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

/* This file contains code from XImUtil.c copyrighted by MIT */

/* $XConsortium: XImUtil.c,v 11.55 91/12/23 09:32:47 rws Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include <stdio.h>

#define NEED_REPLIES
#include <X11/Xproto.h>

#include "xmove.h"

int XMOVEGetBitsPerPixel(ImageFormatPtr image_format, int depth);
int XMOVEGetScanlinePad(ImageFormatPtr image_format, int depth);

static unsigned int Ones(mask)                /* HACKMEM 169 */
    unsigned long mask;
{
    register unsigned long y;

    y = (mask >> 1) &033333333333;
    y = mask - y - ((y >>1) & 033333333333);
    return ((unsigned int) (((y + (y >> 3)) & 030707070707) % 077));
}

XImage *XMOVEGetImage (fd, seqno, server, d, x, y, width, height, plane_mask, format)
int fd;
unsigned short *seqno;
register Server *server;
Drawable d;
int x, y;
unsigned int width, height;
unsigned long plane_mask;
int format;	/* either XYPixmap or ZPixmap */
{
     xGetImageReply rep;
     xGetImageReq req;
     char *data;
     long nbytes;
     XImage *image;

     req.reqType   = X_GetImage;
     ISetShort((unsigned char *)&req.length, 5);
     ISetLong((unsigned char *)&req.drawable, d);
     ISetShort((unsigned char *)&req.x, x);
     ISetShort((unsigned char *)&req.y, y);
     ISetShort((unsigned char *)&req.width, width);
     ISetShort((unsigned char *)&req.height, height);
     ISetLong((unsigned char *)&req.planeMask, plane_mask);
     req.format    = format;
	
     SendBuffer(fd, (unsigned char *)&req, sz_xGetImageReq);
     if (!ReceiveReply(fd, (unsigned char *)&rep, 32, ++(*seqno)))
	  return NULL;
     
     nbytes = ILong((unsigned char *)&rep.length) << 2;
     data = malloc((unsigned) nbytes);
     ReceiveBuffer(fd, (unsigned char *)data, nbytes);
     
     image = XMOVEPreCreateImage(server->formats, NULL, format, NULL,
				 plane_mask, rep.depth, data, width, height);

     
     return (image);
}

/* XMOVEPreCreateImage() serves as an intermediary between xmove and
   Xlib in terms of creating images. It uses standard xmove structures
   instead of the structures expected by the Xlib call. */

/* if gid is not NULL, it is used to determine plane_mask. Otherwise
   if plane_mask is needed it should contain valid data. */

XImage *XMOVEPreCreateImage(image_format, visual, format, gc, plane_mask,
			    depth, data, width, height)
ImageFormatPtr image_format;
VisualPtr visual;
int format;
GCPtr gc;
unsigned int plane_mask;
unsigned int depth;
char *data;
unsigned int width;
unsigned int height;
{
     if (format != ZPixmap) {	/* format == XYBitmap or XYPixmap */

	  if (gc && (gc->values_mask & GCPlaneMask))
	       plane_mask = gc->values.plane_mask;

	  return (XMOVECreateImage(image_format, NULL,
				   Ones (plane_mask &
					 (((unsigned long)0xFFFFFFFF) >>
					  (32 - depth))),
				   format, 0, data, width, height,
				   image_format->image_bitmap_pad, 0));
     } else /* format == ZPixmap */
	  return (XMOVECreateImage (image_format, NULL,
				    depth, ZPixmap, 0, data, width, height,
				    XMOVEGetScanlinePad(image_format, (int) depth), 0));
}

/*
 * CreateImage
 * 
 * Allocates the memory necessary for an XImage data structure. 
 * Initializes the structure with "default" values and returns XImage. 
 * 
 */

XImage *XMOVECreateImage (image_format, visual, depth, format, offset, data, width, height,
    xpad, image_bytes_per_line)
    register ImageFormatPtr image_format;
    register VisualPtr visual;
    unsigned int depth;
    int format;
    int offset; /*How many pixels from the start of the data does the
		picture to be transmitted start?*/

    char *data;
    unsigned int width;
    unsigned int height;
    int xpad;	
    int image_bytes_per_line; 
		/*How many bytes between a pixel on one line and the pixel with
		  the same X coordinate on the next line? 0 means
		  XCreateImage can calculate it.*/
{
	register XImage *image;
	int bits_per_pixel = 1;

	if (depth == 0 || depth > 32 ||
	    (format != XYBitmap && format != XYPixmap && format != ZPixmap) ||
	    (format == XYBitmap && depth != 1) ||
	    (xpad != 8 && xpad != 16 && xpad != 32) ||
	    offset < 0 || image_bytes_per_line < 0)
	    return (XImage *) NULL;
	if ((image = (XImage *) calloc(1, (unsigned) sizeof(XImage))) == NULL)
	    return (XImage *) NULL;

	image->width = width;
	image->height = height;
	image->format = format;
	image->byte_order = image_format->image_byte_order;
	image->bitmap_unit = image_format->image_bitmap_unit;
	image->bitmap_bit_order = image_format->image_bitmap_bit_order;

	if (visual != NULL) {
		image->red_mask = visual->red_mask;
		image->green_mask = visual->green_mask;
		image->blue_mask = visual->blue_mask;
	}
	else {
		image->red_mask = image->green_mask = image->blue_mask = 0;
	}
	if (format == ZPixmap) 
	{
	    bits_per_pixel = XMOVEGetBitsPerPixel(image_format, (int) depth);
	}

	image->xoffset = offset;
	image->bitmap_pad = xpad;
	image->depth = depth;
 	image->data = data;
	/*
	 * compute per line accelerator.
	 */
	if (image_bytes_per_line == 0)
	{
	if (format == ZPixmap)
	    image->bytes_per_line = 
	       ROUNDUP((bits_per_pixel * width), image->bitmap_pad) >> 3;
	else
	    image->bytes_per_line =
	        ROUNDUP((width + offset), image->bitmap_pad) >> 3;
	}
	else image->bytes_per_line = image_bytes_per_line;

	image->bits_per_pixel = bits_per_pixel;
	image->obdata = NULL;
	_XInitImageFuncPtrs (image);

	return image;
}

int
XMOVEGetBitsPerPixel(image_format, depth)
ImageFormatPtr image_format;
int depth;
{
     register PixmapFormatPtr fmt;
     
     ForAllInList(&image_format->format_list)
     {
	  fmt = (PixmapFormatPtr)
	       CurrentContentsOfList(&image_format->format_list);
	  
	  if (fmt->depth == depth)
	       return(fmt->bits_per_pixel);
     }
     
     if (depth <= 4)
	  return 4;
     if (depth <= 8)
	  return 8;
     if (depth <= 16)
	  return 16;
     return 32;
}

int
XMOVEGetScanlinePad(image_format, depth)
ImageFormatPtr image_format;
int depth;
{
     register PixmapFormatPtr fmt;

     ForAllInList(&image_format->format_list)
     {
	  fmt = (PixmapFormatPtr)
	       CurrentContentsOfList(&image_format->format_list);

	  if (fmt->depth == depth)
	       return (fmt->scanline_pad);
     }
     
     return(image_format->image_bitmap_pad);
}
