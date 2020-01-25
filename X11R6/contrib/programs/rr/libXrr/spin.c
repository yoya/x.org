/*************************************************************************
 *                                                                       *
 *  Copyright (c) 1992, 1993 Ronald Joe Record                           *
 *                                                                       *
 *      Ronald Joe Record (408) 458-3718                                 *
 *      212 Owen St., Santa Cruz, California 95062 USA                   *
 *                                                                       *
 *************************************************************************/
 /*
 *  The X Consortium, and any party obtaining a copy of these files from
 *  the X Consortium, directly or indirectly, is granted, free of charge,
 *  a full and unrestricted irrevocable, world-wide, paid up, royalty-free,
 *  nonexclusive right and license to deal in this software and
 *  documentation files (the "Software"), including without limitation the
 *  rights to use, copy, modify, merge, publish, distribute, sublicense,
 *  and/or sell copies of the Software, and to permit persons who receive
 *  copies from any such party to do so.  This license includes without
 *  limitation a license to do the foregoing actions under any patents of
 *  the party supplying this software to the X Consortium.
 */

#include <X11/Xlib.h>

#ifdef USE_DELAY

#include <signal.h>
#include <sys/time.h>

static int timerdone;

static void onalarm() {
	timerdone = 1;
}

/*******/
void Timer(n)   /* waits for 'n' milliseconds (from xv-2.21 source) */
 int  n;
/*******/
{
  long usec;
#ifdef HAS_ITIMER
  struct itimerval it;
#endif

  if (!n) return;

  usec = (long) n * 1000;
  
#ifdef HAS_ITIMER
  memset(&it, 0, sizeof(it));
  if (usec>=1000000L) {  /* more than 1 second */
    it.it_value.tv_sec = usec / 1000000L;
    usec %= 1000000L;
  }

  it.it_value.tv_usec = usec;
#endif
  timerdone=0;
  signal(SIGALRM,onalarm);
#ifdef HAS_ITIMER
  setitimer(ITIMER_REAL, &it, (struct itimerval *)0);
  while (1) {
/*    sigblock(sigmask(SIGALRM));  note:  have to block, so that ALRM */
    if (timerdone) break;        /* doesn't occur between 'if (timerdone)' */
    else sigpause(0);            /* and calling sigpause(0) */
  }

/*  sigblock(0);                    turn ALRM blocking off */
  signal(SIGALRM,SIG_DFL);
#else
  nap((long)n);
#endif
}

#endif /*USE_DELAY */

void
Spin(display, colormap, colors, start_color, num_colors, delay, dir)
Display *display;
Colormap colormap;
XColor *colors;
int start_color, num_colors;
short dir, delay;
{
    static short i;
    static long tmpxcolor;

    while (!XPending(display)) {
        if (dir) {
            tmpxcolor = colors[start_color].pixel;
            for (i=start_color;i<num_colors-1;i++)
                colors[i].pixel = colors[i+1].pixel;
            colors[num_colors-1].pixel = tmpxcolor;
        }
        else {
            tmpxcolor = colors[num_colors-1].pixel;
            for (i=num_colors-1;i>start_color;i--)
                colors[i].pixel = colors[i-1].pixel;
            colors[start_color].pixel = tmpxcolor;
        }
        XStoreColors(display, colormap, colors, num_colors);
#ifdef USE_DELAY
	Timer(delay);
#endif
    }
}

void
DemoSpin(display, colormap, colors, start_color, num_colors, delay, factor)
Display *display;
Colormap colormap;
XColor *colors;
int start_color, num_colors;
short delay, factor;
{
    static short i, j;
    long tmpxcolor;

    for (j=0;j<factor*num_colors;j++) {
        tmpxcolor = colors[start_color].pixel;
        for (i=start_color;i<num_colors-1;i++)
            colors[i].pixel = colors[i+1].pixel;
        colors[num_colors-1].pixel = tmpxcolor;
        XStoreColors(display, colormap, colors, num_colors);
#ifdef USE_DELAY
	Timer(delay);
#endif
    }
    for (j=0;j<factor*num_colors;j++) {
        tmpxcolor = colors[num_colors-1].pixel;
        for (i=num_colors-1;i>start_color;i--)
            colors[i].pixel = colors[i-1].pixel;
        colors[start_color].pixel = tmpxcolor;
        XStoreColors(display, colormap, colors, num_colors);
#ifdef USE_DELAY
	Timer(delay);
#endif
    }
}
