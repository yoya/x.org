/* $XConsortium: connection.c,v 1.6 94/04/17 21:17:12 dpw Exp $ */
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
/*
 *
 * The connection code/ideas for SVR4/Intel environments was contributed by
 * the following companies/groups:
 *
 *	MetroLink Inc
 *	NCR
 *	Pittsburgh Powercomputing Corporation (PPc)/Quarterdeck Office Systems
 *	SGCS
 *	Unix System Laboratories (USL) / Novell
 *	XFree86
 *
 * The goal is to have common connection code among all SVR4/Intel vendors.
 *
 * ALL THE ABOVE COMPANIES DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS 
 * SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, 
 * IN NO EVENT SHALL THESE COMPANIES BE LIABLE FOR ANY SPECIAL, INDIRECT 
 * OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS 
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE 
 * OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE 
 * OR PERFORMANCE OF THIS SOFTWARE.
 */

/*****************************************************************
 *  Stuff to create connections --- OS dependent
 *
 *      EstablishNewConnections, CreateWellKnownSockets, ResetWellKnownSockets,
 *      CloseDownConnection, CheckConnections, AddEnabledDevice,
 *	RemoveEnabledDevice, OnlyListToOneClient,
 *      ListenToAllClients,
 *
 *      (WaitForSomething is in its own file)
 *
 *      In this implementation, a client socket table is not kept.
 *      Instead, what would be the index into the table is just the
 *      file descriptor of the socket.  This won't work for if the
 *      socket ids aren't small nums (0 - 2^8)
 *
 *****************************************************************/

#include "X.h"
#include "Xproto.h"
#include "Xos.h"			/* for strings, file, time */
#include <sys/param.h>
#include <errno.h>
#ifdef X_NOT_STDC_ENV
extern int errno;
#endif
#include <sys/socket.h>

#include <signal.h>
#include <setjmp.h>

#ifdef hpux
#include <sys/utsname.h>
#include <sys/ioctl.h>
#endif

#ifdef AIXV3
#include <sys/ioctl.h>
#endif

#ifdef TCPCONN
# include <netinet/in.h>
# ifndef hpux
#  ifdef apollo
#   ifndef NO_TCP_H
#    include <netinet/tcp.h>
#   endif
#  else
#   include <netinet/tcp.h>
#  endif
# endif
#endif

#if defined(SO_DONTLINGER) && defined(SO_LINGER)
#undef SO_DONTLINGER
#endif

#ifdef UNIXCONN
/*
 * sites should be careful to have separate /tmp directories for diskless nodes
 */
#include <sys/un.h>
#include <sys/stat.h>
static int unixDomainConnection = -1;
#endif

#include <stdio.h>
#include <sys/uio.h>
#include "misc.h"		/* for typedef of pointer */
#include "osdep.h"
#include "opaque.h"
#include "dixstruct.h"

#ifdef DNETCONN
#include <netdnet/dn.h>
#endif /* DNETCONN */

typedef long CCID;      /* mask of indices into client socket table */

#ifndef X_NOT_POSIX
#include <unistd.h>
#else
extern int read(), close();
#endif

#ifndef X_UNIX_PATH
#ifdef hpux
#define X_UNIX_DIR	"/usr/spool/sockets/X11"
#define X_UNIX_PATH	"/usr/spool/sockets/X11/"
#define OLD_UNIX_DIR	"/tmp/.X11-unix"
#else
#define X_UNIX_DIR	"/tmp/.X11-unix"
#define X_UNIX_PATH	"/tmp/.X11-unix/X"
#endif
#endif

extern char *display;		/* The display number */
int lastfdesc;			/* maximum file descriptor */

FdMask WellKnownConnections;	/* Listener mask */
FdSet EnabledDevices;		/* mask for input devices that are on */
FdSet AllSockets;		/* select on this */
FdSet AllClients;		/* available clients */
FdSet LastSelectMask;		/* mask returned from last select call */
FdSet ClientsWithInput;		/* clients with FULL requests in buffer */
FdSet ClientsWriteBlocked;	/* clients who cannot receive output */
FdSet OutputPending;		/* clients with reply/event data ready to go */
int MaxClients = MAXSOCKS;
long NConnBitArrays = mskcnt;
Bool NewOutputPending;		/* not yet attempted to write some new output */
Bool AnyClientsWriteBlocked;	/* true if some client blocked on write */

Bool RunFromSmartParent;	/* send SIGUSR1 to parent process */
Bool PartialNetwork;		/* continue even if unable to bind all addrs */
static int ParentProcess;

static Bool debug_conns = FALSE;

static FdSet IgnoredClientsWithInput;
static FdSet GrabImperviousClients;
static FdSet SavedAllClients;
static FdSet SavedAllSockets;
static FdSet SavedClientsWithInput;
int GrabInProgress = 0;

int ConnectionTranslation[MAXSOCKS];
int ConnectionOutputTranslation[MAXSOCKS];
extern int auditTrailLevel;
extern ClientPtr NextAvailableClient();
extern XID CheckAuthorization();

static void ErrorConnMax(
#if NeedFunctionPrototypes
    register int /*fd*/
#endif
);

void CloseDownFileDescriptor(
#if NeedFunctionPrototypes
    ClientPtr /*client*/
#endif
);

#ifdef XDMCP
void XdmcpOpenDisplay(), XdmcpInit(), XdmcpReset(), XdmcpCloseDisplay();
#endif

extern int  StandardReadRequestFromClient();
extern int  StandardWriteToClient ();
extern unsigned long  StandardRequestLength ();
extern int  StandardFlushClient ();

#ifdef TCPCONN
static int
open_tcp_socket ()
{
    struct sockaddr_in insock;
    int request;
    int retry;
#ifndef SO_DONTLINGER
#ifdef SO_LINGER
    static int linger[2] = { 0, 0 };
#endif /* SO_LINGER */
#endif /* SO_DONTLINGER */

    if ((request = socket (AF_INET, SOCK_STREAM, 0)) < 0) 
    {
	Error ("Creating TCP socket");
	return -1;
    } 
#ifdef SO_REUSEADDR
    /* Necesary to restart the server without a reboot */
    {
	int one = 1;
	setsockopt(request, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int));
    }
#endif /* SO_REUSEADDR */
    {
    bzero ((char *)&insock, sizeof (insock));
#ifdef BSD44SOCKETS
    insock.sin_len = sizeof(insock);
#endif
    insock.sin_family = AF_INET;
    insock.sin_port = htons ((unsigned short)(X_TCP_PORT + atoi (display)));
    insock.sin_addr.s_addr = htonl(INADDR_ANY);
    retry = 20;
    while (bind(request, (struct sockaddr *) &insock, sizeof (insock)))
    {
	if (--retry == 0) {
	    Error ("Binding TCP socket");
	    close (request);
	    return -1;
	}
#ifdef SO_REUSEADDR
	sleep (1);
#else
	sleep (10);
#endif /* SO_REUSEDADDR */
    }
    }
#ifdef SO_DONTLINGER
    if(setsockopt (request, SOL_SOCKET, SO_DONTLINGER, (char *)NULL, 0))
	Error ("Setting TCP SO_DONTLINGER");
#else
#ifdef SO_LINGER
    if(setsockopt (request, SOL_SOCKET, SO_LINGER,
		   (char *)linger, sizeof(linger)))
	Error ("Setting TCP SO_LINGER");
#endif /* SO_LINGER */
#endif /* SO_DONTLINGER */
    if (listen (request, 5)) {
	Error ("TCP Listening");
	close (request);
	return -1;
    }
    return request;
}
#endif /* TCPCONN */

#if defined(UNIXCONN) && !defined(LOCALCONN)

static struct sockaddr_un unsock;

static int
open_unix_socket ()
{
    int oldUmask;
    int request;

    bzero ((char *) &unsock, sizeof (unsock));
    unsock.sun_family = AF_UNIX;
    oldUmask = umask (0);
#ifdef X_UNIX_DIR
    if (!mkdir (X_UNIX_DIR, 0777))
	chmod (X_UNIX_DIR, 0777);
#endif
    strcpy (unsock.sun_path, X_UNIX_PATH);
    strcat (unsock.sun_path, display);
#ifdef BSD44SOCKETS
    unsock.sun_len = strlen(unsock.sun_path);
#endif
#ifdef hpux
    {  
        /*    The following is for backwards compatibility
         *    with old HP clients. This old scheme predates the use
 	 *    of the /usr/spool/sockets directory, and uses hostname:display
 	 *    in the /tmp/.X11-unix directory
         */
        struct utsname systemName;
	static char oldLinkName[256];

        uname(&systemName);
        strcpy(oldLinkName, OLD_UNIX_DIR);
        if (!mkdir(oldLinkName, 0777))
	    chown(oldLinkName, 2, 3);
        strcat(oldLinkName, "/");
        strcat(oldLinkName, systemName.nodename);
        strcat(oldLinkName, display);
        unlink(oldLinkName);
        symlink(unsock.sun_path, oldLinkName);
    }
#endif	/* hpux */
    unlink (unsock.sun_path);
    if ((request = socket (AF_UNIX, SOCK_STREAM, 0)) < 0) 
    {
	Error ("Creating Unix socket");
	return -1;
    } 
#ifdef BSD44SOCKETS
    if (bind(request, (struct sockaddr *)&unsock, SUN_LEN(&unsock)))
#else
    if (bind(request, (struct sockaddr *)&unsock, strlen(unsock.sun_path)+2))
#endif
    {
	Error ("Binding Unix socket");
	close (request);
	return -1;
    }
    if (listen (request, 5))
    {
	Error ("Unix Listening");
	close (request);
	return -1;
    }
    (void)umask(oldUmask);
    return request;
}
#endif /*UNIXCONN */

#ifdef LOCALCONN
/*
 * This code amply demonstrates why vendors need to talk to each other
 * earlier rather than later.
 *
 * The following is an implementation of the X-server-half of a variety
 * of SYSV386 local connection methods.  This includes compatability
 * with ISC STREAMS, SCO STREAMS, ATT pts connections, ATT/USL named-pipes,
 * and SVR4.2 UNIX DOMAIN connections.  To enable the output of various
 * connection-related information messages to stderr, compile with
 * -DXLOCAL_VERBOSE and set the environment variable XLOCAL_VERBOSE.
 *	XLOCAL_VERBOSE	= 0	print connection availability
 *			= 1	also, print message for each connection
 *			= 2	also, print cleanup information
 *			= 3	even more...
 *
 * See note below about the conflict between ISC and UNIX nodes.
 */

#include <sys/stream.h>
#include <sys/stropts.h>
#include <sys/utsname.h>
#ifndef UNIXCONN
#include <sys/stat.h>
#endif

#define X_STREAMS_DIR	"/dev/X"

#ifdef UNIXCONN
#define XLOCAL_UNIX
#define X_UNIX_DEVDIR	"/dev/X/UNIXCON"
#define X_UNIX_DEVPATH	"/dev/X/UNIXCON/X"
#endif

#if !defined(SVR4) || defined(SVR4_ACP)
#define XLOCAL_ISC
#define XLOCAL_SCO
#define X_ISC_DIR	"/dev/X/ISCCONN"
#define X_ISC_PATH	"/dev/X/ISCCONN/X"
#define X_SCO_PATH	"/dev/X"
#define DEV_SPX		"/dev/spx"
static int iscFd = -1;
static int scoFd = -1;
#endif

#ifdef unix
#define XLOCAL_PTS
#define X_PTS_PATH	"/dev/X/server."
#define DEV_PTMX	"/dev/ptmx"
extern char *ptsname();
static int ptsFd = -1;
#endif

#ifdef SVR4
#define XLOCAL_NAMED
static int namedFd = -1;
#define X_NAMED_PATH	"/dev/X/Nserver."
#endif

static FdSet AllStreams; /* bitmap of STREAMS file descriptors */

#ifndef XLOCAL_VERBOSE
#define XLOCAL_MSG(x)	/* as nothing */
#else
static void xlocalMsg();
#define XLOCAL_MSG(x)	xlocalMsg x;
#endif

static int useSlashTmpForUNIX=0;

#ifdef XLOCAL_VERBOSE
/*PRINTFLIKE1*/
static void
xlocalMsg(lvl,str,a,b,c,d,e,f,g,h)
  int lvl;
  char *str;
  int a,b,c,d,e,f,g,h;
{
    static int xlocalMsgLvl = -2;

    if (xlocalMsgLvl == -2) {
	char *tmp;
	if ((tmp = (char *)getenv("XLOCAL_VERBOSE")) != NULL) {
	    xlocalMsgLvl = atoi(tmp);
	} else {
	    xlocalMsgLvl = -1; 
	}
    }
    if (xlocalMsgLvl >= lvl) {
	fprintf(stderr,"X: XLOCAL - ");
	fprintf(stderr,str,a,b,c,d,e,f,g,h);
    }
}
#endif /* XLOCAL_VERBOSE */

/*
 * We have a conflict between ISC and UNIX connections over the use
 * of the /tmp/.X11-unix/Xn path.  Therefore, whichever connection type
 * is specified first in the XLOCAL environment variable gets to use this
 * path for its own device nodes.  The default is ISC.
 *
 * Note that both connection types are always available using their
 * alternate paths at /dev/X/ISCCONN/Xn and /dev/X/UNIXCON/Xn respectively.
 *
 * To make an older client or library use these alternate paths, you
 * need to edit the binary and replace /tmp/.X11-unix with either
 * /dev/X/ISCCONN or /dev/X/UNIXCON depending on its preference.
 */
#define WHITE	" :\t\n\r"

static void
ChooseLocalConnectionType()
{
    char *name,*nameList;

    XLOCAL_MSG((3,"Choosing ISC vs. UNIXDOMAIN connections...\n"));

    useSlashTmpForUNIX=0;

    if ((nameList = (char *)getenv("XLOCAL")) != NULL) {
	nameList = (char *)strdup(nameList);
	name = strtok(nameList,WHITE);
	while (name) {
	    if (!strncmp(name,"SP",2) ||
		!strncmp(name,"ISC",3) ||
		!strncmp(name,"STREAMS",7)) {
		break;
	    } else if (!strncmp(name,"UNIX",4)) {
		useSlashTmpForUNIX=1;
		break;
	    }
	    name = strtok(NULL,WHITE);
	}
	xfree(nameList); 
    } else {
	XLOCAL_MSG((3,"XLOCAL not set in environment.\n"));
    }

    XLOCAL_MSG((3,"Using %s for local connections in /tmp/.X11-unix.\n",
      useSlashTmpForUNIX ? "UNIXCONN" : "ISC"));
}
#undef WHITE

static int
xlocal_unlink(path)
  char *path;
{
    int ret;

    ret = unlink(path);
    if (ret == -1 && errno == EINTR)
      ret = unlink(path);
    if (ret == -1 && errno == ENOENT)
      ret = 0;
    return(ret);
}

#ifdef XLOCAL_UNIX
static struct sockaddr_un unsock;

/* UNIX: UNIX domain sockets as used in SVR4.2 */
static int
open_unix_local()
{
    int oldUmask;
    int request;

    bzero((char *) &unsock, sizeof (unsock));
    unsock.sun_family = AF_UNIX;
    oldUmask = umask (0);

    mkdir(X_STREAMS_DIR, 0777); /* "/dev/X" */
    chmod(X_STREAMS_DIR, 0777);
    if (!mkdir(X_UNIX_DEVDIR, 0777))
      chmod(X_UNIX_DEVDIR, 0777);
    strcpy(unsock.sun_path, X_UNIX_DEVPATH);
    strcat(unsock.sun_path, display);
    xlocal_unlink(unsock.sun_path);
    if ((request = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
	Error("Creating Unix socket");
	return -1;
    } 
    if (bind(request, (struct sockaddr *)&unsock, strlen(unsock.sun_path)+2)) {
	Error("Binding Unix socket");
	close(request);
	return -1;
    }
    if (listen(request, 5)) {
	Error("Unix Listening");
	close(request);
	return -1;
    }
    XLOCAL_MSG((0,"UNIX connections available at [%s]\n", unsock.sun_path));

    if (useSlashTmpForUNIX) {
	char tmpPath[64];
	if (!mkdir(X_UNIX_DIR, 0777))
	  chmod(X_UNIX_DIR, 0777);
	strcpy(tmpPath, X_UNIX_PATH);
	strcat(tmpPath, display);
	xlocal_unlink(tmpPath);
	if (link(unsock.sun_path,tmpPath) == 0) {
	    XLOCAL_MSG((0,"UNIX connections available at [%s]\n", tmpPath));
	}
    }
    (void)umask(oldUmask);
    return request;
}

static void
close_unix_local()
{
    char path[64];

    if (unixDomainConnection != -1) {
	close(unixDomainConnection);
	WellKnownConnections &= ~(1L << unixDomainConnection);
	unixDomainConnection = -1;
    }
    strcpy(path, X_UNIX_DEVPATH);
    strcat(path, display);
    XLOCAL_MSG((2,"removing [%s] and [%s]\n",path,X_UNIX_DEVDIR)); 
    xlocal_unlink(path);
    xlocal_unlink(X_UNIX_DEVDIR);
    if (useSlashTmpForUNIX) {
	strcpy(path, X_UNIX_PATH);
	strcat(path, display);
	XLOCAL_MSG((2,"removing [%s] and [%s]\n",path,X_UNIX_DIR)); 
	xlocal_unlink(path);
	xlocal_unlink(X_UNIX_DIR);
    }
}

static void
reset_unix_local()
{
    char path[64];
    struct stat statb;
    int need_reset = 0;

    if (unixDomainConnection != -1) {
	strcpy(path, X_UNIX_DEVPATH);
	strcat(path, display);
	if ((stat(path, &statb) == -1) ||
	    (statb.st_mode & S_IFMT) != S_IFSOCK)
	  need_reset = 1;

	if (useSlashTmpForUNIX) {
	    strcpy(path, X_UNIX_PATH);
	    strcat(path, display);
	    if ((stat(path, &statb) == -1) ||
		(statb.st_mode & S_IFMT) != S_IFSOCK)
	      need_reset = 1;
	}

	if (need_reset) {
	    close_unix_local();
	    unixDomainConnection = open_unix_local();
	    if (unixDomainConnection != -1)
	      WellKnownConnections |= (1L << unixDomainConnection);
	}
    }
}

#endif /* XLOCAL_UNIX */

#if defined(XLOCAL_ISC) || defined(XLOCAL_SCO)
static int
connect_spipe(fd1, fd2)
  int fd1, fd2;
{
    long temp;
    struct strfdinsert sbuf;

    sbuf.databuf.maxlen = -1;
    sbuf.databuf.len = -1;
    sbuf.databuf.buf = NULL;
    sbuf.ctlbuf.maxlen = sizeof(long);
    sbuf.ctlbuf.len = sizeof(long);
    sbuf.ctlbuf.buf = (caddr_t)&temp;
    sbuf.offset = 0;
    sbuf.fildes = fd2;
    sbuf.flags = 0;
    if (ioctl(fd1, I_FDINSERT, &sbuf) == -1) return (-1);
  
    return(0);
}

static int
named_spipe(fd, path)
  int fd;
  char *path;
{
    int oldUmask, ret;
    struct stat sbuf;

    oldUmask = umask(0);

    (void) fstat(fd, &sbuf);
    ret = mknod(path, 0020666, sbuf.st_rdev);

    umask(oldUmask);

    if (ret < 0) {
	ret = -1;
    } else {
	ret = fd;
    }
    return(ret);
}
#endif /* defined(XLOCAL_ISC) || defined(XLOCAL_SCO) */

#ifdef XLOCAL_ISC
/*
 * ISC: ISC UNIX V.3.2 compatible streams at /dev/X/ISCCONN/Xn.
 * It will link this to /tmp/.X11-unix/Xn if there are no UNIXDOMAIN
 * connection nodes required.
 */
static int
open_isc_local()
{
    int fd = -1,fds = -1;
    char pathISC[64],pathX11[64];

    mkdir(X_STREAMS_DIR, 0777); /* "/dev/X" */
    chmod(X_STREAMS_DIR, 0777);
    mkdir(X_ISC_DIR, 0777); /* "/dev/X/ISCCONN" */
    chmod(X_ISC_DIR, 0777);

    strcpy(pathISC, X_ISC_PATH);
    strcat(pathISC, display);
  
    if (xlocal_unlink(pathISC) < 0) {
	ErrorF("open_isc_local(): Can't unlink node (%s),\n", pathISC);
	return(-1);
    }

    if ((fds = open(DEV_SPX, O_RDWR)) >= 0 &&
	(fd  = open(DEV_SPX, O_RDWR)) >= 0 ) {

	if (connect_spipe(fds, fd) != -1 &&
	    named_spipe(fds, pathISC) != -1) {

	    XLOCAL_MSG((0,"ISC connections available at [%s]\n", pathISC));

	    if (!useSlashTmpForUNIX) {
		mkdir(X_UNIX_DIR, 0777);
		chmod(X_UNIX_DIR, 0777);
		strcpy(pathX11, X_UNIX_PATH);
		strcat(pathX11, display);
		if (xlocal_unlink(pathX11) < 0) {
		    /*EMPTY*/
		    /* can't get the /tmp node, just return the good one...*/
		} else {
#ifdef SVR4 
		    /* we prefer symbolic links as hard links can't
		       cross filesystems */
		    if (symlink(pathISC,pathX11) == 0) {
			XLOCAL_MSG((0,"ISC connections available at [%s]\n",
				    pathX11));
		    }
#else
		    if (link(pathISC,pathX11) == 0) {
			XLOCAL_MSG((0,"ISC connections available at [%s]\n",
				    pathX11));
		    }
		      
#endif
		}
	    }
	    return(fd);
	} else {
	    Error("open_isc_local(): Can't set up listener pipes");
	}
    } else {
	XLOCAL_MSG((0,"open_isc_local(): can't open %s\n",DEV_SPX));
#ifndef SVR4
	/*
	 * At this point, most SVR4 versions will fail on this, so leave out the
	 * warning
	 */
	Error("open_isc_local(): can't open \"%s\"",DEV_SPX);
	return(-1);
#endif
    }

    (void) close(fds);
    (void) close(fd);
    return(-1);
}

static int
accept_isc_local()
{
    struct strrecvfd buf;

    while (ioctl(iscFd, I_RECVFD, &buf) < 0)
      if (errno != EAGAIN) {
	  Error("accept_isc_local(): Can't read fildes");
	  return(-1);
      }

    XLOCAL_MSG((1,"new ISC connection accepted (%d)\n",buf.fd));
    BITSET(AllStreams, buf.fd);
    return(buf.fd);
}

static void
close_isc_local()
{
    char path[64];

    if (iscFd != -1) {
	close(iscFd);
	WellKnownConnections &= ~(1L << iscFd);
	iscFd = -1;
    }
    strcpy(path, X_ISC_PATH);
    strcat(path, display);
    XLOCAL_MSG((2,"removing [%s] and [%s]\n",path,X_ISC_DIR)); 
    xlocal_unlink(path);
    xlocal_unlink(X_ISC_DIR);
    if (!useSlashTmpForUNIX) {
	strcpy(path, X_UNIX_PATH);
	strcat(path, display);
	XLOCAL_MSG((2,"removing [%s] and [%s]\n",path,X_UNIX_DIR)); 
	xlocal_unlink(path);
	xlocal_unlink(X_UNIX_DIR);
    }
}
#endif /* XLOCAL_ISC */

#ifdef XLOCAL_SCO
/* SCO: SCO/XSIGHT style using /dev/spx */
static int
open_sco_local()
{
    int fds = -1,fdr = -1;
    char pathS[64], pathR[64];

    sprintf(pathS, "%s%sS",X_SCO_PATH, display);
    sprintf(pathR, "%s%sR",X_SCO_PATH, display);
  
    if ((xlocal_unlink(pathS) < 0) || (xlocal_unlink(pathR) < 0)) {

	ErrorF("open_sco_local(): can't unlink node (%s)\n",pathR);
	return(-1);
    }
  
    if ((fds = open(DEV_SPX, O_RDWR)) >= 0 &&
	(fdr = open(DEV_SPX, O_RDWR)) >= 0 ) {

	if (connect_spipe(fds, fdr) != -1 &&
	    named_spipe(fds, pathS) != -1 &&
	    named_spipe(fdr, pathR) != -1) {
      
	    XLOCAL_MSG((0,"SCO connections available at [%s]\n",pathR));

	    return(fds);
	} else {
	    Error("open_sco_local(): can't set up listener pipes");
	}  
    } else {
	XLOCAL_MSG((0,"open_sco_local(): can't open %s",DEV_SPX));
#ifndef SVR4
	/*
	 * At this point, most SVR4 versions will fail on this, so
	 * leave out the warning
	 */
	Error("open_sco_local(): can't open \"%s\"",DEV_SPX);
	return(-1);
#endif
    }

    (void) close(fds);
    (void) close(fdr);
    return(-1);
}


static int
accept_sco_local()
{
    char c;
    int fd;

    if (read(scoFd, &c, 1) < 0) {
	Error("accept_sco_local(): can't read from client");
	return(-1);
    }

    if ((fd = open(DEV_SPX, O_RDWR)) < 0) {
	ErrorF("accept_sco_local(): can't open \"%s\"",DEV_SPX);
	return(-1);
    }

    if (connect_spipe(scoFd, fd) < 0) {
	Error("accept_sco_local(): can't connect pipes");
	(void) close(fd);
	return(-1);
    }

    XLOCAL_MSG((1,"new SCO connection accepted (%d)\n",fd));
    BITSET(AllStreams, fd);
    return(fd);
}

static void
close_sco_local()
{
    char pathS[64], pathR[64];

    if (scoFd != -1) {
	close(scoFd);
	WellKnownConnections &= ~(1L << scoFd);
	scoFd = -1;
    }

    sprintf(pathS, "%s%sS",X_SCO_PATH, display);
    sprintf(pathR, "%s%sR",X_SCO_PATH, display);
  
    XLOCAL_MSG((2,"removing [%s] and [%s]\n",pathS,pathR)); 
    xlocal_unlink(pathS);
    xlocal_unlink(pathR);
}

#endif /* XLOCAL_SCO */

#ifdef XLOCAL_PTS
/* PTS: AT&T style using /dev/ptmx */
static int
open_pts_local()
{
    char *slave;
    int fd;
    char path[64];

    mkdir(X_STREAMS_DIR, 0777);
    chmod(X_STREAMS_DIR, 0777);
  
    strcpy(path, X_PTS_PATH);
    strcat(path, display);
  
    if (open(path, O_RDWR) >= 0 || (xlocal_unlink(path) < 0)) {
	ErrorF("open_pts_local(): server is already running (%s)\n", path);
	return(-1);
    }
  
    if ((fd = open(DEV_PTMX, O_RDWR)) < 0) {
	Error("open_pts_local(): open failed");
	ErrorF("open_pts_local(): can't open \"%s\"",DEV_PTMX);
	return(-1);
    }
  
    grantpt(fd);
    unlockpt(fd);
    slave = ptsname(fd);
    if (link(slave, path) < 0 || chmod(path, 0666) < 0) {
	Error("open_pts_local(): can't set up local listener");
	return(-1);
    }

    if (open(path, O_RDWR) < 0) {
	Error("open_pts_local(): open failed");
	ErrorF("open_pts_local(): can't open %s\n", path);
	close(fd);
	return(-1);
    }
    XLOCAL_MSG((0,"PTS connections available at [%s]\n",path));

    return(fd);
}

static int
accept_pts_local()
{
    int newconn;
    char length;
    char path[64];

    /*
     * first get device-name
     */
    if(read(ptsFd, &length, 1) <= 0 ) {
	Error("accept_pts_local(): can't read slave name length");
	return(-1);
    }

    if(read(ptsFd, path, length) <= 0 ) {
	Error("accept_pts_local(): can't read slave name");
	return(-1);
    }

    path[ length ] = '\0';
      
    if((newconn = open(path,O_RDWR)) < 0) {
	Error("accept_pts_local(): can't open slave");
	return(-1);
    }

    (void) write(newconn, "1", 1); /* send an acknowledge to the client */

    XLOCAL_MSG((1,"new PTS connection accepted (%d)\n",newconn));
    BITSET(AllStreams, newconn);
    return(newconn);
}

static void
close_pts_local()
{
    char path[64];

    if (ptsFd != -1) {
	close(ptsFd);
	WellKnownConnections &= ~(1L << ptsFd);
	ptsFd = -1;
    }
    strcpy(path, X_PTS_PATH);
    strcat(path, display);
    XLOCAL_MSG((2,"removing [%s]\n",path)); 
    xlocal_unlink(path);
}
#endif /* XLOCAL_PTS */

#ifdef XLOCAL_NAMED
/* NAMED: USL style using bi-directional named pipes */
static int
open_named_local()
{
    int fd,fld[2];
    char path[64];
    struct stat sbuf;

    mkdir(X_STREAMS_DIR, 0777);
    chmod(X_STREAMS_DIR, 0777);
  
    strcpy(path, X_NAMED_PATH);
    strcat(path, display);
  
    if (stat(path, &sbuf) != 0) {
	if (errno == ENOENT) {
	    if ((fd = creat(path, (mode_t)0666)) == -1) {
		ErrorF("open_named_local(): can't create %s\n",path);
		return(-1);
	    }
	    close(fd);
	    if (chmod(path, (mode_t)0666) < 0) {
		ErrorF("open_named_local(): can't chmod %s\n",path);
		return(-1);
	    }
	} else {
	    ErrorF("open_named_local(): unknown stat error, %d\n",errno);	
	    return(-1);
	}
    }

    if (pipe(fld) != 0) {
	ErrorF("open_named_local(): pipe failed, errno=%d\n",errno);
	return(-1);
    }

    if (ioctl(fld[0], I_PUSH, "connld") != 0) {
	ErrorF("open_named_local(): ioctl error %d\n",errno);
	return(-1);
    }

    if (fattach(fld[0], path) != 0) {
	ErrorF("open_named_local(): fattach failed, errno=%d\n",errno);
	return(-1);
    } 

    XLOCAL_MSG((0,"NAMED connections available at [%s]\n", path));

    return(fld[1]);
}

static int
accept_named_local()
{
    struct strrecvfd str;

    if (ioctl(namedFd, I_RECVFD, &str) < 0) {
	ErrorF("accept_named_local(): I_RECVFD failed\n");
	return(-1);
    }

    XLOCAL_MSG((1,"new NAMED connection accepted (%d)\n",str.fd));
    BITSET(AllStreams, str.fd);
    return(str.fd);
}

static void
close_named_local()
{
    char path[64];

    if (namedFd != -1) {
	close(namedFd);
	WellKnownConnections &= ~(1L << namedFd);
	namedFd = -1;
    }
    strcpy(path, X_NAMED_PATH);
    strcat(path, display);
    XLOCAL_MSG((2,"removing [%s]\n",path)); 
    xlocal_unlink(path);
}
#endif /* XLOCAL_NAMED */

static void
xlocal_create_sockets()
{
    int request;

    ChooseLocalConnectionType();

    CLEARBITS(AllStreams);
#ifdef XLOCAL_PTS
    if ((ptsFd = open_pts_local()) == -1) {
	XLOCAL_MSG((0,"open_pts_local(): failed.\n"));
    } else {
	WellKnownConnections |= (1L << ptsFd);
    }
#endif
#ifdef XLOCAL_NAMED
    if ((namedFd = open_named_local()) == -1) {
	XLOCAL_MSG((0,"open_named_local(): failed.\n"));
    } else {
	WellKnownConnections |= (1L << namedFd);
    }
#endif
#ifdef XLOCAL_ISC
    if ((iscFd = open_isc_local()) == -1) {
	XLOCAL_MSG((0,"open_isc_local(): failed.\n"));
    } else {
	WellKnownConnections |= (1L << iscFd);
    }
#endif
#ifdef XLOCAL_SCO
    if ((scoFd = open_sco_local()) == -1) {
	XLOCAL_MSG((0,"open_sco_local(): failed.\n"));
    } else {
	WellKnownConnections |= (1L << scoFd);
    } 
#endif
#ifdef XLOCAL_UNIX
    if ((request = open_unix_local()) == -1) {
	XLOCAL_MSG((0,"open_unix_local(): failed.\n"));
    } else {
	WellKnownConnections |= (1L << request);
	unixDomainConnection = request;
    }
#endif
#ifdef TCPCONN
    if ((request = open_tcp_socket()) == -1) {
	XLOCAL_MSG((0,"open_tcp_socket(): failed.\n"));
	if (PartialNetwork == FALSE) {
	    FatalError("Cannot establish tcp listening socket");
	}
    } else {
	XLOCAL_MSG((0,"TCP connections available at port %d\n",
		    X_TCP_PORT + atoi(display)));
	WellKnownConnections |= (1L << request);
	DefineSelf(request);
    }
#endif /* TCPCONN */
}

static void
xlocal_reset_sockets()
{
#ifdef XLOCAL_UNIX
    reset_unix_local();
#endif
    CLEARBITS(AllStreams);
}

static int
xlocal_close_sockets()
{
#ifdef XLOCAL_UNIX
    close_unix_local();
#endif /* XLOCAL_UNIX */
#ifdef XLOCAL_ISC
    close_isc_local();
#endif /* XLOCAL_ISC */
#ifdef XLOCAL_SCO
    close_sco_local();
#endif /* XLOCAL_SCO */
#ifdef XLOCAL_PTS
    close_pts_local();
#endif /* XLOCAL_PTS */
#ifdef XLOCAL_NAMED
    close_named_local();
#endif /* XLOCAL_NAMED */
    return(0);
}

int
xlocal_getpeername(fd, from, fromlen)
  int fd;
  struct sockaddr *from;
  int *fromlen;
{
    /* special case for local connections ( ISC, SCO, PTS, NAMED... ) */
    if (GETBIT(AllStreams, fd)) {
	from->sa_family = AF_UNSPEC;
	*fromlen = 0;
	return 0;
    }
#if defined(TCPCONN) || defined(DNETCONN) || defined(UNIXCONN)
    return getpeername(fd, from, fromlen);
#else
    return(-1);
#endif
}
#define	getpeername	xlocal_getpeername

static int
xlocal_accept(fd, from, fromlen)
  int fd;
  struct sockaddr *from;
  int *fromlen;
{
    int ret;

#ifdef XLOCAL_PTS
    if (fd == ptsFd) return accept_pts_local();
#endif
#ifdef XLOCAL_NAMED
    if (fd == namedFd) return accept_named_local();
#endif
#ifdef XLOCAL_ISC
    if (fd == iscFd) return accept_isc_local();
#endif
#ifdef XLOCAL_SCO
    if (fd == scoFd)  return accept_sco_local(); 
#endif
    /*
     * else we are handling the normal accept case
     */
#if defined(TCPCONN) || defined(DNETCONN) || defined(XLOCAL_UNIX)
    ret = accept (fd, from, fromlen);

#ifdef XLOCAL_UNIX
    if (fd == unixDomainConnection) {
	XLOCAL_MSG((1,"new UNIX connection accepted (%d)\n",ret));
    } else
#endif
      {
	  XLOCAL_MSG((1,"new TCP connection accepted (%d)\n",ret));
      }
#endif
    return(ret);
}
#define	accept		xlocal_accept

#endif /* LOCALCONN */


#ifdef hpux
/*
 * hpux returns EOPNOTSUPP when using getpeername on a unix-domain
 * socket.  In this case, smash the socket address with the address
 * used to bind the connection socket and return success.
 */
hpux_getpeername(fd, from, fromlen)
    int	fd;
    struct sockaddr *from;
    int		    *fromlen;
{
    int	    ret;
    int	    len;

    ret = getpeername(fd, from, fromlen);
    if (ret == -1 && errno == EOPNOTSUPP)
    {
	ret = 0;
	len = strlen(unsock.sun_path)+2;
	if (len > *fromlen)
	    len = *fromlen;
	memmove((char *) from, (char *) &unsock, len);
	*fromlen = len;
    }
    return ret;
}

#define getpeername(fd, from, fromlen)	hpux_getpeername(fd, from, fromlen)

#endif

#ifdef DNETCONN
static int
open_dnet_socket ()
{
    int request;
    struct sockaddr_dn dnsock;

    if ((request = socket (AF_DECnet, SOCK_STREAM, 0)) < 0) 
    {
	Error ("Creating DECnet socket");
	return -1;
    } 
    bzero ((char *)&dnsock, sizeof (dnsock));
    dnsock.sdn_family = AF_DECnet;
    sprintf(dnsock.sdn_objname, "X$X%d", atoi (display));
    dnsock.sdn_objnamel = strlen(dnsock.sdn_objname);
    if (bind (request, (struct sockaddr *) &dnsock, sizeof (dnsock)))
    {
	Error ("Binding DECnet socket");
	close (request);
	return -1;
    }
    if (listen (request, 5))
    {
	Error ("DECnet Listening");
	close (request);
	return -1;
    }
    return request;
}
#endif /* DNETCONN */

/*****************
 * CreateWellKnownSockets
 *    At initialization, create the sockets to listen on for new clients.
 *****************/

void
CreateWellKnownSockets()
{
    int		request, i;

    CLEARBITS(AllSockets);
    CLEARBITS(AllClients);
    CLEARBITS(LastSelectMask);
    CLEARBITS(ClientsWithInput);

    for (i=0; i<MAXSOCKS; i++) ConnectionTranslation[i] = 0;
    for (i=0; i<MAXSOCKS; i++) ConnectionOutputTranslation[i] = 0;
#ifndef X_NOT_POSIX
    lastfdesc = sysconf(_SC_OPEN_MAX) - 1;
#else
#ifdef hpux
    lastfdesc = _NFILE - 1;
#else
    lastfdesc = getdtablesize() - 1;
#endif
#endif

    if (lastfdesc > MAXSOCKS)
    {
	lastfdesc = MAXSOCKS;
	if (debug_conns)
	    ErrorF( "GOT TO END OF SOCKETS %d\n", MAXSOCKS);
    }

    WellKnownConnections = 0;

#ifdef LOCALCONN
	xlocal_create_sockets();
#else  /* LOCALCONN */
#ifdef TCPCONN
    if ((request = open_tcp_socket ()) != -1) {
	WellKnownConnections |= (((FdMask)1) << request);
	DefineSelf (request);
    }
    else if (!PartialNetwork) 
    {
	FatalError ("Cannot establish tcp listening socket");
    }
#endif /* TCPCONN */
#ifdef DNETCONN
    if ((request = open_dnet_socket ()) != -1) {
	WellKnownConnections |= (((FdMask)1) << request);
	DefineSelf (request);
    }
    else if (!PartialNetwork) 
    {
	FatalError ("Cannot establish dnet listening socket");
    }
#endif /* DNETCONN */
#ifdef UNIXCONN
    if ((request = open_unix_socket ()) != -1) {
	WellKnownConnections |= (((FdMask)1) << request);
	unixDomainConnection = request;
    }
    else if (!PartialNetwork) 
    {
	FatalError ("Cannot establish unix listening socket");
    }
#endif /* UNIXCONN */

#endif /* LOCALCONN */

    if (WellKnownConnections == 0)
        FatalError ("Cannot establish any listening sockets");
    OsSignal (SIGPIPE, SIG_IGN);
    OsSignal (SIGHUP, AutoResetServer);
    OsSignal (SIGINT, GiveUp);
    OsSignal (SIGTERM, GiveUp);
    AllSockets[0] = WellKnownConnections;
    ResetHosts(display);
    /*
     * Magic:  If SIGUSR1 was set to SIG_IGN when
     * the server started, assume that either
     *
     *  a- The parent process is ignoring SIGUSR1
     *
     * or
     *
     *  b- The parent process is expecting a SIGUSR1
     *     when the server is ready to accept connections
     *
     * In the first case, the signal will be harmless,
     * in the second case, the signal will be quite
     * useful
     */
    if (OsSignal (SIGUSR1, SIG_IGN) == SIG_IGN)
	RunFromSmartParent = TRUE;
    ParentProcess = getppid ();
    if (RunFromSmartParent) {
	if (ParentProcess > 0) {
	    kill (ParentProcess, SIGUSR1);
	}
    }
#ifdef XDMCP
    XdmcpInit ();
#endif
}

void
ResetWellKnownSockets ()
{
    ResetOsBuffers();

#ifdef LOCALCONN
	xlocal_reset_sockets();
#else /* LOCALCONN */
#if defined(UNIXCONN) && !defined(SVR4)
    if (unixDomainConnection != -1)
    {
	/*
	 * see if the unix domain socket has disappeared
	 */
	struct stat	statb;

	if (stat (unsock.sun_path, &statb) == -1 ||
	    (statb.st_mode & S_IFMT) != S_IFSOCK)
	{
	    ErrorF ("Unix domain socket %s trashed, recreating\n",
	    	unsock.sun_path);
	    (void) unlink (unsock.sun_path);
	    (void) close (unixDomainConnection);
	    WellKnownConnections &= ~(((FdMask)1) << unixDomainConnection);
	    unixDomainConnection = open_unix_socket ();
	    if (unixDomainConnection != -1)
		WellKnownConnections |= (((FdMask)1) << unixDomainConnection);
	}
    }
#endif /* UNIXCONN */

#endif /* LOCALCONN */


    ResetHosts(display);
    /*
     * See above in CreateWellKnownSockets about SIGUSR1
     */
    if (RunFromSmartParent) {
	if (ParentProcess > 0) {
	    kill (ParentProcess, SIGUSR1);
	}
    }
    /*
     * restart XDMCP
     */
#ifdef XDMCP
    XdmcpReset ();
#endif
}

/*****************************************************************
 * ClientAuthorized
 *
 *    Sent by the client at connection setup:
 *                typedef struct _xConnClientPrefix {
 *                   CARD8	byteOrder;
 *                   BYTE	pad;
 *                   CARD16	majorVersion, minorVersion;
 *                   CARD16	nbytesAuthProto;    
 *                   CARD16	nbytesAuthString;   
 *                 } xConnClientPrefix;
 *
 *     	It is hoped that eventually one protocol will be agreed upon.  In the
 *        mean time, a server that implements a different protocol than the
 *        client expects, or a server that only implements the host-based
 *        mechanism, will simply ignore this information.
 *
 *****************************************************************/

/* ARGSUSED */
char * 
ClientAuthorized(client, proto_n, auth_proto, string_n, auth_string)
    ClientPtr client;
    char *auth_proto, *auth_string;
    unsigned int proto_n, string_n;
{
    OsCommPtr oc = (OsCommPtr) client->osPrivate;
  
    oc->conn_time = 0;
    return((char *)NULL);
}

ClientConnectionNumber (client)
    ClientPtr	client;
{
    OsCommPtr oc = (OsCommPtr) client->osPrivate;

    return oc->fd;
}

AvailableClientInput (client)
    ClientPtr	client;
{
    OsCommPtr oc = (OsCommPtr)client->osPrivate;

    if (GETBIT(AllSockets, oc->fd))
	BITSET(ClientsWithInput, oc->fd);
}

ClientPtr
AllocNewConnection (fd, Read, Writev, Close)
    int	    fd;
    int	    (*Read)();
    int	    (*Writev)();
    void    (*Close)();
{
    OsCommPtr	oc;
    ClientPtr	client;
    
    if (fd >= lastfdesc)
	return NullClient;
    oc = (OsCommPtr)xalloc(sizeof(OsCommRec));
    if (!oc)
	return NullClient;
    oc->fd = fd;
    oc->input = (ConnectionInputPtr)NULL;
    oc->output = (ConnectionOutputPtr)NULL;
    oc->conn_time = GetTimeInMillis();
    oc->Read = Read;
    oc->Writev = Writev;
    oc->Close = Close;
    oc->flushClient = StandardFlushClient;
    oc->ofirst = (ConnectionOutputPtr) NULL;
    if (!(client = NextAvailableClient((pointer)oc)))
    {
	xfree (oc);
	return NullClient;
    }
    if (!ConnectionTranslation[fd])
    {
	ConnectionTranslation[fd] = client->index;
	ConnectionOutputTranslation[fd] = client->index;
	if (GrabInProgress)
	{
	    BITSET(SavedAllClients, fd);
	    BITSET(SavedAllSockets, fd);
	}
	else
	{
	    BITSET(AllClients, fd);
	    BITSET(AllSockets, fd);
	}
    }
    client->public.readRequest = StandardReadRequestFromClient;
    client->public.writeToClient = StandardWriteToClient;
    client->public.requestLength = StandardRequestLength;
    return client;
}

void
SwitchConnectionFuncs (client, Read, Writev, Close)
    ClientPtr	client;
    int		(*Read)();
    int		(*Writev)();
    void	(*Close)();
{
    OsCommPtr	oc = (OsCommPtr) client->osPrivate;

    oc->Read = Read;
    oc->Writev = Writev;
    oc->Close = Close;
    oc->conn_time = 0;
}

void
StartOutputCompression(client, CompressOn, CompressOff)
    ClientPtr	client;
    void	(*CompressOn)();
    void	(*CompressOff)();
{
    OsCommPtr	oc = (OsCommPtr) client->osPrivate;
    extern int	LbxFlushClient();

    oc->compressOn = CompressOn;
    oc->compressOff = CompressOff;
    oc->flushClient = LbxFlushClient;
}

int Writev(fd, iov, iovcnt)
    int fd;
    struct iovec *iov;
    int iovcnt;
{
    return writev(fd, iov, iovcnt);
}

/*****************
 * EstablishNewConnections
 *    If anyone is waiting on listened sockets, accept them.
 *    Returns a mask with indices of new clients.  Updates AllClients
 *    and AllSockets.
 *****************/

/*ARGSUSED*/
Bool
EstablishNewConnections(clientUnused, closure)
    ClientPtr clientUnused;
    pointer closure;
{
    FdMask readyconnections;     /* mask of listeners that are ready */
    int curconn;                  /* fd of listener that's ready */
    register int newconn;         /* fd of new client */
    CARD32 connect_time;
    register int i;
    register ClientPtr client;
    register OsCommPtr oc;

#ifdef TCP_NODELAY
    union {
	struct sockaddr sa;
#ifdef UNIXCONN
	struct sockaddr_un un;
#endif /* UNIXCONN */
#ifdef TCPCONN
	struct sockaddr_in in;
#endif /* TCPCONN */
#ifdef DNETCONN
	struct sockaddr_dn dn;
#endif /* DNETCONN */
    } from;
    int	fromlen;
#endif /* TCP_NODELAY */

    readyconnections = (((FdMask)closure) & WellKnownConnections);
    if (!readyconnections)
	return TRUE;
    connect_time = GetTimeInMillis();
    /* kill off stragglers */
    for (i=1; i<currentMaxClients; i++)
    {
	if (client = clients[i])
	{
	    oc = (OsCommPtr)(client->osPrivate);
	    if (oc && (oc->conn_time != 0) &&
		(connect_time - oc->conn_time) >= TimeOutValue)
		CloseDownClient(client);     
	}
    }
    while (readyconnections) 
    {
	curconn = ffs (readyconnections) - 1;
	readyconnections &= ~(1 << curconn);
	if ((newconn = accept (curconn,
			      (struct sockaddr *) NULL, 
			      (int *)NULL)) < 0) 
	    continue;
#ifdef TCP_NODELAY
	fromlen = sizeof (from);
	if (!getpeername (newconn, &from.sa, &fromlen))
	{
	    if (fromlen && (from.sa.sa_family == AF_INET)) 
	    {
		int mi = 1;
		setsockopt (newconn, IPPROTO_TCP, TCP_NODELAY,
			   (char *)&mi, sizeof (int));
	    }
	}
#endif /* TCP_NODELAY */
    /* ultrix reads hang on Unix sockets, hpux reads fail, AIX fails too */
#if defined(O_NONBLOCK) && (!defined(ultrix) && !defined(hpux) && !defined(AIXV3) && !defined(uniosu))
	(void) fcntl (newconn, F_SETFL, O_NONBLOCK);
#else
#ifdef FIOSNBIO
	{
	    int	arg;
	    arg = 1;
	    ioctl(newconn, FIOSNBIO, &arg);
	}
#else
#if (defined(AIXV3) || defined(uniosu)) && defined(FIONBIO)
	{
	    int arg;
	    arg = 1;
	    ioctl(newconn, FIONBIO, &arg);
	}
#else
	fcntl (newconn, F_SETFL, FNDELAY);
#endif
#endif
#endif

	client = AllocNewConnection (newconn, read, Writev, CloseDownFileDescriptor);
	if (!client)
	{
	    ErrorConnMax(newconn);
	    close(newconn);
	    continue;
	}
    }
    return TRUE;
}

#define NOROOM "Maximum number of clients reached"

/************
 *   ErrorConnMax
 *     Fail a connection due to lack of client or file descriptor space
 ************/

static void
ErrorConnMax(fd)
    register int fd;
{
    xConnSetupPrefix csp;
    char pad[3];
    struct iovec iov[3];
    char byteOrder = 0;
    int whichbyte = 1;
    struct timeval waittime;
    FdSet mask;

    /* if these seems like a lot of trouble to go to, it probably is */
    waittime.tv_sec = BOTIMEOUT / MILLI_PER_SECOND;
    waittime.tv_usec = (BOTIMEOUT % MILLI_PER_SECOND) *
		       (1000000 / MILLI_PER_SECOND);
    CLEARBITS(mask);
    BITSET(mask, fd);
    (void)select(fd + 1, (int *) mask, (int *) NULL, (int *) NULL, &waittime);
    /* try to read the byte-order of the connection */
    (void)read(fd, &byteOrder, 1);
    if ((byteOrder == 'l') || (byteOrder == 'B'))
    {
	csp.success = xFalse;
	csp.lengthReason = sizeof(NOROOM) - 1;
	csp.length = (sizeof(NOROOM) + 2) >> 2;
	csp.majorVersion = X_PROTOCOL;
	csp.minorVersion = X_PROTOCOL_REVISION;
	if (((*(char *) &whichbyte) && (byteOrder == 'B')) ||
	    (!(*(char *) &whichbyte) && (byteOrder == 'l')))
	{
	    swaps(&csp.majorVersion, whichbyte);
	    swaps(&csp.minorVersion, whichbyte);
	    swaps(&csp.length, whichbyte);
	}
	iov[0].iov_len = sz_xConnSetupPrefix;
	iov[0].iov_base = (char *) &csp;
	iov[1].iov_len = csp.lengthReason;
	iov[1].iov_base = NOROOM;
	iov[2].iov_len = (4 - (csp.lengthReason & 3)) & 3;
	iov[2].iov_base = pad;
	(void)Writev(fd, iov, 3);
    }
}

/************
 *   CloseDownFileDescriptor:
 *     Remove this file descriptor and it's I/O buffers, etc.
 ************/

void
CloseDownFileDescriptor(client)
    ClientPtr	client;
{
    register OsCommPtr oc = (OsCommPtr) client->osPrivate;
    int connection = oc->fd;

    close(connection);
    ConnectionTranslation[connection] = 0;
    ConnectionOutputTranslation[connection] = 0;
    BITCLEAR(AllSockets, connection);
    BITCLEAR(AllClients, connection);
#ifdef LOCALCONN
    BITCLEAR(AllStreams, connection);
#endif
    BITCLEAR(ClientsWithInput, connection);
    BITCLEAR(GrabImperviousClients, connection);
    if (GrabInProgress)
    {
	BITCLEAR(SavedAllSockets, connection);
	BITCLEAR(SavedAllClients, connection);
	BITCLEAR(SavedClientsWithInput, connection);
    }
    BITCLEAR(ClientsWriteBlocked, connection);
    if (!ANYSET(ClientsWriteBlocked))
    	AnyClientsWriteBlocked = FALSE;
    BITCLEAR(OutputPending, connection);
}

/*****************
 * CheckConections
 *    Some connection has died, go find which one and shut it down 
 *    The file descriptor has been closed, but is still in AllClients.
 *    If would truly be wonderful if select() would put the bogus
 *    file descriptors in the exception mask, but nooooo.  So we have
 *    to check each and every socket individually.
 *****************/

void
CheckConnections()
{
    FdMask		mask;
    FdSet		tmask; 
    register int	curclient, curoff;
    int			i;
    struct timeval	notime;
    int r;

    notime.tv_sec = 0;
    notime.tv_usec = 0;

    for (i=0; i<mskcnt; i++)
    {
	mask = AllClients[i];
        while (mask)
    	{
	    curoff = ffs (mask) - 1;
 	    curclient = curoff + (i << 5);
            CLEARBITS(tmask);
            BITSET(tmask, curclient);
            r = select (curclient + 1, (int *)tmask, (int *)NULL, (int *)NULL, 
			&notime);
            if (r < 0)
		CloseDownClient(clients[ConnectionTranslation[curclient]]);
	    mask &= ~(1 << curoff);
	}
    }	
}


/*****************
 * CloseDownConnection
 *    Delete client from AllClients and free resources 
 *****************/

void
CloseDownConnection(client)
    ClientPtr client;
{
    OsCommPtr oc = (OsCommPtr)client->osPrivate;

    if (oc->output && oc->output->count)
	FlushClient(client, oc, (char *)NULL, 0);
    ConnectionTranslation[oc->fd] = 0;
#ifdef XDMCP
    XdmcpCloseDisplay(oc->fd);
#endif
    (*oc->Close) (client);
    FreeOsBuffers(oc);
    xfree(oc);
    client->osPrivate = (pointer)NULL;
    if (auditTrailLevel > 1)
	AuditF("client %d disconnected\n", client->index);
}


AddEnabledDevice(fd)
    int fd;
{
    BITSET(EnabledDevices, fd);
    BITSET(AllSockets, fd);
}


RemoveEnabledDevice(fd)
    int fd;
{
    BITCLEAR(EnabledDevices, fd);
    BITCLEAR(AllSockets, fd);
}

/*****************
 * OnlyListenToOneClient:
 *    Only accept requests from  one client.  Continue to handle new
 *    connections, but don't take any protocol requests from the new
 *    ones.  Note that if GrabInProgress is set, EstablishNewConnections
 *    needs to put new clients into SavedAllSockets and SavedAllClients.
 *    Note also that there is no timeout for this in the protocol.
 *    This routine is "undone" by ListenToAllClients()
 *****************/

OnlyListenToOneClient(client)
    ClientPtr client;
{
    OsCommPtr oc = (OsCommPtr)client->osPrivate;
    int connection = oc->fd;

    if (! GrabInProgress)
    {
	COPYBITS(ClientsWithInput, SavedClientsWithInput);
	MASKANDSETBITS(ClientsWithInput,
		       ClientsWithInput, GrabImperviousClients);
	if (GETBIT(SavedClientsWithInput, connection))
	{
	    BITCLEAR(SavedClientsWithInput, connection);
	    BITSET(ClientsWithInput, connection);
	}
	UNSETBITS(SavedClientsWithInput, GrabImperviousClients);
	COPYBITS(AllSockets, SavedAllSockets);
	COPYBITS(AllClients, SavedAllClients);
	UNSETBITS(AllSockets, AllClients);
	MASKANDSETBITS(AllClients, AllClients, GrabImperviousClients);
	BITSET(AllClients, connection);
	ORBITS(AllSockets, AllSockets, AllClients);
	GrabInProgress = client->index;
    }
}

/****************
 * ListenToAllClients:
 *    Undoes OnlyListentToOneClient()
 ****************/

ListenToAllClients()
{
    if (GrabInProgress)
    {
	ORBITS(AllSockets, AllSockets, SavedAllSockets);
	ORBITS(AllClients, AllClients, SavedAllClients);
	ORBITS(ClientsWithInput, ClientsWithInput, SavedClientsWithInput);
	GrabInProgress = 0;
    }	
}

/****************
 * IgnoreClient
 *    Removes one client from input masks.
 *    Must have cooresponding call to AttendClient.
 ****************/

IgnoreClient (client)
    ClientPtr	client;
{
    OsCommPtr oc = (OsCommPtr)client->osPrivate;
    int connection = oc->fd;

    if (!GrabInProgress || GETBIT(AllClients, connection))
    {
    	if (GETBIT (ClientsWithInput, connection))
	    BITSET(IgnoredClientsWithInput, connection);
    	else
	    BITCLEAR(IgnoredClientsWithInput, connection);
    	BITCLEAR(ClientsWithInput, connection);
    	BITCLEAR(AllSockets, connection);
    	BITCLEAR(AllClients, connection);
	BITCLEAR(LastSelectMask, connection);
    }
    else
    {
    	if (GETBIT (SavedClientsWithInput, connection))
	    BITSET(IgnoredClientsWithInput, connection);
    	else
	    BITCLEAR(IgnoredClientsWithInput, connection);
	BITCLEAR(SavedClientsWithInput, connection);
	BITCLEAR(SavedAllSockets, connection);
	BITCLEAR(SavedAllClients, connection);
    }
    isItTimeToYield = TRUE;
}

/****************
 * AttendClient
 *    Adds one client back into the input masks.
 ****************/

AttendClient (client)
    ClientPtr	client;
{
    OsCommPtr oc = (OsCommPtr)client->osPrivate;
    int connection = oc->fd;

    if (!GrabInProgress || GrabInProgress == client->index ||
	GETBIT(GrabImperviousClients, connection))
    {
    	BITSET(AllClients, connection);
    	BITSET(AllSockets, connection);
	BITSET(LastSelectMask, connection);
    	if (GETBIT (IgnoredClientsWithInput, connection))
	    BITSET(ClientsWithInput, connection);
    }
    else
    {
	BITSET(SavedAllClients, connection);
	BITSET(SavedAllSockets, connection);
	if (GETBIT(IgnoredClientsWithInput, connection))
	    BITSET(SavedClientsWithInput, connection);
    }
}

/* make client impervious to grabs; assume only executing client calls this */

MakeClientGrabImpervious(client)
    ClientPtr client;
{
    OsCommPtr oc = (OsCommPtr)client->osPrivate;
    int connection = oc->fd;

    BITSET(GrabImperviousClients, connection);

    if (ServerGrabCallback)
    {
	ServerGrabInfoRec grabinfo;
	grabinfo.client = client;
	grabinfo.grabstate  = CLIENT_IMPERVIOUS;
	CallCallbacks(&ServerGrabCallback, &grabinfo);
    }
}

/* make client pervious to grabs; assume only executing client calls this */

MakeClientGrabPervious(client)
    ClientPtr client;
{
    OsCommPtr oc = (OsCommPtr)client->osPrivate;
    int connection = oc->fd;

    BITCLEAR(GrabImperviousClients, connection);
    if (GrabInProgress && (GrabInProgress != client->index))
    {
	if (GETBIT(ClientsWithInput, connection))
	{
	    BITSET(SavedClientsWithInput, connection);
	    BITCLEAR(ClientsWithInput, connection);
	}
	BITCLEAR(AllSockets, connection);
	BITCLEAR(AllClients, connection);
	isItTimeToYield = TRUE;
    }

    if (ServerGrabCallback)
    {
	ServerGrabInfoRec grabinfo;
	grabinfo.client = client;
	grabinfo.grabstate  = CLIENT_PERVIOUS;
	CallCallbacks(&ServerGrabCallback, &grabinfo);
    }
}

#ifdef AIXV3

static FdSet pendingActiveClients;
static BOOL reallyGrabbed;

/****************
* DontListenToAnybody:
*   Don't listen to requests from any clients. Continue to handle new
*   connections, but don't take any protocol requests from anybody.
*   We have to take care if there is already a grab in progress, though.
*   Undone by PayAttentionToClientsAgain. We also have to be careful
*   not to accept any more input from the currently dispatched client.
*   we do this be telling dispatch it is time to yield.

*   We call this when the server loses access to the glass
*   (user hot-keys away).  This looks like a grab by the 
*   server itself, but gets a little tricky if there is already
*   a grab in progress.
******************/

void
DontListenToAnybody()
{
    if (!GrabInProgress)
    {
	COPYBITS(ClientsWithInput, SavedClientsWithInput);
	COPYBITS(AllSockets, SavedAllSockets);
	COPYBITS(AllClients, SavedAllClients);
	GrabInProgress = TRUE;
	reallyGrabbed = FALSE;
    }
    else
    {
	COPYBITS(AllClients, pendingActiveClients);
	reallyGrabbed = TRUE;
    }
    CLEARBITS(ClientsWithInput);
    UNSETBITS(AllSockets, AllClients);
    CLEARBITS(AllClients);
    isItTimeToYield = TRUE;
}

void
PayAttentionToClientsAgain()
{
    if (reallyGrabbed)
    {
	ORBITS(AllSockets, AllSockets, pendingActiveClients);
	ORBITS(AllClients, AllClients, pendingActiveClients);
    }
    else
    {
	ListenToAllClients();
    }
    reallyGrabbed = FALSE;
}

#endif
