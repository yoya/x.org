#ifndef STORAGE_H
#define	STORAGE_H 1

#include <X11/Xfuncproto.h>

_XFUNCPROTOBEGIN

  /*\
   * $XConsortium: storage.h,v 1.2 94/04/04 15:31:30 rws Exp $
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

	/*\
	 * Declarations for generic storage structure keys and
	 * data.
	\*/

typedef	unsigned long		 GenKey;
typedef	unsigned long		 GenData;

typedef	int (*HashFunc)(
#if NeedFunctionPrototypes
    GenKey 	/* key */
#endif
);
typedef	GenKey (*KeyConvertFunc)(
#if NeedFunctionPrototypes
    GenKey 	/* oldKey */
#endif
);

typedef	GenData	(*DataConvertFunc)(
#if NeedFunctionPrototypes
    GenData	/* oldData */
#endif
);

typedef	Boolean	(*GenIterFunc)(
#if NeedFunctionPrototypes
    GenKey 	/* key */,
    GenData	/* data */,
    Opaque	/* arg */
#endif
);

typedef	struct	_GenKeyDescRec	*GenKeyDescPtr;
typedef	struct	_GenDataDescRec	*GenDataDescPtr;

#define	NullGenKeyDesc		((GenKeyDescPtr)NULL)
#define	NullGenDataDesc		((GenDataDescPtr)NULL)

extern	int		genHashString(
#if NeedFunctionPrototypes
    GenKey	/* key */
#endif
);
extern	GenKey		genReplaceStringData(
#if NeedFunctionPrototypes
    GenData 	/* oldData */
#endif
);
extern	GenData		genDupStringData(
#if NeedFunctionPrototypes
    GenData	/* data */
#endif
);
extern	Comparison	genCompareStringKey(
#if NeedFunctionPrototypes
    GenKey 	/* key1 */,
    GenKey 	/* key2 */
#endif
);

extern	GenKeyDescPtr	genStringKeyType;
extern	GenKeyDescPtr	genFreedStringKeyType;
extern	GenKeyDescPtr	genIntegerKeyType;
extern	GenKeyDescPtr	genXResourceKeyType;
extern	GenKeyDescPtr	genDefaultKeyType;

extern	GenDataDescPtr	genFreedStringDataType;
extern	GenDataDescPtr	genPointerDataType;
extern	GenDataDescPtr	genFreedPointerDataType;
extern	GenDataDescPtr	genIntegerDataType;
extern	GenDataDescPtr	genXResourceDataType;
extern	GenDataDescPtr	genDefaultDataType;

_XFUNCPROTOEND

#endif /* ifndef STORAGE_H */
