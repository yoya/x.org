/* $SonyId: SjInkey.c,v 1.2 1994/06/03 06:33:04 makoto Exp $ */
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
#include	<stdlib.h>
#include	<ctype.h>
#define XK_KATAKANA
#include	<X11/keysym.h>
#include	<X11/Xatom.h>
#include	<X11/Xlib.h>
#include	<X11/Xlibint.h>
#include	<Xmw/TextOut.h>
#include	<Xmw/Preedit.h>
#include	"SjBuffer.h"
#include	"sjxa.h"
#include	"resource.h"
#include	"SjRk.h"
#include	"SjString.h"

extern Sjxa	resources;
extern Widget	RootTextField;
extern Widget	ServerText, TourokuText[2];
extern Widget	Server, Touroku;


static Boolean
#if NeedFunctionPrototypes
insert_buffer( Buffer *b, unsigned char key, unsigned char str,
	       ConversionMode mode )
#else	/* NeedFunctionPrototypes */
insert_buffer( b, key, str, mode )
    Buffer		*b;
    unsigned char	key, str;
    ConversionMode	mode;
#endif
{
    unsigned int	p = b->point;
    unsigned int	ep = b->end_point;
    int			ii;

    if( (ep == MAX_KEY - 1)  ||  (b->keyC[ep] == (MAX_KEY - 1)) ) {
	load_buffer( b );
	return( False );
    }

    if( b->keyC[p] == b->keyC[p+1] ) {
	if( p + 1 != ep ) {
	    fputs( "buffer error", stderr );
	    done();
	    exit( 1 );
	}
	b->key[b->keyC[p]] = key;
	b->str[b->keyC[p]] = str;
	b->keyC[ep+1] = ++b->keyC[ep];
	b->yomiC[ep+1] = b->yomiC[ep];
	b->mode[p] = mode;
    }
    else if( b->conversion_flag[p]  ||
	     (b->keyC[p] != b->keyC[p+1]  &&  b->yomiC[p] == b->cursor) ) {
	for( ii = b->keyC[ep] - 1; ii >= 0  &&  ii >= b->keyC[p]; ii-- ) {
	    b->key[ii+1] = b->key[ii];
	    b->str[ii+1] = b->str[ii];
	}
	b->key[b->keyC[p]] = key;
	b->str[b->keyC[p]] = str;

	for( ii = ep; ii >= 0  &&  ii >= p; ii-- ) {
	    b->keyC[ii+1] = b->keyC[ii] + 1;
	    b->yomiC[ii+1] = b->yomiC[ii];
	    b->mode[ii+1] = b->mode[ii];
	    b->conversion_flag[ii+1] = b->conversion_flag[ii];
	}
	b->mode[p] = mode;
	b->end_point++;
    }
    else if( b->mode[p] == mode ) {
	for( ii = b->keyC[ep] - 1; ii >= 0  &&  ii >= b->keyC[p+1]; ii-- ) {
	    b->key[ii+1] = b->key[ii];
	    b->str[ii+1] = b->str[ii];
	}
	b->key[b->keyC[p+1]] = key;
	b->str[b->keyC[p+1]] = str;

	for( ii = ep + 1; ii > p; ii-- )
	    b->keyC[ii]++;
    }
    else {
	p = ++b->point;
	for( ii = b->keyC[ep] - 1; ii >= 0  &&  ii >= b->keyC[p]; ii-- ) {
	    b->key[ii+1] = b->key[ii];
	    b->str[ii+1] = b->str[ii];
	}
	b->key[b->keyC[p]] = key;
	b->str[b->keyC[p]] = str;

	for( ii = ep; ii >= 0  &&  ii >= p; ii-- ) {
	    b->keyC[ii+1] = b->keyC[ii] + 1;
	    b->yomiC[ii+1] = b->yomiC[ii];
	    b->mode[ii+1] = b->mode[ii];
	    b->conversion_flag[ii+1] = b->conversion_flag[ii];
	}
	b->mode[p] = mode;
	b->end_point++;
    }
    return( True );
}


void	delete_character_buffer( b, w, yomi, mode )
Buffer		*b;
Widget		w;
wchar_t		*yomi;
ConversionMode	mode;
{
    unsigned int	p, ep;
    wchar_t		ss[MAX_KANA];
    unsigned int	len;
    int			ii;
    int			start, end;

    p = b->point;
    ep = b->end_point;
    if( yomi != NULL ) {
	if( b->conversion_style & XIMPreeditNothing ) {
	    start = b->yomiC[p];
	    end = b->yomiC[p+1];
	    XmwTextOutSetCursorPosition( w, start );
	    wscpy( ss, &b->yomi[b->yomiC[p+1]] );
	    wscpy( &b->yomi[b->yomiC[p]], yomi );
	    wscat( b->yomi, ss );
	    len = wslen(yomi) - (b->yomiC[p+1] - b->yomiC[p]);
	    for( ii = p + 1; ii <= ep; ii++ )
		b->yomiC[ii] += len;
	    XmwTextOutReplaceWcs( w, start, end, yomi );
	}
	else if( b->conversion_style & (XIMPreeditPosition | XIMPreeditArea) ) {
	    if( !(b->state & EDIT) ) {
		XmwPreeditReplaceWcs( w, b->yomiC[p], b->yomiC[p+1], yomi );
	    }
	    else {
		for( ii = 0, start = 0; ii < b->henkan->point; ii++ )
		    start += b->henkan->bun[ii].destlen;
		XmwPreeditReplaceWcs( w, b->yomiC[p] + start,
				      b->yomiC[p+1] + start, yomi );
	    }
	    wscpy( ss, &b->yomi[b->yomiC[p+1]] );
	    wscpy( &b->yomi[b->yomiC[p]], yomi );
	    wscat( b->yomi, ss );
	    len = wslen(yomi) - (b->yomiC[p+1] - b->yomiC[p]);
	    for( ii = p + 1; ii <= ep; ii++ )
		b->yomiC[ii] += len;
	}
    }
    else {
	if( b->conversion_style & XIMPreeditNothing ) {
	    start = b->yomiC[p+1] - 1;
	    end = b->yomiC[p+1];
	    wscpy( ss, &b->yomi[b->yomiC[p+1]] );
	    wscpy( &b->yomi[b->yomiC[p+1]-1], ss );
	    for( ii = p + 1; ii <= ep; ii++ )
		b->yomiC[ii]--;
	    XmwTextOutReplace( w, start, end, NULL );
	}
	else if( b->conversion_style & (XIMPreeditPosition | XIMPreeditArea) ) {
	    if( !(b->state & EDIT) ) {
		XmwPreeditReplace( w, b->yomiC[p+1] - 1, b->yomiC[p+1], NULL );
	    }
	    else {
		for( ii = 0, start = 0; ii < b->henkan->point; ii++ )
		    start += b->henkan->bun[ii].destlen;
		XmwPreeditReplace( w, b->yomiC[p+1] - 1 + start,
				   b->yomiC[p+1]+start, NULL );
	    }
	    wscpy( ss, &b->yomi[b->yomiC[p+1]] );
	    wscpy( &b->yomi[b->yomiC[p+1]-1], ss );
	    for( ii = p + 1; ii <= ep; ii++ )
		b->yomiC[ii]--;
	}
    }

    for( ii = b->keyC[p+1]; ii <= b->keyC[ep]; ii++ ) {
	b->key[ii-1] = b->key[ii];
	b->str[ii-1] = b->str[ii];
    }
    for( ii = p + 1; ii <= ep; ii++ )
	b->keyC[ii]--;

    b->conversion_flag[p] = False;
    b->cursor = b->yomiC[p+1];
}


void	delete_word_buffer( b, w )
Buffer	*b;
Widget	w;
{
    unsigned int	p = b->point;
    unsigned int	ep = b->end_point;
    unsigned char	ss[MAX_KEY];
    wchar_t		ww[MAX_KANA];
    unsigned int	lk, ly;
    int			start, ii;

    if( b->conversion_style & XIMPreeditNothing  ||
	(b->state & (SERVER | ADD)) ) {
	XmwTextOutSetCursorPosition( w, b->yomiC[p] );
	XmwTextOutReplace( w, b->yomiC[p], b->yomiC[p+1], NULL );
    }
    else if( b->conversion_style & (XIMPreeditPosition | XIMPreeditArea) ) {
	if( !(b->state & EDIT) ) {
	    XmwPreeditReplace( b->preedit_widget, b->yomiC[p], b->yomiC[p+1],
			       NULL );
	}
	else {
	    for( ii = 0, start = 0; ii < b->henkan->point; ii++ )
		start += b->henkan->bun[ii].destlen;
	    XmwPreeditReplace( b->preedit_widget, b->yomiC[p]+start,
			       b->yomiC[p+1]+start, NULL );
	}
    }
    b->cursor = b->yomiC[p];

    strcpy( (char *)ss, (char *)&b->key[b->keyC[p+1]] );
    strcpy( (char *)&b->key[b->keyC[p]], (char *)ss );

    if( (lk = b->keyC[ep] - b->keyC[p+1]) != 0 ) {
	bcopy( &b->str[b->keyC[p+1]], ss, lk );
	bcopy( ss, &b->str[b->keyC[p]], lk );
    }
    else
	b->str[b->keyC[p]] = '\0';

    wscpy( ww, &b->yomi[b->yomiC[p+1]] );
    wscpy( &b->yomi[b->yomiC[p]], ww );

    lk = b->keyC[p+1] - b->keyC[p];
    ly = b->yomiC[p+1] - b->yomiC[p];
    for( ii = p; ii < ep; ii++ ) {
	b->keyC[ii] = b->keyC[ii+1] - lk;
	b->yomiC[ii] = b->yomiC[ii+1] - ly;
	b->mode[ii] = b->mode[ii+1];
	b->conversion_flag[ii] = b->conversion_flag[ii+1];
    }
    b->keyC[ep] -= lk;
    b->yomiC[ep] -= ly;

    if( ep > 2  &&  b->keyC[ep-2] == b->keyC[ep] )
	b->end_point--;
}


static Boolean	split_key( b, rt )
Buffer	*b;
RkTable	*rt;
{
    unsigned int	p = b->point++;
    unsigned int	ep = b->end_point;
    unsigned int	length;
    int			ii;

    b->keyC[ep+1] = b->keyC[ep];
    if( b->point == ep  ||  rt->str != NULL )
	ep = ++b->end_point;

    b->conversion_flag[p] = True;
    length = (rt->str == NULL) ? 0 : strlen( (char *)rt->str );
    if( length == 0  &&  (b->keyC[p+1] - b->keyC[p] == strlen((char *)rt->key)) )
	return( False );
    for( ii = ep; ii >= 0  &&  ii > p; ii-- ) {
	b->keyC[ii+1] = b->keyC[ii] + length;
	b->yomiC[ii+1] = b->yomiC[ii];
	b->mode[ii+1] = b->mode[ii];
	b->conversion_flag[ii+1] = b->conversion_flag[ii];
    }
    b->mode[p+1] = b->mode[p];
    b->keyC[p+1] = b->keyC[p] + strlen( (char *)rt->key );
    p = b->point;
    b->yomiC[p] = b->yomiC[p+1];
    b->conversion_flag[p] = False;

    if( length != 0 ) {
	for( ii = b->keyC[b->end_point]; ii >= 0  &&  ii >= b->keyC[p]; ii-- ) {
	    b->key[ii+length] = b->key[ii];
	    b->str[ii+length] = b->str[ii];
	}
	strncpy( (char *)&b->key[b->keyC[p]], (char *)rt->str, length );
	strncpy( (char *)&b->str[b->keyC[p]], (char *)rt->str, length );
    }
    return( True );
}


static Boolean	replace_yomi( b, yomi )
Buffer	*b;
wchar_t	*yomi;
{
    unsigned int	p = b->point;
    unsigned int	ep = b->end_point;
    wchar_t		ww[MAX_KANA];
    int			length;
    int			ii;

    wscpy( ww, &b->yomi[b->yomiC[p+1]] );
    wscpy( &b->yomi[b->yomiC[p]], yomi );
    wscat( b->yomi, ww );

    length = (b->yomiC[p+1] - b->yomiC[p]) - wslen( yomi );
    if( (b->yomiC[ep] - length) >= MAX_KANA ) {
	load_buffer( b );
	return( False );
    }
    for( ii = p + 1; ii <= ep; ii++ )
	b->yomiC[ii] -= length;
    b->yomiC[ep+1] = b->yomiC[ep];

    return( True );
}


static Boolean	add_yomi( b, yomi )
Buffer	*b;
wchar_t	*yomi;
{
    unsigned int	p = b->point;
    unsigned int	ep = b->end_point;
    wchar_t		ww[MAX_KANA];
    int			length;
    int			ii;

    if( (b->yomiC[ep] + wslen(yomi)) >= MAX_KANA ) {
	load_buffer( b );
	return( False );
    }

    wscpy( ww, &b->yomi[b->yomiC[p+1]] );
    wscpy( &b->yomi[b->yomiC[p+1]], yomi );
    wscat( b->yomi, ww );

    length = wslen( yomi );
    for( ii = p + 1; ii <= ep; ii++ )
	b->yomiC[ii] += length;
    b->yomiC[ep+1] = b->yomiC[ep];
    return( True );
}


void
#if NeedFunctionPrototypes
romaji2kana( Buffer *b )
#else /* NeedFunctionPrototypes */
romaji2kana( b )
    Buffer	*b;
#endif /* NeedFunctionPrototypes */
{
    unsigned int	p;
    RkTable		*rt;
    unsigned char	ss[32], c;
    wchar_t		ww[32], xx[32];
    unsigned short	zenkaku;
    int			ii;

    for( p = 0; p < b->end_point; p++ ) {
	if( !b->conversion_flag[p] ) {
	    save_buffer( b );
	    strncpy( (char *)ss, (char *)&b->key[b->keyC[p]],
		     (ii = b->keyC[p+1] - b->keyC[p]) );
	    ss[ii] = '\0';
	    if( (rt = alpha2hiraganaUpperS(ss)) != NULL ) {
		if( b->mode[p] == ZENKAKU_HIRAGANA )
		    wscpy( ww, rt->yomi );
		else if( b->mode[p] == ZENKAKU_KATAKANA )
		    ZHtoZK( rt->yomi, ww );
		else
		    ZHtoHK( rt->yomi, ww );
		b->point = p;
		if( !replace_yomi( b, ww ) )
		    return;
		b->conversion_flag[p] = True;

		if( split_key( b, rt ) ) {
		    if( rt->str[0] != '\0' ) {
			if( b->mode[p] &
			    (HANKAKU_HIRAGANA | HANKAKU_KATAKANA) ) {
			    mbs2wcs( xx, (char *)rt->str,
				      strlen((char *)rt->str) );
			}
			else {
			    for( ii = 0; (c = rt->str[ii]) != '\0'; ii++ ) {
				zenkaku = (unsigned short)hantozen( c );
				mbtowc( &xx[ii], (char *)&zenkaku, MB_CUR_MAX );
			    }
			    xx[ii] = '\0';
			}
			if( !add_yomi( b, xx ) )
			    return;
		    }
		}
	    }
	}
    }
}


static void
#if NeedFunctionPrototypes
hankaku_alpha( Buffer *b, XmwTextVerifyPtr verify )
#else /* NeedFunctionPrototypes */
hankaku_alpha( b, verify )
    Buffer		*b;
    XmwTextVerifyPtr	verify;
#endif /* NeedFunctionPrototypes */
{
    unsigned char	c;

    save_buffer( b );
    c = verify->text->ptr[0];
    mbtowc( verify->text->wcs_ptr, (char *)&c, 1 );
    verify->text->wcs_ptr[1] = '\0';
    if( !insert_buffer( b, c, '\0', HANKAKU_ALPHA )  ||
	!add_yomi( b, verify->text->wcs_ptr ) ) {
	verify->doit = False;
	return;
    }

    b->conversion_flag[b->point++] = True;
    if( b->point == b->end_point )
	b->end_point++;
    b->cursor = verify->currInsert + 1;

    if( b->conversion_style & (XIMPreeditPosition | XIMPreeditArea) ) {
	if( !(b->state & EDIT ) )
	    verify->text->mode = XmwHIGHLIGHT_SECONDARY_SELECTED;
	else
	    verify->text->mode = XmwHIGHLIGHT_SELECTED;
    }
}


static void
#if NeedFunctionPrototypes
zenkaku_alpha( Buffer *b, XmwTextVerifyPtr verify )
#else /* NeedFunctionPrototypes */
zenkaku_alpha( b, verify )
    Buffer		*b;
    XmwTextVerifyPtr	verify;
#endif /* NeedFunctionPrototypes */
{
    unsigned char	c;
    unsigned short	zenkaku;
    char		mb[2];
    wchar_t		*ww = verify->text->wcs_ptr;

    save_buffer( b );
    c = verify->text->ptr[0];
    if( !insert_buffer( b, c, '\0', ZENKAKU_ALPHA ) ) {
	verify->doit = False;
	return;
    }

    zenkaku = hantozen( c );
    mb[0] = zenkaku >> 8;
    mb[1] = zenkaku & 0xff;
    mbtowc( ww, mb, 2 );
    ww[1] = '\0';
    if( !add_yomi( b, ww ) ) {
	verify->doit = False;
	return;
    }

    b->conversion_flag[b->point++] = True;
    if( b->point == b->end_point )
	b->end_point++;
    b->cursor = verify->currInsert + 1;

    verify->text->length = 1;
    if( b->conversion_style & (XIMPreeditPosition | XIMPreeditArea) ) {
	if( b->state & EDIT )
	    verify->text->mode = XmwHIGHLIGHT_SELECTED;
	else
	    verify->text->mode = XmwHIGHLIGHT_SECONDARY_SELECTED;
    }
}


static void
#if NeedFunctionPrototypes
kana( Buffer *b, XmwTextVerifyPtr verify, ConversionMode mode )
#else /* NeedFunctionPrototypes */
kana( b, verify, mode )
    Buffer		*b;
    XmwTextVerifyPtr	verify;
    ConversionMode	mode;
#endif /* NeedFunctionPrototypes */
{
    unsigned char	c;
    unsigned int	point;
    unsigned short	zenkaku;
    char		ss[32];
    wchar_t		ww[32], xx[32];
    RkTable		*rt, rkt;
    int			ii, jj;
    char		mb[2];

    save_buffer( b );
    c = verify->text->ptr[0];
    if( !insert_buffer( b, c, '\0', mode ) ) {
	verify->doit = False;
	return;
    }
    
    point = b->point;
    b->conversion_flag[point] = False;
    if( (ii = b->keyC[point+1] - b->keyC[point]) > 30 ) {
	verify->doit = False;
	return;
    }

    strncpy( ss, (char *)&b->key[b->keyC[point]],
	     (ii = b->keyC[point+1] - b->keyC[point]) );
    ss[ii] = '\0';
    if( (rt = alpha2hiraganaLower( (unsigned char *)ss )) == NULL ) {
	if( alpha2hiraganaLowerD( (unsigned char *)ss ) != NULL ) {
	    if( mode & (HANKAKU_HIRAGANA | HANKAKU_KATAKANA) ) {
		mbtowc( verify->text->wcs_ptr, (char *)&c, MB_CUR_MAX );
	    }
	    else {
		zenkaku = (unsigned short)hantozen( c );
		mb[0] = zenkaku >> 8;
		mb[1] = zenkaku & 0xff;
		mbtowc( verify->text->wcs_ptr, mb, MB_CUR_MAX );
	    }
	    verify->text->wcs_ptr[1] = '\0';
	    if( !add_yomi( b, verify->text->wcs_ptr ) ) {
		verify->doit = False;
		return;
	    }
	    verify->text->length = 1;
	    b->cursor = verify->currInsert + 1;
	}
	else {
	    if( b->rt == NULL ) {
		if( (ii = jj = strlen(ss)) != 1 ) {
		    rkt.key = (unsigned char *)malloc( ii );
		    ii--;
		    strncpy( (char *)rkt.key, ss, ii );
		    rkt.key[ii] = '\0';
		    rkt.str = NULL;
		    split_key( b, &rkt );
		    free( rkt.key );
		}

		point = b->point;
		b->conversion_flag[point-1] = False;
		ss[0] = c;
		ss[1] = '\0';
		rt = alpha2hiraganaLower( (unsigned char *)ss );
		b->rt = rt;
		if( rt != NULL  &&  rt->next != NULL  &&
		    strncmp( ss, (char *)rt->next->key, strlen(ss) ) != 0 ) {
		    b->rt = NULL;
		    if( mode == ZENKAKU_HIRAGANA )
			wscpy( ww, rt->yomi );
		    else if( mode == ZENKAKU_KATAKANA )
			ZHtoZK( rt->yomi, ww );
		    else
			ZHtoHK( rt->yomi, ww );
		    verify->text->length = wslen( ww );
		    b->cursor = verify->currInsert + verify->text->length;
		    b->conversion_flag[point] = True;
		    wscpy( verify->text->wcs_ptr, ww );
		    jj = 1;
		}
		else {
		    if( mode & (HANKAKU_HIRAGANA | HANKAKU_KATAKANA) )
			mbtowc( verify->text->wcs_ptr, (char *)&c, MB_CUR_MAX );
		    else {
			zenkaku = (unsigned short)hantozen( c );
			mb[0] = zenkaku >> 8;
			mb[1] = zenkaku & 0xff;
			mbtowc( verify->text->wcs_ptr, mb, MB_CUR_MAX );
		    }
		    verify->text->wcs_ptr[1] = '\0';
		    verify->text->length = 1;
		    b->cursor = verify->currInsert + 1;
		}
		if( !add_yomi( b, verify->text->wcs_ptr ) ) {
		    verify->doit = False;
		    return;
		}
		if( jj == 1 ) {
		    b->point++;
		    if( b->point == b->end_point )
			b->end_point++;
		}
	    }
	    else {
		rt = b->rt;
		b->rt = NULL;
		verify->startPos = b->yomiC[point];
		verify->endPos = b->yomiC[point+1];
		b->cursor = b->yomiC[point];
		ww[0] = '\0';
		do {
		    if( mode == ZENKAKU_HIRAGANA )
			wscpy( xx, rt->yomi );
		    else if( mode == ZENKAKU_KATAKANA )
			ZHtoZK( rt->yomi, xx );
		    else
			ZHtoHK( rt->yomi, xx );
		    wscat( ww, xx );
		    if( !replace_yomi( b, xx ) ) {
			verify->doit = False;
			return;
		    }
		    if( !split_key( b, rt ) )
			break;

		    point = b->point;
		    strncpy( ss, (char *)&b->key[b->keyC[point]],
			     (ii = b->keyC[point+1] - b->keyC[point]) );
		    ss[ii] = '\0';
		    rt = alpha2hiraganaUpperS( (unsigned char *)ss );
		    if( rt != NULL  &&  rt->next != NULL ) {
			jj = strlen( (char *)rt->key );
			if( ii == jj  &&
			    strncmp( (char *)rt->key, (char *)rt->next->key,
				     jj ) == 0 ) {
			    b->rt = rt;
			    rt = NULL;
			}
		    }
		} while( rt != NULL );

		if( rt == NULL ) {
		    if( mode & (HANKAKU_HIRAGANA | HANKAKU_KATAKANA) ) {
			for( ii = 0, jj = b->keyC[point], xx[0] = '\0';
			     jj < b->keyC[point+1]; jj++ )
			    mbtowc( &xx[ii], (char *)b->key[jj], MB_CUR_MAX );
			    ii++;
		    }
		    else {
			for( ii = 0, jj = b->keyC[point], xx[0] = '\0';
			    jj < b->keyC[point+1]; jj++ ) {
			    zenkaku = (unsigned short)hantozen( b->key[jj] );
			    mb[0] = zenkaku >> 8;
			    mb[1] = zenkaku & 0xff;
			    mbtowc( &xx[ii], mb, MB_CUR_MAX );
			    ii++;
			}
		    }
		    xx[ii] = '\0';
		    wscat( ww, xx );
		    if( !add_yomi( b, xx ) ) {
			verify->doit = False;
			return;
		    }
		}
		verify->text->length = wslen( ww );
		wscpy( verify->text->wcs_ptr, ww );
		b->cursor += verify->text->length;
	    }
	}
    }
    else {
	b->rt = NULL;
	if( rt->next != NULL  &&
	    strncmp( ss, (char *)rt->next->key, strlen(ss) ) == 0 ) {
	    b->rt = rt;
	    if( mode & (HANKAKU_HIRAGANA | HANKAKU_KATAKANA) ) {
		mbtowc( verify->text->wcs_ptr, ss, MB_CUR_MAX );
	    }
	    else {
		zenkaku = hantozen( c );
		mb[0] = zenkaku >> 8;
		mb[1] = zenkaku & 0xff;
		mbtowc( verify->text->wcs_ptr, mb, MB_CUR_MAX );
	    }
	    verify->text->wcs_ptr[1] = '\0';
	    if( !add_yomi( b, verify->text->wcs_ptr ) ) {
		verify->doit = False;
		return;
	    }
	    verify->text->length = 1;
	    b->cursor = verify ->currInsert + 1;
	}
	else {
	    verify->startPos = b->yomiC[point];
	    verify->endPos = b->yomiC[point+1];
	    if( mode == ZENKAKU_HIRAGANA )
		wscpy( ww, rt->yomi );
	    else if( mode == ZENKAKU_KATAKANA )
		ZHtoZK( rt->yomi, ww );
	    else
		ZHtoHK( rt->yomi, ww );
	    verify->text->length = wslen( ww );
	    b->cursor = b->yomiC[point] + verify->text->length;

	    if( !replace_yomi( b, ww ) ) {
		verify->doit = False;
		return;
	    }
	    b->conversion_flag[point] = True;

	    if( split_key( b, rt ) ) {
		if( mode & (HANKAKU_HIRAGANA | HANKAKU_KATAKANA) ) {
		    mbs2wcs( xx, (char *)rt->str, strlen((char *)rt->str) );
		}
		else {
		    for( ii = 0; (c = rt->str[ii]) != '\0'; ii++ ) {
			zenkaku = (unsigned short)hantozen( c );
			mb[0] = zenkaku >> 8;
			mb[1] = zenkaku & 0xff;
			mbtowc( &xx[ii], mb, MB_CUR_MAX );
		    }
		    xx[ii] = '\0';
		}
		wscat( ww, xx );
		if( !add_yomi( b, xx ) ) {
		    verify->doit = False;
		    return;
		}
		b->cursor += wslen( xx );
		verify->text->length += wslen( xx );
	    }
	    wscpy( verify->text->wcs_ptr, ww );
	}
    }
    if( b->conversion_style & (XIMPreeditPosition | XIMPreeditArea) ) {
	if( b->state & EDIT )
	    verify->text->mode = XmwHIGHLIGHT_SELECTED;
	else
	    verify->text->mode = XmwHIGHLIGHT_SECONDARY_SELECTED;
    }
}


static void
#if NeedFunctionPrototypes
code( Buffer *b, XmwTextVerifyPtr verify, ConversionMode mode )
#else /* NeedFunctionPrototypes */
code( b, verify, mode )
    Buffer		*b;
    XmwTextVerifyPtr	verify;
    ConversionMode	mode;
#endif /* NeedFunctionPrototypes */
{
    unsigned char	c;
    unsigned int	p, ep;
    unsigned short	zenkaku;
    char		ss[5], mb[3];
    int			ii, jj;
    wchar_t		wc;

    save_buffer( b );
    c = verify->text->ptr[0];
    if( !isxdigit( c )  ||
	((mode & (KUTEN | SUPP_KUTEN))  &&  !isdigit( c )) ) {
	verify->doit = False;
	return;
    }
    if( !insert_buffer( b, c, '\0', mode ) ) {
	verify->doit = False;
	return;
    }

    p = b->point;
    ep = b->end_point;
    b->conversion_flag[p] = False;

    if( (b->keyC[p+1] - b->keyC[p]) != 4 ) {
	zenkaku = (unsigned short)hantozen( c );
	mb[0] = zenkaku >> 8;
	mb[1] = zenkaku & 0xff;
	mbtowc( verify->text->wcs_ptr, mb, MB_CUR_MAX );
	verify->text->wcs_ptr[1] = '\0';
	if( !add_yomi( b, verify->text->wcs_ptr ) ) {
	    verify->doit = False;
	    return;
	}
	verify->text->length = 1;
	b->cursor = verify->currInsert + 1;
    }
    else {
	if( mode & SUPP_CODE ) {
	    if( mode & SUPP_KUTEN ) {
		ii = b->keyC[p];
		jj = (((b->key[ii] - '0') * 10 + (b->key[ii+1] - '0')) << 8)
		   | ((b->key[ii+2] - '0') * 10 + (b->key[ii+3] - '0'));
		zenkaku = kuten2euc( jj );
	    }
	    else {
		strncpy( ss, (char *)&b->key[b->keyC[p]], 4 );
		ss[4] = '\0';
		sscanf( ss, "%x", &ii );
		zenkaku = (mode == SUPP_EUC) ? ii : jis2euc(ii);
	    }
	    mb[0] = SS3;
	    mb[1] = zenkaku >> 8;
	    mb[2] = zenkaku & 0xff;
	    mbtowc( &wc, mb, MB_CUR_MAX );
	}
	else if( mode != KUTEN ) {
	    strncpy( ss, (char *)&b->key[b->keyC[p]], 4 );
	    ss[4] = '\0';
	    sscanf( ss, "%x", &ii );
	    wc = (mode == SJIS ) ? sjis2wc(ii)
	       : (mode == EUC)   ? euc2wc(ii)
				 : jis2wc(ii);
	}
	else {
	    ii = b->keyC[p];
	    jj = (((b->key[ii] - '0') * 10 + (b->key[ii+1] - '0')) << 8)
	       | ((b->key[ii+2] - '0') * 10 + (b->key[ii+3] - '0'));
	    wc = kuten2wc( jj );
	}
	if( wc == 0 ) {
	    verify->doit = False;
	    for( ii = b->keyC[p+1]; ii <= b->keyC[ep]; ii++ ) {
		b->key[ii-1] = b->key[ii];
		b->str[ii-1] = b->str[ii];
	    }
	    for( ii = p + 1; ii <= ep; ii++ )
		b->keyC[ii]--;
	    return;
	}

	verify->startPos = b->yomiC[p];
	verify->endPos = verify->startPos + 3;
	b->cursor = b->yomiC[p] + 1;
	verify->text->wcs_ptr[0] = wc;
	verify->text->wcs_ptr[1] = '\0';
	verify->text->length = 1;
	replace_yomi( b, verify->text->wcs_ptr );
	b->conversion_flag[p] = True;

	b->point++;
	b->end_point++;
    }
    if( b->conversion_style & (XIMPreeditPosition | XIMPreeditArea) ) {
	if( b->state & EDIT )
	    verify->text->mode = XmwHIGHLIGHT_SELECTED;
	else
	    verify->text->mode = XmwHIGHLIGHT_SECONDARY_SELECTED;
    }
}


static void
#if NeedFunctionPrototypes
zenkaku_kana( Buffer *b, XmwTextVerifyPtr verify )
#else /* NeedFunctionPrototypes */
zenkaku_kana( b, verify )
    Buffer		*b;
    XmwTextVerifyPtr	verify;
#endif /* NeedFunctionPrototypes */
{
    unsigned char		c;
    unsigned short		zenkaku;
    char			mb[2];
    unsigned int		p;

    save_buffer( b );
    c = verify->text->ptr[0];
    if( (c == 0xde  ||  c == 0xdf)  &&  (p = b->point) != 0
	&&  b->conversion_flag[p-1]  &&  ( b->mode[p-1] & ZENKAKU) ) {
	if( c == 0xde  &&
	    !(zenkaku = isdakuon( b->key[b->keyC[p]-1], 0xde, b->mode[p-1] )) )
	    goto next;
	if( c == 0xdf  &&
	    !(zenkaku = ishandakuon( b->key[b->keyC[p]-1], 0xdf,
				     b->mode[p-1] )) )
	    goto next;

	p = --b->point;
	b->conversion_flag[p] = False;
	if( !insert_buffer( b, c, '\0', b->mode[p] ) ) {
	    verify->doit = False;
	    return;
	}

	verify->startPos = b->yomiC[p];
	verify->endPos = b->yomiC[p+1];
	b->cursor = b->yomiC[p] + 1;
	mb[0] = zenkaku >> 8;
	mb[1] = zenkaku & 0xff;
	mbtowc( verify->text->wcs_ptr, mb, MB_CUR_MAX );
	verify->text->wcs_ptr[1] = '\0';
	verify->text->length = 1;
	replace_yomi( b, verify->text->wcs_ptr );
	b->conversion_flag[p] = True;

	b->point++;
	if( b->conversion_style & (XIMPreeditPosition | XIMPreeditArea) ) {
	    if( b->state & EDIT )
		verify->text->mode = XmwHIGHLIGHT_SELECTED;
	    else
		verify->text->mode = XmwHIGHLIGHT_SECONDARY_SELECTED;
	}
	return;
    }
next:
    if( !insert_buffer( b, c, '\0', b->conversion_mode ) ) {
	verify->doit = False;
	return;
    }

    zenkaku = hantozen( c );
    if( b->conversion_mode != ZENKAKU_KATAKANA )
	zenkaku = jtohira( zenkaku );
    mb[0] = zenkaku >> 8;
    mb[1] = zenkaku & 0xff;
    mbtowc( verify->text->wcs_ptr, mb, MB_CUR_MAX );
    verify->text->wcs_ptr[1] = '\0';
    if( !add_yomi( b, verify->text->wcs_ptr ) ) {
	verify->doit = False;
	return;
    }

    b->conversion_flag[b->point++] = True;
    if( b->point == b->end_point )
	b->end_point++;
    b->cursor = verify->currInsert + 1;

    verify->text->length = 1;
    if( b->conversion_style & (XIMPreeditPosition | XIMPreeditArea) ) {
	if( b->state & EDIT )
	    verify->text->mode = XmwHIGHLIGHT_SELECTED;
	else
	    verify->text->mode = XmwHIGHLIGHT_SECONDARY_SELECTED;
    }
}


static void
#if NeedFunctionPrototypes
hankaku_kana( Buffer *b, XmwTextVerifyPtr verify )
#else /* NeedFunctionPrototypes */
hankaku_kana( b, verify )
    Buffer		*b;
    XmwTextVerifyPtr	verify;
#endif /* NeedFunctionPrototypes */
{
    unsigned char	c;

    save_buffer( b );
    c = verify->text->ptr[0];
    if( !insert_buffer( b, c, '\0', b->conversion_mode ) ) {
	verify->doit = False;
	return;
    }
    verify->text->wcs_ptr[0] = sjis2wc( (unsigned short)c );
    verify->text->wcs_ptr[1] = '\0';
    if( !add_yomi( b, verify->text->wcs_ptr ) ) {
	verify->doit = False;
	return;
    }

    b->conversion_flag[b->point++] = True;
    b->end_point++;
    b->cursor = verify->currInsert + 1;

    if( b->conversion_style & (XIMPreeditPosition | XIMPreeditArea) )
	verify->text->mode = XmwHIGHLIGHT_SECONDARY_SELECTED;
}


static void
#if NeedFunctionPrototypes
inkey_sub( Buffer *b, Widget w, XmwTextVerifyPtr verify )
#else /* NeedFunctionPrototypes */
inkey_sub( b, w, verify )
    Buffer		*b;
    Widget		w;
    XmwTextVerifyPtr	verify;
#endif /* NeedFunctionPrototypes */
{
    unsigned char	c;
    int			ii, result;
    int			len1, len2;
    wchar_t		yomi1[MAX_KANA], yomi2[MAX_KANA];
    Window		win;
    wchar_t		kanji[MAX_KANJI], *kk;

    if( verify->event != NULL  &&  verify->text->length != 0 ) {
	if( b->state & SERVER ) {
	    c = verify->text->ptr[0];
	    if( c >= 0x20  &&  c < 0x7f ) {
		hankaku_alpha( b, verify );
		if( !verify->doit )
		    load_buffer( b );
	    }
	    else
		verify->doit = False;
	    return;
	}
	if( b->state & ADD ) {
	    c = verify->text->ptr[0];
	    if( c >= 0x20  &&  c < 0x7f )
		kana( b, verify, ZENKAKU_HIRAGANA );
	    else if( c >= 0xa1  &&  c < 0xe0 )
		zenkaku_kana( b, verify );
	    else
		verify->doit = False;
	    if( verify->doit )
		verify->newInsert = b->cursor;
	    else
		load_buffer( b );
	    return;
	}
	if( b->state & SELECT ) {
	    if( !henkan_inkey( b, w, verify->text->ptr[0] ) )
		verify->doit = False;
	    return;
	}
	if( b->state & WRAP )
	    wrap_end( b, w, True );
	if( b->state & CONVERSION ) {
	    win = verify->event->xkey.subwindow;
	    register_buffer( b );

#ifdef USE_SJ3LIB
	    for( ii = 0; b->henkan->bunsetsu[ii+1] != BUNSETSU_END; ii++ ) {
		len1 = b->henkan->bun[ii].srclen;
		wsncpy( yomi1, b->henkan->bun[ii].srcstr, len1 );
		yomi1[len1] = '\0';
		if( b->henkan->bunsetsu_length[ii] > len1 ) {
		    len2 = b->henkan->bun[ii+1].srclen;
		    wsncpy( yomi2, b->henkan->bun[ii+1].srcstr, len2 );
		    yomi2[len2] = '\0';
		}
		else
		    yomi2[0] = '\0';
		if( b->henkan->bunsetsu[ii] & BUNSETSU_EDIT ) {
		    result = sj3_gakusyuu2_wcs( yomi1, yomi2,
						&(b->henkan->bun[ii+1].dcid) );
		    if( result == -1 ) {
			server_error( b, w, resources.died_error );
			return;
		    }
		    else if( result != 0  &&  (resources.debug & 0x8000) ) {
			server_error( b, w, resources.learning_error2 );
			return;
		    }
		}
	    }
#endif
	    for( ii = 0, kk = kanji; b->henkan->bunsetsu[ii] != BUNSETSU_END;
		 ii++ ) {
		if( b->henkan->bun[ii].deststr == NULL ) {
		    wsncpy( kk, b->henkan->bun[ii].srcstr,
			    b->henkan->bun[ii].srclen );
		    kk += b->henkan->bun[ii].srclen;
		}
		else {
		    wsncpy( kk, b->henkan->bun[ii].deststr,
			    b->henkan->bun[ii].destlen );
		    kk += b->henkan->bun[ii].destlen;
		}
	    }
	    *kk = '\0';

	    if( !SendCompoundText( verify->event->xany.display, win, kanji ) ) {
		verify->doit = False;
		return;
	    }
	    XtVaSetValues( b->preedit_widget, XmwNdisplayCaret, True, NULL );
	    buffer_initialize( b );

	    verify->currInsert = verify->newInsert = 0;
	    verify->startPos = verify->endPos = 0;
	}
	if( b->state & ( INTERMEDIATE | EDIT ) ) {
	    c = verify->text->ptr[0];
	    if( c >= 0x20  &&  c < 0x7f ) {
		switch( b->conversion_mode ) {
		  case ZENKAKU_HIRAGANA:
		  case HANKAKU_HIRAGANA:
		  case ZENKAKU_KATAKANA:
		  case HANKAKU_KATAKANA:
		    kana( b, verify, b->conversion_mode );
		    break;

		  case ZENKAKU_ALPHA:
		    zenkaku_alpha( b, verify );
		    break;

		  case HANKAKU_ALPHA:
		    hankaku_alpha( b, verify );
		    break;

		  default:
		    code( b, verify, b->conversion_mode );
		}
		if( verify->doit )
		    verify->newInsert = b->cursor;
		else
		    load_buffer( b );
	    }
	    else if( c >= 0xa1  &&  c < 0xe0 ) {
		switch( b->conversion_mode ) {
		  case ZENKAKU_HIRAGANA:
		  case ZENKAKU_KATAKANA:
		  case ZENKAKU_ALPHA:
		    zenkaku_kana( b, verify );
		    break;

		  case HANKAKU_HIRAGANA:
		  case HANKAKU_KATAKANA:
		  case HANKAKU_ALPHA:
		    hankaku_kana( b, verify );
		    break;

		  default:
		    verify->doit = False;
		    break;
		}
		if( verify->doit )
		    verify->newInsert = b->cursor;
		else
		    load_buffer( b );
	    }
	    else
		verify->doit = False;
	}
	else {
	    verify->doit = False;
	}
    }
}


void
#if NeedFunctionPrototypes
code_kettei( Buffer *b, wchar_t code )
#else /* NeedFunctionPrototypes */
code_kettei( b, code )
    Buffer	*b;
    wchar_t	code;
#endif /* NeedFunctionPrototypes */
{
    unsigned int	p;
    wchar_t		ww[2];
    int			ii, count;
    Window		window;

    save_buffer( b );
    if( !insert_buffer( b, ' ', '\0', b->conversion_mode | EXTEND ) ) {
	return;
    }
    
    p = b->point;

    ww[0] = code;
    ww[1] = '\0';
    if( b->mode_lock ) {
	window = XtWindow(b->preedit_widget);
	SendCompoundText( XtDisplay(b->preedit_widget), window, ww );
	buffer_initialize( b );
	return;
    }
    if( !replace_yomi( b, ww ) ) {
	return;
    }
    if( b->conversion_style & XIMPreeditNothing ) {
	count = b->yomiC[p];
	if( b->state & EDIT ) {
	    XmwTextOutInsertWcs( RootTextField, count, ww );
	    XmwTextOutSetCursorPosition( RootTextField, count + 1 );
	}
	else {
	    XmwTextOutInsertWcs( b->preedit_widget, count, ww );
	    XmwTextOutSetCursorPosition( b->preedit_widget, count + 1 );
	}
    }
    else if( b->conversion_style & (XIMPreeditPosition | XIMPreeditArea) ) {
	count = 0;
	if( b->state & EDIT ) {
	    for( ii = 0, count = 0; ii < b->henkan->point; ii++ )
		count += b->henkan->bun[ii].destlen;
	}
	count += b->yomiC[p];
	XmwPreeditInsertWcs( b->preedit_widget, count, ww );
	if( b->state & EDIT )
	    XmwPreeditSetHighlight( b->preedit_widget, count, count+1,
				    XmwHIGHLIGHT_SELECTED );
    }

    b->conversion_flag[p] = True;
    b->cursor += 1;
    b->point++;
    b->end_point++;
}


#define	IsKanaKey(keysym) (((unsigned)(keysym) >= XK_overline) && \
			    ((unsigned)(keysym) <= XK_semivoicedsound))

void
#if NeedFunctionPrototypes
inkey( Widget w, XEvent *event, String *params, Cardinal *n )
#else /* NeedFunctionPrototypes */
inkey( w, event, params, n )
    Widget	w;
    XEvent	*event;
    String	*params;
    Cardinal	*n;
#endif /* NeedFunctionPrototypes */
{
    Buffer		*b;
    int			count, ii;
    char		string[256];
    XmwTextVerifyStruct	verify;
    XmwTextBlockRec	text;
    KeySym		keysym;
    Window		window;

    switch( event->type ) {
      case KeyPress:
      case KeyRelease:
	b = set_buffer( w, event->type );
	if( b == NULL  ||  !check_action_args( params, *n, "inkey" ) )
	    return;
	if(  b->state & (ERROR | CODE | DELETE ) )
	    return;

	if( (count = XLookupString( (XKeyEvent *)event, string, 256,
				    &keysym, NULL )) == 0 ) {
	    if( !IsKanaKey(keysym) )
		return;
	    if( !iskana2(string[0] = keysym & 0xff) )
		return;
	    count = 1;
	}

	verify.event = event;
	verify.doit = True;
	verify.currInsert = 
	 verify.newInsert =
	  verify.startPos =
	    verify.endPos = b->cursor;
	verify.text = &text;
	text.ptr[0] = string[0];
	text.ptr[1] = '\0';
	text.wcs_ptr[0] = '\0';
	text.length = 1;
	text.mode = XmwHIGHLIGHT_NORMAL;
	inkey_sub( b, w, &verify );
	if( !verify.doit  ||  (b->state & CONVERSION) )
	    break;
	if( verify.text->wcs_ptr[0] == '\0' ) {
	    XBell( XtDisplay(w), 0 );
	    break;
	}

	if( b->state & SERVER ) {
	    XmwTextOutReplaceWcs( ServerText, verify.startPos, verify.endPos,
				  text.wcs_ptr );
	    XmwTextOutSetCursorPosition( ServerText, b->cursor + 1 );
	    if( b->cursor == 1 )
		XtVaSetValues( Server, XtNsensitive, True, NULL );
	}
	else if( b->state & ADD ) {
	    XmwTextOutReplaceWcs( TourokuText[1], verify.startPos,
				  verify.endPos, text.wcs_ptr );
	    XmwTextOutSetHighlight( TourokuText[1], verify.startPos,
				    verify.endPos, text.mode );
	    XmwTextOutSetCursorPosition( TourokuText[1], verify.newInsert );
	    if( b->cursor == 1 )
		XtVaSetValues( Touroku, XtNsensitive, True, NULL );
	}
	else if( b->state & EDIT ) {
	    if( b->conversion_style & XIMPreeditNothing ) {
		XmwTextOutReplaceWcs( RootTextField, verify.startPos,
				      verify.endPos, text.wcs_ptr );
		XmwTextOutSetHighlight( RootTextField, verify.currInsert,
					verify.newInsert, text.mode );
		XmwTextOutSetCursorPosition( RootTextField, verify.newInsert );
	    }
	    else {
		for( ii = 0, count = 0; ii < b->henkan->point; ii++ )
		    count += b->henkan->bun[ii].destlen;
		XmwPreeditReplaceWcs( b->preedit_widget,
				      verify.startPos + count,
				      verify.endPos + count, text.wcs_ptr );
		XmwPreeditSetHighlight( b->preedit_widget,
					verify.currInsert + count,
					verify.newInsert + count, text.mode );
	    }
	}
	else if( b->state & INTERMEDIATE ) {
	    if( b->mode_lock ) {
		for( ii = 0; ii < b->point; ii++ )
		    if( !b->conversion_flag[ii] )
			break;
		if( ii == b->point  &&  b->conversion_flag[ii]  &&
		    !b->str[b->keyC[ii]]  ) {
		    window = event->xkey.subwindow;
		    SendCompoundText( event->xany.display, window, b->yomi );
		    buffer_initialize( b );
		    return;
		}
	    }
	    if( b->conversion_style & XIMPreeditNothing ) {
		XmwTextOutReplaceWcs( b->preedit_widget, verify.startPos,
				      verify.endPos, text.wcs_ptr );
		XmwTextOutSetHighlight( b->preedit_widget, verify.currInsert,
					verify.newInsert, text.mode );
		XmwTextOutSetCursorPosition( b->preedit_widget,
					     verify.newInsert );
	    }
	    else {
		XmwPreeditReplaceWcs( b->preedit_widget, verify.startPos,
				      verify.endPos, text.wcs_ptr );
		XmwPreeditSetHighlight( b->preedit_widget, verify.startPos,
					verify.endPos, text.mode );
	    }
	}
    }
}

