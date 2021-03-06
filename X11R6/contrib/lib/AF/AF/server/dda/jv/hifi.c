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

#include <stdio.h>
#include <server/include/dia.h>
#include <server/include/misc.h>
#include <server/include/acstruct.h>
#include <server/include/task.h>
#include "jv_io.h"
#include "physdevice.h"
#include "devtime.h"
#include "dsp.h"
#include "dsp_func.h"
#include "hifi.h"
#include <AF/AFUtils.h>
#include "uatables.h"

int epsilon=4;

/*
  There are three hifi devices:  a stereo device, a left channel device, and
  a right channel device.  All three devices share the same server buffers,
  which are maintained in stereo (alternating left/right channel samples).

  The AudioDevice fields 'playNchannels' and 'recNchannels' differentiate
  between the stereo device and the two mono devices.  For the mono devices,
  the HiFiPrivate field 'channel' differentiates (valid 0 or 1) specifies the
  left or right channel.

  The periodic update task runs "on" the stereo device (which is considered
  the "primary" hifi device).  This device also sets the audio time for all
  hifi devices.  The HifiPrivate field 'prim' points back to the stereo 
  device for all hifi devices.
*/
extern int hifiRecord(ATime rtime, unsigned char *dp, int rlen, ACPtr ac);
extern int hifiPlay(ATime ptime, unsigned char *p, int plen, ACPtr ac);

#define	LIMIT(low, val, high) \
	  if (val < (low)) val = (low); \
	  else \
	  if ((val) > (high)) val = (high);

/*
  The hifi device currently doesn't support any device gains.  Min, max, and
  current gains are zero.
*/
static int hifiQueryGain(AudioDevicePtr aDev, int *minp, int *maxp)
{
	if(minp != NULL)
		*minp = 0;

	if(maxp != NULL)
		*maxp = 0;

	return 0;
}

/*
  Handle GetTime request:  read and return the time for a Hifi channel.
  We use the "primary" audio device.  For single channel devices (left and
  right), this points back to the stereo device that the channel is built
  on.
*/
static ATime hifiGetTime(AudioDevicePtr aDev)
{
	HiFiPrivate	*hPtr = (HiFiPrivate *) aDev->privPtr;

	HIFI_UPDATE_TIME(hPtr->prim);
	aDev->time0 = hPtr->prim->time0;
	return hPtr->prim->time0;
}

/*
  Destroy an audio context.  If a record was done on this context, then 
  decrement the reference count for the *primary* device.
*/
void hifiDestroyAC(ACPtr pAC)
{
	HiFiPrivate *hPtr = (HiFiPrivate *) pAC->aDev->privPtr;
	AudioDevicePtr prim=hPtr->prim;
	
	if(pAC->recRef)
		prim->recRefCount--;

	xfree(pAC->funcs);
	xfree(pAC->ops);
}

void hifiChangeAC(void *ptr, ACPtr pAC, int mask)
{
    LIMIT(-100, pAC->playGain, 100);	
    if (mask & ACPlayGain) 
      pAC->playGainMul = AFdBtoLin((double) pAC->playGain);
}

/*
  Create a hifi audio context
*/
ABool hifiCreateAC(ACPtr pAC)
{

	if((pAC->funcs = (ACFuncs *)AFalloc(sizeof(ACFuncs)))==NULL){
		return FALSE;
	}
	if((pAC->ops = (ACOps *)AFalloc(sizeof(ACOps)))==NULL){
		xfree(pAC->funcs);
		return FALSE;
	}
	pAC->recRef = FALSE;
	pAC->funcs->ChangeAC = hifiChangeAC;
	pAC->funcs->DestroyAC = hifiDestroyAC;
	
	pAC->ops->ConvertPlay = hifiPlay;
	pAC->ops->ConvertRec = hifiRecord;

        pAC->playGainMul = 1;
	return TRUE;
}

/*
  Periodic record update.  Copies bytes from the DSP's buffer into the
  server's buffer.
*/
void hifiRecUpdate(AudioDevicePtr aDev)
{
	int	diff;

	diff = DELTA(1, aDev->time0, aDev->timeRecLastUpdated);
	if(diff > HIFI_HWBUF) {
		aDev->timeRecLastUpdated = aDev->time0 - HIFI_HWBUF;
		diff = HIFI_HWBUF;
	}
	if(diff > 0)
		hifi_read_back(aDev->timeRecLastUpdated, diff, aDev);

	aDev->timeRecLastUpdated = aDev->time0;
}

/*
  Periodic update.  Copies play and record data between the DSP's buffers
  and the server's buffers.

  Variables:

  timeNextUpdate	The time through which the module's buffers have been
  			updated (i.e. the next update starts here)

  timeLastValid		The time of the last valid data in the server's play 
			buffers.

  Backfilling of silence is done only when necessary by the play code.
*/
void hifiUpdate(AudioDevicePtr aDev)
{	
	HiFiPrivate	*hPtr = (HiFiPrivate *) aDev->privPtr;
	int		guardband = hPtr->guardBandTicks;
	int 		diff;
	ATime		endtime;

	/* Did we miss the update? */
/* fprintf(stderr,"%d %d %d\n", aDev->time0, aDev->timeNextUpdate,aDev->timeLastValid);  */
	if(FUTURE(aDev->time0, aDev->timeNextUpdate))  {
		aDev->timeNextUpdate = aDev->time0 + epsilon;
		aDev->timeLastUpdated = aDev->time0 - 
			guardband+epsilon;
	}

	/* compute the region we need to update, and update it */
	endtime = aDev->time0 + guardband;
	diff = endtime - aDev->timeNextUpdate;
	if (diff > 0) {
		hifi_write_back(aDev->timeNextUpdate, diff, aDev);
		aDev->timeNextUpdate += diff;
	}

	/* Silence backfill. */
	diff = 	aDev->time0 - aDev->timeLastUpdated;
	if (diff > 0) 
		hifi_silence_fill(aDev->timeLastUpdated, diff, aDev);

	aDev->timeLastUpdated = aDev->time0;

	/* move record data */
	if(aDev->recRefCount)
		hifiRecUpdate(aDev);
}

/*
  Periodic update task:  performs periodic operation and resubmits task
  for execution.
*/
void hifiUpdateTask(TaskPtr oldTask)
{	
	TaskPtr		newTask=NewTask();
	AudioDevicePtr 	aDev=oldTask->aDev;
	HiFiPrivate	*hPtr = (HiFiPrivate *) aDev->privPtr;

	*newTask = *oldTask;		/* Task for next time.		*/

	/* Get the current device time and update audio device time. */
	HIFI_UPDATE_TIME(aDev);

	/* Perform the write-back update with silence fill. */
	hifiUpdate(aDev);

	newTask->time = aDev->time0;	/* Mark new task with old time. */
	AddTask((VoidProc) hifiUpdateTask, newTask, hPtr->updateBandMs);
}

/*
  Intitialize the Hifi periodic update procedure
*/
void hifiUpdateInit(AudioDevicePtr aDev)
{
	HiFiPrivate	*hPtr = (HiFiPrivate *) aDev->privPtr;
	TaskPtr		task;

	aDev->timeLastValid = 0;
	aDev->timeNextUpdate = 0;
	aDev->timeRecLastUpdated = 0;

	task = NewTask();
	task->aDev = aDev;
	task->time = 0;
	AddTask((VoidProc) hifiUpdateTask, task, hPtr->updateBandMs);
}

/*
  Initialize a HiFi device.  'rate' specifies the sample rate for the device.
  'type' specifies the type of the device (stereo, left, or right channel).
  'prim' is a pointer to the primary stereo device for all hifi devices.
*/
ABool hifiInit(AudioDevicePtr aDev, int rate, int type, AudioDevicePtr prim)
{
	jvPhysDevice *pDev=(jvPhysDevice *)aDev->devPtr;
	HiFiPrivate	*hPtr;
	HiFiPrivate	*phPtr;
	int		channels;

	channels = (type == HIFI_STEREO) ? 2 : 1;

	aDev->time0 = 0;		/* The beginning of time, 	*/
	aDev->oldDevTime = 0;		/* the first time.		*/
	aDev->dsptime = 0;

	aDev->QueryOutputGain = hifiQueryGain;
	aDev->QueryInputGain = hifiQueryGain;

	aDev->CreateAC = hifiCreateAC;
	aDev->GetTime = hifiGetTime;		/* Gets device time.	*/

	aDev->playSampleFreq = rate;		/* Describes the play 	*/

	if (rate == 8000) {
		epsilon = 4;
	} else {
		epsilon = 20;
	}
        aDev->playBufType = LIN16;
	aDev->playNchannels = channels; 
	aDev->playNSamplesBuf = rate * 4; 

        aDev->recBufType = LIN16;
	aDev->recSampleFreq = rate;	 	/* Describe the record 	*/
	aDev->recNchannels = channels; 		/* hardware.		*/
	aDev->recNSamplesBuf = rate * 4; 

	aDev->numberOfInputs = 1;
	aDev->numberOfOutputs = 1;
	aDev->inputsFromPhone = 0;
	aDev->outputsToPhone = 0;

	aDev->recRefCount = 0;			/* number of recording ACs */

	if(type == HIFI_STEREO) {
		/* Initialize player.						*/
		if((aDev->playBuf = (pointer)AFalloc(aDev->playNSamplesBuf*HSIZE*2))
			==(pointer)NULL)	return FALSE;
		memset(aDev->playBuf, (int)SILENCE, aDev->playNSamplesBuf*HSIZE*2);	

		/* Initialize recorder.						*/
		if((aDev->recBuf = (pointer)AFalloc(aDev->recNSamplesBuf*HSIZE*2))
			==(pointer)NULL)	return FALSE;
		memset(aDev->recBuf, (int)SILENCE, aDev->recNSamplesBuf*HSIZE*2);
	} else {
		aDev->playBuf = prim->playBuf;
		aDev->recBuf = prim->recBuf;
	}

	/* Create the hifi private structure */
	if((aDev->privPtr = (pointer)AFalloc(sizeof(HiFiPrivate))) == NULL)
		return FALSE;

	hPtr = (HiFiPrivate *) aDev->privPtr;
	hPtr->prim = prim;
	switch(type) {
		case HIFI_STEREO:
			hPtr->leftPlayBuf = dspGetHifiPlayBuf(pDev, 0);
			hPtr->rightPlayBuf = dspGetHifiPlayBuf(pDev, 1);
			hPtr->leftRecBuf = dspGetHifiRecBuf(pDev, 0);
			hPtr->rightRecBuf = dspGetHifiRecBuf(pDev, 1);
			hPtr->devTimePtr = (DSPTime *)dspGetHifiTimePtr(pDev);
			hPtr->sampleRate = rate;

/*
  Here we compute the sizes of the guard and update bands.  The guard band
  size (in samples) is the size of the DSP's hifi ring buffer.  The update band
  is 1/4 the size of the update band.
*/
			hPtr->guardBandTicks = HIFI_HWBUF;
			hPtr->guardBandMs = hPtr->guardBandTicks * 1000 / 
						hPtr->sampleRate;
			hPtr->updateBandTicks = hPtr->guardBandTicks / 4;
			hPtr->updateBandMs = hPtr->updateBandTicks * 1000 / 
						hPtr->sampleRate;

			hifiUpdateInit(aDev);
			break;
		case HIFI_LEFT:
			phPtr = (HiFiPrivate *)prim->privPtr;
			hPtr->updateBandTicks = phPtr->updateBandTicks;
			hPtr->channel = 0;
			break;
		case HIFI_RIGHT:
			phPtr = (HiFiPrivate *)prim->privPtr;
			hPtr->updateBandTicks = phPtr->updateBandTicks;
			hPtr->channel = 1;
			break;
	}
	return TRUE;
}
