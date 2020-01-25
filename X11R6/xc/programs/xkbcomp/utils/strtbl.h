#ifndef STRTBL_H
#define	STRTBL_H 1

#include <X11/Xfuncproto.h>

_XFUNCPROTOBEGIN

  /*\
   * $XConsortium: strtbl.h,v 1.2 94/04/04 15:31:38 rws Exp $
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

typedef	unsigned long StringToken;

#define	NullStringToken	((StringToken)0)

extern	char *stText(
#if NeedFunctionPrototypes
    StringToken 	/* token */
#endif
);
extern	Boolean stInit(
#if NeedFunctionPrototypes
    unsigned 	/* nStr */, 
    unsigned 	/* avgSize */, 
    Boolean 	/* needPriv */,
    Opaque 	/* privDflt */
#endif
);
extern	void stClose(
#if NeedFunctionPrototypes
    void
#endif
);

extern	StringToken stGetToken(
#if NeedFunctionPrototypes
    char *	/* str */
#endif
);
extern	Boolean	 stGetTokenIfExists(
#if NeedFunctionPrototypes
    char *		/* str */,
    StringToken *	/* pToken */
#endif
);

extern	char *stGetString(
#if NeedFunctionPrototypes
    StringToken 	/* token */
#endif
);
extern	Boolean	stRemove(
#if NeedFunctionPrototypes
    StringToken 	/* token */
#endif
);

extern	Opaque	stGetPriv(
#if NeedFunctionPrototypes
    StringToken 	/* token */
#endif
);
extern	Boolean	stSetPriv(
#if NeedFunctionPrototypes
    StringToken 	/* token */, 
    Opaque 		/* priv */
#endif
);

extern	Comparison _stCompare(
#if NeedFunctionPrototypes
    StringToken /* token1 */,
    StringToken /* token2 */
#endif
);
extern	Comparison _stCaseCompare(
#if NeedFunctionPrototypes
    StringToken /* token1 */,
    StringToken /* token2 */
#endif
);

#define	stEqual(t1,t2)	((t1)==(t2))
#define	stCompare(t1,t2) \
	((((unsigned)(t1)&0xff000000)>(((unsigned)(t2)&0xff000000)))?Greater:\
	 ((((unsigned)(t1)&0xff000000)<(((unsigned)(t2)&0xff000000)))?Less:\
	 ((t1)==(t2)?Equal:_stCompare(t1,t2))))
#define	stCaseCompare(t1,t2) \
	((t1)==(t2)?Equal:_stCaseCompare(t1,t2))

_XFUNCPROTOEND

#endif /* STRTBL_H */
