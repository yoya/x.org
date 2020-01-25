/*
 * $XConsortium: omronMouse.c,v 1.3 94/01/11 20:48:51 rob Exp $
 *
 * Copyright 1992, 1993 Data General Corporation;
 * Copyright 1991, 1992, 1993 OMRON Corporation  
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting documentation, and
 * that neither the name OMRON or DATA GENERAL be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission of the party whose name is to be used.  Neither 
 * OMRON or DATA GENERAL make any representation about the suitability of this
 * software for any purpose.  It is provided "as is" without express or 
 * implied warranty.  
 *
 * OMRON AND DATA GENERAL EACH DISCLAIM ALL WARRANTIES WITH REGARD TO THIS
 * SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS,
 * IN NO EVENT SHALL OMRON OR DATA GENERAL BE LIABLE FOR ANY SPECIAL, INDIRECT
 * OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
 * USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
 * OF THIS SOFTWARE.
 *
 */

#include "omron.h"
#include "omronMouse.h"

static Bool omronMouseInit();
static void omronCrossScreen();
static void omronWarpCursor();
static void omronMouseControl(); 
static Bool omronCursorOffScreen();


miPointerScreenFuncRec omronPointerScreenFuncs = {
	omronCursorOffScreen,
	omronCrossScreen,
	omronWarpCursor,
};

int
omronMouseProc(pMouse,what)
DevicePtr 	pMouse;
int		what;
{
#ifndef XTHREADS
	static	Bool initFlag = FALSE;
#else /* XTHREADS */
	static  char	DITNAME[] = "DIT";
#endif /* XTHREADS */
	static	omronMousePrv	prv[1];
	BYTE    map[5];

	switch(what) {
	case DEVICE_INIT:
		pMouse->devicePrivate = (pointer)prv;
#ifndef XTHREADS
		if(initFlag == FALSE)
#endif /* XTHREADS */
		{
			if (!omronMouseInit(prv))
				return (!Success);
#ifdef uniosu
			if (!omronResetTty(prv))
				return (!Success);
#endif
#ifndef XTHREADS
			initFlag = TRUE;
#endif /* XTHREADS */
		}
		prv->button_state = 7;
		map[1] = Button1;
		map[2] = Button2;
		map[3] = Button3;
		InitPointerDeviceStruct( pMouse,map, 3, miPointerGetMotionEvents,
				omronMouseControl, miPointerGetMotionBufferSize());
#ifndef UNUSE_DRV_TIME
		omronSetDriverTimeMode(pMouse, NULL);
#endif
		break;	
	case DEVICE_ON:
#ifndef XTHREADS
#ifndef UNUSE_SIGIO_SIGNAL
		prv->ctl_flags |= FASYNC;
		if (fcntl(prv->fd, F_SETFL, prv->ctl_flags) < 0)
		{
			Error("Can't enable the mouse SIGIO.");
			return (!Success);
		}
#endif
#endif /* XTHREADS */
		AddEnabledDevice (prv->fd);
		pMouse->on = TRUE;
		break;
	case DEVICE_OFF:
	case DEVICE_CLOSE:
#ifndef XTHREADS
#ifndef UNUSE_SIGIO_SIGNAL
		prv->ctl_flags &= ~FASYNC;
		if (fcntl(prv->fd, F_SETFL, prv->ctl_flags) < 0) {
			Error("Can't disable the mouse SIGIO.");
		}
#endif
#endif /* XTHREADS */
		if (ioctl(prv->fd, MSDFLUSH, NULL) < 0) {
			Error("Mouse ioctl MSDFLUSH fault.");
		}
		RemoveEnabledDevice(prv->fd);
		pMouse->on = FALSE;
#ifdef XTHREADS
		(void)close(prv->fd);
#ifdef uniosu
                (void)close(prv->ttyfd);
#endif
#endif /* XTHREADS */
		break;
	}
	return (Success);
}

#ifdef uniosu
static Bool
omronResetTty(prv)
omronMousePrvPtr prv;
{
	int dummy = 0;

	if ((prv->ttyfd = open("/dev/tty",O_RDWR,0) ) == -1 ) {
		if ( errno != ENXIO ) {
			Error("Can't open tty.");
			return FALSE;
		}
	}
		
	if ( ioctl(prv->ttyfd,TIOCNOTTY,dummy) < 0 ) {
		Error("Can't ioctl TIOCNOTTY.");
		return FALSE;
	} 
	return TRUE;
}
#endif

static Bool
omronMouseInit(prv)
omronMousePrvPtr prv;
{
#ifdef	uniosu
	struct mssetlimt limit;
#endif	/* uniosu */

#if defined(XTHREADS) || defined(UNUSE_SIGIO_SIGNAL)
	int arg = 1;
#endif

	if ((prv->fd = open("/dev/mouse",O_RDONLY)) == -1) {
		Error("Can't open mouse device.");
		return FALSE;
	}

#if defined(XTHREADS) || defined(UNUSE_SIGIO_SIGNAL)
	ioctl(prv->fd, FIONBIO, &arg); /* set non-blocking io */
#else
	if ((prv->ctl_flags = fcntl(prv->fd, F_GETFL, NULL)) < 0) {
		Error("Mouse fcntl F_GETFL fault.");
		return FALSE;
	}
	prv->ctl_flags |= FNDELAY;
	if (fcntl(prv->fd, F_SETFL, prv->ctl_flags) < 0
		|| fcntl(prv->fd, F_SETOWN, getpid()) < 0) {
		Error("Can't set up mouse to receive SIGIO.");
		return FALSE;
	}
#endif

	if (ioctl(prv->fd,MSSETCURS,8) < 0) {
		Error("Mouse ioctl MSSETCURS fault.");
		return FALSE;
	}

	if(ioctl(prv->fd, MSSETMODE,1) < 0) {
		Error("Mouse ioctl MSSEMODE fault.");
		return FALSE;

	}
#ifdef	uniosu
	limit.mode = 1;
	if(ioctl(prv->fd, MSSETLIMT,&limit) < 0) {
		Error("Mouse ioctl MSSETLMT fault.");
		return FALSE;
	}
#endif	/* uniosu */
	return TRUE;
}

void
omronMouseGiveUp()
{
	DevicePtr     pMouse;
	omronMousePrvPtr prv;

	if(pMouse = LookupPointerDevice()) {
		prv = (omronMousePrvPtr)(pMouse->devicePrivate);
		if(prv) {
			(void)close(prv->fd);
#ifdef uniosu
			(void)close(prv->ttyfd);
#endif
		}
	}
}

#define MAXEVENTS    1024

static Bool
omronCursorOffScreen(pScreen, x, y)
    ScreenPtr	*pScreen;
    int		*x, *y;
{
	return FALSE;
}

static void
#ifndef XTHREADS
omronCrossScreen(pScreen,x,y)
#else /* XTHREADS */
omronCrossScreen(pScreen, flag)
#endif /* XTHREADS */
    ScreenPtr	pScreen;
#ifndef XTHREADS
    int		x;
    int		y;
#else /* XTHREADS */
    Bool		flag;
#endif /* XTHREADS */
{
}


static short
omronMouseAccelerate (pMouse, delta)
    DevicePtr	  pMouse;
    int	    	  delta;
{
    register PtrCtrl *p;
    register int  s;


    p = &((DeviceIntPtr)pMouse)->ptrfeed->ctrl;
  
    if(delta > 0) {
	s = 1;
    } else {
	s = -1;
	delta = -delta;
    }
	
    if (delta > p->threshold) {
	return ((short)(s * (p->threshold +
		    ((delta - p->threshold) * p->num) / p->den)));
    } else {
	return ((short)(s * delta));
    }
}


static void
omronMouseControl()
{
}

static void
omronWarpCursor (pScreen, x, y)
	ScreenPtr   pScreen;
	int         x, y;
{
#ifndef XTHREADS
	int oldmask;

	oldmask = sigblock (sigmask(SIGIO));
#endif /* XTHREADS */
    /* NOTE - MTX may not pend signal. */
    miPointerWarpCursor (pScreen, x, y);
#ifndef XTHREADS
    sigsetmask (oldmask);
#endif /* XTHREADS */
}




struct msdata *
omronMouseGetEvents(pMouse, pNumEvents, pAgain)
DevicePtr     pMouse;
int           *pNumEvents;
Bool          *pAgain;
{
	struct msdgeta	msdata;
	static struct msdata data[MAXEVENTS];
	omronMousePrvPtr prv =(omronMousePrvPtr) (pMouse->devicePrivate);

	msdata.mode = 1;
	msdata.count = MAXEVENTS;
	msdata.msdatap = data;

	if (ioctl(prv->fd,MSDGETA,&msdata) < 0) {
		return FALSE;
	}

	*pNumEvents = msdata.retval;
	
	*pAgain = (msdata.retval == MAXEVENTS);
	
	return(data);
}

#ifndef UNUSE_DRV_TIME
struct msdatat *
omronMouseGetTEvents(pMouse, pNumEvents, pAgain)
DevicePtr     pMouse;
int           *pNumEvents;
Bool          *pAgain;
{
	struct msdgetat	msdatat;
	static struct msdatat datat[MAXEVENTS];
	omronMousePrvPtr prv =(omronMousePrvPtr) (pMouse->devicePrivate);

	msdatat.mode = 1;
	msdatat.count = MAXEVENTS;
	msdatat.msdatatp = datat;
	if (ioctl(prv->fd,MSDGETAT,&msdatat) < 0) {
		return FALSE;
	}

	*pNumEvents = msdatat.retval;
	
	*pAgain = (msdatat.retval == MAXEVENTS);
	
	return(datat);
}
#endif


void
omronMouseEnqueueEvent(pMouse, data)
DevicePtr     pMouse;
struct msdata           *data;
{
	register 	int i;
	register	int	button_state;
	xEvent              xE;
	short omronMouseAccelerate();
	int delta_X,delta_Y;
	omronMousePrvPtr prv =(omronMousePrvPtr) (pMouse->devicePrivate);

	button_state = prv->button_state;
	lastEventTime = GetTimeInMillis();
	xE.u.keyButtonPointer.time = lastEventTime;
		
	switch(data->type) {
	case COORDEVNT:
		xE.u.u.type = MotionNotify;
		delta_X = omronMouseAccelerate(pMouse,
				data->event.coordinate.X_coord);
		delta_Y = omronMouseAccelerate(pMouse,
				data->event.coordinate.Y_coord);
		miPointerDeltaCursor (delta_X, delta_Y, lastEventTime);
		break;
	case BUTONEVNT:
		i = ((data->event.button.L_button == 0) << 2) |
		    ((data->event.button.M_button == 0) << 1) |
		    (data->event.button.R_button == 0);
		button_state = (((button_state & ~i) & 0x7) << 6) |
			    (((~button_state & i) & 0x7) << 3) |
			    (i & 0x7);

		if (BUTTON_PRESSED(button_state)) {
			xE.u.u.type = ButtonPress;
			if (LEFT_PRESSED(button_state)) {
				xE.u.u.detail = Button1; 
				mieqEnqueue(&xE);
			}
			if (RIGHT_PRESSED(button_state)) {
				xE.u.u.detail = Button3; 
				mieqEnqueue(&xE);
			}
			if (MIDDLE_PRESSED(button_state)) {
				xE.u.u.detail = Button2; 
				mieqEnqueue(&xE);
			}
		}
		if (BUTTON_RELEASED(button_state)) {
			xE.u.u.type = ButtonRelease;
			if (LEFT_RELEASED(button_state)) {
				xE.u.u.detail = Button1; 
				mieqEnqueue(&xE);
			}
			if (RIGHT_RELEASED(button_state)) {
				xE.u.u.detail = Button3; 
				mieqEnqueue(&xE);
			}
			if (MIDDLE_RELEASED(button_state)) {
				xE.u.u.detail = Button2; 
				mieqEnqueue(&xE);
			}
		}
		break;
	}
	prv->button_state = button_state;
}

#ifndef UNUSE_DRV_TIME
void
omronMouseEnqueueTEvent(pMouse, data)
DevicePtr     pMouse;
struct msdatat           *data;
{
	register 	int i;
	register	int	button_state;
	xEvent              xE;
	short omronMouseAccelerate();
	int delta_X,delta_Y;
	register unsigned long  time;
	omronMousePrvPtr prv =(omronMousePrvPtr) (pMouse->devicePrivate);

	button_state = prv->button_state;

	time = xE.u.keyButtonPointer.time = data->time;	

	switch(data->type) {
	case COORDEVNT:
		xE.u.u.type = MotionNotify;
		delta_X = omronMouseAccelerate(pMouse,
				data->event.coordinate.X_coord);
		delta_Y = omronMouseAccelerate(pMouse,
				data->event.coordinate.Y_coord);
		miPointerDeltaCursor (delta_X, delta_Y, time);
		break;
	case BUTONEVNT:
		i = ((data->event.button.L_button == 0) << 2) |
		    ((data->event.button.M_button == 0) << 1) |
		    (data->event.button.R_button == 0);
		button_state = (((button_state & ~i) & 0x7) << 6) |
			    (((~button_state & i) & 0x7) << 3) |
			    (i & 0x7);

		if (BUTTON_PRESSED(button_state)) {
			xE.u.u.type = ButtonPress;
			if (LEFT_PRESSED(button_state)) {
				xE.u.u.detail = 1; 
				mieqEnqueue(&xE);
			}
			if (RIGHT_PRESSED(button_state)) {
				xE.u.u.detail = 3; 
				mieqEnqueue(&xE);
			}
			if (MIDDLE_PRESSED(button_state)) {
				xE.u.u.detail = 2; 
				mieqEnqueue(&xE);
			}
		}
		if (BUTTON_RELEASED(button_state)) {
			xE.u.u.type = ButtonRelease;
			if (LEFT_RELEASED(button_state)) {
				xE.u.u.detail = 1; 
				mieqEnqueue(&xE);
			}
			if (RIGHT_RELEASED(button_state)) {
				xE.u.u.detail = 3; 
				mieqEnqueue(&xE);
			}
			if (MIDDLE_RELEASED(button_state)) {
				xE.u.u.detail = 2; 
				mieqEnqueue(&xE);
			}
		}
		break;
	}
	prv->button_state = button_state;
}
#endif


