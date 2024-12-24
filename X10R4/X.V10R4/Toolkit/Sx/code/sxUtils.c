/*
 *	$Source: /u1/Sx.new/code/RCS/sxUtils.c,v $
 *	$Header: sxUtils.c,v 1.1 86/12/03 16:11:13 swick Exp $
 */

#ifndef lint
static char *rcsid_sxUtils_c = "$Header: sxUtils.c,v 1.1 86/12/03 16:11:13 swick Exp $";
#endif	lint

/* 
 * sxUtils.c --
 *
 *	This file provides routines and variables shared by all of
 *	the files in the sx module, but not used outside this module.
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
static char rcsid[] = "$Header: sxUtils.c,v 1.1 86/12/03 16:11:13 swick Exp $ SPRITE (Berkeley)";
#endif not lint

#include <X/Xlib.h>
#include <sys/time.h>
#include "sprite.h"
#include "sx.h"
#include "sxInt.h"

/*
 * The include below is for information about the shadow pattern.
 */

#include "shadow.bits"

/*
 * The default font:
 */

static FontInfo *defaultFontPtr = NULL;

/*
 *----------------------------------------------------------------------
 *
 * Sx_SetDefaultFont --
 *
 *	Specify a font to be used by default in all Sx routines.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	After this call, the font described in fontPtr will be
 *	used in all Sx routines whenever a font isn't specified
 *	explicitly.  The caller must ensure that the font
 *	information in *fontPtr never changes or becomes
 *	invalid.
 *
 *----------------------------------------------------------------------
 */

void
Sx_SetDefaultFont(fontPtr)
    FontInfo *fontPtr;		/* Describes default font. */
{
    defaultFontPtr = fontPtr;
}

/*
 *----------------------------------------------------------------------
 *
 * Sx_GetDefaultFont --
 *
 *	Provides information about the font to use when users don't
 *	care.
 *
 * Results:
 *	Returns a pointer to a FontInfo structure describing the default
 *	Sx font.  If the font couldn't be opened then NULL is returned.
 *
 * Side effects:
 *	The font gets registered with X, if it wasn't already.
 *
 *----------------------------------------------------------------------
 */

FontInfo *
Sx_GetDefaultFont()
{
    if (defaultFontPtr == NULL) {
	defaultFontPtr = XOpenFont("serif.r.11");
	if (defaultFontPtr == NULL) {
	    defaultFontPtr = XOpenFont("8x13");
	    if (defaultFontPtr == NULL) {
		Sx_Panic("Couldn't locate a default font.");
	    }
	}
    }
    return defaultFontPtr;
}

/*
 *----------------------------------------------------------------------
 *
 * Sx_DefaultHeight --
 *
 *	Returns a "reasonable" height to use for windows that will
 *	contain text in the given font.
 *
 * Results:
 *	The return value is a suggested inside dimension for windows
 *	that will display a single line of text in fontPtr.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

int
Sx_DefaultHeight(fontPtr)
    FontInfo *fontPtr;		/* Font to be used for text. */
{
    if (fontPtr == NULL) {
	fontPtr = Sx_GetDefaultFont();
    }
    return (fontPtr->height + 2);
}

/*
 *----------------------------------------------------------------------
 *
 * Sx_NullProc --
 *
 *	This procedure does absolutely nothing.  Its most common
 *	use is as a null event handler, in order to keep the events
 *	from propagating up the window hierarchy.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

void
Sx_NullProc()
{
    return;
}

/*
 *----------------------------------------------------------------------
 *
 * SxFlashWait --
 *
 *	This procedure flushes X output and waits the "correct"
 *	amount of time for a button or menu entry to flash on or
 *	off.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	Time goes by.  Also, X output gets flushed.
 *
 *----------------------------------------------------------------------
 */

void
SxFlashWait()
{
    static struct timeval flashPeriod = {0, 50000}; /* 50 ms */
    extern int select();

    XFlush();
    (void) select(0, (int *) 0, (int *) 0, (int *) 0, &flashPeriod);
}

/*
 *----------------------------------------------------------------------
 *
 * SxDrawShadow --
 *
 *	This procedure is used to draw boxes with shadows underneath
 *	them, like the box around a pull-down menu or a notifier.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	A box gets drawn with a shadow around its right and bottom
 *	edges.
 *
 *----------------------------------------------------------------------
 */

void
SxDrawShadow(w, x, y, width, height, color, pixmap)
    Window w;			/* Window in which to draw shadowed box. */
    int x, y;			/* UL corner of shadowed box (gives location
				 * of outside pixel of box/shadow). */
    int width, height;		/* Dimensions of useable area INSIDE
				 * shadowed box. */
    int color;			/* Color to use for drawing box. */
    Pixmap pixmap;		/* If non-zero, this is the id for a pixmap
				 * that contains the old screen contents
				 * under w.  It's used here to make the shadow
				 * look nicer by appearing to be on top of
				 * the previous contents. */
{
    static Pixmap shadowPixmap = 0;
    Bitmap tmp;

    XPixSet(w, x, y, width + 2*SHADOW_BORDER, SHADOW_BORDER, color);
    XPixSet(w, x, y, SHADOW_BORDER, height + 2*SHADOW_BORDER, color);
    XPixSet(w, x + width + SHADOW_BORDER, y, SHADOW_BORDER,
	    height + 2*SHADOW_BORDER, color);
    XPixSet(w, x, y+ height + SHADOW_BORDER, width + 2*SHADOW_BORDER,
	    SHADOW_BORDER, color);
    
    if (shadowPixmap == 0) {
	int width, height;

	XQueryTileShape(shadow_width, shadow_height, &width, &height);
	if ((width <= 16) && (height <= 16)) {
	    tmp =  XStoreBitmap(width, height, shadow_bits);
	    if (tmp != 0) {
		shadowPixmap = XMakePixmap(tmp, BlackPixel, WhitePixel);
		XFreeBitmap(tmp);
	    }
	}
	if (shadowPixmap == 0) {
	    shadowPixmap = BlackPixmap;
	}
    }

    XTileSet(w, x + SHADOW_WIDTH, y + height + 2*SHADOW_BORDER,
	    width + 2*SHADOW_BORDER, SHADOW_WIDTH, shadowPixmap);
    XTileSet(w, x + width + 2*SHADOW_BORDER, y + SHADOW_WIDTH,
	    SHADOW_WIDTH, height + 2*SHADOW_WIDTH, shadowPixmap);
    if (pixmap != 0) {
	XPixmapPut(w, 0, y + height + 2*SHADOW_BORDER,
		0, y + height + 2*SHADOW_BORDER, SHADOW_WIDTH, SHADOW_WIDTH,
		pixmap, GXcopy, AllPlanes);
	XPixmapPut(w, x + width + 2*SHADOW_BORDER, 0,
		x + width + 2*SHADOW_BORDER, 0, SHADOW_WIDTH, SHADOW_WIDTH,
		pixmap, GXcopy, AllPlanes);
    }
}
