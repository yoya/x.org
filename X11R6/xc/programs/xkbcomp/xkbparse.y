/* $XConsortium: xkbparse.y,v 1.3 94/04/08 15:30:22 erik Exp $ */
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
%token
	END_OF_FILE	0
	ERROR		255
	XKB_KEYMAP	1
	XKB_KEYCODES	2
	XKB_TYPES	3
	XKB_SYMBOLS	4
	XKB_COMPATMAP	5
	XKB_GEOMETRY	6
	XKB_SEMANTICS	7
	XKB_LAYOUT	8
	INCLUDE		10
	OVERRIDE	11
	AUGMENT		12
	REPLACE		13
	VIRTUAL_MODS	20
	TYPE		21
	INTERPRET	22
	ACTION		23
	KEY		24
	MODIFIER	25
	MODIFIER_MAP	26
	INDICATOR	27
	EQUALS		40
	PLUS		41
	MINUS		42
	DIVIDE		43
	TIMES		44
	OBRACE		45
	CBRACE		46
	OPAREN		47
	CPAREN		48
	OBRACKET	49
	CBRACKET	50
	DOT		51
	COMMA		52
	SEMI		53
	EXCLAM		54
	INVERT		55
	STRING		60
	NUMBER		61
	IDENT		62
	KEYNAME		63
%{
#ifdef DEBUG
#define	YYDEBUG 1
#endif
#define	DEBUG_VAR parseDebug
#include "xkbparse.h"
#include <X11/keysym.h>
%}
%right	EQUALS
%left	PLUS MINUS
%left	TIMES DIVIDE
%left	EXCLAM INVERT
%left	OPAREN
%start	XkbFile
%union	{
	int		 ival;
	unsigned	 uval;
	char		*str;
	StringToken	 sval;
	ParseCommon	*any;
	ExprDef		*expr;
	VarDef		*var;
	VModDef		*vmod;
	InterpDef	*interp;
	KeyTypeDef	*keyType;
	SymbolsDef	*syms;
	ModMapDef	*modMask;
	ModCompatDef	*modCompat;
	IndicatorMapDef	*ledMap;
	KeycodeDef	*keyName;
	XkbFile		*file;
}
%type <ival>	Number
%type <uval>	XkbCompositeType FileType MergeMode OptMergeMode KeySym
%type <str>	KeyName
%type <sval>	FieldSpec Ident Element String OptString
%type <any>	DeclList Decl 
%type <expr>	OptExprList ExprList Expr Term Lhs Terminal ArrayInit
%type <expr>	OptKeySymList KeySymList Action ActionList 
%type <var>	VarDecl VarDeclList SymbolsBody SymbolsVarDecl 
%type <vmod>	VModDecl VModDefList VModDef
%type <interp>	InterpretDecl InterpretMatch
%type <keyType>	KeyTypeDecl
%type <syms>	SymbolsDecl
%type <modMask>	ModMapDecl
%type <modCompat> ModCompatDecl
%type <ledMap>	IndicatorMapDecl
%type <keyName>	KeyNameDecl
%type <file>	XkbFile XkbMapConfigList XkbMapConfig XkbConfig
%%
XkbFile		:	XkbCompositeType OptString XkbMapConfigList
			{ 
			    rtrnValue=CreateXKBFile($1,$2,NULL);
			    $$= (XkbFile *)AppendStmt(&rtrnValue->common,
								&$3->common);
			}
		|	XkbConfig
			{ $$= rtrnValue= $1; }
		;

XkbCompositeType:	XKB_KEYMAP	{ $$= XkmKeymapFile; }
		|	XKB_SEMANTICS	{ $$= XkmSemanticsFile; }
		|	XKB_LAYOUT	{ $$= XkmLayoutFile; }
		;

XkbMapConfigList :	XkbMapConfigList XkbMapConfig
			{ $$= (XkbFile *)AppendStmt(&$1->common,&$2->common); }
		|	XkbMapConfig
			{ $$= $1; }
		;

XkbMapConfig	:	FileType OptString OBRACE
			    DeclList
			CBRACE SEMI
			{ $$= CreateXKBFile($1,$2,$4); }
		;

XkbConfig	:	FileType OptString DeclList
			{ $$= CreateXKBFile($1,$2,$3); }
		;


FileType	:	XKB_KEYCODES		{ $$= XkmKeyNamesIndex; }
		|	XKB_TYPES		{ $$= XkmTypesIndex; }
		|	XKB_COMPATMAP		{ $$= XkmCompatMapIndex; }
		|	XKB_SYMBOLS		{ $$= XkmSymbolsIndex; }
		|	XKB_GEOMETRY		{ $$= XkmGeometryIndex; }
		;

DeclList	:	DeclList Decl
			{ $$= AppendStmt($1,$2); }
		|	{ $$= NULL; }
		;

Decl		:	OptMergeMode VarDecl
			{
			    $2->merge= $1;
			    $$= &$2->common;
			}
		|	OptMergeMode VModDecl
			{
			    $2->merge= $1;
			    $$= &$2->common;
			}
		|	OptMergeMode InterpretDecl
			{
			    $2->merge= $1;
			    $$= &$2->common;
			}
		|	OptMergeMode KeyNameDecl
			{
			    $2->merge= $1;
			    $$= &$2->common;
			}
		|	OptMergeMode KeyTypeDecl
			{
			    $2->merge= $1;
			    $$= &$2->common;
			}
		|	OptMergeMode SymbolsDecl
			{
			    $2->merge= $1;
			    $$= &$2->common;
			}
		|	OptMergeMode ModMapDecl
			{
			    $2->merge= $1;
			    $$= &$2->common;
			}
		|	OptMergeMode ModCompatDecl
			{
			    $2->merge= $1;
			    $$= &$2->common;
			}
		|	OptMergeMode IndicatorMapDecl
			{
			    $2->merge= $1;
			    $$= &$2->common;
			}
		|	MergeMode String
			{
			    IncludeStmt *incl;
                            incl= uTypedAlloc(IncludeStmt);
                            if (incl) {
                                incl->common.stmtType= StmtInclude;
                                incl->common.next= NULL;
                                incl->merge= $1;
                                incl->file= scanStr;
                                scanStr= NULL;
                            }
                            $$= &incl->common;
                        }
		;

VarDecl		:	Lhs EQUALS Expr SEMI
			{ $$= VarCreate($1,$3); }
		|	Ident SEMI
			{ $$= BoolVarCreate($1,1); }
		|	EXCLAM Ident SEMI
			{ $$= BoolVarCreate($2,0); }
		;

KeyNameDecl	:	KeyName EQUALS Expr SEMI
                        {
			    KeycodeDef *def;

			    def= KeycodeCreate($1,$3);
			    if ($1)
				free($1);
			    $$= def;
			}
		;

VModDecl	:	VIRTUAL_MODS VModDefList SEMI
			{ $$= $2; }
		;

VModDefList	:	VModDefList COMMA VModDef
			{ $$= (VModDef *)AppendStmt(&$1->common,&$3->common); }
		|	VModDef
			{ $$= $1; }
		;

VModDef		:	Ident
			{ $$= VModCreate($1,NULL); }
		|	Ident EQUALS Expr
			{ $$= VModCreate($1,$3); }
		;

InterpretDecl	:	INTERPRET InterpretMatch OBRACE
			    VarDeclList
			CBRACE SEMI
			{
			    $2->def= $4;
			    $$= $2;
			}
		;

InterpretMatch	:	KeySym PLUS Expr	
			{ $$= InterpCreate((KeySym)$1,$3); }
		|	KeySym			
			{ $$= InterpCreate((KeySym)$1,NULL); }
		;

VarDeclList	:	VarDeclList VarDecl
			{ $$= (VarDef *)AppendStmt(&$1->common,&$2->common); }
		|	VarDecl
			{ $$= $1; }
		;

KeyTypeDecl	:	TYPE String OBRACE
			    VarDeclList
			CBRACE SEMI
			{ $$= KeyTypeCreate($2,$4); }
		;

SymbolsDecl	:	KEY KeyName OBRACE
			    SymbolsBody
			CBRACE SEMI
			{ $$= SymbolsCreate($2,$4); }
		;

SymbolsBody	:	SymbolsBody COMMA SymbolsVarDecl
			{ $$= (VarDef *)AppendStmt(&$1->common,&$3->common); }
		|	SymbolsVarDecl
			{ $$= $1; }
		;

SymbolsVarDecl	:	Lhs EQUALS Expr
			{ $$= VarCreate($1,$3); }
		|	Lhs EQUALS ArrayInit
			{ $$= VarCreate($1,$3); }
		|	Ident
			{ $$= BoolVarCreate($1,1); }
		|	EXCLAM Ident
			{ $$= BoolVarCreate($2,0); }
		|	ArrayInit
			{ $$= VarCreate(NULL,$1); }
		;

ArrayInit	:	OBRACKET OptKeySymList CBRACKET
			{ $$= $2; }
		|	OBRACKET ActionList CBRACKET
			{ $$= ExprCreateUnary(ExprActionList,TypeAction,$2); }
		;

ModCompatDecl	:	MODIFIER Ident OBRACE VarDeclList CBRACE SEMI
			{ $$= ModCompatCreate($2,$4); }
		;

ModMapDecl	:	MODIFIER_MAP Ident OBRACE ExprList CBRACE SEMI
			{ $$= ModMapCreate($2,$4); }
		;

IndicatorMapDecl:	INDICATOR String OBRACE VarDeclList CBRACE SEMI
			{ $$= IndicatorMapCreate($2,$4); }
		;

FieldSpec	:	Ident			{ $$= $1; }
		|	Element			{ $$= $1; }
		;

Element		:	ACTION			{ $$= stGetToken("action"); }
		|	INTERPRET		{ $$= stGetToken("interpret"); }
		|	TYPE			{ $$= stGetToken("type"); }
		|	KEY			{ $$= stGetToken("key"); }
		|	MODIFIER		{ $$= stGetToken("modifier"); }
		|	MODIFIER_MAP		{$$=stGetToken("modifier_map");}
		|	INDICATOR		{ $$= stGetToken("indicator"); }
		;

OptMergeMode	:	MergeMode		{ $$= $1; }
		|				{ $$= MergeDefault; }
		;

MergeMode	:	INCLUDE			{ $$= MergeDefault; }
		|	AUGMENT			{ $$= MergeAugment; }
		|	OVERRIDE		{ $$= MergeOverride; }
		|	REPLACE			{ $$= MergeReplace; }
		;

OptExprList	:	ExprList			{ $$= $1; }
		|				{ $$= NULL; }
		;

ExprList	:	ExprList COMMA Expr
			{ $$= (ExprDef *)AppendStmt(&$1->common,&$3->common); }
		|	Expr
			{ $$= $1; }
		;

		;

Expr		:	Expr DIVIDE Expr
			{ $$= ExprCreateBinary(OpDivide,TypeUnknown,$1,$3); }
		|	Expr PLUS Expr
			{ $$= ExprCreateBinary(OpAdd,TypeUnknown,$1,$3); }
		|	Expr MINUS Expr
			{ $$= ExprCreateBinary(OpSubtract,TypeUnknown,$1,$3); }
		|	Expr TIMES Expr
			{ $$= ExprCreateBinary(OpMultiply,TypeUnknown,$1,$3); }
		|	Lhs EQUALS Expr
			{ $$= ExprCreateBinary(OpAssign,TypeUnknown,$1,$3); }
		|	Term
			{ $$= $1; }
		;

Term		:	MINUS Term
			{ $$= ExprCreateUnary(OpNegate,TypeUnknown,$2); }
		|	PLUS Term
			{ $$= ExprCreateUnary(OpUnaryPlus,TypeUnknown,$2); }
		|	EXCLAM Term
			{ $$= ExprCreateUnary(OpNot,TypeUnknown,$2); }
		|	INVERT Term
			{ $$= ExprCreateUnary(OpInvert,TypeUnknown,$2); }
		|	Lhs
			{ $$= $1;  }
		|	Ident OPAREN OptExprList CPAREN %prec OPAREN
			{ $$= ActionCreate($1,$3); }
		|	Terminal
			{ $$= $1;  }
		|	OPAREN Expr CPAREN
			{ $$= $2;  }
		;

ActionList	:	ActionList COMMA Action
			{ $$= (ExprDef *)AppendStmt(&$1->common,&$3->common); }
		|	Action
			{ $$= $1; }
		;

Action		:	Ident OPAREN OptExprList CPAREN
			{ $$= ActionCreate($1,$3); }
		;

Lhs		:	FieldSpec
			{
			    ExprDef *expr;
                            expr= ExprCreate(ExprIdent,TypeUnknown);
                            expr->value.str= $1;
                            $$= expr;
			}
		|	FieldSpec DOT FieldSpec
                        {
                            ExprDef *expr;
                            expr= ExprCreate(ExprFieldRef,TypeUnknown);
                            expr->value.field.element= $1;
                            expr->value.field.field= $3;
                            $$= expr;
			}
		|	FieldSpec OBRACKET Expr CBRACKET
			{
			    ExprDef *expr;
			    expr= ExprCreate(ExprArrayRef,TypeUnknown);
			    expr->value.array.element= NullStringToken;
			    expr->value.array.field= $1;
			    expr->value.array.entry= $3;
			    $$= expr;
			}
		|	FieldSpec DOT FieldSpec OBRACKET Expr CBRACKET
			{
			    ExprDef *expr;
			    expr= ExprCreate(ExprArrayRef,TypeUnknown);
			    expr->value.array.element= $1;
			    expr->value.array.field= $3;
			    expr->value.array.entry= $5;
			    $$= expr;
			}
		;

Terminal	:	String
			{
			    ExprDef *expr;
                            expr= ExprCreate(ExprValue,TypeString);
                            expr->value.str= $1;
                            $$= expr;
			}
		|	Number
			{
			    ExprDef *expr;
                            expr= ExprCreate(ExprValue,TypeInt);
                            expr->value.ival= $1;
                            $$= expr;
			}
		|	KeyName
			{
			    ExprDef *expr;
			    expr= ExprCreate(ExprValue,TypeKeyName);
			    memcpy(expr->value.keyName,$1,4);
			    free($1);
			    $$= expr;
			}
		;

OptKeySymList	:	KeySymList			{ $$= $1; }
		|					{ $$= NULL; }
		;

KeySymList	:	KeySymList COMMA KeySym
			{ $$= AppendKeysymList($1,(KeySym)$3); }
		|	KeySym
			{ $$= CreateKeysymList((KeySym)$1); }
		;

KeySym		:	IDENT
			{ 
			    KeySym sym;
			    if (LookupKeysym(scanStr,&sym))
				$$= sym;
			    else {
				char buf[120];
				sprintf(buf,"expected keysym, got %s",
							uStringText(scanStr));
				yyerror(buf);
				yynerrs++;
				$$= NoSymbol;
			    }
			}
		|	Number		{ $$= $1; }
		;

Number		:	NUMBER		{ $$= scanInt; }
		;

KeyName		:	KEYNAME		{ $$= scanStr; scanStr= NULL; }
		;

Ident		:	IDENT		{ $$= stGetToken(scanStr); }
		;

OptString	:	String		{ $$= $1; }
		|			{ $$= NullStringToken; }
		;

String		:	STRING		{ $$= stGetToken(scanStr); }
		;
%%
static ExprDef *
ExprCreate(op,type)
    unsigned	op;
    unsigned	type;
{
ExprDef *expr;
    expr= uTypedAlloc(ExprDef);
    if (expr) {
	expr->common.stmtType= StmtExpr;
	expr->common.next= NULL;
	expr->op= op;
	expr->type= type;
    }
    else {
	uFatalError("Couldn't allocate expression in parser\n");
	/* NOTREACHED */
    }
    return expr;
}

static ExprDef *
ExprCreateUnary(op,type,child)
    unsigned	 op;
    unsigned	 type;
    ExprDef	*child;
{
ExprDef *expr;
    expr= uTypedAlloc(ExprDef);
    if (expr) {
	expr->common.stmtType= StmtExpr;
	expr->common.next= NULL;
	expr->op= op;
	expr->type= type;
	expr->value.child= child;
    }
    else {
	uFatalError("Couldn't allocate expression in parser\n");
	/* NOTREACHED */
    }
    return expr;
}

static ExprDef *
ExprCreateBinary(op,type,left,right)
    unsigned	 op;
    unsigned	 type;
    ExprDef	*left;
    ExprDef	*right;
{
ExprDef *expr;
    expr= uTypedAlloc(ExprDef);
    if (expr) {
	expr->common.stmtType= StmtExpr;
	expr->common.next= NULL;
	expr->op= op;
	expr->type= type;
	expr->value.binary.left= left;
	expr->value.binary.right= right;
    }
    else {
	uFatalError("Couldn't allocate expression in parser\n");
	/* NOTREACHED */
    }
    return expr;
}

static KeycodeDef *
KeycodeCreate(name,value)
    char *	name;
    ExprDef *	value;
{
KeycodeDef *def;

    def= uTypedAlloc(KeycodeDef);
    if (def) {
	def->common.stmtType= StmtKeycodeDef;
	def->common.next= NULL;
	strncpy(def->name,name,4);
	def->name[4]= '\0';
	def->value= value;
    }
    else {
	uFatalError("Couldn't allocate variable definition in parser\n");
	/* NOTREACHED */
    }
    return def;
}

static VModDef *
VModCreate(name,value)
    StringToken	name;
    ExprDef *	value;
{
VModDef *def;
    def= uTypedAlloc(VModDef);
    if (def) {
	def->common.stmtType= StmtVModDef;
	def->common.next= NULL;
	def->name= name;
	def->value= value;
    }
    else {
	uFatalError("Couldn't allocate variable definition in parser\n");
	/* NOTREACHED */
    }
    return def;
}

static VarDef *
VarCreate(name,value)
    ExprDef *	name;
    ExprDef *	value;
{
VarDef *def;
    def= uTypedAlloc(VarDef);
    if (def) {
	def->common.stmtType= StmtVarDef;
	def->common.next= NULL;
	def->name= name;
	def->value= value;
    }
    else {
	uFatalError("Couldn't allocate variable definition in parser\n");
	/* NOTREACHED */
    }
    return def;
}

static VarDef *
BoolVarCreate(nameToken,set)
    StringToken	nameToken;
    unsigned	set;
{
ExprDef	*name,*value;

    name= ExprCreate(ExprIdent,TypeUnknown);
    name->value.str= nameToken;
    value= ExprCreate(ExprValue,TypeBoolean);
    value->value.uval= set;
    return VarCreate(name,value);
}

static InterpDef *
InterpCreate(sym,match)
    KeySym	sym;
    ExprDef *	match;
{
InterpDef *def;

    def= uTypedAlloc(InterpDef);
    if (def) {
	def->common.stmtType= StmtInterpDef;
	def->common.next= NULL;
	def->sym= sym;
	def->match= match;
    }
    else {
	uFatalError("Couldn't allocate interp definition in parser\n");
	/* NOTREACHED */
    }
    return def;
}

static KeyTypeDef *
KeyTypeCreate(name,body)
    StringToken	name;
    VarDef *	body;
{
KeyTypeDef *def;

    def= uTypedAlloc(KeyTypeDef);
    if (def) {
	def->common.stmtType= StmtKeyTypeDef;
	def->common.next= NULL;
	def->merge= MergeDefault;
	def->name= name;
	def->body= body;
    }
    else {
	uFatalError("Couldn't allocate key type definition in parser\n");
	/* NOTREACHED */
    }
    return def;
}

static SymbolsDef *
SymbolsCreate(keyName,symbols)
    char *	keyName;
    ExprDef *	symbols;
{
SymbolsDef *def;

    def= uTypedAlloc(SymbolsDef);
    if (def) {
	def->common.stmtType= StmtSymbolsDef;
	def->common.next= NULL;
	def->merge= MergeDefault;
	bzero(def->keyName,5);
	strncpy(def->keyName,keyName,4);
	def->symbols= symbols;
    }
    else {
	uFatalError("Couldn't allocate symbols definition in parser\n");
	/* NOTREACHED */
    }
    return def;
}

static ModCompatDef *
ModCompatCreate(modifier,body)
    StringToken	modifier;
    VarDef *	body;
{
ModCompatDef *def;

    def= uTypedAlloc(ModCompatDef);
    if (def) {
	def->common.stmtType= StmtModCompatDef;
	def->common.next= NULL;
	def->merge= MergeDefault;
	def->modifier= modifier;
	def->def= body;
    }
    else {
	uFatalError("Couldn't allocate mod compat definition in parser\n");
	/* NOTREACHED */
    }
    return def;
}

static ModMapDef *
ModMapCreate(modifier,keys)
    StringToken	modifier;
    ExprDef *	keys;
{
ModMapDef *def;

    def= uTypedAlloc(ModMapDef);
    if (def) {
	def->common.stmtType= StmtModMapDef;
	def->common.next= NULL;
	def->merge= MergeDefault;
	def->modifier= modifier;
	def->keys= keys;
    }
    else {
	uFatalError("Couldn't allocate mod mask definition in parser\n");
	/* NOTREACHED */
    }
    return def;
}

static IndicatorMapDef *
IndicatorMapCreate(name,body)
    StringToken	name;
    VarDef *	body;
{
IndicatorMapDef *def;

    def= uTypedAlloc(IndicatorMapDef);
    if (def) {
	def->common.stmtType= StmtIndicatorMapDef;
	def->common.next= NULL;
	def->merge= MergeDefault;
	def->name= name;
	def->body= body;
    }
    else {
	uFatalError("Couldn't allocate mod mask definition in parser\n");
	/* NOTREACHED */
    }
    return def;
}

static ExprDef *
ActionCreate(name,args)
    StringToken	 name;
    ExprDef	*args;
{
ExprDef *act;

    act= uTypedAlloc(ExprDef);
    if (act) {
	act->common.stmtType= StmtExpr;
	act->common.next= NULL;
	act->op= ExprActionDecl;
	act->value.action.name= name;
        act->value.action.args= args;
	return act;
    }
    uFatalError("Couldn't allocate ActionDef in parser\n");
    return NULL;
}

static ExprDef *
CreateKeysymList(sym)
    KeySym	sym;
{
ExprDef	 *def;

    def= ExprCreate(ExprKeysymList,TypeSymbols);
    if (def) {
	def->value.list.nSyms= 1;
	def->value.list.szSyms= 2;
	def->value.list.syms= uTypedCalloc(2,KeySym);
	if (def->value.list.syms!=NULL) {
	    def->value.list.syms[0]= sym;
	    return def;
	}
    }
    uFatalError("Couldn't allocate expression for keysym list in parser\n");
    return NULL;
}

static ExprDef *
AppendKeysymList(list,sym)
    ExprDef *	list;
    KeySym	sym;
{
    if (list->value.list.nSyms>=list->value.list.szSyms) {
	list->value.list.szSyms*=2;
	list->value.list.syms= uTypedRealloc(list->value.list.syms,
						list->value.list.szSyms,
						KeySym);
	if (list->value.list.syms==NULL) {
	    uFatalError("Couldn't resize list of symbols for append\n");
	    return NULL;
	}
    }
    list->value.list.syms[list->value.list.nSyms++]= sym;
    return list;
}

static ParseCommon *
AppendStmt(to,append)
    ParseCommon *	to;
    ParseCommon *	append;
{
ParseCommon	*start= to;

    while ((to!=NULL) && (to->next!=NULL)) {
	to= to->next;
    }
    if (to) {
	to->next= append;
	return start;
    }
    return append;
}

static int
LookupKeysym(str,sym_rtrn)
    char	*str;
    KeySym	*sym_rtrn;
{
KeySym sym;

    if ((!str)||(uStrCaseCmp(str,"any")==0)||(uStrCaseCmp(str,"nosymbol")==0)) {
	*sym_rtrn= NoSymbol;
	return 1;
    }
    else if ((uStrCaseCmp(str,"none")==0)||(uStrCaseCmp(str,"voidsymbol")==0)) {
	*sym_rtrn= XK_VoidSymbol;
	return 1;
    }
    sym= XStringToKeysym(str);
    if (sym!=NoSymbol) {
	*sym_rtrn= sym;
	return 1;
    }
    return 0;
}

#ifdef DEBUG
void
PrintStmtAddrs(stmt)
    ParseCommon *	stmt;
{
    fprintf(stderr,"0x%x",stmt);
    if (stmt) {
	do {
	    fprintf(stderr,"->0x%x",stmt->next);
	    stmt= stmt->next;
	} while (stmt);
    }
    fprintf(stderr,"\n");
}
#endif

void
yyerror(s)
char	*s;
{
    (void)fprintf(stderr,"%s: line %d of %s\n",s,lineNum,
					(scanFile?scanFile:"(unknown)"));
    if (scanStr)
	(void)fprintf(stderr,"last scanned symbol is: %s\n",scanStr);
    return;
}


int
yywrap()
{
   return(1);
}

int
XKBParseFile(file,pRtrn)
FILE 		 *file;
XkbFile		**pRtrn;
{
    if (file) {
	extern FILE *yyin;
	yyin= file;
	rtrnValue= NULL;
	if (yyparse()==0) {
	    *pRtrn= rtrnValue;
	    return 1;
	}
	*pRtrn= NULL;
	return 0;
    }
    *pRtrn= NULL;
    return 1;
}

XkbFile *
CreateXKBFile(type,name,defs)
int		type;
StringToken	name;
ParseCommon *	defs;
{
XkbFile *file;
static int	fileID;

    file= uTypedAlloc(XkbFile);
    if (file) {
	file->type= type;
	file->name= name;
	file->defs= defs;
	file->id= fileID++;
    }
    return file;
}
