/* $SonyId: Sakujo.c,v 1.2 1994/06/03 06:33:04 makoto Exp $ */
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
#include	<malloc.h>
#include	<stdio.h>
#include	<X11/Xlib.h>
#include	<X11/Xlibint.h>
#include	<X11/Intrinsic.h>
#include	<X11/StringDefs.h>
#include	<X11/Shell.h>
#include	<X11/Xaw/Dialog.h>
#include	<Xmw/Command.h>
#include	<Xmw/List.h>
#include	<Xmw/Util.h>
#include	"sjxa.h"
#include	"resource.h"
#include	"hinshi.h"
#include	"SjBuffer.h"
#include	"SjString.h"

#define	ITEM_LENGTH	(24+24+12)


static Widget	SakujoDialog, QuestionDialog;

extern Widget	delete_shell, question_shell;
extern char	*program_name, user_name[];
extern Sjxa	resources;
extern int	*depths;

static struct {
    int		entity;
    int		alias;
} hinshi[] = {
    { MEISI_2, MEISI_1 }, { MEISI_3, MEISI_1 }, { MEISI_4, MEISI_1 },
    { MEISI_5, MEISI_1 }, { MEISI_6, MEISI_1 }, { MEISI_7, MEISI_1 },
    { MEISI_9, MEISI_1 },
    { MEISI_10, MEISI_1 }, { MEISI_11, MEISI_1 }, { MEISI_20, MEISI_1 },
    { D_MEISI_2, D_MEISI_1 }, { D_MEISI_3, D_MEISI_1 },
    { D_MEISI_4, D_MEISI_1 }, { D_MEISI_5, D_MEISI_1 },
    { D_MEISI_6, D_MEISI_1 },
    { SETTOU_2, SETTOU_1 }, { SETTOU_3, SETTOU_1 }, { SETTOU_4, SETTOU_1 },
    { SETTOU_5, SETTOU_1 },
    { SETUBI_2, SETUBI_1 }, { SETUBI_3, SETUBI_1 }, { SETUBI_4, SETUBI_1 },
    { SETUBI_5, SETUBI_1 }, { SETUBI_6, SETUBI_1 }, { SETUBI_7, SETUBI_1 },
    { SETUBI_8, SETUBI_1 }, { SETUBI_9, SETUBI_1 },
    { FUKUSI_2, FUKUSI_1 }, { FUKUSI_3, FUKUSI_1 }, { FUKUSI_4, FUKUSI_1 },
    { FUKUSI_5, FUKUSI_1 }, { FUKUSI_6, FUKUSI_1 }, { FUKUSI_7, FUKUSI_1 },
    { FUKUSI_8, FUKUSI_1 }, { FUKUSI_9, FUKUSI_1 },
    { JOSUUSI2, JOSUUSI1 },
    { KEIYOUSI_2, KEIYOUSI_1 }, { KEIYOUSI_3, KEIYOUSI_1 },
    { KEIYOUSI_4, KEIYOUSI_1 }, { KEIYOUSI_5, KEIYOUSI_1 },
    { KEIYOUSI_6, KEIYOUSI_1 }, { KEIYOUSI_7, KEIYOUSI_1 },
    { KEIYOUSI_8, KEIYOUSI_1 }, { KEIYOUSI_9, KEIYOUSI_1 },
    { KEIYOUSI_10, KEIYOUSI_1 }, { KEIYOUSI_11, KEIYOUSI_1 },
    { KE_DOUSI_2, KE_DOUSI_1 }, { KE_DOUSI_3, KE_DOUSI_1 },
    { KE_DOUSI_4, KE_DOUSI_1 }, { KE_DOUSI_5, KE_DOUSI_1 },
    { KE_DOUSI_6, KE_DOUSI_1 }, { KE_DOUSI_7, KE_DOUSI_1 },
    { KE_DOUSI_8, KE_DOUSI_1 }, { KE_DOUSI_9, KE_DOUSI_1 },
    { DO_1DAN_2, DO_1DAN_1 }, { DO_1DAN_3, DO_1DAN_1 },
    { DO_1DAN_4, DO_1DAN_1 },
    { DO_KAGO_2, DO_KAGO_1 }, { DO_KAGO_3, DO_KAGO_1 },
    { DO_KAGO_4, DO_KAGO_1 }, { DO_KAGO_5, DO_KAGO_1 },
    { DO_KAGO_6, DO_KAGO_1 }, { DO_KAGO_7, DO_KAGO_1 },
    { DO_KAGO_8, DO_KAGO_1 },
    { DO_GAGO_2, DO_GAGO_1 }, { DO_GAGO_3, DO_GAGO_1 },
    { DO_GAGO_4, DO_GAGO_1 }, { DO_GAGO_5, DO_GAGO_1 },
    { DO_GAGO_6, DO_GAGO_1 }, { DO_GAGO_7, DO_GAGO_1 },
    { DO_GAGO_8, DO_GAGO_1 },
    { DO_SAGO_2, DO_SAGO_1 }, { DO_SAGO_3, DO_SAGO_1 },
    { DO_SAGO_4, DO_SAGO_1 }, { DO_SAGO_5, DO_SAGO_1 },
    { DO_SAGO_6, DO_SAGO_1 }, { DO_SAGO_7, DO_SAGO_1 },
    { DO_SAGO_8, DO_SAGO_1 },
    { DO_TAGO_2, DO_TAGO_1 }, { DO_TAGO_3, DO_TAGO_1 },
    { DO_TAGO_4, DO_TAGO_1 }, { DO_TAGO_5, DO_TAGO_1 },
    { DO_TAGO_6, DO_TAGO_1 }, { DO_TAGO_7, DO_TAGO_1 },
    { DO_TAGO_8, DO_TAGO_1 },
    { DO_BAGO_2, DO_BAGO_1 }, { DO_BAGO_3, DO_BAGO_1 },
    { DO_BAGO_4, DO_BAGO_1 }, { DO_BAGO_5, DO_BAGO_1 },
    { DO_BAGO_6, DO_BAGO_1 }, { DO_BAGO_7, DO_BAGO_1 },
    { DO_BAGO_8, DO_BAGO_1 },
    { DO_MAGO_2, DO_MAGO_1 }, { DO_MAGO_3, DO_MAGO_1 },
    { DO_MAGO_4, DO_MAGO_1 }, { DO_MAGO_5, DO_MAGO_1 },
    { DO_MAGO_6, DO_MAGO_1 }, { DO_MAGO_7, DO_MAGO_1 },
    { DO_MAGO_8, DO_MAGO_1 },
    { DO_RAGO_2, DO_RAGO_1 }, { DO_RAGO_3, DO_RAGO_1 },
    { DO_RAGO_4, DO_RAGO_1 }, { DO_RAGO_5, DO_RAGO_1 },
    { DO_RAGO_6, DO_RAGO_1 }, { DO_RAGO_7, DO_RAGO_1 },
    { DO_RAGO_8, DO_RAGO_1 },
    { DO_WAGO_2, DO_WAGO_1 }, { DO_WAGO_3, DO_WAGO_1 },
    { DO_WAGO_4, DO_WAGO_1 }, { DO_WAGO_5, DO_WAGO_1 },
    { DO_WAGO_6, DO_WAGO_1 }, { DO_WAGO_7, DO_WAGO_1 },
    { DO_WAGO_8, DO_WAGO_1 },
    { SP_SA_MI1, DO_SAHEN }, { SP_SA_MI2, DO_SAHEN }, { SP_SA_YOU, DO_SAHEN },
    { SP_SA_SYU, DO_SAHEN }, { SP_SA_KAT, DO_SAHEN }, { SP_SA_ME1, DO_SAHEN },
    { SP_SA_ME2, DO_SAHEN },
    { SP_KA_MIZ, DO_KAHEN }, { SP_KA_YOU, DO_KAHEN }, { SP_KA_SYU, DO_KAHEN },
    { SP_KA_KAT, DO_KAHEN }, { SP_KA_MEI, DO_KAHEN },
    { TEINEI2, TEINEI1 },
    { SP_FUKUSI, FUKUSI_1 },
    { SP_KEIYOUSI, KEIYOUSI_1 },
    { 0, -1 }};


int	hinshi_code[] = {
    MEISI_1,		D_MEISI_1,	MYOUJI,		NAMAE,
    KIGYOU,		TIMEI,		KEN_KU,		TANKANJI,
    RENTAISI,		SETUZOKU,	KANDOUSI,	JOSUUSI1,
    SUUSI,		SETTOU_1,	SETUBI_1,	FUKUSI_1,
    KEIYOUSI_1,		KE_DOUSI_1,	DO_SAHEN,	DO_ZAHEN,
    DO_1DAN_1,		DO_KAGO_1,	DO_GAGO_1,	DO_SAGO_1,
    DO_TAGO_1,		DO_NAGO_1,	DO_BAGO_1,	DO_MAGO_1,
    DO_RAGO_1,		DO_WAGO_1,
    DO_KAHEN,		TEINEI1,	SP_KA_ONB,	AISATU,
    0 };

static struct syoukyo_wcs	*items;
static int			 item_count;


static String
#if NeedFunctionPrototypes
hinshi2string( int code )
#else /* NeedFunctionPrototypes */
hinshi2string( code )
    int	code;
#endif /* NeedFunctionPrototypes */
{
    int		n;

    for( n = 0; hinshi[n].entity != 0; n++ ) {
	if( hinshi[n].entity == code ) {
	    code = hinshi[n].alias;
	    break;
	}
    }

    for( n = 0; hinshi_code[n] != 0; n++ )
	if( hinshi_code[n] == code )
	    return( resources.hinshi[n] );
    return( resources.hinshi[n] );
}


void
#if NeedFunctionPrototypes
sakujo_cancel( Widget w, caddr_t client_data, caddr_t call_data )
#else /* NeedFunctionPrototypes */
sakujo_cancel( w, client_data, call_data )
    Widget	w;
    caddr_t	client_data, call_data;
#endif /* NeedFunctionPrototypes */
{
#ifdef USE_SJ3LIB
    Buffer	*b = (Buffer *)client_data;
    int		n;

    if( XtIsManaged(QuestionDialog) ) {
	XtUnmapWidget( question_shell );
	XtUnmanageChild( QuestionDialog );
    }
    else {
	XtUnmapWidget( delete_shell );
	XtDestroyWidget( delete_shell );
	XtDestroyWidget( question_shell );
	delete_shell = NULL;
	b->state &= ~DELETE;

	for( n = 0; n < item_count; n++ ) {
	    free( (char *)items[n].yomi );
	    free( (char *)items[n].kanji );
	}
	free( items );
	items = NULL;
	if( b->state & INTERMEDIATE )
	    sj3_close();
    }
#endif
}


void
#if NeedFunctionPrototypes
sakujo_kettei( Widget w, caddr_t client_data, caddr_t call_data )
#else /* NeedFunctionPrototypes */
sakujo_kettei( w, client_data, call_data )
    Widget	w;
    caddr_t	client_data, call_data;
#endif /* NeedFunctionPrototypes */
{
#ifdef USE_SJ3LIB
    Buffer	*b = (Buffer *)client_data;
    int		 result;
    int		 item_position;

    if( XtIsManaged(QuestionDialog) ) {
	XtVaGetValues( b->list_widget, XmwNselectedItem, &item_position, NULL );
	if( item_position == -1 )
	    return;
	XtUnmapWidget( question_shell );
	XtUnmanageChild( QuestionDialog );

	result = sj3_syoukyo_wcs( items[item_position].yomi,
				  items[item_position].kanji,
				  items[item_position].code );

	switch( result ) {
	  case 0:
	    sakujo_cancel( w, client_data, call_data );
	    break;

	  case -1:
	    server_error( b, SakujoDialog, resources.died_error );
	    break;

	  case SJ3_WORD_NOT_EXIST:
	    server_error( b, SakujoDialog, resources.delete_error1 );
	    break;

	  case SJ3_DICT_LOCKED:
	    server_error( b, SakujoDialog, resources.delete_error2 );
	    break;

	  case SJ3_BAD_YOMI_STR:
	    server_error( b, SakujoDialog, resources.delete_error3 );
	    break;

	  case SJ3_BAD_KANJI_STR:
	    server_error( b, SakujoDialog, resources.delete_error4 );
	    break;

	  default:
	    server_error( b, SakujoDialog, resources.delete_error5 );
	    break;
	}
    }
    else {
	XtVaGetValues( b->list_widget, XmwNselectedItem, &item_position, NULL );
	if( item_position != -1 ) {
	    XtManageChild( QuestionDialog );
	    XtMapWidget( question_shell );
	}
    }
#endif
}


void
#if NeedFunctionPrototypes
sakujo( Widget w, XEvent *event, String *params, Cardinal *nn )
#else /* NeedFunctionPrototypes */
sakujo( w, event, params, nn )
    Widget	 w;
    XEvent	*event;
    String	*params;
    Cardinal	*nn;
#endif /* NeedFunctionPrototypes */
{
#ifdef USE_SJ3LIB
    Buffer		*b;
    int			 flag;
    struct syoukyo_wcs	 item;
    char		 buf[100], s1[256], s2[256];
    String		 string = NULL;
    Arg			 args[20];
    int			 n;
    Widget		 form, icon, label, ok, cancel;
    Position		 x, y;
    Dimension		 width, height, hl, base_height;
    int			 item_count;
    Pixel		 fg, bg, lbg;
    Colormap		 colormap;
    int			 depth;
    Screen		*screen;

    if( (b = set_buffer( w, event->type )) == NULL )
	return;
    if( (b->state & ~INTERMEDIATE)  ||  delete_shell  ||
	!check_action_args( params, *nn, "delete-term" ) )
	return;

    if( b->state & INTERMEDIATE ) {
	switch( sj3_open( resources.server, user_name ) ) {
	  case SJ3_SERVER_DEAD:
	  case SJ3_CONNECT_ERROR:
	    server_error( b, w, resources.died_error );
	    return;

	  case SJ3_ALREADY_CONNECTED:
	    server_error( b, w, resources.open_error1 );
	    return;

	  case SJ3_CANNOT_OPEN_MDICT:
	    server_error( b, w, resources.open_error2 );
	    return;

	  case SJ3_CANNOT_OPEN_UDICT:
	    server_error( b, w, resources.open_error3 );
	    return;

	  case SJ3_CANNOT_OPEN_STUDY:
	    server_error( b, w, resources.open_error4 );
	    return;

	  case SJ3_CANNOT_MAKE_UDIR:
	    server_error( b, w, resources.open_error5 );
	    return;

	  case SJ3_CANNOT_MAKE_UDICT:
	    server_error( b, w, resources.open_error6 );
	    return;

	  case SJ3_CANNOT_MAKE_STUDY:
	    server_error( b, w, resources.open_error7 );
	    return;
	}
    }

    item_count = 0;
    items = NULL;
    flag = sj3_getdict_wcs( &item );
    while( flag == 0 ) {
	wcs2mbs( s1, item.yomi, wslen(item.yomi) );
	wcs2mbs( s2, item.kanji, wslen(item.kanji) );
	sprintf( buf, "%-24s%-24s", s1, s2 );

	if( items == NULL )
	    items = (struct syoukyo_wcs *)malloc( sizeof(struct syoukyo_wcs) );
	else
	    items = (struct syoukyo_wcs *)realloc( items,
				sizeof(struct syoukyo_wcs) * (item_count+1) );
	items[item_count].yomi = item.yomi;
	items[item_count].kanji = item.kanji;
	items[item_count].code = item.code;

	if( string == NULL )
	    string = malloc( ITEM_LENGTH + 1 );
	else
	    string = realloc( string, ITEM_LENGTH * (item_count+1) + 1 );

	bzero( &string[ITEM_LENGTH*item_count], ITEM_LENGTH );
	strcpy( &string[ITEM_LENGTH*item_count], buf );
	strcat( &string[ITEM_LENGTH*item_count], hinshi2string( item.code ) );
	item_count++;

	flag = sj3_nextdict_wcs( &item );
    }
    if( item_count == 0 ) {
	if( b->state & INTERMEDIATE )
	    sj3_close();
	server_error( b, w, resources.delete_error6 );
	return;
    }

    XtVaGetValues( w, XtNforeground, &fg, XtNbackground, &bg,
		   XtNcolormap, &colormap, XtNdepth, &depth,
		   XtNscreen, &screen, NULL );
    n = XScreenNumberOfScreen( screen );
    lbg = (depths[n] == 1) ? bg : light_color( XtDisplay(w), colormap, bg );

    n = 0;
    XtSetArg( args[n], XtNtitle, "sjxa DeleteTermDialog" );	n++;
    XtSetArg( args[n], XtNcolormap, colormap );			n++;
    XtSetArg( args[n], XtNbackground, bg );			n++;
    XtSetArg( args[n], XtNdepth, depth );			n++;
    XtSetArg( args[n], XtNscreen, screen );			n++;
    XtSetArg( args[n], XtNiconic, False );			n++;
    XtSetArg( args[n], XtNgeometry, "+0+0" );			n++;
    XtSetArg( args[n], XtNmappedWhenManaged, False );		n++;
    XtSetArg( args[n], XtNallowShellResize, True );		n++;
    delete_shell = XtAppCreateShell( program_name, CLASS_NAME,
				     topLevelShellWidgetClass, XtDisplay(w),
				     args, n );

    n = 0;
    XtSetArg( args[n], XtNbackground, bg );			n++;
    form = XtCreateManagedWidget( "DeleteTerm", formWidgetClass, delete_shell,
				  args, n );

    n = 0;
    XtSetArg( args[n], XtNforeground, fg );			n++;
    XtSetArg( args[n], XtNbackground, bg );			n++;
    XtSetArg( args[n], XtNtop, XawChainTop );			n++;
    XtSetArg( args[n], XtNleft, XawChainRight );		n++;
    XtSetArg( args[n], XtNright, XawChainRight );		n++;
    XtSetArg( args[n], XtNbottom, XawChainTop );		n++;
    XtSetArg( args[n], XtNborderWidth, 0 );			n++;
    label = XtCreateManagedWidget( "Label", labelWidgetClass, form, args, n );

    n = 0;
    XtSetArg( args[n], XtNborderColor, fg );			n++;
    XtSetArg( args[n], XtNforeground, fg );			n++;
    XtSetArg( args[n], XtNbackground, bg );			n++;
    XtSetArg( args[n], XtNfromVert, label );			n++;
    XtSetArg( args[n], XtNtop, XawChainTop );			n++;
    XtSetArg( args[n], XtNleft, XawChainLeft );			n++;
    XtSetArg( args[n], XtNright, XawChainRight );		n++;
    XtSetArg( args[n], XtNbottom, XawChainBottom );		n++;
    XtSetArg( args[n], XtNjustify, XtJustifyLeft );		n++;
    XtSetArg( args[n], XmwNitemBackground, lbg );		n++;
    XtSetArg( args[n], XmwNitems, string );			n++;
    XtSetArg( args[n], XmwNhighlightAlways, True );		n++;
    XtSetArg( args[n], XmwNitemLength, ITEM_LENGTH*item_count );n++;
    XtSetArg( args[n], XmwNitemCount, item_count );		n++;
    XtSetArg( args[n], XmwNselectedItem, 0 );			n++;
    XtSetArg( args[n], XmwNnumRows, 1 );			n++;
    XtSetArg( args[n], XmwNvisibleHorizontalItemCount, 1 );	n++;
    XtSetArg( args[n], XtNresizable, True );			n++;
    b->list_widget = XmwCreateScrolledList( form, "List", args, n );
    XtAddCallback( b->list_widget, XmwNsetCallback,
		  (XtCallbackProc)sakujo_kettei, (caddr_t)b );
    XtManageChild( b->list_widget );

    XtRealizeWidget( delete_shell );
    XmwInstallTranslations( b->list_widget, b->preedit_widget );
    XmwInstallTranslations( form, b->preedit_widget );
    XtAddGrab( delete_shell, False, False );

    /* Xaw BUG */
    {
	Widget	widget;

	widget = XtNameToWidget( XtParent(b->list_widget), "vertical" );
	if( widget )
	    XtVaSetValues( widget, XtNforeground, fg, NULL );
    }

    XtVaGetValues( delete_shell, XtNwidth, &width, XtNheight, &height, NULL );
    XtVaGetValues( b->list_widget, XmwNitemBaseHeight, &base_height,
		   XmwNvisibleVerticalItemCount, &item_count,
		   XtNheight, &hl, NULL );
    XtVaSetValues( delete_shell, XtNminWidth, width, XtNminHeight, height,
		   XtNmaxWidth, width,
		   XtNmaxHeight, height + hl - base_height * item_count, NULL );
    x = ( WidthOfScreen(screen) - width ) / 2;
    y = ( HeightOfScreen(screen) - height ) / 2;
    XtMoveWidget( delete_shell, x, y );
    XtMapWidget( delete_shell );

    b->state |= DELETE;

    n = 0;
    XtSetArg( args[n], XtNtitle, "sjxa Question" );		n++;
    XtSetArg( args[n], XtNcolormap, colormap );			n++;
    XtSetArg( args[n], XtNbackground, bg );			n++;
    XtSetArg( args[n], XtNdepth, depth );			n++;
    XtSetArg( args[n], XtNscreen, screen );			n++;
    XtSetArg( args[n], XtNiconic, False );			n++;
    XtSetArg( args[n], XtNgeometry, "+0+0" );			n++;
    XtSetArg( args[n], XtNmappedWhenManaged, False );		n++;
    question_shell = XtAppCreateShell( program_name, CLASS_NAME,
				       topLevelShellWidgetClass,
				       XtDisplay(delete_shell), args, n );

    n = 0;
    XtSetArg( args[n], XtNbackground, bg );			n++;
    QuestionDialog = XtCreateManagedWidget( "QuestionDialog", dialogWidgetClass,
					    question_shell, args, n );

    label = XtNameToWidget( QuestionDialog, "label" );
    XtVaSetValues( label, XtNforeground, fg, XtNbackground, bg, NULL );
    icon = XtNameToWidget( QuestionDialog, "icon" );
    if( icon )
	XtVaSetValues( icon, XtNforeground, fg, XtNbackground, bg, NULL );

    n = 0;
    XtSetArg( args[n], XtNforeground, fg );			n++;
    XtSetArg( args[n], XtNbackground, lbg );			n++;
    XtSetArg( args[n], XtNborderColor, fg );			n++;
    ok = XtCreateManagedWidget( "Ok", commandWidgetClass, QuestionDialog,
				args, n );
    XtAddCallback( ok, XtNcallback, (XtCallbackProc)sakujo_kettei, (caddr_t)b );

    cancel = XtCreateManagedWidget( "Cancel", commandWidgetClass,
				    QuestionDialog, args, n );
    XtAddCallback( cancel, XtNcallback, (XtCallbackProc)sakujo_cancel,
		   (caddr_t)b );

    XtRealizeWidget( question_shell );
    XmwInstallTranslations( QuestionDialog, b->preedit_widget );
    XtAddGrab( QuestionDialog, False, False );

    XtVaGetValues( question_shell, XtNwidth, &width, XtNheight, &height, NULL );
    XtVaSetValues( question_shell, XtNminWidth, width, XtNminHeight, height,
		   XtNmaxWidth, width, XtNmaxHeight, height, NULL );
    x = ( WidthOfScreen(screen) - width ) / 2;
    y = ( HeightOfScreen(screen) - height ) / 2;
    XtMoveWidget( question_shell, x, y );

    XtUnmanageChild( QuestionDialog );
#endif
}
