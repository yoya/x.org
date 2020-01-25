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

/* $Header:mpelcursor.c 5.1$ */
/* $ACIS:mpelcursor.c 5.1$ */
/* $Source: /vice/X11/src/server/ddx/ibm/mpel/RCS/mpelcursor.c,v $ */

#ifndef lint
static char *rcsid = "$Header:mpelcursor.c 5.1$";
#endif

/*
 * xcursor.c -- software cursor
 */

#include <machinecons/xio.h>

#include "X.h"
#include "Xmd.h"
#include "miscstruct.h"
#include "screenint.h"
#include "scrnintstr.h"
#include "misc.h"
#include "cursorstr.h"
#include "maskbits.h"

#include "rtio.h"
#include "rtcursor.h"
#include "rtscreen.h"
#include "rtutils.h"

#include "mpel.h"
#include "mpelhdwr.h"
#include "mpelfifo.h"

#ifdef __HIGHC__
#define MAX(a,b)	_max(a,b)
#define MIN(a,b)	_min(a,b)
#else
#define	MAX(a,b)	((a)>(b)?(a):(b))
#define	MIN(a,b)	((a)<(b)?(a):(b))
#endif

#define	CURSOR_MAX_SIZE		((32/8)*32)
#define	CURRENT_WIDTH		(curCursor->width>16?32:16)
#define	CURRENT_HEIGHT		(MIN(curCursor->height,32))

static	short		 cursorX, cursorY;
static	int		 cursorOn;
static	CARD16		 forePtrn[CURSOR_MAX_SIZE/2];
static	CARD16		 backPtrn[CURSOR_MAX_SIZE/2];
static	int		 clip;
static	int		 currentTemp;
static	int		 currentUnder;
static	CursorPtr	 curCursor;
static	int		 foreColor;
static	int		 backColor;

extern	int	mpelShowCursor();

/***==================================================================***/

mpelCursorInit(index)
    int	index;
{
    TRACE(("rtCursorInit(index=%d)\n",index));

    cursorX = 0; cursorY = 0;
    cursorOn = FALSE;
    rtSoftCursor(index) = TRUE;
    rtCursorBounds(index)->x1 =	2000;	/* magic numbers.  sigh. */
    rtCursorBounds(index)->y1 =	2000;
    rtCursorBounds(index)->x2 =	2000;
    rtCursorBounds(index)->y2 =	2000;
    rtCursorShow(index)	      =	mpelShowCursor ;
    rtCurrentCursor(index)    = NULL;
    curCursor = NULL;
    return ;
}

/***==================================================================***/

/*
 * mpelCheckCursor(x,y,w,h)
 */

mpelCheckCursor(x,y,w,h)
    int x,y,w,h;
{
    int	x1, x2, y1, y2;

/*    TRACE(("mpelCheckCursor(x=%d,y=%d,w=%d,h=%d)\n",x,y,w,h));*/

    if (cursorOn) {
	x1 = x;	
	x2 = x + w - 1;
	y1 = y;
	y2 = y1 + h - 1;

	if 	(cursorX+CURRENT_WIDTH-1 < x1)	clip = FALSE;
	else if	(cursorX > x2)			clip = FALSE;
	else if	(cursorY > y2)			clip = FALSE;
	else if	(cursorY+CURRENT_HEIGHT-1 < y1)	clip = FALSE;
	else 					clip = TRUE;

	if (clip) RestoreBackground(cursorX,cursorY);
    }
}

/***==================================================================***/

mpelRestoreCursor()
{
/*    TRACE(("RestoreCursor()\n"));*/

    if (cursorOn) {
	if (clip) {
	    SaveBackGround(cursorX,cursorY);
	    mpelPutCursorOn(cursorX,cursorY);
	}
	clip = FALSE;
    }
}


/***==================================================================***/

void
mpelHideCursor(index)
int	index;
{
    TRACE(("mpelHideCursor(index=%d)\n",index));

    cursorOn= FALSE;
    RestoreBackground(cursorX,cursorY);
}

/***==================================================================***/

mpelShowCursor(x,y)
    short	x, y;
{
    TRACE(("mpelShowCursor(x=%d,y=%d)\n",x,y));

    x -= curCursor->xhot;
    y -= curCursor->yhot;
    if (cursorOn) {
	RestoreBackground(cursorX, cursorY);
	SaveBackGround(x, y);
	mpelPutCursorOn(x,y);
    }
    cursorX = x; cursorY = y;
}

/***==================================================================***/

/*
 * mpelPutCursorOn(x,y)
 */

static
mpelPutCursorOn(x,y)
    short	x, y;
{
    mpelBLTImmedWColorExpansion	blt;
    int		byteSize,i,w;
    CARD16	*fSrc,*fDst,*bSrc,*bDst;
    CARD16	tempFore[CURSOR_MAX_SIZE],tempBack[CURSOR_MAX_SIZE];
    CARD32	mask32;
    CARD16	mask;

    TRACE(("mpelPutCursorOn(x=%d,y=%d)\n",x,y));

    if (x>MPEL_WIDTH-CURRENT_WIDTH) {
	mask32= endtab[(MPEL_WIDTH-x)%16];
	mask= mask32>>16;
	w= (((MPEL_WIDTH-x)+15)/16)*16;
	fSrc= forePtrn; fDst= tempFore;
	bSrc= backPtrn; bDst= tempBack;
	for (i=0;i<CURRENT_HEIGHT;i++) {
	    *fDst= *fSrc;
	    *bDst= *bSrc;
	    fSrc++; bSrc++;
	    if (CURRENT_WIDTH>16) {
		if (w>16) {
		    fDst++; bDst++;
		    *fDst= *fSrc;
		    *bDst= *bSrc;
		}
		fSrc++; bSrc++;
	    }
	    *fDst&= mask;
	    *bDst&= mask;
	    fDst++; bDst++;
	}
	fSrc= tempFore;
	bSrc= tempBack;
    }
    else {
	w= 	CURRENT_WIDTH;
	fSrc=	forePtrn;
	bSrc=	backPtrn;
    }

    blt.dest.lleft.x=	x;
    blt.dest.lleft.y=	1023-(y+(CURRENT_HEIGHT-1));
    blt.dest.uright.x=	x+w-1;         
    blt.dest.uright.y=	1023-y;
    blt.alu=		GXcopy+1;
    blt.color=		foreColor;
    byteSize=		(w/8)*CURRENT_HEIGHT;

    MPELBLTImmedWColorExpansion(byteSize,&blt);
    MPELSendData(byteSize,fSrc);

    blt.color=		backColor;
    MPELBLTImmedWColorExpansion(byteSize,&blt);
    MPELSendData(byteSize,bSrc);
}

/***==================================================================***/

mpelDisplayCursor(pScr,pCurs)
    ScreenPtr	pScr;
    CursorPtr	pCurs;
{
    CARD16	*srcFG,*srcBG,*dstFG,*dstBG;
    int		i;

    TRACE(("mpelDisplayCursor(pScr=0x%x,pCurs=0x%x)\n",pScr,pCurs));

    /* cleanup old cursor */
    if (cursorOn) 
	RestoreBackground(cursorX,cursorY);

    cursorX += curCursor->xhot;
    cursorY += curCursor->yhot;

    curCursor=	pCurs;
    rtCurrentCursor(pScr->myNum)= pCurs;
    srcFG=	pCurs->devPriv[pScr->myNum];
    srcBG=	srcFG+(CURSOR_MAX_SIZE/2);
    dstFG=	forePtrn;
    dstBG=	backPtrn;
    for (i=0;i<CURRENT_HEIGHT;i++) {
	*dstFG++= *srcFG++;
	*dstBG++= *srcBG++;
	if (CURRENT_WIDTH>16) {
	    *dstFG++= *srcFG;
	    *dstBG++= *srcBG;
	}
	srcFG++; srcBG++;
    }

    mpelRecolorCursor(pScr->visuals);

    cursorX -= curCursor->xhot;
    cursorY -= curCursor->yhot;
    SaveBackGround(cursorX,cursorY);
    mpelPutCursorOn(cursorX,cursorY);
    cursorOn = TRUE;
}

/***==================================================================***/

static
SaveBackGround(srcx,srcy)
    int	srcx,srcy;
{
    mpelVPMBLTDestination	blt;

    TRACE(("SaveBackGround(%d,%d)\n",srcx,srcy));

    blt.destaddr= 		mpelAddr(MPEL_CURSOR_SAVE);
    blt.source.lleft.x=		srcx;
    blt.source.lleft.y=		1023-(srcy+CURRENT_HEIGHT-1); 
    blt.source.uright.x= 	srcx+CURRENT_WIDTH-1;
    blt.source.uright.y=	1023-srcy;	
    blt.comp= 			0x0001;  

    MPELVPMBLTDest(&blt);
}

/***==================================================================***/

static
RestoreBackground(x,y)
    int	x,y;
{
    mpelSrcBLTVPM	blt;

    TRACE(("RestoreBackground(%d,%d)\n",x,y));
    blt.srcaddr=	mpelAddr(MPEL_CURSOR_SAVE);
    blt.dest.lleft.x=	x;
    blt.dest.lleft.y=	1023-(y+(CURRENT_HEIGHT-1));
    blt.dest.uright.x=	x+CURRENT_WIDTH-1;
    blt.dest.uright.y=	1023-y;
    blt.bpixel= 	0x0008;
    blt.alu=		GXcopy+1;
  
    MPELSrcBLTVPM(&blt);
}

/***==================================================================***/

mpelRecolorCursor(pVisual)
    VisualPtr	pVisual;
{
    int		fg,bg;

    TRACE(("mpelRecolorCursor(0x%x)\n",pVisual));

    mpelFindColor(&fg,	curCursor->foreRed, 
			curCursor->foreGreen, 
			curCursor->foreBlue);
    mpelFindColor(&bg, 	curCursor->backRed,
			curCursor->backGreen,
			curCursor->backBlue);
    foreColor= fg;
    backColor= bg;
}
