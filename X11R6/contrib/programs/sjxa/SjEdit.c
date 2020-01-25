/* $SonyId: SjEdit.c,v 1.1 1994/05/30 13:03:58 makoto Exp $ */
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

extern Widget	TopLevel, RootText, RootTextField, Box[];
extern Sjxa	resources;
extern LcCtype	user_locale;

static void
#if NeedFunctionPrototypes
edit_position( Buffer *b, Widget w, XEvent *event, String *params, Cardinal *n )
#else /* NeedFunctionPrototypes */
edit_position( b, w, event, params, n )
    Buffer	*b;
    Widget	w;
    XEvent	*event;
    String	*params;
    Cardinal	*n;
#endif /* NeedFunctionPrototypes */
{
    int			ii, jj;
    int			start, end;
    int			key_start, yomi_start;
    unsigned int	hp;

    hp = b->henkan->point;
    ii = b->henkan->bun[hp].srcstr - b->henkan->bun[0].srcstr;
    for( start = 0; b->yomiC[start] != ii; start++ );
    ii += b->henkan->bun[hp].srclen;
    for( end = start; b->yomiC[end] != ii; end++ );
    if( end >= MAX_KEY )	/* XXXXX */
	return;

    push_buffer( b );

    for( ii = 0, jj = b->keyC[start]; jj < b->keyC[end]; ii++, jj++ ) {
	b->key[ii] = b->key[jj];
	b->str[ii] = b->str[jj];
    }
    while( ii < MAX_KEY ) {
	b->key[ii] = b->str[ii] = '\0';
	ii++;
    }
    for( ii = 0, jj = b->yomiC[start]; jj < b->yomiC[end]; ii++, jj++ )
	b->yomi[ii] = b->yomi[jj];
    while( ii < MAX_KANA )
	b->yomi[ii++] = '\0';
    key_start = b->keyC[start];
    yomi_start = b->yomiC[start];
    for( ii = 0, jj = start; jj <= end; ii++, jj++ ) {
	b->keyC[ii] = b->keyC[jj] - key_start;
	b->yomiC[ii] = b->yomiC[jj] - yomi_start;
	b->mode[ii] = b->mode[jj];
	b->conversion_flag[ii] = b->conversion_flag[jj];
    }
    b->keyC[ii+1] = b->keyC[ii];
    b->yomiC[ii+1] = b->yomiC[ii];
    jj = ii;
    while( ++ii < MAX_KEY ) {
	b->keyC[ii] = 0;
	b->mode[ii] = 0;
	b->conversion_flag[ii] = False;
    }
    while( ++jj < MAX_KANA )
	b->yomiC[jj] = 0;

    b->rt = NULL;
    b->end_point = end - start;
    if( b->conversion_flag[b->end_point-1] ) {
	end = ++b->end_point;
	b->keyC[end] = b->keyC[end-1];
	b->yomiC[end] = b->yomiC[end-1];
    }
    b->point = b->end_point - 1;
    b->cursor = b->yomiC[b->end_point];

    for( ii = 0, start = 0; ii < hp; ii++ )
	start += b->henkan->bun[ii].destlen;
    end = start + b->henkan->bun[hp].destlen;
    XmwPreeditReplaceWcs( b->preedit_widget, start, end, b->yomi );
    XmwPreeditSetCursorPosition( b->preedit_widget, b->cursor + start );
    XtVaSetValues( b->preedit_widget, XmwNdisplayCaret, True, NULL );

    b->state = EDIT;
}


void	edit( w, event, params, n )
Widget		w;
XEvent		*event;
String		*params;
Cardinal	*n;
{
    Buffer		*b;
    int			ii, jj;
    int			start, end;
    int			key_start, yomi_start;
    unsigned int	hp;
    Position		x;

    b = set_buffer( w, event->type );
    if( b == NULL  ||  !check_action_args( params, *n, "edit" ) )
	return;

    if( !(b->state & (CONVERSION | WRAP)) )
	return;

    if( b->state == WRAP )
	wrap_end( b, w, False );

    if( b->conversion_style & (XIMPreeditPosition | XIMPreeditArea) ) {
	edit_position( b, w, event, params, n );
	return;
    }

    push_buffer( b );
    b->state = EDIT;
    XtUnmapWidget( Box[0] );
    XtUnmapWidget( Box[1] );
    XtVaGetValues( Box[0], XtNx, &x, NULL );
    XtVaSetValues( RootTextField, XmwNdisplayCaret, True, NULL );
    XtMapWidget( RootTextField );
    XmwTextOutSetHighlight( RootTextField, 0, 1000, XmwHIGHLIGHT_NORMAL );

    XtSetKeyboardFocus( TopLevel, RootTextField );

    hp = b->henkan->point;
    ii = b->henkan->bun[hp].srcstr - b->henkan->bun[0].srcstr;
    for( start = 0; b->yomiC[start] != ii; start++ );
    ii += b->henkan->bun[hp].srclen;
    for( end = start; b->yomiC[end] != ii; end++ );
    if( end >= MAX_KEY )	/* XXXXX */
	return;

    for( ii = 0, jj = b->keyC[start]; jj < b->keyC[end]; ii++, jj++ ) {
	b->key[ii] = b->key[jj];
	b->str[ii] = b->str[jj];
    }
    while( ii < MAX_KEY ) {
	b->key[ii] = b->str[ii] = '\0';
	ii++;
    }
    for( ii = 0, jj = b->yomiC[start]; jj < b->yomiC[end]; ii++, jj++ )
	b->yomi[ii] = b->yomi[jj];
    while( ii < MAX_KANA )
	b->yomi[ii++] = '\0';
    key_start = b->keyC[start];
    yomi_start = b->yomiC[start];
    for( ii = 0, jj = start; jj <= end; ii++, jj++ ) {
	b->keyC[ii] = b->keyC[jj] - key_start;
	b->yomiC[ii] = b->yomiC[jj] - yomi_start;
	b->mode[ii] = b->mode[jj];
	b->conversion_flag[ii] = b->conversion_flag[jj];
    }
    b->keyC[ii+1] = b->keyC[ii];
    b->yomiC[ii+1] = b->yomiC[ii];
    jj = ii;
    while( ++ii < MAX_KEY ) {
	b->keyC[ii] = 0;
	b->mode[ii] = 0;
	b->conversion_flag[ii] = False;
    }
    while( ++jj < MAX_KANA )
	b->yomiC[jj] = 0;

    b->rt = NULL;
    b->end_point = end - start;
    if( b->conversion_flag[b->end_point-1] ) {
	end = ++b->end_point;
	b->keyC[end] = b->keyC[end-1];
	b->yomiC[end] = b->yomiC[end-1];
    }
    b->point = b->end_point - 1;
    b->cursor = b->yomiC[b->end_point];

    XmwTextOutSetStringWcs( RootTextField, b->yomi, XmwHIGHLIGHT_NORMAL );
    XmwTextOutSetCursorPosition( RootTextField, b->cursor );
}


void	edit_cancel( b )
Buffer	*b;
{
    int			start, end;
    unsigned int	hp;
    int			ii;

    if( b->conversion_style & (XIMPreeditPosition | XIMPreeditArea) ) {
	wchar_t	ss[MAX_KEY];

	XtVaSetValues( b->preedit_widget, XmwNdisplayCaret, False, NULL );
	hp = b->henkan->point;
	for( ii = 0, start = 0; ii < hp; ii++ )
	    start += b->henkan->bun[ii].destlen;
	end = start + wslen( b->yomi );
	pop_buffer( b );
	if( b->henkan->bun[hp].deststr == NULL )
	    wscpy( ss, b->henkan->bun[hp].srcstr );
	else
	    wscpy( ss, b->henkan->bun[hp].deststr );
	ss[b->henkan->bun[hp].destlen] = '\0';
	XmwPreeditReplaceWcs( b->preedit_widget, start, end, ss );
	XmwPreeditSetHighlight( b->preedit_widget, start, start+wslen(ss),
			        XmwHIGHLIGHT_SELECTED );
	b->state = CONVERSION;
    }
    else {
	XtUnmapWidget( RootTextField );
	XtMapWidget( Box[0] );
	XtMapWidget( Box[1] );
    
	XtVaSetValues( RootTextField, XmwNdisplayCaret, False, NULL );
	if( b->conversion_style & XIMPreeditNothing )
	    XtSetKeyboardFocus( TopLevel, RootText );
	pop_buffer( b );
	b->state = CONVERSION;
    }
}


void
#if NeedFunctionPrototypes
edit_kettei( Buffer *b )
#else /* NeedFunctionPrototypes */
edit_kettei( b )
Buffer	*b;
#endif /* NeedFunctionPrototypes */
{
    int			ii, jj, delta;
    int			start, end, length;
    unsigned int	hp;
    Buffer		btmp;

    btmp = *b;
    edit_cancel( b );

    hp = b->henkan->point;

    replace_bunsetsu( b, btmp.yomi, BUNSETSU_MUHENKAN, True );

    ii = b->henkan->bun[hp].srcstr - b->henkan->bun[0].srcstr;
    for( start = 0; b->yomiC[start] != ii; start++ );
    ii += b->henkan->bun[hp].srclen;
    for( end = start; b->yomiC[end] != ii; end++ );

    length = wslen( btmp.yomi );
    ii = length - b->henkan->bun[hp].srclen;
    b->henkan->bun[hp].srclen = length;
    while( b->henkan->bunsetsu[++hp] != BUNSETSU_END )
	b->henkan->bun[hp].srcstr += ii;

    for( ii = b->keyC[end]; ii < b->keyC[end+1]; ii++ )
	b->str[ii] = '\0';
    for( ii = btmp.keyC[btmp.end_point], jj = b->keyC[end];
	 jj < b->keyC[b->end_point]; ii++, jj++ ) {
	btmp.key[ii] = b->key[jj];
	btmp.str[ii] = b->str[jj];
    }
    btmp.key[ii] = btmp.str[ii] = '\0';

    for( ii = btmp.yomiC[btmp.end_point], jj = b->yomiC[end];
	 jj < b->yomiC[b->end_point]; ii++, jj++ )
	btmp.yomi[ii] = b->yomi[jj];
    btmp.yomi[ii] = '\0';

    if( btmp.keyC[btmp.end_point-1] == btmp.keyC[btmp.end_point] )
	btmp.end_point--;
    if( btmp.keyC[btmp.end_point] == 0 )
	btmp.end_point = 0;
    for( ii = btmp.end_point, jj = end; jj <= b->end_point; ii++, jj++ ) {
	btmp.keyC[ii] = b->keyC[jj] - b->keyC[end] + btmp.keyC[btmp.end_point];
	btmp.yomiC[ii] = b->yomiC[jj] - b->yomiC[end] +
			 btmp.yomiC[btmp.end_point];
	btmp.mode[ii] = b->mode[jj];
	btmp.conversion_flag[ii] = b->conversion_flag[jj];
    }
    btmp.end_point += b->end_point - end;
    b->end_point = start;


    delta = 0;
    if( b->str[b->keyC[start]] ) {
	ii = strlen( (char *)&b->str[b->keyC[start]] );
	if( ii > b->keyC[start+1] - b->keyC[start] )
	    ii = b->keyC[start+1] - b->keyC[start];
	if( strncmp( (char *)&b->str[b->keyC[start]], (char *)btmp.str, ii ) )
	    delta = ii;
    }

    for( ii = b->keyC[start] - delta, jj = 0; jj < btmp.keyC[btmp.end_point];
	 ii++, jj++ ) {
	b->key[ii] = btmp.key[jj];
	b->str[ii] = btmp.str[jj];
    }
    b->key[ii] = b->str[ii] = '\0';

    for( ii = b->yomiC[start], jj = 0; jj < btmp.yomiC[btmp.end_point];
	 ii++, jj++ )
	b->yomi[ii] = btmp.yomi[jj];
    b->yomi[ii] = '\0';

    for( ii = start, jj = 0, start = b->keyC[start] - delta;
	 jj <= btmp.end_point; ii++, jj++ ) {
	b->keyC[ii] = btmp.keyC[jj] + start;
	b->yomiC[ii] = btmp.yomiC[jj] + b->yomiC[start];
	b->mode[ii] = btmp.mode[jj];
	b->conversion_flag[ii] = btmp.conversion_flag[jj];
    }
    b->end_point += btmp.end_point;

    b->conversion_mode = b->conversion_mode_toggle = btmp.conversion_mode;

    if( b->henkan->bun[b->henkan->point].srclen == 0 )
	bunsetsu_delete( b );
}


void	edit_henkan( b, w )
Buffer	*b;
Widget	w;
{
    int			ii, jj, delta;
    int			start, end, length;
    unsigned int	hp;
    Buffer		btmp;

    btmp = *b;
    edit_cancel( b );

    hp = b->henkan->point;

    switch( sj3_getdouon_wcs( btmp.yomi, &btmp.henkan->dou ) ) {
      case -1:
	server_error( b, w, resources.died_error );
	replace_bunsetsu( b, btmp.yomi, BUNSETSU_KANJI, True );
	break;

      default:
	replace_bunsetsu( b, btmp.henkan->dou[0].ddata, BUNSETSU_KANJI, True );
	break;
    }

    ii = b->henkan->bun[hp].srcstr - b->henkan->bun[0].srcstr;
    for( start = 0; b->yomiC[start] != ii; start++ );
    ii += b->henkan->bun[hp].srclen;
    for( end = start; b->yomiC[end] != ii; end++ );

    length = wslen( btmp.yomi );
    ii = length - b->henkan->bun[hp].srclen;
    b->henkan->bun[hp].srclen = length;
    while( b->henkan->bunsetsu[++hp] != BUNSETSU_END )
	b->henkan->bun[hp].srcstr += ii;

    for( ii = b->keyC[end]; ii < b->keyC[end+1]; ii++ )
	b->str[ii] = '\0';
    for( ii = btmp.keyC[btmp.end_point], jj = b->keyC[end];
	jj < b->keyC[b->end_point]; ii++, jj++ ) {
	btmp.key[ii] = b->key[jj];
	btmp.str[ii] = b->str[jj];
    }
    btmp.key[ii] = btmp.str[ii] = '\0';

    for( ii = btmp.yomiC[btmp.end_point], jj = b->yomiC[end];
	 jj < b->yomiC[b->end_point]; ii++, jj++ )
	btmp.yomi[ii] = b->yomi[jj];
    btmp.yomi[ii] = '\0';

    if( btmp.keyC[btmp.end_point-1] == btmp.keyC[btmp.end_point] )
	btmp.end_point--;
    if( btmp.keyC[btmp.end_point] == 0 )
	btmp.end_point = 0;
    for( ii = btmp.end_point, jj = end; jj <= b->end_point; ii++, jj++ ) {
	btmp.keyC[ii] = b->keyC[jj] - b->keyC[end] + btmp.keyC[btmp.end_point];
	btmp.yomiC[ii] = b->yomiC[jj] - b->yomiC[end] +
			 btmp.yomiC[btmp.end_point];
	btmp.mode[ii] = b->mode[jj];
	btmp.conversion_flag[ii] = b->conversion_flag[jj];
    }
    btmp.end_point += b->end_point - end;
    b->end_point = start;


    delta = 0;
    if( b->str[b->keyC[start]] ) {
	ii = strlen( (char *)&b->str[b->keyC[start]] );
	if( ii > b->keyC[start+1] - b->keyC[start] )
	    ii = b->keyC[start+1] - b->keyC[start];
	if( strncmp( (char *)&b->str[b->keyC[start]], (char *)btmp.str, ii ) )
	    delta = ii;
    }

    for( ii = b->keyC[start] - delta, jj = 0; jj < btmp.keyC[btmp.end_point];
	 ii++, jj++ ) {
	b->key[ii] = btmp.key[jj];
	b->str[ii] =btmp.str[jj];
    }
    b->key[ii] = b->str[ii] = '\0';

    for( ii = b->yomiC[start], jj = 0; jj < btmp.yomiC[btmp.end_point];
	 ii++, jj++ )
	b->yomi[ii] = btmp.yomi[jj];
    b->yomi[ii] = '\0';

    for( ii = start, jj = 0, start = b->keyC[start] - delta;
	 jj <= btmp.end_point; ii++, jj++ ) {
	b->keyC[ii] = btmp.keyC[jj] + start;
	b->yomiC[ii] = btmp.yomiC[jj] + b->yomiC[start];
	b->mode[ii] = btmp.mode[jj];
	b->conversion_flag[ii] = btmp.conversion_flag[jj];
    }
    b->end_point += btmp.end_point;

    b->conversion_mode = b->conversion_mode_toggle = btmp.conversion_mode;

    if( b->henkan->bun[b->henkan->point].srclen == 0 )
	bunsetsu_delete( b );
}
