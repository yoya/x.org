/* $SonyId: SjKettei.c,v 1.1 1994/05/30 13:03:58 makoto Exp $ */
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
#include	<stdlib.h>
#include	<Xmw/Preedit.h>
#include	<Xmw/TextOut.h>
#include	"sjxa.h"
#include	"resource.h"
#include	"SjBuffer.h"
#include	"SjString.h"

extern Widget	TopLevel, RootText, RootLabel, RootTextField;
extern Widget	Box[2];
extern Sjxa	resources;
extern LcCtype	user_locale;


void
#if NeedFunctionPrototypes
kettei( Widget w, XEvent *event, String *params, Cardinal *n )
#else /* NeedFunctionPrototypes */
kettei( w, event, params, n )
    Widget	w;
    XEvent	*event;
    String	*params;
    Cardinal	*n;
#endif /* NeedFunctionPrototypes */
{
    Buffer		*b;
    int			ii, result;
    int			len1, len2;
    wchar_t		yomi1[MAX_KANA], yomi2[MAX_KANA];
    unsigned int	hp, count;
    Position		fp, tp;
    wchar_t		wc, ws[MAX_KANJI], *wp;
    Window		win;
    unsigned short	kanji;
    char		mb[10];

    b = set_buffer( w, event->type );
    if( b == NULL  ||  !check_action_args( params, *n, "decide" ) )
	return;

    if( b->state & ERROR ) {
	error_cancel( (Widget)NULL, (caddr_t)b, (caddr_t)NULL );
	return;
    }
    else if( b->state & DELETE )
	sakujo_kettei( w, (caddr_t)b, (caddr_t)NULL );
    else if( b->state & SERVER ) {
	reconnect_kettei( (Widget)NULL, (caddr_t)b, (caddr_t)NULL );
    }
    else if( b->state & JISX0208 ) {
	code_kettei( b, kuten2wc(b->jisx0208) );
    }
    else if( b->state & JISX0212 ) {
	kanji = kuten2euc( b->jisx0212 );
	mb[0] = SS3;
	mb[1] = kanji >> 8;
	mb[2] = kanji & 0xff;
	mbtowc( &wc, mb, MB_CUR_MAX );
	code_kettei( b, wc );
    }
    else if( b->state & ADD ) {
	touroku_kettei( w, (caddr_t)b, (caddr_t)b );
    }
    else if( b->state & EDIT ) {
	edit_kettei( b );
    }
    else if( b->state & SELECT ) {
	if( b->conversion_style & (XIMPreeditPosition | XIMPreeditArea) ) {
	    select_dialog_kettei( (Widget)NULL, (caddr_t)b, (caddr_t)NULL );
	}
	else {
	    hp = b->henkan->point;
	    count = b->henkan->douon_point;

	    if( (wslen(b->henkan->kanji) - b->henkan->bun[hp].destlen +
		 b->henkan->dou[count].dlen) >= SJ3_BUNSETU_KANJI ) {
		XBell( event->xany.display, 0 );
		return;
	    }

	    for( ii = 0, fp = 0; ii < hp; ii++ )
		fp += b->henkan->bun[ii].destlen;
	    tp = fp + b->henkan->wrap_length;
	    if( b->conversion_style & XIMPreeditNothing )
		XmwTextOutReplaceWcs( b->preedit_widget, fp, tp,
				      b->henkan->dou[count].ddata );
	    else if( b->conversion_style &
		     (XIMPreeditPosition | XIMPreeditArea) )
		XmwPreeditReplaceWcs( b->preedit_widget, fp, tp,
				      b->henkan->dou[count].ddata );

	    if( b->henkan->bun[hp].deststr != NULL  &&
		(wsncmp( b->henkan->bun[hp].deststr, b->henkan->dou[count].ddata,
			 Min( b->henkan->bun[hp].destlen,
			      b->henkan->dou[count].dlen ) ) != 0  ||
		b->henkan->bun[hp].destlen != b->henkan->dou[count].dlen) ) {
		if( b->henkan->bunsetsu[hp+1] == BUNSETSU_END ) {
		    wscpy( b->henkan->bun[hp].deststr,
			   b->henkan->dou[count].ddata );
		    b->henkan->bun[hp].destlen = b->henkan->dou[count].dlen;
		    b->henkan->bun[hp].dcid = b->henkan->dou[count].dcid;
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
		    *wp = '\0'; 
		    wscpy( b->henkan->bun[hp].deststr,
			   b->henkan->dou[count].ddata );
		    wscat( b->henkan->kanji, ws );

		    ii = b->henkan->dou[count].dlen -
			 b->henkan->bun[hp].destlen;

		    b->henkan->bun[hp].destlen = b->henkan->dou[count].dlen;
		    b->henkan->bun[hp].dcid = b->henkan->dou[count].dcid;

		    while( b->henkan->bunsetsu[hp++] != BUNSETSU_END )
			b->henkan->bun[hp].deststr += ii;
		}

#ifdef USE_SJ3LIB
		result = sj3_gakusyuu( &b->henkan->bun[b->henkan->point].dcid );
		if( result == -1 )
		    server_error( b, w, resources.died_error );
		else if( result != 0  &&  (resources.debug & 0x8000) )
		    server_error( b, w, resources.learning_error1 );
#endif
	    }

	    XtUnmapWidget( RootTextField );
	    XtMapWidget( Box[0] );
	    XtMapWidget( Box[1] );

	    b->state &= ~SELECT;
	    b->state |= CONVERSION;
	}
    }
    else if( b->state & (WRAP | CONVERSION) ) {
	if( b->state & WRAP )
	    wrap_end( b, w, True );
	win = event->xkey.subwindow;

#ifdef USE_SJ3LIB
	for( ii = 0; b->henkan->bunsetsu[ii+1] != BUNSETSU_END; ii++ ) {
	    if( b->henkan->bunsetsu[ii] & BUNSETSU_EDIT ) {
		len1 = b->henkan->bun[ii].srclen;
		wsncpy( yomi1, b->henkan->bun[ii].srcstr, len1 );
		yomi1[len1] = '\0';
		len2 = b->henkan->bun[ii+1].srclen;
		wsncpy( yomi2, b->henkan->bun[ii+1].srcstr, len2 );
		yomi2[len2] = '\0';
		result = sj3_gakusyuu2_wcs( yomi1, yomi2,
					    &(b->henkan->bun[ii+1].dcid) );
		if( result == -1 ) {
		    server_error( b, w, resources.died_error );
		    break;
		}
		else if( result != 0  &&  (resources.debug & 0x8000) ) {
		    server_error( b, w, resources.learning_error2 );
		    break;
		}
	    }
	}
#endif
	for( ii = 0, wp = ws; b->henkan->bunsetsu[ii] != BUNSETSU_END;
	     ii++ ) {
	    if( b->henkan->bun[ii].deststr == NULL ) {
		wsncpy( wp, b->henkan->bun[ii].srcstr,
			b->henkan->bun[ii].srclen );
		wp += b->henkan->bun[ii].srclen;
	    }
	    else {
		wsncpy( wp, b->henkan->bun[ii].deststr,
			b->henkan->bun[ii].destlen );
		wp += b->henkan->bun[ii].destlen;
	    }
	}
	*wp = '\0';

	if( !SendCompoundText( event->xany.display, win, ws ) )
	    return;
	register_buffer( b );
	buffer_initialize( b );
	XtVaSetValues( b->preedit_widget, XmwNdisplayCaret, True, NULL );
#ifdef USE_SJ3LIB
	if( !CheckState( CONVERSION|EDIT|ADD|DELETE|SELECT|WRAP ) ) {
	    sj3_unlockserv();
	    sj3_close();
	}
#endif
    }
    else if( b->state & INTERMEDIATE ) {
	if( b->yomi[0] != '\0' ) {
	    win = event->xkey.subwindow;
	    if( SendCompoundText( event->xany.display, win, b->yomi ) ) {
		register_buffer( b );
		buffer_initialize( b );
	    }
	}
    }
}
