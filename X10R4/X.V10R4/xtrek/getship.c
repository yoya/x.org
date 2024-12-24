#ifndef lint
static char *rcsid_getship_c = "$Header: getship.c,v 10.1 86/11/30 15:24:55 jg Rel $";
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

/* fill in ship characteristics */

getship(shipp)
struct ship *shipp;
{
    shipp->s_turns = 64000;
    shipp->s_accint = 100;
    shipp->s_decint = 200;
    shipp->s_accs = 100;
    shipp->s_torpdamage = 30;
    shipp->s_phaserdamage = 100;
    shipp->s_torpspeed = 12;
    shipp->s_maxspeed = 9;
    shipp->s_repair = 100;
    shipp->s_maxfuel = 10000;
    shipp->s_detcost = 100;
    shipp->s_torpcost = 10 * shipp->s_torpdamage;
    shipp->s_phasercost = 10 * shipp->s_phaserdamage;
    shipp->s_warpcost = 2;
    shipp->s_cloakcost = 30;
    shipp->s_recharge = 10;
    shipp->s_maxarmies = 10;
}
