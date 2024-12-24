#ifndef lint
static char *rcsid_main_c = "$Header: main.c,v 10.1 86/11/30 15:26:02 jg Rel $";
#endif	lint
/* Copyright (c) 1986 	Chris Guthrie */

#include <X/Xlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <signal.h>
#include <setjmp.h>
#include "defs.h"
#include "struct.h"
#include "data.h"

struct itimerval udt;
jmp_buf env;
long stime;
int redrawall;
extern Window w, mapw, statwin;
int copilot = 0;
int nopilot = 1;

main(argc, argv)
int argc;
char **argv;
{
    int intrupt();
    int team;
    int pno;
    char str[80];
    char *host;
    register int i;

    if (argc < 2) {
	host = NULL;
    }
    else 
    {
	host = argv[1];
    }
    srandom(getpid() * time((long *) 0));
    /* this creates the necessary x windows for the game */
    newwin(host);
    /* this finds the shared memory information plus a player slot */
    openmem();
    pno = findslot();
    if (pno < 0) {
	/* print some appropriate message */
	exit(1);
    }
    setjmp(env);		/* Reentry point of game */
    /* give the player the motd and find out which team he wants */
    team = entrywindow();
    if (team == -1) {
	if (me != NULL)
	    savestats();
	players[pno].p_status = PFREE;
	exit(0);
    }
    time(&stime);		/* For stat gathering */
    redrawall = 1;
    enter(team, host, pno);
    XClear(w);
    if (showStats)
	statwin = openStats(me);
    /*
    for (i = 0; i < NSIG; i++) {
	signal(i, SIG_IGN);
    }
    */
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    udt.it_interval.tv_sec = 0;
    udt.it_interval.tv_usec = 200000;
    udt.it_value.tv_sec = 1;
    udt.it_value.tv_usec = 0;
    setitimer(ITIMER_REAL, &udt, 0);

    me->p_status = PALIVE;			/* Put player in game */
    /* Get input until the player quits or dies */
    input();

    me->p_status = PFREE;
}
