
/************************************************************************
 *
 * mtxlock.h --
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

/* $XConsortium: mtxlock.h,v 1.1 94/03/17 11:35:36 dpw Exp $ */

#ifndef MTXLOCK_H
#define MTXLOCK_H

#include "X.h"
#include "windowstr.h"
#include "gc.h"
#include "pixmapstr.h"
#include "regionstr.h"
#include "message.h"
#include "conflict.h"
#include "xthreads.h"
#include "resource.h"
#include "POQ.h"

#ifndef XTHREADS
#define MTX_REP_LOCK_DEVICES() /* nothing */
#define MTX_LOCK_DEVICES() /* nothing */
#define MTX_UNLOCK_DEVICES() /* nothing */
#else /* XTHREADS */
#define MTX_REP_LOCK_DEVICES() LockDevices()
#define MTX_LOCK_DEVICES() LockDevices()
#define MTX_UNLOCK_DEVICES() UnlockDevices()
#endif /* XTHREADS */

#ifndef XTHREADS
#define MTX_MUTEX_LOCK(_arg) /* nothing */
#define MTX_MUTEX_UNLOCK(_arg) /* nothing */
#else /* XTHREADS */
#define MTX_MUTEX_LOCK(_arg) X_MUTEX_LOCK(_arg)
#define MTX_MUTEX_UNLOCK(_arg) X_MUTEX_UNLOCK(_arg)
#endif /* XTHREADS */

/***********************************************************************
 *
 * Lock and validate a drawable and GC.
 *
 ***********************************************************************/

#define LOCK_AND_VALIDATE_DRAWABLE_AND_GC(pDraw, pGC, drawID, gcID, client)\
{									\
    int _error;								\
									\
    if (_error = LockAndVerifyDrawableAndGC (&(pDraw), &(pGC), (drawID),\
					      (gcID), (client)))	\
    {									\
	return (_error);						\
    }									\
    POQ_SET_RENDER_CONFLICT((client), (pDraw), (pGC));			\
									\
    POQLock((client), (CM_XRender));					\
									\
    if ((pGC)->serialNumber != (pDraw)->serialNumber)			\
    {									\
        ValidateGC((pDraw), (pGC));					\
    }									\
}
/***********************************************************************
 *
 * STX to MTX merge versions of Lock and validate a drawable and GC.
 *
 ***********************************************************************/
#ifdef XTHREADS
#define MTX_LOCK_AND_VALIDATE_DRAWABLE_AND_GC LOCK_AND_VALIDATE_DRAWABLE_AND_GC
#else
#define MTX_LOCK_AND_VALIDATE_DRAWABLE_AND_GC(pDraw, pGC, drawID, gcID, client)\
{									\
    VALIDATE_DRAWABLE_AND_GC(drawID, pDraw, pGC, client);		\
}
#endif

/***********************************************************************
 *
 * Unlock a drawable and GC.
 *
 ***********************************************************************/

#define UNLOCK_DRAWABLE_AND_GC(pDraw, pGC, drawID, gcID, client)	\
{									\
    POQUnlock((client));						\
    UnlockDrawableAndGC ((pDraw), (pGC), (drawID), (gcID));		\
}
/***********************************************************************
 *
 * STX to MTX merge versions of Unlock a drawable and GC.
 *
 ***********************************************************************/
#ifdef XTHREADS
#define MTX_UNLOCK_DRAWABLE_AND_GC UNLOCK_DRAWABLE_AND_GC
#else
#define MTX_UNLOCK_DRAWABLE_AND_GC /* nothing */
#endif


/***********************************************************************
 *
 * Lock and validate two drawables and GC.  If the srcID and dstID 
 * drawable ID's are the same, then only one drawable needs to be locked.
 *
 ***********************************************************************/

#define LOCK_AND_VALIDATE_TWO_DRAWABLES_AND_GC(pSrc, pDst, pGC, srcID, dstID, gcID, client) \
{									\
    int _error;								\
									\
    if ((srcID) == (dstID))						\
    {									\
        if (_error = LockAndVerifyDrawableAndGC (&(pDst), &(pGC),	\
				      (dstID), (gcID), (client)))	\
        {								\
            return (_error);						\
        }								\
	(pSrc) = (pDst);						\
									\
        POQ_SET_RENDER_CONFLICT((client), (pDst), (pGC));		\
    }									\
    else								\
    {									\
        if (_error = LockAndVerifyTwoDrawablesAndGC (&(pSrc), &(pDst), 	\
			  &(pGC), (srcID), (dstID), (gcID), (client)))	\
        {								\
            return (_error);						\
        }								\
									\
	if ((pSrc->type == DRAWABLE_WINDOW) && 				\
	    (pDst->type == DRAWABLE_WINDOW))				\
	{								\
            POQ_SET_ALL_CONFLICT((client));				\
	}								\
	else if (pSrc->type == DRAWABLE_WINDOW)				\
	{								\
            POQ_SET_RENDER_CONFLICT((client), (pSrc), (pGC));		\
	}								\
	else if (pDst->type == DRAWABLE_WINDOW)				\
	{								\
            POQ_SET_RENDER_CONFLICT((client), (pDst), (pGC));		\
	}								\
	else /* both are pixmaps */					\
	{								\
            POQ_SET_NULL_CONFLICT((client));				\
	}								\
    }									\
									\
    POQLock((client), (CM_XRender));					\
									\
    if ((pGC)->serialNumber != (pDst)->serialNumber)			\
    {									\
        ValidateGC((pDst), (pGC));					\
    }									\
}
/***********************************************************************
 *
 * STX to MTX merge versions of Lock and validate two drawables and GC.  
 * If the srcID and dstID drawable ID's are the same, then only one 
 * drawable needs to be locked.
 *
 ***********************************************************************/
#ifdef XTHREADS
#define MTX_LOCK_AND_VALIDATE_TWO_DRAWABLES_AND_GC LOCK_AND_VALIDATE_TWO_DRAWABLES_AND_GC
#else
#define MTX_LOCK_AND_VALIDATE_TWO_DRAWABLES_AND_GC(pSrc, pDst, pGC, srcID, dstID, gcID, client)\
{									\
    VALIDATE_DRAWABLE_AND_GC(dstID, pDst, pGC, client);			\
    if (dstID != srcID)							\
    {									\
	VERIFY_DRAWABLE(pSrc, srcID, client);				\
	if (pDst->pScreen != pSrc->pScreen)				\
	{								\
	    client->errorValue = dstID;					\
	    return (BadMatch);						\
	}								\
    }									\
    else								\
    {									\
	pSrc = pDst;							\
    }									\
}
#endif

/***********************************************************************
 *
 * Unock two drawables and a GC.  If the srcID and dstID drawable ID's 
 * are the same, then only one drawable was locked.
 *
 ***********************************************************************/

#define UNLOCK_TWO_DRAWABLES_AND_GC(pSrc, pDst, pGC, srcID, dstID, gcID, client)\
{									\
    POQUnlock((client));						\
									\
    if ((srcID) == (dstID))						\
    {									\
	UnlockDrawableAndGC ((pDst), (pGC), (dstID), (gcID));		\
    }									\
    else								\
    {									\
        UnlockTwoDrawablesAndGC ((pSrc), (pDst), (pGC), (srcID), 	\
				 (dstID), (gcID));			\
    }									\
}
/***********************************************************************
 *
 * STX to MTX merge Unlock two drawables and a GC.  If the srcID and dstID 
 * drawable ID's are the same, then only one drawable was locked.
 *
 ***********************************************************************/
#ifdef XTHREADS
#define MTX_UNLOCK_TWO_DRAWABLES_AND_GC UNLOCK_TWO_DRAWABLES_AND_GC
#else
#define MTX_UNLOCK_TWO_DRAWABLES_AND_GC(pSrc, pDst, pGC, srcID, dstID, gcID, client) 	\
	    /* nothing */
#endif



/***********************************************************************
 *
 * Lock and verify a window. 
 *
 * STX to MTX merge versions of Lock and verify a window. 
 *
 ***********************************************************************/
#ifdef XTHREADS
#define MTX_REP_LOCK_AND_VERIFY_WINDOW(pWin, winID, client, regionType, conflictMask)\
{									\
    int _error;								\
									\
    if (_error = LockAndVerifyWindow(&(pWin), (winID), (client)))	\
    {									\
	MTXReturnPooledMessage;						\
	return (_error);						\
    }									\
    POQ_SET_WINDOW_CONFLICT((client), (pWin), (regionType));		\
									\
    POQLock((client), (conflictMask));					\
}
#define MTX_LOCK_AND_VERIFY_WINDOW(pWin, winID, client, regionType, conflictMask)\
{									\
    int _error;								\
									\
    if (_error = LockAndVerifyWindow(&(pWin), (winID), (client)))	\
    {									\
	return (_error);						\
    }									\
    POQ_SET_WINDOW_CONFLICT((client), (pWin), (regionType));		\
									\
    POQLock((client), (conflictMask));					\
}
#else
#define MTX_REP_LOCK_AND_VERIFY_WINDOW MTX_LOCK_AND_VERIFY_WINDOW
#define MTX_LOCK_AND_VERIFY_WINDOW(pWin, winID, client, regionType, conflictMask)\
{									\
    if (!(pWin = (WindowPtr)LookupWindow(winID, client)))		\
	return BadWindow;						\
}
#endif


/***********************************************************************
 *
 * Unlock a window. 
 *
 ***********************************************************************/

#define UNLOCK_WINDOW(pWin, winID, client)				\
{									\
    POQUnlock((client));						\
    UnlockWindow ((pWin), (winID));					\
}
/***********************************************************************
 *
 * STX to MTX merge versions of Unlock a window. 
 *
 ***********************************************************************/
#ifdef XTHREADS
#define MTX_UNLOCK_WINDOW UNLOCK_WINDOW
#else
#define MTX_UNLOCK_WINDOW(pWin, winID, client) /* nothing */
#endif

#define MTX_UNLOCK_WINDOW_IF_FAILURE(pWin, whatStuff,client)		\
	MTX_UNLOCK_WINDOW(pWin,whatStuff,client);

#define MTX_UNLOCK_DRAWABLE_IF_FAILURE(pDraw, drawID, client)		\
	MTX_UNLOCK_DRAWABLE(pDraw, drawID, client);

#define MTX_UNLOCK_DEVICE_IF_FAILURE(client) 				\
	UnlockDevicesAndPOQ(client);

#define MTX_UNLOCK_POQ_IF_FAILURE(client)				\
	UNLOCK_PENDING_OPERATION_QUEUE(client);

#define MTX_UNLOCK_ALL_WINDOWS_IF_FAILURE(pwin, pdst, stuffSrc, stuffDst, client)\
	MTX_UNLOCK_ALL_WINDOWS(pwin, pdst, stuffSrc, stuffDst, client)

#define MTX_UNLOCK_FONT_IF_FAILURE(pFont, fontID, client)		\
	MTX_UNLOCK_FONT(pFont, stuff->fid, client)			\

#define MTX_UNLOCK_COLORMAP_IF_FAILURE(pmap, cmapID, client)		\
	MTX_UNLOCK_COLORMAP(pmap, cmapID, client)


/***********************************************************************
 *
 * Lock and verify one or two windows.
 *
 * STX to MTX merge versions for Lock and verify one or two windows.
 *
 ***********************************************************************/
#ifdef XTHREADS
#define MTX_REP_LOCK_AND_VERIFY_ALL_WINDOWS(pWin1, pWin2, winID1, winID2, client, regionType, conflictMask)\
{									\
    int _error;								\
									\
    if ((winID1) && (winID2))						\
    {									\
        if (_error = LockAndVerifyTwoWindows(&(pWin1), &(pWin2), 	\
					(winID1), (winID2), (client)))	\
        {								\
	    MTXReturnPooledMessage;					\
	    return (_error);						\
        }								\
        POQ_SET_ALL_CONFLICT((client));					\
    }									\
    else if ((winID1))							\
    {									\
        if (_error = LockAndVerifyWindow(&(pWin1), (winID1), (client)))	\
        {								\
	    MTXReturnPooledMessage;					\
	    return (_error);						\
        }								\
	(pWin2) = NullWindow;						\
        POQ_SET_WINDOW_CONFLICT((client), (pWin1), (regionType));	\
    }									\
    else if ((winID2))							\
    {									\
        if (_error = LockAndVerifyWindow(&(pWin2), (winID2), (client)))	\
        {								\
	    MTXReturnPooledMessage;					\
	    return (_error);						\
        }								\
	(pWin1) = NullWindow;						\
        POQ_SET_WINDOW_CONFLICT((client), (pWin2), (regionType));	\
    }									\
    else								\
    {									\
	(pWin1) = NullWindow;						\
	(pWin2) = NullWindow;						\
        POQ_SET_NULL_CONFLICT((client));				\
    }									\
    POQLock((client), (conflictMask));					\
}
#define MTX_LOCK_AND_VERIFY_ALL_WINDOWS(pWin1, pWin2, winID1, winID2, client, regionType, conflictMask)\
{									\
    int _error;								\
									\
    if ((winID1) && (winID2))						\
    {									\
        if (_error = LockAndVerifyTwoWindows(&(pWin1), &(pWin2), 	\
					(winID1), (winID2), (client)))	\
        {								\
	    return (_error);						\
        }								\
        POQ_SET_ALL_CONFLICT((client));					\
    }									\
    else if ((winID1))							\
    {									\
        if (_error = LockAndVerifyWindow(&(pWin1), (winID1), (client)))	\
        {								\
	    return (_error);						\
        }								\
	(pWin2) = NullWindow;						\
        POQ_SET_WINDOW_CONFLICT((client), (pWin1), (regionType));	\
    }									\
    else if ((winID2))							\
    {									\
        if (_error = LockAndVerifyWindow(&(pWin2), (winID2), (client)))	\
        {								\
	    return (_error);						\
        }								\
	(pWin1) = NullWindow;						\
        POQ_SET_WINDOW_CONFLICT((client), (pWin2), (regionType));	\
    }									\
    else								\
    {									\
	(pWin1) = NullWindow;						\
	(pWin2) = NullWindow;						\
        POQ_SET_NULL_CONFLICT((client));				\
    }									\
    POQLock((client), (conflictMask));					\
}
#else
#define MTX_REP_LOCK_AND_VERIFY_ALL_WINDOWS MTX_LOCK_AND_VERIFY_ALL_WINDOWS
#define MTX_LOCK_AND_VERIFY_ALL_WINDOWS(pWin1, pWin2, winID1, winID2, client, regionType, conflictMask)\
{									\
    if ((winID1))							\
    {									\
	if (!(pWin1 = (WindowPtr)LookupWindow(winID1, client)))		\
	    return BadWindow;						\
    }									\
    else								\
	(pWin1) = NullWindow;						\
									\
    if ((winID2))							\
    {									\
	if (!(pWin2 = (WindowPtr)LookupWindow(winID2, client)))		\
	    return BadWindow;						\
    }									\
    else								\
	(pWin2) = NullWindow;						\
}
#endif
    


/***********************************************************************
 *
 * Unlock any windows that are currently locked (pWin1 and/or pWin2).
 *
 ***********************************************************************/

#define UNLOCK_ALL_WINDOWS(pWin1, pWin2, winID1, winID2, client)	\
{									\
    POQUnlock((client));						\
									\
    if ((pWin1) && (pWin2))						\
    {									\
        UnlockTwoWindows ((pWin1), (pWin2), (winID1), (winID2));	\
    }									\
    else if ((pWin1))							\
    {									\
        UnlockWindow ((pWin1), (winID1));				\
    }									\
    else if ((pWin2))							\
    {									\
        UnlockWindow ((pWin2), (winID2));				\
    }									\
}
/***********************************************************************
 *
 * STX to MTX merge versions of Unlock any windows that are currently locked 
 * (pWin1 and/or pWin2).
 *
 ***********************************************************************/
#ifdef XTHREADS
#define MTX_UNLOCK_ALL_WINDOWS UNLOCK_ALL_WINDOWS
#else
#define MTX_UNLOCK_ALL_WINDOWS(pWin1, pWin2, winID1, winID2, client)	\
	/* nothing */
#endif


/***********************************************************************
 *
 * Lock and verify a drawable.  Return a BadMatch error if the drawable 
 * is a UNDRAWABLE_WINDOW.  
 *
 * STX to MTX merge versions of Lock and verify a drawable.  Return a 
 * BadMatch error if the drawable is a UNDRAWABLE_WINDOW.  
 *
 ***********************************************************************/
#ifdef XTHREADS
#define MTX_REP_LOCK_AND_VERIFY_DRAWABLE(pDraw, drawID, client, conflictMask) \
{									\
    int _error;								\
									\
    if (_error = LockAndVerifyDrawable(&(pDraw), (drawID), (client), TRUE))\
    {									\
	MTXReturnPooledMessage;						\
	return (_error);						\
    }									\
    POQ_SET_DRAWABLE_CONFLICT((client), (pDraw));			\
									\
    POQLock((client), (conflictMask));					\
}
#define MTX_LOCK_AND_VERIFY_DRAWABLE(pDraw, drawID, client, conflictMask)	\
{									\
    int _error;								\
									\
    if (_error = LockAndVerifyDrawable(&(pDraw), (drawID), (client), TRUE))\
    {									\
	return (_error);						\
    }									\
    POQ_SET_DRAWABLE_CONFLICT((client), (pDraw));			\
									\
    POQLock((client), (conflictMask));					\
}
#else
#define MTX_REP_LOCK_AND_VERIFY_DRAWABLE MTX_LOCK_AND_VERIFY_DRAWABLE
#define MTX_LOCK_AND_VERIFY_DRAWABLE(pDraw, stuffDrawable, client, conflictMask)\
    VERIFY_DRAWABLE(pDraw, stuffDrawable, client)
#endif


/***********************************************************************
 *
 * Lock and verify a (geomtrable) drawable. 
 *
 * STX to MTX merge version of Lock and verify a ?geometrable. 
 *
 ***********************************************************************/
#ifdef XTHREADS
#define MTX_REP_LOCK_AND_VERIFY_GEOMETRABLE(pDraw, drawID, client, conflictMask)\
{									\
    int _error;								\
									\
    if (_error = LockAndVerifyDrawable(&(pDraw), (drawID), (client), FALSE))\
    {									\
	MTXReturnPooledMessage;						\
	return (_error);						\
    }									\
    POQ_SET_DRAWABLE_CONFLICT((client), (pDraw));			\
									\
    POQLock((client), (conflictMask));					\
}
#define MTX_LOCK_AND_VERIFY_GEOMETRABLE(pDraw, drawID, client, conflictMask)\
{									\
    int _error;								\
									\
    if (_error = LockAndVerifyDrawable(&(pDraw), (drawID), (client), FALSE))\
    {									\
	return (_error);						\
    }									\
    POQ_SET_DRAWABLE_CONFLICT((client), (pDraw));			\
									\
    POQLock((client), (conflictMask));					\
}
#else
#define MTX_REP_LOCK_AND_VERIFY_GEOMETRABLE MTX_LOCK_AND_VERIFY_GEOMETRABLE
#define MTX_LOCK_AND_VERIFY_GEOMETRABLE(pDraw, drawID, client, conflictMask)\
    VERIFY_GEOMETRABLE(pDraw, drawID, client);
#endif


/***********************************************************************
 *
 * Lock and verify a pixmap. 
 *
 ***********************************************************************/

#define LOCK_AND_VERIFY_PIXMAP(pPixmap, pixmapID, client, conflictMask)	\
{									\
    int _error;								\
									\
    if (_error = LockAndVerifyPixmap(&(pPixmap), (pixmapID), (client)))	\
    {									\
	return (_error);						\
    }									\
    POQ_SET_NULL_CONFLICT((client));					\
									\
    POQLock((client), (conflictMask));					\
}
/***********************************************************************
 *
 * STX to MTX merge Lock and verify a pixmap. 
 *
 ***********************************************************************/
#ifdef XTHREADS
#define MTX_LOCK_AND_VERIFY_PIXMAP LOCK_AND_VERIFY_PIXMAP
#else
#define MTX_LOCK_AND_VERIFY_PIXMAP(pMap, pixID, client, CM_XFreePixmap)	\
{									\
    if(!(pMap = (PixmapPtr)LookupIDByType(pixID, RT_PIXMAP)))		\
    {									\
	client->errorValue = pixID;					\
	return (BadPixmap);						\
    }									\
}
#endif


/***********************************************************************
 *
 * Unlock a drawable.
 *
 ***********************************************************************/

#define UNLOCK_DRAWABLE(pDraw, drawID, client)				\
{									\
    POQUnlock((client));						\
    UnlockDrawable((pDraw), (drawID));					\
}
/***********************************************************************
 *
 * STX to MTX merge versions of UNLOCK_DRAWABLE macro.
 *
 ***********************************************************************/
#ifdef XTHREADS
#define MTX_UNLOCK_DRAWABLE UNLOCK_DRAWABLE
#else
#define MTX_UNLOCK_DRAWABLE(pDraw, drawID, client) /* nothing */
#endif
/***********************************************************************
 *
 * STX to MTX merge: Cover-up up the call to the function UnlockDrawable
 *
 ***********************************************************************/
#ifdef XTHREADS
#define MTXUnlockDrawable(drawablePtr, id) UnlockDrawable(drawablePtr, id)
#else
#define MTXUnlockDrawable(drawablePtr, id) /* nothing */
#endif




/***********************************************************************
 *
 * Lock and verify a GC.
 *
 ***********************************************************************/

#define LOCK_AND_VERIFY_GC(pGC, gcID, client, conflictMask)		\
{									\
    int _error;								\
									\
    if (_error = LockAndVerifyGC(&(pGC), (gcID), (client)))		\
    {									\
	return (_error);						\
    }									\
    POQ_SET_NULL_CONFLICT((client));					\
									\
    POQLock((client), (conflictMask));					\
}
/***********************************************************************
 *
 * STX to MTX merge versions of Lock and verify a GC.
 *
 ***********************************************************************/
#ifdef XTHREADS
#define MTX_LOCK_AND_VERIFY_GC LOCK_AND_VERIFY_GC
#else
#define MTX_LOCK_AND_VERIFY_GC(pGC, stuffGC, client, conflictMask)	\
    VERIFY_GC(pGC, stuffGC, client)
#endif


/***********************************************************************
 *
 * Unlock a GC.
 *
 ***********************************************************************/

#define UNLOCK_GC(pGC, gcID, client)					\
{									\
    POQUnlock((client));						\
    UnlockGC ((pGC), (gcID));						\
}
/***********************************************************************
 *
 * STX to MTX merge versions of Unlock a GC.
 *
 ***********************************************************************/
#ifdef XTHREADS
#define MTX_UNLOCK_GC UNLOCK_GC
#else
#define MTX_UNLOCK_GC(pGC, gcID, client) /* nothing */
#endif
/***********************************************************************
 *
 * STX to MTX Cover-up when calls are made directly to the UnlockGC func.
 *
 ***********************************************************************/
#ifdef XTHREADS
#define MTXUnlockGC(pGC, gcID)  UnlockGC (pGC, gcID)
#else
#define MTXUnlockGC(pGC, gcID) /* nothing */
#endif 


/***********************************************************************
 *
 * Lock and verify two GCs.
 *
 ***********************************************************************/

#define LOCK_AND_VERIFY_TWO_GCS(pGC1, pGC2, gcID1, gcID2, client, conflictMask)\
{									\
    int _error;								\
									\
    if (_error = LockAndVerifyTwoGCs(&(pGC1), &(pGC2), (gcID1), (gcID2),\
				      (client)))			\
    {									\
	return (_error);						\
    }									\
    POQ_SET_NULL_CONFLICT((client));					\
									\
    POQLock((client), (conflictMask));					\
}
/***********************************************************************
 *
 * STX to MTX merge versions of Lock and verify two GCs.
 *
 ***********************************************************************/
#ifdef XTHREADS
#define MTX_LOCK_AND_VERIFY_TWO_GCS LOCK_AND_VERIFY_TWO_GCS
#else
#define MTX_LOCK_AND_VERIFY_TWO_GCS(pGC1, pGC2, gcID1, gcID2, client, conflictMask)		\
    VERIFY_GC(pGC1, gcID1, client);					\
    VERIFY_GC(pGC2, gcID2, client)
#endif


/***********************************************************************
 *
 * Unlock two GCs.
 *
 ***********************************************************************/

#define UNLOCK_TWO_GCS(pGC1, pGC2, gcID1, gcID2, client)		\
{									\
    POQUnlock((client));						\
    UnlockTwoGCs ((pGC1), (pGC2), (gcID1), (gcID2));			\
}
/***********************************************************************
 *
 * STX to MTX merge versions of Unlock two GCs.
 *
 ***********************************************************************/
#ifdef XTHREADS
#define MTX_UNLOCK_TWO_GCS UNLOCK_TWO_GCS
#else
#define MTX_UNLOCK_TWO_GCS(pGC1, pGC2, gcID1, gcID2, client) /* nothing */
#endif


/***********************************************************************
 *
 * Lock and verify a colormap.
 *
 * STX to MTX merge version of Lock and verify a colormap.
 *
 ***********************************************************************/
#ifdef XTHREADS
#define MTX_REP_LOCK_AND_VERIFY_COLORMAP(pMap, mapID, client, conflictMask) \
{									\
    int _error;								\
									\
    if (_error = LockAndVerifyColormap(&(pMap), (mapID), (client)))	\
    {									\
	MTXReturnPooledMessage;						\
	return (_error);						\
    }									\
    POQ_SET_NULL_CONFLICT((client));					\
									\
    POQLock((client), (conflictMask));					\
}
#define MTX_LOCK_AND_VERIFY_COLORMAP(pMap, mapID, client, conflictMask)	\
{									\
    int _error;								\
									\
    if (_error = LockAndVerifyColormap(&(pMap), (mapID), (client)))	\
    {									\
	return (_error);						\
    }									\
    POQ_SET_NULL_CONFLICT((client));					\
									\
    POQLock((client), (conflictMask));					\
}
#else
#define MTX_REP_LOCK_AND_VERIFY_COLORMAP MTX_LOCK_AND_VERIFY_COLORMAP
#define MTX_LOCK_AND_VERIFY_COLORMAP(pMap, mapID, client, conflictMask)	\
{									\
    pMap = (ColormapPtr )LookupIDByType(mapID, RT_COLORMAP);		\
    if (!pMap)								\
    {									\
	client->errorValue = mapID;					\
	return (BadColor);						\
    }									\
}
#endif


/***********************************************************************
 *
 * Unlock a colormap.
 *
 ***********************************************************************/

#define UNLOCK_COLORMAP(pMap, mapID, client)				\
{									\
    POQUnlock((client));						\
    UnlockColormap ((pMap), (mapID));					\
}
/***********************************************************************
 *
 * STX to MTX merge versions of Unlock a colormap.
 *
 ***********************************************************************/
#ifdef XTHREADS
#define MTX_UNLOCK_COLORMAP UNLOCK_COLORMAP
#else
#define MTX_UNLOCK_COLORMAP(pMap, mapID, client) /* nothing */
#endif


/***********************************************************************
 *
 * Lock and verify a font.
 *
 ***********************************************************************/

#define LOCK_AND_VERIFY_FONT(pFont, fontID, client, conflictMask)	\
{									\
    int _error;								\
									\
    if (_error = LockAndVerifyFont(&(pFont), (fontID), (client), FALSE))\
    {									\
	return (_error);						\
    }									\
    POQ_SET_NULL_CONFLICT((client));					\
									\
    POQLock((client), (conflictMask));					\
}

/***********************************************************************
 *
 * Lock and verify a font.  If the font is not in the RDB, try to
 * lookup the GC containing the font.
 *
 ***********************************************************************/

/***********************************************************************
 *
 * STX to MTX merge versions of Lock and verify a font.  If the font is 
 * not in the RDB, try to lookup the GC containing the font.
 *
 ***********************************************************************/
#ifdef XTHREADS
#define MTX_REP_LOCK_AND_VERIFY_GC_FONT(pFont, pGC, fontID, client, conflictMask)\
{									\
    int _error;								\
									\
    if (_error = LockAndVerifyFont(&(pFont), (fontID), (client), TRUE))	\
    {									\
	MTXReturnPooledMessage;						\
	return (_error);						\
    }									\
    POQ_SET_NULL_CONFLICT((client));					\
									\
    POQLock((client), (conflictMask));					\
}
#define MTX_LOCK_AND_VERIFY_GC_FONT(pFont, pGC, fontID, client, conflictMask)\
{									\
    int _error;								\
									\
    if (_error = LockAndVerifyFont(&(pFont), (fontID), (client), TRUE))	\
    {									\
	return (_error);						\
    }									\
    POQ_SET_NULL_CONFLICT((client));					\
									\
    POQLock((client), (conflictMask));					\
}
#else
#define MTX_REP_LOCK_AND_VERIFY_GC_FONT MTX_LOCK_AND_VERIFY_GC_FONT
#define MTX_LOCK_AND_VERIFY_GC_FONT(pFont, pGC, fontID, client, conflictMask)\
{									\
    pFont = (FontPtr)LookupIDByType(fontID, RT_FONT);			\
    if (!pFont)								\
    {									\
	/* can't use VERIFY_GC because it might return BadGC */		\
	pGC = (GC *) LookupIDByType(fontID, RT_GC);			\
	if (!pGC)							\
	{								\
	    client->errorValue = fontID;				\
	    return(BadFont);  /* procotol spec says only error is BadFont */\
	}								\
	pFont = pGC->font;						\
    }									\
}
#endif


/***********************************************************************
 *
 * Unlock a font.
 *
 ***********************************************************************/

#define UNLOCK_FONT(pFont, fontID, client)				\
{									\
    POQUnlock((client));						\
    UnlockFont ((pFont), (fontID));					\
}
/***********************************************************************
 *
 * STX to MTX merge version of Unlock a font.
 *
 ***********************************************************************/
#ifdef XTHREADS
#define MTX_UNLOCK_FONT UNLOCK_FONT
#else
#define MTX_UNLOCK_FONT(pFont, fontID, client) /* nothing */
#endif


/***********************************************************************
 *
 * Lock and verify a cursor.
 *
 * STX to MTX merge versions of Lock and verify a cursor.
 *
 ***********************************************************************/
#ifdef XTHREADS
#define MTX_DEV_LOCK_AND_VERIFY_CURSOR(pCursor, cursorID, client, conflictMask) \
{									\
    int _error;								\
									\
    if (_error = LockAndVerifyCursor(&(pCursor), (cursorID), (client)))	\
    {									\
	MTX_UNLOCK_DEVICES();						\
	return (_error);						\
    }									\
    POQ_SET_NULL_CONFLICT((client));					\
									\
    POQLock((client), (conflictMask));					\
}
#define MTX_LOCK_AND_VERIFY_CURSOR(pCursor, cursorID, client, conflictMask) \
{									\
    int _error;								\
									\
    if (_error = LockAndVerifyCursor(&(pCursor), (cursorID), (client)))	\
    {									\
	return (_error);						\
    }									\
    POQ_SET_NULL_CONFLICT((client));					\
									\
    POQLock((client), (conflictMask));					\
}
#else
#define MTX_DEV_LOCK_AND_VERIFY_CURSOR MTX_LOCK_AND_VERIFY_CURSOR
#define MTX_LOCK_AND_VERIFY_CURSOR(pCursor, cursorID, client, conflictMask)\
{									\
    pCursor = (CursorPtr)LookupIDByType(cursorID, RT_CURSOR);		\
    if (!pCursor)							\
    {									\
	client->errorValue = stuff->id;					\
	return (BadCursor);						\
    }									\
}
#endif


/***********************************************************************
 *
 * Unlock a cursor.
 *
 ***********************************************************************/

#define UNLOCK_CURSOR(pCursor, cursorID, client)			\
{									\
    POQUnlock((client));						\
    UnlockCursor ((pCursor), (cursorID));				\
}

/***********************************************************************
 *
 * Lock the pending operation queue.
 *
 * STX to MTX versions of Lock the pending operation queue.
 *
 ***********************************************************************/

#ifdef XTHREADS
/* no return - _REP version same as without */
#define MTX_REP_LOCK_PENDING_OPERATION_QUEUE MTX_LOCK_PENDING_OPERATION_QUEUE
#define MTX_LOCK_PENDING_OPERATION_QUEUE(client, conflictMask)		\
{									\
    POQ_SET_ALL_CONFLICT((client));					\
    POQLock((client), (conflictMask));					\
}
#else
#define MTX_REP_LOCK_PENDING_OPERATION_QUEUE MTX_LOCK_PENDING_OPERATION_QUEUE
#define MTX_LOCK_PENDING_OPERATION_QUEUE(client, conflictMask) /* nothing */
#endif


/***********************************************************************
 *
 * Unlock the pending operation queue.
 *
 ***********************************************************************/

#define UNLOCK_PENDING_OPERATION_QUEUE(client)				\
{									\
    POQUnlock((client));						\
}
/***********************************************************************
 *
 * STX to MTX versions of Unlock the pending operation queue.
 *
 ***********************************************************************/
#ifdef XTHREADS
#define MTX_UNLOCK_PENDING_OPERATION_QUEUE UNLOCK_PENDING_OPERATION_QUEUE
#else
#define MTX_UNLOCK_PENDING_OPERATION_QUEUE(client) /* nothing */
#endif


/***********************************************************************
 *
 * Check to see if a render operation needs to come up for air.
 *
 ***********************************************************************/

#define MAYBE_COME_UP_FOR_AIR()                                         \
{									\
    POQComeUpForAir();							\
}

/***********************************************************************/

#endif /* MTXLOCK_H */
