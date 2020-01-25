
/************************************************************************
 *
 * POQ.h --
 *
 *
 * Copyright 1992, 1993 Data General Corporation;
 * Copyright 1992, 1993 OMRON Corporation  
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting documentation, and
 * that neither the name OMRON or DATA GENERAL be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission of the party whose name is to be used.  Neither 
 * OMRON or DATA GENERAL make any representation about the suitability of this
 * software for any purpose.  It is provided "as is" without express or 
 * implied warranty.  
 *
 * OMRON AND DATA GENERAL EACH DISCLAIM ALL WARRANTIES WITH REGARD TO THIS
 * SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS,
 * IN NO EVENT SHALL OMRON OR DATA GENERAL BE LIABLE FOR ANY SPECIAL, INDIRECT
 * OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
 * USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
 * OF THIS SOFTWARE.
 *
 ************************************************************************/

/* $XConsortium: POQ.h,v 1.1 94/03/17 11:34:54 dpw Exp $ */

#ifndef POQ_H
#define POQ_H

#include "X.h"
#include "windowstr.h"
#include "gc.h"
#include "pixmapstr.h"
#include "regionstr.h"
#include "conflict.h"
#include "xthreads.h"


/***********************************************************************/

typedef struct _Conflict {
    WindowPtr   pWindow;         /* pointer to window to lock */
    RegionPtr   pRegion;         /* pointer to region to lock */
    int		windowMode;	 /* is pWindow a single window or subtree */
} ConflictRec, *ConflictPtr;


typedef struct _Extension {
    int 	extID;           /* extension ID */
    Mask  	extConflictMask; /* extension conflict mask */
    pointer	pExtConflict;	 /* pointer to extension conflict */
} ExtensionRec, *ExtensionPtr;


/*********************************
 * Read/Write conflict mask bits.
 * Bits 0 - 19 are Read/Write.
 *********************************/

#define POQ_R_HIERARCHY        ((Mask)1<<0)
#define POQ_W_HIERARCHY        ((Mask)1<<1)
#define POQ_R_GEOMETRY         ((Mask)1<<2)
#define POQ_W_GEOMETRY         ((Mask)1<<3)
#define POQ_R_COLORMAP         ((Mask)1<<4)
#define POQ_W_COLORMAP         ((Mask)1<<5)
#define POQ_R_EVENT_PROP       ((Mask)1<<6)
#define POQ_W_EVENT_PROP       ((Mask)1<<7)
#define POQ_R_SCREENSAVER      ((Mask)1<<8)
#define POQ_W_SCREENSAVER      ((Mask)1<<9)
#define POQ_R_FONTPATH         ((Mask)1<<10)
#define POQ_W_FONTPATH         ((Mask)1<<11)

/*********************************
 * Write only conflict mask bits.
 * Bits 20 - 29 are Write only.
 *********************************/

#define POQ_X_CURSOR           ((Mask)1<<20)
#define POQ_X_RENDER           ((Mask)1<<21)
#define POQ_X_ICCCM            ((Mask)1<<22)
#define POQ_X_SERVER           ((Mask)1<<23)
#define POQ_X_FONT             ((Mask)1<<24)
#define POQ_CONFIGURE          ((Mask)1<<25)
#define POQ_NO_CONFLICT        ((Mask)1<<26)
#define POQ_GRAB_SERVER        ((Mask)1<<27)

/*********************************************
 * Used to indicate that the extension
 * conflict bits should be checked.
 *********************************************/

#define POQ_EXTENSION_BIT      ((Mask)1<<31)


/*********************************************
 * Used to determine which clipping region
 * in the WindowRec should be used.
 *********************************************/

#define POQ_NULL_REGION	       0
#define POQ_CLIP_LIST	       1
#define POQ_BORDER_CLIP	       2
#define POQ_WIN_SIZE	       3
#define POQ_BORDER_SIZE	       4

/*********************************************
 * Used to indicate whether the pWindow field
 * in the ConflictRec indicates a single 
 * window or a window subtree.
 *********************************************/

#define POQ_NO_WINDOW          0
#define POQ_SINGLE_WINDOW      1
#define POQ_WINDOW_SUBTREE     2
#define POQ_ALL_WINDOWS        3 

/*****************************************************
 * Used to tell the POQ how to handle region conflicts
 *****************************************************/

#define POQ_1_BIT_PER_PIXEL     0
#define POQ_8_BITS_PER_PIXEL    1
#define POQ_24_BITS_PER_PIXEL   2
#define POQ_32_BITS_PER_PIXEL   3
#define POQ_USE_GRAPHICS_ACCEL  4


/***********************************************************************
 *
 * Set no region or window conflicts.
 *
 ***********************************************************************/

#define POQ_SET_NULL_CONFLICT(client)					\
{									\
    ((ConflictPtr)(client)->pConflict)->windowMode = POQ_NO_WINDOW;     \
    ((ConflictPtr)(client)->pConflict)->pWindow = (WindowPtr)NULL;	\
    ((ConflictPtr)(client)->pConflict)->pRegion = (RegionPtr)NULL;	\
}

/***********************************************************************
 *
 * Lock the root window and region representing entire screen.
 *
 ***********************************************************************/

#define POQ_SET_ALL_CONFLICT(client)					\
{									\
    ((ConflictPtr)(client)->pConflict)->windowMode = POQ_ALL_WINDOWS;   \
    ((ConflictPtr)(client)->pConflict)->pWindow = (WindowPtr)NULL;	\
    ((ConflictPtr)(client)->pConflict)->pRegion = &VisibleRegion;	\
}

/***********************************************************************
 *
 * If the drawable is a window, lock the region determined by
 * the subWindowMode in the GC.
 *
 ***********************************************************************/

#define POQ_SET_RENDER_CONFLICT(client, pDraw, pGC)       		\
{                                                                       \
    if ((pDraw)->type == DRAWABLE_WINDOW)                               \
    {                                                                   \
	WindowPtr _pWin = (WindowPtr)(pDraw);				\
									\
        if ((pGC)->subWindowMode == IncludeInferiors)                   \
        {                                                               \
            ((ConflictPtr)(client)->pConflict)->pRegion = &(_pWin->borderClip);\
        }                                                               \
        else                                                            \
        {                                                               \
            ((ConflictPtr)(client)->pConflict)->pRegion = &(_pWin->clipList);\
        }                                                               \
        ((ConflictPtr)(client)->pConflict)->pWindow = _pWin;		\
        ((ConflictPtr)(client)->pConflict)->windowMode = POQ_SINGLE_WINDOW; \
    }                                                                   \
    else                                                                \
    {                                                                   \
	POQ_SET_NULL_CONFLICT(client);					\
    }                                                                   \
}

/***********************************************************************
 *
 * Lock the region in the window specified by regionType.
 *
 ***********************************************************************/

#define POQ_SET_WINDOW_CONFLICT(client, pWin, regionType)     		\
{                                                                       \
    switch (regionType)                                                 \
    {                                                                   \
        case POQ_CLIP_LIST:                                             \
           ((ConflictPtr)(client)->pConflict)->pRegion = &((pWin)->clipList);\
            break;                                                      \
                                                                        \
        case POQ_BORDER_CLIP:                                           \
           ((ConflictPtr)(client)->pConflict)->pRegion = &((pWin)->borderClip);\
            break;                                                      \
                                                                        \
        case POQ_WIN_SIZE:                                              \
           ((ConflictPtr)(client)->pConflict)->pRegion = &((pWin)->winSize);\
            break;                                                      \
                                                                        \
        case POQ_BORDER_SIZE:                                           \
           ((ConflictPtr)(client)->pConflict)->pRegion = &((pWin)->borderSize);\
            break;                                                      \
                                                                        \
        case POQ_NULL_REGION:                                           \
	    /* fall through */						\
        default:                                                        \
            ((ConflictPtr)(client)->pConflict)->pRegion = (RegionPtr)NULL;\
            break;                                                      \
    }                                                                   \
    ((ConflictPtr)(client)->pConflict)->pWindow = (pWin);		\
    ((ConflictPtr)(client)->pConflict)->windowMode = POQ_WINDOW_SUBTREE;\
}

/***********************************************************************
 *
 * If the drawable passed in is a window, lock the border region.
 *
 ***********************************************************************/

#define POQ_SET_DRAWABLE_CONFLICT(client, pDraw)			\
{									\
    if ((pDraw)->type == DRAWABLE_WINDOW)				\
    {									\
        ((ConflictPtr)(client)->pConflict)->pWindow = (WindowPtr)(pDraw);\
        ((ConflictPtr)(client)->pConflict)->windowMode = POQ_WINDOW_SUBTREE;\
        ((ConflictPtr)(client)->pConflict)->pRegion = 			\
				&(((WindowPtr)(pDraw))->borderSize);	\
    }									\
    else								\
    {									\
	POQ_SET_NULL_CONFLICT((client));				\
    }									\
}

/***********************************************************************
 *
 * Set the region and window conflicts.
 *
 ***********************************************************************/

#define POQ_SET_REGION_CONFLICT(client, pWin, pReg)			\
{									\
    ((ConflictPtr)(client)->pConflict)->windowMode = POQ_WINDOW_SUBTREE;\
    ((ConflictPtr)(client)->pConflict)->pWindow = (pWin);		\
    ((ConflictPtr)(client)->pConflict)->pRegion = (pReg);		\
}

/***********************************************************************
 *
 * Set conflicts for extensions.
 *
 ***********************************************************************/

#define POQ_SET_EXTENSION_CONFLICT(client, conflictMask, pExtConflict)	\
{									\
    int extID = (int)(((xReq *)client->requestBuffer)->reqType);	\
									\
    ((ConflictPtr)(client)->pExtInfo)->extID = extID;			\
    ((ConflictPtr)(client)->pExtInfo)->extConflictMask = conflictMask;	\
    ((ConflictPtr)(client)->pExtInfo)->pExtConflict = pExtConflict;	\
}
/***********************************************************************
 *
 * NEW MACROS: For STX,MTX merge, these cover the Lock/UnlockDevicesAndPOQ
 *             function call.
 *
 ***********************************************************************/
#ifdef XTHREADS
#define MTXLockDevicesAndPOQ(client, mapping)				\
    LockDevicesAndPOQ(client, mapping)
#define MTXUnlockDevicesAndPOQ(client)					\
    UnlockDevicesAndPOQ(client)
#else
#define MTXLockDevicesAndPOQ(client, mapping) /* nothing */
#define MTXUnlockDevicesAndPOQ(client) /* nothing */
#endif

/***********************************************************************/

extern void POQInitializeMonitor();
extern void POQDestroyMonitor();
extern void POQSetRegionConflictFunc();
extern void POQSelectRegionConflictType();
extern void POQSetExtConflictFunc();
extern void POQInitClient();
extern void POQLock();
#ifdef XTHREADS
extern void POQUnlock();
#endif
extern void POQGrabServer();
extern void POQUngrabServer();
extern void POQComeUpForAir();

extern RegionRec VisibleRegion;

/***********************************************************************/

#endif /* POQ_H */
