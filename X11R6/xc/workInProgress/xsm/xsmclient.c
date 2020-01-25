/* $XConsortium: xsmclient.c,v 1.20 94/05/20 12:25:42 mor Exp $ */
/******************************************************************************

Copyright (c) 1993  X Consortium

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
X CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of the X Consortium shall not be
used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization from the X Consortium.
******************************************************************************/

/*
 * Sample client for X Session Manager.
 *
 * Written by Ralph Mor, X Consortium.
 *
 * This client simply opens a connection with a session manager,
 * sets some test property values, then responds to any session
 * manager events (SaveYourself, Die, Interact, etc...).
 *
 * Two push buttons are set up: one to retrieve the properties
 * that have been set by the client, the other to quit.
 */

#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <X11/StringDefs.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Dialog.h>
#include <X11/Xaw/Toggle.h>
#include <X11/SM/SMlib.h>
#include <stdio.h>

#ifndef X_NOT_POSIX
#include <unistd.h>
#endif
#include <limits.h>
#include <sys/param.h>
#ifndef PATH_MAX
#ifdef MAXPATHLEN
#define PATH_MAX MAXPATHLEN
#else
#define PATH_MAX 1024
#endif
#endif

typedef struct {
    XtCheckpointToken	token;
    Widget		rootShell;
    Widget		dialog;
    String		smcid;
} ApplicationDataRec, *ApplicationData;

ApplicationDataRec	globals;

static XrmOptionDescRec options[] = {
    { "-verbose", "*verbose", XrmoptionSepArg, (XPointer) False},
};

/* resources specific to the application */
static struct resources {
    Boolean	verbose;
} appResources;

#define offset(field) XtOffsetOf(struct resources, field)
static XtResource Resources[] = {
{"verbose",		"Verbose",	XtRBoolean,	sizeof(Boolean),
     offset(verbose),	XtRImmediate,	False},
};
#undef offset


static Boolean SaveState(ad)
    ApplicationData ad;
{
    String	str;
    Arg		args[10];
    Cardinal	n;

    n = 0;
#ifndef X_NOT_POSIX
    str = getcwd((char *)NULL, PATH_MAX+2);
    XtSetArg(args[n], XtNcurrentDirectory, str);	n++;
    XtSetValues(ad->rootShell, args, n);
    if (appResources.verbose)
	printf("%s:  Set %d properties\n", ad->smcid, n);
#endif

    /* Return a status indicating success or failure in saving state. */
    return True;
}


/*
 * Session management callbacks
 */
static void InteractResponse();


static void SaveResponse(rootShell, client_data, call_data)
    Widget rootShell;
    XtPointer client_data, call_data;
{
    ApplicationData ad = (ApplicationData) client_data;
    XtCheckpointToken token = (XtCheckpointToken) call_data;

    if (appResources.verbose) {
	char *saveType;
	char *shutdown;
	char *interactStyle;
	char *fast;

	switch (token->save_type) {
	  case SmSaveGlobal: saveType = "Global";		break;
	  case SmSaveLocal:  saveType = "Local";		break;
	  case SmSaveBoth:   saveType = "Both";			break;
	  default:	     saveType = "BadValueError";	break;
	}
	switch (token->interact_style) {
	  case SmInteractStyleNone:   interactStyle = "None";		break;
	  case SmInteractStyleErrors: interactStyle = "Errors";		break;
	  case SmInteractStyleAny:    interactStyle = "Any";		break;
	  default:		      interactStyle = "BadValueError";	break;
	}
	shutdown = token->shutdown ? "True" : "False";
	fast = token->fast ? "True" : "False";

	printf ("Client Id %s, received SAVE YOURSELF [", ad->smcid);
	printf ("Save Type = %s, Shutdown = %s, ", saveType, shutdown);
	printf ("Interact Style = %s, Fast = %s]\n", interactStyle, fast);
    }

    if (token->shutdown || token->interact_style != SmInteractStyleNone)
	XtSetSensitive(ad->rootShell, False);

    if (token->interact_style == SmInteractStyleNone) {
	token->save_success = SaveState(ad);
	if (appResources.verbose)
	    printf("Client Id %s, saved state.", ad->smcid);
    } else {
	/* When interaction is allowed, this client always asks for it. */
	if (token->interact_style == SmInteractStyleAny)
	    token->interact_dialog_type = SmDialogNormal;
	else
	    token->interact_dialog_type = SmDialogError;

	/* Registering a callback on this list triggers an InteractRequest.
	 * The callback will be removed from the list immediately before
         * it is executed.  If there are multiple callbacks on the list,
         * they'll be executed serially, as the interact token is returned.
	 */
	XtAddCallback(rootShell, XtNinteractCallback, InteractResponse,
		      client_data);

	if (appResources.verbose)
	    printf("Client Id %s, expect INTERACT REQUEST\n", ad->smcid);
    }
}


static void InteractResponse(rootShell, client_data, call_data)
    Widget rootShell;
    XtPointer client_data, call_data;
{
    ApplicationData ad = (ApplicationData) client_data;
    XtCheckpointToken token = (XtCheckpointToken) call_data;
    Position x, y;
    String label;
    Widget cancel;

    /* If a shutdown was pending, and it has already been cancelled,
       the interact callbacks are still called, but, they may not
       interact with the user.  */

    if (token->cancel_shutdown && token->interact_style == None) {
	token->save_success = False;
	XtSessionReturnToken(token);
	return;
    }

    if (appResources.verbose)
	printf("Client Id %s, received permission to interact\n", ad->smcid);

    /* Stash this, you'll need to return it at the end of interaction. */
    ad->token = token;

    /* If a session shutdown isn't imminent, it can't be cancelled. */
    cancel = XtNameToWidget(ad->dialog, "dialogCancelButton");
    XtSetSensitive(cancel, token->shutdown);

    /* The token always contains the parameters of the SaveYourself request. */
    if (token->interact_style == SmInteractStyleAny) {
	if (token->shutdown)
	    label = "Shutdown in progress, Normal interact with user";
	else
	    label = "Normal interact with user";
    } else {
	if (token->shutdown)
	    label = "Shutdown in progress, Error interact with user";
	else
	    label = "Error interact with user";
    }
    XtVaSetValues(ad->dialog, XtNlabel, label, NULL);

    XtVaGetValues(ad->rootShell, XtNx, &x, XtNy, &y, NULL);
    XtVaSetValues(XtParent(ad->dialog), XtNx, x, XtNy, y, NULL);
    XtPopup(XtParent(ad->dialog), XtGrabNone);
}


static void DieResponse(rootShell, client_data, call_data)
    Widget rootShell;
    XtPointer client_data, call_data;
{
    ApplicationData ad = (ApplicationData) client_data;

    if (appResources.verbose)
	printf("Client Id %s, received DIE or user said Quit\n", ad->smcid);
    XtDestroyApplicationContext(XtWidgetToApplicationContext(rootShell));
    exit(0);
}


static void ResumeResponse(rootShell, client_data, call_data)
    Widget rootShell;
    XtPointer client_data, call_data;
{
    ApplicationData ad = (ApplicationData) client_data;
    
    if (appResources.verbose)
	printf ("Client Id %s, received SHUTDOWN CANCELLED\n", ad->smcid);

    /* When a shutdown is cancelled, we might be
     *  o  waiting for our interact request to be granted
     *	o  currently in the process of interacting with the user
     *	o  previously finished saving state and expecting a Die request
     */

    XtSetSensitive(ad->rootShell, True);
}


void PropReplyProc(smcConn, client_data, numProps, props)
    SmcConn   smcConn;
    SmPointer client_data;
    int       numProps;
    SmProp    **props;
{
    ApplicationData ad = (ApplicationData) client_data;
    int i, j;

    printf ("Client Id %s, there are %d properties set:\n",
	    ad->smcid, numProps);
    printf ("\n");

    for (i = 0; i < numProps; i++) {
	printf ("Name:		%s\n", props[i]->name);
	printf ("Type:		%s\n", props[i]->type);
	printf ("Num values:	%d\n", props[i]->num_vals);
	if (strcmp(props[i]->type, SmCARD8) == 0) {
	    char *card8 = props[i]->vals->value;
	    int value = *card8;
	    printf ("Value 1:	%d\n", value);
	} else {
	    for (j = 0; j < props[i]->num_vals; j++) {
		printf ("Value %d:	%s\n", j + 1,
		    (char *) props[i]->vals[j].value);
	    }
	}
	printf ("\n");

	SmFreeProperty (props[i]);
    }

    free ((char *) props);
    printf ("\n");
}


static void GetSessionProperties(w, client_data, call_data)
    Widget	w;
    XtPointer 	client_data, call_data;
{
    ApplicationData ad = (ApplicationData) client_data;
    Arg args[1];
    SmcConn connection;

    XtSetArg(args[0], XtNconnection, &connection);
    XtGetValues(ad->rootShell, args, (Cardinal) 1);
    if (connection)
	SmcGetProperties(connection, PropReplyProc, ad);
    else
	printf("Not connected to the session manager.\n");
}


static void Quit(w, client_data, call_data)
    Widget	w;
    XtPointer 	client_data, call_data;
{
    ApplicationData ad = (ApplicationData) client_data;
    char *reasonMsg[2];

    /* If you're terminating with an error message, use SmcCloseConnection */
    reasonMsg[0] = "Quit Reason 1";
    reasonMsg[1] = "Quit Reason 2";

    /* A well-behaved client closes the session connection before exiting */
    XtVaSetValues (ad->rootShell, XtNjoinSession, False, NULL);

    /* Do the exactly the same thing we'd do if we just received a Die */
    XtCallCallbacks(ad->rootShell, XtNdieCallback, NULL);
}


static void UserSaysOkay(button, client_data, call_data)
    Widget	button;
    XtPointer 	client_data, call_data;
{
    ApplicationData ad = (ApplicationData) client_data;
    XtCheckpointToken token = ad->token;

    /* Do whatever you need to do after you've finished talking to the user. */
    token->save_success = SaveState(ad);

    XtPopdown(XtParent(ad->dialog));

    /* You have to return the token to Xt when you are through interacting. */
    XtSessionReturnToken(token);
    ad->token = NULL;

    if (appResources.verbose) {
	printf("Client Id %s, expect INTERACT DONE [Cancel Shutdown = False]\n",
	       ad->smcid);
	printf("\n");
    }

}


static void UserSaysCancel(button, client_data, call_data)
    Widget	button;
    XtPointer 	client_data, call_data;
{
    ApplicationData ad = (ApplicationData) client_data;
    XtCheckpointToken token = (XtCheckpointToken) ad->token;

    /* Pass back the information that the user wants the shutdown cancelled. */
    token->request_cancel = True;

    /* If you haven't saved application state, pass that information back. */
    token->save_success = False;

    /* You have to return the token to Xt when you are through interacting. */
    XtSessionReturnToken(token);

    ad->token = NULL;
    XtPopdown(XtParent(ad->dialog));

    if (appResources.verbose) {
	printf (
        "Client Id %s, expect INTERACT DONE [Cancel Shutdown = True]\n",
		ad->smcid);
	printf ("Client Id %s, expect SAVE YOURSELF DONE [Success = False]\n",
		ad->smcid);
	printf ("\n");
    }
}


static void CreateInteractDialog(ad)
    ApplicationData ad;
{
    Widget okay, cancel;
    Widget dialogPopup;

    dialogPopup = XtVaCreatePopupShell (
	"dialogPopup", transientShellWidgetClass, ad->rootShell,
        XtNtitle, "Dialog",
	XtNallowShellResize, True,
	XtNancestorSensitive, True,
        NULL);

    ad->dialog = XtVaCreateManagedWidget (
	"dialog", dialogWidgetClass, dialogPopup,
	NULL);

    XtVaSetValues (XtNameToWidget (ad->dialog, "label"),
	XtNresizable, True,
	NULL);

    okay = XtVaCreateManagedWidget (
	"dialogOkButton", commandWidgetClass, ad->dialog,
	XtNlabel, "OK",
	NULL);

    XtAddCallback(okay, XtNcallback, UserSaysOkay, ad);

    cancel = XtVaCreateManagedWidget (
	"dialogCancelButton", commandWidgetClass, ad->dialog,
	XtNlabel, "Cancel Shutdown", NULL);

    XtAddCallback(cancel, XtNcallback, UserSaysCancel, ad);
}


static void CreateMainInterface(ad)
    ApplicationData ad;
{
    Widget mainWindow;
    Widget quitButton, getPropButton;
    Widget cwdLabel, cwdDataLabel;

    mainWindow = XtCreateManagedWidget (
	"mainWindow", formWidgetClass, ad->rootShell, NULL, 0);

    getPropButton = XtVaCreateManagedWidget (
	"getPropButton", commandWidgetClass, mainWindow,
	XtNlabel, "Get Properties",
	NULL);

    XtAddCallback(getPropButton, XtNcallback, GetSessionProperties, ad);

    quitButton = XtVaCreateManagedWidget (
	"quitButton", commandWidgetClass, mainWindow,
	XtNlabel, "Quit (and exit session)",
        XtNfromVert, getPropButton,
	NULL);

    XtAddCallback(quitButton, XtNcallback, Quit, ad);

    cwdLabel = XtVaCreateManagedWidget (
	"cwdLabel", labelWidgetClass, mainWindow,
	XtNlabel, "Dir:",
        XtNfromVert, quitButton,
        XtNborderWidth, 0,
	NULL);

    cwdDataLabel = XtVaCreateManagedWidget (
	"cwdDataLabel", labelWidgetClass, mainWindow,
	XtNlabel,
#ifndef X_NOT_POSIX
	getcwd((char *)NULL, PATH_MAX),
#else
	"unknown-cwd",
#endif
        XtNfromHoriz, cwdLabel,
        XtNfromVert, quitButton,
        XtNborderWidth, 0,
	NULL);
}


main(argc, argv)
    int  argc;
    char **argv;
{
    XtAppContext appContext;
    ApplicationData ad = &globals;

    ad->rootShell = XtOpenApplication(&appContext, "XSMclient",
				      options, XtNumber(options),
				      &argc, argv, NULL, 
				      sessionShellWidgetClass, NULL, 0);

    XtAddCallback(ad->rootShell, XtNsaveCallback, SaveResponse, ad);
    XtAddCallback(ad->rootShell, XtNcancelCallback, ResumeResponse, ad);
    XtAddCallback(ad->rootShell, XtNsaveCompleteCallback, ResumeResponse, ad);
    XtAddCallback(ad->rootShell, XtNdieCallback, DieResponse, ad);
	
    XtGetApplicationResources(ad->rootShell, (XtPointer)&appResources,
			      Resources, XtNumber(Resources), NULL, 0);

    CreateMainInterface(ad);
    XtRealizeWidget(ad->rootShell);
    CreateInteractDialog(ad);

    if (appResources.verbose) {
	Arg args[1];
	XtSetArg(args[0], XtNsessionID, &ad->smcid);
	XtGetValues(ad->rootShell, args, (Cardinal) 1);
	ad->smcid = XtNewString(ad->smcid);
    }

    XtAppMainLoop(appContext);
}
