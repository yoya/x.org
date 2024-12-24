#ifndef lint
static char *rcsid_robot_c = "$Header: robot.c,v 10.1 86/11/30 15:28:01 jg Rel $";
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

Window mapw;				/* XXX */

struct itimerval udt;
int redrawall;
int lastm;

/* lots of neat flags */
int hostile;
int debug;
int easy;
int level;
int fleet;
int sticky;

char *rnames[4] = { "M5", "Colossus", "Guardian", "HAL"};

main(argc, argv)
int argc;
char **argv;
{
    register int i;
    int rmove();
    int team = -1;
    int bteam;
    int pno;
    char str[80];

    for( ; argc>1 && argv[1][0]=='-'; argc--,argv++) {
	switch(argv[1][1]) {
	    case 'e':
		fprintf(stderr, "-e is now replaced by -llevel\n");
		easy++;
		break;
	    case 'f':
		fleet++;
		break;
	    case 's':
		sticky++;
		break;
	    case 'd':
		debug++;
		break;
	    case 'h':
		hostile++;
		break;
	    case 'l':
		if (argv[1][2] != NULL)
		    level = atoi(&argv[1][2]);
		else
		    level = 0;
		break;
	    case 'T':
		switch (argv[1][2]) {
		    case 'f':
			team = 0;
			bteam = FED;
			break;
		    case 'r':
			team = 1;
			bteam = ROM;
			break;
		    case 'k':
			team = 2;
			bteam = KLI;
			break;
		    case 'o':
			team = 3;
			bteam = ORI;
			break;
		    default:
			fprintf(stderr, "Unknown team type.  Usage -Tx where x is [frko]\n");
			exit(1);
		}
		break;
	    default:
		fprintf(stderr, "Unknown option '%c'\n", argv[1][1]);
		exit(1);
	}


    }
    srandom(getpid() * time((long *) 0));
    if (team < 0 || team >= 4) {
	if (debug)
	    fprintf(stderr, "Choosing random team.\n");
	team = random() % 4;
    }
    openmem();
    pno = findslot();
    /* At this point we have memory set up.  If we aren't a fleet, we don't
       want to replace any other robots on this team, so we'll check the
       other players and get out if there are any on our team.
    */

    if (!fleet) {
	for (i = 0; i < MAXPLAYER; i++) {
	    if ((players[i].p_status == PALIVE) && (players[i].p_team == bteam)) {
		if (debug)
		    fprintf(stderr, "Galaxy already defended\n");
		players[pno].p_status = PFREE;
		exit(1);
	    }
	}
    }
    enter(team, "Nowhere", pno);
    signal(SIGALRM, rmove);
    me->p_flags |= PFROBOT|PFCOPILOT;		/* Mark as a robot */
    strncpy(me->p_name, rnames[team], strlen(rnames[team]));
    me->p_name[strlen(rnames[team])] = NULL;
    strncpy(me->p_login, "Robot", strlen("Robot"));
    me->p_login[strlen("Robot")] = NULL;
    myship->s_phasercost = 0;
    myship->s_torpcost = 0;
    if (easy) {
	udt.it_interval.tv_sec = 1;		/* Robots get to move 1/sec */
	udt.it_interval.tv_usec = 000000;
    }
    else {
	udt.it_interval.tv_sec = 0;		/* Robots get to move 2/sec */
	udt.it_interval.tv_usec = 500000;
    }
    udt.it_value.tv_sec = 1;
    udt.it_value.tv_usec = 0;
    if (setitimer(ITIMER_REAL, &udt, 0) < 0) {
	perror("setitimer");
	me->p_status = PFREE;		/* Put robot in game */
	exit(1);
    }
    /* allows robots to be forked by the daemon -- Evil ultrix bullshit */
    sigsetmask(0);

    me->p_status = PALIVE;		/* Put robot in game */
    while (1) {
	pause();
    }
}

warning(mess)
int mess;
{
    if (debug)
	fprintf(stderr, "warning: %s\n", mess);
}
