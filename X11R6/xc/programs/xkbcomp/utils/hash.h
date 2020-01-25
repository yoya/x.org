#ifndef HASH_H
#define	HASH_H 1

#include <X11/Xfuncproto.h>

_XFUNCPROTOBEGIN

  /*\
   * $XConsortium: hash.h,v 1.2 94/04/04 15:31:20 rws Exp $
   *
   *		              COPYRIGHT 1990
   *		        DIGITAL EQUIPMENT CORPORATION
   *		           MAYNARD, MASSACHUSETTS
   *			    ALL RIGHTS RESERVED.
   *
   * THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE WITHOUT NOTICE AND
   * SHOULD NOT BE CONSTRUED AS A COMMITMENT BY DIGITAL EQUIPMENT CORPORATION.
   * DIGITAL MAKES NO REPRESENTATIONS ABOUT THE SUITABILITY OF THIS SOFTWARE 
   * FOR ANY PURPOSE.  IT IS SUPPLIED "AS IS" WITHOUT EXPRESS OR IMPLIED 
   * WARRANTY.
   *
   * IF THE SOFTWARE IS MODIFIED IN A MANNER CREATING DERIVATIVE COPYRIGHT
   * RIGHTS, APPROPRIATE LEGENDS MAY BE PLACED ON THE DERIVATIVE WORK IN
   * ADDITION TO THAT SET FORTH ABOVE.
   *
   * Permission to use, copy, modify, and distribute this software and its
   * documentation for any purpose and without fee is hereby granted, provided
   * that the above copyright notice appear in all copies and that both that
   * copyright notice and this permission notice appear in supporting
   * documentation, and that the name of Digital Equipment Corporation not be
   * used in advertising or publicity pertaining to distribution of the 
   * software without specific, written prior permission.
  \*/

#include "storage.h"

typedef	struct	_HashTableRec		*HashTablePtr;
#define	NullHashTable		((HashTablePtr)NULL)

typedef	Boolean	(*HashIterFunc)(
#if NeedFunctionPrototypes
    int 	/* row */,
    GenKey 	/* key */,
    GenData 	/* pData */,
    Opaque 	/* arg */ 
#endif
);

extern	HashTablePtr htCreate(
#if NeedFunctionPrototypes
    unsigned 		/* size */,
    GenKeyDescPtr 	/* keyType */,
    GenDataDescPtr 	/* dataType  */
#endif
);
extern void 
htDestroy(
#if NeedFunctionPrototypes
    HashTablePtr 	/* table */
#endif
);

extern	GenData	htAdd(
#if NeedFunctionPrototypes
    HashTablePtr 	/* table */,
    GenKey 		/* key */,
    GenData 		/* data  */
#endif
);
extern	GenData	htLookup(
#if NeedFunctionPrototypes
    HashTablePtr 	/* table */,
    GenKey 		/* key */
#endif
);
extern	GenData	htRemove(
#if NeedFunctionPrototypes
    HashTablePtr 	/* table */,
    GenKey 		/* key */
#endif
);

extern	int htSize(
#if NeedFunctionPrototypes
    HashTablePtr  	/* table */
#endif
);
extern	int htNumStored(
#if NeedFunctionPrototypes
    HashTablePtr 	/* table  */
#endif
);

extern	Boolean	htResize(
#if NeedFunctionPrototypes
    HashTablePtr 	/* table */,
    int 		/* newSize  */
#endif
);
extern	Boolean	htConvertKeys(
#if NeedFunctionPrototypes
    HashTablePtr 	/* table */,
    KeyConvertFunc 	/* cvtFunc */,
    GenKeyDescPtr	/* keyDesc */
#endif
);
extern	Boolean	htConvertData(
#if NeedFunctionPrototypes
    HashTablePtr	/* table */,
    DataConvertFunc 	/* cvtFunc */,
    GenDataDescPtr 	/* dataDesc */
#endif
);
extern	Boolean	htChangeHashFunc(
#if NeedFunctionPrototypes
    HashTablePtr 	/* table */,
    HashFunc 		/* newFunc */
#endif
);

extern	void htIterate(
#if NeedFunctionPrototypes
    HashTablePtr 	/* table */,
    HashIterFunc 	/* pFunc */,
    Opaque 		/* arg */
#endif
);

extern	Boolean	_htInitIter(
#if NeedFunctionPrototypes
    HashTablePtr 	/* table */,
    Opaque *		/* pOutState */
#endif
);
extern	Boolean	_htIterNext(
#if NeedFunctionPrototypes
    HashTablePtr 	/* table */,
    Opaque 		/* pState */,
    GenKey *		/* pkey */,
    GenData *		/* pData */
#endif
);
extern void htEndIterator(
#if NeedFunctionPrototypes
    HashTablePtr	/* table */,
    Opaque 		/* state */
#endif
);

#define	htIterator(t,k,d,s)	\
	if (_htInitIter((t),&(s))) \
	    while (_htIterNext((t),(Opaque)(s),(GenKey *)&(k),(GenData *)&(d))) 
_XFUNCPROTOEND

#endif /* HASH_H */
