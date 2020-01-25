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

/* $Header:mpelutil.c 5.0$ */
/* $ACIS:mpelutil.c 5.0$ */
/* $Source: /vice/X11/src/server/ddx/ibm/mpel/RCS/mpelutil.c,v $ */

#ifndef lint
static char *rcsid = "$Header:mpelutil.c 5.0$";
#endif

/*
 * xutil.c
 */

#define BADREAD

#include "X.h"
#include "Xmd.h"
#include "screenint.h"
#include "misc.h"
#include "miscstruct.h"
#include "pixmapstr.h"
#include "regionstr.h"

#include "mpelhdwr.h"
#include "mpelfifo.h"

#include "rtutils.h"

#define DELAY_ADDR	0xf00080E0		/* IOCC delay location */
#define DELAY(n)	{ register int N = (n); \
				while (--N >= 0) \
					* (char *) DELAY_ADDR = 0xff;	}

#define RESET_INT_TMS_BIT (MPEL_PCR &= ~PCR_INTR_TMS)
#define SET_INT_TMS_BIT (MPEL_PCR |= PCR_INTR_TMS)

#define MAXRETRY 500

#define PAD32
#define HCBUG

/* #define BANK2 */

mpelRectangle	mpelDfltScissor = {  { 0, 0 }, { 1023, 1023 } };

/***==================================================================***/

/*
 * mpelFillSolid(color,alu,planes,x0,y0,lx,ly)
 */

mpelFillSolid(color,alu,mask,x,y,w,h)
int color, alu, mask, x, y, w, h;
{
    mpelPoint points[4];
    mpelRectangle rect;

    TRACE(("mpelFillSolid(%d,%d,0x%x,%d,%d,%d,%d)\n",color,alu,mask,x,y,w,h));

    mpelCheckCursor(x,y,w,h);
    mpelSetPlaneMask(mask);
    mpelSetALU(alu);	/* logical operation p. 95 */

    if ( alu == GXcopy ) {
	MPELSetPgonInteriorColor(color);
	rect.lleft.x = x;
	rect.lleft.y = 1023 - ( y + h - 1 );
	rect.uright.x = x + w - 1;
	rect.uright.y = 1023 - y;
	MPELFillRectangle(&rect);
    }
    else if ( w == 1 ) {
	MPELSetLineType(1);
	MPELSetPolylineColor(color);
	points[0].x = x;
	points[0].y = 1023 - y;
	points[1].x = x;
	points[1].y = 1023 - ( y + h - 1 );
	MPELPolyline(2,points);
    }
    else if ( h == 1 ) {
	MPELSetLineType(1);
	MPELSetPolylineColor(color);
	points[0].x = x;
	points[0].y = 1023 - y;
	points[1].x = x + w - 1;
	points[1].y = 1023 - y;
	MPELPolyline(2,points);
    }
    else {
	MPELSetPgonInteriorStyle(2);
	MPELSetPgonInteriorColor(color);
	MPELSetPgonEdgeFlag(2);

	MPELBeginPgon();
	points[0].x=	x;
	points[0].y=	1023-y;
	points[1].x=	x+w-1;
	points[1].y=	1023-y;
	points[2].x=	x+w-1;
	points[2].y=	1023-(y+h-1);
	points[3].x=	x;
	points[3].y=	1023-(y+h-1);
	MPELPolyline(4,points);
	MPELEndPgon();	/* end polygon p. 115 */
    }
    MPELResetPlaneMask(0xff);
    mpelRestoreCursor();
}

/***==================================================================***/

/*
 * IDENTIFICATION: mpelicmd (copy from vtt8icmd)
 * DESCRIPTIVE NAME: Issue an Adapte command for the Megapel
 *		     Virtual Display Driver (VDD)
 * FUNCTION: Check for completion of any previous commands.
 *	     When done issue the requested command to the adapter
 * END OF SPEC
 */

mpelicmd2(command_code, parameters, parm_count)
unsigned short command_code;
unsigned short parameters[];
int   parm_count;

{
   int register i;
   int retry = 0;
   unsigned short *COMM_Request_parm;
   unsigned short reg;

	/*
	 * Issue COMM Area Command
	 */
   MPEL_COMM_REQ = command_code;     /* Store request code into COMM area  */ 
   COMM_Request_parm = MPEL_COMM_PARM_ADDR;
   for(i = 0; i < parm_count ; i++)  /* Store parameters into COMM area    */
      COMM_Request_parm[i] = parameters[i];
   RESET_INT_TMS_BIT;
   SET_INT_TMS_BIT;   		     /* Interrupt adapter	           */

	/*
         * check for interrupt pending: clear it.
         */
	retry = 0;
	while (retry < MAXRETRY) {
		reg = MPEL_PSR;
		if (reg & PSR_INTR_PENDING) {
			reg = MPEL_IPR;

			break;
		}
		DELAY(1000);
		retry++;
	}

	/*
         * Check for reason code of command+1
         */
	if (MPEL_COMM_REASON == (command_code+1)) {
	}

	MPEL_COMM_REASON = 0x0000;
	MPEL_COMM_REQ = 0x0000;
}



/*
 * IDENTIFICATION: mpelicmd (copy from vtt8icmd)
 * DESCRIPTIVE NAME: Issue an Adapte command for the Megapel
 *		     Virtual Display Driver (VDD)
 * FUNCTION: Check for completion of any previous commands.
 *	     When done issue the requested command to the adapter
 * END OF SPEC
 */

mpelicmd(command_code, parameters, parm_count)
    CARD16	command_code;
    CARD16	parameters[];
    int   	parm_count;

{
    int		i;
    int		retry = 0;
    CARD16	*COMM_Request_parm;

    TRACE(("mpelicmd(0x%x,0x%x,%d)\n",command_code,parameters,parm_count));

    while(MPEL_COMM_REASON == 0) {   /* The previous command has not yet   */   
       				     /* completed.        	           */
	if( retry > MAXRETRY ) {     /* Adaoter is taking too long on      */
	    ErrorF("Adapter not coming ready for command\n");
	    ErrorF("forcing it\n");
	    break;
	}
	DELAY (1000);
	retry++;		     /* Another chance			   */
    }

    MPEL_COMM_REQ = command_code;     /* Store request code into COMM area  */ 
    MPEL_COMM_REASON = 0;
    COMM_Request_parm = MPEL_COMM_PARM_ADDR;
    for(i = 0; i < parm_count ; i++)  /* Store parameters into COMM area    */
	COMM_Request_parm[i] = parameters[i];
    RESET_INT_TMS_BIT;
    SET_INT_TMS_BIT;   		     /* Interrupt adapter	           */
}

/***==================================================================***/

	/*
	 * Poke the adapter -- execute some command (in this case
	 * reset blink, but it really doesn't matter which) and wait
	 * for it to complete.  Useful for making sure the adapter is
	 * alive and talking to us.
	 */

mpelPoke()
{
    int	i;

    TRACE(("mpelPoke()\n"));

    MPEL_COMM_REQ= MPELCMD_RESET_BLINK;
    MPEL_COMM_REASON= 0;
    RESET_INT_TMS_BIT;
    SET_INT_TMS_BIT;
    for (i=1000000;MPEL_COMM_REQ!=MPELRSN_RESET_BLINK_DONE;i--) {
	if (i<0) {
	    ErrorF("poke failed (%d)\n",(*(short *)0xf4c00904));
	    break;
	}
    }
}

/***==================================================================***/

mpelInitPatterns()
{

    TRACE(("mpelInitPatterns()\n"));

    MPEL_STATE_LIST_PTR = mpelAddr(MPEL_PAT_TABLE);
    MPEL_PAT_TABLE[0] = 4;
    MPEL_PAT_TABLE[1] = 4;
    *(long *)(MPEL_PAT_TABLE+2) = mpelAddr(MPEL_PAT1);
    *(long *)(MPEL_PAT_TABLE+4) = mpelAddr(MPEL_PAT2);
    *(long *)(MPEL_PAT_TABLE+6) = mpelAddr(MPEL_PAT3);
    *(long *)(MPEL_PAT_TABLE+8) = mpelAddr(MPEL_PAT4);
}

/***==================================================================***/

    /*
     * figure out how prect1 and prect2 overlap.
     * returns:
     *    rgnIN:	if prect1 contains prect2
     *	  rgnPART:	if prect1 and prect2 intersect
     *			but prect1 does not contain prect2
     *	  rgnOUT:	if prect1 and prect2 do not intersect
     */

int
mpelRectIntersect(prect1, prect2)
    register BoxPtr	prect1;
    register BoxPtr     prect2;
{

    TRACE(("mpelRectIntersect(0x%x,0x%x)\n",prect1,prect2));

    if ((prect2->x1>=prect1->x1)&&(prect2->x2<prect1->x2)&&
	(prect2->y1>=prect1->y1)&&(prect2->y2<prect1->y2)) {
	return(rgnIN);
    }
    if (prect1->x1 > prect2->x2)	/* rect1 right of rect2 */
	return(rgnOUT);

    if (prect1->x2 < prect2->x1)	/* rect1 left of rect2  */
	return(rgnOUT);

    if (prect1->y1 > prect2->y2)	/* rect1 below rect2 */
	return(rgnOUT);

    if (prect1->y2 < prect2->y1)	/* rect1 above rect2 */
	return(rgnOUT);

    return(rgnPART);
}

