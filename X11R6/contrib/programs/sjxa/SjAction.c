/* $SonyId: SjAction.c,v 1.1 1994/05/30 13:03:58 makoto Exp $ */
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
#include	<ctype.h>
#include	<X11/Xlib.h>
#include	<X11/Intrinsic.h>
#include	<X11/StringDefs.h>
#include	<X11/keysym.h>
#include	<X11/Xatom.h>
#include	<Xmw/List.h>
#include	<Xmw/Preedit.h>
#include	<Xmw/TextOut.h>
#include	"sjxa.h"
#include	"resource.h"
#include	"SjBuffer.h"
#include	"SjRk.h"
#include	"SjString.h"


extern Buffer	*buffer;
extern Sjxa	resources;
extern Widget	RootTextField;
extern Widget	TourokuText[], Touroku;
extern Widget	ServerText, Server;
extern Widget	Box[];
extern LcCtype	user_locale;

static void	bs();
void		cancel();
static void	del();
static void	down();
static void	end();
static void	next();
static void	previous();
static void	scroll_down();
static void	scroll_up();
void		sjxa_connect();
void		sjxa_disconnect();
static void	through();
static void	toggle();
static void	top();
static void	up();

extern void	alpha();		/* mode.c	*/
extern void	code();			/* mode.c	*/
extern void	jisx0208();		/* Code.c	*/
extern void	jisx0212();		/* Code.c	*/
extern void	edit();			/* edit.c	*/
extern void	hankaku();		/* mode.c	*/
extern void	hankaku_alpha();	/* mode.c	*/
extern void	hankaku_katakana();	/* mode.c	*/
extern void	henkan();		/* henkan.c	*/
extern void	hiragana();		/* mode.c	*/
extern void	initial_mode();		/* mode.c	*/
extern void	inkey();		/* inkey.c	*/
extern void	kakucyou();		/* bunsetsu.c	*/
extern void	katakana();		/* mode.c	*/
extern void	kettei();		/* kettei.c	*/
extern void	mode_lock();		/* mode.c	*/
extern void	muhenkan();		/* mode.c	*/
extern void	reconnect();		/* Reconnect.c	*/
extern void	saihenkan();		/* buffer.c	*/
extern void	sakujo();		/* Sakujo.c	*/
extern void	syukusyou();		/* bunsetsu.c	*/
extern void	touroku();		/* Touroku.c	*/
extern void	wrapback();		/* henkan.c	*/
extern void	zenkaku();		/* mode.c	*/
extern void	zenkaku_alpha();	/* mode.c	*/
extern void	zenkaku_hiragana();	/* mode.c	*/
extern void	zenkaku_katakana();	/* mode.c	*/


static	XtActionsRec	actionTable[] = {
    { "add-term",			touroku			},
    { "alphanumeric",			alpha			},
    { "cancel",				cancel			},
    { "code-input",			code			},
    { "jisx0208",			jisx0208		},
    { "jisx0212",			jisx0212		},
    { "connect",			sjxa_connect		},
    { "conversion",			henkan			},
    { "commit",				kettei			},
    { "delete",				del			},
    { "delete-term",			sakujo			},
    { "disconnect",			sjxa_disconnect		},
    { "divided-delete",			bs			},
    { "down",				down			},
    { "edit",				edit			},
    { "em-alphanumeric",		zenkaku_alpha		},
    { "em-hiragana",			zenkaku_hiragana	},
    { "em-katakana",			zenkaku_katakana	},
    { "en-alphanumeric",		hankaku_alpha		},
    { "en-katakana",			hankaku_katakana	},
    { "end",				end			},
    { "expand",				kakucyou		},
    { "hankaku",			hankaku			},
    { "hiragana",			hiragana		},
    { "initial-mode",			initial_mode		},
    { "inkey",				inkey			},
    { "input-mode-lock",		mode_lock		},
    { "katakana",			katakana		},
    { "next",				next			},
    { "no-conversion",			muhenkan		},
    { "previous",			previous		},
    { "reconnect",			reconnect		},
    { "reconversion",			saihenkan		},
    { "reduce",				syukusyou		},
    { "scroll-down",			scroll_down		},
    { "scroll-up",			scroll_up		},
    { "through",			through			},
    { "toggle",				toggle			},
    { "top",				top			},
    { "up",				up			},
    { "wrapback",			wrapback		},
    { "zenkaku",			zenkaku			}};

#ifdef DEBUG
static void
action_hook( w, client_data, action_name, event, params, num_params )
    Widget	w;
    XtPointer	client_data;
    String	action_name;
    XEvent	*event;
    String	*params;
    Cardinal	*num_params;
{
    int	n;

    printf( "action_name = \"%s\"\n", action_name );
    for( n = 0; n < *num_params; n++ )
	printf( "params[%d] = \"%s\"\n", n, params[n] );
}
#endif


void
#if NeedFunctionPrototypes
AddActions( XtAppContext app )
#else /* NeedFunctionPrototypes */
AddActions( app )
    XtAppContext	app;
#endif /* NeedFunctionPrototypes */
{
    XtAppAddActions( app, actionTable, XtNumber( actionTable ) );
#ifdef DEBUG
    if( resources.debug & 0x1000 )
	XtAppAddActionHook( app, action_hook, NULL );
#endif
}


void
#if NeedFunctionPrototypes
function_activate( Widget w, caddr_t client_data, caddr_t call_data )
#else
function_activate( w, client_data, call_data )
    Widget	w;
    caddr_t	client_data, call_data;
#endif
{
    XmwListCallback	 lc = (XmwListCallback)call_data;
    Buffer		*b = (Buffer *)client_data;
    Cardinal		 n = 0;
    int			 item = lc->item;

    if( b->box[1] == w )
	item += 5;

    switch( item ) {
      case 0:	touroku( w, lc->event, NULL, NULL, &n );		break;
      case 1:	sakujo( w, lc->event, NULL, NULL, &n );			break;
      case 2:	edit( w, lc->event, NULL, NULL, &n );			break;
      case 3:	jisx0208( w, lc->event, NULL, NULL, &n );		break;
      case 4:	code( w, lc->event, NULL, NULL, &n );			break;
      case 5:	hankaku_alpha( w, lc->event, NULL, NULL, &n );		break;
      case 6:	zenkaku_alpha( w, lc->event, NULL, NULL, &n );		break;
      case 7:	hankaku_katakana( w, lc->event, NULL, NULL, &n );	break;
      case 8:	zenkaku_katakana( w, lc->event, NULL, NULL, &n );	break;
      case 9:	zenkaku_hiragana( w, lc->event, NULL, NULL, &n );	break;
    }
}


#define	ALL_STATE	(INIT | INTERMEDIATE | CONVERSION | EDIT | ADD \
			 | DELETE | SELECT | CODE | WRAP | SERVER |ERROR)

unsigned int
#if NeedFunctionPrototypes
check_action_args( String *params, int n, char *name )
#else /* NeedFunctionPrototypes */
check_action_args( params, n, name )
    String	*params;
    int		n;
    char	*name;
#endif /* NeedFunctionPrototypes */
{
    int			ii;
    State		state1 = 0;
    State		state2 = ALL_STATE;
    static struct {
	char	*name;
	State	flag;
    } string[] = {
	{ "init",		INIT		},
	{ "intermediate",	INTERMEDIATE	},
	{ "conversion",		CONVERSION	},
	{ "edit",		EDIT		},
	{ "add",		ADD		},
	{ "delete",		DELETE		},
	{ "select",		SELECT		},
	{ "code",		CODE		},
	{ "wrap",		WRAP		},
	{ "server",		SERVER		},
	{ "error",		ERROR		}
    };

#define	LEN	3

    while( n-- > 0 ) {
	for( ii = 0; ii < 11; ii++ ) {
	    if( !StrnCaseCmp( params[n], string[ii].name, LEN ) ) {
		state1 |= string[ii].flag;
		break;
	    }
	    else if( params[n][0] == '!'  ||  params[n][0] == '~' ) {
		if( !StrnCaseCmp( params[n]+1, string[ii].name, LEN ) ) {
		    state2 &= ~string[ii].flag;
		    break;
		}
	    }
	}
	if( ii == 11 )
	    fprintf( stderr, "SJXA Warning: Invalid state, %s() ignored.\n",
		     name );
    }
    if( state1 != 0  &&  state2 != ALL_STATE ) {
	fputs( "SJXA Warning: Invalid translation, translation ignored.",
	       stderr );
	return( False );
    }
    if( state1 == 0 )
	state1 = state2;

    if( buffer->state == INTERMEDIATE  &&  buffer->key[0] == '\0' )
	return( state1 & INIT );
    else if( buffer->error_shell )
	return( state1 & ERROR );
    else {
	state2 = buffer->state;
	if( state2 & (CODE | DELETE | CONVERSION) )
	    state2 &= CODE | DELETE | CONVERSION;
	return( state1 & state2 );
    }
}


void
#if NeedFunctionPrototypes
set_label( Buffer *b, ConversionMode mode )
#else /* NeedFunctionPrototypes */
set_label( b, mode )
    Buffer		*b;
    ConversionMode	 mode;
#endif /* NeedFunctionPrototypes */
{
    String	string;

    if( b->mode_lock ) {
	switch( mode ) {
	  case ZENKAKU_HIRAGANA:
	    string = resources.lock_zenkaku_hiragana;	break;
	  case ZENKAKU_KATAKANA:
	    string = resources.lock_zenkaku_katakana;	break;
	  case ZENKAKU_ALPHA:
	    string = resources.lock_zenkaku_alpha;	break;
	  case HANKAKU_HIRAGANA:
	  case HANKAKU_KATAKANA:
	    string = resources.lock_hankaku_katakana;	break;
	  case HANKAKU_ALPHA:
	    string = resources.lock_hankaku_alpha;	break;
	  default:
	    break;
	}
    }
    else {
	switch( mode ) {
	  case ZENKAKU_HIRAGANA:
	    string = resources.zenkaku_hiragana;	break;
	  case ZENKAKU_KATAKANA:
	    string = resources.zenkaku_katakana;	break;
	  case ZENKAKU_ALPHA:
	    string = resources.zenkaku_alpha;		break;
	  case HANKAKU_HIRAGANA:
	  case HANKAKU_KATAKANA:
	    string = resources.hankaku_katakana;	break;
	  case HANKAKU_ALPHA:
	    string = resources.hankaku_alpha;		break;
	  case SJIS:
	    string = resources.sjis;			break;
	  case EUC:
	    string = resources.euc;			break;
	  case JIS:
	    string = resources.jis;			break;
	  case KUTEN:
	    string = resources.kuten;			break;
	  case SUPP_EUC:
	    string = resources.supplementary_euc;	break;
          case SUPP_JIS:
	    string = resources.supplementary_jis;	break;
	  case SUPP_KUTEN:
	    string = resources.supplementary_kuten;	break;
	  default:
	    break;
	}
    }

    XtVaSetValues( b->label, XtNlabel, string, NULL );
    b->conversion_mode = b->conversion_mode_toggle = mode;
}


void
#if NeedFunctionPrototypes
set_highlight( Buffer *b )
#else /* NeedFunctionPrototypes */
set_highlight( b )
    Buffer	*b;
#endif /* NeedFunctionPrototypes */
{
    int			ii;
    unsigned int	sp;

    if( b->conversion_style & XIMPreeditNothing )
	XmwTextOutSetHighlight( b->preedit_widget, 0, wslen(b->henkan->kanji),
				XmwHIGHLIGHT_NORMAL );
    else if( b->conversion_style & (XIMPreeditPosition | XIMPreeditArea) )
	XmwPreeditSetHighlight( b->preedit_widget, 0, 10000,
			        XmwHIGHLIGHT_SECONDARY_SELECTED );
    for( ii = 0, sp = 0; ii < b->henkan->point; ii++ )
	sp += b->henkan->bun[ii].destlen;

    if( b->state & CONVERSION ) {
	if( b->conversion_style & XIMPreeditNothing ) {
	    XmwTextOutSetHighlight( b->preedit_widget, sp,
				    sp+b->henkan->bun[b->henkan->point].destlen,
				    XmwHIGHLIGHT_SELECTED );
	}
	else if( b->conversion_style & (XIMPreeditPosition | XIMPreeditArea) ) {
	    XmwPreeditSetCursorPosition( b->preedit_widget,
				sp+b->henkan->bun[b->henkan->point].destlen );
	    XmwPreeditSetCursorPosition( b->preedit_widget, sp );
	    XmwPreeditSetHighlight( b->preedit_widget, sp,
				    sp+b->henkan->bun[b->henkan->point].destlen,
				    XmwHIGHLIGHT_SELECTED );
	}
	switch( b->henkan->bunsetsu[b->henkan->point] & ~BUNSETSU_EDIT ) {
	  case BUNSETSU_ZENKAKU_HIRAGANA:
	    b->conversion_mode_toggle = ZENKAKU_HIRAGANA;
	    break;

	  case BUNSETSU_HANKAKU_HIRAGANA:
	    b->conversion_mode_toggle = HANKAKU_HIRAGANA;
	    break;

	  case BUNSETSU_ZENKAKU_KATAKANA:
	    b->conversion_mode_toggle = ZENKAKU_KATAKANA;
	    break;

	  case BUNSETSU_HANKAKU_KATAKANA:
	    b->conversion_mode_toggle = HANKAKU_KATAKANA;
	    break;

	  case BUNSETSU_ZENKAKU_ALPHA:
	    b->conversion_mode_toggle = ZENKAKU_ALPHA;
	    break;

	  case BUNSETSU_HANKAKU_ALPHA:
	    b->conversion_mode_toggle = HANKAKU_ALPHA;
	    break;

	  default:
	    b->conversion_mode_toggle = resources.initial_mode;
	}
    }
    else if( b->state & WRAP ) {
	if( b->conversion_style & XIMPreeditNothing )
	    XmwTextOutSetHighlight( b->preedit_widget, sp,
				 sp+b->henkan->dou[b->henkan->douon_point].dlen,
				 XmwHIGHLIGHT_SELECTED );
	else if( b->conversion_style & (XIMPreeditPosition | XIMPreeditArea) )
	    XmwPreeditSetHighlight( b->preedit_widget, sp,
				sp+b->henkan->dou[b->henkan->douon_point].dlen,
				XmwHIGHLIGHT_SELECTED );
	b->conversion_mode_toggle = resources.initial_mode;
    }
}


static void
#if NeedFunctionPrototypes
set_cursor_position( Buffer *b )
#else /* NeedFunctionPrototypes */
set_cursor_position( b )
    Buffer	*b;
#endif /* NeedFunctionPrototypes */
{
    Widget	w;
    Position	pos;

    if( b->state & SERVER )
	XmwTextOutSetCursorPosition( ServerText, b->cursor );
    else if( b->state & ADD )
	XmwTextOutSetCursorPosition( TourokuText[1], b->cursor );
    else if( b->conversion_style & XIMPreeditNothing ) {
	if( b->state & INTERMEDIATE )	w = b->preedit_widget;
	else if( b->state & EDIT )	w = RootTextField;
	XmwTextOutSetCursorPosition( w, b->cursor );
    }
    else if( b->conversion_style & (XIMPreeditPosition | XIMPreeditArea) ) {
	if( b->state & (INTERMEDIATE | CODE) )
	    XmwPreeditSetCursorPosition( b->preedit_widget, b->cursor );
	else if( b->state & EDIT ) {
	    pos = b->henkan->bun[b->henkan->point].deststr
		- b->henkan->kanji + b->cursor;
	    XmwPreeditSetCursorPosition( b->preedit_widget, pos );
	}
    }
}


static void
#if NeedFunctionPrototypes
through( Widget w, XKeyEvent *event, String *params, int *n )
#else /* NeedFunctionPrototypes */
through( w, event, params, n )
    Widget	w;
    XKeyEvent	*event;
    String	*params;
    int		*n;
#endif /* NeedFunctionPrototypes */
{
    Buffer	*b;
    Window	win;

    b = set_buffer( w, event->type );
    if( b == NULL  ||  (b->state & ~INTERMEDIATE)  ||
	!check_action_args( params, *n, "through" ) )
	return;
    if( b->key[0] != '\0' )
	return;

#ifdef USE_SJX
    SjxpSync( event, False );
#endif
    win = event->subwindow;
    SendKeyEvent( win, event );
}


static void
#if NeedFunctionPrototypes
next( Widget w, XEvent *event, String *params, Cardinal *n )
#else /* NeedFunctionPrototypes */
next( w, event, params, n )
    Widget	w;
    XEvent	*event;
    String	*params;
    Cardinal	*n;
#endif /* NeedFunctionPrototypes */
{
    Buffer		*b;
    unsigned int	p;
    Position		pos;
    int			ii;

    b = set_buffer( w, event->type );
    if( b == NULL  ||  (b->state & ERROR )  ||
	!check_action_args( params, *n, "next" ) )
	return;

    p = b->point;

    if( b->state & DELETE ) {
	XmwListNextItem( b->list_widget );
	return;
    }
    if( b->state & CODE ) {
	XmwListNextItem( b->list_widget );
	return;
    }
    if( b->state & SELECT ) {
	if( b->conversion_style & (XIMPreeditPosition | XIMPreeditArea) )
	    XmwListNextItem( b->list_widget );
	else
	    select_right( b );
	return;
    }
    if( (b->state & INTERMEDIATE)  &&  (b->mode_lock  ||  b->yomi[0] == NULL) )
	return;
    if( b->state & (INTERMEDIATE | ADD | EDIT | SERVER) ) {
	pos = b->cursor;
	if( b->yomiC[b->end_point] == pos ) {
	    XBell( event->xany.display, 0 );
	    return;
	}
	else {
	    if( b->yomiC[p] != pos )
		p = ++b->point;
	    if( b->conversion_flag[p] ) {
		do {
		    p = ++b->point;
		    b->cursor = b->yomiC[p];
		} while( b->str[b->keyC[p]] != '\0'  &&
			b->conversion_flag[p] );
		if( b->str[b->keyC[p]] != '\0'  &&
		   !b->conversion_flag[p] ) {
		    b->cursor = b->yomiC[p+1];
		    set_label( b, b->mode[p] );
		}
	    }
	    else if( p + 1 <= b->end_point ) {
		b->cursor = b->yomiC[p+1];
		set_label( b, b->mode[p] );
	    }
	    if( pos != b->cursor )
		set_cursor_position( b );
	}
    }
    else if( b->state & (WRAP | CONVERSION) ) {
	if( b->state & WRAP )
	    wrap_end( b, w, True );
	for( ii = 0, p = 0; ii <= b->henkan->point; ii++ )
	    p += b->henkan->bun[ii].srclen;
	if( b->yomi[p] == '\0' )
	    b->henkan->point = 0;
	else
	    b->henkan->point++;
	set_highlight( b );
    }
}


static void
#if NeedFunctionPrototypes
previous( Widget w, XEvent *event, String *params, Cardinal *n )
#else /* NeedFunctionPrototypes */
previous( w, event, params, n )
    Widget	w;
    XEvent	*event;
    String	*params;
    Cardinal	*n;
#endif /* NeedFunctionPrototypes */
{
    Buffer		*b;
    unsigned int	p;
    int			ii;

    b = set_buffer( w, event->type );
    if( b == NULL  ||  (b->state & ERROR)  ||
	!check_action_args( params, *n, "previous" ) )
	return;

    p = b->point;
    if( b->state & DELETE ) {
	XmwListPreviousItem( b->list_widget );
	return;
    }
    else if( b->state & CODE ) {
	XmwListPreviousItem( b->list_widget );
	return;
    }
    if( (b->state & INTERMEDIATE)  &&  (b->mode_lock  ||  b->yomi[0] == NULL) )
	return;
    if( b->state & (INTERMEDIATE | ADD | EDIT | SERVER) ) {
	if( b->cursor == 0 ) {
	    XBell( event->xany.display, 0 );
	    return;
	}
	else {
	    if( b->yomiC[p] == b->cursor )
		p = --b->point;
	    b->cursor = b->yomiC[p];
	    while( b->str[b->keyC[p]] != '\0' ) {
		p = --b->point;
		b->cursor = b->yomiC[p];
	    }
	    set_cursor_position( b );
	}

	if( p != 0  &&  !b->conversion_flag[p-1] ) {
	    b->point--;
	    set_label( b, b->mode[b->point] );
	}
    }
    else if( b->state & (WRAP | CONVERSION) ) {
	if( b->state & WRAP )
	    wrap_end( b, w, True );
	if( b->henkan->point != 0 )
	    b->henkan->point--;
	else {
	    ii = 1;
	    p = b->henkan->bun[0].srclen;
	    b->henkan->point = 0;
	    while( b->yomi[p] != '\0' ) {
		b->henkan->point++;
		p += b->henkan->bun[ii++].srclen;
	    }
	}
	set_highlight( b );
    }
    else if( b->state & SELECT ) {
	if( b->conversion_style & (XIMPreeditPosition | XIMPreeditArea) )
	    XmwListPreviousItem( b->list_widget );
	else
	    select_left( b );
    }
}


static void
#if NeedFunctionPrototypes
up( Widget w, XEvent *event, String *params, Cardinal *n )
#else /* NeedFunctionPrototypes */
up( w, event, params, n )
    Widget	w;
    XEvent	*event;
    String	*params;
    Cardinal	*n;
#endif /* NeedFunctionPrototypes */
{
    Buffer	*b;

    b = set_buffer( w, event->type );
    if( b == NULL  ||  (b->state & ERROR)  ||
	!check_action_args( params, *n, "up" ) )
	return;

    if( b->state & DELETE )
	XmwListPreviousItem( b->list_widget );
    else if( b->state & CODE )
	XmwListUpItem( b->list_widget );
    else if( b->state & SELECT ) {
	if( b->conversion_style & (XIMPreeditPosition | XIMPreeditArea) )
	    XmwListUpItem( b->list_widget );
	else
	    select_scroll_down( b );
    }
}


static void
#if NeedFunctionPrototypes
down( Widget w, XEvent *event, String *params, Cardinal *n )
#else /* NeedFunctionPrototypes */
down( w, event, params, n )
    Widget	w;
    XEvent	*event;
    String	*params;
    Cardinal	*n;
#endif /* NeedFunctionPrototypes */
{
    Buffer	*b;

    b = set_buffer( w, event->type );
    if( b == NULL  ||  (b->state & ERROR)  ||
	!check_action_args( params, *n, "down" ) )
	return;

    if( b->state & DELETE )
	XmwListNextItem( b->list_widget );
    else if( b->state & CODE )
	XmwListDownItem( b->list_widget );
    else if( b->state & SELECT ) {
	if( b->conversion_style & (XIMPreeditPosition | XIMPreeditArea) )
	    XmwListDownItem( b->list_widget );
	else
	    select_scroll_up( b );
    }
}


static void
#if NeedFunctionPrototypes
top( Widget w, XEvent *event, String *params, Cardinal *n )
#else /* NeedFunctionPrototypes */
top( w, event, params, n )
    Widget	w;
    XEvent	*event;
    String	*params;
    Cardinal	*n;
#endif /* NeedFunctionPrototypes */
{
    Buffer	*b;

    b = set_buffer( w, event->type );
    if( b == NULL  ||  (b->state & ERROR)  ||
	!check_action_args( params, *n, "top" ) )
	return;

    if( b->state & DELETE ) {
	XmwListTopItem( b->list_widget );
	return;
    }
    if( (b->state & INTERMEDIATE)  &&  (b->mode_lock  ||  b->yomi[0] == NULL) )
	return;
    if( b->state & (INTERMEDIATE | ADD | EDIT | CODE | SERVER) ) {
	b->point = 0;
	if( b->cursor != 0 ) {
	    b->cursor = 0;
	    set_cursor_position( b );
	}
    }
    else if( b->state & (WRAP | CONVERSION) ) {
	if( b->state & WRAP )
	    wrap_end( b, w, True );
	if( b->henkan->point ) {
	    b->henkan->point = 0;
	    set_highlight( b );
	}
    }
    else if( b->state & SELECT ) {
	if( !(b->conversion_style & (XIMPreeditPosition | XIMPreeditArea)) )
	    select_top( b );
    }
}


static void
#if NeedFunctionPrototypes
end( Widget w, XEvent *event, String *params, Cardinal *n )
#else /* NeedFunctionPrototypes */
end( w, event, params, n )
    Widget	w;
    XEvent	*event;
    String	*params;
    Cardinal	*n;
#endif
{
    Buffer		*b;
    int			ii;
    unsigned int	p, hp;

    b = set_buffer( w, event->type );
    if( b == NULL  ||  (b->state & ERROR)  ||
	!check_action_args( params, *n, "end" ) )
	return;

    if( b->state & DELETE ) {
	XmwListEndItem( b->list_widget );
	return;
    }
    if( (b->state & INTERMEDIATE)  &&  (b->mode_lock  ||  b->yomi[0] == NULL) )
	return;
    if( b->state & (INTERMEDIATE | ADD | EDIT | CODE | SERVER) ) {
	p = b->point = b->end_point - 1;
	if( b->yomiC[p] == b->yomiC[b->end_point] )
	    b->cursor = b->yomiC[p];
	else {
	    b->cursor = b->yomiC[b->end_point];
	    set_label( b, b->mode[p] );
	}
	set_cursor_position( b );
    }
    else if( b->state & (WRAP | CONVERSION) ) {
	if( b->state & WRAP )
	    wrap_end( b, w, True );
	hp = b->henkan->point;
	ii = 1;
	p = b->henkan->bun[0].srclen;
	b->henkan->point = 0;
	while( b->yomi[p] != '\0' ) {
	    b->henkan->point++;
	    p += b->henkan->bun[ii++].srclen;
	}
	if( hp != b->henkan->point )
	    set_highlight( b );
    }
    else if( b->state & SELECT ) {
	if( !(b->conversion_style & (XIMPreeditPosition | XIMPreeditArea)) )
	    select_end( b );
    }
}


static void
#if NeedFunctionPrototypes
scroll_up( Widget w, XEvent *event, String *params, Cardinal *n )
#else /* NeedFunctionPrototypes */
scroll_up( w, event, params, n )
    Widget	w;
    XEvent	*event;
    String	*params;
    Cardinal	*n;
#endif /* NeedFunctionPrototypes */
{
    Buffer	*b;

    b = set_buffer( w, event->type );
    if( b == NULL  ||  (b->state & (CONVERSION | ERROR))  ||
	!check_action_args( params, *n, "scroll-up" ) )
	return;

    if( b->state & SELECT ) {
	if( b->conversion_style & (XIMPreeditPosition | XIMPreeditArea) )
	    XmwListPageDownItem( b->list_widget );
	else
	    select_scroll_down( b );
    }
    else if( b->state & (CODE | DELETE) )
	XmwListPageDownItem( b->list_widget );
}


static void
#if NeedFunctionPrototypes
scroll_down( Widget w, XEvent *event, String *params, Cardinal *n )
#else /* NeedFunctionPrototypes */
scroll_down( w, event, params, n )
    Widget	w;
    XEvent	*event;
    String	*params;
    Cardinal	*n;
#endif /* NeedFunctionPrototypes */
{
    Buffer	*b;

    b = set_buffer( w, event->type );
    if( b == NULL  ||  (b->state & (CONVERSION | ERROR))  ||
	!check_action_args( params, *n, "scroll-down" ) )
	return;

    if( b->state & SELECT ) {
	if( b->conversion_style & (XIMPreeditPosition | XIMPreeditArea) )
	    XmwListPageUpItem( b->list_widget );
	else
	    select_scroll_up( b );
    }
    else if( b->state & (CODE | DELETE) )
	XmwListPageUpItem( b->list_widget );
}


static void
#if NeedFunctionPrototypes
toggle( Widget w, XEvent *event, String *params, Cardinal *n )
#else /* NeedFunctionPrototypes */
toggle( w, event, params, n )
    Widget	w;
    XEvent	*event;
    String	*params;
    Cardinal	*n;
#endif /* NeedFunctionPrototypes */
{
    Buffer	*b;

    b = set_buffer( w, event->type );
    if( b == NULL  ||  !check_action_args( params, *n, "toggle" ) )
	return;

    if( (b->state & INTERMEDIATE)  &&  b->mode_lock ) {
	XBell( XtDisplay(b->preedit_widget), 0 );
	return;
    }
    switch( b->conversion_mode_toggle ) {
      case ZENKAKU_HIRAGANA:
	zenkaku_katakana( w, event, params, n );
	break;

      case ZENKAKU_KATAKANA:
	hankaku_katakana( w, event, params, n );
	break;

      case HANKAKU_HIRAGANA:
      case HANKAKU_KATAKANA:
	zenkaku_alpha( w, event, params, n );
	break;

      case ZENKAKU_ALPHA:
	hankaku_alpha( w, event, params, n );
	break;

      case HANKAKU_ALPHA:
      default:
	zenkaku_hiragana( w, event, params, n );
	break;
    }
}


void
#if NeedFunctionPrototypes
cancel( Widget w, XEvent *event, String *params, Cardinal *n )
#else /* NeedFunctionPrototypes */
cancel( w, event, params, n )
    Widget	w;
    XEvent	*event;
    String	*params;
    Cardinal	*n;
#endif /* NeedFunctionPrototypes */
{
    Buffer		*b;
    int			ii;
    unsigned int	hp;
    Position		fp, tp;
    wchar_t		ws[256];

    b = set_buffer( w, event->type );
    if( b == NULL  ||  !check_action_args( params, *n, "cancel" ) )
	return;

    if( b->state & ERROR ) {
	error_cancel( (Widget)NULL, (caddr_t)b, (caddr_t)NULL );
    }
    else if( b->state & DELETE ) {
	sakujo_cancel( (Widget)NULL, (caddr_t)b, (caddr_t)NULL );
    }
    else if( b->state & ADD ) {
	touroku_cancel( (Widget)NULL, (caddr_t)b, (caddr_t)NULL );
    }
    else if( b->state & SELECT ) {
	if( b->conversion_style & (XIMPreeditPosition | XIMPreeditArea) ) {
	    select_dialog_cancel( NULL, b, NULL );
	}
	else {
	    hp = b->henkan->point;
	    if( b->henkan->bun[hp].deststr != NULL ) {
		for( ii = 0, fp = 0; ii < hp; ii++ )
		    fp += b->henkan->bun[ii].destlen;
		tp = fp + b->henkan->wrap_length;
		wscpy( ws, b->henkan->bun[hp].deststr );
		ws[b->henkan->bun[hp].destlen] = '\0';
		XmwTextOutReplaceWcs( b->preedit_widget, fp, tp, ws );
	    }
	    b->state = CONVERSION;
	    XtUnmapWidget( RootTextField );
	    XtMapWidget( Box[0] );
	    XtMapWidget( Box[1] );
	}
    }
    else if( b->state & SERVER ) {
	reconnect_cancel( (Widget)NULL, (caddr_t)b, (caddr_t)NULL );
    }
    else if( b->state & CODE ) {
	code_cancel( (Widget)NULL, (caddr_t)b, (caddr_t)NULL );
    }
    else if( b->state & EDIT ) {
	edit_cancel( b );
    }
    else if( b->state & (WRAP | CONVERSION) ) {
	henkan_cancel( b );
    }
    else if( b->state & INTERMEDIATE ) {
	if( b->keyC[1] == 0 )
	    Disconnect( event->xkey.display, event->xkey.subwindow );
	else {
	    XtVaSetValues( b->preedit_widget, XmwNdisplayCaret, True, NULL );
	    buffer_initialize( b );
	    if( b->conversion_style & XIMPreeditNothing )
		XmwTextOutSetString( b->preedit_widget, NULL,
				     XmwHIGHLIGHT_NORMAL );
	    else
		XmwPreeditSetString( b->preedit_widget, NULL,
				     XmwHIGHLIGHT_SECONDARY_SELECTED );
	}
    }
}


static void
#if NeedFunctionPrototypes
bs( Widget w, XEvent *event, String *params, Cardinal *n )
#else /* NeedFunctionPrototypes */
bs( w, event, params, n )
    Widget	w;
    XEvent	*event;
    String	*params;
    Cardinal	*n;
#endif /* NeedFunctionPrototypes */
{
    Buffer		*b;
    unsigned int	p, ep;
    unsigned char	s1[MAX_KEY];
    int			ii;
    RkTable		*rt;
    ConversionMode	mode;
    char		*ss;
    wchar_t		w1[MAX_KEY], w2[MAX_KEY];
    Widget		widget;

    b = set_buffer( w, event->type );
    if( b == NULL  ||  (b->state & ERROR)  ||
	!check_action_args( params, *n, "divided-delete" ) )
	return;

    if( b->state & SERVER ) {
	widget = ServerText;
    }
    else if( b->state & ADD ) {
	widget = TourokuText[1];
    }
    else if( b->state & EDIT ) {
	if( b->conversion_style & (XIMPreeditPosition | XIMPreeditArea) )
	    widget = b->preedit_widget;
	else
	    widget = RootTextField;
    }
    else if( b->state & (INTERMEDIATE | CODE) ) {
	widget = b->preedit_widget;
    }

    if( b->state & (INTERMEDIATE | ADD | EDIT | CODE | SERVER) ) {
	if( b->cursor == 0 )
	    return;

	p = b->point;
	ep = b->end_point;
	if( b->keyC[p] == b->keyC[ep] )
	    ep = --b->end_point;
	if( b->cursor == b->yomiC[p] )
	    p = --b->point;

	switch( mode = b->mode[p] ) {
	  case HANKAKU_ALPHA:
	    delete_word_buffer( b, widget );
	    if( p > 0  &&  !b->conversion_flag[p-1] ) {
		set_label( b, b->mode[p-1] );
		b->point--;
		if( b->keyC[p] == b->keyC[ep] )
		    b->end_point--;
	    }
	    break;

	  case ZENKAKU_ALPHA:
	  case ZENKAKU_HIRAGANA:
	  case ZENKAKU_KATAKANA:
	  case HANKAKU_HIRAGANA:
	  case HANKAKU_KATAKANA:
	    b->rt = NULL;
	    if( p != 0  &&  b->str[b->keyC[p+1]-1] != '\0' ) {
		delete_word_buffer( b, widget );
		if( b->keyC[p] == b->keyC[ep] )
		    b->end_point--;
		p = --b->point;
	    }
	    if( b->conversion_flag[p] ) {
		if( (ii = b->keyC[p+1] - b->keyC[p] - 1) == 0 ) {
		    delete_word_buffer( b, widget );
		    if( p > 0  &&  !b->conversion_flag[p-1] ) {
			set_label( b, b->mode[p-1] );
			b->point--;
			if( b->keyC[p] == b->keyC[p+1] )
			    b->end_point--;
		    }
		}
		else {
		    set_label( b, mode );
		    strncpy( (char *)s1, (char *)&b->key[b->keyC[p]], ii );
		    s1[ii] = '\0';
		    if( !(mode & (HANKAKU_HIRAGANA | HANKAKU_KATAKANA)) ) {
			sjiss2wcs( w2, s1, strlen( (char *)s1 ) );
			hankaku2zenkaku( w2, w1, mode );
		    }
		    else
			mbs2wcs( w1, (char *)s1, strlen( (char *)s1 ) );

		    delete_character_buffer( b, widget, w1, mode );

		    if( 0xa1 <= s1[0]  &&  s1[0] <= 0xdf ) { /*iskana*/
			b->conversion_flag[p] = True;
			p = ++b->point;
			if( p == ep ) {
			    b->end_point++;
			    b->keyC[ep+1] = b->keyC[ep];
			    b->yomiC[ep+1] = b->yomiC[ep];
			}
		    }
		    else {
			rt = alpha2hiraganaUpperS( s1 );
			if( rt != NULL  &&
			    strncmp( (char *)s1, (char *)rt->next->key,
				     strlen((char *)s1) ) == 0 )
			    b->rt = rt;
		    }
		}
	    }
	    else {
		if( (ii = b->keyC[p+1] - b->keyC[p] - 1) == 0 ) {
		    delete_word_buffer( b, widget );
		    if( p > 0  &&  !b->conversion_flag[p-1] ) {
			set_label( b, b->mode[p-1] );
			b->point--;
			if( b->keyC[p] == b->keyC[ep] )
			    b->end_point--;
		    }
		}
		else {
		    delete_character_buffer( b, widget, NULL, mode );
		    strncpy( (char *)s1, (char *)&b->key[b->keyC[p]], ii );
		    s1[ii] = '\0';
		    rt = alpha2hiraganaLowerD( s1 );
		    if( rt != NULL  &&
			strncmp( (char *)s1, (char *)rt->next->key, ii ) == 0 )
			b->rt = rt;
		}
	    }
	    break;

	  case SJIS:
	  case EUC:
	  case JIS:
	  case KUTEN:
	    if( b->conversion_flag[p] ) {
		set_label( b, mode );
		strncpy( (char *)s1, (char *)&b->key[b->keyC[p]], 3 );
		s1[3] = '\0';
		mbs2wcs( w2, (char *)s1, strlen( (char *)s1 ) );
		hankaku2zenkaku( w2, w1, b->mode[p] );
		delete_character_buffer( b, widget, w1, b->mode[p] );
	    }
	    else {
		if( (ii = b->keyC[p+1] - b->keyC[p] - 1) == 0 ) {
		    delete_word_buffer( b, widget );
		    if( p > 0  &&  !b->conversion_flag[p-1] ) {
			set_label( b, b->mode[p-1] );
			b->point--;
			if( b->keyC[p] == b->keyC[ep] )
			    b->end_point--;
		    }
		}
		else {
		    delete_character_buffer( b, widget, NULL, b->mode[p] );
		}
	    }
	    break;

	  default:
	    delete_word_buffer( b, widget );
	    if( p > 0  &&  !b->conversion_flag[p-1] ) {
		set_label( b, b->mode[p-1] );
		b->point--;
		if( b->keyC[p] == b->keyC[ep] )
		    b->end_point--;
	    }
	    break;
	}
    }

    if( b->state & SERVER ) {
	if( !strlen(ss = XmwTextOutGetString( widget )) ) {
	    XtVaSetValues( Server, XtNsensitive, False, NULL );
	}
	XtFree(ss);
    }
    else if( b->state & ADD ) {
	if( !strlen(ss = XmwTextOutGetString( widget )) ) {
	    XtVaSetValues( Touroku, XtNsensitive, False, NULL );
	}
	XtFree(ss);
    }
    set_cursor_position( b );
}


static void
#if NeedFunctionPrototypes
del( Widget w, XEvent *event, String *params, Cardinal *n )
#else /* NeedFunctionPrototypes */
del( w, event, params, n )
    Widget	w;
    XEvent	*event;
    String	*params;
    Cardinal	*n;
#endif /* NeedFunctionPrototypes */
{
    Buffer		*b;
    unsigned int	p, ep;
    unsigned char	s1[MAX_KANA];
    int			ii;
    RkTable		*rt;
    char		*ss;
    Widget		widget;

    b = set_buffer( w, event->type );
    if( b == NULL  ||  (b->state & ERROR)  ||
	!check_action_args( params, *n, "delete" ) )
	return;

    if( b->state & SERVER )
	widget = ServerText;
    else if( b->state & (INTERMEDIATE | CODE) )
	widget = b->preedit_widget;
    else if( b->state & ADD )
	widget = TourokuText[1];
    else if( b->state & EDIT )
	widget = RootTextField;

    if( b->state & (INTERMEDIATE | ADD | EDIT | CODE | SERVER) ) {
	if( b->cursor == 0 )
	    return;

	p = b->point;
	ep = b->end_point;
	if( b->keyC[p] == b->keyC[ep] )
	    ep = --b->end_point;
	if( b->cursor == b->yomiC[p] )
	    p = --b->point;

	switch( b->mode[p] & ~EXTEND ) {
	  case HANKAKU_ALPHA:
	  case ZENKAKU_ALPHA:
	    delete_word_buffer( b, widget );
	    if( p > 0  &&  !b->conversion_flag[p-1] ) {
		set_label( b, b->mode[p-1] );
		b->point--;
		if( b->keyC[p] == b->keyC[ep] )
		    b->end_point--;
	    }
	    break;

	  case ZENKAKU_HIRAGANA:
	  case HANKAKU_HIRAGANA:
	  case ZENKAKU_KATAKANA:
	  case HANKAKU_KATAKANA:
	    b->rt = NULL;
	    if( p != 0  &&  b->str[b->keyC[p+1]-1] != '\0' ) {
		delete_word_buffer( b, widget );
		if( b->keyC[p] == b->keyC[ep] )
		    b->end_point--;
		p = --b->point;
	    }
	    if( b->conversion_flag[p] ) {
		delete_word_buffer( b, widget );
		if( p > 0  &&  !b->conversion_flag[p-1] ) {
		    set_label( b, b->mode[p-1] );
		    b->point--;
		    if( b->keyC[p] == b->keyC[p+1] )
			b->end_point--;
		}
	    }
	    else {
		if( (ii = b->keyC[p+1] - b->keyC[p] - 1) == 0 ) {
		    delete_word_buffer( b, widget );
		    if( p > 0  &&  !b->conversion_flag[p-1] ) {
			set_label( b, b->mode[p-1] );
			b->point--;
			if( b->keyC[p] == b->keyC[ep] )
			    b->end_point--;
		    }
		}
		else {
		    delete_character_buffer( b, widget, NULL, b->mode[p] );
		    strncpy( (char *)s1, (char *)&b->key[b->keyC[p]], ii );
		    s1[ii] = '\0';
		    rt = alpha2hiraganaLowerD( s1 );
		    if( rt != NULL  &&
			strncmp( (char *)s1, (char *)rt->next->key, ii ) == 0 )
			b->rt = rt;
		}
	    }
	    break;

	  case SJIS:
	  case EUC:
	  case JIS:
	  case KUTEN:
	    if( b->conversion_flag[p] ) {
		delete_word_buffer( b, widget );
		if( p > 0  &&  !b->conversion_flag[p-1] ) {
		    set_label( b, b->mode[p-1] );
		    b->point--;
		    if( b->keyC[p] == b->keyC[ep] )
			b->end_point--;
		}
	    }
	    else {
		if( (ii = b->keyC[p+1] - b->keyC[p] - 1) == 0 ) {
		    delete_word_buffer( b, widget );
		    if( p > 0  &&  !b->conversion_flag[p-1] ) {
			set_label( b, b->mode[p-1] );
			b->point--;
			if( b->keyC[p] == b->keyC[ep] )
			    b->end_point--;
		    }
		}
		else {
		    delete_character_buffer( b, widget, NULL, b->mode[p] );
		}
	    }
	    break;
	}
    }

    if( b->state & SERVER ) {
	if( !strlen(ss = XmwTextOutGetString( widget )) ) {
	    XtVaSetValues( Server, XtNsensitive, False, NULL );
	}
	XtFree(ss);
    }
    else if( b->state & ADD ) {
	if( !strlen(ss = XmwTextOutGetString( widget )) ) {
	    XtVaSetValues( Touroku, XtNsensitive, False, NULL );
	}
	XtFree(ss);
    }
    set_cursor_position( b );
}


void
#if NeedFunctionPrototypes
sjxa_connect( Widget w, XEvent *event, String *params, Cardinal *n )
#else /* NeedFunctionPrototypes */
sjxa_connect( w, event, params, n )
    Widget	w;
    XEvent	*event;
    String	*params;
    Cardinal	*n;
#endif /* NeedFunctionPrototypes */
{
}


void
#if NeedFunctionPrototypes
sjxa_disconnect( Widget w, XEvent *event, String *params, Cardinal *n )
#else /* NeedFunctionPrototypes */
sjxa_disconnect( w, event, params, n )
    Widget	w;
    XEvent	*event;
    String	*params;
    Cardinal	*n;
#endif /* NeedFunctionPrototypes */
{
    Buffer	*b;

    b = set_buffer( w, event->type );
    if( b == NULL  ||  !check_action_args( params, *n, "disconnect" ) )
	return;

    Disconnect( event->xany.display, event->xkey.subwindow );
}
