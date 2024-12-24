#ifndef lint
static char *rcsid_warning_c = "$Header: warning.c,v 10.1 86/11/30 15:29:40 jg Rel $";
#endif	lint
/* Copyright (c) 1986 	Chris Guthrie */


#include <X/Xlib.h>
#include <stdio.h>
#include <math.h>
#include <signal.h>
#include "defs.h"
#include "struct.h"
#include "data.h"
#include "bitmaps.h"

extern Window warnw;
extern int udcounter;

int warncount = 0;
int warntimer = -1;

warning(text)
char *text;
{
    warntimer = udcounter + 20;  /* 10 updates later the line will be cleared */
    if (warncount > 0)
	XPixSet(warnw, 5, 5, dfontinfo->width * warncount, dfontinfo->height,
	    backColor);
    warncount = strlen(text);
    XText(warnw, 5, 5, text, warncount, dfont, textColor, backColor);
    XFlush();
}
