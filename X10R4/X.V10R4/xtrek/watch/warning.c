
#include <X/Xlib.h>
#include <stdio.h>
#include <math.h>
#include <signal.h>
#include "../defs.h"
#include "../struct.h"
#include "../data.h"
#include "../bitmaps.h"

extern Window w;
extern Font dfont;
extern FontInfo *dfontinfo;
extern int udcounter;

int warncount = 0;
int warntimer = -1;

warning(text)
char *text;
{
    warntimer = udcounter + 5;  /* 10 updates later the line will be cleared */
    if (warncount > 0)
	XPixSet(w, 50, 490, dfontinfo->width * warncount, dfontinfo->height,
	    backColor);
    warncount = strlen(text);
    XText(w, 50, 490, text, warncount, dfont, textColor, backColor);
    XFlush();
}
