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
#include "libatommap.h"
#include <X11/Xutil.h>

/* programmer's note: it is not safe to change atom->transient_map_func
   while in MapChangeProp, MapGetPropReply, MapDeleteProp or MapGetProperty */

extern Bool ignore_bytes;

void
initialize()
{
     AddAtomMapping("WM_HINTS", WM_HINTS_map);
     AddAtomMapping("WM_NAME", WM_NAME_map);
     AddAtomMapping("_OL_WIN_ATTR", _OL_WIN_ATTR_map);
     AddAtomMapping("WM_PROTOCOLS", WM_PROTOCOLS_map);
     AddAtomMapping("_SUN_DRAGDROP_INTEREST", _SUN_DRAGDROP_INTEREST_map);
     AddAtomMapping("Wabi_CMAP", Wabi_CMAP_map);
     
     AddAtomTypeMapping("ATOM", ATOM_map);
     AddAtomTypeMapping("DRAWABLE", DRAWABLE_map);
     AddAtomTypeMapping("WINDOW", WINDOW_map);
     AddAtomTypeMapping("WM_PROTOCOLS", WM_PROTOCOLS_map);
     AddAtomTypeMapping("_SUN_DRAGDROP_SITE_RECTS", _SUN_DRAGDROP_SITE_RECTS_map);
     AddAtomTypeMapping("_SUN_DRAGDROP_TRIGGER", _SUN_DRAGDROP_TRIGGER_map);
     AddAtomTypeMapping("MULTIPLE", MULTIPLE_map);
}

static void WM_HINTS_map(req_type, property, type, selection, atom,
			 delete, window, data, length)
AtomMapType req_type;
Atom *property;
Atom *type;
Atom *selection;
AtomPtr atom;
Bool delete;
Window *window;
unsigned char *data;
int length;
{
     XWMHints *hints = (XWMHints *)data;
     Direction dir;
     
     if (length != 36)
	  return;
     
     switch (req_type) {
     case MapChangeProp:
	  dir = Request;
	  break;

     case MapGetPropReply:
	  dir = Reply;
	  break;

     case MapMoveClientProps:	/* If icon_window is set, fake an expose event to client */
	  dir = Server2Server;

	  /* This is an example of sending an event to a client in mid-move. client->SequenceMapping
	     holds the most recent sequence number used by the client. We create the event and then
	     enqueue it onto the outgoing buffer of the client. */
	  
	  if (hints->flags & IconWindowHint) {
	       unsigned char expose[32];
	       WindowPtr window = FindWindowFromCurrentClient(MapWindowID(hints->icon_window, Event));

	       if (window == NULL)
		    break;
	       
	       expose[0] = 12;
	       ISetShort(&expose[2], client->SequenceMapping);
	       ISetLong(&expose[4], window->window_id);
	       ISetShort(&expose[8], 0);
	       ISetShort(&expose[10], 0);
	       ISetShort(&expose[12], window->width);
	       ISetShort(&expose[14], window->height);
	       ISetShort(&expose[16], 0);
	       
	       SaveBytes(&client->fdd->outBuffer, &expose[0], 32);
	  }

	  break;

     default:
	  return;
     }
	  
     if (hints->flags & IconPixmapHint)
	  ISetLong((unsigned char *)&hints->icon_pixmap,  MapPixmapID(hints->icon_pixmap, dir));
     
     if (hints->flags & IconWindowHint) {
	  ISetLong((unsigned char *)&hints->icon_window,  MapWindowID(hints->icon_window, dir));
     }
     
     if (hints->flags & IconMaskHint)
	  ISetLong((unsigned char *)&hints->icon_mask,    MapPixmapID(hints->icon_mask, dir));
     
     if (hints->flags & WindowGroupHint)
	  ISetLong((unsigned char *)&hints->window_group, MapWindowID(hints->window_group, dir));
}

static void WM_NAME_map(req_type, property, type, selection,
			atom, delete, window, data, length)
AtomMapType req_type;
Atom *property;
Atom *type;
Atom *selection;
AtomPtr atom;
Bool delete;
Window *window;
unsigned char *data;
int length;
{
     if (req_type == MapChangeProp && !GetCurrentClientName())
	  SetCurrentClientName((char *)data, length);
}

static void _OL_WIN_ATTR_map(req_type, property, type, selection,
			     atom, delete, window, data, length)
AtomMapType req_type;
Atom *property;
Atom *type;
Atom *selection;
AtomPtr atom;
Bool delete;
Window *window;
unsigned char *data;
int length;
{
     Direction dir;
     
     switch (req_type) {
     case MapChangeProp:
	  dir = Request;
	  break;

     case MapGetPropReply:
	  dir = Reply;
	  break;

     case MapMoveClientProps:
	  dir = Server2Server;
	  break;

     default:
	  return;
     }
     
     if (length == 12) {
	  ISetLong(&data[0], MapAtom(ILong(&data[0]), dir));
	  ISetLong(&data[4], MapAtom(ILong(&data[4]), dir));
	  ISetLong(&data[8], MapAtom(ILong(&data[8]), dir));
     } else {
	  if (ILong(&data[0]) & 1)
	       ISetLong(&data[4], MapAtom(ILong(&data[4]), dir));
	  
	  if (ILong(&data[0]) & 2)
	       ISetLong(&data[8], MapAtom(ILong(&data[8]), dir));

	  if (ILong(&data[0]) & 4)
	       ISetLong(&data[12], MapAtom(ILong(&data[12]), dir));

	  if (ILong(&data[0]) & 8)
	       ISetLong(&data[16], MapAtom(ILong(&data[16]), dir));
     }
}

static void WM_PROTOCOLS_map(req_type, property, type, selection,
			     atom, delete, window, data, length)
AtomMapType req_type;
Atom *property;
Atom *type;
Atom *selection;
AtomPtr atom;
Bool delete;
Window *window;
unsigned char *data;
int length;
{
     Direction dir;
     
     switch (req_type) {
     case MapClientMessage:
	  ISetLong(data, MapAtom(ILong(data), Event));
	  return;
	  
     case MapGetPropReply:
	  dir = Reply;
	  break;

     case MapChangeProp:
	  dir = Request;
	  break;

     case MapMoveClientProps:
	  dir = Server2Server;
	  break;

     default:
	  return;
     }

     while (length) {
	  ISetLong(data, MapAtom(ILong(data), dir));
	  data += 4;
	  length -= 4;
     }

     return;
}

static void _SUN_DRAGDROP_INTEREST_map(req_type, property, type, selection,
				       atom, delete, window, data, length)
AtomMapType req_type;
Atom *property;
Atom *type;
Atom *selection;
AtomPtr atom;
Bool delete;
Window *window;
unsigned char *data;
int length;
{
     Direction dir;
     int sites, windows;

     if (length == 0)
	  return;
     
     switch (req_type) {
     case MapChangeProp:
	  dir = Request;
	  break;
	  
     case MapGetPropReply:
	  dir = Reply;
	  break;

     case MapMoveClientProps:
	  dir = Server2Server;
	  break;

     default:
	  return;
     }
     
     /* the data in this property contains window IDs in various
	places. The structure is essentially a list of sites,
	each of which contains a window ID and lists of rectangles
	and/or window IDs. All the window IDs need to be mapped */
     
     sites = ILong(data+4);
     data += 8;

     while (sites--) {
	  ISetLong(data, MapWindowID(ILong(data), dir));
	  data += 12;
	  if (ILong(data)) {	/* list of window IDs -- map them */
	       windows = ILong(data+4);
	       data += 8;
	       while (windows--) {
		    ISetLong(data, MapWindowID(ILong(data), dir));
		    data += 4;
	       }
	  } else {		/* list of rectangles -- skip over them */
	       data += 8 + ILong(data+4)*16;
	  }
     }
}

/* Because this function needs to be applied to transient properties, there
   must be some way to link this function into the transient property and to
   remove it once the client is done with it.

   When this function gets called by ConvertSelection we take the property,
   look up its atom, and set that atom to link into this function. When
   the client deletes that property in the future we remove ourselves.

   We have problems if the client tries to use this same property for
   different purposes on different windows, or if the client uses a
   property which is already linked into a mapping routine due to its
   normal purpose which is here being subverted (ie using WM_HINTS to
   store the site rects). C'est la vie. */

static void _SUN_DRAGDROP_SITE_RECTS_map(req_type, property, type, selection,
					 atom, delete, window, data, length)
AtomMapType req_type;
Atom *property;
Atom *type;
Atom *selection;
AtomPtr atom;
Bool delete;
Window *window;
unsigned char *data;
int length;
{
     Direction dir;
     AtomPtr prop_atom;
     
     switch (req_type) {
     case MapConvertSelection:
     case MapSelectionRequest:
	  FindAtom(*property)->transient_map_func = atom->type_map_func;
	  return;

     case MapChangeProp:
	  dir = Request;
	  break;
	  
     case MapGetPropReply:
	  dir = Reply;
	  break;

     case MapMoveClientProps:
	  dir = Server2Server;
	  break;

     default:
	  return;
     }

     while (length) {
	  ISetLong(&data[8], MapWindowID(ILong(&data[8]), dir));
	  data += 32;
	  length -= 32;
     }
}

static void _SUN_DRAGDROP_TRIGGER_map(req_type, property, type, selection,
				      atom, delete, window, data, length)
AtomMapType req_type;
Atom *property;
Atom *type;
Atom *selection;
AtomPtr atom;
Bool delete;
Window *window;
unsigned char *data;
int length;
{
     Direction dir;

     if (length == 0)
	  return;
     
     switch (req_type) {
     case MapClientMessage:
	  dir = Event;
	  break;

     case MapSendClientMessage:
	  dir = Request;
	  break;

     default:
	  return;
     }

     ISetLong(&data[0], MapAtom(ILong(&data[0]), dir));
}

static void DRAWABLE_map(req_type, property, type, selection, atom,
			 delete, window, data, length)
AtomMapType req_type;
Atom *property;
Atom *type;
Atom *selection;
AtomPtr atom;
Bool delete;
Window *window;
unsigned char *data;
int length;
{
     Direction dir;
     
     switch (req_type) {
     case MapChangeProp:
	  dir = Request;
	  break;

     case MapGetPropReply:
	  dir = Reply;
	  break;

     case MapMoveClientProps:
	  dir = Server2Server;
	  break;

     case MapConvertSelection:
     case MapSelectionRequest:
	  FindAtom(*property)->transient_map_func = atom->type_map_func;
	  return;
	  
     default:
	  return;
     }

     while (length) {
	  Drawable drw;

	  drw = ILong(data);
	  if (drw)
	       ISetLong(data, MapDrawable(ILong(data), dir));
	  data += 4;
	  length -= 4;
     }
}

static void WINDOW_map(req_type, property, type, selection, atom,
		       delete, window, data, length)
AtomMapType req_type;
Atom *property;
Atom *type;
Atom *selection;
AtomPtr atom;
Bool delete;
Window *window;
unsigned char *data;
int length;
{
     Direction dir;
     
     switch (req_type) {
     case MapChangeProp:
	  dir = Request;
	  break;

     case MapGetPropReply:
	  dir = Reply;
	  break;

     case MapMoveClientProps:
	  dir = Server2Server;
	  break;

     case MapConvertSelection:
     case MapSelectionRequest:
	  FindAtom(*property)->transient_map_func = atom->type_map_func;
	  return;
	  
     default:
	  return;
     }

     while (length) {
	  Window win;

	  win = ILong(data);
	  if (win)
	       ISetLong(data, MapWindowID(ILong(data), dir));
	  
	  data += 4;
	  length -= 4;
     }
}

static void ATOM_map(req_type, property, type, selection, atom,
			delete, window, data, length)
AtomMapType req_type;
Atom *property;
Atom *type;
Atom *selection;
AtomPtr atom;
Bool delete;
Window *window;
unsigned char *data;
int length;
{
     Direction dir;
     
     switch (req_type) {
     case MapChangeProp:
	  dir = Request;
	  break;

     case MapGetPropReply:
	  dir = Reply;
	  break;

     case MapMoveClientProps:
	  dir = Server2Server;
	  break;

     case MapConvertSelection:
     case MapSelectionRequest:
	  FindAtom(*property)->transient_map_func = atom->type_map_func;
	  return;
	  
     default:
	  return;
     }

     while (length) {
	  ISetLong(data, MapAtom(ILong(data), dir));
	  data += 4;
	  length -= 4;
     }
}


/* This was painful to write, but I couldn't think of a better way.
   When a ConvertSelection converts with target of type MULTIPLE,
   it should act as a series of ConvertSelections. So we must fake
   the calls to other mapping routines in here, just as if those
   multiple ConvertSelection calls went out.

   But that's not the worst. This ConvertSelection requires us to
   read the contents of the destination property and convert it,
   since it is a list of Atoms. The catch is that the contents of
   this property could have been stored by the prior request, or
   a hundred requests ago. We can't make any assumptions. This
   also means that the storing of the property may still be
   enqueued within xmove in the outBuffer of the server!

   We must clearly first call SendXmoveBuffer to send this request
   to the server, in case the ChangeProperty is in the outBuffer to
   the server. However we also need to read in the property back from
   the server, map the atoms, and store the result again.

   In other similar situations we just use server->alt_fd to send to
   our secondary connection to the server, but this creates a race
   condition (since buffers aren't flushed immediately upon a write)
   and our GetProperty request may be processed at the server before
   the ChangeProperty from the client. If we were guaranteed that
   there would be no contents to the property before this moment,
   we could keep repeating GetProperty until we actually got some data,
   but this is not the case.

   Our solution is to scan the outBuffer to the server for a relevant
   ChangeProperty. If we find one we get our data from the buffer.
   Otherwise we SendXmoveBuffer and then use alt_fd. This fouls us
   up under two conditions:

   1) Something in outBuffer has a side-effect of changing the desired
   property. This brings us back to the race condition.

   2) Something has already been sent, and we are unlucky enough for the
   buffer to still not be flushed, or the server to not have had a context
   switch yet. This is less likely than 1.

   We must live with the above. In most cases the ChangeProperty
   immediately precedes the ConvertSelection, and we will work in that
   case.
*/

static void MULTIPLE_map(req_type, property, type, selection,
			 atom, delete, window, data, length)
AtomMapType req_type;
Atom *property;
Atom *type;
Atom *selection;
AtomPtr atom;
Bool delete;
Window *window;
unsigned char *data;
int length;
{
     Atom actual_type;
     int actual_format;
     unsigned long bytesafter;
     Window map_window = MapWindowID(*window, Request);
     Atom map_property = MapAtom(*property, Request);
     Direction dir;
     Bool called_get_property = False;
     
     switch (req_type) {
     case MapSelectionRequest:
	  FindAtom(*property)->transient_map_func = atom->type_map_func;
	  return;

     case MapGetPropReply:
	  do_MULTIPLE_map(0, *window, length/4, data, True, MapSelectionRequest, Reply);
	  return;

     case MapChangeProp:
	  do_MULTIPLE_map(0, *window, length/4, data, False, 0, Request);
	  return;

     case MapConvertSelection:
	  /* If this is MapConvertSelection then we have to locate the
	     contents of the property MULTIPLE, as explained above. With
	     GetPropertyReply and ChangeProperty the information is in the
	     variables data and length. We will use the same fields to hold
	     the information in the case of ConvertSelection. */

	  synchronize_server_1();
	  synchronize_server_2();
	  ignore_bytes = True;

	  while (XMOVEGetProperty(server->alt_fd, &server->alt_seqno,
				  map_window, map_property, 0, 1000000, False,
				  AnyPropertyType, &actual_type, &actual_format,
				  &length, &bytesafter, &data) != Success);
	  
	  do_MULTIPLE_map(*selection, *window, length, data, True,
			  MapConvertSelection, Request);
	  
	  XMOVEChangeProperty(server->alt_fd, &server->alt_seqno, map_window,
			      map_property, actual_type, actual_format,
			      PropModeReplace, data, length);
	  
	  free(data);
	  FindAtom(*property)->transient_map_func = atom->type_map_func;
	  return;
     }
}

static void do_MULTIPLE_map(selection, window, length, data, call_mapfunc, maptype, dir)
Atom selection;
Window window;
int length;
unsigned char *data;
Bool call_mapfunc;
AtomMapType maptype;
Direction dir;
{
     Atom target_atom, dest_atom, new_target, new_dest, new_selection;
     AtomPtr target_atom_ptr;
     AtomMappingPtr map;
     Window new_window;

     while (length--) {
	  new_target = target_atom = ILong(data);
	  ISetLong(data, MapAtom(target_atom, dir));
	  data += 4;
	  
	  if (length-- == 0)	/* this should NEVER happen. atoms in pairs only */
	       return;

	  new_dest = dest_atom = ILong(data);
	  ISetLong(data, MapAtom(dest_atom, dir));
	  data += 4;

	  if (call_mapfunc && (dest_atom != 0)) {
	       if (dir == Request)
		    target_atom_ptr = FindAtom(target_atom);
	       else
		    target_atom_ptr = FindServerAtom(target_atom);
	       
	       new_selection = selection; /* any changes made by the mapping function to */
	       new_window = window;	     /* either selection or window are ignored. */
	       
	       if (target_atom_ptr->type_map_func) {
		    (target_atom_ptr->type_map_func->MapProperty)
			 (maptype, &new_dest, &new_target, &new_selection,
			  target_atom_ptr, 0, &new_window, NULL, 0);
		    
		    if (new_dest != dest_atom)
			 ISetLong(data-4, new_dest);
		    
		    if (new_target != target_atom)
			 ISetLong(data-8, new_target);
	       }
	  }
     }
}

static void Wabi_CMAP_map(req_type, property, type, selection,
			  atom, delete, window, data, length)
AtomMapType req_type;
Atom *property;
Atom *type;
Atom *selection;
AtomPtr atom;
Bool delete;
Window *window;
unsigned char *data;
int length;
{
     Colormap orig_cmap_id, cmap_id;
     ColorCellPtr cell;
     ColormapPtr cmap;
     int cell_count = (1 << server->default_depth);
     XColor def;
     Bool found_it = False;
     ResourceMappingPtr map;

     /* When we discover the ID of the colormap that Wabi will be (ab)using, we
	intercept it, recreate the identical colormap from alt_fd, and map the
	colormap ID into the ID created from alt_fd. On each new server the colormap
	will be recreated from the alt_fd. The only catch is that we aren't keeping
	track of what resources are created through alt_fd. As of the time this 
	function is being written, we are the only function to create resources from
	alt_fd. If that changes we may have to make a more sophisticated ID
	allocation routine. */

     /* We only need to act when the client gets a reply to a GetProperty on Wabi_CMAP */

     if (req_type != MapGetPropReply || length == 0)
	  return;

     /* Try to find alt_fd's entry in the resource_maps, add one if necessary. */

     ForAllInList(&client->resource_maps) {
	  map = (ResourceMappingPtr)CurrentContentsOfList(&client->resource_maps);
	  if (map->server_base == server->alt_resource_base) {
	       found_it == True;
	       break;
	  }
     }

     if (!found_it) {
	  map = Tcalloc(1, ResourceMappingRec);
	  map->client_base = map->server_base = server->alt_resource_base;
	  map->transient = False;
	  appendToList(&client->resource_maps, (Pointer)map);
     }
	  
     /* get the cmap id, change it to fit client expectations, find the id to fit
	the new server's expectations for alt_fd, and create the colormap. */

     orig_cmap_id = ILong(data);
     ISetLong(data, ((orig_cmap_id & server->resource_mask) | map->client_base));
     cmap_id = ((orig_cmap_id & server->resource_mask) | server->alt_resource_base);
     XMOVECreateColormap(server->alt_fd, &server->alt_seqno, cmap_id,
			 server->root_window, server->default_vid, AllocAll);

     cmap = Tcalloc(1, ColormapRec);

     cmap->colormap_id = cmap_id;
     cmap->visual_id = server->default_vid;
     cmap->cell_count = cell_count;
     cmap->cell_array = Tcalloc(cell_count, ColorCellRec);

     AddColormapToCurrentClient(cmap);

     /* For each pixel, Query the color, and store the results, both internally in
	ColorCells and in our new colormap. */

     def.pixel = cell_count;
     cell = &cmap->cell_array[cell_count];
     
     while (def.pixel) {
	  --def.pixel;
	  --cell;
	  
	  XMOVEQueryColor(server->alt_fd, &server->alt_seqno, orig_cmap_id, &def);
	  
	  cell->client_pixel = cell->server_pixel = def.pixel;
	  cell->read_write = True;
	  cell->usage_count = 1;
	  cell->red = def.red;
	  cell->green = def.green;
	  cell->blue = def.blue;

	  XMOVEStoreColor(server->alt_fd, &server->alt_seqno, cmap_id, &def);
     }
}
