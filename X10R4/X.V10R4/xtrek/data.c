#ifndef lint
static char *rcsid_data_c = "$Header: data.c,v 10.1 86/11/30 15:23:46 jg Rel $";
#endif	lint
/* Copyright (c) 1986 	Chris Guthrie */

#include <X/Xlib.h>
#include "defs.h"
#include "struct.h"
#include "data.h"

struct player *players;
struct player *me;
struct torp *torps;
struct status *status;
struct ship *myship;
struct stats *mystats;
struct planet *planets;
struct phaser *phasers;
struct message *messages;
struct mctl *mctl;

int	mapmode = 1; 
int	statmode = 1;
int	showStats;
int	showShields;

Pixmap	foreTile, backTile, rTile, yTile, gTile;

int	borderColor, backColor, textColor, myColor, warningColor, shipCol[4],
	rColor, yColor, gColor, unColor;

char teamlet[] = {'X', 'F', 'R', 'X', 'K', 'X', 'X', 'X', 'O'};
char *teamshort[9] = {"X", "FED", "ROM", "X", "KLI", "X", "X", "X", "ORI"};

Font 		dfont, bfont;
FontInfo 	*dfontinfo, *bfontinfo;
