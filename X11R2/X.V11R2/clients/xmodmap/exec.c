/*
 * xmodmap - program for loading keymap definitions into server
 *
 * $Source: /usr/expo/X/src/clients/xmodmap/RCS/exec.c,v $
 * $Header: exec.c,v 1.2 88/02/24 08:11:08 jim Exp $
 *
 * Copyright 1988 Massachusetts Institute of Technology
 * Copyright 1987 by Sun Microsystems, Inc. Mountain View, CA.
 *
 *                     All Rights Reserved
 * 
 * Permission  to  use,  copy,  modify,  and  distribute   this
 * software  and  its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright no-
 * tice  appear  in all copies and that both that copyright no-
 * tice and this permission notice appear in  supporting  docu-
 * mentation,  and  that the names of Sun or MIT not be used in
 * advertising or publicity pertaining to distribution  of  the
 * software  without specific prior written permission. Sun and
 * M.I.T. make no representations about the suitability of this
 * software for any purpose. It is provided "as is" without any
 * express or implied warranty.
 * 
 * SUN DISCLAIMS ALL WARRANTIES WITH REGARD TO  THIS  SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FIT-
 * NESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL SUN BE  LI-
 * ABLE  FOR  ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
 * ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,  DATA  OR
 * PROFITS,  WHETHER  IN  AN  ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION  WITH
 * THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Author:  Jim Fulton, MIT X Consortium; derived from parts of the
 * original xmodmap, written by David Rosenthal, of Sun Microsystems.
 */

#include <X11/Xos.h>
#include <X11/Xlib.h>
#include <stdio.h>
#include "xmodmap.h"
#include "wq.h"

/*
 * UpdateModifierMapping - this sends the modifier map to the server
 * and deals with retransmissions due to the keyboard being busy.
 */

int UpdateModifierMapping (map)
    XModifierKeymap *map;
{
    int retries, timeout;

    for (retries = 5, timeout = 2; retries > 0; retries--, timeout <<= 1) {
	int result;

	result = XSetModifierMapping (dpy, map);
	switch (result) {
	  case MappingSuccess:	/* Success */
	    return (0);
	  case MappingBusy:		/* Busy */
	    fprintf (stderr, "%s:  You have %d seconds to lift your hands\n",
		     ProgramName, timeout);
	    sleep (timeout);
	    continue;
	  case MappingFailed:
	    fprintf (stderr, "%s: bad set modifier mapping.\n",
		     ProgramName);
	    return (-1);
	  default:
	    fprintf (stderr, "%s:  bad return %d from XSetModifierMapping\n",
		     ProgramName, result);
	    return (-1);
	}
    }
    fprintf (stderr,
	     "%s:  unable to set modifier mapping, keyboard problem\n",
	     ProgramName);
    return (-1);
}


/*
 * AddModifier - this adds a keycode to the modifier list
 */

int AddModifier (mapp, keycode, modifier)
    XModifierKeymap **mapp;
    KeyCode keycode;
    int modifier;
{
    if (keycode) {
	*mapp = XInsertModifiermapEntry (*mapp, keycode, modifier);
	return (0);
    } else {
	return (-1);
    }
    /*NOTREACHED*/
}


/*
 * DeleteModifier - this removes a keycode from the modifier list
 */

int RemoveModifier (mapp, keycode, modifier)
    XModifierKeymap **mapp;
    KeyCode keycode;
    int modifier;
{
    if (keycode) {
	*mapp = XDeleteModifiermapEntry (*mapp, keycode, modifier);
	return (0);
    } else {
	return (-1);
    }
    /*NOTREACHED*/
}


/*
 * ClearModifier - this removes all entries from the modifier list
 */

int ClearModifier (mapp, modifier)
    XModifierKeymap **mapp;
    int modifier;
{
    int i;
    XModifierKeymap *map = *mapp;
    KeyCode *kcp;

    kcp = &map->modifiermap[modifier * map->max_keypermod];
    for (i = 0; i < map->max_keypermod; i++) {
	*kcp++ = (KeyCode) 0;
    }
    return (0);
}


/*
 * print the contents of the map
 */

PrintModifierMapping (map, fp)
    XModifierKeymap *map;
    FILE *fp;
{
    int i, k = 0;

    fprintf (fp, 
    	     "%s:  up to %d keys per modifier, (keycodes in parentheses):\n\n", 
    	     ProgramName, map->max_keypermod);
    for (i = 0; i < 8; i++) {
	int j;

	fprintf(fp, "%-10s", modifier_table[i]);
	for (j = 0; j < map->max_keypermod; j++) {
	    if (map->modifiermap[k]) {
		KeySym ks = XKeycodeToKeysym(dpy, map->modifiermap[k], 0);
		char *nm = XKeysymToString(ks);

		fprintf (fp, "%s  %s (0x%0x)", (j > 0 ? "," : ""), 
			 (nm ? nm : "BadKey"), map->modifiermap[k]);
	    }
	    k++;
	}
	fprintf(fp, "\n");
    }
    fprintf (fp, "\n");
    return;
}

