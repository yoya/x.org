#ifndef lint
static char *rcsid_enter_c = "$Header: enter.c,v 10.1 86/11/30 15:24:36 jg Rel $";
#endif	lint
/* Copyright (c) 1986 	Chris Guthrie */

#include <X/Xlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <pwd.h>
#include "defs.h"
#include "struct.h"
#include "data.h"

/* Enter the game */

extern int lastm;
int pshmid;
int tshmid;
int stshmid;
int plshmid;
int phshmid;
int mshmid;

enter(tno, disp, pno)
int tno;
char *disp;
int pno;
{
    struct passwd	*pwent;
    char	 	*pseudo;

    me = &players[pno];
    if ((pwent = getpwuid(getuid())) == NULL)
	strncpy(me->p_login, "Bozo", sizeof (me->p_login));
    else
	strncpy(me->p_login, pwent->pw_name, sizeof (me->p_login));
    pseudo = XGetDefault("xtrek", "name");
    if (pseudo == NULL)
	strncpy(me->p_name, me->p_login, sizeof (me->p_name));
    else
	strncpy(me->p_name, pseudo, sizeof (me->p_name));
    me->p_no = pno;
    me->p_updates = 0;
    me->p_flags = PFSHIELD;
    strncpy(me->p_monitor, disp, 16);
    me->p_dir = 0;
    me->p_desdir = 0;
    me->p_speed = 0;
    me->p_desspeed = 0;
    me->p_subspeed = 0;
    me->p_team = (1 << tno);
    me->p_x = planets[tno * 10].pl_x + (random() % 10000) - 5000;
    me->p_y = planets[tno * 10].pl_y + (random() % 10000) - 5000;
    me->p_ntorp = 0;
    me->p_damage = 0;
    me->p_subdamage = 0;
    me->p_etemp = 0;
    me->p_etime = 0;
    me->p_wtemp = 0;
    me->p_wtime = 0;
    me->p_shield = 100;
    me->p_subshield = 0;
    me->p_swar = (FED|ROM|KLI|ORI);
    me->p_swar &= ~me->p_team;
    me->p_kills = 0.0;
    me->p_armies = 0;
    myship = &me->p_ship;
    getship(myship);
    mystats = &me->p_stats;
    mystats->st_entries++;
    me->p_fuel = myship->s_maxfuel;
    lastm = mctl->mc_current;

}
openmem()
{
    register int i;
    struct passwd *pwent;
    extern int errno;

    errno = 0;
    pshmid = shmget(PKEY, 0, 0);
    if (pshmid < 0) {
	if (errno != ENOENT) {
	    perror("shmget");
	    exit(1);
	}
	startdaemon();
	sleep(2);
	pshmid = shmget(PKEY, 0, 0);
	if (pshmid < 0) {
	    fprintf(stderr, "Daemon not running\n");
	    exit (1);
	}
    }

    players = (struct player *) shmat(pshmid, 0, 0);
    if (players < 0) {
	perror("shared memory");
	exit (1);
    }

    tshmid = shmget(TKEY, 0, 0);
    if (tshmid < 0) {
	fprintf(stderr, "Daemon not running\n");
	exit (1);
    }
    torps = (struct torp *) shmat(tshmid, 0, 0);
    if (torps < 0) {
	perror("shared memory");
	exit (1);
    }

    plshmid = shmget(PLKEY, 0, 0);
    if (plshmid < 0) {
	fprintf(stderr, "Daemon not running\n");
	exit (1);
    }
    planets = (struct planet *) shmat(plshmid, 0, 0);
    if (planets < 0) {
	perror("shared memory");
	exit (1);
    }

    phshmid = shmget(PHKEY, 0, 0);
    if (phshmid < 0) {
	fprintf(stderr, "Daemon not running\n");
	exit (1);
    }
    phasers = (struct phaser *) shmat(phshmid, 0, 0);
    if (phasers < 0) {
	perror("shared memory");
	exit (1);
    }

    mshmid = shmget(MKEY, 0, 0);
    if (mshmid < 0) {
	fprintf(stderr, "Daemon not running\n");
	exit (1);
    }
    mctl = (struct mctl *) shmat(mshmid, 0, 0);
    messages = (struct message *) ((int) mctl + sizeof(struct mctl));
    if (mctl < 0) {
	perror("shared memory");
	exit (1);
    }
}

findslot()
{
    register int i;

    for (i = 0; i < MAXPLAYER; i++) {
	if (players[i].p_status == PFREE) {	/* We have a free slot */
	    players[i].p_status = POUTFIT;	/* possible race code */
	    break;
	}
    }
    if (i == MAXPLAYER) {
	fprintf(stderr, "No more room in game\n");
	exit (0);
    }
    bzero(&players[i].p_stats, sizeof(struct stats));  /* Slight problem for
							  copilot */
    return(i);
}
