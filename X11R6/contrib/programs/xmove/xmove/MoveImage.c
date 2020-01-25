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
#include "xmove.h"

Global void
CopyImage(XImage      *image,
	  XImage      *new_image,
	  unsigned int width,
	  unsigned int height)
{
     int x,y;
     
     for (x=0; x < width; x++)
	  for (y=0; y < height; y++)
	       XPutPixel(new_image, x, y, XGetPixel(image, x, y));
}


Global void
MoveImage(XImage      *image,
	  XImage      *new_image,
	  unsigned int width,
	  unsigned int height,
	  ColormapPtr  cmap,
	  Direction    dir)
{
     int x, y;
     
     for (x=0; x < width; x++)
	  for (y=0; y < height; y++)
	       XPutPixel(new_image, x, y,
			 MapColorCell(XGetPixel(image, x, y), cmap, dir));
}
