#ifndef SUNTOOLS
/*
 * X Windows routines - These pretty closely mirror the SunView interface in
 * sunwindows.c. The main difference is that this has more and more flexible
 * bindings, and has the Next-Section, Prev-Section bindings. This one also
 * lacks the StatusPanel stuff - nice, but somewhat unnecessary in my opinion.
 * Instead, it has an emacs-style Minibuffer at the bottom for interaction.
 * Sigh! The suntroff code is remarkably independent of the window system -
 * the routines in this file or windows.c set up the frame window (a Form),
 * the drawing window (Chris Peterson's Window.c, lifted straight from xman.
 * It should be in Xaw), the event handlers, and menus(the menu widget in
 * contrib/menus). The only other X dependent code is in draw.c and a small
 * chunk in the LoadFontBits() routine in font.c!  Its been ifdef'ed assuming
 * that if you're not compiling for SUNTOOLS, you're compiling for X11.  -
 * Mark Moraes.
 */
#ifndef lint
static char rcsid[]="$Header: xwindows.c,v 1.2 89/12/15 12:02:39 kit Exp $";
#endif

#include	<stdio.h>
#include	<ctype.h>
#include	<signal.h>
#include	<sys/wait.h>
#include	<X11/Intrinsic.h>
#include	<X11/StringDefs.h>
#include	<X11/Shell.h>
#include	"x11.h"
#ifdef R4
# include 	<X11/Xaw/Scrollbar.h>
# include	<X11/Xaw/Form.h>
# include	<X11/Xaw/SimpleMenu.h>
# include	<X11/Xaw/SmeBSB.h>
# define XtScrollBarSetThumb XawScrollBarSetThumb
#else
# include 	<X11/Scroll.h>
# include	<X11/Form.h>
#endif
#include	"Window.h"	/* Chris Peterson's Window widget */
#include 	"Minibuf.h"	/* My Minibuf widget */
#include 	"Menu.h"	/* XtStuff after Dana Chee's fixes */
#include	<X11/cursorfont.h>
#include	<X11/Xutil.h>

#define MAXSTR 512

#define YES 	 1
#define NO 	 0
#define ABORT 	-1

/*
 *  These are the various actions allowed in the canvas window - each
 *  related to a procedure
 */
static void StartPan(), EndPan(), PagePan(), Number(), EscapeNumber(), 
	GoToPage(), ForwardPage(), BackPage(), NextSection(), PrevSection(),
	Quit(),	SetMenuPos(), MenuSelection(), Rerasterize(),
	MousePrintPage(), MousePrintDocument(),
	SearchForward(), SearchBackward();

static XtActionsRec canvasActionTable[] = {
	{"SetMenuPos",	SetMenuPos},
	{"start-pan",	StartPan},
	{"end-pan",	EndPan},
	{"page-pan", 	PagePan},
	{"number",	Number},
	{"escape",	EscapeNumber},
	{"go-to-page",	GoToPage},
	{"forward-page",ForwardPage},
	{"back-page",	BackPage},
	{"print-page",	MousePrintPage},
	{"print-document",	MousePrintDocument},
	{"next-section",NextSection},
	{"prev-section",PrevSection},
	{"rerasterize", Rerasterize},
	{"search-forward",SearchForward},
	{"search-backward",SearchBackward},
	{"quit",	Quit},
};

#include	"suntroff.h"

/* Preserved most of the sunview names */
Window	OuterFrame;
Widget	BaseFrame;		/* Top Level Window Frame */
Widget	CanvasWidget;
Window	DrawingCanvas;		/* Main Drawing Window for Text */
Widget	PopupShell;		/* Popup Shell enclosing the Main menu */
Widget	HorizScroll;		/* Horizontal Scroll Bar */
Widget	VertScroll;		/* Vertical Scroll Bar */
Widget	InputWidget = NULL;	/* Minibuffer widget for input and dislay */
Window 	InputWin;
Pixmap	PagePixRect;		/* Full Page Pix Rect for Drawing Page */
int	ViewLeft = 0, ViewTop = 0;	/* Page Offset for display */
/* Set these to the window height and width */
int 	ViewWidth = 0, ViewHeight = 0;
int	OriginalX = -1;
int	OriginalY = -1;
char	*homeDir;

/* 
 *  The following icon of a hand was generated by Stefano Concino@SPAR.
 */
#define hand_width 16
#define hand_height 16
#define hand_x_hot 8
#define hand_y_hot 8
static char hand_bits[] = {
   0x80, 0x00, 0x40, 0x09, 0x48, 0x15, 0x54, 0x15, 0x54, 0x15, 0x54, 0x15,
   0x54, 0x15, 0x54, 0x15, 0x74, 0xd2, 0x04, 0xa8, 0x08, 0xa8, 0x18, 0xa8,
   0x30, 0x90, 0x20, 0x40, 0x20, 0x20, 0x60, 0x18};

static char hand_mask_bits[] = {
   0xc0, 0x09, 0x68, 0x1d, 0x7e, 0x37, 0x76, 0x37, 0x76, 0x37, 0x76, 0x37,
   0x76, 0x37, 0x76, 0xf7, 0x76, 0xf2, 0x06, 0xb8, 0x0e, 0xb8, 0x1c, 0xb8,
   0x3c, 0x90, 0x38, 0xc0, 0x30, 0x60, 0x70, 0x38};

#include "ditroff.bm"

Cursor	HandCursor;
Cursor	WorkingCursor;
Cursor	WaitCursor;
Cursor	TextCursor;

XWMHints xwmh;

Pixel fgcolor, bgcolor;
/* Needed for the undocumented XCreatePixmapCursor() */
static XColor bg = {0, 0, 0, 0};
static XColor fg = {0, ~0, ~0, ~0};
unsigned int depth;
Dimension xwidth, xheight;

Display *dpy;
Screen *scn;
GC gc;			/* The Graphic context we use for all drawing */
GC fillgc;		/* Used for filled objects - presently only bullets */
GC cleargc;		/* The GC used for clearing the Pixmap */

/*
 *  DON'T CHANGE THE ORDER OF THE ARGS IN THE VARIOUS ARG STRUCTS. IF
 *  YOU WANT TO ADD STUFF, ADD IT AT THE END OF THE STRUCT, BECAUSE WE
 *  REFER TO SOME OF THE ELEMENTS BY POSITION IN THE CODE.
 */
/*
 * These are the few resources we want to hardcode, since altering them
 * would change xtroff too dramatically.
 */

static void finished_input();
static XtCallbackRec minibufCallbacks[] = {
	{finished_input, NULL},
	{NULL, NULL},
};

static Arg minibuf_args[] = {
	{XtNwidth, (XtArgVal) 0},
	{XtNfinishedCallback, (XtArgVal) minibufCallbacks},
	{XtNfromVert, (XtArgVal) NULL},
};

/* we use this when we ask what the various attributes of the Canvas. */
static Arg query_args[] = {
	{XtNbackground, (XtArgVal) &bgcolor},
	{XtNforeground, (XtArgVal) &fgcolor},
	{XtNwidth, (XtArgVal) &xwidth},
	{XtNheight, (XtArgVal) &xheight},
};

/* we change this to "full" if -full is specified */
static char *canvas_name = "canvas";

static void Scrolled();
static XtCallbackRec scrollCallbacks[] = {
	{ Scrolled, NULL },
	{ NULL, NULL },
};

static void Jumped();
static XtCallbackRec jumpCallbacks[] = {
	{ Jumped, NULL },
	{ NULL, NULL },
};

/* 
 *  We change the XtNlength, XtNorientation and XtNfromHoriz by getting
 *  the size from the Canvas - don't take this value seriously.
 *  XtNfromHoriz also changes to XtNfromVert.
 */
static Arg scroll_args[] = {
	{XtNlength, 0},
	{XtNorientation, (XtArgVal) XtorientVertical},
	{XtNfromHoriz, (XtArgVal) NULL},
	{XtNscrollProc, (XtArgVal) scrollCallbacks},
	{XtNjumpProc, (XtArgVal) jumpCallbacks},
};

/*
 * We use this for both R3 (using XtStuff's menu widget) and R4 (using
 * Xaw SimpleMenu)
 */
static MenuItemsList menuList[] = {
	{"Rerasterize",   MenuSelection, (caddr_t) 1, 0},
	{"Next Page",   MenuSelection, (caddr_t) 2, 0},
	{"Previous Page", MenuSelection, (caddr_t) 3, 0},
	{"Next Page Section",   MenuSelection, (caddr_t) 4, 0},
	{"Previous Page Section", MenuSelection, (caddr_t) 5, 0},
	{"Show Status", MenuSelection, (caddr_t) 6, 0},
	{"Search Forward", MenuSelection, (caddr_t) 7, 0},
	{"Search Backward", MenuSelection, (caddr_t) 8, 0},
	{"Change File", MenuSelection, (caddr_t) 9, 0},
	{"Change Command", MenuSelection, (caddr_t) 10, 0},
	{"Go To Page", MenuSelection, (caddr_t) 11, 0},
	{"Print Page", MenuSelection, (caddr_t) 12, 0},
	{"Print Document", MenuSelection, (caddr_t) 13, 0},
	{"Set Printer", MenuSelection, (caddr_t) 14, 0},
	{"Quit",       MenuSelection, (caddr_t) 0, 0},
	{NULL, NULL, NULL, NULL}
};

static Arg ourTopLevelShellArgs[] = {
	{ XtNallowShellResize, (XtArgVal) TRUE },
	{ XtNinput, (XtArgVal) TRUE },
	{ XtNiconPixmap, 0 },
};

/* structure containing program level resources */
typedef struct {
    char *font_map_string;
    char *lprcommand;
    char *lprinter;
    Boolean scrollbars;
} ResourceStruct;

ResourceStruct xtroff_resources;

#define offset(field) ((Cardinal) (XtOffset(ResourceStruct *, field)))

/*
 * Note: ALL the defaults specified here are lies. The real defaults are
 * in the XTroff.ad file. Change things there.
 */
static XtResource application_resources[] = {
        {"fontMap", "FontMap", XtRString, sizeof (char *),
         offset(font_map_string), XtRString,
	 (caddr_t) "R	-*-unknown-medium-r-normal--*-*-75-75-*-*-iso8859-1"},
	{"printer", "Printer", XtRString, sizeof(char *),
	 offset(lprinter), XtRString, (caddr_t) "unknown"},
	{"lprcommand", "LprCommand", XtRString, sizeof(char *),
	 offset(lprcommand), XtRString, (caddr_t) "cat > /dev/null"},
	{"scrollbars", "Scrollbars", XtRBoolean, sizeof(Boolean),
	 offset(scrollbars), XtRString, (caddr_t) "off"},
};

/*
 * !! I hate fixed sizes. One day, I'll make this use my dynamic strings
 * package. I also wish there weren't so many globals.
 */
static int CurrentPage = 1;
char	FileName[MAXSTR] = "";	/* File containing ditroff output */
char	TempFileName[MAXSTR];	/* Temp file that can be deleted. */
char	CommandString[MAXSTR] = "";/* String pointer to ditroff command */
char	*PrinterCommand = NULL;
char	*ActualFileName;	/* File that buffers ditroff output */
int	PageRequest = 0;	/* Partially read page number request */
int	CommandMode = 0;	/* Taking input from file or command line */
/* not used in the X version - we don't print any messages. sigh! one day... */
int	ErrorsPending = 0;	/* Set if errors are pending and need to be
				 * displayed.  Used in case tool is iconic
				 * when error message is needed.  This boolean
				 * variable is checked at redisplay time.
				 * This was done at the suggestion of
				 * davy@ecn.purdue.edu.......11/28/86
				 */
int	isMapped = 0;
char 	*SearchItem;
int	SUNRES = 120;

static void RecordMapStatus();
static Widget CreateMenu();

#ifndef R4
/* Pre-R4 didn't provide this -- Scrollbar needs it */
#define	done(address, type) \
	{ (*toVal).size = sizeof(type); (*toVal).addr = (caddr_t) address; }

/*ARGSUSED*/
static Boolean
CvtStringToFloat(args, num_args, fromVal, toVal)
    XrmValuePtr args;
    Cardinal    *num_args;
    XrmValuePtr	fromVal;
    XrmValuePtr	toVal;
{
    static float f;
    extern double atof();
    
    f = atof((char *)fromVal->addr);
    done(&f, float);
}
#endif /* ! R4 */

main(argc, argv)
int argc;
char **argv;
{
	XGCValues   gcv;		/* Struct for creating GC */
	Pixmap cursorPixmap, cursorMask;
	void RepaintCanvas();
	Widget topLevel;
	extern char *getenv();
	char *lp;

	if ((homeDir = getenv("HOME")) == NULL)
		homeDir = "";
	/* make the top level using argc, argv */
	topLevel = XtInitialize("xtroff", "XTroff",
	 (XrmOptionDescRec *) NULL, 0, (Cardinal *) &argc, argv);
#ifndef R4
	XtAddConverter(XtRString, XtRFloat, CvtStringToFloat,
		       (XtConvertArgList) NULL, (Cardinal) 0);
#endif /* ! R4 */
        XtGetApplicationResources(topLevel, (caddr_t) &xtroff_resources,
				  application_resources,
                                  XtNumber(application_resources),
				  (ArgList) NULL, (Cardinal) 0 );
	if ((lp = getenv("PRINTER")) != NULL)
	    xtroff_resources.lprinter = lp;
	PrinterCommand = XtMalloc((unsigned)
				  (strlen(xtroff_resources.lprcommand) +
				  strlen(xtroff_resources.lprinter) + 1));
	(void) sprintf(PrinterCommand, xtroff_resources.lprcommand,
		       xtroff_resources.lprinter);
	dpy = XtDisplay(topLevel);
	scn = XtScreen(topLevel);
	depth = DefaultDepthOfScreen(scn);
	ourTopLevelShellArgs[2].value = XCreateBitmapFromData(dpy, scn->root,
			      ditroff_bits, ditroff_width, ditroff_height);
	XtSetValues(topLevel, ourTopLevelShellArgs,
		    XtNumber(ourTopLevelShellArgs));
	/* just 3 possible options for ourselves - rest are Toolkit options */
	argv++;
	while (*argv && argv[0][0] == '-'){
		if (strncmp(argv[0],"-scrollbar",4) == NULL){
			xtroff_resources.scrollbars = 0;
		} else if (strncmp(argv[0],"-full",3) == NULL){
			SUNRES = 75;
			canvas_name = "full";
		} else if (strncmp(argv[0],"-command",3) == NULL){
			int len = MAXSTR-1;
			
			*CommandString = '\0';
			while (*++argv) {
				if (strlen(*argv) + 1 > len) {
					/* Yech!! */
					fprintf(stderr,
					 "Command too long -- max %d\n",
					 MAXSTR);
				}
				strcat(CommandString,*argv);
				strcat(CommandString, " ");
				len -= strlen(*argv) + 1;
			}
			CommandMode = 1;
		} else {
			fprintf(stderr,"Unknown option %s\n",argv[0]);
		}
		if (*argv){
			argv++;
		}
	}
	if (argv[0] && argv[0][0]){
		CommandMode = 0;
		strcpy(FileName,argv[0]);
	}
#ifdef DEBUG
	if (CommandMode)
		printf("Command = \"%s\"\n", CommandString);
	else {
		if (FileName) printf("File = \"%s\"\n", FileName);
		else printf ("reading from stdin");
	}
#endif
	/* CreatePopUpPanel(); */
	XtAddActions(canvasActionTable, XtNumber(canvasActionTable));
	BaseFrame = XtCreateManagedWidget("form", formWidgetClass, topLevel,
	 (Arg *) NULL, (Cardinal) 0);
	CanvasWidget = XtCreateManagedWidget(canvas_name, windowWidgetClass, 
	 BaseFrame, (Arg *) NULL, (Cardinal) 0);
	XtGetValues(CanvasWidget, query_args, XtNumber(query_args));
	ViewWidth = xwidth;
	ViewHeight = xheight;
	if (xtroff_resources.scrollbars) {
		/* vertical scrollbar to the right of the canvas */
		XtSetArg(scroll_args[0], XtNlength, xheight);
		XtSetArg(scroll_args[2], XtNfromHoriz, CanvasWidget);
		VertScroll = XtCreateManagedWidget("vscroll", 
		 scrollbarWidgetClass, BaseFrame, 
		 scroll_args, XtNumber(scroll_args));
		/* horizontal scrollbar below the canvas */
		XtSetArg(scroll_args[0], XtNlength, xwidth);
		XtSetArg(scroll_args[1], XtNorientation, XtorientHorizontal);
		XtSetArg(scroll_args[2], XtNfromVert, CanvasWidget);
		HorizScroll = XtCreateManagedWidget("hscroll", 
		 scrollbarWidgetClass, BaseFrame, 
		 scroll_args, XtNumber(scroll_args));
	}
	XtAddEventHandler(CanvasWidget, (EventMask) ExposureMask, NULL, 
	 RepaintCanvas, "redraw_data");
	XtAddEventHandler(CanvasWidget, (EventMask) StructureNotifyMask, NULL, 
	 RecordMapStatus, "map_data");
	PopupShell = CreateMenu("popupShell", CanvasWidget, menuList);
	minibuf_args[0].value = (XtArgVal) ViewWidth;
	if (xtroff_resources.scrollbars)
		minibuf_args[2].value = (XtArgVal) HorizScroll;
	else
		minibuf_args[2].value = (XtArgVal) CanvasWidget;
	InputWidget = XtCreateManagedWidget("minibuf", minibufWidgetClass,
		BaseFrame, minibuf_args, XtNumber(minibuf_args));

	XtRealizeWidget(topLevel);
	DrawingCanvas = XtWindow(CanvasWidget);
	OuterFrame = XtWindow(topLevel);
	InputWin = XtWindow(InputWidget);

	fg.pixel = fgcolor;
	bg.pixel = bgcolor;
	XQueryColor(dpy, DefaultColormapOfScreen(scn), &fg);
	XQueryColor(dpy, DefaultColormapOfScreen(scn), &bg);
	cursorPixmap = XCreateBitmapFromData(dpy, DrawingCanvas, 
	 hand_bits, hand_width, hand_height);
	cursorMask = XCreateBitmapFromData(dpy, DrawingCanvas, 
	 hand_mask_bits, hand_width, hand_height);
	HandCursor = XCreatePixmapCursor(dpy, cursorPixmap, cursorMask, 
	 &fg, &bg, hand_x_hot, hand_y_hot);
	XFreePixmap(dpy, cursorPixmap);
	XFreePixmap(dpy, cursorMask);

	WorkingCursor = XCreateFontCursor(dpy, XC_diamond_cross);
	WaitCursor = XCreateFontCursor(dpy, XC_watch);
	TextCursor = XCreateFontCursor(dpy, XC_xterm);
	XDefineCursor(dpy, DrawingCanvas, WorkingCursor);
	gcv.font = XLoadFont(dpy, "variable");
	gcv.foreground = fgcolor;
	gcv.background = bgcolor;
	gcv.function = GXcopy;
	gc = XCreateGC(dpy, DrawingCanvas, 
	 (GCForeground | GCBackground | GCFunction | GCFont), &gcv);
	gcv.foreground = bgcolor;
	gcv.background = fgcolor;
	cleargc = XCreateGC(dpy, DrawingCanvas, 
	 (GCForeground | GCBackground | GCFunction), &gcv);
	gcv.foreground = fgcolor;
	gcv.background = bgcolor;
	gcv.function = GXcopy;
	gcv.fill_style = FillSolid;
	gcv.arc_mode = ArcPieSlice;
	fillgc = XCreateGC(dpy, DrawingCanvas, 
	 (GCForeground | GCBackground | GCFunction | GCFillStyle | GCArcMode),
	 &gcv);
	/* Alloc a 1 bit deep pixmap if PAGE_PIXEL_WIDTH X PAGE_PIXEL_HEIGHT */
	PagePixRect = XCreatePixmap(dpy, DrawingCanvas,
	 (unsigned int) PAGE_PIXEL_WIDTH, (unsigned int) PAGE_PIXEL_HEIGHT,
	 depth);
	XFillRectangle(dpy, PagePixRect, cleargc, 0, 0, 
	 (unsigned int) PAGE_PIXEL_WIDTH, (unsigned int) PAGE_PIXEL_HEIGHT);
	XFlush(dpy);
	/* So if popen() fails, we don't get zonked in PrintDocument */
	(void) signal(SIGPIPE, SIG_IGN);
	/*
	 *  We'll keep changing fonts - simpler, but is it fast enough?
	 *  Should we try setting the font path
	 */
#ifdef DEBUG
	{
		int nfonts;
		int i;
		char **fonts =	XGetFontPath(dpy, &nfonts);
		for (i = 0; i < nfonts; i++) {
			printf("\"%s\"\n", fonts[i]);
		}
		XFreeFontPath(fonts);
	}
#endif
	XtMainLoop();
	if (*TempFileName)
		(void) unlink(TempFileName);
	exit(0);
}

char *
GetFontMapString()
{
	return xtroff_resources.font_map_string;
}

/*ARGSUSED*/
void
RepaintCanvas(w, data, ev)
Widget w;
caddr_t data;
XEvent *ev;
{
	static rastered = 0;
	
	if (!isMapped)
		return;

	/* 
	 * Put the pixmap on the screen at the appropriate ViewLeft,
	 * ViewTop;
	 */
	if( !rastered && ev) {
		rastered = 1;
		Rerasterize(BaseFrame, (XEvent *) NULL, (String *) NULL,
		 (Cardinal *) NULL);
	}
	SetScrollBar();
	
#ifdef WINDOWDEBUG
	printf("repaint - ViewLeft = %d, Top = %d, Width = %d, Height = %d\n", 
	 ViewLeft, ViewTop, ViewWidth, ViewHeight);
#endif
	XCopyArea(dpy, PagePixRect, DrawingCanvas, gc, ViewLeft, ViewTop,
	 (unsigned int) ViewWidth, (unsigned int) ViewHeight, 0, 0);
	XFlush(dpy);
}

/*ARGSUSED*/
static void
RecordMapStatus(w, data, ev)
Widget w;
caddr_t data;
XEvent *ev;
{
	if (ev->type == MapNotify) {
#ifdef WINDOWDEBUG
		printf("window mapped\n");
#endif
		isMapped = TRUE;
		RepaintCanvas(CanvasWidget, (caddr_t) NULL, (XEvent *) NULL);
	} else if (ev->type = ConfigureNotify) {
		XConfigureEvent *cev = (XConfigureEvent *) ev;
#ifdef WINDOWDEBUG
		printf("window resized\n");
#endif
		if (cev->width != ViewWidth || cev->height != ViewHeight) {
			ViewWidth = cev->width;
			ViewHeight = cev->height;
			ResizeScrollBar();
		}
	}
}



/*ARGSUSED*/
static void
StartPan(w, event, params, nparams)
Widget w;
String *params;
Cardinal *nparams;
XMotionEvent	*event;
{
	OriginalX = event->x;
	OriginalY = event->y;
#ifdef DEBUG
	printf("starting pan from %d, %d\n", OriginalX, OriginalY);
#endif
	XDefineCursor(dpy, DrawingCanvas, HandCursor);
	PageRequest = 0;
}

/*ARGSUSED*/
static void
EndPan(w, event, params, nparams)
Widget w;
String *params;
Cardinal *nparams;
XMotionEvent	*event;
{
	if (OriginalX != -1 && OriginalY != -1){
		OriginalX = OriginalY = -1;
#ifdef DEBUG
		printf("ending pan\n");
#endif
		XDefineCursor(dpy, DrawingCanvas, WorkingCursor);
	}
	PageRequest = 0;
}

/*ARGSUSED*/
static void
PagePan(w, event, params, nparams)
Widget w;
String *params;
Cardinal *nparams;
XMotionEvent	*event;
{
	ViewLeft -= event->x - OriginalX;
	if (ViewLeft + ViewWidth > PAGE_PIXEL_WIDTH)
	        ViewLeft = PAGE_PIXEL_WIDTH - ViewWidth;
	if (ViewLeft < 0)
		ViewLeft = 0;

	ViewTop -= event->y - OriginalY;
	if (ViewTop + ViewHeight > PAGE_PIXEL_HEIGHT)
	        ViewTop = PAGE_PIXEL_HEIGHT - ViewHeight;
	if (ViewTop < 0)
		ViewTop = 0;
#ifdef DEBUG
	printf("panning - ViewLeft, Top = %d, %d\n", ViewLeft, ViewTop);
#endif
	if ((OriginalX != event->x) || (OriginalY != event->y)){
		XCopyArea(dpy, PagePixRect, DrawingCanvas, gc, ViewLeft,
		 ViewTop, (unsigned int) ViewWidth, (unsigned int) ViewHeight,
		 0, 0);
#ifdef DEBUG
		printf("panned\n");
#endif
		OriginalX = event->x;
		OriginalY = event->y;
		SetScrollBar();
	}
	PageRequest = 0;
}


/*ARGSUSED*/
static void
Number(w, event, params, nparams)
Widget w;
String *params;
Cardinal *nparams;
XEvent *event;
{
	char ch;
	
	if (XLookupString(&event->xkey, &ch, 1, (KeySym *) NULL,
	 (XComposeStatus *) NULL) != 1)
	    return;
	PageRequest = PageRequest*10 + ch - '0';
#ifdef WINDOWDEBUG
	printf("ch = %c, arg = %d\n", ch, PageRequest);
#endif
	
}

/*ARGSUSED*/
static void
EscapeNumber(w, event, params, nparams)
Widget w;
String *params;
Cardinal *nparams;
XEvent *event;
{
	PageRequest = 0;
#ifdef WINDOWDEBUG
	printf("arg = 0\n");
#endif
}


/*ARGSUSED*/
static void
GoToPage(w, event, params, nparams)
Widget w;
String *params;
Cardinal *nparams;
XEvent *event;
{
#ifdef WINDOWDEBUG
	printf("goto page %d\n", PageRequest);
#endif
	ViewTop = 0;
	CurrentPage = ShowPage(PageRequest);
	PageRequest = 0;
}

/*ARGSUSED*/
static void
BackPage(w, event, params, nparams)
Widget w;
String *params;
Cardinal *nparams;
XEvent *event;
{
	int curpage = CurrentPage;
	
	if (!PageRequest)
		PageRequest++;
	CurrentPage -= PageRequest;
	if (CurrentPage <= 0)
		CurrentPage = 1;
	if (CurrentPage != curpage) {
		ViewTop = 0;
		CurrentPage = ShowPage(CurrentPage);
	}
	PageRequest = 0;
}

/*ARGSUSED*/
static void
ForwardPage(w, event, params, nparams)
Widget w;
String *params;
Cardinal *nparams;
XEvent *event;
{
	if (!PageRequest)
		PageRequest++;
	CurrentPage += PageRequest;
	ViewTop = 0;
	CurrentPage = ShowPage(CurrentPage);
	PageRequest = 0;
}

/*ARGSUSED*/
static void
PrevSection(w, event, params, nparams)
Widget w;
String *params;
Cardinal *nparams;
XEvent *event;
{
	int curpage = CurrentPage;

	/* 
	 *  Back up to the previous section to be viewed. If
	 *  the screen is large enough to show the whole
	 *  page, then back up to the previous page. Else
	 *  back up to the previous half of the page. Only
	 *  change ViewTop - leave ViewLeft alone as
	 *  part of the philosophy - in general, the
	 *  width of the page fits nicely on the screen
	 */
	do {
		ViewTop -= ViewHeight;
		if (ViewTop <= -ViewHeight) {
			if (CurrentPage > 1) {
				ViewTop = PAGE_PIXEL_HEIGHT - ViewHeight;
				CurrentPage--;
			} else {
				CurrentPage = 1;
				ViewTop = 0;
				break;
			}
		} else if (ViewTop < 0) {
			ViewTop = 0;
		}
	} while (--PageRequest > 0);
	if (curpage != CurrentPage)
		CurrentPage = ShowPage(CurrentPage);
	else
		RefreshPage();
	PageRequest = 0;
}


/*ARGSUSED*/
static void
NextSection(w, event, params, nparams)
Widget w;
String *params;
Cardinal *nparams;
XEvent *event;
{
	int curpage = CurrentPage;

	/* 
	 *  Advance to the next section to be viewed. If
	 *  the screen is large enough to show the whole
	 *  page, then advance to the next page. Else
	 *  advance to the next half of the page. Only
	 *  change ViewTop - leave ViewLeft alone as
	 *  part of the philosophy - in general, the
	 *  width of the page fits nicely on the screen
	 */
	do {
		ViewTop += ViewHeight;
		if (ViewTop >= PAGE_PIXEL_HEIGHT) {
			ViewTop = 0;
			CurrentPage++;
		} else if (ViewTop > PAGE_PIXEL_HEIGHT - ViewHeight) {
			ViewTop = PAGE_PIXEL_HEIGHT - ViewHeight;
		}
	} while (--PageRequest > 0);
	if (curpage != CurrentPage)
		CurrentPage = ShowPage(CurrentPage);
	else
		RefreshPage();
	PageRequest = 0;
}

static void
SetPrinterCommand()
{
	char *buf;
	char *get_input();
	
	/* offer user a chance to change default printing command */
	if ((buf = get_input("Print command % ", PrinterCommand, FALSE))
	 == NULL)
		return;
	/* free default command and make new command from user the default */
	XtFree(PrinterCommand);
	PrinterCommand = buf;
}

/*ARGSUSED*/
static void
MousePrintPage(w, event, params, nparams)
Widget w;
String *params;
Cardinal *nparams;
XEvent *event;
{
	PrintPage(CurrentPage, (char *) NULL);
}

/*ARGSUSED*/
static void
MousePrintDocument(w, event, params, nparams)
Widget w;
String *params;
Cardinal *nparams;
XEvent *event;
{
	PrintDocument((char *) NULL, (char *) NULL);
	
}

/*ARGSUSED*/
static void
Quit(w, event, params, nparams)
Widget w;
String *params;
Cardinal *nparams;
XEvent *event;
{
	if (*TempFileName)
		(void) unlink(TempFileName);
	exit(0);
}


/* These are the scrolling and jumping (thumbing no longer supported)
   callbacks */
/*ARGSUSED*/
static void
Scrolled(w, closure, call_data)
Widget w;
caddr_t closure;
int call_data;
{
#ifdef WINDOWDEBUG
	printf( "scrolled by %d pixels.\n", call_data );
#endif
	if (w == VertScroll) {
		ViewTop += call_data * PAGE_PIXEL_HEIGHT / ViewHeight;
		if (ViewTop < 0)
			ViewTop = 0;
		else if (ViewTop > PAGE_PIXEL_HEIGHT - ViewHeight)
			ViewTop = PAGE_PIXEL_HEIGHT - ViewHeight;
	} else if (w == HorizScroll) {
		ViewLeft += call_data * PAGE_PIXEL_WIDTH / ViewWidth;
		if (ViewLeft < 0)
			ViewLeft = 0;
		else if (ViewLeft > PAGE_PIXEL_WIDTH - ViewWidth)
			ViewLeft = PAGE_PIXEL_WIDTH - ViewWidth;
	}
	
	XCopyArea(dpy, PagePixRect, DrawingCanvas, gc, ViewLeft, ViewTop,
	 (unsigned int) ViewWidth, (unsigned int) ViewHeight, 0, 0);
	SetScrollBar();
}


/*ARGSUSED*/
static void
Jumped(w, closure, percent)
Widget w;
caddr_t closure;
caddr_t percent;
{
	float top = *((float *) percent);
#ifdef WINDOWDEBUG
	printf( "thumbed to %f%%\n", top );
#endif
	if (w == VertScroll) {
		ViewTop = top * PAGE_PIXEL_HEIGHT;
		if (ViewTop > PAGE_PIXEL_HEIGHT - ViewHeight)
			ViewTop = PAGE_PIXEL_HEIGHT - ViewHeight;
	} else if (w == HorizScroll) {
		ViewLeft = top * PAGE_PIXEL_WIDTH;
		if (ViewLeft > PAGE_PIXEL_WIDTH - ViewWidth)
			ViewLeft = PAGE_PIXEL_WIDTH - ViewWidth;
	}
	XCopyArea(dpy, PagePixRect, DrawingCanvas, gc, ViewLeft, ViewTop,
	 (unsigned int) ViewWidth, (unsigned int) ViewHeight, 0, 0);
	SetScrollBar();
}


SetScrollBar()
{
	if (!xtroff_resources.scrollbars)
		return;
	XtScrollBarSetThumb(VertScroll, 
	 (float) ((float) ViewTop / (float) PAGE_PIXEL_HEIGHT),
	 (float) ((float) ViewHeight / (float) PAGE_PIXEL_HEIGHT));
	XtScrollBarSetThumb(HorizScroll, 
	 (float) ((float) ViewLeft / (float) PAGE_PIXEL_WIDTH),
	 (float) ((float) ViewWidth / (float) PAGE_PIXEL_WIDTH));
}

ResizeScrollBar()
{
	Dimension tmp;
	if (!xtroff_resources.scrollbars)
		return;
	tmp = (Dimension) ViewHeight;
	XtSetArg(scroll_args[0], XtNlength, tmp);
	XtSetValues(VertScroll, scroll_args, 1);
	tmp = (Dimension) ViewWidth;
	XtSetArg(scroll_args[0], XtNlength, tmp);
	XtSetValues(HorizScroll, scroll_args, 1);
	SetScrollBar();
}


/*ARGSUSED*/
static void
SearchForward(w, event, params, nparams)
Widget w;
String *params;
Cardinal *nparams;
XEvent *event;
{
	int	i;
	char *tmpstr;
	char *get_input();
	
	tmpstr = get_input(": search-forward ? ", SearchItem, FALSE);
	if (!tmpstr)
	    return;
	if (SearchItem)
	    free(SearchItem);
	SearchItem = tmpstr;
	i = SearchFile(SearchItem,CurrentPage,1);
	if (i){
		CurrentPage = i;
		ShowPage(i);
		message("Found!");
	} else {
		SetTitleBar("Displaying",CurrentPage);
		message("Not found!");
	}
	return;
}

/*ARGSUSED*/
static void
SearchBackward(w, event, params, nparams)
Widget w;
String *params;
Cardinal *nparams;
XEvent *event;
{
	int	i;
	char *tmpstr;
	char *get_input();
	
	tmpstr = get_input(": search-backward ? ", SearchItem, FALSE);
	if (!tmpstr)
	    return;
	if (SearchItem)
	    free(SearchItem);
	SearchItem = tmpstr;
	i = SearchFile(SearchItem,CurrentPage,-1);
	if (i){
		CurrentPage = i;
		ShowPage(i);
		message("Found!");
	} else {
		SetTitleBar("Displaying",CurrentPage);
		message("Not found!");
	}
	return;
}

/*
 * This has some really Unix specific code. But then again, so does ditroff...
 */
/*ARGSUSED*/
static void
Rerasterize(w, event, params, nparams)
Widget w;
String *params;
Cardinal *nparams;
XEvent *event;
{
#ifndef STANDALONE
	static	FILE	*FilePointer = 0;
	static	FILE	*PipePointer = 0;
	static	int	ProcessPid = 0;
	int	fd;
	extern char *mktemp();

	if (FilePointer && FilePointer != stdin){
	        (void) fclose(FilePointer);
	}
	if (PipePointer){
		(void) fclose(PipePointer);
		(void) kill(ProcessPid, SIGKILL);
		(void) wait((union wait *) 0);
	}
	ProcessPid = 0;
	PipePointer = FilePointer = 0;
			
	if (!CommandMode && FileName != NULL && FileName[0] != '\0'){
		FilePointer = fopen(FileName,"r");
		if (!FilePointer){
			warning("Can't open %s for reading input.\n",
				FileName);
		} else {
			ActualFileName = FileName;
			InitializeFile(FilePointer, FilePointer);
		}
	} else {
		static char FileName[MAXSTR], *p;
		int	PipeFds[2];

		if (CommandMode) {
			p = CommandString;
			while (isspace(*p))
				p++;
			if (!*p){
				warning("Rasterize called without a command.\nUse the menu to set a command.");
				return;
			}
		}

		(void) pipe(PipeFds);		/* Get two halves of pipe */
		
		ProcessPid = fork();		/* And then Fork */
		if (ProcessPid == -1) {
			warning("Couldn't fork. Help\n");
			return;
		}
		if (!ProcessPid){		/* First the Child */
			int	y;

			/*
			 * reopen stdin as /dev/null just to make sure xtroff
			 * doesn't hang waiting if the user was silly and
			 * forgot to specify a filename, so the command
			 * expects input from stdin.
			 */
			(void) close(0);
			y = open("/dev/null", O_RDONLY, 0);
			if (y == -1) {
				(void) fprintf(stderr,
				 "Someone stole /dev/null\n");
				/* not fatal */
			}
			(void) close(1);
			/* make stdout the pipe to the parent */
			if (dup2(PipeFds[1], 1) == -1) {
				(void) fprintf(stderr, "dup2 failed\n");
				exit(1);
			}
			for( y = 3; y < getdtablesize(); y++)
				(void) close(y);

			if (!CommandMode) {
				(void) execl("/bin/cat", (char *) 0);
				(void) fprintf(stderr,
				 "Someone stole /bin/cat - help!\n");
				exit(1);
			} else {
				system(p);
				exit(0);
			}
			/*NOTREACHED*/
		}
	
		PipePointer = fdopen(PipeFds[0], "r");
		(void) close(PipeFds[1]);
		
		if (*TempFileName) {
		    (void) strcpy(FileName, TempFileName);
		} else {
		    (void) strcpy(FileName,"/tmp/suntroff.XXXXXX");
		    (void) mktemp(FileName);
		    (void) strcpy(TempFileName, FileName);
		}

		fd = open(FileName, O_RDWR|O_CREAT|O_TRUNC,0644);
		if (fd == -1) {
			(void) fprintf(stderr,
			 "Can't open buffer file for the command:\n");
			(void) fprintf(stderr, "\t%s\n", CommandString);
			exit(1);
		}
		
		FilePointer = fdopen(fd, "r+");
		
		if (!FilePointer){
			(void) fprintf(stderr,
			    "Can't open buffer file for the command:\n");
			(void) fprintf(stderr,"\t%s\n", CommandString);
			exit(1);
		}
		ActualFileName = FileName;
		InitializeFile(PipePointer, FilePointer);
	}

	ShowPage(CurrentPage);
#endif STANDALONE
}

/*
 *  For those without a window manager that puts titlebars, maybe we
 *  should put a Label widget
 */
SetTitleBar(status,PageNumber)
char	*status;
int	PageNumber;
{
	extern char	*DefaultTitle;
	char	Buffer[MAXSTR];
	char	TempStatus[MAXSTR];
	int	FrameWidth, Width, i;

	/* No way of getting this from the window manager, is there? */
	FrameWidth = 60; 

	if (PageNumber >= 0)
		(void) sprintf(TempStatus,"%s Page %d",status,PageNumber);
	else
		(void) sprintf(TempStatus, "%s", status);

	strncpy(Buffer,DefaultTitle,MAXSTR);
	i = strlen(DefaultTitle);
	
	Width = strlen(TempStatus);

	while (i + Width < FrameWidth){
		Buffer[i++] = ' ';
	}
	if (FrameWidth - i > 0){
		strncpy(&Buffer[i],TempStatus,FrameWidth-i);
	}
	XStoreName(dpy, OuterFrame, Buffer);
#if 0
	if (PageNumber >= 0){
		(void) sprintf(PageNumberValue,"%d",PageNumber);
		panel_set_value(PageItem,PageNumberValue);
	}
#endif
	if (STREQ(status, "Displaying"))
		XDefineCursor(dpy, DrawingCanvas, WorkingCursor);
	else
		XDefineCursor(dpy, DrawingCanvas, WaitCursor);
	XFlush(dpy);
}

static char *SavedTitleBar = NULL;

SaveTitleBar(){
	XFetchName(dpy, OuterFrame, &SavedTitleBar);
	XFlush(dpy);
}

RestoreTitleBar(){
	if (SavedTitleBar) {
		XStoreName(dpy, OuterFrame, SavedTitleBar);
		XFlush(dpy);
		XDefineCursor(dpy, DrawingCanvas, WorkingCursor);
	}
}

#ifndef lint
#include <varargs.h>

/*VARARGS0*/
fatal(va_alist)
va_dcl
{
	va_list args;
	char *fmt;

	va_start(args);
	fprintf(stderr,"Fatal Error: ");
	fmt = va_arg(args, char *);
	(void) vfprintf(stderr,fmt,args);
	va_end(args);
	if (*TempFileName)
		(void) unlink(TempFileName);
	exit(1);
}

/*VARARGS0*/
warning(va_alist)
va_dcl
{
	va_list args;
	char *fmt;
	char	Message[MAXSTR];

	va_start(args);
	fmt = va_arg(args, char *);
	(void) vsprintf(Message,fmt,args);
	va_end(args);
	message(Message);
}
#else
/* lint is such a pain with varargs functions */
/*ARGSUSED VARARGS1*/ fatal(fmt) char *fmt; {}
/*ARGSUSED VARARGS1*/ warning(fmt) char *fmt; {}
#endif

/*
 *  We move the menu to the current mouse location before popping it up
 *  with MenuPopup. Pity MenuPopup doesn't do this!
 */
/*ARGSUSED*/
static void
SetMenuPos (w, clientdata, calldata)
Widget w;
caddr_t clientdata;
caddr_t calldata;
{
	Window root, child;
	int rootx, rooty, winx, winy;
	unsigned int mask;
	
	XQueryPointer(XtDisplay(w), XtWindow(w), &root, &child, 
	 &rootx, &rooty, &winx, &winy, &mask);
	/*
	 *  The -OFFSET,-OFFSET displacement just makes sure mouse is in
	 *  the menu
	 */
#define OFFSET 10
	XtMoveWidget(PopupShell, (Position) (rootx - OFFSET),
	 (Position) rooty - OFFSET);
}


/*ARGSUSED*/
static void
MenuSelection (widget, closure, call_data)
Widget	widget;
caddr_t	closure, call_data;
{
	char *tmpstr;
	char *get_input();
	char statusbuf[128];
	int menuitem = (int) closure;
#ifdef R4
	Arg  arg[1];

	XtSetArg (arg[0], XtNpopupOnEntry, widget);
	XtSetValues (XtParent(widget), arg, (Cardinal) 1);
#endif

	XtPopdown(PopupShell);
	XFlush(dpy);
	switch (menuitem)
	{
		case 1:
			Rerasterize(BaseFrame, (XEvent *) NULL,
			 (String *) NULL, (Cardinal *) NULL);
			break;
		case 2:
			ForwardPage(BaseFrame, (XEvent *) NULL,
			 (String *) NULL, (Cardinal *) NULL);
			break;
		case 3:
			BackPage(BaseFrame, (XEvent *) NULL,
			 (String *) NULL, (Cardinal *) NULL);
			break;
		case 4:
			NextSection(BaseFrame, (XEvent *) NULL,
			 (String *) NULL, (Cardinal *) NULL);
			break;
		case 5:
			PrevSection(BaseFrame, (XEvent *) NULL,
			 (String *) NULL, (Cardinal *) NULL);
			break;
		case 6:
			if (CommandMode) {
				(void) sprintf(statusbuf, "Command is \"%s\"", 
				 CommandString);
			} else {
				(void) sprintf(statusbuf, "File is \"%s\"",
				 FileName);
			}
			message(statusbuf);
			break;
		case 7:
			SearchForward(BaseFrame, (XEvent *) NULL,
				       (String *) NULL, (Cardinal *) NULL);
			break;
		case 8:
			SearchBackward(BaseFrame, (XEvent *) NULL,
				       (String *) NULL, (Cardinal *) NULL);
			break;
		case 9:
			/* Change File */
			tmpstr = get_input(": file-name ? ", FileName, TRUE);
			if (tmpstr) {
				CommandMode = 0;
				strcpy(FileName, tmpstr);
				free(tmpstr);
				Rerasterize(BaseFrame, (XEvent *) NULL,
				 (String *) NULL, (Cardinal *) NULL);
			}
			break;
		case 10:
			/* Change Command */
			tmpstr = get_input(": formatting-command ? ",
			 CommandString, FALSE);
			if (tmpstr) {
				int len = strlen(tmpstr);

				if (len > MAXSTR) {
					message("Command too long!");
					free(tmpstr);
					break;
				}
				CommandMode = 1;
				strcpy(CommandString, tmpstr);
				free(tmpstr);
				Rerasterize(BaseFrame, (XEvent *) NULL,
				 (String *) NULL, (Cardinal *) NULL);
			}
			break;
		case 11:
			/* Go to page */
			tmpstr = get_input(": go-to-page ? ", (char *) NULL,
			 FALSE);
			if (tmpstr) {
				PageRequest = atoi(tmpstr);
				GoToPage(BaseFrame, (XEvent *) NULL,
				 (String *) NULL, (Cardinal *) NULL);
			}
			break;
		case 12:
			/* Print page */
			MousePrintPage(BaseFrame, (XEvent *) NULL,
			 (String *) NULL, (Cardinal *) NULL);
			break;
		case 13:
			/* Print document */
			MousePrintDocument(BaseFrame, (XEvent *) NULL,
			 (String *) NULL, (Cardinal *) NULL);
			break;
		case 14:
			/* Set Printer name */
			SetPrinterCommand();
			break;
		case 0:
			Quit(BaseFrame, (XEvent *) NULL,
			 (String *) NULL, (Cardinal *) NULL);
			break;
		default:
			message("Programmer error: unknown menu selection");
			break;
	}
}


static char *input;
static int  inputdone = TRUE;

/*
 *  This routine focuses all input on the InputWindow, and starts up a
 *  loop which forms a secondary dispatcher, ignoring all events except
 *  ExposeWindow events on windows other than the InputWindow. This
 *  effectively forces the user to use the InputWindow. The loop
 *  terminates when input is completed, either by inputting a string, in
 *  which case finished_input will be invoked, or by aborting.  Both set
 *  the completion flag and the loop quits, focussing input back to the
 *  previous holder. No harm is done to this application if focus is not
 *  given to the minibuffer, or removed from it - it's just more
 *  convenient for the user who doesn't have to move a mouse around too
 *  much.  The routine returns the input string, or a NULL if the input
 *  was aborted.
 */
char *
get_input(prompt, default_string, complete)
char *prompt, *default_string;
{
	XEvent ev;

	XDefineCursor(dpy, DrawingCanvas, TextCursor);
	MinibufGetInput(InputWidget, prompt, default_string, complete);
	XtAddGrab(InputWidget, False, False);
	XtSetKeyboardFocus(BaseFrame, InputWidget);
/*	XtSetKeyboardFocus(CanvasWidget, InputWidget);*/
	inputdone = FALSE;
	while (!inputdone) {
		XtNextEvent(&ev);
		(void) XtDispatchEvent(&ev);
	}
	XtSetKeyboardFocus(BaseFrame, (Widget) None);
	XtRemoveGrab(InputWidget);
	XDefineCursor(dpy, DrawingCanvas, WorkingCursor);
	return(input);
}


/*
 *  Callback, invoked when user hits RETURN in the InputWidget (or whatever
 *  the user has bound 'newline' to) in which case inp_string points to
 *  an alloc'ed string. If the user aborts input (^G, ^C) then
 *  inp_string is NULL
 */
/*ARGSUSED*/
static void
finished_input(w, tag, inp_string)
Window w;
caddr_t tag;
char *inp_string;
{
	input = inp_string;
	inputdone = TRUE;
}


/* 
 *  Asks to confirm something - If they reply "y", returns YES, if they
 *  reply "n", returns NO, if they abort, returns ABORT. You are given
 *  IMPATIENCE tries to answer
 */
confirm(query, default_answer)
char *default_answer;
char *query;
{
	char *answer;
	char c;
	char *mesg = "Answer yes or no, please (y/n)";
	int count = 0;
#define IMPATIENCE	5

	do {
		if ((answer = get_input(query, default_answer, FALSE)) == NULL)
			return (ABORT);
		c = (isupper(answer[0])) ? tolower(answer[0]) : answer[0];
		if (c == 'y')
			return(YES);
		else if (c == 'n')
			return(NO);
		message(mesg);
		/* Let's get really explicit next time */
		mesg = "Type 'y' and RETURN if you want to answer YES, 'n' for NO, CTRL-G to abort";
	} while (count++ < IMPATIENCE);
	message("Forget it - Aborting");
	return(ABORT);
}


/*
 *  Message is printed on the communication line if the windows are
 *  mapped, otherwise fprintf'ed to stderr
 */
message(s)
char *s;
{
	if (InputWidget != 0)
		MinibufDisplayMessage(InputWidget, s, TRUE);
	else
		fprintf(stderr, "%s\n", s);
}

/*ARGSUSED*/
SetPrinter(s)
char *s;
{
}

#ifdef R4
static Widget
CreateMenu(name, parent, menu_list)
char *name;
Widget parent;
MenuItemsList *menu_list;
{
	Widget w;
	int i;
	
	w = XtCreatePopupShell (name, simpleMenuWidgetClass,
					 parent, NULL, 0);
	for (i = 0; menu_list[i].text != NULL; i++) {
		XtAddCallback(XtCreateManagedWidget(menu_list[i].text,
						    smeBSBObjectClass, w,
						    NULL, (Cardinal) 0),
			      XtNcallback, menu_list[i].proc,
			      menu_list[i].closure);
	}
	return w;
}
#else /* ! R4 */
/* Use the Menu package in XtStuff */
static Widget
CreateMenu(name, parent, menu_list)
char *name;
Widget parent;
MenuItemsList *menu_list;
{
	XtTranslations menuTranslations;
	Widget w;
	char *mname;
	/* We want the menu to go away when the button comes up */
	static String menuTransTbl = "<Btn3Up>:	MenuPopdown(popupShell)";
	static Arg menu_args[] = {
		{XtNtranslations, 0},
		{XtNmenuItemsList, 0},
	};

#define MENUSTR ".menu"
	mname = XtMalloc((unsigned) (strlen(name) + sizeof(MENUSTR)));
	(void) strcpy(mname, name);
	(void) strcat(mname, MENUSTR);
#undef MENUSTR
	
	/* 
	 *  make a menu or button box - ShowStatus, ReRasterize, Print,
	 *  Print Page, previous Page, Next Page, Quit
	 */
	menuTranslations = XtParseTranslationTable(menuTransTbl);
	menu_args[0].value = (XtArgVal) menuTranslations;
	menu_args[1].value = (XtArgVal) menu_list;
	w = XtCreatePopupShell (name, overrideShellWidgetClass,
					parent, (ArgList) menu_args, 1);
	(void) XtCreateManagedWidget (mname, menuWidgetClass, w,
					(ArgList) menu_args,
					XtNumber (menu_args));
	return w;
}
#endif /* R4 */

#ifdef	STANDALONE
ShowPage(){
	printf("Show Page called.\n");
}

char	*DefaultTitle = "Standalone Window Code";

SearchFile(){
	printf("Search file called.\n");
}

RefreshPage(){
}

#endif STANDALONE
#endif SUNTOOLS
/* Don't put anything after this line */
