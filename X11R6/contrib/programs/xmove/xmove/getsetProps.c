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

#include <X11/Xlib.h>
#include <X11/Xproto.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>

#undef Bool			/* Xlib.h #defines Bool as int. undo this. */

#include "xmove.h"
#include "xmond.h"

Global void
XMOVEGetSetProps (Window oldwin,
		  Window newwin,
		  Bool ForceWindowPosition,
		  int x,
		  int y,
		  int width,
		  int height)
{
     xListPropertiesReply XListRep;
     xResourceReq XListReq;
     
     xGetPropertyReply XGetPRep;
     xGetPropertyReq XGetPReq;
     
     xChangePropertyReq XChaPReq;
     
     char *data, devnull[32]; /* devnull holds events to be discarded */
     
     unsigned char my_normal_hints[20];
     Atom *PropList, ActType;
     int PropCnt, ActFormat;
     unsigned long PropNItems, PropBytes;
     unsigned char *PropValue;
     Bool FoundNormalHints = False;
     
     /*
      * first set up the standard stuff in the request
      */
     
     Dprintf(("GETSETPROPS: Copying property list\n"));
	  
     XListReq.reqType = X_ListProperties;
     ISetShort((unsigned char *)&XListReq.length, 2);
     ISetLong((unsigned char *)&XListReq.id, oldwin);
     
     /* send the XListProperties request to the server */
     
     SendBuffer(cur_fd,
		(unsigned char *) &XListReq,
		sizeof(xResourceReq));
     
     /* read back the reply header */
     
     ReceiveReply(cur_fd, (unsigned char *) &XListRep,
		  sizeof(xListPropertiesReply), ++(*cur_seqno));
     
     /* allocate space for the properties list and retrieve list */
     
     PropCnt = XListRep.nProperties;
     PropList = (Atom *) calloc((unsigned int) PropCnt, sizeof(Atom));
     ReceiveBuffer(cur_fd,
		   (unsigned char *)PropList,
		   IShort((unsigned char *)&XListRep.nProperties) * sizeof(Atom));
     
     /* set-up XGetPReq constants*/
     XGetPReq.reqType = X_GetProperty;
     ISetShort((unsigned char *)&XGetPReq.length, 6);
     XGetPReq.delete = False;
     ISetLong((unsigned char *)&XGetPReq.window, oldwin);
     ISetLong((unsigned char *)&XGetPReq.type, 0);
     ISetLong((unsigned char *)&XGetPReq.longOffset, 0);
     ISetLong((unsigned char *)&XGetPReq.longLength, 1000000000); /* n, where n is VERY large */
     
     /* set-up XChaPReq constants */
     XChaPReq.reqType = X_ChangeProperty;
     XChaPReq.mode = 0;
     ISetLong((unsigned char *)&XChaPReq.window, newwin);
     
     while (PropCnt--) {
	  unsigned char *CurValue;
	  AtomPtr atom_ptr;
	  Atom atom, new_atom, map_atom, new_type, map_type;
	  Window map_win, conv_win;
	  void (*map_func)() = NULL;
	  
	  atom = new_atom = PropList[PropCnt];
	  map_atom = MapAtom(atom, Server2Server);
	  atom_ptr = FindServerAtom(atom);

	  /* if atom_ptr is NULL, that means this property has neither been
	     created by this client nor has it been examined by this client.
	     Don't recreate a property created by others and never used by
	     this client! */
	  
	  if (atom_ptr == NULL)
	       continue;

	  ISetLong((unsigned char *)&XGetPReq.property, atom);
	  
	  SendBuffer(cur_fd,
		     (unsigned char *) &XGetPReq,
		     sizeof(xGetPropertyReq));
	  
	  ReceiveReply(cur_fd, (unsigned char *) &XGetPRep, 
		       sizeof(xGetPropertyReply), ++(*cur_seqno));
	  
	  PropValue = (unsigned char *)calloc
	       (ILong((unsigned char *)&XGetPRep.length) << 2, sizeof(char));
	  
	  ReceiveBuffer(cur_fd,
			PropValue,
			ILong((unsigned char *)&XGetPRep.length) << 2);
	  
	  new_type = ILong((unsigned char *)&XGetPRep.propertyType);
	  map_type = MapAtom(new_type, Server2Server);

	  /* if this is the NORMAL_HINTS property, and we need to force the
	     window's position, change the flags in the property to say that
	     the window's position was chosen by the user and not by the
	     client. The window manager SHOULD place the window as specified
	     if this is set. */
	  
	  if (PropList[PropCnt] == XA_WM_NORMAL_HINTS && ForceWindowPosition) {
	       ISetLong(&PropValue[0], (ILong(&PropValue[0]) | USPosition) & ~(PPosition));
	       ISetLong(&PropValue[4], x);
	       ISetLong(&PropValue[8], y);
	       ISetLong(&PropValue[12], width);
	       ISetLong(&PropValue[16], height);
	       FoundNormalHints = True;
	  }

	  map_win = newwin;
	  
	  if (atom_ptr->property_map_func)
	       map_func = atom_ptr->property_map_func->MapProperty;
	  else {
	       atom_ptr = FindServerAtom(new_type);
	       if (atom_ptr && atom_ptr->type_map_func)
		    map_func = atom_ptr->type_map_func->MapProperty;
	  }
	  
	  if (map_func) {
	       conv_win = oldwin;
	       
	       (map_func)(MapMoveClientProps, &new_atom, &new_type, NULL,
			  atom_ptr, False, &conv_win, PropValue,
			  XGetPRep.format / 8  * ILong((unsigned char *)&XGetPRep.nItems));

	       if (new_atom != atom) /* if new_atom was changed */
		    map_atom = new_atom; /* use it instead of MapAtom() */

	       if (new_type != XGetPRep.propertyType)
		    map_type = new_type;

	       if (conv_win != oldwin)
		    map_win = conv_win;
	       
	  }
	  
	  XMOVEChangeProperty(new_fd, new_seqno, map_win, map_atom, map_type,
			      XGetPRep.format, PropModeReplace, PropValue,
			      ILong((unsigned char *)&XGetPRep.nItems));
	  
	  free(PropValue);
     }
     
     if (ForceWindowPosition && !FoundNormalHints) {
	  ISetLong(&my_normal_hints[0], USPosition);
	  ISetLong(&my_normal_hints[4], x);
	  ISetLong(&my_normal_hints[8], y);
	  ISetLong(&my_normal_hints[12], width);
	  ISetLong(&my_normal_hints[16], height);
	  
	  XMOVEChangeProperty(new_fd, new_seqno, newwin,
			      XA_WM_NORMAL_HINTS, XA_WM_SIZE_HINTS, 32,
			      PropModeReplace, &my_normal_hints[0], 18);
     }
     
     free((char *) PropList);
}
