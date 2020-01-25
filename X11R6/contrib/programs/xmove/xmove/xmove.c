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
#include <X11/Xproto.h>
#include "xmove.h"

/*
 *
 * $Author: pds $
 * $Date: 1993/02/04 21:47:36 $
 * $Locker: ethan $
 * $Revision: 1.10 $
 * $Log: xmove.c,v $
 * Revision 1.10  1993/02/04  21:47:36  pds
 * *** empty log message ***
 *
 * Revision 1.9  1992/12/15  19:17:11  ethan
 * stolen by pds.
 *
 * Revision 1.8  1992/12/14  03:49:52  pds
 * *** empty log message ***
 *
 * Revision 1.7  1992/11/21  00:18:33  pds
 * Took out a couple of bugs... nothing earth shattering
 *
 * Revision 1.6  1992/11/11  04:31:36  ethan
 * *** empty log message ***
 *
 * Revision 1.5  1992/11/09  00:10:56  ethan
 * various attributes related routines removed
 * TagAllDescendants now works -- uses recursive method
 *
 * Revision 1.4  1992/11/08  23:38:09  pds
 * *** empty log message ***
 *
 * Revision 1.3  1992/10/20  22:13:45  pds
 * *** empty log message ***
 *
 * Revision 1.2  1992/10/16  16:47:18  pds
 * *** empty log message ***
 *
 * Revision 1.1  1992/10/13  22:18:11  pds
 * Initial revision
 *
 * Revision 2.9  1992/04/11  01:05:37  shamash
 * multiple clients work, all events properly managed
 *
 * Revision 2.8  1992/02/22  07:01:07  shamash
 * little/big endian bug fixed
 *
 * Revision 2.7  1992/02/22  05:52:04  shamash
 * keyboard mapping properly done
 *
 * Revision 2.6  1992/02/11  23:10:43  shamash
 * verbose 0 works!
 *
 * Revision 2.5  1992/01/31  06:50:03  shamash
 * emacs is mobile!!
 *
 * Revision 2.4  1992/01/29  01:52:10  shamash
 * fonts, pixmaps recorded
 *
 *
 */

Global unsigned char *MappingNotifyEventBuf;
Global long MappingNotifyEventBufLength;
extern LinkList AtomMappings;
extern LinkList AtomTypeMappings;
extern LinkList meta_client_list;

Global void
InitXMove(void)
{
    /* here we do all the local initialization stuff that we may need */
    /* to do */

    MappingNotifyEventBuf = (unsigned char *) Tcalloc(32, char);
    MappingNotifyEventBufLength = 32;

    /* fill up the MappingNotifyEvent with the appropriate values */

    MappingNotifyEventBuf[0] = 34;	     /* event code */
    MappingNotifyEventBuf[1] = 0;	     /* unused */

    /* bytes 2,3 are the sequence number.  They will be poked in later */

    MappingNotifyEventBuf[4] = 1;	     /* request:  1 = keyboard */

    /* bytes 5,6 are the first keycode, and count, respectively.  They */
    /* will be poked in later */
    
    /* the rest of the bytes (7..31) are unused. */
}
       

/* given a base and the current client, locate the owner of "base", whether
   it is the current client or it is related via the meta client. Do not
   search other clients unless they are related via the meta client. If the
   base is 0, ie this is a default colormap, root window or such, then
   the information about it is stored in the client locally and so return
   the current client. */

Global Client *
FindClientFromBase(unsigned long base)
{
     LinkLeaf *ll;
     Client *cur_client;
     
     if (client->resource_base == base || base == 0 || client->alt_resource_base == base)
	  return client;

     ScanList(&meta_client->client_list, ll, cur_client, Client *) {
	  if (cur_client->resource_base == base || cur_client->alt_resource_base == base)
	       return cur_client;
     }

     return NULL;
}


void
SetCurrentClientName(char *name, int length)
{
     /* name isn't guaranteed to be null-terminated. store a null-terminated
	string. */

     if (client->window_name)
	  free(client->window_name);
     
     if (name[length-1] == '\0') {
	  client->window_name = malloc(length);
	  strncpy(client->window_name, name, length);
     } else {
	  client->window_name = malloc(length+1);
	  strncpy(client->window_name, name, length);
	  client->window_name[length] = '\0';
     }
}

void
AddGCToCurrentClient(GCPtr gc)
{
#ifdef DEBUG
    Dprintf(("Adding gc to current client:\n"));
    Dprintf(("  id = %x, drawable = %x\n", 
	    gc->gc_id, gc->drawable_id));
#endif

    hash_add(client->gc_table, gc);
}

void
FreeClientLists(Client *client)
{
     Pointer item;
     PixmapPtr pixmap;
     FontPtr font;
     AtomPtr atom;
     hash_location loc;

     if (client->window_name)
	  free(client->window_name);

     if (client->startup_message)
	  free(client->startup_message);
     
     while (item = deleteFirst(&client->resource_maps))
	  Tfree(item);

     hash_free(client->window_table);

     for (pixmap = hashloc_init(&loc, client->pixmap_table);
	  pixmap; pixmap = hashloc_deletenext(&loc))
     {
	  if (pixmap->pixmap_image) {
	       free((char *)pixmap->pixmap_image->data);
	       free((char *)pixmap->pixmap_image);
	  }
	  Tfree(pixmap);
     }
     hash_free(client->pixmap_table);

     for (font = hashloc_init(&loc, client->font_table);
	  font; font = hashloc_deletenext(&loc))
     {
	  Tfree(font->name);
	  Tfree(font);
     }
     hash_free(client->font_table);

     hash_free(client->glyph_cursor_table);

     hash_free(client->cursor_table);

     hash_free(client->gc_table);

     while (item = deleteFirst(&client->colormap_list)) {
	  Tfree((Pointer)(((ColormapPtr)item)->cell_array));
	  Tfree(item);
     }

     while (item = deleteFirst(&client->grab_list))
	  Tfree(item);

     for (atom = hashloc_init(&loc, client->atom_table);
	  atom; atom = hashloc_deletenext(&loc))
     {
	  free(atom->atom_name);
	  Tfree(atom);
     }
     hash_free(client->atom_table);

     while (item = deleteFirst(&client->selection_list))
	  Tfree(item);
     
     if (client->formats) {
	  while (item = deleteFirst(&client->formats->format_list))
	       Tfree(item);
	  
	  while (item = deleteFirst(&client->formats->depth_list)) {
	       Pointer item2;
	       
	       while (item2 = deleteFirst(&((DepthPtr)item)->visual_list))
		    Tfree(item2);
	       
	       Tfree(item);
	  }
	  
	  free(client->formats);
     }
     
     if (client->misc)
	  free(client->misc);
}

void
AddColormapToCurrentClient(ColormapPtr colormap)
{
    Dprintf(("Adding colormap to current client:\n"))

    appendToList(&client->colormap_list, (Pointer) colormap);
    if (colormap->colormap_id != client->default_cmap)
	 client->allocated_colormaps++;
}

void
AddWindowToCurrentClient(WindowPtr window)
{
#ifdef DEBUG
    Dprintf(("Adding window to current client:\n"));
    Dprintf(("    id = 0x%x, parent = 0x%x, geom: %dx%d+%d+%d\n",
	    window->window_id, window->parent_id, 
	    window->width, window->height, window->x, window->y));
#endif

    hash_add(client->window_table, window);
}

void
RemoveGrabsUsingWindow(Window win)
{
     GrabPtr grab;
     
     ForAllInList(&client->grab_list)
     {
	  grab = (GrabPtr) CurrentContentsOfList(&client->grab_list);

	  if (grab->grab_window == win)
	       free((char *) deleteCurrent(&client->grab_list));
	  else
	       if (grab->confine_to == win) {
		    grab->confine_to = (Window)0;
	       }
     }
}
	  

void
RemoveClientFromMetaClient(MetaClient *meta_client, Client *client)
{
     ForAllInList(&meta_client->client_list) {
	  if (CurrentContentsOfList(&meta_client->client_list) ==
	      (Pointer)client)
	  {
	       deleteCurrent(&meta_client->client_list);
	       return;
	  }
     }
}

void
RemoveMetaClient(MetaClient *meta_client)
{
     ForAllInList(&meta_client_list) {
	  if (CurrentContentsOfList(&meta_client_list) ==
	      (Pointer)meta_client)
	  {
	       deleteCurrent(&meta_client_list);
	       return;
	  }
     }
}

void
RemoveCurrentWindow(LinkList *list)
{
     Window xwin = ((WindowPtr)CurrentContentsOfList(list))->window_id;
     RemoveGrabsUsingWindow(xwin);
     free((char *) deleteCurrent(list));
}

void
RemoveWindowFromCurrentClient(Window xwin)
{
    WindowPtr temp_window;
    
    if (temp_window = hash_delete(client->window_table, xwin))
	 free((char *)temp_window);
}

void
ReparentWindowForCurrentClient(Window xwin, Window parent, int16 x, int16 y)
{
    WindowPtr temp_window;
    
    if (temp_window = hash_find(client->window_table, xwin)) {
	 temp_window->parent_id = parent;
	 temp_window->x = x;
	 temp_window->y = y;
    }
}

void
RemoveGCFromCurrentClient(GContext xgc)
{
    GCPtr gc;

    Dprintf(("In RemoveGCFromCurrentClient()\n"));
    
    if (gc = hash_delete(client->gc_table, xgc))
	 free((char *)gc);
}

void
RemoveColormapFromCurrentClient(Colormap xcolormap)
{
     ColormapPtr temp_colormap;
     
     ForAllInList(&client->colormap_list)
     {
	  temp_colormap = (ColormapPtr)
	       CurrentContentsOfList(&client->colormap_list);
	  
	  if (temp_colormap->colormap_id == xcolormap)
	  {
	       ColorCellPtr item;
	       
	       temp_colormap = (ColormapPtr)
		    deleteCurrent(&client->colormap_list);

	       for (item = temp_colormap->cell_array;
		    item < temp_colormap->cell_array + temp_colormap->cell_count;
		    item++)
		    if (item->color_name)
			 free(item->color_name);
	       
	       Tfree((Pointer)temp_colormap->cell_array);
	       Tfree((Pointer)temp_colormap);
	       
	       if (xcolormap != client->default_cmap)
		    client->allocated_colormaps--;
	       return;
	  }
     }

     return;
}

void
RemoveFontFromCurrentClient(Font xfont)
{
    FontPtr temp_font;
    
    if (temp_font = hash_delete(client->font_table, xfont)) {
	 free((char *)temp_font->name);
	 free((char *)temp_font);
    }
}


void
RemoveCursorFromCurrentClient(Cursor xcursor)
{
     CursorPtr cursor;
     GlyphCursorPtr glyph;
     
     /* this routine is a bit different from the above several, since */
     /* we don't know exactly what kind of cursor this is.  It is */
     /* either a glyph cursor (created from a font), or a regular */
     /* cursor (created from a pixmap).  Therefore, we have two check */
     /* the two lists for the cursor. */
     
     if (cursor = hash_delete(client->cursor_table, xcursor))
	  free(cursor);
     else
	  if (glyph = hash_delete(client->glyph_cursor_table, xcursor))
	       free(glyph);

     return;
}


void
RecolorCursorFromCurrentClient(Cursor xcursor, short fg_r, short fg_g, 
			       short fg_b, short bg_r, short bg_g, short bg_b)

{

     /* This routine will find a cursor and will change its color values. */
     /* We don't know exactly what kind of cursor this is.  It is */
     /* either a glyph cursor (created from a font), or a regular */
     /* cursor (created from a pixmap).  Therefore, we have two check */
     /* the two lists for the cursor. */
     
     CursorPtr temp_cursor;
     GlyphCursorPtr temp_glyph_cursor;
     
     if (temp_cursor = hash_find(client->cursor_table, xcursor)) {
	  temp_cursor->foreground_color.red = fg_r;
	  temp_cursor->foreground_color.green = fg_g;
	  temp_cursor->foreground_color.blue = fg_b;
	  temp_cursor->background_color.red =  bg_r;
	  temp_cursor->background_color.green =  bg_g;
	  temp_cursor->background_color.blue =  bg_b;
	  
	  return;
     }
     
     if (temp_glyph_cursor = hash_find(client->glyph_cursor_table, xcursor)) {
	  temp_glyph_cursor->foreground_color.red = fg_r;
	  temp_glyph_cursor->foreground_color.green = fg_g;
	  temp_glyph_cursor->foreground_color.blue = fg_b;
	  temp_glyph_cursor->background_color.red =  bg_r;
	  temp_glyph_cursor->background_color.green =  bg_g;
	  temp_glyph_cursor->background_color.blue =  bg_b;
     }
}

void
AddGlyphCursorToCurrentClient(GlyphCursorPtr cursor)
{
     hash_add(client->glyph_cursor_table, cursor);
}

void
AddCursorToCurrentClient(CursorPtr cursor)
{
     hash_add(client->cursor_table, cursor);
}

void
AddGrabToCurrentClient(GrabPtr grab)
{
    appendToList(&client->grab_list, (Pointer) grab);
}

void
ChangeGrabPointerFromCurrentClient(cursor, time, event_mask)
Cursor cursor;
Time time;
card16 event_mask;
{
    GrabPtr temp_grab;
    
    ForAllInList(&client->grab_list)
    {
	temp_grab = (GrabPtr)CurrentContentsOfList(&client->grab_list);
	if(temp_grab->type == 26)
	{
	    temp_grab->cursor = cursor;
	    temp_grab->time = time;
	    temp_grab->event_mask = event_mask;
	    return;
	}
    }
    return;
}

void
RemoveGrabButtonFromCurrentClient(button, grab_window, modifiers)
card8 button;
Window grab_window;
card16 modifiers;
{
    GrabPtr temp_grab;
    
    ForAllInList(&client->grab_list)
    {
	temp_grab = (GrabPtr)CurrentContentsOfList(&client->grab_list);
	if((temp_grab->type == 28) &&
	   (temp_grab->button == button) &&
	   (temp_grab->grab_window == grab_window) &&
	   (temp_grab->modifiers == modifiers))
	{
	    temp_grab=(GrabPtr)deleteCurrent(&client->grab_list);
	    free((char *)temp_grab);
	    return;
	}
    }
    return;
}

void
RemoveGrabKeyFromCurrentClient(key, grab_window, modifiers)
card8 key;
Window grab_window;
card16 modifiers;
{
    GrabPtr temp_grab;
    
    ForAllInList(&client->grab_list)
    {
	temp_grab = (GrabPtr)CurrentContentsOfList(&client->grab_list);
	if((temp_grab->type == 33) &&
	   (temp_grab->key == key) &&
	   (temp_grab->grab_window == grab_window) &&
	   (temp_grab->modifiers == modifiers))
	{
	    temp_grab=(GrabPtr)deleteCurrent(&client->grab_list);
	    free((char *)temp_grab);
	    return;
	}
    }
    return;
}

void
RemoveGrabPointerFromCurrentClient()
{
    GrabPtr temp_grab;
    
    ForAllInList(&client->grab_list)
    {
	temp_grab = (GrabPtr)CurrentContentsOfList(&client->grab_list);
	if(temp_grab->type == 26)
	{
	    temp_grab=(GrabPtr)deleteCurrent(&client->grab_list);
	    free((char *)temp_grab);
	    return;
	}
    }
    return;
}

void
RemoveGrabKeyboardFromCurrentClient()
{
    GrabPtr temp_grab;
    
    ForAllInList(&client->grab_list)
    {
	temp_grab = (GrabPtr)CurrentContentsOfList(&client->grab_list);
	if(temp_grab->type == 31)
	{
	    temp_grab=(GrabPtr)deleteCurrent(&client->grab_list);
	    free((char *)temp_grab);
	    return;
	}
    }
    return;
}

void
AddPixmapToCurrentClient(PixmapPtr pixmap)
{
    Dprintf(("Adding pixmap to current client:\n"));
    Dprintf(("id:  0x%x, size = (%d,%d), depth = %d\n",
	     pixmap->pixmap_id, pixmap->width, pixmap->height, pixmap->depth));

    hash_add(client->pixmap_table, pixmap);
}

void
RemovePixmapFromCurrentClient(Pixmap pixmap)
{
    PixmapPtr temp_pixmap;
    
    if (temp_pixmap = hash_delete(client->pixmap_table, pixmap)) {
	 if (temp_pixmap->pixmap_image)
	      free((char *)temp_pixmap->pixmap_image);
	 free((char *)temp_pixmap);
    }
}

void
AddSelectionToCurrentClient(SelectionPtr selection)
{
#ifdef DEBUG
    Dprintf(("Adding selection to current client:\n"))
    Dprintf(("selection: %d\n", selection->selection))
#endif

    appendToList(&client->selection_list, (Pointer) selection);
}

void


RemoveSelectionFromCurrentClient(Atom selection)
{
    SelectionPtr temp_selection;
    
    ForAllInList(&client->selection_list)
    {
	temp_selection = (SelectionPtr)
	     CurrentContentsOfList(&client->selection_list);
	
	if(temp_selection->selection == selection)
	{
	    temp_selection =
		 (SelectionPtr)deleteCurrent(&client->selection_list);
	    free((char *)temp_selection);
	    return;
	}
    }
    return;
}

void
AddAtomToCurrentClient(AtomPtr atom)
{
#ifdef DEBUG
    Dprintf(("Adding atom to current client:\n"));
    Dprintf(("name: %s\n", atom->atom_name));
#endif

    hash_add(client->atom_table, atom);
}

AtomPtr
CreateNewAtom(Atom atom, char *name, Direction dir)
{
     AtomPtr new_atom;
     Atom client_atom;

     client_atom = atom;
     if (dir == Request) {
	  if (new_atom = FindAtom(client_atom))
	       return new_atom;
     } else {
	  if (new_atom = FindServerAtom(atom))
	       return new_atom;
	  
	  while (new_atom = FindAtom(client_atom))
	       client_atom = new_atom->server_atom;
     }
     
     new_atom = malloc(sizeof(AtomRec));
     new_atom->client_atom        = client_atom;
     new_atom->server_atom        = atom;
     new_atom->property_map_func  = FindAtomMapping(name);
     new_atom->type_map_func      = FindAtomTypeMapping(name);
     new_atom->transient_map_func = NULL;
     new_atom->atom_name          = malloc(strlen(name)+1);
     strcpy(new_atom->atom_name, name);

     hash_add(client->atom_table, new_atom);
     return new_atom;
}

AtomPtr
FindAtom(Atom atom)
{
     return(hash_find(client->atom_table, atom));
}

AtomPtr
FindServerAtom(Atom atom)
{
     hash_location loc;
     AtomPtr temp_atom;

     for (temp_atom = hashloc_init(&loc, client->atom_table);
	  temp_atom; temp_atom = hashloc_getnext(&loc))
     {
	  if (temp_atom->server_atom == atom)
	       return temp_atom;
     }

     return NULL;
}

void
RemoveAtomFromCurrentClient(Atom atom)
{
    AtomPtr temp_atom;
    
    if (temp_atom = hash_delete(client->atom_table, atom)) {
	 free(temp_atom->atom_name);
	 free((char *)temp_atom);
    }
}

void
AddFontToCurrentClient(FontPtr font)
{
#ifdef DEBUG
    Dprintf(("Adding font to current client:\n"));
    Dprintf(("name = %s, id = 0x%x\n", 
	     font->name, font->font_id));
#endif

    hash_add(client->font_table, font);
}

AtomMappingPtr
FindAtomMapping(char *name)
{
     AtomMappingPtr atommap;

     ForAllInList(&AtomMappings)
     {
	  atommap = (AtomMappingPtr) CurrentContentsOfList(&AtomMappings);
	  if (strcmp(name, atommap->name) == 0)
	       return atommap;
     }

     return NULL;
}

AtomMappingPtr
FindAtomTypeMapping(char *name)
{
     AtomMappingPtr atommap;

     ForAllInList(&AtomTypeMappings)
     {
	  atommap = (AtomMappingPtr) CurrentContentsOfList(&AtomTypeMappings);
	  if (strcmp(name, atommap->name) == 0)
	       return atommap;
     }

     return NULL;
}

AtomMappingPtr
AddAtomMapping(char *name, void (*MapProperty)())
{
     AtomMappingPtr atommap;

     if (atommap = FindAtomMapping(name))
	  return NULL;

     atommap = malloc(sizeof(AtomMappingRec));

     if (atommap == NULL)
	  return NULL;

     atommap->name         = name;
     atommap->MapProperty  = MapProperty;
     
     appendToList(&AtomMappings, (Pointer) atommap);

     return atommap;
}

AtomMappingPtr
AddAtomTypeMapping(char *name, void (*MapProperty)())
{
     AtomMappingPtr atommap;

     if (atommap = FindAtomTypeMapping(name))
	  return NULL;

     atommap = malloc(sizeof(AtomMappingRec));

     if (atommap == NULL)
	  return NULL;

     atommap->name         = name;
     atommap->MapProperty  = MapProperty;
     
     appendToList(&AtomTypeMappings, (Pointer) atommap);

     return atommap;
}

ColormapPtr
FindColormapFromCurrentClient(Colormap cmap_id)
{
     ColormapPtr temp_cmap;
     Client *cur_client = FindClientFromBase(cmap_id & ~(client->resource_mask));

     if (cur_client == NULL)
	  return NULL;

     if ((cmap_id & (~client->resource_mask)) == 0) {
	  temp_cmap = (ColormapPtr)TopOfList(&cur_client->colormap_list);
	  if (cmap_id != client->default_cmap) {
	       /* if client uses screen 1, we correct cmap_id stored in the default
		  colormap. Client may only use one screen, so this is OK */
	       temp_cmap->colormap_id = cmap_id;
	       client->default_cmap = cmap_id;
	       if (!client->xmoved_to_elsewhere)
		    server->default_cmap = cmap_id;
	  }
	  return temp_cmap;
     }
     
     ForAllInList(&cur_client->colormap_list)
     {
	  temp_cmap = (ColormapPtr)
	       CurrentContentsOfList(&cur_client->colormap_list);
	  
	  if(temp_cmap->colormap_id == cmap_id)
	       return(temp_cmap);
     }

     return NULL;
}

/* If the server is unable to provide all the distinct colors the client
   requests and thus begins mapping multiple colors to the same pixel,
   we tell the client that the pixel value is distinct, even though the
   color is not unique. This way if the client is later moved to a screen
   that can hold all the colors we can allocate them for the client.
   
   check if the client already thinks that there is a color with this
   pixel number and if so find a safe pixel number. In particular, it
   makes a "cycle". eg: if client pixel#8 is server pixel#10, and now
   the server creates pixel#8, the client is told it is pixel#10. */

ColorCellPtr
AllocNewColorCell(card32 pixel, ColormapPtr cmap, Bool read_write,
		  card16 red, card16 green, card16 blue, char *name)
{
     ColorCellPtr new_cell, temp_cell;
     card32 client_pixel, tmp_pixel;

     if (temp_cell = FindServerColorCellFromColormap(pixel, cmap)) {
	  if (read_write ||
	      (name && temp_cell->color_name &&
	       (strcmp(name, temp_cell->color_name) == 0)) ||
	      (!name && !temp_cell->color_name &&
	       temp_cell->red == red     &&
	       temp_cell->green == green &&
	       temp_cell->blue == blue))
	       
	       return temp_cell;	/* if already exists, don't add new cell */

	  client_pixel = 0;
	  while (client_pixel < cmap->cell_count &&
		 cmap->cell_array[client_pixel].usage_count)
	       client_pixel++;
	       
	  if (client_pixel == cmap->cell_count)
	       return temp_cell;
     } else {
	  /* if the client is already using colorcell#pixel, we must locate
	     a colorcell unused by the client. Start from the largest color
	     in the colormap and search backward. */
	  
	  if (FindColorCellFromColormap(pixel, cmap)) {
	       client_pixel = cmap->cell_count;
	       while (--client_pixel != ((card32) -1))
		    if (cmap->cell_array[client_pixel].usage_count == 0)
			 break;

	       if (client_pixel == ((card32) -1))
		    return &cmap->cell_array[0];
	  } else
	       client_pixel = pixel;
     }
     
     new_cell = &cmap->cell_array[client_pixel];
     new_cell->client_pixel = client_pixel;
     new_cell->server_pixel = pixel;
     new_cell->read_write = read_write;
     
     /* if cell is read/write, we don't set the colors yet */
     if (!read_write)
	  if (name)
	       new_cell->color_name = name;
	  else {
	       new_cell->red = red;
	       new_cell->green = green;
	       new_cell->blue = blue;
	  }

     return new_cell;
}

/* see xmove.h: now is a macro */

/*ColorCellPtr
FindColorCellFromColormap(card32 pixel, ColormapPtr cmap)
{
     ColorCellPtr temp_colorcell = &(cmap->cell_array[pixel]);
     
     if (temp_colorcell->usage_count)
	  return temp_colorcell;
     else
	  return (ColorCellPtr)NULL;
}*/

/* same as above, but assumes that "pixel" is from the server's perspective
   instead of the client's. */

ColorCellPtr
FindServerColorCellFromColormap(card32 pixel, ColormapPtr cmap)
{
     ColorCellPtr temp_colorcell = cmap->cell_array + pixel;

     /* First, guess that the server pixel is the same as the client pixel, and
	if they are then return colorcell right away. Useful for clients that
	have not yet been moved, or clients using a custom colormap. */

     if (temp_colorcell->usage_count && temp_colorcell->server_pixel == pixel)
	  return temp_colorcell;

     /* Start at the beginning, and scan every color */

     temp_colorcell = cmap->cell_array;
     
     while (temp_colorcell < cmap->cell_array + cmap->cell_count) {
	  if (temp_colorcell->usage_count && temp_colorcell->server_pixel == pixel)
	       return temp_colorcell;

	  temp_colorcell++;
     }

     return (ColorCellPtr)NULL;
}

ColorCellPtr
FindNewServerColorCellFromColormap(card32 pixel, ColormapPtr cmap)
{
     ColorCellPtr temp_colorcell = cmap->cell_array + pixel;

     if (temp_colorcell->usage_count && temp_colorcell->server_pixel == pixel)
	  return temp_colorcell;

     temp_colorcell = cmap->cell_array;

     while (temp_colorcell < cmap->cell_array + cmap->cell_count) {
	  if (temp_colorcell->usage_count && temp_colorcell->new_server_pixel == pixel)
	       return temp_colorcell;

	  temp_colorcell++;
     }

     return (ColorCellPtr)NULL;
}

void
RemoveColorCellFromColormap(card32 pixel, ColormapPtr cmap)
{
     ColorCellPtr cell = &cmap->cell_array[pixel];

     if (cell->color_name) {
	  free(cell->color_name);
	  cell->color_name = NULL;
     }
     
     if (cell->usage_count)
	  cell->usage_count--;
}

GCPtr
FindGCFromCurrentClient(GContext gc_id)
{
     Client *cur_client = FindClientFromBase(gc_id & ~(client->resource_mask));

     if (cur_client == NULL)
	  return NULL;

     return(hash_find(cur_client->gc_table, gc_id));
}

/* The following routines are used to parse the XSetWindowAttributes */
/* structure that is created for each window */

static void
SetBackgroundPixmap(WindowPtr window_ptr,
		    Pixmap background_pixmap)
{
    Dprintf(("setting background pixmap to 0x%x\n", 
	   background_pixmap))

    window_ptr->attributes.background_pixmap = background_pixmap;
    window_ptr->attributes_mask |= CWBackPixmap;
}

static void
SetBackgroundPixel(WindowPtr window_ptr,
		   unsigned long background_pixel)
{
    window_ptr->attributes.background_pixel = background_pixel;
    window_ptr->attributes_mask |= CWBackPixel;

    Dprintf(("setting background pixel to 0x%x.\n", 
	   background_pixel))
}

static void
SetBorderPixmap(WindowPtr window_ptr,
		Pixmap border_pixmap)
{
    window_ptr->attributes.border_pixmap = border_pixmap;
    window_ptr->attributes_mask |= CWBorderPixmap;

    Dprintf(("setting border pixmap to 0x%x.\n", border_pixmap))
}

static void
SetBorderPixel(WindowPtr window_ptr,
	       unsigned long border_pixel)
{
    window_ptr->attributes.border_pixel = border_pixel;
    window_ptr->attributes_mask |= CWBorderPixel;

    Dprintf(("setting border pixel to 0x%x.\n", border_pixel))
}

static void
SetBackingPixel(WindowPtr window_ptr,
		unsigned long backing_pixel)
{
     window_ptr->attributes.backing_pixel = backing_pixel;
     window_ptr->attributes_mask |= CWBackingPixel;

    Dprintf(("setting backing pixel to 0x%x.\n", backing_pixel))
}

static void
SetColormap(WindowPtr window_ptr,
	    Colormap colormap)
{
    window_ptr->attributes.colormap = colormap;
    window_ptr->attributes_mask |= CWColormap;
	    
    Dprintf(("setting colormap to 0x%x.\n", colormap))
}

static void
SetCursor(WindowPtr window_ptr,
	  Cursor cursor)
{
    window_ptr->attributes.cursor = cursor;
    window_ptr->attributes_mask |= CWCursor;
    
    Dprintf(("setting cursor to 0x%x.\n", cursor))
}

/* ParseWindowBitmaskValueList only stores SOME of the attributes
   that can be set for a window. In particular, it stores those attributes
   which cannot be obtained from GetGeometry and GetWindowAttributes. That
   currently includes Background Pixmap and Pixel, Border Pixmap and Pixel,
   and cursor. */

void
ParseWindowBitmaskValueList(WindowPtr window_ptr,
			    unsigned long cmask, 
			    unsigned char *ValueList)
{
     /* there are bits in the controlling bitmask, figure out which */
     /* the ctype is a set type, so this code is similar to PrintSET */
     
     if (cmask & CWBackPixmap)
     {
	  /* background pixmap */
	  Pixmap pixmap = (Pixmap) ILong(ValueList);
	  SetBackgroundPixmap(window_ptr, pixmap);
	  
	  cmask = cmask & ~CWBackPixmap;
	  ValueList += 4;
     }
     
     if (cmask & CWBackPixel)
     {
	  /* background pixel value */
	  unsigned long pixel_value = ILong(ValueList);
	  SetBackgroundPixel(window_ptr, pixel_value);
	  
	  cmask = cmask & ~CWBackPixel;
	  ValueList += 4;
     }
     
     if (cmask & CWBorderPixmap)
     {
	  /* pixmap for the border */
	  Pixmap pixmap = ILong(ValueList);
	  SetBorderPixmap(window_ptr, pixmap);
	  
	  cmask = cmask & ~CWBorderPixmap;
	  ValueList += 4;
     }
     
     if (cmask & CWBorderPixel)
     {
	  /* border color */
	  unsigned long border_pixel = ILong(ValueList);
	  SetBorderPixel(window_ptr, border_pixel);
	  
	  cmask = cmask & ~CWBorderPixel;
	  ValueList += 4;
     }
     
     
     if (cmask & CWBitGravity)
	  ValueList += 4;
     
     if (cmask & CWWinGravity)
	  ValueList += 4;
     
     if (cmask & CWBackingStore)
	  ValueList += 4;
     
     if (cmask & CWBackingPlanes)
	  ValueList += 4;
     
     if (cmask & CWBackingPixel) {
	  SetBackingPixel(window_ptr, ILong(ValueList));
	  
	  cmask = cmask & ~CWBackingPixel;
	  ValueList += 4;
     }
     
     if (cmask & CWSaveUnder)
	  ValueList += 4;
     
     if (cmask & CWEventMask)
	  ValueList += 4;
     
     if (cmask & CWDontPropagate)
	  ValueList += 4;
     
     if (cmask & CWOverrideRedirect)
	  ValueList += 4;
     
     if (cmask & CWColormap)
       {
	 Colormap colormap  = ILong(ValueList);
	 SetColormap(window_ptr, colormap);

	 if ((colormap & ~client->resource_mask) == 0)
	      window_ptr->cmap = (ColormapPtr)client->colormap_list.top->contents;
	 else
	      window_ptr->cmap = FindColormapFromCurrentClient(colormap);
	 
	 cmask = cmask & ~CWColormap;
	 ValueList += 4;
       }
	
     if (cmask & CWCursor)
     {
	  Cursor cursor = ILong(ValueList);
	  SetCursor(window_ptr, cursor);
	  
	  cmask = cmask & ~CWCursor;
	  ValueList += 4;
     }
}

WindowPtr
FindWindowFromCurrentClient(Window xwin)
{
    WindowPtr temp_window;
    Client *cur_client = FindClientFromBase(xwin & ~(client->resource_mask));

    if (cur_client == NULL)
	 return NULL;
    
    return(hash_find(cur_client->window_table, xwin));
}

PixmapPtr
FindPixmapFromCurrentClient(Pixmap pixmap)
{
     Client *cur_client = FindClientFromBase(pixmap & ~(client->resource_mask));

     if (cur_client == NULL)
	 return NULL;

     return(hash_find(cur_client->pixmap_table, pixmap));
}


void
TagWindow(WindowPtr window, short value)
{
  window->tag = value;
}

void
UntagWindow(WindowPtr window)
{
  window->tag = 0;
}


void
UntagAllWindows()
{
     WindowPtr window;
     hash_location loc;

     window = hashloc_init(&loc, client->window_table);
     while (window) {
	  window->tag = 0;
	  window = hashloc_getnext(&loc);
     }
}

void
DestroyTaggedWindows()
{
     WindowPtr window;
     hash_location loc;

     window = hashloc_init(&loc, client->window_table);
     while (window) {
	  if (window->tag) {
	       RemoveGrabsUsingWindow(window->window_id);
	       free((char *)window);
	       window = hashloc_deletenext(&loc);
	  } else
	       window = hashloc_getnext(&loc);
     }
}

/* The X protocol relies on being able to do an operation to a family of
   windows, i.e. deleting a window and all of its descendants.  This is 
   conducive to a tree data structure, but X also relies on a stacking order
   for displaying windows, so it is imperative that we keep windows in the
   order that they were created in.

   To do both of these with the smallest effort, a singly linked list of
   windows is kept, each containing the id of thier parent.  There is also a
   field for marking windows on which we would like an operation (such as
   delete) to be performed.  This is necessary because if we were to try and
   do a delete without first tagging windows, we would delete critical
   information before it had been used to find more descendants.

   TagAllDescendants is recursive. It takes a parent ID and must mark all
   of its children. So it scans through all windows. If a window has the
   parameter as its parent, it is tagged and TagAllDescendants is called
   recursively for this newly tagged window's children.
*/

void DoTagAllDescendants(Window parent_id);
void TagAllDescendants(Window parent_id)
{
  /* start by untagging all of the windows */
  UntagAllWindows();
  DoTagAllDescendants(parent_id);
}

void DoTagAllDescendants(Window parent_id)
{
     WindowPtr window;
     hash_location loc;

     window = hashloc_init(&loc, client->window_table);
     while (window) {
	  if (window->parent_id == parent_id && !window->tag) {
	       window->tag = 1;
	       DoTagAllDescendants(window->window_id);
	  }
	  window = hashloc_getnext(&loc);
     }
}


/* routines need to manipulate the window structure */

static void
SetFunction(GCPtr gc_ptr, int function)
{
    Dprintf(("setting function to 0x%x\n", function))

    gc_ptr->values.function = function;
    gc_ptr->values_mask |= GCFunction;
}

static void
SetPlaneMask(GCPtr gc_ptr, unsigned long plane_mask)
{
    Dprintf(("setting plane mask to 0x%x\n", plane_mask))

    gc_ptr->values.plane_mask = plane_mask;
    gc_ptr->values_mask |= GCPlaneMask;
}

static void
SetForeground(GCPtr gc_ptr, unsigned long foreground)
{
    Dprintf(("setting foreground to 0x%x\n", foreground))

    gc_ptr->values.foreground = foreground;
    gc_ptr->values_mask |= GCForeground;
}

static void
SetBackground(GCPtr gc_ptr, unsigned long background)
{
    Dprintf(("setting background to 0x%x\n", background))

    gc_ptr->values.background = background;
    gc_ptr->values_mask |= GCBackground;
}

static void
SetLineWidth(GCPtr gc_ptr, int line_width)
{
    Dprintf(("setting line_width to 0x%x\n", line_width))

    gc_ptr->values.line_width = line_width;
    gc_ptr->values_mask |= GCLineWidth;
}

static void
SetLineStyle(GCPtr gc_ptr, int line_style)
{
    Dprintf(("setting line_style to 0x%x\n", line_style))

    gc_ptr->values.line_style = line_style;
    gc_ptr->values_mask |= GCLineStyle;
}

static void
SetCapStyle(GCPtr gc_ptr, int cap_style)
{
    Dprintf(("setting cap style to 0x%x\n", cap_style))

    gc_ptr->values.cap_style = cap_style;
    gc_ptr->values_mask |= GCCapStyle;
}

static void
SetJoinStyle(GCPtr gc_ptr, int join_style)
{
    Dprintf(("setting join style to 0x%x\n", join_style))

    gc_ptr->values.join_style = join_style;
    gc_ptr->values_mask |= GCJoinStyle;
}

static void
SetFillStyle(GCPtr gc_ptr, int fill_style)
{
    Dprintf(("setting fill style to 0x%x\n", fill_style))

    gc_ptr->values.fill_style = fill_style;
    gc_ptr->values_mask |= GCFillStyle;
}

static void
SetFillRule(GCPtr gc_ptr, int fill_rule)
{
    Dprintf(("setting fill rule to 0x%x\n", fill_rule))

    gc_ptr->values.fill_rule = fill_rule;
    gc_ptr->values_mask |= GCFillRule;
}

static void
SetTile(GCPtr gc_ptr, Pixmap tile)
{
    Dprintf(("setting tile to 0x%x\n", tile))

    gc_ptr->values.tile = tile;
    gc_ptr->values_mask |= GCTile;
}

static void
SetStipple(GCPtr gc_ptr, Pixmap stipple)
{
    Dprintf(("setting stipple to 0x%x\n", stipple))
    gc_ptr->values.stipple = stipple;
    gc_ptr->values_mask |= GCStipple;
}

static void
SetTileStipXOrigin(GCPtr gc_ptr, int ts_x_origin)
{
    Dprintf(("setting tile/stipple X origin to 0x%x\n", ts_x_origin))

    gc_ptr->values.ts_x_origin = ts_x_origin;
    gc_ptr->values_mask |= GCTileStipXOrigin;
}

static void
SetTileStipYOrigin(GCPtr gc_ptr, int ts_y_origin)
{
    Dprintf(("setting Tile/Stipple Y origin to 0x%x\n", ts_y_origin))

    gc_ptr->values.ts_y_origin = ts_y_origin;
    gc_ptr->values_mask |= GCTileStipYOrigin;
}

static void
SetFont(GCPtr gc_ptr, int font)
{
    Dprintf(("setting font to 0x%x\n", font))
    gc_ptr->values.font = font;
    gc_ptr->values_mask |= GCFont;
}

static void
SetSubwindowMode(GCPtr gc_ptr, int subwindow_mode)
{
    Dprintf(("setting subwindow mode to 0x%x\n", subwindow_mode))
    gc_ptr->values.subwindow_mode = subwindow_mode;
    gc_ptr->values_mask |= GCSubwindowMode;
}

static void
SetGraphicsExposures(GCPtr gc_ptr, int graphics_exposures)
{ 
    Dprintf(("setting graphical exposures to 0x%x\n",
	   graphics_exposures))

    gc_ptr->values.graphics_exposures = graphics_exposures;
    gc_ptr->values_mask |= GCGraphicsExposures;
}

static void
SetClipXOrigin(GCPtr gc_ptr, int clip_x_origin)
{
    Dprintf(("setting clip_x_origin to 0x%x\n", clip_x_origin))

    gc_ptr->values.clip_x_origin = clip_x_origin;
    gc_ptr->values_mask |= GCClipXOrigin;
}

static void
SetClipYOrigin(GCPtr gc_ptr, int clip_y_origin)
{
    Dprintf(("setting clip y origin to 0x%x\n", clip_y_origin))
    gc_ptr->values.clip_y_origin = clip_y_origin;
    gc_ptr->values_mask |= GCClipYOrigin;
}

static void
SetClipMask(GCPtr gc_ptr, Pixmap clip_mask)
{
    Dprintf(("setting clip mask to 0x%x\n", clip_mask))
    gc_ptr->values.clip_mask = clip_mask;
    gc_ptr->values_mask |= GCClipMask;
}


static void
SetDashOffset(GCPtr gc_ptr, int dash_offset)
{
    Dprintf(("setting dash offset to 0x%x\n", dash_offset))
    gc_ptr->values.dash_offset = dash_offset;
    gc_ptr->values_mask |= GCDashOffset;
}

static void
SetDashList(GCPtr gc_ptr, char dashes)
{
    Dprintf(("setting dashes to 0x%x\n", dashes))
    gc_ptr->values.dashes = dashes;
    gc_ptr->values_mask |= GCDashList;
}

static void
SetArcMode(GCPtr gc_ptr, int arc_mode)
{
    Dprintf(("setting arc mode to 0x%x\n", arc_mode))
    gc_ptr->values.arc_mode = arc_mode;
    gc_ptr->values_mask |= GCArcMode;
}

void
ParseGCBitmaskValueList(GCPtr gc_ptr,
			unsigned long cmask, 
			unsigned char *ValueList)
{
    Dprintf(("parsing info for GC 0x%x\n", gc_ptr->gc_id))

    if (cmask & GCFunction)
      {
	/* drawing function */
	int function = (int) ILong(ValueList);
	SetFunction(gc_ptr, function);
	
	cmask = cmask & ~GCFunction;
	ValueList += 4;
      }
    
    if (cmask & GCPlaneMask)
      {
	/* plane mask */
	unsigned long plane_mask = (unsigned long) ILong(ValueList);
	SetPlaneMask(gc_ptr, plane_mask);
	
	cmask = cmask & ~GCPlaneMask;
	ValueList += 4;
      }
    
    if (cmask & GCForeground)
      {
	unsigned long foreground = (unsigned long) ILong(ValueList);
	SetForeground(gc_ptr, foreground);
	
	cmask = cmask & ~GCForeground;
	ValueList += 4;
      }
    
    if (cmask & GCBackground)
      {
	unsigned long background = (unsigned long) ILong(ValueList);
	SetBackground(gc_ptr, background);
	
	cmask = cmask & ~GCBackground;
	ValueList += 4;
      }
    
    if (cmask & GCLineWidth)
      {
	int line_width = (int) ILong(ValueList);
	SetLineWidth(gc_ptr, line_width);
	
	cmask = cmask & ~GCLineWidth;
	ValueList += 4;
      }
    
    if (cmask & GCLineStyle)
      {
	int line_style = (int) ILong(ValueList);
	SetLineStyle(gc_ptr, line_style);
	
	cmask = cmask & ~GCLineStyle;
	ValueList += 4;
      }
    
    if (cmask & GCCapStyle)
      {
	int cap_style = (int) ILong(ValueList);
	SetCapStyle(gc_ptr, cap_style);
	
	cmask = cmask & ~GCCapStyle;
	ValueList += 4;
      }
    
    if (cmask & GCJoinStyle)
      {
	int join_style = (int) ILong(ValueList);
	SetJoinStyle(gc_ptr, join_style);
	
	cmask = cmask & ~GCJoinStyle;
	ValueList += 4;
      }
    
    if (cmask & GCFillStyle)
      {
	int fill_style = (int) ILong(ValueList);
	SetFillStyle(gc_ptr, fill_style);
	
	cmask = cmask & ~GCFillStyle;
	ValueList += 4;
      }
    
    if (cmask & GCFillRule)
      {
	int fill_rule = (int) ILong(ValueList);
	SetFillRule(gc_ptr, fill_rule);
	
	cmask = cmask & ~GCFillRule;
	ValueList += 4;
      }
    
    if (cmask & GCTile)
      {
	Pixmap tile = (Pixmap) ILong(ValueList);
	SetTile(gc_ptr, tile);
	
	cmask = cmask & ~GCTile;
	ValueList += 4;
      }
    
    if (cmask & GCStipple)
      {
	Pixmap stipple = (Pixmap) ILong(ValueList);
	SetStipple(gc_ptr, stipple);
	
	cmask = cmask & ~GCStipple;
	ValueList += 4;
      }
    
    if (cmask & GCTileStipXOrigin)
      {
	int ts_x_origin = (int) ILong(ValueList);
	SetTileStipXOrigin(gc_ptr, ts_x_origin);
	
	cmask = cmask & ~GCTileStipXOrigin;
	ValueList += 4;
      }
    
    if (cmask & GCTileStipYOrigin)
      {
	int ts_y_origin = (int) ILong(ValueList);
	SetTileStipYOrigin(gc_ptr, ts_y_origin);
	
	cmask = cmask & ~GCTileStipYOrigin;
	ValueList += 4;
      }
    
    if (cmask & GCFont)
      {
	int font = (int) ILong(ValueList);
	SetFont(gc_ptr, font);
	
	cmask = cmask & ~GCFont;
	ValueList += 4;
      }
    
    if (cmask & GCSubwindowMode)
      {
	int subwindow_mode = (int) ILong(ValueList);
	SetSubwindowMode(gc_ptr, subwindow_mode);
	
	cmask = cmask & ~GCSubwindowMode;
	ValueList += 4;
      }
    
    if (cmask & GCGraphicsExposures)
      {
	/* graphic exposures really should be a Bool, but there is */
	/* going to be a type conflict with Bool, since xmond */
	/* defines a Bool as a short, but XLib defines it as an */
	/* int */
	
	int graphics_exposures = (int) ILong(ValueList);
	SetGraphicsExposures(gc_ptr, graphics_exposures);
	
	cmask = cmask & ~GCGraphicsExposures;
	ValueList += 4;
      }
    
    if (cmask & GCClipXOrigin)
      {
	int clip_x_origin = (int) ILong(ValueList);
	SetClipXOrigin(gc_ptr, clip_x_origin);
	
	cmask = cmask & ~GCClipXOrigin;
	ValueList += 4;
      }
    
    if (cmask & GCClipYOrigin)
      {
	int clip_y_origin = (int) ILong(ValueList);
	SetClipYOrigin(gc_ptr, clip_y_origin);
	
	cmask = cmask & ~GCClipYOrigin;
	ValueList += 4;
      }
    
    if (cmask & GCClipMask)
      {
	Pixmap clip_mask = (Pixmap) ILong(ValueList);
	SetClipMask(gc_ptr, clip_mask);
	
	cmask = cmask & ~GCClipMask;
	ValueList += 4;
      }
    
    if (cmask & GCDashOffset)
      {
	int dash_offset = (int) ILong(ValueList);
	SetDashOffset(gc_ptr, dash_offset);
	
	cmask = cmask & ~GCDashOffset;
	ValueList += 4;
      }
    
    if (cmask & GCDashList)
      {
	char dashes = (char) ILong(ValueList);
	SetDashList(gc_ptr, dashes);
	
	cmask = cmask & ~GCDashList;
	ValueList += 4;
      }
    
    if (cmask & GCArcMode)
      {
	int arc_mode = (int) ILong(ValueList);
	SetArcMode(gc_ptr, arc_mode);
	
	cmask = cmask & ~GCArcMode;
	ValueList += 4;
      }
    
    /* one pass through this while loop should be all that is */
    /* needed..  If the mask variable is not zero at this point, */
    /* something is wrong */ 
    
    if (cmask != 0)
      {
	Dprintf(("ERROR: ParseGCBitmaskValueList() routine.\n"))
        Dprintf(("CMASK should be 0 but is 0x%x\n",cmask))
	return;
      }
}

void
ParseGCBitmaskcopy(GCPtr gc_ptr_src, GCPtr gc_ptr_dst, unsigned long cmask)
{
  Dprintf(("parsing info for GC copy from #0x%x to #0x%x\n", 
	   gc_ptr_dst->gc_id, gc_ptr_src->gc_id))
  
  /* this code is borrowed from ParseWindowBitmaskValueList() */
  
  if (cmask & GCFunction)
    SetFunction(gc_ptr_dst, gc_ptr_src->values.function);
  
  if (cmask & GCPlaneMask)
    SetPlaneMask(gc_ptr_dst, gc_ptr_src->values.plane_mask);
  
  if (cmask & GCForeground)
    SetForeground(gc_ptr_dst, gc_ptr_src->values.foreground);
  
  if (cmask & GCBackground)
    SetBackground(gc_ptr_dst, gc_ptr_src->values.background);
  
  if (cmask & GCLineWidth)
    SetLineWidth(gc_ptr_dst, gc_ptr_src->values.line_width);
  
  if (cmask & GCLineStyle)
    SetLineStyle(gc_ptr_dst, gc_ptr_src->values.line_style);
  
  if (cmask & GCCapStyle)
    SetCapStyle(gc_ptr_dst, gc_ptr_src->values.cap_style);
  
  if (cmask & GCJoinStyle)
    SetJoinStyle(gc_ptr_dst, gc_ptr_src->values.join_style);
  
  if (cmask & GCFillStyle)
    SetFillStyle(gc_ptr_dst, gc_ptr_src->values.fill_style);
  
  if (cmask & GCFillRule)
    SetFillRule(gc_ptr_dst, gc_ptr_src->values.fill_rule);

  if (cmask & GCTile)
    SetTile(gc_ptr_dst, gc_ptr_src->values.tile);
  
  if (cmask & GCStipple)
    SetStipple(gc_ptr_dst, gc_ptr_src->values.stipple);
  
  if (cmask & GCTileStipXOrigin)
    SetTileStipXOrigin(gc_ptr_dst, gc_ptr_src->values.ts_x_origin);
  
  if (cmask & GCTileStipYOrigin)
    SetTileStipYOrigin(gc_ptr_dst, gc_ptr_src->values.ts_y_origin);
  
  if (cmask & GCFont)
    SetFont(gc_ptr_dst, gc_ptr_src->values.font);
  
  if (cmask & GCSubwindowMode)
    SetSubwindowMode(gc_ptr_dst, gc_ptr_src->values.subwindow_mode);
  
  if (cmask & GCGraphicsExposures)
    SetGraphicsExposures(gc_ptr_dst, gc_ptr_src->values.graphics_exposures);
  
  if (cmask & GCClipXOrigin)
    SetClipXOrigin(gc_ptr_dst, gc_ptr_src->values.clip_x_origin);
  
  if (cmask & GCClipYOrigin)
    SetClipYOrigin(gc_ptr_dst, gc_ptr_src->values.clip_y_origin);
  
  if (cmask & GCClipMask)
    SetClipMask(gc_ptr_dst, gc_ptr_src->values.clip_mask);
  
  if (cmask & GCDashOffset)
    SetDashOffset(gc_ptr_dst, gc_ptr_src->values.dash_offset);
  
  if (cmask & GCDashList)
    SetDashList(gc_ptr_dst, gc_ptr_src->values.dashes);
  
  if (cmask & GCArcMode)
    SetArcMode(gc_ptr_dst, gc_ptr_src->values.arc_mode);

}



void
MapGCBitmaskValueList(GCPtr gc_ptr,
			unsigned long cmask, 
			unsigned char *ValueList)
{
     ColormapPtr cmap = gc_ptr->cmap;

  Dprintf(("mapping info for GC 0x%x\n", gc_ptr->gc_id))
  
  if (cmask & GCFunction)
    {
      cmask = cmask & ~GCFunction;
      ValueList += 4;
    }
  
  if (cmask & GCPlaneMask)
    {
      cmask = cmask & ~GCPlaneMask;
      ValueList += 4;
    }
  
  if (cmask & GCForeground)
    {
      cmask = cmask & ~GCForeground;
      ISetLong(ValueList,
	       MapColorCell(ILong(ValueList), cmap, Request));

      ValueList += 4;
    }
  
  if (cmask & GCBackground)
    {
      cmask = cmask & ~GCBackground;
      ISetLong(ValueList,
	       MapColorCell(ILong(ValueList), cmap, Request));

      ValueList += 4;
    }
  
  if (cmask & GCLineWidth)
    {
      cmask = cmask & ~GCLineWidth;
      ValueList += 4;
    }
  
  if (cmask & GCLineStyle)
    {
      cmask = cmask & ~GCLineStyle;
      ValueList += 4;
    }
  
  if (cmask & GCCapStyle)
    {
      cmask = cmask & ~GCCapStyle;
      ValueList += 4;
    }
  
  if (cmask & GCJoinStyle)
    {
      cmask = cmask & ~GCJoinStyle;
      ValueList += 4;
    }
  
  if (cmask & GCFillStyle)
    {
      cmask = cmask & ~GCFillStyle;
      ValueList += 4;
    }
  
  if (cmask & GCFillRule)
    {
      cmask = cmask & ~GCFillRule;
      ValueList += 4;
    }
  
  if (cmask & GCTile)
    {
      /* Map the pixmap in the value list */
      ISetLong(ValueList, MapPixmapID(ILong(ValueList), Request));
      
      cmask = cmask & ~GCTile;
      ValueList += 4;
    }
  
  if (cmask & GCStipple)
    {
      /* Map the pixmap in the value list */
      ISetLong(ValueList, MapPixmapID(ILong(ValueList), Request));

      cmask = cmask & ~GCStipple;
      ValueList += 4;
    }
  
  if (cmask & GCTileStipXOrigin)
    {
      cmask = cmask & ~GCTileStipXOrigin;
      ValueList += 4;
    }
  
  if (cmask & GCTileStipYOrigin)
    {
      cmask = cmask & ~GCTileStipYOrigin;
      ValueList += 4;
    }
  
  if (cmask & GCFont)
    {
      ISetLong(ValueList, MapFontID(ILong(ValueList), Request));	    
      
      cmask = cmask & ~GCFont;
      ValueList += 4;
    }
  
  if (cmask & GCSubwindowMode)
    {
      cmask = cmask & ~GCSubwindowMode;
      ValueList += 4;
    }
  
  if (cmask & GCGraphicsExposures)
    {
      cmask = cmask & ~GCGraphicsExposures;
      ValueList += 4;
    }
  
  if (cmask & GCClipXOrigin)
    {
      cmask = cmask & ~GCClipXOrigin;
      ValueList += 4;
    }
  
  if (cmask & GCClipYOrigin)
    {
      cmask = cmask & ~GCClipYOrigin;
      ValueList += 4;
    }
  
  if (cmask & GCClipMask)
    {
      if (ILong(ValueList))
	   ISetLong(ValueList, MapPixmapID(ILong(ValueList), Request));
      
      cmask = cmask & ~GCClipMask;
      ValueList += 4;
    }
  
  if (cmask & GCDashOffset)
    {
      cmask = cmask & ~GCDashOffset;
      ValueList += 4;
    }
  
  if (cmask & GCDashList)
    {
      cmask = cmask & ~GCDashList;
      ValueList += 4;
    }
  
  if (cmask & GCArcMode)
    {
      cmask = cmask & ~GCArcMode;
      ValueList += 4;
    }
}



void
MapWindowBitmaskValueList(WindowPtr window_ptr,
			    unsigned long cmask, 
			    unsigned char *ValueList)
{
     ColormapPtr cmap = window_ptr->cmap;

     if ((cmask & CWColormap) &&
	 ((cmask & CWBackPixel) || (cmask & CWBackingPixel) || (cmask & CWBorderPixel)))
     {
	  /* we're screwed. this is about to change the colormap on us, but we have to
	     map the colors before we get to the new colormap. So skip ahead, find the
	     new colormap, and use it. */

	  unsigned char *CmapValue = ValueList;
	  unsigned int attrmask;
	  Colormap colormap;

	  attrmask = (cmask & (CWColormap-1));
	  while (attrmask) {
	       if (attrmask & 1)
		    CmapValue += 4;
	       attrmask >>= 1;
	  }

	  colormap = ILong(CmapValue);

	  if ((colormap == 0) ||
	      ((cmap = FindColormapFromCurrentClient(colormap)) == NULL))
	       cmap = (ColormapPtr)client->colormap_list.top->contents;
     }

  /* there are bits in the controlling bitmask, figure out which */
  /* the ctype is a set type */
  
  if (cmask & CWBackPixmap)
    {
      /* background pixmap */
      if (ILong(ValueList) > 1)
	   ISetLong(ValueList, MapPixmapID(ILong(ValueList), Request));
      
      cmask = cmask & ~CWBackPixmap;
      ValueList += 4;
    }
  
  if (cmask & CWBackPixel)
    {
      cmask = cmask & ~CWBackPixel;
      ISetLong(ValueList,
	       MapColorCell(ILong(ValueList), cmap, Request));
      ValueList += 4;
    }
  
  if (cmask & CWBorderPixmap)
    {
      /* pixmap for the border */
      if (ILong(ValueList))
	   ISetLong(ValueList, MapPixmapID(ILong(ValueList), Request));
      
      cmask = cmask & ~CWBorderPixmap;
      ValueList += 4;
    }
  
  if (cmask & CWBorderPixel)
    {
      cmask = cmask & ~CWBorderPixel;
      ISetLong(ValueList,
	       MapColorCell(ILong(ValueList), cmap, Request));
      ValueList += 4;
    }
  
  if (cmask & CWBitGravity)
    {
      cmask = cmask & ~CWBitGravity;
      ValueList += 4;
    }
  
  if (cmask & CWWinGravity)
    {
      cmask = cmask & ~CWWinGravity;
      ValueList += 4;
    }
  
  if (cmask & CWBackingStore)
    {
      cmask = cmask & ~CWBackingStore;
      ValueList += 4;
    }
  
  if (cmask & CWBackingPlanes)
    {
      cmask = cmask & ~CWBackingPlanes;
      ValueList += 4;

    }
  
  if (cmask & CWBackingPixel)
    {
      cmask = cmask & ~CWBackingPixel;
      ISetLong(ValueList,
	       MapColorCell(ILong(ValueList), cmap, Request));
      ValueList += 4;
    }
  
  if (cmask & CWSaveUnder)
    {
      cmask = cmask & ~CWSaveUnder;
      ValueList += 4;
    }
  
  if (cmask & CWEventMask)
    {
      cmask = cmask & ~CWEventMask;
      ValueList += 4;
    }
  
  if (cmask & CWDontPropagate)
    {
      cmask = cmask & ~CWDontPropagate;
      ValueList += 4;
    }
  
  if (cmask & CWOverrideRedirect)
    {
      cmask = cmask & ~CWOverrideRedirect;
      ValueList += 4;
    }
  
  if (cmask & CWColormap)
    {

      if (ILong(ValueList))
           ISetLong(ValueList, MapColormapID(ILong(ValueList), Request));

      cmask = cmask & ~CWColormap;
      ValueList += 4;
    }
  
  if (cmask & CWCursor)
    {
      if (ILong(ValueList))
	   ISetLong(ValueList, MapCursorID(ILong(ValueList), Request));
      
      cmask = cmask & ~CWCursor;
      ValueList += 4;
    }
  
  /* one pass through this while loop should be all that is */
  /* needed.. */ 
  
  if (cmask != 0)
    {
      Dprintf(("ERROR: MapWindowBitmaskValueList() routine.\n"))
      cmask = 0;
    }
}


Global char *
xmovemalloc(n)
     int n;
{
  char *ret_value;
  ret_value = malloc(n);
  fflush(stdout);
  return(ret_value);
}

Global char *
xmovecalloc(n, size)
     int n,size;
{
  char *ret_value;
  ret_value = calloc(n,size);
  fflush(stdout);
  return(ret_value);

}

/* SaveDepths is called with the server in which to save the information and the
   beginning of the screen from which the depths are to be found in a char pointer.
   The server is expected to have null information for its depths. */

void
SaveDepths(Server *server, unsigned char *buf)
{
     int depths = IByte(buf+39);
     int root_visualid = ILong(buf+32);

     buf += 40;
     while (depths--) {
	  int visuals = IShort(buf+2);
	  DepthPtr depth = Tmalloc(DepthRec);

	  depth->depth = IByte(buf);
	  Dprintf(("XMOVE: depth = %d\n",depth->depth));
	  initList(&depth->visual_list);
	  appendToList(&server->formats->depth_list, (Pointer) depth);

	  buf += 8;
	  while (visuals--) {
	       VisualPtr visual = Tmalloc(VisualRec);

	       visual->id               = ILong(buf);
	       visual->class            = IByte(buf+4);
	       visual->bits_per_rgb     = IByte(buf+5);
	       visual->colormap_entries = IShort(buf+6);
	       visual->red_mask         = ILong(buf+8);
	       visual->green_mask       = ILong(buf+12);
	       visual->blue_mask        = ILong(buf+16);

	       Dprintf(("XMOVE: id = %d, class = %d, bits_per_rgb = %d, colormap_entries = %d\n", visual->id,visual->class,visual->bits_per_rgb,visual->colormap_entries));
	       appendToList(&depth->visual_list, (Pointer) visual);
	       buf += 24;

	       if (visual->id == root_visualid) {
		    if (visual->class == StaticGray || visual->class == GrayScale)
			 server->color = False;
		    else
			 server->color = True;
	       }
	  }
     }
}

Global void
SaveFormats(Server *server, unsigned char *buf, int formats)
{
     while (formats--) {
	  PixmapFormatPtr format = Tmalloc(PixmapFormatRec);

	  format->depth          = IByte(buf);
	  format->bits_per_pixel = IByte(buf+1);
	  format->scanline_pad   = IByte(buf+2);

	  Dprintf(("XMOVE: depth = %d, bits_per_pixel = %d, scanline_pad = %d\n", format->depth, format->bits_per_pixel, format->scanline_pad));

	  appendToList(&server->formats->format_list, (Pointer) format);

	  buf += 8;
     }
}

/* we are guaranteed not to be called unless gc *needs* to be converted */

Global void
ConvertGCToColormap(GCPtr gc, ColormapPtr cmap, Bool *do_sync)
{
     xChangeGCReq req;
     unsigned char buf[8];
     card32 color;

     req.reqType = X_ChangeGC;
     ISetShort((unsigned char *)&req.length, 3);
     ISetLong((unsigned char *)&req.gc, gc->gc_id);

     if (gc->values_mask & GCForeground)
	  if ((color = MapColorCell(gc->values.foreground, cmap, Request)) != gc->values.foreground) {
	       ISetLong(&buf[0], color);
	       req.length++;
	  }

     if (gc->values_mask & GCBackground)
	  if ((color = MapColorCell(gc->values.background, cmap, Request)) != gc->values.background) {
	       if (req.length == 3)
		    ISetLong(&buf[0], color);
	       else
		    ISetLong(&buf[4], color);
	       req.length++;
	  }

     gc->cmap = cmap;

     if (req.length == 3)
	  return;
     
     if (*do_sync) {
	  synchronize_server_1();
	  synchronize_server_2();
	  *do_sync = False;
     }

     server->alt_seqno++;

     SendBuffer(server->alt_fd, (unsigned char *)&req, 12);
     SendBuffer(server->alt_fd, &buf[0], (req.length-3)*4);
}

Global card32 *
MakeServerColorCellMap(ColormapPtr cmap, int server_depth)
{
     card32 *map = malloc((1 << server_depth) * sizeof(card32));
     card32 *endmap = map + (1 << server_depth);
     card32 *curmap = map;
     int cnt = 0;


     while (curmap < endmap)
	  *(curmap++) = MapColorCell(cnt++, cmap, Reply);

     return map;
}
	  
/* as above, but equivalent of Map(Server2Server), not Map(Reply) */

Global card32 *
MakeServer2SColorCellMap(ColormapPtr cmap, int server_depth)
{
     card32 *map = malloc((1 << server_depth) * sizeof(card32));
     card32 *endmap = map + (1 << server_depth);
     card32 *curmap = map;
     int cnt = 0;


     while (curmap < endmap)
	  *(curmap++) = MapColorCell(cnt++, cmap, Reply);

     return map;
}
	  

Global void
ConvertPixmapToColormap(PixmapPtr pixmap, ColormapPtr cmap, Bool *do_sync)
{
     XImage *image;
     int x, y;
     struct _XGC gc;
     register ColorCellPtr cells;
     card32 *server_cell_map;

     gc.gid = server->resource_base;

     if (*do_sync) {
	  synchronize_server_1();
	  server_cell_map = MakeServerColorCellMap(pixmap->cmap, server->default_depth);
	  synchronize_server_2();
	  *do_sync = False;
     } else {
	  server_cell_map = MakeServerColorCellMap(pixmap->cmap, server->default_depth);
     }

     image = XMOVEGetImage(server->alt_fd, &server->alt_seqno, server,
			   MapPixmapID(pixmap->pixmap_id, Request),
			   0, 0, pixmap->width, pixmap->height,
			   0xFFFFFFFF, ZPixmap);

     /*
      * MapImage8Double does a double mapping in one loop through the image, but
      * for 16 and 32 bits_per_pixel I didn't write such a function. It probably
      * would be a good idea. Note that server_cell_map is being created even if
      * it isn't needed, which is true only in the 16 and 32 bit examples.
      */

     if (client->formats->flags & IMAGE_NBIT_SAFE) {
	  if (image->bits_per_pixel == 8)
	       MapImage8Double(image->data, pixmap->width, pixmap->height, pixmap->depth,
			       image->bytes_per_line, server_cell_map, cmap);
	  else if (image->bits_per_pixel == 16) {
	       MapImage16(image->data, pixmap->width, pixmap->height, pixmap->depth,
			  image->bytes_per_line, pixmap->cmap, Reply,
			  (server->formats->image_byte_order != XmoveLittleEndian),
			  (client->formats->image_byte_order != XmoveLittleEndian));

	       MapImage16(image->data, pixmap->width, pixmap->height, pixmap->depth,
			  image->bytes_per_line, cmap, Request,
			  (client->formats->image_byte_order != XmoveLittleEndian),
			  (server->formats->image_byte_order != XmoveLittleEndian));
	  } else if (image->bits_per_pixel == 32) {
	       MapImage32(image->data, pixmap->width, pixmap->height, pixmap->depth,
			  image->bytes_per_line, pixmap->cmap, Reply,
			  (server->formats->image_byte_order != XmoveLittleEndian),
			  (client->formats->image_byte_order != XmoveLittleEndian));
	       MapImage32(image->data, pixmap->width, pixmap->height, pixmap->depth,
			  image->bytes_per_line, cmap, Request,
			  (client->formats->image_byte_order != XmoveLittleEndian),
			  (server->formats->image_byte_order != XmoveLittleEndian));
	  } else
	       goto CP2C_DEFAULT;
     } else {
CP2C_DEFAULT:
	  cells = cmap->cell_array;
	  for (x=0; x < pixmap->width; x++) {
	       for (y=0; y < pixmap->height; y++) {
		    XPutPixel(image, x, y, cells[server_cell_map[XGetPixel(image, x, y)]].server_pixel);
	       }
	  }
     }

     XMOVEPutImage(server->alt_fd, &server->alt_seqno, server->formats,
		   MapPixmapID(pixmap->pixmap_id, Request), &gc, image,
		   0,0,0,0, pixmap->width, pixmap->height);

     pixmap->cmap = cmap;

     free(server_cell_map);

     free(image->data);
     free((char *) image);
}
