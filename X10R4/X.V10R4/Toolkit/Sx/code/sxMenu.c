/*
 *	$Source: /u1/Sx.new/code/RCS/sxMenu.c,v $
 *	$Header: sxMenu.c,v 1.1 86/12/03 16:10:13 swick Exp $
 */

#ifndef lint
static char *rcsid_sxMenu_c = "$Header: sxMenu.c,v 1.1 86/12/03 16:10:13 swick Exp $";
#endif	lint

/* 
 * sxMenu.c --
 *
 *	This file implements pull-down menus using the facilities of the
 *	X window package.
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
static char rcsid[] = "$Header: sxMenu.c,v 1.1 86/12/03 16:10:13 swick Exp $ SPRITE (Berkeley)";
#endif not lint

#include <sys/types.h>
#include <X/Xlib.h>
#include "sprite.h"
#include "mem.h"
#include "string.h"
#include "sx.h"
#include "sxInt.h"

/*
 * The structure below is used internally to this file to describe
 * a menu entry.
 */

typedef struct {
    char *leftText;		/* Text to display left-justified in
				 * menu entry, with an extra space character
				 * added at each end.  NULL means no text. */
    int leftSize;		/* # of characters in leftText. */
    char *centerText;		/* Text to display in center of menu
				 * entry (space-padded as for leftText).
				 * NULL means no centered text. */
    int centerSize;		/* Number of characters in centerText. */
    int centerPos;		/* X position at which to draw text in menu. */
    char *rightText;		/* Text to display right-justified in
				 * menu entry (space-padded as for leftText).
				 * NULL means no text. */
    int rightSize;		/* Number of characters in rightText. */
    int rightPos;		/* X position at which to draw text in menu. */
    int background;		/* Background color for menu entry. */
    void (*proc)();		/* Procedure to call when menu entry
				 * is selected. */
    ClientData clientData;	/* Client-supplied information to pass
				 * to proc. */
} MenuEntry;

/*
 * The structure below defines a pull-down menu.  It consists of a
 * number of menu entries plus a mask indicating which entries are
 * enabled:  disabled entries are displayed but do not respond to
 * button presses.  Two windows are used to display the menu:  one
 * for the menu title, and one for the menu itself (which is a child
 * of the root window and is only displayed when the menu is active).
 */

typedef struct Menu {
    FontInfo *fontPtr;			/* Font to be used for this menu. */
    int foreground;			/* Foreground color for title and
					 * menu. */
    int background;			/* Background color for title. */
    char *name;				/* Name of menu (displayed in
					 * titleWindow). */
    int nameWidth;			/* Size of menu name, in pixels. */
    Window titleWindow;			/* Window that is used to display menu
					 * name, and which user clicks on to
					 * pull menu down. */
    Window menuWindow;			/* Window used to display menu
					 * entries. */
    struct MenuGroup *groupPtr;		/* Structure describing group of
					 * pulldown menus, all created in
					 * the same containing window. */
    int numEntries;			/* Number of entries in menu. */
    MenuEntry *entryPtrs[SX_MAX_MENU_ENTRIES];
					/* Array of entries for this menu.
					 * Entry 0 is displayed at the top.
					 * NULL means entry doesn't exist.
					 * Only first numEntries are valid. */
    int mask;				/* Indicates which entries are enabled.
					 * 0 means disabled, 1 means enabled.
					 * The lowest-order bit corresponds to
					 * menu entry 0. */
    int titleWidth, titleHeight;	/* Dimensions of titleWindow. */
    int menuWidth;			/* Width of menuWindow in pixels,
					 * including border and shadow. */
    int menuHeight;			/* Height of menuWindow in pixels,
					 * including border and shadow. */
} Menu;

/*
 * The structure below defines a menu group, which consists of a bunch
 * of pulldown menus all lined up in a row in some containing window.
 */

typedef struct MenuGroup {
    Window w;				/* X window that contains the title
					 * windows for all the menus in
					 * this group. */
    Menu *menuPtrs[SX_MAX_MENUS];	/* Array of menus.  NULL means menu
					 * not defined.  All defined menus
					 * are together at beginning of
					 * array. */
} MenuGroup;

/*
 * Constants for displaying menus:
 *
 * MARGIN		Vertical spacing to leave around entries in menus.
 */

#define MARGIN 1

/*
 * Hash table used to find group and menu information from window ids.
 */

static XAssocTable *menuTable;
static XAssocTable *groupTable;
extern XAssocTable *XCreateAssocTable();

/*
 * The include's below are used to define the cursors used for
 * menu titles and menu windows.
 */

#include "cursors/dot"
#include "cursors/dotMask"
#include "cursors/star"
#include "cursors/starMask"

static Cursor cursorDot, cursorStar;

/*
 * If a menu is actually pulled down, the information below is used
 * to keep track of information about it.
 */

static Pixmap savedPixmap;		/* Used to save pixels underneath menu
					 * so they don't have to be redrawn. 0
					 * means nothing is saved. */
static int savedWidth, savedHeight;	/* Dimensions of savedPixmap. */
static int menuX, menuY;		/* Location of origin of pulled-down
					 * menu, in coordinates of RootWindow.
					 */
static int groupX, groupY;		/* Location of UL corner of leftmost
					 * menu in group of pulled-down menu,
					 * in coordinates of RootWindow.  Used
					 * to determine when the pulled-down
					 * menu should be changed.  */
static Menu *selectedMenuPtr = NULL;	/* Selected menu (the one that's
					 * pulled down or that the mouse is
					 * over), or NULL if none selected. */
static int selectedEntry = -1;		/* Index of selected entry, or -1 if
					 * no entry is selected. */

static Boolean init = FALSE;		/* TRUE means initialized OK. */
static int displayWidth;		/* Dimensions of display. */
static int displayHeight;

/*
 * Forward references:
 */

extern void	ComputeMenuLayout();
extern void	EraseMenu();
extern void	MenuDeleteEntries();
extern void	MenuDestroyProc();
extern void	MenuEntryDisplay();
extern void	MenuExposeProc();
extern void	MenuInit();
extern void	MenuMouseProc();
extern void	MenuRedisplayProc();
extern void	MenuTitleAdjust();
extern void	MenuTitleMouseProc();
extern void	MenuTitleRedisplay();
extern void	PadAndCopy();
extern void	PullMenuDown();

/*
 *----------------------------------------------------------------------
 *
 * Sx_MenuCreate --
 *
 *	Create a new pulldown menu.
 *
 * Results:
 *	The return value is an X id for the window containing the
 *	pulldown menu's title.  This can be used to manipulate the
 *	menu (e.g. call XDestroyWindow to delete the menu).
 *
 * Side effects:
 *	A new pulldown menu is created and displayed.  It will have
 *	numEntries entries, as described by the entries in the entries[]
 *	array.  If there was already a menu by the given name in the
 *	given parent window, then the existing menu is replaced.
 *
 *	Later, when buttons are pressed over the entries in the pulldown
 *	menu, the client procedures indicated in entries[] will be called
 *	as follows:
 *
 *	void
 *	proc(clientData, entry, menuWindow)
 *	    ClientData clientData;
 *	    int entry;
 *	    Window menuWindow;
 *	{
 *	}
 *	
 *	The clientData parameter is the one passed in the entries[]
 *	element that was buttoned, entry is the index of the entry
 *	that was invoked, and menuWindow indicates the menu (it's
 *	the same as the value returned by this procedure).
 *
 *----------------------------------------------------------------------
 */

Window
Sx_MenuCreate(parent, name, numEntries, entries, fontPtr,
	foreground, background)
    Window parent;			/* Containing window in which to
					 * create menu.  If many menus are
					 * created in this window, their
					 * titles will be ordered from left
					 * to right in order of creation. */
    char *name;				/* Name of menu. */
    int numEntries;			/* Number of entries in menu.  Must be
					 * less than or equal to
					 * SX_MAX_MENU_ENTRIES. */
    Sx_MenuEntry entries[];		/* Entries:  must have numEntries
					 * values. */
    FontInfo *fontPtr;			/* Font to use for displaying menu.
					 * NULL means use default font. */
    int foreground;			/* Foreground color to use to display
					 * menu title and menu entries. */
    int background;			/* Background color to use for title.
					 * Each menu entry indicates its own
					 * background color. */
{
    register MenuGroup *groupPtr;
    register Menu *menuPtr;
    register MenuEntry *entryPtr;
    register Sx_MenuEntry *paramPtr;
    int i, index;

    if (!init) {
	MenuInit();
    }

    if (fontPtr == NULL) {
	fontPtr = Sx_GetDefaultFont();
    }

    /*
     * See if there's already a MenuGroup set up for the parent window.
     * If not then create one.
     */
    
    groupPtr = (MenuGroup *) XLookUpAssoc(groupTable, parent);
    if (groupPtr == NULL) {
	groupPtr = (MenuGroup *) Mem_Alloc(sizeof(MenuGroup));
	groupPtr->w = parent;
	for (i = 0; i < SX_MAX_MENUS; i++) {
	    groupPtr->menuPtrs[i] = NULL;
	}
	XMakeAssoc(groupTable, parent, (caddr_t) groupPtr);
    }

    /*
     * Figure out which entry to use (or replace).
     */

    for (index = 0; index < SX_MAX_MENUS; index++) {
	menuPtr = groupPtr->menuPtrs[index];
	if (menuPtr == NULL) {
	    break;
	}
	if (String_Compare(menuPtr->name, name) == 0) {
	    break;
	}
    }
    if (index >= SX_MAX_MENUS) {
	Sx_Panic("Sx_MenuCreate: tried to create too many menus.");
    }

    /*
     * If replacing, clean up old stuff that will be redone.  If
     * creating from scratch, do once-only initialization.
     */
    
    if (menuPtr != NULL) {
	MenuDeleteEntries(menuPtr);
    } else {
	menuPtr = (Menu *) Mem_Alloc(sizeof(Menu));
	groupPtr->menuPtrs[index] = menuPtr;
	menuPtr->name = (char *) Mem_Alloc(String_Length(name) + 1);
	String_Copy(name, menuPtr->name);
	menuPtr->nameWidth = XStringWidth(name, fontPtr, 0, 0);
	menuPtr->titleWindow = XCreateWindow(groupPtr->w, 0, 0,
		1, 1, 0, BlackPixmap, WhitePixmap);
	(void) Sx_HandlerCreate(menuPtr->titleWindow,
		ButtonPressed|ButtonReleased|EnterWindow|LeaveWindow,
		MenuTitleMouseProc, (ClientData) menuPtr);
	(void) Sx_HandlerCreate(menuPtr->titleWindow, ExposeWindow,
		MenuExposeProc, (ClientData) menuPtr);
	(void) Sx_HandlerCreate(menuPtr->titleWindow, SX_DESTROYED,
		MenuDestroyProc, (ClientData) menuPtr);
	(void) Sx_HandlerCreate(menuPtr->titleWindow, KeyPressed|KeyReleased,
		Sx_NullProc, (ClientData) NULL);
	Sx_Pack(menuPtr->titleWindow, parent, SX_LEFT,
		menuPtr->nameWidth + XStringWidth("  ", fontPtr, 0, 0),
		0, (Window) 0, (Window) 0);
	XMakeAssoc(menuTable, menuPtr->titleWindow, (caddr_t) menuPtr);
	menuPtr->menuWindow = XCreateWindow(RootWindow, 0, 0, 1, 1,
		0, BlackPixmap, WhitePixmap);
	(void) Sx_HandlerCreate(menuPtr->menuWindow, ExposeWindow,
		MenuRedisplayProc, (ClientData) menuPtr);
	(void) Sx_HandlerCreate(menuPtr->menuWindow, ButtonReleased|MouseMoved,
		MenuMouseProc, (ClientData) menuPtr);
	menuPtr->groupPtr = groupPtr;
    }

    /*
     * Do the initialization that's independent of whether this is a
     * replacement or creation.
     */
    
    menuPtr->fontPtr = fontPtr;
    menuPtr->foreground = foreground;
    menuPtr->background = background;
    if (numEntries > SX_MAX_MENU_ENTRIES) {
	numEntries = SX_MAX_MENU_ENTRIES;
    }
    menuPtr->numEntries = numEntries;
    for (i = 0, paramPtr = entries; i < numEntries; i++, paramPtr++) {
	entryPtr = (MenuEntry *) Mem_Alloc(sizeof(MenuEntry));
	menuPtr->entryPtrs[i] = entryPtr;
	if (paramPtr->leftText == NULL) {
	    entryPtr->leftText = NULL;
	    entryPtr->leftSize = 0;
	} else {
	    PadAndCopy(paramPtr->leftText, &entryPtr->leftText,
		    &entryPtr->leftSize);
	}
	if (paramPtr->centerText == NULL) {
	    entryPtr->centerText = NULL;
	    entryPtr->centerSize = 0;
	} else {
	    PadAndCopy(paramPtr->centerText, &entryPtr->centerText,
		    &entryPtr->centerSize);
	}
	if (paramPtr->rightText == NULL) {
	    entryPtr->rightText = NULL;
	    entryPtr->rightSize = 0;
	} else {
	    PadAndCopy(paramPtr->rightText, &entryPtr->rightText,
		    &entryPtr->rightSize);
	}
	if (paramPtr->background != -1) {
	    entryPtr->background = paramPtr->background;
	} else {
	    entryPtr->background = menuPtr->background;
	}
	entryPtr->proc = paramPtr->proc;
	entryPtr->clientData = paramPtr->clientData;
    }
    menuPtr->mask = (1<<numEntries) - 1;
    menuPtr->menuHeight = numEntries*(menuPtr->fontPtr->height + 2*MARGIN)
	    + SHADOW_TOP + SHADOW_BOTTOM;
    ComputeMenuLayout(menuPtr);

    return menuPtr->titleWindow;
}

/*
 *----------------------------------------------------------------------
 *
 * Sx_MenuSetMask --
 *
 *	Select which entries in a menu are enabled.
 *
 * Results:
 *	The return value is the previous value of the enabled mask
 *	for the menu.
 *
 * Side effects:
 *	After this call, only the entries given by ones in mask will be
 *	enabled in the menu given by window.  The low-order bit of mask
 *	corresponds to the first entry in the menu, etc.  Disabled menu
 *	entries are displayed differently, and are not sensitive to
 *	button pushes (i.e. the client procedure isn't invoked).  If
 *	every entry in a menu is disabled, then the menu title is
 *	displayed differently to indicate this fact.
 *
 *----------------------------------------------------------------------
 */

int
Sx_MenuSetMask(window, mask)
    Window window;		/* Window containing menu title (i.e. value
				 * returned by Sx_MenuCreate). */
    int mask;			/* New value for mask.  Low-order bit
				 * corresponds to first menu entry. */
{
    Menu *menuPtr;
    int oldMask;

    if (!init) {
	MenuInit();
    }

    menuPtr = (Menu *) XLookUpAssoc(menuTable, window);
    if (menuPtr == NULL) {
	Sx_Panic("Sx_MenuSetMask:  window parameter isn't a menu.");
    }

    oldMask = menuPtr->mask;
    menuPtr->mask = mask & ((1<<menuPtr->numEntries) - 1);
    if (menuPtr->mask == 0) {
	if (oldMask != 0) {
	    MenuTitleRedisplay(menuPtr);
	}
    } else if (oldMask == 0) {
	MenuTitleRedisplay(menuPtr);
    }
    return oldMask;
}

/*
 *----------------------------------------------------------------------
 *
 * Sx_MenuGetMask --
 *
 *	Find out which entries in a menu are enabled.
 *
 * Results:
 *	The return value is the current value of the enabled mask for
 *	the menu associated with window.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

int
Sx_MenuGetMask(window)
    Window window;		/* Window containing menu's title (i.e. return
				 * value from Sx_MenuCreate). */
{
    Menu *menuPtr;

    if (!init) {
	MenuInit();
    }

    menuPtr = (Menu *) XLookUpAssoc(menuTable, window);
    if (menuPtr == NULL) {
	Sx_Panic("Sx_MenuGetMask:  window parameter isn't a menu.");
    }
    return menuPtr->mask;
}

/*
 *----------------------------------------------------------------------
 *
 * Sx_MenuReplaceEntry --
 *
 *	This procedure replaces a single entry in a pulldown menu.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	The entryIndex'th entry in the menu associated with window
 *	is replaced with newEntry.
 *
 *----------------------------------------------------------------------
 */

void
Sx_MenuReplaceEntry(window, entryIndex, newEntryPtr)
    Window window;			/* Window that's a pulldown menu
					 * title (i.e. window was a return
					 * value from Sx_MenuCreate). */
    int entryIndex;			/* Index of desired entry.  This
					 * entry must already exist. */
    register Sx_MenuEntry *newEntryPtr;	/* Pointer to new menu entry. */
{
    Menu *menuPtr;
    register MenuEntry *entryPtr;

    if (!init) {
	MenuInit();
    }

    menuPtr = (Menu *) XLookUpAssoc(menuTable, window);
    if (menuPtr == NULL) {
	Sx_Panic("Sx_MenuReplaceEntry:  window parameter isn't a menu.");
    }

    entryPtr = menuPtr->entryPtrs[entryIndex];
    if (entryPtr->leftText != NULL) {
	Mem_Free(entryPtr->leftText);
    }
    if (entryPtr->centerText != NULL) {
	Mem_Free(entryPtr->centerText);
    }
    if (entryPtr->rightText != NULL) {
	Mem_Free(entryPtr->rightText);
    }
    if (newEntryPtr->leftText == NULL) {
	entryPtr->leftText = NULL;
	entryPtr->leftSize = 0;
    } else {
	PadAndCopy(newEntryPtr->leftText, &entryPtr->leftText,
		&entryPtr->leftSize);
    }
    if (newEntryPtr->centerText == NULL) {
	entryPtr->centerText = NULL;
	entryPtr->centerSize = 0;
    } else {
	PadAndCopy(newEntryPtr->centerText, &entryPtr->centerText,
		&entryPtr->centerSize);
    }
    if (newEntryPtr->rightText == NULL) {
	entryPtr->rightText = NULL;
	entryPtr->rightSize = 0;
    } else {
	PadAndCopy(newEntryPtr->rightText, &entryPtr->rightText,
		&entryPtr->rightSize);
    }
    if (newEntryPtr->background != -1) {
	entryPtr->background = newEntryPtr->background;
    } else {
	entryPtr->background = menuPtr->background;
    }
    entryPtr->proc = newEntryPtr->proc;
    entryPtr->clientData = newEntryPtr->clientData;
    ComputeMenuLayout(menuPtr);
}

/*
 *----------------------------------------------------------------------
 *
 * Sx_MenuGetInfo --
 *
 *	This procedure returns a complete description of a particular
 *	menu.
 *
 * Results:
 *	The return value is a count of the number of entries in the
 *	menu, or -1 if window isn't a menu window.  Entries,
 *	*fontPtrPtr, *foregroundPtr, and *backgroundPtr all get
 *	filled in with the corresponding information as it was
 *	passed to Sx_MenuCreate or Sx_MenuReplaceEntry.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

int
Sx_MenuGetInfo(window, entries, fontPtrPtr, foregroundPtr, backgroundPtr)
    Window window;			/* Window corresponding to menu. */
    Sx_MenuEntry entries[];		/* Array of menu entries;  gets filled
					 * in by this procedure.  The string
					 * pointers will all refer to statics
					 * in the menu manager;  they will
					 * become invalid whenever the menu
					 * is modified or deleted.  If entries
					 * is NULL then no entry information
					 * is returned. */
    FontInfo **fontPtrPtr;		/* Gets filled in with font info
					 * pointer (if NULL, then ignored). */
    int *foregroundPtr;			/* Gets filled in with menu's
					 * foreground color (if not NULL). */
    int *backgroundPtr;			/* Gets filled in with menu's
					 * background color (if not NULL). */
{
    register MenuEntry *srcPtr;
    register Sx_MenuEntry *dstPtr;
    register Menu *menuPtr;
    int i;

    if (!init) {
	MenuInit();
    }

    menuPtr = (Menu *) XLookUpAssoc(menuTable, window);
    if (menuPtr == NULL) {
	return -1;
    }

    if (fontPtrPtr != NULL) {
	*fontPtrPtr = menuPtr->fontPtr;
    }
    if (foregroundPtr != NULL) {
	*foregroundPtr = menuPtr->foreground;
    }
    if (backgroundPtr != NULL) {
	*backgroundPtr = menuPtr->background;
    }
    if (entries == NULL) {
	return menuPtr->numEntries;
    }

    for (i = 0, dstPtr = entries; i < menuPtr->numEntries; i++, dstPtr++) {
	srcPtr = menuPtr->entryPtrs[i];
	dstPtr->leftText = srcPtr->leftText;
	dstPtr->centerText = srcPtr->centerText;
	dstPtr->rightText = srcPtr->rightText;
	dstPtr->background = srcPtr->background;
	dstPtr->proc = srcPtr->proc;
	dstPtr->clientData = srcPtr->clientData;
    }
    return menuPtr->numEntries;
}

/*
 *----------------------------------------------------------------------
 *
 * Sx_MenuGetNames --
 *
 *	Given a parent window containing 0 or more menu children, return
 *	the names of all the children menus (and also their X window ids).
 *
 * Results:
 *	The return value is a count of the number of menus in parent,
 *	which may be zero.  The arrays "names" and "windows" get filled
 *	in with information about the menus in parent.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

int
Sx_MenuGetNames(parent, names, windows)
    Window parent;		/* Parent window, which ostensibly contains
				 * one or menus as subwindows. */
    char *(names[]);		/* Entries in this array get filled in with
				 * pointers to statically-allocated strings
				 * giving names of menus, unless names is NULL.
				 * Array must have SX_MAX_MENUS entries.
				 * Strings are statically allocated and are
				 * part of the menu manager;  they will become
				 * invalid when the corresponding menu is
				 * modified or deleted. */
    Window windows[];		/* Entries in this array get filled in with
				 * X window ids corresponding to names, unless
				 * windows is NULL. */
{
    register Menu *menuPtr;
    register MenuGroup *groupPtr;
    int i;

    if (!init) {
	MenuInit();
    }

    groupPtr = (MenuGroup *) XLookUpAssoc(groupTable, parent);
    if (groupPtr == NULL) {
	return 0;
    }
    for (i = 0; i < SX_MAX_MENUS; i++) {
	menuPtr = groupPtr->menuPtrs[i];
	if (menuPtr == NULL) {
	    break;
	}
	if (names != NULL) {
	    names[i] = menuPtr->name;
	}
	if (windows != NULL) {
	    windows[i] = menuPtr->titleWindow;
	}
    }
    return i;
}

/*
 *----------------------------------------------------------------------
 *
 * Sx_MenuGetWindow --
 *
 *	Given the parent window containing a menu, and the name of the
 *	menu, get the X window id corresponding to the menu.
 *
 * Results:
 *	The return value is the X window id of a menu subwindow of
 *	parent whose name (title) is as given.  If there isn't any
 *	such menu, NULL is returned.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

Window
Sx_MenuGetWindow(parent, name)
    Window parent;		/* Parent window, which ostensibly has one
				 * or more menu subwindows as children. */
    char *name;			/* Title of desired menu. */
{
    register Menu *menuPtr;
    register MenuGroup *groupPtr;
    int i;

    if (!init) {
	MenuInit();
    }

    groupPtr = (MenuGroup *) XLookUpAssoc(groupTable, parent);
    if (groupPtr == NULL) {
	return NULL;
    }
    for (i = 0; i < SX_MAX_MENUS; i++) {
	menuPtr = groupPtr->menuPtrs[i];
	if (menuPtr == NULL) {
	    break;
	}
	if (String_Compare(name, menuPtr->name) == 0) {
	    return menuPtr->titleWindow;
	}
    }
    return NULL;
}

/*
 *----------------------------------------------------------------------
 *
 * MenuInit --
 *
 *	Performs various once-only initialization functions.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	Random stuff gets initialized.  See code for details.
 *
 *----------------------------------------------------------------------
 */

static void
MenuInit()
{
    displayWidth = DisplayWidth();
    displayHeight = DisplayHeight();

    cursorDot = XCreateCursor(dot_width, dot_height, dot_bits,
	    dotMask_bits, dot_x_hot, dot_y_hot,
	    BlackPixel, WhitePixel, GXcopy);
    cursorStar = XCreateCursor(star_width, star_height, star_bits,
	    starMask_bits, star_x_hot, star_y_hot,
	    BlackPixel, WhitePixel, GXcopy);
    
    groupTable = XCreateAssocTable(4);
    menuTable = XCreateAssocTable(8);

    if ((cursorDot == 0) || (cursorStar == 0)
	    || (menuTable == NULL) || (groupTable == NULL)) {
	Sx_Panic("Sx_MenuCreate: couldn't initialize cursor \
and/or hash tables.");
    }

    init = TRUE;
}

/*
 *----------------------------------------------------------------------
 *
 * MenuTitleRedisplay--
 *
 *	Redisplay the title window for a menu.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	The menu title for menu is redisplayed.
 *
 *----------------------------------------------------------------------
 */

static void
MenuTitleRedisplay(menuPtr)
    register Menu *menuPtr;		/* Menu to be redisplayed. */
{
    int background, foreground, x, y;

    /*
     * If this menu is currently selected, display it in reverse video.
     */
    
    if (menuPtr == selectedMenuPtr) {
	background = menuPtr->foreground;
	foreground = menuPtr->background;
    } else {
	background = menuPtr->background;
	foreground = menuPtr->foreground;
    }
    XPixSet(menuPtr->titleWindow, 0, 0, menuPtr->titleWidth,
	    menuPtr->titleHeight, background);
    x = (menuPtr->titleWidth - menuPtr->nameWidth)/2;
    y = (menuPtr->titleHeight - menuPtr->fontPtr->height)/2;
    XText(menuPtr->titleWindow, x, y, menuPtr->name,
	    String_Length(menuPtr->name), menuPtr->fontPtr->id,
	    foreground, background);
    
    /*
     * If the entire menu is disabled, display a bar through the title.
     */
    
    if (menuPtr->mask == 0) {
	XPixSet(menuPtr->titleWindow, 2, menuPtr->titleHeight/2 - 1,
		menuPtr->titleWidth-4, 2, foreground);
    }
}

/*
 *----------------------------------------------------------------------
 *
 * MenuExposeProc --
 *
 *	This procedure is called for each expose event on a menu title.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	The title gets redisplayed, and its size information gets
 *	reset.
 *
 *----------------------------------------------------------------------
 */

static void
MenuExposeProc(menuPtr, eventPtr)
    Menu *menuPtr;			/* Menu whose title was exposed. */
    XExposeEvent *eventPtr;		/* Gives new dimensions of title. */
{
    if (eventPtr->subwindow != NULL) {
	return;
    }

    menuPtr->titleWidth = eventPtr->width;
    menuPtr->titleHeight = eventPtr->height;

    MenuTitleRedisplay(menuPtr);
}

/*
 *----------------------------------------------------------------------
 *
 * MenuTitleMouseProc --
 *
 *	This procedure is called by the dispatcher when mouse-related
 *	events (entry, exit, or button press/release) occur in the
 *	title for a menu.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	The menu title gets highlighted when the cursor is in the
 *	title window.  When a button is pressed, the pull-down menu
 *	gets displayed and it grabs the mouse.
 *
 *----------------------------------------------------------------------
 */

static void
MenuTitleMouseProc(menuPtr, eventPtr)
    register Menu *menuPtr;		/* Menu title for which event
					 * occurred. */
    register XButtonEvent *eventPtr;	/* Describes what happened. */
{
    if (eventPtr->subwindow != 0) {
	return;
    }

    if (eventPtr->type == EnterWindow) {
	selectedMenuPtr = menuPtr;
	MenuTitleRedisplay(menuPtr);
    } else if (eventPtr->type == LeaveWindow) {
	if (selectedMenuPtr == menuPtr) {
	    selectedMenuPtr = NULL;
	}
	MenuTitleRedisplay(menuPtr);
    } else if (eventPtr->type == ButtonPressed) {
	register Menu **menuPtrPtr;
	
	/*
	 * Time to pull a menu down.  Save the origin of the menu group
	 * window, for use later in mouse tracking.  Compute where the
	 * menu should go on the screen, then display it.
	 */

	groupX = ((eventPtr->location >> 16) & 0xffff) - eventPtr->x;
	groupY = (eventPtr->location & 0xffff) - eventPtr->y;
	PullMenuDown(menuPtr, groupX, groupY + menuPtr->titleHeight);
	Sx_GrabMouse(menuPtr->menuWindow, cursorStar,
		ButtonReleased|MouseMoved);
	selectedEntry = -1;
	for (menuPtrPtr = menuPtr->groupPtr->menuPtrs;
		(*menuPtrPtr != menuPtr) && (*menuPtrPtr != NULL);
		menuPtrPtr++) {
	    groupX -=(*menuPtrPtr)->titleWidth;
	}
    }
}

/*
 *----------------------------------------------------------------------
 *
 * MenuDeleteEntries --
 *
 *	Recycle all of the memory associated with entries for a given
 *	menu.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	Memory gets reallocated.
 *
 *----------------------------------------------------------------------
 */

static void
MenuDeleteEntries(menuPtr)
    register Menu *menuPtr;	/* Menu whose entries are to be deleted. */
{
    register MenuEntry *entryPtr;
    int i;

    for (i = 0; i < menuPtr->numEntries; i++) {
	entryPtr = menuPtr->entryPtrs[i];
	if (entryPtr->leftText != NULL) {
	    Mem_Free((Address) entryPtr->leftText);
	}
	if (entryPtr->centerText != NULL) {
	    Mem_Free((Address) entryPtr->centerText);
	}
	if (entryPtr->rightText != NULL) {
	    Mem_Free((Address) entryPtr->rightText);
	}
	Mem_Free((Address) entryPtr);
    }
    menuPtr->numEntries = 0;
}

/*
 *----------------------------------------------------------------------
 *
 * MenuDestroyProc --
 *
 *	This procedure destroys a menu.  It's called by Sx when the
 *	window containing the menu is deleted.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	Internal storage associated with the pulldown menu is released.
 *
 *----------------------------------------------------------------------
 */

void
MenuDestroyProc(menuPtr)
    register Menu *menuPtr;		/* Menu to be deleted. */
{
    register MenuGroup *groupPtr;
    int i;

    /*
     * Free up everything associated with the menu itself.
     */

    XDeleteAssoc(menuTable, menuPtr->titleWindow);
    XDestroyWindow(menuPtr->menuWindow);
    MenuDeleteEntries(menuPtr);
    Mem_Free((Address) menuPtr->name);
    Mem_Free((Address) menuPtr);

    /*
     * Remove this menu from its group.  If the group is
     * now empty, delete it also.
     */

    groupPtr = menuPtr->groupPtr;
    for (i = 0; i < SX_MAX_MENUS; i++) {
	if (groupPtr->menuPtrs[i] == menuPtr) {
	    int j;

	    groupPtr->menuPtrs[i] = NULL;
	    for (j = i+1;
		    (j < SX_MAX_MENUS) && (groupPtr->menuPtrs[j] != NULL);
		    j++) {
		groupPtr->menuPtrs[j-1] = groupPtr->menuPtrs[j];
		groupPtr->menuPtrs[j] = NULL;
	    }
	}
    }
    if (groupPtr->menuPtrs[0] == NULL) {
	XDeleteAssoc(groupTable, groupPtr->w);
	Mem_Free((Address) groupPtr);
    }
}

/*
 *----------------------------------------------------------------------
 *
 * ComputeMenuLayout --
 *
 *	This procedure scans all the entries in a menu and computes
 *	the overall size of the menu as well as where to position
 *	each text field of each entry.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	Entries in the menu get modified to reflect the new layout.
 *	Menu->menuWindow also gets modified in size to reflect
 *	the new layout.
 *
 *----------------------------------------------------------------------
 */

static void
ComputeMenuLayout(menuPtr)
    register Menu *menuPtr;		/* Menu to resize. */
{
    register MenuEntry *entryPtr;
    int leftMax, centerMax, rightMax;
    int i, length;

    /*
     * Compute the maximum width of any entry for each column.
     */
    
    leftMax = centerMax = rightMax = 0;
    for (i = 0; i < menuPtr->numEntries; i++) {
	entryPtr = menuPtr->entryPtrs[i];
	if (entryPtr->leftText != NULL) {
	    length = XStringWidth(entryPtr->leftText,
		    menuPtr->fontPtr, 0, 0);
	    if (length > leftMax) {
		leftMax = length;
	    }
	}
	if (entryPtr->centerText != NULL) {
	    length = XStringWidth(entryPtr->centerText,
		    menuPtr->fontPtr, 0, 0);
	    if (length > centerMax) {
		centerMax = length;
	    }
	    entryPtr->centerPos = length;   /* Save to avoid recomputation. */
	}
	if (entryPtr->rightText != NULL) {
	    length = XStringWidth(entryPtr->rightText,
		    menuPtr->fontPtr, 0, 0);
	    if (length > rightMax) {
		rightMax = length;
	    }
	    entryPtr->rightPos = length;   /* Save to avoid recomputation. */
	}
    }

    /*
     * Size the window so that none of the columns overlap (be sure
     * to leave space for the fancy border).  Then go back and fill
     * in the exact drawing position for each string.
     */
    
    menuPtr->menuWidth = leftMax + centerMax + rightMax +
	    SHADOW_LEFT + SHADOW_RIGHT;
    XChangeWindow(menuPtr->menuWindow, menuPtr->menuWidth,
	    menuPtr->menuHeight);
    
    for (i = 0; i < menuPtr->numEntries; i++) {
	entryPtr = menuPtr->entryPtrs[i];
	entryPtr->centerPos = SHADOW_LEFT + leftMax
		+ (centerMax - entryPtr->centerPos)/2;
	entryPtr->rightPos = menuPtr->menuWidth - entryPtr->rightPos
		- SHADOW_RIGHT;
    }
}

/*
 *----------------------------------------------------------------------
 *
 * PadAndCopy --
 *
 *	This procedure makes a Mem_Alloc'ed copy of a string,
 *	adding a space onto each end of the string in the process.
 *
 * Results:
 *	*newPtr is filled in with a pointer to an area of memory (allocated
 *	with Mem_Alloc) that contains a copy of string with a space
 *	added onto each end.  If lengthPtr is non-NULL, *lengthPtr is filled
 *	in with the length of the new string (not including the terminator).
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

static void
PadAndCopy(string, newPtr, lengthPtr)
    char *string;		/* String to copy. */
    char **newPtr;		/* Tells where to store pointer to copy. */
    int *lengthPtr;		/* Where to store length of copy, or NULL. */
{
    register char *new;
    int length;

    length = String_Length(string);
    new = (char *) Mem_Alloc(length + 3);
    new[0] = ' ';
    (void) String_Copy(string, &new[1]);
    new[length+1] = ' ';
    new[length+2] = 0;
    *newPtr = new;
    if (lengthPtr != NULL) {
	*lengthPtr = length+2;
    }
}

/*
 *----------------------------------------------------------------------
 *
 * MenuEntryDisplay --
 *
 *	This procedure is called to display one entry of a menu.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	The entry is displayed in its menu.
 *
 *----------------------------------------------------------------------
 */

static void
MenuEntryDisplay(menuPtr, index)
    register Menu *menuPtr;	/* Menu containing entry to display. */
    int index;			/* Index of entry within menu. */
{
    register MenuEntry *entryPtr;
    register FontInfo *font = menuPtr->fontPtr;
    int y, background, foreground;

    entryPtr = menuPtr->entryPtrs[index];
    if (index == selectedEntry) {
	background = menuPtr->foreground;
	foreground = entryPtr->background;
    } else {
	background = entryPtr->background;
	foreground = menuPtr->foreground;
    }
    y = index*(font->height + 2*MARGIN) + SHADOW_LEFT;
    XPixSet(menuPtr->menuWindow, SHADOW_LEFT, y,
	    menuPtr->menuWidth - (SHADOW_LEFT + SHADOW_RIGHT),
	    font->height + 2*MARGIN, background);
    y += MARGIN;
    if (entryPtr->leftText != NULL) {
	XText(menuPtr->menuWindow, SHADOW_LEFT, y, entryPtr->leftText,
		entryPtr->leftSize, font->id, foreground, background);
    }
    if (entryPtr->centerText != NULL) {
	XText(menuPtr->menuWindow, entryPtr->centerPos, y,
		entryPtr->centerText, entryPtr->centerSize, font->id,
		foreground, background);
    }
    if (entryPtr->rightText != NULL) {
	XText(menuPtr->menuWindow, entryPtr->rightPos, y, entryPtr->rightText,
	    entryPtr->rightSize, font->id, foreground, background);
    }

    /*
     * If the entry is disabled, display a bar through it.
     */
    
    if (((1<<index) & menuPtr->mask) == 0) {
	y += (font->height)/2 - 1;
	XPixSet(menuPtr->menuWindow, SHADOW_LEFT + 2, y,
		menuPtr->menuWidth - (SHADOW_LEFT + SHADOW_RIGHT + 4),
		2, foreground);
    }
}

/*
 *----------------------------------------------------------------------
 *
 * MenuRedisplayProc --
 *
 *	This procedure is invoked by the Sx_ dispatcher to redisplay
 *	menus.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	The menu gets redisplayed.
 *
 *----------------------------------------------------------------------
 */

static void
MenuRedisplayProc(menuPtr)
    register Menu *menuPtr;		/* Menu to be redisplayed. */
{
    int i;

    SxDrawShadow(menuPtr->menuWindow, 0, 0,
	    menuPtr->menuWidth - SHADOW_LEFT - SHADOW_RIGHT,
	    menuPtr->menuHeight - SHADOW_TOP - SHADOW_BOTTOM,
	    menuPtr->foreground, savedPixmap);

    for (i = 0; i < menuPtr->numEntries; i++) {
	MenuEntryDisplay(menuPtr, i);
    }
}

/*
 *----------------------------------------------------------------------
 *
 * MenuFindTitle --
 *
 *	When tracking the mouse, this procedure determines if the
 *	mouse is in the title area of a menu.
 *
 * Results:
 *	The return value is a pointer to the menu over whose title
 *	the cursor is currently position, or NULL if the cursor
 *	isn't over any menu title in the caption containing menuPtr.
 *	If xPtr isn't NULL, *xPtr is filled in with the x-location
 *	(in RootWindow coords.) of the left edge of the returned
 *	title.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

static Menu *
MenuFindTitle(menuPtr, eventPtr, xPtr)
    register Menu *menuPtr;		/* Menu entry.  The idea is to find
					 * out if some other menu in this
					 * one's group is selected. */
    register XMouseEvent *eventPtr;	/* X event giving current mouse
					 * location. */
    int *xPtr;
{
    register MenuGroup *groupPtr;
    int x, curX, y, i;


    /*
     * Compute mouse location relative to upper-left corner of the menu group.
     */
    
    x = ((eventPtr->location >> 16) & 0xffff);
    y = (eventPtr->location & 0xffff) - groupY;

    /*
     * Make sure that y is within the range of this group.
     */

    if ((y < 0) || (y > menuPtr->titleHeight)) {
	return NULL;
    }

    /*
     * See which title's area it's in, if any.
     */
    
    if (x < groupX) {
	return NULL;
    }
    for (i = 0, curX = groupX, groupPtr = menuPtr->groupPtr;
	    i < SX_MAX_MENUS; i++, curX += menuPtr->titleWidth) {
	menuPtr = groupPtr->menuPtrs[i];
	if (menuPtr == NULL) {
	    return NULL;
	}
	if (x < curX + menuPtr->titleWidth) {
	    if (xPtr != NULL) {
		*xPtr = curX;
	    }
	    return menuPtr;
	}
    }
    return NULL;
}

/*
 *----------------------------------------------------------------------
 *
 * MenuMouseProc --
 *
 *	This procedure is called by the dispatcher when mouse-related
 *	events occur for a menu window.  This can only happen when
 *	the menu is being displayed, which means that a mouse button
 *	was pressed over a menu title and hasn't been released yet.
 *	When this procedure is called, the mouse has been grabbed
 *	for the menu window.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	This procedure tracks the motion of the mouse and button
 *	releases.  It highlights the menu entry underneath the mouse
 *	(if the entry is enabled) and calls a client-supplied
 *	procedure if the mouse button is released over an entry.
 *	When the buttons are released, the menu is unmapped and
 *	mouse-grabbing stops.
 *
 *----------------------------------------------------------------------
 */

static void
MenuMouseProc(menuPtr, eventPtr)
    register Menu *menuPtr;	/* Menu that is currently active. */
    XEvent *eventPtr;		/* Describes what just happened. */
{
    Menu *newPtr;

    if (eventPtr->type == MouseMoved) {
	register XMouseMovedEvent *mouseEventPtr;
	int index;

	mouseEventPtr = (XMouseMovedEvent *) eventPtr;

	/*
	 * Find the entry under the mouse (if any).  If it's enabled,
	 * then select it and unselect the previous one, if any.
	 */
	
	index = mouseEventPtr->y/(menuPtr->fontPtr->height
		+ 2*MARGIN);
	if ((mouseEventPtr->y < 0) || (index >= menuPtr->numEntries)
		|| (mouseEventPtr->x < SHADOW_LEFT)
		|| (mouseEventPtr->x > (menuPtr->menuWidth - SHADOW_RIGHT))
		|| (((1<<index) & menuPtr->mask) == 0)) {
	    index = -1;
	}
	if (selectedEntry != index) {
	    int prev = selectedEntry;

	    selectedEntry = index;
	    if (index >= 0) {
		MenuEntryDisplay(menuPtr, index);
	    }
	    if (prev >= 0) {
		MenuEntryDisplay(menuPtr, prev);
	    }
	}

	/*
	 * If no entry is currently selected, see if the mouse has
	 * moved over the title of a different menu.  If so, then
	 * deselect the current menu and select the one whose title
	 * is under the cursor.
	 */
	
	if (index == -1) {
	    int x;

	    newPtr = MenuFindTitle(menuPtr, mouseEventPtr, &x);
	    if ((newPtr != NULL) && (newPtr != menuPtr)) {
		selectedMenuPtr = newPtr;
		selectedEntry = -1;
		EraseMenu(menuPtr);
		MenuTitleRedisplay(menuPtr);
		PullMenuDown(newPtr, x, groupY + menuPtr->titleHeight);
		MenuTitleRedisplay(newPtr);
		Sx_GrabMouse(newPtr->menuWindow, cursorStar,
			ButtonReleased|MouseMoved);
	    }
	}
    } else if (eventPtr->type == ButtonReleased) {
	register XButtonEvent *butEventPtr = (XButtonEvent *) eventPtr;
	static int masks[] = {
	    LeftMask|MiddleMask,
	    LeftMask|RightMask,
	    MiddleMask|RightMask
	};

	/*
	 * When all of the buttons have been released, then unmap
	 * the menu.  If an entry was selected at the time, flash it
	 * before erasing the menu, then call its action procedure
	 * after erasing the menu.  The computation of whether all
	 * buttons have been released is a little tricky because the
	 * "detail" value doesn't take into account the current event.
	 */

	if ((butEventPtr->detail & masks[butEventPtr->detail&0377]) == 0) {
	    if (selectedEntry >= 0) {
		int i, index;

		index = selectedEntry;
		for (i = 0; i < 2; i++) {
		    selectedEntry = -1;
		    MenuEntryDisplay(menuPtr, index);
		    SxFlashWait();
		    selectedEntry = index;
		    MenuEntryDisplay(menuPtr, index);
		    SxFlashWait();
		}
	    }
	    selectedMenuPtr = NULL;
	    EraseMenu(menuPtr);
	    MenuTitleRedisplay(menuPtr);
	    Sx_UngrabMouse();
	    XFlush();

	    if (selectedEntry >= 0) {
		MenuEntry *entry =
			menuPtr->entryPtrs[selectedEntry];
		(*entry->proc)(entry->clientData, selectedEntry,
			menuPtr->titleWindow);

		/*
		 * Be VERY CAREFUL after returning from the client procedure.
		 * It's possible that the client deleted the menu, leaving
		 * us with a dangling pointer.  The best thing here is to
		 * do nothing but return.
		 */
	    } else {
		newPtr = MenuFindTitle(menuPtr, butEventPtr, (int *) NULL);
		if (newPtr != NULL) {
		    selectedMenuPtr = newPtr;
		    MenuTitleRedisplay(newPtr);
		}
	    }
	}
    }
}

/*
 *----------------------------------------------------------------------
 *
 * PullMenuDown --
 *
 *	This procedure does all the work of "pulling a menu down",
 *	i.e. making the menu visible on the screen.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	Information on the screen changes, and the previous pixels under
 *	the menu area get saved away.  MenuX and menuY get set, and the
 *	X server is grabbed to make sure that the saved pixels stay
 *	up-to-date.
 *
 *----------------------------------------------------------------------
 */

static void
PullMenuDown(menuPtr, x, y)
    register Menu *menuPtr;	/* Menu to pull down.  There must not
				 * currently be a menu pulled down. */
    int x, y;			/* Screen coordinates at which to pull
				 * menu down. */
{
    /*
     * Compute the location and dimensions of the new menu to be
     * displayed.  Float the menu up or to the left if that is
     * necessary to get it all on-screen.
     */
    
    savedWidth = menuPtr->menuWidth;
    if (savedWidth > displayWidth) {
	savedWidth = displayWidth;
    }
    savedHeight = menuPtr->menuHeight;
    if (savedHeight > displayHeight) {
	savedHeight = displayHeight;
    }
    if ((x+savedWidth) > displayWidth) {
	x = displayWidth - savedWidth;
    }
    if ((y+savedHeight) > displayHeight) {
	y = displayHeight - savedHeight;
    }
    if (x < 0) {
	x = 0;
    }
    if (y < 0) {
	y = 0;
    }
    menuX = x;
    menuY = y;

    /*
     * Save the previous pixels under the menu, then display the menu.
     */

    XGrabServer();
    savedPixmap = XPixmapSave(RootWindow, x, y, savedWidth, savedHeight);
    XMoveWindow(menuPtr->menuWindow, x, y);
    XMapWindow(menuPtr->menuWindow);
}

/*
 *----------------------------------------------------------------------
 *
 * EraseMenu --
 *
 *	This procedure does all the work of erasing a pulled-down
 *	menu from the screen and restoring the screen contents.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	Information changes on the screen.
 *
 *----------------------------------------------------------------------
 */

static void
EraseMenu(menuPtr)
    Menu *menuPtr;		/* Menu to erase.  Must currently be
				 * pulled down. */
{
    /*
     * If pixels were successfully saved, then restore from them.
     * Otherwise, just let the are of the menuPtr be redisplayed in
     * the normal fashion.
     */

    if (savedPixmap == 0) {
	XUnmapWindow(menuPtr->menuWindow);
    } else {
	XPixmapPut(menuPtr->menuWindow,  0, 0, 0, 0,
		savedWidth, savedHeight, savedPixmap, GXcopy, AllPlanes);
	XUnmapTransparent(menuPtr->menuWindow);
	XFreePixmap(savedPixmap);
    }
    savedPixmap = 0;
    XUngrabServer();
}
