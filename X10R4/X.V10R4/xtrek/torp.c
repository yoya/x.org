#ifndef lint
static char *rcsid_torp_c = "$Header: torp.c,v 10.1 86/11/30 15:29:20 jg Rel $";
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

/* Launch torp */

ntorp(course, type)
unsigned char course;
int type;
{
    register int i;
    register struct torp *k;

    if (me->p_ntorp == MAXTORP) {
	warning("Torps limited to 12");
	return;
    }
    if (me->p_fuel < myship->s_torpcost) {
	warning("Not enough fuel for torp");
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
    me->p_ntorp++;
    me->p_fuel -= myship->s_torpcost;
    me->p_wtemp += myship->s_torpcost / 10;
    for (i = me->p_no * MAXTORP, k = &torps[i];
	i < me->p_no * MAXTORP + MAXTORP; i++, k++) {
	    if (k->t_status == TFREE)
		break;
    }
    k->t_no = i;
    k->t_status = type;
    k->t_owner = me->p_no;
    k->t_x = me->p_x;
    k->t_y = me->p_y;
    k->t_dir = course;
    k->t_damage = myship->s_torpdamage;
    k->t_speed = myship->s_torpspeed;
    k->t_war = me->p_war | me->p_swar;
    k->t_fuse = (random() % 60) + 30;
    mystats->st_torps++;
}
