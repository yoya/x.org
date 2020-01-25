
/************************************************************************
 *
 * message.h --
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

/* $XConsortium: message.h,v 1.1 94/03/17 11:35:32 dpw Exp $ */

#ifndef _message_h
#define _message_h


#include "Xmd.h"


typedef enum
    {
	reply_message,
	event_message,
	error_message
    } MessageType;


typedef struct _PooledMessage
    {
    union
        {
        char reply[32];
        char error[32];
        char event[32];
	int replySize;
        } un;
 
    MessageType type;
    char *pLargeReply;
    char *pReplyData;
    int lenReplyData;
    Bool freeReplyData;
    struct _PooledMessage *next;
    struct _PoolPage *page;
    } PooledMessageRec, *PooledMessagePtr;

typedef struct _PoolPage
    {
    int numAvail;
    int numPerPage;
    PooledMessagePtr avail;
    struct _PoolPage *nextPage;
    struct _PoolPage *prevPage;
    } PoolPageRec, *PoolPagePtr;

typedef struct _ClientMessageHeader
    {
    int client_index;
    PooledMessagePtr head;
    PooledMessagePtr tail;
    struct _ClientMessageHeader *next;
    } ClientMsgHeaderRec, *ClientMsgHeaderPtr;

typedef struct
    {
    ClientMsgHeaderRec forClient[MAXCLIENTS];
    ClientMsgHeaderPtr first;
    } MsgBufferRec, *MsgBufferPtr;

#define GetPooledMessage()     (GetPooledMessages(1))


/***********************************************************************
 *
 * STX to MTX merge versions of ReturnPooledMessage
 *
 ***********************************************************************/
#define ReturnPooledMessage(p) (ReturnPooledMessages(p))

#ifdef XTHREADS
#define MTXReturnPooledMessage ReturnPooledMessage(msg)
#else
#define MTXReturnPooledMessage /* nothing */
#endif


#define GetReplyMessage(t, p)  ((t *)GetPooledReplyMessage(sizeof(t), p))

#define GetReplyPointer(t, m) ((t *)(m->pLargeReply?m->pLargeReply:m->un.reply))
#define GetEventPointer(t, m) ((t *)(m->un.event))
#define GetErrorPointer(t, m) ((t *)(m->un.error))

/***********************************************************************
 *
 * Transfer any local messages to the global buffer.
 *
 ***********************************************************************/

#define TRANSFER_LOCAL_MESSAGES_TO_GLOBAL(client)                       \
{                                                                       \
    register MsgBufferPtr localMsgBuffer;                               \
                                                                        \
    if (!client->clientGone)                                            \
    {                                                                   \
        localMsgBuffer = (MsgBufferPtr) client->msgBuffer;              \
        if (localMsgBuffer->first)                                      \
        {                                                               \
            TransferLocalMessagesToGlobal(localMsgBuffer);              \
        }                                                               \
    }                                                                   \
}
/***********************************************************************
 *
 * Merge MTX with STX, cover the GetReplyMessage macro.
 * 
 * I don't see any other way of impelementing this.  I don't know, but 
 * there may be something left in the client structure that indicates 
 * if it is locked.
 *
 * See mtxlock.h for the MTX_UNLOCK_* macros.
 *
 ***********************************************************************/
#ifdef XTHREADS
#define MTXGetReplyMessage(mapping, rep, repRec, MTX_UNLOCK_SOMETHING)	\
    if (!(rep = GetReplyMessage (mapping, msg )))			\
    {									\
	MTX_UNLOCK_SOMETHING;						\
	return BadAlloc;						\
    }
#else
#define MTXGetReplyMessage(mapping, rep, repRec, MTX_UNLOCK_SOMETHING)	\
{									\
    rep = &repRec;							\
}
#endif


extern PooledMessagePtr GetPooledMessages();
extern char *GetPooledReplyMessage();
extern void ReturnPooledMessages();
extern int  InitializeMessageMonitor();
extern void CleanupMessageMonitor();
extern MsgBufferPtr RegisterLocalMessageBufferForThread();
extern void UnregisterLocalMessageBufferForThread();
extern void TransferLocalMessagesToGlobal();
extern void AllowEventTimestampUpdates();
extern void SendReplyToClient();
extern void WriteEventsToClient();
extern void SendErrorToClient();
extern void FlushAllMessages();
extern void GrabClientMessageDelivery();
extern void UngrabClientMessageDelivery();
extern void WriteReplyDataToGrabbedClient();
extern void SendConnectionSetupInfo();

#endif
