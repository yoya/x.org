/* $XConsortium: compat.c,v 1.3 94/04/08 15:22:28 erik Exp $ */
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
#include "indicators.h"
#include "action.h"

typedef struct _ModCompatInfo {
    StringToken			name;
    Bool			realMod;
    unsigned char		ndx;
    unsigned char		mods;
    unsigned char		groups;
    struct _ModCompatInfo *	next;
} ModCompatInfo;

#define	INTERP_CHUNK	32
typedef struct _CompatInfo {
    StringToken		name;
    int			errorCount;
    int			szInterps;
    int			nInterps;
    XkbSymInterpretPtr	interps;
    XkbSymInterpretRec	dflt;
    XkbFileLEDInfo	ledDflt;
    ModCompatInfo *	modCompat;
    XkbFileLEDInfo *	leds;
    VModInfo		vmods;
    ActionInfo *	act;
    Display *		dpy;
} CompatInfo;

static void
InitCompatInfo(dpy,info,xkb)
    Display *		dpy;
    CompatInfo *	info;
    XkbDescPtr		xkb;
{
register int i;

    info->name= NullStringToken;
    info->errorCount= 0;
    info->szInterps= INTERP_CHUNK;
    info->nInterps= 0;
    info->interps= uTypedCalloc(INTERP_CHUNK,XkbSymInterpretRec);
    info->act= NULL;
    info->dflt.flags=	0;
    info->dflt.virtual_mod= XkbNoModifier;
    info->dflt.act.type= XkbSA_NoAction;
    ClearIndicatorMapInfo(dpy,&info->ledDflt);
    for (i=0;i<XkbAnyActionDataSize;i++) {
	info->dflt.act.data[i]= 0;
    }
    info->modCompat= NULL;
    info->leds= NULL;
    InitVModInfo(&info->vmods,xkb);
    info->dpy= dpy;
    return;
}

static void
ClearCompatInfo(info,xkb)
    CompatInfo *	info;
    XkbDescPtr		xkb;
{
register int i;

    info->name= NullStringToken;
    info->nInterps= 0;
    info->dflt.flags= 0;
    info->dflt.virtual_mod= XkbNoModifier;
    info->dflt.act.type= XkbSA_NoAction;
    for (i=0;i<XkbAnyActionDataSize;i++) {
	info->dflt.act.data[i]= 0;
    }
    ClearIndicatorMapInfo(info->dpy,&info->ledDflt);
    if (info->modCompat!=NULL) {
	ModCompatInfo *mc,*next;
	for (mc=info->modCompat;mc!=NULL;mc=next) {
	    next= mc->next;
	    uFree(mc);
	}
    }
    if (info->leds!=NULL) {
	XkbFileLEDInfo *led,*next;
	for (led=info->leds;led!=NULL;led=next) {
	    next= led->next;
	    uFree(led);
	}
    }
    /* 3/30/94 (ef) -- XXX! Should free action info here */
    ClearVModInfo(&info->vmods,xkb);
    return;
}

static void
FreeCompatInfo(info)
    CompatInfo *	info;
{

    if (info->interps)
	uFree(info->interps);
    if (info->modCompat!=NULL) {
	ModCompatInfo *mc,*next;

	for (mc=info->modCompat;mc!=NULL;mc=next) {
	    next= mc->next;
	    uFree(mc);
	}
    }
    if (info->leds!=NULL) {
	XkbFileLEDInfo *led,*next;

	for (led=info->leds;led!=NULL;led=next) {
	    next= led->next;
	    uFree(led);
	}
    }
    bzero((char *)info,sizeof(CompatInfo));
    /* 3/30/94 (ef) -- XXX! Should free action info here */
    return;
}

static XkbSymInterpretPtr
NextInterp(info)
    CompatInfo *	info;
{
    if (info->interps==NULL)
	return NULL;
    if (info->nInterps>=info->szInterps) {
	info->szInterps+= INTERP_CHUNK;
	info->interps= uTypedRealloc(info->interps,info->szInterps,
						   XkbSymInterpretRec);
	if (info->interps==NULL)
	    return NULL;
    }
    bzero((char *)&info->interps[info->nInterps],sizeof(XkbSymInterpretRec));
    return &info->interps[info->nInterps++];
}

static Bool
FindMatchingInterp(info,ndx_rtrn,interp)
    CompatInfo *	info;
    unsigned *		ndx_rtrn;
    XkbSymInterpretPtr	interp;
{
register int i;

    for (i=0;i<info->nInterps;i++) {
	if ((info->interps[i].sym==interp->sym)&&
				(info->interps[i].mods==interp->mods)&&
				(info->interps[i].match==interp->match)) {
	    *ndx_rtrn= i;
	    return  True;
	}
    }
    return False;
}

static void
DeleteInterp(info,ndx)
    CompatInfo *	info;
    unsigned		ndx;
{
register int i;

    if (ndx<(info->nInterps-1)) {
	for (i=ndx;i<(info->nInterps-1);i++) {
	    info->interps[i]= info->interps[i+1];
	}
	info->nInterps--;
    }
    return;
}

static Bool
AddInterp(info,interp,mergeMode,reportCollisions)
    CompatInfo *	info;
    XkbSymInterpretPtr	interp;
    unsigned		mergeMode;
    Bool		reportCollisions;
{
Bool			clobber,collision;
unsigned 		ndx;
XkbSymInterpretPtr	new;

    clobber= (mergeMode==MergeOverride);
    collision= FindMatchingInterp(info,&ndx,interp);
    if (collision) {
	if ((reportCollisions)&&(warningLevel>=5)) {
	    uWarning("Multiple interpretations of \"%s+%s(%s)\"\n",
				XkbKeysymText(interp->sym,XkbMessage),
				SIMatchText(interp->match,XkbMessage),
				XkbModMaskText(interp->mods,XkbMessage));
	    uAction("Using %s definition\n",(clobber?"last":"first"));
	}
	if (clobber)	new= &info->interps[ndx];
	else		return True;
    }
    else new= NextInterp(info);
    if (new==NULL)
	return False;
    *new= *interp;
    return True;
}

static Bool
AddModCompat(info,modCompat,mergeMode,reportCollisions)
    CompatInfo *	info;
    ModCompatInfo *	modCompat;
    unsigned		mergeMode;
    Bool		reportCollisions;
{
ModCompatInfo *mc;

    for (mc=info->modCompat;mc!=NULL;mc=mc->next) {
	if ((mc->realMod==modCompat->realMod)&&(mc->ndx==modCompat->ndx)) {
	    if ((mc->mods==modCompat->mods)&&(mc->groups==modCompat->groups))
		return True;
	    if ((reportCollisions)&&(warningLevel>0)) {
		uWarning("Compatibility map for %s modifier %s redefined\n",
						(mc->realMod?"real":"virtual"),
						stText(mc->name));
		uAction("Using %s definition\n",
				(mergeMode==MergeAugment?"first":"last"));
	    }
	    if (mergeMode!=MergeAugment) {
		ModCompatInfo *next= mc->next;
		*mc= *modCompat;
		mc->next= next;
	    }
	    return True;
	}
    }
    mc= uTypedAlloc(ModCompatInfo);
    if (!mc) {
	mc= modCompat;
	uInternalError("Couldn't allocate description of modifier map\n");
	uAction("Modifier compatibility map for %s modifier %s incorrect\n",
						(mc->realMod?"real":"virtual"),
						stText(mc->name));
	return False;
    }
    *mc= *modCompat;
    mc->next= info->modCompat;
    info->modCompat= mc;
    return True;
}

/***====================================================================***/

Bool
ResolveStateAndPredicate(expr,pred_rtrn,mods_rtrn)
    ExprDef *	expr;
    unsigned *	pred_rtrn;
    unsigned *	mods_rtrn;
{
ExprResult	result;

    if (expr==NULL) {
	*pred_rtrn= XkbSI_AnyOfOrNone;
	*mods_rtrn= ~0;
	return True;
    }

    *pred_rtrn= XkbSI_Exactly;
    if (expr->op==ExprActionDecl) {
	char *pred_txt= stGetString(expr->value.action.name);
	if (uStrCaseCmp(pred_txt,"noneof")==0)
	     *pred_rtrn= XkbSI_NoneOf;
	else if (uStrCaseCmp(pred_txt,"anyofornone")==0)
	     *pred_rtrn= XkbSI_AnyOfOrNone;
	else if (uStrCaseCmp(pred_txt,"anyof")==0)
	     *pred_rtrn= XkbSI_AnyOf;
	else if (uStrCaseCmp(pred_txt,"allof")==0)
	     *pred_rtrn= XkbSI_AllOf;
	else if (uStrCaseCmp(pred_txt,"exactly")==0)
	     *pred_rtrn= XkbSI_Exactly;
	else {
	     uError("Illegal modifier predicate \"%s\"\n",pred_txt);
	     uAction("Ignored\n");
	     return False;
	}
	expr= expr->value.action.args;
    }
    else if (expr->op==ExprIdent) {
	char *pred_txt= stGetString(expr->value.str);
	if ((pred_txt)&&(uStrCaseCmp(pred_txt,"any")==0)) {
	    *pred_rtrn= XkbSI_AnyOf;
	    *mods_rtrn= 0xff;
	    return True;
	}
    }

    if (ExprResolveModMask(expr,&result,NULL,NULL)) {
	*mods_rtrn= result.uval;
	return True;
    }
    return False;
}

/***====================================================================***/

static Bool
HandleIncludeCompatMap(stmt,xkb,info,hndlr)
    IncludeStmt	*	  stmt;
    XkbDescPtr		  xkb;
    CompatInfo *	  info;
    void		(*hndlr)();
{
unsigned 	newMerge,tmp;
FILE	*	file;
XkbFile	*	rtrn;

    if (ProcessIncludeFile(stmt,XkmCompatMapIndex,&rtrn,&newMerge)) {
	CompatInfo 	myInfo;

	InitCompatInfo(info->dpy,&myInfo,xkb);
	myInfo.dflt= info->dflt;
	myInfo.act= info->act;
	(*hndlr)(rtrn,xkb,MergeOverride,&myInfo);
	/* 3/14/94 (ef) -- XXX! should free rtrn and contents here */
	if (newMerge==MergeReplace) {
	    ClearCompatInfo(info,xkb);
	    newMerge= MergeAugment;
	}
	info->errorCount+= myInfo.errorCount;
	if (myInfo.errorCount==0) {
	    register int i;
	    XkbSymInterpretPtr interp;

	    if (info->name==(Atom)NullStringToken)
		info->name= myInfo.name;
	    for (i=0,interp=myInfo.interps;i<myInfo.nInterps;i++,interp++) {
		if (!AddInterp(info,interp,newMerge,False))
		    info->errorCount++;
	    }
	}
	if (myInfo.modCompat!=NULL) {
	    ModCompatInfo *mc;
	    for (mc=myInfo.modCompat;mc!=NULL;mc=mc->next) {
		if (!AddModCompat(info,mc,newMerge,False))
		    info->errorCount++;
	    }
	}
	if (myInfo.leds!=NULL) {
	    XkbFileLEDInfo *led,*rtrn;
	    for (led=myInfo.leds;led!=NULL;led=led->next) {
		rtrn= AddIndicatorMap(led,info->leds,newMerge,False);
		if (rtrn!=NULL) 
		     info->leds= rtrn;
		else info->errorCount++;
		
	    }
	}
	ClearCompatInfo(&myInfo,xkb);
	return (info->errorCount==0);
    }
    info->errorCount+= 10;
    return False;
}

static LookupEntry useModMapValues[] = {
	{	"levelone",	1	},
	{	"level1",	1	},
	{	"anylevel",	0	},
	{	"any",		0	},
	{	NULL,		0	}
};

static int
SetInterpField(interp,xkb,field,arrayNdx,value,merge,info)
    XkbSymInterpretPtr	interp;
    XkbDescPtr		xkb;
    char *		field;
    ExprDef *		arrayNdx;
    ExprDef *		value;
    unsigned		merge;
    CompatInfo *	info;
{
int 		ok= 1;
ExprResult	tmp;

    if (uStrCaseCmp(field,"action")==0) {
	ok= HandleActionDef(value,xkb,&interp->act,merge,info->act);
    }
    else if (uStrCaseCmp(field,"virtualmodifier")==0) {
	ok= ResolveVirtualModifier(value,&tmp,&info->vmods);
	if (ok)
	    interp->virtual_mod= tmp.uval;
    }
    else if (uStrCaseCmp(field,"repeat")==0) {
	ok= ExprResolveBoolean(value,&tmp,NULL,NULL);
	if (ok) {
	    if (tmp.uval)	interp->flags|= XkbSI_Autorepeat;
	    else		interp->flags&= ~XkbSI_Autorepeat;
	}
    }
    else if (uStrCaseCmp(field,"locking")==0) {
	ok= ExprResolveBoolean(value,&tmp,NULL,NULL);
	if (ok) {
	    if (tmp.uval)	interp->flags|= XkbSI_LockingKey;
	    else		interp->flags&= ~XkbSI_LockingKey;
	}
    }
    else if ((uStrCaseCmp(field,"usemodmap")==0)||
	 	(uStrCaseCmp(field,"usemodmapmods")==0)) {
	ok= ExprResolveEnum(value,&tmp,useModMapValues);
	if (ok) {
	    if (tmp.uval)	interp->match|= XkbSI_LevelOneOnly;
	    else		interp->match&= ~XkbSI_LevelOneOnly;
	}
    }
    else {
	uError("Unknown field %s in a symbol interpretation\n",field);
	uAction("Definition ignored\n");
	ok= False;
    }
    return ok;
}

LookupEntry groupNames[]= {
	{	"group1",	0x01	},
	{	"group2",	0x02	},
	{	"group3",	0x04	},
	{	"group4",	0x08	},
	{	"group5",	0x10	},
	{	"group6",	0x20	},
	{	"group7",	0x40	},
	{	"group8",	0x80	},
	{	"none",		0x00	},
	{	"all",		0xff	},
	{	NULL,		0	}
};

static int
ReportNotArray(type,field,name)
    char *	type;
    char *	field;
    char *	name;
{
    uError("The %s %s field is not an array\n",type,field);
    uAction("Ignoring illegal assignment in definition of %s\n",name);
    return False;
}

static int
ReportBadType(type,field,name,wanted)
{
    uError("The %s %s field must be a %s\n",type,field,wanted);
    uAction("Ignoring illegal assignment in definition of %s\n",name);
    return False;
}

static int
HandleInterpVar(stmt,xkb,mergeMode,info)
    VarDef *		stmt;
    XkbDescPtr		xkb;
    unsigned 		mergeMode;
    CompatInfo *	info;
{
ExprResult	elem,field;
ExprDef *	arrayNdx;

    if (ExprResolveLhs(stmt->name,&elem,&field,&arrayNdx)==0) 
	return 0; /* internal error, already reported */
    if (elem.str&&(uStrCaseCmp(elem.str,"interpret")==0)) {
	return SetInterpField(&info->dflt,xkb,field.str,arrayNdx,stmt->value,
							     mergeMode,info);	
    }
    if (elem.str&&(uStrCaseCmp(elem.str,"indicator")==0)) {
	return SetIndicatorMapField(&info->ledDflt,xkb,field.str,arrayNdx,
						stmt->value,mergeMode);
    }
    return SetActionField(xkb,elem.str,field.str,arrayNdx,stmt->value,
							     &info->act);
}

static int
HandleInterpBody(def,xkb,interp,mergeMode,info)
    VarDef *		def;
    XkbDescPtr		xkb;
    XkbSymInterpretPtr	interp;
    unsigned		mergeMode;
    CompatInfo *	info;
{
int		ok= 1;
ExprResult	tmp,field;
ExprDef *	arrayNdx;

    for (;def!=NULL;def= (VarDef *)def->common.next) {
	if ((def->name)&&(def->name->type==ExprFieldRef)) {
	    ok= HandleInterpVar(def,xkb,mergeMode,info);
	    continue;
	}
	ok= ExprResolveLhs(def->name,&tmp,&field,&arrayNdx);
	if (ok)
	    ok= SetInterpField(interp,xkb,field.str,arrayNdx,def->value,
							 mergeMode,info);
    }
    return ok;
}

static int
HandleInterpDef(stmt,xkb,mergeMode,info)
    InterpDef *		stmt;
    XkbDescPtr		xkb;
    unsigned 		mergeMode;
    CompatInfo *	info;
{
unsigned		pred,mods,tmp;
XkbSymInterpretRec	interp;
 
    if (!ResolveStateAndPredicate(stmt->match,&pred,&mods)) {
	uError("Couldn't determine matching modifiers\n");
	uAction("Symbol interpretation ignored\n");
	return False;
    }
    if (stmt->merge!=MergeDefault) {
	if (stmt->merge==MergeReplace)
	     mergeMode= MergeOverride;
	else mergeMode= stmt->merge;
    }

    interp= info->dflt;
    interp.sym= stmt->sym;
    interp.match= (interp.match&(~XkbSI_OpMask))|(pred&XkbSI_OpMask);
    interp.mods= mods;
    if (!HandleInterpBody(stmt->def,xkb,&interp,mergeMode,info)) {
	info->errorCount++;
	return False;
    }

    if (!AddInterp(info,&interp,mergeMode,True)) {
	info->errorCount++;
	return False;
    }
    return True;
}

static int
HandleModCompatDef(def,xkb,mergeMode,info)
    ModCompatDef *	def;
    XkbDescPtr		xkb;
    unsigned 		mergeMode;
    CompatInfo *	info;
{
VarDef	*	body;
ExprResult	elem,field,val;
ExprDef *	arrayNdx;
ModCompatInfo	tmp;

    if (LookupVModIndex((XPointer)xkb,NullStringToken,def->modifier,TypeInt,
								     &elem)) {
	tmp.realMod= False;
    }
    else if (LookupModIndex((XPointer)xkb,NullStringToken,def->modifier,
								TypeInt,&elem)){
	tmp.realMod= True;
    }
    else {
	uError("Illegal identifier for modifier compatibility map\n");
	uAction("Map for non-modifier \"%s\" ignored\n",stText(def->modifier));
	return False;
    }
    tmp.name= def->modifier;
    tmp.ndx= elem.uval;
    tmp.groups= 0;
    if (tmp.realMod)
	 tmp.mods= (1<<tmp.ndx);
    else tmp.mods= 0;
    for (body=def->def;body!=NULL;body= (VarDef *)body->common.next) {
	if (!ExprResolveLhs(body->name,&elem,&field,&arrayNdx))
	    return False; /* internal error already reported */
	if (elem.str!=NULL) {
	    uError("Unknown element in a modifier compat map definition\n");
	    uAction("Illegal element %s ignored\n",elem.str);
	    continue;
	}
	if (field.str==NULL) {
	    uInternalError("Empty field in modifier compatibility map\n");
	    continue;
	}
	if (uStrCaseCmp(field.str,"groups")==0) {
	    if (arrayNdx!=NULL) {
		uError("Index for groups field of a mod compat map\n");
		uAction("Illegal array index ignored\n");
	    }
	    if (!ExprResolveMask(body->value,&elem,SimpleLookup,groupNames)) {
		uError("Expected a groups mask in map for %s modifier %s\n",
						(tmp.realMod?"real":"virtual"),
						stText(def->modifier));
		uAction("Ignoring illegal value for \"groups\"\n");
		continue;
	    }
	    tmp.groups= elem.uval;
	}
	else if ((uStrCaseCmp(field.str,"mods")==0)||
		 (uStrCaseCmp(field.str,"modifiers")==0)) {
	    if (arrayNdx!=NULL) {
		uError("Index for groups field of a mod compat map\n");
		uAction("Illegal array index ignored\n");
	    }
	    if (!ExprResolveModMask(body->value,&elem,NULL,NULL)) {
		uError("Expected a modifier mask in map for %s modifier %s\n",
						(tmp.realMod?"real":"virtual"),
						stText(def->modifier));
		uAction("Ignoring illegal value for \"modifiers\"\n");
		continue;
	    }
	    tmp.mods= elem.uval;
	}
	else {
	    uError("Illegal field in compatibility map for %s modifier %s\n",
					(tmp.realMod?"real":"virtual"),
					stText(def->modifier));
	    uAction("Values assigned to %s ignored\n",field.str);
	    continue;
	}
    }
    return AddModCompat(info,&tmp,mergeMode,True);
}

static void
HandleCompatMapFile(file,xkb,mergeMode,info)
    XkbFile		*file;
    XkbDescPtr	 	 xkb;
    unsigned		 mergeMode;
    CompatInfo	*info;
{
ParseCommon	*stmt;

    info->name= file->name;
    stmt= file->defs;
    while (stmt) {
	switch (stmt->stmtType) {
	    case StmtInclude:
		if (!HandleIncludeCompatMap((IncludeStmt *)stmt,xkb,info,
						HandleCompatMapFile))
		    info->errorCount++;
		break;
	    case StmtInterpDef:
		if (!HandleInterpDef((InterpDef *)stmt,xkb,mergeMode,info))
		    info->errorCount++;
		break;
	    case StmtModCompatDef:
		if (!HandleModCompatDef((ModCompatDef*)stmt,xkb,mergeMode,info))
		    info->errorCount++;
		break;
	    case StmtIndicatorMapDef:
		{
		    XkbFileLEDInfo *rtrn;
		    rtrn= HandleIndicatorMapDef((IndicatorMapDef *)stmt,xkb,
						&info->ledDflt,info->leds,
						mergeMode);
		    if (rtrn!=NULL)
		 	 info->leds= rtrn;
		    else info->errorCount++;
		}
		break;
	    case StmtVarDef:
		if (!HandleInterpVar((VarDef *)stmt,xkb,mergeMode,info))
		    info->errorCount++;
		break;
	    case StmtVModDef:
		if (!HandleVModDef((VModDef *)stmt,mergeMode,&info->vmods))
		    info->errorCount++;
		break;
	    case StmtKeycodeDef:
		uError("Interpretation files may not include other types\n");
		uAction("Ignoring definition of key name\n");
		info->errorCount++;
		break;
	    default:
		uInternalError(
			"Unexpected statement type %d in HandleCompatMapFile\n",
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

static void
CopyInterps(info,compat,needSymbol,pred)
    CompatInfo *	info;
    XkbCompatPtr	compat;
    Bool		needSymbol;
    unsigned		pred;
{
register int 		i;
XkbSymInterpretPtr	interp;

    for (i=0,interp=info->interps;i<info->nInterps;i++,interp++) {
	if (((interp->match&XkbSI_OpMask)!=pred)||
		(needSymbol&&(interp->sym==NoSymbol))||
		((!needSymbol)&&(interp->sym!=NoSymbol)))
	    continue;
	if (compat->num_si>=compat->size_si) {
	    uInternalError("No room to merge symbol interpretations\n");
	    uAction("Symbol interpretations lost\n");
	    return;
	}
	compat->sym_interpret[compat->num_si++]= *interp;
    }
    return;
}

Bool
CompileCompatMap(file,result,mergeMode)
    XkbFile *		file;
    XkbFileInfo *	result;
    unsigned	 	mergeMode;
{
int		errorCount= 0;
CompatInfo	info;
XkbDescPtr	xkb;

    xkb= &result->xkb;
    InitCompatInfo(result->dpy,&info,xkb);
    HandleCompatMapFile(file,xkb,mergeMode,&info);

    if (info.errorCount==0) {
	int size;
	if (!XkbAllocCompatMap(xkb,XkbAllCompatMask,info.nInterps)) {
	    uInternalError("Couldn't allocate compatibility map\n");
	    uAction("Exiting\n");
	    return False;
	}
	if (info.name!=NullStringToken) {
	    if (XkbAllocNames(xkb,XkbSemanticsNameMask))
		xkb->names->semantics= (Atom)info.name;
	    else {
		uInternalError("Couldn't allocate space for semantics name\n");
		uAction("Name \"%s\" (from %s) NOT assigned\n",scanFile,
							stText(info.name));
	    }
	}
	size= info.nInterps*sizeof(XkbSymInterpretRec);
	if (size>0) {
	    CopyInterps(&info,xkb->compat,True,XkbSI_Exactly);
	    CopyInterps(&info,xkb->compat,True,XkbSI_AllOf|XkbSI_NoneOf);
	    CopyInterps(&info,xkb->compat,True,XkbSI_AnyOf);
	    CopyInterps(&info,xkb->compat,True,XkbSI_AnyOfOrNone);
	    CopyInterps(&info,xkb->compat,False,XkbSI_Exactly);
	    CopyInterps(&info,xkb->compat,False,XkbSI_AllOf|XkbSI_NoneOf);
	    CopyInterps(&info,xkb->compat,False,XkbSI_AnyOf);
	    CopyInterps(&info,xkb->compat,False,XkbSI_AnyOfOrNone);
	}
	if (info.modCompat!=NULL) {
	    ModCompatInfo *	cm;
	    for (cm= info.modCompat;cm!=NULL;cm= cm->next) {
		if (cm->realMod) {
		    xkb->compat->real_mod_compat[cm->ndx].mods= cm->mods;
		    xkb->compat->real_mod_compat[cm->ndx].groups= cm->groups;
		}
		else {
		    xkb->compat->vmod_compat[cm->ndx].mods= cm->mods;
		    xkb->compat->vmod_compat[cm->ndx].groups= cm->groups;
		}
	    }
	}
	if (info.leds!=NULL) {
	    if (!CopyIndicatorMapDefs(result,info.leds))
		info.errorCount++;
	    info.leds= NULL;
	}
	ClearCompatInfo(&info,xkb);
	return True;
    }
    if (info.interps!=NULL)
	free(info.interps);
    return False;
}
