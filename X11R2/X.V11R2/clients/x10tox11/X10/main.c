/*
 * Copyright 1987 by Tektronix Inc, Beaverton, Oregon and
 * Massachusetts Institute of Technology
 * 
 *
 *                         All Rights Reserved
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted,
 * provided that the above copyright notice appear in all copies and that
 * both that copyright notice and this permission notice appear in
 * supporting documentation, and that the name of Tektronix and 
 * Massachusetts Institute of Technology not be used in advertising
 * or publicity pertaining to distribution of the software without
 * specific, written prior permission.
 *
 *
 * TEKTRONIX DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
 * ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
 * TEKTRONIX BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
 * ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
 * ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 */
/* 
 * $Header: main.c,v 4.3 88/02/15 10:04:39 jim Exp $ 
 * $Locker:  $ 
 */
#include <copyright.h>

/* Initialization and socket routines */

#undef NULL
#include <errno.h>
#include "Xint.h"
#include <sys/socket.h>

#include <X11/Xos.h>

#include <signal.h>
#include <netinet/in.h>
#ifdef UNIXCONN
#include <sys/un.h>
#endif
#include <setjmp.h>
#ifdef DNETCONN
#include <netdnet/dn.h>
#endif


int HangUp();
char *Xalloc();
extern u_char	Xincomplete;
extern int errno;
extern int sys_nerr;
extern char *sys_errlist[];

static char *devdisp = "unix:0";/* The display device */
static char *display = "0";	/* The display number */

static int socketOn[maxsocks];	/* 1: active, 0: inactive */
int requestId[maxsocks];	/* Request count associated with the socket */
static char *sockbuf[maxsocks];	/* Request buffers */
char *bufptr[maxsocks];		/* Buffer pointers */
				/* Pointer to start of data in sockbuf */
int bufcnt[maxsocks];		/* Buffer counts */
				/* Count of data bytes available in sockbuf */
#define DEFBUFSIZE 4096		/* Default buffer size */
#define MAXBUFSIZE (1<<17)	/* Maximum buffer size */
static int bufsize[maxsocks];	/* Buffer sizes */
				/* Current size in bytes of sockbuf buffer */
#ifdef DUALTCP
int swapped[maxsocks];		/* 1: must byte swap on connection */
#endif
static int lastfdesc;		/* Maximum file descriptor */
static int maxsock;		/* Highest socket in use + 1 */
static int firstsock;		/* First possible socket */
static int firstmask;		/* First possible socket mask */
#ifdef DUALTCP
static int swapmask;		/* Swap ports */
#endif
static int requestmask;		/* Listener mask */
static int selmask[mskcnt];	/* Mask of open connections */
static int servermask[mskcnt];	/* Mask of open connections to X11 server */
int havemask[mskcnt];		/* Connections with buffered requests */
int havestate;			/* <0: many, 0: none, >0: single */
static int servergrabber;	/* Grabbing client */
static int grabselmask[mskcnt];	/* Saved mask during GrabServer */
static int grabhavemask[mskcnt];/* Saved mask during GrabServer */
static int grabhavestate;	/* Saved state during GrabServer */
int rr_counter;			/* Round robin counter */
struct timeval waittime = {300, 0};
#define OUTTIME 10		/* 10 seconds */
static struct timeval outtime = {OUTTIME, 0};
static struct timeval notime = {0, 0};
static unsigned errdelay = 15;
static jmp_buf	environ;

X10main(argc, argv)
	int argc;
	char **argv;
{
	char	*p, *getenv();
	int	i;

	if (p = getenv("DISPLAY")) {
		devdisp = p;
		p = index(p, ':');
		if(p == NULL)
			Error("Invalid DISPLAY environment variable=\"%s\"\n",
				devdisp);
		display = p + 1;
	}

	for(i = 1; i < argc; i++)
	{
		if (strcmp (argv[i], "-display") == 0 || 
		    strcmp (argv[i], "-d") == 0) {
		    p = argv[i];
		} else if((p = (char *)index(argv[i], ':')) != NULL) {
			display = p+1;
			devdisp = argv[i];
		}
	}
	Initialize ();

	/*
	 * We return here on X11 IO errors.
	 */
	setjmp(environ);

	Dispatcher ();
}

RestartDispatcher()
{
	longjmp(environ, 0);
	/*NOTREACHED*/
}

/* Called from Dispatcher when there are no complete requests to process, or
 * when multiple sockets have requests, or when there is input to be read.
 */

Receive ()
{
	int mask[mskcnt];
	register int i, serverfd, maskval, rrmask;
	int mi, numread;
	union {
	    struct sockaddr sa;
#ifdef UNIXCONN
	    struct sockaddr_un un;
#endif
#ifdef TCPCONN
	    struct sockaddr_in in;
#endif
#ifdef DNETCONN
	    struct sockaddr_dn dn;
#endif
	} from;
	int fromlen;

	while (1) {

	    /* If we already have data available, don't try to read more */

	    if (havestate)
		break;

	    /* Wait for input or requests */

	    X11Flush();
	    copybits(selmask, mask);
	    i = select (maxsock, mask, (int *) NULL, (int *) NULL, &waittime);
	    if (i <= 0) {
		if (errno == EBADF)
		    Check_connections ();
		continue;
	    }

	    /* If request socket, deal with the new request */

	    while (rrmask = (mask[0] & requestmask)) {
		i = ffs (rrmask) - 1;
		rrmask = 1 << i;
		mask[0] &= ~rrmask;
		if ((i = accept (i, (struct sockaddr *) NULL, (int *) NULL)) >= 0) {
		    fromlen = sizeof (from);
		    if (i >= lastfdesc ||
			getpeername (i, &from.sa, &fromlen) ||
			Invalid_host (&from.sa, fromlen)) {
			close (i);
		    } else {
#ifdef TCP_NODELAY
			if (fromlen && (from.sa.sa_family == AF_INET)) {
			    mi = 1;
			    setsockopt (i, IPPROTO_TCP, TCP_NODELAY,
					&mi, sizeof (int));
			}
#endif

#ifdef SYSV
                        {
                            int fstat;
                            fstat = fcntl (i, F_GETFL, fstat);
                            if (fstat >= 0) {
                                fstat |= O_NDELAY;
                                fstat = fcntl (i, F_SETFL, fstat);
                            }
                        }
#else
			fcntl (i, F_SETFL, FNDELAY);
#endif /* SYSV */

			/*
			 * Set up the connection to the server for this client.
			 */
			if (! X11connect(devdisp, i)) {
			    close(i);
			    continue;
			}
			serverfd = X11serverfd(i);
			bitset(selmask, serverfd);
			bitset(servermask, serverfd);

			socketOn[i] = 1;
			requestId[i] = 0;
			if (servergrabber) {
			    bitset(grabselmask, i);
			    bitset(grabselmask, serverfd);
			} else {
			    bitset(selmask, i);
			}
			bufptr[i] = sockbuf[i] = Xalloc (DEFBUFSIZE);
			bufcnt[i] = 0;
			bufsize[i] = DEFBUFSIZE;
			if (serverfd >= maxsock)
			    maxsock = serverfd + 1;
#ifdef DUALTCP
			if (rrmask & swapmask)
			    swapped[i] = 1;
#endif
		    }
		}
	    }

	    /* Read from all ready sockets */

	    mi = 0;
	    i = firstsock;
	    rrmask = firstmask;
	    maskval = mask[0];
	    while (1) {
		if (!maskval) {
		    if (mi == (mskcnt - 1))
			break;
		    mi++;
		    i = mi << 5;
		    rrmask = 1;
		    maskval = mask[mi];
		    continue;
		}
		while (!(maskval & rrmask)) {
		    rrmask += rrmask;
		    i++;
		}
		maskval &= ~rrmask;
		if (servermask[ mi ] & rrmask) {
			X11events(i);
		} else {
		    /* copy down any existing data */
		    if (bufcnt[i] && (bufptr[i] != sockbuf[i]))
			bcopy (bufptr[i], sockbuf[i], bufcnt[i]);

		    /* then read as much as we can */
		    bufptr[i] = sockbuf[i];
		    if ((numread = read (i, bufptr[i] + bufcnt[i],
					 bufsize[i] - bufcnt[i])) <= 0)
			Close_down (i);
		    /* see if we have enough for a request */
		    else if ((bufcnt[i] += numread) >= sizeof (XReq)) {
			havemask[mi] |= rrmask;
			if (havestate == 0)
			    havestate = i;
			else if (havestate > 0)
			    havestate = -2;
			else
			    havestate--;
		    }
		}
		rrmask += rrmask;
		i++;
	    }
	}

	if (havestate > 0) {
	    rr_counter = havestate;
	    return;
	}

	/* Handle multiple requests on a round-robin basis */

	i = rr_counter + 1;
	while (1) {
	    if (i >= maxsock)
		i = firstsock;
	    rrmask = bitmask(i);
	    if (maskval = (maskword(havemask, i) & -rrmask))
		break;
	    i += 32;
	    i &= ~31;
	}

	while (!(maskval & rrmask)) {
	    rrmask += rrmask;
	    i++;
	}

	rr_counter = i;
	if (havestate == -1)
	    havestate = i;
}

/* Write data to client.
 * We might have to wait, if the client isn't keeping up with us.  We wait for
 * a short time, then close the connection.  This isn't a wonderful solution,
 * but it rarely seems to be a problem right now, and buffering output for
 * asynchronous delivery sounds complicated and expensive.
 */

Write (client, buf, total)
	int client, total;
	char *buf;
{
	int count = total;
	register int n;
	int mask[mskcnt];

	while (1) {
	    if ((n = write (client, buf, count)) == total)
		return;
	    if (n > 0) {
		buf += n;
		total -= n;
		if (total < count)
		    count = total;
	    } else if (errno == EMSGSIZE)
		count >>= 1;
	    else if (errno != EWOULDBLOCK)
		return;
	    singlebit(mask, client);
	    n = select (client + 1, (int *) NULL, mask, (int *) NULL, &outtime);
	    if (n != 1) {
		close (client);
		return;
	    }
	}
}

/* Read data from client.
 * Returns NULL if the data isn't immediately available, and backs up the
 * buffer structures to re-read the request.
 */

caddr_t Read_segment (client, size)
	register int client;
	register int size;
{
	register int idx, mask;
	char *ptr;

	if (bufcnt[client] >= size) {
	    ptr = bufptr[client];
	    bufptr[client] += size;
	    /* see if there is a request left */
	    if ((bufcnt[client] -= size) < sizeof (XReq)) {
		idx = maskidx(client);
		mask = bitmask(client);
		if (havemask[idx] & mask) {
		    havemask[idx] &= ~mask;
		    if (havestate < 0)
			havestate++;
		    else
			havestate = 0;
		}
	    }
	    return (ptr);
	}

	/* back up to the request */
	bufptr[client] -= sizeof (XReq);
	bufcnt[client] += sizeof (XReq);
	requestId[client]--;
#ifdef DUALTCP
	if (swapped[client])
	    Swap_request ((XReq *) bufptr[client]);
#endif
	/* but make it look like not enough is there */
	idx = maskidx(client);
	mask = bitmask(client);
	if (havemask[idx] & mask) {
	    havemask[idx] &= ~mask;
	    if (havestate < 0)
		havestate++;
	    else
		havestate = 0;
	}
	if (size + sizeof (XReq) > bufsize[client]) {
	    /* must increase the buffer to accomodate what's coming */
	    if (size <= MAXBUFSIZE) {
		ptr = Xalloc (bufsize[client] = size + sizeof (XReq));
		bcopy (bufptr[client], ptr, bufcnt[client]);
		free (sockbuf[client]);
		sockbuf[client] = bufptr[client] = ptr;
	    } else
		Close_down (client);
	}

	Xincomplete = 1;
	return (NULL);
}

/* Give client sole access to server */

Grab_server (client)
	register int client;
{
	register int idx, mask;

	if (servergrabber == 0) {
	    copybits(selmask, grabselmask);
	    clearbits(selmask);
	    copybits(servermask, selmask);
	    selmask[0] |= requestmask;
	    idx = maskidx(client);
	    mask = bitmask(client);
	    selmask[idx] |= mask;
	    copybits(havemask, grabhavemask);
	    clearbits(havemask);
	    grabhavestate = havestate;
	    if (grabhavemask[idx] & mask) {
		grabhavemask[idx] &= ~mask;
		havemask[idx] = mask;
		havestate = client;
		if (grabhavestate < 0)
		    grabhavestate++;
		else
		    grabhavestate = 0;
	    } else
		havestate = 0;
	    servergrabber = client;
	}
}

/* Restore global access to server */

Ungrab_server ()
{
	if (servergrabber) {
	    copybits(grabselmask, selmask);
	    if (havestate) {
		bitset(grabhavemask, servergrabber);
		if (grabhavestate == 0)
		    grabhavestate = servergrabber;
		else if (grabhavestate > 0)
		    grabhavestate = -2;
		else
		    grabhavestate--;
	    }
	    havestate = grabhavestate;
	    copybits(grabhavemask, havemask);
	    servergrabber = 0;
	}
}

/* Check the status of each client, and close our side of any goners. */

Check_connections ()
{
	register int i;
	int mask[mskcnt];

	for (i = firstsock; i < maxsock; i++) {
	    if (!socketOn[i])
		continue;
	    singlebit(mask, i);
	    if (select (i + 1, mask, (int *) NULL, (int *) NULL, &notime) < 0)
		Close_down (i);
	}
}

/* Close down a client, freeing its resources. */

Close_down (i)
	register int i;
{
	register int idx, mask, serverfd = X11serverfd(i);

	if (servergrabber)
	    Ungrab_server ();
	idx = maskidx(i);
	mask = bitmask(i) | bitmask(serverfd);
	selmask[idx] &= ~mask;
	if (havemask[idx] & mask) {
	    havemask[idx] &= ~mask;
	    if (havestate < 0)
		havestate++;
	    else
		havestate = 0;
	}
	close (i);
	socketOn[i] = 0;
	if (i + 1 == maxsock) {
	    while ((maxsock > firstsock) && (!socketOn[maxsock - 1]))
		--maxsock;
	}
	free (sockbuf[i]);
#ifdef DUALTCP
	swapped[i] = 0;
#endif
	Free_client_resources (i);
	/* if nobody is left, clean up after them */
}

/* This is where we set up socket connections, etc. */

Initialize ()
{
	char fname[32];
	int request, temp;
#ifdef TCPCONN
	int whichbyte;
	int tcpport1;
#ifdef DUALTCP
	int tcpport2;
#endif
	struct sockaddr_in mysock;
#ifndef SO_DONTLINGER
	static int linger[2] = { 0, 0 };
#endif
#endif
#ifdef UNIXCONN
	struct sockaddr_un unsock;
#endif
#ifdef DNETCONN
	struct sockaddr_dn dnsock;
#endif
	int retry;

#ifdef SYSV
	lastfdesc = _NFILE - 1;
#else
	lastfdesc = getdtablesize() - 1;
#endif
	if (lastfdesc > maxsocks)
	    lastfdesc = maxsocks;
	/* hack test to decide where to log errors */
	if (write (2, fname, 0)) {
	    strcpy (fname, "/usr/adm/Xtrans");
	    strcat (fname, display);
	    strcat (fname, "msgs");
	    freopen (fname, "w+", stderr);
	}
	if (getpgrp (0) == 0)
	    setpgrp (0, getpid ());

	requestmask = 0;
#ifdef DUALTCP
	swapmask = 0;
#endif

#ifdef TCPCONN
	tcpport1 = atoi (display);
	whichbyte = 1;
	if (*(char *) &whichbyte) {
#ifdef DUALTCP
	    tcpport2 = tcpport1 + X_TCP_BI_PORT;
#endif
	    tcpport1 += X_TCP_LI_PORT;
	} else {
#ifdef DUALTCP
	    tcpport2 = tcpport1 + X_TCP_LI_PORT;
#endif
	    tcpport1 += X_TCP_BI_PORT;
	}
#endif

	signal (SIGPIPE, SIG_IGN);
#ifdef TCPCONN
	if ((temp = socket (AF_INET, SOCK_STREAM, 0)) < 0) {
	    Notice ("Creating TCP socket");
	} else {
	    request = temp;
	    bzero ((char *)&mysock, sizeof (mysock));
	    mysock.sin_family = AF_INET;
	    mysock.sin_port = htons (tcpport1);
	    mysock.sin_addr.s_addr = htonl(INADDR_ANY);
	    retry = 20;
	    while (bind (request, (struct sockaddr *) &mysock,
			 sizeof (mysock))) {
		if (--retry == 0)
		    Error ("Binding TCP socket");
		sleep (10);
	    }
#ifdef SO_DONTLINGER
	    if (setsockopt (request, SOL_SOCKET, SO_DONTLINGER, 0, 0))
#else
	    if (setsockopt (request, SOL_SOCKET, SO_LINGER,
			    linger, sizeof (linger)))
#endif
		Notice ("Setting TCP DONTLINGER");
	    if (listen (request, 5)) Error ("TCP Listening");
	    requestmask |= (1 << request);
	    Define_self (request);
	}
#ifdef DUALTCP
	if ((temp = socket (AF_INET, SOCK_STREAM, 0)) < 0) {
	    Notice ("Creating dual TCP socket");
	} else {
	    request = temp;
	    mysock.sin_port = htons (tcpport2);
	    retry = 20;
	    while (bind (request, (struct sockaddr *) &mysock,
			 sizeof (mysock))) {
		if (--retry == 0)
		    Error ("Binding dual TCP socket");
		sleep (10);
	    }
#ifdef SO_DONTLINGER
	    if (setsockopt (request, SOL_SOCKET, SO_DONTLINGER, 0, 0))
#else
	    if (setsockopt (request, SOL_SOCKET, SO_LINGER,
			    linger, sizeof (linger)))
#endif
		Notice ("Setting dual TCP DONTLINGER");
	    if (listen (request, 5)) Error ("dual TCP Listening");
	    requestmask |= (1 << request);
	    swapmask |= (1 << request);
	    Define_self (request);
	}
#endif
#endif
#ifdef UNIXCONN
	unsock.sun_family = AF_UNIX;
	strcpy (unsock.sun_path, X_UNIX_PATH);
	strcat (unsock.sun_path, display);
	unlink (unsock.sun_path);
	if ((temp = socket (AF_UNIX, SOCK_STREAM, 0)) < 0) {
	    Notice ("Creating Unix socket");
	} else {
	    request = temp;
	    if (bind (request, (struct sockaddr *) &unsock,
		      strlen (unsock.sun_path) + 2))
		Error ("Binding Unix socket");
	    if(chmod(unsock.sun_path, 0666))
		Error ("Setting Mode on Unix socket");
	    if (listen (request, 5)) Error ("Unix Listening");
	    requestmask |= (1 << request);
	}
#endif
#ifdef DNETCONN
	if ((temp = socket (AF_DECnet, SOCK_STREAM, 0)) < 0) {
	    Notice ("Creating DECnet socket");
	} else {
	    request = temp;
	    bzero ((char *)&dnsock, sizeof (dnsock));
	    dnsock.sdn_family = AF_DECnet;
	    sprintf(dnsock.sdn_objname, "X%d", atoi (display));
	    dnsock.sdn_objnamel = strlen(dnsock.sdn_objname);
	    if (bind (request, (struct sockaddr *) &dnsock, sizeof (dnsock)))
		    Error ("Binding DECnet socket");
	    if (listen (request, 5)) Error ("DECnet Listening");
	    requestmask |= (1 << request);
	    Define_self (request);
	}
#endif
	if (requestmask == 0) Error ("No Listeners");
	signal (SIGHUP, HangUp);
	selmask[0] = requestmask;
	rr_counter = request;
	havestate = 0;
	firstsock = maxsock = request + 1;
	firstmask = 1 << maxsock;
	Reset_hosts(display);
}

/* Force connections to close on SIGHUP from init */

HangUp ()
{
	int i;

#ifdef GPROF
	chdir ("/tmp");
	exit (0);
#endif
	for (i = firstsock; i < maxsock; i++) {
	    if (socketOn[i])
		close (i);
	}
}

/* log a server error */

Notice (fmt, x0,x1,x2,x3,x4,x5,x6,x7,x8,x9,xa,xb,xc,xd,xe,xf,xg,xh,xi,xj)
	char *fmt;
{
	if (errno > 0 && errno < sys_nerr)
		fprintf (stderr, "error: %s\n", sys_errlist[errno]);
	errno = 0;
	fprintf (stderr, fmt, x0,x1,x2,x3,x4,x5,x6,x7,x8,x9,
			      xa,xb,xc,xd,xe,xf,xg,xh,xi,xj);
	putc('\n', stderr);
	fflush (stderr);
	errno = 0;
}

/* log a fatal server error */

Error (where, x0,x1,x2,x3,x4,x5,x6,x7,x8,x9)
	char *where;
{
	fprintf (stderr, "fatal ");
	Notice (where, x0,x1,x2,x3,x4,x5,x6,x7,x8,x9);
	if (errdelay)
	    sleep (errdelay);
	exit (1);
}

