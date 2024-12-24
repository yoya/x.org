#ifndef lint
static char *rcsid_rmove_c = "$Header: rmove.c,v 10.1 86/11/30 15:27:49 jg Rel $";
#endif	lint
/* Copyright (c) 1986 	Chris Guthrie */

#include <X/Xlib.h>
#include <stdio.h>
#include <signal.h>
#include "defs.h"
#include "struct.h"
#include "data.h"
#include "sintab.h"

#define AVOID_TIME		4

#define STAY	0x1
#define RUN	0x2
#define ATTACK	0x3
#define REPAIR	0x4
#define AVOID	0x5

#define NORMALIZE(d) 		((((d) % 256) + 256) % 256)

struct player *enemy;
int edist;
unsigned char ecourse;
int rstatus;
int timer;
static int avoidTime;

extern int debug;
extern int hostile;
extern int easy;
extern int sticky;

double atan2();
double hypot();
unsigned char	getcourse();

rmove()
{
    register int i;
    register struct player *j;
    register int burst;
    register int numHits, tDir;
    int		avDir, found;

    double dx, dy;
    int tdist;
    unsigned char tcourse = 10;

    /* Check that I'm alive */
    if (me->p_status == PEXPLODE) {
	signal(SIGALRM, SIG_IGN);
	while (me->p_status == PEXPLODE)
	    ;
	while (me->p_ntorp > 0)
	    ;
	me->p_status = PFREE;
	exit(0);
    }
    if (me->p_status == PDEAD) {
	signal(SIGALRM, SIG_IGN);
	me->p_status = PFREE;
	exit(0);
    }

    /* Find an enemy */
    enemy = me;
    edist = GWIDTH + 1;
    for (i = 0, j = &players[i]; i < MAXPLAYER; i++, j++) {
	if ((j->p_status != PALIVE) || (j == me) ||
	    ((j->p_flags & PFROBOT) && (!hostile)))
	    continue;
	if ((j->p_swar | j->p_war) & me->p_team) {
	    /* We have an enemy */
	    /* Get his range */
	    dx = j->p_x - me->p_x;
	    dy = j->p_y - me->p_y;
	    tdist = hypot(dx, dy);
	    /* Check to see if ship is in our space. */
	    if (!hostile) {
		if (tdist > 15000) {
		/*
		if (1) {
		*/
		    switch (me->p_team) {
			case FED:
			    if ((j->p_x > GWIDTH/2) || (j->p_y < GWIDTH/2))
				continue;
			    break;
			case ROM:
			    if ((j->p_x > GWIDTH/2) || (j->p_y > GWIDTH/2))
				continue;
			    break;
			case KLI:
			    if ((j->p_x < GWIDTH/2) || (j->p_y > GWIDTH/2))
				continue;
			    break;
			case ORI:
			    if ((j->p_x < GWIDTH/2) || (j->p_y < GWIDTH/2))
				continue;
			    break;
		    }
		}
	    }
	    if (debug)
		fprintf(stderr, "%d) found enemy %d in our space at %d,%d\n",
		    me->p_no,
		    j->p_no,
		    j->p_x,
		    j->p_y);
	    if (tdist < edist) {
		enemy = j;
		edist = tdist;
		continue;
	    }
	}
    }
    if ((enemy == me) && (!sticky)) { 		/* No more enemies */
	if (timer == 0)
	    timer = me->p_updates + 600;
	if (me->p_updates >= timer) {
	    signal(SIGALRM, SIG_IGN);
	    me->p_status = PFREE;
	    exit(0);
	}
	go_home();
    }
    else {
	timer = 0;
    }

    /* Get course to nearest enemy */
    found = 0;
    for (i = 0; i < 50; i++) {
	double  he_x, he_y, area;

	he_x = enemy->p_x + cos[enemy->p_dir] * enemy->p_speed * i * WARP1;
	he_y = enemy->p_y + sin[enemy->p_dir] * enemy->p_speed * i * WARP1;
	area = i * me->p_ship.s_torpspeed * WARP1;
	if (hypot(he_x - me->p_x, he_y - me->p_y) < area) {
	    found = 1;
	    tcourse = getcourse((int) he_x, (int) he_y);
	    break;
	}
    }
    if (debug)
	fprintf(stderr, "torpedo course to enemy %d is %d (%d) - %s\n", 
		(int)tcourse, enemy->p_no, (int) tcourse * 360 / 256,
		found ? "aiming to hit" : "no hit possible");
    if (debug)
	fprintf(stderr, "Set course to enemy is %d (%d)\n", (int)ecourse, 
		(int) ecourse * 360 / 256);

    ecourse = getcourse(enemy->p_x, enemy->p_y);

    if ((edist > 40000) && !(hostile)) {
	if (me->p_damage > 0) {
	    me->p_desspeed = 0;
	    shield_down();
	    repair();
	    if (debug)
		fprintf(stderr, "%d) repairing damage at %d\n",
		    me->p_no,
		    me->p_damage);
	}
	else {
	    rstatus = STAY;
	    go_home();
	    if ((me->p_updates % 200) == 0)
		if (debug)
		    fprintf(stderr, "%d(%d) nearest enemy %c%d %16s (%d/%d) dist %d\n",
			me->p_no,
			(int) me->p_kills,
			teamlet[enemy->p_team],
			enemy->p_no,
			enemy->p_login,
			enemy->p_damage,
			enemy->p_shield,
			edist);
	}
	return;
    }
    if (me->p_damage > 0 && edist < 25000) {		/* Run away */
	me->p_desspeed = 6;
	if (!(me->p_flags & PFCLOAK))
		cloak();
	shield_down();
	me->p_desdir = ecourse - 128;
	if (debug)
	    fprintf(stderr, "%d(%d)(%d/%d) running from %c%d %16s damage (%d/%d) dist %d\n",
		me->p_no,
		(int) me->p_kills,
		me->p_damage,
		me->p_shield,
		teamlet[enemy->p_team],
		enemy->p_no,
		enemy->p_login,
		enemy->p_damage,
		enemy->p_shield,
		edist);
    } else if (rstatus == AVOID) {
	if (--avoidTime <= 0)
	    rstatus = ATTACK;
	if (debug)
	    fprintf(stderr, "avoiding: dir = %d\n", me->p_desdir);
    } else {
	rstatus = ATTACK;
	if (me->p_flags & PFCLOAK)
		cloak();
	if (debug)
	    fprintf(stderr, "%d(%d)(%d/%d) attacking %c%d %16s damage (%d/%d) dist %d\n",
		me->p_no,
		(int) me->p_kills,
		me->p_damage,
		me->p_shield,
		teamlet[enemy->p_team],
		enemy->p_no,
		enemy->p_login,
		enemy->p_damage,
		enemy->p_shield,
		edist);

	if (me->p_desdir != me->p_dir)
	    me->p_desspeed = 2;
	else if (me->p_etemp > 900)		/* 90% of 1000 */
	    me->p_desspeed = 5;
	else
	    me->p_desspeed = 5;

	shield_up();
	if (edist < 15000) {
	    numHits = projectDamage(enemy->p_no, &avDir);
	    if (debug) {
		fprintf(stderr, "%d hits expected from %d from dir = %d\n",
			numHits, enemy->p_no, avDir);
	    }
	    if (numHits == 0) {
		me->p_desdir = ecourse + 32;
		if (debug)
		    fprintf(stderr, "attacking: dir = %d\n", me->p_desdir);
	    } else {
		/*
		 * Avoid Torps
		 */ 
		avoidTime = AVOID_TIME;
		tDir = avDir - me->p_dir;
		/* put into 0->255 range */
		tDir = NORMALIZE(tDir);
		if (debug)
		    fprintf(stderr, "mydir = %d avDir = %d tDir = %d q = %d\n",
			me->p_dir, avDir, tDir, tDir / 64);
		switch (tDir / 64) {
		case 0:
		case 1:
			me->p_desdir = NORMALIZE(avDir + 64);
			break;
		case 2:
		case 3:
			me->p_desdir = NORMALIZE(avDir - 64);
			break;
		}
		if (!(me->p_flags & PFCLOAK))
			cloak();

		rstatus = AVOID;
		if (debug)
		    fprintf(stderr, "evading to dir = %d\n", me->p_desdir);
	    }
	} else {
	    me->p_desdir = ecourse;
	}
    }

    /* Fire weapons!!! */
    if (edist < 10000) {
	if (debug)
	    fprintf(stderr, "%d) firing torps\n", me->p_no);
	for (burst = 0; (burst < 2) && (me->p_ntorp < MAXTORP); burst++) {
	    if (me->p_flags & PFCLOAK)
		cloak();
	    if (easy)
		ntorp(tcourse, TMOVE);
	    else
		ntorp(tcourse, TSTRAIGHT);
	}
    }
    if ((edist < 5000) && (!easy)) {
	if (debug)
	    fprintf(stderr, "%d) phaser firing\n", me->p_no);
	if (me->p_flags & PFCLOAK)
	    cloak();
	phaser(ecourse);
    }
}

unsigned char
getcourse(x, y)
int x, y;
{
	return((unsigned char) (atan2((double) (x - me->p_x),
	    (double) (me->p_y - y)) / 3.14159 * 128.));
}

struct {
    int x;
    int y;
} center[] = { {0, 0},
		{GWIDTH / 4, GWIDTH * 3 / 4},		/* Fed */
		{GWIDTH / 4, GWIDTH / 4},		/* Rom */
		{0, 0},
		{GWIDTH * 3 / 4, GWIDTH  / 4},		/* Kli */
		{0, 0},
		{0, 0},
		{0, 0},
		{GWIDTH * 3 / 4, GWIDTH * 3 / 4}};	/* Ori */

/* This function will send the robot back to the center of it's
   space when it has nothing better to do.  Centers are defined
   above.
*/
go_home()
{
    int x, y, speed;
    double dx, dy;
    int tdist;

    x = center[me->p_team].x;
    y = center[me->p_team].y;
    if ((ABS(x - me->p_x) < 100) && (ABS(y - me->p_y) < 100))
	me->p_desspeed = 0;
    else {
	me->p_desdir = getcourse(x, y);
	dx = x - me->p_x;
	dy = y - me->p_y;
	me->p_desspeed = (hypot(dx, dy) / 10000) + 1;
    }
}

projectDamage(eNum, dirP)
	int	*dirP;
{
	register int		i, j, numHits = 0, mx, my, tx, ty, dx, dy;
	double			tdx, tdy, mdx, mdy;
	register struct torp	*t;

	*dirP = 0;
	for (i = 0, t = &torps[eNum * MAXTORP]; i < MAXTORP; i++, t++) {
		if (t->t_status == TFREE)
			continue;
		tx = t->t_x; ty = t->t_y;
		mx = me->p_x; my = me->p_y;
		tdx = (double) t->t_speed * cos[t->t_dir] * WARP1;
		tdy = (double) t->t_speed * sin[t->t_dir] * WARP1;
		mdx = (double) me->p_speed * cos[me->p_dir] * WARP1;
		mdy = (double) me->p_speed * sin[me->p_dir] * WARP1;
		for (j = t->t_fuse; j > 0; j--) {
			tx += tdx; ty += tdy;
			mx += mdx; my += mdy;
			dx = tx - mx; dy = ty - my;
			if (ABS(dx) < EXPDIST && ABS(dy) < EXPDIST) {
				numHits++;
				*dirP += t->t_dir;
				break;
			}
		}
	}
	if (numHits > 0)
		*dirP /= numHits;
	return (numHits);
}
