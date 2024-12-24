/* 
 * sxButton.c --
 *
 *	This file implements buttons using the facilities of the X
 *	window package and the Sx dispatcher.  Buttons are rectangular
 *	windows with text displayed inside them, which light up when
 *	the cursor passes over them and cause client-supplied procedures
 *	to be invoked when buttoned.
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
static char rcsid[] = "$Header: sxButton.c,v 1.30 86/10/18 14:45:09 ouster Exp $ SPRITE (Berkeley)";
#endif not lint

#include "sprite.h"
#include "mem.h"
#include "sx.h"
#include "sxInt.h"
#include "string.h"

/*
 * For each button there is one structure of the type defined below,
 * which is used to hold information about the button.
 */

typedef struct Button {
    Window w;			/* Window corresponding to button. */
    int width, height;		/* Size of button window. */
    FontInfo *fontPtr;		/* How to draw text. */
    char *text;			/* String to display in button (dynamically
				 * allocated). */
    int textSize;		/* Number of non-NULL chars in text. */
    int textWidth;		/* Width of text, in pixels using fontPtr. */
    int foreground, background;	/* Colors in which to display button. */
    Boolean active;		/* TRUE means cursor is over button so
				 * the foreground and background colors
				 * should be reversed. */
    void (*proc)();		/* Procedure to call when mouse button
				 * is pressed. */
    ClientData clientData;	/* Information to pass to proc. */
} Button;

/*
 * The assoc table used below maps from X window ids to Button structures.
 */

static XAssocTable *buttonTable = NULL;
extern XAssocTable *XCreateAssocTable();

/*
 * The stuff below is used to define the cursors used for button windows.
 */

#include "cursors/dot"
#include "cursors/dotMask"
#include "cursors/star"
#include "cursors/starMask"

static Cursor cursorNoButton;	/* Used when no button has been pressed. */
static Cursor cursorButton;	/* Used when button has been pressed. */

static Boolean init = FALSE;	/* TRUE means initialized OK. */

/*
 * Forward references:
 */

extern void	ButtonInit();
extern void	ButtonDestroyProc();
extern void	ButtonEventProc();
extern void	ButtonRedisplay();

/*
 *----------------------------------------------------------------------
 *
 * Sx_ButtonMake --
 *
 *	This procedure turns an ordinary window into a button window.
 *	If the window is already a button window, then its parameters
 *	are changed.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	From now on, this window is a button window.  The given text
 *	will be displayed centered in the window, in the given colors.
 *	Whenever the cursor passes over the window, its foreground
 *	and background colors are switched.  When a mouse button is
 *	pressed over the window, the given procedure is called.  It
 *	must have the following form:
 *	
 *	void
 *	proc(clientData, buttonPressed, window)
 *	    ClientData clientData;
 *	    int buttonPressed;
 *	    Window window;
 *	{
 *	}
 *
 *	ClientData is just the parameter passed into Sx_ButtonMake;
 *	it can be used by the client in any fashion whatsoever, typically
 *	as a pointer to client-specific information about the button.
 *	ButtonPressed indicates which button was pressed; it has one of
 *	the values RightButton, MiddleButton, or LeftButton, as defined
 *	by the X package.  Window is the X token for the button window.
 *
 *----------------------------------------------------------------------
 */

void
Sx_ButtonMake(window, text, fontPtr, foreground, background, proc, clientData)
    Window window;			/* Window that is to become a button
					 * window.  If this isn't already a
					 * button window, then it shouldn't
					 * be mapped.  If it's already a button
					 * window, its parameters will be
					 * changed and the window will be
					 * redisplayed if it's mapped. */
    char *text;				/* Text to display inside button.  May
					 * be NULL. */
    FontInfo *fontPtr;			/* Font in which to display text.  If
					 * NULL, a default font is used. */
    int foreground;			/* Color in which to display text. */
    int background;			/* Color for background of button. */
    void (*proc)();			/* Procedure to call when mouse button
					 * is pressed. */
    ClientData clientData;		/* Arbitrary value to pass to proc. */
{
    register Button *buttonPtr;

    if (!init) {
	ButtonInit();
    }

    if (fontPtr == NULL) {
	fontPtr = Sx_GetDefaultFont();
    }

    /*
     * See if this window is already a button window.  If so, then
     * prepare it for re-allocation.  If not, then make a new Button
     * structure.
     */
    
    buttonPtr = (Button *) XLookUpAssoc(buttonTable, window);
    if (buttonPtr != NULL) {
	if (buttonPtr->text != NULL) {
	    Mem_Free(buttonPtr->text);
	}
    } else {
	buttonPtr = (Button *) Mem_Alloc(sizeof(Button));
	buttonPtr->w = window;
	buttonPtr->width = buttonPtr->height = 0;
	buttonPtr->active = FALSE;
	(void) Sx_HandlerCreate(window, ExposeWindow|ButtonPressed
		|ButtonReleased|EnterWindow|LeaveWindow,
		ButtonEventProc, (ClientData) buttonPtr);
	(void) Sx_HandlerCreate(window, SX_DESTROYED, ButtonDestroyProc,
		(ClientData) buttonPtr);
	(void) Sx_HandlerCreate(window, KeyPressed|KeyReleased,
		Sx_NullProc, (ClientData) NULL);
	XDefineCursor(window, cursorNoButton);
	XMakeAssoc(buttonTable, window, (caddr_t) buttonPtr);
    }

    /*
     * Initialize the contents of the button.
     */

    buttonPtr->fontPtr = fontPtr;
    if (text != NULL) {
	buttonPtr->textSize = String_Length(text);
	buttonPtr->text = Mem_Alloc(buttonPtr->textSize + 1);
	(void) String_Copy(text, buttonPtr->text);
	buttonPtr->textWidth = XStringWidth(text, fontPtr, 0, 0);
    } else {
	buttonPtr->text = NULL;
	buttonPtr->textSize = buttonPtr->textWidth = 0;
    }
    buttonPtr->foreground = foreground;
    buttonPtr->background = background;
    buttonPtr->proc = proc;
    buttonPtr->clientData = clientData;

    if ((buttonPtr->width > 0) && (buttonPtr->height > 0)) {
	ButtonRedisplay(buttonPtr);
    }
}

/*
 *----------------------------------------------------------------------
 *
 * Sx_ButtonCreate --
 *
 *	Like Sx_ButtonMake, but creates a new window for the button
 *	rather than using an existing window.
 *
 * Results:
 *	The return value is the X id for a new window, which has
 *	location and size given by the parameters, and behaves
 *	just like a button window made by calling Sx_ButtonMake.
 *	The new window is mapped.
 *
 * Side effects:
 *	The new window will behave like a button window.  See
 *	Sx_ButtonMake's documentation for details.
 *
 *----------------------------------------------------------------------
 */

Window
Sx_ButtonCreate(parent, x, y, width, height, border, text, fontPtr,
	foreground, background, proc, clientData)
    Window parent;			/* Window in which new button window
					 * is to be created. */
    int x, y;				/* Location of upper-left corner of
					 * new window's border, in coords of
					 * parent. */
    int width, height;			/* Inside dimensions of new window.  If
					 * either is zero, then the dimension
					 * is chosen to be just large enough to
					 * contain the text. */
    int border;				/* Border width for new window. */
    char *text;				/* Text to display inside button.  May
					 * be NULL, in which case nothing
					 * appears inside the button. */
    FontInfo *fontPtr;			/* Font in which to display text.  If
					 * NULL, a default font is used. */
    int foreground;			/* Color in which to display text and
					 * border. */
    int background;			/* Color for background of button. */
    void (*proc)();			/* Procedure to call when mouse button
					 * is pressed. */
    ClientData clientData;		/* Arbitrary value to pass to proc. */
{
    Window w;
    Pixmap borderPixmap;

    if (fontPtr == NULL) {
	fontPtr = Sx_GetDefaultFont();
    }
    if (height <= 0) {
	height = Sx_DefaultHeight(fontPtr);
    }
    if (width <= 0) {
	width = XStringWidth(text, fontPtr, 0, 0)
		+ XStringWidth("  ", fontPtr, 0, 0);
    }
    if ((foreground == BlackPixel) || (border == 0)) {
	borderPixmap = BlackPixmap;
    } else if (foreground == WhitePixel) {
	borderPixmap = WhitePixmap;
    } else {
	borderPixmap = XMakeTile(foreground);
	if (borderPixmap == NULL) {
	    Sx_Panic("Sx_ButtonCreate:  couldn't create border pixmap.");
	}
    }
    w = XCreateWindow(parent, x, y, width, height, border, borderPixmap,
	    WhitePixmap);
    if (w == NULL) {
	Sx_Panic("Sx_ButtonCreate:  couldn't create new window.");
    }
    if ((borderPixmap != BlackPixmap) && (borderPixmap != WhitePixmap)) {
	XFreePixmap(borderPixmap);
    }
    Sx_ButtonMake(w, text, fontPtr, foreground, background, proc, clientData);
    XMapWindow(w);
    return w;
}

/*
 *----------------------------------------------------------------------
 *
 * ButtonDestroyProc --
 *
 *	Called by the Sx dispatcher whenever a button window is
 *	destroyed.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	The additional data structures associated with the button
 *	are deleted.
 *
 *----------------------------------------------------------------------
 */

void
ButtonDestroyProc(buttonPtr)
    register Button *buttonPtr;		/* Internal info about button. */
{
    XDeleteAssoc(buttonTable, buttonPtr->w);
    if (buttonPtr->text != NULL) {
	Mem_Free(buttonPtr->text);
    }
    Mem_Free((char *) buttonPtr);
}

/*
 *----------------------------------------------------------------------
 *
 * ButtonInit --
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
ButtonInit()
{
    cursorNoButton = XCreateCursor(dot_width, dot_height, dot_bits,
	    dotMask_bits, dot_x_hot, dot_y_hot,
	    BlackPixel, WhitePixel, GXcopy);
    cursorButton = XCreateCursor(star_width, star_height,
	    star_bits, starMask_bits, star_x_hot, star_y_hot,
	    BlackPixel, WhitePixel, GXcopy);
    buttonTable = XCreateAssocTable(8);

    if ((cursorNoButton == 0) || (cursorButton == 0)
	    || (buttonTable == NULL)) {
	Sx_Panic("Sx_ButtonMake: couldn't initialize cursors and/or hash table.");
    }
    init = TRUE;
}

/*
 *----------------------------------------------------------------------
 *
 * ButtonRedisplay --
 *
 *	Redraw the information in a button window.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	The button gets redrawn.
 *
 *----------------------------------------------------------------------
 */

static void
ButtonRedisplay(buttonPtr)
    register Button *buttonPtr;	/* Button to redisplay. */
{
    int fg, bg, x, y;

    if (buttonPtr->active) {
	fg = buttonPtr->background;
	bg = buttonPtr->foreground;
    } else {
	fg = buttonPtr->foreground;
	bg = buttonPtr->background;
    }
    XPixSet(buttonPtr->w, 0, 0, buttonPtr->width, buttonPtr->height, bg);
    x = (buttonPtr->width - buttonPtr->textWidth)/2;
    y = (buttonPtr->height - buttonPtr->fontPtr->height)/2;
    if (buttonPtr->text != NULL) {
	XText(buttonPtr->w, x, y, buttonPtr->text, buttonPtr->textSize,
		buttonPtr->fontPtr->id, fg, bg);
    }
}

/*
 *----------------------------------------------------------------------
 *
 * ButtonEventProc --
 *
 *	This procedure is invoked automatically by the Sx_ dispatcher
 * 	whenever an event occurs for a button window.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	This procedure performs most of the button-related side-effects,
 *	such as displaying the button in reverse video and calling the
 *	client's procedure when a mouse button is pressed.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
static void
ButtonEventProc(buttonPtr, eventPtr)
    register Button *buttonPtr;		/* Button for which it happened. */
    XExposeEvent *eventPtr;		/* Describes what happened. */
{
    int i;

    if (eventPtr->subwindow != NULL) {
	return;
    }

    switch ((int) eventPtr->type) {

	case EnterWindow:
	    if ((eventPtr->detail & 0xff) != VirtualCrossing) {
		buttonPtr->active = TRUE;
		ButtonRedisplay(buttonPtr);
	    }
	    break;

	case LeaveWindow:
	    if ((eventPtr->detail & 0xff) != VirtualCrossing) {
		buttonPtr->active = FALSE;
		ButtonRedisplay(buttonPtr);
	    }
	    break;

	case ExposeWindow:
	    buttonPtr->width = eventPtr->width;
	    buttonPtr->height = eventPtr->height;
	    ButtonRedisplay(buttonPtr);
	    break;

	case ButtonPressed:
	    XDefineCursor(buttonPtr->w, cursorButton);

	    /*
	     * Flash the button before calling its action procedure.
	     */

	    for (i = 0; i < 2; i++) {
		buttonPtr->active = FALSE;
		ButtonRedisplay(buttonPtr);
		SxFlashWait();
		buttonPtr->active = TRUE;
		ButtonRedisplay(buttonPtr);
		SxFlashWait();
	    }
	    buttonPtr->proc(buttonPtr->clientData, eventPtr->detail & 0xff,
		    buttonPtr->w);

	    /*
	     * It's important that we don't do anything with the button
	     * after the procedure returns:  the procedure could have
	     * deleted the button, leaving us with a dangling pointer.
	     */

	    break;
	
	case ButtonReleased:
	    XDefineCursor(buttonPtr->w, cursorNoButton);
	    break;

	default:
	    break;
    }
}
