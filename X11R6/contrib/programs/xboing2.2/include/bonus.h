#ifndef _BONUS_H_
#define _BONUS_H_

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

#define SMALL_TITLE_WIDTH		237
#define SMALL_TITLE_HEIGHT		37

#define SMALL_TITLE_WC		(SMALL_TITLE_WIDTH / 2)
#define SMALL_TITLE_HC		(SMALL_TITLE_HEIGHT / 2)

#define MAX_BONUS           8

/*
 *  Type declarations:
 */

enum BonusStates 
{ 
	BONUS_TEXT, 
	BONUS_SCORE, 
	BONUS_BONUS, 
	BONUS_LEVEL, 
	BONUS_BULLET, 
	BONUS_TIME, 
	BONUS_HSCORE, 
	BONUS_END_TEXT, 
	BONUS_WAIT, 
	BONUS_FINISH
};

/*
 *  Function prototypes:
 */

#if NeedFunctionPrototypes
void SetUpBonus(Display *display, Window window, Colormap colormap);
void DoBonus(Display *display, Window window);
void RedrawBonus(Display *display, Window window);
void FreeBonus(Display *display);
void ResetBonus(void);
void SetupBonusScreen(Display *display, Window window);
void IncNumberBonus(void);
void ResetNumberBonus(void);
void DrawSmallIntroTitle(Display *display, Window window, int x, int y);
void MoveSmallIntroTitle(Display *display, Window window, int x, int y);
void ComputeAndAddBonusScore(void);
void SetBonusWait(enum BonusStates newMode, int waitFrame);
int GetNumberBonus(void);
#else
int GetNumberBonus();
void SetBonusWait();
void ComputeAndAddBonusScore();
void DrawSmallIntroTitle();
void MoveSmallIntroTitle();
void SetUpBonus();
void DoBonus();
void RedrawBonus();
void FreeBonus();
void ResetBonus();
void SetupBonusScreen();
void IncNumberBonus();
void ResetNumberBonus();
#endif


extern enum BonusStates BonusState;

#endif
