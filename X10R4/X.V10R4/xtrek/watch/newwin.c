#include <X/Xlib.h>
#include <stdio.h>
#include <math.h>
#include <signal.h>
#include "../defs.h"
#include "../struct.h"
#include "../data.h"
#include "../bitmaps.h"

#define WINSIDE 500

Window w, mapw, statwin;
Display *display;
Font dfont;
FontInfo *dfontinfo;

newwin(hostmon)
char *hostmon;
{
    register int i;
    Window junk;
    char buf[BUFSIZ];
    char	*font;

    if ((display = XOpenDisplay(hostmon)) == NULL) {
	perror(hostmon);
	exit(1);
    }
    if ((font = XGetDefault("xtrek", "font")) == NULL)
	font = "6x10";
    if ((dfontinfo = XOpenFont(font)) == NULL) {
	perror(font);
	exit(1);
    }
    showShields = (XGetDefault("xtrek", "showshields") != NULL);
    showStats = (XGetDefault("xtrek", "showstats") != NULL);

    dfont = dfontinfo->id;
    getColorDefs("xtrek");
    backTile = XMakeTile(backColor);
    foreTile = XMakeTile(borderColor);
    savebitmaps();
    w = XCreateWindow(RootWindow, 0, 100, WINSIDE, WINSIDE, 4, foreTile,
	backTile);
    mapw = XCreateWindow(RootWindow, WINSIDE + 4, 100, WINSIDE,
	WINSIDE, 4, foreTile, backTile);
    XDefineCursor(w, crosshair);
    XDefineCursor(mapw, crosshair);
    XMapWindow(w);
    XMapWindow(mapw);
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

