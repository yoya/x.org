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
 *
 * Version 1 of this program used only Xlib, not Xt.  The dynamically linked
 * sparc executable was 81k, and geometry and resource handling were kludgy
 * and broken, because I had to duplicate the obscure, undocumented things
 * that Xt does.  Version 2 uses Xt, and geometry and resource handling work
 * correctly - but the dynamically linked executable has more than tripled in
 * size, swelling to 270k.  This is what is commonly refered to as "progress."
 * (Bear in mind that the first (or second) program ever to implement this
 * algorithm ran perfectly well on a Macintosh with a 128k address space.)
 */

#include "version.h"

#include <stdio.h>
#include <X11/Intrinsic.h>
#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/CoreP.h>

#include "xdaliclock.h"
#include "defaults.h"

char *progname;
char *progclass;
char *hacked_version;
XrmDatabase db;
Bool wander_p;
Bool root_p;
Bool do_cycle;

static void usage P((void));
static void hack_version P((void));

static XrmOptionDescRec options [] = {
  /* global options */
  { "-root",		".root",		XrmoptionNoArg, "True" },
  { "-onroot",		".root",		XrmoptionNoArg, "True" },
  { "-window",		".root",		XrmoptionNoArg, "False" },
  { "-fullscreen",	".fullScreen",		XrmoptionNoArg, "True" },
  { "-mono",		".mono",		XrmoptionNoArg, "True" },
  { "-visual",		".visualID",		XrmoptionSepArg, 0 },
  { "-install",		".installColormap",	XrmoptionNoArg, "True" },
  { "-title",		".title",		XrmoptionSepArg, 0  }, /* R3 */
  { "-xrm",		0,			XrmoptionResArg, 0 },

  /* options from digital.c */
  { "-12",		".mode",		XrmoptionNoArg, "12" },
  { "-24",		".mode",		XrmoptionNoArg, "24" },
  { "-datemode",	".datemode",		XrmoptionSepArg, 0 },
  { "-mm/dd/yy",	".datemode",		XrmoptionNoArg, "MM/DD/YY" },
  { "-dd/mm/yy",	".datemode",		XrmoptionNoArg, "DD/MM/YY" },
  { "-seconds",		".seconds",		XrmoptionNoArg, "True" },
  { "-noseconds",	".seconds",		XrmoptionNoArg, "False" },
  { "-cycle",		".cycle",		XrmoptionNoArg, "True" },
  { "-nocycle",		".cycle",		XrmoptionNoArg, "False" },
  { "-font",		".font",		XrmoptionSepArg, 0 },
  { "-fn",		".font",		XrmoptionSepArg, 0 },
  { "-builtin",		".font",		XrmoptionNoArg, "BUILTIN" },
  { "-builtin2",	".font",		XrmoptionNoArg, "BUILTIN2" },
  { "-memory",		".memory",		XrmoptionSepArg, 0 },
  { "-sleaze-level",	".memory",		XrmoptionSepArg, 0 },
  { "-oink",		".memory",		XrmoptionNoArg, "Medium" },
  { "-oink-oink",	".memory",		XrmoptionNoArg, "High" },
  { "-shape",		".shape",		XrmoptionNoArg, "True" },
  { "-noshape",		".shape",		XrmoptionNoArg, "False" }
};

int
main (argc, argv)
     int argc;
     char **argv;
{
  Widget toplevel;
  Display *dpy;
  unsigned int w, h;
  Window window;
  Visual *visual;
  Colormap cmap;
  unsigned long fg, bg, bd;
  XWindowAttributes xgwa;
  Bool full_screen_p;
  XEvent event;

  hack_version ();
  progclass = "XDaliClock";

#if (XtSpecificationRelease >= 4)
  {
    XtAppContext app;
    toplevel = XtAppInitialize (&app, progclass, options,
				sizeof (options) / sizeof (options [0]),
				&argc, argv, defaults, 0, 0);
    dpy = XtDisplay (toplevel);
    db = XtDatabase (dpy);
    XtGetApplicationNameAndClass (dpy, &progname, &progclass);
  }
#else /* R3 */
  {
    char *tmp;
    int i, j;
    XrmDatabase argv_db, server_db, fallback_db;
    progname = argv [0];
# ifdef VMS
    while (tmp = (char *) strrchr (progname, ']'))
      progname = tmp + 1;
    if (tmp = (char *) strrchr (progname, '.'))
      *tmp = '\0';
# else /* !VMS */
    if (tmp = (char *) strrchr (progname, '/'))
      progname = tmp + 1;
# endif /* !VMS */

    argv_db = 0;
    XrmParseCommand (&argv_db,
		     options, sizeof (options) / sizeof (options [0]),
		     progname, &argc, argv);
    toplevel = XtInitialize (progname, progclass, options,
			     sizeof (options) / sizeof (options [0]),
			     &argc, argv);
    dpy = XtDisplay (toplevel);
    server_db = XtDatabase (dpy);
    for (i = 0, j = 0; defaults [i]; i++)
      j += strlen (defaults [i]) + 1;
    tmp = (char *) malloc (j + 1);
    for (i = 0; defaults [i]; i++)
      {
	tmp = strcat (tmp, defaults [i]);
	tmp += strlen (tmp);
	*tmp++ = '\n';
	*tmp = 0;
      }
    tmp -= j;
    fallback_db = XrmGetStringDatabase (tmp);
    free (tmp);
    db = fallback_db;
    XrmMergeDatabases (server_db, &db);
    XrmMergeDatabases (argv_db, &db);
  }
#endif /* !R3 */

  if (argc > 1)
    {
      if (strcmp (argv [1], "-help"))
	fprintf (stderr, "%s: unrecognised option \"%s\"\n",
		 progname, argv[1]);
      usage ();
      exit (-1);
    }

  root_p = get_boolean_resource ("root", "Boolean");
  full_screen_p = get_boolean_resource ("fullScreen", "Boolean");
  do_cycle = get_boolean_resource ("cycle", "Cycle");

  if (root_p)
    {
      visual = DefaultVisual (dpy, DefaultScreen (dpy));
      cmap = DefaultColormap (dpy, DefaultScreen (dpy));
    }
  else
    {
      /* Note: we can't use the resource ".visual" because Xt is SO FUCKED. */
      visual = get_visual_resource (dpy, "visualID", "VisualID");
      if ((visual != DefaultVisual (dpy, DefaultScreen (dpy))) ||
	  get_boolean_resource ("installColormap", "InstallColormap"))
	cmap = XCreateColormap (dpy, RootWindow (dpy, DefaultScreen (dpy)),
				visual, AllocNone);
      else
	cmap = DefaultColormap (dpy, DefaultScreen (dpy));
    }

  wander_p = (root_p || full_screen_p);

  initialize_digital (dpy, visual, cmap, &fg, &bg, &bd, &w, &h);

  if (root_p)
    {
      window = RootWindowOfScreen (XtScreen (toplevel));
      XtDestroyWidget (toplevel);
      XGetWindowAttributes (dpy, window, &xgwa);
      cmap = xgwa.colormap;
      visual = xgwa.visual;
    }
  else
    {
      int xmargin = 10;
      int ymargin = 10;
      Dimension ow, oh, bw;
      Arg av [20];
      int ac;

      ac = 0;
      XtSetArg (av [ac], XtNinput, True); ac++;
      XtSetArg (av [ac], XtNvisual, visual); ac++;
      XtSetArg (av [ac], XtNcolormap, cmap); ac++;
      XtSetArg (av [ac], XtNforeground, ((Pixel) fg)); ac++;
      XtSetArg (av [ac], XtNbackground, ((Pixel) bg)); ac++;
      XtSetArg (av [ac], XtNborderColor, (Pixel) bd); ac++;
      /* This sometimes causes -geometry to be ignored... */
      /* XtSetArg (av [ac], XtNminWidth, (Dimension) w); ac++; */
      /* XtSetArg (av [ac], XtNminHeight, (Dimension) h); ac++; */
      XtSetValues (toplevel, av, ac);

      ac = 0;
      XtSetArg (av [ac], XtNwidth, &ow); ac++;
      XtSetArg (av [ac], XtNheight, &oh); ac++;
      XtSetArg (av [ac], XtNborderWidth, &bw); ac++;
      XtGetValues (toplevel, av, ac);

      if (full_screen_p)
	{
	  w = DisplayWidth  (dpy, DefaultScreen (dpy));
	  h = DisplayHeight (dpy, DefaultScreen (dpy));
	}
      if (ow <= 1 || oh <= 1)
	{
	  Dimension ww = w + xmargin + xmargin;
	  Dimension hh = h + ymargin + ymargin;
	  ac = 0;
	  XtSetArg (av [ac], XtNwidth,  ww); ac++;
	  XtSetArg (av [ac], XtNheight, hh); ac++;
	  XtSetValues (toplevel, av, ac);
	}
      if (full_screen_p)
	{
	  /* #### Want this to set UPosition, not PPosition... */
	  ac = 0;
	  XtSetArg (av [ac], XtNx, -xmargin); ac++;
	  XtSetArg (av [ac], XtNy, -ymargin); ac++;
	  XtSetValues (toplevel, av, ac);
	}

      XtRealizeWidget (toplevel);
      window = XtWindow (toplevel);
    }

  run_digital (dpy, window);
  return (0);
}


static void
usage ()
{
  fprintf (stderr, "%s\n\
usage: %s [ options ]\n\
where options are\n\
\n\
  -12				Display twelve hour time (default).\n\
  -24				Display twenty-four hour time.\n\
  -seconds			Display seconds (default).\n\
  -noseconds			Don't display seconds.\n\
  -cycle			Do color-cycling.\n\
  -nocycle			Don't do color-cycling (default).\n\
  -display <host:dpy>		The display to run on.\n\
  -visual <visual-class>	The visual to use.\n\
  -geometry <geometry>		Size and position of window.\n\
  -foreground or -fg <color>	Foreground color (default black).\n\
  -background or -bg <color>	Background color (default white).\n\
  -reverse or -rv		Swap foreground and background.\n\
  -borderwidth or -bw <int>	Border width.\n\
  -bd or -border <color>	Border color.\n\
  -title <string>		Window title.\n\
  -name <string>		Resource-manager class name (XDaliClock).\n\
  -fullscreen			Use a window that takes up the whole screen.\n\
  -root				Draw on the root window instead.\n\
", version + 4, progname);
#if defined (BUILTIN_FONT) && defined (BUILTIN_FONT_2)
  fprintf (stderr, "\
  -font	or -fn <font>		Name of an X font to use, or the string\n\
				\"BUILTIN\", meaning to use the large builtin\n\
				font (this is the default), or \"BUILTIN2\",\n\
				meaning to use the even larger builtin font.\n\
  -builtin			Same as -font BUILTIN.\n\
  -builtin2			Same as -font BUILTIN2.\n\
");
#else /* ! (BUILTIN_FONT && BUILTIN_FONT_2) */
#if (defined (BUILTIN_FONT) || defined (BUILTIN_FONT_2))
  fprintf (stderr, "\
  -font	or -fn <font>		Name of an X font to use, or the string\n\
				\"BUILTIN\", meaning to use the large builtin\n\
				font (this is the default).\n\
  -builtin			Same as -font BUILTIN.\n\
");
#else /* ! (BUILTIN_FONT || BUILTIN_FONT_2) */
  fprintf (stderr, "\
  -font	or -fn <font>		Name of an X font to use.\n\
");
#endif /* ! (BUILTIN_FONT || BUILTIN_FONT_2) */
#endif /* ! (BUILTIN_FONT && BUILTIN_FONT_2) */
  fprintf (stderr, "\
  -memory <high|medium|low>	Tune the memory versus bandwidth consumption;\n\
				default is \"low\".\n\
  -oink				Same as -memory medium.\n\
  -oink-oink			Same as -memory high.\n\
");
#ifdef SHAPE
  fprintf (stderr, "\
  -shape			Use the Shape extension if available.\n\
  -noshape			Don't.\n");
#endif
#if !(defined (BUILTIN_FONT) || defined (BUILTIN_FONT_2)) || !defined (SHAPE)
  fprintf (stderr, "\n");
# ifndef BUILTIN_FONT
   fprintf(stderr, "This version has been compiled without the builtin fonts");
# endif
# ifndef SHAPE
#  ifndef BUILTIN_FONT
  fprintf (stderr, ", or support for the\nShape Extension.\n");
#  else
  fprintf (stderr, "\
This version has been compiled without support for the Shape Extension.\n");
#  endif
# else
  fprintf (stderr, ".\n");
# endif
#endif
  exit (-1);
}

static void
hack_version ()
{
  char *src = version + 4;
  char *dst = hacked_version = (char *) malloc (strlen (src) + 1);
#define digitp(x) (('0' <= (x)) && ((x) <= '9'))
  while (*src)
    if (!strncmp ("Copyright (c)", src, 13))
      *dst++ = '\251', src += 13;
    else if (*src == '(') *dst++ = '<', *src++;
    else if (*src == ')') *dst++ = '>', *src++;
    else if (digitp (src[0]) && digitp (src[1]) && digitp (src[2]) &&
	     digitp (src[3]) && src[4] == ',' && src[5] == ' ')
      src += 6;
    else
      *dst++ = *src++;
  *dst = 0;
}

#ifdef VMS
/* this isn't right, but it's good enough (only returns 1 or 0) */
int
strcasecmp (s1, s2)
    char *s1, *s2;
{
  int i;
  int L1 = strlen (s1);
  int L2 = strlen (s2);
  if (L1 != L2)
    return 1;
  for (i = 0; i < L1; i++)
    if ((isupper (s1[i]) ? _tolower (s1[i]) : s1[i]) !=
	(isupper (s2[i]) ? _tolower (s2[i]) : s2[i]))
      return 1;
  return 0;
}
#endif /* VMS */
