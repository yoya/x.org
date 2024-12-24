#ifndef lint
static char *rcsid_planetlist_c = "$Header: planetlist.c,v 10.1 86/11/30 15:26:55 jg Rel $";
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

Window planetw;
extern Window w;
extern int remap[];

char *teamname[9] = {
    "",
    "FED",
    "ROM",
    "",
    "KLI",
    "",
    "",
    "",
    "ORI"
};

planetlist()
{
    register int i;
    register int k = 0;
    char buf[BUFSIZ];
    register struct planet *j;
    XEvent foo;

    planetw = XCreateWindow(w, 10, 10,
	47 * dfontinfo->width,
	(MAXPLANETS + 3) * dfontinfo->height,
	2, foreTile, backTile);
    XMapWindow(planetw);
    sprintf(buf, "  # Planet Name      own armies REPAIR FUEL");
    XText(planetw, 0, dfontinfo->height, buf,
       strlen(buf), dfont, textColor, backColor);
    k = 2;
    for (i = 0, j = &planets[i]; i < MAXPLANETS; i++, j++) {
	if (j->pl_info & me->p_team) {
	    sprintf(buf, " %2d %-16s %3s %3d    %6s %4s",
		j->pl_no,
		j->pl_name,
		teamname[j->pl_owner],
		j->pl_armies,
		(j->pl_flags & PLREPAIR ? "REPAIR" : "      "),
		(j->pl_flags & PLFUEL ? "FUEL" : "    "));
	    XText(planetw, 0, dfontinfo->height * k++, buf, strlen(buf),
	       dfont, planetColor(j), backColor);
	}
	else {
	    sprintf(buf, " %2d %-16s",
		j->pl_no,
		j->pl_name);
	    XText(planetw, 0, dfontinfo->height * k++, buf, strlen(buf),
	       dfont, unColor, backColor);
	}

	XFlush();
    }
    do {
	XNextEvent(&foo);
    } while (foo.type != ButtonPressed && foo.type != KeyPressed);
    XDestroyWindow(planetw);
}
