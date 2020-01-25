
  /*\
   * $XConsortium: storage.c,v 1.2 94/04/04 15:31:27 rws Exp $
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

#define	DEBUG_VAR	genDebug
#include "utils.h"
#include "storagestr.h"

/***====================================================================***/

int
genHashString(inKey)
    GenKey	inKey;
{
char	*key= (char *)inKey;
int	total;

    uENTRY1("genHashString(%s)\n",key);

    total= *key++;
    if (*key)	total|= (((int)(*key++))<<16)&0x00ff0000;
    if (*key)	total|=	(((int)(*key++))<<8)&0x0000ff00;
    if (*key)	total|=	(((int)(*key++))<<24)&0xff000000;
    uRETURN(total);
}

GenData
genReplaceFreedStringData( newDataIn, ppOldDataIn )
    GenData	newDataIn;
    GenData *	ppOldDataIn;
{
char	 *newData=	(char *)newDataIn;
char	**ppOldData=	(char **)ppOldDataIn;

    uENTRY3("genReplaceStringData(%s,0x%x(%s))\n",newData,ppOldData,
							*ppOldData); 
    if ((ppOldData)&&(*ppOldData)) {
	uFree((Opaque)*ppOldData);
	*ppOldData=	NULL;
    }
    uRETURN((GenData)uStringDup(newData));
}

GenData
genReplaceFreedPointerData( newDataIn, ppOldDataIn )
    GenData 	newDataIn;
    GenData *	ppOldDataIn;
{
char	*newData=	(char *)newDataIn;
char	**ppOldData=	(char **)ppOldDataIn;

    uENTRY2("genReplacePointerData(%s,0x%x)\n",newData,ppOldData); 
    if ((ppOldData)&&(*ppOldData)) {
	uFree((Opaque)*ppOldData);
	*ppOldData=	NULL;
    }
    uRETURN((GenData)newData);
}

Comparison
genCompareStringKey( key1, key2 )
    GenKey	key1;
    GenKey	key2;
{
char	*str1=	(char *)key1;
char	*str2=	(char *)key2;
    uENTRY2("genCompareStringKey(%s,%s)\n",uStringText(str1),uStringText(str2));
    uRETURN(uStringCompare(str1,str2));
}

GenData
genDupStringData( data )
    GenData	data;
{
char	*str=	(char *)data;
    uENTRY1("genDupStringData(%s)\n",uStringText(str));
    str= (char *)uStringDup(str);
    uRETURN((GenData)str);
}

void
genFreeKey( key )
    GenKey	key;
{
    uENTRY1("genFreeKey(0x%x)\n",key);
    uFree((Opaque)key);
    uVOIDRETURN;
}

GenData
genFreeData( data )
    GenData	data;
{
    uENTRY1("genFreeData(0x%x)\n",data);
    uFree((Opaque)data);
    uRETURN(data);
}

/***====================================================================***/

static	GenKeyDescRec	genFreedStringKeyTypeRec= {
	(HashFunc)genHashString,	
	(CompareKeyFunc)genCompareStringKey,
	(StoreKeyFunc)genDupStringData,
	(RemoveKeyFunc)genFreeKey,
	(GenKey)NULL
};

static	GenKeyDescRec	genStringKeyTypeRec= {
	(HashFunc)genHashString,	
	(CompareKeyFunc)genCompareStringKey,
	(StoreKeyFunc)NULL,
	(RemoveKeyFunc)NULL,
	(GenKey)NULL
};

static	GenKeyDescRec	genIntegerKeyTypeRec=	{
	(HashFunc)NULL,	
	(CompareKeyFunc)NULL,
	(StoreKeyFunc)NULL,
	(RemoveKeyFunc)NULL,
	(GenKey)-1
};

static	GenKeyDescRec	genXResourceKeyTypeRec=	{
	(HashFunc)NULL,	
	(CompareKeyFunc)NULL,
	(StoreKeyFunc)NULL,
	(RemoveKeyFunc)NULL,
	(GenKey)0 /* should be None */
};

GenKeyDescPtr	genStringKeyType=	&genStringKeyTypeRec;
GenKeyDescPtr	genFreedStringKeyType=	&genFreedStringKeyTypeRec;
GenKeyDescPtr	genIntegerKeyType=	&genIntegerKeyTypeRec;
GenKeyDescPtr	genXResourceKeyType=	&genXResourceKeyTypeRec;
GenKeyDescPtr	genDefaultKeyType=	&genFreedStringKeyTypeRec;

static	GenDataDescRec genFreedStringDataTypeRec= {
	(StoreDataFunc)genDupStringData,
	(ReplaceDataFunc)genReplaceFreedStringData,
	(RemoveDataFunc)genFreeData,
	(GenData)NULL,
};

static	GenDataDescRec	genPointerDataTypeRec=	{
	(StoreDataFunc)NULL,
	(ReplaceDataFunc)NULL,
	(RemoveDataFunc)NULL,
	(GenData)NULL,
};

static	GenDataDescRec	genFreedPointerDataTypeRec=	{
	(StoreDataFunc)NULL,
	(ReplaceDataFunc)genReplaceFreedPointerData,
	(RemoveDataFunc)genFreeData,
	(GenData)NULL,
};


static	GenDataDescRec	genIntegerDataTypeRec=	{
	(StoreDataFunc)NULL,
	(ReplaceDataFunc)NULL,
	(RemoveDataFunc)NULL,
	(GenData)-1,
};

static	GenDataDescRec	genXResourceDataTypeRec=	{
	(StoreDataFunc)NULL,
	(ReplaceDataFunc)NULL,
	(RemoveDataFunc)NULL,
	(GenData)0, /* should be None */
};

GenDataDescPtr	genFreedStringDataType=	&genFreedStringDataTypeRec;
GenDataDescPtr	genPointerDataType=	&genPointerDataTypeRec;
GenDataDescPtr	genFreedPointerDataType=	&genFreedPointerDataTypeRec;
GenDataDescPtr	genIntegerDataType=	&genIntegerDataTypeRec;
GenDataDescPtr	genXResourceDataType=	&genXResourceDataTypeRec;
GenDataDescPtr	genDefaultDataType=	&genPointerDataTypeRec;

