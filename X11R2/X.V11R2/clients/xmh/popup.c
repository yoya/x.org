/* $Header: popup.c,v 2.4 88/02/22 10:37:31 swick Exp $ */
/* popup.c -- Handle pop-up widgets. */

#include "xmh.h"

/*
static Widget confirmwidget = NULL;
static char *confirmstring;

static Widget confirmparent;
static count = 0;

static Widget promptwidget;
static int (*promptfunc)();
static char promptstring[210];

extern TellPrompt();
void DestroyPromptWidget();

ArgList arglist, promptarglist;
*/

/*
InitPopup()
{
}
*/

static Scrn lastscrn = NULL;
static char laststr[500];

static Widget confirmwidget = NULL;
static int buttoncount = 0;
static Widget promptwidget = NULL;
static void (*promptfunction)();


void CenterWidget(parent, child)
Widget parent, child;
{
    int x, y;
    x = (GetWidth(parent) - GetWidth(child)) / 2;
    y = (GetHeight(parent) - GetHeight(child)) / 2;
    if (x < 0) x = 0;
    XtMoveWidget(child, x, y);
}


void DestroyConfirmWidget()
{
    lastscrn = NULL;
    *laststr = 0;
    if (confirmwidget) {
	XtDestroyWidget(confirmwidget);
	confirmwidget = NULL;
    }
}


int Confirm(scrn, str)
Scrn scrn;
char *str;
{
    Arg args[1];
    extern void RedoLastButton();
    if (lastscrn == scrn && strcmp(str, laststr) == 0) {
	DestroyConfirmWidget();
	return TRUE;
    }
    DestroyConfirmWidget();
    lastscrn = scrn;
    scrn = LastButtonScreen();
    (void) strcpy(laststr, str);
    XtSetArg( args[0], XtNlabel, str );
    confirmwidget = XtCreateWidget( "confirm", dialogWidgetClass,
				    scrn->widget, args, XtNumber(args) );
    XtDialogAddButton(confirmwidget, "yes", RedoLastButton, (caddr_t)NULL);
    XtDialogAddButton(confirmwidget, "no", DestroyConfirmWidget,(caddr_t)NULL);
    XtRealizeWidget( confirmwidget );
    CenterWidget(scrn->widget, confirmwidget);
    XtMapWidget( confirmwidget );
    buttoncount = 0;
    return FALSE;
}


HandleConfirmEvent(event)
XEvent *event;
{
    if (confirmwidget &&
	    (event->type == ButtonRelease || event->type == KeyPress)) {
	if (++buttoncount > 1)
	    DestroyConfirmWidget();
    }
}


void DestroyPromptWidget()
{
    if (promptwidget) {
	XtDestroyWidget(promptwidget);
	promptwidget = NULL;
    }
}


void TellPrompt()
{
    (*promptfunction)(XtDialogGetValueString(promptwidget));
    DestroyPromptWidget();
}

MakePrompt(scrn, prompt, func)
Scrn scrn;
char *prompt;
void (*func)();
{
    DestroyPromptWidget();
    promptwidget = XtCreateWidget( "prompt", dialogWidgetClass, scrn->widget,
				   NULL, (Cardinal)0 );
    XtDialogAddButton(promptwidget, "goAhead", TellPrompt, (caddr_t)NULL);
    XtDialogAddButton(promptwidget, "cancel", DestroyPromptWidget, (caddr_t)NULL);
    XtRealizeWidget(promptwidget);
    CenterWidget(scrn->widget, promptwidget);
    XtMapWidget( promptwidget );
    promptfunction = func;
}
