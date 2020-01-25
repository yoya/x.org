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
#include <stdio.h>
#include "xmove.h"
#include "xmond.h"

extern Bool MoveInProgress;	/* True if a client is being moved */
extern LinkList client_list;
static ResourceMappingPtr MapResourceBase(unsigned long base);
static ResourceMappingPtr MapServerResourceBase(unsigned long base);

/*
 *
 * $author: pds $
 * $Date: 1993/02/02 09:26:30 $
 * $Locker: ethan $
 * $Revision: 1.2 $
 * $Log: map.c,v $
 * Revision 1.2  1993/02/02  09:26:30  pds
 * *** empty log message ***
 *
 * Revision 1.1  1992/10/13  22:25:59  pds
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
 *
 */

static unsigned long
MapID(WinID, D_Default, S_Base, S_Mask, D_Base)
unsigned long WinID, D_Default, S_Base, S_Mask, D_Base;
{
     int Win_ClientID = WinID & ~(S_Mask);

     if (Win_ClientID == 0)
	  return D_Default;
     else if (Win_ClientID == S_Base)
	  return (WinID & S_Mask) | D_Base;

#ifndef DONT_MAP_IDCONFLICT
	  /* This code deals with the case of: what happens if we */
	  /* switch a client to a new server, and the client's */
	  /* original ID is already in use by another client on */
	  /* the new server. Further, if this client pokes around */
	  /* on the new server, it will find resources with IDs */
	  /* it thought reserved for itself. We will, simply, map */
	  /* such resource IDs into the only other safe client, */
	  /* which is the client's current ID on the current */
	  /* server. This is an ID which it never sees. */

	  /* Note that if the client and server both think the */
	  /* client still has the same ID, then this case won't */
	  /* be entered, as the former case will be evaluated */
	  /* instead. */

/*     else if (Win_ClientID == D_Base)
	  return (WinID & S_Mask) | S_Base; */
#endif

     else
	  return WinID;
}

/* MapResource tries to be sophisticated about manipulating the client
   IDs of resources that pass through. The result of this is complexity and
   slowdown, however it is necessary to make clients work when they
   interact with other clients as they are moved from server to server.

   Information about resource mapping is stored in two locations. The
   base and mask information for the client's own connection are stored
   directly in the Client and Server structures. The Client structure
   contains what the client believes to be its own base and mask, and the
   Server structure contains what the server believes to be its own base
   and mask.
   
   As well, the Client structure has a list containing all of the
   resource IDs it has encountered on this server, as well as all clients
   which this client has shown "inside-knowledge" of.

   inside-knowledge means that the client referred to this second client
   without the server having ever sent an event or reply with its ID. This
   implies that the client is in communication with that client directly.
   When this happens we assume that the two clients are directly linked and
   must always travel together. They are then "joined" by a meta-client and
   are permanently linked.

   The reason why there are two locations for storage of mappings is
   that the vast majority of references will be to the client's own
   resources, so we speed up the common case by not searching the larger
   list.

   All mappings done by MapResource assume that the resource ID came
   either from the client (Request or Client2NewServer) or from the server
   (Event, Reply, Error or Server2Server). During a move of a client we
   often need to convert into the new server's format, but we never have to
   convert back from it, so we don't have any directions from the new
   server.

   If the ID is from the client, we first see if it is the client's own
   resource base, and if so we do the default mapping. Next we check the
   list and convert using any matching entry found there.  If the ID is of
   a completely new base, we compare it against the base of every client
   using this xmove which was started directly through this xmove (ie.
   xmoved_from_elsewhere is False). If we find a match, we assume
   "inside-knowledge" as described above and join the two clients via a
   meta-client. If the two clients are on different servers (possibly
   because one has been moved and the second one has just been started
   onto the original server) we move this one to the new server. This
   can be quite rude, and we do display a message, but it is necessary
   for some clients that spawn a new connection periodically throughout
   their existence. An example of such a client is ghostview, which
   is a postscript previewer. Whenever it needs to update the previewing
   window it starts a new connection to the server and does the drawing
   from there.

   NOTE: we assume that clients with "inside knowledge" of each other
   only share their own client bases. If client A tells client B about
   some client C that it found on a server, and then client B tries to
   use that base to refer to client C, we are stuck. The reference coming
   from B will be out-of-the-blue. We won't know whether B found this
   client base on the server or whether it was "inside-information". If
   the latter, the base client A sent might need to be mapped before being
   used at the server, and we won't know to map it when used by client
   B.

   NOTE: the forced move of a client described above is not implemented.
   It should be implemented, but is impractical based on the design of
   xmove. Future work.

   If we still can't find it then we have to give up as we don't know
   where the client ID has come from. We default to assuming that it is on
   the client's current server, and that it found out about it via some
   property or selection process which we don't know about. We add the
   mapping to the client's list, but we assume that it is transient and
   will not move to the new server with us.

   If the ID is from the server, we first see if it is already known,
   either in the client or in the client's list. If not, then the client is
   about to find out about another client on the server. This isn't a
   problem at move time because clients are transient and thus can
   disappear at any time -- after the client moves the client will just
   assume that the newly found client simply exited.

   However things aren't quite that simple: because the server and the
   client may use different bits of the ID to specify the client base, we
   can't necessarily just pass along the same base to the client. If some
   bits that are intended as clientID bits will now appear as resource bits
   we must make up a client number for this new client arbitrarily that
   fits our client's expectations.

   If the reverse problem happens, ie. some bits intended to specify the
   resource appear as clientID bits to our client, we fail. This situation is
   rare, because most servers leave more than enough room for the resource
   part of the ID, place the resource part at the low-end of the ID, and
   because XLib numbers resources counting from 0 on upwards.

   So this is fairly complicated and obtuse, but it should work, and
   hopefully by using the data stored locally in the client for most cases
   it shouldn't be too slow.
   
*/

Global unsigned long
MapResource(unsigned long resource, Direction dir, unsigned long primary)
{
     unsigned long base;
     unsigned long id;
     ResourceMappingPtr map;
     
     switch (dir) {
     case Request:
	  base = resource & ~(client->resource_mask);
	  id   = resource & client->resource_mask;
	  
	  if (base == client->resource_base)
	       return (id | server->resource_base);

	  if (base == 0)	/* owned by server/window-manager */
	       return primary;

	  return (id | MapResourceBase(base)->server_base);

     case Event:
     case Reply:
     case Error:
	  base = resource & ~(server->resource_mask);
	  id   = resource & server->resource_mask;

	  if (base == server->resource_base)
	       return (id | client->resource_base);

	  if (base == 0)
	       return primary;

	  return (id | MapServerResourceBase(base)->client_base);

     /* because Client2NewServer and Server2Server only happen during
	moves we should never have a base that can't be found in the
	lists (ie. there should be nothing new). Because of this we
	don't call the Map[Server]ResourceBase() functions and just
	look up in the list. If this impossibility should happen,
	however, we just return the default value passed in "primary".
	MapResourceBase depends on the fact that it is never called
	during a move. */

     case Client2NewServer:
	  base = resource & ~(client->resource_mask);
	  id   = resource & client->resource_mask;
	  
	  if (base == client->resource_base)
	       return (id | new_server->resource_base);

	  if (base == 0)
	       return primary;

	  if ((map = FindResourceBase(base)))
	       return (id | map->new_server_base);
	  else
	       return primary;

     case Server2Server:
	  base = resource & ~(server->resource_mask);
	  id   = resource & server->resource_mask;

	  if (base == server->resource_base)
	       return (id | new_server->resource_base);

	  if (base == 0)
	       return primary;

	  if ((map = FindServerResourceBase(base)))
	       return (id | map->new_server_base);
	  else
	       return primary;
     }
}


Global ResourceMappingPtr
FindServerResourceBase(unsigned long base)
{
     ResourceMappingPtr map;

     ForAllInList(&client->resource_maps) {
	  map = (ResourceMappingPtr) CurrentContentsOfList(&client->resource_maps);

	  if (map->server_base == base)
	       return map;
     }

     return NULL;
}


Global ResourceMappingPtr
FindResourceBase(unsigned long base)
{
     ResourceMappingPtr map;

     ForAllInList(&client->resource_maps) {
	  map = (ResourceMappingPtr) CurrentContentsOfList(&client->resource_maps);

	  if (map->client_base == base)
	       return map;
     }

     return NULL;
}


/* We are guaranteed never to be called during a move, thus we are
   mapping from server to client. */

static ResourceMappingPtr
MapServerResourceBase(unsigned long base)
{
     ResourceMappingPtr map;
     unsigned long client_base;
     
     /* check if base can be found in client's resource_maps */
 
     if (map = FindServerResourceBase(base))
	  return map;

     /* this base is new to this client. create new map */

     map = malloc(sizeof(ResourceMappingRec));
     map->server_base = base;
     
     /* If the new base meets one of the following two conditions,
	then it must be mapped to a newly generated base for the client:

	1) the base uses bits not in the client's resource mask
	2) the base already exists, from the client's perspective,
	   either as its own base or as a base in its resource_maps
	   list. */

     if (((base & ~(client->resource_mask)) != base) ||
	 (base == client->resource_base) ||
	 FindResourceBase(base))
     {
	  /* base isn't acceptable. To find an alternative, start at
	     the maximum allowable base and decrement the base until
	     we find a base which the client hasn't heard of. */
	  
	  client_base = ~client->resource_mask;
	  while (client->resource_base == client_base || FindResourceBase(client_base))
	       client_base = DecrementResourceBase(client_base, client->resource_mask);
	  
	  map->client_base = client_base;
     } else
	  map->client_base = base;

     map->transient = True;
     appendToList(&client->resource_maps, (Pointer)map);
     return map;
}


/* We are guaranteed never to be called during a move, thus we are
   mapping from client to server. */

static ResourceMappingPtr
MapResourceBase(unsigned long base)
{
     ResourceMappingPtr map;
     unsigned long server_base;
     LinkLeaf *ll;
     Client *cur_client;
     
     /* check if base can be found in client's resource_maps */
 
     if (map = FindResourceBase(base))
	  return map;

     /* this base is new to this client. create new map */

     map = malloc(sizeof(ResourceMappingRec));
     map->client_base = base;

     /* look for another client started at this xmove with the same
	resource base. If found assume this client is asking about
	the located client. Join them together via their meta_client */

     ScanList(&client_list, ll, cur_client, Client *) {
	  if (!cur_client->xmoved_from_elsewhere &&
	      cur_client->resource_base == base)
	  {
	       Client *tmp_client;
	       
	       map->server_base = cur_client->server->resource_base;
	       map->transient = False;
	       appendToList(&client->resource_maps, (Pointer)map);

	       /* Say we have two clients, a and b, both the same application.
		  First a references b. We add a mapping to a and combine
		  meta-clients. Then b references a. We still need to add
		  the mapping, but the meta-clients are already combined.
		  Check for this here: */
	       
	       if (meta_client == cur_client->meta_client)
		    return map;

	       /* move clients from our meta client to the located client's meta client */
	       while (tmp_client = (Client *)deleteFirst(&meta_client->client_list)) {
		    tmp_client->meta_client = cur_client->meta_client;
		    appendToList(&cur_client->meta_client->client_list, (Pointer)tmp_client);
	       }
	       
	       /* remove the current meta client and replace it with the located client's */
	       RemoveMetaClient(meta_client);
	       free(meta_client);
	       meta_client = cur_client->meta_client;

	       /* NOTE: this is where we should check if client is on the same server
		  as cur_client. If not we should force-move it over to the same server
		  BEFORE this request is sent to the server. How the hell do we do that?? */
	       
	       return map;
	  }
     }

     map->server_base = base;
     map->transient = True;
     appendToList(&client->resource_maps, (Pointer)map);

     return map;
}

/* This routine is used when trying to create a new base ID for
   the client when the server and client have incompatible
   resource masks. It takes a potential base and a mask. It
   increments the base to the next acceptable base. */

DecrementResourceBase(int base, unsigned long mask)
{
     unsigned long cur_bit = 1;

     /* while loop will exit if we try to decrement past zero */
     
     mask = ~mask;
     while (cur_bit) {
	  if (mask & cur_bit) {
	       if (base & cur_bit)
		    return (base & ~(cur_bit));
	       else
		    base |= cur_bit;
	  }

	  cur_bit <<= 1;
     }

     /* we've just decremented 0, which works out to setting
	all bits to one, which is what we want to return */

     return base;
}


/* MapAtom takes more responsibility than the other mapping routines.
   If a move is NOT occurring, ie the client is just interacting normally
   with its server, and if MapAtom can't find the mentioned atom in the
   client's atom list, then it will do a GetAtomName directly to the
   server. Since a move is not occurring we are guaranteed that the
   direction will never be Server2Server or Client2NewServer. */

Atom
MapAtom(Atom xatom, Direction dir)
{
     AtomPtr atom_ptr;
     hash_location loc;

     if (xatom == (Atom)0)
	  return (Atom)0;
     
     if (dir == Request || dir == Client2NewServer) {
	  if (atom_ptr = hash_find(client->atom_table, xatom)) {
	       if (dir == Request)
		    return atom_ptr->server_atom;
	       else
		    return atom_ptr->new_server_atom;
	  }
	  
	  /* atom not found. if no move in progress, ask
	     server about this atom. */

	  if (MoveInProgress)
	       return xatom;

	  atom_ptr = MapNewAtom(xatom, dir);
	  if (atom_ptr)
	       return atom_ptr->server_atom;
	  else
	       return (Atom)0;
	  
     } else {
	  for (atom_ptr = hashloc_init(&loc, client->atom_table);
	       atom_ptr; atom_ptr = hashloc_getnext(&loc))
	  {
	       if (atom_ptr->server_atom == xatom)
		    if (dir == Server2Server)
			 return atom_ptr->new_server_atom;
		    else
			 return atom_ptr->client_atom;
	  }

	  /* atom not found. if no move in progress, ask
	     server about this atom. */

	  if (MoveInProgress)
	       return xatom;

	  atom_ptr = MapNewAtom(xatom, dir);
	  if (atom_ptr)
	       return atom_ptr->client_atom;
	  else
	       return xatom;
     }
}

AtomPtr
MapNewAtom(Atom xatom, Direction dir)
{
     Server *server = client->server;
     char *name = NULL;
     AtomPtr new_atom;

     if (xatom == 0)
	  return NULL;

     if (client->grabbed_server) {
	  unsigned short seqno, orig_seqno;

	  seqno = orig_seqno = client->SequenceNumber - 1 - CountRequestsInBuffer(&server->fdd->outBuffer);
	  name = XMOVEGetAtomName(server->fdd->fd, &seqno, xatom, dir);
	  client->SequenceNumber += (unsigned short)(seqno - orig_seqno); /* presumably == 1 */
	  --client->SequenceMapping;
     } else {
	  name = XMOVEGetAtomName(server->alt_fd, &server->alt_seqno, xatom, dir);

     }
     
     if (name == NULL)
	  return NULL;
     
     new_atom = CreateNewAtom(xatom, name, dir);
     
     free(name);
     return new_atom;
}

Window
MapWindowID(Window xwin, Direction dir)
{
     switch (dir) {
     case Request:
	  return MapResource(xwin, dir,
			     (server->screen == -1) ? xwin : server->root_window);
     case Reply:
     case Event:
     case Error:
	  return MapResource(xwin, dir,
			     (server->screen == -1) ? xwin : client->root_window);
     case Server2Server:
	  return MapResource(xwin, dir, new_server->root_window);
	  
     case Client2NewServer:
	  return MapResource(xwin, dir,
			     (new_server->screen == -1) ? xwin : new_server->root_window);
     }
}

Colormap
MapColormapID(Colormap xcmap, Direction dir)
{
     switch (dir) {
     case Request:
	  return MapResource(xcmap, dir,
			     (server->screen == -1) ? xcmap : server->default_cmap);
     case Reply:
     case Event:
     case Error:
	  return MapResource(xcmap, dir,
			     (server->screen == -1) ? xcmap : client->default_cmap);
     case Server2Server:
	  return MapResource(xcmap, dir, new_server->default_cmap);
	  
     case Client2NewServer:
	  return MapResource(xcmap, dir,
			     (new_server->screen == -1) ? xcmap : new_server->default_cmap);
     }
}

card32
MapColorCell(card32 pixel, ColormapPtr cmap, Direction dir)
{
     ColorCellPtr cell;

     if (pixel >= cmap->cell_count)
	  return pixel;		/* invalid pixel sent, ignore */
     
     switch (dir) {
     case Request:
	  if (cell = FindColorCellFromColormap(pixel, cmap))
	       return cell->server_pixel;
	  else
	       return pixel;

     case Client2NewServer:
	  if (cell = FindColorCellFromColormap(pixel, cmap))
	       return cell->new_server_pixel;
	  else
	       return pixel;

     case Server2Server:
	  if (cell = FindServerColorCellFromColormap(pixel, cmap))
	       return cell->new_server_pixel;
	  else
	       return pixel;

     default:
	  if (cell = FindServerColorCellFromColormap(pixel, cmap))
	       return cell->client_pixel;
	  else
	       return pixel;
     }
}

		    
VisualID
MapVisualID (VisualID xvisual, Direction dir)
{
     if (server->screen == -1)
	  return (xvisual);

     if (dir == Request)
	  return (GetCurrentServerDefaultVisualID());
     else
	  return (GetCurrentClientDefaultVisualID());
}
