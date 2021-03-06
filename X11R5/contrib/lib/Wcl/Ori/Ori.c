/*
** Copyright (c) 1990 David E. Smyth
**
** Redistribution and use in source and binary forms are permitted
** provided that the above copyright notice and this paragraph are
** duplicated in all such forms and that any documentation, advertising
** materials, and other materials related to such distribution and use
** acknowledge that the software was developed by David E. Smyth.  The
** name of David E. Smyth may not be used to endorse or promote products
** derived from this software without specific prior written permission.
** THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR IMPLIED
** WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF
** MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
*/

/******************************************************************************
**
** Description:	This file contains main() for a Athena Resource Interpreter
**		which allows prototype interfaces to be built from
**		resource files.  The Widget Creation library is used.
**
** Notes:       This program uses the Xrm (X resource management) database
**              for widget tree definition and management.  This program
**              is dependent on the X Athena (Xaw) widget set, ALL the Xaw
**              widget classes are registered by the call to XpRegisterAll().
**
**              This version is slightly DIFFERENT than the original Mri
**              provided by David Smyth with Wcl 1.05. It uses the first
**              command line argument argv[1] as an application class, thus
**              directly selecting controlled user interface
**
**              09/27/1991 Martin Brunecky
**
******************************************************************************/

/******************************************************************************
**   Include_files.
******************************************************************************/

#include <Wc/WcCreate.h>
#include <Xol/OpenLook.h>

/******************************************************************************
*   MAIN function
******************************************************************************/

main ( argc, argv )
    int   argc;
    char* argv[];
{   
    XtAppContext app;
    Display     *dpy;
    Widget       appShell;

    if ( argc < 2 ) {
      printf("usage: Ori app-class-name [-dislay ...]\n");
      printf("where app-class-name specifies the app-defaults resource file\n");
      exit(1);
    }

    /* XtInitialize uses argv[0] as shell instance name */
    argv[0] = "Ori";   /* Use the old name for compatibility reasons */

    /*  -- Intialize Toolkit creating the application shell */
    /*  -- OlInitialize must be used rather than XtInitialize */
    appShell = OlInitialize ( "Ori", argv[1], NULL, 0, &argc, argv );
    app      = XtWidgetToApplicationContext(appShell);

    /*  -- Register all Athena and Public widget classes */
    XopRegisterAll ( app );

    /*  -- Create widget tree below toplevel shell using Xrm database */
    if ( WcWidgetCreation ( appShell )  != 0 )
       exit (1);

    /*  -- Realize the widget tree and enter the main application loop */
    XtRealizeWidget ( appShell );
    XtMainLoop ( );
}
