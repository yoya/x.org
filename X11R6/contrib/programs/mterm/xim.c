/* $Header: /var/home/sm/ws/michael/work/xterm/RCS/xim.c,v 1.1 1994/06/07 00:05:30 michael Exp $ */
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
#include	<X11/Xlib.h>
#include	<X11/StringDefs.h>
#include	"ptyx.h"
#include	"data.h"

extern char *ProgramName;
extern char *ClassName;

#ifdef USE_XIM
SetPreedit(screen)
    register TScreen *screen;
{
    XVaNestedList list;
    XRectangle area;
    XPoint spot;

    if (screen->im_style & XIMPreeditPosition) {
	spot.x = CursorX(screen, screen->cur_col);
	spot.y = CursorY(screen, screen->cur_row) + screen->max_ascent + screen->top_space;
	area.x = screen->scrollbar;
	area.y = 0;
	area.width = Width(screen);
	area.height = Height(screen);
	list = XVaCreateNestedList(NULL,
				   XNSpotLocation, &spot,
				   XNArea, &area,
				   XNLineSpace, FontHeight(screen),
				   NULL);
	XSetICValues(screen->ic, XNPreeditAttributes, list, NULL);
	XFree(list);
    }
}

SetStatus(screen)
    register TScreen *screen;
{
    XVaNestedList list;
    XRectangle area, *need_area = NULL;

    if (screen->im_style & XIMStatusArea) {
	area.x = screen->scrollbar;
	area.y = FullHeight(screen) - screen->im_statusheight - screen->border;
	if (screen->im_style & XIMPreeditArea) {
	    list = XVaCreateNestedList(NULL,
				       XNAreaNeeded, &need_area,
				       NULL);
	    XGetICValues(screen->ic, XNStatusAttributes, list, NULL);
	    XFree(list);
	    area.width = need_area->width;
	    if (screen->im_statusheight < need_area->height) {
		screen->im_statusheight = need_area->height + 2;
		DoResizeScreen(term);
	    }
	} else {
	    area.width = Width(screen);
	}
	area.height = screen->im_statusheight;
	list = XVaCreateNestedList(NULL,
				   XNArea, &area,
				   XNLineSpace, FontHeight(screen),
				   NULL);
	XSetICValues(screen->ic, XNStatusAttributes, list, NULL);
	XFree(list);
    }
    if (screen->im_style & XIMPreeditArea) {
	if (screen->im_style & XIMStatusArea) {
	    area.x = screen->scrollbar + need_area->width;
	    area.width = Width(screen) - need_area->width;
	} else {
	    area.x = screen->scrollbar;
	    area.width = Width(screen);
	}
	area.y = FullHeight(screen) - screen->im_statusheight - screen->border;
	area.height = screen->im_statusheight;
	list = XVaCreateNestedList(NULL,
				   XNArea, &area,
				   XNLineSpace, FontHeight(screen),
				   NULL);
	XSetICValues(screen->ic, XNPreeditAttributes, list, NULL);
	XFree(list);
    } else if (screen->im_style & XIMPreeditPosition) {
	area.x = screen->scrollbar;
	area.y = 0;
	area.width = Width(screen);
	area.height = Height(screen);
	list = XVaCreateNestedList(NULL,
				   XNArea, &area,
				   XNLineSpace, FontHeight(screen),
				   NULL);
	XSetICValues(screen->ic, XNPreeditAttributes, list, NULL);
	XFree(list);
    }
    if (need_area) {
	XFree(need_area);
    }
}

static Boolean
CheckStyle( im, im_style )
    XIM		im;
    XIMStyle	im_style;
{
    XIMStyles  *im_supported_styles;
    XIMStyle	im_supported_style;
    int		n;

    XGetIMValues( im, XNQueryInputStyle, &im_supported_styles, NULL );
    if( !im_supported_styles )
	return( False );
    for( n = 0; n < im_supported_styles->count_styles; n++ ) {
	im_supported_style = im_supported_styles->supported_styles[n];
	if( (im_style & im_supported_style) == im_style ) {
	    XFree( im_supported_styles );
	    return( True );
	}
    }
    XFree( im_supported_styles );
    return( False );
}

static void
destroy_callback( ic, client_data, call_data )
    XIC		 ic;
    XPointer	 client_data;
    XPointer	*call_data;
{
    TScreen *screen = (TScreen *)client_data;

    screen->im = NULL;
    screen->ic = NULL;
}

static XIMProc
process_start_callback(display, client_data, call_data)
    Display *display;
    XPointer client_data;
    XPointer call_data;
{
    TScreen *screen = (TScreen *)client_data;

    if (!screen->im) {
	XIMCallback destroy;
	XVaNestedList preedit_attr, status_attr;
	XPoint spot;

	screen->im = XOpenIM(display, XtDatabase(display),
			     ProgramName, ClassName);
	if (!screen->im) {
	    return;
	}
	destroy.callback = destroy_callback;
	destroy.client_data = (XPointer)screen;
	XSetIMValues(screen->im,
		     XNDestroyCallback, &destroy,
		     NULL );
	if (!CheckStyle(screen->im, screen->im_style)) {
		screen->im_style = XIMPreeditNothing|XIMStatusNothing;
	}
	if (screen->im_fontset) {
	    preedit_attr = XVaCreateNestedList(NULL,
					       XNSpotLocation, &spot,
					       XNForeground, screen->foreground,
					       XNBackground, term->core.background_pixel,
					       XNFontSet, screen->im_fontset,
					       NULL);
	    status_attr = XVaCreateNestedList(NULL,
					      XNForeground, screen->foreground,
					      XNBackground, term->core.background_pixel,
					      XNFontSet, screen->im_fontset,
					      NULL);
	    screen->ic = XCreateIC( screen->im,
				    XNClientWindow, term->core.window,
				    XNFocusWindow, term->core.window,
				    XNInputStyle, screen->im_style,
				    XNPreeditAttributes, preedit_attr,
				    XNStatusAttributes, status_attr,
				    NULL );
	    XFree( preedit_attr );
	    XFree( status_attr );
	}
	if( screen->ic == NULL ) {
	    XCloseIM(screen->im);
	    screen->im = NULL;
	    return;
	}
    }
    if (screen->ic) {
	SetStatus(screen);
    }
}

Boolean ConnectXIM( screen )
    TScreen	*screen;
{
    XrmDatabase		rdb = NULL, r;

    rdb = XtDatabase(screen->display);
    r = NULL;
    if( screen->im_style & XIMPreeditArea )
	r = XrmGetStringDatabase( "*preeditDefaultStyle:XIMPreeditArea" );
    if( screen->im_style & XIMPreeditPosition )
	r = XrmGetStringDatabase( "*preeditDefaultStyle:XIMPreeditPosition" );
    if( screen->im_style & XIMPreeditNothing )
	r = XrmGetStringDatabase( "*preeditDefaultStyle:XIMPreeditNothing" );
    if( r != NULL )
	XrmMergeDatabases( r, &rdb );
    r = NULL;
    if( screen->im_style & XIMStatusArea )
	r = XrmGetStringDatabase( "*statusDefaultStyle:XIMStatusArea" );
    if( screen->im_style & XIMStatusNothing )
	r = XrmGetStringDatabase( "*statusDefaultStyle:XIMStatusNothing" );
    if( r != NULL )
	XrmMergeDatabases( r, &rdb );

    XRegisterIMInstantiateCallback(screen->display, rdb,
				   ProgramName, ClassName,
				   process_start_callback, (XPointer)screen);
    return True;
}
#endif /* USE_XIM */
