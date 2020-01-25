/* $SonyId: Error.c,v 1.1 1994/05/30 13:03:58 makoto Exp $ */
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
#include	<X11/Xaw/Dialog.h>
#include	<Xmw/Command.h>
#include	<Xmw/Util.h>
#include	"sjxa.h"

extern char	*program_name;
extern int	*depths;


void
#if NeedFunctionPrototypes
error_cancel( Widget w, caddr_t client_data, caddr_t call_data )
#else /* NeedFunctionPrototypes */
error_cancel( w, client_data, call_data )
    Widget	w;
    caddr_t	client_data, call_data;
#endif /* NeedFunctionPrototypes */
{
    Buffer	*b = (Buffer *)client_data;

    XtDestroyWidget( b->error_shell );
    b->error_shell = NULL;
    b->state &= ~ERROR;
}


void
#if NeedFunctionPrototypes
server_error( Buffer *b, Widget widget, String string )
#else /* NeedFunctionPrototypes */
server_error( b, widget, string )
    Buffer	*b;
    Widget	widget;
    String	string;
#endif /* NeedFunctionPrototypes */
{
    Arg			args[10];
    int			n;
    Widget		dialog, label, icon, ok;
    Position		x, y;
    Dimension		width, height;
    Pixel		fg, bg, lbg;
    Colormap		colormap;
    int			depth;
    Screen		*screen;

    if( b->state & ERROR )
	return;

    XtVaGetValues( widget, XtNforeground, &fg, XtNbackground, &bg,
		   XtNcolormap, &colormap, XtNdepth, &depth,
		   XtNscreen, &screen, NULL );
    n = XScreenNumberOfScreen( screen );
    lbg = ( depths[n] == 1 ) ? bg
			     : light_color( XtDisplay(widget), colormap, bg );

    n = 0;
    XtSetArg( args[n], XtNtitle, "sjxa ErrorDialog" );		n++;
    XtSetArg( args[n], XtNcolormap, colormap );			n++;
    XtSetArg( args[n], XtNbackground, bg );			n++;
    XtSetArg( args[n], XtNdepth, depth );			n++;
    XtSetArg( args[n], XtNscreen, screen );			n++;
    XtSetArg( args[n], XtNiconic, False );			n++;
    XtSetArg( args[n], XtNgeometry, "+0+0" );			n++;
    XtSetArg( args[n], XtNmappedWhenManaged, False );		n++;
    b->error_shell = XtAppCreateShell( program_name, CLASS_NAME,
				       topLevelShellWidgetClass,
				       XtDisplay(widget), args, n );

    n = 0;
    XtSetArg( args[n], XtNbackground, bg );			n++;
    XtSetArg( args[n], XtNlabel, string );			n++;
    dialog = XtCreateManagedWidget( "Error", dialogWidgetClass, b->error_shell,
				    args, n );

    n = 0;
    XtSetArg( args[n], XtNforeground, fg );			n++;
    XtSetArg( args[n], XtNbackground, lbg );			n++;
    XtSetArg( args[n], XtNborderColor, fg );			n++;
    ok = XtCreateManagedWidget( "Ok", commandWidgetClass, dialog, args, n );
    XtAddCallback( ok, XtNcallback, (XtCallbackProc)error_cancel, (caddr_t)b );

    label = XtNameToWidget( dialog, "label" );
    XtVaSetValues( label, XtNforeground, fg, XtNbackground, bg, NULL );
    icon = XtNameToWidget( dialog, "icon" );
    if( icon )
	XtVaSetValues( icon, XtNforeground, fg, XtNbackground, bg, NULL );

    XtRealizeWidget( b->error_shell );

    XtVaGetValues( b->error_shell, XtNwidth, &width, XtNheight, &height, NULL );
    XtVaSetValues( b->error_shell, XtNminWidth, width, XtNminHeight, height,
		   XtNmaxWidth, width, XtNmaxHeight, height, NULL );
    x = ( WidthOfScreen(screen) - width ) / 2;
    y = ( HeightOfScreen(screen) - height ) / 2;
    XtMoveWidget( b->error_shell, x, y );

    XtMapWidget( b->error_shell );

    XmwInstallTranslations( dialog, b->preedit_widget );
    XtAddGrab( b->error_shell, False, False );

    b->state |= ERROR;
}
