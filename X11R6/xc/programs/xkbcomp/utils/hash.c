
  /*\
   * $XConsortium: hash.c,v 1.1 94/04/02 17:10:26 erik Exp $
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

#define	DEBUG_VAR	hashDebug

#include "utils.h"
#include "hashstr.h"

/***====================================================================***/

#define	_htHash(pt,k)	((pt)->hashFunc?(*(pt)->hashFunc)(k):(unsigned)(k))
#define	_htCompareKey(pt,k1,k2) (_genCompareKey((pt)->keyType,k1,k2))
#define	_htStoreKey(pt,k)	(_genStoreKey((pt)->keyType,k))
#define	_htRemoveKey(pt,k)	_genRemoveKey((pt)->keyType,k)
#define	_htNullKey(pt)		(_genNullKey((pt)->keyType))

#define	_htStoreData(pt,d)		(_genStoreData((pt)->dataType,d))
#define	_htReplaceData(pt,nd,pod)	(_genReplaceData((pt)->dataType,nd,pod))
#define	_htRemoveData(pt,d)		(_genRemoveData((pt)->dataType,d))
#define	_htNullData(pt)			(_genNullData((pt)->dataType))

/***====================================================================***/

HashTablePtr
htCreate(size,keyType,dataType)
    unsigned		size;
    GenKeyDescPtr 	keyType;
    GenDataDescPtr 	dataType;
{
HashTablePtr	pTbl;

    uENTRY3("htCreate(%d,0x%x,0x%x)\n",size,keyType,dataType);

    if (size<1)		uRETURN(NULL);
    if (keyType==NULL)	keyType=	genDefaultKeyType;
    if (dataType==NULL)	dataType=	genDefaultDataType;

    uASSERT("htCreate",((keyType!=NULL)&&(dataType!=NULL)))

    pTbl=	uTypedAlloc(HashTableRec);
    if (pTbl!=NULL) {
	pTbl->hashFunc=		keyType->dfltHash;
	pTbl->keyType=		keyType;
	pTbl->dataType=		dataType;

	pTbl->nStored=		0;
	pTbl->nBuckets=		size;
	pTbl->ppBuckets=	uTypedCalloc(size,HashBucketPtr);
	if (pTbl->ppBuckets==NULL) {
	    uFree((Opaque)pTbl);
	    uRETURN(NULL);
	}
    }
    uRETURN(pTbl);
}

/***====================================================================***/

void
htDestroy(pTbl)
    HashTablePtr pTbl;
{
int		i;
HashBucketPtr	pBkt,pNext;

    uENTRY1("htDestroy(0x%x)\n",pTbl);

    if ((pTbl==NULL)||(pTbl->ppBuckets==NULL))
	uVOIDRETURN;

    if (pTbl->nStored>0) {
	for (i=0;i<pTbl->nBuckets;i++) {
	    pBkt=	pTbl->ppBuckets[i];
	    pTbl->ppBuckets[i]=	NULL;
	    while (pBkt!=NULL) {
		pNext=	pBkt->next;
		_htRemoveKey(pTbl,pBkt->key);
		(void)_htRemoveData(pTbl,pBkt->data);
		pBkt->next=	NULL;
		uFree((Opaque)pBkt);
		pTbl->nStored--;
		pBkt=	pNext;
	    }
	}
    }
    uASSERT("htDestroy",pTbl->nStored==0);
    uFree((Opaque)pTbl->ppBuckets);
    pTbl->ppBuckets=	NULL;
    uFree((Opaque)pTbl);
    uVOIDRETURN;
}

/***====================================================================***/

GenData
htAdd(pTbl,key,data)
    HashTablePtr	pTbl;
    GenKey 		key;
    GenData 		data;
{
unsigned	index;
HashBucketPtr	pBkt;

    uENTRY3("htAdd(0x%x,0x%x,0x%x)\n",pTbl,key,data);

    if (pTbl==NULL) {
	uRETURN((GenData)NULL);
    }

    uASSERT("htAdd",pTbl->nBuckets>0);

    index=	_htHash(pTbl,key)%pTbl->nBuckets;

    pBkt=	pTbl->ppBuckets[index];

    while (pBkt!=NULL) {	/* search through bucket list for match */
	if (_htCompareKey(pTbl,key,pBkt->key)==Equal)	break;
	pBkt=	pBkt->next;
    }

    if (pBkt==NULL) {	/* no matching bucket, add */
	pBkt=			uTypedAlloc(HashBucketRec);
	if (pBkt==NULL) {	/* alloc failed */
	    uRETURN(pTbl->dataType->null);
	}
	pBkt->key=		_htStoreKey(pTbl,key);
	pBkt->data=		_htStoreData(pTbl,data);
	pBkt->next=		pTbl->ppBuckets[index];
	pTbl->ppBuckets[index]=	pBkt;
	pTbl->nStored++;
	uRETURN(_htNullData(pTbl));
    }
    else {	/* matching bucket, replace */
	GenData	oldData=	pBkt->data;

	pBkt->data=		_htReplaceData(pTbl,data,&oldData);
	uRETURN(oldData);
    }
}

/***====================================================================***/

GenData
htLookup(pTbl,key)
    HashTablePtr pTbl;
    GenKey	 key;
{
unsigned	index;
HashBucketPtr	pBkt;

    uENTRY2("htLookup(0x%x,0x%x)\n",pTbl,key);

    if (pTbl==NULL) {
	uRETURN((GenData)NULL);
    }

    uASSERT("htLookup",pTbl->nBuckets>0);

    index=	_htHash(pTbl,key)%pTbl->nBuckets;

    pBkt=	pTbl->ppBuckets[index];

    while (pBkt!=NULL) {	/* search through bucket list for match */
	if (_htCompareKey(pTbl,key,pBkt->key)==Equal)	break;
	pBkt=	pBkt->next;
    }
    if (pBkt!=NULL) {
	uRETURN(pBkt->data);
    }
    uRETURN(_htNullData(pTbl));
}

/***====================================================================***/

GenData
htRemove(pTbl,key)
    HashTablePtr	pTbl;
    GenKey 		key;
{
unsigned	index;
HashBucketPtr	pBkt,pLast;
GenData	oldData;

    uENTRY2("htRemove(0x%x,0x%x)\n",pTbl,key);
    if (pTbl==NULL)
	uRETURN((GenData)NULL);

    uASSERT("htRemove",pTbl->nBuckets>0);

    index=	_htHash(pTbl,key)%pTbl->nBuckets;

    pBkt=	pTbl->ppBuckets[index];

    pLast=	(HashBucketPtr)NULL;
    while (pBkt!=NULL) {	/* search through bucket list for match */
	if (_htCompareKey(pTbl,key,pBkt->key)==Equal)	break;
	pLast=	pBkt;
	pBkt=	pBkt->next;
    }
    if (pBkt!=NULL) {
	_htRemoveKey(pTbl,pBkt->key);
	oldData=	_htRemoveData(pTbl,pBkt->data);
	if (pLast)	pLast->next=		pBkt->next;
	else		pTbl->ppBuckets[index]=	pBkt->next;
	pBkt->next=	NULL;
	uFree((Opaque)pBkt);
	pTbl->nStored--;
	uRETURN(oldData);
    }
    uRETURN(_htNullData(pTbl));
}

/***====================================================================***/

int
htSize(pTbl)
    HashTablePtr pTbl;
{
    uENTRY1("htSize(0x%x)\n",pTbl);
    uRETURN(pTbl->nBuckets);
}

/***====================================================================***/

int
htNumStored(pTbl)
    HashTablePtr pTbl;
{
    uENTRY1("htNumStored(0x%x)\n",pTbl);
    uRETURN(pTbl->nStored);
}

/***====================================================================***/

Boolean
htResize(pTbl,newSize)
    HashTablePtr 	pTbl;
    int			newSize;
{
int		i,newI,nMoved;
HashBucketPtr	*ppNewBuckets;
HashBucketPtr	pBkt,pNext;

    uENTRY2("htResize(0x%x,%d)\n",pTbl,newSize);

    if ((pTbl==NULL)||(newSize<1))
	uRETURN(False);
    if (pTbl->nBuckets==newSize)
	uRETURN(True);

    uASSERT("htResize",pTbl->nBuckets>0);

    ppNewBuckets=	uTypedCalloc(newSize,HashBucketPtr);
    if (ppNewBuckets==NULL)
	uRETURN(False);

    for (nMoved=i=0;i<pTbl->nBuckets;i++) {
	pBkt=	pTbl->ppBuckets[i];
	while (pBkt) {
	    pNext=		pBkt->next;
	    newI=		_htHash(pTbl,pBkt->key)%newSize;
	    pBkt->next=		ppNewBuckets[newI];
	    ppNewBuckets[newI]=	pBkt;
	    pBkt=		pNext;
	    nMoved++;
	}
	pTbl->ppBuckets[i]=	NULL;
    }
    uASSERT("htResize",nMoved==pTbl->nStored);
    uFree((Opaque)pTbl->ppBuckets);
    pTbl->nBuckets=	newSize;
    pTbl->ppBuckets=	ppNewBuckets;
    uRETURN(True);
}

/***====================================================================***/

Boolean
htConvertKeys(pTbl,cvtFunc,newKeyType)
    HashTablePtr	pTbl;
    KeyConvertFunc	cvtFunc;
    GenKeyDescPtr	newKeyType;
{
int		i,newI,nChanged;
HashBucketPtr	*ppNewBuckets;
HashBucketPtr	pBkt,pNext;
GenKey		oldKey;

    uENTRY3("htConvertKeys(0x%x,0x%x,0x%x)\n",pTbl,cvtFunc,newKeyType);
    if ((pTbl==NULL)||(cvtFunc==NULL))
	uRETURN(False);

    if (newKeyType==NULL)
	newKeyType=	genDefaultKeyType;

    if (pTbl->keyType==newKeyType)
	uRETURN(True);

    uASSERT("htConvertKeys",pTbl->nBuckets>0);

    ppNewBuckets=	uTypedCalloc(pTbl->nBuckets,HashBucketPtr);
    if (ppNewBuckets==NULL)
	uRETURN(False);

    pTbl->keyType=	newKeyType;
    for (nChanged=i=0;i<pTbl->nBuckets;i++) {
	pBkt=	pTbl->ppBuckets[i];
	while (pBkt) {
	    pNext=		pBkt->next;
	    oldKey=		pBkt->key;
	    pBkt->key=		(*cvtFunc)(oldKey);
	    newI=		_htHash(pTbl,pBkt->key)%pTbl->nBuckets;
	    pBkt->next=		ppNewBuckets[newI];
	    ppNewBuckets[newI]=	pBkt;
	    pBkt=		pNext;
	    nChanged++;
	}
	pTbl->ppBuckets[i]=	NULL;
    }
    uASSERT("htConvertKeys",nChanged==pTbl->nStored);
    uFree((Opaque)pTbl->ppBuckets);
    pTbl->ppBuckets=	ppNewBuckets;
    uRETURN(True);
}

/***====================================================================***/

Boolean
htConvertData(pTbl,cvtFunc,newDataType)
    HashTablePtr	pTbl;
    DataConvertFunc	cvtFunc;
    GenDataDescPtr	newDataType;
{
int		i,nChanged;
HashBucketPtr	pBkt;

    uENTRY3("htConvertData(0x%x,0x%x,0x%x)\n",pTbl,cvtFunc,newDataType);
    if ((pTbl==NULL)||(cvtFunc==NULL))
	uRETURN(False);

    if (newDataType==NULL)
	newDataType=	genDefaultDataType;

    if (pTbl->dataType==newDataType)
	uRETURN(True);

    uASSERT("htConvertData",pTbl->nBuckets>0);

    pTbl->dataType=	newDataType;
    for (nChanged=i=0;i<pTbl->nBuckets;i++) {
	pBkt=	pTbl->ppBuckets[i];
	while (pBkt!=NULL) {
	    pBkt->data=	(*cvtFunc)(pBkt->data);
	    pBkt=	pBkt->next;
	    nChanged++;
	}
    }
    uASSERT("htConvertData",nChanged==pTbl->nStored);
    uRETURN(True);
}

/***====================================================================***/

Boolean
htChangeHashFunc(pTbl,newHash)
    HashTablePtr	pTbl;
    HashFunc		newHash;
{
int		i,newI,nChanged;
HashBucketPtr	pBkt,pNext;
HashBucketPtr	*ppNewBuckets;

    uENTRY2("htChangeHashFunc(0x%x,0x%x)\n",pTbl,newHash);
    if (pTbl==NULL)
	uRETURN(False);

    if (newHash==NULL)
	newHash=	pTbl->keyType->dfltHash;

    if (pTbl->hashFunc==newHash)
	uRETURN(True);

    uASSERT("htChangeHashFunc",pTbl->nBuckets>0);

    ppNewBuckets=	uTypedCalloc(pTbl->nBuckets,HashBucketPtr);
    if (ppNewBuckets==NULL)
	uRETURN(False);

    pTbl->hashFunc=	newHash;
    for (nChanged=i=0;i<pTbl->nBuckets;i++) {
	pBkt=	pTbl->ppBuckets[i];
	while (pBkt) {
	    pNext=		pBkt->next;
	    newI=		_htHash(pTbl,pBkt->key)%pTbl->nBuckets;
	    pBkt->next=		ppNewBuckets[newI];
	    ppNewBuckets[newI]=	pBkt;
	    pBkt=		pNext;
	    nChanged++;
	}
    }
    uASSERT("htChangeHashFunc",nChanged==pTbl->nStored);
    uFree((Opaque)pTbl->ppBuckets);
    pTbl->ppBuckets=	ppNewBuckets;
    uRETURN(True);
}

/***====================================================================***/

void
htIterate(pTbl,pFunc,arg)
    HashTablePtr	pTbl;
    HashIterFunc	pFunc;
    Opaque 		arg;
{
HashBucketPtr	pBkt;
int		i,nSeen;

    uENTRY3("htIterate(0x%x,0x%x,0x%x)\n",pTbl,pFunc,arg);
    if ((!pTbl)||(!pFunc))
	uVOIDRETURN;

    for (nSeen=i=0;i<pTbl->nBuckets;i++) {
	pBkt=	pTbl->ppBuckets[i];
	while (pBkt) {
	    if (!(*pFunc)(i,pBkt->key,pBkt->data,arg)) {
		uVOIDRETURN;
	    }
	    pBkt= pBkt->next;
	    nSeen++;
	}
    }
    uASSERT("htIterate",nSeen==pTbl->nStored);
    uVOIDRETURN;
}

/***====================================================================***/

static struct htIterState {
	int		index;
	HashBucketPtr	pBkt;
} iState= { -1, NULL };

Boolean
_htInitIter(pTbl, pOutState)
HashTablePtr	  pTbl;
Opaque		 *pOutState;
{
struct	htIterState	*pState;

    uENTRY2("_htInitIter(0x%x,0x%x)\n",pTbl,pOutState);
    if ((pTbl!=NullHashTable)&&(pTbl->nStored>0)) {
	if (iState.index== -1)	pState=	&iState;
	else 			pState=	uTypedAlloc(struct htIterState);
	if (pState!=NULL) {
	    pState->index=	0;
	    pState->pBkt=	NULL;
	    *pOutState=		(Opaque)pState;
	    uRETURN(True);
	}
    }
    *pOutState=	(Opaque)NULL;
    uRETURN(False);
}

/***====================================================================***/

Boolean
_htIterNext(pTbl, pInState, pKey, pData)
HashTablePtr	  pTbl;
Opaque		  pInState;
GenKey 		 *pKey;
GenData		 *pData;
{
unsigned	i;
struct htIterState	*pState= (struct htIterState *)pInState;

    uENTRY4("_htIterNext(0x%x,0x%x,0x%x,0x%x)\n",pTbl,pState,pKey,pData);
    if ((pTbl!=NullHashTable)&&(pInState!=(Opaque)NULL)) {
	if (pState->pBkt!=NULL) {
	    pState->pBkt=	pState->pBkt->next;
	}
	if (pState->pBkt==NULL) {
	    for (i= pState->index;i<pTbl->nBuckets;i++) {
		if (pTbl->ppBuckets[i]!=NULL) {
		    pState->pBkt=	pTbl->ppBuckets[i];
		    pState->index=	i+1;
		    break;
		}
	    }
	}
	if (pState->pBkt!=NULL){
	    *pKey=		pState->pBkt->key;
	    *pData=		pState->pBkt->data;
	    uRETURN(True);
	}
    }
    uRETURN(False);
}

/***====================================================================***/

void
htEndIterator(pTbl,state)
    HashTablePtr	pTbl;
    Opaque 		state;
{
struct htIterState	*pState= (struct htIterState *)state;

    uENTRY2("htEndIterator(0x%x,%d)\n",pTbl,state);
    if (state!=(Opaque)NULL) {
	pState->index=	-1;
	if (pState!=&iState) {
	    (void)uFree((Opaque)pState);
	}
    }
    uVOIDRETURN;
}
