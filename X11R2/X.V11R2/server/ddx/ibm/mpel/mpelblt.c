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

/* $Header:mpelblt.c 5.0$ */
/* $ACIS:mpelblt.c 5.0$ */
/* $Source: /vice/X11/src/server/ddx/ibm/mpel/RCS/mpelblt.c,v $ */

#ifndef lint
static char *rcsid = "$Header:mpelblt.c 5.0$";
#endif

#include "Xmd.h"
#include "misc.h"

#include "rtutils.h"

#include "mpelfifo.h"

/*
 * mpelBitBlt(alu, rmask, wmask, x0,y0,x1,y1,w,h)
 */

mpelBitBlt(alu, rmask, wmask, x0,y0,x1,y1,w,h)
int x0,y0,x1,y1,w,h,rmask,wmask,alu;
{
    mpelVPMCopy	vpm;

    TRACE(("mpelBitBlt(0x%x,0x%x,0x%x,%d,%d,%d,%d,%d,%d)\n",
				alu,rmask,wmask,x0,y0,x1,y1,w,h));

    mpelCheckCursor(x0,y0,w,h);
    mpelCheckCursor(x1,y1,w,h);
    mpelSetPlaneMask(wmask);

    vpm.source.lleft.x=		x0;
    vpm.source.lleft.y=		1023-(y0 + h - 1);
    vpm.source.uright.x=	x0 + w - 1;
    vpm.source.uright.y=	1023 - y0;
    vpm.dlleft.x=		x1;
    vpm.dlleft.y=		1023-(y1+h-1);
    vpm.alu=			alu + 1;
    MPELVPMCopy(&vpm);

    MPELResetPlaneMask();
    mpelRestoreCursor();
}

