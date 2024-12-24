#ifndef lint
static char *rcsid_death_c = "$Header: death.c,v 10.1 86/11/30 15:24:05 jg Rel $";
#endif	lint
/* Copyright (c) 1986 	Chris Guthrie */

#include <X/Xlib.h>
#include <stdio.h>
#include <signal.h>
#include <setjmp.h>
#include <sys/types.h>
#include <sys/time.h>
#include "defs.h"
#include "struct.h"
#include "data.h"

extern Font dfont;
extern Window w, mapw, statwin, baseWin;
extern int oldalert;
extern char teamlet[];
extern jmp_buf env;
struct itimerval udt;
double oldmax = 0;

death()
{
    char buf[80];
    XEvent foo;
    int mustexit = 0;

    /* Allow player to watch final updates */ 
    while (me->p_status == PEXPLODE)
	;
    while (me->p_ntorp > 0)
	;

    signal(SIGALRM, SIG_IGN);
    udt.it_interval.tv_sec = 0;
    udt.it_interval.tv_usec = 0;
    udt.it_value.tv_sec = 0;
    udt.it_value.tv_usec = 0;
    setitimer(ITIMER_REAL, &udt, 0);
    signal(SIGALRM, SIG_DFL);

    me->p_status = POUTFIT;		/* Stop the ghost buster */

    XClear(w);
    XChangeBorder(baseWin, gTile);
    if (showStats)
	closeStats(statwin);

    switch (me->p_whydead) {
	case KQUIT:
	    sprintf(buf, "You have self-destructed");
	    XText(w, 50, 250, buf, strlen(buf), dfont, textColor, backColor);
	    break;
	case KTORP:
	    sprintf(buf, "You were killed by a torp from %s (%c%x)",
		players[me->p_whodead].p_name,
		teamlet[players[me->p_whodead].p_team],
		me->p_whodead);
	    XText(w, 50, 250, buf, strlen(buf), dfont, textColor, backColor);
	    sprintf(buf, "who was %d%% damaged",
		players[me->p_whodead].p_damage);
	    XText(w, 50, 260, buf, strlen(buf), dfont, textColor, backColor);
	    break;
	case KPHASER:
	    sprintf(buf, "You were killed by a phaser shot from %s (%c%x)",
		players[me->p_whodead].p_name,
		teamlet[players[me->p_whodead].p_team],
		me->p_whodead);
	    XText(w, 50, 250, buf, strlen(buf), dfont, textColor, backColor);
	    sprintf(buf, "who was %d%% damaged",
		players[me->p_whodead].p_damage);
	    XText(w, 50, 260, buf, strlen(buf), dfont, textColor, backColor);
	    break;
	case KPLANET:
	    sprintf(buf, "You were killed by planetary fire from %s (%c)",
		planets[me->p_whodead].pl_name,
		teamlet[planets[me->p_whodead].pl_owner]);
	    XText(w, 50, 250, buf, strlen(buf), dfont, textColor, backColor);
	    break;
	case KSHIP:
	    sprintf(buf, "You were killed by an exploding ship formerly owned by %s (%c%x)",
		players[me->p_whodead].p_name,
		teamlet[players[me->p_whodead].p_team],
		me->p_whodead);
	    XText(w, 50, 250, buf, strlen(buf), dfont, textColor, backColor);
	    sprintf(buf, "who was %d%% damaged",
		players[me->p_whodead].p_damage);
	    XText(w, 50, 260, buf, strlen(buf), dfont, textColor, backColor);
	    break;
	case KDAEMON:
	    sprintf(buf, "You were killed by a dying daemon");
	    XText(w, 50, 250, buf, strlen(buf), dfont, textColor, backColor);
	    mustexit = 1;
	    break;
	case KWINNER:
	    sprintf(buf, "Galaxy has been conquered by %s (%c%x)",
		players[me->p_whodead].p_name,
		teamlet[players[me->p_whodead].p_team],
		players[me->p_whodead].p_no);
	    XText(w, 50, 250, buf, strlen(buf), dfont, textColor, backColor);
	    mustexit = 1;
	    break;
	default:
	    sprintf(buf, "You were killed by something unknown to this game?");
	    XText(w, 50, 250, buf, strlen(buf), dfont, textColor, backColor);
	    break;
	}
    if (oldmax < me->p_kills)
	oldmax = me->p_kills;
    mystats->st_kills += me->p_kills;
    /* the exit routine in 'entryWindow' handles reentry now */
    XSync(1);
    longjmp(env, 0);
}
