/* $XConsortium: xkbPrKeyEv.c,v 1.1 94/04/01 18:46:13 erik Exp $ */
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


/***====================================================================***/

static XkbAction
XkbKeyAction(xkb,xkbState,keycode)
    XkbDescRec	*xkb;
    XkbStateRec *xkbState;
    CARD8 	 keycode;
{
int		n;
int		col;
XkbKeyTypeRec	*type;
XkbAction	*pActs;
static XkbAction fake;

    n= XkbKeyNumActions(xkb,keycode);
    pActs= XkbKeyActionsPtr(xkb,keycode);
    col= 0;
    if (n>1) {
	type= XkbKeyKeyType(xkb,keycode);
	if (type->map!=NULL) {
	    register unsigned		i,mods;
	    register XkbKTMapEntryPtr	entry;
	    mods= xkbState->mods&type->mask;
	    for (entry= type->map,i=0;i<type->map_count;i++,entry++) {
		if ((entry->active)&&(entry->mask==mods)) {
		    col= entry->level;
		    break;
		}
	    }
	}
	if ((xkbState->group!=0)&&(XkbKeyNumGroups(xkb,keycode)>(unsigned)1)) {
	    unsigned effectiveGroup = xkbState->group;
	    if (effectiveGroup>=XkbKeyNumGroups(xkb,keycode)) {
		if ( XkbKeyGroupsWrap(xkb,keycode) ) {
		     effectiveGroup %= XkbKeyNumGroups(xkb,keycode);
		}
		else effectiveGroup  = XkbKeyNumGroups(xkb,keycode)-1;
	    }
	    col+= (effectiveGroup*type->group_width);
	}
    }
    if (XkbIsPtrAction(&pActs[col])&&
	     ((xkb->ctrls->enabled_ctrls&XkbMouseKeysMask)==0)) {
	fake.type = XkbSA_NoAction;
	return fake;
    }
    if (XkbDisableLockActions) {
	switch (pActs[col].type) {
	    case XkbSA_LockMods:
		fake.mods.type  = XkbSA_SetMods;
		fake.mods.flags = 0;
		fake.mods.mask  = pActs[col].mods.mask;
		return fake;
	    case XkbSA_LatchMods:
		fake.mods.type  = XkbSA_SetMods;
		fake.mods.flags = 0;
		fake.mods.mask  = pActs[col].mods.mask;
		return fake;
	    case XkbSA_ISOLock:
		if (pActs[col].iso.flags&XkbSA_ISODfltIsGroup) {
		     fake.group.type = XkbSA_SetGroup;
		     fake.group.flags = pActs[col].iso.flags&XkbSA_GroupAbsolute;
		     XkbSASetGroup(&fake.group,XkbSAGroup(&pActs[col].iso));
		}
		else {
		     fake.mods.type  = XkbSA_SetMods;
		     fake.mods.flags = 0;
		     fake.mods.mask  = pActs[col].iso.mask;
		}
		return fake;
	    case XkbSA_LockGroup:
	    case XkbSA_LatchGroup:
		XkbSASetGroup(&fake.group,XkbSAGroup(&pActs[col].iso));
		fake.group.type = XkbSA_SetGroup;
		return fake;
	}
    }
    else 
    if (xkb->ctrls->enabled_ctrls&XkbStickyKeysMask) {
	if (pActs[col].any.type==XkbSA_SetMods) {
	    fake.mods.type = XkbSA_LatchMods;
	    fake.mods.flags= XkbSA_ClearLocks|XkbSA_LatchToLock;
	    fake.mods.mask = pActs[col].mods.mask;
	    return fake;
	}
	if (pActs[col].any.type==XkbSA_SetGroup) {
	    fake.group.type = XkbSA_LatchGroup;
	    fake.group.flags= XkbSA_ClearLocks|XkbSA_LatchToLock;
	    XkbSASetGroup(&fake.group,XkbSAGroup(&pActs[col].group));
	    return fake;
	}
    }
    return pActs[col];
}

#define	SYNTHETIC_KEYCODE	1

typedef struct _XkbFilter {
	CARD8			  what;
	CARD8			  keycode;
	CARD8			  active;
	CARD8			  filterOthers;
	CARD32			  priv;
	XkbAction		  upAction;
	int			(*filter)();
	struct _XkbFilter	 *next;
} XkbFilter;

static int
_XkbFilterSetState(xkb,filter,keycode,pAction)
    XkbSrvInfoRec	*xkb;
    XkbFilter		*filter;
    CARD8		 keycode;
    XkbAction		*pAction;
{

    if (filter->keycode==0) {		/* initial press */
	filter->keycode = keycode;
	filter->active = 1;
	filter->filterOthers = ((pAction->mods.mask&XkbSA_ClearLocks)!=0);
	filter->priv = 0;
	filter->filter = _XkbFilterSetState;
	if (pAction->type==XkbSA_SetMods) {
	    filter->upAction = *pAction;
	    xkb->setMods= pAction->mods.mask;
	}
	else {
	    xkb->groupChange = XkbSAGroup(&pAction->group);
	    if (pAction->group.flags&XkbSA_GroupAbsolute)
		xkb->groupChange-= xkb->state.base_group;
	    filter->upAction= *pAction;
	    XkbSASetGroup(&filter->upAction.group,xkb->groupChange);
	}
    }
    else if (filter->keycode==keycode) {
	if (filter->upAction.type==XkbSA_SetMods) {
	    xkb->clearMods = filter->upAction.mods.mask;
	    if (filter->upAction.mods.flags&XkbSA_ClearLocks) {
		xkb->state.locked_mods&= ~filter->upAction.mods.mask;
	    }
	}
	else {
	    if (filter->upAction.group.flags&XkbSA_ClearLocks) {
		xkb->state.locked_group = 0;
	    }
	    xkb->groupChange = -XkbSAGroup(&filter->upAction.group);
	}
	filter->active = 0;
    }
    else {
	filter->upAction.mods.flags&= ~XkbSA_ClearLocks;
	filter->filterOthers = 0;
    }
    return 1;
}

#define	LATCH_KEY_DOWN	1
#define	LATCH_PENDING	2
#define	NO_LATCH	3

static int
_XkbFilterLatchState(xkb,filter,keycode,pAction)
    XkbSrvInfoRec	*xkb;
    XkbFilter		*filter;
    CARD8		 keycode;
    XkbAction		*pAction;
{

    if (filter->keycode==0) {			/* initial press */
	filter->keycode = keycode;
	filter->active = 1;
	filter->filterOthers = 1;
	filter->priv = LATCH_KEY_DOWN;
	filter->filter = _XkbFilterLatchState;
	if (pAction->type==XkbSA_LatchMods) {
	    filter->upAction = *pAction;
	    xkb->setMods = pAction->mods.mask;
	}
	else {
	    xkb->groupChange = XkbSAGroup(&pAction->group);
	    if (pAction->group.flags&XkbSA_GroupAbsolute)
		 xkb->groupChange-= xkb->state.base_group;
	    filter->upAction= *pAction;
	    XkbSASetGroup(&filter->upAction.group,xkb->groupChange);
	}
    }
    else if ( pAction && (filter->priv==LATCH_PENDING) ) {
	if (pAction->type==XkbSA_NoAction) {
	    filter->active = 0;
	    if (filter->upAction.type==XkbSA_LatchMods)
		 xkb->state.latched_mods&= ~filter->upAction.mods.mask;
	    else xkb->state.latched_group-=XkbSAGroup(&filter->upAction.group);
	}
	else if ((pAction->type==filter->upAction.type)&&
		 (pAction->mods.flags==filter->upAction.mods.flags)&&
		 (pAction->mods.mask==filter->upAction.mods.mask)) {
	    if (filter->upAction.mods.flags&XkbSA_LatchToLock) {
		if (filter->upAction.type==XkbSA_LatchMods)
		     pAction->mods.type= XkbSA_LockMods;
		else pAction->group.type= XkbSA_LockGroup;
	    }
	    else {
		if (filter->upAction.type==XkbSA_LatchMods)
		     pAction->mods.type= XkbSA_SetMods;
		else pAction->group.type= XkbSA_SetGroup;
	    }
	    if (filter->upAction.type==XkbSA_LatchMods)
		 xkb->state.latched_mods&= ~filter->upAction.mods.mask;
	    else xkb->state.latched_group-=XkbSAGroup(&filter->upAction.group);
	    filter->active = 0;
	}
    }
    else if (filter->keycode==keycode) {	/* release */
	if (filter->upAction.type==XkbSA_LatchMods) {
	    xkb->clearMods = filter->upAction.mods.mask;
	    if ((filter->upAction.mods.flags&XkbSA_ClearLocks)&&
		 (xkb->clearMods&xkb->state.locked_mods)==xkb->clearMods) {
		xkb->state.locked_mods&= ~xkb->clearMods;
		filter->priv= NO_LATCH;
	    }
	}
	else {
	    xkb->groupChange = -XkbSAGroup(&filter->upAction.group);
	    if ((filter->upAction.group.flags&XkbSA_ClearLocks)&&
						(xkb->state.locked_group)) {
		xkb->state.locked_group = 0;
		filter->priv = NO_LATCH;
	    }
	}
	if (filter->priv==NO_LATCH) {
	    filter->active= 0;
	}
	else {
	    filter->priv= LATCH_PENDING;
	    if (filter->upAction.type==XkbSA_LatchMods)
		 xkb->state.latched_mods |= filter->upAction.mods.mask;
	    else xkb->state.latched_group+=XkbSAGroup(&filter->upAction.group);
	}
    }
    else if (filter->priv==LATCH_KEY_DOWN) {
	filter->priv= NO_LATCH;
	filter->filterOthers = 0;
    }
    return 1;
}

static int
_XkbFilterLockState(xkb,filter,keycode,pAction)
    XkbSrvInfoRec	*xkb;
    XkbFilter		*filter;
    CARD8		 keycode;
    XkbAction		*pAction;
{

    if (pAction&&(pAction->type==XkbSA_LockGroup)) {
	if (pAction->group.flags&XkbSA_GroupAbsolute)
	     xkb->state.locked_group= XkbSAGroup(&pAction->group);
	else xkb->state.locked_group+= XkbSAGroup(&pAction->group);
	return 1;
    }
    if (filter->keycode==0) {		/* initial press */
	unsigned tmp;

	filter->keycode = keycode;
	filter->active = 1;
	filter->filterOthers = 0;
	filter->priv = 0;
	filter->filter = _XkbFilterLockState;
	filter->upAction = *pAction;
	xkb->state.locked_mods^= pAction->mods.mask;
	xkb->setMods = pAction->mods.mask;
    }
    else if (filter->keycode==keycode) {
	filter->active = 0;
	xkb->clearMods = filter->upAction.mods.mask;
    }
    return 1;
}

#define	ISO_KEY_DOWN		0
#define	NO_ISO_LOCK		1

static int
_XkbFilterISOLock(xkb,filter,keycode,pAction)
    XkbSrvInfoRec	*xkb;
    XkbFilter		*filter;
    CARD8		 keycode;
    XkbAction		*pAction;
{

    if (filter->keycode==0) {		/* initial press */
	CARD8	flags = pAction->iso.flags;
	filter->keycode = keycode;
	filter->active = 1;
	filter->filterOthers = 1;
	filter->priv = ISO_KEY_DOWN;
	filter->upAction = *pAction;
	filter->filter = _XkbFilterISOLock;
	if (flags&XkbSA_ISODfltIsGroup) {
	    xkb->groupChange = XkbSAGroup(&pAction->iso);
	    xkb->setMods = 0;
	}
	else {
	    xkb->setMods = pAction->iso.mask;
	    xkb->groupChange = 0;
	}
	if ((!(flags&XkbSA_ISONoAffectMods))&&(xkb->state.base_mods)) {
	    filter->priv= NO_ISO_LOCK;
	    xkb->state.locked_mods^= xkb->state.base_mods;
	}
	if ((!(flags&XkbSA_ISONoAffectGroup))&&(xkb->state.base_group)) {
/* 6/22/93 (ef) -- lock groups if group key is down first */
	}
	if (!(flags&XkbSA_ISONoAffectPtr)) {
/* 6/22/93 (ef) -- lock mouse buttons if they're down */
	}
    }
    else if (filter->keycode==keycode) {
	CARD8	flags = filter->upAction.iso.flags;
	if (flags&XkbSA_ISODfltIsGroup) {
	    xkb->groupChange = -XkbSAGroup(&filter->upAction.iso);
	    xkb->clearMods = 0;
	    if (filter->priv==ISO_KEY_DOWN)
		xkb->state.locked_group+= XkbSAGroup(&filter->upAction.iso);
	}
	else {
	    xkb->clearMods= filter->upAction.iso.mask;
	    xkb->groupChange= 0;
	    if (filter->priv==ISO_KEY_DOWN)
		xkb->state.locked_mods^= filter->upAction.iso.mask;
	}
	filter->active = 0;
    }
    else if (pAction) {
	CARD8	flags = filter->upAction.iso.flags;
	switch (pAction->type) {
	    case XkbSA_SetMods: case XkbSA_LatchMods:
		if (!(flags&XkbSA_ISONoAffectMods)) {
		    pAction->type= XkbSA_LockMods;
		    filter->priv= NO_ISO_LOCK;
		}
		break;
	    case XkbSA_SetGroup: case XkbSA_LatchGroup:
		if (!(flags&XkbSA_ISONoAffectGroup)) {
		    pAction->type= XkbSA_LockGroup;
		    filter->priv= NO_ISO_LOCK;
		}
		break;
	    case XkbSA_PtrBtn:
		if (!(flags&XkbSA_ISONoAffectPtr)) {
		     pAction->type= XkbSA_LockPtrBtn;
		     filter->priv= NO_ISO_LOCK;
		}
		break;
	    case XkbSA_SetControls:
		if (!(flags&XkbSA_ISONoAffectCtrls)) {
		    pAction->type= XkbSA_LockControls;
		    filter->priv= NO_ISO_LOCK;
		}
		break;
	}
    }
    return 1;
}


static CARD32
_XkbPtrAccelExpire(timer,now,arg)
    OsTimerPtr	 timer;
    CARD32	 now;
    pointer	 arg;
{
XkbSrvInfoRec	*xkbInfo= (XkbSrvInfoRec *)arg;
XkbControlsRec	*ctrls= xkbInfo->desc.ctrls;
int	dx,dy;

    if (xkbInfo->mouseKey==0)
	return 0;

    if (xkbInfo->mouseKeysAccel) {
	if ((xkbInfo->mouseKeysCounter)<ctrls->mouse_keys_time_to_max) {
	    double step;
	    xkbInfo->mouseKeysCounter++;
	    step= xkbInfo->mouseKeysCurveFactor*
		 pow((double)xkbInfo->mouseKeysCounter,xkbInfo->mouseKeysCurve);
	    dx= ceil( ((double)xkbInfo->mouseKeysDX)*step );
	    dy= ceil( ((double)xkbInfo->mouseKeysDY)*step );
	}
	else {
	    dx= xkbInfo->mouseKeysDX*ctrls->mouse_keys_max_speed;
	    dy= xkbInfo->mouseKeysDY*ctrls->mouse_keys_max_speed;
	}
    }
    else {
	dx= xkbInfo->mouseKeysDX;
	dy= xkbInfo->mouseKeysDY;
    }
    DDXFakePointerMotion(dx,dy);
    return xkbInfo->desc.ctrls->mouse_keys_interval;
}

static int
_XkbFilterPointerMove(xkb,filter,keycode,pAction)
    XkbSrvInfoRec	*xkb;
    XkbFilter		*filter;
    CARD8		 keycode;
    XkbAction		*pAction;
{
    if (filter->keycode==0) {		/* initial press */
	filter->keycode = keycode;
	filter->active = 1;
	filter->filterOthers = 0;
	filter->priv=0;
	filter->filter = _XkbFilterPointerMove;
	filter->upAction= *pAction;
	xkb->mouseKeysCounter= 0;
	xkb->mouseKey= keycode;
	DDXFakePointerMotion(XkbPtrActionX(&pAction->ptr),
					XkbPtrActionY(&pAction->ptr));
	xkb->mouseKeysAccel= 
		((pAction->type==XkbSA_AccelPtr)||
		 ((pAction->type==XkbSA_MovePtr)&&
		  (xkb->desc.ctrls->enabled_ctrls&XkbMouseKeysAccelMask)));
	if (xkb->mouseKeysAccel) {
	    AccessXCancelRepeatKey(xkb,keycode);
	    xkb->mouseKeysDX= XkbPtrActionX(&pAction->ptr);
	    xkb->mouseKeysDY= XkbPtrActionY(&pAction->ptr);
	    xkb->mouseKeyTimer= TimerSet(xkb->mouseKeyTimer, 0,
				xkb->desc.ctrls->mouse_keys_delay,
				_XkbPtrAccelExpire,(pointer)xkb);
	}
    }
    else if (filter->keycode==keycode) {
	filter->active = 0;
	if (xkb->mouseKey==keycode) {
	    xkb->mouseKey= 0;
	    xkb->mouseKeyTimer= TimerSet(xkb->mouseKeyTimer, 0, 0,
							NULL, NULL);
	}
    }
    return 0;
}

static int
_XkbFilterPointerBtn(xkb,filter,keycode,pAction)
    XkbSrvInfoRec	*xkb;
    XkbFilter		*filter;
    CARD8		 keycode;
    XkbAction		*pAction;
{
    if (filter->keycode==0) {		/* initial press */
	int button = pAction->btn.button;
	if (button==XkbSA_UseDfltButton)
	    button = xkb->desc.ctrls->mouse_keys_dflt_btn;

	filter->keycode = keycode;
	filter->active = 1;
	filter->filterOthers = 0;
	filter->priv=0;
	filter->filter = _XkbFilterPointerBtn;
	filter->upAction= *pAction;
	filter->upAction.btn.button= button;
	switch (pAction->type) {
	    case XkbSA_LockPtrBtn:
		if ((xkb->lockedPtrButtons&(1<<button))==0) {
		    xkb->lockedPtrButtons|= (1<<button);
		    DDXFakePointerButton(ButtonPress,button);
		    filter->upAction.type= XkbSA_NoAction;
		}
		break;
	    case XkbSA_PtrBtn:
		DDXFakePointerButton(ButtonPress,button);
		break;
	    case XkbSA_ClickPtrBtn:
		{
		    register int i,nClicks;
		    nClicks= pAction->btn.count;
		    for (i=0;i<nClicks;i++) {
			DDXFakePointerButton(ButtonPress,button);
			DDXFakePointerButton(ButtonRelease,button);
		    }
		    filter->upAction.type= XkbSA_NoAction;
		}
		break;
	    case XkbSA_SetPtrDflt:
		{
		    XkbControlsRec	*ctrls= xkb->desc.ctrls;
		    XkbControlsRec	old;
		    xkbControlsNotify	cn;
		    old= *ctrls;
		    switch (pAction->dflt.affect) {
			case XkbSA_AffectDfltBtn:
			    if (pAction->dflt.flags&XkbSA_DfltBtnAbsolute)
				ctrls->mouse_keys_dflt_btn= 
					XkbSAPtrDfltValue(&pAction->dflt);
			    else {
				ctrls->mouse_keys_dflt_btn+=
					XkbSAPtrDfltValue(&pAction->dflt);
				if (ctrls->mouse_keys_dflt_btn>5)
				    ctrls->mouse_keys_dflt_btn= 5;
				else if (ctrls->mouse_keys_dflt_btn<1)
				    ctrls->mouse_keys_dflt_btn= 1;
			    }
			    break;
			default:
			    ErrorF(
		"Attempt to change unknown pointer default (%d) ignored\n",
							pAction->dflt.affect);
			    break;
		    }
		    if (XkbComputeControlsNotify(xkb->device,
						&old,xkb->desc.ctrls,
						&cn)) {
			cn.keycode = keycode;
			cn.eventType = KeyPress;
			cn.requestMajor = 0;
			cn.requestMinor = 0;
			XkbSendControlsNotify(xkb->device,&cn);
		    }
		}
		break;
	}
    }
    else if (filter->keycode==keycode) {
	int button= filter->upAction.btn.button;
	switch (filter->upAction.type) {
	    case XkbSA_LockPtrBtn:
		xkb->lockedPtrButtons&= ~(1<<button);
	    case XkbSA_PtrBtn:
		DDXFakePointerButton(ButtonRelease,button);
		break;
	}
	filter->active = 0;
    }
    return 0;
}

static int
_XkbFilterControls(xkb,filter,keycode,pAction)
    XkbSrvInfoRec	*xkb;
    XkbFilter		*filter;
    CARD8		 keycode;
    XkbAction		*pAction;
{
XkbControlsRec	old;
CARD16		change;

    old= *xkb->desc.ctrls;
    if (filter->keycode==0) {		/* initial press */
	filter->keycode = keycode;
	filter->active = 1;
	filter->filterOthers = 0;
	filter->priv = change= XkbActionCtrls(&pAction->ctrls);
	filter->filter = _XkbFilterControls;
	filter->upAction = *pAction;

	if (pAction->type==XkbSA_LockControls) {
	    filter->priv= (xkb->desc.ctrls->enabled_ctrls&change);
	    change&= ~xkb->desc.ctrls->enabled_ctrls;
	}

	if (change) {
	    xkbControlsNotify cn;
	    xkb->desc.ctrls->enabled_ctrls|= change;
	    if (XkbComputeControlsNotify(xkb->device,&old,xkb->desc.ctrls,
									&cn)) {
		cn.keycode = keycode;
		cn.eventType = KeyPress;
		cn.requestMajor = 0;
		cn.requestMinor = 0;
		XkbSendControlsNotify(xkb->device,&cn);
	    }
	    if (xkb->iAccel.usesControls)
		XkbUpdateIndicators(xkb->device,xkb->iAccel.usesControls,NULL);
	}
    }
    else if (filter->keycode==keycode) {
	change= filter->priv;
	if (change) {
	    xkbControlsNotify cn;
	    xkb->desc.ctrls->enabled_ctrls&= ~change;
	    if (XkbComputeControlsNotify(xkb->device,&old,xkb->desc.ctrls,
									&cn)) {
		cn.keycode = keycode;
		cn.eventType = KeyRelease;
		cn.requestMajor = 0;
		cn.requestMinor = 0;
		XkbSendControlsNotify(xkb->device,&cn);
	    }
	    if (xkb->iAccel.usesControls)
		XkbUpdateIndicators(xkb->device,xkb->iAccel.usesControls,NULL);
	}
	filter->keycode= 0;
	filter->active= 0;
    }
    return 0;
}

static int
_XkbFilterActionMessage(xkb,filter,keycode,pAction)
    XkbSrvInfoRec	*xkb;
    XkbFilter		*filter;
    CARD8		 keycode;
    XkbAction		*pAction;
{
XkbMessageAction *pMsg;

    if (filter->keycode==0) {		/* initial press */
	pMsg= &pAction->msg;
	if ((pMsg->flags&XkbSA_MessageOnRelease)||
	    ((pMsg->flags&XkbSA_MessageGenKeyEvent)==0)) {
	    filter->keycode = keycode;
	    filter->active = 1;
	    filter->filterOthers = 0;
	    filter->priv = 0;
	    filter->filter = _XkbFilterActionMessage;
	    filter->upAction = *pAction;
	}
	if (pMsg->flags&XkbSA_MessageOnPress)  {
	    xkbActionMessage msg;
	    msg.keycode= keycode;
	    msg.press= 1;
	    msg.keyEventFollows=((pMsg->flags&XkbSA_MessageGenKeyEvent)!=0);
	    memcpy((char *)msg.message,
				(char *)pMsg->message,XkbActionMessageLength);
	    XkbSendActionMessage(xkb->device,&msg);
	}
	return ((pAction->msg.flags&XkbSA_MessageGenKeyEvent)!=0);
    }
    else if (filter->keycode==keycode) {
	pMsg= &filter->upAction.msg;
	if (pMsg->flags&XkbSA_MessageOnRelease) {
	    xkbActionMessage msg;
	    msg.keycode= keycode;
	    msg.press= 0;
	    msg.keyEventFollows=((pMsg->flags&XkbSA_MessageGenKeyEvent)!=0);
	    memcpy((char *)msg.message,(char *)pMsg->message,
						XkbActionMessageLength);
	    XkbSendActionMessage(xkb->device,&msg);
	}
	filter->keycode= 0;
	filter->active= 0;
	return ((pAction->msg.flags&XkbSA_MessageGenKeyEvent)!=0);
    }
    return 0;
}

static	int		 szFilters = 0;
static	XkbFilter	*filters = NULL;

static XkbFilter *
_XkbNextFreeFilter()
{
register int i;

    if (szFilters==0) {
	szFilters = 4;
	filters = (XkbFilter *)Xcalloc(szFilters*sizeof(XkbFilter));
	/* 6/21/93 (ef) -- XXX! deal with allocation failure */
    }
    for (i=0;i<szFilters;i++) {
	if (!filters[i].active) {
	    filters[i].keycode = 0;
	    return &filters[i];
	}
    }
    szFilters*=2;
    filters= (XkbFilter *)Xrealloc(filters,szFilters*sizeof(XkbFilter));
    /* 6/21/93 (ef) -- XXX! deal with allocation failure */
    bzero(&filters[szFilters/2],(szFilters/2)*sizeof(XkbFilter));
    return &filters[szFilters/2];
}

static int
_XkbApplyFilters(xkb,keycode,pAction)
    XkbSrvInfoRec	*xkb;
    CARD8		 keycode;
    XkbAction		*pAction;
{
register int i,send;

    send= 1;
    for (i=0;i<szFilters;i++) {
	if ((filters[i].active)&&(filters[i].filter))
	    send= ((*filters[i].filter)(xkb,&filters[i],keycode,pAction)&&send);
    }
    return send;
}

void
XkbHandleActions(xE,keybd,count)
    xEvent	 *xE;
    DeviceIntPtr  keybd;
    int		  count;
{
int		key,bit,i;
register CARD8	realMods;
XkbSrvInfoRec	*xkb;
KeyClassRec	*keyc = keybd->key;
int		 changed,sendEvent;
CARD16		 data;
XkbStateRec	 oldState;
XkbAction	 act;
XkbFilter	*filter;

    key= xE->u.u.detail;
    realMods = keyc->modifierMap[key];
    keyc->modifierMap[key]= 0;
    xkb= keyc->xkbInfo;
    oldState= xkb->state;

    xkb->clearMods = xkb->setMods = 0;
    xkb->groupChange = 0;

    sendEvent = 1;

    if ( xE->u.u.type == KeyPress ) {
	act = XkbKeyAction(&xkb->desc,&xkb->state,key);
	sendEvent = _XkbApplyFilters(xkb,key,&act);
	if (sendEvent) {
	    switch (act.type) {
		case XkbSA_SetMods:
		case XkbSA_SetGroup:
		    filter = _XkbNextFreeFilter();
		    sendEvent = _XkbFilterSetState(xkb,filter,key,&act);
		    break;
		case XkbSA_LatchMods:
		case XkbSA_LatchGroup:
		    filter = _XkbNextFreeFilter();
		    sendEvent=_XkbFilterLatchState(xkb,filter,key,&act);
		    break;
		case XkbSA_LockMods:
		case XkbSA_LockGroup:
		    filter = _XkbNextFreeFilter();
		    sendEvent=_XkbFilterLockState(xkb,filter,key,&act);
		    break;
		case XkbSA_ISOLock:
		    filter = _XkbNextFreeFilter();
		    sendEvent=_XkbFilterISOLock(xkb,filter,key,&act);
		    break;
		case XkbSA_AccelPtr:
		case XkbSA_MovePtr:
		    filter = _XkbNextFreeFilter();
		    sendEvent= _XkbFilterPointerMove(xkb,filter,key,&act);
		    break;
		case XkbSA_PtrBtn:
		case XkbSA_ClickPtrBtn:
		case XkbSA_LockPtrBtn:
		case XkbSA_SetPtrDflt:
		    filter = _XkbNextFreeFilter();
		    sendEvent= _XkbFilterPointerBtn(xkb,filter,key,&act);
		    break;
		case XkbSA_Terminate:
		    sendEvent= DDXTerminateServer(keybd,key,&act);
		    break;
		case XkbSA_SwitchScreen:
		    sendEvent= DDXSwitchScreen(keybd,key,&act);
		    break;
		case XkbSA_SetControls:
		case XkbSA_LockControls:
		    filter = _XkbNextFreeFilter();
		    sendEvent=_XkbFilterControls(xkb,filter,key,&act);
		    break;
		case XkbSA_ActionMessage:
		    filter = _XkbNextFreeFilter();
		    sendEvent=_XkbFilterActionMessage(xkb,filter,key,&act);
		    break;
	    }
	}
    }
    else {
	sendEvent = _XkbApplyFilters(xkb,key,NULL);
    }

    xkb->state.base_group+= xkb->groupChange;
    if (xkb->setMods) {
	for (i=0,bit=1; xkb->setMods; i++,bit<<=1 ) {
	    if (xkb->setMods&bit) {
		keyc->modifierKeyCount[i]++;
		xkb->state.base_mods|= bit;
		xkb->setMods&= ~bit;
	    }
	}
    }
    if (xkb->clearMods) {
	for (i=0,bit=1; xkb->clearMods; i++,bit<<=1 ) {
	    if (xkb->clearMods&bit) {
		keyc->modifierKeyCount[i]--;
		if (keyc->modifierKeyCount[i]<=0) {
		    xkb->state.base_mods&= ~bit;
		    keyc->modifierKeyCount[i] = 0;
		}
		xkb->clearMods&= ~bit;
	    }
	}
    }

    keyc->modifierMap[key] = 0;
    if (sendEvent)
	CoreProcessKeyboardEvent(xE,keybd,count);
    keyc->modifierMap[key] = realMods;
    XkbComputeDerivedState(xkb);
    keyc->state= xkb->lookupState&0xE0FF;

    changed = XkbStateChangedFlags(&oldState,&xkb->state);
    if (changed) {
	xkbStateNotify	sn;
	sn.keycode= key;
	sn.eventType= xE->u.u.type;
	sn.requestMajor = sn.requestMinor = 0;
	sn.changed= changed;
	XkbSendStateNotify(keybd,&sn);
    }
    if (changed&xkb->iAccel.usedComponents) {
	changed= XkbIndicatorsToUpdate(keybd,changed);
	if (changed)
	    XkbUpdateIndicators(keybd,changed,NULL);
    }

    return;
}

void
XkbProcessKeyboardEvent(xE,keybd,count)
    xEvent *xE;
    DeviceIntPtr keybd;
    int count;
{
KeyClassRec	*keyc = keybd->key;
XkbSrvInfoRec	*xkb;
int		 key;
XkbBehavior	 behavior;

    xkb= keyc->xkbInfo;
    key= xE->u.u.detail;

    if ( (xkb->repeatKey==key) && (xE->u.u.type==KeyRelease) &&
	 ((xkb->desc.ctrls->enabled_ctrls&XkbRepeatKeysMask)==0) ) {
	AccessXCancelRepeatKey(xkb,key);
    }

    behavior= xkb->desc.server->behaviors[key];
    /* The "permanent" flag indicates a hard-wired behavior that occurs */
    /* below XKB, such as a key that physically locks.   XKB does not   */
    /* do anything to implement the behavior, but it *does* report that */
    /* key is hardwired */
    if ((behavior.type&XkbKB_Permanent)==0) {
	switch (behavior.type) {
	    case XkbKB_Default:
		if (( xE->u.u.type == KeyPress ) && 
		    (keyc->down[key>>3] & (1<<(key&7)))) {
		    xE->u.u.type = KeyRelease;
		    XkbHandleActions(xE,keybd,count);
		    xE->u.u.type = KeyPress;
		    XkbHandleActions(xE,keybd,count);
		    return;
		}
		else if ((xE->u.u.type==KeyRelease) &&
			(!(keyc->down[key>>3]&(1<<(key&7))))) {
		    xE->u.u.type = KeyPress;
		    XkbHandleActions(xE,keybd,count);
		    xE->u.u.type = KeyRelease;
		    XkbHandleActions(xE,keybd,count);
		    return;
		}
		break;
	    case XkbKB_Lock:
		if ( xE->u.u.type == KeyRelease )
		    return;
		else {
		    int	bit= 1<<(key&7);
		    if ( keyc->down[key>>3]&bit )
			xE->u.u.type= KeyRelease;
		}
		break;
	    case XkbKB_RadioGroup:
		if ( xE->u.u.type == KeyRelease )
		    return;
		else {
		    unsigned	ndx = behavior.data;
		    if ( ndx<xkb->nRadioGroups ) {
			XkbRadioGroupRec	*rg;
			register int i;
			rg = &xkb->radioGroups[ndx];
			if ( rg->currentDown == xE->u.u.detail ) {
			    xE->u.u.type = KeyRelease;
			    XkbHandleActions(xE,keybd,count);
			    rg->currentDown= 0;
			    return;
			}
			if ( rg->currentDown!=0 ) {
			    int key = xE->u.u.detail;
			    xE->u.u.type= KeyRelease;
			    xE->u.u.detail= rg->currentDown;
			    XkbHandleActions(xE,keybd,count);
			    xE->u.u.type= KeyPress;
			    xE->u.u.detail= key;
			}
			rg->currentDown= key;
		    }
		    else ErrorF("InternalError! Illegal radio group %d\n",ndx);
		}
		break;
	    default:
		ErrorF("unknown key behavior 0x%04x\n",behavior.type);
		break;
	}
    }
    XkbHandleActions(xE,keybd,count);
    return;
}

void
ProcessKeyboardEvent(xE,keybd,count)
    xEvent *xE;
    DeviceIntPtr keybd;
    int	count;
{
KeyClassRec	*keyc = keybd->key;
XkbSrvInfoRec	*xkb;
int		 key;
XkbAction	 act;

    xkb= keyc->xkbInfo;
    key= xE->u.u.detail;

    if (!(xkb->desc.ctrls->enabled_ctrls&ALL_FILTERED_MASK))
	XkbProcessKeyboardEvent(xE,keybd,count);
    else if (xE->u.u.type==KeyPress)
	AccessXFilterPressEvent(xE,keybd,count);
    else if (xE->u.u.type==KeyRelease)
	AccessXFilterReleaseEvent(xE,keybd,count);
    return;
}

int
XkbLatchModifiers(pXDev,mask,latches)
    DeviceIntPtr pXDev;
    CARD8 mask;
    CARD8 latches;
{
XkbSrvInfoRec	*xkb;
XkbFilter	*filter;
XkbAction	 act;
unsigned	 clear;

    if ( pXDev && pXDev->key && pXDev->key->xkbInfo ) {
	xkb = pXDev->key->xkbInfo;
	clear= (mask&(~latches));
	xkb->state.latched_mods&= ~clear;
	act.type = XkbSA_LatchMods;
	act.mods.flags = 0;
	act.mods.mask  = mask&latches;
	filter = _XkbNextFreeFilter();
	_XkbFilterLatchState(xkb,filter,SYNTHETIC_KEYCODE,&act);
	_XkbFilterLatchState(xkb,filter,SYNTHETIC_KEYCODE,NULL);
	return Success;
    }
    return BadValue;
}

int
XkbLatchGroup(pXDev,group)
    DeviceIntPtr  pXDev;
    int		  group;
{
XkbSrvInfoRec	*xkb;
XkbFilter	*filter;
XkbAction	 act;

    if ( pXDev && pXDev->key && pXDev->key->xkbInfo ) {
	xkb = pXDev->key->xkbInfo;
	act.type = XkbSA_LatchGroup;
	act.group.flags = 0;
	XkbSASetGroup(&act.group,group);
	filter = _XkbNextFreeFilter();
	_XkbFilterLatchState(xkb,filter,SYNTHETIC_KEYCODE,&act);
	_XkbFilterLatchState(xkb,filter,SYNTHETIC_KEYCODE,NULL);
	return Success;
    }
    return BadValue;
}
