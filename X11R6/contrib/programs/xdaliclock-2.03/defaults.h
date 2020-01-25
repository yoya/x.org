/* Resource and command-line junk
 *
 * xdaliclock.c, Copyright (c) 1991, 1992 Jamie Zawinski <jwz@lucid.com>
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation.  No representations are made about the suitability of this
 * software for any purpose.  It is provided "as is" without express or 
 * implied warranty.
 */


static char *defaults[] = {
  "*name:		XDaliClock",
  "*mode:		12",
  "*datemode:		MM/DD/YY",
  "*seconds:		True",
  "*cycle:		False",
  "*shape:		False",
  "*memory:		Low",
  "*title:		Dali Clock",
  "*foreground:		black",
  "*background:		white",
  "XDaliClock*font:	BUILTIN",
  0
};

/* The font resource above has the class specified so that if the user
   has "*font:" in their personal resource database, XDaliClock won't
   inherit it unless its class is specified explicitly.  To do otherwise
   would be annoying.
 */
