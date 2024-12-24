#ifndef lint
static char *rcsid_pstats_c = "$Header: pstats.c,v 10.1 86/11/30 15:27:21 jg Rel $";
#endif	lint
/* Copyright (c) 1986 	Chris Guthrie */


#include <X/Xlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/file.h>
#include <signal.h>
#include <setjmp.h>
#include "defs.h"
#include "struct.h"
#include "data.h"

int fd;
struct stats old;
struct rusage buf;
long now;
extern long stime;
extern double oldmax;

savestats()
{
    fd = open(SCOREFILE, O_RDWR, 0777);
    if (fd < 0)
	return;
    lseek(fd, getuid() * sizeof(struct stats), 0);
    read(fd, &old, sizeof(struct stats));

    me->p_stats.st_kills += old.st_kills;
    me->p_stats.st_losses += old.st_losses;
    me->p_stats.st_entries += old.st_entries;
    me->p_stats.st_conqs += old.st_conqs;
    me->p_stats.st_coups += old.st_coups;
    me->p_stats.st_torps += old.st_torps;
    me->p_stats.st_phasers += old.st_phasers;
    me->p_stats.st_armsbomb += old.st_armsbomb;
    me->p_stats.st_armsship += old.st_armsship;
    me->p_stats.st_planets += old.st_planets;
    me->p_stats.st_genocides += old.st_genocides;

    time(&now);
    me->p_stats.st_time = old.st_time + (now - stime);

    getrusage(0, &buf);
    me->p_stats.st_cpu = old.st_cpu + buf.ru_utime.tv_sec + buf.ru_stime.tv_sec;

    if (oldmax > old.st_maxkills)
	mystats->st_maxkills = oldmax;
    else
	mystats->st_maxkills = old.st_maxkills;

    lseek(fd, getuid() * sizeof(struct stats), 0);
    write(fd, mystats, sizeof(struct stats));
}
