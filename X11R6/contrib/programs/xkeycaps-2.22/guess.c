/* xkeycaps, Copyright (c) 1991, 1992, 1993, 1994
 * Jamie Zawinski <jwz@lucid.com>
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
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include "KbdWidgetP.h"
#include "KeyWidgetP.h"

#include "kbddef.h"

/* Strict versus relaxed ANSI modes tend to be random about these... */
#if defined (__unix) && !defined(unix)
# define unix
#endif
#if defined (__hpux) && !defined(hpux)
# define hpux
#endif
#if defined (__sun) && !defined(sun)
# define sun
#endif

#ifdef unix
# include <netdb.h>
static int display_is_on_console_p ();
#endif

#if defined(sun) || defined(hpux)
extern char *xkeycaps_guess_local_keyboard_type ();
#endif

static char *guess_tek_keyboard_type ();
static char *tek_kbd_type, *tek_kbd_layout, *tek_kbd_nationality;

static void
print_kbd_choices ()
{
  int i = 0;
  while (all_kbds [i].short_name)
    {
      fprintf (stderr, "    %s%s\t- %s\n",
	       all_kbds [i].short_name,
	       (strlen (all_kbds [i].short_name) < 4 ? "\t" : ""),
	       all_kbds [i].long_name);
      i++;
    }
}

/* Make sure nobody messed up the data in all-kbds.h... */
static void
check_kbds ()
{
  int i = 0;
  while (all_kbds [i].short_name)
    {
      int j = i + 1;
      while (all_kbds [j].short_name)
	{
	  char *loser1 = 0, *loser2 = 0;
	  if (!strcmp (all_kbds [i].short_name, all_kbds [j].short_name))
	    loser1 = "short names", loser2 = all_kbds [j].short_name;
	  else if (!strcmp (all_kbds [i].long_name, all_kbds [j].long_name))
	    loser1 = "long names", loser2 = all_kbds [j].long_name;
	  else if (all_kbds [i].vendor_id && all_kbds [j].vendor_id &&
		   !strcmp (all_kbds [i].vendor_id, all_kbds [j].vendor_id))
	    loser1 = "vendor ids", loser2 = all_kbds [j].vendor_id;
	  j++;

	  if (loser1)
	    fprintf (stderr,
		     "%s: DATA ERROR: duplicate %s in all-kbds.h for \"%s\"\n",
		     progname, loser1, loser2);
	}
      i++;
    }
}


#define strprefix(target,prefix)				\
      ((strlen ((target)) >= strlen ((prefix))) &&		\
       !(strncmp ((target), (prefix), strlen ((prefix)))))


char choose_kbd_message [2048];
int choose_kbd_dubious_p;

struct keyboard_instance *
choose_kbd (dpy, kbd_name)
     Display *dpy;
     char *kbd_name;
{
  char *vendor = XServerVendor (dpy);
  tek_kbd_type = 0;
  tek_kbd_layout = 0;
  tek_kbd_nationality = 0;

  choose_kbd_message [0] = 0;
  choose_kbd_dubious_p = 0;

  check_kbds ();

#ifdef sun
  if ((!kbd_name || !*kbd_name) &&
      display_is_on_console_p (dpy) &&
      (kbd_name = xkeycaps_guess_local_keyboard_type ()))
    {
      /* evil hack - append "OW" to the default keyboard name if
	 running OpenWindows, since the default keymaps are different.
	 Likewise for R6, since the default keymaps AND keycodes have
	 changed there!  Arrrrgh!
       */
      char *suffix = 0;
      if (!strcmp (vendor, "X11/NeWS - Sun Microsystems Inc."))
	suffix = "OW";
      else if (!strcmp (vendor, "X Consortium"))
	/* In R6, the vendor id is "X Consortium".
	   In R4 and R5, it was "MIT X Consortium". */
	suffix = "R6";

      if (suffix)
	{
	  int L = strlen (kbd_name);
	  char *s = (char *) malloc (L + 4);
	  strncpy (s, kbd_name, L);
	  if (L > 6) strcat (s + L, "-");
	  strcat (s + L, suffix);
	  kbd_name = s;
	}
      sprintf (choose_kbd_message,
	       "\nkeyboard hardware claims to be \"%s\".\n",
	       /*progname,*/ kbd_name);
    }
  else
#endif /* sun */

#ifdef hpux
  if ((!kbd_name || !*kbd_name) &&
      display_is_on_console_p (dpy) &&
      (kbd_name = xkeycaps_guess_local_keyboard_type ()))
    {
      /* we're done */ ;
    }
  else
#endif /* hpux */

  if (!kbd_name || !*kbd_name)
    {
      kbd_name = 0;

      if (strprefix (vendor, "Tektronix, Inc"))
	/* Tek cleverly tells us what the keyboard is via X properties. */
	kbd_name = guess_tek_keyboard_type (dpy);

      else
	{
	  /* Search all_kbds for an instance that matches the vendor id. */
	  int i = 0;
	  struct keyboard_instance *kbd;

	  while ((kbd = &all_kbds [i++]), kbd->short_name)
	    if (kbd->vendor_id && strprefix (vendor, kbd->vendor_id))
	      break;

	  kbd_name = kbd->short_name;
	}

      if (kbd_name)
	{
	  char *out = choose_kbd_message;
	  choose_kbd_dubious_p = 1;
	  sprintf (out,
 "\nA keyboard type was not specified.  Based on the vendor\n\
 identification string of the display \"%s\", which is\n \"%s\"\n",
		   /* progname, */ DisplayString (dpy), vendor);
	  out += strlen (out);
	  if (tek_kbd_type && tek_kbd_layout && tek_kbd_nationality)
	    sprintf (out,
 " and the root window's _TEK_KEYBOARD_TYPE, _TEK_KEYBOARD_LAYOUT, and\n\
 _TEK_KEYBOARD_NATIONALITY properties, which are %s, %s, and %s,\n",
		     tek_kbd_type, tek_kbd_layout, tek_kbd_nationality);
	  else if (tek_kbd_type && tek_kbd_layout)
	    sprintf (out, " and the root window's _TEK_KEYBOARD_TYPE\
 and _TEK_KEYBOARD_LAYOUT\n properties, which are \"%s\" and \"%s\",\n",
		     tek_kbd_type, tek_kbd_layout);
	  else if (tek_kbd_type)
	    sprintf (out, " and the root window's _TEK_KEYBOARD_TYPE\
 property, which is \"%s\",\n",
		     tek_kbd_type);
	  out += strlen (out);
	  sprintf (out,
	    " we will assume that you are using a keyboard of type \"%s\".",
		   kbd_name);
	}
#ifdef DEFAULT_KBD_NAME
      else
	{
	  kbd_name = DEFAULT_KBD_NAME;
	  if (kbd_name)
	    {
	      choose_kbd_dubious_p = 1;
	      sprintf (choose_kbd_message,
 "\nA keyboard type was not specified, and the vendor ID string,\n\
 \"%s\"\n\
 is not recognised.  We will guess that you are using a keyboard of\n\
 type \"%s\".",
		       /* progname, */ vendor, kbd_name);
	    }
	}
#endif
    }

  {
    int i = 0;
    struct keyboard_instance *kbd;

    while ((kbd = &all_kbds [i++]), kbd->short_name)
      if (string_equal (kbd_name, kbd->short_name) ||
	  string_equal (kbd_name, kbd->long_name))
	return kbd;

    choose_kbd_dubious_p = 1;

    if (!kbd_name || !strcmp (kbd_name, "help"))
      fprintf (stderr,
	   "%s: please specify -keyboard with one of the following names:\n\n",
	       progname);
    else
      fprintf (stderr, "%s: unknown keyboard type \"%s\".\n\
Please specify the -keyboard option with one of the following names:\n\n",
	       progname, kbd_name);

    print_kbd_choices ();
    exit (-1);
  }
}

#ifdef unix

static int
display_is_on_console_p (display)
     Display *display;
{
  int not_on_console = 1;
  char *dpy = DisplayString (display);
  char *tail = (char *) strchr (dpy, ':');
  if (! tail || strncmp (tail, ":0", 2))
    not_on_console = 1;
  else
    {
      char dpyname[255], localname[255];
      strncpy (dpyname, dpy, tail-dpy);
      dpyname [tail-dpy] = 0;
      if (!*dpyname ||
	  !strcmp(dpyname, "unix") ||
	  !strcmp(dpyname, "localhost"))
	not_on_console = 0;
      else if (gethostname (localname, sizeof (localname)))
	not_on_console = 1;  /* can't find hostname? */
      else
	{
	  /* We have to call gethostbyname() on the result of gethostname()
	     because the two aren't guarenteed to be the same name for the
	     same host: on some losing systems, one is a FQDN and the other
	     is not.  Here in the wide wonderful world of Unix it's rocket
	     science to obtain the local hostname in a portable fashion.
	     
	     And don't forget, gethostbyname() reuses the structure it
	     returns, so we have to copy the fucker before calling it again.
	     Thank you master, may I have another.
	   */
	  struct hostent *h = gethostbyname (dpyname);
	  if (!h)
	    not_on_console = 1;
	  else
	    {
	      char hn [255];
	      struct hostent *l;
	      strcpy (hn, h->h_name);
	      l = gethostbyname (localname);
	      not_on_console = (!l || !!(strcmp (l->h_name, hn)));
	    }
	}
    }
  return !not_on_console;
}

#endif

static char *
get_prop_string (dpy, prop)
    Display *dpy;
     char *prop;
{
  Atom prop_atom;
  unsigned char *value;
  Boolean status;
  Atom actual_type;
  int actual_format;
  unsigned long nitems, bytes_after;

  prop_atom = XInternAtom (dpy, prop, True);
  if (prop_atom == None)
    return 0;
  status = XGetWindowProperty (dpy, DefaultRootWindow (dpy), prop_atom,
			       0L, 6L, False, XA_STRING, &actual_type,
			       &actual_format, &nitems, &bytes_after,
			       &value);
  if (status != Success || value == 0 || actual_type != XA_STRING ||
      actual_format != 8 || bytes_after != 0)
    return 0;
  else
    return (char *) value;
}

static char *
guess_tek_keyboard_type (dpy)
    Display *dpy;
{
  /* Tek stores the keyboard type on a property on the root window.
     The _TEK_KEYBOARD_TYPE property is ibm101, ibm102, or vt200.
     If _TEK_KEYBOARD_TYPE is vt200, then _TEK_KEYBOARD_LAYOUT is
     ultrix, vms, x_esc, or x_f11. Also, _TEK_KEYBOARD_NATIONALITY
     is usascii, uk, french, swedish, danish, norwegian, german,
     italian, spanish, swiss-german, katakana, or finnish.
   */
  char buf [255];
  tek_kbd_type = get_prop_string (dpy, "_TEK_KEYBOARD_TYPE");

  if (!tek_kbd_type || !strcmp (tek_kbd_type, "ibm101"))
    {
      /* the default keymap changed between release 4 and release 5. */
      if (VendorRelease(dpy) <= 4) return "TEK101-4";
      return "TEK101";
    }
  else if (!strcmp (tek_kbd_type, "ibm102"))
    return "TEK102";
  else if (!strcmp (tek_kbd_type, "vt200"))
    {
      tek_kbd_layout = get_prop_string (dpy, "_TEK_KEYBOARD_LAYOUT");
      tek_kbd_nationality = get_prop_string (dpy, "_TEK_KEYBOARD_NATIONALITY");
      if (!tek_kbd_layout)
	return "TEK200";
      else if (!strcmp (tek_kbd_layout, "x_esc"))
	return "TEK200ESC";
      else if (!strcmp (tek_kbd_layout, "x_f11"))
	return "TEK200F11";
      else if (!strcmp (tek_kbd_layout, "vms"))
	return "TEK200VMS";
      else if (!strcmp (tek_kbd_layout, "ultrix"))
	{
	  char *n = tek_kbd_nationality;
	  if (! n)				return "TEK200US";
	  else if (!strcmp (n, "usascii"))	return "TEK200US";
	  else if (!strcmp (n, "danish"))	return "TEK200DA";
	  else if (!strcmp (n, "german"))	return "TEK200DE";
	  else if (!strcmp (n, "finnish"))	return "TEK200FI";
	  else if (!strcmp (n, "french"))	return "TEK200FR";
	  else if (!strcmp (n, "italian"))	return "TEK200IT";
	  else if (!strcmp (n, "katakana"))	return "TEK200KA";
	  else if (!strcmp (n, "norwegian"))	return "TEK200NO";
	  else if (!strcmp (n, "spanish"))	return "TEK200SP";
	  else if (!strcmp (n, "swedish"))	return "TEK200SW";
	  else if (!strcmp (n, "swiss-german"))	return "TEK200SWDE";
	  else if (!strcmp (n, "uk"))		return "TEK200UK";
	  else
	    {
	      /* some otherwise unknown nationality... */
	      char *s;
	      sprintf (buf, "TEK200_%s", tek_kbd_nationality);
	      s = (char *) malloc (strlen (buf) + 1);
	      strcpy (s, buf);
	      return s;
	    }
	}
      else
	{
	  /* some otherwise unknown layout... */
	  char *s;
	  sprintf (buf, "TEK200_%s", tek_kbd_layout);
	  s = (char *) malloc (strlen (buf) + 1);
	  strcpy (s, buf);
	  return s;
	}
    }
  else
    {
      char *s;
      /* some otherwise unknown type of keyboard... */
      sprintf (buf, "TEK_%s", tek_kbd_type);
      s = (char *) malloc (strlen (buf) + 1);
      strcpy (s, buf);
      return s;
    }
}
