/*
 * Copyright 1989 O'Reilly and Associates, Inc.
 * See ../Copyright for complete rights and liability information.
 */

/* 
 *  xbitmap1.c - bitmap in main window with help and quit
 */

/*  So that we can use fprintf: */
#include <stdio.h>

/* Standard Toolkit include files: */
#include <X11/Intrinsic.h>
#include <Xm/Xm.h>

/* Public include files for widgets used in this file.  */
#include <Xm/PushBG.h>   /* push button gadget */
#include <Xm/PushB.h>    /* push button */
#include <Xm/MainW.h>    /* main window */
#include <Xm/MessageB.h> /* message box */
#include <Xm/CascadeB.h> /* cascade button */

#include "BitmapEdit.h"

/*
 * The printout routine prints an array of 1s and 0s representing the
 * contents of the bitmap.  This data can be processed into any
 * desired form, including standard X11 bitmap file format.
 */
/* ARGSUSED */
static void 
PrintOut(widget, client_data, call_data)
Widget widget;
XtPointer client_data;   /* cast to bigBitmap */
XtPointer call_data;    /* unused */
{
    Widget bigBitmap = (Widget) client_data;
    int x, y;
    int width_in_cells, height_in_cells;
    char *cell;

    cell = BitmapEditGetArray(bigBitmap, &width_in_cells, 
             &height_in_cells);

    (void) putchar('\n');
    for (y = 0; y < height_in_cells; y++) {
        for (x = 0; x < width_in_cells; x++)
            (void) putchar(cell[x + y * width_in_cells] ? '1' : '0');
        (void) putchar('\n');
    }
    (void) putchar('\n');
}

/* 
 * callback to pop up help dialog widget 
 */
/*ARGSUSED*/
void ShowHelp(w, client_data, call_data)
Widget w;
XtPointer client_data;
XtPointer call_data;
{
    Widget dialog = (Widget) client_data;
    XtManageChild(dialog);
}

/*
 * quit button callback function
 */
/*ARGSUSED*/
void Quit(w, client_data, call_data)
Widget w;
XtPointer client_data, call_data;
{ 
    exit(0); 
}

main(argc, argv)
int argc;
char **argv;
{
    XtAppContext app_context;
    Widget topLevel, mainWindow, menuBar;
    Widget fileButton, fileMenu, quit, getHelp, helpBox;
    Widget temp;
    Widget bigBitmap, output;

    /* never call a Widget variable "exit"! */
    extern exit();

    static XrmOptionDescRec table[] = {
        {"-pw",            "*pixmapWidthInCells",        XrmoptionSepArg, NULL},
        {"-pixmapwidth",   "*pixmapWidthInCells",        XrmoptionSepArg, NULL},
        {"-ph",            "*pixmapHeightInCells",       XrmoptionSepArg, NULL},
        {"-pixmapheight",  "*pixmapHeightInCells",       XrmoptionSepArg, NULL},
        {"-cellsize",      "*cellSizeInPixels",           XrmoptionSepArg, NULL},

    };
    
    topLevel = XtVaAppInitialize( &app_context, "XBitmap1",
            table, XtNumber(table), &argc, argv, NULL, NULL);

    /* create main window */
    mainWindow = XtVaCreateManagedWidget( "mainWindow",
            xmMainWindowWidgetClass, topLevel, NULL);

    /* create menu bar along top inside of main window */
    menuBar = XmCreateMenuBar( mainWindow, "menuBar",
            (ArgList) NULL, (Cardinal) 0);
    XtManageChild(menuBar);

    bigBitmap = XtVaCreateManagedWidget("bigBitmap", 
            bitmapEditWidgetClass, mainWindow, 
            NULL);

    /*  Set MainWindow areas and add tab groups */
    XmMainWindowSetAreas (mainWindow, menuBar, NULL, NULL, NULL,
            bigBitmap);

    /*
     *  CREATE FILE MENU AND CHILDREN
     */

    /* create button that will pop up the menu */
    fileButton = XtVaCreateManagedWidget("fileButton",
            xmCascadeButtonWidgetClass, menuBar, NULL);

    /* create menu (really a Shell widget and RowColumn widget combo) */
    fileMenu = XmCreatePulldownMenu( menuBar,
            "fileMenu", NULL, 0);

    /* create the quit button up in the menu */
    quit = XtVaCreateManagedWidget( "quit",
            xmPushButtonWidgetClass, fileMenu, NULL);

    /* 
     * Specify which menu fileButton will pop up.
     */
    XtVaSetValues(fileButton,
            XmNsubMenuId, fileMenu,
            NULL);

    /* arrange for quit button to call function that exits. */
    XtAddCallback(quit, XmNactivateCallback, Quit, 0);

    /*
     *  CREATE BUTTON TO OUTPUT BITMAP
     */

    /* create button that will pop up the menu */
    output = XtVaCreateManagedWidget( "output",
            xmPushButtonGadgetClass, fileMenu, NULL);

    XtAddCallback(output, XmNactivateCallback, PrintOut, bigBitmap);

    /*
     *  CREATE HELP BUTTON AND BOX
     */

    /* create button that will bring up help popup */
    getHelp = XtVaCreateManagedWidget( "getHelp",
            xmCascadeButtonWidgetClass, menuBar, NULL);

    /* tell menuBar which is the help button 
     * (will be specially positioned. */
    XtVaSetValues(menuBar,
            XmNmenuHelpWidget, getHelp,
            NULL);

    /* create popup that will contain help */
    helpBox = XmCreateMessageDialog( getHelp,
            "helpBox", NULL, 0);

    temp = XmMessageBoxGetChild (helpBox, XmDIALOG_CANCEL_BUTTON);
    XtUnmanageChild (temp);
    temp = XmMessageBoxGetChild (helpBox, XmDIALOG_HELP_BUTTON);
    XtUnmanageChild (temp);

    /* arrange for getHelp button to pop up helpBox */
    XtAddCallback(getHelp, XmNactivateCallback, ShowHelp, helpBox);

    XtRealizeWidget(topLevel);

    XtAppMainLoop(app_context);
}
