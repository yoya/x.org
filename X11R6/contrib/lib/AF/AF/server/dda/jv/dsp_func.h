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

#ifndef DSP_FUNC_H
#define DSP_FUNC_H

#include <audio.h>
#include <audioproto.h>
#include "dsp.h"
#include "physdevice.h"

#ifndef	__DSP56K__
int dspLoad(JvRegs *lp, char *file);

/*
 * These subroutines are in 'dsp.c'
 */
CARD32 *dspGetCodecTimePtr(jvPhysDevice *pDev);
CARD32  *dspGetHifiTimePtr(jvPhysDevice *pDev);
CARD32 *dspGetHifiRecBuf(jvPhysDevice *pDev, int flag);
CARD32 *dspGetHifiPlayBuf(jvPhysDevice *pDev, int flag);
void dspSetBufs(jvPhysDevice *pDev, int hifilen);
CARD32 *dspGetCodecPlayBuf(jvPhysDevice *pDev, int flag);
CARD32 *dspGetCodecRecBuf(jvPhysDevice *pDev, int flag);
extern	ABool dspCraftEvent(ATime etime, int devNum, JvEvent *ep, aEvent *aep);
#endif

#endif
