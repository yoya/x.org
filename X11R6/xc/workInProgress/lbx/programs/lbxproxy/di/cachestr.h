/* $XConsortium: cachestr.h,v 1.3 94/04/12 22:01:26 dpw Exp $ */
/*
 * Copyright 1990, 1991 Network Computing Devices;
 * Portions Copyright 1987 by Digital Equipment Corporation and the
 * Massachusetts Institute of Technology
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the names of M.I.T., Network Computing Devices,
 * or Digital not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  M.I.T. Network Computing Devices, or Digital
 * make no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * NETWORK COMPUTING DEVICES, DIGITAL AND MIT DISCLAIM ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS, IN NO EVENT SHALL NETWORK COMPUTING DEVICES, DIGITAL OR MIT BE
 * LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * $NCDId: @(#)cachestr.h,v 1.1 1993/12/01 23:46:40 lemke Exp $
 *
 */

#ifndef _CACHSTR_H_
#define _CACHSTR_H_
#include	"cache.h"

typedef struct _cache_entry {
    pointer     data;
    unsigned long timestamp;
    CacheID     id;
    unsigned long size;
    CacheFree   free_func;
    struct _cache_entry *next;
}           CacheEntryRec, *CacheEntryPtr;

typedef struct _cache {
    Cache       id;
    CacheEntryPtr *entries;
    int         elements;
    int         buckets;
    int         hashsize;
    CacheID     nextid;
    unsigned long maxsize;
    unsigned long cursize;
}           CacheRec;

#define	NullCacheEntryPtr	((CacheEntryPtr) 0)

#endif				/* _CACHSTR_H_ */
