/*
 * $XConsortium: omron.h,v 1.3 94/01/11 20:47:39 rob Exp $
 *
 * Copyright 1992, 1993 Data General Corporation;
 * Copyright 1991, 1992, 1993 OMRON Corporation  
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting documentation, and
 * that neither the name OMRON or DATA GENERAL be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission of the party whose name is to be used.  Neither 
 * OMRON or DATA GENERAL make any representation about the suitability of this
 * software for any purpose.  It is provided "as is" without express or 
 * implied warranty.  
 *
 * OMRON AND DATA GENERAL EACH DISCLAIM ALL WARRANTIES WITH REGARD TO THIS
 * SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS,
 * IN NO EVENT SHALL OMRON OR DATA GENERAL BE LIABLE FOR ANY SPECIAL, INDIRECT
 * OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
 * USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
 * OF THIS SOFTWARE.
 *
 */
#include <stdio.h>

#include <fcntl.h>
#ifdef	uniosu
# include <termio.h>
#endif
#include <sys/types.h>
#include <sys/param.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/signal.h>

#ifdef	uniosu
# include <sys/sys9100.h>
# include <sys/kbms.h>
#else /* not uniosu */
# if defined(luna88k) || defined(luna2)
#  include <machine/sysomron.h>
#  include <dev/kbms.h>
# else /* uniosb */
#  include <om68k/sysomron.h>
#  include <om68kdev/kbms.h>
# endif
#endif

#include <errno.h>

#include "X.h"
#include "Xmd.h"

#define	NEED_EVENTS
#include "Xproto.h"

#include "osdep.h"
#include "misc.h"

#include "scrnintstr.h"
#include "screenint.h"

#include "servermd.h"

#include "input.h"
#include "inputstr.h"

#include "mipointer.h"

#ifdef XTHREADS
#include "colormapst.h"
#endif /* XTHREADS */

#include "mfb.h"

#define SET_KBTIME 1
#define SET_MSTIME 0

/* libc */
extern int	open();
extern int  read();
extern int  write();
extern int	ioctl();
extern int	close();
extern int	getpagesize();
extern int	getpid();
extern char *valloc();
extern int	free();
extern int	mmap();
extern int	munmap();
extern int	exit();
extern int  sysomron();
extern int  fcntl();
extern int  ffs();
extern int  sigblock();
extern int  sigsetmask();

/* dix */
extern void NoopDDA();
extern int	AddScreen();
extern int	AllocateScreenPrivateIndex();
#ifdef	UNUSE_SIGIO_SIGNAL
extern Bool RegisterBlockAndWakeupHandlers();
#endif

/* os */
extern int  AddEnabledDevice();
extern int  RemoveEnabledDevice();

/* ddx/mi */
extern Bool mieqInit();
extern int  mieqProcessInputEvents();
extern int  mieqEnqueue();
extern void miRegisterPointerDevice();
extern Bool miDCInitialize();

/* ddx/omron */
extern void omronSetLastEventTime();
extern Bool omronParseFbArg();
extern void omronSetIoHandler();
extern void omronInitEventPrvRec();
extern void omronSetDriverTimeMode();

#ifndef	UNUSE_SIGIO_SIGNAL
extern void omronSigIOHandler();
#else
extern void omronWakeupProc();
#endif

extern void omronEnqueueEvents();
#ifndef UNUSE_DRV_TIME
extern void omronEnqueueTEvents();
#endif

/* libc */
#ifdef X_NOT_STDC_ENV
extern int errno;
#endif

/* os */
extern long EnabledDevices[];
extern long LastSelectMask[];

/* ddx/omron */
extern int omronScreenIndex;	
extern int monitorResolution;

extern int  scrw;
extern int  scrh;
extern int  QueryFb;
extern char *fb_type;

extern int lastEventTime;

extern CARD8 *omronKeyModMap[];
extern KeySymsRec omronKeySyms[];

