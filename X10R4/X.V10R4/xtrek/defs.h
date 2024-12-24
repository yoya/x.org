/* $Header: defs.h,v 10.1 86/11/30 15:30:19 jg Rel $ */
/* Copyright (c) 1986 	Chris Guthrie */

#define MAXPLAYER 16
#define MAXPLANETS 40
#define MAXTORP 12
#define PKEY 257
#define TKEY 258
#define STKEY 259
#define PLKEY 260
#define PHKEY 261
#define MKEY 262

/* These are configuration definitions */

#define GWIDTH 100000   /* galaxy is 100000 spaces on a side */
#define WARP1 20	/* warp one will move 20 spaces per update */
#define SCALE 40	/* Window will be one pixel for 20 spaces */
#define EXPDIST 400	/* At this range a torp will explode */
#define DETDIST 2500	/* At this range a player can detonate a torp */
#define PHASEDIST 6000	/* At this range a player can do damage with phasers */
#define ORBDIST 800	/* At this range a player can orbit a planet */
#define ORBSPEED 2	/* This is the fastest a person can go into orbit */
#define PFIREDIST 1500	/* At this range a planet will shoot at a player */
#define UPDATE 100000	/* Update time is 100000 micro-seconds */


/* These are memory sections */
#define PLAYER 1
#define MAXMESSAGE 50

#define rosette(x)	((((x) + 256/VIEWS/2) / (256/VIEWS)) % VIEWS)

/* These are the teams */
/* Note that I used bit types for these mostly for messages and
   war status.  This was probably a mistake.  It meant that Ed
   had to add the 'remap' area to map these (which are used throughout
   the code as the proper team variable) into a nice four team deep
   array for his color stuff.  Oh well.
*/
#define FED 0x1
#define ROM 0x2
#define KLI 0x4
#define ORI 0x8
#define ALLTEAM (FED|ROM|KLI|ORI)
#define MAXTEAM (ORI)
#define NUMTEAM 4

/* Data files to make the game play across daemon restarts. */

/* #define DIR		"/eros/staff/chris/nt */
#define DIR		"/usr/games/lib/xtrek

#define PLFILE		DIR/.planets"
#define SCOREFILE	DIR/.scores"
#define MOTD		DIR/.motd"
#define DAEMON		DIR/daemon"
#define ROBOT		DIR/robot"

/* Other stuff that Ed added */

#define ABS(a)			/* abs(a) */ (((a) < 0) ? -(a) : (a))

#define myTorp(t)		(me->p_no == (t)->t_owner)
#define friendlyTorp(t)		(!(me->p_team & (t)->t_war))
#define myPhaser(p)		(&phasers[me->p_no] == (p))
#define friendlyPhaser(p)	(me->p_team == players[(p) - phasers].p_team)
#define myPlayer(p)		(me == (p))
#define friendlyPlayer(p)	(me->p_team == (p)->p_team)
#define isAlive(p)		((p)->p_status == PALIVE)
#define friendlyPlanet(p)	((p)->pl_owner & me->p_team)

#define torpColor(t)		\
	(myTorp(t) ? myColor : shipCol[remap[players[(t)->t_owner].p_team]])
#define phaserColor(p)		\
	(myPhaser(p) ? myColor : shipCol[remap[players[(p) - phasers].p_team]])
#define playerColor(p)		\
	(myPlayer(p) ? myColor : shipCol[remap[(p)->p_team]])
#define planetColor(p)		\
	(((p)->pl_info & me->p_team) ? shipCol[remap[(p)->pl_owner]] : unColor)

#define planetFont(p)		\
	(friendlyPlanet(p) ? bfont : dfont)
#define shipFont(p)		\
	(myPlayer(p) ? bfont : dfont)
