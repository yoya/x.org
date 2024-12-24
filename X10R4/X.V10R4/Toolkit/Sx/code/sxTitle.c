/*
 *	$Source: /u1/Sx.new/code/RCS/sxTitle.c,v $
 *	$Header: sxTitle.c,v 1.1 86/12/03 16:11:04 swick Exp $
 */

#ifndef lint
static char *rcsid_sxTitle_c = "$Header: sxTitle.c,v 1.1 86/12/03 16:11:04 swick Exp $";
#endif	lint

/* 
 * sxTitle.c --
 *
 *	This file implements title windows using the facilities of the X
 *	window package and the Sx dispatcher.  Titles are rectangular
 *	windows that display up to three pieces of text:  one on the
 *	left, one in the center, and one on the right.  They are
 *	"output-only" windows:  keystrokes and mouse actions have no
 *	impact on them.
 *
 * Copyright (C) 1986 Regents of the University of California.
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appear in all copies.  The University of California
 * makes no representations about the suitability of this
 * software for any purpose.  It is provided "as is" without
 * express or implied warranty.
 */

#ifndef lint
static char rcsid[] = "$Header: sxTitle.c,v 1.1 86/12/03 16:11:04 swick Exp $ SPRITE (Berkeley)";
#endif not lint

#include <sys/types.h>
#include <X/Xlib.h>
#include "sprite.h"
#include "mem.h"
#include "sx.h"
#include "sxInt.h"
#include "string.h"

/*
 * For each title there is one structure of the type defined below,
 * which is used to hold information about the title.
 */

typedef struct Title {
    Window w;			/* Window corresponding to title. */
    int width, height;		/* Dimensions of w, in pixels. */
    FontInfo *fontPtr;		/* How to draw text. */
    int foreground, background;	/* Colors to use for display. */
    char *leftText;		/* String to display left-justified in
				 * title (space is dynamically allocated). */
    int leftChars;		/* Number of non-null chars. in leftText. */
    char *centerText;		/* String to display in center. */
    int centerChars;		/* Number of non-null chars. in centerText. */
    int centerWidth;		/* Width of centerText in pixels. */
    char *rightText;		/* String to display on right. */
    int rightChars;		/* Number of non-null chars. in rightText. */
    int rightWidth;		/* Width of rightText in pixels. */
} Title;

static Boolean init = FALSE;

/*
 * Margins to use at edges of text:
 */

#define MARGIN 2

/*
 * The assoc table used below maps from X window ids to Title structures.
 */

static XAssocTable *titleTable = NULL;
extern XAssocTable *XCreateAssocTable();

/*
 * Forward references:
 */

extern void	TitleInit();
extern void	TitleDestroyProc();
extern void	TitleExposeProc();
extern void	TitleRedisplay();

/*
 *----------------------------------------------------------------------
 *
 * Sx_TitleMake --
 *
 *	This procedure turns an ordinary window into a title window.
 *	If the window is already a title window, then its parameters
 *	are changed.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	From now on, this window is a title window.  The given strings
 *	will be displayed in the window, in the given font and colors.
 *
 *----------------------------------------------------------------------
 */

void
Sx_TitleMake(window, fontPtr, foreground, background,
	leftText, centerText, rightText)
    Window window;			/* Window that is to become a title
					 * window.  If this isn't already a
					 * title window, then it shouldn't
					 * be mapped.  If it's already a title
					 * window, its parameters will be
					 * changed and the window will be
					 * redisplayed if it's mapped. */
    FontInfo *fontPtr;			/* Font in which to display text.  If
					 * NULL, a default font is used. */
    int foreground;			/* Color in which to display text. */
    int background;			/* Color for background of title. */
    char *leftText;			/* Text to display left-justified in
					 * window.  If NULL, no left-justified
					 * text is displayed. */
    char *centerText;			/* Text to display in center of window,
					 * or NULL. */
    char *rightText;			/* Text to display right-justified in
					 * window, or NULL. */
{
    register Title *titlePtr;

    if (!init) {
	TitleInit();
    }

    if (fontPtr == NULL) {
	fontPtr = Sx_GetDefaultFont();
    }

    /*
     * See if this window is already a title window.  If so, then
     * prepare it for re-allocation.  If not, then make a new Title
     * structure.
     */
    
    titlePtr = (Title *) XLookUpAssoc(titleTable, window);
    if (titlePtr != NULL) {
	if (titlePtr->leftText != NULL) {
	    Mem_Free(titlePtr->leftText);
	}
	if (titlePtr->centerText != NULL) {
	    Mem_Free(titlePtr->centerText);
	}
	if (titlePtr->rightText != NULL) {
	    Mem_Free(titlePtr->rightText);
	}
    } else {
	titlePtr = (Title *) Mem_Alloc(sizeof(Title));
	titlePtr->w = window;
	titlePtr->width = titlePtr->height = 0;
	(void) Sx_HandlerCreate(window, ExposeWindow, TitleExposeProc,
		(ClientData) titlePtr);
	(void) Sx_HandlerCreate(window, SX_DESTROYED, TitleDestroyProc,
		(ClientData) titlePtr);
	(void) Sx_HandlerCreate(window, KeyPressed|KeyReleased
		|ButtonPressed|ButtonReleased|EnterWindow|LeaveWindow,
		Sx_NullProc, (ClientData) NULL);
	XMakeAssoc(titleTable, window, (caddr_t) titlePtr);
    }

    /*
     * Initialize the contents of the title.
     */

    titlePtr->fontPtr = fontPtr;
    titlePtr->foreground = foreground;
    titlePtr->background = background;
    if (leftText != NULL) {
	titlePtr->leftChars = String_Length(leftText);
	titlePtr->leftText = Mem_Alloc(titlePtr->leftChars + 1);
	(void) String_Copy(leftText, titlePtr->leftText);
    } else {
	titlePtr->leftText = NULL;
	titlePtr->leftChars = 0;
    }
    if (centerText != NULL) {
	titlePtr->centerChars = String_Length(centerText);
	titlePtr->centerText = Mem_Alloc(titlePtr->centerChars + 1);
	(void) String_Copy(centerText, titlePtr->centerText);
	titlePtr->centerWidth = XStringWidth(centerText, fontPtr, 0, 0);
    } else {
	titlePtr->centerText = NULL;
	titlePtr->centerChars = titlePtr->centerWidth = 0;
    }
    if (rightText != NULL) {
	titlePtr->rightChars = String_Length(rightText);
	titlePtr->rightText = Mem_Alloc(titlePtr->rightChars + 1);
	(void) String_Copy(rightText, titlePtr->rightText);
	titlePtr->rightWidth = XStringWidth(rightText, fontPtr, 0, 0);
    } else {
	titlePtr->rightText = NULL;
	titlePtr->rightChars = titlePtr->rightWidth = 0;
    }

    if ((titlePtr->width > 0) && (titlePtr->height > 0)) {
	TitleRedisplay(titlePtr);
    }
}

/*
 *----------------------------------------------------------------------
 *
 * Sx_TitleCreate --
 *
 *	This procedure is like Sx_TitleMake except that it creates the
 *	title window and also packs it into a parent.
 *
 * Results:
 *	The return value is a pointer to a newly-created title window,
 *	that will appear on "side" of "parent" and have its geometry
 *	managed by the Sx packer.
 *
 * Side effects:
 *	The given strings will be displayed in the window, in the given
 *	font and colors.
 *
 *----------------------------------------------------------------------
 */

Window
Sx_TitleCreate(parent, side, size, border, fontPtr, foreground, background,
	leftText, centerText, rightText)
    Window parent;			/* Parent window in which to create
					 * the new title window. */
    Sx_Side side;			/* Side of parent on which to create
					 * the new title (normally SX_TOP or
					 * SX_BOTTOM). */
    int size;				/* Size of new window perpendicular to
					 * length of side. */
    int border;				/* Width of border for new window. */
    FontInfo *fontPtr;			/* Font in which to display text.  If
					 * NULL, a default font is used. */
    int foreground;			/* Color in which to display text. */
    int background;			/* Color for background of title. */
    char *leftText;			/* Text to display left-justified in
					 * window.  If NULL, no left-justified
					 * text is displayed. */
    char *centerText;			/* Text to display in center of window,
					 * or NULL. */
    char *rightText;			/* Text to display right-justified in
					 * window, or NULL. */
{
    Window w;
    Pixmap borderPixmap;

    if ((foreground == BlackPixel) || (border == 0)) {
	borderPixmap = BlackPixmap;
    } else if (foreground == WhitePixel) {
	borderPixmap = WhitePixmap;
    } else {
	borderPixmap = XMakeTile(foreground);
	if (borderPixmap == NULL) {
	    Sx_Panic("Sx_TitleCreate:  couldn't create border pixmap.");
	}
    }
    w = Sx_CreatePacked(parent, side, size, border, (Window) 0, (Window) 0,
	    borderPixmap, WhitePixmap);
    if ((borderPixmap != BlackPixmap) && (borderPixmap != WhitePixmap)) {
	XFreePixmap(borderPixmap);
    }
    Sx_TitleMake(w, fontPtr, foreground, background, leftText, centerText,
	    rightText);
    return w;
}

/*
 *----------------------------------------------------------------------
 *
 * TitleDestroyProc --
 *
 *	Called by the Sx dispatcher whenever a title window is
 *	destroyed.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	The additional data structures associated with the title
 *	are deleted.
 *
 *----------------------------------------------------------------------
 */

static void
TitleDestroyProc(titlePtr)
    register Title *titlePtr;		/* Internal info about title. */
{
    XDeleteAssoc(titleTable, titlePtr->w);
    if (titlePtr->leftText != NULL) {
	Mem_Free(titlePtr->leftText);
    }
    if (titlePtr->centerText != NULL) {
	Mem_Free(titlePtr->centerText);
    }
    if (titlePtr->rightText != NULL) {
	Mem_Free(titlePtr->rightText);
    }
    Mem_Free((char *) titlePtr);
}

/*
 *----------------------------------------------------------------------
 *
 * TitleInit --
 *
 *	This procedure is called once only to initialize shared
 *	variables for the module.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	Random initializations are performed.  See the code.
 *
 *----------------------------------------------------------------------
 */

static void
TitleInit()
{
    titleTable = XCreateAssocTable(8);

    if (titleTable == NULL) {
	Sx_Panic("Sx_TitleMake: couldn't initialize cursors and/or hash table.");
    }
    init = TRUE;
}

/*
 *----------------------------------------------------------------------
 *
 * TitleRedisplay --
 *
 *	Redraw the information in a title window.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	The title gets redrawn.
 *
 *----------------------------------------------------------------------
 */

static void
TitleRedisplay(titlePtr)
    register Title *titlePtr;	/* Button to redisplay. */
{
    int y;

    XPixSet(titlePtr->w, 0, 0, titlePtr->width, titlePtr->height,
	    titlePtr->background);
    y = (titlePtr->height - titlePtr->fontPtr->height)/2;
    if (titlePtr->leftText != NULL) {
	XText(titlePtr->w, MARGIN, y, titlePtr->leftText,
		titlePtr->leftChars, titlePtr->fontPtr->id,
		titlePtr->foreground, titlePtr->background);
    }
    if (titlePtr->centerText != NULL) {
	XText(titlePtr->w, (titlePtr->width - titlePtr->centerWidth)/2,
		y, titlePtr->centerText, titlePtr->centerChars,
		titlePtr->fontPtr->id, titlePtr->foreground,
		titlePtr->background);
    }
    if (titlePtr->rightText != NULL) {
	XText(titlePtr->w, titlePtr->width - titlePtr->rightWidth - MARGIN,
		y, titlePtr->rightText, titlePtr->rightChars,
		titlePtr->fontPtr->id, titlePtr->foreground,
		titlePtr->background);
    }
}

/*
 *----------------------------------------------------------------------
 *
 * TitleExposeProc --
 *
 *	This procedure is invoked automatically by the Sx_ dispatcher
 * 	whenever an expose event occurs for a title window.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	The title window gets redisplayed.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
static void
TitleExposeProc(titlePtr, eventPtr)
    Title *titlePtr;			/* Button for which it happened. */
    XExposeEvent *eventPtr;		/* Describes what happened. */
{
    if (eventPtr->subwindow != 0) {
	return;
    }
    titlePtr->width = eventPtr->width;
    titlePtr->height = eventPtr->height;
    TitleRedisplay(titlePtr);
}
