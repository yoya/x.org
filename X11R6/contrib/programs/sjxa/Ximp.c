/* $SonyId: Ximp.c,v 1.1 1994/05/30 13:03:58 makoto Exp $ */
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
#ifdef	USE_XIMP
#include	<stdio.h>
#include	<X11/keysym.h>
#include	<X11/Xatom.h>
#include	<X11/Xlib.h>
#include	<X11/Intrinsic.h>
#include	<X11/StringDefs.h>
#include	<X11/Xaw/Form.h>
#include	<Xmw/Command.h>
#include	<Xmw/Label.h>
#include	<Xmw/List.h>
#include	<Xmw/Preedit.h>
#include	<Xmw/StsArea.h>
#include	<Xmw/TextOut.h>
#include	"XimpP.h"
#include	"sjxa.h"
#include	"resource.h"
#include	"version.h"
#include	"SjBuffer.h"
#include	"SjString.h"
#include	"SjXkey.h"

#define	LANGUAGE	"ja"
#define	TERRITORY	"JP"
#define	VENDOR_NAME	"SONY"

typedef struct	_XimpQueue {
    struct _XimpQueue		*next;
    struct _XimpQueue		*previous;
    Window			 client_window;
    Window			 focus_window;
    ICID			 icid;
    unsigned long		 protocol;
    unsigned long		 style;
    unsigned long		 mask;
    unsigned long		 ximp_type;
    long			 event_mask;
    XimpPreeditPropRec		 preedit;
    XimpStatusPropRec		 status;
    XimpSizeRec			 preedit_area;
    XimpSizeRec			 status_area;
    char			*preedit_font;
    char			*status_font;
    int				 preedit_ascent;
    Boolean			 connect_flag;
    Boolean			 has_focus;
    Pixel			 icon_fg;
    Pixel			 icon_bg;
    Buffer			*buffer;
} XimpQueue;


static unsigned long	supported_styles[] = {
    XIMPreeditPosition | XIMStatusArea,
    XIMPreeditArea     | XIMStatusArea,
    XIMPreeditPosition | XIMStatusNothing,
    XIMPreeditArea     | XIMStatusNothing,
    XIMPreeditNothing  | XIMStatusNothing
/*  XIMPreeditNothing  | XIMStatusArea		*/	/* unsupported style */
};

static unsigned long	supported_type[] = {
    XIMP_FE_TYPE1,
    XIMP_BE_TYPE1,
    XIMP_SYNC_BE_TYPE1,
    XIMP_FE_TYPE2,
    XIMP_BE_TYPE2,
    XIMP_SYNC_BE_TYPE2,
    XIMP_FE_TYPE3
};


/* Protocol Name */
static Atom		ximp_protocol;

/* Property Name */
static Atom	ximp_basic_declaration[2];
static Atom	ximp_detailed_declaration[10];

/* IMS Window Property Name */
static Atom	ximp_version;
static Atom	ximp_style;
static Atom	ximp_type;
static Atom	ximp_keys;
static Atom	ximp_started_keys;
static Atom	ximp_stopped_keys;
static Atom	ximp_server_name;
static Atom	ximp_server_version;
static Atom	ximp_extensions;
static Atom	ximp_vendor_name;
static Atom	ximp_commit;

/* Client Window Property Name */
static Atom	ximp_focus;
static Atom	ximp_preedit;
static Atom	ximp_status;
static Atom	ximp_preedit_font;
static Atom	ximp_status_font;
static Atom	ximp_compound_text;
static Atom	ximp_reset;

/* Extension atom */
static Atom	ximp_ext_conversion;
static Atom	ximp_ext_back_front;

#ifdef DEBUG
XimpQueue	*ximp_queue = NULL;
#else
static XimpQueue	*ximp_queue = NULL;
#endif

extern void		sjxa_connect();
extern void		sjxa_disconnect();
extern void		function_activate();

extern Sjxa		 resources;
extern Widget		 TopLevel, RootTextField;
extern Widget		 RootLabel, Box[2];
extern Widget		 delete_shell, question_shell, server_shell;
extern Buffer		*buffer, root_window;
extern char		*program_name;
extern unsigned long	 default_foreground;
extern unsigned long	 default_background;
extern LcCtype		 user_locale;
extern int		*depths;
extern Endian		 endian;


static void
#if NeedFunctionPrototypes
set_atom_string( Display *display, Window window, Atom p, char *s )
#else /* NeedFunctionPrototypes */
set_atom_string( display, window, p, s )
    Display	*display;
    Window	 window;
    Atom	 p;
    char	*s;
#endif /* NeedFunctionPrototypes */
{
    XChangeProperty( display, window, p, XA_STRING, 8, PropModeReplace,
		     (unsigned char *)s, strlen( s ) );
}


static void
#if NeedFunctionPrototypes
set_atom_keys( Display *display, Window window, Atom p, XimpKey *keys )
#else /* NeedFunctionPrototypes */
set_atom_keys( display, window, p, keys )
    Display	*display;
    Window	 window;
    Atom	 p;
    XimpKey	*keys;
#endif /* NeedFunctionPrototypes */
{
    int		ii;

    if( keys == NULL ) {
	XtError( "conversion key undefined" );
	exit( 1 );
    }

    for( ii = 0; keys[ii].keysym != XK_VoidSymbol; ii++ );

    if( ii != 0 )
	XChangeProperty( display, window, p, p, 32, PropModeReplace,
			(unsigned char *)keys, sizeof(XimpKey) * ii / 4 );
}


Boolean
#if NeedFunctionPrototypes
XimpCheckState( State state )
#else /* NeedFunctionPrototypes */
XimpCheckState( state )
    State	state;
#endif /* NeedFunctionPrototypes */
{
    XimpQueue	*q;

    for( q = ximp_queue; q != NULL; q = q->next )
	if( q->buffer->state & state )
	    return( True );
    return( False );
}


static void
#if NeedFunctionPrototypes
send_client_message( Display *display, Window win, long d0, long d1, long d2,
		     long d3, long d4 )
#else /* NeedFunctionPrototypes */
send_client_message( display, win, d0, d1, d2, d3, d4 )
    Display	*display;
    Window	win;
    long	d0, d1, d2, d3, d4;
#endif /* NeedFunctionPrototypes */
{
    XClientMessageEvent	cm;
#ifdef	DEBUG
    int		ii;
    static struct {
	int	number;
	char	*name;
    } protocol[] = {
	{ XIMP_KEYPRESS3,		"XIMP_KEYPRESS3"		},
	{ XIMP_CREATE_RETURN3,		"XIMP_CREATE_RETURN3"		},
	{ XIMP_PROCESS_BEGIN3,		"XIMP_PROCESS_BEGIN3"		},
	{ XIMP_PROCESS_END3,		"XIMP_PROCESS_END3"		},
	{ XIMP_READPROP3,		"XIMP_READPROP3"		},
	{ XIMP_GETVALUE_RETURN3,	"XIMP_GETVALUE_RETURN3"		},
	{ XIMP_RESET_RETURN3,		"XIMP_RESET_RETURN3"		},
	{ XIMP_ERROR3,			"XIMP_ERROR3"			},
	{ XIMP_KEYPRESS4,		"XIMP_KEYPRESS4"		},
	{ XIMP_KEYRELEASE4,		"XIMP_KEYRELEASE4"		},
	{ XIMP_SPROC_STARTED4,		"XIMP_SPROC_STARTED4"		},
	{ XIMP_SPROC_STOPPED4,		"XIMP_SPROC_STOPPED4"		},
	{ XIMP_READPROP4,		"XIMP_READPROP4"		},
	{ XIMP_CLIENT_WINDOW_RETURN4,	"XIMP_CLIENT_WINDOW_RETURN4"	},
	{ XIMP_FOCUS_WINDOW_RETURN4,	"XIMP_FOCUS_WINDOW_RETURN4"	},
	{ XIMP_GETVALUE_RETURN4,	"XIMP_GETVALUE_RETURN4"		},
	{ XIMP_RESET_RETURN4,		"XIMP_RESET_RETURN4"		},
	{ XIMP_CREATE_RETURN4,		"XIMP_CREATE_RETURN4"		},
	{ XIMP_KEYPRESS_RETURN4,	"XIMP_KEYPRESS_RETURN4"		},
	{ XIMP_KEYRELEASE_RETURN4,	"XIMP_KEYRELEASE_RETURN4"	},
	{ XIMP_ERROR4,			"XIMP_ERROR4"			},
	{ XIMP_EVENTMASK_NOTIFY4,	"XIMP_EVENTMASK_NOTIFY4"	},
	{ XIMP_EVENTMASK_NOTIFY_RETURN4,"XIMP_EVENTMASK_NOTIFY_RETURN4"	},
	{ 0,				"XIMP_???"			}};

    if( resources.debug & 1 ) {
	for( ii = 0; protocol[ii].number != 0; ii++ )
	    if( protocol[ii].number == d0 )
		break;
	printf( "%s [%d] %08x %08x %08x\n", protocol[ii].name,
	       d1, d2, d3, d4 );
    }
#endif
    cm.type = ClientMessage;
    cm.window = win;
    cm.message_type = ximp_protocol;
    cm.format = 32;
    cm.data.l[0] = d0;
    cm.data.l[1] = d1;
    cm.data.l[2] = d2;
    cm.data.l[3] = d3;
    cm.data.l[4] = d4;
    XSendEvent( display, win, False, 0, (XEvent *)(&cm) );
}


#define	send_error_notify( d, w, protocol, icid, request, error )	\
    send_client_message( d, w, (long)XIMP_ERROR(protocol), (long)icid, \
			 (long)request, (long)error, 0L )


#define	CT_MAX_IN_CM 15

static void
#if NeedFunctionPrototypes
send_multi_client_message( Display *display, Window win, ICID icid, int size,
			   unsigned char *ct )
#else /* NeedFunctionPrototypes */
send_multi_client_message( display, win, icid, size, ct )
    Display	*display;
    Window	win;
    ICID	icid;
    int		size;
unsigned char	*ct;
#endif /* NeedFunctionPrototypes */
{
    int			ii, jj;
    XClientMessageEvent	cm;

#ifdef	DEBUG
    if( resources.debug & 1 )
	printf( "XIMP_MULTI_CLIENT_MESSAGE [%d] %d\n", icid, size );
#endif
    cm.type = ClientMessage;
    cm.window = win;
    cm.message_type = ximp_protocol;
    cm.format = 8;
    if( endian == BIGENDIAN )
	cm.data.l[0] = icid;
    else {
	cm.data.b[0] = icid >> 24;
	cm.data.b[1] = (icid & 0x00ffffff) >> 16;
	cm.data.b[2] = (icid & 0x0000ffff) >> 8;
	cm.data.b[3] = icid & 0x000000ff;
    }
    do {
	cm.data.b[4] = size;
	ii = (size >= CT_MAX_IN_CM) ? CT_MAX_IN_CM : size;
	size -= CT_MAX_IN_CM;
	for( jj = 0; jj < ii; jj++ ) {
	    cm.data.b[5+jj] = *ct;
	    ct++;
	}
	XSendEvent( display, win, False, 0, (XEvent *)(&cm) );
    } while( size >= 0 );
}


static Boolean
#if NeedFunctionPrototypes
get_ximp_queue_data( XimpQueue *q, unsigned long mask )
#else /* NeedFunctionPrototypes */
get_ximp_queue_data( q, mask )
    XimpQueue	*q;
    unsigned long	mask;
#endif /* NeedFunctionPrototypes */
{
    Display		*display = XtDisplay(TopLevel);
    Window		client = q->client_window;
    int			result;
    Atom		actual_type;
    int			actual_format;
    unsigned long	nitems, bytes_after;
    long		*prop;
    XStandardColormap	*std_colormap;
    int			count;

#ifdef DEBUG
    if( resources.debug & 1 ) {
	if( ISXimp4(q) ) {
	    if( mask & XIMP_FOCUS_WIN_MASK4 ) puts( "\tFOCUS_WIN_MASK" );
	    if( mask & XIMP_PRE_AREA_MASK4 ) puts( "\tPRE_AREA_MASK" );
	    if( mask & XIMP_PRE_AREANEED_MASK4 ) puts( "\tPRE_AREANEED_MASK" );
	    if( mask & XIMP_PRE_COLORMAP_MASK4 ) puts( "\tPRE_COLORMAP_MASK" );
	    if( mask & XIMP_PRE_STD_COLORMAP_MASK4 ) puts( "\tPRE_STD_COLORMAP_MASK" );
	    if( mask & XIMP_PRE_FG_MASK4 ) puts( "\tPRE_FG_MASK" );
	    if( mask & XIMP_PRE_BG_MASK4 ) puts( "\tPRE_BG_MASK" );
	    if( mask & XIMP_PRE_BGPIXMAP_MASK4 ) puts( "\tPRE_BGPIXMAP_MASK" );
	    if( mask & XIMP_PRE_LINESP_MASK4 )	puts( "\tPRE_LINESP_MASK" );
	    if( mask & XIMP_PRE_CURSOR_MASK4 )	puts( "\tPRE_CURSOR_MASK" );
	    if( mask & XIMP_PRE_SPOTL_MASK4 )	puts( "\tPRE_SPOTL_MASK" );
	    if( mask & XIMP_STS_AREA_MASK4 )	puts( "\tSTS_AREA_MASK" );
	    if( mask & XIMP_STS_AREANEED_MASK4 ) puts( "\tSTS_AREANEED_MASK" );
	    if( mask & XIMP_STS_COLORMAP_MASK4 ) puts( "\tSTS_COLORMAP_MASK" );
	    if( mask & XIMP_STS_STD_COLORMAP_MASK4 ) puts( "\tSTS_STD_COLORMAP_MASK" );
	    if( mask & XIMP_STS_FG_MASK4 )	puts( "\tSTS_FG_MASK" );
	    if( mask & XIMP_STS_BG_MASK4 ) puts( "\tSTS_BG_MASK" );
	    if( mask & XIMP_STS_BGPIXMAP_MASK4 ) puts( "\tSTS_BGPIXMAP_MASK" );
	    if( mask & XIMP_STS_LINESP_MASK4 ) puts( "\tSTS_LINESP_MASK" );
	    if( mask & XIMP_STS_CURSOR_MASK4 )	puts( "\tSTS_CURSOR_MASK" );
	    if( mask & XIMP_STS_WINDOW_MASK4 )	puts( "\tSTS_WINDOW_MASK" );
	    if( mask & XIMP_PRE_FONT_MASK4 )	puts( "\tPRE_FONT_MASK" );
	    if( mask & XIMP_STS_FONT_MASK4 )	puts( "\tSTS_FONT_MASK" );
	    if( mask & XIMP_SERVERTYPE_MASK4 )	puts( "\tSERVERTYPE_MASK" );
	}
	else if( ISXimp3(q) ) {
	    if( mask & XIMP_FOCUS_WIN_MASK3 ) puts( "\tFOCUS_WIN_MASK" );
	    if( mask & XIMP_PRE_AREA_MASK3 ) puts( "\tPRE_AREA_MASK" );
	    if( mask & XIMP_PRE_FG_MASK3 ) puts( "\tPRE_FG_MASK" );
	    if( mask & XIMP_PRE_BG_MASK3 ) puts( "\tPRE_BG_MASK" );
	    if( mask & XIMP_PRE_COLORMAP_MASK3 ) puts( "\tPRE_COLORMAP_MASK" );
	    if( mask & XIMP_PRE_BGPIXMAP_MASK3 ) puts( "\tPRE_BGPIXMAP_MASK" );
	    if( mask & XIMP_PRE_LINESP_MASK3 ) puts( "\tPRE_LINESP_MASK" );
	    if( mask & XIMP_PRE_CURSOR_MASK3 ) puts( "\tPRE_CURSOR_MASK" );
	    if( mask & XIMP_PRE_AREANEED_MASK3 ) puts( "\tPRE_AREANEED_MASK" );
	    if( mask & XIMP_PRE_SPOTL_MASK3 ) puts( "\tPRE_SPOTL_MASK" );
	    if( mask & XIMP_STS_AREA_MASK3 ) puts( "\tSTS_AREA_MASK" );
	    if( mask & XIMP_STS_FG_MASK3 ) puts( "\tSTS_FG_MASK" );
	    if( mask & XIMP_STS_BG_MASK3 ) puts( "\tSTS_BG_MASK" );
	    if( mask & XIMP_STS_COLORMAP_MASK3 ) puts( "\tSTS_COLORMAP_MASK" );
	    if( mask & XIMP_STS_BGPIXMAP_MASK3 ) puts( "\tSTS_BGPIXMAP_MASK" );
	    if( mask & XIMP_STS_LINESP_MASK3 ) puts( "\tSTS_LINESP_MASK" );
	    if( mask & XIMP_STS_CURSOR_MASK3 ) puts( "\tSTS_CURSOR_MASK" );
	    if( mask & XIMP_STS_AREANEED_MASK3 ) puts( "\tSTS_AREANEED_MASK" );
	    if( mask & XIMP_STS_WINDOW_MASK3 ) puts( "\tSTS_WINDOW_MASK" );
	    if( mask & XIMP_PRE_FONT_MASK3 ) puts( "\tPRE_FONT_MASK" );
	    if( mask & XIMP_STS_FONT_MASK3 ) puts( "\tSTS_FONT_MASK" );
	}
    }
#endif

    if( mask & XIMP_FOCUS_WIN_MASK(q) ) {
	result = XGetWindowProperty( display, client, ximp_focus, 0, 1L, True,
				     XA_WINDOW, &actual_type, &actual_format,
				     &nitems, &bytes_after,
				     (unsigned char **)&prop );
	if( result != Success  ||  nitems == 0 ) {
	    XFree( (char *)prop );
	    XDeleteProperty( display, client, ximp_preedit );
	    XDeleteProperty( display, client, ximp_preedit_font );
	    XDeleteProperty( display, client, ximp_status );
	    XDeleteProperty( display, client, ximp_status_font );
	    XDeleteProperty( display, client, ximp_type );
	    return( False );
	}
	if( q->style & (XIMPreeditPosition | XIMPreeditArea) ) {
	    if( q->focus_window != q->client_window ) {
		XSelectInput( display, q->focus_window, NoEventMask );
	    }
	    else {
		if( ISFE2(q)  ||  ISFE3(q) )
		    XSelectInput( display, q->client_window,
				  ExposureMask | StructureNotifyMask );
	    }
	}
	q->focus_window = *prop;
	if( q->style & (XIMPreeditPosition | XIMPreeditArea) )
	    XSelectInput( display, q->focus_window,
			  ExposureMask | StructureNotifyMask );
	XFree( prop );
#ifdef DEBUG
	if( resources.debug & 2 )
	    printf( " focus window = %08x\n", q->focus_window );
#endif
    }
    if( mask & XIMP_PRE_MASK(q) ) {
	result = XGetWindowProperty( display, client, ximp_preedit, 0,
				     ISXimp4(q) ? 15L : 14L, True, ximp_preedit,
				     &actual_type, &actual_format, &nitems,
				     &bytes_after, (unsigned char **)&prop );
	if( result != Success  ||  nitems == 0 ) {
	    XFree( (char *)prop );
	    XDeleteProperty( display, client, ximp_preedit_font );
	    XDeleteProperty( display, client, ximp_status );
	    XDeleteProperty( display, client, ximp_status_font );
	    XDeleteProperty( display, client, ximp_type );
	    return( False );
	}

	if( mask & XIMP_PRE_AREA_MASK(q) ) {
	    q->preedit.area.x = prop[0];
	    q->preedit.area.y = prop[1];
	    if( (q->preedit.area.width = prop[2]) <= 0 )
		q->preedit.area.width = 1;
	    if( (q->preedit.area.height = prop[3]) <= 0 )
		q->preedit.area.height = 1;
#ifdef	DEBUG
	    if( resources.debug & 2 ) {
		printf( " Set PreeditArea : x      : %d\n", q->preedit.area.x );
		printf( " Set PreeditArea : y      : %d\n", q->preedit.area.y );
		printf( " Set PreeditArea : width  : %d\n",
			q->preedit.area.width );
		printf( " Set PreeditArea : height : %d\n",
			q->preedit.area.height );
	    }
#endif
	}
	if( mask & XIMP_PRE_FG_MASK(q) ) {
	    q->preedit.foreground = (unsigned long)prop[ISXimp3(q) ? 4 : 10];
	}
	if( mask & XIMP_PRE_BG_MASK(q) ) {
	    q->preedit.background = (unsigned long)prop[ISXimp3(q) ? 5 : 11];
	}
	if( mask & XIMP_PRE_COLORMAP_MASK(q) ) {
	    q->preedit.colormap = (Colormap)prop[ISXimp3(q) ? 6 : 8];
	}
	if( mask & XIMP_PRE_STD_COLORMAP_MASK(q) ) {
	    q->preedit.std_colormap = (Atom)prop[9];
	    if( XGetRGBColormaps( display, q->focus_window, &std_colormap,
				  &count, q->preedit.std_colormap ) ) {
		q->preedit.colormap = std_colormap->colormap;
	    }

	}
	if( mask & XIMP_PRE_BGPIXMAP_MASK(q) ) {
	    q->preedit.bg_pixmap = (Pixmap)prop[ISXimp3(q) ? 7 : 12];
	}
	if( mask & XIMP_PRE_LINESP_MASK(q) ) {
	    q->preedit.line_spacing = prop[ISXimp3(q) ? 8 : 13];
	}
	if( mask & XIMP_PRE_CURSOR_MASK(q) ) {
	    q->preedit.cursor = (Cursor)prop[ISXimp3(q) ? 9 : 14];
	}
	if( mask & XIMP_PRE_AREANEED_MASK(q) ) {
	    q->preedit.area_needed.width = prop[ISXimp3(q) ? 10 : 4];
	    q->preedit.area_needed.height = prop[ISXimp3(q) ? 11 : 5];
#ifdef	DEBUG
	    if( resources.debug & 2 ) {
		printf( " Set PreeditAreaNeeded : width  : %d\n",
			q->preedit.area_needed.width );
		printf( " Set PreeditAreaNeeded : height : %d\n",
			q->preedit.area_needed.height );
	    }
#endif
	}
	if( mask & XIMP_PRE_SPOTL_MASK(q) ) {
	    q->preedit.spot_location.x = prop[ISXimp3(q) ? 12 : 6];
	    q->preedit.spot_location.y = prop[ISXimp3(q) ? 13 : 7];
#ifdef	DEBUG
	    if( resources.debug & 2 ) {
		printf( " Set SpotLocation : x : %d\n",
			q->preedit.spot_location.x );
		printf( " Set SpotLocation : y : %d\n",
			q->preedit.spot_location.y );
	    }
#endif
	}
	XFree( (char *)prop );
    }

    if( mask & XIMP_PRE_FONT_MASK(q) ) {
	char	*font = NULL;
	result = XGetWindowProperty( display, client, ximp_preedit_font,
				     0L, 10000L, True, XA_STRING, &actual_type,
				     &actual_format, &nitems, &bytes_after,
				     (unsigned char **)&font );
	if( result != Success  ||  nitems == 0 ) {
	    if( font )	XFree( font );
	    XDeleteProperty( display, client, ximp_status );
	    XDeleteProperty( display, client, ximp_status_font );
	    XDeleteProperty( display, client, ximp_type );
	    return( False );
	}

	if( q->preedit_font == NULL )
	    q->preedit_font = (char *)malloc( nitems+1 );
	else
	    q->preedit_font = (char *)realloc( q->preedit_font, nitems+1 );
	strcpy( q->preedit_font, font );
	XFree( font );
    }

    if( mask & XIMP_STS_MASK(q) ) {
	result = XGetWindowProperty( display, client, ximp_status, 0,
				     ISXimp4(q) ? 14L : 13L,
				     True, ximp_status, &actual_type,
				     &actual_format, &nitems, &bytes_after,
				     (unsigned char **)&prop );
	if( result != Success  ||  nitems == 0 ) {
	    XFree( (char *)prop );
	    XDeleteProperty( display, client, ximp_status_font );
	    XDeleteProperty( display, client, ximp_type );
	    return( False );
	}

	if( mask & XIMP_STS_AREA_MASK(q) ) {
	    q->status.area.x = prop[0];
	    q->status.area.y = prop[1];
	    if( (q->status.area.width = prop[2]) <= 0 )
		q->status.area.width = 1;
	    if( (q->status.area.height = prop[3]) <= 0 )
		q->status.area.height = 1;
#ifdef	DEBUG
	    if( resources.debug & 2 ) {
		printf( " Set StatusArea : x      : %d\n", (int)prop[0] );
		printf( " Set StatusArea : y      : %d\n", (int)prop[1] );
		printf( " Set StatusArea : width  : %d\n", (int)prop[2] );
		printf( " Set StatusArea : height : %d\n", (int)prop[3] );
	    }
#endif
	}
	if( mask & XIMP_STS_FG_MASK(q) )
	    q->status.foreground = (unsigned long)prop[ISXimp3(q) ? 4 : 8];
	if( mask & XIMP_STS_BG_MASK(q) )
	    q->status.background = (unsigned long)prop[ISXimp3(q) ? 5 : 9];
	if( mask & XIMP_STS_COLORMAP_MASK(q) )
	    q->status.colormap = (Colormap)prop[6];
	if( mask & XIMP_STS_STD_COLORMAP_MASK(q) ) {
	    q->status.std_colormap = (Atom)prop[7];
	    if( XGetRGBColormaps( display, q->client_window, &std_colormap,
				  &count, q->status.std_colormap ) ) {
		q->status.colormap = std_colormap->colormap;
	    }
	}
	if( mask & XIMP_STS_BGPIXMAP_MASK(q) )
	    q->status.bg_pixmap = (Pixmap)prop[ISXimp3(q) ? 7 : 10];
	if( mask & XIMP_STS_LINESP_MASK(q) )
	    q->status.line_spacing = prop[ISXimp3(q) ? 8 : 11];
	if( mask & XIMP_STS_CURSOR_MASK(q) )
	    q->status.cursor = (Cursor)prop[ISXimp3(q) ? 9 : 12];
	if( mask & XIMP_STS_AREANEED_MASK(q) ) {
	    if( (q->status.area_needed.width = prop[ISXimp3(q)?10:4]) <= 0 )
		q->status.area_needed.width = 1;
	    if( (q->status.area_needed.height = prop[ISXimp3(q)?11:5]) <= 0 )
		q->status.area_needed.height = 1;
#ifdef	DEBUG
	    if( resources.debug & 2 ) {
		printf( " Set StatusAreaNeeded : width  : %d\n",
			q->status.area_needed.width );
		printf( " Set StatusAreaNeeded : height : %d\n",
			q->status.area_needed.height );
	    }
#endif
	}
	if( mask & XIMP_STS_WINDOW_MASK(q) )
	    q->status.window = (Window)prop[12];
	XFree( (char *)prop );
    }

    if( mask & XIMP_STS_FONT_MASK(q) ) {
	char	*font = NULL;
	result = XGetWindowProperty( display, client, ximp_status_font,
				     0, 10000L, True, XA_STRING, &actual_type,
				     &actual_format, &nitems, &bytes_after,
				     (unsigned char **)&font );
	if( result != Success  ||  nitems == 0 ) {
	    if( font )	XFree( font );
	    XDeleteProperty( display, client, ximp_type );
	    return( False );
	}

	if( q->status_font == NULL )
	    q->status_font = (char *)malloc( nitems+1 );
	else
	    q->status_font = (char *)realloc( q->status_font, nitems+1 );

	strcpy( q->status_font, font );
	XFree( font );
    }

    if( mask & XIMP_SERVERTYPE_MASK(q) ) {
	if( XGetWindowProperty( display, q->client_window, ximp_type,
			        0, 1L, True, ximp_type, &actual_type,
			        &actual_format, &nitems, &bytes_after,
			        (unsigned char **)&prop ) == Success ) {
	    if( nitems == 0 ) {
		XFree( prop );
		return( False );
	    }
	    else {
		int	ii;
		q->ximp_type = *prop;
		XFree( prop );
		for( ii = 0; ii < sizeof(supported_type) /
				  sizeof(unsigned long); ii++ )
		    if( supported_type[ii] == q->ximp_type )
			break;
		if( ii == sizeof(supported_type) / sizeof(unsigned long) )
		    return( False );
	    }
	}
	else {
	    return( False );
	}
#ifdef	DEBUG
	if( resources.debug & 1 ) {
	    switch( q->ximp_type ) {
	      case XIMP_FE_TYPE1:	puts( "[XIMP_FE_TYPE1]" );	break;
	      case XIMP_FE_TYPE2:	puts( "[XIMP_FE_TYPE2]" );	break;
	      case XIMP_FE_TYPE3:	puts( "[XIMP_FE_TYPE3]" );	break;
	      case XIMP_BE_TYPE1:	puts( "[XIMP_BE_TYPE1]" );	break;
	      case XIMP_BE_TYPE2:	puts( "[XIMP_BE_TYPE2]" );	break;
	      case XIMP_SYNC_BE_TYPE1:	puts( "[XIMP_SYNC_BE_TYPE1]" );	break;
	      case XIMP_SYNC_BE_TYPE2:	puts( "[XIMP_SYNC_BE_TYPE2]" );	break;
	    }
	    switch( q->style ) {
	      case XIMPreeditPosition | XIMStatusArea:
		puts( "[XIMPreeditPosition | XIMStatusArea]" );		break;
	      case XIMPreeditArea | XIMStatusArea:
		puts( "XIMPreeditArea | XIMStatusArea" );		break;
	      case XIMPreeditPosition | XIMStatusNothing:
		puts( "XIMPreeditPosition | XIMStatusNothing" );	break;
	      case XIMPreeditArea | XIMStatusNothing:
		puts( "XIMPreeditArea | XIMStatusNothing" );		break;
	      case XIMPreeditNothing | XIMStatusNothing:
		puts( "XIMPreeditNothing | XIMStatusNothing" );		break;
	    }
	}
#endif
    }
    return( True );
}


static unsigned long
#if NeedFunctionPrototypes
set_preedit_values( XimpQueue *q, unsigned long mask )
#else /* NeedFunctionPrototypes */
set_preedit_values( q, mask )
    XimpQueue		*q;
    unsigned long	mask;
#endif /* NeedFunctionPrototypes */
{
    Display		*display = XtDisplay(TopLevel);
    int			n, ii;
    Arg			args[30];
    char		ss[100];
    Dimension		width, height;
    Window		win;
    XWindowAttributes	attr;
    XFontSet		fs;
    XFontStruct		**fsl;
    char		**fn;
    char		**missing_list;
    int			missing_count;
    char		*def_string;

    n = 0;
    if( (mask & XIMP_FOCUS_WIN_MASK(q))  &&  (q->style & XIMPreeditPosition) ) {
	XtSetArg( args[n], XmwNfocusWindow, q->focus_window );		n++;
    }
    if( mask & XIMP_PRE_AREA_MASK(q) ) {
	XtSetArg( args[n], XmwNareaX, q->preedit.area.x );		n++;
	XtSetArg( args[n], XmwNareaY, q->preedit.area.y );		n++;
	XtSetArg( args[n], XmwNareaWidth, q->preedit.area.width );	n++;
	XtSetArg( args[n], XmwNareaHeight, q->preedit.area.height );	n++;
	if( q->style & XIMPreeditArea ) {
	    XtSetArg( args[n], XmwNspotLocationX, q->preedit.area.x );	n++;
	    XtSetArg( args[n], XmwNspotLocationY, q->preedit.area.y );	n++;
	}
    }
    if( resources.ignore_preedit_color )
	mask &= ~(XIMP_PRE_COLORMAP_MASK(q) |
		  XIMP_PRE_FG_MASK(q) |
		  XIMP_PRE_BG_MASK(q));
    if( mask & XIMP_PRE_FG_MASK(q) ) {
	XtSetArg( args[n], XtNforeground, q->preedit.foreground );
	n++;
    }
    if( mask & XIMP_PRE_BG_MASK(q) ) {
	XtSetArg( args[n], XtNbackground, q->preedit.background );
	n++;
    }
    if( q->buffer->preedit_widget == NULL ) {
	if( mask & (XIMP_PRE_STD_COLORMAP_MASK(q) | XIMP_PRE_COLORMAP_MASK(q)) ) {
	    XtSetArg( args[n], XtNcolormap, q->preedit.colormap );
	    n++;
	}
    }
    if( mask & XIMP_PRE_BGPIXMAP_MASK(q) ) {
	XtSetArg( args[n], XtNbackgroundPixmap, q->preedit.bg_pixmap );
	n++;
    }
    if( mask & XIMP_PRE_LINESP_MASK(q) ) {
	XtSetArg( args[n], XmwNlineSpacing, q->preedit.line_spacing );
	n++;
    }
    if( mask & XIMP_PRE_CURSOR_MASK(q) ) {
	XtSetArg( args[n], XtNcursor, q->preedit.cursor );
	n++;
    }
    if( (mask & XIMP_PRE_AREANEED_MASK(q))  &&  !(mask & XIMP_PRE_AREA_MASK(q)) ) {
	XtSetArg( args[n], XmwNareaNeededWidth, q->preedit.area_needed.width );
	n++;
	XtSetArg( args[n], XmwNareaNeededHeight, q->preedit.area_needed.height );
	n++;
    }
    if( mask & XIMP_PRE_FONT_MASK(q) ) {
	fs = XCreateFontSet( display, q->preedit_font, &missing_list,
			     &missing_count, &def_string );
	if( missing_list )
	    XFree( missing_list );
	if( fs == NULL )
	    return( XIMP_PRE_FONT_MASK(q) );
	XtSetArg( args[n], XtNfontSet, fs );			n++;
	ii = XFontsOfFontSet( fs, &fsl, &fn );
	q->preedit_ascent = 0;
	while( --ii >= 0 ) {
	    if( q->preedit_ascent < fsl[ii]->ascent )
		q->preedit_ascent = fsl[ii]->ascent;
	}
	if( user_locale == LC_CTYPE_EUC ) {
	    def_string = XBaseFontNameListOfFontSet( fs );
	    q->buffer->use_jisx0212 = (strstr( def_string, "0212" ) != NULL);
	}
    }
    if( (mask & XIMP_PRE_SPOTL_MASK(q))  &&  (q->style & XIMPreeditPosition) ) {
	XtSetArg( args[n], XmwNspotLocationX, q->preedit.spot_location.x );
	n++;
	XtSetArg( args[n], XmwNspotLocationY,
		  q->preedit.spot_location.y - q->preedit_ascent );	n++;
    }

    if( q->buffer->preedit_widget == NULL ) {
	XtSetArg( args[n], XtNmappedWhenManaged, False );	n++;
	win = q->focus_window;
	if( (q->style & XIMPreeditPosition)  &&
	   !(mask & XIMP_FOCUS_WIN_MASK(q)) ) {
	    win = q->client_window;
	    XtSetArg( args[n], XmwNfocusWindow, q->client_window );	n++;
	}
	else if( q->style & XIMPreeditArea ) {
	    win = q->client_window;
	    XtSetArg( args[n], XmwNfocusWindow, q->client_window );	n++;
	}

	if( XGetWindowAttributes( display, win, &attr ) == 0 )
	    return( mask );
	if( !(mask & XIMP_PRE_AREA_MASK(q)) ) {
	    q->preedit.area.x = q->preedit.spot_location.x;
	    q->preedit.area.y = q->preedit.spot_location.y - q->preedit_ascent;
	    q->preedit.area.width = attr.width - q->preedit.area.x;
	    q->preedit.area.height = attr.height - q->preedit.area.y;
	    XtSetArg( args[n], XmwNareaX, q->preedit.area.x );		n++;
	    XtSetArg( args[n], XmwNareaY, q->preedit.area.y );		n++;
	    XtSetArg( args[n], XmwNareaWidth, q->preedit.area.width );	n++;
	    XtSetArg( args[n], XmwNareaHeight, q->preedit.area.height );n++;
	}
	if( !(mask & (XIMP_PRE_STD_COLORMAP_MASK(q) | XIMP_PRE_COLORMAP_MASK(q))) ) {
	    q->preedit.colormap = attr.colormap;
	    XtSetArg( args[n], XtNcolormap, q->preedit.colormap );	n++;
	}
	if( DefaultScreen(display) == XScreenNumberOfScreen(attr.screen) ) {
	    if( !(mask & XIMP_PRE_FG_MASK(q)) ) {
		q->preedit.foreground = default_foreground;
		XtSetArg( args[n], XtNforeground, default_foreground );	n++;
	    }
	    if( !(mask & XIMP_PRE_BG_MASK(q)) ) {
		q->preedit.background = default_background;
		XtSetArg( args[n], XtNbackground, default_background );	n++;
	    }
	}
	XtSetArg( args[n], XtNdepth, attr.depth );			n++;
	XtSetArg( args[n], XtNscreen, attr.screen );			n++;
	XtSetArg( args[n], XtNaccelerators, resources.base_accelerators );
									n++;
	sprintf( ss, "pre%08x", q->icid );
	q->buffer->preedit_widget = XmwCreatePreedit( TopLevel, ss, args, n );
	XtInstallAccelerators( q->buffer->preedit_widget,
			       q->buffer->preedit_widget );
	if( resources.accelerators ) {
	    XtOverrideTranslations( q->buffer->preedit_widget,
				    resources.accelerators );
	    XtInstallAccelerators( q->buffer->preedit_widget,
				   q->buffer->preedit_widget );
	}
    }
    else
	XtSetValues( q->buffer->preedit_widget, args, n );

    XtVaGetValues( q->buffer->preedit_widget,
		   XmwNareaNeededWidth, &width,
		   XmwNareaNeededHeight, &height, NULL );
    q->preedit_area.width = width;
    q->preedit_area.height = height;

    return( 0L );
}


static void
#if NeedFunctionPrototypes
set_status_values( XimpQueue *q, unsigned long mask )
#else /* NeedFunctionPrototypes */
set_status_values( q, mask )
    XimpQueue		*q;
    unsigned long	mask;
#endif /* NeedFunctionPrototypes */
{
    Display		*display = XtDisplay(TopLevel);
    int			ii, n;
    Arg			args[20];
    char		ss[100];
    Widget		widget;
    Dimension		width, height;
    XWindowAttributes	attr;
    XFontSet		fs = NULL;
    char		**missing_list;
    int			missing_count;
    char		*def_string;
    long		lbg;

    if( q->buffer->status_widget == NULL ) {
	if( XGetWindowAttributes( display, q->client_window, &attr ) == 0 )
	    return;
	if( !(mask & XIMP_STS_AREA_MASK(q)) ) {
	    q->status.area.width = attr.width;
	    q->status.area.height = attr.height;
	}
    }

    n = 0;
    XtSetArg( args[n], XtNx, q->status.area.x );			n++;
    XtSetArg( args[n], XtNy, q->status.area.y );			n++;
    if( (mask & XIMP_STS_AREANEED_MASK(q))  &&
	!(q->mask & XIMP_STS_AREA_MASK(q)) ) {
	XtSetArg( args[n], XtNwidth, q->status.area_needed.width );	n++;
	XtSetArg( args[n], XtNheight, q->status.area_needed.height );	n++;
    }
    else {
	XtSetArg( args[n], XtNwidth, q->status.area.width );		n++;
	XtSetArg( args[n], XtNheight, q->status.area.height );		n++;
    }
    if( mask & XIMP_STS_FG_MASK(q) ) {
	XtSetArg( args[n], XtNforeground, q->status.foreground );	n++;
    }
    if( mask & XIMP_STS_BG_MASK(q) ) {
	XtSetArg( args[n], XtNbackground, q->status.background );	n++;
    }
    if( q->buffer->status_widget == NULL ) {
	if( mask & (XIMP_STS_STD_COLORMAP_MASK(q) | XIMP_STS_COLORMAP_MASK(q)) ) {
	    XtSetArg( args[n], XtNcolormap, q->status.colormap );
	    n++;
	}
	else {
	    if( !resources.ignore_status_color ) {
		q->status.colormap = attr.colormap;
		XtSetArg( args[n], XtNcolormap, q->status.colormap );	n++;
	    }
	}
    }
    if( mask & XIMP_STS_CURSOR_MASK(q) ) {
	XtSetArg( args[n], XtNcursor, q->status.cursor );		n++;
    }
    if( mask & XIMP_STS_FONT_MASK(q) ) {
	fs = XCreateFontSet( display, q->status_font, &missing_list,
			     &missing_count, &def_string );
	if( missing_list )
	    XFree( missing_list );
	if( fs != NULL ) {
	    XtSetArg( args[n], XtNfontSet, fs );			n++;
	}
    }

    if( q->buffer->status_widget == NULL ) {
	if( !(mask & XIMP_STS_WINDOW_MASK(q)) ) {
	    XtSetArg( args[n], XmwNparentWindow, q->client_window );	n++;
	}
	XtSetArg( args[n], XtNmappedWhenManaged, False );		n++;
	if( !(mask & XIMP_STS_FG_MASK(q)) ) {
	    q->status.foreground = default_foreground;
	    XtSetArg( args[n], XtNforeground, default_foreground );	n++;
	}
	if( !(mask & XIMP_STS_BG_MASK(q)) ) {
	    q->status.background = default_background;
	    XtSetArg( args[n], XtNbackground, default_background );	n++;
	}
	XtSetArg( args[n], XtNdepth, attr.depth );			n++;
	XtSetArg( args[n], XtNscreen, attr.screen );			n++;

	sprintf( ss, "sts%08x", q->icid );
	widget = q->buffer->status_widget =
	    XmwCreateManagedStatusArea( TopLevel, ss, args, n );

	n = 0;
	if( fs ) {
	    XtSetArg( args[n], XtNfontSet, fs );			n++;
	}
	if( !resources.ignore_status_color ) {
	    XtSetArg( args[n], XtNcolormap, q->status.colormap );	n++;
	}
	XtSetArg( args[n], XtNborderColor, q->status.background );	n++;
	XtSetArg( args[n], XtNforeground, q->status.foreground );	n++;
	XtSetArg( args[n], XtNbackground, q->status.background );	n++;
	XtSetArg( args[n], XtNlabel, resources.sjxa );			n++;
	XtSetArg( args[n], XtNdepth, attr.depth );			n++;
	XtSetArg( args[n], XtNscreen, attr.screen );			n++;
	q->buffer->label = XtCreateWidget( "Label", labelWidgetClass, widget,
					   args, n );
	XtManageChild( q->buffer->label );

	if( !(q->style & XIMPreeditArea) ) {
/*
	    n = 0;
	    if( !resources.ignore_status_color ) {
		XtSetArg( args[n], XtNcolormap, q->status.colormap );	n++;
	    }
*/
	    ii = XScreenNumberOfScreen( attr.screen );
	    lbg = (depths[ii] == 1) ? q->status.background :
		  light_color( display, q->status.colormap,
			       q->status.background );
	    n = 0;
	    if( fs ) {
		XtSetArg( args[n], XtNfontSet, fs );			n++;
	    }
	    XtSetArg( args[n], XtNforeground, q->status.foreground );	n++;
	    XtSetArg( args[n], XtNbackground, q->status.background );	n++;
	    XtSetArg( args[n], XtNhorizDistance, 2 );			n++;
	    XtSetArg( args[n], XtNborderWidth, 0 );			n++;
	    XtSetArg( args[n], XtNmappedWhenManaged, False );		n++;
	    XtSetArg( args[n], XmwNitemBackground, lbg );		n++;
	    XtSetArg( args[n], XmwNitemCount, 5 );			n++;
	    XtSetArg( args[n], XmwNitemLength,
		      resources.function_par_length * 5 );		n++;
	    XtSetArg( args[n], XmwNitems, resources.function_items );	n++;
	    XtSetArg( args[n], XmwNnumRows, 5 );			n++;
	    XtSetArg( args[n], XmwNvisibleHorizontalItemCount, 5 );	n++;
	    XtSetArg( args[n], XmwNvisibleVerticalItemCount, 1 );	n++;
	    q->buffer->box[0] = XmwCreateList( widget, "LeftBox", args, n );
	    n = 0;
	    if( fs ) {
		XtSetArg( args[n], XtNfontSet, fs );			n++;
	    }
	    XtSetArg( args[n], XtNforeground, q->status.foreground );	n++;
	    XtSetArg( args[n], XtNbackground, q->status.background );	n++;
	    XtSetArg( args[n], XtNhorizDistance, 2 );			n++;
	    XtSetArg( args[n], XtNborderWidth, 0 );			n++;
	    XtSetArg( args[n], XtNmappedWhenManaged, False );		n++;
	    XtSetArg( args[n], XmwNitemBackground, lbg );		n++;
	    XtSetArg( args[n], XmwNitemCount, 5 );			n++;
	    XtSetArg( args[n], XmwNitemLength,
		      resources.function_par_length * 5 );		n++;
	    XtSetArg( args[n], XmwNitems,
		&resources.function_items[resources.function_par_length * 5] );
									n++;
	    XtSetArg( args[n], XmwNnumRows, 5 );			n++;
	    XtSetArg( args[n], XmwNvisibleHorizontalItemCount, 5 );	n++;
	    XtSetArg( args[n], XmwNvisibleVerticalItemCount, 1 );	n++;
	    q->buffer->box[1] = XmwCreateList( widget, "RightBox", args, n );
	    XtManageChildren( q->buffer->box, 2 );
	    XtAddCallback( q->buffer->box[0], XmwNsetCallback,
			   (XtCallbackProc)function_activate,
			   (caddr_t)q->buffer );
	    XtAddCallback( q->buffer->box[1], XmwNsetCallback,
			   (XtCallbackProc)function_activate,
			   (caddr_t)q->buffer );
	}
	XtMapWidget( q->buffer->status_widget );
    }
    else {
	XtSetValues( q->buffer->status_widget, args, n );
	if( (mask & XIMP_STS_FONT_MASK(q))  &&  fs ) {
	    XtVaSetValues( q->buffer->label, XtNfontSet, fs, NULL );
	    XtVaSetValues( q->buffer->box[0], XtNfontSet, fs, NULL );
	    XtVaSetValues( q->buffer->box[1], XtNfontSet, fs, NULL );
	}
	if( mask & XIMP_STS_FG_MASK(q) ) {
	    XtVaSetValues( q->buffer->label,
			   XtNbackground, q->status.foreground, NULL );
	    XtVaSetValues( q->buffer->box[0],
			   XtNforeground, q->status.foreground, NULL );
	    XtVaSetValues( q->buffer->box[1],
			   XtNforeground, q->status.foreground, NULL );
	}
	if( mask & XIMP_STS_BG_MASK(q) ) {
	    XtVaSetValues( q->buffer->label,
			   XtNforeground, q->status.background, NULL );
	    XtVaSetValues( q->buffer->box[0],
			   XtNbackground, q->status.background, NULL );
	    XtVaSetValues( q->buffer->box[1],
			   XtNbackground, q->status.background, NULL );
	}
    }

    XtVaGetValues( q->buffer->status_widget,
		   XmwNareaNeededWidth, &width,
		   XmwNareaNeededHeight, &height, NULL );
    q->status_area.width = width;
    q->status_area.height = height;

    if( !(q->style & XIMPreeditArea)  &&  q->connect_flag ) {
	if( width > q->status.area.width ) {
	    XtUnmapWidget( q->buffer->box[0] );
	    XtUnmapWidget( q->buffer->box[1] );
	}
	else {
	    XtMapWidget( q->buffer->box[0] );
	    XtMapWidget( q->buffer->box[1] );
	}
    }
}


static XimpQueue *
#if NeedFunctionPrototypes
add_ximp_queue( Window client, unsigned long protocol, unsigned long style,
		unsigned long mask, unsigned long event_mask )
#else /* NeedFunctionPrototypes */
add_ximp_queue( client, protocol, style, mask, event_mask )
    Window		client;
    unsigned long	protocol, style, mask, event_mask;
#endif /* NeedFunctionPrototypes */
{
    Display		*display = XtDisplay(TopLevel);
    XimpQueue		*q1, *q2;
    static ICID		icid = 0x0001;

    if( protocol == 40  &&  !(mask & XIMP_SERVERTYPE_MASK4) )
	return( NULL );
    if( (q1 = (XimpQueue *)calloc( 1, sizeof(XimpQueue) )) == NULL )
	return( NULL );
    if( resources.ignore_preedit_color )
	mask &= ~(XIMP_PRE_FG_MASK(q1) |
		  XIMP_PRE_BG_MASK(q1) |
		  XIMP_PRE_COLORMAP_MASK(q1) );
    if( resources.ignore_status_color )
	mask &= ~(XIMP_STS_FG_MASK(q1) |
		  XIMP_STS_BG_MASK(q1) |
		  XIMP_STS_COLORMAP_MASK(q1) );
    q1->client_window = client;
    q1->focus_window = client;
    q1->icid = icid++;
    q1->protocol = protocol;
    q1->style = style;
    q1->ximp_type = XIMP_FE_TYPE1;
    q1->mask = mask;
    q1->event_mask = (protocol == 40) ? event_mask : KeyPressMask;

    if( !get_ximp_queue_data( q1, mask ) ) {
	free( q1 );
	return( NULL );
    }

    if( style == (XIMPreeditNothing | XIMStatusNothing) )
	q1->buffer = &root_window;
    else {
	if( (q1->buffer = buffer_alloc()) == NULL ) {
	    free( q1 );
	    return( NULL );
	}
	q1->buffer->conversion_style = style;
    }

    if( style & (XIMPreeditPosition | XIMPreeditArea) ) {
	set_preedit_values( q1, q1->mask );
	if( mask & XIMP_PRE_AREA_MASK(q1) )
	    q1->mask &= ~XIMP_PRE_AREANEED_MASK(q1);
    }
    if( style & XIMStatusArea ) {
	set_status_values( q1, q1->mask );
	if( mask & XIMP_STS_AREA_MASK(q1) )
	    q1->mask &= ~XIMP_STS_AREANEED_MASK(q1);
    }

    if( style & XIMStatusNothing ) {
	q1->buffer->label = RootLabel;
	q1->buffer->box[0] = Box[0];
	q1->buffer->box[1] = Box[1];
    }

    if( (q2 = ximp_queue) == NULL )
	ximp_queue = q1;
    else {
	while( q2->next != NULL )
	    q2 = q2->next;
	q2->next = q1;
	q1->previous = q2;
    }

    if( q1->status.colormap == None ) {
	Window			win;
	XWindowAttributes	attr;

	win = q1->status.window;
	if( win == None )
	    win = q1->client_window;
	XGetWindowAttributes( display, win, &attr );
	q1->status.colormap = attr.colormap;
    }
    if( mask & XIMP_STS_FG_MASK(q1) ) {
	q1->icon_fg = q1->status.foreground;
    }
    else
	q1->icon_fg = XmwICON_DEFAULT_COLOR;
    if( mask & XIMP_STS_BG_MASK(q1) ) {
	q1->icon_bg = q1->status.background;
    }
    else
	q1->icon_bg = XmwICON_DEFAULT_COLOR;

    return( q1 );
}


static void
#if NeedFunctionPrototypes
delete_ximp_queue( XimpQueue *q )
#else /* NeedFunctionPrototypes */
delete_ximp_queue( q )
    XimpQueue	*q;
#endif /* NeedFunctionPrototypes */
{
    if( q->previous )
	q->previous->next = q->next;
    else
	ximp_queue = q->next;
    if( q->next )
	q->next->previous = q->previous;

    if( q->connect_flag ) {
	if( q->buffer->error_shell ) {
	    error_cancel( (Widget)NULL, (caddr_t)q->buffer, (caddr_t)NULL );
	}

	if( q->buffer->state & DELETE )
	    sakujo_cancel( (Widget)NULL, (caddr_t)q->buffer, (caddr_t)NULL );
	if( q->buffer->state & CODE )
	    code_cancel( (Widget)NULL, (caddr_t)q->buffer, (caddr_t)NULL );
	if( q->buffer->state & ADD )
	    touroku_cancel( (Widget)NULL, (caddr_t)q->buffer, (caddr_t)NULL );
	if( q->buffer->state & SERVER )
	    reconnect_cancel( (Widget)NULL, (caddr_t)q->buffer, (caddr_t)NULL );
	if( q->buffer->state & SELECT ) {
	    if( q->style == (XIMPreeditNothing | XIMStatusNothing) ) {
		XtUnmapWidget( RootTextField );
		XtMapWidget( q->buffer->label );
	    }
	    else if( q->style & (XIMPreeditPosition | XIMPreeditArea) )
		select_dialog_cancel( (Widget)NULL, (caddr_t)q->buffer,
				      (caddr_t)NULL );
	}
	if( q->buffer->state & EDIT )
	    edit_cancel( q->buffer );
	if( q->buffer->state & (WRAP | CONVERSION) )
	    henkan_cancel( q->buffer );
    }
    if( q->preedit_font )	free( q->preedit_font );
    if( q->status_font )	free( q->status_font );
    if( !(q->style & XIMPreeditNothing) )
	XtDestroyWidget( q->buffer->preedit_widget );
    if( q->buffer->status_widget )
	XtDestroyWidget( q->buffer->status_widget );
    if( q->style != (XIMPreeditNothing | XIMStatusNothing) ) {
	if( buffer == q->buffer )
	    sort_buffer( &root_window );
	buffer_free( q->buffer );
    }
    else
	q->buffer->connect--;
    free( q );
}


static XimpQueue *
#if NeedFunctionPrototypes
FocusWindow2XimpQueue( Window f )
#else
FocusWindow2XimpQueue( f )
    Window	f;
#endif
{
    XimpQueue	*p, *q;

    for( p = NULL, q = ximp_queue; q != NULL; q = q->next ) {
	if( (q->mask & XIMP_FOCUS_WIN_MASK(q))  &&  q->focus_window == f ) {
	    if( q->has_focus )
		return( q );
	    p = q;
	}
    }
    if( p ) {
	for( q = ximp_queue; q != NULL; q = q->next ) {
	    if( p->client_window == q->client_window  &&  q->has_focus ) {
		if( q->connect_flag )
		    return( q );
		else
		    p = q;
	    }
	}
    }
    return( p );
}


static XimpQueue *
#if NeedFunctionPrototypes
PreeditWindow2XimpQueue( Window w )
#else
PreeditWindow2XimpQueue( w )
    Window	w;
#endif
{
    XimpQueue	*q;

    for( q = ximp_queue; q != NULL; q = q->next )
	if( q->buffer->preedit_widget  &&
	    XtWindow(q->buffer->preedit_widget) == w )
	    return( q );
    return( NULL );
}


static XimpQueue *
#if NeedFunctionPrototypes
StatusWindow2XimpQueue( Window w )
#else
StatusWindow2XimpQueue( w )
    Window	w;
#endif
{
    XimpQueue	*q;

    for( q = ximp_queue; q!= NULL; q = q->next )
	if( q->buffer->status_widget )
	    if( XtWindow(q->buffer->status_widget) == w  ||
		XtWindow(q->buffer->label) == w  ||
		XtWindow(q->buffer->box[0]) == w  ||
		XtWindow(q->buffer->box[1]) == w )
		    return( q );
    return( NULL );
}


static XimpQueue *
#if NeedFunctionPrototypes
ICID2XimpQueue( ICID icid )
#else
ICID2XimpQueue( icid )
    ICID	icid;
#endif
{
    XimpQueue	*q;

    for( q = ximp_queue; q != NULL; q = q->next )
	if( q->icid == icid )
	    return( q );
    return( NULL );
}


static void
#if NeedFunctionPrototypes
XimpKeyPress( XimpQueue *q, XClientMessageEvent *event )
#else
XimpKeyPress( q, event )
    XimpQueue		*q;
    XClientMessageEvent	*event;
#endif
{
    XKeyEvent		key;

    key.type = KeyPress;
    key.serial = event->serial;
    key.send_event = False;
    key.display = event->display;
    key.window = q->focus_window;
    key.root = DefaultRootWindow(event->display);
    key.time = ISXimp3(q) ? 0 : event->data.l[4];
    key.x = 0;
    key.y = 0;
    key.x_root = 0;
    key.y_root = 0;
    key.state = event->data.l[3];
    key.keycode = event->data.l[2];
    key.same_screen = True;
    XPutBackEvent( event->display, (XEvent *)&key );
}


static void
#if NeedFunctionPrototypes
XimpKeyRelease( XimpQueue *q, XClientMessageEvent *event )
#else
XimpKeyRelease( q, event )
    XimpQueue		*q;
    XClientMessageEvent	*event;
#endif
{
    if( !q->connect_flag )
	send_client_message( event->display, q->focus_window,
			     (long)XIMP_KEYRELEASE4, (long)q->icid,
			     (long)event->data.l[2], (long)event->data.l[3],
			     (long)event->data.l[4] );
    if( ISSYNC(q) )
	send_client_message( event->display, q->focus_window,
			     (long)XIMP_KEYRELEASE_RETURN4, (long)q->icid,
			     0, 0, 0 );
}


static void
#if NeedFunctionPrototypes
XimpCreate( XClientMessageEvent *event )
#else
XimpCreate( event )
    XClientMessageEvent	*event;
#endif
{
    Window		win;
    unsigned long	protocol, style, mask;
    XimpQueue		*q;
    int			ii, jj;

    protocol = (event->data.l[0] < 100) ? 35 : 40;
    win = event->data.l[1];
    style = event->data.l[2];
    jj = sizeof(supported_styles) / sizeof(unsigned long);
    for( ii = 0; ii < jj; ii++ )
	if( style == supported_styles[ii] )
	    break;
    if( ii == jj ) {
	send_error_notify( event->display, win, protocol, NULL,
			   XIMP_CREATE(protocol), XIMP_BadStyle );
	return;
    }

    for( q = ximp_queue; q != NULL; q = q->next ) {
	if( q->client_window == win ) {
	    if( q->protocol != protocol ) {
		send_error_notify( event->display, win, protocol, NULL,
				   XIMP_CREATE(protocol), XIMP_BadProtocol );
		return;
	    }
	}
    }

    mask = event->data.l[3];
    if( ((style & (XIMPreeditPosition | XIMPreeditArea))  &&
	 !(mask & _XIMP_PRE_FONT_MASK(protocol)))  ||
	((style & XIMPreeditPosition)  &&
	 !(mask & _XIMP_PRE_SPOTL_MASK(protocol)))  ||
	((style & XIMStatusArea)  &&
	 !(mask & _XIMP_STS_FONT_MASK(protocol))) ) {
	send_error_notify( event->display, win, protocol, NULL,
			   XIMP_CREATE(protocol), XIMP_BadProtocol );
	return;
    }

    if( (q = add_ximp_queue( win, protocol, style, mask, event->data.l[4] ))
	== NULL ) {
	send_error_notify( event->display, win, protocol, NULL,
			   XIMP_CREATE(protocol), XIMP_BadProperty );
    }
    else {
	if( IsRegisteredWindow( XIM_PROTOCOL, q->client_window,
			       q->focus_window ) ) {
	    send_error_notify( event->display, win, protocol, NULL,
			       XIMP_CREATE(protocol), XIMP_BadProtocol );
	    return;
	}
	if( win != q->focus_window )
	    XSelectInput( event->display, win,
			  ExposureMask | StructureNotifyMask );
	switch( q->ximp_type ) {
	  case XIMP_FE_TYPE1:
	  case XIMP_BE_TYPE1:
	  case XIMP_BE_TYPE2:
	  case XIMP_SYNC_BE_TYPE1:
	  case XIMP_SYNC_BE_TYPE2:
	    XSelectInput( event->display, q->focus_window,
			  ExposureMask | StructureNotifyMask );
	    break;

	  case XIMP_FE_TYPE2:
	  case XIMP_FE_TYPE3:
	    XSelectInput( event->display, q->focus_window,
			  KeyPressMask | ExposureMask | StructureNotifyMask );
	    break;
	}
	send_client_message( event->display, win, (long)XIMP_CREATE_RETURN(q),
			     (long)q->icid, 0L, 0L, 0L );
    }
}


static void
#if NeedFunctionPrototypes
XimpDestroy( XimpQueue *q )
#else
XimpDestroy( q )
    XimpQueue	*q;
#endif
{
    if( q->connect_flag ) {
	if( q->style & (XIMPreeditNothing | XIMStatusNothing) )
	    RootWindowUnmap( q->style );
    }
    XSelectInput( XtDisplay(TopLevel), q->client_window, NoEventMask );
    delete_ximp_queue( q );
}


static void
#if NeedFunctionPrototypes
XimpBegin( XimpQueue *q )
#else
XimpBegin( q )
    XimpQueue	*q;
#endif
{
    Display	*display = XtDisplay(TopLevel);

    if( !q->connect_flag ) {
	q->connect_flag = True;
	q->buffer->connect++;
	q->has_focus = True;
	if( q->style & (XIMPreeditNothing | XIMStatusNothing) )
	    RootWindowMap( q->buffer, q->style );
	if( q->style & (XIMPreeditPosition | XIMPreeditArea) ) {
	    buffer_initialize( q->buffer );
	    XmwPreeditMapWidget( q->buffer->preedit_widget );
	}
	if( (q->style & XIMStatusArea)  &&  q->buffer->status_widget ) {
	    set_label( q->buffer, q->buffer->conversion_mode );
	    if( !(q->style & XIMPreeditArea)
	      &&  q->status.area.width >= q->status_area.width ) {
		XtMapWidget( q->buffer->box[0] );
		XtMapWidget( q->buffer->box[1] );
	    }
	}
	if( ISFE1(q) ) {
	    XSelectInput( display, q->focus_window,
			  KeyPressMask | ExposureMask | StructureNotifyMask );
	    if( ISXimp4(q) ) {
		send_client_message( display, q->focus_window,
				     (long)XIMP_PROCESS_BEGIN(q),
				     q->icid, 0L, 0L, 0L );
	    }
	}
    }
    else {
	if( ISFE3(q) ) {
	    send_client_message( display, q->focus_window,
				 (long)XIMP_PROCESS_BEGIN(q), q->icid,
				 0L, 0L, 0L );
	}
    }

    if( q->style & (XIMPreeditPosition | XIMPreeditArea) ) {
	XtVaSetValues( q->buffer->preedit_widget, XmwNfocus, True, NULL );
	XtSetKeyboardFocus( TopLevel, q->buffer->preedit_widget );
    }
    set_buffer( q->buffer->preedit_widget, KeyPress );
}


static void
#if NeedFunctionPrototypes
XimpEnd( XimpQueue *q )
#else
XimpEnd( q )
    XimpQueue	*q;
#endif
{
    if( q->connect_flag ) {
	q->connect_flag = False;
	q->buffer->connect--;
	send_client_message( XtDisplay(TopLevel), q->focus_window,
			     (long)XIMP_PROCESS_END(q), q->icid, 0L, 0L, 0L );
	if( q->style & (XIMPreeditNothing | XIMStatusNothing) )
	    RootWindowUnmap( q->style );
	if( q->style & (XIMPreeditPosition | XIMPreeditArea) )
	    XmwPreeditUnmapWidget( q->buffer->preedit_widget );
	if( ISFE1(q) )
	    XSelectInput( XtDisplay(TopLevel), q->focus_window,
			  ExposureMask | StructureNotifyMask );
    }
}


static void	XimpSetFocus( q )
XimpQueue	*q;
{
    if( q->style & (XIMPreeditPosition | XIMPreeditArea) ) {
	XtVaSetValues( q->buffer->preedit_widget, XmwNfocus, True, NULL );
	XtSetKeyboardFocus( TopLevel, q->buffer->preedit_widget );
	if( q->buffer->state == SELECT )
	    XRaiseWindow( XtDisplay(q->buffer->shell),
			  XtWindow(q->buffer->shell) );
    }
    if( q->buffer->error_shell )
	XRaiseWindow( XtDisplay(q->buffer->error_shell),
		      XtWindow(q->buffer->error_shell) );

    if( q->buffer->state & DELETE ) {
	XRaiseWindow( XtDisplay(delete_shell), XtWindow(delete_shell) );
	XRaiseWindow( XtDisplay(question_shell), XtWindow(question_shell) );
    }
    if( q->buffer->state & ADD )
	XRaiseWindow( XtDisplay(q->buffer->shell), XtWindow(q->buffer->shell) );
    if( q->buffer->state & CODE )
	XRaiseWindow( XtDisplay(q->buffer->shell), XtWindow(q->buffer->shell) );
    if( q->buffer->state & SERVER )
	XRaiseWindow( XtDisplay(server_shell), XtWindow(server_shell) );

    if( (q->style & XIMStatusArea)  &&  q->buffer->status_widget )
	XRaiseWindow( XtDisplay(q->buffer->status_widget),
		      XtWindow(q->buffer->status_widget) );
    else if( q->style & XIMStatusNothing ) {
	q->buffer->conversion_mode = root_window.conversion_mode;
	q->buffer->conversion_mode_toggle = root_window.conversion_mode_toggle;
    }
    q->has_focus = True;
    set_buffer( q->buffer->preedit_widget, KeyPress );
}


static void	XimpUnsetFocus( p )
XimpQueue	*p;
{
    if( p->style & (XIMPreeditPosition | XIMPreeditArea) )
	XtVaSetValues( p->buffer->preedit_widget, XmwNfocus, False, NULL );
    p->has_focus = False;
}


static void	XimpMove( q, x, y )
XimpQueue	*q;
Position	x, y;
{
    Arg		args[2];

    if( q->style & XIMPreeditPosition ) {
	q->preedit.spot_location.x = x;
	q->preedit.spot_location.y = y;
	XtSetArg( args[0], XmwNspotLocationX, x );
	XtSetArg( args[1], XmwNspotLocationY, y - q->preedit_ascent );
	XtSetValues( q->buffer->preedit_widget, args, 2 );
#ifdef	DEBUG
	if( resources.debug & 2 ) {
	    printf( " Set SpotLocation : x : %d\n", (int)x );
	    printf( " Set SpotLocation : y : %d\n", (int)y );
	}
#endif
    }
}


static void	XimpSetValue( q, mask )
XimpQueue	*q;
unsigned long	mask;
{
    if( resources.ignore_preedit_color )
	mask &= ~(XIMP_PRE_FG_MASK(q) |
		  XIMP_PRE_BG_MASK(q) |
		  XIMP_PRE_COLORMAP_MASK(q) );
    if( resources.ignore_status_color )
	mask &= ~(XIMP_STS_FG_MASK(q) |
		  XIMP_STS_BG_MASK(q) |
		  XIMP_STS_COLORMAP_MASK(q) );
    get_ximp_queue_data( q, mask );
    q->mask |= mask;
    if( q->style & (XIMPreeditPosition | XIMPreeditArea) ) {
	if( mask & (XIMP_PRE_MASK(q) | XIMP_FOCUS_WIN_MASK(q)) ) {
	    set_preedit_values( q, mask );
	}
    }
    if( (q->style & XIMStatusArea)  &&  q->buffer->status_widget
       &&  (mask & (XIMP_STS_MASK(q) | XIMP_STS_FONT_MASK(q) | XIMP_FOCUS_WIN_MASK(q))) ) {
	set_status_values( q, mask );
	if( mask & XIMP_STS_AREA_MASK(q) )
	    q->mask &= ~XIMP_STS_AREANEED_MASK(q);
    }

    if( mask & (XIMP_STS_FG_MASK(q) | XIMP_STS_BG_MASK(q) | XIMP_STS_COLORMAP_MASK(q)) ) {
	if( q->status.colormap == None ) {
	    Window		win;
	    XWindowAttributes	attr;

	    win = q->status.window;
	    if( win == None )
		win = q->client_window;
	    XGetWindowAttributes( XtDisplay(TopLevel), win, &attr );
	    q->status.colormap = attr.colormap;
	}
	if( mask & XIMP_STS_FG_MASK(q) )
	    q->icon_fg = q->status.foreground;
	else
	    q->icon_fg = XmwICON_DEFAULT_COLOR;
	if( mask & XIMP_STS_BG_MASK(q) )
	    q->icon_bg = q->status.background;
	else
	    q->icon_bg = XmwICON_DEFAULT_COLOR;
    }
}


static void	XimpClientWindow( q, win, mask )
XimpQueue	*q;
Window		win;
long		mask;
{
    Display		*display = XtDisplay(TopLevel);
    Window		 root, old_focus;
    XWindowAttributes	 attr;

#ifdef DEBUG
    if( resources.debug & 2 )
	printf( " window = %08x\n", win );
#endif

    XSelectInput( display, q->client_window, NoEventMask );
    XSelectInput( display, win, ExposureMask | StructureNotifyMask );

    if( !(q->mask & XIMP_FOCUS_WIN_MASK4) )
	q->event_mask = mask;

    XGetWindowAttributes( display, q->client_window, &attr );
    root = attr.root;
    XGetWindowAttributes( display, win, &attr );
    if( root == attr.root ) {
	if( (q->style & XIMPreeditPosition)  &&
	    !(q->mask & XIMP_FOCUS_WIN_MASK4) ) {
	    XtVaSetValues( q->buffer->preedit_widget, XmwNfocusWindow, win,
			   NULL );
	    q->focus_window = win;
	    if( (ISFE1(q)  &&  q->connect_flag)  ||  ISFE2(q)  ||  ISFE3(q) )
		XSelectInput( display, win,
			      KeyPressMask | ExposureMask |
			      StructureNotifyMask );
	}
	else if( q->style & XIMPreeditArea ) {
	    XtVaSetValues( q->buffer->preedit_widget, XmwNfocusWindow, win,
			   NULL );
	    if( !(q->mask & XIMP_FOCUS_WIN_MASK4) )
		q->focus_window = win;
	    if( (ISFE1(q)  &&  q->connect_flag)  ||  ISFE2(q)  ||  ISFE3(q) )
		XSelectInput( display, win,
			      KeyPressMask | ExposureMask |
			      StructureNotifyMask );
	}

	if( q->style & XIMStatusArea )
	    XtVaSetValues( q->buffer->status_widget, XmwNparentWindow, win,
			   NULL );

	q->client_window = win;
    }
    else {
	old_focus = q->focus_window;
	if( !(q->mask & XIMP_FOCUS_WIN_MASK4)  ||
	    q->focus_window == q->client_window )
	    q->focus_window = win;
	q->client_window = win;

	if( q->style & (XIMPreeditPosition | XIMPreeditArea) ) {
	    XtRemoveGrab( q->buffer->preedit_widget );
	    XtDestroyWidget( q->buffer->preedit_widget );
	    q->buffer->preedit_widget = NULL;
	    set_preedit_values( q, q->mask );
	    if( old_focus != win ) {
		if( (ISFE1(q)  &&  q->connect_flag)  ||  ISFE2(q)  ||
		    ISFE3(q) )
		    XSelectInput( display, win, KeyPressMask | ExposureMask |
				  StructureNotifyMask );
	    }
	}
	if( q->style & XIMStatusArea ) {
	    XtDestroyWidget( q->buffer->status_widget );
	    q->buffer->status_widget = NULL;
	    set_status_values( q, q->mask );
	}

    }
    send_client_message( display, q->focus_window,
			 (long)XIMP_CLIENT_WINDOW_RETURN4, q->icid,
			 0L, 0L, 0L );
}


static void	XimpFocusWindow( q, win, mask )
XimpQueue	*q;
Window		win;
long		mask;
{
    Display		*display = XtDisplay(TopLevel);
    Window		old;
    XWindowAttributes	attr;

#ifdef DEBUG
    if( resources.debug & 2 )
	printf( " widow = %08x\n", win );
#endif
    if( q->focus_window != q->client_window )
	XSelectInput( XtDisplay(TopLevel), q->focus_window, NoEventMask );
    else {
	if( ISFE2(q)  ||  ISFE3(q) )
	    XSelectInput( display, q->client_window,
			  ExposureMask | StructureNotifyMask );
    }
    old = q->focus_window;
    q->focus_window = win;
    q->event_mask = mask;
    switch( q->ximp_type ) {
      case XIMP_FE_TYPE1:
	if( q->connect_flag ) {
	    XSelectInput( display, win,
			  KeyPressMask | ExposureMask | StructureNotifyMask );
	    break;
	}
      case XIMP_BE_TYPE1:
      case XIMP_BE_TYPE2:
      case XIMP_SYNC_BE_TYPE1:
      case XIMP_SYNC_BE_TYPE2:
	XSelectInput( display, win, ExposureMask | StructureNotifyMask );
	break;

      case XIMP_FE_TYPE2:
      case XIMP_FE_TYPE3:
	XSelectInput( display, win,
		      KeyPressMask | ExposureMask | StructureNotifyMask );
	break;
    }

    q->mask |= XIMP_FOCUS_WIN_MASK4;
    if( q->style & XIMPreeditPosition ) {
	set_preedit_values( q, XIMP_FOCUS_WIN_MASK4 );
	if( !(q->mask & XIMP_PRE_AREA_MASK4) ) {
	    if( XGetWindowAttributes( display, win, &attr ) != 0 ) {
		q->preedit.area.width = attr.width - q->preedit.area.x;
		q->preedit.area.height = attr.height - q->preedit.area.y;
		XtVaSetValues( q->buffer->preedit_widget,
			       XmwNareaWidth, q->preedit.area.width,
			       XmwNareaHeight, q->preedit.area.height, NULL );
	    }
	}
    }

    send_client_message( display, old, (long)XIMP_FOCUS_WINDOW_RETURN4,
			 q->icid, 0L, 0L, 0L );
}


static void	XimpGetValue( q, mask )
XimpQueue	*q;
unsigned long	mask;
{
    Display	*display = XtDisplay(TopLevel);
    Window	win;
    long	prop[15];
    Boolean	change;

    win = q->client_window;

    if( mask & XIMP_FOCUS_WIN_MASK(q) ) {
	XChangeProperty( display, XtWindow(TopLevel), ximp_focus, XA_WINDOW, 32,
			 PropModeAppend, (unsigned char *)(&q->client_window), 1 );
    }
    change = False;
    if( mask & XIMP_PRE_AREA_MASK(q) ) {
	prop[0] = q->preedit.area.x;
	prop[1] = q->preedit.area.y;
	prop[2] = q->preedit.area.width;
	prop[3] = q->preedit.area.height;
#ifdef	DEBUG
	if( resources.debug & 2 ) {
	    printf( " Get PreeditArea : width  : %d\n", (int)prop[2] );
	    printf( " Get PreeditArea : height : %d\n", (int)prop[3] );
	}
#endif
	change = True;
    }
    if( mask & XIMP_PRE_FG_MASK(q) ) {
	prop[ISXimp3(q)?4:10] = q->preedit.foreground;
	change = True;
    }
    if( mask & XIMP_PRE_BG_MASK(q) ) {
	prop[ISXimp3(q)?5:11] = q->preedit.background;
	change = True;
    }
    if( mask & XIMP_PRE_COLORMAP_MASK(q) ) {
	prop[ISXimp3(q)?6:8] = q->preedit.colormap;
	change = True;
    }
    if( ISXimp4(q)  &&  mask & XIMP_PRE_STD_COLORMAP_MASK(q) ) {
	prop[9] = q->preedit.std_colormap;
	change = True;
    }
    if( mask & XIMP_PRE_BGPIXMAP_MASK(q) ) {
	prop[ISXimp3(q)?7:12] = q->preedit.bg_pixmap;
	change = True;
    }
    if( mask & XIMP_PRE_LINESP_MASK(q) ) {
	prop[ISXimp3(q)?8:13] = q->preedit.line_spacing;
	change = True;
    }
    if( mask & XIMP_PRE_CURSOR_MASK(q) ) {
	prop[ISXimp3(q)?9:14] = q->preedit.cursor;
	change = True;
    }
    if( mask & XIMP_PRE_AREANEED_MASK(q) ) {
	prop[ISXimp3(q)?10:4] = q->preedit_area.width;
	prop[ISXimp3(q)?11:5] = q->preedit_area.height;
#ifdef	DEBUG
	if( resources.debug & 2 ) {
	    printf( " Get PreeditAreaNeeded : width  : %d\n",
		    q->preedit_area.width );
	    printf( " Get PreeditAreaNeeded : height : %d\n",
		    q->preedit_area.height );
	}
#endif
	change = True;
    }
    if( mask & XIMP_PRE_SPOTL_MASK(q) ) {
	prop[ISXimp3(q)?12:6] = q->preedit.spot_location.x;
	prop[ISXimp3(q)?13:7] = q->preedit.spot_location.y;
	change = True;
    }
    if( change ) {
	XChangeProperty( display, win, ximp_preedit, ximp_preedit, 32,
			PropModeAppend, (unsigned char *)prop,
			ISXimp3(q)?14:15 );
    }

    if( mask & XIMP_PRE_FONT_MASK(q) ) {
	XChangeProperty( display, win, ximp_preedit_font, XA_STRING, 8,
			PropModeAppend, (unsigned char *)q->preedit_font, strlen(q->preedit_font) );
    }
    
    change = False;
    if( mask & XIMP_STS_AREA_MASK(q) ) {
	prop[0] = q->status.area.x;
	prop[1] = q->status.area.y;
	prop[2] = q->status.area.width;
	prop[3] = q->status.area.height;
#ifdef	DEBUG
	if( resources.debug & 2 ) {
	    printf( " Get StatusArea : width  : %d\n", (int)prop[2] );
	    printf( " Get StatusArea : height : %d\n", (int)prop[3] );
	}
#endif
	change = True;
    }
    if( mask & XIMP_STS_FG_MASK(q) ) {
	prop[ISXimp3(q)?4:8] = q->status.foreground;
	change = True;
    }
    if( mask & XIMP_STS_BG_MASK(q) ) {
	prop[ISXimp3(q)?5:9] = q->status.background;
	change = True;
    }
    if( mask & XIMP_STS_COLORMAP_MASK(q) ) {
	prop[6] = q->status.colormap;
	change = True;
    }
    if( ISXimp4(q)  &&  mask & XIMP_STS_STD_COLORMAP_MASK(q) ) {
	prop[7] = q->status.std_colormap;
	change = True;
    }
    if( mask & XIMP_STS_BGPIXMAP_MASK(q) ) {
	prop[ISXimp3(q)?7:10] = q->status.bg_pixmap;
	change = True;
    }
    if( mask & XIMP_STS_LINESP_MASK(q) ) {
	prop[ISXimp3(q)?8:11] = q->status.line_spacing;
	change = True;
    }
    if( mask & XIMP_STS_CURSOR_MASK(q) ) {
	prop[ISXimp3(q)?9:12] = q->status.cursor;
	change = True;
    }
    if( (mask & XIMP_STS_AREANEED_MASK(q))  &&  (q->style & XIMStatusArea) ) {
	if( q->buffer->status_widget ) {
	    Position	x;
	    Dimension	width, height, margin_height;

	    XtVaGetValues( q->buffer->label, XtNx, &x,
			   XtNwidth, &width, XtNheight, &height, NULL );
	    XtVaGetValues( q->buffer->status_widget, XmwNmarginHeight,
			   &margin_height, NULL );

	    prop[ISXimp3(q)?10:4] = q->status_area.width;
	    prop[ISXimp3(q)?11:5] = q->status_area.height;

	    if( q->mask & XIMP_STS_AREANEED_MASK(q) ) {
		if( prop[ISXimp3(q)?10:4] > q->status.area_needed.width ) {
		    prop[ISXimp3(q)?10:4] = x * 2 + width;
		    prop[ISXimp3(q)?11:5] = height + margin_height * 2;
		}
	    }
	    else {
		if( prop[ISXimp3(q)?10:4] > q->status.area.width ) {
		    prop[ISXimp3(q)?10:4] = x * 2 + width;
		    prop[ISXimp3(q)?11:5] = height + margin_height * 2;
		}
	    }
#ifdef	DEBUG
	    if( resources.debug & 2 ) {
		printf( " Get StatusAreaNeeded : width  : %d\n",
			(int)prop[ISXimp3(q)?10:4] );
		printf( " Get StatusAreaNeeded : height : %d\n",
			(int)prop[ISXimp3(q)?11:5] );
	    }
#endif
	}
	else {
	    prop[ISXimp3(q)?10:4] = q->status_area.width;
	    prop[ISXimp3(q)?11:5] = 0;
	}
	change = True;
    }
    if( change ) {
	XChangeProperty( display, win, ximp_status, ximp_status, 32,
			 PropModeAppend, (unsigned char *)prop, 13 );
    }

    if( mask & XIMP_STS_FONT_MASK(q) ) {
	XChangeProperty( display, win, ximp_status_font, XA_STRING, 8,
			 PropModeAppend, (unsigned char *)q->status_font,
			 strlen(q->status_font) );
    }

    if( mask )
	send_client_message( display, q->focus_window, (long)XIMP_GETVALUE_RETURN(q),
			     q->icid, 0L, 0L, 0L );
}


static void	XimpReset( q, event )
XimpQueue		*q;
XClientMessageEvent	*event;
{
    wchar_t		*ss, wcs[512];
    int			ii;
    Buffer		*b;
    unsigned int	mode;
    XTextProperty	text;
    wchar_t		*list[1];

    b = q->buffer;
    if( !q->connect_flag ) {
	text.nitems = 0;
	text.value = (unsigned char *)malloc( 1 );
	text.value[0] = '\0';
    }
    else {
	if( b->state & ~(INTERMEDIATE | CONVERSION) ) {
	    ii = 0;
	    cancel( b->preedit_widget, event, NULL, &ii );
	}
	if( b->state & INTERMEDIATE ) {
	    register_buffer( b );
	    wscpy( wcs, b->yomi );
	}
	else if( b->state & CONVERSION ) {
	    for( ii = 0, ss = wcs; b->henkan->bunsetsu[ii] != BUNSETSU_END;
		 ii++ ) {
		if( b->henkan->bun[ii].deststr == NULL ) {
		    wsncpy( ss, b->henkan->bun[ii].srcstr,
			    b->henkan->bun[ii].srclen );
		    ss += b->henkan->bun[ii].srclen;
		}
		else {
		    wsncpy( ss, b->henkan->bun[ii].deststr,
			    b->henkan->bun[ii].destlen );
		    ss += b->henkan->bun[ii].destlen;
		}
	    }
	    *ss = '\0';
	    register_buffer( b );
	}
	if( wcs[0] != (wchar_t)NULL ) {
	    list[0] = wcs;
	    XwcTextListToTextProperty( event->display, list, 1,
				       XCompoundTextStyle, &text );
	}
	else {
	    text.nitems = 0;
	    text.value = (unsigned char *)malloc( 1 );
	    text.value[0] = '\0';
	}
    }

    if( ISXimp3(q) ) {
	XChangeProperty( event->display, XtWindow(TopLevel), ximp_reset,
			 ximp_compound_text, 8, PropModeAppend,
			 text.value, text.nitems );
	send_client_message( event->display, q->focus_window,
			     (long)XIMP_RESET_RETURN3, (long)q->icid,
			     (long)ximp_reset, 0L, 0L );
    }
    else {
	if( text.nitems == 0 ) {
	    mode = NO_RESET_DATA;
	}
	else if( resources.reset_message ) {
	    mode = RESET_DATA_VIA_CM;
	    send_multi_client_message( event->display, q->focus_window, q->icid,
				       text.nitems, text.value );
	}
	else {
	    mode = RESET_DATA_VIA_PROP;
	    XChangeProperty( event->display, XtWindow(TopLevel), ximp_reset,
			     ximp_compound_text, 8, PropModeAppend,
			     text.value, text.nitems );
	}
	send_client_message( event->display, q->focus_window,
			     (long)XIMP_RESET_RETURN4, (long)q->icid, mode,
			     (long)ximp_reset, 0L );
    }
    XFree( text.value );

    q->buffer->state = INTERMEDIATE;
    ximp_end( event->display, q->focus_window );

#ifdef USE_SJ3LIB
    if( !CheckState( CONVERSION|EDIT|ADD|DELETE|SELECT|WRAP ) ) {
	sj3_unlockserv();
	sj3_close();
    }
#endif
}


static void	XimpEventMaskNotify( q, mask )
XimpQueue	*q;
long		mask;
{
    Display	*display = XtDisplay(TopLevel);

    if( ISBACKEND(q) ) {
	fprintf( stderr, "%s : bad XIM protocol received. [%d]\n",
		 program_name, XIMP_EVENTMASK_NOTIFY4 );
	send_error_notify( display, q->focus_window, q->protocol, (long)q->icid,
			   XIMP_EVENTMASK_NOTIFY4, XIMP_BadProtocol );
	return;
    }
    q->event_mask = mask;
    send_client_message( display, q->focus_window, (long)XIMP_EVENTMASK_NOTIFY_RETURN4,
			 (long)q->icid, 0L, 0L, 0L );
}


static void	XimpExtension( q, event )
XimpQueue		*q;
XClientMessageEvent	*event;
{
    if( (Atom)event->data.l[2] == ximp_ext_conversion ) {
	if( (Bool)event->data.l[3] == False ) {
	    /* Get Conversion mode */
	    send_client_message( event->display, q->focus_window,
				 (long)XIMP_EXTENSION(q), (long)q->icid,
				 (long)ximp_ext_conversion, 0L,
				 (long)q->connect_flag );
	}
	else {
	    /* Set Conversion mode */
	    if( (Bool)event->data.l[4] == False ) {
		/* Conversion Off */
	       if( q->connect_flag ) {
		    q->connect_flag = False;
		    q->buffer->connect--;
		    send_client_message( event->display, q->focus_window,
					 (long)XIMP_PROCESS_END(q),
					 q->icid, 0L, 0L, 0L );
		    if( q->style & (XIMPreeditNothing | XIMStatusNothing) )
			RootWindowUnmap( q->style );
		    if( q->style & (XIMPreeditPosition | XIMPreeditArea) )
			XmwPreeditUnmapWidget( q->buffer->preedit_widget );
		    if( ISFE1(q) ) {
			XSelectInput( event->display, q->focus_window,
				      ExposureMask | StructureNotifyMask );
		    }
		}
	    }
	    else {
		/* Conversion On */
		if( !q->connect_flag ) {
		    q->connect_flag = True;
		    q->buffer->connect++;
		    if( q->style & (XIMPreeditNothing | XIMStatusNothing) )
			RootWindowMap( q->buffer, q->style );
		    if( q->style & (XIMPreeditPosition | XIMPreeditArea) ) {
			buffer_initialize( q->buffer );
			XmwPreeditMapWidget( q->buffer->preedit_widget );
		    }
		    if( (q->style & XIMStatusArea)  &&
			q->buffer->status_widget ) {
		        set_label( q->buffer, q->buffer->conversion_mode );
			if( q->status.area.width >= q->status_area.width ) {
			    XtMapWidget( q->buffer->box[0] );
			    XtMapWidget( q->buffer->box[1] );
			}
		    }
		    send_client_message( event->display, q->focus_window,
					 (long)XIMP_PROCESS_BEGIN(q),
					 q->icid, 0L, 0L, 0L );
		    if( ISFE1(q) ) {
			XSelectInput( event->display, q->focus_window,
			    KeyPressMask | ExposureMask | StructureNotifyMask );
		    }
		}
		if( q->style & (XIMPreeditPosition | XIMPreeditArea) ) {
		    XtVaSetValues( q->buffer->preedit_widget, XmwNfocus, True,
				   NULL );
		    XtSetKeyboardFocus( TopLevel, q->buffer->preedit_widget );
		}
	    }
	}
    }
    else if( (Atom)event->data.l[2] == ximp_ext_back_front ) {
	if( event->data.l[3] != 0 )
	    q->ximp_type = XIMP_BE_TYPE1;
    }
    else {
	send_error_notify( event->display, q->focus_window, q->protocol,
			   (long)q->icid, XIMP_EXTENSION(q), XIMP_BadProtocol );
    }
}


Boolean
#if NeedFunctionPrototypes
XimpSendCompoundText( Display *display, Window w, wchar_t *string )
#else /* NeedFunctionPrototypes */
XimpSendCompoundText( display, w, string )
    Display	*display;
    Window	w;
    wchar_t	*string;
#endif /* NeedFunctionPrototypes */
{
    XimpQueue		*q;
    XTextProperty	 text;
    wchar_t		*list[1];
    int			 cm_max;

    if( (q = FocusWindow2XimpQueue( w )) == NULL )
	return( False );

    list[0] = string;
    XwcTextListToTextProperty( display, list, 1, XCompoundTextStyle, &text );
    cm_max = (q->protocol == 35) ? CT_MAX_IN_CM
				 : CT_MAX_IN_CM * 2;
    if( (resources.conversion_message  &&  text.nitems < 256)
	||  text.nitems < cm_max ) {
	send_multi_client_message( display, q->focus_window, q->icid,
				   text.nitems, text.value );
    }
    else {
	XChangeProperty( display, XtWindow(TopLevel), ximp_commit,
			ximp_compound_text, 8, PropModeAppend, text.value,
			text.nitems );
	send_client_message( display, q->focus_window, (long)XIMP_READPROP(q),
			     (long)q->icid, (long)ximp_commit, 0L, 0L );
    }
    XFree( text.value );

    if( q->style & XIMPreeditNothing )
	XmwTextOutSetString( q->buffer->preedit_widget, NULL,
			     XmwHIGHLIGHT_NORMAL );
    else if( q->style & XIMPreeditPosition ) {
	if( resources.auto_replace )
	    XmwPreeditAutoReplace( q->buffer->preedit_widget );
	else
	    XmwPreeditSetString( q->buffer->preedit_widget, NULL,
				 XmwHIGHLIGHT_SECONDARY_SELECTED );
    }
    else if( q->style & XIMPreeditArea )
	XmwPreeditSetString( q->buffer->preedit_widget, NULL,
			     XmwHIGHLIGHT_SECONDARY_SELECTED );

    return( True );
}


Boolean
#if NeedFunctionPrototypes
XimpSendKeyEvent( Window w, XKeyEvent *event )
#else
XimpSendKeyEvent( w, event )
    Window	 w;
    XKeyEvent	*event;
#endif
{
    XimpQueue		*q;

    if( (q = FocusWindow2XimpQueue( w )) == NULL )
	return( False );
    send_client_message( event->display, q->focus_window,
			 (long)XIMP_KEYPRESS(q), (long)q->icid,
			 (long)event->keycode, (long)event->state,
			 (long)event->time );

    if( ISXimp4(q)  &&  (q->event_mask & KeyReleaseMask) )
	send_client_message( event->display, q->focus_window,
			     (long)XIMP_KEYRELEASE4, (long)q->icid,
			     (long)event->keycode, (long)event->state,
			     (long)event->time );
    return( True );
}


static void
ximp_receive( event )
    XClientMessageEvent	*event;
{
    XimpQueue	*q;

#define	ISPROTOCOL3	(event->data.l[0]<100)

    if( event->data.l[0] == XIMP_CREATE3  ||
       event->data.l[0] == XIMP_CREATE4 ) {
#ifdef	DEBUG
	if( resources.debug & 1 )
	    puts( ISPROTOCOL3 ? "XIMP_CREATE3" : "XIMP_CREATE4" );
#endif
	XimpCreate( event );
	return;
    }

    if( (q = ICID2XimpQueue( (ICID)event->data.l[1] )) == NULL ) {
	fprintf( stderr, "%s : bad protocol received.[%d]\n",
		 program_name, event->data.l[0] );
	return;
    }
    if( (ISXimp3(q)  &&  event->data.l[0] >= 100)  ||
       (ISXimp4(q)  &&  event->data.l[0] < 100) ) {
	fprintf( stderr, "%s : bad protocol received.[%d]\n",
		 program_name, event->data.l[0] );
	send_error_notify( event->display, q->focus_window, q->protocol,
			(long)q->icid, event->data.l[0], XIMP_BadProtocol );
	return;
    }

    switch( event->data.l[0] ) {
      case XIMP_KEYPRESS3:
      case XIMP_KEYPRESS4:
#ifdef	DEBUG
	if( resources.debug & 1 )
	    puts( ISPROTOCOL3 ? "XIMP_KEYPRESS3" : "XIMP_KEYPRESS4" );
#endif
	XimpKeyPress( q, event );
	break;

      case XIMP_KEYRELEASE4:
#ifdef	DEBUG
	if( resources.debug & 1 )
	    puts( "XIMP_KEYRELEASE4" );
#endif
	XimpKeyRelease( q, event );
	break;

      case XIMP_DESTROY3:
      case XIMP_DESTROY4:
#ifdef	DEBUG
	if( resources.debug & 1 )
	    puts( ISPROTOCOL3 ? "XIMP_DESTROY3" : "XIMP_DESTROY4" );
#endif
	XimpDestroy( q );
	break;

      case XIMP_BEGIN3:
      case XIMP_REG_KEY_PRESSED4:
#ifdef	DEBUG
	if( resources.debug & 1 )
	    puts( ISPROTOCOL3 ? "XIMP_BEGIN3" : "XIMP_REG_KEY_PRESSD4" );
#endif
	XimpBegin( q );
	break;

      case XIMP_END3:
#ifdef	DEBUG
	if( resources.debug & 1 )
	    puts( "XIMP_END3" );
#endif
	XimpEnd( q );
	break;

      case XIMP_SETFOCUS3:
      case XIMP_SETFOCUS4:
#ifdef	DEBUG
	if( resources.debug & 1 )
	    printf( ISPROTOCOL3 ? "XIMP_SETFOCUS3 [%d]\n"
				: "XIMP_SETFOCUS4 [%d]\n", q->icid );
#endif
	XimpSetFocus( q );
	break;

      case XIMP_UNSETFOCUS3:
      case XIMP_UNSETFOCUS4:
#ifdef	DEBUG
	if( resources.debug & 1 )
	    printf( ISPROTOCOL3 ? "XIMP_UNSETFOCUS3 [%d]\n"
				: "XIMP_UNSETFOCUS4 [%d]\n", q->icid );
#endif
	XimpUnsetFocus( q );
	break;

      case XIMP_SETVALUE3:
      case XIMP_SETVALUE4:
#ifdef	DEBUG
	if( resources.debug & 1 )
	    puts( ISPROTOCOL3 ? "XIMP_SETVALUE3" : "XIMP_SETVALUE4" );
#endif
	XimpSetValue( q, event->data.l[2] );
	break;

      case XIMP_CLIENT_WINDOW4:
#ifdef	DEBUG
	if( resources.debug & 1 )
	    puts( "XIMP_CLIENT_WINDOW4" );
#endif
	XimpClientWindow( q, (Window)event->data.l[2], (long)event->data.l[3] );
	break;

      case XIMP_FOCUS_WINDOW4:
#ifdef	DEBUG
	if( resources.debug & 1 )
	    puts( "XIMP_FOCUS_WINDOW4" );
#endif
	XimpFocusWindow( q, (Window)event->data.l[2], (long)event->data.l[3] );
	break;

      case XIMP_MOVE3:
      case XIMP_MOVE4:
#ifdef	DEBUG
	if( resources.debug & 1 )
	    puts( ISPROTOCOL3 ? "XIMP_MOVE3" : "XIMP_MOVE4" );
#endif
	XimpMove( q, event->data.l[2], event->data.l[3] );
	break;

      case XIMP_GETVALUE3:
      case XIMP_GETVALUE4:
#ifdef	DEBUG
	if( resources.debug & 1 )
	    puts( ISPROTOCOL3 ? "XIMP_GETVALUE3" : "XIMP_GETVALUE4" );
#endif
	XimpGetValue( q, event->data.l[2] );
	break;

      case XIMP_RESET3:
      case XIMP_RESET4:
#ifdef	DEBUG
	if( resources.debug & 1 )
	    puts( ISPROTOCOL3 ? "XIMP_RESET3" : "XIMP_RESET4" );
#endif
	XimpReset( q, event );
	break;

      case XIMP_EVENTMASK_NOTIFY4:
#ifdef	DEBUG
	if( resources.debug & 1 )
	    puts( "XIMP_EVENTMASK_NOTIFY4" );
#endif
	XimpEventMaskNotify( q, event->data.l[2] );
	break;

      case XIMP_EXTENSION3:
      case XIMP_EXTENSION4:
#ifdef	DEBUG
	if( resources.debug & 1 )
	    puts( ISPROTOCOL3 ? "XIMP_EXTENSION3" : "XIMP_EXTENSION4" );
#endif
	XimpExtension( q, event );
	break;

      default:
#ifdef	DEBUG
	fprintf( stderr, "%s : unknown XIM protocol received. [%d]\n",
		 program_name, event->data.l[0] );
#endif
	send_error_notify( event->display, q->focus_window, q->protocol,
			   (long)q->icid, event->data.l[0], XIMP_BadProtocol );
	break;
    }
}


void
#if NeedFunctionPrototypes
ximp_end( Display *display, Window w )
#else
ximp_end( display, w )
    Display	*display;
    Window	w;
#endif
{
    XimpQueue	*q;

    if( (q = FocusWindow2XimpQueue( w )) == NULL )
	return;

    if( q->connect_flag ) {
	if( q->buffer->error_shell ) {
	    error_cancel( (Widget)NULL, (caddr_t)q->buffer, (caddr_t)NULL );
	}
	if( q->buffer->state & DELETE )
	    sakujo_cancel( (Widget)NULL, (caddr_t)q->buffer, (caddr_t)NULL );
	if( q->buffer->state & CODE )
	    code_cancel( (Widget)NULL, (caddr_t)q->buffer, (caddr_t)NULL );
	if( q->buffer->state & ADD )
	    touroku_cancel( (Widget)NULL, (caddr_t)q->buffer, (caddr_t)NULL );
	if( q->buffer->state & SERVER )
	    reconnect_cancel( (Widget)NULL, (caddr_t)q->buffer, (caddr_t)NULL );
	if( q->buffer->state & SELECT ) {
	    if( q->style == (XIMPreeditNothing | XIMStatusNothing) ) {
		XtUnmapWidget( RootTextField );
		XtMapWidget( q->buffer->label );
	    }
	    else if( q->style & (XIMPreeditPosition | XIMPreeditArea) )
		select_dialog_cancel( (Widget)NULL, (caddr_t)q->buffer,
				      (caddr_t)NULL );
	}
	if( q->buffer->state & EDIT )
	    edit_cancel( q->buffer );
	if( q->buffer->state & (WRAP | CONVERSION) )
	    henkan_cancel( q->buffer );
	q->connect_flag = False;
	q->buffer->connect--;
	if( ISFE1(q) )
	    XSelectInput( display, q->focus_window,
			ExposureMask | StructureNotifyMask );
	if( !ISTYPE2(q) )
	    send_client_message( display, q->focus_window,
				 (long)XIMP_PROCESS_END(q), q->icid,
				 0L, 0L, 0L );
	if( q->style & (XIMPreeditNothing | XIMStatusNothing) )
	    RootWindowUnmap( q->style );
	if( q->style & (XIMPreeditPosition | XIMPreeditArea) )
	    XmwPreeditUnmapWidget( q->buffer->preedit_widget );
	if( (q->style & XIMStatusArea)  &&  q->buffer->status_widget ) {
	    if( !(q->style & XIMPreeditArea) ) {
		XtUnmapWidget( q->buffer->box[0] );
		XtUnmapWidget( q->buffer->box[1] );
	    }
	    XtVaSetValues( q->buffer->label,
			   XtNlabel, resources.sjxa, NULL );
	}
    }
}


void	ximp_all_end()
{
    XimpQueue	*q;

    for( q = ximp_queue; q != NULL; q = q->next )
	if( q->connect_flag )
	    ximp_end( XtDisplay(TopLevel), q->client_window );
}


Boolean
#if NeedFunctionPrototypes
XimpDestroyNotify( XEvent *event )
#else /* NeedFunctionPrototypes */
XimpDestroyNotify( event )
    XEvent	*event;
#endif /* NeedFunctionPrototypes */
{
    XimpQueue   *q, *tmp;
    Boolean     flag = False;

    for( q = ximp_queue; q != NULL; q = tmp ) {
	tmp = q->next;
	if( q->client_window == event->xdestroywindow.window ) {
	    if( q->connect_flag ) {
		if( q->style & (XIMPreeditNothing | XIMStatusNothing) )
		    RootWindowUnmap( q->style );
	    }
	    flag = True;
	    delete_ximp_queue( q );
	}
    }
    return( flag );
}


Boolean
#if NeedFunctionPrototypes
XimpChangeExpose( XEvent *event )
#else /* NeedFunctionPrototypes */
XimpChangeExpose( event )
    XEvent	*event;
#endif /* NeedFunctionPrototypes */
{
    XimpQueue		*q;

    for( q = ximp_queue; q != NULL; q = q->next ) {
	if( (q->client_window == event->xexpose.window  ||
	   q->focus_window == event->xexpose.window)  &&
	   (q->style & (XIMPreeditPosition | XIMPreeditArea)) ) {
	    event->xexpose.window = XtWindow( q->buffer->preedit_widget );
	    return( True );
	}
    }
    return( False );
}


Boolean
#if NeedFunctionPrototypes
XimpPreeditChangeKeyDirection( XEvent *event )
#else /* NeedFunctionPrototypes */
XimpPreeditChangeKeyDirection( event )
    XEvent	*event;
#endif /* NeedFunctionPrototypes */
{
    XimpQueue	*q;

    if( (q = FocusWindow2XimpQueue( event->xkey.window )) != NULL ) {
	event->xkey.window = XtWindow( q->buffer->preedit_widget );
	XtSetKeyboardFocus( TopLevel, q->buffer->preedit_widget );
	return( True );
    }
    if( (q = PreeditWindow2XimpQueue( event->xkey.window )) != NULL ) {
	return( True );
    }
    if( (q = StatusWindow2XimpQueue( event->xkey.window )) != NULL ) {
	return( True );
    }
    return( False );
}


Boolean
#if NeedFunctionPrototypes
XimpFilterEvent( XKeyEvent *event )
#else /* NeedFunctionPrototypes */
XimpFilterEvent( event )
    XKeyEvent	*event;
#endif /* NeedFunctionPrototypes */
{
    XimpQueue	*q, *p = NULL;
    Boolean	ret = True;

    for( q = ximp_queue; q; q = q->next ) {
	if( q->focus_window == event->window  &&
	    (q->mask & XIMP_FOCUS_WIN_MASK(q)) ) {
	    if( q->connect_flag ) {
		if( ISSYNC(q) )
		    send_client_message( event->display, q->focus_window,
					 (long)XIMP_KEYPRESS_RETURN4,
					 (long)q->icid, 0, 0, 0 );
		if( !q->has_focus ) {
		    p = FocusWindow2XimpQueue( event->window );
		    if( p->has_focus  &&  !p->connect_flag ) {
			send_client_message( event->display, q->focus_window,
					     (long)XIMP_KEYPRESS(p),
					     (long)q->icid,
					     (long)event->keycode,
					     (long)event->state,
					     (long)event->time );
			return( True );
		    }
		}
		return( False );
	    }
	    p = q;
	}
    }
    if( p == NULL )
	return( False );
    switch( p->ximp_type ) {
      case XIMP_FE_TYPE1:
	if( is_started_key( event ) )
	    XimpBegin( p );
	break;

      case XIMP_FE_TYPE2:
	if( is_started_key( event ) )
	    XimpBegin( p );
	else
	    send_client_message( event->display, p->focus_window,
				 (long)XIMP_KEYPRESS(p), (long)p->icid,
				 (long)event->keycode, (long)event->state,
				 (long)event->time );
	break;

      case XIMP_FE_TYPE3:
	if( is_started_key( event ) )
	    XimpBegin( p );
	break;

      case XIMP_BE_TYPE1:
      case XIMP_SYNC_BE_TYPE1:
	if( is_started_key( event ) )
	    XimpBegin( p );
	else
	    ret = False;
	break;

      case XIMP_BE_TYPE2:
      case XIMP_SYNC_BE_TYPE2:
	if( is_started_key( event ) )
	    XimpBegin( p );
	else
	    send_client_message( event->display, p->focus_window,
				 (long)XIMP_KEYPRESS(p), (long)p->icid,
				 (long)event->keycode, (long)event->state,
				 (long)event->time );
	break;
    }
    if( ISSYNC(p) )
	send_client_message( event->display, p->focus_window,
			     (long)XIMP_KEYPRESS_RETURN4, (long)p->icid,
			     0, 0, 0 );
    return( ret );
}


Boolean
#if NeedFunctionPrototypes
XimpIsRegisteredWindow( Window w1, Window w2 )
#else /* NeedFunctionPrototypes */
XimpIsRegisteredWindow( w1, w2 )
    Window	w1, w2;
#endif /* NeedFunctionPrototypes */
{
    XimpQueue	*q;

    for( q = ximp_queue; q; q = q->next )
	if( q->client_window == w1  ||  q->focus_window == w1  ||
	    q->client_window == w2  ||  q->focus_window == w2 )
	    return( True );
    return( False );
}


static void
procNoEventMask( w, client_data, event, continue_to_dispatch )
    Widget	w;
    XtPointer	client_data;
    XEvent	*event;
    Boolean	*continue_to_dispatch;
{
    int		ii;

    switch( event->type ) {
      case SelectionClear:
	if( event->xselectionclear.selection == ximp_basic_declaration[0]  ||
	    event->xselectionclear.selection == ximp_basic_declaration[1]  ) {
	    fprintf( stderr, "%s : selection clear event occured.\n\r",
		    program_name );
	    done();
	}
	for( ii = 0; ii < 10; ii++ ) {
	    if( event->xselectionclear.selection ==
		ximp_detailed_declaration[ii] ) {
		fprintf( stderr, "%s : selection clear event occured.\n\r",
			program_name );
		done();
	    }
	}
	break;

      case ClientMessage:
	if( event->xclient.message_type == ximp_protocol )
	    ximp_receive( event );
	break;
    }
}


static void
procPropertyChangeMask( w, client_data, event, continue_to_dispatch )
    Widget	w;
    XtPointer	client_data;
    XEvent	*event;
    Boolean	*continue_to_dispatch;
{
    XPropertyEvent	*ev = (XPropertyEvent *)event;
    Display		*display = XtDisplay(w);
    Window		window = XtWindow(w);
    char		ss[256];
    int			count, ii;

    sprintf( ss, "%s%s_%s", _XIMP_BASE, LANGUAGE, TERRITORY );
    ximp_basic_declaration[0] = XInternAtom( display, ss, False );
    XSetSelectionOwner( display, ximp_basic_declaration[0], window, ev->time );
    if( resources.xim_protocol == 3 )
	ximp_basic_declaration[1] = 0;
    else {
	sprintf( ss, "%s%s", _XIMP_BASE, LANGUAGE );
	ximp_basic_declaration[1] = XInternAtom( display, ss, False );
	XSetSelectionOwner( display, ximp_basic_declaration[1], window,
			    ev->time );
    }
    count = ScreenCount(display);
    for( ii = 0; ii < count; ii++ ) {
	sprintf( ss, "%s%s_%s@%s.%d", _XIMP_BASE, LANGUAGE, TERRITORY,
				      program_name, ii );
	ximp_detailed_declaration[ii] = XInternAtom( display, ss, False );
	XSetSelectionOwner( display, ximp_detailed_declaration[ii], window,
			    ev->time );
    }
    for( ;ii < 10; ii++ )
	ximp_detailed_declaration[ii] = 0;

    XtRemoveEventHandler( (Widget)client_data, PropertyChangeMask, False,
			  procPropertyChangeMask, client_data );
}


void
#if NeedFunctionPrototypes
ximp_initialize( Widget TopLevel )
#else /* NeedFunctionPrototypes */
ximp_initialize( TopLevel )
    Widget	TopLevel;
#endif /* NeedFunctionPrototypes */
{
    Display	*display = XtDisplay(TopLevel);
    Window	 window = XtWindow(TopLevel);
    Atom	 atom[2];
    char	 ss[80];
    XimpKey	*keys;

    XtAddEventHandler( TopLevel, NoEventMask, True, procNoEventMask, NULL );
    XtAddEventHandler( TopLevel, PropertyChangeMask, False,
		       procPropertyChangeMask, TopLevel );

    ximp_version	= XInternAtom( display, _XIMP_VERSION, False );
    ximp_style		= XInternAtom( display, _XIMP_STYLE, False );
    ximp_server_name	= XInternAtom( display, _XIMP_SERVERNAME, False );
    ximp_server_version	= XInternAtom( display, _XIMP_SERVERVERSION, False );
    ximp_extensions	= XInternAtom( display, _XIMP_EXTENSIONS, False );
    ximp_vendor_name	= XInternAtom( display, _XIMP_VENDORNAME, False );
    ximp_ext_conversion = XInternAtom( display, _XIMP_EXT_XIMP_CONVERSION,
				       False );
    ximp_ext_back_front = XInternAtom( display, _XIMP_EXT_XIMP_BACK_FRONT,
				       False );
    ximp_protocol	= XInternAtom( display, _XIMP_PROTOCOL, False );
    ximp_focus		= XInternAtom( display, _XIMP_FOCUS, False );
    ximp_preedit	= XInternAtom( display, _XIMP_PREEDIT, False );
    ximp_status		= XInternAtom( display, _XIMP_STATUS, False );
    ximp_preedit_font	= XInternAtom( display, _XIMP_PREEDITFONT, False );
    ximp_status_font	= XInternAtom( display, _XIMP_STATUSFONT, False );
    ximp_commit		= XInternAtom( display, _XIMP_COMMIT, False );
    ximp_compound_text	= XInternAtom( display, _XIMP_CTEXT, False );
    ximp_reset		= XInternAtom( display, _XIMP_RESET, False );

    ximp_type = XInternAtom( display, _XIMP_TYPE, False );
    ximp_keys = XInternAtom( display, _XIMP_KEYS, False );
    ximp_started_keys = XInternAtom( display, _XIMP_SPROC_STARTED_KEYS, False );
    ximp_stopped_keys = XInternAtom( display, _XIMP_SPROC_STOPPED_KEYS, False );

    keys = resources.process_start_keys;
    if( resources.xim_protocol == 3 ) {
	set_atom_string( display, window, ximp_version,
			 XIMP_PROTOCOL_VERSION3 );
    }
    else {
	set_atom_string( display, window, ximp_version,
			 XIMP_PROTOCOL_VERSION4 );
	XChangeProperty( display, window, ximp_type, ximp_type, 32,
			 PropModeReplace, (unsigned char *)supported_type,
			 sizeof(supported_type) / sizeof(unsigned long) );
	set_atom_keys( display, window, ximp_started_keys, keys );
	set_atom_keys( display, window, ximp_stopped_keys,
		       Action2XimpKey( sjxa_disconnect ) );
    }

    XChangeProperty( display, window, ximp_style, ximp_style, 32,
		     PropModeReplace, (unsigned char *)supported_styles,
		     sizeof(supported_styles) / sizeof(unsigned long) );
    set_atom_keys( display, window, ximp_keys, keys );
    set_atom_string( display, window, ximp_server_name, program_name );
    sprintf( ss, "%d.%d.%d", MAJOR_VERSION, MINOR_VERSION, BUGFIX_VERSION );
    set_atom_string( display, window, ximp_server_version, ss );
    set_atom_string( display, window, ximp_vendor_name, VENDOR_NAME );

    atom[0] = ximp_ext_conversion;
    atom[1] = ximp_ext_back_front;
    XChangeProperty( display, window, ximp_extensions, ximp_extensions, 32,
		     PropModeReplace, (unsigned char *)atom, 2 );
}
#endif
