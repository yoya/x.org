/* $Header: data.h,v 10.1 86/11/30 15:30:10 jg Rel $ */
/* Copyright (c) 1986 	Chris Guthrie */

extern struct player *players;
extern struct player *me;
extern struct torp *torps;
extern struct status *status;
extern struct ship *myship;
extern struct stats *mystats;
extern struct planet *planets;
extern struct phaser *phasers;
extern struct message *messages;
extern struct mctl *mctl;

extern int mapmode; 
extern int statmode;

extern int showShields;
extern int showStats;

extern Pixmap	foreTile, backTile, rTile, yTile, gTile;

extern int	borderColor, backColor, textColor, myColor, 
		warningColor, shipCol[4], rColor, yColor,
		gColor, unColor;

extern char teamlet[];
extern char *teamshort[];

extern Font dfont, bfont;
extern FontInfo *dfontinfo, *bfontinfo;
