/*
 * PRPQ 5799-WZQ (C) COPYRIGHT IBM CORPORATION 1987,1988
 * LICENSED MATERIALS - PROPERTY OF IBM
 * REFER TO COPYRIGHT INSTRUCTIONS FORM NUMBER G120-2083
 */
/* $Header:ppcutils.h 1.2$ */
/* $ACIS:ppcutils.h 1.2$ */
/* $Source: /ibm/acis/usr/src/X11/server/ddx/ibm/ppc/RCS/ppcutils.h,v $ */

#if !defined(lint) && !defined(LOCORE) && defined(RCS_HDRS)
static char *rcsidppcutils = "$Header:ppcutils.h 1.2$";
#endif

/* This screwy macro is used in all the spans routines and you find
   it all over the place, so it is a macro just to tidy things up.
*/

#define SETSPANPTRS(IN,N,IPW,PW,IPPT,PPT,FPW,FPPT,FSORT)		\
	{								\
	N = IN * miFindMaxBand(((mfbPrivGC *)(pGC->devPriv))->pCompositeClip);\
	PW = (int *)ALLOCATE_LOCAL(N * sizeof(int));			\
	PPT = (DDXPointRec *)ALLOCATE_LOCAL(N * sizeof(DDXPointRec));	\
	if(!PPT || !PW)							\
		{							\
		DEALLOCATE_LOCAL(PPT);					\
		DEALLOCATE_LOCAL(PW);					\
		return;							\
    		}							\
	FPW = PW;							\
	FPPT = PPT;							\
	N = miClipSpans(((ppcPrivGC *)(pGC->devPriv))->pCompositeClip,	\
		IPPT, IPW, IN,						\
		PPT, PW, FSORT);					\
	}

/* The following defines are for MetaWare HighC (hc) compiler on IBM hardware */
#define MAX _max
#define MIN _min
#define ABS _abs
