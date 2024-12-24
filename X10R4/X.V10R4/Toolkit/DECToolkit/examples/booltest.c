

/* File: klpxtest.c - last edit by */
/* langone:	07 Jul 86 10:21 AM */

#include <X/Xlib.h>
#include <strings.h>
#include "Toolkit.h"

extern char *malloc();

/* -----> Global Font Information */


#define map0_width 60
#define map0_height 60
static short map0_bits[] = {
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0xffe0, 0x0001,
   0x0000, 0x0000, 0x003f, 0x0003,
   0x0000, 0xe000, 0x0000, 0x0006,
   0x0000, 0x3800, 0x0000, 0x0018,
   0x0000, 0x0e00, 0x0000, 0x0030,
   0x0000, 0x0380, 0x0000, 0x0060,
   0x0000, 0x00c0, 0x0000, 0x0040,
   0x0000, 0x0060, 0x0000, 0x00c0,
   0x0000, 0x0020, 0x0000, 0x0080,
   0x0000, 0x0010, 0x0000, 0x0080,
   0x0000, 0x0010, 0x0000, 0x0100,
   0xff00, 0x0018, 0x0000, 0x0100,
   0x81c0, 0x000b, 0x0000, 0x0100,
   0x0060, 0x000e, 0x0000, 0x0300,
   0x0010, 0x000c, 0x0000, 0x0200,
   0x0010, 0x0000, 0x0000, 0x0600,
   0x0018, 0x0000, 0x0000, 0x0400,
   0x0008, 0x0000, 0x0000, 0x0400,
   0x0008, 0x0000, 0x0000, 0x0c00,
   0x0004, 0x0000, 0x0000, 0x0800,
   0x0004, 0x0000, 0x0000, 0x0800,
   0xfffc, 0xffff, 0xffff, 0x0fff,
   0xc000, 0x40c0, 0x0180, 0x000c,
   0x4000, 0x4040, 0x0080, 0x0004,
   0x6000, 0x4060, 0x00c0, 0x0006,
   0x2000, 0x6020, 0x0040, 0x0002,
   0x2000, 0x2030, 0x0020, 0x0003,
   0x1000, 0x2010, 0x0020, 0x0001,
   0x1800, 0x1010, 0x0030, 0x0001,
   0x0800, 0x1018, 0x0010, 0x0001,
   0x0800, 0x0808, 0x8018, 0x0001,
   0x0c00, 0x0808, 0x8008, 0x0000,
   0x0400, 0x0c0c, 0x8008, 0x0000,
   0x0400, 0x0404, 0x8008, 0x0000,
   0x0600, 0x0404, 0x800c, 0x0000,
   0x0200, 0x0602, 0x8004, 0x0000,
   0x0200, 0x0202, 0x4004, 0x0000,
   0x0300, 0x0201, 0x4004, 0x0000,
   0x0100, 0x0201, 0x4004, 0x0000,
   0x0100, 0x0101, 0x6002, 0x0000,
   0x8100, 0x0101, 0x2002, 0x0000,
   0x8100, 0x0100, 0x2002, 0x0000,
   0x8080, 0x0180, 0x1002, 0x0000,
   0x8080, 0x0080, 0x1003, 0x0000,
   0x8080, 0x0080, 0x1801, 0x0000,
   0x8080, 0x0080, 0x0801, 0x0000,
   0x4040, 0x0080, 0x0801, 0x0000,
   0x4040, 0x0080, 0x0801, 0x0000,
   0x6040, 0x8040, 0x0401, 0x0000,
   0x2040, 0x8040, 0x0400, 0x0000,
   0x0000, 0x8040, 0x0200, 0x0000,
   0x0000, 0x8040, 0x0200, 0x0000,
   0x0000, 0x0000, 0x0200, 0x0000,
   0x0000, 0x0000, 0x0100, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000};

#define map1_width 60
#define map1_height 60
static short map1_bits[] = {
   0x0000, 0x0180, 0x8008, 0x0000,
   0x0000, 0x0300, 0xc018, 0x0800,
   0x0000, 0x0600, 0x60f8, 0x0c00,
   0x0000, 0x0c00, 0x3f8e, 0x0600,
   0x0000, 0xf000, 0x3803, 0x0300,
   0x0000, 0xc000, 0x7000, 0x0180,
   0x0000, 0x6000, 0x4000, 0x00c0,
   0x0000, 0x3000, 0x8000, 0x0060,
   0x0000, 0x1000, 0x8000, 0x0030,
   0x0000, 0x101c, 0x8000, 0x0011,
   0x0000, 0x13f0, 0x0000, 0x0019,
   0x0000, 0x1e00, 0x0000, 0x000f,
   0x0000, 0x0800, 0x0000, 0x0006,
   0x0000, 0x0800, 0x0000, 0x0002,
   0x0000, 0x0800, 0x0000, 0x0002,
   0x0000, 0x0800, 0x0000, 0x0006,
   0x0000, 0xe800, 0x0003, 0x0004,
   0x0000, 0x1f80, 0x001e, 0x0004,
   0x0000, 0x00f0, 0x0070, 0x0004,
   0x0000, 0x0010, 0x03c0, 0x0004,
   0x0000, 0x0008, 0x0600, 0x0002,
   0x0000, 0x0008, 0x0400, 0x01ff,
   0x0000, 0x0004, 0x8400, 0x0f01,
   0x0000, 0x0004, 0xe800, 0x0000,
   0x0000, 0x0004, 0xf800, 0x0000,
   0x7f80, 0x0004, 0x8800, 0x0001,
   0xc0c0, 0x0003, 0x0800, 0x0003,
   0x0060, 0x0003, 0x1800, 0x0006,
   0x0030, 0x0000, 0x1000, 0x000c,
   0x0010, 0x0000, 0x1000, 0x0018,
   0x0018, 0x0000, 0x1000, 0x0030,
   0x0008, 0x0000, 0x3000, 0x0060,
   0x0008, 0x0000, 0x2000, 0x0180,
   0x000c, 0x0000, 0x2000, 0x0300,
   0x0004, 0x0000, 0x2000, 0x0200,
   0x0004, 0x0000, 0x2000, 0x0000,
   0x0004, 0x0000, 0x6000, 0x0000,
   0xfffc, 0xffff, 0x7fff, 0x0000,
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
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000};

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


CProc(tooldata)
caddr_t tooldata;
{
int *ptr;
int dummy;

    printf("Boolean Toggled\n");
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
    
    Bitmap bitmap0, bitmap1;
    Pixmap button0, button1;
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
	w = XCreateWindow(RootWindow, 0, 0, 400, 250, 2, 
			BlackPixmap, WhitePixmap);

	if (w == 0)
	    printf("test: Error While trying to create main window\n");
	else {

	/* -----> Open local font */
	lfontinfo = XOpenFont("/usr/lib/Xfont/oldeng.onx");


  	/* -----> create the bitmap */
	if ((bitmap0=XStoreBitmap(60, 60, map0_bits))==0)
			{
				printf("bitmap0 not stored\n");
				exit(32);
			}
	if ((bitmap1=XStoreBitmap(60, 60, map1_bits))==0)
			{
				printf("bitmap1 not stored\n");
				exit(32);
			}
	/* -----> create the pixmap */
	if ((button0 = XMakePixmap(bitmap0, BlackPixel, WhitePixel))==0)
	{
		printf("button0 was not created\n");
		exit(32);
	}
	if ((button1 = XMakePixmap(bitmap1, BlackPixel, WhitePixel))==0)
	{
		printf("button1 was not created\n");
		exit(32);
	}


	integerdata = 36;
	extradata = &integerdata;

	pixw = 48;
	pixh = 16;

	ni =2;



	TSetArg (args[0], T_BOOLEAN_TYPE, 0);
	TSetArg (args[1], T_BOOLEAN_TEXT0, "On");
	TSetArg (args[2], T_BOOLEAN_TEXT1, "Off");
        TSetArg (args[3], T_BOOLEAN_BRWIDTH, 2);
	TSetArg (args[4], T_BOOLEAN_PROC, CProc);
	TSetArg (args[5], T_BOOLEAN_FONTINFO, lfontinfo);
	TSetArg (args[6], 0, 0);

  	tw1 = TCreateBoolean(w, args);

	TSetArg (args[0], T_LABEL_STRING, "BOOLEAN --->");
	TSetArg (args[1], 0, 0);

  	tw2 = TCreateLabel(w, args);

	TSetArg (args[0], T_BOOLEAN_TYPE, 1);
	TSetArg (args[1], T_BOOLEAN_PIXMW, 60);
	TSetArg (args[2], T_BOOLEAN_PIXMH, 60);
	TSetArg (args[3], T_BOOLEAN_BRWIDTH, 2);
	TSetArg (args[4], T_BOOLEAN_PROC, CProc);
	TSetArg (args[5], T_BOOLEAN_PIXM0, button0);
	TSetArg (args[6], T_BOOLEAN_PIXM1, button1);
	TSetArg (args[7], 0, 0);

	tw3 = TCreateBoolean(w, args);

	XMoveWindow(tw1, 150, 100);
	XMoveWindow(tw2, 20, 100);
	XMoveWindow(tw3, 230, 100);

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