#ifndef lint
static char *rcsid_dmessage_c = "$Header: dmessage.c,v 10.1 86/11/30 15:24:23 jg Rel $";
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

extern Window messagew;
extern int udcounter;
extern int messpend;	/* If the player is sending a message, go away */
int lastm;

int lastcount;
int mdisplayed;

dmessage()
{
    struct message *cur;
    char buf[80];

    if (messpend)
	return;
    if (mctl->mc_current == lastm) {
	if (mdisplayed) {
	    XPixSet(messagew, 5, 5, dfontinfo->width * lastcount,
		dfontinfo->height, backColor);
	    mdisplayed = 0;
	}
	return;
    }
    do {
	if (++lastm >= MAXMESSAGE)
	    lastm = 0;
	cur = &messages[lastm];
	if (cur->m_flags & MVALID) {
	    if ((cur->m_flags & MALL) ||
		((cur->m_flags & MTEAM) && (cur->m_recpt & me->p_team)) ||
		((cur->m_flags & MINDIV) && (cur->m_recpt == me->p_no))) {
		    if (mdisplayed) {
			XPixSet(messagew, 5, 5, dfontinfo->width * lastcount,
			    dfontinfo->height, backColor);
			mdisplayed = 0;
		    }
		    sprintf(buf, "%s", cur->m_data);
		    lastcount = strlen(buf);
		    if (lastcount > 80)
			lastcount = 80;
		    XText(messagew, 5, 5, buf, lastcount, dfont,
			textColor, backColor);
		    XFeep(0);
		    XFlush();
		    mdisplayed = 1;
		    return;
	    }
	}
    } while (lastm != mctl->mc_current);
}
