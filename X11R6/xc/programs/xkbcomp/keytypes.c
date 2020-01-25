/* $XConsortium: keytypes.c,v 1.4 94/04/08 15:28:14 erik Exp $ */
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
#include "vmod.h"
#include "action.h"

#define	NAME_DEFINED	(1<<0)
#define	MASK_DEFINED	(1<<1)
#define	WRAP_DEFINED	(1<<2)

typedef struct _PreserveInfo {
    short		matchingMapIndex;
    unsigned char	indexMods;
    unsigned char	preMods;
    unsigned short	indexVMods;
    unsigned short	preVMods;
    struct _PreserveInfo *next;
} PreserveInfo;

typedef struct _KeyTypeInfo {
    Display *		dpy;
    Atom		name;
    int			fileID;
    unsigned		defined;
    unsigned		mask;
    unsigned		vmask;
    Bool		groupsWrap;
    int			group_width;
    int			nEntries;
    int			szEntries;
    XkbKTMapEntryPtr	entries;
    PreserveInfo *	preserve;
    int			szNames;
    Atom	*	lvl_names;
} KeyTypeInfo;

#define	KEYTYPE_CHUNK	8

typedef struct _KeyTypesInfo {
    Display *		dpy;
    Atom		name;
    int			errorCount;
    int			fileID;
    unsigned		stdPresent;
    int			szTypes;
    int			nTypes;
    KeyTypeInfo *	types;
    KeyTypeInfo		dflt;
    VModInfo		vmods;
} KeyTypesInfo;

Atom	tok_ONE_LEVEL;
Atom	tok_TWO_LEVEL;
Atom	tok_KEYPAD;

static void
InitKeyTypesInfo(info,xkb)
    KeyTypesInfo *	info;
    XkbDescPtr		xkb;
{
    tok_ONE_LEVEL= stGetToken("ONE_LEVEL");
    tok_TWO_LEVEL= stGetToken("TWO_LEVEL");
    tok_KEYPAD= stGetToken("KEYPAD");
    info->dpy= NULL;
    info->name= stGetToken("default");
    info->errorCount= 0;
    info->stdPresent= 0;
    info->szTypes= KEYTYPE_CHUNK;
    info->nTypes= 0;
    info->types= uTypedCalloc(KEYTYPE_CHUNK,KeyTypeInfo);
    info->dflt.name= None;
    info->dflt.defined= 0;
    info->dflt.mask= 0;
    info->dflt.vmask= 0;
    info->dflt.groupsWrap= False;
    info->dflt.group_width= 1;
    info->dflt.nEntries= info->dflt.szEntries= 0;
    info->dflt.entries= NULL;
    info->dflt.szNames= 0;
    info->dflt.lvl_names= NULL;
    info->dflt.preserve= NULL;
    InitVModInfo(&info->vmods,xkb);
    return;
}

static void
ClearKeyTypesInfo(info,xkb)
    KeyTypesInfo *	info;
    XkbDescPtr		xkb;
{
    info->dpy= NULL;
    info->name= stGetToken("default");
    info->nTypes= 0;
    info->stdPresent= 0;
    info->dflt.name= None;
    info->dflt.defined= 0;
    info->dflt.mask= 0;
    info->dflt.vmask= 0;
    info->dflt.groupsWrap= False;
    info->dflt.group_width= 1;
    info->dflt.nEntries= 0;
    ClearVModInfo(&info->vmods,xkb);
    return;
}

static void
FreeKeyTypesInfo(info)
    KeyTypesInfo *	info;
{
    info->dpy= NULL;
    if (info->types) {
	register int i;
	register KeyTypeInfo *type;
	type= info->types;
	for (i=0,type=info->types;i<info->nTypes;i++,type++) {
	    if (type->entries!=NULL) {
		uFree(type->entries);
		type->entries= NULL;
	    }
	    if (type->lvl_names!=NULL) {
		uFree(type->lvl_names);
		type->lvl_names= NULL;
	    }
	    if (type->preserve!=NULL) {
		PreserveInfo *this,*next;
		for (this=type->preserve;this!=NULL;this=next) {
		    next= this->next;
		    uFree(this);
		}
	    }
	}
	uFree(info->types);
	info->types= NULL;
    }
    if (info->dflt.entries!=NULL) {
	uFree(info->dflt.entries);
	info->dflt.entries= NULL;
    }
    if (info->dflt.lvl_names!=NULL) {
	uFree(info->dflt.lvl_names);
	info->dflt.lvl_names= NULL;
    }
    return;
}

static KeyTypeInfo *
NextKeyType(info)
    KeyTypesInfo *	info;
{
    if (info->types==NULL)
	return NULL;
    if (info->nTypes>=info->szTypes) {
	info->szTypes+= KEYTYPE_CHUNK;
	info->types= uTypedRealloc(info->types,info->szTypes,KeyTypeInfo);
	if (info->types==NULL)
	    return NULL;
    }
    bzero((char *)&info->types[info->nTypes],sizeof(KeyTypeInfo));
    info->types[info->nTypes].dpy= info->dpy;
    return &info->types[info->nTypes++];
}

static Bool
FindMatchingKeyType(info,ndx_rtrn,type)
    KeyTypesInfo *	info;
    unsigned *		ndx_rtrn;
    KeyTypeInfo	*	type;
{
register int i;

    for (i=0;i<info->nTypes;i++) {
	if (info->types[i].name==type->name) {
	    *ndx_rtrn= i;
	    return  True;
	}
    }
    return False;
}

static Bool
DeleteKeyType(info,ndx)
    KeyTypesInfo *	info;
    unsigned		ndx;
{
register int i;

    if (ndx<=XkbLastRequiredType)
	return False;
    if (ndx<(info->nTypes-1)) {
	if (info->types[ndx].entries!=NULL) {
	    uFree(info->types[ndx].entries);
	    info->types[ndx].entries= NULL;
	}
	if (info->types[ndx].lvl_names!=NULL) {
	    uFree(info->types[ndx].lvl_names);
	    info->types[ndx].lvl_names= NULL;
	}
	if (info->types[ndx].name==tok_ONE_LEVEL)
	    info->stdPresent&= ~XkbOneLevelMask;
	else if (info->types[ndx].name==tok_TWO_LEVEL)
	    info->stdPresent&= ~XkbTwoLevelMask;
	else if (info->types[ndx].name==tok_KEYPAD)
	    info->stdPresent&= ~XkbKeypadMask;
	for (i=ndx;i<(info->nTypes-1);i++) {
	    info->types[i]= info->types[i+1];
	}
	info->nTypes--;
    }
    return True;
}

static Bool
AddKeyType(info,type,merge,report)
    KeyTypesInfo *	info;
    KeyTypeInfo	*	type;
    unsigned		merge;
    Bool		report;
{
Bool			clobber,collision;
unsigned 		ndx;
KeyTypeInfo *		new;

    if (type->name==tok_ONE_LEVEL) {
	if (type->group_width>1) {
	    uError("ONE_LEVEL key type has %d levels\n",type->group_width);
	    uAction("Illegal type definition ignored\n");
	    return False;
	}
	info->stdPresent|= XkbOneLevelMask;
    }
    else if (type->name==tok_TWO_LEVEL) {
	if (type->group_width>2) {
	    uError("TWO_LEVEL key type has %d levels\n",type->group_width);
	    uAction("Illegal type definition ignored\n");
	    return False;
	}
	else if (type->group_width<2)
	    type->group_width= 2;
	info->stdPresent|= XkbTwoLevelMask;
    }
    else if (type->name==tok_KEYPAD) {
	if (type->group_width>2) {
	    uError("KEYPAD key type has %d levels\n",type->group_width);
	    uAction("Illegal type definition ignored\n");
	    return False;
	}
	else if (type->group_width<2)
	    type->group_width= 2;
	info->stdPresent|= XkbKeypadMask;
    }

    clobber= (merge==MergeOverride);
    collision= FindMatchingKeyType(info,&ndx,type);
    if (collision) {
	if ((report)&&(info->types[ndx].fileID==info->fileID)) {
	    uWarning("Multiple definitions of the %s key type\n",
						stGetString(type->name));
	    uAction("Using %s definition\n",(clobber?"last":"first"));
	}
	if (clobber) {
	    new= &info->types[ndx];
	    if (new->entries!=NULL) {
		uFree(new->entries);
		new->entries= NULL;
	    }
	    if (new->lvl_names!=NULL) {
		uFree(new->lvl_names);
		new->lvl_names= NULL;
	    }
	    if (new->preserve!=NULL) {
		PreserveInfo *this,*next;
		for (this=new->preserve;(this!=NULL);this=next) {
		    next= this->next;
		    uFree(this);
		}
	    }
	}
	else		return True;
    }
    else new= NextKeyType(info);
    if (new==NULL)
	return False;

    *new= *type;
    type->nEntries= type->szEntries= 0;
    type->entries= NULL;
    type->szNames= 0;
    type->lvl_names= NULL;
    type->preserve= NULL;
    return True;
}

/***====================================================================***/

static Bool
HandleIncludeKeyTypes(stmt,xkb,info,hndlr)
    IncludeStmt	*	  stmt;
    XkbDescPtr		  xkb;
    KeyTypesInfo *	  info;
    void		(*hndlr)();
{
unsigned 	newMerge,tmp;
FILE	*	file;
XkbFile	*	rtrn;

    if (ProcessIncludeFile(stmt,XkmTypesIndex,&rtrn,&newMerge)) {
	KeyTypesInfo 	myInfo;

	InitKeyTypesInfo(&myInfo,xkb);
	myInfo.fileID= rtrn->id;
	myInfo.dflt.name= info->dflt.name;
	myInfo.dflt.defined= info->dflt.defined;
	myInfo.dflt.mask= info->dflt.mask;
	myInfo.dflt.vmask= info->dflt.vmask;
	myInfo.dflt.groupsWrap= info->dflt.groupsWrap;
	myInfo.dflt.group_width= info->dflt.group_width;
	myInfo.dflt.nEntries= info->dflt.nEntries;
	myInfo.dflt.szEntries= info->dflt.szEntries;
	myInfo.dflt.entries= info->dflt.entries;
	myInfo.dflt.szNames= info->dflt.szNames;
	myInfo.dflt.lvl_names= info->dflt.lvl_names;
	myInfo.dflt.preserve= info->dflt.preserve;
	(*hndlr)(rtrn,xkb,MergeOverride,&myInfo);
	
	if (newMerge==MergeReplace) {
	    ClearKeyTypesInfo(info,xkb);
	    newMerge= MergeAugment;
	}
	info->errorCount+= myInfo.errorCount;
	info->dflt.nEntries= myInfo.dflt.nEntries;
	info->dflt.szEntries= myInfo.dflt.szEntries;
	info->dflt.entries= myInfo.dflt.entries;
	myInfo.dflt.nEntries= myInfo.dflt.szEntries= 0;
	myInfo.dflt.entries= NULL;
	info->dflt.szNames= myInfo.dflt.szNames;
	info->dflt.lvl_names= myInfo.dflt.lvl_names;
	myInfo.dflt.szNames= 0;
	myInfo.dflt.lvl_names= NULL;
	info->dflt.preserve= myInfo.dflt.preserve;
	myInfo.dflt.preserve= NULL;
	if (myInfo.errorCount==0) {
	    register int i;
	    KeyTypeInfo * type;

	    for (i=0,type=myInfo.types;i<myInfo.nTypes;i++,type++) {
		if (!AddKeyType(info,type,newMerge,False))
		    info->errorCount++;
	    }
	    info->stdPresent|= myInfo.stdPresent;
	}
/* 3/22/94 (ef) -- XXX! Should we copy the default back out? Probably.  */
	FreeKeyTypesInfo(&myInfo);
	return (info->errorCount==0);
    }
    info->errorCount+= 10;
    return False;
}

/***====================================================================***/

static XkbKTMapEntryPtr
FindMatchingMapEntry(type,mask,vmask)
    KeyTypeInfo *	type;
    unsigned		mask;
    unsigned		vmask;
{
register int 		i;
XkbKTMapEntryPtr	entry;

    for (i=0,entry=type->entries;i<type->nEntries;i++,entry++) {
	if ((entry->real_mods==mask)&&(entry->vmods==vmask))
	    return entry;
    }
    return NULL;
}

static void
DeleteLevel1MapEntries(type)
    KeyTypeInfo *	type;
{
register int i,n;

    for (i=0;i<type->nEntries;i++) {
	if (type->entries[i].level==0) {
	    for (n=i;n<type->nEntries-1;n++) {
		type->entries[n]= type->entries[n+1];
	    }
	    type->nEntries--;
	}
    }
    return;
}

static XkbKTMapEntryPtr
NextMapEntry(type)
    KeyTypeInfo *	type;
{
    if (type->entries==NULL) {
	type->entries= uTypedCalloc(2,XkbKTMapEntryRec);
	if (type->entries==NULL) {
	    uError("Couldn't allocate map entries for key type %s\n",
				XkbAtomText(type->dpy,type->name,XkbMessage));
	    uAction("Map entries lost\n");
	    return NULL;
	}
	type->szEntries= 2;
	type->nEntries= 0;
    }
    else if (type->nEntries>=type->szEntries) {
	type->szEntries*=2;
	type->entries= uTypedRecalloc(type->entries,
					type->nEntries,type->szEntries,
					XkbKTMapEntryRec);
	if (type->entries==NULL) {
	    uError("Couldn't reallocate map entries for key type %s\n",
				XkbAtomText(type->dpy,type->name,XkbMessage));
	    uAction("Map entries lost\n");
	    return NULL;
	}
    }
    return &type->entries[type->nEntries++];
}

static Bool
AddPreserve(xkb,type,mods,vmods,preMods,preVMods,merge,report)
    XkbDescPtr		xkb;
    KeyTypeInfo *	type;
    unsigned		mods;
    unsigned		vmods;
    unsigned		preMods;
    unsigned		preVMods;
    unsigned		merge;
    Bool		report;
{
PreserveInfo	*pre;

    pre= type->preserve;
    while (pre!=NULL) {
	if ((pre->indexMods!=mods)||(pre->indexVMods!=vmods))
	    continue;
	if ((pre->preMods==preMods)&&(pre->preVMods==preVMods)) {
	    if (report) {
		uWarning("Identical definitions for preserve[%s] in %s\n",
			XkbVModMaskText(type->dpy,xkb,mods,vmods,XkbMessage),
			XkbAtomText(type->dpy,type->name,XkbMessage));
		uAction("Ignored\n");
	    }
	    return True;
	}
	if (report) {
	    char *str;
	    uWarning("Multiple defintions for preserve[%s] in %s\n",
			XkbVModMaskText(type->dpy,xkb,mods,vmods,XkbMessage),
			XkbAtomText(type->dpy,type->name,XkbMessage));
	    if (merge==MergeAugment)
		 str= XkbVModMaskText(type->dpy,xkb,pre->preMods,pre->preVMods,
								XkbMessage);
	    else str=XkbVModMaskText(type->dpy,xkb,preMods,preVMods,XkbMessage);
	    uAction("Using %s, ",str);
	    if (merge==MergeAugment)
		 str=XkbVModMaskText(type->dpy,xkb,preMods,preVMods,XkbMessage);
	    else str= XkbVModMaskText(type->dpy,xkb,pre->preMods,pre->preVMods,
								XkbMessage);
	    uInformation("ignoring %s\n",str);
	}
	if (merge==MergeOverride) {
	    pre->preMods= preMods;
	    pre->preVMods= preVMods;
	    return True;
	}
	pre= pre->next;
    }
    pre= uTypedAlloc(PreserveInfo);
    if (!pre) {
	uInternalError("Couldn't allocate preserve entry in %s\n",
				XkbAtomText(type->dpy,type->name,XkbMessage));
	uAction("Definition of preserve[%s] lost\n",
		XkbVModMaskText(type->dpy,xkb,preMods,preVMods,XkbMessage));
	return False;
    }
    pre->matchingMapIndex= -1;
    pre->indexMods= mods;
    pre->indexVMods= vmods;
    pre->preMods= preMods;
    pre->preVMods= preVMods;
    pre->next= type->preserve;
    type->preserve= pre;
    return True;

}

static Bool
AddMapEntry(xkb,type,mods,vmods,level,merge,report)
    XkbDescPtr		xkb;
    KeyTypeInfo *	type;
    unsigned		mods;
    unsigned		vmods;
    unsigned		level;
    unsigned		merge;
    Bool		report;
{
XkbKTMapEntryPtr	entry;

    if ((entry=FindMatchingMapEntry(type,mods,vmods))!=NULL) {
	if ((report)&&(entry->level!=level)) {
	    uWarning("Duplicate map entries for %s in %s\n",
			XkbVModMaskText(type->dpy,xkb,mods,vmods,XkbMessage),
			XkbAtomText(type->dpy,type->name,XkbMessage));
	    uInformation("Using %d, ignoring %d\n",
			     (merge==MergeOverride?level+1:entry->level+1));
	}
	else if (report) {
	    uWarning("Multiple occurences of map[%s]= %s in %s\n",
			XkbVModMaskText(type->dpy,xkb,mods,vmods,XkbMessage),
			level+1,XkbAtomText(type->dpy,type->name,XkbMessage));
	    uInformation("Ignored\n");
	    return True;
	}
	if (merge==MergeOverride)
	    entry->level= level;
	return True;
    }
    if ((entry=NextMapEntry(type))==NULL)
	return False;	/* allocation failure, already reported */
    if (level>=type->group_width)
	type->group_width= level+1;
    if (vmods==0)	entry->active= True;
    else		entry->active= False;
    entry->mask= mods;
    entry->real_mods= mods;
    entry->vmods= vmods;
    entry->level= level;
    return True;
}

static LookupEntry lnames[] = {
	{	"level1",	1	},
	{	"level2",	2	},
	{	"level3",	3	},
	{	"level4",	4	},
	{	"level5",	5	},
	{	"level6",	6	},
	{	"level7",	7	},
	{	"level8",	8	},
	{	NULL,		0	}
};

static Bool
SetMapEntry(type,xkb,arrayNdx,value,merge,report)
    KeyTypeInfo *	type;
    XkbDescPtr		xkb;
    ExprDef *		arrayNdx;
    ExprDef *		value;
    unsigned		merge;
    Bool		report;
{
ExprResult		rtrn;
unsigned		mods,vmods;
XkbKTMapEntryPtr	entry;

    if (arrayNdx==NULL) {
	uError("Map entry without array subscript in key type %s\n",
				XkbAtomText(type->dpy,type->name,XkbMessage));
	uAction("Ignoring illegal map entry\n");
	return False;
    }
    if (!ExprResolveModMask(arrayNdx,&rtrn,LookupVModMask,(XPointer)xkb)) {
	uError("Map entry index not a modifier mask in key type %s\n",
				XkbAtomText(type->dpy,type->name,XkbMessage));
	uAction("Ignoring illegal map entry\n");
	return False;
    }
    mods= rtrn.uval&0xff;
    vmods= (rtrn.uval>>8)&0xffff;
    if (((mods&(~type->mask))!=0)||((vmods&(~type->vmask))!=0)) {
	uWarning("Map entry for modifiers that are not used by the %s type\n",
				XkbAtomText(type->dpy,type->name,XkbMessage));
	uAction("Using %s instead of ",XkbVModMaskText(type->dpy,xkb,
							mods&type->mask,
							vmods&type->vmask,
							XkbMessage));
	uInformation("%s\n",XkbVModMaskText(type->dpy,xkb,mods,vmods,
							XkbMessage));
	mods= mods&type->mask;
	vmods= vmods&type->vmask;
    }
    if (!ExprResolveInteger(value,&rtrn,SimpleLookup,(XPointer)lnames)) {
	uError("Level specifications in a key type must be integer\n");
	uAction("Ignoring malformed level specification\n");
	return False;
    }
    if ((rtrn.ival<1)||(rtrn.ival>256)) {
	uError("Shift level %d out of range (1..256) in key type %s\n",
				XkbAtomText(type->dpy,type->name,XkbMessage));
	uAction("Ignoring illegal map entry %s\n",
				XkbVModMaskText(type->dpy,xkb,mods,vmods,
							XkbMessage));
	return False;
    }
    return AddMapEntry(xkb,type,mods,vmods,rtrn.ival-1,merge,report);
}

static Bool
SetPreserve(type,xkb,arrayNdx,value,merge,report)
    KeyTypeInfo *	type;
    XkbDescPtr		xkb;
    ExprDef *		arrayNdx;
    ExprDef *		value;
    unsigned		merge;
    Bool		report;
{
ExprResult	 rtrn;
unsigned	 mods,vmods;
unsigned	 pmods,pvmods;
XkbKTMapEntryPtr entry;
PreserveInfo *	 pre;

    if (arrayNdx==NULL) {
	uError("Preseve entry without array subscript in key type %s\n",
				XkbAtomText(type->dpy,type->name,XkbMessage));
	uAction("Ignoring illegal entry\n");
	return False;
    }
    if (!ExprResolveModMask(arrayNdx,&rtrn,LookupVModMask,(XPointer)xkb)) {
	uError("Preserve entry index not a modifier mask in key type %s\n",
				XkbAtomText(type->dpy,type->name,XkbMessage));
	uAction("Ignoring illegal entry\n");
	return False;
    }
    mods= rtrn.uval&0xff;
    vmods= (rtrn.uval>>8)&0xffff;
    if (((mods&(~type->mask))!=0)||((vmods&(~type->vmask))!=0)) {
	uWarning("Preserve for modifiers that are not used by the %s type\n",
				XkbAtomText(type->dpy,type->name,XkbMessage));
	uAction("Using %s instead of ",XkbVModMaskText(type->dpy,xkb,
							mods&type->mask,
							vmods&type->vmask,
							XkbMessage));
	uInformation("%s\n",XkbVModMaskText(type->dpy,xkb,mods,vmods,
							XkbMessage));
	mods= mods&type->mask;
	vmods= vmods&type->vmask;
    }
    if (!ExprResolveModMask(value,&rtrn,LookupVModMask,(XPointer)xkb)) {
	uError("Preserve value in a key type must be a modifier mask\n");
	uAction("Ignoring malformed preserve result\n");
	return False;
    }
    pmods= rtrn.uval&0xff;
    pvmods= (rtrn.uval>>16)&0xffff;
    if ((pmods&(~mods))||(pvmods&&(~vmods))) {
	uError("Preserve value cannot include mods that are not in the mask\n");
	uAction("Ignoring %s in preserve result\n",
	     XkbVModMaskText(type->dpy,xkb,pmods&(~mods),pvmods&(~vmods),
							XkbMessage));
	pmods&= mods;
	pvmods&= vmods;
    }
    return AddPreserve(xkb,type,mods,vmods,pmods,pvmods,merge,report);
}

/***====================================================================***/

static Bool
AddLevelName(type,level,name,merge,report)
    KeyTypeInfo *	type;
    unsigned		level;
    Atom		name;
    unsigned		merge;
    Bool		report;
{
    if ((type->lvl_names==NULL)||(type->szNames<=level)) {
	register int i;
	type->lvl_names= uTypedRealloc(type->lvl_names,level+1,Atom);
	if (type->lvl_names==NULL) {
	    uError("Couldn't allocate level names for key type %s\n",
				XkbAtomText(type->dpy,type->name,XkbMessage));
	    uAction("Level names lost\n");
	    type->szNames= 0;
	    return False;
	}
	type->szNames= level+1;
    }
    else if (type->lvl_names[level]!=None) {
	if (report) {
	    uError("Multiple names for level %d of key type %s\n",level+1,
				XkbAtomText(type->dpy,type->name,XkbMessage));
	    if (merge==MergeOverride)
		 uAction("Using %s, ignoring %s\n",
				XkbAtomText(type->dpy,name,XkbMessage),
				XkbAtomText(type->dpy,type->lvl_names[level],
								XkbMessage));
	    else uAction("Using %s, ignoring %s\n",
				 XkbAtomText(type->dpy,type->lvl_names[level],
								XkbMessage),
				 XkbAtomText(type->dpy,name,XkbMessage));
	}
	if (merge!=MergeOverride)
	    return True;
    }
    if (level>=type->group_width)
	type->group_width= level+1;
    type->lvl_names[level]= name;
    return True;
}

static Bool
SetLevelName(type,arrayNdx,value,merge,report)
    KeyTypeInfo *	type;
    ExprDef *		arrayNdx;
    ExprDef *		value;
    unsigned		merge;
    Bool		report;
{
ExprResult	rtrn;
unsigned 	level;

    if (arrayNdx==NULL) {
	uError("Level name without array subscript in key type %s\n",
				XkbAtomText(type->dpy,type->name,XkbMessage));
	uAction("Ignoring illegal level name definition\n");
	return False;
    }
    if (!ExprResolveInteger(arrayNdx,&rtrn,SimpleLookup,(XPointer)lnames)) {
	uError("Level name index not an integer in key type %s\n",
				XkbAtomText(type->dpy,type->name,XkbMessage));
	uAction("Ignoring illegal level name definition\n");
	return False;
    }
    if ((rtrn.ival<1)||(rtrn.ival>256)) {
	uError("Level name %d out of range (1..256) in key type %s\n",
				rtrn.ival,
				XkbAtomText(type->dpy,type->name,XkbMessage));
	uAction("Ignoring illegal level name definition\n");
	return False;
    }
    level= rtrn.ival-1;
    if (!ExprResolveString(value,&rtrn,NULL,NULL)) {
	uError("Non-string name for level %d in key type %s\n",level+1,
				XkbAtomText(type->dpy,type->name,XkbMessage));
	uAction("Ignoring illegal level name definition\n");
	return False;
    }
    return AddLevelName(type,level,stGetToken(rtrn.str),merge,report);
}

/***====================================================================***/

static Bool
SetKeyTypeField(type,xkb,field,arrayNdx,value,merge,info)
    KeyTypeInfo *	type;
    XkbDescPtr		xkb;
    char *		field;
    ExprDef *		arrayNdx;
    ExprDef *		value;
    unsigned		merge;
    KeyTypesInfo *	info;
{
int 		ok= 1;
ExprResult	tmp;

    if (uStrCaseCmp(field,"modifiers")==0) {
	unsigned mods,vmods;
	if (arrayNdx!=NULL) {
	    uWarning("The modifiers field of a is not an array\n");
	    uAction("Illegal array subscript ignored\n");
	}
	if (!ExprResolveModMask(value,&tmp,LookupVModMask,(XPointer)xkb)) {
	    uError("Key type mask field must be a modifier mask\n");
	    uAction("Key type definition ignored\n");
	    return False;
	}
	mods= tmp.uval&0xff;
	vmods= (tmp.uval>>8)&0xffff;
	if ((type->mask!=0)||(type->vmask!=0)) {
	    uWarning("Multiple modifier mask definitions for key type %s\n",
				XkbAtomText(type->dpy,type->name,XkbMessage));
	    uAction("Using %s, ",XkbVModMaskText(type->dpy,xkb,type->mask,
								type->vmask,
								XkbMessage));
	    uInformation("ignoring %s\n",XkbVModMaskText(type->dpy,xkb,mods,
								vmods,
								XkbMessage));
	    return False;
	}
	type->mask= mods;
	type->vmask= vmods;
	return True;
    }
    else if (uStrCaseCmp(field,"map")==0)
	return SetMapEntry(type,xkb,arrayNdx,value,merge,True);
    else if (uStrCaseCmp(field,"preserve")==0)
	return SetPreserve(type,xkb,arrayNdx,value,merge,True);
    else if ((uStrCaseCmp(field,"levelname")==0)||
	     (uStrCaseCmp(field,"level_name")==0))
	return SetLevelName(type,arrayNdx,value,merge,True);
    else if (uStrCaseCmp(field,"groupswrap")==0) {
	if (!ExprResolveBoolean(value,&tmp,NULL,NULL)) {
	    uError("Non-boolean value for groupsWrap in key type %s\n",
				XkbAtomText(type->dpy,type->name,XkbMessage));
	    uInternalError("Illegal value ignored\n");
	    return False;
	}
	type->groupsWrap= tmp.uval;
    }
    uError("Unknown field %s in key type %s\n",field,
				XkbAtomText(type->dpy,type->name,XkbMessage));
    uAction("Definition ignored\n");
    return False;
}

static Bool
HandleKeyTypeVar(stmt,xkb,merge,info)
    VarDef *		stmt;
    XkbDescPtr		xkb;
    unsigned 		merge;
    KeyTypesInfo *	info;
{
ExprResult	elem,field;
ExprDef *	arrayNdx;

    if (!ExprResolveLhs(stmt->name,&elem,&field,&arrayNdx)) 
	return False; /* internal error, already reported */
    if (elem.str&&(uStrCaseCmp(elem.str,"type")==0))
	return SetKeyTypeField(&info->dflt,xkb,field.str,arrayNdx,stmt->value,
							     merge,info);	
    if (elem.str!=NULL) {
	uError("Default for unknown element %s\n",uStringText(elem.str));
	uAction("Value for field %s ignored\n",uStringText(field.str));
    }
    else if (field.str!=NULL) {
	uError("Default defined for unknown field %s\n",uStringText(field.str));
	uAction("Ignored\n");
    }
    return False;
}

static int
HandleKeyTypeBody(def,xkb,type,merge,info)
    VarDef *		def;
    XkbDescPtr		xkb;
    KeyTypeInfo *	type;
    unsigned		merge;
    KeyTypesInfo *	info;
{
int		ok= 1;
ExprResult	tmp,field;
ExprDef *	arrayNdx;

    for (;def!=NULL;def= (VarDef *)def->common.next) {
	if ((def->name)&&(def->name->type==ExprFieldRef)) {
	    ok= HandleKeyTypeVar(def,xkb,merge,info);
	    continue;
	}
	ok= ExprResolveLhs(def->name,&tmp,&field,&arrayNdx);
	if (ok)
	    ok= SetKeyTypeField(type,xkb,field.str,arrayNdx,def->value,
							 merge,info);
    }
    return ok;
}

static int
HandleKeyTypeDef(stmt,xkb,merge,info)
    KeyTypeDef *	stmt;
    XkbDescPtr		xkb;
    unsigned 		merge;
    KeyTypesInfo *	info;
{
register int		i;
KeyTypeInfo 		type;
 
    if (stmt->merge!=MergeDefault) {
	if (stmt->merge==MergeReplace)
	     merge= MergeOverride;
	else merge= stmt->merge;
    }

    type.name= stmt->name;
    type.fileID= info->fileID;
    type.defined= NAME_DEFINED;
    type.mask= info->dflt.mask;
    type.vmask= info->dflt.vmask;
    type.groupsWrap= info->dflt.groupsWrap;
    type.group_width= 1;
    type.nEntries= type.szEntries= 0;
    type.entries= NULL;
    type.szNames= 0;
    type.lvl_names= NULL;
    type.preserve= NULL;

    if (!HandleKeyTypeBody(stmt->body,xkb,&type,merge,info)) {
	info->errorCount++;
	return False;
    }

    for (i=0;i<info->dflt.nEntries;i++) {
	XkbKTMapEntryPtr entry;
	entry= &info->dflt.entries[i];
	if (((entry->real_mods&type.mask)==entry->real_mods)&&
	    ((entry->vmods&type.vmask)==entry->vmods)) {
	    AddMapEntry(xkb,&type,entry->real_mods,entry->vmods,
					entry->level,MergeAugment,False);
	}
    }
    for (i=0;i<info->dflt.szNames;i++) {
	if ((i<type.group_width)&&(info->dflt.lvl_names[i]!=None)) {
	    AddLevelName(&type,i,info->dflt.lvl_names[i],MergeAugment,False);
	}
    }
    if (!AddKeyType(info,&type,merge,True)) {
	info->errorCount++;
	return False;
    }
    return True;
}

static void
HandleKeyTypesFile(file,xkb,merge,info)
    XkbFile		*file;
    XkbDescPtr	 	 xkb;
    unsigned		 merge;
    KeyTypesInfo	*info;
{
ParseCommon	*stmt;

    if ((merge==MergeOverride)||(info->name==None))
	info->name= file->name;

    stmt= file->defs;
    while (stmt) {
	switch (stmt->stmtType) {
	    case StmtInclude:
		if (!HandleIncludeKeyTypes((IncludeStmt *)stmt,xkb,info,
						HandleKeyTypesFile))
		    info->errorCount++;
		break;
	    case StmtKeyTypeDef:
		if (!HandleKeyTypeDef((KeyTypeDef *)stmt,xkb,merge,info))
		    info->errorCount++;
		break;
	    case StmtVarDef:
		if (!HandleKeyTypeVar((VarDef *)stmt,xkb,merge,info))
		    info->errorCount++;
		break;
	    case StmtVModDef:
		if (!HandleVModDef((VModDef *)stmt,merge,&info->vmods))
		    info->errorCount++;
		break;
	    case StmtKeycodeDef:
		uError("Interpretation files may not include other types\n");
		uAction("Ignoring definition of key name\n");
		info->errorCount++;
		break;
	    case StmtInterpDef:
		uError("Interpretation files may not include other types\n");
		uAction("Ignoring definition of symbol interpretation\n");
		info->errorCount++;
		break;
	    default:
		uInternalError(
			"Unexpected statement type %d in HandleKeyTypesFile\n",
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

static Bool
CopyDefToKeyType(xkb,type,def)
    XkbDescPtr		xkb;
    XkbKeyTypePtr	type;
    KeyTypeInfo *	def;
{
register int i;
PreserveInfo *pre;

    for (pre=def->preserve;pre!=NULL;pre=pre->next) {
	XkbKTMapEntryPtr match;
	AddMapEntry(xkb,def,pre->indexMods,pre->indexVMods,0,MergeAugment,0);
	match= FindMatchingMapEntry(def,pre->indexMods,pre->indexVMods);
	if (!match) {
	    uInternalError("Couldn't find matching entry for preserve\n");
	    uAction("Aborting\n");
	    return False;
	}
	pre->matchingMapIndex= match-def->entries;
    }
    type->real_mods= def->mask;
    type->vmods= def->vmask;
    type->group_width= def->group_width;
    type->map_count= def->nEntries;
    type->map= def->entries;
    if (def->preserve) {
	type->preserve= uTypedCalloc(type->map_count,XkbKTPreserveRec);
	if (!type->preserve) {
	    uWarning("Couldn't allocate preserve array in CopyDefToKeyType\n");
	    uAction("Preserve setting for type %s lost\n",
				XkbAtomText(def->dpy,def->name,XkbMessage));
	}
	else {
	    for (pre=def->preserve;pre!=NULL;pre=pre->next) {
		int ndx= pre->matchingMapIndex;
		type->preserve[ndx].mask= pre->preMods;
		type->preserve[ndx].real_mods= pre->preMods;
		type->preserve[ndx].vmods= pre->preVMods;
	    }
	}
    }
    else type->preserve= NULL;
    type->name= (Atom)def->name;
    if (def->szNames>0) {
	type->lvl_names= uTypedCalloc(def->group_width,Atom);

	/* assert def->szNames<=def->group_width */
	for (i=0;i<def->szNames;i++) {
	    type->lvl_names[i]= (Atom)def->lvl_names[i];
	}
    }
    else {
	type->lvl_names= NULL;
    }

    def->nEntries= def->szEntries= 0;
    def->entries= NULL;
    return XkbComputeEffectiveMap(xkb,type,NULL);
}

Bool
CompileKeyTypes(file,result,merge)
    XkbFile *		file;
    XkbFileInfo *	result;
    unsigned	 	merge;
{
int		errorCount= 0;
KeyTypesInfo	info;
XkbDescPtr	xkb;

    xkb= &result->xkb;
    InitKeyTypesInfo(&info,xkb);
    info.fileID= file->id;
    HandleKeyTypesFile(file,xkb,merge,&info);

    if (info.errorCount==0) {
	register int i;
	register KeyTypeInfo *def;
	register XkbKeyTypePtr type,next;

	i= info.nTypes;
	if ((info.stdPresent&XkbOneLevelMask)==0)
	    i++;
	if ((info.stdPresent&XkbTwoLevelMask)==0)
	    i++;
	if ((info.stdPresent&XkbKeypadMask)==0)
	    i++;
	if (!XkbAllocClientMap(xkb,XkbKeyTypesMask,i)) {
	    uInternalError("Couldn't allocate client map\n");
	    uAction("Exiting\n");
	    return False;
	}
	xkb->map->num_types= i;
	if (XkbAllRequiredTypes&(~info.stdPresent)) {
	    unsigned missing,keypadVMod;

	    missing= XkbAllRequiredTypes&(~info.stdPresent);
	    keypadVMod= FindKeypadVMod(xkb);
	    if (!XkbInitCanonicalKeyTypes(xkb,missing,keypadVMod)) {
		uInternalError("Couldn't initialize canonical key types\n");
		uAction("Exiting\n");
		return False;
	    }
	    if (missing&XkbOneLevelMask)
		xkb->map->types[XkbOneLevelIndex].name= (Atom)tok_ONE_LEVEL;
	    if (missing&XkbTwoLevelMask)
		xkb->map->types[XkbTwoLevelIndex].name= (Atom)tok_TWO_LEVEL;
	    if (missing&XkbKeypadMask)
		xkb->map->types[XkbKeypadIndex].name= (Atom)tok_KEYPAD;
	}
	next= &xkb->map->types[XkbLastRequiredType+1];
	for (i=0,def=info.types;i<info.nTypes;i++,def++) {
	    int map_count;
	    if (def->name==tok_ONE_LEVEL)
		 type= &xkb->map->types[XkbOneLevelIndex];
	    else if (def->name==tok_TWO_LEVEL)
		 type= &xkb->map->types[XkbTwoLevelIndex];
	    else if (def->name==tok_KEYPAD)
		 type= &xkb->map->types[XkbKeypadIndex];
	    else type= next++;
	    DeleteLevel1MapEntries(def);
	    if (!CopyDefToKeyType(xkb,type,def))
		return False;
	}
	return True;
    }
    return False;
}

