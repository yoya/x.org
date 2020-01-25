#ifndef _INTRO_H_
#define _INTRO_H_

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

/*
 *  Type declarations:
 */

enum IntroStates 
{ 
	INTRO_TITLE, 
	INTRO_BLOCKS, 
	INTRO_TEXT, 
	INTRO_EXPLODE, 
	INTRO_WAIT, 
	INTRO_FINISH 
};

/*
 *  Function prototypes:
 */

#if NeedFunctionPrototypes
void SetUpIntroduction(Display *display, Window window, Colormap colormap);
void Introduction(Display *display, Window window);
void RedrawIntroduction(Display *display, Window window);
void FreeIntroduction(Display *display);
void ResetIntroduction(void);
void DoIntroTitle(Display *display, Window window);
void DrawIntroTitle(Display *display, Window window, int x, int y);
#else
void DrawIntroTitle();
void SetUpIntroduction();
void Introduction();
void RedrawIntroduction();
void FreeIntroduction();
void ResetIntroduction();
void DoIntroTitle();
#endif

extern 	enum IntroStates IntroState;
extern 	Pixmap bigtitlePixmap, bigtitlePixmapM;
extern	Pixmap stars[12], starsM[12];


#endif
