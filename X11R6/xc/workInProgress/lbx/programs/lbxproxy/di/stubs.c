/* $XConsortium: stubs.c,v 1.6 94/12/01 20:45:10 mor Exp $ */
/*
 * $NCDOr: stubs.c,v 1.1 1993/11/16 17:58:49 keithp Exp keithp $
 * $NCDId: @(#)stubs.c,v 1.16 1994/11/18 20:35:22 lemke Exp $
 *
 * Copyright 1992 Network Computing Devices
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of NCD. not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  NCD. makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * NCD. DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL NCD.
 * BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN 
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Author:  Keith Packard, Network Computing Devices
 */

#include "X.h"
#include "Xmd.h"
#include "misc.h"
#include "dixstruct.h"
#define  XK_LATIN1
#include "keysymdef.h"


AbortDDX ()
{
}

OsInitColors ()
{
}

extern int  lbxDebug;
extern int  lbxTagCacheSize;
extern Bool lbxUseTags;
extern Bool lbxUseLbx;
extern Bool lbxDoSquishing;
extern Bool lbxCompressImages;
extern Bool lbxDoShortCircuiting;
extern Bool lbxDoLbxGfx;

ddxProcessArgument (argc, argv, i)
    char    **argv;
{
    if (strcmp (argv[i], "-debug") == 0)
    {
	if (++i < argc)
	    lbxDebug = atoi(argv[i]);
	else
	    UseMsg ();
	return 2;
    }
    if (strcmp (argv[i], "-nogfx") == 0)
    {
	lbxDoLbxGfx = 0;
	return 1;
    }
    if (strcmp (argv[i], "-nosc") == 0)
    {
	lbxDoShortCircuiting = 0;
	return 1;
    }
    if (strcmp (argv[i], "-nolzw") == 0)
    {
	LbxNoComp();
	return 1;
    }
    if (strcmp (argv[i], "-nocomp") == 0)
    {
	LbxNoComp();
	return 1;
    }
    if (strcmp (argv[i], "-nodelta") == 0)
    {
	LbxNoDelta();
	return 1;
    }
    if (strcmp (argv[i], "-notags") == 0)
    {
	lbxUseTags = 0;
	return 1;
    }
    if (strcmp (argv[i], "-nolbx") == 0)
    {
	lbxUseLbx = 0;
	return 1;
    }
    if (strcmp (argv[i], "-noimage") == 0)
    {
	lbxCompressImages = 0;
	return 1;
    }
    if (strcmp (argv[i], "-nosquish") == 0)
    {
	LbxNoSquish();
	return 1;
    }
    if (strcmp (argv[i], "-tagcachesize") == 0)
    {
	if (++i < argc)
	    lbxTagCacheSize = atoi(argv[i]);
	else
	    UseMsg ();
	return 2;
    }
    return 0;
}

CheckMemory ()
{
}

void
CopyISOLatin1Lowered(dest, source, length)
    register unsigned char *dest, *source;
    int length;
{
    register int i;

    for (i = 0; i < length; i++, source++, dest++)
    {
	if ((*source >= XK_A) && (*source <= XK_Z))
	    *dest = *source + (XK_a - XK_A);
	else if ((*source >= XK_Agrave) && (*source <= XK_Odiaeresis))
	    *dest = *source + (XK_agrave - XK_Agrave);
	else if ((*source >= XK_Ooblique) && (*source <= XK_Thorn))
	    *dest = *source + (XK_oslash - XK_Ooblique);
	else
	    *dest = *source;
    }
    *dest = '\0';
}

typedef struct _BlockHandler {
    void    (*BlockHandler)();
    void    (*WakeupHandler)();
    pointer blockData;
    Bool    deleted;
} BlockHandlerRec, *BlockHandlerPtr;

static BlockHandlerPtr	handlers;
static int		numHandlers;
static int		sizeHandlers;
static Bool		inHandler;
static Bool		handlerDeleted;

/* called from the OS layer */
void
BlockHandler(pTimeout, pReadmask)
pointer	pTimeout;	/* DIX doesn't want to know how OS represents time */
pointer pReadmask;	/* nor how it represents the set of descriptors */
{
    register int i, j;
    
    ++inHandler;
    for (i = 0; i < numHandlers; i++)
	(*handlers[i].BlockHandler) (handlers[i].blockData,
				     pTimeout, pReadmask);
    if (handlerDeleted)
    {
	for (i = 0; i < numHandlers;)
	    if (handlers[i].deleted)
	    {
	    	for (j = i; j < numHandlers - 1; j++)
		    handlers[j] = handlers[j+1];
	    	numHandlers--;
	    }
	    else
		i++;
	handlerDeleted = FALSE;
    }
    --inHandler;
}

void
WakeupHandler(result, pReadmask)
int	result;	/* 32 bits of undefined result from the wait */
pointer pReadmask;	/* the resulting descriptor mask */
{
    register int i, j;

    ++inHandler;
    for (i = numHandlers - 1; i >= 0; i--)
	(*handlers[i].WakeupHandler) (handlers[i].blockData,
				      result, pReadmask);
    if (handlerDeleted)
    {
	for (i = 0; i < numHandlers;)
	    if (handlers[i].deleted)
	    {
	    	for (j = i; j < numHandlers - 1; j++)
		    handlers[j] = handlers[j+1];
	    	numHandlers--;
	    }
	    else
		i++;
	handlerDeleted = FALSE;
    }
    --inHandler;
}

/* Reentrant with BlockHandler and WakeupHandler, except wakeup won't
 * get called until next time
 */

Bool
RegisterBlockAndWakeupHandlers (blockHandler, wakeupHandler, blockData)
    void    (*blockHandler)();
    void    (*wakeupHandler)();
    pointer blockData;
{
    BlockHandlerPtr new;

    if (numHandlers >= sizeHandlers)
    {
    	new = (BlockHandlerPtr) xrealloc (handlers, (numHandlers + 1) *
				      	  sizeof (BlockHandlerRec));
    	if (!new)
	    return FALSE;
    	handlers = new;
	sizeHandlers = numHandlers + 1;
    }
    handlers[numHandlers].BlockHandler = blockHandler;
    handlers[numHandlers].WakeupHandler = wakeupHandler;
    handlers[numHandlers].blockData = blockData;
    handlers[numHandlers].deleted = FALSE;
    numHandlers = numHandlers + 1;
    return TRUE;
}

void
RemoveBlockAndWakeupHandlers (blockHandler, wakeupHandler, blockData)
    void    (*blockHandler)();
    void    (*wakeupHandler)();
    pointer blockData;
{
    int	    i;

    for (i = 0; i < numHandlers; i++)
	if (handlers[i].BlockHandler == blockHandler &&
	    handlers[i].WakeupHandler == wakeupHandler &&
	    handlers[i].blockData == blockData)
	{
	    if (inHandler)
	    {
		handlerDeleted = TRUE;
		handlers[i].deleted = TRUE;
	    }
	    else
	    {
	    	for (; i < numHandlers - 1; i++)
		    handlers[i] = handlers[i+1];
	    	numHandlers--;
	    }
	    break;
	}
}

void
InitBlockAndWakeupHandlers ()
{
    xfree (handlers);
    handlers = (BlockHandlerPtr) 0;
    numHandlers = 0;
    sizeHandlers = 0;
}

/*
 * A general work queue.  Perform some task before the server
 * sleeps for input.
 */

WorkQueuePtr		workQueue;
static WorkQueuePtr	*workQueueLast = &workQueue;

/* ARGSUSED */
void
ProcessWorkQueue()
{
    WorkQueuePtr    q, n, p;

    p = NULL;
    /*
     * Scan the work queue once, calling each function.  Those
     * which return TRUE are removed from the queue, otherwise
     * they will be called again.  This must be reentrant with
     * QueueWorkProc, hence the crufty usage of variables.
     */
    for (q = workQueue; q; q = n)
    {
	if ((*q->function) (q->client, q->closure))
	{
	    /* remove q from the list */
	    n = q->next;    /* don't fetch until after func called */
	    if (p)
		p->next = n;
	    else
		workQueue = n;
	    xfree (q);
	}
	else
	{
	    n = q->next;    /* don't fetch until after func called */
	    p = q;
	}
    }
    if (p)
	workQueueLast = &p->next;
    else
    {
	workQueueLast = &workQueue;
    }
}

Bool
QueueWorkProc (function, client, closure)
    Bool	(*function)();
    ClientPtr	client;
    pointer	closure;
{
    WorkQueuePtr    q;

    q = (WorkQueuePtr) xalloc (sizeof *q);
    if (!q)
	return FALSE;
    q->function = function;
    q->client = client;
    q->closure = closure;
    q->next = NULL;
    *workQueueLast = q;
    workQueueLast = &q->next;
    return TRUE;
}

/*
 * Manage a queue of sleeping clients, awakening them
 * when requested, by using the OS functions IgnoreClient
 * and AttendClient.  Note that this *ignores* the troubles
 * with request data interleaving itself with events, but
 * we'll leave that until a later time.
 */

typedef struct _SleepQueue {
    struct _SleepQueue	*next;
    ClientPtr		client;
    Bool		(*function)();
    pointer		closure;
} SleepQueueRec, *SleepQueuePtr;

static SleepQueuePtr	sleepQueue = NULL;

Bool
ClientSleep (client, function, closure)
    ClientPtr	client;
    Bool	(*function)();
    pointer	closure;
{
    SleepQueuePtr   q;

    q = (SleepQueuePtr) xalloc (sizeof *q);
    if (!q)
	return FALSE;

    IgnoreClient (client);
    q->next = sleepQueue;
    q->client = client;
    q->function = function;
    q->closure = closure;
    sleepQueue = q;
    return TRUE;
}

Bool
ClientSignal (client)
    ClientPtr	client;
{
    SleepQueuePtr   q;

    for (q = sleepQueue; q; q = q->next)
	if (q->client == client)
	{
	    return QueueWorkProc (q->function, q->client, q->closure);
	}
    return FALSE;
}

void
ClientWakeup (client)
    ClientPtr	client;
{
    SleepQueuePtr   q, *prev;

    prev = &sleepQueue;
    while (q = *prev)
    {
	if (q->client == client)
	{
	    *prev = q->next;
	    xfree (q);
	    if (!client->clientGone)
		AttendClient (client);
	    break;
	}
	prev = &q->next;
    }
}

Bool
ClientIsAsleep (client)
    ClientPtr	client;
{
    SleepQueuePtr   q;

    for (q = sleepQueue; q; q = q->next)
	if (q->client == client)
	    return TRUE;
    return FALSE;
}

#ifdef NCDHOST
/*
 * something screwy in the config is converting things to use this,
 * which we don't have
 */
void
memmove(b1, b2, len)
    char	*b1, *b2;
    int	len;
{
    bcopy(b2, b1, len);
}
#endif

/* these are used in Xserver/os/libos.a:connection.o but defined in
 * Xserver/dix, so we need stubs
 */

CallbackListPtr ServerGrabCallback;

/* ARGSUSED */
void 
CallCallbacks(pcbl, call_data)
    CallbackListPtr    *pcbl;
    pointer         call_data;
{
}

