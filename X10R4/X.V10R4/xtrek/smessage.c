#ifndef lint
static char *rcsid_smessage_c = "$Header: smessage.c,v 10.1 86/11/30 15:28:48 jg Rel $";
#endif	lint
/* Copyright (c) 1986 	Chris Guthrie */


#include <X/Xlib.h>
#include <stdio.h>
#include <math.h>
#include <signal.h>
#include <ctype.h>
#include "defs.h"
#include "struct.h"
#include "data.h"
#include "bitmaps.h"

extern Window messagew;

int messpend;
int lcount;
char buf[80];
char addr;
#define ADDRLEN 10
char addrmesg[ADDRLEN];

int lastcount;
extern int mdisplayed;

smessage(ichar)
char ichar;
{
    int group;
    int recip;
    char ch;

    if (messpend == 0) {
	messpend = 1;
	if (mdisplayed) {
	    XPixSet(messagew, 5, 5, dfontinfo->width * lastcount,
		dfontinfo->height, backColor);
	    mdisplayed = 0;
	}
	/* Put the proper recipient in the window */
	if (getaddr(ichar) < 0) {
	    /* print error message */
	    messpend = 0;
	    return;
	}
	XText(messagew, 5 + dfontinfo->width, 5,
	    addrmesg, ADDRLEN, dfont, textColor, backColor);
	lcount = ADDRLEN;
	return;
    }
    switch (ichar) {
	case '\b':
	case '\177':
	    if (--lcount < ADDRLEN) {
		lcount = ADDRLEN;
		break;
	    }
	    XPixSet(messagew, 5 + dfontinfo->width * lcount, 5,
		dfontinfo->width, dfontinfo->height, backColor);
	    break;
	case '\025':
	case '\030':
	    if (--lcount < ADDRLEN) {
		lcount = ADDRLEN;
		break;
	    }
	    XPixSet(messagew, 5 + dfontinfo->width * lcount, 5,
		dfontinfo->width, dfontinfo->height, backColor);
	    while(--lcount)
		XPixSet(messagew, 5 + dfontinfo->width * lcount, 5,
		    dfontinfo->width, dfontinfo->height, backColor);
	    break;
	case '\r':
	    buf[lcount - ADDRLEN] = 0;
	    messpend = 0;
	    switch (addr) {
		case 'A':
		    pmessage(buf, 0, MALL, addrmesg);
		    break;
		case 'F':
		    pmessage(buf, FED, MTEAM, addrmesg);
		    break;
		case 'R':
		case 'r':
		    pmessage(buf, ROM, MTEAM, addrmesg);
		    break;
		case 'K':
		case 'k':
		    pmessage(buf, KLI, MTEAM, addrmesg);
		    break;
		case 'O':
		case 'o':
		    pmessage(buf, ORI, MTEAM, addrmesg);
		    break;
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
		    pmessage(buf, addr - '0', MINDIV, addrmesg);
		    break;
		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
		    pmessage(buf, addr - 'a' - 10, MINDIV, addrmesg);
		    break;
		default:
		    warning("Not legal recipient");
	    }
	    XPixSet(messagew, 5, 5, dfontinfo->width * lcount,
		dfontinfo->height, backColor);
	    mdisplayed = 0;
	    lcount = 0;
	    break;
	default:
	    if (lcount == 80) {
		XFeep(0);
		break;
	    }
	    if (iscntrl(ichar))
		break;
	    XText(messagew, 5 + dfontinfo->width * lcount, 5,
		&ichar, 1, dfont, textColor, backColor);
	    buf[(lcount++) - ADDRLEN] = ichar;
	    break;
    }
}
	    

pmessage(str, recip, group, address)
char *str;
int recip;
int group;
char *address;
{
    struct message *cur;
    if (++(mctl->mc_current) >= MAXMESSAGE)
	mctl->mc_current = 0;
    cur = &messages[mctl->mc_current];
    cur->m_no = mctl->mc_current;
    cur->m_flags = group;
    cur->m_time = 0;
    cur->m_recpt = recip;
    sprintf(cur->m_data, "%-9s %s", address, str);
    cur->m_flags |= MVALID;
}

getaddr(who)
char who;
{
    addr = who;
    sprintf(addrmesg, " %c%x->", teamlet[me->p_team], me->p_no);
    switch (who) {
	case 'A':
	    sprintf(&addrmesg[5], "ALL");
	    break;
	case 'F':
	    sprintf(&addrmesg[5], "FED");
	    break;
	case 'R':
	case 'r':
	    sprintf(&addrmesg[5], "ROM");
	    break;
	case 'K':
	case 'k':
	    sprintf(&addrmesg[5], "KLI");
	    break;
	case 'O':
	case 'o':
	    sprintf(&addrmesg[5], "ORI");
	    break;
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
	    if (isAlive(&players[who - '0'])) {
		sprintf(&addrmesg[5], "%c%x ",
		    teamlet[players[who - '0'].p_team], who - '0');
	    }
	    else {
		warning("Player is not in game");
		return(-1);
	    }
	    break;
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	    if (isAlive(&players[who - 'a' + 10])) {
		sprintf(&addrmesg[5], "%c%x ",
		    teamlet[players[who - 'a' + 10].p_team], who - 'a' + 10);
	    }
	    else {
		warning("Player is not in game");
		return(-1);
	    }
	    break;
	default:
	    warning("Not legal recipient");
	    return(-1);
    }
    return(0);
}
