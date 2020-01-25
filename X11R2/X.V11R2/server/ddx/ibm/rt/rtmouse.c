/***********************************************************
		Copyright IBM Corporation 1987

                      All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the name of IBM not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.  

IBM DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
IBM BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/
/* $Header:rtmouse.c 6.0$ */
/* $ACIS:rtmouse.c 6.0$ */
/* $Source: /vice/X11/src/server/ddx/ibm/rt/RCS/rtmouse.c,v $ */

#ifndef lint
static char *rcsid = "$Header:rtmouse.c 6.0$";
#endif

#include <stdio.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/tbioctl.h>
#include <machineio/mouseio.h>
#include <machinecons/xio.h>

#include "X.h"
#include "Xproto.h"
#include "miscstruct.h"
#include "scrnintstr.h"
#include "input.h"
#include "cursorstr.h"

#include "rtio.h"
#include "rtmouse.h"
#include "rtscreen.h"
#include "rtutils.h"

extern	char	*getenv();

DevicePtr	rtPtr;

/***================================================================***/

/*
 * Initialize mouse 
 */

static
rtMouseInit(MouseName)
char	*MouseName;
{
	char *MouseType = getenv("MOUSETYPE");
	struct sgttyb MouseSG;
	int ldisc, ioarg;
	int mdev = -1;

	TRACE(( "INITNAME()\n"));

	/*
	 * Open mouse 
	 */

	if ((mdev = open (MouseName, O_RDWR|O_NDELAY)) < 0) {
		fprintf  (stderr, "Error in open of (%s)\n", MouseName);
		fflush(stderr);
		exit (2);
	}

	/*
	 * Switch to mouse line discipline
	 */

	ldisc = TABLDISC;
	ioctl(mdev, TIOCSETD, (caddr_t) &ldisc);

	/*
	 * Check for Mouse Systems Mouse
	 */

	if (MouseType && (strcmp (MouseType, "MSCMOUSE") == 0)) {
		MouseSG.sg_ispeed = 9;
		MouseSG.sg_ospeed = 9;
		MouseSG.sg_erase = -1;
		MouseSG.sg_kill = -1;
		MouseSG.sg_flags = RAW | ANYP;
		ioctl(mdev, TIOCSETP, (caddr_t) &MouseSG);

		/*
		 * Set to MSCmouse emulation
		 */

		ldisc = PCMS_DISC;
		if (ioctl (mdev, TBIOSETD, (caddr_t) &ldisc) < 0)
		    ErrorF("Error in setting PCMS_DISC Line Discipline");

	} else {	/* Assume the Planar Mouse */

		/*
		 * Use 3 button emulation
		 */

		ldisc = PLANMS_DISC3;
		if (ioctl (mdev, TBIOSETD, (caddr_t) &ldisc) < 0)
		   ErrorF("Error in setting PLANMS_DISC3 Line Discipline");

		/*
		 * Set default mouse sample rate
		 */

		ioarg = MS_RATE_40;
		if (ioctl (mdev, MSIC_SAMP, (caddr_t) &ioarg) < 0)
			ErrorF("Error in setting mouse sample rate");

		/*
		 * Set default mouse resolution
		 */

		ioarg = MS_RES_200;
		if (ioctl (mdev, MSIC_RESL, (caddr_t) &ioarg) < 0)
			ErrorF("Error in setting mouse resolution");
	}
	 
}

/***================================================================***/

static void
rtChangePointerControl(pDevice,pCtrl)
DevicePtr	pDevice;
PtrCtrl		*pCtrl;
{
int	scale;

    TRACE(("rtChangePointerControl( pDevice= 0x%x, pCtrl= 0x%x )\n",
						pDevice,pCtrl));
    scale= pCtrl->num/pCtrl->den;
    if (scale<1) scale= 1;
    rtXaddr->mscale= scale;
    rtXaddr->mthreshold= pCtrl->threshold;
}

static int
rtGetMotionEvents(buff, start, stop)
    CARD32 start, stop;
    xTimecoord *buff;
{
    TRACE(("rtGetMotionEvents( buff= 0x%x, start= %d, stop= %d )\n",
							buff,start,stop));
    return 0;
}

/***================================================================***/

int
rtMouseProc(pDev, onoff)
    DevicePtr	pDev;
    int onoff;
{
    static int been_here= FALSE;
    BYTE map[4];

    TRACE(("rtMouseProc( pDev= 0x%x, onoff= 0x%x )\n",pDev, onoff ));

    switch (onoff)
    {
    case DEVICE_INIT: 
	    rtPtr = pDev;
	    pDev->devicePrivate = (pointer) &rtQueue;
	    map[1] = 1;
	    map[2] = 2;
	    map[3] = 3;
	    if (!been_here) {
	       rtMouseInit(rtScreenPointer(0));
	       been_here= TRUE;
	    }
	    InitPointerDeviceStruct(
		rtPtr, map, 3, rtGetMotionEvents, rtChangePointerControl );
	    SetInputCheck( &rtQueue->head, rtQueue->tail);
	    break;
    case DEVICE_ON:
	    pDev->on = TRUE;
	    AddEnabledDevice(rtEmulatorFD);
	    break;
    case DEVICE_OFF:
	pDev->on = FALSE;
/* (ef) 2/8/87 -- see comment above */
/*	RemoveEnabledDevice(rtEmulatorFD);*/
	break;
    case DEVICE_CLOSE:
	break;
    }
    return Success;
}

