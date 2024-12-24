#ifndef lint
static char *rcsid_interface_c = "$Header: interface.c,v 10.1 86/11/30 15:25:49 jg Rel $";
#endif	lint

/* This file will include all the interfaces between the input routines
    and the daemon.  They should be useful for writing robots and the
    like */

/* Copyright (c) 1986 	Chris Guthrie */

#include <X/Xlib.h>
#include <stdio.h>
#include <math.h>
#include <signal.h>
#include "defs.h"
#include "struct.h"
#include "data.h"
#include "bitmaps.h"

set_speed(speed)
int speed;
{
    me->p_desspeed = speed;
    me->p_flags &= ~(PFREPAIR | PFBOMB | PFORBIT | PFBEAMUP | PFBEAMDOWN);
}

set_course(dir)
unsigned char dir;
{
    me->p_desdir = dir;
    me->p_flags &= ~(PFBOMB | PFORBIT | PFBEAMUP | PFBEAMDOWN);
}

shield_up()
{
    me->p_flags |= PFSHIELD;
    me->p_flags &= ~(PFBOMB | PFREPAIR | PFBEAMUP | PFBEAMDOWN);
}

shield_down()
{
    me->p_flags &= ~PFSHIELD;
}

shield_tog()
{
    me->p_flags ^= PFSHIELD;
    me->p_flags &= ~(PFBOMB | PFREPAIR | PFBEAMUP | PFBEAMDOWN);
}

bomb_planet()
{
    if (!(me->p_flags & PFORBIT)) {
	warning("Must be orbiting to bomb");
	return;
    }
    me->p_flags |= PFBOMB;
    me->p_flags &= ~(PFSHIELD | PFREPAIR | PFBEAMUP | PFBEAMDOWN);
}

beam_up()
{
    if (!(me->p_flags & PFORBIT)) {
	warning("Must be orbiting to beam up.");
	return;
    }
    if (me->p_team != planets[me->p_planet].pl_owner) {
	warning("Those aren't our men.");
	return;
    }
    me->p_flags |= PFBEAMUP;
    me->p_flags &= ~(PFSHIELD | PFREPAIR | PFBOMB | PFBEAMDOWN);
}

beam_down()
{
    if (!(me->p_flags & PFORBIT)) {
	warning("Must be orbiting to beam down.");
	return;
    }
    me->p_flags |= PFBEAMDOWN;
    me->p_flags &= ~(PFSHIELD | PFREPAIR | PFBOMB | PFBEAMUP);
}

repair()
{
    me->p_desspeed = 0;
    me->p_flags |= PFREPAIR;
    me->p_flags &= ~(PFSHIELD | PFBOMB);
}

/*
case 'Q':
    me->p_flags |= PFSELFDEST;
    selfdest = me->p_updates + 100;
    warning("Self destruct initiated");
    return;
*/

extern int lastm;
repeat_message()
{
    if (++lastm == MAXMESSAGE) ;
	lastm = 0;
}

cloak()
{
    me->p_flags ^= PFCLOAK;
}

