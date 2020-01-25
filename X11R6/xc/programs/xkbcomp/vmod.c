/* $XConsortium: vmod.c,v 1.3 94/04/08 15:25:30 erik Exp $ */
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
#define	DEBUG_VAR_NOT_LOCAL
#define	DEBUG_VAR debugFlags
#include <stdio.h>
#include "xkbcomp.h"
#include "xkbfile.h"
#include "tokens.h"
#include "expr.h"
#include "misc.h"

#include <X11/extensions/XKB.h>
#include <X11/extensions/XKBstr.h>

#include "vmod.h"

void
InitVModInfo(info,xkb)
    VModInfo *	info;
    XkbDescPtr	xkb;
{
    ClearVModInfo(info,xkb);
    info->errorCount= 0;
    return;
}

void
ClearVModInfo(info,xkb)
    VModInfo *	info;
    XkbDescPtr	xkb;
{
register int i;

    XkbAllocNames(xkb,XkbVirtualModNamesMask);
    XkbAllocServerMap(xkb,XkbVirtualModsMask,0);
    info->xkb= xkb;
    info->newlyDefined= info->defined= info->available= 0;
    if (xkb && xkb->names) {
	register int bit;
	for (i=0,bit=1;i<XkbNumVirtualMods;i++,bit<<=1) {
	    if (xkb->names->vmods[i]!=(Atom)NullStringToken)
		info->defined|= bit;
	}
    }
    return;
}

/***====================================================================***/

Bool
HandleVModDef(stmt,mergeMode,info)
    VModDef *	stmt;
    unsigned	mergeMode;
    VModInfo * 	info;
{
register int 	i,bit,nextFree;
ExprResult 	mod;
XkbServerMapPtr	srv;
XkbNamesPtr	names;
    
    srv= info->xkb->server;
    names= info->xkb->names;
    for (i=0,bit=1,nextFree= -1;i<XkbNumVirtualMods;i++,bit<<=1) {
	if (info->defined&bit) {
	    if (names->vmods[i]==(stmt->name)) {	/* already defined */
		info->available|= bit;
		if (stmt->value==NULL)
		    return True;
		else {
		    char *str1,*str2;
		    if (!ExprResolveModIndex(stmt->value,&mod,NULL,NULL)) {
			str1= stText(stmt->name);
			uAction("Declaration of %s ignored\n",str1);
			return False;
		    }
		    if (mod.uval==srv->vmods[i])
			return True;

		    str1= stText(stmt->name);
		    uWarning("Virtual modifier %s multiply defined\n",str1);
		    str1= XkbModIndexText(srv->vmods[i],XkbCFile);
		    if (mergeMode==MergeOverride) {
			str2= str1;
			str1= XkbModIndexText(mod.uval,XkbCFile);
		    }
		    uAction("Using %s, ignoring %s\n",str1,str2);
		    if (mergeMode==MergeOverride)
			srv->vmods[i]= mod.uval;
		    return True;
		}
	    }
	}
	else if (nextFree<0)
	    nextFree= i;
    }
    if (nextFree<0) {
	uError("Too many virtual modifiers defined (maximum 16)\n");
	uAction("Exiting\n");
	return False;
    }
    info->defined|= (1<<nextFree);
    info->newlyDefined|= (1<<nextFree);
    info->available|= (1<<nextFree);
    names->vmods[nextFree]= stmt->name;
    if (stmt->value==NULL)
	return True;
    if (ExprResolveModIndex(stmt->value,&mod,NULL,NULL)) {
	srv->vmods[nextFree]= mod.uval;
	return True;
    }
    uAction("Declaration of %s ignored\n",stText(stmt->name));
    return False;
}

int
LookupVModIndex(priv,elem,field,type,val_rtrn)
    XPointer 		priv;
    StringToken		elem;
    StringToken		field;
    unsigned		type;
    ExprResult *	val_rtrn;
{
register int	i;
register char *	fieldStr;
register char *	modStr;
XkbDescPtr	xkb;

    xkb= (XkbDescPtr)priv;
    if ((xkb==NULL)||(xkb->names==NULL)||(elem!=NullStringToken)||
		     				(type!=TypeInt)) {
	return False;
    }
    fieldStr= stGetString(field);
    if (fieldStr==NULL)
	return False;
    for (i=0;i<XkbNumVirtualMods;i++) {
	modStr= stGetString(xkb->names->vmods[i]);
	if ((modStr!=NULL)&&(uStrCaseCmp(fieldStr,modStr)==0)) {
	    val_rtrn->uval= i;
	    return True;
	}
    }
    return False;
}

int
LookupVModMask(priv,elem,field,type,val_rtrn)
    XPointer 		priv;
    StringToken		elem;
    StringToken		field;
    unsigned		type;
    ExprResult *	val_rtrn;
{
    if (LookupVModIndex(priv,elem,field,type,val_rtrn)) {
	register unsigned ndx= val_rtrn->uval;
	val_rtrn->uval= (1<<(8+ndx));
	return True;
    }
    return False;
}

int
FindKeypadVMod(xkb)
    XkbDescPtr	xkb;
{
StringToken name;
ExprResult rtrn;

    name= stGetToken("NumLock");
    if ((xkb)&&
	LookupVModIndex((XPointer)xkb,NullStringToken,name,TypeInt,&rtrn)) {
	return rtrn.ival;
    }
    return -1;
}

Bool
ResolveVirtualModifier(def,val_rtrn,info)
    ExprDef *		def;
    ExprResult *	val_rtrn;
    VModInfo *		info;
{
XkbNamesPtr	names;

    names= info->xkb->names;
    if (def->op==ExprIdent) {
	register int i,bit;
	for (i=0,bit=1;i<XkbNumVirtualMods;i++,bit<<=1) {
	    if ((info->available&bit)&&
		(stCaseCompare(names->vmods[i],def->value.str)==Equal)) {
		val_rtrn->uval= i;
		return True;
	    }
	}
    }
    if (ExprResolveInteger(def,val_rtrn,NULL,NULL)) {
	if (val_rtrn->uval<XkbNumVirtualMods)
	    return True;
	uError("Illegal virtual modifier %d (must be 0..15 inclusive)\n",
							val_rtrn->uval);
    }
    return False;
}
