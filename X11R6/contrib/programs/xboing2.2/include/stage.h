#ifndef _STAGE_H_
#define _STAGE_H_

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

#define MAIN_WIDTH  70
#define MAIN_HEIGHT 130

#define PLAY_WIDTH  495
#define PLAY_HEIGHT 580

#define TOTAL_WIDTH  (MAIN_WIDTH + PLAY_WIDTH)
#define TOTAL_HEIGHT (MAIN_HEIGHT + PLAY_HEIGHT)

#define MESS_HEIGHT 30
#define TYPE_HEIGHT (MESS_HEIGHT + 5)

#define BACKGROUND_WHITE   -2
#define BACKGROUND_BLACK   -1
#define BACKGROUND_0    	0
#define BACKGROUND_1    	1
#define BACKGROUND_2    	2
#define BACKGROUND_3    	3
#define BACKGROUND_4    	4
#define BACKGROUND_5    	5
#define BACKGROUND_6    	6
#define BACKGROUND_SEE_THRU 10
#define BACKGROUND_SPACE 	11

/*
 *  Type declarations:
 */

/*
 *  Function prototypes:
 */

extern Window mainWindow;
extern Window scoreWindow;
extern Window levelWindow;
extern Window playWindow, bufferWindow;
extern Window messWindow;
extern Window specialWindow;
extern Window timeWindow;
extern Window inputWindow;
extern Window blockWindow, typeWindow;

#if NeedFunctionPrototypes
void CreateAllWindows(Display *display, Colormap colormap, char **argv, 
	int argc);
void RedrawPlayWindow(Display *display, Window window);
void MapAllWindows(Display *display);
void ClearMainWindow(Display *display, Window window);
void DrawStageBackground(Display *display, Window window, int stageType,
	int clear);
void SetBackgrounds(Display *display, Colormap colormap);
int BlinkDevilEyes(Display *display, Window window);
void SetWindowSizeHints(Display *display, int w, int h);
#else
void SetWindowSizeHints();
int BlinkDevilEyes();
void CreateAllWindows();
void RedrawPlayWindow();
void MapAllWindows();
void ClearMainWindow();
void DrawStageBackground();
void SetBackgrounds();
#endif


#endif
