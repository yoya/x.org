/* xscreensaver, Copyright (c) 1992, 1993 Jamie Zawinski <jwz@lucid.com>
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation.  No representations are made about the suitability of this
 * software for any purpose.  It is provided "as is" without express or 
 * implied warranty.
 */

#if 0
 * Found in Don Hopkin`s .plan file:
 *
 *   The color situation is a total flying circus.  The X approach to
 *   device independence is to treat everything like a MicroVax framebuffer
 *   on acid.  A truely portable X application is required to act like the
 *   persistent customer in the Monty Python ``Cheese Shop'' sketch.  Even
 *   the simplest applications must answer many difficult questions, like:
 *
 *   WHAT IS YOUR DISPLAY?
 *       display = XOpenDisplay("unix:0");
 *   WHAT IS YOUR ROOT?
 *       root = RootWindow(display, DefaultScreen(display));
 *   AND WHAT IS YOUR WINDOW?
 *       win = XCreateSimpleWindow(display, root, 0, 0, 256, 256, 1,
 *                                 BlackPixel(display, DefaultScreen(display)),
 *                                 WhitePixel(display, DefaultScreen(display)))
 *   OH ALL RIGHT, YOU CAN GO ON.
 *
 *   WHAT IS YOUR DISPLAY?
 *         display = XOpenDisplay("unix:0");
 *   WHAT IS YOUR COLORMAP?
 *         cmap = DefaultColormap(display, DefaultScreen(display));
 *   AND WHAT IS YOUR FAVORITE COLOR?
 *         favorite_color = 0; /* Black. */
 *         /* Whoops! No, I mean: */
 *         favorite_color = BlackPixel(display, DefaultScreen(display));
 *         /* AAAYYYYEEEEE!! (client dumps core & falls into the chasm) */
 *
 *   WHAT IS YOUR DISPLAY?
 *         display = XOpenDisplay("unix:0");
 *   WHAT IS YOUR VISUAL?
 *         struct XVisualInfo vinfo;
 *         if (XMatchVisualInfo(display, DefaultScreen(display),
 *                              8, PseudoColor, &vinfo) != 0)
 *            visual = vinfo.visual;
 *   AND WHAT IS THE NET SPEED VELOCITY OF AN XConfigureWindow REQUEST?
 *         /* Is that a SubStructureRedirectMask or a ResizeRedirectMask? */
 *   WHAT??! HOW AM I SUPPOSED TO KNOW THAT?
 *   AAAAUUUGGGHHH!!!! (server dumps core & falls into the chasm)
 *
#endif /* 0 */

#ifndef _SCREENHACK_H_
#define _SCREENHACK_H_

#if __STDC__
#include <stdlib.h>
#endif

#include <X11/Xlib.h>
#include <X11/Xresource.h>
#include <X11/Xos.h>
#include "vroot.h"

extern Bool mono_p;
extern char *progname;
extern char *progclass;
extern XrmDatabase db;
extern XrmOptionDescRec options [];
extern int options_size;
extern char *defaults [];

#if __STDC__
# if defined(SVR4) || defined(SYSV)
#  ifndef random
    extern int rand (void);
#   define random() rand()
#  endif
#  ifndef srandom
    extern void srand (unsigned int);
#   define srandom(i) srand((unsigned int)(i))
#  endif
# else /* !totally-losing-SYSV */
#  ifndef random
    extern long random (void);
#  endif
#  ifndef srandom
    extern void srandom (int);
#  endif
# endif /* !totally-losing-SYSV */
#endif /* __STDC__ */

#if __STDC__
# define P(x)x
#else
# define P(x)()
#endif

extern void screenhack P((Display*,Window));

#define usleep screenhack_usleep

extern void screenhack_usleep P((unsigned long));
extern char *get_string_resource P((char*,char*));
extern Bool get_boolean_resource P((char*,char*));
extern int get_integer_resource P((char*,char*));
extern double get_float_resource P((char*,char*));
extern unsigned int get_pixel_resource P((char*,char*,Display*,Colormap));
extern unsigned int get_minutes_resource P((char*,char*));
extern unsigned int get_seconds_resource P((char*,char*));

extern Visual *get_visual_resource P((Display *, char *, char *));
extern int get_visual_depth P((Display *, Visual *));

extern void hsv_to_rgb P((int,double,double,unsigned short*,
			  unsigned short*,unsigned short*));
extern void rgb_to_hsv P((unsigned short,unsigned short,unsigned short,
			  int*,double*,double*));
extern void cycle_hue P((XColor*,int));

extern void make_color_ramp P((int h1, double s1, double v1,
			       int h2, double s2, double v2,
			       XColor *pixels, int npixels));

extern Pixmap grab_screen_image P((Display *dpy, Window window, int root_p));

static double _frand_tmp_;
#define frand(f)							\
 (_frand_tmp_ = (((double) random()) / 					\
		 (((double) ((unsigned int)~0)) / ((double) (f+f)))),	\
  _frand_tmp_ < 0 ? -_frand_tmp_ : _frand_tmp_)

#undef P
#endif /* _SCREENHACK_H_ */
