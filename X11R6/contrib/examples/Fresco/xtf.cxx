#include <X11/Fresco/Impls/XtIntrinsic.h>
#include <X11/Fresco/Impls/Xdefs.h>
#include <X11/StringDefs.h>
#include <X11/cursorfont.h>
#include <X11/Xatom.h>
#include <X11/Shell.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/AsciiText.h>
#include <X11/Xaw/Paned.h>
#include <X11/Xaw/Viewport.h>
#include <X11/Xaw/Cardinals.h>
#include <X11/Fresco/Impls/Xundefs.h>

#include <X11/Fresco/figures.h>
#include <X11/Fresco/layouts.h>
#include <X11/Fresco/Impls/XtfFresco.h>
#include <X11/Fresco/Impls/XtfViewer.h>
#include <stdio.h>
#include <stdlib.h>

#if XtSpecificationRelease > 4
typedef int argc_t;
#else
typedef Cardinal argc_t;
#endif

static Option options[] = {
    { "-v", "*traceEvents", Option::implicit, "on" },
    { nil }
};

static void exit_callback(Widget, XtPointer, XtPointer) {
    exit(0);
}

int main(int argc, char* argv[]) {
    XtToolkitInitialize();

    argc_t args = argc_t(argc);
    XtAppContext app_context = XtCreateApplicationContext();
    XDisplay* xdisplay = XtOpenDisplay(app_context, NULL,
	"xtcircle",    		/* application name */
	"XtCircle",             /* application class */
	NULL, 0,                /* options */
	&args, argv);           /* command line parameters */

    if (xdisplay == NULL) {
	fprintf(stderr, "Unable to open display\n");
	exit(0);
    }

    Arg arglist[2] ;

    XtSetArg(arglist[0], XtNallowShellResize, FALSE) ;
    Widget toplevel = XtAppCreateShell(
	"XtTest", NULL,
	applicationShellWidgetClass,
	xdisplay, arglist, 1
    );

    Widget paned = XtCreateManagedWidget(
	"paned", panedWidgetClass, toplevel, NULL, 0
    );
    Widget buttons = XtCreateManagedWidget(
	"buttons", panedWidgetClass, paned, NULL, 0
    );
    Widget quit_button = XtCreateManagedWidget(
	"quit", commandWidgetClass, buttons, NULL, 0
    );
    XtAddCallback(quit_button, XtNcallback, exit_callback, NULL);
    
    Widget fw = XtCreateManagedWidget(
	"circle", XtfviewerWidgetClass, paned, nil, 0
    );

    XtfFresco* f = XtfFresco_init(
	app_context, xdisplay, "XtTest", args, argv, options
    );

    FigureKit_var figures = f->figure_kit();
    f->init_widget_glyph(
	fw, figures->figure_root(
	    figures->circle(
		FigureKit::fill, figures->default_style(), 0.0, 0.0, 100.0
	    )
	)
    );


    XtRealizeWidget(toplevel);
    XtMapWidget(fw);
    XtAppMainLoop(app_context);

    Fresco::unref(f);
    return 0;
}
