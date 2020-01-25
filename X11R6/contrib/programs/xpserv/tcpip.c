/*
Copyright (c) 1994  Matthew Reynolds and Qualcomm Inc.

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
QUALCOMM INC., OR MATTHEW REYNOLDS BE LIABLE FOR ANY CLAIM, 
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE 
OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of Qualcomm Inc., or Matthew 
Reynolds shall not be used in advertising or otherwise to promote the sale, 
use or other dealings in this Software without their prior written 
authorization.

*/

#include <sys/types.h>
#include <sys/uio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "tcpip.h"

/* create a socket for accepting connections on and listen on it.  If the
   port_no is 0, then create a socket using INADDR_ANY for the port number */

int create_accept_socket( int	port_no)
{
    int                 conn_sock, length, opt = 1;
    struct sockaddr_in  sin;
    char                hostname[ 128];
    struct hostent      *hp;

/* create a socket file desciptor */

    conn_sock = socket( AF_INET, SOCK_STREAM, 0);

/* create the structure to bind it to for a well-known address */

    memset(( char *) &sin, 0, sizeof( sin));
    sin.sin_family = AF_INET;
    gethostname( hostname, sizeof( hostname));
    hp = gethostbyname( hostname);
    memcpy(( void *) &sin.sin_addr, ( void *) hp->h_addr, hp->h_length);

/* if the requested port number is 0, then we generate a port number */

    sin.sin_port = ( port_no) ? htons( port_no) : INADDR_ANY;

/* bind the socket to its address */

    if ( bind( conn_sock, ( struct sockaddr *) &sin, sizeof( sin)) < 0) {
        perror( "bind failed\n");
        return( -1);
    }

/* make sure the name was bound properly and listen on it for connections */

    length = sizeof( sin);
    if ( getsockname( conn_sock, ( struct sockaddr *) &sin, &length) < 0) {
        perror( "bad address for socket");
        return( -1);
    }
    if ( listen( conn_sock, BACKLOG) < 0) {
        perror( "listen failed");
        return( -1);
    }
    return( conn_sock);
}

/* accept a connection from a socket */

int accept_connection( int accept_fd)
{
    struct sockaddr	addr;
    int			addrlen = sizeof( struct sockaddr);

    return( accept( accept_fd, &addr, &addrlen));
}

/* get the port number of the socket */

int get_port_no( int	socket)
{
    struct sockaddr_in	name;
    int			length = sizeof( name);

    if ( getsockname( socket, &name, &length) < 0) {
	perror( "getsockname failed");
	return( -1);
    }
    return(( int) name.sin_port);
}

/* create a socket and use it to connect to another host */

int create_connect_socket( int	port,
			   char	*hostname)
{
    struct sockaddr_in  sin;
    struct hostent      *hp;
    int			conn_sock;

/* set up the socket for making connections on */

    conn_sock = socket( AF_INET, SOCK_STREAM, 0);
    memset(( char *) &sin, 0, sizeof( sin));
    sin.sin_family = AF_INET;
    hp = gethostbyname( hostname);
    memcpy(( void *) &sin.sin_addr, ( void *) hp->h_addr, hp->h_length);
    sin.sin_port = htons( port);

/* connect and bind the socket to its address */

    if ( connect( conn_sock, ( struct sockaddr *) &sin, sizeof( sin)) < 0) {
        perror( "connect failed");
        return( -1);
    }
    return( conn_sock);
}
