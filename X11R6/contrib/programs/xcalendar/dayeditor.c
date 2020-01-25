/*
 * 
 * Copyright (C) 1988 Massachusetts Institute of Technology
 * 
 */


/*
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of M.I.T. not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission. M.I.T. makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 * 
 */

/***********************************************************************
* Copyright 1990-1994 by Richard Bingle and Purdue University.  All rights
* reserved.  Some individual files may be covered by other copyrights.
*
* Redistribution and use in source and binary forms are permitted
* provided that this entire copyright notice is duplicated in all such
* copies.  Neither the name of the University, nor the name of the author
* may be used to endorse or promote products derived from this material
* without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF
* MERCHANTIBILITY AND FITNESS FOR ANY PARTICULAR PURPOSE.
************************************************************************/

/** Written by Roman J. Budzianowski - Project Athena, MIT **/
/** Modified by Richard Bingle - Purdue University **/

#include "xcalendar.h"

char           *GetHoliday();

static XtCallbackRec callbacks[] = {
    {NULL, NULL},
    {NULL, NULL}
};

List            editorList = NULL;
Boolean         initialized = False;
char           *homedir = NULL;
char           *calendarDir = NULL;

extern Cardinal displayedMonth, displayedYear;
void            SaveButtonHandler();


Boolean         already_open();
void            SaveText();

/*
 * ChangeDay() - Edit a new day with an existing dayeditor.
 */

void
ChangeDay(editor)
    DayEditor       editor;

{
    Arg             args[10];
    int             n;
    char            mon[4];
    char            title[80];
#ifndef ATHENA
    XmString        xms;
#endif

    if (appResources.oldStyle) {
	strncpy(mon, smonth[editor->month], 3);
	mon[3] = '\0';
	sprintf(editor->filename, "xc%d%s%d", editor->day, mon, editor->year);
    } else {
	sprintf(editor->filename, "xc%d.%02d.%02d", editor->year,
		editor->month, editor->day);
    }

    editor->open = False;

    if (!ReadFile(editor))
	return;

    editor->used = True;

    sprintf(title, " %d %s %d ",
	    editor->day, smonth[editor->month], editor->year);

    n = 0;
#ifdef ATHENA
    XtSetArg(args[n], XtNlabel, title);
    n++;
    XtSetValues(editor->title, args, n);
#else
    xms = XMS(title);
    XtSetArg(args[n], XmNlabelString, xms);
    n++;
    XtSetValues(editor->title, args, n);
    XmStringFree(xms);
#endif

    n = 0;
    XtSetArg(args[n], XtNiconName, title);
    n++;
    XtSetValues(editor->shell, args, n);

    XtSetSensitive(editor->saveButton, False);

    n = 0;
    callbacks[0].callback = (XtCallbackProc) SaveButtonHandler;
    callbacks[0].closure = (XtPointer) editor;
#ifdef ATHENA
    XtSetArg(args[n], XtNcallback, callbacks);
    n++;
    XtSetValues(XawTextGetSource(editor->editor), args, n);
#else
    XtSetArg(args[n], XmNmodifyVerifyCallback, callbacks);
    n++;
    XtSetValues(editor->editor, args, n);
#endif

    if (EmptyBuffer(editor) || already_open(editor->filename))
	XtSetSensitive(editor->clearEntry, False);
    else
	XtSetSensitive(editor->clearEntry, True);

    if (!already_open(editor->filename))
	editor->open = True;
}

/*
 * PrevDay() - Previous day callback.
 */

void
PrevDay(w, closure, call_data)
    Widget          w;
    XtPointer       closure;
    XtPointer       call_data;
{
    DayEditor       editor = (DayEditor) closure;
    Cardinal        numberofDays;

    XtUnmapWidget(w);

    if (editor->saved == False) {
	SaveText(w, closure, call_data);
    }
    if (editor->day == 1) {
	if (editor->month == 1) {
	    editor->month = 12;
	    editor->year--;
	} else
	    editor->month--;
	numberofDays = NumberOfDays(editor->month, editor->year);
	editor->day = numberofDays;
    } else
	editor->day--;

    ChangeDay(editor);
    XtMapWidget(w);
}

/*
 * SuccDay() - Next day callback.
 */

void
SuccDay(w, closure, call_data)
    Widget          w;
    XtPointer       closure;
    XtPointer       call_data;
{
    DayEditor       editor = (DayEditor) closure;
    Cardinal        numberofDays;

    XtUnmapWidget(w);

    if (editor->saved == False) {
	SaveText(w, closure, call_data);
    }
    numberofDays = NumberOfDays(editor->month, editor->year);
    if (editor->day == numberofDays) {
	if (editor->month == 12) {
	    editor->month = 1;
	    editor->year++;
	} else
	    editor->month++;
	editor->day = 1;
    } else
	editor->day++;

    ChangeDay(editor);
    XtMapWidget(w);
}

/*
 * already_open() - Returns true if there is already an editor
 * open for that day (within this process).
 */

Boolean
already_open(filename)
    char           *filename;
{
    DayEditor       editor;
    int             i;

    if (editorList == NULL)
	return (False);

    for (i = 1; i <= ListLength(editorList); i++) {
	editor = GetEditorFromList(editorList, i);
	if (!strcmp(editor->filename, filename)) {
	    if (editor->open == True) {
		return (True);
	    }
	}
    }
    return (False);
}

/*
 * InitEditors() - Initialize dayeditors.
 */

Boolean
InitEditors()
{
    char           *getenv();
    DIR            *dirp;
#if defined (SYSV) || defined(__osf__) || defined(SVR4)
    struct dirent  *dp;
#else
    struct direct  *dp;
#endif
    char            yearStr[5];
    char            MoYear[16];
    Cardinal        firstDay;
    int             error_status;
    struct stat     dir_status;

    sprintf(yearStr, "%d", displayedYear);
    sprintf(MoYear, "xc%d.%02d.", displayedYear, displayedMonth);
    firstDay = FirstDay(month, year);

    if (calendarDir == NULL) {
	if (appResources.calendarDir == NULL) {
	    /* open the ~/Calendar directory - create if necessary */
	    homedir = getenv("HOME");

	    calendarDir = XtMalloc(strlen(homedir) + 10);
	    strcpy(calendarDir, homedir);
	    strcat(strcat(calendarDir, "/"), "Calendar");
	} else {
	    calendarDir = XtMalloc(strlen(appResources.calendarDir) + 3);
	    strcpy(calendarDir, appResources.calendarDir);
	}
    }
    error_status = stat(calendarDir, &dir_status);

    if (error_status == -1)
	if (errno == ENOENT) {	/* not found */
	    /* it should be a popup asking the user ... */
	    fprintf(stderr, "%s doesn't exist : Creating...", calendarDir);

	    if (mkdir(calendarDir, 0700) == -1) {
		XBell(XtDisplay(toplevel), 100);
		fprintf(stderr, "Couldn't create %s directory.\n");
		perror("xcalendar:mkdir:");
		return False;
	    }
	} else {
	    XtWarning("Cannot stat calendar directory");
	    return False;
	}

    chdir(calendarDir);

    dirp = (DIR *) opendir(calendarDir);

    if (dirp == NULL) {
	XtWarning("Cannot open calendar (maybe it's not a directory)");
	return False;
    }
    while ((dp = readdir(dirp)) != NULL) {
	char           *s;
	/* find entries from the displayedMonth and store them in a list */

	if (appResources.oldStyle) {
	    s = (char *) strchr(dp->d_name, *smonth[displayedMonth]);

	    if (s != NULL && !strncmp(s + 3, yearStr, 4) &&
		!strncmp(s, smonth[displayedMonth], 3))
		MarkDayEntry(GetWidgetFromList(daylist, atoi(dp->d_name + 2) +
					       firstDay - 1), True);
	} else {
	    if (!strncmp(dp->d_name, MoYear, strlen(MoYear))) {
		s = dp->d_name + strlen(MoYear);
		MarkDayEntry(GetWidgetFromList(daylist, atoi(s) + firstDay - 1),
			     True);
	    }
	}
    }

    if (!editorList)
	editorList = CreateList(5, sizeof(DayEditor));

    closedir(dirp);

    initialized = True;
    return True;
}

/*
 * EditDayEntry() - Callback for day buttons.
 */

void
EditDayEntry(w, closure, call_data)
    Widget          w;
    XtPointer       closure;
    XtPointer       call_data;
{
    int             i;
    DayEditor       editor = NULL;
    Arg             args[10];
    int             n;
    char            title[80];
    Boolean         success;
    char            mon[4];
#ifndef ATHENA
    XmString        xms;
#endif

    if (!initialized) {
	success = InitEditors();
	if (!success)
	    return;
    }
    /* find free editor */

    for (i = 1; i <= ListLength(editorList); i++) {
	editor = GetEditorFromList(editorList, i);
	if (editor->used)
	    editor = NULL;
	else
	    break;
    }

    if (editor == NULL) {
#ifdef ATHENA
#ifdef XI18N
	editor = CreateDayEditor("dayEditor", XawtextWrapLine, NULL);
#else
	editor = CreateDayEditor("dayEditor", XawtextWrapWord, NULL);
#endif
#else
	editor = CreateMotifDayEditor("dayEditor", NULL, NULL);
#endif
	PushEditorOnList(editorList, editor);
    }
    editor->button = w;
    editor->day = (int) closure;
    editor->month = displayedMonth;
    editor->year = displayedYear;
    editor->saved = True;

    editor->filename = XtMalloc(12);

    if (appResources.oldStyle) {
	strncpy(mon, smonth[editor->month], 3);
	mon[3] = '\0';
	sprintf(editor->filename, "xc%d%s%d", editor->day, mon, editor->year);
    } else {
	sprintf(editor->filename, "xc%d.%02d.%02d", editor->year, editor->month,
		editor->day);
    }

    if (!ReadFile(editor))
	return;

    editor->used = True;

    sprintf(title, " %d %s %d ",
	    editor->day, smonth[editor->month], editor->year);

    n = 0;
#ifdef ATHENA
    XtSetArg(args[n], XtNlabel, title);
    n++;
    XtSetValues(editor->title, args, n);
#else
    xms = XMS(title);
    XtSetArg(args[n], XmNlabelString, xms);
    n++;
    XtSetValues(editor->title, args, n);
    XmStringFree(xms);
#endif

    XtSetArg(args[n], XtNiconName, title);
    n++;
    XtSetValues(editor->shell, args, n);

    XtSetSensitive(editor->saveButton, False);

    n = 0;
    callbacks[0].callback = (XtCallbackProc) SaveButtonHandler;
    callbacks[0].closure = (XtPointer) editor;
#ifdef ATHENA
    XtSetArg(args[n], XtNcallback, callbacks);
    n++;
    XtSetValues(XawTextGetSource(editor->editor), args, n);
#else
    XtSetArg(args[n], XmNmodifyVerifyCallback, callbacks);
    n++;
    XtSetValues(editor->editor, args, n);
#endif

    if (EmptyBuffer(editor) || already_open(editor->filename))
	XtSetSensitive(editor->clearEntry, False);
    else
	XtSetSensitive(editor->clearEntry, True);

    if (!already_open(editor->filename))
	editor->open = True;

    StartEditor(editor);

}

/*
 * SaveButtonHandler() - Updates sensitivity of save and clear buttons
 * once text is entered.
 */

void
SaveButtonHandler(w, data, call_data)
    Widget          w;
    XtPointer       data;
    XtPointer       call_data;
{
    DayEditor       editor = (DayEditor) data;
    Arg             args[10];
    int             n;
    char            temp;

    if (editor->open == False)
	return;

    XtSetSensitive(editor->saveButton, True);
    editor->saved = False;
    XtSetSensitive(editor->clearEntry, True);

    n = 0;
#ifdef ATHENA
    XtSetArg(args[n], XtNcallback, NULL);
    n++;
    XtSetValues(XawTextGetSource(editor->editor), args, n);
#else
    XtSetArg(args[n], XmNmodifyVerifyCallback, NULL);
    n++;
    XtSetValues(editor->editor, args, n);
#endif
}

/*
 * SaveText() - Save callback.
 */

void
SaveText(w, closure, call_data)
    Widget          w;
    XtPointer       closure;
    XtPointer       call_data;
{
    DayEditor       editor = (DayEditor) closure;
    Arg             args[10];
    int             n;
    int             saveStatus;
#ifndef ATHENA
    char           *sb;
#endif

    if (editor->filename == NULL)
	return;

    if (editor->open == False)
	return;

#ifdef XI18N
    {
	String          str;
	Arg             args[10];
	int             n;

	n = 0;
	XtSetArg(args[n], XtNstring, &str);
	n++;
	XtGetValues(editor->editor, args, n);
	strcpy(editor->buffer, str);
	/* XawAsciiSourceFreeString(w); */
    }
#endif

#ifdef ATHENA
    if ((saveStatus = write_to_file(editor->filename, editor->buffer, TextSize(editor))) == -1)
	return;
#else
    sb = XmTextGetString(editor->editor);
    if ((saveStatus = write_to_file(editor->filename, sb, strlen(sb))) == -1) {
	XtFree(sb);
	return;
    }
    XtFree(sb);
#endif

    if (saveStatus == 0) {

	if (editor->month == displayedMonth && editor->year == displayedYear)
	    MarkDayEntry(GetWidgetFromList(daylist,
		 editor->day + FirstDay(displayedMonth, displayedYear) - 1),
			 False);
	XtSetSensitive(editor->clearEntry, False);
    } else if (editor->month == displayedMonth && editor->year == displayedYear)
	MarkDayEntry(GetWidgetFromList(daylist, editor->day +
			FirstDay(displayedMonth, displayedYear) - 1), True);

    XtSetSensitive(editor->saveButton, False);

    n = 0;
    callbacks[0].callback = (XtCallbackProc) SaveButtonHandler;
    callbacks[0].closure = (XtPointer) editor;
#ifdef ATHENA
    XtSetArg(args[n], XtNcallback, callbacks);
    n++;
    XtSetValues(XawTextGetSource(editor->editor), args, n);
#else
    XtSetArg(args[n], XmNmodifyVerifyCallback, callbacks);
    n++;
    XtSetValues(editor->editor, args, n);
#endif

    editor->saved = True;
}

/*
 * ExitEditor() - Done callback.
 */

void
ExitEditor(w, closure, call_data)
    Widget          w;
    XtPointer       closure;
    XtPointer       call_data;
{
    DayEditor       editor = (DayEditor) closure;
    Arg             args[10];
    int             n;

    if (editor->saved == False)
	SaveText(w, closure, call_data);

    n = 0;
#ifdef ATHENA
    XtSetArg(args[n], XtNcallback, NULL);
    n++;
    XtSetValues(XawTextGetSource(editor->editor), args, n);
#else
    XtSetArg(args[n], XmNmodifyVerifyCallback, callbacks);
    n++;
    XtSetValues(editor->editor, args, n);
#endif

    editor->used = False;

    XtPopdown(editor->shell);

    editor->open = False;
    editor->year = displayedYear;
    editor->month = displayedMonth;
    if (editor->button == help_button)
	XtSetSensitive(editor->button, True);
}

/*
 * ClearEntry() - Clear callback.
 */

void
ClearEntry(w, closure, call_data)
    Widget          w;
    XtPointer       closure;
    XtPointer       call_data;
{
    DayEditor       editor = (DayEditor) closure;

    bzero(editor->buffer, appResources.textBufferSize);

    ChangeTextSource(editor, editor->buffer, "", True);

    SaveText(w, closure, call_data);

}


#ifdef ATHENA

/*
 * CreateDayEditor() - Creates an individual day editor.
 */

DayEditor
CreateDayEditor(name, options, initialtext)
    char           *name;
    int             options;
    char           *initialtext;
{
    DayEditor       editor = (DayEditor) XtCalloc(1, sizeof(DayEditorRec));
    Arg             args[10];
    int             n;
    Widget          frame, title, text, controls;

    Widget          daybuttons;

    editor->shell = XtCreatePopupShell(name, topLevelShellWidgetClass,
				       toplevel, NULL, 0);

    n = 0;
    frame = XtCreateWidget("editorFrame", panedWidgetClass, editor->shell,
			   args, n);

    n = 0;
    daybuttons = XtCreateManagedWidget("daybuttons", formWidgetClass, frame,
				       args, n);

    n = 0;
    callbacks[0].callback = PrevDay;
    callbacks[0].closure = (XtPointer) editor;
    XtSetArg(args[n], XtNcallback, callbacks);
    n++;
    XtSetArg(args[n], XtNfromHoriz, NULL);
    n++;
    XtSetArg(args[n], XtNleft, XtChainLeft);
    n++;
    XtSetArg(args[n], XtNright, XtChainLeft);
    n++;

    editor->prevday = XtCreateManagedWidget("prevday", commandWidgetClass,
					    daybuttons, args, n);

    /** create title **/

    n = 0;
    XtSetArg(args[n], XtNskipAdjust, True);
    n++;
    XtSetArg(args[n], XtNfromHoriz, editor->prevday);
    n++;
    XtSetArg(args[n], XtNleft, XtChainLeft);
    n++;
    XtSetArg(args[n], XtNright, XtChainRight);
    n++;

    title = XtCreateManagedWidget("editorTitle", labelWidgetClass, daybuttons,
				  args, n);
    editor->title = title;

    editor->buffer = XtCalloc(appResources.textBufferSize, sizeof(char));

    n = 0;
    callbacks[0].callback = SuccDay;
    callbacks[0].closure = (XtPointer) editor;
    XtSetArg(args[n], XtNcallback, callbacks);
    n++;
    XtSetArg(args[n], XtNfromHoriz, title);
    n++;
    XtSetArg(args[n], XtNleft, XtChainRight);
    n++;
    XtSetArg(args[n], XtNright, XtChainRight);
    n++;

    editor->succday = XtCreateManagedWidget("succday", commandWidgetClass,
					    daybuttons, args, n);

    if (initialtext)
	strcpy(editor->buffer, initialtext);

    editor->bufSize = appResources.textBufferSize;

    n = 0;
    XtSetArg(args[n], XtNstring, editor->buffer);
    n++;
    XtSetArg(args[n], XtNlength, appResources.textBufferSize);
    n++;
    XtSetArg(args[n], XtNwrap, options);
    n++;

    text = XtCreateManagedWidget("editor", asciiTextWidgetClass, frame,
				 args, n);
    editor->editor = text;

    n = 0;
    XtSetArg(args[n], XtNstring, "");
    n++;
    XtSetArg(args[n], XtNlength, 1);
    n++;
    XtSetArg(args[n], XtNwrap, XawtextWrapNever);
    n++;
    XtSetArg(args[n], XtNskipAdjust, True);
    n++;
    editor->holiday = XtCreateManagedWidget("holiday", asciiTextWidgetClass,
					    frame, args, n);

    n = 0;
    XtSetArg(args[n], XtNfromVert, editor->holiday);
    n++;
    XtSetArg(args[n], XtNskipAdjust, True);
    n++;
    controls = XtCreateManagedWidget("editorControls", boxWidgetClass,
				     frame, args, n);

    n = 0;
    callbacks[0].callback = ExitEditor;
    callbacks[0].closure = (XtPointer) editor;
    XtSetArg(args[n], XtNcallback, callbacks);
    n++;

    editor->doneButton = XtCreateManagedWidget("doneButton", 
                                    commandWidgetClass, controls, args, n);

    n = 0;
    callbacks[0].callback = SaveText;
    callbacks[0].closure = (XtPointer) editor;
    XtSetArg(args[n], XtNcallback, callbacks);
    n++;

    editor->saveButton = XtCreateManagedWidget("saveButton",
				     commandWidgetClass, controls, args, n);

    n = 0;
    callbacks[0].callback = ClearEntry;
    callbacks[0].closure = (XtPointer) editor;
    XtSetArg(args[n], XtNcallback, callbacks);
    n++;

    editor->clearEntry = XtCreateManagedWidget("clearEntry",
				     commandWidgetClass, controls, args, n);

    XtInstallAllAccelerators(editor->editor, editor->shell);

    XtManageChild(frame);

    XtRealizeWidget(editor->shell);

    return editor;

}
#else

/*
 * CreateDayEditor() - Creates an individual day editor.
 */

DayEditor
CreateMotifDayEditor(name, options, initialtext)
    char           *name;
    int             options;
    char           *initialtext;
{
    DayEditor       editor = (DayEditor) XtCalloc(1, sizeof(DayEditorRec));
    Arg             args[10];
    int             n;
    Widget          frame, title, text, controls;

    Widget          daybuttons;
    XtWidgetGeometry size;

    editor->shell = XtCreatePopupShell(name, topLevelShellWidgetClass,
				       toplevel, NULL, 0);

    XtAddEventHandler(editor->shell, (EventMask)0, True,
                      _XEditResCheckMessages, NULL);

    n = 0;
    frame = XtCreateWidget("editorFrame", xmPanedWindowWidgetClass,
			   editor->shell, args, n);

    n = 0;
    daybuttons = XtCreateManagedWidget("daybuttons", xmFormWidgetClass,
				       frame, args, n);

    n = 0;
    callbacks[0].callback = PrevDay;
    callbacks[0].closure = (XtPointer) editor;
    XtSetArg(args[n], XmNactivateCallback, callbacks);
    n++;
    XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM);
    n++;

    editor->prevday = XtCreateManagedWidget("prevday", xmPushButtonWidgetClass,
					    daybuttons, args, n);

    n = 0;
    callbacks[0].callback = SuccDay;
    callbacks[0].closure = (XtPointer) editor;
    XtSetArg(args[n], XmNactivateCallback, callbacks);
    n++;
    XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM);
    n++;

    editor->succday = XtCreateManagedWidget("succday", xmPushButtonWidgetClass,
					    daybuttons, args, n);

    /** create title **/

    n = 0;
    XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET);
    n++;
    XtSetArg(args[n], XmNleftWidget, editor->prevday);
    n++;
    XtSetArg(args[n], XmNrightAttachment, XmATTACH_WIDGET);
    n++;
    XtSetArg(args[n], XmNrightWidget, editor->succday);
    n++;

    title = XtCreateManagedWidget("editorTitle", xmLabelWidgetClass, daybuttons,
				  args, n);
    editor->title = title;

    editor->buffer = XtCalloc(appResources.textBufferSize, sizeof(char));

    if (initialtext)
	strcpy(editor->buffer, initialtext);

    editor->bufSize = appResources.textBufferSize;

    n = 0;
    XtSetArg(args[n], XmNstring, editor->buffer);
    n++;
    XtSetArg(args[n], XmNlength, appResources.textBufferSize);
    n++;
    XtSetArg(args[n], XmNeditMode, XmMULTI_LINE_EDIT);
    n++;

    text = XmCreateScrolledText(frame, "editor", args, n);
    XtManageChild(text);
    editor->editor = text;

    n = 0;
    XtSetArg(args[n], XmNeditMode, XmSINGLE_LINE_EDIT);
    n++;
    XtSetArg(args[n], XmNeditable, False);
    n++;

    editor->holiday = XtCreateManagedWidget("holiday", xmTextWidgetClass,
					    frame, args, n);
    size.request_mode = CWHeight;
    XtQueryGeometry(editor->holiday, NULL, &size);

    n = 0;
    XtSetArg(args[n], XmNpaneMinimum, size.height);
    n++;
    XtSetArg(args[n], XmNpaneMaximum, size.height);
    n++;
    XtSetValues(editor->holiday, args, n);

    n = 0;
    XtSetArg(args[n], XmNfractionBase, 3);
    n++;
    controls = XtCreateManagedWidget("editorControls", xmFormWidgetClass,
				     frame, args, n);

    n = 0;
    callbacks[0].callback = ExitEditor;
    callbacks[0].closure = (XtPointer) editor;
    XtSetArg(args[n], XmNactivateCallback, callbacks);
    n++;
    XtSetArg(args[n], XmNleftAttachment, XmATTACH_POSITION);
    n++;
    XtSetArg(args[n], XmNleftPosition, 0);
    n++;
    XtSetArg(args[n], XmNrightAttachment, XmATTACH_POSITION);
    n++;
    XtSetArg(args[n], XmNrightPosition, 1);
    n++;

    editor->doneButton = XtCreateManagedWidget("doneButton", 
                                xmPushButtonWidgetClass, controls, args, n);

    n = 0;
    callbacks[0].callback = SaveText;
    callbacks[0].closure = (XtPointer) editor;
    XtSetArg(args[n], XmNactivateCallback, callbacks);
    n++;
    XtSetArg(args[n], XmNleftAttachment, XmATTACH_POSITION);
    n++;
    XtSetArg(args[n], XmNleftPosition, 1);
    n++;
    XtSetArg(args[n], XmNrightAttachment, XmATTACH_POSITION);
    n++;
    XtSetArg(args[n], XmNrightPosition, 2);
    n++;

    editor->saveButton = XtCreateManagedWidget("saveButton",
				xmPushButtonWidgetClass, controls, args, n);

    n = 0;
    callbacks[0].callback = ClearEntry;
    callbacks[0].closure = (XtPointer) editor;
    XtSetArg(args[n], XmNactivateCallback, callbacks);
    n++;
    XtSetArg(args[n], XmNleftAttachment, XmATTACH_POSITION);
    n++;
    XtSetArg(args[n], XmNleftPosition, 2);
    n++;
    XtSetArg(args[n], XmNrightAttachment, XmATTACH_POSITION);
    n++;
    XtSetArg(args[n], XmNrightPosition, 3);
    n++;

    editor->clearEntry = XtCreateManagedWidget("clearEntry",
				xmPushButtonWidgetClass, controls, args, n);

    size.request_mode = CWHeight;
    XtQueryGeometry(controls, NULL, &size);

    n = 0;
    XtSetArg(args[n], XmNpaneMinimum, size.height);
    n++;
    XtSetArg(args[n], XmNpaneMaximum, size.height);
    n++;
    XtSetValues(controls, args, n);

    HideSashes(frame);
    XtManageChild(frame);

    XtRealizeWidget(editor->shell);

    return editor;

}
#endif

/*
 * ReadFile() - Returns true if it manages to read the file associated
 * with editor.
 */

Boolean
ReadFile(editor)
    DayEditor       editor;
{
    int             size;
    Arg             args[10];
    int             n;

    /*
     * #ifdef ATHENA XtSetArg(args[0], XtNeditType, XawtextEdit); #else
     * XtSetArg(args[0], XmNeditable, True); #endif
     * XtSetValues(editor->editor, args, 1);
     */

    bzero(editor->buffer, editor->bufSize);

    size = read_file(editor->filename, editor->buffer, editor->bufSize);

    if (size == -1)
	return False;
    else if (size > editor->bufSize) {
	editor->buffer = XtRealloc(editor->buffer, size + 1);
	editor->bufSize = size + 1;
	size = read_file(editor->filename, editor->buffer, editor->bufSize);
	if (size == -1)
	    return False;
    }
#define ALREADYOPEN "	** Editor Already Open! ** \n\n"

    n = 0;
#ifdef ATHENA
    XtSetArg(args[n], XtNeditType, XawtextEdit);
#else
    XtSetArg(args[n], XmNeditable, True);
#endif
    if (already_open(editor->filename)) {
	static char    *sb_tmp;

	sb_tmp = XtMalloc(editor->bufSize + strlen(ALREADYOPEN) + 1);
	strcpy(sb_tmp, ALREADYOPEN);
	strcat(sb_tmp, editor->buffer);
	XtFree(editor->buffer);
	editor->buffer = sb_tmp;
	editor->bufSize = editor->bufSize + strlen(ALREADYOPEN) + 1;
#ifdef ATHENA
	XtSetArg(args[n], XtNeditType, XawtextRead);
	n++;
#else
	XtSetArg(args[n], XmNeditable, False);
	n++;
#endif
    }
    ChangeTextSource(editor, editor->buffer, GetHoliday(editor), False);

    /*
     * XtSetArg(args[n], XtNstring, editor->buffer); n++;
     */
    XtSetValues(editor->editor, args, n);

    return True;
}

/*
 * ChangeTextSource() - Changes the text in the dayeditor.
 */

void
ChangeTextSource(editor, newtext, newhol, clear)
    DayEditor       editor;
    char           *newtext;
    char           *newhol;
    Boolean         clear;
{
    Arg             args[10];
    int             n;
#ifdef ATHENA
    Widget          old = XawTextGetSource(editor->editor);
    Widget          oldhol = XawTextGetSource(editor->holiday);
#endif

    n = 0;
#ifdef ATHENA
    XtSetArg(args[n], XtNstring, newtext);
    n++;
    XtSetArg(args[n], XtNlength, editor->bufSize);
    n++;
    XtSetArg(args[n], XtNeditType, XawtextEdit);
    n++;
    XtSetArg(args[n], XtNtype, XawAsciiString);
    n++;
#ifdef XI18N
    XtSetArg(args[n], XtNuseStringInPlace, False);
    n++;
#else
    XtSetArg(args[n], XtNuseStringInPlace, True);
    n++;
#endif				/* XI18N */
    XawTextSetSource(editor->editor,
		     XtCreateWidget("textSource", asciiSrcObjectClass,
			     editor->editor, args, n), (XawTextPosition) 0);
    if (!clear) {
        XtSetArg(args[0], XtNstring, newhol);
        XtSetArg(args[1], XtNlength, strlen(newhol));
        XawTextSetSource(editor->holiday,
                         XtCreateWidget("holSource", asciiSrcObjectClass,
                         editor->holiday, args, n), (XawTextPosition) 0);
        XtDestroyWidget(oldhol);
    }
    XtDestroyWidget(old);
#else
    XtSetArg(args[n], XmNeditable, True);
    n++;
    XtSetValues(editor->editor, args, n);
    XmTextSetString(editor->editor, newtext);
    if (!clear)
        if (strlen(newhol)) {
	    XmTextSetString(editor->holiday, newhol);
            XtManageChild(editor->holiday);
        }
        else
            XtUnmanageChild(editor->holiday);
#endif				/* ATHENA */
}

/*
 * StartEditor() - Pops up dayeditor.
 */

void
StartEditor(editor)
    DayEditor       editor;
{

    XtPopup(editor->shell, XtGrabNone);

}

/*
 * CloseEditor() - Pops down (and if needed, saves) dayeditor.
 */

void
CloseEditors()
{
    int             i;
    DayEditor       editor;

    if (editorList == NULL)
	return;

    for (i = 1; i <= ListLength(editorList); i++) {
	editor = GetEditorFromList(editorList, i);
	if (editor->saved == False)
	    SaveText(NULL, (XtPointer) editor, NULL);
    }
}

/*
 * read_file() - reads a file.
 */

int
read_file(filename, buffer, size)
    char           *filename;
    char           *buffer;
    int             size;
{
    int             fd, len;
    struct stat     sb;

    if ((fd = open(filename, O_RDONLY, 0666)) < 0) {
	if (errno == ENOENT)
	    return 0;
	else {
	    perror("xcalendar:open");
	    return (-1);
	}
    }
    if (fstat(fd, &sb) != 0) {
	close(fd);
	perror("xcalendar:fstat:");
	return (-1);
    }
    if (sb.st_size == 0) {
	unlink(filename);
	return 0;
    }
    len = (size < sb.st_size) ? size : sb.st_size;

    if (read(fd, buffer, len) < len) {
	close(fd);
	fprintf(stderr, "Couldn't read all file %s\n", filename);
	perror("xcalendar:read:");
	return (-1);
    }
    close(fd);

    return sb.st_size;

}

/*
 * write_to_file() - writes to file.
 */

int
write_to_file(filename, buffer, len)
    char           *filename;
    char           *buffer;
    int             len;
{
    int             fd;

    if ((fd = open(filename, O_WRONLY | O_TRUNC | O_CREAT, 0666)) < 0) {
	perror("xcalendar:open");
	return (-1);
    }
    if (len == 0) {
	unlink(filename);
	return 0;
    }
    if (write(fd, buffer, len) < len) {
	fprintf(stderr, "Sorry couldn't write all file %s\n", filename);
	perror("xcalendar:write:");
	close(fd);
	return (-1);
    }
    close(fd);

    return len;
}

/*
 * GetHoliday() - returns holiday text associated with editor.
 */

char           *
GetHoliday(editor)
    DayEditor       editor;
{
    int             i;
    struct holiday *holi;

    for (i = 0; i < holilist->pos; i++) {
	holi = *((Holiday *) holilist->list + i);
	if ((holi->day == editor->day) &&
	    (holi->month == editor->month) &&
	    (holi->year == editor->year))
	    return (holi->text);
    }
    return ("");
}
