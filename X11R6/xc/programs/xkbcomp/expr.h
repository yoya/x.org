/* $XConsortium: expr.h,v 1.2 94/04/05 15:14:19 erik Exp $ */
/************************************************************
 Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.

 Permission to use, copy, modify, and distribute this
 software and its documentation for any purpose and without
 fee is hereby granted, provided that the above copyright
 notice appear in all copies and that both that copyright
 notice and this permission notice appear in supporting
 documentation, and that the name of Silicon Graphics not be 
 used in advertising or publicity pertaining to distribution 
 of the software without specific prior written permission.
 Silicon Graphics makes no representation about the suitability 
 of this software for any purpose. It is provided "as is"
 without any express or implied warranty.
 
 SILICON GRAPHICS DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS 
 SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY 
 AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL SILICON
 GRAPHICS BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL 
 DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, 
 DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE 
 OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION  WITH
 THE USE OR PERFORMANCE OF THIS SOFTWARE.

 ********************************************************/

#ifndef EXPR_H
#define EXPR_H 1

typedef union _ExprResult  {
    char *	str;
    int		ival;
    unsigned	uval;
} ExprResult;

extern	char *exprTypeText(
#if NeedFunctionProtoytypes
    unsigned 		/* type */
#endif
);

extern	int ExprResolveLhs(
#if NeedFunctionProtoytypes
    ExprDef *		/* expr */,
    ExprResult *	/* elem_rtrn */,
    ExprResult *	/* field_rtrn */,
    ExprDef **		/* index_rtrn */
#endif
);

typedef	struct _LookupPriv {
    XPointer 		priv;
    IdentLookupFunc	chain;
    XPointer 		chainPriv;
} LookupPriv;

typedef struct _LookupEntry {
    char *	name;
    unsigned	result;
} LookupEntry;

typedef struct _LookupTable {
    char *	 		element;
    LookupEntry	*		entries;
    struct _LookupTable *	nextElement;
} LookupTable;

extern int SimpleLookup(
#if NeedFunctionProtoytypes
    XPointer 		/* priv */,
    StringToken		/* elem */,
    StringToken		/* field */,
    unsigned		/* type */,
    ExprResult *	/* val_rtrn */
#endif
);

extern int TableLookup(
#if NeedFunctionProtoytypes
    XPointer 		/* priv */,
    StringToken		/* elem */,
    StringToken		/* field */,
    unsigned		/* type */,
    ExprResult *	/* val_rtrn */
#endif
);

extern int LookupModIndex(
#if NeedFunctionProtoytypes
    XPointer 		/* priv */,
    StringToken		/* elem */,
    StringToken		/* field */,
    unsigned		/* type */,
    ExprResult *	/* val_rtrn */
#endif
);

extern int LookupModMask(
#if NeedFunctionProtoytypes
    XPointer 		/* priv */,
    StringToken		/* elem */,
    StringToken		/* field */,
    unsigned		/* type */,
    ExprResult *	/* val_rtrn */
#endif
);

extern int ExprResolveModIndex(
#if NeedFunctionProtoytypes
    ExprDef *		/* expr */,
    ExprResult *	/* val_rtrn */,
    IdentLookupFunc	/* lookup */,
    XPointer		/* lookupPriv */
#endif
);

extern int ExprResolveModMask(
#if NeedFunctionProtoytypes
    ExprDef *		/* expr */,
    ExprResult *	/* val_rtrn */,
    IdentLookupFunc	/* lookup */,
    XPointer		/* priv */
#endif
);

extern int ExprResolveBoolean(
#if NeedFunctionProtoytypes
    ExprDef *		expr;
    ExprResult *	val_rtrn;
    IdentLookupFunc	lookup;
    XPointer		lookupPriv;
#endif
);

extern int ExprResolveInteger(
#if NeedFunctionProtoytypes
    ExprDef *		/* expr */,
    ExprResult *	/* val_rtrn */,
    IdentLookupFunc	/* lookup */,
    XPointer		/* lookupPriv */
#endif
);

extern int ExprResolveString(
#if NeedFunctionProtoytypes
    ExprDef *		/* expr */,
    ExprResult *	/* val_rtrn */,
    IdentLookupFunc	/* lookup */,
    XPointer		/* lookupPriv */
#endif
);

extern int ExprResolveEnum(
#if NeedFunctionProtoytypes
    ExprDef *		/* expr */,
    ExprResult *	/* val_rtrn */,
    LookupEntry	*	/* values */
#endif
);

extern int ExprResolveMask(
#if NeedFunctionProtoytypes
    ExprDef *		/* expr */,
    ExprResult *	/* val_rtrn */,
    IdentLookupFunc	/* lookup */,
    XPointer		/* lookupPriv */
#endif
);

extern int ExprResolveKeySym(
#if NeedFunctionProtoytypes
    ExprDef *		/* expr */,
    ExprResult *	/* val_rtrn */,
    IdentLookupFunc	/* lookup */,
    XPointer		/* lookupPriv */
#endif
);
#endif /* EXPR_H */
