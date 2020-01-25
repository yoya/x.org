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
#define NEED_REPLIES
#include <X11/Xproto.h>
#include "xmove.h"
#include "select_args.h"

Bool MoveInProgress;
Server *new_server;
unsigned int cur_mask, cur_base, new_mask, new_base, client_mask, client_base;
int cur_fd, new_fd;
unsigned short *cur_seqno, *new_seqno;
unsigned char new_depth;

static char *newdisp_key;
static int newdisp_keylen;
static unsigned long server_ip_addr;

static char *Move_stage1(char *new_display, char new_screen);
static char *Move_stage2();
static char *Move_stage3();
static char *Move_stage4();
static char *Move_stage5();
static void UnmapClientWindows();
static Window RemapStackWindows();
static void RemapWindows();
static void FixClientSeqnos();

extern LinkList meta_client_list;
extern unsigned long HostIPAddr;
extern long WriteDescriptors[mskcnt];
extern Bool littleEndian;

/* THE MOVE ROUTINES */

void SetGlobals()
{
     if (client == NULL)
	  return;

     littleEndian = client->fdd->littleEndian;
     
     client_mask = client->resource_mask;
     client_base = client->resource_base;
     cur_seqno = &client->SequenceNumber;
     new_seqno = &client->new_seqno;

     server = client->server;
     cur_mask = server->resource_mask;
     cur_base = server->resource_base;
     cur_fd   = server->fdd->fd;

     new_server = client->new_server;
     if (new_server) {
	  new_mask = new_server->resource_mask;
	  new_base = new_server->resource_base;
	  new_fd   = new_server->fdd->fd;
	  new_depth = new_server->default_depth;
     }
}
     

CloseNewServers()
{
     LinkLeaf *ll;
     Client *cur_client;

     ScanList(&meta_client->client_list, ll, cur_client, Client *) {
	  if (client->new_server) {
	       Server *cur_server = client->new_server;
	       
	       if (cur_server->fdd) {
		    close(cur_server->fdd->fd);
		    NotUsingFD(cur_server->fdd->fd);
	       }

	       if (cur_server->alt_fd)
		    close(cur_server->alt_fd);

	       if (cur_server->server_name) free(cur_server->server_name);
	       
	       if (cur_server->formats != client->formats) {
		    Pointer item;
		    
		    while (item = deleteFirst(&cur_server->formats->format_list))
			 Tfree(item);
		    while (item = deleteFirst(&cur_server->formats->depth_list)) {
			 Pointer item2;
			 
			 while (item2 = deleteFirst(&((DepthPtr)item)->visual_list))
			      Tfree(item2);
			 
			 Tfree(item);
		    }
		    
		    free(cur_server->formats);
	       }
	       Tfree(cur_server);
	       client->new_server = NULL;
	  }
     }
}

/*
  move_all() will move all clients to new_display_name, with the exception
  of the client pointed to by exclude_client
*/

Global void
MoveAll(char *new_display_name, char new_screen, MetaClient *exclude_client,
	int keylen, char *key, char **rettext)
{

     ForAllInList(&meta_client_list)
     {
	  meta_client = (MetaClient *)CurrentContentsOfList(&meta_client_list);
	  if (meta_client != exclude_client)
	       MoveClient(new_display_name, new_screen, keylen, key, rettext);
     }
}

Global void
MoveClient(char *new_display, char new_screen, int keylen, char *key, char **rettext)
{
     Bool old_endianism = littleEndian;

     /* The move is on! Let the world know! */
     MoveInProgress = True;

     newdisp_key = key;
     newdisp_keylen = keylen;

     /* Move_stage1 is assumed to not affect the state of the original client
	connections */

     ForAllInList(&meta_client->client_list) {
	  client = (Client *)CurrentContentsOfList(&meta_client->client_list);
	  littleEndian = client->fdd->littleEndian;
	  if ((*rettext = Move_stage1(new_display, new_screen)) != NULL) {
	       CloseNewServers();
	       littleEndian = old_endianism;
	       MoveInProgress = False;
	       return;
	  }
     }

     ForAllInList(&meta_client->client_list) {
	  client = (Client *)CurrentContentsOfList(&meta_client->client_list);
	  SetGlobals();
	  UnmapClientWindows();
     }

     ForAllInList(&meta_client->client_list) {
	  client = (Client *)CurrentContentsOfList(&meta_client->client_list);
	  SetGlobals();
	  if ((*rettext = Move_stage2()) != NULL) {
	       CloseNewServers();
	       RemapWindows();
	       FixClientSeqnos();
	       littleEndian = old_endianism;
	       MoveInProgress = False;
	       return;
	  }
     }

     ForAllInList(&meta_client->client_list) {
	  client = (Client *)CurrentContentsOfList(&meta_client->client_list);
	  SetGlobals();
	  if ((*rettext = Move_stage3()) != NULL) {
	       CloseNewServers();
	       RemapWindows();
	       FixClientSeqnos();
	       littleEndian = old_endianism;
	       MoveInProgress = False;
	       return;
	  }
     }

     ForAllInList(&meta_client->client_list) {
	  client = (Client *)CurrentContentsOfList(&meta_client->client_list);
	  SetGlobals();
	  if ((*rettext = Move_stage4()) != NULL) {
	       CloseNewServers();
	       RemapWindows();
	       FixClientSeqnos();
	       littleEndian = old_endianism;
	       MoveInProgress = False;
	       return;
	  }
     }

     /* Move_stage5 shouldn't return error, since it won't be handled correctly.
	stage5 changes too much client state at the end. */

     ForAllInList(&meta_client->client_list) {
	  client = (Client *)CurrentContentsOfList(&meta_client->client_list);
	  SetGlobals();
	  if ((*rettext = Move_stage5()) != NULL) {
	       CloseNewServers();
	       RemapWindows();
	       littleEndian = old_endianism;
	       MoveInProgress = False;
	       return;
	  }
     }

     ForAllInList(&meta_client->client_list) {
	  client = (Client *)CurrentContentsOfList(&meta_client->client_list);
	  
	  /* this may not be the best place to put this, but it can't be called until
	     we know the move has completed. Update the client's output-buffer's pointer
	     to the data source, ie the server. */
	  
	  client->fdd->outBuffer.src_fd = client->server->fdd->fd;
	  
	  if (server_ip_addr == HostIPAddr)
	       client->xmoved_to_elsewhere = False;
	  else
	       client->xmoved_to_elsewhere = True;
     }
     
     /* The move is done! Let the world know! */
     littleEndian = old_endianism;
     MoveInProgress = False;
}

static char *
Move_stage1(char *new_display, char new_screen)
{
     unsigned char conn_reply[32], *display_data, *screen, *orig_screen;
     char *full_display_name, *new_display_name, *colon;
     int new_display_name_len, reply_len, num_screens;
     int alt_fd;
     XGCValues new_gc_val;
     
     if (client->xmoved_from_elsewhere) {
	  unsigned char buf[32];
	  int disp_name_len = strlen(new_display) + 1;

	  ISetByte(&buf[0], 253);
	  ISetLong(&buf[4], 1 + ROUNDUP4(disp_name_len)/4 + ROUNDUP4(newdisp_keylen)/4);
	  ISetShort(&buf[8], disp_name_len);
	  ISetShort(&buf[10], newdisp_keylen);

	  SendBuffer(client->fdd->fd, buf, 32);
	  SendBuffer(client->fdd->fd, (unsigned char *)&new_screen, 1);
	  SendBuffer(client->fdd->fd, buf, 3);
	  SendBuffer(client->fdd->fd, (unsigned char *)new_display, disp_name_len);
	  SendBuffer(client->fdd->fd, buf, ROUNDUP4(disp_name_len)-disp_name_len);
	  SendBuffer(client->fdd->fd, (unsigned char *)newdisp_key, newdisp_keylen);
	  SendBuffer(client->fdd->fd, buf, ROUNDUP4(newdisp_keylen)-newdisp_keylen);
	  return NULL;
     }

     if (client->grabbed_server)
	  return strdup("Cannot move an application which has grabbed the server exclusively\n");

     if (newdisp_keylen > 0) {
	  ISetShort(client->startup_message + 6, 18);
	  ISetShort(client->startup_message + 8, newdisp_keylen);
     } else {
	  ISetShort(client->startup_message + 6, 0);
	  ISetShort(client->startup_message + 8, 0);
     }

     new_display_name = malloc(strlen(new_display)+1);
     strcpy(new_display_name, new_display);
     new_display_name_len = strlen(new_display_name);
     full_display_name = malloc(new_display_name_len + 3);
     
     /* if new_display_name has a colon in it, assume that we are being
	told precisely what server to move the client to and that it isn't
	an xmove. If there is no colon, first try to find an xmove running
	on the destination machine, then fall back to using the X11 server
	directly. */
     
     if (colon = strchr(new_display_name, ':')) {
	  strcpy(full_display_name, new_display_name);
	  *(colon++) = '\0';
	  new_fd = ConnectToServer(new_display_name, atoi(colon),
				   &server_ip_addr);
	  if (new_fd < 0) {
	       free(full_display_name);
	       free(new_display_name);
	       return strdup("Unable to connect with new server\n");
	  }

	  alt_fd = ConnectToServer(new_display_name, atoi(colon), NULL);
	  if (alt_fd < 0) {
	       free(full_display_name);
	       free(new_display_name);
	       close(new_fd);
	       return strdup("Unable to connect with new server\n");
	  }
	  
	  SendBuffer(new_fd, client->startup_message, 12);
	  SendBuffer(alt_fd, client->startup_message, 12);
	  if (newdisp_keylen > 0) {
	       SendBuffer(new_fd, (u_char *)"MIT-MAGIC-COOKIE-1 ", 20);
	       SendBuffer(new_fd, (u_char *)newdisp_key, newdisp_keylen);	/* keylen == 16, no padding needed */
	       SendBuffer(alt_fd, (u_char *)"MIT-MAGIC-COOKIE-1 ", 20);
	       SendBuffer(alt_fd, (u_char *)newdisp_key, newdisp_keylen);
	  }
	  
     } else {
	  new_fd = ConnectToServer(new_display_name, 1, &server_ip_addr);
	  if (new_fd < 0) {
	       new_fd = ConnectToServer(new_display_name, 0, &server_ip_addr);
	       if (new_fd < 0) {
		    free(full_display_name);
		    free(new_display_name);
		    return strdup("Unable to connect with new server\n");
	       }

	       alt_fd = ConnectToServer(new_display_name, 0, NULL);
	       if (alt_fd < 0) {
		    free(full_display_name);
		    free(new_display_name);
		    close(new_fd);
		    return strdup("Unable to connect with new server\n");
	       }
	       
	       sprintf(full_display_name, "%s:0", new_display_name);
	       SendBuffer(new_fd, client->startup_message, 12);
	       SendBuffer(alt_fd, client->startup_message, 12);
	       if (newdisp_keylen > 0) {
		    SendBuffer(new_fd, (u_char *)"MIT-MAGIC-COOKIE-1 ", 20);
		    SendBuffer(new_fd, (u_char *)newdisp_key, newdisp_keylen); /* keylen == 16, no padding needed */
		    SendBuffer(alt_fd, (u_char *)"MIT-MAGIC-COOKIE-1 ", 20);
		    SendBuffer(alt_fd, (u_char *)newdisp_key, newdisp_keylen);
	       }
	  
	  } else {
	       alt_fd = ConnectToServer(new_display_name, 0, NULL);
	       if (alt_fd < 0) {
		    free(full_display_name);
		    free(new_display_name);
		    close(new_fd);
		    return strdup("Unable to connect with new server\n");
	       }
	       
	       sprintf(full_display_name, "%s:1", new_display_name);
	       ISetByte(client->startup_message + 1,
			client->window_name ?
			strlen(client->window_name)+1 : 0);
	       ISetShort(client->startup_message + 10,
			 IShort(client->startup_message + 2));
	       ISetShort(client->startup_message + 2, 253);
	       
	       SendBuffer(new_fd, client->startup_message, 12);
	       
	       ISetShort(client->startup_message + 2,
			 IShort(client->startup_message + 10));
	       
	       SendBuffer(alt_fd, client->startup_message, 12);
	       
	       if (newdisp_keylen > 0) {
		    SendBuffer(new_fd, (u_char *)"MIT-MAGIC-COOKIE-1 ", 20);
		    SendBuffer(new_fd, (u_char *)newdisp_key, newdisp_keylen); /* keylen == 16, no padding needed */
		    SendBuffer(alt_fd, (u_char *)"MIT-MAGIC-COOKIE-1 ", 20);
		    SendBuffer(alt_fd, (u_char *)newdisp_key, newdisp_keylen);
	       }
	  
	       if (client->window_name) {
		    SendBuffer(new_fd, (unsigned char *)client->window_name,
			       strlen(client->window_name)+1);
	       }
	  }
     }

     free(new_display_name);
     
     /* receive reply from server */
     
     ReceiveBuffer(new_fd, conn_reply, 8);
     reply_len = IShort(conn_reply+6)*4;
     display_data = malloc(reply_len);
     ReceiveBuffer(new_fd, display_data, reply_len);

     if (*conn_reply == 0) {
	  char *retval = malloc(36 + IByte(conn_reply+1));

	  bcopy("Unable to connect with new server\n", retval, 34);
	  bcopy((char *)display_data, retval+34, IByte(conn_reply+1));
	  retval[34 + IByte(conn_reply+1)] = '\n';
	  retval[35 + IByte(conn_reply+1)] = '\0';

	  free(full_display_name);
	  free(display_data);
	  close(new_fd);
	  return retval;
     }

     /* receive success/failure from alternate fd. If failure, stop here */
     
     ReceiveBuffer(alt_fd, conn_reply, 8);
     if (*conn_reply == 0) {
	  free(full_display_name);
	  free(display_data);
	  close(new_fd);
	  close(alt_fd);
	  return strdup("Unable to connect with new server\n");
     }
     
     /* instantiate this new server */
     
     new_server = Tcalloc(1, Server);
     new_server->fdd = UsingFD(new_fd, DataFromServer, (Pointer) new_server);
     new_server->fdd->fd = new_fd;
     new_server->alt_fd = alt_fd;
     new_server->alt_seqno = 0;

     /* According to the X11 proocol specification, communication */
     /* between the client and the server is done according to the */
     /* client's byte ordering, so the server ends up doing all the */
     /* byte swapping.  Therefore, any new server that we connect */
     /* to has to use the same byte ordering as this particular */
     /* client used previously. */
     /* set the little endian flag for this server */
     
     new_server->fdd->littleEndian = client->fdd->littleEndian;
     new_server->client = client;
     client->new_server = new_server;
     new_server->server_name = full_display_name;
     RestartServerConnection(new_server);
     
     /* Take the connection setup reply from the server and parse out
	the important bits into new_server */
     
     new_server->resource_base = ILong(display_data + 4);
     new_server->resource_mask = ILong(display_data + 8);

     SaveFormats(new_server,
		 display_data + 32 + ROUNDUP4(IShort(display_data + 16)),
		 IByte(display_data + 21));
     
     /* take root window from screen 0, which is located after the
	main data (32), after the vendor string (ROUNDUP4(IShort(display_data+16)))
	and after the pixmap formats (IByte(display_data+21)*8). */

     num_screens = IByte(display_data + 20);
     orig_screen = screen = display_data + 32 + ROUNDUP4(IShort(display_data + 16)) +
	  (IByte(display_data + 21) * 8);

     /* take defaults from requested new screen. If new screen is -1, then
	the user didn't specify a specific screen. We go to screen 0 unless
	the client is moved back to its original server, in which case we
	allow the client to specify root windows uninterpreted. If the screen
	is not screen 0, the following routine will skip to the beginning of
	the information for the desired screen. */
     
     if (new_screen == -1)
	  if (client->xmoved_to_elsewhere == True)
	       new_screen = 0;
	  else {
	       /* search through all the screens until we find the screen
		  whose root window matches client->root_window. */

	       while (num_screens && ILong(screen) != client->root_window) {
		    int num_depths = IByte(&screen[39]);
		    
		    screen += 40;
		    while (num_depths--) {
			 int num_visuals = IShort(&screen[2]);

			 screen += 8 + num_visuals * 24;
		    }

		    num_screens--;
	       }

	       if (num_screens == 0) {
		    screen = orig_screen;
	       } else
		    new_server->screen = -1;
	  }

     if (new_screen >= 0) {
	  if ((unsigned)new_screen >= IByte(display_data + 20))
	       new_screen = 0;
	  
	  new_server->screen = new_screen;
	  while (new_screen--) {
	       int num_depths = IByte(&screen[39]);
	       
	       screen += 40;
	       while (num_depths--) {
		    int num_visuals = IShort(&screen[2]);
		    
		    screen += 8 + num_visuals * 24;
	       }
	  }
     }
     
     new_server->root_window   = ILong(screen);
     new_server->default_cmap  = ILong(screen + 4);
     new_server->default_vid   = ILong(screen + 32);
     new_depth                 = IByte(screen + 38);
     new_server->default_depth = new_depth;
     new_gc_val.background     = ILong(screen + 8);
     new_gc_val.foreground     = ILong(screen + 12);
     
     SaveDepths(new_server, screen);
     
     if (client->default_depth == 1 && new_server->default_depth != 1) {
	  free(display_data);
	  return strdup("Cannot move a monochrome client to a color or grey-scale display\n");
     }
     
     /* set the image characteristics for this new server */
     new_server->formats->max_request_size       = IShort(display_data + 18);
     new_server->formats->image_byte_order       =  IByte(display_data + 22);
     new_server->formats->image_bitmap_bit_order =  IByte(display_data + 23);
     new_server->formats->image_bitmap_unit      =  IByte(display_data + 24);
     new_server->formats->image_bitmap_pad       =  IByte(display_data + 25);
     
     new_server->formats->flags = 0;
     if ((new_server->formats->image_bitmap_pad == XMOVEGetScanlinePad(new_server->formats, 1)) &&
	 (new_server->formats->image_bitmap_pad == client->formats->image_bitmap_pad) &&
	 (new_server->formats->image_bitmap_pad == XMOVEGetScanlinePad(client->formats, 1)) &&
	 (XMOVEGetBitsPerPixel(new_server->formats, 1) == 1) &&
	 (XMOVEGetBitsPerPixel(client->formats, 1) == 1) &&
	 (new_server->formats->image_byte_order == new_server->formats->image_bitmap_bit_order) &&
	 (client->formats->image_byte_order == new_server->formats->image_bitmap_bit_order))
     {
	  new_server->formats->flags |= IMAGE_1BIT_SAFE;
     }

     if ((client->default_depth == new_server->default_depth) &&
	 (client->default_depth == XMOVEGetBitsPerPixel(new_server->formats, new_server->default_depth)) &&
	 (client->default_depth == XMOVEGetBitsPerPixel(client->formats, new_server->default_depth)) &&
	 (XMOVEGetScanlinePad(new_server->formats, new_server->default_depth) ==
	  XMOVEGetScanlinePad(client->formats, new_server->default_depth)) &&
	 (client->default_depth == 8 || client->default_depth == 16 || client->default_depth == 32))
     {  
	  new_server->formats->flags |= IMAGE_NBIT_SAFE;
     }

     new_server->min_keycode                     =  IByte(display_data + 26);
     new_server->max_keycode                     =  IByte(display_data + 27);

     /* receive setup reply from alt_fd */

     if (IShort(conn_reply+6)*4 != reply_len) {
	  free(display_data);
	  display_data = malloc(IShort(conn_reply+6)*4);
     }
     ReceiveBuffer(alt_fd, display_data, IShort(conn_reply+6)*4);
     new_server->alt_resource_base = ILong(display_data+4);
     free(display_data);

     /* set global variable server to client's current server */
     server = client->server;

     while (server->fdd->outBuffer.num_Saved)
	  SendXmoveBuffer(server->fdd->fd, &server->fdd->outBuffer);

     /* This isn't the prettiest of ways to do it, but it works. If
	there is a reply pending from the server then we cannot move
	the client until it comes. The client will wait forever to get
	that reply. */

     while (!ListIsEmpty(&server->reply_list)) {
	  printf("waiting for reply from server to client's pending request\n");
	  if (!ReadAndProcessData((Pointer)server, server->fdd, client->fdd, True))
	       return strdup("Unexpected error while moving client\n");
     }

     fprintf(stdout, "XMOVE:  switching client %d to display %s\n", 
	     client->ClientNumber, full_display_name);

     client->orig_seqno = client->SequenceNumber;
     client->new_seqno = 0; /* start off at 0, will be pre-incremented */

     new_seqno = &client->new_seqno;
     new_base = new_server->resource_base;
     new_mask = new_server->resource_mask;
     
     Dprintf(("New display base = 0x%x, mask = 0x%x\n", new_base, new_mask));
     Dprintf(("Switching from fd#%d to fd#%d\n",cur_fd,
              new_fd));

     /* create the GC with ID#0 that XLib always creates */
     
     XMOVECreateGC(new_fd, new_seqno, new_base, new_server->root_window,
		   GCForeground | GCBackground, &new_gc_val);

     XMOVECreatePixmap(new_fd, new_seqno, (new_base | (new_mask >> 1)) - 1,
		       new_server->root_window, 1, 1, 1);
     
     XMOVECreateGC(new_fd, new_seqno, (new_base | (new_mask >> 1)),
		   (new_base | (new_mask >> 1)) - 1, 0, NULL);
     return NULL;
}

static char *Move_stage2()
{
     LinkLeaf *ll;
     Client *tmp_client;
     hash_location loc;
     PixmapPtr pixmap;
     FontPtr font;
     AtomPtr atom;
     char *retval;

     /* Remove transient items from the resource mapping list.
        If the item points to our client's alt_fd, update. */

     ForAllInList(&client->resource_maps) {
	  ResourceMappingPtr map;

	  map = (ResourceMappingPtr)CurrentContentsOfList(&client->resource_maps);

	  if (map->server_base == server->alt_resource_base)
	       map->new_server_base = new_server->alt_resource_base;
	  
	  if (map->transient) {
	       deleteCurrent(&client->resource_maps);
	       free(map);
	  }
     }
     

     /* Look in our fellow clients and gather their new_server resource bases
	and masks. Update our resource_maps with that information */

     ScanList(&meta_client->client_list, ll, tmp_client, Client *) {
	  ResourceMappingPtr map;
	  
	  if (tmp_client == client)
	       continue;	/* no need to update ourselves */

	  if ((map = FindResourceBase(tmp_client->resource_base)))
	       map->new_server_base = tmp_client->new_server->resource_base;
     }


     /* Move all the colormaps to the new server and allocate all of
	their color cells. */
     
     retval = MoveColormaps();
     if (retval)
	  return retval;
     
     for (atom = hashloc_init(&loc, client->atom_table);
	  atom; atom = hashloc_getnext(&loc))
     {
	  Dprintf(("About to Intern Atom %s == (c)%d (s)%d\n",atom->atom_name,atom->client_atom,atom->server_atom));
	  
	  atom->new_server_atom = XMOVEInternAtom(new_fd, new_seqno, atom->atom_name, False);
     }
     
     XMOVEGetInputFocus(new_fd, new_seqno);
     
     for (font = hashloc_init(&loc, client->font_table);
	  font; font = hashloc_getnext(&loc))
     {
	  Font xfont;
	  
	  xfont = MapFontID(font->font_id, Client2NewServer);
	  
	  XMOVELoadFont(new_fd, new_seqno, xfont, font->name);
	  
	  Dprintf(("Font %s loaded with id 0x%x\n", 
		   font->name, xfont));
     }
     
     if (!XMOVEVerifyRequest(new_fd, new_seqno))
	  return strdup("Some fonts used by this client are unavailable on the new server\n");

     for (pixmap = hashloc_init(&loc, client->pixmap_table);
	  pixmap; pixmap = hashloc_getnext(&loc))
     {
	  Pixmap xpixmap;
	  
	  xpixmap = MapPixmapID(pixmap->pixmap_id, Client2NewServer);
	  
	  /* This is a REALLY cheap hack, but according to O'Reilly X 0, it
	     should be all right. The drawable in the CreatePixmap call
	     is only used to determine the screen with which to associate
	     the pixmap. So we use DefaultRootWindow. This allows us to create
	     the pixmaps before the windows and GCs. */
	  
	  XMOVECreatePixmap(new_fd, new_seqno, xpixmap, new_server->root_window,
			    pixmap->width, pixmap->height,
			    (pixmap->depth == 1) ? 1 : new_depth);
	  
	  pixmap->new_pixmap = xpixmap;
	  Dprintf(("New pixmap %x old pixmap %x\n",xpixmap,
		   pixmap->pixmap_id));
     }

     return NULL;
}

static char *Move_stage3()
{
     hash_location loc;
     void *pos;
     WindowPtr window;
     PixmapPtr pixmap;
     GlyphCursorPtr cursor_ptr;
     
     for (cursor_ptr = hashloc_init(&loc, client->glyph_cursor_table);
	  cursor_ptr; cursor_ptr = hashloc_getnext(&loc))
     {
	  Cursor xcursor;
	  Font src_font, mask_font;
	  
	  xcursor = MapCursorID(cursor_ptr->cursor_id, Client2NewServer);
	  
	  src_font = MapFontID(cursor_ptr->source_font, Client2NewServer);
	  if (cursor_ptr->mask_font)
	       mask_font = MapFontID(cursor_ptr->mask_font, Client2NewServer);
	  else
	       mask_font = 0;
	  
	  XMOVECreateGlyphCursor(new_fd, new_seqno, xcursor, src_font,
				 mask_font, cursor_ptr->source_char, 
				 cursor_ptr->mask_char,
				 &cursor_ptr->foreground_color,
				 &cursor_ptr->background_color);
	  
	  Dprintf(("cursor id 0x%x loaded.\n", cursor_ptr->cursor_id));
     }

     /* Create all the windows that this client has already created */
     
     window = hash_getfirst(&pos, client->window_table);
     while (window) {
	  move_window(window, new_depth);
	  window = hash_getnext(&pos, client->window_table);
     }

     /* now copy any pixmap image from the original server to the new server
	if there is still valid data in the original servers pixmap */
     
     Dprintf(("Entering putimage loop\n"));
     for (pixmap = hashloc_init(&loc, client->pixmap_table);
	  pixmap; pixmap = hashloc_getnext(&loc))
     {
	  Pixmap xpixmap;
	  XImage *image, *new_image;
	  struct _XGC gc;
	  
	  /* use the gc created by Xlib (and us) at startup, the one
	     with resource ID 0 */

	  /* only store an image to the new pixmap if it either hasn't been
	     removed from the original server, or if there is an image saved
	     within pixmap_ptr */

	  if (pixmap->pixmap_image)
	       image = pixmap->pixmap_image;
	  else
	       image = XMOVEGetImage(cur_fd, cur_seqno, server,
				     MapPixmapID(pixmap->pixmap_id, Request),
				     0, 0, pixmap->width, pixmap->height,
				     0xFFFFFFFF, ZPixmap);

	  /* don't map a 1 bitplane image. see print11.c:ProcessPutImage()
	     for reasons. */

	  if ((pixmap->depth == 1) && (new_depth != 1))
	       gc.gid = new_base | (new_mask >> 1);
	  else
	       gc.gid = new_base;
	  
	  if (pixmap->depth == 1 && (new_server->formats->flags & IMAGE_1BIT_SAFE) &&
	      (client->formats->flags & IMAGE_1BIT_SAFE))
	  {
	       new_image = image;
	  }
	  else if ((pixmap->depth != 1) && (pixmap->depth == new_server->default_depth) &&
		   (client->formats->flags & IMAGE_NBIT_SAFE) &&
		   (new_server->formats->flags & IMAGE_NBIT_SAFE))
	  {
	       if (image->bits_per_pixel == 8)
		    MapImage8(image->data, pixmap->width, pixmap->height, pixmap->depth,
			      image->bytes_per_line, pixmap->cmap, Server2Server);
	       else if (image->bits_per_pixel == 16)
		    MapImage16(image->data, pixmap->width, pixmap->height, pixmap->depth,
			       image->bytes_per_line, pixmap->cmap, Server2Server,
			       (server->formats->image_byte_order != XmoveLittleEndian),
			       (new_server->formats->image_byte_order != XmoveLittleEndian));
	       else if (image->bits_per_pixel == 32)
		    MapImage32(image->data, pixmap->width, pixmap->height, pixmap->depth,
			       image->bytes_per_line, pixmap->cmap, Server2Server,
			       (server->formats->image_byte_order != XmoveLittleEndian),
			       (new_server->formats->image_byte_order != XmoveLittleEndian));
	       
	       new_image = image;
	  }
	  else
	  {
	       new_image = XMOVEPreCreateImage(new_server->formats, NULL, ZPixmap,
					       NULL, 0xFFFFFFFF, (pixmap->depth == 1 ? 1 : new_depth),
					       NULL, pixmap->width, pixmap->height);
	       
	       new_image->data = malloc(new_image->bytes_per_line * pixmap->height);
	       
	       if (pixmap->depth == 1)
		    CopyImage(image, new_image, pixmap->width, pixmap->height);
	       else
		    MoveImage(image, new_image, pixmap->width, pixmap->height,
			      pixmap->cmap, Server2Server);
	  }
	  
	  XMOVEPutImage(new_fd, new_seqno, new_server->formats,
			pixmap->new_pixmap, &gc, new_image,
			0,0,0,0, pixmap->width, pixmap->height);
	  
	  if (new_image != image) {
	       free(new_image->data);
	       free((char *) new_image);
	  }
	  
	  Dprintf(("putimage with gc 0x%x\n",gc.gid));
	  
	  /* and we can free the image, if it wasn't stored with the pixmap */
	  if (!pixmap->pixmap_image) {
	       free(image->data);
	       free((char *) image);
	  }
     }
     
     return NULL;
}

static char *Move_stage4()
{
     hash_location loc;
     CursorPtr cursor_ptr;
     GCPtr gc_ptr;

     /* create all of the gc's */

     for (gc_ptr = hashloc_init(&loc, client->gc_table);
	  gc_ptr; gc_ptr = hashloc_getnext(&loc))
     {
	  GContext xgc;
	  Drawable drawable;
	  XGCValues values;
	  ColormapPtr cmap;
	  
	  if (gc_ptr->gc_id == client_base)
	       continue;

	  cmap = gc_ptr->cmap;
	  
	  /* map all the new resource ID's to the current server */

	  drawable = MapDrawable(gc_ptr->drawable_id, Client2NewServer);
	  
	  memcpy(&values, &gc_ptr->values, sizeof(XGCValues));

	  if (gc_ptr->values_mask & GCForeground)
	       values.foreground = MapColorCell(values.foreground, cmap, Client2NewServer);

	  if (gc_ptr->values_mask & GCBackground)
	       values.background = MapColorCell(values.background, cmap, Client2NewServer);

	  if(gc_ptr->values_mask & GCFont)
	  {
	       values.font = MapFontID(values.font, Client2NewServer);
	       Dprintf(("remapped font for new gc to be 0x%x\n",gc_ptr->values.font));
	       }
	  
	  if(gc_ptr->values_mask & GCTile)
	       values.tile = MapPixmapID(values.tile, Client2NewServer);
	  
	  if(gc_ptr->values_mask & GCStipple)
	       values.stipple = MapPixmapID(values.stipple, Client2NewServer);
	  
	  /* The clip mask can have a value of 0 which should remain 0 */
	  
	  if ((gc_ptr->values_mask & GCClipMask) && gc_ptr->values.clip_mask)
	       values.clip_mask = MapPixmapID(values.clip_mask, Client2NewServer);
	  
	  xgc = MapGCID(gc_ptr->gc_id, Client2NewServer);
	  
	  XMOVECreateGC(new_fd, new_seqno, xgc,
			(gc_ptr->depth == 1)
			? (new_base | (new_mask >> 1)) - 1
			: new_server->root_window,
			gc_ptr->values_mask, &values);
	  
	  Dprintf(("Created new GC with gc_id 0x%x  old gc_id 0x%x\n",
		   xgc, gc_ptr->gc_id));
     }
     
     for (cursor_ptr = hashloc_init(&loc, client->cursor_table);
	  cursor_ptr; cursor_ptr = hashloc_getnext(&loc))
     {
	  Cursor xcursor;
	  Pixmap src_pixmap, mask_pixmap;
	  
	  src_pixmap = MapPixmapID(cursor_ptr->source_pixmap, Client2NewServer);
	  if (cursor_ptr->mask_pixmap)
	       mask_pixmap = MapPixmapID(cursor_ptr->mask_pixmap, Client2NewServer);
	  else
	       mask_pixmap = 0;
	  
	  xcursor = MapCursorID(cursor_ptr->cursor_id, Client2NewServer);
	  
	  XMOVECreatePixmapCursor(new_fd, new_seqno, xcursor, src_pixmap,
				  mask_pixmap, &cursor_ptr->foreground_color,
				  &cursor_ptr->background_color,
				  cursor_ptr->x, cursor_ptr->y);
	  
	  Dprintf(("cursor id 0x%x loaded.\n", xcursor));
     } 
     
     return NULL;
}

static char *Move_stage5()
{
     int counter;
     Pointer item;
     xChangeWindowAttributesReq ChangeAttrReq;
     WindowPtr window;
     PixmapPtr pixmap;
     AtomPtr atom;
     hash_location loc;
     void *pos;
     
     /* erase any pixmap on the new server that the client freed on the 
	original server */

     for (pixmap = hashloc_init(&loc, client->pixmap_table);
	  pixmap; pixmap = hashloc_getnext(&loc))
     {
	  Pixmap xpixmap;
	  
	  if (pixmap -> pixmap_image)
	  {
	       xpixmap = MapPixmapID(pixmap->pixmap_id, Client2NewServer);
	       XMOVEFreePixmap(new_fd, new_seqno, xpixmap);
	       Dprintf(("XMOVE freeing pixmap 0x%x\n",xpixmap));
	  }
     }

     ForAllInList(&client->grab_list)
     {
	  GrabPtr grab_ptr;
	  
	  grab_ptr = (GrabPtr)
	       CurrentContentsOfList(&client->grab_list);
	  
	  if (grab_ptr->type == 28) /* GrabButton */ {
	       Dprintf(("Grabbing Button %d on new display",
			grab_ptr->button));
	       
	       XMOVEGrabButton(new_fd, new_seqno, grab_ptr->button,
			       grab_ptr->modifiers,
			       MapWindowID(grab_ptr->grab_window, Client2NewServer),
			       grab_ptr->owner_events, grab_ptr->event_mask,
			       grab_ptr->pointer_mode, grab_ptr->keyboard_mode,
			       grab_ptr->confine_to ?
			       MapWindowID(grab_ptr->confine_to, Client2NewServer) : 0,
			       (grab_ptr->cursor == None) ? None :
			       MapCursorID(grab_ptr->cursor, Client2NewServer));
	  } else { /* GrabKey */
	       
	       Dprintf(("Grabbing Key %d on new display",grab_ptr->key));
	       /* check this -- map key codes? */
		    
	       XMOVEGrabKey(new_fd, new_seqno, grab_ptr->key,
			    grab_ptr->modifiers,
			    MapWindowID(grab_ptr->grab_window, Client2NewServer),
			    grab_ptr->owner_events, grab_ptr->pointer_mode,
			    grab_ptr->keyboard_mode);
	  }
     }
     
     
     /* move all of the client's selections to the new server */
     
     move_selections();
     
     /* ChangeWindowAttributes for cursor */
     
     ChangeAttrReq.reqType = X_ChangeWindowAttributes;
     ISetShort((unsigned char *)&ChangeAttrReq.length, 4);

     for (window = hashloc_init(&loc, client->window_table);
	  window; window = hashloc_getnext(&loc))
     {
	  XID xwin;

	  if (window->attributes_mask & CWCursor) {
	       Cursor new_cursor =
		    MapCursorID(window->attributes.cursor, Client2NewServer);
	       
	       xwin = MapWindowID(window->window_id, Client2NewServer);
	       
	       ISetLong((unsigned char *)&ChangeAttrReq.window, xwin);
	       ISetLong((unsigned char *)&ChangeAttrReq.valueMask, CWCursor);
	       SendBuffer(new_fd, (unsigned char *)&ChangeAttrReq, 12);
	       SendBuffer(new_fd, (unsigned char *)&new_cursor, 4);
	       ++(*new_seqno);
	  }
     }
     
     /* map all of the mapped windows in the correct stacking order */
     
     for (counter = 1; counter <= GetCurrentClientMaxMappedOrder(); counter++)
     {
	  for (window = hash_getfirst(&pos, client->window_table);
	       window; window = hash_getnext(&pos, client->window_table))
	  {
	       XID xwin;
	       
	       if (window->mapped_order == counter) {
		    xwin = MapWindowID(window->window_id, Client2NewServer);
		    XMOVEMapWindow(new_fd, new_seqno, xwin);
	       }
	       
	       if(window->submapped_order == counter)
		    XMOVEStackWindows(window);
	  }
     }

     /* move new server atom mappings into current server section */
     

     for (atom = hashloc_init(&loc, client->atom_table);
	  atom; atom = hashloc_getnext(&loc))
     {
	  atom->server_atom = atom->new_server_atom;
     }
	  
     /* move new colorcell mappings into current server section */

     ForAllInList(&client->colormap_list)
     {
	  ColormapPtr cmap;
	  ColorCellPtr cell;

	  cmap = (ColormapPtr)
	       CurrentContentsOfList(&client->colormap_list);

	  for (cell = cmap->cell_array + cmap->cell_count; cell > cmap->cell_array ; )
	  {
	       if ((--cell)->usage_count)
		    cell->server_pixel = cell->new_server_pixel;
	  }
     }

     /* move new resource base mappings into current server section */
     
     ForAllInList(&client->resource_maps)
     {
	  ResourceMappingPtr map;

	  map = (ResourceMappingPtr)CurrentContentsOfList(&client->resource_maps);
	  if (map->new_server_base)
	       map->server_base = map->new_server_base;
     }

     /* free depth 1 GC created (id == new_base | new_mask) for screens with
	depth > 1. */
     
     XMOVEFreePixmap(new_fd, new_seqno, (new_base | (new_mask >> 1)) - 1);
     XMOVEFreeGC(new_fd, new_seqno, new_base | (new_mask >> 1));

     /* We have to update SequenceMapping, since the new server will be at a
	different sequence number. If the original server was at seqno 100, and
	the new one is at seqno 50, we will need to add (100-50)==50 to each new
	sequence number we see. */

     client->SequenceMapping += client->orig_seqno - client->new_seqno;
     client->SequenceMapping &= 0xFFFF;
     client->LastMoveSeqNo = client->new_seqno;

     /* set the client sequence number to the current sequence */
     /* number */  
     
     client->formats->flags = new_server->formats->flags;
     
     client->min_keycode = new_server->min_keycode;
     client->max_keycode = new_server->max_keycode;
     
     client->server = new_server;
     client->SequenceNumber = client->new_seqno;
     client->new_server = NULL;

     /* spoof the old client into believing that it just got a */
     /* RefreshMapping event by writing in the appropriate 32 byte */
     /* block into its output buffer */
     
     /* XXX - this sequence number that is being used is *very* */
     /* wrong.  It should be the sequence number of the event, not */
     /* the request sequence number.  However, xmon never kept the */
     /* sequence number for the events, and I don't feel like */
     /* implementing that counter now.  Besides, I don't think that */
     /* counter is used for anything useful, anyway. */
     
     /* poke in the sequence number into bytes 2,3 */
     ISetShort(&MappingNotifyEventBuf[2], (unsigned short)
	       ((client->SequenceNumber +
		 client->SequenceMapping) & 0xFFFF));
     
     /* check  this */
     /* XXX - eventually, these values should be whatever the new */
     /* server range is, but this can't be done until the */
     /* supporting cast of routines in print11.c is set up, so put */
     /* in dummy values right now */
     
     /* poke in the min, count values */
     ISetByte(&MappingNotifyEventBuf[5], (unsigned char) 0x8);
     ISetByte(&MappingNotifyEventBuf[6], (unsigned char) 0x10);
     
     /* send it off to the client */
     
     SaveBytes(&(client->fdd->outBuffer), 
	       (unsigned char *)MappingNotifyEventBuf, 
	       MappingNotifyEventBufLength);
     BITSET(WriteDescriptors, client->fdd->fd);

     /* check this: not freeing the old server's information. How does
	this conflict with moving the client due to a request from another
	xmove? */
     
     /* can the old server.  Here we make sure that we disable this */
     /* file descriptor, so we don't bother listening to it, in */
     /* case the old server decides to start sending us information */
     /* about something or other */
     
     if (server->fdd->outBuffer.BlockSize > 0)
	  Tfree(server->fdd->outBuffer.data - server->fdd->outBuffer.data_offset);
     if (server->fdd->inBuffer.BlockSize > 0)
	  Tfree(server->fdd->inBuffer.data - server->fdd->inBuffer.data_offset);
     
     NotUsingFD(cur_fd);
     
     /* closing the file descriptor is enough to get the old server */
     /* to delete all the currently active windows */
     
     close(cur_fd);
     if (server->alt_fd)
	  close(server->alt_fd);
     
     if (server->server_name) free(server->server_name);
     while (item = deleteFirst(&server->reply_list))
	  Tfree(item);

     /* because the client and its original server share pointers to the same 
	format, don't free up the lists if this is the client's original
	server. */

     if (server->formats != client->formats) {
	  while (item = deleteFirst(&server->formats->format_list))
	       Tfree(item);
	  while (item = deleteFirst(&server->formats->depth_list)) {
	       Pointer item2;
	       
	       while (item2 = deleteFirst(&((DepthPtr)item)->visual_list))
		    Tfree(item2);
	       
	       Tfree(item);
	  }

	  free(server->formats);
     }

     free(server);

     return NULL;
}

/*
  FixClientSeqnos scans through all clients and fixes the SequenceMapping. This
  is called when the move is cancelled mid-way through. Since some requests have
  probably been sent to the original server in the interim, we need to update
  sequencemapping.
  */

static void
FixClientSeqnos()
{
     Client *init_client = client;
     LinkLeaf *ll;

     ScanList(&meta_client->client_list, ll, client, Client *) {
	  client->SequenceMapping += client->orig_seqno - client->SequenceNumber;
	  client->SequenceMapping &= 0xFFFF;
     }

     client = init_client;
}
     

/* note that this routine scans through all clients, not just the current one. */

static void
RemapWindows()
{
     int counter;
     void *pos;
     Client *init_client = client;
     LinkLeaf *ll;
     
     /* map all of the mapped windows in the correct stacking order */
     /* this is called when a move has failed and we need to restore the
	windows onto the original screen. window->mapped has already been
	set with the correct info from the beginning of the move. */
     
     ScanList(&meta_client->client_list, ll, client, Client *) {
	  WindowPtr window;
	  
	  SetGlobals();
	  
	  for (counter = 1; counter <= GetCurrentClientMaxMappedOrder(); counter++)
	  {
	       for (window = hash_getfirst(&pos, client->window_table);
		    window; window = hash_getnext(&pos, client->window_table))
	       {
		    if (!window->mapped)
			 continue;
		    
		    if (window->mapped_order == counter)
			 XMOVEMapWindow(cur_fd, cur_seqno, MapWindowID(window->window_id, Request));
		    
		    if (window->submapped_order == counter)
			 RemapStackWindows(window);
	       }
	  }
     }

     client = init_client;
     SetGlobals();
}

/* figure out the stacking of siblings that are mapped and recreate.
 * Variation on XmoveStackWindows, except this maps onto cur_fd
 */
static Window
RemapStackWindows(window_ptr)
WindowPtr window_ptr;
{
     Window xwin;
     unsigned char *stack;
     int num_windows, counter;
     Window cur_window;
     xResourceReq XGetReq;
     xQueryTreeReply XGetTRep;
     WindowPtr temp_window;

     cur_window = MapWindowID(window_ptr->window_id, Request);

     Dprintf(("XMOVE stack windows called for window 0x%x\n",
	    window_ptr->window_id))


     XGetReq.reqType = X_QueryTree;
     ISetShort((unsigned char *)&XGetReq.length, 2);
     ISetLong((unsigned char *)&XGetReq.id, cur_window);

     SendBuffer(cur_fd,
		(unsigned char *) &XGetReq,
		sizeof(xResourceReq));


     ReceiveReply(cur_fd, (unsigned char *) &XGetTRep, 32, ++(*cur_seqno));

     num_windows = IShort((unsigned char *)&XGetTRep.nChildren);
     
     stack = malloc(4*num_windows);
     
     ReceiveBuffer(cur_fd, stack, (4*num_windows));

     for (counter = 0; counter < num_windows; counter++)
     {
	  /* check this: new_display->screens should be replaced with a
	     check to the correct screen */
	  
	  xwin = ILong(stack+(4*counter));

	  Dprintf(("Stacking 0x%x parented by 0x%x\n",ILong(stack+(4*counter))
		 ,cur_window))

	  temp_window = FindWindowFromCurrentClient(MapWindowID(xwin, Reply));

	  if (temp_window && temp_window->mapped)
	       XMOVEMapWindow(cur_fd, cur_seqno, xwin);
     }

     free(stack);
}

/* take windows off the screen so as not to "annoy" the user */
static void
UnmapClientWindows()
{
     hash_location loc;
     WindowPtr window;

     for (window = hashloc_init(&loc, client->window_table);
	  window; window = hashloc_getnext(&loc))
     {
	  if ((window->parent_id & ~client->resource_mask) == 0) {
	       xResourceReq req;
	       xGetWindowAttributesReply rep;
	       Window new_win = MapWindowID(window->window_id, Request);

	       req.reqType = X_GetWindowAttributes;
	       ISetShort((unsigned char *)&req.length, 2);
	       ISetLong((unsigned char *)&req.id, new_win);

	       SendBuffer(cur_fd,
			  (unsigned char *) &req,
			  sizeof(xResourceReq));

	       ReceiveReply(cur_fd, (unsigned char *) &rep,
			    sizeof(xGetWindowAttributesReply), ++(*cur_seqno));

	       if (rep.mapState) {
		    window->mapped = 1;
		    XMOVEUnmapWindow(cur_fd, cur_seqno, new_win);
	       } else
		    window->mapped = 0;
	  }
     }
}	       

