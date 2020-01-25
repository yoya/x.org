/* $SonyId: Xlcp.c,v 1.1 1994/05/30 13:03:58 makoto Exp $ */
/******************************************************************

Copyright (c) 1992, 1993, 1994  Sony Corporation

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
#ifdef	USE_XLC
#include	<stdio.h>
#include	<stdlib.h>
#include	<X11/Xlib.h>
#include	<X11/Xatom.h>
#include	<X11/keysym.h>
#include	<X11/Intrinsic.h>
#include	<Xmw/TextOut.h>
#include	<Xmw/Preedit.h>
#include	"XlcpP.h"
#include	"sjxa.h"
#include	"resource.h"
#include	"SjBuffer.h"
#include	"SjString.h"

#define	ON	1
#define	OFF	0

static int		 xlc_count = 0;
static PropertyQueue	*property_queue = NULL;
static Atom		 xlcp_selection;
static Atom		 xlcp_selection2;
static Atom		 conversion_request;
static Atom		 conversion_notify;
static Atom		 conversion_end_request;
static Atom		 conversion_end;
static Atom		 conversion_close;
static Atom		 conversion_property;
static Atom		 conversion_inplace;
static Atom		 compound_text;

extern Buffer	 root_window;
extern Sjxa	 resources;
extern Widget	 TopLevel, RootText, RootTextField;
extern Widget	 RootLabel, Box[];
extern Buffer	*buffer;
extern char	*program_name;


static PropertyQueue *
#if NeedFunctionPrototypes
search_client_window( Window w )
#else
search_client_window( w )
    Window	w;
#endif
{
    PropertyQueue	*p;

    for( p = property_queue; p != NULL; p = p->next )
	if( p->client_window == w  ||  p->input_window == w )
	    return( p );
    return( NULL );
}


static char *
#if NeedFunctionPrototypes
fontid2xlfd( Display *display, Font fid )
#else
fontid2xlfd( display, fid )
    Display	*display;
    Font	 fid;
#endif
{
    XFontStruct	*fs;
    XFontProp	*fp;
    int		 ii;
    char	*ss = NULL;
    Atom	 charset;

    charset = XInternAtom( display, "CHARSET_REGISTRY", False );

    if( fid != NULL ) {
	fs = XQueryFont( display, fid );
	for( ii = 0, fp = fs->properties; ii < fs->n_properties;
	    ii++, fp++ ) {
	    if( fp->name == charset ) {
		ss = XGetAtomName( display, fp->card32 );
		break;
	    }
	}
	XFreeFontInfo( NULL, fs, 1 );
    }
    return( ss );
}


static void
#if NeedFunctionPrototypes
XlcpConversion( int s )
#else
XlcpConversion( s )
    int		 s;
#endif
{
    Display	*display = XtDisplay( TopLevel );
    Window	 window = XtWindow( TopLevel );
    Atom	 prop;
    static int	 status = ON;

    if( status == s )
	return;
    status = s;
    prop = XInternAtom( display, CONVERSION_STATUS, False );
    XChangeProperty( display, window, prop, XA_INTEGER, 32,
		     PropModeReplace, (unsigned char *)&s, 1 );
}


static void
#if NeedFunctionPrototypes
SendConversionEnd( PropertyQueue *p )
#else
SendConversionEnd( p )
    PropertyQueue	*p;
#endif
{
    XClientMessageEvent	event;

    event.type = ClientMessage;
    event.display = XtDisplay(TopLevel);
    event.window = p->client_window;
    event.send_event = True;
    event.message_type = conversion_end;
    event.format = 32;
    event.data.l[0] = xlcp_selection;
    event.data.l[1] = XtWindow(TopLevel);
    XSendEvent( XtDisplay(TopLevel), p->client_window, False, NULL,
		(XEvent *)&event ); 
}



static void
#if NeedFunctionPrototypes
xlcp_start( PropertyQueue *p )
#else
xlcp_start( p )
    PropertyQueue	*p;
#endif
{
    if( !p->conversion_close_flag ) {
	RootWindowMap( p->buffer, p->buffer->conversion_style );
	xlc_count++;
	XlcpConversion( ON );
    }
    else
	p->conversion_close_flag = False;
}


static Boolean
#if NeedFunctionPrototypes
xlcp_end( Display *display, Window w )
#else
xlcp_end( display, w )
    Display	*display;
    Window	 w;
#endif
{
    PropertyQueue	*p;

    if( (p = search_client_window( w )) == NULL  ||  !p->connect_flag )
	return( False );
    if( p->buffer->state & DELETE )
	sakujo_cancel( (Widget)NULL, (caddr_t)p->buffer, (caddr_t)NULL );
    if( p->buffer->state & CODE )
	code_cancel( (Widget)NULL, (caddr_t)p->buffer, (caddr_t)NULL );
    if( p->buffer->state & ADD )
	touroku_cancel( (Widget)NULL, (caddr_t)p->buffer, (caddr_t)NULL );
    if( p->buffer->state & SERVER )
	reconnect_cancel( (Widget)NULL, (caddr_t)p->buffer, (caddr_t)NULL );
    if( p->buffer->state & SELECT ) {
	if( p->buffer->conversion_style ==
	    (XIMPreeditNothing | XIMStatusNothing) )
	    XtUnmapWidget( RootTextField );
	else if( p->buffer->conversion_style & XIMPreeditPosition )
	    select_dialog_cancel( (Widget)NULL, (caddr_t)p->buffer,
				  (caddr_t)NULL );
    }
    if( p->buffer->state & EDIT )
	edit_cancel( p->buffer );
    if( p->buffer->state & (WRAP | CONVERSION) )
	henkan_cancel( p->buffer );

    if( !p->conversion_close_flag ) {
	xlc_count--;
	RootWindowUnmap( p->buffer->conversion_style );
	if( xlc_count == 0 )
	    XlcpConversion( OFF );
    }
    XUnmapWindow( display, p->input_window );
    if( p->buffer->conversion_style & XIMPreeditPosition )
	XmwPreeditUnmapWidget( p->preedit_widget );
    SendConversionEnd( p );
    p->connect_flag = False;
    p->buffer->connect--;

    return( True );
}


Boolean
#if NeedFunctionPrototypes
xlcp_all_end( Window w )
#else
xlcp_all_end( w )
    Window	w;
#endif
{
    PropertyQueue	*p;

    if( w != NULL  &&  search_client_window( w ) == NULL )
	return( False );
    for( p = property_queue; p != NULL; p = p->next ) {
	if( p->connect_flag ) {
	    p->conversion_close_flag = False;
	    xlcp_end( XtDisplay(TopLevel), p->client_window );
	}
	else if( p->conversion_close_flag ) {
	    p->conversion_close_flag = False;
	    xlc_count--;
	    RootWindowUnmap( p->buffer->conversion_style );
	    if( xlc_count == 0 )
		XlcpConversion( OFF );
	}
    }
    return( True );
}



static void
#if NeedFunctionPrototypes
SendConversionNotify( register PropertyQueue *p )
#else
SendConversionNotify( p )
    register PropertyQueue	*p;
#endif
{
    XClientMessageEvent	event;

    event.type = ClientMessage;
    event.display = XtDisplay(TopLevel);
    event.window = p->client_window;
    event.send_event = True;
    event.message_type = conversion_notify;
    event.format = 32;
    event.data.l[0] = xlcp_selection;
    event.data.l[1] = p->client_text;
    event.data.l[2] = p->client_property;
    event.data.l[3] = p->input_window;
    event.data.l[4] = p->client_inplace;
    XSendEvent( XtDisplay(TopLevel), p->client_window, False, NULL,
		(XEvent *)&event );
}



static PropertyQueue *
#if NeedFunctionPrototypes
add_property_queue( Window w, Window client, Atom text, Atom property,
		    Atom inplace )
#else /* NeedFunctionPrototypes */
add_property_queue( w, client, text, property, inplace )
    Window	w, client;
    Atom	text, property, inplace;
#endif /* NeedFunctionPrototypes */
{
    PropertyQueue	*p1, *p2;

    p1 = (PropertyQueue *)calloc( 1, sizeof(PropertyQueue) );

    for( p2 = property_queue; p2 != NULL; p2 = p2->next ) {
	if( p2->next == NULL ) {
	    p2->next = p1;
	    p1->previous = p2;
	    break;
	}
    }
    if( p2 == NULL )
	property_queue = p1;

    p1->input_window = w;
    p1->client_window = client;
    p1->client_text = text;
    p1->client_property = property;
    p1->client_inplace = inplace;

    if( inplace == NULL ) {
	p1->buffer = &root_window;
	p1->preedit_widget = RootText;
    }
    else {
	p1->buffer = buffer_alloc();
	p1->buffer->conversion_style = XIMPreeditPosition | XIMStatusNothing;
	p1->buffer->label = RootLabel;
	p1->buffer->box[0] = Box[0];
	p1->buffer->box[1] = Box[1];
	p1->info = (InplaceInfo *)calloc( 1, sizeof(InplaceInfo) );
    }

    return( p1 );
}


static void
#if NeedFunctionPrototypes
ConversionRequest( XClientMessageEvent *event )
#else
ConversionRequest( event )
    XClientMessageEvent	*event;
#endif
{
    Display		*display = event->display;
    Window		 w;
    PropertyQueue	*p;

    if( (p = search_client_window( (Window)event->data.l[1] )) == NULL ) {
	if( IsRegisteredWindow( XLC_PROTOCOL, (Window)event->data.l[1], NULL ) )
	    return;
	if( (Atom)event->data.l[2] != compound_text )
	    return;

	w = XCreateWindow( display, (Window)event->data.l[1],
			   0, 0, 10000, 10000, 0, 0, InputOnly, NULL, 0, NULL );

	p = add_property_queue( w, (Window)event->data.l[1],
			        (Atom)event->data.l[2], (Atom)event->data.l[3],
			        (Atom)event->data.l[4] );

	XSelectInput( display, w, KeyPressMask );
	XSelectInput( display, p->client_window, StructureNotifyMask );
    }
    else {
	if( (Atom)event->data.l[2] != compound_text )
	    return;

	p->client_property = event->data.l[3];
	p->client_inplace = event->data.l[4];
    }

    if( p->client_property == NULL )
	p->client_property = conversion_property;
    if( !p->connect_flag ) {
	p->connect_flag = True;
	p->buffer->connect++;

	xlcp_start( p );
	XMapWindow( display, p->input_window );
	if( (p->buffer->conversion_style & XIMPreeditPosition)  &&
	    p->preedit_widget ) {
	    XmwPreeditMapWidget( p->preedit_widget );
	    XtVaSetValues( p->preedit_widget, XmwNfocus, True, NULL );
	    buffer_initialize( p->buffer );
	}
	SendConversionNotify( p );
    }

    XFlush( display );
    set_buffer( p->buffer->preedit_widget, KeyPress );
}


static void
#if NeedFunctionPrototypes
ConversionEndRequest( XClientMessageEvent *event )
#else
ConversionEndRequest( event )
    XClientMessageEvent	*event;
#endif
{
    register PropertyQueue	*p;

    if( (p = search_client_window( (Window)event->data.l[1] )) != NULL  &&
       p->connect_flag )
	xlcp_end( event->display, p->client_window );
}


static void
#if NeedFunctionPrototypes
ConversionClose( XClientMessageEvent *event )
#else
ConversionClose( event )
    XClientMessageEvent	*event;
#endif
{
    register PropertyQueue	*p;
    Window			w;

    w = event->data.l[1];
    if( (p = search_client_window( w )) == NULL )
	return;
    if( p->connect_flag ) {
	p->conversion_close_flag = True;
	xlcp_end( event->display, p->client_window );
    }
}


static void
#if NeedFunctionPrototypes
proc_clientmsg( XClientMessageEvent *event )
#else
proc_clientmsg( event )
    XClientMessageEvent	*event;
#endif
{
#ifdef	DEBUG
    Window	w;

    w = event->data.l[1];
#endif
    if( event->message_type == conversion_request ) {
#ifdef	DEBUG
	if( resources.debug & 4 )
	    printf( "ConversionRequest [%x]\n", w );
#endif
	ConversionRequest( event );
    } else if( event->message_type == conversion_end_request ) {
#ifdef	DEBUG
	if( resources.debug & 4 )
	    printf( "ConversionEndRequest [%x]\n", w );
#endif
	if( property_queue != NULL );
	    ConversionEndRequest( event );
    } else if( event->message_type == conversion_close ) {
#ifdef	DEBUG
	if( resources.debug & 4 )
	    printf( "ConversionClose [%x]\n", w );
#endif
	ConversionClose( event );
    }
}


static void
#if NeedFunctionPrototypes
procNoEventMask( Widget w, XtPointer client_data, XEvent *event,
		 Boolean *continue_to_dispatch )
#else
procNoEventMask( w, client_data, event, continue_to_dispatch )
    Widget	 w;
    XtPointer	 client_data;
    XEvent	*event;
    Boolean	*continue_to_dispatch;
#endif
{
    switch( event->type ) {
      case SelectionClear:
	if( event->xselectionclear.selection == xlcp_selection ) {
	    fprintf( stderr, "%s : selection clear event occured.\n\r",
		     program_name );
	    done();
	}
	break;

      case ClientMessage:
	proc_clientmsg( (XClientMessageEvent *)event );
	break;
    }
}


static void
#if NeedFunctionPrototypes
set_xlc_selection_owner( Display *d, Window w, Time time )
#else
set_xlc_selection_owner( d, w, time )
    Display	*d;
    Window	 w;
    Time	 time;
#endif
{
    xlcp_selection = XInternAtom( d, JAPANESE_CONVERSION, False );
    XSetSelectionOwner( d, xlcp_selection, w, time );
}


static void
#if NeedFunctionPrototypes
XlcpPropertyChange( XPropertyEvent *event )
#else
XlcpPropertyChange( event )
    XPropertyEvent	*event;
#endif
{
    Display		*display = event->display;
    PropertyQueue	*p;
    Arg			 args[30];
    int			 n;
    Atom		 actual_type;
    int			 actual_format;
    unsigned long	 nitems, bytes_after;
    long		*prop;
    char		*font0, *font1;
    XWindowAttributes	 attr;

    if( event->atom == conversion_inplace ) {
	XGetWindowProperty( display, event->window, conversion_inplace,
			    0, 10000, True, conversion_inplace,
			    &actual_type, &actual_format, &nitems, &bytes_after,
			    (unsigned char **)&prop );

	if( actual_type == None  &&  actual_format == 0  &&
	    bytes_after == 0 )
	    return;

	if( (p = search_client_window( prop[1] )) == NULL  ||
	   p->info == NULL ) {
	    XFree( prop );
	    return;
	}

	n = 0;
	p->info->in_flag = prop[0];
	p->info->in_win = prop[1];
	if( prop[0] & AllInformation ) {
	    p->info->in_attr.back = prop[2];
	    p->info->in_attr.border = prop[3];
	    p->info->in_attr.bwidth = prop[4];
	    p->info->in_draw.fore = prop[5];
	    p->info->in_draw.back = prop[6];
	    p->info->in_draw.font8 = prop[7];
	    p->info->in_draw.font16 = prop[8];
	    p->info->in_draw.efont16 = prop[9];

	    font0 = fontid2xlfd( display, prop[7] );
	    font1 = fontid2xlfd( display, prop[8] );
	    if( font0 != NULL ) {
		if( font1 != NULL ) {
		    font0 = realloc( font0, strlen(font1) + 2 );
		    strcat( font0, "," );
		    strcat( font0, font1 );
		    XFree( font1 );
		}
	    }
	    else {
		font0 = font1;
	    }

	    XtSetArg( args[n], XtNbackground, prop[2] );	n++;
	    XtSetArg( args[n], XtNborderColor, prop[3] );	n++;
	    XtSetArg( args[n], XtNborderWidth, prop[4] );	n++;
	    XtSetArg( args[n], XtNforeground, prop[5] );	n++;
	    if( font0 ) {
		XtSetArg( args[n], XtNfontSet, font0 );		n++;
		XFree( font0 );
	    }
	}
	if( prop[0] & (AllInformation | FrameInformation) ) {
	    p->info->in_frame.x = prop[10];
	    p->info->in_frame.y = prop[11];
	    p->info->in_frame.width = prop[12];
	    p->info->in_frame.height = prop[13];
	    p->info->in_frame.line_height = prop[16];

	    if( prop[12] == 0  ||  prop[13] == 0 ) {
		XGetWindowAttributes( display, prop[1], &attr );
		p->info->in_frame.width = prop[12] = attr.width;
		p->info->in_frame.height = prop[13] = attr.height;
	    }

	    XtSetArg( args[n], XmwNareaX, prop[10] );		n++;
	    XtSetArg( args[n], XmwNareaY, prop[11] );		n++;
	    XtSetArg( args[n], XmwNareaWidth, prop[12] );	n++;
	    XtSetArg( args[n], XmwNareaHeight, prop[13] );	n++;
	}
	if( prop[0] &
	    (AllInformation | FrameInformation | OffsetInformation) ) {
	    p->info->in_frame.x_off = prop[14];
	    p->info->in_frame.y_off = prop[15];

	    XtSetArg( args[n], XmwNspotLocationX, prop[14] );	n++;
	    XtSetArg( args[n], XmwNspotLocationY, prop[15] );	n++;
	}
	if( prop[0] & (AllInformation | FrameInformation) ) {
	    XtSetArg( args[n], XmwNlineSpacing, prop[16] );	n++;
	}

	if( p->preedit_widget == NULL ) {
	    static int	id = 0;
	    char	ss[20];

	    XGetWindowAttributes( display, prop[1], &attr );

	    XtSetArg( args[n], XmwNfocusWindow, p->info->in_win );	n++;
	    XtSetArg( args[n], XmwNblinkRate, 0 );			n++;
	    XtSetArg( args[n], XmwNfocus, True );			n++;
	    XtSetArg( args[n], XtNdepth, attr.depth );			n++;
	    XtSetArg( args[n], XtNscreen, attr.screen );		n++;
	    XtSetArg( args[n], XtNcolormap, attr.colormap );		n++;
	    XtSetArg( args[n], XtNaccelerators, resources.base_accelerators );
	    n++;
	    sprintf( ss, "xlc_pre%08x", id++ );
	    p->preedit_widget = p->buffer->preedit_widget =
		XmwCreatePreedit( TopLevel, ss, args, n );
	    XtInstallAccelerators( p->preedit_widget, p->preedit_widget );
	    XmwPreeditMapWidget( p->preedit_widget );
	    XMapRaised( display, p->input_window );
	    XSelectInput( display, p->info->in_win,
			  ExposureMask | StructureNotifyMask );
	    SendConversionNotify( p );
	}
	else
	    XtSetValues( p->preedit_widget, args, n );

	XFree( prop );
    }
}


static void
#if NeedFunctionPrototypes
procPropertyChangeMask( Widget w, XtPointer client_data, XEvent *event,
			Boolean *continue_to_dispatch )
#else
procPropertyChangeMask( w, client_data, event, continue_to_dispatch )
    Widget	 w;
    XtPointer	 client_data;
    XEvent	*event;
    Boolean	*continue_to_dispatch;
#endif
{
    static Boolean	 init = False;
    XPropertyEvent	*ev = (XPropertyEvent *)event;

    if( resources.xlc_protocol ) {
	if( !init ) {
	    set_xlc_selection_owner( XtDisplay(w), XtWindow(w), ev->time );
	    init = True;
	}
	else if( ev->state == PropertyNewValue )
	    XlcpPropertyChange( ev );
    }
}


void	xlc_protocol_initialize( display )
    Display	*display;
{
    if( !resources.xlc_protocol )
	return;

    XtAddEventHandler( TopLevel, NoEventMask, True, procNoEventMask, NULL );
    XtAddEventHandler( TopLevel, PropertyChangeMask, False,
		       procPropertyChangeMask, NULL );

    /* setup properties */
    conversion_request = XInternAtom( display, CONVERSION_REQUEST, False );
    conversion_notify = XInternAtom( display, CONVERSION_NOTIFY, False );
    conversion_end_request = XInternAtom( display, CONVERSION_END_REQUEST,
					  False );
    conversion_end = XInternAtom( display, CONVERSION_END, False );
    conversion_close = XInternAtom( display, CONVERSION_CLOSE, False );
    conversion_property = XInternAtom( display, CONVERSION_PROPERTY, False );
    conversion_inplace = XInternAtom( display, CONVERSION_INPLACE, False );
    compound_text = XInternAtom( display, COMPOUND_TEXT, False );
}


static void
#if NeedFunctionPrototypes
delete_property_queue( PropertyQueue *p )
#else
delete_property_queue( p )
    PropertyQueue	*p;
#endif
{
    if( p->previous )
	p->previous->next = p->next;
    else
	property_queue = p->next;
    if( p->next )
	p->next->previous = p->previous;

    if( p->connect_flag ) {
	if( p->buffer->error_shell )
	    error_cancel( (Widget)NULL, (caddr_t)p->buffer, (caddr_t)NULL );
	if( p->buffer->state & DELETE )
	    sakujo_cancel( (Widget)NULL, (caddr_t)p->buffer, (caddr_t)NULL );
	if( p->buffer->state & CODE )
	    code_cancel( (Widget)NULL, (caddr_t)p->buffer, (caddr_t)NULL );
	if( p->buffer->state & ADD )
	    touroku_cancel( (Widget)NULL, (caddr_t)p->buffer, (caddr_t)NULL );
	if( p->buffer->state & SERVER )
	    reconnect_cancel( (Widget)NULL, (caddr_t)p->buffer, (caddr_t)NULL );
	if( p->buffer->state & SELECT ) {
	    if( p->buffer->conversion_style ==
		(XIMPreeditNothing | XIMStatusNothing) )
		XtUnmapWidget( RootTextField );
	    else if( p->buffer->conversion_style & XIMPreeditPosition )
		select_dialog_cancel( (Widget)NULL, (caddr_t)p->buffer,
				      (caddr_t)NULL );
	}
	if( p->buffer->state & EDIT )
	    edit_cancel( p->buffer );
	if( p->buffer->state & (WRAP | CONVERSION) )
	    henkan_cancel( p->buffer );
    }
    if( p->info ) {
	if( buffer == p->buffer )
	    sort_buffer( &root_window );
	free( (char *)p->info );
	buffer_free( p->buffer );
    }
    else
	p->buffer->connect--;
    free( (char *)p );
}


Boolean	XlcpSendCompoundText( display, w, string )
    Display	*display;
    Window	 w;
    wchar_t	*string;
{
    register PropertyQueue	*p;
    XTextProperty		 text;
    wchar_t			*list[1];

    if( (p = search_client_window( w )) == NULL )
	return( False );
    list[0] = string;
    XwcTextListToTextProperty( display, list, 1, XCompoundTextStyle, &text );
    XChangeProperty( display, p->client_window, p->client_property,
		     p->client_text, 8, PropModeAppend,
		     text.value, text.nitems );
    XFree( text.value );

    if( p->buffer->conversion_style & XIMPreeditNothing )
	XmwTextOutSetString( RootText, NULL, XmwHIGHLIGHT_NORMAL );
    else {
	if( p->info->in_flag & 1 )
	    XmwPreeditAutoReplace( p->preedit_widget );
	else
	    XmwPreeditSetString( p->preedit_widget, NULL,
				 XmwHIGHLIGHT_SECONDARY_SELECTED );
    }

    return( True );
}


Boolean	XlcpSendKeyEvent( w, event )
    Window	 w;
    XKeyEvent	*event;
{
    register PropertyQueue	*p;

    p = search_client_window( w );
    if( p != NULL ) {
	event->window = p->client_window;
	XSendEvent( event->display, p->client_window, False, NULL,
		    (XEvent *)event );
	return( True );
    }
    return( False );
}


void	XlcpSetBCKey( widget )
    Widget	widget;
{
    Display		*display = XtDisplay(widget);
    Window		 window = XtWindow(widget);
    Atom		 kprop, mprop;
    unsigned int	 keycode, modifier;

    if( !resources.xlc_protocol )
	return;
    XlcpConversion( OFF );
    if( resources.process_start_keys[0].keysym == XK_VoidSymbol )
	return;

    kprop = XInternAtom( display, "_XLC_BC_KEYCODE", False );
    mprop = XInternAtom( display, "_XLC_BC_MODIFIER", False );

    keycode = XKeysymToKeycode( display,
				resources.process_start_keys[0].keysym );
    modifier = resources.process_start_keys[0].modifier;

    XChangeProperty( display, window, kprop, XA_INTEGER, 32,
		     PropModeReplace, (unsigned char *)&keycode, 1 );
    XChangeProperty( display, window, mprop, XA_INTEGER, 32,
		     PropModeReplace, (unsigned char *)&modifier, 1 );
}


Boolean
#if NeedFunctionPrototypes
XlcpCheckState( State state )
#else /* NeedFunctionPrototypes */
XlcpCheckState( state )
    State	state;
#endif /* NeedFunctionPrototypes */
{
    PropertyQueue	*q;

    for( q = property_queue; q != NULL; q = q->next )
	if( q->buffer->state & state )
	    return( True );
    return( False );
}


Boolean
#if NeedFunctionPrototypes
XlcpDestroyNotify( XEvent *event )
#else /* NeedFunctionPrototypes */
XlcpDestroyNotify( event )
    XEvent	*event;
#endif /* NeedFunctionPrototypes */
{
    register PropertyQueue	*p;

    if( (p = search_client_window(event->xdestroywindow.window)) != NULL ) {
	if( p->connect_flag  ||  p->conversion_close_flag ) {
	    RootWindowUnmap( p->buffer->conversion_style );
	    xlc_count--;
	    if( xlc_count == 0 )
		XlcpConversion( OFF );
	}
	delete_property_queue( p );
	return( True );
    }
    else
	return( False );
}


Boolean
#if NeedFunctionPrototypes
XlcpChangeExpose( XEvent *event )
#else /* NeedFunctionPrototypes */
XlcpChangeExpose( event )
    XEvent	*event;
#endif /* NeedFunctionPrototypes */
{
    PropertyQueue	*p;

    for( p = property_queue; p != NULL; p = p->next ) {
	if( p->info  &&  p->info->in_win == event->xexpose.window ) {
	    event->xexpose.window = XtWindow( p->preedit_widget );
	    return( True );
	}
    }
    return( False );
}


Boolean
#if NeedFunctionPrototypes
XlcpPreeditChangeKeyDirection( XEvent *event )
#else /* NeedFunctionPrototypes */
XlcpPreeditChangeKeyDirection( event )
    XEvent	*event;
#endif /* NeedFunctionPrototypes */
{
    PropertyQueue	*q;

    if( (q = search_client_window( event->xkey.window )) == NULL )
	return( False );

    event->xkey.window = XtWindow( q->preedit_widget );
    XtSetKeyboardFocus( TopLevel, q->preedit_widget );
    return( True );
}


Boolean
#if NeedFunctionPrototypes
XlcpIsRegisteredWindow( Window c, Window f )
#else /* NeedFunctionPrototypes */
XlcpIsRegisteredWindow( c, f )
    Window	c, f;
#endif /* NeedFunctionPrototypes */
{
    PropertyQueue	*q;

    for( q = property_queue; q != NULL; q = q->next )
	if( q->client_window == c  ||  q->client_window == f )
	    return( True );
    return( False );
}
#endif
