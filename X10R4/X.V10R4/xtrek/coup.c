#ifndef lint
static char *rcsid_coup_c = "$Header: coup.c,v 10.1 86/11/30 15:23:14 jg Rel $";
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

/* throw a coup */

coup()
{
    register int i;
    register struct planet *l;
    unsigned char dir;
    int dx, dy;

    if (me->p_kills < 1.0) {
	warning("You must have one kill to throw a coup");
	return;
    }
    if (!(me->p_flags & PFORBIT)) {
	warning("You must orbit your home planet to throw a coup");
	return;
    }
    for (i = 0, l = &planets[i]; i < MAXPLANETS; i++, l++) {
	if (l->pl_owner == me->p_team) {
	    warning("You already own a planet!!!");
	    return;
	}
    }
    l = &planets[me->p_planet];

    if ((!(l->pl_flags & PLHOME)) && ((l->pl_flags & ALLTEAM) != me->p_team)) {
	warning("You must orbit your home planet to throw a coup");
	return;
    }

    if (l->pl_armies > 4) {
	warning("Too many armies on planet to throw a coup");
	return;
    }

    /* the cases are now met.  We can have a coup. */

    l->pl_flags |= PLCOUP;
    mystats->st_coups++;
}
