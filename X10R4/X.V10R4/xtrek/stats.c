#ifndef lint
static char *rcsid_stats_c = "$Header: stats.c,v 10.1 86/11/30 15:29:08 jg Rel $";
#endif	lint
/* Copyright (c) 1986 	Chris Guthrie */

#include <X/Xlib.h>
#include <stdio.h>
#include "defs.h"
#include "struct.h"
#include "data.h"

#define	MIN(a,b)	(((a) < (b)) ? (a) : (b))
#define	MAX(a,b)	(((a) > (b)) ? (a) : (b))

#define	BX_OFF()	((textWidth + 1) * dfontinfo->width + S_IBORDER)
#define	BY_OFF(line)	((line) * (dfontinfo->height + S_IBORDER) + S_IBORDER)
#define	TX_OFF(len)	((textWidth - len) * dfontinfo->width + S_IBORDER)
#define	TY_OFF(line)	BY_OFF(line)

#define STAT_WIDTH		160
#define STAT_HEIGHT		BY_OFF(NUM_SLIDERS)
#define STAT_BORDER		2
#define S_IBORDER		5
#define STAT_X			422
#define STAT_Y			13

#define SL_WID			\
	(STAT_WIDTH - 2 * S_IBORDER - (textWidth + 1) * dfontinfo->width)
#define SL_HEI			(dfontinfo->height)

#define NUM_ELS(a)		(sizeof (a) / sizeof (*(a)))
#define NUM_SLIDERS		NUM_ELS(sliders)

typedef struct slider {
	char	*label;
	int	min, max;
	int	low_red, high_red;
	int	label_length;
	int	diff;
} SLIDER;

typedef struct record {
	short	*data;
	short	last_value;
} RECORD;

static SLIDER	sliders[] = {
	{ "Shields",		0,	100,	20,	100	},
	{ "Damage",		0,	100,	0,	0	},
	{ "Fuel",		0,	10000,	1000,	10000	},
	{ "Warp",		0,	9,	0,	9	},
	{ "Weapon Temp",	0,	1200,	0,	1000	},
	{ "Engine Temp",	0,	1200,	0,	1000	},
};

static XAssocTable	*table;
static int		textWidth = 0;
static int		statX = STAT_X, statY = STAT_Y;

initStats(prog)
	char	*prog;
{
	int	i, dummy;
	char	*str;

	if ((str = XGetDefault(prog, "stats.geometry")) != NULL)
		XParseGeometry(str, &statX, &statY, &dummy, &dummy);
	table = (XAssocTable *) XCreateAssocTable(8);
	for (i = 0; i < NUM_SLIDERS; i++) {
		sliders[i].label_length = strlen(sliders[i].label);
		textWidth = MAX(textWidth, sliders[i].label_length);
		sliders[i].diff = sliders[i].max - sliders[i].min;
	}
}

Window
openStats(p)
	struct player	*p;
{
	Window		w;
	RECORD		*rp;
	extern Cursor	crosshair;

	w = XCreateWindow(RootWindow, statX, statY, STAT_WIDTH, STAT_HEIGHT,
		STAT_BORDER, foreTile, backTile);
	XDefineCursor(w, crosshair);
	rp = (RECORD *) calloc(NUM_SLIDERS, sizeof (RECORD));
	rp[0].data = &(p->p_shield);
	rp[1].data = &(p->p_damage);
	rp[2].data = &(p->p_fuel);
	rp[3].data = &(p->p_speed);
	rp[4].data = &(p->p_wtemp);
	rp[5].data = &(p->p_etemp);
	XMakeAssoc(table, w, rp);
	XSelectInput(w, ExposeWindow|ExposeRegion);
	XMapWindow(w);
	return (w);
}

redrawStats(w)
	Window	w;
{
	int	i;
	RECORD	*rp;

	rp = (RECORD *) XLookUpAssoc(table, w);
	if (rp == NULL) {
		fputs("You gave redrawStats a bum window\n", stderr);
		return;
	}
	XClear(w);
	for (i = 0; i < NUM_SLIDERS; i++) {
		rp[i].last_value = 0;
		XText(w, TX_OFF(sliders[i].label_length), TY_OFF(i),
			sliders[i].label, sliders[i].label_length, dfont,
			textColor, backColor);
		box(w, 0, BX_OFF() - 1, BY_OFF(i) - 1, SL_WID+2, SL_HEI+2,
			borderColor, GXcopy, AllPlanes);
	}
}

closeStats(w)
	Window	w;
{
	WindowInfo	wi;

	XDeleteAssoc(table, w);
	if (XQueryWindow(w, &wi) != 0) {
		statX = wi.x;
		statY = wi.y;
	}
	XDestroyWindow(w);
}

updateStats(w)
	Window		w;
{
	int	i, value, diff, color, old_x, new_x, wid;
	RECORD	*rp, *r;
	SLIDER	*s;

	rp = (RECORD *) XLookUpAssoc(table, w);
	for (i = 0; i < NUM_SLIDERS; i++) {
		r = &rp[i];
		s = &sliders[i];
		value = *(r->data);
		if (value < s->min)
			value = s->min;
		else if (value > s->max)
			value = s->max;
		if (value == r->last_value)
			continue;
		diff = value - r->last_value;
		if (diff < 0)
			color = backColor;
		else {
			if (value < s->low_red)
				color = warningColor;
			else if (value > s->high_red) {
				color = warningColor;
			    	if (r->last_value <= s->high_red)
					r->last_value = 0;
			} else {
				color = myColor;
				if (r->last_value < s->low_red)
					r->last_value = 0;
			}
		}
		old_x = r->last_value * SL_WID / s->diff;
		new_x = value * SL_WID / s->diff;
		wid = new_x - old_x;
		/*
		if (wid > 0)
			wid++;
		else if (wid < 0)
			wid--;
		*/
		box(w, 1, BX_OFF() + old_x, BY_OFF(i), wid,
			SL_HEI, color, GXcopy, AllPlanes);
		if (diff < 0) {
			if (r->last_value >= s->low_red && value < s->low_red)
				box(w, 1, BX_OFF(), BY_OFF(i), new_x, SL_HEI,
					warningColor, GXcopy, AllPlanes);
			else if (r->last_value > s->high_red && 
				 value <= s->high_red)
				box(w, 1, BX_OFF(), BY_OFF(i), new_x, SL_HEI,
					myColor, GXcopy, AllPlanes);
		}
		r->last_value = value;
	}
}


			
box(w, filled, x, y, wid, hei, pixel, func, planes)
	Window	w;
{
	Vertex	vertex[5];

	if (wid == 0 || hei == 0)
		return;

	if (filled) {
		int	minX, minY;

		minX = MIN(x, x + wid);
		wid = ABS(wid);
		minY = MIN(y, y + hei);
		hei = ABS(hei);
		XPixFill(w, minX, minY, wid + 1, hei + 1, pixel, 0,
			func, planes);
		return;
	}

	vertex[0].x = x;
	vertex[0].y = y;
	vertex[0].flags = VertexStartClosed;

	vertex[1].x = x + wid;
	vertex[1].y = y;
	vertex[1].flags = 0;

	vertex[2].x = x + wid;
	vertex[2].y = y + hei;
	vertex[2].flags = 0;

	vertex[3].x = x;
	vertex[3].y = y + hei;
	vertex[3].flags = 0;

	vertex[4].x = x;
	vertex[4].y = y;
	vertex[4].flags = VertexEndClosed;

	XDraw(w, vertex, 5, 1, 1, pixel, func, planes);
}
