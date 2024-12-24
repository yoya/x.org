#ifndef lint
static char *rcsid_util_c = "$Header: util.c,v 10.1 86/11/30 15:29:28 jg Rel $";
#endif	lint
/* Copyright (c) 1986 	Chris Guthrie */

#include <X/Xlib.h>
#include <stdio.h>
#include <math.h>
#include <signal.h>
#include "defs.h"
#include "struct.h"
#include "data.h"

angdist(x, y)
{
    register int res;

    res = ABS(x - y);
    if (res > 128)
	res = 256 - res;
    return(res);
}

#define WINSIDE 500

/* Find the object nearest mouse.  Returns a pointer to an
   obtype structure.  This is used for info and locking on */

/* Because we are never interested in it, this function will
   never return your own ship as the target. */

extern Window mapw, w;
struct obtype _target;

struct obtype *
gettarget(ww, x, y)
Window ww;
int x, y;
{
    register int i;
    register struct player *j;
    register struct planet *k;
    int	g_x, g_y;
    double dist, closedist;

    if (ww == mapw) {
	g_x = x * GWIDTH / WINSIDE;
	g_y = y * GWIDTH / WINSIDE;
    }
    else {
	g_x = me->p_x + ((x - WINSIDE/2) * SCALE);
	g_y = me->p_y + ((y - WINSIDE/2) * SCALE);
    }
    closedist = GWIDTH;

    for (i = 0, k = &planets[i]; i < MAXPLANETS; i++, k++) {
	dist = hypot((double) (g_x - k->pl_x), (double) (g_y - k->pl_y));
	if (dist < closedist) {
	    _target.o_type = PLANETTYPE;
	    _target.o_num = i;
	    closedist = dist;
	}

    }

    for (i = 0, j = &players[i]; i < MAXPLAYER; i++, j++) {
	if ((j->p_status != PALIVE) || (j->p_flags & PFCLOAK))
	    continue;
	if (j == me)
	    continue;
	dist = hypot((double) (g_x - j->p_x), (double) (g_y - j->p_y));
	if (dist < closedist) {
	    _target.o_type = PLAYERTYPE;
	    _target.o_num = i;
	    closedist = dist;
	}
    }

    return(&_target);
}
