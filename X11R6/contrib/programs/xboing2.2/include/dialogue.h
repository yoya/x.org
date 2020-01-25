#ifndef _DIALOGUE_H_
#define _DIALOGUE_H_

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

#define DIALOGUE_WIDTH 		((int) (PLAY_WIDTH / 1.3))
#define DIALOGUE_HEIGHT 	120

#define DISK_ICON			1
#define TEXT_ICON			2

#define TEXT_ENTRY_ONLY		1
#define NUMERIC_ENTRY_ONLY	2
#define ALL_ENTRY			3
#define YES_NO_ENTRY		4

/*
 *  Type declarations:
 */

enum DialogueStates
{
    DIALOGUE_MAP,
    DIALOGUE_UNMAP,
    DIALOGUE_TEXT,
    DIALOGUE_NONE,
    DIALOGUE_FINISHED
};

/*
 *  Function prototypes:
 */

#if NeedFunctionPrototypes
void ProcessDialogue(Display *display);
void handleDialogueKeys(Display *display, XEvent event);
char *UserInputDialogueMessage(Display *display, char *message, int type,
	int entryValidation);
void FreeDialoguePixmaps(Display *display);
void InitialiseDialoguePixmaps(Display *display, Window window, 
		Colormap colormap);
#else
void FreeDialoguePixmaps();
void InitialiseDialoguePixmaps();
char *UserInputDialogueMessage();
void ProcessDialogue();
void handleDialogueKeys();
#endif

extern Pixmap question, questionM;
extern Pixmap floppy, floppyM;
extern Pixmap text, textM;

#endif
