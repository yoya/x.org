
/************************************************************************
 *
 * cit.c --
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
#define NEED_REPLIES
#define NEED_EVENTS

#include "Xproto.h"
#include "windowstr.h"
#include "fontstruct.h"
#include "gcstruct.h"
#include "dixfontstr.h"
#include "selection.h"
#include "colormapst.h"
#include "cursorstr.h"
#include "scrnintstr.h"
#include "opaque.h"
#include "input.h"
#include "servermd.h"

#include "cit.h"
#include "mtxlock.h"
#include "message.h"
#include "../os/osdep.h"

extern X_COND_TYPE  ServerCond;
extern X_MUTEX_TYPE ConnectionMutex;
extern X_COND_TYPE ConnectionCond;


extern int ClientInitialConnection();
extern Bool CreateClientResources();
extern unsigned short MinorOpcodeOfRequest();
extern void DeleteClientFromAnySelections();
extern void ReleaseActiveGrabs();

BOOL terminateAtReset = FALSE;

X_KEY_TYPE ClientKey;

static Bool ClientKeyCreated = FALSE;

static int nClients;	      /* number active clients */
int nextFreeClientID; /* always MIN free client ID */

static void CloseDownClientInputThread();

#define MAJOROP ((xReq *)client->requestBuffer)->reqType


/***********************************************************************/

static
void *
ClientInputThread(ospriv)
    pointer ospriv;

/*
 * Summary
 *
 *      Authorize the client, setup local message buffer, and
 *      dispatch the protocol request.
 *
 ***********************************************************************/

{
    register int result;
    register MsgBufferPtr localMsgBuffer;
    ClientPtr client;
    int status;


    result = ClientInitialConnection(ospriv, &client);
    if (result < 0)
    {
        ErrorF("ClientInputThread: initial connection failed. \n");
	X_THREAD_EXIT(&status);
    }

    localMsgBuffer = RegisterLocalMessageBufferForThread();
    if (!localMsgBuffer)
    {
        ErrorF("ClientInputThread: local buffer allocation failed. \n");
	CloseDownConnection(client);
	X_THREAD_EXIT(&status);
    }
    client->msgBuffer = (pointer) localMsgBuffer;

    X_CLEANUP_PUSH(CloseDownClientInputThread, client);

    while (1)
    {
	READ_REQUEST_FROM_CLIENT(result, client);

        client->sequence++;
 
#ifdef DEBUG
        if (client->requestLogIndex == MAX_REQUEST_LOG)
        {
            client->requestLogIndex = 0;
        }
        client->requestLog[client->requestLogIndex] = MAJOROP;
        client->requestLogIndex++;
#endif
        if (result > (MAX_BIG_REQUEST_SIZE << 2))
	{
            result = BadLength;
	}
        else
	{
            result = (* client->requestVector[MAJOROP])(client);
	}
 
        if (result != Success)
        {
            if (client->noClientException != Success)
	    {
		break;
	    }
            else
	    {
                SendErrorToClient(client, MAJOROP, MinorOpcodeOfRequest(client),
				  client->errorValue, result);
                TransferLocalMessagesToGlobal(localMsgBuffer);
	    }
        }
    }

close_down:
    X_CLEANUP_POP( TRUE );
    X_THREAD_EXIT(&status);
}

/***********************************************************************/

Bool
CreateClientInputThread(ospriv)
    pointer ospriv;

/*
 * Summary
 *
 *      Wrapper to create and detach the CIT.
 *
 ***********************************************************************/

{
    X_THREAD_TYPE client_thread;

    /* Start the client input thread */
    if (X_THREAD_CREATE(&client_thread, X_CIT_ATTR_DEFAULT,
			 ClientInputThread, ospriv))
    {
        ErrorF("ClientInputThread: thread create failed\n");
        return (FALSE);
    }
    ((OsCommPtr)ospriv)->thread = client_thread;
    X_THREAD_DETACH(&client_thread);
    return (TRUE);
}

/***********************************************************************/

static 
void
CloseDownClientInputThread(client)
    ClientPtr client;

/*
 * Summary
 *
 *      Close down the current CIT.
 *	Called from the CIT before exiting.
 *
 ***********************************************************************/

{
    int clientIndex;
    MsgBufferPtr localMsgBuffer;


    clientIndex = client->index;
    localMsgBuffer = (MsgBufferPtr)client->msgBuffer;

    /* ungrab server if grabbing client dies */
    POQUngrabServer(client);

    MTX_LOCK_PENDING_OPERATION_QUEUE(client, CM_XKillClient); /* lock it all */

    DeleteClientFromAnySelections(client);
    ReleaseActiveGrabs(client);
    DeleteClientFontStuff(client);

    CloseDownConnection(client); 
    client->clientGone = TRUE; /* so events aren't sent to client */
    
    if (client->closeDownMode == DestroyAll)
    {
	FreeClientResources(client);

        X_MUTEX_LOCK(&ConnectionMutex);
        if (--nClients == 0)
        {
	    if (terminateAtReset)
	    {
	        SignalServerTerminate();
	    }
	    else
	    {
		SignalServerReset();
	    }
	    X_COND_SIGNAL(&ConnectionCond);
	}
        X_MUTEX_UNLOCK(&ConnectionMutex);
    }
    else
    {
        X_MUTEX_LOCK(&ConnectionMutex);
        if (--nClients == 0)
	{
	    X_COND_SIGNAL(&ConnectionCond);
	}
        X_MUTEX_UNLOCK(&ConnectionMutex);
    }

    MTX_UNLOCK_PENDING_OPERATION_QUEUE(client);

    TransferLocalMessagesToGlobal(localMsgBuffer);
    FlushAllMessages(NULL);
    UnregisterLocalMessageBufferForThread(clientIndex, localMsgBuffer);

    FreeOsBuffers(client->osPrivate);
    xfree(client->osPrivate);
    client->osPrivate = (pointer)NULL;
    if (client->closeDownMode == DestroyAll)
    {
        X_MUTEX_LOCK(&ConnectionMutex);
	RECALCULATE_NEXT_FREE_CLIENT(clientIndex);
        X_MUTEX_UNLOCK(&ConnectionMutex);
        xfree(client);
    }
}

/***********************************************************************/

void
CloseDownClient(client)
    register ClientPtr client;

/*
 * Summary
 *
 *  Simply close the connection and let the CIT clean up.
 *  The CIT will call CloseDownClientInputThread(client)
 *
 *  Client can either mark his resources destroy or retain.  If retained and
 *  then killed again, the client is really destroyed.
 *
 *  The calling routine should have already locked the POQ.
 *
 *
 ***********************************************************************/

{
    if (client && (!client->clientGone))
    {
        CloseDownConnection(client);
    }
    else /* client is already gone */
    {
        /* client was previously closed down with resources retained */

        /* really kill resources this time */
        FreeClientResources(client);

        X_MUTEX_LOCK(&ConnectionMutex);
        RECALCULATE_NEXT_FREE_CLIENT(client->index);
        X_MUTEX_UNLOCK(&ConnectionMutex);

        xfree(client);
    }
}

/***********************************************************************/

void
KillAllClients()

/*
 * Summary
 *
 *
 ***********************************************************************/

{
    int i, maxclients;
    ClientPtr client;

    X_MUTEX_LOCK(&ConnectionMutex);
    maxclients = currentMaxClients;
    X_MUTEX_UNLOCK(&ConnectionMutex);

    for (i=1; i<maxclients; i++)
    {
	client = clients[i];
        if (client && (!client->clientGone))
        {
            CloseDownConnection(client);
        }
    }
    /* 
     * wait for all CITs to exit 
     */
    X_MUTEX_LOCK(&ConnectionMutex);
    while (nClients)
    {
	X_COND_WAIT(&ConnectionCond, &ConnectionMutex);
    }
    X_MUTEX_UNLOCK(&ConnectionMutex);

    /* 
     * now that all CITs are gone, close any remaining clients with
     * retained resources 
     */
    for (i=1; i<maxclients; i++)
    {
        if (client = clients[i])
        {
            CloseDownClient(client);
        }
    }
}

/***********************************************************************/

void
CloseDownRetainedResources()

/*
 * Summary
 *
 *    Find all clients that are gone and have terminated in RetainTemporary
 *    and  destroy their resources.
 *
 ***********************************************************************/

{
    register int i, maxclients;
    register ClientPtr client;

    X_MUTEX_LOCK(&ConnectionMutex);
    maxclients = currentMaxClients;
    X_MUTEX_UNLOCK(&ConnectionMutex);

    for (i=1; i<maxclients; i++)
    {
        client = clients[i];
        if (client && (client->closeDownMode == RetainTemporary)
            && (client->clientGone))
	{
            FreeClientResources(client);

            X_MUTEX_LOCK(&ConnectionMutex);
            RECALCULATE_NEXT_FREE_CLIENT(client->index);
            X_MUTEX_UNLOCK(&ConnectionMutex);

            xfree(client);
	}
    }
}

/***********************************************************************/

ClientPtr
NextAvailableClient(ospriv)
    pointer ospriv;

/*
 * Summary
 *
 * OS dependent portion can't assign client id's because of CloseDownModes.
 * Returns NULL if there are no free clients.
 *
 * Called from ClientInitialConnection() holding the ConnectionMutex.
 *
 ***********************************************************************/

{
    register int i;
    register ClientPtr client;


    i = nextFreeClientID;
    clients[i] = client = (ClientPtr)xalloc(sizeof(ClientRec));
    if (!client)
    {
        return (ClientPtr)NULL;
    }
    InitClient(client, i, ospriv);
    if (!CreateClientResources(client))
    {
        xfree(client);
        return (ClientPtr)NULL;
    }
    if (i == currentMaxClients)
    {
        currentMaxClients++;
    }
    while ((nextFreeClientID < MAXCLIENTS) && clients[nextFreeClientID])
    {
        nextFreeClientID++;
    }
    nClients++;

    return(client);
}

/***********************************************************************/

void
InitializeClientKey(client)
    ClientPtr client;
/*
 * Summary
 *
 *      Create the serialnumber key if not yet created. 
 *      The ClientKey for a thread  will be a pointer to the thread's 
 *	client record (even the DIT has a dummy client record). 
 *
 ***********************************************************************/

{
    if (!ClientKeyCreated)
    {
        if (X_KEY_CREATE(&ClientKey, NULL))
	{
	    FatalError("InitializeClientKey: ClientKey create failed \n");
	}
        ClientKeyCreated = TRUE;
    }

    INITIALIZE_CLIENT_SERIAL_NUMBER(client);
    X_SETSPECIFIC(ClientKey, client);
}

/***********************************************************************/
  
void
InitClientGlobals()

/*
 * Summary
 *
 ***********************************************************************/

{
    nextFreeClientID = 1;
    nClients = 0;
}

/***********************************************************************/
  
void
InitClient(client, i, ospriv)
    ClientPtr client;
    int i;
    pointer ospriv;

/*
 * Summary
 *
 ***********************************************************************/

{
    client->index = i;
    client->sequence = 0;
    client->clientAsMask = ((Mask)i) << CLIENTOFFSET;
    client->clientGone = FALSE;
    if (i)
    {
      client->closeDownMode = DestroyAll;
    }
    else
    {
      client->closeDownMode = RetainPermanent;
    }
    client->numSaved = 0;
    client->msgBuffer = NULL;
    client->saveSet = (pointer *)NULL;
    client->noClientException = Success;
#ifdef DEBUG
    client->requestLogIndex = 0;
#endif
    client->osPrivate = ospriv;
    client->swapped = FALSE;
    client->big_requests = FALSE;

    POQInitClient(client);
    InitializeClientKey(client);
}

/***********************************************************************/
  
unsigned long
GetNextSerialNumber()

/*
 * Summary
 *
 *      Increment the calling CIT's drawable count in the 
 *      serial number. Note: The client id never changes once initialized..
 *
 ***********************************************************************/

{
    ClientPtr client;
    unsigned long passed;

    X_GET_CLIENT_REC(client);

    NEXT_CLIENT_SERIAL_NUMBER(passed, client);

    return (passed);
}

/***********************************************************************/

void
MarkClientException(client)
    ClientPtr client;

/*
 * Summary
 *
 ***********************************************************************/

{
    client->noClientException = -1;
}
#endif /* XTHREADS */
