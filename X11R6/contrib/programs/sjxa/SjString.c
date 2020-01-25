/* $SonyId: SjString.c,v 1.2 1994/06/03 06:33:04 makoto Exp $ */
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
#include	"sjxa.h"
#include	"SjString.h"

extern LcCtype	user_locale;

static struct {
    unsigned short	hiragana;
    unsigned short	katakana;
} Ktable[2][64] = {
{	/* EUC */
    { 0xa1a3, 0xa1a5 }, { 0xa1d6, 0xa1d8 }, { 0xa1d7, 0xa1d9 },
    { 0xa1a2, 0xa1a4 }, { 0xa1a6, 0xa1a6 },
    { 0xa4f2, 0xa5f2 }, { 0xa4a1, 0xa5a1 }, { 0xa4a3, 0xa5a3 },
    { 0xa4a5, 0xa5a5 }, { 0xa4a7, 0xa5a7 }, { 0xa4a9, 0xa5a9 },
    { 0xa4e3, 0xa5e3 }, { 0xa4e5, 0xa5e5 }, { 0xa4e7, 0xa5e7 },
    { 0xa4c3, 0xa5c3 },
    { 0xa1bc, 0xa1bc }, { 0xa4a2, 0xa5a2 }, { 0xa4a4, 0xa5a4 },
    { 0xa4a6, 0xa5a6 }, { 0xa4a8, 0xa5a8 }, { 0xa4aa, 0xa5aa },
    { 0xa4ab, 0xa5ab }, { 0xa4ad, 0xa5ad }, { 0xa4af, 0xa5af },
    { 0xa4b1, 0xa5b1 }, { 0xa4b3, 0xa5b3 }, { 0xa4b5, 0xa5b5 },
    { 0xa4b7, 0xa5b7 }, { 0xa4b9, 0xa5b9 }, { 0xa4bb, 0xa5bb },
    { 0xa4bd, 0xa5bd },
    { 0xa4bf, 0xa5bf }, { 0xa4c1, 0xa5c1 }, { 0xa4c4, 0xa5c4 },
    { 0xa4c6, 0xa5c6 }, { 0xa4c8, 0xa5c8 }, { 0xa4ca, 0xa5ca },
    { 0xa4cb, 0xa5cb }, { 0xa4cc, 0xa5cc }, { 0xa4cd, 0xa5cd },
    { 0xa4ce, 0xa5ce }, { 0xa4cf, 0xa5cf }, { 0xa4d2, 0xa5d2 },
    { 0xa4d5, 0xa5d5 }, { 0xa4d8, 0xa5d8 }, { 0xa4db, 0xa5db },
    { 0xa4de, 0xa5de },
    { 0xa4df, 0xa5df }, { 0xa4e0, 0xa5e0 }, { 0xa4e1, 0xa5e1 },
    { 0xa4e2, 0xa5e2 }, { 0xa4e4, 0xa5e4 }, { 0xa4e6, 0xa5e6 },
    { 0xa4e8, 0xa5e8 }, { 0xa4e9, 0xa5e9 }, { 0xa4ea, 0xa5ea },
    { 0xa4eb, 0xa5eb }, { 0xa4ec, 0xa5ec }, { 0xa4ed, 0xa5ed },
    { 0xa4ef, 0xa5ef }, { 0xa4f3, 0xa5f3 }, { 0xa1ab, 0xa1ab },
    { 0xa1ac, 0xa1ac },
    { 0x0000, 0x0000 }
},
{	/* SHIFT JIS */
    { 0x8142, 0x8144 }, { 0x8175, 0x8177 }, { 0x8176, 0x8178 },
    { 0x8141, 0x8143 }, { 0x8145, 0x8145 },
    { 0x82f0, 0x8392 }, { 0x829f, 0x8340 }, { 0x82a1, 0x8342 },
    { 0x82a3, 0x8344 }, { 0x82a5, 0x8346 }, { 0x82a7, 0x8348 },
    { 0x82e1, 0x8383 }, { 0x82e3, 0x8385 }, { 0x82e5, 0x8387 },
    { 0x82c1, 0x8362 },
    { 0x815b, 0x815b }, { 0x82a0, 0x8341 }, { 0x82a2, 0x8343 },
    { 0x82a4, 0x8345 }, { 0x82a6, 0x8347 }, { 0x82a8, 0x8349 },
    { 0x82a9, 0x834a }, { 0x82ab, 0x834c }, { 0x82ad, 0x834e },
    { 0x82af, 0x8350 }, { 0x82b1, 0x8352 }, { 0x82b3, 0x8354 },
    { 0x82b5, 0x8356 }, { 0x82b7, 0x8358 }, { 0x82b9, 0x835a },
    { 0x82bb, 0x835c },
    { 0x82bd, 0x835e }, { 0x82bf, 0x8360 }, { 0x82c2, 0x8363 },
    { 0x82c4, 0x8365 }, { 0x82c6, 0x8367 }, { 0x82c8, 0x8369 },
    { 0x82c9, 0x836a }, { 0x82ca, 0x836b }, { 0x82cb, 0x836c },
    { 0x82cc, 0x836d }, { 0x82cd, 0x836e }, { 0x82d0, 0x8371 },
    { 0x82d3, 0x8374 }, { 0x82d6, 0x8377 }, { 0x82d9, 0x837a },
    { 0x82dc, 0x837d },
    { 0x82dd, 0x837e }, { 0x82de, 0x8380 }, { 0x82df, 0x8381 },
    { 0x82e0, 0x8382 }, { 0x82e2, 0x8384 }, { 0x82e4, 0x8386 },
    { 0x82e6, 0x8388 }, { 0x82e7, 0x8389 }, { 0x82e8, 0x838a },
    { 0x82e9, 0x838b }, { 0x82ea, 0x838c }, { 0x82eb, 0x838d },
    { 0x82ed, 0x838f }, { 0x82f1, 0x8393 }, { 0x814a, 0x814a },
    { 0x814b, 0x814b }}};

static struct {
    unsigned char	kana;
    unsigned short	Kdakuon_euc, Kdakuon_sjis;
    unsigned short	Hdakuon_euc, Hdakuon_sjis;
    unsigned short	Khandakuon_euc, Khandakuon_sjis;
    unsigned short	Hhandakuon_euc, Hhandakuon_sjis;
} table[] = {
    { 0xca, 0xa5d0, 0x836f, 0xa4d0, 0x82ce, 0xa5d1, 0x8370, 0xa4d1, 0x82cf },
    { 0xcb, 0xa5d3, 0x8372, 0xa4d3, 0x82d1, 0xa5d4, 0x8373, 0xa4d4, 0x82d2 },
    { 0xcc, 0xa5d6, 0x8375, 0xa4d6, 0x82d4, 0xa5d7, 0x8376, 0xa4d7, 0x82d5 },
    { 0xcd, 0xa5d9, 0x8378, 0xa4d9, 0x82d7, 0xa5da, 0x8379, 0xa4da, 0x82d8 },
    { 0xce, 0xa5dc, 0x837b, 0xa4dc, 0x82da, 0xa5dd, 0x837c, 0xa4dd, 0x82db },
    { 0xb6, 0xa5ac, 0x834b, 0xa4ac, 0x82aa, 0x0000, 0x0000, 0x0000, 0x0000 },
    { 0xb7, 0xa5ae, 0x834d, 0xa4ae, 0x82ac, 0x0000, 0x0000, 0x0000, 0x0000 },
    { 0xb8, 0xa5b0, 0x834f, 0xa4b0, 0x82ae, 0x0000, 0x0000, 0x0000, 0x0000 },
    { 0xb9, 0xa5b2, 0x8351, 0xa4b2, 0x82b0, 0x0000, 0x0000, 0x0000, 0x0000 },
    { 0xba, 0xa5b4, 0x8353, 0xa4b4, 0x82b2, 0x0000, 0x0000, 0x0000, 0x0000 },
    { 0xbb, 0xa5b6, 0x8355, 0xa4b6, 0x82b4, 0x0000, 0x0000, 0x0000, 0x0000 },
    { 0xbc, 0xa5b8, 0x8357, 0xa4b8, 0x82b6, 0x0000, 0x0000, 0x0000, 0x0000 },
    { 0xbd, 0xa5ba, 0x8359, 0xa4ba, 0x82b8, 0x0000, 0x0000, 0x0000, 0x0000 },
    { 0xbe, 0xa5bc, 0x835b, 0xa4bc, 0x82ba, 0x0000, 0x0000, 0x0000, 0x0000 },
    { 0xbf, 0xa5be, 0x835d, 0xa4be, 0x82bc, 0x0000, 0x0000, 0x0000, 0x0000 },
    { 0xc0, 0xa5c0, 0x835f, 0xa4c0, 0x82be, 0x0000, 0x0000, 0x0000, 0x0000 },
    { 0xc1, 0xa5c2, 0x8361, 0xa4c2, 0x82c0, 0x0000, 0x0000, 0x0000, 0x0000 },
    { 0xc2, 0xa5c5, 0x8364, 0xa4c5, 0x82c3, 0x0000, 0x0000, 0x0000, 0x0000 },
    { 0xc3, 0xa5c7, 0x8366, 0xa4c7, 0x82c5, 0x0000, 0x0000, 0x0000, 0x0000 },
    { 0xc4, 0xa5c9, 0x8368, 0xa4c9, 0x82c7, 0x0000, 0x0000, 0x0000, 0x0000 },
    { 0xb3, 0xa5f4, 0x8394, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 },
    { 0x00, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 }};


#ifndef HAS_JLS_FUNC
unsigned short
#if NeedFunctionPrototypes
sjis2euc( unsigned short code )
#else
sjis2euc( code )
    unsigned short	code;
#endif
{
    int		byte1, byte2;

    byte1 = code / 0x100;
    byte2 = code & 0xff;

    if( byte1 <= 0x80  ||  byte1 >= 0xf0  ||
	( byte1 >= 0xa0  &&  byte1 <= 0xdf )  ||
	byte2 <= 0x3f  ||  byte2 >= 0xfe  ||  byte2 == 0x7f ) {
	return( 0xa1a1 );
    }
    byte1 -= (byte1 >= 0xa0) ? 0xc1 : 0x81;
    if( byte2 >= 0x9f )
	code = ( ((byte1 << 1) + 0xa2) << 8 ) | ( byte2 - 0x7e + 0x80);
    else
	code = ( ((byte1 << 1) + 0xa1) << 8 ) |
	       ( byte2 - ((byte2 <= 0x7e) ? 0x1f : 0x20) + 0x80 );
    return( code );
}


int	iskanji( code )
int	code;
{
    switch( user_locale ) {
      case LC_CTYPE_SHIFTJIS:
	return( (code >= 0x81  &&  code <= 0x9f)  ||
		(code >= 0xe0  &&  code <= 0xfc) );
	break;

      case LC_CTYPE_EUC:
	return( code >= 0xa1  &&  code <= 0xfe );
	break;
    }
    return( 0 );
}


unsigned short
#if NeedFunctionPrototypes
jis2sjis( unsigned short code )
#else
jis2sjis( code )
    unsigned short	code;
#endif
{
    int		hi, low;
    int		nh, nl;

    hi = code / 0x100;
    low = code & 0xff;

    nh = ((hi - 0x21) >> 1) + 0x81;
    if( nh > 0x9f )
	nh += 0x40;
    if( hi & 1 ) {
	nl = low + 0x1f;
	if( low > 0x5f )
	    nl++;
    }
    else
	nl = low + 0x7e;

    return( (nh << 8) | nl );
}


unsigned short
#if NeedFunctionPrototypes
kuten2jis( unsigned short code )
#else
kuten2jis( code )
    unsigned short	code;
#endif
{
    int		hi, low;

    hi = code / 0x100 + 0x20;
    low = code % 0x100 + 0x20;

    return( (hi << 8) | low );
}


int	iskana( code )
int	code;
{
    switch( user_locale ) {
      case LC_CTYPE_SHIFTJIS:
	return( code >= 0xa1  &&  code <= 0xdf );
	break;

      case LC_CTYPE_EUC:
	return( code == SS2 );		/* SS2 */
	break;
    }
    return( 0 );
}


static int	jishira( code )
unsigned short	code;
{
    switch( user_locale ) {
      case LC_CTYPE_SHIFTJIS:
	return( code >= 0x829f  &&  code <= 0x82f1 );
	break;

      case LC_CTYPE_EUC:
	return( code >= 0xa4a1  &&  code <= 0xa4f3 );
	break;
    }
}


static int	jiskata( code )
unsigned short	code;
{
    switch( user_locale ) {
      case LC_CTYPE_SHIFTJIS:
	return( code >= 0x8340  &&  code <= 0x8396  &&  code != 0x837f );
	break;

      case LC_CTYPE_EUC:
	return( code >= 0xa5a1  &&  code <= 0xa5f6 );
	break;
    }
}


unsigned short
#if NeedFunctionPrototypes
jtohira( unsigned short code )
#else
jtohira( code )
    unsigned short	code;
#endif
{
    switch( user_locale ) {
      case LC_CTYPE_SHIFTJIS:
	if( code < 0x8340  ||  code > 0x8393  ||  code == 0x837f )
	    break;
	if( code > 0x837f )
	    return( code - 0xa2 );
	else
	    return( code - 0xa1 );
	break;

      case LC_CTYPE_EUC:
	if( code < 0xa5a1  ||  code > 0xa5f3 )
	    break;
	return( code - 0x0100 );
	break;
    }
    return( code );
}
#endif /* !HAS_JLS_FUNC */


size_t
#if NeedFunctionPrototypes
mbs2wcs( wchar_t *ws, const char *s, size_t n )
#else /* NeedFunctionPrototypes */
mbs2wcs( ws, s, n )
    wchar_t	*ws;
    char	*s;
    size_t	n;
#endif /* NeedFunctionPrototypes */
{
    int		ii = 0;
    int		len;
    wchar_t	wc;

    while( *s  &&  n-- > 0 ) {
	len = mbtowc( &wc, s, MB_CUR_MAX );
	if( len == -1 )
	    return( -1 );
	if( ws )
	    *(ws++) = wc;
	s += len;
	ii++;
    }
    if( ws )
	*ws = '\0';
    return( ii );
}


size_t
#if NeedFunctionPrototypes
wcs2mbs( char *s, const wchar_t *ws, size_t n )
#else /* NeedFunctionPrototypes */
wcs2mbs( s, ws, n )
    char	*s;
    wchar_t	*ws;
    size_t	n;
#endif /* NeedFunctionPrototypes */
{
    int		ii = 0;
    int		len;
    char	ss[10];

    while( *ws  &&  n-- > 0 ) {
	len = wctomb( ss, *ws );
	if( len == -1 ) {
#if defined(__sony_news)  &&  !defined(SVR4)
	    if( user_locale == LC_CTYPE_EUC  &&  (*ws & 0x8080) == 0x8000 ) {
		    ss[0] = SS3;
		    ss[1] = *ws >> 8;
		    ss[2] = (*ws & 0x7f) | 0x80;
		    len = 3;
	    }
	    else
#endif
	    return( -1 );
	}
	if( s ) {
	    strncpy( s, ss, len );
	    s += len;
	}
	ws++;
	ii += len;
    }
    if( s )
	*s = '\0';
    return( ii );
}


wchar_t	*
#if NeedFunctionPrototypes
wscat( wchar_t *s1, wchar_t *s2 )
#else /* NeedFunctionPrototypes */
wscat( s1, s2 )
    wchar_t	*s1, *s2;
#endif /* NeedFunctionPrototypes */
{
    wchar_t	*s3 = s1;

    while( *s1 )
	s1++;
    do {
	*s1 = *(s2++);
    } while( *(s1++) );

    return( s3 );
}


int
#if NeedFunctionPrototypes
wscmp( wchar_t *s1, wchar_t *s2 )
#else /* NeedFunctionPrototypes */
wscmp( s1, s2 )
    wchar_t	*s1, *s2;
#endif /* NeedFunctionPrototypes */
{
    while( *s1  &&  *s2  &&  *s1 == *s2 ) {
	s1++;
	s2++;
    }
    return( *s1 - *s2 );
}


int
#if NeedFunctionPrototypes
wsncmp( wchar_t *s1, wchar_t *s2, int n )
#else /* NeedFunctionPrototypes */
wsncmp( s1, s2, n )
    wchar_t	*s1, *s2;
    int		n;
#endif /* NeedFunctionPrototypes */
{
    while( *s1  &&  *s2  &&  *s1 == *s2  &&  n > 0 ) {
	s1++;
	s2++;
	n--;
    }
    if( n == 0 )
	return( 0 );
    return( *s1 - *s2 );
}


wchar_t	*
#if NeedFunctionPrototypes
wscpy( wchar_t *s1, wchar_t *s2 )
#else /* NeedFunctionPrototypes */
wscpy( s1, s2 )
    wchar_t	*s1, *s2;
#endif /* NeedFunctionPrototypes */
{
    wchar_t	*s3 = s1;

    while( *s1 = *s2 ) {
	s1++;
	s2++;
    }
    return( s3 );
}


wchar_t	*
#if NeedFunctionPrototypes
wsncpy( wchar_t *s1, wchar_t *s2, int n )
#else /* NeedFunctionPrototypes */
wsncpy( s1, s2, n )
    wchar_t	*s1, *s2;
    int		n;
#endif /* NeedFunctionPrototypes */
{
    wchar_t	*s3 = s1;

    while( n > 0  &&  (*s1 = *s2) ) {
	s1++;
	s2++;
	n--;
    }
    return( s3 );
}


int
#if NeedFunctionPrototypes
wslen( wchar_t *s )
#else /* NeedFunctionPrototypes */
wslen( s )
    wchar_t	*s;
#endif /* NeedFunctionPrototypes */
{
    int		n = 0;

    while( *(s++) )
	n++;
    return( n );
}


int
#if NeedFunctionPrototypes
wcswidth( wchar_t *ws )
#else /* NeedFunctionPrototypes */
wcswidth( ws )
    wchar_t	*ws;
#endif /* NeedFunctionPrototypes */
{
    int		w = 0;
    char	mb[4];

    while( *ws ) {
	switch( wctomb( mb, *(ws++) ) ) {
	  case 1:
	    w++;
	    break;

	  case 2:
	    if( iskana((unsigned char)mb[0]) )
		w++;
	    else
		w += 2;
	    break;

	  case 3:
	    w += 2;
	    break;
	}
    }
    return( w );
}


wchar_t
#if NeedFunctionPrototypes
kuten2wc( unsigned short code )
#else /* NeedFunctionPrototypes */
kuten2wc( code )
    unsigned short	code;
#endif /* NeedFunctionPrototypes */
{
    unsigned short	kanji;
    char		mb[4];
    wchar_t		wc;

    switch( user_locale ) {
      case LC_CTYPE_SHIFTJIS:
	kanji = kuten2sjis( code );
	break;
      case LC_CTYPE_EUC:
	kanji = kuten2euc( code );
	break;
    }
    mb[0] = kanji >> 8;
    mb[1] = kanji & 0xff;
    if( mbtowc( &wc, mb, MB_CUR_MAX ) == -1 )
	return( 0 );
    return( wc );
}


wchar_t
#if NeedFunctionPrototypes
sjis2wc( unsigned short code )
#else /* NeedFunctionPrototypes */
sjis2wc( code )
    unsigned short	code;
#endif /* NeedFunctionPrototypes */
{
    unsigned char	mb[2];
    wchar_t		wc;

    mb[0] = code >> 8;
    mb[1] = code & 0xff;

    if( mb[0] == 0x00  &&  isascii(mb[1]) )
	mb[0] = mb[1];
    else {
	switch( user_locale ) {
	  case LC_CTYPE_SHIFTJIS:
	    if( mb[0] == 0x00 )
		mb[0] = mb[1];
	    break;

	  case LC_CTYPE_EUC:
	    if( mb[0] == 0x00 )
		mb[0] = SS2;
	    else {
		code = sjis2euc( code );
		mb[0] = code >> 8;
		mb[1] = code & 0xff;
	    }
	    break;
	}
    }
    if( mbtowc( &wc, (char *)mb, MB_CUR_MAX ) == -1 )
	return( 0 );
    return( wc );
}


wchar_t
#if NeedFunctionPrototypes
euc2wc( unsigned short code )
#else /* NeedFunctionPrototypes */
euc2wc( code )
    unsigned short	code;
#endif /* NeedFunctionPrototypes */
{
    unsigned short	kanji;
    char		mb[2];
    wchar_t		wc;

    switch( user_locale ) {
      case LC_CTYPE_SHIFTJIS:
	kanji = euc2sjis( code );
	break;
      case LC_CTYPE_EUC:
	kanji = code;
	break;
    }
    mb[0] = kanji >> 8;
    mb[1] = kanji & 0xff;
    if( mbtowc( &wc, mb, MB_CUR_MAX ) == -1 )
	return( 0 );
    return( wc );
}


wchar_t
#if NeedFunctionPrototypes
jis2wc( unsigned short code )
#else /* NeedFunctionPrototypes */
jis2wc( code )
    unsigned short	code;
#endif /* NeedFunctionPrototypes */
{
    unsigned short	kanji;
    char		mb[2];
    wchar_t		wc;

    switch( user_locale ) {
      case LC_CTYPE_SHIFTJIS:
	kanji = jis2sjis( code );
	break;
      case LC_CTYPE_EUC:
	kanji = jis2euc( code );
	break;
    }
    mb[0] = kanji >> 8;
    mb[1] = kanji & 0xff;
    if( mbtowc( &wc, mb, MB_CUR_MAX ) == -1 )
	return( 0 );
    return( wc );
}


int
#if NeedFunctionPrototypes
wc2sjis( char *mb, wchar_t wc )
#else /* NeedFunctionPrototypes */
wc2sjis( mb, wc )
    char	*mb;
    wchar_t	wc;
#endif /* NeedFunctionPrototypes */
{
    int			len;
    unsigned short	code;

    if( (len = wctomb( mb, wc )) == -1  ||  len == 3 )
	return( 0 );

    switch( user_locale ) {
      case LC_CTYPE_SHIFTJIS:
	break;
      case LC_CTYPE_EUC:
	if( len == 2 ) {
	    if( iskana( (unsigned char)mb[0] ) ) {
		mb[0] = mb[1];
		len = 1;
	    }
	    else {
		code = euc2sjis( ((unsigned char)mb[0] << 8)  |
				 (unsigned char)mb[1] );
		mb[0] = code / 0x100;
		mb[1] = code % 0x100;
	    }
	}
	break;
    }
    return( len );
}


int
#if NeedFunctionPrototypes
sjiss2wcs( wchar_t *ws, unsigned char *s, int n )
#else /* NeedFunctionPrototypes */
sjiss2wcs( ws, s, n )
    wchar_t		*ws;
    unsigned char	*s;
    int			n;
#endif /* NeedFunctionPrototypes */
{
    int			ii = 0;
    int			len;
    wchar_t		wc;
    char		mb[10];
    unsigned short	kanji;

    while( *s  &&  n-- > 0 ) {
	switch( user_locale ) {
	  case LC_CTYPE_SHIFTJIS:
	    mb[0] = *s;
	    if( iskanji( *s ) ) {
		mb[1] = *(s+1);
		s += 2;
		n--;
	    }
	    else
		s++;
	    break;
	  case LC_CTYPE_EUC:
	    if( isprint( *s ) )
		mb[0] = *(s++);
	    else if( iskana2( *s ) ) {
		mb[0] = SS2;
		mb[1] = *(s++);
	    }
	    else {
		kanji = sjis2euc( (*s << 8) | *(s+1) );
		mb[0] = kanji / 0x100;
		mb[1] = kanji % 0x100;
		s += 2;
		n--;
	    }
	}
	len = mbtowc( &wc, mb, MB_CUR_MAX );
	if( len == -1 )
	    return( -1 );
	if( ws )
	    *(ws++) = wc;
	ii++;
    }
    if( ws )
	*ws = '\0';
    return( ii );
}


int
#if NeedFunctionPrototypes
wcs2sjiss( char mb[], wchar_t *ws, int count )
#else /* NeedFunctionPrototypes */
wcs2sjiss( mb, ws, count )
    char	mb[];
    wchar_t	*ws;
    int	count;
#endif /* NeedFunctionPrototypes */
{
    int		len, ii;
    char	ss[10];

    len = 0;
    while( *ws != '\0'  &&  count > 0 ) {
	ii = wc2sjis( ss, *ws );
	if( ii <= 0 )
	    return( -1 );
	if( mb ) {
	    strncpy( mb, ss, ii );
	    mb += ii;
	}
	len += ii;
	ws++;
	count--;
    }
    if( mb )
	*mb = '\0';
    return( len );
}


int
#if NeedFunctionPrototypes
eucs2wcs( wchar_t wcs[], char *mbs, int n )
#else /* NeedFunctionPrototypes */
eucs2wcs( wcs, mbs, n )
    wchar_t	wcs[];
    char	*mbs;
    int		n;
#endif /* NeedFunctionPrototypes */
{
    int			ii = 0;
    int			len;
    wchar_t		wc;
    char		mb[10];
    unsigned short	kanji;

    while( *mbs  &&  n > 0 ) {
	switch( user_locale ) {
	  case LC_CTYPE_SHIFTJIS:
	    if( (unsigned char)*mbs == SS3 ) {
		mbs += 3;
		n -= 3;
		continue;
	    }
	    else if( (unsigned char)*mbs == SS2 ) {
		mb[0] = *(mbs+1);
		mbs += 2;
		n -= 2;
	    }
	    else if( isalpha( (unsigned char)*mbs ) ) {
		mb[0] = *(mbs++);
		n--;
	    }
	    else {
		kanji = euc2sjis( ((unsigned char)*mbs << 8) |
				  (unsigned char)*(mbs+1) );
		mb[0] = kanji / 0x100;
		mb[1] = kanji % 0x100;
		mbs += 2;
		n -= 2;
	    }
	    break;
	  case LC_CTYPE_EUC:
	    if( (unsigned char)*mbs == SS3 ) {
		mb[0] = *(mbs++);
		mb[1] = *(mbs++);
		mb[2] = *(mbs++);
		n -= 3;
	    }
	    else if( isalpha( (unsigned char)*mbs ) ) {
		mb[0] = *(mbs++);
		n--;
	    }
	    else {
		mb[0] = *(mbs++);
		mb[1] = *(mbs++);
		n -= 2;
	    }
	    break;
	}
	len = mbtowc( &wc, mb, MB_CUR_MAX );
	if( len == -1 )
	    return( -1 );
	if( wcs )
	    *(wcs++) = wc;
	ii++;
    }
    if( wcs )
	*wcs = '\0';
    return( ii );
}


static wchar_t
#if NeedFunctionPrototypes
_tojhira( wchar_t wc )
#else /* NeedFunctionPrototypes */
_tojhira( wc )
wchar_t	wc;
#endif /* NeedFunctionPrototypes */
{
    unsigned char	mb[10];
    int			len;
    unsigned short	kanji;

    if( (len = wctomb( (char *)mb, wc )) == -1  ||  len != 2 )
	return( wc );
    switch( user_locale ) {
      case LC_CTYPE_SHIFTJIS:
	if( mb[0] != 0x83  ||  mb[1] >= 0x94 )
	    return( wc );
	kanji = ((mb[0] - 1) << 8) | (mb[1] + 0x5f);
	if( mb[1] >= 0x80 )
	    kanji--;
	break;

      case LC_CTYPE_EUC:
	if( mb[0] != 0xa5  ||  mb[1] >= 0xf4 )
	    return( wc );
	kanji = ((mb[0] - 1) << 8) | mb[1];
	break;
    }
    mb[0] = kanji >> 8;
    mb[1] = kanji & 0xff;
    mbtowc( &wc, (char *)mb, MB_CUR_MAX );
    return( wc );
}


static wchar_t
#if NeedFunctionPrototypes
_tojkata( wchar_t wc )
#else /* NeedFunctionPrototypes */
_tojkata( wc )
    wchar_t	wc;
#endif /* NeedFunctionPrototypes */
{
    unsigned char	mb[10];
    int			len;
    unsigned short	kanji;

    if( (len = wctomb( (char *)mb, wc )) == -1  ||  len != 2 )
	return( wc );
    switch( user_locale ) {
      case LC_CTYPE_SHIFTJIS:
	if( mb[0] != 0x82  ||  mb[1] < 0x9f )
	    return( wc );
	kanji = ((mb[0] + 1) << 8) | (mb[1] - 0x5f);
	if( mb[1] >= 0xde )
	    kanji++;
	break;

      case LC_CTYPE_EUC:
	if( mb[0] != 0xa4 )
	    return( wc );
	kanji = ((mb[0] + 1) << 8) | mb[1];
	break;
    }
    mb[0] = kanji >> 8;
    mb[1] = kanji & 0xff;
    mbtowc( &wc, (char *)mb, MB_CUR_MAX );
    return( wc );
}


char *
#if NeedFunctionPrototypes
Sjis2Euc( char *s )
#else
Sjis2Euc( s )
    char	*s;
#endif
{
    static char		euc[256];
    int			n = 0;
    unsigned char	*sjis = (unsigned char *)s;
    unsigned short	code;

    euc[0] = '\0';

    while( *sjis ) {
	if( issjis1(*sjis) ) {
	    code = sjis2euc( (*sjis << 8) | *(sjis+1) );
	    euc[n++] = code >> 8;
	    euc[n++] = code & 0xff;
	    sjis += 2;
	}
	else {
	    if( *sjis >= 0xa1  &&  *sjis <= 0xdf )	/* iskana */
		euc[n++] = SS2;			/* SS2 */
	    euc[n++] = *sjis;
	    sjis++;
	}
    }
    euc[n] = '\0';
    return( euc );
}


char *
#if NeedFunctionPrototypes
Euc2Sjis( char *s )
#else
Euc2Sjis( s )
    char	*s;
#endif
{
    static char		sjis[256];
    int			n = 0;
    unsigned char	*euc = (unsigned char *)s;
    unsigned short	code;

    sjis[0] = '\0';

    while( *euc ) {
	if( iseuc(*euc) ) {
	    code = euc2sjis( (*euc << 8) | *(euc+1) );
	    sjis[n++] = code >> 8;
	    sjis[n++] = code & 0xff;
	    euc += 2;
	}
	else {
	    if( *euc == SS2 )				/* iskana */
		euc++;
	    sjis[n++] = *euc;
	    euc++;
	}
    }
    sjis[n] = '\0';
    return( sjis );
}


unsigned short
#if NeedFunctionPrototypes
isdakuon( unsigned char ch, unsigned cl, ConversionMode mode )
#else /* NeedFunctionPrototypes */
isdakuon( ch, cl, mode )
    unsigned char	ch, cl;
    ConversionMode	mode;
#endif /* NeedFunctionPrototypes */
{
    int	    ii;

    if( cl == 0xde ) {
	for( ii = 0; table[ii].kana != 0x00; ii++ ) {
	    if( table[ii].kana == ch ) {
		switch( user_locale ) {
		  case LC_CTYPE_SHIFTJIS:
		    return( (mode == ZENKAKU_KATAKANA) ?
			     table[ii].Kdakuon_sjis : table[ii].Hdakuon_sjis );
		  case LC_CTYPE_EUC:
		    return( (mode == ZENKAKU_KATAKANA) ?
			     table[ii].Kdakuon_euc : table[ii].Hdakuon_euc );
		}
	    }
	}
    }
    return( 0 );
}


static unsigned short
#if NeedFunctionPrototypes
jisdakuon( unsigned short s )
#else /* NeedFunctionPrototypes */
jisdakuon( s )
    unsigned short	s;
#endif /* NeedFunctionPrototypes */
{
    int		    ii;

    if( jishira( s ) ) {
	switch( user_locale ) {
	  case LC_CTYPE_SHIFTJIS:
	    for( ii = 0; table[ii].Hdakuon_sjis != 0x0000; ii++ )
		if( table[ii].Hdakuon_sjis == s )
		    return( table[ii].kana );
	    break;
	  case LC_CTYPE_EUC:
	    for( ii = 0; table[ii].Hdakuon_euc != 0x0000; ii++ )
		if( table[ii].Hdakuon_euc == s )
		    return( (SS2 << 8) | table[ii].kana );
	    break;
	}
    }
    else if( jiskata( s ) ) {
	switch( user_locale ) {
	  case LC_CTYPE_SHIFTJIS:
	    for( ii = 0; table[ii].Kdakuon_sjis != 0x0000; ii++ )
		if( table[ii].Kdakuon_sjis == s )
		    return( table[ii].kana );
	    break;
	  case LC_CTYPE_EUC:
	    for( ii = 0; table[ii].Kdakuon_euc != 0x0000; ii++ )
		if( table[ii].Kdakuon_euc == s )
		    return( (SS2 << 8) | table[ii].kana );
	    break;
	}
    }

    return( 0 );
}


unsigned short
#if NeedFunctionPrototypes
ishandakuon( unsigned char ch, unsigned char cl, ConversionMode mode )
#else /* NeedFunctionPrototypes */
ishandakuon( ch, cl, mode )
    unsigned char	ch, cl;
    ConversionMode	mode;
#endif /* NeedFunctionPrototypes */
{
    int	    ii;

    if( cl == 0xdf ) {
	for( ii = 0; ii < 5; ii++ ) {
	    if( table[ii].kana == ch ) {
		switch( user_locale ) {
		  case LC_CTYPE_SHIFTJIS:
		    return( (mode == ZENKAKU_KATAKANA) ?
		       table[ii].Khandakuon_sjis : table[ii].Hhandakuon_sjis );
		  case LC_CTYPE_EUC:
		    return( (mode == ZENKAKU_KATAKANA) ?
		       table[ii].Khandakuon_euc : table[ii].Hhandakuon_euc );
		}
	    }
	}
    }
    return( False );
}


static unsigned short
#if NeedFunctionPrototypes
jishandakuon( unsigned short s )
#else /* NeedFunctionPrototypes */
jishandakuon( s )
    unsigned short	s;
#endif /* NeedFunctionPrototypes */
{
    int		    ii;

    if( jishira( s ) ) {
	switch( user_locale ) {
	  case LC_CTYPE_SHIFTJIS:
	    for( ii = 0; table[ii].Hhandakuon_sjis != 0x0000; ii++ )
		if( table[ii].Hhandakuon_sjis == s )
		    return( table[ii].kana );
	    break;
	  case LC_CTYPE_EUC:
	    for( ii = 0; table[ii].Hhandakuon_euc != 0x0000; ii++ )
		if( table[ii].Hhandakuon_euc == s )
		    return( (SS2 << 8) | table[ii].kana );
	    break;
	}
    }
    else if( jiskata( s ) ) {
	switch( user_locale ) {
	  case LC_CTYPE_SHIFTJIS:
	    for( ii = 0; table[ii].Khandakuon_sjis != 0x0000; ii++ )
		if( table[ii].Khandakuon_sjis == s )
		    return( table[ii].kana );
	    break;
	  case LC_CTYPE_EUC:
	    for( ii = 0; table[ii].Khandakuon_euc != 0x0000; ii++ )
		if( table[ii].Khandakuon_euc == s )
		    return( (SS2 << 8) | table[ii].kana );
	    break;
	}
    }

    return( 0 );
}


void
#if NeedFunctionPrototypes
hankaku2zenkaku( wchar_t *in, wchar_t out[], ConversionMode mode )
#else /* NeedFunctionPrototypes */
hankaku2zenkaku( in, out, mode )
    wchar_t		*in, out[];
    ConversionMode	mode;
#endif /* NeedFunctionPrototypes */
{
    wchar_t		wc;
    unsigned char	mb1[10], mb2[10];
    unsigned char	c1, c2;
    unsigned short	zenkaku;
    int			len;

    while( (wc = *(in++)) != '\0' ) {
	len = wctomb( (char *)mb1, wc );
	if( !iskana( mb1[0] ) ) {
	    if( len == 1 ) {
		zenkaku = hantozen( mb1[0] );
		mb2[0] = zenkaku >> 8;
		mb2[1] = zenkaku & 0xff;
		mbtowc( out, (char *)mb2, MB_CUR_MAX );
		out++;
	    }
	    else
		*(out++) = wc;
	    continue;
	}
	else if( *in == '\0'  ||  wctomb( (char *)mb2, *in ) == -1  ||
		 !iskana( mb2[0] ) ) {
	    if( user_locale == LC_CTYPE_EUC )
		zenkaku = hantozen( (mb1[0] << 8) | mb1[1] );
	    else
		zenkaku = hantozen( mb1[0] );
	    mb2[0] = zenkaku >> 8;
	    mb2[1] = zenkaku & 0xff;
	    mbtowc( out, (char *)mb2, MB_CUR_MAX );
	    if( mode & (ZENKAKU_HIRAGANA | ZENKAKU_ALPHA) )
		*out = _tojhira( *out );
	    out++;
	    continue;
	}

	if( user_locale == LC_CTYPE_EUC ) {
	    c1 = mb1[1];
	    c2 = mb2[1];
	}
	else {
	    c1 = mb1[0];
	    c2 = mb2[0];
	}

	if( zenkaku = isdakuon( c1, c2, mode ) )
	    in++;
	else if( zenkaku = ishandakuon( c1, c2, mode ) )
	    in++;
	else
	    zenkaku = Ktable[user_locale][c1-0xa1].hiragana;
	mb2[0] = zenkaku >> 8;
	mb2[1] = zenkaku & 0xff;
	mbtowc( out, (char *)mb2, MB_CUR_MAX );
	out++;
    }
    *out = '\0';
}


void
#if NeedFunctionPrototypes
ZKtoZH( wchar_t *in, wchar_t out[] )
#else /* NeedFunctionPrototypes */
ZKtoZH( in, out )
    wchar_t	*in, out[];
#endif /* NeedFunctionPrototypes */
{
    do {
	*(out++) = _tojhira( *in );
    } while( *in++ != '\0' );
    *out = '\0';
}


void
#if NeedFunctionPrototypes
ZHtoZK( wchar_t *in, wchar_t out[] )
#else /* NeedFunctionPrototypes */
ZHtoZK( in, out )
    wchar_t	*in, out[];
#endif /* NeedFunctionPrototypes */
{
    do {
	*(out++) = _tojkata( *in );
    } while( *in++ != '\0' );
    *out = '\0';
}


void
#if NeedFunctionPrototypes
ZHtoHK( wchar_t *in, wchar_t out[] )
#else /* NeedFunctionPrototypes */
ZHtoHK( in, out )
wchar_t	*in, out[];
#endif /* NeedFunctionPrototypes */
{
    wchar_t		wc;
    unsigned short	kanji, hankaku;
    int			ii;
    char		mb[10];
    int			n;

    while( (wc = *(in++)) != '\0' ) {
	n = wctomb( mb, wc );
	if( n == 2  &&  iskanji((unsigned char)mb[0]) ) {
	    if( hankaku = jisdakuon(kanji = ((unsigned char)mb[0] << 8) |
					     (unsigned char)mb[1]) ) {
		switch( user_locale ) {
		  case LC_CTYPE_SHIFTJIS:
		    mb[0] = hankaku;
		    mb[1] = 0xde;
		    mb[2] = '\0';
		    break;
		  case LC_CTYPE_EUC:
		    mb[0] = hankaku >> 8;
		    mb[1] = hankaku & 0xff;
		    mb[2] = SS2;
		    mb[3] = 0xde;
		    mb[4] = '\0';
		    break;
		}
		n = mbs2wcs( out, mb, 4 );
		out += n;
	    }
	    else if( hankaku = jishandakuon(kanji) ) {
		switch( user_locale ) {
		  case LC_CTYPE_SHIFTJIS:
		    mb[0] = hankaku;
		    mb[1] = 0xdf;
		    mb[2] = '\0';
		    break;
		  case LC_CTYPE_EUC:
		    mb[0] = hankaku >> 8;
		    mb[1] = hankaku & 0xff;
		    mb[2] = SS2;
		    mb[3] = 0xdf;
		    mb[4] = '\0';
		    break;
		}
		n = mbs2wcs( out, mb, 4 );
		out += n;
	    }
	    else {
		for( ii = 0; Ktable[user_locale][ii].hiragana != 0x0000;
		     ii++ ) {
		    if( Ktable[user_locale][ii].hiragana == kanji  ||
			Ktable[user_locale][ii].katakana == kanji ) {
			switch( user_locale ) {
			  case LC_CTYPE_EUC:
			    mb[0] = SS2;
			    mb[1] = 0xa1 + ii;
			    break;
			  case LC_CTYPE_SHIFTJIS:
			    mb[0] = 0xa1 + ii;
			    break;
			}
			mbtowc( out, mb, MB_CUR_MAX );
			out++;
			break;
		    }
		}
		if( Ktable[user_locale][ii].hiragana == 0x0000 ) {
		    if( (hankaku = zentohan(kanji)) != kanji ) {
			if( iskana( hankaku )  &&
			    user_locale == LC_CTYPE_EUC ) {
			    mb[0] = SS2;
			    mb[1] = hankaku;
			}
			else
			    mb[0] = hankaku;
			mbtowc( out, mb, MB_CUR_MAX );
			out++;
		    }
		    else {
			*(out++) = wc;
		    }
		}
	    }
	}
	else
	    *(out++) = wc;
    }
    *out = '\0';
}


int
#if NeedFunctionPrototypes
StrnCaseCmp( char *s1, char *s2, int n )
#else
StrnCaseCmp( s1, s2, n )
    char	*s1, *s2;
    int		n;
#endif
{
    while( --n >= 0  &&  tolower(*s1) == tolower(*s2++) )
	if( *s1++ == '\0' )
	    return( 0 );
    return( n < 0 ? 0 : tolower(*s1) - tolower(*--s2) );
}
