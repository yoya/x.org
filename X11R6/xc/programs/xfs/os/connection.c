/* $XConsortium: connection.c,v 1.29 94/04/17 19:56:05 mor Exp $ */
/*
 * handles connections
 */
/*
 
Copyright (c) 1990, 1991  X Consortium

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

 * Copyright 1990, 1991 Network Computing Devices;
 * Portions Copyright 1987 by Digital Equipment Corporation 
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
 */
/*
 * Copyright 1990, 1991 Network Computing Devices;
 * Portions Copyright 1987 by Digital Equipment Corporation 
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
 */

#include	<X11/Xtrans.h>
#include	"misc.h"
#include	<stdio.h>
#include	<errno.h>
#include	<sys/param.h>
#include	<sys/socket.h>
#include	<sys/uio.h>
#include	<signal.h>

#include	"FS.h"
#include	"FSproto.h"
#include	"clientstr.h"
#include	"osdep.h"
#include	"globals.h"
#include	"osstruct.h"
#include	"servermd.h"

#ifdef X_NOT_STDC_ENV
extern int errno;
#endif


int         ListenPort = DEFAULT_FS_PORT;   /* port to listen on */
int         lastfdesc;

long        WellKnownConnections;
long        AllSockets[mskcnt];
long        AllClients[mskcnt];
long        LastSelectMask[mskcnt];
long        ClientsWithInput[mskcnt];
long        ClientsWriteBlocked[mskcnt];
long        OutputPending[mskcnt];
extern long MaxClients;
long        OutputBufferSize = BUFSIZE;

Bool        NewOutputPending;
Bool        AnyClientsWriteBlocked;

int         ConnectionTranslation[MAXSOCKS];

XtransConnInfo 	*ListenTransConns = NULL;
int	       	*ListenTransFds = NULL;
int		ListenTransCount;

extern ClientPtr NextAvailableClient();

#ifdef SIGNALRETURNSINT
#define SIGVAL int
#else
#define SIGVAL void
#endif

extern SIGVAL AutoResetServer();
extern SIGVAL GiveUp();
extern SIGVAL ServerReconfig();
extern SIGVAL ServerCacheFlush();
extern SIGVAL CleanupChild();

extern void FreeOsBuffers();

static void error_conn_max();
static void close_fd();


static XtransConnInfo
lookup_trans_conn (fd)

int fd;

{
    if (ListenTransFds)
    {
	int i;
	for (i = 0; i < ListenTransCount; i++)
	    if (ListenTransFds[i] == fd)
		return ListenTransConns[i];
    }

    return (NULL);
}

StopListening()
{
    int i;

    for (i = 0; i < ListenTransCount; i++)
    {
	BITCLEAR (AllSockets, ListenTransFds[i]);
	_FontTransCloseForCloning (ListenTransConns[i]);
    }

    free ((char *) ListenTransFds);
    free ((char *) ListenTransConns);

    ListenTransFds = NULL;
    ListenTransConns = NULL;
}

/*
 * creates the sockets for listening to clients
 *
 * only called when server first started
 */
void
CreateSockets(old_listen_count, old_listen)

int 	     old_listen_count;
OldListenRec *old_listen;

{
    int         request,
                i;

    CLEARBITS(AllSockets);
    CLEARBITS(AllClients);
    CLEARBITS(LastSelectMask);
    CLEARBITS(ClientsWithInput);

    for (i = 0; i < MAXSOCKS; i++)
	ConnectionTranslation[i] = 0;

#if defined(hpux) || defined(SVR4)
    lastfdesc = _NFILE - 1;
#else
    lastfdesc = getdtablesize() - 1;
#endif				/* hpux */

    if (lastfdesc > MAXSOCKS) {
	lastfdesc = MAXSOCKS;
    }
    WellKnownConnections = 0;

    if (old_listen_count > 0) {

	/*
	 * The font server cloned itself.  Re-use previously opened
	 * transports for listening.
	 */

	ListenTransConns = (XtransConnInfo *) malloc (
	    old_listen_count * sizeof (XtransConnInfo));

	ListenTransFds = (int *) malloc (old_listen_count * sizeof (int));

	ListenTransCount = 0;

	for (i = 0; i < old_listen_count; i++)
	{
	    char portnum[10];

	    if (old_listen[i].portnum != ListenPort)
		continue;		/* this should never happen */
	    else
		sprintf (portnum, "%d", old_listen[i].portnum);

	    if ((ListenTransConns[ListenTransCount] =
		_FontTransReopenCOTSServer (old_listen[i].trans_id,
		old_listen[i].fd, portnum)) != NULL)
	    {
		ListenTransFds[ListenTransCount] = old_listen[i].fd;
		WellKnownConnections |= (1L << old_listen[i].fd);

		NoticeF("Reusing existing file descriptor %d\n",
		    old_listen[i].fd);

		ListenTransCount++;
	    }
	}
    } else {
	char port[20];
	int partial;

	sprintf (port, "%d", ListenPort);

	if ((_FontTransMakeAllCOTSServerListeners (port, &partial,
	    &ListenTransCount, &ListenTransConns) >= 0) &&
	    (ListenTransCount >= 1))
	{
	    ListenTransFds = (int *) malloc (ListenTransCount * sizeof (int));

	    for (i = 0; i < ListenTransCount; i++)
	    {
		int fd = _FontTransGetConnectionNumber (ListenTransConns[i]);

		ListenTransFds[i] = fd;
		WellKnownConnections |= (1L << fd);
	    }
	}
    }

    if (WellKnownConnections == 0)
	FatalError("Cannot establish any listening sockets\n");


    /* set up all the signal handlers */
    signal(SIGPIPE, SIG_IGN);
    signal(SIGHUP, AutoResetServer);
    signal(SIGINT, GiveUp);
    signal(SIGTERM, GiveUp);
    signal(SIGUSR1, ServerReconfig);
    signal(SIGUSR2, ServerCacheFlush);
    signal(SIGCHLD, CleanupChild);

    AllSockets[0] = WellKnownConnections;
}

/*
 * called when server cycles
 */
ResetSockets()
{
}

/*
 * accepts new connections
 */
void
MakeNewConnections()
{
    long        readyconnections;
    int         curconn;
    int         newconn;
    long        connect_time;
    int         i;
    ClientPtr   client;
    OsCommPtr   oc;

    readyconnections = (LastSelectMask[0] & WellKnownConnections);
    if (!readyconnections)
	return;
    connect_time = GetTimeInMillis();

    /* kill off stragglers */
    for (i = MINCLIENT; i < currentMaxClients; i++) {
	if ((client = clients[i]) != NullClient) {
	    oc = (OsCommPtr) client->osPrivate;
	    if (oc && (oc->conn_time != 0) &&
		    (connect_time - oc->conn_time) >= TimeOutValue)
		CloseDownClient(client);
	}
    }

    while (readyconnections) {
	XtransConnInfo trans_conn, new_trans_conn;
	int status;

	curconn = ffs(readyconnections) - 1;
	readyconnections &= ~(1 << curconn);

	if ((trans_conn = lookup_trans_conn (curconn)) == NULL)
	    continue;

	if ((new_trans_conn = _FontTransAccept (trans_conn, &status)) == NULL)
	    continue;

	newconn = _FontTransGetConnectionNumber (new_trans_conn);

	_FontTransSetOption(new_trans_conn, TRANS_NONBLOCKING, 1);

	oc = (OsCommPtr) fsalloc(sizeof(OsCommRec));
	if (!oc) {
	    fsfree(oc);
	    error_conn_max(new_trans_conn);
	    _FontTransClose(new_trans_conn);
	    continue;
	}
	BITSET(AllClients, newconn);
	BITSET(AllSockets, newconn);
	oc->fd = newconn;
	oc->trans_conn = new_trans_conn;
	oc->input = (ConnectionInputPtr) NULL;
	oc->output = (ConnectionOutputPtr) NULL;
	oc->conn_time = connect_time;

	if ((newconn < lastfdesc) &&
		(client = NextAvailableClient((pointer) oc))) {
	    ConnectionTranslation[newconn] = client->index;
	} else {
	    error_conn_max(new_trans_conn);
	    close_fd(oc);
	}
    }
}

#define	NOROOM	"Maximum number of clients reached"

static void
error_conn_max(trans_conn)

XtransConnInfo trans_conn;

{
    int fd = _FontTransGetConnectionNumber (trans_conn);
    fsConnSetup conn;
    char        pad[3];
    char        byteOrder = 0;
    int         whichbyte = 1;
    struct timeval waittime;
    long        mask[mskcnt];


    waittime.tv_usec = BOTIMEOUT / MILLI_PER_SECOND;
    waittime.tv_usec = (BOTIMEOUT % MILLI_PER_SECOND) *
	(1000000 / MILLI_PER_SECOND);
    CLEARBITS(mask);
    BITSET(mask, fd);
    (void) select(fd + 1, (int *) mask, (int *) NULL, (int *) NULL, &waittime);
    /* try to read the byteorder of the connection */
    (void) _FontTransRead(trans_conn, &byteOrder, 1);
    if ((byteOrder == 'l') || (byteOrder == 'B')) {
	int         num_alts;
	AlternateServerPtr altservers,
	            as;
	int         i,
	            altlen = 0;

	num_alts = ListAlternateServers(&altservers);
	conn.status = AuthDenied;
	conn.major_version = FS_PROTOCOL;
	conn.minor_version = FS_PROTOCOL_MINOR;
	conn.num_alternates = num_alts;
	for (i = 0, as = altservers; i < num_alts; i++, as++) {
	    altlen += (2 + as->namelen + 3) >> 2;
	}
	conn.alternate_len = altlen;
	/* blow off the auth info */
	conn.auth_index = 0;
	conn.auth_len = 0;

	if (((*(char *) &whichbyte) && (byteOrder == 'B')) ||
		(!(*(char *) &whichbyte) && (byteOrder == 'l'))) {
	    conn.status = lswaps(conn.status);
	    conn.major_version = lswaps(conn.major_version);
	    conn.minor_version = lswaps(conn.minor_version);
	    conn.alternate_len = lswaps(conn.alternate_len);
	}
	(void) _FontTransWrite(trans_conn,
	    (char *) &conn, SIZEOF(fsConnSetup));
	/* dump alternates */
	for (i = 0, as = altservers; i < num_alts; i++, as++) {
	    (void) _FontTransWrite(trans_conn,
		(char *) as, 2);  /* XXX */
	    (void) _FontTransWrite(trans_conn,
		(char *) as->name, as->namelen);
	    altlen = 2 + as->namelen;
	    /* pad it */
	    if (altlen & 3)
		(void) _FontTransWrite(trans_conn,
		(char *) pad, ((4 - (altlen & 3)) & 3));
	}
	if (num_alts)
	    fsfree((char *) altservers);
    }
}

static void
close_fd(oc)
    OsCommPtr   oc;
{
    int         fd = oc->fd;

    if (oc->trans_conn)
	_FontTransClose(oc->trans_conn);
    FreeOsBuffers(oc);
    BITCLEAR(AllSockets, fd);
    BITCLEAR(AllClients, fd);
    BITCLEAR(ClientsWithInput, fd);
    BITCLEAR(ClientsWriteBlocked, fd);
    if (!ANYSET(ClientsWriteBlocked))
	AnyClientsWriteBlocked = FALSE;
    BITCLEAR(OutputPending, fd);
    fsfree(oc);
}

CheckConnections()
{
    long        mask[mskcnt];
    long        tmask[mskcnt];
    int         curclient;
    int         i;
    struct timeval notime;
    int         r;

    notime.tv_sec = 0;
    notime.tv_usec = 0;

    COPYBITS(AllClients, mask);
    for (i = 0; i < mskcnt; i++) {
	while (mask[i]) {
	    curclient = ffs(mask[i]) - 1 + (i << 5);
	    CLEARBITS(tmask);
	    BITSET(tmask, curclient);
	    r = select(curclient + 1, (int *) tmask, (int *) NULL,
		       (int *) NULL, &notime);
	    if (r < 0)
		CloseDownClient(clients[ConnectionTranslation[curclient]]);
	    BITCLEAR(mask, curclient);
	}
    }
}

CloseDownConnection(client)
    ClientPtr   client;
{
    OsCommPtr   oc = (OsCommPtr) client->osPrivate;

    if (oc->output && oc->output->count)
	FlushClient(client, oc, (char *) NULL, 0, 0);
    ConnectionTranslation[oc->fd] = 0;
    close_fd(oc);
    client->osPrivate = (pointer) NULL;
}


/****************
 * IgnoreClient
 *    Removes one client from input masks.
 *    Must have cooresponding call to AttendClient.
 ****************/

static long IgnoredClientsWithInput[mskcnt];

IgnoreClient(client)
    ClientPtr   client;
{
    OsCommPtr   oc = (OsCommPtr) client->osPrivate;
    int         connection = oc->fd;

    if (GETBIT(ClientsWithInput, connection))
	BITSET(IgnoredClientsWithInput, connection);
    else
	BITCLEAR(IgnoredClientsWithInput, connection);
    BITCLEAR(ClientsWithInput, connection);
    BITCLEAR(AllSockets, connection);
    BITCLEAR(AllClients, connection);
    BITCLEAR(LastSelectMask, connection);
    isItTimeToYield = TRUE;
}

/****************
 * AttendClient
 *    Adds one client back into the input masks.
 ****************/

AttendClient(client)
    ClientPtr   client;
{
    OsCommPtr   oc = (OsCommPtr) client->osPrivate;
    int         connection = oc->fd;

    BITSET(AllClients, connection);
    BITSET(AllSockets, connection);
    BITSET(LastSelectMask, connection);
    if (GETBIT(IgnoredClientsWithInput, connection))
	BITSET(ClientsWithInput, connection);
}

/*
 * figure out which clients need to be toasted
 */
ReapAnyOldClients()
{
    int         i;
    long        cur_time = GetTimeInMillis();
    ClientPtr   client;
    extern void SendKeepAliveEvent();

#ifdef DEBUG
    fprintf(stderr, "Looking for clients to reap\n");
#endif

    for (i = MINCLIENT; i < currentMaxClients; i++) {
	client = clients[i];
	if (client) {
	    if ((cur_time - client->last_request_time) >= ReapClientTime) {
		if (client->clientGone == CLIENT_AGED) {
		    client->clientGone = CLIENT_TIMED_OUT;

#ifdef DEBUG
		    fprintf(stderr, "reaping client #%d\n", i);
#endif

		    CloseDownClient(client);
		} else {
		    client->clientGone = CLIENT_AGED;
		    SendKeepAliveEvent(client);
		}
	    }
	}
    }
}
