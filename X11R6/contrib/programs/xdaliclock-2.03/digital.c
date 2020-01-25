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

#if defined (BUILTIN_FONT) || defined (BUILTIN_FONT_2)

static int use_builtin_font;

struct raw_number {
  unsigned char *bits;
  int width, height;
};

#endif /* BUILTIN_FONT || BUILTIN_FONT_2 */

#ifdef BUILTIN_FONT

# include "zero.xbm"
# include "one.xbm"
# include "two.xbm"
# include "three.xbm"
# include "four.xbm"
# include "five.xbm"
# include "six.xbm"
# include "seven.xbm"
# include "eight.xbm"
# include "nine.xbm"
# include "colon.xbm"
# include "slash.xbm"

static struct raw_number numbers [] = {
  { zero_bits, zero_width, zero_height },
  { one_bits, one_width, one_height },
  { two_bits, two_width, two_height },
  { three_bits, three_width, three_height },
  { four_bits, four_width, four_height },
  { five_bits, five_width, five_height },
  { six_bits, six_width, six_height },
  { seven_bits, seven_width, seven_height },
  { eight_bits, eight_width, eight_height },
  { nine_bits, nine_width, nine_height },
  { colon_bits, colon_width, colon_height },
  { slash_bits, slash_width, slash_height },
  0
};

#endif /* BUILTIN_FONT */

#ifdef BUILTIN_FONT_2

# include "zero2.xbm"
# include "one2.xbm"
# include "two2.xbm"
# include "three2.xbm"
# include "four2.xbm"
# include "five2.xbm"
# include "six2.xbm"
# include "seven2.xbm"
# include "eight2.xbm"
# include "nine2.xbm"
# include "colon2.xbm"
# include "slash2.xbm"

static struct raw_number numbers_2 [] = {
  { zero2_bits, zero2_width, zero2_height },
  { one2_bits, one2_width, one2_height },
  { two2_bits, two2_width, two2_height },
  { three2_bits, three2_width, three2_height },
  { four2_bits, four2_width, four2_height },
  { five2_bits, five2_width, five2_height },
  { six2_bits, six2_width, six2_height },
  { seven2_bits, seven2_width, seven2_height },
  { eight2_bits, eight2_width, eight2_height },
  { nine2_bits, nine2_width, nine2_height },
  { colon2_bits, colon2_width, colon2_height },
  { slash2_bits, slash2_width, slash2_height },
  0
};

#endif /* BUILTIN_FONT_2 */

static char *window_title;
static char *font_name;
static int twelve_hour_time;
static int be_a_pig;
static int minutes_only;
static enum { MMDDYY, DDMMYY, YYMMDD, YYDDMM, MMYYDD, DDYYMM } date_format;

#ifdef SHAPE
static int do_shape;
#endif


static Pixmap pixmap;
static GC pixmap_draw_gc, pixmap_erase_gc;
static GC window_draw_gc, window_erase_gc;
static XColor fg_color, bg_color, bd_color;

static struct frame *base_frames [10];
static struct frame *current_frames [6];
static struct frame *target_frames [6];
static struct frame *clear_frame;
static int character_width, character_height;
static int x_offsets [6];
static int window_offset_x, window_offset_y;
static int wander_x, wander_y, wander_delta_x, wander_delta_y;
static Pixmap colon, slash;
static int colon_char_width;
static int time_digits [6];
static int last_time_digits [6];
static int button_down = 0; /* #### */

static Pixmap memory_pig_zeros [9] [10];
static Pixmap memory_pig_digits [8] [10];
static Pixmap total_oinker_digits [9] [10];

#undef MAX
#undef MIN
#define MAX(a, b) ((a)>(b)?(a):(b))
#define MIN(a, b) ((a)<(b)?(a):(b))

typedef short unsigned int POS;

/* Number of horizontal segments/line.  Enlarge this if you are trying
   to use a font that is too "curvy" for XDaliClock to cope with.
   This code was sent to me by Dan Wallach <c169-bg@auriga.berkeley.edu>.
   I'm highly opposed to ever using statically-sized arrays, but I don't
   really feel like hacking on this code enough to clean it up.
 */
#ifndef MAX_SEGS_PER_LINE
#define MAX_SEGS_PER_LINE 3
#endif

struct scanline {
  POS left[MAX_SEGS_PER_LINE], right[MAX_SEGS_PER_LINE];
};

struct frame {
  struct scanline scanlines [1]; /* scanlines are contiguous here */
};


static struct frame *
image_to_frame (image)
     XImage *image;
{
  register int y, x;
  struct frame *frame;
  int width = image->width;
  int height = image->height;
  POS *left, *right;
/*  int *nsegs; */
  int maxsegments = 0;

  frame = (struct frame *)
    malloc (sizeof (struct frame) +
	    (sizeof (struct scanline) * (height - 1)));
/*  nsegs = (int *) malloc (sizeof (int) * height); */

  for (y = 0; y < height; y++)
    {
      int seg, end;
      x = 0;
#define getbit(x) (XGetPixel (image, (x), y))
      left = frame->scanlines[y].left;
      right = frame->scanlines[y].right;

      for (seg = 0; seg < MAX_SEGS_PER_LINE; seg++)
	left [seg] = right [seg] = width / 2;

      for (seg = 0; seg < MAX_SEGS_PER_LINE; seg++)
	{
	  for (; x < width; x++)
	    if (getbit (x)) break;
	  if (x == width) break;
	  left [seg] = x;
	  for (; x < width; x++)
	    if (! getbit (x)) break;
	  right [seg] = x;
	}

      for (; x < width; x++)
	if (getbit (x))
	  {
	    fprintf (stderr,
   "%s: font is too curvy.  Increase MAX_SEGS_PER_LINE (%d) and recompile.\n",
		     progname, MAX_SEGS_PER_LINE);
	    exit (-1);
	  }

      /* If there were any segments on this line, then replicate the last
	 one out to the end of the line.  If it's blank, leave it alone,
	 meaning it will be a 0-pixel-wide line down the middle.
       */
      end = seg;
      if (end > 0)
	for (; seg < MAX_SEGS_PER_LINE; seg++)
	  {
	    left [seg] = left [end-1];
	    right [seg] = right [end-1];
	  }
      if (end > maxsegments) maxsegments = end;
/*      nsegs [y] = end; */

#undef getbit
    }
  return frame;
}


/* This is kind of gross.
 */
static char *default_fonts [] = {
  "-*-charter-bold-i-*-*-*-500-*-*-*-*-*-*",
  "-*-lucidabright-demibold-i-*-*-*-500-*-*-*-*-*-*",
  "-*-new century schoolbook-bold-i-*-*-*-500-*-*-*-*-*-*",
  "-*-helvetica-bold-o-*-*-*-500-*-*-*-*-*-*",
  "-*-lucida-bold-i-*-*-*-500-*-*-*-*-*-*",

  "-*-charter-bold-i-*-*-*-360-*-*-*-*-*-*",
  "-*-lucidabright-demibold-i-*-*-*-360-*-*-*-*-*-*",
  "-*-new century schoolbook-bold-i-*-*-*-360-*-*-*-*-*-*",
  "-*-helvetica-bold-o-*-*-*-360-*-*-*-*-*-*",
  "-*-lucida-bold-i-*-*-*-360-*-*-*-*-*-*",

  "-*-charter-bold-i-*-*-*-240-*-*-*-*-*-*",
  "-*-lucidabright-demibold-i-*-*-*-240-*-*-*-*-*-*",
  "-*-new century schoolbook-bold-i-*-*-*-240-*-*-*-*-*-*",
  "-*-helvetica-bold-o-*-*-*-240-*-*-*-*-*-*",
  "-*-lucida-bold-i-*-*-*-240-*-*-*-*-*-*",	/* too wide.  bug? */

  "-*-charter-bold-i-*-*-*-180-*-*-*-*-*-*",
  "-*-lucidabright-demibold-i-*-*-*-180-*-*-*-*-*-*",
  "-*-new century schoolbook-bold-i-*-*-*-180-*-*-*-*-*-*",
  "-*-helvetica-bold-o-*-*-*-180-*-*-*-*-*-*",	/* too wide */
  "-*-lucida-bold-i-*-*-*-180-*-*-*-*-*-*",	/* too wide */
  0
};

static void
load_font (dpy, fn)
     Display *dpy;
     char *fn;
{
  Screen *screen = DefaultScreenOfDisplay (dpy);
  int i, max_lbearing, max_rbearing, max_ascent, max_descent;
  XFontStruct *font = 0;
  Pixmap pixmap;
  XImage *image = 0;
  XGCValues gcvalues;
  GC draw_gc, erase_gc;
  char **fonts = default_fonts;
  int bad_font_name = 0;

  if (fn)
    if (! (font = XLoadQueryFont (dpy, fn)))
      {
	bad_font_name = 1;
	fprintf (stderr, "%s: Couldn't load font \"%s\";\n", progname,
		 fn);
      }
  if (! font)
    for (; fonts; fonts++)
      if (font = XLoadQueryFont (dpy, fn = *fonts))
	break;
  if (bad_font_name && font)
    fprintf (stderr, " using font \"%s\" instead.\n", fonts [0]);
  else if (! font)
    {
      if (bad_font_name)
	fprintf (stderr, " couldn't load any of the default fonts either.\n");
      else
	fprintf (stderr, "%s: Couldn't load any of the default fonts.\n",
		 progname);
      exit (-1);
    }

  if (font->min_char_or_byte2 > '0' || font->max_char_or_byte2 < '9')
    {
      fprintf (stderr, "%s: font %s doesn't contain characters '0'-'9'.\n",
	       progname);
      exit (-1);
    }
  max_lbearing = font->min_bounds.lbearing;
  max_rbearing = font->min_bounds.rbearing;
  max_ascent  = font->min_bounds.ascent;
  max_descent = font->min_bounds.descent;
  for (i = '0'; i <= '9'; i++)
    {
      XCharStruct *ch = (font->per_char
			 ? &font->per_char [i - font->min_char_or_byte2]
			 : &font->min_bounds);
      max_lbearing = MAX (max_lbearing, ch->lbearing);
      max_rbearing = MAX (max_rbearing, ch->rbearing);
      max_ascent  = MAX (max_ascent,  ch->ascent);
      max_descent = MAX (max_descent, ch->descent);
      if (ch->lbearing == ch->rbearing || ch->ascent == -ch->descent)
	{
	  fprintf (stderr,
	      "%s: char '%c' has bbox %dx%d (%d - %d x %d + %d) in font %s\n",
		   progname,
		   i, ch->rbearing - ch->lbearing, ch->ascent + ch->descent,
		   ch->rbearing, ch->lbearing, ch->ascent, ch->descent,
		   fn);
	  exit (-1);
	}
    }
  character_width = max_rbearing + max_lbearing + 1; /* min enclosing rect */
  character_height = max_descent + max_ascent + 1;

  /* Now we know the combined bbox of the characters we're interested in;
     for each character, write it into a pixmap; grab the bits from that
     pixmap; and fill the scanline-buffers.
   */
  pixmap = XCreatePixmap (dpy, RootWindowOfScreen (screen),
			  character_width + 1, character_height + 1, 1);
  gcvalues.font = font->fid;
  gcvalues.foreground = 1L;
  draw_gc = XCreateGC (dpy, pixmap, GCFont | GCForeground, &gcvalues);
  gcvalues.foreground = 0L;
  erase_gc = XCreateGC (dpy, pixmap, GCForeground, &gcvalues);

  for (i = 0; i <= 9; i++)
    {
      XCharStruct *ch = (font->per_char
			 ? &font->per_char [i + '0' - font->min_char_or_byte2]
			 : &font->min_bounds);
      char s[1];
      s[0] = i + '0';
      XFillRectangle (dpy, pixmap, erase_gc, 0, 0,
		      character_width + 1, character_height + 1);
      XDrawString (dpy, pixmap, draw_gc, max_lbearing, max_ascent, s, 1);
      if (! image)
	image = XGetImage (dpy, pixmap, 0, 0,
			   character_width, character_height, 1, XYPixmap);
      else
	XGetSubImage (dpy, pixmap, 0, 0,
		      character_width, character_height, 1, XYPixmap,
		      image, 0, 0);
      base_frames [i] = image_to_frame (image);
    }

  {
    XCharStruct *ch1, *ch2;
    int maxl, maxr, w;
    if (font->per_char)
      {
	ch1 = &font->per_char [':' - font->min_char_or_byte2];
	ch2 = &font->per_char ['/' - font->min_char_or_byte2];
      }
    else
      ch1 = ch2 = &font->min_bounds;

    maxl = MAX (ch1->lbearing, ch2->lbearing);
    maxr = MAX (ch1->rbearing, ch2->rbearing);
    w = maxr + maxl + 1;
    colon =
      XCreatePixmap (dpy, RootWindowOfScreen (screen),
		     w+1, character_height+1, 1);
    slash =
      XCreatePixmap (dpy, RootWindowOfScreen (screen),
		     w+1, character_height+1, 1);
    XFillRectangle (dpy, colon, erase_gc, 0, 0, w+1, character_height+1);
    XFillRectangle (dpy, slash, erase_gc, 0, 0, w+1, character_height+1);
    XDrawString (dpy, colon, draw_gc, maxl+1, max_ascent, ":", 1);
    XDrawString (dpy, slash, draw_gc, maxl+1, max_ascent, "/", 1);
    colon_char_width = w;
  }

  XDestroyImage (image);
  XFreePixmap (dpy, pixmap);
  XFreeFont (dpy, font);
  XFreeGC (dpy, draw_gc);
  XFreeGC (dpy, erase_gc);
}

#if defined (BUILTIN_FONT) || defined (BUILTIN_FONT_2)
static void
load_builtin_font (dpy, visual, which)
     Display *dpy;
     Visual *visual;
     int which;
{
  int i;
  Pixmap pixmap;
  char **fonts = default_fonts;

#if defined (BUILTIN_FONT) && defined (BUILTIN_FONT_2)
  struct raw_number *nums = (which == 1 ? numbers : numbers_2);
#else /* ! BUILTIN_FONT && BUILTIN_FONT_2 */
# ifdef BUILTIN_FONT
  struct raw_number *nums = numbers;
# else /* BUILTIN_FONT_2 */
  struct raw_number *nums = numbers_2;
# endif /* BUILTIN_FONT_2 */
#endif /* ! BUILTIN_FONT && BUILTIN_FONT_2 */

  XImage *image =
    XCreateImage (dpy, visual,
		  1, XYBitmap, 0,	/* depth, format, offset */
		  (char *) 0,		/* data */
		  0, 0, 8, 0); 		/* w, h, pad, bytes_per_line */
  /* This stuff makes me nervous, but XCreateBitmapFromData() does it too. */
  image->byte_order = LSBFirst;
  image->bitmap_bit_order = LSBFirst;

  character_width = character_height = 0;

  for (i = 0; i < 10; i++)
    {
      struct raw_number *number = &nums [i];
      character_width = MAX (character_width, number->width);
      character_height = MAX (character_height, number->height);
      image->width = number->width;
      image->height = number->height;
      image->data = (char *) number->bits;
      image->bytes_per_line = (number->width + 7) / 8;
      base_frames [i] = image_to_frame (image);
    }
  image->data = 0;
  XDestroyImage (image);

  colon_char_width = MAX (nums [10].width, nums [11].width);
  colon = XCreateBitmapFromData (dpy, DefaultRootWindow (dpy),
				 (char *) nums[10].bits,
				 nums[10].width, nums[10].height);
  slash = XCreateBitmapFromData (dpy, DefaultRootWindow (dpy),
				 (char *) nums[11].bits,
				 nums[11].width, nums[11].height);
}
#endif /* BUILTIN_FONT || BUILTIN_FONT_2 */

/*  It doesn't matter especially what MAX_SEGS is -- it gets flushed
    when it might fill up -- this number is just for performance tuning
 */
#define MAX_SEGS MAX_SEGS_PER_LINE * 200
static XSegment segment_buffer [MAX_SEGS + 2];
static int segment_count = 0;

static void
flush_segment_buffer (dpy, drawable, draw_gc)
     Display *dpy;
     Drawable drawable;
     GC draw_gc;
{
  if (! segment_count) return;
  XDrawSegments (dpy, drawable, draw_gc, segment_buffer, segment_count);
  segment_count = 0;
}



static void
draw_frame (dpy, frame, drawable, draw_gc, x_off)
     Display *dpy;
     struct frame *frame;
     Drawable drawable;
     GC draw_gc;
     int x_off;
{
  register int y, x;
  for (y = 0; y < character_height; y++)
    {
      struct scanline *line = &frame->scanlines [y];
      for (x = 0; x < MAX_SEGS_PER_LINE; x++) {
	if (line->left[x] == line->right[x] ||
	    (x > 0 &&
	     line->left[x] == line->left[x-1] &&
	     line->right[x] == line->right[x-1]))
	  continue;
	segment_buffer [segment_count].x1 = x_off + line->left[x];
	segment_buffer [segment_count].x2 = x_off + line->right[x];
	segment_buffer [segment_count].y1 = y;
	segment_buffer [segment_count].y2 = y;
	segment_count++;

	if (segment_count >= MAX_SEGS)
	  flush_segment_buffer (dpy, drawable, draw_gc);
      }
    }
}

static void
set_current_scanlines (into_frame, from_frame)
     struct frame *into_frame, *from_frame;
{
  register int i;
  for (i = 0; i < character_height; i++)
    into_frame->scanlines [i] = from_frame->scanlines [i];
}

static void
one_step (current_frame, target_frame, tick)
     struct frame *current_frame, *target_frame;
     int tick;
{
  register struct scanline *line = &current_frame->scanlines [0];
  register struct scanline *target = &target_frame->scanlines [0];
  register int i = 0, x;
  for (i = 0; i < character_height; i++)
    {
#define STEP(field) (line->field += ((int) (target->field - line->field))  \
		     / tick)
      for (x = 0; x < MAX_SEGS_PER_LINE; x++)
	{
	  STEP (left [x]);
	  STEP (right [x]);
	}
      line++;
      target++;
    }
}

static char test_hack; /* gag */

static long
fill_time_digits ()
{
  long clock = time ((time_t *) 0);
  struct tm *tm = localtime (&clock);
  if (test_hack)
    {
      time_digits [0] = time_digits [1] = time_digits [2] =
	time_digits [3] = time_digits [4] = time_digits [5] =
	  (test_hack == '-' ? -1 : test_hack - '0');
      test_hack = 0;
    }
  else if (! button_down)
    {
      if (twelve_hour_time && tm->tm_hour > 12) tm->tm_hour -= 12;
      if (twelve_hour_time && tm->tm_hour == 0) tm->tm_hour = 12;
      time_digits [0] = (tm->tm_hour - (tm->tm_hour % 10)) / 10;
      time_digits [1] = tm->tm_hour % 10;
      time_digits [2] = (tm->tm_min - (tm->tm_min % 10)) / 10;
      time_digits [3] = tm->tm_min % 10;
      time_digits [4] = (tm->tm_sec - (tm->tm_sec % 10)) / 10;
      time_digits [5] = tm->tm_sec % 10;
    }
  else
    {
      int m0,m1,d0,d1,y0,y1;
      tm->tm_mon++; /* 0 based */
      m0 = (tm->tm_mon - (tm->tm_mon % 10)) / 10;
      m1 = tm->tm_mon % 10;
      d0 = (tm->tm_mday - (tm->tm_mday % 10)) / 10;
      d1 = tm->tm_mday % 10;
      y0 = (tm->tm_year - (tm->tm_year % 10)) / 10;
      y1 = tm->tm_year % 10;

      switch (date_format)
	{
	case MMDDYY:
	  time_digits [0] = m0; time_digits [1] = m1;
	  time_digits [2] = d0; time_digits [3] = d1;
	  time_digits [4] = y0; time_digits [5] = y1;
	  break;
	case DDMMYY:
	  time_digits [0] = d0; time_digits [1] = d1;
	  time_digits [2] = m0; time_digits [3] = m1;
	  time_digits [4] = y0; time_digits [5] = y1;
	  break;
	case YYMMDD:
	  time_digits [0] = y0; time_digits [1] = y1;
	  time_digits [2] = m0; time_digits [3] = m1;
	  time_digits [4] = d0; time_digits [5] = d1;
	  break;
	case YYDDMM:
	  time_digits [0] = y0; time_digits [1] = y1;
	  time_digits [2] = d0; time_digits [3] = d1;
	  time_digits [4] = m0; time_digits [5] = m1;
	  break;
	  /* These are silly, but are included for completeness... */
	case MMYYDD:
	  time_digits [0] = m0; time_digits [1] = m1;
	  time_digits [2] = y0; time_digits [3] = y1;
	  time_digits [4] = d0; time_digits [5] = d1;
	  break;
	case DDYYMM:
	  time_digits [0] = d0; time_digits [1] = d1;
	  time_digits [2] = y0; time_digits [3] = y1;
	  time_digits [4] = m0; time_digits [5] = m1;
	  break;
	}
    }
  return clock;
}


static void
fill_pig_cache (dpy, drawable, work_frame)
     Display *dpy;
     Drawable drawable;
     struct frame *work_frame;
{
  int i;
  /* do `[1-9]' to `0'
     We have very little to gain by caching the `[347]' to `0' transitions,
     but what the hell.
   */
  for (i = 0; i < 9; i++)
    {
      int tick;
      set_current_scanlines (work_frame, base_frames [0]);
      for (tick = 9; tick >= 0; tick--)
	{
	  Pixmap p = XCreatePixmap (dpy, drawable,
				    character_width, character_height, 1);
	  XFillRectangle (dpy, p, pixmap_erase_gc, 0, 0,
			  character_width, character_height);
	  draw_frame (dpy, work_frame, p, pixmap_draw_gc, 0);
	  flush_segment_buffer (dpy, p, pixmap_draw_gc);
	  memory_pig_zeros [i] [9 - tick] = p;
	  if (tick)
	    one_step (work_frame, base_frames [i+1], tick);
	}
    }
  /* do `[1-8]' to `[2-9]' */
  for (i = 0; i < 8; i++)
    {
      int tick;
      set_current_scanlines (work_frame, base_frames [i+1]);
      for (tick = 9; tick >= 0; tick--)
	{
	  Pixmap p = XCreatePixmap (dpy, drawable,
				    character_width, character_height, 1);
	  XFillRectangle (dpy, p, pixmap_erase_gc, 0, 0,
			  character_width, character_height);
	  draw_frame (dpy, work_frame, p, pixmap_draw_gc, 0);
	  flush_segment_buffer (dpy, p, pixmap_draw_gc);
	  memory_pig_digits [i] [9 - tick] = p;
	  if (tick)
	    one_step (work_frame, base_frames [i+2], tick);
	}
    }
  if (be_a_pig > 1)
    /* do `[1-7]' to `[3-9]' and `9' to `1' */
    for (i = 0; i < 9; i++)
      {
	int tick;
	if (i == 7) continue; /* zero transitions are already done */
	set_current_scanlines (work_frame, base_frames [i+1]);
	for (tick = 9; tick >= 0; tick--)
	  {
	    Pixmap p = XCreatePixmap (dpy, drawable,
				      character_width, character_height, 1);
	    XFillRectangle (dpy, p, pixmap_erase_gc, 0, 0,
			    character_width, character_height);
	    draw_frame (dpy, work_frame, p, pixmap_draw_gc, 0);
	    flush_segment_buffer (dpy, p, pixmap_draw_gc);
	    total_oinker_digits [i] [9 - tick] = p;
	    if (tick)
	      one_step (work_frame, base_frames [(i+3)%10], tick);
	  }
      }
}


static void
initialize_digits (dpy, visual)
     Display *dpy;
     Visual *visual;
{
  int i, x;
#if defined (BUILTIN_FONT) || defined (BUILTIN_FONT_2)
  if (use_builtin_font)
    load_builtin_font (dpy, visual, use_builtin_font);
  else
#endif
    load_font (dpy, font_name);

  memset ((char *) memory_pig_zeros,  0, sizeof (memory_pig_zeros));
  memset ((char *) memory_pig_digits, 0, sizeof (memory_pig_digits));

  for (i = 0; i < 6; i++)
    current_frames [i] = (struct frame *)
      malloc (sizeof (struct frame) +
	      (sizeof (struct scanline) * (character_height - 1)));

  clear_frame = (struct frame *)
    malloc (sizeof (struct frame) +
	    (sizeof (struct scanline) * (character_height - 1)));
  for (i = 0; i < character_height; i++)
    for (x = 0; x < MAX_SEGS_PER_LINE; x++)
      clear_frame->scanlines [i].left [x] =
	clear_frame->scanlines [i].right [x] = character_width / 2;

  x_offsets [0] = 0;
  x_offsets [1] = x_offsets [0] + character_width;
  x_offsets [2] = x_offsets [1] + character_width + colon_char_width;
  x_offsets [3] = x_offsets [2] + character_width;
  x_offsets [4] = x_offsets [3] + character_width + colon_char_width;
  x_offsets [5] = x_offsets [4] + character_width;

  wander_x = character_width / 2;
  wander_y = character_height / 2;
  wander_delta_x = wander_delta_y = 1;
}


static void draw_colon P((Display *dpy, Drawable pixmap, Window window));

static void
initialize_window (dpy, window)
     Display *dpy;
     Window window;
{
  XWindowAttributes xgwa;
  Screen *screen = DefaultScreenOfDisplay (dpy);
  XSetWindowAttributes attributes;
  unsigned long attribute_mask;
  XGCValues gcvalues;
  int ndigits = minutes_only ? 4 : 6;
  int i;

  XGetWindowAttributes (dpy, window, &xgwa);
#ifdef SHAPE
  if (!wander_p)
    {
      int shape_event_base, shape_error_base;
      if (do_shape && !XShapeQueryExtension (dpy,
					     &shape_event_base,
					     &shape_error_base))
	{
	  fprintf (stderr, "%s: no shape extension on this display\n",
		   progname);
	  do_shape = 0;
	}
    }
#endif

  attribute_mask = (CWBackPixel | CWEventMask | CWDontPropagate);
  attributes.background_pixel = bg_color.pixel;
  attributes.event_mask = (xgwa.your_event_mask |
			   KeyPressMask | ButtonPressMask |
			   ButtonReleaseMask | StructureNotifyMask |
			   ExposureMask );
  attributes.do_not_propagate_mask = 0;
  XChangeWindowAttributes (dpy, window, attribute_mask, &attributes);

  /* Center stuff in window correctly when first created */
  {
    int width = x_offsets [minutes_only ? 3 : 5] + character_width;
#ifdef SHAPE
    if (do_shape)
      window_offset_x = window_offset_y = 0;
    else
#endif
    if (wander_p)
      {
	window_offset_x = (WidthOfScreen (screen) - width) / 2;
	window_offset_y = (HeightOfScreen (screen) - character_height) / 2;
      }
    else
      {
	window_offset_x = (xgwa.width - width) / 2;
	window_offset_y = (xgwa.height - character_height) / 2;
      }
  }

  XStoreName (dpy, window, window_title);

  pixmap = XCreatePixmap (dpy, window,
			  x_offsets [ndigits-1] + character_width + 1,
			  character_height + 1, 1);

  gcvalues.foreground = fg_color.pixel;
  gcvalues.background = bg_color.pixel;
  gcvalues.function = GXcopy;
  gcvalues.graphics_exposures = False;
  window_draw_gc = XCreateGC (dpy, window,
			      GCForeground | GCBackground | GCFunction |
			      GCGraphicsExposures,
			      &gcvalues);
  gcvalues.foreground = bg_color.pixel;
  gcvalues.background = fg_color.pixel;
  window_erase_gc = XCreateGC (dpy, window,
			       GCForeground | GCBackground | GCFunction |
			       GCGraphicsExposures,
			       &gcvalues);
  gcvalues.foreground = 1;
  gcvalues.background = 0;
  pixmap_draw_gc = XCreateGC (dpy, pixmap,
			      GCForeground | GCBackground | GCFunction |
			      GCGraphicsExposures,
			      &gcvalues);
  gcvalues.foreground = 0;
  gcvalues.background = 1;
  pixmap_erase_gc = XCreateGC (dpy, pixmap,
			       GCForeground | GCBackground | GCFunction |
			       GCGraphicsExposures,
			       &gcvalues);

  XFillRectangle (dpy, pixmap, pixmap_erase_gc,
		  0, 0, x_offsets [ndigits-1] + character_width + 1,
		  character_height + 1);
}


static void
get_resources (dpy)
     Display *dpy;
{
  char *buf;

  /* #### maybe should be elsewhere */
  window_title = get_string_resource ("title", "Title");

  minutes_only = !get_boolean_resource ("seconds", "Seconds");
  font_name = get_string_resource ("font", "Font");
#if defined (BUILTIN_FONT) || defined (BUILTIN_FONT_2)
  use_builtin_font = 0;
  if (!strcasecmp (font_name, "BUILTIN")) use_builtin_font = 1;
  if (!strcasecmp (font_name, "BUILTIN2")) use_builtin_font = 2;
#endif /* BUILTIN_FONT || BUILTIN_FONT_2 */
#ifdef SHAPE
  if (wander_p)
    do_shape = 0;
  else
    do_shape = get_boolean_resource ("shape", "Shape");
#endif

  buf = get_string_resource ("mode", "Mode");
  if (buf == 0 || !strcmp (buf, "12"))
    twelve_hour_time = 1;
  else if (!strcmp (buf, "24"))
    twelve_hour_time = 0;
  else
    {
      fprintf (stderr, "%s: mode must be \"12\" or \"24\", not %s.\n",
	       progname, buf);
      twelve_hour_time = 1;
    }
  if (buf) free (buf);

  buf = get_string_resource ("memory", "Memory");
  if (!strcasecmp (buf, "high") ||
      !strcasecmp (buf, "hi") ||
      !strcasecmp (buf, "sleazy"))
    be_a_pig = 2;
  else if (!strcasecmp (buf, "medium") ||
	   !strcasecmp (buf, "med"))
    be_a_pig = 1;
  else if (!strcasecmp (buf, "low") ||
	   !strcasecmp (buf, "lo"))
    be_a_pig = 0;
  else
    {
      fprintf (stderr,
         "%s: memory must be \"high\", \"medium\", or \"low\", not \"%s\".\n",
	       progname, buf);
      be_a_pig = 0;
    }
  if (buf) free (buf);

  buf = get_string_resource ("datemode", "DateMode");
  if (!strcasecmp (buf, "mmddyy") || !strcasecmp (buf, "mm/dd/yy") ||
      !strcasecmp (buf, "mm-dd-yy"))
    date_format = MMDDYY;
  else if (!strcasecmp (buf, "ddmmyy") || !strcasecmp (buf, "dd/mm/yy") ||
	   !strcasecmp (buf, "dd-mm-yy"))
    date_format = DDMMYY;
  else if (!strcasecmp (buf, "yymmdd") || !strcasecmp (buf, "yy/mm/dd") ||
	   !strcasecmp (buf, "yy-mm-dd"))
    date_format = YYMMDD;
  else if (!strcasecmp (buf, "yyddmm") || !strcasecmp (buf, "yy/dd/mm") ||
	   !strcasecmp (buf, "yy-dd-mm"))
    date_format = YYDDMM;
  else if (!strcasecmp (buf, "mmyydd") || !strcasecmp (buf, "mm/yy/dd") ||
	   !strcasecmp (buf, "mm-yy-dd"))
    date_format = MMYYDD;
  else if (!strcasecmp (buf, "ddyymm") || !strcasecmp (buf, "dd/yy/mm") ||
	   !strcasecmp (buf, "dd-yy-mm"))
    date_format = DDYYMM;
  else
    {
      fprintf (stderr,
         "%s: DateMode must be \"MM/DD/YY\", \"DD/MM/YY\", etc; not \"%s\"\n",
	       progname, buf);
      date_format = MMDDYY;
    }
  if (buf) free (buf);
}



static void
wander ()
{
  wander_x += wander_delta_x;
  if (wander_x == character_width || wander_x == 0)
    {
      wander_delta_x = -wander_delta_x;
      wander_y += wander_delta_y;
      if (wander_y == character_height || wander_y == 0)
	wander_delta_y = -wander_delta_y;
    }
}


static void
draw_colon (dpy, pixmap, window)
     Display *dpy;
     Drawable pixmap;
     Window window;
{
  Pixmap glyph = button_down ? slash : colon;
  XCopyPlane (dpy, glyph, pixmap, pixmap_draw_gc,
	      0, 0, character_width, character_height,
	      x_offsets [1] + character_width, 0, 1);
  if (! minutes_only)
    XCopyPlane (dpy, glyph, pixmap, pixmap_draw_gc,
		0, 0, character_width, character_height,
		x_offsets [3] + character_width, 0, 1);
  XStoreName (dpy, window, button_down ? hacked_version : window_title);
}


#define TICK_SLEEP()	clock_usleep (80000L)		/* 8/100th second */
#define SEC_SLEEP()	clock_usleep (1000000L)		/* 1 second */

/* The above pair of routines can't be implemented using a combination of
   sleep() and usleep() if you system has them, because they interfere with
   each other: using sleep() will cause later calls to usleep() to fail.
   The select() version is more efficient anyway (fewer system calls.)
 */


static int event_loop P((Display *, Window));

void
initialize_digital (dpy, visual, cmap,
		    fgP, bgP, bdP, widthP, heightP)
     Display *dpy;
     Visual *visual;
     Colormap cmap;
     unsigned long *fgP, *bgP, *bdP;
     unsigned int *widthP, *heightP;
{
  int ndigits;
  get_resources (dpy);
  initialize_digits (dpy, visual);

  allocate_colors (dpy, visual, cmap,
		   get_string_resource ("foreground", "Foreground"),
		   get_string_resource ("background", "Background"),
		   get_string_resource ("borderColor", "Foreground"),
		   &fg_color, &bg_color, &bd_color);
  *fgP = fg_color.pixel;
  *bgP = bg_color.pixel;
  *bdP = bd_color.pixel;

  ndigits = (minutes_only ? 4 : 6);
  *widthP = x_offsets [ndigits-1] + character_width;
  *heightP = character_height;
}


void
run_digital (dpy, window)
     Display *dpy;
     Window window;
{
  int i;
  int ndigits = (minutes_only ? 4 : 6);
  XWindowAttributes xgwa;
  XGetWindowAttributes (dpy, window, &xgwa);
  initialize_window (dpy, window);

  if (be_a_pig)
    fill_pig_cache (dpy, window, current_frames [0]);

  for (i = 0; i < ndigits; i++)
    {
      last_time_digits [i] = -1;
      set_current_scanlines (current_frames [i], clear_frame);
    }

  event_loop (dpy, window); /* wait for initial mapwindow event */

  XFillRectangle (dpy, window, window_erase_gc,
		  0, 0, x_offsets [ndigits-1] + character_width,
		  character_height + 1);
  draw_colon (dpy, pixmap, window);

  while (1)
    {
      int n, tick;
      long clock;
      int different_minute;

      clock = fill_time_digits ();

      different_minute = (time_digits [3] != last_time_digits [3]);

      for (n = 0; n < ndigits; n++)
	if (time_digits [n] == last_time_digits [n])
	  target_frames [n] = 0;
	else if (time_digits [n] < 0)
	  target_frames [n] = clear_frame;
	else
	  target_frames [n] = base_frames [time_digits [n]];

      if (twelve_hour_time && target_frames [0] && time_digits [0] == 0)
	{
	  target_frames [0] = clear_frame;
	  time_digits [0] = -1;
	}
      if (time_digits [0] < 0 && last_time_digits [0] < 0)
	target_frames [0] = 0;
      if (last_time_digits [0] == -2) /* evil hack for 12<->24 mode toggle */
	target_frames [0] = clear_frame;

      for (tick = 9; tick >= 0; tick--)
	{
	  int j;
	  if (do_cycle)
	    cycle_colors (dpy, xgwa.colormap, &fg_color, &bg_color);
	  for (j = 0; j < ndigits; j++)
	    {
	      if (target_frames [j])
		{
		  if (be_a_pig && time_digits [j] == 0 &&
		      last_time_digits [j] >= 0)
		    {
		      Pixmap p =
			memory_pig_zeros [last_time_digits [j] - 1] [tick];
		      XCopyPlane (dpy, p, pixmap, pixmap_draw_gc, 0, 0,
				  character_width, character_height,
				  x_offsets [j], 0, 1);
		    }
		  else if (be_a_pig && last_time_digits [j] == 0 &&
			   time_digits [j] >= 0)
		    {
		      Pixmap p =
			memory_pig_zeros [time_digits [j] - 1] [9 - tick];
		      XCopyPlane (dpy, p, pixmap, pixmap_draw_gc, 0, 0,
				  character_width, character_height,
				  x_offsets [j], 0, 1);
		    }
		  else if (be_a_pig && last_time_digits [j] >= 0 &&
			   time_digits [j] == last_time_digits [j] + 1)
		    {
		      Pixmap p =
			memory_pig_digits [last_time_digits [j] - 1] [9-tick];
		      XCopyPlane (dpy, p, pixmap, pixmap_draw_gc, 0, 0,
				  character_width, character_height,
				  x_offsets [j], 0, 1);
		    }
		  /* This case isn't terribly common, but we've got it cached,
		     so why not use it. */
		  else if (be_a_pig && time_digits [j] >= 0 &&
			   last_time_digits [j] == time_digits [j] + 1)
		    {
		      Pixmap p =
			memory_pig_digits [time_digits [j] - 1] [tick];
		      XCopyPlane (dpy, p, pixmap, pixmap_draw_gc, 0, 0,
				  character_width, character_height,
				  x_offsets [j], 0, 1);
		    }

		  else if (be_a_pig > 1 && last_time_digits [j] >= 0 &&
			   time_digits [j] == ((last_time_digits [j] + 2)
					       % 10))
		    {
		      Pixmap p =
			total_oinker_digits [last_time_digits[j] - 1] [9-tick];
		      XCopyPlane (dpy, p, pixmap, pixmap_draw_gc, 0, 0,
				  character_width, character_height,
				  x_offsets [j], 0, 1);
		    }
		  else if (be_a_pig > 1 && time_digits [j] >= 0 &&
			   last_time_digits [j] == ((time_digits [j] + 2)
						    % 10))
		    {
		      Pixmap p =
			total_oinker_digits [time_digits [j] - 1] [tick];
		      XCopyPlane (dpy, p, pixmap, pixmap_draw_gc, 0, 0,
				  character_width, character_height,
				  x_offsets [j], 0, 1);
		    }
		  else
		    {
#if 0
		      if (be_a_pig && tick == 9)
			fprintf (stderr, "cache miss!  %d -> %d\n",
				 last_time_digits [j], time_digits [j]);
#endif
		      /* sends 20 bytes */
		      XFillRectangle (dpy, pixmap, pixmap_erase_gc,
				      x_offsets [j], 0, character_width + 1,
				      character_height);
		      draw_frame (dpy, current_frames [j],
				  pixmap, pixmap_draw_gc, x_offsets [j]);
		    }
		  if (tick)
		    one_step (current_frames[j], target_frames [j], tick);
		}
	    }
	  /* sends up to 1k in non-pig mode */
	  flush_segment_buffer (dpy, pixmap, pixmap_draw_gc);

#ifdef SHAPE
	  if (do_shape)
	    XShapeCombineMask (dpy, window, ShapeBounding,
			       0, 0, pixmap, ShapeSet);
	  else
#endif
	    /* sends 28 bytes */
	    XCopyPlane (dpy, pixmap, window, window_draw_gc, 0, 0,
			x_offsets [ndigits-1] + character_width,
			character_height,
			window_offset_x + (wander_x - (character_width / 2)),
			window_offset_y + (wander_y - (character_height / 2)),
			1);
	  XFlush (dpy);
	  TICK_SLEEP ();
	}
      memcpy (last_time_digits, time_digits, sizeof (time_digits));

      /* sends up to 1k in non-pig mode */
      flush_segment_buffer (dpy, pixmap, pixmap_draw_gc);

#ifdef SHAPE
      if (do_shape)
	XShapeCombineMask (dpy, window, ShapeBounding, 0, 0,
			   pixmap, ShapeSet);
      else
#endif

	if (wander_p && different_minute
#ifdef SHAPE
	    && !do_shape
#endif
	    )
	  {
	    XClearWindow (dpy, window);
	    wander ();
	  }

      /* sends 28 bytes */
      XCopyPlane (dpy, pixmap, window, window_draw_gc, 0, 0,
		  x_offsets [ndigits-1] + character_width, character_height,
		  window_offset_x + (wander_x - (character_width / 2)),
		  window_offset_y + (wander_y - (character_height / 2)),
		  1);

    if (minutes_only)
      {
	/* This is slightly sleazy: when in no-seconds mode, wake up
	   once a second to cycle colors and poll for events, until the
	   minute has expired.  We could do this with an interrupt timer
	   or by select()ing on the display file descriptor, but that
	   would be more work.
	 */
	long now = time ((time_t *) 0);
	struct tm *tm = localtime (&now);
	long target = now + (60 - tm->tm_sec);
	while ((now = time ((time_t *) 0)) <= target)
	  {
	    /* if event_loop returns true, we need to go and repaint stuff
	       right now, instead of waiting for the minute to elapse.
	       */
	    if (event_loop (dpy, window))
	      break;

	    if (now == target-1)	/* the home stretch; sync up */
	      TICK_SLEEP ();
	    else
	      {
		if (do_cycle)
		  cycle_colors (dpy, xgwa.colormap, &fg_color, &bg_color);
		SEC_SLEEP ();
	      }
	  }
      }
    else
      {
	/* Sync to the second-strobe by repeatedly sleeping for about 1/10th
	   second until time() returns a different value.  This is so that
	   multiple instances of this program run in lock-step instead of
	   being randomly staggered by up to a second, depending on when they
	   were started, and scheduling delays.  (It's the details like this
	   that make the difference between a True Hack and just another app.)
	 */
	long now = clock;
	while (clock == now)
	  {
	    TICK_SLEEP ();
	    now = time ((time_t *) 0);
	    event_loop (dpy, window);
	  }
      }
    }
}


static int
event_loop (dpy, window)
     Display *dpy;
     Window window;
{
  static int mapped_p = 0;
  int wait_for_buttonup = button_down;
  int redraw_p = 0;
  XSync (dpy, False);
  /* #### */
  while (XPending (dpy) || !(mapped_p || root_p) ||
	 (wait_for_buttonup && button_down))
    {
      XEvent event;
      XNextEvent (dpy, &event);
      switch (event.xany.type)
	{
	case KeyPress:
	  {
	    KeySym keysym;
	    XComposeStatus status;
	    char buffer [10];
	    int nbytes = XLookupString (&event.xkey, buffer, sizeof (buffer),
					&keysym, &status);
	    if (nbytes == 0) break;
	    if (nbytes != 1) buffer [0] = 0;
	    switch (buffer [0]) {
	    case 'q': case 'Q': case 3:
#ifdef VMS
	      exit (1);
#else
	      exit (0);
#endif
	    case ' ':
	      twelve_hour_time = !twelve_hour_time;
	      if (twelve_hour_time &&
		  time_digits [0] == 0 &&
		  time_digits [1] != 0)
		last_time_digits [0] = -2; /* evil hack */
	      redraw_p = 1;
	      break;
	    case '0': case '1': case '2': case '3': case '4': case '5':
	    case '6': case '7': case '8': case '9': case '-':
	      if (event.xkey.state)
		{
		  test_hack = buffer [0];
		  redraw_p = 1;
		  break;
		}
	    default:
	      XBell (dpy, 0);
	      break;
	    }
	  }
	  break;
	case ButtonPress:
	  button_down |= (1<<event.xbutton.button);
	  if (! wait_for_buttonup) draw_colon (dpy, pixmap, window);
	  redraw_p = 1;
	  break;
	case ButtonRelease:
	  button_down &= ~(1<<event.xbutton.button);
	  if (! button_down) draw_colon (dpy, pixmap, window);
	  redraw_p = 1;
	  break;
	case ConfigureNotify:
	  {
	    int width = x_offsets [minutes_only ? 3 : 5] + character_width;
#ifdef SHAPE
	    if (do_shape) break;
#endif
	    window_offset_x = (event.xconfigure.width - width) / 2;
	    window_offset_y = (event.xconfigure.height - character_height) / 2;
	    XClearWindow (dpy, window);
	    redraw_p = 1;
	  }
	  break;
	case Expose:
	case MapNotify:
	  mapped_p = 1;
	  redraw_p = 1;
	  break;
	case UnmapNotify:
	  mapped_p = 0;
	  break;
	}
    }
  return redraw_p;
}

