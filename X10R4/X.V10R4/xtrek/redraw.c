#ifndef lint
static char *rcsid_redraw_c = "$Header: redraw.c,v 10.1 86/11/30 15:27:33 jg Rel $";
#endif	lint
/* Copyright (c) 1986 	Chris Guthrie */

#include <X/Xlib.h>
#include <stdio.h>
#include <signal.h>
#include "defs.h"
#include "struct.h"
#include "data.h"
#include "bitmaps.h"

#define WINSIDE 500

int remap[9] = { -1, 0, 1, -1, 2, -1, -1, -1, 3 };

extern Window w, mapw, statwin, warnw, tstatw, baseWin;

char *shipnos = "0123456789abcdef";
extern char teamlet[];
extern double sin[], cos[], atan2(), hypot();

int clearzone[4][(MAXTORP + 1) * MAXPLAYER + MAXPLANETS];
int clearcount;
int clearline[4][MAXPLAYER];
int clearlcount;
int mclearzone[4][MAXPLAYER + MAXPLANETS];	/* For map window */
int mclearcount;
int udcounter;
extern int redrawall;
extern int warncount;
extern int warntimer;
extern int selfdest;

short nplayers;
extern copilot;

intrupt()
{
    signal(SIGALRM, SIG_IGN);
    udcounter++;

    if ((me->p_status == PDEAD) && (me->p_ntorp == 0))
	death();

    auto_features();
    redraw();
    signal(SIGALRM, intrupt);
}

redraw()
{

    /* erase warning line if necessary */
    if ((warntimer <= udcounter) && (warncount > 0)) {
	XPixSet(warnw, 5, 5, dfontinfo->width * warncount, dfontinfo->height,
	    backColor);
	warncount = 0;
    }

    while (clearcount) {
	clearcount--;
	XPixSet(w, clearzone[0][clearcount], clearzone[1][clearcount],
	    clearzone[2][clearcount], clearzone[3][clearcount],
	    backColor);
    }
    while (clearlcount) {
	clearlcount--;
	XLine(w, clearline[0][clearlcount], clearline[1][clearlcount],
	    clearline[2][clearlcount], clearline[3][clearlcount],
	    1, 1, backColor, GXcopy, AllPlanes);
    }

    if ((mapmode) && (udcounter % ((nplayers == 0) ? 1 : nplayers) == 0))
	map();

    if (udcounter % 20 == 0)
	dmessage();

    local();	/* redraw local window */

    stline();
}

int oldalert = PFGREEN;		/* Avoid changing more than we have to. */

local()
{
    char buf[80];
    register int i;
    register struct player *j;
    register struct torp *k;
    register struct planet *l;
    register struct phaser *php;

    int junk;
    int dx, dy;
    int view;

    /*
    robot();
    */

    /* Draw Planets */
    for (i = 0, l = &planets[i]; i < MAXPLANETS; i++, l++) {
	dx = l->pl_x - me->p_x;
	dy = l->pl_y - me->p_y;
	view = SCALE * WINSIDE / 2;
	if (dx > view || dx < -view || dy > view || dy < -view)
	    continue;
	dx = dx / SCALE + WINSIDE / 2;
	dy = dy / SCALE + WINSIDE / 2;
	XPixFill(w, dx - (planet_width/2), dy - (planet_height/2),
	    planet_width, planet_height,
	    planetColor(l), bplanet, GXcopy, AllPlanes);
	XText(w, dx - (planet_width/2), dy + (planet_height/2),
	    l->pl_name, strlen(l->pl_name), planetFont(l),
	    planetColor(l), backColor);

	clearzone[0][clearcount] = dx - (planet_width/2);
	clearzone[1][clearcount] = dy + (planet_height/2);
	clearzone[2][clearcount] = dfontinfo->width * strlen(l->pl_name);
	clearzone[3][clearcount] = dfontinfo->height;
	clearcount++;
	clearzone[0][clearcount] = dx - (planet_width/2);
	clearzone[1][clearcount] = dy - (planet_height/2);
	clearzone[2][clearcount] = planet_width;
	clearzone[3][clearcount] = planet_height;
	clearcount++;
    }

    /* Draw ships */
    nplayers = 0;
    for (i = 0, j = &players[i]; i < MAXPLAYER; i++, j++) {
	int tx, ty;
	if ((j->p_status != PALIVE) && (j->p_status != PEXPLODE))
	    continue;
	if (j->p_flags & PFCLOAK)
	    continue;
	nplayers++;
	dx = j->p_x - me->p_x;
	dy = j->p_y - me->p_y;
	view = SCALE * WINSIDE / 2;
	if (dx > view || dx < -view || dy > view || dy < -view)
	    continue;
	dx = dx / SCALE + WINSIDE / 2;
	dy = dy / SCALE + WINSIDE / 2;
	if (j->p_status == PALIVE) {
	    switch (j->p_team) {
		case FED:
		    XPixFill(w, dx - (ship_width/2), dy - (ship_height/2),
			ship_width, ship_height, playerColor(j),
			fedview[rosette(j->p_dir)], GXcopy, AllPlanes);
		    break;
		case ROM:
		    XPixFill(w, dx - (ship_width/2), dy - (ship_height/2),
			ship_width, ship_height, playerColor(j),
			romview[rosette(j->p_dir)], GXcopy, AllPlanes);
		    break;
		case KLI:
		    XPixFill(w, dx - (ship_width/2), dy - (ship_height/2),
			ship_width, ship_height, playerColor(j),
			kliview[rosette(j->p_dir)], GXcopy, AllPlanes);
		    break;
		case ORI:
		    XPixFill(w, dx - (ship_width/2), dy - (ship_height/2),
			ship_width, ship_height, playerColor(j),
			oriview[rosette(j->p_dir)], GXcopy, AllPlanes);
		    break;
	    }
	    XText(w, dx + (ship_width/2), dy - (ship_height/2),
		shipnos + j->p_no, 1, shipFont(j), playerColor(j), backColor);
	       
	    if (showShields && j->p_flags & PFSHIELD)
		XPixFill(w, dx - (shield_width/2), dy - (shield_height/2),
		    shield_width, shield_height, playerColor(j),
		    shield, GXcopy, AllPlanes);

	    clearzone[0][clearcount] = dx + (shield_width/2);
	    clearzone[1][clearcount] = dy - (shield_height/2);
	    clearzone[2][clearcount] = dfontinfo->width;
	    clearzone[3][clearcount] = dfontinfo->height;
	    clearcount++;
	    clearzone[0][clearcount] = dx - (shield_width/2);
	    clearzone[1][clearcount] = dy - (shield_height/2);
	    clearzone[2][clearcount] = shield_width;
	    clearzone[3][clearcount] = shield_height;
	    clearcount++;
	}
	else if (j->p_status == PEXPLODE) {
	    XPixFill(w, dx - (ex_width/2), dy - (ex_height/2),
		ex_width, ex_height, playerColor(j),
		expview[(10 - j->p_explode)/2], GXcopy, AllPlanes);
	    clearzone[0][clearcount] = dx - (ex_width/2);
	    clearzone[1][clearcount] = dy - (ex_height/2);
	    clearzone[2][clearcount] = ex_width;
	    clearzone[3][clearcount] = ex_height;
	    clearcount++;
	}
	/* Now draw his phaser (if it exists) */
	php = &phasers[j->p_no];
	if (php->ph_status != PHFREE) {
	    if (php->ph_status == PHMISS) {
		/* Here I will have to compute end coordinate */
		tx = j->p_x + PHASEDIST * cos[php->ph_dir];
		ty = j->p_y + PHASEDIST * sin[php->ph_dir];
		tx = (tx - me->p_x) / SCALE + WINSIDE / 2;
		ty = (ty - me->p_y) / SCALE + WINSIDE / 2;
		XLine(w, dx, dy, tx, ty, 1, 1, phaserColor(php),
			GXcopy, AllPlanes);
	    }
	    else { /* Start point is dx, dy */
		tx = (players[php->ph_target].p_x - me->p_x) /
		    SCALE + WINSIDE / 2;
		ty = (players[php->ph_target].p_y - me->p_y) /
		    SCALE + WINSIDE / 2;
		XLine(w, dx, dy, tx, ty, 1, 1, phaserColor(php),
			GXcopy, AllPlanes);
	    }
	    clearline[0][clearlcount] = dx;
	    clearline[1][clearlcount] = dy;
	    clearline[2][clearlcount] = tx;
	    clearline[3][clearlcount] = ty;
	    clearlcount++;
	}
    }
    /* Draw torps */
    for (i = 0, k = &torps[i]; i < MAXPLAYER * MAXTORP; i++, k++) {
	if (!k->t_status)
	    continue;
	dx = k->t_x - me->p_x;
	dy = k->t_y - me->p_y;
	view = SCALE * WINSIDE / 2;
	if (dx > view || dx < -view || dy > view || dy < -view)
	    continue;
	dx = dx / SCALE + WINSIDE / 2;
	dy = dy / SCALE + WINSIDE / 2;
	if (k->t_status == TEXPLODE) {
	    XPixFill(w, dx - (cloud_width/2), dy - (cloud_height/2),
		cloud_width, cloud_height, torpColor(k),
		cloud, GXcopy, AllPlanes);
	    clearzone[0][clearcount] = dx - (cloud_width/2);
	    clearzone[1][clearcount] = dy - (cloud_height/2);
	    clearzone[2][clearcount] = cloud_width;
	    clearzone[3][clearcount] = cloud_height;
	    clearcount++;
	}
	else if (k->t_war & me->p_team) {
	    XPixFill(w, dx - (etorp_width/2), dy - (etorp_height/2),
		etorp_width, etorp_height, torpColor(k),
		etorp, GXcopy, AllPlanes);
	    clearzone[0][clearcount] = dx - (etorp_width/2);
	    clearzone[1][clearcount] = dy - (etorp_height/2);
	    clearzone[2][clearcount] = etorp_width;
	    clearzone[3][clearcount] = etorp_height;
	    clearcount++;
	}
	else {
	    XPixFill(w, dx - (mtorp_width/2), dy - (mtorp_height/2),
		mtorp_width, mtorp_height, torpColor(k),
		mtorp, GXcopy, AllPlanes);
	    clearzone[0][clearcount] = dx - (mtorp_width/2);
	    clearzone[1][clearcount] = dy - (mtorp_height/2);
	    clearzone[2][clearcount] = mtorp_width;
	    clearzone[3][clearcount] = mtorp_height;
	    clearcount++;
	}
    }
    /* Draw Edges */
    if (me->p_x < (WINSIDE / 2) * SCALE) {
	int	sy, ey;

	dx = (WINSIDE / 2) - (me->p_x) / SCALE;
	sy = (WINSIDE / 2) + (0 - me->p_y) / SCALE;
	ey = (WINSIDE / 2) + (GWIDTH - me->p_y) / SCALE;
	if (sy < 0) sy = 0;
	if (ey > WINSIDE - 1) ey = WINSIDE - 1;
	XLine(w, dx, sy, dx, ey, 1, 1, warningColor, GXcopy, AllPlanes);
	clearline[0][clearlcount] = dx;
	clearline[1][clearlcount] = sy;
	clearline[2][clearlcount] = dx;
	clearline[3][clearlcount] = ey;
	clearlcount++;
    }
    if ((GWIDTH - me->p_x) < (WINSIDE / 2) * SCALE) {
	int	sy, ey;

	dx = (WINSIDE / 2) + (GWIDTH - me->p_x) / SCALE;
	sy = (WINSIDE / 2) + (0 - me->p_y) / SCALE;
	ey = (WINSIDE / 2) + (GWIDTH - me->p_y) / SCALE;
	if (sy < 0) sy = 0;
	if (ey > WINSIDE - 1) ey = WINSIDE - 1;
	XLine(w, dx, sy, dx, ey, 1, 1, warningColor, GXcopy, AllPlanes);
	clearline[0][clearlcount] = dx;
	clearline[1][clearlcount] = sy;
	clearline[2][clearlcount] = dx;
	clearline[3][clearlcount] = ey;
	clearlcount++;
    }
    if (me->p_y < (WINSIDE / 2) * SCALE) {
	int	sx, ex;

	dy = (WINSIDE / 2) - (me->p_y) / SCALE;
	sx = (WINSIDE / 2) + (0 - me->p_x) / SCALE;
	ex = (WINSIDE / 2) + (GWIDTH - me->p_x) / SCALE;
	if (sx < 0) sx = 0;
	if (ex > WINSIDE - 1) ex = WINSIDE - 1;
	XLine(w, sx, dy, ex, dy, 1, 1, warningColor, GXcopy, AllPlanes);
	clearline[0][clearlcount] = sx;
	clearline[1][clearlcount] = dy;
	clearline[2][clearlcount] = ex;
	clearline[3][clearlcount] = dy;
	clearlcount++;
    }
    if ((GWIDTH - me->p_y) < (WINSIDE / 2) * SCALE) {
	int	sx, ex;

	dy = (WINSIDE / 2) + (GWIDTH - me->p_y) / SCALE;
	sx = (WINSIDE / 2) + (0 - me->p_x) / SCALE;
	ex = (WINSIDE / 2) + (GWIDTH - me->p_x) / SCALE;
	if (sx < 0) sx = 0;
	if (ex > WINSIDE - 1) ex = WINSIDE - 1;
	XLine(w, sx, dy, ex, dy, 1, 1, warningColor, GXcopy, AllPlanes);
	clearline[0][clearlcount] = sx;
	clearline[1][clearlcount] = dy;
	clearline[2][clearlcount] = ex;
	clearline[3][clearlcount] = dy;
	clearlcount++;
    }

    /* Change border color to signify alert status */

    if (oldalert != (me->p_flags & (PFGREEN|PFYELLOW|PFRED))) {
        oldalert = (me->p_flags & (PFGREEN|PFYELLOW|PFRED));
	switch (oldalert) {
	    case PFGREEN:
		XChangeBorder(baseWin, gTile);
		break;
	    case PFYELLOW:
		XChangeBorder(baseWin, yTile);
		break;
	    case PFRED:
		XChangeBorder(baseWin, rTile);
		break;
	}
    }
}

map()
{
    char buf[80];
    register int i;
    register struct player *j;
    register struct planet *l;
    int junk;
    int dx, dy;

    while (mclearcount) {
	mclearcount--;
	XPixSet(mapw, mclearzone[0][mclearcount], mclearzone[1][mclearcount],
	    mclearzone[2][mclearcount], mclearzone[3][mclearcount],
	    backColor);
    }

    /* Draw Planets */
    for (i = 0, l = &planets[i]; i < MAXPLANETS; i++, l++) {
	if (!(l->pl_flags & PLREDRAW) && (!redrawall))
	    continue;
	dx = l->pl_x * WINSIDE / GWIDTH;
	dy = l->pl_y * WINSIDE / GWIDTH;
	XPixFill(mapw, dx - (mplanet_width/2), dy - (mplanet_height/2),
	    mplanet_width, mplanet_height,
	    planetColor(l), mbplanet, GXcopy, AllPlanes);
	XText(mapw, dx - (mplanet_width/2), dy + (mplanet_height/2),
	    l->pl_name, 3, planetFont(l), planetColor(l), backColor);

/*
	clearzone[0][clearcount] = dx - (mplanet_width/2);
	clearzone[1][clearcount] = dy + (mplanet_height/2);
	clearzone[2][clearcount] = dfontinfo->width * 3;
	clearzone[3][clearcount] = dfontinfo->height;
	clearcount++;
	clearzone[0][clearcount] = dx - (mplanet_width/2);
	clearzone[1][clearcount] = dy - (mplanet_height/2);
	clearzone[2][clearcount] = mplanet_width;
	clearzone[3][clearcount] = mplanet_height;
	clearcount++;
*/
    }
    redrawall = 0;

    /* Draw ships */
    nplayers = 0;
    for (i = 0, j = &players[i]; i < MAXPLAYER; i++, j++) {
	if (j->p_status != PALIVE)
	    continue;
	nplayers++;
	dx = j->p_x * WINSIDE / GWIDTH;
	dy = j->p_y * WINSIDE / GWIDTH;
	if (j->p_flags & PFCLOAK) {
	    sprintf(buf, "??");
	    XText(mapw, dx - dfontinfo->width, dy - dfontinfo->height/2,
		buf, 2, shipFont(j), unColor, backColor);
	}
	else {
	    sprintf(buf, "%c%x", teamlet[j->p_team], j->p_no);
	    XText(mapw, dx - dfontinfo->width, dy - dfontinfo->height/2,
		buf, 2, shipFont(j), playerColor(j), backColor);
	}

	mclearzone[0][mclearcount] = dx - dfontinfo->width;
	mclearzone[1][mclearcount] = dy - dfontinfo->height/2;
	mclearzone[2][mclearcount] = dfontinfo->width * 2;
	mclearzone[3][mclearcount] = dfontinfo->height;
	mclearcount++;
    }
}

stline()
{
    char buf[80];
    char alertchar = '?';

    if (me->p_flags & PFGREEN)
	alertchar = 'G';
    else if (me->p_flags & PFYELLOW)
	alertchar = 'Y';
    else if (me->p_flags & PFRED)
	alertchar = 'R';
    /* Draw status line */
    sprintf(buf,
"%c%c%c%c%c%c%c%c%c%c%c%c   %1d  %3d %3d  %2d    %5.2f  %2d    %5d   %3d   %3d",
	(me->p_flags & PFSHIELD ? 'S': ' '),
	alertchar,
	(me->p_flags & (PFPLLOCK | PFPLOCK) ? 'L': ' '),
	(me->p_flags & PFREPAIR ? 'R': ' '),
	(me->p_flags & PFBOMB ? 'B': ' '),
	(me->p_flags & PFORBIT ? 'O': ' '),
	(me->p_flags & PFCLOAK ? 'C': ' '),
	(me->p_flags & PFWEP ? 'W': ' '),
	(me->p_flags & PFENG ? 'E': ' '),
	(me->p_flags & PFBEAMUP ? 'u': ' '),
	(me->p_flags & PFBEAMDOWN ? 'd': ' '),
	(me->p_flags & PFCOPILOT ? 'P' : ' '),
	me->p_speed,
	me->p_damage,
	me->p_shield,
	me->p_ntorp,
	me->p_kills,
	me->p_armies,
	me->p_fuel,
	me->p_wtemp/10,
	me->p_etemp/10);
    XText(tstatw, 50, 20, buf, strlen(buf), dfont, textColor, backColor);
    XFlush();
    if (showStats)
	updateStats(statwin);
}

/* These are routines that need to be done on interrupts but
   don't belong in the redraw routine and particularly don't
   belong in the daemon. */

auto_features()
{
    char buf[80];
    struct player *pl;
    struct planet *pln;
    unsigned char course;

    if (copilot && (!(me->p_flags & PFCOPILOT))) {
	printf("Owning player has kicked you out\n");
	exit(0);
    }
    if (me->p_flags & PFSELFDEST) {
	if ((me->p_updates >= selfdest) ||
	    ((me->p_flags & PFGREEN) && (me->p_damage == 0)
		&& (me->p_shield == 100))) {
	    me->p_flags &= ~PFSELFDEST;
	    me->p_explode = 10;
	    me->p_whydead = KQUIT;
	    me->p_status = PEXPLODE;
	}
	else {
	    sprintf(buf, "Self Destruct in %d seconds",
		(selfdest - me->p_updates) / 10);
	    warning(buf);
	}
    }
    /* give certain information about bombing or beaming */
    if (me->p_flags & PFBOMB) {
	if (planets[me->p_planet].pl_armies < 5) {
	    sprintf(buf, "Cannot bomb %s while armies are less than 5",
		planets[me->p_planet].pl_name);
	    warning(buf);
	    me->p_flags &= ~PFBOMB;
	}
	else {
	    sprintf(buf, "Bombing %s.  %d armies left",
		planets[me->p_planet].pl_name,
		planets[me->p_planet].pl_armies);
	    warning(buf);
	}
    }

    if (me->p_flags & PFBEAMUP) {
	if (planets[me->p_planet].pl_armies < 5) {
	    sprintf(buf, "%s: Too few armies to beam up",
		planets[me->p_planet].pl_name);
	    warning(buf);
	    me->p_flags &= ~PFBEAMUP;
	}
	else if ((me->p_armies == (int) (me->p_kills * 2)) ||
	    (me->p_armies == myship->s_maxarmies)) {
		sprintf(buf, "No more room on board for armies");
		warning(buf);
		me->p_flags &= ~PFBEAMUP;
	}
	else {
	    sprintf(buf, "Beaming up.  (%d/%d)", me->p_armies,
		((me->p_kills * 2) > myship->s_maxarmies) ?
		    myship->s_maxarmies : (int) (me->p_kills * 2));
	    warning(buf);
	}
    }
    if (me->p_flags & PFBEAMDOWN) {
	if (me->p_armies == 0) {
	    sprintf(buf, "No more armies to beam down to %s.",
		planets[me->p_planet].pl_name);
	    warning(buf);
	    me->p_flags &= ~PFBEAMDOWN;
	}
	else {
	    sprintf(buf, "Beaming down.  (%d/%d) %s has %d armies left",
		me->p_armies,
		((me->p_kills * 2) > myship->s_maxarmies) ?
		    myship->s_maxarmies : (int) (me->p_kills * 2),
		planets[me->p_planet].pl_name,
		planets[me->p_planet].pl_armies);
	    warning(buf);
	}
    }
    if (me->p_flags & PFREPAIR) {
	if ((me->p_damage == 0) && (me->p_shield == 100))
	    me->p_flags &= ~PFREPAIR;
    }
    if (me->p_flags & PFPLOCK) { 	/* set course to player x */
	pl = &players[me->p_playerl];
	if (pl->p_status != PALIVE)
	    me->p_flags &= ~PFPLOCK;
	course = newcourse(pl->p_x, pl->p_y);
	set_course(course);
    }
    if (me->p_flags & PFPLLOCK) { 	/* set course to planet x */
	int dist;
	pln = &planets[me->p_planet];
	dist = hypot((double) (me->p_x - pln->pl_x),
	    (double) (me->p_y - pln->pl_y));

	/* This is magic.  It should be based on defines, but it slows
	   the ship down to warp two an appropriate distance from the
	   planet for orbit */

	if (dist < (50 * ((me->p_speed * (me->p_speed+1)) + 10)))
	    set_speed(2);
	if ((dist < ORBDIST) && (me->p_speed <= 2))  {
	    me->p_flags &= ~PFPLLOCK;
	    orbit();
	}
	else {
	    course = newcourse(pln->pl_x, pln->pl_y);
	    set_course(course);
	}
    }
}

newcourse(x, y)
int x, y;
{
	return((unsigned char) (atan2((double) (x - me->p_x),
	    (double) (me->p_y - y)) / 3.14159 * 128.));
}

redrawTstats()
{
    char	buf[BUFSIZ];

    sprintf(buf, 
	"Flags        warp dam shd torps  kills armies  fuel  wtemp etemp");
    XText(tstatw, 50, 10, buf, strlen(buf), dfont, textColor, backColor);
}
