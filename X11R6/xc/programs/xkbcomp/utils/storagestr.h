#ifndef STORAGESTR_H
#define	STORAGESTR_H	1

#include <X11/Xfuncproto.h>

_XFUNCPROTOBEGIN

  /*\
   * $XConsortium: storagestr.h,v 1.2 94/04/04 15:31:33 rws Exp $
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

typedef	Comparison (*CompareKeyFunc)(
#if NeedFunctionPrototypes
    GenKey 	/* key1 */,
    GenKey 	/* key2 */
#endif
);

typedef	GenKey	(*StoreKeyFunc)(
#if NeedFunctionPrototypes
    GenKey 	/* key */
#endif
);
typedef	void (*RemoveKeyFunc)(
#if NeedFunctionPrototypes
    GenKey 	/* key */
#endif
);
typedef	GenData	(*StoreDataFunc)(
#if NeedFunctionPrototypes
    GenData	/* data */
#endif
);
typedef	GenData	(*ReplaceDataFunc)(
#if NeedFunctionPrototypes
    GenData	/* newData */,
    GenData *	/* oldData */
#endif
);
typedef	GenData	(*RemoveDataFunc)(
#if NeedFunctionPrototypes
    GenData 	/* data */
#endif
);
typedef	Boolean	(*StorageIterateFunc)(
#if NeedFunctionPrototypes
    GenKey	key,
    GenData	pData, 
    Opaque arg
#endif
);

typedef	struct	_GenKeyDescRec	{
	HashFunc	dfltHash;
	CompareKeyFunc	compare;
	StoreKeyFunc	store;
	RemoveKeyFunc	remove;
	GenKey		null;
} GenKeyDescRec;

typedef	struct	_GenDataDescRec {
	StoreDataFunc	store;
	ReplaceDataFunc	replace;
	RemoveDataFunc	remove;
	GenData		null;
} GenDataDescRec;

#define	_genCompareKey(pkt,k1,k2)  \
				((pkt)->compare?(*(pkt)->compare)((k1),(k2)):\
				    ((k1)>(k2)?Greater:((k1)<(k2)?Less:Equal)))
#define	_genStoreKey(pkt,k)	((pkt)->store?(*(pkt)->store)(k):(k))
#define	_genRemoveKey(pkt,k)	{ if ((pkt)->remove)	(*(pkt)->remove)(k); }
#define	_genNullKey(pkt)	((pkt)->null)

#define	_genStoreData(pdt,d)	((pdt)->store?(*(pdt)->store)(d):(d))
#define	_genReplaceData(pdt,nd,pod) \
				((pdt)->replace?(*(pdt)->replace)((nd),(pod)):\
						(nd))
#define	_genRemoveData(pdt,d)	((pdt)->remove?(*(pdt)->remove)((d)):(d))
#define	_genNullData(pdt)	((pdt)->null)

_XFUNCPROTOEND

#endif
