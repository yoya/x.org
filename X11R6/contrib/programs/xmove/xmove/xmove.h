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
/*
 * Project: XMOVE - An X window mover
 *
 * File: xmove.h
 *
 */

/*
 *
 * $Author: pds $
 * $Date: 1993/02/06 23:15:37 $
 * $Locker: ethan $
 * $Revision: 1.9 $
 * $Log: xmove.h,v $
 * Revision 1.9  1993/02/06  23:15:37  pds
 * *** empty log message ***
 *
 * Revision 1.8  1992/12/15  18:31:57  ethan
 * stolen back by pds.
 *
 * Revision 1.7  1992/12/14  03:55:23  pds
 * *** empty log message ***
 *
 * Revision 1.6  1992/11/20  03:50:31  ethan
 * *** empty log message ***
 *
 * Revision 1.5  1992/11/16  22:25:46  pds
 * added some stuff to data structures to support removal of gc and pixmaps
 * after a switch to a new server.
 *
 * Revision 1.4  1992/11/16  22:22:52  ethan
 * *** empty log message ***
 *
 * Revision 1.3  1992/11/08  19:00:24  pds
 * *** empty log message ***
 *
 * Revision 1.2  1992/10/16  16:46:59  pds
 * *** empty log message ***
 *
 * Revision 1.1  1992/10/13  22:16:34  pds
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
 *
 */

#ifndef XMOVE_H
#define XMOVE_H

#include "common.h"
#include "xmond.h"
#include "hash.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>

/* standard X11 type definitions */

typedef char int8;
typedef unsigned char card8;

typedef short int16;
typedef unsigned short card16;

typedef long int32;
typedef unsigned long card32;

/*typedef enum {StaticGray, GrayScale, StaticColor, PseudoColor, TrueColor, DirectColor} VisualClass;*/

typedef struct
{
     char *name;

     /*void (*MapProperty) (AtomMapType, Atom *, Atom *, Atom *,
                            AtomPtr, Bool, Window *, char *, int);*/
     void (*MapProperty) ();
} AtomMappingRec, *AtomMappingPtr;

typedef struct
{
    VisualID id;
    card8 class;
    card8 bits_per_rgb;
    card16 colormap_entries;
    card32 red_mask, green_mask, blue_mask;
} VisualRec, *VisualPtr;

typedef struct
{
    int depth;
    LinkList visual_list;
} DepthRec, *DepthPtr;

typedef struct
{
    card8 depth;
    card8 bits_per_pixel;
    card8 scanline_pad;
} PixmapFormatRec, *PixmapFormatPtr;

/* Keep this structure a power of 2 bytes in length! Since it
   is used in an array, the optimizer can use shifts instead of
   multiplies to do memory references. */

typedef struct
{
    card32 server_pixel, client_pixel, new_server_pixel;
    unsigned long usage_count;	/* 0 == empty colorcell entry */
    card16 red, green, blue;
    Bool read_write;
    char *color_name;
} ColorCellRec, *ColorCellPtr;    

typedef struct
{
    Colormap colormap_id;
    VisualID visual_id;
    int cell_count;
    ColorCellPtr cell_array;
} ColormapRec, *ColormapPtr;

typedef struct
{
    Window window_id;
    Window parent_id;
    Window sibling_id;
    int8 sibling_stack_mode;
    int16 x,y;
    card16 width, height, border_width, class;
    Visual visual;
    card8 depth;
    XSetWindowAttributes attributes;
    unsigned long attributes_mask;
    ColormapPtr cmap;
    short Raise_Lowest;
    short Lower_Highest;
    short mapped;
    int mapped_order;
    int submapped_order;
    short tag;
} WindowRec, *WindowPtr;

typedef struct
{
    GContext gc_id;
    Drawable drawable_id;
    XGCValues values;
    unsigned long values_mask;

    card8 depth;
    ColormapPtr cmap;
} GCRec, *GCPtr;

typedef struct
{
    Font font_id;
    char *name;
} FontRec, *FontPtr;

typedef struct
{
    Cursor cursor_id;
    Font source_font, mask_font;
    unsigned int source_char, mask_char;
    XColor foreground_color, background_color;
} GlyphCursorRec, *GlyphCursorPtr;

typedef struct
{
    Cursor cursor_id;
    Pixmap source_pixmap, mask_pixmap;
    XColor foreground_color, background_color;
    int16 x,y;
} CursorRec, *CursorPtr;

typedef struct
{
    Atom client_atom;
    Atom server_atom;
    Atom new_server_atom;
    AtomMappingPtr property_map_func;
    AtomMappingPtr type_map_func;
    AtomMappingPtr transient_map_func;
    char *atom_name;
} AtomRec, *AtomPtr;

typedef struct
{
    Atom selection;
} SelectionRec, *SelectionPtr;

typedef struct
{
    Pixmap pixmap_id;
    Drawable drawable;
    unsigned int width, height, depth;

    /* temp storage of the newly created pixmap id for putimage loop */
    Pixmap new_pixmap;

    /* if a pixmap is used in a cursor which is then freed, we must first load
       the image, otherwise we cannot recreate the cursor. */
    Bool used_by_cursor;
    XImage *pixmap_image;

    /* Pointer to the colormap associated with this pixmap */
    ColormapPtr cmap;
    
} PixmapRec, *PixmapPtr;

typedef struct
{
  int type; /* the opcode from the grab */
  Window grab_window;
  Window confine_to;
  Cursor cursor;
  Time time;
  card16 event_mask;
  card16 modifiers;
  Bool owner_events;
  card8 pointer_mode;
  card8 keyboard_mode;
  card8 button;
  card8 key;
} GrabRec, *GrabPtr; 

typedef enum {Request, Event, Reply, Error, Server2Server, Client2NewServer} Direction;
typedef enum {MapChangeProp, MapDeleteProp, MapGetProp, MapGetPropReply,
	      MapClientMessage, MapConvertSelection, MapMoveClientProps,
	      MapSendClientMessage, MapSelectionRequest} AtomMapType;

/* global variables described in this file */

Global unsigned char *MappingNotifyEventBuf;
Global long MappingNotifyEventBufLength;

/* function prototypes specific to xmove */

/* xmove.c */

Global void InitXMove P((void));

Global void SetCurrentClientName P((char *name, int length));

Global char *xmovemalloc P((int n));
Global char *xmovecalloc P((int n, int size));

Global void AddGCToCurrentClient P((GCPtr gc));
Global void AddWindowToCurrentClient P((WindowPtr window));
Global void RemoveWindowFromCurrentClient P((Window window));
Global void ReparentWindowForCurrentClient P((Window window, Window parent,
					      int16 x, int16 y));
Global void RemoveGCFromCurrentClient P((GContext gc));
Global void RemoveFontFromCurrentClient P((Font font));
Global void RemoveCursorFromCurrentClient P((Cursor font));
Global void RemoveColormapFromCurrentClient P((Colormap xcolormap));

Global Client *FindClientFromBase P((unsigned long base));
Global WindowPtr FindWindowFromCurrentClient P((Window xwin));
Global WindowPtr FindWindowFromCurrentClient2 P((Window xwin));
Global PixmapPtr FindPixmapFromCurrentClient P((Pixmap pixmap));
Global ColormapPtr FindColormapFromCurrentClient P((Colormap cmap_id));
Global ColorCellPtr AllocNewColorCell P((card32 pixel, ColormapPtr cmap, Bool read_write, card16 red, card16 green, card16 blue, char *name));
/*Global ColorCellPtr FindColorCellFromColormap P((card32 pixel, ColormapPtr cmap));*/
#define FindColorCellFromColormap(p, c) ((c)->cell_array[(p)].usage_count ? &((c)->cell_array[(p)]) : NULL)
Global ColorCellPtr FindServerColorCellFromColormap P((card32 pixel, ColormapPtr cmap));
Global ColorCellPtr FindNewServerColorCellFromColormap P((card32 pixel, ColormapPtr cmap));
Global void RemoveColorCellFromColormap P((card32 pixel, ColormapPtr cmap));
Global AtomPtr FindAtom P((Atom atom));
Global AtomPtr FindServerAtom P((Atom atom));
Global AtomMappingPtr FindAtomMapping P((char *name));
Global AtomMappingPtr FindAtomTypeMapping P((char *name));
Global AtomMappingPtr AddAtomMapping P((char *name, void (*MapProperty)()));
Global AtomMappingPtr AddAtomTypeMapping P((char *name, void (*MapProperty)()));
Global AtomPtr CreateNewAtom P((Atom atom, char *name, Direction dir));
Global void ConvertGCToColormap P((GCPtr gc, ColormapPtr cmap, Bool *do_sync));
Global card32 *MakeServerColorCellMap P((ColormapPtr cmap, int server_depth));
Global void ConvertPixmapToColormap P((PixmapPtr pixmap, ColormapPtr cmap, Bool *do_sync));

Global void TagWindow P((WindowPtr window, short value));
Global void UntagWindow P((WindowPtr window));
Global void UntagAllWindows P(());

Global void AddGlyphCursorToCurrentClient P((GlyphCursorPtr cursor));
Global void AddCursorToCurrentClient P((CursorPtr cursor));
Global void AddPixmapToCurrentClient P((PixmapPtr pixmap));
Global void AddFontToCurrentClient P((FontPtr window));
Global void AddColormapToCurrentClient P((ColormapPtr colormap));

Global GCPtr FindGCFromCurrentClient P((GContext gc_id));

Global void ParseWindowBitmaskValueList P((WindowPtr window_ptr,
					   unsigned long cmask, 
					   unsigned char *ValueList));

Global void MapWindowBitmaskValueList P((WindowPtr window_ptr,
					   unsigned long cmask, 
					   unsigned char *ValueList));

Global void ParseGCBitmaskValueList P((GCPtr window_ptr,
				       unsigned long cmask, 
				       unsigned char *ValueList));


Global void MapGCBitmaskValueList P((GCPtr window_ptr,
				       unsigned long cmask, 
				       unsigned char *ValueList));


Global void ParseConfigWindowBitmaskValueList P((WindowPtr window_ptr,
						 unsigned short cmask, 
						 unsigned char *ValueList));

/* map.h */
Global ResourceMappingPtr FindResourceBase P((unsigned long base));
Global ResourceMappingPtr FindServerResourceBase P((unsigned long base));
Global unsigned long MapResource P((unsigned long resource, Direction dir, unsigned long primary));
#define MapDrawable(d, dir) ((Drawable)MapWindowID((Window)(d), (dir)))
#define MapPixmapID(p, dir) (MapResource((p), (dir), (p)))
#define MapGCID(g, dir) (MapResource((g), (dir), (g)))
#define MapFontID(f, dir) (MapResource((f), (dir), (f)))
#define MapCursorID(c, dir) (MapResource((c), (dir), (c)))
Global Window   MapWindowID P((Window xwin, Direction dir));
Global Colormap MapColormapID P((Colormap xcmap, Direction dir));
Global VisualID MapVisualID P((VisualID xvisual, Direction dir));
Global card32   MapColorCell P((card32 pixel, ColormapPtr cmap, Direction dir));
Global Atom     MapAtom P((Atom xatom, Direction dir));
Global AtomPtr  MapNewAtom P((Atom xatom, Direction dir));

/* get_image.h */

Global XImage *XMOVEGetImage P((int fd, unsigned short *seqno, register Server *server, Drawable d, int x, int y, unsigned int width, unsigned int height, unsigned long plane_mask, int format));
Global XImage *XMOVEPreCreateImage P((ImageFormatPtr image_format, VisualPtr visual, int format, GCPtr gc, unsigned int plane_mask, unsigned int depth, char *data, unsigned int width, unsigned int height));
Global XImage *XMOVECreateImage P((register ImageFormatPtr image_format, register VisualPtr visual, unsigned int depth, int format, int offset, char *data, unsigned int width, unsigned int height, int xpad, int image_bytes_per_line));

/* print11.h */

Global void SaveDepths P((Server *server, unsigned char *buf));
Global void SaveFormats P((Server *server, unsigned char *buf, int formats));

/* move.h */

Global void MoveAll P((char *new_display_name, char new_screen, MetaClient *exclude_client, int keylen, char *key, char **rettext));
Global void MoveClient P((char *new_display_name, char new_screen, int keylen, char *key, char **rettext));

/* colormaps.h */

Global char *MoveColormaps P((void));
Global void MoveColorCells P((int new_fd, unsigned short *seqno, ColormapPtr cmap, Colormap mid));
Global void MoveColorCell P((int new_fd, unsigned short *seqno, ColorCellPtr cell, Colormap mid));

/* XMOVELib.h */

Global Bool SendBuffer P((int fd, unsigned char *buf, int BytesToWrite));
Global Bool ReceiveBuffer P((int fd, unsigned char *buf, int BytesToRead));
Global Bool ReceiveReply P((int fd, unsigned char *buf, int BytesToRead, unsigned short SequenceNumber));
Global Font XMOVELoadFont P((int fd, unsigned short *seqno, Font id, char *name));
Global Atom XMOVEInternAtom P((int fd, unsigned short *seqno, char *name, Bool onlyifexists));
Global Cursor XMOVECreateGlyphCursor P((int fd, unsigned short *seqno, Cursor id, Font source_font, Font mask_font, unsigned int source_char, unsigned int mask_char, XColor *foreground, XColor *background));
Global Pixmap XMOVECreatePixmap P((int fd, unsigned short *seqno, Pixmap id, Drawable d, unsigned int width, unsigned int height, unsigned int depth));
Global void XMOVESetSelectionOwner P((int fd, unsigned short *seqno, Atom selection, Window owner, Time time));
Global Window XMOVEGetSelectionOwner P((int fd, unsigned short *seqno, Atom selection));
Global GContext XMOVECreateGC P((int fd, unsigned short *seqno, GContext id, Drawable d, unsigned long valuemask, XGCValues *values));
Global Cursor XMOVECreatePixmapCursor P((int fd, unsigned short *seqno, Cursor id, Pixmap source, Pixmap mask, XColor *foreground, XColor *background, unsigned int x, unsigned int y));
Global void XMOVEFreeGC P((int fd, unsigned short *seqno, GContext gc));
Global void XMOVEFreePixmap P((int fd, unsigned short *seqno, Pixmap pixmap));
Global void XMOVEMapWindow P((int fd, unsigned short *seqno, Window w));
Global Window XMOVECreateWindow P((int fd, unsigned short *seqno, Window id, Window parent, int x, int y, unsigned int width, unsigned int height, unsigned int border_width, int depth, unsigned int class, Visual *visual, unsigned long valuemask, XSetWindowAttributes *attributes));
Global void XMOVEGrabButton P((int fd, unsigned short *seqno, unsigned int modifiers, unsigned int button, Window grab_window, Bool owner_events, unsigned int event_mask, int pointer_mode, int keyboard_mode, Window confine_to, Cursor curs));
Global void XMOVEGrabKey P((int fd, unsigned short *seqno, int key, unsigned int modifiers, Window grab_window, Bool owner_events, int pointer_mode, int keyboard_mode));
Global void XMOVEChangeProperty P((int fd, unsigned short *seqno, Window w, Atom property, Atom type, int format, int mode, unsigned char *data, int nelements));
Global Status XMOVEAllocColor P((int fd, unsigned short *seqno, Colormap cmap, XColor *def));
Global Status XMOVEAllocNamedColor P((int fd, unsigned short *seqno, Colormap cmap, char *colorname, XColor *hard_def, XColor *exact_def));
Global void XMOVEStoreColor P((int fd, unsigned short *seqno, Colormap cmap, XColor *def));
Global void XMOVEStoreNamedColor P((int fd, unsigned short *seqno, Colormap cmap, char *name, unsigned long pixel, int flags));
Global void XMOVEQueryColor P((int fd, unsigned short *seqno, Colormap cmap, XColor *def));
Global Status XMOVEAllocColorCells P((int fd, unsigned short *seqno, Colormap cmap, Bool contig, unsigned long *masks, unsigned int nplanes, unsigned long *pixels, unsigned int ncolors));
Global Colormap XMOVECreateColormap P((int fd, unsigned short *seqno, Pixmap mid, Window w, VisualID visual, int alloc));
Global Status XMOVELookupColor P((int fd, unsigned short *seqno, Colormap cmap, char *spec, XColor *def, XColor *scr));
Global char *XMOVEGetAtomName P((int fd, unsigned short *seqno, Atom atom, Direction dir));
Global void XMOVEUnmapWindow P((int fd, unsigned short *seqno, Window w));
Global void XMOVEGetInputFocus P((int fd, unsigned short *seqno));
Global Bool XMOVEVerifyRequest P((int fd, unsigned short *seqno));

/* MoveImage.h */

Global void CopyImage P((XImage *image, XImage *new_image, unsigned int width, unsigned int height));
Global void MoveImage P((XImage *image, XImage *new_image, unsigned int width, unsigned int height, ColormapPtr cmap, Direction dir));

/* move_window.h */

Global Window move_window P((WindowPtr window_ptr, unsigned char new_depth));
Global Window XMOVEStackWindows P((WindowPtr window_ptr));

/* getsetProps.h */

Global void XMOVEGetSetProps P((Window oldwin, Window newwin, Bool ForceWindowPosition, int x, int y, int width, int height));

/* XMOVEPutImage.h */

Global void XMOVEPutImage P((int fd, unsigned short *seqno, register ImageFormatPtr image_format, Drawable d, GC gc, register XImage *image, int req_xoffset, int req_yoffset, int x, int y, unsigned int req_width, unsigned int req_height));

/* move_selections.h */

Global void move_selections P(());

/* check_auth.h */

Global Bool CheckAuth P((unsigned long ip_addr, int typelen, unsigned char *type, int keylen, unsigned char *key));
Global void InitMagicCookie P((char *DefaultHost, char *LocalHostName, int ListenForClientsPort));

/* global variables within xmove */

extern Bool PrintDebug;
extern Bool MoveInProgress;
extern Server *new_server, *server;
extern unsigned int cur_mask, cur_base, new_mask, new_base, client_mask, client_base;
extern int cur_fd, new_fd;
extern unsigned short *cur_seqno, *new_seqno;
extern Client *client;
extern MetaClient *meta_client;
extern char AuthType[256], AuthKey[256];
extern int AuthKeyLen;
extern Display *DefaultDisp;
extern unsigned char *ValidSetUpMessage;
extern int ValidSetUpMessageLen;

/* client/server access routines */

#define SetCurrentClientSequenceID(seqno) (client->SequenceNumber = seqno)
#define SetCurrentClientMaxMappedOrder(order) (client->max_mapped_order = order)
#define GetCurrentClientMaxMappedOrder() (client->max_mapped_order)
#define SetCurrentServerMaxReqLength(len) (server->max_request_size = server->formats->max_request_size = len)
#define SetCurrentClientMinKeycode(keycode) (client->min_keycode = keycode)
#define SetCurrentClientMaxKeycode(keycode) (client->max_keycode = keycode)
#define SetCurrentServerMinKeycode(keycode) (server->min_keycode = keycode)
#define SetCurrentServerImageByteOrder(order) (server->formats->image_byte_order = order)
#define SetCurrentServerImageBitmapUnit(unit) (server->formats->image_bitmap_unit = unit)
#define SetCurrentServerImageBitmapBitOrder(order) (server->formats->image_bitmap_bit_order = order)
#define SetCurrentServerImageBitmapPad(pad) (server->formats->image_bitmap_pad = pad)
#define SetCurrentServerMaxKeycode(keycode) (server->max_keycode = keycode)
#define GetCurrentClientMinKeycode() (client->min_keycode)
#define GetCurrentClientMaxKeycode() (client->max_keycode)
#define GetCurrentServerMinKeycode() (server->min_keycode)
#define GetCurrentServerMaxKeycode() (server->max_keycode)
#define GetCurrentClientName() (client->window_name)
#define SetCurrentClientRootWindow(root) (client->root_window = root)
#define SetCurrentServerRootWindow(root) (server->root_window = root)
#define GetCurrentClientRootWindow() (client->root_window)
#define GetCurrentServerRootWindow() (server->root_window)
#define SetCurrentServerDefaultDepth(depth) (server->default_depth = depth)
#define GetCurrentServerDefaultDepth() (server->default_depth)
#define SetCurrentServerDefaultVisualID(vid) (server->default_vid = vid)
#define GetCurrentServerDefaultVisualID() (server->default_vid)
#define SetCurrentClientDefaultVisualID(vid) (client->default_vid = vid)
#define GetCurrentClientDefaultVisualID() (client->default_vid)
#define SetCurrentServerDefaultColormap(map) (server->default_cmap = map)
#define SetCurrentClientDefaultColormap(map) (client->default_cmap = map)
#define GetCurrentClientDefaultColormap() (client->default_cmap)
#define GetCurrentServerDefaultColormap() (server->default_cmap)

#endif /* XMOVE_H */

