/*
 *	$Source: /u1/Sx.new/code/RCS/sxEntry.c,v $
 *	$Header: sxEntry.c,v 1.1 86/12/03 16:09:57 swick Exp $
 */

#ifndef lint
static char *rcsid_sxEntry_c = "$Header: sxEntry.c,v 1.1 86/12/03 16:09:57 swick Exp $";
#endif	lint

/* 
 * sxEntry.c --
 *
 *	This file contains the code that implements text entry
 *	subwindows for the Sx package.  Text entries are windows
 *	that display a label and allow the user to type in (and
 *	edit) a textual value to associate with the label.
 *
 * Copyright (C) 1986 Regents of the University of California.
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appear in all copies.  The University of California makes
 * no representations about the suitability of this software for
 * any purpose.  It is provided "as is" without express or
 * implied warranty.
 */

#ifndef lint
static char rcsid[] = "$Header: sxEntry.c,v 1.1 86/12/03 16:09:57 swick Exp $ SPRITE (Berkeley)";
#endif not lint

#include <sys/types.h>
#include <X/Xlib.h>
#include "sprite.h"
#include "char.h"
#include "mem.h"
#include "string.h"
#include "sx.h"
#include "sxInt.h"

/*
 * A record of the following type is used to store information about
 * each entry.
 */

typedef struct {
    Window w;			/* Window used by entry. */
    char *label;		/* Fixed label for entry (storage is
				 * dynamically allocated).  If NULL, then
				 * no label is displayed. */
    char *text;			/* Pointer to client-owned space in which
				 * to store user type-in. */
    int textSize;		/* Number of bytes available for non-null
				 * characters at text (actual space is this
				 * + 1, to account for null terminator). */
    FontInfo *fontPtr;		/* Font to use for displaying entry. */
    int x;			/* X-location at which to display left edge
				 * of first character in text. */
    int y;			/* Y-location at which to display text.
				 * Recomputed each time window changes size. */
    int foreground;		/* Foreground color for display. */
    int background;		/* Background color for display. */
    int caret;			/* Index of character whose left edge is
				 * the caret position.  Always >= 0. */
    int caretX, caretY;		/* Location (in pixels, in w) of upper-left
				 * corner of caret pixmap. */
    int selectFirst;		/* First character that's selected.  -1
				 * means nothing's selected. */
    int selectLast;		/* Last character that's selected.  -1
				 * means nothing's selected. */
    int selectAnchor;		/* The right mouse button adjusts one end
				 * of the selection;  this gives the position
				 * of the end that's fixed. */
    int flags;			/* Miscellaneous flag values:  see below. */
} Entry;

/*
 * Flags for entries:
 *
 * FOCUS_WINDOW:		1 means that this window has the focus.
 * CARET_OFF:			1 means that the caret has been turned off,
 *				pending other changes to the window.
 */

#define FOCUS_WINDOW	1
#define CARET_OFF	2

/*
 * How much space to leave between label and left edge of window:
 */

#define LEFT_MARGIN 2

/*
 * The cursor used when the pointer is in entry windows:
 */

#include "cursors/ptr"
static Cursor cursor = 0;

/*
 * Bitmaps used to draw the caret:
 */

#include "caret.bits"
#include "caretMask.bits"
static Bitmap caretBitmap;
static Bitmap caretMaskBitmap;

/*
 * Hash table used to map from X window ids to Entry structures:
 */

static XAssocTable *entryTable;
extern XAssocTable *XCreateAssocTable();

/*
 * Forward references to things defined in this file:
 */

extern void		EntryDelete();
extern void		EntryDestroyProc();
extern void		EntryDrawCaret();
extern void		EntryEraseCaret();
extern void		EntryExposeProc();
extern Boolean		EntryFindChar();
extern void		EntryInit();
extern void		EntryInsert();
extern void		EntryKeyProc();
extern void		EntryMouseProc();
extern void		EntryRedisplay();
extern void		EntrySelChanged();
extern int		EntrySelGet();

/*
 *----------------------------------------------------------------------
 *
 * Sx_EntryMake --
 *
 *	Make a window into an entry subwindow.  An entry is a label
 *	with space after it for typing in text.  The text appears in
 *	a string area provided by the caller.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	From this moment on, the entire area of window will be
 *	used to display the entry.  The contents of text will
 *	change spontaneously as the user invokes operations in
 *	the entry window.
 *
 *----------------------------------------------------------------------
 */

void
Sx_EntryMake(window, label, fontPtr, foreground, background, text, size)
    Window window;		/* Window to use for entry.  If the window
				 * is already in use for an entry, this
				 * call can be used to change the text and
				 * other parameters (the window will be
				 * redisplayed).  If the window isn't already
				 * an entry window, then it shouldn't yet be
				 * mapped (no redisplay will be done). */
    char *label;		/* Text to appear at left edge of window,
				 * labelling the entry.  May be NULL. */
    FontInfo *fontPtr;		/* Font to use for displaying info in window,
				 * or NULL to use default font. */
    int foreground;		/* Color to use for displaying text. */
    int background;		/* Background color for window. */
    char *text;			/* Stuff that user types will be stored here,
				 * null-terminated.  Caller must initialize. */
    int size;			/* Number of bytes of storage at text:
				 * determines longest string that will be
				 * accepted. */
{
    register Entry *entryPtr;
    Boolean redisplay;

    EntryInit();

    if (fontPtr == NULL) {
	fontPtr = Sx_GetDefaultFont();
    }

    /*
     * See if this window is already an entry window.  If so, release
     * the fields that will be reallocated.  If not, allocate a new
     * structure and do once-only initialization.
     */

    entryPtr = (Entry *) XLookUpAssoc(entryTable, window);
    if (entryPtr != NULL) {
	redisplay = TRUE;
	if (entryPtr->label != NULL) {
	    Mem_Free(entryPtr->label);
	}
	entryPtr->flags |= CARET_OFF;
    } else {
	redisplay = FALSE;
	entryPtr = (Entry *) Mem_Alloc(sizeof(Entry));
	entryPtr->w = window;
	entryPtr->flags = CARET_OFF;
	(void) Sx_HandlerCreate(window, ButtonPressed|ButtonReleased
		|LeftDownMotion|RightDownMotion|EnterWindow|LeaveWindow,
		EntryMouseProc, (ClientData) entryPtr);
	(void) Sx_HandlerCreate(window, KeyPressed, EntryKeyProc,
		(ClientData) entryPtr);
	(void) Sx_HandlerCreate(window, ExposeWindow, EntryExposeProc,
		(ClientData) entryPtr);
	(void) Sx_HandlerCreate(window, SX_DESTROYED, EntryDestroyProc,
		(ClientData) entryPtr);
	XDefineCursor(window, cursor);
	XMakeAssoc(entryTable, window, (caddr_t) entryPtr);
    }

    /*
     * Reset the fields of the entry for the new parameters.
     */

    if (label == NULL) {
	entryPtr->label = NULL;
	entryPtr->x = LEFT_MARGIN;
    } else {
	entryPtr->label = (char *) Mem_Alloc(String_Length(label) + 1);
	String_Copy(label, entryPtr->label);
	entryPtr->x =  LEFT_MARGIN + XStringWidth(label, fontPtr, 0, 0);
    }
    entryPtr->text = text;
    entryPtr->textSize = size - 1;
    text[entryPtr->textSize] = 0;	/* In case caller didn't terminate. */
    entryPtr->fontPtr = fontPtr;
    entryPtr->y = 0;
    entryPtr->foreground = foreground;
    entryPtr->background = background;
    entryPtr->caret = 0;
    entryPtr->selectFirst = -1;
    entryPtr->selectLast = -1;
    entryPtr->selectAnchor = -1;

    if (redisplay) {
	XPixSet(window, 0, 0, DisplayWidth(), DisplayHeight(),
		entryPtr->background);
	if (entryPtr->label != NULL) {
	    XText(window, LEFT_MARGIN, entryPtr->y, entryPtr->label,
		    String_Length(entryPtr->label), entryPtr->fontPtr->id,
		    entryPtr->foreground, entryPtr->background);
	}
	EntryRedisplay(entryPtr, 0, String_Length(entryPtr->text) - 1);
	EntryDrawCaret(entryPtr);
    }
}

/*
 *----------------------------------------------------------------------
 *
 * Sx_EntryCreate --
 *
 *	Like Sx_EntryMake, except also create the window that will hold
 *	the entry.
 *
 * Results:
 *	The return value is the X id for a new window that behaves
 *	as an entry.  It will have the given location and size in
 *	parent.
 *
 * Side effects:
 *	The area of the new window will be used to display the entry.
 *	The contents of text will change spontaneously as the user
 *	invokes operations in the entry window.
 *
 *----------------------------------------------------------------------
 */

Window
Sx_EntryCreate(parent, x, y, width, height, border, label, fontPtr, foreground,
	background, text, size)
    Window parent;		/* Parent in which to create entry window. */
    int x, y;			/* Location of upper left corner of new
				 * window, in coords. of parent. */
    int width, height;		/* Dimensions of new window, in pixels. */
    int border;			/* Width of border for new window. */
    char *label;		/* Text to appear at left edge of window,
				 * labelling the entry. */
    FontInfo *fontPtr;		/* Font to use for displaying info in window,
				 * or NULL to use default font. */
    int foreground;		/* Color to use for displaying text. */
    int background;		/* Background color for window. */
    char *text;			/* Stuff that user types will be stored here,
				 * null-terminated.  Caller must initialize. */
    int size;			/* Number of bytes of storage at text:
				 * determines longest string that will be
				 * accepted. */
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
	    Sx_Panic("Sx_EntryCreate:  couldn't create border pixmap.");
	}
    }
    w = XCreateWindow(parent, x, y, width, height, border, borderPixmap,
	    WhitePixmap);
    if (w == NULL) {
	Sx_Panic("Sx_EntryCreate:  couldn't create new window.");
    }
    if ((borderPixmap != BlackPixmap) && (borderPixmap != WhitePixmap)) {
	XFreePixmap(borderPixmap);
    }
    Sx_EntryMake(w, label, fontPtr, foreground, background, text, size);
    return w;
}

/*
 *----------------------------------------------------------------------
 *
 * EntryExposeProc --
 *
 *	This procedure is called by the Sx dispatcher whenever an
 *	ExposeWindow event occurs for an Entry window.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	The entry gets redisplayed.
 *
 *----------------------------------------------------------------------
 */

static void
EntryExposeProc(entryPtr, eventPtr)
    register Entry *entryPtr;		/* Entry for which event occurred. */
    XExposeEvent *eventPtr;		/* Event that occurred. */
{
    if (eventPtr->subwindow != NULL) {
	return;
    }
    entryPtr->y = (eventPtr->height - entryPtr->fontPtr->height)/2;
    XPixSet(entryPtr->w, 0, 0, DisplayWidth(), DisplayHeight(),
	    entryPtr->background);
    if (entryPtr->label != NULL) {
	XText(entryPtr->w, LEFT_MARGIN, entryPtr->y, entryPtr->label,
		String_Length(entryPtr->label), entryPtr->fontPtr->id,
		entryPtr->foreground, entryPtr->background);
    }
    EntryRedisplay(entryPtr, 0, String_Length(entryPtr->text) - 1);
    EntryDrawCaret(entryPtr);
}

/*
 *----------------------------------------------------------------------
 *
 * EntryMouseProc --
 *
 *	This procedure is invoked by the Sx dispatcher whenever the
 *	mouse enters or leaves an entry window, whenever a mouse button
 *	goes down or up, and whenever the mouse moves with a button
 *	down.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	The selection and caret get adjusted in response to button
 *	presses.
 *
 *----------------------------------------------------------------------
 */

static void
EntryMouseProc(entryPtr, eventPtr)
    register Entry *entryPtr;		/* Entry for which event occurred. */
    register XButtonEvent *eventPtr;	/* Event that occurred. */
{
    int oldFirst, oldLast, tmp, index;
    Boolean rightSide;
    Boolean setCaret = FALSE;
    static int button = -1;
    static int lastX, lastY;		/* Last place where left button
					 * was clicked.  Used to detect
					 * multiple clicks. */
    static Entry *lastEntryPtr;		/* Last entry clicked in.  Also used
					 * to detect multiple clicks. */
    static int repeatCount = 0;		/* Count of multiple clicks. */

    if (eventPtr->subwindow != NULL) {
	return;
    }

    /*
     * When entering the window, turn on the caret.  Turn it off
     * again when leaving the window.
     */
    
    if ((eventPtr->type == EnterWindow)
	    && ((eventPtr->detail & 0x77) != VirtualCrossing)) {
	entryPtr->flags |= FOCUS_WINDOW;
	EntryDrawCaret(entryPtr);
	return;
    }
    if ((eventPtr->type == LeaveWindow)
	    && ((eventPtr->detail & 0x77) != VirtualCrossing)) {
	entryPtr->flags &= ~FOCUS_WINDOW;
	EntryEraseCaret(entryPtr);
	return;
    }

    /*
     * When the button goes up, release the grab we put on the mouse.
     */
    
    if (eventPtr->type == ButtonReleased) {
	if ((eventPtr->detail & 07) == button) {
	    Sx_UngrabMouse();
	    button = -1;
	}
	return;
    }

    /*
     * When the first button goes down, put a grab on the mouse.
     * Ignore mouse events if we didn't see the down event.  Count
     * successive clicks in the same place.
     */
    
    if (eventPtr->type == ButtonPressed) {
	button = eventPtr->detail & 07;
	if (button == LeftButton) {
	    Sx_GrabMouse(entryPtr->w, cursor, ButtonReleased|LeftDownMotion);
	    if ((lastEntryPtr == entryPtr)
		    && ((lastX + 1) >= eventPtr->x)
		    && ((lastX - 1) <= eventPtr->x)
		    && ((lastY + 1) >= eventPtr->y)
		    && ((lastY - 1) <= eventPtr->y)) {
		repeatCount += 1;
		if (repeatCount > 2) {
		    repeatCount = 0;
		}
	    } else {
		repeatCount = 0;
	    }
	    lastX = eventPtr->x;
	    lastY = eventPtr->y;
	    lastEntryPtr = entryPtr;
	} else if (button == RightButton) {
	    Sx_GrabMouse(entryPtr->w, cursor, ButtonReleased|RightDownMotion);
	} else {
	    button = -1;
	    return;
	}
    } else {
	if (button == -1) {
	    return;
	}
    }

    /*
     * A mouse button is down.  If it's the left button, or if it's
     * the right button and the selection isn't in this window, then
     * start a new selection.  Also set the caret unless the shift key
     * is down.  (It's a little easier to see what you're pointing to
     * if the mouse doesn't have to be right on top of it to select it.
     * Offset the hot spot to accomplish this).
     */
    
    rightSide = EntryFindChar(entryPtr, eventPtr->x - 2, &index);
    if (index < 0) {
	return;
    }
    oldFirst = entryPtr->selectFirst;
    oldLast = entryPtr->selectLast;
    if (oldFirst < 0) {
	Sx_SelectionSet(EntrySelGet, EntrySelChanged, (ClientData) entryPtr);
    }
    if ((oldFirst < 0) || (button == LeftButton)) {
	entryPtr->selectFirst = entryPtr->selectLast = index;
	entryPtr->selectAnchor = index;
	if ((eventPtr->detail & ShiftMask) == 0) {
	    setCaret = TRUE;
	}
	goto setSelection;
    } else if (button == RightButton) {
	if (index >= entryPtr->selectAnchor) {
	    entryPtr->selectFirst = entryPtr->selectAnchor;
	    entryPtr->selectLast = index;
	} else {
	    entryPtr->selectFirst = index;
	    entryPtr->selectLast = entryPtr->selectAnchor;
	}
    }

    /*
     * Depending on how many clicks there have been, round the selection up:
     * 1st click:		no rounding
     * 2nd click:		round to word boundary
     * 3rd click:		round to line boundary
     */
    
    setSelection:
    switch(repeatCount) {
	
	case 1: {
	    static char wordMask[16] = {0, 0, 0, 0, 0, 0, 0xff, 0x3,
		    0xfe, 0xff, 0xff, 0x87, 0xfe, 0xff, 0xff, 0x3};
	    static char bit[8] = {0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80};
	    register char c;

	    c = entryPtr->text[entryPtr->selectFirst];
	    if (!(c & 0200) && (wordMask[c>>3] & bit[c&07])) {
		while (entryPtr->selectFirst > 0) {
		    entryPtr->selectFirst--;
		    c = entryPtr->text[entryPtr->selectFirst];
		    if ((c & 0200) || !(wordMask[c>>3] & bit[c&07])) {
			entryPtr->selectFirst++;
			break;
		    }
		}
	    }
	    c = entryPtr->text[entryPtr->selectLast];
	    if ((c != 0) && !(c & 0200) && (wordMask[c>>3] & bit[c&07])) {
		while (TRUE) {
		    entryPtr->selectLast++;
		    c = entryPtr->text[entryPtr->selectLast];
		    if ((c == 0) || (c & 0200) ||
			    !(wordMask[c>>3] & bit[c&07])) {
			entryPtr->selectLast--;
			break;
		    }
		}
	    }
	    break;
	}

	case 2: {
	    entryPtr->selectFirst = 0;
	    entryPtr->selectLast = String_Length(entryPtr->text) - 1;
	    break;
	}
    }

    /*
     * Set the caret (if necessary) to the beginning or end of the
     * selection, whichever is closer.
     */

    if (setCaret) {
	int newCaret, tmp;

	tmp = (2*index) - (entryPtr->selectFirst + entryPtr->selectLast);
	if ((tmp > 0) || ((tmp == 0) && rightSide)) {
	    newCaret = entryPtr->selectLast + 1;
	} else {
	    newCaret = entryPtr->selectFirst;
	}
	if (newCaret != entryPtr->caret) {
	    EntryEraseCaret(entryPtr);
	    entryPtr->caret = newCaret;
	}
    }

    /*
     * Compute how much to redisplay.  Don't redisplay the part of
     * the selection that was selected before.
     */
    
    if (oldFirst >= 0) {
	if (oldFirst < entryPtr->selectFirst) {
	    tmp = oldLast;
	    if (tmp >= entryPtr->selectFirst) {
		tmp = entryPtr->selectFirst-1;
	    }
	    EntryEraseCaret(entryPtr);
	    EntryRedisplay(entryPtr, oldFirst, tmp);
	}
	if  (oldLast > entryPtr->selectLast) {
	    tmp = oldFirst;
	    if (tmp <= entryPtr->selectLast) {
		tmp = entryPtr->selectLast + 1;
	    }
	    EntryEraseCaret(entryPtr);
	    EntryRedisplay(entryPtr, tmp, oldLast);
	}
    }
    if (entryPtr->selectFirst >= 0) {
	if (entryPtr->selectFirst < oldFirst) {
	    tmp = entryPtr->selectLast;
	    if (tmp >= oldFirst) {
		tmp = oldFirst-1;
	    }
	    EntryEraseCaret(entryPtr);
	    EntryRedisplay(entryPtr, entryPtr->selectFirst, tmp);
	}
	if (entryPtr->selectLast > oldLast) {
	    tmp = entryPtr->selectFirst;
	    if (tmp <= oldLast) {
		tmp = oldLast+1;
	    }
	    EntryEraseCaret(entryPtr);
	    EntryRedisplay(entryPtr, tmp, entryPtr->selectLast);
	}
    }
    EntryDrawCaret(entryPtr);
}

/*
 *----------------------------------------------------------------------
 *
 * EntryKeyProc --
 *
 *	This procedure is invoked by the Sx dispatcher whenever a
 *	key is pressed in an entry window.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	The text in the window is modified.
 *
 *----------------------------------------------------------------------
 */

static void
EntryKeyProc(entryPtr, eventPtr)
    register Entry *entryPtr;		/* Entry for which event occurred. */
    XKeyEvent *eventPtr;		/* Event that occurred. */
{
    char *keyString;
    char insert[2];
    int nBytes;

    if (eventPtr->subwindow != NULL) {
	return;
    }

    /*
     * Convert the weird raw key value to an ASCII string.  For
     * each character, insert it if it's printable.  Process
     * line-editing control characters specially, and throw
     * away anything else.
     */
    
    keyString = XLookupMapping(eventPtr, &nBytes);
    if (nBytes == 0) {
	return;
    }
    insert[1] = 0;
    EntryEraseCaret(entryPtr);
    for ( ; *keyString != 0; keyString++) {
	if (Char_IsPrint(*keyString)) {
	    insert[0] = *keyString;
	    EntryInsert(entryPtr, insert, entryPtr->caret);
	} else if (*keyString == '\03') {
	    /*
	     * Control-C: copy the selection to the insertion point.
	     */
#define MAX_AT_ONCE 50
	    char selection[MAX_AT_ONCE+1], format[SX_FORMAT_SIZE];
	    int bytesThisTime, offset;
	    for (offset = 0; entryPtr->caret < entryPtr->textSize;
		    offset += bytesThisTime) {
		bytesThisTime = Sx_SelectionGet("text", offset, MAX_AT_ONCE,
			selection, format);
		if ((bytesThisTime <= 0)
			|| (String_Compare(format, "text") != 0)) {
		    break;
		}
		selection[bytesThisTime] = 0;
		EntryInsert(entryPtr, selection, entryPtr->caret);
	    }
	} else if (*keyString == '\04') {
	    /*
	     * Control-D: delete the selection, if it's in this window.
	     */
	    if (entryPtr->selectFirst >= 0) {
		EntryDelete(entryPtr, entryPtr->selectFirst,
			entryPtr->selectLast);
	    }
	} else if ((*keyString == '\b') || (*keyString == '\177')) {
	    /*
	     * Control-H or delete: erase a character.
	     */
	    if (entryPtr->caret != 0) {
		EntryDelete(entryPtr, entryPtr->caret-1, entryPtr->caret-1);
	    }
	} else if (*keyString == '\25') {
	    /*
	     * Control-U: delete the whole line.
	     */
	    if (entryPtr->caret != 0) {
		EntryDelete(entryPtr, 0, entryPtr->caret-1);
	    }
	} else if (*keyString == '\27') {
	    /*
	     * Control-W: delete the last word typed.
	     */
	    int i;
	    Boolean seenNonSpace = FALSE;
	    for (i = entryPtr->caret-1; i >= 0; i--) {
		if (Char_IsSpace(entryPtr->text[i])) {
		    if (seenNonSpace) {
			break;
		    }
		} else {
		    seenNonSpace = TRUE;
		}
	    }
	    EntryDelete(entryPtr, i+1, entryPtr->caret-1);
	}
    }
    EntryDrawCaret(entryPtr);
}

/*
 *----------------------------------------------------------------------
 *
 * EntryRedisplay --
 *
 *	This procedure is called to redisplay part or all of the
 *	variable portion of an entry.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	The characters indexed first through last from the typed-in
 *	part of the entry are redisplayed.  The entry's label is NOT
 *	redisplayed.
 *
 *----------------------------------------------------------------------
 */

static void
EntryRedisplay(entryPtr, first, last)
    register Entry *entryPtr;		/* What to redisplay. */
    int first, last;			/* Range of characters to redisplay. */
{
    char savedChar;
    int x, next;

    /*
     * Compute the starting location.
     */

    savedChar = entryPtr->text[first];
    entryPtr->text[first] = 0;
    x = entryPtr->x + XStringWidth(entryPtr->text, entryPtr->fontPtr, 0, 0);
    entryPtr->text[first] = savedChar;

    /*
     * Display the text in up to three chunks:  one chunk that is
     * to the left of the selected text, one chunk that is highlighted,
     * and one chunk that is to the right of the selected text.
     */
    
    if (entryPtr->selectFirst > first) {
	next = entryPtr->selectFirst;
	if (next > last) {
	    next = last+1;
	}
	XText(entryPtr->w, x, entryPtr->y, &entryPtr->text[first], next-first,
		entryPtr->fontPtr->id, entryPtr->foreground,
		entryPtr->background);
	savedChar = entryPtr->text[next];
	entryPtr->text[next] = 0;
	x += XStringWidth(&entryPtr->text[first], entryPtr->fontPtr, 0, 0);
	entryPtr->text[next] = savedChar;
	first = next;
    }

    if (entryPtr->selectLast >= first) {
	next = entryPtr->selectLast + 1;
	if (next > last) {
	    next = last+1;
	}
	XText(entryPtr->w, x, entryPtr->y, &entryPtr->text[first], next-first,
		entryPtr->fontPtr->id, entryPtr->background,
		entryPtr->foreground);
	savedChar = entryPtr->text[next];
	entryPtr->text[next] = 0;
	x += XStringWidth(&entryPtr->text[first], entryPtr->fontPtr, 0, 0);
	entryPtr->text[next] = savedChar;
	first = next;
    }

    if (first <= last) {
	XText(entryPtr->w, x, entryPtr->y, &entryPtr->text[first],
		last + 1 - first, entryPtr->fontPtr->id,
		entryPtr->foreground, entryPtr->background);
    }
}

/*
 *----------------------------------------------------------------------
 *
 * EntryInsert --
 *
 *	Insert new characters into the middle of an entry, and
 *	redisplay them.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	The characters in string are inserted into the entry just
 *	before the "before"th character.  The entry will be
 *	truncated if necessary to keep it from overflowing its
 *	allotted storage area.
 *
 *----------------------------------------------------------------------
 */

static void
EntryInsert(entryPtr, string, before)
    register Entry *entryPtr;		/* Entry to be modified. */
    char *string;			/* What to insert. */
    int before;				/* Which character to insert it in
					 * front of. */
{
    int insertLength, suffixLength, spaceLeft;
    register char *src, *dst;
    char *insert;

    /*
     * This code is a bit tricky because of the space limitations on text.
     * Compute how much space is left, then truncate the resulting new
     * string if necessary to make it fit.  This can involve chopping
     * characters from the old string, or even ignoring some of the
     * characters from the insert string.
     */
    
    insert = &entryPtr->text[before];
    suffixLength = String_Length(insert);
    insertLength = String_Length(string);
    spaceLeft = entryPtr->textSize - (before + insertLength + suffixLength);
    if (spaceLeft < 0) {
	suffixLength += spaceLeft;
	spaceLeft = 0;
	if (suffixLength < 0) {
	    insertLength += suffixLength;
	    suffixLength = 0;
	    if (insertLength == 0) {
		return;
	    }
	}
    }

    /*
     * Move the tail of the current text to its new location, then
     * copy the inserted text into the gap.
     */

    dst = &entryPtr->text[entryPtr->textSize - spaceLeft];
    *dst = 0;
    dst--;
    for (src = dst - insertLength; src >= insert; src--, dst--) {
	*dst = *src;
    }
    String_NCopy(insertLength, string, insert);

    /*
     * Modify the selection and caret, if necessary, to keep their
     * same positions relative to the text.
     */

    if (entryPtr->selectFirst >= before) {
	entryPtr->selectFirst += insertLength;
	if (entryPtr->selectFirst >= entryPtr->textSize) {
	    entryPtr->selectFirst = entryPtr->selectLast = -1;
	}
    }
    if (entryPtr->selectLast >= before) {
	entryPtr->selectLast += insertLength;
	if (entryPtr->selectLast >= entryPtr->textSize) {
	    entryPtr->selectLast = entryPtr->textSize - 1;
	}
    }
    if (entryPtr->caret >= before) {
	entryPtr->caret += insertLength;
	if (entryPtr->caret > entryPtr->textSize) {
	    entryPtr->caret = entryPtr->textSize;
	}
    }
    EntryRedisplay(entryPtr, before, before + insertLength + suffixLength);
}

/*
 *----------------------------------------------------------------------
 *
 * EntryDelete --
 *
 *	Delete one or more characters from an entry.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	Characters indexed first to last are deleted from the text
 *	of entry.  The screen is updated to reflect the change.
 *
 *----------------------------------------------------------------------
 */

static void
EntryDelete(entryPtr, first, last)
    register Entry *entryPtr;		/* Entry to be modified. */
    int first, last;			/* Range of characters to delete. */
{
    int x;

    /*
     * Copy the tail of the string down over the deleted part, and
     * update the selection and caret, if any.
     */
    
    String_Copy(&entryPtr->text[last+1], &entryPtr->text[first]);
    if (entryPtr->selectFirst >= first) {
	if (entryPtr->selectFirst > last) {
	    entryPtr->selectFirst -= last+1-first;
	} else {
	    entryPtr->selectFirst = first;
	}
	if (entryPtr->text[entryPtr->selectFirst] == 0) {
	    entryPtr->selectFirst = entryPtr->selectLast = -1;
	}
    }
    if (entryPtr->selectLast >= first) {
	if (entryPtr->selectLast > last) {
	    entryPtr->selectLast -= last+1-first;
	} else {
	    entryPtr->selectLast = first-1;
	}
	if (entryPtr->selectLast < entryPtr->selectFirst) {
	    entryPtr->selectFirst = entryPtr->selectLast = -1;
	}
    }
    if (entryPtr->caret > first) {
	if (entryPtr->caret > last) {
	    entryPtr->caret -= last+1-first;
	} else {
	    entryPtr->caret = first;
	}
    }

    /*
     * Redisplay the entry starting at the first byte deleted,
     * and also clear the area where the tail of the string
     * used to be displayed.
     */
    
    EntryRedisplay(entryPtr, first, String_Length(entryPtr->text) - 1);
    x = entryPtr->x + XStringWidth(entryPtr->text, entryPtr->fontPtr, 0, 0);
    XPixSet(entryPtr->w, x, entryPtr->y, DisplayWidth(), DisplayHeight(),
	    entryPtr->background);
}

/*
 *----------------------------------------------------------------------
 *
 * EntryFindChar --
 *
 *	Determine which character lies at a given position in an
 *	entry's window.
 *
 * Results:
 *	The word at *indexPtr is filled in with the index of the
 *	closest character in entry's text to the x-position given
 *	by x.  The return value is TRUE if x is on the right side
 *	of the closest character, and FALSE if it's on the left.
 *	If entryPtr's text is empty, then -1 is stored at *indexPtr.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

static Boolean
EntryFindChar(entryPtr, x, indexPtr)
    register Entry *entryPtr;		/* Entry of interest. */
    int x;				/* Location in entryPtr's window. */
    int *indexPtr;			/* Where to store index of closest
					 * character to x. */
{
    register char *p, c;
    int curX, width;
    register FontInfo *fontPtr = entryPtr->fontPtr;

    for (curX = entryPtr->x, p = entryPtr->text, c = *p; c != 0; p++, c = *p) {
	if (fontPtr->fixedwidth) {
	    width = fontPtr->width;
	} else if ((c >= fontPtr->firstchar) && (c <= fontPtr->lastchar)) {
	    width = fontPtr->widths[c - fontPtr->firstchar];
	} else {
	    width = 0;
	}
	curX += width;
	if (curX > x) {
	    *indexPtr = p - entryPtr->text;
	    return ((curX - x) < (width/2));
	}
    }
    *indexPtr = p - entryPtr->text - 1;
    return TRUE;
}

/*
 *----------------------------------------------------------------------
 *
 * EntrySelGet --
 *
 *	Called by the Sx selection package when someone wants to know
 *	what's selected.
 *
 * Results:
 *	See the documentation for Sx_SelectionGet.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */
    /* ARGSUSED */
static int
EntrySelGet(entryPtr, desiredFormat, firstByte, numBytes, selectionPtr,
	formatPtr)
    register Entry *entryPtr;		/* Entry that contains selection. */
    char *desiredFormat;		/* Desired format of entry.  This
					 * procedure can only handle text, so
					 * this parameter is ignored. */
    int firstByte;			/* Index of first desired byte. */
    int numBytes;			/* Max no. of bytes to return. */
    char *selectionPtr;			/* Store bytes of selection here. */
    char *formatPtr;			/* Store format of selection here. */
{
    int bytesAvailable;

    if (entryPtr->selectFirst < 0) {
	return -1;
    }
    String_Copy("text", formatPtr);
    bytesAvailable = entryPtr->selectLast + 1
	    - (entryPtr->selectFirst + firstByte);
    if (numBytes > bytesAvailable) {
	numBytes = bytesAvailable;
	if (numBytes <= 0) {
	    return 0;
	}
    }
    String_NCopy(numBytes, &entryPtr->text[entryPtr->selectFirst + firstByte],
	    selectionPtr);
    return numBytes;
}

/*
 *----------------------------------------------------------------------
 *
 * EntrySelChanged --
 *
 *	Called by Sx whenever the selection is changed away from
 *	an entry window.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	The selection is unhighlighted, and marked as not being in
 *	this window.
 *
 *----------------------------------------------------------------------
 */

static void
EntrySelChanged(entryPtr)
    register Entry *entryPtr;		/* Entry that lost the selection. */
{
    int oldFirst, oldLast;

    oldFirst = entryPtr->selectFirst;
    oldLast = entryPtr->selectLast;
    entryPtr->selectFirst = entryPtr->selectLast = -1;
    
    if (oldFirst >= 0) {
	EntryEraseCaret(entryPtr);
	EntryRedisplay(entryPtr, oldFirst, oldLast);
	EntryDrawCaret(entryPtr);
    }
}

/*
 *----------------------------------------------------------------------
 *
 * EntryDrawCaret --
 *
 *	Display the caret in an entry window.  This procedure is
 *	typically called after anything happened that might have
 *	caused the caret to be erased.  If it was erased, then
 *	this procedure redraws it.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	The caret is redrawn, unless it's already visible.
 *
 *----------------------------------------------------------------------
 */

void
EntryDrawCaret(entryPtr)
    register Entry *entryPtr;	/* Where to draw caret. */
{
    char savedChar;

    if ((entryPtr->flags & (CARET_OFF|FOCUS_WINDOW))
	    != (CARET_OFF|FOCUS_WINDOW)) {
	return;
    }
    entryPtr->flags &= ~CARET_OFF;

    /*
     * Compute where to draw the caret.
     */
    
    savedChar = entryPtr->text[entryPtr->caret];
    entryPtr->text[entryPtr->caret] = 0;
    entryPtr->caretX = XStringWidth(entryPtr->text, entryPtr->fontPtr, 0, 0)
    	+ entryPtr->x - caret_x_hot;
    entryPtr->caretY = entryPtr->y
	    + (entryPtr->fontPtr->height - entryPtr->fontPtr->baseline)
	    - caret_y_hot;
    entryPtr->text[entryPtr->caret] = savedChar;

    /*
     * White out a mask area, then blacken the caret area.
     */
    
    XPixFill(entryPtr->w, entryPtr->caretX, entryPtr->caretY, caret_width,
	    caret_height, entryPtr->background, caretMaskBitmap, GXcopy,
	    AllPlanes);
    XPixFill(entryPtr->w, entryPtr->caretX, entryPtr->caretY, caret_width,
	    caret_height, entryPtr->foreground, caretBitmap, GXcopy,
	    AllPlanes);
}

/*
 *----------------------------------------------------------------------
 *
 * EntryEraseCaret --
 *
 *	Erase the caret from the given entry.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	The caret is no longer displayed in entryPtr's window. If
 *	it already wasn't displayed, then nothing additional
 *	happens.
 *
 *----------------------------------------------------------------------
 */

void
EntryEraseCaret(entryPtr)
    register Entry *entryPtr;
{
    int first;

    if (entryPtr->flags & CARET_OFF) {
	return;
    }
    entryPtr->flags |= CARET_OFF;

    /*
     * Erase the area of the caret.
     */
    
    XPixSet(entryPtr->w, entryPtr->caretX, entryPtr->caretY,
	    caret_width, caret_height, entryPtr->background);

    /*
     * Display the characters on either side of the caret.  The
     * first and last positions in the text have to be handled
     * specially.
     */
    
    if ((entryPtr->caret == 0) && (entryPtr->label != NULL)) {
	XText(entryPtr->w, LEFT_MARGIN, entryPtr->y, entryPtr->label,
		String_Length(entryPtr->label), entryPtr->fontPtr->id,
		entryPtr->foreground, entryPtr->background);
    }
    first = entryPtr->caret-1;
    if (first < 0) {
	first = 0;
    }
    if (entryPtr->text[entryPtr->caret] == 0) {
	entryPtr->text[entryPtr->caret] = ' ';
	EntryRedisplay(entryPtr, first, entryPtr->caret);
	entryPtr->text[entryPtr->caret] = 0;
    } else {
	EntryRedisplay(entryPtr, first, entryPtr->caret);
    }
}

/*
 *----------------------------------------------------------------------
 *
 * EntryInit --
 *
 *	Initialize this module.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	Bitmaps and cursors get allocated.
 *
 *----------------------------------------------------------------------
 */

static void
EntryInit()
{
    static Boolean initialized = FALSE;

    if (initialized) {
	return;
    }
    initialized = TRUE;

    caretBitmap = XStoreBitmap(caret_width, caret_height, caret_bits);
    caretMaskBitmap = XStoreBitmap(caretMask_width, caretMask_height,
	    caretMask_bits);
    cursor = XCreateCursor(ptr_width, ptr_height, ptr_bits, ptr_bits,
	    ptr_x_hot, ptr_y_hot, BlackPixel, WhitePixel, GXcopy);
    entryTable = XCreateAssocTable(8);
    
    if ((cursor == 0) || (caretBitmap == 0) || (caretMaskBitmap == 0)
	    || (entryTable == NULL)) {
	Sx_Panic("EntryMake:  couldn't initialize bitmaps and/or hash table.");
    }
}

/*
 *----------------------------------------------------------------------
 *
 * EntryDestroyProc --
 *
 *	This procedure is called by the Sx dispatcher just after
 *	an entry window has been deleted.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	The internal data structures associated with the entry
 *	are deleted.
 *
 *----------------------------------------------------------------------
 */

void
EntryDestroyProc(entryPtr)
    Entry *entryPtr;			/* Entry whose window died. */
{
    if (entryPtr->selectFirst >= 0) {
	entryPtr->selectFirst = entryPtr->selectLast = -1;
	Sx_SelectionClear();
    }
    XDeleteAssoc(entryTable, entryPtr->w);
    if (entryPtr->label != NULL) {
	Mem_Free((Address) entryPtr->label);
    }
    Mem_Free((Address) entryPtr);
}
