#ifndef _SFX_H_
#define _SFX_H_

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

#define SFX_NONE        0
#define SFX_SHAKE      	1
#define SFX_FADE  		2
#define SFX_BLIND  		3
#define SFX_SHATTER  	4
#define SFX_STATIC  	5

/*
 *  Type declarations:
 */

/*
 *  Function prototypes:
 */

#if NeedFunctionPrototypes
void SetSfxEndFrame(int endFrame);
void changeSfxMode(int newMode);
int oldSfxMode(void);
int currentSfxMode(void);
int WindowShakeEffect(Display *display, Window window);
int WindowShatterEffect(Display *display, Window window);
int WindowBlindEffect(Display *display, Window window);
int WindowFadeEffect(Display *display, Window window, int w, int h);
int WindowStaticEffect(Display *display, Window window, int w, int h);
int getSpecialEffects(Display *display);
void useSpecialEffects(int state);
void ResetBorderGlow(Display *display, Window window);
void BorderGlow(Display *display, Window window);
void FadeAwayArea(Display *display, Window window, int x, int y, int w, int h);
#else
void FadeAwayArea();
void ResetBorderGlow();
int WindowShatterEffect();
int WindowBlindEffect();
int WindowFadeEffect();
int WindowShakeEffect();
int WindowStaticEffect();
void BorderGlow();
void useSpecialEffects();
int getSpecialEffects();
void SetSfxEndFrame();
void changeSfxMode();
int oldSfxMode();
int currentSfxMode();
#endif


#endif
