/* $SonyId: Sj3wc.c,v 1.1 1994/05/30 13:03:58 makoto Exp $ */
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
#include	"sjxa.h"
#include	"SjString.h"

int
#if NeedFunctionPrototypes
sj3_getkan_wcs( wchar_t *yomi, struct bunsetu_wcs *bun, wchar_t *knjbuf,
		int knjsiz )
#else /* NeedFunctionPrototypes */
sj3_getkan_wcs( yomi, bun, knjbuf, knjsiz )
    wchar_t		*yomi;
    struct bunsetu_wcs	*bun;
    wchar_t		*knjbuf;
    int			 knjsiz;
#endif /* NeedFunctionPrototypes */
{
    unsigned char	string[MAX_KANA*3], ss[MAX_KANA*3];
    unsigned char	kanji[MAX_KANJI*3];
    struct bunsetu	bun_mb[256];
    int			n;
    int			result;

#ifdef USE_SJ3LIB
#if defined(SVR4)  ||  !defined(__sony_news)
    wcs2mbs( (char *)string, yomi, MAX_KANA*3 );
    result = sj3_getkan_mb( string, bun_mb, kanji, MAX_KANJI*3 );
    n = 0;
    bun[0].srcstr = yomi;
    while( result > 0 ) {
	strcpy( (char *)ss, (char *)bun_mb[n].srcstr );
	ss[bun_mb[n].srclen] = '\0';
	bun[n].srclen = mbs2wcs( NULL, (char *)ss, MAX_KANA*3 );
	bun[n+1].srcstr = bun[n].srcstr + bun[n].srclen;
	strcpy( (char *)ss, (char *)bun_mb[n].deststr );
	ss[bun_mb[n].destlen] = '\0';
	bun[n].destlen = mbs2wcs( knjbuf, (char *)ss, knjsiz );
	bun[n].deststr = knjbuf;
	knjbuf += bun[n].destlen;
	if( (knjsiz -= bun[n].destlen) < 0 ) {
	    bun[0].srclen = wslen( yomi );
	    bun[0].destlen = 0;
	    bun[0].deststr = NULL;
	    n = 1;
	    break;
	}
	bun[n].dcid = bun_mb[n].dcid;
	n++;
	result--;
    }
    bun[n].srclen = bun[n].destlen = 0;
    bun[n].srcstr = bun[n].deststr = NULL;

    return( n );
#else
    int		len, count;
    Boolean	convert = False;

    n = count = 0;
    bun[0].srcstr = yomi;
    while( *yomi != NULL ) {
	len = wc2sjis( ss, *yomi );
	if( len <= 0 ) {
	    if( n > 0 ) {
		string[n] = NULL;
		result = sj3_getkan( string, bun_mb, kanji, MAX_KANJI*3 );
		n = 0;
		while( result > 0 ) {
		    bun[count].srclen = sjiss2wcs( NULL, bun_mb[n].srcstr,
						    bun_mb[n].srclen );
		    bun[count+1].srcstr = bun[count].srcstr + bun[count].srclen;
		    bun[count].destlen = sjiss2wcs( knjbuf, bun_mb[n].deststr,
						    bun_mb[n].destlen );
		    bun[count].deststr = knjbuf;
		    knjbuf += bun[count].destlen;
		    count++;
		    n++;
		    result--;
		}
	    }
	    bun[count].srclen = bun[count].destlen = 1;
	    bun[count].srcstr = yomi;
	    *(knjbuf++) = *yomi;
	    *knjbuf = NULL;
	    count++;
	    yomi++;
	    n = 0;
	    convert = True;
	}
	else {
	    strncpy( (char *)&string[n], (char *)ss, len );
	    n += len;
	    yomi++;
	    convert = False;
	}
    }
    if( !convert ) {
	string[n] = NULL;
	result = sj3_getkan( string, bun_mb, kanji, MAX_KANJI*3 );
	n = 0;
	while( result > 0 ) {
	    bun[count].srclen = sjiss2wcs( NULL, bun_mb[n].srcstr,
					   bun_mb[n].srclen );
	    bun[count+1].srcstr = bun[count].srcstr + bun[count].srclen;
	    bun[count].destlen = sjiss2wcs( knjbuf, bun_mb[n].deststr,
					    bun_mb[n].destlen );
	    bun[count].deststr = knjbuf;
	    knjbuf += bun[count].destlen;
	    bun[count].dcid = bun_mb[n].dcid;
	    n++;
	    count++;
	    result--;
	}
    }
    bun[count].srclen = bun[count].destlen = 0;
    bun[count].srcstr = bun[count].deststr = NULL;

    return( count );
#endif
#else /* !USE_SJ3LIB */
    bun[0].srclen = wslen( yomi );
    bun[0].destlen = 0;
    bun[0].deststr = NULL;
    bun[1].srclen = bun[1].destlen = 0;
    bun[1].srcstr = bun[1].deststr = NULL;
    return( 1 );
#endif
}


void
#if NeedFunctionPrototypes
sj3_freedouon_wcs( struct douon_wcs *dou_wc )
#else /* NeedFunctionPrototypes */
sj3_freedouon_wcs( dou_wc )
    struct douon_wcs *dou_wc;
#endif /* NeedFunctionPrototypes */
{
    int		n;

    if( dou_wc ) {
	for( n = 0; dou_wc[n].ddata != NULL; n++ )
	    free( dou_wc[n].ddata );
	free( dou_wc );
    }
}


static Boolean
#if NeedFunctionPrototypes
add_kana( wchar_t *yomi, struct douon_wcs **dou )
#else /* NeedFunctionPrototypes */
add_kana( yomi, dou )
    wchar_t		*yomi;
    struct douon_wcs	**dou;
#endif /* NeedFunctionPrototypes */
{
    wchar_t		kana[MAX_KANA];
    struct douon_wcs	*d;
    int			n = 0, ii;

    ZHtoZK( yomi, kana );
    d = *dou;
    while( d->dlen ) {
	if( wscmp( d->ddata, kana ) == 0 )
	    return( False );
	n++;
	d++;
    }
    *dou = (struct douon_wcs *)realloc( *dou,
					(n+2) * sizeof(struct douon_wcs) );
    (*dou)[n].dlen = wslen( kana );
    (*dou)[n].width = wcswidth( kana );
    (*dou)[n].ddata = (wchar_t *)malloc( sizeof(wchar_t) * ((*dou)[n].dlen+1) );
    wscpy( (*dou)[n].ddata, kana );
    (*dou)[n+1].dlen = 0;
    (*dou)[n+1].width = 0;
    (*dou)[n+1].ddata = NULL;
    for( ii = 0; ii < SJ3_WORD_ID_SIZE; ii++ ) {
	(*dou)[n].dcid.dummy[ii] = 0;
	(*dou)[n+1].dcid.dummy[ii] = 0;
    }

    return( True );
}


int
#if NeedFunctionPrototypes
sj3_getdouon_wcs( wchar_t *yomi, struct douon_wcs **dou_wc )
#else /* NeedFunctionPrototypes */
sj3_getdouon_wcs( yomi, dou_wc )
    wchar_t	 	*yomi;
    struct douon_wcs	**dou_wc;
#endif /* NeedFunctionPrototypes */
{
    unsigned char	 string[MAX_KANA];
    int			 len, n, count;
    int			 result;
    struct douon	*dou;
    wchar_t		 kanji[MAX_KANJI];
#if !defined(SVR4)  &&  defined(__sony_news)
    unsigned char	 ss[256];
    wchar_t		*wp;
#endif

    sj3_freedouon_wcs( *dou_wc );
    *dou_wc = NULL;

#ifdef USE_SJ3LIB
    n = 0;
#if defined(SVR4)  ||  !defined(__sony_news)
    len = wcs2mbs( (char *)string, yomi, MAX_KANA );
    if( len < 0 )
	return( 0 );
    count = sj3_douoncnt_mb( string );
#else
    wp = yomi;
    while( *wp != NULL ) {
	len = wc2sjis( ss, *wp );
	if( len < 0 )
	    return( 0 );
	else if( len == 0 ) {
	    if( (*dou_wc = (struct douon_wcs *)calloc( 2,
					sizeof(struct douon_wcs) )) == NULL )
		done();
	    (*dou_wc)[0].dlen = wslen( yomi );
	    (*dou_wc)[0].width = wcswidth( yomi );
	    (*dou_wc)[0].ddata = (wchar_t *)malloc( sizeof(wchar_t) *
						    ((*dou_wc)[0].dlen+1) );
	    if( (*dou_wc)[0].ddata == NULL )
		done();
	    wscpy( (*dou_wc)[0].ddata, yomi );
	    if( add_kana( yomi, dou_wc ) )
		return( 2 );
	    else
		return( 1 );
	}
	strncpy( (char *)&string[n], (char *)ss, len );
	n += len;
	wp++;
    }
    string[n] = NULL;
    count = sj3_douoncnt( string );
#endif
    if( count < 0 )
	return( count );
    if( count == 0 ) {
	if( (*dou_wc = (struct douon_wcs *)calloc( 2,
					sizeof(struct douon_wcs) )) == NULL )
	    done();
	(*dou_wc)[0].dlen = wslen( yomi );
	(*dou_wc)[0].width = wcswidth( yomi );
	(*dou_wc)[0].ddata = (wchar_t *)malloc( sizeof(wchar_t) *
						((*dou_wc)[0].dlen+1) );
	if( (*dou_wc)[0].ddata == NULL )
	    done();
	wscpy( (*dou_wc)[0].ddata, yomi );
	if( add_kana( yomi, dou_wc ) )
	    return( 2 );
	else
	    return( 1 );
    }
    if( (dou = (struct douon *)malloc( sizeof(struct douon) * count )) == NULL )
	done();
    if( (*dou_wc = (struct douon_wcs *)calloc( count + 1,
					sizeof(struct douon_wcs) )) == NULL ) {
	free( dou );
	done();
    }
#if defined(SVR4)  ||  !defined(__sony_news)
    result = sj3_getdouon_mb( string, dou );
#else
    result = sj3_getdouon( string, dou );
#endif
    if( result < 0 ) {
	free( dou );
	free( *dou_wc );
	return( result );
    }
    for( n = 0; n < count; n++ ) {
#if defined(SVR4)  ||  !defined(__sony_news)
	strcpy( (char *)string, (char *)dou[n].ddata );
	string[dou[n].dlen] = '\0';
	(*dou_wc)[n].dlen = mbs2wcs( kanji, (char *)string, MAX_KANA );
#else
	(*dou_wc)[n].dlen = sjiss2wcs( kanji, dou[n].ddata, dou[n].dlen );
#endif
	(*dou_wc)[n].width = dou[n].dlen;
	(*dou_wc)[n].dcid = dou[n].dcid;
	(*dou_wc)[n].ddata = (wchar_t *)malloc( sizeof(wchar_t) *
						((*dou_wc)[n].dlen+1) );
	if( (*dou_wc)[n].ddata == NULL ) {
	    for( n--; n >= 0; n-- )
		free( (*dou_wc)[n].ddata );
	    free( *dou_wc );
	    done();
	}
	wscpy( (*dou_wc)[n].ddata, kanji );
    }
    if( add_kana( yomi, dou_wc ) )
	return( count + 1 );
    else
	return( count );
#else /* USE_SJ3LIB */
    if( (*dou_wc = (struct douon_wcs *)calloc( 2, sizeof(struct douon_wcs) ))
      == NULL )
	done();
    (*dou_wc)[0].dlen = wslen( yomi );
    (*dou_wc)[0].width = wcswidth( yomi );
    (*dou_wc)[0].ddata = (wchar_t *)malloc( sizeof(wchar_t) *
					    ((*dou_wc)[0].dlen+1) );
    if( (*dou_wc)[0].ddata == NULL )
	done();
    wscpy( (*dou_wc)[0].ddata, yomi );
    if( add_kana( yomi, dou_wc ) )
	return( 2 );
    else
	return( 1 );
#endif
}


int
#if NeedFunctionPrototypes
sj3_gakusyuu2_wcs( wchar_t *yomi1, wchar_t *yomi2, struct studyrec *id )
#else /* NeedFunctionPrototypes */
sj3_gakusyuu2_wcs( yomi1, yomi2, id )
    wchar_t		*yomi1, *yomi2;
    struct studyrec	*id;
#endif /* NeedFunctionPrototypes */
{
    unsigned char	string1[MAX_KANA], string2[MAX_KANA];

#ifdef USE_SJ3LIB
#if defined(SVR4)  ||  !defined(__sony_news)
    if( wcs2mbs( (char *)string1, yomi1, MAX_KANA ) < 0 )
	return( 0 );
    if( wcs2mbs( (char *)string2, yomi2, MAX_KANA ) < 0 )
	return( 0 );
    return( sj3_gakusyuu2_mb( string1, string2, id ) );
#else
    int			len, n = 0;
    unsigned char	ss[256];

    while( *yomi1 != NULL ) {
	len = wc2sjis( ss, *yomi1 );
	if( len <= 0 )
	    return( 0 );
	strncpy( (char *)&string1[n], (char *)ss, len );
	n += len;
	yomi1++;
    }
    string1[n] = NULL;

    n = 0;
    while( *yomi2 != NULL ) {
	len = wc2sjis( ss, *yomi2 );
	if( len <= 0 )
	    return( 0 );
	strncpy( (char *)&string2[n], (char *)ss, len );
	n += len;
	yomi2++;
    }
    string2[n] = NULL;

    return( sj3_gakusyuu2( string1, string2, id ) );
#endif
#else /* !USE_SJ3LIB */
    return( 0 );
#endif
}


int
#if NeedFunctionPrototypes
sj3_touroku_wcs( wchar_t *yomi, wchar_t *kanji, int code )
#else /* NeedFunctionPrototypes */
sj3_touroku_wcs( yomi, kanji, code )
    wchar_t	*yomi, *kanji;
    int	code;
#endif /* NeedFunctionPrototypes */
{
    unsigned char	string1[MAX_KANA], string2[MAX_KANA];
    int			result;

#ifdef USE_SJ3LIB
#if defined(SVR4)  ||  !defined(__sony_news)
    if( wcs2mbs( (char *)string1, yomi, wslen(yomi) ) == -1 )
	return( SJ3_BAD_YOMI_STR );

    if( wcs2mbs( (char *)string2, kanji, wslen(kanji) ) == -1 )
	return( SJ3_BAD_KANJI_STR );

    sj3_unlockserv();
    result = sj3_touroku_mb( string1, string2, code );
#else
    if( wcs2sjiss( string1, yomi, wslen(yomi) ) == -1 )
	return( SJ3_BAD_YOMI_STR );

    if( wcs2sjiss( string2, kanji, wslen(kanji) ) == -1 )
	return( SJ3_BAD_KANJI_STR );

    sj3_unlockserv();
    result = sj3_touroku( string1, string2, code );
#endif
    sj3_lockserv();
    return( result );
#else /* !USE_SJ3LIB */
    return( 0 );
#endif
}


int
#if NeedFunctionPrototypes
sj3_syoukyo_wcs( wchar_t *yomi, wchar_t *kanji, int code )
#else /* NeedFunctionPrototypes */
sj3_syoukyo_wcs( yomi, kanji, code )
    wchar_t	*yomi, *kanji;
    int		code;
#endif /* NeedFunctionPrototypes */
{
    unsigned char	string1[MAX_KANA], string2[MAX_KANA];

#ifdef USE_SJ3LIB
#if defined(SVR4)  ||  !defined(__sony_news)
    if( wcs2mbs( (char *)string1, yomi, wslen(yomi) ) == -1 )
	return( SJ3_BAD_YOMI_STR );

    if( wcs2mbs( (char *)string2, kanji, wslen(kanji) ) == -1 )
	return( SJ3_BAD_KANJI_STR );

    return( sj3_syoukyo_mb( string1, string2, code ) );
#else
    if( wcs2sjiss( string1, yomi, wslen(yomi) ) == -1 )
	return( SJ3_BAD_YOMI_STR );

    if( wcs2sjiss( string2, kanji, wslen(kanji) ) == -1 )
	return( SJ3_BAD_KANJI_STR );

    return( sj3_syoukyo( string1, string2, code ) );
#endif
#else /* !USE_SJ3LIB */
    return( 0 );
#endif
}


int
#if NeedFunctionPrototypes
sj3_getdict_wcs( struct syoukyo_wcs *item )
#else /* NeedFunctionPrototypes */
sj3_getdict_wcs( item )
    struct syoukyo_wcs	*item;
#endif /* NeedFunctionPrototypes */
{
    int		flag;
    char	ss[512], *sp;
    wchar_t	ws[256];
    int		len;

#ifdef USE_SJ3LIB
#if defined(SVR4)  ||  !defined(__sony_news)
    if( (flag = sj3_getdict_mb( (unsigned char *)ss )) != 0 )
	return( flag );

    len = strlen( ss );
    sp = ss + len + 1;

    len = mbs2wcs( ws, ss, len );
    item->yomi = (wchar_t *)malloc( sizeof(wchar_t) * (len + 1) );
    wscpy( item->yomi, ws );

    len = mbs2wcs( ws, sp, strlen(sp) );
    item->kanji = (wchar_t *)malloc( sizeof(wchar_t) * (len + 1) );
    wscpy( item->kanji, ws );
#else
    if( (flag = sj3_getdict( ss )) != 0 )
	return( flag );

    len = strlen( ss );
    sp = ss + len + 1;

    len = sjiss2wcs( ws, ss, len );
    item->yomi = (wchar_t *)malloc( sizeof(wchar_t) * (len + 1) );
    wscpy( item->yomi, ws );

    len = sjiss2wcs( ws, sp, strlen(sp) );
    item->kanji = (wchar_t *)malloc( sizeof(wchar_t) * (len + 1) );
    wscpy( item->kanji, ws );
#endif
    sp += strlen(sp) + 1;
    item->code = *((unsigned char *)sp);
    return( 0 );
#else /* !USE_SJ3LIB */
    return( -1 );
#endif
}


int
#if NeedFunctionPrototypes
sj3_nextdict_wcs( struct syoukyo_wcs *item )
#else /* NeedFunctionPrototypes */
sj3_nextdict_wcs( item )
    struct syoukyo_wcs	*item;
#endif /* NeedFunctionPrototypes */
{
    int		flag;
    char	ss[512], *sp;
    wchar_t	ws[256];
    int		len;

#ifdef USE_SJ3LIB
#if defined(SVR4)  ||  !defined(__sony_news)
    if( (flag = sj3_nextdict_mb( (unsigned char *)ss )) != 0 )
	return( flag );

    len = strlen( ss );
    sp = ss + len + 1;

    len = mbs2wcs( ws, ss, len );
    item->yomi = (wchar_t *)malloc( sizeof(wchar_t) * (len + 1) );
    wscpy( item->yomi, ws );

    len = mbs2wcs( ws, sp, strlen(sp) );
    item->kanji = (wchar_t *)malloc( sizeof(wchar_t) * (len + 1) );
    wscpy( item->kanji, ws );
#else
    if( (flag = sj3_nextdict( ss )) != 0 )
	return( flag );

    len = strlen( ss );
    sp = ss + len + 1;

    len = sjiss2wcs( ws, ss, len );
    item->yomi = (wchar_t *)malloc( sizeof(wchar_t) * (len + 1) );
    wscpy( item->yomi, ws );

    len = sjiss2wcs( ws, sp, strlen(sp) );
    item->kanji = (wchar_t *)malloc( sizeof(wchar_t) * (len + 1) );
    wscpy( item->kanji, ws );
#endif
    sp += strlen(sp) + 1;
    item->code = *((unsigned char *)sp);

    return( 0 );
#else /* !USE_SJ3LIB */
    return( -1 );
#endif
}

