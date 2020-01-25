/* $XConsortium: atomcache.c,v 1.5 94/03/27 13:30:22 dpw Exp $ */
/*
 * Copyright 1994 Network Computing Devices, Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software and
 * its documentation for any purpose is hereby granted without fee, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name Network Computing Devices, Inc. not be
 * used in advertising or publicity pertaining to distribution of this 
 * software without specific, written prior permission.
 * 
 * THIS SOFTWARE IS PROVIDED `AS-IS'.  NETWORK COMPUTING DEVICES, INC.,
 * DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING WITHOUT
 * LIMITATION ALL IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE, OR NONINFRINGEMENT.  IN NO EVENT SHALL NETWORK
 * COMPUTING DEVICES, INC., BE LIABLE FOR ANY DAMAGES WHATSOEVER, INCLUDING
 * SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES, INCLUDING LOSS OF USE, DATA,
 * OR PROFITS, EVEN IF ADVISED OF THE POSSIBILITY THEREOF, AND REGARDLESS OF
 * WHETHER IN AN ACTION IN CONTRACT, TORT OR NEGLIGENCE, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 * 
 * $NCDId: @(#)atomcache.c,v 1.7 1994/03/24 17:55:05 lemke Exp $
 */
/*
 * Original Author:  Keith Packard, MIT X Consortium
 *
 * LBX hacking:	 Dave Lemke, Networking Computing Devices, Inc
 */

/*
 * atom cache for LBX
 *
 * real close to standard atom code, but with a slight twist.
 * LBX doesn't want to invent the Atom associated with a string,
 * but insert it itself.  only LbxMakeAtom() ends up being modified.
 *
 * the end result is a (probably) sparse array of atoms.
 *
 * when a client does a GetAtomName(), we use NameForAtom()
 * - if it works, great, return the value.
 * - if its doesn't, send it on to the server, and when the answer
 * 	comes back, call LbxMakeAtom()
 *
 * when a client does InternAtom(), LbxMakeAtom() is called.
 * if its already there (or only-if-exists is true) we return
 * the appropriate value. if not, we send it on to the server,
 * and call LbxMakeAtom() with the answer.
 */


#include "os.h"
#include "atomcache.h"

typedef struct _AtomList {
    char       *name;
    int         len;
    int         hash;
    Atom        atom;
}           AtomListRec, *AtomListPtr;

static AtomListPtr *hashTable;

static int  hashSize,
            hashUsed;
static int  hashMask;
static int  rehash;

static AtomListPtr *reverseMap;
static int  reverseMapSize;
static Atom lastAtom;

static
Hash(string, len)
    char       *string;
{
    int         h;

    h = 0;
    while (len--)
	h = (h << 3) ^ *string++;
    if (h < 0)
	return -h;
    return h;
}

static
ResizeHashTable()
{
    int         newHashSize;
    int         newHashMask;
    AtomListPtr *newHashTable;
    int         i;
    int         h;
    int         newRehash;
    int         r;

    if (hashSize == 0)
	newHashSize = 1024;
    else
	newHashSize = hashSize * 2;
    newHashTable = (AtomListPtr *) xalloc(newHashSize * sizeof(AtomListPtr));
    if (!newHashTable)
	return FALSE;
    bzero((char *) newHashTable, newHashSize * sizeof(AtomListPtr));
    newHashMask = newHashSize - 1;
    newRehash = (newHashMask - 2);
    for (i = 0; i < hashSize; i++) {
	if (hashTable[i]) {
	    h = (hashTable[i]->hash) & newHashMask;
	    if (newHashTable[h]) {
		r = hashTable[i]->hash % newRehash | 1;
		do {
		    h += r;
		    if (h >= newHashSize)
			h -= newHashSize;
		} while (newHashTable[h]);
	    }
	    newHashTable[h] = hashTable[i];
	}
    }
    xfree(hashTable);
    hashTable = newHashTable;
    hashSize = newHashSize;
    hashMask = newHashMask;
    rehash = newRehash;
    return TRUE;
}

ResizeReverseMap()
{
    if (reverseMapSize == 0)
	reverseMapSize = 1000;
    else
	reverseMapSize *= 2;
    reverseMap = (AtomListPtr *) xrealloc(reverseMap, reverseMapSize * sizeof(AtomListPtr));
    bzero((char *)reverseMap, (reverseMapSize * sizeof(AtomListPtr)));
    if (!reverseMap)
	return FALSE;
    return TRUE;
}

static
NameEqual(a, b, l)
    char       *a,
               *b;
{
    while (l--)
	if (*a++ != *b++)
	    return FALSE;
    return TRUE;
}

Atom
LbxMakeAtom(string, len, atom, makeit)
    char       *string;
    Atom        atom;
    unsigned    len;
    int         makeit;
{
    AtomListPtr a;
    int         hash;
    int         h;
    int         r;

    hash = Hash(string, len);
    if (hashTable) {
	h = hash & hashMask;
	if (hashTable[h]) {
	    if (hashTable[h]->hash == hash && hashTable[h]->len == len &&
		    NameEqual(hashTable[h]->name, string, len)) {
		return hashTable[h]->atom;
	    }
	    r = (hash % rehash) | 1;
	    for (;;) {
		h += r;
		if (h >= hashSize)
		    h -= hashSize;
		if (!hashTable[h])
		    break;
		if (hashTable[h]->hash == hash && hashTable[h]->len == len &&
			NameEqual(hashTable[h]->name, string, len)) {
		    return hashTable[h]->atom;
		}
	    }
	}
    }
    if (!makeit)
	return None;
    a = (AtomListPtr) xalloc(sizeof(AtomListRec) + len + 1);
    a->name = (char *) (a + 1);
    a->len = len;
    strncpy(a->name, string, len);
    a->name[len] = '\0';
    a->atom = atom;
    if (atom > lastAtom)
	lastAtom = atom;
    a->hash = hash;
    if (hashUsed >= hashSize / 2) {
	ResizeHashTable();
	h = hash & hashMask;
	if (hashTable[h]) {
	    r = (hash % rehash) | 1;
	    do {
		h += r;
		if (h >= hashSize)
		    h -= hashSize;
	    } while (hashTable[h]);
	}
    }
    hashTable[h] = a;
    hashUsed++;
    if (reverseMapSize <= a->atom)
	ResizeReverseMap();
    reverseMap[a->atom] = a;
    return a->atom;
}

#ifdef undef
/* XXX this will lie */
ValidAtom(atom)
    Atom        atom;
{
    return (atom != None) && (atom <= lastAtom);
}
#endif

char       *
NameForAtom(atom)
    Atom        atom;
{
    if (atom != None && atom <= lastAtom && reverseMap[atom])
	return reverseMap[atom]->name;
    return 0;
}

void
FreeAtoms()
{
    int         i;

    if (reverseMap) {
	for (i = 0; i <= lastAtom; i++)
	    xfree(reverseMap[i]);
	xfree(reverseMap);
    }
    xfree(hashTable);
    reverseMapSize = 0;
    reverseMap = NULL;
    hashTable = NULL;
    lastAtom = 0;
    hashSize = 0;
    hashUsed = 0;
}
