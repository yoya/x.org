/*
Copyright (c) 1994  Matthew Reynolds and Qualcomm Inc.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
QUALCOMM INC., OR MATTHEW REYNOLDS BE LIABLE FOR ANY CLAIM, 
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE 
OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of Qualcomm Inc., or Matthew 
Reynolds shall not be used in advertising or otherwise to promote the sale, 
use or other dealings in this Software without their prior written 
authorization.

*/

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#define NEED_REPLIES 1
#include <X11/Xproto.h>
#include <X11/Xlib.h>
#include "printer.h"
#include "printlib.h"
#include "printlibP.h"

/*
  There are 4 types of things in X11: requests, replies, errors, and events.

  Each of them has a format defined by a small integer that defines
  the type of the thing.

  Requests have an opcode in the first byte.
  Events have a code in the first byte.
  Errors have a code in the second byte (the first byte is 0)
  Replies ...

  Replies have a sequence number in bytes 2 and 3.  The sequence
  number should be used to identify the request that was sent, and
  from that request we can determine the type of the reply.
*/

#define REQ_HEADERSIZE  sizeof( xReq)
#define RPLY_HEADERSIZE sizeof( xGenericReply)
#define EXTENSION_BASE  128

/* opcode for our private protocol */

extern int debug;
int THE_OPCODE = 144;
extern Display *THE_DISPLAY;
extern struct PrintClient PCS[ STATICMAXFD];

/* font information mapping tables and cache of loaded fonts */

extern int              num_cache_font;

/* standard GC created with all default values */

XGCValues initial_GC = {
    GXcopy,     	/* function */
    AllPlanes,  	/* plane_mask */
    0L,         	/* foreground */
    1L,         	/* background */
    0,          	/* line_width */
    LineSolid,  	/* line_style */
    CapButt,    	/* cap_style */
    JoinMiter,  	/* join_style */
    FillSolid,  	/* fill_style */
    EvenOddRule,	/* fill_rule */
    ArcPieSlice,	/* arc_mode */
    ~0L,        	/* tile, impossible (unknown) resource */
    ~0L,        	/* stipple, impossible (unknown) resource */
    0,          	/* ts_x_origin */
    0,          	/* ts_y_origin */
    ~0L,        	/* font, impossible (unknown) resource */
    ClipByChildren, 	/* subwindow_mode */
    True,       	/* graphics_exposures */
    0,          	/* clip_x_origin */
    0,          	/* clip_y_origin */
    None,       	/* clip_mask */
    0,          	/* dash_offset */
    4           	/* dashes (list [4,4]) */
};

XWindowAttributes initial_Attrs = {
    0,			/* x */
    0,			/* y */
    0,			/* width */
    0,			/* height */
    0,			/* border width */
    8,			/* default depth */
    NULL, 		/* visual */
    0,			/* root window of containing screen */
    InputOutput,	/* class */
    0,			/* bit gravity */
    0, 			/* win gravity */
    NotUseful,		/* backing store */
    0,			/* backing pixel */
    False,		/* save under */
    0,			/* colormap */
    False,		/* colormap installed flag */
    IsUnmapped,		/* map state */
    0,			/* all event mask */
    0,			/* my event mask */
    0,			/* do-not-propogate mask */
    False,		/* override-redirect */
    NULL		/* screen pointer */
};

/* set window attributes from a request */

void set_winattrs( XWindowAttributes	*attr,
		   unsigned long	value_mask,
		   unsigned long	*request)
{
#if 0
    if ( value_mask & CWBackPixmap) {
	attr->background_pixmap = *request++;
    }
    if ( value_mask & CWBackPixel) {
	attr->background_pixel = *request++;
    }
    if ( value_mask & CWBorderPixmap) {
	attr->border_pixmap = *request++;
    }
#endif
    if ( value_mask & CWBitGravity) {
	attr->bit_gravity = *request++;
    }
    if ( value_mask & CWWinGravity) {	
	attr->win_gravity = *request++;
    }
    if ( value_mask & CWBackingStore) {	
	attr->backing_store = *request++;
    }
    if ( value_mask & CWBackingPlanes) {	
	attr->backing_planes = *request++;
    }
    if ( value_mask & CWBackingPixel) {	
	attr->backing_pixel = *request++;
    }
    if ( value_mask & CWOverrideRedirect) {	
	attr->override_redirect = *request++;
    }
    if ( value_mask & CWSaveUnder) {	
	attr->save_under = *request++;
    }
#if 0
    if ( value_mask & CWEventMask) {	
	attr->event_mask = *request++;
    }
#endif
    if ( value_mask & CWDontPropagate) {	
	attr->do_not_propagate_mask = *request++;
    }
    if ( value_mask & CWColormap) {	
	attr->colormap = *request++;
    }
#if 0
    if ( value_mask & CWCursor) {	
	attr->cursor = *request++;
    }
#endif
}

/* request decoder for graphics commands */

winfo_table_p		winfo_table = NULL;
gcinfo_table_p		gcinfo_table = NULL;
cmapinfo_table_p	cmapinfo_table = NULL;
atom_table_p		atom_table = NULL;
font_table_p		font_table = NULL;

atom_p getatom( unsigned long	atom,
		atom_table_p	atom_table)
{
    int	i;

    for ( i = 0; i < atom_table->num_atoms; i++) {
	if( atom_table->atom_table[ i].id == atom) {
	    return( &atom_table->atom_table[ i]);
	}
    }
    return( NULL);
}

/* get the colormap attributes corresponding to a colormap id */

cmapinfo_p getcmapinfo( unsigned long 		cmap,
		        cmapinfo_table_p	table)
{
    int i;

    for ( i = 0; i < table->num_cmap; i++) {
	if ( table->cmap_table[ i].cmap == cmap) {
	    return( &( table->cmap_table[ i]));
	}
    }

/* set the pixels and allocation for this colormap.  If it's the default
   colormap, set the writeable flags to false */

    for ( i = 0; i < NUM_COLORS; i++) {
	table->cmap_table[ table->num_cmap].colors[ i].pixel = i;
	table->cmap_table[ table->num_cmap].num_alloc[ i] = 0;
	table->cmap_table[ table->num_cmap].writeable[ i] = TRUE;
#if 0
	   ( cmap == DefaultColormap( THE_DISPLAY, 
				      DefaultScreen( THE_DISPLAY)));
#endif
    }
    table->cmap_table[ table->num_cmap++].cmap = cmap;
    return( &table->cmap_table[ table->num_cmap - 1]);
}

/* get the GC attributes corresponding to a GC id */

gcinfo_p getgcinfo( unsigned long 	gcid, 
		    gcinfo_table_p 	table)
{
    int i;

    for ( i = 0; i < table->num_gc; i++) {
	if ( table->gc_table[ i].gcid == gcid) {
	    return( &( table->gc_table[ i]));
	}
    }
    return( NULL);
}

/* get the window attributes corresponding to a window */

winfo_p getwinfo( unsigned long	win,
		  winfo_table_p	table)
{
    int i;

    for ( i = 0; i < table->num_win; i++) {
	if ( table->win_table[ i].win == win) {
	    return( &( table->win_table[ i]));
	}
    }

/* this probably indicates that the window is on the REAL server */

    return( NULL);
}

/* remove the window structures associated with a client */

void removewins( int		fd,
		 winfo_table_p	table)
{
    int	i, j;

    for ( i = 0, j = 0; i < table->num_win; i++) {
	if ( table->win_table[ i].fd != fd) {
	    table->win_table[ j++] = table->win_table[ i];
	}
    }
    table->num_win = j;
}

/* remove the GC structures associate with a client */

void removegcs( int		fd,
		gcinfo_table_p	table)
{
    int	i, j;

    for ( i = 0, j = 0; i < table->num_gc; i++) {
	if ( table->gc_table[ i].fd != fd) {
	    table->gc_table[ j++] = table->gc_table[ i];
	}
    }
    table->num_gc = j;
}

/* remove the cmap structures associate with a client */

void removecmaps( int			fd,
		  cmapinfo_table_p	table)
{
    int	i, j;

    for ( i = 0, j = 0; i < table->num_cmap; i++) {
	if ( table->cmap_table[ i].fd != fd) {
	    table->cmap_table[ j++] = table->cmap_table[ i];
	}
    }
    table->num_cmap = j;
}

/* delete a window from a table of windows */

void delete_win( winfo_table_p	table,
		 unsigned long	win)
{
    int	i, j;

    for ( i = 0, j = 0; i < table->num_win; i++) {
	if ( table->win_table[ i].win != win) {
	    table->win_table[ j++] = table->win_table[ i];
	}
    }
    table->num_win = j;
}

/* delete a GC from a table of GCs */

void delete_gc( gcinfo_table_p	table,
		unsigned long	gcid)
{
    int	i, j;

    for ( i = 0, j = 0; i < table->num_gc; i++) {
	if ( table->gc_table[ i].gcid != gcid) {
	    table->gc_table[ j++] = table->gc_table[ i];
	}
    }
    table->num_gc = j;
}

/* delete a colormap from a table of colormaps */

void delete_cmap( cmapinfo_table_p	table,
		  unsigned long		cmap)
{
    int	i, j;

    for ( i = 0, j = 0; i < table->num_cmap; i++) {
	if ( table->cmap_table[ i].cmap != cmap) {
	    table->cmap_table[ j++] = table->cmap_table[ i];
	}
    }
    table->num_cmap = j;
}
		  
/* clean up the structures associated with a client */

void remove_client( int	fd)
{
    removewins( fd, winfo_table);
    removegcs( fd, gcinfo_table);
    removecmaps( fd, cmapinfo_table);
}

/* return TRUE/FALSE as to whether this request should be forwarded to the
   server */

Bool DecodeRequest( FD 		  	fd,
		    unsigned char 	*buf,
		    unsigned char	*newmsg,
		    int			*newlen)
{
    short   		Request = *buf;
    unsigned long	value_mask, *request;
    unsigned short	val, x, y, width, height;
    unsigned char	flag, depth;
    XGCValues		*gcvp;
    int			index, i, nrect, nextensions;
    winfo_p		winfo;
    gcinfo_p		gcinfo;
    cmapinfo_p		cmapinfo;
    atom_p		atom;
    FILE		*print_fp = PCS[ fd].printfp;
    Drawable		drawable = PCS[ fd].win;
    xGenericReply	*reply = ( xGenericReply *) newmsg;

    *newlen = 0;

/* set reply sequence # in advance */

    reply->sequenceNumber = ++(PCS[ fd].sequence);
    reply->type = X_Reply;

/* initialize the window and gc tables if they haven't been created yet. */

    if ( winfo_table == NULL) {

/* have to define the RootWindow (and the defaultGC, I think) */

	winfo_table = ( winfo_table_p) malloc( sizeof( winfo_table_s));
	winfo_table->win_table = ( winfo_p) malloc( sizeof( winfo_s)*NUMWIN);
	winfo_table->num_win = 1;
	winfo_table->win_table[ 0].win = 0;
	winfo_table->win_table[ 0].attr = initial_Attrs;
	gcinfo_table = ( gcinfo_table_p) malloc( sizeof( gcinfo_table_s));
	gcinfo_table->gc_table = ( gcinfo_p) malloc( sizeof( gcinfo_s)*NUMGC);
	gcinfo_table->num_gc = 0;
	cmapinfo_table = 
	    ( cmapinfo_table_p) malloc( sizeof( cmapinfo_table_s));
	cmapinfo_table->cmap_table = 
	    ( cmapinfo_p) malloc( sizeof( cmapinfo_s)*NUMGC);
	cmapinfo_table->num_cmap = 0;
	atom_table = ( atom_table_p) malloc( sizeof( atom_table_s));
	atom_table->atom_table = ( atom_p) malloc( sizeof( atom_s)*NUMATOM);
	atom_table->num_atoms = 0;
	font_table = ( font_table_p) malloc( sizeof( font_table_s));
	font_table->font_table = ( font_p) malloc( sizeof( font_s)*NUMFONT);
	font_table->num_fonts = 0;
    }
if ( debug) printf( "request opcode = %d\n", Request);
    if ( Request <= 0 || 127 < Request) {
	xOpenPrintReq   *open_req;
	xClosePrintReq  *close_req;
	xSetPrintReq    *set_req;
	xPrintReply	*print_rply = ( xPrintReply *) newmsg;

	if ( *buf == THE_OPCODE) {
	    print_rply->type = 1;
	    print_rply->length = 0;
	    switch( buf[ 1]) {
	    case X_OpenPrint:

/* read the print request */

		open_req = ( xOpenPrintReq *) buf;
		PCS[ fd].printfile = 
		    ( char *) malloc( open_req->strlength + 1);
		strncpy( PCS[ fd].printfile, open_req->filename, 
			 open_req->strlength);
		PCS[ fd].printfile[ open_req->strlength] = '\0';
		PCS[ fd].printfp = fopen( PCS[ fd].printfile, "w");
		print_rply->flag = ( PCS[ fd].printfp != NULL);
		PCS[ fd].win = open_req->drawable;

/* put in the postscript header information */

		if ( print_rply->flag) {
		    initprocs( &PCS[ fd], fd);
		}
		*newlen = sizeof( print_rply);
		break;

/* print showpage, and close the file */

	    case X_ClosePrint:
		close_req = ( xClosePrintReq *) buf;
		if ( PCS[ fd].printfp != NULL) {
		    fprintf( PCS[ fd].printfp, "\nshowpage\n");
		    fclose( PCS[ fd].printfp);
		    PCS[ fd].printfp = NULL;
		}
		break;

/* for setting printer attributes, can be extended */

	    case X_SetPrint:
		set_req = ( xSetPrintReq *) buf;
		PCS[ fd].preserve_aspect = set_req->aspect;
		PCS[ fd].font_scale = set_req->fontScale;
		PCS[ fd].pixmap_scale = set_req->pixmapScale;
		PCS[ fd].page_width = set_req->pageWidth;
		PCS[ fd].page_height = set_req->pageHeight;
		break;
	    default:
		fprintf( stderr, "received erroroneous minor opcode\n");
		break;
	    }
	}
    } else {
	switch (Request) {
	case X_CreateWindow:
	    {
		xCreateWindowReq *req = ( xCreateWindowReq *) buf;
		XWindowAttributes	*attr;
		
		index = winfo_table->num_win++;
		winfo_table->win_table[ index].created = TRUE;
		winfo_table->win_table[ index].win = req->wid;
		winfo_table->win_table[ index].parent = req->parent;
		winfo_table->win_table[ index].fd = fd;
		memcpy(( void *) &winfo_table->win_table[ index].attr,
		       ( void *) &initial_Attrs, sizeof( XWindowAttributes));
		attr = &winfo_table->win_table[ index].attr;
		attr->x = req->x;
		attr->y = req->y;
		attr->width = req->width;
		attr->height = req->height;
		attr->class = req->class;
		attr->colormap = 
		    DefaultColormap( THE_DISPLAY, DefaultScreen( THE_DISPLAY));
#if 0
		attr->visual = req->visual; /* FTM, we have only one visual */
#endif
		set_winattrs( attr, req->mask, 
			      ( unsigned long *) 
			       ( buf + sizeof( xCreateWindowReq)));
	    }
	    return( FALSE);		/* don't forward the request */
	    break;
	case X_ChangeWindowAttributes:
	    {
		xChangeWindowAttributesReq 	*req =
		    ( xChangeWindowAttributesReq *) buf;

		winfo = getwinfo( PCS[ fd].win, winfo_table);
		if ( winfo != NULL) {
		    set_winattrs( &winfo->attr, req->valueMask,
				  ( unsigned long *)
				   ( buf + 
				     sizeof( xChangeWindowAttributesReq)));
		} else {
		    fprintf( stderr, "X_ChangeWindowAttributes bad window %d\n",
			     req->window);
		}
	    }	
	    break;
	case X_GetWindowAttributes: 
	    {
		xResourceReq			*req = ( xResourceReq *) buf;
		xGetWindowAttributesReply	*reply = 
		    ( xGetWindowAttributesReply *) newmsg;
		
		winfo = getwinfo( req->id, winfo_table);
		if ( winfo != NULL) {
		    reply->backingStore = winfo->attr.backing_store;
		    reply->length = 3;
		    reply->class = winfo->attr.class;
		    reply->bitGravity = winfo->attr.bit_gravity;
		    reply->winGravity = winfo->attr.win_gravity;
		    reply->backingBitPlanes = winfo->attr.backing_planes;
		    reply->backingPixel = winfo->attr.backing_pixel;
		    reply->saveUnder = winfo->attr.save_under;
		    reply->colormap = winfo->attr.colormap;
		    reply->allEventMasks = winfo->attr.all_event_masks;
		    reply->doNotPropagateMask = 
			winfo->attr.do_not_propagate_mask;
		    *newlen = sizeof( *reply);
		} else {
		    fprintf( stderr, "X_GetWindowAttributes bad window %d\n",
			     req->id);
		}
	    }
	    break;
	case X_DestroyWindow:
	    {
		xResourceReq	*req = ( xResourceReq *) buf;

		delete_win( winfo_table, req->id);
	    }
	    break;
	case X_DestroySubwindows:	/* I really should do something here */
	    break;
	case X_ChangeSaveSet:
	    break;
	case X_ReparentWindow:
	    break;
	case X_MapWindow:
	    break;
	case X_MapSubwindows:
	    break;
	case X_UnmapWindow:
	    break;
	case X_UnmapSubwindows:
	    break;
	case X_ConfigureWindow:
	    break;
	case X_CirculateWindow:
	    break;
	case X_GetGeometry:
	    { 
		xResourceReq	   *req = ( xResourceReq *) buf;
		xGetGeometryReply  *reply = ( xGetGeometryReply *) newmsg;

		winfo = getwinfo( req->id, winfo_table);
		if ( winfo != NULL) {
		    reply->x = winfo->attr.x;
		    reply->y = winfo->attr.y;
		    reply->width = winfo->attr.width;
		    reply->height = winfo->attr.height;
		    reply->depth = winfo->attr.depth;
		    reply->length = 0;
		    reply->root = winfo_table->win_table[ 0].win;
		    reply->borderWidth = winfo->attr.border_width;
		    *newlen = sizeof( *reply);
		} else {
		    fprintf( stderr, "X_GetGeometry bad window id %d\n",
			     req->id);
		}
	    }
	    break;
	case X_QueryTree:
	    {
		xResourceReq	*req = ( xResourceReq *) buf;
		xQueryTreeReply	*reply = ( xQueryTreeReply *) newmsg;

		winfo = getwinfo( req->id, winfo_table);
		if ( winfo != NULL) {
		    reply->root = winfo_table->win_table[ 0].win;
		    reply->parent = winfo->parent;

/* FTM, number of children is zero */

		    reply->nChildren = 0;
		    reply->length = 0;
		    *newlen = sizeof( *reply);
		} else {
		    fprintf( stderr, "X_QueryTree bad window id %d\n", req->id);
		}
	    }	
	    break;
	case X_InternAtom:
	    {
		xInternAtomReq	*req = ( xInternAtomReq	*) buf;
		xInternAtomReply *reply = ( xInternAtomReply *) newmsg;

		atom_table->atom_table[ atom_table->num_atoms].id = 
		    atom_table->num_atoms;
		atom_table->atom_table[ atom_table->num_atoms].name = 
		    ( char *) malloc( req->nbytes + 1);
		strncpy( atom_table->atom_table[ atom_table->num_atoms].name,
			 ( char *) buf + sizeof( *req), req->nbytes + 1);
		reply->atom = atom_table->num_atoms++;
		*newlen = sizeof( *reply);
	    }
	    break;
	case X_GetAtomName:
	    {
		xResourceReq	*req = ( xResourceReq *) buf;
		xGetAtomNameReply *reply = ( xGetAtomNameReply *) newmsg;

		atom = getatom( req->id, atom_table);
		if ( atom != NULL) {
		    reply->nameLength = strlen( atom->name);
		    strncpy(( char *) newmsg + sizeof( *reply), 
			    ( char *) newmsg, reply->nameLength); 
		} else {
		    reply->nameLength = 0;
		}
		*newlen = sizeof( *reply);
	    }	
	    break;
	case X_ChangeProperty:
	    break;
	case X_DeleteProperty:
	    break;
	case X_GetProperty:
	    {

/* FTM, don't return ANY properties */

		xGetPropertyReq		*req = ( xGetPropertyReq *) buf;
		xGetPropertyReply	*reply = ( xGetPropertyReply *) newmsg;

		reply->propertyType = None;
		reply->bytesAfter = 0;
		reply->length = 0;
		reply->format = 0;
		reply->nItems = 0;
		*newlen = sizeof( *reply);
	    }
	    break;
	case X_ListProperties:
	    { 
		xListPropertiesReply	*reply = 
		    ( xListPropertiesReply *) newmsg;
		
		reply->nProperties = 0;
		*newlen = sizeof( *reply);
	    }
	    break;
	case X_SetSelectionOwner:
	    break;
	case X_GetSelectionOwner:
	    {
		 xGetSelectionOwnerReply	*reply = 
		     ( xGetSelectionOwnerReply *) newmsg;
		 
		 reply->owner = 0;
		 *newlen = sizeof( *reply);
	    }
	    break;
	case X_ConvertSelection:
	    break;
	case X_SendEvent:
	    break;
	case X_GrabPointer:
	    {
		xGrabPointerReply	*reply = ( xGrabPointerReply *) newmsg;

		reply->status = False;
		*newlen = sizeof( *reply);
	    }
	    break;
	case X_UngrabPointer:
	    break;
	case X_GrabButton:
	    break;
	case X_UngrabButton:
	    break;
	case X_ChangeActivePointerGrab:
	    break;
	case X_GrabKeyboard:
	    {
		xGrabKeyboardReply	*reply = ( xGrabKeyboardReply *) newmsg;

		reply->status = False;
		*newlen = sizeof( *reply);
	    }
	    break;
	    break;
	case X_UngrabKeyboard:
	    break;
	case X_GrabKey:
	    break;
	case X_UngrabKey:
	    break;
	case X_AllowEvents:
	    break;
	case X_GrabServer:
	    break;
	case X_UngrabServer:
	    break;
	case X_QueryPointer:

/* obviously, this is truly bogus */

	    {
		xQueryPointerReply	*reply = ( xQueryPointerReply *) newmsg;

		reply->root = 0;
		reply->child = 0;
		reply->rootX = 0;
		reply->rootY = 0;
		reply->winX = 0;
		reply->winY = 0;
		*newlen = sizeof( *reply);
	    }
	    break;
	case X_GetMotionEvents:
	    {
		xGetMotionEventsReply	*reply = 
		    ( xGetMotionEventsReply *) newmsg;
		
		reply->nEvents =0;
		*newlen = sizeof( *reply);
	    }
	    break;
	case X_TranslateCoords:
	    {
		xTranslateCoordsReply	*reply = 
		    ( xTranslateCoordsReply *) newmsg;
		
		reply->dstX = 0;
		reply->dstY = 0;
		*newlen = sizeof( *reply);
	    }
	    break;
	case X_WarpPointer:
	    break;
	case X_SetInputFocus:
	    break;
	case X_GetInputFocus:
	    {
		xReq			*req = ( xReq *) buf;
		xGetInputFocusReply	*reply = 
			( xGetInputFocusReply *) newmsg;

		reply->focus = 0;	/* make it the root window */
		reply->revertTo = None;
		reply->length = 0;	/* the documentation says this is 0 */
		*newlen = sizeof( *reply);
	    }
	    break;
	case X_QueryKeymap:	/* needs reply */
	    break;
	case X_OpenFont:

/* add the font to the font cache. */

	    {
		xOpenFontReq	*req = ( xOpenFontReq *) buf;
		char		*font_name = 
				    ( char *) buf + sizeof( xOpenFontReq);
		int		found = FALSE, n = font_table->num_fonts;
		font_p		font;

		for ( i = 0; i < font_table->num_fonts; i++) {
		    if ( !strncmp( font_table->font_table[ i].xfont_name, 
				   font_name, req->nbytes)) {
			found = TRUE;
		    }
		}

/* save the name and font ID, get the postscript name for the font, 
   then query the font for the font struct information */

		if ( !found) {
		    font = &font_table->font_table[ n];
		    font->fd = fd;
		    font->fid = req->fid;
		    font->server_fid = 0;
		    font->xfont_name = 
			( char *) malloc( req->nbytes + 1);
		    strncpy( font->xfont_name, font_name, req->nbytes);
		    font->post_name = get_post_name( font->xfont_name);
		    if ( font->post_name == NULL) {
			fprintf( stderr, "PostScript font for X %s not found\n",
				 font->xfont_name);
		    }
		    font->font_info = 
			XLoadQueryFont( THE_DISPLAY, font->xfont_name);
		    if ( font->font_info == NULL) {
			fprintf( stderr, "X font %s not found\n",
				 font->xfont_name);
		    }
		    font_table->num_fonts++;
		}
	    }
	    break;
	case X_CloseFont:
	    break;
	case X_QueryFont:		/* needs reply */	
	    {
		xResourceReq	*req = ( xResourceReq *) buf;
		xQueryFontReply	*reply = ( xQueryFontReply *) newmsg;
		XFontStruct	*font_struct;
		int		found = FALSE;

		for ( i = 0; i < font_table->num_fonts; i++) {
		    if ( font_table->font_table[ i].fid == req->id) {
			found = TRUE;
		    }
		}

/* if the font is not loaded on the server, we have to load it before
   querying the font */
	
		if ( !found) {
		    fprintf( stderr, "font ID %d not found\n");
		    reply->length = 
			( sizeof( xQueryFontReply) - sizeof( xReply)) >> 2;
		    reply->nFontProps = 0;
		    reply->nCharInfos = 0;
		    *newlen = sizeof( *reply);
		} else {
		    if ( font_table->font_table[ i].server_fid == 0) {
			font_table->font_table[ i].server_fid = 
			    XLoadFont( THE_DISPLAY, 
				       font_table->font_table[ i].xfont_name);
		    }
		    font_struct = 
			XQueryFont( THE_DISPLAY, 
				    font_table->font_table[ i].server_fid); 
		    reply->minBounds = *( xCharInfo *) &font_struct->min_bounds;
		    reply->maxBounds = *( xCharInfo *) &font_struct->max_bounds;
		    reply->minCharOrByte2 = font_struct->min_char_or_byte2;
		    reply->maxCharOrByte2 = font_struct->max_char_or_byte2;
		    reply->defaultChar = font_struct->default_char;
		    reply->nFontProps = font_struct->n_properties;
		    reply->drawDirection = font_struct->direction;
		    reply->minByte1 = font_struct->min_byte1;
		    reply->maxByte1 = font_struct->max_byte1;
		    reply->allCharsExist = font_struct->all_chars_exist;
		    reply->fontAscent = font_struct->ascent;
		    reply->fontDescent = font_struct->descent;
		    reply->nCharInfos = font_struct->max_char_or_byte2 - 
					    font_struct->min_char_or_byte2;

/* copy the charinfo + fontprop structures */

		    memcpy(( void *) (req + sizeof( xQueryFontReply)),
			   ( void *) font_struct->per_char, 
			   sizeof( xCharInfo)*reply->nCharInfos);
		    memcpy(( void *) (req + sizeof( xQueryFontReply) +
			       sizeof( XCharStruct)*reply->nCharInfos),
			   ( void *) font_struct->properties,
			   sizeof( xFontProp)*reply->nFontProps);
		    *newlen = sizeof( *reply) + 
			      sizeof( xCharInfo)*reply->nCharInfos + 
			      sizeof( xFontProp)*reply->nFontProps;
		    reply->length = 7 + reply->nFontProps*2 +
				    reply->nCharInfos*3;
		}
	    }
	    break;
	case X_QueryTextExtents:	/* needs reply */
	    {
		xQueryTextExtentsReq	*req = 
		    ( xQueryTextExtentsReq *) buf;
		xQueryTextExtentsReply	*reply = 
		    ( xQueryTextExtentsReply *) newmsg;
		int			nchars, dir, ascent, descent,
					found = FALSE;
		XCharStruct		overall;

		for ( i = 0; i < font_table->num_fonts; i++) {
		    if ( font_table->font_table[ i].fid != req->fid) {
			found = TRUE;
		    }
		}
		if ( font_table->font_table[ i].server_fid == 0) {
		    font_table->font_table[ i].server_fid = 
			XLoadFont( THE_DISPLAY, 
				   font_table->font_table[ i].xfont_name);
		}
		nchars = (req->length >> 1) - req->oddLength;
		XQueryTextExtents16( THE_DISPLAY, 
				     font_table->font_table[ i].server_fid,
				     ( XChar2b *) buf + sizeof( *req),
				     nchars, &dir, &ascent, &descent, &overall);
		reply->fontAscent = ascent;
		reply->fontDescent = descent;
		reply->overallAscent = overall.ascent;
		reply->overallDescent = overall.descent;
		reply->overallWidth = overall.width;
		reply->overallLeft = overall.lbearing;
		reply->overallRight = overall.rbearing;
		*newlen = sizeof( *reply);
	    }
	    break;
	case X_ListFonts:		/* needs reply */
	    {
		xListFontsReq	*req = ( xListFontsReq *) buf;
		xListFontsReply	*reply = ( xListFontsReply *) newmsg;
		char		**fonts; 
		unsigned char	*ptr;
		int		i, length = 0, nfonts;

		fonts = XListFonts( THE_DISPLAY, 
				    ( char *) buf + sizeof( *req),
				    req->maxNames, &nfonts);
		reply->nFonts = nfonts;
		for ( i = 0, ptr = buf + sizeof( *req); 
		      i < nfonts;
		      i++, ptr += strlen( fonts[ i]) + 1) {
		    strcpy(( char *) ptr, fonts[ i]);
		    length += strlen( fonts[ i]) + 1;
		}
		reply->length = ( length + 3) >> 2;
		*newlen = sizeof( *reply) + reply->length << 2;
	    }
	    break;
	case X_ListFontsWithInfo:	/* needs reply */
	    {
		xListFontsWithInfoReq	*req = 
		    ( xListFontsWithInfoReq *) buf;
		xListFontsWithInfoReply	*reply = 
		    ( xListFontsWithInfoReply *) newmsg;
		char		**fonts, *ptr;
		int		i, nfonts;
		XFontStruct	*finfo_vec;

		*newlen = 0;
		fonts = XListFontsWithInfo( THE_DISPLAY, 
					    ( char *) buf + sizeof( *req),
					    req->maxNames, &nfonts,
					    &finfo_vec);

/* this generates a set of replies.  Right now, all I do is write then,
   but there's a high likelihood of overflow */
  
		for ( i = 0; i < nfonts; i++) {
		    reply->nameLength = strlen( fonts[ i]) + 1;
		    reply->minBounds = 
			*( xCharInfo *) &finfo_vec[ i].min_bounds;
		    reply->maxBounds = 
			*( xCharInfo *) &finfo_vec[ i].max_bounds;
		    reply->minCharOrByte2 = finfo_vec[ i].min_char_or_byte2;
		    reply->maxCharOrByte2 = finfo_vec[ i].max_char_or_byte2;
		    reply->defaultChar = finfo_vec[ i].default_char;
		    reply->nFontProps = finfo_vec[ i].n_properties;
		    reply->drawDirection = finfo_vec[ i].direction;
		    reply->minByte1 = finfo_vec[ i].min_byte1;
		    reply->maxByte1 = finfo_vec[ i].max_byte1;
		    reply->allCharsExist = finfo_vec[ i].all_chars_exist;
		    reply->fontAscent = finfo_vec[ i].ascent;
		    reply->fontDescent = finfo_vec[ i].descent;
		    reply->nReplies = nfonts - (i + 1);

/* copy the propertyu structures, but not char infos */

		    memcpy(( void *) (req + sizeof( xQueryFontReply)),
			   ( void *) finfo_vec[ i].properties,
			   sizeof( xFontProp)*reply->nFontProps);

/* copy the name of the font */

		    strcpy(( char *) req + sizeof( xQueryFontReply) +
			    sizeof( xFontProp)*reply->nFontProps, 
			    fonts[ i]);
		    *newlen += sizeof( *reply) + strlen( fonts[ i]) + 1 +
			       sizeof( xFontProp)*reply->nFontProps;
		    reply = ( xListFontsWithInfoReply *) (buf + pad( *newlen));
		}

/* nameLength = 0 implies end of sequence */

		reply->nameLength = 0;
		*newlen += sizeof( reply);
	    }
	    break;
	case X_SetFontPath:
	    break;
	case X_GetFontPath:		/* needs reply */
	    {
		xReq			*req = ( xReq *) buf;
		xGetFontPathReply	*reply = 
		    ( xGetFontPathReply *) newmsg;
		char			**paths; 
		unsigned char		*ptr;
		int			npaths, i, length = 0;
	
		paths = XGetFontPath( THE_DISPLAY, &npaths);
		reply->nPaths = npaths;
		for ( i = 0, ptr = buf + sizeof( *req); 
		      i < npaths; 
		      i++, ptr += strlen( paths[ i]) + 1) {
		    length += strlen( paths[ i]) + 1;
		    strcpy(( char *) ptr, paths[ i]);
		}
		reply->length = ( length + 3) >> 2;
		*newlen = sizeof( *reply) + length << 2;
	    }
	    break;
	case X_CreatePixmap:		/* FTM, these are ignored */
	    break;
	case X_FreePixmap:
	    {
		xResourceReq	*req = ( xResourceReq *) buf;

		delete_win( winfo_table, req->id);
	    }
	    break;
	case X_CreateGC:
	    {
		xCreateGCReq	*req = ( xCreateGCReq *) buf;

/* copy the standard GC, then use the flags to get the attributes from
   the message */

		index = gcinfo_table->num_gc++;
		gcinfo_table->gc_table[ index].attr = initial_GC;
		gcinfo_table->gc_table[ index].gcid = req->gc;
		gcinfo_table->gc_table[ index].clip_rects = NULL;
		gcinfo_table->gc_table[ index].num_clip_rects = 0;
		gcinfo_table->gc_table[ index].fd = fd;
		gcvp = &( gcinfo_table->gc_table[ index].attr);

/* pull out the value mask and use it to get the values from the request, 
   set a pointer into the values section of the request */
		
		winfo = getwinfo( req->drawable, winfo_table);
		if ( winfo != NULL) {
		    set_gcvalues( fd, print_fp, gcvp, req->mask, 
				  ( unsigned long *) 
				      (buf + sizeof( xCreateGCReq)), 
				  winfo->attr.colormap);
		} else {
		    fprintf( stderr, "X_CreateGC: bad window id %d\n", 
			     req->drawable);
		}
	    }
	    break;
	case X_ChangeGC:
	    {
		xChangeGCReq	*req = ( xChangeGCReq *) buf;

/* have to intercept these, because there's no other way to get the info 
   send the print window down for the colormap for setcolor */

		gcinfo = getgcinfo( req->gc, gcinfo_table);
		if ( gcinfo != NULL) {
		    winfo = getwinfo( PCS[ fd].win, winfo_table);
		    set_gcvalues( fd, print_fp, &gcinfo->attr, req->mask, 
				  ( unsigned long *)
				      ( buf + sizeof( xChangeGCReq)),
				  winfo->attr.colormap);
		} else {
		    fprintf( stderr, "X_ChangeGC bad GC %d\n", req->gc);
		}
	    }
	    break;
	case X_CopyGC:
	    { 
		xCopyGCReq	*req = ( xCopyGCReq *) buf;
		gcinfo_p	gcinfo2;

		gcinfo = getgcinfo( req->srcGC, gcinfo_table);
		gcinfo2 = getgcinfo( req->dstGC, gcinfo_table);
		if ( gcinfo && gcinfo2) {
		    copy_gcvalues( &gcinfo->attr, &gcinfo2->attr, req->mask);
		}
	    }
	    break;
	case X_SetDashes:
	    {
		xSetDashesReq	*req = ( xSetDashesReq *) buf;
		
		gcinfo = getgcinfo( req->gc, gcinfo_table);
		if ( gcinfo != NULL) {
		    gcvp = &( gcinfo->attr);
		    gcvp->dash_offset = req->dashOffset;
		    gcinfo->num_dashes = req->nDashes;
		    gcinfo->dashes = ( char *) malloc( req->nDashes);
		    memcpy( gcinfo->dashes, buf + sizeof( *req), 
			    gcinfo->num_dashes);
		} else {
		    fprintf( stderr, "X_SetDashes bad GC %d\n", req->gc);
		}
	    }
	    break;
	case X_SetClipRectangles:
	    {
		xSetClipRectanglesReq 	*req = 
			( xSetClipRectanglesReq *) buf;

		gcinfo = getgcinfo( req->gc, gcinfo_table);
		if ( gcinfo != NULL) {
		    gcvp = &( gcinfo->attr);

/* set the clip x and y origin */

		    gcvp->clip_x_origin = req->xOrigin;
		    gcvp->clip_y_origin = req->yOrigin;

/* then copy the clip rectangles */

		    if ( gcinfo->clip_rects != NULL) {
			free( gcinfo->clip_rects);
		    }
		    nrect = ( req->length - 3) >> 1;
		    gcinfo->clip_rects = 
			( XRectangle *) malloc( sizeof( XRectangle)*nrect);
		    memcpy( gcinfo->clip_rects, 
			    buf + sizeof( xSetClipRectanglesReq),
			    nrect*sizeof( XRectangle));
		    gcinfo->num_clip_rects = nrect;
		} else {
		    fprintf( stderr, "X_SetClipRectangles bad GC %d\n", 
			     req->gc);
		}
	    }
	    break;
	case X_FreeGC:
	    {
		xResourceReq	*req = ( xResourceReq *) buf;

		delete_gc( gcinfo_table, req->id);
		break;
	    }
	    break;
	case X_ClearArea:
	    {
		xClearAreaReq	*req = ( xClearAreaReq *) buf;

		winfo = getwinfo( req->window, winfo_table);
		if ( winfo != NULL) {
		    cleararea( print_fp, req->window, &( winfo->attr), 
			       req->x, req->y, req->width, req->height);
		} else {
		    fprintf( stderr, "X_ClearArea: bad window ID %d\n", 
			     req->window);
		}
	    }
	    break;
	case X_CopyArea:
	    {
		xCopyAreaReq	*req = ( xCopyAreaReq *) buf;

		winfo = getwinfo( req->dstDrawable, winfo_table);
		if ( winfo != NULL) {
		    cmapinfo = getcmapinfo( winfo->attr.colormap, 
					    cmapinfo_table);
		    copyarea( print_fp, req->srcDrawable, req->dstDrawable,
			      req->srcX, req->srcY, req->dstX, req->dstY,
			      req->width, req->height,
			      NUM_COLORS, cmapinfo->colors);
		} else {
		    fprintf( stderr, "X_CopyArea: bad window ID %d\n", 
			     req->dstDrawable);
		}
	    }
	    break;
	case X_CopyPlane:
	    break;
	case X_PolyPoint:
	    {
		xPolyPointReq	*req = ( xPolyPointReq *) buf;

		flag = req->coordMode;
		gcinfo = getgcinfo( req->gc, gcinfo_table);
		if ( gcinfo == NULL) {
		    fprintf( stderr, "X_Polyline bad GC %d\n", req->gc);
		    gcvp = &initial_GC;
		} else {
		    gcvp = &gcinfo->attr;
		}
		winfo = getwinfo( req->drawable, winfo_table);
		if ( winfo != NULL) {
		    cmapinfo = getcmapinfo( winfo->attr.colormap, 
					    cmapinfo_table);
		    set_curr_gc( fd, print_fp, gcvp, cmapinfo->cmap);
		    polypoint( print_fp, req->drawable, gcvp, 
			       req->length - 3, ( int) req->coordMode,
			       ( XPoint *) (buf + sizeof( xPolyPointReq)));
		} else {
		    fprintf( stderr, "X_PolyPoint: bad window ID %d\n",
			     req->drawable);
		}
	    }
	    break;
	case X_PolyLine:
	    {
		xPolyLineReq	*req = ( xPolyLineReq *) buf;

		flag = req->coordMode;
		gcinfo = getgcinfo( req->gc, gcinfo_table);
		if ( gcinfo == NULL) {
		    fprintf( stderr, "X_Polyline bad GC %d\n", req->gc);
		    gcvp = &initial_GC;
		} else {
		    gcvp = &gcinfo->attr;
		}
		winfo = getwinfo( req->drawable, winfo_table);
		if ( winfo != NULL) {
		    cmapinfo = getcmapinfo( winfo->attr.colormap, 
					    cmapinfo_table);
		    set_curr_gc( fd, print_fp, gcvp, cmapinfo->cmap);
		    polyline( print_fp, req->drawable, gcvp, 
			      req->length - 3, ( int) req->coordMode,
			      ( XPoint *) (buf + sizeof( xPolyLineReq)));
		} else {
		    fprintf( stderr, "X_PolyLine: bad window ID %d\n",
			     req->drawable);
		}
	    }
	    break;
	case X_PolySegment:
	    {
		xPolySegmentReq	*req = ( xPolySegmentReq *) buf;

		gcinfo = getgcinfo( req->gc, gcinfo_table);
		if ( gcinfo == NULL) {
		    fprintf( stderr, "X_PolySegment bad GC %d\n", req->gc);
		    gcvp = &initial_GC;
		} else {
		    gcvp = &gcinfo->attr;
		}
		winfo = getwinfo( req->drawable, winfo_table);
		if ( winfo != NULL) {
		    cmapinfo = getcmapinfo( winfo->attr.colormap, 
					    cmapinfo_table);
		    set_curr_gc( fd, print_fp, gcvp, cmapinfo->cmap);
		    polysegment( print_fp, req->drawable, gcvp, 
				 ( req->length - 3) >> 1,
				 ( XSegment *) 
				 	(buf + sizeof( xPolySegmentReq)));
		} else {
		    fprintf( stderr, "X_PolySegment: bad window ID %d\n",
			     req->drawable);
		}
	    }
	    break;
	case X_PolyRectangle:
	    { 
		xPolyRectangleReq	*req = ( xPolyRectangleReq *) buf;

		gcinfo = getgcinfo( req->gc, gcinfo_table);
		if ( gcinfo == NULL) {
		    fprintf( stderr, "X_PolyRectangle bad GC %d\n", req->gc);
		    gcvp = &initial_GC;
		} else {
		    gcvp = &gcinfo->attr;
		}
		winfo = getwinfo( req->drawable, winfo_table);
		if ( winfo != NULL) {
		    cmapinfo = getcmapinfo( winfo->attr.colormap, 
					    cmapinfo_table);
		    set_curr_gc( fd, print_fp, gcvp, cmapinfo->cmap);
		    polyrectangle( print_fp, req->drawable, gcvp, 
				   ( req->length - 3) >> 1,
				   ( XRectangle *) 
				       ( buf + sizeof( xPolyRectangleReq)));
		} else {
		    fprintf( stderr, "X_PolyRectangle: bad window ID %d\n",
			     req->drawable);
		}
	    }
	    break;
	case X_PolyArc:
	    {
		xPolyArcReq 	*req = ( xPolyArcReq *) buf;

		gcinfo = getgcinfo( req->gc, gcinfo_table);
		if ( gcinfo == NULL) {
		    fprintf( stderr, "X_Polyline bad GC %d\n", req->gc);
		    gcvp = &initial_GC;
		} else {
		    gcvp = &gcinfo->attr;
		}
		winfo = getwinfo( req->drawable, winfo_table);
		if ( winfo != NULL) {
		    cmapinfo = getcmapinfo( winfo->attr.colormap, 
					    cmapinfo_table);
		    set_curr_gc( fd, print_fp, gcvp, cmapinfo->cmap);
		    polyarc( print_fp, req->drawable, gcvp, (req->length - 3)/3,
			     ( xArc *) buf + sizeof( xPolyArcReq));
		} else {
		    fprintf( stderr, "X_PolyRectangle: bad window ID %d\n",
			     req->drawable);
		}
	    }
	    break;
	case X_FillPoly:
	    {
		xFillPolyReq	*req = ( xFillPolyReq *) buf;

/* flag for coordinate mode */

		gcinfo = getgcinfo( req->gc, gcinfo_table);
		if ( gcinfo == NULL) {
		    fprintf( stderr, "X_FillPoly bad GC %d\n", req->gc);
		    gcvp = &initial_GC;
		} else {
		    gcvp = &gcinfo->attr;
		}
		winfo = getwinfo( req->drawable, winfo_table);
		if ( winfo != NULL) {
		    cmapinfo = getcmapinfo( winfo->attr.colormap, 
					    cmapinfo_table);
		    set_curr_gc( fd, print_fp, gcvp, cmapinfo->cmap);
		    fillpoly( print_fp, req->drawable, &( gcinfo->attr), 
			      req->length - 4, req->coordMode,
			      ( XPoint *) (buf + sizeof( xFillPolyReq)));
		} else {
		    fprintf( stderr, "X_PolyRectangle: bad window ID %d\n",
			     req->drawable);
		}
	    }
	    break;
	case X_PolyFillRectangle:
	    {
		xPolyRectangleReq	*req = ( xPolyRectangleReq *) buf;

		gcinfo = getgcinfo( req->gc, gcinfo_table);
		if ( gcinfo == NULL) {
		    fprintf( stderr, "X_PolyFillRectangle bad GC %d\n", 
			    req->gc);
		    gcvp = &initial_GC;
		} else {
		    gcvp = &gcinfo->attr;
		}
		winfo = getwinfo( req->drawable, winfo_table);
		if ( winfo != NULL) {
		    cmapinfo = getcmapinfo( winfo->attr.colormap, 
					    cmapinfo_table);
		    set_curr_gc( fd, print_fp, gcvp, cmapinfo->cmap);
		    polyfillrectangle( print_fp, req->drawable, gcvp, 
				       (req->length - 3) >> 1,
				       ( XRectangle *) 
					   ( buf + sizeof( xPolyRectangleReq)));
		} else {
		    fprintf( stderr, "X_PolyFillRectangle: bad window ID %d\n",
			     req->drawable);
		}
	    }
	    break;
	case X_PolyFillArc:
	    break;
	case X_PutImage:
	    { 
		xPutImageReq 	*req = ( xPutImageReq *) buf;

		gcinfo = getgcinfo( req->gc, gcinfo_table);
		if ( gcinfo == NULL) {
		    fprintf( stderr, "X_PutImage bad GC %d\n", req->gc);
		    gcvp = &initial_GC;
		} else {
		    gcvp = &gcinfo->attr;
		}

/* get the window's colormap so we know what colors to use for the image */

		winfo = getwinfo( req->drawable, winfo_table);
		if ( winfo != NULL) {
		    cmapinfo = getcmapinfo( winfo->attr.colormap, 
					    cmapinfo_table);
		    set_curr_gc( fd, print_fp, gcvp, cmapinfo->cmap);
		    putimage( print_fp, req->drawable, gcvp,
			      req->depth, req->width, req->height, 
			      req->dstX, req->dstY, req->pad, req->format, 
			      NUM_COLORS, cmapinfo->colors,
			      buf + sizeof( xPutImageReq));
		} else {
		    fprintf( stderr, "X_PutImage: bad window ID %d\n",
			     req->drawable);
		}
	    }
	    break;

/* GetImage is totally kludged,  It sends back an entirely blank image */

	case X_GetImage:		/* needs reply */
	    {
		xGetImageReq	*req = ( xGetImageReq *) buf;
		xGetImageReply	*reply = ( xGetImageReply *) newmsg;

		reply->depth = 8;
		reply->length = req->width*req->height*2;
		*newlen = sizeof( *reply) + reply->length*2;
	    }
	    break;
	case X_PolyText8:
	    {
		xPolyText8Req	*req = ( xPolyText8Req *) buf;

		gcinfo = getgcinfo( req->gc, gcinfo_table);
		if ( gcinfo == NULL) {
		    fprintf( stderr, "X_PolyText8 bad GC %d\n", req->gc);
		    gcvp = &initial_GC;
		} else {
		    gcvp = &gcinfo->attr;
		}
		winfo = getwinfo( req->drawable, winfo_table);
		if ( winfo != NULL) {
		    cmapinfo = getcmapinfo( winfo->attr.colormap, 
					    cmapinfo_table);
		    set_curr_gc( fd, print_fp, gcvp, cmapinfo->cmap);
		    polytext8( print_fp, req->drawable, gcvp, req->x, req->y, 
			       ( req->length*4) - sizeof( xPolyText8Req), 
			       buf + sizeof( xPolyText8Req));
		} else {
		    fprintf( stderr, "X_PolyText8: bad window ID %d\n",
			     req->drawable);
		}
	    }
	    break;
	case X_PolyText16:
	    { 
		xPolyText16Req	*req = ( xPolyText16Req *) buf;

		gcinfo = getgcinfo( req->gc, gcinfo_table);
		if ( gcinfo == NULL) {
		    fprintf( stderr, "X_PolyText16 bad GC %d\n", req->gc);
		    gcvp = &initial_GC;
		} else {
		    gcvp = &gcinfo->attr;
		}
		winfo = getwinfo( req->drawable, winfo_table);
		if ( winfo != NULL) {
		    cmapinfo = getcmapinfo( winfo->attr.colormap, 
					    cmapinfo_table);
		    set_curr_gc( fd, print_fp, gcvp, cmapinfo->cmap);
		    polytext16( print_fp, req->drawable, gcvp, req->x, req->y, 
				(req->length - 4) >> 2,
				buf + sizeof( xPolyText16Req));
		} else {
		    fprintf( stderr, "X_PolyText8: bad window ID %d\n",
			     req->drawable);
		}
	    }
	    break;
	case X_ImageText8:
	    {
		xImageText8Req	*req = ( xImageText8Req *) buf;

		gcinfo = getgcinfo( req->gc, gcinfo_table);
		if ( gcinfo == NULL) {
		    fprintf( stderr, "X_ImageText8 bad GC %d\n", req->gc);
		    gcvp = &initial_GC;
		} else {
		    gcvp = &gcinfo->attr;
		}
		winfo = getwinfo( req->drawable, winfo_table);
		if ( winfo != NULL) {
		    cmapinfo = getcmapinfo( winfo->attr.colormap, 
					    cmapinfo_table);
		    set_curr_gc( fd, print_fp, gcvp, cmapinfo->cmap);
		    imagetext8( print_fp, req->drawable, gcvp, req->x, req->y, 
				( req->length*4) - sizeof( xPolyText8Req), 
				buf + sizeof( xImageText8Req));
		} else {
		    fprintf( stderr, "X_ImageText8: bad window ID %d\n",
			     req->drawable);
		}
	    }
	    break;
	case X_ImageText16:
	    {
		xImageText16Req	*req = ( xImageText16Req *) buf;

		gcinfo = getgcinfo( req->gc, gcinfo_table);
		if ( gcinfo == NULL) {
		    fprintf( stderr, "X_ImageText16 bad GC %d\n", req->gc);
		    gcvp = &initial_GC;
		} else {
		    gcvp = &gcinfo->attr;
		}
		winfo = getwinfo( req->drawable, winfo_table);
		if ( winfo != NULL) {
		    cmapinfo = getcmapinfo( winfo->attr.colormap, 
					    cmapinfo_table);
		    set_curr_gc( fd, print_fp, gcvp, cmapinfo->cmap);
		    imagetext16( print_fp, req->drawable, gcvp, req->x, req->y, 
				 ( req->length - 4) >> 2,
				 buf + sizeof( xImageText16Req));
		} else {
		    fprintf( stderr, "X_ImageText16: bad window ID %d\n",
			     req->drawable);
		}
	    }
	    break;
	case X_CreateColormap:
	    {
		xCreateColormapReq *req = ( xCreateColormapReq *) buf;
		int		   i, n = cmapinfo_table->num_cmap++;

		cmapinfo_table->cmap_table[ n].num_colors = 0;
		cmapinfo_table->cmap_table[ n].cmap = req->mid;
		cmapinfo_table->cmap_table[ n].fd = fd;
		for ( i = 0; i < NUM_COLORS; i++) {
		    cmapinfo_table->cmap_table[ n].colors[ i].pixel = i;
		    cmapinfo_table->cmap_table[ n].num_alloc[ i] = 0;
		}
	    }
	    break;
	case X_FreeColormap:
	    {
		xResourceReq	*req = ( xResourceReq *) buf;

		delete_cmap( cmapinfo_table, req->id);
	    }
	    break;
	case X_CopyColormapAndFree:
	    {

/* create a new colormap and copy the old colormap to it */

		xCopyColormapAndFreeReq *req = ( xCopyColormapAndFreeReq *) buf;
		int			i, n = cmapinfo_table->num_cmap;

		cmapinfo = getcmapinfo( req->srcCmap, cmapinfo_table);
		cmapinfo_table->cmap_table[ n].num_colors = 
		    cmapinfo->num_colors;
		for ( i = 0; i < NUM_COLORS; i++) {
		    cmapinfo_table->cmap_table[ n].colors[ i] = 
			cmapinfo->colors[ i];
		    cmapinfo_table->cmap_table[ n].num_alloc[ i] = 
			cmapinfo->num_alloc[ i];
		}
		cmapinfo_table->num_cmap++;
	    }
	    break;
	case X_InstallColormap:
	    break;
	case X_UninstallColormap:
	    break;
	case X_ListInstalledColormaps:		/* needs reply */
	    break;
	case X_AllocColor:			/* needs reply */
	    {
		xAllocColorReq		*req = ( xAllocColorReq *) buf;
		xAllocColorReply	*reply = ( xAllocColorReply *) newmsg;
		int			found;
		
		cmapinfo = getcmapinfo( req->cmap, cmapinfo_table);
		found = FALSE;
		for ( i = 0; i < NUM_COLORS; i++) {
		    if (( cmapinfo->colors[ i].red == req->red) &&
			( cmapinfo->colors[ i].green == req->green) &&
			( cmapinfo->colors[ i].blue == req->blue) &&
			( cmapinfo->num_alloc[ i] > 0))  {
			cmapinfo->num_alloc[ i]++;
			found = TRUE;
			break;
		    }
		}

/* allocate a new pixel, then return it (whether allocated or not) */ 

		if ( !found) {
		    for ( i = 0; i < NUM_COLORS; i++) {
			if ( cmapinfo->num_alloc[ i] == 0) {
			    break;
			}
		    }
		    if ( i < NUM_COLORS) {
			cmapinfo->colors[ i].red = req->red;
			cmapinfo->colors[ i].green = req->green;
			cmapinfo->colors[ i].blue = req->blue;
			cmapinfo->num_alloc[ i]++;
			cmapinfo->num_colors++;
		    }
		}
		reply->pixel = i;
		reply->red = req->red;
		reply->green = req->green;
		reply->blue = req->blue;

/* increment the number of people allocated to this color */

		*newlen = sizeof( *reply);
	    }
	    break;
	case X_AllocNamedColor:			/* needs reply */
	    {
		xAllocNamedColorReq	*req = ( xAllocNamedColorReq *) buf;
		xAllocNamedColorReply	*reply = 
		    ( xAllocNamedColorReply *) newmsg;
		char 			name[ 256];
		XColor			exact;
		int			found;

/* use the real server to get the mapping from the name to RGB values */

		strncpy( name, ( char *) buf + sizeof( *req), req->nbytes);
		XParseColor( THE_DISPLAY, 
		    DefaultColormap( THE_DISPLAY, DefaultScreen( THE_DISPLAY)),
		    name, &exact);
		cmapinfo = getcmapinfo( req->cmap, cmapinfo_table);
		found = FALSE;
		for ( i = 0; i < NUM_COLORS; i++) {
		    if (( cmapinfo->colors[ i].red == exact.red) &&
			( cmapinfo->colors[ i].green == exact.green) &&
			( cmapinfo->colors[ i].blue == exact.blue)) {
			cmapinfo->num_alloc[ i]++;
			found = TRUE;
			break;
		    }
		}

/* need to allocate color in vector here */

		if ( !found) {
		    for ( i = 0; i < NUM_COLORS; i++) {
			if ( cmapinfo->num_alloc[ i] == 0) {
			    break;
			}
		    }
		    if ( i < NUM_COLORS) {
			cmapinfo->colors[ i] = exact;
			cmapinfo->num_alloc[ i]++;
			cmapinfo->num_colors++;
		    }
		}
		reply->pixel = i;
		reply->exactRed = exact.red;
		reply->exactGreen = exact.green;
		reply->exactBlue = exact.blue;
		reply->screenRed = exact.red;
		reply->screenGreen = exact.green;
		reply->screenBlue = exact.blue;

/* increment the number of people allocated to this color */

		*newlen = sizeof( *reply);
	    }
	    break;

/* both of these are just hacked so that I've got a reply */

	case X_AllocColorCells:			/* needs reply */
	    {
		xAllocColorCellsReq	*req = ( xAllocColorCellsReq *) buf;
		xAllocColorCellsReply 	*reply = 
		    ( xAllocColorCellsReply *) newmsg;
		unsigned long		*pixel;
		
		cmapinfo = getcmapinfo( req->cmap, cmapinfo_table);
		reply->nPixels = req->colors;
		reply->nMasks = req->planes;
		pixel = ( unsigned long *) buf + sizeof( *req);
		for ( i = 0; i < reply->nPixels; i++) {
		    *pixel++ = i;
		}
		for ( i = 0; i < reply->nMasks; i++) {
		    *pixel++ = ( 1 << i);
		}
		reply->length = reply->nPixels + reply->nMasks;
		*newlen = sizeof( *reply) + ( reply->nPixels << 2) +
			  ( reply->nMasks << 2);
	    }
	    break;
	case X_AllocColorPlanes:		/* needs reply */
	    {
		xAllocColorPlanesReq	*req = ( xAllocColorPlanesReq *) buf;
		xAllocColorPlanesReply	*reply = 
		    ( xAllocColorPlanesReply *) newmsg;
		unsigned long		*pixel;

		cmapinfo = getcmapinfo( req->cmap, cmapinfo_table);
		reply->nPixels = req->colors;
		pixel = ( unsigned long *) (buf + sizeof( *req));
		for ( i = 0; i < reply->nPixels; i++) {
		    *pixel++ = i;
		}
		reply->redMask = 0;
		reply->greenMask = 0;
		reply->blueMask = 0;
		reply->length = reply->nPixels;
		*newlen = sizeof( *reply) + ( reply->nPixels << 2);
	    }
	    break;
	case X_FreeColors:
	    {
		xFreeColorsReq	*req = ( xFreeColorsReq *) buf;
		unsigned long	*pixel;

		pixel = ( unsigned long *) (buf + sizeof( *req));
		cmapinfo = getcmapinfo( req->cmap, cmapinfo_table);
		for ( i = 0; i < req->length - 3; i++) {
		    if (( *pixel >= 0) && ( *pixel < NUM_COLORS)) {
			cmapinfo->num_alloc[ *pixel]--;
		    } else {  
			fprintf( stderr, "pixel out of range %d\n", *pixel);
		    }
		}
	    }
	    break;
	case X_StoreColors:
	    {
		xStoreColorsReq	*req = ( xStoreColorsReq *) buf;
		int		ncolors = (req->length - 2)/3, index;
		xColorItem	*color = ( xColorItem *) ( buf + sizeof( *req));

		cmapinfo = getcmapinfo( req->cmap, cmapinfo_table);
		for ( i = 0; i < ncolors; i++) {
		    index = color->pixel;
		    if (( index >= 0) && ( index < NUM_COLORS)) {
			if ( cmapinfo->writeable[ i]) {
			    cmapinfo->colors[ index].red = color->red;
			    cmapinfo->colors[ index].green = color->green;
			    cmapinfo->colors[ index].blue = color->blue;
			    cmapinfo->colors[ index].pixel = index;
			}
		    } else {
			fprintf( stderr, "pixel out of range %d\n", index);
		    }
		    color++;
		}
	    }
	    break;
	case X_StoreNamedColor:
	    {
		xStoreNamedColorReq	*req = ( xStoreNamedColorReq *) buf;
		XColor			exact, screen;
		int			index = req->pixel;
		char			colorname[ 256];

		strncpy( colorname, ( char *) buf + sizeof( *req), req->nbytes);
		XLookupColor( THE_DISPLAY,
		    DefaultColormap( THE_DISPLAY, DefaultScreen( THE_DISPLAY)),
		    colorname, &exact, &screen);
		cmapinfo = getcmapinfo( req->cmap, cmapinfo_table);
		if (( index >= 0) && ( index < NUM_COLORS)) {
		    if ( cmapinfo->writeable[ i]) {
			cmapinfo->colors[ index].red = exact.red;
			cmapinfo->colors[ index].green = exact.green;
			cmapinfo->colors[ index].blue = exact.blue;
			cmapinfo->colors[ index].pixel = index;
		    } else {
			fprintf( stderr, "pixel out of range %d\n", index);
		    }
		}
	    }
	    break;
	case X_QueryColors:			/* needs reply */
	    {
		xQueryColorsReq	*req = ( xQueryColorsReq *) buf;
		xQueryColorsReply *reply = ( xQueryColorsReply *) newmsg;

		reply->length = 2*(req->length - 2);
	    }
	    break;
	case X_LookupColor:			/* needs reply */
	    {
		xLookupColorReq		*req = ( xLookupColorReq *) buf;
		xLookupColorReply 	*reply = ( xLookupColorReply *) newmsg;
		char 		*colorname = ( char *) buf + sizeof( *req);
		XColor			exact, screen;

		XLookupColor( THE_DISPLAY,
		    DefaultColormap( THE_DISPLAY, DefaultScreen( THE_DISPLAY)),
		    colorname, &exact, &screen);
		reply->exactRed = exact.red;
		reply->exactGreen = exact.green;
		reply->exactBlue = exact.blue;
		reply->screenRed = screen.red;
		reply->screenGreen = screen.green;
		reply->screenBlue = screen.blue;
		*newlen = sizeof( *reply);
	    }
	    break;
	case X_CreateCursor:
	    break;
	case X_CreateGlyphCursor:
	    break;
	case X_FreeCursor:
	    break;
	case X_RecolorCursor:
	    break;
	case X_QueryBestSize:			/* needs reply */
	    {
		xQueryBestSizeReply	*reply = 
		    ( xQueryBestSizeReply *) newmsg;
		
		reply->width = 16;		/* guesses.. */
		reply->height = 16;
		*newlen = sizeof( *reply);
	    }
	    break;
	case X_QueryExtension:

/* delete the extension request from the client and insert our reply that
   the extension exists */
	    {
		xQueryExtensionReq    *req= ( xQueryExtensionReq *) buf;
		xQueryExtensionReply  *reply = 
		    ( xQueryExtensionReply *) newmsg;
		char		      *extname, **extlist;

		extname = ( char *) buf + sizeof( xQueryExtensionReq);
		if ( strncmp( extname, PRINTNAME, strlen( PRINTNAME)) == 0) {

/* set up our fake reply */

		    reply->length = 0;
		    reply->present = True;

/* find out the number of extensions supported by the server, and use that
   for basing the major opcode # on */

		    extlist = XListExtensions( THE_DISPLAY, &nextensions);
		    reply->major_opcode = EXTENSION_BASE + nextensions + 1;
		    THE_OPCODE = EXTENSION_BASE + nextensions + 1;
		    reply->first_event = 0;
		    reply->first_error = 0;
		} else {
		    reply->length = 0;
		    reply->present = False;
		}
		*newlen = sizeof( *reply);
	    }
	    break;
	case X_ListExtensions:			/* needs reply */
	    {
		xListExtensionsReply	*reply = 
		    ( xListExtensionsReply *) newmsg;
		
		reply->nExtensions = 1;		/* just this extension */
		strcpy(( char *) newmsg + sizeof( reply), "Printer");
		reply->length = ( strlen( "Printer") + 3) >> 2;
		*newlen = sizeof( *reply) + ( reply->length << 2);
	    }
	    break;
	case X_ChangeKeyboardMapping:	
	    break;
	case X_GetKeyboardMapping:			/* needs reply */
	    {
		xGetKeyboardMappingReq	*req = 
		     ( xGetKeyboardMappingReq *) buf;
		xGetKeyboardMappingReply *reply = 
		     ( xGetKeyboardMappingReply *) newmsg;

		reply->keySymsPerKeyCode = 1;
		reply->length = req->count;
		*newlen = sizeof( *reply) + (reply->length << 2);
	    }
	    break;
	case X_ChangeKeyboardControl:
	    break;
	case X_GetKeyboardControl:			/* needs reply */
	    {
		xGetKeyboardControlReply *reply = 
		     ( xGetKeyboardControlReply *) newmsg;
	
/* fake it totally, don't even bother with the masks */ 

		reply->globalAutoRepeat = False;
		reply->ledMask = 0;
		reply->keyClickPercent = 0;
		reply->bellPercent = 0;
		reply->bellPitch = 0;
		reply->bellDuration = 0;
		reply->length = 5;
		*newlen = sizeof( *reply) + (reply->length << 2);
	    }
	    break;
	case X_Bell:
	    break;
	case X_ChangePointerControl:
	    break;
	case X_GetPointerControl:			/* needs reply */
	    {
		xGetPointerControlReply	*reply =
		    ( xGetPointerControlReply *) newmsg;
		
		reply->accelNumerator = 1;
		reply->accelDenominator = 1;
		*newlen = sizeof( *reply);
	    }
	    break;
	case X_SetScreenSaver:
	    break;
	case X_GetScreenSaver:			/* needs reply */
	    {
		xGetScreenSaverReply	*reply =
		    ( xGetScreenSaverReply *) newmsg;
		
		reply->timeout = 10;
		reply->interval = 10;
		reply->preferBlanking = FALSE;
		reply->allowExposures = FALSE;
		*newlen = sizeof( *reply);
	    }
	    break;
	case X_ChangeHosts:
	    break;
	case X_ListHosts:	/* no other hosts */	
	    {
		xListHostsReq	*req = ( xListHostsReq *) buf;
		xListHostsReply	*reply = ( xListHostsReply *) newmsg;

		reply->nHosts = 0;
		*newlen = sizeof( *reply);
	    }
	    break;
	case X_SetAccessControl:
	    break;
	case X_SetCloseDownMode:
	    break;
	case X_KillClient:
	    break;
	case X_RotateProperties:
	    break;
	case X_ForceScreenSaver:
	    break;
	case X_SetPointerMapping:
	    break;
	case X_GetPointerMapping: /* generate a totally bogus response */
	    {
		xGetPointerMappingReply	*reply = 
		    ( xGetPointerMappingReply *) newmsg;
		
		reply->nElts = 0;
		*newlen = sizeof( *reply);
	    }
	    break;
	case X_SetModifierMapping:
	    break;
	case X_GetModifierMapping:			/* needs reply */
	    {
		xGetModifierMappingReply *reply =
		    ( xGetModifierMappingReply *) newmsg;
		reply->numKeyPerModifier  = 0;
		*newlen = sizeof( *reply);
	    }
	    break;
	case X_NoOperation:
	    break;
	default:
	    fprintf( stderr, "Unimplemented request opcode");
	    break;
	}
    }
    return( True);		/* maybe this will be useful in the future */
}

/* round up to next multiple of 4 */

long pad( long n)
{
    return((n + 3) & ~0x3);
}

long pad2( long n)
{
    return(( n + 1) & ~0x1);
}

