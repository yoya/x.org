/* $XConsortium: xkbAccessX.c,v 1.7 94/04/07 18:37:35 erik Exp $ */
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
#include "inputstr.h"
#include "XKBsrv.h"

#include <sys/time.h>

int	XkbDfltRepeatDelay=	660;
int	XkbDfltRepeatInterval=	40;

void
AccessXInit(keybd)
    DeviceIntPtr keybd;
{
    XkbSrvInfoRec	*xkbInfo = keybd->key->xkbInfo;
    XkbControlsRec	*ctrls = xkbInfo->desc.ctrls;
    xkbInfo->shiftKeyCount= 0;
    xkbInfo->mouseKeysCounter= 0;
    xkbInfo->inactiveKey= 0;
    xkbInfo->slowKey= 0;
    xkbInfo->repeatKey= 0;
    xkbInfo->krgTimerActive= _OFF_TIMER;
    xkbInfo->accessXFlags= 0;
    xkbInfo->mouseKeyTimer= NULL;
    xkbInfo->slowKeysTimer= NULL;
    xkbInfo->bounceKeysTimer= NULL;
    xkbInfo->repeatKeyTimer= NULL;
    xkbInfo->krgTimer= NULL;
    xkbInfo->accessXFlags= 0;
    ctrls->repeat_delay = XkbDfltRepeatDelay;
    ctrls->repeat_interval = XkbDfltRepeatInterval;
    ctrls->debounce_delay = 300;
    ctrls->slow_keys_delay = 300;
    ctrls->mouse_keys_delay = 160;
    ctrls->mouse_keys_interval = 40;
    ctrls->mouse_keys_time_to_max = 30;
    ctrls->mouse_keys_max_speed = 30;
    ctrls->mouse_keys_curve = 500;
    ctrls->mouse_keys_dflt_btn = 1;
    ctrls->accessx_timeout = 120;
    ctrls->accessx_timeout_mask = KRG_MASK|XkbStickyKeysMask ;
    xkbInfo->mouseKeysCurve= 1.0+(((double)ctrls->mouse_keys_curve)*0.001);
    xkbInfo->mouseKeysCurveFactor= ( ((double)ctrls->mouse_keys_max_speed)/
		pow((double)ctrls->mouse_keys_time_to_max,xkbInfo->mouseKeysCurve));
    return;
}

/************************************************************************/
/*									*/
/* AccessXKeyboardEvent							*/
/*									*/
/*	Generate a synthetic keyboard event.				*/
/*									*/
/************************************************************************/
#if NeedFunctionPrototypes
static void AccessXKeyboardEvent(DeviceIntPtr	keybd,
				 BYTE		type,
				 BYTE		keyCode)
#else
static void AccessXKeyboardEvent(keybd,type,keyCode)
    DeviceIntPtr keybd;
    BYTE	 type;
    BYTE	 keyCode;
#endif
{
    xEvent	xE;
#ifdef XINPUT
    extern int	DeviceKeyPress;
#endif
    
    xE.u.u.type = type;
    xE.u.u.detail = keyCode;
    xE.u.keyButtonPointer.time = GetTimeInMillis();	    

    if ((type==KeyPress) 
#ifdef XINPUT
    || (type==DeviceKeyPress)
#endif
       )
	DDXKeyClick(keybd,keyCode,TRUE);
    XkbProcessKeyboardEvent(&xE,keybd,1L);

    return;
    
} /* AccessXKeyboardEvent */

/************************************************************************/
/*									*/
/* AccessXKRGTurnOn							*/
/*									*/
/*	Turn the keyboard response group on.				*/
/*									*/
/************************************************************************/
#if NeedFunctionPrototypes
static void AccessXKRGTurnOn(DeviceIntPtr	 keybd,
			     CARD16 	  	 KRGControl,
			     xkbControlsNotify	*pCN)
#else
static void AccessXKRGTurnOn(keybd,KRGControl,pCN)
    DeviceIntPtr	keybd;
    CARD16 		KRGControl;
    xkbControlsNotify	*pCN;
#endif
{
    XkbSrvInfoRec	*xkbInfo = keybd->key->xkbInfo;
    XkbControlsRec	*ctrls = xkbInfo->desc.ctrls;
    XkbControlsRec	old;

    old= *ctrls;
    ctrls->enabled_ctrls |= (KRGControl&KRG_MASK);
    if (XkbComputeControlsNotify(keybd,&old,ctrls,pCN))
	XkbSendControlsNotify(keybd,pCN);
    if (xkbInfo->iAccel.usesControls)
	XkbUpdateIndicators(keybd,xkbInfo->iAccel.usesControls,NULL);
    return;
    
} /* AccessXKRGTurnOn */

/************************************************************************/
/*									*/
/* AccessXKRGTurnOff							*/
/*									*/
/*	Turn the keyboard response group off.				*/
/*									*/
/************************************************************************/
#if NeedFunctionPrototypes
static void AccessXKRGTurnOff(DeviceIntPtr keybd,xkbControlsNotify *pCN)
#else
static void AccessXKRGTurnOff(keybd,pCN)
    DeviceIntPtr keybd;
    xkbControlsNotify *pCN;
#endif
{
    XkbSrvInfoRec	*xkbInfo = keybd->key->xkbInfo;
    XkbControlsRec	*ctrls = xkbInfo->desc.ctrls;
    XkbControlsRec	 old;

    old = *ctrls;
    ctrls->enabled_ctrls &= ~KRG_MASK;
    if (XkbComputeControlsNotify(keybd,&old,ctrls,pCN))
	XkbSendControlsNotify(keybd,pCN);
    if (xkbInfo->iAccel.usesControls)
	XkbUpdateIndicators(keybd,xkbInfo->iAccel.usesControls,NULL);
    return;
    
} /* AccessXKRGTurnOff */

/************************************************************************/
/*									*/
/* AccessXStickyKeysTurnOn						*/
/*									*/
/*	Turn StickyKeys on.						*/
/*									*/
/************************************************************************/
#if NeedFunctionPrototypes
static void AccessXStickyKeysTurnOn(DeviceIntPtr keybd,xkbControlsNotify *pCN)
#else
static void AccessXStickyKeysTurnOn(keybd,pCN)
    DeviceIntPtr	 keybd;
    xkbControlsNotify	*pCN;
#endif
{
    XkbSrvInfoRec	*xkbInfo = keybd->key->xkbInfo;
    XkbControlsRec	*ctrls = xkbInfo->desc.ctrls;
    XkbControlsRec	 old;
    
    old = *ctrls;
    ctrls->enabled_ctrls |= XkbStickyKeysMask;
    xkbInfo->shiftKeyCount = 0;
    if (XkbComputeControlsNotify(keybd,&old,ctrls,pCN)) 
	XkbSendControlsNotify(keybd,pCN);
    if (xkbInfo->iAccel.usesControls)
	XkbUpdateIndicators(keybd,xkbInfo->iAccel.usesControls,NULL);
    return;
    
} /* AccessXStickyKeysTurnOn */

/************************************************************************/
/*									*/
/* AccessXStickyKeysTurnOff						*/
/*									*/
/*	Turn StickyKeys off.						*/
/*									*/
/************************************************************************/
#if NeedFunctionPrototypes
static void AccessXStickyKeysTurnOff(DeviceIntPtr keybd,xkbControlsNotify *pCN)
#else
static void AccessXStickyKeysTurnOff(keybd,pCN)
    DeviceIntPtr	 keybd;
    xkbControlsNotify	*pCN;
#endif
{
    XkbSrvInfoRec	*xkbInfo = keybd->key->xkbInfo;
    XkbControlsRec	*ctrls = xkbInfo->desc.ctrls;
    XkbControlsRec	 old;
    unsigned ledUpdate;

    old = *ctrls;
    ctrls->enabled_ctrls &= ~XkbStickyKeysMask;
    xkbInfo->shiftKeyCount = 0;
    if (XkbComputeControlsNotify(keybd,&old,ctrls,pCN))
	XkbSendControlsNotify(keybd,pCN);

    if (xkbInfo->state.locked_mods || xkbInfo->state.locked_group) {
	xkbInfo->state.locked_mods= 0;
	xkbInfo->state.locked_group= 0;
	ledUpdate= XkbIndicatorsToUpdate(keybd,
					(XkbModifierLockMask|XkbGroupLockMask));
	ledUpdate|= xkbInfo->iAccel.usesControls;
    }
    else ledUpdate= xkbInfo->iAccel.usesControls;
    if (ledUpdate)
	XkbUpdateIndicators(keybd,ledUpdate,NULL);
    return;
    
} /* AccessXStickyKeysTurnOff */

/************************************************************************/
/*									*/
/* AccessXTwoKeysDown							*/
/*									*/
/*	Turn StickyKeys Off if the user pressed a modifier key and	*/
/*	another key at the same time.					*/
/*									*/
/************************************************************************/
#if NeedFunctionPrototypes
void AccessXTwoKeysDown(DeviceIntPtr keybd,xkbControlsNotify *pCN)
#else
void AccessXTwoKeysDown(keybd,pCN)
    DeviceIntPtr	 keybd;
    xkbControlsNotify	*pCN;
#endif
{
    XkbSrvInfoRec	*xkbInfo = keybd->key->xkbInfo;
    
    if (xkbInfo->accessXFlags & TWO_KEYS_MASK) {
	/* unlatch and unlock locked modifiers if this option is called. */
	AccessXStickyKeysTurnOff(keybd,pCN);
    }
    
} /* AccessXTwoKeysDown */

static CARD32
AccessXKRGExpire(timer,now,arg)
    OsTimerPtr	 timer;
    CARD32	 now;
    pointer	 arg;
{
XkbSrvInfoRec	*xkbInfo= ((DeviceIntPtr)arg)->key->xkbInfo;
xkbControlsNotify	cn;

    cn.keycode = 0;
    cn.eventType = 0;
    cn.requestMajor = 0;
    cn.requestMinor = 0;
    if (xkbInfo->desc.ctrls->enabled_ctrls&XkbSlowKeysMask)
	 AccessXKRGTurnOff((DeviceIntPtr)arg,&cn);
    else AccessXKRGTurnOn((DeviceIntPtr)arg,XkbSlowKeysMask,&cn);
    xkbInfo->krgTimerActive= _OFF_TIMER;
    return 0;
}

static CARD32
AccessXRepeatKeyExpire(timer,now,arg)
    OsTimerPtr	 timer;
    CARD32	 now;
    pointer	 arg;
{
XkbSrvInfoRec	*xkbInfo= ((DeviceIntPtr)arg)->key->xkbInfo;
    if (xkbInfo->repeatKey!=0) {
	AccessXKeyboardEvent((DeviceIntPtr)arg,KeyRelease,xkbInfo->repeatKey);
	AccessXKeyboardEvent((DeviceIntPtr)arg,KeyPress,xkbInfo->repeatKey);
	return xkbInfo->desc.ctrls->repeat_interval;
    }
    return 0;
}

void
AccessXCancelRepeatKey(xkbInfo,key)
    XkbSrvInfoPtr	xkbInfo;
    KeyCode		key;
{
    if (xkbInfo->repeatKey==key) {
	xkbInfo->repeatKey= 0;
    }
    return;
}

static CARD32
AccessXSlowKeyExpire(timer,now,arg)
    OsTimerPtr	 timer;
    CARD32	 now;
    pointer	 arg;
{
DeviceIntPtr	 keybd= (DeviceIntPtr)arg;
XkbSrvInfoPtr	 xkbInfo= keybd->key->xkbInfo;

    if (xkbInfo->slowKey!=0) {
	xkbSlowKeyNotify ev;
	ev.slowKeyState= XkbSK_Accept;
	ev.keycode= xkbInfo->slowKey;
	ev.delay= xkbInfo->desc.ctrls->slow_keys_delay;
	XkbSendSlowKeyNotify(keybd,&ev);
	AccessXKeyboardEvent(keybd,KeyPress,xkbInfo->slowKey);

	/* Start repeating if necessary.  Stop autorepeating if the user
	 * presses a non-modifier key that doesn't autorepeat.
	 */
	if (keybd->kbdfeed->ctrl.autoRepeat && 
	    (xkbInfo->desc.ctrls->enabled_ctrls&XkbRepeatKeysMask)) {
#ifndef AIXV3
	    if (BitIsOn(keybd->kbdfeed->ctrl.autoRepeats,xkbInfo->slowKey))
#endif
	    {
		xkbInfo->repeatKey = xkbInfo->slowKey;
		xkbInfo->repeatKeyTimer= TimerSet(xkbInfo->repeatKeyTimer,
					0, xkbInfo->desc.ctrls->repeat_delay,
					AccessXRepeatKeyExpire, (pointer)keybd);
	    }
	}
    }
    return 0;
}

static CARD32
AccessXBounceKeyExpire(timer,now,arg)
    OsTimerPtr	 timer;
    CARD32	 now;
    pointer	 arg;
{
XkbSrvInfoRec	*xkbInfo= ((DeviceIntPtr)arg)->key->xkbInfo;

    xkbInfo->inactiveKey= 0;
    return 0;
}

static CARD32
AccessXTimeoutExpire(timer,now,arg)
    OsTimerPtr	 timer;
    CARD32	 now;
    pointer	 arg;
{
DeviceIntPtr	 keybd = (DeviceIntPtr)arg;
XkbSrvInfoRec	*xkbInfo= keybd->key->xkbInfo;
XkbControlsRec	*ctrls= xkbInfo->desc.ctrls;
XkbControlsRec	 old;
xkbControlsNotify	cn;

    old= *ctrls;
    if (xkbInfo->lastPtrEventTime) {
	unsigned timeLeft;
	timeLeft= (xkbInfo->desc.ctrls->accessx_timeout*1000);
	timeLeft-= (now-xkbInfo->lastPtrEventTime);
	return timeLeft;
    }
    ctrls->enabled_ctrls&= ~(xkbInfo->desc.ctrls->accessx_timeout_mask);
    if (XkbComputeControlsNotify(keybd,&old,ctrls,&cn)) {
	cn.keycode = 0;
	cn.eventType = 0;
	cn.requestMajor = 0;
	cn.requestMinor = 0;
	XkbSendControlsNotify(keybd,&cn);
    }
    if (xkbInfo->iAccel.usesControls)
	XkbUpdateIndicators(keybd,xkbInfo->iAccel.usesControls,NULL);
    xkbInfo->krgTimerActive= _OFF_TIMER;
    return 0;
}


/************************************************************************/
/*									*/
/* AccessXFilterPressEvent						*/
/*									*/
/* Filter events before they get any further if SlowKeys is turned on.	*/
/* In addition, this routine handles the ever so popular magic key	*/
/* acts for turning various accessibility features on/off.		*/
/*									*/
/* Returns TRUE if this routine has discarded the event.		*/
/* Returns FALSE if the event needs further processing.			*/
/*									*/
/************************************************************************/
#if NeedFunctionPrototypes
Bool AccessXFilterPressEvent(register xEvent 		*xE, 
				    register DeviceIntPtr	keybd, 
				    int				count)
#else
Bool AccessXFilterPressEvent(xE,keybd,count)
    register xEvent 		*xE;
    register DeviceIntPtr	keybd;
    int				count;    
#endif
{
    XkbSrvInfoRec	*xkbInfo = keybd->key->xkbInfo;
    XkbControlsRec	*ctrls = xkbInfo->desc.ctrls;
    Bool		 ignoreKeyEvent = FALSE;
    KeyCode		 key = xE->u.u.detail;
    KeySym		*sym = XkbKeySymsPtr(&xkbInfo->desc,key);

    if (ctrls->enabled_ctrls&XkbAccessXKeysMask) {
	/* check for magic sequences */
	if ((sym[0]==XK_Shift_R)||(sym[0]==XK_Shift_L)) {
	    xkbInfo->krgTimerActive = _KRG_TIMER;
	    xkbInfo->krgTimer= TimerSet(xkbInfo->krgTimer, 0,4000,
					AccessXKRGExpire, (pointer)keybd);
	    xkbInfo->shiftKeyCount++;
	}
	else {
	    if (xkbInfo->krgTimerActive) {
		xkbInfo->krgTimer= TimerSet(xkbInfo->krgTimer,0, 0, NULL, NULL);
		xkbInfo->krgTimerActive= _OFF_TIMER;
	    }
	}
    }
	
    /* Don't transmit the KeyPress if SlowKeys is turned on;
     * The wakeup handler will synthesize one for us if the user
     * has held the key long enough.
     */
    if (ctrls->enabled_ctrls & XkbSlowKeysMask) {
	xkbSlowKeyNotify	ev;
	ev.slowKeyState= XkbSK_Press;
	ev.keycode= key;
	ev.delay= xkbInfo->desc.ctrls->slow_keys_delay;
	XkbSendSlowKeyNotify(keybd,&ev);
	xkbInfo->slowKey= key;
	xkbInfo->slowKeysTimer = TimerSet(xkbInfo->slowKeysTimer,
				 0, xkbInfo->desc.ctrls->slow_keys_delay,
				 AccessXSlowKeyExpire, (pointer)keybd);
	ignoreKeyEvent = TRUE;
    }

    /* Don't transmit the KeyPress if BounceKeys is turned on
     * and the user pressed the same key within a given time period
     * from the last release.
     */
    else if ((ctrls->enabled_ctrls & XkbBounceKeysMask) && 
					(key == xkbInfo->inactiveKey)) {
	ignoreKeyEvent = TRUE;
    }

    /* Start repeating if necessary.  Stop autorepeating if the user
     * presses a non-modifier key that doesn't autorepeat.
     */
    if ((keybd->kbdfeed->ctrl.autoRepeat) &&
	((ctrls->enabled_ctrls&(XkbSlowKeysMask|XkbRepeatKeysMask))==
							XkbRepeatKeysMask)) 
    {
#ifndef AIXV3
	if (BitIsOn(keybd->kbdfeed->ctrl.autoRepeats,key))
#endif
	{
	    xkbInfo->repeatKey = key;
	    xkbInfo->repeatKeyTimer= TimerSet(xkbInfo->repeatKeyTimer,
					0, xkbInfo->desc.ctrls->repeat_delay,
					AccessXRepeatKeyExpire, (pointer)keybd);
	}
    }
    
    /* Check for two keys being pressed at the same time.  This section
     * essentially says the following:
     *
     *	If StickyKeys is on, and a modifier is currently being held down,
     *  and one of the following is true:  the current key is not a modifier
     *  or the currentKey is a modifier, but not the only modifier being
     *  held down, turn StickyKeys off if the TwoKeys off ctrl is set.
     */
    if ((ctrls->enabled_ctrls & XkbStickyKeysMask) && 
	(xkbInfo->state.base_mods!=0)) {
	xkbControlsNotify cn;
	cn.keycode = key;
	cn.eventType = KeyPress;
	cn.requestMajor = 0;
	cn.requestMinor = 0;
	AccessXTwoKeysDown(keybd,&cn);
    }
    
    if (!ignoreKeyEvent)
	XkbProcessKeyboardEvent(xE,keybd,count);
    return ignoreKeyEvent;
} /* AccessXFilterPressEvent */

/************************************************************************/
/*									*/
/* AccessXFilterReleaseEvent						*/
/*									*/
/* Filter events before they get any further if SlowKeys is turned on.	*/
/* In addition, this routine handles the ever so popular magic key	*/
/* acts for turning various accessibility features on/off.		*/
/*									*/
/* Returns TRUE if this routine has discarded the event.		*/
/* Returns FALSE if the event needs further processing.			*/
/*									*/
/************************************************************************/
#if NeedFunctionPrototypes
Bool AccessXFilterReleaseEvent(register xEvent 		*xE, 
				      register DeviceIntPtr	keybd, 
				      int			count)
#else
Bool AccessXFilterReleaseEvent(xE,keybd,count)
    register xEvent 		*xE;
    register DeviceIntPtr	keybd;
    int				count;    
#endif
{
    XkbSrvInfoRec	*xkbInfo = keybd->key->xkbInfo;
    XkbControlsRec	*ctrls = xkbInfo->desc.ctrls;
    KeyCode		 key = xE->u.u.detail;
    Bool		 ignoreKeyEvent = FALSE;
    
    /* Don't transmit the KeyRelease if BounceKeys is on and
     * this is the release of a key that was ignored due to 
     * BounceKeys.
     */
    if (ctrls->enabled_ctrls & XkbBounceKeysMask) {
	if (!BitIsOn(keybd->key->down,key))
	    ignoreKeyEvent = TRUE;
	xkbInfo->inactiveKey= key;
	xkbInfo->bounceKeysTimer= TimerSet(xkbInfo->bounceKeysTimer, 0,
					xkbInfo->desc.ctrls->debounce_delay,
					AccessXBounceKeyExpire, (pointer)keybd);
    }

    /* Don't transmit the KeyRelease if SlowKeys is turned on and
     * the user didn't hold the key long enough.  We know we passed
     * the key if the down bit was set by CoreProcessKeyboadEvent.
     */
    if (ctrls->enabled_ctrls & XkbSlowKeysMask) {
	xkbSlowKeyNotify	ev;
	ev.keycode= key;
	ev.delay= xkbInfo->desc.ctrls->slow_keys_delay;
	if (BitIsOn(keybd->key->down,key)) {
	    ev.slowKeyState= XkbSK_Release;
	}
	else {
	    ev.slowKeyState= XkbSK_Reject;
	    ignoreKeyEvent = TRUE;
	}
	XkbSendSlowKeyNotify(keybd,&ev);
	if (xkbInfo->slowKey==key)
	    xkbInfo->slowKey= 0;
    }

    /* Stop Repeating if the user releases the key that is currently
     * repeating.
     */
    if (xkbInfo->repeatKey==key) {
	xkbInfo->repeatKey= 0;
    }

    if ((ctrls->enabled_ctrls&XkbAccessXTimeoutMask)&&
		(ctrls->accessx_timeout>0)&&
		(ctrls->enabled_ctrls&ctrls->accessx_timeout_mask)) {
	xkbInfo->lastPtrEventTime= 0;
	xkbInfo->krgTimer= TimerSet(xkbInfo->krgTimer, 0, 
					ctrls->accessx_timeout*1000,
					AccessXTimeoutExpire, (pointer)keybd);
	xkbInfo->krgTimerActive= _TIMEOUT_TIMER;
    }
    else if (xkbInfo->krgTimerActive) {
	xkbInfo->krgTimer= TimerSet(xkbInfo->krgTimer, 0, 0, NULL, NULL);
	xkbInfo->krgTimerActive= _OFF_TIMER;
    }
	
    /* Keep track of how many times the Shift key has been pressed.
     * If it has been pressed and released 5 times in a row, toggle
     * the state of StickyKeys.
     */
    if ((!ignoreKeyEvent)&&(xkbInfo->shiftKeyCount)) {
	KeySym *pSym= XkbKeySymsPtr(&xkbInfo->desc,key);
	if ((pSym[0]!=XK_Shift_L)&&(pSym[0]!=XK_Shift_R)) {
	    xkbInfo->shiftKeyCount= 0;
	}
	else if (xkbInfo->shiftKeyCount>=5) {
	     xkbControlsNotify cn;
	     cn.keycode = key;
	     cn.eventType = KeyPress;
	     cn.requestMajor = 0;
	     cn.requestMinor = 0;
	     if (ctrls->enabled_ctrls & XkbStickyKeysMask)
		AccessXStickyKeysTurnOff(keybd,&cn);
	     else
		AccessXStickyKeysTurnOn(keybd,&cn);
	     xkbInfo->shiftKeyCount= 0;
	}
    }
    
    if (!ignoreKeyEvent)
	XkbProcessKeyboardEvent(xE,keybd,count);
    return ignoreKeyEvent;
    
} /* AccessXFilterReleaseEvent */

/************************************************************************/
/*									*/
/* ProcessPointerEvent							*/
/*									*/
/* This routine merely sets the shiftKeyCount and clears the keyboard   */
/* response group timer (if necessary) on a mouse event.  This is so	*/
/* multiple shifts with just the mouse and shift-drags with the mouse	*/
/* don't accidentally turn on StickyKeys or the Keyboard Response Group.*/
/*									*/
/************************************************************************/
#if NeedFunctionPrototypes
void ProcessPointerEvent(register xEvent 	*xE, 
			 register DeviceIntPtr	mouse, 
			 int		        count)
#else
void ProcessPointerEvent(xE,mouse,count)
    register xEvent 		*xE;
    register DeviceIntPtr	mouse;
    int		        	count;
#endif
{
    DeviceIntPtr ptr = (DeviceIntPtr)LookupPointerDevice();
    DeviceIntPtr keybd = (DeviceIntPtr)LookupKeyboardDevice();
    XkbSrvInfoRec	*xkbInfo = keybd->key->xkbInfo;

    xkbInfo->shiftKeyCount = 0;
    if (xkbInfo->krgTimerActive==_KRG_TIMER) {
	xkbInfo->krgTimer= TimerSet(xkbInfo->krgTimer, 0,
				xkbInfo->desc.ctrls->accessx_timeout*1000,
				AccessXTimeoutExpire, (pointer)keybd);
	xkbInfo->krgTimerActive== _OFF_TIMER;
    }
    xkbInfo->lastPtrEventTime= xE->u.keyButtonPointer.time;
    if (xE->u.u.type==ButtonPress) {
#ifdef XTEST_BOGOSITY
	if (ptr->button->down[xE->u.u.detail>>3]&(1<<(xE->u.u.detail&0x7)))
	    return;
#endif
    }
    else if (xE->u.u.type==ButtonRelease) {
#ifdef XTEST_BOGOSITY
	if ((ptr->button->down[xE->u.u.detail>>3]&(1<<(xE->u.u.detail&0x7)))==0)
	    return;
#endif

	xkbInfo->lockedPtrButtons&= ~(1<<(xE->u.u.detail&0x7));
    }
    CoreProcessPointerEvent(xE,mouse,count);

    /* clear any latched modifiers */
    if ( xkbInfo->state.latched_mods && (xE->u.u.type==ButtonRelease) ) {
	unsigned changed;
	XkbStateRec oldState;

	oldState= xkbInfo->state;
	xkbInfo->state.latched_mods= 0;

	XkbComputeDerivedState(xkbInfo);
	changed = XkbStateChangedFlags(&oldState,&xkbInfo->state);
	if (changed) {
	    xkbStateNotify	sn;
	    sn.keycode= xE->u.u.detail;
	    sn.eventType= xE->u.u.type;
	    sn.requestMajor = sn.requestMinor = 0;
	    sn.changed= changed;
	    XkbSendStateNotify(keybd,&sn);
	}
	if (changed&xkbInfo->iAccel.usedComponents) {
	    changed= XkbIndicatorsToUpdate(keybd,changed);
	    if (changed)
		    XkbUpdateIndicators(keybd,changed,NULL);
	}
	keybd->key->state= xkbInfo->lookupState&0xE0FF;
    }
} /* ProcessPointerEvent */

