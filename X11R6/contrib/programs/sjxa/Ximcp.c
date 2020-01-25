/* $SonyId: Ximcp.c,v 1.2 1994/06/03 06:33:04 makoto Exp $ */
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
#include	<X11/Xlib.h>
#if defined(USE_XIMCP)
#define NEED_EVENTS
#include	<stdio.h>
#include	<X11/keysym.h>
#include	<X11/Intrinsic.h>
#include	<X11/StringDefs.h>
#include	<X11/Xatom.h>
#include	<X11/Xlibint.h>
#include	<Xmw/Label.h>
#include	<Xmw/List.h>
#include	<Xmw/Preedit.h>
#include	<Xmw/StsArea.h>
#include	"XimcpP.h"
#include	"resource.h"
#include	"SjBuffer.h"
#include	"SjXkey.h"

#define	MAXSHORT	65535
#define XIMCP_BUFSIZE	2048

#define Pad(n)	((4 - ((n) % 4)) % 4)

static Atom	xim_selection;

static Atom	xim_servers;
static Atom	xim_transport;
static Atom	xim_locales;

static Atom	xim_connect;
static Atom	xim_protocol;

static Atom	xim_open;
static Atom	xim_register_triggerkeys;
static Atom	xim_query_extension;
static Atom	xim_im_values;
static Atom	xim_ic_values;
static Atom	xim_commit;
static Atom	xim_reset;
static Atom	xim_forward_event;

#define XIM_TRANSPORT_REPLY	"@transport=X/"
#define XIM_LOCALES_REPLY	"@locale=ja,ja_JP"

#define XimInitNone			0x00
#define XimInitOpen			0x01
#define XimInitEncodingNegotiation	0x02
#define XimInitQueryExtension		0x04
#define XimInitDone			0x07

static XimcpConnection	*connection = NULL;

static XimcpAttrRec im_attr[] = {
    { XNQueryInputStyle, XIM_QUERY_INPUT_STYLE, XimRimStyles, 0, 0, NULL },
    { 0, 0, 0, NULL }};

#define Offset(f)	(Cardinal)&(((XimcpInputContext *)NULL))->preedit.f
static XimcpAttrRec ic_preedit_attr[] = {
    { XNArea, XIC_AREA, XimRrectangle,
      sizeof(XRectangle), Offset(area), NULL },
    { XNAreaNeeded, XIC_AREA_NEEDED, XimRrectangle,
      sizeof(XRectangle), Offset(area_needed), NULL },
    { XNSpotLocation, XIC_SPOT_LOCATION, XimRpoint,
      sizeof(XPoint), Offset(spot_location), NULL },
    { XNColormap, XIC_COLORMAP, XimRlongData,
      sizeof(Colormap), Offset(colormap), NULL },
    { XNForeground, XIC_FOREGROUND, XimRlongData,
      sizeof(unsigned long), Offset(foreground), NULL },
    { XNBackground, XIC_BACKGROUND, XimRlongData,
      sizeof(unsigned long), Offset(foreground), NULL },
    { XNBackgroundPixmap, XIC_BACKGROUND_PIXMAP, XimRlongData,
      sizeof(Pixmap), Offset(pixmap), NULL },
    { XNFontSet, XIC_FONTSET, XimRfontSet,
      0, Offset(font_set), NULL },
    { XNLineSpace, XIC_LINE_SPACING, XimRlongData,
      sizeof(int), Offset(line_spacing), NULL },
    { XNCursor, XIC_CURSOR, XimRlongData,
      sizeof(Cursor), Offset(cursor), NULL },
    { XNPreeditState, XIC_PREEDIT_STATE, XimRimPreeditState,
      sizeof(unsigned long), Offset(state), NULL },
    { XNSeparatorofNestedList, XIC_SEPARATOR_OF_NESTED_LIST,
      XimRseparatorOfNestedList, 0, 0, NULL },
    { 0, 0, 0, 0, 0, NULL }};

#undef Offset
#define Offset(f)	(Cardinal)&(((XimcpInputContext *)NULL))->status.f
static XimcpAttrRec ic_status_attr[] = {
    { XNArea, XIC_AREA, XimRrectangle,
      sizeof(XRectangle), Offset(area), NULL },
    { XNAreaNeeded, XIC_AREA_NEEDED, XimRrectangle,
      sizeof(XRectangle), Offset(area_needed), NULL },
    { XNColormap, XIC_COLORMAP, XimRlongData,
      sizeof(Colormap), Offset(colormap), NULL },
    { XNForeground, XIC_FOREGROUND, XimRlongData,
      sizeof(unsigned long), Offset(foreground), NULL },
    { XNBackground, XIC_BACKGROUND, XimRlongData,
      sizeof(unsigned long), Offset(foreground), NULL },
    { XNBackgroundPixmap, XIC_BACKGROUND_PIXMAP, XimRlongData,
      sizeof(Pixmap), Offset(pixmap), NULL },
    { XNFontSet, XIC_FONTSET, XimRfontSet,
      0, Offset(font_set), NULL },
    { XNCursor, XIC_CURSOR, XimRlongData,
      sizeof(Cursor), Offset(cursor), NULL },
    { XNSeparatorofNestedList, XIC_SEPARATOR_OF_NESTED_LIST,
      XimRseparatorOfNestedList, 0, 0, NULL },
    { 0, 0, 0, 0, 0, NULL }};

#undef Offset
#define Offset(f)	(Cardinal)&(((XimcpInputContext *)NULL))->f
static XimcpAttrRec ic_attr[] = {
    { XNInputStyle, XIC_INPUT_STYLE, XimRlongData,
      sizeof(unsigned long), Offset(input_style), NULL },
    { XNClientWindow, XIC_CLIENT_WINDOW, XimRwindow,
      sizeof(Window), Offset(client_window), NULL },
    { XNFocusWindow, XIC_FOCUS_WINDOW, XimRwindow,
      sizeof(Window), Offset(focus_window), NULL },
    { XNFilterEvents, XIC_FILTER_EVENTS, XimRlongData,
      sizeof(long), Offset(filter_events), NULL },
    { XNResetState, XIC_RESET_STATE, XimRimResetState,
      sizeof(unsigned long), Offset(reset_state), NULL },
    { XNHotKey, XIC_HOT_KEY, XimRimHotKeyTriggers,
      sizeof(XIMHotKeyTriggers), Offset(hot_key), NULL },
    { XNHotKeyState, XIC_HOT_KEY_STATE, XimRimHotKeyState,
      sizeof(unsigned long), Offset(hot_key_state), NULL },
    { XNStringConversion, XIC_STRING_CONVERSION, XimRimStringConversion,
      0, 0, NULL },
    { XNPreeditAttributes, XIC_PREEDIT_ATTRIBUTES, XimRnestedList,
      0, 0, ic_preedit_attr },
    { XNStatusAttributes, XIC_STATUS_ATTRIBUTES, XimRnestedList,
      0, 0, ic_status_attr },
    { 0, 0, 0, 0, 0, NULL }};

static XimcpExtRec extension[] = {
    /*{ XIM_EXT_SET_EVENT_MASK, 0, "XIM_EXT_SET_EVENT_MASK" },*/
    /*{ XIM_EXT_FORWARD_KEYEVENT, 0, "XIM_EXT_FORWARD_KEYEVENT" },*/
    { XIM_EXT_MOVE, 0, "XIM_EXT_MOVE" },
    { 0, 0, NULL }};

static CARD16	*attr_list = NULL;
static int	 attr_size = 0;
static CARD16	*triggerkeys = NULL;
static int	 triggerkeys_size = 0;
static CARD32	 input_styles[] = {
    XIMPreeditPosition | XIMStatusArea,
    XIMPreeditArea     | XIMStatusArea,
    XIMPreeditPosition | XIMStatusNothing,
    XIMPreeditArea     | XIMStatusNothing,
    XIMPreeditNothing  | XIMStatusNothing,
    0
};
static Endian	current_endian, last_endian;


extern char		*program_name;
extern Sjxa		 resources;
extern Endian		 endian;
extern Buffer		 root_window;
extern Widget		 TopLevel, RootTextField;
extern Widget		 delete_shell, question_shell, server_shell;
extern unsigned long	 default_foreground;
extern unsigned long	 default_background;
extern int		*depths;

extern void		 sjxa_connect();
extern void		 sjxa_disconnect();
extern void		 function_activate();


static CARD16
#if NeedFunctionPrototypes
swap16( CARD16 s )
#else
swap16( s )
    CARD16	s;
#endif
{
    CARD8	*d;

    if( endian == current_endian )
	return( s );
    else {
	d = (CARD8 *)&s;
	return( (d[0] << 8) | d[1] );
    }
}


static CARD32
#if NeedFunctionPrototypes
swap32( CARD32 s )
#else
swap32( s )
    CARD32	s;
#endif
{
    CARD8	*d;

    if( endian == current_endian )
	return( s );
    else {
	d = (CARD8 *)&s;
	return( (d[0] << 24) | (d[1] << 16) | (d[2] << 8) | d[3] );
    }
}


static void
#if NeedFunctionPrototypes
XimSendClientMessage( Display *display, XimcpConnection *xc, CARD8 protocol,
		      CARD16 length, CARD8 *data )
#else
XimSendClientMessage( display, xc, protocol, length, data )
    Display		*display;
    XimcpConnection	*xc;
    CARD8		 protocol;
    CARD16		 length;
    CARD8		*data;
#endif
{
    XClientMessageEvent	ev;

    bzero( (char *)&ev, sizeof(XClientMessageEvent) );

    ev.type = ClientMessage;
    ev.window = xc->client_window;
    ev.message_type = xim_protocol;
    ev.format = 8;
    ev.data.b[0] = protocol;
    ev.data.b[1] = 0;
    ev.data.s[1] = xc->bug ? swap16( length + Pad(length) )
			   : swap16( (length + 3) / 4 );
    if( length > 0 )
	memcpy( &ev.data.s[2], data, length );

    XSendEvent( display, xc->client_window, False, 0, (XEvent *)&ev );
}


static void
#if NeedFunctionPrototypes
XimSendProperty( Display *display, XimcpConnection *xc, Atom atom,
		 CARD16 length, CARD8 *data )
#else
XimSendProperty( display, xc, atom, length, data )
    Display		*display;
    XimcpConnection	*xc;
    Atom		 atom;
    CARD16		 length;
    CARD8		*data;
#endif
{
    XClientMessageEvent	ev;

    length = ((length + 3) / 4) * 4;

    *((CARD16*)&data[2]) = swap16( xc->bug ? length - 4 : (length - 4) / 4 );
    XChangeProperty( display, xc->client_window, atom, XA_STRING, 8,
		     PropModeAppend, data, length );

    ev.type = ClientMessage;
    ev.window = xc->client_window;
    ev.message_type = xim_protocol;
    ev.format = 32;
    ev.data.l[0] = swap32( length );
    ev.data.l[1] = swap32( atom );
    XSendEvent( display, xc->client_window, False, 0, (XEvent *)&ev );
}


static void
#if NeedFunctionPrototypes
XimSendErrorMessage( Display *display, XimcpConnection *xc, CARD16 imid,
		     CARD16 icid, CARD16 error_code )
#else
XimSendErrorMessage( display, xc, imid, icid, error_code )
    Display		*display;
    XimcpConnection	*xc;
    CARD16		 imid;
    CARD16		 icid;
    CARD16		 error_code;
#endif
{
    XClientMessageEvent	ev;

    ev.type = ClientMessage;
    ev.window = xc->client_window;
    ev.message_type = xim_protocol;
    ev.format = 8;
    ev.data.b[0] = XIM_ERROR;
    ev.data.b[1] = 0;
    ev.data.s[1] = swap16( xc->bug ? 12 : 12/4 );
    ev.data.s[2] = swap16( imid );
    ev.data.s[3] = swap16( icid );
    ev.data.s[4] = swap16( (imid ? 1 : 0) | (icid ? 2 : 0) );
    ev.data.s[5] = swap16( error_code );
    ev.data.s[6] = 0;

    XSendEvent( display, xc->client_window, False, 0, (XEvent *)&ev );
}



static XimcpInputMethod *
#if NeedFunctionPrototypes
imid2xim( Display *display, XimcpConnection *xc, CARD16 imid, Boolean error )
#else
imid2xim( display, xc, imid, error )
    Display		*display;
    XimcpConnection	*xc;
    CARD16		 imid;
    Boolean		 error;
#endif
{
    XimcpInputMethod	*xim;

    for( xim = xc->xim; xim != NULL; xim = xim->next )
	if( xim->imid == imid )
	    return( xim );
    if( error )
	XimSendErrorMessage( display, xc, imid, 0, XIM_ERROR_BAD_PROTOCOL );
    return( NULL );
}


static XimcpInputContext *
#if NeedFunctionPrototypes
icid2xic( Display *display, XimcpInputMethod *xim, CARD16 icid, Boolean error )
#else
icid2xic( display, xim, icid, error )
    Display		*display;
    XimcpInputMethod	*xim;
    CARD16		 icid;
    Boolean		 error;
#endif
{
    XimcpInputContext	*xic;

    for( xic = xim->xic; xic != NULL; xic = xic->next )
	if( xic->icid == icid )
	    return( xic );
    if( error )
	XimSendErrorMessage( display, xim->xc, xim->imid, icid,
			     XIM_ERROR_BAD_PROTOCOL );
    return( NULL );
}


static XimcpInputContext *
#if NeedFunctionPrototypes
window2xic( Window f )
#else
window2xic( f )
    Window	f;
#endif
{
    XimcpConnection	*xc;
    XimcpInputMethod	*xim;
    XimcpInputContext	*xic1, *xic2;

    for( xic1 = NULL, xc = connection; xc != NULL; xc = xc->next ) {
	for( xim = xc->xim; xim != NULL; xim = xim->next ) {
	    for( xic2 = xim->xic; xic2 != NULL; xic2 = xic2->next ) {
		if( (((xic2->mask & (1L << XIC_FOCUS_WINDOW))  ||
		      (xic2->mask & (1L << XIC_CLIENT_WINDOW)))  &&
		     xic2->focus_window == f)  ||
		     XtWindow(xic2->buffer->preedit_widget) == f ) {
		    if( xic2->has_focus )
			return( xic2 );
		    xic1 = xic2;
		}
	    }
	}
    }
    return( xic1 );
}


static void
#if NeedFunctionPrototypes
destroy_ic( XimcpInputContext *xic )
#else
destroy_ic( xic )
    XimcpInputContext	*xic;
#endif
{
    if( xic->preedit.state == XIMPreeditEnable ) {
	if( xic->input_style & (XIMPreeditNothing | XIMStatusNothing) )
	    RootWindowUnmap( xic->input_style );

	if( xic->buffer->error_shell )
	    error_cancel( (Widget)NULL, (caddr_t)xic->buffer, (caddr_t)NULL );

	if( xic->buffer->state & DELETE )
	    sakujo_cancel( (Widget)NULL, (caddr_t)xic->buffer, (caddr_t)NULL );
	if( xic->buffer->state & CODE )
	    code_cancel( (Widget)NULL, (caddr_t)xic->buffer, (caddr_t)NULL );
	if( xic->buffer->state & ADD )
	    touroku_cancel( (Widget)NULL, (caddr_t)xic->buffer, (caddr_t)NULL );
	if( xic->buffer->state & SERVER )
	    reconnect_cancel( (Widget)NULL, (caddr_t)xic->buffer,
			      (caddr_t)NULL );
	if( xic->buffer->state & SELECT ) {
	    if( xic->input_style == (XIMPreeditNothing | XIMStatusNothing) ) {
		XtUnmapWidget( RootTextField );
		XtMapWidget( xic->buffer->label );
	    }
	    else if( xic->input_style & (XIMPreeditPosition | XIMPreeditArea) )
		select_dialog_cancel( (Widget)NULL, (caddr_t)xic->buffer,
				      (caddr_t)NULL );
	}
	if( xic->buffer->state & EDIT )
	    edit_cancel( xic->buffer );
	if( xic->buffer->state & (WRAP | CONVERSION) )
	    henkan_cancel( xic->buffer );
    }
    if( xic->preedit.font_set )
	free( xic->preedit.font_set );
    if( xic->status.font_set )
	free( xic->status.font_set );
    if( xic->input_style & (XIMPreeditPosition | XIMPreeditArea) )
	XtDestroyWidget( xic->buffer->preedit_widget );
    if( xic->input_style & XIMStatusArea )
	XtDestroyWidget( xic->buffer->status_widget );
    if( xic->input_style != (XIMPreeditNothing | XIMStatusNothing) )
	buffer_free( xic->buffer );
    free( (char *)xic );
}



Boolean
#if NeedFunctionPrototypes
XimcpCheckState( State state )
#else /* NeedFunctionPrototypes */
XimcpCheckState( state )
    State	state;
#endif /* NeedFunctionPrototypes */
{
    XimcpConnection	*xc;
    XimcpInputMethod	*xim;
    XimcpInputContext	*xic;

    for( xc = connection; xc != NULL; xc = xc->next )
	for( xim = xc->xim; xim != NULL; xim = xim->next )
	    for( xic = xim->xic; xic != NULL; xic = xic->next )
		if( xic->buffer->state & state )
		    return( True );
    return( False );
}



Boolean
#if NeedFunctionPrototypes
XimcpSendCompoundText( Display *display, Window w, wchar_t *string )
#else
XimcpSendCompoundText( display, w, string )
    Display	*display;
    Window	 w;
    wchar_t	*string;
#endif
{
    XimcpInputContext	*xic;
    XTextProperty	 text;
    wchar_t		*list[1];
    CARD8		 reply[XIMCP_BUFSIZE];
    int			 size;

    if( (xic = window2xic( w )) == NULL )
	return( False );

    list[0] = string;
    XwcTextListToTextProperty( display, list, 1, XCompoundTextStyle, &text );
    if( text.nitems >= 3  &&
	!strncmp( (char *)&text.value[text.nitems-3], "\033(B", 3 ) )
	text.nitems -= 3;
    bzero( reply, XIMCP_BUFSIZE );
    memcpy( &reply[12], text.value, text.nitems );
    size = 4 + 8 + text.nitems;
    *((CARD16 *)&reply[4]) = swap16( xic->xim->imid );
    *((CARD16 *)&reply[6]) = swap16( xic->icid );
    *((CARD16 *)&reply[8]) = swap16( 0x0002 );
    *((CARD16 *)&reply[10]) = swap16( text.nitems );
    if( size <= 20 )
	XimSendClientMessage( display, xic->xim->xc, XIM_COMMIT, size - 4,
			      &reply[4] );
    else {
	reply[0] = XIM_COMMIT;
	reply[1] = 0;
	XimSendProperty( display, xic->xim->xc, xim_commit, size, reply );
    }
    XFree( text.value );

    if( xic->input_style & XIMPreeditNothing )
	XmwTextOutSetString( xic->buffer->preedit_widget, NULL );
    else if( xic->input_style & XIMPreeditPosition ) {
	if( resources.auto_replace )
	    XmwPreeditAutoReplace( xic->buffer->preedit_widget );
	else
	    XmwPreeditSetString( xic->buffer->preedit_widget, NULL,
				 XmwHIGHLIGHT_SECONDARY_SELECTED );
    }
    else if( xic->input_style & XIMPreeditArea )
	XmwPreeditSetString( xic->buffer->preedit_widget, NULL,
			     XmwHIGHLIGHT_SECONDARY_SELECTED );

    return( True );
}


Boolean
#if NeedFunctionPrototypes
XimcpSendKeyEvent( Window w, XKeyEvent *key )
#else
XimcpSendKeyEvent( w, key )
    Window	 w;
    XKeyEvent	*key;
#endif
{
    XimcpInputContext	*xic;
    CARD8		 reply[XIMCP_BUFSIZE];

    if( (xic = window2xic( w )) == NULL )
	return( False );
    reply[0] = XIM_FORWARD_EVENT;;
    reply[1] = 0;
    ((CARD16 *)reply)[2] = swap16( xic->xim->imid );
    ((CARD16 *)reply)[3] = swap16( xic->icid );
    ((CARD16 *)reply)[4] = 0;
    ((CARD16 *)reply)[5] = swap16( key->serial >> 16 );

    ((xEvent *)&reply[12])->u.u.type = KeyPress;
    ((xEvent *)&reply[12])->u.u.sequenceNumber = swap16( key->serial & 0xffff );
    ((xEvent *)&reply[12])->u.keyButtonPointer.event =
	swap32( xic->focus_window );
    ((xEvent *)&reply[12])->u.keyButtonPointer.root = swap32( key->root );
    ((xEvent *)&reply[12])->u.keyButtonPointer.time = swap32( key->time );
    ((xEvent *)&reply[12])->u.keyButtonPointer.eventX = swap16( key->x );
    ((xEvent *)&reply[12])->u.keyButtonPointer.eventY = swap16( key->y );
    ((xEvent *)&reply[12])->u.keyButtonPointer.rootX = swap16( key->x_root );
    ((xEvent *)&reply[12])->u.keyButtonPointer.rootY = swap16( key->y_root );
    ((xEvent *)&reply[12])->u.keyButtonPointer.state = swap16( key->state );
    ((xEvent *)&reply[12])->u.u.detail = key->keycode;

    XimSendProperty( key->display, xic->xim->xc, xim_forward_event,
		     12 + sz_xEvent, reply );
    return( True );
}



static void
#if NeedFunctionPrototypes
XimConnect( Display *display, XimcpConnection *xc, CARD data )
#else
XimConnect( display, xc, data )
    Display		*display;
    XimcpConnection	*xc;
    CARD		 data;
#endif
{
    CARD16		reply[2];

    if( data.s[5] != 0 ) {
	XimSendClientMessage( display, xc, XIM_AUTH_NG, 0, NULL );
    }
    else {
	if( swap16(data.s[3]) != 1  ||  swap16(data.s[4]) != 0 ) {
	    XimSendErrorMessage( display, xc, 0, 0, XIM_ERROR_BAD_SOMETHING );
	}
	else {
	    reply[0] = 1;
	    reply[1] = 0;
	    XimSendClientMessage( display, xc, XIM_CONNECT_REPLY, 4,
				  (CARD8 *)reply );
	}
    }
}


static void
#if NeedFunctionPrototypes
XimDisconnect( Display *display, XimcpConnection *xc, CARD data )
#else
XimDisconnect( display, xc, data )
    Display		*display;
    XimcpConnection	*xc;
    CARD		 data;
#endif
{
    XimcpInputMethod	*xim;
    XimcpInputContext	*xic;

    XimSendClientMessage( display, xc, XIM_DISCONNECT_REPLY, 0, NULL );

    if( xc->previous )
	xc->previous->next = xc->next;
    else
	connection = xc->next;
    if( xc->next )
	xc->next->previous = xc->previous;

    for( xim = xc->xim; xim != NULL; xim = xim->next ) {
	for( xic = xim->xic; xic != NULL; xic = xic->next )
	    destroy_ic( xic );
	free( xim );
    }
    XDestroyWindow( display, xc->ims_window );
    free( (char *)xc );
}


static int
#if NeedFunctionPrototypes
create_attr_list( XimcpAttrRec *attr, CARD16 *list )
#else
create_attr_list( attr, list )
    XimcpAttrRec	*attr;
    CARD16		*list;
#endif
{
    int		size = 0;
    int		n;

    while( attr->id != 0 ) {
	list[0] = swap16( attr->id );
	list[1] = swap16( attr->type );
	list[2] = swap16( strlen(attr->name) );
	strcpy( (char *)&list[3], attr->name );

	n = 6 + list[2] + Pad(2 + list[2]);
	size += n;
	list = (CARD16 *)((char *)list + n);

	if( attr->nested_list ) {
	    n = create_attr_list( attr->nested_list, list );
	    size += n;
	    list = (CARD16 *)((char *)list + n);
	}

	attr++;
    }
    return( size );
}


static void
#if NeedFunctionPrototypes
XimOpen( Display *display, XimcpConnection *xc, CARD data )
#else
XimOpen( display, xc, data )
    Display		*display;
    XimcpConnection	*xc;
    CARD		 data;
#endif
{
    XimcpInputMethod	*xim1, *xim2;
    int			 im_size, ic_size;
    CARD16		 im_list[XIMCP_BUFSIZE], ic_list[XIMCP_BUFSIZE];
    XIMTRIGGERKEY	*onkey, *offkey;
    int			 n, m, nn, mm, ii;

    xim1 = (XimcpInputMethod *)malloc( sizeof(XimcpInputMethod) );
    xim1->xc = xc;
    for( n = 1; n <= MAXSHORT; n++ ) {
	for( xim2 = xc->xim; xim2 != NULL; xim2 = xim2->next )
	    if( xim2->imid == n )
		break;
	if( xim2 == NULL )
	    break;
    }
    xim1->imid = n;

    if( !((data.b[4] == 5  &&  !strncmp( (char *)&data.b[5], "ja_JP", 5 ))  ||
	  (data.b[4] == 2  &&  !strncmp( (char *)&data.b[5], "ja", 2 ))) ) {
	XimSendErrorMessage( display, xc, 0, 0,
			     XIM_ERROR_LOCALE_NOT_SUPPORTED );
	free( (char *)xim1 );
	return;
    }

    if( current_endian != last_endian ) {
	free( attr_list );
	free( triggerkeys );
	attr_list = NULL;
	triggerkeys = NULL;
    }

    if( triggerkeys == NULL ) {
	onkey = Action2XimcpKey( sjxa_connect );
	offkey = Action2XimcpKey( sjxa_disconnect );
	n = m = nn = mm = 0;
	if( onkey ) {
	    for( n = 0; onkey[n].keysym != XK_VoidSymbol; n++ );
	    nn = n * sizeof(XIMTRIGGERKEY);
	}
	if( offkey ) {
	    for( m = 0; offkey[m].keysym != XK_VoidSymbol; m++ );
	    mm = m * sizeof(XIMTRIGGERKEY);
	}
	triggerkeys_size = sizeof(CARD8) + sizeof(CARD8) + sizeof(CARD16)
			 + sizeof(CARD16) + 2 + 4 + nn + 4 + mm;
	triggerkeys = (CARD16 *)malloc( triggerkeys_size );
	((CARD8 *)triggerkeys)[0] = XIM_REGISTER_TRIGGERKEYS;
	((CARD8 *)triggerkeys)[1] = 0;
	((CARD32 *)triggerkeys)[2] = swap16( nn );
	for( ii = 0; ii < n; ii++ ) {
	    ((CARD32 *)triggerkeys)[3+ii*3] = swap32( onkey[ii].keysym );
	    ((CARD32 *)triggerkeys)[4+ii*3] = swap32( onkey[ii].modifier );
	    ((CARD32 *)triggerkeys)[5+ii*3]
		= swap32( onkey[ii].modifier_mask );
	}
	((CARD32 *)triggerkeys)[n*3+3] = swap16( mm );
	for( ii = 0; ii < m; ii++, n++ ) {
	    ((CARD32 *)triggerkeys)[4+n*3] = swap32( offkey[ii].keysym );
	    ((CARD32 *)triggerkeys)[5+n*3] = swap32( offkey[ii].modifier );
	    ((CARD32 *)triggerkeys)[6+n*3]
		= swap32( offkey[ii].modifier_mask );
	}
	free( onkey );
	free( offkey );
    }
#ifdef notdef
    triggerkeys[2] = swap16( xim1->imid );
#else
    triggerkeys[2] = 0;
#endif
    XimSendProperty( display, xc, xim_register_triggerkeys, triggerkeys_size,
		     (CARD8 *)triggerkeys );

    if( attr_list == NULL ) {
	im_size = create_attr_list( im_attr, im_list );
	ic_size = create_attr_list( ic_attr, ic_list );
	attr_size = 4 + 2 + 2 + im_size + 2 + 2 + ic_size;
	attr_list = (CARD16 *)malloc( attr_size );
	((CARD8 *)attr_list)[0] = XIM_OPEN_REPLY;
	((CARD8 *)attr_list)[1] = 0;
	attr_list[3] = swap16( im_size );
	memcpy( &attr_list[4], im_list, im_size );
	attr_list[im_size/2+4] = swap16( ic_size );
	memcpy( &attr_list[im_size/2+6], ic_list, ic_size );
    }
    attr_list[2] = swap16( xim1->imid );
    XimSendProperty( display, xc, xim_open, attr_size, (CARD8 *)attr_list );

    xim1->initialize = XimInitOpen;
    xim1->extension = 0;
    xim1->xic = NULL;

    xim1->next = NULL;
    if( xc->xim == NULL ) {
	xc->xim = xim1;
	xim1->previous = NULL;
    }
    else {
	for( xim2 = xc->xim; xim2->next != NULL; xim2 = xim2->next );
	xim2->next = xim1;
	xim1->previous = xim2;
    }
}


static void
#if NeedFunctionPrototypes
XimClose( Display *display, XimcpConnection *xc, CARD data )
#else
XimClose( display, xc, data )
    Display		*display;
    XimcpConnection	*xc;
    CARD		 data;
#endif
{
    XimcpInputMethod	*xim;
    XimcpInputContext	*xic;
    CARD16		 reply[1];

    if( (xim = imid2xim( display, xc, data.s[2], True )) == NULL )
	return;
    reply[0] = swap16( xim->imid );
    XimSendClientMessage( display, xc, XIM_CLOSE_REPLY, 2, (CARD8 *)reply );
    if( xim->previous )
	xim->previous->next = xim->next;
    else
	xc->xim = xim->next;
    if( xim->next )
	xim->next->previous = xim->previous;
    for( xic = xim->xic; xic != NULL; xic = xic->next )
	destroy_ic( xic );
    free( (char *)xim );
}


static void
#if NeedFunctionPrototypes
XimEncodingNegotiation( Display *display, XimcpConnection *xc, CARD data )
#else
XimEncodingNegotiation( display, xc, data )
    Display		*display;
    XimcpConnection	*xc;
    CARD		 data;
#endif
{
    XimcpInputMethod	*xim;
    CARD16		 replay[3];
    int			 n;
    int			 point;

    if( (xim = imid2xim( display, xc, data.s[2], True )) == NULL )
	return;

    replay[0] = swap16( xim->imid );
    replay[1] = 0;
    replay[2] = -1;
    for( n = 0, point = 8; point < data.s[3];
	 point += data.b[point] + Pad(data.b[point]), n++ ) {
	if( strncmp( "COMPOUND_TEXT", (char *)&data.b[point+1],
		     data.b[point] ) ) {
	    replay[2] = n;
	    break;
	}
    }
    XimSendClientMessage( display, xc, XIM_ENCODING_NEGOTIATION_REPLY, 6,
			  (CARD8 *)replay );
    xim->initialize |= XimInitEncodingNegotiation;
}


static void
#if NeedFunctionPrototypes
XimQueryExtension( Display *display, XimcpConnection *xc, CARD data )
#else
XimQueryExtension( display, xc, data )
    Display		*display;
    XimcpConnection	*xc;
    CARD		 data;
#endif
{
    XimcpInputMethod	*xim;
    XimcpExtRec		*ext;
    CARD8		 reply[XIMCP_BUFSIZE];
    int			 size = 8;
    int			 n, m;

    if( (xim = imid2xim( display, xc, data.s[2], True )) == NULL )
	return;

    for( n = 8; n < data.s[3] + 8; n += data.b[n] + Pad(data.b[n]) ) {
	for( ext = extension; ext->major_opcode != 0; ext++ ) {
	    if( strncmp( ext->name, (char *)&data.b[n+1], data.b[n] ) == 0 ) {
		xim->extension |= ext->major_opcode;
		reply[size] = ext->major_opcode;
		reply[size+1] = ext->minor_opcode;
		*((CARD16 *)&reply[size+2]) = swap16( strlen(ext->name) );
		strcpy( (char *)&reply[size+4], ext->name );

		m = 4 + Pad(*((CARD16 *)&reply[size+2]));
		size += m;
		break;
	    }
	}
    }
    *((CARD16 *)&reply[4]) = swap16( xim->imid );
    *((CARD16 *)&reply[6]) = swap16( size - 4 );
    if( size <= 20 )
	XimSendClientMessage( display, xc, XIM_QUERY_EXTENSION_REPLY, size-4,
			      &reply[4] );
    else {
	reply[0] = XIM_QUERY_EXTENSION_REPLY;
	reply[1] = 0;
	XimSendProperty( display, xc, xim_query_extension, size, reply );
    }

    xim->initialize |= XimInitQueryExtension;
}


static void
#if NeedFunctionPrototypes
XimSetImValues( Display *display, XimcpConnection *xc, CARD data )
#else
XimSetImValues( display, xc, data )
    Display		*display;
    XimcpConnection	*xc;
    CARD		 data;
#endif
{
    XimcpInputMethod	*xim;
    CARD16		 reply[1];

    if( (xim = imid2xim( display, xc, data.s[2], True )) == NULL )
	return;

    /* XXXXX */
    reply[0] = swap16( xim->imid );
    XimSendClientMessage( display, xc, XIM_SET_IM_VALUES_REPLY, 2,
			  (CARD8 *)reply );
}


static void
#if NeedFunctionPrototypes
XimGetImValues( Display *display, XimcpConnection *xc, CARD data )
#else
XimGetImValues( display, xc, data )
    Display		*display;
    XimcpConnection	*xc;
    CARD		 data;
#endif
{
    XimcpInputMethod	*xim;
    CARD16		 reply[XIMCP_BUFSIZE];
    int			 size = 4 + 2 + 2;
    int			 n, m;

    if( (xim = imid2xim( display, xc, data.s[2], True )) == NULL )
	return;

    reply[2] = xim->imid;
    for( n = 0; n < data.s[3] / 2; n++ ) {
	switch( data.s[4+n] ) {
	  case XIM_QUERY_INPUT_STYLE:
	    reply[size/2] = swap16( XIM_QUERY_INPUT_STYLE );
	    for( m = 0; input_styles[m]; m++ );
	    reply[size/2+2] = swap16( m );
	    m *= sizeof(CARD32);
	    reply[size/2+1] = swap16( 2 + 2 + m );
	    memcpy( &reply[size/2+4], input_styles, m );
	    size += 2 + 2 + 2 + 2 + m;
	    break;

	  default:
	    XimSendErrorMessage( display, xc, xim->imid, 0,
				 XIM_ERROR_BAD_SOMETHING );
	    return;
	}
    }
    reply[3] = swap16( size - (4 + 2 + 2) );

    if( size <= 20 )
	XimSendClientMessage( display, xc, XIM_GET_IM_VALUES_REPLY, size-4,
			      (CARD8 *)&reply[2] );
    else {
	((CARD8 *)reply)[0] = XIM_GET_IM_VALUES_REPLY;
	((CARD8 *)reply)[1] = 0;
	XimSendProperty( display, xc, xim_im_values, size, (CARD8 *)reply );
    }
}


static CARD16
#if NeedFunctionPrototypes
set_ic_attribute( Display *display, XimcpInputContext *xic, int length,
		  CARD data )
#else
set_ic_attribute( display, xic, length, data )
    Display		*display;
    XimcpInputContext	*xic;
    int			 length;
    CARD		 data;
#endif
{
    int			  n, m, ii;
    int			  len;
    int			  p, s;
    Arg			  pargs[20], sargs[20];
    CARD32		  mask, pmask, smask;
    XFontSet		  pfs, sfs;
    XFontStruct		**fsl;
    char		**fn, **missing_list, *def_string, *base_name;
    int			  missing_count;
    XWindowAttributes	  attr;
    char		  name[20];
    unsigned long	  lbg;

    for( n = 0, mask = pmask = smask = 0; n < length;
	 n += ((2 + 2 + swap16(*((CARD16 *)&data.b[n+2])) + 3) / 4) * 4 ) {
	mask |= 1L << (swap16(*((CARD16 *)&data.b[n])) & 0x000f);
	switch( swap16(*((CARD16 *)&data.b[n])) ) {
	  case XIC_INPUT_STYLE:
	    if( xic->input_style != 0 )
		return( XIM_ERROR_BAD_SOMETHING );
	    xic->input_style = swap32(*((CARD32 *)&data.b[n+4]));
	    if( xic->input_style == (XIMPreeditNothing | XIMStatusNothing) )
		xic->buffer = &root_window;
	    else {
		if( (xic->buffer = buffer_alloc()) == NULL )
		    return( XIM_ERROR_BAD_ALLOC );
		xic->buffer->conversion_style = xic->input_style;
		if( xic->input_style & XIMStatusNothing ) {
		    xic->buffer->label = root_window.label;
		    xic->buffer->box[0] = root_window.box[0];
		    xic->buffer->box[1] = root_window.box[1];
		}
	    }
	    break;

	  case XIC_PREEDIT_ATTRIBUTES:
	    for( m = 4, len = swap16(*((CARD16 *)&data.b[n+2])) + 4; m < len;
		 m += ((4 + swap16(*((CARD16 *)&data.b[n+m+2]) + 3)) / 4) * 4 )
		pmask |= 1L << (swap16(*((CARD16 *)&data.b[n+m])) & 0x000f);
	    break;

	  case XIC_STATUS_ATTRIBUTES:
	    for( m = 4, len = swap16(*((CARD16 *)&data.b[n+2])) + 4; m < len;
		 m += ((4 + swap16(*((CARD16 *)&data.b[n+m+2])) + 3) / 4) * 4 )
		smask |= 1L << (swap16(*((CARD16 *)&data.b[n+m])) & 0x000f);
	    break;
	}
    }
    xic->mask |= mask;
    xic->preedit.mask |= pmask;
    xic->status.mask |= smask;

    for( n = 0, p = s = 0; n < length;
	 n += ((4 + swap16(*((CARD16 *)&data.b[n+2])) + 3) / 4) * 4 ) {
	switch( swap16(*((CARD16 *)&data.b[n])) ) {
	  case XIC_INPUT_STYLE:
#ifdef DEBUG
	    if( resources.debug & 0x10 )
		puts( " XIC_INPUT_STYLE" );
#endif
	    break;

	  case XIC_CLIENT_WINDOW:
#ifdef DEBUG
	    if( resources.debug & 0x10 )
		puts( " XIC_CLIENT_WINDOW" );
#endif
	    xic->client_window = swap32(*((Window *)&data.b[n+4]));
	    if( !(xic->mask & _XIC_FOCUS_WINDOW) ) {
		xic->focus_window = xic->client_window;
		XSelectInput( display, xic->focus_window,
			      ExposureMask | StructureNotifyMask );
		if( xic->input_style & (XIMPreeditPosition | XIMPreeditArea) ) {
		    XtSetArg( pargs[p], XmwNfocusWindow, xic->focus_window );
		    p++;
		}
	    }
	    if( xic->input_style & XIMStatusArea ) {
		XtSetArg( sargs[s], XmwNparentWindow, xic->client_window );
		s++;
	    }
	    break;

	  case XIC_FOCUS_WINDOW:
#ifdef DEBUG
	    if( resources.debug & 0x10 )
		puts( " XIC_FOCUS_WINDOW" );
#endif
	    xic->focus_window = swap32(*((Window *)&data.b[n+4]));
	    XSelectInput( display, xic->focus_window,
			  ExposureMask | StructureNotifyMask );
	    if( !(xic->mask & _XIC_CLIENT_WINDOW) )
		xic->client_window = xic->focus_window;
	    if( xic->input_style & (XIMPreeditPosition | XIMPreeditArea) ) {
		XtSetArg( pargs[p], XmwNfocusWindow, xic->focus_window );
		p++;
	    }
	    break;

	  case XIC_RESET_STATE:
#ifdef DEBUG
	    if( resources.debug & 0x10 )
		puts( " XIC_RESET_STATE" );
#endif
	    xic->reset_state = swap32(*((CARD32 *)&data.b[n+4]));
	    break;

	  case XIC_HOT_KEY:
#ifdef DEBUG
	    if( resources.debug & 0x10 )
		puts( " XIC_HOT_KEY" );
#endif
	    /* XXXXX */
	    break;

	  case XIC_HOT_KEY_STATE:
#ifdef DEBUG
	    if( resources.debug & 0x10 )
		puts( " XIC_HOT_KEY_STATE" );
#endif
	    xic->hot_key_state = swap32(*((CARD32 *)&data.b[n+4]));
	    break;

	  case XIC_STRING_CONVERSION:
#ifdef DEBUG
	    if( resources.debug & 0x10 )
		puts( " XIC_STRING_CONVERSION" );
#endif
	    /* XXXXX */
	    break;

	  case XIC_PREEDIT_ATTRIBUTES:
#ifdef DEBUG
	    if( resources.debug & 0x10 )
		puts( " XIC_PREEDIT_ATTRIBUTES" );
#endif
	    for( m = 4, len = swap16(*((CARD16 *)&data.b[n+2])) + 4; m < len;
		 m += ((4 + swap16(*((CARD16 *)&data.b[n+m+2])) + 3)/4) * 4 ) {
		switch( swap16(*((CARD16 *)&data.b[n+m])) ) {
		  case XIC_AREA:
		    xic->preedit.area.x = swap16(*((INT16 *)&data.b[n+m+4]));
		    xic->preedit.area.y = swap16(*((INT16 *)&data.b[n+m+6]));
		    xic->preedit.area.width
			= swap16(*((INT16 *)&data.b[n+m+8]));
		    xic->preedit.area.height
			= swap16(*((INT16 *)&data.b[n+m+10]));
		    XtSetArg( pargs[p], XmwNareaX, xic->preedit.area.x );
		    p++;
		    XtSetArg( pargs[p], XmwNareaY, xic->preedit.area.y );
		    p++;
		    XtSetArg( pargs[p], XmwNareaWidth,
			      xic->preedit.area.width );		p++;
		    XtSetArg( pargs[p], XmwNareaHeight,
			      xic->preedit.area.height );		p++;
#ifdef DEBUG
		    if( resources.debug & 0x10 )
			printf( "  XIC_AREA (%d,%d) %dx%d\n",
				xic->preedit.area.x,
				xic->preedit.area.y,
				xic->preedit.area.width,
				xic->preedit.area.height );
#endif
		    break;

		  case XIC_AREA_NEEDED:
		    xic->preedit.area_needed.width
			= swap16(*((INT16 *)&data.b[n+m+4]));
		    xic->preedit.area_needed.height
			= swap16(*((INT16 *)&data.b[n+m+6]));
		    XtSetArg( pargs[p], XmwNareaNeededWidth,
			      xic->preedit.area_needed.width );		p++;
		    XtSetArg( pargs[p], XmwNareaNeededHeight,
			      xic->preedit.area_needed.height );	p++;
#ifdef DEBUG
		    if( resources.debug & 0x10 )
			printf( "  XIC_AREA_NEEDED %dx%d\n",
				xic->preedit.area_needed.width,
				xic->preedit.area_needed.height );
#endif
		    break;

		  case XIC_SPOT_LOCATION:
		    xic->preedit.spot_location.x
			= swap16(*((INT16 *)&data.b[n+m+4]));
		    xic->preedit.spot_location.y
			= swap16(*((INT16 *)&data.b[n+m+6]));
		    if( !(pmask & _XIC_FONTSET) ) {
			XtSetArg( pargs[p], XmwNspotLocationX,
				  xic->preedit.spot_location.x );	p++;
			XtSetArg( pargs[p], XmwNspotLocationY,
				  xic->preedit.spot_location.y -
				  xic->preedit.ascent );		p++;
		    }
#ifdef DEBUG
		    if( resources.debug & 0x10 )
			printf( "  XIC_SPOT_LOCATION (%d,%d)\n",
				xic->preedit.spot_location.x,
				xic->preedit.spot_location.y );
#endif
		    break;

		  case XIC_COLORMAP:
#ifdef DEBUG
		    if( resources.debug & 0x10 )
			puts( "  XIC_COLORMAP" );
#endif
		    xic->preedit.colormap
			= swap32(*((CARD32 *)&data.b[n+m+4]));
		    XtSetArg( pargs[p], XtNcolormap, xic->preedit.colormap );
		    p++;
		    break;

		  case XIC_FOREGROUND:
		    xic->preedit.foreground
			= swap32(*((CARD32 *)&data.b[n+m+4]));
		    XtSetArg( pargs[p], XtNforeground,
			      xic->preedit.foreground );		p++;
#ifdef DEBUG
		    if( resources.debug & 0x10 )
			printf( "  XIC_FOREGROUND %ld\n",
				xic->preedit.foreground );
#endif
		    break;

		  case XIC_BACKGROUND:
		    xic->preedit.background
			= swap32(*((CARD32 *)&data.b[n+m+4]));
		    XtSetArg( pargs[p], XtNbackground,
			      xic->preedit.background );		p++;
#ifdef DEBUG
		    if( resources.debug & 0x10 )
			printf( "  XIC_BACKGROUND %ld\n",
				xic->preedit.background );
#endif
		    break;

		  case XIC_BACKGROUND_PIXMAP:
#ifdef DEBUG
		    if( resources.debug & 0x10 )
			puts( "  XIC_BACKGROUND_PIXMAP" );
#endif
		    xic->preedit.pixmap = swap32(*((CARD32 *)&data.b[n+m+4]));
		    XtSetArg( pargs[p], XtNbackgroundPixmap,
			      xic->preedit.pixmap );			p++;
		    break;

		  case XIC_FONTSET:
#ifdef DEBUG
		    if( resources.debug & 0x10 )
			puts( "  XIC_FONTSET" );
#endif
		    base_name
			= malloc( swap16(*((CARD16 *)&data.b[n+m+4])) + 1 );
		    memcpy( base_name, &data.b[n+m+6],
			    swap16(*((CARD16 *)&data.b[n+m+4])) );
		    base_name[swap16(*((INT16 *)&data.b[n+m+4]))] = '\0';
		    pfs = XCreateFontSet( display, base_name, &missing_list,
					  &missing_count, &def_string );
		    if( missing_list )
			XFree( missing_list );
		    if( pfs == NULL ) {
			free( base_name );
			return( XIM_ERROR_BAD_SOMETHING );
		    }
		    XtSetArg( pargs[p], XtNfontSet, pfs );		p++;

		    ii = XFontsOfFontSet( pfs, &fsl, &fn );
		    xic->preedit.ascent = 0;
		    while( --ii >= 0 )
			if( xic->preedit.ascent < fsl[ii]->ascent )
			    xic->preedit.ascent = fsl[ii]->ascent;

		    if( xic->preedit.font_set )
			free( xic->preedit.font_set );
		    xic->preedit.font_set = base_name;

		    if( pmask & _XIC_SPOT_LOCATION ) {
			XtSetArg( pargs[p], XmwNspotLocationX,
				  xic->preedit.spot_location.x );	p++;
			XtSetArg( pargs[p], XmwNspotLocationY,
				  xic->preedit.spot_location.y -
				  xic->preedit.ascent );		p++;
		    }
		    break;

		  case XIC_LINE_SPACING:
#ifdef DEBUG
		    if( resources.debug & 0x10 )
			puts( "  XIC_LINE_SPACING" );
#endif
		    xic->preedit.line_spacing
			= swap32(*((INT32 *)&data.b[n+m+4]));
		    XtSetArg( pargs[p], XmwNlineSpacing,
			      xic->preedit.line_spacing );		p++;
		    break;

		  case XIC_CURSOR:
#ifdef DEBUG
		    if( resources.debug & 0x10 )
			puts( "  XIC_CURSOR" );
#endif
		    xic->preedit.cursor = swap32(*((CARD32 *)&data.b[n+m+4]));
		    XtSetArg( pargs[p], XtNcursor, xic->preedit.cursor );
		    p++;
		    break;

		  case XIC_PREEDIT_STATE:
#ifdef DEBUG
		    if( resources.debug & 0x10 )
			puts( "  XIC_PREEDIT_STATE" );
#endif
		    xic->preedit.state = swap32(*((CARD32 *)&data.b[n+m+4]));
		    break;

		  default:
		    return( XIM_ERROR_BAD_SOMETHING );
		}
	    }
	    break;

	  case XIC_STATUS_ATTRIBUTES:
#ifdef DEBUG
	    if( resources.debug & 0x10 )
		puts( " XIC_STATUS_ATTRIBUTES" );
#endif
	    for( m = 4, len = swap16(*((CARD16 *)&data.b[n+2])) + 4; m < len;
		 m += ((4 + swap16(*((CARD16 *)&data.b[n+m+2])) + 3) / 4)*4 ) {
		switch( swap16(*((CARD16 *)&data.b[n+m])) ) {
		  case XIC_AREA:
		    xic->status.area.x = swap16(*((INT16 *)&data.b[n+m+4]));
		    xic->status.area.y = swap16(*((INT16 *)&data.b[n+m+6]));
		    xic->status.area.width
			= swap16(*((CARD16 *)&data.b[n+m+8]));
		    xic->status.area.height
			= swap16(*((CARD16 *)&data.b[n+m+10]));
		    XtSetArg( sargs[s], XtNx, xic->status.area.x );
		    s++;
		    XtSetArg( sargs[s], XtNy, xic->status.area.y );
		    s++;
		    XtSetArg( sargs[s], XtNwidth, xic->status.area.width );
		    s++;
		    XtSetArg( sargs[s], XtNheight, xic->status.area.height );
		    s++;
#ifdef DEBUG
		    if( resources.debug & 0x10 )
			printf( "  XIC_AREA (%d,%d) %dx%d\n",
				xic->status.area.x,
				xic->status.area.y,
				xic->status.area.width,
				xic->status.area.height );
#endif
		    break;

		  case XIC_AREA_NEEDED:
		    xic->status.area_needed.width
			= swap16(*((CARD16 *)&data.b[n+m+4]));
		    xic->status.area_needed.height
			= swap16(*((CARD16 *)&data.b[n+m+6]));
		    if( !(smask & _XIC_AREA) ) {
			XtSetArg( sargs[s], XtNwidth,
				  xic->status.area_needed.width );	s++;
			XtSetArg( sargs[s], XtNheight,
				  xic->status.area_needed.height );	s++;
		    }
#ifdef DEBUG
		    if( resources.debug & 0x10 )
			printf( "  XIC_AREA_NEEDED %dx%d\n",
				xic->status.area_needed.width,
				xic->status.area_needed.height );
#endif
		    break;

		  case XIC_COLORMAP:
#ifdef DEBUG
		    if( resources.debug & 0x10 )
			puts( "  XIC_COLORMAP" );
#endif
		    xic->status.colormap = swap32(*((CARD32 *)&data.b[n+m+4]));
		    XtSetArg( sargs[s], XtNcolormap, xic->status.colormap );
		    s++;
		    break;

		  case XIC_FOREGROUND:
		    xic->status.foreground
			= swap32(*((CARD32 *)&data.b[n+m+4]));
		    XtSetArg( sargs[s], XtNforeground, xic->status.foreground );
		    s++;
#ifdef DEBUG
		    if( resources.debug & 0x10 )
			printf( "  XIC_FOREGROUND %ld\n",
				xic->status.foreground );
#endif
		    break;

		  case XIC_BACKGROUND:
		    xic->status.background
			= swap32(*((CARD32 *)&data.b[n+m+4]));
		    XtSetArg( sargs[s], XtNbackground, xic->status.background );
		    s++;
#ifdef DEBUG
		    if( resources.debug & 0x10 )
			printf( "  XIC_BACKGROUND %ld\n",
				xic->status.background );
#endif
		    break;

		  case XIC_BACKGROUND_PIXMAP:
#ifdef DEBUG
		    if( resources.debug & 0x10 )
			puts( "  XIC_BACKGROUND_PIXMAP" );
#endif
		    xic->status.pixmap = swap32(*((CARD32 *)&data.b[n+m+4]));
		    XtSetArg( sargs[s], XtNbackgroundPixmap,
			      xic->status.pixmap );			s++;
		    break;

		  case XIC_FONTSET:
#ifdef DEBUG
		    if( resources.debug & 0x10 )
			puts( "  XIC_FONTSET" );
#endif
		    base_name
			= malloc( swap16(*((CARD16 *)&data.b[n+m+4])) + 1 );
		    memcpy( base_name, &data.b[n+m+6],
			    swap16(*((CARD16 *)&data.b[n+m+4])) );
		    base_name[swap16(*((INT16 *)&data.b[n+m+4]))] = '\0';
		    sfs = XCreateFontSet( display, base_name, &missing_list,
					  &missing_count, &def_string );
		    if( missing_list )
			XFree( missing_list );
		    if( sfs == NULL ) {
			free( base_name );
			return( XIM_ERROR_BAD_SOMETHING );
		    }
		    XtSetArg( sargs[s], XtNfontSet, sfs );		s++;

		    if( xic->status.font_set )
			free( xic->status.font_set );
		    xic->status.font_set = base_name;
		    break;

		  case XIC_CURSOR:
#ifdef DEBUG
		    if( resources.debug & 0x10 )
			puts( "  XIC_CURSOR" );
#endif
		    xic->status.cursor = swap32(*((CARD32 *)&data.b[n+m+4]));
		    XtSetArg( sargs[p], XtNcursor, xic->status.cursor );
		    s++;
		    break;

		  default:
		    return( XIM_ERROR_BAD_SOMETHING );
		}
	    }
	    n += m;
	    break;

	  default:
	    return( XIM_ERROR_BAD_SOMETHING );
	}
    }

    if( xic->input_style & (XIMPreeditPosition | XIMPreeditArea) ) {
	if( xic->buffer->preedit_widget == NULL ) {
	    if( !(pmask & (_XIC_CLIENT_WINDOW | _XIC_FOCUS_WINDOW)) ) {
		XtSetArg( pargs[p], XmwNfocusWindow, xic->focus_window );
		p++;
	    }

	    XGetWindowAttributes( display, xic->focus_window, &attr );
	    XtSetArg( pargs[p], XtNdepth, attr.depth );			p++;
	    XtSetArg( pargs[p], XtNscreen, attr.screen );		p++;
	    XtSetArg( pargs[p], XtNaccelerators, resources.base_accelerators );
	    p++;

	    if( !(pmask & _XIC_AREA) ) {
		xic->preedit.area.x = xic->preedit.spot_location.x;
		xic->preedit.area.y = xic->preedit.spot_location.y
				    - xic->preedit.ascent;
		xic->preedit.area.width = attr.width - xic->preedit.area.x;
		xic->preedit.area.height = attr.height - xic->preedit.area.y;
		XtSetArg( pargs[p], XmwNareaX, xic->preedit.area.x );	p++;
		XtSetArg( pargs[p], XmwNareaY, xic->preedit.area.y );	p++;
		XtSetArg( pargs[p], XmwNareaWidth, xic->preedit.area.width );
		p++;
		XtSetArg( pargs[p], XmwNareaHeight, xic->preedit.area.height );
		p++;
	    }
	    if( !(pmask & _XIC_COLORMAP)  &&
		(pmask & (_XIC_FOREGROUND | _XIC_BACKGROUND)) ) {
		xic->preedit.colormap = attr.colormap;
		XtSetArg( pargs[p], XtNcolormap, xic->preedit.colormap );
		p++;
	    }

	    sprintf( name, "Pre%04x%04x", xic->xim->imid, xic->icid );
	    xic->buffer->preedit_widget = XmwCreatePreedit( TopLevel, name,
							    pargs, p );
	    if( xic->preedit.state == XIMPreeditEnable )
		XmwPreeditMapWidget( xic->buffer->preedit_widget );
	    else
		XmwPreeditUnmapWidget( xic->buffer->preedit_widget );
	    XtInstallAccelerators( xic->buffer->preedit_widget,
				   xic->buffer->preedit_widget );
	    if( resources.accelerators ) {
		XtOverrideTranslations( xic->buffer->preedit_widget,
					resources.accelerators );
		XtInstallAccelerators( xic->buffer->preedit_widget,
				       xic->buffer->preedit_widget );
	    }

	    p = 0;
	    if( !(pmask & _XIC_SPOT_LOCATION ) ) {
		XtSetArg( pargs[p], XmwNspotLocationX,
			  &xic->preedit.spot_location.x );		p++;
		XtSetArg( pargs[p], XmwNspotLocationY,
			  &xic->preedit.spot_location.y );		p++;
	    }
	    if( !(pmask & _XIC_COLORMAP) ) {
		XtSetArg( pargs[p], XtNcolormap, &xic->preedit.colormap );
		p++;
	    }
	    if( !(pmask & _XIC_FOREGROUND) ) {
		XtSetArg( pargs[p], XtNforeground, &xic->preedit.foreground );
		p++;
	    }
	    if( !(pmask & _XIC_BACKGROUND) ) {
		XtSetArg( pargs[p], XtNbackground, &xic->preedit.background );
		p++;
	    }
	    if( !(pmask & _XIC_BACKGROUND_PIXMAP) ) {
		XtSetArg( pargs[p], XtNbackgroundPixmap,
			  &xic->preedit.pixmap );			p++;
	    }
	    if( !(pmask & _XIC_FONTSET ) ) {
		XtVaGetValues( xic->buffer->preedit_widget, XtNfontSet, &pfs,
			       NULL );
		base_name = XBaseFontNameListOfFontSet( pfs );
		xic->preedit.font_set = malloc( strlen(base_name) + 1 );
		strcpy( xic->preedit.font_set, base_name );
	    }
	    if( !(pmask & _XIC_LINE_SPACING) ) {
		XtSetArg( pargs[p], XmwNlineSpacing,
			  &xic->preedit.line_spacing );			p++;
	    }
	}
	else {
	    XtSetValues( xic->buffer->preedit_widget, pargs, p );
	    p = 0;
	}
	XtSetArg( pargs[p], XmwNareaNeededWidth,
		  &xic->preedit.area_needed.width );			p++;
	XtSetArg( pargs[p], XmwNareaNeededHeight,
		  &xic->preedit.area_needed.height );			p++;
	XtGetValues( xic->buffer->preedit_widget, pargs, p );
    }

    if( xic->input_style & XIMStatusArea ) {
	XGetWindowAttributes( display, xic->client_window, &attr );
	if( xic->buffer->status_widget == NULL ) {
	    if( !(smask & _XIC_CLIENT_WINDOW) ) {
		XtSetArg( sargs[s], XmwNparentWindow, xic->client_window );
		s++;
	    }

	    XtSetArg( sargs[s], XtNdepth, attr.depth );			s++;
	    XtSetArg( sargs[s], XtNscreen, attr.screen );		s++;
	    XtSetArg( sargs[s], XtNaccelerators, resources.base_accelerators );
	    s++;

	    if( !(smask & _XIC_AREA) ) {
		xic->status.area.width = attr.width;
		xic->status.area.height = attr.height;
		if( !(smask & _XIC_AREA_NEEDED) ) {
		    XtSetArg( sargs[s], XtNwidth, xic->status.area.width );
		    s++;
		    XtSetArg( sargs[s], XtNheight, xic->status.area.height );
		    s++;
		}
	    }
	    if( !(smask & _XIC_COLORMAP) ) {
		xic->status.colormap = attr.colormap;
		XtSetArg( sargs[s], XtNcolormap, xic->status.colormap );
		s++;
	    }
	    if( !(smask & _XIC_FOREGROUND) ) {
		xic->status.foreground = default_foreground;
	    }
	    if( !(smask & _XIC_BACKGROUND) ) {
		xic->status.background = default_background;
		XtSetArg( sargs[s], XtNbackground, default_background );
		s++;
	    }

	    sprintf( name, "Sts%04x%04x", xic->xim->imid, xic->icid );
	    xic->buffer->status_widget = XmwCreateManagedStatusArea( TopLevel,
								     name,
								     sargs, s );

	    s = 0;
	    n = XScreenNumberOfScreen( attr.screen );
	    lbg = (depths[n] == 1) ? xic->status.background
				   : light_color( display, xic->status.colormap,
						  xic->status.background );

	    XtSetArg( sargs[s], XtNborderWidth, 0 );			s++;
	    XtSetArg( sargs[s], XtNbackground, xic->status.background );s++;
	    XtSetArg( sargs[s], XtNforeground, xic->status.foreground );s++;
	    XtSetArg( sargs[s], XtNlabel, resources.sjxa );		s++;
	    if( smask & _XIC_BACKGROUND_PIXMAP ) {
		XtSetArg( sargs[s], XtNbackgroundPixmap, xic->status.pixmap );
		s++;
	    }
	    if( smask & _XIC_FONTSET ) {
		XtSetArg( sargs[s], XtNfontSet, sfs );			s++;
	    }
	    xic->buffer->label =
		XtCreateManagedWidget( "Label", labelWidgetClass,
				       xic->buffer->status_widget, sargs, s );

	    if( !(xic->input_style & XIMPreeditArea) ) {
		s = 0;
		if( smask & _XIC_FONTSET ) {
		    XtSetArg( sargs[s], XtNfontSet, sfs );		s++;
		}
		XtSetArg( sargs[s], XtNforeground, xic->status.foreground );
		s++;
		XtSetArg( sargs[s], XtNbackground, xic->status.background );
		s++;
		XtSetArg( sargs[s], XtNhorizDistance, 2 );		s++;
		XtSetArg( sargs[s], XtNborderWidth, 0 );		s++;
		XtSetArg( sargs[s], XtNmappedWhenManaged, False );	s++;
		XtSetArg( sargs[s], XmwNitemBackground, lbg );		s++;
		XtSetArg( sargs[s], XmwNitemCount, 5 );			s++;
		XtSetArg( sargs[s], XmwNitemLength,
			  resources.function_par_length * 5 );		s++;
		XtSetArg( sargs[s], XmwNitems, resources.function_items );
		s++;
		XtSetArg( sargs[s], XmwNnumRows, 5 );			s++;
		XtSetArg( sargs[s], XmwNvisibleHorizontalItemCount, 5 );
		s++;
		XtSetArg( sargs[s], XmwNvisibleVerticalItemCount, 1 );	s++;
		xic->buffer->box[0] = XmwCreateList( xic->buffer->status_widget,
						     "LeftBox", sargs, s );
		s = 0;
		if( smask & _XIC_FONTSET ) {
		    XtSetArg( sargs[s], XtNfontSet, sfs );		s++;
		}
		XtSetArg( sargs[s], XtNforeground, xic->status.foreground );
		s++;
		XtSetArg( sargs[s], XtNbackground, xic->status.background );
		s++;
		XtSetArg( sargs[s], XtNhorizDistance, 2 );		s++;
		XtSetArg( sargs[s], XtNborderWidth, 0 );		s++;
		XtSetArg( sargs[s], XtNmappedWhenManaged, False );	s++;
		XtSetArg( sargs[s], XmwNitemBackground, lbg );		s++;
		XtSetArg( sargs[s], XmwNitemCount, 5 );			s++;
		XtSetArg( sargs[s], XmwNitemLength,
			  resources.function_par_length * 5 );		s++;
		XtSetArg( sargs[s], XmwNitems,
		  &resources.function_items[resources.function_par_length *5] );
		s++;
		XtSetArg( sargs[s], XmwNnumRows, 5 );			s++;
		XtSetArg( sargs[s], XmwNvisibleHorizontalItemCount, 5 );
		s++;
		XtSetArg( sargs[s], XmwNvisibleVerticalItemCount, 1 );	s++;
		xic->buffer->box[1] = XmwCreateList( xic->buffer->status_widget,
						     "RightBox", sargs, s );
		XtManageChildren( xic->buffer->box, 2 );
		XtAddCallback( xic->buffer->box[0], XmwNsetCallback,
			       (XtCallbackProc)function_activate,
			       (caddr_t)xic->buffer );
		XtAddCallback( xic->buffer->box[1], XmwNsetCallback,
			       (XtCallbackProc)function_activate,
			       (caddr_t)xic->buffer );
	    }
	    XtMapWidget( xic->buffer->status_widget );

	    if( !(smask & _XIC_FONTSET ) ) {
		XtVaGetValues( xic->buffer->label, XtNfontSet, &sfs,
			       NULL );
		base_name = XBaseFontNameListOfFontSet( sfs );
		xic->status.font_set = malloc( strlen(base_name) + 1 );
		strcpy( xic->status.font_set, base_name );
	    }
	}
	else {
	    if( s > 0 ) {
		XtSetValues( xic->buffer->status_widget, sargs, s );

		n = XScreenNumberOfScreen( attr.screen );
		lbg = (depths[n] == 1) ? xic->status.background
				       : light_color( display,
						      xic->status.colormap,
						      xic->status.background );
		s = 0;
		if( smask & _XIC_FOREGROUND ) {
		    XtSetArg( sargs[s], XtNbackground, xic->status.foreground );
		    s++;
		}
		if( smask & _XIC_BACKGROUND ) {
		    XtSetArg( sargs[s], XtNforeground, lbg );		s++;
		}
		if( smask & _XIC_BACKGROUND_PIXMAP ) {
		    XtSetArg( sargs[s], XtNbackgroundPixmap,
			      xic->status.pixmap );			s++;
		}
		if( smask & _XIC_FONTSET ) {
		    XtSetArg( sargs[s], XtNfontSet, sfs );		s++;
		}
		if( s > 0 )
		    XtSetValues( xic->buffer->label, sargs, s );

		if( !(xic->input_style & XIMPreeditArea) ) {
		    s = 0;
		    if( smask & _XIC_FOREGROUND ) {
			XtSetArg( sargs[s], XtNforeground,
				  xic->status.foreground );		s++;
		    }
		    if( smask & _XIC_BACKGROUND ) {
			XtSetArg( sargs[s], XtNforeground,
				  xic->status.background );		s++;
			XtSetArg( sargs[s], XmwNitemBackground, lbg );	s++;
		    }
		    if( smask & _XIC_BACKGROUND_PIXMAP ) {
			XtSetArg( sargs[s], XtNbackgroundPixmap,
				  xic->status.pixmap );			s++;
		    }
		    if( smask & _XIC_FONTSET ) {
			XtSetArg( sargs[s], XtNfontSet, sfs );		s++;
		    }
		    if( s > 0 ) {
			XtSetValues( xic->buffer->box[0], sargs, s );
			XtSetValues( xic->buffer->box[1], sargs, s );
		    }
		}
	    }
	}

	XtVaGetValues( xic->buffer->status_widget,
		       XmwNareaNeededWidth, &xic->status.area_needed.width,
		       XmwNareaNeededHeight, &xic->status.area_needed.height,
		       NULL );
    }

    return( XIM_NO_ERROR );
}


static void
#if NeedFunctionPrototypes
XimCreateIc( Display *display, XimcpConnection *xc, CARD data )
#else
XimCreateIc( display, xc, data )
    Display		*display;
    XimcpConnection	*xc;
    CARD		 data;
#endif
{
    XimcpInputMethod	*xim;
    XimcpInputContext	*xic1, *xic2;
    int			 n;
    int			 length;
    CARD16		 reply[2];
    CARD16		 error;

    if( (xim = imid2xim( display, xc, swap16(data.s[2]), True )) == NULL )
	return;

    xic1 = (XimcpInputContext *)calloc( 1, sizeof(XimcpInputContext) );
    xic1->xim = xim;
    for( n = 1; n <= MAXSHORT; n++ ) {
	for( xic2 = xim->xic; xic2 != NULL; xic2 = xic2->next )
	    if( xic2->icid == n )
		break;
	if( xic2 == NULL )
	    break;
    }
    xic1->icid = n;

    xic1->input_style = 0;
    xic1->client_window = None;
    xic1->focus_window = None;
    xic1->reset_state = XIMInitialState;
    xic1->hot_key.num_hot_key = 0;
    xic1->hot_key_state = XIMHotKeyStateOFF;
    xic1->has_focus = False;
    xic1->buffer = NULL;
    xic1->mask = 0;

    xic1->preedit.mask = 0;
    xic1->preedit.font_set = NULL;
    xic1->preedit.state = XIMPreeditDisable;

    xic1->status.font_set = NULL;
    xic1->status.mask = 0;

    length = swap16(data.s[3]);
    data.l = &data.l[2];
    if( (error = set_ic_attribute( display, xic1, length, data ))
	!= XIM_NO_ERROR ) {
	XimSendErrorMessage( display, xc, xim->imid, 0, error );
	free( (char *)xic1 );
	return;
    }

    xic1->next = NULL;
    if( xim->xic == NULL ) {
	xim->xic = xic1;
	xic1->previous = NULL;
    }
    else {
	for( xic2 = xim->xic; xic2->next != NULL; xic2 = xic2->next );
	xic2->next = xic1;
	xic1->previous = xic2;
    }

    reply[0] = xim->imid;
    reply[1] = xic1->icid;
    XimSendClientMessage( display, xc, XIM_CREATE_IC_REPLY, 4, (CARD8 *)reply );
}


static void
#if NeedFunctionPrototypes
XimDestroyIc( Display *display, XimcpConnection *xc, CARD data )
#else
XimDestroyIc( display, xc, data )
    Display		*display;
    XimcpConnection	*xc;
    CARD		 data;
#endif
{
    XimcpInputMethod	*xim;
    XimcpInputContext	*xic;
    CARD16		 reply[2];

    if( (xim = imid2xim( display, xc, swap16(data.s[2]), True )) == NULL )
	return;
    if( (xic = icid2xic( display, xim, swap16(data.s[3]), True )) == NULL )
	return;

    reply[0] = swap16(xim->imid);
    reply[1] = swap16(xic->icid);
    XimSendClientMessage( display, xc, XIM_DESTROY_IC_REPLY, 4,
			  (CARD8 *)reply );
    if( xic->previous )
	xic->previous->next = xic->next;
    else
	xim->xic = xic->next;
    if( xic->next )
	xic->next->previous = xic->previous;

    destroy_ic( xic );
}


static void
#if NeedFunctionPrototypes
XimSetIcValues( Display *display, XimcpConnection *xc, CARD data )
#else
XimSetIcValues( display, xc, data )
    Display		*display;
    XimcpConnection	*xc;
    CARD		 data;
#endif
{
    XimcpInputMethod	*xim;
    XimcpInputContext	*xic;
    int			 length;
    CARD16		 reply[2];
    CARD16		 error;

    if( (xim = imid2xim( display, xc, swap16(data.s[2]), True )) == NULL )
	return;
    if( (xic = icid2xic( display, xim, swap16(data.s[3]), True )) == NULL )
	return;

    length = data.s[4];
    data.l = &data.l[3];
    if( (error = set_ic_attribute( display, xic, length, data ))
	!= XIM_NO_ERROR ) {
	XimSendErrorMessage( display, xc, xim->imid, xic->icid, error );
	return;
    }

    reply[0] = swap16(xim->imid);
    reply[1] = swap16(xic->icid);
    XimSendClientMessage( display, xc, XIM_SET_IC_VALUES_REPLY, 4,
			  (CARD8 *)reply );
}


static void
#if NeedFunctionPrototypes
XimGetIcValues( Display *display, XimcpConnection *xc, CARD data )
#else
XimGetIcValues( display, xc, data )
    Display		*display;
    XimcpConnection	*xc;
    CARD		 data;
#endif
{
    XimcpInputMethod	*xim;
    XimcpInputContext	*xic;
    CARD16		 reply[XIMCP_BUFSIZE];
    int			 size;
    int			 n;
    XimcpAttrRec	*a, *attr;
    String		 s;
    int			 len;

    if( (xim = imid2xim( display, xc, swap16(data.s[2]), True )) == NULL )
	return;
    if( (xic = icid2xic( display, xim, swap16(data.s[3]), True )) == NULL )
	return;
    reply[2] = swap16(xim->imid);
    reply[3] = swap16(xic->icid);
    for( n = 0, size = 5, attr = ic_attr; n < swap16(data.s[3]) / 2; n++ ) {
	for( a = attr; a->id != 0; a++ ) {
	    if( swap16(data.s[4+n]) == a->id ) {
		*((CARD16 *)&reply[size]) = swap16(a->id);
		size += 2;
		if( a->id == XIC_FONTSET ) {
		    s = (String)((CARD8 *)xic + a->offset);
		    len = strlen(s);
		    *((CARD16 *)&reply[size]) = swap16(len);
		    size +=2 ;
		    memcpy( &reply[size], s, len );
		    size += len + Pad(len);
		}
		else {
		    *((CARD16 *)&reply[size]) = swap16(a->size);
		    size += 2;
		    memcpy( &reply[size], (CARD8 *)xic + a->offset, a->size );
		    size += a->size + Pad(a->size);
		}

		if( a->nested_list != NULL )
		    attr = attr->nested_list;
		else if( a->type == XimRseparatorOfNestedList )
		    attr = ic_attr;
		break;
	    }
	}
    }

    reply[3] = swap16(size - 8);

    if( size <= 20 )
	XimSendClientMessage( display, xc, XIM_GET_IC_VALUES_REPLY, size-4,
			      (CARD8 *)&reply[2] );
    else {
	((CARD8 *)reply)[0] = XIM_GET_IC_VALUES_REPLY;
	((CARD8 *)reply)[1] = 0;
	XimSendProperty( display, xc, xim_ic_values, size, (CARD8 *)reply );
    }
}


static void
#if NeedFunctionPrototypes
XimSetIcFocus( Display *display, XimcpConnection *xc, CARD data )
#else
XimSetIcFocus( display, xc, data )
    Display		*display;
    XimcpConnection	*xc;
    CARD		 data;
#endif
{
    XimcpInputMethod	*xim;
    XimcpInputContext	*xic;

    if( (xim = imid2xim( display, xc, swap16(data.s[2]), False )) == NULL )
	return;
    if( (xic = icid2xic( display, xim, swap16(data.s[3]), False )) == NULL )
	return;

    if( xic->input_style & (XIMPreeditPosition | XIMPreeditArea) ) {
	XtVaSetValues( xic->buffer->preedit_widget, XmwNfocus, True, NULL );
	XtSetKeyboardFocus( TopLevel, xic->buffer->preedit_widget );
	if( xic->buffer->state == SELECT )
	    XRaiseWindow( XtDisplay(xic->buffer->shell),
			  XtWindow(xic->buffer->shell) );
    }
    if( xic->buffer->error_shell )
	XRaiseWindow( XtDisplay(xic->buffer->error_shell),
		      XtWindow(xic->buffer->error_shell) );

    if( xic->buffer->state & DELETE ) {
	XRaiseWindow( XtDisplay(delete_shell), XtWindow(delete_shell) );
	XRaiseWindow( XtDisplay(question_shell), XtWindow(question_shell) );
    }
    if( xic->buffer->state & (ADD | CODE) )
	XRaiseWindow( XtDisplay(xic->buffer->shell),
		      XtWindow(xic->buffer->shell) );
    if( xic->buffer->state & SERVER )
	XRaiseWindow( XtDisplay(server_shell), XtWindow(server_shell) );

    if( (xic->input_style & XIMStatusArea)  &&  xic->buffer->status_widget )
	XRaiseWindow( XtDisplay(xic->buffer->status_widget),
		      XtWindow(xic->buffer->status_widget) );
    else if( xic->input_style & XIMStatusNothing ) {
	xic->buffer->conversion_mode = root_window.conversion_mode;
	xic->buffer->conversion_mode_toggle = root_window.conversion_mode_toggle;
    }
    xic->has_focus = True;
}


static void
#if NeedFunctionPrototypes
XimUnsetIcFocus( Display *display, XimcpConnection *xc, CARD data )
#else
XimUnsetIcFocus( display, xc, data )
    Display		*display;
    XimcpConnection	*xc;
    CARD		 data;
#endif
{
    XimcpInputMethod	*xim;
    XimcpInputContext	*xic;

    if( (xim = imid2xim( display, xc, swap16(data.s[2]), False )) == NULL )
	return;
    if( (xic = icid2xic( display, xim, swap16(data.s[3]), False )) == NULL )
	return;

    if( xic->input_style & (XIMPreeditPosition | XIMPreeditArea) )
	XtVaSetValues( xic->buffer->preedit_widget, XmwNfocus, False, NULL );
    xic->has_focus = False;
}


static void
#if NeedFunctionPrototypes
XimForwardEvent( Display *display, XimcpConnection *xc, CARD data )
#else
XimForwardEvent( display, xc, data )
    Display		*display;
    XimcpConnection	*xc;
    CARD		 data;
#endif
{
    XimcpInputMethod	*xim;
    XimcpInputContext	*xic;
    XKeyEvent		 key;

    if( (xim = imid2xim( display, xc, swap16(data.s[2]), False )) == NULL )
	return;
    if( (xic = icid2xic( display, xim, swap16(data.s[3]), False )) == NULL )
	return;

    key.type = KeyPress;
    key.serial = swap16( ((xEvent *)&data.l[3])->u.u.sequenceNumber );
    key.serial |= swap16( data.s[5] ) << 16;
    key.send_event = False;
    key.display = display;
    key.window = XtWindow( xic->buffer->preedit_widget );
    key.root = swap32( ((xEvent *)&data.l[3])->u.keyButtonPointer.root );
    key.time = swap32( ((xEvent *)&data.l[3])->u.keyButtonPointer.time );
    key.x = swap16( ((xEvent *)&data.l[3])->u.keyButtonPointer.eventX );
    key.y = swap16( ((xEvent *)&data.l[3])->u.keyButtonPointer.eventY );
    key.x_root = swap16( ((xEvent *)&data.l[3])->u.keyButtonPointer.rootX );
    key.y_root = swap16( ((xEvent *)&data.l[3])->u.keyButtonPointer.rootY );
    key.state = swap16( ((xEvent *)&data.l[3])->u.keyButtonPointer.state );
    key.keycode = ((xEvent *)&data.l[3])->u.u.detail;
    key.same_screen = True;

    XPutBackEvent( display, (XEvent *)&key );
}


static void
#if NeedFunctionPrototypes
XimSync( Display *display, XimcpConnection *xc, CARD data )
#else
XimSync( display, xc, data )
    Display		*display;
    XimcpConnection	*xc;
    CARD		 data;
#endif
{
    XimcpInputMethod	*xim;
    XimcpInputContext	*xic;
    CARD16		 reply[2];

    if( (xim = imid2xim( display, xc, swap16(data.s[2]), True )) == NULL )
	return;
    if( (xic = icid2xic( display, xim, swap16(data.s[3]), True )) == NULL )
	return;

    reply[0] = swap16(xim->imid);
    reply[1] = swap16(xic->icid);
    XimSendClientMessage( display, xc, XIM_SYNC_REPLY, 4, (CARD8 *)reply );
}


static void
#if NeedFunctionPrototypes
XimEnd( Display *display, XimcpInputContext *xic )
#else
XimEnd( display, xic )
    Display		*display;
    XimcpInputContext	*xic;
#endif
{
    CARD16	reply[6];

    if( xic->preedit.state == XIMPreeditEnable ) {
	if( xic->buffer->error_shell ) {
	    error_cancel( (Widget)NULL, (caddr_t)xic->buffer, (caddr_t)NULL );
	}
	if( xic->buffer->state & DELETE )
	    sakujo_cancel( (Widget)NULL, (caddr_t)xic->buffer, (caddr_t)NULL );
	if( xic->buffer->state & CODE )
	    code_cancel( (Widget)NULL, (caddr_t)xic->buffer, (caddr_t)NULL );
	if( xic->buffer->state & ADD )
	    touroku_cancel( (Widget)NULL, (caddr_t)xic->buffer, (caddr_t)NULL );
	if( xic->buffer->state & SERVER )
	    reconnect_cancel( (Widget)NULL, (caddr_t)xic->buffer,
			      (caddr_t)NULL );
	if( xic->buffer->state & SELECT ) {
	    if( xic->input_style == (XIMPreeditNothing | XIMStatusNothing) ) {
		XtUnmapWidget( RootTextField );
		XtMapWidget( root_window.label );
	    }
	    else if( xic->input_style & (XIMPreeditPosition | XIMPreeditArea) )
		select_dialog_cancel( (Widget)NULL, (caddr_t)xic->buffer,
				      (caddr_t)NULL );
	}
	if( xic->buffer->state & EDIT )
	    edit_cancel( xic->buffer );
	if( xic->buffer->state & (WRAP | CONVERSION) )
	    henkan_cancel( xic->buffer );
	xic->preedit.state = XIMPreeditDisable;
	xic->buffer->connect--;

	reply[0] = swap16(xic->xim->imid);
	reply[1] = swap16(xic->icid);
	((CARD32 *)reply)[1] = NoEventMask;
	((CARD32 *)reply)[2] = NoEventMask;
	XimSendClientMessage( display, xic->xim->xc, XIM_SET_EVENT_MASK, 12,
			      (CARD8 *)reply );

	if( xic->input_style & (XIMPreeditNothing | XIMStatusNothing) )
	    RootWindowUnmap( xic->input_style );
	if( xic->input_style & (XIMPreeditPosition | XIMPreeditArea) )
	    XmwPreeditUnmapWidget( xic->buffer->preedit_widget );
	if( (xic->input_style & XIMStatusArea)  &&
	    xic->buffer->status_widget ) {
	    if( !(xic->input_style & XIMPreeditArea) ) {
		XtUnmapWidget( xic->buffer->box[0] );
		XtUnmapWidget( xic->buffer->box[1] );
	    }
	    XtVaSetValues( xic->buffer->label,
			   XtNlabel, resources.sjxa, NULL );
	}
    }
}


static void
#if NeedFunctionPrototypes
XimResetIc( XClientMessageEvent *event, XimcpConnection *xc, CARD data )
#else
XimResetIc( event, xc, data )
    XClientMessageEvent	*event;
    XimcpConnection	*xc;
    CARD		 data;
#endif
{
    XimcpInputMethod	*xim;
    XimcpInputContext	*xic;
    CARD8		 reply[XIMCP_BUFSIZE];
    Buffer		*b;
    XTextProperty	 text;
    int			 n;
    wchar_t		*wcsp, wcs[512], *list[1];

    if( (xim = imid2xim( event->display, xc, swap16(data.s[2]), True ))
	== NULL )
	return;
    if( (xic = icid2xic( event->display, xim, swap16(data.s[3]), True ))
	== NULL )
	return;

    if( xic->reset_state == XIMInitialState ) {
	((CARD16 *)reply)[0] = swap16(xim->imid);
	((CARD16 *)reply)[1] = swap16(xic->icid);
	((CARD32 *)reply)[1] = NoEventMask;
	((CARD32 *)reply)[2] = NoEventMask;
	XimSendClientMessage( event->display, xc, XIM_SET_EVENT_MASK, 12,
			      (CARD8 *)reply );
    }

    b = xic->buffer;
    if( xic->preedit.state == XIMPreeditDisable ) {
	text.nitems = 0;
	text.value = (unsigned char *)malloc( 1 );
	text.value[0] = '\0';
    }
    else {
	if( b->state & ~(INTERMEDIATE | CONVERSION) ) {
	    n = 0;
	    cancel( b->preedit_widget, event, NULL, &n );
	}
	if( b->state & INTERMEDIATE ) {
	    register_buffer( b );
	    wscpy( wcs, b->yomi );
	}
	else if( b->state & CONVERSION ) {
	    for( n = 0, wcsp = wcs; b->henkan->bunsetsu[n] != BUNSETSU_END;
		 n++ ) {
		if( b->henkan->bun[n].deststr == NULL ) {
		    wsncpy( wcsp, b->henkan->bun[n].srcstr,
			    b->henkan->bun[n].srclen );
		    wcsp += b->henkan->bun[n].srclen;
		}
		else {
		    wsncpy( wcsp, b->henkan->bun[n].deststr,
			    b->henkan->bun[n].destlen );
		    wcsp += b->henkan->bun[n].destlen;
		}
	    }
	    *wcsp = '\0';
	    register_buffer( b );
	}
	if( wcs[0] != (wchar_t)NULL ) {
	    list[0] = wcs;
	    XwcTextListToTextProperty( event->display, list, 1,
				       XCompoundTextStyle, &text );
	}
	else {
	    text.nitems = 0;
	    text.value = (unsigned char *)malloc( 1 );
	    text.value[0] = '\0';
	}
    }
    if( text.nitems >= 3  &&
	!strncmp( (char *)&text.value[text.nitems-3], "\033(B", 3 ) )
	text.nitems -= 3;

    ((CARD16 *)reply)[2] = swap16(xim->imid);
    ((CARD16 *)reply)[3] = swap16(xic->icid);
    ((CARD16 *)reply)[4] = swap16(text.nitems);
    memcpy( &reply[10], text.value, text.nitems );
    n = 4 + 6 + text.nitems;
    if( n <= 20 )
	XimSendClientMessage( event->display, xc, XIM_RESET_IC_REPLY, n - 4,
			      &reply[4] );
    else {
	reply[0] = XIM_RESET_IC_REPLY;
	reply[1] = 0;
	XimSendProperty( event->display, xim->xc, xim_reset, n, reply );
    }
    XFree( text.value );

    b->state = INTERMEDIATE;
    if( xic->reset_state == XIMInitialState )
	XimEnd( event->display, xic );

#ifdef USE_SJ3LIB
    if( !CheckState( CONVERSION|EDIT|ADD|DELETE|SELECT|WRAP ) ) {
	sj3_unlockserv();
	sj3_close();
    }
#endif
}


static void
#if NeedFunctionPrototypes
XimTriggerNotify( Display *display, XimcpConnection *xc, CARD data )
#else
XimTriggerNotify( display, xc, data )
    Display		*display;
    XimcpConnection	*xc;
    CARD		 data;
#endif
{
    XimcpInputMethod	*xim;
    XimcpInputContext	*xic;
    CARD16		 reply[8];
    Arg			 args[1];

    if( (xim = imid2xim( display, xc, swap16(data.s[2]), False )) == NULL )
	return;
    if( (xic = icid2xic( display, xim, swap16(data.s[3]), False )) == NULL )
	return;

    if( data.l[2] == 0 ) {
#ifdef DEBUG
	if( resources.debug & 0x10 )
	    puts( " ON" );
#endif
	if( xic->preedit.state == XIMPreeditDisable ) {
	    xic->preedit.state = XIMPreeditEnable;
	    reply[0] = xim->imid;
	    reply[1] = xic->icid;
	    ((CARD32 *)reply)[1] = swap32(KeyPressMask);
	    ((CARD32 *)reply)[2] = NoEventMask;
	    XimSendClientMessage( display, xc, XIM_SET_EVENT_MASK, 12,
				  (CARD8 *)reply );

	    xic->buffer->connect++;
	    if( xic->input_style & (XIMPreeditNothing | XIMStatusNothing) )
		RootWindowMap( xic->buffer, xic->input_style );
	    if( xic->input_style & (XIMPreeditPosition | XIMPreeditArea) ) {
		buffer_initialize( xic->buffer );
		XmwPreeditMapWidget( xic->buffer->preedit_widget );
	    }
	    if( xic->input_style & XIMStatusArea ) {
		switch( xic->buffer->conversion_mode ) {
		  case ZENKAKU_HIRAGANA:
		    XtSetArg( args[0], XtNlabel, resources.zenkaku_hiragana );
		    break;
		  case ZENKAKU_KATAKANA:
		    XtSetArg( args[0], XtNlabel, resources.zenkaku_katakana );
		    break;
		  case HANKAKU_HIRAGANA:
		  case HANKAKU_KATAKANA:
		    XtSetArg( args[0], XtNlabel, resources.hankaku_katakana );
		    break;
		  case ZENKAKU_ALPHA:
		    XtSetArg( args[0], XtNlabel, resources.zenkaku_alpha );
		    break;
		  case HANKAKU_ALPHA:
		    XtSetArg( args[0], XtNlabel, resources.hankaku_alpha );
		    break;
		  case SJIS:
		    XtSetArg( args[0], XtNlabel, resources.sjis );
		    break;
		  case EUC:
		    XtSetArg( args[0], XtNlabel, resources.euc );
		    break;
		  case JIS:
		    XtSetArg( args[0], XtNlabel, resources.jis );
		    break;
		  case KUTEN:
		    XtSetArg( args[0], XtNlabel, resources.kuten );
		    break;
		  case SUPP_EUC:
		    XtSetArg( args[0], XtNlabel, resources.supplementary_euc );
		    break;
		  case SUPP_JIS:
		    XtSetArg( args[0], XtNlabel, resources.supplementary_jis );
		    break;
		  case SUPP_KUTEN:
		    XtSetArg( args[0], XtNlabel, resources.supplementary_kuten );
		    break;
		}
		XtSetValues( xic->buffer->label, args, 1 );
		if( !(xic->input_style & XIMPreeditArea)  &&
		    xic->status.area.width >= xic->status.area_needed.width ) {
		    XtMapWidget( xic->buffer->box[0] );
		    XtMapWidget( xic->buffer->box[1] );
		}
	    }

	    if( xic->input_style & (XIMPreeditPosition | XIMPreeditArea) ) {
		XtVaSetValues( xic->buffer->preedit_widget, XmwNfocus, True,
			       NULL );
	    }
	    set_buffer( xic->buffer->preedit_widget, KeyPress );
	}
    }
    else {
#ifdef DEBUG
	if( resources.debug & 0x10 )
	    puts( " OFF" );
#endif
	if( xic->preedit.state == XIMPreeditEnable ) {
	    xic->preedit.state = XIMPreeditDisable;
	    reply[0] = swap16(xim->imid);
	    reply[1] = swap16(xic->icid);
	    ((CARD32 *)reply)[1] = NoEventMask;
	    ((CARD32 *)reply)[2] = NoEventMask;
	    XimSendClientMessage( display, xc, XIM_SET_EVENT_MASK, 12,
				  (CARD8 *)reply );
	    xic->buffer->connect--;
	    if( xic->input_style & (XIMPreeditNothing | XIMStatusNothing) )
		RootWindowUnmap( xic->input_style );
	    if( xic->input_style & (XIMPreeditPosition | XIMPreeditArea) )
		XmwPreeditUnmapWidget( xic->buffer->preedit_widget );
	    if( xic->input_style & XIMStatusArea ) {
		XtVaSetValues( xic->buffer->label, XtNlabel, resources.sjxa,
			       NULL );
		if( !(xic->input_style & XIMPreeditArea) ) {
		    XtUnmapWidget( xic->buffer->box[0] );
		    XtUnmapWidget( xic->buffer->box[1] );
		}
	    }
	}
    }

    reply[0] = swap16(xim->imid);
    reply[1] = swap16(xic->icid);
    XimSendClientMessage( display, xc, XIM_TRIGGER_NOTIFY_REPLY, 4,
			  (CARD8 *)reply );
}


static void
#if NeedFunctionPrototypes
XimExtForwardKeyevent( XClientMessageEvent *event, XimcpConnection *xc,
		       CARD data )
#else
XimExtForwardKeyevent( event, xc, data )
    XClientMessageEvent	*event;
    XimcpConnection	*xc;
    CARD		 data;
#endif
{
    XimcpInputMethod	*xim;
    XimcpInputContext	*xic;
    XKeyEvent		 key;

    if( (xim = imid2xim( event->display, xc, swap16(data.s[2]), False ))
	== NULL )
	return;
    if( (xic = icid2xic( event->display, xim, swap16(data.s[3]), False ))
	== NULL )
	return;

    key.type = event->data.b[12];
    key.serial = event->serial;
    key.send_event = False;
    key.display = event->display;
    key.window = xic->focus_window;
    key.root = DefaultRootWindow(event->display);
    key.time = swap32(data.l[4]);
    key.x = 0;
    key.y = 0;
    key.x_root = 0;
    key.y_root = 0;
    key.state = swap16(data.s[7]);
    key.keycode = event->data.b[13];
    key.same_screen = True;
    XPutBackEvent( event->display, (XEvent *)&key );
}


static void
#if NeedFunctionPrototypes
XimExtMove( Display *display, XimcpConnection *xc, CARD data )
#else
XimExtMove( display, xc, data )
    Display		*display;
    XimcpConnection	*xc;
    CARD		 data;
#endif
{
    XimcpInputMethod	*xim;
    XimcpInputContext	*xic;
    Arg			 args[2];
    short		 x, y;

    if( (xim = imid2xim( display, xc, swap16(data.s[2]), False )) == NULL )
	return;
    if( (xic = icid2xic( display, xim, swap16(data.s[3]), False )) == NULL )
	return;

    if( xic->input_style & XIMPreeditPosition ) {
	xic->preedit.spot_location.x = x = swap16(data.s[4]);
	xic->preedit.spot_location.y = y = swap16(data.s[5]);
	XtSetArg( args[0], XmwNspotLocationX, x );
	XtSetArg( args[1], XmwNspotLocationY, y - xic->preedit.ascent );
	XtSetValues( xic->buffer->preedit_widget, args, 2 );
#ifdef	DEBUG
	if( resources.debug & 0x20 ) {
	    printf( " Set PreeditArea : x : %d\n", (int)x );
	    printf( " Set PreeditArea : y : %d\n", (int)y );
	}
#endif
    }
}


static void
#if NeedFunctionPrototypes
ximcp_connect( XClientMessageEvent *event )
#else
ximcp_connect( event )
    XClientMessageEvent	*event;
#endif
{
    XimcpConnection	*xc;
    Window		 window = (Window)event->data.l[0];
    XClientMessageEvent	 ev;

    xc = (XimcpConnection *)calloc( 1, sizeof(XimcpConnection) );

    xc->ims_window = XCreateSimpleWindow( event->display, event->window,
					  0, 0, 1, 1, 0, 0, 0 );
    xc->client_window = window;		/* XXXXX */
    xc->endian = 0;
    xc->xim = NULL;

    XSelectInput( event->display, window, StructureNotifyMask );

    xc->next = connection;
    connection = xc;
    if( xc->next != NULL )
	xc->next->previous = xc;
    xc->previous = NULL;

    ev.type = ClientMessage;
    ev.window = window;
    ev.message_type = xim_connect;
    ev.format = 32;
    ev.data.l[0] = xc->ims_window;
    ev.data.l[1] = 0;
    ev.data.l[2] = 0;
    XSendEvent( event->display, window, False, 0, (XEvent *)&ev );
}


static void
#if NeedFunctionPrototypes
ximcp_receive( XClientMessageEvent *event )
#else
ximcp_receive( event )
    XClientMessageEvent	*event;
#endif
{
    XimcpConnection	*xc = connection;
    Atom		 actual_type;
    int			 actual_format;
    unsigned long	 nitems;
    unsigned long	 bytes_after;
    CARD		 data;

    event->window = xc->ims_window;

    if( event->format == 8 ) {
	data.b = (CARD8 *)&event->data.b[0];
    }
    else {
	if( XGetWindowProperty( event->display, xc->ims_window,
				(Atom)event->data.l[1], 0L, 1000000L, True,
				AnyPropertyType, &actual_type, &actual_format,
				&nitems, &bytes_after,
				(unsigned char **)&data ) != Success ) {
	    XimSendErrorMessage( event->display, xc, 0, 0,
				 XIM_ERROR_BAD_PROTOCOL );
	    return;
	}
    }

    last_endian = current_endian;
    current_endian = xc->endian;

    switch( data.b[0] ) {
      case XIM_CONNECT:
	current_endian = xc->endian = data.b[4];
	if( event->format == 8 )
	    xc->bug = (swap16( data.s[1] ) > 4);
	else
	    xc->bug = (swap16( data.s[1] ) == nitems - 4);
#ifdef	DEBUG
	if( resources.debug & 0x10 )
	    puts( xc->bug ? "XIM_CONNECT:bug" : "XIM_CONNECT" );
#endif
	XimConnect( event->display, xc, data );
	break;

      case XIM_DISCONNECT:
#ifdef	DEBUG
	if( resources.debug & 0x10 )
	    puts( "XIM_DISCONNECT" );
#endif
	XimDisconnect( event->display, xc, data );
	break;

      case XIM_OPEN:
#ifdef	DEBUG
	if( resources.debug & 0x10 )
	    puts( "XIM_OPEN" );
#endif
	XimOpen( event->display, xc, data );
	break;

      case XIM_CLOSE:
#ifdef	DEBUG
	if( resources.debug & 0x10 )
	    puts( "XIM_CLOSE" );
#endif
	XimClose( event->display, xc, data );
	break;

      case XIM_TRIGGER_NOTIFY:
#ifdef	DEBUG
	if( resources.debug & 0x10 )
	    puts( "XIM_TRIGGER_NOTIFY" );
#endif
	XimTriggerNotify( event->display, xc, data );
	break;

      case XIM_ENCODING_NEGOTIATION:
#ifdef	DEBUG
	if( resources.debug & 0x10 )
	    puts( "XIM_ENCODING_NEGOTIATION" );
#endif
	XimEncodingNegotiation( event->display, xc, data );
	break;

      case XIM_QUERY_EXTENSION:
#ifdef	DEBUG
	if( resources.debug & 0x10 )
	    puts( "XIM_QUERY_EXTENSION" );
#endif
	XimQueryExtension( event->display, xc, data );
	break;

      case XIM_SET_IM_VALUES:
#ifdef DEBUG
	if( resources.debug & 0x10 )
	    puts( "XIM_SET_IM_VALUES" );
#endif
	XimSetImValues( event->display, xc, data );
	break;

      case XIM_GET_IM_VALUES:
#ifdef DEBUG
	if( resources.debug & 0x10 )
	    puts( "XIM_GET_IM_VALUES" );
#endif
	XimGetImValues( event->display, xc, data );
	break;

      case XIM_CREATE_IC:
#ifdef DEBUG
	if( resources.debug & 0x10 )
	    puts( "XIM_CREATE_IC" );
#endif
	XimCreateIc( event->display, xc, data );
	break;

      case XIM_DESTROY_IC:
#ifdef DEBUG
	if( resources.debug & 0x10 )
	    puts( "XIM_DESTROY_IC" );
#endif
	XimDestroyIc( event->display, xc, data );
	break;

      case XIM_SET_IC_VALUES:
#ifdef DEBUG
	if( resources.debug & 0x10 )
	    puts( "XIM_SET_IC_VALUES" );
#endif
	XimSetIcValues( event->display, xc, data );
	break;

      case XIM_GET_IC_VALUES:
#ifdef DEBUG
	if( resources.debug & 0x10 )
	    puts( "XIM_GET_IC_VALUES" );
#endif
	XimGetIcValues( event->display, xc, data );
	break;

      case XIM_SET_IC_FOCUS:
#ifdef DEBUG
	if( resources.debug & 0x10 )
	    puts( "XIM_SET_IC_FOCUS" );
#endif
	XimSetIcFocus( event->display, xc, data );
	break;

      case XIM_UNSET_IC_FOCUS:
#ifdef DEBUG
	if( resources.debug & 0x10 )
	    puts( "XIM_UNSET_IC_FOCUS" );
#endif
	XimUnsetIcFocus( event->display, xc, data );
	break;

      case XIM_FORWARD_EVENT:
#ifdef DEBUG
	if( resources.debug & 0x10 )
	    puts( "XIM_FORWARD_EVENT" );
#endif
	XimForwardEvent( event->display, xc, data );
	break;

      case XIM_SYNC:
#ifdef DEBUG
	if( resources.debug & 0x10 )
	    puts( "XIM_SYNC" );
#endif
	XimSync( event->display, xc, data );
	break;

      case XIM_RESET_IC:
#ifdef DEBUG
	if( resources.debug & 0x10 )
	    puts( "XIM_RESET_IC" );
#endif
	XimResetIc( event, xc, data );
	break;

      case XIM_EXT_FORWARD_KEYEVENT:
#ifdef DEBUG
	if( resources.debug & 0x10 )
	    puts( "XIM_EXT_FORWARD_KEYEVENT" );
#endif
	XimExtForwardKeyevent( event, xc, data );
	break;

      case XIM_EXT_MOVE:
#ifdef DEBUG
	if( resources.debug & 0x10 )
	    puts( "XIM_EXT_MOVE" );
#endif
	XimExtMove( event->display, xc, data );
	break;

      case XIM_ERROR:
	if( resources.debug & 0xffff )
	    fprintf( stderr, "%s : XIM_ERROR protocol received.\n",
		     program_name );
	break;

      default:
#ifdef	DEBUG
	fprintf( stderr, "%s : unknown XIM common protocol received. [%d]\n",
		 program_name, data.b[0] );
#endif
	XimSendErrorMessage( event->display, xc, 0, 0,
			     XIM_ERROR_BAD_PROTOCOL );
	break;
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
    XSelectionEvent	 xselection;
    Display		*display = event->xany.display;

    switch( event->type ) {
      case SelectionClear:
	if( event->xselectionclear.selection == xim_selection ) {
	    fprintf( stderr, "%s : selection clear event occured.\n\r",
		     program_name );
	    done();
	}
	break;

      case SelectionRequest:
	xselection.type = SelectionNotify;
	xselection.requestor = event->xselectionrequest.requestor;
	xselection.selection = event->xselectionrequest.selection;
	xselection.target = event->xselectionrequest.target;
	xselection.time = event->xselectionrequest.time;
	xselection.property = event->xselectionrequest.property;
#ifdef DEBUG
	if( resources.debug & 0x10 ) {
	    printf( "requestor : 0x%08x\n",
		    event->xselectionrequest.requestor );
	    printf( "selection : %s\n", XGetAtomName( display,
					event->xselectionrequest.selection ) );
	    printf( "target    : %s\n", XGetAtomName( display,
					event->xselectionrequest.target ) );
	    printf( "property  : %s\n", XGetAtomName( display,
					event->xselectionrequest.property ) );
	}
#endif
	if( event->xselectionrequest.selection == xim_selection ) {
	    if( event->xselectionrequest.target == xim_transport )
		XChangeProperty( display, xselection.requestor,
				 xselection.property, xselection.target,
				 8, PropModeReplace,
				 (unsigned char *)XIM_TRANSPORT_REPLY,
				 strlen(XIM_TRANSPORT_REPLY) );
	    else if( event->xselectionrequest.target == xim_locales )
		XChangeProperty( display, xselection.requestor,
				 xselection.property, xselection.target,
				 8, PropModeReplace,
				 (unsigned char *)XIM_LOCALES_REPLY,
				 strlen(XIM_LOCALES_REPLY) );
	    else
		xselection.property = None;
	}
	else
	    xselection.property = None;
	XSendEvent( display, xselection.requestor, False, 0,
		    (XEvent *)&xselection );
	break;

      case ClientMessage:
	if( event->xclient.message_type == xim_protocol )
	    ximcp_receive( (XClientMessageEvent *)event );
	else if( event->xclient.message_type == xim_connect )
	    ximcp_connect( (XClientMessageEvent *)event );
	break;
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
    XPropertyEvent	*ev = (XPropertyEvent *)event;
    Display		*display = XtDisplay(w);
    Window		 window = XtWindow(w);
    Window		 root = RootWindow(display, 0);
    char		 ss[256];
    int			 ii, jj;
    Atom		 type, *prop;
    int			 format;
    unsigned long	 nitems, after;

    XGrabServer( display );

    sprintf( ss, "@server=%s", program_name );
    xim_selection = XInternAtom( display, ss, False );
    XSetSelectionOwner( display, xim_selection, window, ev->time );

    XGetWindowProperty( display, root, xim_servers, 0, 10000, True, XA_ATOM,
			&type, &format, &nitems, &after,
			(unsigned char **)&prop );

    if( type == None  &&  format == 0  &&  after == 0 )
	XChangeProperty( display, root, xim_servers, XA_ATOM, 32,
			 PropModeReplace, (unsigned char *)&xim_selection, 1 );
    else {
	for( ii = 0; ii < nitems; ii++ ) {
	    if( prop[ii] == xim_selection ) {
		for( jj = ii; jj > 0; jj-- )
		    prop[jj] = prop[jj-1];
		ii = -1;
		break;
	    }
	}
	if( ii == nitems ) {
	    nitems++;
	    prop = (Atom *)realloc( (char *)prop, sizeof(Atom) * nitems );
	    for( ii = nitems - 1; ii > 0; ii-- )
		prop[ii] = prop[ii-1];
	}
	prop[0] = xim_selection;
	XChangeProperty( display, root, xim_servers, XA_ATOM, 32,
			 PropModeReplace, (unsigned char *)prop, nitems );
	XFree( (char *)prop );
    }

    XUngrabServer( display );

    XtRemoveEventHandler( (Widget)client_data, PropertyChangeMask, False,
			  procPropertyChangeMask, client_data );
}


void
#if NeedFunctionPrototypes
ximcp_initialize( Widget TopLevel )
#else
ximcp_initialize( TopLevel )
    Widget	TopLevel;
#endif
{
    Display	*display = XtDisplay(TopLevel);

    XtAddEventHandler( TopLevel, NoEventMask, True, procNoEventMask, NULL );
    XtAddEventHandler( TopLevel, PropertyChangeMask, False,
		       procPropertyChangeMask, (XtPointer)TopLevel );

    xim_servers	= XInternAtom( display, _XIM_SERVERS, False );
    xim_transport = XInternAtom( display, _XIM_TRANSPORT, False );
    xim_locales = XInternAtom( display, _XIM_LOCALES, False );

    xim_connect = XInternAtom( display, _XIM_XCONNECT, False );
    xim_protocol = XInternAtom( display, _XIM_PROTOCOL, False );

    xim_open = XInternAtom( display, _XIM_OPEN, False );
    xim_register_triggerkeys = XInternAtom( display, _XIM_REGISTER_TRIGGERKEYS,
					    False );
    xim_query_extension = XInternAtom( display, _XIM_QUERY_EXTENSION, False );
    xim_im_values = XInternAtom( display, _XIM_IM_VALUES, False );
    xim_ic_values = XInternAtom( display, _XIM_IC_VALUES, False );
    xim_commit = XInternAtom( display, _XIM_COMMIT, False );
    xim_reset = XInternAtom( display, _XIM_RESET, False );
    xim_forward_event = XInternAtom( display, _XIM_FORWARD_EVENT, False );

    current_endian = last_endian = endian;
}


Boolean
#if NeedFunctionPrototypes
XimcpChangeClientMessage( Widget TopLevel, XEvent *event )
#else /* NeedFunctionPrototypes */
XimcpChangeClientMessage( TopLevel, event )
    Widget	 TopLevel;
    XEvent	*event;
#endif /* NeedFunctionPrototypes */
{
    XimcpConnection   *xc;

    if( event->xclient.message_type == xim_protocol ) {
	for( xc = connection; xc != NULL; xc = xc->next ) {
	    if( xc->ims_window == event->xclient.window ) {
		if( xc->previous ) {
		    xc->previous->next = xc->next;
		    xc->next->previous = xc->previous;
		    xc->next = connection;
		    xc->previous = NULL;
		    connection = xc;
		}
		event->xclient.window = XtWindow(TopLevel);
		return( True );
	    }
	}
    }
    return( False );
}


Boolean
#if NeedFunctionPrototypes
XimcpChangeExpose( XEvent *event )
#else /* NeedFunctionPrototypes */
XimcpChangeExpose( event )
    XEvent	*event;
#endif /* NeedFunctionPrototypes */
{
    XimcpConnection	*xc;
    XimcpInputMethod	*xim;
    XimcpInputContext	*xic;

    for( xc = connection; xc != NULL; xc = xc->next ) {
	for( xim = xc->xim; xim != NULL; xim = xim->next ) {
	    for( xic = xim->xic; xic != NULL; xic = xic->next ) {
		if( xic->focus_window == event->xexpose.window  &&
		    (xic->input_style &
		     (XIMPreeditPosition | XIMPreeditArea)) ) {
		    event->xexpose.window =
			XtWindow( xic->buffer->preedit_widget );
		    return( True );
		}
	    }
	}
    }
    return( False );
}


Boolean
#if NeedFunctionPrototypes
XimcpDestroyNotify( XEvent *event )
#else
XimcpDestroyNotify( event )
    XEvent	*event;
#endif
{
    XimcpConnection	*xc;
    XimcpInputMethod	*xim1, *xim2;
    XimcpInputContext	*xic1, *xic2;

    for( xc = connection; xc != NULL; xc = xc->next ) {
	if( xc->client_window == event->xdestroywindow.window ) {
	    for( xim1 = xc->xim; xim1 != NULL; xim1 = xim2 ) {
		for( xic1 = xim1->xic; xic1 != NULL; xic1 = xic2 ) {
		    xic2 = xic1->next;
		    destroy_ic( xic1 );
		}
		xim2 = xim1->next;
		free( xim1 );
	    }
	    if( xc->previous )
		xc->previous->next = xc->next;
	    else
		connection = xc->next;
	    if( xc->next )
		xc->next->previous = xc->previous;
	    XDestroyWindow( event->xdestroywindow.display, xc->ims_window );
	    free( (char *)xc );
	    return( True );
	    break;
	}
    }
    return( False );
}
#endif
