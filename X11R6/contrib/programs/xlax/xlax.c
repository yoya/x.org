/*
 *   Copyright (c) 1994    Frank Adelstein
 *
 *   The X Consortium, and any party obtaining a copy of these files from
 *   the X Consortium, directly or indirectly, is granted, free of charge, a
 *   full and unrestricted irrevocable, world-wide, paid up, royalty-free,
 *   nonexclusive right and license to deal in this software and
 *   documentation files (the "Software"), including without limitation the
 *   rights to use, copy, modify, merge, publish, distribute, sublicense,
 *   and/or sell copies of the Software, and to permit persons who receive
 *   copies from any such party to do so.  This license includes without
 *   limitation a license to do the foregoing actions under any patents of
 *   the party supplying this software to the X Consortium.
 *
 */


/*
 * Neat program to send typed text to multiple windows.
 *
 * by Frank Adelstein
 *
 */

#include "xlax.h"
#include "vroot.h"

XtAppContext
  Appcon;

Widget
  Toplevel;

GC
  drawGC,
  xorGC;

Windows_t
  Windows[50];
  
int 
  WindowIndex;

void DoNothing() {
  /* stupid, but used to override button click default actions */
  return;
}

void ErrorHandler(mydisp, myerr) 
Display *mydisp;
XErrorEvent *myerr;

{
  char msg[80];
  XGetErrorText(mydisp, myerr->error_code, msg, 80);
  (void) fprintf(stderr, "%s\n", msg);
  if (myerr->error_code == BadWindow) {
    return;
  } else {
    (void) fprintf(stderr, 
               "Fatal!!  We got errors: error [%d] request [%d] minor [%d]\n",
               myerr->error_code, myerr->request_code, myerr->minor_code); 
    exit(1); 
  } 
}

main (argc, argv)
int    argc;
char **argv;

{

  void DoNothing();
  static XtActionsRec TextActions[] = {
    {"DoNothing", DoNothing},
    {NULL, NULL}
  };

  /* initialize toolkit */
  Toplevel = XtAppInitialize (&Appcon, TITLE, NULL, 0, &argc, argv,
			      NULL, NULL, 0);

  XtAppAddActions(Appcon, TextActions, XtNumber(TextActions));
  XSetErrorHandler(ErrorHandler);

  /* do all the dirty work */
  SetupInterface();

  /* sit back and process events */
  XtAppMainLoop(Appcon);
}

void
SetupInterface()

{
  int cnt;

  Arg args[15];

  Widget
    Frame,
    thing,
    quit,
    sendit,
    view,
    draw;

  Display
    *display;

  /* create a frame widget to hold things */
  cnt = 0;
  Frame = XtCreateManagedWidget ("Frame", formWidgetClass, 
				 Toplevel, args, cnt);

  XtAddEventHandler (Frame, KeyPressMask, False, keyboardCB, NULL);

  cnt = 0;
  XtSetArg (args[cnt], XtNborderWidth, 2);                  cnt++;
  XtSetArg (args[cnt], XtNlabel, "Add Windows");            cnt++;

  thing = XtCreateManagedWidget ("addbutton", 
				 commandWidgetClass,
				 Frame, args, cnt);
  XtAddCallback (thing, XtNcallback, selectCB, NULL);

  cnt = 0;
  XtSetArg (args[cnt], XtNborderWidth, 2);                  cnt++;
  XtSetArg (args[cnt], XtNfromVert, thing);                 cnt++;
  XtSetArg (args[cnt], XtNlabel, "Send String");            cnt++;

  sendit = XtCreateManagedWidget ("senditbutton", 
				  commandWidgetClass,
				  Frame, args, cnt);
  XtAddCallback (sendit, XtNcallback, senditCB, NULL);

  cnt = 0;
  XtSetArg (args[cnt], XtNborderWidth, 2);                  cnt++;
  XtSetArg (args[cnt], XtNlabel, "Kill Window");            cnt++;
  XtSetArg (args[cnt], XtNfromVert, sendit);                cnt++;

  thing = XtCreateManagedWidget ("killbutton", 
				 commandWidgetClass,
				 Frame, args, cnt);
  XtAddCallback (thing, XtNcallback, killCB, NULL);

  cnt = 0;
  XtSetArg (args[cnt], XtNborderWidth, 2);                  cnt++;
  XtSetArg (args[cnt], XtNfromVert, thing);                 cnt++;
  XtSetArg (args[cnt], XtNlabel, "Quit");                   cnt++;

  quit = XtCreateManagedWidget ("quitbutton", 
				 commandWidgetClass,
				 Frame, args, cnt);
  XtAddCallback (quit, XtNcallback, quitCB, NULL);

  /* create a viewport widget and a frame widget to hold things */
  cnt = 0;
  XtSetArg (args[cnt], XtNfromHoriz, sendit);                 cnt++;
  XtSetArg (args[cnt], XtNborderWidth, 2);                    cnt++;
  XtSetArg(args[cnt], XtNwidth,  400);                        cnt++;
  XtSetArg(args[cnt], XtNheight,  150);                       cnt++;
  XtSetArg(args[cnt], XtNallowHoriz, True);  cnt++;
  XtSetArg(args[cnt], XtNallowVert,  True);  cnt++;
  view = XtCreateManagedWidget("View", viewportWidgetClass,
                               Frame, args, cnt);

  /* I don't understand why I need to do this, but...*/
  cnt = 0;
  XtSetArg(args[cnt], XtNwidth,  10);                         cnt++;
  XtSetArg(args[cnt], XtNheight, 10);                         cnt++;
  XtSetValues(XtNameToWidget(Toplevel, "*clip"), args, cnt);

  cnt = 0;

  XtSetArg (args[cnt], XtNborderWidth, 2);                    cnt++;
  XtSetArg(args[cnt], XtNwidth,  200);                        cnt++;
  XtSetArg(args[cnt], XtNheight,  150);                       cnt++;
  draw = XtCreateWidget ("drawingarea", formWidgetClass,
			 view, args, cnt);

  XtRealizeWidget (Toplevel);

  return;

}


/*
 * Routine to let user select a window using the mouse
 * (taken from dsimple.c from xwd)
 */

Window Select_Window(dpy)
     Display *dpy;
{
  int status;
  Cursor cursor;
  XEvent event;
  Window target_win = None, root = RootWindow(dpy,DefaultScreen(dpy));
  int buttons = 0;

  /* Make the target cursor */
  cursor = XCreateFontCursor(dpy, XC_crosshair);

  /* Grab the pointer using target cursor, letting it room all over */
  status = XGrabPointer(dpy, root, False,
                        ButtonPressMask|ButtonReleaseMask, GrabModeSync,
                        GrabModeAsync, root, cursor, CurrentTime);
  if (status != GrabSuccess) {
    fprintf(stderr, "Can't grab the mouse.");
    exit (2);
  }

  /* Let the user select a window... */
  while ((target_win == None) || (buttons != 0)) {
    /* allow one more event */
    XAllowEvents(dpy, SyncPointer, CurrentTime);
    XWindowEvent(dpy, root, ButtonPressMask|ButtonReleaseMask, &event);
    switch (event.type) {
    case ButtonPress:
      if (target_win == None) {
        target_win = event.xbutton.subwindow; /* window selected */
        if (target_win == None)  {
	  fprintf(stderr, "target win = None\n");
	  target_win = root;
	}
      }
      buttons++;
      break;
    case ButtonRelease:
      if (buttons > 0) /* there may have been some down before we started */
        buttons--;
       break;
    }
  }

  XUngrabPointer(dpy, CurrentTime);      /* Done with pointer */

  return(target_win);
}

void
selectCB (w, client_data, call_data)
Widget w;
caddr_t  client_data;
caddr_t  call_data;

{
  Display *dpy;
  Window target;
  Widget frame;
  int x, y;
  char *name;

  XtTranslations text_trans;
  static String text_trans_str =
    "<Btn1Down>: DoNothing() \n\
     <Btn2Down>: DoNothing() \n\
     <Btn3Down>: DoNothing() \n\
     <Key>Return: insert-string(\"^M\") \n\
     <Key>BackSpace: insert-string(\"^H\") \n\
     <Key>Delete: insert-string(\"^?\")";

  int cnt;
  Arg args[15];
  
  cnt = 0;
  XtSetArg (args[cnt], XtNlabel, "Adding...");            cnt++;
  XtSetValues(w, args, cnt);
  do {
    XtAppProcessEvent (Appcon, XtIMAll);
  } while (XtAppPending(Appcon));


  dpy = XtDisplay(w);
  while (1) {
    /* keep selecting windows until the xlax window is selected */
    target = Select_Window(dpy);

    /* stupid undocumented kludge */
    target = XmuClientWindow (dpy, target);

    if (target == XmuClientWindow(dpy, XtWindow(Toplevel))) {
       /* we just clicked on ourself...let's avoid an endless loop */
       cnt = 0;
       XtSetArg (args[cnt], XtNlabel, "Add Windows");            cnt++;
       XtSetValues(w, args, cnt);
       return;
    }
    /* just return if we've already got that window yet */
    for (x = 0; x < WindowIndex; x++) 
      {
        if (Windows[x].wind == target) 
	  {
            cnt = 0;
            XtSetArg (args[cnt], XtNlabel, "Add Windows");            cnt++;
            XtSetValues(w, args, cnt);
	    return;
	  }
      }

    XFlush(dpy);
    x = XFetchName(dpy, target, &name);

    /* add a toggle button */
    frame = XtNameToWidget (Toplevel, "*drawingarea");

    cnt = 0;
    XtSetArg (args[cnt], XtNborderWidth, 2);                  cnt++;
    XtSetArg (args[cnt], XtNlabel, name);                     cnt++;
    XtSetArg (args[cnt], XtNfromVert, 
	      (WindowIndex)?Windows[WindowIndex - 1].button:NULL);  cnt++;
    XtSetArg (args[cnt], XtNtop, XtChainTop);                 cnt++;
    XtSetArg (args[cnt], XtNbottom, XtChainTop);              cnt++;
    XtSetArg (args[cnt], XtNleft, XtChainLeft);               cnt++;
    XtSetArg (args[cnt], XtNright, XtChainLeft);              cnt++;
    XtSetArg (args[cnt], XtNwidth, 190);                      cnt++;
    XtSetArg (args[cnt], XtNheight, 20);                      cnt++;
    XtSetArg (args[cnt], XtNstate, True);                     cnt++;
    Windows[WindowIndex].button = XtCreateManagedWidget ("togglebutton", 
						         toggleWidgetClass,
						         frame, args, cnt);
    /* add a toggle button */
    frame = XtNameToWidget (Toplevel, "*drawingarea");

    text_trans = XtParseTranslationTable(text_trans_str);
    cnt = 0;
    XtSetArg (args[cnt], XtNborderWidth, 2);                  cnt++;
    XtSetArg (args[cnt], XtNlabel, name);                     cnt++;
    XtSetArg (args[cnt], XtNfromVert, 
	      (WindowIndex)?Windows[WindowIndex - 1].text:NULL);  cnt++;
    XtSetArg (args[cnt], XtNfromHoriz, 
	      Windows[WindowIndex].button);                   cnt++;
    XtSetArg (args[cnt], XtNtop, XtChainTop);                 cnt++;
    XtSetArg (args[cnt], XtNbottom, XtChainTop);              cnt++;
    XtSetArg (args[cnt], XtNleft, XtChainLeft);               cnt++;
    XtSetArg (args[cnt], XtNright, XtChainLeft);              cnt++;
    XtSetArg (args[cnt], XtNwidth, 190);                      cnt++;
    XtSetArg (args[cnt], XtNheight, 20);                      cnt++;
    XtSetArg (args[cnt], XtNeditType, XawtextEdit);           cnt++;
    Windows[WindowIndex].text = XtCreateManagedWidget ("toggletext", 
						       asciiTextWidgetClass,
						       frame, args, cnt);

    XtOverrideTranslations(Windows[WindowIndex].text, text_trans);

    XtAddCallback (Windows[WindowIndex].button, XtNcallback, 
		   toggleCB, WindowIndex);

    XtAddEventHandler (Windows[WindowIndex].text, ButtonPressMask,
		       False, togglestringCB, WindowIndex);

    XtAddEventHandler (Windows[WindowIndex].text, KeyPressMask,
		       False, stringinputCB, WindowIndex);

    XtUnmanageChild(frame);
    XtManageChild(frame);

    Windows[WindowIndex].wind = target;
    Windows[WindowIndex].eventindex = 0;
    Windows[WindowIndex++].active = 1;

  }  	/* end while (1) */  
}


void
killCB (w, client_data, call_data)
Widget w;
caddr_t  client_data;
caddr_t  call_data;

{
  Display *dpy;
  Window  target;
  Widget  frame;
  int     x, length;

  Arg     args[10];
  int     cnt;

  dpy = XtDisplay(w);
  /* get the selection */
  target = Select_Window(XtDisplay(w));

  /* tell me what that really means */
  target = XmuClientWindow(dpy, target);

  if (target == XmuClientWindow(dpy, XtWindow(Toplevel))) {
    /* it's us..don't bother */
    return;
  } 

  /* otherwise see if that window is in our list */
  for (x = 0; x < WindowIndex; x++) {
    if (Windows[x].wind == target) {
      /* remove it from our list then */
      XtDestroyWidget(Windows[x].button);
      XtDestroyWidget(Windows[x].text);
      frame = XtNameToWidget (Toplevel, "*drawingarea");
      if (x+1 < WindowIndex) {
        cnt = 0;
        XtSetArg(args[cnt], XtNfromVert, 
                 (x) ? Windows[x-1].button : NULL); cnt++;
        XtSetValues(Windows[x+1].button, args, cnt);
        cnt = 0;
        XtSetArg(args[cnt], XtNfromVert, 
                 (x) ? Windows[x-1].text : NULL);   cnt++;
        XtSetValues(Windows[x+1].text, args, cnt);

        length = sizeof(Windows_t) * (WindowIndex-x-1);
        bcopy(&Windows[x+1], &Windows[x], length);
      }
      WindowIndex--;
      XtUnmanageChild(frame);
      XtManageChild(frame);
      break;
    }
  }
}

void
keyboardCB (w, client_data, event)
Widget w;
caddr_t  client_data;
XKeyEvent *event;

{
  int x;

  /* send the keys to every active window that's been selected */
  for (x = 0; x < WindowIndex; x++)
    {
      if (Windows[x].active == 1)
	{
	  event->window = Windows[x].wind;
	  XSendEvent(XtDisplay(w), Windows[x].wind, True, KeyPressMask, event);
	}
    }
  return;
}

void
stringinputCB (w, index, event)
Widget w;
int  index;
XKeyEvent *event;

{
  if (Windows[index].eventindex == MAXEVENTS) {
    /* if we've reached the max, just beep and return */
    XBell(XtDisplay(w), 0);
    return;
  }

  /* save each key event for that window and bump up the counter */
  bcopy (event, 
	 &Windows[index].events[Windows[index].eventindex], 
	 sizeof (XKeyEvent));

  Windows[index].events[Windows[index].eventindex].window 
    = Windows[index].wind;
  Windows[index].eventindex++;
  
}

void
togglestringCB (w, index, event)
Widget w;
int  index;
XButtonEvent *event;

{
  Arg args[10];
  int cnt ;
  char text[500];

  /* clear the text widget if button 1 was pressed */
  if (event->button == 1) {
    Windows[index].eventindex = 0;
    cnt = 0;
    XtSetArg(args[cnt], XtNstring, "");    cnt++;
    XtSetValues(w, args, cnt);
  }
  return;
}

void
toggleCB (w, windex, state)
Widget w;
int  windex;
int  state;

{
  Windows[windex].active = state;
  return;
}

void 
senditCB (w, client_data, call_data)
Widget w;		/* unused */
caddr_t  client_data;	/* unused */
caddr_t  call_data;	/* unused */

{
  int x, y;

  for (x = 0; x < WindowIndex; x++) {
    if (Windows[x].active == 1) {
      for (y = 0; y < Windows[x].eventindex; y++) {
	XSendEvent(XtDisplay(w), Windows[x].wind, True, 
		   KeyPressMask, &Windows[x].events[y]);
      }
    }
  }
  return;
}

void 
quitCB (w, client_data, call_data)
Widget w;		/* unused */
caddr_t  client_data;	/* unused */
caddr_t  call_data;	/* unused */

{
  exit (0);
}
