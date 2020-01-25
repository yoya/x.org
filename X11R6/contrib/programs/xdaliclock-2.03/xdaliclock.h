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

#include "vroot.h"

#ifdef __STDC__
# include <stdlib.h>
# if defined(unix) || defined(__unix) || defined(UNIX)
#  include <unistd.h>
# endif
#endif

#ifdef VMS
#include <descrip.h>
# define R_OK 4
#endif

#ifdef SHAPE
#include <X11/extensions/shape.h>
#endif

#ifndef isupper
# define isupper(c)  ((c) >= 'A' && (c) <= 'Z')
#endif
#ifndef _tolower
# define _tolower(c)  ((c) - 'A' + 'a')
#endif

#ifdef __STDC__
# define P(x) x
#else
# define P(x) ()
#endif

/* hack hack */
#define clock_usleep screenhack_usleep
extern void clock_usleep P((unsigned long usecs));

/* From xdaliclock.c: */
extern char *progname;
extern char *progclass;
extern char *hacked_version;
extern Bool wander_p;
extern Bool root_p;
extern XrmDatabase db;
extern int do_cycle;

/* From resources.c: */
extern char *get_string_resource P((char *res_name, char *res_class));
extern Bool get_boolean_resource P((char *res_name, char *res_class));
extern int  get_integer_resource P((char *res_name, char *res_class));
extern double get_float_resource P((char *res_name, char *res_class));
extern unsigned int get_pixel_resource P((char *res_name, char *res_class,
					  Display *dpy, Colormap cmap));

/* From visual.c: */
#ifndef XtNvisual
# define XtNvisual "visual"
#endif
extern int get_visual_class P((Display *dpy, Visual *visual));
extern Visual *get_visual_resource P((Display *dpy, char *name, char *class));
extern void describe_visual P((FILE *f, Display *dpy, Visual *visual));

/* From hsv.c: */
extern void hsv_to_rgb P((int h, double s, double v, 
			  unsigned short *r, unsigned short *g,
			  unsigned short *b));

/* From colors.c: */
extern void allocate_colors P((Display *dpy, Visual *visual, Colormap cmap,
			       char *fg_name, char *bg_name, char *bd_name,
			       XColor *fg_color, XColor*bg_color,
			       XColor *bd_color));
extern void cycle_colors P((Display *dpy, Colormap cmap,
			    XColor *fg_color, XColor *bg_color));

/* From digital.c: */
extern void initialize_digital P((Display *dpy, Visual *visual, Colormap cmap,
				  unsigned long *fgP,
				  unsigned long *bgP,
				  unsigned long *bdP,
				  unsigned int *widthP,
				  unsigned int *heightP));
extern void run_digital P((Display *dpy, Window window));
