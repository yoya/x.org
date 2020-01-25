#ifndef lint
static char rcsid[] = "$Header: xboxes.c,v 1.12 88/02/14 15:08:38 rws Exp $";
#endif	lint

#include <stdio.h>
#include <X11/Intrinsic.h>
#include <X11/Atoms.h>
#include <X11/Label.h>
#include <X11/Command.h>
#include <X11/Box.h>
#include <X11/Viewport.h>
#include <X11/Cardinals.h>

static XrmOptionDescRec options[] = {
    {"-scroll", "scroll",	XrmoptionNoArg, "True"},
};


static Boolean scroll;

static XtResource resources[] = {
    {"scroll", "Scroll", XtRBoolean, sizeof(Boolean),
	 (Cardinal)&scroll, XtRString, "False"},
};

static Widget toplevel;

Syntax(call)
	char *call;
{
    fprintf( stderr, "Usage: %s\n", call );
}

/* ARGSUSED */
void callback(widget,closure,callData)
    Widget widget;
    caddr_t closure;
    caddr_t callData;
{
 (void) fprintf(stderr,"command callback\n");
  XtDestroyWidget(toplevel);
  exit(0);
}

void main(argc, argv)
    unsigned int argc;
    char **argv;
{
    Widget viewport, box, box1, box2, box3;

    static XtCallbackRec callbackList[] = { {callback, NULL}, {NULL, NULL} };
    static Arg viewArgs[] = { {XtNallowVert, True}, {XtNallowHoriz, True} };
    static Arg arg[] = { {XtNcallback,(XtArgVal)callbackList} };


    toplevel = XtInitialize( NULL, "Demo", options, XtNumber(options),
			     &argc, argv);

    if (argc != 1) Syntax(argv[0]);

    XtGetApplicationResources( toplevel, (caddr_t)NULL,
			       resources, XtNumber(resources),
			       NULL, ZERO );

    if (scroll)
	viewport = XtCreateManagedWidget(
			 "view", viewportWidgetClass,
			 toplevel, viewArgs, XtNumber(viewArgs));
    else
	viewport = toplevel;

    box = XtCreateManagedWidget("outerbox", boxWidgetClass,
				viewport, NULL, ZERO);

    box1 = XtCreateManagedWidget("box1", boxWidgetClass,
				 box, NULL, ZERO);

    box2 = XtCreateManagedWidget("box2", boxWidgetClass,
				 box, NULL, ZERO);

    box3 = XtCreateManagedWidget("box3", boxWidgetClass,
				 box, NULL, ZERO);

    XtCreateManagedWidget("label1", labelWidgetClass, box1, NULL, ZERO);
    XtCreateManagedWidget("label2", labelWidgetClass, box2, NULL, ZERO);
    XtCreateManagedWidget("label3", labelWidgetClass, box3, NULL, ZERO);

    XtCreateManagedWidget("quit", commandWidgetClass, box1, arg, ONE);
    XtCreateManagedWidget("command2", commandWidgetClass, box2, NULL, ZERO);
    XtCreateManagedWidget("command3", commandWidgetClass, box3, NULL, ZERO);
    
    XtRealizeWidget(toplevel);
    XtMainLoop();
}
