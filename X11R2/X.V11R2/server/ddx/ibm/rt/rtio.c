/***********************************************************
Copyright 1987 by Digital Equipment Corporation, Maynard, Massachusetts,
and the Massachusetts Institute of Technology, Cambridge, Massachusetts.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its
documentation for any purpose and without fee is hereby granted,
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in
supporting documentation, and that the names of Digital or MIT not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.

DIGITAL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/
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
/* $Header:rtio.c 1.3$ */
/* $ACIS:rtio.c 1.3$ */
/* $Source: /ibm/acis/usr/src/X11/server/ddx/ibm/rt/RCS/rtio.c,v $ */

#ifndef lint
static char *rcsid = "$Header:rtio.c 1.3$";
#endif

#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <machinecons/xio.h>

#define NEED_EVENTS

#include "X.h"
#include "Xproto.h"
#include "input.h"
#include "miscstruct.h"
#include "scrnintstr.h"
#include "cursorstr.h"

#include "rtcursor.h"
#include "rtkeyboard.h"
#include "rtmouse.h"
#include "rtscreen.h"
#include "rtutils.h"

XIoAddr		*rtXaddr;
XEventQueue	*rtQueue;
int		lastEventTime;

extern	int	screenIsSaved;

/*****************
 * ProcessInputEvents:
 *    processes all the pending input events
 *****************/

void
ProcessInputEvents()
{
    register int    i ;
    register rtPerScreenInfo *screenInfo ;
    register int x, y;
    register XEvent *pE ;
    xEvent e;
    int nowInCentiSecs, nowInMilliSecs, adjustCentiSecs;
    struct timeval tp;
    int needTime = 1;
    int	oldScr;
    int	setCursor;
#ifdef RT_SPECIAL_MALLOC
    extern int rtShouldDumpArena;
#endif /* RT_SPECIAL_MALLOC */

/*    TRACE(("ProcessInputEvents()\n"));*/

#ifdef RT_SPECIAL_MALLOC
    if ( rtShouldDumpArena ) {
       rtDumpArena() ;
    }
#endif /* RT_SPECIAL_MALLOC */

    for ( i = rtQueue->head ;
	  i != rtQueue->tail ;
i = ( rtQueue->head = ( ( i == rtQueue->size ) ? 0 : ( rtQueue->head + 1 ) ) ) )
    {
	if ( screenIsSaved == SCREEN_SAVER_ON )
	    SaveScreens( SCREEN_SAVER_OFF, ScreenSaverReset ) ;
	pE = &rtQueue->events[i] ;

	if (pE->xe_device) {

		x = (signed short) pE->xe_x ;
		y = (signed short) pE->xe_y ;
		screenInfo = rtScreens[ oldScr = rtCurrentScreen ] ;
		setCursor = FALSE;

		if ( rtYWrapScreen ) {
		    while ( y < screenInfo->rt_ScreenBounds.y1 ) {
			y +=
	screenInfo->rt_ScreenBounds.y2 - screenInfo->rt_ScreenBounds.y1 ;
			setCursor = TRUE ;
		    }

		    while ( y > screenInfo->rt_ScreenBounds.y2 ) {
			y -=
	screenInfo->rt_ScreenBounds.y2 - screenInfo->rt_ScreenBounds.y1 ;
			setCursor = TRUE ;
		    }
		}

		while ( x < screenInfo->rt_ScreenBounds.x1 ) {
		    if ( screenInfo == rtScreens[ 0 ] ) {
			/* Already At First Possible Screen */
			if ( rtXWrapScreen ) {
			    x -= screenInfo->rt_ScreenBounds.x1 ; /* x < 0 */
			    screenInfo = rtScreens[ rtCurrentScreen = rtNumScreens - 1 ] ;
			    x += screenInfo->rt_ScreenBounds.x2 ;
			}
			else {
			    x = screenInfo->rt_ScreenBounds.x1 ;
			}
		    }
		    else {
			x -= screenInfo->rt_ScreenBounds.x1 ; /* Now x < 0 */
			screenInfo = rtScreens[ --rtCurrentScreen ] ;
			x += screenInfo->rt_ScreenBounds.x2 ;
		    }
		    setCursor = TRUE ;
		}

		while ( x > screenInfo->rt_ScreenBounds.x2 ) {
		    if ( screenInfo == rtScreens[ rtNumScreens - 1 ] ) {
			/* Already At Last Possible Screen */
			if ( rtXWrapScreen ) {
			    x -= screenInfo->rt_ScreenBounds.x2 ; /* x > 0 */
			    screenInfo = rtScreens[ rtCurrentScreen = 0 ] ;
			    x += screenInfo->rt_ScreenBounds.x1 ;
			}
			else {
			    x = screenInfo->rt_ScreenBounds.x2 ;
			}
		    }
		    else {
			x -= screenInfo->rt_ScreenBounds.x2 ; /* Now x > 0 */
			screenInfo = rtScreens[ ++rtCurrentScreen ] ;
			x += screenInfo->rt_ScreenBounds.x1 ;
		    }
		    setCursor = TRUE ;
		}

		if ( y > screenInfo->rt_ScreenBounds.y2 ) {
		    y = screenInfo->rt_ScreenBounds.y2 ;
		    setCursor = TRUE ;
		}
		else if ( y < screenInfo->rt_ScreenBounds.y1 ) {
		    y = screenInfo->rt_ScreenBounds.y1 ;
		    setCursor = TRUE ;
		}

		if (setCursor) {
		    XCursor pos;
		    pos.x = x /* + screenInfo->rt_ScreenBounds.x1 */ ;
		    pos.y = y /* + screenInfo->rt_ScreenBounds.y1 */ ;
		    ioctl(rtEmulatorFD,QIOCSMSTATE,(caddr_t)&pos);
		}

		e.u.keyButtonPointer.rootX =
		  ( x -= screenInfo->rt_ScreenBounds.x1 ) ;
		e.u.keyButtonPointer.rootY =
		  ( y -= screenInfo->rt_ScreenBounds.y1 ) ;

		if ( oldScr != rtCurrentScreen ) {
		    (*rtHideCursor( oldScr ) )( oldScr ) ;
		    NewCurrentScreen( screenInfo->rt_Screen, x, y ) ;
		    if ( screenInfo->rt_CurrentCursor != rtCurrentCursor( oldScr ) ) {
			(* screenInfo->rt_Screen->RealizeCursor )(
				screenInfo->rt_Screen,
				rtCurrentCursor( oldScr ) ) ;
		    }
		    (* screenInfo->rt_Screen->DisplayCursor )(
				screenInfo->rt_Screen,
				rtCurrentCursor( oldScr ) ) ;
		}

		if (screenInfo->rt_SoftCursor&&pE->xe_device==XE_MOUSE) {
	/*
		    if ( ( x >= screenInfo->rt_CursorBounds.x1 )
		      && ( x <  screenInfo->rt_CursorBounds.x2 )
		      && ( y >= screenInfo->rt_CursorBounds.y1 )
		      && ( y <  screenInfo->rt_CursorBounds.y2 ) ) {
	*/
			/* this means the cursor IS within bounds */
			(* screenInfo->rt_CursorShow )( x, y ) ;
	/*
		    }
		    else {
			*( screenInfo->rt_CursorX ) =
				( x - screenInfo->rt_CursorHotX ) & 0x3FF ;
			*( screenInfo->rt_CursorY ) =
				( y - screenInfo->rt_CursorHotY ) & 0x3FF ;
		    }
	*/
		}
	    /*
	     * The following silly looking code is because the old version of
	     * the driver only delivers 16 bits worth of centiseconds. We are
	     * supposed to be keeping time in terms of 32 bits of milliseconds.
	     */
		if (needTime)
		{
		    needTime = 0;
		    gettimeofday(&tp, 0);
		    nowInCentiSecs = ((tp.tv_sec * 100) + (tp.tv_usec / 10000)) & 0xFFFF;
		/* same as driver */
		    nowInMilliSecs = (tp.tv_sec * 1000) + (tp.tv_usec / 1000);
		/* beware overflow */
		}
		if ((adjustCentiSecs = nowInCentiSecs - pE->xe_time) < -20000)
		    adjustCentiSecs += 0x10000;
		else
		    if (adjustCentiSecs > 20000)
			adjustCentiSecs -= 0x10000;
		e.u.keyButtonPointer.time = lastEventTime =
		    nowInMilliSecs - adjustCentiSecs * 10;

		if ((pE->xe_type == XE_BUTTON)&&(pE->xe_device==XE_DKB))
		{
		    e.u.u.detail= pE->xe_key;
		    switch (pE->xe_direction) {
			case XE_KBTDOWN:
				e.u.u.type= KeyPress;
				(rtKeybd->processInputProc)(&e,rtKeybd);
				break;
			case XE_KBTUP:
				e.u.u.type= KeyRelease;
				(rtKeybd->processInputProc)(&e,rtKeybd);
				break;
			default:	/* hopefully BUTTON_RAW_TYPE */
				ErrorF("got a raw button, what do I do?\n");
				break;
		    }
		}
		else if ((pE->xe_device==XE_MOUSE)||(pE->xe_device==XE_TABLET))
		{
		    if (pE->xe_type == XE_BUTTON )
		    {
			if (pE->xe_direction == XE_KBTDOWN)
			    e.u.u.type= ButtonPress;
			else
			    e.u.u.type= ButtonRelease;
			/* mouse buttons numbered from one */
			e.u.u.detail = pE->xe_key+1;
		    }
		    else
			e.u.u.type = MotionNotify;
		    (*rtPtr->processInputProc)(&e,rtPtr);
		}
	}
    }
    return ;
}

TimeSinceLastInputEvent()
{
/*    TRACE(("TimeSinceLastInputEvent()\n"));*/

    if (lastEventTime == 0)
	lastEventTime = GetTimeInMillis();
    return GetTimeInMillis() - lastEventTime;
}

void
rtQueryBestSize(class, pwidth, pheight)
register int class;
register short *pwidth;
register short *pheight;
{
    register unsigned width, test;

    switch(class)
    {
      case CursorShape:
	  *pwidth = 16;
	  *pheight = 16;
	  break;
      case TileShape:
      case StippleShape:
	  width = *pwidth;
	  if (width > 0) {
	      /* Return the closes power of two not less than what they gave me */
	      test = 0x80000000;
	      /* Find the highest 1 bit in the width given */
	      while(!(test & width))
		 test >>= 1;
	      /* If their number is greater than that, bump up to the next
	       *  power of two */
	      if((test - 1) & width)
		 test <<= 1;
	      *pwidth = test;
	  }
	  /* We don't care what height they use */
	  break;
    }
    return ;
}
