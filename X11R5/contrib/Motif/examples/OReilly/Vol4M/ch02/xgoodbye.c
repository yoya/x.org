/*
 * xgoodbye.c - simple program to put up a banner on the display
 *      and callback an application function.
 */

#include <stdio.h>
/*
 * Include files required for all Toolkit programs
 */
#include <X11/Intrinsic.h>  /* Intrinsics Definitions */
#include <Xm/Xm.h>  /* Standard Motif definitions */

/*
 * Public include file for widgets we actually use in this file.
 */
#include <Xm/PushB.h>    /* Motif PushButton Widget */

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

    topLevel = XtVaAppInitialize(
            &app_context,           /* Application context */
            "XGoodbye",             /* Application class */
            NULL, 0,                /* command line option list */
            &argc, argv,            /* command line args */
            NULL,                   /* for missing app-defaults file */
            NULL);                  /* terminate varargs list */

    goodbye = XtVaCreateManagedWidget(
            "goodbye",              /* arbitrary widget name */
            xmPushButtonWidgetClass,     /* widget class from PushB.h */
            topLevel,               /* parent widget*/
            NULL);                  /* terminate varargs list */
        
    XtAddCallback(goodbye, XmNactivateCallback, Quit, 0 /* client_data */);

    /*
     *  Create windows for widgets and map them.
     */
    XtRealizeWidget(topLevel);

    /*
     *  Loop for events.
     */
    XtAppMainLoop(app_context);
}
