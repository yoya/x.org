/* xdaliclock - a melting digital clock
 * Copyright (c) 1991, 1992, 1993, 1994 Jamie Zawinski <jwz@lucid.com>
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation.  No representations are made about the suitability of this
 * software for any purpose.  It is provided "as is" without express or
 * implied warranty.
 */

#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xresource.h>

#if (XtSpecificationRelease > 3)
# include <X11/Xfuncs.h>
#endif

#include "xdaliclock.h"

void
allocate_colors (dpy, visual, cmap,
		 fg_name, bg_name, bd_name,
		 fg_color, bg_color, bd_color)
     Display *dpy;
     Visual *visual;
     Colormap cmap;
     char *fg_name, *bg_name, *bd_name;
     XColor *fg_color, *bg_color, *bd_color;
{
  Screen *screen = DefaultScreenOfDisplay (dpy);

  if (! fg_name) fg_name = "black";
  if (! bg_name) bg_name = "white";

 AGAIN:
  if (do_cycle)
    {
      int visual_class = get_visual_class (dpy, visual);
      unsigned long plane_masks;
      unsigned long pixels [2];

      if (visual_class == StaticGray ||
	  visual_class == StaticColor ||
	  visual_class == TrueColor)
	{
	  fprintf (stderr,
		   (!root_p
		    ? "%s: -cycle is incompatible with visual\n\t"
	: "%s: -cycle is incompatible with the root window's visual\n\t"),
		   progname);
	  describe_visual (stderr, dpy, visual);
	  do_cycle = 0;
	  goto AGAIN;
	}
      else if (XAllocColorCells (dpy, cmap, False,
				 &plane_masks, 0, pixels, 2))
	{
	  fg_color->pixel = pixels [0];
	  bg_color->pixel = pixels [1];
	  XParseColor (dpy, cmap, fg_name, fg_color);
	  XParseColor (dpy, cmap, bg_name, bg_color);
	  fg_color->flags = bg_color->flags = DoRed | DoGreen | DoBlue;
	  XStoreColor (dpy, cmap, fg_color);
	  XStoreColor (dpy, cmap, bg_color);
	}
      else
	{
	  fprintf (stderr,
	      "%s: couldn't allocate two read-write color cells on visual\n\t",
		   progname);
	  describe_visual (stderr, dpy, visual);
	  do_cycle = 0;
	  goto AGAIN;
	}
    }
  else
    {
      if (! XParseColor (dpy, cmap, fg_name, fg_color))
	{
	  fprintf (stderr, "%s: can't parse color %s; using black\n",
		   progname, fg_name);
	  fg_color->pixel = BlackPixelOfScreen (screen);
	}
      else if (! XAllocColor (dpy, cmap, fg_color))
	{
	  fprintf (stderr,
		   "%s: couldn't allocate color \"%s\", using black\n",
		   progname, fg_name);
	  fg_color->pixel = BlackPixelOfScreen (screen);
	}

      if (! XParseColor (dpy, cmap, bg_name, bg_color))
	{
	  fprintf (stderr, "%s: can't parse color %s; using white\n",
		   progname, bg_name);
	  bg_color->pixel = WhitePixelOfScreen (screen);
	}
      else if (! XAllocColor (dpy, cmap, bg_color))
	{
	  fprintf (stderr,
		   "%s: couldn't allocate color \"%s\", using white\n",
		   progname, bg_name);
	  bg_color->pixel = WhitePixelOfScreen (screen);
	}

      /* kludge -rv */
      if (get_boolean_resource ("reverseVideo", "ReverseVideo"))
	{
	  XColor swap;
	  swap = *fg_color;
	  *fg_color = *bg_color;
	  *bg_color = swap;
	}
    }

  if (! bd_name)
    bd_name = fg_name;

  /* Set border color to something reasonable in the colormap */
  if (! XParseColor (dpy, cmap, bd_name, bd_color))
    {
      fprintf (stderr, "%s: can't parse color %s; using white\n",
	       progname, bd_name);
      bd_color->pixel = WhitePixelOfScreen (screen);
    }
  else if (! XAllocColor (dpy, cmap, bd_color))
    {
      fprintf (stderr, "%s: couldn't allocate color \"%s\", using white\n",
	       progname, bd_name);
      bd_color->pixel = WhitePixelOfScreen (screen);
    }
}

void
cycle_colors (dpy, cmap, fg_color, bg_color)
     Display *dpy;
     Colormap cmap;
     XColor *fg_color, *bg_color;
{
  static int hue_tick;
  hsv_to_rgb (hue_tick,
	      1.0, 1.0,
	      &fg_color->red, &fg_color->green, &fg_color->blue);
  hsv_to_rgb ((hue_tick + 180) % 360,
	      1.0, 1.0,
	      &bg_color->red, &bg_color->green, &bg_color->blue);
  hue_tick = (hue_tick+1) % 360;
  XStoreColor (dpy, cmap, fg_color);
  XStoreColor (dpy, cmap, bg_color);
}
