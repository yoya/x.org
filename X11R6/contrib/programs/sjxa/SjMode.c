/* $SonyId: SjMode.c,v 1.1 1994/05/30 13:03:58 makoto Exp $ */
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
#include	<X11/Intrinsic.h>
#include	<X11/StringDefs.h>
#include	<Xmw/Preedit.h>
#include	<Xmw/TextOut.h>
#include	"resource.h"
#include	"SjBuffer.h"
#include	"SjString.h"

#if	NeedFunctionPrototypes
extern RkTable	*alpha2hiraganaUpperS( unsigned char *key );
#else	/* NeedFunctionPrototypes */
extern RkTable	*alpha2hiraganaUpperS();
#endif	/* NeedFunctionPrototypes */

extern Sjxa	resources;
extern char	user_name[];


void
#if NeedFunctionPrototypes
replace_bunsetsu( Buffer *b, wchar_t *string, Bunsetsu mode, Boolean redisplay )
#else /* NeedFunctionPrototypes */
replace_bunsetsu( b, string, mode, redisplay )
    Buffer	*b;
    wchar_t	*string;
    Bunsetsu	 mode;
    Boolean	 redisplay;
#endif /* NeedFunctionPrototypes */
{
    int			ii, len, offset;
    unsigned int	hp;
    Position		fp, tp, htp;
    wchar_t		ss[512], *tt;

    hp = b->henkan->point;

    for( ii = 0, fp = 0, len = 0; ii < hp; ii++ ) {
	fp += b->henkan->bun[ii].destlen;
	if( b->henkan->bun[ii].deststr != NULL )
	    len += b->henkan->bun[ii].destlen;
    }
    if( b->state != WRAP )
	tp = fp + b->henkan->bun[hp].destlen;
    else
	tp = fp + b->henkan->dou[b->henkan->douon_point].dlen;
    if( redisplay ) {
	if( b->conversion_style & XIMPreeditNothing ) {
	    htp = fp + wslen(string);
	    XmwTextOutReplaceWcs( b->preedit_widget, fp, tp, string );
	    XmwTextOutSetHighlight( b->preedit_widget, fp, htp,
				    XmwHIGHLIGHT_SELECTED );
	}
	else if( b->conversion_style & XIMPreeditPosition ) {
	    offset = wslen(string);
	    XmwPreeditReplaceWcs( b->preedit_widget, fp, tp, string );
	    XmwPreeditSetHighlight( b->preedit_widget, fp, fp+offset,
				    XmwHIGHLIGHT_SELECTED );
	    XmwPreeditSetHighlight( b->preedit_widget, fp+offset, 10000,
				    XmwHIGHLIGHT_SECONDARY_SELECTED );
	}
    }

    b->henkan->bunsetsu[hp] &= BUNSETSU_EDIT;
    b->henkan->bunsetsu[hp] |= mode;

    offset = wslen( string );
    if( b->henkan->bun[hp].deststr != NULL )
	offset -= b->henkan->bun[hp].destlen;
    b->henkan->bun[hp].destlen = wslen( string );
    b->henkan->bun[hp].deststr = b->henkan->kanji + len;
    if( b->henkan->bunsetsu[hp+1] == BUNSETSU_END )
	wscpy( b->henkan->bun[hp].deststr, string );
    else {
	for( ii = hp + 1, tt = ss; b->henkan->bunsetsu[ii] != BUNSETSU_END;
	     ii++ ) {
	    if( b->henkan->bun[ii].deststr != NULL ) {
		wsncpy( tt, b->henkan->bun[ii].deststr,
		b->henkan->bun[ii].destlen );
		tt += b->henkan->bun[ii].destlen;
	    }
	}
	*tt = '\0';
	wscpy( b->henkan->bun[hp].deststr, string );
	wscat( b->henkan->kanji, ss );

	while( b->henkan->bunsetsu[hp++] != BUNSETSU_END )
	    b->henkan->bun[hp].deststr += offset;
    }
}


static int
#if NeedFunctionPrototypes
key2kana( Buffer *b, unsigned int start, unsigned int end, wchar_t w[] )
#else
key2kana( b, start, end, w )
    Buffer		*b;
    unsigned int	 start, end;
    wchar_t		 w[];
#endif
{
    int		 ii, jj;
    char	 key[256], ss[256];
    RkTable	*rt;

    for( ii = 0; (b->mode[start] & (ZENKAKU_ALPHA | HANKAKU_ALPHA))
		 &&  start != end; start++ ) {
	jj = b->keyC[start+1] - b->keyC[start];
	strncpy( &key[ii], (char *)&b->key[b->keyC[start]], jj );
	ii += jj;
    }
    key[ii] = '\0';
    for( ii = 0, w[0] = '\0'; key[ii]; ) {
	if( (rt = alpha2hiraganaUpperS( (unsigned char *)&key[ii] )) != NULL ) {
	    wscat( w, rt->yomi );
	    ii += strlen( (char *)rt->key );
	    if( rt->str ) {
		strcpy( ss, &key[ii] );
		strcpy( key, (char *)rt->str );
		strcat( key, ss );
		ii = 0;
	    }
	}
	else {
	    jj = wslen( w );
	    w[jj] = sjis2wc( (unsigned char)key[ii] );
	    w[jj+1] = '\0';
	    ii++;
	}
    }
    return( start );
}


void
#if NeedFunctionPrototypes
code( Widget w, XEvent *event, String *params, Cardinal *n )
#else /* NeedFunctionPrototypes */
code( w, event, params, n )
    Widget	 w;
    XEvent	*event;
    String	*params;
    Cardinal	*n;
#endif /* NeedFunctionPrototypes */
{
    Buffer	*b;

    b = set_buffer( w, event->type );
    if( b == NULL  ||  !check_action_args( params, *n, "code" ) )
	return;

    if( !(b->state & (INTERMEDIATE | EDIT)) )
	return;

    switch( b->conversion_mode ) {
      case SJIS:
	set_label( b, EUC );
	break;

      case EUC:
	set_label( b, JIS );
	break;

      case JIS:
	set_label( b, KUTEN );
	break;

      case KUTEN:
	if( b->use_jisx0212 )
	    set_label( b, SUPP_EUC );
	else
	    set_label( b, SJIS );
	break;

      case SUPP_EUC:
	set_label( b, SUPP_JIS );
	break;

      case SUPP_JIS:
	set_label( b, SUPP_KUTEN );
	break;

      case SUPP_KUTEN:
	set_label( b, SJIS );
	break;

      default:
	switch( resources.code ) {
	  case _SJIS:
	    set_label( b, SJIS );
	    break;

	  case _EUC:
	    set_label( b, EUC );
	    break;

	  case _JIS:
	    set_label( b, JIS );
	    break;

	  case _KUTEN:
	    if( b->use_jisx0212 )
		set_label( b, SUPP_KUTEN );
	    else
		set_label( b, KUTEN );
	    break;

	  case _SUPP_EUC:
	    set_label( b, SUPP_JIS );
	    break;

	  case _SUPP_JIS:
	    set_label( b, SUPP_KUTEN );
	    break;

	  case _SUPP_KUTEN:
	    set_label( b, SJIS );
	    break;
	}
    }
}


void
#if NeedFunctionPrototypes
hankaku_alpha( Widget w, XEvent *event, String *params, Cardinal *n )
#else /* NeedFunctionPrototypes */
hankaku_alpha( w, event, params, n )
    Widget	 w;
    XEvent	*event;
    String	*params;
    Cardinal	*n;
#endif /* NeedFunctionPrototypes */
{
    Buffer		*b;
    int			 ii, jj, len;
    wchar_t		 w1[256], w2[256];
    unsigned int	 hp, start, end, point;
    SJ3_BUNSETU_WCS	 bun;

    b = set_buffer( w, event->type );
    if( b == NULL  ||  (b->state & (DELETE | CODE | ERROR)) )
	return;
    if( !check_action_args( params, *n, "en-alphanumeric" ) )
	return;

    if( b->state & (WRAP | CONVERSION) ) {
	hp = b->henkan->point;
	bun = b->henkan->bun[hp];

	ii = bun.srcstr - b->henkan->bun[0].srcstr;
	for( start = 0; b->yomiC[start] != ii; start++ );
	ii += bun.srclen;
	for( end = start; b->yomiC[end] != ii; end++ );

	for( point = 0; start != end; start++ ) {
	    if( b->mode[start] & (BASIC_CODE | EXTEND) ) {
		ii = b->yomiC[start+1] - b->yomiC[start];
		wsncpy( &w1[point], &b->yomi[b->yomiC[start]], ii );
		point += ii;
	    }
	    else if( (b->str[b->keyC[start]]  &&  point == 0)  ||
		     (b->str[b->keyC[start+1]]  &&  start + 1 == end) ) {
		ii = b->yomiC[start+1] - b->yomiC[start];
		wsncpy( w2, &b->yomi[b->yomiC[start]], ii );
		w2[ii] = '\0';
		ZHtoHK( w2, &w1[point] );
		point += wslen( &w1[point] );
	    }
	    else {
		ii = b->keyC[start+1] - b->keyC[start];
		if( b->str[b->keyC[start]] ) {
		    jj = strlen( (char *)&b->str[b->keyC[start]] );
		    if( ii < jj )
			jj = ii;
		    len = sjiss2wcs( &w1[point], &b->key[b->keyC[start]+jj],
				     ii - jj );
		}
		else
		    len = sjiss2wcs( &w1[point], &b->key[b->keyC[start]], ii );
		point += len;
	    }
	}
	w1[point] = '\0';
	replace_bunsetsu( b, w1, BUNSETSU_HANKAKU_ALPHA, True );
	b->state = CONVERSION;
	b->conversion_mode_toggle = HANKAKU_ALPHA;
    }
    else if( b->state & (INTERMEDIATE | EDIT) ) {
	if( b->conversion_mode == HANKAKU_ALPHA )
	    return;
	if( (b->state & INTERMEDIATE)  &&  b->mode_lock ) {
	    XBell( XtDisplay(b->preedit_widget), 0 );
	    return;
	}
	set_label( b, HANKAKU_ALPHA );
    }
}


void
#if NeedFunctionPrototypes
zenkaku_alpha( Widget w, XEvent *event, String *params, Cardinal *n )
#else /* NeedFunctionPrototypes */
zenkaku_alpha( w, event, params, n )
    Widget	w;
    XEvent	*event;
    String	*params;
    Cardinal	*n;
#endif /* NeedFunctionPrototypes */
{
    Buffer		*b;
    int			 ii, jj, kk;
    unsigned char	 s1[256];
    wchar_t		 w1[256], w2[256];
    unsigned int	 hp, start, end;
    SJ3_BUNSETU_WCS	 bun;

    b = set_buffer( w, event->type );
    if( b == NULL  ||  (b->state & (DELETE | CODE | ERROR)) )
	return;
    if( !check_action_args( params, *n, "em-alphanumeric" ) )
	return;

    if( b->state & (WRAP | CONVERSION) ) {
	hp = b->henkan->point;
	bun = b->henkan->bun[hp];

	ii = bun.srcstr - b->henkan->bun[0].srcstr;
	for( start = 0; b->yomiC[start] != ii; start++ );
	ii += bun.srclen;
	for( end = start; b->yomiC[end] != ii; end++ );

	for( ii = 0; start != end; start++ ) {
	    if( b->mode[start] & (BASIC_CODE | SUPP_CODE | EXTEND) ) {
		w1[ii] = b->yomi[b->yomiC[start]];
		ii++;
	    }
	    else if( (b->str[b->keyC[start]]  &&  ii == 0)  ||
		     (b->str[b->keyC[start+1]]  &&  start + 1 == end) ) {
		jj = b->yomiC[start+1] - b->yomiC[start];
		wsncpy( &w1[ii], &b->yomi[b->yomiC[start]], jj );
		ii += jj;
	    }
	    else {
		jj = b->keyC[start+1] - b->keyC[start];
		if( b->str[b->keyC[start]] ) {
		    kk = strlen( (char *)&b->str[b->keyC[start]] );
		    if( jj < kk )
			kk = jj;
		    ii += sjiss2wcs( &w1[ii], &b->key[b->keyC[start]+kk],
				     jj - kk );
		}
		else {
		    strncpy( (char *)s1, (char *)&b->key[b->keyC[start]], jj );
		    s1[jj] = '\0';
		    ii += sjiss2wcs( &w1[ii], s1, jj );
		}
	    }
	}
	w1[ii] = '\0';
	hankaku2zenkaku( w1, w2, ZENKAKU_HIRAGANA );
	replace_bunsetsu( b, w2, BUNSETSU_ZENKAKU_ALPHA, True );
	b->state = CONVERSION;
	b->conversion_mode_toggle = ZENKAKU_ALPHA;
    }
    else if( b->state & (INTERMEDIATE | EDIT) ) {
	if( b->conversion_mode == ZENKAKU_ALPHA )
	    return;
	if( (b->state & INTERMEDIATE)  &&  b->mode_lock ) {
	    XBell( XtDisplay(b->preedit_widget), 0 );
	    return;
	}
	set_label( b, ZENKAKU_ALPHA );
    }
}


static void
#if NeedFunctionPrototypes
hankaku_hira_kata( Buffer *b, ConversionMode cmode, Bunsetsu bunsetsu )
#else /* NeedFunctionPrototypes */
hankaku_hira_kata( b, cmode, bunsetsu )
    Buffer		*b;
    ConversionMode	 cmode;
    Bunsetsu		 bunsetsu;
#endif /* NeedFunctionPrototypes */
{
    int			 ii, jj;
    wchar_t		 w1[256], w2[256];
    unsigned int	 hp, start, end;
    SJ3_BUNSETU_WCS	 bun;

    if( b->state & (WRAP | CONVERSION) ) {
	hp = b->henkan->point;
	bun = b->henkan->bun[hp];

	ii = bun.srcstr - b->henkan->bun[0].srcstr;
	for( start = 0; b->yomiC[start] != ii; start++ );
	ii += bun.srclen;
	for( end = start; b->yomiC[end] != ii; end++ );

	for( ii = 0, w1[0] = '\0'; start < end; ) {
	    if( b->mode[start] & (ZENKAKU_ALPHA | HANKAKU_ALPHA) ) {
		start = key2kana( b, start, end, w2 );
		wscat( w1, w2 );
		ii += wslen( w2 );
	    }
	    else {
		jj = b->yomiC[start+1] - b->yomiC[start];
		wsncpy( &w1[ii], &b->yomi[b->yomiC[start]], jj );
		ii += jj;
		start++;
		w1[ii] = '\0';
	    }
	}
	ZHtoHK( w1, w2 );
	replace_bunsetsu( b, w2, bunsetsu, True );
	b->state = CONVERSION;
	b->conversion_mode_toggle = cmode;
    }
    else if( b->state & (INTERMEDIATE | EDIT) ) {
	if( b->conversion_mode & (HANKAKU_HIRAGANA | HANKAKU_KATAKANA) )
	    return;
	if( (b->state & INTERMEDIATE)  &&  b->mode_lock ) {
	    XBell( XtDisplay(b->preedit_widget), 0 );
	    return;
	}
	if( b->conversion_mode == ZENKAKU_HIRAGANA )
	    set_label( b, HANKAKU_HIRAGANA );
	else
	    set_label( b, HANKAKU_KATAKANA );
    }
}


void
#if NeedFunctionPrototypes
hankaku_katakana( Widget w, XEvent *event, String *params, Cardinal *n )
#else /* NeedFunctionPrototypes */
hankaku_katakana( w, event, params, n )
    Widget	w;
    XEvent	*event;
    String	*params;
    Cardinal	*n;
#endif /* NeedFunctionPrototypes */
{
    Buffer		*b;

    b = set_buffer( w, event->type );
    if( b == NULL  ||  (b->state & (DELETE | CODE | ERROR)) )
	return;
    if( !check_action_args( params, *n, "en-katakana" ) )
	return;

    hankaku_hira_kata( b, HANKAKU_KATAKANA, BUNSETSU_HANKAKU_KATAKANA );
}


void
#if NeedFunctionPrototypes
zenkaku_katakana( Widget w, XEvent *event, String *params, Cardinal *n )
#else /* NeedFunctionPrototypes */
zenkaku_katakana( w, event, params, n )
    Widget	w;
    XEvent	*event;
    String	*params;
    Cardinal	*n;
#endif /* NeedFunctionPrototypes */
{
    Buffer		*b;
    int			 ii, jj;
    wchar_t		 w1[256], w2[256];
    unsigned int	 hp, start, end;
    SJ3_BUNSETU_WCS	 bun;

    b = set_buffer( w, event->type );
    if( b == NULL  ||  (b->state & (DELETE | CODE | ERROR)) )
	return;
    if( !check_action_args( params, *n, "em-katakana" ) )
	return;

    if( b->state & (WRAP | CONVERSION) ) {
	hp = b->henkan->point;
	bun = b->henkan->bun[hp];

	ii = bun.srcstr - b->henkan->bun[0].srcstr;
	for( start = 0; b->yomiC[start] != ii; start++ );
	ii += bun.srclen;
	for( end = start; b->yomiC[end] != ii; end++ );

	for( ii = 0, w1[0] = '\0'; start < end; ) {
	    if( b->mode[start] & (ZENKAKU_ALPHA | HANKAKU_ALPHA) ) {
		start = key2kana( b, start, end, w2 );
		wscat( w1, w2 );
		ii += wslen( w2 );
	    }
	    else {
		jj = b->yomiC[start+1] - b->yomiC[start];
		wsncpy( &w1[ii], &b->yomi[b->yomiC[start]], jj );
		ii += jj;
		start++;
		w1[ii] = '\0';
	    }
	}

	hankaku2zenkaku( w1, w2, ZENKAKU_KATAKANA );
	ZHtoZK( w2, w1 );
	replace_bunsetsu( b, w1, BUNSETSU_ZENKAKU_KATAKANA, True );
	b->state = CONVERSION;
	b->conversion_mode_toggle = ZENKAKU_KATAKANA;
    }
    else if( b->state & (INTERMEDIATE | EDIT) ) {
	if( b->conversion_mode == ZENKAKU_KATAKANA )
	    return;
	if( (b->state & INTERMEDIATE)  &&  b->mode_lock ) {
	    XBell( XtDisplay(b->preedit_widget), 0 );
	    return;
	}
	set_label( b, ZENKAKU_KATAKANA );
    }
}


void
#if NeedFunctionPrototypes
zenkaku_hiragana( Widget w, XEvent *event, String *params, Cardinal *n )
#else /* NeedFunctionPrototypes */
zenkaku_hiragana( w, event, params, n )
    Widget	 w;
    XEvent	*event;
    String	*params;
    Cardinal	*n;
#endif /* NeedFunctionPrototypes */
{
    Buffer		*b;
    int			 ii, jj;
    wchar_t		 w1[256], w2[256];
    unsigned int	 hp, start, end;
    SJ3_BUNSETU_WCS	 bun;

    b = set_buffer( w, event->type );
    if( b == NULL  ||  (b->state & (DELETE | CODE | ERROR)) )
	return;
    if( !check_action_args( params, *n, "em-hiragana" ) )
	return;

    if( b->state & (WRAP | CONVERSION) ) {
	hp = b->henkan->point;
	bun = b->henkan->bun[hp];

	ii = bun.srcstr - b->henkan->bun[0].srcstr;
	for( start = 0; b->yomiC[start] != ii; start++ );
	ii += bun.srclen;
	for( end = start; b->yomiC[end] != ii; end++ );

	for( ii = 0, w1[0] = '\0'; start < end; ) {
	    if( b->mode[start] & (ZENKAKU_ALPHA | HANKAKU_ALPHA) ) {
		start = key2kana( b, start, end, w2 );
		wscat( w1, w2 );
		ii += wslen( w2 );
	    }
	    else {
		jj = b->yomiC[start+1] - b->yomiC[start];
		wsncpy( &w1[ii], &b->yomi[b->yomiC[start]], jj );
		ii += jj;
		start++;
		w1[ii] = '\0';
	    }
	}

	hankaku2zenkaku( w1, w2, ZENKAKU_HIRAGANA );
	ZKtoZH( w2, w1 );
	replace_bunsetsu( b, w1, BUNSETSU_ZENKAKU_HIRAGANA, True );
	b->state = CONVERSION;
	b->conversion_mode_toggle = ZENKAKU_HIRAGANA;
    }
    else if( b->state & (INTERMEDIATE | EDIT) ) {
	if( b->conversion_mode == ZENKAKU_HIRAGANA )
	    return;
	if( (b->state & INTERMEDIATE)  &&  b->mode_lock ) {
	    XBell( XtDisplay(b->preedit_widget), 0 );
	    return;
	}
	set_label( b, ZENKAKU_HIRAGANA );
    }
}


void
#if NeedFunctionPrototypes
hankaku( Widget w, XEvent *event, String *params, Cardinal *n )
#else /* NeedFunctionPrototypes */
hankaku( w, event, params, n )
    Widget	 w;
    XEvent	*event;
    String	*params;
    Cardinal	*n;
#endif /* NeedFunctionPrototypes */
{
    Buffer	*b;

    b = set_buffer( w, event->type );
    if( b == NULL  ||  (b->state & (DELETE | CODE | ERROR)) )
	return;
    if( !check_action_args( params, *n, "hankaku" ) )
	return;

    if( b->state & (WRAP | CONVERSION) ) {
	switch( b->henkan->bunsetsu[b->henkan->point] & ~BUNSETSU_EDIT ) {
	  case BUNSETSU_ZENKAKU_HIRAGANA:
	  default:
	    hankaku_hira_kata( b, HANKAKU_HIRAGANA, BUNSETSU_HANKAKU_HIRAGANA );
	    break;

	  case BUNSETSU_ZENKAKU_KATAKANA:
	    hankaku_hira_kata( b, HANKAKU_KATAKANA, BUNSETSU_HANKAKU_KATAKANA );
	    break;

	  case BUNSETSU_ZENKAKU_ALPHA:
	    hankaku_alpha( w, event, params, n );
	    break;

	  case BUNSETSU_HANKAKU_HIRAGANA:
	  case BUNSETSU_HANKAKU_KATAKANA:
	  case BUNSETSU_HANKAKU_ALPHA:
	    break;
	}
    }
    else if( b->state & (INTERMEDIATE | EDIT) ) {
	switch( b->conversion_mode ) {
	  case ZENKAKU_ALPHA:
	    hankaku_alpha( w, event, params, n );
	    break;

	  case ZENKAKU_HIRAGANA:
	  case ZENKAKU_KATAKANA:
	    hankaku_katakana( w, event, params, n );
	    break;

	  default:
	    break;
	}
    }
}


void
#if NeedFunctionPrototypes
zenkaku( Widget w, XEvent *event, String *params, Cardinal *n )
#else /* NeedFunctionPrototypes */
zenkaku( w, event, params, n )
    Widget	 w;
    XEvent	*event;
    String	*params;
    Cardinal	*n;
#endif /* NeedFunctionPrototypes */
{
    Buffer	*b;

    b = set_buffer( w, event->type );
    if( b == NULL  ||  (b->state & (DELETE | CODE | ERROR)) )
	return;
    if( !check_action_args( params, *n, "zenkaku" ) )
	return;

    if( b->state & (WRAP | CONVERSION) ) {
	switch( b->henkan->bunsetsu[b->henkan->point] & ~BUNSETSU_EDIT ) {
	  case BUNSETSU_HANKAKU_HIRAGANA:
	  default:
	    zenkaku_hiragana( w, event, params, n );
	    break;

	  case BUNSETSU_HANKAKU_KATAKANA:
	    zenkaku_katakana( w, event, params, n );
	    break;

	  case BUNSETSU_HANKAKU_ALPHA:
	    zenkaku_alpha( w, event, params, n );
	    break;
	}
    }
    else if( b->state & (INTERMEDIATE | EDIT) ) {
	switch( b->conversion_mode ) {
	  case HANKAKU_HIRAGANA:
	    zenkaku_hiragana( w, event, params, n );
	    break;

	  case HANKAKU_KATAKANA:
	    zenkaku_katakana( w, event, params, n );
	    break;

	  case HANKAKU_ALPHA:
	    zenkaku_alpha( w, event, params, n );
	    break;

	  default:
	    break;
	}
    }
}


void
#if NeedFunctionPrototypes
hiragana( Widget w, XEvent *event, String *params, Cardinal *n )
#else /* NeedFunctionPrototypes */
hiragana( w, event, params, n )
    Widget	 w;
    XEvent	*event;
    String	*params;
    Cardinal	*n;
#endif /* NeedFunctionPrototypes */
{
    Buffer	*b;

    b = set_buffer( w, event->type );
    if( b == NULL  ||  (b->state & (DELETE | CODE | ERROR)) )
	return;
    if( !check_action_args( params, *n, "hiragana" ) )
	return;

    if( b->state & (WRAP | CONVERSION) ) {
	switch( b->henkan->bunsetsu[b->henkan->point] & ~BUNSETSU_EDIT ) {
	  case BUNSETSU_ZENKAKU_HIRAGANA:
	  case BUNSETSU_HANKAKU_HIRAGANA:
	    break;

	  case BUNSETSU_ZENKAKU_KATAKANA:
	  case BUNSETSU_ZENKAKU_ALPHA:
	  default:
	    zenkaku_hiragana( w, event, params, n );
	    break;

	  case BUNSETSU_HANKAKU_KATAKANA:
	  case BUNSETSU_HANKAKU_ALPHA:
	    hankaku_hira_kata( b, HANKAKU_HIRAGANA, BUNSETSU_HANKAKU_HIRAGANA );
	    break;
	}
    }
    else if( b->state & (INTERMEDIATE | EDIT) ) {
	switch( b->conversion_mode ) {
	  case ZENKAKU_KATAKANA:
	  case ZENKAKU_ALPHA:
	    zenkaku_hiragana( w, event, params, n );
	    break;

	  case HANKAKU_KATAKANA:
	  case HANKAKU_ALPHA:
	    hankaku_hira_kata( b, HANKAKU_HIRAGANA, BUNSETSU_HANKAKU_HIRAGANA );

	  default:
	    break;
	}
    }
}


void
#if NeedFunctionPrototypes
katakana( Widget w, XEvent *event, String *params, Cardinal *n )
#else /* NeedFunctionPrototypes */
katakana( w, event, params, n )
    Widget	 w;
    XEvent	*event;
    String	*params;
    Cardinal	*n;
#endif /* NeedFunctionPrototypes */
{
    Buffer		*b;

    b = set_buffer( w, event->type );
    if( b == NULL  ||  (b->state & (DELETE | CODE | ERROR)) )
	return;
    if( !check_action_args( params, *n, "katakana" ) )
	return;

    if( b->state & (WRAP | CONVERSION) ) {
	switch( b->henkan->bunsetsu[b->henkan->point] & ~BUNSETSU_EDIT ) {
	  case BUNSETSU_ZENKAKU_HIRAGANA:
	  case BUNSETSU_ZENKAKU_ALPHA:
	  default:
	    zenkaku_katakana( w, event, params, n );
	    break;

	  case BUNSETSU_ZENKAKU_KATAKANA:
	  case BUNSETSU_HANKAKU_KATAKANA:
	    break;

	  case BUNSETSU_HANKAKU_HIRAGANA:
	  case BUNSETSU_HANKAKU_ALPHA:
	    hankaku_hira_kata( b, HANKAKU_KATAKANA, BUNSETSU_HANKAKU_KATAKANA );
	    break;
	}
    }
    else if( b->state & (INTERMEDIATE | EDIT) ) {
	switch( b->conversion_mode ) {
	  case ZENKAKU_HIRAGANA:
	  case ZENKAKU_ALPHA:
	    zenkaku_katakana( w, event, params, n );
	    break;

	  case HANKAKU_HIRAGANA:
	  case HANKAKU_ALPHA:
	    hankaku_katakana( w, event, params, n );
	    break;

	  default:
	    break;
	}
    }
}


void
#if NeedFunctionPrototypes
alpha( Widget w, XEvent *event, String *params, Cardinal *n )
#else /* NeedFunctionPrototypes */
alpha( w, event, params, n )
    Widget	 w;
    XEvent	*event;
    String	*params;
    Cardinal	*n;
#endif /* NeedFunctionPrototyeps */
{
    Buffer		*b;

    b = set_buffer( w, event->type );
    if( b == NULL  ||  (b->state & (DELETE | CODE | ERROR)) )
	return;
    if( !check_action_args( params, *n, "katakana" ) )
	return;

    if( b->state & (WRAP | CONVERSION) ) {
	switch( b->henkan->bunsetsu[b->henkan->point] & ~BUNSETSU_EDIT ) {
	  case BUNSETSU_ZENKAKU_HIRAGANA:
	  case BUNSETSU_ZENKAKU_KATAKANA:
	  default:
	    zenkaku_alpha( w, event, params, n );
	    break;

	  case BUNSETSU_ZENKAKU_ALPHA:
	  case BUNSETSU_HANKAKU_ALPHA:
	    break;

	  case BUNSETSU_HANKAKU_HIRAGANA:
	  case BUNSETSU_HANKAKU_KATAKANA:
	    hankaku_alpha( w, event, params, n );
	}
    }
    else if( b->state & (INTERMEDIATE | EDIT) ) {
	switch( b->conversion_mode ) {
	  case ZENKAKU_HIRAGANA:
	  case ZENKAKU_KATAKANA:
	    zenkaku_alpha( w, event, params, n );
	    break;

	  case HANKAKU_HIRAGANA:
	  case HANKAKU_KATAKANA:
	    hankaku_alpha( w, event, params, n );
	    break;

	  default:
	    break;
	}
    }
}


void
#if NeedFunctionPrototypes
muhenkan( Widget w, XEvent *event, String *params, Cardinal *n )
#else /* NeedFunctionPrototypes */
muhenkan( w, event, params, n )
    Widget	 w;
    XEvent	*event;
    String	*params;
    Cardinal	*n;
#endif /* NeedFunctionPrototypes */
{
    Buffer		*b;
    int			 ii;
    wchar_t		 ss[256], tt[256];
    unsigned int	 hp, start, end;
    SJ3_BUNSETU_WCS	 bun;
    Bunsetsu		 mode;
    wchar_t		*string;
    int			 length;


    b = set_buffer( w, event->type );
    if( b == NULL  ||  (b->state & ~(INTERMEDIATE | CONVERSION | WRAP))  ||
	!check_action_args( params, *n, "no-conversion" ) )
	return;

    if( b->state & (WRAP | CONVERSION) ) {
	hp = b->henkan->point;
	bun = b->henkan->bun[hp];

	ii = bun.srcstr - b->henkan->bun[0].srcstr;
	for( start = 0; b->yomiC[start] != ii; start++ );
	ii += bun.srclen;
	for( end = start; b->yomiC[end] != ii; end++ );

	for( ss[0] = '\0'; start != end; start++ ) {
	    ii = b->yomiC[start+1] - b->yomiC[start];
	    wsncpy( tt, &b->yomi[b->yomiC[start]], ii );
	    tt[ii] = '\0';
	    wscat( ss, tt );
	}
	mode = BUNSETSU_MUHENKAN | BUNSETSU_ZENKAKU_HIRAGANA;
	if( (b->henkan->bunsetsu[hp] & mode) == mode ) {
	    mode = BUNSETSU_MUHENKAN | BUNSETSU_ZENKAKU_KATAKANA;
	    ZHtoZK( ss, tt );
	    wscpy( ss, tt );
	}
	replace_bunsetsu( b, ss, mode, True );
	b->state = CONVERSION;
	b->conversion_mode_toggle = resources.initial_mode;
    }
    else if( b->state & INTERMEDIATE ) {
	if( b->yomi[0] == '\0' )
	    return;

#ifdef USE_SJ3LIB
	switch( sj3_open( resources.server, user_name ) ) {
	  case SJ3_SERVER_DEAD:
	  case SJ3_CONNECT_ERROR:
	    server_error( b, w, resources.died_error );
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
#endif
	b->henkan = (Henkan *)calloc( 1, sizeof(Henkan) );
	if( b->henkan == NULL ) {
	    XBell( event->xany.display, 0 );
	    return;
	}
	b->state = CONVERSION;
	romaji2kana( b );

	b->henkan->bunsetsu[0] = BUNSETSU_KANJI;
	b->henkan->bunsetsu[1] = BUNSETSU_END;
	string = b->henkan->bun[0].srcstr = b->yomi;
	b->henkan->bun[0].deststr = b->henkan->kanji;
	length =
	b->henkan->bunsetsu_length[0] =
	b->henkan->bun[0].srclen =
	b->henkan->bun[0].destlen = wslen( b->yomi );
	wscpy( b->henkan->kanji, b->yomi );
	b->henkan->bunsetsu[0] = BUNSETSU_MUHENKAN | BUNSETSU_ZENKAKU_HIRAGANA;

	XtVaSetValues( b->preedit_widget, XmwNdisplayCaret, False, NULL );
	if( b->conversion_style & XIMPreeditNothing ) {
	    XmwTextOutSetStringWcs( b->preedit_widget, string,
				    XmwHIGHLIGHT_NORMAL );
	    XmwTextOutSetHighlight( b->preedit_widget, 0, length,
				    XmwHIGHLIGHT_SELECTED );
	    XmwTextOutSetCursorPosition( b->preedit_widget, 0 );
	}
	else {
	    XmwPreeditSetStringWcs( w, string,
				    XmwHIGHLIGHT_SECONDARY_SELECTED );
	    XmwPreeditSetHighlight( b->preedit_widget, 0, length,
				    XmwHIGHLIGHT_SELECTED );
	    XmwPreeditSetCursorPosition( w, 0 );
	}
	b->henkan->point = 0;

	b->conversion_mode_toggle = b->conversion_mode;
    }
}


void
#if NeedFunctionPrototypes
mode_lock( Widget w, XEvent *event, String *params, Cardinal *n )
#else /* NeedFunctionPrototypes */
mode_lock( w, event, params, n )
    Widget	 w;
    XEvent	*event;
    String	*params;
    Cardinal	*n;
#endif /* NeedFunctionPrototypes */
{
    Buffer	*b;
    Window	 window;

    b = set_buffer( w, event->type );
    if( b == NULL  ||  (b->state & ~INTERMEDIATE)  ||
	!check_action_args( params, *n, "input-mode-lock" ) )
	return;

    if( b->state & INTERMEDIATE ) {
	if( !b->mode_lock  &&  b->yomi[0] ) {
	    window = event->xkey.subwindow;
	    SendCompoundText( event->xany.display, window, b->yomi );
	    buffer_initialize( b );
	}
	b->mode_lock = !b->mode_lock;
	set_label( b, b->conversion_mode );
    }
}


void
#if NeedFunctionPrototypes
initial_mode( Widget w, XEvent *event, String *params, Cardinal *n )
#else /* NeedFunctionPrototypes */
initial_mode( w, event, params, n )
    Widget	 w;
    XEvent	*event;
    String	*params;
    Cardinal	*n;
#endif /* NeedFunctionPrototypes */
{
    Buffer	*b;

    b = set_buffer( w, event->type );
    if( b == NULL  ||  (b->state & ~(INTERMEDIATE | EDIT))  ||
	!check_action_args( params, *n, "initial-mode" ) )
	return;

    if( b->state & (INTERMEDIATE | EDIT) ) {
	b->mode_lock = False;
	set_label( b, resources.initial_mode );
    }
}
