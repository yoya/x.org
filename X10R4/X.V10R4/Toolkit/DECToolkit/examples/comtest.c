

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

#define commap_width 60
#define commap_height 60
static short commap_bits[] = {
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0xffff, 0x007f, 0x0000,
   0x0000, 0x0003, 0x00c0, 0x0000,
   0x0000, 0x0006, 0x0180, 0x0000,
   0x0000, 0x000c, 0x0302, 0x0000,
   0x0000, 0x0018, 0x0602, 0x0000,
   0x0000, 0x3c30, 0x0c02, 0x0000,
   0x0000, 0xc060, 0x1803, 0x0000,
   0x0000, 0x0040, 0x301e, 0x0000,
   0x0000, 0x00c0, 0x6062, 0x0000,
   0x0000, 0x0180, 0xc182, 0x0000,
   0x0000, 0x0300, 0x8002, 0x0001,
   0x0000, 0x0600, 0x0002, 0x0003,
   0x0000, 0x0c00, 0x0000, 0x0002,
   0x0000, 0x1800, 0x0000, 0x0006,
   0xff80, 0x3fff, 0x0000, 0x007c,
   0x0380, 0x3000, 0x0000, 0x00cc,
   0x0680, 0x2000, 0x0000, 0x018c,
   0x1c80, 0xffe0, 0xffff, 0x031f,
   0x7080, 0x0000, 0x0000, 0x0e00,
   0xc080, 0x0000, 0x0000, 0x0800,
   0x8080, 0x0003, 0x0000, 0x0000,
   0x0080, 0x000e, 0x0000, 0x0000,
   0x0080, 0x0018, 0x0000, 0x0000,
   0x0080, 0xfff0, 0xffff, 0x0fff,
   0x0080, 0x0010, 0x0000, 0x0000,
   0x0080, 0x0010, 0x0000, 0x0000,
   0x0080, 0x0010, 0x0000, 0x0000,
   0x0080, 0x0010, 0x0000, 0x0000,
   0x0080, 0x0010, 0x0000, 0x0000,
   0x0080, 0x0010, 0x0000, 0x0000,
   0x0080, 0x0010, 0x0000, 0x0000,
   0x0080, 0x0010, 0x0000, 0x0000,
   0x0080, 0x0010, 0x0000, 0x0000,
   0x0080, 0x0010, 0x0000, 0x0000,
   0x0080, 0x0010, 0x0000, 0x0000,
   0x0080, 0x0010, 0x0000, 0x0000,
   0x0080, 0x0010, 0x0000, 0x0000,
   0x0080, 0x0010, 0x0000, 0x0000,
   0x0080, 0x0010, 0x0000, 0x0000,
   0x0080, 0x0010, 0x0000, 0x0000,
   0x0080, 0x0010, 0x0000, 0x0000,
   0x0080, 0x0010, 0x0000, 0x0000,
   0x0080, 0x0010, 0x0000, 0x0000,
   0x0180, 0x0010, 0x0000, 0x0000,
   0x0600, 0x0010, 0x0000, 0x0000,
   0x0c00, 0x0010, 0x0000, 0x0000,
   0x1800, 0x0010, 0x0000, 0x0000,
   0x3000, 0x0010, 0x0000, 0x0000,
   0xe000, 0x0010, 0x0000, 0x0000,
   0x8000, 0x0013, 0x0000, 0x0000};


CProc1(ev, tooldata)
XEvent *ev;
caddr_t tooldata;
{
int *ptr;
int dummy;

    printf("The file is being printed\n");
}
CProc2(ev, tooldata)
XEvent *ev;
caddr_t tooldata;
{
int *ptr;
int dummy;

    printf("The ballot is inserted\n");
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
    Window tw1, tw2, tw3, tw;
    Window *wa;
    caddr_t *tags;
    
    Bitmap mybitmap;
    Pixmap mypixmap;
    Pixmap pixmaps[];
    int planes;
    int pixw, pixh, newx, newy;
    int ni, i, pixmw, pixmh, txcolor;
    Targ args[4];
    FontInfo *lfontinfo;
    Status status;
     
    debug = FALSE;


    display = XOpenDisplay("");
    if (display == 0)
	printf("test: Error While trying to open display\n");
    else {
	event = (XEvent *) malloc(sizeof(XEvent));
	w = XCreateWindow(RootWindow, 0, 0, 450, 320, 2, 
			BlackPixmap, WhitePixmap);

	if (w == 0)
	    printf("test: Error While trying to create main window\n");
	else {

	/* -----> Open global font */
	gfontinfo = XOpenFont("/usr/lib/Xfont/helv12b.onx");
	lfontinfo = XOpenFont("/usr/lib/Xfont/oldeng.onx");

  	/* -----> create the pixmap */
	if ((mybitmap=XStoreBitmap(commap_width,commap_height,commap_bits))==0)
			{
				printf("cursor1 bitmap not stored\n");
				exit(32);
			}



	if ((mypixmap = XMakePixmap(mybitmap, BlackPixel, WhitePixel))==0)
	{
		printf("mypixmap was not created\n");
		exit(32);
	}



	TSetArg (args[0], T_COMMAND_TYPE, 0);
	TSetArg (args[1], T_COMMAND_TEXT, "Print File");
        TSetArg (args[2], T_COMMAND_BRWIDTH, 2);
	TSetArg (args[3], T_COMMAND_PROC, CProc1);
	TSetArg (args[4], T_COMMAND_ONHI, (EnterWindow | ButtonPressed) );
	TSetArg (args[5], T_COMMAND_OFFHI, 
	(LeaveWindow|ButtonPressed|ButtonReleased) );
	TSetArg (args[6], 0, 0);

  	tw1 = TCreateCommand(w, args);

	TSetArg (args[0], T_COMMAND_TYPE, 1);
	TSetArg (args[1], T_COMMAND_PIXMW, commap_width);
	TSetArg (args[2], T_COMMAND_PIXMH, commap_height);
	TSetArg (args[3], T_COMMAND_BRWIDTH, 1);
	TSetArg (args[4], T_COMMAND_PROC, CProc2);
	TSetArg (args[5], T_COMMAND_PIXM, mypixmap);
	TSetArg (args[6], T_COMMAND_ONHI, (EnterWindow | ButtonPressed) );
	TSetArg (args[7], T_COMMAND_OFFHI, 
	(LeaveWindow|ButtonPressed|ButtonReleased) );
	TSetArg (args[8], 0, 0);

	tw2 = TCreateCommand(w, args);

	TSetArg (args[0], T_LABEL_STRING, "COMMAND --->");
	TSetArg (args[1], 0, 0);

	tw3 = TCreateLabel(w, args);

	XMoveWindow(tw1, 175, 100);
	XMoveWindow(tw2, 320, 100);
	XMoveWindow(tw3, 25, 100);

	/* -----> Map the parent window */
	XMapWindow(tw1);
	XMapWindow(tw2);
	XMapWindow(tw3);

	XMapWindow(w);

 	/* -----> Select all input events */
        XSelectInput(w, allEvents);


	/* -----> Goto to process event loop */
	ProcessEvent();

	}
    }
}
