#ifndef lint
static char *rcsid_inform_c = "$Header: inform.c,v 10.1 86/11/30 15:25:12 jg Rel $";
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

#define WINSIDE 500

Window infow;
extern Window w, mapw;

extern char teamlet[];
extern int remap[];

/* Display information about the nearest objext to mouse */

inform(ww, x, y)
Window ww;
int x, y;
{
    register int i;
    char buf[BUFSIZ];
    int line = 0;
    register struct player *j;
    register struct planet *k;
    XEvent foo;
    int mx, my;
    double dist;
    Window subw;
    struct obtype *gettarget(), *target;

    target = gettarget(ww, x, y);

    XQueryMouse(RootWindow, &mx, &my, &subw);

    if (target->o_type == PLAYERTYPE) {
	/* Too close to the edge? */
	if (mx + 23 * dfontinfo->width + 2 > 1024)
	    mx = 1024 - 23 * dfontinfo->width - 2;
	if (my + 7 * dfontinfo->height + 2 > 864)
	    my = 864 - 7 * dfontinfo->height - 2;

	infow = XCreateWindow(RootWindow, mx, my, 23 * dfontinfo->width,
	    5 * dfontinfo->height, 2, foreTile, backTile);
	XMapWindow(infow);
	XSelectInput(infow, KeyPressed|ButtonPressed);
	j = &players[target->o_num];
	dist = hypot((double) (me->p_x - j->p_x),
	    (double) (me->p_y - j->p_y));
	sprintf(buf, "%s (%c%1x):", j->p_name, teamlet[j->p_team], j->p_no);
	XText(infow, dfontinfo->width, dfontinfo->height * line++, buf,
	    strlen(buf), dfont, textColor, backColor);
	sprintf(buf, "Login   %-s", j->p_login);
	XText(infow, dfontinfo->width, dfontinfo->height * line++, buf,
	    strlen(buf), dfont, textColor, backColor);
	sprintf(buf, "Speed   %-d", j->p_speed);
	XText(infow, dfontinfo->width, dfontinfo->height * line++, buf,
	    strlen(buf), dfont, textColor, backColor);
	/*
	sprintf(buf, "Damage  %-d", j->p_damage);
	XText(infow, dfontinfo->width, dfontinfo->height * line++, buf,
	    strlen(buf), dfont, textColor, backColor);
	sprintf(buf, "Shields %-d", j->p_shield);
	XText(infow, dfontinfo->width, dfontinfo->height * line++, buf,
	    strlen(buf), dfont, textColor, backColor);
	*/
	sprintf(buf, "kills   %-4.2f", j->p_kills);
	XText(infow, dfontinfo->width, dfontinfo->height * line++, buf,
	    strlen(buf), dfont, textColor, backColor);
	sprintf(buf, "dist    %-d", (int) dist);
	XText(infow, dfontinfo->width, dfontinfo->height * line++, buf,
	    strlen(buf), dfont, textColor, backColor);
    }
    else {
	/* Too close to the edge? */
	if (mx + 20 * dfontinfo->width + 2 > 1024)
	    mx = 1024 - 20 * dfontinfo->width - 2;
	if (my + 3 * dfontinfo->height + 2 > 864)
	    my = 864 - 3 * dfontinfo->height - 2;

	infow = XCreateWindow(RootWindow, mx, my, 20 * dfontinfo->width,
	    3 * dfontinfo->height, 2, foreTile, backTile);
	XMapWindow(infow);
	XSelectInput(infow, KeyPressed|ButtonPressed);
	k = &planets[target->o_num];
	dist = hypot((double) (me->p_x - k->pl_x),
	    (double) (me->p_y - k->pl_y));
	if (k->pl_info & me->p_team) {
	    sprintf(buf, "%s (%c)", k->pl_name, teamlet[k->pl_owner]);
	    XText(infow, dfontinfo->width, dfontinfo->height * line++, buf,
		strlen(buf), dfont, planetColor(k), backColor);
	    sprintf(buf, "Armies %d", k->pl_armies);
	    XText(infow, dfontinfo->width, dfontinfo->height * line++, buf,
		strlen(buf), dfont, planetColor(k), backColor);
	    sprintf(buf, "%s %s",
		(k->pl_flags & PLREPAIR ? "REPAIR" : "      "),
		(k->pl_flags & PLFUEL ? "FUEL" : "    "));
	    XText(infow, dfontinfo->width, dfontinfo->height * line++, buf,
		strlen(buf), dfont, planetColor(k), backColor);
	}
	else {
	    sprintf(buf, "%s", k->pl_name);
	    XText(infow, dfontinfo->width, dfontinfo->height * line++, buf,
		strlen(buf), dfont, planetColor(k), backColor);
	    sprintf(buf, "No other info");
	    XText(infow, dfontinfo->width, dfontinfo->height * line++, buf,
		strlen(buf), dfont, planetColor(k), backColor);
	}
    }
    XFlush();
    do {
	XNextEvent(&foo);
    } while (foo.type != ButtonPressed && foo.type != KeyPressed);
    XDestroyWindow(infow);
}

