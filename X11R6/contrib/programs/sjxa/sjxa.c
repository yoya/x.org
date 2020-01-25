/* $SonyId: sjxa.c,v 1.2 1994/06/03 06:33:04 makoto Exp $ */
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
/*
 *	sjxa: X Window System Version 11 (Xaw version)
 *	      Input Method Server
 */
#include	<stdio.h>
#include	<stdlib.h>
#include	<locale.h>
#include	<string.h>
#include	<pwd.h>
#include	<netdb.h>
#include	<sys/param.h>
#include	<signal.h>
#include	<unistd.h>
#include	"sjxa.h"
#include	"version.h"
#include	"SjString.h"
#include	"Ximp.h"
#ifdef SVR4
#include	<sys/utsname.h>
#include	<sys/systeminfo.h>
#define sigmask(n)              ((unsigned long)1 << ((n) - 1))
#endif /* SVR4 */

char	*program_name;
char	 user_name[BUFSIZ]		= { "" };
char	 home_directory[BUFSIZ]		= { "" };
LcCtype	 user_locale;
char	*locale_name;
Endian	 endian;

static Boolean init_done = False;

static XtAppContext	app;
Widget			delete_shell	= NULL;
Widget			question_shell	= NULL;
Widget			server_shell	= NULL;

int	*depths = NULL;

#if	NeedFunctionPrototypes
extern XtAppContext root_window_init(Buffer *b, int argc,char *argv[]);
#else	/* NeedFunctionPrototypes */
extern XtAppContext root_window_init();
#endif

extern Widget		TopLevel;
extern Buffer		root_window;


void
#if NeedFunctionPrototypes
usage( void )
#else
usage()
#endif
{
    puts( "Sony Kana-Kanji Conversion IM Server" );
    printf( "%s ver.%d.%d.%d\n", program_name,
	   MAJOR_VERSION, MINOR_VERSION, BUGFIX_VERSION );
    puts( "Copyright (c) 1993 Sony Corporation" );
    puts( "\nusage:" );
    printf( "        %s [-options ...] &", program_name );
    puts( "\nwhere options include:" );
    puts( "    -help                        print out this message" );
    puts( "    -display displayname         X server to contact" );
    puts( "    -geometry geom               size (in characters) and position" );
    puts( "    -bg color                    background color" );
    puts( "    -fg color                    foreground color" );
    puts( "    -bd color                    border color" );
    puts( "    -bw number                   border width in pixels" );
    puts( "    -iconic                      start iconic" );
    puts( "    -name string                 client instance, icon, and title strings" );
    puts( "    -reverse                     reverse of foreground color" );
    puts( "    -/+rv                        turn on/off reverse video" );
    puts( "    -/+synchronous               turn on/off synchronous mode" );
    puts( "    -title string                title string" );
    puts( "    -server hostname             host name for sj3serv");
    puts( "    -no_auto_replace             turn off auto replace mode" );
    puts( "    -/+auto_raise                turn on/off auto raise" );
    puts( "    -/+kana                      turn on/off kana input mode" );
#if defined(USE_XIMCP)
    puts( "    -/+ximcp                     turn on/off xim common protocol" );
#endif
#ifdef USE_XIMP
    puts( "    -ximp version                ximp version" );
#endif
#ifdef USE_XLC
    puts( "    -/+xlc                       turn on/off xlc protocol" );
#endif
#ifdef USE_SJX
    puts( "    -/+sjx                       turn on/off sjx protocol" );
#endif
    puts( "    -xrm resourcestring          additional resource specifications" );
    exit( 1 );
}


static void
#if NeedFunctionPrototypes
initialize( int argc, char *argv[] )
#else
initialize( argc, argv )
    int		argc;
    char	*argv[];
#endif
{
    int			ii;
    char		*ss, *tt;
    struct passwd	*pwd;

   /* set program name */
    if( (ss = strrchr( argv[0], '/' )) == 0 )
	program_name = argv[0];
    else
	program_name = ss + 1;

   /* get user locale */
    if( (locale_name = setlocale( LC_CTYPE, "" )) == NULL ) {
	fprintf( stderr, "%s error: locale could not be set.\n", program_name );
	exit( 1 );
    }
    if( strcmp( locale_name, "ja_JP.SJIS" ) == 0 )
	user_locale = LC_CTYPE_SHIFTJIS;
    else if( strncmp( locale_name, "ja_JP.EUC", 2 ) == 0 )
	user_locale = LC_CTYPE_EUC;
    else if( strstr( locale_name, "ja" ) != NULL )
	user_locale = LC_CTYPE_EUC;
    else {
	fprintf( stderr, "%s error: locale could not be set.\n", program_name );
	exit( 1 );
    }

   /* set user name and home directory */
    ss = getlogin();
    tt = getenv( "HOME" );
    setpwent();
    if( ss == NULL )
	pwd = getpwuid( getuid() );
    else
	pwd = getpwnam( ss );
    if( pwd != NULL ) {
	strcpy( user_name, pwd->pw_name );
	if( tt == NULL )
	    tt = pwd->pw_dir;
    }
    if( tt != NULL )
	strcpy( home_directory, tt );
    endpwent();

   /* argument previous check */
    for( ii = 1; ii < argc; ii++ )
	if( !strcmp( "-help", argv[ii] ) )
	    usage();

    /* endian */
    ii = 1;
    endian = *(char *)&ii ? LITTLEENDIAN : BIGENDIAN;
}


void
#if NeedFunctionPrototypes
done( int sig )
#else
done( sig )
    int	sig;
#endif
{
    if( init_done ) {
#ifdef USE_SJ3LIB
	sj3_unlockserv();
	sj3_close();
#endif

#ifdef USE_SJX
	sjxp_all_end();
#endif
#ifdef USE_XLC
	xlcp_all_end( NULL );
#endif
#ifdef USE_XIMP
	ximp_all_end();
#endif

	XSync( XtDisplay(TopLevel), False );
    }
    fprintf( stderr, "\n\r%s done.\n\r", program_name );
    exit( 0 );
}


static void
#if NeedFunctionPrototypes
suspend( int sig )
#else
suspend( sig )
    int	sig;
#endif
{
#ifdef USE_SJ3LIB
    sj3_unlockserv();
    sj3_close();
#endif

#ifdef USE_SJX
    sjxp_end();
#endif
#ifdef USE_XLC
    xlcp_all_end( NULL );
#endif
#ifdef USE_XIMP
    ximp_all_end();
#endif

    XSync( XtDisplay(TopLevel), False );

    signal( SIGTSTP, SIG_DFL );
#ifndef	SVR4
    sigsetmask( sigblock( 0 ) & ~sigmask(SIGTSTP) );
#else
    sigrelse( SIGTSTP );
#endif
    kill( 0, SIGTSTP );

#ifndef	SVR4
    sigblock( sigmask(SIGTSTP) );
    signal( SIGTSTP, suspend );
#else
    sighold( SIGTSTP );
    sigset( SIGTSTP, suspend );
#endif
}


static void
#if NeedFunctionPrototypes
makecore( int sig )
#else
makecore( sig )
    int	sig;
#endif
{
    if( init_done ) {
#ifdef USE_SJ3LIB
	sj3_unlockserv();
	sj3_close();
#endif

#ifdef USE_SJX
	sjxp_end();
#endif
#ifdef USE_XLC
	xlcp_all_end( NULL );
#endif
#ifdef USE_XIMP
	ximp_all_end();
#endif

	XSync( XtDisplay(TopLevel), False );
    }
    fprintf( stderr, "\n\r%s terminate.\n\r", program_name );
    abort();
}


static void	signal_initialize()
{
#ifndef	SVR4
    signal( SIGTSTP, SIG_IGN );		/* CTRL z */
    signal( SIGTERM, done );
    signal( SIGINT, done );
#ifndef	DEBUG
    signal( SIGQUIT, makecore );
    signal( SIGBUS, makecore );
    signal( SIGSEGV, makecore );
    signal( SIGFPE, makecore );
#endif	/* DEBUG */
#else	/* SVR4 */
    sigignore( SIGTSTP );		/* CTRL z */
    sigset( SIGTERM, done );
    sigset( SIGINT, done );
#ifndef	DEBUG
    sigset( SIGQUIT, makecore );
    sigset( SIGBUS, makecore );
    sigset( SIGSEGV, makecore );
    sigset( SIGFPE, makecore );
#endif	/* DEBUG */
#endif	/* SVR4 */
}


static Boolean
#if NeedFunctionPrototypes
event_pre_hook( XEvent *event )
#else /* NeedFunctionPrototypes */
event_pre_hook( event )
    XEvent	*event;
#endif /* NeedFunctionPrototypes */
{
    switch( event->type ) {
      case KeyPress:
#ifdef USE_XIMP
	if( XimpFilterEvent( (XKeyEvent *)event ) )
	    return( True );
#endif
	event->xkey.subwindow = event->xany.window;
	if( PreeditChangeKeyDirection( event ) )
	    break;
	break;

      case DestroyNotify:
	if( ChangeDestroyNotify( event ) )
	    return( True );
	break;

      case Expose:
	if( event->xexpose.count == 0 )
	    if( ChangeExpose( event ) )
		break;
	break;

      case MapNotify:
      case UnmapNotify:
	if( IconFilterEvent( event ) )
	    return( True );
	break;

      case ClientMessage:
	ChangeClientMessage( TopLevel, event );
	break;
    }

    return( False );
}


static void
#if NeedFunctionPrototypes
event_post_hook( XEvent *event )
#else /* NeedFunctionPrototypes */
event_post_hook( event )
    XEvent	*event;
#endif /* NeedFunctionPrototypes */
{
    switch( event->type ) {
      case KeyPress:
#ifdef USE_SJX
	SjxpSync( event, True );
#endif
	break;
    }
}


void
#if NeedFunctionPrototypes
main( int argc, char *argv[] )
#else
main( argc, argv )
    int		argc;
    char	*argv[];
#endif
{
    Display	*display;
    XEvent	 event;

    initialize( argc, argv );

    app = root_window_init( &root_window, argc, argv );
    display = XtDisplay( TopLevel );
    getsjxark();
    signal_initialize();
    depth_initialize( display );
#if defined(USE_XIMCP)
    ximcp_initialize( TopLevel );
#endif
#ifdef USE_XIMP
    ximp_initialize( TopLevel );
#endif
#ifdef USE_XLC
    xlc_protocol_initialize( display );
    XlcpSetBCKey( TopLevel );
#endif
#ifdef USE_SJX
    sjxp_initialize( app, display );
#endif

#ifndef SVR4
    signal( SIGTSTP, suspend );		/* CTRL z */
#else
    sigset( SIGTSTP, suspend );		/* CTRL z */
#endif
    init_done = True;

    XBell( display, 0 );
    XFlush( display );

   /*
    *	loop
    */
    for(;;) {
	XtAppNextEvent( app, &event );
	if( event_pre_hook( &event ) )
	    continue;
	XtDispatchEvent( &event );
	event_post_hook( &event );
    }
}
