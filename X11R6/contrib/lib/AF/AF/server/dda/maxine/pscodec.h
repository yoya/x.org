/*
 * Copyright 1993 by Digital Equipment Corporation, Maynard, Massachusetts.
 * 
 * Permission to use, copy, modify, distribute, and sell this software and its 
 * documentation for any purpose is hereby granted without fee, provided that 
 * the above copyright notice appear in all copies and that both that 
 * copyright notice and this permission notice appear in supporting 
 * documentation, and that the name of Digital not be used in advertising or 
 * publicity pertaining to distribution of the software without specific, 
 * written prior permission.  Digital makes no representations about the 
 * suitability of this software for any purpose.  It is provided "as is" 
 * without express or implied warranty.
 * 
 * DIGITAL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING 
 * ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL 
 * DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY 
 * DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN 
 * AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF 
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef	PSCODEC_H
#define	PSCODEC_H

#include <server/include/misc.h>
#include <server/include/ac.h>
#include <server/include/acstruct.h>
#include <server/include/task.h>
#include "ringbuffers.h"

/* 
 * Definitions of this pseudo device implementation.
 */
/* Physical device.							    */
#define SAMPLEFREQ	8000		/* 8000 samples/second		    */
#define SILENCE 	0x0ff		/* mu-255 silence.		    */
#define	MSHWALERT	100		/* Interval of hw->host alert. 	*/
#define	LENHWBUF	8*DMASIZE	/* Number of words in HW Buffer.    */
#define	TIMEHWBUF	(LENHWBUF * 1)	/* Number of samples.		    */

#define TICKSTOMS(tick)	((tick)>>3)
#define MSTOTICKS(ms)	((ms)*SAMPLEFREQ/1000) /* Convert milliseconds to ticks*/
#define	TIMEHWALERT	MSTOTICKS(MSHWALERT)

/* Server Interface to physical device.					    */
#define	MSGUARDBAND	250
#define	MSUPDATE	(MSGUARDBAND/4)
#define PLAY_CHUNK_SIZE	(MSGUARDBAND*2)	/* XXX */
#define	SCHED_LATENCY	100		/* XXX */

#ifndef	MIN
#define	MIN(x,y)	((x) < (y) ? x : y)
#endif

extern void codecUpdateTask(TaskPtr oldTask);
extern void codecUpdateInit(AudioDevicePtr aDev);

static ATime codecGetTime(AudioDevicePtr aDev);
extern void codecChangeAC(void *ptr, ACPtr pAC, int mask);
extern ABool codecCreateAC(ACPtr pAC);
extern void codecDestroyAC(ACPtr pAC);
extern ABool codecPrimaryInit(AudioDevicePtr aDev);
extern void codecUpdate(AudioDevicePtr aDev);
/* extern void codecRecUpdate(AudioDevicePtr aDev); */
extern void codecChangeOutput(AudioDevicePtr aDev, int onoff, int nmask, int *omaskp, int *amaskp);
extern void codecChangeInput(AudioDevicePtr aDev, int onoff, int nmask, int *omaskp, int *amaskp);
extern void codecChangePassThrough(AudioDevicePtr aDev, int onoff, int nmask, int *omaskp, int *amaskp);
extern int codecQueryOutputGain(AudioDevicePtr aDev, int *minp, int *maxp);
extern int  codecSelectOutputGain(AudioDevicePtr aDev, int gdB);
extern int codecQueryInputGain(AudioDevicePtr aDev, int *minp, int *maxp);
extern int  codecSelectInputGain(AudioDevicePtr aDev, int gdB);
#endif
