 
/************************************************************************
 *
 * POQ.c --
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

#ifdef XTHREADS

#include "X.h"
#include "Xproto.h"
#include "windowstr.h"
#include "dixstruct.h"
#include "gcstruct.h"
#include "misc.h"
#include "scrnintstr.h"
#include "message.h"
#include "POQ.h"


#define MAX_POQ_ELEMENTS  MAXCLIENTS + MAXDITS 

typedef struct _POQElement *POQElementPtr;

typedef struct _POQElement {
    Mask 	   conflictMask;  /* conflict bits */
    int            status;        /* indicates current status of thread */
    ConflictPtr	   pConflict;	  /* pointer to conflict rec */
    ClientPtr      pClient;       /* pointer back to client */
    ExtensionPtr   pExtInfo;	  /* pointer to extension information */
    POQElementPtr  pNext;         /* pointer to next element */
    POQElementPtr  pPrev;         /* pointer to prev element */
} POQElementRec;


static X_COND_TYPE  POQCondition;
static X_MUTEX_TYPE POQMutex;

static POQElementRec POQTail;
static POQElementPtr pPOQTail;

static POQElementRec POQGrabElement;
static POQElementPtr pPOQGrabElement;
static POQElementRec POQGrabTail;
static POQElementPtr pPOQGrabTail;

static POQElementRec POQElements[MAX_POQ_ELEMENTS];
static ConflictRec POQConflicts[MAX_POQ_ELEMENTS];

static ClientPtr grabClient;

static int ServerGrab;
static int POQWaiting;

static int (* CheckRegionConflicts[MAXSCREENS])();
static int (* CheckExtensionConflicts[MAXEXTENSIONS])();
static int DefExtConflictFunc();
static int CheckRegions1Bit();
static int CheckRegions8Bit();
static int CheckRegions32Bit();

RegionRec VisibleRegion = {    /* entire visible region */
                          0,
                          0,
                          MAXSHORT, MAXSHORT,
                          NULL
};

#define WORD_ALIGN_32_LEFT(x)	((x) & 0xffe0)
#define WORD_ALIGN_32_RIGHT(x)	(((x) + 32) & 0xffe0)
#define WORD_ALIGN_8_LEFT(x)	((x) & 0xfff8)
#define WORD_ALIGN_8_RIGHT(x)	(((x) + 8) & 0xfff8)

#define IS_GRAB_CLIENT(pPOQElement)  ((pPOQElement)->pClient == grabClient)
#define IS_DIT(pPOQElement)  ((pPOQElement)->pClient->index >= MAXCLIENTS)

#define BLOCKED  (1L<<0)
#define RUNNING  (1L<<1)
#define WAITING  (1L<<2)

/***********************************************************************
 *
 * Add an element to the pending operation queue.
 * The POQ mutex must be aquired before calling this macro.
 *
 ***********************************************************************/

#define POQ_ADD_ELEMENT(pPOQElement)					\
{   	                                                        	\
    if (ServerGrab && (IS_GRAB_CLIENT(pPOQElement) || IS_DIT(pPOQElement)))\
    {									\
	/* 								\
	 * if the server is grabbed, insert at tail of GrabServer queue.\
	 */								\
        (pPOQElement)->pNext = pPOQGrabTail->pNext; 			\
        (pPOQElement)->pPrev = pPOQGrabTail;       			\
    }									\
    else								\
    {									\
	/* 								\
	 * if the server is not grabbed, insert at tail of POQ.		\
	 */								\
        (pPOQElement)->pNext = pPOQTail->pNext;   			\
        (pPOQElement)->pPrev = pPOQTail;          			\
    }									\
    (pPOQElement)->pPrev->pNext = (pPOQElement);			\
    (pPOQElement)->pNext->pPrev = (pPOQElement);			\
}


/***********************************************************************
 *
 * Check the pending operation queue for conflicts.
 * The POQ mutex must be aquired before calling this macro.
 *
 ***********************************************************************/

#define POQ_CHECK_FOR_CONFLICTS(pPOQElement)				\
{									\
    register POQElementPtr _currentPOQElement;				\
    register POQElementPtr _currentPOQTail;				\
    register Bool moveToHead = FALSE;					\
									\
    _currentPOQElement = (pPOQElement)->pNext;				\
    _currentPOQTail = (pPOQElement)->pPrev;				\
									\
    /*------------------------------------------------			\
     * If there are any other elements on the queue, 			\
     * traverse it and test for conflicts.				\
     *-----------------------------------------------*/			\
									\
    while (_currentPOQElement != _currentPOQTail)			\
    {									\
        if (_currentPOQElement->conflictMask & (pPOQElement)->conflictMask)\
        {								\
	    if (IS_DIT(pPOQElement) &&					\
		(!(_currentPOQElement->status & RUNNING))) 		\
	    {								\
		moveToHead = TRUE;					\
	    }								\
	    else if (CheckForConflicts((pPOQElement), _currentPOQElement))\
            {								\
		/* tell blocking thread that someone is waiting */	\
		_currentPOQElement->status |= WAITING;			\
									\
                POQWaiting = 1;						\
                X_COND_WAIT(&POQCondition, &POQMutex);			\
									\
		/* need to start over again */				\
                _currentPOQElement = (pPOQElement);			\
		moveToHead = FALSE;					\
            }								\
        }								\
        _currentPOQElement = _currentPOQElement->pNext;			\
    }									\
    /* indicate that thread is now runnable */				\
    (pPOQElement)->status |= RUNNING;					\
									\
    if (moveToHead)							\
    {									\
        (pPOQElement)->pNext->pPrev = (pPOQElement)->pPrev;		\
        (pPOQElement)->pPrev->pNext = (pPOQElement)->pNext;		\
									\
        (pPOQElement)->pNext = _currentPOQTail;				\
        (pPOQElement)->pPrev = _currentPOQTail->pPrev;			\
									\
        (pPOQElement)->pPrev->pNext = (pPOQElement);			\
        (pPOQElement)->pNext->pPrev = (pPOQElement);			\
    }									\
}

/***********************************************************************
 *
 * Delete an element from the pending operation queue.
 * The POQ mutex must be aquired before calling this macro.
 *
 ***********************************************************************/

#define POQ_DELETE_ELEMENT(pPOQElement)					\
{									\
    (pPOQElement)->pNext->pPrev = (pPOQElement)->pPrev;			\
    (pPOQElement)->pPrev->pNext = (pPOQElement)->pNext;			\
    (pPOQElement)->status = 0;						\
	                                                                \
    if (POQWaiting)							\
    {									\
        POQWaiting = 0;							\
        X_COND_BROADCAST(&POQCondition);				\
    }									\
}


/***********************************************************************/

void
POQInitializeMonitor()

/*
 * Summary
 *
 *      Initialize the pending operation queue monitor.
 *
 ***********************************************************************/
{
    int i;

    /*--------------------------------------------------------
     * initialize pending operation queue condition and mutex.
     *--------------------------------------------------------*/

    if (serverGeneration == 1)
    {
        X_MUTEX_INIT(&POQMutex, X_MUTEX_ATTR_DEFAULT);
        X_COND_INIT(&POQCondition, X_COND_ATTR_DEFAULT);

        X_SET_SYMNAME(&POQMutex,"POQ_M");
        X_SET_SYMNAME(&POQCondition,"POQ_C");
    }

    POQWaiting = 0;

    grabClient = NullClient;
    ServerGrab = 0;

    /*------------------------------------------
     * initialize the head element for the POQ.
     *------------------------------------------*/

    pPOQTail = &POQTail;
    pPOQTail->conflictMask = 0;
    pPOQTail->status = 0;
    pPOQTail->pConflict = NULL;
    pPOQTail->pClient = NULL;
    pPOQTail->pExtInfo = NULL;
    pPOQTail->pNext = pPOQTail;
    pPOQTail->pPrev = pPOQTail;

    /*------------------------------------
     * initialize the grab server queue.
     *------------------------------------*/

    pPOQGrabTail = &POQGrabTail;
    pPOQGrabTail->conflictMask = 0;
    pPOQGrabTail->status = 0;
    pPOQGrabTail->pConflict = NULL;
    pPOQGrabTail->pClient = NULL;
    pPOQGrabTail->pExtInfo = NULL;
    pPOQGrabTail->pNext = pPOQGrabTail;
    pPOQGrabTail->pPrev = pPOQGrabTail;

    /*------------------------------------
     * initialize the grab server element.
     *------------------------------------*/

    pPOQGrabElement = &POQGrabElement;
    pPOQGrabElement->conflictMask = CM_XGrabServer;
    pPOQGrabElement->status = 0;
    pPOQGrabElement->pConflict = NULL;
    pPOQGrabElement->pClient = NULL;
    pPOQGrabElement->pExtInfo = NULL;
    pPOQGrabElement->pNext = pPOQGrabElement;
    pPOQGrabElement->pPrev = pPOQGrabElement;

    /*---------------------------------------------
     * The RegionConflictFunc should be set by the 
     * DDX implementor after adding a new screen in
     * InitOutput.  This loop initializes the 
     * RegionConflictFuncs assuming the worst case.
     *---------------------------------------------*/
    for (i = 0; i < MAXSCREENS; i++)
    {
	POQSetRegionConflictFunc(i, CheckRegions1Bit);
    }

    for (i = 0; i < MAXEXTENSIONS; i++)
    {
        POQSetExtConflictFunc(i, DefExtConflictFunc);
    }
}

/***********************************************************************/

void
POQDestroyMonitor()

/*
 * Summary
 *
 *      Destroy the pending operation queue monitor.
 *
 ***********************************************************************/
{
    /* nothing to do here except destroy POQ mutex and condition ??? */
}

/***********************************************************************/

void
POQInitClient(client)
    ClientPtr client;

/*
 * Summary
 *
 * 	Initialize the client POQ element and conflict rec.
 *
 ***********************************************************************/
{
    int cid = client->index;

    client->pConflict = (pointer)&POQConflicts[cid];
    POQElements[cid].pConflict = (ConflictPtr)client->pConflict;
    POQElements[cid].conflictMask = 0;
    POQElements[cid].status = 0;
    POQElements[cid].pClient = client;
    POQElements[cid].pExtInfo = NULL;
}

/***********************************************************************/

static int
CheckRegions1Bit(pRegion1, pRegion2)
    RegionPtr  pRegion1;
    RegionPtr  pRegion2;

/*
 * Summary
 *
 *      Check regions for overlap after x values have been adjusted
 *	to word boundaries.
 *      Returns 1 if regions overlap.
 *	Returns 0 if no region conflicts exist.
 *
 ***********************************************************************/
{
    register BoxPtr pExt1, pExt2;


    if ((!pRegion1) || (!pRegion2))
    {
	/* if either region is NULL, no conflict exists. */
	return (0); 
    }

    pExt1 = &pRegion1->extents;
    pExt2 = &pRegion2->extents;

    if (!((WORD_ALIGN_32_RIGHT(pExt1->x2) <= WORD_ALIGN_32_LEFT(pExt2->x1)) ||
          (WORD_ALIGN_32_LEFT(pExt1->x1) >= WORD_ALIGN_32_RIGHT(pExt2->x2)) ||
          (pExt1->y2 <= pExt2->y1)  ||
          (pExt1->y1 >= pExt2->y2) ) )
    {
        return (1);
    }
    return (0);
}

/***********************************************************************/

static int
CheckRegions8Bit(pRegion1, pRegion2)
    RegionPtr  pRegion1;
    RegionPtr  pRegion2;

/*
 * Summary
 *
 *      Check regions for overlap after x values have been adjusted
 *	to word boundaries.
 *      Returns 1 if regions overlap.
 *	Returns 0 if no region conflicts exist.
 *
 ***********************************************************************/
{
    register BoxPtr pExt1, pExt2;

    if ((!pRegion1) || (!pRegion2))
    {
	/* if either region is NULL, no conflict exists. */
	return (0); 
    }

    pExt1 = &pRegion1->extents;
    pExt2 = &pRegion2->extents;

    if (!((WORD_ALIGN_8_RIGHT(pExt1->x2) <= WORD_ALIGN_8_LEFT(pExt2->x1)) ||
          (WORD_ALIGN_8_LEFT(pExt1->x1) >= WORD_ALIGN_8_RIGHT(pExt2->x2)) ||
          (pExt1->y2 <= pExt2->y1)  ||
          (pExt1->y1 >= pExt2->y2) ) )
    {
        return (1);
    }
    return (0);
}

/***********************************************************************/

static int
CheckRegions32Bit(pRegion1, pRegion2)
    RegionPtr  pRegion1;
    RegionPtr  pRegion2;

/*
 * Summary
 *
 *      Check regions for overlap after x values have been adjusted
 *	to word boundaries.
 *      Returns 1 if regions overlap.
 *	Returns 0 if no region conflicts exist.
 *
 ***********************************************************************/
{
    register BoxPtr pExt1, pExt2;

    if ((!pRegion1) || (!pRegion2))
    {
	/* if either region is NULL, no conflict exists. */
	return (0); 
    }

    pExt1 = &pRegion1->extents;
    pExt2 = &pRegion2->extents;

    if (!((pExt1->x2 <= pExt2->x1)  || (pExt1->x1 >= pExt2->x2)  ||
          (pExt1->y2 <= pExt2->y1)  || (pExt1->y1 >= pExt2->y2) ) )
    {
	register BoxPtr pRect1, pRect2, pCurRect;
	register BoxPtr pRectEnd1, pRectEnd2;


	pRect1 = REGION_RECTS(pRegion1);
	pRect2 = REGION_RECTS(pRegion2);

	pRectEnd1 = pRect1 + REGION_NUM_RECTS(pRegion1);
	pRectEnd2 = pRect2 + REGION_NUM_RECTS(pRegion2);

	while ((pRect1->y2 <= pRect2->y1) && (pRect1 != pRectEnd1))
	{
	    pRect1++; /* find fisrt band that intersects */
	}

	for (; pRect1 != pRectEnd1; pRect1++)
	{
	    while ((pRect2->y2 <= pRect1->y1) && (pRect2 != pRectEnd2))
	    {
	        pRect2++; 
	    }

	    for (pCurRect = pRect2; 
		(pCurRect->y1 < pRect1->y2) && (pCurRect != pRectEnd2); 
		 pCurRect++)
	    {
		if (!((pRect1->x2 <= pCurRect->x1)  || 
		      (pRect1->x1 >= pCurRect->x2)  ||
		      (pRect1->y2 <= pCurRect->y1)  || 
		      (pRect1->y1 >= pCurRect->y2) ) )
		{
            	    return (1);
		}
	    }
        }
    }
    return (0);
}

/***********************************************************************/

void
POQSetRegionConflictFunc(screenNum, RegionConflictFunc)
    int screenNum;
    int (* RegionConflictFunc)();

/*
 * Summary
 *
 * 	Set the conflict function for the screen.
 *
 ***********************************************************************/
{
    CheckRegionConflicts[screenNum] = RegionConflictFunc;
}

/***********************************************************************/

void
POQSelectRegionConflictType(screenNum, conflictType)
    int screenNum;
    int conflictType;
/*
 * Summary
 *
 *      Given the conflict type, set the conflict function for the screen.
 *
 ***********************************************************************/
{
    switch (conflictType)
    {
	case POQ_USE_GRAPHICS_ACCEL:
	case POQ_32_BITS_PER_PIXEL:
	case POQ_24_BITS_PER_PIXEL:
            POQSetRegionConflictFunc(screenNum, CheckRegions32Bit);
	    break;

	case POQ_8_BITS_PER_PIXEL:
            POQSetRegionConflictFunc(screenNum, CheckRegions8Bit);
	    break;

	default: /* POQ_1_BIT_PER_PIXEL */
            POQSetRegionConflictFunc(screenNum, CheckRegions1Bit);
	    break;
    }
}

/***********************************************************************/

void
POQSetExtConflictFunc(extID, ExtConflictFunc)
    int extID;
    int (* ExtConflictFunc)();

/*
 * Summary
 *
 * 	Set the extension conflict function for an extension.
 *
 ***********************************************************************/
{
    CheckExtensionConflicts[extID] = ExtConflictFunc;
}

/***********************************************************************/

static int
DefExtConflictFunc(pPOQElement, currentPOQElement)
    POQElementPtr    pPOQElement;
    POQElementPtr    currentPOQElement;

/*
 * Summary
 *
 *	Default extension conflict function.
 *	Just returns no conflict.
 *
 ***********************************************************************/
{
    ErrorF ("DefExtConflictFunc: No extension bits defined.");
    return (0);
}

#ifdef USE_SOFTWARE_CURSOR

/***********************************************************************/

static int
CursorIsInRegion(pRegion)
    RegionPtr  pRegion;

/*
 * Summary
 *
 *      Check pRegion for overlap with the current cursor location.
 *      Returns 1 if region overlap.
 *      Returns 0 if no conflict exist.
 *
 ***********************************************************************/
{
    RegionRec CursorLockReg;
    ScreenPtr pScreen = screenInfo.screens[0];
    int result;

    if (!pRegion)
    {
        return(0);
    }
    miSpriteCalculateCursorRegion(pScreen, &CursorLockReg);
    result = (* CheckRegionConflicts[0])(pRegion, &CursorLockReg);
    return (result);
}

#endif

/***********************************************************************/

static int
CheckForConflicts(pPOQElement, currentPOQElement)
    POQElementPtr    pPOQElement;
    POQElementPtr    currentPOQElement;

/*
 * Summary
 *
 *      Check the pending operation queue for conflicts.
 *	Returns 1 if a conflict exists.
 *	Returns 0 if no conflicts.
 *
 ***********************************************************************/
{
    register Mask index;
    register Mask mask;
    register Mask curmask;
    register Bool NeedToCheckRegions = FALSE;


    mask = pPOQElement->conflictMask;
    curmask = currentPOQElement->conflictMask;

    if ((mask & POQ_GRAB_SERVER) || (curmask & POQ_GRAB_SERVER))
    {
	return (1);
    }

    while (mask) 
    {
	index = lowbit (mask);
	mask &= ~index;
	switch (index)
	{
	    case POQ_NO_CONFLICT:
	        break;

	    /*--------------
	     * Read - Write 
	     *-------------*/

	    case POQ_R_HIERARCHY:
		if (curmask & POQ_W_HIERARCHY)
		{
		    return (1);
		}
	        break;

	    case POQ_W_HIERARCHY:
		if (curmask & (POQ_R_HIERARCHY | POQ_W_HIERARCHY))
		{
		    return (1);
		}
	        break;

	    case POQ_R_GEOMETRY:
		if (curmask & POQ_W_GEOMETRY)
		{
                    if ((currentPOQElement->pConflict->windowMode != 
                         POQ_SINGLE_WINDOW) || 
                        (pPOQElement->pConflict->windowMode != 
                         POQ_SINGLE_WINDOW) ||
                        (currentPOQElement->pConflict->pWindow ==
                         pPOQElement->pConflict->pWindow))
                    {
                        return (1);
                    }
		}
	        break;

	    case POQ_W_GEOMETRY:
		if (curmask & (POQ_R_GEOMETRY | POQ_W_GEOMETRY))
		{
                    if ((currentPOQElement->pConflict->windowMode != 
                         POQ_SINGLE_WINDOW) || 
                        (pPOQElement->pConflict->windowMode != 
                         POQ_SINGLE_WINDOW) ||
                        (currentPOQElement->pConflict->pWindow ==
                         pPOQElement->pConflict->pWindow))
                    {
                        return (1);
                    }
		}
	        break;

	    case POQ_R_COLORMAP:
		if (curmask & POQ_W_COLORMAP)
		{
		    return (1);
		}
	        break;

	    case POQ_W_COLORMAP:
		if (curmask & (POQ_R_COLORMAP | POQ_W_COLORMAP))
		{
		    return (1);
		}
	        break;

	    case POQ_R_EVENT_PROP:
		if (curmask & POQ_W_EVENT_PROP)
		{
		    return (1);
		}
	        break;

	    case POQ_W_EVENT_PROP:
		if (curmask & (POQ_R_EVENT_PROP | POQ_W_EVENT_PROP))
		{
		    return (1);
		}
	        break;

	    case POQ_R_SCREENSAVER:
		if (curmask & POQ_W_SCREENSAVER)
		{
		    return (1);
		}
		break;

	    case POQ_W_SCREENSAVER:
		if (curmask & (POQ_R_SCREENSAVER | POQ_W_SCREENSAVER))
		{
		    return (1);
		}
		break;

	    case POQ_R_FONTPATH:
		if (curmask & POQ_W_FONTPATH)
		{
		    return (1);
		}
		break;

	    case POQ_W_FONTPATH:
		if (curmask & (POQ_R_FONTPATH | POQ_W_FONTPATH))
		{
		    return (1);
		}
		break;

	    /*------------
	     * Write Only
	     *-----------*/

	    case POQ_X_CURSOR:
		if (curmask & POQ_X_CURSOR)
		{
		    return (1);
		}
#ifdef USE_SOFTWARE_CURSOR
                if (curmask & POQ_X_RENDER)
                {
                    if (CursorIsInRegion(currentPOQElement->pConflict->pRegion))
                    {
                        return (1);
                    }
                }
#endif
	        break;

	    case POQ_X_RENDER:
#ifdef USE_SOFTWARE_CURSOR
		if (pPOQElement->conflictMask & POQ_X_CURSOR)
		{
		    /* If the POQ_X_CURSOR bit is set, 
		     * has already been handled above.
		     */
		    break;
		}
		if (curmask & POQ_X_CURSOR)
		{
		    if (CursorIsInRegion(pPOQElement->pConflict->pRegion))
		    {
		        return (1);
		    }
		}
		else 
#endif
		if (curmask & POQ_X_RENDER)
		{
		    NeedToCheckRegions = TRUE;
		}
	        break;

            case POQ_X_ICCCM:
                if (curmask & POQ_X_ICCCM)
                {
                    return (1);
                }
                break;

            case POQ_X_SERVER:
                if (curmask & POQ_X_SERVER)
                {
                    return (1);
                }
                break;

            case POQ_X_FONT:
                if (curmask & POQ_X_FONT)
                {
                    return (1);
                }
                break;

            case POQ_CONFIGURE:
		break;

            case POQ_EXTENSION_BIT:
		if (curmask & POQ_EXTENSION_BIT)
		{
		    if ((pPOQElement->pExtInfo && 
			 currentPOQElement->pExtInfo) &&
			(pPOQElement->pExtInfo->extID ==
		         currentPOQElement->pExtInfo->extID) &&
		        (pPOQElement->pExtInfo->extConflictMask &
			 currentPOQElement->pExtInfo->extConflictMask))
		    {
			int extID = pPOQElement->pExtInfo->extID;
			if ((* CheckExtensionConflicts[extID])
					(pPOQElement, currentPOQElement))
                        {
                            return (1);
			}
                    }
		}
                break;

	    default:
		ErrorF ("CheckForConflicts: undefined conflict bit \n");
		break;
	}
    }

    if (NeedToCheckRegions)
    {
	if (pPOQElement->conflictMask & POQ_CONFIGURE)
	{
	    /*-----------------------------------------------------------
	     * This is special case code for ConfigureWindow requests.
	     * The generation of the configure region is posponed until
	     * a region conflict is detected.  This way, regions are
	     * not computed needlessly and proper locking of the configure
	     * window can occur.  The configure request will set the 
	     * POQ_CONFIGURE bit in the conflict mask to indicate that a 
	     * region should be generated when a render conflict occurs.
	     *-----------------------------------------------------------*/

	    CalculateConfigureRegion(pPOQElement->pClient, 
				     pPOQElement->pConflict->pWindow, 
				     pPOQElement->pConflict->pRegion);

	    /*-------------------------------------------------
	     * Clear the POQ_CONFIGURE bit after the configure 
	     * region is generated.
	     *-------------------------------------------------*/
	    pPOQElement->conflictMask &= (~POQ_CONFIGURE);
	}

	if (currentPOQElement->conflictMask & POQ_CONFIGURE)
	{
	    /*-----------------------------------------------------
	     * If the conflicting request has the POQ_CONFIGURE bit
	     * set, don't try to generate a region, simply wait for 
	     * the configure request to finish.
	     *-----------------------------------------------------*/
	    return (1);
	}

        if ((* CheckRegionConflicts[0])(pPOQElement->pConflict->pRegion, 
					currentPOQElement->pConflict->pRegion))
        {
            return (1);
        }
    }
    return (0);
}

/***********************************************************************/

void
POQLock(client, conflictMask)
    ClientPtr	client;
    Mask        conflictMask;

/*
 * Summary
 *
 *	Add the client element to the pending operation queue.
 *      Check the pending operation queue for conflicts.
 *
 ***********************************************************************/
{
    register POQElementPtr pPOQElement = &POQElements[client->index];

    X_MUTEX_LOCK(&POQMutex);

    pPOQElement->pConflict = (ConflictPtr)client->pConflict;
    pPOQElement->conflictMask = conflictMask;

    POQ_ADD_ELEMENT(pPOQElement);

    POQ_CHECK_FOR_CONFLICTS(pPOQElement);

    X_MUTEX_UNLOCK(&POQMutex);
}

/***********************************************************************/

void
POQUnlock(client)
    ClientPtr client;

/*
 * Summary
 *
 *      Delete an element from anywhere in the POQ.
 *
 ***********************************************************************/
{
    register POQElementPtr pPOQElement = &POQElements[client->index];

    TRANSFER_LOCAL_MESSAGES_TO_GLOBAL(client);

    X_MUTEX_LOCK(&POQMutex);

    POQ_DELETE_ELEMENT(pPOQElement);

    X_MUTEX_UNLOCK(&POQMutex);
}

/***********************************************************************/

void
POQComeUpForAir()

/*
 * Summary
 *
 *	Called from DDX during long protocol requests.
 *
 ***********************************************************************/
{
    ClientPtr client;  
    register POQElementPtr pPOQElement;
    extern X_KEY_TYPE ClientKey;


    X_GETSPECIFIC(ClientKey, (void *)&client);

    pPOQElement = &POQElements[client->index];

    X_MUTEX_LOCK(&POQMutex);

    if (pPOQElement->status & WAITING)
    {
	POQ_DELETE_ELEMENT(pPOQElement);

	POQ_ADD_ELEMENT(pPOQElement);

	POQ_CHECK_FOR_CONFLICTS(pPOQElement);
    }
    X_MUTEX_UNLOCK(&POQMutex);
}

/***********************************************************************/

void
POQLockSuspend()

/*
 * Summary
 *
 *
 ***********************************************************************/
{
    ClientPtr client;  
    register POQElementPtr pPOQElement;
    extern X_KEY_TYPE ClientKey;


    X_GETSPECIFIC(ClientKey, (void *)&client);

    pPOQElement = &POQElements[client->index];

    X_MUTEX_LOCK(&POQMutex);

    POQ_DELETE_ELEMENT(pPOQElement);

    X_MUTEX_UNLOCK(&POQMutex);
}

/***********************************************************************/

void
POQLockResume()

/*
 * Summary
 *
 *
 ***********************************************************************/
{
    ClientPtr client;  
    register POQElementPtr pPOQElement;
    extern X_KEY_TYPE ClientKey;


    X_GETSPECIFIC(ClientKey, (void *)&client);

    pPOQElement = &POQElements[client->index];

    X_MUTEX_LOCK(&POQMutex);

    POQ_ADD_ELEMENT(pPOQElement);

    POQ_CHECK_FOR_CONFLICTS(pPOQElement);

    X_MUTEX_UNLOCK(&POQMutex);
}

/***********************************************************************/

void
POQGrabServer(client)
    register ClientPtr client;

/*
 * Summary
 *
 *
 ***********************************************************************/

{
    register POQElementPtr pPOQElement = &POQElements[client->index];

    X_MUTEX_LOCK(&POQMutex);

    if ((ServerGrab) && (grabClient == client))
    {
    	X_MUTEX_UNLOCK(&POQMutex);
	return;
    }
    ServerGrab++;

    pPOQElement->conflictMask = CM_XGrabServer;
    pPOQElement->status |= RUNNING;

    POQ_ADD_ELEMENT(pPOQElement);

    POQ_CHECK_FOR_CONFLICTS(pPOQElement);

    /* 
     * Set grabClient only after the grab succeeds 
     */
    grabClient = client;

    /* 
     * Now link in the grabServer element
     */
    pPOQElement->pNext->pPrev = pPOQGrabElement;
    pPOQElement->pPrev->pNext = pPOQGrabElement;
    pPOQGrabElement->pNext = pPOQElement->pNext;
    pPOQGrabElement->pPrev = pPOQElement->pPrev;

    X_MUTEX_UNLOCK(&POQMutex);
}

/***********************************************************************/

void
POQUngrabServer(client)
    register ClientPtr client;

/*
 * Summary
 *
 *
 ***********************************************************************/
{
    TRANSFER_LOCAL_MESSAGES_TO_GLOBAL(client);

    X_MUTEX_LOCK(&POQMutex);

    if ((!ServerGrab) || (grabClient != client))
    {
        X_MUTEX_UNLOCK(&POQMutex);
	return;
    }

    ServerGrab--;
    grabClient = NullClient;

    POQ_DELETE_ELEMENT(pPOQGrabElement);

    X_MUTEX_UNLOCK(&POQMutex);
}

/***********************************************************************/

#endif /* XTHREADS */
