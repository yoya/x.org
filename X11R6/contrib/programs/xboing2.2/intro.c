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
 *  Include file dependencies:
 */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <xpm.h>

#include "error.h"
#include "misc.h"
#include "gun.h"
#include "main.h"
#include "init.h"
#include "inst.h"
#include "stage.h"
#include "blocks.h"
#include "sfx.h"
#include "special.h"
#include "ball.h"
#include "score.h"
#include "paddle.h"
#include "level.h"
#include "mess.h"
#include "audio.h"
#include "version.h"

#include "bitmaps/presents/titleBig.xpm"

#include "bitmaps/stars/star1.xpm"
#include "bitmaps/stars/star2.xpm"
#include "bitmaps/stars/star3.xpm"
#include "bitmaps/stars/star4.xpm"
#include "bitmaps/stars/star5.xpm"
#include "bitmaps/stars/star6.xpm"
#include "bitmaps/stars/star7.xpm"
#include "bitmaps/stars/star8.xpm"
#include "bitmaps/stars/star9.xpm"
#include "bitmaps/stars/star10.xpm"
#include "bitmaps/stars/star11.xpm"

#include "intro.h"

/*
 *  Internal macro definitions:
 */

#define GAP 10
#define BLINK_GAP	1000
#define BLINK_RATE	25

/*
 *  Internal type declarations:
 */

#if NeedFunctionPrototypes
void SetIntroWait(enum IntroStates newMode, int waitFrame);
void DoIntroWait(void);
#else
void SetIntroWait();
void DoIntroWait();
#endif

/*
 *  Internal variable declarations:
 */

static int endFrame = 0;
static int startFrame = 0;
static int nextBlink = 0;
enum IntroStates IntroState;
Pixmap bigtitlePixmap, bigtitlePixmapM;
Pixmap stars[12], starsM[12];
static int waitingFrame;
enum IntroStates waitMode;

#if NeedFunctionPrototypes
void SetUpIntroduction(Display *display, Window window, Colormap colormap)
#else
void SetUpIntroduction(display, window, colormap)
	Display *display;
	Window window;
	Colormap colormap;
#endif
{
	XpmAttributes   attributes;
	int             XpmErrorStatus;

    attributes.valuemask = XpmColormap;
	attributes.colormap = colormap;

	XpmErrorStatus = XpmCreatePixmapFromData(display, window, titleBig_xpm,
		&bigtitlePixmap, &bigtitlePixmapM, &attributes);
	HandleXPMError(display, XpmErrorStatus, "InitialiseIntro()");

    /* Create the stars for the letter shine */
	XpmErrorStatus = XpmCreatePixmapFromData(display, window, star1_xpm,
		&stars[0], &starsM[0], &attributes);
	HandleXPMError(display, XpmErrorStatus, "InitialiseIntro(star1)");

	XpmErrorStatus = XpmCreatePixmapFromData(display, window, star2_xpm,
		&stars[1], &starsM[1], &attributes);
	HandleXPMError(display, XpmErrorStatus, "InitialiseIntro(star2)");

	XpmErrorStatus = XpmCreatePixmapFromData(display, window, star3_xpm,
		&stars[2], &starsM[2], &attributes);
	HandleXPMError(display, XpmErrorStatus, "InitialiseIntro(star3)");

	XpmErrorStatus = XpmCreatePixmapFromData(display, window, star4_xpm,
		&stars[3], &starsM[3], &attributes);
	HandleXPMError(display, XpmErrorStatus, "InitialiseIntro(star4)");

	XpmErrorStatus = XpmCreatePixmapFromData(display, window, star5_xpm,
		&stars[4], &starsM[4], &attributes);
	HandleXPMError(display, XpmErrorStatus, "InitialiseIntro(star5)");

	XpmErrorStatus = XpmCreatePixmapFromData(display, window, star6_xpm,
		&stars[5], &starsM[5], &attributes);
	HandleXPMError(display, XpmErrorStatus, "InitialiseIntro(star6)");

	XpmErrorStatus = XpmCreatePixmapFromData(display, window, star7_xpm,
		&stars[6], &starsM[6], &attributes);
	HandleXPMError(display, XpmErrorStatus, "InitialiseIntro(star7)");

	XpmErrorStatus = XpmCreatePixmapFromData(display, window, star8_xpm,
		&stars[7], &starsM[7], &attributes);
	HandleXPMError(display, XpmErrorStatus, "InitialiseIntro(star8)");

	XpmErrorStatus = XpmCreatePixmapFromData(display, window, star9_xpm,
		&stars[8], &starsM[8], &attributes);
	HandleXPMError(display, XpmErrorStatus, "InitialiseIntro(star9)");

	XpmErrorStatus = XpmCreatePixmapFromData(display, window, star10_xpm,
		&stars[9], &starsM[9], &attributes);
	HandleXPMError(display, XpmErrorStatus, "InitialiseIntro(star10)");

	XpmErrorStatus = XpmCreatePixmapFromData(display, window, star11_xpm,
		&stars[10], &starsM[10], &attributes);
	HandleXPMError(display, XpmErrorStatus, "InitialiseIntro(star11)");

    /* Free the xpm pixmap attributes */
	XpmFreeAttributes(&attributes);

	ResetIntroduction();
}

#if NeedFunctionPrototypes
void DrawIntroTitle(Display *display, Window window, int x, int y)
#else
void DrawIntroTitle(display, window, x, y)
	Display *display;
	Window window;
	int x;
	int y;
#endif
{
	RenderShape(display, window, bigtitlePixmap, bigtitlePixmapM,
		x, y, 474, 74, True);
}

#if NeedFunctionPrototypes
void DoIntroTitle(Display *display, Window window)
#else
void DoIntroTitle(display, window)
	Display *display;
	Window window;
#endif
{
	/* Clear and draw background pattern */
    DrawStageBackground(display, window, BACKGROUND_0, True);

	/* Draw the intro title bitmap XBOING */
	DrawIntroTitle(display, window, 10, 10);
}

#if NeedFunctionPrototypes
static void DoBlocks(Display *display, Window window)
#else
static void DoBlocks(display, window)
	Display *display;
	Window window;
#endif
{
	int y = 120;
	int x = 40;

	/* Be very friendly */
	SetCurrentMessage(display, messWindow, "Welcome to XBoing", False);

	/* Now draw the blocks on the screen */
	DrawTheBlock(display, window, x, y, RED_BLK, 0, 0, 0);
	DrawShadowText(display, window, textFont, 
		"- Normal block", x + 60, y, green);
	y += 40;

	DrawTheBlock(display, window, x+6, y, ROAMER_BLK, 0, 0, 0);	
	DrawShadowText(display, window, textFont, 
		"- Roamer dude", x + 60, y, green);
	y += 40;

	DrawTheBlock(display, window, x, y, PAD_EXPAND_BLK, 0, 0, 0);
	DrawShadowText(display, window, textFont, 
		"- Paddle sizer", x + 60, y, green);
	y += 40;

	DrawTheBlock(display, window, x+5, y, BONUSX2_BLK, 0, 0, 0);	
	DrawShadowText(display, window, textFont, 
		"- x2 scores", x + 60, y, green);
	y += 40;

	DrawTheBlock(display, window, x, y, MAXAMMO_BLK, 0, 0, 0);
	DrawShadowText(display, window, textFont, 
		"- Full Ammo", x + 60, y, green);
	y += 40;

	DrawTheBlock(display, window, x, y, DROP_BLK, 0, 0, 0);	
	DrawShadowText(display, window, textFont, 
		"- Dropper", x + 60, y, green);
	y += 40;

	DrawTheBlock(display, window, x, y, BULLET_BLK, 0, 0, 0);	
	DrawShadowText(display, window, textFont, 
		"- Ammunition", x + 60, y, green);
	y += 40;

	DrawTheBlock(display, window, x+5, y-5, HYPERSPACE_BLK, 0, 0, 0);	
	DrawShadowText(display, window, textFont, 
		"- Teleport", x + 60, y, green);
	y += 40;

	DrawTheBlock(display, window, x+5, y+5, REVERSE_BLK, 0, 0, 0);	
	DrawShadowText(display, window, textFont, 
		"- Reverse Control", x + 60, y, green);
	y += 40;

	DrawTheBlock(display, window, x+5, y+5, MGUN_BLK, 0, 0, 0);	
	DrawShadowText(display, window, textFont, 
		"- Machine Gun", x + 60, y, green);
	y += 40;

	DrawTheBlock(display, window, x, y, MULTIBALL_BLK, 0, 0, 0);
	DrawShadowText(display, window, textFont, 
		"- Multi Balls", x + 60, y, green);
	y += 40;

	/* New line from top again on right */
	y = 120;
	x = 260;

	DrawTheBlock(display, window, x+5, y, BONUS_BLK, 0, 0, 0);
	DrawShadowText(display, window, textFont, 
		"- 3000 points", x + 60, y, green);
	y += 40;

	DrawTheBlock(display, window, x, y, COUNTER_BLK, 5, 0, 0);
	DrawShadowText(display, window, textFont, 
		"- 200 points", x + 60, y, green);
	y += 40;

	DrawTheBlock(display, window, x+10, y, TIMER_BLK, 5, 0, 0);
	DrawShadowText(display, window, textFont, 
		"- Extra Time", x + 60, y, green);
	y += 40;

	DrawTheBlock(display, window, x, y, BLACK_BLK, 0, 0, 0);
	DrawShadowText(display, window, textFont, 
		"- Solid wall", x + 60, y, green);
	y += 40;

	DrawTheBlock(display, window, x+9, y, BOMB_BLK, 0, 0, 0);
	DrawShadowText(display, window, textFont, 
		"- Bomb!", x + 60, y, green);
	y += 40;

	DrawPaddle(display, window, x+20, y, PADDLE_SMALL);
	DrawShadowText(display, window, textFont, 
		"- The Paddle", x + 60, y, green);
	y += 40;

	DrawTheBullet(display, window, x+20, y+5);
	DrawShadowText(display, window, textFont, 
		"- Bullet", x + 60, y, green);
	y += 40;

	DrawTheBlock(display, window, x+8, y, DEATH_BLK, 0, 0, 0);
	DrawShadowText(display, window, textFont, 
		"- Instant Death!", x + 60, y, green);
	y += 40;

	DrawTheBlock(display, window, x+10, y, EXTRABALL_BLK, 1, 0, 0);
	DrawShadowText(display, window, textFont, 
		"- Extra Ball", x + 60, y, green);
	y += 40;

	DrawTheBlock(display, window, x+10, y, WALLOFF_BLK, 0, 0, 0);
	DrawShadowText(display, window, textFont, 
		"- Walls Off", x + 60, y, green);
	y += 40;

	DrawTheBlock(display, window, x+10, y - 5, STICKY_BLK, 0, 0, 0);
	DrawShadowText(display, window, textFont, 
		"- Sticky Ball", x + 60, y, green);
	y += 40;
}

#if NeedFunctionPrototypes
static void DoText(Display *display, Window window)
#else
static void DoText(display, window)
	Display *display;
	Window window;
#endif
{
	char string[80];
	int y;

	y = PLAY_HEIGHT - 27;

	strcpy(string, "Insert coin to start the game");
	DrawShadowCentredText(display, window, textFont, 
		string, y, tann, PLAY_WIDTH);
}

#if NeedFunctionPrototypes
static void DoSparkle(Display *display, Window window)
#else
static void DoSparkle(display, window)
	Display *display;
	Window window;
#endif
{
	static Pixmap store;
	static int x = 100;
	static int y = 20;
	static int in = 0;

	if (!store)
	{
		store = XCreatePixmap(display, window, 20, 20,
			DefaultDepth(display, XDefaultScreen(display)));
	}

	if (in == 0) 
		XCopyArea(display, window, store, gc, x, y, 20, 20, 0, 0);

	if (frame == endFrame)
		IntroState = INTRO_FINISH;

	if (frame == startFrame)
	{
		XCopyArea(display, store, window, gc, 0, 0, 20, 20, x, y);
		RenderShape(display, window, stars[in], starsM[in],
			x, y, 20, 20, False);

	 	in++;
		startFrame = frame + 15;

		if (in == 11) 
		{
			XCopyArea(display, store, window, gc, 0, 0, 20, 20, x, y);
			in = 0;
			startFrame = frame + 500;
			x = (rand() % 474) + 5;
			y = (rand() % 74) + 5;
		}	
	}
}

#if NeedFunctionPrototypes
static void DoFinish(Display *display, Window window)
#else
static void DoFinish(display, window)
	Display *display;
	Window window;
#endif
{
	ResetInstructions();
	mode = MODE_INSTRUCT;

	if (noSound == False)
		playSoundFile("whoosh", 50);
}


#if NeedFunctionPrototypes
void HandleBlink(Display *display, Window window)
#else
void HandleBlink(display, window)
	Display *display;
	Window window;
#endif
{
	if (frame == nextBlink)
	{
		if (BlinkDevilEyes(display, playWindow) == False)
			nextBlink = frame + BLINK_GAP;	
		else
			nextBlink = frame + BLINK_RATE;	
	}
}

#if NeedFunctionPrototypes
void Introduction(Display *display, Window window)
#else
void Introduction(display, window)
	Display *display;
	Window window;
#endif
{
	static int firstTime = True;

	switch (IntroState)
	{
		case INTRO_TITLE:
			if (getSpecialEffects(display) == True && !firstTime)
				DoIntroTitle(display, bufferWindow);
			else
				DoIntroTitle(display, window);
			IntroState = INTRO_BLOCKS;
			break;

		case INTRO_BLOCKS:
			if (getSpecialEffects(display) == True && !firstTime)
				DoBlocks(display, bufferWindow);
			else
				DoBlocks(display, window);
			IntroState = INTRO_TEXT;
			break;

		case INTRO_TEXT:
			if (getSpecialEffects(display) == True && !firstTime)
			{
				DoText(display, bufferWindow);
				while (WindowShatterEffect(display, window));
			}
			else
				DoText(display, window);
			if (firstTime) firstTime = False;
			IntroState = INTRO_EXPLODE;
			break;

		case INTRO_EXPLODE:
			DoSparkle(display, window);
			if ((frame % FLASH) == 0)
				RandomDrawSpecials(display);
			BorderGlow(display, window);
			HandleBlink(display, window);
			break;

		case INTRO_FINISH:
			DoFinish(display, window);
			break;

		case INTRO_WAIT:
			DoIntroWait();
			break;

		default:
			break;
	}
}

#if NeedFunctionPrototypes
void RedrawIntroduction(Display *display, Window window)
#else
void RedrawIntroduction(display, window)
	Display *display;
	Window window;
#endif
{
	DoIntroTitle(display, window);
	DoBlocks(display, window);
	DoText(display, window);
}

#if NeedFunctionPrototypes
void FreeIntroduction(Display *display)
#else
void FreeIntroduction(display)
	Display *display;
#endif
{
	int i;

	for (i = 0; i < 10; i++)
	{
    	if (stars[i])	XFreePixmap(display, stars[i]);         
    	if (starsM[i])	XFreePixmap(display, starsM[i]);         
	}

    if (bigtitlePixmap)		XFreePixmap(display, bigtitlePixmap);         
	if (bigtitlePixmapM)	XFreePixmap(display, bigtitlePixmapM);
}

#if NeedFunctionPrototypes
void ResetIntroduction(void)
#else
void ResetIntroduction()
#endif
{
	IntroState = INTRO_TITLE;
	startFrame = frame + 10;
	endFrame = frame + 3000;
	nextBlink = frame + 10;

	DEBUG("Reset Introduction mode.")
}

#if NeedFunctionPrototypes
void SetIntroWait(enum IntroStates newMode, int waitFrame)
#else
void SetIntroWait(newMode, waitFrame)
	enum IntroStates newMode;
	int waitFrame;
#endif
{
	waitingFrame = waitFrame;
	waitMode = newMode;
	IntroState = INTRO_WAIT;
}

#if NeedFunctionPrototypes
void DoIntroWait(void)
#else
void DoIntroWait()
#endif
{
	if (frame == waitingFrame)
		IntroState = waitMode;
}
