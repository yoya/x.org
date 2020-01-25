/* $SonyId: Touroku.c,v 1.1 1994/05/30 13:03:58 makoto Exp $ */
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
#include	<stdio.h>
#include	<X11/Xlib.h>
#include	<X11/Intrinsic.h>
#include	<X11/StringDefs.h>
#include	<X11/Shell.h>
#include	<X11/Xaw/Dialog.h>
#include	<X11/Xaw/SimpleMenu.h>
#include	<Xmw/MenuBtn.h>
#include	<Xmw/TextOut.h>
#include	<Xmw/Preedit.h>
#include	<Xmw/SmeBSB.h>
#include	<Xmw/Util.h>
#include	"sjxa.h"
#include	"resource.h"
#include	"SjBuffer.h"
#include	"SjString.h"

static unsigned int	henkan_end_point, code;
static Widget	Hinshi;

Widget		TourokuText[2];
Widget		Touroku;

extern char	*program_name;
extern Sjxa	resources;
extern int	*depths;
extern int       hinshi_code[];


static void
#if NeedFunctionPrototypes
buttonCB( Widget w, caddr_t client_data, caddr_t call_data )
#else /* NeedFunctionPrototypes */
buttonCB( w, client_data, call_data )
    Widget	w;
    caddr_t	client_data, call_data;
#endif /* NeedFunctionPrototypes */
{
    int		n = (int)client_data;

    code = n;
    XtVaSetValues( Hinshi, XtNlabel, resources.hinshi[n], NULL );
}


void
#if NeedFunctionPrototypes
touroku_cancel( Widget w, caddr_t client_data, caddr_t call_data )
#else /* NeedFunctionPrototypes */
touroku_cancel( w, client_data, call_data )
    Widget	w;
    caddr_t	client_data, call_data;
#endif /* NeedFunctionPrototypes */
{
    Buffer	*b = (Buffer *)client_data;
    Henkan	*henkan;

    if( b->state & ERROR )
	error_cancel( w, client_data, call_data );

    XtDestroyWidget( b->shell );
    henkan = b->henkan;
    pop_buffer( b );
    b->shell = NULL;
    b->henkan = henkan;
    b->state = CONVERSION;
}


void
#if NeedFunctionPrototypes
touroku_kettei( Widget w, caddr_t client_data, caddr_t call_data )
#else /* NeedFunctionPrototypes */
touroku_kettei( w, client_data, call_data )
    Widget	w;
    caddr_t	client_data, call_data;
#endif /* NeedFunctionPrototypes */
{
    Buffer		*b = (Buffer *)client_data;
    wchar_t		*kanji;
    int			result;

    kanji = XmwTextOutGetStringWcs( TourokuText[0] );
    result = sj3_touroku_wcs( b->yomi, kanji, hinshi_code[code] );
    XtFree( (char *)kanji );

    if( !result ) {
	touroku_cancel( w, client_data, call_data );
	return;
    }

    switch( result ) {
      case -1:
	server_error( b, w, resources.died_error );
	break;

      case SJ3_WORD_EXIST:
	server_error( b, w, resources.add_error1 );
	break;

      case SJ3_BAD_YOMI_STR:
	server_error( b, w, resources.add_error2 );
	break;

      case SJ3_BAD_KANJI_STR:
	server_error( b, w, resources.add_error3 );
	break;

      case SJ3_DOUON_FULL:
	server_error( b, w, resources.add_error4 );
	break;

      case SJ3_DICT_FULL:
	server_error( b, w, resources.add_error5 );
	break;

      case SJ3_DICT_LOCKED:
	server_error( b, w, resources.add_error6 );
	break;

      default:
	server_error( b, w, resources.add_error7 );
	break;
    }
}


void	touroku( w, event, params, nn )
Widget		 w;
XEvent		*event;
String		*params;
Cardinal	*nn;
{
    Buffer		*b;
    int			 n, ii;
    Arg			 args[20];
    Pixel		 fg, bg, lbg;
    Colormap		 colormap;
    int			 depth;
    Screen		*screen;
    Widget		 form, yomi, kanji, part, menu, entry, cancel;
    Dimension		 width, height;
    char		 ss[20];
    wchar_t		 ws[512];
    unsigned int	 hp;
    Henkan		*henkan;

    b = set_buffer( w, event->type );
    if( b == NULL  ||  !(b->state & (CONVERSION | WRAP))  ||
	(b->state & ERROR)  ||  !check_action_args( params, *nn, "add-term" ) )
	return;

    if( b->state == WRAP )
	wrap_end( b, w, True );

    XtVaGetValues( w, XtNforeground, &fg, XtNbackground, &bg,
		   XtNcolormap, &colormap, XtNdepth, &depth,
		   XtNscreen, &screen, NULL );
    n = XScreenNumberOfScreen( screen );
    lbg = (depths[n] == 1) ? bg : light_color( XtDisplay(w), colormap, bg );

    n = 0;
    XtSetArg( args[n], XtNtitle, "sjxa AddTermDialog" );	n++;
    XtSetArg( args[n], XtNdepth, depth );			n++;
    XtSetArg( args[n], XtNscreen, screen );			n++;
    XtSetArg( args[n], XtNiconic, False );			n++;
    XtSetArg( args[n], XtNmappedWhenManaged, False );		n++;
    XtSetArg( args[n], XtNgeometry, "+0+0" );			n++;
    b->shell = XtAppCreateShell( program_name, CLASS_NAME,
				 topLevelShellWidgetClass, XtDisplay(w),
				 args, n );

    n = 0;
    XtSetArg( args[n], XtNforeground, fg );			n++;
    XtSetArg( args[n], XtNbackground, bg );			n++;
    form = XtCreateManagedWidget( "AddTerm", formWidgetClass, b->shell,
				  args, n );

    n = 0;
    XtSetArg( args[n], XtNforeground, fg );			n++;
    XtSetArg( args[n], XtNbackground, bg );			n++;
    XtSetArg( args[n], XtNborderWidth, 0 );			n++;
    XtSetArg( args[n], XtNtop, XawChainTop );			n++;
    XtSetArg( args[n], XtNbottom, XawChainTop );		n++;
    XtSetArg( args[n], XtNleft, XawChainLeft );			n++;
    XtSetArg( args[n], XtNright, XawChainLeft );		n++;
    kanji = XtCreateManagedWidget( "Term", labelWidgetClass, form, args, n );

    n = 0;
    XtSetArg( args[n], XtNforeground, fg );			n++;
    XtSetArg( args[n], XtNbackground, lbg );			n++;
    XtSetArg( args[n], XtNtop, XawChainTop );			n++;
    XtSetArg( args[n], XtNbottom, XawChainTop );		n++;
    XtSetArg( args[n], XtNleft, XawChainLeft );			n++;
    XtSetArg( args[n], XtNright, XawChainRight );		n++;
    XtSetArg( args[n], XtNfromHoriz, kanji );			n++;
    XtSetArg( args[n], XmwNdisplayCaret, False );		n++;
    TourokuText[0] = XmwCreateTextOut( form, "TermText", args, n );
    XtManageChild( TourokuText[0] );

    n = 0;
    XtSetArg( args[n], XtNforeground, fg );			n++;
    XtSetArg( args[n], XtNbackground, bg );			n++;
    XtSetArg( args[n], XtNborderWidth, 0 );			n++;
    XtSetArg( args[n], XtNtop, XawChainTop );			n++;
    XtSetArg( args[n], XtNbottom, XawChainTop );		n++;
    XtSetArg( args[n], XtNleft, XawChainLeft );			n++;
    XtSetArg( args[n], XtNright, XawChainLeft );		n++;
    XtSetArg( args[n], XtNfromVert, TourokuText[0] );		n++;
    yomi = XtCreateManagedWidget( "KeyWord", labelWidgetClass, form, args, n );

    n = 0;
    XtSetArg( args[n], XtNforeground, fg );			n++;
    XtSetArg( args[n], XtNbackground, lbg );			n++;
    XtSetArg( args[n], XtNtop, XawChainTop );			n++;
    XtSetArg( args[n], XtNbottom, XawChainTop );		n++;
    XtSetArg( args[n], XtNleft, XawChainLeft );			n++;
    XtSetArg( args[n], XtNright, XawChainRight );		n++;
    XtSetArg( args[n], XtNfromHoriz, yomi );			n++;
    XtSetArg( args[n], XtNfromVert, TourokuText[0] );		n++;
    TourokuText[1] = XmwCreateTextOut( form, "TermText", args, n );
    XtManageChild( TourokuText[1] );

    n = 0;
    XtSetArg( args[n], XtNforeground, fg );			n++;
    XtSetArg( args[n], XtNbackground, bg );			n++;
    XtSetArg( args[n], XtNborderWidth, 0 );			n++;
    XtSetArg( args[n], XtNtop, XawChainTop );			n++;
    XtSetArg( args[n], XtNbottom, XawChainTop );		n++;
    XtSetArg( args[n], XtNleft, XawChainLeft );			n++;
    XtSetArg( args[n], XtNright, XawChainLeft );		n++;
    XtSetArg( args[n], XtNfromVert, TourokuText[1] );		n++;
    part = XtCreateManagedWidget( "Part", labelWidgetClass, form, args, n );

    n = 0;
    XtSetArg( args[n], XtNforeground, fg );			n++;
    XtSetArg( args[n], XtNbackground, lbg );			n++;
    XtSetArg( args[n], XtNlabel, resources.hinshi[0] );		n++;
    XtSetArg( args[n], XtNborderColor, fg );			n++;
    XtSetArg( args[n], XtNtop, XawChainTop );			n++;
    XtSetArg( args[n], XtNbottom, XawChainTop );		n++;
    XtSetArg( args[n], XtNleft, XawChainLeft );			n++;
    XtSetArg( args[n], XtNright, XawChainLeft );		n++;
    XtSetArg( args[n], XtNfromHoriz, part );			n++;
    XtSetArg( args[n], XtNfromVert, TourokuText[1] );		n++;
    XtSetArg( args[n], XtNresizable, True );			n++;
    Hinshi = XtCreateManagedWidget( "menuButton", menuButtonWidgetClass, form,
				    args, n );

    n = 0;
    XtSetArg( args[n], XtNforeground, fg );			n++;
    XtSetArg( args[n], XtNbackground, lbg );			n++;
    XtSetArg( args[n], XtNborderColor, fg );			n++;
    menu = XtCreatePopupShell( "menu", simpleMenuWidgetClass, Hinshi, args, n );

    n = 0;
    XtSetArg( args[n], XtNforeground, fg );			n++;
    XtSetArg( args[n], XtNbackground, lbg );			n++;
    n++;
    for( ii = 0; ii < 30; ii++ ) {
	XtSetArg( args[n-1], XtNlabel, resources.hinshi[ii] );
	sprintf( ss, "Hinshi%02d", ii );
	entry = XtCreateManagedWidget( ss, smeBSBObjectClass, menu, args, n );
	XtAddCallback( entry, XtNcallback, (XtCallbackProc)buttonCB,
		       (caddr_t)ii );
    }

    n = 0;
    XtSetArg( args[n], XtNforeground, fg );			n++;
    XtSetArg( args[n], XtNbackground, lbg );			n++;
    XtSetArg( args[n], XtNborderColor, fg );			n++;
    XtSetArg( args[n], XtNfromVert, Hinshi );			n++;
    XtSetArg( args[n], XtNleft, XawChainLeft );			n++;
    XtSetArg( args[n], XtNright, XawChainLeft );		n++;
    Touroku = XtCreateManagedWidget( "Ok", commandWidgetClass, form, args, n );
    XtAddCallback( Touroku, XtNcallback, (XtCallbackProc)touroku_kettei,
		   (caddr_t)b );

    XtSetArg( args[n], XtNfromHoriz, Touroku );			n++;
    cancel = XtCreateManagedWidget( "Cancel", commandWidgetClass, form, args,
				    n );
    XtAddCallback( cancel, XtNcallback, (XtCallbackProc)touroku_cancel,
		   (caddr_t)b );

    XtRealizeWidget( b->shell );
    XmwInstallTranslations( TourokuText[1], b->preedit_widget );
    XmwInstallTranslations( form, b->preedit_widget );
    XtAddGrab( b->shell, False, False );

    XtVaGetValues( b->shell, XtNwidth, &width, XtNheight, &height, NULL );
    XtVaSetValues( b->shell, XtNminWidth, width, XtNminHeight, height,
		   XtNmaxHeight, height, NULL );
    if( b->conversion_style & (XIMPreeditPosition | XIMPreeditArea) )
	XmwPreeditMoveWidget( b->preedit_widget, b->yomiC[b->point], b->shell );
    else
	MoveWidget( b->shell );

    code = 0;
    XtVaSetValues( Touroku, XtNsensitive, False, NULL );

    henkan = b->henkan;
    b->henkan = NULL;
    push_buffer( b );
    buffer_initialize( b );
    b->henkan = henkan;
    b->state = ADD;
    b->conversion_style = XIMPreeditNothing | XIMStatusNothing;

    hp = henkan_end_point = b->henkan->point;
    if( b->henkan->bun[hp].deststr == NULL )
	wscpy( ws, b->henkan->bun[hp].srcstr );
    else
	wscpy( ws, b->henkan->bun[hp].deststr );
    ws[b->henkan->bun[hp].destlen] = (wchar_t)NULL;
    XmwTextOutSetStringWcs( TourokuText[0], ws, XmwHIGHLIGHT_NORMAL );
    XmwTextOutSetString( TourokuText[1], NULL, XmwHIGHLIGHT_NORMAL );
}


void	touroku_kakucyou( b )
Buffer	*b;
{
    wchar_t	ws[512];
    Position	position;

    if( b->henkan->bunsetsu[henkan_end_point+1] == BUNSETSU_END ) {
	XBell( XtDisplay(b->preedit_widget), 0 );
	return;
    }

    henkan_end_point++;
    if( b->henkan->bun[henkan_end_point].deststr == NULL )
	wscpy( ws, b->henkan->bun[henkan_end_point].srcstr );
    else
	wscpy( ws, b->henkan->bun[henkan_end_point].deststr );
    ws[b->henkan->bun[henkan_end_point].destlen] = (wchar_t)NULL;
    position = XmwTextOutGetLastPosition( TourokuText[0] );
    XmwTextOutReplaceWcs( TourokuText[0], position, position, ws );
}


void	touroku_syukusyou( b )
Buffer	*b;
{
    Position	position;

    if( b->henkan->point == henkan_end_point ) {
	XBell( XtDisplay(b->preedit_widget), 0 );
	return;
    }

    position = XmwTextOutGetLastPosition( TourokuText[0] );
    XmwTextOutReplace( TourokuText[0],
		       position -  b->henkan->bun[henkan_end_point].destlen,
		       position, NULL );
    henkan_end_point--;
}
