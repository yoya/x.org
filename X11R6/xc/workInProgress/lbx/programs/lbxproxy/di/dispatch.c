/* $XConsortium: dispatch.c,v 1.7 94/12/01 20:47:30 mor Exp $ */
/*
 * $NCDOr: dispatch.c,v 1.2 1993/11/19 21:28:48 keithp Exp keithp $
 * $NCDId: @(#)dispatch.c,v 1.27 1994/11/18 20:35:09 lemke Exp $
 *
 * Copyright 1992 Network Computing Devices
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of NCD. not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  NCD. makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * NCD. DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL NCD.
 * BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN 
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Author:  Keith Packard, Network Computing Devices
 */

#include "X.h"
#define NEED_REPLIES
#define NEED_EVENTS
#include "assert.h"
#include "Xproto.h"
#include "opaque.h"
#include "lbx.h"
#include "lbxdata.h"
#include "wire.h"
#include "dixstruct.h"
#include "input.h"
#include "servermd.h"
#define _XLBX_SERVER_
#include "lbxstr.h"
#include "swap.h"
#include "lbxext.h"

extern int (* InitialVector[3]) ();
extern int (* ProcVector[256]) ();
extern void WriteSConnSetupPrefix();
extern char *ClientAuthorized();
extern Bool InsertFakeRequest();
static void KillAllClients();
extern void ProcessWorkQueue();
extern Bool lbxUseTags;

static int nextFreeClientID; /* always MIN free client ID */

static int	nClients;	/* number active clients */

char *display_name = 0;
char dispatchException = 0;
char isItTimeToYield;
Bool lbxUseLbx = TRUE;
Bool lbxCompressImages = TRUE;
Bool lbxDoShortCircuiting = TRUE;
Bool lbxDoLbxGfx = TRUE;

#define MAJOROP ((xReq *)client->requestBuffer)->reqType

int
Dispatch ()
{
    register int        *clientReady;     /* array of request ready clients */
    register int	result;
    register ClientPtr	client;
    register int	nready;

    nextFreeClientID = 1;
/*    InitSelections(); */
    nClients = 0;

    clientReady = (int *) xalloc(sizeof(int) * MaxClients);
    if (!clientReady)
	FatalError("couldn't create client ready array");

    if (!ConnectToServer (display_name))
	FatalError("couldn't connect to X server");
    SendInitLBXPackets();
    
    while (!dispatchException)
    {
	nready = WaitForSomething(clientReady);

       /***************** 
	*  Handle events in round robin fashion, doing input between 
	*  each round 
	*****************/

	while (!dispatchException && (--nready >= 0))
	{
	    client = clients[clientReady[nready]];
	    if (! client)
	    {
		/* KillClient can cause this to happen */
		continue;
	    }
#ifdef NOTDEF
	    /* XXX what to do about grabs ? */
	    /* GrabServer activation can cause this to be true */
	    if (grabbingClient && (client != onlyClient))
		break;
#endif
	    isItTimeToYield = FALSE;
 
            requestingClient = client;
	    while (!isItTimeToYield)
	    {
		/* now, finally, deal with client requests */

	        result = ReadRequestFromClient(client);
	        if (result <= 0) 
	        {
		    if (result < 0)
			CloseDownClient(client);
		    break;
	        }

		client->sequence++;
#ifdef DEBUG
		if (client->requestLogIndex == MAX_REQUEST_LOG)
		    client->requestLogIndex = 0;
		client->requestLog[client->requestLogIndex] = MAJOROP;
		client->requestLogIndex++;
#endif
                LBXSequenceNumber(client)++;
		if (result > (MAX_REQUEST_SIZE << 2))
		    result = BadLength;
		else
		    result = (* client->requestVector[MAJOROP])(client);
	    
#ifdef NOTDEF
		if (result != Success) 
		{
		    if (client->noClientException != Success)
                        CloseDownClient(client);
                    else
		        SendErrorToClient(client, MAJOROP,
					  MinorOpcodeOfRequest(client),
					  client->errorValue, result);
		    break;
	        }
#endif
	    }
	    FlushAllOutput();
	}
    }
    KillAllClients();
    LbxCleanupSession();
    xfree (clientReady);
    dispatchException &= ~DE_RESET;
    return (dispatchException & DE_TERMINATE);
}

void
SendErrorToClient(client, majorCode, minorCode, resId, errorCode)
    ClientPtr client;
    unsigned int majorCode;
    unsigned int minorCode;
    XID resId;
    int errorCode;
{
    xError      rep;

    rep.type = X_Error;
    rep.sequenceNumber = client->sequence;
    rep.errorCode = errorCode;
    rep.majorCode = majorCode;
    rep.minorCode = minorCode;
    rep.resourceID = resId;

    WriteToClient(client, sizeof(rep), (char *) &rep);
}

/************************
 * int NextAvailableClient(ospriv)
 *
 * OS dependent portion can't assign client id's because of CloseDownModes.
 * Returns NULL if there are no free clients.
 *************************/

extern unsigned long	StandardRequestLength ();

ClientPtr
NextAvailableClient(ospriv)
    pointer ospriv;
{
    register int i;
    register ClientPtr client;
    xReq data;

    i = nextFreeClientID;
    if (i == MAXCLIENTS)
	return (ClientPtr)NULL;
    clients[i] = client = (ClientPtr)xalloc(sizeof(ClientRec));
    if (!client)
	return (ClientPtr)NULL;
    if (!MakeLBXStuff(client)) {
    	xfree(client);
        return (ClientPtr)NULL;
    }
    client->index = i;
    client->sequence = 0; 
    client->clientAsMask = ((Mask)i) << CLIENTOFFSET;
    client->closeDownMode = DestroyAll;
    client->clientGone = FALSE;
    client->awaitingSetup = TRUE;
    client->saveSet = (pointer *)NULL;
    client->noClientException = Success;
    client->public.requestLength = StandardRequestLength;
#ifdef DEBUG
    client->requestLogIndex = 0;
#endif
    client->requestVector = InitialVector;
    client->osPrivate = ospriv;
    client->server = servers[0];    /* XXX want to use multiple servers */
    client->swapped = FALSE;
    LBXCacheSafe(client) = FALSE;
    LBXCanDelayReply(client) = FALSE;
    if (!InitClientResources(client))
    {
	xfree(client);
	return (ClientPtr)NULL;
    }
    if (i == currentMaxClients)
	currentMaxClients++;
    while ((nextFreeClientID < MAXCLIENTS) && clients[nextFreeClientID])
	nextFreeClientID++;
    if (client->server)
    {
	data.reqType = 1;
	data.length = (sz_xReq + sz_xConnClientPrefix) >> 2;
	if (!InsertFakeRequest(client, (char *)&data, sz_xReq))
	{
	    xfree (client);
	    return (ClientPtr) NULL;
	}
    }
    return(client);
}

int
ProcInitialConnection(client)
    register ClientPtr client;
{
    REQUEST(xReq);
    register xConnClientPrefix *prefix;
    int whichbyte = 1;

    prefix = (xConnClientPrefix *)((char *)stuff + sz_xReq);
    if ((prefix->byteOrder != 'l') && (prefix->byteOrder != 'B'))
	return (client->noClientException = -1);
    if (((*(char *) &whichbyte) && (prefix->byteOrder == 'B')) ||
	(!(*(char *) &whichbyte) && (prefix->byteOrder == 'l')))
    {
	client->swapped = TRUE;
	SwapConnClientPrefix(prefix);
    }
    stuff->reqType = 2;
    stuff->length += ((prefix->nbytesAuthProto + 3) >> 2) +
		     ((prefix->nbytesAuthString + 3) >> 2);
    if (client->swapped) {
	swaps(&stuff->length, whichbyte);
    }
    ResetCurrentRequest(client);
    return (client->noClientException);
}

int
ProcEstablishConnection(client)
    register ClientPtr client;
{
    char       *reason,
               *auth_proto,
               *auth_string;
    register xLbxConnClientPrefix *prefix;
    register xWindowRoot *root;
    register int i;
    int         len;

    REQUEST(xReq);

    prefix = (xLbxConnClientPrefix *) ((char *) stuff + sz_xReq);
    auth_proto = (char *) prefix + sz_xConnClientPrefix;
    auth_string = auth_proto + ((prefix->nbytesAuthProto + 3) & ~3);
    if ((prefix->majorVersion != X_PROTOCOL) ||
	    (prefix->minorVersion != X_PROTOCOL_REVISION))
	reason = "Protocol version mismatch";
    else
	reason = ClientAuthorized(client,
				  (unsigned short) prefix->nbytesAuthProto,
				  auth_proto,
				  (unsigned short) prefix->nbytesAuthString,
				  auth_string);

#ifdef notyet
    if (reason) {
	xConnSetupPrefix csp;
	char        pad[3];

	csp.success = xFalse;
	csp.lengthReason = strlen(reason);
	csp.length = (csp.lengthReason + 3) >> 2;
	csp.majorVersion = X_PROTOCOL;
	csp.minorVersion = X_PROTOCOL_REVISION;
	if (client->swapped)
	    WriteSConnSetupPrefix(client, &csp);
	else
	    (void) WriteToClient(client, sz_xConnSetupPrefix, (char *) &csp);
	(void) WriteToClient(client, (int) csp.lengthReason, reason);
	if (csp.lengthReason & 3)
	    (void) WriteToClient(client, (int) (4 - (csp.lengthReason & 3)),
				 pad);
	return (client->noClientException = -1);
    }
#endif

    nClients++;
    client->requestVector = ProcVector;
    client->sequence = 0;
    LBXSequenceNumber(client) = 0;
    /* wait for X server to kill client */
    client->closeDownMode = RetainPermanent;

    /*
     * NewClient outputs the LbxNewClient request header - have to follow it
     * up with the setup connection info.
     */
    /* length is still swapped */
    if (client->swapped) {
	swaps(&stuff->length, i);
	/* put data back to the way server will expect it */
	SwapConnClientPrefix((xConnClientPrefix *) prefix);
    }
    len = (stuff->length << 2) - sz_xReq;
    if (!NewClient(client, len))
	return (client->noClientException = -1);
    prefix->useTag = lbxUseTags;
    WriteToServer(clients[0], len, (char *) prefix);

    /*
     * Can't allow any requests to be passed on to the server until the
     * connection setup reply has been received.
     */
    IgnoreClient(client);

    return (client->noClientException);
}

/**********************
 * CloseDownClient
 *
 *  Client can either mark his resources destroy or retain.  If retained and
 *  then killed again, the client is really destroyed.
 *********************/

Bool terminateAtReset = FALSE;

void
CloseDownClient(client)
    register ClientPtr client;
{
    if (!client->clientGone)
    {
	CloseClient (client);
	
	
	/* X server is telling us this client is dead */
	if (client->closeDownMode == DestroyAll)
	{
	    client->clientGone = TRUE;  /* so events aren't sent to client */
	    CloseDownConnection(client);
	    FreeClientResources(client);
	    if (ClientIsAsleep (client))
		ClientSignal (client);
	    if (client->index < nextFreeClientID)
		nextFreeClientID = client->index;
	    clients[client->index] = NullClient;
	    if ((client->requestVector != InitialVector) &&
		(client->server && client->server->serverClient != client) &&
		(--nClients == 0))
	    {
		if (terminateAtReset)
		    dispatchException |= DE_TERMINATE;
		else
		    dispatchException |= DE_RESET;
	    }
	    FreeLBXStuff(client);
	    xfree(client);
	}
	else
	{
	    client->clientGone = TRUE;
	    CloseDownConnection(client);
	    --nClients;
	}
    }
    else
    {
	/* really kill resources this time */
        FreeClientResources(client);
	if (ClientIsAsleep (client))
	    ClientSignal (client);
	if (client->index < nextFreeClientID)
	    nextFreeClientID = client->index;
	clients[client->index] = NullClient;
	FreeLBXStuff(client);
	DeleteClientExtensions(client);
        xfree(client);
	if (nClients == 0)
	{
	    if (terminateAtReset)
		dispatchException |= DE_TERMINATE;
	    else
		dispatchException |= DE_RESET;
	}
    }

    while (!clients[currentMaxClients-1])
      currentMaxClients--;
}

static void
KillAllClients()
{
    int i;
    for (i=1; i<currentMaxClients; i++)
    {
        if (clients[i])
	{
	    clients[i]->closeDownMode = DestroyAll;   
            CloseDownClient(clients[i]);
	}
    }
}

ProcStandardRequest (client)
    ClientPtr	client;
{
    REQUEST(xReq);

    MakeLBXReply(client);
    WriteReqToServer(client, stuff->length << 2, (char *) stuff);
    return Success;
}

/* ARGSUSED */
ProcBadRequest (client)
    ClientPtr	client;
{
    return BadRequest;
}

/* turn off optional features */
AdjustProcVector()
{
    int         i;

    /*
     * to turn off all LBX request reencodings, set all proc vectors to
     * ProcStandardRequest
     */
    if (!lbxUseLbx) {
    	for (i = 1; i < 256; i++) {
            ProcVector[i] = ProcStandardRequest;
        }
    }
    /*
     * to turn off tag support, simply remove the tagged versions of X
     * requests
     */

    if (!lbxUseTags) {
	ProcVector[X_GetModifierMapping] = ProcStandardRequest;
	ProcVector[X_GetKeyboardMapping] = ProcStandardRequest;
	ProcVector[X_QueryFont] = ProcStandardRequest;
	ProcVector[X_ChangeProperty] = ProcStandardRequest;
	ProcVector[X_GetProperty] = ProcStandardRequest;
    }

    if (!lbxCompressImages) {
	ProcVector[X_PutImage] = ProcStandardRequest;
	ProcVector[X_GetImage] = ProcStandardRequest;
    }

    if (!lbxDoShortCircuiting) {
	ProcVector[X_InternAtom] = ProcStandardRequest;
	ProcVector[X_GetAtomName] = ProcStandardRequest;
	ProcVector[X_CreateColormap] = ProcStandardRequest;
	ProcVector[X_FreeColormap] = ProcStandardRequest;
	ProcVector[X_CopyColormapAndFree] = ProcStandardRequest;
	ProcVector[X_AllocColor] = ProcStandardRequest;
	ProcVector[X_AllocNamedColor] = ProcStandardRequest;
	ProcVector[X_FreeColors] = ProcStandardRequest;
	ProcVector[X_LookupColor] = ProcStandardRequest;
    }

    if (!lbxDoLbxGfx) {
	ProcVector[X_CopyArea] = ProcStandardRequest;
	ProcVector[X_CopyPlane] = ProcStandardRequest;
	ProcVector[X_PolyPoint] = ProcStandardRequest;
	ProcVector[X_PolyLine] = ProcStandardRequest;
	ProcVector[X_PolySegment] = ProcStandardRequest;
	ProcVector[X_PolyRectangle] = ProcStandardRequest;
	ProcVector[X_PolyArc] = ProcStandardRequest;
	ProcVector[X_FillPoly] = ProcStandardRequest;
	ProcVector[X_PolyFillRectangle] = ProcStandardRequest;
	ProcVector[X_PolyFillArc] = ProcStandardRequest;
	ProcVector[X_PolyText8] = ProcStandardRequest;
	ProcVector[X_PolyText16] = ProcStandardRequest;
	ProcVector[X_ImageText8] = ProcStandardRequest;
	ProcVector[X_ImageText16] = ProcStandardRequest;
    }
}
