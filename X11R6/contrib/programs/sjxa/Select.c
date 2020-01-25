/* $SonyId: Select.c,v 1.1 1994/05/30 13:03:58 makoto Exp $ */
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
#include	<stdlib.h>
#include	<X11/Intrinsic.h>
#include	<X11/StringDefs.h>
#include	<X11/Xaw/Form.h>
#include	<X11/Shell.h>
#include	<Xmw/List.h>
#include	<Xmw/Preedit.h>
#include	<Xmw/Util.h>
#include	"sjxa.h"
#include	"resource.h"
#include	"SjString.h"

extern char		*program_name;
extern Sjxa		resources;
extern int		*depths;


void
#if NeedFunctionPrototypes
select_dialog_kettei( Widget w, caddr_t client_data, caddr_t call_data )
#else /* NeedFunctionPrototypes */
select_dialog_kettei( w, client_data, call_data )
    Widget	w;
    caddr_t	client_data, call_data;
#endif /* NeedFunctionPrototypes */
{
    Buffer		*b = (Buffer *)client_data;
    int			ii, result;
    unsigned int	hp;
    Position		fp, tp;
    wchar_t		ws[MAX_KANJI], *wp;
    int			selected_item;

    XtVaGetValues( b->list_widget, XmwNselectedItem, &selected_item, NULL );

    XtDestroyWidget( b->shell );
    b->shell = NULL;

    hp = b->henkan->point;

    for( ii = 0, fp = 0; ii < hp; ii++ )
	fp += b->henkan->bun[ii].destlen;
    tp = fp + b->henkan->wrap_length;
    XmwPreeditReplaceWcs( b->preedit_widget, fp, tp,
			  b->henkan->dou[selected_item].ddata );

    if( b->henkan->bun[hp].deststr != NULL  &&
       (wsncmp( b->henkan->bun[hp].deststr, b->henkan->dou[selected_item].ddata,
		Min( b->henkan->bun[hp].destlen,
		     b->henkan->dou[selected_item].dlen ) ) != 0  ||
	b->henkan->bun[hp].destlen != b->henkan->dou[selected_item].dlen) ) {
	if( b->henkan->bunsetsu[hp+1] == BUNSETSU_END ) {
	    wscpy( b->henkan->bun[hp].deststr,
		   b->henkan->dou[selected_item].ddata );
	    b->henkan->bun[hp].destlen = b->henkan->dou[selected_item].dlen;
	    b->henkan->bun[hp].dcid = b->henkan->dou[selected_item].dcid;
	}
	else {
	    for( ii = hp + 1, wp = ws;
		 b->henkan->bunsetsu[ii] != BUNSETSU_END; ii++ ) {
		if( b->henkan->bun[ii].deststr != NULL ) {
		    wsncpy( wp, b->henkan->bun[ii].deststr,
			    b->henkan->bun[ii].destlen );
		    wp += b->henkan->bun[ii].destlen;
		}
	    }
	    *wp = (wchar_t)NULL; 
	    wscpy( b->henkan->bun[hp].deststr,
		   b->henkan->dou[selected_item].ddata );
	    wscat( b->henkan->kanji, ws );

	    ii = b->henkan->dou[selected_item].dlen -
		 b->henkan->bun[hp].destlen;

	    b->henkan->bun[hp].destlen = b->henkan->dou[selected_item].dlen;
	    b->henkan->bun[hp].dcid = b->henkan->dou[selected_item].dcid;

	    while( b->henkan->bunsetsu[hp++] != BUNSETSU_END )
		b->henkan->bun[hp].deststr += ii;
	}

#ifdef USE_SJ3LIB
	result = sj3_gakusyuu( &b->henkan->bun[b->henkan->point].dcid );
	if( result == -1 )
	    server_error( b, b->preedit_widget, resources.died_error );
	else if( result != 0  &&  (resources.debug & 8) )
	    server_error( b, b->preedit_widget, resources.learning_error1 );
#endif
    }

    b->state &= ERROR;
    b->state |= CONVERSION;
}


void
#if NeedFunctionPrototypes
select_dialog_cancel( Widget w, caddr_t client_data, caddr_t call_data )
#else /* NeedFunctionPrototypes */
select_dialog_cancel( w, client_data, call_data )
    Widget	w;
    caddr_t	client_data, call_data;
#endif /* NeedFunctionPrototypes */
{
    Buffer		*b = (Buffer *)client_data;
    int			ii;
    wchar_t		ws[256];
    unsigned int	hp;
    Position		fp, tp;

    XtDestroyWidget( b->shell );
    b->shell = NULL;

    hp = b->henkan->point;
    if( b->henkan->bun[hp].deststr != NULL ) {
	for( ii = 0, fp = 0; ii < hp; ii++ )
	    fp += b->henkan->bun[ii].destlen;
	tp = fp + b->henkan->wrap_length;
	wscpy( ws, b->henkan->bun[hp].deststr );
	ws[b->henkan->bun[hp].destlen] = (wchar_t)NULL;
	XmwPreeditReplaceWcs( b->preedit_widget, fp, tp, ws );
	XmwPreeditSetHighlight( b->preedit_widget, fp, fp + wslen(ws),
			        XmwHIGHLIGHT_SELECTED );
    }
    b->state = CONVERSION;
}


void
#if NeedFunctionPrototypes
PopupSelectDialog( Buffer *b )
#else /* NeedFunctionPrototypes */
PopupSelectDialog( b )
    Buffer	*b;
#endif /* NeedFunctionPrototypes */
{
    Widget		widget = b->preedit_widget;
    Pixel		fg, bg, lbg;
    Colormap		colormap;
    int			depth;
    Screen		*screen;
    int			n, ii, max, len;
    Arg			args[10];
    SJ3_DOUON_WCS	*douon = b->henkan->dou;
    char		*item;
    Dimension		width, height, bar_width;
    Dimension		base_height, base_width;
    int			visible_vertical_item_count;
    short		num_rows;
    int			horiz_distance, vert_distance;

    XtVaGetValues( widget, XtNforeground, &fg, XtNbackground, &bg,
		   XtNcolormap, &colormap, XtNdepth, &depth,
		   XtNscreen, &screen, NULL );
    n = XScreenNumberOfScreen( screen );
    lbg = ( depths[n] == 1 ) ? bg
			     : light_color( XtDisplay(widget), colormap, bg );

    n = 0;
    XtSetArg( args[n], XtNtitle, "sjxa SelectDialog" );		n++;
    XtSetArg( args[n], XtNcolormap, colormap );			n++;
    XtSetArg( args[n], XtNdepth, depth );			n++;
    XtSetArg( args[n], XtNscreen, screen );			n++;
    XtSetArg( args[n], XtNiconic, False );			n++;
    XtSetArg( args[n], XtNmappedWhenManaged, False );		n++;
    b->shell = XtAppCreateShell( program_name, CLASS_NAME,
				 topLevelShellWidgetClass, XtDisplay(widget),
				 args, n );

    for( ii = max = 0; ii < b->henkan->douon_count; ii++ ) {
	len = wcs2mbs( NULL, douon[ii].ddata, douon[ii].dlen );
	if( max < len )
	    max = len;
    }
    item = calloc( b->henkan->douon_count + 1, max );
    for( ii = 0; ii < b->henkan->douon_count; ii++ )
	 wcs2mbs( &item[ii*max], douon[ii].ddata, douon[ii].dlen );

    n = 0;
    XtSetArg( args[n], XtNforeground, fg );				n++;
    XtSetArg( args[n], XtNbackground, bg );				n++;
    XtSetArg( args[n], XmwNitemBackground, lbg );			n++;
    XtSetArg( args[n], XmwNitemCount, b->henkan->douon_count );		n++;
    XtSetArg( args[n], XmwNitemLength, b->henkan->douon_count * max );	n++;
    XtSetArg( args[n], XmwNitems, item );				n++;
    XtSetArg( args[n], XmwNselectedItem, b->henkan->douon_point );	n++;
    XtSetArg( args[n], XmwNhighlightAlways, True );			n++;
    b->list_widget = XmwCreateScrolledList( b->shell, "Select", args, n );
    XtAddCallback( b->list_widget, XmwNsetCallback,
		   (XtCallbackProc)select_dialog_kettei, (caddr_t)b );
    XtManageChild( b->list_widget );


    XtVaSetValues( b->shell, XtNgeometry, "+0+0", NULL );

    XtRealizeWidget( b->shell );

    widget = XtNameToWidget( XtParent(b->list_widget), "vertical" );
    if( widget ) {
	XtVaGetValues( widget, XtNwidth, &bar_width, NULL );
	XtVaSetValues( widget, XtNforeground, fg, NULL );	/* Xaw BUG */
    }
    else
	bar_width = 0;
    XtVaGetValues( b->list_widget,
		   XtNheight, &height,
		   XmwNnumRows, &num_rows,
		   XmwNvisibleVerticalItemCount, &visible_vertical_item_count,
		   XmwNitemBaseHeight, &base_height,
		   XmwNitemBaseWidth, &base_width,
		   XtNhorizDistance, &horiz_distance,
		   XtNvertDistance, &vert_distance,
		   NULL );
    width = bar_width + base_width * num_rows + horiz_distance;
    XtVaSetValues( b->shell, XtNminWidth, width,
		   XtNminHeight, base_height * visible_vertical_item_count +
			         vert_distance,
		   XtNmaxWidth, width, XtNmaxHeight, height, NULL );

    for( ii = 0, n = 0; ii < b->henkan->point; ii++ )
	n += b->henkan->bun[ii].destlen;
    XmwPreeditMoveWidget( b->preedit_widget, n, b->shell );

    XmwInstallTranslations( b->list_widget, b->preedit_widget );
    XmwInstallTranslations( XtParent(b->list_widget), b->preedit_widget );
    XtAddGrab( b->shell, False, False );
}
