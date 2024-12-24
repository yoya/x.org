#ifndef lint
static char *rcsid_input_c = "$Header: input.c,v 10.1 86/11/30 15:25:32 jg Rel $";
#endif	lint
/* Copyright (c) 1986 	Chris Guthrie */

#include <X/Xlib.h>
#include <stdio.h>
#include <math.h>
#include <signal.h>
#include "defs.h"
#include "struct.h"
#include "data.h"
#include "bitmaps.h"

#define WINSIDE 500

extern Window iconWin, w, mapw, statwin, messagew, tstatw;
extern Font dfont;
extern Display *display;
extern int redrawall;
extern int lastm;		/* Last new message */
extern int copilot;
int selfdest;
int doTheRedrawDude, skipUpdates = 1;

initinput()
{
    XSelectInput(iconWin, ExposeWindow|ExposeRegion);
    XSelectInput(w, 
	KeyPressed|ButtonPressed|ButtonReleased|ExposeRegion|ExposeWindow);
    XSelectInput(mapw, 
	KeyPressed|ButtonPressed|ButtonReleased|ExposeWindow|ExposeRegion);
    XSelectInput(messagew, 
	KeyPressed|ButtonPressed|ButtonReleased|ExposeWindow|ExposeRegion);
    XSelectInput(tstatw, ExposeWindow|ExposeRegion);
}

setRedrawFlag()
{
	if (skipUpdates)
		doTheRedrawDude = 1;
	else
		doTheRedrawDude++;
}

input()
{
    XEvent data;
    XKeyOrButtonEvent *tmp;
    int quit = 0;
    char *buf;
    int nchar;
    int fd;

    tmp = (XKeyOrButtonEvent *) &data;

    signal(SIGALRM, setRedrawFlag);
    doTheRedrawDude = 1;
    while (1) {
	while (doTheRedrawDude-- > 0) {
		intrupt();
	}
	fd = (1 << display->fd);
	if (!XPending()) {
	    while (select(32, &fd, 0, 0, 0) < 0)
		fd = (1 << display->fd);
	}
	XNextEvent(&data);
	if ((me->p_status == PEXPLODE) || (me->p_status == PDEAD)) {
	    if (copilot)
		exit(0);
	    else
		death();
	}
	switch (data.type) {
	    case KeyPressed:
		if (me->p_flags & PFSELFDEST) {
		    me->p_flags &= ~PFSELFDEST;
		    warning("Self Destruct has been canceled");
		}
		buf = XLookupMapping(&data, &nchar);
		if (nchar > 0) {
		    if (data.window == messagew)
			smessage(*buf);
		    else
			keyaction(*buf, tmp);
		}
		break;
	    case ButtonPressed:
		if (me->p_flags & PFSELFDEST) {
		    me->p_flags &= ~PFSELFDEST;
		    warning("Self Destruct has been canceled");
		}
		buttonaction(tmp);
		break;
	    case ExposeRegion:
	    case ExposeWindow:
		if (data.window == statwin && showStats)
			redrawStats(statwin);
		else if (data.window == tstatw)
			redrawTstats();
		else if (data.window == mapw)
		    redrawall = 1;
		else if (data.window == iconWin)
		    XBitmapBitsPut(iconWin, 0, 0, icon_width, icon_height,
			icon_bits, borderColor, backColor, 0,
			GXcopy, AllPlanes);
		break;
	    default:
		break;
	}
    }
}

keyaction(key, data)
char key;
XKeyOrButtonEvent *data;
{
    char buf[80];
    unsigned char course;
    struct obtype *gettarget(), *target;
    struct player *p;
    struct planet *pl;

    switch(key) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	    set_speed(key - '0');
	    break;
	case 'k': /* k = set course */
	    course = getcourse(data->window, data->x, data->y);
	    set_course(course);
	    me->p_flags &= ~(PFPLOCK | PFPLLOCK);
	    break;
	case 'p': /* p = fire phasers */
	    course = getcourse(data->window, data->x, data->y);
	    phaser(course);
	    break;
	case 't': /* t = launch torps */
	    course = getcourse(data->window, data->x, data->y);
	    ntorp(course, TMOVE);
	    break;
	case 'd': /* d = detonate other torps */
	    detothers();
	    break;
	case 'D': /* D = detonate my torps */
	    detmine();
	    break;
	case 'L': /* L = Player list */
	    playerlist();
	    break;
	case 'P': /* P = Planet list */
	    planetlist();
	    break;
	case 'S': /* S = toggle stat mode */
	   if (showStats) {
		showStats = !showStats;
		closeStats(statwin);
	   } else {
		statwin = openStats(me);
		showStats = !showStats;
	   }
	   break;
	case 'M': /* M = Toggle Map mode */
	    mapmode = !mapmode;
	    break;
	case '+': /* + = Put shields up */
	    shield_up();
	    break;
	case '-': /* - = Put shields down */
	    shield_down();
	    break;
	case 'u': /* u = toggle shields */
	    shield_tog();
	    break;
	case 'i': /* i = get information */
	    inform(data->window, data->x, data->y);
	    break;
	case 'b': /* b = bomb planet */
	    bomb_planet();
	    break;
	case 'z': /* z = beam up */
	    beam_up();
	    break;
	case 'x': /* x = beam down */
	    beam_down();
	    break;
	case 'R': /* R = Go into repair mode */
	    me->p_flags &= ~(PFPLOCK | PFPLLOCK);
	    repair();
	    break;
	case 'o': /* o = orbit nearest planet */
	    me->p_flags &= ~(PFPLOCK | PFPLLOCK);
	    orbit();
	    break;
	case 'Q':
	    if (copilot)
		exit(1);
	    me->p_flags |= PFSELFDEST;
	    selfdest = me->p_updates + 100;
	    warning("Self destruct initiated");
	    break;
	case '?': /* ? = Redisplay all messages */
	    repeat_message();
	    break;
	case 'c': /* c = cloak */
	    cloak();
	    break;
	case 'C': /* C = coups */
	    coup();
	    break;
	case 'l': /* l = lock onto */
	    /* since a robot would never use this function (it's user
	       interface dependent,) all the work is done here instead
	       of in interface.c */
	    target = gettarget(data->window, data->x, data->y);
	    if (target->o_type == PLAYERTYPE) {
		me->p_flags |= PFPLOCK;
		me->p_flags &= ~(PFPLLOCK|PFORBIT);
		me->p_playerl = target->o_num;
		p = &players[target->o_num];
		sprintf(buf, "Locking onto %s (%c%d)",
		    p->p_name,
		    teamlet[p->p_team],
		    p->p_no);
		warning(buf);
	    }
	    else {
		me->p_flags |= PFPLLOCK;
		me->p_flags &= ~(PFPLOCK|PFORBIT);
		me->p_planet = target->o_num;
		pl = &planets[target->o_num];
		sprintf(buf, "Locking onto %s",
		    pl->pl_name);
		warning(buf);
	    }
	    break;
	case '@': /* u = toggle shields */
	    me->p_flags ^= PFCOPILOT;
	    break;
	default:
	    XFeep(0);
	    break;
    }
}
buttonaction(data)
XKeyOrButtonEvent *data;
{
    unsigned char course;
    if ((data->detail & 0xf) == RightButton) {
	course = getcourse(data->window, data->x, data->y);
	me->p_desdir = course;
	me->p_flags &= ~(PFPLOCK | PFPLLOCK);
    }
    else if ((data->detail & 0xf) == LeftButton) {
	course = getcourse(data->window, data->x, data->y);
	ntorp(course, TMOVE);
    }
    else if ((data->detail & 0xf) == MiddleButton) {
	course = getcourse(data->window, data->x, data->y);
	phaser(course);
    }
}

getcourse(ww, x, y)
Window ww;
int x, y;
{
    if (ww == mapw) {
	int	me_x, me_y;

	me_x = me->p_x * WINSIDE / GWIDTH;
	me_y = me->p_y * WINSIDE / GWIDTH;
	return((unsigned char) (atan2((double) (x - me_x),
	    (double) (me_y - y)) / 3.14159 * 128.));
    }
    else
	return((unsigned char) (atan2((double) (x - WINSIDE/2),
	    (double) (WINSIDE/2 - y))
		/ 3.14159 * 128.));
}
