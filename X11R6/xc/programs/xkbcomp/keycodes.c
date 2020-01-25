/* $XConsortium: keycodes.c,v 1.3 94/04/08 15:27:17 erik Exp $ */
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

#include "xkbcomp.h"
#include "xkbfile.h"
#include "tokens.h"
#include "expr.h"
#include "keycodes.h"

char *
keyNameText(name)
    char *name;
{
static char buf[5];

    memcpy(buf,name,4);
    buf[4]= '\0';
    return buf;
}

char *
longText(val)
    unsigned long val;
{
char buf[4];
    LongToKeyName(val,buf);
    return keyNameText(buf);
}

/***====================================================================***/

#define	HTBL_SIZE 257

typedef struct _KeyNameHash {
	char		codes[HTBL_SIZE];
	unsigned long	names[HTBL_SIZE];
} KeyNameHash;

#define	HashKeyName(n)	(((n[0]^n[2])<<8)|(n[1]^n[3]))

void
LongToKeyName(val,name)
    unsigned long 	val;
    char *		name;
{
    name[0]= ((val>>24)&0xff);
    name[1]= ((val>>16)&0xff);
    name[2]= ((val>>8)&0xff);
    name[3]= (val&0xff);
    return;
}

static int
HashLookup(tbl,name,ndxRtrn,kcRtrn)
    KeyNameHash *	tbl;
    char *		name;
    int *		ndxRtrn;
    int *		kcRtrn;
{
unsigned short hval,ndx;
unsigned long lval;
register unsigned i;

    hval= HashKeyName(name);
    lval= KeyNameToLong(name);
    ndx= hval/HTBL_SIZE;
    if (ndxRtrn)
	*ndxRtrn= ndx;
    for (i=ndx;i<HTBL_SIZE;i++) {
	if (tbl->codes[i]==0)
	    return 0;
	else if (tbl->names[i]==lval) {
	    if (ndxRtrn)
		*ndxRtrn= i;
	    if (kcRtrn)
		*kcRtrn= tbl->codes[i];
	    return 1;
	}
    }
    for (i=0;i<ndx;i++) {
	if (tbl->codes[i]==0)
	    return 0;
	else if (tbl->names[i]==lval) {
	    if (ndxRtrn)
		*ndxRtrn= i;
	    if (kcRtrn)
		*kcRtrn= tbl->codes[i];
	    return 1;
	}
    }

    return 0;
}

static int
HashAdd(tbl,name,kc,replace,kcRtrn)
    KeyNameHash *	tbl;
    char *		name;
    int			kc;
    int			replace;
    int *		kcRtrn;
{
unsigned short hval,ndx;
unsigned long lval;
register unsigned i;

    hval= HashKeyName(name);
    lval= KeyNameToLong(name);
    ndx= hval/HTBL_SIZE;
    for (i=ndx;i<HTBL_SIZE;i++) {
	if (tbl->codes[i]==0) {
	    tbl->names[i]= lval;
	    tbl->codes[i]= kc;
	    return i;
	}
	else if (tbl->names[i]==lval) {
	    if (kcRtrn)
		*kcRtrn= tbl->codes[i];
	    if (replace)
		tbl->codes[i]= kc;
	    return i;
	}
    }
    for (i=0;i<ndx;i++) {
	if (tbl->codes[i]==0) {
	    tbl->names[i]= lval;
	    tbl->codes[i]= kc;
	    return i;
	}
	else if (tbl->names[i]==lval) {
	    if (kcRtrn)
		*kcRtrn= tbl->codes[i];
	    if (replace)
		tbl->codes[i]= kc;
	    return i;
	}
    }
    uInternalError("Couldn't add \"<%s> = %d\" to table\n",name,kc);
    return -1;
}

static int
HashRemoveName(tbl,name,kcRtrn)
    KeyNameHash *	tbl;
    char *		name;
    int *		kcRtrn;
{
    return HashAdd(tbl,name,0,True,kcRtrn);
}

/***====================================================================***/

typedef struct _KeyNamesInfo {
    StringToken		name;
    int			errorCount;
    int			computedMin;
    int			computedMax;
    int			explicitMin;
    int			explicitMax;
    int			effectiveMin;
    int			effectiveMax;
    KeyNameHash		hash;
    unsigned long	names[256];
} KeyNamesInfo;

static void
ClearKeyNamesInfo(info)
    KeyNamesInfo *info;
{
    info->name= NullStringToken;
    info->computedMax= info->explicitMax= info->explicitMin= -1;
    info->computedMin= 256;
    info->effectiveMin= 8;
    info->effectiveMax= 255;
    bzero((char *)&info->hash,sizeof(info->hash));
    bzero((char *)info->names,sizeof(info->names));
    return;
}

static void
InitKeyNamesInfo(info)
    KeyNamesInfo *info;
{
    ClearKeyNamesInfo(info);
    info->errorCount= 0;
    return;
}

static Bool
AddKeyName(info,code,name,mergeMode,reportCollisions)
    KeyNamesInfo *	info;
    int			code;
    char *		name;
    unsigned		mergeMode;
    Bool		reportCollisions;
{
int	old,override;
unsigned long	lval;

    if ((code<info->effectiveMin)||(code>info->effectiveMax)) {
	uError("Illegal keycode %d for name <%s>\n",code,name);
	uAction("Must be in the range %d-%d inclusive\n",info->effectiveMin,
							 info->effectiveMax);
	return 0;
    }
    if (code<info->computedMin)	info->computedMin= code;
    if (code>info->computedMax)	info->computedMax= code;
    lval= KeyNameToLong(name);
    if (info->names[code]!=0) {
	char buf[6];

	LongToKeyName(info->names[code],buf);
	buf[4]= '\0';
	if (info->names[code]==lval) {
	    if (reportCollisions) {
		uWarning("Multiple identical key name defintions\n");
		uAction("Later occurences of \"<%s> = %d\" ignored\n",buf,code);
	    }
	    return 1;
	}
	if (mergeMode==MergeAugment) {
	    if (reportCollisions) {
		uError("Multiple names for keycode %d\n",code);
		uAction("Using <%s>, ignoring <%s>\n",buf,name);
	    }
	    return 0;
	}
	else {
	    int old;
	    if (reportCollisions) {
		uError("Multiple names for keycode %d\n",code);
		uAction("Using <%s>, ignoring <%s>\n",name,buf);
	    }
	    if (!HashRemoveName(&info->hash,buf,&old))
		return 0;
	    info->names[code]= 0;
	}
    }
    override= (mergeMode==MergeOverride);
    if (HashAdd(&info->hash,name,code,override,&old)<0)
	return 0;
    else if ((old!=0)&&(old!=code)) {
	if (override) {
	    info->names[code]= lval;
	    info->names[old]= 0;
	    if (reportCollisions) {
		uError("Key name <%s> assigned to multiple keys\n",name);
		uAction("Using %d, ignoring %d\n",code,old);
		return False;
	    }
	}
	else if (reportCollisions) {
	    uError("Key name <%s> assigned to multiple keys\n",name);
	    uAction("Using %d, ignoring %d\n",old,code);
	    return False;
	}
    }
    else info->names[code]= lval;
    return True;
}

/***====================================================================***/

static Bool
HandleIncludeKeycodes(stmt,xkb,info,hndlr)
    IncludeStmt	*	  stmt;
    XkbDescPtr		  xkb;
    KeyNamesInfo *	  info;
    void		(*hndlr)();
{
unsigned 	newMerge,tmp;
FILE	*	file;
XkbFile	*	rtrn;

    if (ProcessIncludeFile(stmt,XkmKeyNamesIndex,&rtrn,&newMerge)) {
	KeyNamesInfo 	myInfo;

	InitKeyNamesInfo(&myInfo);
	(*hndlr)(rtrn,xkb,MergeOverride,&myInfo);
	/* 3/14/94 (ef) -- XXX! should free rtrn and contents here */
	if (newMerge==MergeReplace) {
	    ClearKeyNamesInfo(info);
	    newMerge= MergeAugment;
	}
	info->errorCount+= myInfo.errorCount;
	if (myInfo.errorCount==0) {
	    register int i,nErrs;
	    char buf[5];
	    if (info->name==(Atom)NullStringToken)
		info->name= myInfo.name;
	    for (nErrs=0,i=myInfo.computedMin;i<=myInfo.computedMax;i++) {
		if (myInfo.names[i]==0)
		    continue;
		LongToKeyName(myInfo.names[i],buf);
		buf[4]= '\0';
		if (!AddKeyName(info,i,buf,newMerge,False))
		    nErrs++;
	    }
	    info->errorCount+= nErrs;
	}
	return (info->errorCount==0);
    }
    else {
	info->errorCount+= 10;
	return False;
    }
}

static int
HandleKeycodeDef(stmt,xkb,mergeMode,info)
    KeycodeDef *	stmt;
    XkbDescPtr		xkb;
    unsigned 		mergeMode;
    KeyNamesInfo *	info;
{
int		code;
ExprResult	result;

    if (!ExprResolveInteger(stmt->value,&result,NULL,NULL)) {
	uAction("No value keycode assigned to name <%s>\n",stmt->name);
	return 0;
    }
    code= result.ival;
    if ((code<info->effectiveMin)||(code>info->effectiveMax)) {
	uError("Illegal keycode %d for name <%s>\n",code,stmt->name);
	uAction("Must be in the range %d-%d inclusive\n",info->effectiveMin,
							 info->effectiveMax);
	return 0;
    }
    if (stmt->merge!=MergeDefault) {
	if (stmt->merge==MergeReplace)
	     mergeMode= MergeOverride;
	else mergeMode= stmt->merge;
    }
    return AddKeyName(info,code,stmt->name,mergeMode,True);
}

#define	MIN_KEYCODE_DEF		0
#define	MAX_KEYCODE_DEF		1

static int
HandleKeyNameVar(stmt,xkb,mergeMode,info)
    VarDef *		stmt;
    XkbDescPtr		xkb;
    unsigned 		mergeMode;
    KeyNamesInfo *	info;
{
ExprResult	tmp,field;
ExprDef	*	arrayNdx;
int		which;

    if (ExprResolveLhs(stmt->name,&tmp,&field,&arrayNdx)==0) 
	return 0; /* internal error, already reported */

    if (tmp.str!=NULL) {
	uError("Unknown element %s encountered\n",tmp.str);
	uAction("Default for field %s ignored\n",field.str);
	return 0;
    }
    if (uStrCaseCmp(field.str,"minimum")==0)	 which= MIN_KEYCODE_DEF;
    else if (uStrCaseCmp(field.str,"maximum")==0) which= MAX_KEYCODE_DEF;
    else {
	uError("Unknown field encountered\n");
	uAction("Assigment to field %s ignored\n",field.str);
	return 0;
    }
    if (arrayNdx!=NULL) {
	uError("The %s setting is not an array\n",field.str);
	uAction("Illegal array reference ignored\n");
	return 0;
    }

    if (ExprResolveInteger(stmt->value,&tmp,NULL,NULL)==0) {
	uAction("Assignment to field %s ignored\n",field.str);
	return 0;
    }
    if ((tmp.ival<8)||(tmp.ival>255)) {
	uError("Illegal keycode %d (must be in the range 8-255 inclusive)\n",
								tmp.ival);
	uAction("Value of \"%s\" not changed\n",field.str);
	return 0;
    }
    if (which==MIN_KEYCODE_DEF) {
	if ((info->explicitMax>0)&&(info->explicitMax<tmp.ival)) {
	    uError("Minimum key code (%d) must be <= maximum key code (%d)\n",
						tmp.ival,info->explicitMax);
	    uAction("Minimum key code value not changed\n");
	    return 0;
	}
	if ((info->computedMax>0)&&(info->computedMin<tmp.ival)) {
	    uError("Minimum key code (%d) must be <= lowest defined key (%d)\n",
						tmp.ival,info->computedMin);
	    uAction("Minimum key code value not changed\n");
	    return 0;
	}
	info->explicitMin= tmp.ival;
	info->effectiveMin= tmp.ival;
    }
    if (which==MAX_KEYCODE_DEF) {
	if ((info->explicitMin>0)&&(info->explicitMin>tmp.ival)) {
	    uError("Maximum code (%d) must be >= minimum key code (%d)\n",
						tmp.ival,info->explicitMin);
	    uAction("Maximum code value not changed\n");
	    return 0;
	}
	if ((info->computedMax>0)&&(info->computedMax>tmp.ival)) {
	    uError("Maximum code (%d) must be >= highest defined key (%d)\n",
						tmp.ival,info->computedMin);
	    uAction("Minimum code value not changed\n");
	    return 0;
	}
	info->explicitMax= tmp.ival;
	info->effectiveMax= tmp.ival;
    }
    return 1;
}

static void
HandleKeycodesFile(file,xkb,mergeMode,info)
    XkbFile		*file;
    XkbDescPtr	 	 xkb;
    unsigned		 mergeMode;
    KeyNamesInfo	*info;
{
ParseCommon	*stmt;

    info->name= file->name;
    stmt= file->defs;
    while (stmt) {
	switch (stmt->stmtType) {
	    case StmtInclude:
		if (!HandleIncludeKeycodes((IncludeStmt *)stmt,xkb,info,
						HandleKeycodesFile))
		    info->errorCount++;
		break;
	    case StmtKeycodeDef:
		if (!HandleKeycodeDef((KeycodeDef *)stmt,xkb,mergeMode,info))
		    info->errorCount++;
		break;
	    case StmtVarDef:
		if (!HandleKeyNameVar((VarDef *)stmt,xkb,mergeMode,info))
		    info->errorCount++;
		break;
	    case StmtInterpDef:
	    case StmtVModDef:
		uError("Keycode files may define key names only\n");
		uAction("Ignoring definition of %s\n",
				((stmt->stmtType==StmtInterpDef)?
					"a symbol interpretation":
					"virtual modifiers"));
		info->errorCount++;
		break;
	    default:
		uInternalError(
			"Unexpected statement type %d in HandleKeycodesFile\n",
			stmt->stmtType);
		break;
	}
	stmt= stmt->next;
	if (info->errorCount>10) {
	    uError("Too many errors\n");
	    uAction("Abandoning %s\n",stText(file->name));
	    break;
	}
    }
    return;
}

Bool
CompileKeycodes(file,result,mergeMode)
    XkbFile	*	file;
    XkbFileInfo *	result;
    unsigned	 	mergeMode;
{
int		errorCount= 0;
KeyNamesInfo	info;
XkbDescPtr	xkb;

    xkb= &result->xkb;
    InitKeyNamesInfo(&info);
    HandleKeycodesFile(file,xkb,mergeMode,&info);

    if (info.errorCount==0) {
	if (info.explicitMin>0)
	     xkb->min_key_code= info.effectiveMin;
	else xkb->min_key_code= info.computedMin;
	if (info.explicitMax>0)
	     xkb->max_key_code= info.effectiveMax;
	else xkb->max_key_code= info.computedMax;
	if (XkbAllocNames(xkb,XkbKeyNamesMask)) {
	    register int i;
	    xkb->names->keycodes= (Atom)info.name;
	    uDEBUG2(1,"key range: %d..%d\n",xkb->min_key_code,xkb->max_key_code);
	    for (i=info.computedMin;i<=info.computedMax;i++) {
		LongToKeyName(info.names[i],xkb->names->keys[i].name);
		uDEBUG2(2,"key %d = <%s>\n",i,
				keyNameText(xkb->names->keys[i].name));
	    }
	    return True;
	}
    }
    return False;
}
