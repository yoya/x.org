/* @(#)XimSocket.c      6.1 94/06/01 20:04:28 FUJITSU LIMITED. */
/*

Copyright (c) 1992, 1993, 1994  FUJITSU LIMITED

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
IN NO EVENT SHALL THE FUJITSU LIMITED BE LIABLE FOR ANY CLAIM, DAMAGES
OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of the FUJITSU LIMITED shall
not be used in advertising or otherwise to promote the sale, use or
other dealings in this Software without prior written authorization
from the FUJITSU LIMITED.

  Author: Takashi Fujiwara     FUJITSU LIMITED 
                               fujiwara@a80.tech.yk.fujitsu.co.jp
          Riki Kawaguchi       FUJITSU LIMITED
	  Toshimitu Terazono   FUJITSU LIMITED

*/

/*
 *  xfeoak : Input Server (kana-kanji covertion IM Server)
 */

#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include "Xfeoak.h"

#ifdef SVR4
#if !defined(hpux)
#include <sys/stropts.h>
#endif /* !defined(hpux) */
#include <unistd.h>
#include <sys/filio.h>
#else /* SVR4 */
#include <sys/ioctl.h>
#endif /* SVR4 */

#ifdef __sxg__
#include <sys/utsname.h>
#endif

typedef struct  _Xfe_Tcp_TransRec {
    BITMASK16            flag;
    CARD16               port_number;
    int                  fd;
    char                 host_name[64];
    struct hostent      *hostent;
    struct servent      *servent;
} Xfe_Tcp_TransRec;

/* flag of Xfe_Tcp_TransRec */
#define XFE_TCP_READY           (1 << 0)

#define	XFE_INTERNAL_DOMAIN_PATH	"/tmp/.xfeoak"
#define XIM_SERVICE_NAME	"xim"
#define MAX_LISTEN_NO		10
#define SELECT_TIMEOUT_SEC	0
#define SELECT_TIMEOUT_USEC	0

Static void
TcpDestroyConnect()
{
    if (current_connect && current_connect->fd) {
	shutdown(current_connect->fd, 2);
	(void)close(current_connect->fd);
	FD_CLR(current_connect->fd, &server_fd_mask);
	if (server_max_fd == current_connect->fd) {
	    server_max_fd -= 1;
	}
    }
    return;
}

Static Bool
TcpNewConnect(transP)
    Xfe_TransRec	*transP;
{
    int			 arg = 1;
    int			 fd;
    FXim_Connect	*xim_connect;
    Xfe_Tcp_TransRec	*trans = (Xfe_Tcp_TransRec *)transP->trans_data;

    if (!trans || !(trans->flag & XFE_TCP_READY))
	return False;

    if ((fd = accept(trans->fd, 0, 0)) < 0) {
	if (errno != EWOULDBLOCK)
	    return False;
	return True;
    }

    if (ioctl(fd, FIONBIO, &arg) < 0)
	return False;

    if (!(xim_connect = NewConnect()))
	return False;

    xim_connect->trans = transP;
    xim_connect->recv  = (Xfe_Recv)0;
    xim_connect->fd    = fd;

    FD_SET(fd, &server_fd_mask);
    if (server_max_fd < fd)
	server_max_fd = fd;

    return True;
}

Static void
TcpCheckClient(xim_connect)
    FXim_Connect        *xim_connect;
{
    Xfe_Recv		 rec;
    int			 errcd;
    int			 ret;
    char		 check;

    if ((ret = recv(xim_connect->fd, &check, 1, MSG_PEEK)) < 0) {
	if (errno != EWOULDBLOCK) {
	    TcpDestroyConnect();
	    DeleteConnect(xim_connect);
	}
	return;
    }

    current_connect = xim_connect;
    if (!(rec = (*xim_connect->trans->method.read_client)(xim_connect,
								&errcd))) {
	if (errcd) {
	    TcpDestroyConnect();
	    DeleteConnect(xim_connect);
	}
	return;
    }

    (void)XimEvent(rec);
    return;
}

Static void
TcpEventProc(transP)
    Xfe_TransRec	*transP;
{
    fd_set		 rd_fds;
    struct timeval       timeout;
    Xfe_Tcp_TransRec	*trans = (Xfe_Tcp_TransRec *)transP->trans_data;
    register int	 i;
    register int	 n;

    timeout.tv_sec  = SELECT_TIMEOUT_SEC;
    timeout.tv_usec = SELECT_TIMEOUT_USEC;
    FD_ZERO(&rd_fds);
    memcpy((char *)&rd_fds, (char *)&server_fd_mask, sizeof(fd_set));
    if ((n = select(server_max_fd + 1, &rd_fds,
		 (fd_set *)0, (fd_set *)0, &timeout) < 0)) {
	return;
    }
    for (i = 0; i < (server_max_fd + 1); i++) {
	if (!(FD_ISSET(i, &rd_fds)))
	    continue;

        if (i == trans->fd) {
	    TcpNewConnect(transP);
        } else {
	    FXim_Connect	*xim_connect;

	    if (!(xim_connect = FindConnectByFd(i)))
		continue;
	    TcpCheckClient(xim_connect);
	}
	n--;
	if (n == 0)
	    break;
    }
    return;
}

Static int
TcpReadCore(fd, read_area, read_length)
    int			 fd;
    char		*read_area;
    int			 read_length;
{
    int			 more_bytes = read_length;
    int			 read_bytes;
    int			 errnum;

    while (more_bytes > 0) {
	if ((read_bytes = read(fd, read_area, more_bytes)) < 0) {
	    errnum = errno;
	    if (errnum == EWOULDBLOCK || errnum == EINPROGRESS)
		continue;
	    else
		return -1;
	}
	if (!read_bytes)
	    return -1;
	more_bytes -= read_bytes;
	read_area  += read_bytes;
    }

    return (read_length - more_bytes);
}

Static int
TcpMoreRead(fd, buf, len)
    int		 fd;
    char	*buf;
    int		*len;
{
    int		 size;
    char	 byte_order;

    if (TcpReadCore(fd, buf, XIM_HEADER_SIZE) < XIM_HEADER_SIZE) {
	return -1;
    }
    if (buf[0] == XIM_CONNECT) {
	if (TcpReadCore(fd, &byte_order, 1) < 1) {
	    return -1;
	}
	current_connect->byte_order = byte_order;
	buf[XIM_HEADER_SIZE] = byte_order;
	XimSetEndian();

	*len = XIM_HEADER_SIZE  + 1;
	size = (*((CARD16 *)&buf[2]) * 4);
	size = XfeCard16(size) - 1;
    } else {
	*len = XIM_HEADER_SIZE;
	size = (*((CARD16 *)&buf[2]) * 4);
	size = XfeCard16(size);
    }

    return size;
}

Static Xfe_Recv
TcpReadClient(xim_connect, errcd)
    FXim_Connect	*xim_connect;
    int			*errcd;
{
    int			 read_size;
    char		*buf;
    Xfe_Recv		 rec;
    char		 tmp[2048];
    int			 len;

    *errcd = 0;

    if ((read_size = TcpMoreRead(xim_connect->fd, tmp, &len)) < 0) {
	*errcd = -1;
	return (Xfe_Recv)NULL;
    }

    if (!len) {
	*errcd = -1;
	return (Xfe_Recv)NULL;
    }

    if (!(buf = (char *)malloc(read_size + len))) {
	*errcd = -1;
	return (Xfe_Recv)NULL;
    }
    bzero(buf, read_size + len);
    memcpy(buf, tmp, len);

    if (TcpReadCore(xim_connect->fd, &buf[len], read_size) < read_size) {
	free(buf);
	*errcd = -1;
	return (Xfe_Recv)NULL;
    }

    rec = XimprotoNewRecv((XPointer)buf, read_size + len);
    return rec;
}

Static void
TcpFlush(fd)
    int		fd;
{
    return;
}

Static int
TcpWrite(area, len)
    unsigned char	*area;
    int			 len;
{
    int			 more = len;
    int			 wbytes = 0;
    int			 bytes;
    int			 errnum;

    while (more > 0) {
	if ((bytes = write(current_connect->fd, (area + wbytes), more)) < 0) {
	    errnum = errno;
	    if (errnum == EWOULDBLOCK || errnum == EINPROGRESS)
		continue;
	    else
		break;
	}
	more   -= bytes;
	wbytes += bytes;
    }
    return wbytes;
}

Static Bool
TcpSendClient(buf, len)
    XPointer		 buf;
    INT16		 len;
{
    int			 length;

    if (TcpWrite((unsigned char *)buf, (int)len) < len)
	return False;
    return True;
}

Static Bool
GetHostName(array, size)
    char		*array;
    int			 size;
{
#ifdef __sxg__
    struct utsname	 name;

    if ((size < sizeof(name.nodename)) || (uname(&name) < 0))
	return False;

    (void)strcpy(array, name.nodename);
    return True;
#else
    if (gethostname(array, size) < 0)
	return False;
    return True;
#endif
}

Static void
RegTcpFunctions(transP)
    Xfe_TransRec	*transP;
{
    transP->method.event_proc	= TcpEventProc;
    transP->method.read_client	= TcpReadClient;
    transP->method.send_client	= TcpSendClient;
    transP->method.flush	= TcpFlush;
    transP->method.disconnect	= TcpDestroyConnect;
    return;
}

Bool
InitialTcp(transP)
    Xfe_TransRec	*transP;
{
    Xfe_Tcp_TransRec	*trans;
    struct sockaddr_in	 sa;
    int			 arg = 1;

    if (!(trans = (Xfe_Tcp_TransRec *)malloc(sizeof(Xfe_Tcp_TransRec))))
	return False;
    bzero(trans, sizeof(Xfe_Tcp_TransRec));

    if (!(GetHostName(trans->host_name, sizeof(trans->host_name)))) {
	free(trans);
	return False;
    }

    bzero(&sa, sizeof(sa));
    if (!(trans->hostent = gethostbyname(trans->host_name))) {
	free(trans);
	return False;
    }

#ifndef __sxg__
    if (trans->servent = getservbyname(XIM_SERVICE_NAME, "tcp"))
	trans->port_number = trans->servent->s_port;
    else
#endif
	trans->port_number = 6100; /* FIX ME !! */

    if ((trans->fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
	free(trans);
	return False;
    }

#ifdef BSD44SOCKETS
    sa.sin_len = sizeof (sa);
#endif
    sa.sin_family = trans->hostent->h_addrtype;
    sa.sin_port = trans->port_number;
    sa.sin_port = htons(sa.sin_port);
    memcpy((char *) &sa.sin_addr,
		 (char *)trans->hostent->h_addr, trans->hostent->h_length);

    if (bind(trans->fd, (struct sockaddr *)&sa, sizeof(sa)) < 0) {
	(void)close(trans->fd);
	free(trans);
	return False;
    }

    if (listen(trans->fd, MAX_LISTEN_NO) < 0) {
	(void)close(trans->fd);
	free(trans);
	return False;
    }

    if (ioctl(trans->fd, FIONBIO, &arg) < 0) {
	(void)close(trans->fd);
	free(trans);
	return False;
    }

    FD_SET(trans->fd, &server_fd_mask);
    if (server_max_fd < trans->fd)
	server_max_fd = trans->fd;
    RegTcpFunctions(transP);
    trans->flag |= XFE_TCP_READY;
    transP->trans_data = (XPointer)trans;
    return True;
}

Bool
InitialInternal(transP)
    Xfe_TransRec	*transP;
{
    Xfe_Tcp_TransRec	*trans;
    struct sockaddr_un	 sa;
    int			 arg = 1;

    if (!(trans = (Xfe_Tcp_TransRec *)malloc(sizeof(Xfe_Tcp_TransRec))))
	return False;
    bzero(trans, sizeof(Xfe_Tcp_TransRec));

    if (!(GetHostName(trans->host_name, sizeof(trans->host_name)))) {
	free(trans);
	return False;
    }

    (void)unlink(XFE_INTERNAL_DOMAIN_PATH);
    if ((trans->fd = socket(AF_UNIX, SOCK_STREAM, 0)) <0) {
	free(trans);
	return False;
    }

    sa.sun_family = AF_UNIX;
    strcpy(sa.sun_path, XFE_INTERNAL_DOMAIN_PATH);

    if (bind(trans->fd, (struct sockaddr *)&sa,
			(strlen(sa.sun_path) + sizeof(sa.sun_family))) < 0) {
	(void)close(trans->fd);
	free(trans);
	return False;
    }

    if (listen(trans->fd, MAX_LISTEN_NO) < 0) {
	(void)close(trans->fd);
	free(trans);
	return False;
    }

    if (ioctl(trans->fd, FIONBIO, &arg) < 0) {
	(void)close(trans->fd);
	free(trans);
	return False;
    }

    FD_SET(trans->fd, &server_fd_mask);
    if (server_max_fd < trans->fd)
	server_max_fd = trans->fd;
    RegTcpFunctions(transP);
    trans->flag |= XFE_TCP_READY;
    transP->trans_data = (XPointer)trans;;
    return True;
}

void
ExitTcp(transP)
    Xfe_TransRec	*transP;
{
    Xfe_Tcp_TransRec	*trans = (Xfe_Tcp_TransRec *)transP->trans_data;

    if (trans && trans->flag & XFE_TCP_READY) {
	shutdown(trans->fd, 2);
	(void)close(trans->fd);
	free(trans);
    }
    return;
}

void
ExitInternal(transP)
    Xfe_TransRec	*transP;
{
    Xfe_Tcp_TransRec	*trans = (Xfe_Tcp_TransRec *)transP->trans_data;

    if (trans && trans->flag & XFE_TCP_READY) {
	shutdown(trans->fd, 2);
	(void)close(trans->fd);
	(void)unlink(XFE_INTERNAL_DOMAIN_PATH);
	free(trans);
    }
    return;
}

Bool
GetTcpName(transP, array)
    Xfe_TransRec	*transP;
    char		*array;
{
    Xfe_Tcp_TransRec	*trans = (Xfe_Tcp_TransRec *)transP->trans_data;

    sprintf(array, "tcp/%s:%d", trans->host_name, trans->port_number);
    return True;
}

Bool
GetInternalName(transP, array)
    Xfe_TransRec	*transP;
    char		*array;
{
    Xfe_Tcp_TransRec	*trans = (Xfe_Tcp_TransRec *)transP->trans_data;

    sprintf(array, "local/%s:%s", trans->host_name, XFE_INTERNAL_DOMAIN_PATH);
    return True;
}
