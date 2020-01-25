#ifndef _GUN_H_
#define _GUN_H_

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

#define MAX_BULLETS         20

/*
 *  Type declarations:
 */

/*
 *  Function prototypes:
 */

#if NeedFunctionPrototypes
void InitialiseBullet(Display *display, Window window, Colormap colormap);
void FreeBullet(Display *display);
void SetNumberBullets(int num);
void IncNumberBullets(void);
void DecNumberBullets(void);
int GetNumberBullets(void);
void shootBullet(Display *display, Window window);
void DrawTheBullet(Display *display, Window window, int x, int y);
void EraseTheBullet(Display *display, Window window, int x, int y);
void HandleBulletMode(Display *display, Window window);
void ClearBullets(void);
void SetUnlimitedBullets(int state);
#else
void SetUnlimitedBullets();
void InitialiseBullet();
void FreeBullet();
void SetNumberBullets();
void IncNumberBullets();
void DecNumberBullets();
int GetNumberBullets();
void shootBullet();
void DrawTheBullet();
void EraseTheBullet();
void HandleBulletMode();
void ClearBullets();
#endif

#endif
