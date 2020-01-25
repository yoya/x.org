/*
 *   Copyright (c) 1994    Frank Adelstein
 *
 *   The X Consortium, and any party obtaining a copy of these files from
 *   the X Consortium, directly or indirectly, is granted, free of charge, a
 *   full and unrestricted irrevocable, world-wide, paid up, royalty-free,
 *   nonexclusive right and license to deal in this software and
 *   documentation files (the "Software"), including without limitation the
 *   rights to use, copy, modify, merge, publish, distribute, sublicense,
 *   and/or sell copies of the Software, and to permit persons who receive
 *   copies from any such party to do so.  This license includes without
 *   limitation a license to do the foregoing actions under any patents of
 *   the party supplying this software to the X Consortium.
 */


/************/
/* INCLUDES */
/************/

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>

#include <X11/Xaw/AsciiText.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/Scrollbar.h>
#include <X11/Xaw/Toggle.h>
#include <X11/Xaw/Viewport.h>

#include <X11/cursorfont.h>

#include <stdio.h>
#include <errno.h>
#include <math.h>
#include <signal.h>

/*************/
/* DEFINES   */
/*************/

#define TITLE     "xlax"
#define CLASS     "Xlax"
#define MAXEVENTS 150

/*************/
/* types     */
/*************/

typedef struct {
  Window wind;
  int    active;
  XKeyEvent events[MAXEVENTS];
  int    eventindex;
  Widget button;
  Widget text;
} Windows_t;

/*************/
/* GLOBALS   */
/*************/

void SetupInterface();
void selectCB();
void killCB();
void keyboardCB();
void toggleCB();
void quitCB();
void senditCB();
void togglestringCB();
void stringinputCB();
