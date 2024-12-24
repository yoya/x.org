

/* File: klpxtest.c - last edit by */
/* langone:	07 Jul 86 10:21 AM */

#include <X/Xlib.h>
#include <strings.h>
#include "Toolkit.h"

extern char *malloc();

/* -----> Global Font Information */

#define tagFontName "timrom10b"
#define fontName "helv10"
#define textFontName "6x13p"
#define xMargin 2
#define yMargin 2

#define bitmap_w 48
#define bitmap_h 16


FontInfo *gfontinfo;
static Window w;
static XEvent *event;
static Window wa[10];
static int hints[10];
static int *f;
static OpaqueFrame *frame;
static Display *display;
static FontInfo *tagFontInfo;
static FontInfo *textFontInfo;
static Pixmap black, white;
static int myBool1, myBool2, myBool3, myBool4;
static int myEnumeration;
static int myNumber;
static char *str;
static Cursor cursor;
static int debug;


Dummy()

{
}

SProc(position, tooldata)
float position;
caddr_t tooldata;
{

  int *ptr;
  int dummy;

	/*(ptr = (int*)tooldata;
	dummy = *ptr;
 	printf("dummy = %d\n", dummy);*/
	printf("thumb position = %f \n", position);
	
}

PProc()
{
    printf ("PageProc Called \n");
}


ProcessEvent()
{
  for (;;) {
    XNextEvent (event);
    TDispatchXEvent(event);

  }
}



main()
{
    int     error;
    int *extradata;
    int integerdata;
    OpaqueFrame *fa;
    Window tw1, tw2, tw;
    Window *wa;
    caddr_t *tags;
    
    Bitmap mybitmap;
    Pixmap mypixmap;
    Pixmap pixmaps[];
    int planes;
    int pixw, pixh, newx, newy;
    int ni, i, pixmw, pixmh, txcolor;
    Targ args[6];
    float fvalue;
    FontInfo *lfontinfo;
    Status status;
     
    debug = FALSE;


    display = XOpenDisplay("");
    if (display == 0)
	printf("test: Error While trying to open display\n");
    else {
	event = (XEvent *) malloc(sizeof(XEvent));
	w = XCreateWindow(RootWindow, 400, 0, 300, 300, 2, 
			BlackPixmap, WhitePixmap);

	if (w == 0)
	    printf("test: Error While trying to create main window\n");
	else {
	fvalue = 0.4;


	TSetArg (args[0], T_SCROLLBAR_ORIENT, Right);
	TSetArg (args[1], T_SCROLLBAR_THUMBPOS, &fvalue);
	TSetArg (args[1], T_SCROLLBAR_THUMBSIZE, 15);
	TSetArg (args[2], T_SCROLLBAR_TPROC, SProc);
	TSetArg (args[3], T_SCROLLBAR_STHICK, 10);
	TSetArg (args[4], T_SCROLLBAR_THUMBPOS, &fvalue);
	TSetArg (args[5], 0, 0);

  	tw = TCreateScrollBar(w, args);

	TSetArg (args[0], T_LABEL_STRING, "SCROLL BAR --->");
	TSetArg (args[1], 0, 0);

	tw1 = TCreateLabel(w, args);

	/* -----> Map the scroll bar tool window */
	XMapWindow(tw);

	/* -----> Map the label window */
	XMapWindow(tw1);

	/* -----> Move the label window */
	XMoveWindow(tw1, 50, 120);

	/* -----> Map the parent window */
	XMapWindow(w);

 	/* -----> Select all input events */
	TSetXEventDispatch(w, Dummy, allEvents, 0);


	TSetArg (args[1], 0, 0);
	TSetScrollBarAttr(tw, args);

	/*TSetArg (args[0], T_COMMAND_PIXM, 0);
	TSetArg (args[1], 0, 0);
	TGetCommandAttr(tw2, args);*/

	/* -----> Goto to process event loop */
	ProcessEvent();

	}
    }
}
