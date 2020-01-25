/* $XConsortium: xkmread.c,v 1.2 94/04/09 12:41:41 rws Exp $ */
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
#include <X11/Xlib.h>

#define	DEBUG_VAR_NOT_LOCAL
#define	DEBUG_VAR debugFlags
#include "xkbcomp.h"
#include "tokens.h"

#include <X11/extensions/XKB.h>
#include <X11/XKBlib.h>
#include "XKMformat.h"


/***====================================================================***/

static Bool
XkmInitAtoms(dpy)
    Display	*dpy;
{
static Bool st_initialized= False;

    if ((dpy==NULL)&&(!st_initialized)) {
	st_initialized= True;
	return stInit(100,15,False,NULL);
    }
    return True;
}

static Atom
XkmInternAtom(dpy,str,only_if_exists)
    Display *	dpy;
    char *	str;
    Bool	only_if_exists;
{
    if (str==NULL)
	return None;
    else if (dpy!=NULL)
	return XInternAtom(dpy,str,only_if_exists);
    else if (only_if_exists) {
	StringToken	tok;
	if (stGetTokenIfExists(str,&tok))
	    return tok;
	return None;
    }
    else return (Atom)stGetToken(str);
}

/***====================================================================***/

#define	XkmGetCARD8(f)	(getc(f))

XPointer
XkmInsureSize(oldPtr,oldCount,newCountRtrn,elemSize)
    XPointer	oldPtr;
    int		oldCount;
    int *	newCountRtrn;
    int		elemSize;
{
int	newCount= *newCountRtrn;

    if (oldPtr==NULL) {
	if (newCount==0)
	    return NULL;
	oldPtr= (XPointer)calloc(newCount,elemSize);
    }
    else if (oldCount<newCount) {
	oldPtr= (XPointer)realloc(oldPtr,newCount*elemSize);
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
CARD16		val;
int		tmp;

    fread(&val,2,1,file);
    return val;
}

CARD32
XkmGetCARD32(file)
    FILE *		file;
{
unsigned long	val;

    fread(&val,4,1,file);
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
ReadXkmVirtualMods(file,dpy,xkb,changes)
    FILE *		file;
    Display *		dpy;
    XkbDescPtr		xkb;
    XkbChangesPtr	changes;
{
register unsigned int i,bit,bound,named,tmp;

    if (!XkbAllocServerMap(xkb,XkbVirtualModsMask,0))
	return False;
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
    if (!XkbAllocNames(xkb,XkbVirtualModNamesMask))
	return False;
    for (i=0,bit=1;i<XkbNumVirtualMods;i++,bit<<=1) {
	char name[100];
	if (named&bit) {
	    if (XkmGetCountedString(file,name)) {
		xkb->names->vmods[i]= XkmInternAtom(dpy,name,False);
		if (changes)
		    changes->names.changed_vmods|= bit;
	    }
	}
    }
    return True;
}

/***====================================================================***/

static Bool
ReadXkmKeycodes(file,dpy,xkb,changes)
    FILE *		file;
    Display *		dpy;
    XkbDescPtr		xkb;
    XkbChangesPtr	changes;
{
register int	i;
unsigned	tmp,minKC,maxKC;
char name[100];

    if (!XkbAllocNames(xkb,XkbKeycodesNameMask|XkbKeyNamesMask))
	return False;
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
	    return False;
    }
    XkmSkipPadding(file,2);
    name[0]= '\0';
    XkmGetCountedString(file,name);
    if (name!=NULL) {
	xkb->names->keycodes= XkmInternAtom(dpy,name,False);
    }
    for (i=minKC;i<=maxKC;i++) {
	if (fread(xkb->names->keys[i].name,1,4,file)!=4)
	    return False;
    }
    if (changes)
	changes->names.changed|= XkbKeyNamesMask;
    return True;
}

/***====================================================================***/

static Bool
ReadXkmKeyTypes(file,dpy,xkb,changes)
    FILE *		file;
    Display *		dpy;
    XkbDescPtr		xkb;
    XkbChangesPtr	changes;
{
register unsigned	i,n;
unsigned		num_types;
int			tmp;
XkbKeyTypePtr		type;
xkmKeyTypeDesc		wire;
XkbKTMapEntryPtr	entry;
xkmKTMapEntryDesc	wire_entry;
char 			buf[100];

    num_types= XkmGetCARD16(file);
    XkmSkipPadding(file,2);
    if (num_types<1)
	return True;
    if (!XkbAllocClientMap(xkb,XkbKeyTypesMask,num_types))
	return False;
    xkb->map->num_types= num_types;
    type= xkb->map->types;
    for (i=0;i<num_types;i++,type++) {
	if ((int)fread(&wire,SIZEOF(xkmKeyTypeDesc),1,file)<1)
	    return False;
	if (((i==0)&&(wire.groupWidth!=1))||
	    (((i==1)||(i==2))&&(wire.groupWidth!=2))) {
	    return False;
	}
	tmp= wire.nMapEntries;
	XkmInsureTypedSize(type->map,type->map_count,&tmp,XkbKTMapEntryRec);
	if ((wire.nMapEntries>0)&&(type->map==NULL))
	    return False;
	for (n=0,entry= type->map;n<wire.nMapEntries;n++,entry++) {
	    if ((int)fread(&wire_entry,SIZEOF(xkmKTMapEntryDesc),1,file)<(int)1)
		return False;
	    entry->active= (wire_entry.virtualMods==0);
	    entry->level= wire_entry.level;
	    entry->mask= wire_entry.realMods;
	    entry->real_mods= wire_entry.realMods;
	    entry->vmods= wire_entry.virtualMods;
	}
	XkmGetCountedString(file,buf);
	if (strlen(buf)>(unsigned)0) {
	     type->name= XkmInternAtom(dpy,buf,False);
	}
	else type->name= None;

	if (wire.preserve) {
	    xkmKTPreserveDesc	p_entry;
	    XkbKTPreservePtr	pre;
	    XkmInsureTypedSize(type->preserve,type->map_count,&tmp,
						XkbKTPreserveRec);
	    if (type->preserve==NULL)
		return False;
	    for (n=0,pre=type->preserve;n<wire.nMapEntries;n++,pre++) {
		if ((int)fread(&p_entry,SIZEOF(xkmKTPreserveDesc),1,file)<(int)1)
		    return False;
		pre->mask= p_entry.realMods;
		pre->real_mods= p_entry.realMods;
		pre->vmods= p_entry.virtualMods;
	    }
	}
	if (wire.nLevelNames>0) {
	    int width= wire.groupWidth;
	    if (wire.nLevelNames>(unsigned)width)
		return False;
	    XkmInsureTypedSize(type->lvl_names,type->group_width,&width,Atom);
	    if (type->lvl_names!=NULL)
	    for (n=0;n<wire.nLevelNames;n++) {
		if (!XkmGetCountedString(file,buf))
		     return False;
		if (strlen(buf)==0)
		     type->lvl_names[n]= None;
		else type->lvl_names[n]= XkmInternAtom(dpy,buf,False);
if (type->lvl_names[n]!=None)
    fprintf(stderr,"type %d: lvl_name[%d]= %s\n",i,n,buf);
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
    return True;
}

/***====================================================================***/

static Bool
ReadXkmCompatMap(file,dpy,xkb,changes)
    FILE *		file;
    Display *		dpy;
    XkbDescPtr		xkb;
    XkbChangesPtr	changes;
{
register int		i;
unsigned		num_si,mods,vmods;
char 			name[100];
XkbSymInterpretPtr	interp;
xkmSymInterpretDesc	wire;

    num_si= XkmGetCARD16(file);
    XkmSkipPadding(file,2);
    if (XkmGetCountedString(file,name)>0) {
	if (!XkbAllocNames(xkb,XkbSemanticsNameMask))
	    return False;
	xkb->names->semantics= XkmInternAtom(dpy,name,False);
    }
    if (!XkbAllocCompatMap(xkb,XkbAllCompatMask,num_si))
	return False;
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
    return True;
}

static Bool
ReadXkmIndicators(file,dpy,xkb,changes)
    FILE *		file;
    Display *		dpy;
    XkbDescPtr		xkb;
    XkbChangesPtr	changes;
{
register unsigned	nLEDs;
xkmIndicatorMapDesc	wire;
char			name[100];

    if (xkb->indicators==NULL) {
	xkb->indicators= uTypedAlloc(XkbIndicatorRec);
	if (xkb->indicators==NULL) {
	    uInternalError("Couldn't allocate IndicatorRec\n");
	    uAction("Indicator maps not loaded\n");
	    return False;
	}
	bzero((char *)xkb->indicators,sizeof(XkbIndicatorRec));
    }
    if (!XkbAllocNames(xkb,XkbIndicatorNamesMask)) {
	uInternalError("Couldn't allocate space for indicator names\n");
	uAction("Indicator maps should be okay, but names will be wrong\n");
    }
    nLEDs= xkmGetCARD8(file);
    xkmSkipPadding(file,3);
    while (nLEDs-->0) {
	Atom name;
	if (xkmGetCountedString(file,name)>0)
	     name= XkbInternAtom(dpy,name,False);
	else name= None;
	if (fread(&wire,SIZEOF(xkmIndicatorMapDesc),1,file)<1) {
	    uInternalError("Couldn't read all indicator map descriptions\n");
	    uAction("Exiting\n");
	    return False;
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
    return True;
}

#ifdef NOTYET
/***====================================================================***/

static Bool
WriteXkmSymbols(file,result,info)
    FILE *		file;
    XkbFileResult *	result;
    XkmInfo *		info;
{
XkbDescPtr		xkb;
register int		i,n;
xkmKeySymMapDesc 	wireMap;

    xkb= &result->xkb;
    XkmPutCARD8(file,xkb->min_key_code);
    XkmPutCARD8(file,xkb->max_key_code);
    for (i=xkb->min_key_code;i<(int)xkb->max_key_code;i++)  {
	register char *typeName;
	wireMap.group_width= XkbKeyGroupWidth(xkb,i);
	wireMap.num_groups= XkbKeyNumGroups(xkb,i);
	if (result->defined&ModMapDefined)
	     wireMap.modifier_map= result->modmap[i];
	else wireMap.modifier_map= 0;
	wireMap.flags= 0;
	typeName= NULL;
	if (xkb->server) {
	    if (xkb->server->explicit[i]&XkbExplicitKeyTypeMask) {
		typeName= stGetString((StringToken)XkbKeyKeyType(xkb,i)->name);
		if (typeName!=NULL)
		    wireMap.flags|= XkmKeyHasType;
	    }
	    if (XkbKeyHasActions(xkb,i))
		wireMap.flags|= XkmKeyHasActions;
	    if (xkb->server->behaviors[i].type!=XkbKB_Default)
		wireMap.flags|= XkmKeyHasBehavior;
	    if ((result->defined&PerKeyRepeatDefined)&&
		(xkb->server->explicit[i]&XkbExplicitAutorepeatMask)) {
		if (result->repeat[(i/8)]&(1<<(i%8)))
		     wireMap.flags|= XkmRepeatingKey;
		else wireMap.flags|= XkmNonRepeatingKey;
	    }
	}
	fwrite(&wireMap,SIZEOF(xkmKeySymMapDesc),1,file);
	if (typeName!=NULL)
	    XkmPutCountedString(file,typeName);
	if (wireMap.num_groups>0) {
	    KeySym	*sym;
	    sym= XkbKeySymsPtr(xkb,i);
	    for (n=XkbKeyNumSyms(xkb,i);n>0;n--,sym++) {
		XkmPutCARD32(file,(CARD32)*sym);
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

static int
GetXkmSemanticsTOC(result,info,max_toc,toc_rtrn)
    XkbFileResult *	result;
    XkmInfo *		info;
    int			max_toc;
    xkmSectionInfo *	toc_rtrn;
{
int	num_toc;
int	total_size;

    total_size= num_toc=0;
    toc_rtrn[num_toc].size= SizeXkmVirtualMods(result,info);
    if (toc_rtrn[num_toc].size>0) {
	toc_rtrn[num_toc].type= XkmVirtualModsIndex;
	toc_rtrn[num_toc].format= MSBFirst;
	toc_rtrn[num_toc].size+= SIZEOF(xkmSectionInfo);
	toc_rtrn[num_toc].offset= total_size;
	total_size+= toc_rtrn[num_toc].size;
	num_toc++;
    }
    toc_rtrn[num_toc].size= SizeXkmKeyTypes(result,info);
    if (toc_rtrn[num_toc].size>0) {
	toc_rtrn[num_toc].type= XkmTypesIndex;
	toc_rtrn[num_toc].format= MSBFirst;
	toc_rtrn[num_toc].size+= SIZEOF(xkmSectionInfo);
	toc_rtrn[num_toc].offset= total_size;
	total_size+= toc_rtrn[num_toc].size;
	num_toc++;
    }
    toc_rtrn[num_toc].size= SizeXkmCompatMap(result,info);
    if (toc_rtrn[num_toc].size>0) {
	toc_rtrn[num_toc].type= XkmCompatMapIndex;
	toc_rtrn[num_toc].format= MSBFirst;
	toc_rtrn[num_toc].size+= SIZEOF(xkmSectionInfo);
	toc_rtrn[num_toc].offset= total_size;
	total_size+= toc_rtrn[num_toc].size;
	num_toc++;
    }
    return num_toc;
}

static int
GetXkmLayoutTOC(result,info,max_toc,toc_rtrn)
    XkbFileResult *	result;
    XkmInfo *		info;
    int			max_toc;
    xkmSectionInfo *	toc_rtrn;
{
    return 0;
}

static int
GetXkmKeymapTOC(result,info,max_toc,toc_rtrn)
    XkbFileResult *	result;
    XkmInfo *		info;
    int			max_toc;
    xkmSectionInfo *	toc_rtrn;
{
    return 0;
}

static Bool
WriteXkmFile(file,result,num_toc,toc,info)
    FILE *		file;
    XkbFileResult *	result;
    int			num_toc;
    xkmSectionInfo *	toc;
    XkmInfo *		info;
{
register int i,size;
Bool	ok= True;
    
    for (i=0;(i<num_toc)&&(ok);i++) {
	fwrite(&toc[i],SIZEOF(xkmSectionInfo),1,file);
	switch (toc[i].type) {
	    case XkmVirtualModsIndex:
		ok= WriteXkmVirtualMods(file,result,info);
		break;
	    case XkmTypesIndex:
		ok= WriteXkmKeyTypes(file,result,info);
		break;
	    case XkmCompatMapIndex:
		ok= WriteXkmCompatMap(file,result,info);
		break;
	    default:
		fprintf(stderr,"Bogus toc type %d in WriteXkmSemantics\n",
								toc[i].type);
		break;
	}
    }
    return ok;
}
#endif

Bool
XkmReadTOC(file,xkb,file_info,max_toc,toc)
    FILE *		file;
    XkbDescPtr		xkb;
    int			max_toc;
    xkmFileInfo *	file_info;
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

#define	MAX_TOC	16
Bool
XkmReadFile(file,dpy,type,xkb)
    FILE *		file;
    Display *		dpy;
    unsigned		type;
    XkbDescPtr		xkb;
{
register unsigned	i;
xkmSectionInfo		toc[MAX_TOC],tmpTOC;
xkmFileInfo		fileInfo;
Bool			ok;
    if (!XkmReadTOC(file,xkb,&fileInfo,MAX_TOC,toc))
	return 1;
    if (fileInfo.type!=type) {
	fprintf(stderr,"Bad type in XKM file.  Expected %d, got %d\n",type,
								fileInfo.type);
	return 1;
    }
    for (i=0;i<fileInfo.num_toc;i++) {
#ifdef SEEK_SET
	fseek(file,toc[i].offset,SEEK_SET);
#else
	fseek(file,toc[i].offset,0);
#endif
	fread(&tmpTOC,SIZEOF(xkmSectionInfo),1,file);
	if ((tmpTOC.type!=toc[i].type)||(tmpTOC.format!=toc[i].format)||
	    (tmpTOC.size!=toc[i].size)||(tmpTOC.offset!=toc[i].offset)) {
	    return 0;
	}
	switch (tmpTOC.type) {
	    case XkmVirtualModsIndex:
		ok= ReadXkmVirtualMods(file,dpy,xkb,NULL);
		break;
	    case XkmTypesIndex:
		ok= ReadXkmKeyTypes(file,dpy,xkb,NULL);
		break;
	    case XkmCompatMapIndex:
		ok= ReadXkmCompatMap(file,dpy,xkb,NULL);
		break;
	    case XkmKeyNamesIndex:
		ok= ReadXkmKeycodes(file,dpy,xkb,NULL);
		break;
	    case XkmIndicatorsIndex:
		ok= ReadXkmIndicators(file,dpy,xkb,NULL);
		break;
	    default:
		fprintf(stderr,"Reader for %s not implemented yet\n",
						XkbConfigText(tmpTOC.type));
		ok= 1;
		break;
	}
    }
    return 1;
}

