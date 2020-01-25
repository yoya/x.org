/* $XConsortium: xkbUtils.c,v 1.14 94/05/09 13:07:14 dpw Exp $ */
/************************************************************
Copyright (c) 1993 by Silicon Graphics Computer Systems, Inc.

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
#include <math.h>
#define NEED_EVENTS 1
#include <X11/X.h>
#include <X11/Xproto.h>
#include <X11/keysym.h>
#include "misc.h"
#include "inputstr.h"
#include "XKBsrv.h"
#include <ctype.h>

	int	XkbComputeAutoRepeat = 1;
	int	XkbDisableLockActions = 0;

#define IsKeypadKey(keysym) \
  (((unsigned)(keysym) >= XK_KP_Space) && ((unsigned)(keysym) <= XK_KP_Equal))

/***====================================================================***/

DeviceIntPtr
XkbLookupDevice(id)
    int id;
{
DeviceIntPtr coreKbd = (DeviceIntPtr)LookupKeyboardDevice();
DeviceIntPtr dev = NULL;
extern	DeviceIntPtr LookupDeviceIntRec();

   if (( id == XkbUseCoreKbd ) || (coreKbd->id==id))
	dev= coreKbd;
#ifdef XINPUT
   else {
	dev = LookupDeviceIntRec(id);
   }
#endif
   if (dev) {
	if ((!dev->key)||(!dev->key->xkbInfo))
	    dev= NULL;
   }
   return dev;
}

static void
XkbSetActionKeyMods(xkb,act,mods)
    XkbDescPtr	xkb;
    XkbAction *	act;
    unsigned	mods;
{
register unsigned	tmp;

    switch (act->type) {
	case XkbSA_SetMods: case XkbSA_LatchMods: case XkbSA_LockMods:
	    if (act->mods.flags&XkbSA_UseModMapMods)
		act->mods.real_mods= act->mods.mask= mods;
	    if ((tmp= XkbModActionVMods(&act->mods))!=0)
		act->mods.mask|= XkbMaskForVMask(xkb,tmp);
	    break;
	case XkbSA_ISOLock:
	    if (act->iso.flags&XkbSA_UseModMapMods)
		act->iso.real_mods= act->iso.mask= mods;
	    if ((tmp= XkbModActionVMods(&act->iso))!=0)
		act->iso.mask|= XkbMaskForVMask(xkb,tmp);
	    break;
    }
    return;
}

/***====================================================================***/

unsigned
XkbMaskForVMask(xkb,vmask)
    XkbDescPtr	xkb;
    unsigned	vmask;
{
register int i,bit;
register unsigned mask;
    
    for (mask=i=0,bit=1;i<XkbNumVirtualMods;i++,bit<<=1) {
	if ((vmask&bit)&&(xkb->server->vmods[i]!=XkbNoModifier)) {
	    mask|= (1<<xkb->server->vmods[i]);
	}
    }
    return mask;
}

static Bool
XkbVMUpdateKeyType(xkb,index,changes)
    XkbDescPtr		xkb;
    int			index;
    XkbChangesPtr	changes;
{
register unsigned i;
XkbKeyTypePtr	type;
CARD8		mask;

#ifdef DEBUG
    if (xkbDebugFlags>0)
	ErrorF("Updating key type %d due to virtual modifier change\n",index);
#endif
    type= &xkb->map->types[index];
    type->mask= type->real_mods|XkbMaskForVMask(xkb,type->vmods);
    if ((type->map_count>0)&&(type->vmods!=0)) {
	XkbKTMapEntryPtr entry;
	for (i=0,entry=type->map;i<type->map_count;i++,entry++) {
	    entry->mask=entry->real_mods|XkbMaskForVMask(xkb,entry->vmods);
	}
    }
    if (changes) {
	if (changes->map.changed&XkbKeyTypesMask) {
	    unsigned first,last;
	    first= last= i;
	    if (changes->map.first_type<first)
		first= changes->map.first_type;
	    if ((changes->map.first_type+changes->map.num_types-1)>last)
		last= changes->map.first_type+changes->map.num_types-1;
	    changes->map.first_type= first;
	    changes->map.num_types= last-first+1;
	}
	else {
	    changes->map.changed|= XkbKeyTypesMask;
	    changes->map.first_type= i;
	    changes->map.num_types= 1;
	}
    }
    return TRUE;
}

Bool
XkbApplyVirtualModChanges(info,changed,changes)
    XkbSrvInfoPtr	 info;
    unsigned		 changed;
    XkbChangesPtr	 changes;
{
register unsigned 	i,n,bit;
int 			lowChange,highChange;
XkbDescPtr		xkb;

    xkb= &info->desc;
    for (i=0,bit=1;i<XkbNumVirtualMods;i++,bit<<=1) {
	if ((changed&bit)==0)
	    continue;
#ifdef DEBUG
	if (xkbDebugFlags)
	    ErrorF("Should be applying: change vmod %d to 0x%x\n",i,
					xkb->server->vmods[i]);
#endif
    }
    for (i=0;i<xkb->map->num_types;i++) {
	if (xkb->map->types[i].vmods & changed)
	    XkbVMUpdateKeyType(xkb,i,changes);
    }

    if (changed&xkb->ctrls->internal_vmods) {
	CARD8 newMask;
	XkbControlsPtr ctrl= xkb->ctrls;

	newMask= XkbMaskForVMask(xkb,ctrl->internal_vmods);
	newMask|= ctrl->internal_real_mods;
	if (newMask!=ctrl->internal_mask) {
	    ctrl->internal_mask= newMask;
	    changes->ctrls.changed_ctrls|= XkbInternalModsMask;
	    /* 3/1/94 (ef) - XXX! Don't forget to (possibly) change state */
	}
    }
    if (changed&xkb->ctrls->ignore_lock_vmods) {
	CARD8 newMask;
	XkbControlsPtr ctrl= xkb->ctrls;

	newMask= XkbMaskForVMask(xkb,ctrl->ignore_lock_vmods);
	newMask|= ctrl->ignore_lock_real_mods;
	if (newMask!=ctrl->ignore_lock_mask) {
	    ctrl->ignore_lock_mask= newMask;
	    changes->ctrls.changed_ctrls|= XkbIgnoreLockModsMask;
	    /* 3/1/94 (ef) - XXX! Don't forget to (possibly) change state */
	}
    }
    for (i=0;i<XkbNumIndicators;i++) {
	if (xkb->indicators->maps[i].vmods&changed) {
	    CARD8 newMask;
	    XkbIndicatorMapPtr map= &xkb->indicators->maps[i];

	    newMask= XkbMaskForVMask(xkb,map->vmods)|map->real_mods;
	    if (newMask!=map->mask)
		map->mask= newMask;
	    changes->indicators.map_changes|= (1<<i);
	}
	/* 3/1/94 (ef) - XXX! If indicator maps change, don't forget to    */
	/*               change the indicator state (and send events)      */
    }
    for (i=0,bit=1;i<XkbNumVirtualMods;i++,bit<<=1) {
	int realMod= xkb->server->vmods[i];
	XkbCompatPtr	compat;
	if (((changed&bit)==0)||(realMod==XkbNoModifier))
	    continue;
	compat= xkb->compat;
	if (compat->vmod_compat[i].mods||compat->vmod_compat[i].groups){
	    compat->mod_compat[realMod]= &compat->vmod_compat[i];
	}
	changes->compat.changed_mods|= (1<<realMod);
	/* 3/1/94 (ef) - XXX! If modifier compatibility maps change, don't */
	/*               forget to change the compatibility state          */
    }
    lowChange= -1;
    for (i=xkb->min_key_code;i<=xkb->max_key_code;i++) {
	if (XkbKeyHasActions(xkb,i)) {
	    register XkbAction *pAct;
	    pAct= XkbKeyActionsPtr(xkb,i);
	    for (n=XkbKeyNumActions(xkb,i);n>0;n--,pAct++) {
		register unsigned tmp;
		switch (pAct->type) {
		    case XkbSA_SetMods: case XkbSA_LatchMods: case XkbSA_LockMods:
			if (((tmp= XkbModActionVMods(&pAct->mods))!=0)&&
							((tmp&changed)!=0)) {
			    pAct->mods.mask= pAct->mods.real_mods;
			    pAct->mods.mask|= XkbMaskForVMask(xkb,tmp);
			    if (lowChange<0)	lowChange= i;
			    highChange= i;
			}
			break;
		    case XkbSA_ISOLock:
			if (((tmp= XkbModActionVMods(&pAct->iso))!=0)&&
							((tmp&changed)!=0)) {
			    pAct->iso.mask= pAct->iso.real_mods;
			    pAct->iso.mask|= XkbMaskForVMask(xkb,tmp);
			    if (lowChange<0)	lowChange= i;
			    highChange= i;
			}
			break;
		}
	    }
	}
    }
    if (lowChange>0) {	/* something changed */
	if (changes->map.changed&XkbKeyActionsMask) {
	    i= changes->map.first_key_act;
	    if (i<lowChange)
		lowChange= i;
	    i+= changes->map.num_key_acts-1;
	    if (i>highChange)
		highChange= i;
	}
	changes->map.changed|= XkbKeyActionsMask;
	changes->map.first_key_act= lowChange;
	changes->map.num_key_acts= (highChange-lowChange)+1;
    }
    return 1;
}

/***====================================================================***/

XkbAction *
_XkbNewActionsForKey(xkb,key,needed)
    XkbDescRec *xkb;
    unsigned key;
    unsigned needed;
{
register unsigned	i,nActs;
XkbAction *		newActs;

    if ((xkb->server->key_acts[key]!=0)&&(XkbKeyNumSyms(xkb,key)>=needed)) {
	return XkbKeyActionsPtr(xkb,key);
    }
    if (xkb->server->size_acts-xkb->server->num_acts>=needed) {
	xkb->server->key_acts[key]= xkb->server->num_acts;
	xkb->server->num_acts+= needed;
	return &xkb->server->acts[xkb->server->key_acts[key]];
    }
    xkb->server->size_acts+= 32;
    newActs = (XkbAction *)Xcalloc(xkb->server->size_acts*sizeof(XkbAction));
    newActs[0].type = XkbSA_NoAction;
    nActs = 1;
    for (i=xkb->min_key_code;i<=xkb->max_key_code;i++) {
	if (xkb->server->key_acts[i]!=0) {
	    memcpy(&newActs[nActs],XkbKeyActionsPtr(xkb,i),
				XkbKeyNumActions(xkb,i)*sizeof(XkbAction));
	    xkb->server->key_acts[i]= nActs;
	    if (i!=key)	nActs+= XkbKeyNumActions(xkb,i);
	    else	nActs+= needed;
	}
    }
    free(xkb->server->acts);
    xkb->server->acts = newActs;
    xkb->server->num_acts = nActs;
    return &xkb->server->acts[xkb->server->key_acts[key]];
}

KeySym *
_XkbNewSymsForKey(xkb,key,needed)
    XkbDescRec *xkb;
    unsigned key;
    unsigned needed;
{
register unsigned	i,nSyms;
KeySym	*		newSyms;

    if (needed==0) {
	if (xkb->server->key_acts[key]!=0)
	    _XkbNewActionsForKey(xkb,key,needed);
	xkb->map->key_sym_map[key].offset= 0;
	return xkb->map->syms;
    }
    else if (XkbKeyNumSyms(xkb,key)>=needed) {
	return XkbKeySymsPtr(xkb,key);
    }
    if (xkb->server->key_acts[key]!=0)
	_XkbNewActionsForKey(xkb,key,needed);

    if (xkb->map->size_syms-xkb->map->num_syms>=needed) {
	xkb->map->key_sym_map[key].offset = xkb->map->num_syms;
	xkb->map->num_syms+= needed;
	return &xkb->map->syms[xkb->map->key_sym_map[key].offset];
    }
    xkb->map->size_syms+= 128;
    newSyms = (KeySym *)Xcalloc(xkb->map->size_syms*sizeof(KeySym));
    nSyms = 1;
    for (i=xkb->min_key_code;i<=xkb->max_key_code;i++) {
	if (xkb->map->key_sym_map[i].offset==0) {
	    if (i==key) {
		xkb->map->key_sym_map[i].offset= nSyms;
		nSyms+= needed;
	    }
	    continue;
	}
	else {
	    memcpy(&newSyms[nSyms],XkbKeySymsPtr(xkb,i),
					XkbKeyNumSyms(xkb,i)*sizeof(KeySym));
	    xkb->map->key_sym_map[i].offset = nSyms;
	}
	if (i!=key)	nSyms+= XkbKeyNumSyms(xkb,i);
	else		nSyms+= needed;
    }
    free(xkb->map->syms);
    xkb->map->syms = newSyms;
    xkb->map->num_syms = nSyms;
    return &xkb->map->syms[xkb->map->key_sym_map[key].offset];
}

void
_XkbResizeKeyType(xkb,ndx,nLevels)
    XkbDescRec	*xkb;
    unsigned	 ndx;
    unsigned	 nLevels;
{
XkbKeyTypePtr		type= &xkb->map->types[ndx];
unsigned 		nTotal;
KeySym	*		newSyms;
register unsigned	i,nSyms;

    if (type->group_width==nLevels)
	return;
    for (nTotal=0,i=xkb->min_key_code;i<=xkb->max_key_code;i++) {
	if (xkb->map->key_sym_map[i].kt_index==ndx) 
	     nTotal+= XkbKeyNumGroups(xkb,i)*nLevels;
	else nTotal+= XkbKeyNumSyms(xkb,i);
    }
    xkb->map->size_syms= (nTotal*12)/10;
    newSyms = (KeySym *)Xcalloc(xkb->map->size_syms*sizeof(KeySym));
    /* 9/3/93 (ef) -- XXX! deal with allocation failure */
    nSyms= 1;
    for (i=xkb->min_key_code;i<=xkb->max_key_code;i++) {
	if (xkb->map->key_sym_map[i].kt_index==ndx) {
	    register int g;
	    unsigned lastLevel,nGroups;

	    if (type->group_width>nLevels)	lastLevel= nLevels;
	    else				lastLevel= type->group_width;
	    nGroups= XkbKeyNumGroups(xkb,i);
	    for (g=0;g<nGroups;g++) {
		KeySym *pOld;
		pOld= XkbKeySymsPtr(xkb,i)+(g*XkbKeyGroupWidth(xkb,i));
		memcpy(&newSyms[nSyms+(nLevels*g)],pOld,nLevels*sizeof(KeySym));
	    }
	    xkb->map->key_sym_map[i].offset = nSyms;
	    nSyms+= nGroups*nLevels;
	}
	else {
	    memcpy(&newSyms[nSyms],XkbKeySymsPtr(xkb,i),
					XkbKeyNumSyms(xkb,i)*sizeof(KeySym));
	    xkb->map->key_sym_map[i].offset = nSyms;
	    nSyms+= XkbKeyNumSyms(xkb,i);
	}
    }
    free(xkb->map->syms);
    xkb->map->syms = newSyms;
    xkb->map->num_syms = nSyms;
    return;
}

/***====================================================================***/

unsigned
XkbIndicatorsToUpdate(keybd,modsChanged)
    DeviceIntRec		*keybd;
    unsigned long	 	 modsChanged;
{
register unsigned update = 0;
XkbSrvInfoRec	*xkb = keybd->key->xkbInfo;

    if (modsChanged&(XkbModifierStateMask|XkbGroupStateMask))
	update|= xkb->iAccel.usesEffective;
    if (modsChanged&(XkbModifierBaseMask|XkbGroupBaseMask))
	update|= xkb->iAccel.usesBase;
    if (modsChanged&(XkbModifierLatchMask|XkbGroupLatchMask))
	update|= xkb->iAccel.usesLatched;
    if (modsChanged&(XkbModifierLockMask|XkbGroupLockMask))
	update|= xkb->iAccel.usesLocked;
    if (modsChanged&XkbCompatStateMask)
	update|= xkb->iAccel.usesCompat;
    return update;
}

void
XkbSetIndicators(keybd,affect,values,pChanges)
    DeviceIntPtr		keybd;
    CARD32			affect;
    CARD32			values;
    XkbIndicatorChangesPtr	pChanges;
{
XkbSrvInfoRec	*xkb = keybd->key->xkbInfo;

    xkb->iStateExplicit&= ~affect;
    xkb->iStateExplicit|= (affect&values);

    XkbUpdateIndicators(keybd,affect,pChanges);
    return;
}

void
XkbUpdateIndicators(keybd,update,pChanges)
    DeviceIntPtr		keybd;
    register CARD32		update;
    XkbIndicatorChangesPtr	pChanges;
{
register int	i,bit;
XkbSrvInfoRec	*xkb = keybd->key->xkbInfo;
XkbIndicatorRec	*map = xkb->desc.indicators;
XkbStateRec	*kbdState = &xkb->state;
CARD32		 oldState;


    oldState= xkb->iStateEffective;
    for (i=0,bit=1;update;i++,bit<<=1) {
	if (update&bit) {
	    int on;
	    CARD8 mods,group;
	    XkbIndicatorMapRec *map= &xkb->desc.indicators->maps[i];
	    on= mods= group= 0;
	    if (map->which_mods&XkbIM_UseBase)
		mods|= kbdState->base_mods;
	    if (map->which_groups&XkbIM_UseBase)
		group|= (1L << kbdState->base_group);

	    if (map->which_mods&XkbIM_UseLatched)
		mods|= kbdState->latched_mods;
	    if (map->which_groups&XkbIM_UseLatched)
		group|= (1L << kbdState->latched_group);

	    if (map->which_mods&XkbIM_UseLocked)
		mods|= kbdState->locked_mods;
	    if (map->which_groups&XkbIM_UseLocked)
		group|= (1L << kbdState->locked_group);

	    if (map->which_mods&XkbIM_UseEffective)
		mods|= kbdState->mods;
	    if (map->which_groups&XkbIM_UseEffective)
		group|= (1L << kbdState->group);

	    if (map->which_mods&XkbIM_UseCompat)
		mods|= kbdState->compat_state;

	    if ((map->which_mods|map->which_groups)&XkbIM_UseAnyState) {
		on = (((map->mask&mods)!=0)||(map->mask==mods));
		on = on && (((map->groups&group)!=0)||(map->groups==0));
	    }
	    if (map->ctrls)
		on = on || (xkb->desc.ctrls->enabled_ctrls&map->ctrls);

	    if (on)	xkb->iStateAuto|= bit;
	    else	xkb->iStateAuto&= ~bit;

	    if ((map->flags&XkbIM_NoExplicit)==0) {
		if ((map->flags&XkbIM_NoAutomatic)!=0)
		    on= FALSE;
		on= on||((xkb->iStateExplicit&bit)!=0);
	    }

	    if (on)	xkb->iStateEffective|= bit;
	    else	xkb->iStateEffective&= ~bit;
	    update&= ~bit;
	}
    }
    if (xkb->iStateEffective!=oldState) {
	DDXUpdateIndicators(keybd,oldState,xkb->iStateEffective);
    }
    if (pChanges)
	pChanges->state_changes|= (xkb->iStateEffective^oldState);
    else if ((xkb->iStateEffective^oldState)!=0) {
	xkbIndicatorNotify	in;
	in.stateChanged= (xkb->iStateEffective^oldState);
	in.state = xkb->iStateEffective;
	in.mapChanged = 0;
	XkbSendIndicatorNotify(keybd,&in);
    }
    return;
}

void
XkbCheckIndicatorMaps(xkb,which)
    XkbSrvInfoPtr	xkb;
    unsigned		which;
{
register unsigned i,bit;
XkbIndicatorPtr	leds;

    leds= xkb->desc.indicators;
    for (i=0,bit=1;i<XkbNumIndicators;i++,bit<<=1) {
	if (which&bit) {
	    CARD8 which;
	    which= (leds->maps[i].which_mods|leds->maps[i].which_groups);

	    if (which&XkbIM_UseBase)
		 xkb->iAccel.usesBase|= bit;
	    else xkb->iAccel.usesBase&= ~bit;
	    if (which&XkbIM_UseLatched)
		 xkb->iAccel.usesLatched|= bit;
	    else xkb->iAccel.usesLatched&= ~bit;
	    if (which&XkbIM_UseLocked)
		 xkb->iAccel.usesLocked|= bit;
	    else xkb->iAccel.usesLocked&= ~bit;
	    if (which&XkbIM_UseEffective)
		 xkb->iAccel.usesEffective|= bit;
	    else xkb->iAccel.usesEffective&= ~bit;
	    if (which&XkbIM_UseCompat)
		 xkb->iAccel.usesCompat|= bit;
	    else xkb->iAccel.usesCompat&= ~bit;
	    if (leds->maps[i].ctrls)
		 xkb->iAccel.usesControls|= bit;
	    else xkb->iAccel.usesControls&= ~bit;

	    if (leds->maps[i].ctrls || 
		(leds->maps[i].which_groups && leds->maps[i].groups) ||
		(leds->maps[i].which_mods && leds->maps[i].real_mods))
		 xkb->iAccel.haveMap|= bit;
	    else xkb->iAccel.haveMap&= ~bit;
	    if (leds->maps[i].vmods!=0) {
		leds->maps[i].mask= leds->maps[i].real_mods;
		leds->maps[i].mask|= 
				XkbMaskForVMask(&xkb->desc,leds->maps[i].vmods);
	    }
	}
    }
    xkb->iAccel.usedComponents= 0;
    if (xkb->iAccel.usesBase)
	xkb->iAccel.usedComponents|= XkbModifierBaseMask|XkbGroupBaseMask;
    if (xkb->iAccel.usesLatched)
	xkb->iAccel.usedComponents|= XkbModifierLatchMask|XkbGroupLatchMask;
    if (xkb->iAccel.usesLocked)
	xkb->iAccel.usedComponents|= XkbModifierLockMask|XkbGroupLockMask;
    if (xkb->iAccel.usesEffective)
	xkb->iAccel.usedComponents|= XkbModifierStateMask|XkbGroupStateMask;
    if (xkb->iAccel.usesCompat)
	xkb->iAccel.usedComponents|= XkbCompatStateMask;
    return;
}

#if NeedFunctionPrototypes
void
XkbUpdateKeyTypesFromCore(	DeviceIntPtr	pXDev,
				KeyCode	 	first,
				CARD8	 	num,
				XkbChangesPtr	pChanges)
#else
void
XkbUpdateKeyTypesFromCore(pXDev,first,num,pChanges)
    DeviceIntPtr pXDev;
    KeyCode first;
    CARD8 num;
    XkbChangesPtr pChanges;
#endif
{
XkbDescRec *xkb;
XkbSymMapRec	*map;
unsigned	key,mapWidth,nLevels,nGroups;
KeySym	*pSym,*newSyms;
KeySym	core[4];

    xkb= &pXDev->key->xkbInfo->desc;
    xkb->min_key_code = pXDev->key->curKeySyms.minKeyCode;
    xkb->max_key_code = pXDev->key->curKeySyms.maxKeyCode;

    mapWidth= pXDev->key->curKeySyms.mapWidth;
    map= &xkb->map->key_sym_map[first];
    pSym= &pXDev->key->curKeySyms.map[(first-xkb->min_key_code)*mapWidth];
    for (key=first; key<(first+num); key++,map++,pSym+= mapWidth) {
	if (xkb->server->explicit[key]&XkbExplicitKeyTypeMask) {
	    if (key==first) {
		first++; num--;	/* don't report unchanged types */
	    }
	    continue;
	}
	core[0]= pSym[0];
	if (mapWidth>1)	core[1]= pSym[1];
	else		core[1]= NoSymbol;
	if (mapWidth>2)	core[2]= pSym[2];
	else		core[2]= NoSymbol;
	if (mapWidth>3)	core[3]= pSym[3];
	else		core[3]= NoSymbol;

	/* 3/24/94 (ef) -- Horrible kludge to fake core proto capitalization */
	if (((core[0]&0xffffff00)==0)&&(core[1]==NoSymbol)) {
	    if (isupper(core[0])) {
		core[1]= core[0];
		core[0]= tolower(core[0]);
	    }
	}
	if (((core[2]&0xffffff00)==0)&&(core[3]==NoSymbol)) {
	    if (isupper(core[2])) {
		core[3]= core[2];
		core[2]= tolower(core[2]);
	    }
	}
	if ((core[1]==NoSymbol)&&(core[3]==NoSymbol))
	     nLevels= 1;
	else nLevels= 2;
	if ((core[2]==NoSymbol)&&(core[3]==NoSymbol)) {
	     if ((core[0]==NoSymbol)&&(core[1]==NoSymbol))
		  nGroups= 0;
	     else nGroups= 1;
	}
	else nGroups= 2;
	if (nGroups>0) {
	    newSyms= _XkbNewSymsForKey(xkb,key,nGroups*nLevels);
	    if (nLevels==1) {
		newSyms[0]= core[0];
		if (nGroups>1)
		    newSyms[1]= core[2];
	    }
	    else {
		newSyms[0]= core[0];
		newSyms[1]= core[1];
		if (nGroups>1) {
		    newSyms[2]= core[2];
		    newSyms[3]= core[3];
		}
	    }
	}
	if ((nGroups>1)&&(xkb->ctrls->num_groups<2)) {
	   XkbControlsRec *ctrls= xkb->ctrls;
	   ctrls->num_groups= nGroups;
	   /* 8/19/93 (ef) -- XXX! generate ctrls notify here */
	}
	if (nLevels==1)
	     map->kt_index= XkbOneLevelIndex;
	else if (IsKeypadKey(newSyms[0])||IsKeypadKey(newSyms[1]))
	     map->kt_index= XkbKeypadIndex;
	else map->kt_index= XkbTwoLevelIndex;
	map->group_info = XkbSetNumGroups(map->group_info,nGroups);
    }
    if (pChanges->map.changed&XkbKeySymsMask) {
	CARD8 oldLast,newLast;
	oldLast = pChanges->map.first_key_sym+pChanges->map.num_key_syms-1;
	newLast = first+num-1;

	if (first<pChanges->map.first_key_sym)
	    pChanges->map.first_key_sym = first;
	if (oldLast>newLast)
	    newLast= oldLast;
	pChanges->map.num_key_syms = newLast-pChanges->map.first_key_sym+1;
    }
    else {
	pChanges->map.changed|= XkbKeySymsMask;
	pChanges->map.first_key_sym = first;
	pChanges->map.num_key_syms = num;
    }
    return;
}

/***====================================================================***/

static XkbSymInterpretPtr
_XkbFindMatchingInterp(xkb,sym,real_mods,level)
    XkbDescPtr	xkb;
    KeySym	sym;
    CARD8	real_mods;
    CARD8	level;
{
register unsigned	 i;
XkbSymInterpretPtr	 interp,rtrn;
CARD8			 mods;

    rtrn= NULL;
    interp= xkb->compat->sym_interpret;
    for (i=0;i<xkb->compat->num_si;i++,interp++) {
	if ((interp->sym==NoSymbol)||(sym==interp->sym)) {
	    int match;
	    if ((level==0)||((interp->match&XkbSI_LevelOneOnly)==0))
		 mods= real_mods;
	    else mods= 0;
	    switch (interp->match&XkbSI_OpMask) {
		case XkbSI_NoneOf:
		    match= ((interp->mods&mods)==0);
		    break;
		case XkbSI_AnyOfOrNone:
		    match= ((mods==0)||((interp->mods&mods)!=0));
		    break;
		case XkbSI_AnyOf:
		    match= ((interp->mods&mods)!=0);
		    break;
		case XkbSI_AllOf:
		    match= ((interp->mods&mods)==interp->mods);
		    break;
		case XkbSI_Exactly:
		    match= (interp->mods==mods);
		    break;
		default:
		    ErrorF("Illegal match in UpdateActions\n");
		    match= 0;
		    break;
	    }
	    if (match) {
		if (interp->sym!=NoSymbol) {
		    return interp;
		}
		else if (rtrn==NULL) {
		    rtrn= interp;
		}
	    }
	}
    }
    return rtrn;
}

#if NeedFunctionPrototypes
void
XkbUpdateActions(	DeviceIntPtr	 pXDev,
			KeyCode		 first,
			CARD8		 num,
			XkbChangesPtr	 pChanges)
#else
void
XkbUpdateActions(pXDev,first,num,pChanges)
    DeviceIntPtr pXDev;
    KeyCode first;
    CARD8 num;
    XkbChangesPtr pChanges;
#endif
{
XkbDescRec *xkb;
register unsigned	i,key,n;
unsigned	 	nSyms,found;
KeySym	*		pSym;
XkbSymInterpretPtr	interps[8];
CARD8		 	mods,*repeat,explicit;
CARD16		 	changedVMods;

    xkb= &pXDev->key->xkbInfo->desc;
    repeat= pXDev->key->xkbInfo->repeat;
    changedVMods= 0;
    if (XkbComputeAutoRepeat && pXDev->kbdfeed)
	memcpy(repeat,pXDev->kbdfeed->ctrl.autoRepeats,32);

    for (key=first;key<(first+num);key++) {
	if ((xkb->server->explicit[key]&XkbExplicitInterpretMask)!=0) {
	    if (key==first) {
		first++; num--;	/* don't report unchanged keys */
	    }
	    continue;
	}
	explicit= xkb->server->explicit[key];
	mods= pXDev->key->modifierMap[key];
	pSym= XkbKeySymsPtr(xkb,key);
	nSyms= XkbKeyNumSyms(xkb,key);
	found= 0;
	for (n=0;n<nSyms;n++,pSym++) {
	    CARD8 level= (n%XkbKeyGroupWidth(xkb,key));
	    interps[n]= NULL;
	    if (*pSym!=NoSymbol) {
		interps[n]= _XkbFindMatchingInterp(xkb,*pSym,mods,level);
		if (interps[n]&&interps[n]->act.type!=XkbSA_NoAction)
		     found++;
		else interps[n]= NULL;
	    }
	}
	if (!found) {
	    xkb->server->key_acts[key]= 0;
	}
	else {
	    XkbAction *pActs;
	    pActs= _XkbNewActionsForKey(xkb,key,nSyms);
	    for (i=0;i<nSyms;i++) {
		if (interps[i]) {
		    unsigned effMods;

		    pActs[i]= *((XkbAction *)&interps[i]->act);
		    if ((i==0)||((interps[i]->match&XkbSI_LevelOneOnly)==0))
			 effMods= mods;
		    else effMods= 0;
		    XkbSetActionKeyMods(xkb,&pActs[i],effMods);
		}
		else pActs[i].type= XkbSA_NoAction;
	    }
	    if (interps[0]) {
		if ((interps[0]->flags&XkbSI_LockingKey)&&
		    ((explicit&XkbExplicitBehaviorMask)==0)) {
		    xkb->server->behaviors[key].type= XkbKB_Lock;
		}
		if (XkbComputeAutoRepeat&&
		    ((explicit&XkbExplicitAutorepeatMask)==0)) {
		    if (interps[0]->flags&XkbSI_Autorepeat)
			 repeat[key/8]|= (1<<(key%8));
		    else repeat[key/8]&= ~(1<<(key%8));
		}
		if (interps[0]->virtual_mod!=XkbNoModifier) {
		    register CARD8 bit,modNdx,old;

		    if (mods==0)	modNdx= XkbNoModifier;
		    else {
			for (modNdx=7,bit=0x80;modNdx>=0;modNdx--,bit>>=1) {
			    if (mods&bit)
				break;
			}
		    }
		    old= xkb->server->vmods[interps[0]->virtual_mod];
		    if (old!=modNdx) {
			xkb->server->vmods[interps[0]->virtual_mod]=modNdx;
			changedVMods|= (1<<interps[0]->virtual_mod);
		    }
		}
	    }
	}
	if ((!found)||(interps[0]==NULL)) {
	    if (XkbComputeAutoRepeat&&(explicit&XkbExplicitAutorepeatMask)==0)
		repeat[key/8]|= (1<<(key%8));
	    if (((explicit&XkbExplicitBehaviorMask)==0)&&
		(xkb->server->behaviors[key].type==XkbKB_Lock)) {
		xkb->server->behaviors[key].type= XkbKB_Default;
	    }
	}
    }

    if (XkbComputeAutoRepeat && pXDev->kbdfeed) {
	int realRepeat= pXDev->kbdfeed->ctrl.autoRepeat;
	if (XkbUsesSoftRepeat(pXDev))
	    pXDev->kbdfeed->ctrl.autoRepeat= FALSE;
        memcpy(pXDev->kbdfeed->ctrl.autoRepeats,repeat, 32);
	(*pXDev->kbdfeed->CtrlProc)(pXDev, &pXDev->kbdfeed->ctrl);
	pXDev->kbdfeed->ctrl.autoRepeat= realRepeat;
    }
    if (changedVMods) {
	XkbApplyVirtualModChanges(pXDev->key->xkbInfo,changedVMods,pChanges);
    }
    if (pChanges->map.changed&XkbKeyActionsMask) {
	CARD8 oldLast,newLast;
	oldLast= pChanges->map.first_key_act+pChanges->map.num_key_acts-1;
	newLast = first+num-1;

	if (first<pChanges->map.first_key_act)
	    pChanges->map.first_key_act = first;
	if (newLast>oldLast)
	    newLast= oldLast;
	pChanges->map.num_key_acts= newLast-pChanges->map.first_key_act+1;
    }
    else {
	pChanges->map.changed|= XkbKeyActionsMask;
	pChanges->map.first_key_act = first;
	pChanges->map.num_key_acts = num;
    }
    return;
}

void
XkbUpdateCoreDescription(keybd)
    DeviceIntPtr keybd;
{
    /* 8/3/93 (ef) -- XXX! Not implemented yet */
}

void
XkbSetRepeatKeys(pXDev,onoff)
    DeviceIntPtr pXDev;
    int		 onoff;
{
    if (pXDev && pXDev->key && pXDev->key->xkbInfo) {
	unsigned old,new;
	new= old= pXDev->key->xkbInfo->desc.ctrls->enabled_ctrls;
	if (onoff)	new|= XkbRepeatKeysMask;
	else		new&= ~XkbRepeatKeysMask;
	if (new!=old) {
	    xkbControlsNotify	cn;
	    cn.changedControls=	0;
	    cn.enabledControlChanges= XkbRepeatKeysMask;
	    XkbSendControlsNotify(pXDev,&cn);
	}
    }
    return;
}

int
XkbUsesSoftRepeat(pXDev)
    DeviceIntPtr pXDev;
{
    if (pXDev && pXDev->kbdfeed ) {
	if (pXDev->kbdfeed->ctrl.autoRepeat) {
	    if (pXDev->key && pXDev->key->xkbInfo) {
		XkbDescPtr	xkb;
		xkb= &pXDev->key->xkbInfo->desc;
		return ((xkb->ctrls->enabled_ctrls&XkbRepeatKeysMask)!=0);
	    }
	}
    }
    return 0;
}

#if NeedFunctionPrototypes
void
XkbApplyMappingChange(	DeviceIntRec	*kbd,
			CARD8		 request,
			KeyCode		 firstKey,
			CARD8		 num)
#else
void
XkbApplyMappingChange(kbd,request,firstKey,num)
    DeviceIntPtr kbd;
    CARD8 request;
    KeyCode firstKey;
    CARD8 num;
#endif
{
XkbChangesRec	 changes;
int		 req;

    if (kbd->key->xkbInfo==NULL)
	XkbInitDevice(kbd);
    bzero(&changes,sizeof(XkbChangesRec));
    if (request==MappingKeyboard) {
	XkbUpdateKeyTypesFromCore(kbd,firstKey,num,&changes);
	XkbUpdateActions(kbd,firstKey,num,&changes);
	req= X_ChangeKeyboardMapping;
    }
    else if (request==MappingModifier) {
	XkbDescRec *desc= &kbd->key->xkbInfo->desc;
	num = desc->max_key_code-desc->min_key_code+1;
	XkbUpdateActions(kbd,desc->min_key_code,num,&changes);
	req= X_SetModifierMapping;
    }
    /* 3/26/94 (ef) -- XXX! Doesn't deal with input extension requests */
    XkbSendNotification(kbd,&changes,0,0,req,0);
    return;
}

void
XkbDisableComputedAutoRepeats()
{
     XkbComputeAutoRepeat = 0;
}

unsigned
XkbStateChangedFlags(old,new)
    XkbStateRec *old;
    XkbStateRec *new;
{
int		groupUnlatch,changed;

    changed=(old->group!=new->group?XkbGroupStateMask:0);
    changed|=(old->base_group!=new->base_group?XkbGroupBaseMask:0);
    changed|=(old->latched_group!=new->latched_group?XkbGroupLatchMask:0);
    changed|=(old->locked_group!=new->locked_group?XkbGroupLockMask:0);
    changed|=(old->mods!=new->mods?XkbModifierStateMask:0);
    changed|=(old->base_mods!=new->base_mods?XkbModifierBaseMask:0);
    changed|=(old->latched_mods!=new->latched_mods?XkbModifierLatchMask:0);
    changed|=(old->locked_mods!=new->locked_mods?XkbModifierLockMask:0);
    changed|=(old->compat_state!=new->compat_state?XkbCompatStateMask:0);
    return changed;
}

void
XkbComputeCompatState(xkb)
    XkbSrvInfoRec	*xkb;
{
    register int i,bit;
    CARD8 grabGroup,lookupGroup;
    XkbStateRec	*state= &xkb->state;
    XkbModCompatRec *map;

    lookupGroup= (xkb->lookupState>>13)&0x7;
    grabGroup= (xkb->grabState>>13)&0x7;
    state->compat_state = 0;
    xkb->compatLookupState= 0;
    xkb->compatGrabState= 0;
    for (i=0,bit=1;i<8;i++,bit<<=1) {
	map= xkb->desc.compat->mod_compat[i];
	if (map==NULL) {
	    map= &xkb->desc.compat->real_mod_compat[i];
	}
	if ((map->mods&state->mods)||(map->groups&(1<<state->group)))
	    state->compat_state|= bit;

	if ((map->mods&xkb->lookupState)||(map->groups&(1<<lookupGroup)))
	    xkb->compatLookupState|= bit;

	if ((map->mods&xkb->grabState)||(map->groups&(1<<grabGroup)))
	    xkb->compatGrabState|= bit;
    }
}

static void
_XkbAdjustGroup(pGroup,nGroups,groupsWrap)
    CARD8 *	pGroup;
    unsigned	nGroups;
    Bool	groupsWrap;
{
register int group = XkbCharToInt(*pGroup);

    if (group<0) {
	while ( group < 0 )  {
	    if ( groupsWrap )	group+= nGroups;
	    else	    	group= 0;
	}
    }
    else if (((unsigned)group)>=nGroups) {
	if ( groupsWrap )	group%= nGroups;
	else 			group= nGroups-1;
    }
    *pGroup= group;
    return;
}

void
XkbComputeDerivedState(xkb)
    XkbSrvInfoRec	*xkb;
{
XkbStateRec	*state= &xkb->state;
XkbControlsRec	*ctrls= xkb->desc.ctrls;
Bool		wrap= ((ctrls->enabled_ctrls&XkbGroupsWrapMask)!=0);

    state->mods= (state->base_mods|state->latched_mods);
    state->mods|= state->locked_mods;
    xkb->lookupState=state->mods&(~ctrls->internal_mask);
    xkb->grabState= xkb->lookupState&(~ctrls->ignore_lock_mask);
    xkb->grabState|= (state->base_mods&ctrls->ignore_lock_mask);

    _XkbAdjustGroup(&state->base_group,ctrls->num_groups,wrap);
    _XkbAdjustGroup(&state->latched_group,ctrls->num_groups,wrap);
    _XkbAdjustGroup(&state->locked_group,ctrls->num_groups,wrap);

    xkb->state.group = state->base_group+state->latched_group;
    xkb->state.group+= state->locked_group;
    _XkbAdjustGroup(&state->group,ctrls->num_groups,wrap);

    xkb->lookupState|= ((state->group&0x7)<<13);
    xkb->grabState|= ((state->group&0x7)<<13);

    XkbComputeCompatState(xkb);
    return;
}

/***====================================================================***/

void
XkbSetPhysicalLockingKey(dev,key)
   DeviceIntPtr	dev;
   unsigned	key;
{
XkbSrvInfoRec	*xkb = dev->key->xkbInfo;

    if (( xkb && xkb->desc.server )&&
	( (key>=xkb->desc.min_key_code) && (key<=xkb->desc.max_key_code))) {
	XkbAction	*pActs;
	xkb->desc.server->behaviors[key].type= XkbKB_Lock|XkbKB_Permanent;
    }
    else ErrorF("Internal Error!  Bad XKB info in SetPhysicalLockingKey\n");
    return;
}
