/* $XConsortium: macIIKbd.c,v 1.24 94/04/17 20:29:29 dpw Exp $ */
/*****************************************************************************
Copyright 1988-1993 by Apple Computer, Inc, Cupertino, California
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

*****************************************************************************/
/*-
 * macIIKbd.c --
 *	Functions for retrieving data from a keyboard.
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
#include "macII.h"
#include <stdio.h>
#include "Xproto.h"
#include "inputstr.h"

extern CARD8 *macIIModMap[];
extern KeySymsRec macIIKeySyms[];
extern struct timeval lastEventTimeTv;

static void 	  macIIBell();
static void 	  macIIKbdCtrl();
void 	  	  macIIKbdEnqueueEvent();
static int	  autoRepeatKeyDown = 0;
static int  	  autoRepeatDebug = 0;
static int	  autoRepeatReady;
static int	  autoRepeatFirst;
static struct timeval autoRepeatLastKeyDownTv;
static struct timeval autoRepeatDeltaTv;
static KeybdCtrl  sysKbCtrl;

static KbPrivRec  	sysKbPriv = {
    -1,				/* Type of keyboard */
    macIIKbdEnqueueEvent,	/* Function to process an event */
    0,				/* offset for device keycodes */
    &sysKbCtrl,                 /* Initial full duration = .20 sec. */
};

extern int consoleFd;
int devosmFd = 0;
int noOsm = FALSE;

#ifdef XTESTEXT1
#define XTestSERVER_SIDE
#include "xtestext1.h"
/*
 * defined in xtestext1di.c
 */
extern int	on_steal_input;
extern Bool	XTestStealKeyData();
extern short	xtest_mousex, xtest_mousey;
#endif /* XTESTEXT1 */

/*-
 *-----------------------------------------------------------------------
 * macIIKbdProc --
 *	Handle the initialization, etc. of a keyboard.
 *
 * Results:
 *	None.
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
macIIKbdProc (pKeyboard, what)
    DevicePtr	  pKeyboard;	/* Keyboard to manipulate */
    int	    	  what;	    	/* What to do to it */
{
    switch (what) {
	case DEVICE_INIT:
	    if (pKeyboard != LookupKeyboardDevice()) {
		ErrorF ("Cannot open non-system keyboard. \r\n");
		return (!Success);
	    }
	    if (consoleFd == 0) {
		if ((consoleFd = open("/dev/console", O_RDWR|O_NDELAY)) < 0) {
			FatalError("Could not open /dev/console. \r\n");
			return (!Success);
		}
            }
	    if (devosmFd == 0 && !noOsm) {
		if ((devosmFd = open("/dev/osm", O_RDONLY)) < 0) {
			MessageF("Could not open /dev/osm. \r\n");
		}
            }

	    /*
	     * Perform final initialization of the system private keyboard
	     * structure and fill in various slots in the device record
	     * itself which couldn't be filled in before.
	     */
	    pKeyboard->devicePrivate = (pointer)&sysKbPriv;
	    pKeyboard->on = FALSE;
	    sysKbCtrl = defaultKeyboardControl;
	    sysKbPriv.ctrl = &sysKbCtrl;

	    /*
	     * ensure that the keycodes on the wire are >= MIN_KEYCODE
	     */

	    sysKbPriv.type = KBTYPE_MACII;  

	    if (macIIKeySyms[sysKbPriv.type].minKeyCode < MIN_KEYCODE) {
		int offset = MIN_KEYCODE -macIIKeySyms[sysKbPriv.type].minKeyCode;

		macIIKeySyms[sysKbPriv.type].minKeyCode += offset;
		macIIKeySyms[sysKbPriv.type].maxKeyCode += offset;
		sysKbPriv.offset = offset;
	    }
	    InitKeyboardDeviceStruct(
		    pKeyboard,
		    &(macIIKeySyms[sysKbPriv.type]),
		    (macIIModMap[sysKbPriv.type]),
		    macIIBell,
		    macIIKbdCtrl);
	    break;

	case DEVICE_ON:
	    macIIKbdSetUp(consoleFd, TRUE);

	    AddEnabledDevice(consoleFd);

	    pKeyboard->on = TRUE;
	    break;

	case DEVICE_CLOSE:
	    macIIKbdSetUp(consoleFd, FALSE);

	    RemoveEnabledDevice(consoleFd);

	    pKeyboard->on = FALSE;

	    if (devosmFd > 0) close(devosmFd);
	    close(consoleFd);
	    devosmFd = 0;
	    consoleFd = 0;
	    break;

	case DEVICE_OFF:
	    macIIKbdSetUp(consoleFd, FALSE);

	    RemoveEnabledDevice(consoleFd);

	    pKeyboard->on = FALSE;
	    break;

    }
    return (Success);
}

#include <sys/termio.h>

int
macIIKbdSetUp(fd, openClose)
    int		fd;
    Bool	openClose;
{
    struct strioctl ctl;
    struct termio tio;
    static struct termio save_tio;
    static struct ltchars tc;
    char buff[FMNAMESZ+1];
    int iarg;

    if (openClose) {

	if (ioctl(fd, TCGETA, &save_tio) < 0) {
		FatalError("Failed to ioctl TCGETA.\r\n");
		return (!Success);
	}
        if (ioctl(fd, TIOCGLTC, &tc) < 0) {
		FatalError("Failed to ioctl TIOCGLTC.\r\n");
		return (!Success);
        }

	/* 
	 * Pop all streams modules off /dev/console. Someday we
	 * will remember which ones and restore them on exit.
	 */
	errno = 0;
	ioctl(fd, I_LOOK, buff);
	while (errno != EINVAL) {
		if(ioctl(fd, I_POP, 0) < 0) {
			FatalError("Failed to ioctl I_POP %s.\r\n", buff);
			return (!Success);
		}
		ioctl(fd, I_LOOK, buff);
	}
		
	iarg = 1;
	if (ioctl(fd, FIONBIO, &iarg) < 0) {
		FatalError("Could not ioctl FIONBIO. \r\n");
		return (!Success);
	}

	iarg = 1;
	if (fcntl (fd, F_SETOWN, getpid()) < 0)
	{
		FatalError("Could not fcntl F_SETOWN \r\n");
		return !Success;
	}

	if (ioctl(fd, FIOASYNC, &iarg) < 0) {
		FatalError("Could not ioctl FIOASYNC. \r\n");
		return (!Success);
	}

	tio.c_iflag = (IGNPAR|IGNBRK) & (~PARMRK) & (~ISTRIP);
	tio.c_oflag = 0;
	tio.c_cflag = CREAD|CS8|B9600;
	tio.c_lflag = 0;
	if (ioctl(fd, TCSETA, &tio) < 0) {
		FatalError("Failed to ioctl TCSETA.\r\n");
		return (!Success);
	}

	ctl.ic_len = 0;
	ctl.ic_cmd = VIDEO_RAW;
	if (ioctl(fd, I_STR, &ctl) < 0) {
		FatalError("Failed to ioctl I_STR VIDEO_RAW.\r\n");
		return(!Success);
	}

	ctl.ic_len = 0;
	ctl.ic_cmd = VIDEO_MOUSE;
	if (ioctl(fd, I_STR, &ctl) < 0) {

#define MSG "Failed to ioctl I_STR VIDEO_MOUSE.\r\n Run Xrepair and try again.\r\n"
		FatalError(MSG);
#undef MSG
		return(!Success);
	}

	if (ioctl(fd, I_FLUSH, FLUSHRW) < 0) {
		FatalError("Failed to ioctl I_FLUSH FLUSHRW.\r\n");
		return (!Success);
	}

#ifdef CONS_REDIRECT
	ctl.ic_len = 0;
	ctl.ic_cmd = CONS_REDIRECT;
	if (ioctl(fd, I_STR, &ctl) < 0) {
		/*
		 * Not fatal! Convenience for A/UX 1.1 and later.
		 */
		MessageF("Unable to ioctl I_STR CONS_REDIRECT.\r\n");
	}
#endif

    } else {
#ifdef CONS_UNDIRECT
	ctl.ic_len = 0;
	ctl.ic_cmd = CONS_UNDIRECT;
	if (ioctl(fd, I_STR, &ctl) < 0) {
		MessageF("Failed to ioctl I_STR CONS_UNDIRECT.\r\n");
	}
#endif

	iarg = 0;
	if (ioctl(fd, FIONBIO, &iarg) < 0) {
		ErrorF("Could not ioctl FIONBIO. \r\n");
	}

	iarg = 0;
	if (ioctl(fd, FIOASYNC, &iarg) < 0) {
		ErrorF("Could not ioctl FIOASYNC. \r\n");
	}

	if (ioctl(fd, I_FLUSH, FLUSHRW) < 0) {
		MessageF("Failed to ioctl I_FLUSH FLUSHRW.\r\n");
	}
    
	ctl.ic_len = 0;
	ctl.ic_cmd = VIDEO_NOMOUSE;
	if (ioctl(fd, I_STR, &ctl) < 0) {
		MessageF("Failed to ioctl I_STR VIDEO_NOMOUSE.\r\n");
	}

        ctl.ic_len = 0;
        ctl.ic_cmd = VIDEO_MAC;
        if (ioctl(fd, I_STR, &ctl) < 0) {
            ctl.ic_len = 0;
            ctl.ic_cmd = VIDEO_ASCII; /* A/UX 1.* */
            if (ioctl(fd, I_STR, &ctl) < 0) {
		MessageF("Failed to ioctl I_STR VIDEO_MAC VIDEO_ASCII.\r\n");
            }
        }

	if(ioctl(fd, I_PUSH, "line") < 0) {
		MessageF("Failed to ioctl I_PUSH.\r\n");
	}

	if (ioctl(fd, TCSETA, &save_tio) < 0) {
		MessageF("Failed to ioctl TCSETA.\r\n");
	}
        if (ioctl(fd, TIOCSLTC, &tc) < 0) {
            	MessageF("Failed to ioctl TIOCSLTC.\r\n");
        }

    }
}

#include <sys/utsname.h>
#include <sys/sm_aux.h>

/*-
 *-----------------------------------------------------------------------
 * macIIBell --
 *	Ring the terminal/keyboard bell
 *
 * Results:
 *    Ring the keyboard bell for an amount of time proportional to
 *    "loudness."
 *
 * Side Effects:
 *	None, really...
 *
 *-----------------------------------------------------------------------
 */
static void
macIIBell (loudness, pKeyboard)
    int	    	  loudness;	    /* Percentage of full volume */
    DevicePtr	  pKeyboard;	    /* Keyboard to ring */
{
    KbPrivPtr   pPriv = (KbPrivPtr) pKeyboard->devicePrivate;
    struct utsname name;

    if (loudness == 0) {
      return;
    }

    if (uname(&name) != 0)
	MessageF("uname failed to ID machine type.\r\n");

    if (strcmp(name.machine, "mc68040") == 0) {
    /* This hack provides a beep on the Quadras, because the old ioctl doesn't
       work with the new sound chip. It is a horrible sounding beep, but the
       volume is adjustable. mjb */
	char vol;
	int sndFd;
	int dev, sndFile;

	vol = (char)((loudness*7)/100);
	if ((sndFd = open(DEV_SAMP, O_RDWR)) < 0) {
	    MessageF("Unable to change volume, failed to open /dev/snd/samp: %d\n", errno);
	} else {
	    ioctl(sndFd, SND_VOL, &vol);
	    close(sndFd);
	}

	if ((dev = open("/dev/snd/raw", O_RDWR|O_NDELAY)) < 0)
	    ErrorF("Failed to open raw sound device, errno = %d\r\n", errno);
	else {
	    if ((sndFile = open("/mac/lib/Resources/sampledBeep", O_RDONLY)) < 0)
		ErrorF("Failed to open beep sound file, errno = %d\r\n", errno);
	    else {
		int cnt;
		char buffer[1024];

		while ((cnt = read(sndFile, buffer, 1024)) > 0)
		    write(dev, buffer, cnt);
		close(sndFile);
	    }
	    close(dev);
	}
    } else {	/* everybody else uses this age-old ioctl */
	struct strioctl ctl;
	long countdown;

	countdown = (long)pPriv->ctrl->bell_duration * 100;
	ctl.ic_len = sizeof(long);
	ctl.ic_dp = (char *)&countdown;
	ctl.ic_cmd = VIDEO_BELL;
	if (ioctl(consoleFd, I_STR, &ctl) < 0) {
		MessageF("Failed to ioctl I_STR VIDEO_BELL.\r\n");
	}
    }
}

/*-
 *-----------------------------------------------------------------------
 * macIIKbdCtrl --
 *	Alter some of the keyboard control parameters
 *
 * Results:
 *	None.
 *
 * Side Effects:
 *	Some...
 *
 *-----------------------------------------------------------------------
 */
static void
macIIKbdCtrl (pKeyboard, ctrl)
    DevicePtr	  pKeyboard;	    /* Keyboard to alter */
    KeybdCtrl     *ctrl;
{
    KbPrivPtr   pPriv = (KbPrivPtr) pKeyboard->devicePrivate;
    *pPriv->ctrl = *ctrl;
}


/*-
 *-----------------------------------------------------------------------
 * macIIKbdEnqueueEvent
 *	Process macIIevent destined for the keyboard.
 * 	
 * Results:
 *
 * Side Effects:
 *
 *-----------------------------------------------------------------------
 */

static xEvent	autoRepeatEvent;

void
macIIKbdEnqueueEvent(pKeyboard,me)
    DeviceRec *pKeyboard;
    register unsigned char *me;
{   
    xEvent		xE;
    PtrPrivPtr	  	ptrPriv;
    KbPrivPtr           pPriv;
    int                 delta;
    BYTE		key;
    CARD16              keyModifiers;

    ptrPriv = (PtrPrivPtr) LookupPointerDevice()->devicePrivate;

    key = KEY_DETAIL(*me) + sysKbPriv.offset;
    if ((keyModifiers = ((DeviceIntPtr)pKeyboard)->key->modifierMap[key]) == 0) {
        /*
         * Kill AutoRepeater on any real Kbd event.
         */
        autoRepeatKeyDown = 0;
	if (autoRepeatDebug)
	    ErrorF("macIIKbdEnqueueEvent: autoRepeat off\r\n");
    }

    xE.u.keyButtonPointer.time = lastEventTime;
    xE.u.u.type = (KEY_UP(*me) ? KeyRelease : KeyPress);
    xE.u.u.detail = key;

    if ((xE.u.u.type == KeyPress) && (keyModifiers == 0)) {
	  /* initialize new AutoRepeater event & mark AutoRepeater on */
	if (autoRepeatDebug)
	    ErrorF("macIIKbdEnqueueEvent: KEY_DOWN\r\n");
        autoRepeatEvent = xE;
        autoRepeatFirst = TRUE;
        autoRepeatKeyDown++;
        autoRepeatLastKeyDownTv = lastEventTimeTv;
    }

#ifdef XTESTEXT1
    if (!on_steal_input ||
	XTestStealKeyData(xE.u.u.detail, xE.u.u.type, XE_KEYBOARD,
			  xtest_mousex, xtest_mousey))
#endif /* XTESTEXT1 */
    mieqEnqueue (&xE);
}

static
macIIEnqueueAutoRepeat ()
{
    int	oldmask;
    int	delta;

    if (sysKbPriv.ctrl->autoRepeat != AutoRepeatModeOn) {
	autoRepeatKeyDown = 0;
	return;
    }
    /*
     * Generate auto repeat event.	XXX one for now.
     * Update time & pointer location of saved KeyPress event.
     */
    if (autoRepeatDebug)
	ErrorF("macIIKbdProcessEvent: autoRepeatKeyDown = %d\r\n",
		autoRepeatKeyDown);

    delta = TVTOMILLI(autoRepeatDeltaTv);
    autoRepeatFirst = FALSE;

    /*
     * Fake a key up event and a key down event
     * for the last key pressed.
     */
    autoRepeatEvent.u.keyButtonPointer.time += delta;
    autoRepeatEvent.u.u.type = KeyRelease;
    oldmask = sigblock (sigmask(SIGIO));
#ifdef XTESTEXT1
    if (!on_steal_input ||
	XTestStealKeyData(autoRepeatEvent.u.u.detail, autoRepeatEvent.u.u.type,
			  XE_KEYBOARD, xtest_mousex, xtest_mousey))
#endif /* XTESTEXT1 */
    mieqEnqueue (&autoRepeatEvent);
    autoRepeatEvent.u.u.type = KeyPress;
#ifdef XTESTEXT1
    if (!on_steal_input ||
	XTestStealKeyData(autoRepeatEvent.u.u.detail, autoRepeatEvent.u.u.type,
			  XE_KEYBOARD, xtest_mousex, xtest_mousey))
#endif /* XTESTEXT1 */
    mieqEnqueue (&autoRepeatEvent);
    sigsetmask (oldmask);
    /* Update time of last key down */
    tvplus(autoRepeatLastKeyDownTv, autoRepeatLastKeyDownTv,
	   autoRepeatDeltaTv);
}

Bool
LegalModifier(key, pDev)
    unsigned int key;
    DevicePtr	pDev;
{
    return (TRUE);
}

static KeybdCtrl *pKbdCtrl = (KeybdCtrl *) 0;

#include <sys/time.h>
/*ARGSUSED*/
void
macIIBlockHandler(nscreen, pbdata, pptv, pReadmask)
    int nscreen;
    pointer pbdata;
    struct timeval **pptv;
    pointer pReadmask;
{
    static struct timeval artv = { 0, 0 };    /* autorepeat timeval */

    if (!autoRepeatKeyDown)
        return;

    if (pKbdCtrl == (KeybdCtrl *) 0)
      pKbdCtrl = ((KbPrivPtr) LookupKeyboardDevice()->devicePrivate)->ctrl;

    if (pKbdCtrl->autoRepeat != AutoRepeatModeOn)
      return;

    if (autoRepeatFirst == TRUE)
        artv.tv_usec = 1000 * AUTOREPEAT_INITIATE;
    else
        artv.tv_usec = 1000 * AUTOREPEAT_DELAY;
    *pptv = &artv;

    if (autoRepeatDebug)
        ErrorF("macIIBlockHandler(%d,%d): \r\n", artv.tv_sec, artv.tv_usec);

}

/*ARGSUSED*/
void
macIIWakeupHandler(nscreen, pbdata, err, pReadmask)
    int nscreen;
    pointer pbdata;
    unsigned long err;
    pointer pReadmask;
{
    long now;

    if (autoRepeatDebug)
        ErrorF("macIIWakeupHandler(ar=%d, err=%d):\r\n", autoRepeatKeyDown, err);

    if (pKbdCtrl == (KeybdCtrl *) 0)
      pKbdCtrl = ((KbPrivPtr) LookupKeyboardDevice()->devicePrivate)->ctrl;
    
    if (pKbdCtrl->autoRepeat != AutoRepeatModeOn)
      return;

    if (autoRepeatKeyDown) {
	struct timeval tv;

	gettimeofday (&tv, (struct timezone *)0);
	tvminus(autoRepeatDeltaTv, tv, autoRepeatLastKeyDownTv);
	if (autoRepeatDeltaTv.tv_sec > 0 ||
			(!autoRepeatFirst && autoRepeatDeltaTv.tv_usec >
				(AUTOREPEAT_DELAY * 1000)) ||
			(autoRepeatDeltaTv.tv_usec >
				(AUTOREPEAT_INITIATE * 1000)))
                autoRepeatReady++;
    }

    if (autoRepeatReady)
    {
        macIIEnqueueAutoRepeat();
	autoRepeatReady = 0;
    }

}

#ifdef XTESTEXT1
void
XTestGenerateEvent(dev_type, keycode, keystate, mousex, mousey)
    int dev_type;
    int keycode;
    int keystate;
    int mousex;
    int mousey;
{
    xEvent tevent;

    tevent.u.u.type = (dev_type == XE_POINTER) ?
	(keystate == XTestKEY_UP) ? ButtonRelease : ButtonPress :
	(keystate == XTestKEY_UP) ? KeyRelease : KeyPress;
    tevent.u.u.detail = keycode;
    tevent.u.keyButtonPointer.rootX = mousex;
    tevent.u.keyButtonPointer.rootY = mousey;
    SetTimeSinceLastInputEvent();
    tevent.u.keyButtonPointer.time = lastEventTime;
    mieqEnqueue(&tevent);
    ProcessInputEvents();
}
#endif
