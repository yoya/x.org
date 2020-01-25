/* $SonyId: SjHenkan.c,v 1.1 1994/05/30 13:03:58 makoto Exp $ */
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
#include	<ctype.h>
#include	<X11/Xlib.h>
#include	<X11/Intrinsic.h>
#include	<X11/StringDefs.h>
#include	<Xmw/Preedit.h>
#include	<Xmw/TextOut.h>
#include	"sjxa.h"
#include	"resource.h"
#include	"SjBuffer.h"
#include	"SjString.h"

#define	MAX	100

static int	member_point, member_max;
static short	member[MAX];
static Position	fp;

extern Widget	RootTextField, Box[2];
extern Sjxa	resources;
extern char	user_name[];
extern LcCtype	user_locale;


void	getkan( w, b, begin )
Widget		w;
Buffer		*b;
unsigned int	begin;
{
    int			ii;
    int			result, len;
    unsigned int	end;

    if( begin == 0 )
	b->henkan->bun[0].srcstr = b->yomi;
    else
	b->henkan->bun[begin].srcstr = b->henkan->bun[begin-1].srcstr +
				       b->henkan->bun[begin-1].srclen;
    for( len = 0, ii = 0; ii < begin; ii++ )
	if( b->henkan->bun[ii].deststr != NULL )
	    len += b->henkan->bun[ii].destlen;
    b->henkan->kanji[len] = '\0';
#ifdef USE_SJ3LIB
    result = sj3_getkan_wcs( b->henkan->bun[begin].srcstr,
			     &b->henkan->bun[begin], &b->henkan->kanji[len],
			     MAX_KANJI - len );
    switch( result ) {
      case -1:
	server_error( b, w, resources.died_error );

      case 0:
	b->henkan->bunsetsu[begin] = BUNSETSU_KANJI;
	b->henkan->bunsetsu[begin+1] = BUNSETSU_END;
	b->henkan->bunsetsu_length[begin] =
	b->henkan->bun[begin].srclen =
	b->henkan->bun[begin].destlen = wslen( b->henkan->bun[begin].srcstr );
	b->henkan->kanji[len] = '\0';
	b->henkan->bun[begin].deststr = NULL;
	return;
    }

    for( end = begin; end < (begin + result); end++ ) {
	b->henkan->bunsetsu[end] = BUNSETSU_KANJI;
	b->henkan->bunsetsu_length[end] = b->henkan->bun[end].srclen;
    }
    b->henkan->bunsetsu[end] = BUNSETSU_END;
#else
    b->henkan->bunsetsu[begin] = BUNSETSU_KANJI;
    b->henkan->bunsetsu[begin+1] = BUNSETSU_END;
    b->henkan->bunsetsu_length[begin] =
    b->henkan->bun[begin].srclen =
    b->henkan->bun[begin].destlen = wslen( b->henkan->bun[begin].srcstr );
    b->henkan->kanji[len] = '\0';
    b->henkan->bun[begin].deststr = NULL;
#endif
}


static void
#if NeedFunctionPrototypes
redraw( Buffer *b, unsigned int count, Boolean replace )
#else /* NeedFunctionPrototypes */
redraw( b, count, replace )
    Buffer		*b;
    unsigned int	count;
    Boolean		replace;
#endif /* NeedFunctionPrototypes */
{
    static wchar_t	string[256];
    static Position	point[30];
    char		ss[256], cc;
    wchar_t		ww[256], xx[256];
    int			ii, offset, limit;

    sprintf( ss, (b->henkan->douon_count < 10) ? "%1d/%1d" :
		 (b->henkan->douon_count < 100) ? "%02d/%2d" :
		 "%03d/%03d", count+1, b->henkan->douon_count );

    if( replace ) {
	mbs2wcs( ww, ss, fp );
	offset = member[member_point];
	limit = member[member_point+1] - offset;
	for( ii = 0, cc = '1'; ii < limit; ii++, cc++ ) {
	    if( cc == 0x3a )
		cc = 'a';
	    point[ii] = wslen( ww ) + 1;
	    sprintf( ss, " [%c]", cc );
	    mbs2wcs( xx, ss, strlen(ss) );
	    wscat( ww, xx );
	    wscat( ww, b->henkan->dou[ii+offset].ddata );
	}
	wscpy( string, ww );
	XmwTextOutSetStringWcs( RootTextField, string, XmwHIGHLIGHT_NORMAL );
    }
    else
	XmwTextOutReplace( RootTextField, 0, fp, ss );

    ii = count - member[member_point];
    XmwTextOutSetHighlight( RootTextField, 0, fp, XmwHIGHLIGHT_SELECTED );
    XmwTextOutSetHighlight( RootTextField, fp, wslen( string ),
			    XmwHIGHLIGHT_NORMAL );
    XmwTextOutSetHighlight( RootTextField, point[ii], point[ii]+3,
			 XmwHIGHLIGHT_SELECTED );
}


void	select_left( b )
Buffer	*b;
{
    b->henkan->douon_point--;
    if( b->henkan->douon_point < member[member_point] ) {
	member_point--;
	if( member_point < 0 ) {
	    b->henkan->douon_point = b->henkan->douon_count - 1;
	    member_point = member_max - 1;
	}
	redraw( b, b->henkan->douon_point,
		b->henkan->douon_count != member[1] );
    }
    else
	redraw( b, b->henkan->douon_point, False );
}


void	select_right( b )
Buffer	*b;
{
    b->henkan->douon_point++;
    if( b->henkan->douon_point == member[member_point+1] ) {
	member_point++;
	if( member[member_point] == b->henkan->douon_count )
	    b->henkan->douon_point = member_point = 0;
	redraw( b, b->henkan->douon_point,
		b->henkan->douon_count != member[1] );
    }
    else
	redraw( b, b->henkan->douon_point, False );
}


void	select_scroll_up( b )
Buffer	*b;
{
    int			ii;

    if( member[1] != b->henkan->douon_count ) {
	ii = b->henkan->douon_point - member[member_point];
	member_point++;
	if( member[member_point] == b->henkan->douon_count )
	    member_point = 0;
	b->henkan->douon_point = member[member_point] + ii;
	if( b->henkan->douon_point >= member[member_point+1] )
	    b->henkan->douon_point = member[member_point+1] - 1;
	redraw( b, b->henkan->douon_point, True );
    }
}


void	select_scroll_down( b )
Buffer	*b;
{
    int			ii;

    if( member[1] != b->henkan->douon_count ) {
	ii = b->henkan->douon_point - member[member_point];
	member_point--;
	if( member_point < 0 )
	    member_point = member_max - 1;
	b->henkan->douon_point = member[member_point] + ii;
	if( b->henkan->douon_point >= member[member_point+1] )
	    b->henkan->douon_point = member[member_point+1] - 1;
	redraw( b, b->henkan->douon_point, True );
    }
}


void	select_top( b )
Buffer	*b;
{
    b->henkan->douon_point = member[member_point];
    redraw( b, b->henkan->douon_point, False );
}


void	select_end( b )
Buffer	*b;
{
    b->henkan->douon_point = member[member_point+1] - 1;
    redraw( b, b->henkan->douon_point, False );
}


static void
#if NeedFunctionPrototypes
rewrite( Buffer *b, int len )
#else /* NeedFunctionPrototypes */
rewrite( b, len )
    Buffer	*b;
    int		len;
#endif /* NeedFunctionPrototypes */
{
    int			ii;
    unsigned int	hp, count;
    Position		fp, tp;

    hp = b->henkan->point;
    count = b->henkan->douon_point;

    for( ii = 0, fp = 0; ii < hp; ii++ )
	fp += b->henkan->bun[ii].destlen;
    tp = fp + len;
    if( b->conversion_style & XIMPreeditNothing ) {
	XmwTextOutReplaceWcs( b->preedit_widget, fp, tp,
			      b->henkan->dou[count].ddata );
	tp = fp + b->henkan->dou[count].dlen;
	XmwTextOutSetHighlight( b->preedit_widget, fp, tp,
				XmwHIGHLIGHT_SELECTED );
    }
    else if( b->conversion_style & (XIMPreeditPosition | XIMPreeditArea) ) {
	XmwPreeditReplaceWcs( b->preedit_widget, fp, tp,
			      b->henkan->dou[count].ddata );
    }
}


void	henkan( w, event, params, n )
Widget		w;
XEvent		*event;
String		*params;
Cardinal	*n;
{
    Buffer		*b;
    wchar_t		ss[1024], *uu;
    int			ii, jj;
    short		len, column;
    unsigned int	hp;

    b = set_buffer( w, event->type );
    if( b == NULL  ||  !check_action_args( params, *n, "conversion" ) )
	return;
    if( b->state & (ADD | DELETE | CODE | SERVER | ERROR) )
	return;

    if( b->state & INTERMEDIATE ) {
	if( b->yomi[0] == '\0' )
	    return;

#ifdef USE_SJ3LIB
	switch( sj3_open( resources.server, user_name ) ) {
	  case SJ3_SERVER_DEAD:
	  case SJ3_CONNECT_ERROR:
	    server_error( b, w, resources.died_error );
	    return;
/*
	  case SJ3_ALREADY_CONNECTED:
	    server_error( b, w, resources.open_error1 );
	    return;
*/
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
	    XBell( XtDisplay(w), 0 );
	    return;
	}
	b->state = CONVERSION;
	romaji2kana( b );

#ifdef USE_SJ3LIB
	sj3_lockserv();
#endif
	getkan( w, b, 0 );
	for( uu = ss, ii = 0; b->henkan->bunsetsu[ii] != BUNSETSU_END; ii++ ) {
	    if( b->henkan->bun[ii].deststr == NULL )
		wsncpy( uu, b->henkan->bun[ii].srcstr,
			b->henkan->bun[ii].srclen );
	    else
		wsncpy( uu, b->henkan->bun[ii].deststr,
			b->henkan->bun[ii].destlen );
	    uu += b->henkan->bun[ii].destlen;
	}
	*uu = '\0';

	XtVaSetValues( b->preedit_widget, XmwNdisplayCaret, False, NULL );
	if( b->conversion_style & XIMPreeditNothing ) {
	    XmwTextOutSetStringWcs( b->preedit_widget, ss,
				    XmwHIGHLIGHT_NORMAL );
	    XmwTextOutSetHighlight( b->preedit_widget, 0,
				    b->henkan->bun[0].destlen,
				    XmwHIGHLIGHT_SELECTED );
	    XmwTextOutSetCursorPosition( b->preedit_widget, 0 );
	}
	else {
	    XmwPreeditSetStringWcs( w, ss, XmwHIGHLIGHT_SECONDARY_SELECTED );
	    XmwPreeditSetHighlight( b->preedit_widget, 0,
				    b->henkan->bun[0].destlen,
				    XmwHIGHLIGHT_SELECTED );
	    XmwPreeditSetCursorPosition( w, 0 );
	}
	b->henkan->point = 0;

	b->conversion_mode_toggle = b->conversion_mode;
    }
    else if( b->state & CONVERSION ) {
	hp = b->henkan->point;
	if( b->henkan->bunsetsu[hp] & ~(BUNSETSU_KANJI | BUNSETSU_EDIT) ) {
	    wscpy( ss, b->henkan->bun[hp].deststr );
	    ss[b->henkan->bun[hp].destlen] = '\0';
	}
	else {
	    wscpy( ss, b->henkan->bun[hp].srcstr );
	    ss[b->henkan->bun[hp].srclen] = '\0';
	}
	b->henkan->douon_count = sj3_getdouon_wcs( ss, &b->henkan->dou );
	if( b->henkan->douon_count == -1 ) {
	    server_error( b, w, resources.died_error );
	    return;
	}

	b->state = WRAP;

	b->henkan->douon_point = 0;
	if( b->henkan->bun[hp].deststr != NULL ) {
	    for( ii = 0; ii < b->henkan->douon_count; ii++ ) {
		if( wsncmp( b->henkan->bun[hp].deststr,
			    b->henkan->dou[ii].ddata,
			    Min( b->henkan->bun[hp].destlen,
				 b->henkan->dou[ii].dlen ) ) != 0  ||
		    b->henkan->bun[hp].destlen != b->henkan->dou[ii].dlen ) {
		    b->henkan->douon_point = ii;
		    break;
		}
	    }
	}
	rewrite( b, b->henkan->bun[hp].destlen );
    }
    else if( b->state & WRAP ) {
	len = b->henkan->dou[b->henkan->douon_point].dlen;
	b->henkan->douon_point++;
	if( b->henkan->douon_point == b->henkan->douon_count )
	    b->henkan->douon_point = 0;
	if( resources.wrap_to_select <= b->henkan->douon_point  ||
	    b->henkan->douon_point == 0 ) {
	    b->state = SELECT;
	    b->henkan->wrap_length = len;
	    hp = b->henkan->point;

	    if( b->conversion_style & (XIMPreeditPosition | XIMPreeditArea) ) {
		PopupSelectDialog( b );
		return;
	    }
	    XtUnmapWidget( Box[0] );
	    XtUnmapWidget( Box[1] );
	    XtMapWidget( RootTextField );
	    XmwTextOutSetHighlight( RootTextField, 0, 10000,
				    XmwHIGHLIGHT_NORMAL );

	    if( b->henkan->douon_count < 10 )
		fp = 3;
	    else if( b->henkan->douon_count < 100 )
		fp = 5;
	    else
		fp = 7;
	    b->henkan->dou[b->henkan->douon_count].dlen = 0;

	    XtVaGetValues( RootTextField, XmwNcolumns, &column, NULL );
	    column *= 2;		/* XXXXX */
	    member[0] = 0;
	    member_max = 0;
	    ii = 0;
	    do {
		len = fp;
		jj = 1;
		while( (len + b->henkan->dou[ii].width + 4) < column  &&
		       ii < b->henkan->douon_count  &&  jj++ < 30 ) {
		    len += b->henkan->dou[ii++].width + 4;
		}
		member[++member_max] = ii;
		if( member[member_max] == member[member_max-1] )
		    member[member_max] = ++ii;
	    } while( ii < b->henkan->douon_count );
	    for( ii = member_max; b->henkan->douon_point < member[ii]; ii-- );
	    member_point = ii;

	    redraw( b, b->henkan->douon_point, True );
	    XmwTextOutSetHighlight( RootTextField, 0, fp,
				    XmwHIGHLIGHT_SELECTED );
	}
	else
	    rewrite( b, len );
    }
    else if( b->state & SELECT ) {
	if( b->conversion_style & (XIMPreeditPosition | XIMPreeditArea) )
	    XmwListNextItem( b->list_widget );
	else
	    select_right( b );
    }
    else if( b->state & EDIT )
#ifdef USE_SJ3LIB
	edit_henkan( b, w );
#else
	edit_kettei( b );
#endif
}


void	henkan_cancel( b )
Buffer	*b;
{
    wchar_t		ss[MAX_KANA];
    unsigned int	cursor;
    int			ii;

    if( b->henkan ) {
	sj3_freedouon_wcs( b->henkan->dou );
	free( b->henkan );
	b->henkan = NULL;
    }

    b->state = INTERMEDIATE;
    b->conversion_mode_toggle = b->conversion_mode;
    b->point = b->end_point - 1;
    cursor = b->conversion_flag[b->point] ? b->yomiC[b->end_point-1]
					  : b->yomiC[b->end_point];

    wscpy( ss, b->yomi );
    if( b->conversion_style & XIMPreeditNothing ) {
	XmwTextOutSetStringWcs( b->preedit_widget, ss, XmwHIGHLIGHT_NORMAL );
	for( ii = 0; ii <= b->point; ii++ )
	    if( !b->conversion_flag[ii] )
		XmwTextOutSetHighlight( b->preedit_widget, b->yomiC[ii],
					b->yomiC[ii+1],
					XmwHIGHLIGHT_SECONDARY_SELECTED );
	XmwTextOutSetCursorPosition( b->preedit_widget, cursor );
    }
    else if( b->conversion_style & (XIMPreeditPosition | XIMPreeditArea) ) {
	b->cursor = cursor;
	XmwPreeditSetStringWcs( b->preedit_widget, ss,
				XmwHIGHLIGHT_SECONDARY_SELECTED );
    }
    XtVaSetValues( b->preedit_widget, XmwNdisplayCaret, True, NULL );

#ifdef USE_SJ3LIB
    if( !CheckState( CONVERSION|EDIT|ADD|DELETE|SELECT|WRAP ) ) {
	sj3_unlockserv();
	sj3_close();
    }
#endif
}


Boolean	henkan_inkey( b, w, c )
Buffer		*b;
Widget		w;
unsigned char	c;
{
    int			ii;
    unsigned int	hp, count;
    Position		fp, tp;
    wchar_t		ss[256];

    if( isdigit(c)  &&  c != '0' )
	c -= '1';
    else if( islower(c) )
	c -= 'a' - 9;
    else
	return( False );
    
    if( (count = member[member_point] + c) >= member[member_point+1] )
	return( False );

    hp = b->henkan->point;
    for( ii = 0, fp = 0; ii < hp; ii++ )
	fp += b->henkan->bun[ii].destlen;
    tp = fp + b->henkan->wrap_length;
    if( b->conversion_style & XIMPreeditNothing )
	XmwTextOutReplaceWcs( b->preedit_widget, fp, tp,
			      b->henkan->dou[count].ddata );
    else if( b->conversion_style & (XIMPreeditPosition | XIMPreeditArea) )
	XmwPreeditReplaceWcs( b->preedit_widget, fp, tp,
			      b->henkan->dou[count].ddata );

    if( b->henkan->bun[hp].deststr != NULL  &&
	wsncmp( b->henkan->bun[hp].deststr, b->henkan->dou[count].ddata,
		b->henkan->dou[count].dlen ) ) {
	if( b->henkan->bunsetsu[hp+1] == BUNSETSU_END ) {
	    wscpy( b->henkan->bun[hp].deststr, b->henkan->dou[count].ddata );
	    b->henkan->bun[hp].destlen = b->henkan->dou[count].dlen;
	    b->henkan->bun[hp].dcid = b->henkan->dou[count].dcid;
	}
	else {
	    wscpy( ss, b->henkan->bun[hp+1].deststr );
	    wscpy( b->henkan->bun[hp].deststr, b->henkan->dou[count].ddata );
	    wscat( b->henkan->kanji, ss );

	    ii = b->henkan->dou[count].dlen - b->henkan->bun[hp].destlen;

	    b->henkan->bun[hp].destlen = b->henkan->dou[count].dlen;
	    b->henkan->bun[hp].dcid = b->henkan->dou[count].dcid;

	    while( b->henkan->bunsetsu[hp++] != BUNSETSU_END )
		b->henkan->bun[hp].deststr += ii;
	}

#ifdef USE_SJ3LIB
	if( sj3_gakusyuu( &b->henkan->bun[b->henkan->point].dcid ) == -1 )
	    server_error( b, w, resources.died_error );
#endif
    }

    XtUnmapWidget( RootTextField );
    XtMapWidget( Box[0] );
    XtMapWidget( Box[1] );

    b->state &= ERROR;
    b->state |= CONVERSION;
    return( True );
}


void	wrapback( w, event, params, n )
Widget		w;
XEvent		*event;
String		*params;
Cardinal	*n;
{
    Buffer		*b;
    int			ii, jj, len;
    unsigned int	hp;
    wchar_t		ss[256];
    Arg			args[1];
    short		column;

    b = set_buffer( w, event->type );
    if( b == NULL  ||  (b->state & ~(CONVERSION | WRAP | SELECT))  ||
	!check_action_args( params, *n, "wrap-back" ) )
	return;

    if( b->state & CONVERSION ) {
	hp = b->henkan->point;
	if( b->henkan->bunsetsu[hp] & ~(BUNSETSU_KANJI | BUNSETSU_EDIT) ) {
	    wscpy( ss, b->henkan->bun[hp].deststr );
	    ss[b->henkan->bun[hp].destlen] = '\0';
	}
	else {
	    wscpy( ss, b->henkan->bun[hp].srcstr );
	    ss[b->henkan->bun[hp].srclen] = '\0';
	}
	b->henkan->douon_count = sj3_getdouon_wcs( ss, &b->henkan->dou );
	if( b->henkan->douon_count == -1 ) {
	    server_error( b, w, resources.died_error );
	    return;
	}

	b->state = WRAP;
	b->henkan->dou[b->henkan->douon_count].dlen = 0;

	b->henkan->douon_point = b->henkan->douon_count - 1;
	for( ii = b->henkan->douon_count - 1; ii >= 0; ii++ ) {
	    if( wsncmp( b->henkan->bun[hp].deststr,
			b->henkan->dou[ii].ddata,
			Min( b->henkan->bun[hp].destlen,
			     b->henkan->dou[ii].dlen ) ) != 0  ||
	       b->henkan->bun[hp].destlen != b->henkan->dou[ii].dlen ) {
		b->henkan->douon_point = ii;
		break;
	    }
	}

	rewrite( b, b->henkan->bun[hp].destlen );
    }
    else if( b->state & WRAP ) {
	len = b->henkan->dou[b->henkan->douon_point--].dlen;
	hp = b->henkan->point;

	if( b->henkan->douon_point < 0 )
	    b->henkan->douon_point = b->henkan->douon_count - 1;
	if( resources.wrap_to_select <= b->henkan->douon_point  ||
	    b->henkan->douon_point == (b->henkan->douon_count - 1) ) {
	    b->state = SELECT;
	    b->henkan->wrap_length = len;
	    if( b->conversion_style & (XIMPreeditPosition | XIMPreeditArea) ) {
		PopupSelectDialog( b );
		return;
	    }
	    XtUnmapWidget( Box[0] );
	    XtUnmapWidget( Box[1] );
	    XtMapWidget( RootTextField );
	    XmwTextOutSetHighlight( RootTextField, 0, 1000,
				    XmwHIGHLIGHT_NORMAL );

	    if( b->henkan->douon_count < 10 )
		fp = 3;
	    else if( b->henkan->douon_count < 100 )
		fp = 5;
	    else
		fp = 7;
	    b->henkan->dou[b->henkan->douon_count].dlen = 0;

	    XtSetArg( args[0], XmwNcolumns, &column );
	    XtGetValues( RootTextField, args, 1 );
	    member[0] = 0;
	    member_max = 0;
	    ii = 0;
	    do {
		len = fp;
		jj = 1;
		while( (len + b->henkan->dou[ii].dlen + 4) < column  &&
		      ii < b->henkan->douon_count  &&  jj++ < 30 ) {
		    len += b->henkan->dou[ii++].dlen + 4;
		}
		member[++member_max] = ii;
	    } while( ii < b->henkan->douon_count );
	    for( ii = member_max; b->henkan->douon_point < member[ii]; ii-- );
	    member_point = ii;

	    redraw( b, b->henkan->douon_point, True );
	    XmwTextOutSetHighlight( RootTextField, 0, fp,
				    XmwHIGHLIGHT_SELECTED );
	}
	else
	    rewrite( b, len );
    }
    else if( b->state & SELECT ) {
	if( b->conversion_style & (XIMPreeditPosition | XIMPreeditArea) )
	    XmwListPreviousItem( b->list_widget );
	else
	    select_left( b );
    }
}


void	wrap_end( b, w, flag )
Buffer	*b;
Widget	w;
Boolean	flag;
{
    int			ii, jj;
    unsigned int	hp, count;
    Position		fp;
    wchar_t		ss[256];

    hp = b->henkan->point;
    count = b->henkan->douon_point;
    b->state = CONVERSION;

    if( b->henkan->bun[hp].deststr != NULL  &&
	(wsncmp( b->henkan->bun[hp].deststr, b->henkan->dou[count].ddata,
		 Min( b->henkan->bun[hp].destlen,
		      b->henkan->dou[count].dlen ) ) != 0
       ||  b->henkan->bun[hp].destlen != b->henkan->dou[count].dlen) ) {
	for( ii = 0, fp = 0; ii < hp; ii++ )
	    fp += b->henkan->bun[ii].destlen;

	if( b->henkan->bunsetsu[hp+1] == BUNSETSU_END ) {
	    wscpy( b->henkan->bun[hp].deststr, b->henkan->dou[count].ddata );
	    b->henkan->bun[hp].destlen = b->henkan->dou[count].dlen;
	    b->henkan->bun[hp].dcid = b->henkan->dou[count].dcid;
	}
	else {
	    for( ii = hp; b->henkan->bunsetsu[ii] != BUNSETSU_END; ii++ )
		if( b->henkan->bun[ii].deststr != NULL )
		    break;
	    if( b->henkan->bunsetsu[ii] == BUNSETSU_END ) {
		wscat( b->henkan->kanji, b->henkan->dou[count].ddata );
		for( ii = jj = 0; ii < hp; ii++ )
		    if( b->henkan->bun[ii].deststr != NULL )
			jj = ii;
		if( b->henkan->bun[jj].deststr == NULL )
		    b->henkan->bun[hp].deststr = b->henkan->kanji;
		else
		    b->henkan->bun[hp].deststr = b->henkan->bun[jj].deststr +
			b->henkan->bun[jj].destlen;
	    }
	    else {
		for( jj = hp + 1, ss[0] = '\0';
		    b->henkan->bunsetsu[jj] != BUNSETSU_END; jj++ ) {
		    if( b->henkan->bun[jj].deststr != NULL ) {
			wscpy( ss, b->henkan->bun[jj].deststr );
			break;
		    }
		}
		wscpy( b->henkan->bun[ii].deststr,
		       b->henkan->dou[count].ddata );
		wscat( b->henkan->kanji, ss );
	    }

	    ii = b->henkan->dou[count].dlen - b->henkan->bun[hp].destlen;

	    b->henkan->bun[hp].destlen = b->henkan->dou[count].dlen;
	    b->henkan->bun[hp].dcid = b->henkan->dou[count].dcid;

	    while( b->henkan->bunsetsu[hp] != BUNSETSU_END ) {
		hp++;
		if( b->henkan->bun[hp].deststr != NULL )
		    b->henkan->bun[hp].deststr += ii;
	    }
	}

#ifdef USE_SJ3LIB
	if( flag )
	    if( sj3_gakusyuu( &b->henkan->bun[b->henkan->point].dcid ) == -1 )
		server_error( b, w, resources.died_error );
#endif
    }
}
