/************************************************************

Copyright (c) 1987  X Consortium

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


Copyright 1987 by Digital Equipment Corporation, Maynard, Massachusetts.

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

/* $XConsortium: resource.c,v 1.8 94/04/17 21:15:52 rob Exp $ */

/*	Routines to manage various kinds of resources:
 *
 *	CreateNewResourceType, CreateNewResourceClass, InitClientResources,
 *	FakeClientID, AddResource, FreeResource, FreeClientResources,
 *	FreeAllResources, LookupIDByType, LookupIDByClass, GetXIDRange
 */

/* 
 *      A resource ID is a 32 bit quantity, the upper 3 bits of which are
 *	off-limits for client-visible resources.  The next 7 bits are
 *      used as client ID, and the low 22 bits come from the client.
 *	A resource ID is "hashed" by extracting and xoring subfields
 *      (varying with the size of the hash table).
 *
 *      It is sometimes necessary for the server to create an ID that looks
 *      like it belongs to a client.  This ID, however,  must not be one
 *      the client actually can create, or we have the potential for conflict.
 *      The 30th bit of the ID is reserved for the server's use for this
 *      purpose.  By setting CLIENT_ID(id) to the client, the SERVER_BIT to
 *      1, and an otherwise arbitrary ID in the low 22 bits, we can create a
 *      resource "owned" by the client.
 */

#include "X.h"
#include "misc.h"
#include "os.h"
#include "resource.h"
#include "dixstruct.h" 
#include "opaque.h"
#include "windowstr.h"

#ifdef XTHREADS
#include "pixmapstr.h"
#include "gcstruct.h"
#include "colormapst.h"
#include "cursorstr.h"
#include "fontstruct.h"
#include "xthreads.h"
#include "mtxlock.h"
#endif /* XTHREADS */

extern void HandleSaveSet();

#ifndef XTHREADS
extern void FlushClientCaches();
static void RebuildTable();
extern WindowPtr *WindowTable;
#endif /* not XTHREADS */

#ifdef XTHREADS	/* since lock field referenced need additional wrapper */
#define MTX_CLIENT_MUTEX_LOCK(cid)			\
    MTX_MUTEX_LOCK(&clientTable[cid].lock.mutex)
#define MTX_CLIENT_MUTEX_UNLOCK(cid)			\
    MTX_MUTEX_UNLOCK(&clientTable[cid].lock.mutex)
#else /* XTHREADS */
#define MTX_CLIENT_MUTEX_LOCK(cid)	/* nothing */
#define MTX_CLIENT_MUTEX_UNLOCK(cid)	/* nothing */
#endif /* XTHREADS */

#define SERVER_MINID 32

#define INITBUCKETS 64
#define INITHASHSIZE 6
#define MAXHASHSIZE 11

#ifdef XTHREADS
#define RTA_INITIAL_SIZE   32
#define RTA_INCREMENT_SIZE 16

#define LOCKBITS_EXCLUSIVE_MASK (0x00000001)
#define LOCKBITS_WRITE_MASK     (0x80000000)
#define LOCKBITS_READ_MASK      (0x00000fff)
#define LOCKBITS_INTEREST_MASK  (0x00fff000)

#define RESOURCE_LOCKED_EXCLUSIVE(bits)   ((bits) & LOCKBITS_EXCLUSIVE_MASK)
#define RESOURCE_LOCKED_WRITING(bits)     ((bits) & LOCKBITS_WRITE_MASK)
#define RESOURCE_LOCKED_READING(bits)     ((bits) & LOCKBITS_READ_MASK)
#define LOCKBITS_SET_EXCLUSIVE(bits)      ((bits) |= LOCKBITS_EXCLUSIVE_MASK)
#define LOCKBITS_SET_WRITE(bits)          ((bits) |= LOCKBITS_WRITE_MASK)
#define LOCKBITS_SET_READ(bits)           ((bits)++)
#define LOCKBITS_CLEAR_EXCLUSIVE(bits)    ((bits) &= ~LOCKBITS_EXCLUSIVE_MASK)
#define LOCKBITS_CLEAR_WRITE(bits)        ((bits) &= ~LOCKBITS_WRITE_MASK)
#define LOCKBITS_CLEAR_READ(bits)         ((bits)--)

#define RESOURCE_HAS_NO_READERS(bits)     (((bits) & LOCKBITS_READ_MASK) == 0)
#define RESOURCE_HAS_WAITER(bits)         ((bits) & LOCKBITS_INTEREST_MASK)
#define RESOURCE_INCREMENT_INTEREST(bits) ((bits) += 0x00001000)
#define RESOURCE_DECREMENT_INTEREST(bits) ((bits) -= 0x00001000)

#define PEND_FOR_RESOURCE_AVAILABILITY(lockbits, pLock)                       \
	{                                                                     \
	    RESOURCE_INCREMENT_INTEREST((lockbits));                          \
	    pLock->waiting = TRUE;                                            \
	    X_COND_WAIT(&pLock->condition, &pLock->mutex);                    \
	    RESOURCE_DECREMENT_INTEREST((lockbits));                          \
        }

#define LOCK_RESOURCE_EXCLUSIVE(lockbits, pLock)                              \
        {                                                                     \
	    while (RESOURCE_LOCKED_EXCLUSIVE((lockbits)))                     \
	    {                                                                 \
                PEND_FOR_RESOURCE_AVAILABILITY((lockbits), (pLock));          \
            }                                                                 \
	    LOCKBITS_SET_EXCLUSIVE((lockbits));                               \
        }

#define LOCK_RESOURCE_READ(lockbits, pLock)                                   \
        {                                                                     \
	    while (RESOURCE_LOCKED_WRITING((lockbits)))                       \
	    {                                                                 \
                PEND_FOR_RESOURCE_AVAILABILITY((lockbits), (pLock));          \
	    }                                                                 \
	    LOCKBITS_SET_READ((lockbits));                                    \
        }

#define LOCK_RESOURCE_WRITE(lockbits, pLock)                                  \
        {                                                                     \
	    while (RESOURCE_LOCKED_WRITING((lockbits)))                       \
	    {                                                                 \
                PEND_FOR_RESOURCE_AVAILABILITY((lockbits), (pLock));          \
	    }                                                                 \
                                                                              \
            LOCKBITS_SET_WRITE((lockbits));                                   \
	    while (RESOURCE_LOCKED_READING((lockbits)))                       \
	    {                                                                 \
                PEND_FOR_RESOURCE_AVAILABILITY((lockbits), (pLock));          \
	    }                                                                 \
        }

#define LOCK_RESOURCE(pRes, lockOffset, lockType, pLock)                      \
	{                                                                     \
	    ResourceLockbitsPtr pLockbits;                                    \
            pLockbits = (ResourceLockbitsPtr)                                 \
                        (((char *) pRes) + lockOffset);                       \
                                                                              \
            switch (lockType)                                                 \
            {                                                                 \
            case no_lock:                                                     \
    	        break;                                                        \
                                                                              \
            case exclusive_lock:                                              \
	        LOCK_RESOURCE_EXCLUSIVE(*pLockbits, pLock);                   \
                break;                                                        \
                                                                              \
            case read_lock:                                                   \
	        LOCK_RESOURCE_READ(*pLockbits, pLock);                        \
                break;                                                        \
                                                                              \
            case write_lock:                                                  \
	        LOCK_RESOURCE_WRITE(*pLockbits, pLock);                       \
                break;                                                        \
            }                                                                 \
	}

#define CHECK_NEXT_LOOKUP_LOCK(firstID, secondID, pLock)                      \
        {                                                                     \
	    if (firstID != secondID)                                          \
	    {                                                                 \
		X_MUTEX_UNLOCK(&pLock->mutex);                                \
		pLock = &clientTable[secondID].lock;                          \
		X_MUTEX_LOCK(&pLock->mutex);                                  \
            }                                                                 \
	}

#define CHECK_NEXT_LOCK_WITH_BROADCAST(cid1, cid2, pLock, broadcast)          \
	{                                                                     \
            if (cid1 != cid2)                                                 \
	    {                                                                 \
	        if (broadcast)                                                \
	        {                                                             \
	            pLock->waiting = FALSE;                                   \
	            X_COND_BROADCAST(&pLock->condition);                      \
	            broadcast = FALSE;                                        \
                }                                                             \
            X_MUTEX_UNLOCK(&pLock->mutex);                                    \
            pLock = &clientTable[cid2].lock;                                  \
            X_MUTEX_LOCK(&pLock->mutex);                                      \
            }                                                                 \
        }

#ifdef LOOKUP_DRAWABLE
#undef LOOKUP_DRAWABLE
#endif
#define LOOKUP_DRAWABLE(drawID, pDraw, pCache)                                \
	{                                                                     \
	    if (pCache->lastID == drawID)                                     \
	    {                                                                 \
		pDraw = (DrawablePtr) pCache->lastValue;                      \
            }                                                                 \
	    else                                                              \
	    {                                                                 \
              pDraw=(DrawablePtr)LookupResourceByClass(drawID, RC_DRAWABLE);  \
              if (pDraw)                                                      \
	      {                                                               \
	          pCache->lastID = drawID;                                    \
	          pCache->lastValue = (pointer) pDraw;                        \
              }                                                               \
	    }                                                                 \
	}

#define LOOKUP_RESOURCE_TYPE(resID, pRes, pCache, resource_t, rt)             \
        {                                                                     \
	    if (pCache && pCache->lastID == resID)                            \
	    {                                                                 \
		pRes = (resource_t *) pCache->lastValue;                      \
	    }                                                                 \
	    else                                                              \
	    {                                                                 \
                int cid = CLIENT_ID(resID);                                   \
		pRes = NULL;                                                  \
                if (cid < MAXCLIENTS && clientTable[cid].buckets)             \
                {                                                             \
	            ResourcePtr res;                                          \
	            res = clientTable[cid].resources[Hash(cid, resID)];       \
                    for (; res; res = res->next)                              \
                    {                                                         \
                        if ((res->id == resID)  && (res->type == rt))         \
                        {                                                     \
                            pRes = (resource_t *) res->value;                 \
			    if (pCache)                                       \
			    {                                                 \
		                pCache->lastID = resID;                       \
		                pCache->lastValue = (pointer) pRes;           \
			    }                                                 \
                        }                                                     \
		    }                                                         \
                }                                                             \
            }                                                                 \
        }

#define BROADCAST_WAKEUP(pLock)                                               \
	{                                                                     \
	    if (pLock->waiting)                                               \
	    {                                                                 \
                pLock->waiting = FALSE;                                       \
                X_COND_BROADCAST(&pLock->condition);                          \
	    }                                                                 \
	}

#define NullResource  ((ResourcePtr)NULL)

#endif /* XTHREADS */

typedef struct _Resource {
    struct _Resource	*next;
    XID			id;
    RESTYPE		type;
    pointer		value;
} ResourceRec, *ResourcePtr;

#ifndef XTHREADS
#define NullResource ((ResourcePtr)NULL)
#else /* XTHREADS */

typedef struct _CacheElementRec {
    XID lastID;
    pointer lastValue;
} CacheElementRec, *CacheElementPtr;

typedef struct _ClientCache {
    int nCached;
    CacheElementPtr rt_cache;
    CacheElementRec drawable[2];
} ClientCacheRec, *ClientCachePtr;

typedef struct _ResourceLockRec {
    X_MUTEX_TYPE mutex;
    X_COND_TYPE condition;
    Bool waiting;
} ResourceLockRec, *ResourceLockPtr;

typedef struct _ResourceArrayRec
{
    XID resID;
    RESTYPE resKind;
    ResourceLockType lockType;
    RESTYPE resType;
    pointer pResource;
    ResourceLockPtr pLock;
} ResourceArrayRec, *ResourceArrayPtr;

typedef struct _ResourceTypeAttrRec
{
    int lockOffset;
    ResourceLockType lockType;
    ResourceLockPtr shared;
} ResourceTypeAttrRec, *ResourceTypeAttrPtr;
typedef long ResourceLockbits, *ResourceLockbitsPtr;
#endif /* XTHREADS */

typedef struct _ClientResource {
    ResourcePtr *resources;
    int		elements;
    int		buckets;
    int		hashsize;	/* log(2)(buckets) */
    XID		fakeID;
    XID		endFakeID;
    XID		expectID;
#ifdef XTHREADS
    ResourceLockRec lock;
    ClientCacheRec  cache;
#endif /* XTHREADS */
} ClientResourceRec, *ClientResourcePtr ;

#ifdef XTHREADS
/* XXX -- put these in resource.h */
extern void UnlockResourceByType();

static pointer LookupResourceByClass();
static ResourcePtr LookupResourceRecByClass();
static Bool UnlockResourceBits();
static int ResourceBadValue();
#endif /* XTHREADS */

static RESTYPE lastResourceType;
static RESTYPE lastResourceClass;
static RESTYPE TypeMask;

static DeleteType *DeleteFuncs = (DeleteType *)NULL;

#ifndef XTHREADS
RESTYPE
CreateNewResourceType(deleteFunc)
    DeleteType deleteFunc;
{
    RESTYPE next = lastResourceType + 1;
    DeleteType *funcs;

    if (next & lastResourceClass)
	return 0;
    funcs = (DeleteType *)xrealloc(DeleteFuncs,
				   (next + 1) * sizeof(DeleteType));
    if (!funcs)
	return 0;
    lastResourceType = next;
    DeleteFuncs = funcs;
    DeleteFuncs[next] = deleteFunc;
    return next;
}

RESTYPE
CreateNewResourceClass()
{
    RESTYPE next = lastResourceClass >> 1;

    if (next & lastResourceType)
	return 0;
    lastResourceClass = next;
    TypeMask = next - 1;
    return next;
}

ClientResourceRec clientTable[MAXCLIENTS];
#else /* XTHREADS */
static ResourceTypeAttrPtr ResourceTypeAttributes = NULL;
static int numResourceTypeEntries = 0;
static int sizeResourceTypeTable = 0;
static int multiThreadMode = TRUE;
static ClientResourceRec clientTable[MAXCLIENTS];
#endif /* XTHREADS */

#ifndef XTHREADS
/*****************
 * InitClientResources
 *    When a new client is created, call this to allocate space
 *    in resource table
 *****************/

Bool
InitClientResources(client)
    ClientPtr client;
{
    register int i, j;
 
    if (client == serverClient)
    {
	extern int DeleteWindow(), dixDestroyPixmap(), FreeGC();
	extern int CloseFont(), FreeCursor();
	extern int FreeColormap(), FreeClientPixels();
	extern int OtherClientGone(), DeletePassiveGrab();

	lastResourceType = RT_LASTPREDEF;
	lastResourceClass = RC_LASTPREDEF;
	TypeMask = RC_LASTPREDEF - 1;
	if (DeleteFuncs)
	    xfree(DeleteFuncs);
	DeleteFuncs = (DeleteType *)xalloc((lastResourceType + 1) *
					   sizeof(DeleteType));
	if (!DeleteFuncs)
	    return FALSE;
	DeleteFuncs[RT_NONE & TypeMask] = (DeleteType)NoopDDA;
	DeleteFuncs[RT_WINDOW & TypeMask] = DeleteWindow;
	DeleteFuncs[RT_PIXMAP & TypeMask] = dixDestroyPixmap;
	DeleteFuncs[RT_GC & TypeMask] = FreeGC;
	DeleteFuncs[RT_FONT & TypeMask] = CloseFont;
	DeleteFuncs[RT_CURSOR & TypeMask] = FreeCursor;
	DeleteFuncs[RT_COLORMAP & TypeMask] = FreeColormap;
	DeleteFuncs[RT_CMAPENTRY & TypeMask] = FreeClientPixels;
	DeleteFuncs[RT_OTHERCLIENT & TypeMask] = OtherClientGone;
	DeleteFuncs[RT_PASSIVEGRAB & TypeMask] = DeletePassiveGrab;
    }
    clientTable[i = client->index].resources =
	(ResourcePtr *)xalloc(INITBUCKETS*sizeof(ResourcePtr));
    if (!clientTable[i].resources)
	return FALSE;
    clientTable[i].buckets = INITBUCKETS;
    clientTable[i].elements = 0;
    clientTable[i].hashsize = INITHASHSIZE;
    /* Many IDs allocated from the server client are visible to clients,
     * so we don't use the SERVER_BIT for them, but we have to start
     * past the magic value constants used in the protocol.  For normal
     * clients, we can start from zero, with SERVER_BIT set.
     */
    clientTable[i].fakeID = client->clientAsMask |
			    (client->index ? SERVER_BIT : SERVER_MINID);
    clientTable[i].endFakeID = (clientTable[i].fakeID | RESOURCE_ID_MASK) + 1;
    clientTable[i].expectID = client->clientAsMask;
    for (j=0; j<INITBUCKETS; j++) 
    {
        clientTable[i].resources[j] = NullResource;
    }
    return TRUE;
}
#else /* XTHREADS */
/******************************** MTX **********************************/

Bool
InitializeRDBMonitor(client)
ClientPtr client;
{
    extern int DeleteWindow(), dixDestroyPixmap(), FreeGC();
    extern int CloseFont(), FreeCursor();
    extern int FreeColormap(), FreeClientPixels();
    extern int OtherClientGone(), DeletePassiveGrab();

    static int beenHere = FALSE;
    int result;
    int allocSize;
    int lockOffset;
    WindowRec window;
    PixmapRec pixmap;
    GC gc;

    lastResourceType = RT_LASTPREDEF;
    lastResourceClass = RC_LASTPREDEF;
    TypeMask = RC_LASTPREDEF - 1;
    if (!beenHere)
    {
        int i, j;
        int minRTCacheEntries;
        beenHere = TRUE;
        minRTCacheEntries = max((RT_WINDOW & TypeMask), (RT_GC & TypeMask));
        minRTCacheEntries = max(minRTCacheEntries, (RT_PIXMAP & TypeMask));
        minRTCacheEntries = max(minRTCacheEntries, (RT_COLORMAP & TypeMask));
        minRTCacheEntries = max(minRTCacheEntries, (RT_FONT & TypeMask));
        minRTCacheEntries = max(minRTCacheEntries, (RT_CURSOR & TypeMask)) + 1;
        for (i = 0; i < MAXCLIENTS; i++)
        {
            ResourceLockPtr lock;
            ClientCachePtr cache;
            CacheElementPtr rtcache;

            /* initialie client lock */
            lock = &clientTable[i].lock;
            X_MUTEX_INIT(&lock->mutex, X_MUTEX_ATTR_DEFAULT);
            X_COND_INIT(&lock->condition, X_COND_ATTR_DEFAULT);
            X_SET_SYMNAME(&lock->mutex, "Client_M");
            X_SET_SYMNAME(&lock->condition, "Client_C");
            lock->waiting = FALSE;

            /* initialize client resource cache */
            allocSize = minRTCacheEntries * sizeof(ClientCacheRec);
            rtcache = (CacheElementPtr) xalloc(allocSize);
            if (!rtcache)
            {
                return FALSE;
            }

            for (j = 0; j < minRTCacheEntries; j++)
            {
                rtcache[j].lastID = INVALID;
                rtcache[j].lastValue = NULL;
            }

            cache = &clientTable[i].cache;
            cache->drawable[0].lastID = INVALID;
            cache->drawable[0].lastValue = NULL;
            cache->drawable[1].lastID = INVALID;
            cache->drawable[1].lastValue = NULL;
            cache->nCached = minRTCacheEntries;
            cache->rt_cache = rtcache;
        }
    }

    if (ResourceTypeAttributes)
    {
        int i;
        for (i = 0; i <= RT_LASTPREDEF; i++)
        {
            ResourceTypeAttrPtr pAttr;
            pAttr = &ResourceTypeAttributes[i];
            if (pAttr->shared)
            {
                X_MUTEX_DESTROY(&pAttr->shared->mutex);
                X_COND_DESTROY(&pAttr->shared->condition);
                xfree(pAttr->shared);
            }
        }
        xfree(ResourceTypeAttributes);
    }

    sizeResourceTypeTable = RTA_INITIAL_SIZE;
    numResourceTypeEntries = RT_LASTPREDEF + 1;
    allocSize = sizeResourceTypeTable * sizeof(ResourceTypeAttrRec);
    ResourceTypeAttributes = (ResourceTypeAttrPtr)xalloc(allocSize);
    if (!ResourceTypeAttributes)
    {
        return FALSE;
    }

    lockOffset = ((char *) &window.drawable.lockBits) - ((char *) &window);
    ResourceTypeAttributes[RT_WINDOW & TypeMask].shared = NULL;
    ResourceTypeAttributes[RT_WINDOW & TypeMask].lockType = reader_writer_lock;
    ResourceTypeAttributes[RT_WINDOW & TypeMask].lockOffset = lockOffset;

    lockOffset = ((char *) &pixmap.drawable.lockBits) - ((char *) &pixmap);
    ResourceTypeAttributes[RT_PIXMAP & TypeMask].shared = NULL;
    ResourceTypeAttributes[RT_PIXMAP & TypeMask].lockType = exclusive_lock;
    ResourceTypeAttributes[RT_PIXMAP & TypeMask].lockOffset = lockOffset;

    lockOffset = ((char *) &gc.lockBits) - ((char *) &gc);
    ResourceTypeAttributes[RT_GC & TypeMask].lockType = exclusive_lock;
    ResourceTypeAttributes[RT_GC & TypeMask].shared = NULL;
    ResourceTypeAttributes[RT_GC & TypeMask].lockOffset = lockOffset;

    if (DeleteFuncs)
    {
        xfree(DeleteFuncs);
    }

    allocSize = (lastResourceType + 1) * sizeof(DeleteType);
    DeleteFuncs = (DeleteType *)xalloc(allocSize);
    if (!DeleteFuncs)
    {
        return FALSE;
    }

    DeleteFuncs[RT_NONE & TypeMask] = (int (*)()) NoopDDA;
    DeleteFuncs[RT_WINDOW & TypeMask] = DeleteWindow;
    DeleteFuncs[RT_PIXMAP & TypeMask] = dixDestroyPixmap;
    DeleteFuncs[RT_GC & TypeMask] = FreeGC;
    DeleteFuncs[RT_FONT & TypeMask] = CloseFont;
    DeleteFuncs[RT_CURSOR & TypeMask] = FreeCursor;
    DeleteFuncs[RT_COLORMAP & TypeMask] = FreeColormap;
    DeleteFuncs[RT_CMAPENTRY & TypeMask] = FreeClientPixels;
    DeleteFuncs[RT_OTHERCLIENT & TypeMask] = OtherClientGone;
    DeleteFuncs[RT_PASSIVEGRAB & TypeMask] = DeletePassiveGrab;

    result = CreateClientResources(client);
    return result;
}

/******************************** MTX **********************************/

Bool
CreateClientResources(client)
    ClientPtr client;
{
    ResourcePtr *resources;
    register int i, cid;
    int nrts, nrcs;
    int allocSize;

    resources = (ResourcePtr *) xalloc(INITBUCKETS*sizeof(ResourcePtr));
    if (!resources)
    {
	return FALSE;
    }

    cid = client->index;
    X_MUTEX_LOCK(&clientTable[cid].lock.mutex);
    clientTable[cid].resources = resources;
    clientTable[cid].buckets = INITBUCKETS;
    clientTable[cid].elements = 0;
    clientTable[cid].hashsize = INITHASHSIZE;

    /* 
     * Many IDs allocated from the server client are visible to clients,
     * so we don't use the SERVER_BIT for them, but we have to start
     * past the magic value constants used in the protocol.  For normal
     * clients, we can start from zero, with SERVER_BIT set.
     */
    clientTable[cid].fakeID = client->clientAsMask |
			     (client->index ? SERVER_BIT : SERVER_MINID);
    clientTable[cid].endFakeID = (clientTable[cid].fakeID | 
				  RESOURCE_ID_MASK) + 1;
    clientTable[cid].expectID = client->clientAsMask;
    for (i=0; i<INITBUCKETS; i++) 
    {
        clientTable[cid].resources[i] = NullResource;
    }

    X_MUTEX_UNLOCK(&clientTable[cid].lock.mutex);
    return TRUE;
}

/******************************** MTX **********************************/

static
void
GrabRDBLocks()
{
    int i;
    if (!multiThreadMode)
        return;

    for (i = 0; i < MAXCLIENTS; i++)
    {
        X_MUTEX_LOCK(&clientTable[i].lock.mutex);
    }
}

/******************************** MTX **********************************/

static
void
UngrabRDBLocks()
{
    int i;
    if (!multiThreadMode)
        return;

    for (i = 0; i < MAXCLIENTS; i++)
    {
        X_MUTEX_UNLOCK(&clientTable[i].lock.mutex);
    }
}

/******************************** MTX **********************************/

RESTYPE
CreateNewResourceType(deleteFunc, lockType, lockOffset, shared)
    DeleteType deleteFunc;
    ResourceLockType lockType;
    int lockOffset;
    Bool shared;
{
    int allocSize;
    RESTYPE next;
    DeleteType *funcs;
    ResourceLockPtr sharedPtr;
    ResourceTypeAttrPtr table;

    GrabRDBLocks();
    next = lastResourceType + 1;
    if (next & lastResourceClass)
    {
        UngrabRDBLocks();
	return (0);
    }

    allocSize = (next + 1) * sizeof(DeleteType);
    funcs = (DeleteType *)xrealloc(DeleteFuncs, allocSize);
    if (!funcs)
    {
        UngrabRDBLocks();
	return (0);
    }

    if (next > sizeResourceTypeTable - 1)
    {
        int newSize;
        allocSize = newSize * sizeof(ResourceTypeAttrRec);
        newSize = sizeResourceTypeTable + RTA_INCREMENT_SIZE;
        table = (ResourceTypeAttrPtr)xrealloc(ResourceTypeAttributes, newSize);
        if (!table)
        {
            UngrabRDBLocks();
	    return (0);
        }
        ResourceTypeAttributes = table;
        sizeResourceTypeTable = newSize;
    }

    sharedPtr = NULL;
    if (shared)
    {
        sharedPtr = (ResourceLockPtr) xalloc(sizeof(ResourceLockRec));
        if (!sharedPtr)
        {
            UngrabRDBLocks();
            return (0);
        }
        X_MUTEX_INIT(&sharedPtr->mutex, X_MUTEX_ATTR_DEFAULT);
        X_COND_INIT(&sharedPtr->condition, X_COND_ATTR_DEFAULT);
        X_SET_SYMNAME(&sharedPtr->mutex, "ShRec__M");
        X_SET_SYMNAME(&sharedPtr->condition, "ShRec__C");
        sharedPtr->waiting = FALSE;
    }

    ResourceTypeAttributes[next].shared = sharedPtr;
    ResourceTypeAttributes[next].lockType = lockType;
    ResourceTypeAttributes[next].lockOffset = lockOffset;

    numResourceTypeEntries++;
    lastResourceType = next;
    DeleteFuncs = funcs;
    DeleteFuncs[next] = deleteFunc;
    UngrabRDBLocks();
    return (next);
}

/******************************** MTX **********************************/

RESTYPE
CreateNewResourceClass()
{
    RESTYPE next;

    GrabRDBLocks();
    next = lastResourceClass >> 1;
    if (next & lastResourceType)
    {
        UngrabRDBLocks();
	return (0);
    }
    lastResourceClass = next;
    TypeMask = next - 1;
    UngrabRDBLocks();
    return (next);
}
#endif /* not XTHREADS */

static int
Hash(cid, id)
    int cid;
    register XID id;
{
    id &= RESOURCE_ID_MASK;
    switch (clientTable[cid].hashsize)
    {
	case 6:
	    return ((int)(0x03F & (id ^ (id>>6) ^ (id>>12))));
	case 7:
	    return ((int)(0x07F & (id ^ (id>>7) ^ (id>>13))));
	case 8:
	    return ((int)(0x0FF & (id ^ (id>>8) ^ (id>>16))));
	case 9:
	    return ((int)(0x1FF & (id ^ (id>>9))));
	case 10:
	    return ((int)(0x3FF & (id ^ (id>>10))));
	case 11:
	    return ((int)(0x7FF & (id ^ (id>>11))));
    }
    return -1;
}

static XID
AvailableID(cid, id, maxid, goodid)
    register int cid;
    register XID id, maxid, goodid;
{
    register ResourcePtr res;

    if ((goodid >= id) && (goodid <= maxid))
	return goodid;
    for (; id <= maxid; id++)
    {
	res = clientTable[cid].resources[Hash(cid, id)];
	while (res && (res->id != id))
	    res = res->next;
	if (!res)
	    return id;
    }
    return 0;
}

/* MTX note: GetXIDRange must be called while protected by client's MUTEX */
void
GetXIDRange(cid, server, minp, maxp)
    int cid;
    BOOL server;
    XID *minp, *maxp;
{
    register XID id, maxid;
    register ResourcePtr *resp;
    register ResourcePtr res;
    register int i;
    XID goodid;

    id = (Mask)cid << CLIENTOFFSET;
    if (server)
	id |= cid ? SERVER_BIT : SERVER_MINID;
    maxid = id | RESOURCE_ID_MASK;
    goodid = 0;
    for (resp = clientTable[cid].resources, i = clientTable[cid].buckets;
	 --i >= 0;)
    {
	for (res = *resp++; res; res = res->next)
	{
	    if ((res->id < id) || (res->id > maxid))
		continue;
	    if (((res->id - id) >= (maxid - res->id)) ?
		(goodid = AvailableID(cid, id, res->id - 1, goodid)) :
		!(goodid = AvailableID(cid, res->id + 1, maxid, goodid)))
		maxid = res->id - 1;
	    else
		id = res->id + 1;
	}
    }
    if (id > maxid)
	id = maxid = 0;
    *minp = id;
    *maxp = maxid;
}

/*
 * Return the next usable fake client ID.
 *
 * Normally this is just the next one in line, but if we've used the last
 * in the range, we need to find a new range of safe IDs to avoid
 * over-running another client.
 */

XID
FakeClientID(cid)
    register int cid;
{
    XID id, maxid;

    MTX_CLIENT_MUTEX_LOCK(cid);

    id = clientTable[cid].fakeID++;
    if (id != clientTable[cid].endFakeID)
    {
	MTX_CLIENT_MUTEX_UNLOCK(cid);
	return id;
    }
    GetXIDRange(cid, TRUE, &id, &maxid);
    if (!id) {
	if (!cid)
	{
	    MTX_CLIENT_MUTEX_UNLOCK(cid);
	    FatalError("FakeClientID: server internal ids exhausted\n");
	}
	MarkClientException(clients[cid]);
	id = ((Mask)cid << CLIENTOFFSET) | (SERVER_BIT * 3);
	maxid = id | RESOURCE_ID_MASK;
    }
    clientTable[cid].fakeID = id + 1;
    clientTable[cid].endFakeID = maxid + 1;
    MTX_CLIENT_MUTEX_UNLOCK(cid);
    return id;
}

static void
RebuildTable(cid)
    int cid;
{
    register int j;
    register ResourcePtr res, next;
    ResourcePtr **tails, *resources;
    register ResourcePtr **tptr, *rptr;

    /*
     * For now, preserve insertion order, since some ddx layers depend
     * on resources being free in the opposite order they are added.
     */

    j = 2 * clientTable[cid].buckets;
    tails = (ResourcePtr **)ALLOCATE_LOCAL(j * sizeof(ResourcePtr *));
    if (!tails)
	return;
    resources = (ResourcePtr *)xalloc(j * sizeof(ResourcePtr));
    if (!resources)
    {
	DEALLOCATE_LOCAL(tails);
	return;
    }
    for (rptr = resources, tptr = tails; --j >= 0; rptr++, tptr++)
    {
	*rptr = NullResource;
	*tptr = rptr;
    }
    clientTable[cid].hashsize++;
    for (j = clientTable[cid].buckets, rptr = clientTable[cid].resources;
	 --j >= 0;
	 rptr++)
    {
	for (res = *rptr; res; res = next)
	{
	    next = res->next;
	    res->next = NullResource;
	    tptr = &tails[Hash(cid, res->id)];
	    **tptr = res;
	    *tptr = &res->next;
	}
    }
    DEALLOCATE_LOCAL(tails);
    clientTable[cid].buckets *= 2;
    xfree(clientTable[cid].resources);
    clientTable[cid].resources = resources;
}

Bool
AddResource(id, rtype, value)
    XID id;
    RESTYPE rtype;
    pointer value;
{
    register ClientResourceRec *rrec;
    register ResourcePtr res, *head;
    int cid = CLIENT_ID(id);
    	
    MTX_CLIENT_MUTEX_LOCK(cid);

    rrec = &clientTable[cid];
    if (!rrec->buckets)
    {
	ErrorF("AddResource(%x, %x, %x), cid=%d \n",
		id, rtype, (unsigned long)value, cid);
        FatalError("client not in use\n");
    }
    if ((rrec->elements >= 4*rrec->buckets) &&
	(rrec->hashsize < MAXHASHSIZE))
	RebuildTable(cid);
    head = &rrec->resources[Hash(cid, id)];
    res = (ResourcePtr)xalloc(sizeof(ResourceRec));
    if (!res)
    {
        MTX_CLIENT_MUTEX_UNLOCK(cid);
	(*DeleteFuncs[rtype & TypeMask])(value, id);
	return FALSE;
    }
    res->next = *head;
    res->id = id;
    res->type = rtype;
    res->value = value;
    *head = res;
    rrec->elements++;
    if (!(id & SERVER_BIT) && (id >= rrec->expectID))
	rrec->expectID = id + 1;
    MTX_CLIENT_MUTEX_UNLOCK(cid);
    return TRUE;
}

#ifdef XTHREADS
/******************************** MTX **********************************/

static void
CheckLockBits (cid, id, rtype, value)
    int     cid;
    XID     id;
    RESTYPE rtype;
    pointer value;

/*
 *  Summary
 *
 * 	Check the lock bits in the resource to be deleted.
 * 	Wait on condition if resource is currently in use.
 * 	Flush the client cache if necessary.
 *
 ***********************************************************************/
{
    CacheElementPtr pCache;
    if (rtype & RC_DRAWABLE)
    {
	if (((DrawablePtr)value)->lockBits)
	{
	    POQLockSuspend();
	    while (((DrawablePtr)value)->lockBits)
	    {
	        clientTable[cid].lock.waiting = TRUE;
	        RESOURCE_INCREMENT_INTEREST(((DrawablePtr)value)->lockBits);
	        X_COND_WAIT(&clientTable[cid].lock.condition,
	    		          &clientTable[cid].lock.mutex);
	        RESOURCE_DECREMENT_INTEREST(((DrawablePtr)value)->lockBits);
	    }
	    MTX_CLIENT_MUTEX_UNLOCK(cid);
	    POQLockResume();
	    MTX_CLIENT_MUTEX_LOCK(cid);
	}

	pCache = &clientTable[cid].cache.drawable[0];
	if (pCache->lastID == id)
	{
	    pCache->lastID = INVALID;
	    pCache->lastValue = NULL;
	}
	pCache = &clientTable[cid].cache.drawable[1];
	if (pCache->lastID == id)
	{
	    pCache->lastID = INVALID;
	    pCache->lastValue = NULL;
	}
	pCache = &clientTable[cid].cache.rt_cache[RT_WINDOW];
	if (pCache->lastID == id)
	{
	    pCache->lastID = INVALID;
	    pCache->lastValue = NULL;
	}
	pCache = &clientTable[cid].cache.rt_cache[RT_PIXMAP];
	if (pCache->lastID == id)
	{
	    pCache->lastID = INVALID;
	    pCache->lastValue = NULL;
	}
    }
    else if (rtype == RT_GC)
    {
	if (((GC *)value)->lockBits)
	{
	    POQLockSuspend();
	    while (((GC *)value)->lockBits)
	    {
	        clientTable[cid].lock.waiting = TRUE;
	        RESOURCE_INCREMENT_INTEREST(((GC *)value)->lockBits);
	        X_COND_WAIT(&clientTable[cid].lock.condition,
			          &clientTable[cid].lock.mutex);
	        RESOURCE_DECREMENT_INTEREST(((GC *)value)->lockBits);
	    }
	    MTX_CLIENT_MUTEX_UNLOCK(cid);
	    POQLockResume();
	    MTX_CLIENT_MUTEX_LOCK(cid);
	}

        pCache = &clientTable[cid].cache.rt_cache[RT_GC];
        if (pCache->lastID == id)
        {
	    pCache->lastID = INVALID;
	    pCache->lastValue = NULL;
        }
    }
    else
    {
	/* TBD -- ??? wait on lockbits */
	int entry;
	entry = rtype & TypeMask;
	if (entry <= clientTable[cid].cache.nCached)
	{
            pCache = &clientTable[cid].cache.rt_cache[entry];
            if (pCache->lastID == id)
            {
	        pCache->lastID = INVALID;
	        pCache->lastValue = NULL;
            }
	}
    }
}
#endif /* XTHREADS */

void
FreeResource(id, skipDeleteFuncType)
    XID id;
    RESTYPE skipDeleteFuncType;
{
    int		cid = CLIENT_ID(id);
    register    ResourcePtr res;
    register	ResourcePtr *prev, *head;
    register	int *p_elements;
    int		elements;
    Bool	gotOne = FALSE;

    MTX_CLIENT_MUTEX_LOCK(cid);

    if ((cid < MAXCLIENTS) && clientTable[cid].buckets)
    {
	head = &clientTable[cid].resources[Hash(cid, id)];
	p_elements = &clientTable[cid].elements;

	prev = head;
	while (res = *prev)
	{
	    if (res->id == id)
	    {
		RESTYPE rtype = res->type;
		*prev = res->next;
		elements = --*p_elements;
#ifndef XTHREADS
		if (rtype & RC_CACHED)
		    FlushClientCaches(res->id);
#endif /* XTHREADS */
		if (rtype != skipDeleteFuncType)
		{
#ifdef XTHREADS
		    CheckLockBits (cid, id, rtype, res->value);
#endif /* XTHREADS */
		    MTX_CLIENT_MUTEX_UNLOCK(cid);
		    (*DeleteFuncs[rtype & TypeMask])(res->value, res->id);
		    MTX_CLIENT_MUTEX_LOCK(cid);
		}
		xfree(res);
		if (*p_elements != elements)
		    prev = head; /* prev may no longer be valid */
		gotOne = TRUE;
	    }
	    else
		prev = &res->next;
        }
#ifndef XTHREADS
	if(clients[cid] && (id == clients[cid]->lastDrawableID))
	{
	    clients[cid]->lastDrawable = (DrawablePtr)WindowTable[0];
	    clients[cid]->lastDrawableID = WindowTable[0]->drawable.id;
	}
#endif /* XTHREADS */
    }
#ifndef XTHREADS
    if (!gotOne)
	FatalError("Freeing resource id=%X which isn't there", id);
#endif /* XTHREADS */
    MTX_CLIENT_MUTEX_UNLOCK(cid);
}


void
FreeResourceByType(id, rtype, skipFree)
    XID id;
    RESTYPE rtype;
    Bool    skipFree;
{
    register    ResourcePtr res;
    register	ResourcePtr *prev, *head;
    pointer	value;
    int		cid = CLIENT_ID(id);

    if (((cid = CLIENT_ID(id)) < MAXCLIENTS) && clientTable[cid].buckets)

    MTX_CLIENT_MUTEX_LOCK(cid);

    if ((cid < MAXCLIENTS) && clientTable[cid].buckets)
    {
	head = &clientTable[cid].resources[Hash(cid, id)];

	prev = head;
	while (res = *prev)
	{
	    if (res->id == id && res->type == rtype)
	    {
		*prev = res->next;
#ifndef XTHREADS
		if (rtype & RC_CACHED)
		    FlushClientCaches(res->id);
		if (!skipFree)
		    (*DeleteFuncs[rtype & TypeMask])(res->value, res->id);
#else /* XTHREADS */
		value = res->value;
#endif /* XTHREADS */
		xfree(res);
#ifdef XTHREADS
		if (!skipFree)
		{
		    CheckLockBits (cid, id, rtype, value);
		    MTX_CLIENT_MUTEX_UNLOCK(cid);
		    (*DeleteFuncs[rtype & TypeMask])(value, id);
		    return;
		}
#endif /* XTHREADS */
		break;
	    }
	    else
		prev = &res->next;
        }
#ifndef XTHREADS
	if(clients[cid] && (id == clients[cid]->lastDrawableID))
	{
	    clients[cid]->lastDrawable = (DrawablePtr)WindowTable[0];
	    clients[cid]->lastDrawableID = WindowTable[0]->drawable.id;
	}
#endif /* XTHREADS */
    }
    MTX_CLIENT_MUTEX_UNLOCK(cid);
}


/*
 * Change the value associated with a resource id.  Caller
 * is responsible for "doing the right thing" with the old
 * data
 */

Bool
ChangeResourceValue (id, rtype, value)
    XID	id;
    RESTYPE rtype;
    pointer value;
{
    int cid = CLIENT_ID(id);
    register ResourcePtr res;

    MTX_CLIENT_MUTEX_LOCK(cid);

    if ((cid < MAXCLIENTS) && clientTable[cid].buckets)
    {
	res = clientTable[cid].resources[Hash(cid, id)];

	for (; res; res = res->next)
	    if ((res->id == id) && (res->type == rtype))
	    {
#ifndef XTHREADS
		if (rtype & RC_CACHED)
		    FlushClientCaches(res->id);
#else /* XTHREADS */
		/* make sure res->value is not currently in use */
		CheckLockBits (cid, id, rtype, res->value);
#endif /* XTHREADS */
		res->value = value;
	        MTX_CLIENT_MUTEX_UNLOCK(cid);
		return TRUE;
	}
    }
    MTX_CLIENT_MUTEX_UNLOCK(cid);
    return FALSE;
}

void
FreeClientNeverRetainResources(client)
    ClientPtr client;
{
    ResourcePtr *resources;
    ResourcePtr this;
    ResourcePtr *prev;
    int j;
    int cid;

    if (!client)
	return;

    cid = client->index;
    MTX_CLIENT_MUTEX_LOCK(cid);

    resources = clientTable[cid].resources;
    for (j=0; j < clientTable[cid].buckets; j++) 
    {
	prev = &resources[j];
        while ( (this = *prev) )
	{
	    RESTYPE rtype = this->type;
	    if (rtype & RC_NEVERRETAIN)
	    {
		*prev = this->next;
#ifndef XTHREADS
		if (rtype & RC_CACHED)
		    FlushClientCaches(this->id);
#else /* not XTHREADS */
		/* make sure res->value is not currently in use */
		CheckLockBits (cid, this->id, rtype, this->value);
#endif /* not XTHREADS */

		MTX_CLIENT_MUTEX_UNLOCK(cid);
		(*DeleteFuncs[rtype & TypeMask])(this->value, this->id);
		MTX_CLIENT_MUTEX_LOCK(cid);
		xfree(this);	    
	    }
	    else
		prev = &this->next;
	}
    }
    MTX_CLIENT_MUTEX_UNLOCK(cid);
}

void
FreeClientResources(client)
    ClientPtr client;
{
    register ResourcePtr *resources;
    register ResourcePtr this;
    int j;
    int cid;

    /* This routine shouldn't be called with a null client, but just in
	case ... */

    if (!client)
	return;

    cid = client->index;

    MTX_CLIENT_MUTEX_LOCK(cid);
    HandleSaveSet(client);

    resources = clientTable[cid].resources;
    for (j=0; j < clientTable[cid].buckets; j++) 
    {
        /* It may seem silly to update the head of this resource list as
	we delete the members, since the entire list will be deleted any way, 
	but there are some resource deletion functions "FreeClientPixels" for 
	one which do a LookupID on another resource id (a Colormap id in this
	case), so the resource list must be kept valid up to the point that
	it is deleted, so every time we delete a resource, we must update the
	head, just like in FreeResource. I hope that this doesn't slow down
	mass deletion appreciably. PRH */

	ResourcePtr *head;

	head = &resources[j];

        for (this = *head; this; this = *head)
	{
	    RESTYPE rtype = this->type;
	    *head = this->next;
#ifndef XTHREADS
	    if (rtype & RC_CACHED)
		FlushClientCaches(this->id);
#endif /* XTHREADS */
#ifdef XTHREADS
	    /* make sure res->value is not currently in use */
	    CheckLockBits (cid, this->id, rtype, this->value);
#endif /* XTHREADS */
	    MTX_CLIENT_MUTEX_UNLOCK(cid);
	    (*DeleteFuncs[rtype & TypeMask])(this->value, this->id);
	    MTX_CLIENT_MUTEX_LOCK(cid);
	    xfree(this);	    
	}
    }
    xfree(clientTable[cid].resources);
    clientTable[cid].buckets = 0;
    MTX_CLIENT_MUTEX_UNLOCK(cid);
}

void
FreeAllResources()
{
    int	i;

    for (i = currentMaxClients; --i >= 0; ) 
    {
        if (clientTable[i].buckets) 
	    FreeClientResources(clients[i]);
    }
}

Bool
LegalNewID(id, client)
    XID id;
    register ClientPtr client;
{
    return ((client->clientAsMask == (id & ~RESOURCE_ID_MASK)) &&
	    ((clientTable[client->index].expectID <= id) ||
	     !LookupIDByClass(id, RC_ANY)));
}

/*
 *  LookupIDByType returns the object with the given id and type, else NULL.
 *	MTX: The client mutex must be aquired before calling this routine.
 */ 
pointer
LookupIDByType(id, rtype)
    XID id;
    RESTYPE rtype;
{
    int    cid;
    register    ResourcePtr res;

    if (((cid = CLIENT_ID(id)) < MAXCLIENTS) && clientTable[cid].buckets)
    {
	res = clientTable[cid].resources[Hash(cid, id)];

	for (; res; res = res->next)
	    if ((res->id == id) && (res->type == rtype))
		return res->value;
    }
    return (pointer)NULL;
}

/*
 *  LookupIDByClass returns the object with the given id and any one of the
 *  given classes, else NULL.
 *	MTX: The client mutex must be aquired before calling this routine.
 */ 
pointer
LookupIDByClass(id, classes)
    XID id;
    RESTYPE classes;
{
    int    cid;
    register    ResourcePtr res;

    if (((cid = CLIENT_ID(id)) < MAXCLIENTS) && clientTable[cid].buckets)
    {
	res = clientTable[cid].resources[Hash(cid, id)];

	for (; res; res = res->next)
	    if ((res->id == id) && (res->type & classes))
		return res->value;
    }
    return (pointer)NULL;
}

/***** MTX only from here to end of file */
#ifdef XTHREADS
pointer
LookupDrawable(id, client)
    XID id;
    ClientPtr client;
{
    /* ??? */
} 

WindowPtr
LookupWindow(rid, client)
    XID rid;
    ClientPtr client;
{
    int cid = CLIENT_ID(rid);
    CacheElementPtr pCache;

    client->errorValue = rid;
    if(rid == INVALID)
    {
        return NULL;
    }

    pCache = &clientTable[cid].cache.rt_cache[RT_WINDOW];
    if (pCache->lastID == rid)
    {
        if (((DrawablePtr)(pCache->lastValue))->type != DRAWABLE_PIXMAP)
        {
            return ((WindowPtr) pCache->lastValue);
        }
        return (WindowPtr) NULL;
    }
    return (WindowPtr)LookupIDByType(rid, RT_WINDOW);
}

/*--------------------------------------------------------------------
 *-------------------------------------------------------------------*/

pointer
LockResourceByType(resID, resType, lockType)
XID resID;
RESTYPE resType;
ResourceLockType lockType;
{
    pointer pRes;
    ResourceLockPtr pLock;
    CacheElementPtr pCache = NULL;
    int resCID = CLIENT_ID(resID);

    pLock = &clientTable[resCID].lock;
    if ((resType & TypeMask) <= clientTable[resCID].cache.nCached)
    {
        pCache = &clientTable[resCID].cache.rt_cache[resType];
    }

    MTX_MUTEX_LOCK(&pLock->mutex);
    LOOKUP_RESOURCE_TYPE(resID, pRes, pCache, void, resType);
    if (pRes)
    {
	int lockOffset;
        lockOffset = ResourceTypeAttributes[resType].lockOffset;
        LOCK_RESOURCE(pRes, lockOffset, lockType, pLock);
    }

    MTX_MUTEX_UNLOCK(&pLock->mutex);
    return pRes;
}

/*--------------------------------------------------------------------
 *-------------------------------------------------------------------*/

pointer
LockResourceByClass(resID, resType, lockType)
XID resID;
RESTYPE resType;
ResourceLockType lockType;
{
    pointer pRes;
    ResourceLockPtr pLock;
    CacheElementPtr pCache = NULL;
    int resCID = CLIENT_ID(resID);

    pLock = &clientTable[resCID].lock;
    if ((resType & TypeMask) <= clientTable[resCID].cache.nCached)
    {
        pCache = &clientTable[resCID].cache.rt_cache[resType];
    }

    MTX_MUTEX_LOCK(&pLock->mutex);
    LOOKUP_RESOURCE_TYPE(resID, pRes, pCache, void, resType);
    if (pRes)
    {
	int lockOffset;
        lockOffset = ResourceTypeAttributes[resType].lockOffset;
        LOCK_RESOURCE(pRes, lockOffset, lockType, pLock);
    }

    MTX_MUTEX_UNLOCK(&pLock->mutex);
    return pRes;
}

/*--------------------------------------------------------------------
 *-------------------------------------------------------------------*/

static
void
OrderResources(resArray, numResources, order)
ResourceArrayPtr resArray;
int numResources;
int *order;
{
    int i;
    for (i = 0; i < numResources; i++)
    {
	order[i] = i;
    }
}

/*--------------------------------------------------------------------
 *-------------------------------------------------------------------*/

int
LockResources(resArray, numResources, doOrdering)
ResourceArrayPtr resArray;
int numResources;
Bool doOrdering;
{
    int i, order[64];
    RESTYPE resTypePrev = INVALID;
    ResourceLockPtr pLockCurrent = NULL;

    if (doOrdering)
    {
	OrderResources(resArray, numResources, order);
    }

    for (i = 0; i < numResources; i++)
    {
        XID resID;
	int j, nextIndex;
	int lockOffset;
	pointer pRes;
        RESTYPE resType;
	ResourcePtr pResRec;
        ResourceLockPtr pLock;
        ResourceLockPtr pShared;
        ResourceArrayPtr current;
	ResourceLockType lockType;
	ResourceLockType resLockType;

        nextIndex = (doOrdering)? order[i]: i;
	current = &resArray[nextIndex];
	lockType = current->lockType;
	resType = current->resKind;
	resID = current->resID;

        pLock = &clientTable[CLIENT_ID(resID)].lock;
        if (pLock != pLockCurrent)
	{
	    if (pLockCurrent != NULL)
	    {
	        MTX_MUTEX_UNLOCK(&pLockCurrent->mutex);
            }
            MTX_MUTEX_LOCK(&pLock->mutex);
	    pLockCurrent = pLock;
	}

        /*----------------------------------------------------------
	 *---------------------------------------------------------*/

        if (resType & TypeMask)
	{
            int cid = CLIENT_ID(resID);
            CacheElementPtr pCache = NULL;
            if ((resType & TypeMask) <= clientTable[cid].cache.nCached)
	    {
                pCache = &clientTable[cid].cache.rt_cache[resType];
            }

            LOOKUP_RESOURCE_TYPE(resID, pRes, pCache, void, resType);
            if (!pRes)
	    {
	        MTX_MUTEX_UNLOCK(&pLock->mutex);
	        for (j = i - 1; j >= 0; j--)
	        {
		    UnlockResourceByType(resArray[j].pResource,
		    	                 resArray[j].resID,
			                 resArray[j].resType,
			                 resArray[j].lockType);

		    resArray[j].pResource = NULL;
                }
		return ResourceBadValue(resType);
            }
        }
	else
	{
	    pResRec = LookupResourceRecByClass(resID, resType);
            if (!pResRec)
	    {
	        MTX_MUTEX_UNLOCK(&pLock->mutex);
	        for (j = i - 1; j >= 0; j--)
	        {
		    UnlockResourceByType(resArray[j].pResource,
		    	                 resArray[j].resID,
			                 resArray[j].resType,
			                 resArray[j].lockType);

		    resArray[j].pResource = NULL;
                }
		return ResourceBadValue(resType);
            }

	    pRes = pResRec->value;
            resType = pResRec->type & TypeMask;
            resLockType = ResourceTypeAttributes[resType].lockType;

	    if (resLockType == reader_writer_lock)
	    {
		if (lockType != read_lock)
		{
		    lockType = write_lock;
		}
            }
	    else
	    {
		lockType = resLockType;
	    }

            current->lockType = lockType;
	}

        /*----------------------------------------------------------
	 *---------------------------------------------------------*/

	if (resType != resTypePrev)
	{
	    lockOffset = ResourceTypeAttributes[resType].lockOffset;
	    pShared = ResourceTypeAttributes[resType].shared;
	    if (pShared && lockType != no_lock)
	    {
		pLock = pShared;
            }
	}

        if (pLock != pLockCurrent)
	{
	    if (pLockCurrent != NULL)
	    {
	        MTX_MUTEX_UNLOCK(&pLockCurrent->mutex);
            }
            MTX_MUTEX_LOCK(&pLock->mutex);
	    pLockCurrent = pLock;
	}

        LOCK_RESOURCE(pRes, lockOffset, lockType, pLock);
	resArray[i].pLock = pLockCurrent;
	resArray[i].pResource = pRes;
	resArray[i].resType = resType;
	resTypePrev = resType;
    }

    if (pLockCurrent != NULL)
    {
        MTX_MUTEX_UNLOCK(&pLockCurrent->mutex);
    }
    return Success;
}

/*--------------------------------------------------------------------
 *-------------------------------------------------------------------*/

UnlockResources(resArray, numResources, doOrdering)
ResourceArrayPtr resArray;
int numResources;
Bool doOrdering;
{
    int i, order[64];
    Bool check_broadcast = FALSE;
    ResourceLockPtr pLockCurrent = NULL;

    if (doOrdering)
    {
	/* TBD -- ordering algorithm */
	for (i = 0; i < numResources; i++)
	{
	    order[i] = i;
        }
    }

    for (i = numResources - 1; i >= 0; i--)
    {
        XID resID;
	Bool broadcast;
	int lockOffset;
	int j, nextIndex;
	pointer pRes;
        ResourceLockPtr pLock;
        ResourceArrayPtr current;
	ResourceLockType lockType;
	ResourceLockbitsPtr pLockbits;

        nextIndex = (doOrdering)? order[i]: i;
	current = &resArray[nextIndex];
	lockType = current->lockType;
	resID = current->resID;

        pLock = &clientTable[CLIENT_ID(resID)].lock;
        if (pLock != pLockCurrent)
	{
	    if (pLockCurrent != NULL)
	    {
		if (check_broadcast)
		{
		    BROADCAST_WAKEUP(pLock);
		    check_broadcast = FALSE;
                }
	        MTX_MUTEX_UNLOCK(&pLockCurrent->mutex);
            }
            MTX_MUTEX_LOCK(&pLock->mutex);
	    pLockCurrent = pLock;
	}
	
	pRes = current->pResource;
	lockOffset = ResourceTypeAttributes[current->resType].lockOffset;
        pLockbits = (ResourceLockbitsPtr) ((char *) pRes) + lockOffset;
        broadcast = UnlockResourceBits(lockType, pLockbits);
	check_broadcast = check_broadcast || broadcast;
	resArray[i].pLock = NULL;
	resArray[i].pResource = NULL;
    }

    if (pLockCurrent != NULL)
    {
        if (check_broadcast)
        {
            BROADCAST_WAKEUP(pLockCurrent);
        }
        MTX_MUTEX_UNLOCK(&pLockCurrent->mutex);
    }
}

/*--------------------------------------------------------------------
 *-------------------------------------------------------------------*/

static
Bool
UnlockResourceBits(lockType, pLockbits)
ResourceLockType lockType;
ResourceLockbitsPtr pLockbits;
{
    switch (lockType)
    {
    case no_lock:
        return FALSE;

    case exclusive_lock:
        LOCKBITS_CLEAR_EXCLUSIVE(*pLockbits);
        return RESOURCE_HAS_WAITER(*pLockbits);

    case read_lock:
        LOCKBITS_CLEAR_READ(*pLockbits);
        return RESOURCE_HAS_NO_READERS(*pLockbits) && 
	       RESOURCE_HAS_WAITER(*pLockbits);

    case write_lock:
        LOCKBITS_CLEAR_WRITE(*pLockbits);
        return RESOURCE_HAS_WAITER(*pLockbits);

    default:
        return FALSE;
    }
}

static
int
ResourceBadValue(resType)
RESTYPE resType;
{
    switch (resType)
    {
    case RT_WINDOW:   return BadWindow;
    case RT_PIXMAP:   return BadPixmap;
    case RT_CURSOR:   return BadCursor;
    case RT_COLORMAP: return BadColor;
    case RT_FONT:     return BadFont;
    case RC_DRAWABLE: return BadDrawable;
    default:          return BadValue;
    }
}

/*--------------------------------------------------------------------
 *-------------------------------------------------------------------*/


static
pointer
LookupResourceByClass(rid, rc)
XID rid;
RESTYPE rc;
{
    ResourcePtr res;
    res = LookupResourceRecByClass(rid, rc);
    if (res)
    {
	return res->value;
    }

    return NULL;
}

/*--------------------------------------------------------------------
 *-------------------------------------------------------------------*/

static
ResourcePtr
LookupResourceRecByClass(rid, rc)
XID rid;
RESTYPE rc;
{
    int cid = CLIENT_ID(rid);

    if (cid < MAXCLIENTS && clientTable[cid].buckets)
    {
	ResourcePtr res;
        res = clientTable[cid].resources[Hash(cid, rid)];
        for (; res; res = res->next)
        {
            if ((res->id == rid)  && (res->type & rc))
            {
                return res;
            }
        }
    }

    return NULL;
}

/*--------------------------------------------------------------------
 *-------------------------------------------------------------------*/

void
UnlockResourceByType(pResource, rid, rt, lockType)
pointer pResource;
XID rid;
RESTYPE rt;
ResourceLockType lockType;
{
    ResourceLockPtr pLock;
    ResourceLockPtr pSharedLock;
    ResourceLockbitsPtr pLockbits;
    int cid = CLIENT_ID(rid);
    int lockOffset;

    lockOffset = ResourceTypeAttributes[rt].lockOffset;
    pSharedLock = ResourceTypeAttributes[rt].shared;
    pLockbits = (ResourceLockbitsPtr) (((char *) pResource) + lockOffset);
    if (pSharedLock)
    {
        pLock = pSharedLock;
    }
    else
    {
	pLock = &clientTable[cid].lock;
    }

    MTX_MUTEX_LOCK(&pLock->mutex);
    if (UnlockResourceBits(lockType, pLockbits))
    {
        BROADCAST_WAKEUP(pLock);
    }
    MTX_MUTEX_UNLOCK(&pLock->mutex);
}

/*--------------------------------------------------------------------
 *-------------------------------------------------------------------*/

int
LockAndVerifyDrawableAndGC(ppDraw, ppGC, drawID, gcID, client)
DrawablePtr *ppDraw;
GC **ppGC;
XID drawID;
XID gcID;
ClientPtr client;
{
    GC *pGC;
    DrawablePtr pDraw;
    ResourceLockPtr pLock;
    CacheElementPtr pCache;
    int gcCID = CLIENT_ID(gcID);
    int drawCID = CLIENT_ID(drawID);
    
    pLock = &clientTable[drawCID].lock;
    pCache = &clientTable[drawCID].cache.drawable[0];

    MTX_MUTEX_LOCK(&pLock->mutex);
    LOOKUP_DRAWABLE(drawID, pDraw, pCache);
    if (!pDraw)
    {
	MTX_MUTEX_UNLOCK(&pLock->mutex);
	return BadDrawable;
    }

    if (pDraw->type == DRAWABLE_WINDOW)
    {
        LOCK_RESOURCE_READ(pDraw->lockBits, pLock);
    }
    else if (pDraw->type == DRAWABLE_PIXMAP)
    {
        LOCK_RESOURCE_EXCLUSIVE(pDraw->lockBits, pLock);
    }
    else
    {
	client->errorValue = drawID;
	MTX_MUTEX_UNLOCK(&pLock->mutex);
	return BadMatch;
    }

    CHECK_NEXT_LOOKUP_LOCK(drawCID, gcCID, pLock);
    pCache = &clientTable[gcCID].cache.rt_cache[RT_GC];
    LOOKUP_RESOURCE_TYPE(gcID, pGC, pCache, GC, RT_GC);
    if (!pGC)
    {
	MTX_MUTEX_UNLOCK(&pLock->mutex);
	UnlockDrawable(pDraw, drawID);
	return BadGC;
    }

    LOCK_RESOURCE_EXCLUSIVE(pGC->lockBits, pLock);
    MTX_MUTEX_UNLOCK(&pLock->mutex);

    if ((pGC->depth != pDraw->depth) || (pGC->pScreen != pDraw->pScreen))
    {
	UnlockDrawableAndGC(pDraw, pGC, drawID, gcID);
	return BadMatch;
    }

    *ppDraw = pDraw;
    *ppGC = pGC;
    return Success;
}

/*--------------------------------------------------------------------
 *-------------------------------------------------------------------*/

int
LockAndVerifyTwoDrawablesAndGC(ppSrc, ppDst, ppGC, srcID, dstID, gcID, client)
DrawablePtr *ppSrc;
DrawablePtr *ppDst;
GC **ppGC;
XID srcID;
XID dstID;
XID gcID;
ClientPtr client;
{
    GC *pGC;
    DrawablePtr pSrc;
    DrawablePtr pDst;
    DrawablePtr pFirstDraw;
    DrawablePtr pSecondDraw;
    ResourceLockPtr pLock;
    CacheElementPtr pCache;

    int firstDrawID;
    int secondDrawID;
    int firstDrawCID;
    int secondDrawCID;
    int gcCID;

    firstDrawID = min(srcID, dstID);
    secondDrawID = max(srcID, dstID);
    firstDrawCID = CLIENT_ID(firstDrawID);
    secondDrawCID = CLIENT_ID(secondDrawID);
    gcCID = CLIENT_ID(gcID);

    pLock = &clientTable[firstDrawCID].lock;
    pCache = &clientTable[firstDrawCID].cache.drawable[0];

    MTX_MUTEX_LOCK(&pLock->mutex);
    LOOKUP_DRAWABLE(firstDrawID, pFirstDraw, pCache);

    if (!pFirstDraw)
    {
        client->errorValue = firstDrawID;
        MTX_MUTEX_UNLOCK(&pLock->mutex);
        return BadDrawable;
    }
    else if (pFirstDraw->type == DRAWABLE_WINDOW)
    {
        LOCK_RESOURCE_READ(pFirstDraw->lockBits, pLock);
    }
    else if (pFirstDraw->type == DRAWABLE_PIXMAP)
    {
        LOCK_RESOURCE_EXCLUSIVE(pFirstDraw->lockBits, pLock);
    }
    else
    {
        client->errorValue = firstDrawID;
        MTX_MUTEX_UNLOCK(&pLock->mutex);
        return BadMatch;
    }

    CHECK_NEXT_LOOKUP_LOCK(firstDrawCID, secondDrawCID, pLock);
    pCache = &clientTable[secondDrawCID].cache.drawable[1];
    LOOKUP_DRAWABLE(secondDrawID, pSecondDraw, pCache);

    if (!pSecondDraw)
    {
        client->errorValue = secondDrawID;
        MTX_MUTEX_UNLOCK(&pLock->mutex);
	UnlockDrawable(pFirstDraw, firstDrawID);
        return BadDrawable;
    }
    else if (pSecondDraw->type == DRAWABLE_WINDOW)
    {
        LOCK_RESOURCE_READ(pSecondDraw->lockBits, pLock);
    }
    else if (pSecondDraw->type == DRAWABLE_PIXMAP)
    {
        LOCK_RESOURCE_EXCLUSIVE(pSecondDraw->lockBits, pLock);
    }
    else
    {
        client->errorValue = secondDrawID;
        MTX_MUTEX_UNLOCK(&pLock->mutex);
	UnlockDrawable(pFirstDraw, firstDrawID);
        return BadMatch;
    }

    CHECK_NEXT_LOOKUP_LOCK(secondDrawCID, gcCID, pLock);
    pCache = &clientTable[gcCID].cache.rt_cache[RT_GC];

    LOOKUP_RESOURCE_TYPE(gcID, pGC, pCache, GC, RT_GC);
    if (!pGC)
    {
	MTX_MUTEX_UNLOCK(&pLock->mutex);
	UnlockDrawable(pSecondDraw, secondDrawID);
	UnlockDrawable(pFirstDraw, firstDrawID);
	return BadGC;
    }

    LOCK_RESOURCE_EXCLUSIVE(pGC->lockBits, pLock);
    MTX_MUTEX_UNLOCK(&pLock->mutex);

    if (srcID < dstID)
    {
	pSrc = pFirstDraw;
	pDst = pSecondDraw;
    }
    else
    {
	pSrc = pSecondDraw;
	pDst = pFirstDraw;
    }

    if ((pGC->depth != pDst->depth)     || 
        (pGC->pScreen != pDst->pScreen) ||
        (pGC->pScreen != pSrc->pScreen))
    {
	UnlockTwoDrawablesAndGC(pSrc, pDst, pGC, srcID, dstID, gcID);
        return BadMatch;
    }

    *ppSrc = pSrc;
    *ppDst = pDst;
    *ppGC = pGC;

    return Success;
}

/*--------------------------------------------------------------------
 *-------------------------------------------------------------------*/

int
LockAndVerifyDrawable(ppDraw, drawID, client, returnBadMatch)
DrawablePtr *ppDraw;
XID drawID;
ClientPtr client;
Bool returnBadMatch;
{
    DrawablePtr pDraw;
    ResourceLockPtr pLock;
    CacheElementPtr pCache;
    int drawCID = CLIENT_ID(drawID);
    
    pLock = &clientTable[drawCID].lock;
    pCache = &clientTable[drawCID].cache.drawable[0];

    MTX_MUTEX_LOCK(&pLock->mutex);
    LOOKUP_DRAWABLE(drawID, pDraw, pCache);
    if (!pDraw)
    {
	MTX_MUTEX_UNLOCK(&pLock->mutex);
	return BadDrawable;
    }

    if (pDraw->type == DRAWABLE_WINDOW)
    {
        LOCK_RESOURCE_READ(pDraw->lockBits, pLock);
    }
    else if (pDraw->type == DRAWABLE_PIXMAP)
    {
        LOCK_RESOURCE_EXCLUSIVE(pDraw->lockBits, pLock);
    }
    else
    {
	if (returnBadMatch)
	{
	    client->errorValue = drawID;
	    MTX_MUTEX_UNLOCK(&pLock->mutex);
	    return BadMatch;
	}
	else
	{
            LOCK_RESOURCE_READ(pDraw->lockBits, pLock);
	}
    }

    MTX_MUTEX_UNLOCK(&pLock->mutex);
    *ppDraw = pDraw;
    return Success;
}

/*--------------------------------------------------------------------
 *-------------------------------------------------------------------*/

int
LockAndVerifyPixmap(ppPixmap, pixmapID, client)
PixmapPtr *ppPixmap;
XID pixmapID;
ClientPtr client;
{
    PixmapPtr pPixmap;
    ResourceLockPtr pLock;
    CacheElementPtr pCache;
    int pixmapCID = CLIENT_ID(pixmapID);

    pLock = &clientTable[pixmapCID].lock;
    pCache = &clientTable[pixmapCID].cache.rt_cache[RT_PIXMAP];
    MTX_MUTEX_LOCK(&pLock->mutex);

    LOOKUP_RESOURCE_TYPE(pixmapID, pPixmap, pCache, struct _Pixmap, RT_PIXMAP);
    if (!pPixmap)
    {
	client->errorValue = pixmapID;
	MTX_MUTEX_UNLOCK(&pLock->mutex);
	return BadPixmap;
    }

    LOCK_RESOURCE_EXCLUSIVE(pPixmap->drawable.lockBits, pLock);
    MTX_MUTEX_UNLOCK(&pLock->mutex);
    *ppPixmap = pPixmap;
    return Success;
}

/*--------------------------------------------------------------------
 *-------------------------------------------------------------------*/

int
LockAndVerifyGC(ppGC, gcID, client)
GC **ppGC;
XID gcID;
ClientPtr client;
{
    GC *pGC;
    ResourceLockPtr pLock;
    CacheElementPtr pCache;
    int gcCID = CLIENT_ID(gcID);

    pLock = &clientTable[gcCID].lock;
    pCache = &clientTable[gcCID].cache.rt_cache[RT_GC];
    MTX_MUTEX_LOCK(&pLock->mutex);

    LOOKUP_RESOURCE_TYPE(gcID, pGC, pCache, GC, RT_GC);
    if (!pGC)
    {
	MTX_MUTEX_UNLOCK(&pLock->mutex);
	return BadGC;
    }

    LOCK_RESOURCE_EXCLUSIVE(pGC->lockBits, pLock);
    MTX_MUTEX_UNLOCK(&pLock->mutex);
    *ppGC = pGC;
    return Success;
}

/*--------------------------------------------------------------------
 *-------------------------------------------------------------------*/

int
LockAndVerifyTwoGCs (ppGC1, ppGC2, gcID1, gcID2, client)
GC **ppGC1;
GC **ppGC2;
XID gcID1;
XID gcID2;
ClientPtr client;
{
    GC *pGC1, *pFirstGC;
    GC *pGC2, *pSecondGC;
    ResourceLockPtr pLock;
    CacheElementPtr pCache;

    int firstID;
    int secondID;
    int firstCID;
    int secondCID;

    firstID = min(gcID1, gcID2);
    secondID = max(gcID1, gcID2);
    firstCID = CLIENT_ID(firstID);
    secondCID = CLIENT_ID(secondID);

    pLock = &clientTable[firstCID].lock;
    pCache = &clientTable[firstCID].cache.rt_cache[RT_GC];
    MTX_MUTEX_LOCK(&pLock->mutex);

    LOOKUP_RESOURCE_TYPE(firstID, pFirstGC, pCache, GC, RT_GC);
    if (!pFirstGC)
    {
	MTX_MUTEX_UNLOCK(&pLock->mutex);
	return BadGC;
    }

    LOCK_RESOURCE_EXCLUSIVE(pFirstGC->lockBits, pLock);
    CHECK_NEXT_LOOKUP_LOCK(firstCID, secondCID, pLock);

    LOOKUP_RESOURCE_TYPE(secondID, pSecondGC, pCache, GC, RT_GC);
    if (!pSecondGC)
    {
	MTX_MUTEX_UNLOCK(&pLock->mutex);
	UnlockGC(pFirstGC, firstID);
	return BadGC;
    }

    LOCK_RESOURCE_EXCLUSIVE(pSecondGC->lockBits, pLock);
    MTX_MUTEX_UNLOCK(&pLock->mutex);

    if (gcID1 < gcID2)
    {
	*ppGC1 = pFirstGC;
	*ppGC2 = pSecondGC;
    }
    else
    {
	*ppGC1 = pSecondGC;
	*ppGC2 = pFirstGC;
    }

    return (Success);
}

/*--------------------------------------------------------------------
 *-------------------------------------------------------------------*/

int
LockAndVerifyWindow (ppWin, winID, client)
WindowPtr *ppWin;
XID winID;
ClientPtr client;
{
    WindowPtr pWin;
    ResourceLockPtr pLock;
    CacheElementPtr pCache;
    int winCID = CLIENT_ID(winID);

    pLock = &clientTable[winCID].lock;
    pCache = &clientTable[winCID].cache.rt_cache[RT_WINDOW];
    MTX_MUTEX_LOCK(&pLock->mutex);

    LOOKUP_RESOURCE_TYPE(winID, pWin, pCache, struct _Window, RT_WINDOW);
    if (!pWin)
    {
        MTX_MUTEX_UNLOCK(&pLock->mutex);
        return BadWindow;
    }

    LOCK_RESOURCE_READ(pWin->drawable.lockBits, pLock);
    MTX_MUTEX_UNLOCK(&pLock->mutex);
    *ppWin = pWin;
    return Success;
}

/*--------------------------------------------------------------------
 *-------------------------------------------------------------------*/

int
LockAndVerifyTwoWindows (ppWin1, ppWin2, win1ID, win2ID, client)
WindowPtr *ppWin1;
WindowPtr *ppWin2;
XID win1ID;
XID win2ID;
ClientPtr client;
{
    WindowPtr pWin1;
    WindowPtr pWin2;
    ResourceLockPtr pLock;
    CacheElementPtr pCache;
    int win1CID = CLIENT_ID(win1ID);
    int win2CID = CLIENT_ID(win2ID);

    pLock = &clientTable[win1CID].lock;
    pCache = &clientTable[win1CID].cache.drawable[0];
    MTX_MUTEX_LOCK(&pLock->mutex);

    LOOKUP_RESOURCE_TYPE(win1ID, pWin1, pCache, struct _Window, RT_WINDOW);
    if (!pWin1)
    {
        MTX_MUTEX_UNLOCK(&pLock->mutex);
        return BadWindow;
    }

    LOCK_RESOURCE_READ(pWin1->drawable.lockBits, pLock);
    CHECK_NEXT_LOOKUP_LOCK(win1CID, win2CID, pLock);
    pCache = &clientTable[win2CID].cache.drawable[1];

    LOOKUP_RESOURCE_TYPE(win2ID, pWin2, pCache, struct _Window, RT_WINDOW);
    if (!pWin2)
    {
        MTX_MUTEX_UNLOCK(&pLock->mutex);
#ifdef XTHREADS
	UnlockWindow(pWin1, win1ID);
#endif /* XTHREADS */
        return BadWindow;
    }

    LOCK_RESOURCE_READ(pWin2->drawable.lockBits, pLock);
    MTX_MUTEX_UNLOCK(&pLock->mutex);

    *ppWin1 = pWin1;
    *ppWin2 = pWin2;
    return Success;
}

/*--------------------------------------------------------------------
 *-------------------------------------------------------------------*/

int
LockAndVerifyColormap (ppMap, mapID, client)
ColormapPtr *ppMap;
XID mapID;
ClientPtr client;
{
    ColormapPtr pMap;
    ResourceLockPtr pLock;
    CacheElementPtr pCache;
    int mapCID = CLIENT_ID(mapID);

    pLock = &clientTable[mapCID].lock;
    pCache = &clientTable[mapCID].cache.rt_cache[RT_COLORMAP];
    MTX_MUTEX_LOCK(&pLock->mutex);

    LOOKUP_RESOURCE_TYPE(mapID, pMap, pCache, struct _ColormapRec, RT_COLORMAP);
    if (!pMap)
    {
	MTX_MUTEX_UNLOCK(&pLock->mutex);
	client->errorValue = mapID;
	return BadColor;
    }

    /* no RDB locking for colormaps */
    MTX_MUTEX_UNLOCK(&pLock->mutex);
    *ppMap = pMap;
    return Success;
}

/*--------------------------------------------------------------------
 *-------------------------------------------------------------------*/

int
LockAndVerifyFont (ppFont, fontID, client, lookupGC)
FontPtr *ppFont;
XID fontID;
ClientPtr client;
Bool lookupGC;
{
    FontPtr pFont;
    ResourceLockPtr pLock;
    CacheElementPtr pCache;
    int fontCID = CLIENT_ID(fontID);

    pLock = &clientTable[fontCID].lock;
    pCache = &clientTable[fontCID].cache.rt_cache[RT_FONT];
    MTX_MUTEX_LOCK(&pLock->mutex);

    LOOKUP_RESOURCE_TYPE(fontID, pFont, pCache, struct _Font, RT_FONT);
    if (!pFont)
    {
	if (lookupGC)
	{
	    GC *pGC;
	    int error;

            pCache = &clientTable[fontCID].cache.rt_cache[RT_GC];
            LOOKUP_RESOURCE_TYPE(fontID, pGC, pCache, GC, RT_GC);
	    if (!pGC)
	    {
		MTX_MUTEX_UNLOCK(&pLock->mutex);
		client->errorValue = fontID;
		return BadFont;
	    }

	    pFont = pGC->font;
	}
	else
	{
            MTX_MUTEX_UNLOCK(&pLock->mutex);
	    client->errorValue = fontID;
	    return BadFont;
	}
    }

    MTX_MUTEX_UNLOCK(&pLock->mutex);
    *ppFont = pFont;
    return Success;
}


/*--------------------------------------------------------------------
 *-------------------------------------------------------------------*/

int
LockAndVerifyCursor (ppCursor, cursorID, client)
CursorPtr *ppCursor;
XID cursorID;
ClientPtr client;
{
    CursorPtr pCursor;
    ResourceLockPtr pLock;
    CacheElementPtr pCache;
    int cursorCID = CLIENT_ID(cursorID);

    pLock = &clientTable[cursorCID].lock;
    pCache = &clientTable[cursorCID].cache.rt_cache[RT_CURSOR];
    MTX_MUTEX_LOCK(&pLock->mutex);

    LOOKUP_RESOURCE_TYPE(cursorID, pCursor, pCache, struct _Cursor, RT_CURSOR);
    if (!pCursor)
    {
	MTX_MUTEX_UNLOCK(&pLock->mutex);
	client->errorValue = cursorID;
	*ppCursor = NULL;
	return BadCursor;
    }

    /* no RDB locking for cursors */
    MTX_MUTEX_UNLOCK(&pLock->mutex);
    *ppCursor = pCursor;
    return Success;
}

/*--------------------------------------------------------------------
 *-------------------------------------------------------------------*/

void
UnlockDrawableAndGC (pDraw, pGC, drawID, gcID)
DrawablePtr pDraw;
GC *pGC;
XID drawID;
XID gcID;
{
    Bool broadcast;
    ResourceLockPtr pLock;
    ResourceLockType drawLockType;
    int drawCID = CLIENT_ID(drawID);
    int gcCID = CLIENT_ID(gcID);

    pLock = &clientTable[drawCID].lock;
    MTX_MUTEX_LOCK(&pLock->mutex);

    drawLockType = (pDraw->type == DRAWABLE_PIXMAP)? exclusive_lock: read_lock;
    broadcast = UnlockResourceBits(drawLockType, &pDraw->lockBits);

    CHECK_NEXT_LOCK_WITH_BROADCAST(drawCID, gcCID, pLock, broadcast);
    broadcast |= UnlockResourceBits(exclusive_lock, &pGC->lockBits);
    if (broadcast)
    {
	pLock->waiting = FALSE;
	X_COND_BROADCAST(&pLock->condition);
    }

    MTX_MUTEX_UNLOCK(&pLock->mutex);
}

/*--------------------------------------------------------------------
 *-------------------------------------------------------------------*/

void
UnlockTwoDrawablesAndGC (pSrc, pDst, pGC, srcID, dstID, gcID)
DrawablePtr pSrc;
DrawablePtr pDst;
GC *pGC;
XID srcID;
XID dstID;
XID gcID;
{
    Bool broadcast;
    ResourceLockPtr pLock;
    ResourceLockType drawLockType;
    int srcCID = CLIENT_ID(srcID);
    int dstCID = CLIENT_ID(dstID);
    int gcCID = CLIENT_ID(gcID);


    pLock = &clientTable[srcCID].lock;
    MTX_MUTEX_LOCK(&pLock->mutex);

    drawLockType = (pSrc->type == DRAWABLE_PIXMAP)? exclusive_lock: read_lock;
    broadcast = UnlockResourceBits(drawLockType, &pSrc->lockBits);

    CHECK_NEXT_LOCK_WITH_BROADCAST(srcCID, dstCID, pLock, broadcast);
    drawLockType = (pDst->type == DRAWABLE_PIXMAP)? exclusive_lock: read_lock;
    broadcast |= UnlockResourceBits(drawLockType, &pDst->lockBits);

    CHECK_NEXT_LOCK_WITH_BROADCAST(dstCID, gcCID, pLock, broadcast);
    broadcast |= UnlockResourceBits(exclusive_lock, &pGC->lockBits);
    if (broadcast)
    {
	pLock->waiting = FALSE;
	X_COND_BROADCAST(&pLock->condition);
    }

    MTX_MUTEX_UNLOCK(&pLock->mutex);
}

/*--------------------------------------------------------------------
 *-------------------------------------------------------------------*/

void
UnlockDrawable(pDraw, drawID)
DrawablePtr pDraw;
XID drawID;
{
    Bool broadcast;
    ResourceLockPtr pLock;
    ResourceLockType drawLockType;
    int drawCID = CLIENT_ID(drawID);

    pLock = &clientTable[drawCID].lock;
    MTX_MUTEX_LOCK(&pLock->mutex);

    drawLockType = (pDraw->type == DRAWABLE_PIXMAP)? exclusive_lock: read_lock;
    broadcast = UnlockResourceBits(drawLockType, &pDraw->lockBits);

    if (broadcast)
    {
	pLock->waiting = FALSE;
	X_COND_BROADCAST(&pLock->condition);
    }

    MTX_MUTEX_UNLOCK(&pLock->mutex);
}

/*--------------------------------------------------------------------
 *-------------------------------------------------------------------*/

void
UnlockGC (pGC, gcID)
GC *pGC;
XID gcID;
{
    Bool broadcast;
    ResourceLockPtr pLock;
    int gcCID = CLIENT_ID(gcID);

    pLock = &clientTable[gcCID].lock;
    MTX_MUTEX_LOCK(&pLock->mutex);

    broadcast = UnlockResourceBits(exclusive_lock, &pGC->lockBits);
    if (broadcast)
    {
	pLock->waiting = FALSE;
	X_COND_BROADCAST(&pLock->condition);
    }

    MTX_MUTEX_UNLOCK(&pLock->mutex);
}

/*--------------------------------------------------------------------
 *-------------------------------------------------------------------*/

void
UnlockTwoGCs (pGC1, pGC2, gcID1, gcID2)
GC *pGC1;
GC *pGC2;
XID gcID1;
XID gcID2;
{
    Bool broadcast;
    ResourceLockPtr pLock;
    int gc1CID = CLIENT_ID(gcID1);
    int gc2CID = CLIENT_ID(gcID2);

    pLock = &clientTable[gc1CID].lock;
    MTX_MUTEX_LOCK(&pLock->mutex);

    broadcast = UnlockResourceBits(exclusive_lock, &pGC1->lockBits);
    CHECK_NEXT_LOCK_WITH_BROADCAST(gc1CID, gc2CID, pLock, broadcast);

    broadcast |= UnlockResourceBits(exclusive_lock, &pGC2->lockBits);
    if (broadcast)
    {
	pLock->waiting = FALSE;
	X_COND_BROADCAST(&pLock->condition);
    }

    MTX_MUTEX_UNLOCK(&pLock->mutex);
}

/*--------------------------------------------------------------------
 *-------------------------------------------------------------------*/

void
UnlockWindow (pWin, winID)
WindowPtr pWin;
XID winID;
{
    Bool broadcast;
    ResourceLockPtr pLock;
    int winCID = CLIENT_ID(winID);

    pLock = &clientTable[winCID].lock;
    MTX_MUTEX_LOCK(&pLock->mutex);

    broadcast = UnlockResourceBits(read_lock, &pWin->drawable.lockBits);
    if (broadcast)
    {
	pLock->waiting = FALSE;
	X_COND_BROADCAST(&pLock->condition);
    }
    MTX_MUTEX_UNLOCK(&pLock->mutex);
}

/*--------------------------------------------------------------------
 *-------------------------------------------------------------------*/

void
UnlockTwoWindows (pWin1, pWin2, winID1, winID2)
WindowPtr pWin1;
WindowPtr pWin2;
XID winID1;
XID winID2;
{
    Bool broadcast;
    ResourceLockPtr pLock;
    int win1CID = CLIENT_ID(winID1);
    int win2CID = CLIENT_ID(winID2);

    pLock = &clientTable[win1CID].lock;
    MTX_MUTEX_LOCK(&pLock->mutex);

    broadcast = UnlockResourceBits(read_lock, &pWin1->drawable.lockBits);
    CHECK_NEXT_LOCK_WITH_BROADCAST(win1CID, win2CID, pLock, broadcast);

    broadcast |= UnlockResourceBits(read_lock, &pWin2->drawable.lockBits);
    if (broadcast)
    {
	pLock->waiting = FALSE;
	X_COND_BROADCAST(&pLock->condition);
    }

    MTX_MUTEX_UNLOCK(&pLock->mutex);
}

/*--------------------------------------------------------------------
 *-------------------------------------------------------------------*/

void
UnlockFont (pFont, fontID)
WindowPtr pFont;
XID fontID;
{
}

void
UnlockColormap (pMap, mapID)
ColormapPtr pMap;
XID mapID;
{
}

void
UnlockCursor (pCursor, cursorID)
CursorPtr pCursor;
XID cursorID;
{
}
#endif /* XTHREADS */
