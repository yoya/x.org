/*
 * xexit - X logout manager
 *
 * $Id: xexit.c,v 1.2 1994/06/06 14:29:18 stumpf Exp $
 *
 * Copyright (c) 1992-1994   Markus Stumpf
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
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE X CONSORTIUM OR THE AUTHOR BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
 * THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 
 * Except as contained in this notice, the name of the X Consortium shall
 * not be used in advertising or otherwise to promote the sale, use or
 * other dealings in this Software without prior written authorization
 * from the X Consortium.
 *
 * Author:  Markus Stumpf, Technische Universitaet Muenchen
 *          <stumpf@informatik.tu-muenchen.de>
 *
 * Filename: xexit.c
 * Date:     Wed, 07 Jan 1992  16:55:31
 * Creator:  Markus Stumpf <Markus.Stumpf@Informatik.TU-Muenchen.DE>
 *
 * $Source: /usr/wiss/stumpf/src/CVS/xexit/xexit.c,v $
 */

#ifndef lint
static char *RCSId ="$Id: xexit.c,v 1.2 1994/06/06 14:29:18 stumpf Exp $";
#endif /* lint */

#include "xexit.h"

extern void DeleteAllWindows _P((Display *));

static void exitApplCBF _P((Widget, XtPointer, XtPointer));
static void confirmExitCBF _P((Widget, XtPointer, XtPointer));
static void cancelConfirmExitCBF _P((Widget, XtPointer, XtPointer));
static void versionPopdownCBF _P((Widget, XtPointer, XtPointer));
static void PositionOnScreen _P((Widget));
void main       _P((int, char *[]));

Widget	toplevel;

static char    *fallback_resources[]=
{
    "*confirm.borderWidth:      		4",
    "*confirmCancel.fromHoriz:  		confirmOk",
    "*confirmCancel.fromVert:   		confirmWarn",
    "*confirmCancel.label:      		continue",
    "*confirmLabel.Font:        -schumacher-clean-bold-r-*-*-14-*-*-*-*-*-*-*",
    "*confirmLabel.borderWidth:			0",
    "*confirmLabel.fromHoriz:   		confirmWarn",
    "*confirmLabel.label:       		Session ...",
    "*confirmOk.fromVert:       		confirmWarn",
    "*confirmOk.label:  			terminate",
    "*confirmWarn.bitmap:       		Excl",
    "*confirmWarn.borderWidth:  		0",
    "*exitButton.label:     			Terminate\\nSession",
    "*exitButton.bitmap:     			ExitSess.xbm",
    "*exitButton.justify:     			center",
    "*versionForm.defaultDistance:		0",
    "*versionLabel.highlightThickness:		0",
    NULL
};

static XrmOptionDescRec options[]=
{
    {XtOnoConfirm, XtORnoConfirm, XrmoptionNoArg, "false"},
    {XtOshowVersion, XtORshowVersion, XrmoptionNoArg, "true"}
};

#define MOffset(a)      XtOffset(xexit_resourcePtr, a)
static XtResource resources[]=
{
    {XtNconfirmExit, XtCConfirmExit, XtRBoolean, sizeof(Boolean),
     MOffset(confirm_exit), XtRString, "true"},
    {XtNshowVersion, XtCShowVersion, XtRBoolean, sizeof(Boolean),
     MOffset(print_version), XtRString, "false"}
};
#undef MOffset

xexit_resourceRec app_resources;

/* ------------------------------------------------------------------------
 * Position a popup window under the cursor but totally
 * viewable on the screen
 */
#if NeedFunctionPrototypes
static void
PositionOnScreen(Widget widget)
#else
static void
PositionOnScreen(widget)
    Widget          widget;
#endif				/* NeedFunctionPrototypes */
{
    Window          root, child;
    int             root_x, root_y, win_x, win_y;
    unsigned int    mask;
    Bool            sameScreen;
    int             dpywidth, dpyheight;
    Dimension       e_width, e_height, e_border_width;

    sameScreen = XQueryPointer(XtDisplay(widget),
			       XtWindow(widget),
			       &root, &child,
			       &root_x, &root_y,
			       &win_x, &win_y,
			       &mask);

    dpywidth = XDisplayWidth(XtDisplay(widget),
			     DefaultScreen(XtDisplay(widget)));
    dpyheight = XDisplayHeight(XtDisplay(widget),
			       DefaultScreen(XtDisplay(widget)));
    XtVaGetValues(widget,
		  XtNwidth, &e_width,
		  XtNheight, &e_height,
		  XtNborderWidth, &e_border_width,
		  NULL, NULL);

    e_width += 2 * e_border_width;
    e_height += 2 * e_border_width;

    root_x -= e_border_width;
    root_y -= e_height / 2;

    if ((root_x + e_width) > dpywidth)
	root_x = dpywidth - e_width;
    if ((root_y + e_height) > dpyheight)
	root_y = dpyheight - e_height;

    root_x = (root_x < 0 ? 0 : root_x);
    root_y = (root_y < 0 ? 0 : root_y);
    XtVaSetValues(widget,
		  XtNx, (Position) root_x,
		  XtNy, (Position) root_y,
		  NULL, NULL);
}


/* ------------------------------------------------------------------------
 * This is the routine that is called on press of the endbutton; exits
 * program with status 0
 */
#if NeedFunctionPrototypes
static void
exitApplCBF(Widget widget,
	    XtPointer client_d,
	    XtPointer call_d)
#else
static void
exitApplCBF(widget, client_d, call_d)
    Widget          widget;
    XtPointer       client_d;
    XtPointer       call_d;
#endif				/* NeedFunctionPrototypes */
{
    Widget          confirmW = (Widget) client_d;

    XDestroyWindow(XtDisplay(toplevel), XtWindow(toplevel));
    if (toplevel != confirmW)
	XDestroyWindow(XtDisplay(confirmW), XtWindow(confirmW));
    DeleteAllWindows(XtDisplay(widget));
    XtDestroyWidget(toplevel);
    exit(0);
}


/* ------------------------------------------------------------------------
 * pop down the version window
 */
#if NeedFunctionPrototypes
static void
versionPopdownCBF(Widget widget,
		  XtPointer client_d,
		  XtPointer call_d)
#else
static void
versionPopdownCBF(widget, client_d, call_d)
    Widget          widget;
    XtPointer       client_d;
    XtPointer       call_d;
#endif				/* NeedFunctionPrototypes */
{
    Widget          versionW = (Widget) client_d;

    XtPopdown(versionW);
    XtDestroyWidget(versionW);
}


/* ------------------------------------------------------------------------
 * User does not want to terminate;
 * pop down dialog box
 */
#if NeedFunctionPrototypes
static void
cancelConfirmExitCBF(Widget widget,
		     XtPointer client_d,
		     XtPointer call_d)
#else
static void
cancelConfirmExitCBF(widget, client_d, call_d)
    Widget          widget;
    XtPointer       client_d;
    XtPointer       call_d;
#endif				/* NeedFunctionPrototypes */
{
    Widget          confirmW = (Widget) client_d;

    XtPopdown(confirmW);
}


/* ------------------------------------------------------------------------
 * Ask user, if s/he really wants to terminate
 */
#if NeedFunctionPrototypes
static void
confirmExitCBF(Widget widget,
	       XtPointer client_d,
	       XtPointer call_d)
#else
static void
confirmExitCBF(widget, client_d, call_d)
    Widget          widget;
    XtPointer       client_d;
    XtPointer       call_d;
#endif				/* NeedFunctionPrototypes */
{
    Widget          confirmW = (Widget) client_d;

    XtRealizeWidget(confirmW);
    PositionOnScreen(confirmW);
    XtPopupSpringLoaded(confirmW);
}


/* ------------------------------------------------------------------------
 * ------------------------------------------------------------------------
 * ------------------------------------------------------------------------
 */
#if NeedFunctionPrototypes
void
main(int argc,
     char *argv[])
#else
void
main(argc, argv)
    int             argc;
    char           *argv[];
#endif				/* NeedFunctionPrototypes */
{
    Display        *mDisplay;
    XtAppContext    app_ctxt;
    XtCallbackProc  the_callback;
    Widget          exitButton, form, label, warn, ok, cancel,
                    queue, versionW;
    Dimension       warn_height, label_height;
    String          vers_str;
    Cardinal	    i;


    for (i=1; i<argc; i++)
    {
	if (0 == strcmp(argv[i], "-version"))
	{
	    fprintf(stdout, "%s: version %s\n", XAPPL_NAME, VERSION);
	    exit(0);
	}
    }

#ifdef XtSpecificationRelease
# if XtSpecificationRelease >= 5
    XtSetLanguageProc(NULL, NULL, NULL);
# endif
#endif                          /* XtSpecificationRelease */

    XtToolkitInitialize();
    app_ctxt = XtCreateApplicationContext();
    XtAppSetFallbackResources(app_ctxt, fallback_resources);

    mDisplay = XtOpenDisplay(app_ctxt, NULL, XAPPL_NAME, XAPPL_CLASS,
			     options, XtNumber(options), &argc, argv);
    if (mDisplay == (Display *) NULL)
    {
	fprintf(stderr, "%s: Cannot open display \"%s\" \n",
		argv[0], XDisplayName(NULL));
	XtDestroyApplicationContext(app_ctxt);
	exit(1);
    }
    toplevel =
	XtVaAppCreateShell(XAPPL_NAME, XAPPL_CLASS,
			   applicationShellWidgetClass, mDisplay,
			   XtNallowShellResize, True,
			   NULL);

    XtGetApplicationResources(toplevel, &app_resources, resources,
			      XtNumber(resources), NULL, 0);

    if (app_resources.print_version)
    {
	fprintf(stdout, "%s: version %s\n", XAPPL_NAME, VERSION);
	vers_str = (String) malloc(strlen(XAPPL_NAME) * strlen(AUTHOR_INFO)
				    + 100);
	sprintf(vers_str, "%s: version %s\n%s",
		XAPPL_NAME, VERSION, AUTHOR_INFO);

	versionW =
	    XtVaCreatePopupShell("version",
				 overrideShellWidgetClass,
				 toplevel,
				 NULL);
	queue =
	    XtVaCreateManagedWidget("versionForm",
				    formWidgetClass,
				    versionW,
				    NULL);
	queue =
	    XtVaCreateManagedWidget("versionLabel",
				    commandWidgetClass,
				    queue,
				    XtNlabel, vers_str,
				    NULL);
	XtAddCallback(queue, XtNcallback,
		      versionPopdownCBF, (XtPointer) versionW);
    }

    if (app_resources.confirm_exit)
    {
	the_callback = confirmExitCBF;

	queue =
	    XtVaCreatePopupShell("confirm",
				 overrideShellWidgetClass,
				 toplevel,
				 NULL);
	form =
	    XtVaCreateManagedWidget("confirmForm",
				    formWidgetClass,
				    queue,
				    NULL);
	warn =
	    XtVaCreateManagedWidget("confirmWarn",
				    labelWidgetClass,
				    form,
				    NULL);
	label =
	    XtVaCreateManagedWidget("confirmLabel",
				    labelWidgetClass,
				    form,
				    NULL);
	ok =
	    XtVaCreateManagedWidget("confirmOk",
				    commandWidgetClass,
				    form,
				    NULL, NULL);
	XtAddCallback(ok, XtNcallback, exitApplCBF, (XtPointer) queue);

	cancel =
	    XtVaCreateManagedWidget("confirmCancel",
				    commandWidgetClass,
				    form,
				    NULL);
	XtAddCallback(cancel, XtNcallback,
		      cancelConfirmExitCBF, (XtPointer) queue);
    }
    else
	the_callback = exitApplCBF;

    exitButton =
	XtVaCreateManagedWidget("exitButton",
				commandWidgetClass,
				toplevel,
				NULL);
    XtAddCallback(exitButton, XtNcallback,
		  the_callback, (XtPointer) queue);

    XtRealizeWidget(toplevel);

    if (app_resources.confirm_exit)
    {
	XtVaGetValues(label,
		      XtNheight, &label_height,
		      NULL);
	XtVaGetValues(warn,
		      XtNheight, &warn_height,
		      NULL);

	if (warn_height > label_height)
	    XtVaSetValues(label,
			  XtNheight, warn_height,
			  NULL);
	else
	    XtVaSetValues(warn,
			  XtNheight, label_height,
			  NULL);
    }

    if (app_resources.print_version)
    {
	XtPopup(versionW, XtGrabNone);
    }

    XtAppMainLoop(app_ctxt);
}
