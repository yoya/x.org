/* $XConsortium: xkbcomp.h,v 1.4 94/04/08 15:30:05 erik Exp $ */
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

#ifndef XKBCOMP_H
#define	XKBCOMP_H 1

#ifndef DEBUG_VAR
#define	DEBUG_VAR_NOT_LOCAL
#define	DEBUG_VAR debugFlags
#endif

#include <X11/Xlib.h>
#include <X11/XKBlib.h>

#include "utils.h"
#include "strtbl.h"

#include "XKM.h"

extern char *scanFile;

#define	TypeUnknown	0
#define	TypeBoolean	1
#define	TypeInt		2
#define	TypeString	3
#define	TypeAction	4
#define	TypeKeyName	5
#define	TypeSymbols	6

#define	StmtUnknown		0
#define	StmtInclude		1
#define	StmtKeycodeDef		2
#define	StmtExpr		3
#define	StmtVarDef		4
#define	StmtKeyTypeDef		5
#define	StmtInterpDef		6
#define	StmtVModDef		7
#define	StmtSymbolsDef		8
#define	StmtModMapDef		9
#define	StmtModCompatDef 	10
#define	StmtIndicatorMapDef	11

#define	FileSymInterp	100

typedef	struct _ParseCommon {
    unsigned		 stmtType;
    struct _ParseCommon	*next;
} ParseCommon;

#define	ExprValue	0
#define	ExprIdent	1
#define	ExprActionDecl	2
#define	ExprFieldRef	3
#define	ExprArrayRef	4
#define	ExprKeysymList	5
#define	ExprActionList	6

#define	OpAdd		20
#define	OpSubtract	21
#define	OpMultiply	22
#define	OpDivide	23
#define	OpAssign	24
#define	OpNot		25
#define	OpNegate	26
#define	OpInvert	27
#define	OpUnaryPlus	28

typedef	Bool	(*IdentLookupFunc)(/* priv, elem, field, type, val_rtrn */);

#define	MergeDefault	0
#define	MergeAugment	1
#define	MergeOverride	2
#define	MergeReplace	3

extern	unsigned	warningLevel;

typedef struct _IncludeStmt {
    ParseCommon		common;
    int			merge;
    char		*file;
} IncludeStmt;

typedef struct _Expr {
    ParseCommon		common;
    unsigned		op;
    unsigned		type;
    union {
	struct {
	    struct _Expr *left;
	    struct _Expr *right;
	} binary;
	struct {
	    StringToken	element;
	    StringToken field;
	} field;
	struct {
	    StringToken	element;
	    StringToken	field;
	    struct _Expr *entry;
	} array;
	struct {
	    StringToken	  name;
	    struct _Expr *args;
	} action;
	struct {
	    int		 nSyms;
	    int		 szSyms;
	    KeySym *	 syms;
	} list;
	struct _Expr	*child;
	StringToken	 str;
	unsigned	 uval;
	int		 ival;
	char	 	 keyName[5];
	Opaque		 ptr;
    } value;
} ExprDef;

typedef struct _VarDef {
    ParseCommon	common;
    unsigned	 merge;
    ExprDef 	*name;
    ExprDef	*value;
} VarDef;

typedef struct _VModDef {
    ParseCommon	 common;
    unsigned	 merge;
    StringToken	 name;
    ExprDef	*value;
} VModDef;

typedef struct _KeycodeDef {
    ParseCommon	common;
    unsigned	merge;
    char 	name[5];
    ExprDef	*value;
} KeycodeDef;

typedef struct _KeyTypeDef {
    ParseCommon	 common;
    unsigned	 merge;
    StringToken	 name;
    VarDef	*body;
} KeyTypeDef;

typedef struct _SymbolsDef {
    ParseCommon	 common;
    unsigned	 merge;
    char 	 keyName[5];
    ExprDef *	 symbols;
} SymbolsDef;

typedef struct _ModMapDef {
    ParseCommon	 common;
    unsigned	 merge;
    StringToken	 modifier;
    ExprDef *	 keys;
} ModMapDef;

typedef struct _ModCompatDef {
    ParseCommon	 common;
    unsigned	 merge;
    StringToken	 modifier;
    VarDef *	 def;
} ModCompatDef;

typedef struct _InterpDef {
    ParseCommon	 common;
    unsigned	 merge;
    KeySym	 sym;
    ExprDef	*match;
    VarDef	*def;
} InterpDef;

typedef struct _IndicatorMapDef {
    ParseCommon	 common;
    unsigned	 merge;
    StringToken	 name;
    VarDef *	 body;
} IndicatorMapDef;

typedef struct _XkbFile {
    ParseCommon	 common;
    int		 type;
    StringToken	 name;
    ParseCommon	*defs;
    int		 id;
} XkbFile;

#endif /* XKBCOMP_H */
