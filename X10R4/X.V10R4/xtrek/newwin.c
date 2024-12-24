#ifndef lint
static char *rcsid_newwin_c = "$Header: newwin.c,v 10.1 86/11/30 15:26:11 jg Rel $";
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

#define SIZEOF(a)	(sizeof (a) / sizeof (*(a)))

#define WINSIDE		500
#define BOXSIDE		(WINSIDE / 5)
#define BORDER		4
#define TILESIDE	16
#define MESSAGESIZE	20
#define STATSIZE	(MESSAGESIZE * 2 + BORDER)
#define YOFF		100

Window baseWin, iconWin, w, mapw, statwin, messagew, warnw, tstatw;
Display *display;
FontInfo	*bigFont;
    
newwin(hostmon)
char *hostmon;
{
    register int i;
    Window junk;
    char buf[BUFSIZ];

    if ((display = XOpenDisplay(hostmon)) == NULL) {
	perror(hostmon);
	exit(1);
    }
    getResources("xtrek");

    savebitmaps();

    baseWin = XCreateWindow(RootWindow, 0, YOFF, WINSIDE * 2 + 1 * BORDER,
	WINSIDE + 2 * BORDER + 2 * MESSAGESIZE, BORDER, gTile, backTile);
    iconWin = XCreateWindow(RootWindow, 0, 0, icon_width, icon_height,
	BORDER, foreTile, backTile);
    XSetIconWindow(baseWin, iconWin);
    w = XCreateWindow(baseWin, -BORDER, -BORDER, WINSIDE, WINSIDE, 
	BORDER, foreTile, backTile);
    mapw = XCreateWindow(baseWin, WINSIDE, -BORDER, WINSIDE,
	WINSIDE, BORDER, foreTile, backTile);
    tstatw = XCreateWindow(baseWin, -BORDER, WINSIDE, WINSIDE,
	STATSIZE, BORDER, foreTile, backTile);
    warnw = XCreateWindow(baseWin, WINSIDE, WINSIDE,
	WINSIDE, MESSAGESIZE, BORDER, foreTile, backTile);
    messagew = XCreateWindow(baseWin, WINSIDE,
	WINSIDE + BORDER + MESSAGESIZE,
	WINSIDE, MESSAGESIZE, BORDER, foreTile, backTile);
    XDefineCursor(baseWin, crosshair);
    XDefineCursor(iconWin, crosshair);

    initinput();
    XMapWindow(mapw);
    XMapWindow(tstatw);
    XMapWindow(warnw);
    XMapWindow(messagew);
    XMapWindow(w);
    XMapWindow(baseWin);
}

savebitmaps()
{
    register int i;
    crosshair = XCreateCursor(crossw, crossh, crossbits, crossmask_bits, 8, 8,
	myColor, backColor, GXcopy);
    for (i = 0; i < VIEWS; i++) {
	fedview[i] = XStoreBitmap(ship_width, ship_height, fed_bits[i]);
	romview[i] = XStoreBitmap(ship_width, ship_height, rom_bits[i]);
	kliview[i] = XStoreBitmap(ship_width, ship_height, kli_bits[i]);
	oriview[i] = XStoreBitmap(ship_width, ship_height, ori_bits[i]);
    }
    cloud = XStoreBitmap(cloud_width, cloud_height, cloud_bits);
    etorp = XStoreBitmap(etorp_width, etorp_height, etorp_bits);
    mtorp = XStoreBitmap(mtorp_width, mtorp_height, mtorp_bits);
    bplanet = XStoreBitmap(planet_width, planet_height, planet_bits);
    /*
    e_bplanetPix = XMakePixmap(bplanet, enemyColor, backColor);
    a_bplanetPix = XMakePixmap(bplanet, allyColor, backColor);
    */
    mbplanet = XStoreBitmap(mplanet_width, mplanet_height, mplanet_bits);
    /*
    e_mbplanetPix = XMakePixmap(mbplanet, enemyColor, backColor);
    a_mbplanetPix = XMakePixmap(mbplanet, allyColor, backColor);
    */
    for (i = 0; i < EX_FRAMES; i++) {
	expview[i] = XStoreBitmap(ex_width, ex_height, ex_bits[i]);
    }
    shield = XStoreBitmap(shield_width, shield_height, shield_bits);
}

/* This routine throws up an entry window for the player. */

entrywindow() 
{
    int team;
    Window fwin, rwin, kwin, owin, qwin;
    XEvent event;
    char buf[BUFSIZ];

    /* The following allows quick choosing of teams */
    fwin = XCreateWindow(w, 0 * BOXSIDE, 400, BOXSIDE, BOXSIDE, 1, 
	XMakeTile(shipCol[0]), backTile);
    rwin = XCreateWindow(w, 1 * BOXSIDE, 400, BOXSIDE, BOXSIDE, 1, 
	XMakeTile(shipCol[1]), backTile);
    kwin = XCreateWindow(w, 2 * BOXSIDE, 400, BOXSIDE, BOXSIDE, 1, 
	XMakeTile(shipCol[2]), backTile);
    owin = XCreateWindow(w, 3 * BOXSIDE, 400, BOXSIDE, BOXSIDE, 1,
	XMakeTile(shipCol[3]), backTile);
    qwin = XCreateWindow(w, 4 * BOXSIDE, 400, BOXSIDE, BOXSIDE, 1,
	XMakeTile(textColor), backTile);

    XSelectInput(fwin, KeyPressed|ButtonPressed|ButtonReleased|ExposeRegion);
    XSelectInput(rwin, KeyPressed|ButtonPressed|ButtonReleased|ExposeRegion);
    XSelectInput(kwin, KeyPressed|ButtonPressed|ButtonReleased|ExposeRegion);
    XSelectInput(owin, KeyPressed|ButtonPressed|ButtonReleased|ExposeRegion);
    XSelectInput(qwin, KeyPressed|ButtonPressed|ButtonReleased|ExposeRegion);

    XMapWindow(fwin);
    XMapWindow(rwin);
    XMapWindow(kwin);
    XMapWindow(owin);
    XMapWindow(qwin);

    team = -1;
    do {
	    XNextEvent(&event);
	    switch (event.type) {
	    case KeyPressed:
	    case ButtonPressed:
		if (event.window == fwin)
		    team = 0;
		else if (event.window == rwin)
		    team = 1;
		else if (event.window == kwin)
		    team = 2;
		else if (event.window == owin)
		    team = 3;
		else if (event.window == qwin)
		    team = 4;
		break;
	    case ExposeRegion:
	    case ExposeWindow:
		if (event.window == fwin)
		    redrawFed(fwin);
		else if (event.window == rwin)
		    redrawRom(rwin);
		else if (event.window == kwin)
		    redrawKli(kwin);
		else if (event.window == owin)
		    redrawOri(owin);
		else if (event.window == qwin)
		    redrawQuit(qwin);
		else if (event.window == tstatw)
		    redrawTstats();
		else if (event.window == w)
		    showMotd();
		break;
	    }
    } while (team < 0);

    if (team == 4)
	team = -1;
				
    XDestroyWindow(fwin);
    XDestroyWindow(rwin);
    XDestroyWindow(kwin);
    XDestroyWindow(owin);
    XDestroyWindow(qwin);
    return(team);
}

numShips(owner)
{
	int		i, num = 0;
	struct player	*p;

	for (i = 0, p = players; i < MAXPLAYER; i++, p++)
		if (p->p_status == PALIVE && p->p_team == owner)
			num++;
	return (num);
}

static char	*AUTHOR[] = {
    "",
    "---  XTREK Release Version 1.0 ---",
    "",
    "By Chris Guthrie (chris@ic.berkeley.edu)",
    "with help from Ed James (edjames@ic.berkeley.edu)"
};

showMotd()
{
    char buf[BUFSIZ];
    FILE *motd, *fopen();
    int	i, length, top, center;

    /* Author Gratification */
    XClear(w);
    for (i = 0; i < SIZEOF(AUTHOR); i++) {
	length = strlen(AUTHOR[i]);
	center = WINSIDE / 2 - (length * dfontinfo->width) / 2;
	XText(w, center, i * dfontinfo->height, AUTHOR[i], 
	    length, dfont, textColor, backColor);
    }
    top = SIZEOF(AUTHOR) + 2;

    /* the following will print a motd */
    if ((motd = fopen(MOTD, "r")) != NULL) {
	for (i = top; fgets(buf, sizeof (buf), motd) != NULL; i++) {
	    length = strlen(buf);
	    buf[length-1] = NULL;
	    if (length > 80)
		length = 80;
	    XText(w, 20, i * dfontinfo->height, buf, length, dfont,
		textColor, backColor);
	}
    }
    fclose(motd);
}

getResources(prog)
	char	*prog;
{
    getColorDefs(prog);
    getFonts(prog);
    getTiles(prog);
    initStats(prog);

    showShields = booleanDefault(prog, "showshields");
    showStats = booleanDefault(prog, "showstats");
}

static short	solid[TILESIDE] = {
	0xffff, 0xffff, 0xffff, 0xffff,
	0xffff, 0xffff, 0xffff, 0xffff,
	0xffff, 0xffff, 0xffff, 0xffff,
	0xffff, 0xffff, 0xffff, 0xffff,
};
static short	gray[TILESIDE] = {
	0xaaaa, 0x5555, 0xaaaa, 0x5555,
	0xaaaa, 0x5555, 0xaaaa, 0x5555,
	0xaaaa, 0x5555, 0xaaaa, 0x5555,
	0xaaaa, 0x5555, 0xaaaa, 0x5555,
};
static short	striped[TILESIDE] = {
	0xff00, 0xff00, 0xff00, 0xff00,
	0x0ff0, 0x0ff0, 0x0ff0, 0x0ff0,
	0x00ff, 0x00ff, 0x00ff, 0x00ff,
	0xf00f, 0xf00f, 0xf00f, 0xf00f,
};

getTiles(prog)
	char	*prog;
{
	char	*str;
	short	rPatt[TILESIDE], yPatt[TILESIDE], gPatt[TILESIDE];
	int	rSize = sizeof (rPatt);
	int	ySize = sizeof (yPatt);
	int	gSize = sizeof (gPatt);

	backTile = XMakeTile(backColor);
	foreTile = XMakeTile(borderColor);

	if (DisplayCells() > 2) {
		rTile = XMakeTile(rColor);
		yTile = XMakeTile(yColor);
		gTile = XMakeTile(gColor);
	} else {
		if (arrayDefault(prog, "RalertPattern", &rSize, rPatt) < 0) {
			rSize = TILESIDE;
			bcopy(striped, rPatt, sizeof (rPatt));
		}
		if (arrayDefault(prog, "YalertPattern", &ySize, yPatt) < 0) {
			ySize = TILESIDE;
			bcopy(gray, yPatt, sizeof (yPatt));
		}
		if (arrayDefault(prog, "GalertPattern", &gSize, gPatt) < 0) {
			gSize = TILESIDE;
			bcopy(solid, gPatt, sizeof (gPatt));
		}

		rTile = XMakePixmap(XStoreBitmap(rSize, rSize, rPatt), 
			rColor, backColor);
		yTile = XMakePixmap(XStoreBitmap(ySize, ySize, yPatt), 
			yColor, backColor);
		gTile = XMakePixmap(XStoreBitmap(gSize, gSize, gPatt), 
			gColor, backColor);
	}
}

getFonts(prog)
	char	*prog;
{
    char	*font;

    if ((font = XGetDefault(prog, "font")) == NULL)
	font = "6x10";
    if ((dfontinfo = XOpenFont(font)) == NULL) {
	perror(font);
	exit(1);
    }

    if ((font = XGetDefault(prog, "boldfont")) == NULL)
	font = "6x10b";
    if ((bfontinfo = XOpenFont(font)) == NULL)
	bfontinfo = dfontinfo;

    if ((bigFont = XOpenFont("sbdr40sx")) == NULL)
	bigFont = dfontinfo;

    dfont = dfontinfo->id;
    bfont = bfontinfo->id;
}

redrawFed(fwin)
	Window fwin;
{
    char buf[BUFSIZ];

    XText(fwin, 5, 5, "Federation", 10, dfont, shipCol[0], backColor);
    sprintf(buf, "%d", numShips(FED));
    XText(fwin, 50, 50, buf, strlen(buf), bigFont->id, shipCol[0], backColor);
}

redrawRom(rwin)
	Window rwin;
{
    char buf[BUFSIZ];

    XText(rwin, 5, 5, "Romulan", 7, dfont, shipCol[1], backColor);
    sprintf(buf, "%d", numShips(ROM));
    XText(rwin, 50, 50, buf, strlen(buf), bigFont->id, shipCol[0], backColor);
}

redrawKli(kwin)
	Window kwin;
{
    char buf[BUFSIZ];

    XText(kwin, 5, 5, "Klingon", 7, dfont, shipCol[2], backColor);
    sprintf(buf, "%d", numShips(KLI));
    XText(kwin, 50, 50, buf, strlen(buf), bigFont->id, shipCol[0], backColor);
}

redrawOri(owin)
	Window owin;
{
    char buf[BUFSIZ];

    XText(owin, 5, 5, "Orion", 5, dfont, shipCol[3], backColor);
    sprintf(buf, "%d", numShips(ORI));
    XText(owin, 50, 50, buf, strlen(buf), bigFont->id, shipCol[0], backColor);
}

redrawQuit(qwin)
	Window qwin;
{
    char buf[BUFSIZ];

    XText(qwin, 5, 5, "Quit xtrek", 10, dfont, textColor, backColor);
}
