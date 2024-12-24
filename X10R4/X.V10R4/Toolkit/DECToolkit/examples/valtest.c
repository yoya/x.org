/* File: geotest.c - last edit by */
/* weissman:	07 Jul 86 10:21 AM */
/* Smokey:	16 Apr 86 9:27 AM */

#include <X/Xlib.h>
#include <strings.h>
#include "Toolkit.h"


extern char *malloc();

#define tagFontName "timrom10b"
#define fontName "helv10"
#define textFontName "6x13p"
#define xMargin 2
#define yMargin 2
#define myStr1Length 100
#define myStr2Length 200


#define ocean_width 60
#define ocean_height 180
static short ocean_bits[] = {
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x1800, 0x0000, 0x0000, 0x0000,
   0x1000, 0x0000, 0x0000, 0x0000,
   0x1000, 0x0000, 0x0000, 0x0000,
   0x1800, 0x0038, 0x0000, 0x0000,
   0x7060, 0x000c, 0x0000, 0x0000,
   0x8cc0, 0x0007, 0x0000, 0x0000,
   0x0780, 0x0002, 0x0000, 0x0000,
   0x0380, 0x0006, 0x0000, 0x0000,
   0x0180, 0x0004, 0x0000, 0x0000,
   0x0080, 0x000c, 0x0000, 0x0000,
   0x00c0, 0x0008, 0x0000, 0x0000,
   0x00cc, 0x00f8, 0x0000, 0x0000,
   0x00f8, 0x000c, 0x0000, 0x0000,
   0x00c0, 0x0004, 0x0000, 0x0000,
   0x0080, 0x0004, 0x0000, 0x0000,
   0x0180, 0x0006, 0x0000, 0x0000,
   0x0700, 0x000f, 0x0000, 0x0000,
   0xfde0, 0x0019, 0x0000, 0x0000,
   0x2030, 0x0030, 0x0000, 0x0000,
   0x2000, 0x0000, 0x0000, 0x0000,
   0x2000, 0x0000, 0x0000, 0x0000,
   0x2000, 0x0000, 0x0000, 0x0000,
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
   0x0000, 0x0000, 0x7800, 0x0000,
   0x0000, 0x0000, 0xce00, 0x0001,
   0x0000, 0x0000, 0x0300, 0x0003,
   0x0000, 0x0000, 0x0180, 0x0006,
   0x0000, 0x0000, 0x00d8, 0x00fc,
   0x0000, 0x0000, 0x006e, 0x0380,
   0x0000, 0x0000, 0x0003, 0x0600,
   0x0000, 0x8000, 0x0001, 0x0400,
   0x0000, 0x8000, 0x0000, 0x0400,
   0x0000, 0xc000, 0xffff, 0x0fff,
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
   0x0000, 0x01e0, 0x0000, 0x0000,
   0x7f80, 0x033c, 0x0000, 0x0000,
   0x40e0, 0x0007, 0x0000, 0x0000,
   0xc030, 0x0001, 0x0000, 0x0000,
   0x8018, 0x0000, 0x0000, 0x0000,
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
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0002, 0x0000, 0x0000,
   0x0000, 0x0002, 0x0000, 0x0000,
   0x0000, 0x0002, 0x0000, 0x0000,
   0x8000, 0x00ff, 0x0000, 0x0000,
   0x8000, 0x00ff, 0x0000, 0x0000,
   0xfc00, 0x7fff, 0x0000, 0x0000,
   0x0800, 0x4000, 0x0000, 0x0000,
   0x1000, 0x2000, 0x0000, 0x0000,
   0x6000, 0x1800, 0x0000, 0x0000,
   0x8000, 0x07ff, 0x0000, 0x0000,
   0xffff, 0xffff, 0xffff, 0x0fff,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0200, 0x0000, 0x0000, 0x0000,
   0x8400, 0x003f, 0x0000, 0x0000,
   0xe800, 0x00ff, 0x0000, 0x0000,
   0xf000, 0x017f, 0x0000, 0x0000,
   0xe800, 0x017f, 0x0000, 0x0000,
   0xe400, 0x00ff, 0x0000, 0x0000,
   0x8200, 0x003f, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x8000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0ba1, 0x0000,
   0x0000, 0x0000, 0x3bba, 0x0000,
   0x0000, 0x0000, 0x5bbc, 0x0000,
   0x0000, 0x0000, 0xfbba, 0x0000,
   0x0000, 0x0000, 0x3fb9, 0x0000,
   0x0000, 0x8018, 0x0fa0, 0x0000,
   0x0000, 0x00fc, 0x0000, 0x0000,
   0x0000, 0x008e, 0x0000, 0x0000,
   0x0000, 0x018e, 0x0000, 0x0000,
   0x8000, 0x0303, 0x0000, 0x0006,
   0xe000, 0x0200, 0x0000, 0x000f,
   0x3000, 0x0400, 0x8000, 0x000f,
   0x1000, 0x0400, 0xc000, 0x000c,
   0x1001, 0x0c00, 0x7000, 0x0038,
   0x1807, 0x0800, 0x1f80, 0x07c0,
   0x0c1c, 0x0800, 0x0780, 0x0c00,
   0x0730, 0x0800, 0x0080, 0x0000,
   0x01c0, 0x3000, 0x00c0, 0x0000,
   0x00c0, 0xe000, 0x007f, 0x0000,
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


static Window wleft, wright, w, labelw1, labelw2;
static Window wa[4];
XEvent *event;
static OpaqueFrame *frame;
static Display *display;
static Pixmap black, white;



VProc(position, tooldata)
float position;
caddr_t tooldata;
{

  int *ptr;
  int dummy;
  float range, range1, range2;
  int intrange;
  Targ args[7];
  char str[9];
	
	/*ptr = (float*)tooldata;
	dummy = *ptr;*/
	
	range1 = 2000;
	range2 = 4000;
	if (position < .820)
		range = range2*(1.0 - (1.0/.820)*position);
	else
		range = range1*( (position-.820)/(1.0-.820) );

	/* -----> Convert floating point number to ASCII */
	intrange = (int)range;

	if (intrange == 0)
		strcpy(str,"sea level");
	else {
   		sprintf(str,"%d", intrange);

		/* ----->Pad blankds for less than 4 digits*/
			if (strlen(str) == 1)
				strcat(str, "   ");
			else if (strlen(str) == 2)
				strcat(str, "  ");
			else if (strlen(str) == 3)
				strcat(str, " ");

		if (position < .820)
			strcat(str, "  FT");
		else
			strcat(str, "FATH");
	}

	TSetArg (args[0], T_LABEL_STRING, str );
	TSetArg (args[1], 0, 0 );
	TSetLabelAttr(labelw1, args);

	
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
FontInfo *fontinfo;
Bitmap oceanbit;
Pixmap oceanpix;
    int     error;
    Window toolwindow, twv, twc, tws;
    Window testw;
    WindowInfo info;
    Status status;
    int integ;
    int *extra;
    Targ args[7];

    display = XOpenDisplay("");
    if (display == 0)
	printf("test: Error While trying to open display\n");
    else {
	event = (XEvent *) malloc(sizeof(XEvent));
	wleft = XCreateWindow(RootWindow, 0, 0, 300, 200, 2, 
			BlackPixmap, WhitePixmap, 0);


	if ( wleft ==0)
	    printf("test: Error While trying to create main window\n");
	else {

	
	fontinfo = XOpenFont("/usr/lib/Xfont/helv12b.onx");

        oceanbit = XStoreBitmap(ocean_width, ocean_height, ocean_bits);
        oceanpix = XMakePixmap(oceanbit, BlackPixel, WhitePixel);

	
	/* ----->Send out tool parameters */

	 TSetArg (args[0], T_VALUATOR_TYPE, Invisible );
	 TSetArg (args[1], T_VALUATOR_ORIENT, Vertical );
	 TSetArg (args[2], T_VALUATOR_MAXDIM, 180);
	 TSetArg (args[3], T_VALUATOR_MINDIM, 60);
	 TSetArg (args[4], T_VALUATOR_PROC, VProc);
	 TSetArg (args[5], 0, 0 );

	 twv = TCreateValuator(wleft, args);

	 
	 XMoveWindow(twv, 20, 20);
	 XMapWindow(twv);

	 /*fontinfo = XOpenFont("/usr/lib/Xfont/9x15.onx");*/

	 TSetArg (args[0], T_LABEL_STRING, "sea level" );
	 TSetArg (args[1], T_LABEL_JUSTIFY, 0);
	 TSetArg (args[2], 0, 0 );
	 labelw1 = TCreateLabel(wleft, args);


	 XMoveWindow(labelw1, 50, 0);
	 XMapWindow(labelw1);

	 TSetArg (args[0], T_LABEL_STRING, "<--- VALUATOR" );
	 TSetArg (args[1], T_LABEL_JUSTIFY, 0);
	 TSetArg (args[2], 0, 0 );
	 labelw2 = TCreateLabel(wleft, args);


	 XMoveWindow(labelw2, 170,75);
	 XMapWindow(labelw2);

	/* -----> Map the parent window */

	XMapWindow(wleft);
	XPixmapPut(wleft, 0, 0, 50, 30, 60, 180, oceanpix,
		GXcopy, AllPlanes);

	XFlush();
	XWarpMouse(twv, 30, 135);
	
	XFlush();

	
	/* -----> Goto to process event loop */
	ProcessEvent();



	}
    }
}










