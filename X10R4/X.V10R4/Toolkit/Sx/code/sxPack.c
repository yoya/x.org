/*
 *	$Source: /u1/Sx.new/code/RCS/sxPack.c,v $
 *	$Header: sxPack.c,v 1.1 86/12/03 16:10:35 swick Exp $
 */

#ifndef lint
static char *rcsid_sxPack_c = "$Header: sxPack.c,v 1.1 86/12/03 16:10:35 swick Exp $";
#endif	lint

/* 
 * sxPack.c --
 *
 *	This file provides simple layout management within a window
 *	by packing subwindows from the outside in to fill the available
 *	space.
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
static char rcsid[] = "$Header: sxPack.c,v 1.1 86/12/03 16:10:35 swick Exp $ SPRITE (Berkeley)";
#endif not lint

#include <sys/types.h>
#include <X/Xlib.h>
#include "sprite.h"
#include "list.h"
#include "mem.h"
#include "sx.h"
#include "sxInt.h"

/*
 * Records of the type below are used to keep track of windows for
 * packing purposes.  Each window that's involved in packing, either
 * because its location and size are managed automatically or because
 * it has other windows packed inside it (or both), has an associated
 * record.
 */

typedef struct Packer{
    List_Links links;		/* Refers to siblings that are also packed
				 * inside this window's parent.  Being earlier
				 * on this list gives priority in space
				 * allocation. */
    Window w;			/* X's identifier for this window. */
    int x, y;			/* Location of window inside parent. */
    int width, height;		/* Last known dimensions of window (inside
				 * dimensions, not including border).  <= 0
				 * means window isn't mapped at all. */
    int border;			/* Twice the width of w's border. */
    struct Packer *parentPtr;	/* Info describing parent.  NULL means this
				 * window isn't packed right now. */
    Sx_Side side;		/* Side of cavity that window abuts. */
    int size;			/* Size of window in pixels not including
				 * border,in direction perpendicular to
				 * side. */
    List_Links children;	/* Head of list of children packed inside
				 * this window. */
    int flags;			/* Miscellaneous flag values.  See below. */
} Packer;

/*
 * Flag values for Packers:
 *
 * PARENT:		1 means this window has (at some time, if not
 *			presently) been a parent (which means it has
 *			event handlers set up for it).
 */

#define PARENT 1

/*
 * Association table used to keep track of all the Packers, hashed on
 * window id.
 */

static XAssocTable *packingTable = NULL;
extern XAssocTable *XCreateAssocTable();

/*
 * Forward references to procedures defined later in this file:
 */

extern void		ArrangePacking();
extern void		PackDestroyProc();
extern void		PackExposeProc();

/*
 *----------------------------------------------------------------------
 *
 * Sx_Pack --
 *
 *	Pack a child window inside a parent window.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	From  now on, child's size and location will be managed
 *	automatically.  Child will be placed along one side of
 *	the cavity remaining inside parent after packing all
 *	children from prior calls to Sx_Pack.  If size is greater
 *	than zero, it specifies the size of child in the direction
 *	perpendicular to side.  The child will completely fill the
 *	cavity along the given side, and will fill the entire
 *	remaining cavity if size is <= 0.  Once this child is placed,
 *	other children (specified in later calls to Sx_Pack)
 *	will only use the space that remains inside parent's cavity
 *	after this child (which may be none).  If before or after
 *	is given, the child will be inserted in the packing order,
 *	either before before or after after, rather than at the
 *	end of the packing order.
 *
 *----------------------------------------------------------------------
 */

void
Sx_Pack(child, parent, side, size, border, before, after)
    Window child;		/* Window whose size and location are to
				 * be managed automatically from now on.
				 * Must not already be managed by packer,
				 * and must not be mapped right now.  This
				 * module takes care of mapping the child,
				 * either now (if the parent has already
				 * been mapped) or when the parent gets
				 * mapped. */
    Window parent;		/* Parent of child;  child will be packed
				 * in here.  Caller must eventually arrange
				 * for this window to be mapped, if it isn't
				 * already mapped. */
    Sx_Side side;		/* Which side of parent child should be
				 * lined up against.  */
    int size;			/* Dimension of child (not including border)
				 * in direction perpendicular to side.  <= 0
				 * means fill entire cavity. */
    int border;			/* Width of child's border. */
    Window before;		/* If non-zero, then insert child in order
				 * before this window. */
    Window after;		/* If non-zero, then insert child in order
				 * after this window.  Must be zero if
				 * before is non-zero. */
{
    Packer *parentPtr, *child2Ptr;
    Packer *childPtr;

    /*
     * First find the information for the parent, and create a new
     * Packer if we don't already know about the parent.  Before
     * that, build the association table if it didn't exist already.
     */

    border *= 2;
    if (packingTable == NULL) {
	packingTable = XCreateAssocTable(16);
    }
    parentPtr = (Packer *) XLookUpAssoc(packingTable, (XId) parent);
    if (parentPtr == NULL) {
	WindowInfo info;

	XQueryWindow(parent, &info);
	parentPtr = (Packer *) Mem_Alloc(sizeof(Packer));
	parentPtr->w = parent;
	parentPtr->x = info.x;
	parentPtr->y = info.y;
	if (info.mapped == IsMapped) {
	    parentPtr->width = info.width;
	    parentPtr->height = info.height;
	} else {
	    parentPtr->width = parentPtr->height = 0;
	}
	parentPtr->parentPtr = NULL;
	List_Init(&parentPtr->children);
	parentPtr->flags = 0;
	(void) Sx_HandlerCreate(parent, SX_DESTROYED, PackDestroyProc,
		(ClientData) parentPtr);
	XMakeAssoc(packingTable, (XId) parent, (caddr_t) parentPtr);
    }
    if (!(parentPtr->flags & PARENT)) {
	(void) Sx_HandlerCreate(parent, ExposeWindow, PackExposeProc,
		(ClientData) parentPtr);
	parentPtr->flags |= PARENT;
    }

    /*
     * See if there's already a Packer for the child (it might be a parent
     * too).  If so, it better not already be a child.  If there isn't
     * already a Packer for the child, make one.
     */
    
    childPtr = (Packer *) XLookUpAssoc(packingTable, (XId) child);
    if (childPtr == NULL) {
	childPtr = (Packer *) Mem_Alloc(sizeof(Packer));
	childPtr->w = child;
	childPtr->width = childPtr->height = 0;
	List_Init(&childPtr->children);
	childPtr->flags = 0;
	(void) Sx_HandlerCreate(child, SX_DESTROYED, PackDestroyProc,
		(ClientData) childPtr);
	XMakeAssoc(packingTable, (XId) child, (caddr_t) childPtr);
    } else if (childPtr->parentPtr != NULL) {
	Sx_Panic("Sx_Pack: child was already packed.");
    }

    /*
     * Make sure that the parent isn't already somehow a child of
     * the child.  A cycle would cause all sorts of bizarre behavior.
     */
    
    for (child2Ptr = parentPtr; child2Ptr != NULL;
	    child2Ptr = child2Ptr->parentPtr) {
	if (child2Ptr == childPtr) {
	    Sx_Panic("Sx_Pack: parent was already packed as descendant of child.  Can't have circular packings.");
	}
    }
    
    /*
     * Insert the child in the packing order for the parent.  Then
     * reconfigure the windows inside the parent.
     */
    
    childPtr->border = border;
    childPtr->parentPtr = parentPtr;
    childPtr->side = side;
    childPtr->size = size;
    if (before != NULL) {
	after = before;
    }
    if (after != NULL) {
	child2Ptr = (Packer *) XLookUpAssoc(packingTable, (XId) after);
	if ((child2Ptr == NULL) || (child2Ptr->parentPtr != parentPtr))  {
	    Sx_Panic("Sx_Pack: tried to pack next to a window that isn't \
packed or isn't a sibling.");
	}
	if (before != NULL) {
	    List_Insert(&childPtr->links, LIST_BEFORE(&child2Ptr->links));
	} else {
	    List_Insert(&childPtr->links, LIST_AFTER(&child2Ptr->links));
	}
    } else {
	List_Insert(&childPtr->links, LIST_ATREAR(&parentPtr->children));
    }

    ArrangePacking(parentPtr);
}

/*
 *----------------------------------------------------------------------
 *
 * Sx_CreatePacked --
 *
 *	Like Sx_Pack, except create the window to be packed.
 *
 * Results:
 *	The return value is a new window whose size and location
 *	will be managed automatically.  If parent is mapped, new child
 *	will be too;  otherwise, child will be mapped automatically
 *	when parent becomes mapped.  See the comments for Sx_Pack
 *	for information on how the layout is managed.
 *
 * Side effects:
 *	A new window is created and packing information is set up.
 *
 *----------------------------------------------------------------------
 */

Window
Sx_CreatePacked(parent, side, size, border, before, after, borderPixmap,
	backgroundPixmap)
    Window parent;		/* Parent of child;  child will be packed
				 * in here.  Caller must eventually arrange
				 * for this window to be mapped, if it isn't
				 * already mapped. */
    Sx_Side side;		/* Which side of parent child should be
				 * lined up against.  */
    int size;			/* Dimension of child (not including border)
				 * in direction perpendicular to side.  <= 0
				 * means fill entire cavity. */
    int border;			/* Width of child's border. */
    Window before;		/* If non-zero, then insert child in order
				 * before this window. */
    Window after;		/* If non-zero, then insert child in order
				 * after this window.  Must be zero if
				 * before is non-zero. */
    Pixmap borderPixmap;	/* Border pixmap to use for new window. */
    Pixmap backgroundPixmap;	/* Background pixmap to use for new window. */
{
    Window w;

    w = XCreateWindow(parent, 0, 0, 1, 1, border, borderPixmap,
	    backgroundPixmap);
    if (w == NULL) {
	Sx_Panic("Sx_CreatePacked:  couldn't create new window.");
    }
    Sx_Pack(w, parent, side, size, border, before, after);
    return w;
}

/*
 *----------------------------------------------------------------------
 *
 * Sx_Unpack --
 *	
 *	Don't manage window's location and size anymore.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	Window is removed from the packing list for its parent.  It's
 *	also unmapped.
 *
 *----------------------------------------------------------------------
 */

void
Sx_Unpack(window)
    Window window;		/* Window whose layout is no longer to be
				 * managed automatically. */
{
    register Packer *childPtr;

    if (packingTable == NULL) {
	return;
    }
    childPtr = (Packer *) XLookUpAssoc(packingTable, (XId) window);
    if ((childPtr == NULL) || (childPtr->parentPtr == NULL)) {
	return;
    }

    List_Remove(&childPtr->links);
    childPtr->width = childPtr->height = 0;
    XUnmapWindow(childPtr->w);
    ArrangePacking(childPtr->parentPtr);
    childPtr->parentPtr = NULL;
}

/*
 *----------------------------------------------------------------------
 *
 * ArrangePacking --
 *
 *	This procedure scans through the packing list for a parent
 *	window, computing the correct size for each child and modifying
 *	that child's size if it isn't already correct.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	Windows' sizes get changed.
 *
 *----------------------------------------------------------------------
 */

static void
ArrangePacking(parentPtr)
    register Packer *parentPtr;		/* Pointer to structure describing
					 * containing window that is to be
					 * re-arranged. */
{
    register Packer *childPtr;
    int x, y, width, height;
    int cavityX, cavityY, cavityWidth, cavityHeight;

    cavityX = cavityY = 0;
    cavityWidth = parentPtr->width;
    cavityHeight = parentPtr->height;
    if ((cavityWidth == 0) || (cavityHeight == 0)) {
	return;			/* Why bother? */
    }
    LIST_FORALL(&parentPtr->children, ((List_Links *) childPtr)) {
	if ((childPtr->side == SX_TOP) || (childPtr->side == SX_BOTTOM)) {
	    width = cavityWidth - childPtr->border;
	    height = childPtr->size;
	    if (height <= 0) {
		height = cavityHeight - childPtr->border;
	    }
	    cavityHeight -= height + childPtr->border;
	    if (cavityHeight < 0) {
		height += cavityHeight;
		cavityHeight = 0;
	    }
	    x = cavityX;
	    if (childPtr->side == SX_TOP) {
		y = cavityY;
		cavityY += height + childPtr->border;
	    } else {
		y = cavityY + cavityHeight;
	    }
	} else {
	    height = cavityHeight - childPtr->border;
	    width = childPtr->size;
	    if (width <= 0) {
		width = cavityWidth - childPtr->border;
	    }
	    cavityWidth -= width + childPtr->border;
	    if (cavityWidth < 0) {
		width += cavityWidth;
		cavityWidth = 0;
	    }
	    if (childPtr->side == SX_LEFT) {
		x = cavityX;
		cavityX += width + childPtr->border;
	    } else {
		x = cavityX + cavityWidth;
	    }
	    y = cavityY;
	}

	/*
	 * If the size or location of the child has changed, any
	 * of several things may have to happen: a) reconfigure
	 * the child;  b) unmap the child (if it's too small to be
	 * interesting);  c) first reconfigure and then map the
	 * child (if it's just gotten large enough to be interesting
	 * again);  and/or d) recursively process the child, if its
	 * interior is also packed.
	 */
	
	if ((x != childPtr->x) || (y != childPtr->y)
		|| (width != childPtr->width)
		|| (height != childPtr->height)) {
	    if ((width <= 0) || (height <= 0)) {
	        if ((childPtr->width > 0) && (childPtr->height > 0)) {
		    XUnmapWindow(childPtr->w);
		}
	    } else {
		XConfigureWindow(childPtr->w, x, y, width, height);
		if ((childPtr->width <= 0) || (childPtr->height <= 0)) {
		    XMapWindow(childPtr->w);
		}
	    }
	    childPtr->x = x;
	    childPtr->y = y;
	    childPtr->width = width;
	    childPtr->height = height;
	    if (!List_IsEmpty(&childPtr->children)) {
		ArrangePacking(childPtr);
	    }
	}
    }
}

/*
 *----------------------------------------------------------------------
 *
 * PackExposeProc --
 *
 *	Called by the Sx dispatcher whenever an ExposeWindow event
 *	occurs for a window whose internal structure is managed by
 *	this file.  If the window's size has changed, then all
 *	the children are re-arranged to match it.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	The sizes and locations of children for this window get
 *	re-arranged.
 *
 *----------------------------------------------------------------------
 */

void
PackExposeProc(parentPtr, eventPtr)
    register Packer *parentPtr;		/* Parent window whose size may have
					 * changed. */
    register XExposeEvent *eventPtr;	/* Gives new size of parent. */
{
    if (eventPtr->subwindow != 0) {
	return;
    }
    if ((parentPtr->width == eventPtr->width)
	    && (parentPtr->height == eventPtr->height)) {
	return;
    }
    parentPtr->height = eventPtr->height;
    parentPtr->width = eventPtr->width;
    ArrangePacking(parentPtr);
}

/*
 *----------------------------------------------------------------------
 *
 * PackDestroyProc --
 *
 *	This procedure is called by the Sx dispatcher whenever
 *	a window is destoryed that is packed or has stuff packed
 *	inside it.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	The local data structures get cleaned up.
 *
 *----------------------------------------------------------------------
 */

void
PackDestroyProc(parentPtr, eventPtr)
    register Packer *parentPtr;		/* Info about deleted window. */
    register XExposeEvent *eventPtr;	/* Gives window id. */
{
    register Packer *childPtr;
    Window saved;

    /*
     * Make sure that we haven't already cleaned up this window.
     */
    
    if (XLookUpAssoc(packingTable, (XId) eventPtr->window) == NULL) {
	return;
    }
    XDeleteAssoc(packingTable, (XId) eventPtr->window);

    /*
     * Unlink from the parent in which it's packed, if any.
     */

    if (parentPtr->parentPtr != NULL) {
	List_Remove(&parentPtr->links);
	ArrangePacking(parentPtr->parentPtr);
    }

    /*
     * Recursively process children.
     */

    parentPtr->width = parentPtr->height = 0;
    saved = eventPtr->window;
    LIST_FORALL(&parentPtr->children, (List_Links *) childPtr) {
	eventPtr->window = childPtr->w;
	PackDestroyProc(childPtr, eventPtr);
    }
    eventPtr->window = saved;

    Mem_Free((Address) parentPtr);
}
