/* $XConsortium: record.c,v 1.15 94/04/17 21:17:29 dpw Exp $ */
/***************************************************************************
Copyright (c) 1988  X Consortium

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
X CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of the X Consortium shall not be
used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization from the X Consortium.

 * Copyright 1994 Network Computing Devices;
 * Portions Copyright 1988 by Digital Equipment Corporation
 *
 * Permission to use, copy, modify, distribute, and sell this software and
 * its documentation for any purpose is hereby granted without fee, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the names of Network Computing Devices, or Digital
 * not be used in advertising or publicity pertaining to distribution
 * of the software without specific, written prior permission.
 *
 * NETWORK COMPUTING DEVICES, AND DIGITAL DISCLAIM ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL NETWORK COMPUTING DEVICES,
 * OR DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF
 * THIS SOFTWARE.
 **************************************************************************/

#define NEED_REPLIES
#define NEED_EVENTS
#include <stdio.h>
#include "X.h"
#include "Xproto.h"
#include "Xmd.h"
#include "misc.h"
#include "os.h"
#include "extnsionst.h"
#include "dixstruct.h"
#include "resource.h"
#define _XRECORD_SERVER_
#include "record.h"
#include "recordstr.h"

typedef int (*ReqProc) (
#if NeedNestedPrototypes
		ClientPtr /* pClient */
#endif
);

static int RecordRequest(
#if NeedFunctionPrototypes
	ClientPtr client
#endif
);

static int ProcRecordDispatch(
#if NeedFunctionPrototypes
	ClientPtr client
#endif
);
static int sProcRecordDispatch(
#if NeedFunctionPrototypes
	ClientPtr client
#endif
);

/*
 * Local Global Variables
 */
static int RecordReqCode;
static int RecordEventBase;
static int RecordErrorBase;
static int RecordPrivateIndex;

static RESTYPE RTConfig;
static RESTYPE RecordDelete;   /* Resource type for intercepted clients */

#define VERIFY_RCONFIG(id, config, client) \
{ \
    config = (RecordConfig *)LookupIDByType(id, RTConfig); \
    if (!config) \
    { \
		client->errorValue = id; \
		return RecordErrorBase + XRecordBadConfiguration; \
    } \
}

#define VERIFY_FLAGS(flags) \
{ \
        if( (flags.events.first > XRecordMaxEvent) || \
	    (flags.events.last  > XRecordMaxEvent) || \
            (flags.errors.first > XRecordMaxError) || \
            (flags.errors.last  > XRecordMaxError) || \
            (flags.core_requests.first > XRecordMaxCoreRequest) || \
            (flags.core_requests.last > XRecordMaxCoreRequest)  || \
            (flags.core_replies.first > XRecordMaxCoreRequest) || \
            (flags.core_replies.last > XRecordMaxCoreRequest)  || \
            (flags.ext_requests.ext_major > XRecordMaxExtRequest)  || \
            (flags.ext_replies.ext_major > XRecordMaxExtRequest)   || \
            (flags.ext_requests.ext_major != 0 && \
	    flags.ext_requests.ext_major < XRecordMinExtRequest)  || \
            (flags.ext_replies.ext_major != 0 && \
	    flags.ext_replies.ext_major < XRecordMinExtRequest) ) \
            	return BadValue; \
}

static int RecordedEvents[128];

typedef struct _RecordIntercept {
    struct _RecordIntercept *next;
    XRecordFlags        flags;          /* Protocol to be intercepted */
    XID			intercept_id;	/* client base */
    XID			rec_id;		/* client-death resource id */
} RecordIntercept;

typedef struct 	_RecordConfig
{
    ClientPtr		data_client;  	/* Data connection */
    XRecordConfig      	id;		/* config XID */
    RecordIntercept	*pInterceptClients; /* Clients to intercept */
} RecordConfig, *RecordConfigPtr;

static int		RecordNumConfigs = 0;
static RecordConfig 	**RecordConfigList = NULL;

/*----------------------------*
 *  Forward Declarations
 *----------------------------*/
static void 	RecordClientProto();

/*
 * Procedures to support config objects
 */
static int
FreeConfig(env, id)
    pointer         env;
    XID             id;
{
    RecordConfig 	*pConfig = (RecordConfig *)env;
    RecordIntercept	*pCur;
    int i;

    while(pCur = pConfig->pInterceptClients)
    {
	if (pCur->rec_id)
	    FreeResource(pCur->rec_id, RT_NONE);
	else
	{
	    pConfig->pInterceptClients = pCur->next;
	    xfree(pCur);
	}
    }
    for (i = 0; i < RecordNumConfigs; i++)
    {
	if (RecordConfigList[i] == pConfig)
	{
	    RecordNumConfigs--;
	    for ( ; i < RecordNumConfigs; i++)
		RecordConfigList[i] = RecordConfigList[i+1];
#ifdef VERBOSE
	    ErrorF("%s:  Free config: 0x%lx\n", XRecordExtName, pConfig);
#endif
	    break;
	}
    }
    xfree(pConfig);
    return Success;
}

static void
SendRecordDone(pConfig)
    RecordConfig *pConfig;
{
    xRecordEnableConfigReply 	rep;
    int 			n;

    if (!pConfig->data_client ||
	pConfig->data_client->clientState != ClientStateRunning)
	return;
    rep.type		= X_Reply;
    rep.sequenceNumber 	= pConfig->data_client->sequence;
    rep.length		= 0;
    rep.nReplies	= 0;

    if(pConfig->data_client->swapped)
    {
	swaps(&rep.sequenceNumber, n);
	swapl(&rep.length, n);
    }

    WriteToClient(pConfig->data_client, sizeof(xRecordEnableConfigReply), (char *)&rep);
    AttendClient(pConfig->data_client);
    pConfig->data_client 	= NULL;
}

static int
FreeInterceptedClient(value, id)
    pointer value; /* must conform to DeleteType */
    XID   id;
{
    RecordConfig   	*pConfig = (RecordConfig *)value;
    RecordIntercept 	*pCur, **pPrev;

    for (pPrev = &pConfig->pInterceptClients; pCur = *pPrev; pPrev = &pCur->next)
    {
	if (pCur->rec_id == id)
	{
	    *pPrev = pCur->next;
	    RecordClientProto(pCur, xTrue);
	    xfree(pCur);
	    if (!pConfig->pInterceptClients)
		SendRecordDone(pConfig);
	    break;
	}
    }
    return (Success);
}

static void
XRecordCloseDown(extEntry)
    ExtensionEntry *extEntry;
{
    xfree(RecordConfigList);
    RecordConfigList = NULL;
    RecordNumConfigs = 0;
    RTConfig = 0;
    return;
}

static int
InitIntercept(client, pCur, pConfig)
    ClientPtr client;
    RecordIntercept *pCur;
    RecordConfig *pConfig;
{
    ReqProc *vec;

    if (!client->devPrivates[RecordPrivateIndex].ptr)
    {
	vec = (ReqProc *) xalloc(sizeof(ReqProc) * 256);
	if (!vec)
	    return BadAlloc;
	memcpy((char *)vec, (char *)client->requestVector, sizeof(ReqProc) * 256);
	client->devPrivates[RecordPrivateIndex].ptr = (pointer)client->requestVector;
	client->requestVector = vec;
    }
    pCur->rec_id = FakeClientID(client->index);
    if (!AddResource(pCur->rec_id, RecordDelete, (pointer)pConfig))
	return BadAlloc;
    RecordClientProto(pCur, xFalse);
    return Success;
}

static Bool
RecordSelectForIntercept(pConfig, client_id, flags, addtolist)
    RecordConfig *pConfig;
    XID client_id;
    XRecordFlags *flags;
    Bool addtolist;
{
    RecordIntercept *pCur, **pPrev;
    ClientPtr client;

    if(addtolist)
    {
	for (pPrev = &pConfig->pInterceptClients; pCur = *pPrev; pPrev = &pCur->next)
        {
	    if (pCur->intercept_id == client_id) /*already there */
	    {
		*pPrev = pCur->next;
		RecordClientProto(pCur, xTrue);
		pCur->flags = *flags;
		*pPrev = pCur;
		RecordClientProto(pCur, xFalse);
		return Success;
	    }
        }
        /* add new client to pConfig->pInterceptClients */
        pCur = (RecordIntercept *) xalloc(sizeof(RecordIntercept));
        if (!pCur)
	    return BadAlloc;    	
	pCur->flags = *flags;
        pCur->intercept_id = client_id;
	pCur->rec_id = 0;
        pCur->next = pConfig->pInterceptClients;
        pConfig->pInterceptClients = pCur;
	client = clients[CLIENT_ID(client_id)];
        if (client && client->clientState == ClientStateRunning)
	{
	    if (InitIntercept(client, pCur, pConfig) != Success)
		return BadAlloc;
	}
        return Success;
    }
    else
    {
	for (pCur = pConfig->pInterceptClients; pCur; pCur = pCur->next)
        {
	    if (pCur->intercept_id == client_id)
	    {
		FreeResource(pCur->rec_id, RT_NONE);
                break;
            }
        }
        return Success;
    }
}

static RecordConfig *
XRecordCreateConfig(client, id)
    ClientPtr		client;
    XRecordConfig 	id;
{
    RecordConfig	*pConfig;

    if((pConfig = (RecordConfig*)Xcalloc(sizeof(RecordConfig))) == NULL)
	return (RecordConfig *)NULL;
    pConfig->id = id;
    pConfig->pInterceptClients = NULL;
    pConfig->data_client = (ClientPtr)NULL;

    if (!AddResource(id, RTConfig, (pointer) pConfig))
	return (RecordConfig *)NULL;

    RecordConfigList = (RecordConfig **)xrealloc(
	RecordConfigList, (RecordNumConfigs+1) * sizeof(RecordConfig *) );
    RecordConfigList[RecordNumConfigs++] = pConfig;
	
#ifdef VERBOSE
    ErrorF("%s:  Configuration '0x%lx' Created\n", XRecordExtName, id);
#endif
    return pConfig;
}

static int
AnyReqsLeft(client, req)
    ClientPtr client;
    int req;
{
    int i;

    for(i = 0; i < RecordNumConfigs; i++)
    {
	RecordConfig *pConfig = RecordConfigList[i];
    	if(pConfig->data_client)
        {
    	    RecordIntercept 	*pCur;

    	    for (pCur = RecordConfigList[i]->pInterceptClients;
		 pCur;  pCur = pCur->next)
    	    {
		if (pCur->intercept_id == client->clientAsMask)
		{
		    if( (req >= pCur->flags.core_requests.first &&
			 req <= pCur->flags.core_requests.last) ||
		       (req == pCur->flags.ext_requests.ext_major) )
			return TRUE;
		    break;
		}
	    }
	}
    }
    return FALSE;
}

static void
RecordClientProto(pCur, delete)
    RecordIntercept *pCur;
    BOOL	delete;
{
    register ClientPtr client;
    int incr;
    register int j;

    client = clients[CLIENT_ID(pCur->intercept_id)];
    if (!client || client->clientState != ClientStateRunning)
	return;
#ifdef VERBOSE
    ErrorF("%s:  Client [%d]  Mask 0x%lx\n", XRecordExtName,
	   client->index, client->clientAsMask);
#endif
    for(j=0L; j<XRecordMaxExtRequest; j++)
    {
	if( (j >= pCur->flags.core_requests.first &&
		j <= pCur->flags.core_requests.last) ||
		(j == pCur->flags.ext_requests.ext_major) )
	{
	    if (!delete)
		client->requestVector[j] = RecordRequest;
	    else if (!AnyReqsLeft(client, j))
		client->requestVector[j] = ((ReqProc *)client->devPrivates[RecordPrivateIndex].ptr)[j];
	}
    }
    incr = delete ? -1 : 1;
    for(j=0L; j<XRecordMaxEvent; j++)
    {
	if(pCur->flags.events.last)
	{
	    if(j >= pCur->flags.events.first && j <= pCur->flags.events.last)
		RecordedEvents[j] += incr;
	}
    }
}

static int
RecordRequest(client)
    ClientPtr client;
{
    REQUEST(xResourceReq);
    register int	i;

    for(i = 0; i < RecordNumConfigs; i++)
    {
    	if(RecordConfigList[i]->data_client)
        {
    	    RecordIntercept 	*pCur;

    	    for (pCur = RecordConfigList[i]->pInterceptClients;
		 pCur;  pCur = pCur->next)
    	    {
		if (pCur->intercept_id == client->clientAsMask)
	        {   	
		    xRecordEnableConfigReply 	rep;

	  	    rep.type        	= X_Reply;
    	            rep.sequenceNumber 	= RecordConfigList[i]->data_client->sequence;
                    rep.length      	= client->req_len;
                    rep.id_base		= pCur->intercept_id;
                    rep.nReplies	= 1;
                    rep.client_swapped 	= client->swapped;
                    rep.client_seq     	= client->sequence;
               	    rep.direction	= XRecordFromClient;
	
#ifdef VERBOSE
     		    ErrorF("%s:  Client: 0x%lx Config: 0x%lx RecordRequest[%d] (%d - %d)\n",
		    XRecordExtName,
		    pCur->intercept_id,
		    RecordConfigList[i]->id,
                    stuff->reqType,
                    pCur->flags.core_requests.first,
		    pCur->flags.core_requests.last);
#endif
   	            WriteToClient(RecordConfigList[i]->data_client,
			      sizeof(xRecordEnableConfigReply), (char *)&rep);
   	            WriteToClient(RecordConfigList[i]->data_client,
				  client->req_len<<2, (char *)stuff);
		    break;
             	}
            }
        }
    }
    return((*((ReqProc *)client->devPrivates[RecordPrivateIndex].ptr)[stuff->reqType])(client));
}

static void
RecordEvents(pcbl, nulldata, pinfo)
    CallbackListPtr *pcbl;
    pointer nulldata;
    pointer pinfo;
{
    register int i, j;
    EventInfoRec *info = (EventInfoRec *)pinfo;
    ClientPtr client = info->client;

    for (j = 0; j < info->count; j++)
    {
	xEvent *x_event = &info->events[j];
	if (!RecordedEvents[x_event->u.u.type & 0177])
	    continue;
	for(i = 0; i < RecordNumConfigs; i++)
	{
	    if(RecordConfigList[i]->data_client)
	    {
		RecordIntercept *pCur;

		for (pCur = RecordConfigList[i]->pInterceptClients;
		     pCur;
		     pCur = pCur->next)
		{
		    if (pCur->intercept_id == client->clientAsMask)
		    {
			if (x_event->u.u.type >= pCur->flags.events.first &&
			    x_event->u.u.type <= pCur->flags.events.last)
			{
			    xRecordEnableConfigReply rep;
			    xEvent eventTo;

			    if (x_event->u.u.type == X_Error &&
				pCur->flags.errors.first == 0 &&
				pCur->flags.errors.last == 0)
				return;

			    rep.type        	= X_Reply;
			    rep.sequenceNumber 	= RecordConfigList[i]->data_client->sequence;
			    rep.length     	= sizeof(xEvent) >> 2;
			    rep.nReplies	= 1;
			    rep.id_base		= pCur->intercept_id;
			    rep.client_swapped	= client->swapped;
			    rep.client_seq      = client->sequence;
			    rep.direction	= XRecordFromServer;
#ifdef VERBOSE
			    ErrorF("%s:  Client: 0x%lx Config: 0x%lx RecordEvents[%d]  (%d - %d)\n",
				   XRecordExtName,
				   pCur->intercept_id,
				   RecordConfigList[i]->id,
				   x_event->u.u.type,
				   pCur->flags.events.first,
				   pCur->flags.events.last);
#endif
			    WriteToClient(RecordConfigList[i]->data_client,
					  sizeof(xRecordEnableConfigReply),
					  (char *)&rep);
			    if (client->swapped)
				(*EventSwapVector[x_event->u.u.type & 0177])
				    (x_event, &eventTo);
			    WriteToClient(RecordConfigList[i]->data_client,
					  sizeof(xEvent),
					  client->swapped ? (char *)&eventTo :
					  (char *)x_event);
			}
			break;
		    }
		}
	    }
	}
    }
}

static void
RecordClientState(pcbl, nulldata, pclient)
    CallbackListPtr *pcbl;
    pointer nulldata;
    pointer pclient;
{
    ClientPtr client = (ClientPtr)pclient;
    int i;

    switch (client->clientState)
    {
    case ClientStateGone:
	if (client->devPrivates[RecordPrivateIndex].ptr)
	    xfree(client->requestVector);
    case ClientStateRunning:
	for(i = 0; i < RecordNumConfigs; i++)
	{
	    RecordConfig *pConfig = RecordConfigList[i];
	    RecordIntercept *pCur;

	    for (pCur = RecordConfigList[i]->pInterceptClients;
		 pCur;  pCur = pCur->next)
	    {
		if (pCur->intercept_id == client->clientAsMask)
		{
		    InitIntercept(client, pCur, pConfig);
		    break;
		}
	    }
	}
    }
}

/*
 * Initialize the Record extension
 */
void XRecordExtensionInit()
{
    register ExtensionEntry 	*extEntry;

    RecordPrivateIndex = AllocateClientPrivateIndex();
    if (!AllocateClientPrivate(RecordPrivateIndex, 0) ||
	!AddCallback(&ClientStateCallback, RecordClientState, (pointer)NULL) ||
	!AddCallback(&EventCallback, RecordEvents, (pointer)NULL))
	return;
    RTConfig = CreateNewResourceType(FreeConfig)|RC_NEVERRETAIN;
    RecordDelete = CreateNewResourceType(FreeInterceptedClient)|RC_NEVERRETAIN;
    if(RTConfig == 0 || RecordDelete == 0 ||
       (extEntry = AddExtension(XRecordExtName,
				XRecordNumEvents, XRecordNumErrors,
				ProcRecordDispatch, sProcRecordDispatch,
				XRecordCloseDown, StandardMinorOpcode)) == NULL)
    {
        ErrorF("%s:  Extension failed to Initialise.\n",
		XRecordExtName);
        return;
    }
#ifdef VERBOSE
    ErrorF("%s:  AddExtension assigned Major Opcode '%d'\n",
           XRecordExtName, extEntry->base);
#endif
    RecordReqCode = extEntry->base;
    RecordErrorBase = extEntry->errorBase;
    RecordEventBase = extEntry->eventBase;

#ifdef VERBOSE
    ErrorF("%s:  Max Events (%d)  Max Requests (%d)\n",
	   XRecordExtName, XRecordMaxEvent, XRecordMaxExtRequest);
    ErrorF("%s:  Protocol version: %d.%d successfully loaded\n",
	   XRecordExtName, XRecordMajorVersion, XRecordMinorVersion);
#endif
}

/*
 * Protocol requests
 */

/*
 * Query extension
 */
static int
ProcRecordQueryVersion(client)
    ClientPtr client;
{
    REQUEST(xRecordQueryVersionReq);
    xRecordQueryVersionReply 	rep;
    int 		n;

    REQUEST_SIZE_MATCH(xRecordQueryVersionReq);
    rep.type        	= X_Reply;
    rep.sequenceNumber 	= client->sequence;
    rep.length         	= 0;

    /* if requested version is higher than the version of this extension */
    /* use this extension version */
    if ((stuff->majorVersion > XRecordMajorVersion) ||
        ((stuff->majorVersion == XRecordMajorVersion) &&
          (stuff->minorVersion >= XRecordMinorVersion)))
    {
        rep.majorVersion  	= XRecordMajorVersion;
        rep.minorVersion  	= XRecordMinorVersion;
    }
    /* if requested version is lower than supported by the extension */
    /* use the lowest version supported by this extension */
    else if ((stuff->majorVersion < XRecordLowestMajorVersion) ||
        ((stuff->majorVersion == XRecordLowestMajorVersion) &&
          (stuff->minorVersion <= XRecordLowestMinorVersion)))
    {
        rep.majorVersion = XRecordLowestMajorVersion;
        rep.minorVersion = XRecordLowestMinorVersion;
    }
    else
    {
        rep.majorVersion = stuff->majorVersion;
        rep.minorVersion = stuff->minorVersion;
    }

    if(client->swapped) {
    	swaps(&rep.sequenceNumber, n);
    	swapl(&rep.length, n);
	swaps(&rep.majorVersion, n);
	swaps(&rep.minorVersion, n);
    }

    WriteToClient(client, sizeof(xRecordQueryVersionReply), (char *)&rep);

    return(client->noClientException);
}

/*
 * Create config
 */
static int
ProcRecordCreateConfig(client)
    ClientPtr client;
{
    REQUEST(xRecordCreateConfigReq);
    RecordConfigPtr pConfig;

    REQUEST_SIZE_MATCH(xRecordCreateConfigReq);
    LEGAL_NEW_RESOURCE(stuff->cid, client);
    if (!XRecordCreateConfig(client, stuff->cid))
	return BadAlloc;

    return (client->noClientException);
}

/*
 * Destroy a config object
 */	
static int
ProcRecordFreeConfig(client)
    ClientPtr       client;
{
    RecordConfigPtr    pConfig;
    REQUEST(xRecordFreeConfigReq);

    REQUEST_SIZE_MATCH(xRecordFreeConfigReq);
    VERIFY_RCONFIG(stuff->cid, pConfig, client);

    FreeResource(pConfig->id, RT_NONE);
#ifdef VERBOSE
    ErrorF("%s:  Free resource '0x%lx'\n",
	XRecordExtName, stuff->cid);
#endif

    return(client->noClientException);
}

/*
 * Change a config object
 */	
static int
ProcRecordChangeConfig(client)
    ClientPtr client;
{
    REQUEST(xRecordChangeConfigReq);
    RecordConfigPtr 	pConfig;
    int 		clientIndex, status;

    REQUEST_SIZE_MATCH(xRecordChangeConfigReq);
    VERIFY_RCONFIG(stuff->cid, pConfig, client);
    VERIFY_FLAGS(stuff->record_flags);

    clientIndex = CLIENT_ID(stuff->id_base);

    if (stuff->id_base != CLIENT_BITS(stuff->id_base) ||
	!clientIndex || client->index == clientIndex)
    {
	client->errorValue = stuff->id_base;
        return BadMatch;
    }

    /* Create delete resource for intercept_id */
    status = RecordSelectForIntercept(pConfig, stuff->id_base,
				      &stuff->record_flags, stuff->add);
    if (status != Success)
		return status;

    return(client->noClientException);
}

/*
 * Get a config object
 */
static int
ProcRecordGetConfig(client)
    ClientPtr client;
{
    REQUEST(xRecordGetConfigReq);
    RecordConfigPtr 		pConfig;
    ClientPtr 			recordclient;
    xRecordGetConfigReply 	rep;
    register int		i;
    int 			n;

    REQUEST_SIZE_MATCH(xRecordGetConfigReq);
    VERIFY_RCONFIG(stuff->cid, pConfig, client);
                 	
    rep.type        	= X_Reply;
    rep.sequenceNumber  = client->sequence;
    rep.length      	= 0;
    rep.enabled = (pConfig->data_client != NULL);

#ifdef VERBOSE
    ErrorF("%s:  Config '0x%lx' intercept\n", XRecordExtName, stuff->cid);
#endif

    if(client->swapped)
    {
    	swaps(&rep.sequenceNumber, n);
    	swapl(&rep.length, n);
    }

    WriteToClient(client, sizeof(xRecordGetConfigReply), (char *)&rep);

    return(client->noClientException);
}

/*
 * Enable a config object
 */
static int
ProcRecordEnableConfig(client)
    ClientPtr client;
{
    REQUEST(xRecordEnableConfigReq);
    RecordConfigPtr 		pConfig;
    register int		i;
    int 			n;

    REQUEST_SIZE_MATCH(xRecordEnableConfigReq);
    VERIFY_RCONFIG(stuff->cid, pConfig, client);
    if (pConfig->data_client)
	return BadMatch;
    pConfig->data_client 	= client;
#ifdef VERBOSE
    ErrorF("%s:  Enable config '0x%lx': %d\n",
	XRecordExtName, stuff->cid, stuff->enable);
#endif
          	
    IgnoreClient(client);
    return(client->noClientException);
}

/*
 * Disable a config object
 */
static int
ProcRecordDisableConfig(client)
    ClientPtr client;
{
    REQUEST(xRecordDisableConfigReq);
    RecordConfigPtr 		pConfig;

    REQUEST_SIZE_MATCH(xRecordEnableConfigReq);
    VERIFY_RCONFIG(stuff->cid, pConfig, client);
#ifdef VERBOSE
    ErrorF("%s:  Enable config '0x%lx': %d\n",
	XRecordExtName, stuff->cid, stuff->enable);
#endif
          	
    SendRecordDone(pConfig);
    return(client->noClientException);
}

/*
 * Protocol dispatch procedure
 */
static int ProcRecordDispatch(client)
    ClientPtr client;
{
    REQUEST(xReq);
    register int status = Success;

    switch (stuff->data)
    {
	case X_RecordQueryVersion:
	    return ProcRecordQueryVersion(client);
	case X_RecordCreateConfig:
	    return ProcRecordCreateConfig(client);
	case X_RecordFreeConfig:
	    return ProcRecordFreeConfig(client);
	case X_RecordChangeConfig:
	    return ProcRecordChangeConfig(client);
	case X_RecordGetConfig:
	    return ProcRecordGetConfig(client);
	case X_RecordEnableConfig:
	    return ProcRecordEnableConfig(client);
	case X_RecordDisableConfig:
	    return ProcRecordDisableConfig(client);
       default:
#ifdef VERBOSE
	ErrorF("%s:  Invalid Request.  Minor opcode=%d\n",
	       XRecordExtName, stuff->data);
#endif
	    return BadRequest;
    }
}

static int
sProcRecordQueryVersion(client)
    ClientPtr client;
{
    REQUEST(xRecordQueryVersionReq);
    register char 	n;

    swaps(&stuff->length, n);
    REQUEST_SIZE_MATCH(xRecordQueryVersionReq);
    swaps(&stuff->majorVersion, n);
    swaps(&stuff->minorVersion,n);
    return ProcRecordQueryVersion(client);
}

static
int sProcRecordCreateConfig(client)
    ClientPtr client;
{
    REQUEST(xRecordCreateConfigReq);
    register char 	n;

    swaps(&stuff->length, n);
    REQUEST_SIZE_MATCH(xRecordCreateConfigReq);
    swapl(&stuff->cid, n);
    return ProcRecordCreateConfig(client);
}

static int
sProcRecordFreeConfig(client)
    ClientPtr client;
{
    REQUEST(xRecordFreeConfigReq);
    register char 	n;

    swaps(&stuff->length, n);
    REQUEST_SIZE_MATCH(xRecordFreeConfigReq);
    swapl(&stuff->cid, n);
    return ProcRecordFreeConfig(client);
}

static int
sProcRecordChangeConfig(client)
    ClientPtr client;
{
    REQUEST(xRecordChangeConfigReq);
    register char 	n;

    swaps(&stuff->length, n);
    REQUEST_SIZE_MATCH(xRecordChangeConfigReq);
    swapl(&stuff->cid, n);
    swapl(&stuff->id_base, n);
    swaps(&stuff->record_flags.ext_requests.ext_minor.first, n);
    swaps(&stuff->record_flags.ext_requests.ext_minor.last, n);
    swaps(&stuff->record_flags.ext_replies.ext_minor.first, n);
    swaps(&stuff->record_flags.ext_replies.ext_minor.last, n);
    return ProcRecordChangeConfig(client);
}

static int
sProcRecordGetConfig(client)
    ClientPtr client;
{
    REQUEST(xRecordGetConfigReq);
    register char n;

    swaps(&stuff->length, n);
    REQUEST_SIZE_MATCH(xRecordGetConfigReq);
    swapl(&stuff->cid, n);
    return ProcRecordGetConfig(client);
}

static int
sProcRecordEnableConfig(client)
    ClientPtr client;
{
    REQUEST(xRecordEnableConfigReq);
    register char 	n;

    swaps(&stuff->length, n);
    REQUEST_SIZE_MATCH(xRecordEnableConfigReq);
    swapl(&stuff->cid, n);
    return ProcRecordEnableConfig(client);
}

static int
sProcRecordDisableConfig(client)
    ClientPtr client;
{
    REQUEST(xRecordDisableConfigReq);
    register char 	n;

    swaps(&stuff->length, n);
    REQUEST_SIZE_MATCH(xRecordDisableConfigReq);
    swapl(&stuff->cid, n);
    return ProcRecordDisableConfig(client);
}

static int
sProcRecordDispatch(client)
    ClientPtr client;
{
    REQUEST(xReq);
    register int status = Success;

    switch (stuff->data)
    {
	case X_RecordQueryVersion:
	    return sProcRecordQueryVersion(client);
	case X_RecordCreateConfig:
	    return sProcRecordCreateConfig(client);
	case X_RecordFreeConfig:
	    return sProcRecordFreeConfig(client);
	case X_RecordChangeConfig:
	    return sProcRecordChangeConfig(client);
	case X_RecordGetConfig:
	    return sProcRecordGetConfig(client);
	case X_RecordEnableConfig:
	    return sProcRecordEnableConfig(client);
	case X_RecordDisableConfig:
	    return sProcRecordDisableConfig(client);
       default:
#ifdef VERBOSE
	ErrorF("%s:  Invalid Request.  Minor opcode=%d\n",
	       XRecordExtName, stuff->data);
#endif
	    return BadRequest;
    }
}
