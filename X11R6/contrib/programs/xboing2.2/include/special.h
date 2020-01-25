#ifndef _SPECIAL_H_
#define _SPECIAL_H_

/*
 * XBoing - An X11 blockout style computer game
 *
 * (c) Copyright 1993, 1994, Justin C. Kibell, All Rights Reserved
 *
 * The X Consortium, and any party obtaining a copy of these files from
 * the X Consortium, directly or indirectly, is granted, free of charge, a
 * full and unrestricted irrevocable, world-wide, paid up, royalty-free,
 * nonexclusive right and license to deal in this software and
 * documentation files (the "Software"), including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons who receive
 * copies from any such party to do so.  This license includes without
 * limitation a license to do the foregoing actions under any patents of
 * the party supplying this software to the X Consortium.
 *
 * In no event shall the author be liable to any party for direct, indirect,
 * special, incidental, or consequential damages arising out of the use of
 * this software and its documentation, even if the author has been advised
 * of the possibility of such damage.
 *
 * The author specifically disclaims any warranties, including, but not limited
 * to, the implied warranties of merchantability and fitness for a particular
 * purpose.  The software provided hereunder is on an "AS IS" basis, and the
 * author has no obligation to provide maintenance, support, updates,
 * enhancements, or modifications.
 */

/*
 *  Dependencies on other include files:
 */

#include <X11/Xlib.h>

/*
 *  Constants and macros:
 */

#define REVERSE     1
#define STICKY      2

#define FLASH		500

/*
 *  Type declarations:
 */

/*
 *  Function prototypes:
 */

#if NeedFunctionPrototypes
void DrawSpecials(Display *display);
void ToggleFastGun(Display *display, int state);
void ToggleWallsOn(Display *display, int state);
void TurnSpecialsOff(Display *display);
void ToggleStickyBat(Display *display, int state);
void ToggleSaving(Display *display, int state);
void Togglex2Bonus(Display *display, int state);
void Togglex4Bonus(Display *display, int state);
void ToggleKiller(Display *display, int state);
void RandomDrawSpecials(Display *display);
#else
void RandomDrawSpecials();
void ToggleKiller();
void TurnSpecialsOff();
void DrawSpecials();
void ToggleFastGun();
void ToggleWallsOn();
void ToggleStickyBat();
void ToggleSaving();
void Togglex2Bonus();
void Togglex4Bonus();
#endif

extern int saving;
extern int stickyBat;
extern int fastGun;
extern int noWalls;
extern int Killer;
extern int x2Bonus;
extern int x4Bonus;

#endif
