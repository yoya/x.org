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
/* $Header:rtcursor.c 1.4$ */
/* $ACIS:rtcursor.c 1.4$ */
/* $Source: /ibm/acis/usr/src/X11/server/ddx/ibm/rt/RCS/rtcursor.c,v $ */

#ifndef lint
static char *rcsid = "$Header:rtcursor.c 1.4$";
#endif

#include <stdio.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/time.h>
#include <machinecons/xio.h>

#define NEED_EVENTS
#include "X.h"
#include "Xproto.h"
#include "input.h"
#include "scrnintstr.h"
#include "cursorstr.h"
#include "miscstruct.h"

#include "rtio.h"
#include "rtcursor.h"
#include "rtscreen.h"
#include "rtutils.h"

/***============================================================***/

    /*
     * munge the SERVER-DEPENDENT, device-independent cursor bits into
     * what the device wants.
     * Server cursor image is *fullword* aligned,
     * RT cursor image is 16x16.
     */

#define SERVER_PAD(w)	( ( ( ( w ) + 31 ) >> 5 ) << 2 ) /* i.e. ... /32)*4) */

Bool
rtRealizeCursor( pScr, pCurs)
    ScreenPtr	pScr;
    CursorPtr 	pCurs;	/* a SERVER-DEPENDENT cursor */
{
    QIOLocator	*loc;
    int		row_bytes;
    int		i,num_rows;

    TRACE(("rtRealizeCursor( pScr= 0x%x, pCurs= 0x%x)\n",pScr,pCurs));

    loc = (QIOLocator *)Xalloc( sizeof(QIOLocator) );
    bzero( (char *)loc, sizeof(QIOLocator) );
    pCurs->devPriv[ pScr->myNum ]= (pointer)loc;
    row_bytes= SERVER_PAD(pCurs->width);
    num_rows= (16<pCurs->height?16:pCurs->height);

    for (i=0;i<num_rows;i++) {
       loc->data[i]= ((~pCurs->source[row_bytes*i]
	      & pCurs->mask[row_bytes*i])<<8)&0xff00;
       loc->mask[i]= (pCurs->mask[row_bytes*i]<<8)&0xff00;
       if (pCurs->width>8) {	/* second byte? */
	  loc->data[i]|= ~pCurs->source[row_bytes*i+1]
	      & pCurs->mask[row_bytes*i+1];
	  loc->mask[i]|= pCurs->mask[row_bytes*i+1];
       }
    }
    loc->hotSpot.v=	pCurs->yhot;
    loc->hotSpot.h=	pCurs->xhot;
    return TRUE ;
}

/***============================================================***/

Bool
rtUnrealizeCursor( pScr, pCurs)
    ScreenPtr	pScr;
    CursorPtr	pCurs;
{
    TRACE(("rtUnrealizeCursor( pScr= 0x%x, pCurs= 0x%x )\n",pScr,pCurs));

    Xfree( pCurs->devPriv[ pScr->myNum]);
    return TRUE ;
}

/***============================================================***/

int
rtDisplayCursor( pScr, pCurs )
    ScreenPtr	pScr;
    CursorPtr	pCurs;
{
    TRACE(("rtDisplayCursor( pScr= 0x%x, pCurs= 0x%x )\n",pScr,pCurs));

    rtSoftCursor(pScr->myNum)= FALSE;
    rtCurrentCursor(pScr->myNum)= pCurs;
    ioctl( rtEmulatorFD, QIOCLDCUR, pCurs->devPriv[ pScr->myNum ]);
    return ;
}

/***============================================================***/

int
rtSetCursorPosition( pScr, x, y, generateEvent )
    register ScreenPtr 	pScr;
    register int	x,y;
    Bool	generateEvent;
{
    xEvent	motion;
    DevicePtr	mouse;
    register BoxRec	*bounds;
    int		rtrn;

    TRACE(("rtSetCursorPosition( pScr= 0x%x, x= %d, y= %d )\n",pScr,x,y));

    if (pScr->myNum!=rtCurrentScreen) {
	(*rtHideCursor(rtCurrentScreen))(rtScreen(rtCurrentScreen));
	rtCurrentScreen= pScr->myNum;
    }

    if (generateEvent)
    {
	if (rtQueue->head != rtQueue->tail)
	    ProcessInputEvents();
	motion.u.keyButtonPointer.rootX = x;
	motion.u.keyButtonPointer.rootY = y;
	motion.u.keyButtonPointer.time = lastEventTime;
	motion.u.u.type = MotionNotify;

	mouse = LookupPointerDevice();
	(*mouse->processInputProc) (&motion, mouse);
    }
    {
	    XCursor pos ;

	    pos.x = x + rtScreenMinX( pScr->myNum ) ;
	    pos.y = y + rtScreenMinY( pScr->myNum ) ;

	    rtrn = ioctl( rtEmulatorFD, QIOCSMSTATE, (caddr_t) &pos ) ;
    }

    bounds= rtCursorBounds(pScr->myNum);
    if (rtSoftCursor(pScr->myNum)) {
	/*
	if ( ( x >= bounds->x1 ) && ( x < bounds->x2 )
	  && ( y >= bounds->y1 ) && ( y < bounds->y2 ) ) {
	*/
		/* This means the cursor IS Within the bounds */
		(*(rtCursorShow(pScr->myNum)))(x,y);
	/*
	}
	else {
		(*(rtCursorX(pScr->myNum)))=(x-rtCursorHotX(pScr->myNum))&0x3ff;
		(*(rtCursorY(pScr->myNum)))=(y-rtCursorHotY(pScr->myNum))&0x3ff;
	}
	*/
    }
    return rtrn;
}

/***============================================================***/

void
rtPointerNonInterestBox( pScr, pBox )
ScreenPtr	pScr;
BoxPtr		pBox;
{

    TRACE(("rtPointerNonInterestBox( pScr= 0x%x, pBox= 0x%x )\n"));

    rtXaddr->mbox.top=		pBox->y1-rtScreenMinY(rtCurrentScreen);
    rtXaddr->mbox.bottom=	pBox->y2-rtScreenMinY(rtCurrentScreen);
    rtXaddr->mbox.left=		pBox->x1-rtScreenMinX(rtCurrentScreen);
    rtXaddr->mbox.right=	pBox->x2-rtScreenMinX(rtCurrentScreen);
    return ;
}

/***============================================================***/

void
rtConstrainCursor( pScr, pBox )
register ScreenPtr	pScr;
register BoxPtr		pBox;
{
    register BoxRec *bounds ;

    TRACE(("rtConstrainCursor( pScr= 0x%x, pBox= 0x%x )\n"));
    bounds = rtCursorBounds( pScr->myNum ) ;
    bounds->x1 = pBox->x1 ;
    bounds->x2 = pBox->x2 ;
    bounds->y1 = pBox->y1 ;
    bounds->y2 = pBox->y2 ;

    return ;
}

/***============================================================***/

void
rtCursorLimits( pScr, pCurs, pHotBox, pTopLeftBox )
ScreenPtr	pScr;
CursorPtr	pCurs;
BoxPtr		pHotBox;
BoxPtr		pTopLeftBox;
{
    TRACE(("rtCursorLimits( pScr= 0x%x, pCurs= 0x%x, pHotBox= 0x%x, pTopLeftBox= 0x%x)\n", pScr, pCurs, pHotBox, pTopLeftBox));

/*
ErrorF("rtCursorLimits(%d, (%d,%d,%d,%d) (%d,%d))\n",
	pScr->myNum,pHotBox->x1,pHotBox->y1,pHotBox->x2,pHotBox->y2,
	pScr->width,pScr->height);
*/
    pTopLeftBox->x1= max( pHotBox->x1, 0 );
    pTopLeftBox->y1= max( pHotBox->y1, 0 );
    pTopLeftBox->x2= min( pHotBox->x2, pScr->width );
    pTopLeftBox->y2= min( pHotBox->y2, pScr->height );

    return ;
}
