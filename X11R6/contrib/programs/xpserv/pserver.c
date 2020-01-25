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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#define NEED_REPLIES		/* because Xproto.h is stupid */
#include <X11/Xproto.h>
#include <X11/Xlib.h>
#include "tcpip.h"
#include "printer.h"

#define SERVBASE		6000
#define MAXREQSZ		262140
#define RESOURCE_ID_MASK	0x3FFFFF
#define CLIENTOFFSET		22
#define VENBYTES		16
#define STATICMAXFD		32

Display	*THE_DISPLAY;
struct PrintClient	PCS[ STATICMAXFD];
char **fontlist;

typedef union xmsg_ {
    xReq		req;
    unsigned char	buf[ MAXREQSZ];
} xmsg_u;

typedef union xreply_ {
    xGenericReply	reply;
    unsigned char	buf[ MAXREQSZ];
} xreply_u;

int debug = 1;

print_options_s	PRINT_OPTIONS; 

void main( int argc, char *argv[])
{
    int 		port, accept_fd, client_fd, reply_len, highest_fd, 
			client_flag, i, c;
    xmsg_u		message; 
    xreply_u		reply;
    fd_set		readfds, writefds, exceptfds, readmask;
    struct sigaction	act, oact;
    char		*disp_name;
    FILE		*fp;

/* options:
	-p <port #>			port # for server 
	-d <display-name>		name of real display to connect to 
	-g 				debug flag on/off

   set the defaults:
*/

    debug = FALSE;
    port = SERVBASE + 1;
    disp_name = getenv( "DISPLAY");
    if ( disp_name == NULL) {
	disp_name = "unix:0.0";
    }

/* parse the arguments */

    while (( c = getopt( argc, argv, "gp:d:")) != EOF) {
	switch (c) {
	case 'g':
	    debug = TRUE;
	    break;
	case 'p':
	    port = atoi( optarg) + SERVBASE;
	    break;
	case 'd':
	    disp_name = strdup( optarg);
	    break;
	default:
	    fprintf( stderr, 
		     "Usage %s [-p port#][-d display][ -g]\n", argv[ 0]);
	    exit( -1);
	    break;
	}
    }

/* try to open the display to the real server */

    THE_DISPLAY = XOpenDisplay( disp_name);
    if ( THE_DISPLAY == NULL) {
	fprintf( stderr, "failed to open display %s\n", disp_name);
	exit( -1);
    }

/* open and read the font file */

    if ( getenv( "XPRINTFONTS") == NULL) { 
	fprintf( stderr, "you must set the environment variable XPRINTFONTS\n");
	exit( -1);
    }
    fontlist = read_strvec( getenv( "XPRINTFONTS"));
    if ( fontlist == NULL) {
	fprintf( stderr, "failed to open font mapping file %s\n",
		 getenv( "XPRINTFONTS"));
	exit( -1);
    }

/* set the defaults, and read the options file if there is one */

    PRINT_OPTIONS.index_cspace_support = TRUE;
    if ( getenv( "XPRINTOPTS") == NULL) {
	if ( debug) printf( "no options file set, using default values\n");
    } else {
	fp = fopen( getenv( "XPRINTOPTS"), "r");
	if ( fp == NULL) {
	    fprintf( stderr, "failed to open file %s, using defaults\n",
		     getenv( "XPRINTOPTS"));
	} else {

/* read options here. Add more for extensions */

	    fscanf( fp, "%d", &PRINT_OPTIONS.index_cspace_support);
	    fclose( fp);
	}
    }

/* block out SIGPIPE for when clients die */

    act.sa_flags = 0;
    act.sa_handler = SIG_IGN;
    sigaction( SIGPIPE, &act, &oact);

/* create the accept socket and the file descriptors for the select call */

    if (( accept_fd = create_accept_socket( port)) < 0) {
	perror( "create_accept_socket failed");
	exit( -1);
    }
    highest_fd = accept_fd;
    FD_ZERO( &readfds);
    FD_ZERO( &writefds);
    FD_ZERO( &exceptfds);
    FD_SET( accept_fd, &readfds);

/* initialize the client data structures */

    init_print_clients( PCS, STATICMAXFD);
    do {

/* copy to the select mask */

	for ( i = 0; i <= highest_fd; i++){
	    if ( FD_ISSET( i, &readfds)) {
		FD_SET( i, &readmask);
	    }
	}
	if ( select( highest_fd + 1, &readmask, &writefds, 
		     &exceptfds, NULL) < 0) {

/* some client disconnected */

	    if ( errno == EBADF) {
if ( debug) printf( "client closed connection\n");
		for ( client_fd = 0; client_fd <= highest_fd; client_fd++) {
		    if ( client_fd == accept_fd) {
			continue;
		    }
		    if ( FD_ISSET( client_fd, &readmask)) {
			FD_CLR( client_fd, &readfds);
		    }
		}
	    }

/* if there's an input on the accept socket, accept the connection */

	} else if ( FD_ISSET( accept_fd, &readmask)) {
	    client_fd = accept_connection( accept_fd);
if ( debug) printf( "accepting connection\n");

/* do the "init" conversation right away, since it's a little different 
   than the rest */

	    if ( init_connection( client_fd)) {
		FD_SET( client_fd, &readmask);
		PCS[ client_fd].printfp = NULL;
		if ( client_fd > highest_fd) {
		    highest_fd = client_fd;
		}
	    }

/* otherwise, it's just input from the client, and a response */

	} else {
	    for ( client_fd = 0; client_fd <= highest_fd; client_fd++) {
		if ( client_fd == accept_fd) {
		    continue;
		}
		if ( FD_ISSET( client_fd, &readmask)) {
		    client_flag = True;

/* read the request, find out how big it is, and read the rest of it, 
   then send the reply if there is one */

		    if ( read_loop( client_fd, &message.req, 
				    sizeof( message.req)) <= 0) {
if ( debug) printf( "client %d disconnected\n", client_fd);
			client_flag = False;
		    }
		    if ( message.req.length > 1) {
			if ( read_loop( client_fd, 
					message.buf + sizeof( xReq), 
					( message.req.length << 2) - 
					sizeof( xReq)) <= 0) {
if ( debug) printf( "client %d disconnected\n", client_fd);
			    client_flag = False;
			}
		    }
if ( debug) printf( "reading request %d words\n", message.req.length);
		    DecodeRequest( client_fd, message.buf, reply.buf, 
				   &reply_len);
		    if ( reply_len > 0) {

/* if the reply length is less than the length of a generic reply, then 
   pump up the volume */

			if ( reply_len < sizeof( xGenericReply)) {
			    reply_len = sizeof( xGenericReply);
			}

if ( debug) printf( "sending reply %d words\n", reply_len >> 2);

/* if the reply write failed, then the client disconnected */

			if ( write( client_fd, reply.buf, reply_len) < 0){
if ( debug) printf( "client %d disconnected\n", client_fd);
			    client_flag = False;
			}
		    }

/* if anything failed, the close out the client, and remove the 
   associated structures */

		    if ( !client_flag) {
			close( client_fd);
			remove_client( client_fd);
			FD_CLR( client_fd, &readmask);
		    }
		}
	    }
	}
    } while ( True);
}

/* initialize the connection with the client. Some of these are wild guesses */

int init_connection( int	client_fd)
{
    xConnClientPrefix	prefix_msg;
    xConnSetupPrefix	setup_msg;
    xConnSetup		conn_setup_msg;
    xPixmapFormat	pix_format_msg;
    xWindowRoot		window_msg;
    xDepth		depth_msg;
    xVisualType		visual_msg;
    unsigned char	*auth_msg;
    int			auth_len, prefix_len;
    char		vendor[ VENBYTES];

/* read the prefix message from the client */

    if ( read_loop( client_fd, &prefix_msg, sizeof( prefix_msg)) < 0) {
	perror( "prefix read failed");
	return( False);
    }

/* read the authorization information if there is any */

    auth_len = prefix_msg.nbytesAuthProto + prefix_msg.nbytesAuthString;
    if ( auth_len > 0) {
	auth_msg = ( unsigned char *) malloc( auth_len);
	if ( read_loop( client_fd, auth_msg, auth_len) < 0) {
	    perror( "auth read failed");
	    free( auth_msg);
	    return( False);
	}
	free( auth_msg);
    }

/* write the setup message to the client */

    setup_msg.success = True;
    setup_msg.lengthReason = 0;
    setup_msg.majorVersion = 11;
    setup_msg.minorVersion = 0;
    setup_msg.length = ( sizeof( xConnSetup) +
			 VENBYTES + 2*sizeof( xPixmapFormat) +
			 1*sizeof( xWindowRoot) + sizeof( xDepth) +
			 sizeof( xVisualType))/4;
    if ( write( client_fd, &setup_msg, sizeof( setup_msg)) < 0) {
	perror( "setup write failed");
	return( False);
    }

/* write the connection setup  message to the client */

    conn_setup_msg.release = 1;
    conn_setup_msg.ridBase = client_fd << CLIENTOFFSET;
    conn_setup_msg.ridMask = RESOURCE_ID_MASK;
    conn_setup_msg.motionBufferSize = 256;
    conn_setup_msg.nbytesVendor = VENBYTES;
    conn_setup_msg.maxRequestSize = MAXREQSZ / 4;
    conn_setup_msg.numRoots = 1;
    conn_setup_msg.numFormats = 2;
    conn_setup_msg.imageByteOrder = MSBFirst;
    conn_setup_msg.bitmapBitOrder = MSBFirst;
    conn_setup_msg.bitmapScanlineUnit = 8;
    conn_setup_msg.bitmapScanlinePad = 32;
    conn_setup_msg.minKeyCode = 8;
    conn_setup_msg.maxKeyCode = 132;
if ( debug) {
    printf(  "rid base = %d, mask = 0x%x, maxrequest = %d\n",
	    conn_setup_msg.ridBase, conn_setup_msg.ridMask,
	    conn_setup_msg.maxRequestSize);
}
    if ( write( client_fd, &conn_setup_msg, sizeof( conn_setup_msg)) < 0) {
	perror( "conn setup message write failed");
	return( False);
    }

/* write out the vendor string */

    strcpy( vendor, "print server");
    if ( write( client_fd, vendor, sizeof( vendor)) < 0) {
	perror( "vendor write failed");
	return( False);
    }

/* write the pixmap formats */

    pix_format_msg.depth = 1;
    pix_format_msg.bitsPerPixel = 1;
    pix_format_msg.scanLinePad = 32;
    if ( write( client_fd, &pix_format_msg, sizeof( pix_format_msg)) < 0) {
	perror( "pixmap format message write failed");
	return( False);
    }
    pix_format_msg.depth = 8;
    pix_format_msg.bitsPerPixel = 8;
    pix_format_msg.scanLinePad = 32;
    if ( write( client_fd, &pix_format_msg, sizeof( pix_format_msg)) < 0) {
	perror( "pixmap format message write failed");
	return( False);
    }

/* write the message describing our hypothetical screen and root window */

    window_msg.windowId = 0;
    window_msg.defaultColormap = 
	DefaultColormap( THE_DISPLAY, DefaultScreen( THE_DISPLAY));
    window_msg.whitePixel = 0;
    window_msg.blackPixel = 1;
    window_msg.currentInputMask = 0;
    window_msg.pixWidth = 1024;
    window_msg.pixHeight = 1024;
    window_msg.mmWidth = 256;
    window_msg.mmHeight = 256;
    window_msg.minInstalledMaps = 1;
    window_msg.maxInstalledMaps = 12;
    window_msg.rootVisualID = 0x20;
    window_msg.backingStore = False;
    window_msg.saveUnders = False;
    window_msg.rootDepth = 8;
    window_msg.nDepths = 1;
    if ( write( client_fd, &window_msg, sizeof( window_msg)) < 0) {
	perror( "window message write failed");
	return( False);
    }

/* write the depth information */

    depth_msg.depth = 8;
    depth_msg.nVisuals = 1;
    if ( write( client_fd, &depth_msg, sizeof( depth_msg)) < 0) {
	perror( "window message write failed");
	return( False);
    }

/* write the visual */

    visual_msg.visualID = 0x20;
    visual_msg.class = PseudoColor;
    visual_msg.bitsPerRGB = 8;
    visual_msg.redMask = 0;
    visual_msg.greenMask = 0;
    visual_msg.blueMask = 0;
    if ( write( client_fd, &visual_msg, sizeof( visual_msg)) < 0) {
	perror( "window message write failed");
	return( False);
    }
    return( True);
}

/* call read in a loop, to make sure we read everything we're supposed to */

int read_loop( int	fd,
	       char 	*msg,
	       int	length)
{
    int	nbytes, total = 0;

    do {
	nbytes = read( fd, msg, length);
	if ( nbytes <= 0) {
	    return( nbytes);
	} else {
	    length -= nbytes;
	    total += nbytes;
	    msg += nbytes;
	}
    } while ( length > 0);
if ( debug) printf( "read %d bytes\n", total);
    return( total);
}

/* read a NULL-terminated vector of strings from a file */

char **read_strvec( char *filename)
{
    FILE 	*fp;
    char	buffer[ 512];
    int		lineno = 1;
    char	**vector, **vec_ptr;

/* read it to find the number of lines */

    if (( fp = fopen( filename, "r")) == NULL) {
	return( NULL);
    }
    do {	
	if ( fgets( buffer, sizeof( buffer), fp) == NULL) {
	    break;
	}
	lineno++;
    } while ( TRUE);

/* rewind and allocate the vector */

    rewind( fp);
    vector = vec_ptr = ( char **) malloc( sizeof( char *) * ( lineno + 1));

/* read the lines and "strdup" them after getting rid of the \n */

    do {
	if ( fgets( buffer, sizeof( buffer), fp) == NULL) {
	    break;
	}
	buffer[ strlen( buffer) - 1] = '\0';
	*vec_ptr++ = strdup( buffer);
    } while ( TRUE);
    *vec_ptr = NULL;

/* close and return */

    fclose( fp);
    return( vector);
}
