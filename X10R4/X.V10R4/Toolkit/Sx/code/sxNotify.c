/*
 *	$Source: /u1/Sx.new/code/RCS/sxNotify.c,v $
 *	$Header: sxNotify.c,v 1.1 86/12/03 16:10:26 swick Exp $
 */

#ifndef lint
static char *rcsid_sxNotify_c = "$Header: sxNotify.c,v 1.1 86/12/03 16:10:26 swick Exp $";
#endif	lint

/* 
 * sxNotify.c --
 *
 *	This file provides a notification facility, whereby users
 *	are notified of various conditions and given a choice of
 *	options.  Notifiers are built using the facilities of
 *	the X window manager and the Sx dispatcher.
 *
 * Copyright (C) 1986 Regents of the University of California.
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting
 * documentation.  The University of California makes no
 * representations about the suitability of this software for
 * any purpose.  It is provided "as is" without express or
 * implied warranty.
 */

#ifndef lint
static char rcsid[] = "$Header: sxNotify.c,v 1.1 86/12/03 16:10:26 swick Exp $ SPRITE (Berkeley)";
#endif not lint

#include <X/Xlib.h>
#include "sprite.h"
#include "char.h"
#include "io.h"
#include "string.h"
#include "sx.h"
#include "sxInt.h"
#include "varg.h"

/*
 * A notifier is a window that pops up and displays a message and
 * a set of options.  The message is displayed at the top of the
 * window, with the options spaced across the bottom of the window.
 * Each option is drawn in a box, much like a button.  The user
 * reads the message and clicks a mouse button over one of the
 * options.  This file does cursor tracking to see when the mouse
 * is over one of the options.
 */

typedef struct {
    char *text;			/* Text to display for option. */
    int x, y;			/* Location of option within notifier. */
    int width, height;		/* Dimensions of option, including border. */
} Option;

/*
 * Parameters controlling layout of notifier:
 *
 * SPACING:		spacing between lines of message.
 * OPTION_MARGIN	space around top and bottom of text in option buttons.
 * OPTION_BORDER:	thickness of borders around options.
 * WINDOW_BORDER:	thickness of border around notifier itself.
 */

#define SPACING 0
#define OPTION_MARGIN 2
#define OPTION_BORDER 1
#define WINDOW_BORDER 2

/*
 * Cursor for notifier windows:
 */

#include "cursors/dot"
#include "cursors/dotMask"
static Cursor cursor;

/*
 * Bitmap used to paint a fancy header area around the options.
 */

#include "notify.bits"
static Pixmap header;

/*
 * Display dimensions:
 */

static int displayWidth, displayHeight;

static Boolean init = FALSE;

/*
 * Forward declarations:
 */

extern void	NotifierInit();
extern char *	EndOfLine();
extern void	OptionRedisplay();

/*
 *----------------------------------------------------------------------
 *
 * Sx_Notify --
 *
 *	This procedure creates a notifier window displaying text and
 *	all the option strings.  It waits until one of the option
 *	strings has been buttoned with the mouse, then returns.  This
 *	procedure grabs the server, which prevents any other window
 *	activity until the user has selected an option.
 *
 * Results:
 *	The return value is the index of the option that was
 *	buttoned, where 0 corresponds to the first option in
 *	the list.
 *
 * Side effects:
 *	A window is created, then deleted.
 *
 *----------------------------------------------------------------------
 */
    /* VARARGS7 */
int
Sx_Notify(parent, x, y, width, message, fontPtr, center, Varg_Args)
    Window parent;		/* Window in whose coordinates x and y are
				 * specified below. */
    int x, y;			/* Location of UL corner of notifier, in
				 * coords of parent. */
    int width;			/* Width of notifier, in pixels.  If 0,
				 * then this procedure picks a width that
				 * makes the notifier look pretty. */
    char *message;		/* Message to be displayed in notifier.
				 * May contain newlines to break into
				 * lines explicitly.  This procedure will
				 * automatically break lines at spaces
				 * to make sure that no line is longer
				 * than width pixels.  */
    FontInfo *fontPtr;		/* Describes font in which to display text
				 * in the notifier.  If NULL, then a default
				 * font will be chosen. */
    Boolean center;		/* TRUE means center each line in the window,
				 * FALSE means left-justify lines. */
    Varg_Decl;			/* Two or more options, each a "char *".
				 * The last option must be NULL. */
{
#define MAXOPTIONS 16
    register Option *o;
    Option options[MAXOPTIONS];	/* Space for options. */
    int nOptions;		/* Actual number of options. */
    int nLines;			/* Number of lines in notifier, not including
				 * options. */
    int minOptionLength;	/* Total width of all options, assuming
				 * optionPad is 0. */
    int optionPad;		/* Amount of extra space to leave between
				 * options (in addition to "space"). */
    int optionY;		/* Y-coord of top of option boxes. */
    int optionHeight;		/* Size of options. */
    int currentOption;		/* Option that the mouse is over, or -1. */
    int space;			/* Width of average character in font.  Used
				 * to determine how much space to leave around
				 * edges of message and other places. */
    WindowInfo info;		/* Holds dimensions of parent. */
    Pixmap savedPixmap;		/* If non-zero, holds image of what was
				 * underneath notifier window (to save
				 * redrawing). */
    Window w;
    int outerWidth, outerHeight;/* Outside dimensions of w, including space
				 * for shadowed box. */
    int i, trialWidth, inc, height, tmp;
    Varg_List args;
    char *p;

    if (!init) {
	NotifierInit();
    }

    if (fontPtr == NULL) {
	fontPtr = Sx_GetDefaultFont();
    }

    /*
     * Fill in most of the info in the options array, and compute
     * how much screen space is needed for all the options if they're
     * placed side-to-side.  If I get to choose how wide the window
     * is, this will be a lower bound.
     */

    minOptionLength = 0;
    optionHeight = fontPtr->height + 2*OPTION_BORDER + 2*OPTION_MARGIN;
    space = fontPtr->width;
    Varg_Start(args);
    for (nOptions = 0; nOptions < MAXOPTIONS; nOptions++) {
	o = &options[nOptions];
	o->text = Varg_Next(args, char *);
	if (o->text == NULL) {
	    break;
	}
	o->width = XStringWidth(o->text, fontPtr, 0, 0) + 2*space
		+ 2*OPTION_BORDER;
	o->height = optionHeight;
	minOptionLength += o->width;
    }
    minOptionLength += (nOptions+1) * space;

    /*
     * Choose the width, if the caller didn't get one.  Start with
     * a wide width, and narrow it down until the window is about
     * 1.5 times as wide as it is high (within 10%).  If the caller
     * already chose a width, then just compute the height it needs.
     */
    
    optionY = optionHeight/2 + SHADOW_TOP;
    if (width <= 0) {
	trialWidth = DisplayWidth()/2;
    } else {
	trialWidth = width;
    }
    for (inc = trialWidth/2; ; inc /= 2) {
	p = message;
	for (nLines = 0, p = message; *p != 0; nLines++) {
	    p = EndOfLine(p, fontPtr, trialWidth - 2*space, (int *) NULL);
	    while (*p == ' ') {
		p++;
	    }
	    if (*p == '\n') {
		p++;
	    }
	}
	height = 2*optionHeight + nLines*(fontPtr->height + SPACING)
		+ 2*space;
	if (width > 0) {
	    break;
	}
	if (inc < 5) {
	    width = trialWidth;
	    break;
	}
	if ((height + height/2) > trialWidth) {
	    trialWidth += inc;
	} else {
	    trialWidth -= inc;
	}
	if (trialWidth < minOptionLength) {
	    trialWidth = minOptionLength;
	}
    }
    outerWidth = width + SHADOW_LEFT + SHADOW_RIGHT;
    outerHeight = height + SHADOW_TOP + SHADOW_BOTTOM;

    /*
     * Fill in the rest of the option information, distributing the
     * extra space evenly between the options if there's any to spare.
     */

    optionPad = (width - minOptionLength)/(nOptions+1);
    if (optionPad < 0) {
	optionPad = 0;
    }
    tmp = (width - minOptionLength - (nOptions+1)*optionPad)/2
	    + optionPad + space + SHADOW_LEFT;
    for (i = 0; i < nOptions; i++) {
	o = &options[i];
	o->x = tmp;
	o->y = optionY;
	tmp += o->width + optionPad + space;
    }

    /*
     * Compute the final location of the notifier window, which will
     * be in RootWindow.  If the specified parent wasn't RootWindow,
     * the coordinate transformation will also be needed.  If the
     * caller didn't give a location, center the notifier in its
     * parent.
     */
    
    XQueryWindow(parent, &info);
    if (x < 0) {
	x = (info.width - outerWidth)/2;
    }
    if (y < 0) {
	y = (info.height - outerHeight)/2;
    }
    if (parent != RootWindow) {
	int xOffset, yOffset;
	Window dummy;

	XInterpretLocator(parent, &xOffset, &yOffset, &dummy, 0);
	x -= xOffset;
	y -= yOffset;
    }
    displayWidth = DisplayWidth();
    displayHeight = DisplayHeight();
    if ((x + outerWidth) > displayWidth) {
	x = displayWidth - outerWidth;
    }
    if ((y + outerHeight) > displayHeight) {
	y = displayHeight - outerHeight;
    }
    if (x < 0) {
	x = 0;
    }
    if (y < 0) {
	y = 0;
    }

    /*
     * Create a window for the notifier.
     */
    
    w = XCreateWindow(RootWindow, x, y, outerWidth, outerHeight, 0,
	    BlackPixmap, WhitePixmap);
    if (w == 0) {
	Sx_Panic("Sx_Notify:  couldn't create window.");
    }
    XDefineCursor(w, cursor);
    XSelectInput(w, ButtonPressed|MouseMoved|ExposeWindow);

    /*
     * Grab the server, then save the information that's about to get
     * overlayed by the notifier window, if the entire area is on-screen.
     * Then map the window.
     */
   
    Sx_GrabMouse(w, cursor, ButtonPressed|MouseMoved|ExposeWindow);
    XGrabServer();
    if (((x + outerWidth) <= displayWidth)
	    && ((y + outerHeight) <= displayHeight)) {
	savedPixmap = XPixmapSave(RootWindow, x, y, outerWidth, outerHeight);
    } else {
	savedPixmap = 0;
    }
    XMapWindow(w);

    /*
     * Process events until a button is pushed over an option.
     */
    
    currentOption = -1;
    while (TRUE) {
	XMouseMovedEvent event;

	XWindowEvent(w, MouseMoved|ButtonPressed|ExposeWindow, &event);
	switch ((int) event.type) {

	    case MouseMoved:
		
		/*
		 * See if the mouse is over an option.  If so, display
		 * the option in reverse video.
		 */
		
		if ((event.y >= optionY) && (event.y <=
			optionY + options[0].height)) {
		    for (i = 0; i < nOptions; i++) {
			o = &options[i];
			if ((event.x < o->x) || (event.x > o->x + o->width)) {
			    continue;
			}
			goto newOption;
		    }
		}
		i = -1;
		newOption:
		if (i != currentOption) {
		    if (currentOption != -1) {
			OptionRedisplay(w, &options[currentOption],
				fontPtr, FALSE, space);
		    }
		    if (i != -1) {
			OptionRedisplay(w, &options[i], fontPtr, TRUE, space);
		    }
		    currentOption = i;
		}
		break;
	    
	    case ButtonPressed:
		if (currentOption >= 0) {
		    
		    /*
		     * Flash the button before returning.
		     */
		    
		    for (i = 0; i < 2; i++) {
			OptionRedisplay(w, &options[currentOption],
				fontPtr, FALSE, space);
			SxFlashWait();
			OptionRedisplay(w, &options[currentOption],
				fontPtr, TRUE, space);
			SxFlashWait();
		    }
		    if (savedPixmap != 0) {
			XPixmapPut(w, 0, 0, 0, 0, outerWidth, outerHeight,
				savedPixmap, GXcopy, AllPlanes);
			XUnmapTransparent(w);
			XFreePixmap(savedPixmap);
		    }
		    XUngrabServer();
		    Sx_UngrabMouse();
		    XDestroyWindow(w);
		    XFlush();
		    return currentOption;
		}
		break;
	    
	    case ExposeWindow:

		/*
		 * Clear the background and draw the shadow.
		 */

		XPixSet(w, 0, 0, outerWidth, outerHeight, WhitePixel);
		SxDrawShadow(w, 0, 0, width, height, BlackPixel, savedPixmap);

		/*
		 * Display the message, one line at a time.
		 */
    
		p = message;
		for (y = 2*optionHeight + space + SHADOW_TOP; ;
			y += SPACING + fontPtr->height) {
		    char *next;
		    int extraSpace;

		    if (*p == 0) {
			break;
		    }
		    next = EndOfLine(p, fontPtr, width - 2*space,
			    &extraSpace);
		    if (center) {
			extraSpace /= 2;
		    } else {
			extraSpace = 0;
		    }
		    if (next != p) {
			XText(w, extraSpace + space + SHADOW_LEFT, y, p,
				next-p, fontPtr->id, BlackPixel, WhitePixel);
		    }
		    
		    /*
		     * For lines not terminated by newline, eat up any white
		     * space at the beginning of the next line.  For lines
		     * terminated by newline, treat the space as significant.
		     */
		    
		    p = next;
		    while (*p == ' ') {
			p++;
		    }
		    if (*p == '\n') {
			p++;
		    }
		}
		
		/*
		 * Display the options.
		 */
		
		XTileSet(w, SHADOW_LEFT, SHADOW_TOP, width,
			2*optionHeight, header);
		XLine(w, SHADOW_LEFT, 2*optionHeight + SHADOW_TOP - 1,
			width, 2*optionHeight + SHADOW_TOP - 1, 1, 1,
			BlackPixel, GXcopy, AllPlanes);
		for (i = 0; i < nOptions; i++) {
		    OptionRedisplay(w, &options[i], fontPtr,
			    i == currentOption, space);
		}
		break;
	}
    }
}

/*
 *----------------------------------------------------------------------
 *
 * Sx_Panic --
 *
 *	This procedure is called when some totally unexpected and
 *	irremediable error has occurred.  It displays a message,
 *	waits for the user to acknowledge it, and terminates the
 *	process.
 *
 * Results:
 *	None.  Never returns.
 *
 * Side effects:
 *	The process dies a terrible, agonizing death.
 *
 *----------------------------------------------------------------------
 */

void
Sx_Panic(msg)
    char *msg;			/* Message explaining what happened. */
{
    char realMsg[2000];
    static Boolean alreadyPanicked = FALSE;

    if (!alreadyPanicked) {
	alreadyPanicked = TRUE;
	Io_PrintString(realMsg, "PANIC!!\n\n%.1950s", msg);
	(void) Sx_Notify(RootWindow, -1, -1, 0, realMsg, (FontInfo *) NULL,
		TRUE, "Kill Process", NULL);
    }
    exit(1);
}

/*
 *----------------------------------------------------------------------
 *
 * EndOfLine --
 *
 *	This procedure is used to figure out how much stuff will fit
 *	on one line of a notifier.  Assuming that the first character
 *	of string is to be at the beginning of a line, and that the
 *	line may be at most width pixels wide using fontPtr to display the
 *	text, this procedure determines	how much stuff will fit on the
 *	line.  The end of the line occurs either when a newline character
 *	appears, or at the end of a word if the next word would extend
 *	the line wider than width.  If a single word would more than fill
 *	the entire line, then the word is broken in the middle.
 *
 * Results:
 *	The return value is the address of the character just after the
 *	last one that fits on the line.  It can be a newline character,
 *	a space character, a NULL character (end of string), or even
 *	a regular character if a single word overflows the line.  If
 *	extraPtr is non-NULL, *extraPtr gets filled in with the number
 *	of unused pixels in this line.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

static char *
EndOfLine(string, fontPtr, width, extraPtr)
    register char *string;	/* String to be displayed. */
    register FontInfo *fontPtr;	/* Describes font in which string is to
				 * be displayed. */
    int width;			/* Maximum number of pixels across line. */
    int *extraPtr;		/* If non-NULL, fill in the value pointed
				 * to with the number of pixels leftover
				 * in the line, or 0 if this line is
				 * width pixels wide. */
{
    char *lastWordBreak = NULL;
    int lastWidth, charSize;
    register char c;

    for (c = *string; c != 0; string++, c = *string) {
	if (Char_IsSpace(c)) {
	    lastWordBreak = string;
	    lastWidth = width;
	}
	if (c == '\n') {
	    goto done;
	}
	if ((c >= fontPtr->firstchar) && (c <= fontPtr->lastchar)) {
	    if (fontPtr->fixedwidth) {
		charSize = fontPtr->width;
	    } else {
		charSize = fontPtr->widths[c-fontPtr->firstchar];
	    }
	}
	width -= charSize;
	if (width < 0) {
	    if (lastWordBreak != NULL) {
		goto done;
	    }
	    width += charSize;
	    break;
	}
    }
    lastWordBreak = string;
    lastWidth = width;

    done:
    if (extraPtr != NULL) {
	*extraPtr = lastWidth;
    }
    return lastWordBreak;
}

/*
 *----------------------------------------------------------------------
 *
 * OptionRedisplay --
 *
 *	This procedure redraws a single option button.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	Information gets drawn on the screen.
 *
 *----------------------------------------------------------------------
 */

static void
OptionRedisplay(w, optionPtr, fontPtr, reverse, space)
    Window w;			/* Window in which to draw option. */
    register Option *optionPtr;	/* Option to redraw. */
    FontInfo *fontPtr;		/* Font in which to draw. */
    Boolean reverse;		/* TRUE means draw option as white letters
				 * on black background.  FALSE means
				 * reverse. */
    int space;			/* Blank space to leave on each end of text. */
{
    if (reverse) {
	XPixSet(w, optionPtr->x, optionPtr->y, optionPtr->width,
		optionPtr->height, BlackPixel);
	XText(w, optionPtr->x + OPTION_BORDER + space,
		optionPtr->y + OPTION_BORDER + OPTION_MARGIN,
		optionPtr->text, String_Length(optionPtr->text), fontPtr->id,
		WhitePixel, BlackPixel);
    } else {
	XPixSet(w, optionPtr->x, optionPtr->y, optionPtr->width,
		optionPtr->height, WhitePixel);
	XPixSet(w, optionPtr->x, optionPtr->y, optionPtr->width,
		OPTION_BORDER, BlackPixel);
	XPixSet(w, optionPtr->x, optionPtr->y, OPTION_BORDER,
		optionPtr->height, BlackPixel);
	XPixSet(w, optionPtr->x + optionPtr->width - OPTION_BORDER,
		optionPtr->y, OPTION_BORDER, optionPtr->height, BlackPixel);
	XPixSet(w, optionPtr->x, optionPtr->y + optionPtr->height
		- OPTION_BORDER, optionPtr->width, OPTION_BORDER, BlackPixel);
	XText(w, optionPtr->x + OPTION_BORDER + space,
		optionPtr->y + OPTION_BORDER + OPTION_MARGIN,
		optionPtr->text, String_Length(optionPtr->text),
		fontPtr->id, BlackPixel, WhitePixel);
    }
}

/*
 *----------------------------------------------------------------------
 *
 * NotifierInit --
 *
 *	This procedure is called once only to initialize shared
 *	variables for the module.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	Random initializations get performed.  See the code for
 *	details.  If there's any problem, the procedure panics.
 *
 *----------------------------------------------------------------------
 */

static void
NotifierInit()
{
    Bitmap tmp;
    int width, height;

    cursor = XCreateCursor(dot_width, dot_height, dot_bits,
	    dotMask_bits, dot_x_hot, dot_y_hot,
	    BlackPixel, WhitePixel, GXcopy);

    header = 0;
    XQueryTileShape(notify_width, notify_height, &width, &height);
    if ((width <= 16) && (height <= 16)) {
	tmp = XStoreBitmap(width, height, notify_bits);
	if (tmp != 0) {
	    header = XMakePixmap(tmp, BlackPixel, WhitePixel);
	    XFreeBitmap(tmp);
	}
    }
    if (header == 0) {
	header = WhitePixmap;
    }

    displayWidth = DisplayWidth();
    displayHeight = DisplayHeight();

    if (cursor == 0) {
	Sx_Panic("Sx_Notify: couldn't initialize cursor.");
    }

    init = TRUE;
}
