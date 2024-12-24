#ifndef lint
static char *rcsid_phaser_c = "$Header: phaser.c,v 10.1 86/11/30 15:26:33 jg Rel $";
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

phaser(course)
unsigned char course;
{
    register int i;
    register struct player *j, *target;
    register struct phaser *mine;
    unsigned char dir;
    int range, trange;
    char buf[80];

    mine = &phasers[me->p_no];

    if (mine->ph_status != PHFREE) {
	warning("Phasers have not recharged");
	return;
    }
    if (me->p_fuel < 100) {
	warning("Not enough fuel for phaser");
	return;
    }
    if (me->p_flags & PFREPAIR) {
	warning("Can't fire while repairing");
	return;
    }
    if (me->p_flags & PFWEP) {
	warning("Weapons overheated");
	return;
    }
    if (me->p_flags & PFCLOAK) {
	warning("Cannot fire while cloaked");
	return;
    }

    me->p_fuel -= myship->s_phasercost;
    me->p_wtemp += myship->s_phasercost / 10;
    target = (struct player *) 0;
    mine->ph_dir = course;
    for (i = 0, j = &players[i]; i < MAXPLAYER; i++, j++) {
	if ((j->p_status != PALIVE) || (j == me))
	    continue;
	dir = (unsigned char) (atan2((double) (j->p_x - me->p_x),
	    (double) (me->p_y - j->p_y))
	    / 3.14159 * 128.);
	if (angdist(dir, course) < 5) {
	    trange = (int) hypot((double) (j->p_x - me->p_x),
		(double) (j->p_y - me->p_y));
	    if (target == 0) {
		target = j;
		range = trange;
	    }
	    else if (range > trange) {
		target = j;
		range = trange;
	    }
	}
    }
    if ((target == 0) || (range > PHASEDIST)) {
	mine->ph_fuse = 10;
	mine->ph_status = PHMISS;
	warning("Phaser missed!!!");
    }
    else {
	mine->ph_fuse = 10;
	mine->ph_target = target->p_no;
	mine->ph_damage = (PHASEDIST- range) * 100 / PHASEDIST;
	mine->ph_status = PHHIT;
	sprintf(buf, "Phaser hit %s for %d points",
	    target->p_name,
	    mine->ph_damage);
	warning(buf);
    }
    mystats->st_phasers++;
}
