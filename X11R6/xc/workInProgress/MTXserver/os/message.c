
/************************************************************************
 *
 * message.c --
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

/**********************************************************************
 **********************************************************************
 **                                                                  **
 **                     Message Output Monitor                       **
 **                                                                  **
 ** Description:                                                     **
 **     The Message Output Monitor manages all output from the       **
 **     server to any client.  Any data that flows from the server   **
 **     to a client must pass though the MOM.  Replies, events, and  **
 **     errors are generically referred to as messages.  The         **
 **     message is the basic unit around which the MOM components    **
 **     are built.  See the MTX CDS for a full description of the    **
 **     MOM design and data structures.                              **
 **                                                                  **
 ** Code Groups/Exported Functions:                                  **
 **                                                                  **
 **     INITIALIZATION AND TERMINATION:                              **
 **         InitializeMessageMonitor                                 **
 **         CleanupMessageMonitor                                    **
 **                                                                  **
 **     MESSAGE POOL MANIPULATION:                                   **
 **         GetPooledMessages                                        **
 **         GetPooledReplyMessage                                    **
 **         ReturnPooledMessages                                     **
 **                                                                  **
 **     MESSAGE BUFFER MANIPULATION:                                 **
 **         RegisterLocalMessageBufferForThread                      **
 **         UnregisterLocalMessageBufferForThread                    **
 **         TransferLocalMessagesToGlobal                            **
 **         AllowEventTimestampUpdates                               **
 **                                                                  **
 **     BUFFER A MESSAGE LOCALLY:                                    **
 **         SendReplyToClient                                        **
 **         WriteEventsToClient                                      **
 **         SendErrorToClient XXX:SM - Now located in dispatch.c     **
 **                                                                  **
 **     DELIVER MESSAGES TO CLIENT:                                  **
 **         FlushAllMessages                                         **
 **         GrabClientMessageDelivery                                **
 **         UngrabClientMessageDelivery                              **
 **         WriteReplyDataToGrabbedClient                            **
 **         SendConnectionSetupInfo                                  **
 **                                                                  **
 **********************************************************************
 **********************************************************************/
/*
 * XXX:SM - Hmmm, I don't think that any of these functions are necessary
 *          during ST operation.
 */

#ifdef XTHREADS /* XXX:SM This eliminates the entire contents of this file. */

#define NEED_EVENTS
#define NEED_REPLIES

#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/uio.h>
#include "X.h"
#include "misc.h"
#include "Xproto.h"
#include "os.h"
#include "osdep.h"
#include "dixstruct.h" 
#include "message.h"
#include "mtxlock.h"

#define PAGESIZE (2048)
#define PAGEWATERMARK (5)
#define MSGS_PER_PAGE \
            ((PAGESIZE - sizeof(PoolPageRec)) / sizeof(PooledMessageRec))

typedef struct
        {
            X_MUTEX_TYPE mutex;
            X_COND_TYPE condition;
            long status;
            long time;
        } MsgDeliveryLockRec, *MsgDeliveryLockPtr;

#define MSG_DELIVERY_WAITING_MASK   (0x80000000)
#define MSG_DELIVERY_ACTIVE_MASK    (0x40000000)
#define MSG_DELIVERY_ATTEMPTS       (0x3fffffff)

#define TS_OFFSET_NONE ((BYTE) 0xff)
#define TS_TABLE_SIZE  128

static PoolPagePtr AllocateMessagePoolPage();
static PooledMessagePtr GetMessage();
static MsgBufferPtr AllocateMessageBuffer();
static PooledMessagePtr GetPendingMessagesForClient();

static void ReturnMessage();
static void FreeMessagePoolPage();
static int  InitializeGlobalMessageBuffer();
static int  InitializeLocalMessageBufferKey();
static void InitializeTimestampOffsets();
static void InitializeMessageDeliveryLocks();
static void FreeGlobalMessageBuffer();
static int  InitializeMessagePool();
static void ClearLocalMessageBuffer();
static void MoveLocalMessagesToGlobalBuffer();
static void FreeMessagePool();
static void FreeMessageBuffer();
void BufferMessageForClient();
static void WriteMessagesToSocket();
static void DeliverMessagesToClient();
static void CreateClientOutputThread();
static void TryToFreeMessages();
static void WriteToSocket();
static void ClientOutputThread();


/***********************************************************************
 ****************   Private Monitor Data   *****************************
 ***********************************************************************/

static int numPoolPages;
static PoolPagePtr currentPage;
static PoolPagePtr headOfPageList;
static X_MUTEX_TYPE messagePoolMutex;

static MsgBufferPtr globalMsgBuffer;
static X_MUTEX_TYPE globalMsgBufferMutex;
static X_KEY_TYPE messageKey;

static MsgDeliveryLockRec messageDeliveryLock[MAXCLIENTS];
static BYTE eventTimeOffsets[TS_TABLE_SIZE];
static char nulPadding[4];


/**********************************************************************
 **********************************************************************
 **                                                                  **
 **                  Initialization and Termination                  **
 **                                                                  **
 ** Description:                                                     **
 **                                                                  **
 **     This portion of the message output monitor provides a set    **
 **     of functions that initialize and terminate the monitor.      **
 **     Monitor data structures and locks are allocated and          **
 **     deallocated by these routines.  The initialization routine   **
 **     must be called prior to any other message output monitor     **
 **     function.                                                    **
 **                                                                  **
 ** Exported Functions:                                              **
 **                                                                  **
 **     int                                                          **
 **     InitializeMessageMonitor()                                   **
 **         Allocates and initializes all  monitor data structures   **
 **         and locks.  Must be called prior to any other monitor    **
 **         function.  Returns non-Success on allocation error.      **
 **                                                                  **
 **     void                                                         **
 **     CleanupMessageMonitor()                                      **
 **         Deallocates monitor data structures and destroys locks.  **
 **         Assumes that no other threads will be accessing the      **
 **         monitor during cleanup.                                  **
 **                                                                  **
 ** Private Functions:                                               **
 **                                                                  **
 **     int                                                          **
 **     InitializeMessagePool()                                      **
 **         Allocate pages for the message pool and divide them      **
 **         into message lists.  Allocate and initialize the         **
 **         exclusive message pool lock.  Returns non-Success        **
 **         if memory cannot be allocated.                           **
 **                                                                  **
 **     void                                                         **
 **     FreeMessagePool()                                            **
 **         Deallocate all pages of memory in the message pool.      **
 **         Destroy the exclusive message pool lock.                 **
 **                                                                  **
 **     int                                                          **
 **     InitializeGlobalMessageBuffer()                              **
 **         Allocate and initialize the global message buffer and    **
 **         the exculsive global message buffer lock.  Returns       **
 **         non-Success if memory cannot be allocated.               **
 **                                                                  **
 **     void                                                         **
 **     FreeGlobalMessageBuffer()                                    **
 **         Deallocate the global message buffer and destroy the     **
 **         exclusive global message buffer lock.                    **
 **                                                                  **
 **     int                                                          **
 **     InitializeLocalMessageBufferKey()                            **
 **         Allocate a key to be used to locate pointers to local    **
 **         message buffers which can be accessed via per-thread     **
 **         data.  Returns non-Success if key cannot be allocated.   **
 **                                                                  **
 **     void                                                         **
 **     InitializeMessageDeliveryLocks()                             **
 **         Allocate and initialize the locks required to implement  **
 **         the implicit message delivery locks.  A single message   **
 **         delivery lock is present for each potential client.      **
 **                                                                  **
 **     void                                                         **
 **     InitializeTimestampOffsets()                                 **
 **         Initialize the static timestamp offset table.  The       **
 **         table is indexed by event number and contains the byte   **
 **         offset of a timestamp field in the event structure.      **
 **         Only events that have timestamps that should be updated  **
 **         will place entries in this table.  All others will have  **
 **         the value TS_OFFSET_NONE.  This routine will assign the  **
 **         appropriate values for all core event types.             **
 **                                                                  **
 **********************************************************************
 **********************************************************************/

int
InitializeMessageMonitor()
{
    if (InitializeMessagePool() != Success)
	return !Success;

    if (InitializeGlobalMessageBuffer() != Success)
        return !Success;

    InitializeTimestampOffsets();
    InitializeMessageDeliveryLocks();
    InitializeLocalMessageBufferKey();
    bzero(nulPadding, 4);
    return Success;
}

void
CleanupMessageMonitor()
{
    FreeMessagePool();
    FreeGlobalMessageBuffer();
}

static
int
InitializeMessagePool()
{
    numPoolPages = 0;
    headOfPageList = AllocateMessagePoolPage();
    currentPage = headOfPageList;
    if (currentPage == NULL)
	return !Success;

    X_MUTEX_INIT(&messagePoolMutex, X_MUTEX_ATTR_DEFAULT);
    X_SET_SYMNAME(&messagePoolMutex, "MsgPoolM");
    return Success;
}

static
void
FreeMessagePool()
{
    while (headOfPageList != NULL)
    {
        PoolPagePtr page;
        page = headOfPageList;
        headOfPageList = headOfPageList->nextPage;
        FreeMessagePoolPage(page);
    }

    /*TBD pthread_mutex_destroy */
}

static
int
InitializeGlobalMessageBuffer()
{
    globalMsgBuffer = AllocateMessageBuffer();
    if (!globalMsgBuffer)
        return !Success;
    
    X_MUTEX_INIT(&globalMsgBufferMutex, X_MUTEX_ATTR_DEFAULT);
    X_SET_SYMNAME(&globalMsgBufferMutex, "GMsg_M  ");
    return Success;
}

static
void
FreeGlobalMessageBuffer()
{
    FreeMessageBuffer(globalMsgBuffer);
    /* TBD pthread_mutex_destroy */
}

static
int
InitializeLocalMessageBufferKey()
{
    if (X_KEY_CREATE(&messageKey, NULL))
	return !Success;

    return Success;
}

static
void
InitializeMessageDeliveryLocks()
{
    int i;
    for (i = 0; i < MAXCLIENTS; i++)
    {
        char thread_name[20];
        messageDeliveryLock[i].status = 0;
        X_MUTEX_INIT(&messageDeliveryLock[i].mutex, X_MUTEX_ATTR_DEFAULT);
        X_COND_INIT(&messageDeliveryLock[i].condition, X_COND_ATTR_DEFAULT);
        sprintf(thread_name, "Msg_M%03d", i);
        X_SET_SYMNAME(&messageDeliveryLock[i].mutex, thread_name);
        sprintf(thread_name, "Msg_C%03d", i);
        X_SET_SYMNAME(&messageDeliveryLock[i].condition, thread_name);
    }
}

static
void
InitializeTimestampOffsets()
{
    int i;
    BYTE elOffset;
    BYTE kbpOffset;
    BYTE propOffset;
    xEvent dummyEvent;
    char *dummyBasePtr;
    char *dummyTimePtr;

    for (i = 0; i < TS_TABLE_SIZE; i++)
    {
	eventTimeOffsets[i] = TS_OFFSET_NONE;
    }

    dummyBasePtr = (char *) &dummyEvent;
    dummyTimePtr = (char *) &dummyEvent.u.enterLeave.time;
    elOffset = (BYTE) (dummyTimePtr - dummyBasePtr);
    dummyTimePtr = (char *) &dummyEvent.u.keyButtonPointer.time;
    kbpOffset = (BYTE) (dummyTimePtr - dummyBasePtr);
    dummyTimePtr = (char *) &dummyEvent.u.property.time;
    propOffset = (BYTE) (dummyTimePtr - dummyBasePtr);

    eventTimeOffsets[KeyPress] = kbpOffset;
    eventTimeOffsets[KeyRelease] = kbpOffset;
    eventTimeOffsets[KeyRelease] = kbpOffset;
    eventTimeOffsets[ButtonPress] = kbpOffset;
    eventTimeOffsets[ButtonRelease] = kbpOffset;
    eventTimeOffsets[MotionNotify] = kbpOffset;
    eventTimeOffsets[EnterNotify] = elOffset;
    eventTimeOffsets[LeaveNotify] = elOffset;
    eventTimeOffsets[PropertyNotify] = propOffset;
}


/*************************************************************************
 *************************************************************************
 **                                                                     **
 **                     Message Pool Manipulation                       **
 **                                                                     **
 **  Description:                                                       **
 **                                                                     **
 **     This portion of the message output monitor provides a set of    **
 **     functions that operate on the global message pool.  Operations  **
 **     include fetching and returning message lists from the pool.     **
 **     Pool pages will be allocated from the system when the pool      **
 **     becomes empty, and will be returned to the system when the      **
 **     number of pool pages is above a predefined threshold when       **
 **     there exists a page with all its messages unallocated.  If      **
 **     a pool page cannot be allocated from the system, the message    **
 **     output monitor will attempt of free some messages which may     **
 **     result in the death of a lazy client (see TryToFreeMessages).   **
 **                                                                     **
 **  Exported Functions:                                                **
 **                                                                     **
 **      PooledMessagePtr                                               **
 **      GetPooledMessages(count)                                       **
 **          This routine will return a linked list of <count>          **
 **          messages retrieved from the global message pool.  A        **
 **          NULL pointer is returned if all messages cannot be         **
 **          allocated.  The exclusive message pool lock is obtained    **
 **          and held for the duration of this operaton.                **
 **                                                                     **
 **      char *                                                         **
 **      GetPooledReplyMessage(replySize, pMessage)                     **
 **          This routine will return a pointer to a reply and a        **
 **          pointer to a message that is retrieved from the global     **
 **          pool.  The message pointer is returned via the parameter   **
 **          <pMessage>.  All messages structures have 32 bytes of      **
 **          storage available for events, errors, and the fixed        **
 **          portion of replies.  Although most fixed portions of       **
 **          replies fit into 32 bytes, some require more storage.      **
 **          If the reply requires more storage than is available in    **
 **          the message, it will be allocated separatly and attached   **
 **          to the message structure.  The location of the reply       **
 **          storage will be irrelavent outside the monitor.  A         **
 **          calling routine will receive a pointer to the reply and    **
 **          not have to worry about whether the reply was allocated    **
 **          separately or not.  The exclusive message pool lock is     **
 **          obtained and held for the duration of this operation.      **
 **                                                                     **
 **      void                                                           **
 **      ReturnPooledMessages(messages)                                 **
 **          This routine will return the linked list of <messages>     **
 **          to the global message pool.  First the list will be        **
 **          traversed to free all large replies that may have been     **
 **          allocated separate from the message, if any.  Then it      **
 **          obtains the exclusive message pool lock and holds it       **
 **          until each message in the list has been returned to its    **
 **          pool page.                                                 **
 **                                                                     **
 **  Private Functions:                                                 **
 **                                                                     **
 **      PoolPagePtr                                                    **
 **      AllocateMessagePoolPage()                                      **
 **          This routine allocates a pool page from the system and     **
 **          divides it up into a page header and a free list of        **
 **          messages.  A pointer to the pool page header is returned.  **
 **          A NULL pointer is returned if memory cannot be allocated   **
 **          from the system.  No locks are required by this routine.   **
 **                                                                     **
 **      void                                                           **
 **      FreeMessagePoolPage(page)                                      **
 **          This routine returns a pool page to the system. No         **
 **          locks are required by this routine.                        **
 **                                                                     **
 **      PooledMessagePtr                                               **
 **      GetMessage()                                                   **
 **          This routine will retreive a single message from the       **
 **          the message pool.  If the message pool is empty, it        **
 **          will attempt to allocate a new pool page.  If a pool       **
 **          page cannot be allocated, it will try to free messages.    **
 **          Finally, if it cannot free any messages, it will return    **
 **          a NULL pointer.  The exclusive message pool lock must      **
 **          be held prior to calling this routine.                     **
 **                                                                     **
 **      void                                                           **
 **      ReturnMessage(message)                                         **
 **          This routine will return a single message to its pool      **
 **          page.  If this results in a pool page with no remaining    **
 **          allocated messages and the number of pool pages is above   **
 **          a defined high watermark, the pool page will be returned   **
 **          to the system.  The exclusive message pool lock must be    ** 
 **          held prior to calling this routine.                        **
 **                                                                     **
 **      void                                                           **
 **      TryToFreeMessages()                                            **
 **          This routine will attempt to free previously allocated     **
 **          messages so they can be returned to the message pool.      **
 **          Presumably, if this routine is called, the system is not   **
 **          able to allocate any pool pages and the pool is empty.     **
 **          Before giving up on the client that needs the message,     **
 **          we will look around and see if we can locate another       **
 **          client that may not be reading its messages and allowing   **
 **          them to accumulate in the server.  We will first look      **
 **          for clients that have had messages pending in the global   **
 **          buffer for what seems an unusual amount of time.  See      **
 **          the description in the code for other criteria.  Once we   **
 **          reach this point, a client must die to free any messages.  **
 **          This routine will obtain and hold the exclusive message    **
 **          pool lock at times during its execution.                   **
 **                                                                     **
 *************************************************************************
 *************************************************************************/


PooledMessagePtr
GetPooledMessages(count)
int count;
{
    PooledMessagePtr head;
    PooledMessagePtr message;

    X_MUTEX_LOCK(&messagePoolMutex);
    for (head = NULL; count > 0; count--)
    {
        message = GetMessage();
	if (!message)
	{
	    ReturnPooledMessages(head);
            X_MUTEX_UNLOCK(&messagePoolMutex);
	    return NULL;
        }
        message->next = head;
        head = message;
    }
    X_MUTEX_UNLOCK(&messagePoolMutex);
    return head;
}

char *
GetPooledReplyMessage(replySize, pMessage)
int replySize;
PooledMessagePtr *pMessage;
{
    char *reply;
    PooledMessagePtr message;

    X_MUTEX_LOCK(&messagePoolMutex);
    message = GetMessage();
    if (!message)
    {
        X_MUTEX_UNLOCK(&messagePoolMutex);
	*pMessage = NULL;
	return NULL;
    }

    if (replySize > 32)
    {
	reply = (char *) xalloc(replySize);
	if (!reply)
	{
	    ReturnPooledMessages(message);
            X_MUTEX_UNLOCK(&messagePoolMutex);
	    *pMessage = NULL;
	    return NULL;
        }
	message->pLargeReply = reply;
	message->un.replySize = replySize;
    }
    else
    {
        message->pLargeReply = NULL;
	reply = (char *) &message->un.reply;
    }

    X_MUTEX_UNLOCK(&messagePoolMutex);
    *pMessage = message;
    return reply;
}

void
ReturnPooledMessages(messages)
PooledMessagePtr messages;
{
    PooledMessagePtr temp;
    for (temp = messages; temp != NULL; temp = temp->next)
    {
        if (temp->pLargeReply != NULL)
        { 
            xfree(temp->pLargeReply);
        }
    }
  
    X_MUTEX_LOCK(&messagePoolMutex);
    while (messages != NULL)
    {
        PooledMessagePtr next;
	next = messages->next;
        ReturnMessage(messages);
        messages = next;
    }
    X_MUTEX_UNLOCK(&messagePoolMutex);
}
 
static
PoolPagePtr
AllocateMessagePoolPage()
{
    int i;
    PoolPagePtr page;
    PooledMessagePtr message;
 
    page = (PoolPagePtr) xalloc(PAGESIZE);
    if (!page)
	return NULL;

    page->numAvail = MSGS_PER_PAGE;
    page->numPerPage = MSGS_PER_PAGE;
    page->avail = (PooledMessagePtr) &page[1];
    page->nextPage = NULL;
    page->prevPage = NULL;

    message = (PooledMessagePtr) &page[1];
    for (i = 0; i < page->numPerPage - 1; i++)
    {
        message[i].next = &message[i+1];
        message[i].page = page;
    }
 
    message[i].next = NULL;
    message[i].page = page;

    numPoolPages++;
    return page;
}

static
void
FreeMessagePoolPage(page)
PoolPagePtr page;
{
    xfree(page);
    numPoolPages--;
}

static
PooledMessagePtr
GetMessage()
{
    Bool retry = FALSE;
    PooledMessagePtr message;
    PoolPagePtr page;
 
    while (currentPage->avail == NULL)
    {
        /* try to find any page with available messages */
        currentPage = headOfPageList;
        while (currentPage != NULL && currentPage->avail == NULL)
            currentPage = currentPage->nextPage;
 
        if (currentPage == NULL)
        {
            /* no pages have available messages */
            PoolPagePtr newPage;
            newPage = AllocateMessagePoolPage();
            if (!newPage)
            {
                if (retry)
                    return NULL;

                X_MUTEX_UNLOCK(&messagePoolMutex);
                FlushAllMessages(NULL);
                TryToFreeMessages();
                X_MUTEX_LOCK(&messagePoolMutex);
                retry = TRUE;
                currentPage = headOfPageList;
                continue;
            }

            /* add newly allocated page to pool */
            newPage->nextPage = headOfPageList;
            newPage->prevPage = NULL;
            headOfPageList->prevPage = newPage;
            headOfPageList = newPage;
            currentPage = newPage;
        }
    }
 
    /* currentPage has available messages */
    currentPage->numAvail--;
    message = currentPage->avail;
    currentPage->avail = message->next;
    message->pLargeReply = NULL;
    message->pReplyData = NULL;
    message->next = NULL;
    return message;
}
 
static
void
ReturnMessage(message)
PooledMessagePtr message;
{
    PoolPagePtr page;
    if (message == NULL)
        return;
 
    /* put message at head of free list */
    page = message->page;
    message->next = page->avail;
    page->avail = message;
 
    /* deallocate page if no allocated messages and above high watermark */
    if (++page->numAvail == page->numPerPage && numPoolPages > PAGEWATERMARK)
    {
        if (page->nextPage != NULL)
            page->nextPage->prevPage = page->prevPage;
        if (page->prevPage != NULL)
            page->prevPage->nextPage = page->nextPage;
        if (page == headOfPageList)
            headOfPageList = page->nextPage;
        if (page == currentPage)
            currentPage = headOfPageList;
	    
        FreeMessagePoolPage(page);
    }
}

static
void
TryToFreeMessages()
{
    int i;
    int killIndex;
    int maxMessages;
    MsgDeliveryLockPtr sync;
    long clientMessages[MAXCLIENTS];
    long clientWaiting[MAXCLIENTS];
    long currentMillis;

    killIndex = 0;
    currentMillis = GetTimeInMillis();

    /*-------------------------------------------------------------
     * get number of messages pending for each client.
     *------------------------------------------------------------*/
    X_MUTEX_LOCK(&globalMsgBufferMutex);
    for (i = 0; i < MAXCLIENTS; i++)
    {
	int count;
	PooledMessagePtr msgList;

	count = 0;
	msgList = globalMsgBuffer->forClient[i].head;
	while (msgList)
	{
	    msgList = msgList->next;
	    count++;
        }
        clientMessages[i] = count;
    }
    X_MUTEX_UNLOCK(&globalMsgBufferMutex);

    /*-------------------------------------------------------------
     * determine the amount of time, if any, that a thread has 
     * been blocked trying to deliver messages to each client.
     *------------------------------------------------------------*/
    for (i = 0; i < MAXCLIENTS; i++)
    {
        sync = &messageDeliveryLock[i];
        X_MUTEX_LOCK(&sync->mutex);
	if (sync->time)
	{
	    clientWaiting[i] = currentMillis - sync->time;
	}
	else
	{
	    clientWaiting[i] = 0;
	}
        X_MUTEX_UNLOCK(&sync->mutex);
    }

    /*-------------------------------------------------------------
     * We're going to try and free up messages by locating a client
     * with pending messages and killing it. The R5 server will
     * kill a client it's trying to send a message to if it can't
     * resize the client's output buffer.  We will try to target a
     * client for termination using the following rules:
     *
     * 1) Locate all clients with message delivery threads that 
     *    have been pending over a certain threshold.  Target the
     *    client in this set with the most messages pending.
     * 2) If no clients meet the first condition, locate all
     *    clients with messages delivery threads that have been
     *    pending for any amount of time.  Target the client in
     *    this set with the most messages pending.
     * 3) If no clients meet the second condition, target the
     *    client with the most messages pending.
     * 4) If there are no messages in the global buffer, give up.
     *------------------------------------------------------------*/

    killIndex = 0;
    maxMessages = 0;
    for (i = 0; i < MAXCLIENTS; i++)
    {
	if (clientWaiting[i] > 20000 && clientMessages[i] > maxMessages)
	{
	    maxMessages = clientMessages[i];
	    killIndex = i;
	}
    }

    if (killIndex == 0)
    {
        for (i = 0; i < MAXCLIENTS; i++)
        {
            if (clientWaiting[i] > 0 && clientMessages[i] > maxMessages)
            {
                maxMessages = clientMessages[i];
                killIndex = i;
            }
        }
    }

    if (killIndex == 0)
    {
        for (i = 0; i < MAXCLIENTS; i++)
        {
            if (clientMessages[i] > maxMessages)
            {
                maxMessages = clientMessages[i];
                killIndex = i;
            }
        }
    }

    /*-------------------------------------------------------------
     * If a client has been selected for termination, get it's
     * pending messages and give them back to the pool.  Then,
     * terminate the client.  Otherwise, we just return without
     * having freed any messages.
     *------------------------------------------------------------*/
    if (killIndex)
    {
        PooledMessagePtr msgList;

        X_MUTEX_LOCK(&globalMsgBufferMutex);
        msgList = GetPendingMessagesForClient(killIndex, globalMsgBuffer);
        X_MUTEX_UNLOCK(&globalMsgBufferMutex);
        ReturnPooledMessages(msgList);
        CloseDownClient(clients[killIndex]);
    }
}



/*************************************************************************
 *************************************************************************
 **                                                                     **
 **                    Message Buffer Manipulation                      **
 **                                                                     **
 **  Description:                                                       **
 **                                                                     **
 **     This portion of the message output monitor provides a set of    **
 **     routines that manipulate local and global message buffers.      **
 **     There is one local message buffer allocated for each CIT and    **
 **     DIT and one global message buffer.  The structure of a local    **
 **     message buffer is identical to the stucrutre of a global        **
 **     message buffer.  Generally, all messages generated by a single  **
 **     protocol request will be buffered in the generating thread's    **
 **     local buffer without requiring any locks.  At the end of each   **
 **     request, all (if any) messages will be atomically transferred   **
 **     from the local buffer to the global message buffer.  Once a     **
 **     message reaches the global message buffer it is considered to   **
 **     to be deliverable to the target client in the order that it     **
 **     appears in the linked list of messages pending for that client. **
 **                                                                     **
 **  Exported Functions:                                                **
 **                                                                     **
 **      MsgBufferPtr                                                   **
 **      RegisterLocalMessageBufferForThread                            **
 **          This routine must be called during the initialization      **
 **          of each CIT and DIT.  A local message buffer will be       **
 **          allocated and stored in the calling thread's per thread    **
 **          data area and associated with the key 'messageKey'.        **
 **                                                                     **
 **      void                                                           **
 **      UnregisterLocalMessageBufferForThread                          **
 **          This routine is called during the termination of each      **
 **          CIT and DIT.  The calling thread's associated local        **
 **          message buffer is deallocated.  Any messages that may be   **
 **          present in the local message buffer will be transferred    **
 **          to the global message buffer prior to deallocation.        **
 **                                                                     **
 **      void                                                           **
 **      TransferLocalMessagesToGlobal                                  **
 **          All locally buffered messages must be transferred to the   **
 **          global message buffer at the end of each protocol request  **
 **          or device processing.  All messages must be transferred    **
 **          atomically to the global buffer.  An exclusive mutex is    **
 **          while transfering the all messages.                        **
 **                                                                     **
 **      void                                                           **
 **      AllowEventTimestampUpdates                                     **
 **          Certain timestamps are updated when they are transferred   **
 **          to the global buffer.  The timestamp update mechanism must **
 **          be enabled for each individual event.  This routine        **
 **          enables that mechanism for an individual event.            **
 **                                                                     **
 **  Private Functions:                                                 **
 **                                                                     **
 **      MsgBufferPtr                                                   **
 **      AllocateMessageBuffer                                          **
 **          Allocate and initialize a message buffer.                  **
 **                                                                     **
 **      void                                                           **
 **      FreeMessageBuffer                                              **
 **          Deallocate a message buffer.                               **
 **                                                                     **
 **      void                                                           **
 **      ClearLocalMessageBuffer                                        **
 **          Remove all messages from a message buffer.  This routine   **
 **          is called only after the messages have been processed in   **
 **          some mannor and need to removed from a message buffer.     **
 **          No processing of messages is performed here.               **
 **                                                                     **
 **      void                                                           **
 **      MoveLocalMessagesToGlobalBuffer                                **
 **          Implementation of the local to global transfer.  For each  **
 **          target client in the specified local buffer with messages  **
 **          pending, append the list of messages to the current list   **
 **          of deliverable messages for that client in the global      **
 **          message buffer.  If appended to an empty list, update the  **
 **          "clients with pending messages" linked list.               **
 **                                                                     **
 **      PooledMessagePtr                                               **
 **      GetPendingMessagesForClient                                    **
 **          Retrieve the head of the pending messages linked list for  **
 **          a specified client.  Remove that client from the list of   **
 **          "clients with pending messages".                           **
 **                                                                     **
 *************************************************************************
 *************************************************************************/


static
MsgBufferPtr
AllocateMessageBuffer()
{
    int i;
    MsgBufferPtr msgbuf;
    msgbuf = (MsgBufferPtr) xalloc(sizeof(MsgBufferRec));
    if (!msgbuf)
        return NULL;

    msgbuf->first = NULL;
    for (i = 0; i < MAXCLIENTS; i++)
    {
        msgbuf->forClient[i].head = NULL;
        msgbuf->forClient[i].tail = NULL;
        msgbuf->forClient[i].next = NULL;
        msgbuf->forClient[i].client_index = i;
    }

    return msgbuf;
}

static
void
FreeMessageBuffer(msgbuf)
MsgBufferPtr msgbuf;
{
    xfree(msgbuf);
}

MsgBufferPtr
RegisterLocalMessageBufferForThread()
{
    MsgBufferPtr localMsgBuffer;

    localMsgBuffer = (MsgBufferPtr) AllocateMessageBuffer();
    if (!localMsgBuffer)
        return NULL;

    X_SETSPECIFIC(messageKey, localMsgBuffer);
    return localMsgBuffer;
}

void
UnregisterLocalMessageBufferForThread(clientIndex, msgBuffer)
int clientIndex;
MsgBufferPtr msgBuffer;
{
    PooledMessagePtr msgList;

    TransferLocalMessagesToGlobal(msgBuffer);
    xfree(msgBuffer);

    X_MUTEX_LOCK(&globalMsgBufferMutex);
    msgList = GetPendingMessagesForClient(clientIndex, globalMsgBuffer);
    X_MUTEX_UNLOCK(&globalMsgBufferMutex);

    if (msgList)
    {
        ReturnPooledMessages(msgList);
    }
}

void
TransferLocalMessagesToGlobal(localbuf)
MsgBufferPtr localbuf;
{
    X_MUTEX_LOCK(&globalMsgBufferMutex);
    MoveLocalMessagesToGlobalBuffer(localbuf);
    X_MUTEX_UNLOCK(&globalMsgBufferMutex);
    ClearLocalMessageBuffer(localbuf);
}

void
AllowEventTimestampUpdates(eventNumber, timestampOffset)
BYTE eventNumber;
int timestampOffset;
{
    
    X_MUTEX_LOCK(&globalMsgBufferMutex);
    if (timestampOffset <= 28 && eventNumber < TS_TABLE_SIZE)
    {
        eventTimeOffsets[eventNumber] = (BYTE) timestampOffset;
    }
    X_MUTEX_UNLOCK(&globalMsgBufferMutex);
}

static
void
ClearLocalMessageBuffer(localbuf)
MsgBufferPtr localbuf;
{
    ClientMsgHeaderPtr localHdr;
    localHdr = localbuf->first;
    localbuf->first = NULL;
    while (localHdr != NULL)
    {
        ClientMsgHeaderPtr next;
        next = localHdr->next;
        localHdr->head = NULL;
        localHdr->tail = NULL;
        localHdr->next = NULL;
        localHdr = next;
    }
}

static
void
MoveLocalMessagesToGlobalBuffer(localbuf)
MsgBufferPtr localbuf;
{
    BYTE tsOffset;
    Time timestamp = 0;
    Bool tsInitialized = FALSE;
    ClientMsgHeaderPtr globalHdr;
    ClientMsgHeaderPtr localHdr;

    for (localHdr = localbuf->first; localHdr; localHdr = localHdr->next)
    {
        CARD16 sequence;
        PooledMessagePtr message;

        sequence = clients[localHdr->client_index]->sequence;
        for (message = localHdr->head; message; message = message->next)
        {
            switch (message->type)
            {
                xGenericReply *reply;
                xEvent *event;
                xError *error;

                case reply_message:
                    reply = GetReplyPointer(xGenericReply, message);
                    reply->sequenceNumber = sequence;
                    break;
                case error_message:
                    error = GetErrorPointer(xError, message);
                    error->sequenceNumber = sequence;
                    break;
                case event_message:
                    event = GetEventPointer(xEvent, message);
                    event->u.u.sequenceNumber = sequence;

                    tsOffset = eventTimeOffsets[event->u.u.type & ~0x80];
                    if (tsOffset != TS_OFFSET_NONE)
		    {
			if (!tsInitialized)
			{
                            timestamp = (Time) GetTimeInMillis();
                            tsInitialized = TRUE;
			}
			*((Time *)(((char *)event) + tsOffset)) = timestamp;
		    }
                    break;
            }
        }

        globalHdr = &globalMsgBuffer->forClient[localHdr->client_index];
        if (globalHdr->head == NULL)
        {
            globalHdr->head = localHdr->head;
            globalHdr->tail = localHdr->tail;
            globalHdr->next = globalMsgBuffer->first;
            globalMsgBuffer->first = globalHdr;
        }
        else
        {
            globalHdr->tail->next = localHdr->head;
            globalHdr->tail = localHdr->tail;
        }
    }
}

static
PooledMessagePtr
GetPendingMessagesForClient(clientIndex, msgBuffer)
int clientIndex;
MsgBufferPtr msgBuffer;
{
    ClientMsgHeaderPtr bptr;
    PooledMessagePtr mptr;

    bptr = &msgBuffer->forClient[clientIndex];
    mptr = bptr->head;
    if (mptr != NULL)
    {
        bptr->head = NULL;
        if (globalMsgBuffer->first == bptr)
        {
            globalMsgBuffer->first = bptr->next;
            bptr->next = NULL;
        }
        else
        {
            ClientMsgHeaderPtr btemp;
            btemp = globalMsgBuffer->first;
            while (btemp->next != bptr)
            {
                btemp = btemp->next;
            }
            btemp->next = bptr->next;
            bptr->next = NULL;
        }
    }

    return mptr;
}


/*************************************************************************
 *************************************************************************
 **                                                                     **
 **                     Buffer a Message Locally                        **
 **                                                                     **
 **  Description:                                                       **
 **                                                                     **
 **     This portion of the message output monitor provides a set of    **
 **     routines that locally buffer messages generated by the calling  **
 **     client to be delivered to a target client.                      **
 **                                                                     **
 **  Exported Functions:                                                **
 **                                                                     **
 **      void                                                           **
 **      SendReplyToClient                                              **
 **          Reply message has been allocated and initialized prior to  **
 **          calling this routine.  Tag the message as a reply, swap    **
 **          the reply if client has differing byte order, and place    **
 **          the message in the calling thread's local buffer.          **
 **                                                                     **
 **      void                                                           **
 **      WriteEventsToClient                                            **
 **          Event messages must be allocated and each event copied     **
 **          to a message.  Tag each message as an event, byte swap     **
 **          if required, and place each message in the calling         **
 **          thread's local buffer.                                     **
 **                                                                     **
 **      SendErrorToClient                                              **
 **  XXX:SM This function has been merged and is now located in         **
 **         dispatch.c                                                  **
 **          Error messages must be allocated and the error data        **
 **          copied to the message.  Tag the message as an error,       **
 **          byte swap if required, and place the message in the        **
 **          calling thread's local buffer.                             **
 **                                                                     **
 **  Private Funcions:                                                  **
 **                                                                     **
 **      void                                                           **
 **      BufferMessageForClient                                         **
 **          Buffers a message for a target client in the calling       **
 **          thread's local message buffer.  The local message buffer   **
 **          is located via the per thread data mechanism.  The message **
 **          is appended to the current list of message pending for the **
 **          target client.  If appended to a NULL list, insert the     **
 **          target client into the "clients with pending messages"     **
 **          list for this message buffer.                              **
 **                                                                     **
 *************************************************************************
 *************************************************************************/


void
SendReplyToClient(client, message)
    ClientPtr client;
    PooledMessagePtr message;
{
    if (client->swapped)
        {
        int len;
        char *rep;
        extern void (*ReplySwapVector[128])();

        rep = (message->pLargeReply) ? message->pLargeReply: message->un.reply;
        len = (message->pLargeReply) ? message->un.replySize: 32;
        (*ReplySwapVector[((xReq *)(client)->requestBuffer)->reqType])
             (client, len, rep);
        }

    message->type = reply_message;
    BufferMessageForClient(client, message);
}

void
WriteEventsToClient(client, count, events)
    ClientPtr client;
    int count;
    xEvent *events;
{
    int i;
    PooledMessagePtr message;

    message = GetPooledMessages(count);
    for (i = 0; i < count; i++)
    {
	PooledMessagePtr next;
	next = message->next;
	message->next = NULL;

	if (client->swapped)
	{
	    xEvent *evFrom, *evTo;
            extern void (* EventSwapVector[128])();

	    evFrom = &events[i];
	    evTo = (xEvent *) &message->un.event;
            (*EventSwapVector[evFrom->u.u.type & 0177]) (evFrom, evTo);
	}
	else
	{
	    bcopy(&events[i], &message->un.event, sizeof(xEvent));
	}

	message->type = event_message;
	BufferMessageForClient(client, message);
	message = next;
    }
}

void
BufferMessageForClient(client, message)
ClientPtr client;
PooledMessagePtr message;
{
    ClientMsgHeaderPtr ptr;
    MsgBufferPtr msgBuffer;
    int index = client->index;
    CARD16 sequence;

    X_GETSPECIFIC(messageKey, (void *)&msgBuffer);
    if (msgBuffer == NULL)
    {
        ReturnPooledMessage(message);
	return;
    }

    ptr = &msgBuffer->forClient[index];
    if (ptr->head == NULL)
    {
	ptr->head = message;
	ptr->tail = message;
        ptr->next = msgBuffer->first;
        msgBuffer->first = ptr;
    }
    else
    {
        ptr->tail->next = message;
        ptr->tail = message;
    }
}


/*************************************************************************
 *************************************************************************
 **                                                                     **
 **                   Deliver Messages to Clients                       **
 **                                                                     **
 **  Description:                                                       **
 **                                                                     **
 **      This portion of the message output monitor provides a set of   **
 **      routines that will perform the actual delivery of data to      **
 **      clients.  With blocking writes, we need to avoid a situation   **
 **      were a CIT/DIT thread blocks when delivering data to another   **
 **      client.  We don't want a DIT blocking attempting to deliver    **
 **      data to any client, and we don't want a CIT blocking trying    **
 **      to deliver data to a client other than his own.  For this      **
 **      reason, a ClientOutputThread (COT) will be created to deliver  **
 **      all currently pending messages for each target client.  There  **
 **      is one special case, if a CIT is writting data back to his own **
 **      client, it will not create a COT to deliver.  When messages    **
 **      are removed from the global buffer, they must be delivered     **
 **      before any other thread delivers subsequent messages to the    **
 **      same target client.  The messageDeliveryLocks are used to      **
 **      syncronize message deliver to clients from a multiple threads. **
 **      Also, a specail feature is provided that allows for the        **
 **      message delivery flow to a client to be 'grabbed'.  This is    **
 **      required when data needs to be sent in more than one block     **
 **      from the server and must arrive contiguously at the client.    **
 **      This is only allowed for replies.                              **
 **                                                                     **
 **  External Functions:                                                **
 **                                                                     **
 **      void                                                           **
 **      FlushAllMessages                                               **
 **          Initiate a flush for a messages in the global message      **
 **          buffer.  This routine determines which target clients      **
 **          have messages pending and will initiate the proper         **
 **          delivery mechanism.                                        **
 **                                                                     **
 **      void                                                           **
 **      GrabClientMessageDelivery                                      **
 **          Disable delivery of all data to a specified client for     **
 **          all threads other than the grabbing thread.  Must first    **
 **          obtain the messageDeliveryLocks for synchronization with   **
 **          currently executing flush requests.  Message delivery must **
 **          be grabbed if the server is sending data in separate       **
 **          blocks that must arrive to the client in a contiguous      **
 **          block.  A CIT can only disable the message delivery        **
 **          mechanism for it own client.  Since this is provided to    **
 **          support very large replies, that is not a problem.         **
 **                                                                     **
 **      void                                                           **
 **      UngrabClientMessageDelivery                                    **
 **          Enable delivery of all data to a previous message grab.    **
 **                                                                     **
 **      void                                                           **
 **      WriteReplyDataToGrabbedClient                                  **
 **          Write reply data to a grabbed client.  This bypasses the   **
 **          normal message delivery mechansim and is assumed to only   **
 **          be called while message delivery is grabbed for the        **
 **          target client.                                             **
 **                                                                     **
 **      void                                                           **
 **      SendConnectionSetupInfo                                        **
 **          Write connection informaion to a client.  This is the      **
 **          only non-message data that passes through the MOM.         **
 **                                                                     **
 **  Private Functions:                                                 **
 **                                                                     **
 **      void                                                           **
 **      DeliverMessagesToClient                                        **
 **          This routine is called by a COT or CIT to deliver messages **
 **          to a client.  It provides the required syncronization,     **
 **          removes the pending messages from the global message       **
 **          buffer and initiates the delivery of the data.             **
 **                                                                     **
 **      void                                                           **
 **      WriteMessagesToSocket                                          **
 **          This routine receives a list of messages and builds an     **
 **          iov array, including any required padding, for delivery.   **
 **                                                                     **
 **      void                                                           **
 **      WriteToSocket                                                  **
 **          This routine receives and iov array and issues the writev. **
 **                                                                     **
 **      void                                                           **
 **      CreateClientOutputThread                                       **
 **          Creates a client output threads and detaches.              **
 **                                                                     **
 **      void                                                           **
 **      ClientOutputThread                                             **
 **          Delivers current pending message for a specific client     **
 **          and dies.  Will die immediately if other COT are already   **
 **          blocked waiting to delivery messages to the same client.   **
 **          A COT will pend on the messageDeliveryLocks prior to       **
 **          removing the messages from the global buffer.  Thus, if    **
 **          more messages are added to the buffer while the COT pends  **
 **          they will be picked up eventually.                         **
 **                                                                     **
 *************************************************************************
 *************************************************************************/


void
FlushAllMessages(client)
ClientPtr client;
{
    int i;
    int numberToFlush = 0;
    int flushThisClient = FALSE;
    int clientsToFlush[MAXCLIENTS];
    ClientMsgHeaderPtr ghdr;

    X_MUTEX_LOCK(&globalMsgBufferMutex);
    for (ghdr = globalMsgBuffer->first; ghdr; ghdr = ghdr->next)
    {
	if (client && client->index == ghdr->client_index)
        {
	    flushThisClient = TRUE;
        }
        else
        {
            clientsToFlush[numberToFlush++] = ghdr->client_index;
        }
    }
    X_MUTEX_UNLOCK(&globalMsgBufferMutex);

    for (i = 0; i < numberToFlush; i++)
    {
        CreateClientOutputThread(clientsToFlush[i]);
    }

    if (flushThisClient)
    {
        DeliverMessagesToClient(client->index);
    }
}

static
void
WriteMessagesToSocket(clientIndex, msgList)
int clientIndex;
PooledMessagePtr msgList;
{
    int i, niov;
    struct iovec iov[16];
    PooledMessagePtr temp;

    niov = 0;
    for (temp = msgList; temp; temp = temp->next)
    {
        if (temp->pLargeReply != NULL)
        {
            int rem;
            iov[niov].iov_len = temp->un.replySize;
            iov[niov].iov_base = (caddr_t) temp->pLargeReply;
            niov++;
            if ((rem = (temp->un.replySize & 3)) > 0)
            {
                iov[niov].iov_base = &nulPadding[4 - rem];
                iov[niov].iov_len = 4 - rem;
                niov++;
            }
        }
        else
        {
            iov[niov].iov_len = 32;
            iov[niov].iov_base = (caddr_t) &temp->un.reply;
            niov++;
        }

        if (temp->pReplyData != NULL)
        {
            int rem;
            iov[niov].iov_len = temp->lenReplyData;
            iov[niov].iov_base = (caddr_t) temp->pReplyData;
            niov++;
            if ((rem = (temp->lenReplyData & 3)) > 0)
            {
                iov[niov].iov_base = &nulPadding[4 - rem];
                iov[niov].iov_len = 4 - rem;
                niov++;
            }
        }

        if (niov >= 13)
        {
	    WriteToSocket(clientIndex, iov, niov);
            niov = 0;
        }
    }

    if (niov > 0)
    {
	WriteToSocket(clientIndex, iov, niov);
    }

    for (temp = msgList; temp; temp = temp->next)
    {
        if (temp->pReplyData && temp->freeReplyData)
        {
            xfree(temp->pReplyData);
        }
    }
}

static
void
DeliverMessagesToClient(clientIndex)
int clientIndex;
{
    PooledMessagePtr first;
    MsgDeliveryLockPtr sync;

    /*-------------------------------------------------------------------
     * Obtain message delivery synchroization lock.  Keep track of how
     * many delivery attempts have been made since the last successful
     * delivery.  The attempts value may be used to indentify a lazy
     * client that is not reading messages, which may be selected for
     * termination when the server is desparate for memory.
     * Don't bother waiting if there is already another thread waiting
     * to flush to this client.  Note that the synchronization lock
     * for message delivery is implicit.  The status is set to active
     * when a thread gains access to the output sockets.  Threads will
     * block until the status indicates that no other thread is actively
     * flushing to the target client.
     *------------------------------------------------------------------*/

    sync = &messageDeliveryLock[clientIndex];
    X_MUTEX_LOCK(&sync->mutex);
    sync->status++;
    if (sync->status & MSG_DELIVERY_WAITING_MASK)
    {
        X_MUTEX_UNLOCK(&sync->mutex);
        return;
    }

    while (sync->status & MSG_DELIVERY_ACTIVE_MASK)
    {
        sync->status |= MSG_DELIVERY_WAITING_MASK;
	sync->time = GetTimeInMillis();
        X_COND_WAIT(&sync->condition, &sync->mutex);
        sync->status &= ~MSG_DELIVERY_WAITING_MASK;
    }

    sync->status |= MSG_DELIVERY_ACTIVE_MASK;
    X_MUTEX_UNLOCK(&sync->mutex);

    /*-------------------------------------------------------------------
     * Grab all messages pending for the target client from the global
     * buffer and write them to socket.  Note that it is possible that
     * there will be no messages pending by the time we reach this point.
     * Some other thread may have already delivered them.
     *------------------------------------------------------------------*/

    X_MUTEX_LOCK(&globalMsgBufferMutex);
    first = GetPendingMessagesForClient(clientIndex, globalMsgBuffer);
    X_MUTEX_UNLOCK(&globalMsgBufferMutex);

    if (first != NULL)
    {
        WriteMessagesToSocket(clientIndex, first);
        ReturnPooledMessages(first);
    }

    /*-------------------------------------------------------------------
     * Release the implicit message delivery syncronization lock.  Signal
     * if any threads are waiting for this lock to be released.
     *------------------------------------------------------------------*/

    X_MUTEX_LOCK(&sync->mutex);
    if ((sync->status &= MSG_DELIVERY_WAITING_MASK))
    {
        X_COND_BROADCAST(&sync->condition);
    }
    sync->time = 0;
    X_MUTEX_UNLOCK(&sync->mutex);
}

void
GrabClientMessageDelivery(client)
ClientPtr client;
{
    PooledMessagePtr clientMsgs;
    MsgBufferPtr localMsgBuffer;
    MsgDeliveryLockPtr sync;

    X_GETSPECIFIC(messageKey, (void *)&localMsgBuffer);
    if (!localMsgBuffer || localMsgBuffer != (MsgBufferPtr) client->msgBuffer)
        return;

    FlushAllMessages(client);

    sync = &messageDeliveryLock[client->index];
    X_MUTEX_LOCK(&sync->mutex);
    sync->status++;

    while (sync->status & MSG_DELIVERY_ACTIVE_MASK)
    {
        sync->status |= MSG_DELIVERY_WAITING_MASK;
        X_COND_WAIT(&sync->condition, &sync->mutex);
        sync->status &= ~MSG_DELIVERY_WAITING_MASK;
    }

    sync->status |= MSG_DELIVERY_ACTIVE_MASK;
    X_MUTEX_UNLOCK(&sync->mutex);

    X_MUTEX_LOCK(&globalMsgBufferMutex);
    MoveLocalMessagesToGlobalBuffer(localMsgBuffer);
    clientMsgs = GetPendingMessagesForClient(client->index, globalMsgBuffer);
    X_MUTEX_UNLOCK(&globalMsgBufferMutex);

    ClearLocalMessageBuffer(localMsgBuffer);
    WriteMessagesToSocket(client->index, clientMsgs);
}

void
UngrabClientMessageDelivery(client)
ClientPtr client;
{
    MsgBufferPtr localMsgBuffer;
    MsgDeliveryLockPtr sync;

    X_GETSPECIFIC(messageKey, (void *)&localMsgBuffer);
    if (!localMsgBuffer || localMsgBuffer != (MsgBufferPtr) client->msgBuffer)
    {
	return;
    }

    if (localMsgBuffer->first != NULL)
    {
        FatalError("Inproper use of <Grab/Ungrab>ClientMessageDelivery\n");
    }

    sync = &messageDeliveryLock[client->index];
    X_MUTEX_LOCK(&sync->mutex);

    if ((sync->status &= MSG_DELIVERY_WAITING_MASK))
    {
        X_COND_BROADCAST(&sync->condition);
    }

    X_MUTEX_UNLOCK(&sync->mutex);
}

void
WriteReplyDataToGrabbedClient(client, data, length)
ClientPtr client;
void *data;
int length;
{
    struct iovec iov;
    iov.iov_len = length;
    iov.iov_base = (caddr_t) data;
    WriteToSocket(client->index, &iov, 1);
}

static
void
ClientOutputThread(clientIndex)
int clientIndex;
{
    int status;

    DeliverMessagesToClient(clientIndex);
    X_THREAD_EXIT(&status);
}

static
void
CreateClientOutputThread(clientIndex)
int clientIndex;
{
    X_THREAD_TYPE clientThread;
    if (X_THREAD_CREATE(&clientThread, X_COT_ATTR_DEFAULT, 
			(void *)ClientOutputThread, (void *)clientIndex))
    {
	ErrorF("COT create failed\n");
    }
    X_THREAD_DETACH(&clientThread);
}

static
void
WriteToSocket(clientIndex, iov, niov)
int clientIndex;
struct iovec iov[];
int niov;
{
    extern int errno;
    ClientPtr client;
    OsCommPtr oc;

    client = clients[clientIndex];
    if ((client != NULL) && (!client->clientGone))
    {
        oc = (OsCommPtr) clients[clientIndex]->osPrivate;
        if (writev(oc->fd, iov, niov) == -1)
	{
            /* ErrorF("writev failed, errno = %d, fd = %d\n", errno, oc->fd); */
        }
    }
}

void
SendConnectionSetupInfo(client, csp, data, length)
ClientPtr client;
xConnSetupPrefix *csp;
char *data;
int length;
{
    static char pad[3] = {0, 0, 0};
    struct iovec iov[3];
    int niov = 0;

    iov[niov].iov_base = (caddr_t) csp;
    iov[niov].iov_len = sz_xConnSetupPrefix;
    niov++;

    iov[niov].iov_base = (caddr_t) data;
    iov[niov].iov_len = length;
    niov++;

    if (length & 3)
    {
        iov[niov].iov_base = (caddr_t) pad;
        iov[niov].iov_len = 4 - (length & 3);
        niov++;
    }

    WriteToSocket(client->index, iov, niov);
}

int
WriteToClient (who, count, buf)
    ClientPtr who;
    char *buf;
    int count;
{
    ErrorF("Unexpected WriteToClient");
}

#endif /* XTHREADS */

