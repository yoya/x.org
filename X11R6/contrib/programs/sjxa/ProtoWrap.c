/* $SonyId: ProtoWrap.c,v 1.1 1994/05/30 13:03:58 makoto Exp $ */
/******************************************************************

Copyright (c) 1994  Sony Corporation

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
#include	"Ximcp.h"
#include	"Ximp.h"
#include	"Xlcp.h"
#include	"Sjxp.h"

Boolean
#if NeedFunctionPrototypes
SendCompoundText( Display *display, Window window, wchar_t *string )
#else
SendCompoundText( display, window, string )
    Display	*display;
    Window	 window;
    wchar_t	*string;
#endif
{
#if defined(USE_XIMCP)
    if( XimcpSendCompoundText( display, window, string ) )
	return( True );
#endif
#ifdef USE_XIMP
    if( XimpSendCompoundText( display, window, string ) )
	return( True );
#endif
#ifdef USE_XLC
    if( XlcpSendCompoundText( display, window, string ) )
	return( True );
#endif
#ifdef USE_SJX
    if( SjxpSendText( window, string ) )
	return( True );
#endif
    return( False );
}


Boolean
#if NeedFunctionPrototypes
CheckState( State state )
#else
CheckState( state )
    State	state;
#endif
{
#if defined(USE_XIMCP)
    if( XimcpCheckState( state ) )
	return( True );
#endif
#ifdef USE_XIMP
    if( XimpCheckState( state ) )
	return( True );
#endif
#ifdef USE_XLC
    if( XlcpCheckState( state ) )
	return( True );
#endif
    return( False );
}


Boolean
#if NeedFunctionPrototypes
SendKeyEvent( Window window, XKeyEvent *event )
#else
SendKeyEvent( window, event )
    Window	 window;
    XKeyEvent	*event;
#endif
{
#if defined(USE_XIMCP)
    if( XimcpSendKeyEvent( window, event ) )
	return( True );
#endif
#ifdef USE_XIMP
    if( XimpSendKeyEvent( window, event ) )
	return( True );
#endif
#ifdef USE_XLC
    if( XlcpSendKeyEvent( window, event ) )
	return( True );
#endif
    return( False );
}


void
#if NeedFunctionPrototypes
Disconnect( Display *display, Window window )
#else
Disconnect( display, window )
    Display	*display;
    Window	 window;
#endif
{
#ifdef USE_XIMP
    ximp_end( display, window );
#endif
#ifdef USE_XLC
    xlcp_all_end( window );
#endif
#ifdef USE_SJX
    sjxp_end();
#endif
}


Boolean
#if NeedFunctionPrototypes
IsRegisteredWindow( Protocol protocol, Window w1, Window w2 )
#else
IsRegisteredWindow( protocol, w1, w2 )
    Protocol	protocol;
    Window	w1, w2;
#endif
{
#ifdef USE_XIMP
    if( protocol != XIM_PROTOCOL  &&  XimpIsRegisteredWindow( w1, w2 ) )
	return( True );
#endif
#ifdef USE_XLC
    if( protocol != XLC_PROTOCOL  &&  XlcpIsRegisteredWindow( w1, w2 ) )
	return( True );
#endif
    return( False );
}


Boolean
#if NeedFunctionPrototypes
PreeditChangeKeyDirection( XEvent *event )
#else
PreeditChangeKeyDirection( event )
    XEvent	*event;
#endif
{
#ifdef USE_XIMP
    if( XimpPreeditChangeKeyDirection( event ) )
	return( True );
#endif
#ifdef USE_XLC
    if( XlcpPreeditChangeKeyDirection( event ) )
	return( True );
#endif
#ifdef USE_SJX
    if( SjxpPreeditChangeKeyDirection( event ) )
	return( True );
#endif
    return( False );
}


Boolean
#if NeedFunctionPrototypes
ChangeDestroyNotify( XEvent *event )
#else
ChangeDestroyNotify( event )
    XEvent	*event;
#endif
{
#if defined(USE_XIMCP)
    if( XimcpDestroyNotify( event ) )
	return( True );
#endif
#ifdef USE_XIMP
    if( XimpDestroyNotify( event ) )
	return( True );
#endif
#ifdef USE_XLC
    if( XlcpDestroyNotify( event ) )
	return( True );
#endif
    return( False );
}


Boolean
#if NeedFunctionPrototypes
ChangeExpose( XEvent *event )
#else
ChangeExpose( event )
    XEvent	*event;
#endif
{
#if defined(USE_XIMCP)
    if( XimcpChangeExpose( event ) )
	return( True );
#endif
#ifdef USE_XIMP
    if( XimpChangeExpose( event ) )
	return( True );
#endif
#ifdef USE_XLC
    if( XlcpChangeExpose( event ) )
	return( True );
#endif
    return( False );
}


Boolean
#if NeedFunctionPrototypes
ChangeClientMessage( Widget w, XEvent *event )
#else
ChangeClientMessage( w, event )
    Widget	 w;
    XEvent	*event;
#endif
{
#if defined(USE_XIMCP)
    if( XimcpChangeClientMessage( w, event ) )
	return( True );
#endif
    return( False );
}
