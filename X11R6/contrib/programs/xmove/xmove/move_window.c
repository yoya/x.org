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

#define NEED_REPLIES
#include <X11/Xproto.h>
#include <X11/Xatom.h>
#include <malloc.h>

#include "xmove.h"
#include "xmond.h"

Global Window
move_window(WindowPtr window_ptr, unsigned char new_depth)
{
     Window xwin, parent_window;
     short GeomX, GeomY;

     Bool ForceWindowPosition = False;
     unsigned long new_winattr_mask;
     Window cur_window = MapWindowID(window_ptr->window_id, Request);
     XSetWindowAttributes new_attribs;

     xResourceReq XGetReq;
     xGetWindowAttributesReply XGetARep;
     xGetGeometryReply XGetGRep;
     xTranslateCoordsReq XTransReq;
     xTranslateCoordsReply XTransRep;
     
     parent_window = MapWindowID(window_ptr->parent_id, Client2NewServer);
     
     XGetReq.reqType = X_GetWindowAttributes;
     ISetShort((unsigned char *)&XGetReq.length, 2);
     ISetLong((unsigned char *)&XGetReq.id, cur_window);

     SendBuffer(cur_fd,
		(unsigned char *) &XGetReq,
		sizeof(xResourceReq));

     ReceiveReply(cur_fd, (unsigned char *) &XGetARep,
		  sizeof(xGetWindowAttributesReply), ++(*cur_seqno));


     XGetReq.reqType = X_GetGeometry;

     SendBuffer(cur_fd,
		(unsigned char *) &XGetReq,
		sizeof(xResourceReq));


     ReceiveReply(cur_fd, (unsigned char *) &XGetGRep,
		  sizeof(xGetGeometryReply), ++(*cur_seqno));

     GeomX = IShort((unsigned char *)&XGetGRep.x);
     GeomY = IShort((unsigned char *)&XGetGRep.y);

     /* The code below which has been ifdef'd out was used to determine
	where a window really is on the screen. GetGeometry tells us where
	the window is relative to its parent, which is a window manager shell
	typically. However, since we currently unmap all windows at the beginning
	of movement, it appears, with olwm at least, that the window is reparented
	back to root. Hence the below is unnecessary. */

     /* if this window was parented to the root window, attempt to
	place it in the same location once moved by using
	TranslateCoordinates to find where location (0,0) on our
	window lies relative to the root window */

     if (window_ptr->parent_id == client->root_window) {
#if 0
	  short TransX, TransY;
	  
	  XTransReq.reqType = X_TranslateCoords;
	  ISetShort((unsigned char *)&XTransReq.length, 4);
	  ISetLong((unsigned char *)&XTransReq.srcWid, cur_window);
	  ISetLong((unsigned char *)&XTransReq.dstWid, server->root_window);
	  XTransReq.srcX    = 0;
	  XTransReq.srcY    = 0;
	  
	  SendBuffer(cur_fd,
		     (unsigned char *) &XTransReq,
		     sizeof(xTranslateCoordsReq));
	  
	  ReceiveReply(cur_fd, (unsigned char *) &XTransRep,
		       sizeof(xTranslateCoordsReply), ++(*cur_seqno));

	  TransX = IShort((unsigned char *)&XTransRep.dstX);
	  TransY = IShort((unsigned char *)&XTransRep.dstY);

	  /* place results in XGetGRep -- easy reference when window is made */
	  /* account for window managers -- create the new window where its
	     frame's upper left corner is. */
	  
	  if (GeomX != TransX || GeomY != TransY) {
	       GeomX = TransX - GeomX;
	       GeomY = TransY - GeomY;
	       printf("putting window at (%d,%d) (x,y)\n",GeomX,GeomY);
	  }
#endif <ifdef 0>

	  ForceWindowPosition = True; /* when setting properties */
     }
     
     /* map all of the mappable attributes */
     
     memcpy(&new_attribs, &window_ptr->attributes, sizeof(XSetWindowAttributes));

     if (window_ptr->attributes_mask & CWBackPixel) {
	  new_attribs.background_pixel =
	       MapColorCell(new_attribs.background_pixel, window_ptr->cmap, Client2NewServer);
     }

     if (window_ptr->attributes_mask & CWBorderPixel) {
	  new_attribs.border_pixel =
	       MapColorCell(new_attribs.border_pixel, window_ptr->cmap, Client2NewServer);
     }

     if (window_ptr->attributes_mask & CWBackingPixel) {
	  new_attribs.backing_pixel =
	       MapColorCell(new_attribs.backing_pixel, window_ptr->cmap, Client2NewServer);
     }

     /* If the background pixmap isn't None or ParentRelative then map */
     if ((window_ptr->attributes_mask & CWBackPixmap) &&
	 (new_attribs.background_pixmap > 1)) {
	  new_attribs.background_pixmap =  
	       MapPixmapID(new_attribs.background_pixmap, Client2NewServer);
     }
     
     /* If the border pixmap isn't CopyFromParent the map */
     if ((window_ptr->attributes_mask & CWBorderPixmap) &&
	 new_attribs.border_pixmap) {
	  new_attribs.border_pixmap =  
	       MapPixmapID(new_attribs.border_pixmap, Client2NewServer);
     }
     
     /* Check this.  Change all colormaps to root colormap */
     if ((window_ptr->attributes_mask & CWColormap) &&
	 (new_attribs.colormap != CopyFromParent)) {
	  new_attribs.colormap =
	       MapColormapID(new_attribs.colormap, Client2NewServer);
     }

     new_attribs.bit_gravity           = XGetARep.bitGravity;
     new_attribs.win_gravity           = XGetARep.winGravity;
     new_attribs.backing_store         = XGetARep.backingStore;
     new_attribs.backing_planes        = ILong((unsigned char *)&XGetARep.backingBitPlanes);
     new_attribs.backing_pixel         = ILong((unsigned char *)&XGetARep.backingPixel);
     new_attribs.save_under            = XGetARep.saveUnder;
     new_attribs.event_mask            = ILong((unsigned char *)&XGetARep.yourEventMask);
     new_attribs.do_not_propagate_mask = IShort((unsigned char *)&XGetARep.doNotPropagateMask);
     new_attribs.override_redirect     = XGetARep.override;

     new_winattr_mask = (IShort((unsigned char *)&XGetARep.class) == InputOnly)
	  ?
	  window_ptr->attributes_mask | (CWWinGravity | CWOverrideRedirect |
					 CWEventMask  | CWDontPropagate)
	  :
	  window_ptr->attributes_mask | (CWBitGravity    | CWWinGravity       |
					 CWBackingStore  | CWBackingPlanes    |
					 CWBackingPixel  | CWOverrideRedirect |
					 CWSaveUnder     | CWEventMask        |
					 CWDontPropagate);

     /* don't do the cursor now -- it might not be created yet. It'll be
	rechecked and applied later */
     
     new_winattr_mask &= ~CWCursor;
     
     xwin = XMOVECreateWindow(new_fd,
			      new_seqno,
			      MapWindowID(window_ptr->window_id, Client2NewServer),
			      parent_window,
			      GeomX,
			      GeomY,
			      IShort((unsigned char *)&XGetGRep.width),
			      IShort((unsigned char *)&XGetGRep.height),
			      IShort((unsigned char *)&XGetGRep.borderWidth),
			      IShort((unsigned char *)&XGetARep.class) == InputOnly ? 0 : new_depth,
			      IShort((unsigned char *)&XGetARep.class),
			      CopyFromParent,
			      new_winattr_mask,
			      &new_attribs);
     
     Dprintf(("Creating window with id 0x%x\n parented by 0x%x\n",xwin,
	    parent_window));

     XMOVEGetSetProps (cur_window,
		       xwin,
		       ForceWindowPosition,
		       IShort((unsigned char *)&XGetGRep.x),
		       IShort((unsigned char *)&XGetGRep.y),
		       IShort((unsigned char *)&XGetGRep.width),
		       IShort((unsigned char *)&XGetGRep.height));
     
     /* top-most windows already had window_ptr->mapped set in move.c */

     if (window_ptr->parent_id != client->root_window) {
	  if (XGetARep.mapState)
	       window_ptr->mapped = 1;
	  else
	       window_ptr->mapped = 0;
     }
     
     return xwin;

}


/* figure out the stacking of siblings that are mapped and recreate */
Global Window
XMOVEStackWindows(window_ptr)
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
	  
	  xwin = MapWindowID(ILong(stack+(4*counter)), Reply);

	  Dprintf(("Stacking 0x%x parented by 0x%x\n",ILong(stack+(4*counter))
		 ,cur_window))

	  temp_window = FindWindowFromCurrentClient(xwin);

	  if (temp_window && temp_window->mapped) {
	       xwin = MapWindowID(xwin, Client2NewServer);
	       XMOVEMapWindow(new_fd, new_seqno, xwin);
	  }
     }

     free(stack);
}
