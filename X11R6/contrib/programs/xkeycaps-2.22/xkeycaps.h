/* xkeycaps, Copyright (c) 1991, 1992, 1993 Jamie Zawinski <jwz@lucid.com>
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation.  No representations are made about the suitability of this
 * software for any purpose.  It is provided "as is" without express or 
 * implied warranty.
 */

#ifndef _XKEYCAPS_H_
#define _XKEYCAPS_H_

#if __STDC__
#include <stdlib.h>
#endif

extern char *progname;

struct keyboard_instance *choose_kbd ();

#if __STDC__
extern void message (KeyboardWidget, char *);
extern void message2 (KeyboardWidget, char *);
extern int string_equal (char *, char *);
extern void describe_key (KeyWidget);
extern void replace_keyboard (KeyboardWidget, char *);
extern void keyboard_handle_keymap_notify (Widget, XtPointer,
					   XEvent *, Boolean *);
extern void keyboard_handle_mapping_notify (Widget, XtPointer,
					    XEvent *, Boolean *);
#endif


/* When we have to guess, and haven't a clue.
   The Imakefile can define this to be 0, to make there be no default.
 */
#ifndef DEFAULT_KBD_NAME
# define DEFAULT_KBD_NAME "Sun4"
#endif

#endif /* _XKEYCAPS_H_ */
