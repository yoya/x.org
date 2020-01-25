/* $SonyId: SjRk.c,v 1.1 1994/05/30 13:03:58 makoto Exp $ */
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
#include	<string.h>
#include	<ctype.h>
#include	<locale.h>
#include	"sjxa.h"
#include	"SjString.h"

#ifndef DEFRKFILE
#define DEFRKFILE	"/usr/lib/X11/sjxa/sjxark"
#endif
#define RKFILE		".sjxark"

static RkTable	*rk_tableLower = NULL;
static RkTable	*rk_tableUpper = NULL;

extern char	home_directory[];
extern LcCtype	user_locale;
extern char	*locale_name;


static Boolean
#if NeedFunctionPrototypes
ischarinc( unsigned char *s, unsigned char c )
#else	/* NeedFunctionPrototypes */
ischarinc( s, c )
    unsigned char	*s, c;
#endif	/* NeedFunctionPrototypes */
{
    while( *s != '\0' )
	if( *(s++) == c )
	    return( True );
    return( False );
}


static unsigned char *
#if NeedFunctionPrototypes
_strtok( unsigned char *s1, unsigned char *s2 )
#else /* NeedFunctionPrototypes */
_strtok( s1, s2 )
    unsigned char	*s1, *s2;
#endif /* NeedFunctionPrototypes */
{
    static unsigned char	*s = NULL;
    unsigned char		*t, *u;

    if( s1 != NULL )
	s = s1;
    if( s == NULL )
	return( NULL );

    while( ischarinc( s2, *s ) ) {
	if( *(++s) == '\0' ) {
	    s = NULL;
	    return( NULL );
	}
    }
    t = u = s;
    do {
	if( *s == '\\' )
	    if( *(++s) == '\0' )
		break;
	*(u++) = *s;
	if( iskanji( *(s++) ) )
	    *(u++) = *(s++);
    } while( !ischarinc( s2, *s )  &&  *s != '\0' );
    if( *(s++) == '\0' )
	s = '\0';
    *u = '\0';
    if( *t == '\0' ) {
	s = NULL;
	return( NULL );
    }
    return( t );
}


static Boolean
#if NeedFunctionPrototypes
rkinit( char *rkfile, LcCtype locale )
#else /* NeedFunctionPrototypes */
rkinit( rkfile, locale )
    char	*rkfile;
    LcCtype	locale;
#endif /* NeedFunctionPrototypes */
{
    FILE		*fp;
    unsigned char	line[BUFSIZ];
    unsigned char	*key;
    unsigned char	*ss;
    wchar_t		yomi[32];
    unsigned char	*str;
    RkTable		*rt, *r;
    int			len, ii, jj, diff;

    if( (fp = fopen( rkfile, "r")) == NULL )
	return( False );
    while( fgets( (char *)line, BUFSIZ, fp ) != NULL ) {
	if( line[0] == '#' )
	    continue;

	if( (key = _strtok( line, (unsigned char *)" \t\n" )) == NULL )
	    continue;
	if( !(ss = _strtok( (unsigned char *)NULL, (unsigned char *)" \t\n" )) )
	    continue;
	if( user_locale == locale )
	    len = mbs2wcs( yomi, (char *)ss, strlen((char *)ss) );
	else {
	    switch( locale ) {
	      case LC_CTYPE_SHIFTJIS:
		len = sjiss2wcs( yomi, (unsigned char *)ss,
				 strlen((char *)ss) );
		break;

	      case LC_CTYPE_EUC:
		len = eucs2wcs( yomi, (char *)ss, strlen((char *)ss) );
		break;
	    }
	}
	str = _strtok( (unsigned char *)NULL, (unsigned char *)" \t\n" );

	if( str != NULL ) {
	    for( ii = 0; *(str+ii) != NULL; ii++ )
		if( !isalpha(*(str+ii)) )
		    goto next;

	    jj = strlen( (char *)key );
	    if( ii > jj  ||  strcmp( (char *)&key[jj-ii], (char *)str ) )
	       goto next;
	}

	rt = (RkTable *)calloc( 1, sizeof(RkTable) );
	rt->key = (unsigned char *)malloc( strlen((char *)key) + 1 );
	strcpy( (char *)rt->key, (char *)key );
	rt->yomi = (wchar_t *)malloc( sizeof(wchar_t) * (len + 1) );
	wscpy( rt->yomi, yomi );
	if( str != NULL ) {
	    rt->str = (unsigned char *)malloc( strlen((char *)str) + 1 );
	    strcpy( (char *)rt->str, (char *)str );
	}

	for( r = rk_tableLower; r != NULL; r = r->next ) {
	    if( (diff = strcmp( (char *)key, (char *)r->key )) == 0 ) {
	        free( rt->key );
		free( rt->yomi );
		if( str != NULL )
		    free( rt->str );
		free( rt );
		break;
	    }
	    if( diff < 0 ) {
		if( r->previous == NULL ) {
		    rk_tableLower = rt;
		    rt->next = r;
		    r->previous = rt;
		}
		else {
		    r->previous->next = rt;
		    rt->next = r;
		    rt->previous = r->previous;
		    r->previous = rt;
		}
		break;
	    }
	    if( r->next == NULL ) {
		rk_tableUpper = rt;
		r->next = rt;
		rt->previous = r;
		break;
	    }
	}
	if( rk_tableLower == NULL )
	    rk_tableLower = rk_tableUpper = rt;
next:;
    }
    return( rk_tableLower != NULL );
}


void
#if NeedFunctionPrototypes
getsjxark( void )
#else /* NeedFunctionPrototypes */
getsjxark()
#endif /* NeedFunctionPrototypes */
{
    register char	*p;
    char		RKfile[BUFSIZ];

    RKfile[0] = '\0';
    if( (p = getenv( "SJXARK" )) != NULL  &&  *p != '\0' ) {
	if( *p != '/' ) {
	    strcpy( RKfile, home_directory );
	    strcat( RKfile, "/" );
	}
	strcat( RKfile, p );
	if( rkinit( RKfile, user_locale ) )
	    return;
	strcat( RKfile, "/" );
	strcat( RKfile, RKFILE );
	if( rkinit( RKfile, user_locale ) )
	    return;
    }
    if( (p = getenv( "XAPPLRESDIR" )) != NULL  &&  *p != '\0' ) {
	sprintf( RKfile, "%s/%s/%s", p, locale_name, RKFILE );
	if( rkinit( RKfile, user_locale ) )
	    return;
	sprintf( RKfile, "%s/%s", p, RKFILE );
	if( rkinit( RKfile, user_locale ) )
	    return;
    }
    if( home_directory[0] != '\0' ) {
	sprintf( RKfile, "%s/%s/%s", home_directory, locale_name, RKFILE );
	if( rkinit( RKfile, user_locale ) )
	    return;

	strcpy( RKfile, home_directory );
	strcat( RKfile, "/" );
	strcat( RKfile, RKFILE );
	if( rkinit( RKfile, user_locale ) )
	    return;
    }
    if( !rkinit( DEFRKFILE, LC_CTYPE_EUC ) )
	fprintf( stderr, "Warning:cannot open rule file %s\n\r", DEFRKFILE );
}


RkTable	*
#if NeedFunctionPrototypes
alpha2hiraganaLower( unsigned char *key )
#else /* NeedFunctionPrototypes */
alpha2hiraganaLower( key )
    unsigned char	*key;
#endif /* NeedFunctionPrototypes */
{
    int		diff;
    RkTable	*rt;

    for( rt = rk_tableLower; rt != NULL; rt = rt->next ) {
	if( (diff = strcmp( (char *)key, (char *)rt->key )) == 0 )
	    return( rt );
	if( diff < 0 )
	    break;
    }
    return( NULL );
}


RkTable	*
#if NeedFunctionPrototypes
alpha2hiraganaUpperS( unsigned char *key )
#else /* NeedFunctionPrototypes */
alpha2hiraganaUpperS( key )
    unsigned char	*key;
#endif /* NeedFunctionPrototypes */
{
    RkTable	*rt;

    for( rt = rk_tableUpper; rt != NULL; rt = rt->previous ) {
	if( strncmp( (char *)key, (char *)rt->key, strlen( (char *)rt->key ) )
	    == 0 )
	    return( rt );
	if( key[0] > rt->key[0] )
	    break;
    }
    return( NULL );
}


RkTable	*
#if NeedFunctionPrototypes
alpha2hiraganaLowerD( unsigned char *key )
#else /* NeedFunctionPrototypes */
alpha2hiraganaLowerD( key )
    unsigned char	*key;
#endif /* NeedFunctionPrototypes */
{
    int		diff;
    int		length = strlen((char *)key);
    RkTable	*rt;

    for( rt = rk_tableLower; rt != NULL; rt = rt->next ) {
	if( (diff = strncmp( (char *)key, (char *)rt->key, length )) == 0 )
	    return( rt );
	if( diff < 0 )
	    break;
    }
    return( NULL );
}
