/*
 *	$Source: /u1/Sx.new/code/RCS/sx.h,v $
 *	$Header: sx.h,v 1.1 86/12/03 16:12:18 swick Exp $
 */

/*
 * sx.h --
 *
 *	This file contains declarations for things exported by the
 *	sx module.
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
 *
 *
 * $Header: sx.h,v 1.1 86/12/03 16:12:18 swick Exp $ SPRITE (Berkeley)
 */

#ifndef _SX
#define _SX

#ifndef AllPlanes
#include <X/Xlib.h>
#endif

#ifndef _SPRITE
#include "sprite.h"
#endif _SPRITE

/*
 * Exported handles for data structures:
 */

typedef struct Sx_EventHandler *	Sx_EventHandler;

/*
 * Structure used to describe one entry in a pull-down menu:
 */

typedef struct Sx_MenuEntry {
    char *leftText;		/* Text to display left-justified in
				 * menu entry.  NULL means no text. */
    char *centerText;		/* Text to display in center of menu
				 * entry.  NULL means nothing. */
    char *rightText;		/* Text to display right-justified in
				 * menu entry.  NULL means no text. */
    int background;		/* Color for background of menu entry.  -1
				 * means pick a default color. */
    void (*proc)();		/* Procedure to call when menu entry
				 * is selected. */
    ClientData clientData;	/* Client-supplied information to pass
				 * to proc. */
} Sx_MenuEntry;

/*
 * New event masks for finding out about size changes and window destruction:
 */

#define SX_DESTROYED	((int) 0x80000000)

/*
 * Units for scrolling:
 */

#define SX_SCROLL_ABSOLUTE	0
#define SX_SCROLL_PAGES		1

/*
 * Menu-related constants:
 */

#define SX_MAX_MENU_ENTRIES	16
#define SX_MAX_MENUS		16

/*
 * Selection-related constants:
 */

#define SX_FORMAT_SIZE		20

/*
 * Directions, used in packing:
 */

typedef enum {SX_TOP, SX_BOTTOM, SX_LEFT, SX_RIGHT} Sx_Side;

/*
 * Exported procedures:
 */

extern Sx_EventHandler	Sx_HandlerCreate();
extern void		Sx_HandlerDelete();
extern void		Sx_HandleEvent();
extern void		Sx_GrabMouse();
extern void		Sx_UngrabMouse();

extern Window		Sx_ButtonCreate();
extern void		Sx_ButtonMake();

extern Window		Sx_EntryCreate();
extern void		Sx_EntryMake();

extern Window		Sx_MenuCreate();
extern int		Sx_MenuGetInfo();
extern int		Sx_MenuGetMask();
extern int		Sx_MenuGetNames();
extern Window		Sx_MenuGetWindow();
extern void		Sx_MenuReplaceEntry();
extern int		Sx_MenuSetMask();

extern int		Sx_Notify();
extern void		Sx_NullProc();
extern void		Sx_Panic();

extern void		Sx_Pack();
extern Window		Sx_CreatePacked();

extern FontInfo *	Sx_GetDefaultFont();
extern void		Sx_SetDefaultFont();

extern Window		Sx_ScrollbarCreate();
extern void		Sx_ScrollbarMake();
extern void		Sx_ScrollbarSetRange();
extern void		Sx_ScrollbarGetRange();
extern int		Sx_ScrollbarWidth();

extern void		Sx_SelectionSet();
extern void		Sx_SelectionClear();
extern int		Sx_SelectionGet();

extern Window		Sx_TitleCreate();
extern void		Sx_TitleMake();

#endif _SX
