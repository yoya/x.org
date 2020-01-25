/* $SonyId: SjInit.c,v 1.1 1994/05/30 13:03:58 makoto Exp $ */
/******************************************************************

Copyright (c) 1993, 1994  Sony Corporation

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
#include	<X11/keysym.h>
#include	<X11/Xlib.h>
#include	<X11/Intrinsic.h>
#include	<X11/StringDefs.h>
#include	<X11/Shell.h>
#include	<X11/Xaw/Form.h>
#include	<X11/Xaw/Box.h>
#include	<Xmw/Command.h>
#include	<Xmw/TextOut.h>
#include	<Xmw/List.h>
#include	"sjxa.h"
#include	"resource.h"
#include	"SjBuffer.h"
#include	"SjString.h"
#include	"SjXkey.h"

#define min( a, b )	(((a) < (b)) ? (a) : (b))
#define	LIGHT(x,y)	(((ii = (int)x * y / 1000) > 65535) ? 65535 : ii)
#define	C_LIGHT(x)	((((ii = (int)x * 1194 / 1000) > 65535) ? 65535 : ii) * 1000 / x)

Widget	TopLevel, RootText, RootLabel, Box[2], RootTextField;

static XrmOptionDescRec	option[] = {
    { "-server",		"*server",	XrmoptionSepArg,  NULL   },
    { "-no_auto_replace",	"*autoReplace",	XrmoptionNoArg,  "False" },
    { "-nar",			"*autoReplace",	XrmoptionNoArg,  "False" },
    { "-ignore_preedit_color",	"*ignorePreeditColor",
						XrmoptionNoArg,  "True"  },
    { "-ipc",		"*ignorePreeditColor",	XrmoptionNoArg,  "True"  },
    { "-ignore_status_color",	"*ignoreStatusColor",
						XrmoptionNoArg,  "True"  },
    { "-isc",		"*ignoreStatusColor",	XrmoptionNoArg,  "True"  },
    { "-auto_raise",		"*autoRaise",	XrmoptionNoArg,  "True"  },
    { "+auto_raise",		"*autoRaise",	XrmoptionNoArg,  "False" },
    { "-kana",			"*kanaMode",	XrmoptionNoArg,  "True"  },
    { "+kana",			"*kanaMode",	XrmoptionNoArg,  "False" },
#if defined(USE_XIMCP)
    { "-ximcp",			"*ximcProtocol",XrmoptionNoArg,  "True"  },
    { "+ximcp",			"*ximcProtocol",XrmoptionNoArg,  "False" },
#endif
#ifdef USE_XIMP
    { "-ximp",			"*ximProtocol", XrmoptionSepArg,  NULL   },
#endif
#ifdef USE_XLC
    { "-xlc",			"*xlcProtocol",	XrmoptionNoArg,  "True"  },
    { "+xlc",			"*xlcProtocol",	XrmoptionNoArg,  "False" },
#endif
#ifdef USE_SJX
    { "-sjx",			"*sjxProtocol", XrmoptionNoArg,  "True"  },
    { "+sjx",			"*sjxProtocol", XrmoptionNoArg,  "False" },
#endif
    { "-debug",			"*debug",	XrmoptionSepArg,  NULL   }
};

static Colormap		colormap = None;

extern char	*program_name;
extern Sjxa	resources;
extern int	*depths;

extern void	done();
extern void	function_activate();
extern void	sjxa_connect();


static int
error_handler( d, event )
    Display	*d;
    XErrorEvent	*event;
{
    char	error[80];

    if( resources.debug > 1 ) {
	XGetErrorText( d, event->error_code, error, 80 );
	fprintf( stderr, "%s: X Error %s\n", program_name, error );
    }
    return( 0 );
}


Pixel
#if NeedFunctionPrototypes
light_color( Display *display, Colormap cm, Pixel pixel )
#else
light_color( display, cm, pixel )
    Display	*display;
    Colormap	cm;
    Pixel	pixel;
#endif
{
    XColor	xcolor;
    int		ii;
    int		bye = 1194;

    if( cm == None )
	cm = colormap;
    xcolor.pixel = pixel;
    XQueryColor( display, cm, &xcolor );

    if( xcolor.red != 0 )	bye = min( bye, C_LIGHT(xcolor.red) );
    if( xcolor.green != 0 )	bye = min( bye, C_LIGHT(xcolor.green) );
    if( xcolor.blue != 0 )	bye = min( bye, C_LIGHT(xcolor.blue) );
    if( bye == 1000 )	bye = 1100;

    xcolor.red = LIGHT(xcolor.red,bye);
    xcolor.green = LIGHT(xcolor.green,bye);
    xcolor.blue = LIGHT(xcolor.blue,bye);
    if( XAllocColor( display, cm, &xcolor ) )
	return( xcolor.pixel );
    else
	return( pixel );
}


XtAppContext
root_window_init( b, argc, argv )
    Buffer	*b;
    int		argc;
    char	*argv[];
{
    Display		*display;
    XtAppContext 	 app;
    Widget		 Form;
    Arg			 args[20];
    int			 m, n;
    long		 bg, lbg;
    Position		 xl, yl, x1, y1, x2, y2, xt, yt;
    Dimension		 width, border_width, w1, w2;
    Dimension		 height, hl, h1, h2, ht;
    XtWidgetGeometry	 request;
    XimpKey		*keys;

    XtSetLanguageProc( NULL, NULL, NULL );
    TopLevel = XtAppInitialize( &app, CLASS_NAME, option, XtNumber(option),
				&argc, argv, NULL, NULL, 0 );
    display = XtDisplay( TopLevel );

    if( argc > 1 )
	usage();
    AddConverter( app );
    GetResources( TopLevel );
    AddActions( app );
    XtVaGetValues( TopLevel, XtNcolormap, &colormap, XtNbackground, &bg, NULL );
    lbg = light_color( display, None, bg );

    Form = XtCreateWidget( "Root", formWidgetClass, TopLevel, NULL, 0 );
    XtManageChild( Form );

    b->previous = NULL;
    b->next = NULL;
    b->connect = 0;
    b->conversion_style = XIMPreeditNothing | XIMStatusNothing;
    b->conversion_mode =
    b->conversion_mode_toggle = resources.initial_mode;
    b->jisx0208 = 0x0101;
    b->jisx0212 = 0x020f;
    b->use_jisx0212 = False;
    buffer_initialize( b );

   /*
    *	Create Text Widget on IM Server
    */
    n = 0;
    XtSetArg( args[n], XtNbackground, lbg );			n++;
    XtSetArg( args[n], XtNhorizDistance, 5 );			n++;
    XtSetArg( args[n], XtNvertDistance, 5 );			n++;
    XtSetArg( args[n], XtNleft, XawChainLeft );			n++;
    XtSetArg( args[n], XtNright, XawChainRight );		n++;
    XtSetArg( args[n], XtNtop, XawChainTop );			n++;
    XtSetArg( args[n], XtNbottom, XawChainBottom );		n++;
    XtSetArg( args[n], XtNresizable, True );			n++;
    XtSetArg( args[n], XmwNdisplayCaret, False );		n++;
    RootText = XmwCreateTextOut( Form, "Text", args, n );
    XtManageChild( RootText );
    b->preedit_widget = RootText;

   /*
    *	Label
    */
    n = 0;
    XtSetArg( args[n], XtNlabel, resources.sjxa );		n++;
    XtSetArg( args[n], XtNborderWidth, 0 );			n++;
    XtSetArg( args[n], XtNfromVert, RootText );			n++;
    XtSetArg( args[n], XtNleft, XawChainLeft );			n++;
    XtSetArg( args[n], XtNright, XawChainLeft );		n++;
    XtSetArg( args[n], XtNbottom, XawChainBottom );		n++;
    XtSetArg( args[n], XtNtop, XawChainBottom );		n++;
    XtSetArg( args[n], XtNhorizDistance, 5 );			n++;
    RootLabel = XtCreateWidget( "Label", labelWidgetClass, Form, args, n );
    XtManageChild( RootLabel );
    b->label = RootLabel;

   /*
    *	List
    */
    n = 0;
    XtSetArg( args[n], XtNfromHoriz, RootLabel );		n++;
    XtSetArg( args[n], XtNfromVert, RootText );			n++;
    XtSetArg( args[n], XtNleft, XawChainLeft );			n++;
    XtSetArg( args[n], XtNright, XawChainLeft );		n++;
    XtSetArg( args[n], XtNbottom, XawChainBottom );		n++;
    XtSetArg( args[n], XtNtop, XawChainBottom );		n++;
    XtSetArg( args[n], XtNborderWidth, 0 );			n++;
    XtSetArg( args[n], XtNmappedWhenManaged, False );		n++;
    XtSetArg( args[n], XmwNitemBackground, lbg );		n++;
    XtSetArg( args[n], XmwNitemCount, 5 );			n++;
    XtSetArg( args[n], XmwNitemLength, resources.function_par_length * 5 );
								n++;
    XtSetArg( args[n], XmwNitems, resources.function_items );	n++;
    XtSetArg( args[n], XmwNnumRows, 5 );			n++;
    XtSetArg( args[n], XmwNvisibleHorizontalItemCount, 5 );	n++;
    XtSetArg( args[n], XmwNvisibleVerticalItemCount, 1 );	n++;
    Box[0] = XmwCreateList( Form, "LeftBox", args, n );
    n = 0;
    XtSetArg( args[n], XtNfromHoriz, Box[0] );			n++;
    XtSetArg( args[n], XtNfromVert, RootText );			n++;
    XtSetArg( args[n], XtNleft, XawChainLeft );			n++;
    XtSetArg( args[n], XtNright, XawChainLeft );		n++;
    XtSetArg( args[n], XtNbottom, XawChainBottom );		n++;
    XtSetArg( args[n], XtNtop, XawChainBottom );		n++;
    XtSetArg( args[n], XtNborderWidth, 0 );			n++;
    XtSetArg( args[n], XtNmappedWhenManaged, False );		n++;
    XtSetArg( args[n], XmwNitemBackground, lbg );		n++;
    XtSetArg( args[n], XmwNitemCount, 5 );			n++;
    XtSetArg( args[n], XmwNitemLength, resources.function_par_length * 5 );
								n++;
    XtSetArg( args[n], XmwNitems,
	      &resources.function_items[resources.function_par_length * 5] );
								n++;
    XtSetArg( args[n], XmwNnumRows, 5 );			n++;
    XtSetArg( args[n], XmwNvisibleHorizontalItemCount, 5 );	n++;
    XtSetArg( args[n], XmwNvisibleVerticalItemCount, 1 );	n++;
    Box[1] = XmwCreateList( Form, "RightBox", args, n );
    XtManageChildren( Box, 2 );

    b->box[0] = Box[0];
    b->box[1] = Box[1];
    XtAddCallback( Box[0], XmwNsetCallback, (XtCallbackProc)function_activate,
		   (caddr_t)b );
    XtAddCallback( Box[1], XmwNsetCallback, (XtCallbackProc)function_activate,
		   (caddr_t)b );


   /*
    *	Work text widget
    */
    n = 0;
    XtSetArg( args[n], XtNmappedWhenManaged, False );		n++;
    XtSetArg( args[n], XtNbackground, lbg );			n++;
    XtSetArg( args[n], XtNfromHoriz, RootLabel );		n++;
    XtSetArg( args[n], XtNfromVert, RootText );			n++;
    XtSetArg( args[n], XtNleft, XawChainLeft );			n++;
    XtSetArg( args[n], XtNright, XawChainRight );		n++;
    XtSetArg( args[n], XtNbottom, XawChainBottom );		n++;
    XtSetArg( args[n], XtNtop, XawChainBottom );		n++;
    XtSetArg( args[n], XtNhorizDistance, 5 );			n++;
    XtSetArg( args[n], XtNresizable, True );			n++;
    XtSetArg( args[n], XmwNdisplayCaret, False );		n++;
    RootTextField = XmwCreateTextOut( Form, "TextField", args, n );
    XtManageChild( RootTextField );

   /*
    *	Create Windows
    */
    XtUninstallTranslations( RootText );
    XtVaSetValues( RootText, XtNaccelerators, resources.base_accelerators,
		   NULL );
    XtInstallAccelerators( RootText, RootText );
    if( resources.accelerators ) {
	XtOverrideTranslations( RootText, resources.accelerators );
	XtInstallAccelerators( RootText, RootText );
    }

    XtRealizeWidget( TopLevel );

   /*
    *	set icon
    */
    MakeIconPixmap( TopLevel, RootLabel );

   /*
    *	Widget values reset
    */
    XtVaGetValues( Form, XtNwidth, &width, NULL );
    XtVaGetValues( RootText, XtNborderWidth, &border_width, NULL );
    request.request_mode = CWWidth;
    request.width = width - 11 - border_width * 2;
    XtMakeGeometryRequest( RootText, &request, NULL );

    XtVaGetValues( Box[0], XtNx, &x1, XtNy, &y1, XtNwidth, &w1, XtNheight, &h1,
		   NULL );
    XtVaGetValues( Box[1], XtNx, &x2, XtNy, &y2, XtNwidth, &w2, XtNheight, &h2,
		   NULL );
    XtVaGetValues( RootTextField, XtNx, &xt, XtNy, &yt, XtNheight, &ht,
		   XtNborderWidth, &border_width, NULL );
    request.width = w1 + w2 - 3 - border_width * 2;
    XtMakeGeometryRequest( RootTextField, &request, NULL );

    XtVaGetValues( RootLabel, XtNx, &xl, XtNy, &yl, XtNheight, &hl, NULL );
    height = hl;
    if( height < h1 )	height = h1;
    if( height < h2 )	height = h2;
    if( height < ht )	height = ht;
    XtMoveWidget( RootLabel, xl, yl + (height - hl) / 2 );
    XtMoveWidget( Box[0], x1, y1 + (height - h1) / 2 );
    XtMoveWidget( Box[1], x2, y2 + (height - h2) / 2 );
    XtMoveWidget( RootTextField, xt, yt + (height - ht) / 2 );

    XtVaGetValues( TopLevel, XtNwidth, &width, XtNheight, &height, NULL );
    XtVaSetValues( TopLevel, XtNminWidth, width, XtNminHeight, height,
		   XtNmaxHeight, height, NULL );

   /*
    *	Error Handler
    */
    XSetErrorHandler( error_handler );

   /*
    *	reset the list on keys
    */
    if( (keys = Action2XimpKey( sjxa_connect )) != NULL ) {
	for( m = 0;
	     resources.process_start_keys[m].keysym != XK_VoidSymbol; m++ ) {
	    for( n = 0; keys[n].keysym != XK_VoidSymbol; n++ ) {
		if( keys[n].keysym
			== resources.process_start_keys[m].keysym  &&
		    keys[n].modifier
			== resources.process_start_keys[m].modifier  &&
		    keys[n].modifier_mask
			== resources.process_start_keys[m].modifier_mask )
			goto next;
	    }
	    keys = (XimpKey *)realloc( keys, sizeof(XimpKey) * (n+2) );
	    if( keys == NULL ) {
		XtError( "memory allocation" );
		exit( 1 );
	    }
	    keys[n].keysym = resources.process_start_keys[m].keysym;
	    keys[n].modifier = resources.process_start_keys[m].modifier;
	    keys[n].modifier_mask =
		resources.process_start_keys[m].modifier_mask;
	    keys[n+1].keysym = XK_VoidSymbol;
next:;
	}
	free( (char *)resources.process_start_keys );
	resources.process_start_keys = keys;
    }

    XtAddGrab( Form, False, False );
    return( app );
}


void	depth_initialize( display )
Display	*display;
{
    int		n;
    int		ii, jj;
    int		ndepths, *depth;
    Visual	*visual;

    n = ScreenCount( display );
    depths = (int *)calloc( sizeof(int), n );
    for( ii = 0; ii < n; ii++ ) {
	visual = XDefaultVisual( display, ii );
	depths[ii] = DefaultDepth( display, ii );
	if( (visual->class == StaticGray  ||  visual->class == GrayScale)  &&
	    depths[ii] != 1 ) {
	    depth = XListDepths( display, ii, &ndepths );
	    for( jj = 0; jj < ndepths; jj++ ) {
		if( depth[jj] == 1 ) {
		    depths[ii] = 1;
		    break;
		}
	    }
	    if( depth )
		XFree( depth );
	}
    }
}

