
/************************************************************************
 *
 * dit.c --
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
#include <sys/types.h>
#include <sys/param.h>
#include <errno.h>
#include <sys/socket.h>
#include <signal.h>
#include "osdep.h"
#include "dixstruct.h"
#include "scrnintstr.h"
#include "mtxlock.h"
#include "message.h"
#include "cit.h"


#define CHANGE_SELECT_MASK  1
#define SET_SCREEN_SAVER    2
#define DESTROY_DIT 	    3

extern X_MUTEX_TYPE DITMutex;
extern X_COND_TYPE DITCondition;

extern int errno;

void* DeviceInputThread();
void SetIoHandler();
static void (* ddxIoHandler)();

static int deviceWakeupFDs[2];
static int numberOfDITs = 0;
long LastSelectMask[mskcnt];    /* mask returned from last select call */
long EnabledDevices[mskcnt];    /* mask for input devices that are on */


/***********************************************************************/

void
InitDeviceInputThread()

/*
 * Summary
 *
 * 	Initialize globals used by the device input thread.
 *
 ***********************************************************************/
{
    SetIoHandler(NoopDDA);

    CLEARBITS(EnabledDevices);

    deviceWakeupFDs[0] = -1;
    deviceWakeupFDs[1] = -1;

    numberOfDITs = 0;
}

/***********************************************************************/

void
CreateDeviceInputThread()

/*
 * Summary
 *
 * 	Create the device input thread.
 *
 ***********************************************************************/
{
    X_THREAD_TYPE deviceThread;

    /* 
     * This socket is used to wakeup the DIT from a select so that the
     * select mask can be updated or the DIT can be told to exit.  
     * A single byte is sent to the DIT from AddEnabledDevice()
     * and RemoveEnabledDevice() on deviceWakeupFDs[0]. The DIT will
     * select on deviceWakeupFDs[1] and read the byte from the
     * socket.  It will then look at the byte to determine whether
     * to reset or exit.
     */
    if (socketpair(PF_UNIX, SOCK_STREAM, 0, deviceWakeupFDs) < 0)
    {
	FatalError("CreateDeviceInputThread: socketpair failed. \n");
    }
    /*
     * Add deviceWakeupFDs[1] to the select mask in the DIT.
     */
    AddEnabledDevice(deviceWakeupFDs[1]);

    /* 
     * Create the device input thread.
     */
    if (X_THREAD_CREATE(&deviceThread, X_DIT_ATTR_DEFAULT,
                        DeviceInputThread, (void *)numberOfDITs))
    {
        FatalError("CreateDeviceInputThread: thread create failed. \n");
    }
    numberOfDITs++;
    X_THREAD_DETACH(&deviceThread);
}

/***********************************************************************/

static void
SignalDeviceInputThread(signalToSend)
    char signalToSend;

/*
 * Summary
 *
 * 	Signal the DIT to wakeup and either reset or exit.
 *
 ***********************************************************************/
{
    if (deviceWakeupFDs[0] != -1)
    {
        /*
         * Write one byte to the file descriptor to wake up the DIT.
         */
        if (write(deviceWakeupFDs[0], &signalToSend, 1) == -1)
        {
	    ErrorF("SignalDeviceInputThread: write failed. \n");
        }
    }
}

/***********************************************************************/

void
DITSetScreenSaver()

/*
 * Summary
 *
 * 	Tell the DIT to reset the screen saver.
 *
 ***********************************************************************/
{
    SignalDeviceInputThread(SET_SCREEN_SAVER);
}

/***********************************************************************/

void
DestroyDeviceInputThread()

/*
 * Summary
 *
 * 	Destroy the DIT.
 *
 ***********************************************************************/
{
    X_MUTEX_LOCK(&DITMutex);
    /* 
     * Signal the DIT to exit.
     */
    SignalDeviceInputThread(DESTROY_DIT);

    /* 
     * Wait for a signal to indicate the DIT has shut down.
     */
    X_COND_WAIT(&DITCondition, &DITMutex);
    X_MUTEX_UNLOCK(&DITMutex);

    /*
     * Remove the fd from the EnabledDevices select mask.
     */
    RemoveEnabledDevice(deviceWakeupFDs[1]);

    /*
     * Close the socketpair created in CreateDeviceInputThread().
     */
    close(deviceWakeupFDs[0]);
    close(deviceWakeupFDs[1]);

    /* 
     * Reinitialize globals.
     */
    InitDeviceInputThread();
}

/***********************************************************************/

AddEnabledDevice(fd)
    int fd;
/*
 * Summary
 *
 * 	Enable a device by adding its fd to the DIT select mask.
 *	Wake up the DIT to tell it the select mask has changed.
 *
 ***********************************************************************/
{
    X_MUTEX_LOCK(&DITMutex);
    BITSET(EnabledDevices, fd);
    X_MUTEX_UNLOCK(&DITMutex);
    SignalDeviceInputThread(CHANGE_SELECT_MASK);
}

/***********************************************************************/

RemoveEnabledDevice(fd)
    int fd;
/*
 * Summary
 *
 * 	Remove an enabled device by removing its fd from the DIT select mask.
 *	Wake up the DIT to tell it the select mask has changed.
 *
 ***********************************************************************/
{
    X_MUTEX_LOCK(&DITMutex);
    BITCLEAR(EnabledDevices, fd);
    X_MUTEX_UNLOCK(&DITMutex);
    SignalDeviceInputThread(CHANGE_SELECT_MASK);
}

/***********************************************************************/

void
SetIoHandler(ioHandler)
    void (* ioHandler)();

/*
 * Summary
 *
 * 	Set the DDX IO handler.
 *	This is provided for DDX implementors who wish to use
 *	the mi software cursor code.  It requires that the current
 *	pointer location be updated before aquiring the POQ lock
 *	so that the correct cursor region gets locked.
 *
 ***********************************************************************/
{
    ddxIoHandler = ioHandler;
}

/***********************************************************************/

void*
DeviceInputThread(DITindex)
    int DITindex;

/*
 * Summary
 *
 * 	Reads input from devices by calling ProcessInputEvents().
 *	Sets the screen saver time out.
 *
 ***********************************************************************/
{
    ClientPtr client;               /* pointer to dummy client */
    int result;			    /* error status from select and exit */
    char signalSent;		    /* wakeup byte sent from CIT */

    /* The DIT uses a dummy client record to hold the serialNumber and
     * the local message buffer. 
     */
    client = (ClientPtr)xalloc(sizeof(ClientRec));
    if (!client)
    {
        FatalError("couldn't create DIT client");
    }
    InitClient(client, MAXCLIENTS + DITindex, NULL);

    if (!(client->msgBuffer = (pointer)RegisterLocalMessageBufferForThread()))
    {
	FatalError("DeviceInputThread: local buffer allocation failed. \n");
    }
    UnblockSigalrmSignal();

    POQ_SET_NULL_CONFLICT(client);

    POQLock(client, CM_XDeviceInputThread);
    SetScreenSaver(client);
    POQUnlock(client);

    while (1)
    {
        X_MUTEX_LOCK(&DITMutex);
	COPYBITS(EnabledDevices, LastSelectMask);
        X_MUTEX_UNLOCK(&DITMutex);

        result = select(MAXSOCKS, (int *)LastSelectMask, (int *)NULL, 
			(int *)NULL, (int *)NULL);
	if (result < 0)
	{
            if (errno == EINTR)
            {
                continue;
            }
            else
            {
                ErrorF("DeviceInputThread: select failed. \n");
                ErrorF("result = %d, errno = %d \n", result, errno);
                break;
            }
	}

	if (GETBIT(LastSelectMask, deviceWakeupFDs[1]))
	{
	    /* 
	     * Read the wakeup byte from the socket.
	     */
	    read(deviceWakeupFDs[1], &signalSent, 1);
	    BITCLEAR(LastSelectMask, deviceWakeupFDs[1]);

	    if (signalSent == CHANGE_SELECT_MASK)
	    {
		continue;
	    }
	    else if (signalSent == SET_SCREEN_SAVER)
	    {
		POQLock(client, CM_XDeviceInputThread);
		SetScreenSaver(client);
		POQUnlock(client);
		continue;
	    }
	    else /* if (signalSent == DESTROY_DIT) */
	    {
	        break;
	    }
	}
#ifdef USE_SOFTWARE_CURSOR
	(* ddxIoHandler)();
#endif
	POQLock(client, CM_XDeviceInputThread);

        ProcessInputEvents();
	SetScreenSaver(client);

	POQUnlock(client);

        FlushAllMessages(NullClient);
    }
    UnregisterLocalMessageBufferForThread(client->index, client->msgBuffer);
    xfree(client);

    X_MUTEX_LOCK(&DITMutex);
    X_COND_SIGNAL(&DITCondition);
    X_MUTEX_UNLOCK(&DITMutex);

    X_THREAD_EXIT(&result);
}

#endif /*XTHREADS*/
