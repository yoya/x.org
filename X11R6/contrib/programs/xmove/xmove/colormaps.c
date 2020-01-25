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

Global char *
MoveColormaps(void)
{
     ForAllInList(&client->colormap_list)
     {
	  ColormapPtr cmap;
	  Colormap mid;
	  
	  cmap = (ColormapPtr)
	       CurrentContentsOfList(&client->colormap_list);
	  
	  mid = MapColormapID(cmap->colormap_id, Client2NewServer);
	  
	  /* Some programs, like Sun's WABI, use a colormap created by someone else.
	     What we do is a hack, and here it is: we create it on all servers the client
	     will visit from alt_fd. This gives it a different resource base from the
	     client's own. If we have such a colormap, create it on alt_fd */
	  
	  if (mid != new_server->default_cmap) {
	       if ((mid & ~client->resource_mask) == new_server->alt_resource_base) {
		    XMOVEGetInputFocus(new_server->alt_fd, &new_server->alt_seqno);
		    XMOVECreateColormap(new_server->alt_fd, &new_server->alt_seqno, mid,
					new_server->root_window,
					new_server->default_vid, AllocNone);
		    if (!XMOVEVerifyRequest(new_server->alt_fd, &new_server->alt_seqno))
			 return strdup("Unable to create custom colormaps on new server\n");
	       } else {
		    XMOVEGetInputFocus(new_fd, new_seqno);
		    XMOVECreateColormap(new_fd, new_seqno, mid,
					new_server->root_window,
					new_server->default_vid, False);
		    if (!XMOVEVerifyRequest(new_fd, new_seqno))
			 return strdup("Unable to create custom colormaps on new server\n");
	       }
	  }
	  
	  MoveColorCells(new_fd, new_seqno, cmap, mid);
     }
     
     return NULL;
}

Global void
MoveColorCells(int new_fd, unsigned short *seqno, ColormapPtr cmap, Colormap mid)
{
     ColorCellPtr cell;
     int count;

     for (cell = cmap->cell_array; cell < cmap->cell_array + cmap->cell_count;
	  cell++)
     {
	  count = cell->usage_count;
	  if (count > 1 && cell->read_write)
	       fprintf(stderr, "xmove error: r/w colorcell has usage_count>1?\n");
	  
	  while (count--)
	       MoveColorCell(new_fd, seqno, cell, mid);
     }
}

Global void
MoveColorCell(int new_fd, unsigned short *seqno, ColorCellPtr cell, Colormap mid)
{
     XColor color;
     int count;
     
     if (cell->read_write) {
	  if (XMOVEAllocColorCells(new_fd, seqno, mid, False, NULL, 0,
				   &cell->new_server_pixel, 1)) {
	       if (cell->color_name) {
		    XMOVEStoreNamedColor(new_fd, seqno, mid, cell->color_name,
					 cell->new_server_pixel,
					 DoRed | DoGreen | DoBlue);
	       } else {
		    if (client->color == True && new_server->color == False) {
			 color.red = color.blue = color.green =
			      RGB2BW(cell->red, cell->green, cell->blue);
		    } else {
			 color.red = cell->red;
			 color.blue = cell->blue;
			 color.green = cell->green;
		    }
		    color.flags = DoRed | DoGreen | DoBlue;
		    color.pixel = cell->new_server_pixel;
		    XMOVEStoreColor(new_fd, seqno, mid, &color);
	       }
	       return;
	  }
	  /* no read/write cells available for client.  We'll try
	     allocating the color read_only and hope the client doesn't
	     try to change it. */
     }
     
     if (cell->color_name) {
	  if (XMOVEAllocNamedColor(new_fd, seqno, mid, cell->color_name,
				   &color, &color)) {
	       cell->new_server_pixel = color.pixel;
	       return;
	  }
     } else {
	  if (client->color == True && new_server->color == False) {
	       color.red = color.blue = color.green =
		    RGB2BW(cell->red, cell->green, cell->blue);
	  } else {
	       color.red = cell->red;
	       color.blue = cell->blue;
	       color.green = cell->green;
	  }

	  if (XMOVEAllocColor(new_fd, seqno, mid, &color)) {
	       cell->new_server_pixel = color.pixel;
	       return;
	  }
     }
     
     /* OK, xmove gives up. If the color is more than half maximum intensity,
	we set it to white. Otherwise we set it to black. That should work. 
	If the color is named, we try looking it up. If the name doesn't exist,
	we just set it to black. */
     
     if ((cell->color_name &&
	  XMOVELookupColor(new_fd, seqno, mid, cell->color_name, &color, &color) &&
	  RGB2BW(color.red, color.blue, color.green) > (unsigned)0x18000)
	 ||
	 (!cell->color_name &&
	  RGB2BW(cell->red, cell->blue, cell->green) > (unsigned)0x18000))
     {
	  (void)XMOVEAllocNamedColor(new_fd, seqno, mid, "white", &color, &color);
     } else {
	  (void)XMOVEAllocNamedColor(new_fd, seqno, mid, "black", &color, &color);
     }
     
     cell->new_server_pixel = color.pixel;
}
