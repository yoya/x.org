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

/* $Header:mpelfifo.c 5.0$ */
/* $ACIS:mpelfifo.c 5.0$ */
/* $Source: /vice/X11/src/server/ddx/ibm/mpel/RCS/mpelfifo.c,v $ */

#ifndef lint
static char *rcsid = "$Header:mpelfifo.c 5.0$";
#endif

/***==================================================================***/

#include "Xmd.h"
#include "misc.h"

#include "mpelhdwr.h"
#include "mpelfifo.h"

#include "rtutils.h"

#undef min
#ifdef __HIGHC__
#define	min(a,b)	_min(a,b)
#else
#define	min(a,b)	((a)<(b)?(a):(b))
#endif

#define DELAY_ADDR	0xf00080E0		/* IOCC delay location */
#define DELAY(n)	{ register int N = (n); \
				while (--N >= 0) \
					* (char *) DELAY_ADDR = 0xff;	}

/***==================================================================***/

/*
 *  mpelWFifo writes commands to the megapel fifo.
 */

int
mpelWFifo(nargs,command,arg1,arg2)
    int		 nargs;
    CARD16	 command;
    CARD32	 arg1;
    CARD32	 arg2;
{
    int		 retry;			/* Current RETRY count	     */
    int		 i;			/* DELAY loop index	     */
    int		 safe;			/* safe number of bytes to write */
    CARD16 	*data_ptr,tmp[2];
    CARD16 	 fifo_flags;
    int		 out_data,		/* num of bytes in command */
		 data_len;		/* num of bytes we have to send... */


    TRACE(("mpelWFifo(%d,0x%x,0x%x,0x%x)\n",nargs,command,arg1,arg2));

    if (!command) {
	data_len= nargs;
	if (data_len&0x1) data_len++;
	out_data= data_len;
	data_ptr= (CARD16 *)arg1;
    }
    else if (nargs<=2) {
	out_data= data_len= 4+(nargs*2);
	tmp[0]= (CARD16)arg1;
	tmp[1]= (CARD16)arg2;
	data_ptr= &tmp[0];
    }
    else {
	data_len= nargs;
	out_data= nargs+arg2;
	if (out_data&0x1) out_data++;
	data_ptr= (CARD16 *)arg1;
    }

    if (command) {
	while (!(MPEL_PSR&PSR_FIFO_NOT_FULL));
	MPEL_FIFO= out_data;

	while (!(MPEL_PSR&PSR_FIFO_NOT_FULL));
	MPEL_FIFO= command;
	data_len-= 4;
    }

    retry = 0;
    while(data_len>0) {
	fifo_flags = MPEL_PSR & PSR_FIFO_FLAGS;	/* fifo flags		     */
	 
	/*
	 * figure out how many bytes we can safely write to the fifo.
         */
		
	if (!(fifo_flags&PSR_FIFO_NOT_EMPTY)) 		safe= MPEL_FIFO_LEN;
	else if (fifo_flags&PSR_FIFO_NOT_HALF_FULL)	safe= MPEL_FIFO_LEN/2;
	else if (fifo_flags&PSR_FIFO_NOT_FULL) 		safe= 2;
	else {
	    safe= 0;
	    if(retry > MPEL_MAXRETRY) {
		ErrorF("WSGO!! FIFO jammed -- MPEL_MAXRETRY reached\n");
		return(-1);			/* give up	    */
	    }
	    DELAY(1024);
	    retry++;
	    continue;
	}

	i= min(safe,data_len);
	data_len-= i;
	while (i>0) {
	    MPEL_FIFO= *data_ptr++;
	    i-= 2;
	}
    } 						/* end of while loop     */
    return(0);
}

/***==================================================================***/

mpelStartFifo()
{
    TRACE(("mpelStartFifo()\n"));

    MPEL_COMM_REQ= MPELCMD_ENTER_FIFO;
    MPEL_COMM_REASON= 0;
    MPEL_PCR&= (~PCR_INTR_TMS);
    MPEL_PCR|= PCR_INTR_TMS;
}

/***==================================================================***/

mpelContFifo()
{
    TRACE(("mpelContFifo()\n"));

    MPEL_COMM_REQ= MPELCMD_CONT_FIFO;;
    MPEL_COMM_REASON= 0;
    MPEL_PCR&= (~PCR_INTR_TMS);
    MPEL_PCR|= PCR_INTR_TMS;
}
