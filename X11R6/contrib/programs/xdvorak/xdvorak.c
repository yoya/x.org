/* xdvorak installs a Dvorak keyboard map on an X server. */
/* This is xdvorak.c version 1.2 of 94/05/20. */

/* Install a Dvorak keyboard map with the command:

		xdvorak

With no arguments, the program toggles between a Dvorak and a qwerty
keyboard map.  Any arguments force the installation of a qwerty
keyboard map.  Execute "xdvorak -qwerty" when you log out. */

/* John D. Ramsdell -- July 1988.  Original version. */

/* Jim Blandy -- March 1991.  Modified xdvorak so that KeyCode
permutations can be the result of changes required by KeySym's in
other than the the first column of a keymap row, and made the program
easier to understand. */

/* John D. Ramsdell -- May 1994.  Used XDisplayKeycodes to get
min_keycode and max_keycode.  The program exits by returning from
main rather than calling exit(). */

static char copyright[] = "Copyright 1994 by John D. Ramsdell.";
/* The X Consortium, and any party obtaining a copy of these files
from the X Consortium, directly or indirectly, is granted, free of
charge, a full and unrestricted irrevocable, world-wide, paid up,
royalty-free, nonexclusive right and license to deal in this software
and documentation files (the "Software"), including without limitation
the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons
who receive copies from any such party to do so, provided that the
above copyright notice appear in all copies.  This license includes
without limitation a license to do the foregoing actions under any
patents of the party supplying this software to the X Consortium.
John D. Ramsdell makes no representations about the suitability of
this software for any purpose.  It is provided "as is" without express
or implied warranty.  */

static char what[] = "@(#)xdvorak.c	1.2";

/* Compile with the command: cc -O -o xdvorak xdvorak.c -lX11
   Possible switches:
	-DDEBUG spews info about which keys are being remapped.
	-DDO_NOT_REMAP doesn't actually give the new mapping to the server.
*/

#include <stdio.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>

int is_qwerty(dpy)		/* Root window is checked for a */
     Display *dpy;		/* property that gives the keycode */
{				/* of the key marked D. */
  KeyCode current_key_marked_D;
  Window root;
  Atom key, type, actual_type_return;
  int actual_format_return;
  unsigned long nitems_return;
  unsigned long bytes_after_return;
  unsigned char *prop_return;

  current_key_marked_D = XKeysymToKeycode(dpy, XK_D);
  key = XInternAtom(dpy, "KEYCODE_OF_KEY_MARKED_D", False);
  type = XInternAtom(dpy, "KEYCODE", False);
  root = XDefaultRootWindow(dpy);

  XGetWindowProperty(dpy, root, key, 0L, 1L, False, type,
		     &actual_type_return, &actual_format_return,
		     &nitems_return, &bytes_after_return,
		     &prop_return);

  if (actual_type_return != type) { /* Property not there. */
    XChangeProperty(dpy, root, key, type, 8*sizeof(KeyCode),
		    PropModeReplace,
		    (unsigned char *)(&current_key_marked_D), 1);
    XFlush(dpy);		/* Record keycode with root window. */
    return True;
  } else
    return *((KeyCode *) prop_return) == current_key_marked_D;
}

#define NCHAR 256
KeySym qwerty[NCHAR], dvorak[NCHAR]; /* These will contain the permutations. */
int old_keysym_to_map_index[NCHAR];
Display *dpy;

/* translate from marked key to dvorak value.  If Q or D are too large
   for the table, we need to make a new table.  */
#define translate(Q, D)				\
{						\
  KeySym q = (Q);				\
  KeySym d = (D);				\
						\
  if (q >= NCHAR || d >= NCHAR)			\
    abort ();					\
  dvorak[q] = d;				\
  qwerty[d] = q;				\
}

/* Here is the ANSI Standard Dvorak layout.

	! @ # $ % ^ & * ( ) [ +
	1 2 3 4 5 6 7 8 9 0 ] =

	 " , . P Y F G C R L ?
	 ' , . p y f g c r l /

	  A O E U I D H T N S _
	  a o e u i d h t n s -

	   : Q J K X B M W V Z
	   ; Q J K X B M W V Z
*/

/* The following is a fairly machine independent description of
   minimal changes required to make a Dvorak keyboard.  You might want
   to tailor this for your machine.  */

void make_permutation()
{
  int i;
  for (i = 0; i < NCHAR; i++) {
    dvorak[i] = i;		/* Initialize with the  */
    qwerty[i] = i;		/* identify permutation. */
  }

  translate (XK_minus, 	XK_bracketleft);
  /* Next row. */
  translate (XK_Q,	XK_quoteright);
  translate (XK_W,	XK_comma);
  translate (XK_E,	XK_period);
  translate (XK_R,	XK_P);
  translate (XK_T,	XK_Y);
  translate (XK_Y,	XK_F);
  translate (XK_U,	XK_G);
  translate (XK_I,	XK_C);
  translate (XK_O,	XK_R);
  translate (XK_P,	XK_L);
  translate (XK_bracketleft,	XK_slash);
  /* Next row. */
  translate (XK_S,	XK_O);
  translate (XK_D,	XK_E);
  translate (XK_F,	XK_U);
  translate (XK_G,	XK_I);
  translate (XK_H,	XK_D);
  translate (XK_J,	XK_H);
  translate (XK_K,	XK_T);
  translate (XK_L,	XK_N);
  translate (XK_semicolon,	XK_S);
  translate (XK_quoteright,	XK_minus);
  /* Next row. */
  translate (XK_Z,	XK_semicolon);
  translate (XK_X,	XK_Q);
  translate (XK_C,	XK_J);
  translate (XK_V,	XK_K);
  translate (XK_B,	XK_X);
  translate (XK_N,	XK_B);
  translate (XK_comma,	XK_W);
  translate (XK_period,	XK_V);
  translate (XK_slash,	XK_Z);
}

int permute_keyboard_map(force_qwerty)
     int force_qwerty;
{
  KeySym *new_map, *old_map, *permutation;
  int min_keycode;
  int max_keycode;
  int num_keys;
  int keysyms_per_keycode;
  int am_qwerty = is_qwerty(dpy);
  int table_size;

  if (force_qwerty) {
    if (am_qwerty) return 0;	/* No change needed. */
    permutation = qwerty;
  } else permutation = am_qwerty ? dvorak : qwerty; /* Else toggle. */
  make_permutation();

  XDisplayKeycodes(dpy, &min_keycode, &max_keycode);
  num_keys = max_keycode - min_keycode + 1;

  /* Note that old_map and new_map are actually TWO-dimensional arrays,
     not flat arrays as their type would suggest.  Each group of
     keysyms_per_keycode elements is a row of the table.  */
  old_map =			/* old_map will not be modified. */
    XGetKeyboardMapping(dpy,
			min_keycode,
			num_keys,
			&keysyms_per_keycode);
  new_map =			/* new_map gets the new map. */
    XGetKeyboardMapping(dpy,
			min_keycode,
			num_keys,
			&keysyms_per_keycode);
#if defined DEBUG
  printf("max, min, per= %d, %d, %d\n",
	 max_keycode, min_keycode, keysyms_per_keycode);
#endif

  table_size = num_keys * keysyms_per_keycode;

  /* Since we want to move whole keys around, we move entire rows of
     the table around, not just particular elements.  If key X is
     becoming key Y, all the keysyms of keycode X are set to the
     keysyms of keycode Y.

     The permutation array only specifies what happens to the capital
     letters, but we need to remap all the keysyms associated with the
     key - the shifted, controlled and otherwise modified meanings.
     Thus, we use permutation to decide which rows of the keyboard
     mapping to change.  */

  /* old_keysym_to_map_index[i] is the index in old_map of the start
     of the row containing keysym i.  */
  {
    /* We scan a keycode at a time; map_row is the index of the first
       element of the current keycode's row in old_map.  */
    int map_row;

    for (map_row = 0; map_row < table_size; map_row += keysyms_per_keycode)
      {
	int i;
	
	for (i = 0; i < keysyms_per_keycode; i++)
	  if (old_map[map_row + i] < NCHAR
	      && old_map[map_row + i] != NoSymbol)
	    old_keysym_to_map_index[old_map[map_row + i]] = map_row;
      }
  }

  /* Scan new_map, looking for keycodes we'd like to swap.  */
  {
    int new_map_ix;			/* "Map Index"  */

    for (new_map_ix = 0; new_map_ix < table_size;)
      /* Does permutation remap this keysym?  */
      if (new_map[new_map_ix] < NCHAR
	  && permutation[new_map[new_map_ix]] != new_map[new_map_ix])
	{
	  int old_map_ix
	    = old_keysym_to_map_index[permutation[new_map[new_map_ix]]];
	  int i;

	  /* new_map_ix might be in the middle of a keycode's row, so back it
	     up to the beginning.  */
	  new_map_ix -= (new_map_ix % keysyms_per_keycode);

	  /* Change all the keysyms on this keycode.  */
	  for (i = 0; i < keysyms_per_keycode; i++)
	    {
#if defined DEBUG
	      if (old_map[old_map_ix] != NoSymbol)
		printf("Mapping %s->%s \t(%d->%d)\n",
		       XKeysymToString(old_map[new_map_ix]),
		       XKeysymToString(old_map[old_map_ix]),
		       old_map_ix, new_map_ix);
#endif
	      new_map[new_map_ix++] = old_map[old_map_ix++];
	    }
	}
      else
	new_map_ix++;
  }
	    
#ifndef DO_NOT_REMAP
  XChangeKeyboardMapping(dpy,	/* Install new map. */
			 min_keycode,
			 keysyms_per_keycode,
			 new_map,
			 num_keys);
#endif

  XFree((char *) old_map);	/* Release storage. */
  XFree((char *) new_map);
  XFlush(dpy);			/* Force out changes. */
  printf("%s keyboard installed.\n",
	 permutation == dvorak ? "Dvorak" : "Qwerty");
  return 0;			/* Permutation succeeded. */
}

int main(argc, argv)
     int argc;
     char **argv;
{ argv;				/* Not used. */

  if (!(dpy = XOpenDisplay(NULL))) {
    perror("Cannot open display\n");
    return 1;
  }

  /* Force installation of qwerty when any extra arguments are given. */
  return permute_keyboard_map(argc > 1);
}
