#ifndef lint
static char rcsid[] = "$Header: Converters.c,v 1.26 88/02/26 12:31:48 swick Exp $";
#endif lint


/***********************************************************
Copyright 1987, 1988 by Digital Equipment Corporation, Maynard, Massachusetts,
and the Massachusetts Institute of Technology, Cambridge, Massachusetts.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the names of Digital or MIT not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.  

DIGITAL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/

/* Conversion.c - implementations of resource type conversion procs */

#include	"IntrinsicI.h"
#include	<X11/CoreP.h>
#include	<X11/Xlib.h>
#include	<X11/Xos.h>
#include	<X11/Xutil.h>
#include	<X11/StringDefs.h>
#include	<stdio.h>
#include	<X11/cursorfont.h>
#include	<X11/Convert.h>
#include	<X11/Quarks.h>
#include	<sys/param.h>		/* just to get MAXPATHLEN */

#define	done(address, type) \
	{ (*toVal).size = sizeof(type); (*toVal).addr = (caddr_t) address; }

static void CvtStringToBoolean();

void XtStringConversionWarning(from, toType)
    char *from, *toType;
{
    char message[1000];
    static enum {Check, Report, Ignore} report_it = Check;

    if (report_it == Check) {
	XrmName xrm_name[3];
	XrmClass xrm_class[3];
	XrmRepresentation rep_type;
	XrmValue value;
	xrm_name[0] = XtApplicationName;
	xrm_name[1] = StringToQuark( "stringConversionWarnings" );
	xrm_name[2] = NULL;
	xrm_class[0] = XtApplicationClass;
	xrm_class[1] = StringToQuark( "StringConversionWarnings" );
	xrm_class[2] = NULL;
	if (XrmQGetResource( XtDefaultDB, xrm_name, xrm_class,
			     &rep_type, &value ))
	{
	    if (rep_type == StringToQuark(XtRBoolean) && value.addr)
		report_it = Report;
	    else if (rep_type == StringToQuark(XtRString)) {
		XrmValue toVal;
		XtDirectConvert( CvtStringToBoolean, NULL, 0, &value, &toVal );
		if (toVal.addr && *(Boolean*)toVal.addr)
		    report_it = Report;
	    }
	}
    }

    if (report_it == Report) {
	(void) sprintf(message, "Cannot convert string \"%s\" to type %s",
		       from, toType);
	XtWarning(message);
    }
}

static void CvtXColorToPixel();
static void CvtDefaultColorToPixel();
static void CvtIntToBoolean();
static void CvtIntToLongBoolean();
static void CvtIntToPixmap();
static void CvtIntToFont();
static void CvtIntOrPixelToXColor();
static void CvtIntToPixel();

static void CvtStringToLongBoolean();
static void CvtStringToXColor();
static void CvtStringToCursor();
static void CvtStringToDisplay();
static void CvtStringToFile();
static void CvtStringToFont();
static void CvtStringToFontStruct();
static void CvtStringToInt();
static void CvtStringToPixel();


/*ARGSUSED*/
static void CvtIntToBoolean(args, num_args, fromVal, toVal)
    XrmValuePtr args;
    Cardinal    *num_args;
    XrmValuePtr	fromVal;
    XrmValuePtr	toVal;
{
    static Boolean	b;

    if (*num_args != 0)
	XtWarning("Integer to Boolean conversion needs no extra arguments");
    b = (*(int *)fromVal->addr != 0);
    done(&b, Boolean);
};


/*ARGSUSED*/
static void CvtStringToBoolean(args, num_args, fromVal, toVal)
    XrmValuePtr args;
    Cardinal    *num_args;
    XrmValuePtr	fromVal;
    XrmValuePtr	toVal;
{
    static Boolean b;
    XrmQuark	q;
    char	lowerName[1000];

    if (*num_args != 0)
	XtWarning("String to Boolean conversion needs no extra arguments");

    LowerCase((char *) fromVal->addr, lowerName);
    q = XrmStringToQuark(lowerName);

    if (q == XtQEtrue || q == XtQEon || q == XtQEyes) {
	b = TRUE;
	done(&b, Boolean);
	return;
    }
    if (q == XtQEfalse || q ==XtQEoff || q == XtQEno) {
	b = FALSE;
	done(&b, Boolean);
	return;
    }

    XtStringConversionWarning((char *) fromVal->addr, "Boolean");
};


/*ARGSUSED*/
static void CvtIntToLongBoolean(args, num_args, fromVal, toVal)
    XrmValuePtr args;
    Cardinal    *num_args;
    XrmValuePtr	fromVal;
    XrmValuePtr	toVal;
{
    static Bool	b;

    if (*num_args != 0)
	XtWarning("Integer to LongBoolean conversion needs no extra arguments");
    b = (*(int *)fromVal->addr != 0);
    done(&b, Bool);
};


/*ARGSUSED*/
static void CvtStringToLongBoolean(args, num_args, fromVal, toVal)
    XrmValuePtr args;
    Cardinal    *num_args;
    XrmValuePtr	fromVal;
    XrmValuePtr	toVal;
{
    static Bool	b;
    XrmQuark	q;
    char	lowerName[1000];

    if (*num_args != 0)
	XtWarning("String to LongBoolean conversion needs no extra arguments");

    LowerCase((char *) fromVal->addr, lowerName);
    q = XrmStringToQuark(lowerName);

    if (q == XtQEtrue || q == XtQEon || q == XtQEyes) {
	b = TRUE;
	done(&b, Bool);
	return;
    }
    if (q == XtQEfalse || q ==XtQEoff || q == XtQEno) {
	b = FALSE;
	done(&b, Bool);
	return;
    }

    XtStringConversionWarning((char *) fromVal->addr, "LongBoolean");
};


static XtConvertArgRec colorConvertArgs[] = {
    {XtBaseOffset, (caddr_t) XtOffset(Widget, core.screen),  sizeof(Screen *)},
    {XtBaseOffset, (caddr_t) XtOffset(Widget, core.colormap),sizeof(Colormap)}
};

static void CvtIntOrPixelToXColor(args, num_args, fromVal, toVal)
    XrmValuePtr args;
    Cardinal    *num_args;
    XrmValuePtr	fromVal;
    XrmValuePtr	toVal;
{    
    static XColor   c;
    Screen	    *screen;
    Colormap	    colormap;

    if (*num_args != 2)
      XtError("Pixel to color conversion needs screen and colormap arguments");

    screen = *((Screen **) args[0].addr);
    colormap = *((Colormap *) args[1].addr);
    c.pixel = *(int *)fromVal->addr;

    XQueryColor(DisplayOfScreen(screen), colormap, &c);
    done(&c, XColor);
};


/*ARGSUSED*/
static void CvtStringToXColor(args, num_args, fromVal, toVal)
    XrmValuePtr args;
    Cardinal    *num_args;
    XrmValuePtr	fromVal;
    XrmValuePtr	toVal;
{
    static XColor   c;
    Screen	    *screen;
    Colormap	    colormap;
    Status	    s;
    char	    message[1000];

    if (*num_args != 2)
     XtError("String to color conversion needs screen and colormap arguments");

    screen = *((Screen **) args[0].addr);
    colormap = *((Colormap *) args[1].addr);
    s = XParseColor(DisplayOfScreen(screen), colormap, (char *) fromVal->addr,
	&c);
    if (s == 0) {
	XtStringConversionWarning((char *) fromVal->addr, "Color");
	return;
    }
    s = XAllocColor(DisplayOfScreen(screen), colormap, &c);
    if (s == 0) {
	(void) sprintf(message, "Cannot allocate colormap entry for %s",
	    (char *) fromVal->addr);
	XtWarning(message);
	return;
    }
    done(&c, XColor);
};


static XtConvertArgRec screenConvertArg[] = {
    {XtBaseOffset, (caddr_t) XtOffset(Widget, core.screen), sizeof(Screen *)}
};

#ifndef BITMAPDIR
#define BITMAPDIR "/usr/include/X11/bitmaps"
#endif

/*ARGSUSED*/
static void CvtStringToCursor(args, num_args, fromVal, toVal)
    XrmValuePtr args;
    Cardinal    *num_args;
    XrmValuePtr	fromVal;
    XrmValuePtr	toVal;
{
    static struct _CursorName {
	char	*name;
	int	shape;
    } cursor_names[] = {
			{"X_cursor",		XC_X_cursor},
			{"arrow",		XC_arrow},
			{"based_arrow_down",	XC_based_arrow_down},
			{"based_arrow_up",	XC_based_arrow_up},
			{"boat",		XC_boat},
			{"bogosity",		XC_bogosity},
			{"bottom_left_corner",	XC_bottom_left_corner},
			{"bottom_right_corner",	XC_bottom_right_corner},
			{"bottom_side",		XC_bottom_side},
			{"bottom_tee",		XC_bottom_tee},
			{"box_spiral",		XC_box_spiral},
			{"center_ptr",		XC_center_ptr},
			{"circle",		XC_circle},
			{"clock",		XC_clock},
			{"coffee_mug",		XC_coffee_mug},
			{"cross",		XC_cross},
			{"cross_reverse",	XC_cross_reverse},
			{"crosshair",		XC_crosshair},
			{"diamond_cross",	XC_diamond_cross},
			{"dot",			XC_dot},
			{"dotbox",		XC_dotbox},
			{"double_arrow",	XC_double_arrow},
			{"draft_large",		XC_draft_large},
			{"draft_small",		XC_draft_small},
			{"draped_box",		XC_draped_box},
			{"exchange",		XC_exchange},
			{"fleur",		XC_fleur},
			{"gobbler",		XC_gobbler},
			{"gumby",		XC_gumby},
			{"hand1",		XC_hand1},
			{"hand2",		XC_hand2},
			{"heart",		XC_heart},
			{"icon",		XC_icon},
			{"iron_cross",		XC_iron_cross},
			{"left_ptr",		XC_left_ptr},
			{"left_side",		XC_left_side},
			{"left_tee",		XC_left_tee},
			{"leftbutton",		XC_leftbutton},
			{"ll_angle",		XC_ll_angle},
			{"lr_angle",		XC_lr_angle},
			{"man",			XC_man},
			{"middlebutton",	XC_middlebutton},
			{"mouse",		XC_mouse},
			{"pencil",		XC_pencil},
			{"pirate",		XC_pirate},
			{"plus",		XC_plus},
			{"question_arrow",	XC_question_arrow},
			{"right_ptr",		XC_right_ptr},
			{"right_side",		XC_right_side},
			{"right_tee",		XC_right_tee},
			{"rightbutton",		XC_rightbutton},
			{"rtl_logo",		XC_rtl_logo},
			{"RTL_logo",		XC_rtl_logo},
			{"sailboat",		XC_sailboat},
			{"sb_down_arrow",	XC_sb_down_arrow},
			{"sb_h_double_arrow",	XC_sb_h_double_arrow},
			{"sb_left_arrow",	XC_sb_left_arrow},
			{"sb_right_arrow",	XC_sb_right_arrow},
			{"sb_up_arrow",		XC_sb_up_arrow},
			{"sb_v_double_arrow",	XC_sb_v_double_arrow},
			{"shuttle",		XC_shuttle},
			{"sizing",		XC_sizing},
			{"spider",		XC_spider},
			{"spraycan",		XC_spraycan},
			{"star",		XC_star},
			{"target",		XC_target},
			{"tcross",		XC_tcross},
			{"top_left_arrow",	XC_top_left_arrow},
			{"top_left_corner",	XC_top_left_corner},
			{"top_right_corner",	XC_top_right_corner},
			{"top_side",		XC_top_side},
			{"top_tee",		XC_top_tee},
			{"trek",		XC_trek},
			{"ul_angle",		XC_ul_angle},
			{"umbrella",		XC_umbrella},
			{"ur_angle",		XC_ur_angle},
			{"watch",		XC_watch},
			{"xterm",		XC_xterm},
    };
    struct _CursorName *cache;
    static Cursor cursor;
    char *name = (char *)fromVal->addr;
    register int i;
    Screen *screen;
    XrmName app_name;
    XrmClass app_class;
    static char* bitmap_file_path = NULL;
    char filename[MAXPATHLEN];
    Pixmap source, mask;
    static XColor bgColor = {0, 0, 0, 0};
    static XColor fgColor = {0, ~0, ~0, ~0};
    int width, height, xhot, yhot;

    if (*num_args != 1)
     XtError("String to cursor conversion needs screen argument");

    screen = *((Screen **) args[0].addr);
    for (i=0, cache=cursor_names; i < XtNumber(cursor_names); i++, cache++ ) {
        /* cacheing is actually done by higher layers of Xrm */
	if (strcmp(name, cache->name) == 0) {
	    cursor = XtGetCursor(DisplayOfScreen(screen), cache->shape);
	    done(&cursor, Cursor);
	    return;
	}
    }
    /* isn't a standard cursor in cursorfont; try to open a bitmap file */
    if (bitmap_file_path == NULL) {
	XrmName xrm_name[3];
	XrmClass xrm_class[3];
	XrmRepresentation rep_type;
	XrmValue value;
	xrm_name[0] = XtApplicationName;
	xrm_name[1] = StringToQuark( "bitmapFilePath" );
	xrm_name[2] = NULL;
	xrm_class[0] = XtApplicationClass;
	xrm_class[1] = StringToQuark( "BitmapFilePath" );
	xrm_class[2] = NULL;
	if (XrmQGetResource( XtDefaultDB, xrm_name, xrm_class,
			     &rep_type, &value )
	    && rep_type == StringToQuark(XtRString))
	    bitmap_file_path = value.addr;
	else
	    bitmap_file_path = BITMAPDIR;
    }

    sprintf( filename, "%s/%s", bitmap_file_path, name );
    if (XReadBitmapFile( DisplayOfScreen(screen), RootWindowOfScreen(screen),
			 filename, &width, &height, &source, &xhot, &yhot )
	!= BitmapSuccess) {
	XtStringConversionWarning( name, "Cursor" );
	cursor = None;		/* absolute fall-back for failed conversion */
	done(&cursor, Cursor);
	return;
    }
    (void) strcat( filename, "Mask" );
    if (XReadBitmapFile( DisplayOfScreen(screen), RootWindowOfScreen(screen),
			 filename, &width, &height, &mask, &width, &height )
	!= BitmapSuccess) {
	mask = None;
    }
    cursor = XCreatePixmapCursor( DisplayOfScreen(screen), source, mask,
				  &fgColor, &bgColor, xhot, yhot );
    XFreePixmap( DisplayOfScreen(screen), source );
    XFreePixmap( DisplayOfScreen(screen), mask );

    done(&cursor, Cursor);
};


/*ARGSUSED*/
static void CvtStringToDisplay(args, num_args, fromVal, toVal)
    XrmValuePtr args;
    Cardinal    *num_args;
    XrmValuePtr	fromVal;
    XrmValuePtr	toVal;
{
    static Display	*d;

    if (*num_args != 0)
	XtWarning("String to Display conversion needs no extra arguments");

    d = XOpenDisplay((char *)fromVal->addr);
    if (d != NULL) {
	done(d, Display);
    } else {
	XtStringConversionWarning((char *) fromVal->addr, "Display");
    }
};


/*ARGSUSED*/
static void CvtStringToFile(args, num_args, fromVal, toVal)
    XrmValuePtr args;
    Cardinal    *num_args;
    XrmValuePtr	fromVal;
    XrmValuePtr	toVal;
{
    static FILE	*f;

    if (*num_args != 0)
	XtWarning("String to File conversion needs no extra arguments");

    f = fopen((char *)fromVal->addr, "r");
    if (f != NULL) {
	done(f, FILE);
    } else {
	XtStringConversionWarning((char *) fromVal->addr, "File");
    }
};


/*ARGSUSED*/
static void CvtStringToFont(args, num_args, fromVal, toVal)
    XrmValuePtr args;
    Cardinal    *num_args;
    XrmValuePtr	fromVal;
    XrmValuePtr	toVal;
{
    static Font	f;
    Screen	    *screen;

    if (*num_args != 1)
     XtError("String to font conversion needs screen argument");

    screen = *((Screen **) args[0].addr);
    f = XLoadFont(DisplayOfScreen(screen), (char *)fromVal->addr);
    if (f != 0) {
	done(&f, Font);
    } else {
	XtStringConversionWarning((char *) fromVal->addr, "Font");
    }
}


/*ARGSUSED*/
static void CvtIntToFont(args, num_args, fromVal, toVal)
    XrmValuePtr args;
    Cardinal    *num_args;
    XrmValuePtr	fromVal;
    XrmValuePtr	toVal;
{
    if (*num_args != 0)
	XtWarning("Integer to Font conversion needs no extra arguments");
    done(fromVal->addr, int);
};


/*ARGSUSED*/
static void CvtStringToFontStruct(args, num_args, fromVal, toVal)
    XrmValuePtr args;
    Cardinal    *num_args;
    XrmValuePtr	fromVal;
    XrmValuePtr	toVal;
{
    static XFontStruct	*f;
    Screen	    *screen;

    if (*num_args != 1)
     XtError("String to cursor conversion needs screen argument");

    screen = *((Screen **) args[0].addr);
    f = XLoadQueryFont(DisplayOfScreen(screen), (char *)fromVal->addr);
    if (f != NULL) {
	done(&f, XFontStruct *);
    } else {
	XtStringConversionWarning((char *) fromVal->addr, "XFontStruct");
    }
}

/*ARGSUSED*/
static void CvtStringToInt(args, num_args, fromVal, toVal)
    XrmValuePtr args;
    Cardinal    *num_args;
    XrmValuePtr	fromVal;
    XrmValuePtr	toVal;
{
    static int	i;

    if (*num_args != 0)
	XtWarning("String to Integer conversion needs no extra arguments");
    if (sscanf((char *)fromVal->addr, "%d", &i) == 1) {
	done(&i, int);
    } else {
	XtStringConversionWarning((char *) fromVal->addr, "Integer");
    }
}


/*ARGSUSED*/
static void CvtStringToPixel(args, num_args, fromVal, toVal)
    XrmValuePtr args;
    Cardinal    *num_args;
    XrmValuePtr	fromVal;
    XrmValuePtr	toVal;
{
    XtDirectConvert((XtConverter) CvtStringToXColor, args, *num_args,
	    fromVal, toVal);
    if ((*toVal).addr == NULL) return;
    done(&((XColor *)toVal->addr)->pixel, int);
};


/*ARGSUSED*/
static void CvtXColorToPixel(args, num_args, fromVal, toVal)
    XrmValuePtr args;
    Cardinal    *num_args;
    XrmValuePtr	fromVal;
    XrmValuePtr	toVal;
{
    if (*num_args != 0)
	XtWarning("Color to Pixel conversion needs no extra arguments");
    done(&((XColor *)fromVal->addr)->pixel, int);
};

/*ARGSUSED*/
static void CvtDefaultColorToPixel(args, num_args, fromVal, toVal)
    XrmValuePtr args;
    Cardinal    *num_args;
    XrmValuePtr fromVal;
    XrmValuePtr toVal;
{
    Screen* screen;
    if (*num_args != 1)
        XtWarning("DefaultColor to Pixel conversion needs screen argument");
    screen = *((Screen **) args[0].addr);
    if (*(Pixel*)fromVal->addr == 0)
        done(&screen->black_pixel, Pixel)
    else if (*(Pixel*)fromVal->addr == 1)
        done(&screen->white_pixel, Pixel)


};

/*ARGSUSED*/
static void CvtIntToPixel(args, num_args, fromVal, toVal)
    XrmValuePtr args;
    Cardinal    *num_args;
    XrmValuePtr	fromVal;
    XrmValuePtr	toVal;
{
    if (*num_args != 0)
	XtWarning("Integer to Pixel conversion needs no extra arguments");
    done(fromVal->addr, int);
};

/*ARGSUSED*/
static void CvtIntToPixmap(args, num_args, fromVal, toVal)
    XrmValuePtr args;
    Cardinal    *num_args;
    XrmValuePtr fromVal;
    XrmValuePtr toVal;
{
    if (*num_args != 0)
        XtWarning("Integer to Pixmap conversion needs no extra arguments");
    done(fromVal->addr, int);
};



void LowerCase(source, dest)
    register char  *source, *dest;
{
    register char ch;

    for (; (ch = *source) != 0; source++, dest++) {
    	if ('A' <= ch && ch <= 'Z')
	    *dest = ch - 'A' + 'a';
	else
	    *dest = ch;
    }
    *dest = 0;
}

XrmQuark  XtQBoolean;
XrmQuark  XtQLongBoolean;
XrmQuark  XtQColor;
XrmQuark  XtQCursor;
XrmQuark  XtQDims;
XrmQuark  XtQDisplay;
XrmQuark  XtQFile;
XrmQuark  XtQFloat;
XrmQuark  XtQFont;
XrmQuark  XtQFontStruct;
XrmQuark  XtQGeometry;
XrmQuark  XtQInt;
XrmQuark  XtQPixel;
XrmQuark  XtQPixmap;
XrmQuark  XtQPointer;
XrmQuark  XtQString;
XrmQuark  XtQWindow;
XrmQuark  XtQDefaultColor;

XrmQuark  XtQEoff;
XrmQuark  XtQEfalse;
XrmQuark  XtQEno;
XrmQuark  XtQEon;
XrmQuark  XtQEtrue;
XrmQuark  XtQEyes;

static Boolean initialized = FALSE;

void _XtConvertInitialize()
{
    if (initialized) return;
    initialized = TRUE;

/* Representation types */

    XtQBoolean		= XrmStringToQuark(XtRBoolean);
    XtQLongBoolean	= XrmStringToQuark(XtRLongBoolean);
    XtQColor		= XrmStringToQuark(XtRColor);
    XtQCursor		= XrmStringToQuark(XtRCursor);
    XtQDisplay		= XrmStringToQuark(XtRDisplay);
    XtQFile		= XrmStringToQuark(XtRFile);
    XtQFont		= XrmStringToQuark(XtRFont);
    XtQFontStruct	= XrmStringToQuark(XtRFontStruct);
    XtQGeometry		= XrmStringToQuark(XtRGeometry);
    XtQInt		= XrmStringToQuark(XtRInt);
    XtQPixel		= XrmStringToQuark(XtRPixel);
    XtQPixmap		= XrmStringToQuark(XtRPixmap);
    XtQPointer		= XrmStringToQuark(XtRPointer);
    XtQString		= XrmStringToQuark(XtRString);
    XtQWindow		= XrmStringToQuark(XtRWindow);
    XtQDefaultColor     = XrmStringToQuark(XtRDefaultColor);

/* Boolean enumeration constants */

    XtQEfalse		= XrmStringToQuark(XtEfalse);
    XtQEno		= XrmStringToQuark(XtEno);
    XtQEoff		= XrmStringToQuark(XtEoff);
    XtQEon		= XrmStringToQuark(XtEon);
    XtQEtrue		= XrmStringToQuark(XtEtrue);
    XtQEyes		= XrmStringToQuark(XtEyes);

#define Add(from, to, proc, convert_args, num_args) \
    _XtAddConverter(from, to, (XtConverter) proc, \
	    (XtConvertArgList) convert_args, num_args)

    Add(XtQColor,   XtQPixel,       CvtXColorToPixel,	    NULL, 0);
    Add(XtQDefaultColor, XtQPixel,  CvtDefaultColorToPixel,
       screenConvertArg, XtNumber(screenConvertArg));
    Add(XtQInt,     XtQBoolean,     CvtIntToBoolean,	    NULL, 0);
    Add(XtQInt,     XtQLongBoolean, CvtIntToLongBoolean,    NULL, 0);
    Add(XtQInt,     XtQPixel,       CvtIntToPixel,          NULL, 0);
    Add(XtQInt,     XtQPixmap,      CvtIntToPixmap,	    NULL, 0);
    Add(XtQInt,     XtQFont,	    CvtIntToFont,	    NULL, 0);
    Add(XtQInt,     XtQColor,       CvtIntOrPixelToXColor,  
	colorConvertArgs, XtNumber(colorConvertArgs));

    Add(XtQString,  XtQBoolean,     CvtStringToBoolean,     NULL, 0);
    Add(XtQString,  XtQLongBoolean, CvtStringToLongBoolean, NULL, 0);
    Add(XtQString,  XtQColor,       CvtStringToXColor,      
	colorConvertArgs, XtNumber(colorConvertArgs));
    Add(XtQString,  XtQCursor,      CvtStringToCursor,      
	screenConvertArg, XtNumber(screenConvertArg));
    Add(XtQString,  XtQDisplay,     CvtStringToDisplay,     NULL, 0);
    Add(XtQString,  XtQFile,	    CvtStringToFile,	    NULL, 0);
    Add(XtQString,  XtQFont,	    CvtStringToFont,	    
	screenConvertArg, XtNumber(screenConvertArg));
    Add(XtQString,  XtQFontStruct,  CvtStringToFontStruct,  
	screenConvertArg, XtNumber(screenConvertArg));
    Add(XtQString,  XtQInt,	    CvtStringToInt,	    NULL, 0);
    Add(XtQString,  XtQPixel,       CvtStringToPixel,       
	colorConvertArgs, XtNumber(colorConvertArgs));

    Add(XtQPixel,   XtQColor,       CvtIntOrPixelToXColor,  
	colorConvertArgs, XtNumber(colorConvertArgs));

}
