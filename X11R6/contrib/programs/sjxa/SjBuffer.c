/* $SonyId: SjBuffer.c,v 1.1 1994/05/30 13:03:58 makoto Exp $ */
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
#include	<malloc.h>
#include	<X11/Xlib.h>
#include	<Xmw/Preedit.h>
#include	<Xmw/TextOut.h>
#include	"sjxa.h"
#include	"resource.h"
#include	"SjBuffer.h"
#include	"SjString.h"

static int	count = 0;
static Buffer	*stack[5];
Buffer		root_window;
Buffer		*buffer = &root_window;

static Boolean		flag = False;
static unsigned int	point;
static unsigned int	end_point[3];
static unsigned char	key[3][MAX_KEY];
static unsigned char	str[3][MAX_KEY];
static unsigned int	keyC[3][MAX_KEY];
static wchar_t		yomi[3][MAX_KANA];
static unsigned int	yomiC[3][MAX_KANA];
static ConversionMode	mode[3][MAX_KEY];
static Boolean		conversion_flag[3][MAX_KEY];
static unsigned int	cursor;

extern Widget	RootText;
extern Sjxa	resources;
extern int	preedit_nothing_count;


void
#if NeedFunctionPrototypes
sort_buffer( Buffer *b )
#else
sort_buffer( b )
    Buffer	*b;
#endif
{
    if( !b->previous )
	return;
    b->previous->next = b->next;
    if( b->next )
	b->next->previous = b->previous;
    b->previous = NULL;
    buffer->previous = b;
    b->next = buffer;
    buffer = b;
}


Buffer *
#if NeedFunctionPrototypes
set_buffer( Widget widget, int type )
#else /* NeedFunctionPrototypes */
set_buffer( widget, type )
    Widget	widget;
    int		type;
#endif /* NeedFunctionPrototypes */
{
    Buffer	*b;

    if( buffer->state & (ERROR | SERVER | ADD | DELETE) )
	return( buffer );

    if( (type == ButtonPress)  ||  (type == ButtonRelease) ) {
	for( b = buffer; b; b = b->next ) {
	    if( (b->conversion_style & XIMStatusNothing)  &&  b->connect ) {
		if( widget == b->box[0]  ||  widget == b->box[1] ) {
		    sort_buffer( b );
		    return( buffer );
		}
	    }
	}
    }
    for( b = buffer; b != NULL; b = b->next ) {
	if( widget == b->preedit_widget ) {
	    if( widget == root_window.preedit_widget  &&
		preedit_nothing_count == 0 )
		return( NULL );
	    sort_buffer( b );
	    break;
	}
    }
    return( buffer );
}


void
#if NeedFunctionPrototypes
buffer_initialize( Buffer *b )
#else /* NeedFunctionPrototypes */
buffer_initialize( b )
    Buffer	*b;
#endif /* NeedFunctionPrototypes */
{
    int			ii;

    b->point = 0;
    b->end_point = 1;
    for( ii = 0; ii < MAX_KEY; ii++ ) {
	b->key[ii] = '\0';
	b->keyC[ii] = 0;
	b->str[ii] = '\0';
    }
    for( ii = 0; ii < MAX_KANA; ii++ ) {
	b->yomi[ii] = '\0';
	b->yomiC[ii] = 0;
    }
    b->mode[0] = b->conversion_mode;
    b->conversion_flag[0] = False;
    b->cursor = 0;
    b->rt = NULL;
    b->state = INTERMEDIATE;

    if( b->henkan ) {
	sj3_freedouon_wcs( b->henkan->dou );
	free( b->henkan );
    }
    b->henkan = NULL;
}


Buffer *
#if NeedFunctionPrototypes
buffer_alloc( void )
#else
buffer_alloc()
#endif
{
    Buffer	*b;

    if( (b = (Buffer *)calloc( 1, sizeof(Buffer) )) == NULL )
	return( NULL );
    buffer_initialize( b );
    b->use_jisx0212 = False;
    b->jisx0208 = 0x0101;
    b->jisx0212 = 0x020f;
    b->conversion_mode = b->conversion_mode_toggle = resources.initial_mode;
    b->mode_lock = False;

    b->next = NULL;
    for( b->previous = buffer; b->previous->next;
	 b->previous = b->previous->next );
    b->previous->next = b;

    return( b );
}


void
#if NeedFunctionPrototypes
buffer_free( Buffer *b )
#else
buffer_free( b )
    Buffer	*b;
#endif
{
    if( b->previous ) {
	b->previous->next = b->next;
	if( b->next )
	    b->next->previous = b->previous;
    }
    else
	buffer = buffer->next;
    free( b );
}


void
#if NeedFunctionPrototypes
pop_buffer( Buffer *b )
#else
pop_buffer( b )
    Buffer	*b;
#endif
{
    if( count == 0 ) {
	fputs( "Irregular pop_buffer access.", stderr );
	exit( 1 );
    }
    *b = *stack[--count];
    free( stack[count] );
}    


void
#if NeedFunctionPrototypes
push_buffer( Buffer *b )
#else
push_buffer( b )
    Buffer	*b;
#endif
{
    if( count == 5 ) {
	fputs( "Irregular push_buffer access.", stderr );
	exit( 1 );
    }

    if( (stack[count] = (Buffer *)malloc( sizeof(Buffer) )) == NULL ) {
	fputs( "not enough memory", stderr );
	exit( 1 );
    }
    *stack[count++] = *b;
}


void
#if NeedFunctionPrototypes
saihenkan( Widget w, XEvent *event, String *params, Cardinal *n )
#else
saihenkan( w, event, params, n )
    Widget	 w;
    XEvent	*event;
    String	*params;
    Cardinal	*n;
#endif
{
    Buffer		*b;
    int			ii;
    unsigned int	cursor;

    if( !flag )
	return;
    b = set_buffer( w, event->type );
    if( b == NULL  ||  b->state != INTERMEDIATE )
	return;

    if( !check_action_args( params, *n, "reconversion" ) )
	return;

    if( wscmp( b->yomi, yomi[0] ) == 0 ) {
	b->point = end_point[1] - 1;
	b->end_point = end_point[1];
	for( ii = 0; ii < MAX_KEY; ii++ ) {
	    b->key[ii] = key[1][ii];
	    b->str[ii] = str[1][ii];
	    b->keyC[ii] = keyC[1][ii];
	    b->mode[ii] = mode[1][ii];
	    b->conversion_flag[ii] = conversion_flag[1][ii];
	}
	for( ii = 0; ii < MAX_KANA; ii++ ) {
	    b->yomi[ii] = yomi[1][ii];
	    b->yomiC[ii] = yomiC[1][ii];
	}
    }
    else {
	end_point[1] = b->end_point;
	b->point = end_point[0] - 1;
	b->end_point = end_point[0];
	for( ii = 0; ii < MAX_KEY; ii++ ) {
	    key[1][ii] = b->key[ii];
	    str[1][ii] = b->str[ii];
	    keyC[1][ii] = b->keyC[ii];
	    mode[1][ii] = b->mode[ii];
	    conversion_flag[1][ii] = b->conversion_flag[ii];

	    b->key[ii] = key[0][ii];
	    b->str[ii] = str[0][ii];
	    b->keyC[ii] = keyC[0][ii];
	    b->mode[ii] = mode[0][ii];
	    b->conversion_flag[ii] = conversion_flag[0][ii];
	}
	for( ii = 0; ii < MAX_KANA; ii++ ) {
	    yomi[1][ii] = b->yomi[ii];
	    yomiC[1][ii] = b->yomiC[ii];
	    b->yomi[ii] = yomi[0][ii];
	    b->yomiC[ii] = yomiC[0][ii];
	}
    }
    cursor = b->conversion_flag[b->point] ?
	b->yomiC[b->end_point-1] : b->yomiC[b->end_point];

    if( b->conversion_style & XIMPreeditNothing ) {
	XmwTextOutSetStringWcs( RootText, b->yomi, XmwHIGHLIGHT_NORMAL );
	for( ii = 0; ii <= b->point; ii++ )
	    if( !b->conversion_flag[ii] )
		XmwTextOutSetHighlight( RootText, b->yomiC[ii], b->yomiC[ii+1],
					XmwHIGHLIGHT_SECONDARY_SELECTED );
	XmwTextOutSetCursorPosition( RootText, cursor );
    }
    else if( b->conversion_style & XIMPreeditPosition )
	XmwPreeditSetStringWcs( b->preedit_widget, b->yomi,
			        XmwHIGHLIGHT_SECONDARY_SELECTED );
}


void
#if NeedFunctionPrototypes
register_buffer( Buffer *b )
#else
register_buffer( b )
    Buffer	*b;
#endif
{
    int		ii;

    flag = True;
    end_point[0] = b->end_point;
    for( ii = 0; ii < MAX_KEY; ii++ ) {
	key[0][ii] = b->key[ii];
	str[0][ii] = b->str[ii];
	keyC[0][ii] = b->keyC[ii];
	mode[0][ii] = b->mode[ii];
	conversion_flag[0][ii] = b->conversion_flag[ii];
    }
    for( ii = 0; ii < MAX_KANA; ii++ ) {
	yomi[0][ii] = b->yomi[ii];
	yomiC[0][ii] = b->yomiC[ii];
    }
}


void
#if NeedFunctionPrototypes
save_buffer( Buffer *b )
#else
save_buffer( b )
    Buffer	*b;
#endif
{
    int		ii;

    point = b->point;
    end_point[2] = b->end_point;
    for( ii = 0; ii < MAX_KEY; ii++ ) {
	key[2][ii] = b->key[ii];
	str[2][ii] = b->str[ii];
	keyC[2][ii] = b->keyC[ii];
	mode[2][ii] = b->mode[ii];
	conversion_flag[2][ii] = b->conversion_flag[ii];
    }
    for( ii = 0; ii < MAX_KANA; ii++ ) {
	yomi[2][ii] = b->yomi[ii];
	yomiC[2][ii] = b->yomiC[ii];
    }
    cursor = b->cursor;
}


void
#if NeedFunctionPrototypes
load_buffer( Buffer *b )
#else
load_buffer( b )
    Buffer	*b;
#endif
{
    int		ii;

    b->point = point;
    b->end_point = end_point[2];
    for( ii = 0; ii < MAX_KEY; ii++ ) {
	b->key[ii] = key[2][ii];
	b->str[ii] = str[2][ii];
	b->keyC[ii] = keyC[2][ii];
	b->mode[ii] = mode[2][ii];
	b->conversion_flag[ii] = conversion_flag[2][ii];
    }
    for( ii = 0; ii < MAX_KANA; ii++ ) {
	b->yomi[ii] = yomi[2][ii];
	b->yomiC[ii] = yomiC[2][ii];
    }
    b->cursor = cursor;
}
