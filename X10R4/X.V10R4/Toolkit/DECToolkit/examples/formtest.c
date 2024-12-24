/* Sample program for building a form */

#include <stdio.h>
#include <X/Xlib.h>
#include "Toolkit.h"
#include "stop.pic"

extern int MakeForm();

Window w;
Window    wa[10];
int       hints[10];
int       integerdata[3];
Bitmap    stop;
Pixmap    black, white;
Pixmap    stop_sign;
Targ      args[10];

Vertex vlist[] = {  0, 246, 0,		/* Drop shadow */
                  346, 246, 0,
                  346,   0, 0,
                  345,   0, VertexDrawLastPoint};



MouseSpeed(tooldata)
caddr_t tooldata;
{

/*
 *  This procedure changes the mouse acceleration.
 */

    int *ptr;
    int dummy;

    ptr = (int*)tooldata;
    dummy = *ptr;   
    XMouseControl(dummy*2, 3);
}

ExitProc()
{
     exit(2);
}

DefineForm()
{
   TSetArg(args[0], T_LABEL_STRING, "Title of Form");
   TSetArg(args[1], 0, 0);
   wa[0]    = TCreateLabel(w, args);
   hints[0] = MakeFormLayoutHints (0,    20,
                                  0,      0,
                                  CenterX, AbsoluteY,
                                  CurrentW, CurrentH);

   TSetArg(args[0], T_LABEL_STRING, "Mouse Response  ");
   TSetArg(args[1], T_LABEL_BORDERWIDTH, 0);
   TSetArg(args[2], 0, 0);
   wa[1]    = TCreateLabel(w, args);
   hints[1] = MakeFormLayoutHints (50, 50, 0, 0,
                     AbsoluteX, AbsoluteY, CurrentW, CurrentH);

   integerdata[0] = 1;
   TSetArg(args[0], T_COMMAND_TYPE, 0);
   TSetArg(args[1], T_COMMAND_TEXT, "Slow");
   TSetArg(args[2], T_COMMAND_PROC, MouseSpeed);
   TSetArg(args[3], T_COMMAND_TAG, (caddr_t) &integerdata[0]);
   TSetArg(args[4], 0, 0);
   wa[2]          = TCreateCommand(w, args);

   integerdata[1] = 5;
   TSetArg(args[1], T_COMMAND_TEXT, "Medium");
   TSetArg(args[3], T_COMMAND_TAG, (caddr_t) &integerdata[1]);
   wa[3]          = TCreateCommand(w, args);

   integerdata[2] = 10;
   TSetArg(args[1], T_COMMAND_TEXT, "Fast");
   TSetArg(args[3], T_COMMAND_TAG, (caddr_t) &integerdata[2]);
   wa[4]          = TCreateCommand(w, args);

   hints[2] = MakeFormLayoutHints (40, 5, 70, 0,
                     UnchangedX, RelativeY, CurrentW, CurrentH);
   hints[3] = MakeFormLayoutHints (40, 0, 0, 0,
                     UnchangedX, RelativeY, CurrentW, CurrentH);
   hints[4] = MakeFormLayoutHints (40, 0, 0, 0,
                     UnchangedX, RelativeY, CurrentW, CurrentH);

   stop      = XStoreBitmap(stop_width, stop_height, stop_bits);
   stop_sign = XMakePixmap(stop, BlackPixel, WhitePixel);

   TSetArg(args[0], T_COMMAND_TYPE, 1);
   TSetArg(args[1], T_COMMAND_PIXM, stop_sign);
   TSetArg(args[2], T_COMMAND_PROC, ExitProc);
   TSetArg(args[3], T_COMMAND_PIXMW, stop_width);
   TSetArg(args[4], T_COMMAND_PIXMH, stop_height);
   TSetArg(args[5], 0, 0);
   wa[5]         = TCreateCommand(w, args);
   hints[5]      = MakeFormLayoutHints (100, 150, 0, 0,
                     RelativeX, AbsoluteY, CurrentW, CurrentH);

   wa[6]    = 0;
   hints[6] = 0;
}

main()
{
XEvent    event;

   XOpenDisplay ("");

   w = XCreateWindow(RootWindow, 300, 300, 350, 250, 2,
                    BlackPixmap, WhitePixmap);
   if (w == 0) {
        printf ("test: Error While trying to create main window\n");
        exit(3);
   }
   DefineForm();
   TSetArg(args[0], T_FORM_TOOLS, wa);
   TSetArg(args[1], T_FORM_HINTS, hints);
   TSetArg(args[2], 0, 0);
   TMakeForm(w, args);
   XMapWindow (w);
   XDraw(w, vlist, 4, 5, 5, BlackPixel, GXcopy, AllPlanes);
   DisplayForm (w);
   XSelectInput (w, allEvents);

   while (TRUE) {
      XNextEvent (&event);
      TDispatchXEvent(&event);
  }
}

