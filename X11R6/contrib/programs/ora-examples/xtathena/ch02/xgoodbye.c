/*
 * xgoodbye.c - simple program to put up a banner on the display
 * 		and callback an application function.
 *
 *    The X Consortium, and any party obtaining a copy of these files from
 *    the X Consortium, directly or indirectly, is granted, free of charge, a
 *    full and unrestricted irrevocable, world-wide, paid up, royalty-free,
 *    nonexclusive right and license to deal in this software and
 *    documentation files (the "Software"), including without limitation the
 *    rights to use, copy, modify, merge, publish, distribute, sublicense,
 *    and/or sell copies of the Software, and to permit persons who receive
 *    copies from any such party to do so.  This license includes without
 *    limitation a license to do the foregoing actions under any patents of
 *    the party supplying this software to the X Consortium.
 */

#include <stdio.h>
/*
 * Include files required for all Toolkit programs
 */
#include <X11/Intrinsic.h>	/* Intrinsics Definitions */
#include <X11/StringDefs.h>	/* Standard Name-String definitions */

/*
 * Public include file for widgets we actually use in this file.
 */
#include <X11/Xaw/Command.h>    /* Athena Command Widget */

/*
 * Quit button callback function
 */
/* ARGSUSED */
void Quit(w, client_data, call_data)
Widget w;
XtPointer client_data, call_data;
{ 
	fprintf(stderr, "It was nice knowing you.\n");
	exit(0); 
}

main(argc, argv)
int argc;
char **argv;
{
	XtAppContext app_context;
	Widget topLevel, goodbye;

        XtSetLanguageProc(NULL, (XtLanguageProc)NULL, NULL);

    topLevel = XtVaAppInitialize(
        &app_context,           /* Application context */
		"XGoodbye",             /* Application class */
        NULL, 0,                /* command line option list */
        &argc, argv,            /* command line args */
        NULL,                   /* for missing app-defaults file */
        NULL);                  /* terminate varargs list */

	goodbye = XtVaCreateManagedWidget(
		"goodbye",              /* arbitrary widget name */
		commandWidgetClass,     /* widget class from Command.h */
		topLevel,               /* parent widget*/
		NULL);                  /* terminate varargs list */
		
	XtAddCallback(goodbye, XtNcallback, Quit, 0 /* client_data */);

	/*
	 *  Create windows for widgets and map them.
	 */
	XtRealizeWidget(topLevel);

	/*
	 *  Loop for events.
	 */
	XtAppMainLoop(app_context);
}
