/* $Header: xtext.c,v 1.4 88/02/14 15:13:56 rws Exp $ */
#include <X11/Intrinsic.h>
#include <X11/AsciiText.h>
#include <stdio.h>

static XrmOptionDescRec table[] = {
    {"-v",	"value",		XrmoptionSepArg, NULL},
    {"-c",	"numwidgets",		XrmoptionSepArg, NULL},
    {"-sync",	"syncit",		XrmoptionNoArg,  "on"},
    {"-w",	"Text.wordWrap",	XrmoptionNoArg,  "on"},
    {"-nw",	"Text.wordWrap",	XrmoptionNoArg,  "off"},
};

static char *value;
static int numwidgets = 1;
static Boolean syncit = FALSE;

main(argc, argv)
unsigned int argc;
char **argv;
{
    static Arg arglist[] = {
	{XtNstring, (XtArgVal) NULL},
	{XtNlength, (XtArgVal) NULL},
	{XtNeditType, (XtArgVal) XttextEdit}
    };
    int *source;
    Widget toplevel, w;

    char *index();

    toplevel = XtInitialize("TTest", "TopLevel", 0, 0, &argc, argv);

    value = "This is a\ntest.  If this\nhad been an actual\nemergency...";
    arglist[0].value = (XtArgVal)value;
    arglist[1].value = (XtArgVal)strlen(value);

    w = XtCreateManagedWidget( argv[0], asciiStringWidgetClass, toplevel,
			       arglist, XtNumber(arglist) );

    XtRealizeWidget(toplevel);
    XtMainLoop();
}
