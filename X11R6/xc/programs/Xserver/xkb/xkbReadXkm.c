/* $XConsortium: xkbReadXkm.c,v 1.1 94/04/08 15:15:55 erik Exp $ */
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
#include <string.h>
#include <stdlib.h>
#define	NEED_EVENTS
#include "X.h"
#include "Xproto.h"
#include "misc.h"
#include "dixstruct.h"
#include "inputstr.h"
#include <extensions/XKB.h>
#include <XKBstr.h>
#include <XKBsrv.h>
#include "XKMformat.h"

/***====================================================================***/

static Atom
XkmInternAtom(str,only_if_exists)
    char *	str;
    Bool	only_if_exists;
{
    if (str==NULL)
	return None;
    return MakeAtom(str,strlen(str),!only_if_exists);
}

/***====================================================================***/

#define	XkmGetCARD8(f)	(getc(f))

pointer
XkmInsureSize(oldPtr,oldCount,newCountRtrn,elemSize)
    pointer	oldPtr;
    int		oldCount;
    int *	newCountRtrn;
    int		elemSize;
{
int	newCount= *newCountRtrn;

    if (oldPtr==NULL) {
	if (newCount==0)
	    return NULL;
	oldPtr= (pointer)Xcalloc(newCount*elemSize);
    }
    else if (oldCount<newCount) {
	oldPtr= (pointer)Xrealloc(oldPtr,newCount*elemSize);
	if (oldPtr!=NULL) {
	    char *tmp= (char *)oldPtr;
	    bzero(&tmp[oldCount*elemSize],(newCount-oldCount)*elemSize);
	}
    }
    else if (newCount<oldCount) {
	*newCountRtrn= oldCount;
    }
    return oldPtr;
}

#define	XkmInsureTypedSize(p,o,n,t) ((p)=((t *)XkmInsureSize((char *)(p),(o),(n),sizeof(t))))

CARD16
XkmGetCARD16(file)
    FILE *		file;
{
unsigned		val;
int			tmp;

    val= (getc(file)&0xff)<<8;
    val|= (getc(file)&0xff);
    return val;
}

CARD32
XkmGetCARD32(file)
    FILE *		file;
{
unsigned long	val;

    val= (getc(file)&0xff)<<24;
    val|= (getc(file)&0xff)<<16;
    val|= (getc(file)&0xff)<<8;
    val|= (getc(file)&0xff);
    return val;
}

void
XkmSkipPadding(file,pad)
    FILE *		file;
    unsigned		pad;
{
int	i;
    for (i=0;i<pad;i++) {
	getc(file);
    }
    return;
}

int
XkmGetCountedBytes(file,ptr)
    FILE *		file;
    char *		ptr;
{
int	count,nRead;

    nRead=2;
    count= XkmGetCARD16(file);
    if (count>0) {
	fread(ptr,1,count,file);
	nRead+= count;
    }
    count= XkbPaddedSize(nRead)-nRead;
    if (count>0)
	XkmSkipPadding(file,count);
    return nRead;
}

int
XkmGetCountedString(file,str)
    FILE *		file;
    char *		str;
{
int	count,nRead;

    nRead=2;
    count= XkmGetCARD16(file);
    if (count>0) {
	fread(str,1,count,file);
	nRead+= count;
    }
    str[count]= '\0';
    count= XkbPaddedSize(nRead)-nRead;
    if (count>0)
	XkmSkipPadding(file,count);
    return nRead;
}

/***====================================================================***/

static Bool
ReadXkmVirtualMods(file,xkb,changes)
    FILE *		file;
    XkbDescPtr		xkb;
    XkbChangesPtr	changes;
{
register unsigned int i,bit,bound,named,tmp;

    bound= XkmGetCARD16(file);
    named= XkmGetCARD16(file);
    for (i=tmp=0,bit=1;i<XkbNumVirtualMods;i++,bit<<=1) {
	if (bound&bit) {
	    xkb->server->vmods[i]= XkmGetCARD8(file);
	    if (changes)
		changes->map.vmods|= bit;
	    tmp++;
	}
    }
    if ((i= XkbPaddedSize(tmp)-tmp)>0)
	XkmSkipPadding(file,i);
    for (i=0,bit=1;i<XkbNumVirtualMods;i++,bit<<=1) {
	char name[100];
	if (named&bit) {
	    if (XkmGetCountedString(file,name)) {
		xkb->names->vmods[i]= XkmInternAtom(name,0);
		if (changes)
		    changes->names.changed_vmods|= bit;
	    }
	}
    }
    return 1;
}

/***====================================================================***/

static Bool
ReadXkmKeycodes(file,xkb,changes)
    FILE *		file;
    XkbDescPtr		xkb;
    XkbChangesPtr	changes;
{
register int	i;
unsigned	tmp,minKC,maxKC;
char name[100];

    minKC= XkmGetCARD8(file);
    maxKC= XkmGetCARD8(file);
    if (xkb->min_key_code==0) {
	xkb->min_key_code= minKC;
	xkb->max_key_code= maxKC;
    }
    else {
	if (minKC<xkb->min_key_code)
	    xkb->min_key_code= minKC;
	if (maxKC>xkb->max_key_code)
	    maxKC= xkb->max_key_code;
    }
    XkmSkipPadding(file,2);
    name[0]= '\0';
    XkmGetCountedString(file,name);
    if (name!=NULL) {
	xkb->names->keycodes= XkmInternAtom(name,0);
    }
    if (xkb->names->keys==NULL) {
	xkb->names->keys= (XkbKeyNamePtr)
			  Xcalloc((xkb->max_key_code+1)*sizeof(XkbKeyNameRec)); 
	if (xkb->names->keys==NULL) {
	    ErrorF("Couldn't allocate key names\n");
	    return 0;
	}
    }
    for (i=minKC;i<=maxKC;i++) {
	if (fread(&xkb->names->keys[i].name[0],1,4,file)!=4)
	    return 0;
    }
    if (changes)
	changes->names.changed|= XkbKeyNamesMask;
    return 1;
}

/***====================================================================***/

static Bool
ReadXkmKeyTypes(file,xkb,changes)
    FILE *		file;
    XkbDescPtr		xkb;
    XkbChangesPtr	changes;
{
register int		i,n;
unsigned		num_types,tmp;
XkbKeyTypePtr		type;
xkmKeyTypeDesc		wire;
XkbKTMapEntryPtr	entry;
xkmKTMapEntryDesc	wire_entry;
char 			buf[100];

    num_types= XkmGetCARD16(file);
    XkmSkipPadding(file,2);
    if (num_types<1)
	return 1;
    if ((xkb->map->types==NULL)||(xkb->map->size_types<num_types)||
				 (xkb->map->types[0].free&XkbNoFreeKTStruct)) {
	if (xkb->map->types!=NULL) {
	    if ((xkb->map->types[0].free&XkbNoFreeKTStruct)==0)
		Xfree(xkb->map->types);
	    xkb->map->types= NULL;
	}
	xkb->map->types= (XkbKeyTypePtr)
			 Xcalloc(num_types*sizeof(XkbKeyTypeRec));
	if (xkb->map->types==NULL)
	    return 0;
	xkb->map->size_types= num_types;
    }
	
    xkb->map->num_types= num_types;
    type= xkb->map->types;
    for (i=0;i<num_types;i++,type++) {
	if (fread(&wire,SIZEOF(xkmKeyTypeDesc),1,file)<1)
	    return 0;
	if (((i==0)&&(wire.groupWidth!=1))||
	    (((i==1)||(i==2))&&(wire.groupWidth!=2))) {
	    return 0;
	}
	tmp= wire.nMapEntries;
	XkmInsureTypedSize(type->map,type->map_count,&tmp,XkbKTMapEntryRec);
	if ((wire.nMapEntries>0)&&(type->map==NULL))
	    return 0;
	for (n=0,entry= type->map;n<wire.nMapEntries;n++,entry++) {
	    if (fread(&wire_entry,SIZEOF(xkmKTMapEntryDesc),1,file)<1)
		return 0;
	    entry->active= (wire_entry.virtualMods==0);
	    entry->level= wire_entry.level;
	    entry->mask= wire_entry.realMods;
	    entry->real_mods= wire_entry.realMods;
	    entry->vmods= wire_entry.virtualMods;
	}
	XkmGetCountedString(file,buf);
	if (strlen(buf)>0) {
	     type->name= XkmInternAtom(buf,0);
	}
	else type->name= None;

	if (wire.preserve) {
	    xkmKTPreserveDesc	p_entry;
	    XkbKTPreservePtr	pre;
	    XkmInsureTypedSize(type->preserve,type->map_count,&tmp,
						XkbKTPreserveRec);
	    if (type->preserve==NULL)
		return 0;
	    for (n=0,pre=type->preserve;n<wire.nMapEntries;n++,pre++) {
		if (fread(&p_entry,SIZEOF(xkmKTPreserveDesc),1,file)<1)
		    return 0;
		pre->mask= p_entry.realMods;
		pre->real_mods= p_entry.realMods;
		pre->vmods= p_entry.virtualMods;
	    }
	}
	if (wire.nLevelNames>0) {
	    int width= wire.groupWidth;
	    if (wire.nLevelNames>width)
		return 0;
	    XkmInsureTypedSize(type->lvl_names,type->group_width,&width,Atom);
	    if (type->lvl_names!=NULL)
	    for (n=0;n<wire.nLevelNames;n++) {
		if (!XkmGetCountedString(file,buf))
		     return 0;
		if (strlen(buf)==0)
		     type->lvl_names[n]= None;
		else type->lvl_names[n]= XkmInternAtom(buf,0);
	    }
	}
	type->mask= wire.realMods;
	type->real_mods= wire.realMods;
	type->group_width= wire.groupWidth;
	type->vmods= wire.virtualMods;
	type->map_count= wire.nMapEntries;
    }
    if (changes) {
	changes->map.changed|= XkbKeyTypesMask;
	changes->map.first_type= 0;
	changes->map.num_types= xkb->map->num_types;
    }
    return 1;
}

/***====================================================================***/

static Bool
ReadXkmCompatMap(file,xkb,changes)
    FILE *		file;
    XkbDescPtr		xkb;
    XkbChangesPtr	changes;
{
register int		i;
unsigned		num_si,mods,vmods;
char *			name[100];
XkbSymInterpretPtr	interp;
xkmSymInterpretDesc	wire;

    num_si= XkmGetCARD16(file);
    XkmSkipPadding(file,2);
    if (XkmGetCountedString(file,name)>0)
	xkb->names->semantics= XkmInternAtom(name,0);
    if ((xkb->compat->sym_interpret==NULL)||(xkb->compat->size_si<num_si)) {
	if (xkb->compat->sym_interpret!=NULL) {
	    Xfree(xkb->compat->sym_interpret);
	}
	xkb->compat->sym_interpret= (XkbSymInterpretPtr)
				    Xcalloc(num_si*sizeof(XkbSymInterpretRec));
	if (xkb->compat->sym_interpret==NULL)
	    return 0;
	xkb->compat->size_si= num_si;
    }
    xkb->compat->num_si= num_si;
    interp= xkb->compat->sym_interpret;
    for (i=0;i<num_si;i++,interp++) {
	fread(&wire,SIZEOF(xkmSymInterpretDesc),1,file);
	interp->sym= wire.sym;
	interp->mods= wire.mods;
	interp->match= wire.match;
	interp->virtual_mod= wire.virtualMod;
	interp->flags= wire.flags;
	interp->act.type= wire.actionType;
	interp->act.data[0]= wire.actionData[0];
	interp->act.data[1]= wire.actionData[1];
	interp->act.data[2]= wire.actionData[2];
	interp->act.data[3]= wire.actionData[3];
	interp->act.data[4]= wire.actionData[4];
	interp->act.data[5]= wire.actionData[5];
	interp->act.data[6]= wire.actionData[6];
    }
    if ((num_si>0)&&(changes)) {
	changes->compat.first_si= 0;
	changes->compat.num_si= num_si;
    }
    mods= XkmGetCARD8(file);
    XkmSkipPadding(file,1);
    vmods= XkmGetCARD16(file);
    if (mods) {
	register unsigned bit;
	for (i=0,bit=1;i<XkbNumModifiers;i++,bit<<=1) {
	    if (mods&bit) {
		xkb->compat->real_mod_compat[i].mods= XkmGetCARD8(file);
		xkb->compat->real_mod_compat[i].groups= XkmGetCARD8(file);
	    }
	}
	if (changes)
	    changes->compat.changed_mods|= mods;
    }
    if (vmods) {
	register unsigned bit;
	for (i=0,bit=1;i<XkbNumVirtualMods;i++,bit<<=1) {
	    if (vmods&bit) {
		xkb->compat->vmod_compat[i].mods= XkmGetCARD8(file);
		xkb->compat->vmod_compat[i].groups= XkmGetCARD8(file);
	    }
	}
	if (changes)
	    changes->compat.changed_vmods|= vmods;
    }
    return 1;
}

static Bool
ReadXkmIndicators(file,xkb,changes)
    FILE *		file;
    XkbDescPtr		xkb;
    XkbChangesPtr	changes;
{
register unsigned	nLEDs;
xkmIndicatorMapDesc	wire;
char			buf[100];

    nLEDs= XkmGetCARD8(file);
    XkmSkipPadding(file,3);
    while (nLEDs-->0) {
	Atom name;
	if (XkmGetCountedString(file,buf)>0)
	     name= XkmInternAtom(buf,0);
	else name= None;
	if (fread(&wire,SIZEOF(xkmIndicatorMapDesc),1,file)<1) {
	    ErrorF("Couldn't read all indicator map descriptions\n");
	    return 0;
	}
	if ((wire.indicator==XkmIndicatorUnbound)&&(xkb->names!=NULL)) {
	    register int i;
	    for (i=0;i<XkbNumIndicators;i++) {
		if (xkb->names->indicators[i]==name) {
		    wire.indicator= i+1;
		    break;
		}
	    }
	}
	if (wire.indicator!=XkmIndicatorUnbound) {
	    XkbIndicatorMapPtr	map;
	    if (xkb->names) {
		xkb->names->indicators[wire.indicator-1]= name;
		if (changes)
		    changes->names.changed_indicators|= (1<<(wire.indicator-1));
	    }
	    map= &xkb->indicators->maps[wire.indicator-1];
	    map->flags= wire.flags;
	    map->which_groups= wire.which_groups;
	    map->groups= wire.groups;
	    map->which_mods= wire.which_mods;
	    map->mask= wire.real_mods;
	    map->real_mods= wire.real_mods;
	    map->vmods= wire.vmods;
	    map->ctrls= wire.ctrls;
	}
    }
    return 1;
}

static XkbKeyTypePtr
FindTypeForKey(xkb,name,width)
    XkbDescPtr	xkb;
    Atom	name;
    unsigned	width;
{
    if ((!xkb)||(!xkb->map))
	return NULL;
    if (name!=None) {
	register unsigned i;
	for (i=0;i<xkb->map->num_types;i++) {
	    if (xkb->map->types[i].name==name) {
		if (xkb->map->types[i].group_width!=width)
		    ErrorF("Group width mismatch between key and type\n");
		return &xkb->map->types[i];
	    }
	}
    }
    if (width==1)
	return &xkb->map->types[XkbOneLevelIndex];
    return &xkb->map->types[XkbTwoLevelIndex];
}

static Bool
ReadXkmSymbols(file,dev,xkb,changes)
    FILE *		file;
    DeviceIntRec *	dev;
    XkbDescPtr		xkb;
    XkbChangesPtr	changes;
{
register int		i,g,s;
xkmKeySymMapDesc 	wireMap;
char 			buf[100];
unsigned		minKC,maxKC;

    minKC= XkmGetCARD8(file);
    maxKC= XkmGetCARD8(file);
    XkmSkipPadding(file,2);
    if ((XkmGetCountedString(file,buf)>0)&&(xkb->names)) {
	Atom name;
	name= XkmInternAtom(buf,0);
	xkb->names->symbols= name;
    }
    if ((xkb->map==NULL)|(xkb->server==NULL))
	return 0;
    if (xkb->min_key_code<8)	xkb->min_key_code= minKC;
    if (xkb->max_key_code<8)	xkb->max_key_code= maxKC;
    if ((minKC>=8)&&(minKC<xkb->min_key_code))
	xkb->min_key_code= minKC;
    if ((maxKC>=8)&&(maxKC>xkb->max_key_code)) {
	ErrorF("Too many keys in symbol map\n");
	maxKC= xkb->max_key_code;
    }
    for (i=minKC;i<(int)maxKC;i++)  {
	Atom 		typeName;
	XkbKeyTypePtr	type;
	if (fread(&wireMap,SIZEOF(xkmKeySymMapDesc),1,file)<1) 
	    return 0;
	typeName= None;
	if ((wireMap.flags&XkmKeyHasType)&&(XkmGetCountedString(file,buf)>0))
	    typeName= XkmInternAtom(buf,1);
	type= FindTypeForKey(xkb,typeName,wireMap.group_width);
	if (type==NULL)
	    return 0;
	if (typeName==type->name)
	    xkb->server->explicit[i]|= XkbExplicitKeyTypeMask;
	if (wireMap.flags&XkmRepeatingKey) {
	    dev->kbdfeed->ctrl.autoRepeats[i/8]|= (1<<(i%8));
	    xkb->server->explicit[i]|= XkbExplicitAutorepeatMask;
	}
	else if (wireMap.flags&XkmNonRepeatingKey) {
	    dev->kbdfeed->ctrl.autoRepeats[i/8]&= ~(1<<(i%8));
	    xkb->server->explicit[i]|= XkbExplicitAutorepeatMask;
	}
	if (wireMap.modifier_map!=0)
	    dev->key->modifierMap[i]= wireMap.modifier_map;
	if (wireMap.num_groups>0) {
	    unsigned	 fileStride,memStride,nPerGroup;
	    KeySym	*sym;
	
	    fileStride= wireMap.group_width;
	    memStride= type->group_width;
	    if (fileStride>memStride)
		 nPerGroup= memStride;
	    else nPerGroup= fileStride;
	    sym= _XkbNewSymsForKey(xkb,i,wireMap.num_groups*memStride);
	    if (!sym)
		return 0;
	    for (g=0;g<wireMap.num_groups;g++) {
		for (s=0;s<nPerGroup;s++) {
		    *sym= XkmGetCARD32(file);
		    sym++;
		}
		if (fileStride>nPerGroup)
		    XkmSkipPadding(file,(fileStride-nPerGroup)*4);
		else if (memStride>nPerGroup)
		    sym+= (memStride-nPerGroup);
	    }
	    if (wireMap.flags&XkmKeyHasActions) {
		XkbAction *	act;
		act= _XkbNewActionsForKey(xkb,i,wireMap.num_groups*memStride);
		for (g=0;g<wireMap.num_groups;g++) {
		    for (s=0;s<nPerGroup;s++) {
			fread(&act,SIZEOF(xkmActionDesc),1,file);
			act++;
		    }
		    if (fileStride>nPerGroup)
			XkmSkipPadding(file,
				  (fileStride-nPerGroup)*SIZEOF(xkmActionDesc));
		    else if (memStride>nPerGroup)
			act+= (memStride-nPerGroup);
		}
		xkb->server->explicit[i]|= XkbExplicitInterpretMask;
	    }
	}
	xkb->map->key_sym_map[i].kt_index= type-(&xkb->map->types[0]);
	xkb->map->key_sym_map[i].group_info= wireMap.num_groups;
	if (wireMap.flags&XkmKeyHasBehavior) {
	    xkmBehaviorDesc	b;
	    fread(&b,SIZEOF(xkmBehaviorDesc),1,file);
	    xkb->server->behaviors[i].type= b.type;
	    xkb->server->behaviors[i].data= b.data;
	    xkb->server->explicit[i]|= XkbExplicitBehaviorMask;
	}
    }
    return 1;
}

/***====================================================================***/

Bool
XkmReadTOC(file,xkb,file_info,max_toc,toc)
    FILE *		file;
    XkbDescPtr		xkb;
    xkmFileInfo *	file_info;
    int			max_toc;
    xkmSectionInfo *	toc;
{
unsigned hdr,tmp;
unsigned i,size_toc,min_kc,max_kc;

    hdr= (('x'<<24)|('k'<<16)|('m'<<8)|XkmFileVersion);
    tmp= XkmGetCARD32(file);
    if (tmp!=hdr) {
	if ((tmp&(~0xff))==(hdr&(~0xff))) {
	    fprintf(stderr,"Bad XKM file version (got %d, expected %d)\n",
							tmp&0xff,hdr&0xff);
	}
	else {
	    fprintf(stderr,"Not an XKM file (got 0x%x, expected 0x%x)\n",
							tmp,hdr);
	}
	return 0;
    }
    fread(file_info,SIZEOF(xkmFileInfo),1,file);
    size_toc= file_info->num_toc;
    if (size_toc>max_toc) {
	fprintf(stderr,"Warning! Too many TOC entries; last %d ignored\n",
							size_toc-max_toc);
	size_toc= max_toc;
    }
    for (i=0;i<size_toc;i++) {
	fread(&toc[i],SIZEOF(xkmSectionInfo),1,file);
    }
    return 1;
}

xkmSectionInfo *
XkmFindTOCEntry(finfo,toc,type)
    xkmFileInfo *	finfo;
    xkmSectionInfo *	toc;
    unsigned		type;
{
register int i;

    for (i=0;i<finfo->num_toc;i++) {
	if (toc[i].type==type)
	    return &toc[i];
    }
    return NULL;
}

Bool
XkmReadFileSection(file,toc,dev,loaded_rtrn)
    FILE *		file;
    xkmSectionInfo *	toc;
    DeviceIntRec *	dev;
    unsigned *		loaded_rtrn;
{
xkmSectionInfo		tmpTOC;
XkbDescPtr		xkb;
Bool			ok;

    if ((!dev)||(!dev->key)||(!dev->key->xkbInfo))
	return 0;
    ok= 1;
    xkb= &dev->key->xkbInfo->desc;
#ifdef SEEK_SET
    fseek(file,toc->offset,SEEK_SET);
#else
    fseek(file,toc->offset,0);
#endif
    fread(&tmpTOC,SIZEOF(xkmSectionInfo),1,file);
    if ((tmpTOC.type!=toc->type)||(tmpTOC.format!=toc->format)||
	(tmpTOC.size!=toc->size)||(tmpTOC.offset!=toc->offset)) {
	return 0;
    }
    switch (tmpTOC.type) {
	case XkmVirtualModsIndex:
	    ok= ReadXkmVirtualMods(file,xkb,NULL);
	    if ((loaded_rtrn)&&(ok))
		*loaded_rtrn|= XkmVirtualModsMask;
	    break;
	case XkmTypesIndex:
	    ok= ReadXkmKeyTypes(file,xkb,NULL);
	    if ((loaded_rtrn)&&(ok))
		*loaded_rtrn|= XkmTypesMask;
	    break;
	case XkmCompatMapIndex:
	    ok= ReadXkmCompatMap(file,xkb,NULL);
	    if ((loaded_rtrn)&&(ok))
		*loaded_rtrn|= XkmCompatMapMask;
	    break;
	case XkmKeyNamesIndex:
	    ok= ReadXkmKeycodes(file,xkb,NULL);
	    if ((loaded_rtrn)&&(ok))
		*loaded_rtrn|= XkmKeyNamesMask;
	    break;
	case XkmSymbolsIndex:
	    ok= ReadXkmSymbols(file,dev,xkb,NULL);
	    if ((loaded_rtrn)&&(ok))
		*loaded_rtrn|= XkmSymbolsMask;
	    break;
	case XkmIndicatorsIndex:
	    ok= ReadXkmIndicators(file,xkb,NULL);
	    if ((loaded_rtrn)&&(ok))
		*loaded_rtrn|= XkmIndicatorsMask;
	    break;
	default:
	    fprintf(stderr,"Can't read XKM file type %d yet\n",tmpTOC.type);
		ok= 1;
		break;
    }
    return ok;
}

/***====================================================================***/

#define	MAX_TOC	16
Bool
XkmReadFile(file,type,dev,loaded_rtrn)
    FILE *		file;
    unsigned		type;
    DeviceIntRec *	dev;
    unsigned *		loaded_rtrn;
{
register int	i;
xkmSectionInfo	toc[MAX_TOC];
xkmFileInfo	fileInfo;
Bool		ok;
XkbDescPtr	xkb;

    if ((!dev)||(!dev->key)||(!dev->key->xkbInfo))
	return 0;
    xkb= &dev->key->xkbInfo->desc;

    if (!XkmReadTOC(file,xkb,&fileInfo,MAX_TOC,toc))
	return 1;
    if (fileInfo.type!=type) {
	ErrorF("Bad type for XKB config file.  Expected %d, got %d\n",type,
								fileInfo.type);
	return 1;
    }
    for (i=0,ok=1;i<fileInfo.num_toc;i++) {
	ok= XkmReadFileSection(file,&toc[i],dev,loaded_rtrn)&&ok;
    }
    return ok;
}
