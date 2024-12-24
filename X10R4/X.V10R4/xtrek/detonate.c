#ifndef lint
static char *rcsid_detonate_c = "$Header: detonate.c,v 10.1 86/11/30 15:24:16 jg Rel $";
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

/* Detonate torp */

detmine()
{
    register int i;
    register struct torp *j;

    if (me->p_flags & PFWEP) {
	warning("Weapons overheated");
	return;
    }
    for (i = 0; i < MAXTORP; i++, j++) {
	if (torps[i + (me->p_no * MAXTORP)].t_status == TMOVE) {
	    torps[i + (me->p_no * MAXTORP)].t_status = TOFF;
	}
	else if (torps[i + (me->p_no * MAXTORP)].t_status == TSTRAIGHT) {
	    torps[i + (me->p_no * MAXTORP)].t_status = TOFF;
	}
    }
}

detothers()
{
    register int h, i;
    int dx, dy;
    register struct torp *j;

    if (me->p_fuel < myship->s_detcost) {
	warning("Not enough fuel to detonate");
	return;
    }
    if (me->p_flags & PFWEP) {
	warning("Weapons overheated");
	return;
    }
    me->p_fuel -= myship->s_detcost;
    me->p_wtemp += myship->s_detcost / 5;

    for (h = 0; h < MAXPLAYER; h++) {
	if ((players[h].p_status == PFREE) || (h == me->p_no))
	    continue;
	for (i = h * MAXTORP; i < MAXTORP * (h + 1); i++) {
	    j = &torps[i];
	    if ((j->t_status == TMOVE) || (j->t_status == TSTRAIGHT)) {
		dx = j->t_x - me->p_x;
		dy = j->t_y - me->p_y;
		if (ABS(dx) > DETDIST || ABS(dy) > DETDIST) /* XXX */
		    continue;
		if (dx * dx + dy * dy < DETDIST * DETDIST)
		    j->t_status = TDET;
	    }
	}
    }
}
