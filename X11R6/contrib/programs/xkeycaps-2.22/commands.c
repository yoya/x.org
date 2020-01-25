/* xkeycaps, Copyright (c) 1991, 1992, 1993, 1994
 * by Jamie Zawinski <jwz@lucid.com>
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation.  No representations are made about the suitability of this
 * software for any purpose.  It is provided "as is" without express or 
 * implied warranty.
 */

#if __STDC__
#include <stdlib.h>
#endif

#include <stdio.h>
#include <X11/X.h>
#include <X11/Xos.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xproto.h>

#include <X11/StringDefs.h>
#include <X11/Intrinsic.h>
#include <X11/Shell.h>

#include <X11/Xmu/WinUtil.h>
#include <X11/Xmu/Error.h>

#include <X11/Xaw/Simple.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/Paned.h>
#include <X11/Xaw/List.h>
#include <X11/Xaw/Viewport.h>
#include <X11/Xaw/Dialog.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Toggle.h>
#include <X11/Xaw/SimpleMenu.h>
#include <X11/Xaw/SmeBSB.h>
#include <X11/Xaw/MenuButton.h>
#include <X11/Xaw/Scrollbar.h>

#ifdef XAWBUG
/* At some point it seemed like the Toggle widget (really, the Command widget)
   didn't provide a GetValues-hook to access the "state" property, so we had
   to access that slot directly.  Well, this isn't the case with MIT R5 pl25,
   or with OpenWindows 3.0, so let's try living without it now.
 */
# include <X11/IntrinsicP.h>
# include <X11/CoreP.h>
# include <X11/Xaw/SimpleP.h>
# include <X11/Xaw/CommandP.h>
#endif /* XAWBUG */

#include "KbdWidgetP.h"
#include "KeyWidgetP.h"

#include "xkeycaps.h"

#include "kbddef.h"

#include "vroot.h"	/* This is standard in R5 but not R4 */

#ifndef NO_PWD
#include <pwd.h>
#endif

/* We can only do Vendor keysyms in X11r5, because earlier versions 
   didn't provide any way to map over the contents of an xrm database.
 */
#ifdef XtSpecificationRelease
# if XtSpecificationRelease >= 5
#  define DO_VENDOR_KEYSYMS
# endif
#endif

extern int mappingNotify_event_expected;

struct key_menus {
  Widget popup;
  Widget popup_kids [10];
  struct edit_key_box *edit_key_box;
  struct select_kbd_box *select_kbd_box;
};

#ifndef isupper
# define isupper(c)  ((c) >= 'A' && (c) <= 'Z')
#endif
#ifndef _tolower
# define _tolower(c)  ((c) - 'A' + 'a')
#endif

int
string_equal (s1, s2)
     char *s1, *s2;
{
  if (s1 == s2) return 1;
  if (!s1 || !s2) return 0;
  while (*s1 && *s2)
    {
      if ((isupper (*s1) ? _tolower (*s1) : *s1) !=
	   (isupper (*s2) ? _tolower (*s2) : *s2))
	 return 0;
       s1++, s2++;
     }
  return ((*s1 || *s2) ? 0 : 1);
}



int y_or_n_p ();
static int y_or_n_p_with_args ();

extern Widget make_label (), make_label_1 ();

Widget
make_button (parent, name, string, left, top, callback, data, menu_name)
     Widget parent;
     char *name, *string;
     Widget left, top;
     void (*callback) ();
     XtPointer data;
     char *menu_name;
{
  Widget w = make_label_1 (parent, name, string, left, top,
			   (menu_name
			    ? menuButtonWidgetClass
			    : commandWidgetClass),
			   callback, data);
  if (menu_name)
    {
      Arg av [10];
      int ac = 0;
      XtSetArg (av [ac], XtNmenuName, menu_name); ac++;
      XtSetValues (w, av, ac);
    }
  return w;
}


Widget
make_toggle (parent, name, left, top, state, callback, data, label,
	     radio_group, radio_data)
     Widget parent;
     char *name;
     Widget left, top;
     int state;
     void (*callback) ();
     XtPointer data;
     char *label;
     Widget radio_group;
     XtPointer radio_data;
{
  Arg av [20];
  int ac = 0;
  Widget w;
  XtSetArg (av[ac], XtNleft, XtChainLeft); ac++;
  XtSetArg (av[ac], XtNright, XtChainLeft); ac++;
  XtSetArg (av[ac], XtNtop, XtChainTop); ac++;
  XtSetArg (av[ac], XtNbottom, XtChainTop); ac++;
  XtSetArg (av[ac], XtNjustify, XtJustifyLeft); ac++;
  XtSetArg (av[ac], XtNstate, (state ? True : False)); ac++;
  if (left) XtSetArg (av[ac], XtNfromHoriz, left),  ac++;
  if (top) XtSetArg (av[ac], XtNfromVert, top),  ac++;
  if (label) XtSetArg (av[ac], XtNlabel, label),  ac++;
  if (radio_group) XtSetArg (av[ac], XtNradioGroup, radio_group),  ac++;
  if (radio_data) XtSetArg (av[ac], XtNradioData, radio_data),  ac++;
  w = XtCreateManagedWidget (name, toggleWidgetClass, parent, av, ac);
  if (callback) XtAddCallback (w, XtNcallback, callback, data);
  return w;
}




static void
button_quit (widget, client_data, call_data)
     Widget widget;
     XtPointer client_data, call_data;
{
  exit (0);
}


static int modify_keyboard_modifiers (), modify_keyboard ();

static void
button_restore (button, client_data, call_data)
     Widget button;
     XtPointer client_data, call_data;
{
  KeyboardWidget widget = *((KeyboardWidget *) client_data);
  KeySym *keysyms;
  KeyCode lowest = 255, highest = 0;
  XModifierKeymap *modmap;
  int per_code = widget->keyboard.default_keysyms_per_code;
  int i, j, k;

  if (0 != y_or_n_p (widget, "restoreQuery", "yes", "no", 0))
    {
      message (widget, "Aborted.");
      return;
    }
  keysyms = (KeySym *) calloc (sizeof (KeySym), per_code * 256);
  modmap = XNewModifiermap (2); /* It'll grow */
  memset (modmap->modifiermap, 0, modmap->max_keypermod * 8);

  for (i = 0; i < widget->keyboard.nrows; i++)
    {
      struct KeyWidget_row *row = &widget->keyboard.rows [i];
      for (j = 0; j < row->nkeys; j++)
	{
	  KeyWidget key = row->keys [j];
	  if (key->key.keycode)
	    {
	      if (key->key.keycode < lowest)
		lowest = key->key.keycode;
	      if (key->key.keycode > highest)
		highest = key->key.keycode;
	      for (k = 0; k < per_code; k++)
		keysyms [key->key.keycode * per_code + k] =
		  key->key.default_keysyms [k];
	      if (key->key.default_mods)
		for (k = 0; k < 8; k++)
		  if (key->key.default_mods & (1<<k))
		    modmap = XInsertModifiermapEntry (modmap,
						      key->key.keycode,
						      k);
	    }
	}
    }
  if (highest <= lowest) exit (-69); /* can't happen */

  if (! modify_keyboard (widget, lowest, per_code,
			 keysyms + (lowest * per_code),
			 highest - lowest, modmap))
    message (widget, "Keyboard restored to default state.");
  XFreeModifiermap (modmap);
  free (keysyms);
}

extern char *version, *short_version;

extern KeyWidget keycode_to_key ();

static char *bit_names[] = { "Shift", "Lock", "Control", "Mod1",
			      "Mod2", "Mod3", "Mod4", "Mod5" };

static void
button_write (button, client_data, call_data)
     Widget button;
     XtPointer client_data, call_data;
{
  KeyboardWidget widget = *((KeyboardWidget *) client_data);
  XModifierKeymap *current_modmap, *default_modmap;
  int per_code = widget->keyboard.default_keysyms_per_code;
  struct { KeyWidget key; int count; char *names[8]; }
     all [256], differences [256];
  int all_count = 0, diff_count = 0;
  KeySym *keysyms;
  int count, i, j, k;
  int partial = y_or_n_p (widget, "writeQuery", "full", "partial", "abort");
  long now = time ((long *) 0);
#ifdef NO_PWD
  char *userid = 0;
#else
  struct passwd *pw = (struct passwd *) getpwuid (getuid ());
  char *userid = (pw ? pw->pw_name : 0);
#endif
  KeyCode added [8][255];
  KeyCode subtracted [8][255];
  int added_count, subtracted_count;
  int cmkpm, dmkpm;
  int any_mod_changes = 0;

  if (partial >= 2)
    {
      message (widget, "Aborted.");
      return;
    }

  current_modmap = XGetModifierMapping (XtDisplay (widget));
  default_modmap = XNewModifiermap (2); /* It'll grow */
  memset (default_modmap->modifiermap, 0, default_modmap->max_keypermod * 8);

  for (i = 0; i < widget->keyboard.nrows; i++)
    {
      struct KeyWidget_row *row = &widget->keyboard.rows [i];
      for (j = 0; j < row->nkeys; j++)
	{
	  KeyWidget key = row->keys [j];
	  if (key->key.keycode)
	    {
	      unsigned long bits = key->key.modifier_bits;
	      keysyms = XGetKeyboardMapping (XtDisplay (widget),
					     key->key.keycode,
					     1, &count);
	      if (! keysyms) count = 0;
	      all [all_count].key = key;
	      for (; count > 0; count--)
		if (keysyms [count-1]) break;
	      if (count == 0)
		{
		  all [all_count].names [0] = "NoSymbol";
		  count = 1;
		}
	      else
		for (k = 0; k < count; k++)
		  {
		    char *str = "NoSymbol";
		    if (keysyms [k])
		      {
			str = XKeysymToString (keysyms [k]);
			if (! str)
			  {
			    /* core leak, but this shouldn't ever happen
			       unless there's a bug in libX11.a, or the
			       user did something stupid with xmodmap.
			     */
			    str = (char *) malloc (255);
			    sprintf (str, "0x%02X", keysyms [k]);
			  }
		      }
		    all [all_count].names [k] = str;
		  }
	      all [all_count].count = count;
	      if (! keysyms) count = 0;
	      if (count > per_code ||
		  ((!partial) && (bits != key->key.default_mods)) ||
		  (count > 0 && keysyms[0] != key->key.default_keysyms[0]) ||
		  (count > 1 && keysyms[1] != key->key.default_keysyms[1]) ||
		  (count > 2 && keysyms[2] != key->key.default_keysyms[2]) ||
		  (count > 3 && keysyms[3] != key->key.default_keysyms[3]) ||
		  (count > 4 && keysyms[4] != key->key.default_keysyms[4]) ||
		  (count > 5 && keysyms[5] != key->key.default_keysyms[5]) ||
		  (count > 6 && keysyms[6] != key->key.default_keysyms[6]) ||
		  (count > 7 && keysyms[7] != key->key.default_keysyms[7]))
		differences [diff_count++] = all [all_count];
	      all_count++;
	      for (k = 0; k < 8; k++)
		{
		  if (key->key.default_mods & (1<<k))
		    default_modmap =
		      XInsertModifiermapEntry (default_modmap,
					       key->key.keycode, k);
		  if (bits & (1<<k))
		    current_modmap =
		      XInsertModifiermapEntry (current_modmap,
					       key->key.keycode, k);
		}
	      if (keysyms) XFree ((char *) keysyms);
	    }
	}
    }

  /* I'd just like to take this moment to point out that C has all
     the expressive power of two dixie cups and a string.
   */
  cmkpm = current_modmap->max_keypermod;
  dmkpm = default_modmap->max_keypermod;
  memset (added, 0, sizeof (added));
  memset (subtracted, 0, sizeof (subtracted));

  for (i = 0; i < 8; i++)
    {
      KeyCode kc1, kc2;
      added_count = subtracted_count = 0;
      if (partial)
	{
	  for (j = 0; j < cmkpm; j++)
	    {
	      kc1 = current_modmap->modifiermap [i * cmkpm + j];
	      if (!kc1) continue;
	      for (k = 0; k < dmkpm; k++)
		{
		  kc2 = default_modmap->modifiermap [i * dmkpm + k];
		  if (kc1 == kc2) break;
		}
	      if (kc1 != kc2) added [i][added_count++] = kc1;
	    }
	  for (j = 0; j < dmkpm; j++)
	    {
	      kc1 = default_modmap->modifiermap [i * dmkpm + j];
	      if (!kc1) continue;
	      for (k = 0; k < cmkpm; k++)
		{
		  kc2 = current_modmap->modifiermap [i * cmkpm + k];
		  if (kc1 == kc2) break;
		}
	      if (kc1 != kc2) subtracted [i][subtracted_count++] = kc1;
	    }
	  if (added_count || subtracted_count) any_mod_changes = 1;
	}
      else
	{
	  for (j = 0; j < cmkpm; j++)
	    {
	      kc1 = current_modmap->modifiermap [i * cmkpm + j];
	      if (kc1)
		added [i][added_count++] = kc1;
	    }
	}
    }

  printf ("!\n! This is an `xmodmap' input file for %s keyboards.\n",
	  widget->keyboard.long_name);
  {
    char *t = ctime (&now);
    t [strlen (t) - 1] = 0; /* bastards */
    printf ("! Automatically generated on %s", t);
  }
  if (userid)
    printf (" by %s", userid);
  printf (" with\n! %s.\n!\n", short_version);

  if (partial && any_mod_changes)
    {
      printf ("! This file presupposes that the keyboard is in the ");
      printf ("default state, and\n! may malfunction if it is not.\n!\n");
    }
  else if (! partial)
    {
      if (diff_count)
	printf ("! This file makes the following changes:\n!\n");
      else
	printf ("! This file encodes the default state.\n!\n");
    }

  /* If we're going to write out "remove" commands, do it before writing
     out any "keycode" statements, since xmodmap is so lame.  We only
     generate "remove" statements in "partial" mode.
   */
  any_mod_changes = 0;
  if (partial)
    for (i = 0; i < 8; i++)
      {
	char *str;
	KeySym ks;
	if (subtracted [i][0])
	  {
	    printf ("remove %-8s=", bit_names [i]);
	    for (j = 0; j < sizeof (subtracted [0]); j++)
	      {
		KeyWidget kw;
		if (! subtracted [i][j]) break;
		any_mod_changes = 1;
		/* note: we don't use the *current* keysym corresponding to the
		   keycode being removed here, but the *default* one...
		 */
		kw = keycode_to_key (widget, subtracted [i][j]);
		ks = kw->key.default_keysyms [0];
		str = XKeysymToString (ks);
		if (str)
		  printf (" %s", str);
		else
		  printf (" 0x%04X", subtracted [i][j]);
	      }
	    printf ("\n");
	  }
      }
  if (any_mod_changes) printf ("\n");

  /* Write out the differences.  This happens in both "partial" and "full"
     modes, but in "full" mode it's in the form of a descriptive comment
     instead of xmodmap commands.
   */
  for (i = 0; i < diff_count; i++)
    {
      if (partial)
	printf ("keycode 0x%02X =\t", differences[i].key->key.keycode);
      else
	printf ("! The \"%s\" key generates ",
		differences[i].key->key.key_name);
      for (j = 0; j < differences[i].count; j++)
	{
	  printf ("%s", differences[i].names[j]);
	  if (j+1 == differences[i].count) continue;
	  if (partial)
	    {
	      putchar ('\t');
	      if (strlen (differences[i].names[j]) < 8) putchar ('\t');
	    }
	  else
	    {
	      if (j+1 == differences[i].count-1)
		printf ((j == 0) ? " and " : ", and ");
	      else
		printf (", ");
	    }
	}
      /* write the diffs of the modifier bits in the full-mode comment. */
      if (!partial &&
	  differences[i].key->key.default_mods !=
	  differences[i].key->key.modifier_bits)
	{
	  unsigned long bits = differences[i].key->key.modifier_bits;
	  int this_mod_count = 0;
	  if (! bits)
	    printf (", and has no modifiers\n");
	  else
	    {
	      printf (", and the ");
	      for (k = 0; k < 8; k++)
		{
		  if (bits & (1<<k))
		    {
		      if (this_mod_count++) printf ("/");
		      printf (bit_names [k]);
		    }
		}
	      printf (" modifier%s\n", (this_mod_count>1 ? "s" : ""));
	    }
	}
      else
	printf ("\n");
    }

  /* In "full" mode, write out all the "keycode" commands.  This is the
     first actual xmodmap command text in full mode.
   */
  if (!partial)
    {
      printf ("\n");
      for (i = 0; i < all_count; i++)
	{
	  printf ("keycode 0x%02X =\t", all [i].key->key.keycode);
	  for (j = 0; j < all[i].count; j++)
	    {
	      printf ("%s", all[i].names[j]);
	      if (j == all[i].count - 1) continue;
	      putchar ('\t');
	      if (strlen (all[i].names[j]) < 8) putchar ('\t');
	    }
	  printf ("\n");
	}
      printf ("\n");
      printf ("clear Shift\nclear Lock\nclear Control\nclear Mod1\n");
      printf ("clear Mod2\nclear Mod3\nclear Mod4\nclear Mod5\n");
    }

  /* In partial mode, write out any "add" commands.  This is after any
     "keycode" commands have already been output (and any "remove" commands
     before them.)  In full mode, the "add" commands are the whole story.
   */
  printf ("\n");
  for (i = 0; i < 8; i++)
    {
      char *str;
      KeySym ks;
      if (added [i][0])
	{
	  printf ("add    %-8s=", bit_names [i]);
	  for (j = 0; j < sizeof (added [0]); j++)
	    {
	      if (! added [i][j]) break;
	      ks = XKeycodeToKeysym (XtDisplay (widget), added [i][j], 0);
	      str = XKeysymToString (ks);
	      if (str)
		printf (" %s", str);
	      else
		printf (" 0x%04X", added [i][j]);
	    }
	  printf ("\n");
	}
    }
  fflush (stdout);
}


static Window
lowest_window (dpy, window, limit, x, y)
     Display *dpy;
     Window window, limit;
     int x, y;
{
  XWindowAttributes xgwa;
  XGetWindowAttributes (dpy, window, &xgwa);

  /* Ignore all windows which are not at or below `limit'.  This prevents
     us from assigning focus to windows which are part of the WM decoration,
     since `limit' is the result of XmuClientWindow() -- it limits us to
     descending the tree of the client proper.
   */
  if (window == limit) limit = 0;

  if (xgwa.x <= x &&
      xgwa.y <= y &&
      xgwa.x + xgwa.width > x &&
      xgwa.y + xgwa.height > y)
    {
      Window root, parent, *kids;
      unsigned int nkids;
      if (!XQueryTree (dpy, window, &root, &parent, &kids, &nkids))
	abort ();

      if (kids)
	{
	  unsigned int i;
	  Window kid = 0;
	  for (i = 0; i < nkids; i++)
	    {
	      kid = lowest_window (dpy, kids[i], limit,
				   x - xgwa.x, y - xgwa.y);
	      if (kid) break;
	    }
	  XFree ((char *)kids);
	  if (kid) return kid;
	}
      /* Don't accept windows outside of the client tree. */
      if (limit) return 0;
      /* Don't accept windows which don't take KeyPress. */
      if (! (xgwa.all_event_masks & KeyPressMask)) return 0;
      return window;
    }
  else
    return 0;
}

static void
button_pick_window (button, client_data, call_data)
     Widget button;
     XtPointer client_data, call_data;
{
  KeyboardWidget keyboard = *((KeyboardWidget *) client_data);
  Widget topmost, target;
  Display *dpy = XtDisplay (keyboard);
  Window root = RootWindowOfScreen (keyboard->core.screen);
  Window window = 0;
  Window wm_window;
  int click_x, click_y;
  int buttons = 0;
  int once = 0;
  XEvent event;

  message (keyboard, "Please select the window to type at:");

  if (XGrabPointer (dpy, root, False, ButtonPressMask | ButtonReleaseMask,
		    GrabModeSync, GrabModeAsync, root,
		    keyboard->keyboard.select_cursor, CurrentTime))
    {
      XBell (dpy, 0);
      message (keyboard, "Grab failed.");
      return;
    }
  
  while (window == 0 || buttons != 0)
    {
      /* allow one more event */
      XAllowEvents (dpy, SyncPointer, CurrentTime);
      XWindowEvent (dpy, root, ButtonPressMask|ButtonReleaseMask, &event);
      switch (event.type)
	{
	case ButtonPress:
	  once = 1;
	  if (window == 0 && event.xbutton.subwindow)
	    {
	      window = event.xbutton.subwindow; /* window selected */
	      click_x = event.xbutton.x;
	      click_y = event.xbutton.y;
	    }
	  buttons++;
	  break;
	case ButtonRelease:
	  if (buttons > 0) buttons--;
	  break;
	}
    }
  XUngrabPointer(dpy, CurrentTime);
  XSync (dpy, 0);

  wm_window = window;
  if (window && window != root) window = XmuClientWindow (dpy, window);
  topmost = (Widget) keyboard;
  while (XtParent (topmost)) topmost = XtParent (topmost);
  target = XtWindowToWidget (XtDisplay (topmost), window);
  if (window == XtWindow (topmost)) target = topmost;
  if (target || window == root) window = 0;
  if (window)
    {
      char buf [255];
      char buf2 [100];
      char *string1 = 0, *string2 = 0, *string3 = 0;
      char *name;
      XClassHint classhint;

      Window lowest = lowest_window (dpy, wm_window, window, click_x, click_y);
      if (! lowest)
	{
	  /* Maybe they clicked in the wrong place in the app.  Let's try
	     again by picking the exact center of the frame and looking
	     there for something that accepts keyboard input.
	   */
	  XWindowAttributes xgwa;
	  XGetWindowAttributes (dpy, wm_window, &xgwa);
	  lowest = lowest_window (dpy, wm_window, window,
				  xgwa.x + (xgwa.width / 2),
				  xgwa.y + (xgwa.height / 2));
	  if (! lowest)
	    {
	      /* Still couldn't find one; give up. */
	      XBell (dpy, 0);
	      message (keyboard,
		       "No window there that accepts keyboard input!");
	      goto done;
	    }
	}

      classhint.res_name = classhint.res_class = 0;
      if (! XGetClassHint (dpy, window, &classhint))
	classhint.res_name = classhint.res_class = 0;

      string1 = classhint.res_name;
      string2 = classhint.res_class;
      XFetchName (dpy, window, &string3);
      name = string3;
      if (string2 && string3 && string_equal (string2, string3)) string3 = 0;
      if (string1 && string3 && string_equal (string1, string3)) string3 = 0;
      if (string1 && string2 && string_equal (string1, string2)) string2 = 0;
      if (!string2) string2 = string3, string3 = 0;
      if (!string1) string1 = string2, string2 = 0;
      if (string1 && string2 && string3)
	sprintf (buf2, "%s / %s / %s", string1, string2, string3);
      else if (string1 && string2)
	sprintf (buf2, "%s / %s", string1, string2);
      else if (string1)
	sprintf (buf2, "%s", string1);
      else
	sprintf (buf2, "unnamed");

      if (window == lowest)
	sprintf (buf, "Keyboard focus locked on window 0x%X (%s)",
		 window, buf2);
      else
	sprintf (buf, "Keyboard focus locked on window 0x%X <- 0x%X (%s)",
		 lowest, window, buf2);
      message (keyboard, buf);

      if (!classhint.res_class)
	;
      else if (string_equal (classhint.res_class, "xterm"))
	message2 (keyboard,
	      "Remember to select \"Allow SendEvents\" from the XTerm menu.");
      else if (string_equal (classhint.res_class, "cmdtool") ||
	       string_equal (classhint.res_class, "shelltool") ||
	       string_equal (classhint.res_class, "textedit"))
	message2 (keyboard,
 "Did you remember to use the -defeateventsecurity option when launching it?");

      if (classhint.res_name) XFree (classhint.res_name);
      if (classhint.res_class) XFree (classhint.res_class);
      if (name) XFree (name);
      window = lowest;
    }
  else
    message (keyboard, "Keyboard-focus cleared.");
  
 done:
  keyboard->keyboard.target_window = window;
}



static int
modify_keyboard_modifiers (widget, modmap)
     KeyboardWidget widget;
     XModifierKeymap *modmap;
{
  Display *display = XtDisplay ((Widget) widget);
  int retries, timeout;
  char buf [255];
  for (retries = 4, timeout = 2; retries > 0; retries--, timeout *= 2)
    {
      int result;
      result = XSetModifierMapping (display, modmap);
      switch (result)
	{
	case MappingSuccess:
	  mappingNotify_event_expected = 1;
	  return 0;
	case MappingBusy:
	  sprintf (buf, "please release all keys withing %d seconds", timeout);
	  /* Calling message() doesn't work because exposes don't get
	     processed while we're sleeping.
	   */
	  message (widget, buf);
	  fprintf (stderr, "%s: %s\n", progname, buf);
	  XSync (display, 0);
	  sleep (timeout);
	  continue;
	case MappingFailed:
	  message (widget, "XSetModifierMapping() failed");
	  XBell (display, 0);
	  return -1;
	default:
	  sprintf (buf, "unknown return code %d from XSetModifierMapping()",
		   result);
	  message (widget, buf);
	  XBell (display, 0);
	  return -1;
	}
    }
  sprintf (buf, "XSetModifierMapping() failed");
  message (widget, buf);
  XBell (display, 0);
  return -1;
}


/* We install this as an error handler around the call to
   XChangeKeyboardMapping(), so that we can trap errors that that
   operation generates.  Gotta love this 1960's condition system...
 */
static int XChangeKeyboardMapping_error = 0;

static int
modify_keyboard_error_handler (dpy, error)
     Display *dpy;
     XErrorEvent *error;
{
  switch (error->request_code)
    {
    case X_ChangeKeyboardMapping:
      XChangeKeyboardMapping_error = 1;
      return 0;
    default:
      XmuPrintDefaultErrorMessage (dpy, error, stderr);
      exit (-1);
    }
}

static int
modify_keyboard (widget, first_keycode, keysyms_per_keycode, keysyms,
		 num_codes, modmap)
     KeyboardWidget widget;
     int first_keycode;
     int keysyms_per_keycode;
     KeySym *keysyms;
     int num_codes;
     XModifierKeymap *modmap;
{
  Display *display = XtDisplay ((Widget) widget);
  int (*old_handler) ();
  if (keysyms_per_keycode == 0) keysyms_per_keycode = 1;
  XSync (display, 0);
  mappingNotify_event_expected = 1;
  XChangeKeyboardMapping_error = 0;
  old_handler = XSetErrorHandler (modify_keyboard_error_handler);
  XChangeKeyboardMapping (display, first_keycode, keysyms_per_keycode,
			  keysyms, num_codes);
  /* Is there a race condition here?  Are we guarenteed that by calling
     XSync() we will get back an error generated by the previously-sent
     request?
   */
  XSync (XtDisplay ((Widget) widget), 0);
  XSetErrorHandler (old_handler);
  if (XChangeKeyboardMapping_error)
    {
      mappingNotify_event_expected = 0;
      message (widget, "XChangeKeyboardMapping() failed.");
      XBell (display, 0);
      return -1;
    }
  if (modmap)
    return modify_keyboard_modifiers (widget, modmap);
  return 0;
}


static void
restore_key_default (parent, client_data, call_data)
     Widget parent;
     XtPointer client_data, call_data;
{
  KeyboardWidget widget = (KeyboardWidget) client_data;
  KeyWidget key = widget->keyboard.key_under_mouse;
  KeySym *keysyms = key->key.default_keysyms;
  int per_code = widget->keyboard.default_keysyms_per_code;
  KeyCode code = key->key.keycode;
  unsigned long bits = key->key.default_mods;
  XModifierKeymap *modmap;
  int i, error;

  if (! code)
    {
      y_or_n_p (widget, "magicKeyError", "ok", 0, 0);
      return;
    }

  modmap = XGetModifierMapping (XtDisplay (widget));
  for (i = 0; i < 8; i++)
    if (bits & (1<<i))
      modmap = XInsertModifiermapEntry (modmap, code, i);
    else
      modmap = XDeleteModifiermapEntry (modmap, code, i);

  XSync (XtDisplay (widget), 0);
  error = modify_keyboard (widget, code, per_code, keysyms, 1, modmap);
  XFreeModifiermap (modmap);

  if (! error)
    {
      char *k, buf [255], buf2 [255], *b = buf;
      for (i = 0; i < per_code; i++)
	{
	  if (i) *b++ = ',', *b++ = ' ';
	  k = XKeysymToString (keysyms [i]);
	  if (keysyms [i] && !k)
	    {
	      sprintf (b, "0x%04X", keysyms [i]);
	      k = b;
	    }
	  else
	    {
	      if (! k) k = "NoSymbol";
	      strcpy (b, k);
	    }
	  b += strlen (k);
	}
      sprintf (buf2, "KeyCode 0x%02X restored to default state (%s)", 
	       key->key.keycode, buf);
      message (widget, buf2);
    }
  XSync (XtDisplay (widget), 0);
}


KeyWidget prompt_for_key ();


static void
swap_key (parent, client_data, call_data)
     Widget parent;
     XtPointer client_data, call_data;
{
  KeyboardWidget widget = (KeyboardWidget) client_data;
  XModifierKeymap *modmap = XGetModifierMapping (XtDisplay (widget));
  KeyWidget source_key = widget->keyboard.key_under_mouse;
  KeyWidget target_key;
  KeySym *source_keysyms;
  KeySym *target_keysyms;
  int source_count, target_count;
  unsigned long source_bits = source_key->key.modifier_bits;
  unsigned long target_bits;
  KeyCode source_code = source_key->key.keycode;
  KeyCode target_code;
  char buf [255];
  int i, error;

  if (source_key->key.keycode == 0)
    {
      y_or_n_p (widget, "magicKeyError", "ok", 0, 0);
      return;
    }

  sprintf (buf, "Click on the key to swap with 0x%02X (%s)",
	   source_key->key.keycode,
	   source_key->key.key_name);
  target_key = prompt_for_key (widget, buf);
  if (! target_key) return;

  target_bits = target_key->key.modifier_bits;
  target_code = target_key->key.keycode;
  
  if (target_code == 0)
    {
      y_or_n_p (widget, "magicKeyError", "ok", 0, 0);
      return;
    }

  if (source_code == target_code)
    {
      XBell (XtDisplay (widget), 0);
      message (widget, "Those keys are the same!");
      return;
    }

  for (i = 0; i < 8; i++)
    {
      if (source_bits & (1<<i))
	modmap = XInsertModifiermapEntry (modmap, target_code, i);
      else
	modmap = XDeleteModifiermapEntry (modmap, target_code, i);

      if (target_bits & (1<<i))
	modmap = XInsertModifiermapEntry (modmap, source_code, i);
      else
	modmap = XDeleteModifiermapEntry (modmap, source_code, i);
    }

  source_keysyms = XGetKeyboardMapping (XtDisplay (widget), source_code,
					1, &source_count);
  target_keysyms = XGetKeyboardMapping (XtDisplay (widget), target_code,
					1, &target_count);

  error = modify_keyboard ((Widget) widget, target_code,
			   source_count, source_keysyms, 1, modmap);
  if (error) return;
  error = modify_keyboard ((Widget) widget, source_code,
			   target_count, target_keysyms, 1, 0);
  if (error) return;

  sprintf (buf, "Keys 0x%02x (%s) and 0x%02x (%s) swapped.",
	   source_key->key.keycode, source_key->key.key_name,
	   target_key->key.keycode, target_key->key.key_name);
  message (widget, buf);

  if (source_keysyms) XFree ((char *) source_keysyms);
  if (target_keysyms) XFree ((char *) target_keysyms);
  if (modmap) XFreeModifiermap (modmap);
}


static void
clone_key (parent, client_data, call_data)
     Widget parent;
     XtPointer client_data, call_data;
{
  KeyboardWidget widget = (KeyboardWidget) client_data;
  XModifierKeymap *modmap = XGetModifierMapping (XtDisplay (widget));
  KeyWidget source_key = widget->keyboard.key_under_mouse;
  KeyWidget target_key;
  KeySym *source_keysyms;
  int source_count;
  unsigned long source_bits = source_key->key.modifier_bits;
  KeyCode source_code = source_key->key.keycode;
  KeyCode target_code;
  char buf [255];
  int i, error;

  if (source_key->key.keycode == 0)
    {
      y_or_n_p (widget, "magicKeyError", "ok", 0, 0);
      return;
    }

  sprintf (buf, "Click on the key to turn into a copy of 0x%02X (%s)",
	   source_key->key.keycode,
	   source_key->key.key_name);
  target_key = prompt_for_key (widget, buf);
  if (! target_key) return;

  target_code = target_key->key.keycode;
  
  if (target_code == 0)
    {
      y_or_n_p (widget, "magicKeyError", "ok", 0, 0);
      return;
    }

  if (source_code == target_code)
    {
      XBell (XtDisplay (widget), 0);
      message (widget, "Those keys are the same!");
      return;
    }
  for (i = 0; i < 8; i++)
    {
      if (source_bits & (1<<i))
	modmap = XInsertModifiermapEntry (modmap, target_code, i);
      else
	modmap = XDeleteModifiermapEntry (modmap, target_code, i);
    }

  source_keysyms = XGetKeyboardMapping (XtDisplay (widget), source_code,
					1, &source_count);
  error = modify_keyboard ((Widget) widget, target_code,
			   source_count, source_keysyms, 1, modmap);
  if (source_keysyms) XFree ((char *) source_keysyms);
  if (modmap) XFreeModifiermap (modmap);
  if (error) return;

  sprintf (buf, "Keys 0x%02x (%s) and 0x%02x (%s) are now the same.",
	   source_key->key.keycode, source_key->key.key_name,
	   target_key->key.keycode, target_key->key.key_name);
  message (widget, buf);
}



static void
disable_key (parent, client_data, call_data)
     Widget parent;
     XtPointer client_data, call_data;
{
  KeyboardWidget widget = (KeyboardWidget) client_data;
  KeyWidget key = widget->keyboard.key_under_mouse;
  KeyCode code = key->key.keycode;
  KeySym keysym = 0;
  int i, error;
  char buf [255];
  XModifierKeymap *modmap = XGetModifierMapping (XtDisplay (widget));
  if (code == 0)
    {
      y_or_n_p (widget, "magicKeyError", "ok", 0, 0);
      return;
    }
  for (i = 0; i < 8; i++)
    modmap = XDeleteModifiermapEntry (modmap, code, i);
  error = modify_keyboard ((Widget) widget, code, 1, &keysym, 1, modmap);
  XFreeModifiermap (modmap);
  if (! error)
    {
      sprintf (buf, "KeyCode 0x%02X (%s) disabled.", key->key.keycode,
	       key->key.key_name);
      message (widget, buf);
    }
  XSync (XtDisplay (widget), 0);
}



extern void key_menu_pre_popup_hook ();


void pop_up_key_dbox ();
void pop_up_kbd_dbox ();
static struct edit_key_box * make_edit_key_dbox ();
static struct select_kbd_box * make_select_kbd_dbox ();

struct key_menus *
make_key_menus (kbd)
     KeyboardWidget *kbd;
{
  KeyboardWidget widget = *kbd;
  Arg av [20];
  int ac = 0, i = 0;
  Widget menu, item;
  struct key_menus *key_menus = (struct key_menus *)
    malloc (sizeof (struct key_menus));
  memset (key_menus->popup_kids, 0, sizeof (key_menus->popup_kids));

  XawSimpleMenuAddGlobalActions (XtWidgetToApplicationContext((Widget)widget));

  XtSetArg (av[ac], XtNlabel, "keyMenu"); ac++;
  menu = XtCreatePopupShell ("keyMenu", simpleMenuWidgetClass,
			     (Widget) widget, av, ac);
  XtAddCallback (menu, XtNpopupCallback, key_menu_pre_popup_hook,
		 (XtPointer) widget);
  key_menus->popup = menu;
  ac = 0;
  item = XtCreateManagedWidget ("editKeysyms", smeBSBObjectClass, menu, av,ac);
  XtAddCallback (item, XtNcallback, pop_up_key_dbox, (XtPointer) widget);
  key_menus->popup_kids [i++] = item;

  item = XtCreateManagedWidget ("swapKey", smeBSBObjectClass, menu, av, ac);
  XtAddCallback (item, XtNcallback, swap_key, (XtPointer) widget);
  key_menus->popup_kids [i++] = item;

  item = XtCreateManagedWidget ("cloneKey", smeBSBObjectClass, menu, av, ac);
  XtAddCallback (item, XtNcallback, clone_key, (XtPointer) widget);
  key_menus->popup_kids [i++] = item;

  item = XtCreateManagedWidget ("disableKey", smeBSBObjectClass, menu, av, ac);
  XtAddCallback (item, XtNcallback, disable_key, (XtPointer) widget);
  key_menus->popup_kids [i++] = item;

  item = XtCreateManagedWidget ("restoreKey", smeBSBObjectClass, menu, av, ac);
  XtAddCallback (item, XtNcallback, restore_key_default, (XtPointer) widget);
  key_menus->popup_kids [i++] = item;

  key_menus->edit_key_box = make_edit_key_dbox (widget);
  key_menus->select_kbd_box = make_select_kbd_dbox (widget);
  return key_menus;
}

void
sensitize_menu (widget, menu, sensitive)
     KeyboardWidget widget;
     Widget menu;
     Bool sensitive;
{
  Arg av [10];
  int ac = 0, i = 0;
  struct key_menus *key_menus = widget->keyboard.key_menus;
  if (menu != key_menus->popup) return;
  XtSetArg (av [ac], XtNsensitive, sensitive); ac++;
  for (i = 0; i < sizeof (key_menus->popup_kids); i++)
    if (! key_menus->popup_kids [i]) return;
    else XtSetValues (key_menus->popup_kids [i], av, ac);
}



extern Pixmap get_keyboard_icon ();

Widget
make_command_widgets (parent, kbd)
     Widget parent;
     Widget *kbd;
{
  Widget box, top, kmenu, item;
  Arg av [20];
  int i, ac = 0;
  XtSetArg (av[ac], XtNleft, XtChainLeft); ac++;
  XtSetArg (av[ac], XtNright, XtChainLeft); ac++;
  XtSetArg (av[ac], XtNtop, XtChainTop); ac++;
  XtSetArg (av[ac], XtNbottom, XtChainTop); ac++;
  box = XtCreateManagedWidget ("buttons", formWidgetClass, parent, av, ac);
  top = 0;
  top = make_button (box, "quit", NULL, 0, top, button_quit, kbd, 0);
  top = make_button (box, "keyboard", NULL, 0, top, pop_up_kbd_dbox, kbd, 0);
  top = make_button (box, "focus", NULL, 0, top, button_pick_window, kbd, 0);
  top = make_button (box, "restore", NULL, 0, top, button_restore, kbd, 0);
  top = make_button (box, "write", NULL, 0, top, button_write, kbd, 0);

  return box;
}



/* These are used to compute the default sizes of the windows.  Hack hack.
 */
#define LONGEST_KEYSYM_NAME "Greek_upsilonaccentdieresis"
#define MEDIUM_LENGTH_KEYSYM_NAME "Greek_IOTAdiaresis"

static char *all_keyset_names [] = {
  "Latin1",
  "Latin2",
  "Latin3",
  "Latin4",
  "Kana",
  "Arabic",
  "Cyrillic",
  "Greek",
  "Technical",
  "Special",
  "Publishing",
  "APL",
  "Hebrew",
  "Keyboard",
#ifdef DO_VENDOR_KEYSYMS
  "Vendor",	/* man, is this hairy... */
#endif
  0
};

#define KEYBOARD_CHARSET_INDEX 13
#ifdef DO_VENDOR_KEYSYMS
# define VENDOR_CHARSET_INDEX 14
#endif

static char **keysym_name_buffer = 0;
static int keysym_name_buffer_size = 0;

static void
ensure_keysym_name_buffer (size)
     int size;
{
  if (keysym_name_buffer_size >= size)
    return;
  if (keysym_name_buffer) free ((char *) keysym_name_buffer);
  keysym_name_buffer_size = size;
  keysym_name_buffer = (char **) malloc (sizeof (char *) * size);
}


#ifdef DO_VENDOR_KEYSYMS
struct vendor_keysym {
  char *name;
  KeySym keysym;
  int index;
};

static struct vendor_keysym *vendor_keysyms = 0;
static int n_vendor_keysyms = 0;

static XrmDatabase vendor_keysym_db = 0;

static Bool
init_vendor_keysyms_mapper (db, bindings, quarks, type, value, closure)
     XrmDatabase *db;
     XrmBindingList bindings;
     XrmQuarkList quarks;
     XrmRepresentation *type;
     XrmValue *value;
     XPointer closure;
{
  KeySym keysym;
  char c;
  int i;
  int *count = (int *) closure;
  if (quarks [1]) /* should only be one level deep. */
    abort ();

  if (! value->addr || value->size <= 1)
    return False;

  keysym = 0;
  for (i = 0; i < value->size - 1; i++)
    {
      c = ((char *) value->addr) [i];
      if ('0' <= c && c <= '9') keysym = (keysym<<4)+c-'0';
      else if ('a' <= c && c <= 'z') keysym = (keysym<<4)+c-'a'+10;
      else if ('A' <= c && c <= 'Z') keysym = (keysym<<4)+c-'A'+10;
      else
	{
	  fprintf (stderr, "%s: unparsable entry in XKeysymDB: \"%s: %s\"\n",
		   progname, XrmQuarkToString (quarks [0]),
		   (char *) value->addr);
	  return False;
	}
    }
  
  if (n_vendor_keysyms <= *count)
    {
      n_vendor_keysyms *= 2;
      vendor_keysyms = (struct vendor_keysym *)
	realloc (vendor_keysyms, sizeof (struct vendor_keysym) *
		 n_vendor_keysyms);
    }
  vendor_keysyms [*count].index = -1; /* we fill this in later */
  vendor_keysyms [*count].keysym = keysym;
  vendor_keysyms [*count].name = (char *) XrmQuarkToString (quarks [0]);
  (*count)++;
  return False;
}


static int
sort_vendor_keysyms_1 (left, right)
     int left, right;
{
  int L = left, R = right, middle;
  struct vendor_keysym swap;
  KeySym pivot = vendor_keysyms [left].keysym;
  while (L < R)
    {
      while (vendor_keysyms [L].keysym <= pivot && L <= right)
	L++;
      while (vendor_keysyms [R].keysym > pivot && R >= left)
	R--;
      if (L < R)
	{
	  swap = vendor_keysyms [L];
	  vendor_keysyms [L] = vendor_keysyms [R];
	  vendor_keysyms [R] = swap;
	}
    }
  middle = R;
  swap = vendor_keysyms [left];
  vendor_keysyms [left] = vendor_keysyms [middle];
  vendor_keysyms [middle] = swap;
  if (left < middle - 1)
    middle = sort_vendor_keysyms_1 (left, middle - 1);
  if (middle + 1 < right)
    middle = sort_vendor_keysyms_1 (middle + 1, right);
  return middle;
}


static void
sort_vendor_keysyms ()
{
  int i;
  sort_vendor_keysyms_1 (0, n_vendor_keysyms - 1);
  for (i = 0; i < n_vendor_keysyms; i++)
    {
      if (i > 0 && vendor_keysyms [i].keysym < vendor_keysyms [i-1].keysym)
	abort ();
      vendor_keysyms [i].index = i;
    }
}


#ifndef NO_XInitKeysymDB
extern XrmDatabase _XInitKeysymDB ();
#endif

static void
init_vendor_keysyms (widget)
     KeyboardWidget widget;
{
  static int done = 0;
  int count = 0;
  XrmName name = { 0 };
  XrmClass class = { 0 };

  if (done) return;
  done = 1;

  if (! vendor_keysym_db)
    {
      char *dbname = (char *) getenv ("XKEYSYMDB");
#ifdef KEYSYMDB
      if (! dbname) dbname = KEYSYMDB;
#endif /*  KEYSYMDB */

#ifdef NO_XInitKeysymDB
      if (dbname)
	vendor_keysym_db = XrmGetFileDatabase (dbname);
#else  /* we have _XInitKeysymDB(), so let Xlib be clever for us. */
      vendor_keysym_db = _XInitKeysymDB ();
#endif /* _XInitKeysymDB */

      if (! vendor_keysym_db) /* still don't have it... */
	{
	  if (dbname)
	    y_or_n_p_with_args (widget, "noKeysymDBError", "ok", 0, 0,
				dbname, 0, 0, 0, 0, 0);
	  else
	    y_or_n_p (widget, "unknownKeysymDBError", "ok", 0, 0);
	  return;
	}
    }

  n_vendor_keysyms = 255; /* probably not more than this; it's realloc'd. */
  vendor_keysyms = (struct vendor_keysym *)
    malloc (n_vendor_keysyms * sizeof (struct vendor_keysym));

  XrmEnumerateDatabase (vendor_keysym_db, &name, &class,
			XrmEnumOneLevel, init_vendor_keysyms_mapper,
			(XtPointer) &count);

  if (count < n_vendor_keysyms) /* might as well shrink it */
    {
      n_vendor_keysyms = count;
      vendor_keysyms = (struct vendor_keysym *)
	realloc (vendor_keysyms, count * sizeof (struct vendor_keysym));
    }
  else if (! count)
    {
      free ((char *) vendor_keysyms);
      n_vendor_keysyms = 0;
      vendor_keysyms = 0;
      return;
    }
  /* Hash order isn't very pretty; sort them by keysym numeric value. */
  sort_vendor_keysyms ();
}

static void
fill_keysym_name_buffer_with_vendor_keysyms (keyboard)
     KeyboardWidget keyboard;
{
  int i;
  if (! vendor_keysyms) init_vendor_keysyms (keyboard);
  ensure_keysym_name_buffer (n_vendor_keysyms + 1);
  for (i = 0; i < n_vendor_keysyms; i++)
    keysym_name_buffer [i] = vendor_keysyms [i].name;
  keysym_name_buffer [i] = 0;
}

static int
vendor_keysym_to_small_index (keyboard, keysym)
     KeyboardWidget keyboard;
     KeySym keysym;
{
  int i;
  char buf [255];
  if (! vendor_keysyms) init_vendor_keysyms (keyboard);
  for (i = 0; i < n_vendor_keysyms; i++)
    if (keysym == vendor_keysyms [i].keysym)
      return vendor_keysyms [i].index;
  sprintf (buf, "Unrecognised vendor keysym 0x%08X.", keysym);
  XBell (XtDisplay ((Widget) keyboard), 0);
  message (keyboard, buf);
  fprintf (stderr, "%s: %s\n", progname, buf);
  return 0;
}

#endif /* DO_VENDOR_KEYSYMS */


struct edit_key_box {
  KeyboardWidget keyboard;
  Widget shell;
  Widget label;
  Widget keysym_buttons [8];
  Widget mod_buttons [8];
  Widget keyset_list, keysym_list;
  Widget autorepeat_widget;
  int autorepeat;
};


static void
keyset_list_cb (widget, client_data, call_data)
     Widget widget;
     XtPointer client_data, call_data;
{
  XawListReturnStruct *lr = (XawListReturnStruct *) call_data;
  struct edit_key_box *box = (struct edit_key_box *) client_data;
  int set = lr->list_index;
  int i, j = 0;
  int set_start = 0;

  if (set == KEYBOARD_CHARSET_INDEX) set = 255;

  if (set != 0) set_start = 1; /* Latin1 is the only set that has NoSymbol */

#ifdef DO_VENDOR_KEYSYMS
  if (set == VENDOR_CHARSET_INDEX)
    fill_keysym_name_buffer_with_vendor_keysyms (box->keyboard);
  else
#endif
    {
      ensure_keysym_name_buffer (256);
      for (i = set_start; i < 256; i++)
	{
	  char *name = XKeysymToString ((set << 8) | i);
	  if (! name && i == 0) name = "NoSymbol";
	  if (name)
	    keysym_name_buffer [j++] = name;
	}
      keysym_name_buffer [j++] = 0;
    }
  XawListChange (box->keysym_list, keysym_name_buffer, 0, 0, True);
}


static void
keysym_list_cb (widget, client_data, call_data)
     Widget widget;
     XtPointer client_data, call_data;
{
  XawListReturnStruct *lr = (XawListReturnStruct *) call_data;
  struct edit_key_box *box = (struct edit_key_box *) client_data;
  int i = (int) XawToggleGetCurrent (box->keysym_buttons [0]);
  if (i > 0)
    {
      Arg av [10];
      int ac = 0;
      XtSetArg (av [ac], XtNlabel, lr->string); ac++;
      XtSetValues (box->keysym_buttons [i-1], av, ac);
    }
}


static void
autorepeat_button_cb (widget, client_data, call_data)
     Widget widget;
     XtPointer client_data, call_data;
{
  struct edit_key_box *box = (struct edit_key_box *) client_data;
  Arg av [10];
  int ac = 0;
  box->autorepeat = !box->autorepeat;
  XtSetArg (av [ac], XtNlabel, (box->autorepeat ? "Yes" : "No")); ac++;
  XtSetValues (widget, av, ac);
}


static void
abort_button_cb (widget, client_data, call_data)
     Widget widget;
     XtPointer client_data, call_data;
{
  struct edit_key_box *box = (struct edit_key_box *) client_data;
  XtPopdown (box->shell);
  message (box->keyboard, "Aborted.");
}


static int stop_the_insanity ();

static void
ok_button_cb (button, client_data, call_data)
     Widget button;
     XtPointer client_data, call_data;
{
  struct edit_key_box *box = (struct edit_key_box *) client_data;
  KeyboardWidget widget = box->keyboard;
  KeyWidget key = widget->keyboard.key_under_mouse;
  KeyCode code = key->key.keycode;
  KeySym keysyms [8];
  int keysym_count;
  unsigned long modbits = 0;
  XModifierKeymap *modmap = XGetModifierMapping (XtDisplay (widget));
  int i, error;
  Arg av [10];
  int ac = 0;
  char *str;

  ac = 0;
  XtSetArg (av [ac], XtNlabel, &str); ac++;
  for (i = 0; i < 8; i++)
    {
      XtGetValues (box->keysym_buttons [i], av, ac);
      if (str && !strcmp (str, "NoSymbol")) str = 0;
      if (! str)
	keysyms [i] = 0;
      else
	{
	  keysyms [i] = XStringToKeysym (str);
	  if (! keysyms [i])
	    fprintf (stderr,
		     "%s: ERROR: XStringToKeysym(\"%s\") returned 0\n",
		     progname, str);
	}
    }
  for (keysym_count = 8; keysym_count > 0; keysym_count--)
    if (keysyms [keysym_count-1]) break;

  for (i = 0; i < 8; i++)
    {
#ifndef XAWBUG
      Boolean state = 0;
      XtVaGetValues (box->mod_buttons[i], XtNstate, &state, NULL);
#else /* XAWBUG */
      int state = ((CommandWidget) (box->mod_buttons [i]))->command.set;
#endif
      modbits |= ((!!state) << i);
      if (state)
	modmap = XInsertModifiermapEntry (modmap, code, i);
      else
	modmap = XDeleteModifiermapEntry (modmap, code, i);
    }

  if (stop_the_insanity (widget, code, keysyms, keysym_count, modbits, modmap))
    return;

  XSync (XtDisplay (widget), 0);
  error = modify_keyboard ((Widget) widget, code, keysym_count,
			   keysyms, 1, modmap);
  XFreeModifiermap (modmap);

  if (!error && box->autorepeat != key->key.auto_repeat_p)
    {
      XKeyboardControl values;
      values.key = key->key.keycode;
      values.auto_repeat_mode =
	(box->autorepeat ? AutoRepeatModeOn : AutoRepeatModeOff);
      XChangeKeyboardControl (XtDisplay (widget), KBKey | KBAutoRepeatMode,
			      &values);
    }

  XtPopdown (box->shell);

  if (! error) message (box->keyboard, "Modified.");
}


static void
move_scrollbar (list, percent)
     Widget list;
     float percent;
{
  Widget vp = XtParent (list);
  Widget scrollbar = XtNameToWidget (vp, "vertical");
  float visible_fraction = (((float) vp->core.height) /
			    ((float) list->core.height));
  if (visible_fraction < 1.0)
    percent -= (visible_fraction / 2.0);
  XawScrollbarSetThumb (scrollbar, 0.0, -1.0);
  XtCallCallbacks (scrollbar, XtNjumpProc, (XtPointer) &percent);
}


static void
keysym_button_cb (widget, client_data, call_data)
     Widget widget;
     XtPointer client_data, call_data;
{
  struct edit_key_box *box = (struct edit_key_box *) client_data;
  KeyboardWidget keyboard = box->keyboard;
  char *keysym_name;
  KeySym real_keysym;
  int keyset, keysym, index, list_size;
  Arg av[10];
  int ac = 0;

  if (call_data == 0)	/* we're being toggled off */
    return;

  XtSetArg (av[ac], XtNlabel, &keysym_name); ac++;
  XtGetValues (widget, av, ac);
  real_keysym = XStringToKeysym (keysym_name);
  /* Get the one that's in the list */
  keysym_name = XKeysymToString (real_keysym);

  if (real_keysym > 0xFFFF)
    {
#ifdef DO_VENDOR_KEYSYMS
      keyset = VENDOR_CHARSET_INDEX;
      keysym = vendor_keysym_to_small_index (keyboard, real_keysym);
#else
      XBell (XtDisplay (keyboard), 0);
      message (keyboard,
	       "XKeyCaps was compiled without support for Vendor keysyms.");
      message2 (keyboard, "Consider upgrading to X11r5...");
      keyset = 0;
      keysym = 0;
#endif
    }
  else
    {
      keyset = (real_keysym >> 8);
      keysym = (real_keysym & 0xff);
      if (keyset == 255) keyset = KEYBOARD_CHARSET_INDEX;
    }

  list_size = (sizeof (all_keyset_names) / sizeof (all_keyset_names[0]));
  XawListHighlight (box->keyset_list, keyset);
  move_scrollbar (box->keyset_list, (((float) keyset) / ((float) list_size)));
  keyset_list_cb (box->keyset_list, box,
		  XawListShowCurrent (box->keyset_list));

  index = 256;
  for (list_size = 0; list_size < 256; list_size++)
    if (keysym_name_buffer [list_size] == keysym_name)
      index = list_size;
    else if (! keysym_name_buffer [list_size])
      break;
  if (! keysym_name) index = 0;
  if (index < 256)
    XawListHighlight (box->keysym_list, index);
  else
    index = 0;
  move_scrollbar (box->keysym_list, (((float) index) / ((float) list_size)));
}


static void
undo_button_cb (widget, client_data, call_data)
     Widget widget;
     XtPointer client_data, call_data;
{
  struct edit_key_box *box = (struct edit_key_box *) client_data;
  KeyboardWidget keyboard = box->keyboard;
  KeyWidget key = keyboard->keyboard.key_under_mouse;
  KeySym *keysyms;
  int syms_per_code;
  char buf [255];
  Arg av[20];
  int ac = 0;
  int i;

  keysyms = XGetKeyboardMapping (XtDisplay (widget), key->key.keycode,
				 1, &syms_per_code);

  sprintf (buf, "Definition of key 0x%02X (%s)", key->key.keycode,
	   key->key.key_name);
  XtSetArg (av[ac], XtNlabel, buf); ac++;
  XtSetValues (box->label, av, ac);
  ac = 0;

  for (i = 0; i < syms_per_code; i++)
    {
      char *sym;
      char buf [255];
      if (! keysyms [i])
	sym = "NoSymbol";
      else
	sym = XKeysymToString (keysyms [i]);
      if (! sym)
	{
	  sprintf (buf, "0x%04X", keysyms [i]);
	  sym = buf;
	}
      ac = 0;
      XtSetArg (av[ac], XtNlabel, sym); ac++;
      XtSetValues (box->keysym_buttons [i], av, ac);
    }
  if (keysyms) XFree ((char *) keysyms);
  ac = 0;
  XtSetArg (av[ac], XtNlabel, "NoSymbol"); ac++;
  for (; i < 8; i++)
    XtSetValues (box->keysym_buttons [i], av, ac);

  for (i = 0; i < 8; i++)
    {
      ac = 0;
      XtSetArg (av[ac], XtNstate,
		((key->key.modifier_bits & 1<<i) ? True : False)); ac++;
      XtSetValues (box->mod_buttons [i], av, ac);
    }

  XawToggleSetCurrent (box->keysym_buttons [0], (XtPointer) 1);
  keysym_button_cb (box->keysym_buttons [0], box, (void *) 1);
  box->autorepeat = !key->key.auto_repeat_p;
  autorepeat_button_cb (box->autorepeat_widget, box, 0);
}


static struct edit_key_box *
make_edit_key_dbox (widget)
     KeyboardWidget widget;
{
  struct edit_key_box *box = (struct edit_key_box *)
    malloc (sizeof (struct edit_key_box));
  Arg av [20];
  int ac = 0;
  Widget toplevel, box1, box2;
  Widget keysym_box, button_box, keyset_box, keyset_syms_box, mod_box;
  Widget line_box, prev, prev_tog;
  Widget set_list, sym_list;
  Widget set_vp, sym_vp;

  toplevel = XtCreatePopupShell ("editKey", transientShellWidgetClass,
				 (Widget) widget, av, ac);
  box1 = XtCreateManagedWidget ("vertical", panedWidgetClass, toplevel, av,ac);
  box->label = make_label (box1, "label", 0, 0, 0);
  ac = 0;
  XtSetArg (av[ac], XtNorientation, "horizontal"); ac++;
  box2 = XtCreateManagedWidget ("horizontal", panedWidgetClass, box1, av, ac);

  ac = 0;
  keysym_box = XtCreateManagedWidget ("keysymBox", formWidgetClass, box2,
				      av, ac);
  prev = make_label (keysym_box, "symsOfCode", 0, 0, 0);
  prev = make_label (keysym_box, "spacer", "", 0, prev);
  ac = 0;
  prev_tog = 0;
  line_box = prev;
#define TOG(var, name, index) \
   { ac = 0; \
     XtSetArg (av[ac], XtNorientation, "horizontal"); ac++; \
     XtSetArg (av [ac], XtNfromVert, line_box); ac++; \
     XtSetArg (av[ac], XtNtop, XtChainTop); ac++; \
     XtSetArg (av[ac], XtNbottom, XtChainTop); ac++; \
     line_box = XtCreateManagedWidget ("keysymLine", boxWidgetClass, \
				       keysym_box, av, ac); \
     var = make_label (line_box, name, 0, 0, prev); \
     if (index) \
       var = make_toggle (line_box, "keysymValue" ,var, prev, 0, \
			  keysym_button_cb, box, MEDIUM_LENGTH_KEYSYM_NAME, \
			  prev_tog, (XtPointer) index); \
     else \
       var = make_button (line_box, "autoRepeatValue", "Yes", var, prev, \
			  autorepeat_button_cb, box, 0); \
       prev_tog = prev = var; }
  TOG (box->keysym_buttons [0], "keysym1", 1);
  TOG (box->keysym_buttons [1], "keysym2", 2);
  TOG (box->keysym_buttons [2], "keysym3", 3);
  TOG (box->keysym_buttons [3], "keysym4", 4);
  TOG (box->keysym_buttons [4], "keysym5", 5);
  TOG (box->keysym_buttons [5], "keysym6", 6);
  TOG (box->keysym_buttons [6], "keysym7", 7);
  TOG (box->keysym_buttons [7], "keysym8", 8);
  prev = prev_tog = 0;
  line_box = make_label (keysym_box, "spacer2", "", 0, line_box);
  TOG (box->autorepeat_widget, "autoRepeat", 0);
#undef TOG

  ac = 0;
  mod_box = XtCreateManagedWidget ("modifierBox", formWidgetClass,
				   box2, av, ac);
  prev = make_label (mod_box, "modifiers", 0, 0, 0);
  prev = make_label (mod_box, "spacer", "", 0, prev);
#define TOG(var, name) \
   { var = make_toggle (mod_box, name, 0, prev, 0, 0, 0, 0, 0, 0); \
     prev = var; }
  TOG (box->mod_buttons [0], "modShift");
  TOG (box->mod_buttons [1], "modLock");
  TOG (box->mod_buttons [2], "modControl");
  TOG (box->mod_buttons [3], "mod1");
  TOG (box->mod_buttons [4], "mod2");
  TOG (box->mod_buttons [5], "mod3");
  TOG (box->mod_buttons [6], "mod4");
  TOG (box->mod_buttons [7], "mod5");
#undef TOG

  ac = 0;
  keyset_box = XtCreateManagedWidget("keysetBox", formWidgetClass,
				     box2, av, ac);
  prev = make_label (keyset_box, "allKeySets", 0, 0, 0);
  ac = 0;
  XtSetArg (av[ac], XtNfromVert, prev); ac++;
  XtSetArg (av[ac], XtNleft, XtChainLeft); ac++;
  XtSetArg (av[ac], XtNright, XtChainRight); ac++;
  XtSetArg (av[ac], XtNtop, XtChainTop); ac++;
  XtSetArg (av[ac], XtNbottom, XtChainBottom); ac++;
  set_vp = XtCreateManagedWidget ("keysetsVp", viewportWidgetClass,
				  keyset_box, av, ac);
  ac = 0;
  XtSetArg (av[ac], XtNlist, all_keyset_names); ac++;
  set_list = XtCreateManagedWidget ("keysets", listWidgetClass, set_vp,av,ac);

  XtAddCallback (set_list, XtNcallback, keyset_list_cb, (XtPointer) box);

  ac = 0;
  keyset_syms_box = XtCreateManagedWidget ("keysetSymsBox", formWidgetClass,
					   box2, av, ac);
  prev = make_label (keyset_syms_box, "keySymsOfSet", 0, 0, 0);
  ac = 0;
  XtSetArg (av[ac], XtNfromVert, prev); ac++;
  XtSetArg (av[ac], XtNleft, XtChainLeft); ac++;
  XtSetArg (av[ac], XtNright, XtChainRight); ac++;
  XtSetArg (av[ac], XtNtop, XtChainTop); ac++;
  XtSetArg (av[ac], XtNbottom, XtChainBottom); ac++;
  sym_vp = XtCreateManagedWidget ("keysymsVp", viewportWidgetClass,
				  keyset_syms_box, av, ac);
  ensure_keysym_name_buffer (256);
  memcpy (keysym_name_buffer, all_keyset_names, sizeof (all_keyset_names));
  keysym_name_buffer [0] = LONGEST_KEYSYM_NAME;
  ac = 0;
  XtSetArg (av[ac], XtNlist, keysym_name_buffer); ac++;
  sym_list = XtCreateManagedWidget ("keysyms", listWidgetClass, sym_vp,av,ac);
  XtAddCallback (sym_list, XtNcallback, keysym_list_cb, (XtPointer) box);

  ac = 0;
  XtSetArg (av[ac], XtNskipAdjust, True); ac++;
  button_box = XtCreateManagedWidget ("buttons", boxWidgetClass, box1, av, ac);

  prev = make_button (button_box, "undo",  0,    0, 0, undo_button_cb, box, 0);
  prev = make_button (button_box, "abort", 0, prev, 0, abort_button_cb, box,0);
  prev = make_button (button_box, "ok",    0, prev, 0, ok_button_cb, box, 0);

  box->keyboard = widget;
  box->shell = toplevel;
  box->keyset_list = set_list;
  box->keysym_list = sym_list;
  return box;
}

void
pop_up_key_dbox  (parent, client_data, call_data)
     Widget parent;
     XtPointer client_data, call_data;
{
  KeyboardWidget widget = (KeyboardWidget) client_data;
  struct edit_key_box *edit_key_box =
    widget->keyboard.key_menus->edit_key_box;

  if (widget->keyboard.key_under_mouse->key.keycode == 0)
    {
      y_or_n_p (widget, "magicKeyError", "ok", 0, 0);
      return;
    }

  {
    Widget topmost = parent;
    int x, y, w, h;
    XtRealizeWidget (edit_key_box->shell);
    w = edit_key_box->shell->core.width;
    h = edit_key_box->shell->core.height;
    while (topmost->core.parent)
      topmost = topmost->core.parent;
    if (topmost->core.width < w) x = topmost->core.x;
    else x = topmost->core.x + ((topmost->core.width - w) / 2);
    if (topmost->core.height < h) y = topmost->core.y;
    else y = topmost->core.y + ((topmost->core.height - h) / 2);
    XtMoveWidget (edit_key_box->shell, x, y);
  }
  XtPopup (edit_key_box->shell, XtGrabNonexclusive);
  undo_button_cb (edit_key_box->shell, edit_key_box, 0);
}



static struct keyboard_set {
  int kbd_index;
  char *type;
  char **maps;
  char **full_maps;
  int map_count;
} *keyboard_sets;

static int keyboard_set_count;
static char **keyboard_types;
static char **keymap_types;
static char *longest_type;

struct select_kbd_box {
  KeyboardWidget keyboard;
  Widget shell;
  Widget label;
  Widget label2;
  Widget icon;
  Widget keyboard_list, keymap_list;
  struct keyboard_set *set;
  char *map;
};


static char *default_maps[] = { "default", 0 };

static void
make_kbd_lists ()
{
  int kbd_count = 0;
  int i, j;
  char last [255];

  for (kbd_count = 0; all_kbds [kbd_count].short_name; kbd_count++)
    ;
  keyboard_sets = (struct keyboard_set *)
    malloc (sizeof (struct keyboard_set) * kbd_count);
  longest_type = 0;
  
  i = 0;
  j = 0;
  while (i < kbd_count)
    {
      char *tail;
      strcpy (last, all_kbds [i].long_name);
      tail = strchr (last, '(');
      keyboard_sets [j].kbd_index = i;
      keyboard_sets [j].map_count = 0;
      if (tail)
	{
	  int L;
	  int count = 0;
	  int k;

	  /* Edit out the parens (but not the trailing space) */
	  *tail = 0;
	  tail++;
	  tail [strlen (tail) - 1] = 0;

	  L = strlen (last);
	  while (strlen (all_kbds [i+count].long_name) >= L
		 && !strncmp (last, all_kbds [i+count].long_name, L))
	    count++;
	  keyboard_sets [j].maps = (char **)
	    malloc (sizeof (char *) * (count+1));
	  keyboard_sets [j].full_maps = (char **)
	    malloc (sizeof (char *) * (count+1));
	  for (k = 0; k < count; k++)
	    {
	      char *full = all_kbds [i+k].long_name;
	      char *new = full + L + 1;
	      int ML = strlen (new);
	      char *map_name = (char *) malloc (ML);
	      memcpy (map_name, new, ML);
	      map_name [--ML] = 0;
	      keyboard_sets [j].maps[k] = map_name;
	      keyboard_sets [j].full_maps[k] = full;
	      if (!longest_type || strlen (longest_type) < ML)
		longest_type = keyboard_sets [j].maps[k];
	    }
	  keyboard_sets [j].maps[k] = 0;
	  i += count;
	  keyboard_sets [j].map_count = count;
	}
      else
	{
	  keyboard_sets [j].maps = default_maps;
	  keyboard_sets [j].full_maps = (char **) malloc (sizeof (char *) * 2);
	  keyboard_sets [j].full_maps [0] = all_kbds [i].long_name;
	  keyboard_sets [j].full_maps [1] = 0;
	  keyboard_sets [j].map_count = 1;
	  i++;
	}
      keyboard_sets [j].type = (char *) malloc (strlen (last) + 1);
      memcpy (keyboard_sets [j].type, last, strlen (last) + 1);
      j++;
    }
  keyboard_set_count = j;

  {
    int max = 0;
    keyboard_types = (char **) malloc (sizeof (char *) * (j + 1));
    for (i = 0; i < j; i++)
      {
	keyboard_types [i] = keyboard_sets [i].type;
	if (keyboard_sets [i].map_count > max)
	  max = keyboard_sets [i].map_count;
      }
    keyboard_types[i] = 0;
    keymap_types = (char **) calloc (sizeof (char *), max + 1);
  }
}

static void
kbd_abort_button_cb (widget, client_data, call_data)
     Widget widget;
     XtPointer client_data, call_data;
{
  struct select_kbd_box *box = (struct select_kbd_box *) client_data;
  Widget shell = box->shell;
  XtPopdown (shell);
}

static void
kbd_ok_button_cb (button, client_data, call_data)
     Widget button;
     XtPointer client_data, call_data;
{
  struct select_kbd_box *box = (struct select_kbd_box *) client_data;
  KeyboardWidget kbd = box->keyboard;
  char *map = box->map;
  kbd_abort_button_cb (button, client_data, call_data);
  replace_keyboard (kbd, map);
}

static void
keymap_list_cb (widget, client_data, call_data)
     Widget widget;
     XtPointer client_data, call_data;
{
  XawListReturnStruct *lr = (XawListReturnStruct *) call_data;
  struct select_kbd_box *box = (struct select_kbd_box *) client_data;
  struct keyboard_set *set = box->set;
  box->map = set->full_maps [lr->list_index];
}

static void
keyboard_list_cb (widget, client_data, call_data)
     Widget widget;
     XtPointer client_data, call_data;
{
  XawListReturnStruct *lr = (XawListReturnStruct *) call_data;
  struct select_kbd_box *box = (struct select_kbd_box *) client_data;
  int kbd = lr->list_index;
  int map = 0;
  int i = 0;
  while (strcmp (keyboard_sets [i].type, keyboard_types [kbd]))
    i++;
  box->set = &keyboard_sets [i];
  XawListChange (box->keymap_list, box->set->maps, 0, 0, True);
  for (i = 0; i < box->set->map_count; i++)
    if (!strcmp (box->set->full_maps [i], box->keyboard->keyboard.long_name))
      {
	map = i;
	break;
      }
  XawListHighlight (box->keymap_list, map);
  lr->list_index = map;
  keymap_list_cb (widget, client_data, call_data);
  move_scrollbar (box->keymap_list,
		  (((float) map) / ((float) box->set->map_count)));
  {
    int w, h;
    Pixmap p = get_keyboard_icon (XtDisplay (widget),
				  &all_kbds [box->set->kbd_index], &w, &h);
    XtVaSetValues (box->icon, XtNbitmap, p, XtNheight, h, 0);
  }
}


static struct select_kbd_box *
make_select_kbd_dbox (widget)
     KeyboardWidget widget;
{
  struct select_kbd_box *box = (struct select_kbd_box *)
    malloc (sizeof (struct select_kbd_box));
  Arg av [20];
  int ac = 0;
  Widget toplevel, box1, box2;
  Widget button_box, keyboard_box, keymap_box;
  Widget line_box, prev, prev_tog;
  Widget kbd_list, map_list;
  Widget kbd_vp, map_vp;

  make_kbd_lists ();

  toplevel = XtCreatePopupShell ("selectKbd", transientShellWidgetClass,
				 XtParent ((Widget) widget), av, ac);
  box1 = XtCreateManagedWidget ("vertical", panedWidgetClass, toplevel, av,ac);
  box->label = make_label (box1, "label", 0, 0, 0);
  box->label2 = make_label (box1, "label2", "", 0, 0);
  ac = 0;
  XtSetArg (av[ac], XtNorientation, "horizontal"); ac++;
  box2 = XtCreateManagedWidget ("horizontal", panedWidgetClass, box1, av, ac);

  ac = 0;
  keyboard_box = XtCreateManagedWidget ("keyboardBox", formWidgetClass,
					box2, av, ac);
  prev = make_label (keyboard_box, "keyboards", 0, 0, 0);
  ac = 0;
  XtSetArg (av[ac], XtNfromVert, prev); ac++;
  XtSetArg (av[ac], XtNleft, XtChainLeft); ac++;
  XtSetArg (av[ac], XtNright, XtChainRight); ac++;
  XtSetArg (av[ac], XtNtop, XtChainTop); ac++;
  XtSetArg (av[ac], XtNbottom, XtChainBottom); ac++;
  kbd_vp = XtCreateManagedWidget ("keyboardsVp", viewportWidgetClass,
				  keyboard_box, av, ac);
  ac = 0;
  XtSetArg (av[ac], XtNlist, keyboard_types); ac++;
  kbd_list = XtCreateManagedWidget ("keyboardsList", listWidgetClass, kbd_vp,
				    av, ac);

  XtAddCallback (kbd_list, XtNcallback, keyboard_list_cb, (XtPointer) box);

  ac = 0;
  keymap_box = XtCreateManagedWidget ("keymapBox", formWidgetClass,
				      box2, av, ac);
  prev = make_label (keymap_box, "keymaps", 0, 0, 0);
  ac = 0;
  XtSetArg (av[ac], XtNfromVert, prev); ac++;
  XtSetArg (av[ac], XtNleft, XtChainLeft); ac++;
  XtSetArg (av[ac], XtNright, XtChainRight); ac++;
  XtSetArg (av[ac], XtNtop, XtChainTop); ac++;
  XtSetArg (av[ac], XtNbottom, XtChainBottom); ac++;
  map_vp = XtCreateManagedWidget ("keymapsVp", viewportWidgetClass,
				  keymap_box, av, ac);
  keymap_types [0] = longest_type;
  keymap_types [1] = 0;
  ac = 0;
  XtSetArg (av[ac], XtNlist, keymap_types); ac++;
  map_list = XtCreateManagedWidget ("keymapsList", listWidgetClass, map_vp,
				    av, ac);
  XtAddCallback (map_list, XtNcallback, keymap_list_cb, (XtPointer) box);

  ac = 0;
  XtSetArg (av[ac], XtNskipAdjust, True); ac++;
  button_box = XtCreateManagedWidget ("buttons", boxWidgetClass, box1, av, ac);

  prev = make_button (button_box, "abort", 0, prev, 0, kbd_abort_button_cb,
		      box, 0);
  prev = make_button (button_box, "ok",    0, prev, 0, kbd_ok_button_cb,
		      box, 0);
  prev = make_label (button_box, "spacer", "", 0, prev);
  box->icon = make_label (button_box, "kbd_icon", 0, prev, 0);

  box->keyboard = 0;
  box->shell = toplevel;
  box->keyboard_list = kbd_list;
  box->keymap_list = map_list;
  box->set = 0;
  box->map = 0;
  return box;
}

extern char choose_kbd_message[];
extern int choose_kbd_dubious_p;

void
pop_up_kbd_dbox (parent, client_data, call_data)
     Widget parent;
     XtPointer client_data, call_data;
{
  KeyboardWidget widget = *((KeyboardWidget *) client_data);
  struct select_kbd_box *select_kbd_box =
    widget->keyboard.key_menus->select_kbd_box;

  select_kbd_box->keyboard = widget;
  XtVaSetValues (select_kbd_box->label2, XtNlabel, choose_kbd_message, 0);
  choose_kbd_dubious_p = 0;
  choose_kbd_message [0] = 0;

  {
    Widget topmost = parent;
    int x, y, w, h;
    XtRealizeWidget (select_kbd_box->shell);
    /* #### It'd be nice to wait here until it was mapped... */
    w = select_kbd_box->shell->core.width;
    h = select_kbd_box->shell->core.height;
    while (topmost->core.parent)
      topmost = topmost->core.parent;
    if (topmost->core.width < w) x = topmost->core.x;
    else x = topmost->core.x + ((topmost->core.width - w) / 2);
    if (topmost->core.height < h) y = topmost->core.y;
    else y = topmost->core.y + ((topmost->core.height - h) / 2);
    XtMoveWidget (select_kbd_box->shell, x, y);
  }

  {
    XawListReturnStruct lr;
    int this_one = 0;
    int i, j;
    for (i = 0; i <= keyboard_set_count; i++)
      {
	for (j = 0; j < keyboard_sets[i].map_count; j++)
	  if (!strcmp (keyboard_sets[i].full_maps[j],
		       widget->keyboard.long_name))
	    {
	      this_one = i;
	      goto out;
	    }
      }
  out:
    lr.list_index = this_one;
    XawListHighlight (select_kbd_box->keyboard_list, this_one);
    move_scrollbar (select_kbd_box->keyboard_list,
		    (((float) this_one) / ((float) keyboard_set_count)));
    keyboard_list_cb ((Widget) widget,
		      (void *) select_kbd_box,
		      (void *) &lr);
  }
  XtPopup (select_kbd_box->shell, XtGrabNonexclusive);
}



static void
yorn_button_cb (widget, client_data, call_data)
     Widget widget;
     XtPointer client_data, call_data;
{
  *((int *) client_data) = 1;
}


static int
y_or_n_p_with_args (widget, name,
		    name0, name1, name2,
		    arg0, arg1, arg2, arg3, arg4, arg5)
     KeyboardWidget widget;
     char *name;
     char *name0, *name1, *name2;
     char *arg0, *arg1, *arg2, *arg3, *arg4, *arg5;
{
  XtAppContext app = XtWidgetToApplicationContext ((Widget) widget);
  Widget topmost = (Widget) widget;
  Widget label;
  XEvent event;
  int x, y, w, h;
  Arg av [10];
  int ac = 0;
  Widget shell, box1, box2, prev;
  int ans [3];

  shell = XtCreatePopupShell (name, transientShellWidgetClass,
			      (Widget) widget, av, ac);
  box1 = XtCreateManagedWidget ("paned", panedWidgetClass, shell, av, ac);
  label = make_label (box1, "label", 0, 0, 0);
  if (arg0)  /* format into the label if desired. */
    {
      char *old_string;
      char new_string [4000];
      ac = 0;
      XtSetArg (av [ac], XtNlabel, &old_string); ac++;
      XtGetValues (label, av, ac);
      if (!strcmp (old_string, XtName (label)))
	strcpy (new_string, "ERROR: RESOURCES ARE NOT INSTALLED CORRECTLY");
      else
	sprintf (new_string, old_string, arg0, arg1, arg2, arg3, arg4, arg5);
      ac = 0;
      XtSetArg (av [ac], XtNlabel, new_string); ac++;
      XtSetValues (label, av, ac);
    }
  ac = 0;
  XtSetArg (av[ac], XtNorientation, "horizontal"); ac++;
  box2 = XtCreateManagedWidget ("buttons", boxWidgetClass, box1, av, ac);
  prev = 0;
  if (name0) prev = make_button (box2,name0,0,prev,0,yorn_button_cb,&ans[0],0);
  if (name1) prev = make_button (box2,name1,0,prev,0,yorn_button_cb,&ans[1],0);
  if (name2) prev = make_button (box2,name2,0,prev,0,yorn_button_cb,&ans[2],0);
  XtRealizeWidget (shell);
  w = shell->core.width;
  h = shell->core.height;
  while (topmost->core.parent)
    topmost = topmost->core.parent;
  if (topmost->core.width < w) x = topmost->core.x;
  else x = topmost->core.x + ((topmost->core.width - w) / 2);
  if (topmost->core.height < h) y = topmost->core.y;
  else y = topmost->core.y + ((topmost->core.height - h) / 2);
  XtMoveWidget (shell, x, y);

  XtPopup (shell, XtGrabNonexclusive);
  memset (ans, 0, sizeof (ans));
  while (! (ans[0] || ans[1] || ans[2]))
    {
      XtAppNextEvent (app, &event);
      if (event.xany.type == KeymapNotify)
	keyboard_handle_keymap_notify ((Widget) widget, 0, &event, 0);
      else if (event.xany.type == MappingNotify)
	keyboard_handle_mapping_notify ((Widget) widget, 0, &event, 0);
      XtDispatchEvent (&event);
    }
  XtPopdown (shell);
  XtDestroyWidget (shell);
  if (ans[0]) return 0;
  if (ans[1]) return 1;
  if (ans[2]) return 2;
  exit (-69);
}

int
y_or_n_p (widget, name, name0, name1, name2)
     KeyboardWidget widget;
     char *name, *name0, *name1, *name2;
{
  return y_or_n_p_with_args (widget, name, name0, name1, name2, 0,0,0,0,0,0);
}


KeyWidget
prompt_for_key (keyboard, msg)
     KeyboardWidget keyboard;
     char *msg;
{
  XtAppContext app = XtWidgetToApplicationContext ((Widget) keyboard);
  KeyWidget key;
  XEvent event;
  message (keyboard, msg);

  if (XGrabPointer (XtDisplay (keyboard), XtWindow (keyboard), True,
		    ButtonPressMask | ButtonReleaseMask,
		    GrabModeAsync, GrabModeAsync, 0,
		    keyboard->keyboard.select_cursor,
		    CurrentTime))
    {
      XBell (XtDisplay (keyboard), 0);
      message (keyboard, "Grab failed.");
      return 0;
    }
  
  while (1)
    {
      XtAppNextEvent (app, &event);

      if (event.xany.type == ButtonPress)
	{
	  XUngrabPointer (XtDisplay (keyboard), CurrentTime);
	  if (key = keyboard->keyboard.key_under_mouse)
	    return key;
	  XBell (XtDisplay (keyboard), 0);
	  message (keyboard, "You must click on a key.");
	  return 0;
	}
      else if (event.xany.type == ButtonRelease ||
	       event.xany.type == KeyPress ||
	       event.xany.type == KeyRelease)
	{
	  XUngrabPointer (XtDisplay (keyboard), CurrentTime);
	  XBell (XtDisplay (keyboard), 0);
	  XPutBackEvent (XtDisplay (keyboard), &event);
	  message (keyboard, "Aborted.");
	  return 0;
	}
      else
	{
	  if (event.xany.type == KeymapNotify)
	    keyboard_handle_keymap_notify ((Widget) keyboard, 0, &event, 0);
	  else if (event.xany.type == MappingNotify)
	    keyboard_handle_mapping_notify ((Widget) keyboard, 0, &event, 0);
	  XtDispatchEvent (&event);
	}
    }
}


/* What a mess the X keyboard model is... */

#include <X11/keysym.h>

static char *
index_to_name (index)
     int index;
{
  return (index == ShiftMapIndex ? "Shift" :
	  index == LockMapIndex ? "Lock" :
	  index == ControlMapIndex ? "Control" :
	  index == Mod1MapIndex ? "1" :
	  index == Mod2MapIndex ? "2" :
	  index == Mod3MapIndex ? "3" :
	  index == Mod4MapIndex ? "4" :
	  index == Mod5MapIndex ? "5" :
	  "???");
}

static char *
keysym_to_name (keysym)
     KeySym keysym;
{
  char *n = XKeysymToString (keysym);
  if (! n) /* ought not happen, but does if XKeysymDB isn't installed right */
    {
      n = (char *) malloc (20); /* leaks! */
      sprintf (n, "Unknown-KeySym-0x%X", (int) keysym);
    }
  return n;
}

static int 
stop_the_insanity (widget, keycode, keysyms, count, modbits, modmap)
     KeyboardWidget widget;
     int keycode;
     KeySym *keysyms;
     int count;
     unsigned long modbits;
     XModifierKeymap *modmap;
{
  int i;
  KeySym type = 0;		/* The canonical modifier keysym on this key */
  KeySym basic_keysym = 0;	/* The non-canonical version */
  KeySym companion = 0;		/* The "other" version of this key */

  /* Check that the keysyms that are attached to this key are compatible:
     It is incorrect to put Control_[LR] and Meta_[LR] on the same key.
   */
  {
    KeySym conflict = 0;
    KeySym nonmod = 0;
    for (i = 0; i < count && !conflict; i++)
      switch (keysyms [i])
	{
#define check(k)				\
	case k:					\
	  if (type && type != k)		\
	    conflict = keysyms [i];		\
	  else					\
	    type = k,				\
	    basic_keysym = keysyms[i];		\
	  break
	case XK_Meta_R:    check (XK_Meta_L);
	case XK_Super_R:   check (XK_Super_L);
	case XK_Hyper_R:   check (XK_Hyper_L);
	case XK_Alt_R:     check (XK_Alt_L);
	case XK_Control_R: check (XK_Control_L);
	case XK_Shift_R:   check (XK_Shift_L);
	check (XK_Shift_Lock);
	check (XK_Caps_Lock);
	check (XK_Mode_switch);
#undef check
	default:
	  nonmod = keysyms [i];
	  break;
	}

    switch (basic_keysym)
      {
      case XK_Meta_L:    companion = XK_Meta_R;    break;
      case XK_Super_L:   companion = XK_Super_R;   break;
      case XK_Hyper_L:   companion = XK_Hyper_R;   break;
      case XK_Alt_L:     companion = XK_Alt_R;     break;
      case XK_Control_L: companion = XK_Control_R; break;
      case XK_Shift_L:   companion = XK_Shift_R;   break;
      case XK_Meta_R:    companion = XK_Meta_L;    break;
      case XK_Super_R:   companion = XK_Super_L;   break;
      case XK_Hyper_R:   companion = XK_Hyper_L;   break;
      case XK_Alt_R:     companion = XK_Alt_L;     break;
      case XK_Control_R: companion = XK_Control_L; break;
      case XK_Shift_R:   companion = XK_Shift_L;   break;
      }

    if (nonmod && type)	/* all modifier syms and non-mod syms conflict. */
      {
	conflict = type;
	type = nonmod;
      }
    if (conflict)
      return (y_or_n_p_with_args (widget, "keysymConflictWarning",
				  "ok", "cancel", 0,
				  keysym_to_name (type),
				  keysym_to_name (conflict),
				  0, 0, 0, 0));
  }


  /* Check that the modifier bits are compatible with the keysyms:
     It is incorrect to put ModControl on a key with any keysym other
     than Control_L or Control_R.
   */
  {
    int losing = 0;
    int bad_index = 0;
    if (modbits & ControlMask && type != XK_Control_L)
      losing = 1, bad_index = ControlMapIndex;
    else if (modbits & ShiftMask && type != XK_Shift_L)
      losing = 1, bad_index = ShiftMapIndex;
    else if (modbits & LockMask && type != XK_Caps_Lock &&
				   type != XK_Shift_Lock)
      losing = 1, bad_index = LockMapIndex;

    if (losing)
      return (y_or_n_p_with_args (widget, "badModifiersWarning",
				  "ok", "cancel", 0,
				  index_to_name (bad_index),
				  (bad_index == ShiftMapIndex ? "Shift_L" :
				   bad_index == LockMapIndex ? "Caps_Lock" :
				   bad_index == ControlMapIndex ? "Control_L" :
				   "<<INTERNAL ERROR>>"),
				  (bad_index == ShiftMapIndex ? "Shift_R" :
				   bad_index == LockMapIndex ? "Shift_Lock" :
				   bad_index == ControlMapIndex ? "Control_R" :
				   "<<INTERNAL ERROR>>"),
				  index_to_name (bad_index),
				  0, 0));
  }

  /* Do the opposite check - the previous code checked for the case of
     attaching ModControl to Meta_L, but this checks for attaching Mod1
     to Control_L. */
  if ((type == XK_Shift_L || type == XK_Control_L ||
       type == XK_Shift_Lock || type == XK_Caps_Lock) &&
      (modbits & (~ (ShiftMask | LockMask | ControlMask))))
    return (y_or_n_p_with_args (widget, "badModifiersWarning2",
				"ok", "cancel", 0,
				keysym_to_name (basic_keysym),
				(type == XK_Shift_L ? "Shift" :
				 type == XK_Control_L ? "Control" :
				 type == XK_Shift_Lock ? "Lock" :
				 type == XK_Caps_Lock ? "Lock" :
				 "<<INTERNAL ERROR>>"),
				(type == XK_Shift_L ? "Shift" :
				 type == XK_Control_L ? "Control" :
				 type == XK_Shift_Lock ? "Shift_Lock" :
				 type == XK_Caps_Lock ? "Caps_Lock" :
				 "<<INTERNAL ERROR>>"),
				0, 0, 0));

  /* Check that the key is generating at most one modifier bit.
     The behavior of more than one modifier bit is undefined.
     If this is a modifier keysym, warn if it has no modifier bits
     at all (which has defined behavior, and may be what is desired,
     but which novices are likely to do by accident.)
   */
  {
    int bitcount = 0;
    int i;
    for (i = 0; i < 8; i++)
      if (modbits & (1<<i)) bitcount++;
    if (bitcount > 1)
      return (y_or_n_p (widget, "multipleModifiersWarning",
			"ok", "cancel", 0));
    else if (bitcount == 0 && type != 0)
      return (y_or_n_p_with_args (widget, "noModifiersWarning",
				  "ok", "cancel", 0,
				  keysym_to_name (type), 0, 0, 0, 0, 0));
  }


  /* Check that some other (incompatible) key is not generating the same
     modifiers as this key. */
  {
    Display *dpy = XtDisplay ((Widget) widget);

    unsigned long conflict_code = 0;
    KeySym conflict_sym;
    int conflict_bit;
    int this_bit;

    int modifier_index, modifier_key;
    int mkpm = modmap->max_keypermod;

    for (modifier_index = 0; modifier_index < 8; modifier_index++)
      /* Ignore modifier bits which this key does not generate. */
      if ((1 << modifier_index) & modbits)
	for (modifier_key = 0; modifier_key < mkpm; modifier_key++)
	  {
	    int column;
	    KeySym last_sym = 0;
	    for (column = 0; column < 4; column += 2)
	      {
		KeyCode code2 = modmap->modifiermap [modifier_index * mkpm
						     + modifier_key];
		KeySym sym = (code2
			      ? XKeycodeToKeysym (dpy, code2, column)
			      : 0);
		KeySym this = 0;
		if (sym == last_sym) continue;
		last_sym = sym;
		switch (sym)
		  {
		  case XK_Meta_L:  case XK_Meta_R: this = XK_Meta_L; break;
		  case XK_Super_L: case XK_Super_R:this = XK_Super_R; break;
		  case XK_Hyper_L: case XK_Hyper_R:this = XK_Hyper_L; break;
		  case XK_Alt_L:   case XK_Alt_R:  this = XK_Alt_L; break;
		  case XK_Mode_switch: 		   this = XK_Mode_switch;break;
		  case XK_Shift_Lock:		   this = XK_Shift_Lock; break;
		  case XK_Caps_Lock:		   this = XK_Caps_Lock; break;
		  default: this = sym;
		  }

		if (this != type &&		/* conflicting keysyms */
		    /* but ignore conflicts with the previous version of
		       this very key! */
		    code2 != keycode)
		  {
		    conflict_code = code2;
		    conflict_sym = sym;
		    conflict_bit = modifier_index;
		    goto DONE;
		  }
	      }
	  }
  DONE:
    if (conflict_code)
      {
	KeyWidget kw = keycode_to_key (widget, conflict_code);
	return (y_or_n_p_with_args (widget, "modifiersConflictWarning",
				    "ok", "cancel", 0,
				    keysym_to_name (basic_keysym),
				    index_to_name (conflict_bit),
				    conflict_code,
				    (kw ? kw->key.key_name : "???"),
				    keysym_to_name (conflict_sym),
				    0));
      }


    /* Check that the semantically equivalent keys do not generate
       different modifier bits than this one. */

    conflict_sym = 0;
    conflict_bit = 0;
    conflict_code = 0;

    for (modifier_index = 0; modifier_index < 8; modifier_index++)
      /* Ignore modifier bits which this key generates. */
      if ((1 << modifier_index) & modbits)
	this_bit = modifier_index;
      else if (conflict_code)
	;
      else
	for (modifier_key = 0; modifier_key < mkpm; modifier_key++)
	  {
	    int column;
	    KeySym last_sym = 0;
	    for (column = 0; column < 4; column += 2)
	      {
		KeyCode code2 = modmap->modifiermap [modifier_index * mkpm
						     + modifier_key];
		KeySym sym = (code2
			      ? XKeycodeToKeysym (dpy, code2, column)
			      : 0);
		KeySym this = 0;
		if (sym &&
		    !conflict_code &&
		    (sym == basic_keysym || sym == companion) &&
		    code2 != keycode)
		  {
		    conflict_code = code2;
		    conflict_sym = sym;
		    conflict_bit = modifier_index;
		    /* keep going to find this_bit */
		    break;
		  }
	      }
	  }

    if (conflict_code)
      {
	KeyWidget kw = keycode_to_key (widget, conflict_code);
	return (y_or_n_p_with_args (widget, "modifiersConflictWarning2",
				    "ok", "cancel", 0,
				    keysym_to_name (basic_keysym),
				    index_to_name (this_bit),
				    conflict_code,
				    (kw ? kw->key.key_name : "???"),
				    keysym_to_name (conflict_sym),
				    index_to_name (conflict_bit)
				    ));
      }
  }

  /* Should we also issue a message along the lines of

       This key is not normally a `chording' key (that is, a key that is
       held down to modify some other keypress.)  It is unlikely that
       attaching a modifier to it will do anything sensible.  If you want
       this to be a {Control,Shift,Lock,Meta} key, change its keysym to
       ***_L or ***_R.

     That wouldn't be appropriate for the default layout of the OpenWound
     keyboard, which attaches modifiers to random function keys (I imagine
     this has something to do with drag-and-drop, but I don't know how it
     works.)

     Actually maybe a different message would be appropriate for Multi_key,
     since people sometimes misinterpret the description of that key as being
     a modifier key, when in fact it's a prefix key and shouldn't have any
     modifier bits on it.

   */

  return 0;
}
