/* $SonyId: Reconnect.c,v 1.1 1994/05/30 13:03:58 makoto Exp $ */
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
#include	<X11/Intrinsic.h>
#include	<X11/StringDefs.h>
#include	<X11/Shell.h>
#include	<X11/Xaw/Form.h>
#include	<Xmw/Command.h>
#include	<Xmw/TextOut.h>
#include	<Xmw/Util.h>
#include	"sjxa.h"
#include	"resource.h"
#include	"SjBuffer.h"

Widget	Server;
Widget	ServerText;

#if	NeedFunctionPrototypes
Buffer	*set_buffer( Widget w, int type );
#else
Buffer	*set_buffer();
#endif

extern Sjxa		 resources;
extern Widget		 server_shell;
extern char		*program_name;
extern Boolean		 server_name_flag;
extern char		 server_name[];
extern int		*depths;


void
#if NeedFunctionPrototypes
reconnect_kettei( Widget w, caddr_t client_data, caddr_t call_data )
#else /* NeedFunctionPrototypes */
reconnect_kettei( w, client_data, call_data )
    Widget	w;
    caddr_t	client_data, call_data;
#endif /* NeedFunctionPrototypes */
{
    Buffer	*b = (Buffer *)client_data;
    char	*ss;

    XtDestroyWidget( server_shell );
    server_shell = NULL;

    XtVaGetValues( ServerText, XtNvalue, &ss, NULL );
    if( ss[0] != '\0' ) {
#ifdef USE_SJ3LIB
	sj3_close();
#endif
	if( !server_name_flag ) {
	    server_name_flag = True;
	    resources.server = server_name;
	}
	strcpy( resources.server, ss );
	XtFree( ss );
    }
    pop_buffer( b );
}


void
#if NeedFunctionPrototypes
reconnect_cancel( Widget w, caddr_t client_data, caddr_t call_data )
#else /* NeedFunctionPrototypes */
reconnect_cancel( w, client_data, call_data )
    Widget	w;
    caddr_t	client_data, call_data;
#endif /* NeedFunctionPrototypes */
{
    Buffer	*b = (Buffer *)client_data;

    XtDestroyWidget( server_shell );
    server_shell = NULL;

    pop_buffer( b );
}


void	reconnect( w, event, params, n )
Widget		 w;
XEvent		*event;
String		*params;
Cardinal	*n;
{
    Buffer	*b;
    Arg		 args[10];
    Widget	 form, label, cancel;
    Pixel	 fg, bg, lbg;
    Colormap	 colormap;
    int		 depth;
    Screen	*screen;
    int		 ii, len;
    Position	 x, y;
    Dimension	 width, height;

    if( server_shell != NULL )
	return;
    b = set_buffer( w, event->type );
    if( b == NULL  ||  !check_action_args( params, *n, "reconnect" ) )
	return;
    if( CheckState( ~INTERMEDIATE ) ) {
	server_error( b, w, resources.open_error1 );
	return;
    }

    XtVaGetValues( w, XtNforeground, &fg, XtNbackground, &bg,
		   XtNcolormap, &colormap, XtNdepth, &depth,
		   XtNscreen, &screen, NULL );
    ii = XScreenNumberOfScreen( screen );
    lbg = (depths[ii] == 1) ? bg : light_color( XtDisplay(w), colormap, bg );

    ii = 0;
    XtSetArg( args[ii], XtNtitle, "sjxa ServerDialog" );	ii++;
    XtSetArg( args[ii], XtNmappedWhenManaged, False );		ii++;
    XtSetArg( args[ii], XtNdepth, depth );			ii++;
    XtSetArg( args[ii], XtNscreen, screen );			ii++;
    XtSetArg( args[ii], XtNiconic, False );			ii++;
    XtSetArg( args[ii], XtNgeometry, "+0+0" );			ii++;
    server_shell = XtAppCreateShell( program_name, CLASS_NAME,
				     topLevelShellWidgetClass,
				     XtDisplay(w), args, ii );

    ii = 0;
    XtSetArg( args[ii], XtNforeground, fg );			ii++;
    XtSetArg( args[ii], XtNbackground, bg );			ii++;
    XtSetArg( args[ii], XtNcolormap, colormap );		ii++;
    XtSetArg( args[ii], XtNvalue, resources.server );		ii++;
    form = XtCreateManagedWidget( "Server", formWidgetClass, server_shell,
				  args, ii );

    ii = 0;
    XtSetArg( args[ii], XtNforeground, fg );			ii++;
    XtSetArg( args[ii], XtNbackground, bg );			ii++;
    XtSetArg( args[ii], XtNborderWidth, 0 );			ii++;
    XtSetArg( args[ii], XtNleft, XawChainLeft );		ii++;
    label = XtCreateManagedWidget( "ServerLabel", labelWidgetClass, form,
				   args, ii );

    ii = 0;
    XtSetArg( args[ii], XtNforeground, fg );			ii++;
    XtSetArg( args[ii], XtNbackground, lbg );			ii++;
    XtSetArg( args[ii], XtNfromVert, label );			ii++;
    XtSetArg( args[ii], XtNleft, XawChainLeft );		ii++;
    XtSetArg( args[ii], XtNright, XawChainRight );		ii++;
    ServerText = XmwCreateTextOut( form, "ServerText", args, ii );

    ii = 0;
    XtSetArg( args[ii], XtNforeground, fg );			ii++;
    XtSetArg( args[ii], XtNbackground, lbg );			ii++;
    XtSetArg( args[ii], XtNborderColor, fg );			ii++;
    XtSetArg( args[ii], XtNfromVert, ServerText );		ii++;
    XtSetArg( args[ii], XtNleft, XawChainLeft );		ii++;
    XtSetArg( args[ii], XtNright, XawChainLeft );		ii++;
    XtSetArg( args[ii], XtNsensitive, True );			ii++;
    Server = XtCreateManagedWidget( "Ok", commandWidgetClass, form, args, ii );
    XtAddCallback( Server, XtNcallback, (XtCallbackProc)reconnect_kettei,
		   (caddr_t)b );

    XtSetArg( args[ii], XtNfromHoriz, Server );			ii++;
    cancel = XtCreateManagedWidget( "Cancel", commandWidgetClass, form, args,
				    ii );
    XtAddCallback( cancel, XtNcallback, (XtCallbackProc)reconnect_cancel,
		   (caddr_t)b );

    len = strlen( resources.server );
    XmwTextOutSetString( ServerText, resources.server, XmwHIGHLIGHT_NORMAL );
    if( len == 0 )
	XtVaSetValues( Server, XtNsensitive, False, NULL );

    XtRealizeWidget( server_shell );

    XtVaGetValues( server_shell, XtNwidth, &width, XtNheight, &height, NULL );
    XtVaSetValues( server_shell, XtNminWidth, width, XtNminHeight, height,
		   XtNmaxWidth, width, XtNmaxHeight, height, NULL );

    XtVaGetValues( w, XtNscreen, &screen, NULL );
    x = ( WidthOfScreen( screen ) - width ) / 2;
    y = ( HeightOfScreen( screen ) - height ) / 2;
    XtMoveWidget( server_shell, x, y );
    XtMapWidget( server_shell );

    XmwInstallTranslations( form, b->preedit_widget );
    XmwInstallTranslations( label, b->preedit_widget );
    XmwInstallTranslations( ServerText, b->preedit_widget );
    XmwInstallTranslations( Server, b->preedit_widget );
    XmwInstallTranslations( cancel, b->preedit_widget );
    XtAddGrab( server_shell, False, False );

    push_buffer( b );
    buffer_initialize( b );
    b->state = SERVER;
    b->conversion_mode = HANKAKU_ALPHA;

    b->point = b->cursor = len;
    b->end_point = len + 1;
    strcpy( (char *)b->key, resources.server );
    strcpy( (char *)b->yomi, resources.server );
    for( ii = 0; ii <= len; ii++ ) {
	b->keyC[ii] = ii;
	b->yomiC[ii] = ii;
    }
    b->keyC[ii] = len;
    b->yomiC[ii] = len;
    for( ii = 0; ii < len; ii++ ) {
	b->mode[ii] = HANKAKU_ALPHA;
	b->conversion_flag[ii] = True;
    }
}
