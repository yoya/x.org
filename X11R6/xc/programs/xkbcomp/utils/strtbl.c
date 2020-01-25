
  /*\
   * $XConsortium: strtbl.c,v 1.2 94/04/04 15:31:35 rws Exp $
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

#define	DEBUG_VAR	strtblDebug
#include "utils.h"
#include "storagestr.h"
#include "strtblstr.h"
#include "hash.h"

/***====================================================================***/

static	Boolean		 stInitialized;

static	Boolean		 stHavePrivate;
static	Opaque		*stPrivate;
static	Opaque		 stPrivDefault;

static	int		 stNextFree;
static	int		 stSize;

static	StringTableEntry	*stringTable;
static	HashTablePtr	 	 stringLookup;

static	GenKeyDescRec	stStringKeyTypeRec=	{
	(HashFunc)genHashString,	
	(CompareKeyFunc)genCompareStringKey,
	(StoreKeyFunc)NULL,
	(RemoveKeyFunc)NULL,
	(GenKey)NULL
};

static	GenDataDescRec	stTokenDataTypeRec=	{
	(StoreDataFunc)NULL,
	(ReplaceDataFunc)NULL,
	(RemoveDataFunc)NULL,
	(GenData)NullStringToken
};

/***====================================================================***/

char *
stText(token)
    StringToken token;
{
static char *nst= "<NullStringToken>";
static char *it=  "<IllegalToken>";

    if (token==NullStringToken) {
	return nst;
    }
    else if ((!stInitialized)||(stTokenIndex(token)>stSize)) {
	return it;
    }
    return stringTable[stTokenIndex(token)].string;
}

/***====================================================================***/

#if NeedFunctionPrototypes
Boolean
stInit(
    unsigned	nStr,
    unsigned	avgSize,
    Boolean	needPriv,
    Opaque	privDflt)
#else
Boolean
stInit(nStr,avgSize,needPriv,privDflt)
    unsigned	nStr;
    unsigned	avgSize;
    Boolean	needPriv;
    Opaque	privDflt;
#endif
{
register int	i;

    uENTRY2("stInit(%d,%d)\n",nStr,avgSize);
    /* doesn't use avgSize yet */
    if (stInitialized)
	uRETURN(False);
    stSize=	nStr;
    stringTable=	uTypedCalloc(nStr,StringTableEntry);
    if (stringTable==NULL) 
	uRETURN(False);
    stringLookup=	htCreate((unsigned)(nStr*11)/10,&stStringKeyTypeRec,
					      &stTokenDataTypeRec);
    if (stringLookup==NULL) {
	uFree((Opaque)stringTable);
	uRETURN(False);
    }

    if (needPriv) {
	stPrivate=	(Opaque *)uTypedCalloc(nStr,Opaque);
	if (stPrivate==NULL) {
	    uFree((Opaque)stringTable);
	    htDestroy(stringLookup);
	    uRETURN(False);
	}
	stHavePrivate=	True;
	stPrivDefault=	privDflt;
	stPrivate[0]=	privDflt;
    }
    else {
	stHavePrivate=	False;
    }

    stringTable[0].string=	NULL;
    stNextFree=		1;
    for (i=1;i<nStr;i++) {
	stringTable[i].next=	i+1;
    }
    stringTable[nStr-1].next=	(unsigned)NullStringToken;
    stInitialized=	True;
    uRETURN(True);
}

/***====================================================================***/

void
stClose()
{
    uENTRY("stClose()\n");
    if (!stInitialized)
	uVOIDRETURN;
    uFree((Opaque)stringTable);
    htDestroy(stringLookup);
    if (stHavePrivate) {
	uFree((Opaque)stPrivate);
    }
    stSize=		0;
    stringTable=	NULL;
    stringLookup=	NULL;
    stNextFree=		(int)NullStringToken;
    stHavePrivate=	False;
    stPrivDefault=	0;
    stInitialized=	False;
    uVOIDRETURN;
}

/***====================================================================***/

StringToken
stAddString(str)
    char *str;
{
StringToken	token,old;

    uENTRY1("stAddString(%s)\n",str);
    if ((str==NULL)||(str[0]=='\0')) {
	uRETURN(NullStringToken);
    }

    if (stNextFree==(int)NullStringToken) {
	register int	i;
	stNextFree=	stSize;
	stSize*=	2;
	stringTable=	(StringTableEntry *)uRealloc((Opaque)stringTable,
				(unsigned)stSize*sizeof(StringTableEntry));
	if (!stringTable) {
	    uFatalError("catastrophic failure! stringTable lost!\n");
	    /* NOTREACHED */
	}
	if (!htResize(stringLookup,(stSize*11)/10)) {
	    uFatalError("catastrophic failure! stringLookup lost!\n");
	    /* NOTREACHED */
	}
	if (stHavePrivate) {
	    stPrivate=	(Opaque *)uRealloc((Opaque)stPrivate,
					   (unsigned)stSize*sizeof(Opaque));
	    if (stPrivate==NULL) {
		uFatalError("catastrophic failure! stPrivate lost!\n");
		/* NOTREACHED */
	    }
	}
	for (i=stNextFree;i<stSize;i++) {
	    stringTable[i].next=	i+1;
	}
	stringTable[stSize-1].next=	(int)NullStringToken;
    }
    token=	stCreateToken(str[0],stNextFree);
    stNextFree=	stringTable[stNextFree].next;
    str=	uStringDup(str);
    stringTable[stTokenIndex(token)].string=	str;
    if (stHavePrivate) {
	stPrivate[stTokenIndex(token)]=	stPrivDefault;
    }
    old=	(StringToken)htAdd(stringLookup,(GenKey)str,(GenData)token);
    if (old!=NullStringToken)
	uWarning("string \"%s\" stored twice!\n",str);
    uRETURN(token);
}

/***====================================================================***/

StringToken
stGetToken(str)
    char *str;
{
StringToken	token;

    uENTRY1("stGetToken(%s)\n",str);
    if (stInitialized) {
	token=	(StringToken)htLookup(stringLookup,(GenKey)str);
	if (token==NullStringToken) {
	    token=	stAddString(str);
	}
    }
    uRETURN(token);
}

/***====================================================================***/

Boolean	
stGetTokenIfExists(str,pToken)
    char *	str;
    StringToken *pToken;
{
StringToken	token= NullStringToken;

    uENTRY2("stGetTokenIfExists(%s,0x%x)\n",str,pToken);
    if (stInitialized) {
	token=	(StringToken)htLookup(stringLookup,(GenKey)str);
    }
    if (pToken)
	*pToken= token;
    if (token==NullStringToken)
	uRETURN(False);
    uRETURN(True);
}

/***====================================================================***/

char *
stGetString(token)
    StringToken token;
{
    uENTRY1("stGetString(0x%x)\n",token);
    if ((!stInitialized)||(token==NullStringToken)||
	(stTokenIndex(token)>stSize)) {
	uRETURN(NULL);
    }
    /* some kind of consistency check here? */
    uRETURN(stringTable[stTokenIndex(token)].string);
}

/***====================================================================***/

Boolean
stRemove(token)
    StringToken token;
{
StringToken	htToken= NullStringToken;

    uENTRY1("stRemove(0x%x)\n",token);
    if (stInitialized) {
	htToken= (StringToken)htRemove(stringLookup,
			(GenData)stringTable[stTokenIndex(token)].string);
	if (htToken==token) {
	    uFree((Opaque)stringTable[stTokenIndex(token)].string);
	    stringTable[stTokenIndex(token)].next=	stNextFree;
	    stNextFree=	(int)token;
	    uRETURN(True);
	}
	else if (htToken!=NullStringToken)
	    uWarning(" htRemove returned 0x%x, expected 0x%x\n",
							htToken,token);
    }
    uRETURN(False);
}

/***====================================================================***/

Boolean
stSetPrivate(token,value)
    StringToken	token;
    Opaque	value;
{
    uENTRY2("stSetPrivate(0x%x,0x%x)\n",token,value);
    if ((!stInitialized)||(!stHavePrivate)) {
	uRETURN(False);
    }
    stPrivate[stTokenIndex(token)]=	value;
    uRETURN(True);
}

/***====================================================================***/

Opaque
stGetPrivate(token)
    StringToken token;
{
    uENTRY1("stGetPrivate(0x%x)\n",token);
    if ((!stInitialized)||(!stHavePrivate)) {
	uRETURN(stPrivDefault);
    }
    uRETURN(stPrivate[stTokenIndex(token)]);
}

/***====================================================================***/

Comparison
_stCompare(token1,token2)
    StringToken token1;
    StringToken token2;
{
    uENTRY2("_stCompare(0x%x,0x%x)\n",token1,token2);
    if (!stInitialized)
	uRETURN(Equal);

    if (token1==NullStringToken) {
	if (token2==NullStringToken)	{ uRETURN(Equal); }
	else				{ uRETURN(Less); }
    }
    else if (token2==NullStringToken)	{ uRETURN(Greater); }

    if ((stTokenIndex(token1)>stSize)||(stTokenIndex(token2)>stSize)) {
	uRETURN(Equal);
    }
    /* some kind of consistency check here? */
    uRETURN(uStringCompare(stringTable[stTokenIndex(token1)].string,
			   stringTable[stTokenIndex(token2)].string));
}

Comparison
_stCaseCompare(token1,token2)
    StringToken token1;
    StringToken token2;
{
    uENTRY2("_stCaseCompare(0x%x,0x%x)\n",token1,token2);
    if (!stInitialized)
	uRETURN(Equal);

    if (token1==NullStringToken) {
	if (token2==NullStringToken)	{ uRETURN(Equal); }
	else				{ uRETURN(Less); }
    }
    else if (token2==NullStringToken)	{ uRETURN(Greater); }

    if ((stTokenIndex(token1)>stSize)||(stTokenIndex(token2)>stSize)) {
	uRETURN(Equal);
    }
    /* some kind of consistency check here? */
    uRETURN(uStrCaseCmp(stringTable[stTokenIndex(token1)].string,
			stringTable[stTokenIndex(token2)].string));
}

