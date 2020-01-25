/* $XConsortium: xkb.c,v 1.9 94/04/08 15:13:15 erik Exp $ */
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

#include "X.h"
#define	NEED_EVENTS
#define	NEED_REPLIES
#include "Xproto.h"
#include "misc.h"
#include "inputstr.h"
#include "XKBsrv.h"
#include "extnsionst.h"

#include "XI.h"
#ifdef XINPUT
DeviceIntPtr LookupDeviceIntRec();
#endif

	int	XkbEventBase;
	int	XkbErrorBase;
	int	XkbReqCode;
	int	XkbKeyboardErrorCode;
Atom	xkbONE_LEVEL;
Atom	xkbTWO_LEVEL;
Atom	xkbKEYPAD;
CARD16	xkbDebugFlags = 0;

RESTYPE	RT_XKBCLIENT;

/***====================================================================***/

int
ProcXkbUseExtension(client)
    ClientPtr client;
{
    REQUEST(xkbUseExtensionReq);
    xkbUseExtensionReply	rep;
    register int n;
    int	supported;

    REQUEST_SIZE_MATCH(xkbUseExtensionReq);
    if (stuff->wantedMajor != XkbMajorVersion)
	supported = 0;
#if XkbMajorVersion==0
    else if (stuff->wantedMinor != XkbMinorVersion)
	supported = 0;
#endif
    else supported = 1;

#ifdef XKB_SWAPPING_BUSTED
    if (client->swapped)
	supported= 0;
#endif

    if ((supported) && (!(client->xkbClientFlags&XKB_INITIALIZED))) {
	client->xkbClientFlags= XKB_INITIALIZED;
	client->mapNotifyMask= XkbKeyTypesMask;
    }
#if XkbMajorVersion==0
    else if (xkbDebugFlags) {
	ErrorF("Rejecting client %d (0x%x) (wants %d.%02d, have %d.%02d)\n",
		client->index, client->clientAsMask,
		stuff->wantedMajor,stuff->wantedMinor,
		XkbMajorVersion,XkbMinorVersion);
    }
#endif
    rep.type = X_Reply;
    rep.supported = supported;
    rep.length = 0;
    rep.sequenceNumber = client->sequence;
    rep.serverMajor = XkbMajorVersion;
    rep.serverMinor = XkbMinorVersion;
    if ( client->swapped ) {
	swaps(&rep.sequenceNumber, n);
	swaps(&rep.serverMajor, n);
	swaps(&rep.serverMinor, n);
    }
    WriteToClient(client,SIZEOF(xkbUseExtensionReply), (char *)&rep);
    return client->noClientException;
}

/***====================================================================***/

int
ProcXkbSelectEvents(client)
    ClientPtr client;
{
    int status;
    DeviceIntPtr dev;
    XkbInterestRec *masks;
    REQUEST(xkbSelectEventsReq);

    REQUEST_AT_LEAST_SIZE(xkbSelectEventsReq);
    dev = XkbLookupDevice(stuff->deviceSpec);
    if (!dev) {
	client->errorValue = XkbError2(0xff,stuff->deviceSpec);
	return XkbKeyboardErrorCode;
    }
    if (((stuff->affectWhich&XkbMapNotifyMask)!=0)&&(stuff->affectMap)) {
	client->mapNotifyMask&= ~stuff->affectMap;
	client->mapNotifyMask|= (stuff->affectMap&stuff->map);
    }
    if (stuff->affectWhich&(~XkbMapNotifyMask)==0) 
	return client->noClientException;

    masks = XkbFindClientResource((DevicePtr)dev,client);
    if (!masks){
	XID id = FakeClientID(client->index);
	AddResource(id,RT_XKBCLIENT,dev);
	masks= XkbAddClientResource((DevicePtr)dev,client,id);
    }
    if (masks) {
	union {
	    BOOL	*b;
	    CARD8	*c8;
	    CARD16	*c16;
	    CARD32	*c32;
	} from,to;
	register unsigned bit,ndx,maskLeft,dataLeft,size;

	from.c8= (CARD8 *)&stuff[1];
	dataLeft= (stuff->length*4)-SIZEOF(xkbSelectEventsReq);
	maskLeft= (stuff->affectWhich&(~XkbMapNotifyMask));
	for (ndx=1,bit=2; (maskLeft!=0); ndx++, bit<<=1) {
	    if ((bit&maskLeft)==0)
		continue;
	    maskLeft&= ~bit;
	    switch (ndx) {
		case XkbStateNotify:
		    to.c16= &masks->stateNotifyMask;
		    size= 2;
		    break;
		case XkbControlsNotify:
		    to.c32= &masks->ctrlsNotifyMask;
		    size= 4;
		    break;
		case XkbIndicatorStateNotify:
		    to.c32= &masks->iStateNotifyMask;
		    size= 4;
		    break;
		case XkbIndicatorMapNotify:
		    to.c32= &masks->iMapNotifyMask;
		    size= 4;
		    break;
		case XkbNamesNotify:
		    to.c16= &masks->namesNotifyMask;
		    size= 2;
		    break;
		case XkbAlternateSymsNotify:
		    to.c16= &masks->altSymsNotifyMask;
		    size= 2;
		    break;
		case XkbBellNotify:
		    to.b= &masks->bellNotifyWanted;
		    size= 1;
		    break;
		case XkbActionMessage:
		    to.b= &masks->actionMessageWanted;
		    size= 1;
		    break;
		case XkbSlowKeyNotify:
		    to.c8= &masks->slowKeyNotifyMask;
		    size= 1;
		    break;
		case XkbCompatMapNotify:
		    to.c8= &masks->compatNotifyMask;
		    size= 1;
		    break;
		default:
		    client->errorValue = XkbError2(0x1,bit);
		    return BadValue;
	    }

	    if (stuff->clear&bit) {
		if (size==2)		to.c16[0]= 0;
		else if (size==4)	to.c32[0]= 0;
		else			to.c8[0]=  0;
	    }
	    else if (stuff->selectAll&bit) {
		if (size==2)		to.c16[0]= ~0;
		else if (size==4)	to.c32[0]= ~0;
		else			to.c8[0]=  ~0;
	    }
	    else {
		if (dataLeft<(size*2))
		    return BadLength;
		if (size==2) {
		    to.c16[0]&= ~from.c16[0];
		    to.c16[0]|= (from.c16[0]&from.c16[1]);
		}
		else if (size==4) {
		    to.c32[0]&= ~from.c32[0];
		    to.c32[0]|= (from.c32[0]&from.c32[1]);
		}
		else  {
		    to.c8[0]&= ~from.c8[0];
		    to.c8[0]|= (from.c8[0]&from.c8[1]);
		}
		from.c8+= (size*2);
		dataLeft-= (size*2);
	    }
	}
	if (dataLeft>2) {
	    ErrorF("Extra data (%d bytes) after SelectEvents\n",dataLeft);
	    return BadLength;
	}
	return client->noClientException;
    }
    return BadAlloc;
}

/***====================================================================***/

int
ProcXkbSendEvent(client)
    ClientPtr client;
{
    REQUEST(xkbSendEventReq);

    REQUEST_SIZE_MATCH(xkbSendEventReq);
    /* 8/4/93 (ef) -- XXX! Implement this */
    return BadImplementation;
}

/***====================================================================***/

int
ProcXkbBell(client)
    ClientPtr client;
{
    REQUEST(xkbBellReq);
    DeviceIntPtr keybd;
    WindowPtr	 pWin;
    int base;
    int newPercent;
    pointer ctrl;
    void (*proc)();

    REQUEST_SIZE_MATCH(xkbBellReq);
    keybd= XkbLookupDevice(stuff->deviceSpec);
    if (!keybd) {
	client->errorValue = XkbError2(0xff,stuff->deviceSpec);
	return XkbKeyboardErrorCode;
    }

    if (stuff->percent < -100 || stuff->percent > 100) {
	client->errorValue = XkbError2(0x1,stuff->percent);
	return BadValue;
    }
    if (stuff->bellClass == KbdFeedbackClass) {
	KbdFeedbackPtr	k;
	for (k=keybd->kbdfeed; k; k=k->next) {
	    if (k->ctrl.id == stuff->bellID)
		break;
	}
	if (!k) {
	    client->errorValue = XkbError2(0x2,stuff->bellID);
	    return BadValue;
	}
	base = k->ctrl.bell;
	proc = k->BellProc;
	ctrl = (pointer) &(k->ctrl);
    }
    else if (stuff->bellClass == BellFeedbackClass) {
	BellFeedbackPtr	b;
	for (b=keybd->bell; b; b=b->next) {
	    if (b->ctrl.id == stuff->bellID)
		break;
	}
	if (!b) {
	    client->errorValue = XkbError2(0x3,stuff->bellID);
	    return BadValue;
	}
	base = b->ctrl.percent;
	proc = b->BellProc;
	ctrl = (pointer) &(b->ctrl);
    }
    else {
	client->errorValue = XkbError2(0x4,stuff->bellClass);;
	return BadValue;
    }
    if (stuff->window!=None) {
	pWin= (WindowPtr)LookupIDByType(stuff->window,RT_WINDOW);
	if (pWin==NULL) {
	    client->errorValue= stuff->window;
	    return BadValue;
	}
    }
    else pWin= NULL;

    newPercent= (base*stuff->percent)/100;
    if (stuff->percent < 0)
         newPercent= base+newPercent;
    else newPercent= base-newPercent+stuff->percent;
    XkbHandleBell(stuff->override, keybd, newPercent, ctrl, stuff->bellClass, 
						stuff->name, pWin, client);
    return Success;
}

/***====================================================================***/

int
ProcXkbGetState(client)
    ClientPtr client;
{
    REQUEST(xkbGetStateReq);
    DeviceIntPtr	keybd;
    xkbGetStateReply	 rep;
    XkbStateRec		*xkb;
    int			n;

    REQUEST_SIZE_MATCH(xkbGetStateReq);
    keybd = XkbLookupDevice(stuff->deviceSpec);
    if ( !keybd ) {
	client->errorValue = XkbError2(0xff,stuff->deviceSpec);
	return XkbKeyboardErrorCode;
    }
    xkb= &keybd->key->xkbInfo->state;
    bzero(&rep,sizeof(xkbGetStateReply));
    rep.type= X_Reply;
    rep.sequenceNumber= client->sequence;
    rep.length = 0;
    rep.deviceID = keybd->id;
    rep.mods = keybd->key->state&0xff;
    rep.baseMods = xkb->base_mods;
    rep.lockedMods = xkb->locked_mods;
    rep.latchedMods = xkb->latched_mods;
    rep.group = xkb->group;
    rep.baseGroup = xkb->base_group;
    rep.latchedGroup = xkb->latched_group;
    rep.lockedGroup = xkb->locked_group;
    rep.compatState = xkb->compat_state;
    if (client->swapped) {
	register int n;
	swaps(&rep.sequenceNumber,n);
    }
    WriteToClient(client, SIZEOF(xkbGetStateReply), (char *)&rep);
    return client->noClientException;
}

/***====================================================================***/

int
ProcXkbLatchLockState(client)
    ClientPtr client;
{
    int status;
    DeviceIntPtr dev;
    XkbStateRec	oldState,*newState;
    CARD16 changed;
    REQUEST(xkbLatchLockStateReq);

    REQUEST_SIZE_MATCH(xkbLatchLockStateReq);
    dev = XkbLookupDevice(stuff->deviceSpec);
    if (!dev) {
	client->errorValue = XkbError2(0xff,stuff->deviceSpec);
	return XkbKeyboardErrorCode;
    }
    status = Success;
    oldState= dev->key->xkbInfo->state;
    newState= &dev->key->xkbInfo->state;
    if ( stuff->affectModLocks ) {
	newState->locked_mods&= ~stuff->affectModLocks;
	newState->locked_mods|= (stuff->affectModLocks&stuff->modLocks);
    }
    if (( status == Success ) && stuff->lockGroup )
	newState->locked_group = stuff->groupLock;
    if (( status == Success ) && stuff->affectModLatches )
	status=XkbLatchModifiers(dev,stuff->affectModLatches,stuff->modLatches);
    if (( status == Success ) && stuff->latchGroup )
	status=XkbLatchGroup(dev,stuff->groupLatch);

    if ( status != Success )
	return status;

    XkbComputeDerivedState(dev->key->xkbInfo);
    dev->key->state= (dev->key->xkbInfo->lookupState&0xE0FF);

    changed = XkbStateChangedFlags(&oldState,&dev->key->xkbInfo->state);
    if (changed) {
	xkbStateNotify	sn;

	sn.keycode= 0;
	sn.eventType= 0;
	sn.requestMajor = XkbReqCode;
	sn.requestMinor = X_kbLatchLockState;
	sn.changed= changed;
	XkbSendStateNotify(dev,&sn);
	changed= XkbIndicatorsToUpdate(dev,changed);
        XkbUpdateIndicators(dev,changed,NULL);
    }
    return client->noClientException;
}

/***====================================================================***/

int
ProcXkbGetControls(client)
    ClientPtr client;
{
    int	status;
    REQUEST(xkbGetControlsReq);
    xkbGetControlsReply rep;
    XkbControlsRec	*xkb;
    DeviceIntPtr dev;
    register int n;

    REQUEST_SIZE_MATCH(xkbGetControlsReq);
    dev = XkbLookupDevice(stuff->deviceSpec);
    if (!dev) {
	client->errorValue = XkbError2(0xff,stuff->deviceSpec);
	return XkbKeyboardErrorCode;
    }
    xkb = dev->key->xkbInfo->desc.ctrls;
    rep.type = X_Reply;
    rep.length = (SIZEOF(xkbGetControlsReply)-
		  SIZEOF(xGenericReply)) >> 2;
    rep.sequenceNumber = client->sequence;
    rep.deviceID = ((DeviceIntPtr)dev)->id;
    rep.numGroups = xkb->num_groups;
    rep.internalMods = xkb->internal_mask;
    rep.ignoreLockMods = xkb->ignore_lock_mask;
    rep.internalRealMods = xkb->internal_real_mods;
    rep.ignoreLockRealMods = xkb->ignore_lock_real_mods;
    rep.internalVirtualMods = xkb->internal_vmods;
    rep.ignoreLockVirtualMods = xkb->ignore_lock_vmods;
    rep.enabledControls = xkb->enabled_ctrls;
    rep.repeatDelay = xkb->repeat_delay;
    rep.repeatInterval = xkb->repeat_interval;
    rep.slowKeysDelay = xkb->slow_keys_delay;
    rep.debounceDelay = xkb->debounce_delay;
    rep.mouseKeysDelay = xkb->mouse_keys_delay;
    rep.mouseKeysInterval = xkb->mouse_keys_interval;
    rep.mouseKeysTimeToMax = xkb->mouse_keys_time_to_max;
    rep.mouseKeysMaxSpeed = xkb->mouse_keys_max_speed;
    rep.mouseKeysCurve = xkb->mouse_keys_curve;
    rep.mouseKeysDfltBtn = xkb->mouse_keys_dflt_btn;
    rep.accessXTimeout = xkb->accessx_timeout;
    rep.accessXTimeoutMask = xkb->accessx_timeout_mask;
    if (client->swapped) {
    	swaps(&rep.sequenceNumber, n);
	swapl(&rep.length,n);
	swaps(&rep.internalVirtualMods, n);
	swaps(&rep.ignoreLockVirtualMods, n);
	swapl(&rep.enabledControls, n);
	swaps(&rep.repeatDelay, n);
	swaps(&rep.repeatInterval, n);
	swaps(&rep.slowKeysDelay, n);
	swaps(&rep.debounceDelay, n);
	swaps(&rep.mouseKeysDelay, n);
	swaps(&rep.mouseKeysInterval, n);
	swaps(&rep.mouseKeysTimeToMax, n);
	swaps(&rep.mouseKeysMaxSpeed, n);
	swaps(&rep.mouseKeysCurve, n);
	swaps(&rep.accessXTimeout, n);
	swapl(&rep.accessXTimeoutMask, n);
    }
    WriteToClient(client, SIZEOF(xkbGetControlsReply), (char *)&rep);
    return(client->noClientException);
}

int
ProcXkbSetControls(client)
    ClientPtr client;
{
    int 		status;
    DeviceIntPtr 	dev;
    XkbSrvInfoPtr	xkb;
    XkbControlsPtr	ctrl;
    XkbControlsRec	old;
    xkbControlsNotify	cn;
    REQUEST(xkbSetControlsReq);

    REQUEST_SIZE_MATCH(xkbSetControlsReq);
    dev = XkbLookupDevice(stuff->deviceSpec);
    if (!dev) {
	client->errorValue = XkbError2(0xff,stuff->deviceSpec);
	return XkbKeyboardErrorCode;
    }
    xkb = dev->key->xkbInfo;
    ctrl = xkb->desc.ctrls;
    old = *ctrl;
    ctrl->internal_real_mods&=~stuff->affectInternalRealMods;
    ctrl->internal_real_mods|=
	      (stuff->affectInternalRealMods&stuff->internalRealMods);
    ctrl->ignore_lock_real_mods&=~stuff->affectIgnoreLockRealMods;
    ctrl->ignore_lock_real_mods|=
	      (stuff->affectIgnoreLockRealMods&stuff->ignoreLockRealMods);
    ctrl->internal_vmods&=~stuff->affectInternalVirtualMods;
    ctrl->internal_vmods|=
	      (stuff->affectInternalVirtualMods&stuff->internalVirtualMods);
    ctrl->ignore_lock_vmods&=~stuff->affectIgnoreLockVirtualMods;
    ctrl->ignore_lock_vmods|=
	      (stuff->affectIgnoreLockVirtualMods&stuff->ignoreLockVirtualMods);
    ctrl->internal_mask= ctrl->internal_real_mods|
	      XkbMaskForVMask(&xkb->desc,ctrl->internal_vmods);
    ctrl->ignore_lock_mask= ctrl->ignore_lock_real_mods|
	      XkbMaskForVMask(&xkb->desc,ctrl->ignore_lock_vmods);
    ctrl->enabled_ctrls&= ~stuff->affectEnabledControls;
    ctrl->enabled_ctrls|=
	      (stuff->affectEnabledControls&stuff->enabledControls);
    if (stuff->changeControls&XkbRepeatKeysMask) {
	ctrl->repeat_delay = stuff->repeatDelay;
	ctrl->repeat_interval = stuff->repeatInterval;
    }
    if (stuff->changeControls&XkbSlowKeysMask)
	ctrl->slow_keys_delay = stuff->slowKeysDelay;
    if (stuff->changeControls&XkbBounceKeysMask)
	ctrl->debounce_delay = stuff->debounceDelay;
    if (stuff->changeControls&XkbMouseKeysMask) {
	ctrl->mouse_keys_delay = stuff->mouseKeysDelay;
	ctrl->mouse_keys_interval = stuff->mouseKeysInterval;
	ctrl->mouse_keys_dflt_btn = stuff->mouseKeysDfltBtn;
    }
    if (stuff->changeControls&XkbMouseKeysAccelMask) {
	ctrl->mouse_keys_time_to_max = stuff->mouseKeysTimeToMax;
	ctrl->mouse_keys_max_speed = stuff->mouseKeysMaxSpeed;
	ctrl->mouse_keys_curve = stuff->mouseKeysCurve;
	xkb->mouseKeysCurve= 1.5+(((double)stuff->mouseKeysCurve)*0.0015);
	xkb->mouseKeysCurveFactor= ((double)stuff->mouseKeysMaxSpeed)/
				    pow((double)stuff->mouseKeysTimeToMax,
					dev->key->xkbInfo->mouseKeysCurve);
    }
    if (stuff->changeControls&XkbAccessXTimeoutMask) {
	ctrl->accessx_timeout = stuff->accessXTimeout;
	ctrl->accessx_timeout_mask = stuff->accessXTimeoutMask;
    }
    DDXChangeXkbControls(dev,&old,ctrl);
    if (XkbComputeControlsNotify(dev,&old,ctrl,&cn)) {
	cn.keycode= 0;
	cn.eventType = 0;
	cn.requestMajor = XkbReqCode;
	cn.requestMinor = X_kbSetControls;
	XkbSendControlsNotify(dev,&cn);
    }
    return client->noClientException;
}

int
XkbSetRepeatRate(dev,timeout,interval,major,minor)
    DeviceIntPtr	dev;
    int			timeout;
    int			interval;
    int			major;
    int			minor;
{
int	changed= 0;
XkbControlsRec old,*xkb;

    if ((!dev)||(!dev->key)||(!dev->key->xkbInfo))
	return 0;
    xkb= dev->key->xkbInfo->desc.ctrls;
    old= *xkb;
    if ((timeout!=0) && (xkb->repeat_delay!=timeout)) {
	xkb->repeat_delay= timeout;
	changed++;
    }
    if ((interval!=0) && (xkb->repeat_interval!=interval)) {
	xkb->repeat_interval= interval;
	changed++;
    }
    if (changed) {
	xkbControlsNotify	cn;
	DDXChangeXkbControls(dev,&old,xkb);
	if (XkbComputeControlsNotify(dev,&old,xkb,&cn)) {
	    cn.keycode= 0;
	    cn.eventType = 0;
	    cn.requestMajor = major;
	    cn.requestMinor = minor;
	    XkbSendControlsNotify(dev,&cn);
	}
    }
    return 1;
}

int
XkbGetRepeatRate(dev,timeout,interval)
    DeviceIntPtr	dev;
    int	*		timeout;
    int	*		interval;
{
XkbControlsRec *xkb;

    if ((!dev)||(!dev->key)||(!dev->key->xkbInfo))
	return 0;
    xkb= dev->key->xkbInfo->desc.ctrls;
    if (timeout)	*timeout= xkb->repeat_delay;
    if (interval)	*interval= xkb->repeat_interval;
    return 1;
}

/***====================================================================***/

static int
XkbSizeKeyTypes(xkb,firstType,nTypes)
    XkbDescRec *xkb;
    CARD8 firstType;
    CARD8 nTypes;
{
    XkbKeyTypeRec 	*type;
    unsigned		i,len;

    len= 0;
    type= &xkb->map->types[firstType];
    for (i=0;i<nTypes;i++,type++){
	len+= SIZEOF(xkbKeyTypeWireDesc);
	if (type->map_count>0) {
	    len+= (type->map_count*SIZEOF(xkbKTMapEntryWireDesc));
	    if (type->preserve)
		len+= (type->map_count*SIZEOF(xkbKTPreserveWireDesc));
	}
    }
    return len;
}

static char *
XkbWriteKeyTypes(xkb,rep,buf,client,wantVMap)
    XkbDescRec *xkb;
    xkbGetMapReply *rep;
    char *buf;
    ClientPtr client;
    int	wantVMap;
{
    char		*start;
    XkbKeyTypeRec 	*type;
    unsigned		i;
    xkbKeyTypeWireDesc	*wire;

    start= buf;
    type= &xkb->map->types[rep->firstType];
    for (i=0;i<rep->nTypes;i++,type++) {
	register unsigned n,nOut;
	wire= (xkbKeyTypeWireDesc *)buf;
	wire->mask = type->mask;
	wire->realMods = type->real_mods;
	wire->virtualMods = type->vmods;
	wire->groupWidth = type->group_width;
	wire->nMapEntries = type->map_count;
	wire->preserve = (type->preserve!=NULL);
	if (client->swapped) {
	    register int n;
	    swaps(&wire->virtualMods,n);
	}	

	buf= (char *)&wire[1];
	if (wire->nMapEntries>0) {
	    xkbKTMapEntryWireDesc *	wire;
	    XkbKTMapEntryPtr		entry;
	    wire= (xkbKTMapEntryWireDesc *)buf;
	    entry= type->map;
	    for (n=0;n<type->map_count;n++,wire++,entry++) {
		wire->active= entry->active;
		wire->mask= entry->mask;
		wire->level= entry->level;
		wire->realMods= entry->real_mods;
		wire->virtualMods= entry->vmods;
		if (client->swapped) {
		    register int n;
		    swaps(&wire->virtualMods,n);
		}
	    }
	    buf= (char *)wire;
	    if (type->preserve!=NULL) {
		xkbKTPreserveWireDesc *	pwire;
		XkbKTPreservePtr	preserve;
		pwire= (xkbKTPreserveWireDesc *)buf;
		preserve= type->preserve;
		for (n=0;n<type->map_count;n++,pwire++,preserve++) {
		    pwire->mask= preserve->mask;
		    pwire->realMods= preserve->real_mods;
		    pwire->virtualMods= preserve->vmods;
		    if (client->swapped) {
			register int n;
			swaps(&pwire->virtualMods,n);
		    }
		}
		buf= (char *)pwire;
	    }
	}
    }
    return buf;
}

static int
XkbSizeKeySyms(xkb,firstKey,nKeys,nSymsRtrn)
    XkbDescRec *xkb;
    CARD8 firstKey;
    CARD8 nKeys;
    CARD16 *nSymsRtrn;
{
    XkbSymMapRec	*symMap;
    XkbKeyTypeRec 	*type;
    unsigned		i,len;
    unsigned		nSyms,nSymsThisKey;

    len= nKeys*SIZEOF(xkbSymMapWireDesc);
    symMap = &xkb->map->key_sym_map[firstKey];
    for (i=nSyms=0;i<nKeys;i++,symMap++) {
	type = &xkb->map->types[symMap->kt_index];
	nSymsThisKey= XkbNumGroups(symMap->group_info)*type->group_width;
	if (symMap->offset!=0)
	    nSyms+= nSymsThisKey;
    }
    len+= nSyms*4;
    *nSymsRtrn = nSyms;
    return len;
}

static int
XkbSizeVirtualMods(xkb,virtualMods)
    XkbDescRec *xkb;
    CARD16	virtualMods;
{
register unsigned i,nMods,bit;

    for (i=nMods=0,bit=1;i<XkbNumVirtualMods;i++,bit<<=1) {
        if (virtualMods&bit)
	    nMods++;
    }
    return XkbPaddedSize(nMods);
}

static char *
XkbWriteKeySyms(xkb,rep,buf,client)
    XkbDescRec *xkb;
    xkbGetMapReply *rep;
    char *buf;
    ClientPtr client;
{
register KeySym *	pSym;
XkbSymMapPtr		symMap;
XkbKeyTypePtr		type;
xkbSymMapWireDesc *	outMap;
register unsigned	i;

    symMap = &xkb->map->key_sym_map[rep->firstKeySym];
    for (i=0;i<rep->nKeySyms;i++,symMap++) {
	type = &xkb->map->types[symMap->kt_index];
	outMap = (xkbSymMapWireDesc *)buf;
	outMap->ktIndex = symMap->kt_index;
	outMap->groupInfo = symMap->group_info;
	outMap->nSyms = XkbNumGroups(symMap->group_info)*type->group_width;
	buf= (char *)&outMap[1];
	if (outMap->nSyms==0)
	    continue;

	pSym = &xkb->map->syms[symMap->offset];
	memcpy((char *)buf,(char *)pSym,outMap->nSyms*4);
	if (client->swapped) {
	    register int n,nSyms= outMap->nSyms;
	    swaps(&outMap->nSyms,n);
	    while (nSyms-->0) {
		swapl(buf,n);
		buf+= 4;
	    }
	}
	else buf+= outMap->nSyms*4;
    }
    return buf;
}

static int
XkbSizeKeyActions(xkb,firstKey,nKeys,nActsRtrn)
    XkbDescPtr	xkb;
    CARD8	firstKey;
    CARD8	nKeys;
    CARD16	*nActsRtrn;
{
    unsigned	i,len,nActs;

    for (nActs=i=0;i<nKeys;i++) {
	if (xkb->server->key_acts[i+firstKey]!=0)
	    nActs+= XkbKeyNumActions(xkb,i+firstKey);
    }
    len= XkbPaddedSize(nKeys)+(nActs*SIZEOF(xkbActionWireDesc));
    *nActsRtrn = nActs;
    return len;
}

static char *
XkbWriteKeyActions(xkb,rep,buf,client)
    XkbDescPtr		xkb;
    xkbGetMapReply *	rep;
    char *		buf;
    ClientPtr		client;
{
    unsigned		i,ndx;
    CARD8 *		numDesc;
    XkbAnyAction *	actDesc;

    numDesc = (CARD8 *)buf;
    for (i=0;i<rep->nKeyActions;i++) {
	if (xkb->server->key_acts[i+rep->firstKeyAction]==0)
	     numDesc[i] = 0;
	else numDesc[i] = XkbKeyNumActions(xkb,(i+rep->firstKeyAction));
    }
    buf+= XkbPaddedSize(rep->nKeyActions);

    actDesc = (XkbAnyAction *)buf;
    for (i=0;i<rep->nKeyActions;i++) {
	if (xkb->server->key_acts[i+rep->firstKeyAction]!=0) {
	    unsigned int num;
	    num = XkbKeyNumActions(xkb,(i+rep->firstKeyAction));
	    memcpy((char *)actDesc,
		   (char*)XkbKeyActionsPtr(xkb,(i+rep->firstKeyAction)),
		   num*SIZEOF(xkbActionWireDesc));
	    actDesc+= num;
	}
    }
    buf = (char *)actDesc;
    return buf;
}

static int
XkbSizeKeyBehaviors(xkb,firstKey,nKeys,nKeysRtrn)
    XkbDescRec	*xkb;
    CARD8	 firstKey;
    CARD8	 nKeys;
    CARD8	*nKeysRtrn;
{
    unsigned		i,len,nBhvr;

    for (nBhvr=i=0;i<nKeys;i++) {
	if (xkb->server->behaviors[i+firstKey].type!=XkbKB_Default)
	    nBhvr++;
    }
    len= nBhvr*SIZEOF(xkbBehaviorWireDesc);
    *nKeysRtrn= nBhvr;
    return len;
}

static char *
XkbWriteKeyBehaviors(xkb,req,rep,buf,client)
    XkbDescRec		*xkb;
    xkbGetMapReq	*req;
    xkbGetMapReply	*rep;
    char		*buf;
    ClientPtr		client;
{
    unsigned		i,ndx;
    xkbBehaviorWireDesc	*wire;
    XkbBehavior		*pBhvr;

    wire = (xkbBehaviorWireDesc *)buf;
    pBhvr= &xkb->server->behaviors[req->firstKeyBehavior];
    for (i=0;i<req->nKeyBehaviors;i++,pBhvr++) {
	if (pBhvr->type!=XkbKB_Default) {
	    wire->key=  i+req->firstKeyBehavior;
	    wire->type= pBhvr->type;
	    wire->data= pBhvr->data;
	    wire++;
	}
    }
    buf = (char *)wire;
    return buf;
}

static int
XkbSizeExplicit(xkb,firstKey,nKeys,nKeysRtrn)
    XkbDescRec	*xkb;
    CARD8	 firstKey;
    CARD8	 nKeys;
    CARD8	*nKeysRtrn;
{
    unsigned	i,len,nRtrn;

    for (nRtrn=i=0;i<nKeys;i++) {
	if (xkb->server->explicit[i+firstKey]!=0)
	    nRtrn++;
    }
    *nKeysRtrn= nRtrn;
    len= XkbPaddedSize(nRtrn*2); /* two bytes per non-zero explicit component */
    return len;
}

static char *
XkbWriteExplicit(xkb,req,rep,buf,client)
    XkbDescPtr		 xkb;
    xkbGetMapReq	*req;
    xkbGetMapReply	*rep;
    char		*buf;
    ClientPtr		client;
{
unsigned	i;
char *		start;
unsigned char *	pExp;

    start= buf;
    pExp= &xkb->server->explicit[req->firstKeyExplicit];
    for (i=0;i<req->nKeyExplicit;i++,pExp++) {
	if (*pExp!=0) {
	    *buf++= i+req->firstKeyExplicit;
	    *buf++= *pExp;
	}
    }
    i= XkbPaddedSize(buf-start)-(buf-start); /* pad to word boundary */
    return buf+i;
}

static int
XkbSendMap(client,keybd,req,rep)
    ClientPtr		client;
    DeviceIntPtr	keybd;
    xkbGetMapReq	*req;
    xkbGetMapReply	*rep;
{
XkbDescRec	*xkb;
unsigned	i,len;
char		*desc,*start;

    xkb= &keybd->key->xkbInfo->desc;
    len= XkbSizeKeyTypes(xkb,rep->firstType,rep->nTypes)/4;
    len+= XkbSizeKeySyms(xkb,rep->firstKeySym,rep->nKeySyms,
						    &rep->totalSyms)/4;
    len+= XkbSizeKeyActions(xkb,rep->firstKeyAction,rep->nKeyActions,
						    &rep->totalActions)/4;
    len+= XkbSizeKeyBehaviors(xkb,req->firstKeyBehavior,req->nKeyBehaviors,
						    &rep->totalKeyBehaviors)/4;
    len+= XkbSizeVirtualMods(xkb,rep->virtualMods)/4;
    len+= XkbSizeExplicit(xkb,req->firstKeyExplicit,req->nKeyExplicit,
						    &rep->totalKeyExplicit)/4;
    start= desc= (char *)ALLOCATE_LOCAL(len*4);
    if (!start)
	return BadAlloc;
    if ( rep->nTypes>0 )
	desc = XkbWriteKeyTypes(xkb,rep,desc,client);
    if ( rep->nKeySyms>0 )
	desc = XkbWriteKeySyms(xkb,rep,desc,client);
    if ( rep->nKeyActions>0 )
	desc = XkbWriteKeyActions(xkb,rep,desc,client);
    if ( rep->totalKeyBehaviors>0 )
	desc = XkbWriteKeyBehaviors(xkb,req,rep,desc,client);
    if ( rep->virtualMods ) {
	register int sz,bit;
	for (i=sz=0,bit=1;i<16;i++,bit<<=1) {
	    if (rep->virtualMods&bit) {
		desc[sz++]= xkb->server->vmods[i];
	    }
	}
	desc+= XkbPaddedSize(sz);
    }
    if ( rep->totalKeyExplicit>0 )
	desc= XkbWriteExplicit(xkb,req,rep,desc,client);
    if ((desc-start)!=(len*4)) {
	ErrorF("BOGUS LENGTH in write keyboard desc, expected %d, got %d\n",
					len*4, desc-start);
    }
    rep->length= len;
    if (client->swapped) {
	register int n;
	swaps(&rep->sequenceNumber,n);
	swapl(&rep->length,n);
	swaps(&rep->present,n);
	swaps(&rep->totalSyms,n);
	swaps(&rep->totalActions,n);
    }
    WriteToClient(client, SIZEOF(xkbGetMapReply), (char *)rep);
    WriteToClient(client, len*4, start);
    DEALLOCATE_LOCAL((char *)start);
    return client->noClientException;
}

int
ProcXkbGetMap(client)
    ClientPtr client;
{
    REQUEST(xkbGetMapReq);
    DeviceIntPtr	 keybd;
    xkbGetMapReply	 rep;
    XkbDescRec		*xkb;
    int			 n;

    REQUEST_SIZE_MATCH(xkbGetMapReq);
    keybd = XkbLookupDevice(stuff->deviceSpec);
    if (!keybd) {
	client->errorValue = XkbError2(0xff,stuff->deviceSpec);
	return XkbKeyboardErrorCode;
    }
    xkb= &keybd->key->xkbInfo->desc;
    bzero(&rep,sizeof(xkbGetMapReply));
    rep.type= X_Reply;
    rep.sequenceNumber= client->sequence;
    rep.length = 0;
    rep.deviceID = keybd->id;
    rep.present = stuff->partial|stuff->full;
    rep.minKeyCode = xkb->min_key_code;
    rep.maxKeyCode = xkb->max_key_code;
    if ( stuff->full&XkbKeyTypesMask ) {
	rep.firstType = 0;
	rep.nTypes = xkb->map->num_types;
    }
    else if (stuff->partial&XkbKeyTypesMask) {
	if (((unsigned)stuff->firstType+stuff->nTypes)>xkb->map->num_types) {
	    client->errorValue = XkbError4(0x01,xkb->map->num_types,
					stuff->firstType,stuff->nTypes);
	    return BadValue;
	}
	rep.firstType = stuff->firstType;
	rep.nTypes = stuff->nTypes;
    }
    else rep.nTypes = 0;
    rep.totalTypes = xkb->map->num_types;

    if ( stuff->full&XkbKeySymsMask ) {
	rep.firstKeySym = xkb->min_key_code;
	rep.nKeySyms = XkbNumKeys(xkb);
    }
    else if (stuff->partial&XkbKeySymsMask) {
	if ((unsigned)(stuff->firstKeySym+stuff->nKeySyms-1)>xkb->max_key_code){
	    client->errorValue = XkbError4(0x02,stuff->firstKeySym,
					stuff->nKeySyms,xkb->max_key_code);
	    return BadValue;
	}
	else if (stuff->firstKeySym<(unsigned)xkb->min_key_code) {
	    client->errorValue = XkbError3(0x03,stuff->firstKeySym,
							xkb->min_key_code);
	    return BadValue;
	}
	rep.firstKeySym = stuff->firstKeySym;
	rep.nKeySyms = stuff->nKeySyms;
    }
    else rep.nKeySyms = 0;

    if ( stuff->full&XkbKeyActionsMask ) {
	rep.firstKeyAction = xkb->min_key_code;
	rep.nKeyActions = XkbNumKeys(xkb);
    }
    else if (stuff->partial&XkbKeyActionsMask) {
	if (((unsigned)stuff->firstKeyAction+stuff->nKeyActions-1)>
							xkb->max_key_code) {
	    client->errorValue = XkbError4(0x04,stuff->firstKeyAction,
					stuff->nKeyActions,xkb->max_key_code);
	    return BadValue;
	}
	else if ( stuff->firstKeyAction<xkb->min_key_code ) {
	    client->errorValue = XkbError3(0x05,stuff->firstKeyAction,
							xkb->min_key_code);
	    return BadValue;
	}
	rep.firstKeyAction = stuff->firstKeyAction;
	rep.nKeyActions = stuff->nKeyActions;
    }
    else rep.nKeyActions = 0;

    n= XkbNumKeys(xkb);
    if ( stuff->full&XkbKeyBehaviorsMask ) {
	stuff->firstKeyBehavior = xkb->min_key_code;
	stuff->nKeyBehaviors = n;
    }
    else if (stuff->partial&XkbKeyBehaviorsMask) {
	if (((unsigned)stuff->firstKeyBehavior<xkb->min_key_code)||
	      (((unsigned)stuff->firstKeyBehavior+stuff->nKeyBehaviors-1)>
							xkb->max_key_code)){
	    client->errorValue = XkbError4(0x03,xkb->max_key_code,
						stuff->firstKeyBehavior,
						stuff->nKeyBehaviors);
	    return BadValue;
	}
    }
    else stuff->nKeyBehaviors = 0;

    if (stuff->full&XkbVirtualModsMask)
	rep.virtualMods= ~0;
    else if (stuff->partial&XkbVirtualModsMask)
	rep.virtualMods= stuff->virtualMods;
    
    if (stuff->full&XkbExplicitComponentsMask) {
	stuff->firstKeyExplicit= xkb->min_key_code;
	stuff->nKeyExplicit= n;
    }
    else if (stuff->partial&XkbExplicitComponentsMask) {
	if ((stuff->firstKeyExplicit<(unsigned)xkb->min_key_code)||
	    (((unsigned)stuff->firstKeyExplicit+stuff->nKeyExplicit-1)>
							xkb->max_key_code)){
	    client->errorValue = XkbError4(0x04,xkb->max_key_code,
						stuff->firstKeyExplicit,
						stuff->nKeyExplicit);
	    return BadValue;
	}
    }
    else stuff->nKeyExplicit = 0;
    return XkbSendMap(client,keybd,stuff,&rep);
}

/***====================================================================***/

static int
CheckKeyTypes(client,xkb,req,wireRtrn,nMapsRtrn,mapWidthRtrn)
    ClientPtr		 client;
    XkbDescPtr	 	 xkb;
    xkbSetMapReq  *	 req;
    xkbKeyTypeWireDesc **wireRtrn;
    int	 *		 nMapsRtrn;
    CARD8 *		 mapWidthRtrn;
{
unsigned		nMaps;
register unsigned	i,n;
register CARD8 *	map;
register xkbKeyTypeWireDesc	*wire = *wireRtrn;

    if (req->firstType>((unsigned)xkb->map->num_types+1)) {
	*nMapsRtrn = XkbError3(0x01,req->firstType,xkb->map->num_types);
	return 0;
    }
    if (req->resize&XkbKeyTypesMask) {
	nMaps = req->firstType+req->nTypes;
	if (nMaps<3) {  /* three canonical key types must be there */
	    *nMapsRtrn= XkbError4(0x02,req->firstType,req->nTypes,3);
	    return 0;
	}
    }
    else if (req->present&XkbKeyTypesMask) {
	nMaps = xkb->map->num_types;
	if ((req->firstType+req->nTypes)>nMaps) {
	    *nMapsRtrn = req->firstType+req->nTypes;
	    return 0;
	}
    }
    else {
	*nMapsRtrn = xkb->map->num_types;
	for (i=0;i<xkb->map->num_types;i++) {
	    mapWidthRtrn[i] = xkb->map->types[i].group_width;
	}
	return 1;
    }

    for (i=0;i<req->firstType;i++) {
	mapWidthRtrn[i] = xkb->map->types[i].group_width;
    }
    map= (CARD8 *)&wire[1];
    for (i=0;i<req->nTypes;i++) {
	unsigned	width;
	n= i+req->firstType;
	width= wire->groupWidth;
	if (width<1) {
	    *nMapsRtrn= XkbError3(0x04,n,width);
	    return 0;
	}
	else if ((n==XkbOneLevelIndex)&&(width!=1)) { /* must be width 1 */
	    *nMapsRtrn= XkbError3(0x05,n,width);
	    return 0;
	}
	else if ((width!=2)&&((n==XkbTwoLevelIndex)||(n==XkbKeypadIndex))) {
	    /* TWO_LEVEL and KEYPAD must be width 2 */
	    *nMapsRtrn= XkbError3(0x05,n,width);
	    return 0;
	}
	if (wire->nMapEntries>0) {
	    xkbKTSetMapEntryWireDesc *	mapWire;
	    xkbKTPreserveWireDesc *	preWire;
	    mapWire= (xkbKTSetMapEntryWireDesc *)map;
	    preWire= (xkbKTPreserveWireDesc *)&mapWire[wire->nMapEntries];
	    for (n=0;n<wire->nMapEntries;n++) {
		if (mapWire[n].realMods&(~wire->realMods)) {
		    *nMapsRtrn= XkbError4(0x06,n,mapWire[n].realMods,
						 wire->realMods);
		    return 0;
		}
		if (mapWire[n].virtualMods&(~wire->virtualMods)) {
		    *nMapsRtrn= XkbError3(0x07,n,mapWire[n].virtualMods);
		    return 0;
		}
		if (mapWire[n].level>=wire->groupWidth) {
		    *nMapsRtrn= XkbError4(0x08,n,wire->groupWidth,
						 mapWire[n].level);
		    return 0;
		}
		if (wire->preserve) {
		    if (preWire[n].realMods&(~mapWire[n].realMods)) {
			*nMapsRtrn= XkbError4(0x09,n,preWire[n].realMods,
							mapWire[n].realMods);
			return 0;
		    }
		    if (preWire[n].virtualMods&(~mapWire[n].virtualMods)) {
			*nMapsRtrn= XkbError3(0x0a,n,preWire[n].virtualMods);
			return 0;
		    }
		}
	    }
	    if (wire->preserve)
		 map= (CARD8 *)&preWire[wire->nMapEntries];
	    else map= (CARD8 *)&mapWire[wire->nMapEntries];
	}
	mapWidthRtrn[i+req->firstType] = wire->groupWidth;
	wire= (xkbKeyTypeWireDesc *)map;
    }
    for (i=req->firstType+req->nTypes;i<nMaps;i++) {
	mapWidthRtrn[i] = xkb->map->types[i].group_width;
    }
    *nMapsRtrn = nMaps;
    *wireRtrn = wire;
    return 1;
}

static int
CheckKeySyms(client,xkb,req,nTypes,mapWidths,symsPerKey,wireRtrn,errorRtrn)
    ClientPtr		client;
    XkbDescPtr		xkb;
    xkbSetMapReq *	req;
    int			nTypes;
    CARD8 *	 	mapWidths;
    CARD16 *	 	symsPerKey;
    xkbSymMapWireDesc **wireRtrn;
    int *		errorRtrn;
{
register unsigned	i;
XkbSymMapPtr		map;
xkbSymMapWireDesc*	wire = *wireRtrn;

    if (!(XkbKeySymsMask&req->present))
	return 1;
    if (req->resize&XkbKeySymsMask) {
        *errorRtrn = XkbError2(0x10,req->resize);
        return 0;
    }
    if (req->firstKeySym<xkb->min_key_code) {
	*errorRtrn = XkbError3(0x11,req->firstKeySym,xkb->min_key_code);
	return 0;
    }
    if (((unsigned)(req->firstKeySym+req->nKeySyms-1))>xkb->max_key_code) {
	*errorRtrn = XkbError4(0x12,req->firstKeySym,req->nKeySyms,
							xkb->max_key_code+1);
	return 0;
    }
    map = &xkb->map->key_sym_map[xkb->min_key_code];
    for (i=xkb->min_key_code;i<(unsigned)req->firstKeySym;i++,map++) {
	if (map->kt_index>=(unsigned)nTypes) {
	    *errorRtrn = XkbError3(0x13,i,map->kt_index);
	    return 0;
	}
	symsPerKey[i] = XkbKeyNumSyms(xkb,i);
    }
    for (i=0;i<req->nKeySyms;i++) {
	KeySym *pSyms;
	register unsigned nG;
	if (client->swapped) {
	    swaps(&wire->nSyms,nG);
	}
	if (wire->ktIndex>=(unsigned)nTypes) {
	    *errorRtrn= XkbError3(0x14,i+req->firstKeySym,wire->ktIndex);
	    return 0;
	}
	nG = XkbNumGroups(wire->groupInfo);
	if ((nG<1)||(nG>8)) {
	    *errorRtrn = XkbError3(0x15,i,nG);
	    return 0;
	}
	symsPerKey[i+req->firstKeySym] = mapWidths[wire->ktIndex]*nG;
	if (symsPerKey[i+req->firstKeySym]!=wire->nSyms) {
	    *errorRtrn = XkbError4(0x16,i+req->firstKeySym,wire->nSyms,
					symsPerKey[i+req->firstKeySym]);
	    return 0;
	}
	pSyms = (KeySym *)&wire[1];
	wire = (xkbSymMapWireDesc *)&pSyms[wire->nSyms];
    }

    map = &xkb->map->key_sym_map[i];
    for (;i<=(unsigned)xkb->max_key_code;i++,map++) {
	if (map->kt_index>=(unsigned)nTypes) {
	    *errorRtrn = XkbError3(0x15,i,map->kt_index);
	    return 0;
	}
	symsPerKey[i] = XkbKeyNumSyms(xkb,i);
    }
    *wireRtrn = wire;
    return 1;
}

static int
CheckKeyActions(xkb,req,nTypes,mapWidths,symsPerKey,wireRtrn,nActsRtrn)
    XkbDescRec		 *xkb;
    xkbSetMapReq	 *req;
    int			  nTypes;
    CARD8		 *mapWidths;
    CARD16		 *symsPerKey;
    CARD8		**wireRtrn;
    int			 *nActsRtrn;
{
int			 nActs;
CARD8 *			 wire = *wireRtrn;
register unsigned	 i;

    if (!(XkbKeyActionsMask&req->present))
	return 1;
    if (req->resize&XkbKeyActionsMask) {
        *nActsRtrn = XkbError2(0x20,req->resize);
        return 0;
    }
    if (req->firstKeyAction<xkb->min_key_code) {
	*nActsRtrn = XkbError3(0x21,req->firstKeyAction,xkb->min_key_code);
	return 0;
    }
    if ((unsigned)(req->firstKeyAction+req->nKeyActions-1)>xkb->max_key_code) {
	*nActsRtrn = XkbError4(0x22,req->firstKeyAction,req->nKeyActions,
							xkb->max_key_code);
	return 0;
    }
    for (nActs=i=0;i<req->nKeyActions;i++) {
	if (wire[0]!=0) {
	    if (wire[0]==symsPerKey[i+req->firstKeyAction])
		nActs+= wire[0];
	    else {
		*nActsRtrn = XkbError3(0x23,i+req->firstKeyAction,wire[0]);
		return 0;
	    }
	}
	wire++;
    }
    if (req->nKeyActions%4)
	wire+= 4-(req->nKeyActions%4);
    /* 8/4/93 (ef) -- XXX! check for legal acts here */
    *wireRtrn = (CARD8 *)(((XkbAnyAction *)wire)+nActs);
    *nActsRtrn = nActs;
    return 1;
}

static int
CheckKeyBehaviors(xkb,req,wireRtrn,errorRtrn)
    XkbDescRec	 	 *xkb;
    xkbSetMapReq	 *req;
    xkbBehaviorWireDesc	**wireRtrn;
    int			 *errorRtrn;
{
register xkbBehaviorWireDesc	*wire = *wireRtrn;
register XkbServerMapPtr	 server = xkb->server;
register unsigned	 	 	 i;

    if (req->totalKeyBehaviors<1) {
	req->present&= ~XkbKeyBehaviorsMask;
	return 1;
    }
    if (!(XkbKeyBehaviorsMask&req->present)) {
	return 1;
    }
    if (XkbKeyBehaviorsMask&req->resize) {
        *errorRtrn = XkbError2(0x30,req->resize);
        return 0;
    }
    for (i=0;i<req->totalKeyBehaviors;i++,wire++) {
	if ((wire->key<xkb->min_key_code)||(wire->key>xkb->max_key_code)) {
	    *errorRtrn = XkbError4(0x31,xkb->min_key_code,xkb->max_key_code,
							wire->key);
	    return 0;
	}
	if (wire->type&XkbKB_Permanent) {
	    *errorRtrn = XkbError3(0x32,wire->key,wire->type);
	    return 0;
	}
	if ((wire->type==XkbKB_RadioGroup)&&(wire->data>XkbMaxRadioGroups)) {
	    *errorRtrn= XkbError4(0x33,wire->key,wire->data,XkbMaxRadioGroups);
	    return 0;
	}
    }
    *wireRtrn = wire;
    return 1;
}

static int
CheckVirtualMods(xkb,req,wireRtrn,errorRtrn)
    XkbDescRec	 	 *xkb;
    xkbSetMapReq	 *req;
    CARD8		**wireRtrn;
    int			 *errorRtrn;
{
register CARD8		*wire = *wireRtrn;
register unsigned 	 i,nMods,bit;

    if (((req->present&XkbVirtualModsMask)==0)||(req->virtualMods==0))
	return 1;
    for (i=nMods=0,bit=1;i<XkbNumVirtualMods;i++,bit<<=1) {
	if (req->virtualMods&bit) {
	    if ((wire[nMods]>7)&&(wire[nMods]!=XkbNoModifier)) {
		*errorRtrn= XkbError3(0x41,i,wire[nMods]);
		return 0;
	    }
	    nMods++;
	}
    }
    *wireRtrn= (wire+XkbPaddedSize(nMods));
    return 1;
}

static int
CheckKeyExplicit(xkb,req,wireRtrn,errorRtrn)
    XkbDescPtr		xkb;
    xkbSetMapReq *	req;
    CARD8 **		wireRtrn;
    int	*		errorRtrn;
{
register CARD8 *	wire = *wireRtrn;
CARD8	*		start;
register unsigned 	i;

    if ((req->present&XkbExplicitComponentsMask)==0)
	return 1;
    start= wire; 
    for (i=0;i<req->totalKeyExplicit;i++,wire+=2) {
	if ((wire[0]<xkb->min_key_code)||(wire[0]>xkb->max_key_code)) {
	    *errorRtrn = XkbError4(0x31,xkb->min_key_code,xkb->max_key_code,
								wire[0]);
	    return 0;
	}
	if (wire[1]&(~XkbAllExplicitMask)) {
	     *errorRtrn= XkbError3(0x52,~XkbAllExplicitMask,wire[1]);
	     return 0;
	}
    }
    wire+= XkbPaddedSize(wire-start)-(wire-start);
    *wireRtrn= wire;
    return 1;
}

static char *
SetKeyTypes(xkb,req,wire,pChanges)
    XkbDescRec		*xkb;
    xkbSetMapReq	*req;
    xkbKeyTypeWireDesc 	*wire;
    xkbMapNotify	*pChanges;
{
register unsigned	i;
unsigned		width,first,last;
CARD8			*map,*preserve;

    if (xkb->map->types[0].free&XkbNoFreeKTStruct) {/* statically allocated */
	XkbKeyTypePtr	pTmp;
	unsigned	nNew;
	nNew= req->firstType+req->nTypes;
	if (nNew<xkb->map->num_types)
	    nNew= xkb->map->num_types;
	pTmp= (XkbKeyTypePtr)Xcalloc(nNew*sizeof(XkbKeyTypeRec));
	if (pTmp==NULL)
	    return NULL;
	memcpy(pTmp,xkb->map->types,xkb->map->num_types*sizeof(XkbKeyTypeRec));
	xkb->map->size_types= nNew;
	xkb->map->types= pTmp;
	xkb->map->types[0].free&= ~XkbNoFreeKTStruct;
    }
    if ((unsigned)(req->firstType+req->nTypes)>xkb->map->size_types) {
	XkbKeyTypeRec	*pNew;
	width = req->firstType+req->nTypes;
	pNew = (XkbKeyTypeRec *)Xrealloc(xkb->map->types,
						width*sizeof(XkbKeyTypeRec));
	if (!pNew)
	    return NULL;
	bzero(&pNew[xkb->map->num_types],
			(width-xkb->map->num_types)*sizeof(XkbKeyTypeRec));
	xkb->map->size_types = width;
	xkb->map->types= pNew;
    }
    if ((unsigned)(req->firstType+req->nTypes)>xkb->map->num_types)
	xkb->map->num_types= req->firstType+req->nTypes;

    for (i=0;i<req->nTypes;i++) {
	XkbKeyTypeRec	*pOld;
	register unsigned n;

	pOld = &xkb->map->types[i+req->firstType];
	if (wire->groupWidth!=pOld->group_width) {
	    _XkbResizeKeyType(xkb,i+req->firstType,wire->groupWidth);
	    pOld = &xkb->map->types[i+req->firstType];
	}
	map = (CARD8 *)&wire[1];
	width= wire->nMapEntries*sizeof(XkbKTMapEntryRec);
	if (pOld->free&XkbNoFreeKTMap)
	    pOld->map = NULL;
	if (!pOld->map) {
	    pOld->map = (XkbKTMapEntryPtr)Xcalloc(width);
	    pOld->free&= ~XkbNoFreeKTMap;
	}
	else if (pOld->map_count<wire->nMapEntries) {
	    pOld->map = (XkbKTMapEntryPtr)Xrealloc(pOld->map,width);
	}
	if (!pOld->map)
	    return NULL;

	if (wire->preserve) {
	    width= wire->nMapEntries*sizeof(XkbKTPreserveRec);
	    if (pOld->preserve==NULL) 
		pOld->preserve= (XkbKTPreservePtr)Xcalloc(width);
	     else {
		pOld->preserve=(XkbKTPreservePtr)Xrealloc(pOld->preserve,width);
	     }
	     if (!pOld->preserve)
		return NULL;
	}
	else if (pOld->preserve) {
	    if ((pOld->free&XkbNoFreeKTPreserve)==0)
		Xfree(pOld->preserve);
	    pOld->preserve= NULL;
	}

	pOld->free = (i+req->firstType==0?0:XkbNoFreeKTMap);
	pOld->real_mods = wire->realMods;
	pOld->vmods= wire->virtualMods;
	pOld->group_width = wire->groupWidth;
	pOld->map_count= wire->nMapEntries;

	pOld->mask= pOld->real_mods|XkbMaskForVMask(xkb,pOld->vmods);

	if (wire->nMapEntries) {
	    xkbKTSetMapEntryWireDesc *mapWire;
	    xkbKTPreserveWireDesc *preWire;
	    unsigned tmp;
	    mapWire= (xkbKTSetMapEntryWireDesc *)map;
	    preWire= (xkbKTPreserveWireDesc *)&mapWire[wire->nMapEntries];
	    for (n=0;n<wire->nMapEntries;n++) {
		pOld->map[n].vmods= mapWire[n].virtualMods;
		pOld->map[n].real_mods= mapWire[n].realMods;
		pOld->map[n].level= mapWire[n].level;
		if (mapWire[n].virtualMods!=0) {
		    tmp= XkbMaskForVMask(xkb,mapWire[n].virtualMods);
		    pOld->map[n].active= (tmp!=0);
		    pOld->map[n].mask= mapWire[n].realMods|tmp;
		}
		if (wire->preserve) {
		    pOld->preserve[n].real_mods= preWire[n].realMods;
		    pOld->preserve[n].vmods= preWire[n].virtualMods;
		    tmp= XkbMaskForVMask(xkb,preWire[n].virtualMods);
		    pOld->preserve[n].mask= preWire[n].realMods|tmp;
		}
	    }
	    if (wire->preserve)
		 map= (CARD8 *)&preWire[wire->nMapEntries];
	    else map= (CARD8 *)&mapWire[wire->nMapEntries];
	}
	wire = (xkbKeyTypeWireDesc *)map;
    }
    first= req->firstType;
    last= first+req->nTypes-1; /* last changed type */
    if (pChanges->changed&XkbKeyTypesMask) {
	int oldLast;
	oldLast= pChanges->firstType+pChanges->nTypes-1;
	if (pChanges->firstType<first)
	    first= pChanges->firstType;
	if (oldLast>last)
	    last= oldLast;
    }
    pChanges->changed|= XkbKeyTypesMask;
    pChanges->firstType = first;
    pChanges->nTypes = (last-first)+1;
    return (char *)wire;
}

static char *
SetKeySyms(client,xkb,req,wire,pChanges)
    ClientPtr		 client;
    XkbDescPtr		 xkb;
    xkbSetMapReq	*req;
    xkbSymMapWireDesc	*wire;
    xkbMapNotify	*pChanges;
{
register unsigned i;
XkbSymMapRec *oldMap;
KeySym *newSyms,*pSyms;
unsigned	first,last;

    oldMap = &xkb->map->key_sym_map[req->firstKeySym];
    for (i=0;i<req->nKeySyms;i++,oldMap++) {
	if (client->swapped) {
	    register int n;
	    swaps(wire->nSyms,n);
	}
	pSyms = (KeySym *)&wire[1];
	if (wire->nSyms>0) {
	    newSyms = _XkbNewSymsForKey(xkb,i+req->firstKeySym,wire->nSyms);
	    memcpy((char *)newSyms,(char *)pSyms,wire->nSyms*4);
	    if (client->swapped) {
		register unsigned n;
		for (i=0;i<wire->nSyms;i++) {
		    swapl(&newSyms[i],n);
		}
	    }
	}
	oldMap->kt_index = wire->ktIndex;
	oldMap->group_info = wire->groupInfo;
	wire= (xkbSymMapWireDesc *)&pSyms[wire->nSyms];
    }
    first= req->firstKeySym;
    last= first+req->nKeySyms-1;
    if (pChanges->changed&XkbKeySymsMask) {
	int oldLast= (pChanges->firstKeySym+pChanges->nKeySyms-1);
	if (pChanges->firstKeySym<first)
	    first= pChanges->firstKeySym;
	if (oldLast>last)
	    last= oldLast;
if (xkbDebugFlags) ErrorF("SetKeySyms: changing range to: %d..%d\n",first,last);
    }
else if (xkbDebugFlags) ErrorF("SetKeySyms: new range: %d..%d\n",first,last);
    pChanges->changed|= XkbKeySymsMask;
    pChanges->firstKeySym = first;
    pChanges->nKeySyms = (last-first+1);
    return (char *)wire;
}

static char *
SetKeyActions(xkb,req,wire,pChanges)
    XkbDescRec		*xkb;
    xkbSetMapReq	*req;
    CARD8		*wire;
    xkbMapNotify	*pChanges;
{
register unsigned	i,first,last;
CARD8 *			nActs = wire;
XkbAction *		newActs;
    
    wire+= XkbPaddedSize(req->nKeyActions);
    for (i=0;i<req->nKeyActions;i++) {
	if (nActs[i]==0)
	    xkb->server->key_acts[i+req->firstKeyAction]= 0;
	else {
	    newActs= _XkbNewActionsForKey(xkb,i+req->firstKeyAction,nActs[i]);
	    memcpy((char *)newActs,(char *)wire,
					nActs[i]*SIZEOF(xkbActionWireDesc));
	    wire+= nActs[i]*SIZEOF(xkbActionWireDesc);
	}
    }
    first= req->firstKeyAction;
    last= (first+req->nKeyActions-1);
    if (pChanges->changed&XkbKeyActionsMask) {
	int oldLast;
	oldLast= pChanges->firstKeyAction+pChanges->nKeyActions-1;
	if (pChanges->firstKeyAction<first)
	    first= pChanges->firstKeyAction;
	if (oldLast>last)
	    last= oldLast;
    }
    pChanges->changed|= XkbKeyActionsMask;
    pChanges->firstKeyAction = first;
    pChanges->nKeyActions = (last-first+1);
    return (char *)wire;
}

static char *
SetKeyBehaviors(xkbSI,req,wire,pChanges)
    XkbSrvInfoPtr	 xkbSI;
    xkbSetMapReq	*req;
    xkbBehaviorWireDesc	*wire;
    xkbMapNotify	*pChanges;
{
register unsigned i;
int maxRG = -1;
XkbDescPtr       xkb = &xkbSI->desc;
XkbServerMapPtr	 server = xkb->server;
unsigned	 lowKey,highKey;


    highKey= xkb->min_key_code;
    lowKey=  xkb->max_key_code;
    for (i=0;i<req->totalKeyBehaviors;i++) {
	if (server->behaviors[wire->key].type&XkbKB_Permanent==0) {
	    server->behaviors[wire->key].type= wire->type;
	    server->behaviors[wire->key].data= wire->data;
	    if ((wire->type==XkbKB_RadioGroup)&&(((int)wire->data)>maxRG))
		maxRG= wire->data;
	    if (wire->key<lowKey)	lowKey= wire->key;
	    if (wire->key>highKey)	highKey= wire->key;
	}
	wire++;
    }
    if (highKey<lowKey)
	return (char *)wire;

    if (maxRG>(int)xkbSI->nRadioGroups) {
        int sz = (maxRG+1)*sizeof(XkbRadioGroupRec);
        if (xkbSI->radioGroups)
            xkbSI->radioGroups=(XkbRadioGroupRec *)Xrealloc(xkbSI->radioGroups,
                                                                        sz);
        else xkbSI->radioGroups= (XkbRadioGroupRec *)Xcalloc(sz);
        if (xkbSI->radioGroups) {
             if (xkbSI->nRadioGroups)
                bzero(&xkbSI->radioGroups[xkbSI->nRadioGroups],
                        (maxRG-xkbSI->nRadioGroups)*sizeof(XkbRadioGroupRec));
             xkbSI->nRadioGroups= maxRG+1;
        }
        else xkbSI->nRadioGroups= 0;
        /* should compute members here */
    }
    if (pChanges->changed&XkbKeyBehaviorsMask) {
	unsigned oldLast;
	oldLast= pChanges->firstKeyBehavior+pChanges->nKeyBehaviors-1;
        if (pChanges->firstKeyBehavior<lowKey)
            lowKey= pChanges->firstKeyBehavior;
        if (oldLast>highKey)
            highKey= oldLast;
    }
    pChanges->changed|= XkbKeyBehaviorsMask;
    pChanges->firstKeyBehavior = lowKey;
    pChanges->nKeyBehaviors = (highKey-lowKey+1);
    return (char *)wire;
}

static char *
SetVirtualMods(xkbSI,req,wire,pChanges)
    XkbSrvInfoRec	*xkbSI;
    xkbSetMapReq	*req;
    CARD8		*wire;
    xkbMapNotify	*pChanges;
{
register int i,bit,nMods;
XkbServerMapRec      *xkb = xkbSI->desc.server;

    if (((req->present&XkbVirtualModsMask)==0)||(req->virtualMods==0))
	return (char *)wire;
    for (i=nMods=0,bit=1;i<16;i++,bit<<=1) {
	if (req->virtualMods&bit) {
	    if (xkb->vmods[i]!=wire[nMods]) {
		pChanges->changed|= XkbVirtualModsMask;
		pChanges->virtualMods|= bit;
		xkb->vmods[i]= wire[nMods];
	    }
	    nMods++;
	}
    }
    return (char *)(wire+XkbPaddedSize(nMods));
}

static char *
SetKeyExplicit(xkbSI,req,wire,pChanges)
    XkbSrvInfoPtr	xkbSI;
    xkbSetMapReq *	req;
    CARD8 *		wire;
    xkbMapNotify *	pChanges;
{
register unsigned	i,first,last;
XkbServerMapPtr		xkb = xkbSI->desc.server;
CARD8 *			start;

    start= wire;
    first= last= -1;
    for (i=0;i<req->totalKeyExplicit;i++,wire+= 2) {
	if (xkb->explicit[wire[0]]!=wire[1]) {
	    xkb->explicit[wire[0]]= *wire;
	    if ((first<0)||(wire[0]<first))
		 first= wire[0];
	    if ((last<0)||(wire[0]>last))
		last= wire[0];
	}
    }
    if (first>0) {
	if (pChanges->changed&XkbExplicitComponentsMask) {
	    int oldLast;
	    oldLast= pChanges->firstKeyExplicit+pChanges->nKeyExplicit-1;
	    if (pChanges->firstKeyExplicit<first)
		first= pChanges->firstKeyExplicit;
	    if (oldLast>last)
		last= oldLast;
	}
	pChanges->firstKeyExplicit= first;
	pChanges->nKeyExplicit= (last-first)+1;
    }
    wire+= XkbPaddedSize(wire-start)-(wire-start);
    return (char *)wire;
}

int
ProcXkbSetMap(client)
    ClientPtr client;
{
    DeviceIntPtr	 dev;
    XkbDescRec		*xkb;
    xkbMapNotify	 mn;
    REQUEST(xkbSetMapReq);
    int	nTypes,nActions,nKeySyms,error;
    char	*tmp;
    CARD8	 mapWidths[256];
    CARD16	 symsPerKey[256];

    REQUEST_AT_LEAST_SIZE(xkbSetMapReq);
    dev = XkbLookupDevice(stuff->deviceSpec);
    if  (!dev) {
	client->errorValue = XkbError2(0xff,stuff->deviceSpec);
	return XkbKeyboardErrorCode;
    }
    xkb = &dev->key->xkbInfo->desc;

    tmp = (char *)&stuff[1];
    if (!CheckKeyTypes(client,xkb,stuff,(xkbKeyTypeWireDesc **)&tmp,
						&nTypes,mapWidths)) {
	client->errorValue = nTypes;
	return BadValue;
    }
    if (!CheckKeySyms(client,xkb,stuff,nTypes,mapWidths,symsPerKey,
						(XkbSymMapRec **)&tmp,&error)) {
	client->errorValue = error;
	return BadValue;
    }

    if (!CheckKeyActions(xkb,stuff,nTypes,mapWidths,symsPerKey,
						(CARD8 **)&tmp,&nActions)){
	client->errorValue = nActions;
	return BadValue;
    }

    if (!CheckKeyBehaviors(xkb,stuff,(xkbBehaviorWireDesc**)&tmp,&error)) {
	client->errorValue = error;
	return BadValue;
    }

    if (!CheckVirtualMods(xkb,stuff,(CARD8 **)&tmp,&error)) {
	client->errorValue= error;
	return BadValue;
    }
    if (!CheckKeyExplicit(xkb,stuff,(CARD8 **)&tmp,&error)) {
	client->errorValue= error;
	return BadValue;
    }

    if (((tmp-((char *)stuff))/4)!=stuff->length) {
	ErrorF("Internal error! Bad length in XkbSetMap (after check)\n");
	client->errorValue = tmp-((char *)&stuff[1]);
	return BadLength;
    }
    bzero(&mn,sizeof(mn));
    tmp = (char *)&stuff[1];
    if (stuff->present&XkbKeyTypesMask) {
	tmp = SetKeyTypes(xkb,stuff,(xkbKeyTypeWireDesc *)tmp,&mn);
	if (!tmp)	goto allocFailure;
    }
    if (stuff->present&XkbKeySymsMask) {
	tmp = SetKeySyms(client,xkb,stuff,(xkbSymMapWireDesc *)tmp,&mn);
	if (!tmp)	goto allocFailure;
    }
    if (stuff->present&XkbKeyActionsMask) {
	tmp = SetKeyActions(xkb,stuff,(CARD8 *)tmp,&mn);
	if (!tmp)	goto allocFailure;
    }
    if (stuff->present&XkbKeyBehaviorsMask) {
	tmp= SetKeyBehaviors(dev->key->xkbInfo,stuff,
				(XkbBehavior *)tmp,&mn);
	if (!tmp)	goto allocFailure;
    }
    if (stuff->present&XkbVirtualModsMask)
	SetVirtualMods(dev->key->xkbInfo,stuff,(CARD8 *)tmp,&mn);
    if (stuff->present&XkbVirtualModsMask)
	SetKeyExplicit(dev->key->xkbInfo,stuff,(CARD8 *)tmp,&mn);
    if (((tmp-((char *)stuff))/4)!=stuff->length) {
	ErrorF("Internal error! Bad length in XkbSetMap (after set)\n");
	client->errorValue = tmp-((char *)&stuff[1]);
	return BadLength;
    }
    if (mn.changed)
	XkbSendMapNotify(dev,&mn);

    XkbUpdateCoreDescription(dev);
    return client->noClientException;
allocFailure:
    return BadAlloc;
}

/***====================================================================***/

int
ProcXkbGetCompatMap(client)
    ClientPtr client;
{
    REQUEST(xkbGetCompatMapReq);
    xkbGetCompatMapReply rep;
    DeviceIntPtr dev;
    XkbDescRec *xkb;
    XkbCompatRec *compat;
    unsigned	 size,nMods,nVMods;
    char	*data;

    REQUEST_SIZE_MATCH(xkbGetCompatMapReq);
    dev = (DeviceIntPtr)XkbLookupDevice(stuff->deviceSpec);
    if (!dev) {
	client->errorValue = XkbError2(0xff,stuff->deviceSpec);
	return XkbKeyboardErrorCode;
    }
    xkb = &dev->key->xkbInfo->desc;
    compat= xkb->compat;

    rep.type = X_Reply;
    rep.sequenceNumber = client->sequence;
    rep.length = 0;
    rep.firstSI = stuff->firstSI;
    rep.nSI = stuff->nSI;
    if (stuff->getAllSI) {
	rep.firstSI = 0;
	rep.nSI = compat->num_si;
    }
    else if ((((unsigned)stuff->nSI)>0)&&
		((unsigned)(stuff->firstSI+stuff->nSI-1)>=compat->num_si)) {
	client->errorValue = XkbError2(0x05,compat->num_si);
	return BadValue;
    }
    rep.mods= stuff->mods;
    rep.virtualMods= stuff->virtualMods;
    nMods= nVMods= 0;
    if (stuff->mods!=0) {
	register int i,bit;
	for (i=0,bit=1;i<XkbNumModifiers;i++,bit<<=1) {
	    if (stuff->mods&bit)
		nMods++;
	}
    }
    if (stuff->virtualMods!=0) {
	register int i,bit;
	for (i=0,bit=1;i<XkbNumVirtualMods;i++,bit<<=1) {
	    if (stuff->virtualMods&bit)
		nVMods++;
	}
    }

    rep.deviceID = dev->id;
    rep.nTotalSI = compat->num_si;

    size = XkbPaddedSize(((nMods+nVMods)*sz_xkbModCompatWireDesc));
    size+= (rep.nSI*sz_xkbSymInterpretWireDesc);
    rep.length= size/4;
    if (size) {
	data = (char *)ALLOCATE_LOCAL(size);
	if (data) {
	    register unsigned i,bit;
	    xkbModCompatWireDesc *mod;
	    XkbSymInterpretRec *sym= &compat->sym_interpret[rep.firstSI];
	    xkbSymInterpretWireDesc *wire = (xkbSymInterpretWireDesc *)data;
	    for (i=0;i<rep.nSI;i++,sym++,wire++) {
		wire->sym= sym->sym;
		wire->mods= sym->mods;
		wire->match= sym->mods;
		wire->virtualMod= sym->virtual_mod;
		wire->flags= sym->flags;
		memcpy((char*)&wire->act,(char*)&sym->act,sz_xkbActionWireDesc);
		if (client->swapped) {
		    register int n;
		    swapl(&wire->sym,n);
		}
	    }
	    mod = (xkbModCompatWireDesc *)wire;
	    if (rep.mods) {
		for (i=0,bit=1;i<XkbNumModifiers;i++,bit<<=1) {
		    if (rep.mods&bit) {
			mod->mods= compat->real_mod_compat[i].mods;
			mod->groups= compat->real_mod_compat[i].groups;
			mod++;
		    }
		}
	    }
	    if (rep.virtualMods) {
		for (i=0,bit=1;i<XkbNumVirtualMods;i++,bit<<=1) {
		    if (rep.virtualMods&bit) {
			mod->mods= compat->vmod_compat[i].mods;
			mod->groups= compat->vmod_compat[i].groups;
			mod++;
		    }
		}
	    }
	}
	else return BadAlloc;
    }
    else data= NULL;

    if (client->swapped) {
	register int n;
	swaps(&rep.sequenceNumber,n);
	swapl(&rep.length,n);
	swaps(&rep.firstSI,n);
	swaps(&rep.nSI,n);
	swaps(&rep.nTotalSI,n);
	swaps(&rep.virtualMods,n);
    }

    WriteToClient(client, sizeof(xkbGetCompatMapReply), (char *)&rep);
    if (data) {
	WriteToClient(client, size, data);
	DEALLOCATE_LOCAL((char *)data);
    }
    return client->noClientException;
}

int
ProcXkbSetCompatMap(client)
    ClientPtr client;
{
    REQUEST(xkbSetCompatMapReq);
    DeviceIntPtr dev;
    XkbSrvInfoRec *xkb;
    XkbCompatRec *compat;
    char	*data;
    int		 nMods,nVMods;

    REQUEST_AT_LEAST_SIZE(xkbSetCompatMapReq);
    dev = (DeviceIntPtr)XkbLookupDevice(stuff->deviceSpec);
    if (!dev) {
	client->errorValue = XkbError2(0xff,stuff->deviceSpec);
	return XkbKeyboardErrorCode;
    }
    data = (char *)&stuff[1];
    xkb = dev->key->xkbInfo;
    compat= xkb->desc.compat;
    if ((stuff->nSI>0)||(stuff->truncateSI)) {
	register unsigned i;
	xkbSymInterpretWireDesc *wire = (xkbSymInterpretWireDesc *)data;
	if (stuff->firstSI>compat->num_si) {
	    client->errorValue = XkbError2(0x02,compat->num_si);
	    return BadValue;
	}
	for (i=0;i<stuff->nSI;i++,wire++) {
	    /* 8/4/93 (ef) -- XXX! verify act and behavior here */
	}
	data = (char *)wire;
    }
    nMods=nVMods=0;
    if (stuff->mods!=0) {
	register unsigned i,bit;
	for (i=0,bit=1;i<8;i++,bit<<=1) {
	    if ( stuff->mods&bit )
		nMods++;
	}
    }
    if (stuff->virtualMods!=0) {
	register unsigned i,bit;
	for (i=0,bit=1;i<16;i++,bit<<=1) {
	    if ( stuff->virtualMods&bit )
		nVMods++;
	}
    }
    data+= XkbPaddedSize((nMods+nVMods)*sizeof(XkbModCompatRec));
    if (((data-((char *)stuff))/4)!=stuff->length) {
	return BadLength;
    }
    data = (char *)&stuff[1];
    if (stuff->nSI>0) {
	register unsigned i;
	xkbSymInterpretWireDesc *wire = (xkbSymInterpretWireDesc *)data;
	XkbSymInterpretRec *sym;
	if ((unsigned)(stuff->firstSI+stuff->nSI)>compat->num_si) {
	    compat->num_si= stuff->firstSI+stuff->nSI;
	    compat->sym_interpret= (XkbSymInterpretRec *)Xrealloc(
			compat->sym_interpret,
			compat->num_si*sizeof(XkbSymInterpretRec));
	    if (!compat->sym_interpret) {
		compat->num_si= 0;
		return BadAlloc;
	    }
	}
	else if (stuff->truncateSI) {
	    compat->num_si = stuff->firstSI+stuff->nSI;
	}
	sym = &compat->sym_interpret[stuff->firstSI];
	for (i=0;i<stuff->nSI;i++,wire++,sym++) {
	    sym->sym= wire->sym;
	    sym->mods= wire->mods;
	    sym->match= wire->match;
	    sym->flags= wire->flags;
	    sym->virtual_mod= wire->virtualMod;
	    memcpy((char *)&sym->act,(char *)&wire->act,sz_xkbActionWireDesc);
	}
	data = (char *)wire;
    }
    else if (stuff->truncateSI) {
	compat->num_si = stuff->firstSI;
    }

    if (stuff->mods!=0) {
	register unsigned i,bit=1;
	xkbModCompatWireDesc *wire = (xkbModCompatWireDesc *)data;
	for (i=0,bit=1;i<8;i++,bit<<=1) {
	    if (stuff->mods&bit) {
		compat->real_mod_compat[i].mods= wire->mods;
		compat->real_mod_compat[i].groups= wire->groups;
		wire++;
	    }
	}
    }
    if (stuff->virtualMods!=0) {
	register int i,bit=1;
	xkbModCompatWireDesc *wire = (xkbModCompatWireDesc *)data;
	for (i=0,bit=1;i<16;i++,bit<<=1) {
	    if (stuff->virtualMods&bit) {
		compat->vmod_compat[i].mods= wire->mods;
		compat->vmod_compat[i].groups= wire->groups;
		wire++;
	    }
	}
    }
    if (xkb->interest) {
	xkbCompatMapNotify ev;
	ev.deviceID = dev->id;
	ev.changedMods = stuff->mods;
	ev.changedVirtualMods = stuff->virtualMods;
	ev.firstSI = stuff->firstSI;
	ev.nSI = stuff->nSI;
	ev.nTotalSI = compat->num_si;
	XkbSendCompatMapNotify(dev,&ev);
    }

    if (stuff->recomputeActions) {
	XkbChangesRec	change;
	bzero(&change,sizeof(XkbChangesRec));
	XkbUpdateActions(dev,xkb->desc.min_key_code,xkb->desc.max_key_code,
								&change);
	XkbUpdateCoreDescription(dev);
	XkbSendNotification(dev,&change,0,0,XkbReqCode,X_kbSetCompatMap);
    }
    return client->noClientException;
}

/***====================================================================***/

int
ProcXkbGetIndicatorState(client)
    ClientPtr client;
{
    REQUEST(xkbGetIndicatorStateReq);
    xkbGetIndicatorStateReply rep;
    DeviceIntPtr dev;
    CARD8 *map;
    register int i,bit;
    int nIndicators;

    REQUEST_SIZE_MATCH(xkbGetIndicatorStateReq);
    dev = (DeviceIntPtr)XkbLookupDevice(stuff->deviceSpec);
    if ( !dev ) {
	client->errorValue = XkbError2(0xff,stuff->deviceSpec);
	return XkbKeyboardErrorCode;
    }
    rep.type = X_Reply;
    rep.sequenceNumber = client->sequence;
    rep.length = 0;
    rep.deviceID = dev->id;
    rep.state = dev->key->xkbInfo->iStateEffective;
    if (client->swapped) {
	swaps(&rep.sequenceNumber,i);
	swapl(&rep.state,i);
    }
    WriteToClient(client, sizeof(xkbGetIndicatorStateReply), (char *)&rep);
    return client->noClientException;
}

int
ProcXkbGetIndicatorMap(client)
    ClientPtr client;
{
    REQUEST(xkbGetIndicatorMapReq);
    xkbGetIndicatorMapReply rep;
    DeviceIntPtr dev;
    XkbDescRec *xkb;
    XkbIndicatorRec *leds;
    CARD8 *map;
    register int i,bit;
    int nIndicators,length;

    REQUEST_SIZE_MATCH(xkbGetIndicatorMapReq);
    dev = (DeviceIntPtr)XkbLookupDevice(stuff->deviceSpec);
    if ( !dev ) {
	client->errorValue = XkbError2(0xff,stuff->deviceSpec);
	return XkbKeyboardErrorCode;
    }
    xkb= &dev->key->xkbInfo->desc;
    leds= xkb->indicators;

    nIndicators= 0;
    for (i=0,bit=1;i<XkbNumIndicators;i++,bit<<=1) {
	if (stuff->which&bit)
	    nIndicators++;
    }

    length = nIndicators*sz_xkbIndicatorMapWireDesc;
    rep.type = X_Reply;
    rep.sequenceNumber = client->sequence;
    rep.length = length/4;
    rep.deviceID = dev->id;
    rep.nRealIndicators = leds->num_phys_indicators;
    rep.which = stuff->which;

    if (nIndicators>0) {
	CARD8 *to;
	to= map= (CARD8 *)ALLOCATE_LOCAL(length);
	if (map) {
	    xkbIndicatorMapWireDesc  *wire = (xkbIndicatorMapWireDesc *)to;
	    for (i=0,bit=1;i<XkbNumIndicators;i++,bit<<=1) {
		if (rep.which&bit) {
		    wire->flags= leds->maps[i].flags;
		    wire->whichGroups= leds->maps[i].which_groups;
		    wire->groups= leds->maps[i].groups;
		    wire->whichMods= leds->maps[i].which_mods;
		    wire->mods= leds->maps[i].mask;
		    wire->realMods= leds->maps[i].real_mods;
		    wire->virtualMods= leds->maps[i].vmods;
		    wire->ctrls= leds->maps[i].ctrls;
		    if (client->swapped) {
			register int n;
			swaps(&wire->virtualMods,n);
			swapl(&wire->ctrls,n);
		    }
		    wire++;
		}
	    }
	    to = (CARD8 *)wire;
	    if ((to-map)!=length) {
		client->errorValue = XkbError2(0xff,length);
		return BadLength;
	    }
	}
	else return BadAlloc;
    }
    else map = NULL;
    if (client->swapped) {
	swaps(&rep.sequenceNumber,i);
	swapl(&rep.length,i);
	swapl(&rep.which,i);
    }
    WriteToClient(client, sizeof(xkbGetIndicatorMapReply), (char *)&rep);
    if (map) {
	WriteToClient(client, length, (char *)map);
	DEALLOCATE_LOCAL((char *)map);
    }
    return(client->noClientException);
}

int
ProcXkbSetIndicatorMap(client)
    ClientPtr client;
{
    register int i,bit;
    int	nIndicators;
    DeviceIntPtr dev;
    XkbIndicatorRec	*leds;
    XkbSrvInfoRec 	*xkb;
    xkbIndicatorMapWireDesc *from;
    REQUEST(xkbSetIndicatorMapReq);

    REQUEST_AT_LEAST_SIZE(xkbSetIndicatorMapReq);
    dev = XkbLookupDevice(stuff->deviceSpec);
    if (!dev) {
	client->errorValue = XkbError2(0xff,stuff->deviceSpec);
	return XkbKeyboardErrorCode;
    }
    xkb= dev->key->xkbInfo;
    leds= xkb->desc.indicators;

    if (stuff->which==0) {
	client->errorValue= 0x01;
	return BadValue;
    }
    nIndicators= 0;
    for (i=0,bit=1;i<XkbNumIndicators;i++,bit<<=1) {
	if (stuff->which&bit)
	    nIndicators++;
    }
    if (stuff->length!=
      ((sz_xkbSetIndicatorMapReq+(nIndicators*sz_xkbIndicatorMapWireDesc))/4)) {
	return BadLength;
    }

    from = (xkbIndicatorMapWireDesc *)&stuff[1];
    for (i=0,bit=1;i<XkbNumIndicators;i++,bit<<=1) {
	if (stuff->which&bit) {
	    CARD8 which= (from->whichMods|from->whichGroups);
	    if (client->swapped) {
		register int n;
		swaps(&from->virtualMods,n);
		swapl(&from->ctrls,n);
	    }

	    leds->maps[i].flags = from->flags;
	    leds->maps[i].which_groups = from->whichGroups;
	    leds->maps[i].groups = from->groups;
	    leds->maps[i].which_mods = from->whichMods;
	    leds->maps[i].mask = from->mods;
	    leds->maps[i].vmods= from->virtualMods;
	    leds->maps[i].ctrls = from->ctrls;

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
	    if (from->ctrls)
		 xkb->iAccel.usesControls|= bit;
	    else xkb->iAccel.usesControls&= ~bit;

	    if (from->ctrls || (from->whichGroups && from->groups) ||
						(from->whichMods && from->mods))
		 xkb->iAccel.haveMap|= bit;
	    else xkb->iAccel.haveMap&= ~bit;
/* 2/23/94 (ef) -- XXX! recompute real mods here if virtual mods != 0 */
	    from++;
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
    if (stuff->which)
	XkbUpdateIndicators(dev,stuff->which,NULL);
    return client->noClientException;
}

/***====================================================================***/

static CARD32
_XkbCountAtoms(atoms,maxAtoms,count)
    Atom *atoms;
    int   maxAtoms;
    int  *count;
{
register unsigned int i,bit,nAtoms;
register CARD32 atomsPresent;

    for (i=nAtoms=atomsPresent=0,bit=1;i<maxAtoms;i++,bit<<=1) {
	if (atoms[i]!=None) {
	    atomsPresent|= bit;
	    nAtoms++;
	}
    }
    if (count)
	*count= nAtoms;
    return atomsPresent;
}

static char *
_XkbWriteAtoms(wire,atoms,maxAtoms,swap)
    char *wire;
    Atom *atoms;
    int   maxAtoms;
    int   swap;
{
register unsigned int i;
Atom *atm;

    atm = (Atom *)wire;
    for (i=0;i<maxAtoms;i++) {
	if (atoms[i]!=None) {
	    *atm= atoms[i];
	    if (swap) {
		register int n;
		swapl(atm,n);
	    }
	    atm++;
	}
    }
    return (char *)atm;
}

int
ProcXkbGetNames(client)
    ClientPtr client;
{
    DeviceIntPtr dev;
    XkbDescRec *xkb;
    xkbGetNamesReply rep;
    register unsigned i,length;
    unsigned which;
    char *start,*desc;
    REQUEST(xkbGetNamesReq);

    REQUEST_SIZE_MATCH(xkbGetNamesReq);
    dev = (DeviceIntPtr)XkbLookupDevice(stuff->deviceSpec);
    if (!dev) {
	client->errorValue = XkbError2(0xff,stuff->deviceSpec);
	return XkbKeyboardErrorCode;
    }
    xkb = &dev->key->xkbInfo->desc;
    rep.type= X_Reply;
    rep.sequenceNumber= client->sequence;
    rep.length = length= 0;
    rep.deviceID = dev->id;
    rep.which = which= stuff->which;
    rep.nTypes = xkb->map->num_types;
    rep.firstKey = xkb->min_key_code;
    rep.nKeys = xkb->max_key_code-xkb->min_key_code+1;
    rep.nRadioGroups = xkb->names->num_rg;
    rep.nCharSets = xkb->names->num_char_sets;

    if ((which&XkbKeycodesNameMask)&&(xkb->names->keycodes!=None))
	 length++;
    else which&= ~XkbKeycodesNameMask;
    if ((which&XkbGeometryNameMask)&&(xkb->names->geometry!=None))
	 length++;
    else which&= ~XkbGeometryNameMask;
    if ((which&XkbSymbolsNameMask)&&(xkb->names->symbols!=None))
	 length++;
    else which&= ~XkbSymbolsNameMask;
    if ((which&XkbSemanticsNameMask)&&(xkb->names->semantics!=None))
	 length++;
    else which&= ~XkbSemanticsNameMask;

    if (which&XkbKeyTypeNamesMask)
	 length+= xkb->map->num_types;
    if (which&XkbKTLevelNamesMask) {
	XkbKeyTypeRec *pType = xkb->map->types;
	length+= XkbPaddedSize(xkb->map->num_types)/4;
	for (i=0;i<xkb->map->num_types;i++,pType++) {
	    if (pType->lvl_names!=NULL)
		length+= pType->group_width;
	}
    }
    if (which&XkbIndicatorNamesMask) {
	int nLeds;
	rep.indicators= 
		_XkbCountAtoms(xkb->names->indicators,XkbNumIndicators,&nLeds);
	length+= nLeds;
	if (nLeds==0)
	    which&= ~XkbIndicatorNamesMask;
    }
    else rep.indicators= 0;

    if (which&XkbModifierNamesMask) {
	unsigned int nMods;
	rep.modifiers=
		_XkbCountAtoms(xkb->names->mods,XkbNumModifiers,&nMods);
	length+= nMods;
	if (nMods==0)
	    which&= ~XkbModifierNamesMask;
    }
    else rep.modifiers= 0;

    if (which&XkbVirtualModNamesMask) {
	unsigned int nVMods;
	rep.virtualMods= 
	     _XkbCountAtoms(xkb->names->vmods,XkbNumVirtualMods,&nVMods);
	length+= nVMods;
	if (nVMods==0)
	    which&= ~XkbVirtualModNamesMask;
    }
    else rep.virtualMods= 0;

    if ((which&XkbKeyNamesMask)&&(xkb->names->keys))
	 length+= rep.nKeys;
    else which&= ~XkbKeyNamesMask;

    if ((which&XkbRGNamesMask)&&(xkb->names->num_rg>0))
	 length+= xkb->names->num_rg;
    else which&= ~XkbRGNamesMask;

    if ((which&XkbCharSetsMask)&&(xkb->names->num_char_sets>0))
	 length+= xkb->names->num_char_sets;
    else which&= ~XkbCharSetsMask;

    if ((which&XkbPhysicalNamesMask)&&
	((xkb->names->phys_symbols!=None)||(xkb->names->phys_geometry!=None)))
	 length+= 2;
    else which&= ~XkbSemanticsNameMask;

    rep.length= length;
    rep.which= which;
    if (client->swapped) {
	register int n;
	swaps(&rep.sequenceNumber,n);
	swapl(&rep.length,n);
	swapl(&rep.which,n);
	swaps(&rep.virtualMods,n);
	swapl(&rep.indicators,n);
    }

    start = desc = (char *)ALLOCATE_LOCAL(length*4);
    if ( !start )
	return BadAlloc;
    if (which&XkbKeycodesNameMask) {
	*((CARD32 *)desc)= xkb->names->keycodes;
	if (client->swapped) {
	    register int n;
	    swapl(desc,n);
	}
	desc+= 4;
    }
    if (which&XkbGeometryNameMask)  {
	*((CARD32 *)desc)= xkb->names->geometry;
	if (client->swapped) {
	    register int n;
	    swapl(desc,n);
	}
	desc+= 4;
    }
    if (which&XkbSymbolsNameMask) {
	*((CARD32 *)desc)= xkb->names->symbols;
	if (client->swapped) {
	    register int n;
	    swapl(desc,n);
	}
	desc+= 4;
    }
    if (which&XkbSemanticsNameMask) {
	*((CARD32 *)desc)= (CARD32)xkb->names->semantics;
	if (client->swapped) {
	    register int n;
	    swapl(desc,n);
	}
	desc+= 4;
    }
    if (which&XkbKeyTypeNamesMask) {
	register CARD32 *atm= (CARD32 *)desc;
	register XkbKeyTypePtr type= xkb->map->types;

	for (i=0;i<xkb->map->num_types;i++,atm++,type++) {
	    *atm= (CARD32)type->name;
	    if (client->swapped) {
		register int n;
		swapl(atm,n);
	    }
	}
	desc= (char *)atm;
    }
    if (which&XkbKTLevelNamesMask) {
	XkbKeyTypePtr type = xkb->map->types;
	register CARD32 *atm;
	for (i=0;i<rep.nTypes;i++,type++) {
	    *desc++ = type->group_width;
	}
	desc+= XkbPaddedSize(rep.nTypes)-rep.nTypes;

	atm= (CARD32 *)desc;
	type = xkb->map->types;
	for (i=0;i<xkb->map->num_types;i++,type++) {
	    register unsigned l;
	    if (type->lvl_names) {
		for (l=0;l<type->group_width;l++,atm++) {
		    *atm= type->lvl_names[l];
		    if (client->swapped) {
			register unsigned n;
			swapl(atm,n);
		    }
		}
		desc+= type->group_width*4;
	    }
	}
    }
    if (which&XkbIndicatorNamesMask) {
	desc= _XkbWriteAtoms(desc,xkb->names->indicators,XkbNumIndicators,
							 client->swapped);
    }
    if (which&XkbModifierNamesMask) {
	desc= _XkbWriteAtoms(desc,xkb->names->mods,XkbNumModifiers,
							client->swapped);
    }
    if (which&XkbVirtualModNamesMask) {
	desc= _XkbWriteAtoms(desc,xkb->names->vmods,XkbNumVirtualMods,
							client->swapped);
    }
    if (which&XkbKeyNamesMask) {
	for (i=0;i<rep.nKeys;i++,desc+= sizeof(XkbKeyNameRec)) {
	    *((XkbKeyNamePtr)desc)= xkb->names->keys[i+xkb->min_key_code];
	}
    }
    if ((which&XkbRGNamesMask)&&(rep.nRadioGroups>0)) {
	register CARD32	*atm= (CARD32 *)desc;
	for (i=0;i<rep.nRadioGroups;i++,atm++) {
	    *atm= (CARD32)xkb->names->radio_groups[i];
	    if (client->swapped) {
		register unsigned n;
		swapl(atm,n);
	    }
	}
	desc+= rep.nRadioGroups*4;
    }
    if ((which&XkbCharSetsMask)&&(rep.nCharSets>0)) {
	register int nLeft,nSets;
	register Atom *cs= xkb->names->char_sets;
	nLeft= rep.nCharSets;
	while (nLeft>0) {
	    if (nLeft>32)	nSets= 32;
	    else		nSets= nLeft;

	    desc= _XkbWriteAtoms(desc,cs,nSets,client->swapped);
	    nLeft-= nSets;
	    cs+= nSets;
	}
    }
    if (which&XkbPhysicalNamesMask) {
	register CARD32 *atm= (CARD32 *)desc;
	atm[0]= (CARD32)xkb->names->phys_symbols;
	atm[1]= (CARD32)xkb->names->phys_geometry;
	if (client->swapped) {
	    register int n;
	    swapl(&atm[0],n);
	    swapl(&atm[1],n);
	}
	desc+= 8;
    }
    if ((desc-start)!=(length*4)) {
	ErrorF("BOGUS LENGTH in write names, expected %d, got %d\n",
					length*4, desc-start);
    }
    WriteToClient(client, sizeof(rep), (char *)&rep);
    WriteToClient(client, length*4, start);
    DEALLOCATE_LOCAL((char *)start);
    return client->noClientException;
}

/***====================================================================***/

static CARD32 *
_XkbCheckAtoms(wire,nAtoms,swapped,pError)
    CARD32 *wire;
    int   nAtoms;
    int   swapped;
    Atom *pError;
{
register int i;

    for (i=0;i<nAtoms;i++,wire++) {
	if (swapped) {
	    register int n;
	    swapl(wire,n);
	}
	if ((((Atom)*wire)!=None)&&(!ValidAtom((Atom)*wire))) {
	    *pError= ((Atom)*wire);
	    return NULL;
	}
    }
    return wire;
}

static CARD32 *
_XkbCheckMaskedAtoms(wire,nAtoms,present,swapped,pError)
    CARD32	*wire;
    int   	 nAtoms;
    CARD32	 present;
    int		 swapped;
    Atom	*pError;
{
register unsigned i,bit;

    for (i=0,bit=1;(i<nAtoms)&&(present);i++,bit<<=1) {
	if ((present&bit)==0)
	    continue;
	if (swapped) {
	    register int n;
	    swapl(wire,n);
	}
	if ((((Atom)*wire)!=None)&&(!ValidAtom(((Atom)*wire)))) {
	    *pError= (Atom)*wire;
	    return NULL;
	}
	wire++;
    }
    return wire;
}

static Atom *
_XkbCopyMaskedAtoms(wire,dest,nAtoms,present)
    Atom	*wire;
    Atom	*dest;
    int   	 nAtoms;
    CARD32	 present;
{
register int i,bit;

    for (i=0,bit=1;(i<nAtoms)&&(present);i++,bit<<=1) {
	if ((present&bit)==0)
	    continue;
	dest[i]= *wire++;
    }
    return wire;
}

int
ProcXkbSetNames(client)
    ClientPtr client;
{
    DeviceIntPtr	 dev;
    XkbDescRec		*xkb;
    XkbNamesRec		*names;
    xkbNamesNotify	 nn;
    CARD32		*tmp;
    Atom		 bad;
    REQUEST(xkbSetNamesReq);

    REQUEST_AT_LEAST_SIZE(xkbSetNamesReq);
    dev = (DeviceIntPtr)XkbLookupDevice(stuff->deviceSpec);
    if  (!dev) {
	client->errorValue = XkbError2(0xff,stuff->deviceSpec);
	return XkbKeyboardErrorCode;
    }
    xkb = &dev->key->xkbInfo->desc;
    names = xkb->names;
    tmp = (CARD32 *)&stuff[1];

    if (stuff->which&XkbKeycodesNameMask) {
	tmp= _XkbCheckAtoms(tmp,1,client->swapped,&bad);
	if (!tmp) {
	    client->errorValue = bad;
	    return BadAtom;
	}
    }
    if (stuff->which&XkbGeometryNameMask) {
	tmp= _XkbCheckAtoms(tmp,1,client->swapped,&bad);
	if (!tmp) {
	    client->errorValue = bad;
	    return BadAtom;
	}
    }
    if (stuff->which&XkbSymbolsNameMask) {
	tmp= _XkbCheckAtoms(tmp,1,client->swapped,&bad);
	if (!tmp) {
	    client->errorValue = bad;
	    return BadAtom;
	}
    }
    if (stuff->which&XkbSemanticsNameMask) {
	tmp= _XkbCheckAtoms(tmp,1,client->swapped,&bad);
	if (!tmp) {
	    client->errorValue = bad;
	    return BadAtom;
	}
    }
    if (stuff->which&XkbKeyTypeNamesMask) {
	register int i;
	if ( stuff->nTypes<1 ) {
	    client->errorValue = XkbError2(0x01,stuff->nTypes);
	    return BadValue;
	}
	if ((unsigned)(stuff->firstType+stuff->nTypes-1)>=xkb->map->num_types) {
	    client->errorValue = XkbError4(0x02,stuff->firstType,
				stuff->nTypes,xkb->map->num_types);
	    return BadValue;
	}
	tmp= _XkbCheckAtoms(tmp,stuff->nTypes,client->swapped,&bad);
	if (!tmp) {
	    client->errorValue= bad;
	    return BadAtom;
	}
    }
    if (stuff->which&XkbKTLevelNamesMask) {
	register unsigned i,l;
	XkbKeyTypeRec *type;
	CARD8 *width;
	if ( stuff->nKTLevels<1 ) {
	    client->errorValue = XkbError2(0x03,stuff->nKTLevels);
	    return BadValue;
	}
	if ((unsigned)(stuff->firstKTLevel+stuff->nKTLevels-1)>=
							xkb->map->num_types) {
	    client->errorValue = XkbError4(0x04,stuff->firstKTLevel,
				stuff->nKTLevels,xkb->map->num_types);
	    return BadValue;
	}
	width = (CARD8 *)tmp;
	tmp+= XkbPaddedSize(stuff->nKTLevels);
	type = &xkb->map->types[stuff->firstType];
	for (i=0;i<stuff->nTypes;i++,type++) {
	    if (width[i]==0)
		continue;
	    else if (width[i]!=type->group_width) {
		client->errorValue= XkbError4(0x05,i+stuff->firstType,
						type->group_width,width[i]);
		return BadMatch;
	    }
	    tmp= _XkbCheckAtoms(tmp,width[i],client->swapped,&bad);
	    if (!tmp) {
		client->errorValue= bad;
		return BadAtom;
	    }
	}
    }
    if (stuff->which&XkbIndicatorNamesMask) {
	if (stuff->indicators==0) {
	    client->errorValue= 0x06;
	    return BadMatch;
	}
	tmp= _XkbCheckMaskedAtoms(tmp,XkbNumIndicators,stuff->indicators,
							client->swapped,&bad);
	if (!tmp) {
	    client->errorValue= bad;
	    return BadAtom;
	}
    }
    if (stuff->which&XkbModifierNamesMask) {
	if (stuff->modifiers==0) {
	    client->errorValue= 0x07;
	    return BadMatch;
	}
	tmp= _XkbCheckMaskedAtoms(tmp,XkbNumModifiers,stuff->modifiers,
							client->swapped,&bad);
	if (!tmp) {
	    client->errorValue= bad;
	    return BadAtom;
	}
    }
    if (stuff->which&XkbVirtualModNamesMask) {
	if (stuff->virtualMods==0) {
	    client->errorValue= 0x08;
	    return BadMatch;
	}
	tmp= _XkbCheckMaskedAtoms(tmp,XkbNumVirtualMods,stuff->virtualMods,
							client->swapped,&bad);
	if (!tmp) {
	    client->errorValue = bad;
	    return BadAtom;
	}
    }
    if (stuff->which&XkbKeyNamesMask) {
	if (stuff->firstKey<(unsigned)xkb->min_key_code) {
	    client->errorValue= XkbError3(0x09,xkb->min_key_code,stuff->firstKey);
	    return BadValue;
	}
	if (((unsigned)(stuff->firstKey+stuff->nKeys-1)>xkb->max_key_code)||
							(stuff->nKeys<1)) {
	    client->errorValue= XkbError4(0x0a,xkb->max_key_code,stuff->firstKey,
								stuff->nKeys);
	    return BadValue;
	}
	tmp+= (stuff->nKeys*sizeof(XkbKeyNameRec))/sizeof(Atom);
    }
    if (stuff->which&XkbRGNamesMask) {
	if ( stuff->nRadioGroups<1 ) {
	    client->errorValue= XkbError2(0x06,stuff->nRadioGroups);
	    return BadValue;
	}
	tmp= _XkbCheckAtoms(tmp,stuff->nRadioGroups,client->swapped,&bad);
	if (!tmp) {
	    client->errorValue= bad;
	    return BadAtom;
	}
    }
    if (stuff->which&XkbCharSetsMask) {
	tmp= _XkbCheckAtoms(tmp,stuff->nCharSets,client->swapped,&bad);
	if (!tmp) {
	    client->errorValue= bad;
	    return BadAtom;
	}
    }
    if (stuff->which&XkbPhysicalNamesMask) {
	tmp= _XkbCheckAtoms(tmp,2,client->swapped,&bad);
	if (!tmp) {
	    client->errorValue= bad;
	    return BadAtom;
	}
    }
    if ((tmp-((CARD32 *)stuff))!=stuff->length) {
	client->errorValue = stuff->length;
	return BadLength;
    }

    /* everything is okay -- update names */
    bzero(&nn,sizeof(xkbNamesNotify));
    nn.changed= stuff->which;
    tmp = (CARD32 *)&stuff[1];
    if (stuff->which&XkbKeycodesNameMask)
	names->keycodes= *tmp++;
    if (stuff->which&XkbGeometryNameMask)
	names->geometry= *tmp++;
    if (stuff->which&XkbSymbolsNameMask)
	names->symbols= *tmp++;
    if (stuff->which&XkbSemanticsNameMask)
	names->semantics= *tmp++;
    if ((stuff->which&XkbKeyTypeNamesMask)&&(stuff->nTypes>0)) {
	register unsigned i;
	register XkbKeyTypePtr type;

	type= &xkb->map->types[stuff->firstType];
	for (i=0;i<stuff->nTypes;tmp++) {
	    type->name= *tmp;
	}
	nn.firstType= stuff->firstType;
	nn.nTypes= stuff->nTypes;
    }
    if (stuff->which&XkbKTLevelNamesMask) {
	register XkbKeyTypePtr	type;
	register unsigned i,l;
	CARD8 *width;

	width = (CARD8 *)tmp;
	tmp+= XkbPaddedSize(stuff->nKTLevels);
	type= &xkb->map->types[stuff->firstKTLevel];
	for (i=0;i<stuff->nTypes;i++,type++) {
	    if (width[i]>0) {
		if ((!type->lvl_names)||(type->free&XkbNoFreeKTLevelNames)) {
		    type->lvl_names= (Atom *)Xcalloc(width[i]*sizeof(Atom));
		    type->free&= ~XkbNoFreeKTLevelNames;
		}
		if (type->lvl_names) {
		    register unsigned n;
		    for (n=0;n<width[i];n++) {
			type->lvl_names[n]= tmp[n];
		    }
		}
		tmp+= width[i];
	    }
	}
	nn.firstLevelName= 0;
	nn.nLevelNames= stuff->nTypes;
    }
    if (stuff->which&XkbIndicatorNamesMask) {
	tmp= _XkbCopyMaskedAtoms(tmp,names->indicators,XkbNumIndicators,
							stuff->indicators);
	nn.changedIndicators= stuff->indicators;
    }
    if (stuff->which&XkbModifierNamesMask) {
	tmp= _XkbCopyMaskedAtoms(tmp,names->mods,XkbNumModifiers,
							stuff->modifiers);
	nn.changedMods= stuff->modifiers;
    }
    if (stuff->which&XkbVirtualModNamesMask) {
	tmp= _XkbCopyMaskedAtoms(tmp,names->vmods,XkbNumVirtualMods,
							stuff->virtualMods);
	nn.changedVirtualMods= stuff->virtualMods;
    }
    if (stuff->which&XkbKeyNamesMask) {
	if (names->keys==NULL) {
	    int totalKeys= xkb->max_key_code-xkb->min_key_code+1;
	    names->keys=(XkbKeyNamePtr)Xcalloc(sizeof(XkbKeyNameRec)*totalKeys);
	    if (names->keys==NULL)
		return BadAlloc;
	}
	memcpy((char*)&names->keys[stuff->firstKey],(char *)tmp,
				     stuff->nKeys*sizeof(XkbKeyNameRec));
	tmp+= stuff->nKeys;
    }
    if (stuff->which&XkbRGNamesMask) {
	if (stuff->nRadioGroups>0) {
	    register unsigned i;
	    if (names->radio_groups==NULL) {
		names->radio_groups= (Atom *)
				Xcalloc(sizeof(Atom)*stuff->nRadioGroups);
	    }
	    else if (stuff->nRadioGroups>=names->num_rg) {
		names->radio_groups=(Atom*)Xrealloc(names->radio_groups,
					sizeof(Atom)*stuff->nRadioGroups);
	    }

	    if (names->radio_groups!=NULL) {
		names->num_rg= stuff->nRadioGroups;
		if (stuff->nRadioGroups>0) {
		    for (i=0;i<stuff->nRadioGroups;i++) {
			names->radio_groups[i]= tmp[i];
		    }
		    tmp+= stuff->nRadioGroups;
		}
	    }
	    else {
		names->num_rg= 0;
		return BadAlloc;
	    }
	}
	else if (names->radio_groups) {
	    Xfree(names->radio_groups);
	    names->radio_groups= NULL;
	    names->num_rg= 0;
	}
	nn.firstRadioGroup= 0;
	nn.nRadioGroups= stuff->nRadioGroups;
    }
    if (stuff->which&XkbCharSetsMask) {
	if (stuff->nCharSets==0) {
	    if (names->char_sets) 
		Xfree(names->char_sets);
	    names->char_sets= NULL;
	    names->num_char_sets= 0;
	}
	else {
	    if (stuff->nCharSets>names->num_char_sets) {
		if (names->num_char_sets==0)
		    names->char_sets= (Atom *)Xcalloc(stuff->nCharSets*
								sizeof(Atom));
		else {
		    names->char_sets=(Atom *)Xrealloc(names->char_sets,
						stuff->nCharSets*sizeof(Atom));
		}
	    }
	    names->num_char_sets= 0;
	    if (names->char_sets!=NULL) {
		register unsigned i;
		names->num_char_sets= stuff->nCharSets;
		for (i=0;i<stuff->nCharSets;i++) {
		    names->char_sets[i]= tmp[i];
		}
		tmp+= stuff->nCharSets;
	    }
	}
	nn.nCharSets= names->num_char_sets;
    }
    if (nn.changed)
	XkbSendNamesNotify(dev,&nn);
    return client->noClientException;
}

/***====================================================================***/

int
ProcXkbListAlternateSyms(client)
    ClientPtr client;
{
    DeviceIntPtr 	dev;
    XkbDescPtr		xkb;
    XkbAlternateSymsPtr	syms;
    xkbListAlternateSymsReply rep;
    unsigned	nMatch,szMatch,length;
    char	*data;
    REQUEST(xkbListAlternateSymsReq);

    REQUEST_SIZE_MATCH(xkbListAlternateSymsReq);
    dev = (DeviceIntPtr)XkbLookupDevice(stuff->deviceSpec);
    if  (!dev) {
	client->errorValue = XkbError2(0xff,stuff->deviceSpec);
	return XkbKeyboardErrorCode;
    }
    xkb = &dev->key->xkbInfo->desc;
    if ((stuff->name!=None)&&(!ValidAtom(stuff->name))) {
	client->errorValue = stuff->name;
	return BadAtom;
    }
    if ((stuff->charset!=None)&&(!ValidAtom(stuff->charset))) {
	client->errorValue = stuff->charset;
	return BadAtom;
    }
    szMatch= nMatch = 0;
    syms = xkb->alt_syms;
    while (syms) {
	if ((stuff->name==None)||(stuff->name==syms->name)) {
	    if (stuff->charset!=None) {
		register unsigned i;
		for (i=0;i<syms->num_char_sets;i++) {
		   if (syms->char_sets[i]==stuff->charset) {
			nMatch++;
			szMatch+= 4;	/* room for index and nCharSets */
			szMatch+= (1+syms->num_char_sets)*sizeof(Atom);
			break;
		   }
		}
	    }
	    else {
		nMatch++;
		szMatch+= 4;	/* room for index and nCharSets */
		szMatch+= (1+syms->num_char_sets)*sizeof(Atom);
	    }
	}
	syms= syms->next;
    }
    length= szMatch;

    rep.type= X_Reply;
    rep.sequenceNumber= client->sequence;
    rep.length = length/4;
    rep.deviceID = dev->id;
    rep.nAlternateSyms = nMatch;
    if (nMatch!=0) {
	Atom *out;
	data= (char *)ALLOCATE_LOCAL(szMatch);
	out = (Atom *)data;
	if (data) {
	    syms = xkb->alt_syms;
	    while (syms) {
		int	match;
		match= 0;
		if ((stuff->name==None)||(stuff->name==syms->name)) {
		    if (stuff->charset!=None) {
			register unsigned i;
			for (i=0;(i<syms->num_char_sets)&&(!match);i++) {
			   if (syms->char_sets[i]==stuff->charset)
				match= 1;
			}
		    }
		}
		else match= 1;
		if (match) {
		    CARD8 *tmp= (CARD8 *)out;
		    register unsigned i;
		    *tmp++= syms->index;
		    *tmp++= syms->num_char_sets;
		    out++;
		    *out= syms->name;
		    if (client->swapped) {
			register int n;
			swapl(out,n);
		    }
		    out++;
		    for (i=0;i<syms->num_char_sets;i++,out++) {
			*out= syms->char_sets[i];
			if (client->swapped) {
			    register int n;
			    swapl(out,n);
			}
		    }
		}
		syms= syms->next;
	    }
    	}
	else {
	    return BadAlloc;
	}
    }
    else data = NULL;
    if (client->swapped) {
	register int n;
	swaps(&rep.sequenceNumber,n);
	swapl(&rep.length,n);
    }
    WriteToClient(client, sz_xkbListAlternateSymsReply, (char *)&rep);
    if (data) {
	WriteToClient(client, length, data);
	DEALLOCATE_LOCAL((char *)data);
    }
    return client->noClientException;
}

int
ProcXkbGetAlternateSyms(client)
    ClientPtr client;
{
    DeviceIntPtr dev;
    XkbDescRec	*xkb;
    XkbAlternateSymsRec *syms;
    xkbGetAlternateSymsReply rep;
    unsigned	  i,totalSyms,length;
    XkbSymMapRec *sMap;
    char	 *data,*to;
    REQUEST(xkbGetAlternateSymsReq);

    REQUEST_SIZE_MATCH(xkbGetAlternateSymsReq);
    dev = (DeviceIntPtr)XkbLookupDevice(stuff->deviceSpec);
    if  (!dev) {
	client->errorValue = XkbError2(0xff,stuff->deviceSpec);
	return XkbKeyboardErrorCode;
    }
    xkb = &dev->key->xkbInfo->desc;
    if (stuff->nKeys>0) {
	if (stuff->firstKey<xkb->min_key_code) {
	    client->errorValue= XkbError4(0x01,stuff->firstKey,stuff->nKeys,
							xkb->min_key_code);
	    return BadValue;
	}
	if ((unsigned)(stuff->firstKey+stuff->nKeys-1)>xkb->max_key_code) {
	    client->errorValue= XkbError4(0x02,stuff->firstKey,stuff->nKeys,
							xkb->max_key_code);
	    return BadValue;
	}
    }
    syms= xkb->alt_syms;
    while (syms) {
	if (syms->index==stuff->index)
	     break;
	else syms= syms->next;
    }
    if (!syms) {
	client->errorValue= stuff->index;
	return BadValue;
    }

    rep.type= X_Reply;
    rep.sequenceNumber= client->sequence;
    rep.length = 0;
    rep.deviceID = dev->id;
    rep.name= syms->name;
    rep.index= syms->index;
    rep.nCharSets= syms->num_char_sets;
    if (stuff->nKeys>0) {
	int reqLast,setLast;
	if (stuff->firstKey<syms->first_key)
	     rep.firstKey= syms->first_key;
	else rep.firstKey= stuff->firstKey;
	setLast= syms->first_key+syms->num_keys-1;
	reqLast= stuff->firstKey+stuff->nKeys-1;
	if (reqLast>setLast)
	     rep.nKeys= setLast-rep.firstKey+1;
	else rep.nKeys= reqLast-rep.firstKey+1;
    }
    else {
	rep.firstKey= rep.nKeys= 0;
    }
    sMap= &syms->maps[rep.firstKey-syms->first_key];
    for (totalSyms=i=0;i<(unsigned)rep.nKeys;i++) {
	if ((unsigned)XkbNumGroups(sMap->group_info)>0) {
	    totalSyms+= XkbNumGroups(sMap->group_info)*
				xkb->map->types[sMap->kt_index].group_width;
	}
    }
    length= (totalSyms*sizeof(Atom))+(rep.nKeys*sizeof(xkbSymMapWireDesc));
    rep.length= length/4;
    if (rep.length) {
	xkbSymMapWireDesc *out;
	KeySym *symOut;
	data= (char *)ALLOCATE_LOCAL(length);
	if (!data)
	    return BadAlloc;
	sMap= &syms->maps[rep.firstKey-syms->first_key];
	out= (xkbSymMapWireDesc *)data;
	for (i=0;i<rep.nKeys;i++) {
	    register unsigned n;
	    if (XkbNumGroups(sMap->group_info)==0) {
		out->ktIndex= 0;
		out->groupInfo= 0;
		out->nSyms= 0;
		out++;
		continue;
	    }
	    n= xkb->map->types[sMap->kt_index].group_width;
	    n*= XkbNumGroups(sMap->group_info);
	    out->ktIndex= sMap->kt_index;
	    out->groupInfo= sMap->group_info;
	    out->nSyms= n;
	    if (client->swapped) {
		swaps(&out->nSyms,n);
	    }
	    symOut= (KeySym *)&out[1];
	    for (n=0;n<out->nSyms;n++,symOut++) {
		*symOut= syms->syms[sMap->offset+n];
		if (client->swapped) {
		    register int s;
		    swapl(symOut,s);
		}
	    }
	    out= (xkbSymMapWireDesc *)symOut;
	}
    }
    else data= NULL;
    if (client->swapped) {
	register int n;
	swaps(&rep.sequenceNumber,n);
	swapl(&rep.length,n);
	swapl(&rep.name,n);
	swaps(&rep.totalSyms,n);
    }
    WriteToClient(client, sz_xkbGetAlternateSymsReply, (char *)&rep);
    if (data) {
	WriteToClient(client, length, data);
	DEALLOCATE_LOCAL((char *)data);
    }
    return client->noClientException;
}

/***====================================================================***/

int
ProcXkbSetDebuggingFlags(client)
    ClientPtr client;
{
    extern int XkbDisableLockActions;
    CARD16 newFlags,extraLength;
    int status;
    DeviceIntPtr dev;
    xkbSetDebuggingFlagsReply rep;
    REQUEST(xkbSetDebuggingFlagsReq);

    REQUEST_AT_LEAST_SIZE(xkbSetDebuggingFlagsReq);
    newFlags= (xkbDebugFlags&(~stuff->mask))|(stuff->flags&stuff->mask);
    if (xkbDebugFlags || newFlags || stuff->msgLength) {
	ErrorF("XkbDebug: Setting debug flags to %d\n",newFlags);
    }
    extraLength= (stuff->length<<2)-sz_xkbSetDebuggingFlagsReq;
    if (stuff->msgLength>0) {
	register int i;
	char *msg;
	if (extraLength!=XkbPaddedSize(stuff->msgLength)) {
	    ErrorF("XkbDebug: msgLength= %d, length= %d (should be %d)\n",
			stuff->msgLength,extraLength,
			XkbPaddedSize(stuff->msgLength));
	    return BadLength;
	}
	msg= (char *)&stuff[1];
	if (msg[stuff->msgLength-1]!='\0') {
	    ErrorF("XkbDebug: message not null-terminated\n");
	    return BadValue;
	}
	ErrorF("XkbDebug: %s\n",msg);
    }
    else if (extraLength!=0) {
	ErrorF("XkbDebug: msgLength==0, length=%d (should be 0)\n",
					extraLength);
	return BadLength;
    }
    xkbDebugFlags = newFlags;
    if (stuff->disableLocks!=XkbLeaveLocks)
	XkbDisableLockActions = stuff->disableLocks;
    rep.type= X_Reply;
    rep.length = 0;
    rep.sequenceNumber = client->sequence;
    rep.currentFlags = newFlags;
    rep.disableLocks = XkbDisableLockActions;
    if ( client->swapped ) {
	register int n;
	swaps(&rep.sequenceNumber, n);
	swaps(&rep.currentFlags, n);
    }
    WriteToClient(client,sizeof(xkbSetDebuggingFlagsReply), (char *)&rep);
    return client->noClientException;
}

/***====================================================================***/

static int
ProcXkbDispatch (client)
    ClientPtr client;
{
    REQUEST(xReq);
    switch (stuff->data)
    {
    case X_kbUseExtension:
	return ProcXkbUseExtension(client);
    case X_kbSelectEvents:
	return ProcXkbSelectEvents(client);
    case X_kbSendEvent:
	return ProcXkbSendEvent(client);
    case X_kbBell:
	return ProcXkbBell(client);
    case X_kbGetState:
	return ProcXkbGetState(client);
    case X_kbLatchLockState:
	return ProcXkbLatchLockState(client);
    case X_kbGetControls:
	return ProcXkbGetControls(client);
    case X_kbSetControls:
	return ProcXkbSetControls(client);
    case X_kbGetMap:
	return ProcXkbGetMap(client);
    case X_kbSetMap:
	return ProcXkbSetMap(client);
    case X_kbGetCompatMap:
	return ProcXkbGetCompatMap(client);
    case X_kbSetCompatMap:
	return ProcXkbSetCompatMap(client);
    case X_kbGetIndicatorState:
	return ProcXkbGetIndicatorState(client);
    case X_kbGetIndicatorMap:
	return ProcXkbGetIndicatorMap(client);
    case X_kbSetIndicatorMap:
	return ProcXkbSetIndicatorMap(client);
    case X_kbGetNames:
	return ProcXkbGetNames(client);
    case X_kbSetNames:
	return ProcXkbSetNames(client);
    case X_kbListAlternateSyms:
	return ProcXkbListAlternateSyms(client);
    case X_kbGetAlternateSyms:
	return ProcXkbGetAlternateSyms(client);
#ifdef NOTYET
    case X_kbSetAlternateSyms:
	return ProcXkbSetAlternateSyms(client);
    case X_kbGetGeometry:
	return ProcXkbGetGeometry(client);
    case X_kbSetGeometry:
	return ProcXkbSetGeometry(client);
#endif
    case X_kbSetDebuggingFlags:
	return ProcXkbSetDebuggingFlags(client);
    default:
	return BadRequest;
    }
}

static int
XkbClientGone(data,id)
    pointer data;
    XID id;
{
    DevicePtr	pXDev = (DevicePtr)data;

    if (!XkbRemoveResourceClient(pXDev,id)) {
	ErrorF("Internal Error! bad RemoveResourceClient in XkbClientGone\n");
    }
    return 1;
}

static void
XkbResetProc(extEntry)
    ExtensionEntry *extEntry;
{
}

void
XkbExtensionInit()
{
    ExtensionEntry *extEntry, *AddExtension();

    if (extEntry = AddExtension(XkbName, XkbNumberEvents, XkbNumberErrors,
				 ProcXkbDispatch, SProcXkbDispatch,
				 XkbResetProc, StandardMinorOpcode)) {
	XkbReqCode = (unsigned char)extEntry->base;
	XkbEventBase = (unsigned char)extEntry->eventBase;
	XkbErrorBase = (unsigned char)extEntry->errorBase;
	XkbKeyboardErrorCode = XkbErrorBase+XkbKeyboard;
	RT_XKBCLIENT = CreateNewResourceType(XkbClientGone);
    }
    return;
}
