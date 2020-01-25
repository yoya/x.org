/* $XConsortium: xkmout.c,v 1.1 94/04/08 15:50:38 erik Exp $ */
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

#include <stdio.h>
#include <ctype.h>
#ifndef X_NOT_STDC_ENV
#include <stdlib.h>
#endif
#include <X11/Xlib.h>
#include <X11/XKBlib.h>

#define	DEBUG_VAR_NOT_LOCAL
#define	DEBUG_VAR debugFlags
#include "utils.h"
#include "strtbl.h"

#include "XKMformat.h"
#include "xkbfile.h"

typedef struct _XkmInfo {
     unsigned short	bound_vmods;
     unsigned short	named_vmods;
     unsigned char	num_bound;
     unsigned char	compat_mods;
     unsigned short	compat_vmods;
     unsigned short	num_mod_compat;
     unsigned short	num_leds;
} XkmInfo;

/***====================================================================***/

#define	xkmPutCARD8(f,v)	(putc(v,f),1)

int
xkmPutCARD16(file,val)
    FILE *		file;
    unsigned		val;
{
CARD16	tmp= val;

    fwrite(&tmp,2,1,file);
    return 2;
}

int
xkmPutCARD32(file,val)
    FILE *		file;
    unsigned long	val;
{
CARD32 tmp= val;

    fwrite(&tmp,4,1,file);
    return 4;
}

int
xkmPutPadding(file,pad)
    FILE *		file;
    unsigned		pad;
{
int	i;
    for (i=0;i<pad;i++) {
	putc('\0',file);
    }
    return pad;
}

int
xkmPutCountedBytes(file,ptr,count)
    FILE *		file;
    char *		ptr;
    unsigned		count;
{
register int nOut,pad;
    if (count==0)
	return xkmPutCARD32(file,(unsigned long)0);

    xkmPutCARD16(file,count);
    nOut= fwrite(ptr,1,count,file);
    if (nOut<0)
	return 2;
    nOut= count+2;
    pad= XkbPaddedSize(nOut)-nOut;
    if (pad)
	xkmPutPadding(file,pad);
    return nOut+pad;
}

unsigned
xkmSizeCountedString(str)
    char *	str;
{
    if (str==NULL)
	return 4;
    return XkbPaddedSize(strlen(str)+2);
}

int
xkmPutCountedString(file,str)
    FILE *	file;
    char *	str;
{
    if (str==NULL)
	 return xkmPutCARD32(file,(unsigned long)0);
    return xkmPutCountedBytes(file,str,strlen(str));
}

/***====================================================================***/

static unsigned
SizeXKMVirtualMods(result,info)
    XkbFileInfo *	result;
    XkmInfo *		info;
{
Display *	dpy;
XkbDescPtr	xkb;
unsigned	nBound,bound;
unsigned	nNamed,named,szNames;
register unsigned	i,bit;

    dpy= result->dpy;
    dpy= result->dpy;
    xkb= &result->xkb;
    if ((!xkb)||(!xkb->names)||(!xkb->server)) {
	uInternalError("Vmod mod bindings or names missing in SizeXKMVirtualMods\n");
	return 0;
    }
    bound=named=0;
    for (i=nBound=nNamed=szNames=0,bit=1;i<XkbNumVirtualMods;i++,bit<<=1) {
	if (xkb->server->vmods[i]!=XkbNoModifier) {
	    bound|= bit;
	    nBound++;
	}
	if (xkb->names->vmods[i]!=None) {
	    register char *name;
	    named|= bit;
	    name= XkbAtomGetString(dpy,xkb->names->vmods[i]);
	    szNames+= xkmSizeCountedString(name);
	    nNamed++;
	}
    }
    info->num_bound= nBound;
    info->bound_vmods= bound;
    info->named_vmods= named;
    if ((nBound==0)&&(nNamed==0))
	return 0;
    return 4+XkbPaddedSize(nBound)+szNames;
}

static Bool
WriteXKMVirtualMods(file,result,info)
    FILE *		file;
    XkbFileInfo *	result;
    XkmInfo *		info;
{
register unsigned int i,bit;
XkbDescPtr	xkb;
Display *	dpy;

    dpy= result->dpy;
    xkb= &result->xkb;
    xkmPutCARD16(file,info->bound_vmods);
    xkmPutCARD16(file,info->named_vmods);
    for (i=0,bit=1;i<XkbNumVirtualMods;i++,bit<<=1) {
	if (info->bound_vmods&bit)
	    xkmPutCARD8(file,xkb->server->vmods[i]);
    }
    if ((i= XkbPaddedSize(info->num_bound)-info->num_bound)>0)
	xkmPutPadding(file,i);
    for (i=0,bit=1;i<XkbNumVirtualMods;i++,bit<<=1) {
	if (info->named_vmods&bit) {
	    register char *name;
	    name= XkbAtomGetString(dpy,xkb->names->vmods[i]);
	    xkmPutCountedString(file,name);
	}
    }
    return True;
}

/***====================================================================***/

static unsigned
SizeXKMKeycodes(result,info)
    XkbFileInfo *	result;
    XkmInfo *		info;
{
XkbDescPtr	xkb;
Atom		kcName;
int		size;
Display *	dpy;

    dpy= result->dpy;
    xkb= &result->xkb;
    if ((!xkb)||(!xkb->names)||(!xkb->names->keys)) {
	uInternalError("Key names not defined in SizeXKMKeycodes\n");
	return 0;
    }
    kcName= xkb->names->keycodes;
    size= 4;	/* min and max keycode */
    size+= xkmSizeCountedString(XkbAtomGetString(dpy,kcName));
    size+= XkbNumKeys(xkb)*sizeof(XkbKeyNameRec);
    return size;
}

static Bool
WriteXKMKeycodes(file,result,info)
    FILE *		file;
    XkbFileInfo *	result;
    XkmInfo *		info;
{
XkbDescPtr	xkb;
Atom	 	kcName;
char 		*start;
register int	 i;
Display *	dpy;

    dpy= result->dpy;
    xkb= &result->xkb;
    kcName= xkb->names->keycodes;
    start= xkb->names->keys[xkb->min_key_code].name;
    
    xkmPutCARD8(file,xkb->min_key_code);
    xkmPutCARD8(file,xkb->max_key_code);
    xkmPutPadding(file,2);
    xkmPutCountedString(file,stGetString(kcName));
    fwrite(start,sizeof(XkbKeyNameRec),XkbNumKeys(xkb),file);
    return True;
}

/***====================================================================***/

static unsigned
SizeXKMKeyTypes(result,info)
    XkbFileInfo *	result;
    XkmInfo *		info;
{
register unsigned	i,n,size;
XkbKeyTypePtr		type;
XkbDescPtr		xkb;
Display *		dpy;

    dpy= result->dpy;
    xkb= &result->xkb;
    if ((!xkb)||(!xkb->map)||(!xkb->map->types)) {
	uInternalError("Key types not defined in SizeXKBKeyTypes\n");
	return 0;
    }
    size= 4;	/* room for # of key types + padding */
    for (i=0,type=xkb->map->types;i<xkb->map->num_types;i++,type++) {
	size+= SIZEOF(xkmKeyTypeDesc);
	size+= SIZEOF(xkmKTMapEntryDesc)*type->map_count;
	size+= xkmSizeCountedString(stGetString(type->name));
	if (type->preserve)
	    size+= SIZEOF(xkmKTPreserveDesc)*type->map_count;
	if (type->lvl_names) {
	    Atom *names;
	    names= type->lvl_names;
	    for (n=0;n<(unsigned)type->group_width;n++) {
		size+= xkmSizeCountedString(XkbAtomGetString(dpy,names[n]));
	    }
	}
    }
    return size;
}

static Bool
WriteXKMKeyTypes(file,result,info)
    FILE *	file;
    XkbFileInfo *	result;
    XkmInfo *	info;
{
register unsigned	i,n;
XkbDescPtr		xkb;
XkbKeyTypePtr		type;
xkmKeyTypeDesc		wire;
XkbKTMapEntryPtr	entry;
xkmKTMapEntryDesc	wire_entry;
Atom *			names;
Display *	dpy;

    dpy= result->dpy;
    xkb= &result->xkb;
    xkmPutCARD16(file,xkb->map->num_types);
    xkmPutPadding(file,2);
    type= xkb->map->types;
    for (i=0;i<xkb->map->num_types;i++,type++) {
	wire.realMods= type->real_mods;
	wire.groupWidth= type->group_width;
	wire.virtualMods= type->vmods;
	wire.nMapEntries= type->map_count;
	wire.preserve= (type->preserve!=NULL);
	if (type->lvl_names!=NULL)
	     wire.nLevelNames= type->group_width;
	else wire.nLevelNames= 0;
	fwrite(&wire,SIZEOF(xkmKeyTypeDesc),1,file);
	for (n=0,entry= type->map;n<type->map_count;n++,entry++) {
	    wire_entry.level= entry->level;
	    wire_entry.realMods= entry->real_mods;
	    wire_entry.virtualMods= entry->vmods;
	    fwrite(&wire_entry,SIZEOF(xkmKTMapEntryDesc),1,file);
	}
	xkmPutCountedString(file,XkbAtomGetString(dpy,type->name));
	if (type->preserve) {
	    xkmKTPreserveDesc	p_entry;
	    XkbKTPreservePtr	pre;
	    for (n=0,pre=type->preserve;n<type->map_count;n++,pre++) {
		p_entry.realMods= pre->real_mods;
		p_entry.virtualMods= pre->vmods;
		fwrite(&p_entry,SIZEOF(xkmKTPreserveDesc),1,file);
	    }
	}
	if (type->lvl_names!=NULL) {
	    names= type->lvl_names;
	    for (n=0;n<wire.nLevelNames;n++) {
		xkmPutCountedString(file,stGetString(names[n]));
	    }
	}
    }
    return True;
}

/***====================================================================***/

static unsigned
SizeXKMCompatMap(result,info)
    XkbFileInfo *	result;
    XkmInfo *		info;
{
XkbDescPtr	xkb;
char	*name;
int	size;
register int	i;
unsigned mods,nMods;
Display *	dpy;

    dpy= result->dpy;
    xkb= &result->xkb;
    if ((!xkb)||(!xkb->compat)||(!xkb->compat->sym_interpret)) {
	uInternalError("Compat map not defined in SizeXKMCompatMap\n");
	return 0;
    }
    if (xkb->names)	name= XkbAtomGetString(dpy,xkb->names->semantics);
    else		name= NULL;

    nMods= 0;
    for (i=mods=0;i<XkbNumModifiers;i++) {
	if ((xkb->compat->real_mod_compat[i].groups!=0)||
			(xkb->compat->real_mod_compat[i].mods!=(1<<i))) {
	    mods|= (1<<i);
	    nMods++;
	}
    }
    info->compat_mods= mods;
    for (i=mods=0;i<XkbNumVirtualMods;i++) {
	if ((xkb->compat->vmod_compat[i].groups!=0)||
				(xkb->compat->vmod_compat[i].mods!=0)) {
	    mods|= (1<<i);
	    nMods++;
	}
    }
    info->compat_vmods= mods;
    info->num_mod_compat= nMods;
    size= 4+xkmSizeCountedString(name);
    size+= (SIZEOF(xkmSymInterpretDesc)*xkb->compat->num_si);
    size+= 4+XkbPaddedSize(2*nMods);
    return size;
}

static Bool
WriteXKMCompatMap(file,result,info)
    FILE *	file;
    XkbFileInfo *	result;
    XkmInfo *	info;
{
register unsigned	i;
char *			name;
XkbDescPtr		xkb;
XkbSymInterpretPtr	interp;
xkmSymInterpretDesc	wire;
Display *	dpy;

    dpy= result->dpy;
    xkb= &result->xkb;
    if (xkb->names)	name= XkbAtomGetString(dpy,xkb->names->semantics);
    else		name= NULL;
    xkmPutCARD16(file,xkb->compat->num_si);
    xkmPutPadding(file,2);
    xkmPutCountedString(file,name);
    interp= xkb->compat->sym_interpret;
    for (i=0;i<xkb->compat->num_si;i++,interp++) {
	wire.sym= interp->sym;
	wire.mods= interp->mods;
	wire.match= interp->match;
	wire.virtualMod= interp->virtual_mod;
	wire.flags= interp->flags;
	wire.actionType= interp->act.type;
	wire.actionData[0]= interp->act.data[0];
	wire.actionData[1]= interp->act.data[1];
	wire.actionData[2]= interp->act.data[2];
	wire.actionData[3]= interp->act.data[3];
	wire.actionData[4]= interp->act.data[4];
	wire.actionData[5]= interp->act.data[5];
	wire.actionData[6]= interp->act.data[6];
	fwrite(&wire,SIZEOF(xkmSymInterpretDesc),1,file);
    }
    xkmPutCARD8(file,info->compat_mods);
    xkmPutPadding(file,1);
    xkmPutCARD16(file,info->compat_vmods);
    if (info->compat_mods) {
	register unsigned bit;
	for (i=0,bit=1;i<XkbNumModifiers;i++,bit<<=1) {
	    if (info->compat_mods&bit) {
		xkmPutCARD8(file,xkb->compat->real_mod_compat[i].mods);
		xkmPutCARD8(file,xkb->compat->real_mod_compat[i].groups);
	    }
	}
    }
    if (info->compat_vmods) {
	register unsigned bit;
	for (i=0,bit=1;i<XkbNumVirtualMods;i++,bit<<=1) {
	    if (info->compat_vmods&bit) {
		xkmPutCARD8(file,xkb->compat->vmod_compat[i].mods);
		xkmPutCARD8(file,xkb->compat->vmod_compat[i].groups);
	    }
	}
    }
    i= XkbPaddedSize((info->num_mod_compat*2))-(info->num_mod_compat*2);
    if (i>0)
	xkmPutPadding(file,i);
    return True;
}

/***====================================================================***/

static unsigned
SizeXKMSymbols(result,info)
    XkbFileInfo *	result;
    XkmInfo *		info;
{
Display *	dpy;
XkbDescPtr	xkb;
unsigned 	size;
register int	i,nSyms;
char *		name;

    dpy= result->dpy;
    xkb= &result->xkb;
    if ((!xkb)||(!xkb->map)||((!xkb->map->syms))) {
	uInternalError("Symbols not defined in SizeXKMSymbols\n");
	return 0;
    }
    if (xkb->names && (xkb->names->symbols!=None))
	 name= XkbAtomGetString(result->dpy,xkb->names->symbols);
    else name= NULL;
    size= 4;	/* min and max keycode */
    size+= xkmSizeCountedString(name);
    for (i=xkb->min_key_code;i<=(int)xkb->max_key_code;i++) {
	nSyms= XkbKeyNumSyms(xkb,i);
	size+= SIZEOF(xkmKeySymMapDesc)+(nSyms*4);
	if (xkb->server) {
	    if (xkb->server->explicit[i]&XkbExplicitKeyTypeMask) {
		char *name;
		name= XkbAtomGetString(dpy,XkbKeyKeyType(xkb,i)->name);
		if (name!=NULL)
		     size+= xkmSizeCountedString(name);
	    }
	    if (XkbKeyHasActions(xkb,i))
		size+= nSyms*SIZEOF(xkmActionDesc);
	    if (xkb->server->behaviors[i].type!=XkbKB_Default)
		size+= SIZEOF(xkmBehaviorDesc);
	}
    }
    return size;
}

static Bool
WriteXKMSymbols(file,result,info)
    FILE *		file;
    XkbFileInfo *	result;
    XkmInfo *		info;
{
Display *	dpy;
XkbDescPtr		xkb;
register int		i,n;
xkmKeySymMapDesc 	wireMap;
char *			name;

    dpy= result->dpy;
    xkb= &result->xkb;
    if (xkb->names && (xkb->names->symbols!=None))
	 name= XkbAtomGetString(result->dpy,xkb->names->symbols);
    else name= NULL;
    xkmPutCARD8(file,xkb->min_key_code);
    xkmPutCARD8(file,xkb->max_key_code);
    xkmPutPadding(file,2);
    xkmPutCountedString(file,name);
    for (i=xkb->min_key_code;i<=(int)xkb->max_key_code;i++)  {
	register char *typeName;
	wireMap.group_width= XkbKeyGroupWidth(xkb,i);
	wireMap.num_groups= XkbKeyNumGroups(xkb,i);
	if (result->defined&XkbModMapDefined)
	     wireMap.modifier_map= result->modmap[i];
	else wireMap.modifier_map= 0;
	wireMap.flags= 0;
	typeName= NULL;
	if (xkb->server) {
	    if (xkb->server->explicit[i]&XkbExplicitKeyTypeMask) {
		typeName= XkbAtomGetString(dpy,XkbKeyKeyType(xkb,i)->name);
		if (typeName!=NULL)
		    wireMap.flags|= XkmKeyHasType;
	    }
	    if (XkbKeyHasActions(xkb,i))
		wireMap.flags|= XkmKeyHasActions;
	    if (xkb->server->behaviors[i].type!=XkbKB_Default)
		wireMap.flags|= XkmKeyHasBehavior;
	    if ((result->defined&XkbPerKeyRepeatDefined)&&
		(xkb->server->explicit[i]&XkbExplicitAutorepeatMask)) {
		if (result->repeat[(i/8)]&(1<<(i%8)))
		     wireMap.flags|= XkmRepeatingKey;
		else wireMap.flags|= XkmNonRepeatingKey;
	    }
	}
	fwrite(&wireMap,SIZEOF(xkmKeySymMapDesc),1,file);
	if (typeName!=NULL)
	    xkmPutCountedString(file,typeName);
	if (wireMap.num_groups>0) {
	    KeySym	*sym;
	    sym= XkbKeySymsPtr(xkb,i);
	    for (n=XkbKeyNumSyms(xkb,i);n>0;n--,sym++) {
		xkmPutCARD32(file,(CARD32)*sym);
	    }
	    if (wireMap.flags&XkmKeyHasActions) {
		XkbAction *	act;
		act= XkbKeyActionsPtr(xkb,i);
		for (n=XkbKeyNumActions(xkb,i);n>0;n--,act++) {
		    fwrite(&act,SIZEOF(xkmActionDesc),1,file);
		}
	    }
	}
	if (wireMap.flags&XkmKeyHasBehavior) {
	    xkmBehaviorDesc	b;
	    b.type= xkb->server->behaviors[i].type;
	    b.data= xkb->server->behaviors[i].data;
	    fwrite(&b,SIZEOF(xkmBehaviorDesc),1,file);
	}
    }
    return True;
}

/***====================================================================***/

static unsigned
SizeXKMIndicators(result,info)
    XkbFileInfo *	result;
    XkmInfo *		info;
{
Display *		dpy;
XkbDescPtr		xkb;
unsigned 		size;
register unsigned	i,nLEDs;

    dpy= result->dpy;
    xkb= &result->xkb;
    if ((!xkb)||((!xkb->indicators)&&(!result->leds))) {
	uInternalError("Indicators not defined in SizeXKMIndicators\n");
	return 0;
    }
    nLEDs=0;
    size= 4;	/* number of indicators */
    if (xkb->indicators!=NULL) {
	for (i=0;i<XkbNumIndicators;i++) {
	    XkbIndicatorMapPtr map= &xkb->indicators->maps[i];
	    if ((map->flags!=0)||(map->which_groups!=0)||(map->groups!=0)||
		(map->which_mods!=0)||(map->real_mods!=0)||(map->vmods!=0)||
		(map->ctrls!=0)) {
		char *name;
		if (xkb->names && xkb->names->indicators[i]!=None) {
		     name= XkbAtomGetString(result->dpy,
					    xkb->names->indicators[i]);
		}
		else name= NULL;
		size+= xkmSizeCountedString(name);
		size+= SIZEOF(xkmIndicatorMapDesc);
		nLEDs++;
	    }
	}
    }
    if (result->leds!=NULL) {
	XkbFileLEDInfo *led;
	for (led=result->leds;led!=NULL;led=led->next) {
	    size+=xkmSizeCountedString(XkbAtomGetString(result->dpy,led->name));
	    size+= SIZEOF(xkmIndicatorMapDesc);
	    nLEDs++;
	}
    }
    info->num_leds= nLEDs;
    return size;
}

static Bool
WriteXKMIndicators(file,result,info)
    FILE *		file;
    XkbFileInfo *	result;
    XkmInfo *		info;
{
Display *		dpy;
XkbDescPtr		xkb;
register unsigned	i;
xkmIndicatorMapDesc	wire;

    dpy= result->dpy;
    xkb= &result->xkb;
    xkmPutCARD8(file,info->num_leds);
    xkmPutPadding(file,3);
    if (xkb->indicators!=NULL) {
	for (i=0;i<XkbNumIndicators;i++) {
	    XkbIndicatorMapPtr map= &xkb->indicators->maps[i];
	    if ((map->flags!=0)||(map->which_groups!=0)||(map->groups!=0)||
		(map->which_mods!=0)||(map->real_mods!=0)||(map->vmods!=0)||
		(map->ctrls!=0)) {
		char *name;
		if (xkb->names && xkb->names->indicators[i]!=None) {
		     name= XkbAtomGetString(result->dpy,
					    xkb->names->indicators[i]);
		}
		else name= NULL;
		xkmPutCountedString(file,name);
		wire.indicator= i+1;
		wire.flags= map->flags;
		wire.which_mods= map->which_mods;
		wire.real_mods= map->real_mods;
		wire.vmods= map->vmods;
		wire.which_groups= map->which_groups;
		wire.groups= map->groups;
		wire.ctrls= map->ctrls;
		fwrite(&wire,SIZEOF(xkmIndicatorMapDesc),1,file);
	    }
	}
    }
    if (result->leds!=NULL) {
	XkbFileLEDInfo *led;
	for (led=result->leds;led!=NULL;led=led->next) {
	    xkmPutCountedString(file,XkbAtomGetString(result->dpy,led->name));
	    wire.indicator= XkmIndicatorUnbound;
	    wire.flags= led->flags;
	    wire.which_mods= led->which_mods;
	    wire.real_mods= led->real_mods;
	    wire.vmods= led->vmods;
	    wire.which_groups= led->which_groups;
	    wire.groups= led->groups;
	    wire.ctrls= led->ctrls;
	    fwrite(&wire,SIZEOF(xkmIndicatorMapDesc),1,file);
	}
    }
    return True;
}

/***====================================================================***/

static int
GetXKMSemanticsTOC(result,info,max_toc,toc_rtrn)
    XkbFileInfo *	result;
    XkmInfo *		info;
    int			max_toc;
    xkmSectionInfo *	toc_rtrn;
{
int	num_toc;
int	total_size;

    total_size= num_toc=0;
    toc_rtrn[num_toc].size= SizeXKMVirtualMods(result,info);
    if (toc_rtrn[num_toc].size>0) {
	toc_rtrn[num_toc].type= XkmVirtualModsIndex;
	toc_rtrn[num_toc].format= MSBFirst;
	toc_rtrn[num_toc].size+= SIZEOF(xkmSectionInfo);
	toc_rtrn[num_toc].offset= total_size;
	total_size+= toc_rtrn[num_toc].size;
	num_toc++;
    }
    toc_rtrn[num_toc].size= SizeXKMKeyTypes(result,info);
    if (toc_rtrn[num_toc].size>0) {
	toc_rtrn[num_toc].type= XkmTypesIndex;
	toc_rtrn[num_toc].format= MSBFirst;
	toc_rtrn[num_toc].size+= SIZEOF(xkmSectionInfo);
	toc_rtrn[num_toc].offset= total_size;
	total_size+= toc_rtrn[num_toc].size;
	num_toc++;
    }
    toc_rtrn[num_toc].size= SizeXKMCompatMap(result,info);
    if (toc_rtrn[num_toc].size>0) {
	toc_rtrn[num_toc].type= XkmCompatMapIndex;
	toc_rtrn[num_toc].format= MSBFirst;
	toc_rtrn[num_toc].size+= SIZEOF(xkmSectionInfo);
	toc_rtrn[num_toc].offset= total_size;
	total_size+= toc_rtrn[num_toc].size;
	num_toc++;
    }
    if (result->defined&XkbIndicatorMapsDefined) {
	toc_rtrn[num_toc].size= SizeXKMIndicators(result,info);
	if (toc_rtrn[num_toc].size>0) {
	    toc_rtrn[num_toc].type= XkmIndicatorsIndex;
	    toc_rtrn[num_toc].format= MSBFirst;
	    toc_rtrn[num_toc].size+= SIZEOF(xkmSectionInfo);
	    toc_rtrn[num_toc].offset= total_size;
	    total_size+= toc_rtrn[num_toc].size;
	    num_toc++;
	}
    }
    return num_toc;
}

static int
GetXKMLayoutTOC(result,info,max_toc,toc_rtrn)
    XkbFileInfo *	result;
    XkmInfo *		info;
    int			max_toc;
    xkmSectionInfo *	toc_rtrn;
{
int	num_toc;
int	total_size;

    total_size= num_toc=0;
    toc_rtrn[num_toc].size= SizeXKMVirtualMods(result,info);
    if (toc_rtrn[num_toc].size>0) {
	toc_rtrn[num_toc].type= XkmVirtualModsIndex;
	toc_rtrn[num_toc].format= MSBFirst;
	toc_rtrn[num_toc].size+= SIZEOF(xkmSectionInfo);
	toc_rtrn[num_toc].offset= total_size;
	total_size+= toc_rtrn[num_toc].size;
	num_toc++;
    }
    toc_rtrn[num_toc].size= SizeXKMKeycodes(result,info);
    if (toc_rtrn[num_toc].size>0) {
	toc_rtrn[num_toc].type= XkmKeyNamesIndex;
	toc_rtrn[num_toc].format= MSBFirst;
	toc_rtrn[num_toc].size+= SIZEOF(xkmSectionInfo);
	toc_rtrn[num_toc].offset= total_size;
	total_size+= toc_rtrn[num_toc].size;
	num_toc++;
    }
#ifdef NOTYET
    toc_rtrn[num_toc].size= SizeXKMGeometry(result,info);
    if (toc_rtrn[num_toc].size>0) {
	toc_rtrn[num_toc].type= XkmGeometryIndex;
	toc_rtrn[num_toc].format= MSBFirst;
	toc_rtrn[num_toc].size+= SIZEOF(xkmSectionInfo);
	toc_rtrn[num_toc].offset= total_size;
	total_size+= toc_rtrn[num_toc].size;
	num_toc++;
    }
#endif
    toc_rtrn[num_toc].size= SizeXKMSymbols(result,info);
    if (toc_rtrn[num_toc].size>0) {
	toc_rtrn[num_toc].type= XkmSymbolsIndex;
	toc_rtrn[num_toc].format= MSBFirst;
	toc_rtrn[num_toc].size+= SIZEOF(xkmSectionInfo);
	toc_rtrn[num_toc].offset= total_size;
	total_size+= toc_rtrn[num_toc].size;
	num_toc++;
    }
    if (result->defined&XkbIndicatorMapsDefined) {
	toc_rtrn[num_toc].size= SizeXKMIndicators(result,info);
	if (toc_rtrn[num_toc].size>0) {
	    toc_rtrn[num_toc].type= XkmIndicatorsIndex;
	    toc_rtrn[num_toc].format= MSBFirst;
	    toc_rtrn[num_toc].size+= SIZEOF(xkmSectionInfo);
	    toc_rtrn[num_toc].offset= total_size;
	    total_size+= toc_rtrn[num_toc].size;
	    num_toc++;
	}
    }
    return num_toc;
}

static int
GetXKMKeymapTOC(result,info,max_toc,toc_rtrn)
    XkbFileInfo *	result;
    XkmInfo *		info;
    int			max_toc;
    xkmSectionInfo *	toc_rtrn;
{
int	num_toc;
int	total_size;

    total_size= num_toc=0;
    toc_rtrn[num_toc].size= SizeXKMVirtualMods(result,info);
    if (toc_rtrn[num_toc].size>0) {
	toc_rtrn[num_toc].type= XkmVirtualModsIndex;
	toc_rtrn[num_toc].format= MSBFirst;
	toc_rtrn[num_toc].size+= SIZEOF(xkmSectionInfo);
	toc_rtrn[num_toc].offset= total_size;
	total_size+= toc_rtrn[num_toc].size;
	num_toc++;
    }
    toc_rtrn[num_toc].size= SizeXKMKeycodes(result,info);
    if (toc_rtrn[num_toc].size>0) {
	toc_rtrn[num_toc].type= XkmKeyNamesIndex;
	toc_rtrn[num_toc].format= MSBFirst;
	toc_rtrn[num_toc].size+= SIZEOF(xkmSectionInfo);
	toc_rtrn[num_toc].offset= total_size;
	total_size+= toc_rtrn[num_toc].size;
	num_toc++;
    }
#ifdef NOTYET
    toc_rtrn[num_toc].size= SizeXKMGeometry(result,info);
    if (toc_rtrn[num_toc].size>0) {
	toc_rtrn[num_toc].type= XkmGeometryIndex;
	toc_rtrn[num_toc].format= MSBFirst;
	toc_rtrn[num_toc].size+= SIZEOF(xkmSectionInfo);
	toc_rtrn[num_toc].offset= total_size;
	total_size+= toc_rtrn[num_toc].size;
	num_toc++;
    }
#endif
    toc_rtrn[num_toc].size= SizeXKMKeyTypes(result,info);
    if (toc_rtrn[num_toc].size>0) {
	toc_rtrn[num_toc].type= XkmTypesIndex;
	toc_rtrn[num_toc].format= MSBFirst;
	toc_rtrn[num_toc].size+= SIZEOF(xkmSectionInfo);
	toc_rtrn[num_toc].offset= total_size;
	total_size+= toc_rtrn[num_toc].size;
	num_toc++;
    }
    toc_rtrn[num_toc].size= SizeXKMCompatMap(result,info);
    if (toc_rtrn[num_toc].size>0) {
	toc_rtrn[num_toc].type= XkmCompatMapIndex;
	toc_rtrn[num_toc].format= MSBFirst;
	toc_rtrn[num_toc].size+= SIZEOF(xkmSectionInfo);
	toc_rtrn[num_toc].offset= total_size;
	total_size+= toc_rtrn[num_toc].size;
	num_toc++;
    }
    toc_rtrn[num_toc].size= SizeXKMSymbols(result,info);
    if (toc_rtrn[num_toc].size>0) {
	toc_rtrn[num_toc].type= XkmSymbolsIndex;
	toc_rtrn[num_toc].format= MSBFirst;
	toc_rtrn[num_toc].size+= SIZEOF(xkmSectionInfo);
	toc_rtrn[num_toc].offset= total_size;
	total_size+= toc_rtrn[num_toc].size;
	num_toc++;
    }
    if (result->defined&XkbIndicatorMapsDefined) {
	toc_rtrn[num_toc].size= SizeXKMIndicators(result,info);
	if (toc_rtrn[num_toc].size>0) {
	    toc_rtrn[num_toc].type= XkmIndicatorsIndex;
	    toc_rtrn[num_toc].format= MSBFirst;
	    toc_rtrn[num_toc].size+= SIZEOF(xkmSectionInfo);
	    toc_rtrn[num_toc].offset= total_size;
	    total_size+= toc_rtrn[num_toc].size;
	    num_toc++;
	}
    }
    return num_toc;
}

static Bool
WriteXKMFile(file,result,num_toc,toc,info)
    FILE *		file;
    XkbFileInfo *	result;
    int			num_toc;
    xkmSectionInfo *	toc;
    XkmInfo *		info;
{
register int i,size;
Bool	ok= True;
    
    for (i=0;(i<num_toc)&&(ok);i++) {
	fwrite(&toc[i],SIZEOF(xkmSectionInfo),1,file);
	switch (toc[i].type) {
	    case XkmTypesIndex:
		ok= WriteXKMKeyTypes(file,result,info);
		break;
	    case XkmCompatMapIndex:
		ok= WriteXKMCompatMap(file,result,info);
		break;
	    case XkmSymbolsIndex:
		ok= WriteXKMSymbols(file,result,info);
		break;
	    case XkmIndicatorsIndex:
		ok= WriteXKMIndicators(file,result,info);
		break;
	    case XkmKeyNamesIndex:
		ok= WriteXKMKeycodes(file,result,info);
		break;
#ifdef NOTYET
	    case XkmGeometryIndex:
		ok= WriteXKMGeometry(file,result,info);
		break;
#endif
	    case XkmVirtualModsIndex:
		ok= WriteXKMVirtualMods(file,result,info);
		break;
	    default:
		uInternalError("Bogus toc type %d in WriteXKMFile\n",
								toc[i].type);
		break;
	}
    }
    return ok;
}


#define	MAX_TOC	16

Bool
XkbWriteXKMFile(name,result)
    char *		name;
    XkbFileInfo *	result;
{
FILE	*		out;
Bool	 		ok;
XkbDescPtr		xkb;
XkmInfo			info;
Bool			(*writer)();
int			(*getTOC)();
int			size_toc,i;
unsigned		hdr;
xkmFileInfo		fileInfo;
xkmSectionInfo		toc[MAX_TOC];

    switch (result->type) {
	case XkmSemanticsFile:
	    writer= WriteXKMFile;
	    getTOC= GetXKMSemanticsTOC;
	    break;
	case XkmLayoutFile:
	    writer= WriteXKMFile;
	    getTOC= GetXKMLayoutTOC;
	    break;
	case XkmKeymapFile:
	    writer= WriteXKMFile;
	    getTOC= GetXKMKeymapTOC;
	    break;
	default:
	    uError("Cannot create an XKM file from a %s description\n",
						XkbConfigText(result->type));
	    uAction("You must specify a semantics, layout or keymap file\n");
	    return False;
    }
    xkb= &result->xkb;

    bzero((char *)&info,sizeof(XkmInfo));
    size_toc= (*getTOC)(result,&info,MAX_TOC,toc);
    if (size_toc<1) {
	uInternalError("Nothing to compile in XkbWriteXKMFile!!\n");
	return False;
    }
    out= fopen(name,"w");
    if (out==NULL) {
	uError("Couldn't open output file \"%s\"\n",name);
	uAction("Exiting\n");
	return False;
    }
    for (i=0;i<size_toc;i++) {
	toc[i].offset+= (8+(size_toc*SIZEOF(xkmSectionInfo)));;
    }
    hdr= (('x'<<24)|('k'<<16)|('m'<<8)|XkmFileVersion);
    xkmPutCARD32(out,(unsigned long)hdr);
    fileInfo.type= result->type;
    fileInfo.min_kc= xkb->min_key_code;
    fileInfo.max_kc= xkb->max_key_code;
    fileInfo.num_toc= size_toc;
    fwrite(&fileInfo,SIZEOF(xkmFileInfo),1,out);
    fwrite(toc,SIZEOF(xkmSectionInfo),size_toc,out);
    ok= (*writer)(out,result,size_toc,toc,&info);
    fclose(out);
    return True;
}
