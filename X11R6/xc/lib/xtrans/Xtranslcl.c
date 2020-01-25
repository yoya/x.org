/* $XConsortium: Xtranslcl.c,v 1.21 95/01/19 18:06:04 mor Exp $ */
/*

Copyright (c) 1993, 1994  X Consortium

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE X CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of the X Consortium shall
not be used in advertising or otherwise to promote the sale, use or
other dealings in this Software without prior written authorization
from the X Consortium.

*/

/* Copyright (c) 1993, 1994 NCR Corporation - Dayton, Ohio, USA
 *
 * All Rights Reserved
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name NCR not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission.  NCR makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * NCR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN
 * NO EVENT SHALL NCR BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/*
 *
 * The connection code/ideas in lib/X and server/os for SVR4/Intel 
 * environments was contributed by the following companies/groups:
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
 * IN NO EVENT SHALL THESE COMPANIES * BE LIABLE FOR ANY SPECIAL, INDIRECT 
 * OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS 
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE 
 * OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE 
 * OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <errno.h>
#include <ctype.h>
#include <sys/signal.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#if 0
#include <sys/ptms.h> /* Maybe for SVR4 only?? */
#endif
#ifdef SVR4
#include <sys/filio.h>
#endif
#include <sys/stropts.h>
#include <sys/wait.h>

/*
 * The local transports should be treated the same as a UNIX domain socket
 * wrt authentication, etc. Because of this, we will use struct sockaddr_un
 * for the address format. This will simplify the code in other places like
 * The X Server.
 */

#include <sys/socket.h>
#include <sys/un.h>


/*
 * These functions actually implement the local connection mechanisms.
 */

/* Type Not Supported */

static int
TRANS(OpenFail)(ciptr, port)

XtransConnInfo	ciptr;
char		*port;

{
    return -1;
}

#ifdef TRANS_REOPEN

static int
TRANS(ReopenFail)(ciptr, fd, port)

XtransConnInfo	ciptr;
int		fd;
char		*port;

{
    return 0;
}

#endif /* TRANS_REOPEN */



static int
TRANS(FillAddrInfo)(ciptr, sun_path, peer_sun_path)

XtransConnInfo	ciptr;
char		*sun_path;
char		*peer_sun_path;

{
    struct sockaddr_un	*sunaddr;
    struct sockaddr_un	*p_sunaddr;

    ciptr->family = AF_UNIX;
    ciptr->addrlen = sizeof (struct sockaddr_un);

    if ((sunaddr = (struct sockaddr_un *) malloc (ciptr->addrlen)) == NULL)
    {
	PRMSG(1,"TRANS(FillAddrInfo)() failed to allocate memory for addr\n",
									0,0,0);
	return 0;
    }

    sunaddr->sun_family = AF_UNIX;

    strcpy (sunaddr->sun_path, sun_path);
#ifdef BSD44SOCKETS
    sunaddr->sun_len = strlen (sunaddr->sun_path);
#endif

    ciptr->addr = (char *) sunaddr;

    ciptr->peeraddrlen = sizeof (struct sockaddr_un);

    if ((p_sunaddr = (struct sockaddr_un *) malloc (
	ciptr->peeraddrlen)) == NULL)
    {
	PRMSG(1,
	   "TRANS(FillAddrInfo)() failed to allocate memory for peer addr\n",
									0,0,0);
	free ((char *) sunaddr);
	ciptr->addr = NULL;

	return 0;
    }

    p_sunaddr->sun_family = AF_UNIX;

    strcpy (p_sunaddr->sun_path, peer_sun_path);
#ifdef BSD44SOCKETS
    p_sunaddr->sun_len = strlen (p_sunaddr->sun_path);
#endif

    ciptr->peeraddr = (char *) p_sunaddr;

    return 1;
}



/* PTS */

#if defined(SYSV) && !defined(SCO)
#define SIGNAL_T int
#else
#define SIGNAL_T void
#endif /* SYSV */

typedef SIGNAL_T (*PFV)();

extern PFV signal();

extern char *ptsname(
#if NeedFunctionPrototypes
    int
#endif
);

static void _dummy(sig)

int sig;

{
}

#define X_STREAMS_DIR	"/dev/X"
#define DEV_PTMX	"/dev/ptmx"
#define DEV_SPX		"/dev/spx"

#if defined(X11_t)

#define PTSNODENAME "/dev/X/server."
#define NAMEDNODENAME "/dev/X/Nserver."

/*
 * ISC and SCO are only defined for X11 since they are there for
 * backwards binary compatability only.
 */

#define X_ISC_DIR	"/dev/X"
#define ISCDEVNODENAME	"/dev/X/ISCCONN/X%s"
#define ISCTMPNODENAME	"/tmp/.X11-unix/X%s"
#define SCORNODENAME	"/dev/X/%1sR"
#define SCOSNODENAME	"/dev/X/%1sS"
#endif
#if defined(XIM_t)
#define PTSNODENAME	"/dev/X/XIM."
#define NAMEDNODENAME	"/dev/X/NXIM."
#endif
#if defined(FS_t) || defined (FONT_t)
/*
 * USL has already defined something here. We need to check with them
 * and see if their choice is usable here.
 */
#define PTSNODENAME	"/dev/X/fontserver."
#define NAMEDNODENAME	"/dev/X/Nfontserver."
#endif
#if defined(ICE_t)
#define PTSNODENAME	"/dev/X/ICE."
#define NAMEDNODENAME	"/dev/X/NICE."
#endif
#if defined(TEST_t)
#define PTSNODENAME	"/dev/X/transtest."
#define NAMEDNODENAME	"/dev/X/Ntranstest."
#endif



#ifdef TRANS_CLIENT

static int
TRANS(PTSOpenClient)(ciptr, port)

XtransConnInfo	ciptr;
char		*port;

{
    int			fd,server,exitval,alarm_time,ret;
    char		server_path[64];
    char		*slave, namelen;
    char		buf[20]; /* MAX_PATH_LEN?? */
    PFV			savef;

    PRMSG(2,"TRANS(PTSOpenClient)(%s)\n", port, 0,0 );

#if !defined(PTSNODENAME)
    PRMSG(1,"Protocol is not supported by a pts connection\n", 0,0,0);
    return -1;
#else
    if (port && *port ) {
	if( *port == '/' ) { /* A full pathname */
		(void) sprintf(server_path, "%s", port);
	    } else {
		(void) sprintf(server_path, "%s%s", PTSNODENAME, port);
	    }
    } else {
	(void) sprintf(server_path, "%s%d", PTSNODENAME, getpid());
    }


    /*
     * Open the node the on which the server is listening.
     */

    if ((server = open (server_path, O_RDWR)) < 0) {
	PRMSG(1,"TRANS(PTSOpenClient)() failed to open %s\n", server_path, 0,0);
	return -1;
    }


    /*
     * Open the streams based pipe that will be this connection.
     */

    if ((fd = open("/dev/ptmx", O_RDWR)) < 0) {
	PRMSG(1,"TRANS(PTSOpenClient)() failed to open /dev/ptmx\n", 0,0,0);
	return -1;
    }

    (void) grantpt(fd);
    (void) unlockpt(fd);

    slave = ptsname(fd); /* get name */

    if( slave == NULL ) {
	PRMSG(1,"TRANS(PTSOpenClient)() failed to get ptsname()\n", 0,0,0);
	close(fd);
	close(server);
	return -1;
    }

    /*
     * This is neccesary for the case where a program is setuid to non-root.
     * grantpt() calls /usr/lib/pt_chmod which is set-uid root. This program will
     * set the owner of the pt device incorrectly if the uid is not restored
     * before it is called. The problem is that once it gets restored, it
     * cannot be changed back to its original condition, hence the fork().
     */

    if( !fork()) {
	uid_t       saved_euid;

	saved_euid = geteuid();
	setuid( getuid() ); /** sets the euid to the actual/real uid **/
	if( chown( slave, saved_euid, -1 ) < 0 ) {
		exit( 1 );
		}

	exit( 0 );
    }

    wait( &exitval );

    if (chmod(slave, 0666) < 0) {
	close(fd);
	close(server);
	PRMSG(1,"Cannot chmod %s\n", slave, 0,0);
	return(-1);
    }

    /*
     * write slave name to server
     */

    namelen = strlen(slave);
    buf[0] = namelen;
    (void) sprintf(&buf[1], slave);
    (void) write(server, buf, namelen+1);
    (void) close(server);

    /*
     * wait for server to respond
     */

    savef = signal(SIGALRM, _dummy);
    alarm_time = alarm (30); /* CONNECT_TIMEOUT */

    ret = read(fd, buf, 1);

    (void) alarm(alarm_time);
    (void) signal(SIGALRM, savef);

    if (ret != 1) {
	PRMSG(1,
	"TRANS(PTSOpenClient)() failed to get acknoledgement from server\n",
									0,0,0);
	(void) close(fd);
	fd = -1;
    }

    /*
     * Everything looks good: fill in the XtransConnInfo structure.
     */

    if (TRANS(FillAddrInfo) (ciptr, slave, server_path) == 0)
    {
	PRMSG(1,"TRANS(PTSOpenClient)() failed to fill in addr info\n",
								0,0,0);
	close(fd);
	return -1;
    }

    return(fd);

#endif /* !PTSNODENAME */
}

#endif /* TRANS_CLIENT */


#ifdef TRANS_SERVER

static int
TRANS(PTSOpenServer)(ciptr, port)

XtransConnInfo	ciptr;
char		*port;

{
    int fd, server;
    char server_path[64], *slave;

    PRMSG(2,"TRANS(PTSOpenServer)(%s)\n", port, 0,0 );

#if !defined(PTSNODENAME)
    PRMSG(1,"Protocol is not supported by a pts connection\n", 0,0,0);
    return -1;
#else
    if (port && *port ) {
	if( *port == '/' ) { /* A full pathname */
		(void) sprintf(server_path, "%s", port);
	    } else {
		(void) sprintf(server_path, "%s%s", PTSNODENAME, port);
	    }
    } else {
	(void) sprintf(server_path, "%s%d", PTSNODENAME, getpid());
    }

    mkdir(X_STREAMS_DIR, 0777);
    chmod(X_STREAMS_DIR, 0777);

    if( (fd=open(server_path, O_RDWR)) >= 0 ) {
	PRMSG(1, "A server is already running on port %s\n", port, 0,0 );
	PRMSG(1, "Remove %s if this is incorrect.\n", server_path, 0,0 );
	close(fd);
	return(-1);
    }

    unlink(server_path);

    if( (fd=open(DEV_PTMX, O_RDWR)) < 0) {
	PRMSG(1, "Unable to open %s\n", DEV_PTMX, 0,0 );
	return(-1);
    }

    grantpt(fd);
    unlockpt(fd);

    if( (slave=ptsname(fd)) == NULL) {
	PRMSG(1, "Unable to get slave device name\n", 0,0,0 );
	close(fd);
	return(-1);
    }

    if( link(slave,server_path) < 0 ) {
	PRMSG(1, "Unable to link %s to %s\n", slave, server_path,0 );
	close(fd);
	return(-1);
    }

    if( chmod(server_path, 0666) < 0 ) {
	PRMSG(1, "Unable to chmod %s to 0666\n", server_path,0,0 );
	close(fd);
	return(-1);
    }

    if( (server=open(server_path, O_RDWR)) < 0 ) {
	PRMSG(1, "Unable to open server device %s\n", server_path,0,0 );
	close(fd);
	return(-1);
    }

    close(server);

    /*
     * Everything looks good: fill in the XtransConnInfo structure.
     */

    if (TRANS(FillAddrInfo) (ciptr, server_path, server_path) == 0)
    {
	PRMSG(1,"TRANS(PTSOpenServer)() failed to fill in addr info\n",
								0,0,0);
	close(fd);
	return -1;
    }

    return fd;

#endif /* !PTSNODENAME */
}

static int
TRANS(PTSAccept)(ciptr, newciptr, status)

XtransConnInfo	ciptr;
XtransConnInfo	newciptr;
int		*status;

{
    int			newfd;
    int			in;
    unsigned char	length;
    char		buf[256];
    struct sockaddr_un	*sunaddr;

    PRMSG(2,"TRANS(PTSAccept)(%x->%d)\n",ciptr,ciptr->fd,0);

    if( (in=read(ciptr->fd,&length,1)) <= 0 ){
	if( !in ) {
		PRMSG(1,
		"TRANS(PTSAccept)() Incoming connection closed\n",0,0,0);
		}
	else {
		PRMSG(1,
	"TRANS(PTSAccept)() Error reading incoming connection. errno=%d \n",
								errno,0,0);
		}
	*status = TRANS_ACCEPT_MISC_ERROR;
	return -1;
    }

    if( (in=read(ciptr->fd,buf,length)) <= 0 ){
	if( !in ) {
		PRMSG(1,
		"TRANS(PTSAccept)() Incoming connection closed\n",0,0,0);
		}
	else {
		PRMSG(1,
"TRANS(PTSAccept)() Error reading device name for new connection. errno=%d \n",
								errno,0,0);
		}
	*status = TRANS_ACCEPT_MISC_ERROR;
	return -1;
    }

    buf[length] = '\0';

    if( (newfd=open(buf,O_RDWR)) < 0 ) {
	PRMSG(1, "TRANS(PTSAccept)() Failed to open %s\n",buf,0,0);
	*status = TRANS_ACCEPT_MISC_ERROR;
	return -1;
    }

    write(newfd,"1",1);

    /*
     * Everything looks good: fill in the XtransConnInfo structure.
     */

    newciptr->addrlen=ciptr->addrlen;
    if( (newciptr->addr=(char *)malloc(newciptr->addrlen)) == NULL ) {
	PRMSG(1,"TRANS(PTSAccept)() failed to allocate memory for peer addr\n",
									0,0,0);
	close(newfd);
	*status = TRANS_ACCEPT_BAD_MALLOC;
	return -1;
    }

    memcpy(newciptr->addr,ciptr->addr,newciptr->addrlen);

    newciptr->peeraddrlen=sizeof(struct sockaddr_un);
    if( (sunaddr=(struct sockaddr_un *)malloc(newciptr->peeraddrlen)) == NULL ) {
	PRMSG(1,"TRANS(PTSAccept)() failed to allocate memory for peer addr\n",
									0,0,0);
	free(newciptr->addr);
	close(newfd);
	*status = TRANS_ACCEPT_BAD_MALLOC;
	return -1;
    }

    sunaddr->sun_family=AF_UNIX;
    strcpy(sunaddr->sun_path,buf);
#ifdef BSD44SOCKETS
    sunaddr->sun_len=strlen(sunaddr->sun_path);
#endif

    newciptr->peeraddr=(char *)sunaddr;

    *status = 0;

    return newfd;
}

#endif /* TRANS_SERVER */


#ifdef SVR4

/* NAMED */

#ifdef TRANS_CLIENT

static int
TRANS(NAMEDOpenClient)(ciptr, port)

XtransConnInfo	ciptr;
char		*port;

{
    int			fd;
    char		server_path[64];
    struct stat		filestat;
    extern int		isastream();

    PRMSG(2,"TRANS(NAMEDOpenClient)(%s)\n", port, 0,0 );

#if !defined(NAMEDNODENAME)
    PRMSG(1,"Protocol is not supported by a NAMED connection\n", 0,0,0);
    return -1;
#else
    if ( port && *port ) {
	if( *port == '/' ) { /* A full pathname */
		(void) sprintf(server_path, "%s", port);
	    } else {
		(void) sprintf(server_path, "%s%s", NAMEDNODENAME, port);
	    }
    } else {
	(void) sprintf(server_path, "%s%d", NAMEDNODENAME, getpid());
    }

    if (stat(server_path, &filestat) < 0 ) {
	PRMSG(1,"No device %s for NAMED connection\n", server_path, 0,0 );
	return -1;
    }

    if ((filestat.st_mode & S_IFMT) != S_IFIFO) {
	PRMSG(1,"Device %s is not a FIFO\n", server_path, 0,0 );
	/* Is this really a failure? */
	return -1;
    }

    if ((fd = open(server_path, O_RDWR)) < 0) {
	PRMSG(1,"Cannot open %s for NAMED connection\n", server_path, 0,0 );
	return -1;
    }

    if (isastream(fd) <= 0) {
	PRMSG(1,"%s is not a streams device\n", server_path, 0,0 );
	(void) close(fd);
	return -1;
    }

    /*
     * Everything looks good: fill in the XtransConnInfo structure.
     */

    if (TRANS(FillAddrInfo) (ciptr, server_path, server_path) == 0)
    {
	PRMSG(1,"TRANS(NAMEDOpenClient)() failed to fill in addr info\n",
								0,0,0);
	close(fd);
	return -1;
    }

    return(fd);

#endif /* !NAMEDNODENAME */
}

#endif /* TRANS_CLIENT */


#ifdef TRANS_SERVER

static int
TRANS(NAMEDOpenServer)(ciptr, port)

XtransConnInfo	ciptr;
char		*port;

{
    int			fd, pipefd[2];
    char		server_path[64];
    struct stat		sbuf;

    PRMSG(2,"TRANS(NAMEDOpenServer)(%s)\n", port, 0,0 );

#if !defined(NAMEDNODENAME)
    PRMSG(1,"Protocol is not supported by a NAMED connection\n", 0,0,0);
    return -1;
#else
    if ( port && *port ) {
	if( *port == '/' ) { /* A full pathname */
	    (void) sprintf(server_path, "%s", port);
	} else {
	    (void) sprintf(server_path, "%s%s", NAMEDNODENAME, port);
	}
    } else {
	(void) sprintf(server_path, "%s%d", NAMEDNODENAME, getpid());
    }

    mkdir(X_STREAMS_DIR, 0777);
    chmod(X_STREAMS_DIR, 0777);

    if(stat(server_path, &sbuf) != 0) {
	if (errno == ENOENT) {
	    if ((fd = creat(server_path, (mode_t)0666)) == -1) {
		PRMSG(1, "Can't open %s\n", server_path, 0,0 );
		return(-1);
	    }
	    close(fd);
	    if (chmod(server_path, (mode_t)0666) < 0) {
		PRMSG(1, "Can't open %s\n", server_path, 0,0 );
		return(-1);
	    }
	} else {
	    PRMSG(1, "stat on %s failed\n", server_path, 0,0 );
	    return(-1);
	}
    }

    if( pipe(pipefd) != 0) {
	PRMSG(1, "pipe() failed, errno=%d\n",errno, 0,0 );
	return(-1);
    }

    if( ioctl(pipefd[0], I_PUSH, "connld") != 0) {
	PRMSG(1, "ioctl(I_PUSH,\"connld\") failed, errno=%d\n",errno, 0,0 );
	close(pipefd[0]);
	close(pipefd[1]);
	return(-1);
    }

    if( fattach(pipefd[0], server_path) != 0) {
	PRMSG(1, "fattach(%s) failed, errno=%d\n", server_path,errno, 0 );
	close(pipefd[0]);
	close(pipefd[1]);
	return(-1);
    }

    /*
     * Everything looks good: fill in the XtransConnInfo structure.
     */

    if (TRANS(FillAddrInfo) (ciptr, server_path, server_path) == 0)
    {
	PRMSG(1,"TRANS(NAMEDOpenServer)() failed to fill in addr info\n",
								0,0,0);
	close(fd);
	return -1;
    }

    return(pipefd[1]);

#endif /* !NAMEDNODENAME */
}

static int
TRANS(NAMEDAccept)(ciptr, newciptr, status)

XtransConnInfo	ciptr;
XtransConnInfo	newciptr;
int		*status;

{
    struct strrecvfd str;

    PRMSG(2,"TRANS(NAMEDAccept)(%x->%d)\n", ciptr, ciptr->fd, 0 );

    if( ioctl(ciptr->fd, I_RECVFD, &str ) < 0 ) {
	PRMSG(1, "ioctl(I_RECVFD) failed, errno=%d\n", errno, 0,0 );
	*status = TRANS_ACCEPT_MISC_ERROR;
	return(-1);
    }

    /*
     * Everything looks good: fill in the XtransConnInfo structure.
     */

    newciptr->addrlen=ciptr->addrlen;
    if( (newciptr->addr=(char *)malloc(newciptr->addrlen)) == NULL ) {
	PRMSG(1,
	      "TRANS(NAMEDAccept)() failed to allocate memory for peer addr\n",
									0,0,0);
	close(str.fd);
	*status = TRANS_ACCEPT_BAD_MALLOC;
	return -1;
    }

    memcpy(newciptr->addr,ciptr->addr,newciptr->addrlen);

    newciptr->peeraddrlen=newciptr->addrlen;
    if( (newciptr->peeraddr=(char *)malloc(newciptr->peeraddrlen)) == NULL ) {
	PRMSG(1,
	"TRANS(NAMEDAccept)() failed to allocate memory for peer addr\n",
									0,0,0);
	free(newciptr->addr);
	close(str.fd);
	*status = TRANS_ACCEPT_BAD_MALLOC;
	return -1;
    }

    memcpy(newciptr->peeraddr,newciptr->addr,newciptr->peeraddrlen);

    *status = 0;

    return str.fd;
}

#endif /* TRANS_SERVER */

#endif /* SVR4 */



/*
 * connect_spipe is used by both the SCO and ISC connection types.
 */
static int
connect_spipe(fd1, fd2)

int	fd1, fd2;

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

    if( ioctl(fd1, I_FDINSERT, &sbuf) < 0 )
	return(-1);

    return(0);
}

/*
 * connect_spipe is used by both the SCO and ISC connection types.
 */

static int
named_spipe(fd, path)

int	fd;
char	*path;

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


/* ISC */

#ifdef TRANS_CLIENT

static int
TRANS(ISCOpenClient)(ciptr, port)

XtransConnInfo	ciptr;
char		*port;

{
    int		fd,fds,server;
    char	server_path[64];
    char	server_dev_path[64];
    struct 	strfdinsert buf;
    long	temp;
    mode_t 	spmode;
    struct stat 	filestat;
    
    PRMSG(2,"TRANS(ISCOpenClient)(%s)\n", port, 0,0 );
    
#if !defined(ISCDEVNODENAME)
    PRMSG(1,"Protocol is not supported by a ISC connection\n", 0,0,0);
    return -1;
#else
    (void) sprintf(server_path, ISCTMPNODENAME, port);
    (void) sprintf(server_dev_path, ISCDEVNODENAME, port);

    fd = fds = server = -1;

    if (stat(DEV_SPX, &filestat) == -1) {
	PRMSG(1, "stat(%s) failed, errno=%d\n", DEV_SPX, errno, 0 );
	return(-1);
    }

    spmode = (filestat.st_mode & S_IFMT);

    if (stat(server_path, &filestat) != -1) {
	if ((filestat.st_mode & S_IFMT) == spmode) {
	    if ((server = open(server_path, O_RDWR)) < 0) {
		PRMSG(1,"TRANS(ISCOpenClient): failed to open %s\n",
		      server_path, 0,0 );
	    }
	}
    }
    
    if (server < 0) {
	/* try the alternate path */
	if (stat(server_dev_path, &filestat) != -1) {
	    if ((filestat.st_mode & S_IFMT) == spmode) {
		if ((server = open(server_dev_path, O_RDWR)) < 0) {
		    PRMSG(1,"TRANS(ISCOpenClient): failed to open %s\n",
			  server_dev_path, 0,0 );
		}
	    }
	}
    }
    
    if (server < 0) {
	PRMSG(1,"TRANS(ISCOpenClient): can't open either device %s or %s\n",
	      server_path, server_dev_path, 0 );
	return -1;
    }

    if ((fds = open(DEV_SPX, O_RDWR)) < 0 ||
	(fd  = open(DEV_SPX, O_RDWR)) < 0) {
	/* Failed to open all of the devices */
	PRMSG(1,"TRANS(ISCOpenClient): can't open %s\n", DEV_SPX, 0,0 );
	(void) close(server);
	if (fds != -1)
	    (void) close(fds);
	if (fd != -1)
	    (void) close(fd);
	return -1;
    }

    /* make a STREAMS-pipe */

    buf.databuf.maxlen = -1;
    buf.databuf.len = -1;
    buf.databuf.buf = NULL;
    buf.ctlbuf.maxlen = sizeof(long);
    buf.ctlbuf.len = sizeof(long);
    buf.ctlbuf.buf = (caddr_t)&temp;
    buf.offset = 0;
    buf.fildes = fd;
    buf.flags = 0;
    
    if (ioctl(fds, I_FDINSERT, &buf) < 0 ||
	ioctl(server, I_SENDFD, fds) < 0) {
	PRMSG(1,"TRANS(ISCOpenClient): ioctl(I_FDINSERT or I_SENDFD) failed\n",
								0,0,0 );
	(void) close(server);
	(void) close(fds);
	(void) close(fd);
	return -1;
    }

    /*
     * Everything looks good: fill in the XtransConnInfo structure.
     */
    
    if (TRANS(FillAddrInfo) (ciptr, server_path, server_path) == 0)
    {
	PRMSG(1,"TRANS(ISCOpenClient)() failed to fill in addr info\n",
								0,0,0);
	close(fd);
	return -1;
    }

    return (fd);

#endif /* !ISCDEVNODENAME */
}

#endif /* TRANS_CLIENT */


#ifdef TRANS_SERVER

static int
TRANS(ISCOpenServer)(ciptr, port)

XtransConnInfo	ciptr;
char		*port;

{
    int	fd = -1,fds = -1;
    char	server_path[64],server_unix_path[64];
    
    PRMSG(2,"TRANS(ISCOpenServer)(%s)\n", port, 0,0 );
    
#if !defined(ISCDEVNODENAME)
    PRMSG(1,"Protocol is not supported by a ISC connection\n", 0,0,0);
    return -1;
#else
    (void) sprintf(server_path, ISCDEVNODENAME, port);
    (void) sprintf(server_unix_path, ISCTMPNODENAME, port);
    
    mkdir(X_STREAMS_DIR, 0777); /* "/dev/X" */
    chmod(X_STREAMS_DIR, 0777);
    mkdir(X_ISC_DIR, 0777); /* "/dev/X/ISCCONN" */
    chmod(X_ISC_DIR, 0777);
    
    unlink(server_path);
    
    if( ((fds=open(DEV_SPX, O_RDWR)) < 0) ||
       ((fd =open(DEV_SPX, O_RDWR)) < 0)) {
	PRMSG(1,"TRANS(ISCOpenServer): failed to open %s\n", DEV_SPX, 0,0 );
	return -1;
    }
    
    if( (connect_spipe(fds, fd) < 0) ||
       (named_spipe(fds, server_path) < 0)) {
	PRMSG(1,"TRANS(ISCOpenServer): failed connect pipes\n", 0,0,0 );
	close(fd);
	close(fds);
	return -1;
    }
    
#if !defined(UNIXCONN)
    /*
     * If the UNIX Domain socket transport is not being used, then link this
     * device to the path /tmp/.X11-unix/X path.
     */
#define X_UNIX_DIR	"/tmp/.X11-unix"
    
    if (!mkdir(X_UNIX_DIR, 0777))
	chmod(X_UNIX_DIR, 0777);
    
    unlink(server_unix_path);
    
#ifdef SVR4
    /* we prefer symbolic links because hard links can't cross file systems */
    if( symlink(server_path, server_unix_path) < 0 )
	PRMSG(1,"TRANS(ISCOpenServer): failed to link %s to %s\n",
	      server_path, server_unix_path, 0 );
    /*
     * Don't make this failure fatal since the listener 
     * is already established, and this just for compatability
     */
#else
    if( link(server_path, server_unix_path) < 0 )
	PRMSG(1,"TRANS(ISCOpenServer): failed to link %s to %s\n",
	      server_path, server_unix_path, 0 );
    /*
     * Don't make this failure fatal since the listener 
     * is already established, and this just for compatability
     */
#endif /* SVR4 */
#endif /* !UNIXCONN */
    
    /*
     * Everything looks good: fill in the XtransConnInfo structure.
     */
    
    if (TRANS(FillAddrInfo) (ciptr, server_path, server_path) == 0)
    {
	PRMSG(1,"TRANS(ISCOpenServer)() failed to fill in addr info\n",
								0,0,0);
	close(fd);
	return -1;
    }

    return fd;

#endif /* !ISCDEVNODENAME */
}

static int
TRANS(ISCAccept)(ciptr, newciptr, status)

XtransConnInfo	ciptr;
XtransConnInfo	newciptr;
int		*status;

{
    struct strrecvfd str;
    
    PRMSG(2,"TRANS(ISCAccept)(%d)\n", ciptr->fd, 0,0 );
    
    while (ioctl(ciptr->fd, I_RECVFD, &str) < 0) {
	if (errno != EAGAIN) {
	    PRMSG(1,"TRANS(ISCAccept): Can't read fildes", 0,0,0 );
	    *status = TRANS_ACCEPT_MISC_ERROR;
	    return(-1);
	}
    }
    
    /*
     * Everything looks good: fill in the XtransConnInfo structure.
     */
    
    newciptr->addrlen=ciptr->addrlen;
    if( (newciptr->addr=(char *)malloc(newciptr->addrlen)) == NULL ) {
	PRMSG(1,
	      "TRANS(ISCAccept)() failed to allocate memory for peer addr\n",
	      0,0,0);
	close(str.fd);
	*status = TRANS_ACCEPT_BAD_MALLOC;
	return -1;
    }
    
    memcpy(newciptr->addr,ciptr->addr,newciptr->addrlen);
    
    newciptr->peeraddrlen=newciptr->addrlen;
    if( (newciptr->peeraddr=(char *)malloc(newciptr->peeraddrlen)) == NULL ) {
	PRMSG(1,
	      "TRANS(ISCAccept)() failed to allocate memory for peer addr\n",
	      0,0,0);
	free(newciptr->addr);
	close(str.fd);
	*status = TRANS_ACCEPT_BAD_MALLOC;
	return -1;
    }
    
    memcpy(newciptr->peeraddr,newciptr->addr,newciptr->peeraddrlen);
    
    *status = 0;

    return(str.fd);
}

#endif /* TRANS_SERVER */




/* SCO */

#ifdef TRANS_CLIENT

static int
TRANS(SCOOpenClient)(ciptr, port)

XtransConnInfo	ciptr;
char		*port;

{
    int			fd, server, fl, ret;
    char		server_path[64];
    struct strbuf	ctlbuf;
    unsigned long	alarm_time;
    void		(*savef)();
    long		temp;
    extern int	getmsg(), putmsg();
    
    PRMSG(2,"TRANS(SCOOpenClient)(%s)\n", port, 0,0 );
    
#if !defined(SCORNODENAME)
    PRMSG(1,"Protocol is not supported by a SCO connection\n", 0,0,0);
    return -1;
#else
    (void) sprintf(server_path, SCORNODENAME, port);
    
    if ((server = open(server_path, O_RDWR)) < 0) {
	PRMSG(1,"TRANS(SCOOpenClient) failed to open %s\n", server_path, 0,0 );
	return -1;
    }
    
    if ((fd = open(DEV_SPX, O_RDWR)) >= 0) {
	PRMSG(1,"TRANS(SCOOpenClient) failed to open %s\n", DEV_SPX, 0,0 );
	close(server);
	return -1;
    }
    
    (void) write(server, &server, 1);
    ctlbuf.len = 0;
    ctlbuf.maxlen = sizeof(long);
    ctlbuf.buf = (caddr_t)&temp;
    fl = 0;
    
    savef = signal(SIGALRM, _dummy);
    alarm_time = alarm(10);
    
    ret = getmsg(server, &ctlbuf, 0, &fl);
    
    (void) alarm(alarm_time);
    (void) signal(SIGALRM, savef);
    
    if (ret < 0) {
	PRMSG(1,"TRANS(SCOOpenClient) error from getmsg\n", 0,0,0 );
	close(fd);
	close(server);
	return -1;
    }
    
    /* The msg we got via getmsg is the result of an
     * I_FDINSERT, so if we do a putmsg with whatever
     * we recieved, we're doing another I_FDINSERT ...
     */
    (void) putmsg(fd, &ctlbuf, 0, 0);
    (void) fcntl(fd,F_SETFL,fcntl(fd,F_GETFL,0)|O_NDELAY);
    
    (void) close(server);
    
    /*
     * Everything looks good: fill in the XtransConnInfo structure.
     */
    
    if (TRANS(FillAddrInfo) (ciptr, server_path, server_path) == 0)
    {
	PRMSG(1,"TRANS(SCOOpenClient)() failed to fill addr info\n",
	      0,0,0);
	close(fd);
	return -1;
    }
    
    return(fd);

#endif  /* !SCORNODENAME */
}

#endif /* TRANS_CLIENT */


#ifdef TRANS_SERVER

static int
TRANS(SCOOpenServer)(ciptr, port)

XtransConnInfo	ciptr;
char		*port;

{
    char		serverR_path[64];
    char		serverS_path[64];
    int	fdr = -1;
    int	fds = -1;
    
    PRMSG(2,"TRANS(SCOOpenServer)(%s)\n", port, 0,0 );
    
#if !defined(SCORNODENAME)
    PRMSG(1,"Protocol is not supported by a SCO connection\n", 0,0,0);
    return -1;
#else
    (void) sprintf(serverR_path, SCORNODENAME, port);
    (void) sprintf(serverS_path, SCOSNODENAME, port);
    
    unlink(serverR_path);
    unlink(serverS_path);
    
    if ((fds = open(DEV_SPX, O_RDWR)) < 0 ||
	(fdr = open(DEV_SPX, O_RDWR)) < 0 ) {
	PRMSG(2,"TRANS(SCOOpenServer) failed to open %s\n", DEV_SPX, 0,0 );
	return -1;
    }
    
    if (connect_spipe(fds, fdr) != -1 &&
	named_spipe(fds, serverS_path) != -1 &&
	named_spipe(fdr, serverR_path) != -1) {
	PRMSG(2,"TRANS(SCOOpenServer) connect pipes\n", 0,0,0 );
	} else {
	PRMSG(2,"TRANS(SCOOpenServer) failed to connect pipes\n", 0,0,0 );
	close(fds);
	close(fdr);
	return -1;
    }
    
    /*
     * Everything looks good: fill in the XtransConnInfo structure.
     */
    
    if (TRANS(FillAddrInfo) (ciptr, serverS_path, serverR_path) == 0)
    {
	PRMSG(1,"TRANS(SCOOpenServer)() failed to fill in addr info\n",
	      0,0,0);
	close(fds);
	close(fdr);
	return -1;
    }
    
    return(fds);

#endif /* !SCORNODENAME */
}

static int
TRANS(SCOAccept)(ciptr, newciptr, status)

XtransConnInfo	ciptr;
XtransConnInfo	newciptr;
int		*status;

{
    char	c;
    int	fd;
    
    PRMSG(2,"TRANS(SCOAccept)(%d)\n", ciptr->fd, 0,0 );
    
    if (read(ciptr->fd, &c, 1) < 0) {
	PRMSG(1,"TRANS(SCOAccept): can't read from client",0,0,0);
	*status = TRANS_ACCEPT_MISC_ERROR;
	return(-1);
    }
    
    if( (fd = open(DEV_SPX, O_RDWR)) < 0 ) {
	PRMSG(1,"TRANS(SCOAccept)(): can't open \"%s\"",DEV_SPX, 0,0 );
	*status = TRANS_ACCEPT_MISC_ERROR;
	return(-1);
    }
    
    if (connect_spipe(ciptr->fd, fd) < 0) {
	PRMSG(1,"TRANS(SCOAccept)(): can't connect pipes", 0,0,0 );
	(void) close(fd);
	*status = TRANS_ACCEPT_MISC_ERROR;
	return(-1);
    }
    
    /*
     * Everything looks good: fill in the XtransConnInfo structure.
     */
    
    newciptr->addrlen=ciptr->addrlen;
    if( (newciptr->addr=(char *)malloc(newciptr->addrlen)) == NULL ) {
	PRMSG(1,
	      "TRANS(SCOAccept)() failed to allocate memory for peer addr\n",
	      0,0,0);
	close(fd);
	*status = TRANS_ACCEPT_BAD_MALLOC;
	return -1;
    }
    
    memcpy(newciptr->addr,ciptr->addr,newciptr->addrlen);
    
    newciptr->peeraddrlen=newciptr->addrlen;
    if( (newciptr->peeraddr=(char *)malloc(newciptr->peeraddrlen)) == NULL ) {
	PRMSG(1,
	      "TRANS(SCOAccept)() failed to allocate memory for peer addr\n",
	      0,0,0);
	free(newciptr->addr);
	close(fd);
	*status = TRANS_ACCEPT_BAD_MALLOC;
	return -1;
    }
    
    memcpy(newciptr->peeraddr,newciptr->addr,newciptr->peeraddrlen);
    
    *status = 0;

    return(fd);
}

#endif /* TRANS_SERVER */



#ifdef TRANS_REOPEN

static int
TRANS(PTSReopenServer)(ciptr, fd, port)

XtransConnInfo	ciptr;
int		fd;
char		*port;
{
    char server_path[64];

    PRMSG(2,"TRANS(PTSReopenServer)(%d,%s)\n", fd, port, 0 );

#if !defined(PTSNODENAME)
    PRMSG(1,"Protocol is not supported by a pts connection\n", 0,0,0);
    return 0;
#else
    if (port && *port ) {
	if( *port == '/' ) { /* A full pathname */
		(void) sprintf(server_path, "%s", port);
	    } else {
		(void) sprintf(server_path, "%s%s", PTSNODENAME, port);
	    }
    } else {
	(void) sprintf(server_path, "%s%d", PTSNODENAME, getpid());
    }

    if (TRANS(FillAddrInfo) (ciptr, server_path, server_path) == 0)
    {
	PRMSG(1,"TRANS(PTSReopenServer)() failed to fill in addr info\n",
								0,0,0);
	return 0;
    }

    return 1;

#endif /* !PTSNODENAME */
}


static int
TRANS(NAMEDReopenServer)(ciptr, fd, port)

XtransConnInfo	ciptr;
int		fd;
char		*port;

{
    char server_path[64];

    PRMSG(2,"TRANS(NAMEDReopenServer)(%s)\n", port, 0,0 );

#if !defined(NAMEDNODENAME)
    PRMSG(1,"Protocol is not supported by a NAMED connection\n", 0,0,0);
    return 0;
#else
    if ( port && *port ) {
	if( *port == '/' ) { /* A full pathname */
	    (void) sprintf(server_path, "%s", port);
	} else {
	    (void) sprintf(server_path, "%s%s", NAMEDNODENAME, port);
	}
    } else {
	(void) sprintf(server_path, "%s%d", NAMEDNODENAME, getpid());
    }

    if (TRANS(FillAddrInfo) (ciptr, server_path, server_path) == 0)
    {
	PRMSG(1,"TRANS(NAMEDReopenServer)() failed to fill in addr info\n",
								0,0,0);
	return 0;
    }

    return 1;

#endif /* !NAMEDNODENAME */
}


static int
TRANS(ISCReopenServer)(ciptr, fd, port)

XtransConnInfo	ciptr;
int		fd;
char		*port;

{
    char server_path[64],server_unix_path[64];
    
    PRMSG(2,"TRANS(ISCReopenServer)(%s)\n", port, 0,0 );
    
#if !defined(ISCDEVNODENAME)
    PRMSG(1,"Protocol is not supported by a ISC connection\n", 0,0,0);
    return 0;
#else
    (void) sprintf(server_path, ISCDEVNODENAME, port);
    (void) sprintf(server_unix_path, ISCTMPNODENAME, port);

    if (TRANS(FillAddrInfo) (ciptr, server_path, server_path) == 0)
    {
	PRMSG(1,"TRANS(ISCReopenServer)() failed to fill in addr info\n",
								0,0,0);
	return 0;
    }
    
    return 1;

#endif /* !ISCDEVNODENAME */
}


static int
TRANS(SCOReopenServer)(ciptr, fd, port)

XtransConnInfo	ciptr;
int		fd;
char		*port;

{
    char		serverR_path[64];
    char		serverS_path[64];
    
    PRMSG(2,"TRANS(SCOReopenServer)(%s)\n", port, 0,0 );
    
#if !defined(SCORNODENAME)
    PRMSG(1,"Protocol is not supported by a SCO connection\n", 0,0,0);
    return 0;
#else
    (void) sprintf(serverR_path, SCORNODENAME, port);
    (void) sprintf(serverS_path, SCOSNODENAME, port);
    
    if (TRANS(FillAddrInfo) (ciptr, serverS_path, serverR_path) == 0)
    {
	PRMSG(1,"TRANS(SCOReopenServer)() failed to fill in addr info\n",
	      0,0,0);
	return 0;
    }
    
    return 1;

#endif /* SCORNODENAME */
}

#endif /* TRANS_REOPEN */



/*
 * This table contains all of the entry points for the different local
 * connection mechanisms.
 */

typedef struct _LOCALtrans2dev {
    char	*transname;

#ifdef TRANS_CLIENT

    int	(*devcotsopenclient)(
#if NeedFunctionPrototypes
	XtransConnInfo, char * /*port*/
#endif
);

#endif /* TRANS_CLIENT */

#ifdef TRANS_SERVER

    int	(*devcotsopenserver)(
#if NeedFunctionPrototypes
	XtransConnInfo, char * /*port*/
#endif
);

#endif /* TRANS_SERVER */

#ifdef TRANS_CLIENT

    int	(*devcltsopenclient)(
#if NeedFunctionPrototypes
	XtransConnInfo, char * /*port*/
#endif
);

#endif /* TRANS_CLIENT */

#ifdef TRANS_SERVER

    int	(*devcltsopenserver)(
#if NeedFunctionPrototypes
	XtransConnInfo, char * /*port*/
#endif
);

#endif /* TRANS_SERVER */

#ifdef TRANS_REOPEN

    int	(*devcotsreopenserver)(
#if NeedFunctionPrototypes
	XtransConnInfo,
	int, 	/* fd */
	char * 	/* port */
#endif
);

    int	(*devcltsreopenserver)(
#if NeedFunctionPrototypes
	XtransConnInfo,
	int, 	/* fd */
	char *	/* port */
#endif
);

#endif /* TRANS_REOPEN */

#ifdef TRANS_SERVER

    int	(*devaccept)(
#if NeedFunctionPrototypes
	XtransConnInfo, XtransConnInfo, int *
#endif

);

#endif /* TRANS_SERVER */

} LOCALtrans2dev;

static LOCALtrans2dev LOCALtrans2devtab[] = {
{"",
#ifdef TRANS_CLIENT
     TRANS(PTSOpenClient),
#endif /* TRANS_CLIENT */
#ifdef TRANS_SERVER
     TRANS(PTSOpenServer),
#endif /* TRANS_SERVER */
#ifdef TRANS_CLIENT
     TRANS(OpenFail),
#endif /* TRANS_CLIENT */
#ifdef TRANS_SERVER
     TRANS(OpenFail),
#endif /* TRANS_SERVER */
#ifdef TRANS_REOPEN
     TRANS(PTSReopenServer),
     TRANS(ReopenFail),
#endif
#ifdef TRANS_SERVER
     TRANS(PTSAccept)
#endif /* TRANS_SERVER */
},

{"local",
#ifdef TRANS_CLIENT
     TRANS(PTSOpenClient),
#endif /* TRANS_CLIENT */
#ifdef TRANS_SERVER
     TRANS(PTSOpenServer),
#endif /* TRANS_SERVER */
#ifdef TRANS_CLIENT
     TRANS(OpenFail),
#endif /* TRANS_CLIENT */
#ifdef TRANS_SERVER
     TRANS(OpenFail),
#endif /* TRANS_SERVER */
#ifdef TRANS_REOPEN
     TRANS(PTSReopenServer),
     TRANS(ReopenFail),
#endif
#ifdef TRANS_SERVER
     TRANS(PTSAccept)
#endif /* TRANS_SERVER */
},

{"pts",
#ifdef TRANS_CLIENT
     TRANS(PTSOpenClient),
#endif /* TRANS_CLIENT */
#ifdef TRANS_SERVER
     TRANS(PTSOpenServer),
#endif /* TRANS_SERVER */
#ifdef TRANS_CLIENT
     TRANS(OpenFail),
#endif /* TRANS_CLIENT */
#ifdef TRANS_SERVER
     TRANS(OpenFail),
#endif /* TRANS_SERVER */
#ifdef TRANS_REOPEN
     TRANS(PTSReopenServer),
     TRANS(ReopenFail),
#endif
#ifdef TRANS_SERVER
     TRANS(PTSAccept)
#endif /* TRANS_SERVER */
},

#ifdef SVR4
{"named",
#ifdef TRANS_CLIENT
     TRANS(NAMEDOpenClient),
#endif /* TRANS_CLIENT */
#ifdef TRANS_SERVER
     TRANS(NAMEDOpenServer),
#endif /* TRANS_SERVER */
#ifdef TRANS_CLIENT
     TRANS(OpenFail),
#endif /* TRANS_CLIENT */
#ifdef TRANS_SERVER
     TRANS(OpenFail),
#endif /* TRANS_SERVER */
#ifdef TRANS_REOPEN
     TRANS(NAMEDReopenServer),
     TRANS(ReopenFail),
#endif
#ifdef TRANS_SERVER
     TRANS(NAMEDAccept)
#endif /* TRANS_SERVER */
},
#endif /* SVR4 */

{"isc",
#ifdef TRANS_CLIENT
     TRANS(ISCOpenClient),
#endif /* TRANS_CLIENT */
#ifdef TRANS_SERVER
     TRANS(ISCOpenServer),
#endif /* TRANS_SERVER */
#ifdef TRANS_CLIENT
     TRANS(OpenFail),
#endif /* TRANS_CLIENT */
#ifdef TRANS_SERVER
     TRANS(OpenFail),
#endif /* TRANS_SERVER */
#ifdef TRANS_REOPEN
     TRANS(ISCReopenServer),
     TRANS(ReopenFail),
#endif
#ifdef TRANS_SERVER
     TRANS(ISCAccept)
#endif /* TRANS_SERVER */
},

{"sco",
#ifdef TRANS_CLIENT
     TRANS(SCOOpenClient),
#endif /* TRANS_CLIENT */
#ifdef TRANS_SERVER
     TRANS(SCOOpenServer),
#endif /* TRANS_SERVER */
#ifdef TRANS_CLIENT
     TRANS(OpenFail),
#endif /* TRANS_CLIENT */
#ifdef TRANS_SERVER
     TRANS(OpenFail),
#endif /* TRANS_SERVER */
#ifdef TRANS_REOPEN
     TRANS(SCOReopenServer),
     TRANS(ReopenFail),
#endif
#ifdef TRANS_SERVER
     TRANS(SCOAccept)
#endif /* TRANS_SERVER */
},
};

#define NUMTRANSPORTS	(sizeof(LOCALtrans2devtab)/sizeof(LOCALtrans2dev))
    
static	char	*XLOCAL=NULL;
static	char	*workingXLOCAL=NULL;
static	char	*freeXLOCAL=NULL;

static void
TRANS(LocalInitTransports)(protocol)

char *protocol;

{
    PRMSG(3,"TRANS(LocalInitTransports)(%s)\n", protocol, 0,0 );
    
    if( strcmp(protocol,"local") && strcmp(protocol,"LOCAL") )
    {
	workingXLOCAL=freeXLOCAL=(char *)malloc (strlen (protocol) + 1);
	if (workingXLOCAL)
	    strcpy (workingXLOCAL, protocol);
    }
    else {
	XLOCAL=(char *)getenv("XLOCAL");
	if(XLOCAL==NULL)
	    XLOCAL="UNIX:PTS:NAMED:ISC:SCO";
	workingXLOCAL=freeXLOCAL=(char *)malloc (strlen (XLOCAL) + 1);
	if (workingXLOCAL)
	    strcpy (workingXLOCAL, XLOCAL);
    }
}

static void
TRANS(LocalEndTransports)()

{
    PRMSG(3,"TRANS(LocalEndTransports)()\n", 0,0,0 );
    free(freeXLOCAL);
}

static LOCALtrans2dev *
TRANS(LocalGetNextTransport)()

{
    int	i,j;
    char	*typetocheck;
#define TYPEBUFSIZE	32
    char	typebuf[TYPEBUFSIZE];
    PRMSG(3,"TRANS(LocalGetNextTransport)()\n", 0,0,0 );
    
    while(1)
    {
	if( workingXLOCAL == NULL || *workingXLOCAL == '\0' )
	    return NULL;
	
	typetocheck=workingXLOCAL;
	workingXLOCAL=strchr(workingXLOCAL,':');
	if(workingXLOCAL && *workingXLOCAL)
	    *workingXLOCAL++='\0';
	
	for(i=0;i<NUMTRANSPORTS;i++)
	{
	    /*
	     * This is equivilent to a case insensitive strcmp(),
	     * but should be more portable.
	     */
	    strncpy(typebuf,typetocheck,TYPEBUFSIZE);
	    for(j=0;j<TYPEBUFSIZE;j++)
		if (isupper(typebuf[j]))
		    typebuf[j]=tolower(typebuf[j]);
	    
	    /* Now, see if they match */
	    if(!strcmp(LOCALtrans2devtab[i].transname,typebuf))
		return &LOCALtrans2devtab[i];
	}
    }
    /*NOTREACHED*/
    return NULL;
}

#ifdef TRANS_CLIENT

static XtransConnInfo
TRANS(LocalOpenClient)(type, protocol, host, port)

int  type;
char *protocol;
char *host;
char *port;

{
    int	fd = -1;
    LOCALtrans2dev *transptr;
    XtransConnInfo ciptr;
    int index;
    
    PRMSG(3,"TRANS(LocalOpenClient)()\n", 0,0,0 );
    
#if defined(X11_t)
    /*
     * X has a well known port, that is transport dependant. It is easier
     * to handle it here, than try and come up with a transport independent
     * representation that can be passed in and resolved the usual way.
     *
     * The port that is passed here is really a string containing the idisplay
     * from ConnectDisplay(). Since that is what we want for the local transports,
     * we don't have to do anything special.
     */
#endif /* X11_t */
    
    if( (ciptr=(XtransConnInfo)calloc(1,sizeof(struct _XtransConnInfo))) == NULL )
    {
	PRMSG(1,"TRANS(LocalOpenClient)() calloc(1,%d) failed\n",
	      sizeof(struct _XtransConnInfo),0,0 );
	return NULL;
    }
    
    ciptr->fd = -1;
    
    TRANS(LocalInitTransports)(protocol);
    
    index = 0;
    for(transptr=TRANS(LocalGetNextTransport)();
	transptr!=NULL;transptr=TRANS(LocalGetNextTransport)(), index++)
    {
	switch( type )
	{
	case XTRANS_OPEN_COTS_CLIENT:
	    ciptr->fd=transptr->devcotsopenclient(ciptr,port);
	    break;
	case XTRANS_OPEN_CLTS_CLIENT:
	    ciptr->fd=transptr->devcltsopenclient(ciptr,port);
	    break;
	case XTRANS_OPEN_COTS_SERVER:
	case XTRANS_OPEN_CLTS_SERVER:
	    PRMSG(1,
		  "TRANS(LocalOpenClient): Should not be opening a server with this function\n",
		  0,0,0);
	    break;
	default:
	    PRMSG(1,
		  "TRANS(LocalOpenClient): Unknown Open type %d\n",
		  type, 0,0 );
	}
	if( ciptr->fd >= 0 )
	    break;
    }
    
    TRANS(LocalEndTransports)();
    
    if( ciptr->fd < 0 )
    {
	free(ciptr);
	return NULL;
    }
    
    ciptr->priv=(char *)transptr;
    ciptr->index = index;
    
    return ciptr;
}

#endif /* TRANS_CLIENT */


#ifdef TRANS_SERVER

static XtransConnInfo
TRANS(LocalOpenServer)(type, protocol, host, port)

int  type;
char *protocol;
char *host;
char *port;

{
    int	i,fd = -1;
    XtransConnInfo ciptr;
    
    PRMSG(2,"TRANS(LocalOpenServer)(%d,%s,%s)\n", type, protocol, port);
    
#if defined(X11_t)
    /*
     * For X11, the port will be in the format xserverN where N is the
     * display number. All of the local connections just need to know
     * the display number because they don't do any name resolution on
     * the port. This just truncates port to the display portion.
     */
#endif /* X11_t */
    
    if( (ciptr=(XtransConnInfo)calloc(1,sizeof(struct _XtransConnInfo))) == NULL )
    {
	PRMSG(1,"TRANS(LocalOpenServer)() calloc(1,%d) failed\n",
	      sizeof(struct _XtransConnInfo),0,0 );
	return NULL;
    }
    
    for(i=1;i<NUMTRANSPORTS;i++)
    {
	if( strcmp(protocol,LOCALtrans2devtab[i].transname) != 0 )
	    continue;
	switch( type )
	{
	case XTRANS_OPEN_COTS_CLIENT:
	case XTRANS_OPEN_CLTS_CLIENT:
	    PRMSG(1,
		  "TRANS(LocalOpenServer): Should not be opening a client with this function\n",
		  0,0,0);
	    break;
	case XTRANS_OPEN_COTS_SERVER:
	    ciptr->fd=LOCALtrans2devtab[i].devcotsopenserver(ciptr,port);
	    break;
	case XTRANS_OPEN_CLTS_SERVER:
	    ciptr->fd=LOCALtrans2devtab[i].devcltsopenserver(ciptr,port);
	    break;
	default:
	    PRMSG(1,"TRANS(LocalOpenServer): Unknown Open type %d\n",
		  type ,0,0);
	}
	if( ciptr->fd >= 0 ) {
	    ciptr->priv=(char *)&LOCALtrans2devtab[i];
	    ciptr->index=i;
	    ciptr->flags=1;
	    return ciptr;
	}
    }
    
    free(ciptr);
    return NULL;
}

#endif /* TRANS_SERVER */


#ifdef TRANS_REOPEN

static XtransConnInfo
TRANS(LocalReopenServer)(type, index, fd, port)

int  type;
int  index;
int  fd;
char *port;

{
    XtransConnInfo ciptr;
    int stat;
    
    PRMSG(2,"TRANS(LocalReopenServer)(%d,%d,%d)\n", type, index, fd);
    
    if( (ciptr=(XtransConnInfo)calloc(1,sizeof(struct _XtransConnInfo))) == NULL )
    {
	PRMSG(1,"TRANS(LocalReopenServer)() calloc(1,%d) failed\n",
	      sizeof(struct _XtransConnInfo),0,0 );
	return NULL;
    }
    
    ciptr->fd = fd;

    switch( type )
    {
    case XTRANS_OPEN_COTS_SERVER:
	stat = LOCALtrans2devtab[index].devcotsreopenserver(ciptr,fd,port);
	break;
    case XTRANS_OPEN_CLTS_SERVER:
	stat = LOCALtrans2devtab[index].devcltsreopenserver(ciptr,fd,port);
	break;
    default:
	PRMSG(1,"TRANS(LocalReopenServer): Unknown Open type %d\n",
	  type ,0,0);
    }

    if( stat > 0 ) {
	ciptr->priv=(char *)&LOCALtrans2devtab[index];
	ciptr->index=index;
	ciptr->flags=1;
	return ciptr;
    }
    
    free(ciptr);
    return NULL;
}

#endif /* TRANS_REOPEN */



/*
 * This is the Local implementation of the X Transport service layer
 */

#ifdef TRANS_CLIENT

static XtransConnInfo
TRANS(LocalOpenCOTSClient)(thistrans, protocol, host, port)

Xtransport *thistrans;
char *protocol;
char *host;
char *port;

{
    PRMSG(2,"TRANS(LocalOpenCOTSClient)(%s,%s,%s)\n",protocol,host,port);
    
    return TRANS(LocalOpenClient)(XTRANS_OPEN_COTS_CLIENT, protocol, host, port);
}

#endif /* TRANS_CLIENT */


#ifdef TRANS_SERVER

static XtransConnInfo
TRANS(LocalOpenCOTSServer)(thistrans, protocol, host, port)

Xtransport *thistrans;
char *protocol;
char *host;
char *port;

{
    PRMSG(2,"TRANS(LocalOpenCOTSServer)(%s,%s,%s)\n",protocol,host,port);
    
    return TRANS(LocalOpenServer)(XTRANS_OPEN_COTS_SERVER, protocol, host, port);
}

#endif /* TRANS_SERVER */


#ifdef TRANS_CLIENT

static XtransConnInfo
TRANS(LocalOpenCLTSClient)(thistrans, protocol, host, port)

Xtransport *thistrans;
char *protocol;
char *host;
char *port;

{
    PRMSG(2,"TRANS(LocalOpenCLTSClient)(%s,%s,%s)\n",protocol,host,port);
    
    return TRANS(LocalOpenClient)(XTRANS_OPEN_CLTS_CLIENT, protocol, host, port);
}

#endif /* TRANS_CLIENT */


#ifdef TRANS_SERVER

static XtransConnInfo
TRANS(LocalOpenCLTSServer)(thistrans, protocol, host, port)

Xtransport *thistrans;
char *protocol;
char *host;
char *port;

{
    PRMSG(2,"TRANS(LocalOpenCLTSServer)(%s,%s,%s)\n",protocol,host,port);
    
    return TRANS(LocalOpenServer)(XTRANS_OPEN_CLTS_SERVER, protocol, host, port);
}

#endif /* TRANS_SERVER */


#ifdef TRANS_REOPEN

static XtransConnInfo
TRANS(LocalReopenCOTSServer)(thistrans, fd, port)

Xtransport *thistrans;
int  	   fd;
char	   *port;

{
    int index;

    PRMSG(2,"TRANS(LocalReopenCOTSServer)(%d,%s)\n", fd, port, 0);
    
    for(index=1;index<NUMTRANSPORTS;index++)
    {
	if( strcmp(thistrans->TransName,
	    LOCALtrans2devtab[index].transname) == 0 )
	    break;
    }
    
    if (index >= NUMTRANSPORTS)
    {
	return (NULL);
    }

    return TRANS(LocalReopenServer)(XTRANS_OPEN_COTS_SERVER,
	index, fd, port);
}

static XtransConnInfo
TRANS(LocalReopenCLTSServer)(thistrans, fd, port)

Xtransport *thistrans;
int  	   fd;
char	   *port;

{
    int index;

    PRMSG(2,"TRANS(LocalReopenCLTSServer)(%d,%s)\n", fd, port, 0);
    
    for(index=1;index<NUMTRANSPORTS;index++)
    {
	if( strcmp(thistrans->TransName,
	    LOCALtrans2devtab[index].transname) == 0 )
	    break;
    }
    
    if (index >= NUMTRANSPORTS)
    {
	return (NULL);
    }

    return TRANS(LocalReopenServer)(XTRANS_OPEN_CLTS_SERVER,
	index, fd, port);
}

#endif /* TRANS_REOPEN */



static
TRANS(LocalSetOption)(ciptr, option, arg)

XtransConnInfo ciptr;
int option;
int arg;

{
    PRMSG(2,"TRANS(LocalSetOption)(%d,%d,%d)\n",ciptr->fd,option,arg);
    
    return -1;
}


#ifdef TRANS_SERVER

static
TRANS(LocalCreateListener)(ciptr, port)

XtransConnInfo ciptr;
char *port;

{
    PRMSG(2,"TRANS(LocalCreateListener)(%x->%d,%s)\n",ciptr,ciptr->fd,port);
    
    return 0;
}

static XtransConnInfo
TRANS(LocalAccept)(ciptr, status)

XtransConnInfo ciptr;
int	       *status;

{
    XtransConnInfo	newciptr;
    LOCALtrans2dev	*transptr;
    
    PRMSG(2,"TRANS(LocalAccept)(%x->%d)\n", ciptr, ciptr->fd,0);
    
    transptr=(LOCALtrans2dev *)ciptr->priv;
    
    if( (newciptr=(XtransConnInfo)calloc(1,sizeof(struct _XtransConnInfo)))==NULL )
    {
	PRMSG(1,"TRANS(LocalAccept)() calloc(1,%d) failed\n",
	      sizeof(struct _XtransConnInfo),0,0 );
	*status = TRANS_ACCEPT_BAD_MALLOC;
	return NULL;
    }
    
    newciptr->fd=transptr->devaccept(ciptr,newciptr,status);
    
    if( newciptr->fd < 0 )
    {
	free(newciptr);
	return NULL;
    }
    
    newciptr->priv=(char *)transptr;
    newciptr->index = ciptr->index;
    
    *status = 0;

    return newciptr;
}

#endif /* TRANS_SERVER */


#ifdef TRANS_CLIENT

static
TRANS(LocalConnect)(ciptr, host, port)

XtransConnInfo ciptr;
char *host;
char *port;

{
    PRMSG(2,"TRANS(LocalConnect)(%x->%d,%s)\n", ciptr, ciptr->fd, port);
    
    return 0;
}

#endif /* TRANS_CLIENT */


static int
TRANS(LocalBytesReadable)(ciptr, pend )

XtransConnInfo ciptr;
BytesReadable_t *pend;

{
    PRMSG(2,"TRANS(LocalBytesReadable)(%x->%d,%x)\n", ciptr, ciptr->fd, pend);
    
#ifdef SCO
    return ioctl(ciptr->fd, I_NREAD, (char *)pend);
#else
    return ioctl(ciptr->fd, FIONREAD, (char *)pend);
#endif
}

static int
TRANS(LocalRead)(ciptr, buf, size)

XtransConnInfo ciptr;
char *buf;
int size;

{
    PRMSG(2,"TRANS(LocalRead)(%d,%x,%d)\n", ciptr->fd, buf, size );
    
    return read(ciptr->fd,buf,size);
}

static int
TRANS(LocalWrite)(ciptr, buf, size)

XtransConnInfo ciptr;
char *buf;
int size;

{
    PRMSG(2,"TRANS(LocalWrite)(%d,%x,%d)\n", ciptr->fd, buf, size );
    
    return write(ciptr->fd,buf,size);
}

static int
TRANS(LocalReadv)(ciptr, buf, size)

XtransConnInfo 	ciptr;
struct iovec 	*buf;
int 		size;

{
    PRMSG(2,"TRANS(LocalReadv)(%d,%x,%d)\n", ciptr->fd, buf, size );
    
    return READV(ciptr,buf,size);
}

static int
TRANS(LocalWritev)(ciptr, buf, size)

XtransConnInfo 	ciptr;
struct iovec 	*buf;
int 		size;

{
    PRMSG(2,"TRANS(LocalWritev)(%d,%x,%d)\n", ciptr->fd, buf, size );
    
    return WRITEV(ciptr,buf,size);
}

static int
TRANS(LocalDisconnect)(ciptr)

XtransConnInfo ciptr;

{
    PRMSG(2,"TRANS(LocalDisconnect)(%x->%d)\n", ciptr, ciptr->fd, 0);
    
    return 0;
}

static int
TRANS(LocalClose)(ciptr)

XtransConnInfo ciptr;

{
    struct sockaddr_un      *sockname=(struct sockaddr_un *) ciptr->addr;
    char    path[200]; /* > sizeof sun_path +1 */
    int	ret;
    
    PRMSG(2,"TRANS(LocalClose)(%x->%d)\n", ciptr, ciptr->fd ,0);
    
    ret=close(ciptr->fd);
    
    if(ciptr->flags
       && sockname
       && sockname->sun_family == AF_UNIX
       && sockname->sun_path[0] )
    {
	strncpy(path,sockname->sun_path,
		ciptr->addrlen-sizeof(sockname->sun_family));
	unlink(path);
    }
    
    return ret;
}

static int
TRANS(LocalCloseForCloning)(ciptr)

XtransConnInfo ciptr;

{
    int ret;

    PRMSG(2,"TRANS(LocalCloseForCloning)(%x->%d)\n", ciptr, ciptr->fd ,0);
    
    /* Don't unlink path */

    ret=close(ciptr->fd);
    
    return ret;
}


/*
 * MakeAllCOTSServerListeners() will go through the entire Xtransports[]
 * array defined in Xtrans.c and try to OpenCOTSServer() for each entry.
 * We will add duplicate entries to that table so that the OpenCOTSServer()
 * function will get called once for each type of local transport. 
 *
 * The TransName is in lowercase, so it will never match during a normal
 * call to SelectTransport() in Xtrans.c.
 */

Xtransport	TRANS(LocalFuncs) = {
	/* Local Interface */
	"local",
	TRANS_ALIAS | TRANS_LOCAL,
#ifdef TRANS_CLIENT
	TRANS(LocalOpenCOTSClient),
#endif /* TRANS_CLIENT */
#ifdef TRANS_SERVER
	TRANS(LocalOpenCOTSServer),
#endif /* TRANS_SERVER */
#ifdef TRANS_CLIENT
	TRANS(LocalOpenCLTSClient),
#endif /* TRANS_CLIENT */
#ifdef TRANS_SERVER
	TRANS(LocalOpenCLTSServer),
#endif /* TRANS_SERVER */
#ifdef TRANS_REOPEN
	TRANS(LocalReopenCOTSServer),
	TRANS(LocalReopenCLTSServer),
#endif
	TRANS(LocalSetOption),
#ifdef TRANS_SERVER
	TRANS(LocalCreateListener),
	NULL,					/* ResetListener */
	TRANS(LocalAccept),
#endif /* TRANS_SERVER */
#ifdef TRANS_CLIENT
	TRANS(LocalConnect),
#endif /* TRANS_CLIENT */
	TRANS(LocalBytesReadable),
	TRANS(LocalRead),
	TRANS(LocalWrite),
	TRANS(LocalReadv),
	TRANS(LocalWritev),
	TRANS(LocalDisconnect),
	TRANS(LocalClose),
	TRANS(LocalCloseForCloning),
};

Xtransport	TRANS(PTSFuncs) = {
	/* Local Interface */
	"pts",
	TRANS_LOCAL,
#ifdef TRANS_CLIENT
	TRANS(LocalOpenCOTSClient),
#endif /* TRANS_CLIENT */
#ifdef TRANS_SERVER
	TRANS(LocalOpenCOTSServer),
#endif /* TRANS_SERVER */
#ifdef TRANS_CLIENT
	TRANS(LocalOpenCLTSClient),
#endif /* TRANS_CLIENT */
#ifdef TRANS_SERVER
	TRANS(LocalOpenCLTSServer),
#endif /* TRANS_SERVER */
#ifdef TRANS_REOPEN
	TRANS(LocalReopenCOTSServer),
	TRANS(LocalReopenCLTSServer),
#endif
	TRANS(LocalSetOption),
#ifdef TRANS_SERVER
	TRANS(LocalCreateListener),
	NULL,					/* ResetListener */
	TRANS(LocalAccept),
#endif /* TRANS_SERVER */
#ifdef TRANS_CLIENT
	TRANS(LocalConnect),
#endif /* TRANS_CLIENT */
	TRANS(LocalBytesReadable),
	TRANS(LocalRead),
	TRANS(LocalWrite),
	TRANS(LocalReadv),
	TRANS(LocalWritev),
	TRANS(LocalDisconnect),
	TRANS(LocalClose),
	TRANS(LocalCloseForCloning),
};

Xtransport	TRANS(NAMEDFuncs) = {
	/* Local Interface */
	"named",
	TRANS_LOCAL,
#ifdef TRANS_CLIENT
	TRANS(LocalOpenCOTSClient),
#endif /* TRANS_CLIENT */
#ifdef TRANS_SERVER
	TRANS(LocalOpenCOTSServer),
#endif /* TRANS_SERVER */
#ifdef TRANS_CLIENT
	TRANS(LocalOpenCLTSClient),
#endif /* TRANS_CLIENT */
#ifdef TRANS_SERVER
	TRANS(LocalOpenCLTSServer),
#endif /* TRANS_SERVER */
#ifdef TRANS_REOPEN
	TRANS(LocalReopenCOTSServer),
	TRANS(LocalReopenCLTSServer),
#endif
	TRANS(LocalSetOption),
#ifdef TRANS_SERVER
	TRANS(LocalCreateListener),
	NULL,					/* ResetListener */
	TRANS(LocalAccept),
#endif /* TRANS_SERVER */
#ifdef TRANS_CLIENT
	TRANS(LocalConnect),
#endif /* TRANS_CLIENT */
	TRANS(LocalBytesReadable),
	TRANS(LocalRead),
	TRANS(LocalWrite),
	TRANS(LocalReadv),
	TRANS(LocalWritev),
	TRANS(LocalDisconnect),
	TRANS(LocalClose),
	TRANS(LocalCloseForCloning),
};

Xtransport	TRANS(ISCFuncs) = {
	/* Local Interface */
	"isc",
	TRANS_LOCAL,
#ifdef TRANS_CLIENT
	TRANS(LocalOpenCOTSClient),
#endif /* TRANS_CLIENT */
#ifdef TRANS_SERVER
	TRANS(LocalOpenCOTSServer),
#endif /* TRANS_SERVER */
#ifdef TRANS_CLIENT
	TRANS(LocalOpenCLTSClient),
#endif /* TRANS_CLIENT */
#ifdef TRANS_SERVER
	TRANS(LocalOpenCLTSServer),
#endif /* TRANS_SERVER */
#ifdef TRANS_REOPEN
	TRANS(LocalReopenCOTSServer),
	TRANS(LocalReopenCLTSServer),
#endif
	TRANS(LocalSetOption),
#ifdef TRANS_SERVER
	TRANS(LocalCreateListener),
	NULL,					/* ResetListener */
	TRANS(LocalAccept),
#endif /* TRANS_SERVER */
#ifdef TRANS_CLIENT
	TRANS(LocalConnect),
#endif /* TRANS_CLIENT */
	TRANS(LocalBytesReadable),
	TRANS(LocalRead),
	TRANS(LocalWrite),
	TRANS(LocalReadv),
	TRANS(LocalWritev),
	TRANS(LocalDisconnect),
	TRANS(LocalClose),
	TRANS(LocalCloseForCloning),
};
Xtransport	TRANS(SCOFuncs) = {
	/* Local Interface */
	"sco",
	TRANS_LOCAL,
#ifdef TRANS_CLIENT
	TRANS(LocalOpenCOTSClient),
#endif /* TRANS_CLIENT */
#ifdef TRANS_SERVER
	TRANS(LocalOpenCOTSServer),
#endif /* TRANS_SERVER */
#ifdef TRANS_CLIENT
	TRANS(LocalOpenCLTSClient),
#endif /* TRANS_CLIENT */
#ifdef TRANS_SERVER
	TRANS(LocalOpenCLTSServer),
#endif /* TRANS_SERVER */
#ifdef TRANS_REOPEN
	TRANS(LocalReopenCOTSServer),
	TRANS(LocalReopenCLTSServer),
#endif
	TRANS(LocalSetOption),
#ifdef TRANS_SERVER
	TRANS(LocalCreateListener),
	NULL,					/* ResetListener */
	TRANS(LocalAccept),
#endif /* TRANS_SERVER */
#ifdef TRANS_CLIENT
	TRANS(LocalConnect),
#endif /* TRANS_CLIENT */
	TRANS(LocalBytesReadable),
	TRANS(LocalRead),
	TRANS(LocalWrite),
	TRANS(LocalReadv),
	TRANS(LocalWritev),
	TRANS(LocalDisconnect),
	TRANS(LocalClose),
	TRANS(LocalCloseForCloning),
};
