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
/* The following code is based, at least in part, on XImUtil.c, which
   is part of the XLib source code. */

/* $XConsortium: XImUtil.c,v 11.51 91/07/23 12:02:13 rws Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

/*
Permission to use, copy, modify, distribute, and sell this software and its
documentation for any purpose is hereby granted without fee, provided that
the above copyright notice appear in all copies and that both that
copyright notice and this permission notice appear in supporting
documentation, and that the name of M.I.T. not be used in advertising or
publicity pertaining to distribution of the software without specific,
written prior permission.  M.I.T. makes no representations about the
suitability of this software for any purpose.  It is provided "as is"
without express or implied warranty.
*/

#include "xmove.h"

#if __STDC__
#define Const const
#else
#define Const /**/
#endif

static unsigned long Const low_bits_table[] = {
    0x00000000, 0x00000001, 0x00000003, 0x00000007,
    0x0000000f, 0x0000001f, 0x0000003f, 0x0000007f,
    0x000000ff, 0x000001ff, 0x000003ff, 0x000007ff,
    0x00000fff, 0x00001fff, 0x00003fff, 0x00007fff,
    0x0000ffff, 0x0001ffff, 0x0003ffff, 0x0007ffff,
    0x000fffff, 0x001fffff, 0x003fffff, 0x007fffff,
    0x00ffffff, 0x01ffffff, 0x03ffffff, 0x07ffffff,
    0x0fffffff, 0x1fffffff, 0x3fffffff, 0x7fffffff,
    0xffffffff
};

#ifndef WORD64
static unsigned long byteorderpixel = MSBFirst << 24;
#endif

Global void MapImage32 (data, width, height, depth, bpl, cmap, dir, src_endian, dst_endian)
char *data;
unsigned int width, height, depth, bpl; /* bpl == bytes_per_line */
ColormapPtr cmap;
Direction dir;
Bool src_endian, dst_endian;
{
     register unsigned char *addr;
     unsigned char *line_start = (unsigned char *)data;
     unsigned int y = 0;
     register unsigned int x;
     register ColorCellPtr cell_array = cmap->cell_array;
     register u_long lowbits = low_bits_table[depth];
								       
     if (src_endian && dst_endian) {
	  if (dir == Request) {
	       while (y++ < height) {
		    x = 0;
		    addr = line_start;
		    
		    while (x++ < width) {
			 *(unsigned long *)addr = cell_array[(*(unsigned long *)addr)&lowbits].server_pixel;
			 addr += 4;
		    }
		    
		    line_start += bpl;
	       }
	       return;
	  }
	  
	  while (y++ < height) {
	       x = 0;
	       addr = line_start;

	       while (x++ < width) {
		    *(unsigned long *)addr = MapColorCell((*(unsigned long *)addr)&lowbits, cmap, dir);
		    addr += 4;
	       }

	       line_start += bpl;
	  }
     } else {
	  unsigned long pixel;
	  unsigned char *pixeladdr = (unsigned char *)&pixel;
	  
	  while (y++ < height) {
	       x = 0;
	       addr = line_start;

	       while (x++ < width) {
		    if (src_endian)
			 pixel = *(unsigned long *)addr;
		    else {
			 if (XmoveLittleEndian)
			      pixel = ((unsigned long)addr[0] << 24 |
				       (unsigned long)addr[1] << 16 |
				       (unsigned long)addr[2] << 8 |
				       addr[3]);
			 else
			      pixel = ((unsigned long)addr[3] << 24 |
				       (unsigned long)addr[2] << 16 |
				       (unsigned long)addr[1] << 8 |
				       addr[0]);
		    }

		    pixel = pixel & lowbits;
		    if (dir == Request)
			 pixel = cell_array[pixel].server_pixel;
		    else
			 pixel = MapColorCell(pixel, cmap, dir);

		    if (dst_endian)
			 *(unsigned long *)addr = pixel;
		    else {
			 if (XmoveLittleEndian)
			      *(unsigned long *)addr = ((unsigned long)pixeladdr[0] << 24 |
							(unsigned long)pixeladdr[1] << 16 |
							(unsigned long)pixeladdr[2] << 8 |
							pixeladdr[3]);
			 else
			      *(unsigned long *)addr = ((unsigned long)pixeladdr[3] << 24 |
							(unsigned long)pixeladdr[2] << 16 |
							(unsigned long)pixeladdr[1] << 8 |
							pixeladdr[0]);
		    }

		    addr += 4;
	       }

	       line_start += bpl;
	  }
     }
}								  

Global void MapImage16 (data, width, height, depth, bpl, cmap, dir, src_endian, dst_endian)
char *data;
unsigned int width, height, depth, bpl; /* bpl == bytes_per_line */
ColormapPtr cmap;
Direction dir;
Bool src_endian, dst_endian; /* is the source/destination of the same endianism as localhost? */
{
     register unsigned char *addr;
     unsigned char *line_start = (unsigned char *)data;
     unsigned int y = 0;
     register unsigned int x;
     register ColorCellPtr cell_array = cmap->cell_array;
     register u_long lowbits = low_bits_table[depth];
								       
     if (src_endian && dst_endian) {
	  if (dir == Request) {
	       while (y++ < height) {
		    x = 0;
		    addr = line_start;
		    
		    while (x++ < width) {
			 *(unsigned short *)addr = cell_array[(*(unsigned short *)addr)&lowbits].server_pixel;
			 addr += 2;
		    }
		    
		    line_start += bpl;
	       }
	       return;
	  }
	  
	  while (y++ < height) {
	       x = 0;
	       addr = line_start;

	       while (x++ < width) {
		    *(unsigned short *)addr = MapColorCell((*(unsigned short *)addr)&lowbits, cmap, dir);
		    addr += 2;
	       }

	       line_start += bpl;
	  }
     } else {
	  unsigned short pixel;
	  unsigned char *pixeladdr = (unsigned char *)&pixel;
	  
	  while (y++ < height) {
	       x = 0;
	       addr = line_start;

	       while (x++ < width) {
		    if (src_endian)
			 pixel = *(unsigned short *)addr;
		    else {
			 if (XmoveLittleEndian)
			      pixel = ((unsigned short)addr[0] << 8 |
				       addr[1]);
			 else
			      pixel = ((unsigned short)addr[1] << 8 |
				       addr[0]);
		    }

		    pixel = pixel & lowbits;
		    if (dir == Request)
			 pixel = cell_array[pixel].server_pixel;
		    else
			 pixel = MapColorCell(pixel, cmap, dir);

		    if (dst_endian) {
			 if (XmoveLittleEndian)
			      *(unsigned short *)addr = ((unsigned short)pixeladdr[0] << 8 |
							 pixeladdr[1]);
			 else
			      *(unsigned short *)addr = ((unsigned short)pixeladdr[1] << 8 |
							 pixeladdr[0]);
		    } else
			 *(unsigned short *)addr = pixel;

		    addr += 2;
	       }

	       line_start += bpl;
	  }
     }
}								  

Global void MapImage8 (data, width, height, depth, bpl, cmap, dir)
char *data;
unsigned int width, height, depth, bpl; /* bpl == bytes_per_line */
ColormapPtr cmap;
Direction dir;
{
     register unsigned char *addr;
     unsigned char *line_start = (unsigned char *)data;
     unsigned int y = 0;
     register int x;
     register u_long lowbits = low_bits_table[depth];
								       
     if (dir == Request) {
	  register ColorCellPtr cell_array = cmap->cell_array;
	  
	  while (y++ < height) {
	       x = 0;
	       addr = line_start;
	       
	       while (x++ < width) {
		    *addr = cell_array[(*addr)&lowbits].server_pixel;
		    addr++;
	       }
	       
	       line_start += bpl;
	  }
	  return;
     }
     
     if (dir == Reply && (width > 16 || height > 16)) {
	  register card32 *server_cell_map = MakeServerColorCellMap(cmap, server->default_depth);

	  while (y++ < height) {
	       x = 0;
	       addr = line_start;

	       while (x++ < width) {
		    *addr = server_cell_map[(*addr)&lowbits];
		    addr++;
	       }

	       line_start += bpl;
	  }
	  free(server_cell_map);
	  return;
     }

     while (y++ < height) {
	  x = 0;
	  addr = line_start;
	  
	  while (x++ < width) {
	       *addr = MapColorCell((*addr)&lowbits, cmap, dir);
	       addr++;
	  }
	  
	  line_start += bpl;
     }
}

/* for ConvertPixmapToColormap, which requires the cells be mapped twice. Once
 * from server to client, then back from client to server. replymap is used to
 * translate from server to client, cmap->cell_array to translate from client
 * to server.
 */

Global void MapImage8Double (data, width, height, depth, bpl, replymap, cmap)
char *data;
unsigned int width, height, depth, bpl;
card32 *replymap;
ColormapPtr cmap;
{
     register unsigned char *addr;
     unsigned char *line_start = (unsigned char *)data;
     unsigned int y = 0;
     register int x;
     register ColorCellPtr cell_array = cmap->cell_array;
     register u_long lowbits = low_bits_table[depth];

     while (y++ < height) {
	  x = 0;
	  addr = line_start;

	  while (x++ < width) {
	       *addr = cell_array[replymap[(*addr)&lowbits]].server_pixel;
	       addr++;
	  }

	  line_start += bpl;
     }
}

