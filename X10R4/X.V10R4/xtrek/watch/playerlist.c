
#include <X/Xlib.h>
#include <stdio.h>
#include <math.h>
#include <signal.h>
#include "defs.h"
#include "struct.h"
#include "data.h"
#include "bitmaps.h"

Window playerw;
extern Window w;
extern Font dfont;
extern FontInfo *dfontinfo;

char teamlet[] = {'X', 'F', 'R', 'X', 'K', 'X', 'X', 'X', 'O'};

playerlist()
{
    register int i;
    register int k = 0;
    char buf[BUFSIZ];
    register struct player *j;
    XEvent foo;

    playerw = XCreateWindow(w, 10, 10, 63 * dfontinfo->width,
	(MAXPLAYER + 3) * dfontinfo->height, 2, foreTile, backTile);
    XMapWindow(playerw);
    sprintf(buf, " # Team Name             Login       x      y    dir spd kills");
    XText(playerw, 0, dfontinfo->height, buf,
       strlen(buf), dfont, textColor, backColor);
    k = 2;
    for (i = 0, j = &players[i]; i < MAXPLAYER; i++, j++) {
	if (j->p_status != PALIVE)
	    continue;
	sprintf(buf, " %1x  %1c   %-16s %-8s %6d %6d %3d %3d %5.2f",
	    j->p_no,
	    teamlet[j->p_team],
	    j->p_name,
	    j->p_login,
	    j->p_x,
	    j->p_y,
	    j->p_dir,
	    j->p_speed,
	    j->p_kills);
	XText(playerw, 0, dfontinfo->height * k++, buf, strlen(buf),
	   dfont, textColor, backColor);
	XFlush();
    }
    XNextEvent(&foo);
    XDestroyWindow(playerw);
}
