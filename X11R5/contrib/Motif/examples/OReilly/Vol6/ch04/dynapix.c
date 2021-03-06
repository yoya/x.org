/* Written by Dan Heller.  Copyright 1991, O'Reilly && Associates.
 * This program is freely distributable without licensing fees and
 * is provided without guarantee or warrantee expressed or implied.
 * This program is -not- in the public domain.
 */

/* dynapix.c -- Display a bitmap in a MainWindow, but allow the user
 * to change the bitmap and its color dynamically.  The design of the
 * program is structured on the pulldown menus of the menubar and the
 * callback routines associated with them.  To allow the user to choose
 * a new bitmap, the "New" button popsup a FileSelectionDialog where
 * a new bitmap file can be chosen.
 */
#include <Xm/MainW.h>
#include <Xm/Label.h>
#include <Xm/MessageB.h>
#include <Xm/FileSB.h>

/* Globals: the toplevel window/widget and the label for the bitmap.
 * "colors" defines the colors we use, "cur_color" is the current
 * color being used, and "cur_bitmap" references the current bitmap file.
 */
Widget toplevel, label;
String colors[] = { "Black", "Red", "Green", "Blue" };
Pixel cur_color;
char cur_bitmap[1024] = "xlogo64"; /* make large enough for full pathnames */

main(argc, argv)
int argc;
char *argv[];
{
    Widget main_w, menubar, menu, widget;
    XtAppContext app;
    Pixmap pixmap;
    XmString file, edit, help, new, quit, red, green, blue, black;
    void file_cb(), change_color(), help_cb();

    /* Initialize toolkit and parse command line options. */
    toplevel = XtVaAppInitialize(&app, "Demos",
        NULL, 0, &argc, argv, NULL, NULL);

    /* main window contains a MenuBar and a Label displaying a pixmap */
    main_w = XtVaCreateManagedWidget("main_window",
        xmMainWindowWidgetClass,   toplevel,
        XmNscrollBarDisplayPolicy, XmAS_NEEDED,
        XmNscrollingPolicy,        XmAUTOMATIC,
        NULL);

    /* Create a simple MenuBar that contains three menus */
    file = XmStringCreateSimple("File");
    edit = XmStringCreateSimple("Edit");
    help = XmStringCreateSimple("Help");
    menubar = XmVaCreateSimpleMenuBar(main_w, "menubar",
        XmVaCASCADEBUTTON, file, 'F',
        XmVaCASCADEBUTTON, edit, 'E',
        XmVaCASCADEBUTTON, help, 'H',
        NULL);
    XmStringFree(file);
    XmStringFree(edit);
    /* don't free "help" compound string yet -- reuse it later */

    /* Tell the menubar which button is the help menu  */
    if (widget = XtNameToWidget(menubar, "button_2"))
        XtVaSetValues(menubar, XmNmenuHelpWidget, widget, NULL);

    /* First menu is the File menu -- callback is file_cb() */
    new = XmStringCreateSimple("New ...");
    quit = XmStringCreateSimple("Quit");
    XmVaCreateSimplePulldownMenu(menubar, "file_menu", 0, file_cb,
        XmVaPUSHBUTTON, new, 'N', NULL, NULL,
        XmVaSEPARATOR,
        XmVaPUSHBUTTON, quit, 'Q', NULL, NULL,
        NULL);
    XmStringFree(new);
    XmStringFree(quit);

    /* Second menu is the Edit menu -- callback is change_color() */
    black = XmStringCreateSimple(colors[0]);
    red = XmStringCreateSimple(colors[1]);
    green = XmStringCreateSimple(colors[2]);
    blue = XmStringCreateSimple(colors[3]);
    menu = XmVaCreateSimplePulldownMenu(menubar, "edit_menu", 1, change_color,
        XmVaRADIOBUTTON, black, 'k', NULL, NULL,
        XmVaRADIOBUTTON, red, 'R', NULL, NULL,
        XmVaRADIOBUTTON, green, 'G', NULL, NULL,
        XmVaRADIOBUTTON, blue, 'B', NULL, NULL,
        XmNradioBehavior, True,     /* RowColumn resources to enforce */
        XmNradioAlwaysOne, True,    /* radio behavior in Menu */
        NULL);
    XmStringFree(black);
    XmStringFree(red);
    XmStringFree(green);
    XmStringFree(blue);

    /* Initialize menu so that "black" is selected. */
    if (widget = XtNameToWidget(menu, "button_0"))
        XtVaSetValues(widget, XmNset, True, NULL);

    /* Third menu is the help menu -- callback is help_cb() */
    XmVaCreateSimplePulldownMenu(menubar, "help_menu", 2, help_cb,
        XmVaPUSHBUTTON, help, 'H', NULL, NULL,
        NULL);
    XmStringFree(help); /* we're done with it; now we can free it */

    XtManageChild(menubar);

    /* user can still specify the initial bitmap */
    if (argv[1])
        strcpy(cur_bitmap, argv[1]);
    /* initialize color */
    cur_color = BlackPixelOfScreen(XtScreen(toplevel)),

    /* create initial bitmap */
    pixmap = XmGetPixmap(XtScreen(toplevel), cur_bitmap,
        cur_color, WhitePixelOfScreen(XtScreen(toplevel)));

    if (pixmap == XmUNSPECIFIED_PIXMAP) {
        puts("can't create initial pixmap");
        exit(1);
    }

    /* Now create label using pixmap */
    label = XtVaCreateManagedWidget("label", xmLabelWidgetClass, main_w,
        XmNlabelType,   XmPIXMAP,
        XmNlabelPixmap, pixmap,
        NULL);

    /* set the label as the "work area" of the main window */
    XtVaSetValues(main_w,
        XmNmenuBar,    menubar,
        XmNworkWindow, label,
        NULL);

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app);
}

/* Any item the user selects from the File menu calls this function.
 * It will either be "New" (item_no == 0) or "Quit" (item_no == 1).
 */
void
file_cb(w, item_no)
Widget w;     /* menu item that was selected */
int item_no;  /* the index into the menu */
{
    static Widget dialog; /* make it static for reuse */
    extern void load_pixmap();

    if (item_no == 1) /* the "quit" item */
        exit(0);

    /* "New" was selected.  Create a Motif FileSelectionDialog w/callback */
    if (!dialog) {
        dialog = XmCreateFileSelectionDialog(toplevel, "file_sel", NULL, 0);
        XtAddCallback(dialog, XmNokCallback, load_pixmap, NULL);
        XtAddCallback(dialog, XmNcancelCallback, XtUnmanageChild, NULL);
    }
    XtManageChild(dialog);
    XtPopup(XtParent(dialog), XtGrabNone);
}

/* The Ok button was selected from the FileSelectionDialog (or, the user
 * double-clicked on a file selection).  Try to read the file as a bitmap.
 * If the user changed colors, we call this function directly from change_color()
 * to reload the pixmap.  In this case, we pass NULL as the callback struct
 * so we can identify this special case.
 */
void
load_pixmap(dialog, client_data, cbs)
Widget dialog;          /* Ok button was pressed in "dialog" (unused) */
XtPointer client_data;    /* ignored -- NULL passed to XtAddCallback() */
XmFileSelectionBoxCallbackStruct *cbs; /* NULL if called from change_color() */
{
    Pixmap pixmap;
    char *file = NULL;

    if (cbs) {
        if (!XmStringGetLtoR(cbs->value, XmSTRING_DEFAULT_CHARSET, &file))
            return; /* internal error */
        (void) strcpy(cur_bitmap, file);
        XtFree(file); /* free allocated data from XmStringGetLtoR() */
    }

    pixmap = XmGetPixmap(XtScreen(toplevel), cur_bitmap,
        cur_color, WhitePixelOfScreen(XtScreen(toplevel)));

    if (pixmap == XmUNSPECIFIED_PIXMAP)
        printf("can't create pixmap from %s\n", cur_bitmap);
    else {
        Pixmap old;
        XtVaGetValues(label, XmNlabelPixmap, &old, NULL);
        XmDestroyPixmap(XtScreen(toplevel), old);
        XtVaSetValues(label,
            XmNlabelType,   XmPIXMAP,
            XmNlabelPixmap, pixmap,
            NULL);
    }
}

/* called from any of the "Edit" menu items.  Change the color of the
 * current bitmap being displayed.  Do this by calling load_pixmap().
 */
void
change_color(w, item_no)
Widget w;     /* menu item that was selected */
int item_no;  /* the index into the menu */
{
    XColor xcolor, unused;
    Display *dpy = XtDisplay(label);
    Colormap cmap = DefaultColormapOfScreen(XtScreen(label));

    if (XAllocNamedColor(dpy, cmap, colors[item_no], &xcolor, &unused) == 0 ||
        cur_color == xcolor.pixel)
        return;

    cur_color = xcolor.pixel;
    load_pixmap(w, NULL, NULL);
}

#define MSG \
"Use the FileSelection dialog to find bitmap files to\n\
display in the scrolling area in the main window.  Use\n\
the edit menu to display the bitmap in different colors."

/* The help button in the help menu from the menubar was selected.
 * Display help information defined above for how to use the program.
 * This is done by creating a Motif information dialog box.  Again,
 * make the dialog static so we can reuse it.
 */
void
help_cb()
{
    static Widget dialog;

    if (!dialog) {
        Arg args[1];
        XmString msg = XmStringCreateLtoR(MSG, XmSTRING_DEFAULT_CHARSET);
        XtSetArg(args[0], XmNmessageString, msg);
        dialog = XmCreateInformationDialog(toplevel, "help_dialog", args, 1);
    }
    XtManageChild(dialog);
    XtPopup(XtParent(dialog), XtGrabNone);
}
