/************************************************************

Copyright (c) 1987, 1989  X Consortium

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


Copyright 1987, 1989 by Digital Equipment Corporation, Maynard, Massachusetts.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the name of Digital not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.  

DIGITAL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

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

********************************************************/

/* $XConsortium: dispatch.c,v 1.10 94/04/17 21:15:42 rob Exp $ */

#include "X.h"
#define NEED_REPLIES
#define NEED_EVENTS
#include "Xproto.h"
#include "windowstr.h"
#include "fontstruct.h"
#include "dixfontstr.h"
#include "gcstruct.h"
#include "selection.h"
#include "colormapst.h"
#include "cursorstr.h"
#include "scrnintstr.h"
#include "opaque.h"
#include "input.h"
#include "servermd.h"
#include "extnsionst.h"
#include "dixfont.h"

#include "mtxlock.h"
#include "xthreads.h"


#define mskcnt ((MAXCLIENTS + 31) / 32)
#define BITMASK(i) (1 << ((i) & 31))
#define MASKIDX(i) ((i) >> 5)
#define MASKWORD(buf, i) buf[MASKIDX(i)]
#define BITSET(buf, i) MASKWORD(buf, i) |= BITMASK(i)
#define BITCLEAR(buf, i) MASKWORD(buf, i) &= ~BITMASK(i)
#define GETBIT(buf, i) (MASKWORD(buf, i) & BITMASK(i))

extern WindowPtr *WindowTable;
extern xConnSetupPrefix connSetupPrefix;
extern char *ConnectionInfo;
extern Atom MakeAtom();
extern char *NameForAtom();
extern void ReleaseActiveGrabs();
extern void NotImplemented();

Selection *CurrentSelections;
int NumCurrentSelections;

#ifdef XTHREADS
extern X_MUTEX_TYPE ConnectionMutex;
#define MTX_STATIC static
#else /* XTHREADS */
#define MTX_STATIC /* */
#endif /* XTHREADS */

extern long defaultScreenSaverTime;
extern long defaultScreenSaverInterval;
extern int  defaultScreenSaverBlanking;
extern int  defaultScreenSaverAllowExposures;
static ClientPtr grabClient;
#define GrabNone 0
#define GrabActive 1
#define GrabKickout 2
static int grabState = GrabNone;
static long grabWaiters[mskcnt];
long	*checkForInput[2];
extern int connBlockScreenStart;

#ifndef XTHREADS
extern int (* InitialVector[3]) ();
#endif
extern int (* ProcVector[256]) ();
#ifdef K5AUTH
extern int (* k5_Vector[256]) ();
#endif
extern int (* SwappedProcVector[256]) ();
extern void (* ReplySwapVector[256]) ();
extern void Swap32Write(), SLHostsExtend(), SQColorsExtend(), WriteSConnectionInfo();
extern void WriteSConnSetupPrefix();

static void KillAllClients(
#if NeedFunctionPrototypes
    void
#endif
);

void DeleteClientFromAnySelections(
#if NeedFunctionPrototypes
    ClientPtr /*client*/
#endif
);

/*
 * XXX:SM For MTX this is referenced in connection.c.
 *   - Choice: either do what I've done here or do what the original MTX
 *     did and globally declare it in cit.c.
 */
#ifdef XTHREADS
int nextFreeClientID;        /* always MIN free client ID */
#else
static int nextFreeClientID;
#endif

static int	nClients;	/* number active clients */

char dispatchException = 0;
char isItTimeToYield;

/* Various of the DIX function interfaces were not designed to allow
 * the client->errorValue to be set on BadValue and other errors.
 * Rather than changing interfaces and breaking untold code we introduce
 * a new global that dispatch can use.
 */
XID clientErrorValue;   /* XXX this is a kludge */

#define SAME_SCREENS(a, b) (\
    (a.pScreen == b.pScreen))

void
SetInputCheck(c0, c1)
    long *c0, *c1;
{
    checkForInput[0] = c0;
    checkForInput[1] = c1;
}

void
UpdateCurrentTime()
{
    TimeStamp systime;

    /* To avoid time running backwards, we must call GetTimeInMillis before
     * calling ProcessInputEvents.
     */
    systime.months = currentTime.months;
    systime.milliseconds = GetTimeInMillis();
    if (systime.milliseconds < currentTime.milliseconds)
	systime.months++;
    if (*checkForInput[0] != *checkForInput[1])
	ProcessInputEvents();
    if (CompareTimeStamps(systime, currentTime) == LATER)
	currentTime = systime;
}

/* Like UpdateCurrentTime, but can't call ProcessInputEvents */
void
UpdateCurrentTimeIf()
{
    TimeStamp systime;

    systime.months = currentTime.months;
    systime.milliseconds = GetTimeInMillis();
    if (systime.milliseconds < currentTime.milliseconds)
	systime.months++;
    if (*checkForInput[0] == *checkForInput[1])
	currentTime = systime;
}

void
InitSelections()
{
    if (CurrentSelections)
	xfree(CurrentSelections);
    CurrentSelections = (Selection *)NULL;
    NumCurrentSelections = 0;
}

#ifndef XTHREADS
void 
FlushClientCaches(id)
    XID id;
{
    int i;
    register ClientPtr client;

    client = clients[CLIENT_ID(id)];
    if (client == NullClient)
        return ;
    for (i=0; i<currentMaxClients; i++)
    {
	client = clients[i];
        if (client != NullClient)
	{
            if (client->lastDrawableID == id)
	    {
		client->lastDrawableID = WindowTable[0]->drawable.id;
		client->lastDrawable = (DrawablePtr)WindowTable[0];
	    }
            else if (client->lastGCID == id)
	    {
                client->lastGCID = INVALID;
		client->lastGC = (GCPtr)NULL;
	    }
	}
    }
}

#define MAJOROP ((xReq *)client->requestBuffer)->reqType

void
Dispatch()
{
    register int        *clientReady;     /* array of request ready clients */
    register int	result;
    register ClientPtr	client;
    register int	nready;
    register long	**icheck = checkForInput;

    nextFreeClientID = 1;
    InitSelections();
    nClients = 0;

    clientReady = (int *) ALLOCATE_LOCAL(sizeof(int) * MaxClients);
    if (!clientReady)
	return;

    while (!dispatchException)
    {
        if (*icheck[0] != *icheck[1])
	{
	    ProcessInputEvents();
	    FlushIfCriticalOutputPending();
	}

	nready = WaitForSomething(clientReady);

       /***************** 
	*  Handle events in round robin fashion, doing input between 
	*  each round 
	*****************/

	while (!dispatchException && (--nready >= 0))
	{
	    client = clients[clientReady[nready]];
	    if (! client)
	    {
		/* KillClient can cause this to happen */
		continue;
	    }
	    /* GrabServer activation can cause this to be true */
	    if (grabState == GrabKickout)
	    {
		grabState = GrabActive;
		break;
	    }
	    isItTimeToYield = FALSE;
 
            requestingClient = client;
	    while (!isItTimeToYield)
	    {
	        if (*icheck[0] != *icheck[1])
		{
		    ProcessInputEvents();
		    FlushIfCriticalOutputPending();
		}
	   
		/* now, finally, deal with client requests */

	        result = ReadRequestFromClient(client);
	        if (result <= 0) 
	        {
		    if (result < 0)
			CloseDownClient(client);
		    break;
	        }

		client->sequence++;
#ifdef DEBUG
		if (client->requestLogIndex == MAX_REQUEST_LOG)
		    client->requestLogIndex = 0;
		client->requestLog[client->requestLogIndex] = MAJOROP;
		client->requestLogIndex++;
#endif
		if (result > (MAX_BIG_REQUEST_SIZE << 2))
		    result = BadLength;
		else
		    result = (* client->requestVector[MAJOROP])(client);
	    
		if (result != Success) 
		{
		    if (client->noClientException != Success)
                        CloseDownClient(client);
                    else
		        SendErrorToClient(client, MAJOROP,
					  MinorOpcodeOfRequest(client),
					  client->errorValue, result);
		    break;
	        }
	    }
	    FlushAllOutput();
	}
	dispatchException &= ~DE_PRIORITYCHANGE;
    }
    KillAllClients();
    DEALLOCATE_LOCAL(clientReady);
    dispatchException &= ~DE_RESET;
}

#undef MAJOROP

#endif /* XTHREADS */

/*ARGSUSED*/
int
ProcBadRequest(client)
    ClientPtr client;
{
    return (BadRequest);
}

int
ProcCreateWindow(client)
    register ClientPtr client;
{
    WindowPtr pParent, pWin;
    REQUEST(xCreateWindowReq);
    int result;
    int len;

    REQUEST_AT_LEAST_SIZE(xCreateWindowReq);
    
    LEGAL_NEW_RESOURCE(stuff->wid, client);
    MTX_LOCK_AND_VERIFY_WINDOW (pParent, stuff->parent, client, 
				POQ_NULL_REGION, CM_XCreateWindow);
    len = client->req_len - (sizeof(xCreateWindowReq) >> 2);
    if (Ones(stuff->mask) != len)
    {
	MTX_UNLOCK_WINDOW(pParent, stuff->parent, client);
        return BadLength;
    }
    if (!stuff->width || !stuff->height)
    {
	client->errorValue = 0;
	MTX_UNLOCK_WINDOW(pParent, stuff->parent, client);
        return BadValue;
    }
    pWin = CreateWindow(stuff->wid, pParent, stuff->x,
			      stuff->y, stuff->width, stuff->height, 
			      stuff->borderWidth, stuff->class,
			      stuff->mask, (XID *) &stuff[1], 
			      (int)stuff->depth, 
			      client, stuff->visual, &result);
    if (pWin)
    {
	Mask mask = pWin->eventMask;

	pWin->eventMask = 0; /* subterfuge in case AddResource fails */
	if (!AddResource(stuff->wid, RT_WINDOW, (pointer)pWin))
	{
	    MTX_UNLOCK_WINDOW(pParent, stuff->parent, client);
	    return BadAlloc;
	}
	pWin->eventMask = mask;
    }
    MTX_UNLOCK_WINDOW(pParent, stuff->parent, client);

    if (client->noClientException != Success)
        return(client->noClientException);
    else
        return(result);
}

int
ProcChangeWindowAttributes(client)
    register ClientPtr client;
{
    WindowPtr pWin;
    REQUEST(xChangeWindowAttributesReq);
    register int result;
    int len;
#ifdef XTHREADS
    int mask, index;
#endif
    unsigned long conflictMask = 0;

    REQUEST_AT_LEAST_SIZE(xChangeWindowAttributesReq);
    len = client->req_len - (sizeof(xChangeWindowAttributesReq) >> 2);
    if (len != Ones(stuff->valueMask))
        return BadLength;

#ifdef XTHREADS
    mask = stuff->valueMask;
    while (mask)
    {
	index = lowbit (mask);
	mask &= ~index;
	switch (index)
	{
	    case CWBackPixmap:
	    case CWBackPixel:
	    case CWBorderPixmap:
	    case CWBorderPixel:
	    case CWBitGravity:
	    case CWWinGravity:
	    case CWBackingStore:
		conflictMask |= (CM_W_GEOMETRY | CM_X_RENDER);
		break;

	    case CWBackingPlanes:
	    case CWBackingPixel:
	    case CWSaveUnder:
		conflictMask |= (CM_W_GEOMETRY | CM_R_HIERARCHY | CM_X_RENDER);
		break;

	    case CWEventMask:
	    case CWDontPropagate:
		conflictMask |= (CM_W_EVENT_PROP);
		break;

	    case CWOverrideRedirect:
		conflictMask |= (CM_W_GEOMETRY | CM_W_EVENT_PROP);
		break;

	    case CWColormap:
		conflictMask |= (CM_W_COLORMAP | CM_R_GEOMETRY | 
				 CM_R_HIERARCHY | CM_R_EVENT_PROP);
		break;

	    case CWCursor:
		conflictMask |= (CM_X_CURSOR | CM_R_GEOMETRY | 
				 CM_R_HIERARCHY | CM_R_EVENT_PROP);
		break;

	    default:
		conflictMask |= CM_XChangeWindowAttributes;
		break;
	}
    }
#endif /* XTHREADS */

    MTX_LOCK_AND_VERIFY_WINDOW(pWin, stuff->window, client, POQ_BORDER_SIZE,
			   conflictMask);

    result =  ChangeWindowAttributes(pWin, 
				  stuff->valueMask, 
				  (XID *) &stuff[1], 
				  client);

    MTX_UNLOCK_WINDOW(pWin, stuff->window, client);

    if (client->noClientException != Success)
        return(client->noClientException);
    else
        return(result);
}

int
ProcGetWindowAttributes(client)
    register ClientPtr client;
{
    WindowPtr pWin;
    REQUEST(xResourceReq);

    REQUEST_SIZE_MATCH(xResourceReq);

    MTX_LOCK_AND_VERIFY_WINDOW(pWin, stuff->id, client, POQ_BORDER_SIZE,
			       CM_XGetWindowAttributes);

    GetWindowAttributes(pWin, client);

    MTX_UNLOCK_WINDOW(pWin, stuff->id, client);
    return(client->noClientException);
}

int
ProcDestroyWindow(client)
    register ClientPtr client;
{
    WindowPtr pWin;
    REQUEST(xResourceReq);

    REQUEST_SIZE_MATCH(xResourceReq);
    MTX_LOCK_AND_VERIFY_WINDOW(pWin, stuff->id, client, POQ_BORDER_SIZE,
			       CM_XDestroyWindow);

#ifdef XTHREADS 
   /* UnlockWindow(pWin, stuff->id);*/
#endif
    if (pWin->parent)
	FreeResource(stuff->id, RT_NONE);

    MTX_UNLOCK_PENDING_OPERATION_QUEUE(client);

    return(client->noClientException);
}

int
ProcDestroySubwindows(client)
    register ClientPtr client;
{
    WindowPtr pWin;
    REQUEST(xResourceReq);

    REQUEST_SIZE_MATCH(xResourceReq);
    MTX_LOCK_AND_VERIFY_WINDOW(pWin, stuff->id, client, POQ_WIN_SIZE,
			       CM_XDestroySubwindows);
    DestroySubwindows(pWin, client);
    MTX_UNLOCK_WINDOW(pWin, stuff->id, client);
    return(client->noClientException);
}

int
ProcChangeSaveSet(client)
    register ClientPtr client;
{
    WindowPtr pWin;
    REQUEST(xChangeSaveSetReq);
    register result;
		  
    REQUEST_SIZE_MATCH(xChangeSaveSetReq);
    MTX_LOCK_AND_VERIFY_WINDOW(pWin, stuff->window, client, POQ_NULL_REGION,
			       CM_XChangeSaveSet);

    if (client->clientAsMask == (CLIENT_BITS(pWin->drawable.id)))
        result = BadMatch;
    else if ((stuff->mode == SetModeInsert) || (stuff->mode == SetModeDelete))
    {
        result = AlterSaveSetForClient(client, pWin, stuff->mode);
	if (client->noClientException != Success)
	    result = client->noClientException;
    }
    else
    {
	client->errorValue = stuff->mode;
	result =  BadValue;
    }
    MTX_UNLOCK_WINDOW(pWin, stuff->window, client);
    return (result);
}

int
ProcReparentWindow(client)
    register ClientPtr client;
{
    WindowPtr pWin, pParent;
    REQUEST(xReparentWindowReq);
    register int result;

    REQUEST_SIZE_MATCH(xReparentWindowReq);

    MTX_LOCK_AND_VERIFY_ALL_WINDOWS(pWin, pParent, 
				    stuff->window, stuff->parent, client, 
				    POQ_BORDER_SIZE, CM_XReparentWindow);

    if (SAME_SCREENS(pWin->drawable, pParent->drawable))
    {
        if ((pWin->backgroundState == ParentRelative) &&
            (pParent->drawable.depth != pWin->drawable.depth))
	{
	    MTX_UNLOCK_ALL_WINDOWS(pWin, pParent, stuff->window, stuff->parent,
				   client);
            return BadMatch;
	}
	if ((pWin->drawable.class != InputOnly) &&
	    (pParent->drawable.class == InputOnly))
	{
	    MTX_UNLOCK_ALL_WINDOWS(pWin, pParent, stuff->window, stuff->parent,
				   client);
	    return BadMatch;
	}
        result =  ReparentWindow(pWin, pParent, 
			 (short)stuff->x, (short)stuff->y, client);

	MTX_UNLOCK_ALL_WINDOWS(pWin, pParent, stuff->window, stuff->parent, 
			       client);

	if (client->noClientException != Success)
            return(client->noClientException);
	else
            return(result);
    }
    else 
    {
	MTX_UNLOCK_ALL_WINDOWS(pWin, pParent, stuff->window, stuff->parent, 
			       client);
        return (BadMatch);
    }
}

int
ProcMapWindow(client)
    register ClientPtr client;
{
    WindowPtr pWin;
    REQUEST(xResourceReq);

    REQUEST_SIZE_MATCH(xResourceReq);
    MTX_LOCK_AND_VERIFY_WINDOW(pWin, stuff->id, client, POQ_BORDER_SIZE,
			       CM_XMapWindow);
    MapWindow(pWin, client);
           /* update cache to say it is mapped */

    MTX_UNLOCK_WINDOW(pWin, stuff->id, client);
    return(client->noClientException);
}

int
ProcMapSubwindows(client)
    register ClientPtr client;
{
    WindowPtr pWin;
    REQUEST(xResourceReq);

    REQUEST_SIZE_MATCH(xResourceReq);
    MTX_LOCK_AND_VERIFY_WINDOW(pWin, stuff->id, client, POQ_WIN_SIZE,
			       CM_XMapSubwindows);

    MapSubwindows(pWin, client);
           /* update cache to say it is mapped */

    MTX_UNLOCK_WINDOW(pWin, stuff->id, client);
    return(client->noClientException);
}

int
ProcUnmapWindow(client)
    register ClientPtr client;
{
    WindowPtr pWin;
    REQUEST(xResourceReq);

    REQUEST_SIZE_MATCH(xResourceReq);
    MTX_LOCK_AND_VERIFY_WINDOW(pWin, stuff->id, client, POQ_BORDER_SIZE,
			       CM_XUnmapWindow);

    UnmapWindow(pWin, FALSE);
           /* update cache to say it is mapped */

    MTX_UNLOCK_WINDOW(pWin, stuff->id, client);
    return(client->noClientException);
}

int
ProcUnmapSubwindows(client)
    register ClientPtr client;
{
    WindowPtr pWin;
    REQUEST(xResourceReq);

    REQUEST_SIZE_MATCH(xResourceReq);
    MTX_LOCK_AND_VERIFY_WINDOW(pWin, stuff->id, client, POQ_WIN_SIZE,
			       CM_XUnmapSubwindows);
    UnmapSubwindows(pWin);

    MTX_UNLOCK_WINDOW(pWin, stuff->id, client);
    return(client->noClientException);
}

int
ProcConfigureWindow(client)
    register ClientPtr client;
{
    WindowPtr pWin;
    REQUEST(xConfigureWindowReq);
    register int result;
    int len;
#ifdef XTHREADS
    RegionRec configureRegion; /* allocate space for configure region */
#endif /* XTHREADS */

    REQUEST_AT_LEAST_SIZE(xConfigureWindowReq);

/*
 * STX to MTX comment: I'm using an #ifdef here because its the only location
 *                     where the LockAndVerifyWindow function is being called
 *                     outside a macro, normally it is hidden inside the
 *                     LOCK_AND_VERIFY_WINDOW macro.  Normally after a 
 *                     successful call to LockAndVerifyWindow a call is made to
 *                     POQ_SET_WINDOW_CONFLICT here we call 
 *                     POQ_SET_REGION_CONFLICT.
 */
#ifdef XTHREADS
    if (result = LockAndVerifyWindow(&pWin, stuff->window, client))
	return (result);
    POQ_SET_REGION_CONFLICT(client, pWin, &configureRegion);
    POQLock(client, CM_XConfigureWindow);
#else
    pWin = (WindowPtr)LookupWindow( stuff->window, client);
    if (!pWin)
        return(BadWindow);
#endif
    len = client->req_len - (sizeof(xConfigureWindowReq) >> 2);
    if (Ones((Mask)stuff->mask) != len)
    {
	MTX_UNLOCK_WINDOW(pWin, stuff->window, client);
        return BadLength;
    }
    result =  ConfigureWindow(pWin, (Mask)stuff->mask, (XID *) &stuff[1], 
			      client);

    MTX_UNLOCK_WINDOW(pWin, stuff->window, client);
    if (client->noClientException != Success)
        return(client->noClientException);
    else
        return(result);
}

int
ProcCirculateWindow(client)
    register ClientPtr client;
{
    WindowPtr pWin;
    REQUEST(xCirculateWindowReq);

    REQUEST_SIZE_MATCH(xCirculateWindowReq);

    MTX_LOCK_AND_VERIFY_WINDOW(pWin, stuff->window, client, POQ_WIN_SIZE,
			       CM_XCirculateWindow);

    if ((stuff->direction != RaiseLowest) &&
	(stuff->direction != LowerHighest))
    {
	client->errorValue = stuff->direction;
	MTX_UNLOCK_WINDOW(pWin, stuff->window, client);
        return BadValue;
    }

    CirculateWindow(pWin, (int)stuff->direction, client);

    MTX_UNLOCK_WINDOW(pWin, stuff->window, client);
    return(client->noClientException);
}

int
ProcGetGeometry(client)
    register ClientPtr client;
{
    REPLY_DECL(xGetGeometryReply,rep);
    DrawablePtr pDraw;
    REQUEST(xResourceReq);

    REQUEST_SIZE_MATCH(xResourceReq);

    MTX_REP_CHECK_RETURN(rep,BadAlloc);

    MTX_REP_LOCK_AND_VERIFY_GEOMETRABLE(pDraw,stuff->id,client,CM_XGetGeometry);

    rep->type = X_Reply;
    rep->length = 0;
    rep->sequenceNumber = client->sequence;
    rep->root = WindowTable[pDraw->pScreen->myNum]->drawable.id;
    rep->depth = pDraw->depth;

    rep->width = pDraw->width;
    rep->height = pDraw->height;

    /* XXX - Because the pixmap-implementation of the multibuffer extension 
     *       may have the buffer-id's drawable resource value be a pointer
     *       to the buffer's window instead of the buffer itself
     *       (this happens if the buffer is the displayed buffer),
     *       we also have to check that the id matches before we can
     *       truly say that it is a DRAWABLE_WINDOW.
     */

    if ((pDraw->type == UNDRAWABLE_WINDOW) ||
        ((pDraw->type == DRAWABLE_WINDOW) && (stuff->id == pDraw->id)))
    {
        WindowPtr pWin = (WindowPtr)pDraw;
	rep->x = pWin->origin.x - wBorderWidth (pWin);
	rep->y = pWin->origin.y - wBorderWidth (pWin);
	rep->borderWidth = pWin->borderWidth;
    }
    else /* DRAWABLE_PIXMAP or DRAWABLE_BUFFER */
    {
	rep->x = rep->y = rep->borderWidth = 0;
    }
    WriteReplyToClient(client, sizeof(xGetGeometryReply), rep);

    MTX_UNLOCK_DRAWABLE(pDraw, stuff->id, client);
    return(client->noClientException);
}

int
ProcQueryTree(client)
    register ClientPtr client;
{
    REPLY_DECL(xQueryTreeReply,reply);
    int numChildren = 0;
    WindowPtr pChild, pWin, pHead;
    Window  *childIDs = (Window *)NULL;
    REQUEST(xResourceReq);

    REQUEST_SIZE_MATCH(xResourceReq);

    MTX_REP_CHECK_RETURN(reply,BadAlloc);

    MTX_REP_LOCK_AND_VERIFY_WINDOW(pWin, stuff->id, client, POQ_NULL_REGION,
			       CM_XQueryTree);

    reply->type = X_Reply;
    reply->root = WindowTable[pWin->drawable.pScreen->myNum]->drawable.id;
    reply->sequenceNumber = client->sequence;

    if (pWin->parent)
	reply->parent = pWin->parent->drawable.id;
    else
	reply->parent = (Window)None;

    pHead = RealChildHead(pWin);
    for (pChild = pWin->lastChild; pChild != pHead; pChild = pChild->prevSib)
	numChildren++;
    if (numChildren)
    {
	int curChild = 0;

	childIDs = (Window *) ALLOCATE_LOCAL(numChildren * sizeof(Window));
	if (!childIDs)
	{
	    MTXReturnPooledMessage;
	    MTX_UNLOCK_WINDOW(pWin, stuff->id, client);
	    return BadAlloc;
	}
	for (pChild = pWin->lastChild; pChild != pHead; pChild = pChild->prevSib)
	    childIDs[curChild++] = pChild->drawable.id;
    }
    
    reply->nChildren = numChildren;
    reply->length = (numChildren * sizeof(Window)) >> 2;
    
/*
 * XXX:SM - This sort of stuff is unfortunate.  I'm not really sure how to
 *          merge this code gracefully.  I'm not really sure that for cases
 *          like this it is worth the effort, time will tell however.
 */
#ifdef XTHREADS
    if (numChildren)
    {
	if (client->swapped)
	    Swap32(numChildren * sizeof(Window), childIDs);
	msg->pReplyData = (char *) childIDs;
	msg->freeReplyData = TRUE;
	msg->lenReplyData = numChildren * sizeof(Window);
    }

    WriteReplyToClient(client, sizeof(xQueryTreeReply), reply);
    MTX_UNLOCK_WINDOW(pWin, stuff->id, client);
#else
    WriteReplyToClient(client, sizeof(xQueryTreeReply), reply);
    if (numChildren)
    {
    	client->pSwapReplyFunc = Swap32Write;
	WriteSwappedDataToClient(client, numChildren * sizeof(Window), childIDs);
	DEALLOCATE_LOCAL(childIDs);
    }
#endif

    return(client->noClientException);
}

int
ProcInternAtom(client)
    register ClientPtr client;
{
    Atom atom;
    char *tchar;
    REQUEST(xInternAtomReq);

    REQUEST_FIXED_SIZE(xInternAtomReq, stuff->nbytes);

    MTX_LOCK_PENDING_OPERATION_QUEUE(client, CM_XInternAtom);

    if ((stuff->onlyIfExists != xTrue) && (stuff->onlyIfExists != xFalse))
    {
	client->errorValue = stuff->onlyIfExists;
	MTX_UNLOCK_PENDING_OPERATION_QUEUE(client);
        return(BadValue);
    }
    tchar = (char *) &stuff[1];
    atom = MakeAtom(tchar, stuff->nbytes, !stuff->onlyIfExists);
    if (atom != BAD_RESOURCE)
    {
	REPLY_DECL(xInternAtomReply,reply);
	if (!reply)
	{
	    MTX_UNLOCK_PENDING_OPERATION_QUEUE(client);
	    return BadAlloc;
	}

	reply->type = X_Reply;
	reply->length = 0;
	reply->sequenceNumber = client->sequence;
	reply->atom = atom;
	WriteReplyToClient(client, sizeof(xInternAtomReply), reply);

	MTX_UNLOCK_PENDING_OPERATION_QUEUE(client);
	return(client->noClientException);
    }
    else
    {
	MTX_UNLOCK_PENDING_OPERATION_QUEUE(client);
	return (BadAlloc);
    }
}

int
ProcGetAtomName(client)
    register ClientPtr client;
{
    char *str;
    REPLY_DECL(xGetAtomNameReply,reply);
    char *atomName;
    int len;
    REQUEST(xResourceReq);

    REQUEST_SIZE_MATCH(xResourceReq);

    MTX_LOCK_PENDING_OPERATION_QUEUE(client, CM_XGetAtomName);

    if ( (str = NameForAtom(stuff->id)) )
    {
	len = strlen(str);

	reply->type = X_Reply;
	reply->length = (len + 3) >> 2;
	reply->sequenceNumber = client->sequence;
	reply->nameLength = len;

#ifdef XTHREADS
	atomName = (char *) xalloc(len);
	if (!atomName)
	{
	    MTXReturnPooledMessage;
	    MTX_UNLOCK_PENDING_OPERATION_QUEUE(client);
	    return (BadAlloc);
	}
	bcopy(str, atomName, len);

	msg->pReplyData = atomName;
	msg->freeReplyData = TRUE;
	msg->lenReplyData = len;
#endif
	WriteReplyToClient(client, sizeof(xGetAtomNameReply), reply);
#ifndef XTHREADS
	(void)WriteToClient(client, len, str);
#endif
	MTX_UNLOCK_PENDING_OPERATION_QUEUE(client);
	return(client->noClientException);
    }
    else 
    { 
	client->errorValue = stuff->id;
	MTX_UNLOCK_PENDING_OPERATION_QUEUE(client);
	return (BadAtom);
    }
}

int 
ProcDeleteProperty(client)
    register ClientPtr client;
{
    WindowPtr pWin;
    REQUEST(xDeletePropertyReq);
    int result;
              
    REQUEST_SIZE_MATCH(xDeletePropertyReq);

    MTX_LOCK_AND_VERIFY_WINDOW(pWin, stuff->window, client, POQ_NULL_REGION,
				CM_XDeleteProperty);

    UpdateCurrentTime();

    if (ValidAtom(stuff->property))
    {
	result = DeleteProperty(pWin, stuff->property);
        if (client->noClientException != Success)
	{
	    MTX_UNLOCK_WINDOW(pWin, stuff->window, client);
            return(client->noClientException);
	}
	else
	{
	    MTX_UNLOCK_WINDOW(pWin, stuff->window, client);
	    return(result);
	}
    }
    else 
    {
	client->errorValue = stuff->property;
	MTX_UNLOCK_WINDOW(pWin, stuff->window, client);
	return (BadAtom);
    }
}

#ifdef K5AUTH
extern int k5_bad();
#endif

int
ProcSetSelectionOwner(client)
    register ClientPtr client;
{
    WindowPtr pWin;
    TimeStamp time;
    REQUEST(xSetSelectionOwnerReq);

    REQUEST_SIZE_MATCH(xSetSelectionOwnerReq);

    MTX_LOCK_PENDING_OPERATION_QUEUE(client, CM_XSetSelectionOwner);

    UpdateCurrentTime();
    time = ClientTimeToServerTime(stuff->time);

    /* If the client's time stamp is in the future relative to the server's
	time stamp, do not set the selection, just return success. */
    if (CompareTimeStamps(time, currentTime) == LATER)
    {
	MTX_UNLOCK_PENDING_OPERATION_QUEUE(client);
    	return Success;
    }
    if (stuff->window != None)
    {
        pWin = (WindowPtr)LookupWindow(stuff->window, client);
        if (!pWin)
	{
	    MTX_UNLOCK_PENDING_OPERATION_QUEUE(client);
            return(BadWindow);
	}
    }
    else
        pWin = (WindowPtr)None;
    if (ValidAtom(stuff->selection))
    {
	int i = 0;

	/*
	 * First, see if the selection is already set... 
	 */
	while ((i < NumCurrentSelections) && 
	       CurrentSelections[i].selection != stuff->selection) 
            i++;
        if (i < NumCurrentSelections)
        {        
	    xEvent event;

	    /* If the timestamp in client's request is in the past relative
		to the time stamp indicating the last time the owner of the
		selection was set, do not set the selection, just return 
		success. */
            if (CompareTimeStamps(time, CurrentSelections[i].lastTimeChanged)
		== EARLIER)
	    {
		MTX_UNLOCK_PENDING_OPERATION_QUEUE(client);
		return Success;
	    }
	    if (CurrentSelections[i].client &&
		(!pWin || (CurrentSelections[i].client != client)))
	    {
		event.u.u.type = SelectionClear;
		event.u.selectionClear.time = time.milliseconds;
		event.u.selectionClear.window = CurrentSelections[i].window;
		event.u.selectionClear.atom = CurrentSelections[i].selection;
		(void) TryClientEvents (CurrentSelections[i].client, &event, 1,
				NoEventMask, NoEventMask /* CantBeFiltered */,
				NullGrab);
	    }
	}
	else
	{
	    /*
	     * It doesn't exist, so add it...
	     */
	    Selection *newsels;

	    if (i == 0)
		newsels = (Selection *)xalloc(sizeof(Selection));
	    else
		newsels = (Selection *)xrealloc(CurrentSelections,
			    (NumCurrentSelections + 1) * sizeof(Selection));
	    if (!newsels)
	    {
		MTX_UNLOCK_PENDING_OPERATION_QUEUE(client);
		return BadAlloc;
	    }
	    NumCurrentSelections++;
	    CurrentSelections = newsels;
	    CurrentSelections[i].selection = stuff->selection;
	}
        CurrentSelections[i].lastTimeChanged = time;
	CurrentSelections[i].window = stuff->window;
	CurrentSelections[i].pWin = pWin;
	CurrentSelections[i].client = (pWin ? client : NullClient);
	return (client->noClientException);
    }
    else 
    {
	client->errorValue = stuff->selection;
	MTX_UNLOCK_PENDING_OPERATION_QUEUE(client);
        return (BadAtom);
    }
}

int
ProcGetSelectionOwner(client)
    register ClientPtr client;
{
    REQUEST(xResourceReq);

    REQUEST_SIZE_MATCH(xResourceReq);

    MTX_LOCK_PENDING_OPERATION_QUEUE(client, CM_XGetSelectionOwner);

    if (ValidAtom(stuff->id))
    {
	int i;
	REPLY_DECL(xGetSelectionOwnerReply,reply);

	if (!reply)
	{
	    MTX_UNLOCK_PENDING_OPERATION_QUEUE(client);
	    return BadAlloc;
	}

	i = 0;
        while ((i < NumCurrentSelections) && 
	       CurrentSelections[i].selection != stuff->id) i++;

        reply->type = X_Reply;
	reply->length = 0;
	reply->sequenceNumber = client->sequence;
        if (i < NumCurrentSelections)
            reply->owner = CurrentSelections[i].window;
        else
            reply->owner = None;
        WriteReplyToClient(client, sizeof(xGetSelectionOwnerReply), reply);
	MTX_UNLOCK_PENDING_OPERATION_QUEUE(client);
        return(client->noClientException);
    }
    else            
    {
	client->errorValue = stuff->id;
	MTX_UNLOCK_PENDING_OPERATION_QUEUE(client);
        return (BadAtom); 
    }
}

int
ProcConvertSelection(client)
    register ClientPtr client;
{
    Bool paramsOkay;
    xEvent event;
    WindowPtr pWin;
    REQUEST(xConvertSelectionReq);

    REQUEST_SIZE_MATCH(xConvertSelectionReq);

    MTX_LOCK_AND_VERIFY_WINDOW(pWin, stuff->requestor, client, POQ_NULL_REGION,
				CM_XConvertSelection);

    paramsOkay = (ValidAtom(stuff->selection) && ValidAtom(stuff->target));
    if (stuff->property != None)
	paramsOkay &= ValidAtom(stuff->property);
    if (paramsOkay)
    {
	int i;

	i = 0;
	while ((i < NumCurrentSelections) && 
	       CurrentSelections[i].selection != stuff->selection) i++;
	if ((i < NumCurrentSelections) && 
	    (CurrentSelections[i].window != None))
	{        
	    event.u.u.type = SelectionRequest;
	    event.u.selectionRequest.time = stuff->time;
	    event.u.selectionRequest.owner = 
			CurrentSelections[i].window;
	    event.u.selectionRequest.requestor = stuff->requestor;
	    event.u.selectionRequest.selection = stuff->selection;
	    event.u.selectionRequest.target = stuff->target;
	    event.u.selectionRequest.property = stuff->property;
	    if (TryClientEvents(
		CurrentSelections[i].client, &event, 1, NoEventMask,
		NoEventMask /* CantBeFiltered */, NullGrab))
	    {
		MTX_UNLOCK_WINDOW(pWin, stuff->requestor, client);
		return (client->noClientException);
	    }
	}
	event.u.u.type = SelectionNotify;
	event.u.selectionNotify.time = stuff->time;
	event.u.selectionNotify.requestor = stuff->requestor;
	event.u.selectionNotify.selection = stuff->selection;
	event.u.selectionNotify.target = stuff->target;
	event.u.selectionNotify.property = None;
	(void) TryClientEvents(client, &event, 1, NoEventMask,
			       NoEventMask /* CantBeFiltered */, NullGrab);
	MTX_UNLOCK_WINDOW(pWin, stuff->requestor, client);
	return (client->noClientException);
    }
    else 
    {
	client->errorValue = stuff->property;
	MTX_UNLOCK_WINDOW(pWin, stuff->requestor, client);
        return (BadAtom);
    }
}

int
ProcGrabServer(client)
    register ClientPtr client;
{
    REQUEST(xReq);
    REQUEST_SIZE_MATCH(xReq);
#ifdef XTHREADS
    POQGrabServer(client);
#else
    if (grabState != GrabNone && client != grabClient)
    {
	ResetCurrentRequest(client);
	client->sequence--;
	BITSET(grabWaiters, client->index);
	IgnoreClient(client);
	return(client->noClientException);
    }
    OnlyListenToOneClient(client);
    grabState = GrabKickout;
    grabClient = client;
#endif
    return(client->noClientException);
}

static void
UngrabServer()
{
    int i;

    grabState = GrabNone;
    ListenToAllClients();
    for (i = mskcnt; --i >= 0 && !grabWaiters[i]; )
	;
    if (i >= 0)
    {
	i <<= 5;
	while (!GETBIT(grabWaiters, i))
	    i++;
	BITCLEAR(grabWaiters, i);
	AttendClient(clients[i]);
    }
}

int
ProcUngrabServer(client)
    register ClientPtr client;
{
    REQUEST(xReq);
    REQUEST_SIZE_MATCH(xReq);
#ifdef XTHREADS
    POQUngrabServer(client);
#else
    UngrabServer();
#endif
    return(client->noClientException);
}

int
ProcTranslateCoords(client)
    register ClientPtr client;
{
    REQUEST(xTranslateCoordsReq);

    WindowPtr pWin, pDst;
    REPLY_DECL(xTranslateCoordsReply,rep);

    REQUEST_SIZE_MATCH(xTranslateCoordsReq);

    MTX_REP_CHECK_RETURN(rep,BadAlloc);

    MTX_REP_LOCK_AND_VERIFY_ALL_WINDOWS(pWin, pDst, stuff->srcWid, 
				    stuff->dstWid, client, POQ_NULL_REGION,
				    CM_XTranslateCoords);

    rep->type = X_Reply;
    rep->length = 0;
    rep->sequenceNumber = client->sequence;

    if (!SAME_SCREENS(pWin->drawable, pDst->drawable))
    {
	rep->sameScreen = xFalse;
        rep->child = None;
	rep->dstX = rep->dstY = 0;
    }
    else
    {
	INT16 x, y;
	rep->sameScreen = xTrue;
	rep->child = None;
	/* computing absolute coordinates -- adjust to destination later */
	x = pWin->drawable.x + stuff->srcX;
	y = pWin->drawable.y + stuff->srcY;
	pWin = pDst->firstChild;
	while (pWin)
	{
#ifdef SHAPE
	    BoxRec  box;
#endif
	    if ((pWin->mapped) &&
		(x >= pWin->drawable.x - wBorderWidth (pWin)) &&
		(x < pWin->drawable.x + (int)pWin->drawable.width +
		 wBorderWidth (pWin)) &&
		(y >= pWin->drawable.y - wBorderWidth (pWin)) &&
		(y < pWin->drawable.y + (int)pWin->drawable.height +
		 wBorderWidth (pWin))
#ifdef SHAPE
		/* When a window is shaped, a further check
		 * is made to see if the point is inside
		 * borderSize
		 */
		&& (!wBoundingShape(pWin) ||
		    POINT_IN_REGION(pWin->drawable.pScreen, 
			    &pWin->borderSize, x, y, &box))
#endif
		)
            {
		rep->child = pWin->drawable.id;
		pWin = (WindowPtr) NULL;
	    }
	    else
		pWin = pWin->nextSib;
	}
	/* adjust to destination coordinates */
	rep->dstX = x - pDst->drawable.x;
	rep->dstY = y - pDst->drawable.y;
    }
    WriteReplyToClient(client, sizeof(xTranslateCoordsReply), rep);
    MTX_UNLOCK_ALL_WINDOWS(pWin, pDst, stuff->srcWid, stuff->dstWid, 
			   client);
    return(client->noClientException);
}

int
ProcOpenFont(client)
    register ClientPtr client;
{
    int	err;
    REQUEST(xOpenFontReq);

    REQUEST_FIXED_SIZE(xOpenFontReq, stuff->nbytes);
    client->errorValue = stuff->fid;
    LEGAL_NEW_RESOURCE(stuff->fid, client);

    MTX_LOCK_PENDING_OPERATION_QUEUE(client, CM_XOpenFont);

    err = OpenFont(client, stuff->fid, (Mask) 0,
		stuff->nbytes, (char *)&stuff[1]);

    MTX_UNLOCK_PENDING_OPERATION_QUEUE(client);
    if (err == Success)
    {
	return(client->noClientException);
    }
    else
	return err;
}

int
ProcCloseFont(client)
    register ClientPtr client;
{
    FontPtr pFont;
    REQUEST(xResourceReq);

    REQUEST_SIZE_MATCH(xResourceReq);

/*
 * No special macros here, LOCK_AND_VERIFY_FONT is only used here.
 */
#ifdef XTHREADS
    LOCK_AND_VERIFY_FONT(pFont, stuff->id, client, CM_XCloseFont);
    UnlockFont(pFont, stuff->id);
    FreeResource(stuff->id, RT_NONE);

    MTX_UNLOCK_PENDING_OPERATION_QUEUE(client);
    return(client->noClientException);
#else
    pFont = (FontPtr)LookupIDByType(stuff->id, RT_FONT);
    if ( pFont != (FontPtr)NULL)	/* id was valid */
    {
        FreeResource(stuff->id, RT_NONE);
	return(client->noClientException);
    }
    else
    {
	client->errorValue = stuff->id;
        return (BadFont);
    }
#endif
}

int
ProcQueryFont(client)
    register ClientPtr client;
{
    xQueryFontReply *reply;	/* ZZZ - no static reply buffer */
#ifdef XTHREADS
    PooledMessagePtr msg;
#endif /* XTHREADS */
    FontPtr pFont;
    GC *pGC;
    xCharInfo	*pmax;
    xCharInfo	*pmin;
    int		nprotoxcistructs;
    int		rlength;
    REQUEST(xResourceReq);

    REQUEST_SIZE_MATCH(xResourceReq);
    client->errorValue = stuff->id;		/* EITHER font or gc */

    MTX_LOCK_AND_VERIFY_GC_FONT(pFont, pGC, stuff->id, client, CM_XQueryFont);

    pmax = FONTINKMAX(pFont);
    pmin = FONTINKMIN(pFont);

    nprotoxcistructs = (
       pmax->rightSideBearing == pmin->rightSideBearing &&
       pmax->leftSideBearing == pmin->leftSideBearing &&
       pmax->descent == pmin->descent &&
       pmax->ascent == pmin->ascent &&
       pmax->characterWidth == pmin->characterWidth) ?
	    0 : N2dChars(pFont);

    rlength = sizeof(xQueryFontReply) +
		 FONTINFONPROPS(FONTCHARSET(pFont)) * sizeof(xFontProp)  +
		 nprotoxcistructs * sizeof(xCharInfo);
#ifdef XTHREADS
    reply = (xQueryFontReply *) GetPooledReplyMessage(rlength, &msg);
#else
    reply = (xQueryFontReply *)ALLOCATE_LOCAL(rlength);
#endif
    if(!reply)
    {
	MTX_UNLOCK_FONT(pFont, stuff->id, client);
	return(BadAlloc);
    }

    reply->type = X_Reply;
    reply->length = (rlength - sizeof(xGenericReply)) >> 2;
    reply->sequenceNumber = client->sequence;
    QueryFont( pFont, reply, nprotoxcistructs);

    WriteReplyToClient(client, rlength, reply);
    MTX_UNLOCK_FONT(pFont, stuff->id, client);
#ifndef XTHREADS
    DEALLOCATE_LOCAL(reply);
#endif
    return(client->noClientException);
}

int
ProcQueryTextExtents(client)
    register ClientPtr client;
{
    REQUEST(xQueryTextExtentsReq);
    REPLY_DECL(xQueryTextExtentsReply,reply);
    FontPtr pFont;
    GC *pGC;
    ExtentInfoRec info;
    unsigned long length;

    REQUEST_AT_LEAST_SIZE(xQueryTextExtentsReq);

    MTX_REP_CHECK_RETURN(reply,BadAlloc);

    MTX_REP_LOCK_AND_VERIFY_GC_FONT(pFont, pGC, stuff->fid, client, 
				CM_XQueryTextExtents);
        
    length = client->req_len - (sizeof(xQueryTextExtentsReq) >> 2);
    length = length << 1;
    if (stuff->oddLength)
    {
	if (length == 0)
	    return(BadLength);
        length--;
    }
    if (!QueryTextExtents(pFont, length, (unsigned char *)&stuff[1], &info))
    {
	MTX_UNLOCK_FONT(pFont, stuff->fid, client);
	return(BadAlloc);
    }

    reply->type = X_Reply;
    reply->length = 0;
    reply->sequenceNumber = client->sequence;
    reply->drawDirection = info.drawDirection;
    reply->fontAscent = info.fontAscent;
    reply->fontDescent = info.fontDescent;
    reply->overallAscent = info.overallAscent;
    reply->overallDescent = info.overallDescent;
    reply->overallWidth = info.overallWidth;
    reply->overallLeft = info.overallLeft;
    reply->overallRight = info.overallRight;

    WriteReplyToClient(client, sizeof(xQueryTextExtentsReply), reply);

    MTX_UNLOCK_FONT(pFont, stuff->fid, client);
    return(client->noClientException);
}

int
ProcListFonts(client)
    register ClientPtr client;
{
    REQUEST(xListFontsReq);
    int retval;

    REQUEST_FIXED_SIZE(xListFontsReq, stuff->nbytes);

    MTX_LOCK_PENDING_OPERATION_QUEUE(client, CM_XListFonts);

    retval = ListFonts(client, (unsigned char *) &stuff[1], stuff->nbytes,
		       stuff->maxNames);

    MTX_UNLOCK_PENDING_OPERATION_QUEUE(client);

    return (retval);
}

int
ProcListFontsWithInfo(client)
    register ClientPtr client;
{
    REQUEST(xListFontsWithInfoReq);
    int retval;

    REQUEST_FIXED_SIZE(xListFontsWithInfoReq, stuff->nbytes);

    MTX_LOCK_PENDING_OPERATION_QUEUE(client, CM_XListFontsWithInfo);

    retval = StartListFontsWithInfo(client, stuff->nbytes,
				    (unsigned char *) &stuff[1], 
				    stuff->maxNames);
    MTX_UNLOCK_PENDING_OPERATION_QUEUE(client);
    return (retval);
}

/*ARGSUSED*/
int
dixDestroyPixmap(value, pid)
    pointer value; /* must conform to DeleteType */
    XID pid;
{
    PixmapPtr pPixmap = (PixmapPtr)value;
    return (*pPixmap->drawable.pScreen->DestroyPixmap)(pPixmap);
}

int
ProcCreatePixmap(client)
    register ClientPtr client;
{
    PixmapPtr pMap;
    DrawablePtr pDraw;
    REQUEST(xCreatePixmapReq);
    DepthPtr pDepth;
    register int i;

    REQUEST_SIZE_MATCH(xCreatePixmapReq);
    client->errorValue = stuff->pid;
    LEGAL_NEW_RESOURCE(stuff->pid, client);
    MTX_LOCK_AND_VERIFY_GEOMETRABLE(pDraw, stuff->drawable, client,
				    CM_XCreatePixmap);

    if (!stuff->width || !stuff->height)
    {
	client->errorValue = 0;
	MTX_UNLOCK_DRAWABLE(pDraw, stuff->drawable, client);
        return BadValue;
    }
    if (stuff->depth != 1)
    {
        pDepth = pDraw->pScreen->allowedDepths;
        for (i=0; i<pDraw->pScreen->numDepths; i++, pDepth++)
	   if (pDepth->depth == stuff->depth)
               goto CreatePmap;
	client->errorValue = stuff->depth;
	MTX_UNLOCK_DRAWABLE(pDraw, stuff->drawable, client);
        return BadValue;
    }
CreatePmap:
    pMap = (PixmapPtr)(*pDraw->pScreen->CreatePixmap)
		(pDraw->pScreen, stuff->width,
		 stuff->height, stuff->depth);
    if (pMap)
    {
	pMap->drawable.serialNumber = NEXT_SERIAL_NUMBER;
	pMap->drawable.id = stuff->pid;
	if (AddResource(stuff->pid, RT_PIXMAP, (pointer)pMap))
	{
	    MTX_UNLOCK_DRAWABLE(pDraw, stuff->drawable, client);
	    return(client->noClientException);
	}
    }
    MTX_UNLOCK_DRAWABLE(pDraw, stuff->drawable, client);
    return (BadAlloc);
}

int
ProcFreePixmap(client)
    register ClientPtr client;
{
    PixmapPtr pMap;

    REQUEST(xResourceReq);

    REQUEST_SIZE_MATCH(xResourceReq);
    MTX_LOCK_AND_VERIFY_PIXMAP(pMap, stuff->id, client, CM_XFreePixmap);
    MTXUnlockDrawable(pMap, stuff->id);
    FreeResource(stuff->id, RT_NONE);
    MTX_UNLOCK_PENDING_OPERATION_QUEUE(client);
    return(client->noClientException);
}

int
ProcCreateGC(client)
    register ClientPtr client;
{
    int error;
    GC *pGC;
    DrawablePtr pDraw;
    unsigned len;
    REQUEST(xCreateGCReq);

    REQUEST_AT_LEAST_SIZE(xCreateGCReq);
    client->errorValue = stuff->gc;
    LEGAL_NEW_RESOURCE(stuff->gc, client);
    MTX_LOCK_AND_VERIFY_DRAWABLE(pDraw, stuff->drawable, client, CM_XCreateGC);
    len = client->req_len -  (sizeof(xCreateGCReq) >> 2);
    if (len != Ones(stuff->mask))
    {
	MTX_UNLOCK_DRAWABLE (pDraw, stuff->drawable, client);
        return BadLength;
    }
    pGC = (GC *)CreateGC(pDraw, stuff->mask, 
			 (XID *) &stuff[1], &error);
    if (error != Success)
    {
	MTX_UNLOCK_DRAWABLE (pDraw, stuff->drawable, client);
        return error;
    }
    if (!AddResource(stuff->gc, RT_GC, (pointer)pGC))
    {
	MTX_UNLOCK_DRAWABLE (pDraw, stuff->drawable, client);
	return (BadAlloc);
    }
    MTX_UNLOCK_DRAWABLE (pDraw, stuff->drawable, client);
    return(client->noClientException);
}

int
ProcChangeGC(client)
    register ClientPtr client;
{
    GC *pGC;
    REQUEST(xChangeGCReq);
    int result;
    unsigned int len;
		
    REQUEST_AT_LEAST_SIZE(xChangeGCReq);
    MTX_LOCK_AND_VERIFY_GC(pGC, stuff->gc, client, CM_XChangeGC);

    len = client->req_len -  (sizeof(xChangeGCReq) >> 2);
    if (len != Ones(stuff->mask))
    {
	MTX_UNLOCK_GC(pGC, stuff->gc, client);
        return BadLength;
    }
    result = DoChangeGC(pGC, stuff->mask, (XID *) &stuff[1], 0);

    MTX_UNLOCK_GC(pGC, stuff->gc, client);

    if (client->noClientException != Success)
        return(client->noClientException);
    else
    {
	client->errorValue = clientErrorValue;
        return(result);
    }
}

int
ProcCopyGC(client)
    register ClientPtr client;
{
    GC *dstGC;
    GC *pGC;
    int result;
    REQUEST(xCopyGCReq);

    REQUEST_SIZE_MATCH(xCopyGCReq);

    MTX_LOCK_AND_VERIFY_TWO_GCS(pGC, dstGC, stuff->srcGC, stuff->dstGC, client,
				CM_XCopyGC);

    if ((dstGC->pScreen != pGC->pScreen) || (dstGC->depth != pGC->depth))
    {
	MTX_UNLOCK_TWO_GCS(pGC, dstGC, stuff->srcGC, stuff->dstGC, client);
        return (BadMatch);    
    }
    result = CopyGC(pGC, dstGC, stuff->mask);

    MTX_UNLOCK_TWO_GCS(pGC, dstGC, stuff->srcGC, stuff->dstGC, client);

    if (client->noClientException != Success)
        return(client->noClientException);
    else
    {
	client->errorValue = clientErrorValue;
        return(result);
    }
}

int
ProcSetDashes(client)
    register ClientPtr client;
{
    GC *pGC;
    int result;
    REQUEST(xSetDashesReq);

    REQUEST_FIXED_SIZE(xSetDashesReq, stuff->nDashes);

    MTX_LOCK_AND_VERIFY_GC(pGC, stuff->gc, client, CM_XSetDashes);

    if (stuff->nDashes == 0)
    {
	 client->errorValue = 0;
	 MTX_UNLOCK_GC(pGC, stuff->gc, client);
         return BadValue;
    }

    result = SetDashes(pGC, stuff->dashOffset, stuff->nDashes,
		       (unsigned char *)&stuff[1]);
    MTX_UNLOCK_GC(pGC, stuff->gc, client);

    if (client->noClientException != Success)
        return(client->noClientException);
    else
    {
	client->errorValue = clientErrorValue;
        return(result);
    }
}

int
ProcSetClipRectangles(client)
    register ClientPtr client;
{
    int	nr;
    int result;
    GC *pGC;
    REQUEST(xSetClipRectanglesReq);

    REQUEST_AT_LEAST_SIZE(xSetClipRectanglesReq);

    MTX_LOCK_AND_VERIFY_GC(pGC, stuff->gc, client, CM_XSetClipRectangles);

    if ((stuff->ordering != Unsorted) && (stuff->ordering != YSorted) &&
	(stuff->ordering != YXSorted) && (stuff->ordering != YXBanded))
    {
	client->errorValue = stuff->ordering;
	MTX_UNLOCK_GC(pGC, stuff->gc, client);
        return BadValue;
    }
		 
    nr = (client->req_len << 2) - sizeof(xSetClipRectanglesReq);
    if (nr & 4)
    {
	MTX_UNLOCK_GC(pGC, stuff->gc, client);
	return(BadLength);
    }
    nr >>= 3;
    result = SetClipRects(pGC, stuff->xOrigin, stuff->yOrigin,
			  nr, (xRectangle *)&stuff[1], (int)stuff->ordering);
    MTX_UNLOCK_GC(pGC, stuff->gc, client);

    if (client->noClientException != Success)
        return(client->noClientException);
    else
        return(result);
}

int
ProcFreeGC(client)
    register ClientPtr client;
{
    GC *pGC;
    REQUEST(xResourceReq);

    REQUEST_SIZE_MATCH(xResourceReq);
    MTX_LOCK_AND_VERIFY_GC(pGC, stuff->id, client, CM_XFreeGC);
    MTXUnlockGC(pGC, stuff->id);
    FreeResource(stuff->id, RT_NONE);
    MTX_UNLOCK_PENDING_OPERATION_QUEUE(client);
    return(client->noClientException);
}

int
ProcClearToBackground(client)
    register ClientPtr client;
{
    REQUEST(xClearAreaReq);
    WindowPtr pWin;

    REQUEST_SIZE_MATCH(xClearAreaReq);

    MTX_LOCK_AND_VERIFY_WINDOW(pWin, stuff->window, client, POQ_CLIP_LIST,	
			       CM_XClearToBackground);

    if (pWin->drawable.class == InputOnly)
    {
	client->errorValue = stuff->window;
	MTX_UNLOCK_WINDOW(pWin, stuff->window, client);
	return (BadMatch);
    }		    
    if ((stuff->exposures != xTrue) && (stuff->exposures != xFalse))
    {
	client->errorValue = stuff->exposures;
	MTX_UNLOCK_WINDOW(pWin, stuff->window, client);
        return(BadValue);
    }
    (*pWin->drawable.pScreen->ClearToBackground)(pWin, stuff->x, stuff->y,
			       stuff->width, stuff->height,
			       (Bool)stuff->exposures);
    MTX_UNLOCK_WINDOW(pWin, stuff->window, client);
    return(client->noClientException);
}

int
ProcCopyArea(client)
    register ClientPtr client;
{
    DrawablePtr pDst;
    DrawablePtr pSrc;
    GC *pGC;
    REQUEST(xCopyAreaReq);
    RegionPtr pRgn;

    REQUEST_SIZE_MATCH(xCopyAreaReq);

    MTX_LOCK_AND_VALIDATE_TWO_DRAWABLES_AND_GC(pSrc, pDst, pGC,
		stuff->srcDrawable, stuff->dstDrawable, stuff->gc, client);

    if ((pDst->pScreen != pSrc->pScreen) || (pDst->depth != pSrc->depth))
    {
	client->errorValue = stuff->dstDrawable;
	MTX_UNLOCK_TWO_DRAWABLES_AND_GC(pSrc, pDst, pGC, stuff->srcDrawable,
				stuff->dstDrawable, stuff->gc, client);
	return (BadMatch);
    }

    pRgn = (*pGC->ops->CopyArea)(pSrc, pDst, pGC, stuff->srcX, stuff->srcY,
				 stuff->width, stuff->height, 
				 stuff->dstX, stuff->dstY);

    if (pGC->graphicsExposures)
    {
	(*pDst->pScreen->SendGraphicsExpose)
 		(client, pRgn, stuff->dstDrawable, X_CopyArea, 0);
	if (pRgn)
	    REGION_DESTROY(pDst->pScreen, pRgn);
    }

    MTX_UNLOCK_TWO_DRAWABLES_AND_GC(pSrc, pDst, pGC, stuff->srcDrawable, 
			        stuff->dstDrawable, stuff->gc, client);
    return(client->noClientException);
}

int
ProcCopyPlane(client)
    register ClientPtr client;
{
    DrawablePtr pSrc, pDst;
    GC *pGC;
    REQUEST(xCopyPlaneReq);
    RegionPtr pRgn;

    REQUEST_SIZE_MATCH(xCopyPlaneReq);

    MTX_LOCK_AND_VALIDATE_TWO_DRAWABLES_AND_GC(pSrc, pDst, pGC,
		stuff->srcDrawable, stuff->dstDrawable, stuff->gc, client);

    /* Check to see if stuff->bitPlane has exactly ONE good bit set */
    if(stuff->bitPlane == 0 || (stuff->bitPlane & (stuff->bitPlane - 1)) ||
       (stuff->bitPlane > (1L << (pSrc->depth - 1))))
    {
       client->errorValue = stuff->bitPlane;
        MTX_UNLOCK_TWO_DRAWABLES_AND_GC(pSrc, pDst, pGC, stuff->srcDrawable, 
			            stuff->dstDrawable, stuff->gc, client);
       return(BadValue);
    }

    pRgn = (*pGC->ops->CopyPlane)(pSrc, pDst, pGC, stuff->srcX, stuff->srcY,
				 stuff->width, stuff->height, 
				 stuff->dstX, stuff->dstY, stuff->bitPlane);

    if (pGC->graphicsExposures)
    {
	(*pDst->pScreen->SendGraphicsExpose)
 		(client, pRgn, stuff->dstDrawable, X_CopyPlane, 0);
	if (pRgn)
	    REGION_DESTROY(pDst->pScreen, pRgn);
    }
    MTX_UNLOCK_TWO_DRAWABLES_AND_GC(pSrc, pDst, pGC, stuff->srcDrawable, 
			        stuff->dstDrawable, stuff->gc, client);
    return(client->noClientException);
}

int
ProcPolyPoint(client)
    register ClientPtr client;
{
    int npoint;
    GC *pGC;
    DrawablePtr pDraw;
    REQUEST(xPolyPointReq);

    REQUEST_AT_LEAST_SIZE(xPolyPointReq);

    MTX_LOCK_AND_VALIDATE_DRAWABLE_AND_GC(pDraw, pGC, stuff->drawable, 
					  stuff->gc, client);

    if ((stuff->coordMode != CoordModeOrigin) && 
	(stuff->coordMode != CoordModePrevious))
    {
	client->errorValue = stuff->coordMode;
	MTX_UNLOCK_DRAWABLE_AND_GC(pDraw, pGC, stuff->drawable, stuff->gc, 
				   client);
        return BadValue;
    }
    npoint = ((client->req_len << 2) - sizeof(xPolyPointReq)) >> 2;

    if (npoint)
        (*pGC->ops->PolyPoint)(pDraw, pGC, stuff->coordMode, npoint,
			  (xPoint *) &stuff[1]);

    MTX_UNLOCK_DRAWABLE_AND_GC(pDraw, pGC, stuff->drawable, stuff->gc, client);
    return (client->noClientException);
}

int
ProcPolyLine(client)
    register ClientPtr client;
{
    int npoint;
    GC *pGC;
    DrawablePtr pDraw;
    REQUEST(xPolyLineReq);

    REQUEST_AT_LEAST_SIZE(xPolyLineReq);

    MTX_LOCK_AND_VALIDATE_DRAWABLE_AND_GC(pDraw, pGC, stuff->drawable, 
					  stuff->gc, client);

    if ((stuff->coordMode != CoordModeOrigin) && 
	(stuff->coordMode != CoordModePrevious))
    {
	client->errorValue = stuff->coordMode;
	MTX_UNLOCK_DRAWABLE_AND_GC(pDraw, pGC, stuff->drawable, stuff->gc, 
				   client);
        return BadValue;
    }
    npoint = ((client->req_len << 2) - sizeof(xPolyLineReq)) >> 2;
    if (npoint > 1)
	(*pGC->ops->Polylines)(pDraw, pGC, stuff->coordMode, npoint, 
			      (DDXPointPtr) &stuff[1]);

    MTX_UNLOCK_DRAWABLE_AND_GC(pDraw, pGC, stuff->drawable, stuff->gc, client);
    return(client->noClientException);
}

int
ProcPolySegment(client)
    register ClientPtr client;
{
    int nsegs;
    GC *pGC;
    DrawablePtr pDraw;
    REQUEST(xPolySegmentReq);

    REQUEST_AT_LEAST_SIZE(xPolySegmentReq);

    MTX_LOCK_AND_VALIDATE_DRAWABLE_AND_GC(pDraw, pGC, stuff->drawable, 
					  stuff->gc, client);

    nsegs = (client->req_len << 2) - sizeof(xPolySegmentReq);
    if (nsegs & 4)
    {
	MTX_UNLOCK_DRAWABLE_AND_GC(pDraw, pGC, stuff->drawable, stuff->gc, 
				   client);
	return(BadLength);
    }
    nsegs >>= 3;
    if (nsegs)
        (*pGC->ops->PolySegment)(pDraw, pGC, nsegs, (xSegment *) &stuff[1]);

    MTX_UNLOCK_DRAWABLE_AND_GC(pDraw, pGC, stuff->drawable, stuff->gc, client);
    return (client->noClientException);
}

int
ProcPolyRectangle (client)
    register ClientPtr client;
{
    int nrects;
    GC *pGC;
    DrawablePtr pDraw;
    REQUEST(xPolyRectangleReq);

    REQUEST_AT_LEAST_SIZE(xPolyRectangleReq);

    MTX_LOCK_AND_VALIDATE_DRAWABLE_AND_GC(pDraw, pGC, stuff->drawable, 
					  stuff->gc, client);

    nrects = (client->req_len << 2) - sizeof(xPolyRectangleReq);
    if (nrects & 4)
    {
	MTX_UNLOCK_DRAWABLE_AND_GC(pDraw, pGC, stuff->drawable, stuff->gc, 
				   client);
	return(BadLength);
    }
    nrects >>= 3;
    if (nrects)
        (*pGC->ops->PolyRectangle)(pDraw, pGC, 
		    nrects, (xRectangle *) &stuff[1]);

    MTX_UNLOCK_DRAWABLE_AND_GC(pDraw, pGC, stuff->drawable, stuff->gc, client);
    return(client->noClientException);
}

int
ProcPolyArc(client)
    register ClientPtr client;
{
    int		narcs;
    GC *pGC;
    DrawablePtr pDraw;
    REQUEST(xPolyArcReq);

    REQUEST_AT_LEAST_SIZE(xPolyArcReq);

    MTX_LOCK_AND_VALIDATE_DRAWABLE_AND_GC(pDraw, pGC, stuff->drawable, 
					  stuff->gc, client);

    narcs = (client->req_len << 2) - sizeof(xPolyArcReq);
    if (narcs % sizeof(xArc))
    {
	MTX_UNLOCK_DRAWABLE_AND_GC(pDraw, pGC, stuff->drawable, stuff->gc, 
				   client);
	return(BadLength);
    }
    narcs /= sizeof(xArc);
    if (narcs)
        (*pGC->ops->PolyArc)(pDraw, pGC, narcs, (xArc *) &stuff[1]);

    MTX_UNLOCK_DRAWABLE_AND_GC(pDraw, pGC, stuff->drawable, stuff->gc, client);
    return (client->noClientException);
}

int
ProcFillPoly(client)
    register ClientPtr client;
{
    int          things;
    GC *pGC;
    DrawablePtr pDraw;
    REQUEST(xFillPolyReq);

    REQUEST_AT_LEAST_SIZE(xFillPolyReq);

    MTX_LOCK_AND_VALIDATE_DRAWABLE_AND_GC(pDraw, pGC, stuff->drawable, 
					  stuff->gc, client);

    if ((stuff->shape != Complex) && (stuff->shape != Nonconvex) &&  
	(stuff->shape != Convex))
    {
	client->errorValue = stuff->shape;
	MTX_UNLOCK_DRAWABLE_AND_GC(pDraw, pGC, stuff->drawable, stuff->gc, 
				   client);
        return BadValue;
    }
    if ((stuff->coordMode != CoordModeOrigin) && 
	(stuff->coordMode != CoordModePrevious))
    {
	client->errorValue = stuff->coordMode;
	MTX_UNLOCK_DRAWABLE_AND_GC(pDraw, pGC, stuff->drawable, stuff->gc, 
				   client);
        return BadValue;
    }

    things = ((client->req_len << 2) - sizeof(xFillPolyReq)) >> 2;
    if (things)
        (*pGC->ops->FillPolygon) (pDraw, pGC, stuff->shape,
			 stuff->coordMode, things,
			 (DDXPointPtr) &stuff[1]);

    MTX_UNLOCK_DRAWABLE_AND_GC(pDraw, pGC, stuff->drawable, stuff->gc, client);
    return(client->noClientException);
}

int
ProcPolyFillRectangle(client)
    register ClientPtr client;
{
    int             things;
    GC *pGC;
    DrawablePtr pDraw;
    REQUEST(xPolyFillRectangleReq);

    REQUEST_AT_LEAST_SIZE(xPolyFillRectangleReq);

    MTX_LOCK_AND_VALIDATE_DRAWABLE_AND_GC(pDraw, pGC, stuff->drawable, 
					  stuff->gc, client);

    things = (client->req_len << 2) - sizeof(xPolyFillRectangleReq);
    if (things & 4)
    {
	MTX_UNLOCK_DRAWABLE_AND_GC(pDraw, pGC, stuff->drawable, stuff->gc, 
				   client);
	return(BadLength);
    }
    things >>= 3;
    if (things)
        (*pGC->ops->PolyFillRect) (pDraw, pGC, things,
		      (xRectangle *) &stuff[1]);

    MTX_UNLOCK_DRAWABLE_AND_GC(pDraw, pGC, stuff->drawable, stuff->gc, client);
    return (client->noClientException);
}

int
ProcPolyFillArc(client)
    register ClientPtr client;
{
    int		narcs;
    GC *pGC;
    DrawablePtr pDraw;
    REQUEST(xPolyFillArcReq);

    REQUEST_AT_LEAST_SIZE(xPolyFillArcReq);

    MTX_LOCK_AND_VALIDATE_DRAWABLE_AND_GC(pDraw, pGC, stuff->drawable, 
					  stuff->gc, client);

    narcs = (client->req_len << 2) - sizeof(xPolyFillArcReq);
    if (narcs % sizeof(xArc))
    {
	MTX_UNLOCK_DRAWABLE_AND_GC(pDraw, pGC, stuff->drawable, stuff->gc, 
				   client);
	return(BadLength);
    }
    narcs /= sizeof(xArc);
    if (narcs)
        (*pGC->ops->PolyFillArc) (pDraw, pGC, narcs, (xArc *) &stuff[1]);

    MTX_UNLOCK_DRAWABLE_AND_GC(pDraw, pGC, stuff->drawable, stuff->gc, client);
    return (client->noClientException);
}

int
ProcPutImage(client)
    register ClientPtr client;
{
    GC *pGC;
    DrawablePtr pDraw;
    long length;
    REQUEST(xPutImageReq);

    REQUEST_AT_LEAST_SIZE(xPutImageReq);

    MTX_LOCK_AND_VALIDATE_DRAWABLE_AND_GC(pDraw, pGC, stuff->drawable, 
					  stuff->gc, client);

    if (stuff->format == XYBitmap)
    {
        if ((stuff->depth != 1) ||
	    (stuff->leftPad >= screenInfo.bitmapScanlinePad))
	{
	    MTX_UNLOCK_DRAWABLE_AND_GC(pDraw, pGC, stuff->drawable, stuff->gc,
				       client);
            return BadMatch;
	}
        length = BitmapBytePad(stuff->width + stuff->leftPad);
    }
    else if (stuff->format == XYPixmap)
    {
        if ((pDraw->depth != stuff->depth) || 
	    (stuff->leftPad >= screenInfo.bitmapScanlinePad))
	{
	    MTX_UNLOCK_DRAWABLE_AND_GC(pDraw, pGC, stuff->drawable, stuff->gc,
				       client);
            return BadMatch;
	}
        length = BitmapBytePad(stuff->width + stuff->leftPad);
	length *= stuff->depth;
    }
    else if (stuff->format == ZPixmap)
    {
        if ((pDraw->depth != stuff->depth) || (stuff->leftPad != 0))
	{
	    MTX_UNLOCK_DRAWABLE_AND_GC(pDraw, pGC, stuff->drawable, stuff->gc,
				       client);
            return BadMatch;
	}
        length = PixmapBytePad(stuff->width, stuff->depth);
    }
    else
    {
	client->errorValue = stuff->format;
	MTX_UNLOCK_DRAWABLE_AND_GC(pDraw, pGC, stuff->drawable, stuff->gc,
				   client);
        return BadValue;
    }
    length *= stuff->height;
    if ((((length + 3) >> 2) + (sizeof(xPutImageReq) >> 2)) != client->req_len)
    {
	MTX_UNLOCK_DRAWABLE_AND_GC(pDraw, pGC, stuff->drawable, stuff->gc, 
				   client);
	return BadLength;
    }
    (*pGC->ops->PutImage) (pDraw, pGC, stuff->depth, stuff->dstX, stuff->dstY,
		  stuff->width, stuff->height, 
		  stuff->leftPad, stuff->format, 
		  (char *) &stuff[1]);

     MTX_UNLOCK_DRAWABLE_AND_GC(pDraw, pGC, stuff->drawable, stuff->gc, 
				client);

     return (client->noClientException);
}

int
ProcGetImage(client)
    register ClientPtr	client;
{
    DrawablePtr pDraw;
    int			nlines, linesPerBuf;
    register int	height, linesDone;
    long		widthBytesLine, length;
    Mask		plane;
    char		*pBuf;
    REPLY_DECL(xGetImageReply,xgi);

    REQUEST(xGetImageReq);
    REQUEST_SIZE_MATCH(xGetImageReq);

    MTX_REP_CHECK_RETURN(xgi,BadAlloc);

    MTX_REP_LOCK_AND_VERIFY_DRAWABLE(pDraw,stuff->drawable,client,CM_XGetImage);

    height = stuff->height;
    if ((stuff->format != XYPixmap) && (stuff->format != ZPixmap))
    {
	client->errorValue = stuff->format;
	MTX_UNLOCK_DRAWABLE (pDraw, stuff->drawable, client);
        return(BadValue);
    }
    if(pDraw->type == DRAWABLE_WINDOW)
    {
      if( /* check for being viewable */
	 !((WindowPtr) pDraw)->realized ||
	  /* check for being on screen */
         pDraw->x + stuff->x < 0 ||
 	 pDraw->x + stuff->x + (int)stuff->width > pDraw->pScreen->width ||
         pDraw->y + stuff->y < 0 ||
         pDraw->y + stuff->y + height > pDraw->pScreen->height ||
          /* check for being inside of border */
         stuff->x < - wBorderWidth((WindowPtr)pDraw) ||
         stuff->x + (int)stuff->width >
		wBorderWidth((WindowPtr)pDraw) + (int)pDraw->width ||
         stuff->y < -wBorderWidth((WindowPtr)pDraw) ||
         stuff->y + height >
		wBorderWidth ((WindowPtr)pDraw) + (int)pDraw->height
        )
	{
	    MTX_UNLOCK_DRAWABLE (pDraw, stuff->drawable, client);
	    return(BadMatch);
	}
	xgi->visual = wVisual (((WindowPtr) pDraw));
    }
    else
    {
      if(stuff->x < 0 ||
         stuff->x+(int)stuff->width > pDraw->width ||
         stuff->y < 0 ||
         stuff->y+height > pDraw->height
        )
	{
	    MTX_UNLOCK_DRAWABLE (pDraw, stuff->drawable, client);
	    return(BadMatch);
	}
	xgi->visual = None;
    }

    xgi->type = X_Reply;
    xgi->sequenceNumber = client->sequence;
    xgi->depth = pDraw->depth;

    if(stuff->format == ZPixmap)
    {
	widthBytesLine = PixmapBytePad(stuff->width, pDraw->depth);
	length = widthBytesLine * height;
    }
    else 
    {
	widthBytesLine = BitmapBytePad(stuff->width);
	plane = ((Mask)1) << (pDraw->depth - 1);
	/* only planes asked for */
	length = widthBytesLine * height *
		 Ones(stuff->planeMask & (plane | (plane - 1)));
    }
    xgi->length = (length + 3) >> 2;

/* XXX:SM Arrrg come back to this later. */

    if (widthBytesLine == 0 || height == 0)
	linesPerBuf = 0;
    else if (widthBytesLine >= IMAGE_BUFSIZE)
	linesPerBuf = 1;
    else
    {
	linesPerBuf = IMAGE_BUFSIZE / widthBytesLine;
	if (linesPerBuf > height)
	    linesPerBuf = height;
    }
    length = linesPerBuf * widthBytesLine;
    if (linesPerBuf < height)
    {
	/* we have to make sure intermediate buffers don't need padding */
	while ((linesPerBuf > 1) && (length & 3))
	{
	    linesPerBuf--;
	    length -= widthBytesLine;
	}
	while (length & 3)
	{
	    linesPerBuf++;
	    length += widthBytesLine;
	}
    }
    if(!(pBuf = (char *) ALLOCATE_LOCAL(length)))
        return (BadAlloc);

    WriteReplyToClient(client, sizeof (xGetImageReply), xgi);

    if (linesPerBuf == 0)
    {
	/* nothing to do */
    }
    else if (stuff->format == ZPixmap)
    {
        linesDone = 0;
        while (height - linesDone > 0)
        {
	    nlines = min(linesPerBuf, height - linesDone);
	    (*pDraw->pScreen->GetImage) (pDraw,
	                                 stuff->x,
				         stuff->y + linesDone,
				         stuff->width, 
				         nlines,
				         stuff->format,
				         stuff->planeMask,
				         (pointer) pBuf);
	    /* Note that this is NOT a call to WriteSwappedDataToClient,
               as we do NOT byte swap */
	    (void)WriteToClient(client, (int)(nlines * widthBytesLine), pBuf);
	    linesDone += nlines;
        }
    }
    else
    {
        for (; plane; plane >>= 1)
	{
	    if (stuff->planeMask & plane)
	    {
	        linesDone = 0;
	        while (height - linesDone > 0)
	        {
		    nlines = min(linesPerBuf, height - linesDone);
	            (*pDraw->pScreen->GetImage) (pDraw,
	                                         stuff->x,
				                 stuff->y + linesDone,
				                 stuff->width, 
				                 nlines,
				                 stuff->format,
				                 plane,
				                 (pointer)pBuf);
		    /* Note: NOT a call to WriteSwappedDataToClient,
		       as we do NOT byte swap */
		    (void)WriteToClient(client, (int)(nlines * widthBytesLine),
					pBuf);
		    linesDone += nlines;
		}
            }
	}
    }
    DEALLOCATE_LOCAL(pBuf);
    return (client->noClientException);
}


int
ProcPolyText(client)
    register ClientPtr client;
{
    int	err;
    REQUEST(xPolyTextReq);
    DrawablePtr pDraw;
    GC *pGC;

    REQUEST_AT_LEAST_SIZE(xPolyTextReq);

    MTX_LOCK_AND_VALIDATE_DRAWABLE_AND_GC(pDraw, pGC, stuff->drawable, 
					  stuff->gc, client);

    err = PolyText(client,
		   pDraw,
		   pGC,
		   (unsigned char *)&stuff[1],
		   ((unsigned char *) stuff) + (client->req_len << 2),
		   stuff->x,
		   stuff->y,
		   stuff->reqType,
		   stuff->drawable);

    if (err == Success)
    {
	MTX_UNLOCK_DRAWABLE_AND_GC(pDraw, pGC, stuff->drawable, stuff->gc,
				  client);
	return(client->noClientException);
    }
    else
    {
	MTX_UNLOCK_DRAWABLE_AND_GC(pDraw, pGC, stuff->drawable, stuff->gc,
				  client);
	return err;
    }
}

int
ProcImageText8(client)
    register ClientPtr client;
{
    int	err;
    DrawablePtr pDraw;
    GC *pGC;

    REQUEST(xImageTextReq);

    REQUEST_FIXED_SIZE(xImageTextReq, stuff->nChars);

    MTX_LOCK_AND_VALIDATE_DRAWABLE_AND_GC(pDraw, pGC, stuff->drawable, 
					  stuff->gc, client);

    err = ImageText(client,
		    pDraw,
		    pGC,
		    stuff->nChars,
		    (unsigned char *)&stuff[1],
		    stuff->x,
		    stuff->y,
		    stuff->reqType,
		    stuff->drawable);

    if (err == Success)
    {
	MTX_UNLOCK_DRAWABLE_AND_GC(pDraw, pGC, stuff->drawable, stuff->gc, 
				   client);
	return(client->noClientException);
    }
    else
    {
	MTX_UNLOCK_DRAWABLE_AND_GC(pDraw, pGC, stuff->drawable, stuff->gc, 
				   client);
	return err;
    }
}

int
ProcImageText16(client)
    register ClientPtr client;
{
    int	err;
    DrawablePtr pDraw;
    GC *pGC;

    REQUEST(xImageTextReq);

    REQUEST_FIXED_SIZE(xImageTextReq, stuff->nChars << 1);

    MTX_LOCK_AND_VALIDATE_DRAWABLE_AND_GC(pDraw, pGC, stuff->drawable, 
					  stuff->gc, client);
    err = ImageText(client,
		    pDraw,
		    pGC,
		    stuff->nChars,
		    (unsigned char *)&stuff[1],
		    stuff->x,
		    stuff->y,
		    stuff->reqType,
		    stuff->drawable);

    if (err == Success)
    {
	MTX_UNLOCK_DRAWABLE_AND_GC(pDraw, pGC, stuff->drawable, stuff->gc, 
				   client);
	return(client->noClientException);
    }
    else
    {
	MTX_UNLOCK_DRAWABLE_AND_GC(pDraw, pGC, stuff->drawable, stuff->gc, 
				   client);
	return err;
    }
}


int
ProcCreateColormap(client)
    register ClientPtr client;
{
    VisualPtr	pVisual;
    ColormapPtr	pmap;
    Colormap	mid;
    WindowPtr   pWin;
    ScreenPtr pScreen;
    REQUEST(xCreateColormapReq);
    int i, result;

    REQUEST_SIZE_MATCH(xCreateColormapReq);

    mid = stuff->mid;
    LEGAL_NEW_RESOURCE(mid, client);
    MTX_LOCK_AND_VERIFY_WINDOW(pWin, stuff->window, client, POQ_NULL_REGION,
			       CM_XCreateColormap);

    if ((stuff->alloc != AllocNone) && (stuff->alloc != AllocAll))
    {
	client->errorValue = stuff->alloc;
	MTX_UNLOCK_WINDOW(pWin, stuff->window, client);
        return(BadValue);
    }

    pScreen = pWin->drawable.pScreen;
    for (i = 0, pVisual = pScreen->visuals;
	 i < pScreen->numVisuals;
	 i++, pVisual++)
    {
	if (pVisual->vid != stuff->visual)
	    continue;
	result =  CreateColormap(mid, pScreen, pVisual, &pmap,
				 (int)stuff->alloc, client->index);
	if (client->noClientException != Success)
	{
	    MTX_UNLOCK_WINDOW(pWin, stuff->window, client);
	    return(client->noClientException);
	}
	else
	{
	    MTX_UNLOCK_WINDOW(pWin, stuff->window, client);
	    return(result);
	}
    }
    client->errorValue = stuff->visual;
    MTX_UNLOCK_WINDOW(pWin, stuff->window, client);
    return(BadValue);
}

int
ProcFreeColormap(client)
    register ClientPtr client;
{
    ColormapPtr pmap;
    REQUEST(xResourceReq);

    REQUEST_SIZE_MATCH(xResourceReq);

    MTX_LOCK_AND_VERIFY_COLORMAP(pmap, stuff->id, client, CM_XFreeColormap);

/*
 * XXX: Right now the function UnlockColormap is MTX specific and resides in
 *      resource.c.  It's just an empty function, so I'm not going to bother
 *      calling it.
 *  UnlockColormap(pmap, stuff->id);
 */

    /* Freeing a default colormap is a no-op */
    if (!(pmap->flags & IsDefault))
	FreeResource(stuff->id, RT_NONE);
    return (client->noClientException);
}


int
ProcCopyColormapAndFree(client)
    register ClientPtr client;
{
    Colormap	mid;
    ColormapPtr	pSrcMap;
    REQUEST(xCopyColormapAndFreeReq);
    int result;

    REQUEST_SIZE_MATCH(xCopyColormapAndFreeReq);

    mid = stuff->mid;
    LEGAL_NEW_RESOURCE(mid, client);
    MTX_LOCK_AND_VERIFY_COLORMAP(pSrcMap, stuff->srcCmap, client, 
			     CM_XCopyColormapAndFree);
    result = CopyColormapAndFree(mid, pSrcMap, client->index);
    MTX_UNLOCK_COLORMAP(pSrcMap, stuff->srcCmap, client);
    if (client->noClientException != Success)
	return(client->noClientException);
    else
	return(result);
}

int
ProcInstallColormap(client)
    register ClientPtr client;
{
    ColormapPtr pcmp;
    REQUEST(xResourceReq);

    REQUEST_SIZE_MATCH(xResourceReq);

    MTX_LOCK_AND_VERIFY_COLORMAP(pcmp, stuff->id, client, CM_XInstallColormap);

    (*(pcmp->pScreen->InstallColormap)) (pcmp);
    MTX_UNLOCK_COLORMAP(pcmp, stuff->id, client);
    return (client->noClientException);        
}

int
ProcUninstallColormap(client)
    register ClientPtr client;
{
    ColormapPtr pcmp;
    REQUEST(xResourceReq);

    REQUEST_SIZE_MATCH(xResourceReq);

    MTX_LOCK_AND_VERIFY_COLORMAP(pcmp,stuff->id,client,CM_XUninstallColormap);
    if(pcmp->mid != pcmp->pScreen->defColormap)
	(*(pcmp->pScreen->UninstallColormap)) (pcmp);
    MTX_UNLOCK_COLORMAP(pcmp, stuff->id, client);
    return (client->noClientException);        
}

int
ProcListInstalledColormaps(client)
    register ClientPtr client;
{
    REPLY_DECL(xListInstalledColormapsReply,preply);
    int nummaps;
    WindowPtr pWin;
    REQUEST(xResourceReq);
#ifdef XTHREADS
    Colormap *colorMaps;
    char *replyMaps;
    int sizeMaps;
    int maxMaps;
#endif /* XTHREADS */

    MTX_REP_CHECK_RETURN(preply,BadAlloc);

    REQUEST_SIZE_MATCH(xResourceReq);

    MTX_REP_LOCK_AND_VERIFY_WINDOW(pWin, stuff->id, client, POQ_NULL_REGION,
			       CM_XListInstalledColormaps);

#ifndef XTHREADS	/* ZZZ XXX: Sid wishes there were a better way */
    preply = (xListInstalledColormapsReply *) 
		ALLOCATE_LOCAL(sizeof(xListInstalledColormapsReply) +
		     pWin->drawable.pScreen->maxInstalledCmaps *
		     sizeof(Colormap));
    if(!preply)
        return(BadAlloc);

    preply->type = X_Reply;
    preply->sequenceNumber = client->sequence;
    nummaps = (*pWin->drawable.pScreen->ListInstalledColormaps)
        (pWin->drawable.pScreen, (Colormap *)&preply[1]);

    preply->nColormaps = nummaps;
    preply->length = nummaps;

    WriteReplyToClient(client, sizeof (xListInstalledColormapsReply), preply);

    client->pSwapReplyFunc = Swap32Write;
    WriteSwappedDataToClient(client, nummaps * sizeof(Colormap), &preply[1]);

    DEALLOCATE_LOCAL(preply);
#else /* XTHREADS */
    maxMaps = pWin->drawable.pScreen->maxInstalledCmaps;
    colorMaps = (Colormap *) ALLOCATE_LOCAL(maxMaps * sizeof(Colormap));
    if (!colorMaps)
    {
        MTXReturnPooledMessage;
        MTX_UNLOCK_WINDOW(pWin, stuff->id, client);
        return BadAlloc;
    }

    nummaps = (*pWin->drawable.pScreen->ListInstalledColormaps)
                         (pWin->drawable.pScreen, colorMaps);

    sizeMaps = nummaps * sizeof(Colormap);
    replyMaps = (char *) xalloc(sizeMaps);
    if (!replyMaps)
    {
        MTXReturnPooledMessage;
        DEALLOCATE_LOCAL(colorMaps);
        return BadAlloc;
    }

    bcopy(colorMaps, replyMaps, sizeMaps);
    DEALLOCATE_LOCAL(colorMaps);
    if (client->swapped)
        Swap32(sizeMaps, replyMaps);

    preply->type = X_Reply;
    preply->sequenceNumber = client->sequence;

    preply->nColormaps = nummaps;
    preply->length = nummaps;

    msg->pReplyData = replyMaps;
    msg->freeReplyData = TRUE;
    msg->lenReplyData = sizeMaps;

    SendReplyToClient(client, msg);
#endif
    MTX_UNLOCK_WINDOW(pWin, stuff->id, client);
    return(client->noClientException);
}

int
ProcAllocColor                (client)
    register ClientPtr client;
{
    ColormapPtr pmap;
    int	retval;
    REPLY_DECL(xAllocColorReply,acr);
    REQUEST(xAllocColorReq);

    REQUEST_SIZE_MATCH(xAllocColorReq);

    MTX_REP_CHECK_RETURN(acr,BadAlloc);

    MTX_REP_LOCK_AND_VERIFY_COLORMAP(pmap, stuff->cmap, client, CM_XAllocColor);

    acr->type = X_Reply;
    acr->length = 0;
    acr->sequenceNumber = client->sequence;
    acr->red = stuff->red;
    acr->green = stuff->green;
    acr->blue = stuff->blue;
    acr->pixel = 0;

    if( (retval = AllocColor(pmap, &(acr->red), &(acr->green), &(acr->blue),
			     &(acr->pixel), client->index)) )
    {
	MTXReturnPooledMessage;
	MTX_UNLOCK_COLORMAP(pmap, stuff->cmap, client);
	if (client->noClientException != Success)
	    return(client->noClientException);
	else
	    return (retval);
    }
    WriteReplyToClient(client, sizeof(xAllocColorReply), acr);
    MTX_UNLOCK_COLORMAP(pmap, stuff->cmap, client);
    return (client->noClientException);
}

int
ProcAllocNamedColor           (client)
    register ClientPtr client;
{
    ColormapPtr pcmp;
    int		retval;
    REPLY_DECL(xAllocNamedColorReply,ancr);

    REQUEST(xAllocNamedColorReq);
    REQUEST_FIXED_SIZE(xAllocNamedColorReq, stuff->nbytes);

    MTX_REP_CHECK_RETURN(ancr,BadAlloc);

    MTX_REP_LOCK_AND_VERIFY_COLORMAP(pcmp,stuff->cmap,client,CM_XAllocNamedColor);

    ancr->type = X_Reply;
    ancr->length = 0;
    ancr->sequenceNumber = client->sequence;

    if(OsLookupColor(pcmp->pScreen->myNum, (char *)&stuff[1], stuff->nbytes,
		 &ancr->exactRed, &ancr->exactGreen, &ancr->exactBlue))
    {
	ancr->screenRed = ancr->exactRed;
	ancr->screenGreen = ancr->exactGreen;
	ancr->screenBlue = ancr->exactBlue;
	ancr->pixel = 0;

	if(retval = AllocColor(pcmp,
		 &ancr->screenRed, &ancr->screenGreen,
		 &ancr->screenBlue, &ancr->pixel,
		 client->index))
	{
	    MTXReturnPooledMessage;
	    MTX_UNLOCK_COLORMAP(pcmp, stuff->cmap, client);
	    if (client->noClientException != Success)
		return(client->noClientException);
	    else
		return(retval);
	}
	WriteReplyToClient(client, sizeof (xAllocNamedColorReply), ancr);
	MTX_UNLOCK_COLORMAP(pcmp, stuff->cmap, client);
	return (client->noClientException);
    }
    else
    {
	MTX_UNLOCK_COLORMAP(pcmp, stuff->cmap, client);
	return(BadName);
    }
}

int
ProcAllocColorCells           (client)
    register ClientPtr client;
{
    ColormapPtr pcmp;
    REPLY_DECL(xAllocColorCellsReply,accr);
    int				npixels, nmasks, retval;
    long			length;
    unsigned long		*ppixels, *pmasks;
    REQUEST(xAllocColorCellsReq);

    REQUEST_SIZE_MATCH(xAllocColorCellsReq);

    MTX_REP_CHECK_RETURN(accr,BadAlloc);

    MTX_REP_LOCK_AND_VERIFY_COLORMAP(pcmp,stuff->cmap,client,CM_XAllocColorCells);

    npixels = stuff->colors;
    if (!npixels)
    {
	client->errorValue = npixels;
	MTXReturnPooledMessage;
	MTX_UNLOCK_COLORMAP(pcmp, stuff->cmap, client);
	return (BadValue);
    }

    if (stuff->contiguous != xTrue && stuff->contiguous != xFalse)
    {
	client->errorValue = stuff->contiguous;
	MTXReturnPooledMessage;
	MTX_UNLOCK_COLORMAP(pcmp, stuff->cmap, client);
	return (BadValue);
    }

    nmasks = stuff->planes;
    length = ((long)npixels + (long)nmasks) * sizeof(Pixel);
    ppixels = (Pixel *)MTX_LOCAL_ALLOC(length);
    if(!ppixels)
    {
	MTXReturnPooledMessage;
	MTX_UNLOCK_COLORMAP(pcmp, stuff->cmap, client);
	return(BadAlloc);
    }
    pmasks = ppixels + npixels;

    retval = AllocColorCells(client->index, pcmp, npixels, nmasks, 
			 (Bool)stuff->contiguous, ppixels, pmasks);
    if (retval)
    {
	MTX_LOCAL_FREE(ppixels);
	MTX_UNLOCK_COLORMAP(pcmp, stuff->cmap, client);
	MTXReturnPooledMessage;
	if (client->noClientException != Success)
	    return(client->noClientException);
	else
	    return(retval);
    }
    accr->type = X_Reply;
    accr->length = length >> 2;
    accr->sequenceNumber = client->sequence;
    accr->nPixels = npixels;
    accr->nMasks = nmasks;
#ifndef XTHREADS
    WriteReplyToClient(client, sizeof (xAllocColorCellsReply), accr);
    client->pSwapReplyFunc = Swap32Write;
    WriteSwappedDataToClient(client, length, ppixels);
    MTX_LOCAL_FREE(ppixels);
#else /* XTHREADS */
    msg->pReplyData = (char *) ppixels;
    msg->freeReplyData = TRUE;
    msg->lenReplyData = length;
    if (client->swapped)
	Swap32(length, ppixels);
    SendReplyToClient(client, msg);
#endif /* XTHREADS */
    MTX_UNLOCK_COLORMAP(pcmp, stuff->cmap, client);
    return (client->noClientException);        
}

int
ProcAllocColorPlanes(client)
    register ClientPtr client;
{
    ColormapPtr pcmp;
    REPLY_DECL(xAllocColorPlanesReply,acpr);
    int			npixels, retval;
    long			length;
    unsigned long		*ppixels;
    REQUEST(xAllocColorPlanesReq);

    REQUEST_SIZE_MATCH(xAllocColorPlanesReq);

    MTX_REP_CHECK_RETURN(acpr,BadAlloc);

    MTX_REP_LOCK_AND_VERIFY_COLORMAP(pcmp,stuff->cmap,client,CM_XAllocColorPlanes);

    npixels = stuff->colors;
    if (!npixels)
    {
	client->errorValue = npixels;
	MTXReturnPooledMessage;
	MTX_UNLOCK_COLORMAP(pcmp, stuff->cmap, client);
	return (BadValue);
    }

    if (stuff->contiguous != xTrue && stuff->contiguous != xFalse)
    {
	client->errorValue = stuff->contiguous;
	MTXReturnPooledMessage;
	MTX_UNLOCK_COLORMAP(pcmp, stuff->cmap, client);
	return (BadValue);
    }
    acpr->type = X_Reply;
    acpr->sequenceNumber = client->sequence;
    acpr->nPixels = npixels;

    length = (long)npixels * sizeof(Pixel);

    ppixels = (Pixel *)MTX_LOCAL_ALLOC(length);
    if(!ppixels)
    {
	MTXReturnPooledMessage;
	MTX_UNLOCK_COLORMAP(pcmp, stuff->cmap, client);
	return(BadAlloc);
    }
    retval = AllocColorPlanes(client->index, pcmp, npixels,
	(int)stuff->red, (int)stuff->green, (int)stuff->blue,
	(int)stuff->contiguous, ppixels,
	&acpr->redMask, &acpr->greenMask, &acpr->blueMask);
    if (retval)
    {
	MTX_LOCAL_FREE(ppixels);
	MTXReturnPooledMessage;
	MTX_UNLOCK_COLORMAP(pcmp, stuff->cmap, client);
	if (client->noClientException != Success)
	    return(client->noClientException);
	else
	    return(retval);
    }

    acpr->length = length >> 2;

#ifndef XTHREADS
    WriteReplyToClient(client, sizeof(xAllocColorPlanesReply), acpr);
    client->pSwapReplyFunc = Swap32Write;
    WriteSwappedDataToClient(client, length, ppixels);
    MTX_LOCAL_FREE(ppixels);
#else /* XTHREADS */
    msg->pReplyData = (char *) ppixels;
    msg->freeReplyData = TRUE;
    msg->lenReplyData = length;
    if (client->swapped)
	Swap32(length, ppixels);

    SendReplyToClient(client, msg);
#endif /* XTHREADS */
    MTX_UNLOCK_COLORMAP(pcmp, stuff->cmap, client);
    return (client->noClientException);        
}

int
ProcFreeColors          (client)
    register ClientPtr client;
{
    ColormapPtr pcmp;
    int	count;
    int     retval;
    REQUEST(xFreeColorsReq);

    REQUEST_AT_LEAST_SIZE(xFreeColorsReq);
    MTX_LOCK_AND_VERIFY_COLORMAP(pcmp, stuff->cmap, client, CM_XFreeColors);

    if(pcmp->flags & AllAllocated)
    {
	MTX_UNLOCK_COLORMAP(pcmp, stuff->cmap, client);
	return(BadAccess);
    }
    count = ((client->req_len << 2)- sizeof(xFreeColorsReq)) >> 2;
    retval =  FreeColors(pcmp, client->index, count,
	(unsigned long *)&stuff[1], stuff->planeMask);
    MTX_UNLOCK_COLORMAP(pcmp, stuff->cmap, client);
    if (client->noClientException != Success)
	return(client->noClientException);
    else
    {
	/* ZZZ XXX RTC - clientErrorValue used here */
	client->errorValue = clientErrorValue;
	return(retval);
    }
}

int
ProcStoreColors               (client)
    register ClientPtr client;
{
    ColormapPtr pcmp;
    int	count;
    int     retval;
    REQUEST(xStoreColorsReq);

    REQUEST_AT_LEAST_SIZE(xStoreColorsReq);
    MTX_LOCK_AND_VERIFY_COLORMAP(pcmp, stuff->cmap, client, CM_XStoreColors);

    count = (client->req_len << 2) - sizeof(xStoreColorsReq);
    if (count % sizeof(xColorItem))
    {
	MTX_UNLOCK_COLORMAP(pcmp, stuff->cmap, client);
	return(BadLength);
    }
    count /= sizeof(xColorItem);
    retval = StoreColors(pcmp, count, (xColorItem *)&stuff[1]);
    MTX_UNLOCK_COLORMAP(pcmp, stuff->cmap, client);
    if (client->noClientException != Success)
	return(client->noClientException);
    else
    {
	/* ZZZ XXX RTC - clientErrorValue used here */
	client->errorValue = clientErrorValue;
	return(retval);
    }
}

int
ProcStoreNamedColor           (client)
    register ClientPtr client;
{
    ColormapPtr pcmp;
    xColorItem	def;
    int             retval;
    REQUEST(xStoreNamedColorReq);

    REQUEST_FIXED_SIZE(xStoreNamedColorReq, stuff->nbytes);
    MTX_LOCK_AND_VERIFY_COLORMAP(pcmp,stuff->cmap,client,CM_XStoreNamedColor);

    if(OsLookupColor(pcmp->pScreen->myNum, (char *)&stuff[1],
		     stuff->nbytes, &def.red, &def.green, &def.blue))
    {
	def.flags = stuff->flags;
	def.pixel = stuff->pixel;
	retval = StoreColors(pcmp, 1, &def);
	MTX_UNLOCK_COLORMAP(pcmp, stuff->cmap, client);
	if (client->noClientException != Success)
	    return(client->noClientException);
	return(retval);
    }
    MTX_UNLOCK_COLORMAP(pcmp, stuff->cmap, client);
    return (BadName);        
}

int
ProcQueryColors(client)
    register ClientPtr client;
{
    ColormapPtr pcmp;
    REQUEST(xQueryColorsReq);
    REPLY_DECL(xQueryColorsReply,qcr);
    int			count, retval;
    xrgb 			*prgbs;

    REQUEST_AT_LEAST_SIZE(xQueryColorsReq);

    MTX_REP_CHECK_RETURN(qcr,BadAlloc);

    MTX_REP_LOCK_AND_VERIFY_COLORMAP(pcmp, stuff->cmap, client, CM_XQueryColors);

    count = ((client->req_len << 2) - sizeof(xQueryColorsReq)) >> 2;
    prgbs = (xrgb *)MTX_LOCAL_ALLOC(count * sizeof(xrgb));
    if(!prgbs && count)
    {
	MTXReturnPooledMessage;
	MTX_UNLOCK_COLORMAP(pcmp, stuff->cmap, client);
	return(BadAlloc);
    }
    retval = QueryColors(pcmp, count, (unsigned long *)&stuff[1], prgbs);
    if (retval)
    {
	if (prgbs)
	    MTX_LOCAL_FREE(prgbs);
	MTX_UNLOCK_COLORMAP(pcmp, stuff->cmap, client);

	if (client->noClientException != Success)
	    return(client->noClientException);

	/* ZZZ XXX RTC - clientErrorValue used here */
	client->errorValue = clientErrorValue;
	return (retval);
    }
    qcr->type = X_Reply;
    qcr->length = (count * sizeof(xrgb)) >> 2;
    qcr->sequenceNumber = client->sequence;
    qcr->nColors = count;
#ifdef XTHREADS
    msg->pReplyData = (char *) prgbs;
    msg->freeReplyData = TRUE;
    msg->lenReplyData = count * sizeof(xrgb);
    if (client->swapped)
	SQColorsExtend(client, count * sizeof(xrgb), prgbs);
    SendReplyToClient(client, msg);
#endif /* XTHREADS */
    MTX_UNLOCK_COLORMAP(pcmp, stuff->cmap, client);
    WriteReplyToClient(client, sizeof(xQueryColorsReply), qcr);
#ifndef XTHREADS
    if (count)
    {
	client->pSwapReplyFunc = SQColorsExtend;
	WriteSwappedDataToClient(client, count * sizeof(xrgb), prgbs);
    }
    if (prgbs) DEALLOCATE_LOCAL(prgbs);
#endif /* XTHREADS */
    return(client->noClientException);
} 

int
ProcLookupColor(client)
    register ClientPtr client;
{
    ColormapPtr pcmp;
    REPLY_DECL(xLookupColorReply,lcr);
    REQUEST(xLookupColorReq);

    REQUEST_FIXED_SIZE(xLookupColorReq, stuff->nbytes);

    MTX_REP_CHECK_RETURN(lcr,BadAlloc);

    MTX_REP_LOCK_AND_VERIFY_COLORMAP(pcmp,stuff->cmap,client,CM_XLookupColor);

    if(OsLookupColor(pcmp->pScreen->myNum, (char *)&stuff[1], stuff->nbytes,
		 &lcr->exactRed, &lcr->exactGreen, &lcr->exactBlue))
    {
	lcr->type = X_Reply;
	lcr->length = 0;
	lcr->sequenceNumber = client->sequence;
	lcr->screenRed = lcr->exactRed;
	lcr->screenGreen = lcr->exactGreen;
	lcr->screenBlue = lcr->exactBlue;
	(*pcmp->pScreen->ResolveColor)(&lcr->screenRed,
				       &lcr->screenGreen,
				       &lcr->screenBlue,
				       pcmp->pVisual);
	WriteReplyToClient(client, sizeof(xLookupColorReply), lcr);
	MTX_UNLOCK_COLORMAP(pcmp, stuff->cmap, client);
	return(client->noClientException);
    }
    MTXReturnPooledMessage;
    MTX_UNLOCK_COLORMAP(pcmp, stuff->cmap, client);
    return (BadName);        
}

int
ProcCreateCursor( client)
    register ClientPtr client;
{
    CursorPtr	pCursor;

    register PixmapPtr 	src;
    register PixmapPtr 	msk;
    unsigned char *	srcbits;
    unsigned char *	mskbits;
    unsigned short	width, height;
    long		n;
    CursorMetricRec cm;


    REQUEST(xCreateCursorReq);

    REQUEST_SIZE_MATCH(xCreateCursorReq);
    LEGAL_NEW_RESOURCE(stuff->cid, client);

/*** XXX:SM ??? - It didn't look like the DG guys were sure about locking the
 *		  POQ here.  I sure don't know.
 */
    MTX_LOCK_PENDING_OPERATION_QUEUE(client, CM_XCreateCursor);

    src = (PixmapPtr)LookupIDByType(stuff->source, RT_PIXMAP);
    msk = (PixmapPtr)LookupIDByType(stuff->mask, RT_PIXMAP);
    if (src == (PixmapPtr)NULL)
    {
	MTX_UNLOCK_PENDING_OPERATION_QUEUE(client);
	client->errorValue = stuff->source;
	return (BadPixmap);
    }
    if (msk == (PixmapPtr)NULL)
    {
	if (stuff->mask != None)
	{
	    MTX_UNLOCK_PENDING_OPERATION_QUEUE(client);
	    client->errorValue = stuff->mask;
	    return (BadPixmap);
	}
    }
    else if (src->drawable.width != msk->drawable.width
	    || src->drawable.height != msk->drawable.height
	    || src->drawable.depth != 1
	    || msk->drawable.depth != 1)
    {
	MTX_UNLOCK_PENDING_OPERATION_QUEUE(client);
	return (BadMatch);
    }

    width = src->drawable.width;
    height = src->drawable.height;

    if ( stuff->x > width || stuff->y > height )
    {
	MTX_UNLOCK_PENDING_OPERATION_QUEUE(client);
	return (BadMatch);
    }

    n = BitmapBytePad(width)*height;
    srcbits = (unsigned char *)xalloc(n);
    if (!srcbits)
    {
	MTX_UNLOCK_PENDING_OPERATION_QUEUE(client);
	return (BadAlloc);
    }
    mskbits = (unsigned char *)xalloc(n);
    if (!mskbits)
    {
	MTX_UNLOCK_PENDING_OPERATION_QUEUE(client);
	xfree(srcbits);
	return (BadAlloc);
    }

    /* zeroing the (pad) bits helps some ddx cursor handling */
    bzero((char *)srcbits, n);
    (* src->drawable.pScreen->GetImage)( (DrawablePtr)src, 0, 0, width, height,
					 XYPixmap, 1, (pointer)srcbits);
    if ( msk == (PixmapPtr)NULL)
    {
	register unsigned char *bits = mskbits;
	while (--n >= 0)
	    *bits++ = ~0;
    }
    else
    {
	/* zeroing the (pad) bits helps some ddx cursor handling */
	bzero((char *)mskbits, n);
	(* msk->drawable.pScreen->GetImage)( (DrawablePtr)msk, 0, 0, width,
					height, XYPixmap, 1, (pointer)mskbits);
    }
    cm.width = width;
    cm.height = height;
    cm.xhot = stuff->x;
    cm.yhot = stuff->y;
    pCursor = AllocCursor( srcbits, mskbits, &cm,
	    stuff->foreRed, stuff->foreGreen, stuff->foreBlue,
	    stuff->backRed, stuff->backGreen, stuff->backBlue);

    if (pCursor && AddResource(stuff->cid, RT_CURSOR, (pointer)pCursor))
    {
	MTX_UNLOCK_PENDING_OPERATION_QUEUE(client);
	return (client->noClientException);
    }
    MTX_UNLOCK_PENDING_OPERATION_QUEUE(client);
    return BadAlloc;
}

int
ProcCreateGlyphCursor( client)
    register ClientPtr client;
{
    CursorPtr pCursor;
    int res;

    REQUEST(xCreateGlyphCursorReq);

    REQUEST_SIZE_MATCH(xCreateGlyphCursorReq);
    LEGAL_NEW_RESOURCE(stuff->cid, client);

    MTX_LOCK_PENDING_OPERATION_QUEUE(client, CM_XCreateGlyphCursor);

    res = AllocGlyphCursor(stuff->source, stuff->sourceChar,
			   stuff->mask, stuff->maskChar,
			   stuff->foreRed, stuff->foreGreen, stuff->foreBlue,
			   stuff->backRed, stuff->backGreen, stuff->backBlue,
			   &pCursor, client);
    if (res != Success)
    {
	MTX_UNLOCK_PENDING_OPERATION_QUEUE(client);
	return res;
    }
    if (AddResource(stuff->cid, RT_CURSOR, (pointer)pCursor))
    {
	MTX_UNLOCK_PENDING_OPERATION_QUEUE(client);
	return client->noClientException;
    }
    else
    {
	MTX_UNLOCK_PENDING_OPERATION_QUEUE(client);
	return BadAlloc;
    }
}

int
ProcFreeCursor(client)
    register ClientPtr client;
{
    CursorPtr pCursor;
    REQUEST(xResourceReq);

    REQUEST_SIZE_MATCH(xResourceReq);

    MTX_LOCK_AND_VERIFY_CURSOR(pCursor, stuff->id, client, CM_XFreeCursor);

#ifdef XTHREADS	/* ZZZ XTHREADS_? */
    UnlockCursor(pCursor, stuff->id);
#endif /* XTHREADS */

    FreeResource(stuff->id, RT_NONE);
    MTX_UNLOCK_PENDING_OPERATION_QUEUE(client);
    return (client->noClientException);
}

int
ProcQueryBestSize   (client)
    register ClientPtr client;
{
    REPLY_DECL(xQueryBestSizeReply,reply);
    DrawablePtr pDraw;
    ScreenPtr pScreen;
    REQUEST(xQueryBestSizeReq);

    REQUEST_SIZE_MATCH(xQueryBestSizeReq);

    MTX_REP_CHECK_RETURN(reply,BadAlloc);

    MTX_REP_LOCK_AND_VERIFY_GEOMETRABLE (pDraw, stuff->drawable, client, 
				 CM_XQueryBestSize);
    if ((stuff->class != CursorShape) && 
	(stuff->class != TileShape) && 
	(stuff->class != StippleShape))
    {
	client->errorValue = stuff->class;
	MTXReturnPooledMessage;
	MTX_UNLOCK_DRAWABLE(pDraw, stuff->drawable, client);
        return(BadValue);
    }

    if (stuff->class != CursorShape && pDraw->type == UNDRAWABLE_WINDOW)
    {
	MTXReturnPooledMessage;
	MTX_UNLOCK_DRAWABLE(pDraw, stuff->drawable, client);
	return (BadMatch);
    }
    pScreen = pDraw->pScreen;
    (* pScreen->QueryBestSize)(stuff->class, &stuff->width,
			       &stuff->height, pScreen);

    reply->type = X_Reply;
    reply->length = 0;
    reply->sequenceNumber = client->sequence;
    reply->width = stuff->width;
    reply->height = stuff->height;

    WriteReplyToClient(client, sizeof(xQueryBestSizeReply), reply);

    MTX_UNLOCK_DRAWABLE(pDraw, stuff->drawable, client);
    return (client->noClientException);
}

int
ProcSetScreenSaver            (client)
    register ClientPtr client;
{
    int blankingOption, exposureOption;
    REQUEST(xSetScreenSaverReq);

    REQUEST_SIZE_MATCH(xSetScreenSaverReq);

    MTX_LOCK_PENDING_OPERATION_QUEUE(client, CM_XSetScreenSaver);

    blankingOption = stuff->preferBlank;
    if ((blankingOption != DontPreferBlanking) &&
        (blankingOption != PreferBlanking) &&
        (blankingOption != DefaultBlanking))
    {
	client->errorValue = blankingOption;
	MTX_UNLOCK_PENDING_OPERATION_QUEUE(client);
        return BadValue;
    }
    exposureOption = stuff->allowExpose;
    if ((exposureOption != DontAllowExposures) &&
        (exposureOption != AllowExposures) &&
        (exposureOption != DefaultExposures))
    {
	client->errorValue = exposureOption;
	MTX_UNLOCK_PENDING_OPERATION_QUEUE(client);
        return BadValue;
    }
    if (stuff->timeout < -1)
    {
	client->errorValue = stuff->timeout;
	MTX_UNLOCK_PENDING_OPERATION_QUEUE(client);
        return BadValue;
    }
    if (stuff->interval < -1)
    {
	client->errorValue = stuff->interval;
	MTX_UNLOCK_PENDING_OPERATION_QUEUE(client);
        return BadValue;
    }

    if (blankingOption == DefaultBlanking)
	ScreenSaverBlanking = defaultScreenSaverBlanking;
    else
	ScreenSaverBlanking = blankingOption; 
    if (exposureOption == DefaultExposures)
	ScreenSaverAllowExposures = defaultScreenSaverAllowExposures;
    else
	ScreenSaverAllowExposures = exposureOption;

    if (stuff->timeout >= 0)
	ScreenSaverTime = stuff->timeout * MILLI_PER_SECOND;
    else 
	ScreenSaverTime = defaultScreenSaverTime;
    if (stuff->interval >= 0)
	ScreenSaverInterval = stuff->interval * MILLI_PER_SECOND;
    else
	ScreenSaverInterval = defaultScreenSaverInterval;

    MTX_UNLOCK_PENDING_OPERATION_QUEUE(client);
#ifdef XTHREADS
    DITSetScreenSaver();
#endif
    return (client->noClientException);
}

int
ProcGetScreenSaver(client)
    register ClientPtr client;
{
    REQUEST(xReq);
    REPLY_DECL(xGetScreenSaverReply,rep);

    MTX_REP_CHECK_RETURN(rep,BadAlloc);

    REQUEST_SIZE_MATCH(xReq);

    MTX_REP_LOCK_PENDING_OPERATION_QUEUE(client, CM_XGetScreenSaver);

    rep->type = X_Reply;
    rep->length = 0;
    rep->sequenceNumber = client->sequence;
    rep->timeout = ScreenSaverTime / MILLI_PER_SECOND;
    rep->interval = ScreenSaverInterval / MILLI_PER_SECOND;
    rep->preferBlanking = ScreenSaverBlanking;
    rep->allowExposures = ScreenSaverAllowExposures;

    WriteReplyToClient(client, sizeof(xGetScreenSaverReply), rep);
    MTX_UNLOCK_PENDING_OPERATION_QUEUE(client);
    return (client->noClientException);
}

int
ProcChangeHosts(client)
    register ClientPtr client;
{
    REQUEST(xChangeHostsReq);
    int result;

    REQUEST_FIXED_SIZE(xChangeHostsReq, stuff->hostLength);

    MTX_LOCK_PENDING_OPERATION_QUEUE(client, CM_XChangeHosts);
    MTX_MUTEX_LOCK(&ConnectionMutex);

    if(stuff->mode == HostInsert)
	result = AddHost(client, (int)stuff->hostFamily,
			 stuff->hostLength, (pointer)&stuff[1]);
    else if (stuff->mode == HostDelete)
	result = RemoveHost(client, (int)stuff->hostFamily, 
			    stuff->hostLength, (pointer)&stuff[1]);  
    else
    {
	client->errorValue = stuff->mode;
	MTX_MUTEX_UNLOCK(&ConnectionMutex);
	MTX_UNLOCK_PENDING_OPERATION_QUEUE(client);
        return BadValue;
    }
    if (!result)
	result = client->noClientException;

    MTX_MUTEX_UNLOCK(&ConnectionMutex);
    MTX_UNLOCK_PENDING_OPERATION_QUEUE(client);
    return (result);
}

int
ProcListHosts(client)
    register ClientPtr client;
{
extern int GetHosts();
    REPLY_DECL(xListHostsReply,reply);
    int	len, nHosts, result;
    pointer	pdata;

    REQUEST(xListHostsReq);
    REQUEST_SIZE_MATCH(xListHostsReq);

    MTX_REP_CHECK_RETURN(reply,BadAlloc);

    MTX_REP_LOCK_PENDING_OPERATION_QUEUE(client, CM_XListHosts);

    MTX_MUTEX_LOCK(&ConnectionMutex);
    result = GetHosts(&pdata, &nHosts, &len, &(reply->enabled));
    MTX_MUTEX_UNLOCK(&ConnectionMutex);

    if (result != Success)
    {
	MTXReturnPooledMessage;
	MTX_UNLOCK_PENDING_OPERATION_QUEUE(client);
	return(result);
    }

    reply->type = X_Reply;
    reply->sequenceNumber = client->sequence;
    reply->nHosts = nHosts;
    reply->length = len >> 2;

#ifndef XTHREADS
    WriteReplyToClient(client, sizeof(xListHostsReply), reply);
#endif

    if (nHosts)
    {
#ifdef XTHREADS
	msg->pReplyData = (char *) pdata;
	msg->freeReplyData = TRUE;
	msg->lenReplyData = len;
	if (client->swapped)
	    SLHostsExtend(client, len, pdata);
#else
	client->pSwapReplyFunc = SLHostsExtend;
	WriteSwappedDataToClient(client, len, pdata);
#endif
    }
#ifdef XTHREADS
    WriteReplyToClient(client, sizeof(xListHostsReply), reply);
#endif

    MTX_UNLOCK_PENDING_OPERATION_QUEUE(client);
    xfree(pdata);
    return (client->noClientException);
}

int
ProcChangeAccessControl(client)
    register ClientPtr client;
{
    int result;
    REQUEST(xSetAccessControlReq);

    REQUEST_SIZE_MATCH(xSetAccessControlReq);

    MTX_LOCK_PENDING_OPERATION_QUEUE(client, CM_XChangeAccessControl);

    if ((stuff->mode != EnableAccess) && (stuff->mode != DisableAccess))
    {
	client->errorValue = stuff->mode;
	MTX_UNLOCK_PENDING_OPERATION_QUEUE(client);
        return BadValue;
    }
    MTX_MUTEX_LOCK(&ConnectionMutex);
    result = ChangeAccessControl(client, stuff->mode == EnableAccess);
    MTX_MUTEX_UNLOCK(&ConnectionMutex);
    if (!result)
	result = client->noClientException;

    MTX_UNLOCK_PENDING_OPERATION_QUEUE(client);
    return (result);
}

int
ProcKillClient(client)
    register ClientPtr client;
{
    REQUEST(xResourceReq);
    ClientPtr	killclient;

    REQUEST_SIZE_MATCH(xResourceReq);

    MTX_LOCK_PENDING_OPERATION_QUEUE(client, CM_XKillClient);

    if (stuff->id == AllTemporary)
    {
	CloseDownRetainedResources();
	MTX_UNLOCK_PENDING_OPERATION_QUEUE(client);
        return (client->noClientException);
    }

    if ((killclient = LookupClient(stuff->id)))
    {
	CloseDownClient(killclient);
	MTX_UNLOCK_PENDING_OPERATION_QUEUE(client);
	if (client == killclient)
	{
	    /* force yield and return Success, so that Dispatch()
	     * doesn't try to touch client
	     */
	    isItTimeToYield = TRUE;
	    return (Success);
	}
	return (client->noClientException);
    }
    else
    {
	client->errorValue = stuff->id;
	MTX_UNLOCK_PENDING_OPERATION_QUEUE(client);
	return (BadValue);
    }
}

int
ProcSetFontPath(client)
    register ClientPtr client;
{
    unsigned char *ptr;
    unsigned long nbytes, total;
    long nfonts;
    int n, result;
    int error;
    REQUEST(xSetFontPathReq);
    
    REQUEST_AT_LEAST_SIZE(xSetFontPathReq);

    MTX_LOCK_PENDING_OPERATION_QUEUE(client, CM_XSetFontPath);
    
    nbytes = (client->req_len << 2) - sizeof(xSetFontPathReq);
    total = nbytes;
    ptr = (unsigned char *)&stuff[1];
    nfonts = stuff->nFonts;
    while (--nfonts >= 0)
    {
	if ((total == 0) || (total < (n = (*ptr + 1))))
	{
	    MTX_UNLOCK_PENDING_OPERATION_QUEUE(client);
	    return(BadLength);
	}
	total -= n;
	ptr += n;
    }
    if (total >= 4)
    {
	MTX_UNLOCK_PENDING_OPERATION_QUEUE(client);
	return(BadLength);
    }
    result = SetFontPath(client, stuff->nFonts, (unsigned char *)&stuff[1],
			 &error);
    if (!result)
    {
	result = client->noClientException;
	client->errorValue = error;
    }
    MTX_UNLOCK_PENDING_OPERATION_QUEUE(client);
    return (result);
}

int
ProcGetFontPath(client)
    register ClientPtr client;
{
    REPLY_DECL(xGetFontPathReply,reply);
    int stringLens, numpaths;
    unsigned char *bufferStart;
#ifdef XTHREADS
    char *replyData;
    int dataSize;
#endif /* XTHREADS */
    REQUEST (xReq);

    MTX_REP_CHECK_RETURN(reply,BadAlloc);

    REQUEST_SIZE_MATCH(xReq);

    MTX_REP_LOCK_PENDING_OPERATION_QUEUE(client, CM_XGetFontPath);

    bufferStart = GetFontPath(&numpaths, &stringLens);

    reply->type = X_Reply;
    reply->sequenceNumber = client->sequence;
    reply->length = (stringLens + numpaths + 3) >> 2;
    reply->nPaths = numpaths;
#ifdef XTHREADS
    dataSize = stringLens + numpaths;
    if (dataSize > 0)
    {
        replyData = (char *) xalloc(numpaths + stringLens);
        if (!replyData)
        {
            MTXReturnPooledMessage;
            MTX_UNLOCK_PENDING_OPERATION_QUEUE(client);
            return BadAlloc;
        }
        bcopy(bufferStart, replyData, dataSize);
        msg->pReplyData = replyData;
        msg->freeReplyData = TRUE;
        msg->lenReplyData = dataSize;
    }
#endif
    WriteReplyToClient(client, sizeof(xGetFontPathReply), reply);
#ifndef XTHREADS
    if (stringLens || numpaths)
	(void)WriteToClient(client, stringLens + numpaths, (char *)bufferStart);
#endif

    MTX_UNLOCK_PENDING_OPERATION_QUEUE(client);
    return(client->noClientException);
}

int
ProcChangeCloseDownMode(client)
    register ClientPtr client;
{
    REQUEST(xSetCloseDownModeReq);

    REQUEST_SIZE_MATCH(xSetCloseDownModeReq);

    MTX_LOCK_PENDING_OPERATION_QUEUE(client, CM_XChangeCloseDownMode);

    if ((stuff->mode == AllTemporary) ||
	(stuff->mode == RetainPermanent) ||
	(stuff->mode == RetainTemporary))
    {
	client->closeDownMode = stuff->mode;
	MTX_UNLOCK_PENDING_OPERATION_QUEUE(client);
	return (client->noClientException);
    }
    else   
    {
	client->errorValue = stuff->mode;
	MTX_UNLOCK_PENDING_OPERATION_QUEUE(client);
	return (BadValue);
    }
}

int ProcForceScreenSaver(client)
    register ClientPtr client;
{    
    REQUEST(xForceScreenSaverReq);

    REQUEST_SIZE_MATCH(xForceScreenSaverReq);

    MTX_LOCK_PENDING_OPERATION_QUEUE(client, CM_XForceScreenSaver);
    
    if ((stuff->mode != ScreenSaverReset) && 
	(stuff->mode != ScreenSaverActive))
    {
	client->errorValue = stuff->mode;
	MTX_UNLOCK_PENDING_OPERATION_QUEUE(client);
        return BadValue;
    }
    SaveScreens(SCREEN_SAVER_FORCER, (int)stuff->mode);
    MTX_UNLOCK_PENDING_OPERATION_QUEUE(client);
    return client->noClientException;
}

int ProcNoOperation(client)
    register ClientPtr client;
{
    REQUEST(xReq);

    REQUEST_AT_LEAST_SIZE(xReq);
    
    /* noop -- don't do anything */
    return(client->noClientException);
}

void
InitProcVectors()
{
    int i;
    for (i = 0; i<256; i++)
    {
	if(!ProcVector[i])
	{
            ProcVector[i] = SwappedProcVector[i] = ProcBadRequest;
	    ReplySwapVector[i] = NotImplemented;
	}
#ifdef K5AUTH
	if (!k5_Vector[i])
	{
	    k5_Vector[i] = k5_bad;
	}
#endif
    }
    for(i = LASTEvent; i < 128; i++)
    {
	EventSwapVector[i] = NotImplemented;
    }
    
}

/**********************
 * CloseDownClient
 *
 *  Client can either mark his resources destroy or retain.  If retained and
 *  then killed again, the client is really destroyed.
 *********************/

#ifndef XTHREADS
Bool terminateAtReset = FALSE;

void
CloseDownClient(client)
    register ClientPtr client;
{
    Bool really_close_down = client->clientGone ||
			     client->closeDownMode == DestroyAll;

    if (!client->clientGone)
    {
	/* ungrab server if grabbing client dies */
	if (grabState != GrabNone && grabClient == client)
	{
	    UngrabServer();
	}
	BITCLEAR(grabWaiters, client->index);
	DeleteClientFromAnySelections(client);
	ReleaseActiveGrabs(client);
	DeleteClientFontStuff(client);
	if (!really_close_down)
	{
	    /*  This frees resources that should never be retained
	     *  no matter what the close down mode is.  Actually we
	     *  could do this unconditionally, but it's probably
	     *  better not to traverse all the client's resources
	     *  twice (once here, once a few lines down in
	     *  FreeClientResources) in the common case of
	     *  really_close_down == TRUE.
	     */
	    FreeClientNeverRetainResources(client);
	}
	client->clientGone = TRUE;  /* so events aren't sent to client */
	CloseDownConnection(client);
	--nClients;
    }

    if (really_close_down)
    {
	if (ClientIsAsleep(client))
	    ClientSignal (client);
	else
	{
	    FreeClientResources(client);
	    if (client->index < nextFreeClientID)
		nextFreeClientID = client->index;
	    clients[client->index] = NullClient;
	    if (client->requestVector != InitialVector && nClients == 0)
	    {
		if (terminateAtReset)
		    dispatchException |= DE_TERMINATE;
		else
		    dispatchException |= DE_RESET;
	    }
	    xfree(client);

	    while (!clients[currentMaxClients-1])
		currentMaxClients--;
	}
    }
}
#endif /* not XTHREADS */

static void
KillAllClients()
{
    int i;
    for (i=1; i<currentMaxClients; i++)
        if (clients[i])
            CloseDownClient(clients[i]);     
}

/*********************
 * CloseDownRetainedResources
 *
 *    Find all clients that are gone and have terminated in RetainTemporary 
 *    and  destroy their resources.
 *********************/

#ifndef XTHREADS
void
CloseDownRetainedResources()
{
    register int i;
    register ClientPtr client;

    for (i=1; i<currentMaxClients; i++)
    {
        client = clients[i];
        if (client && (client->closeDownMode == RetainTemporary)
	    && (client->clientGone))
	    CloseDownClient(client);
    }
}

void
InitClient(client, i, ospriv)
    ClientPtr client;
    int i;
    pointer ospriv;
{
    client->index = i;
    client->sequence = 0; 
    client->clientAsMask = ((Mask)i) << CLIENTOFFSET;
    client->clientGone = FALSE;
    if (i)
    {
	client->closeDownMode = DestroyAll;
	client->lastDrawable = (DrawablePtr)WindowTable[0];
	client->lastDrawableID = WindowTable[0]->drawable.id;
    }
    else
    {
	client->closeDownMode = RetainPermanent;
	client->lastDrawable = (DrawablePtr)NULL;
	client->lastDrawableID = INVALID;
    }
    client->lastGC = (GCPtr) NULL;
    client->lastGCID = INVALID;
    client->numSaved = 0;
    client->saveSet = (pointer *)NULL;
    client->noClientException = Success;
#ifdef DEBUG
    client->requestLogIndex = 0;
#endif
    client->requestVector = InitialVector;
    client->osPrivate = ospriv;
    client->swapped = FALSE;
    client->big_requests = FALSE;
    client->priority = 0;
#ifdef XKB
    client->xkbClientFlags = 0;
    client->mapNotifyMask = 0;
#endif
}

/************************
 * int NextAvailableClient(ospriv)
 *
 * OS dependent portion can't assign client id's because of CloseDownModes.
 * Returns NULL if there are no free clients.
 *************************/

ClientPtr
NextAvailableClient(ospriv)
    pointer ospriv;
{
    register int i;
    register ClientPtr client;
    xReq data;

    i = nextFreeClientID;
    if (i == MAXCLIENTS)
	return (ClientPtr)NULL;
    clients[i] = client = (ClientPtr)xalloc(sizeof(ClientRec));
    if (!client)
	return (ClientPtr)NULL;
    InitClient(client, i, ospriv);
    if (!InitClientResources(client))
    {
	xfree(client);
	return (ClientPtr)NULL;
    }
    data.reqType = 1;
    data.length = (sz_xReq + sz_xConnClientPrefix) >> 2;
    if (!InsertFakeRequest(client, (char *)&data, sz_xReq))
    {
	FreeClientResources(client);
	xfree(client);
	return (ClientPtr)NULL;
    }
    if (i == currentMaxClients)
	currentMaxClients++;
    while ((nextFreeClientID < MAXCLIENTS) && clients[nextFreeClientID])
	nextFreeClientID++;
    return(client);
}
#endif /* no XTHREADS */

int
ProcInitialConnection(client)
    register ClientPtr client;
{
    REQUEST(xReq);
    register xConnClientPrefix *prefix;
    int whichbyte = 1;

    prefix = (xConnClientPrefix *)((char *)stuff + sz_xReq);
    if ((prefix->byteOrder != 'l') && (prefix->byteOrder != 'B'))
	return (client->noClientException = -1);
    if (((*(char *) &whichbyte) && (prefix->byteOrder == 'B')) ||
	(!(*(char *) &whichbyte) && (prefix->byteOrder == 'l')))
    {
	client->swapped = TRUE;
	SwapConnClientPrefix(prefix);
    }
    stuff->reqType = 2;
    stuff->length += ((prefix->nbytesAuthProto + 3) >> 2) +
		     ((prefix->nbytesAuthString + 3) >> 2);
    if (client->swapped)
    {
	swaps(&stuff->length, whichbyte);
    }
    ResetCurrentRequest(client);
    return (client->noClientException);
}

#ifndef XTHREADS
int
ProcEstablishConnection(client)
    register ClientPtr client;
{
    char *reason, *auth_proto, *auth_string;
    register xConnClientPrefix *prefix;
    REQUEST(xReq);

    prefix = (xConnClientPrefix *)((char *)stuff + sz_xReq);
    auth_proto = (char *)prefix + sz_xConnClientPrefix;
    auth_string = auth_proto + ((prefix->nbytesAuthProto + 3) & ~3);
    if ((prefix->majorVersion != X_PROTOCOL) ||
	(prefix->minorVersion != X_PROTOCOL_REVISION))
	reason = "Protocol version mismatch";
    else
	reason = ClientAuthorized(client,
				  (unsigned short)prefix->nbytesAuthProto,
				  auth_proto,
				  (unsigned short)prefix->nbytesAuthString,
				  auth_string);
    /*
     * if auth protocol does some magic, fall back through to the
     * dispatcher.
     */
    if (client->requestVector == InitialVector)
	return(SendConnSetup(client, reason));
    else
    {
	/*
	 * bump up nClients because otherwise if the client has to get
	 * terminated in the middle of a multi-packet authorization,
	 * bad things might happen, like nClients going negative
	 */
	nClients++;
	return(client->noClientException);
    }
}

int
SendConnSetup(client, reason)
    register ClientPtr client;
    char *reason;
{
    register xWindowRoot *root;
    register int i;

    if (reason)
    {
	xConnSetupPrefix csp;
	char pad[3];

	csp.success = xFalse;
	csp.lengthReason = strlen(reason);
	csp.length = (csp.lengthReason + 3) >> 2;
	csp.majorVersion = X_PROTOCOL;
	csp.minorVersion = X_PROTOCOL_REVISION;
	if (client->swapped)
	    WriteSConnSetupPrefix(client, &csp);
	else
	    (void)WriteToClient(client, sz_xConnSetupPrefix, (char *) &csp);
        (void)WriteToClient(client, (int)csp.lengthReason, reason);
	if (csp.lengthReason & 3)
	    (void)WriteToClient(client, (int)(4 - (csp.lengthReason & 3)),
				pad);
	return (client->noClientException = -1);
    }

    if (client->requestVector == InitialVector)
	nClients++;		/* nClients not already bumped up */
    client->requestVector = client->swapped ? SwappedProcVector : ProcVector;
    client->sequence = 0;
    ((xConnSetup *)ConnectionInfo)->ridBase = client->clientAsMask;
    ((xConnSetup *)ConnectionInfo)->ridMask = RESOURCE_ID_MASK;
    /* fill in the "currentInputMask" */
    root = (xWindowRoot *)(ConnectionInfo + connBlockScreenStart);
    for (i=0; i<screenInfo.numScreens; i++) 
    {
	register int j;
	register xDepth *pDepth;

        root->currentInputMask = WindowTable[i]->eventMask |
			         wOtherEventMasks (WindowTable[i]);
	pDepth = (xDepth *)(root + 1);
	for (j = 0; j < root->nDepths; j++)
	{
	    pDepth = (xDepth *)(((char *)(pDepth + 1)) +
				pDepth->nVisuals * sizeof(xVisualType));
	}
	root = (xWindowRoot *)pDepth;
    }

    if (client->swapped)
    {
	WriteSConnSetupPrefix(client, &connSetupPrefix);
	WriteSConnectionInfo(client,
			     (unsigned long)(connSetupPrefix.length << 2),
			     ConnectionInfo);
    }
    else
    {
	(void)WriteToClient(client, sizeof(xConnSetupPrefix),
			    (char *) &connSetupPrefix);
	(void)WriteToClient(client, (int)(connSetupPrefix.length << 2),
			    ConnectionInfo);
    }
    return (client->noClientException);
}
#endif /* not XTHREADS */

void
SendErrorToClient(client, majorCode, minorCode, resId, errorCode)
    ClientPtr client;
    unsigned int majorCode;
    unsigned int minorCode;
    XID resId;
    int errorCode;
{
/*
 * XXX:SM -Could have been made into macros like "REPLY_DECL", but it looks
 * like this is the only occurrence.
 */
    xError rep, *error = &rep;
#ifdef XTHREADS
    PooledMessagePtr message;
    message = GetPooledMessage();
    error = GetErrorPointer(xError, message);
#endif

    error->type = X_Error;
#ifdef XTHREADS
    if (client->swapped)
    {
        register char n;
        swaps(&error->sequenceNumber, n);
        swapl(&error->resourceID, n);
        swaps(&error->minorCode, n);
    }
#else
    error->sequenceNumber = client->sequence;
#endif
    error->errorCode = errorCode;
    error->majorCode = majorCode;
    error->minorCode = minorCode;
    error->resourceID = resId;

#ifdef XTHREADS
    message->type = error_message;
    BufferMessageForClient(client, message);
#else
    WriteEventsToClient (client, 1, (xEvent *)error);
#endif

}

void
DeleteWindowFromAnySelections(pWin)
    WindowPtr pWin;
{
    register int i;

    for (i = 0; i< NumCurrentSelections; i++)
        if (CurrentSelections[i].pWin == pWin)
        {
            CurrentSelections[i].pWin = (WindowPtr)NULL;
            CurrentSelections[i].window = None;
	    CurrentSelections[i].client = NullClient;
	}
}

#ifndef XTHREADS
static
#endif
void
DeleteClientFromAnySelections(client)
    ClientPtr client;
{
    register int i;

    for (i = 0; i< NumCurrentSelections; i++)
        if (CurrentSelections[i].client == client)
        {
            CurrentSelections[i].pWin = (WindowPtr)NULL;
            CurrentSelections[i].window = None;
	    CurrentSelections[i].client = NullClient;
	}
}

#ifndef XTHREADS
void
MarkClientException(client)
    ClientPtr client;
{
    client->noClientException = -1;
}
#endif /* no XTHREADS */
