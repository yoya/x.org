/**************************************************************** 
Copyright 1988,1993 by Apple Computer, Inc, Cupertino, California
			All Rights Reserved

Permission to use, copy, modify, and distribute this software
for any purpose and without fee is hereby granted, provided
that the above copyright notice appear in all copies.

APPLE MAKES NO WARRANTY OR REPRESENTATION, EITHER EXPRESS,
OR IMPLIED, WITH RESPECT TO THIS SOFTWARE, ITS QUALITY,
PERFORMANCE, MERCHANABILITY, OR FITNESS FOR A PARTICULAR
PURPOSE. AS A RESULT, THIS SOFTWARE IS PROVIDED "AS IS,"
AND YOU THE USER ARE ASSUMING THE ENTIRE RISK AS TO ITS
QUALITY AND PERFORMANCE. IN NO EVENT WILL APPLE BE LIABLE 
FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
DAMAGES RESULTING FROM ANY DEFECT IN THE SOFTWARE.

THE WARRANTY AND REMEDIES SET FORTH ABOVE ARE EXCLUSIVE
AND IN LIEU OF ALL OTHERS, ORAL OR WRITTEN, EXPRESS OR
IMPLIED.

****************************************************************/
/*-
 * macIIMouse.c --
 *	Functions for playing cat and mouse... sorry.
 *
 * Copyright (c) 1987 by the Regents of the University of California
 *
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appear in all copies.  The University of California
 * makes no representations about the suitability of this
 * software for any purpose.  It is provided "as is" without
 * express or implied warranty.
 *
 *
 */

/************************************************************
Copyright 1987 by Sun Microsystems, Inc. Mountain View, CA.

                    All Rights Reserved

Permission  to  use,  copy,  modify,  and  distribute   this
software  and  its documentation for any purpose and without
fee is hereby granted, provided that the above copyright no-
tice  appear  in all copies and that both that copyright no-
tice and this permission notice appear in  supporting  docu-
mentation,  and  that the names of Sun or X Consortium
not be used in advertising or publicity pertaining to 
distribution  of  the software  without specific prior 
written permission. Sun and X Consortium make no 
representations about the suitability of this software for 
any purpose. It is provided "as is" without any express or 
implied warranty.

SUN DISCLAIMS ALL WARRANTIES WITH REGARD TO  THIS  SOFTWARE,
INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FIT-
NESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL SUN BE  LI-
ABLE  FOR  ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,  DATA  OR
PROFITS,  WHETHER  IN  AN  ACTION OF CONTRACT, NEGLIGENCE OR
OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION  WITH
THE USE OR PERFORMANCE OF THIS SOFTWARE.

********************************************************/

#define NEED_EVENTS
#include    "macII.h"
#include	"mipointer.h"

#ifdef XTESTEXT1
extern short xtest_mousex;
extern short xtest_mousey;
extern int   on_steal_input;
extern Bool  XTestStealKeyData();
extern void  XTestStealMotionData();
#endif

Bool ActiveZaphod = TRUE;

static Bool macIICursorOffScreen();
static void macIICrossScreen();
static void macIIWarpCursor();

miPointerScreenFuncRec macIIPointerCursorFuncs = {
    macIICursorOffScreen,
    macIICrossScreen,
    macIIWarpCursor
};

static void 	  	macIIMouseCtrl();
static void 	  	macIIMouseEnqueueEvent();

static PtrPrivRec 	sysMousePriv = {
    macIIMouseEnqueueEvent,	/* Function to process an event */
    NULL			/* Screen pointer is on */
};

extern BoxRec     currentLimits;
/*-
 *-----------------------------------------------------------------------
 * macIIMouseProc --
 *	Handle the initialization, etc. of a mouse
 *
 * Results:
 *	none.
 *
 * Side Effects:
 *
 * Note:
 *	When using macII, all input comes off a single fd, stored in the
 *	global consoleFd.  Therefore, only one device should be enabled and
 *	disabled, even though the application still sees both mouse and
 *	keyboard.  We have arbitrarily chosen to enable and disable consoleFd
 *	in the keyboard routine macIIKbdProc rather than in macIIMouseProc.
 *
 *-----------------------------------------------------------------------
 */
int
macIIMouseProc (pMouse, what)
    DevicePtr	  pMouse;   	/* Mouse to play with */
    int	    	  what;	    	/* What to do with it */
{
    BYTE    	  map[4];

    switch (what) {
	case DEVICE_INIT:
	    if (pMouse != LookupPointerDevice()) {
		ErrorF ("Cannot open non-system mouse");	
		return (!Success);
	    }

	    sysMousePriv.pScreen = screenInfo.screens[0];

	    pMouse->devicePrivate = (pointer) &sysMousePriv;
	    pMouse->on = FALSE;
	    map[1] = 1;
	    map[2] = 2;
	    map[3] = 3;
	    InitPointerDeviceStruct(
		pMouse, map, 3, miPointerGetMotionEvents, macIIMouseCtrl, 
				    miPointerGetMotionBufferSize());
	    break;

	case DEVICE_ON:
	    pMouse->on = TRUE;
	    break;

	case DEVICE_CLOSE:
	    break;

	case DEVICE_OFF:
	    pMouse->on = FALSE;
	    break;
    }
    return (Success);
}
	    
/*-
 *-----------------------------------------------------------------------
 * macIIMouseCtrl --
 *	Alter the control parameters for the mouse. Since acceleration
 *	etc. is done from the PtrCtrl record in the mouse's device record,
 *	there's nothing to do here.
 *
 * Results:
 *	None.
 *
 * Side Effects:
 *	None.
 *
 *-----------------------------------------------------------------------
 */
static void
macIIMouseCtrl (pMouse)
    DevicePtr	  pMouse;
{
}

/*-
 *-----------------------------------------------------------------------
 * MouseAccelerate --
 *	Given a delta and a mouse, return the acceleration of the delta.
 *
 * Results:
 *	The corrected delta
 *
 * Side Effects:
 *	None.
 *
 *-----------------------------------------------------------------------
 */
static short
MouseAccelerate (pMouse, delta)
    DevicePtr	  pMouse;
    int	    	  delta;
{
    register int  sgn = sign(delta);
    register PtrCtrl *pCtrl;

    delta = abs(delta);
    pCtrl = &((DeviceIntPtr) pMouse)->ptrfeed->ctrl;

    if (delta > pCtrl->threshold) {
	return ((short) (sgn * (pCtrl->threshold +
				((delta - pCtrl->threshold) * pCtrl->num) /
				pCtrl->den)));
    } else {
	return ((short) (sgn * delta));
    }
}

static void
macIIMouseEnqueueEvent(pMouse,me)
    DeviceRec *pMouse;
    register unsigned char *me;
{   
    xEvent		xE;
    GrabPtr     	grab = ((DeviceIntPtr)pMouse)->grab;
    register PtrPrivPtr	pPriv;	/* Private data for pointer */
    unsigned long time;
    short xpos, ypos; /* SIGNED shorts for valid comparisons */
    static short lastx, lasty;
    static unsigned char last_button = 0x80;
    short dx, dy;

    pPriv = (PtrPrivPtr)pMouse->devicePrivate;

    time = xE.u.keyButtonPointer.time = lastEventTime;

    if (IS_MIDDLE_KEY(*me)) {
	    static int pseudo_middle_state = ButtonRelease;

            xE.u.u.detail = MS_MIDDLE - MS_LEFT + 1;
	    xE.u.u.type = (KEY_UP(*me) ? ButtonRelease : ButtonPress);

#ifdef OPTION_KEY_MOUSE
	    /*
	     * Apple extended keyboard under A/UX produces two release events
	     * each time the option key is released. The following causes second
	     * and subsequent release events to be ignored.
	     */
	    if (xE.u.u.type == pseudo_middle_state) return;
	    else pseudo_middle_state = xE.u.u.type;
#endif OPTION_KEY_MOUSE

#ifdef XTESTEXT1
	    if (!on_steal_input || 
		XTestStealKeyData(xE.u.u.detail, xE.u.u.type, XE_POINTER,
				  xtest_mousex, xtest_mousey))
#endif /* XTESTEXT1 */
	    mieqEnqueue (&xE);
	    return;
	
    }
    if (IS_RIGHT_KEY(*me)) {
            xE.u.u.detail = MS_RIGHT - MS_LEFT + 1;
	    xE.u.u.type = (KEY_UP(*me) ? ButtonRelease : ButtonPress);
#ifdef XTESTEXT1
	    if (!on_steal_input || 
		XTestStealKeyData(xE.u.u.detail, xE.u.u.type, XE_POINTER,
				  xtest_mousex, xtest_mousey))
#endif /* XTESTEXT1 */
	    mieqEnqueue (&xE);
	    return;
	
    }
    /*
     * When we detect a change in the mouse coordinates, we call
     * the cursor module to move the cursor. It has the option of
     * simply removing the cursor or just shifting it a bit.
     * If it is removed, DIX will restore it before we goes to sleep...
     *
     * What should be done if it goes off the screen? Move to another
     * screen? For now, we just force the pointer to stay on the
     * screen...
     */
    xpos = *(me + 2) & 0x7f; /* DELTA: low 7 bits */
    if (xpos & 0x0040) xpos = xpos - 0x0080; /* 2's complement */
    ypos = *(me + 1) & 0x7f;
    if (ypos & 0x0040) ypos = ypos - 0x0080;
    dx = MouseAccelerate(pMouse, xpos);
    dy = MouseAccelerate(pMouse, ypos);
#ifdef XTESTEXT1
    if (on_steal_input)
	XTestStealMotionData(dx, dy, XE_POINTER, xtest_mousex, xtest_mousey);
#endif
    miPointerDeltaCursor (dx, dy, time);

    if (KEY_UP(*(me + 1)) != last_button) {
	xE.u.u.detail = (MS_LEFT - MS_LEFT) + 1;
	xE.u.u.type = (KEY_UP(*(me + 1)) ? ButtonRelease : ButtonPress);
	last_button = KEY_UP(*(me + 1));
#ifdef XTESTEXT1
	    if (!on_steal_input || 
		XTestStealKeyData(xE.u.u.detail, xE.u.u.type, XE_POINTER,
				  xtest_mousex, xtest_mousey))
#endif /* XTESTEXT1 */
	mieqEnqueue (&xE);
    }
}
   
/*ARGSUSED*/
static Bool
macIICursorOffScreen (pScreen, x, y)
    ScreenPtr   *pScreen;
    int         *x, *y;
{
    int     index;

    /*
     * Active Zaphod implementation:
     *    increment or decrement the current screen
     *    if the x is to the right or the left of
     *    the current screen.
     */
    if (ActiveZaphod &&
        screenInfo.numScreens > 1 && (*x >= (*pScreen)->width || *x < 0))
    {
        index = (*pScreen)->myNum;
        if (*x < 0)
        {
            index = (index ? index : screenInfo.numScreens) - 1;
            *pScreen = screenInfo.screens[index];
            *x += (*pScreen)->width;
        }
        else
        {
            *x -= (*pScreen)->width;
            index = (index + 1) % screenInfo.numScreens;
            *pScreen = screenInfo.screens[index];
        }
        return TRUE;
    }
    return FALSE;
}

static void
macIICrossScreen (pScreen, entering)
    ScreenPtr   pScreen;
    Bool        entering;
{
}

static void
macIIWarpCursor (pScreen, x, y)
    ScreenPtr	pScreen;
    int		x, y;
{
    int		oldmask;

    oldmask = sigblock (sigmask(SIGIO));
    miPointerWarpCursor (pScreen, x, y);
    sigsetmask (oldmask);
#ifdef XTESTEXT1
    if (on_steal_input)
	XTestStealMotionData(x-xtest_mousex,y-xtest_mousey,XE_POINTER,
			     xtest_mousex,xtest_mousey);
#endif
}

#ifdef XTESTEXT1
void
XTestGetPointerPos(fmousex, fmousey)
    short *fmousex;
    short *fmousey;
{
    int x,y;

    miPointerPosition(&x, &y);
    *fmousex = x;
    *fmousey = y;
}

void
XTestJumpPointer(jx, jy, dev_type)
    int jx;
    int jy;
    int dev_type;
{
    int x, y;
    struct timeval now;

    gettimeofday (&now, (struct timezone *)0);
    miPointerAbsoluteCursor(jx, jy, TVTOMILLI(now));
    miPointerPosition (&x, &y);
    ProcessInputEvents();
}
#endif
