 
/************************************************************************
 *
 * mtxlock.c --
 *
 *
 * Copyright 1992, 1993 Data General Corporation;
 * Copyright 1992, 1993 OMRON Corporation  
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
 ************************************************************************/

#ifdef XTHREADS

#include "X.h"
#include "Xproto.h"
#include "windowstr.h"
#include "fontstruct.h"
#include "dixfontstr.h"
#include "gcstruct.h"
#include "selection.h"
#include "colormapst.h"
#include "cursorstr.h"
#include "scrnintstr.h"
#include "opaque.h"
#include "input.h"
#include "servermd.h"
#include "extnsionst.h"
#include "mtxlock.h"


X_MUTEX_TYPE PrintfMutex;
X_MUTEX_TYPE DeviceMutex;
X_MUTEX_TYPE MiArcMutex;
X_MUTEX_TYPE FontLockMutex;
X_MUTEX_TYPE FPEFuncLockMutex;
X_MUTEX_TYPE ScratchGCMutex;

X_MUTEX_TYPE ConnectionMutex;
X_COND_TYPE  ConnectionCond;

X_MUTEX_TYPE DITMutex;
X_COND_TYPE  DITCondition;

X_MUTEX_TYPE ServerMutex;
X_COND_TYPE  ServerCond;



/***********************************************************************/

void
InitializeMTXLocks()

/*
 * Summary
 *
 *      Initialize global mutex and condition variables.
 *
 ***********************************************************************/
{
#ifdef CTHREADS
    pthread_init(PTHREAD_PROCSYNC);
#endif

    X_MUTEX_INIT(&PrintfMutex, X_MUTEX_ATTR_DEFAULT);
    X_MUTEX_INIT(&DeviceMutex, X_MUTEX_ATTR_DEFAULT);
    X_MUTEX_INIT(&MiArcMutex, X_MUTEX_ATTR_DEFAULT);
    X_MUTEX_INIT(&FontLockMutex, X_MUTEX_ATTR_DEFAULT);
    X_MUTEX_INIT(&FPEFuncLockMutex, X_MUTEX_ATTR_DEFAULT);
    X_MUTEX_INIT(&ScratchGCMutex, X_MUTEX_ATTR_DEFAULT);

    X_MUTEX_INIT(&ConnectionMutex, X_MUTEX_ATTR_DEFAULT);
    X_COND_INIT(&ConnectionCond, X_COND_ATTR_DEFAULT);

    X_MUTEX_INIT(&DITMutex, X_MUTEX_ATTR_DEFAULT);
    X_COND_INIT(&DITCondition, X_COND_ATTR_DEFAULT);

    X_MUTEX_INIT(&ServerMutex, X_MUTEX_ATTR_DEFAULT);
    X_COND_INIT(&ServerCond, X_COND_ATTR_DEFAULT);

    X_SET_SYMNAME(&PrintfMutex,"Printf_M");
    X_SET_SYMNAME(&DeviceMutex,"Device_M ");
    X_SET_SYMNAME(&MiArcMutex,"MiArc_M");
    X_SET_SYMNAME(&FontLockMutex,"Font_M ");
    X_SET_SYMNAME(&FPEFuncLockMutex,"FPEFunc_M ");
    X_SET_SYMNAME(&ScratchGCMutex,"ScratchGC_M ");

    X_SET_SYMNAME(&ConnectionMutex,"Connection_M ");
    X_SET_SYMNAME(&ConnectionCond,"Connection_Cond ");

    X_SET_SYMNAME(&DITMutex,"DIT_M ");
    X_SET_SYMNAME(&DITCondition,"DIT_Cond ");

    X_SET_SYMNAME(&ServerMutex,"Server_M ");
    X_SET_SYMNAME(&ServerCond,"Server_Cond ");
}

/***********************************************************************/

void
LockDevices()
{
    X_MUTEX_LOCK(&DeviceMutex);
}

/***********************************************************************/

void
UnlockDevices()
{
    X_MUTEX_UNLOCK(&DeviceMutex);
}

/***********************************************************************/

void
LockDevicesAndPOQ(client, conflictMask)
    ClientPtr client;
    Mask conflictMask;
{
    MTX_LOCK_PENDING_OPERATION_QUEUE(client, conflictMask);
    LockDevices();
    return;
}

/***********************************************************************/

void
UnlockDevicesAndPOQ(client)
    ClientPtr client;
{
    UnlockDevices();
    MTX_UNLOCK_PENDING_OPERATION_QUEUE(client);
}

#endif /* XTHREADS */
