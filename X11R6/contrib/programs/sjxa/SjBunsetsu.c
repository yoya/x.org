/* $SonyId: SjBunsetsu.c,v 1.1 1994/05/30 13:03:58 makoto Exp $ */
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
#include	<Xmw/TextOut.h>
#include	<Xmw/Preedit.h>
#include	"sjxa.h"
#include	"resource.h"
#include	"SjBuffer.h"
#include	"SjString.h"

extern Sjxa	resources;


static void
#if NeedFunctionPrototypes
bunsetsu_saihenkan( Buffer *b )
#else /* NeedFunctionPrototypes */
bunsetsu_saihenkan( b )
    Buffer	*b;
#endif /* NeedFunctionPrototypes */
{
    int			ii, jj;
    SJ3_DOUON_WCS	*dou;
    wchar_t		ws[MAX_KANJI], *wp;
    unsigned int	hp = b->henkan->point;

    wsncpy( ws, b->henkan->bun[hp].srcstr, b->henkan->bun[hp].srclen );
    ws[b->henkan->bun[hp].srclen] = '\0';
    if( sj3_getdouon_wcs( ws, &b->henkan->dou ) == -1 ) {
	server_error( b, b->preedit_widget, resources.died_error );
	return;
    }
    dou = b->henkan->dou;
    b->henkan->bun[hp].destlen = dou[0].dlen;
    for( ii = jj = 0; ii < hp; ii++ )
	if( b->henkan->bun[ii].deststr != NULL )
	    jj += b->henkan->bun[ii].destlen;
    wscpy( &b->henkan->kanji[jj], dou[0].ddata );
    b->henkan->bun[hp].deststr = &b->henkan->kanji[jj];
    b->henkan->bun[hp].dcid = dou[0].dcid;

    if( b->henkan->bunsetsu_length[hp] == b->henkan->bun[hp].srclen )
	b->henkan->bunsetsu[hp] = BUNSETSU_KANJI;
    else
	b->henkan->bunsetsu[hp] = BUNSETSU_EDIT;

    b->henkan->bun[hp+1].srcstr = b->henkan->bun[hp].srcstr +
				  b->henkan->bun[hp].srclen;

    hp++;
    if( b->henkan->bun[hp].srcstr[0] == '\0' )
	b->henkan->bunsetsu[hp] = BUNSETSU_END;
    else
	getkan( b->preedit_widget, b, hp );

    for( ii = 0, wp = ws; b->henkan->bunsetsu[ii] != BUNSETSU_END; ii++ ) {
	if( b->henkan->bun[ii].deststr == NULL ) {
	    wsncpy( wp, b->henkan->bun[ii].srcstr, b->henkan->bun[ii].srclen );
	    b->henkan->bun[ii].destlen = b->henkan->bun[ii].srclen;
	}
	else
	    wsncpy( wp, b->henkan->bun[ii].deststr,
		    b->henkan->bun[ii].destlen );
	wp += b->henkan->bun[ii].destlen;
    }
    *wp = (wchar_t)NULL;
    if( b->conversion_style & XIMPreeditNothing ) {
	XmwTextOutSetStringWcs( b->preedit_widget, ws, XmwHIGHLIGHT_NORMAL );
	XmwTextOutInsert( b->preedit_widget, wslen(ws), " " );
    }
    else if( b->conversion_style & XIMPreeditPosition )
	XmwPreeditSetStringWcs( b->preedit_widget, ws,
				XmwHIGHLIGHT_SECONDARY_SELECTED );
    set_highlight( b );
}


void
#if NeedFunctionPrototypes
kakucyou( Widget w, XEvent *event, String *params, Cardinal *n )
#else /* NeedFunctionPrototypes */
kakucyou( w, event, params, n )
    Widget	w;
    XEvent	*event;
    String	*params;
    Cardinal	*n;
#endif /* NeedFunctionPrototypes */
{
    Buffer		*b;
    int			ii;
    unsigned int	hp, end;
    SJ3_BUNSETU_WCS	bun;

    b = set_buffer( w, event->type );
    if( b == NULL  ||  (b->state & ERROR)  ||
	!check_action_args( params, *n, "expand" ) )
	return;

    if( b->state & ADD )
	touroku_kakucyou( b );
    else if( b->state & (CONVERSION | WRAP) ) {
	b->state = CONVERSION;
	hp = b->henkan->point;

	if( b->henkan->bunsetsu[hp+1] == BUNSETSU_END ) {
	    XBell( XtDisplay(w), 0 );
	    return;
	}

	bun = b->henkan->bun[hp];
	ii = (bun.srcstr - b->henkan->bun[0].srcstr) + bun.srclen;
	for( end = 0; b->yomiC[end] != ii; end++ );
	ii = b->yomiC[end+1] - b->yomiC[end];
	b->henkan->bun[hp].srclen += ii;
	b->henkan->bun[hp+1].srcstr += ii;

	bunsetsu_saihenkan( b );
    }
}


void
#if NeedFunctionPrototypes
syukusyou( Widget w, XEvent *event, String *params, Cardinal *n )
#else /* NeedFunctionPrototypes */
syukusyou( w, event, params, n )
    Widget	w;
    XEvent	*event;
    String	*params;
    Cardinal	*n;
#endif /* NeedFunctionPrototypes */
{
    Buffer		*b;
    int			ii;
    unsigned int	hp, start, end;
    SJ3_BUNSETU_WCS	bun;

    b = set_buffer( w, event->type );
    if( b == NULL  ||  (b->state & ERROR)  ||
	!check_action_args( params, *n, "reduce" ) )
	return;

    if( b->state & ADD )
	touroku_syukusyou( b );
    else if( b->state & (CONVERSION | WRAP) ) {
	b->state = CONVERSION;
	hp = b->henkan->point;
	bun = b->henkan->bun[hp];

	ii = bun.srcstr - b->henkan->bun[0].srcstr;
	for( start = 0; b->yomiC[start] != ii; start++ );
	ii += bun.srclen;
	for( end = start; b->yomiC[end] != ii; end++ );
	if( start + 1 == end ) {
	    XBell( XtDisplay(w), 0 );
	    return;
	}
	ii = b->yomiC[end] - b->yomiC[end-1];

	b->henkan->bun[hp].srclen -= ii;
	b->henkan->bun[hp+1].srcstr = bun.srcstr + b->henkan->bun[hp].srclen;

	bunsetsu_saihenkan( b );
    }
}


void	bunsetsu_delete( b )
Buffer	*b;
{
    int			ii, jj;
    int			start, end;
    unsigned int	hp, fp;
    wchar_t		ws[1];

    ws[0] = (wchar_t)NULL;
    replace_bunsetsu( b, ws, 0, True );

    hp = b->henkan->point;
    ii = b->henkan->bun[hp].srcstr - b->henkan->bun[0].srcstr;
    for( start = 0; b->yomiC[start] != ii; start++ );
    ii += b->henkan->bun[hp].srclen;
    for( end = start; b->yomiC[end] != ii; end++ );

    ii = b->henkan->bun[hp].srclen;
    b->henkan->bun[hp].srclen = 0;
    while( b->henkan->bunsetsu[++hp] != BUNSETSU_END )
	b->henkan->bun[hp].srcstr -= ii;

    for( ii = b->keyC[start], jj = b->keyC[end]; jj < b->keyC[b->end_point];
	 ii++, jj++ ) {
	b->key[ii] = b->key[jj];
	b->str[ii] = b->str[jj];
    }
    b->key[ii] = b->str[ii] = '\0';

    for( ii = b->yomiC[start], jj = b->yomiC[end]; jj < b->yomiC[b->end_point];
	 ii++, jj++ )
	b->yomi[ii] = b->yomi[jj];
    b->yomi[ii] = '\0';

    for( ii = start, jj = end; jj <= b->end_point; ii++, jj++ ) {
	b->keyC[ii] -= b->keyC[end] - b->keyC[start];
	b->yomiC[ii] -= b->yomiC[end] - b->yomiC[start];
	b->mode[ii] = b->mode[jj];
	b->conversion_flag[ii] = b->conversion_flag[jj];
    }
    b->end_point -= end - start;

    for( ii = b->henkan->point; b->henkan->bunsetsu[ii] != BUNSETSU_END;
	 ii++ ) {
	b->henkan->bun[ii] = b->henkan->bun[ii+1];
	b->henkan->bunsetsu[ii] = b->henkan->bunsetsu[ii+1];
    }

    if( b->henkan->bunsetsu[b->henkan->point] == BUNSETSU_END ) {
	if( b->henkan->point == 0 )
	    henkan_cancel( b );
	else
	    b->henkan->point--;
    }

    if( b->state == CONVERSION ) {
	hp = b->henkan->point;
	for( ii = 0, fp = 0; ii < hp; ii++ )
	    fp += b->henkan->bun[ii].destlen;
	if( b->conversion_style & XIMPreeditNothing )
	    XmwTextOutSetHighlight( b->preedit_widget, fp,
				    fp+b->henkan->bun[hp].destlen,
				    XmwHIGHLIGHT_SELECTED );
	else if( b->conversion_style & XIMPreeditPosition )
	    XmwPreeditSetHighlight( b->preedit_widget,
				   fp, fp+b->henkan->bun[hp].destlen,
				   XmwHIGHLIGHT_SELECTED );
    }
}
