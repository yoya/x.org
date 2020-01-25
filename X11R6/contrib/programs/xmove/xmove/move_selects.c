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

#include "xmove.h"
#include "xmond.h"

Global void
move_selections()
{
     unsigned char SelectionClearNotify[32];
     
     /* check this -- how to remove selections no longer held? */
     
     ISetByte(&SelectionClearNotify[0], 29);
     ISetLong(&SelectionClearNotify[4], 0); /* check this -- set time
					       correctly */
     
     ForAllInList(&client->selection_list)
     {
	  SelectionPtr selection_ptr;
	  Window owner;
	  
	  selection_ptr = (SelectionPtr)
	       CurrentContentsOfList(&client->selection_list);

	  owner = XMOVEGetSelectionOwner(cur_fd, cur_seqno,
					 MapAtom(selection_ptr->selection, Request));
	  
	  if ((owner & ~cur_mask) == cur_base) {
	       Atom new_atom = MapAtom(selection_ptr->selection, Client2NewServer);
	       Window new_window = MapWindowID(owner, Server2Server);
	       
	       Dprintf(("Attempting to set selection (s)%d to owner %X\n",
		      selection_ptr->selection, owner));
	       
	       XMOVESetSelectionOwner(new_fd, new_seqno, new_atom, new_window,
				      CurrentTime);

	       if (XMOVEGetSelectionOwner(new_fd, new_seqno, new_atom) ==
		   new_window)
		    Dprintf(("Selection successfully set\n"))
	       else {
		    /* check this -- this should remove the entry from the
		       client's selection_list */

		    ISetShort(&SelectionClearNotify[2],
			      client->SequenceMapping);
		    ISetLong(&SelectionClearNotify[8],
			     MapWindowID(owner, Event));
		    ISetLong(&SelectionClearNotify[12],
			     selection_ptr->selection);
		    SaveBytes(&(client->fdd->outBuffer),
			      (unsigned char *)&SelectionClearNotify, 32);
	       }
	  }
     }
}
