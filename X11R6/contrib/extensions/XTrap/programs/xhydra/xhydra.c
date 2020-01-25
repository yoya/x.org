/****************************************************************************

Copyright (c) 1994  Robert Chesler, Absol-Puter

Note:
Motif GUI courtesy Jem Treadwell of Concurrent Computer Corporation.
Thanks to Ray Tice of Concurrent Computer Corporation for his help with
tripwires and InputOnly windows.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

****************************************************************************/

/********* Compile-time configuration options *********/
#define XTEST
#define XTRAP
/*#define DEBUG			/* various printfs */

/* tripwires still under construction and GUI reconfiguration doesn't work */
#define HYPER   		/* do "hyperspace" tripwire display hopping */

#define RTX_NOT_NECESSARY	/* try to work without RealTimeX */
#define XTRAP_NOT_NECESSARY	/* try to work without XTrap */


#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define NEED_EVENTS
#define NEED_REPLIES

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xresource.h>
#include <X11/Xatom.h>

#ifdef RTX
#include <X11/Xrtlib.h>
#endif /* RTX */

#include <X11/Xos.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>

#ifdef XTRAP
#include <X11/extensions/xtraplib.h>
#include <X11/extensions/xtraplibp.h>
#endif /* XTRAP */

#ifdef XTEST
#include <X11/extensions/XTest.h>
#include <X11/extensions/Xext.h>
#endif /* XTEST */

#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include <Xm/ToggleB.h>
#include <Xm/PushB.h>
#include <Xm/BulletinB.h>
#include <Xm/RowColumn.h>
#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/Text.h>
#include <Xm/Separator.h>
#include <Xm/DialogS.h>

#define MAX_DISPLAYS 32

#define WtSTRING(x)	    XmStringCreate(x,XmSTRING_DEFAULT_CHARSET)
#define WtSTRING_L_TO_R(x)  XmStringCreateLtoR(x,XmSTRING_DEFAULT_CHARSET)
#define WtGETSTRING(x,y)    XmStringGetLtoR(x,XmSTRING_DEFAULT_CHARSET,y)
#define WtMAX(a,b) (a > b ? a : b)
#define WtFLAG_IS_ON(field,flags) ((field & (flags)) != 0)
#define WtIS_SAME(a,b) (strcmp(a,b) == 0)
#define WtIN_RANGE(n,min,max) ((n >= min) && (n <= max))

static char		*ProgramName,*DisplayName[MAX_DISPLAYS];
static XtAppContext	AppContext;
static Widget		TopLevel[MAX_DISPLAYS];
static Widget		RadioBox[MAX_DISPLAYS];
static Widget		Toggles[MAX_DISPLAYS][MAX_DISPLAYS];
static Widget		TripToggle[4];
static Widget		WarpToggle[MAX_DISPLAYS+1],SelectedWarpToggle;
static Widget		AddText;
static Atom		DeleteWindowAtom[MAX_DISPLAYS];
static Display		*DisplayId[MAX_DISPLAYS];
static int		NumDisplays = 0;
static int		SelectedTripSide;
static int		Connection[MAX_DISPLAYS];
static int		CurrentDisplay = -1;

static void	AddDisplay();
static void	ParseTripOptions();
static void	ToggleBtnCB();
static void	QuitBtnCB();
static void	TripBtnCB();
static void	TripDialogBtnCB();
static void	TripToggleCB();
static void	WarpToggleCB();
void	RegisterProtocolHandlers();
Widget	WtDialogCreate();
Widget	WtDialogAddButtons();
Widget	WtLabel();
void	WtLabelSetValue();
Boolean WtIsBlank();
Boolean WtIsNumeric();
Widget  WtNearestShellWidget();

/*==============================================*
 *						*
 *	       DIALOG BOX CONTROL		*
 *						*
 *==============================================*/

/*
 * The dialog buttons are laid out in the order specified
 * by the OSF/Motif Certification Checklist.
 */
enum
{
    WtDIALOG_BTN_YES,
    WtDIALOG_BTN_NO,
    WtDIALOG_BTN_OK,
    WtDIALOG_BTN_APPLY,
    WtDIALOG_BTN_RETRY,
    WtDIALOG_BTN_STOP,
    WtDIALOG_BTN_TEXT,
    WtDIALOG_BTN_RESET,
    WtDIALOG_BTN_CANCEL,
    WtDIALOG_BTN_CLOSE,	    /* Last left button	  */
    WtDIALOG_BTN_HELP	    /* First right button */
};

#define WtDIALOG_FLG_YES	1 << WtDIALOG_BTN_YES
#define WtDIALOG_FLG_NO		1 << WtDIALOG_BTN_NO
#define WtDIALOG_FLG_OK		1 << WtDIALOG_BTN_OK
#define WtDIALOG_FLG_APPLY	1 << WtDIALOG_BTN_APPLY
#define WtDIALOG_FLG_RETRY	1 << WtDIALOG_BTN_RETRY
#define WtDIALOG_FLG_STOP	1 << WtDIALOG_BTN_STOP
#define WtDIALOG_FLG_TEXT	1 << WtDIALOG_BTN_TEXT
#define WtDIALOG_FLG_RESET	1 << WtDIALOG_BTN_RESET
#define WtDIALOG_FLG_CANCEL	1 << WtDIALOG_BTN_CANCEL
#define WtDIALOG_FLG_CLOSE	1 << WtDIALOG_BTN_CLOSE
#define WtDIALOG_FLG_HELP	1 << WtDIALOG_BTN_HELP

#define WtDIALOG_FLG_MODAL	1 << 16
#define WtDIALOG_FLG_NODEFPOS	1 << 17

#define WtDIALOG_MAX_LEFT_BTN	WtDIALOG_BTN_CLOSE+1
#define WtDIALOG_MAX_RIGHT_BTN	1
#define WtDIALOG_MAX_BTN	(WtDIALOG_MAX_LEFT_BTN + WtDIALOG_MAX_RIGHT_BTN)
#define WtDIALOG_BUTTON_MARGIN	6

typedef struct
{
    char	*title;
    void	(*callback)();	/* Callback routine			     */
    int		cb_data;	/* Callback data - button number	     */
				/* is masked into l.s. four bits	     */
    int		def_btn;	/* Default btn (-1 = none)		     */
    int		flags;
    char	*ok_label;	/* Label for OK button (OK if NULL or blank) */
    char	*text_label;	/* Label for Text button		     */
    Widget	shell_w;
    Widget	bb_w;		/* Return - BulletinBoard id		     */
    Widget	rc_w;		/* Return - RowColumn id		     */
    Widget	button_w[WtDIALOG_MAX_BTN]; /* Widget-ids of all buttons     */
} WtDialogSpec_t;

static WtDialogSpec_t TripDialogSpec =	
{
    " Assign Tripwires ", TripDialogBtnCB, NULL, WtDIALOG_BTN_OK,
	WtDIALOG_FLG_NODEFPOS | WtDIALOG_FLG_OK , "OK", "",
};

/***/
Display *control_dpy;
Display *remote_dpy;
Window myWin;
short screen;
short rem_screen;
short lastx, lasty;

XSizeHints hints;
int off_x=0;
int off_y=0;
int off_x_max=0;
int off_y_max=0;
int dpy_w=0;
int dpy_h=0;
int min_x=0, max_x=0;
int min_y=0, max_y=0;

static int last_client_data = -1;

#define dir_left   0
#define dir_right  1
#define dir_top    2
#define dir_bottom 3
#define SIDES      4

Bool hyper_symetric=True;		/* make all mappings symmetric */
char opposide[SIDES]={1,0,3,2};		/* could twiddle lsb */

/*==============================================*
 *						*
 *	     RESOURCES AND OPTIONS              *
 *						*
 *==============================================*/

typedef struct
{
    Boolean	horizontal;	/* Horizontal display mode 		      */
    char	*trip_side[SIDES]; /* Tripwire assignment strings             */
    int		trip_thickness;	/* Thickness of sensitized regions (resource) */
} AppDefaults_t, *AppDefaults_p;

AppDefaults_t	AppDefaults; 
AppDefaults_t	*Ap = &AppDefaults;

static XtResource Resources[] =
{
    {
	"optHorizontal",
	"OptHorizontal",
	XtRBoolean, sizeof(Boolean),
	XtOffset(AppDefaults_p,horizontal),
	XtRString, "False",
    },
    {
	"optTripLeft",
	"OptTripLeft",
	XtRString, sizeof(String),
	XtOffset(AppDefaults_p,trip_side[0]),
	XtRString, "",
    },
    {
	"optTripRight",
	"OptTripRight",
	XtRString, sizeof(String),
	XtOffset(AppDefaults_p,trip_side[1]),
	XtRString, "",
    },
    {
	"optTripTop",
	"OptTripTop",
	XtRString, sizeof(String),
	XtOffset(AppDefaults_p,trip_side[2]),
	XtRString, "",
    },
    {
	"optTripBottom",
	"OptTripBottom",
	XtRString, sizeof(String),
	XtOffset(AppDefaults_p,trip_side[3]),
	XtRString, "",
    },
    {
	"xhTripThickness",
	"XhTripThickness",
	XtRInt, sizeof(int),
	XtOffset(AppDefaults_p,trip_thickness),
	XtRString, "5",
    },
};

static XrmOptionDescRec Options[] =
{
    {"-horizontal","*optHorizontal",	XrmoptionNoArg,	    "True" },
    {"-tripleft",  "*optTripLeft",	XrmoptionSepArg,    ""     },
    {"-tripright", "*optTripRight",	XrmoptionSepArg,    ""     },
    {"-triptop",   "*optTripTop",	XrmoptionSepArg,    ""     },
    {"-tripbottom","*optTripBottom",	XrmoptionSepArg,    ""     },
};


struct hyper
{
    Display *dpy;	/* need to get Display * from display number */
    int dpy_w;		/* display width & height */
    int dpy_h;		/* for trip sizes and for warp placement */

    short map[SIDES];	/* map of edge to display - per display */
    Window win[SIDES];	/* the tripwire windows */
}
hyper[MAX_DISPLAYS];

#ifdef HYPER
/* call for each display before any mappings */
void
init_hyper(disp,dpy)
    int disp;
    Display *dpy;
{
    int screen = DefaultScreen(dpy);

#ifdef DEBUG
    printf("init_hyper(%d,%s)\n",disp,DisplayString(dpy));
#endif

    hyper[disp].dpy = dpy;
    hyper[disp].dpy_w=DisplayWidth(dpy, screen);
    hyper[disp].dpy_h=DisplayHeight(dpy, screen);

    hyper[disp].map[dir_left] = hyper[disp].map[dir_right] = 
    hyper[disp].map[dir_top] = hyper[disp].map[dir_bottom] = -1;

    hyper[disp].win[dir_left] = hyper[disp].win[dir_right] =
    hyper[disp].win[dir_top] = hyper[disp].win[dir_bottom] = (Window)0;
}

/* call for each tripwire mapping */
void
hyper_map(src,dst,side)
    int src,dst;
    int side;
{
#ifdef DEBUG
    printf("hyper_map(%d,%d,%d)\n",src,dst,side);
#endif

    hyper[src].map[side]=dst;
    if (hyper_symetric)
    {
	side = opposide[side];
#ifdef DEBUG
	printf("symmetric(%d,%d,%d)\n",src,dst,side);
#endif
	hyper[dst].map[side]=src;
    }
}

#define hyper_set_size_position(side,hints)			\
{								\
    switch(side)		/* size */			\
    {								\
	case dir_left:						\
	case dir_right:						\
	    hints.width = Ap->trip_thickness;			\
	    hints.height = dpy_h-2;				\
	    break;						\
								\
	case dir_top:						\
	case dir_bottom:					\
	    hints.width = dpy_w-2;				\
	    hints.height = Ap->trip_thickness;			\
	    break;						\
    }								\
    switch(side)		/* position */			\
    {								\
	case dir_left:						\
	case dir_top:						\
	    hints.x = hints.y = 0;				\
	    break;						\
								\
	case dir_right:						\
	    hints.x = dpy_w - Ap->trip_thickness - 2;		\
	    hints.y = 0;					\
	    break;						\
								\
	case dir_bottom:					\
	    hints.x = 0;					\
	    hints.y = dpy_h - Ap->trip_thickness - 2;		\
	    break;						\
    }								\
}

/* call for each display to create needed tripwire window */
void
realize_hyper(disp)
    int disp;
{
    Display *dpy=hyper[disp].dpy;
    int dpy_w=hyper[disp].dpy_w;
    int dpy_h=hyper[disp].dpy_h;
    Window root=RootWindow(dpy,screen);
    int side;

    XSizeHints hints;
    XSetWindowAttributes xswa;

#ifdef DEBUG
    printf("realize_hyper(%s)\n",DisplayString(dpy));
#endif

    xswa.override_redirect = True;
    xswa.event_mask = EnterWindowMask|LeaveWindowMask;

    hints.flags = USPosition|USSize;

    for (side=dir_left; side<SIDES; side++)
    {
	if (hyper[disp].map[side] >=0 )
	{
	    hyper_set_size_position(side,hints);
	    hyper[disp].win[side]=XCreateWindow(dpy,root,
		hints.x, hints.y, hints.width, hints.height, 0, 0, InputOnly,
		CopyFromParent, CWOverrideRedirect|CWEventMask, &xswa);
	}
	else if (hyper[disp].win[side])
	{
	    /* first destroy window -- ZZZ */
	    hyper[disp].win[side]=(Window)0;
	}
    }
    XFlush(hyper[disp].dpy);
}

/* call when entering a display to map all its tripwire windows */
void
map_hyper(disp)
    int disp;
{
    int side;

#ifdef DEBUG
    printf("map_hyper(%d)\n",disp);
#endif

    for (side=dir_left; side<SIDES; side++)
	if ((hyper[disp].map[side] >= 0) && (hyper[disp].win[side]))
	{
	    XMapWindow(hyper[disp].dpy,hyper[disp].win[side]);
#ifdef DEBUG
	    printf("XMapWindow for side %d\n",side);
#endif
	}
    XFlush(hyper[disp].dpy);
}

/* call when leaving a display to unmap all its tripwire windows */
void
unmap_hyper(disp)
    int disp;
{
    int side;

#ifdef DEBUG
    printf("unmap_hyper(%d)\n",disp);
#endif
    
    for (side=dir_left; side<SIDES; side++)
	if ((hyper[disp].map[side] >= 0) && (hyper[disp].win[side]))
	{
	    XUnmapWindow(hyper[disp].dpy,hyper[disp].win[side]);
#ifdef DEBUG
	    printf("XUnmapWindow for side %d\n",side);
#endif
	}
    XFlush(hyper[disp].dpy);
}

#define hyper_warp_place(side,src,dst,x,y)	/* resets x & y */	\
switch(side)								\
{									\
    case dir_left:							\
	x=hyper[dst].dpy_w - x;						\
	break;								\
    case dir_right:							\
	x=hyper[dst].dpy_w - hyper[src].dpy_w + x;			\
	break;								\
    case dir_top:							\
	y=hyper[dst].dpy_h - y;						\
	break;								\
    case dir_bottom:							\
	y=hyper[dst].dpy_h - hyper[src].dpy_h + y;			\
	break;								\
}
#endif /* HYPER */

#ifdef XTRAP
int tcs_connection[MAX_DISPLAYS];
XETC *tcs[MAX_DISPLAYS];
XETC *tc;
#endif /* XTRAP */
#ifdef RTX
Bool rtx[MAX_DISPLAYS];
#endif /* RTX */

void
do_ext_init(dpy,disno)
    Display *dpy;
    int disno;
{
#ifdef XTRAP
    XETC *tc = XECreateTC(dpy,0L,NULL);
	
    if (tc==NULL)
    {
#ifdef DEBUG
	printf("could not initialize XTrap extension on display %s\n",
	    DisplayString(dpy));
#endif
	return;
    }

    tcs_connection[disno] = ConnectionNumber(dpy);
    tcs[ConnectionNumber(dpy)] = tc;
#endif /* XTRAP */

#ifdef XTEST
    if (XTestGrabControl(dpy, True))
#ifdef DEBUG
	printf("grab true\n");
    else
	printf("grab false\n")
#endif
	;
#endif /* XTEST */

#ifdef XTRAP
#ifdef RTX_NOT_NECESSARY
    /* if no rtx on this display then revector grab servers ZZZ */

#ifdef RTX
    /* use RTX grab preemption here */
    if (!(rtx[ConnectionNumber(dpy)]))
#endif /* RTX */
    {
	/* 
         * In order to ignore GrabServer's we must configure at least one 
         * trap.  Let's make it X_GrabServer.  We don't have to receive
         * a callback, though.
         */
        ReqFlags requests;

#ifdef DEBUG
	printf("using XTrap to disable server grabs on display %s\n",
	    DisplayString(dpy));
#endif

        (void)memset(requests,0L,sizeof(requests));
        BitTrue(requests, X_GrabServer);
        XETrapSetRequests(tc, True, requests);
        (void)XETrapSetGrabServer(tc, True);
	(void)XEStartTrapRequest(tc);
    }
#endif /* RTX_NOT_NECESSARY */
#endif /* XTRAP */
}

void
do_sim_input(ev)
    XEvent	*ev;
{
    short type=ev->type;
    int x, y;

    if (!remote_dpy)
	return;

    /* NOTE:  I wish there were a better way to handle XTRAP or XTEST
     *		without risking duplicate case values... --RTC */
    switch(type)
    {
	case KeyPress:
#ifdef XTRAP
	    if (tc)
		XESimulateXEventRequest(tc, type,
		    ev->xkey.keycode,
		    lastx, lasty, rem_screen);
	    else
#endif /* XTRAP */
#ifdef XTEST
	    if (1)
		XTestFakeKeyEvent(remote_dpy, ev->xkey.keycode, 1, 0);
#endif /* XTEST */
		;
	    break;

	case KeyRelease:
#ifdef XTRAP
	    if (tc)
		XESimulateXEventRequest(tc, type,
		    ev->xkey.keycode,
		    lastx, lasty, rem_screen);
	    else
#endif /* XTRAP */
#ifdef XTEST
	    if (1)
		XTestFakeKeyEvent(remote_dpy, ev->xkey.keycode, 0, 0);
#endif /* XTEST */
		;
	    break;

	case ButtonPress:
#ifdef XTRAP
	    if (tc)
		XESimulateXEventRequest(tc, type,
		    ev->xbutton.button,
		    (lastx = ev->xbutton.x),
		    (lasty = ev->xbutton.y),
		    rem_screen);
	    else
#endif /* XTRAP */
#ifdef XTEST
	    if (1)
		XTestFakeButtonEvent(remote_dpy, ev->xbutton.button, 1, 0);
#endif /* XTEST */
		;
	    break;

	case ButtonRelease:
#ifdef XTRAP
	    if (tc)
		XESimulateXEventRequest(tc, type,
		    ev->xbutton.button,
		    (lastx = ev->xbutton.x),
		    (lasty = ev->xbutton.y),
		    rem_screen);
	    else
#endif /* XTRAP */
#ifdef XTEST
	    if (1)
		XTestFakeButtonEvent(remote_dpy, ev->xbutton.button, 0, 0);
#endif /* XTEST */
		;
	    break;

	/*** Use a window larger than screen resolution and move
	 *** window so that window origin is not screen origin and use
	 *** xmotion.x,y fields for resolution translation
	 *** when control dpy different resolution than remote dpy ***/
	case MotionNotify:
	    x = ev->xmotion.x_root;
	    y = ev->xmotion.y_root;
	    lastx = ev->xmotion.x;
	    lasty = ev->xmotion.y;

	    if ((x < min_x) || (x > max_x) ||
		(y < min_y) || (y > max_y))
	    {
		if (x < min_x)
		    if ((off_x += Ap->trip_thickness) > 0)
			off_x = 0;
		if (y < min_y)
		    if ((off_y += Ap->trip_thickness) > 0)
			off_y = 0;

		/* maximum offset is in negative direction */
		if (x > max_x)
		    if ((off_x -= Ap->trip_thickness) < off_x_max)
			off_x = off_x_max;
		if (y > max_y)
		    if ((off_y -= Ap->trip_thickness) < off_y_max)
			off_y = off_y_max;
		
		XMoveWindow(control_dpy, myWin, off_x, off_y);
		XFlush(control_dpy);
	    }
#ifdef XTRAP
	    if (tc)
		XESimulateXEventRequest(tc, type, 0, lastx, lasty, rem_screen);
	    else
#endif /* XTRAP */
#ifdef XTEST
	    if (1)
	    {
		/* printf("test: %d,%d\n",lastx,lasty); */
		XTestFakeMotionEvent(remote_dpy, rem_screen, lastx, lasty, 0);
	    }
	    else
#endif /* XTEST */
#ifdef XTRAP_NOT_NECESSARY
		XWarpPointer(remote_dpy,None,RootWindow(remote_dpy,rem_screen),
		    0, 0, 0, 0, lastx, lasty);
#endif /* XTRAP_NOT_NECESSARY */
		1;
	    break;
    }
}

int	SaveArgc=0;
char 	*SaveArgv[MAX_DISPLAYS+10];

#ifdef NDEF
static int
IgnoreErrors(dpy, ev, codes, rc)
    Display *dpy;
    XErrorEvent *ev;
    XExtCodes *codes;
    int *rc;
{
    fprintf("ignoring an error...\n");

    *rc = 0;
    return False;
}

void
restart_main(errdpy)
    Display *errdpy;
{
    int i;
    for (i=0; i<MAX_DISPLAYS; i++)
    {
#ifdef RTX
	rtx[i]=NULL;
#endif /* RTX */
#ifdef XTRAP
	if (tcs[i])
	{
	    XtCloseDisplay(tcs[i]->dpy);
	    tcs[i]=NULL;
	}
#endif /* XTRAP */
	if (Connection[i])
	{
	    close(Connection[i]);
	    Connection[i]=0;
	}
	TopLevel[i]=0;
    }
    exit(main(SaveArgc,SaveArgv));
}
#endif /* NDEF */

void
xhydra_move_to(client_data)
    int client_data;
{
    if (client_data==0)
    {
	/*printf("taking down input only window\n");*/
	XUnmapWindow(control_dpy,myWin);
	XFlush(control_dpy);
#ifdef XTRAP
	tc=NULL;
#endif /* XTRAP */
	remote_dpy=NULL;
    }
    else
    {
	/*printf("putting up input only window\n");*/
	XMapWindow(control_dpy,myWin);
	/* XXX  -- should be XTRAP or XTEST */
	XSetInputFocus(control_dpy,myWin,RevertToNone,CurrentTime);
#ifdef XTRAP
#endif /* XTRAP */
	XFlush(control_dpy);

	remote_dpy=XtDisplay(TopLevel[client_data]);
	rem_screen=DefaultScreen(remote_dpy);

#ifdef XTRAP
	tc = tcs[tcs_connection[client_data]];
#endif /* XTRAP */
    }
#ifdef HYPER
    if (last_client_data >= 0)
	unmap_hyper(last_client_data);
    map_hyper(last_client_data=client_data);
#endif /* HYPER */

    CurrentDisplay = client_data;
}

int just_tripped_from=0;
int just_tripped_to=0;
int just_tripped=0;

void
trip_to(from,to,side)
    int from,to,side;
{
    unsigned int	nchildren;
    Window  root,parent,*children;
    int j;

    just_tripped_from = from;
    just_tripped_to = to;
    just_tripped = 1;

    off_x = off_y = 0;	/* reinitialize offsets */
    XMoveWindow(control_dpy, myWin, off_x, off_y);
    XFlush(control_dpy);

    /*
     * Unmap the corresponding window:
     */
    if (XtIsRealized(TopLevel[from]))
	XtUnmapWidget(TopLevel[from]);

    /*
    * Clear all toggles:
    */
    for (j = 0; j < NumDisplays; j++)
	if ((DisplayId[j] != NULL) && (Toggles[from][j] != NULL))
	    XmToggleButtonSetState(Toggles[from][j],False,False);

    /*
    * Set the selected window's toggle corresponding to itself:
    */
    XmToggleButtonSetState(Toggles[to][to],True,False);

    /*
    * Now show the window:
    */
    if (XtIsRealized(TopLevel[to]))
	XtMapWidget(TopLevel[to]);
    else
    {
	XtRealizeWidget(TopLevel[to]);
	RegisterProtocolHandlers(to);

	/*
	 * Add a DestroyNotify event selection:
	 */
	XQueryTree(XtDisplay(TopLevel[to]),
	    XtWindow(TopLevel[to]),
	    &root,&parent,&children,&nchildren);
	XtFree((XtPointer)children);
	XSelectInput(XtDisplay(TopLevel[to]),parent,SubstructureNotifyMask);

	XFlush(DisplayId[to]);
    }

    xhydra_move_to(to);
#ifdef HYPER
    if (hyper_symetric)
    {
	short width = hyper[from].dpy_w;
	short height = hyper[from].dpy_h;

	switch(side)
	{
	    case dir_left:
		off_x = 0;
		lastx = width - lastx - Ap->trip_thickness - 2;
		break;
	    case dir_right:
		off_x = off_x_max;
		lastx = width - lastx - off_x - Ap->trip_thickness + 2;
		break;
	    case dir_top:
		off_y = 0;
		lasty = height - lasty - Ap->trip_thickness - 2;
		break;
	    case dir_bottom:
		off_y = off_y_max;
		lasty = height - lasty - off_y - Ap->trip_thickness + 2;
		break;
	}
	if ((side==dir_left) || (side==dir_right))
	    printf("hop accross horizontally to (%d,%d)\n",lastx,lasty);
	else	/* if ((side==dir_top) || (side==dir_bottom)) */
	    printf("hop accross vertically to (%d,%d)\n",lastx,lasty);
	XWarpPointer(control_dpy,None,RootWindow(control_dpy,screen),
	    0, 0, 0, 0, lastx, lasty);
	/* XMoveWindow(control_dpy, myWin, off_x, off_y); */
	XFlush(control_dpy);
    }
#endif /* HYPER */
}

void
main(argc,argv)
    int 	argc;
    char	*argv[];
{
    int		i,j,dispno;
    XEvent	event,*ev;
    XSetWindowAttributes	xswa;

    if (SaveArgc==0)
    {
	for (i=0; i<argc; i++)
	    SaveArgv[i] = argv[i];
	SaveArgc = argc;
    }
    else
	printf("restarting main\n");

    /*
     * Initialize:
     */
    ProgramName = argv[0];

    for (i = 0; i < MAX_DISPLAYS; i++)
    {
	DisplayId[i] = NULL;
	DisplayName[i] = XtNewString("");
	TopLevel[i] = NULL;

	for (j = 0; j < MAX_DISPLAYS; j++)
	    Toggles[i][j] = NULL;

	for (j = 0; j < 4; j++)
	    hyper[i].map[j] = -1;
    }

#ifdef NDEF
    XSetErrorHandler(IgnoreErrors);
    XSetIOErrorHandler(restart_main);
#endif /* NDEF */

    /*
     * Initialize the control display, and get the resources:
     */
    TopLevel[0] = XtAppInitialize(&AppContext,"XHydra",Options,
		XtNumber(Options),&argc,argv,NULL,NULL,0);
    DisplayId[0] = XtDisplay(TopLevel[0]);

    XtGetApplicationResources(TopLevel[0],Ap,
		Resources,XtNumber(Resources), NULL,0);

    control_dpy = DisplayId[0];

#ifdef RTX
    if (rtx[ConnectionNumber(DisplayId[0])]= XrtInit(DisplayId[0],False))
	XrtSetClientPriority(DisplayId[0],
	    XrtMaxPriority,XrtAbsolute,XrtImmediate);
    else
	printf("could not initialize RTX extension on display %s\n",
	    DisplayString(DisplayId[0]));
#endif /* RTX */

    screen = DefaultScreen(control_dpy);

    hints.flags = USPosition|USSize;
    hints.x = 0;
    hints.y = 0;
    dpy_w = hints.width = DisplayWidth(control_dpy, screen);
    dpy_h = hints.height = DisplayHeight(control_dpy, screen);
    myWin = 0;			/* so addDisplay knows not created yet */

    /*
     * Set up the initial display names:
     */
    XtFree(DisplayName[0]);
    DisplayName[0] = XtNewString(DisplayString(DisplayId[0]));
    for (i = 1; i < argc; i++)
    {
	XtFree(DisplayName[i]);
	DisplayName[i] = XtNewString(argv[i]);
    }

    /*
     * Add the displays:
     */
    for (i = 0; ((i < argc) || (i > MAX_DISPLAYS)); i++)
	AddDisplay(i);

    /*
     * Parse any trip option strings:
     */
    ParseTripOptions();

#ifdef HYPER
    for (i = 0; ((i < argc) || (i > MAX_DISPLAYS)); i++)
	realize_hyper(i);
#endif /* HYPER */

/* begin RTC */
/* start up inputOnly window on control display - after initial addDisplays */
    xswa.override_redirect = True;
    xswa.event_mask =
/* XXX  -- should be XTRAP or XTEST */
      KeyPressMask|KeyReleaseMask|
      ButtonPressMask|ButtonReleaseMask|
#ifdef XTRAP
#endif /* XTRAP */
      PointerMotionMask;

    myWin=XCreateWindow(control_dpy,
      RootWindow(control_dpy,DefaultScreen(control_dpy)),
      hints.x, hints.y, hints.width, hints.height,
      off_x=0, off_y=0, InputOnly,
      CopyFromParent, CWOverrideRedirect|CWEventMask, &xswa);

#ifdef RTX
    if (rtx[ConnectionNumber(control_dpy)])
      XrtSetWindowPriority(control_dpy,myWin,
          XrtMaxPriority,XrtAbsolute,XrtImmediate);
#endif /* RTX */
    XFlush(control_dpy);

    /* maximum offset is in negative direction */
    if (hints.x != dpy_w)
    {
	min_x = hints.width - dpy_w + Ap->trip_thickness;
	max_x = dpy_w - Ap->trip_thickness;
	off_x_max = dpy_w - hints.width;
    }
    else
	max_x = dpy_w + Ap->trip_thickness;

    if (hints.y != dpy_h)
    {
	min_y = hints.height - dpy_h + Ap->trip_thickness;
	max_y = dpy_h - Ap->trip_thickness;
	off_y_max = dpy_h - hints.height;
    }
    else
	max_y = dpy_h + Ap->trip_thickness;
/* end RTC */

    /*
     * Realize the control display:
     */
    XmToggleButtonSetState(Toggles[0][0],True,False);
    XtRealizeWidget(TopLevel[0]);
    RegisterProtocolHandlers(0);
    CurrentDisplay = 0;

#ifdef HYPER
    map_hyper(0);
#endif /* HYPER */
    /*
     * Event loop:
     */
    ev = &event;
    for (;;)
    {
	Boolean dont_dispatch;

	dont_dispatch = False;

        XtAppNextEvent(AppContext,ev);
	for (dispno = 0; dispno < NumDisplays; dispno++)
	    if (ev->xany.display == DisplayId[dispno])
		break;

	switch (ev->type)
	{
#ifdef HYPER
	    case	EnterNotify:
	    {
		int i;
		int to;

		for (i=0; i<SIDES; i++)
		    if (((to=hyper[dispno].map[i])>=0) &&
			(ev->xany.window == hyper[dispno].win[i]))
		    {
#ifdef DEBUG
			printf("Enter: recognize: side %d as disp %d\n", i, to);
#endif
			if (just_tripped && (just_tripped_to == dispno))
			    printf("will not trigger another trip\n");
			else
			    trip_to(dispno,to,i);
			dont_dispatch = True;
			break;
		    }
		just_tripped=0;
		break;
	    }
#endif /* HYPER */
	    case	DestroyNotify:
		if (ev->xdestroywindow.window != XtWindow(TopLevel[dispno]))
		    break;

		if (ev->xdestroywindow.window == XtWindow(TopLevel[0]))
		{
		    fprintf(stderr,
			"%s: Control display closed - terminating\n",
			ProgramName);
		    exit(0);
		}

		fprintf(stderr,"%s: Display \"%s\" closed\n",
		    ProgramName,DisplayName[dispno]);

		XtCloseDisplay(DisplayId[dispno]);
		DisplayId[dispno] = NULL;

		for (i = 0; i < NumDisplays; i++)
		    if (DisplayId[i] != NULL)
			XtSetSensitive(Toggles[i][dispno],False);

		XmToggleButtonSetState(Toggles[0][0],True,True);

		dont_dispatch = True;
		break;

	    case	ClientMessage:
		/*
		 * If this is a DeleteWindow window manager
		 * protocol request we'll fake a click on
		 * toggle 0, to make sure that the main
		 * window is visible:
		 */
		if (ev->xclient.data.l[0] == DeleteWindowAtom[dispno])
		{
		    XmToggleButtonSetState(Toggles[0][0],True,True);
		    dont_dispatch = True;
		}
		break;
	}

	/* ZZZ: parse inputonly */
	if ((dispno==0) && (ev->xany.display == control_dpy) &&
	    (ev->xany.window == myWin))
	{
	    just_tripped=0;
	    /* this is an input event to resimulate */
	    do_sim_input(ev);
	    dont_dispatch = True;
	}

	if (! dont_dispatch)
	    XtDispatchEvent(ev);
    }
}

static void
AddDisplay(p_dispno)
    int	p_dispno;
{
    int				i,j,n;
    Arg				args[20];
    Widget			main_rc,radio_box,command_rc,frame,button;
    int				w,h;
    int				tmp_argc=0;
    char 			*tmp_argv[MAX_DISPLAYS+10];
    Display			*my_display;
    XSetWindowAttributes	xswa;
    int				resizeMyWin=0;

    if (p_dispno != 0)
    {
	/* this section is only for non-control displays */

	/** copy orig args from SaveArgv into tmp_argv **/
	for (i=0; i<SaveArgc; i++)
	    tmp_argv[i] = SaveArgv[i];
	tmp_argc = SaveArgc;

        /*
	 * Open a display connection, using tmp args to reuse all X options: 
	 */
	my_display = DisplayId[p_dispno] = XtOpenDisplay(AppContext,
		DisplayName[p_dispno],NULL,"XHydra",NULL,0,
		&tmp_argc,tmp_argv);

	if (DisplayId[p_dispno] == NULL)
	{
	    fprintf(stderr,"Can't open display \"%s\"\n",DisplayName[p_dispno]);
	    XtFree(DisplayName[p_dispno]);
	    DisplayName[p_dispno] = XtNewString("");
	    return;
	}

#ifdef RTX
	if (rtx[ConnectionNumber(my_display)]= XrtInit(my_display,False))
          /*
           * we set the RTX client priority on this display to be max
           * so that we will preempt any window manager grabs on this
           * this display
           */
	    XrtSetClientPriority(my_display,
		XrtMaxPriority,XrtAbsolute,XrtImmediate);
#ifdef DEBUG
	else
	    printf("could not initialize RTX extension on display %s\n",
		DisplayString(my_display));
#endif
#endif /* RTX */

	do_ext_init(my_display, p_dispno);

	XFlush(my_display);

/* begin RTC */
      /*
       * we look up the display width in case we need to do screen resolution
       * mapping trickery
       */
	if ((w=DisplayWidth(my_display, screen)) > hints.width)
	{
	    resizeMyWin++;
	    hints.width = w;
	}
	if ((h=DisplayHeight(my_display, screen)) > hints.height)
	{
	    resizeMyWin++;
	    hints.height = h;
	}
      /*
       * if we are adding a display after creating the inputOnly window
       * we may need to change the control window's size.
       */
      if (myWin && resizeMyWin)
          XResizeWindow(control_dpy, myWin, hints.width, hints.height);
/* end RTC */

	TopLevel[p_dispno] = XtAppCreateShell(NULL,"XHydra",
	    applicationShellWidgetClass,DisplayId[p_dispno],NULL,0);
    }

#ifdef HYPER
    init_hyper(p_dispno,DisplayId[p_dispno]);
#endif /* HYPER */

    XtVaSetValues(TopLevel[p_dispno],
		    XmNtitle,DisplayName[p_dispno],
		    XmNallowShellResize,True,
		    NULL);

    if (p_dispno >= NumDisplays)
	NumDisplays = p_dispno+1;

    Connection[p_dispno] = ConnectionNumber(XtDisplay(TopLevel[p_dispno]));

    /*
     * Looks nicer with a stickey-out frame:
     */
    n = 0;
    XtSetArg(args[n], XmNshadowThickness,2);  n++;
    XtSetArg(args[n], XmNshadowType,XmSHADOW_OUT);  n++;
    frame = XmCreateFrame(TopLevel[p_dispno],"frame",args,n);
    XtManageChild(frame);

    /*
     * Main RowColumn:
     */
    n = 0;
    if (Ap->horizontal)
    {
	XtSetArg(args[n], XmNorientation,XmHORIZONTAL);	 n++;
    }
    else
    {
	XtSetArg(args[n], XmNorientation,XmVERTICAL);  n++;
    }
    main_rc = XmCreateRowColumn(frame,"base",args,n);  
    XtManageChild(main_rc);

    /*
     * Framed radio box:
     */
    n = 0;
    XtSetArg(args[n], XmNshadowType,XmSHADOW_ETCHED_IN);  n++;
    frame = XmCreateFrame(main_rc,"radio_frame",args,n);
    XtManageChild(frame);

    n = 0;
    if (Ap->horizontal)
    {
	XtSetArg(args[n], XmNorientation,XmHORIZONTAL);	 n++;
    }
    else
    {
	XtSetArg(args[n], XmNorientation,XmVERTICAL);  n++;
    }
    XtSetArg(args[n], XmNradioBehavior,True);  n++;
    XtSetArg(args[n], XmNradioAlwaysOne,True);	n++;
    radio_box = XmCreateRowColumn(frame,"radio",args,n);  
    XtManageChild(radio_box);
    RadioBox[p_dispno] = radio_box;

    /*
     * Create one toggle button for each existing display:
     */
    for (i = 0; i < NumDisplays; i++)
    {
	if (*(DisplayName[i]) != '\0')
	{
	    n = 0;
	    button = XmCreateToggleButton(radio_box,"btn",args,n);  
	    XtManageChild(button);
	    XtAddCallback(button,XmNvalueChangedCallback,ToggleBtnCB,
		(XtPointer)i);
	    Toggles[p_dispno][i] = button;

	    if (Ap->horizontal)
		WtLabelSetValue(button,"%d",i);
	    else
		WtLabelSetValue(button,"%s",DisplayName[i]);
	}
    }

    /*
     * Now add self to all other radio boxes:
     */
    for (i = 0; i < NumDisplays; i++)
    {
	if ((i != p_dispno) && (*(DisplayName[i]) != '\0'))
	{
	    n = 0;
	    button = XmCreateToggleButton(RadioBox[i],"btn",args,n);  
	    XtManageChild(button);
	    XtAddCallback(button,XmNvalueChangedCallback,ToggleBtnCB,
		(XtPointer)p_dispno);
	    Toggles[i][p_dispno] = button;

	    if (Ap->horizontal)
		WtLabelSetValue(button,"%d",p_dispno);
	    else
		WtLabelSetValue(button,"%s",DisplayName[p_dispno]);
	}
    }

    /*
     * Command area:
     */
    n = 0;
    XtSetArg(args[n], XmNmarginHeight,0);  n++;
    XtSetArg(args[n], XmNmarginWidth,0);  n++;
    XtSetArg(args[n], XmNorientation,XmHORIZONTAL);  n++;
    command_rc = XmCreateRowColumn(main_rc,"command_row",args,n);  
    XtManageChild(command_rc);

    /*
     * Command buttons:
     */
    n = 0;
    button = XmCreatePushButton(command_rc,"Trip...",args,n);
    XtManageChild(button);
    XtAddCallback(button,XmNactivateCallback,TripBtnCB,(XtPointer)p_dispno);

    if (p_dispno == 0)
    {
	n = 0;
	button = XmCreatePushButton(command_rc,"Quit",args,n);
	XtAddCallback(button,XmNactivateCallback,QuitBtnCB,(XtPointer)NULL);
	XtManageChild(button);
    }
}

static void
ParseTripOptions()
{
    int		side,attach_from,attach_to;
    Boolean	valid,side_valid;
    char	*anchor,*string,*token;
    static char	*side_name[] = { "left","right","top","bottom" };

    valid = True;

    for (side = 0; side < SIDES; side++)
    {
	side_valid = True;
	anchor = string = XtNewString(Ap->trip_side[side]);

	while ((token = strtok(string,":")) != NULL)
	{
	    attach_from = attach_to = -1;
	    if (! WtIsNumeric(token))
		side_valid = False;
	    else
	    {
		sscanf(token,"%d",&attach_from);
		if (! WtIN_RANGE(attach_from,0,NumDisplays-1))
		    side_valid = False;
	    }

	    token = strtok(0,",");
	    if (! WtIsNumeric(token))
		side_valid = False;
	    else
	    {
		sscanf(token,"%d",&attach_to);
		if (! WtIN_RANGE(attach_to,0,NumDisplays-1))
		    side_valid = False;
	    }

	    /*
	     * Abandon this side if the spec is invalid:
	     */
	    if (! side_valid)
	    {
		fprintf(stderr,"Invalid specification \"%s\" for %s tripwire\n",
		    Ap->trip_side[side],side_name[side]);
		valid = False;
		break;
	    }

#ifdef HYPER
	    hyper_map(attach_from,attach_to,side);
#else */ HYPER */
#ifdef DEBUG
	    printf("Side %s, %d to %d\n",side_name[side],attach_from,attach_to);
#endif
	    hyper[attach_from].map[side] = attach_to;
#endif /* HYPER */

	    /*
	     * Set up for the next spec:
	     */
	    string = 0;
	}
	XtFree(anchor);
    }

    if (! valid)
	exit(1);
}

static void
ToggleBtnCB(w,client_data,call_data)
    Widget	w;
    int 	client_data;
    int 	call_data;
{
    int	    i,j;
    unsigned int	nchildren;
    Window  root,parent,*children;

    /*
     * This routine will also get callbacks for toggles becoming unset,
     * so get the current state to make sure we don't action them:
     */
    if (! XmToggleButtonGetState(w))
	return;

    for (i = 0; i < NumDisplays; i++)
    {
	if (DisplayId[i] != NULL)
	{
	    /*
	     * Don't unmap if the user clicked on the button 
	     * corresponding to the currently-displayed window:
	     */
	    if (client_data != i)
	    {
		/*
		 * Unmap the corresponding window:
		 */
		if (XtIsRealized(TopLevel[i]))
		    XtUnmapWidget(TopLevel[i]);
	    }

	    /*
	     * Clear all toggles:
	     */
	    for (j = 0; j < NumDisplays; j++)
		if ((DisplayId[j] != NULL) && (Toggles[i][j] != NULL))
		    XmToggleButtonSetState(Toggles[i][j],False,False);
	}
    }

    /*
     * Set the selected window's toggle corresponding to itself:
     */
    XmToggleButtonSetState(Toggles[client_data][client_data],True,False);

    /*
     * Now show the window:
     */
    if (XtIsRealized(TopLevel[client_data]))
	XtMapWidget(TopLevel[client_data]);
    else
    {
	XtRealizeWidget(TopLevel[client_data]);
	RegisterProtocolHandlers(client_data);

	/*
	 * Add a DestroyNotify event selection:
	 */
	XQueryTree(XtDisplay(TopLevel[client_data]),
		XtWindow(TopLevel[client_data]),&root,&parent,
		&children,&nchildren);
	XtFree((XtPointer)children);
	XSelectInput(XtDisplay(TopLevel[client_data]),parent,
		SubstructureNotifyMask);

	XFlush(DisplayId[client_data]);
    }

    xhydra_move_to(client_data);
}

static void
QuitBtnCB(w,client_data,call_data)
    Widget	w;
    int 	client_data;
    int 	call_data;
{
    exit(0);
}

static void
TripBtnCB(w,client_data,call_data)
    Widget	w;
    int 	client_data;	/* Internal display number */
    int 	call_data;
{
    int		i,n;
    Arg		args[10];
    Widget	rc,radio_box,frame,button;
    Position	x,y;

    static char *trip_label[] = { "Left","Right","Top","Bottom" };

    /*
     * Desensitize the main window while the dialog is up:
     */
    XtSetSensitive(TopLevel[client_data],False);

    /*
     * Create the basic dialog:
     */
    TripDialogSpec.cb_data = (client_data << 4);
    WtDialogCreate(TopLevel[client_data],"trip_dialog",&TripDialogSpec);

    /*
     * Add the body items to the dialog:
     */
    n = 0;
    rc = XmCreateRowColumn(TripDialogSpec.rc_w,"",args,n);
    XtManageChild(rc);

    WtLabel(rc,""," Tripwire:");

    n = 0;
    XtSetArg(args[n], XmNshadowType, XmSHADOW_ETCHED_IN);  n++;
    frame = XmCreateFrame(rc,"frame",args,n);
    XtManageChild(frame);

    n = 0;
    XtSetArg(args[n], XmNradioBehavior,True);  n++;
    XtSetArg(args[n], XmNradioAlwaysOne,True);	n++;
    radio_box = XmCreateRowColumn(frame,"trip_radio",args,n);  
    XtManageChild(radio_box);

    /*
     * Tripwire toggle buttons:
     */
    for (i = 0; i < 4; i++)
    {
	n = 0;
	button = XmCreateToggleButton(radio_box,trip_label[i],args,n);	
	XtManageChild(button);
	XtAddCallback(button,XmNvalueChangedCallback,TripToggleCB,(XtPointer)i);
	TripToggle[i] = button;
    }

    WtLabel(rc,""," Warps to:");

    n = 0;
    XtSetArg(args[n], XmNshadowType, XmSHADOW_ETCHED_IN);  n++;
    frame = XmCreateFrame(rc,"frame",args,n);
    XtManageChild(frame);

    n = 0;
    XtSetArg(args[n], XmNorientation,XmVERTICAL);  n++;
    XtSetArg(args[n], XmNradioBehavior,True);  n++;
    XtSetArg(args[n], XmNradioAlwaysOne,True);	n++;
    radio_box = XmCreateRowColumn(frame,"warp_radio",args,n);  
    XtManageChild(radio_box);

    /*
     * Warp toggle buttons:
     */
    n = 0;
    button = XmCreateToggleButton(radio_box,"None",args,n);  
    XtManageChild(button);
    XtAddCallback(button,XmNvalueChangedCallback,WarpToggleCB,(XtPointer)-1);
    WarpToggle[MAX_DISPLAYS] = button;

    for (i = 0; i < NumDisplays; i++)
    {
	if (DisplayId[i] != NULL)
	{
	    n = 0;
	    button = XmCreateToggleButton(radio_box,"button",args,n);  
	    WtLabelSetValue(button,"%s",DisplayName[i]);
	    XtManageChild(button);
	    XtAddCallback(button,XmNvalueChangedCallback,WarpToggleCB,
		(XtPointer)i);
	    WarpToggle[i] = button;

	    XtVaSetValues(button,XmNmarginTop,0,
				 XmNmarginBottom,0,
				 NULL);
	}
    }

    /*
     * Now add the control buttons to the dialog base:
     */
    WtDialogAddButtons("trip_buttons",&TripDialogSpec);

    /*
     * Get the position of the parent window, and position the
     * child accordingly:
     */
    XtVaGetValues(TopLevel[client_data],
		XmNx,&x,
		XmNy,&y,
		NULL);

    XtVaSetValues(TripDialogSpec.bb_w,
		XmNx,x+8,
		XmNy,y+8,
		NULL);
    XtManageChild(TripDialogSpec.bb_w);

    /*
     * Fake a click on the Left tripwire toggle,
     * to get the initial setup:
     */
    SelectedWarpToggle = NULL;
    XmToggleButtonSetState(TripToggle[0],True,True);
}

static void
TripToggleCB(w,client_data,call_data)
    Widget	w;
    int 	client_data;	/* Trip side (0 = left) */
    int 	call_data;
{
    Widget  set_toggle;

    /*
     * Ignore it if the callback is the result of an unset:
     */
    if (! XmToggleButtonGetState(w))
	return;

    SelectedTripSide = client_data;
	
    if (hyper[CurrentDisplay].map[SelectedTripSide] == -1)
	set_toggle = WarpToggle[MAX_DISPLAYS];
    else
	set_toggle = WarpToggle[hyper[CurrentDisplay].map[SelectedTripSide]];

    /*
     * Set the appropriate toggle - the callback will clear
     * the currently set one:
     */
    XmToggleButtonSetState(set_toggle,True,True);
}

static void
WarpToggleCB(w,client_data,call_data)
    Widget	w;
    int 	client_data;	/* -1 for "None", display no for others */
    int 	call_data;
{
    /*
     * Ignore it if the callback is the result of an unset:
     */
    if (! XmToggleButtonGetState(w))
	return;

    if (SelectedWarpToggle != NULL)
	XmToggleButtonSetState(SelectedWarpToggle,False,False);

    SelectedWarpToggle = w;
    hyper[CurrentDisplay].map[SelectedTripSide] = client_data;
}


static void
TripDialogBtnCB(w,client_data,call_data)
    Widget	w;
    int 	client_data;
    int 	call_data;
{
    int dispno;

    dispno = (client_data >> 4);
    XtDestroyWidget(XtParent(TripDialogSpec.bb_w));
    XtSetSensitive(TopLevel[dispno],True);
}


void
RegisterProtocolHandlers(p_dispno)
    int	p_dispno;
{
    Display *disp;
    Window  window;

    window = XtWindow(TopLevel[p_dispno]);
    disp = XtDisplay(TopLevel[p_dispno]);

    /*
     * Register Protocol Handlers for intercepting KillClient actions:
     */
    DeleteWindowAtom[p_dispno] = XmInternAtom(disp,"WM_DELETE_WINDOW",False);
    XSetWMProtocols(disp,window,&DeleteWindowAtom[p_dispno],1);
}

/*=========================================================================*
 *									   *
 * Subroutine:	WtDialogCreate						   *
 *									   *
 * Function:								   *
 *    Creates a Dialog Shell with a borderless BulletinBoard/Row Column	   *
 *    widget hierarchy.	  The dialog is unmanaged;  the caller should	   *
 *    set up the body as necessary,  then call WtDialogAddButtons to	   *
 *    complete the creation,  and then manage the BulletinBoard child.	   *
 *									   *
 *    The reasons for creating the dialog in two stages are that it	   *
 *    reduces the complexity of the widget hierarchy,  and that because	   *
 *    the body widgets are created before the buttons they will		   *
 *    automatically get the keyboard focus first.			   *
 *									   *
 * Return Value:							   *
 *    Widget-id of the Bulletin Board which is the immediate child	   *
 *    of the Dialog Shell.						   *
 *									   *
 *=========================================================================*/
Widget
WtDialogCreate(p_parent,p_name,p_spec)
    Widget		p_parent;
    char		*p_name;
    WtDialogSpec_t	*p_spec;
{
    int		n;
    Arg		args[10];

    /*
     * Create a Dialog shell:
     */
    n = 0;
    XtSetArg(args[n], XmNtitle, p_spec->title); n++;
    p_spec->shell_w = XmCreateDialogShell(p_parent,p_name,args,n);

    /*
     * Create a Bulletin Board as its child:
     */
    n = 0;
    if (p_spec->flags & WtDIALOG_FLG_NODEFPOS)
    {
	XtSetArg(args[n], XmNdefaultPosition, False); n++;
    }
    if (p_spec->flags & WtDIALOG_FLG_MODAL)
    {
	XtSetArg(args[n], XmNdialogStyle, XmDIALOG_APPLICATION_MODAL); n++;
    }
    XtSetArg(args[n], XmNshadowThickness, 2); n++;
    XtSetArg(args[n], XmNshadowType, XmSHADOW_OUT); n++;
    XtSetArg(args[n], XmNmarginHeight, 2); n++;
    XtSetArg(args[n], XmNmarginWidth, 2); n++;
    p_spec->bb_w = XmCreateBulletinBoard(p_spec->shell_w,p_name,args,n);

    /*
     * Now create a RowColumn:
     */
    n = 0;
    XtSetArg(args[n], XmNorientation,XmVERTICAL);  n++;
    XtSetArg(args[n], XmNmarginHeight,0);  n++;
    XtSetArg(args[n], XmNmarginWidth,0);  n++;
    p_spec->rc_w = XmCreateRowColumn(p_spec->bb_w,p_name,args,n);
    XtManageChild(p_spec->rc_w);

    /*
     * Exit:
     */
    return p_spec->bb_w;
}

/*=========================================================================*
 *									   *
 * Subroutine:	WtDialogAddButtons					   *
 *									   *
 * Function:								   *
 *    Adds the separator and the button row to a Dialog which		   *
 *    has been created using WtDialogCreate.				   *
 *									   *
 * Return Value:							   *
 *    Widget-id of the Bulletin Board which is the immediate child	   *
 *    of the Dialog Shell.						   *
 *									   *
 *=========================================================================*/
Widget
WtDialogAddButtons(p_name,p_spec)
    char		*p_name;
    WtDialogSpec_t	*p_spec;
{
    int		i,n,count,default_button,offset;
    Arg		args[10];
    Widget	temp_w,form_w;
    Dimension	width,max_width,height,default_height;
    short	shadow,highlight;
    Widget	btns[WtDIALOG_MAX_LEFT_BTN+1],last_btn;

    static char *labels[] = 
    {
	"Yes","No","OK","Apply","Retry","Stop",
	"","Reset","Cancel","Close", "Help",
    };

    /*
     * Clear returned widget-id's:
     */
    for (i = 0; i < WtDIALOG_MAX_BTN; i++)
	p_spec->button_w[i] = NULL;

    /*
     * Create the separator:
     */
    n = 0;
    temp_w = XmCreateSeparator(p_spec->rc_w,"separator",args,n);
    XtManageChild(temp_w);

    /*
     * Create a Form to hold the buttons:
     */
    n = 0;
    XtSetArg (args[n], XmNmarginHeight, 0);  n++;
    XtSetArg (args[n], XmNmarginWidth, 3);  n++;
    form_w = XmCreateForm(p_spec->rc_w,p_name,args,n);
    XtManageChild(form_w);

    /*
     * Now the left pushbuttons:
     */
    count = 0;
    last_btn = NULL;
    default_button = -1;
    for (i = 0; i < WtDIALOG_MAX_LEFT_BTN; i++)
    {
	if (WtFLAG_IS_ON(p_spec->flags,(1 << i)))
	{
	    n = 0;
	    XtSetArg(args[n], XmNalignment,XmALIGNMENT_CENTER);	 n++;
	    XtSetArg(args[n], XmNtopAttachment,XmATTACH_FORM);	n++;
	    XtSetArg(args[n], XmNbottomAttachment,XmATTACH_FORM);  n++;
	    if (last_btn == NULL)
	    {
		XtSetArg(args[n], XmNleftAttachment,XmATTACH_FORM);  n++;
	    }
	    else
	    {
		XtSetArg(args[n], XmNleftAttachment,XmATTACH_WIDGET);  n++;
		XtSetArg(args[n], XmNleftWidget,last_btn);  n++;
	    }
	    p_spec->button_w[i] = btns[count] = 
		XmCreatePushButton(form_w,labels[i],args,n);

	    /*
	     * If this is the OK button,  replace the text
	     * if any supplied:
	     */
	    if (i == WtDIALOG_BTN_OK)
	    {
		if (p_spec->ok_label != NULL)
		    if (*(p_spec->ok_label) == '\0')
			WtLabelSetValue(btns[count],p_spec->ok_label);
	    }

	    /*
	     * If this is the Text button,  set the label: 
	     */
	    if (i == WtDIALOG_BTN_TEXT)
		WtLabelSetValue(btns[count],p_spec->text_label);

	    /*
	     * Callback required?
	     */
	    if (p_spec->callback != NULL)
	    {
		/*
		 * Mask the button number into the bottom four
		 * bits of the callback data:
		 */
		XtAddCallback(btns[count],XmNactivateCallback,p_spec->callback,
		    (XtPointer) ((p_spec->cb_data & 0xfffffff0) | i));
	    }

	    /*
	     * If this is the default button, save its 
	     * actual button number:
	     */
	    if (i == p_spec->def_btn)
		default_button = count;

	    last_btn = btns[count];
	    count++;
	}
    }
    XtManageChildren(btns,count);

    /*
     * Now the right (Help) pushbutton:
     */
    if (WtFLAG_IS_ON(p_spec->flags,WtDIALOG_FLG_HELP))
    {
	n = 0;
	XtSetArg(args[n], XmNtopAttachment,XmATTACH_FORM);  n++;
	XtSetArg(args[n], XmNbottomAttachment,XmATTACH_FORM);  n++;
	XtSetArg(args[n], XmNrightAttachment,XmATTACH_FORM);  n++;
	XtSetArg(args[n], XmNalignment,XmALIGNMENT_CENTER); n++;
	p_spec->button_w[WtDIALOG_BTN_HELP] = btns[count] = 
	     XmCreatePushButton(form_w,"Help",args,n);

	if (p_spec->callback != NULL)
	{
	    /*
	     * Mask the button number into the bottom four
	     * bits of the callback data:
	     */
	    XtAddCallback(btns[count],XmNactivateCallback,p_spec->callback,
		(XtPointer)
		    ((p_spec->cb_data & 0xfffffff0) | WtDIALOG_BTN_HELP));
	}
	XtManageChild(btns[count]);
	count++;
    }

    /*
     * Find out the basic dimensions of the buttons
     * (assume they're all the same height):
     */
    max_width = 0;
    for (i = 0; i < count; i++)
    {
	XtVaGetValues(btns[i],XmNwidth,&width,
			      XmNheight,&height,
			      NULL);
	max_width = WtMAX(max_width,width);
    }
    default_height = height;

    /*
     * Get info about the specified default button:
     */
    shadow = 0;
    if (default_button != -1)
    {
	XtVaGetValues(btns[default_button],XmNshadowThickness,&shadow,NULL);
	XtVaSetValues(p_spec->bb_w,XmNdefaultButton,btns[default_button],NULL);
    }

    /*
     * Find out whether we're using keyboard traversal.	 If we are,
     * assume that the highlight thickness will separate the buttons;
     * otherwise separate them manually:
     */
    if (count > 0)
    {
	XtVaGetValues(btns[0],XmNhighlightThickness,&highlight,NULL);
	if (highlight == 0)
	    XtVaSetValues(form_w,XmNhorizontalSpacing,6,NULL);
    }

    /*
     * Now force the dimensions of all of the buttons:
     */
    for (i = 0; i < count; i++)
    {
	/*
	 * If this is the default button,  give it a recessed "default"
	 * border with a shadow thickness of 1.	 Since Motif separates
	 * the two shadows by the sum of their thicknesses,  this will
	 * result in an overall increase of (shadow + 2) pixels in all
	 * four directions.
	 */
	if (i == default_button)
	{
	    XtVaSetValues(btns[i],
		XmNshowAsDefault,1,
		XmNwidth, max_width + ((
#ifdef XM_3D_ENHANCE_PIXEL
		XM_3D_ENHANCE_PIXEL +
#endif /* XM_3D_ENHANCE_PIXEL */
		shadow + 2) * 2),
		NULL);

	    XtVaGetValues(btns[i],XmNheight,&default_height,NULL);
	}
	else
	    XtVaSetValues(btns[i],XmNwidth,max_width,NULL);
    }

    /*
     * Locate the buttons vertically:
     */
    for (i = 0; i < count; i++)
    {
	if (i == default_button)
	    offset = 0;
	else
	    offset = ((default_height - height) / 2);

	XtVaSetValues(btns[i],
		XmNtopOffset,offset,
		XmNbottomOffset,offset,
		NULL);
    }

    return p_spec->bb_w;
}


/*=========================================================================*
 *									   *
 * Subroutine:	WtLabel							   *
 *									   *
 * Function:								   *
 *    Creates a label widget, using printf() semantics to form the	   *
 *    text value.							   *
 *									   *
 * Return Value:							   *
 *    Widget-id of created label.					   *
 *									   *
 *=========================================================================*/
Widget
WtLabel(p_parent,p_name,p_template,p1,p2,p3,p4,p5)
    Widget	p_parent;
    char	*p_name;
    char	*p_template;
    int 	p1,p2,p3,p4,p5;
{
    Widget  label;
    Arg	    args[5];
    int	    n;

    n = 0; 
    label = (Widget)XmCreateLabel(p_parent,p_name,args,n);
    XtManageChild(label);

    if (*p_template != '\0')
	WtLabelSetValue(label,p_template,p1,p2,p3,p4,p5);

    return label;
}


/*=========================================================================*
 *									   *
 * Subroutine:	WtLabelSetValue						   *
 *									   *
 * Function:								   *
 *   Sets the ascii value of a label,  using printf() semantics.	   *
 *									   *
 * Return Value:  None							   *
 *									   *
 *=========================================================================*/
void
WtLabelSetValue(w,p_template,p1,p2,p3,p4,p5)
    Widget	w;
    char	*p_template;
    int 	p1,p2,p3,p4,p5;
{
    Arg		args[5];
    int		n;
    XmString	string,oldstring;
    char	*sp,text[256];

    sprintf(text,p_template,p1,p2,p3,p4,p5);

    /*
     * Get the old string,  convert it to characters,  and check
     * if it's the same as the new one.	 If so,	 just return:
     */
    XtVaGetValues(w,XmNlabelString,&oldstring,NULL);

    if (oldstring != NULL)
    {
	WtGETSTRING(oldstring,&sp);
	if (WtIS_SAME(sp,text))
	{
	    XtFree(sp);
	    return;
	}
	XtFree(sp);
    }

    /*
     * Only use WtSTRING_L_TO_R if the text contains newlines,
     * because at Motif 1.0.3 it leaks memory:
     */
    if (strchr(text,'\n') != NULL)
	string = WtSTRING_L_TO_R(text);
    else
	string = WtSTRING(text);

    /*
     * Update the label:
     */
    XtVaSetValues(w,XmNlabelString,string,NULL);

    /*
     * No need to free the old string,	because GetValues just
     * returns a pointer to the Widget's private copy:
     */
    XmStringFree(string);
}


/*=========================================================================*
 *									   *
 * Subroutine:	WtIsBlank						   *
 *									   *
 * Function:								   *
 *     Checks for a blank character string				   *
 *									   *
 * Return Value:							   *
 *     False:  Not blank						   *
 *     True:   Blank							   *
 *									   *
 *=========================================================================*/
Boolean 
WtIsBlank(p)
char	*p;
{
    while (*p != '\0')
	if (! isspace(*p++))
	    return False;
    return True;
}


/*=========================================================================*
 *									   *
 * Subroutine:	WtIsNumeric						   *
 *									   *
 * Function:								   *
 *     Checks for a numeric character string				   *
 *									   *
 * Return Value:							   *
 *     False:  Not numeric						   *
 *     True:   Numeric							   *
 *									   *
 *=========================================================================*/
Boolean 
WtIsNumeric(p)
    char	*p;
{
    if (*p == '\0')
	return False;
    while (*p != '\0')
	if (! isdigit(*p++))
	    return False;
    return True;
}


/*=========================================================================*
 *									   *
 * Subroutine:	WtLeftJustify						   *
 *									   *
 * Function:								   *
 *     In-place deletion of leading whitespace from a character string	   *
 *									   *
 * Return Value:							   *
 *     New length of string						   *
 *									   *
 *=========================================================================*/
int 
WtLeftJustify(p)
    char	*p;
{
    int i;

    i = 0;
    while (isspace(*(p+i)))
	i++;

    if (i > 0)
	strcpy(p,p+i);

    return strlen(p);
}


/*=========================================================================*
 *									   *
 * Subroutine:	WtRightTruncate						   *
 *									   *
 * Function:								   *
 *     Deletes any trailing whitespace from the end of a character	   *
 *     string by shifting the NULL character left.			   *
 *									   *
 * Return Value:							   *
 *     New length of string						   *
 *									   *
 *=========================================================================*/
int 
WtRightTruncate(p)
    char	*p;
{
    char    *p2;

    p2 = p + strlen(p);
    while (p2 > p)
    {
	if (isspace(*(--p2)))
	    *p2 = '\0';
	else
	    break;
    }

    return strlen(p);
}
