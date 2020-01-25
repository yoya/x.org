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


/** Authors:  Roman J. Budzianowski - Project Athena, MIT **/
/**           Richard Bingle - Comp. Sci. Dept., Purdue University **/
/** Version 4.0  **/

#include "xcalendar.h"

Cardinal        displayedMonth, displayedYear;
Widget          toplevel;
Display        *dsp;
int             debug = 0;
List            daylist;
List            holilist;
void            FormGeometryHandler();
LList          *dayEntryList;
Cardinal        day, month, year;
Boolean         dayEntryState = False;
Arg             markedDayColors[2];
Arg             unmarkedDayColors[2];
void            ShowEntries();
Window          markedDaysKey;
Window          allDaysKey;
LList          *allDaysList;
Widget          date;
Widget          daynumbers;
Widget          help_button;
Widget          cdate;
char           *smonth[13];
Cardinal        defFirstDay = 1;

XtActionsRec    appActions[] = {
    {"ShowEntries", ShowEntries},
};

String          defTranslations =
"<Btn1Down>:ShowEntries()";

#define SetResOffset(n,field) \
  appResourceList[n].resource_offset = ((char *)&appResources.field - (char *)&appResources)
#define offset(field) XtOffset(struct _appRes *, field)

AppResourcesRec appResources;	/* application specific resources */

XtResource      appResourceList[] = {
    {"reverseVideoMark", "ReverseVideo", XtRBoolean, sizeof(Boolean),
    offset(reverseVideo), XtRString, "False"},
    {"setMarkForeground", "SetForeground", XtRBoolean, sizeof(Boolean),
    offset(setForeground), XtRString, "False"},
    {"markForeground", "MarkForeground", XtRPixel, sizeof(Pixel),
    offset(markForeground), XtRString, XtDefaultForeground},
    {"setMarkBackground", "SetBackground", XtRBoolean, sizeof(Boolean),
    offset(setBackground), XtRString, "False"},
    {"markBackground", "MarkBackground", XtRPixel, sizeof(Pixel),
    offset(markBackground), XtRString, XtDefaultBackground},
    {"monthnames", "Monthnames", XtRString, sizeof(String),
    offset(monthnames), XtRString, "January/February/March/April/May/June/July/August/September/October/November/December"},
    {"monthnms", "Monthnms", XtRString, sizeof(String),
    offset(monthnms), XtRString, "Jan/Feb/Mar/Apr/May/Jun/Jul/Aug/Sep/Oct/Nov/Dec"},
    {"daynames", "Daynames", XtRString, sizeof(String),
    offset(daynames), XtRString, "Sunday/Monday/Tuesday/Wednesday/Thursday/Friday/Saturday"},
    {"daynms", "Daynms", XtRString, sizeof(String),
    offset(daynms), XtRString, "Sun/Mon/Tue/Wed/Thu/Fri/Sat"},
    {"date", "Date", XtRString, sizeof(String),
    offset(date), XtRString, "%W, %M %d"},
    {"january", "January", XtRString, sizeof(String),
    offset(months[0]), XtRString, "January"},
    {"february", "February", XtRString, sizeof(String),
    offset(months[1]), XtRString, "February"},
    {"march", "March", XtRString, sizeof(String),
    offset(months[2]), XtRString, "March"},
    {"april", "April", XtRString, sizeof(String),
    offset(months[3]), XtRString, "April"},
    {"may", "May", XtRString, sizeof(String),
    offset(months[4]), XtRString, "May"},
    {"june", "June", XtRString, sizeof(String),
    offset(months[5]), XtRString, "June"},
    {"july", "July", XtRString, sizeof(String),
    offset(months[6]), XtRString, "July"},
    {"august", "August", XtRString, sizeof(String),
    offset(months[7]), XtRString, "August"},
    {"september", "September", XtRString, sizeof(String),
    offset(months[8]), XtRString, "September"},
    {"october", "October", XtRString, sizeof(String),
    offset(months[9]), XtRString, "October"},
    {"november", "November", XtRString, sizeof(String),
    offset(months[10]), XtRString, "November"},
    {"december", "December", XtRString, sizeof(String),
    offset(months[11]), XtRString, "December"},
    {"firstDay", "FirstDay", XtRInt, sizeof(int),
    offset(firstDay), XtRInt, (XtPointer) & defFirstDay},
    {"markOnStartup", "MarkOnStarup", XtRBoolean, sizeof(Boolean),
    offset(markOnStartup), XtRString, "False"},
    {"helpFile", "HelpFile", XtRString, sizeof(String),
    offset(helpFile), XtRString, "/usr/lib/X11/xcalendar/xcalendar.hlp"},
    {"textBufferSize", "TextBufferSize", XtRInt, sizeof(int),
    offset(textBufferSize), XtRString, "2048"},
    {"calendarDir", "CalendarDir", XtRString, sizeof(String),
    offset(calendarDir), XtRImmediate, NULL},
    {"holidayFile", "HolidayFile", XtRString, sizeof(String),
    offset(holidayFile), XtRImmediate, "/usr/lib/X11/xcalendar/xcalendar.hol"},
    {"oldStyle", "OldStyle", XtRBoolean, sizeof(Boolean),
    offset(oldStyle), XtRString, "True"},
    {"markCurrent", "MarkCurrent", XtRBoolean, sizeof(Boolean),
    offset(markCurrent), XtRString, "False"},
    {"updateCurrent", "UpdateCurrent", XtRInt, sizeof(int),
    offset(updateCurrent), XtRString, "60"},
    {"currentForeground", "CurrentForeground", XtRPixel, sizeof(Pixel),
    offset(currentForeground), XtRString, XtDefaultForeground},
    {"markHoliday", "MarkHoliday", XtRBoolean, sizeof(Boolean),
    offset(markHoliday), XtRString, "False"},
    {"holidayForeground", "HolidayForeground", XtRPixel, sizeof(Pixel),
    offset(holidayForeground), XtRString, XtDefaultForeground},
};

static int      marked_day;

Arg             markColors[2];
Cardinal        nMark = 0;

static XtCallbackRec callbacks[] = {
    {NULL, NULL},
    {NULL, NULL},
};

/*
 * quit() - Quit callback.
 */

void
quit(w, closure, call_data)
    Widget          w;
    XtPointer       closure;
    XtPointer       call_data;
{
    CloseEditors();
    exit(0);
}

char           *helpText =
"\n\
The full text of help is not available, because the help file\n\
couldn't be opened. Set the resource 'helpFile' to\n\
the full pathname of the xcalendar.hlp on your system.\n\
\n\
XCalendar is a simple interactive calendar program with a notebook\n\
capability. It is built on the X Toolkit and uses either the Athena\n\
or Motif Widgets.\n\
\n\
You can start the program as:  xcalendar [month [year]]\n\
\n\
CREDITS:\n\
      Version 4.0 \n\
      Authors: Roman J. Budzianowski - MIT Project Athena\n\
                    Copyright 1988 Massachusetts Institute of Technology\n\
               Richard Bingle - Comp. Sci. Dept., Purdue University\n\
                    Copyright 1990 Purdue University\n\
      Contributions:\n\
               Beth Chaney - Comp. Sci. Dept., Purdue University\n\
               Mike Urban - JPL, NASA\n\
               Joel Neisen - Minnesota Supercomputer Center\n\
               Hiroshi Kuribayashi - Omron Corp.\n\
\n\
";

String          on[1] = {"on"};

/*
 * help() - Help callback.
 */

void
help(w, closure, call_data)
    Widget          w;
    XtPointer       closure;
    XtPointer       call_data;
{
    static DayEditor help = NULL;
    char           *iconName = "xcalendar: help";
    Arg             args[10];
    int             n;

    if (!help) {
#ifdef ATHENA
	help = CreateDayEditor("helpWindow", XawtextWrapNever, NULL);
#else
	help = CreateMotifDayEditor("helpWindow", NULL, NULL);
#endif

	help->button = w;
	help->used = True;
	help->saved = True;
	help->filename = appResources.helpFile;

	if (ReadFile(help) == False || EmptyBuffer(help)) {
	    XtFree(help->buffer);
	    (void) ChangeTextSource(help, helpText);
	}
	XtUnmanageChild(help->saveButton);
	XtUnmanageChild(help->clearEntry);

	XtUnmanageChild(help->prevday);
	XtUnmanageChild(help->succday);

	XtUnmanageChild(help->holiday);

	n = 0;
#ifdef ATHENA
	XtSetArg(args[n], XtNeditType, XawtextRead);
	n++;
#else
	XtSetArg(args[n], XmNeditable, False);
	n++;
#endif
	XtSetValues(help->editor, args, n);
    }
    n = 0;
    XtSetArg(args[n], XtNiconName, iconName);
    n++;
    XtSetValues(help->shell, args, n);

#ifndef ATHENA
    n = 0;
    XtSetArg(args[n], XmNleftPosition, 1);
    n++;
    XtSetArg(args[n], XmNrightPosition, 2);
    n++;
    XtSetValues(help->doneButton, args, n);
#endif

    StartEditor(help);
}

/*
 * ChangeMonth() - changes view to month and year specified by the global
 * variable month and year.
 */

void
ChangeMonth()
{
    Widget          CreateCalendarFrame();
    void            prevmonth(), succmonth();
    Cardinal        numberOfDays, firstDay;
    Arg             args[10];
    int             n;
    char            title[80];
    void            NewDayNumbers();
#ifndef ATHENA
    XmString        xms;
#endif

    /**  in order to add the new buttons **/

    sprintf(title, "%s %d", smonth[month], year);

    firstDay = FirstDay(month, year);
    numberOfDays = NumberOfDays(month, year);


    /* date label */

    n = 0;
#ifdef ATHENA
    XtSetArg(args[n], XtNlabel, title);
    n++;
    XtSetValues(date, args, n);
#else
    xms = XMS(title);
    XtSetArg(args[n], XmNlabelString, xms);
    n++;
    XtSetValues(date, args, n);
    XmStringFree(xms);
#endif

    /* call NewDayNumbers */
    NewDayNumbers(numberOfDays, firstDay);
}

/*
 * prevmonth() - Sets month (and if needed, year) to the prior month.
 */

void
prevmonth(w, closure, call_data)
    Widget          w;
    XtPointer       closure;
    XtPointer       call_data;

{
    if (displayedMonth == 1) {
	displayedMonth = month = 12;
	displayedYear = year = --displayedYear;
    } else
	displayedMonth = month = --displayedMonth;

    ChangeMonth();
}

/*
 * succmonth() - Sets month (and if needed, year) to the next month.
 */

void
succmonth(w, closure, call_data)
    Widget          w;
    XtPointer       closure;
    XtPointer       call_data;

{
    if (displayedMonth == 12) {
	displayedMonth = month = 1;
	displayedYear = year = ++displayedYear;
    } else
	displayedMonth = month = ++displayedMonth;
    ChangeMonth();
}

/*
 * main() - Main routine.
 */

void
main(argc, argv)
    int             argc;
    char          **argv;
{
    Arg             args[10];
    int             n;
    char            title[80];
    char            iconName[80];
    int             i, resNo = 0;
    Widget          frame;

#ifdef XI18N
    XtSetLanguageProc(NULL, NULL, NULL);
#endif
    toplevel = XtInitialize(NULL, "XCalendar", NULL, 0, &argc, argv);

    dsp = XtDisplay(toplevel);

    /* get application specific resources */

    if (XtScreen(toplevel)->root_visual->class == StaticGray)
	appResourceList[0].default_addr = "True";

    XtGetApplicationResources(toplevel, (XtPointer) & appResources,
			      appResourceList, XtNumber(appResourceList),
			      NULL, 0);

    for (i = 0; i < 12; i++) {
	smonth[i + 1] = appResources.months[i];
    }

    if (appResources.setBackground) {

	XtSetArg(markColors[nMark], XtNbackground, appResources.markBackground);
	nMark++;
    }
    if (appResources.setForeground) {

	XtSetArg(markColors[nMark], XtNforeground, appResources.markForeground);
	nMark++;
    }

#ifndef ATHENA
    XtAddEventHandler(toplevel, (EventMask)0, True,
                      _XEditResCheckMessages, NULL);
#endif

    LoadHolidays();

    day = current_day();
    month = current_month();
    year = current_year();

    if (argc >= 2)
	month = atoi(argv[1]);
    if (!month)
	month = current_month();

    if (argc == 3)
	year = atoi(argv[2]);
    if (!year)
	year = current_year();

    displayedMonth = month;
    displayedYear = year;

    sprintf(iconName, "xcalendar: %s %d", smonth[displayedMonth],
	    displayedYear);

    n = 0;
    XtSetArg(args[n], XtNiconName, iconName);
    n++;
    XtSetValues(toplevel, args, n);

    sprintf(title, "%s %d", smonth[month], year);

#ifdef ATHENA
    frame = CreateCalendar(toplevel);
#else
    frame = CreateMotifCalendar(toplevel);
#endif

    /*
     * Create the windows
     */

    XtRealizeWidget(toplevel);

    allDaysKey = XtWindow(toplevel);
    allDaysList = NULL;
    markedDaysKey = XtWindow(frame);
    MakeDayList();

    /** mark the entries **/

    if (appResources.markOnStartup == True) {
	n = 1;
	ShowEntries(toplevel, NULL, on, &n);
    }
    marked_day = -1;
    if (appResources.updateCurrent <= 0)
	appResources.updateCurrent = 60;
    CurrentTic(toplevel, NULL);

    /*
     * Now process the events.
     */
    XtMainLoop();
}

/*
 * ShowEntries() - Toggles the marking of days with something in them.
 */

void
ShowEntries(w, event, params, numb)
    Widget          w;
    XEvent         *event;
    String         *params;
    Cardinal       *numb;
{
    LList          *listp;
    Arg             colors[2];
    Arg            *args = colors;
    Cardinal        n = 2;

    initialized = False;

    if (!(initialized || InitEditors()))
	return;

    if (numb && *numb) {
	if (!strcmp(params[0], "on"))
	    dayEntryState = True;
	else if (!strcmp(params[0], "off"))
	    dayEntryState = False;
    } else {
	if (dayEntryState)
	    dayEntryState = False;
	else
	    dayEntryState = True;
    }


    listp = allDaysList;
    if (allDaysList)
	do {

	    XtSetArg(colors[0], XtNforeground, DayForeground(listp));

	    if (is_holiday((Widget) listp->element) &&
		appResources.markHoliday) {
		XtSetArg(colors[0], XtNforeground,
			 appResources.holidayForeground);
	    }
	    if (is_today((Widget) listp->element) && appResources.markCurrent) {
		XtSetArg(colors[0], XtNforeground,
			 appResources.currentForeground);
	    }
	    XtSetValues((Widget) listp->element, colors, 1);

	    listp = listp->next;

	} while (listp != allDaysList);


    listp = dayEntryList;

    if (dayEntryList)
	do {
	    if (!dayEntryState) {
		XtSetArg(colors[0], XtNbackground, DayBackground(listp));
		XtSetArg(colors[1], XtNforeground, DayForeground(listp));
	    } else {
		if (appResources.reverseVideo) {
		    XtSetArg(colors[0], XtNbackground, DayForeground(listp));
		    XtSetArg(colors[1], XtNforeground, DayBackground(listp));
		} else {
		    args = markColors;
		    n = nMark;
		}
	    }
	    XtSetValues((Widget) listp->element, args, n);

	    if (is_holiday((Widget) listp->element) &&
		appResources.markHoliday) {
		XtSetArg(colors[0], XtNforeground,
			 appResources.holidayForeground);
		XtSetValues((Widget) listp->element, colors, 1);
	    }
	    if (is_today((Widget) listp->element) &&
		appResources.markCurrent) {
		XtSetArg(colors[0], XtNforeground,
			 appResources.currentForeground);
		XtSetValues((Widget) listp->element, colors, 1);
	    }
	    listp = listp->next;

	} while (listp != dayEntryList);
}

/*
 * MarkDayEntry() - Adds or removes days (widgets) from the marked
 * list.
 */

void
MarkDayEntry(w, doFlag)
    Widget          w;
    Boolean         doFlag;
{
    Arg             colors[10];
    int             n;
    LList          *lp;
    LList          *lp2;

    if (doFlag == True) {
	if (lookup(w, markedDaysKey, dayEntryList) != NULL)
	    return;		/* it's already on the list */

	dayEntryList = (LList *) pput(w, markedDaysKey, dayEntryList);
	dayEntryList->data = XtMalloc(sizeof(MarkColors));
	lp2 = (LList *) lookup(w, allDaysKey, allDaysList);
	DayBackground(dayEntryList) =
	    DayBackground(lp2);
	DayForeground(dayEntryList) =
	    DayForeground(lp2);

	if (dayEntryState == True) {
	    if (appResources.reverseVideo) {
		n = 0;
		XtSetArg(colors[n], XtNbackground, DayForeground(dayEntryList));
		n++;
		XtSetArg(colors[n], XtNforeground, DayBackground(dayEntryList));
		n++;
		XtSetValues(w, colors, n);
	    } else
		XtSetValues(w, markColors, nMark);
	}
	if (is_holiday(w) && appResources.markHoliday) {
	    n = 0;
	    XtSetArg(colors[n], XtNforeground, appResources.holidayForeground);
	    n++;
	    XtSetValues(w, colors, n);
	}
	if (is_today(w) && appResources.markCurrent) {
	    n = 0;
	    XtSetArg(colors[n], XtNforeground, appResources.currentForeground);
	    n++;
	    XtSetValues(w, colors, n);
	}
    } else {
	if ((lp = (LList *) lookup(w, markedDaysKey, dayEntryList)) == NULL) {
	    return;		/* it's not on the list */
	}
	if (dayEntryState == True) {
	    dayEntryList = lp;
	    n = 0;
	    XtSetArg(colors[n], XtNbackground, DayBackground(dayEntryList));
	    n++;
	    XtSetArg(colors[n], XtNforeground, DayForeground(dayEntryList));
	    n;
	    if (is_holiday(w) && appResources.markHoliday)
		XtSetArg(colors[n], XtNforeground,
			 appResources.holidayForeground);
	    if (is_today(w) && appResources.markCurrent)
		XtSetArg(colors[n], XtNforeground,
			 appResources.currentForeground);
	    n++;
	    XtSetValues(w, colors, n);
	}
	XtFree(dayEntryList->data);
	dayEntryList = (LList *) premove(w, markedDaysKey, dayEntryList);
    }
}

/*
 * CreateCalendarFrame() - Creates the calendar pane and calls
 * other routines responsible for creating day labels and buttons.
 */

Widget
CreateCalendarFrame(parent, args, nargs, numDays, firstDay)
    Widget          parent;
    Cardinal        nargs;
    ArgList         args;
    Cardinal        numDays, firstDay;
{
    Widget          calendar, CreateDayLabels();
    void            CreateDayNumbers();
    Widget          widget;
    Arg             bargs[2];

    /* callist = CreateList(2,sizeof(Widget)); */

#ifdef ATHENA
    calendar = XtCreateManagedWidget("calendar", panedWidgetClass,
#else
    calendar = XtCreateManagedWidget("calendar", xmPanedWindowWidgetClass,
#endif
				     parent, args, nargs);

    widget = CreateDayLabels(calendar);

    CreateDayNumbers(calendar, numDays, firstDay);
    widget = daynumbers;

#ifndef ATHENA
    HideSashes(calendar);
#endif

    return calendar;

}

#ifdef ATHENA

/*
 * CreateDayNumbers() - Creates the buttons for the days of the month.
 */

void
CreateDayNumbers(parent, numDays, firstDay)
    Widget          parent;
    Cardinal        numDays, firstDay;
{
    Widget          prevButton = NULL;
    Cardinal        numButtons = 42;
    Arg             bargs[10];
    int             n;
    int             i;
    char            buttonText[4];
    char            buttonName[8];
    void            EditDayEntry();

#ifdef notdef
    if (firstDay + numDays - 1 > 35)
	numButtons = 42;
    else
	numButtons = 35;
#endif

    daylist = CreateList(numButtons, sizeof(Widget));

    n = 0;
    daynumbers = XtCreateManagedWidget("daynumbers", formWidgetClass,
				       parent, bargs, n);

    callbacks[0].callback = EditDayEntry;

    XtSetArg(bargs[0], XtNcallback, callbacks);

    for (i = 1; i <= numButtons; i++) {
	n = 1;

	if (i < firstDay || i > firstDay + numDays - 1) {
	    XtSetArg(bargs[n], XtNmappedWhenManaged, False);
	    n++;
	    callbacks[0].closure = NULL;
	} else {
	    XtSetArg(bargs[n], XtNmappedWhenManaged, True);
	    n++;
	    callbacks[0].closure = (XtPointer) (i - firstDay + 1);
	}

	if ((i - 1) % 7 == 0) {
	    XtSetArg(bargs[n], XtNfromHoriz, NULL);
	    n++;
	} else {
	    XtSetArg(bargs[n], XtNfromHoriz, prevButton);
	    n++;
	}

	if (i <= 7) {
	    XtSetArg(bargs[n], XtNfromVert, NULL);
	    n++;
	} else {
	    XtSetArg(bargs[n], XtNfromVert, GetWidgetFromList(daylist, i - 7));
	    n++;
	}

	sprintf(buttonText, "%3.d", i - firstDay + 1);
	XtSetArg(bargs[n], XtNlabel, buttonText);
	n++;

	/**       sprintf(buttonName,"%d",(i-1)%7 + 1);  **/
	sprintf(buttonName, "%d", i);

	prevButton = XtCreateManagedWidget(buttonName, commandWidgetClass,
					   daynumbers, bargs, n);

	(void) PushWidgetOnList(daylist, prevButton);
    }

}

/*
 * NewDayNumbers() - Changes the buttons to reflect chosen month.
 */

void
NewDayNumbers(numDays, firstDay)
    Cardinal        numDays, firstDay;
{
    Cardinal        numButtons = 42;
    Arg             bargs[10];
    int             n;
    int             i;
    char            buttonText[4];
    char            buttonName[8];
    void            EditDayEntry();
    Widget          tempWidget = NULL;

    XtUnmapWidget(daynumbers);

    callbacks[0].callback = EditDayEntry;

    XtSetArg(bargs[0], XtNcallback, callbacks);

    for (i = 1; i <= numButtons; i++) {
	n = 1;

	tempWidget = GetWidgetFromList(daylist, i);

	if (i < firstDay || i > firstDay + numDays - 1) {
	    XtSetArg(bargs[n], XtNmappedWhenManaged, False);
	    n++;
	    callbacks[0].closure = NULL;
	} else {
	    XtSetArg(bargs[n], XtNmappedWhenManaged, True);
	    n++;
	    callbacks[0].closure = (XtPointer) (i - firstDay + 1);
	}

	if ((i - 1) % 7 == 0) {
	    XtSetArg(bargs[n], XtNfromHoriz, NULL);
	    n++;
	} else {
	    XtSetArg(bargs[n], XtNfromHoriz, GetWidgetFromList(daylist, i - 1));
	    n++;
	}


	MarkDayEntry(tempWidget, False);

	if (i <= 7) {
	    XtSetArg(bargs[n], XtNfromVert, NULL);
	    n++;
	} else {
	    XtSetArg(bargs[n], XtNfromVert, GetWidgetFromList(daylist, i - 7));
	    n++;
	}


	sprintf(buttonText, "%3.d", i - firstDay + 1);
	XtSetArg(bargs[n], XtNlabel, buttonText);
	n++;

	sprintf(buttonName, "%d", i);

	XtSetValues(tempWidget, bargs, n);
    }

    if (dayEntryState == True) {
	n = 1;
	ShowEntries(NULL, NULL, on, &n);
    }
    XtMapWidget(daynumbers);
}

/*
 * CreateDayLabels() - Creates the day name labels (SUN,...).
 */

Widget
CreateDayLabels(parent)
    Widget          parent;
{
    Widget          prevButton = NULL, daynames;
    Arg             bargs[10];
    int             n;
    int             i, day;
    static char    *names[8] =
    {NULL, "MON", "TUE", "WED", "THU", "FRI", "SAT", "SUN"};

    /* end of declarations */

    n = 0;
    daynames = XtCreateManagedWidget("daynames", formWidgetClass,
				     parent, bargs, n);

    for (i = 1; i <= 7; i++) {
	n = 0;
	XtSetArg(bargs[n], XtNfromHoriz, prevButton);
	n++;
	day = (i + appResources.firstDay - 1) % 7;
	day = day > 0 ? day : 7;
	prevButton = XtCreateManagedWidget(names[day],
				      labelWidgetClass, daynames, bargs, n);
    }

    return daynames;

}

#else

/*
 * CreateDayNumbers() - Creates the buttons for the days of the month.
 */

void
CreateDayNumbers(parent, numDays, firstDay)
    Widget          parent;
    Cardinal        numDays, firstDay;
{
    Widget          prevButton = NULL;
    Cardinal        numButtons = 42;
    Arg             bargs[20];
    int             n;
    int             i;
    char            buttonText[10];
    char            buttonName[8];
    void            EditDayEntry();
    XmString        xms;

#ifdef notdef
    if (firstDay + numDays - 1 > 35)
	numButtons = 42;
    else
	numButtons = 35;
#endif

    daylist = CreateList(numButtons, sizeof(Widget));

    n = 0;
    XtSetArg(bargs[n], XmNfractionBase, 42);
    n++;
    daynumbers = XtCreateManagedWidget("daynumbers", xmFormWidgetClass,
				       parent, bargs, n);

    callbacks[0].callback = EditDayEntry;


    for (i = 1; i <= numButtons; i++) {
	n = 0;

	if (i < firstDay || i > firstDay + numDays - 1) {
	    XtSetArg(bargs[n], XmNmappedWhenManaged, False);
	    n++;
	    callbacks[0].closure = NULL;
	} else {
	    XtSetArg(bargs[n], XmNmappedWhenManaged, True);
	    n++;
	    callbacks[0].closure = (XtPointer) (i - firstDay + 1);
	}
	XtSetArg(bargs[n], XmNactivateCallback, callbacks);
	n++;

	XtSetArg(bargs[n], XmNleftAttachment, XmATTACH_POSITION);
	n++;
	XtSetArg(bargs[n], XmNleftPosition, ((i - 1) % 7) * 6);
	n++;
	XtSetArg(bargs[n], XmNrightAttachment, XmATTACH_POSITION);
	n++;
	XtSetArg(bargs[n], XmNrightPosition, (((i - 1) % 7) + 1) * 6);
	n++;
	XtSetArg(bargs[n], XmNtopAttachment, XmATTACH_POSITION);
	n++;
	XtSetArg(bargs[n], XmNtopPosition, ((int) ((i - 1) / 7)) * 7);
	n++;
	XtSetArg(bargs[n], XmNbottomAttachment, XmATTACH_POSITION);
	n++;
	XtSetArg(bargs[n], XmNbottomPosition, ((int) ((i - 1) / 7) + 1) * 7);
	n++;

	sprintf(buttonText, "%2.d", i - firstDay + 1);
	xms = XMS(buttonText);
	XtSetArg(bargs[n], XmNlabelString, xms);
	n++;

	/**       sprintf(buttonName,"%d",(i-1)%7 + 1);  **/
	sprintf(buttonName, "%d", i);

	prevButton = XtCreateManagedWidget(buttonName, xmPushButtonWidgetClass,
					   daynumbers, bargs, n);

	(void) PushWidgetOnList(daylist, prevButton);
	XmStringFree(xms);
    }
}

/*
 * NewDayNumbers() - Changes the buttons to reflect chosen month.
 */

void
NewDayNumbers(numDays, firstDay)
    Cardinal        numDays, firstDay;
{
    Cardinal        numButtons = 42;
    Arg             bargs[10];
    int             i;
    char            buttonText[4];
    char            buttonName[8];
    void            EditDayEntry();
    int             n;
    XmString        xms;
    Widget          tempWidget = NULL;

    XtUnmapWidget(daynumbers);

    callbacks[0].callback = EditDayEntry;

    for (i = 1; i <= numButtons; i++) {
	n = 0;

	tempWidget = GetWidgetFromList(daylist, i);

	if (i < firstDay || i > firstDay + numDays - 1) {
	    XtSetArg(bargs[n], XmNmappedWhenManaged, False);
	    n++;
	    callbacks[0].closure = NULL;
	} else {
	    XtSetArg(bargs[n], XmNmappedWhenManaged, True);
	    n++;
	    callbacks[0].closure = (XtPointer) (i - firstDay + 1);
	}
	XtSetArg(bargs[n], XmNactivateCallback, callbacks);
	n++;

	if ((i - 1) % 7 == 0) {
	    XtSetArg(bargs[n], XmNleftAttachment, XmATTACH_FORM);
	    n++;
	} else {
	    XtSetArg(bargs[n], XmNleftAttachment, XmATTACH_WIDGET);
	    n++;
	    XtSetArg(bargs[n], XmNleftWidget,
		     GetWidgetFromList(daylist, i - 1));
	    n++;
	}

	MarkDayEntry(tempWidget, False);

	if (i <= 7) {
	    XtSetArg(bargs[n], XmNtopAttachment, XmATTACH_FORM);
	    n++;
	} else {
	    XtSetArg(bargs[n], XmNtopAttachment, XmATTACH_WIDGET);
	    n++;
	    XtSetArg(bargs[n], XmNtopWidget, GetWidgetFromList(daylist, i - 7));
	    n++;
	}


	sprintf(buttonText, "%2.d", i - firstDay + 1);
	xms = XMS(buttonText);
	XtSetArg(bargs[n], XmNlabelString, xms);
	n++;

	sprintf(buttonName, "%d", i);

	XtSetValues(tempWidget, bargs, n);
	XmStringFree(xms);
    }

    if (dayEntryState == True) {
	n = 1;
	ShowEntries(NULL, NULL, on, &n);
    }
    XtMapWidget(daynumbers);
}

/*
 * CreateDayLabels() - Creates the day name labels (SUN,...).
 */

Widget
CreateDayLabels(parent)
    Widget          parent;
{
    Widget          prevButton = NULL, daynames;
    Arg             bargs[10];
    int             i, n, day;
    XtWidgetGeometry size;
    static char    *names[8] =
    {NULL, "MON", "TUE", "WED", "THU", "FRI", "SAT", "SUN"};

    /* end of declarations */

    n = 0;
    XtSetArg(bargs[n], XmNfractionBase, 7);
    n++;
    daynames = XtCreateManagedWidget("daynames", xmFormWidgetClass,
				     parent, bargs, n);

    for (i = 1; i <= 7; i++) {
	n = 0;
	XtSetArg(bargs[n], XmNleftAttachment, XmATTACH_POSITION);
	n++;
	XtSetArg(bargs[n], XmNleftPosition, i - 1);
	n++;
	XtSetArg(bargs[n], XmNrightAttachment, XmATTACH_POSITION);
	n++;
	XtSetArg(bargs[n], XmNrightPosition, i);
	n++;

	day = (i + appResources.firstDay - 1) % 7;
	day = day > 0 ? day : 7;
	prevButton = XtCreateManagedWidget(names[day],
				    xmLabelWidgetClass, daynames, bargs, n);
    }

    size.request_mode = CWHeight;
    XtQueryGeometry(daynames, NULL, &size);

    n = 0;
    XtSetArg(bargs[n], XmNpaneMinimum, size.height);
    n++;
    XtSetArg(bargs[n], XmNpaneMaximum, size.height);
    n++;
    XtSetValues(daynames, bargs, n);

    return daynames;
}

#endif

static struct tm *today = NULL;

/*
 * gettime() - Updates the shared tm structure to the current time.
 */

struct tm      *
gettime()
{
    struct timeval  t;

    gettimeofday(&t, NULL);
    today = localtime(&t.tv_sec);
    return today;
}

/*
 * current_month() - Returns the number of the current month (1-12).
 */

int
current_month()
{
    if (today == NULL)
	(void) gettime();
    return (today->tm_mon + 1);
}

/*
 * current_day() - Returns the day of the month of the current day.
 */

int
current_day()
{
    if (today == NULL)
	(void) gettime();
    return (today->tm_mday);
}

/*
 * current_year() - Returns the current year (yyyy).
 */

int
current_year()
{
    if (today == NULL)
	(void) gettime();
    return (today->tm_year + 1900);
}

char            mon[] = {
    0,
    31, 29, 31, 30,
    31, 30, 31, 31,
    30, 31, 30, 31,
};

static int      calInit = 0;

/*
 * NumberOfDays() - Returns the number of days for month of year.
 */

int
NumberOfDays(m, y)
    int             m, y;
{
    if (calInit != y)
	FirstDay(m, y);
    return mon[m];
}

/* should be called first */

/*
 * FirstDay() - Returns the day of the week of the first day of the
 * given month and year.
 */

int
FirstDay(m, y)
    int             m, y;
{
    register        d, i;

    calInit = y;
    d = jan1(y);
    mon[2] = 29;
    mon[9] = 30;

    switch ((jan1(y + 1) + 7 - d) % 7) {

	/*
	 * non-leap year
	 */
    case 1:
	mon[2] = 28;
	break;

	/*
	 * 1752
	 */
    default:
	mon[9] = 19;
	break;

	/*
	 * leap year
	 */
    case 2:
	;
    }

    for (i = 1; i < m; i++)
	d += mon[i];
    d %= 7;

    d = d > 0 ? d : 7;		/* returns 1-7, not 0-6 */

    d = d + 1 - appResources.firstDay;
    return d > 0 ? d : 7 + d;

}

/*
 * jan1() - Returns day of the week of jan 1 of given year.
 */

int
jan1(yr)
    int             yr;
{
    register        y, d;

    /*
     * normal gregorian calendar one extra day per four years
     */

    y = yr;
    d = 4 + y + (y + 3) / 4;

    /*
     * julian calendar regular gregorian less three days per 400
     */

    if (y > 1800) {
	d -= (y - 1701) / 100;
	d += (y - 1601) / 400;
    }
    /*
     * great calendar changeover instant
     */

    if (y > 1752)
	d += 3;

    return (d % 7);
}

/*
 * CurrentTic() - Updates current day.
 */

void
CurrentTic(client_data, tid)
    XtPointer       client_data;
    XtIntervalId   *tid;
{
    int             day;
    char            sb[80];
    Arg             args[10];
    int             n;
#ifndef ATHENA
    XmString        xms;
#endif

    today = NULL;
    day = current_day();
    if (day != marked_day) {
	if ((day == 1) &&
	    ((displayedMonth != current_month()) ||
	     (displayedYear != current_year()))) {
	    displayedMonth = month = current_month();
	    displayedYear = year = current_year();
	    ChangeMonth();
	}
	marked_day = day;
	if (appResources.markCurrent) {
	    n = 1;
	    ShowEntries(NULL, NULL, on, &n);
	}
	Fill_In_Date(sb);
	n = 0;
#ifdef ATHENA
	XtSetArg(args[n], XtNlabel, sb);
	n++;
	XtSetValues(cdate, args, n);
#else
	xms = XMS(sb);
	XtSetArg(args[n], XmNlabelString, xms);
	n++;
	XtSetValues(cdate, args, n);
	XmStringFree(xms);
#endif
    }
    XtAppAddTimeOut(XtWidgetToApplicationContext((Widget) (client_data)),
		    appResources.updateCurrent * 1000, CurrentTic,
		    client_data);
}

/*
 * is_today() - Returns true if the widget represents today.
 */

Boolean
is_today(w)
    Widget          w;
{
    Widget          foo = GetWidgetFromList(daylist,
	       FirstDay(displayedMonth, displayedYear) - 1 + current_day());

    if ((displayedMonth == current_month()) &&
	(displayedYear == current_year())) {
	if (w == foo) {
	    return (True);
	} else {
	    return (False);
	}
    } else {
	return (False);
    }
}

/*
 * is_holiday() - Returns true if the widget is for a holiday.
 */

Boolean
is_holiday(w)
    Widget          w;
{
    int             i;
    struct holiday *holi;

    for (i = 0; i < holilist->pos; i++) {
	holi = *((Holiday *) holilist->list + i);
	if (((GetWidgetFromList(daylist,
				FirstDay(displayedMonth, displayedYear) -
				1 + holi->day)) == w) &&
	    (holi->month == displayedMonth) &&
	    (holi->year == displayedYear))
	    return (True);
    }
    return (False);
}

/*
 * MakeDayList() - Keeps a list of all days.
 */

void
MakeDayList()
{
    int             i;
    Widget          w;
    Arg             args[10];
    int             n;

    for (i = 1; i <= 42; i++) {
	n = 0;
	w = GetWidgetFromList(daylist, i);
	allDaysList = (LList *) pput(w, allDaysKey, allDaysList);
	allDaysList->data = XtMalloc(sizeof(MarkColors));
	XtSetArg(args[n], XtNbackground, &DayBackground(allDaysList));
	n++;
	XtSetArg(args[n], XtNforeground, &(DayForeground(allDaysList)));
	n++;
	XtGetValues(w, args, n);
    }
}

#ifdef ATHENA

/*
 * CreateCalendar() - Creates the main calendar pane and control areas.
 */

Widget
CreateCalendar(toplevel)
    Widget          toplevel;
{
    Widget          calendar, controls;
    Widget          prev_button, succ_button;
    Widget          quit_button;
    Widget          CreateCalendarFrame();
    Cardinal        numberOfDays, firstDay;
    Widget          frame;

    Arg             args[10];
    int             n;
    char            title[80];

    Widget          bcontrols;

    firstDay = FirstDay(month, year);
    numberOfDays = NumberOfDays(month, year);

    /*
     * Create the top level pane widget
     */

    frame = XtCreateWidget("xcalendar", panedWidgetClass, toplevel, NULL, 0);

    markedDaysKey = XtWindow(frame);

    /** create control panel **/

    n = 0;
    XtSetArg(args[n], XtNskipAdjust, True);
    n++;
    controls = XtCreateManagedWidget("controls", formWidgetClass, frame,
				     args, n);

    /* prev button */

    n = 0;
    callbacks[0].callback = prevmonth;
    XtSetArg(args[n], XtNcallback, callbacks);
    n++;
    XtSetArg(args[n], XtNfromHoriz, NULL);
    n++;
    XtSetArg(args[n], XtNleft, XtChainLeft);
    n++;
    XtSetArg(args[n], XtNright, XtChainLeft);
    n++;

    prev_button = XtCreateManagedWidget("prev", commandWidgetClass,
					controls, args, n);

    /* month label */

    sprintf(title, "%s %d", smonth[month], year);

    n = 0;
    XtSetArg(args[n], XtNlabel, title);
    n++;
    XtSetArg(args[n], XtNfromHoriz, prev_button);
    n++;
    XtSetArg(args[n], XtNleft, XtChainLeft);
    n++;
    XtSetArg(args[n], XtNright, XtChainRight);
    n++;

    date = XtCreateManagedWidget("date", labelWidgetClass, controls, args, n);

    XtAppAddActions(XtWidgetToApplicationContext(date), appActions, 1);

    XtAugmentTranslations(date, XtParseTranslationTable(defTranslations));

    /* next button */

    n = 0;
    callbacks[0].callback = succmonth;
    XtSetArg(args[n], XtNcallback, callbacks);
    n++;
    XtSetArg(args[n], XtNfromHoriz, date);
    n++;
    XtSetArg(args[n], XtNleft, XtChainRight);
    n++;
    XtSetArg(args[n], XtNright, XtChainRight);
    n++;

    succ_button = XtCreateManagedWidget("succ",
					commandWidgetClass,
					controls, args, n);

    /** end of control panel code **/

    /** create calendar form **/

    calendar = CreateCalendarFrame(frame, args, 0, numberOfDays, firstDay);

    n = 0;
    XtSetArg(args[n], XtNskipAdjust, True);
    n++;
    bcontrols = XtCreateManagedWidget("bcontrols", formWidgetClass,
				      frame, args, n);

    n = 0;
    callbacks[0].callback = quit;
    XtSetArg(args[n], XtNcallback, callbacks);
    n++;
    XtSetArg(args[n], XtNfromHoriz, NULL);
    n++;
    XtSetArg(args[n], XtNleft, XtChainLeft);
    n++;
    XtSetArg(args[n], XtNright, XtChainLeft);
    n++;

    quit_button = XtCreateManagedWidget("quitButton", commandWidgetClass,
					bcontrols, args, n);

    Fill_In_Date(title);

    n = 0;
    XtSetArg(args[n], XtNlabel, title);
    n++;
    XtSetArg(args[n], XtNfromHoriz, quit_button);
    n++;
    XtSetArg(args[n], XtNleft, XtChainLeft);
    n++;
    XtSetArg(args[n], XtNright, XtChainRight);
    n++;

    cdate = XtCreateManagedWidget("cdate", labelWidgetClass, bcontrols,
				  args, n);

    XtAugmentTranslations(quit_button,
			  XtParseTranslationTable(defTranslations));

    n = 0;
    callbacks[0].callback = help;
    XtSetArg(args[n], XtNcallback, callbacks);
    n++;
    XtSetArg(args[n], XtNfromHoriz, cdate);
    n++;
    XtSetArg(args[n], XtNleft, XtChainRight);
    n++;
    XtSetArg(args[n], XtNright, XtChainRight);
    n++;

    help_button = XtCreateManagedWidget("helpButton",
					commandWidgetClass,
					bcontrols, args, n);

    /** end of top level form **/

    XtInstallAllAccelerators(frame, frame);

    XtManageChild(frame);
    return (frame);
}

#else

/*
 * HideSashes() - Removes Motif sashes from traversal list and view.
 */

void
HideSashes(w)
    Widget          w;
{
    Widget         *children;
    int             num;
    Arg             args[10];
    int             n;

    n = 0;
    XtSetArg(args[n], XmNchildren, &children);
    n++;
    XtSetArg(args[n], XmNnumChildren, &num);
    n++;
    XtGetValues(w, args, n);

    while (num-- > 0)
	if (XmIsSash(children[num])) {
	    n = 0;
	    XtSetArg(args[n], XmNmappedWhenManaged, False);
	    n++;
	    XtSetArg(args[n], XmNtraversalOn, False);
	    n++;
	    XtSetValues(children[num], args, n);
	}
}

/*
 * CreateMotifCalendar() - Creates the main calendar pane and control areas.
 */

Widget
CreateMotifCalendar(toplevel)
    Widget          toplevel;
{
    Widget          calendar, controls;
    Widget          prev_button, succ_button;
    Widget          quit_button;
    Widget          CreateCalendarFrame();
    Cardinal        numberOfDays, firstDay;
    Widget          frame;
#ifndef ATHENA
    XmString        xms;
#endif
    Arg             args[10];
    char            title[80];
    Widget          bcontrols;
    int             n;
    XtWidgetGeometry size;

    firstDay = FirstDay(month, year);
    numberOfDays = NumberOfDays(month, year);

    /*
     * Create the top level form widget
     */

    n = 0;
    frame = XtCreateWidget("xcalendar", xmPanedWindowWidgetClass, toplevel,
			   args, n);

    markedDaysKey = XtWindow(frame);

    /** create control panel **/

    n = 0;
    controls = XtCreateManagedWidget("controls", xmFormWidgetClass, frame,
				     args, n);

    /* prev button */

    n = 0;
    callbacks[0].callback = prevmonth;
    XtSetArg(args[n], XmNactivateCallback, callbacks);
    n++;

    prev_button = XtCreateManagedWidget("prev", xmPushButtonWidgetClass,
					controls, args, n);

    /* next button */

    n = 0;
    callbacks[0].callback = succmonth;
    XtSetArg(args[n], XmNactivateCallback, callbacks);
    n++;
    XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM);
    n++;

    succ_button = XtCreateManagedWidget("succ",
					xmPushButtonWidgetClass,
					controls, args, n);

    /* month label */

    sprintf(title, "%s %d", smonth[month], year);

    n = 0;
    xms = XMS(title);
    XtSetArg(args[n], XmNlabelString, xms);
    n++;
    XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET);
    n++;
    XtSetArg(args[n], XmNleftWidget, prev_button);
    n++;
    XtSetArg(args[n], XmNrightAttachment, XmATTACH_WIDGET);
    n++;
    XtSetArg(args[n], XmNrightWidget, succ_button);
    n++;

    date = XtCreateManagedWidget("date", xmLabelWidgetClass, controls, args, n);
    XmStringFree(xms);

    XtAppAddActions(XtWidgetToApplicationContext(date), appActions, 1);

    XtAugmentTranslations(date, XtParseTranslationTable(defTranslations));

    size.request_mode = CWHeight;
    XtQueryGeometry(controls, NULL, &size);

    n = 0;
    XtSetArg(args[n], XmNpaneMinimum, size.height);
    n++;
    XtSetArg(args[n], XmNpaneMaximum, size.height);
    n++;
    XtSetValues(controls, args, n);

    /** end of control panel code **/

    /** create calendar form **/

    n = 0;
    calendar = CreateCalendarFrame(frame, args, n, numberOfDays, firstDay);

    n = 0;
    bcontrols = XtCreateManagedWidget("bcontrols", xmFormWidgetClass,
				      frame, args, n);

    n = 0;
    callbacks[0].callback = quit;
    XtSetArg(args[n], XmNactivateCallback, callbacks);
    n++;

    quit_button = XtCreateManagedWidget("quitButton", xmPushButtonWidgetClass,
					bcontrols, args, n);

    XtAugmentTranslations(quit_button,
			  XtParseTranslationTable(defTranslations));

    n = 0;
    callbacks[0].callback = help;
    XtSetArg(args[n], XmNactivateCallback, callbacks);
    n++;
    XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM);
    n++;

    help_button = XtCreateManagedWidget("helpButton",
					xmPushButtonWidgetClass,
					bcontrols, args, n);

    Fill_In_Date(title);
    xms = XMS(title);

    n = 0;
    XtSetArg(args[n], XmNlabelString, xms);
    n++;
    XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET);
    n++;
    XtSetArg(args[n], XmNleftWidget, quit_button);
    n++;
    XtSetArg(args[n], XmNrightAttachment, XmATTACH_WIDGET);
    n++;
    XtSetArg(args[n], XmNrightWidget, help_button);
    n++;
    XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM);
    n++;
    XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM);
    n++;

    cdate = XtCreateManagedWidget("cdate", xmLabelWidgetClass,
				  bcontrols, args, n);
    XmStringFree(xms);

    size.request_mode = CWHeight;
    XtQueryGeometry(bcontrols, NULL, &size);

    n = 0;
    XtSetArg(args[n], XmNpaneMinimum, size.height);
    n++;
    XtSetArg(args[n], XmNpaneMaximum, size.height);
    n++;
    XtSetValues(bcontrols, args, n);

    /** end of top level form **/

    XtInstallAllAccelerators(frame, frame);

    HideSashes(frame);
    XtManageChild(frame);
    return (frame);
}
#endif

/*
 * getmonthday() - Returns month or day string from the given list.
 */

char           *
getmonthday(num, startptr)
    int             num;	/* Month or day ordinal */
    char           *startptr;	/* String to pick it from */
{
    static char     returnval[128];
    char           *start, *end;
    int             i;

    start = startptr;
    strcpy(returnval, "");
    for (i = 0; i < num; i++) {
	end = strchr(start, '/');
	if (end == (char *) NULL)
	    return returnval;
	start = end + 1;
    }
    end = strchr(start, '/');
    if (end == (char *) NULL)
	strcpy(returnval, start);
    else {
	char           *op;

	op = returnval;
	while (start < end)
	    *(op++) = *(start++);
	*op = '\0';
    }
    return returnval;
}

/*
 * Fill_In_Date() - Formats current date for display.
 */

void
Fill_In_Date(sb)
    char           *sb;
{
    int             i, len, prev_len;
    char           *startp, *p;
    struct tm       tm;
    time_t          time_value;



    if (appResources.date == NULL) {
	strcpy(sb, "");
	return;
    }
    (void) time(&time_value);
    tm = *localtime(&time_value);

    startp = sb;
    for (p = appResources.date; p && *p; p++) {
	if (*p != '%')
	    *sb++ = *p;
	else
	    switch (*++p) {
	    case 'M':
		sb += strlen(strcpy(sb,
				    getmonthday(tm.tm_mon,
					(char *) appResources.monthnames)));
		break;
	    case 'm':
		sb += strlen(strcpy(sb,
				    getmonthday(tm.tm_mon,
					  (char *) appResources.monthnms)));
		break;
	    case 'W':
		sb += strlen(strcpy(sb,
				    getmonthday(tm.tm_wday,
					  (char *) appResources.daynames)));
		break;
	    case 'w':
		sb += strlen(strcpy(sb,
				    getmonthday(tm.tm_wday,
					    (char *) appResources.daynms)));
		break;
	    case 'd':
		if (tm.tm_mday >= 10)
		    *sb++ = (tm.tm_mday / 10 + '0');
		*sb++ = tm.tm_mday % 10 + '0';
		break;
	    case 'Y':
		*sb++ = '1', *sb++ = '9';
		/* fall thru */
	    case 'y':
		*sb++ = tm.tm_year / 10 + '0';
		*sb++ = tm.tm_year % 10 + '0';
		break;
	    case '%':
		*sb++ = *p;
	otherwise:;		/* nothing */
	    }
    }
    *sb = '\0';
    sb = startp;
}

/*
 * LoadHolidays() - Loads the holiday file.
 */

void
LoadHolidays()
{
    char           *buf;
    int             n;
    int             len;
    char           *next;
    char           *tab;
    char           *start;
    struct holiday *holi;

    n = 2048;
    buf = XtMalloc(sizeof(char) * n);

    holilist = CreateList(1, sizeof(Holiday));

    len = read_file(appResources.holidayFile, buf, n);
    if (len == -1)
	return;
    else if (len > n) {
	buf = XtRealloc(buf, len + 1);
	n = len + 1;
	len = read_file(appResources.holidayFile, buf, n);
	if (len == -1)
	    return;
    }
    start = buf;

    while (len > 0) {
	next = strchr(buf, '\n');
	tab = strchr(buf, '\t');
	if (tab == NULL) {
	    XtFree(start);
	    return;
	}
	if (next != NULL) {
	    next[0] = '\0';
	}
	holi = (struct holiday *) XtMalloc(sizeof(struct holiday));
	holi->text = XtMalloc(sizeof(char) * strlen(tab));
	sscanf(buf, "%d/%d/%d", &holi->month, &holi->day, &holi->year);
	strcpy(holi->text, tab + 1);
	(void) PushOnList(holilist, &holi);
	if (next == NULL) {
	    XtFree(start);
	    return;
	}
	buf = next + 1;
	len = strlen(buf);
    }
    XtFree(start);
}

