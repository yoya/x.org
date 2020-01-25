/* $XConsortium: symbols.c,v 1.4 94/04/08 15:29:04 erik Exp $ */
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

#include <X11/keysym.h>
#include <X11/Xutil.h>

#include "expr.h"
#include "vmod.h"
#include "action.h"
#include "keycodes.h"

extern StringToken	tok_ONE_LEVEL;
extern StringToken	tok_TWO_LEVEL;
extern StringToken	tok_KEYPAD;

/***====================================================================***/

#define	RepeatYes	1
#define	RepeatNo	0
#define	RepeatUndefined	~((unsigned)0)

typedef struct _KeyInfo {
    unsigned		fileID;
    unsigned long	name;
    unsigned char	symsDefined;
    unsigned char	actsDefined;
    short		groupWidth[XkbNumKbdGroups];
    KeySym *		syms[XkbNumKbdGroups];
    XkbAction *		acts[XkbNumKbdGroups];
    unsigned		repeat;
    XkbBehavior		behavior;
    StringToken		type;
} KeyInfo;

static void
InitKeyInfo(info)
    KeyInfo *	info;
{
register int i;
static char dflt[4]= "*";

    info->fileID= 0;
    info->name= KeyNameToLong(dflt);
    info->symsDefined= info->actsDefined= 0;
    for (i=0;i<XkbNumKbdGroups;i++) {
	info->groupWidth[i]= 0;
	info->syms[i]= NULL;
	info->acts[i]= NULL;
    }
    info->type= NullStringToken;
    info->behavior.type= XkbKB_Default;
    info->behavior.data= 0;
    info->repeat= RepeatUndefined;
    return;
}

static void
FreeKeyInfo(info)
    KeyInfo *	info;
{
register int i;

    info->fileID= 0;
    info->symsDefined= info->actsDefined= 0;
    for (i=0;i<XkbNumKbdGroups;i++) {
	info->groupWidth[i]= 0;
	if (info->syms[i]!=NULL)
	    uFree(info->syms[i]);
	info->syms[i]= NULL;
	if (info->acts[i]!=NULL)
	    uFree(info->acts[i]);
	info->acts[i]= NULL;
    }
    info->type=	 NullStringToken;
    info->behavior.type= XkbKB_Default;
    info->behavior.data= 0;
    info->repeat= RepeatUndefined;
    return;
}

static Bool
CopyKeyInfo(old,new,clearOld)
    KeyInfo *	old;
    KeyInfo *	new;
    Bool	clearOld;
{
register int i;

    *new= *old;
    if (clearOld) {
	for (i=0;i<XkbNumKbdGroups;i++) {
	    old->groupWidth[i]= 0;
	    old->syms[i]= NULL;
	    old->acts[i]= NULL;
	}
    }
    else {
	int width;
	for (i=0;i<XkbNumKbdGroups;i++) {
	    width= new->groupWidth[i];
	    if (old->syms[i]!=NULL) {
		new->syms[i]= uTypedCalloc(width,KeySym);
		if (!new->syms[i]) {
		    new->syms[i]= NULL;
		    new->groupWidth[i]= 0;
		    return False;
		}
		memcpy((char *)new->syms[i],(char *)old->syms[i],
						width*sizeof(KeySym));
	    }
	    if (old->acts[i]!=NULL) {
		new->acts[i]= uTypedCalloc(width,XkbAction);
		if (!new->acts[i]) {
		    new->acts[i]= NULL;
		    return False;
		}
		memcpy((char *)new->acts[i],(char *)old->acts[i],
						width*sizeof(XkbAction));
	    }
	}
    }
    return True;
}

/***====================================================================***/

typedef struct _ModMapEntry {
    Bool			haveSymbol;
    int				modifier;
    union {
	unsigned long		keyName;
	KeySym			keySym;
    } u;
    struct _ModMapEntry *	next;
} ModMapEntry;

#define	SYMBOLS_INIT_SIZE	110
#define	SYMBOLS_CHUNK		20
typedef struct _SymbolsInfo {
    StringToken			name;
    int				errorCount;
    unsigned			fileID;
    Bool			allowActions;
    unsigned			szKeys;
    unsigned			nKeys;
    KeyInfo *			keys;
    KeyInfo			dflt;
    VModInfo			vmods;
    ActionInfo *		action;

    ModMapEntry *		modMap;

    unsigned			parentMerge;
    struct _SymbolsInfo	*	parent;
} SymbolsInfo;

static void
InitSymbolsInfo(info,xkb)
    SymbolsInfo *	info;
    XkbDescPtr		xkb;
{
register int i;

    tok_ONE_LEVEL= stGetToken("ONE_LEVEL");
    tok_TWO_LEVEL= stGetToken("TWO_LEVEL");
    tok_KEYPAD= stGetToken("KEYPAD");
    info->name= NullStringToken;
    info->errorCount= 0;
    info->fileID= 0;
    info->allowActions= True;
    info->szKeys= SYMBOLS_INIT_SIZE;
    info->nKeys= 0;
    info->keys= uTypedCalloc(SYMBOLS_INIT_SIZE,KeyInfo);
    info->parent= NULL;
    info->parentMerge= MergeDefault;
    info->modMap= NULL;
    InitKeyInfo(&info->dflt);
    InitVModInfo(&info->vmods,xkb);
    info->action= NULL;
    return;
}

static void
ClearSymbolsInfo(info,xkb)
    SymbolsInfo *	info;
    XkbDescPtr		xkb;
{
register int i;
ModMapEntry *mm,*next;

    info->name= NullStringToken;
    info->fileID= 0;
    for (i=0;i<info->nKeys;i++) {
	FreeKeyInfo(&info->keys[i]);
    }
    for (mm=info->modMap;mm!=NULL;mm=next) {
	next= mm->next;
	uFree(mm);
    }
    info->nKeys= 0;
    FreeKeyInfo(&info->dflt);
    ClearVModInfo(&info->vmods,xkb);
    return;
}

static void
FreeSymbolsInfo(info)
    SymbolsInfo *	info;
{
register int	i;
ModMapEntry *	mm,*next;

    if (info->keys) {
	for (i=0;i<info->nKeys;i++) {
	    FreeKeyInfo(&info->keys[i]);
	}
	uFree(info->keys);
	info->keys= NULL;
    }
    for (mm=info->modMap;mm!=NULL;mm=next) {
	next= mm->next;
	uFree(mm);
    }
    bzero((char *)info,sizeof(SymbolsInfo));
    return;
}

static Bool
ResizeKeyGroup(key,group,newWidth,forceActions)
    KeyInfo *		key;
    unsigned		group;
    unsigned		newWidth;
    Bool		forceActions;
{
Bool	tooSmall;
    tooSmall= (key->groupWidth[group]<newWidth);
    if ((key->syms[group]==NULL)||tooSmall) {
	key->syms[group]= uTypedRecalloc(key->syms[group],
					 key->groupWidth[group],
					 newWidth,KeySym);
	if (!key->syms[group])
	    return False;
    }
    if (((forceActions)&&(tooSmall||(key->acts[group]==NULL)))||
				(tooSmall&&(key->acts[group]!=NULL))) {
	key->acts[group]= uTypedRecalloc(key->acts[group],
					     key->groupWidth[group],
					     newWidth,XkbAction);
	if (!key->acts[group])
	    return False;
    }
    key->groupWidth[group]= newWidth;
    return True;
}

static Bool
MergeKeyGroups(info,into,from,group,clobber,report)
    SymbolsInfo *	info;
    KeyInfo *		into;
    KeyInfo *		from;
    unsigned		group;
    unsigned		clobber;
    Bool		report;
{
KeySym	*	resultSyms;
XkbAction *	resultActs;
int		resultWidth;
register int	i;

    report= report&&(into->fileID==from->fileID);
    if (into->groupWidth[group]>=from->groupWidth[group]) {
	resultSyms= into->syms[group];
	resultActs= into->acts[group];
	resultWidth= into->groupWidth[group];
    }
    else {
	resultSyms= from->syms[group];
	resultActs= from->acts[group];
	resultWidth= from->groupWidth[group];
    }
    if (resultSyms==NULL) {
	resultSyms= uTypedCalloc(resultWidth,KeySym);
	if (!resultSyms) {
	    uInternalError("Couldn't allocate symbols for group merge\n");
	    uAction("Group %d of key %s not merged\n",group,
						longText(into->name));
	    return False;
	}
    }
    if ((resultActs==NULL)&&(into->acts[group]||from->acts[group])) {
	resultActs= uTypedCalloc(resultWidth,XkbAction);
	if (!resultActs) {
	    uInternalError("Couldn't allocate actions for group merge\n");
	    uAction("Group %d of key %s not merged\n",group,
						longText(into->name));
	    return False;
	}
    }
    for (i=0;i<resultWidth;i++) {
	KeySym fromSym,toSym;
	if (from->syms[group] && (i<from->groupWidth[group]))
	     fromSym= from->syms[group][i];
	else fromSym= NoSymbol;
	if (into->syms[group] && (i<into->groupWidth[group]))
	     toSym= into->syms[group][i];
	else toSym= NoSymbol;
	if ((fromSym==NoSymbol)||(fromSym==toSym))
	    resultSyms[i]= toSym;
	else if (toSym==NoSymbol)
	    resultSyms[i]= fromSym;
	else {
	    if ((report)&&(warningLevel>0)) {
		uWarning("Multiple symbols for level %d/group %d on key %s\n",
						i,group,longText(into->name));
		uAction("Using %s, ignoring %s\n",
			XkbKeysymText(clobber?fromSym:toSym,XkbMessage),
			XkbKeysymText(clobber?toSym:fromSym,XkbMessage));
	    }
	    if (clobber)	resultSyms[i]= fromSym;
	    else		resultSyms[i]= toSym;
	}
	if (resultActs!=NULL) {
	    XkbAction *fromAct,*toAct;
	    fromAct= (from->acts[group]?&from->acts[group][i]:NULL);
	    toAct= (into->acts[group]?&into->acts[group][i]:NULL);
	    if (((fromAct==NULL)||(fromAct->type==XkbSA_NoAction))&&
						(toAct!=NULL)) {
		resultActs[i]= *toAct;
	    }
	    else if (((toAct==NULL)||(toAct->type==XkbSA_NoAction))&&
						(fromAct!=NULL)) {
		resultActs[i]= *fromAct;
	    }
	    else {
		if ((report)&&(warningLevel>0)) {
		  uWarning("Multiple actions for level %d/group %d on key %s\n",
						  i,group,longText(into->name));
		  uAction("Using %s, ignoring %s\n",
			actionTypeText(clobber?fromAct->type:toAct->type),
			actionTypeText(clobber?toAct->type:fromAct->type));
		}
		if (clobber)	resultActs[i]= *fromAct;
		else		resultActs[i]= *toAct;
	    }
	}
    }
    if ((into->syms[group]!=NULL)&&(resultSyms!=into->syms[group]))
	uFree(into->syms[group]);
    if ((from->syms[group]!=NULL)&&(resultSyms!=from->syms[group]))
	uFree(from->syms[group]);
    into->groupWidth[group]= resultWidth;
    into->syms[group]= resultSyms;
    from->syms[group]= NULL;
    into->symsDefined|= (1<<group);
    from->symsDefined&= ~(1<<group);
    return True;
}

static Bool
MergeKeys(info,into,from,clobber,report)
    SymbolsInfo *	info;
    KeyInfo *		into;
    KeyInfo *		from;
    unsigned		clobber;
    Bool		report;
{
register int i;
    
    for (i=0;i<XkbNumKbdGroups;i++) {
	if (from->groupWidth[i]>0) {
	    if (into->groupWidth[i]==0) {
		into->groupWidth[i]= from->groupWidth[i];
		into->syms[i]= from->syms[i];
		into->acts[i]= from->acts[i];
		into->symsDefined|= (1<<i);
		from->syms[i]= NULL;
		from->acts[i]= NULL;
		from->groupWidth[i]= 0;
		from->symsDefined&= ~(1<<i);
	    }
	    else {
		MergeKeyGroups(info,into,from,i,clobber,report);
	    }
	}
    }
    if (clobber) {
	if (from->behavior.type!=XkbKB_Default)
	    into->behavior= from->behavior;
	if (from->repeat!=RepeatUndefined)
	    into->repeat= from->repeat;
	if (from->type!=NullStringToken)
	    into->type= from->type;
    }
    return True;
}

static Bool
AddKeySymbols(info,key,clobber,report)
    SymbolsInfo *	  info;
    KeyInfo *		  key;
    unsigned		  clobber;
    Bool		  report;
{
register int i,n;

    for (i=0;i<info->nKeys;i++) {
	if (info->keys[i].name==key->name)
	    return MergeKeys(info,&info->keys[i],key,clobber,report);
    }
    if (info->nKeys>=info->szKeys) {
	info->szKeys+= SYMBOLS_CHUNK;
	info->keys= uTypedRealloc(info->keys,info->szKeys,KeyInfo);
	if (!info->keys) {
	    uInternalError("Couldn't allocate key symbols descriptions\n");
	    uAction("Some key symbols definitions may be lost\n");
	    return False;
	}
    }
    return CopyKeyInfo(key,&info->keys[info->nKeys++],True);
}

static Bool
AddModMapEntry(info,new,clobber)
    SymbolsInfo *	info;
    ModMapEntry *	new;
    Bool		clobber;
{
ModMapEntry *	mm;

    for (mm=info->modMap;mm!=NULL;mm= mm->next) {
	if (new->haveSymbol&&mm->haveSymbol&&(new->u.keySym==mm->u.keySym)) {
	    unsigned	use,ignore;
	    if (mm->modifier!=new->modifier) {
		if (clobber) {
		    use= new->modifier;
		    ignore= mm->modifier;
		}
		else {
		    use= mm->modifier;
		    ignore= new->modifier;
		}
		uError("Symbol %s added to map for multiple modifiers\n",
				XkbKeysymText(new->u.keySym,XkbMessage));
		uAction("Using %s, ",XkbModIndexText(use,XkbMessage));
		uInformation("ignoring %s\n",
					XkbModIndexText(ignore,XkbMessage));
		mm->modifier= use;
	    }
	    return True;
	}
	if ((!new->haveSymbol)&&(!mm->haveSymbol)&&
					(new->u.keyName==mm->u.keyName)) {
	    unsigned use,ignore;
	    if (mm->modifier!=new->modifier) {
		if (clobber) {
		    use= new->modifier;
		    ignore= mm->modifier;
		}
		else {
		    use= mm->modifier;
		    ignore= new->modifier;
		}
		uError("Key <%s> added to map for multiple modifiers\n",
						longText(new->u.keyName));
		uAction("Using %s, ",XkbModIndexText(use,XkbMessage));
		uInformation("ignoring %s\n",
				     XkbModIndexText(ignore,XkbMessage));
		mm->modifier= use;
	    }
	    return True;
	}
    }
    mm= uTypedAlloc(ModMapEntry);
    if (mm==NULL) {
	uInternalError("Couldn't allocate modifier map entry\n");
	uAction("Modifier map for %s will be incomplete\n",
				XkbModIndexText(new->modifier,XkbMessage));
	return False;
    }
    *mm= *new;
    mm->next= info->modMap;
    info->modMap= mm;
    return True;
}

/***====================================================================***/

static Bool
HandleIncludeSymbols(stmt,xkb,info,hndlr)
    IncludeStmt	*	  stmt;
    XkbDescPtr		  xkb;
    SymbolsInfo *	  info;
    void		(*hndlr)();
{
unsigned 	newMerge,tmp;
Bool		clobber;
FILE	*	file;
XkbFile	*	rtrn;

    if (ProcessIncludeFile(stmt,XkmSymbolsIndex,&rtrn,&newMerge)) {
	SymbolsInfo 	myInfo;

	if (newMerge==MergeReplace) {
	    ClearSymbolsInfo(info,xkb);
	    newMerge= MergeAugment;
	}
	InitSymbolsInfo(&myInfo,xkb);
	myInfo.allowActions= info->allowActions;
	myInfo.parent= info;
	myInfo.parentMerge= newMerge;
	(*hndlr)(rtrn,xkb,MergeOverride,&myInfo);
	/* 3/14/94 (ef) -- XXX! should free rtrn and contents here */
	info->errorCount+= myInfo.errorCount;
	if (myInfo.errorCount==0) {
	    register int i;
	    KeyInfo *key;
	    clobber= (newMerge!=MergeAugment);
	    if (info->name==(Atom)NullStringToken)
		info->name= myInfo.name;

	    for (i=0,key=myInfo.keys;i<myInfo.nKeys;i++,key++) {
		if (!AddKeySymbols(info,key,clobber,False))
		    info->errorCount++;
	    }
	    if (myInfo.modMap!=NULL) {
		ModMapEntry *mm,*next;
		for (mm=myInfo.modMap;mm!=NULL;mm=next) {
		    if (!AddModMapEntry(info,mm,clobber))
			info->errorCount++;
		    next= mm->next;
		    uFree(mm);
		}
		myInfo.modMap= NULL;
	    }
	}
	FreeSymbolsInfo(&myInfo);
	return (info->errorCount==0);
    }
    info->errorCount+= 10;
    return False;
}

static LookupEntry	groupNames[]= {
	{	"group1",	1	},
	{	"group2",	2	},
	{	"group3",	3	},
	{	"group4",	4	},
	{	"group5",	5	},
	{	"group6",	6	},
	{	"group7",	7	},
	{	"group8",	8	},
	{	    NULL,	0	}
};


#define	SYMBOLS 1
#define	ACTIONS	2

static Bool
GetGroupIndex(key,arrayNdx,what,ndx_rtrn)
    KeyInfo *	key;
    ExprDef *	arrayNdx;
    unsigned	what;
    unsigned *	ndx_rtrn;
{
char *		name;
ExprResult	tmp;

    if (what==SYMBOLS)	name= "symbols";
    else		name= "actions";

    if (arrayNdx==NULL) {
	register int	i;
	unsigned	defined;
	if (what==SYMBOLS)	defined= key->symsDefined;
	else			defined= key->actsDefined;

	for (i=0;i<XkbNumKbdGroups;i++) {
	    if ((defined&(1<<i))==0) {
		*ndx_rtrn= i;
		return True;
	    }
	}
	uError("Too many groups of %s for key <%s> (max 8)\n",name,
							longText(key->name));
	uAction("Ignoring %s defined for extra groups\n",name);
	return False;
    }
    if (!ExprResolveInteger(arrayNdx,&tmp,SimpleLookup,groupNames)) {
	uError("Illegal group index for %s of key <%s>\n",name,
							longText(key->name));
	uAction("Definition with non-integer array index ignored\n");
	return False;
    }
    if ((tmp.uval<1)||(tmp.uval>XkbNumKbdGroups)) {
	uError("Group index for %s of key <%s> is out of range (1..8)\n",name,
							longText(key->name));
	uError("Symbols for group %d ignored\n",tmp.uval);
	return False;
    }
    *ndx_rtrn= tmp.uval-1;
    return True;
}

static Bool
AddSymbolsToKey(key,xkb,field,arrayNdx,value,clobber,info)
    KeyInfo *		key;
    XkbDescPtr		xkb;
    char *		field;
    ExprDef *		arrayNdx;
    ExprDef *		value;
    unsigned		clobber;
    SymbolsInfo *	info;
{
unsigned	ndx,nSyms;
ExprResult	tmp;

    if (!GetGroupIndex(key,arrayNdx,SYMBOLS,&ndx))
	return False;
    if (value==NULL) {
	key->symsDefined|= (1<<ndx);
	return True;
    }
    if (value->op!=ExprKeysymList) {
	uError("Expected a list of symbols, found %s\n",exprOpText(value->op));
	uAction("Ignoring symbols for group %d of <%s>\n",ndx,
							longText(key->name));
	return False;
    }
    if (key->syms[ndx]!=NULL) {
	uInternalError("Symbols for key <%s>, group %d already defined\n",
						longText(key->name),ndx);
	return False;
    }
    nSyms= value->value.list.nSyms;
    if (((key->groupWidth[ndx]<nSyms)||(key->syms[ndx]==NULL))&&
				(!ResizeKeyGroup(key,ndx,nSyms,False))) {
	uInternalError("Couldn't resize group %d of key %s\n",ndx,
							longText(key->name));
	uAction("Symbols lost\n");
	return False;
    }
    key->symsDefined|= (1<<ndx);
    memcpy((char *)key->syms[ndx],(char *)value->value.list.syms,
							nSyms*sizeof(KeySym));
    uFree(value->value.list.syms);
    value->value.list.syms= NULL;
    return True;
}

static Bool
AddActionsToKey(key,xkb,field,arrayNdx,value,clobber,info)
    KeyInfo *		key;
    XkbDescPtr		xkb;
    char *		field;
    ExprDef *		arrayNdx;
    ExprDef *		value;
    unsigned		clobber;
    SymbolsInfo *	info;
{
register int	i;
unsigned	ndx,nActs;
ExprResult	tmp;
ExprDef	*	act;
XkbAnyAction *	toAct;

    if (!GetGroupIndex(key,arrayNdx,ACTIONS,&ndx))
	return False;

    if (!info->allowActions) {
	if (warningLevel>0) {
	    uWarning("Alternate symbol sets cannot define key actions\n");
	    uAction("Ignoring actions assigned to group %d of key <%s>\n",ndx,
							  longText(key->name));
	}
	return False;
    }
    if (value==NULL) {
	key->actsDefined|= (1<<ndx);
	return True;
    }
    if (value->op!=ExprActionList) {
	uInternalError("Bad expression type (%d) for action list value\n");
	uAction("Ignoring actions for group %d of <%s>\n",ndx,
							longText(key->name));
	return False;
    }
    if (key->acts[ndx]!=NULL) {
	uInternalError("Actions for key <%s>, group %d already defined\n",
						longText(key->name),ndx);
	return False;
    }
    for (nActs=0,act= value->value.child;act!=NULL;nActs++) {
	act= (ExprDef *)act->common.next;
    }
    if (nActs<1) {
	uInternalError("Action list but not actions in AddActionsToKey\n");
	return False;
    }
    if (((key->groupWidth[ndx]<nActs)||(key->acts[ndx]==NULL))&&
				(!ResizeKeyGroup(key,ndx,nActs,True))) {
	uInternalError("Couldn't resize group %d of key %s\n",ndx,
							longText(key->name));
	uAction("Actions lost\n");
	return False;
    }
    key->actsDefined|= (1<<ndx);

    toAct= (XkbAnyAction *)key->acts[ndx];
    act= value->value.child;
    for (i=0;i<nActs;i++,toAct++) {
	if (!HandleActionDef(act,xkb,toAct,MergeOverride,info->action)) {
	    uError("Illegal action definition for <%s>\n",longText(key->name));
	    uAction("Action for group %d/level %d ignored\n",ndx,i);
	}
	act= (ExprDef *)act->common.next;
    }
    return True;
}

static LookupEntry	lockingEntries[] = {
	{	"true",		XkbKB_Lock	},
	{	"yes",		XkbKB_Lock	},
	{	"on",		XkbKB_Lock	},
	{	"false",	XkbKB_Default	},
	{	"no",		XkbKB_Default	},
	{	"off",		XkbKB_Default	},
	{	"permanent",	XkbKB_Lock|XkbKB_Permanent },
	{	NULL,		0	}
};

static LookupEntry	repeatEntries[]= {
	{	"true",		RepeatYes	},
	{	"yes",		RepeatYes	},
	{	"on",		RepeatYes	},
	{	"false",	RepeatNo	},
	{	"no",		RepeatNo	},
	{	"off",		RepeatNo	},
	{	"default",	RepeatUndefined	},
	{	NULL,		0	}
};

static	LookupEntry	rgEntries[]= {
	{	"none",		0	},
	{	NULL,		0	}
};

static Bool
SetSymbolsField(key,xkb,field,arrayNdx,value,clobber,info)
    KeyInfo *		key;
    XkbDescPtr		xkb;
    char *		field;
    ExprDef *		arrayNdx;
    ExprDef *		value;
    unsigned		clobber;
    SymbolsInfo *	info;
{
Bool 		ok= True;
ExprResult	tmp;

    if (uStrCaseCmp(field,"type")==0) {
	if ((arrayNdx!=NULL)&&(warningLevel>0)) {
	    uWarning("The type field of a key symbol map is not an array\n");
	    uAction("Illegal subscript ignored\n");
	}
	if ((!ExprResolveString(value,&tmp,NULL,NULL))&&(warningLevel>0)) {
	    uWarning("The type field of a key symbol map must be a string\n");
	    uAction("Ignoring illegal type definition\n");
	}
	key->type= stGetToken(tmp.str);
    }
    else if (uStrCaseCmp(field,"symbols")==0)
	return AddSymbolsToKey(key,xkb,field,arrayNdx,value,clobber,info);
    else if (uStrCaseCmp(field,"actions")==0)
	return AddActionsToKey(key,xkb,field,arrayNdx,value,clobber,info);
    else if ((uStrCaseCmp(field,"locking")==0)||(uStrCaseCmp(field,"lock")==0)||
   	     (uStrCaseCmp(field,"locks")==0)) {
	ok= ExprResolveEnum(value,&tmp,lockingEntries);
	if (ok)
	    key->behavior.type= tmp.uval;
    }
    else if ((uStrCaseCmp(field,"radiogroup")==0)||
	     (uStrCaseCmp(field,"permanentradiogroup")==0)) {
	Bool permanent= False;
	if (uStrCaseCmp(field,"permanentradiogroup")==0)
	    permanent= True;
	ok= ExprResolveInteger(value,&tmp,SimpleLookup,rgEntries);
	if (!ok) {
	    uError("Illegal radio group specification for <%s>\n",
							longText(key->name));
	    uAction("Non-integer radio group ignored\n");
	    return False;
	}
	if (tmp.uval==0) {
	    key->behavior.type= XkbKB_Default;
	    key->behavior.data= 0;
	    return ok;
	}
	if ((tmp.uval<1)||(tmp.uval>XkbMaxRadioGroups)) {
	    uError("Radio group specification for <%s> out of range (1..32)\n",
							longText(key->name));
	    uAction("Illegal radio group %d ignored\n",tmp.uval);
	    return False;
	}
	key->behavior.type= XkbKB_RadioGroup|(permanent?XkbKB_Permanent:0);
	key->behavior.data= tmp.uval-1;
    }
    else if ((uStrCaseCmp(field,"repeating")==0)||
	     (uStrCaseCmp(field,"repeats")==0)||(uStrCaseCmp(field,"repeat")==0)){
	ok= ExprResolveEnum(value,&tmp,repeatEntries);
	if (!ok) {
	    uError("Illegal repeat setting for <%s>\n",longText(key->name));
	    uAction("Non-boolean repeat setting ignored\n");
	    return False;
	}
	key->repeat= tmp.uval;
    }
    else {
	uError("Unknown field %s in a symbol interpretation\n",field);
	uAction("Definition ignored\n");
	ok= False;
    }
    return ok;
}

static int
HandleSymbolsVar(stmt,xkb,clobber,info)
    VarDef *		stmt;
    XkbDescPtr		xkb;
    Bool 		clobber;
    SymbolsInfo *	info;
{
ExprResult	elem,field;
ExprDef *	arrayNdx;

    if (ExprResolveLhs(stmt->name,&elem,&field,&arrayNdx)==0) 
	return 0; /* internal error, already reported */
    if (elem.str&&(uStrCaseCmp(elem.str,"key")==0)) {
	return SetSymbolsField(&info->dflt,xkb,field.str,arrayNdx,stmt->value,
							     clobber,info);	
    }
    return SetActionField(xkb,elem.str,field.str,arrayNdx,stmt->value,
							     &info->action);
}

static Bool
HandleSymbolsBody(def,xkb,key,clobber,info)
    VarDef *		def;
    XkbDescPtr		xkb;
    KeyInfo *		key;
    unsigned 		clobber;
    SymbolsInfo *	info;
{
Bool		ok= True;
ExprResult	tmp,field;
ExprDef *	arrayNdx;

    for (;def!=NULL;def= (VarDef *)def->common.next) {
	if ((def->name)&&(def->name->type==ExprFieldRef)) {
	    ok= HandleSymbolsVar(def,xkb,clobber,info);
	    continue;
	}
	else {
	    if (def->name==NULL) {
		if ((def->value==NULL)||(def->value->op==ExprKeysymList))
		     field.str= "symbols";
		else field.str= "actions";
		arrayNdx= NULL;
	    }
	    else {
		ok= ExprResolveLhs(def->name,&tmp,&field,&arrayNdx);
	    }
	    if (ok)
		ok= SetSymbolsField(key,xkb,field.str,arrayNdx,def->value,
							 clobber,info);
	}
    }
    return ok;
}

static int
HandleSymbolsDef(stmt,xkb,clobber,info)
    SymbolsDef *	stmt;
    XkbDescPtr		xkb;
    unsigned 		clobber;
    SymbolsInfo *	info;
{
KeyInfo			key;
char			buf[5];
 
    if (stmt->merge!=MergeDefault) {
	if ((stmt->merge==MergeReplace)||(stmt->merge==MergeOverride))
	    clobber= True;
	else if (stmt->merge==MergeAugment)
	    clobber= False;
	else {
	    uInternalError("Unknown merge mode %d in HandleSymbolsDef\n",
								stmt->merge);
	    uAction("Ignored\n");
	}
    }

    InitKeyInfo(&key);
    CopyKeyInfo(&info->dflt,&key,False);
    key.name= KeyNameToLong(stmt->keyName);
    if (!HandleSymbolsBody(stmt->symbols,xkb,&key,clobber,info)) {
	info->errorCount++;
	return False;
    }

    if (!AddKeySymbols(info,&key,clobber,True)) {
	info->errorCount++;
	return False;
    }
    return True;
}

static Bool
HandleModMapDef(def,xkb,mergeMode,info)
    ModMapDef *		def;
    XkbDescPtr		xkb;
    unsigned		mergeMode;
    SymbolsInfo *	info;
{
ExprDef	*	key;
ModMapEntry	tmp;
ExprResult	rtrn;
int		mod;
Bool 		ok;

    if (!LookupModIndex(NULL,NullStringToken,def->modifier,TypeInt,&rtrn)) {
	uError("Illegal modifier map definition\n");
	uAction("Ignoring map for non-modifier \"%s\"\n",stText(def->modifier));
	return False;
    }
    ok= True;
    tmp.modifier= rtrn.uval;
    for (key=def->keys;key!=NULL;key=(ExprDef *)key->common.next) {
	if ((key->op==ExprValue)&&(key->type==TypeKeyName)) {
	    tmp.haveSymbol= False;
	    tmp.u.keyName= KeyNameToLong(key->value.keyName);
	}
	else if (ExprResolveKeySym(key,&rtrn,NULL,NULL)) {
	    tmp.haveSymbol= True;
	    tmp.u.keySym= rtrn.uval;
	}
	else {
	    uError("Modmap entries may contain only key names or keysyms\n");
	    uAction("Illegal definition for %s modifier ignored\n",
				XkbModIndexText(tmp.modifier,XkbMessage));
	    continue;
	}

	ok= AddModMapEntry(info,&tmp,(mergeMode!=MergeAugment))&&ok;
    }
    return ok;
}

static void
HandleSymbolsFile(file,xkb,mergeMode,info)
    XkbFile	*	file;
    XkbDescPtr	 	xkb;
    unsigned		mergeMode;
    SymbolsInfo	*	info;
{
ParseCommon	*stmt;

    info->name= file->name;
    stmt= file->defs;
    while (stmt) {
	switch (stmt->stmtType) {
	    case StmtInclude:
		if (!HandleIncludeSymbols((IncludeStmt *)stmt,xkb,info,
						HandleSymbolsFile))
		    info->errorCount++;
		break;
	    case StmtSymbolsDef:
		if (!HandleSymbolsDef((SymbolsDef *)stmt,xkb,mergeMode,info))
		    info->errorCount++;
		break;
	    case StmtVarDef:
		if (!HandleSymbolsVar((VarDef *)stmt,xkb,mergeMode,info))
		    info->errorCount++;
		break;
	    case StmtVModDef:
		if (!HandleVModDef((VModDef *)stmt,mergeMode,&info->vmods))
		    info->errorCount++;
		break;
	    case StmtInterpDef:
		uError("Interpretation files may not include other types\n");
		uAction("Ignoring definition of symbol interpretation\n");
		info->errorCount++;
		break;
	    case StmtKeycodeDef:
		uError("Interpretation files may not include other types\n");
		uAction("Ignoring definition of key name\n");
		info->errorCount++;
		break;
	    case StmtModMapDef:
		if (!HandleModMapDef((ModMapDef *)stmt,xkb,mergeMode,info))
		    info->errorCount++;
		break;
	    default:
		uInternalError(
			"Unexpected statement type %d in HandleSymbolsFile\n",
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
FindNamedKey(xkb,name,kc_rtrn)
    XkbDescPtr		xkb;
    unsigned long	name;
    unsigned int *	kc_rtrn;
{
register unsigned n;

    if (xkb&&xkb->names&&xkb->names->keys) {
	for (n=xkb->min_key_code;n<=xkb->max_key_code;n++) {
	    unsigned long tmp;
	    tmp= KeyNameToLong(xkb->names->keys[n].name);
	    if (tmp==name) {
		*kc_rtrn= n;
		return True;
	    }
	}
    }
    return False;
}

Bool
FindKeyForSymbol(xkb,sym,kc_rtrn)
    XkbDescPtr		xkb;
    KeySym		sym;
    unsigned int *	kc_rtrn;
{
register int 	i, j;
register Bool	gotOne;

    j= 0;
    do {
        gotOne= False;
        for (i = xkb->min_key_code; i <= (int)xkb->max_key_code; i++) {
            if ( j<(int)XkbKeyNumSyms(xkb,i) ) {
                gotOne = True;
                if ((XkbKeySym(xkb,i,j)==sym)) {
		    *kc_rtrn= i;
                    return True;
		}
            }
        }
        j++;
    } while (gotOne);
    return False;
}

Bool
FindNamedType(xkb,name,type_rtrn)
    XkbDescPtr		xkb;
    StringToken		name;
    unsigned *		type_rtrn;
{
register unsigned n;

    if (xkb&&xkb->map&&xkb->map->types) {
	for (n=0;n<xkb->map->num_types;n++) {
	    if (xkb->map->types[n].name==(Atom)name) {
		*type_rtrn= n;
		return True;
	    }
	}
    }
    return False;
}

static Bool
CopySymbolsDef(result,key)
    XkbFileInfo *	result;
    KeyInfo *		key;
{
register int	i;
unsigned	kc,width,tmp,nGroups;
XkbKeyTypePtr	type;
Bool		haveActions;
KeySym *	outSyms;
XkbAction *	outActs;
XkbDescPtr	xkb;

    xkb= &result->xkb;
    if (!FindNamedKey(xkb,key->name,&kc)) {
	if (warningLevel>=5) {
	    uWarning("Key <%s> not found in %s keycodes\n",longText(key->name),
						stText(xkb->names->keycodes));
	    uAction("Symbols ignored\n");
	}
	return False;
    }

    haveActions= False;
    for (i=width=nGroups=0;i<XkbNumKbdGroups;i++) {
	if (key->groupWidth[i]>width)
	    width= key->groupWidth[i];
	if (((i+1)>nGroups)&&((key->symsDefined|key->actsDefined)&(1<<i)))
	    nGroups= i+1;
	if (key->acts[i])
	    haveActions= True;
    }

    if (key->type==NullStringToken) {
	if (width==1)	
	    tmp= XkbOneLevelIndex;
	else if (width==2) {
	    if (key->syms[0]&&(IsKeypadKey(key->syms[0])||
				IsKeypadKey(key->syms[1]))) {
		 tmp= XkbKeypadIndex;
	    }
	    else tmp= XkbTwoLevelIndex;
	}
	else {
	    if (warningLevel>=5) {
		uWarning("No automatic type for keys with %d symbols\n",width);
		uAction("Using TWO_LEVEL for the <%s> key (keycode %d)\n",
							longText(key->name),kc);
	    }
	    tmp= XkbTwoLevelIndex;
	}
    }
    else if (!FindNamedType(xkb,key->type,&tmp)) {
	if (warningLevel>=3) {
	    uWarning("Type \"%s\" is not defined\n",stText(key->type));
	    uAction("Using TWO_LEVEL for the <%s> key (keycode %d)\n",
							longText(key->name),kc);
	}
	tmp= XkbTwoLevelIndex;
    }
    else xkb->server->explicit[kc]|= XkbExplicitKeyTypeMask;

    type= &xkb->map->types[tmp];
    if (type->group_width<width) {
	if (warningLevel>0) {
	    uWarning("Type \"%s\" has %d levels, but <%s> has %d symbols\n",
					stText(type->name),type->group_width,
					longText(key->name),width);
	    uAction("Ignoring extra symbols\n");
	}
	width= type->group_width;
    }
    else if (type->group_width>width)
	width= type->group_width;
    i= width*nGroups;
    outSyms= XkbEnlargeKeySyms(xkb,kc,i);
    xkb->map->key_sym_map[kc].kt_index= tmp;
    xkb->map->key_sym_map[kc].group_info= 
		XkbSetNumGroups(xkb->map->key_sym_map[kc].group_info,nGroups);
    if (outSyms==NULL) {
	uInternalError("Couldn't enlarge symbols for <%s> (keycode %d)\n",
							longText(key->name),kc);
	return False;
    }
    if (haveActions) {
	outActs= XkbEnlargeKeyActions(xkb,kc,i);
	if (outActs==NULL) {
	    uInternalError("Couldn't enlarge actions for <%s> (keycode %d)\n",
							longText(key->name),kc);
	    return False;
	}
	xkb->server->explicit[kc]|= XkbExplicitInterpretMask;
    }
    else outActs= NULL;
    for (i=0;i<nGroups;i++) {
	if (key->syms[i]!=NULL) {
	    for (tmp=0;tmp<width;tmp++) {
		if (tmp<key->groupWidth[i])
		     outSyms[tmp]= key->syms[i][tmp];
		else outSyms[tmp]= NoSymbol;
		if ((outActs!=NULL)&&(key->acts[i]!=NULL)) {
		    if (tmp<key->groupWidth[i])
			 outActs[tmp]= key->acts[i][tmp];
		    else outActs[tmp].type= XkbSA_NoAction;
		}
	    }
	}
	outSyms+= width;
	if (outActs)
	    outActs+= width;
    }
    if (key->behavior.type!=XkbKB_Default) {
	xkb->server->behaviors[kc]= key->behavior;
	xkb->server->explicit[kc]|= XkbExplicitBehaviorMask;
    }
    if (key->repeat!=RepeatUndefined) {
	result->defined|=XkbPerKeyRepeatDefined;
	if (key->repeat==RepeatYes)
	     result->repeat[kc/8]|= (1<<(kc%8));
	else result->repeat[kc/8]&= ~(1<<(kc%8));
	xkb->server->explicit[kc]|= XkbExplicitAutorepeatMask;
    }
    return True;
}

static Bool
CopyModMapDef(result,entry)
    XkbFileInfo *	result;
    ModMapEntry *	entry;
{
unsigned	kc;
XkbDescPtr	xkb;

    xkb= &result->xkb;
    if ((!entry->haveSymbol)&&(!FindNamedKey(xkb,entry->u.keyName,&kc))) {
	if (warningLevel>=5) {
	    uWarning("Key <%s> not found in %s keycodes\n",
						longText(entry->u.keyName),
						stText(xkb->names->keycodes));
	    uAction("Modifier map entry for %s not updated\n",
				XkbModIndexText(entry->modifier,XkbMessage));
	}
	return False;
    }
    else if (entry->haveSymbol&&(!FindKeyForSymbol(xkb,entry->u.keySym,&kc))) {
	if (warningLevel>5) {
	    uWarning("Symbol %s not found in the %s symbol map\n",
				XkbKeysymText(entry->u.keySym,XkbMessage),
				stText(xkb->names->symbols));
	    uAction("Modifier map entry for %s not updated\n",
				XkbModIndexText(entry->modifier,XkbMessage));
	}
	return False;
    }
    result->defined|= XkbModMapDefined;
    result->modmap[kc]|= (1<<entry->modifier);
    return True;
}

Bool
CompileSymbols(file,result,mergeMode,allowActions)
    XkbFile *		file;
    XkbFileInfo *	result;
    unsigned	 	mergeMode;
    Bool	 	allowActions;
{
register int	i;
SymbolsInfo	info;
XkbDescPtr	xkb;

    xkb= &result->xkb;
    InitSymbolsInfo(&info,xkb);
    info.allowActions= allowActions;
    HandleSymbolsFile(file,xkb,mergeMode,&info);

    if (info.errorCount==0) {
	KeyInfo *key;
	if (xkb->names!=NULL)
	    xkb->names->symbols= (Atom)info.name;
	if ((xkb->names==NULL)||(xkb->names->keys==NULL)) {
	    uInternalError("Key names not defined in CompileSymbols\n");
	    uAction("Symbols not added\n");
	    return False;
	}
	if (!XkbAllocClientMap(xkb,XkbKeySymsMask,0)) {
	    uInternalError("Couldn't allocate client map in CompileSymbols\n");
	    uAction("Symbols not added\n");
	    return False;
	}
	if (!XkbAllocServerMap(xkb,XkbFullServerInfoMask,32)) {
	    uInternalError("Couldn't allocate client map in CompileSymbols\n");
	    uAction("Symbols not added\n");
	    return False;
	}
	for (key=info.keys,i=0;i<info.nKeys;i++,key++) {
	    if (!CopySymbolsDef(result,key))
		info.errorCount++;
	}
	if (info.modMap) {
	    ModMapEntry	*mm,*next;
	    for (mm=info.modMap;mm!=NULL;mm=next) {
		if (!CopyModMapDef(result,mm))
		    info.errorCount++;
		next= mm->next;
	    }
	}
	return True;
    }
    return False;
}
