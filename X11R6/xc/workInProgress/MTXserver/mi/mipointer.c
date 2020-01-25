/*
 * mipointer.c
 */

/* $XConsortium: mipointer.c,v 1.5 94/04/17 21:16:06 rob Exp $ */

/*

Copyright (c) 1989  X Consortium

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
X CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of the X Consortium shall not be
used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization from the X Consortium.

Copyright 1992, 1993 Data General Corporation;
Copyright 1992, 1993 OMRON Corporation  

Permission to use, copy, modify, distribute, and sell this software and its
documentation for any purpose is hereby granted without fee, provided that the
above copyright notice appear in all copies and that both that copyright
notice and this permission notice appear in supporting documentation, and that
neither the name OMRON or DATA GENERAL be used in advertising or publicity
pertaining to distribution of the software without specific, written prior
permission of the party whose name is to be used.  Neither OMRON or 
DATA GENERAL make any representation about the suitability of this software
for any purpose.  It is provided "as is" without express or implied warranty.  

OMRON AND DATA GENERAL EACH DISCLAIM ALL WARRANTIES WITH REGARD TO THIS
SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS,
IN NO EVENT SHALL OMRON OR DATA GENERAL BE LIABLE FOR ANY SPECIAL, INDIRECT
OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
OF THIS SOFTWARE.

*/

# define NEED_EVENTS
# include   "X.h"
# include   "Xmd.h"
# include   "Xproto.h"
# include   "mtxlock.h"
# include   "misc.h"
# include   "windowstr.h"
# include   "pixmapstr.h"
# include   "mi.h"
# include   "scrnintstr.h"
# include   "mipointrst.h"
# include   "cursorstr.h"
# include   "dixstruct.h"

static int  miPointerScreenIndex;
static unsigned long miPointerGeneration = 0;

#define GetScreenPrivate(s) ((miPointerScreenPtr) ((s)->devPrivates[miPointerScreenIndex].ptr))
#define SetupScreen(s)	miPointerScreenPtr  pScreenPriv = GetScreenPrivate(s)

/*
 * until more than one pointer device exists.
 */

static miPointerRec miPointer;

static Bool miPointerRealizeCursor (),	    miPointerUnrealizeCursor ();
static Bool miPointerDisplayCursor ();
static void miPointerConstrainCursor (),    miPointerPointerNonInterestBox();
static void miPointerCursorLimits ();
static Bool miPointerSetCursorPosition ();
static void miPointerSetCursor();
static void miPointerCheckScreen();

static Bool miPointerCloseScreen();

static void miPointerMove ();

extern void NewCurrentScreen ();
#ifndef XTHREADS
extern void ProcessInputEvents ();
#else /* XTHREADS */
/*
 * for MTX server
 */
extern Bool miPointerLock ();
extern void miPointerUnlock ();

static void miPointerAbsoluteCursorNoLock ();
static void miPointerUpdateNoLock ();
#endif /* XTHREADS */

Bool
miPointerInitialize (pScreen, spriteFuncs, screenFuncs, waitForUpdate)
    ScreenPtr		    pScreen;
    miPointerSpriteFuncPtr  spriteFuncs;
    miPointerScreenFuncPtr  screenFuncs;
    Bool		    waitForUpdate;
{
    miPointerScreenPtr	pScreenPriv;

    if (miPointerGeneration != serverGeneration)
    {
	miPointerScreenIndex = AllocateScreenPrivateIndex();
	if (miPointerScreenIndex < 0)
	    return FALSE;
	miPointerGeneration = serverGeneration;
    }
    pScreenPriv = (miPointerScreenPtr) xalloc (sizeof (miPointerScreenRec));
    if (!pScreenPriv)
	return FALSE;
    pScreenPriv->spriteFuncs = spriteFuncs;
    pScreenPriv->screenFuncs = screenFuncs;
    /*
     * check for uninitialized methods
     */
    if (!screenFuncs->EnqueueEvent)
	screenFuncs->EnqueueEvent = mieqEnqueue;
    if (!screenFuncs->NewEventScreen)
	screenFuncs->NewEventScreen = mieqSwitchScreen;
    pScreenPriv->waitForUpdate = waitForUpdate;
    pScreenPriv->CloseScreen = pScreen->CloseScreen;
    pScreen->CloseScreen = miPointerCloseScreen;
    pScreen->devPrivates[miPointerScreenIndex].ptr = (pointer) pScreenPriv;
    /*
     * set up screen cursor method table
     */
    pScreen->ConstrainCursor = miPointerConstrainCursor;
    pScreen->CursorLimits = miPointerCursorLimits;
    pScreen->DisplayCursor = miPointerDisplayCursor;
    pScreen->RealizeCursor = miPointerRealizeCursor;
    pScreen->UnrealizeCursor = miPointerUnrealizeCursor;
    pScreen->SetCursorPosition = miPointerSetCursorPosition;
    pScreen->RecolorCursor = miRecolorCursor;
    pScreen->PointerNonInterestBox = miPointerPointerNonInterestBox;
    /*
     * set up the pointer object
     */
    miPointer.pScreen = NULL;
    miPointer.pSpriteScreen = NULL;
    miPointer.pCursor = NULL;
    miPointer.pSpriteCursor = NULL;
    miPointer.limits.x1 = 0;
    miPointer.limits.x2 = 32767;
    miPointer.limits.y1 = 0;
    miPointer.limits.y2 = 32767;
    miPointer.confined = FALSE;
    miPointer.x = 0;
    miPointer.y = 0;
    miPointer.history_start = miPointer.history_end = 0;
#ifdef XTHREADS
    X_MUTEX_INIT(&miPointer.mutex, X_MUTEX_ATTR_DEFAULT);
#ifdef  USE_SOFTWARE_CURSOR
    miPointer.last_thread_id = (X_THREAD_TYPE)-1;
#ifdef USE_MONITOR_MTX
    X_SET_SYMNAME(&(miPointer.mutex),"PTR_M");
#endif /* USE_MONITOR_MTX */
#endif /* USE_SOFTWARE_CURSOS */
#endif /* XTHREADS */
    return TRUE;
}

static Bool
miPointerCloseScreen (index, pScreen)
    int		index;
    ScreenPtr	pScreen;
{
    SetupScreen(pScreen);

    if (pScreen == miPointer.pScreen)
	miPointer.pScreen = 0;
    if (pScreen == miPointer.pSpriteScreen)
	miPointer.pSpriteScreen = 0;
    pScreen->CloseScreen = pScreenPriv->CloseScreen;
    xfree ((pointer) pScreenPriv);
    return (*pScreen->CloseScreen) (index, pScreen);
}

/*
 * DIX/DDX interface routines
 */

static Bool
miPointerRealizeCursor (pScreen, pCursor)
    ScreenPtr	pScreen;
    CursorPtr	pCursor;
{
    Bool ret;
    SetupScreen(pScreen);

#ifdef XTHREADS
    (void)miPointerLock();
#endif /* XTHREADS */
    ret = (*pScreenPriv->spriteFuncs->RealizeCursor) (pScreen, pCursor);
#ifdef XTHREADS
    (void)miPointerUnlock();
#endif /* XTHREADS */
    return ret;
}

static Bool
miPointerUnrealizeCursor (pScreen, pCursor)
    ScreenPtr	pScreen;
    CursorPtr	pCursor;
{
    Bool ret;
    SetupScreen(pScreen);

#ifdef XTHREADS
    (void)miPointerLock();
#endif /* XTHREADS */
    ret = (*pScreenPriv->spriteFuncs->UnrealizeCursor) (pScreen, pCursor);
#ifdef XTHREADS
    (void)miPointerUnlock();
#endif /* XTHREADS */
    return ret;
}

static Bool
miPointerDisplayCursor (pScreen, pCursor)
    ScreenPtr	pScreen;
    CursorPtr	pCursor;
{
    SetupScreen(pScreen);

#ifdef XTHREADS
    (void)miPointerLock();
#endif /* XTHREADS */
    miPointer.pCursor = pCursor;
    miPointer.pScreen = pScreen;
#ifndef XTHREADS
    miPointerUpdate ();
#else /* XTHREADS */
    miPointerUpdateNoLock ();
    (void)miPointerUnlock();
#endif /* XTHREADS */
    return TRUE;
}

static void
miPointerConstrainCursor (pScreen, pBox)
    ScreenPtr	pScreen;
    BoxPtr	pBox;
{
#ifdef XTHREADS
    (void)miPointerLock();
#endif /* XTHREADS */
    miPointer.limits = *pBox;
    miPointer.confined = PointerConfinedToScreen();
#ifdef XTHREADS
    (void)miPointerUnlock();
#endif /* XTHREADS */
}

/*ARGSUSED*/
static void
miPointerPointerNonInterestBox (pScreen, pBox)
    ScreenPtr	pScreen;
    BoxPtr	pBox;
{
    /* until DIX uses this, this will remain a stub */
}

/*ARGSUSED*/
static void
miPointerCursorLimits(pScreen, pCursor, pHotBox, pTopLeftBox)
    ScreenPtr	pScreen;
    CursorPtr	pCursor;
    BoxPtr	pHotBox;
    BoxPtr	pTopLeftBox;
{
    *pTopLeftBox = *pHotBox;
}

static Bool GenerateEvent;

static Bool
miPointerSetCursorPosition(pScreen, x, y, generateEvent)
    ScreenPtr pScreen;
    int       x, y;
    Bool      generateEvent;
{
    SetupScreen (pScreen);

#ifdef XTHREADS
    (void)miPointerLock();
#endif /* XTHREADS */
    GenerateEvent = generateEvent;
    /* device dependent - must pend signal and call miPointerWarpCursor */
    (*pScreenPriv->screenFuncs->WarpCursor) (pScreen, x, y);
#ifndef XTHREADS
    if (!generateEvent)
	miPointerUpdate();
#else /* XTHREADS */
    if (generateEvent)
      {
          xEvent  e;

	  e.u.u.type = MotionNotify;
	  e.u.keyButtonPointer.rootX = x;
	  e.u.keyButtonPointer.rootY = y;
	  e.u.keyButtonPointer.time = GetTimeInMillis ();
	  (*miPointer.pPointer->processInputProc) (&e, miPointer.pPointer, 1);
      }	
    /* if (!generateEvent) 
	miPointerUpdateNoLock();
    */
    miPointerUpdateNoLock();

    miPointerUnlock ();
#endif /* XTHREADS */
    return TRUE;
}

/* Once signals are ignored, the WarpCursor function can call this */

void
miPointerWarpCursor (pScreen, x, y)
    ScreenPtr	pScreen;
    int		x, y;
{
    SetupScreen (pScreen);

    if (miPointer.pScreen != pScreen)
	(*pScreenPriv->screenFuncs->NewEventScreen) (pScreen, TRUE);

    if (GenerateEvent)
    {
	miPointerMove (pScreen, x, y, GetTimeInMillis()
#ifdef XTHREADS
	    , FALSE
#endif /* XTHREADS */
	    ); 
    }
    else
    {
	/* everything from miPointerMove except the event and history */

    	if (!pScreenPriv->waitForUpdate && pScreen == miPointer.pSpriteScreen)
    	{
	    miPointer.devx = x;
	    miPointer.devy = y;
	    (*pScreenPriv->spriteFuncs->MoveCursor) (pScreen, x, y);
    	}
	miPointer.x = x;
	miPointer.y = y;
	miPointer.pScreen = pScreen;
    }
}

/*
 * Pointer/CursorDisplay interface routines
 */

int
miPointerGetMotionBufferSize ()
{
    return MOTION_SIZE;
}

int
miPointerGetMotionEvents (pPtr, coords, start, stop, pScreen)
    DeviceIntPtr    pPtr;
    xTimecoord	    *coords;
    unsigned long   start, stop;
    ScreenPtr	    pScreen;
{
    int		    i;
    int		    count = 0;
    miHistoryPtr    h;

#ifdef XTHREADS
    (void)miPointerLock();
#endif /* XTHREADS */
    for (i = miPointer.history_start; i != miPointer.history_end;)
    {
	h = &miPointer.history[i];
	if (h->event.time >= stop)
	    break;
	if (h->event.time >= start)
	{
	    *coords++ = h->event;
	    count++;
	}
	if (++i == MOTION_SIZE) i = 0;
    }
#ifdef XTHREADS
    (void)miPointerUnlock();
#endif /* XTHREADS */
    return count;
}

    
/*
 * miPointerUpdate
 *
 * Syncronize the sprite with the cursor - called from ProcessInputEvents
 */

void
miPointerUpdate ()
#ifdef XTHREADS
{
    (void)miPointerLock ();

    miPointerUpdateNoLock ();

    miPointerUnlock ();
}

static
void
miPointerUpdateNoLock ()
#endif /* XTHREADS */
{
    ScreenPtr		pScreen;
    miPointerScreenPtr	pScreenPriv;
    int			x, y, devx, devy;
    Bool		newScreen = FALSE;

    pScreen = miPointer.pScreen;
    x = miPointer.x;
    y = miPointer.y;
    devx = miPointer.devx;
    devy = miPointer.devy;
    if (!pScreen)
	return;
    pScreenPriv = GetScreenPrivate (pScreen);
    /*
     * if the cursor has switched screens, disable the sprite
     * on the old screen
     */
    if (pScreen != miPointer.pSpriteScreen)
    {
	if (miPointer.pSpriteScreen)
	{
	    miPointerScreenPtr  pOldPriv;
    	
	    pOldPriv = GetScreenPrivate (miPointer.pSpriteScreen);
	    if (miPointer.pCursor)
	    {
	    	(*pOldPriv->spriteFuncs->SetCursor)
			    	(miPointer.pSpriteScreen, NullCursor, 0, 0);
	    }
	    (*pOldPriv->screenFuncs->CrossScreen) (miPointer.pSpriteScreen, FALSE);
	}
	(*pScreenPriv->screenFuncs->CrossScreen) (pScreen, TRUE);
	(*pScreenPriv->spriteFuncs->SetCursor)
				(pScreen, miPointer.pCursor, x, y);
	miPointer.devx = x;
	miPointer.devy = y;
	miPointer.pSpriteCursor = miPointer.pCursor;
	miPointer.pSpriteScreen = pScreen;
    }
    /*
     * if the cursor has changed, display the new one
     */
    else if (miPointer.pCursor != miPointer.pSpriteCursor)
    {
	(*pScreenPriv->spriteFuncs->SetCursor) 
	    (pScreen, miPointer.pCursor, x, y);
	miPointer.devx = x;
	miPointer.devy = y;
	miPointer.pSpriteCursor = miPointer.pCursor;
    }
    else if (x != devx || y != devy)
    {
	miPointer.devx = x;
	miPointer.devy = y;
	(*pScreenPriv->spriteFuncs->MoveCursor) (pScreen, x, y);
    }
}

/*
 * miPointerDeltaCursor.  The pointer has moved dx,dy from it's previous
 * position.
 */

void
miPointerDeltaCursor (dx, dy, time)
    int		    dx, dy;
    unsigned long   time;
{
#ifndef XTHREADS
    miPointerAbsoluteCursor (miPointer.x + dx, miPointer.y + dy, time);
#else /* XTHREADS */
    (void)miPointerLock ();

    miPointerAbsoluteCursorNoLock (miPointer.x + dx, miPointer.y + dy, time);

    miPointerUnlock ();
#endif /* XTHREADS */
}

/*
 * miPointerAbsoluteCursor.  The pointer has moved to x,y
 */

void
miPointerAbsoluteCursor (x, y, time)
    int		    x, y;
    unsigned long   time;
#ifdef XTHREADS
{
    (void)miPointerLock ();

    miPointerAbsoluteCursorNoLock (x, y, time);

    miPointerUnlock ();
}

static
void
miPointerAbsoluteCursorNoLock (x, y, time)
    int		    x, y;
    unsigned long   time;
#endif /* XTHREADS */
{
    miPointerScreenPtr	pScreenPriv;
    ScreenPtr		pScreen;
    ScreenPtr		newScreen;

    pScreen = miPointer.pScreen;
    if (!pScreen)
	return;	    /* called before ready */
    if (x < 0 || x >= pScreen->width || y < 0 || y >= pScreen->height)
    {
	pScreenPriv = GetScreenPrivate (pScreen);
	if (!miPointer.confined)
	{
	    newScreen = pScreen;
	    (*pScreenPriv->screenFuncs->CursorOffScreen) (&newScreen, &x, &y);
	    if (newScreen != pScreen)
	    {
		pScreen = newScreen;
		(*pScreenPriv->screenFuncs->NewEventScreen) (pScreen, FALSE);
		pScreenPriv = GetScreenPrivate (pScreen);
	    	/* Smash the confine to the new screen */
	    	miPointer.limits.x2 = pScreen->width;
	    	miPointer.limits.y2 = pScreen->height;
	    }
	}
    }
    /*
     * constrain the hot-spot to the current
     * limits
     */
    if (x < miPointer.limits.x1)
	x = miPointer.limits.x1;
    if (x >= miPointer.limits.x2)
	x = miPointer.limits.x2 - 1;
    if (y < miPointer.limits.y1)
	y = miPointer.limits.y1;
    if (y >= miPointer.limits.y2)
	y = miPointer.limits.y2 - 1;
    if (miPointer.x == x && miPointer.y == y && miPointer.pScreen == pScreen)
	return;
    miPointerMove (pScreen, x, y, time
#ifdef XTHREADS
	,TRUE
#endif /* XTHREADS */
	);
}

void
miPointerPosition (x, y)
    int	    *x, *y;
{
    *x = miPointer.x;
    *y = miPointer.y;
}

/*
 * miPointerMove.  The pointer has moved to x,y on current screen
 */

static void
miPointerMove (pScreen, x, y, time
#ifdef XTHREADS
    ,enqueue
#endif /* XTHREADS */
    )
    ScreenPtr	    pScreen;
    int		    x, y;
    unsigned long   time;
#ifdef XTHREADS
    Bool	    enqueue;
#endif /* XTHREADS */
{
    SetupScreen(pScreen);
    xEvent		xE;
    miHistoryPtr	history, prevHistory;
    int			prev, end, start;

    if (!pScreenPriv->waitForUpdate && pScreen == miPointer.pSpriteScreen)
    {
	miPointer.devx = x;
	miPointer.devy = y;
	(*pScreenPriv->spriteFuncs->MoveCursor) (pScreen, x, y);
    }
    miPointer.x = x;
    miPointer.y = y;
    miPointer.pScreen = pScreen;

#ifdef XTHREADS
    if (enqueue)	
       {
#endif /* XTHREADS */
    xE.u.u.type = MotionNotify;
    xE.u.keyButtonPointer.rootX = x;
    xE.u.keyButtonPointer.rootY = y;
    xE.u.keyButtonPointer.time = time;
    (*pScreenPriv->screenFuncs->EnqueueEvent) (&xE);
#ifdef XTHREADS
       }	
#endif /* XTHREADS */

    end = miPointer.history_end;
    start = miPointer.history_start;
    prev = end - 1;
    if (end == 0)
	prev = MOTION_SIZE - 1;
    history = &miPointer.history[prev];
    if (end == start || history->event.time != time)
    {
    	history = &miPointer.history[end];
    	if (++end == MOTION_SIZE) 
	    end = 0;
    	if (end == start)
    	{
	    start = end + 1;
	    if (start == MOTION_SIZE)
	    	start = 0;
	    miPointer.history_start = start;
    	}
    	miPointer.history_end = end;
    }
    history->event.x = x;
    history->event.y = y;
    history->event.time = time;
    history->pScreen = pScreen;
}

void
miRegisterPointerDevice (pScreen, pDevice)
    ScreenPtr	pScreen;
    DevicePtr	pDevice;
{
#ifdef XTHREADS
    (void)miPointerLock();
#endif /* XTHREADS */
    miPointer.pPointer = pDevice;
#ifdef XTHREADS
    (void)miPointerUnlock();
#endif /* XTHREADS */
}


#ifdef XTHREADS
/*
 * pointer lock/unlock functions
 *
 * miPointerLock   -- Lock the pointer data.
 *                    Return TRUE if successfully locked the pointer data.
 *                    Return FALSE if already locked the pointer data by
 *                    the same thread. 
 *
 * miPointerUnlock -- Unlock to the pointer data.
 *
 */


Bool
miPointerLock ()
{
#ifdef	USE_SOFTWARE_CURSOR
    extern X_THREAD_TYPE X_THREAD_SELF();
    pthread_t self   = X_THREAD_SELF();
    Bool      status = FALSE;

    if(miPointer.last_thread_id != self)
    {
	X_MUTEX_LOCK (&(miPointer.mutex));
	miPointer.last_thread_id = self;
        status = TRUE;
    }

    return status;
#else
    X_MUTEX_LOCK (&(miPointer.mutex));

    return TRUE;
#endif
}
   
void
miPointerUnlock ()
{

#ifdef	USE_SOFTWARE_CURSOR
    miPointer.last_thread_id = (X_THREAD_TYPE)-1;
#endif

    X_MUTEX_UNLOCK (&(miPointer.mutex));
}
#endif /* XTHREADS */
