/***********************************************************

Copyright (c) 1987, 1989  X Consortium

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


Copyright 1987, 1989 by Digital Equipment Corporation, Maynard, Massachusetts.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the name of Digital not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.  

DIGITAL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/
/* $XConsortium: io.c,v 1.4 94/04/17 21:17:13 dpw Exp $ */
/*****************************************************************
 * i/o functions
 *
 *   WriteToClient, ReadRequestFromClient
 *   InsertFakeRequest, ResetCurrentRequest
 *
 *****************************************************************/

#include <stdio.h>
#include "Xos.h"
#ifdef X_NOT_STDC_ENV
extern int errno;
#endif
#include "Xmd.h"
#include <errno.h>
#include <sys/param.h>
#include <sys/uio.h>
#include "X.h"
#include "Xproto.h"
#include "os.h"
#include "osdep.h"
#include "opaque.h"
#include "dixstruct.h"
#include "misc.h"

/* check for both EAGAIN and EWOULDBLOCK, because some supposedly POSIX
 * systems are broken and return EWOULDBLOCK when they should return EAGAIN
 */
#if defined(EAGAIN) && defined(EWOULDBLOCK)
#define ETEST(err) (err == EAGAIN || err == EWOULDBLOCK)
#else
#ifdef EAGAIN
#define ETEST(err) (err == EAGAIN)
#else
#define ETEST(err) (err == EWOULDBLOCK)
#endif
#endif

extern void MarkClientException();
extern FdSet ClientsWithInput;
extern FdSet ClientsWriteBlocked;
extern FdSet OutputPending;
extern int ConnectionTranslation[];
extern int ConnectionOutputTranslation[];
extern Bool NewOutputPending;
extern Bool AnyClientsWriteBlocked;
static Bool CriticalOutputPending;
static int timesThisConnection = 0;
static ConnectionInputPtr FreeInputs = (ConnectionInputPtr)NULL;
static ConnectionOutputPtr FreeOutputs = (ConnectionOutputPtr)NULL;
static OsCommPtr AvailableInput = (OsCommPtr)NULL;

static ConnectionInputPtr AllocateInputBuffer(
#if NeedFunctionPrototypes
    void
#endif
);
static ConnectionOutputPtr AllocateOutputBuffer(
#if NeedFunctionPrototypes
    void
#endif
);
static ConnectionOutputPtr AllocateUncompBuffer();

ClientPtr   ReadingClient;
ClientPtr   WritingClient;

#ifdef LBX
#define get_req_len(req,cli) (((cli)->swapped ? \
			      lswaps((req)->length) : (req)->length) << 2)

#else
#define get_req_len(req,cli) ((cli)->swapped ? \
			      lswaps((req)->length) : (req)->length)
#endif

unsigned long
StandardRequestLength(req,client,got,partp)
    xReq	*req;
    ClientPtr	client;
    int		got;
    Bool    	*partp;
{
    int	    len;
    
    if (!req)
	req = (xReq *) client->requestBuffer;
    if (got < sizeof (xReq))
    {
	*partp = TRUE;
	return sizeof (xReq);
    }
    len = get_req_len(req,client);
    if (len > MAXBUFSIZE)
    {
	*partp = TRUE;
	return -1;
    }
    *partp = FALSE;
    return len;
}

#ifdef BIGREQS
typedef struct {
	CARD8 reqType;
	CARD8 data;
	CARD16 zero B16;
        CARD32 length B32;
} xBigReq;

#define get_big_req_len(req,cli) ((cli)->swapped ? \
				  lswapl(((xBigReq *)(req))->length) : \
				  ((xBigReq *)(req))->length)
#endif

#define MAX_TIMES_PER         10

/*****************************************************************
 * ReadRequestFromClient
 *    Returns one request in client->requestBuffer.  Return status is:
 *
 *    > 0  if  successful, specifies length in bytes of the request
 *    = 0  if  entire request is not yet available
 *    < 0  if  client should be terminated
 *
 *    The request returned must be contiguous so that it can be
 *    cast in the dispatcher to the correct request type.  Because requests
 *    are variable length, ReadRequestFromClient() must look at the first 4
 *    bytes of a request to determine the length (the request length is
 *    always the 3rd and 4th bytes of the request).  
 *
 *    Note: in order to make the server scheduler (WaitForSomething())
 *    "fair", the ClientsWithInput mask is used.  This mask tells which
 *    clients have FULL requests left in their buffers.  Clients with
 *    partial requests require a read.  Basically, client buffers
 *    are drained before select() is called again.  But, we can't keep
 *    reading from a client that is sending buckets of data (or has
 *    a partial request) because others clients need to be scheduled.
 *****************************************************************/

#define YieldControl()				\
        { isItTimeToYield = TRUE;		\
	  timesThisConnection = 0; }
#define YieldControlNoInput()			\
        { YieldControl();			\
	  BITCLEAR(ClientsWithInput, fd); }
#define YieldControlDeath()			\
        { timesThisConnection = 0; }

int
StandardReadRequestFromClient(client)
    ClientPtr client;
{
    OsCommPtr oc = (OsCommPtr)client->osPrivate;
    register ConnectionInputPtr oci = oc->input;
    int fd = oc->fd;
    register int gotnow, needed;
    int result;
    register xReq *request;
    Bool need_header;
    int	nextneeds;
    Bool part;
#ifdef BIGREQS
    Bool move_header;
#endif

    if (AvailableInput)
    {
	if (AvailableInput != oc)
	{
	    register ConnectionInputPtr aci = AvailableInput->input;
	    if (aci->size > BUFWATERMARK)
	    {
		xfree(aci->buffer);
		xfree(aci);
	    }
	    else
	    {
		aci->next = FreeInputs;
		FreeInputs = aci;
	    }
	    AvailableInput->input = (ConnectionInputPtr)NULL;
	}
	AvailableInput = (OsCommPtr)NULL;
    }
    if (!oci)
    {
	if (oci = FreeInputs)
	{
	    FreeInputs = oci->next;
	}
	else if (!(oci = AllocateInputBuffer()))
	{
	    YieldControlDeath();
	    return -1;
	}
	oc->input = oci;
    }
    oci->bufptr += oci->lenLastReq;
    oci->lenLastReq = 0;

    need_header = FALSE;
#ifdef BIGREQS
    move_header = FALSE;
#endif
    gotnow = oci->bufcnt + oci->buffer - oci->bufptr;
#ifdef LBX
    client->requestBuffer = (pointer)oci->bufptr;
    needed = RequestLength (NULL, client, gotnow, &part);
    client->req_len = needed >> 2;
#else
    if (gotnow < sizeof(xReq))
    {
	needed = sizeof(xReq);
	need_header = TRUE;
    }
    else
    {
	request = (xReq *)oci->bufptr;
	needed = get_req_len(request, client);
#ifdef BIGREQS
	if (!needed && client->big_requests)
	{
	    move_header = TRUE;
	    if (gotnow < sizeof(xBigReq))
	    {
		needed = sizeof(xBigReq) >> 2;
		need_header = TRUE;
	    }
	    else
		needed = get_big_req_len(request, client);
	}
#endif
	client->req_len = needed;
	needed <<= 2;
    }
#endif	/* LBX */
    if (gotnow < needed
#ifdef LBX
	|| part
#endif
    )
    {
#ifndef LBX
	oci->lenLastReq = 0;
	if (needed > MAXBUFSIZE)
#else
	if (needed == -1)
#endif
	{
	    YieldControlDeath();
	    return -1;
	}
	if ((gotnow == 0) ||
	    ((oci->bufptr - oci->buffer + needed) > oci->size))
	{
	    if ((gotnow > 0) && (oci->bufptr != oci->buffer))
		memmove(oci->buffer, oci->bufptr, gotnow);
	    if (needed > oci->size)
	    {
		char *ibuf;

		ibuf = (char *)xrealloc(oci->buffer, needed);
		if (!ibuf)
		{
		    YieldControlDeath();
		    return -1;
		}
		oci->size = needed;
		oci->buffer = ibuf;
	    }
	    oci->bufptr = oci->buffer;
	    oci->bufcnt = gotnow;
	}
#ifdef LBX
	ReadingClient = client;
	result = (*oc->Read)(fd, oci->buffer + oci->bufcnt, 
		      oci->size - oci->bufcnt); 
#else
	result = read(fd, oci->buffer + oci->bufcnt, 
		      oci->size - oci->bufcnt); 
#endif
	if (result <= 0)
	{
	    if ((result < 0) && ETEST(errno))
	    {
		YieldControlNoInput();
		return 0;
	    }
	    YieldControlDeath();
	    return -1;
	}
	oci->bufcnt += result;
	gotnow += result;
	/* free up some space after huge requests */
	if ((oci->size > BUFWATERMARK) &&
	    (oci->bufcnt < BUFSIZE) && (needed < BUFSIZE))
	{
	    char *ibuf;

	    ibuf = (char *)xrealloc(oci->buffer, BUFSIZE);
	    if (ibuf)
	    {
		oci->size = BUFSIZE;
		oci->buffer = ibuf;
		oci->bufptr = ibuf + oci->bufcnt - gotnow;
	    }
	}
#ifdef LBX
	client->requestBuffer = (pointer) oci->bufptr;
#endif
	if (
#ifdef LBX
	    part &&
#else
	    need_header &&
#endif
	    gotnow >= needed)
	{
#ifdef LBX
	    needed = RequestLength (NULL, client, gotnow, &part);
	    client->req_len = needed >> 2;
#else
	    request = (xReq *)oci->bufptr;
	    needed = get_req_len(request, client);
#ifdef BIGREQS
	    if (!needed && client->big_requests)
	    {
		move_header = TRUE;
		if (gotnow < sizeof(xBigReq))
		    needed = sizeof(xBigReq) >> 2;
		else
		    needed = get_big_req_len(request, client);
	    }
#endif
	    client->req_len = needed;
	    needed <<= 2;
#endif	/* !LBX */
	}
	if (gotnow < needed
#ifdef LBX
		|| part
#endif
	)
	{
#ifdef LBX
	    if (needed == -1)
	    {
		YieldControlDeath();
		return -1;
	    }
#endif
	    YieldControlNoInput();
	    return 0;
	}
    }
#ifndef LBX
    if (needed == 0)
    {
#ifdef BIGREQS
	if (client->big_requests)
	    needed = sizeof(xBigReq);
	else
#endif
	    needed = sizeof(xReq);
    }
#endif	/* !LBX */
    oci->lenLastReq = needed;

    /*
     *  Check to see if client has at least one whole request in the
     *  buffer.  If there is only a partial request, treat like buffer
     *  is empty so that select() will be called again and other clients
     *  can get into the queue.   
     */

#ifdef LBX
    if (gotnow > needed)
    {
	request = (xReq *)(oci->bufptr + needed);
	nextneeds = RequestLength (request, client, gotnow - needed, &part);
	if (gotnow >= needed + nextneeds && !part)
	    BITSET(ClientsWithInput, fd);
	else
	    YieldControlNoInput();
    }
#else
    gotnow -= needed;
    if (gotnow >= sizeof(xReq)) 
    {
	request = (xReq *)(oci->bufptr + needed);
	if (gotnow >= (result = (get_req_len(request, client) << 2))
#ifdef BIGREQS
	    && (result ||
		(client->big_requests &&
		 (gotnow >= sizeof(xBigReq) &&
		  gotnow >= (get_big_req_len(request, client) << 2))))
#endif
	    )
	    BITSET(ClientsWithInput, fd);
	else
	    YieldControlNoInput();
    }
#endif	/* !LBX */
    else
    {
#ifndef LBX
	if (!gotnow)
#endif
	    AvailableInput = oc;
	YieldControlNoInput();
    }
    if (++timesThisConnection >= MAX_TIMES_PER)
	YieldControl();
#ifdef BIGREQS
    if (move_header)
    {
	request = (xReq *)oci->bufptr;
	oci->bufptr += (sizeof(xBigReq) - sizeof(xReq));
	*(xReq *)oci->bufptr = *request;
	oci->lenLastReq -= (sizeof(xBigReq) - sizeof(xReq));
	client->req_len -= (sizeof(xBigReq) - sizeof(xReq)) >> 2;
    }
#endif
#ifndef LBX
    client->requestBuffer = (pointer)oci->bufptr;
#endif
    return needed;
}

Bool
SwitchClientInput (from, to, preserve)
    ClientPtr	from, to;
    int		preserve;	/* bytes to preserve for from */
{
    OsCommPtr ocFrom = (OsCommPtr) from->osPrivate;
    OsCommPtr ocTo = (OsCommPtr) to->osPrivate;
    ConnectionInputPtr	ociFrom = ocFrom->input;
    ConnectionInputPtr	ociTo = ocTo->input;
    int			remain;
    int			gotnowFrom, gotnowTo;
    int			needed;
    
    if (ociTo && !ociTo->bufcnt)
    {
	xfree (ociTo->buffer);
	xfree (ociTo);
	ociTo = NULL;
    }
    if (ociFrom)
    {
	ociFrom->bufptr += ociFrom->lenLastReq;
	ociFrom->lenLastReq = 0;
    }
    if (ociTo)
    {
	ociTo->bufptr += ociTo->lenLastReq;
	ociTo->lenLastReq = 0;
    }
    ConnectionTranslation[ocTo->fd] = to->index;
    YieldControl();
    if (!preserve && !ociTo)
    {
	ocTo->input = ociFrom;
	ocFrom->input = NULL;
	if (AvailableInput == ocFrom)
	    AvailableInput = ocTo;
	return TRUE;
    }
    gotnowFrom = 0;
    if (ociFrom)
	gotnowFrom = ociFrom->bufcnt + ociFrom->buffer - ociFrom->bufptr;
    remain = gotnowFrom - preserve;
    if (!ociTo)
    {
	ociTo = AllocateInputBuffer ();
	if (!ociTo)
	    return FALSE;
	ocTo->input = ociTo;
    }
    /* Append any data in the from buffer onto the end of the to buffer */
    needed = ociTo->bufcnt + remain;
    if (needed > ociTo->size)
    {
	gotnowTo = ociTo->bufcnt + ociTo->buffer - ociTo->bufptr;
	/* move any existing data to the begining */
	if (gotnowTo > 0 && ociTo->bufptr != ociTo->buffer)
	    bcopy (ociTo->bufptr, ociTo->buffer, gotnowTo);
	needed = gotnowTo + remain;
	/* if we really do need more space, realloc */
	if (needed > ociTo->size)
	{
	    char	*ibuf;
	    ibuf = (char *) xrealloc (ociTo->buffer, needed);
	    if (!ibuf)
		return FALSE;
	    ociTo->size = needed;
	    ociTo->buffer = ibuf;
	}
	ociTo->bufptr = ociTo->buffer;
	ociTo->bufcnt = gotnowTo;
    }
    if (remain)
    {
	bcopy (ociFrom->bufptr + preserve, ociTo->buffer + ociTo->bufcnt, remain);
	ociTo->bufcnt += remain;
	ociFrom->bufcnt -= remain;
    }
    CheckPendingClientInput (to);
    if (AvailableInput == ocFrom)
	AvailableInput = ocTo;
    return TRUE;
}

SwitchClientOutput (from, to)
    ClientPtr	from, to;
{
    OsCommPtr ocFrom = (OsCommPtr) from->osPrivate;
    OsCommPtr ocTo = (OsCommPtr) to->osPrivate;
    
    ConnectionOutputTranslation[ocTo->fd] = to->index;
    if (PendingClientOutput (to))
    {
	NewOutputPending = TRUE;
	BITSET(OutputPending, ocTo->fd);
    }
}

int
PendingClientOutput (client)
    ClientPtr	client;
{
    OsCommPtr oc = (OsCommPtr) client->osPrivate;
    ConnectionOutputPtr	oco = oc->output;
    
    return (oco && oco->count != 0) || oc->ofirst;
}

int
CheckPendingClientInput (client)
    ClientPtr	client;
{
    OsCommPtr oc = (OsCommPtr)client->osPrivate;
    register ConnectionInputPtr oci = oc->input;
    xReq    *request;
    int	    gotnow;
    int	    needed;
    Bool    part;
    
    if (!oci)
	return 0;
    needed = oci->lenLastReq;
    gotnow = oci->bufcnt + oci->buffer - oci->bufptr;
    request = (xReq *) (oci->bufptr + needed);
    if (gotnow >= needed + RequestLength(request, client, gotnow - needed, &part) && !part)
    {
	BITSET(ClientsWithInput, oc->fd);
	return 1;
    }
    return 0;
}

void
MarkConnectionWriteBlocked (client)
    ClientPtr	client;
{
    OsCommPtr oc = (OsCommPtr)client->osPrivate;

    BITSET(ClientsWriteBlocked, oc->fd);
    AnyClientsWriteBlocked = TRUE;
}

int
BytesInClientBuffer (client)
    ClientPtr	client;
{
    OsCommPtr oc = (OsCommPtr)client->osPrivate;
    register ConnectionInputPtr oci = oc->input;

    if (!oci)
	return 0;
    return oci->bufcnt + oci->buffer - (oci->bufptr + oci->lenLastReq);
}

SkipInClientBuffer (client, nbytes, lenLastReq)
    ClientPtr	client;
    int		nbytes;
    int		lenLastReq;
{
    OsCommPtr oc = (OsCommPtr)client->osPrivate;
    register ConnectionInputPtr oci = oc->input;

    if (!oci)
	return;
    oci->bufptr += nbytes;
    oci->lenLastReq = lenLastReq;
}

/*****************************************************************
 * InsertFakeRequest
 *    Splice a consed up (possibly partial) request in as the next request.
 *
 **********************/

Bool
InsertFakeRequest(client, data, count)
    ClientPtr client;
    char *data;
    int count;
{
    OsCommPtr oc = (OsCommPtr)client->osPrivate;
    register ConnectionInputPtr oci = oc->input;
    int fd = oc->fd;
    register xReq *request;
    register int gotnow, moveup;
    Bool part;

    if (AvailableInput)
    {
	if (AvailableInput != oc)
	{
	    register ConnectionInputPtr aci = AvailableInput->input;
	    if (aci->size > BUFWATERMARK)
	    {
		xfree(aci->buffer);
		xfree(aci);
	    }
	    else
	    {
		aci->next = FreeInputs;
		FreeInputs = aci;
	    }
	    AvailableInput->input = (ConnectionInputPtr)NULL;
	}
	AvailableInput = (OsCommPtr)NULL;
    }
    if (!oci)
    {
	if (oci = FreeInputs)
	    FreeInputs = oci->next;
	else if (!(oci = AllocateInputBuffer()))
	    return FALSE;
	oc->input = oci;
    }
    oci->bufptr += oci->lenLastReq;
    oci->lenLastReq = 0;
    gotnow = oci->bufcnt + oci->buffer - oci->bufptr;
    if ((gotnow + count) > oci->size)
    {
	char *ibuf;

	ibuf = (char *)xrealloc(oci->buffer, gotnow + count);
	if (!ibuf)
	    return(FALSE);
	oci->size = gotnow + count;
	oci->buffer = ibuf;
	oci->bufptr = ibuf + oci->bufcnt - gotnow;
    }
    moveup = count - (oci->bufptr - oci->buffer);
    if (moveup > 0)
    {
	if (gotnow > 0)
	    memmove(oci->bufptr + moveup, oci->bufptr, gotnow);
	oci->bufptr += moveup;
	oci->bufcnt += moveup;
    }
    memmove(oci->bufptr - count, data, count);
    oci->bufptr -= count;
    request = (xReq *)oci->bufptr;
    gotnow += count;
    if (gotnow >= RequestLength (request, client, gotnow, &part) && !part)
	BITSET(ClientsWithInput, fd);
    else
	YieldControlNoInput();
    return(TRUE);
}

/*****************************************************************
 * AppendFakeRequest
 *    Append a (possibly partial) request in as the last request.
 *
 **********************/
 
Bool
AppendFakeRequest (client, data, count)
    ClientPtr client;
    char *data;
    int count;
{
    OsCommPtr oc = (OsCommPtr)client->osPrivate;
    register ConnectionInputPtr oco = oc->input;
    int fd = oc->fd;
    register int gotnow;
    Bool part;

#ifdef NOTDEF
    /* can't do this as the data may actually be coming from this buffer! */
    if (AvailableInput)
    {
	if (AvailableInput != oc)
	{
	    register ConnectionInputPtr aci = AvailableInput->input;
	    if (aci->size > BUFWATERMARK)
	    {
		xfree(aci->buffer);
		xfree(aci);
	    }
	    else
	    {
		aci->next = FreeInputs;
		FreeInputs = aci;
	    }
	    AvailableInput->input = (ConnectionInputPtr)NULL;
	}
	AvailableInput = (OsCommPtr)NULL;
    }
#endif
    if (!oco)
    {
	if (oco = FreeInputs)
	    FreeInputs = oco->next;
	else if (!(oco = AllocateInputBuffer()))
	    return FALSE;
	oc->input = oco;
    }
    oco->bufptr += oco->lenLastReq;
    oco->lenLastReq = 0;
    gotnow = oco->bufcnt + oco->buffer - oco->bufptr;
    if ((gotnow + count) > oco->size)
    {
	char *ibuf;

	ibuf = (char *)xrealloc(oco->buffer, gotnow + count);
	if (!ibuf)
	    return(FALSE);
	oco->size = gotnow + count;
	oco->buffer = ibuf;
	oco->bufptr = ibuf;
    }
    bcopy(data, oco->bufptr + gotnow, count);
    oco->bufcnt += count;
    gotnow += count;
    if (gotnow >= RequestLength (oco->bufptr, client, gotnow, &part) && !part)
	BITSET(ClientsWithInput, fd);
    else
	YieldControlNoInput();
    return(TRUE);
}

/*****************************************************************
 * ResetRequestFromClient
 *    Reset to reexecute the current request, and yield.
 *
 **********************/

ResetCurrentRequest(client)
    ClientPtr client;
{
    OsCommPtr oc = (OsCommPtr)client->osPrivate;
    register ConnectionInputPtr oci = oc->input;
    int fd = oc->fd;
    register xReq *request;
    int gotnow, needed;
#ifdef LBX
    Bool part;
#endif

    if (AvailableInput == oc)
	AvailableInput = (OsCommPtr)NULL;
    oci->lenLastReq = 0;
    gotnow = oci->bufcnt + oci->buffer - oci->bufptr;
#ifdef LBX
    request = (xReq *)oci->bufptr;
    if (gotnow >= RequestLength (request, client, gotnow, &part) && !part)
    {
	BITSET(ClientsWithInput, fd);
	YieldControl();
    }
    else
	YieldControlNoInput();
#else
    if (gotnow < sizeof(xReq))
    {
	YieldControlNoInput();
    }
    else
    {
	request = (xReq *)oci->bufptr;
	needed = get_req_len(request, client);
#ifdef BIGREQS
	if (!needed && client->big_requests)
	{
	    oci->bufptr -= sizeof(xBigReq) - sizeof(xReq);
	    *(xReq *)oci->bufptr = *request;
	    ((xBigReq *)oci->bufptr)->length = client->req_len;
	    if (client->swapped)
	    {
		char n;
		swapl(&((xBigReq *)oci->bufptr)->length, n);
	    }
	}
#endif
	if (gotnow >= (needed << 2))
	{
	    BITSET(ClientsWithInput, fd);
	    YieldControl();
	}
	else
	    YieldControlNoInput();
    }
#endif	/* !LBX */
}

    /* lookup table for adding padding bytes to data that is read from
    	or written to the X socket.  */
static int padlength[4] = {0, 3, 2, 1};

 /********************
 * FlushClient()
 *    If the client isn't keeping up with us, then we try to continue
 *    buffering the data and set the apropriate bit in ClientsWritable
 *    (which is used by WaitFor in the select).  If the connection yields
 *    a permanent error, or we can't allocate any more space, we then
 *    close the connection.
 *
 **********************/

int
StandardFlushClient(who, oc, extraBuf, extraCount)
    ClientPtr who;
    OsCommPtr oc;
    char *extraBuf;
    int extraCount; /* do not modify... returned below */
{
    register ConnectionOutputPtr oco = oc->output;
    int connection = oc->fd;
    struct iovec iov[3];
    static char padBuffer[3];
    long written;
    long padsize;
    long notWritten;
    long todo;

    if (!oco)
	return 0;
    written = 0;
    padsize = padlength[extraCount & 3];
    notWritten = oco->count + extraCount + padsize;
    todo = notWritten;
    while (notWritten) {
	long before = written;	/* amount of whole thing written */
	long remain = todo;	/* amount to try this time, <= notWritten */
	int i = 0;
	long len;

	/* You could be very general here and have "in" and "out" iovecs
	 * and write a loop without using a macro, but what the heck.  This
	 * translates to:
	 *
	 *     how much of this piece is new?
	 *     if more new then we are trying this time, clamp
	 *     if nothing new
	 *         then bump down amount already written, for next piece
	 *         else put new stuff in iovec, will need all of next piece
	 *
	 * Note that todo had better be at least 1 or else we'll end up
	 * writing 0 iovecs.
	 */
#define InsertIOV(pointer, length) \
	len = (length) - before; \
	if (len > remain) \
	    len = remain; \
	if (len <= 0) { \
	    before = (-len); \
	} else { \
	    iov[i].iov_len = len; \
	    iov[i].iov_base = (pointer) + before; \
	    i++; \
	    remain -= len; \
	    before = 0; \
	}

	InsertIOV ((char *)oco->buf, oco->count)
	InsertIOV (extraBuf, extraCount)
	InsertIOV (padBuffer, padsize)

	errno = 0;
	WritingClient = who;
	if ((len = (*oc->Writev) (connection, iov, i)) >= 0)
	{
	    written += len;
	    notWritten -= len;
	    todo = notWritten;
	}
	else if (ETEST(errno)
#ifdef SUNSYSV /* check for another brain-damaged OS bug */
		 || (errno == 0)
#endif
#ifdef EMSGSIZE /* check for another brain-damaged OS bug */
		 || ((errno == EMSGSIZE) && (todo == 1))
#endif
		)
	{
	    /* If we've arrived here, then the client is stuffed to the gills
	       and not ready to accept more.  Make a note of it and buffer
	       the rest. */
	    BITSET(ClientsWriteBlocked, connection);
	    AnyClientsWriteBlocked = TRUE;

	    if (written < oco->count)
	    {
		if (written > 0)
		{
		    oco->count -= written;
		    memmove((char *)oco->buf,
			    (char *)oco->buf + written,
			  oco->count);
		    written = 0;
		}
	    }
	    else
	    {
		written -= oco->count;
		oco->count = 0;
	    }

	    if (notWritten > oco->size)
	    {
		unsigned char *obuf;

		obuf = (unsigned char *)xrealloc(oco->buf,
						 notWritten + BUFSIZE);
		if (!obuf)
		{
		    close(connection);
		    MarkClientException(who);
		    oco->count = 0;
		    return(-1);
		}
		oco->size = notWritten + BUFSIZE;
		oco->buf = obuf;
	    }

	    /* If the amount written extended into the padBuffer, then the
	       difference "extraCount - written" may be less than 0 */
	    if ((len = extraCount - written) > 0)
		memmove ((char *)oco->buf + oco->count,
			 extraBuf + written,
		       len);

	    oco->count = notWritten; /* this will include the pad */
	    /* return only the amount explicitly requested */
	    return extraCount;
	}
#ifdef EMSGSIZE /* check for another brain-damaged OS bug */
	else if (errno == EMSGSIZE)
	{
	    todo >>= 1;
	}
#endif
	else
	{
	    close(connection);
	    MarkClientException(who);
	    oco->count = 0;
	    return(-1);
	}
    }

    /* everything was flushed out */
    oco->count = 0;
    /* check to see if this client was write blocked */
    if (AnyClientsWriteBlocked)
    {
	BITCLEAR(ClientsWriteBlocked, oc->fd);
 	if (! ANYSET(ClientsWriteBlocked))
	    AnyClientsWriteBlocked = FALSE;
    }
    if (oco->size > BUFWATERMARK)
    {
	xfree(oco->buf);
	xfree(oco);
    }
    else
    {
	oco->next = FreeOutputs;
	FreeOutputs = oco;
    }
    oc->output = (ConnectionOutputPtr)NULL;
    return extraCount; /* return only the amount explicitly requested */
}

static int
ExpandOutputBuffer(oco, len)
    ConnectionOutputPtr oco;
    int len;
{
    unsigned char *obuf;

    obuf = (unsigned char *)xrealloc(oco->buf, len + BUFSIZE);
    if (!obuf)
    {
	oco->count = 0;
	return(-1);
    }
    oco->size = len + BUFSIZE;
    oco->buf = obuf;
    return 0;
}

int
LbxFlushClient(who, oc, extraBuf, extraCount)
    ClientPtr who;
    OsCommPtr oc;
    char *extraBuf;
    int extraCount; /* do not modify... returned below */
{
    ConnectionOutputPtr nextbuf;
    register ConnectionOutputPtr oco;
    int retval = extraCount;

    if (!oc->ofirst) {
	return StandardFlushClient(who, oc, extraBuf, extraCount);
    }

    if (oco = oc->output) {
	oc->olast->next = oco;
	oc->olast = oco;
    }

    oco = oc->ofirst;
    do {
	Bool nocomp = oco->nocompress;
	nextbuf = (oco != oc->olast) ? oco->next : NULL;
	oc->output = oco;
	if (nocomp)
	    (*oc->compressOff)(oc->fd);
	if (oc->olast == oco) {
	    StandardFlushClient(who, oc, extraBuf, extraCount);
	    extraCount = 0;
	}
	else
	    StandardFlushClient(who, oc, (char *)NULL, 0);
	if (nocomp)
	    (*oc->compressOn)(oc->fd);
	if (oc->output != (ConnectionOutputPtr) NULL) {
	    oc->output = (ConnectionOutputPtr) NULL;
	    break;
	}
    } while (oco = nextbuf);
    oc->ofirst = oco;

    /*
     * If we didn't get a chance to flush the extraBuf above, then
     * we need to buffer it here.
     */
    if (extraCount) {
	int newlen = oco->count + extraCount + padlength[extraCount & 3];
	oco = oc->olast;
	if (ExpandOutputBuffer(oco, newlen) < 0) {
	    close (oc->fd);
	    MarkClientException(who);
	    return(-1);
	}
	bcopy(extraBuf, (char *)oco->buf + oco->count, extraCount);
	oco->count = newlen;
    }

    return retval;
}

 /********************
 * FlushAllOutput()
 *    Flush all clients with output.  However, if some client still
 *    has input in the queue (more requests), then don't flush.  This
 *    will prevent the output queue from being flushed every time around
 *    the round robin queue.  Now, some say that it SHOULD be flushed
 *    every time around, but...
 *
 **********************/

void
FlushAllOutput()
{
    register int index, base, mask;
    OsCommPtr oc;
    register ClientPtr client;

    if (! NewOutputPending)
	return;

    /*
     * It may be that some client still has critical output pending,
     * but he is not yet ready to receive it anyway, so we will
     * simply wait for the select to tell us when he's ready to receive.
     */
    CriticalOutputPending = FALSE;
    NewOutputPending = FALSE;

    for (base = 0; base < mskcnt; base++)
    {
	mask = OutputPending[ base ];
	OutputPending[ base ] = 0;
	while (mask)
	{
	    index = ffs(mask) - 1;
	    mask &= ~lowbit(mask);
	    if ((index = ConnectionOutputTranslation[(base << 5) + index]) == 0)
		continue;
	    client = clients[index];
	    if (client->clientGone)
		continue;
	    oc = (OsCommPtr)client->osPrivate;
	    if (GETBIT(ClientsWithInput, oc->fd))
	    {
		BITSET(OutputPending, oc->fd); /* set the bit again */
		NewOutputPending = TRUE;
	    }
	    else
		(void)FlushClient(client, oc, (char *)NULL, 0);
	}
    }

}

void
FlushIfCriticalOutputPending()
{
    if (CriticalOutputPending)
	FlushAllOutput();
}

void
SetCriticalOutputPending()
{
    CriticalOutputPending = TRUE;
}

/*****************
 * WriteToClient
 *    Copies buf into ClientPtr.buf if it fits (with padding), else
 *    flushes ClientPtr.buf and buf to client.  As of this writing,
 *    every use of WriteToClient is cast to void, and the result
 *    is ignored.  Potentially, this could be used by requests
 *    that are sending several chunks of data and want to break
 *    out of a loop on error.  Thus, we will leave the type of
 *    this routine as int.
 *****************/

int
StandardWriteToClient (who, count, buf)
    ClientPtr who;
    char *buf;
    int count;
{
    OsCommPtr oc = (OsCommPtr)who->osPrivate;
    register ConnectionOutputPtr oco = oc->output;
    int padBytes;

    if (!count)
	return(0);

    if (!oco)
    {
	if (oco = FreeOutputs)
	{
	    FreeOutputs = oco->next;
	}
	else if (!(oco = AllocateOutputBuffer()))
	{
	    close(oc->fd);
	    MarkClientException(who);
	    return -1;
	}
	oc->output = oco;
    }

    padBytes =  padlength[count & 3];

    if (oco->count + count + padBytes > oco->size)
    {
	BITCLEAR(OutputPending, oc->fd);
	CriticalOutputPending = FALSE;
	NewOutputPending = FALSE;
	return FlushClient(who, oc, buf, count);
    }

    NewOutputPending = TRUE;
    BITSET(OutputPending, oc->fd);
    memmove((char *)oco->buf + oco->count, buf, count);
    oco->count += count + padBytes;
    
    return(count);
}

int
UncompressWriteToClient (who, count, buf)
    ClientPtr who;
    char *buf;
    int count;
{
    OsCommPtr oc = (OsCommPtr)who->osPrivate;
    register ConnectionOutputPtr oco;
    int paddedLen = count + padlength[count & 3];

    if (!count)
	return(0);

    if (oco = oc->output) {
	/*
	 * we're currently filling a buffer, and it must be compressible,
	 * so put it on the queue
	 */
	if (oc->ofirst) {
	    oc->olast->next = oco;
	    oc->olast = oco;
	}
	else {
	    oc->ofirst = oc->olast = oco;
	}
	oco = oc->output = (ConnectionOutputPtr)NULL;
    }
    else if (oc->ofirst) {
	oco = oc->olast;
	if (!oco->nocompress || ((oco->count + paddedLen) > oco->size))
	    oco = (ConnectionOutputPtr)NULL;
    }

    if (!oco) {
	if (!(oco = AllocateUncompBuffer(paddedLen))) {
	    close(oc->fd);
	    MarkClientException(who);
	    return -1;
	}
    }
    bcopy(buf, (char *)oco->buf + oco->count, count);
    oco->count += paddedLen;

    if (oc->ofirst) {
	oc->olast->next = oco;
	oc->olast = oco;
    }
    else {
	oc->ofirst = oc->olast = oco;
    }

    NewOutputPending = TRUE;
    BITSET(OutputPending, oc->fd);
    return(count);
}

static ConnectionInputPtr
AllocateInputBuffer()
{
    register ConnectionInputPtr oci;

    oci = (ConnectionInputPtr)xalloc(sizeof(ConnectionInput));
    if (!oci)
	return (ConnectionInputPtr)NULL;
    oci->buffer = (char *)xalloc(BUFSIZE);
    if (!oci->buffer)
    {
	xfree(oci);
	return (ConnectionInputPtr)NULL;
    }
    oci->size = BUFSIZE;
    oci->bufptr = oci->buffer;
    oci->bufcnt = 0;
    oci->lenLastReq = 0;
    return oci;
}

static ConnectionOutputPtr
AllocateOutputBuffer()
{
    register ConnectionOutputPtr oco;

    oco = (ConnectionOutputPtr)xalloc(sizeof(ConnectionOutput));
    if (!oco)
	return (ConnectionOutputPtr)NULL;
    oco->buf = (unsigned char *) xalloc(BUFSIZE);
    if (!oco->buf)
    {
	xfree(oco);
	return (ConnectionOutputPtr)NULL;
    }
    oco->size = BUFSIZE;
    oco->count = 0;
    oco->nocompress = FALSE;
    return oco;
}

static ConnectionOutputPtr
AllocateUncompBuffer(count)
    int count;
{
    register ConnectionOutputPtr oco;
    int len = (count > BUFSIZE) ? count : BUFSIZE;

    oco = (ConnectionOutputPtr)xalloc(sizeof(ConnectionOutput));
    if (!oco)
	return (ConnectionOutputPtr)NULL;
    oco->buf = (unsigned char *) xalloc(len);
    if (!oco->buf)
    {
	xfree(oco);
	return (ConnectionOutputPtr)NULL;
    }
    oco->size = len;
    oco->count = 0;
    oco->nocompress = TRUE;
    return oco;
}

void
FreeOsBuffers(oc)
    OsCommPtr oc;
{
    register ConnectionInputPtr oci;
    register ConnectionOutputPtr oco;

    if (AvailableInput == oc)
	AvailableInput = (OsCommPtr)NULL;
    if (oci = oc->input)
    {
	if (FreeInputs)
	{
	    xfree(oci->buffer);
	    xfree(oci);
	}
	else
	{
	    FreeInputs = oci;
	    oci->next = (ConnectionInputPtr)NULL;
	    oci->bufptr = oci->buffer;
	    oci->bufcnt = 0;
	    oci->lenLastReq = 0;
	}
    }
    if (oco = oc->output)
    {
	if (FreeOutputs)
	{
	    xfree(oco->buf);
	    xfree(oco);
	}
	else
	{
	    FreeOutputs = oco;
	    oco->next = (ConnectionOutputPtr)NULL;
	    oco->count = 0;
	}
    }
    if (oco = oc->ofirst) {
	ConnectionOutputPtr nextoco;
	do {
	    nextoco = oco->next;
	    xfree(oco->buf);
	    xfree(oco);
	    if (oco == oc->olast)
		break;
	    oco = nextoco;
	} while (1);
    }
}

void
ResetOsBuffers()
{
    register ConnectionInputPtr oci;
    register ConnectionOutputPtr oco;

    while (oci = FreeInputs)
    {
	FreeInputs = oci->next;
	xfree(oci->buffer);
	xfree(oci);
    }
    while (oco = FreeOutputs)
    {
	FreeOutputs = oco->next;
	xfree(oco->buf);
	xfree(oco);
    }
}
