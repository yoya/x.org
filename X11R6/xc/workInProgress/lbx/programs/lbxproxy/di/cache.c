/* $XConsortium: cache.c,v 1.7 94/12/01 21:46:00 dpw Exp $ */
/*
Copyright (c) 1994  X Consortium

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

 * Copyright 1990, 1991 Network Computing Devices;
 * Portions Copyright 1987 by Digital Equipment Corporation 
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the names of Network Computing Devices,
 * or Digital not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  Network Computing Devices, or Digital
 * make no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * NETWORK COMPUTING DEVICES, AND DIGITAL DISCLAIM ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS, IN NO EVENT SHALL NETWORK COMPUTING DEVICES, OR DIGITAL BE
 * LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * $NCDId: @(#)cache.c,v 1.5 1994/03/24 17:54:50 lemke Exp $
 *
 */
#include	"cachestr.h"
#include        "assert.h"

#define INITBUCKETS 64
#define INITHASHSIZE 6
#define MAXHASHSIZE 11


#define	ENTRYOFFSET		22
#define CACHE_ENTRY_MASK	0x3FFFFF
#define	CACHE_ENTRY_BITS(id)	((id) & 0x1fc00000)
#define	CACHE_ID(id)		((int)(CACHE_ENTRY_BITS(id) >> ENTRYOFFSET))

#define	NullCacheEntry	((CacheEntryPtr) 0)

#define	MAX_NUM_CACHES	32
/* XXX make this dynamic? */
static CachePtr caches[MAX_NUM_CACHES];
static int  num_caches = 1;

/*-
 * Notes on cache implementation
 *
 * This is basically the X11 resource code, with some modifications
 * to handle aging.
 *
 * Its currently optimized for lookup & store.  Flushing old stuff
 * is a lot slower than it should probably be, but there's tradeoffs
 * in tuning.
 */

Cache
CacheInit(maxsize)
    unsigned long maxsize;
{
    Cache       id = (Cache) num_caches;
    CachePtr    cache;

    cache = (CachePtr) xalloc(sizeof(CacheRec));
    if (!cache)
	return (Cache) 0;
    cache->entries = (CacheEntryPtr *)
	xalloc(INITBUCKETS * sizeof(CacheEntryPtr));
    if (!cache->entries) {
	xfree(cache);
	return (Cache) 0;
    }
    bzero((char *) cache->entries, (INITBUCKETS * sizeof(CacheEntryPtr)));
    caches[id] = cache;
    cache->elements = 0;
    cache->buckets = INITBUCKETS;
    cache->hashsize = INITHASHSIZE;
    cache->maxsize = maxsize;
    cache->cursize = 0;
    cache->nextid = id << ENTRYOFFSET;
    cache->id = id;
    num_caches++;
    return id;
}

void
CacheFreeCache(cid)
    Cache       cid;
{
    CachePtr    cache = caches[cid];

    CacheReset(cid);
    caches[cid] = 0;
    xfree(cache->entries);
    xfree(cache);
}

static int
hash(cid, id)
    Cache       cid;
    CacheID     id;
{
    CachePtr    cache = caches[cid];

    switch (cache->hashsize) {

#ifdef DEBUG			/* only need this if INITHASHSIZE < 6 */
    case 2:
	return ((int) (0x03 & (id ^ (id >> 2) ^ (id >> 8))));
    case 3:
	return ((int) (0x07 & (id ^ (id >> 3) ^ (id >> 9))));
    case 4:
	return ((int) (0x0F & (id ^ (id >> 4) ^ (id >> 10))));
    case 5:
	return ((int) (0x01F & (id ^ (id >> 5) ^ (id >> 11))));
#endif

    case 6:
	return ((int) (0x03F & (id ^ (id >> 6) ^ (id >> 12))));
    case 7:
	return ((int) (0x07F & (id ^ (id >> 7) ^ (id >> 13))));
    case 8:
	return ((int) (0x0FF & (id ^ (id >> 8) ^ (id >> 16))));
    case 9:
	return ((int) (0x1FF & (id ^ (id >> 9))));
    case 10:
	return ((int) (0x3FF & (id ^ (id >> 10))));
    case 11:
	return ((int) (0x7FF & (id ^ (id >> 11))));
    }
    return -1;
}

static void
rebuild_cache(cache)
    CachePtr    cache;
{
    int         j;
    CacheEntryPtr cp,
                next,
              **tails,
               *entries,
              **tptr,
               *cptr;

    assert(cache);
    j = 2 * cache->buckets;
    tails = (CacheEntryPtr **) ALLOCATE_LOCAL(j * sizeof(CacheEntryPtr *));
    if (!tails)
	return;
    entries = (CacheEntryPtr *) xalloc(j * sizeof(CacheEntryPtr));
    if (entries) {
	DEALLOCATE_LOCAL(tails);
	return;
    }
    for (cptr = entries, tptr = tails; --j >= 0; cptr++, tptr++) {
	*cptr = NullCacheEntry;
	*tptr = cptr;
    }
    cache->hashsize++;
    for (j = cache->buckets, cptr = cache->entries; --j >= 0; cptr++) {
	for (cp = *cptr; cp; cp = next) {
	    next = cp->next;
	    cp->next = NullCacheEntry;
	    tptr = &tails[hash(cache->id, cp->id)];
	    **tptr = cp;
	    *tptr = &cp->next;
	}
    }
    DEALLOCATE_LOCAL(tails);
    cache->buckets *= 2;
    xfree(cache->entries);
    cache->entries = entries;
}

/*
 * throws out all existing entries
 */
void
CacheReset(cid)
    Cache       cid;
{
    CacheEntryPtr cp,
                ocp;
    CachePtr    cache;
    int         i;

    cache = caches[cid];
    if (!cache) {
	assert(0);
	return;
    }
    for (i = 0; i < cache->buckets; i++) {
	for (cp = cache->entries[i]; cp; cp = ocp) {
	    cache->elements--;
	    cache->cursize -= cp->size;
	    (*cp->free_func) (cp->id, cp->data, CacheWasReset);
	    ocp = cp->next;
	    xfree(cp);
	}
	cache->entries[i] = (CacheEntryPtr) 0;
    }
    assert(cache->cursize == 0);
}

void
CacheFreeAll()
{
    int         i;

    for (i = 1; i < num_caches; i++) {
	if (caches[i])
	    CacheFreeCache(caches[i]);
    }
    num_caches = 1;
}


static void
flush_cache(cache, needed)
    CachePtr    cache;
    unsigned long needed;
{
/* XXX -- try to set oldprev properly inside search loop */
    CacheEntryPtr cp,
                oldest,
               *oldprev;
    int         oldbucket,
                i;

    while ((cache->cursize + needed) > cache->maxsize) {
	oldest = (CacheEntryPtr) 0;
	/* find oldest */
	for (i = 0; i < cache->buckets; i++) {
	    cp = cache->entries[i];
	    if (!cp)
		continue;
	    if (!oldest) {
		oldbucket = i;
		oldest = cp;
	    }
	    while (cp) {
		if (cp->timestamp < oldest->timestamp) {
		    oldest = cp;
		    oldbucket = i;
		}
		cp = cp->next;
	    }
	}
	/* fixup list */
	oldprev = &cache->entries[oldbucket];
	cp = *oldprev;
	for (; cp = *oldprev; oldprev = &cp->next) {
	    if (cp == oldest) {
		*oldprev = oldest->next;
		break;
	    }
	}
	/* clobber it */
	cache->elements--;
	cache->cursize -= oldest->size;
	(*oldest->free_func) (oldest->id, oldest->data, CacheEntryOld);
	xfree(oldest);
    }
}

void
CacheResize(cid, newsize)
    Cache       cid;
{
    CachePtr    cache = caches[cid];

    if (!cache)
	return;

    if (newsize < cache->maxsize) {
	/* have to toss some stuff */
	flush_cache(cache, cache->maxsize - newsize);
    }
    cache->maxsize = newsize;
}

Bool
CacheStoreMemory(cid, id, data, size, free_func)
    Cache       cid;
    CacheID     id;
    pointer     data;
    unsigned long size;
    CacheFree   free_func;
{
    CacheEntryPtr cp,
               *head;
    CachePtr    cache = caches[cid];

    if (size > cache->maxsize)	/* beyond cache limits */
	return (CacheID) 0;

    if ((cache->elements >= 4 * cache->buckets) &&
	    (cache->hashsize < MAXHASHSIZE)) {
	rebuild_cache(cache);
    }
    if ((cache->cursize + size) > cache->maxsize) {
	flush_cache(cache, size);
    }
    head = &cache->entries[hash(cid, id)];
    cp = (CacheEntryPtr) xalloc(sizeof(CacheEntryRec));
    if (!cp) {
	return (CacheID) 0;
    }
    cp->next = *head;
    cp->id = id;
    cp->timestamp = GetTimeInMillis();
    cp->free_func = free_func;
    cp->size = size;
    cp->data = data;
    cache->cursize += size;
    cache->elements++;
    *head = cp;

    return id;
}

pointer
CacheFetchMemory(cid, id, update)
    Cache       cid;
    CacheID     id;
    Bool        update;
{
    CachePtr    cache = caches[cid];
    CacheEntryPtr cp,
               *head;

    head = &cache->entries[hash(cid, id)];
    for (cp = *head; cp; cp = cp->next) {
	if (cp->id == id) {
	    if (update) {
		cp->timestamp = GetTimeInMillis();

#ifdef bogus
/* XXX this gets the damn thing circular -- why is it even here? */
		if (cp != *head) {	/* put it in the front */
		    cp->next = *head;
		    *head = cp;
		}
#endif
	    }
	    return cp->data;
	}
    }
    return (pointer) 0;
}

int
CacheFreeMemory(cacheid, cid, notify)
    Cache       cacheid;
    CacheID     cid;
    Bool        notify;
{
    CachePtr    cache = caches[cacheid];
    CacheEntryPtr cp,
               *prev,
               *head;
    int        *elptr;
    int         elements;
    Bool        found = FALSE;

    head = &cache->entries[hash(cacheid, cid)];
    elptr = &cache->elements;
    prev = head;
    while ((cp = *prev) != NullCacheEntry) {
	if (cp->id == cid) {
	    *prev = cp->next;
	    elements = --*elptr;
	    if (notify) {
		(*(cp->free_func)) (cid, cp->data, CacheEntryFreed);
	    }
	    cache->cursize -= cp->size;
	    xfree(cp);
	    if (*elptr != elements)
		prev = head;
	    found = TRUE;
	} else {
	    prev = &cp->next;
	}
    }
    if (!found)
	FatalError("Freeing cache entry %d which isn't there\n", cid);
}

/* ARGSUSED */
void
CacheSimpleFree(cid, data, reason)
    CacheID     cid;
    pointer     data;
    int         reason;
{
    xfree(data);
}
