/***********************************************************
		Copyright IBM Corporation 1987,1988

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
/* $Header: /andrew/X11/R3src/Xbeta/server/ddx/ibm/BSDrt/RCS/bsdCursor.c,v 9.1 88/10/17 14:35:49 erik Exp $ */
/* $Source: /andrew/X11/R3src/Xbeta/server/ddx/ibm/BSDrt/RCS/bsdCursor.c,v $ */

#ifndef lint
static char *rcsid = "$Header: /andrew/X11/R3src/Xbeta/server/ddx/ibm/BSDrt/RCS/bsdCursor.c,v 9.1 88/10/17 14:35:49 erik Exp $";
#endif

#include <stdio.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/time.h>

#define NEED_EVENTS
#include "X.h"
#include "Xproto.h"
#include "input.h"
#include "scrnintstr.h"
#include "cursorstr.h"
#include "miscstruct.h"

#include "bsdIO.h"
#include "bsdCursor.h"

#include "ibmScreen.h"
#include "ibmTrace.h"

/***============================================================***/

int
BSDSetCursorPosition( pScr, x, y, generateEvent )
    register ScreenPtr 	pScr;
    register int	x,y;
    Bool	generateEvent;
{
    xEvent	motion;
    DevicePtr	mouse;
    int		rtrn;

    TRACE(("BSDSetCursorPosition( pScr= 0x%x, x= %d, y= %d )\n",pScr,x,y));

    if (pScr->myNum!=ibmCurrentScreen) {
	(*ibmHideCursor(ibmCurrentScreen))(ibmScreen(ibmCurrentScreen));
	ibmCurrentScreen= pScr->myNum;
    }

    if (generateEvent)
    {
	if (ibmQueue->head != ibmQueue->tail)
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

	    pos.x = x + ibmScreenMinX( pScr->myNum ) ;
	    pos.y = y + ibmScreenMinY( pScr->myNum ) ;

	    rtrn = ioctl( BSDEmulatorFD, QIOCSMSTATE, (caddr_t) &pos ) ;
    }

    (*(ibmCursorShow(pScr->myNum)))(x,y);
    return rtrn;
}

/***============================================================***/

void
BSDPointerNonInterestBox( pScr, pBox )
ScreenPtr	pScr;
BoxPtr		pBox;
{

    TRACE(("BSDPointerNonInterestBox( pScr= 0x%x, pBox= 0x%x )\n"));

    BSDXaddr->mbox.top=		pBox->y1-ibmScreenMinY(ibmCurrentScreen);
    BSDXaddr->mbox.bottom=	pBox->y2-ibmScreenMinY(ibmCurrentScreen);
    BSDXaddr->mbox.left=	pBox->x1-ibmScreenMinX(ibmCurrentScreen);
    BSDXaddr->mbox.right=	pBox->x2-ibmScreenMinX(ibmCurrentScreen);
    return ;
}

/***============================================================***/

void
BSDConstrainCursor( pScr, pBox )
register ScreenPtr	pScr;
register BoxPtr		pBox;
{

    TRACE(("BSDConstrainCursor( pScr= 0x%x, pBox= 0x%x )\n"));
    /* XXX -- not implemented yet */
    return ;
}

/***============================================================***/

void
BSDCursorLimits( pScr, pCurs, pHotBox, pTopLeftBox )
ScreenPtr	pScr;
CursorPtr	pCurs;
BoxPtr		pHotBox;
BoxPtr		pTopLeftBox;
{
    TRACE(("BSDCursorLimits( pScr= 0x%x, pCurs= 0x%x, pHotBox= 0x%x, pTopLeftBox= 0x%x)\n", pScr, pCurs, pHotBox, pTopLeftBox));

/*
ErrorF("BSDCursorLimits(%d, (%d,%d,%d,%d) (%d,%d))\n",
	pScr->myNum,pHotBox->x1,pHotBox->y1,pHotBox->x2,pHotBox->y2,
	pScr->width,pScr->height);
*/
    pTopLeftBox->x1= max( pHotBox->x1, 0 );
    pTopLeftBox->y1= max( pHotBox->y1, 0 );
    pTopLeftBox->x2= min( pHotBox->x2, pScr->width );
    pTopLeftBox->y2= min( pHotBox->y2, pScr->height );

    return ;
}
