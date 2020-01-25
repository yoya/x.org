#ifndef _LEVEL_H_
#define _LEVEL_H_

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

#define SHOTS_TO_KILL_SPECIAL   3
#define BUF_SIZE 	1024

/*
 *  Type declarations:
 */

/*
 *  Function prototypes:
 */

#if NeedFunctionPrototypes
void InitialiseLevelInfo(Display *display, Window window, Colormap colormap);
void FreeLevelInfo(Display *display);
void DisplayLevelInfo(Display *display, Window window, u_long level);
void CheckGameRules(Display *display, Window window);
void DeadBall(Display *display, Window window);
void DeleteABullet(Display *display);
void AddABullet(Display *display);
void ReDrawBulletsLeft(Display *display);
void RedrawLevelInfo(Display *display, Window window);
void SetLevelNumber(int levelNum);
void SetStartingLevel(int levelNum);
int GetStartingLevel(void);
char *GetLevelName(void);
void DecLevelTimeBonus(Display *display, Window window);
void SetLevelTimeBonus(Display *display, Window window, int seconds);
int GetLevelTimeBonus(void);
void UpdateHighScores(Display *display);
void AddExtraLife(Display *display);
void EndTheGame(Display *display, Window window);
void HandleGameTimer(Display *display, Window window);
void DecExtraLife(Display *display);
int GetNumberLife(void);
void AddToLevelTimeBonus(Display *display, Window window, int seconds);
void ChangeStartingLevel(Display *display);
void SetLivesLeft(int new);
#else
void SetLivesLeft();
void ChangeStartingLevel();
void AddToLevelTimeBonus();
int GetNumberLife();
void DecExtraLife();
void HandleGameTimer();
void EndTheGame();
void AddExtraLife();
void UpdateHighScores();
int GetLevelTimeBonus();
void SetLevelTimeBonus();
void DecLevelTimeBonus();
char *GetLevelName();
void InitialiseLevelInfo();
void FreeLevelInfo();
void DisplayLevelInfo();
void CheckGameRules();
void DeadBall();
void DeleteABullet();
void AddABullet();
void ReDrawBulletsLeft();
void RedrawLevelInfo();
void SetLevelNumber();
void SetStartingLevel();
int GetStartingLevel();
#endif

extern int bonus, livesLeft, bonusBlock;
extern time_t gameTime;
extern u_long level;
extern char levelTitle[BUF_SIZE];


#endif
