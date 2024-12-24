#ifndef lint
static char *rcsid_copilot_c = "$Header: copilot.c,v 10.1 86/11/30 15:23:01 jg Rel $";
#endif	lint
/* Copyright (c) 1986 	Chris Guthrie */

#include <X/Xlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <signal.h>
#include <setjmp.h>
#include <pwd.h>
#include "defs.h"
#include "struct.h"
#include "data.h"

struct itimerval udt;
jmp_buf env;
long stime;
int redrawall;
extern Window w, mapw, statwin;
extern int lastm;
int copilot = 1;

main(argc, argv)
int argc;
char **argv;
{
    int intrupt();
    int team;
    int pno;
    char buf[80];
    char *host;
    register int i;
    struct passwd	*pwent;

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
    pno = atoi(argv[2]);
    if (pno < 0) {
	/* print some appropriate message */
	exit(1);
    }
    setjmp(env);		/* Reentry point of game */
    if (players[pno].p_status != PALIVE)
	exit(1);
    if (!(players[pno].p_flags & PFCOPILOT)) {
	printf("%s is not allowing copilots\n", players[pno].p_name);
	exit(1);
    }
    time(&stime);		/* For stat gathering */
    redrawall = 1;
    /*
    enter(team, host, pno);
    */
    me = &players[pno];
    if (showStats)
	statwin = openStats(me);
    myship = &me->p_ship;
    mystats = &me->p_stats;
    lastm = mctl->mc_current;

    if ((pwent = getpwuid(getuid())) == NULL)
	sprintf(buf, "Unknown person joining as a copilot");
    else
	sprintf(buf, "%-8s joining as a copilot", pwent->pw_name);
    
    pmessage(buf, pno, "MINDIV", "GOD->YOU");

    XClear(w);
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
    initinput();
    input();

    me->p_status = PFREE;
}
