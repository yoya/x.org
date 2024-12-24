/*
 *	$Source: /u1/Sx.new/code/RCS/sxScrollbar.c,v $
 *	$Header: sxScrollbar.c,v 1.1 86/12/03 16:10:45 swick Exp $
 */

#ifndef lint
static char *rcsid_sxScrollbar_c = "$Header: sxScrollbar.c,v 1.1 86/12/03 16:10:45 swick Exp $";
#endif	lint

/* 
 * sxScrollbar.c --
 *
 *	This module implements scrollbars on top of the X window
 *	package.
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
static char rcsid[] = "$Header: sxScrollbar.c,v 1.1 86/12/03 16:10:45 swick Exp $ SPRITE (Berkeley)";
#endif not lint

#include <sys/types.h>
#include <X/Xlib.h>
#include "sprite.h"
#include "mem.h"
#include "sx.h"

/*
 * The structure type below is used to hold information about a scrollbar:
 */

typedef struct Scrollbar {
    Window window;		/* Window corresponding to scrollbar. */
    int foreground, background, elevatorColor;
				/* Colors to use when displaying scrollbar. */
    int width, height;		/* Dimensions of scrollbar window. */
    float top, bottom;		/* Range of view.  Each number is a fraction
				 * between 0 and 1.  Top gives the position
				 * of the top (or left) edge of the view,
				 * as a fraction of the height (or width)
				 * of the object being displayed.  Bottom
				 * gives the same information for the bottom
				 * (or right) edge of the view. */
    int flags;			/* Miscellaneous flag values:  see below. */
    void (*proc)();		/* Procedure to call when scrollbar is
				 * buttoned.
				 */
    ClientData clientData;	/* Info to pass to proc. */
} Scrollbar;

/*
 * Scrollbar flags:
 *
 * VERTICAL:		1 means scrollbar has vertical orientation, 0
 *			means it has horizontal orientation.
 */

#define VERTICAL 1

/*
 * Widths of various parts of scrollbars, in pixels:
 */

#define WIDTH 13
#define ELEVATOR_MARGIN 3

/*
 * Other information used to display scrollbars:
 */

#define BGSIZE 16
static short bgBits[] = {	/* Bit pattern for background. */
    0x0, 0x2222, 0x0, 0x8888,
    0x0, 0x2222, 0x0, 0x8888,
    0x0, 0x2222, 0x0, 0x8888,
    0x0, 0x2222, 0x0, 0x8888
};
static int bgWidth, bgHeight;	/* Dimensions to use for tile. Either dimension
				 * may be up to BGSIZE. */

/*
 * The cursors used by the scrollbar package are defined in separate files
 * so they can be edited with the "bitmap" program.  See the procedure
 * ScrollButtonProc for usage.
 */

#include "cursors/left"
#include "cursors/leftMask"
#include "cursors/right"
#include "cursors/rightMask"
#include "cursors/up"
#include "cursors/upMask"
#include "cursors/down"
#include "cursors/downMask"
#include "cursors/vert"
#include "cursors/vertMask"
#include "cursors/horiz"
#include "cursors/horizMask"
#include "cursors/thumbH"
#include "cursors/thumbHMask"
#include "cursors/thumbV"
#include "cursors/thumbVMask"

static Cursor cursorUp, cursorDown, cursorLeft, cursorRight;
static Cursor cursorHoriz, cursorVert, cursorThumbV, cursorThumbH;

static Boolean init = FALSE;

/*
 * The association table below is used to map from X window ids
 * to our internal Scrollbar structures.
 */

static XAssocTable *scrollbarTable = NULL;
extern XAssocTable *XCreateAssocTable();

/*
 * Forward references:
 */

extern void	ScrollbarInit();
extern void	ScrollButtonProc();
extern void	ScrollComputeLoc();
extern void	ScrollDestroyProc();
extern void	ScrollExposeProc();
extern void	ScrollRedisplay();

/*
 *----------------------------------------------------------------------
 *
 * Sx_ScrollbarMake --
 *
 *	Given an existing window, this procedure sets things up so that
 *	it will behave like a scrollbar.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	Data structures are set up so that window will behave like a
 *	scrollbar:  whenever a user clicks a button in the scrollbar,
 *	proc will be invoked in the following way:
 *
 *	void
 *	proc(clientData, distance, units, window)
 *	    ClientData clientData;
 *	    float distance;
 *	    int units;
 *	    Window window;
 *	{
 *	}
 *
 *	The clientData parameter is the same as the parameter by the
 *	same name passed to this routine.  It has no meaning to the
 *	scrollbar code, but is typically used to point to information
 *	relevant to the client.  Distance and units indicate the scroll
 *	action requested by the user, and window of the window parameter
 *	passed to this procedure.  If units is SX_SCROLL_ABSOLUTE,
 *	then distance gives a value between 0 and 1 indicating the
 *	desired positioning of the window's view:  0 means at the very
 *	top or right or beginning and 1 means at the very bottom or
 *	left or end.  Otherwise, units is SX_SCROLL_PAGES, and distance
 *	gives a value between -1 and 1 indicating how much the window
 *	should be scrolled up or down:  1 means the window should be
 *	scrolled up (left)  by its size and -1 means the window should
 *	be scrolled down (right) by its size.
 *
 *	The scrollbar module does not actually adjust the position of
 *	the scrollbar elevator:  proc must do that (if it wishes) by
 *	calling Sx_ScrollbarSetRange.  If proc doesn't call
 *	Sx_ScrollbarSetRange, it means that it wishes to override the
 *	user's request for a change in position and not change anything.
 *
 *----------------------------------------------------------------------
 */

void
Sx_ScrollbarMake(window, vertical, foreground, background,
	elevatorColor, proc, clientData)
    Window window;		/* Window to make into scrollbar.  If this
				 * window is already a scrollbar, then the
				 * call just changes the scrollbar's
				 * parameters (and redisplays it). If the
				 * window isn't already a scrollbar, then
				 * it should not be mapped (this proc won't
				 * redisplay it). */
    Boolean vertical;		/* TRUE means use vertical scrollbar
				 * orientation, FALSE means horizontal. */
    int foreground;		/* Pixel value to use for the dark part of
				 * the scrollbar background.  BlackPixel
				 * is typical. */
    int background;		/* Pixel value to use for the light part of
				 * the scrollbar background.  WhitePixel
				 * is typical. */
    int elevatorColor;		/* Pixel value to use for the elevator that
				 * shows current scroll position.  WhitePixel
				 * is typical. */
    void (*proc)();		/* Procedure to call when scrollbar is buttoned
				 * by user. */
    ClientData clientData;	/* Arbitrary data to pass to proc. */
{
    register Scrollbar *sbPtr;
    Bitmap tmp;
    Pixmap tile;

    if (!init) {
	ScrollbarInit();
    }

    /*
     * If the display can accomodate tiles that fit in our background
     * bitmap, then make a background pixmap from it.  Otherwise, use a
     * solid background bitmap in the light color.
     */

    if ((bgWidth <= BGSIZE) && (bgHeight <= BGSIZE)) {
	tmp = XStoreBitmap(bgWidth, bgHeight, bgBits);
	if (tmp == 0) {
	    error:
	    Sx_Panic("Sx_ScrollbarMake:  couldn't create background tile");
	}
	tile = XMakePixmap(tmp, foreground, background);
	XFreeBitmap(tmp);
    } else {
	tile = XMakeTile(background);
    }
    if (tile == NULL) {
	goto error;
    }
    XChangeBackground(window, tile);
    XFreePixmap(tile);

    /*
     * See if this window is already a scrollbar.  If not, create a new
     * one.
     */
    
    sbPtr = (Scrollbar *) XLookUpAssoc(scrollbarTable, window);
    if (sbPtr == NULL) {
	sbPtr = (Scrollbar *) Mem_Alloc(sizeof(Scrollbar));
	sbPtr->window = window;
	sbPtr->height = sbPtr->width = 0;
	sbPtr->top = 0.0;
	sbPtr->bottom = 1.0;
	(void) Sx_HandlerCreate(window, ExposeWindow, ScrollExposeProc,
		(ClientData) sbPtr);
	(void) Sx_HandlerCreate(window, ButtonPressed|ButtonReleased,
		ScrollButtonProc, (ClientData) sbPtr);
	(void) Sx_HandlerCreate(window, KeyPressed|KeyReleased
		|EnterWindow|LeaveWindow, Sx_NullProc, (ClientData) NULL);
	(void) Sx_HandlerCreate(window, SX_DESTROYED, ScrollDestroyProc,
		(ClientData) sbPtr);
	XMakeAssoc(scrollbarTable, window, (caddr_t) sbPtr);
    }

    /*
     * Complete the initialization.
     */

    sbPtr->foreground = foreground;
    sbPtr->background = background;
    sbPtr->elevatorColor = elevatorColor;
    if (vertical) {
	sbPtr->flags = VERTICAL;
	XDefineCursor(window, cursorVert);
    } else {
	sbPtr->flags = 0;
	XDefineCursor(window, cursorHoriz);
    }
    sbPtr->proc = proc;
    sbPtr->clientData = clientData;

    if ((sbPtr->width > 0) && (sbPtr->height > 0)) {
	ScrollRedisplay(sbPtr);
    }
}

/*
 *----------------------------------------------------------------------
 *
 * Sx_ScrollbarCreate --
 *
 *	Create a new scrollbar window.  Similar to Sx_ScrollbarMake
 *	except that it creates the window and arranges for the packer
 *	to manage its geometry.
 *
 * Results:
 *	The return value is an X window id for a new scrollbar, which
 *	will always appear on the side'th side of parent.
 *
 * Side effects:
 *	Data structures are set up so that window will behave like a
 *	scrollbar:  whenever a user clicks a button in the scrollbar,
 *	proc will be invoked in the same way as for Sx_MakeScrollbar.
 *
 *----------------------------------------------------------------------
 */

Window
Sx_ScrollbarCreate(parent, side, foreground, background, elevatorColor,
	proc, clientData)
    Window parent;		/* Parent window in which to create new
				 * scrollbar. */
    Sx_Side side;		/* Which side of parent scrollbar should
				 * be displayed on. */
    int foreground;		/* Pixel value to use for the dark part of
				 * the scrollbar background.  BlackPixel
				 * is typical. */
    int background;		/* Pixel value to use for the light part of
				 * the scrollbar background.  WhitePixel
				 * is typical. */
    int elevatorColor;		/* Pixel value to use for the elevator that
				 * shows current scroll position.  WhitePixel
				 * is typical. */
    void (*proc)();		/* Procedure to call when scrollbar is buttoned
				 * by user. */
    ClientData clientData;	/* Arbitrary data to pass to proc. */
{
    Window w;
    Boolean vertical;
    Pixmap borderPixmap;

    if (foreground == BlackPixel) {
	borderPixmap = BlackPixmap;
    } else if (foreground == WhitePixel) {
	borderPixmap = WhitePixmap;
    } else {
	borderPixmap = XMakeTile(foreground);
	if (borderPixmap == NULL) {
	    Sx_Panic("Sx_ScrollbarCreate:  couldn't create border pixmap.");
	}
    }
    w = Sx_CreatePacked(parent, side, WIDTH, 1, (Window) 0, (Window) 0,
	    borderPixmap, WhitePixmap);
    if ((borderPixmap != BlackPixmap) && (borderPixmap != WhitePixmap)) {
	XFreePixmap(borderPixmap);
    }
    vertical = ((side == SX_LEFT) || (side == SX_RIGHT));
    Sx_ScrollbarMake(w, vertical, foreground, background, elevatorColor,
	    proc, clientData);
    return w;
}

/*
 *----------------------------------------------------------------------
 *
 * Sx_ScrollbarSetRange --
 *
 *	This procedure is used to indicate the position of the elevator
 *	in a scrollbar.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	The elevator will be redisplayed to indicate the fraction
 *	of the object or file that is currently visible in the window.
 *	Top and bottom indicate this fraction.
 *
 *----------------------------------------------------------------------
 */

void
Sx_ScrollbarSetRange(window, top, bottom)
    Window window;		/* X id for window containing scrollbar. */
    float top;			/* The position of the top (or left) of the
				 * window's view, relative to the overall
				 * size of the file or object being
				 * displayed.  Must be between 0 and 1.
				 * 0 means the top or left of the window is
				 * at the beginning or top or left of the
				 * file or object, and 1 means it's at the
				 * bottom or right of the object. */
    float bottom;		/* Similar to top, but indicates where the
				 * bottom or right side of the view is
				 * located. */
{
    register Scrollbar *sbPtr;
    
    /*
     * Locate our data structures for the scrollbar.
     */

    sbPtr = (Scrollbar *) XLookUpAssoc(scrollbarTable, window);
    if (sbPtr == NULL) {
	Sx_Panic("Sx_ScrollbarSetRange:  not a scrollbar window.");
    }

    if (top > 1.0) {
	top = 1.0;
    } else if (top < 0.0) {
	top = 0.0;
    }
    if (bottom > 1.0) {
	bottom = 1.0;
    } else if (bottom < top) {
	bottom = top;
    }
    if ((sbPtr->top == top) && (sbPtr->bottom == bottom)) {
	return;
    }
    sbPtr->top = top;
    sbPtr->bottom = bottom;
    ScrollRedisplay(sbPtr);
}

/*
 *----------------------------------------------------------------------
 *
 * Sx_ScrollbarGetRange --
 *
 *	Returns the current range displayed in the scrollbar.
 *
 * Results:
 *	The values pointed to by topPtr and bottomPtr are filled
 *	in with the last values of top and bottom passed to
 *	Sx_ScrollbarSetRange for this scrollbar.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

void
Sx_ScrollbarGetRange(window, topPtr, bottomPtr)
    Window window;		/* X id for window containing scrollbar. */
    float *topPtr;		/* Put top value (between 0 and 1) here. */
    float *bottomPtr;		/* Put bottom value (between 0 and 1) here. */
{
    register Scrollbar *sbPtr;
    
    /*
     * Locate our data structure for the scrollbar.
     */

    sbPtr = (Scrollbar *) XLookUpAssoc(scrollbarTable, window);
    if (sbPtr == NULL) {
	Sx_Panic("Sx_ScrollbarGetRange:  not a scrollbar window.");
    }

    *topPtr = sbPtr->top;
    *bottomPtr = sbPtr->bottom;
}

/*
 *----------------------------------------------------------------------
 *
 * Sx_ScrollbarWidth --
 *
 *	Returns a suggested width for scrollbars, in pixels.
 *
 * Results:
 *	The return value is the suggested width of scrollbars, in pixels
 *	not including the border.  This may be of use to clients in
 *	laying out windows.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

int
Sx_ScrollbarWidth()
{
    return WIDTH;
}

/*
 *----------------------------------------------------------------------
 *
 * ScrollDestroyProc --
 *
 *	Called by the Sx dispatcher whenever a scrollbar window is deleted.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	Resources associated with the scrollbar will be freed up.
 *
 *----------------------------------------------------------------------
 */

static void
ScrollDestroyProc(sbPtr)
    register Scrollbar *sbPtr;		/* Internal structure for scrollbar. */
{
    XDeleteAssoc(scrollbarTable, sbPtr->window);
    Mem_Free((char *) sbPtr);
}

/*
 *----------------------------------------------------------------------
 *
 * ScrollbarInit --
 *
 *	Performs once-only initialization for the scrollbar module.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	Generally-useful stuff, like the cursors, gets created and set up.
 *
 *----------------------------------------------------------------------
 */

static void
ScrollbarInit()
{
    cursorUp = XCreateCursor(up_width, up_height, up_bits, upMask_bits,
	    up_x_hot, up_y_hot, BlackPixel, WhitePixel, GXcopy);
    cursorDown = XCreateCursor(down_width, down_height, down_bits,
	    downMask_bits, down_x_hot, down_y_hot,
	    BlackPixel, WhitePixel, GXcopy);
    cursorLeft = XCreateCursor(left_width, left_height, left_bits,
	    leftMask_bits, left_x_hot, left_y_hot,
	    BlackPixel, WhitePixel, GXcopy);
    cursorRight = XCreateCursor(right_width, right_height, right_bits,
	    rightMask_bits, right_x_hot, right_y_hot,
	    BlackPixel, WhitePixel, GXcopy);
    cursorVert = XCreateCursor(vert_width, vert_height, vert_bits,
	    vertMask_bits, vert_x_hot, vert_y_hot,
	    BlackPixel, WhitePixel, GXcopy);
    cursorHoriz = XCreateCursor(horiz_width, horiz_height, horiz_bits,
	    horizMask_bits, horiz_x_hot, horiz_y_hot,
	    BlackPixel, WhitePixel, GXcopy);
    cursorThumbV = XCreateCursor(thumbV_width, thumbV_height, thumbV_bits,
	    thumbVMask_bits, thumbV_x_hot, thumbV_y_hot,
	    BlackPixel, WhitePixel, GXcopy);
    cursorThumbH = XCreateCursor(thumbH_width, thumbH_height, thumbH_bits,
	    thumbHMask_bits, thumbH_x_hot, thumbH_y_hot,
	    BlackPixel, WhitePixel, GXcopy);

    scrollbarTable = XCreateAssocTable(4);

    /*
     * Find out about how big background tiles have to be, and see if
     * our background pattern will work.
     */
    
    XQueryTileShape(BGSIZE, BGSIZE, &bgWidth, &bgHeight);

    if ((cursorUp == 0) || (cursorDown == 0) || (cursorLeft == 0)
	    || (cursorRight == 0) || (cursorVert == 0) || (cursorHoriz == 0)
	    || (cursorThumbV == 0) || (cursorThumbH == 0)
	    || (scrollbarTable == NULL)) {
	Sx_Panic("Sx_ScrollbarMake: couldn't initialize cursors and/or hash table.");
    }
    init = TRUE;
}

/*
 *----------------------------------------------------------------------
 *
 * ScrollRedisplay --
 *
 *	This procedure is invoked to redisplay scrollbars.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	The scrollbar gets redisplayed.
 *
 *----------------------------------------------------------------------
 */

static void
ScrollRedisplay(sbPtr)
    register Scrollbar *sbPtr;		/* Scrollbar to be redisplayed. */
{
    int x, y, width, height;

    /*
     * Compute location of elevator bar, then display it.
     */

    XClear(sbPtr->window);
    if (sbPtr->flags & VERTICAL) {
	x = ELEVATOR_MARGIN;
	y = sbPtr->height*sbPtr->top;
	width = sbPtr->width - 2*ELEVATOR_MARGIN;
	height = sbPtr->height*sbPtr->bottom;
	height -= y;
    } else {
	x = sbPtr->width*sbPtr->top;
	y = ELEVATOR_MARGIN;
	width = sbPtr->width*sbPtr->bottom;
	width -= x;
	height = sbPtr->height - 2*ELEVATOR_MARGIN;
    }

    if ((width <= 0) || (height <= 0)) {
	return;
    }

    XPixSet(sbPtr->window, x, y, width, height, sbPtr->elevatorColor);
    width -= 1;
    height -= 1;
    XLine(sbPtr->window, x, y, x+width, y, 1, 1, sbPtr->foreground,
	    GXcopy, AllPlanes);
    XLine(sbPtr->window, x, y, x, y+height, 1, 1, sbPtr->foreground,
	    GXcopy, AllPlanes);
    XLine(sbPtr->window, x+width, y, x+width, y+height, 1, 1,
	    sbPtr->foreground, GXcopy, AllPlanes);
    XLine(sbPtr->window, x, y+height, x+width, y+height, 1, 1,
	    sbPtr->foreground, GXcopy, AllPlanes);
}

/*
 *----------------------------------------------------------------------
 *
 * ScrollExposeProc --
 *
 *	Invoked by the Sx_ dispatcher for each ExposeWindow event on
 *	a scrollbar.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	The window is redisplayed.
 *
 *----------------------------------------------------------------------
 */

void
ScrollExposeProc(sbPtr, eventPtr)
    Scrollbar *sbPtr;		/* Scrollbar that was exposed. */
    XExposeEvent *eventPtr;	/* Gives new dimensions of window. */
{
    if (eventPtr->subwindow != 0) {
	return;
    }
    sbPtr->width = eventPtr->width;
    sbPtr->height = eventPtr->height;
    ScrollRedisplay(sbPtr);
}

/*
 *----------------------------------------------------------------------
 *
 * ScrollButtonProc --
 *
 *	This procedure is invoked by the Sx_ dispatcher whenever
 *	a button is pressed or released inside a scrollbar window.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	Calls client-supplied procedure if scrolling has been
 *	requested.
 *
 *----------------------------------------------------------------------
 */

static void
ScrollButtonProc(sbPtr, eventPtr)
    register Scrollbar *sbPtr;			/* Scrollbar being buttoned. */
    register XKeyOrButtonEvent *eventPtr;	/* Describes what happened. */
{
    float distance;
    int units, button;

    if (eventPtr->subwindow != NULL) {
	return;
    }

    /*
     * When a button goes down, just change the cursor to reflect the
     * operation that's in progress.
     */

    button = eventPtr->detail & 0377;
    if (eventPtr->type == ButtonPressed) {
	if (sbPtr->flags & VERTICAL) {
	    if (button == LeftButton) {
		Sx_GrabMouse(sbPtr->window, cursorUp,
		    ButtonPressed|ButtonReleased);
	    } else if (button == RightButton) {
		Sx_GrabMouse(sbPtr->window, cursorDown,
		    ButtonPressed|ButtonReleased);
	    } else {
		Sx_GrabMouse(sbPtr->window, cursorThumbV,
		    ButtonPressed|ButtonReleased);
	    }
	} else {
	    if (button == LeftButton) {
		Sx_GrabMouse(sbPtr->window, cursorLeft,
		    ButtonPressed|ButtonReleased);
	    } else if (button == RightButton) {
		Sx_GrabMouse(sbPtr->window, cursorRight,
		    ButtonPressed|ButtonReleased);
	    } else {
		Sx_GrabMouse(sbPtr->window, cursorThumbH,
		    ButtonPressed|ButtonReleased);
	    }
	}
	return;
    }

    /*
     * Button release.  Restore the cursor, and process the command.
     */

    Sx_UngrabMouse();
    if (sbPtr->flags & VERTICAL) {
	distance = eventPtr->y;
	distance /= sbPtr->height;
    } else {
	distance = eventPtr->x;
	distance /= sbPtr->width;
    }
    if (distance < 0.0) {
	distance = 0.0;
    } else if (distance > 1.0) {
	distance = 1.0;
    }

    if (button == LeftButton) {
	units = SX_SCROLL_PAGES;
    } else if (button == RightButton) {
	units = SX_SCROLL_PAGES;
	distance = -distance;
    } else {
	units = SX_SCROLL_ABSOLUTE;
    }

    (*sbPtr->proc)(sbPtr->clientData, distance, units, sbPtr->window);
    
    /*
     * It's important that we do NOTHING to the scrollbar after calling
     * the client, since the client could conceivable have deleted the
     * scrollbar, leaving us with a dangling pointer.
     */
}
