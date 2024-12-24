#ifndef lint
static char *rcsid_orbit_c = "$Header: orbit.c,v 10.1 86/11/30 15:26:24 jg Rel $";
#endif	lint
/* Copyright (c) 1986 	Chris Guthrie */

#include <X/Xlib.h>

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "defs.h"
#include "struct.h"
#include "data.h"
#include "sintab.h"

/* orbit a planet */

orbit()
{
    register int i;
    register struct planet *l;
    unsigned char dir;
    int dx, dy;

    if (me->p_speed > ORBSPEED) {
	warning("Speed must be less than two to orbit");
	return;
    }
    for (i = 0, l = &planets[i]; i < MAXPLANETS; i++, l++) {
	dx = ABS(l->pl_x - me->p_x);
	dy = ABS(l->pl_y - me->p_y);
	if (dx > ORBDIST || dy > ORBDIST)		/*XXX*/
	    continue;
	if (dx * dx + dy * dy > ORBDIST * ORBDIST)
	    continue;

	dir = (unsigned char) (atan2((double) (l->pl_x - me->p_x),
	    (double) (l->pl_y - me->p_y))
	    / 3.14159 * 128.);
	l->pl_info |= me->p_team;
	me->p_dir = dir + 64;
	me->p_flags |= PFORBIT;
	me->p_x = l->pl_x + ORBDIST * cos[dir];
	me->p_y = l->pl_y + ORBDIST * sin[dir];
	me->p_speed = me->p_desspeed = 0;
	me->p_planet = l->pl_no;
	return;
    }
    warning("Not close enough to any planet to orbit");
}
