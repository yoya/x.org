/* $XConsortium: lbxmain.c,v 1.14 94/12/01 20:30:50 mor Exp $ */
/*
 * $NCDId: @(#)lbxmain.c,v 1.61 1994/11/18 20:32:36 lemke Exp $
 * $NCDOr: lbxmain.c,v 1.4 1993/12/06 18:47:18 keithp Exp keithp $
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
 
#include <sys/types.h>
#define NEED_REPLIES
#define NEED_EVENTS
#include "X.h"
#include "Xproto.h"
#include "Xos.h"
#include "misc.h"
#include "os.h"
#include "dixstruct.h"
#include "resource.h"
#include "scrnintstr.h"
#include "windowstr.h"
#include "pixmapstr.h"
#include "gcstruct.h"
#include "extnsionst.h"
#include "servermd.h"
#include "lbxdeltastr.h"
#define _XLBX_SERVER_
#include "lbxstr.h"
#include "lbxserve.h"
#include "lbxsrvopts.h"
#include "Xfuncproto.h"
#include <errno.h>
#ifdef X_NOT_STDC_ENV
extern int errno;
#endif
#include <sys/uio.h>
#include <stdio.h>

#ifndef X_NOT_POSIX
#include <unistd.h>
#else
extern int read();
#endif

#define CloseLbxClient	0xff

#define MAXBYTESDIFF	8
#define MAXDELTASIZE	(((sz_xLbxDeltaReq + (MAXBYTESDIFF << 1) + 2) / 4) * 4)

extern Bool	LbxThrottleMotionEvents();
extern void	LbxIncrementPixel();
extern void	LbxAllowMotion();
extern int	LbxDecodePoints();
extern int	LbxDecodeSegment();
extern int	LbxDecodeRectangle();
extern int	LbxDecodeArc();

int ProcLbxDispatch();
extern int SProcLbxDispatch();
static void LbxResetProc();
extern void SLbxEvent();

Bool	LbxInitClient ();
void	LbxFreeClient ();
void	LbxCloseClient ();
void	LbxShutdownProxy ();

int	LbxWritev ();
int	LbxWriteToClient ();
int	LbxUncompressedWriteToClient ();

LbxProxyPtr proxyList;
static unsigned char LbxReqCode;
int 	LbxEventCode;
static int BadLbxClientCode;

extern int  LzwWriteV (), LzwRead ();
extern void LzwCompressOn (), LzwCompressOff ();

int		lbxBlockHandlerCount;
int		lbxCompressWorkProcCount;
Bool		lbxAnyOutputPending;

LbxClientPtr	lbxClients[MAXCLIENTS];

extern xConnSetupPrefix connSetupPrefix;
extern char *ConnectionInfo;
extern int  (*LbxInitialVector[3])();
extern ClientPtr	ReadingClient, WritingClient;

#ifndef NDEBUG
int lbxDebug = DBG_CLIENT|DBG_SWITCH;

#define LbxSequence(i)	lbxClients[i]->client->sequence
#endif

void
LbxExtensionInit()
{
    ExtensionEntry *extEntry;

    lbxBlockHandlerCount = 0;
    lbxCompressWorkProcCount = 0;
    proxyList = NULL;
    if ((extEntry = AddExtension(LBXNAME, LbxNumberEvents, LbxNumberErrors,
				 ProcLbxDispatch, SProcLbxDispatch,
				 LbxResetProc, StandardMinorOpcode)))
    {
	LbxReqCode = (unsigned char)extEntry->base;
	LbxEventCode = extEntry->eventBase;
	BadLbxClientCode = extEntry->errorBase;
	EventSwapVector[LbxEventCode] = SLbxEvent;
        LbxDixInit();
    }
}

/*ARGSUSED*/
static void
LbxResetProc (extEntry)
ExtensionEntry	*extEntry;
{
   LbxResetTags();
}

void
LbxCloseClient (client)
    ClientPtr	client;
{
    xLbxEvent	    closeEvent;
    ClientPtr	    master;
    LbxProxyPtr	    proxy;
    LbxClientPtr    lbxClient = LbxClient(client);
    int		    i;

    if (!lbxClient)
	return;
    proxy = lbxClient->proxy;
    DBG (DBG_CLIENT, (stderr, "Close client %d\n", lbxClient->index));
    i = lbxClient->index;
    LbxFreeClient (client);
    if (i == LbxMasterClientIndex)
    {
	CloseDownFileDescriptor (client);
	LbxShutdownProxy (proxy);
    } 
    else
    {
	master = NULL;
	if (proxy->lbxClients[LbxMasterClientIndex])
	    master = proxy->lbxClients[LbxMasterClientIndex]->client;
	if (master && !master->clientGone)
	{
	    closeEvent.type = LbxEventCode;
	    closeEvent.lbxType = LbxCloseEvent;
	    closeEvent.client = i;
	    closeEvent.sequenceNumber = master->sequence;
	    if (master->swapped) {
		int	    n;

		swaps(&closeEvent.sequenceNumber, n);
		swapl(&closeEvent.client, n);
	    }
	    WriteToClient (master, sizeof (closeEvent), &closeEvent);
	}
    }
    /* Switch output to some other client */
    if (lbxAnyOutputPending)
	LbxPickSend ();
}

void
LbxComputeReplyLen(lbxClient, buf)
    LbxClientPtr    lbxClient;
    char	*buf;
{
    LbxProxyPtr	    proxy = lbxClient->proxy;

    if (lbxClient->awaiting_setup)
    {
	xConnSetupPrefix	*csp = (xConnSetupPrefix *) buf;
	short			len = csp->length;
	int			n;
	if (lbxClient->client->swapped) {
	    swaps(&len, n);
        }
	lbxClient->reply_remaining = 8 + (len << 2);
	DBG(DBG_LEN, (stderr, "%d setup bytes remaining\n", lbxClient->reply_remaining));
    }
    else
    {
	xReply    *reply = (xReply *) buf;

	lbxClient->reply_remaining = LbxEventLength(proxy, (xEvent *) buf);
	if (reply->generic.type == X_Reply) {
	    int   len = reply->generic.length;
	    int	  n;
	    if (lbxClient->client->swapped) {
		swapl(&len, n);
            }
	    lbxClient->reply_remaining += len << 2;
	}
	DBG (DBG_LEN, (stderr, "%d reply bytes remaining\n", lbxClient->reply_remaining));
    }
}

int
LbxRead (connection, buf, len)
    int		    connection;
    char	    *buf;
    int		    len;
{
    LbxProxyPtr	proxy = LbxProxy(ReadingClient);
    
    if (ReadingClient != proxy->lbxClients[0]->client)
    {
	DBG (DBG_BLOCK, (stderr, "client %d not on wire\n", LbxClient(ReadingClient)->index));
	errno = EWOULDBLOCK;
	return -1;
    }
    len = (*proxy->read) (connection, buf, len);
    DBG (DBG_IO, (stderr, "read from client %d len %d\n", 
		      proxy->curRecv ? proxy->curRecv->index : -1, len));
    return len;
}

static char *
LbxMakeContiguous (proxy, iov, num, contlen)
    LbxProxyPtr	    proxy;
    struct iovec    *iov;
    int		    num;
    int		    contlen;
{
    int		    i;
    int		    len;
    int		    offset;

    if (num == 1)
	return iov[0].iov_base;
    for (i = 0, offset = 0; i < num; i++) {
	len = min(iov[i].iov_len, contlen);
	bcopy(iov[i].iov_base, &proxy->tempDeltaBuf[offset], len);
	offset += len;
	if ((contlen -= len) == 0)
	    break;
    }
    return (char *) proxy->tempDeltaBuf;
}

static void
LbxComposeDelta(proxy, reply, len)
    LbxProxyPtr	 proxy;
    char	 *reply;
    int		 len;
{
    int		 diffs;
    int		 cindex;
    xLbxDeltaReq *p = (xLbxDeltaReq *)proxy->tempEventBuf;

    if ((diffs = LBXDeltaMinDiffs(&proxy->outdeltas, reply, len,
			    min(MAXBYTESDIFF, (len - sz_xLbxDeltaReq) >> 1),
			    &cindex)) >= 0) {
	LBXEncodeDelta(&proxy->outdeltas, reply, diffs, cindex,
		       &proxy->tempEventBuf[sz_xLbxDeltaReq]);
	p->reqType = LbxEventCode;
	p->lbxReqType = LbxDeltaEvent;
	p->diffs = diffs;
	p->cindex = cindex;
	proxy->deltaEventRemaining = sz_xLbxDeltaReq + sz_xLbxDiffItem * diffs;
	p->length = (proxy->deltaEventRemaining + 3) >> 2;
	proxy->deltaEventRemaining = p->length << 2;
	if (proxy->lbxClients[LbxMasterClientIndex]->client->swapped) {
	    int         n;

	    swaps(&p->length, n);
	}
	proxy->outputDeltaPtr = proxy->tempEventBuf;
    }
}

int
LbxWritev (connection, iov, num)
    int		    connection;
    struct iovec    *iov;
    int		    num;
{
    int		    i;
    int		    len;
    int		    done;
    int		    total;
    LbxClientPtr    lbxClient = LbxClient(WritingClient);
    LbxProxyPtr	    proxy = lbxClient->proxy;
    long	    this_time;
    Bool	    new_reply;
    int		    n;
    
    if (lbxClient != proxy->curSend)
    {
	DBG (DBG_BLOCK, (stderr, "client %d not on write wire\n", 
			 LbxClient(WritingClient)->index));
	errno = EWOULDBLOCK;
	return -1;
    }
    DBG (DBG_IO, (stderr, "writing to client %d no more than %d\n", 
		      proxy->curSend->index, proxy->curSend->reply_remaining));
    if (proxy->curSend->needs_output_switch)
    {
	xLbxEvent *ev = (xLbxEvent *) proxy->tempEventBuf;
#ifndef NDEBUG
	{
        int	client;

        client = ((xLbxEvent *)(proxy->tempEventBuf))->client; 
        if (proxy->lbxClients[0] &&
	    proxy->lbxClients[0]->client->swapped) {
            swapl(&client, n);
        }
        if (proxy->switchEventRemaining)
	    DBG (DBG_SWITCH, (stderr, "needs switch: writing %d switch event bytes to client %d\n",
		proxy->switchEventRemaining, client));
	}
#endif
	ev->type = LbxEventCode;
	ev->lbxType = LbxSwitchEvent;
	ev->client = proxy->curSend->index;

        if (proxy->lbxClients[0] &&
	    proxy->lbxClients[0]->client->swapped) {
            swapl(&ev->client, n);
        }
	proxy->curSend->needs_output_switch = FALSE;
	proxy->switchEventRemaining = sizeof (xLbxEvent);
    }
    if (proxy->switchEventRemaining)
    {
	char	*b;
	struct iovec	v;

#ifndef NDEBUG
	{
        int	client;

        client = ((xLbxEvent *)(proxy->tempEventBuf))->client; 
        if (proxy->lbxClients[0] &&
	    proxy->lbxClients[0]->client->swapped) {
            swapl(&client, n);
        }
	DBG (DBG_SWITCH, (stderr, "writing %d switch event bytes to client %d\n",
	    proxy->switchEventRemaining,  client));
        }
#endif
	b = (char *) proxy->tempEventBuf;
	b += sizeof (xLbxEvent) - proxy->switchEventRemaining;
	v.iov_len = proxy->switchEventRemaining;
	v.iov_base = b;
	done = (*proxy->writev) (connection, &v, 1);
	if (done < 0)
	    return -1;
	if (done > 0)
	    proxy->switchEventRemaining -= done;
	if (proxy->switchEventRemaining)
	{
	    errno = EWOULDBLOCK;
	    return -1;
	}
    }

    total = 0;
    new_reply = (this_time = proxy->curSend->reply_remaining) == 0;
    for (i = 0; i < num; i++)
    {
	total += iov->iov_len;
	if (!this_time && total)
	{
	    LbxComputeReplyLen (proxy->curSend, iov->iov_base);
            /* length fields are always in proxy format */
	    if (proxy->lbxClients[0]->client->swapped != 
            			proxy->curSend->client->swapped) {
		xGenericReply *buf = (xGenericReply *) iov->iov_base;
		xConnSetupPrefix *csp = (xConnSetupPrefix *) iov->iov_base;
		int         n;

		if (proxy->curSend->awaiting_setup) {
		    swaps(&csp->length, n);
		} else {
		    swapl(&buf->length, n);
		}
	    }
	    if (proxy->curSend->awaiting_setup)
		lbxClient->awaiting_setup = FALSE;
	    this_time = proxy->curSend->reply_remaining;
	}
    }
    DBG (DBG_IO, (stderr, "want %d\n", total));
    if (total > this_time)
    {
	DBG (DBG_IO, (stderr, "limit %d\n", this_time));
	for (i = 0; i < num; i++)
	{
	    if (iov->iov_len >= this_time)
	    {
		iov->iov_len = this_time;
		break;
	    }
	    this_time -= iov->iov_len;
	}
	num = i + 1;
    }

    if (new_reply) {
	len = proxy->curSend->reply_remaining;
	if (DELTA_CACHEABLE(&proxy->outdeltas, len)) {
	    char *reply = (char *)LbxMakeContiguous(proxy, iov, num, len);
	    LbxComposeDelta(proxy, reply, len);
	    LBXAddDeltaOut(&proxy->outdeltas, reply, len);
	}
    }
    if (proxy->deltaEventRemaining) {
	struct iovec	v;
	v.iov_len = proxy->deltaEventRemaining;
	v.iov_base = (char *) proxy->outputDeltaPtr;
	done = (*proxy->writev) (connection, &v, 1);
	if (done < 0)
	    return -1;
	proxy->outputDeltaPtr += done;
	if (proxy->deltaEventRemaining -= done) {
	    errno = EWOULDBLOCK;
	    return -1;
	}
	total = proxy->curSend->reply_remaining;
    }
    else {
	if (proxy->nocompression)
	    LzwCompressOff (connection);
	total = (*proxy->writev) (connection, iov, num);
	if (proxy->nocompression)
	    LzwCompressOn (connection);
    }
    if (total > 0)
    {
	DBG (DBG_IO, (stderr, "wrote %d\n", total));
	proxy->curSend->reply_remaining -= total;
    }
    return total;
}

/* ARGSUSED */
void
LbxWakeupHandler (data, result, pReadmask)
    pointer	    data;
    int		    result;
    pointer	    pReadmask;
{
    return;
}

Bool
LbxPickSend ()
{
    int		    i;
    LbxProxyPtr	    proxy;
    LbxClientPtr    lbxClient;
    Bool	    ret;
    
    lbxAnyOutputPending = FALSE;
    ret = FALSE;
    for (i = 0; i < MAXCLIENTS; i++)
    {
	lbxClient = lbxClients[i];
	if (lbxClient && 
	    !lbxClient->client->clientGone &&
	    PendingClientOutput(lbxClient->client))
	{
	    lbxAnyOutputPending = TRUE;
	    proxy = lbxClient->proxy;
	    if (proxy->curSend && 
		(proxy->curSend->reply_remaining ||
		 proxy->curSend->needs_output_switch))
	    {
		DBG (DBG_LEN, (stderr, "output client has %d bytes remaining\n",
			       proxy->curSend->reply_remaining));
	    }
	    else if (proxy->curSend != lbxClient && 
		     !lbxClient->client->clientGone)
	    {
		DBG (DBG_SWITCH, (stderr, 
				  "switching output to %d sequence %d\n", 
				  i, LbxSequence(i)));
		lbxClient->needs_output_switch = TRUE;
		if (!proxy->curSend)
		    proxy->curSend = lbxClient;
		SwitchClientOutput (proxy->curSend->client, 
				    lbxClient->client);
		proxy->curSend = lbxClient;
		ret = TRUE;
	    }
	}
    }
    return ret;
}

void
LbxCompressOn (fd)
{
    LbxProxyPtr proxy;

    for (proxy = proxyList; proxy; proxy = proxy->next) {
	if (proxy->fd == fd) {
	    proxy->nocompression = FALSE;
	    break;
	}
    }
}

void
LbxCompressOff (fd)
{
    LbxProxyPtr proxy;

    for (proxy = proxyList; proxy; proxy = proxy->next) {
	if (proxy->fd == fd) {
	    proxy->nocompression = TRUE;
	    break;
	}
    }
}

/*
 * Flush anything that may be left accumulated in the compression
 * buffers.
 */
LbxFlushCompress ()
{
    LbxProxyPtr	    proxy;

    for (proxy = proxyList; proxy; proxy = proxy->next) {
	if (proxy->compHandle) {
	    if (LzwFlush (proxy->fd) != 0) {
		/* Assume we're write blocked.  What if it's a real error -
		   leave it for the OS layer to figure out? */
		MarkConnectionWriteBlocked (proxy->lbxClients[0]->client);
	    }
	}
    }
}

/*
 * If there is any data left in the input compression buffers, we
 * want the select() call in WaitForSomething() to return immediately,
 * so we cause ClientsWithInput to be set.
 */
/* ARGSUSED */
Bool
LbxCheckCompressInput (dummy1, dummy2)
    pointer dummy1;
    pointer dummy2;
{
    LbxProxyPtr	    proxy;

    if (lbxCompressWorkProcCount == 0)
	return TRUE;

    for (proxy = proxyList; proxy; proxy = proxy->next) {
	if (proxy->compHandle) {
	    if (LzwInputAvail (proxy->fd))
		AvailableClientInput (proxy->lbxClients[0]->client);
	}
    }
    return FALSE;
}

extern int  NewOutputPending;

#ifdef NCD
#define	OSTimePtr	pointer
#endif

/* ARGSUSED */
void
LbxBlockHandler (data, timeout, readmask)
    pointer data;
    OSTimePtr	 timeout;
    pointer readmask;
{
    if (lbxAnyOutputPending)
	LbxPickSend ();

    while (NewOutputPending)
    {
	FlushAllOutput ();
	if (!LbxPickSend())
	    break;
    }
    LbxFlushCompress ();
}

LbxIsClientBlocked (client)
    ClientPtr	client;
{
    extern int		GrabInProgress;
    LbxProxyPtr		proxy = LbxProxy(client);
    LbxClientPtr	lbxClient = LbxClient(client);
    
    if (lbxClient->ignored)
    	return TRUE;
    if (GrabInProgress && client->index != GrabInProgress &&
        lbxClient != proxy->lbxClients[0])
	return TRUE;
    return FALSE;
}

void
LbxIgnoreClient(client)
    ClientPtr	client;
{
    LbxClientPtr lbxClient = LbxClient(client);

    if (lbxClient)
	lbxClient->ignored = TRUE;
    else
	IgnoreClient(client);
}

void
LbxAttendClient(client)
    ClientPtr	client;
{
    LbxClientPtr lbxClient = LbxClient(client);

    if (lbxClient)
	lbxClient->ignored = FALSE;
    else
	AttendClient(client);
}

void
LbxSwitchRecv (proxy, lbxClient)
    LbxProxyPtr		proxy;
    LbxClientPtr	lbxClient;
{
    ClientPtr	client;
    
    proxy->curRecv = lbxClient;
    if (!lbxClient || lbxClient->client->clientGone)
    {
	DBG(DBG_CLIENT, (stderr, "switching to dispose input\n"));
	lbxClient = proxy->lbxClients[0];
        if (!lbxClient)
            return;
    }
    client = lbxClient->client;
    DBG (DBG_SWITCH, (stderr, "switching input to client %d\n", lbxClient->index));
    SwitchClientInput (client, proxy->lbxClients[0]->client);
    proxy->curDix = lbxClient;
}

/* ARGSUSED */
Bool
LbxWaitForUnblocked (client, closure)
    ClientPtr	client;
    pointer	closure;
{
    LbxClientPtr    lbxClient = LbxClient(client);
    LbxProxyPtr	    proxy;

    if (!lbxClient)
	return TRUE;
    proxy = LbxProxy(client);
    if (!LbxIsClientBlocked (client) && !proxy->curDix->reading_pending)
    {
	lbxClient->input_blocked = FALSE;
	DBG (DBG_BLOCK, (stderr, "client %d no longer blocked, switching\n", lbxClient->index));
	SwitchClientInput (client, client);
	proxy->curDix = lbxClient;
	lbxClient->reading_pending = TRUE;
	return TRUE;
    }
    return FALSE;
}

/* ARGSUSED */
int
LbxWaitForUngrab (client, closure)
    ClientPtr	client;
    pointer	closure;
{
    extern int   GrabInProgress;
    LbxClientPtr lbxClient = LbxClient(client);
    LbxProxyPtr  proxy;
    xLbxEvent	 ungrabEvent;

    if (!lbxClient)
	return TRUE;
    proxy = LbxProxy(client);
    if (!GrabInProgress) {
	ungrabEvent.type = LbxEventCode;
	ungrabEvent.lbxType = LbxListenToAll;
	WriteToClient (proxy->lbxClients[0]->client,
		       sizeof(xLbxEvent), &ungrabEvent);
	proxy->grabClient = 0;
	return TRUE;
    }
    return FALSE;
}

/*
 * Returns TRUE if the server has been grabbed by a client not
 * belonging to the current proxy
 */
int
LbxCheckServerGrabs (client)
    ClientPtr	client;
{
    extern int		GrabInProgress;
    int			retval = FALSE;
    LbxProxyPtr		proxy = LbxProxy(client);
    LbxClientPtr	grabbingLbxClient;
    
    if (GrabInProgress) {
	int	  index;
	if ((grabbingLbxClient = lbxClients[GrabInProgress]) == NULL ||
	    grabbingLbxClient->proxy != proxy) {
	    /* client other than a proxy client has grabbed the server */
	    index = -1;
	    retval = TRUE;
	}
	else {
	    index = grabbingLbxClient->index;
	}

	/*
	 * If the current grabbing client has changed, then we need
	 * to send a message to update the proxy.
	 */
	if (proxy->grabClient != GrabInProgress) {
	    xLbxEvent grabEvent;
	    grabEvent.type = LbxEventCode;
	    grabEvent.lbxType = LbxListenToOne;
	    grabEvent.client = index;
	    if (proxy->lbxClients[0]->client->swapped) {
		int n;
		swapl(&grabEvent.client, n);
            }
	    WriteToClient (proxy->lbxClients[0]->client, 
			   sizeof(xLbxEvent), &grabEvent);
	    if (!proxy->grabClient) {
		QueueWorkProc (LbxWaitForUngrab, 
			       proxy->lbxClients[0]->client, NULL);
	    }
	    proxy->grabClient = GrabInProgress;
	}
    }
    return retval;
}

#define MAJOROP(client) ((xReq *)client->requestBuffer)->reqType
#define MINOROP(client) ((xReq *)client->requestBuffer)->data

static Bool lbxCacheable[] = {
	FALSE,	/* LbxQueryVersion	  0 */
	FALSE,	/* LbxStartProxy	  1 */
	TRUE,	/* LbxStopProxy		  2 */
	FALSE,	/* LbxSwitch		  3 */
	FALSE,	/* LbxNewClient		  4 */
	TRUE,	/* LbxCloseClient	  5 */
	TRUE,	/* LbxModifySequence	  6 */
	FALSE,	/* LbxAllowMotion	  7 */
	TRUE,	/* LbxIncrementPixel	  8 */
	FALSE,	/* LbxDelta		  9 */
	TRUE,	/* LbxGetModifierMapping 10 */
	TRUE,	/* LbxQueryTag		 11 */
	TRUE,	/* LbxInvalidateTag	 12 */
	TRUE,	/* LbxPolyPoint		 13 */
	TRUE,	/* LbxPolyLine		 14 */
	TRUE,	/* LbxPolySegment	 15 */
	TRUE,	/* LbxPolyRectangle	 16 */
	TRUE,	/* LbxPolyArc		 17 */
	TRUE,	/* LbxFillPoly		 18 */
	TRUE,	/* LbxPolyFillRectangle	 19 */
	TRUE,	/* LbxPolyFillArc	 20 */
	TRUE,	/* LbxGetKeyboardMapping 21 */
	TRUE,	/* LbxQueryFont		 22 */
	TRUE,	/* LbxChangeProperty	 23 */
	TRUE,	/* LbxGetProperty	 24 */
	TRUE,	/* LbxTagData		 25 */
	TRUE,	/* LbxCopyArea		 26 */
	TRUE,	/* LbxCopyPlane		 27 */
	TRUE,	/* LbxPolyText8		 28 */
	TRUE,	/* LbxPolyText16	 29 */
	TRUE,	/* LbxImageText8	 30 */
	TRUE,	/* LbxImageText16	 31 */
	FALSE,	/* LbxQueryExtension	 32 */
	FALSE,	/* LbxPutImage		 33 */
	FALSE,	/* LbxGetImage		 34 */
};

#define NUM(a)	(sizeof (a) / sizeof (a[0]))

int
LbxReadRequestFromClient (client)
    ClientPtr	client;
{
    int		    ret;
    LbxClientPtr    lbxClient = LbxClient(client);
    LbxProxyPtr	    proxy = lbxClient->proxy;
    ClientPtr	    masterClient = proxy->lbxClients[LbxMasterClientIndex]->client;

    DBG (DBG_READ_REQ, (stderr, "Reading request from client %d\n", lbxClient->index));

    LbxCheckServerGrabs (client);

    if (lbxClient->reading_pending) {
	if (LbxIsClientBlocked (client)) {
	    /* Someone grabbed server before we could process requests! */
	    lbxClient->input_blocked = TRUE;
	    QueueWorkProc (LbxWaitForUnblocked, client, NULL);
	    lbxClient->reading_pending = FALSE;
	    LbxSwitchRecv (proxy, proxy->curRecv);
	    return 0;
	}
	else {
	    ret = LBXReadRequestFromClient (client);
	    if (ret <= 0) {
		lbxClient->reading_pending = FALSE;
	        DBG (DBG_BLOCK, (stderr, "ending reading_pending for client %d\n", lbxClient->index));
	        LbxSwitchRecv (proxy, proxy->curRecv);
	    }
	    else if (lbxClient->reqs_pending)
		--lbxClient->reqs_pending;
	    return ret;
	}
    }

    for (;;)
    {
	Bool		cacheable;

	ret = LBXReadRequestFromClient (masterClient);
	DBG (DBG_READ_REQ, (stderr, "Real readRequest returns %d\n", ret));
	if (ret <= 0)
	    return ret;

	client->requestBuffer = masterClient->requestBuffer;
	client->req_len = masterClient->req_len;

	cacheable = TRUE;
	if (MAJOROP(client) == LbxReqCode) {
	    /* Check to see if this request is delta cached */
	    if (MINOROP(client) < NUM(lbxCacheable))
		cacheable = lbxCacheable[MINOROP(client)];
	    if (MINOROP(client) == X_LbxSwitch)
	    {
		/* Switch is sent by proxy */
		if (masterClient->swapped)
		    SProcLbxSwitch (client);
		else
		    ProcLbxSwitch (client);
		return 0;
	    }
	    else if (MINOROP(client) == X_LbxDelta)
	    {
		ret = ProcLbxDelta (client);
		DBG(DBG_DELTA, (stderr, "delta decompressed msg %d, len = %d\n",
		    (unsigned) ((unsigned char *)client->requestBuffer)[0], ret));
	    }
	}

	if (cacheable && DELTA_CACHEABLE(&proxy->indeltas, ret)) {
	    DBG(DBG_DELTA, (stderr, "caching msg %d, len = %d, index = %d\n",
		    (unsigned) ((unsigned char *)client->requestBuffer)[0], ret,
				proxy->indeltas.nextDelta));
	    LBXAddDeltaIn(&proxy->indeltas, client->requestBuffer, ret);
	}
	if (client->swapped != masterClient->swapped) {
	    xReq       *req = (xReq *) client->requestBuffer;
	    char        n;

	    /* put length in client order */
	    swaps(&req->length, n);
	}

	if (!proxy->curRecv)
	{
	    DBG (DBG_CLIENT, (stderr, "No client on wire\n"));
	}
	else if (!LbxIsClientBlocked (client) && !lbxClient->reqs_pending) {
	    return ret;
	}
	else
	{
	    DBG (DBG_BLOCK, (stderr, "Stashing %d bytes for %d\n", 
				 ret, proxy->curRecv->index));
	    AppendFakeRequest (proxy->curRecv->client,
			       client->requestBuffer,
			       ret);
	    ++proxy->curRecv->reqs_pending;
	    if (!proxy->curRecv->input_blocked)
	    {
		proxy->curRecv->input_blocked = TRUE;
		QueueWorkProc (LbxWaitForUnblocked, proxy->curRecv->client, NULL);
	    }
	}
    }
}

/*
 * have to copy data off into pending buffer 
 */
void
LbxResetCurrentRequest(client)
    ClientPtr	client;
{
    LbxClientPtr lbxClient = LbxClient(client);
    LbxProxyPtr proxy;
    xReq       *req = (xReq *) client->requestBuffer;

    if (lbxClient) {
	proxy = lbxClient->proxy;
	AppendFakeRequest(client, client->requestBuffer, req->length * 4);

	++proxy->curRecv->reqs_pending;
	if (!proxy->curRecv->input_blocked) {
	    proxy->curRecv->input_blocked = TRUE;
	    QueueWorkProc(LbxWaitForUnblocked, proxy->curRecv->client, NULL);
	}
    } else {
	ResetCurrentRequest(client);
    }
}

/* ARGSUSED */
static int
LbxWriteEventToClient(client, ev)
    ClientPtr	client;
    xEvent	*ev;
{
    LbxClientPtr lbxClient = LbxClient(client);
    LbxProxyPtr proxy = lbxClient->proxy;
    int         len = sizeof(xEvent);
    char        evbuf[32];
    char       *bp = (char *) ev;

    /* see if this is extraneous motion */
    if (LbxThrottleMotionEvents(client, ev)) {
	lbxClient->bytes_in_reply -= sizeof(xEvent);
	return Success;
    }
    if (proxy->dosquishing) {
	len = LbxSquishEvent(proxy, ev, evbuf);
	bp = evbuf;
    }
    lbxClient->bytes_in_reply -= sizeof(xEvent);
    lbxAnyOutputPending = TRUE;
    return (*lbxClient->writeToClient) (client, len, bp);
}

/* determine how much data is being sent by server */
/* these are real lengths, not LBX lengths */
static int
reply_length(lbxClient, buf, len)
    LbxClientPtr	lbxClient;
    char	*buf;
{
    xReply     *rep = (xReply *) buf;
    char        n;

    if (rep->generic.type != X_Reply) {
	return len;		/* catch the multiple-event case */
    }
    if (lbxClient->awaiting_setup) {
	CARD16      slen = ((xConnSetupPrefix *) buf)->length;

	if (lbxClient->client->swapped) {
	    swaps(&slen, n)
	}
	return (2 + slen) << 2;
    } else {
	CARD32      slen = rep->generic.length;

	if (lbxClient->client->swapped) {
	    swapl(&slen, n)
	}
	return (8 + slen) << 2;
    }
}

/*
 * assumes that only events & errors might be grouped.  if anything
 * is somehow sending multiple replies in one WriteToClient() (which
 * shouldn't be legal) this will break
 */
int
LbxWriteToClient (client, len, buf)
    ClientPtr	client;
    int		len;
    char	*buf;
{
    LbxClientPtr lbxClient = LbxClient(client);
    xReply     *rep = (xReply *) buf;
    int         rlen = len,
                i,
                num;
    if (!len)
	return Success;

    /*
     * make sure we look at the start of a reply/event before trying to play
     * with it
     */
    if (lbxClient->bytes_in_reply == 0) {
	lbxClient->bytes_in_reply = reply_length(lbxClient, buf, len);

	/* catch any events (or errors) that can be tossed or squished */
	/* may be a whole mess of them, so break them up */
	if (rep->generic.type != X_Reply) {
	    num = len / 32;
	    for (i = 0; i < num; i++, buf += 32) {
		(void) LbxWriteEventToClient(client, (xEvent *) buf);
	    }
	    return Success;
	}
    } else {
	/*
	 * round length, since that's what reply len did, and extra reply is
	 * often not a word-multiple
	 */
	rlen = ((len + 3) >> 2) << 2;
    }
    lbxClient->bytes_in_reply -= rlen;
    lbxAnyOutputPending = TRUE;
    return (*lbxClient->writeToClient) (client, len, buf);
}

int
LbxUncompressedWriteToClient (client, len, buf)
    ClientPtr	client;
    int		len;
    char	*buf;
{
    LbxClientPtr    lbxClient = LbxClient(client);

    /* see if this is extraneous motion */
    /* XXX will this handle mutiple events at the same time? */
    if ((len == sizeof(xEvent)) && 
	LbxThrottleMotionEvents(client, (xEvent *) buf)) 
    {
	    return Success;
    }
    lbxAnyOutputPending = TRUE;
    return (*lbxClient->uncompressedWriteToClient) (client, len, buf);
}

Bool
LbxInitClient (proxy, client, index)
    LbxProxyPtr	proxy;
    ClientPtr	client;
    int		index;
{
    LbxClientPtr    lbxClient;
    
    lbxClient = (LbxClientPtr) xalloc (sizeof (LbxClientRec));
    if (!lbxClient)
	return FALSE;
    lbxClient->index = index;
    lbxClient->client = client;
    lbxClient->proxy = proxy;
    lbxClient->reply_remaining = 0;
    lbxClient->awaiting_setup = TRUE;
    lbxClient->needs_output_switch = FALSE;
    lbxClient->input_blocked = FALSE;
    lbxClient->reading_pending = FALSE;
    lbxClient->reqs_pending = 0;
    lbxClient->bytes_in_reply = 0;
    lbxClient->ignored = FALSE;
    lbxClient->writeToClient = client->public.writeToClient;
    lbxClient->uncompressedWriteToClient = client->public.uncompressedWriteToClient;
    lbxClient->readRequest = client->public.readRequest;
    bzero (lbxClient->drawableCache, sizeof (lbxClient->drawableCache));
    bzero (lbxClient->gcontextCache, sizeof (lbxClient->gcontextCache));
    lbxClients[client->index] = lbxClient;
    proxy->lbxClients[index] = lbxClient;
    proxy->numClients++;
    client->public.writeToClient = LbxWriteToClient;
    client->public.readRequest = LbxReadRequestFromClient;
    client->public.uncompressedWriteToClient = LbxUncompressedWriteToClient;
#ifdef notused
    client->public.requestLength = LbxRequestLength;
#endif
    return TRUE;
}

void
LbxFreeClient (client)
    ClientPtr	client;
{
    LbxClientPtr    lbxClient = LbxClient(client);
    LbxProxyPtr	    proxy = lbxClient->proxy;

    if (lbxClient == proxy->curSend)
	proxy->curSend = 0;

    if (lbxClient != proxy->lbxClients[0]) {
	if (lbxClient == proxy->curRecv) {
	    LbxSwitchRecv(proxy, NULL);
	}
	else if (lbxClient == proxy->curDix) {
	    LbxSwitchRecv(proxy, proxy->curRecv);
	}
    }
    else
	proxy->curSend = 0;
	
    --proxy->numClients;
    proxy->lbxClients[lbxClient->index] = 0;
    lbxClients[client->index] = 0;
    client->public.writeToClient = lbxClient->writeToClient;
    client->public.readRequest = lbxClient->readRequest;
    xfree (lbxClient);
}

static void
LbxFreeProxy (proxy)
    LbxProxyPtr proxy;
{
    LbxProxyPtr *p;

    LBXFreeDeltaCache(&proxy->indeltas);
    LBXFreeDeltaCache(&proxy->outdeltas);
    if (proxy->tempDeltaBuf)
	xfree(proxy->tempDeltaBuf);
    if (proxy->tempEventBuf)
	xfree(proxy->tempEventBuf);
    if (proxy->compHandle)
	LzwFree(proxy->compHandle);
    for (p = &proxyList; *p; p = &(*p)->next) {
	if (*p == proxy) {
	    *p = proxy->next;
	    break;
	}
    }
    xfree (proxy);
}

void
LbxShutdownProxy (proxy)
    LbxProxyPtr	proxy;
{
    int		    i;
    ClientPtr	    client;

    for (i = 0; i < MAX_LBX_CLIENTS; i++)
    {
	if (proxy->lbxClients[i])
	{
	    client = proxy->lbxClients[i]->client;
	    if (!client->clientGone)
		CloseDownClient (client);
	}
    }
    LbxFlushTags(proxy);
    if (proxy->compHandle)
	--lbxCompressWorkProcCount;
    LbxFreeProxy(proxy);
    if (!--lbxBlockHandlerCount)
	RemoveBlockAndWakeupHandlers (LbxBlockHandler, LbxWakeupHandler, NULL);
}

#ifdef notused
/* figures out request length in proxy byte order */
unsigned long
LbxRequestLength(req, client, got, partp)
    xReq	*req;
    ClientPtr	client;
    int		got;
    Bool    	*partp;
{
    unsigned long len;
    LbxProxyPtr proxy;
    ClientPtr   cp = client;
    int         pid;

#define MAXBUFSIZE (1 << 22)

    if (!req)
	req = (xReq *) client->requestBuffer;
    if (got < sizeof(xReq)) {
	*partp = TRUE;
	return sizeof(xReq);
    }
    pid = LbxProxyID(client);
    if (pid)
	proxy = LbxPidToProxy(pid);
    if (proxy)
	cp = proxy->lbxClients[LbxMasterClientIndex]->client;
    len = ((cp->swapped ? lswaps(req->length) : req->length) << 2);

    if (len > MAXBUFSIZE) {
	*partp = TRUE;
	return -1;
    }
    *partp = FALSE;

/*
    client->req_len = len >> 2;
*/
    return len;
}
#endif

int
ProcLbxQueryVersion(client)
    register ClientPtr client;
{
    REQUEST(xLbxQueryVersionReq);
    xLbxQueryVersionReply rep;
    register int n;

    REQUEST_SIZE_MATCH(xLbxQueryVersionReq);
    rep.type = X_Reply;
    rep.length = 0;
    rep.sequenceNumber = client->sequence;
    rep.majorVersion = LBX_MAJOR_VERSION;
    rep.minorVersion = LBX_MINOR_VERSION;
    if (client->swapped) {
    	swaps(&rep.sequenceNumber, n);
    	swapl(&rep.length, n);
	swaps(&rep.majorVersion, n);
	swaps(&rep.minorVersion, n);
    }
    WriteToClient(client, sizeof(xLbxQueryVersionReply), (char *)&rep);
    return (client->noClientException);
}

int Writev(fd, iov, iovcnt)
    int	fd;
    struct iovec *iov;
    int iovcnt;
{
    return writev(fd, iov, iovcnt);
}

int
ProcLbxStartProxy(client)
    register ClientPtr	client;
{
    REQUEST(xLbxStartProxyReq);
    LbxProxyPtr	    proxy;
    LbxClientPtr    lbxClient;
    int		    reqlen;
    int		    replylen;
    xLbxStartReply  *replybuf;
    LbxNegOptsRec   negopt;
    register int    n;

    REQUEST_AT_LEAST_SIZE(xLbxStartProxyReq);
    if (lbxClients[client->index])
	return BadLbxClient;
    proxy = (LbxProxyPtr) xalloc (sizeof (LbxProxyRec));
    if (!proxy)
	return BadAlloc;
    bzero(proxy, sizeof (LbxProxyRec));
    proxy->next = proxyList;
    proxy->pid = client->index;	/* XXX have to be sure this doesn't go over
    				 * MAX_NUM_PROXIES */
    proxyList = proxy;

    /*
     * Don't know exactly how big the reply will be, but it won't be
     * bigger than the request
     */
    reqlen = stuff->length << 2;
    replybuf = (xLbxStartReply *) xalloc(max(reqlen, sz_xLbxStartReply));
    if (!replybuf) {
	LbxFreeProxy(proxy);
	return BadAlloc;
    }

    LbxOptionInit(&negopt);

    replylen = LbxOptionParse(&negopt,
			      &stuff[1],
			      reqlen - sz_xLbxStartProxyReq,
			      &replybuf->optDataStart);
    if (replylen < 0) {
	/*
	 * Didn't understand option format, so we'll just end up
	 * using the defaults.  Set nopts so that the proxy will
	 * be informed that we rejected the options because of
	 * decoding problems.
	 */
	LbxOptionInit(&negopt);
	negopt.nopts = 0xff;
	replylen = 0;
    }

    if (LBXInitDeltaCache(&proxy->indeltas, negopt.proxyDeltaN,
			  negopt.proxyDeltaMaxLen) < 0
			||
	LBXInitDeltaCache(&proxy->outdeltas, negopt.serverDeltaN,
			  negopt.serverDeltaMaxLen) < 0) {
	LbxFreeProxy(proxy);
	xfree(replybuf);
	return BadAlloc;
    }

    n = 0;
    if (negopt.proxyDeltaN)
	n = negopt.proxyDeltaMaxLen;
    if (negopt.serverDeltaN && negopt.serverDeltaMaxLen > n)
	n = negopt.serverDeltaMaxLen;
    if (n) {
	if ((proxy->tempDeltaBuf = (unsigned char *)xalloc (n)) == NULL) {
	    LbxFreeProxy(proxy);
	    xfree(replybuf);
	    return BadAlloc;
	}
    }

#ifndef NCD
    MakeClientGrabImpervious(client);	/* proxy needs to be grab-proof */
#else
    AddGrabproofClient(client);	/* proxy needs to be grab-proof */
#endif
    if ((proxy->tempEventBuf = (unsigned char *)
		xalloc (max(MAXDELTASIZE, sizeof (xLbxEvent)))) == NULL) {
	LbxFreeProxy(proxy);
	xfree(replybuf);
	return BadAlloc;
    }
    proxy->fd = ClientConnectionNumber(client);
    if (negopt.streamCompInit) {
	proxy->compHandle =
	    (*negopt.streamCompInit)(proxy->fd, negopt.streamCompArg);
	if (proxy->compHandle == NULL) {
	    LbxFreeProxy(proxy);
	    xfree(replybuf);
	    return BadAlloc;
	}
    }
    if (!LbxInitClient (proxy, client, 0))
    {
	LbxFreeProxy(proxy);
	xfree(replybuf);
	return BadAlloc;
    }
    proxy->dosquishing = negopt.squish;

    /* send reply */
    replybuf->type = X_Reply;
    replybuf->nOpts = negopt.nopts;
    replybuf->sequenceNumber = client->sequence;
    replylen += sz_xLbxStartReplyHdr;
    if (replylen < sz_xLbxStartReply)
	replylen = sz_xLbxStartReply;
    replybuf->length = (replylen - sz_xLbxStartReply + 3) >> 2;
    if (client->swapped) {
	swaps(&replybuf->sequenceNumber, n);
	swapl(&replybuf->length, n);
    }
    lbxClient = LbxClient(client);
    (*lbxClient->writeToClient) (client, replylen, (char *)replybuf);
    FlushAllOutput();	/* what if entire reply doesn't get out here????? */
#ifndef NDEBUG
    if (PendingClientOutput(client))
	fprintf(stderr, "Warning! StartProxy reply not transmitted\n");
#endif

    if (proxy->compHandle) {
	int  len = client->req_len << 2;
	int  left = BytesInClientBuffer (client);
	char *extra = ((char *) stuff) + len;
	LzwStuffInput (proxy->fd, extra, left);
	SkipInClientBuffer (client, left + len, 0);
	proxy->writev = LzwWriteV;
	proxy->read = LzwRead;
	StartOutputCompression (client, LbxCompressOn, LbxCompressOff);
    }
    else {
	proxy->read = read;
	proxy->writev = Writev;
    }

    SwitchConnectionFuncs (client, LbxRead, LbxWritev, LbxCloseClient);
    lbxClient = proxy->lbxClients[0];
    proxy->curDix = lbxClient;
    proxy->curRecv = lbxClient;
    proxy->curSend = lbxClient;
    lbxClient->awaiting_setup = FALSE;
    if (!lbxBlockHandlerCount++)
	RegisterBlockAndWakeupHandlers(LbxBlockHandler, LbxWakeupHandler, NULL);
    if (proxy->compHandle) {
	if (!lbxCompressWorkProcCount++)
	    QueueWorkProc (LbxCheckCompressInput, NULL, NULL);
    }

    xfree(replybuf);
    return Success;
}

int
ProcLbxStopProxy(client)
    register ClientPtr	client;
{
    REQUEST(xLbxStopProxyReq);
    LbxProxyPtr	    proxy;
    LbxClientPtr    lbxClient = LbxClient(client);

    REQUEST_SIZE_MATCH(xLbxStopProxyReq);
    if (!lbxClient)
	return BadLbxClient;
    if (lbxClient->index != LbxMasterClientIndex)
	return BadLbxClient;
    
    proxy = lbxClient->proxy;
    LbxFreeClient (client);
    LbxShutdownProxy (proxy);
    return Success;
}
    
int
ProcLbxSwitch(client)
    register ClientPtr	client;
{
    REQUEST(xLbxSwitchReq);
    LbxProxyPtr	proxy = LbxMaybeProxy(client);

    if (!proxy)
	return BadLbxClient;
    if (stuff->client >= MAX_LBX_CLIENTS || !proxy->lbxClients[stuff->client])
    {
	LbxSwitchRecv (proxy, NULL);
	return BadLbxClient;
    }
    LbxSwitchRecv (proxy, proxy->lbxClients[stuff->client]);
    return Success;
}
    
int
ProcLbxNewClient(client)
    register ClientPtr client;
{
    REQUEST(xLbxNewClientReq);
    ClientPtr	    newClient, AllocNewConnection ();
    LbxProxyPtr	    proxy = LbxMaybeProxy(client);
    LbxClientPtr    newLbxClient;
    int		    c;
    int		    len;
    char	    *setupbuf;

    if (stuff->client >= MAX_LBX_CLIENTS || 
	!proxy || proxy->lbxClients[stuff->client])
	return BadLbxClientCode;

    /* save info before our request disappears */
    c = stuff->client;
    len = (stuff->length << 2) - sizeof(xLbxNewClientReq);
    setupbuf = (char *)xalloc (len);
    if (!setupbuf)
	return BadAlloc;
    bcopy ((char *)&stuff[1], setupbuf, len);

    newClient = AllocNewConnection (
#ifndef NCD	/* R6-ism */
    ClientTransportObject(client),
#endif
				    ClientConnectionNumber (client), 
				    LbxRead, LbxWritev, LbxCloseClient);
    if (!newClient)
	return BadAlloc;
    if (((xLbxConnClientPrefix *)setupbuf)->useTag)
	newClient->requestVector = LbxInitialVector;
    if (proxy->compHandle)
	StartOutputCompression (newClient, LbxCompressOn, LbxCompressOff);
    if (!LbxInitClient (proxy, newClient, c))
    {
	CloseDownClient (newClient);
	return BadAlloc;
    }
    newLbxClient = proxy->lbxClients[c];
    
    AppendFakeRequest (newClient, setupbuf, len);
    xfree (setupbuf);
    QueueWorkProc (LbxWaitForUnblocked, newClient, NULL);

    DBG (DBG_CLIENT, (stderr, "lbxNewClient X %d lbx %d\n", newClient->index, newLbxClient->index));
    return Success;
}

int
ProcLbxEstablishConnection(client)
    register ClientPtr client;
{
    char *reason, *auth_proto, *auth_string;
    register xConnClientPrefix *prefix;
    REQUEST(xReq);

    prefix = (xConnClientPrefix *)((char *)stuff + sz_xReq);
    auth_proto = (char *)prefix + sz_xConnClientPrefix;
    auth_string = auth_proto + ((prefix->nbytesAuthProto + 3) & ~3);
    if ((prefix->majorVersion != X_PROTOCOL) ||
	(prefix->minorVersion != X_PROTOCOL_REVISION))
	reason = "Protocol version mismatch";
    else
	reason = ClientAuthorized(client,
				  (unsigned short)prefix->nbytesAuthProto,
				  auth_proto,
				  (unsigned short)prefix->nbytesAuthString,
				  auth_string);
    /*
     * if auth protocol does some magic, fall back through to the
     * dispatcher.
     */
#ifndef NCD
    if (client->clientState == ClientStateInitial)
	return(LbxSendConnSetup(client, reason));
    return(client->noClientException);
#else
    return(LbxSendConnSetup(client, reason));
#endif
}

int
ProcLbxCloseClient (client)
    register ClientPtr	client;
{
    REQUEST(xLbxCloseClientReq);
    LbxProxyPtr	    proxy = LbxMaybeProxy(client);

    if (stuff->client >= MAX_LBX_CLIENTS || 
	!proxy || !proxy->lbxClients[stuff->client])
	return BadLbxClientCode;

    /* this will cause the client to be closed down back in Dispatch() */
    return(client->noClientException = CloseLbxClient);
}

int
ProcLbxModifySequence (client)
    register ClientPtr	client;
{
    REQUEST(xLbxModifySequenceReq);

    client->sequence += (stuff->adjust - 1);	/* Dispatch() adds 1 */
    return Success;
}

int
ProcLbxAllowMotion (client)
    register ClientPtr	client;
{
    REQUEST(xLbxAllowMotionReq);

    client->sequence--;
    LbxAllowMotion(client, stuff->num);
    return Success;
}

int
ProcLbxIncrementPixel(client)
    register ClientPtr	client;
{
    REQUEST(xLbxIncrementPixelReq);

    LbxIncrementPixel(client->index, stuff->cmap, stuff->pixel, stuff->amount);
    return Success;
}

int
ProcLbxDelta(client)
    register ClientPtr	client;
{
    REQUEST(xLbxDeltaReq);
    LbxClientPtr    lbxClient = LbxClient(client);
    LbxProxyPtr	    proxy = lbxClient->proxy;
    int		    len;
    char	    *buf;

    /* Note that LBXDecodeDelta decodes and adds current msg to the cache */
    len = LBXDecodeDelta(&proxy->indeltas, ((char *)stuff) + sz_xLbxDeltaReq,
			 stuff->diffs, stuff->cindex, &buf);
    /*
     * Some requests, such as FillPoly, result in the protocol input
     * buffer being modified.  So we need to copy the request
     * into a temporary buffer where a write would be harmless.
     * Maybe some day do this copying on a case by case basis,
     * since not all requests are guilty of this.
     */
    bcopy(buf, proxy->tempDeltaBuf, len);

    client->requestBuffer = proxy->tempDeltaBuf;
    client->req_len = len >> 2;
    return len;
}

int
ProcLbxGetModifierMapping(client)
    ClientPtr	client;
{
    REQUEST(xLbxGetModifierMappingReq);

    return LbxGetModifierMapping(client);
}

int
ProcLbxGetKeyboardMapping(client)
    ClientPtr	client;
{
    REQUEST(xLbxGetKeyboardMappingReq);

    return LbxGetKeyboardMapping(client);
}

int
ProcLbxQueryFont(client)
    ClientPtr	client;
{
    REQUEST(xLbxQueryFontReq);

    return LbxQueryFont(client);
}

int
ProcLbxChangeProperty(client)
    ClientPtr	client;
{
    REQUEST(xLbxChangePropertyReq);

    return LbxChangeProperty(client);
}

ProcLbxGetProperty(client)
    ClientPtr	client;
{
    REQUEST(xLbxGetPropertyReq);

    return LbxGetProperty(client);
}

ProcLbxTagData(client)
    ClientPtr	client;
{
    REQUEST(xLbxTagDataReq);

    client->sequence--;		/* not a counted request */

    return LbxTagData(client, stuff->tag, stuff->real_length,
    		 (pointer)&stuff[1]);	/* better not give any errors */
}

ProcLbxQueryTag(client)
    ClientPtr	client;
{
    REQUEST(xLbxQueryTagReq);

    client->sequence--;
    return LbxQueryTag(client, stuff->tag);
}

ProcLbxInvalidateTag(client)
    ClientPtr	client;
{
    REQUEST(xLbxInvalidateTagReq);

    client->sequence--;
    return LbxInvalidateTag(client, stuff->tag);
}

int
ProcLbxPolyPoint(client)
    register ClientPtr	client;
{
    return LbxDecodePoly(client, X_PolyPoint, LbxDecodePoints);
}

int
ProcLbxPolyLine(client)
    register ClientPtr	client;
{
    return LbxDecodePoly(client, X_PolyLine, LbxDecodePoints);
}

int
ProcLbxPolySegment(client)
    register ClientPtr	client;
{
    return LbxDecodePoly(client, X_PolySegment, LbxDecodeSegment);
}

int
ProcLbxPolyRectangle(client)
    register ClientPtr	client;
{
    return LbxDecodePoly(client, X_PolyRectangle, LbxDecodeRectangle);
}

int
ProcLbxPolyArc(client)
    register ClientPtr	client;
{
    return LbxDecodePoly(client, X_PolyArc, LbxDecodeArc);
}

int
ProcLbxFillPoly(client)
    register ClientPtr	client;
{
    return LbxDecodeFillPoly(client);
}

int
ProcLbxPolyFillRectangle(client)
    register ClientPtr	client;
{
    return LbxDecodePoly(client, X_PolyFillRectangle, LbxDecodeRectangle);
}

int
ProcLbxPolyFillArc(client)
    register ClientPtr	client;
{
    return LbxDecodePoly(client, X_PolyFillArc, LbxDecodeArc);
}

int
ProcLbxCopyArea (client)
    register ClientPtr	client;
{
    return LbxDecodeCopyArea(client);
}

int
ProcLbxCopyPlane (client)
    register ClientPtr	client;
{
    return LbxDecodeCopyPlane(client);
}


int
ProcLbxPolyText (client)
    register ClientPtr	client;
{
    return LbxDecodePolyText(client);
}

int
ProcLbxImageText (client)
    register ClientPtr	client;
{
    return LbxDecodeImageText(client);
}

int
ProcLbxQueryExtension(client)
    ClientPtr	client;
{
    REQUEST(xLbxQueryExtensionReq);
    char	*ename;

    ename = (char *) &stuff[1];
    return LbxQueryExtension(client, ename, stuff->nbytes);
}

int
ProcLbxPutImage(client)
    register ClientPtr	client;
{
    return LbxDecodePutImage(client);
}

int
ProcLbxGetImage(client)
    register ClientPtr	client;
{
    return LbxDecodeGetImage(client);
}

int
ProcLbxDispatch (client)
    register ClientPtr	client;
{
    REQUEST(xReq);
    switch (stuff->data)
    {
    case X_LbxQueryVersion:
	return ProcLbxQueryVersion(client);
    case X_LbxStartProxy:
	return ProcLbxStartProxy(client);
    case X_LbxStopProxy:
	return ProcLbxStopProxy(client);
    case X_LbxSwitch:
	return ProcLbxSwitch(client);
    case X_LbxNewClient:
	return ProcLbxNewClient(client);
    case X_LbxCloseClient:
	return ProcLbxCloseClient(client);
    case X_LbxModifySequence:
	return ProcLbxModifySequence(client);
    case X_LbxAllowMotion:
	return ProcLbxAllowMotion(client);
    case X_LbxIncrementPixel:
	return ProcLbxIncrementPixel(client);
    case X_LbxDelta:
	return ProcLbxDelta (client);
    case X_LbxGetModifierMapping:
	return ProcLbxGetModifierMapping(client);
    case X_LbxGetKeyboardMapping:
	return ProcLbxGetKeyboardMapping(client);
    case X_LbxQueryTag:
	return ProcLbxQueryTag(client);
    case X_LbxInvalidateTag:
	return ProcLbxInvalidateTag(client);
    case X_LbxPolyPoint:
	return ProcLbxPolyPoint (client);
    case X_LbxPolyLine:
	return ProcLbxPolyLine (client);
    case X_LbxPolySegment:
	return ProcLbxPolySegment (client);
    case X_LbxPolyRectangle:
	return ProcLbxPolyRectangle (client);
    case X_LbxPolyArc:
	return ProcLbxPolyArc (client);
    case X_LbxFillPoly:
	return ProcLbxFillPoly (client);
    case X_LbxPolyFillRectangle:
	return ProcLbxPolyFillRectangle (client);
    case X_LbxPolyFillArc:
	return ProcLbxPolyFillArc (client);
    case X_LbxQueryFont:
	return ProcLbxQueryFont (client);
    case X_LbxChangeProperty:
	return ProcLbxChangeProperty (client);
    case X_LbxGetProperty:
	return ProcLbxGetProperty (client);
    case X_LbxTagData:
	return ProcLbxTagData (client);
    case X_LbxCopyArea:
	return ProcLbxCopyArea (client);
    case X_LbxCopyPlane:
	return ProcLbxCopyPlane (client);
    case X_LbxPolyText8:
    case X_LbxPolyText16:
	return ProcLbxPolyText (client);
    case X_LbxImageText8:
    case X_LbxImageText16:
	return ProcLbxImageText (client);
    case X_LbxQueryExtension:
	return ProcLbxQueryExtension (client);
    case X_LbxPutImage:
	return ProcLbxPutImage (client);
    case X_LbxGetImage:
	return ProcLbxGetImage (client);
    default:
	return BadRequest;
    }
}

#ifndef NDEBUG
LbxDumpBuffer(title, data, size)
    char *title;
    char *data;
    int  size;
{
    int i;
    char outbuf[80];

    fprintf(stderr, "%s of %d bytes:\n", title, size);
    for (i=0, outbuf[0] = '\0'; i<size; ++i) {
        sprintf(&outbuf[strlen(outbuf)], " %02X", data[i]&0xFF);
        if (15 == (i%16)) {
            fprintf(stderr, "%s\n", outbuf);
            outbuf[0] = '\0';
        }
    }
    if (strlen(outbuf))
        fprintf(stderr, "%s\n", outbuf);
}
#endif
