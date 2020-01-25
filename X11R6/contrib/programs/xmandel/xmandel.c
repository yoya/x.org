/*
 *    Copyright (c) 1994 Cray Research Inc.
 * 
 * The X Consortium, and any party obtaining a copy of these files from
 * the X Consortium, directly or indirectly, is granted, free of charge, a
 * full and unrestricted irrevocable, world-wide, paid up, royalty-free,
 * nonexclusive right and license to deal in this software and
 * documentation files (the "Software"), including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons who receive
 * copies from any such party to do so.  This license includes without
 * limitation a license to do the foregoing actions under any patents of
 * the party supplying this software to the X Consortium.
 * 
 * 
 * THE SOFTWARE IS PROVIDED "AS IS". EXCEPT AS EXPRESSLY PROVIDED IN
 * THE CONTRIBUTION LETTER, ALL WARRANTIES WITH RESPECT TO THE SOFTWARE
 * ARE, EXPRESS OR IMPLIED, ARE HEREBY DISCLAIMED AND EXCLUDED, INCLUDING
 * WITHOUT LIMITATION ANY WARRANTY OF MERCHANTABILITY, FITNESS FOR A 
 * PARTICULAR PURPOSE OR USE, OR FREEDOM FROM INFRINGEMENT OR VIOLATION
 * OF THIRD PARTY INTELLECTUAL PROPERTY RIGHTS, AND ALL OBLIGATIONS
 * OR LIABILITIES FOR DAMAGES ARISING OUT OF OR IN CONNECTION WITH THE
 * PERFORMANCE OF, OR THE USE OF THE SOFTWARE.
 */

#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xaw/Cardinals.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Paned.h>
#include <X11/Xaw/Box.h>
#include <X11/cursorfont.h>
#include <stdio.h>

Widget drawform;
Widget detaillabel;					 /* in widget ibox */
Widget xlabel, ylabel;					/* in widget bbox */
Cursor crosshair_xcr;
int depth;
int iter;
int maxcpus = 8;
void mandel();

void clear_callback(w, closure, call_data)
    Widget w;
    caddr_t closure;
    caddr_t call_data;
{
    Display *draw_d;
    Window draw_win;

    draw_d = XtDisplay(drawform);
    draw_win = XtWindow(drawform);
    XClearWindow(draw_d, draw_win);
}
void quit_callback(w, closure, call_data)
    Widget w;
    caddr_t closure;
    caddr_t call_data;
{
    exit(0);
}
XtCallbackRec clear_callbacks[] = {
  { (XtCallbackProc) clear_callback, NULL },
  { NULL, NULL },
};
void julia();
XtCallbackRec julia_callbacks[] = {
  { julia, NULL },
  { NULL, NULL },
};
void zoom();
XtCallbackRec zoom_callbacks[] = {
  { zoom, NULL },
  { NULL, NULL },
};
void mooz();
XtCallbackRec mooz_callbacks[] = {
  { mooz, NULL },
  { NULL, NULL },
};
void redo();
XtCallbackRec redo_callbacks[] = {
  { redo, NULL },
  { NULL, NULL },
};
void resetmandel();
XtCallbackRec mandel_callbacks[] = {
  { resetmandel, NULL },
  { NULL, NULL },
};
XtCallbackRec quit_callbacks[] = {
  { (XtCallbackProc) quit_callback, NULL },
  { NULL, NULL },
};

void detail();
XtCallbackRec detail_callbacks[] = {
  { detail, NULL },
  { NULL, NULL },
};
void reset();
XtCallbackRec reset_callbacks[] = {
  { reset, NULL },
  { NULL, NULL },
};


main(argc,argv)
int argc;
char **argv;
{
	Widget toplevel, pane;
	Widget box, ibox, bbox;				/* in widget pane */
	Widget hostlabel;				/* in widget box */
	Widget juliabutton, quitbutton, mandelbutton;	/* in widget box */
	Widget moozbutton, zoombutton, redobutton, clearbutton;	/* in box */
	Widget detailbutton, resetbutton;	/* in widget ibox */
	Display *dp;
	int i;
	char siter[20];
	Arg argies[10];

	toplevel = XtInitialize("Anyname", "Xmandelf", NULL, 0, &argc, argv);
	dp = XtDisplay(toplevel);
	/* do_colormap(dp); */

	depth = DisplayPlanes(dp,DefaultScreen(dp));

	pane = XtCreateManagedWidget("pane", panedWidgetClass, toplevel,
		NULL, ZERO);

	box = XtCreateManagedWidget("box", boxWidgetClass, pane, NULL, ZERO);
	ibox = XtCreateManagedWidget("ibox", boxWidgetClass, pane, NULL, ZERO);

	gethostname(siter, 20);
	i = 0;
        XtSetArg( argies[i], XtNborderWidth, (XtArgVal) 0); i++;
	hostlabel = XtCreateManagedWidget(siter, labelWidgetClass,
		box, argies, i);

        i = 0;
        XtSetArg( argies[i], XtNcallback, (XtArgVal) mandel_callbacks); i++;
	mandelbutton = XtCreateManagedWidget("mandel", commandWidgetClass,
		box, argies, i);

        i = 0;
        XtSetArg( argies[i], XtNcallback, (XtArgVal) zoom_callbacks); i++;
	zoombutton = XtCreateManagedWidget("mandelzoom", commandWidgetClass,
		box, argies, i);

	i = 0;
        XtSetArg( argies[i], XtNcallback, (XtArgVal) mooz_callbacks); i++;
	moozbutton = XtCreateManagedWidget("unzoom", commandWidgetClass,
		box, argies, i);

	i = 0;
        XtSetArg( argies[i], XtNcallback, (XtArgVal) redo_callbacks); i++;
	redobutton = XtCreateManagedWidget("redo", commandWidgetClass,
		box, argies, i);

        i = 0;
        XtSetArg( argies[i], XtNcallback, (XtArgVal) julia_callbacks); i++;
	juliabutton = XtCreateManagedWidget("julia", commandWidgetClass,
		box, argies, i);

        i = 0;
        XtSetArg( argies[i], XtNcallback, (XtArgVal) clear_callbacks); i++;
	clearbutton = XtCreateManagedWidget("clear", commandWidgetClass,
		box, argies, i);

        i = 0;
        XtSetArg( argies[i], XtNcallback, (XtArgVal) quit_callbacks); i++;
	quitbutton = XtCreateManagedWidget("quit", commandWidgetClass,
		box, argies, i);

        i = 0;
	if (depth > 1) iter = 256; else iter = 64;
	sprintf(siter, "%d iterations", iter);
        XtSetArg( argies[i], XtNborderWidth, (XtArgVal) 0); i++;
	detaillabel = XtCreateManagedWidget(siter, labelWidgetClass,
		ibox, argies, i);

        i = 0;
        XtSetArg( argies[i], XtNcallback, (XtArgVal) detail_callbacks); i++;
detailbutton = XtCreateManagedWidget("increase iterations", commandWidgetClass,
		ibox, argies, i);

        i = 0;
        XtSetArg( argies[i], XtNcallback, (XtArgVal) reset_callbacks); i++;
    resetbutton = XtCreateManagedWidget("reset iterations", commandWidgetClass,
		ibox, argies, i);

	i = 0;
	XtSetArg( argies[i], XtNheight, (XtArgVal) 512); i++;
	XtSetArg( argies[i], XtNwidth, (XtArgVal) 512); i++;
	XtSetArg( argies[i], XtNbackground, (XtArgVal) 
		BlackPixel(dp, DefaultScreen(dp))); i++;
	drawform = XtCreateManagedWidget("DrawForm", simpleWidgetClass, pane,
		argies, i);

	i = 0;
	XtSetArg( argies[i], XtNmax, (XtArgVal) 50); i++;
	bbox = XtCreateManagedWidget("box", boxWidgetClass, pane, argies, i);

	i = 0;
	XtSetArg( argies[i], XtNwidth, (XtArgVal) 512); i++;
	XtSetArg( argies[i], XtNresize, (XtArgVal) False); i++;
        XtSetArg( argies[i], XtNborderWidth, (XtArgVal) 0); i++;
	xlabel = XtCreateManagedWidget("", labelWidgetClass,
		bbox, argies, i);

	i = 0;
	XtSetArg( argies[i], XtNwidth, (XtArgVal) 512); i++;
	XtSetArg( argies[i], XtNresize, (XtArgVal) False); i++;
        XtSetArg( argies[i], XtNborderWidth, (XtArgVal) 0); i++;
	ylabel = XtCreateManagedWidget("", labelWidgetClass,
		bbox, argies, i);

	XtRealizeWidget(toplevel);
	crosshair_xcr = XCreateFontCursor(dp, XC_crosshair);
        XDefineCursor(dp, XtWindow(drawform), crosshair_xcr);

	XtMainLoop();
}
