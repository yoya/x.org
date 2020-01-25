/* $SonyId: Sjxp.c,v 1.1 1994/05/30 13:03:58 makoto Exp $ */
/******************************************************************

Copyright (c) 1993, 1994  Sony Corporation

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL SONY CORPORATION BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of Sony Corporation shall
not be used in advertising or otherwise to promote the sale, use or ot
her dealings in this Software without prior written authorization from
Sony Corporation.

******************************************************************/
#ifdef	USE_SJX
#include	<stdio.h>
#include	<netdb.h>
#include	<sys/param.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<X11/Xlib.h>
#include	<X11/Intrinsic.h>
#include	<X11/StringDefs.h>
#include	<Xmw/Xmw.h>
#include	<X11/Xatom.h>
#define	XK_KATAKANA
#include	<X11/keysym.h>
#ifdef SVR4
#include	<sys/systeminfo.h>
#endif
#include	"SjString.h"
#include	"resource.h"

#define SJX_TCP_PORT	3104
#define SJX_PORT_LIMIT	512
#define SJX_MACHINE	"_SJX_MACHINE"
#define SJX_PORT	"_SJX_PORT"

#define	SJX_SERVICE	"sjx"

/* Protocol Number */
#define SJX_CONFIGURE_REQUEST	1
#define SJX_MAP_REQUEST		2
#define SJX_UNMAP_REQUEST	3
#define SJX_SEND_KEYEVENT	4
#define SJX_CODE_REQUEST	5

#define SJX_CODE_SJIS		0
#define SJX_CODE_EUC		1

typedef struct _SjxpQueue {
    struct _SjxpQueue	*next;
    struct _SjxpQueue	*previous;
    Window		window;
    int			descriptor;
    XtInputId		id;
    int			code;
    Boolean		connect_flag;
    Boolean		send_event;
    Buffer		*buffer;
} SjxpQueue;


static Atom		sjx_host, sjx_port;
static fd_set		ready_clients;
static SjxpQueue	*sjxp_queue = NULL;
static KeySym		keysym;
static char		string[20], send_data[1024];

extern Widget		TopLevel, RootTextField;
extern LcCtype		user_locale;
extern Buffer		root_window;
extern Sjxa		resources;


static void
#if NeedFunctionPrototypes
send_to_socket( SjxpQueue *q, KeySym keysym, char *str )
#else /* NeedFunctionPrototypes */
send_to_socket( q, keysym, str )
    SjxpQueue	*q;
    KeySym	keysym;
    char	*str;
#endif /* NeedFunctionPrototypes */
{
    char	buffer[BUFSIZ];
    int		length;
    int		wlen = 1;

    buffer[0] = q->connect_flag ? 1 : 0;

    bcopy( &keysym, &buffer[1], sizeof(keysym) );
    wlen += sizeof(keysym);

    if( (q->code == SJX_CODE_SJIS  &&  user_locale == LC_CTYPE_EUC)  ||
	(q->code == SJX_CODE_EUC  &&  user_locale == LC_CTYPE_SHIFTJIS) ) {
	switch( q->code ) {
	  case SJX_CODE_SJIS:
	    str = Euc2Sjis( str );
	    break;
	  case SJX_CODE_EUC:
	    str = Sjis2Euc( str );
	    break;
	}
    }
    length = strlen( str );
    bcopy( &length, &buffer[wlen], sizeof(length) );
    wlen += sizeof(length);

    bcopy( str, &buffer[wlen], length );
    wlen += length;

    write( q->descriptor, buffer, wlen );
    q->send_event = True;
}


static void
#if NeedFunctionPrototypes
SjxpConfigureRequest( int descriptor )
#else /* NeedFunctionPrototypes */
SjxpConfigureRequest( descriptor )
    int	descriptor;
#endif /* NeedFunctionPrototypes */
{
    Dimension			border_width;
    int				len;
    struct {
	int	x, y;
	int	width, height; }	info;

    len = read( descriptor, &info, sizeof(info) );
    if( len <= 0 )
	return;
    XtVaGetValues( TopLevel, XtNborderWidth, &border_width, NULL );
    XtConfigureWidget( TopLevel, info.x, info.y, info.width, info.height,
		       border_width );
}


static void
#if NeedFunctionPrototypes
SjxpMapRequest( void )
#else /* NeedFunctionPrototypes */
SjxpMapRequest()
#endif /* NeedFunctionPrototypes */
{
    XRaiseWindow( XtDisplay(TopLevel), XtWindow(TopLevel) );
}


static void
#if NeedFunctionPrototypes
SjxpUnmapRequest( void )
#else /* NeedFunctionPrototypes */
SjxpUnmapRequest()
#endif /* NeedFunctionPrototypes */
{
    XUnmapWindow( XtDisplay(TopLevel), XtWindow(TopLevel) );
}


#define IsKanaKey(k)	(((unsigned)(k) >= XK_overline) && \
			((unsigned)(k) <= XK_semivoicedsound))

static void
#if NeedFunctionPrototypes
SjxpSendKeyevent( SjxpQueue *q )
#else /* NeedFunctionPrototypes */
SjxpSendKeyevent( q )
    SjxpQueue	*q;
#endif /* NeedFunctionPrototypes */
{
    XKeyEvent		event;
    int			count;

    if( read( q->descriptor, &event, sizeof(event) ) <= 0 )
	return;
    q->window = event.window;
    event.display = XtDisplay(TopLevel);
    string[0] = send_data[0] = '\0';

    count = XLookupString( &event, string, sizeof(string), &keysym, NULL );
    if( count == 0 ) {
	if( IsKanaKey(keysym) ) {
	    switch( user_locale ) {
	      LC_CTYPE_SHIFTJIS:
		string[0] = keysym & 0xff;
		string[1] = '\0';
		count = 1;
		break;

	      LC_CTYPE_EUC:
		string[0] = SS2;
		string[1] = keysym & 0xff;
		string[2] = '\0';
		count = 2;
		break;
	    }
	}
    }

    if( q->connect_flag ) {
	q->send_event = False;
	q->buffer->connect--;
	XPutBackEvent( event.display, (XEvent *)&event );
    }
    else {
	if( is_started_key( &event ) ) {
	    q->connect_flag = True;
	    q->buffer->connect++;
	    RootWindowMap( q->buffer, q->buffer->conversion_style );
	    send_to_socket( q, NoSymbol, "" );
	}
	else {
	    send_to_socket( q, keysym, string );
	}
    }
}


static void
#if NeedFunctionPrototypes
SjxpCodeRequest( SjxpQueue *q )
#else /* NeedFunctionPrototypes */
SjxpCodeRequest( q )
    SjxpQueue	*q;
#endif
{
    int	code;

    if( read( q->descriptor, &code, sizeof(code) ) <= 0 )
	return;
    switch( code ) {
      case SJX_CODE_SJIS:
      case SJX_CODE_EUC:
	q->code = code;
	break;
    }
}


static void
#if NeedFunctionPrototypes
SjxpDestroy( SjxpQueue *q )
#else /* NeedFunctionPrototypes */
SjxpDestroy( q )
    SjxpQueue	*q;
#endif /* NeedFunctionPrototypes */
{
    if( q->previous )
	q->previous->next = q->next;
    else
	sjxp_queue = q->next;
    if( q->next )
	q->next->previous = q->previous;
    XtRemoveInput( q->id );
    FD_CLR( q->descriptor, &ready_clients );
    shutdown( q->descriptor, 2 );
    close( q->descriptor );

    if( q->connect_flag ) {
	if( q->buffer->error_shell )
	    error_cancel( (Widget)NULL, (caddr_t)q->buffer, (caddr_t)NULL );
	if( q->buffer->state & DELETE )
	    sakujo_cancel( (Widget)NULL, (caddr_t)q->buffer, (caddr_t)NULL );
	if( q->buffer->state & CODE )
	    code_cancel( (Widget)NULL, (caddr_t)q->buffer, (caddr_t)NULL );
	if( q->buffer->state & ADD )
	    touroku_cancel( (Widget)NULL, (caddr_t)q->buffer, (caddr_t)NULL );
	if( q->buffer->state & SERVER )
	    reconnect_cancel( (Widget)NULL, (caddr_t)q->buffer, (caddr_t)NULL );
	if( q->buffer->state & SELECT )
	    XtUnmapWidget( RootTextField );
	if( q->buffer->state & EDIT )
	    edit_cancel( q->buffer );
	if( q->buffer->state & (WRAP | CONVERSION) )
	    henkan_cancel( q->buffer );

	RootWindowUnmap( q->buffer->conversion_style );
    }
    free( q );
}


static void
#if NeedFunctionPrototypes
SjxpRequest( XtPointer client_data, int *descriptor, XtInputId *ptr )
#else /* NeedFunctionPrototypes */
SjxpRequest( client_data, descriptor, ptr )
    XtPointer	client_data;
    int		*descriptor;
    XtInputId	*ptr;
#endif /* NeedFunctionPrototypes */
{
    SjxpQueue	*q = (SjxpQueue *)client_data;
    int		len;
    u_char	pnum;

    len = read( *descriptor, &pnum, sizeof(pnum) );
    if( len <= 0 ) {
	SjxpDestroy( q );
	return;
    }
    switch( pnum ) {
      case SJX_CONFIGURE_REQUEST:
	SjxpConfigureRequest( *descriptor );
	break;

      case SJX_MAP_REQUEST:
	SjxpMapRequest();
	break;

      case SJX_UNMAP_REQUEST:
	SjxpUnmapRequest();
	break;

      case SJX_SEND_KEYEVENT:
	SjxpSendKeyevent( q );
	break;

      case SJX_CODE_REQUEST:
	SjxpCodeRequest( q );
	break;

      default:
	fprintf( stderr, "unknown protocol number 0x%x\n", pnum );
	break;
    }
}


static void
#if NeedFunctionPrototypes
SjxpCreate( XtPointer client_data, int *socket, XtInputId *ptr )
#else /* NeedFunctionPrototypes */
SjxpCreate( client_data, socket, ptr )
    XtPointer	client_data;
    int		*socket;
    XtInputId	*ptr;
#endif /* NeedFunctionPrototypes */
{
    XtAppContext	app = (XtAppContext)client_data;
    int			client;
    SjxpQueue		*q1, *q2;

    client = accept( *socket, 0, 0 );
    if( client == -1 )
	return;

    FD_SET( client, &ready_clients );

    q1 = (SjxpQueue *)calloc( 1, sizeof(SjxpQueue) );

    for( q2 = sjxp_queue; q2 != NULL; q2 = q2->next ) {
	if( q2->next == NULL ) {
	    q2->next = q1;
	    q1->previous = q2;
	    break;
	}
    }
    if( q2 == NULL )
	sjxp_queue = q1;

    q1->descriptor = client;
    q1->code = SJX_CODE_SJIS;
    q1->buffer = &root_window;
    q1->id = XtAppAddInput( app, client, (XtPointer)XtInputReadMask,
			    (XtInputCallbackProc)SjxpRequest,
			    (XtPointer)q1 );
}


static Boolean
#if NeedFunctionPrototypes
IsLocalDisplay( char *hostname, Display *display )
#else /* NeedFunctionPrototypes */
IsLocalDisplay( hostname, display )
    char	*hostname;
    Display	*display;
#endif /* NeedFunctionPrototypes */
{
    char	*p, *q;
    char	*display_name;
    char	local_host[MAXHOSTNAMELEN];

    display_name = XDisplayName( DisplayString(display) );

    if( display_name != (char *)NULL  &&  *display_name != '\0'  &&
	*display_name != ':' ) {
	if( strncmp( display_name, "unix:", 5 ) == 0 )
	    return( True );
	strcpy( local_host, hostname );
	strcat( local_host, ":" );
	p = local_host;
	q = display_name;
	for(;;) {
	    if( *p == '\0' )
		break;
	    if( *(q++) != *(p++) )
		return( False );
	}
    }
    return( True );
}


void
#if NeedFunctionPrototypes
sjxp_initialize( XtAppContext app, Display *display )
#else /* NeedFunctionPrototypes */
sjxp_initialize( app, display )
    XtAppContext	app;
    Display		*display;
#endif /* NeedFunctionPrototypes */
{
    char		hostname[MAXHOSTNAMELEN];
    struct servent	*sp;
    struct sockaddr_in	server;
    struct hostent	*hp;
    int			port;
    int			port_limit;
    int			so;
    int 		optval;

    if( !resources.sjx_protocol )
	return;
#ifdef SVR4
    sysinfo( SI_HOSTNAME, hostname, MAXHOSTNAMELEN );
#else
    gethostname( hostname, MAXHOSTNAMELEN );
#endif
    if( (sp = getservbyname( SJX_SERVICE, "tcp" )) == NULL )
	port = SJX_TCP_PORT;
    else
	port = sp->s_port;

    if( (hp = gethostbyname( hostname )) == NULL ) {
	fputs( "sjx protocol can't use.", stderr );
	resources.sjx_protocol = False;
	return;
    }

    if( (so = socket( hp->h_addrtype, SOCK_STREAM, 0 )) < 0 ) {
	fputs( "sjx protocol can't use.", stderr );
	resources.sjx_protocol = False;
	return;
    }

    bzero( (char *)&server, sizeof(server) );
    bcopy( (char *)hp->h_addr, (char *)&server.sin_addr, hp->h_length );
    server.sin_family = hp->h_addrtype;
    setsockopt( so, SOL_SOCKET, SO_REUSEADDR, (char *)&optval, sizeof(optval) );
    if( !IsLocalDisplay( hostname, display ) ) {
	for( port_limit = 0, port++;; port_limit++, port++ ) {
	    if( port_limit >= SJX_PORT_LIMIT ) {
		shutdown( so, 2 );
		close( so );
		fputs( "sjx protocol can't use.", stderr );
		resources.sjx_protocol = False;
		return;
	    }
	    server.sin_port = port;
	    if( bind( so, (struct sockaddr *)&server, sizeof(server) ) == 0 )
		break;
	}
    }
    else {
	server.sin_port = port;
	if( bind( so, (struct sockaddr *)&server, sizeof(server) ) ) {
	    shutdown( so, 2 );
	    close( so );
	    fputs( "sjx protocol can't use.", stderr );
	    resources.sjx_protocol = False;
	    return;
	}
    }

    FD_ZERO( &ready_clients );

    listen( so, 5 );
    sjx_host = XInternAtom( display, SJX_MACHINE, False );
    sjx_port = XInternAtom( display, SJX_PORT, False );
    XChangeProperty( display, RootWindow(display, 0), sjx_host, XA_STRING, 8,
		     PropModeReplace, (unsigned char *)hostname,
		     strlen(hostname) );
    XChangeProperty( display, RootWindow(display, 0), sjx_port, XA_INTEGER, 32,
		     PropModeReplace, (unsigned char *)&port, 1 );

    XtAppAddInput( app, so, (XtPointer)XtInputReadMask,
		   (XtInputCallbackProc)SjxpCreate, (XtPointer)app );
}


void
#if NeedFunctionPrototypes
sjxp_all_end( void )
#else /* NeedFunctionPrototypes */
sjxp_all_end()
#endif /* NeedFunctionPrototypes */
{
    Display	*display = XtDisplay(TopLevel);

    if( !resources.sjx_protocol )
	return;
    XDeleteProperty( display, RootWindow(display, 0), sjx_host );
    XDeleteProperty( display, RootWindow(display, 0), sjx_port );
    XFlush( display );
}


Boolean
#if NeedFunctionPrototypes
SjxpPreeditChangeKeyDirection( XEvent *event )
#else /* NeedFunctionPrototypes */
SjxpPreeditChangeKeyDirection( event )
    XEvent	*event;
#endif /* NeedFunctionPrototypes */
{
    SjxpQueue	*q;

    for( q = sjxp_queue; q; q = q->next ) {
	if( q->window == event->xkey.window ) {
	    event->xkey.window = XtWindow(TopLevel);
	    XtSetKeyboardFocus( TopLevel, q->buffer->preedit_widget );
	    return( True );
	}
    }
    return( False );
}


Boolean
#if NeedFunctionPrototypes
SjxpSendText( Window win, wchar_t *wcs )
#else /* NeedFunctionPrototypes */
SjxpSendText( win, wcs )
    Window	win;
    wchar_t	*wcs;
#endif /* NeedFunctionPrototypes */
{
    SjxpQueue	*q;
    char	mbs[1024];

    for( q = sjxp_queue; q; q = q->next ) {
	if( q->window == win ) {
	    wcs2mbs( mbs, wcs, 1024 );
	    keysym = NoSymbol;
	    strcat( send_data, mbs );
	    XmwTextOutSetString( q->buffer->preedit_widget, NULL );
	    return( True );
	}
    }
    return( False );
}


void
#if NeedFunctionPrototypes
SjxpSync( XEvent *event, Boolean discard )
#else /* NeedFunctionPrototypes */
SjxpSync( event, discard )
    XEvent	*event;
    Boolean	discard;
#endif /* NeedFunctionPrototypes */
{
    SjxpQueue	*q;

    for( q = sjxp_queue; q; q = q->next ) {
	if( q->window == event->xkey.subwindow ) {
	    if( !q->send_event ) {
		if( discard ) {
		    if( send_data[0] == '\0' )
			send_to_socket( q, NoSymbol, "" );
		    else
			send_to_socket( q, keysym, send_data );
		}
		else
		    strcat( send_data, string );
	    }
	    break;
	}
    }
}


void
#if NeedFunctionPrototypes
sjxp_end( void )
#else /* NeedFunctionPrototypes */
sjxp_end()
#endif /* NeedFunctionPrototypes */
{
    SjxpQueue	*q;

    for( q = sjxp_queue; q; q = q->next ) {
	if( q->connect_flag ) {
	    if( q->buffer->error_shell )
		error_cancel( (Widget)NULL, (caddr_t)q->buffer, (caddr_t)NULL );
	    if( q->buffer->state & DELETE )
		sakujo_cancel( (Widget)NULL, (caddr_t)q->buffer,
			       (caddr_t)NULL );
	    if( q->buffer->state & CODE )
		code_cancel( (Widget)NULL, (caddr_t)q->buffer, (caddr_t)NULL );
	    if( q->buffer->state & ADD )
		touroku_cancel( (Widget)NULL, (caddr_t)q->buffer,
				(caddr_t)NULL );
	    if( q->buffer->state & SERVER )
		reconnect_cancel( (Widget)NULL, (caddr_t)q->buffer,
				  (caddr_t)NULL );
	    if( q->buffer->state & SELECT )
		XtUnmapWidget( RootTextField );
	    if( q->buffer->state & EDIT )
		edit_cancel( q->buffer );
	    if( q->buffer->state & (WRAP | CONVERSION) )
		henkan_cancel( q->buffer );
	    q->connect_flag = False;
	    q->buffer->connect--;
	    RootWindowUnmap( XIMPreeditNothing | XIMStatusNothing );
	}
    }
}
#endif
