#include <X/Xlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <pwd.h>
#include "../defs.h"
#include "../struct.h"
#include "../data.h"

/* Enter the game */

int pshmid;
int tshmid;
int stshmid;
int plshmid;
int phshmid;

enter(pno)
int pno;
{
    me = &players[pno];
    myship = &me->p_ship;
    mystats = &me->p_stats;
}

findslot()
{
    extern int errno;

    errno = 0;
    pshmid = shmget(PKEY, 0, 0);
    if (pshmid < 0) {
	exit(1);
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
}
