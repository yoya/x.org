/*
 *	$Source: /u1/Sx.new/code/RCS/sxDemo.c,v $
 *	$Header: sxDemo.c,v 1.1 86/12/03 16:09:39 swick Exp $
 */

#ifndef lint
static char *rcsid_sxDemo_c = "$Header: sxDemo.c,v 1.1 86/12/03 16:09:39 swick Exp $";
#endif	lint

/* 
 * sxDemo.c --
 *
 *	This is a sample window application that demonstrates many
 *	of the features of the Sx package.  Unfortunately, it's so
 *	simple that it doesn't make very good use of the "clientData"
 *	arguments.
 *
 * Copyright (C) 1986 Regents of the University of California.
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appear in all copies.  The University of California
 * makes no representations about the suitability of this software
 * for any purpose.  It is provided "as is" without express or
 * implied warranty.
 */

#ifndef lint
static char rcsid[] = "$Header: sxDemo.c,v 1.1 86/12/03 16:09:39 swick Exp $ SPRITE (Berkeley)";
#endif not lint


#include <stdio.h>
#include <X/Xlib.h>
#undef NULL
#include "sx.h"

/*
 * Forward declarations of callback procedures.
 */

void MakeButtonProc(), ChangeTextProc(), DeleteButtonProc();
void NewButtonMenuProc(), DeleteButtonMenuProc(), EnableMaskProc();
void Notifier1Proc(), Notifier2Proc();
void QuitProc();
void ScrollProc();
void RedisplayProc();
void ButtonProc();

/*
 * Pull-down menu declarations:
 */

Sx_MenuEntry buttons[] = {
    {NULL, "Make Button", NULL, -1, MakeButtonProc, (ClientData) NULL},
    {NULL, "Change Text", NULL, -1, ChangeTextProc, (ClientData) NULL},
    {NULL, "Delete Button", NULL, -1, DeleteButtonProc, (ClientData) NULL}
};

Sx_MenuEntry menus[] = {
    {"Make new button menu", NULL, NULL, -1, NewButtonMenuProc,
	(ClientData) NULL},
    {"Delete newest button menu", NULL, NULL, -1, DeleteButtonMenuProc,
	(ClientData) NULL},
    {"Disable this menu entry", NULL, NULL, -1, EnableMaskProc,
	(ClientData) ~0x4},
    {"Enable previous menu entry", NULL, NULL, -1, EnableMaskProc,
	(ClientData) ~0}
};

Sx_MenuEntry notifiers[] = {
    {"Test Notifier 1", NULL, NULL, -1, Notifier1Proc,
	(ClientData) NULL},
    {"Test Notifier 2", NULL, NULL, -1, Notifier2Proc,
	(ClientData) NULL}
};

Sx_MenuEntry quit[] = {
    {NULL, "Quit", NULL, -1, QuitProc, (ClientData) NULL}
};

/*
 * Windows that make up SxDemo's application.
 */

Window w;			/* Top-level window. */
Window title, vertScrollbar, horizScrollbar;
Window menuBar, buttonsMenu, menusMenu, notifiersMenu, quitMenu;
Window entry;
Window buttonsMenu2 = NULL;
Window button = NULL;

/*
 * Font and color information:
 */

static FontInfo *fontPtr;
static int foreground, background;
static Pixmap fgPixmap, bgPixmap;

main(argc, argv)
    int argc;
    char **argv;
{
    int height;
    char entryText[100], *defString;
    Color hard, exact;

    /*
     * Open a connection to the display.
     */

    if (XOpenDisplay((char *) NULL) == NULL) {
	printf("Couldn't open display.  Are you sure X is running?\n");
	return;
    }

    /*
     * Read defaults, and set up color and font information.
     */
    
    foreground = BlackPixel;
    fgPixmap = BlackPixmap;
    background = WhitePixel;
    bgPixmap = WhitePixmap;
    if (DisplayCells() > 2) {
	defString = XGetDefault(argv[0], "Foreground");
	if ((defString != NULL) && (XGetColor(defString, &hard, &exact) != 0)
		&& (XGetHardwareColor(&hard) != 0)) {
	    foreground = hard.pixel;
	    fgPixmap = XMakeTile(foreground);
	}
	defString = XGetDefault(argv[0], "Background");
	if ((defString != NULL) && (XGetColor(defString, &hard, &exact) != 0)
		&& (XGetHardwareColor(&hard) != 0)) {
	    background = hard.pixel;
	    bgPixmap = XMakeTile(background);
	}
    }
    defString = XGetDefault(argv[0], "BodyFont");
    if (defString != NULL) {
	fontPtr = XOpenFont(defString);
    }
    if (fontPtr == NULL) {
	fontPtr = XOpenFont("8x13");
	if (fontPtr == NULL) {
	    Sx_Panic("SxDemo:  couldn't find a font to use.");
	}
    }

    /*
     * Create the window and its subwindows.
     */
    
    w = XCreateWindow(RootWindow, 100, 100, 450, 300, 2,
	    fgPixmap, bgPixmap);
    (void) Sx_HandlerCreate(w, ExposeWindow, RedisplayProc, (ClientData) NULL);

    height = Sx_DefaultHeight(fontPtr);
    title = Sx_TitleCreate(w, SX_TOP, height, 0, fontPtr, background,
	    foreground, (char *) NULL, "Sx Demonstrator Program",
	    (char *) NULL);

    menuBar = Sx_CreatePacked(w, SX_TOP, height, 1, (Window) NULL,
	    (Window) NULL, fgPixmap, bgPixmap);
    menusMenu = Sx_MenuCreate(menuBar, "Menus", 4, menus, fontPtr,
	    foreground, background);
    buttonsMenu = Sx_MenuCreate(menuBar, "Buttons", 3, buttons, fontPtr,
	    foreground, background);
    notifiersMenu = Sx_MenuCreate(menuBar, "Notifiers", 2, notifiers, fontPtr,
	    foreground, background);
    quitMenu = Sx_MenuCreate(menuBar, "Quit", 1, quit, fontPtr,
	    foreground, background);
    
    horizScrollbar = Sx_ScrollbarCreate(w, SX_BOTTOM, foreground,
	    background, background, ScrollProc, (ClientData) NULL);
    Sx_ScrollbarSetRange(horizScrollbar, 0.0, .2);
    vertScrollbar = Sx_ScrollbarCreate(w, SX_RIGHT,foreground,
	    background, background, ScrollProc, (ClientData) NULL);
    Sx_ScrollbarSetRange(vertScrollbar, 0.0, .2);

    entry = Sx_CreatePacked(w, SX_BOTTOM, height, 1, (Window) NULL,
	    (Window) NULL, fgPixmap, bgPixmap);
    Sx_EntryMake(entry, "You can type text here: ", fontPtr, foreground,
	    background, entryText, 100);
    
    XMapWindow(w);
    while (1) {
	XEvent event;

	XNextEvent(&event);
	Sx_HandleEvent(&event);
    }
}

/*
 * Callback procedure invoked whenever a mouse button is pressed over
 * the button subwindow:
 */

void
ButtonProc(clientData, whichButton, window)
    ClientData clientData;	/* Not used. */
    int whichButton;		/* Tells which button was clicked. */
    Window window;		/* Not used. */
{
    static char *name[] = {"right", "middle", "left"};

    printf("You touched me with the %s mouse button.\n", name[whichButton]);
}

/*
 * Callback procedure invoked whenever the "Make Button" menu entry is invoked.
 */

void
MakeButtonProc(clientData, index, window)
    ClientData clientData;	/* Not used. */
    int index;			/* Index of menu entry;  not used. */
    Window window;		/* Not used. */
{
    if (button != NULL) {
	printf("Can't create the button:  it exists already.\n");
	return;
    }
    button = Sx_ButtonCreate(w, 100, 200, 0, 0, 1, "Click on me",
	    fontPtr, foreground, background, ButtonProc, (ClientData) NULL);
}

/*
 * Callback procedure invoked whenever the "Change Text" menu entry is invoked.
 */

void
ChangeTextProc(clientData, index, window)
    ClientData clientData;	/* Not used. */
    int index;			/* Not used. */
    Window window;		/* Not used. */
{
    if (button == NULL) {
	printf("Create the button before changing its text.\n");
	return;
    }
    Sx_ButtonMake(button, "New text", fontPtr, foreground, background,
	    ButtonProc, (ClientData) NULL);
}

/*
 * Callback procedure invoked whenever the "Delete Button" menu entry
 * is invoked.
 */

void
DeleteButtonProc(clientData, index, window)
    ClientData clientData;	/* Not used. */
    int index;			/* Not used. */
    Window window;		/* Not used. */
{
    if (button == NULL) {
	printf("Can't delete the button:  it doesn't exist.\n");
	return;
    }
    XDestroyWindow(button);
    button = NULL;
}

/*
 * Callback procedure invoked whenever the "Make new button menu" menu entry
 * is invoked.
 */

void
NewButtonMenuProc(clientData, index, window)
    ClientData clientData;	/* Not used. */
    int index;			/* Not used. */
    Window window;		/* Not used. */
{
    if (buttonsMenu2 != NULL) {
	printf("You've already got a second buttons menu.  Delete it\n");
	printf("before making a new one.\n");
	return;
    }
    buttonsMenu2 = Sx_MenuCreate(menuBar, "2nd Buttons", 3, buttons,
	   fontPtr, foreground, background);
}

/*
 * Callback procedure invoked whenever the "Delete newest button menu"
 * menu entry is invoked.
 */

void
DeleteButtonMenuProc(clientData, index, window)
    ClientData clientData;	/* Not used. */
    int index;			/* Not used. */
    Window window;		/* Not used. */
{
    if (buttonsMenu2 == NULL) {
	printf("Create a new buttons menu before trying to delete.\n");
	return;
    }
    XDestroyWindow(buttonsMenu2);
    buttonsMenu2 = NULL;
}

/*
 * Callback procedure invoked by several different menu entries to
 * enable or disable menu entries.
 */

void
EnableMaskProc(clientData, index, window)
    ClientData clientData;	/* New mask. */
    int index;			/* Not used. */
    Window window;		/* Not used. */
{
    int mask;

    /*
     * The clientData value here is actually the new mask for the
     * "Menus" menu.  By passing different clientData values in,
     * the same procedure can handle several different menu entries.
     */
    
    mask = (int) clientData;
    (void) Sx_MenuSetMask(menusMenu, mask);
}

/*
 * Callback procedure invoked whenever the "Test Notifier 1"
 * menu entry is invoked.
 */

void
Notifier1Proc(clientData, index, window)
    ClientData clientData;	/* Not used. */
    int index;			/* Not used. */
    Window window;		/* Not used. */
{
    static char *message = "This notifier is centered in the root window.  \
Notice that it grabs the server, so you can't do anything else until \
you select an option.  Notice also how the Sx package automatically \
chopped one long string up into left-justified lines for display in the \
window.";

    (void) Sx_Notify(RootWindow, -1, -1, 0, message, fontPtr,
	    FALSE, "OK", NULL);
}

/*
 * Callback procedure invoked whenever the "Test Notifier 2"
 * menu entry is invoked.
 */

void
Notifier2Proc(clientData, index, window)
    ClientData clientData;	/* Not used. */
    int index;			/* Not used. */
    Window window;		/* Not used. */
{
    static char *message = "This notifier was centered in the sxDemo window, \
but still grabs the server.\n\n\It uses center justification \
for the text.";
    int result;

    result = Sx_Notify(w, -1, -1, 0, message, fontPtr,
	    TRUE, "She loves me", "She loves me not", NULL);
    if (result == 0) {
	printf("You selected the \"she loves me\" option.\n");
    } else {
	printf("You selected the \"she loves me not\" option.\n");
    }
}

/*
 * Callback procedure invoked whenever the "Quit"
 * menu entry is invoked.
 */

void
QuitProc(clientData, index, window)
    ClientData clientData;	/* Not used. */
    int index;			/* Not used. */
    Window window;		/* Not used. */
{
    exit(0);
}

/*
 * Callback procedure invoked whenever the user requests scrolling by
 * clicking over one of the scrollbars.
 */

void
ScrollProc(clientData, distance, units, window)
    ClientData clientData;	/* Not used. */
    float distance;		/* Amount of motion. */
    int units;			/* SX_SCROLL_ABSOLUTE or SX_SCROLL_PAGES. */
    Window window;		/* Scrollbar window. */
{
    char *direction;

    if (window == vertScrollbar) {
	direction = "vertical";
    } else {
	direction = "horizontal";
    }

    printf("You just requested %s scrolling", direction);
    if (units == SX_SCROLL_ABSOLUTE) {
	printf(" to center position %.2f in the view.\n", distance);
	distance -= .1;
    } else {
	float top, bottom;
	printf(" by %.2f pages.\n", distance);
	Sx_ScrollbarGetRange(window, &top, &bottom);
	distance = (distance * .2) + top;
    }
    printf("I can't actually scroll the window, but I will ");
    printf("update the scrollbar display.\n");
    if (distance < 0) {
	distance = 0.0;
    }
    if (distance > .8) {
	distance = .8;
    }
    Sx_ScrollbarSetRange(window, distance, distance + .2);
}

/*
 * Callback procedure that is invoked whenever the window needs to
 * be redrawn.  It just displays some help text.
 */

void
RedisplayProc(clientData, eventPtr)
    ClientData clientData;	/* Not used. */
    XEvent *eventPtr;		/* ExposeWindow event;  not used. */
{
    static char *(msgs[]) = {
	"To try out the Sx features:",
	"(a) press a mouse button over a menu name (above)",
	"    and select a menu entry, or",
	"(b) click a button over a scrollbar, or",
	"(c) resize the window (the packer will re-size",
	"    subwindows).",
	"Information messages will appear on standard",
	"output (not in this window).",
	NULL
    };
    char **stringPtr;
    int y;

    for (y = 50, stringPtr = msgs; *stringPtr != NULL;
	    stringPtr++, y += fontPtr->height + 1) {
	XText(w, 5, y, *stringPtr, strlen(*stringPtr), fontPtr->id,
		foreground, background);
    }
}
