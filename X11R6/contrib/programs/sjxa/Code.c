/* $SonyId: Code.c,v 1.2 1994/06/03 06:33:04 makoto Exp $ */
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
#include	<X11/Xlib.h>
#include	<X11/Intrinsic.h>
#include	<X11/StringDefs.h>
#include	<X11/Shell.h>
#include	<X11/Xaw/Form.h>
#include	<Xmw/List.h>
#include	<Xmw/Label.h>
#include	<Xmw/Preedit.h>
#include	<Xmw/Util.h>
#include	"sjxa.h"
#include	"resource.h"
#include	"SjBuffer.h"
#include	"SjString.h"

extern char		*program_name;
extern Sjxa		resources;
extern LcCtype		user_locale;
extern Widget		RootText;
extern int		*depths;

typedef struct {
    unsigned short	start;
    unsigned short	end;
} EscapeCode;

static EscapeCode	escape0208[] = {
    { 0x222f, 0x2239 }, { 0x2242, 0x2249 }, { 0x2251, 0x225b },
    { 0x226b, 0x2271 }, { 0x227a, 0x227d }, { 0x2321, 0x232f },
    { 0x233a, 0x2340 }, { 0x235b, 0x2360 }, { 0x237b, 0x237e },
    { 0x2474, 0x247e }, { 0x2577, 0x257e }, { 0x2639, 0x2640 },
    { 0x2659, 0x267e }, { 0x2742, 0x2750 }, { 0x2772, 0x277e },
    { 0x2841, 0x2f7e }, { 0x4f54, 0x4f7e }, /*{ 0x7425, 0x747e },*/
    { 0x0000, 0x0000 }};

static EscapeCode	escape0212[] = {
    { 0x2121, 0x222e }, { 0x223a, 0x2241 }, { 0x2245, 0x226a },
    { 0x2272, 0x2660 }, { 0x2666, 0x2666 }, { 0x2668, 0x2668 },
    { 0x266b, 0x266b }, { 0x266d, 0x2670 }, { 0x267d, 0x2741 },
    { 0x274f, 0x2771 }, { 0x2821, 0x287e }, { 0x2923, 0x2923 },
    { 0x2925, 0x2925 }, { 0x2927, 0x2927 }, { 0x292a, 0x292a },
    { 0x292e, 0x292e }, { 0x2931, 0x2940 }, { 0x2949, 0x297e },
    { 0x2a39, 0x2a39 }, { 0x2a78, 0x2a7e }, { 0x2b3c, 0x2b3c },
    { 0x2b44, 0x2b44 }, { 0x2b78, 0x2f7e }, /*{ 0x6d64, 0x6d7f },*/
    { 0x0000, 0x0000 }};


static void
#if NeedFunctionPrototypes
rewrite_codes( Buffer *b )
#else /* NeedFunctionPrototypes */
rewrite_codes( b )
    Buffer	*b;
#endif /* NeedFunctionPrototypes */
{
    char		ss[256];
    unsigned short	code;
    unsigned char	hi, low;

    if( b->state & JISX0208 ) {
	code = b->jisx0208;
	hi = code >> 8;
	low = code & 0xff;
	sprintf( ss, "%s %02d%02d  %s %04x  %s %04x  %s %04x",
		 resources.code_kuten, hi, low,
		 resources.code_jis, kuten2jis(code),
		 resources.code_euc, kuten2euc(code),
		 resources.code_sjis, kuten2sjis(code) );
    }
    else {
	code= b->jisx0212;
	hi = code >> 8;
	low = code & 0xff;
	sprintf( ss, "%s %02d%02d  %s %04x  %s %04x",
		 resources.code_kuten, hi, low,
		 resources.code_jis, kuten2jis(code),
		 resources.code_euc, kuten2euc(code) );
    }
    XtVaSetValues( b->code_label, XtNlabel, ss, NULL );
}


static unsigned char
#if NeedFunctionPrototypes
code_correct( State state, unsigned char hi )
#else /* NeedFunctionPrototypes */
code_correct( state, hi )
    State		state;
    unsigned char	hi;
#endif /* NeedFunctionPrototypes */
{
    if( state & JISX0208 ) {
	if( hi >= 9 )
	    hi += 7;
    }
    else {
	if( hi >= 7 )
	    hi += 9;
	else if( hi >= 4 )
	    hi += 5;
	else if( hi >= 2 )
	    hi += 4;
	else if( hi >= 1 )
	    hi += 1;
    }
    return( hi );
}


void
#if NeedFunctionPrototypes
code_cancel( Widget w, caddr_t client_data, caddr_t call_data )
#else /* NeedFunctionPrototypes */
code_cancel( w, client_data, call_data )
    Widget	w;
    caddr_t	client_data, call_data;
#endif /* NeedFunctionPrototypes */
{
    Buffer	*b = (Buffer *)client_data;

    XtDestroyWidget( b->shell );
    b->shell = NULL;
    b->state &= ~CODE;
}


static void
#if NeedFunctionPrototypes
activate( Widget w, caddr_t client_data, caddr_t call_data )
#else /* NeedFunctionPrototypes */
activate( w, client_data, call_data )
    Widget	w;
    caddr_t	client_data, call_data;
#endif /* NeedFunctionPrototypes */
{
    XmwListCallback	 lc = (XmwListCallback)call_data;
    Buffer		*b = (Buffer *)client_data;
    unsigned char	 hi, low;
    unsigned short	 kanji;
    char		 mb[10];
    wchar_t		 wc;

    hi = code_correct( b->state, lc->item / 96 + 1 );
    low = lc->item % 96;

    if( b->state & JISX0208 )
	code_kettei( b, kuten2wc( (hi << 8) | low) );
    else {
	kanji = kuten2euc( (hi << 8) | low );
	mb[0] = (char)SS3;
	mb[1] = kanji >> 8;
	mb[2] = kanji & 0xff;
	mbtowc( &wc, mb, MB_CUR_MAX );
	code_kettei( b, wc );
    }
}


static void
#if NeedFunctionPrototypes
browse_selection( Widget w, caddr_t client_data, caddr_t call_data )
#else /* NeedFunctionPrototypes */
browse_selection( w, client_data, call_data )
    Widget	w;
    caddr_t	client_data, call_data;
#endif /* NeedFunctionPrototypes */
{
    XmwListCallback	 lc = (XmwListCallback)call_data;
    Buffer		*b = (Buffer *)client_data;
    unsigned short	 code;

    code = (code_correct( b->state, lc->item / 96 + 1 ) << 8)
	 | (lc->item % 96);
    if( b->state & JISX0208 )
	b->jisx0208 = code;
    else
	b->jisx0212 = code;
    rewrite_codes( b );
}


static Boolean
#if NeedFunctionPrototypes
isescape( State state, unsigned short code )
#else /* NeedFunctionPrototypes */
isescape( state, code )
    State		state;
    unsigned short	code;
#endif /* NeedFunctionPrototypes */
{
    int			ii;
    unsigned char	low;
    EscapeCode		*escape;

    low = code & 0xff;
    if( low > 0x7e  ||  low < 0x21 )
	return( True );
    escape = (state & JISX0208) ? escape0208 : escape0212;
    for( ii = 0; escape[ii].start != 0  &&  code >= escape[ii].start;
	ii++ )
	if( code <= escape[ii].end )
	    return( True );
    return( False );
}


static void
#if NeedFunctionPrototypes
CreateCodeSelectWidget( Buffer *b, Widget widget )
#else /* NeedFunctionPrototypes */
CreateCodeSelectWidget( b, widget )
    Buffer	*b;
    Widget	widget;
#endif /* NeedFunctionPrototypes */
{
    Widget		 form;
    int			 n;
    Arg			 args[20];
    unsigned char	 hi, low;
    unsigned long	 kanji;
    int			 selected_item;
    Pixel		 fg, bg, lbg;
    Colormap		 colormap;
    int			 depth;
    Screen		*screen;
    Dimension		 width, height;
    unsigned char	*item;

    XtVaGetValues( widget, XtNforeground, &fg, XtNbackground, &bg,
		   XtNcolormap, &colormap, XtNdepth, &depth,
		   XtNscreen, &screen, NULL );
    n = XScreenNumberOfScreen( screen );
    lbg = ( depths[n] == 1 ) ? bg
			     : light_color( XtDisplay(widget), colormap, bg );

    n = 0;
    XtSetArg( args[n], XtNtitle, "sjxa CodeDialog" );		n++;
    XtSetArg( args[n], XtNcolormap, colormap );			n++;
    XtSetArg( args[n], XtNdepth, depth );			n++;
    XtSetArg( args[n], XtNscreen, screen );			n++;
    XtSetArg( args[n], XtNiconic, False );			n++;
    XtSetArg( args[n], XtNgeometry, "+0+0" );			n++;
    XtSetArg( args[n], XtNmappedWhenManaged, False );		n++;
    XtSetArg( args[n], XtNallowShellResize, True );		n++;
    b->shell = XtAppCreateShell( program_name, CLASS_NAME,
				 topLevelShellWidgetClass, XtDisplay(widget),
				 args, n );

    n = 0;
    XtSetArg( args[n], XtNforeground, fg );			n++;
    XtSetArg( args[n], XtNbackground, bg );			n++;
    form = XtCreateManagedWidget( "Form", formWidgetClass, b->shell, args, n );

    n = 0;
    XtSetArg( args[n], XtNforeground, fg );			n++;
    XtSetArg( args[n], XtNbackground, bg );			n++;
    XtSetArg( args[n], XtNtop, XawChainTop );			n++;
    XtSetArg( args[n], XtNleft, XawChainLeft );			n++;
    XtSetArg( args[n], XtNright, XawChainRight );		n++;
    XtSetArg( args[n], XtNbottom, XawChainTop );		n++;
    XtSetArg( args[n], XtNborderWidth, 0 );			n++;
    b->code_label = XtCreateWidget( "CodeLabel", labelWidgetClass, form,
				    args, n );
    XtManageChild( b->code_label );

    rewrite_codes( b );

    item = (b->state & JISX0208) ? (unsigned char *)malloc( 96*(94-7)*2 )
				 : (unsigned char *)malloc( 96*(94-9)*3 );
    for( n = 0, hi = 1; hi <= 94; hi++ ) {
	if( b->state & JISX0208 ) {
	    if( hi == 9 )
		hi = 16;
	}
	else {
	    if( hi == 12 )
		hi = 16;
	    else if( hi == 8 )
		hi = 9;
	    else if( hi == 3 )
		hi = 6;
	    else if( hi == 1 )
		hi = 2;
	}
	for( low = 0x20; low <= 0x7f; low++, n++ ) {
	    kanji = ((hi + 0x20) << 8) | low;
	    if( isescape( b->state, (unsigned short)kanji ) )
		kanji = (unsigned long)NULL;
	    else {
		switch( user_locale ) {
		  case LC_CTYPE_SHIFTJIS:
		    kanji = jis2sjis(kanji);
		    break;

		  case LC_CTYPE_EUC:
		    kanji = jis2euc(kanji);
		    if( b->state & JISX0212 )
			kanji |= SS3 << 16;
		    break;

		  default:
		    kanji = 0;
		    break;
		}
	    }
	    if( b->state & JISX0208 ) {
		item[n*2+0] = kanji >> 8;
		item[n*2+1] = kanji & 0xff;
	    }
	    else {
		item[n*3+0] = kanji >> 16;
		item[n*3+1] = kanji >> 8;
		item[n*3+2] = kanji & 0xff;
	    }
	}
    }

    n = 0;
    XtSetArg( args[n], XtNborderColor, fg );			n++;
    XtSetArg( args[n], XtNforeground, fg );			n++;
    XtSetArg( args[n], XtNbackground, bg );			n++;
    XtSetArg( args[n], XtNfromVert, b->code_label );		n++;
    XtSetArg( args[n], XtNtop, XawChainTop );			n++;
    XtSetArg( args[n], XtNleft, XawChainLeft );			n++;
    XtSetArg( args[n], XtNright, XawChainRight );		n++;
    XtSetArg( args[n], XtNbottom, XawChainBottom );		n++;
    XtSetArg( args[n], XmwNitemBackground, lbg );		n++;
    XtSetArg( args[n], XmwNitems, item );			n++;
    XtSetArg( args[n], XmwNhighlightAlways, True );		n++;
    if( b->state & JISX0208 ) {
	hi = b->jisx0208 >> 8;
	low = b->jisx0208 & 0xff;
	XtSetArg( args[n], XmwNitemLength, 96*(94-7)*2 );	n++;
	XtSetArg( args[n], XmwNitemCount, 96*(94-7) );		n++;
	selected_item = (hi - 1) * 96 + low;
	if( hi >= 16 )
	    selected_item -= 96 * 7;
    }
    else {
	hi = b->jisx0212 >> 8;
	low = b->jisx0212 & 0xff;
	XtSetArg( args[n], XmwNitemLength, 96*(94-9)*3 );	n++;
	XtSetArg( args[n], XmwNitemCount, 96*(94-9) );		n++;
	selected_item = (hi - 1) * 96 + low;
	if( hi >= 16 )
	    selected_item -= 96 * 9;
	else if( hi >= 9 )
	    selected_item -= 96 * 5;
	else if( hi >= 6 )
	    selected_item -= 96 * 4;
	else if( hi >= 2 )
	    selected_item -= 96 * 1;
    }
    XtSetArg( args[n], XmwNselectedItem, selected_item );	n++;
    XtSetArg( args[n], XmwNnumRows, 16 );			n++;
    XtSetArg( args[n], XmwNvisibleHorizontalItemCount, 16 );	n++;
    XtSetArg( args[n], XmwNvisibleVerticalItemCount, 6 );	n++;
    XtSetArg( args[n], XtNresizable, True );			n++;
    b->list_widget = XmwCreateScrolledList( form, "List", args, n );
    XtAddCallback( b->list_widget, XmwNsetCallback, (XtCallbackProc)activate,
		   (caddr_t)b );
    XtAddCallback( b->list_widget, XmwNbrowseSelectionCallback,
		   (XtCallbackProc)browse_selection, (caddr_t)b );
    XtManageChild( b->list_widget );

    XtRealizeWidget( b->shell );

    /* Xaw BUG */
    {
	Widget  widget;

	widget = XtNameToWidget( XtParent(b->list_widget), "vertical" );
	if( widget )
	    XtVaSetValues( widget, XtNforeground, fg, NULL );
    }

    /* XXX */
    XtVaGetValues( b->shell, XtNwidth, &width, XtNheight, &height, NULL );
    XtVaSetValues( b->shell, XtNminWidth, width, XtNminHeight, height,
		   XtNmaxWidth, width, NULL );
    /* XXX */

    if( b->conversion_style & (XIMPreeditPosition | XIMPreeditArea) )
	XmwPreeditMoveWidget( b->preedit_widget, b->yomiC[b->point], b->shell );
    else
	MoveWidget( b->shell );

    XmwInstallTranslations( b->list_widget, b->preedit_widget );
    XmwInstallTranslations( form, b->preedit_widget );
    XtAddGrab( b->shell, False, False );

    free( item );
}


void
#if NeedFunctionPrototypes
jisx0208( Widget w, XEvent *event, String *params, Cardinal *n )
#else /* NeedFunctionPrototypes */
jisx0208( w, event, params, n )
    Widget	w;
    XEvent	*event;
    String	*params;
    Cardinal	*n;
#endif /* NeedFunctionPrototypes */
{
    Buffer	*b;

    if( (b = set_buffer( w, event->type )) == NULL )
	return;
    if( (b->state & ERROR)  ||  b->shell )
	return;
    if( !check_action_args( params, *n, "jisx0208" ) )
	return;

    if( b->state & (INTERMEDIATE | EDIT) ) {
	b->state |= JISX0208;
	CreateCodeSelectWidget( b, (b->preedit_widget == RootText) ?
				XtParent(RootText) : b->preedit_widget );
    }
}


void
#if NeedFunctionPrototypes
jisx0212( Widget w, XEvent *event, String *params, Cardinal *n )
#else /* NeedFunctionPrototypes */
jisx0212( w, event, params, n )
    Widget	w;
    XEvent	*event;
    String	*params;
    Cardinal	*n;
#endif /* NeedFunctionPrototypes */
{
    Buffer	*b;

    if( (b = set_buffer( w, event->type )) == NULL )
	return;
    if( (b->state & ERROR)  ||  b->shell )
	return;
    if( !check_action_args( params, *n, "jisx0212" ) )
	return;

    if( b->state & (INTERMEDIATE | EDIT) ) {
	if( !b->use_jisx0212 ) {
	    server_error( b, w, resources.supplementary_kanji_error );
	    return;
	}
	b->state |= JISX0212;
	CreateCodeSelectWidget( b, (b->preedit_widget == RootText) ?
				XtParent(RootText) : b->preedit_widget );
    }
}

