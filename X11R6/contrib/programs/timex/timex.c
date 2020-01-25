/* TIMEX - the Time Usage Meter */
/* 
     Copyright (c) 1994 Harald T. Alvestrand

     The X Consortium, and any party obtaining a copy of these files from
     the X Consortium, directly or indirectly, is granted, free of charge, a
     full and unrestricted irrevocable, world-wide, paid up, royalty-free,
     nonexclusive right and license to deal in this software and
     documentation files (the "Software"), including without limitation the
     rights to use, copy, modify, merge, publish, distribute, sublicense,
     and/or sell copies of the Software, and to permit persons who receive
     copies from any such party to do so.  This license includes without
     limitation a license to do the foregoing actions under any patents of
     the party supplying this software to the X Consortium.

*/
#ifdef __STDC__
#define USE(var) \
            static void * use_##var = (&use_##var, (void *) &var)
#endif
/* Keeping the RCS keyword around */
static char     rcsid[] = "$Id: timex.c,v 1.12 1993/08/17 21:08:16 hta Exp hta $";
static char     rcsdate[] = "$Date: 1993/08/17 21:08:16 $";
#ifdef __STDC__
USE(rcsid);
#endif

/* ------------ Shared definitions --------------- */
#include "timex.h"
#include "patchlevel.h"
/* The icon bitmap */
#include "timex.xbm"


/* ------------- APPLICATION RESOURCES AND OPTIONS -------------- */

String          fallbackResources[] = {
#ifdef notdef
   "Timex.appDefaultsLoaded: false",
   "Timex.errorMessage: Timex application defaults not found",
   NULL
#else
#include "Timex.ad.h"
#endif 
};

struct _resources {
   Boolean         project_icon_name;	/* put current project in icon name */
   char           *icon_prefix;	/* fixed prefix in icon name */
   Boolean         start_paused;/* start in paused mode */
   char           *timex_directory;	/* fixed prefix in icon name */
   Boolean         appDefaultsLoaded;
   char           *errorMessage;
}               app_resources;

#define Offset(field) XtOffsetOf(struct _resources, field)

static XtResource resources[] = {
   {"projectIconName", "ProjectIconName", XtRBoolean, sizeof(Boolean),
   Offset(project_icon_name), XtRImmediate, False},
   {"iconPrefix", "IconPrefix", XtRString, sizeof(char *),
   Offset(icon_prefix), XtRString, "Timex"},
   {"startPaused", "StartPaused", XtRBoolean, sizeof(Boolean),
   Offset(start_paused), XtRImmediate, False},
   {"timexDirectory", "TimexDirectory", XtRString, sizeof(char *),
   Offset(timex_directory), XtRString, NULL},
   {"appDefaultsLoaded", "AppDefaultsLoaded", XtRBoolean, sizeof(Boolean),
   Offset(appDefaultsLoaded), XtRImmediate, False},
   {"errorMessage", "ErrorMessage", XtRString, sizeof(char *),
   Offset(errorMessage), XtRString, "Timex app-defaults file not found"},
};

static XrmOptionDescRec options[] = {
   {"+pi", "*projectIconName", XrmoptionNoArg, "True"},
   {"-pi", "*projectIconName", XrmoptionNoArg, "False"},
   {"-ip", "*iconPrefix", XrmoptionSepArg, NULL},
   {"-sp", "*startPaused", XrmoptionNoArg, "True"},
   {"-dir", "*timexDirectory", XrmoptionSepArg, NULL},
};

void Usage()
{
   fprintf(stderr, "\
usage: timex [-pi]                  ... put project in icon name\n\
             [-sp]                  ... start in paused mode\n\
             [-dir directory]       ... directory name for log files\n\
             [-ip icon_prefix] ... icon name prefix\n");
   exit(1);
}

/* ------------ GLOBAL VARIABLES ----------------- */

String         *timeList = NULL;
String         *nameList = NULL;
int            *timeUsed = NULL;
int             totaltime;
int             adjustedtime;
int             incPlus = 10, incMinus = -10;
int             lockfd;		/* Locking file descriptor */
char            lockfilename[80];	/* Lockfile name */
char            title[80];


XtAppContext    appContext;

Widget          toplevel;
Widget          topForm;
Widget          list_widget;
Widget          viewport;
Widget          timelist_widget;
Widget          commandBox;
Widget          thisCommand;
Widget          addproject;
Widget          commandBox;
Widget          totalDisplay;

/* Filenames should be legal, and all legal ones should be allowed */
#define PREFIXLEN (MAXPATHLEN-20)
char            timexdirectory[MAXPATHLEN];	/* file name prefix to use */
int             lastDate, lastTime;
char           *timestring;	/* Non-NULL when used as an EDITOR */
static Pixmap  iconP;


void            timeshavechanged();	/* Declared later in this file */
void            DeleteProject();
int             testfortime();
int             TestForNoOldProject();
void            printtime();

/* ------------- UTILITY FUNCTIONS -------------- */
/*
 *  Set up the timex file directory based on timexdirectory resource,
 *  a leading '~' is expanded to $HOME
 *  Default value is $HOME/.timex
 */

void fix_timex_directory()
{
   struct stat     buf;
   String          s;

   timexdirectory[0] = '\0';
   if ((s = getenv("TIMEXDIR"))) {
      if (s[0] == '~')
	 sprintf(timexdirectory, "%s%s", getenv("HOME"), &s[1]);
      else
	 sprintf(timexdirectory, "%s", s);
   } else if (app_resources.timex_directory) {
      if (app_resources.timex_directory[0] == '~')
	 sprintf(timexdirectory, "%s%s",
		 getenv("HOME"), &app_resources.timex_directory[1]);
      else
	 strcpy(timexdirectory, app_resources.timex_directory);
   } else
      sprintf(timexdirectory, "%s/.timex", getenv("HOME"));

   stat(timexdirectory, &buf);
   if (!S_ISDIR(buf.st_mode)) {
      fprintf(stderr, "timex: bad directory: %s\n", timexdirectory);
      exit(1);
   }
   return;
}

/* Handling of dayfiles */

char           *dayfilename(dayoffset)
     int dayoffset;
{
   static char     filename[PREFIXLEN + 20];
   struct tm       today;

   if (timestring) {
      (void) sprintf(filename, "%.*s/%s", PREFIXLEN,
		     timexdirectory, timestring);
   } else {
      time_t          now = time((time_t *) 0);

      now += dayoffset * 24 * 60 * SECONDSPERMINUTE;	/* Add in adjusted day */
      today = *(localtime(&now));

      (void) sprintf(filename, "%.*s/%04d-%02d-%02d",
		     PREFIXLEN, timexdirectory,
		     today.tm_year + 1900, today.tm_mon + 1, today.tm_mday);
   }
   return filename;
}

/* Save current listing to a dayfile */
void            save(dayoffset, reason)
     int dayoffset;
     char *reason;
{
   char           *filename;
   int             scan;
   FILE           *outfile;
   time_t          now;

   filename = dayfilename(dayoffset);
   if ((outfile = fopen(filename, "w")) == NULL) {
      char            ermsg[100];

      (void) sprintf(ermsg, "Unable to open dayfile %s", filename);
      perror(ermsg);
   }
   now = time((time_t *) 0);
   fprintf(outfile, "# TIMEX log saved at %s",
	   ctime(&now));
   fprintf(outfile, "# %s\n", reason);
   for (scan = 0; nameList[scan]; ++scan) {
      if (timeUsed[scan]) {
	 fprintf(outfile, "%2d:%02d %s\n",
		 timeUsed[scan] / 60, timeUsed[scan] % 60, nameList[scan]);
      }
   }
   fclose(outfile);
}


void            resumtimes()
{
   int             scan;
   int             summedtime;

   summedtime = 0;
   for (scan = 0; nameList[scan]; ++scan)
      if (timeUsed[scan]) {
	 printtime(scan);
	 summedtime += timeUsed[scan];
      }
   adjustedtime = summedtime - totaltime;
}


int             fetchdayfile(filename)
     char *filename;
{
   char            project[80];
   char            buffer[80];
   int             hours, minutes;
   int             scan, count;
   FILE           *infile;

/* Add times from a previous incarnation of Timex for today */
   if ((infile = fopen(filename, "r")) == NULL) {
      return 0;			/* No file found, assume fresh day */
   }
   /* Skip the header line */
   (void) fgets(buffer, sizeof(buffer), infile);
   while (fgets(buffer, sizeof(buffer), infile)) {
      if (buffer[0] == '#')
	 continue;		/* Skip comment lines */
      count = sscanf(buffer, "%d:%d %[^\n]", &hours, &minutes, project);
      if (count < 3) {
	 fprintf(stderr, "Bogus project line in dayfile: %s\n", buffer);
	 continue;
      }
      for (scan = 0; nameList[scan]; ++scan) {
	 if (strcmp(nameList[scan], project) == 0) {
	    timeUsed[scan] = hours * 60 + minutes;
	    break;
	 }
      }
      if (!nameList[scan]) {
         addDeadProject(project, hours * 60 + minutes);
/*	 fprintf(stderr, "Bogus project: %s\n", project); */
      }
   }
   fclose(infile);
   /* at this time (no other), reset totaltime */
   resumtimes();
   totaltime = adjustedtime;
   adjustedtime = 0;
   timeshavechanged();
   return 1;
}

int             addprevtimes()
{
   char           *filename;

   filename = dayfilename(0);
   return fetchdayfile(filename);
}

/* Ensure that only 1 copy of timex runs at any one time */
int             lockTimex()
{
   FILE           *lockfile;
   char            hostname[80];

   (void) sprintf(lockfilename, "%s/LOCK", timexdirectory);
   lockfd = open(lockfilename, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
   if (lockfd < 0) {
      if (errno == EEXIST) {
	 int             pid, count;

	 lockfile = fopen(lockfilename, "r");
	 if (lockfile == NULL) {
	    perror("Unable to open lock file for read");
	    return -1;
	 }
	 count = fscanf(lockfile, "%s %d", hostname, &pid);
	 if (count == 2) {
	    fprintf(stderr, "TIMEX may be running on machine %s as pid %d\n",
		    hostname, pid);
	 } else {
	    fprintf(stderr, "TIMEX may be running - lock file unintelligible\n");
	 }
	 fprintf(stderr, "Remove %s if you are sure that this is not so\n",
		 lockfilename);
      } else {
	 perror("Failed to create lock file");
      }
      return -1;

   }
   lockfile = fdopen(lockfd, "w");
   gethostname(hostname, sizeof(hostname));
   fprintf(lockfile, "%s %d\n", hostname, getpid());
   fclose(lockfile);
   close(lockfd);
   return 0;
}

int             unlockTimex()
{
   if (unlink(lockfilename) < 0) {
      perror("Failed to remove lock file");
      return -1;
   }
   return 0;
}

/* Clear all the time-used fields */
void            clear()
{
   int             scan;

   for (scan = 0; nameList[scan]; scan++) {
      timeUsed[scan] = 0;
      timeList[scan][0] = '\0';
   }
   /* Make the right width for the time list... */
   for (scan = 0; scan < TIMELEN; scan++)
      timeList[0][scan] = ' ';
   timeList[0][TIMELEN] = 0;
   totaltime = 0;
   adjustedtime = 0;
}


/* ------------- CALLBACK FUNCTIONS ------------ */

/* Callback function: Desctruction completed */
/*ARGSUSED*/
void            destructioncompleted(widget, client_data, call_data)
     Widget widget;
     XtPointer client_data;
     XtPointer call_data;
{
   XtDestroyApplicationContext(appContext);
   exit(0);
}

/* Fatal error handler - try to save and die */
void            gaspanddie(str)
     String str;
{
   char            errormsg[100];

   fprintf(stderr, "TIMEX saving and dying, error = %s\n", str);
   (void) sprintf(errormsg, "Exit on %.90s", str);
   save(0, errormsg);
   unlockTimex();
   exit(1);
}

/*ARGSUSED*/
int             IOErrorHandler(display)
     Display *display;
{
   gaspanddie("fatal X I/O error");
   return 0;			/* make Lint happy */
}

void            signalhandler(sig)
     int sig;	/* Yes, I know: it has more
					 * parameters */
                       
{
   if (sig == SIGHUP)
      gaspanddie("HANGUP signal");
   if (sig == SIGINT)
      gaspanddie("INT signal");
   if (sig == SIGTERM)
      gaspanddie("TERM signal");
   if (sig == SIGQUIT)
      gaspanddie("QUIT signal");
   if (sig == SIGPIPE)
      gaspanddie("PIPE signal");
   gaspanddie("Unknown signal");
}

/*ARGSUSED*/
void            savewhendying(widget, client_data, call_data)
     Widget widget;
     XtPointer client_data;
     XtPointer call_data;
{
   save(0, "Normal X termination");
   unlockTimex();
}

/* ----------- DISPLAY HANDLING ------------- */

/*
 *  Update the icon title to show current project name and time
 */

#define MAX_ICON_NAME_LEN 32

void            set_icon_name(cur)
     int cur;
{
   char            icon_name[256], *names[1];
   XTextProperty   iconName;

   if (!app_resources.project_icon_name)
      return;
   if (cur == -1)
      sprintf(icon_name, "%s PAUSE", app_resources.icon_prefix);
   else {
      sprintf(icon_name,
	      "%s %2d:%02d %.*s",
	      app_resources.icon_prefix,
	      timeUsed[cur] / 60, timeUsed[cur] % 60,
	      MAX_ICON_NAME_LEN - 12, nameList[cur]);
   }

   names[0] = icon_name;
   XStringListToTextProperty(names, 1, &iconName);
   XSetWMIconName(XtDisplay(toplevel), XtWindow(toplevel), &iconName);
   XFree(iconName.value);
}

void            printtime(cur)
     int cur;
{
   if (cur >= 0 && timeList[cur]) {
      (void) sprintf(timeList[cur], "%2d:%.2d",
		     timeUsed[cur] / 60, timeUsed[cur] % 60);
   }
}

void            redisplayTotal()
{
   Arg             arg[1];
   static char     buf[20];

   (void) sprintf(buf, "%2d:%.2d%+d",
		  totaltime / 60, totaltime % 60, adjustedtime);
   XtSetArg(arg[0], XtNlabel, buf);
   XtSetValues(totalDisplay, arg, 1);
}

void            redisplayTime(cur)
     int cur;
{
   if (cur >= 0) {
      printtime(cur);
      /* Redisplay time, keep highlight */
      XawListHighlight(timelist_widget, cur);
      set_icon_name(cur);
      redisplayTotal();
   }
}

int             currentProject()
{
   XawListReturnStruct *cur_p;
   int             cur;

   cur_p = XawListShowCurrent(list_widget);
   if (cur_p && cur_p->list_index >= 0) {
      cur = cur_p->list_index;
   } else
      cur = -1;
   if (cur_p)
      XFree(cur_p);
   return cur;
}


void            changeProject(adjustIncrement)
     int adjustIncrement;
{
   int             index;
   int             oldval;

   index = currentProject();
   if (index >= 0) {
      oldval = timeUsed[index];
      timeUsed[index] += adjustIncrement;
      if (timeUsed[index] < 0)
	 timeUsed[index] = 0;
      adjustedtime += timeUsed[index] - oldval;
   }
   redisplayTime(index);
}

/* ----------- REGISTERED ACTIONS ----------- */

/* Quit the show */
/*ARGSUSED*/
static void     Quit(widget, event, params, parmcount)
     Widget widget;
     XEvent *event;
     String *params;
     Cardinal *parmcount;
{
   XtDestroyWidget(toplevel);
}

/* Callback function: Save */
/*ARGSUSED*/
static void     Save(widget, event, params, parmcount)
     Widget widget;
     XEvent *event;
     String *params;
     Cardinal *parmcount;
{
   save(0, "Save button hit");
}

/* Load file from disk */
/*ARGSUSED*/
static void     Load(widget, event, params, parmcount)
     Widget widget;
     XEvent *event;
     String *params;
     Cardinal *parmcount;
{
   addprevtimes();
}

/*ARGSUSED*/
static void     Increment(widget, event, params, parmcount)
     Widget widget;
     XEvent *event;
     String *params;
     Cardinal *parmcount;
{
   if (*parmcount > 0)
      changeProject(atoi(params[0]));
   else
      changeProject(incPlus);
}


/*ARGSUSED*/
static void     Decrement(widget, event, params, parmcount)
     Widget widget;
     XEvent *event;
     String *params;
     Cardinal *parmcount;
{
   if (*parmcount > 0)
      changeProject(-atoi(params[0]));
   else
      changeProject(incMinus);
}

/*ARGSUSED*/
static void     Pause(widget, event, params, parmcount)
     Widget widget;
     XEvent *event;
     String *params;
     Cardinal *parmcount;
{
   XawListUnhighlight(list_widget);
   XawListUnhighlight(timelist_widget);
   set_icon_name(-1);
   set_icon_name(-1);
}

static void     RegisterActions()
{
   /* Must be static to allow initialization */
   static XtActionsRec actions[] = {
      {"quit", Quit},
      {"save", Save},
      {"load", Load},
      {"increment", Increment},
      {"decrement", Decrement},
      {"pause", Pause},
   };

   XtAppAddActions(appContext, actions, XtNumber(actions));
}


/* Note the time, and maybe do specific actions.
   Updates lastDate and lastTime.
   Returns number of minutes since last called. */
int             notethetime(maysave)
     int maysave;
{
   time_t          now = time((time_t *) 0);
   struct tm       today;
   int             rettime;

   today = *(localtime(&now));
   rettime = (now / SECONDSPERMINUTE) - lastTime;
   if (maysave && today.tm_mday != lastDate) {
      save(-1, "End of day");	/* Save YESTERDAY's times */
      clear();
      timeshavechanged();
      /* This is to tell the statistics package that it is in use, not just
         tried once and then discarded */
#ifndef DO_NOT_TELL_ABOUT_ME
      sayiamhere(title);
#endif
   }
   lastDate = today.tm_mday;
   lastTime = now / SECONDSPERMINUTE;
   return rettime;		/* Difference in minutes between now and prev
				 * time */
}

/*ARGSUSED*/
void            clockTick(clientData, intervalID)
     XtPointer clientData;
     XtIntervalId *intervalID;
{
   int             cur;

   cur = currentProject();
   if (cur >= 0) {
      int             increment;

      increment = notethetime(True);
      timeUsed[cur] += increment;
      totaltime += increment;
      redisplayTime(cur);
   } else {
      /* No time defined - note the time and throw away result */
      (void) notethetime(True);
   }
   XtAppAddTimeOut(appContext,
		   TICKPERIOD,
		   clockTick, clientData);
}

/*ARGSUSED*/
void            saveOnclockTick(clientData, intervalID)
     XtPointer clientData;
     XtIntervalId *intervalID;
{
   save(0, "Periodic save");
   XtAppAddTimeOut(appContext,
		   SAVEPERIOD,
		   saveOnclockTick, clientData);
}

/* Times have been changed in the timelist. Update and set. */
void            timeshavechanged()
{
   resumtimes();
   XawListChange(timelist_widget, timeList, 0, 0, False);
   redisplayTime(currentProject());
}


/* Called on the Notify action of both list widgets:
   Ensure that both lists are equally highlighted */
/*ARGSUSED*/
void            notifycallback(widget, client_data, call_data)
     Widget widget;
     XtPointer client_data;
     XtPointer call_data;
{
   XawListReturnStruct *cur;

   cur = XawListShowCurrent(widget);
   XawListHighlight(list_widget, cur->list_index);
   XawListHighlight(timelist_widget, cur->list_index);
   set_icon_name(cur->list_index);
   XFree(cur);
}

/* A short VMS hack......be nice to other people */
void            purge_working_set()
{
#ifdef VMS
   long            status, npages, nfaults;
   long            inadr[2];

   inadr[0] = 0;
   inadr[1] = 0x7FFFFFFF;
   status = sys$purgws(&inadr);
#endif
}

void            complainAndDie(message)
     char *message;
{
   fprintf(stderr, "%s\n", message);
   exit(1);
}

void setIcon()
{
   Display *dpy;
   Arg wargs[1];

   dpy = XtDisplay(topForm);
   iconP = XCreateBitmapFromData(dpy, DefaultRootWindow(dpy),
				 timex_bits, timex_width, timex_height);
   XtSetArg(wargs[0], XtNiconPixmap, iconP);
   XtSetValues(toplevel, wargs, 1);
}


int             main(argc, argv)
     Cardinal argc;
     char **argv;
{
   Arg             wargs[10];
   int             n;
   Widget          thisCommand, listForm;

   toplevel = XtAppInitialize(&appContext, "Timex", options, XtNumber(options),
			      &argc, argv, fallbackResources,
			      NULL, (Cardinal) 0);
   XtGetApplicationResources(toplevel, (XtPointer) & app_resources,
			     resources, XtNumber(resources),
			     NULL, (Cardinal) 0);
   if (!app_resources.appDefaultsLoaded)
      complainAndDie(app_resources.errorMessage);
   if (argc > 2)
      Usage();

   if (argc == 2) {
      timestring = argv[1];
   } else {
      timestring = NULL;
   }

   fix_timex_directory();
#ifdef DEBUG
   (void) sprintf(title, "Timex %d.%d %s",
		  VERSION, PATCHLEVEL, rcsdate);
#else
   (void) sprintf(title, "Timex %d.%d %s", VERSION, PATCHLEVEL, COMMENT);
#endif
   n = 0;
   XtSetArg(wargs[n], XtNtitle, title);
   ++n;
   XtSetValues(toplevel, wargs, n);

   if (argc > 1) {
      timestring = argv[1];
   } else {
      timestring = NULL;
   }
   if (makethelist() < 0) {
      exit(errno);
   }
   if (timestring == NULL) {
      if (lockTimex() < 0) {
	 exit(errno);
      }
      notethetime(0);
   }
   /* Create the upper Form widget */
   n = 0;
   topForm = XtCreateManagedWidget("form", formWidgetClass, toplevel, wargs, n);
   XtAddCallback(topForm, XtNdestroyCallback, destructioncompleted, NULL);
   /* Create command box */
   n = 0;
   commandBox = XtCreateManagedWidget("command", boxWidgetClass,
				      topForm, wargs, n);
   /* Create commands */
   addButton(commandBox, "quit", NULL, NULL);
   addButton(commandBox, "plus", NULL, NULL);
   addButton(commandBox, "minus", NULL, NULL);

   n = 0;
   initprojectlisteditor();
   /* A total-time display to the right of the commandbox */
   n = 0;
   XtSetArg(wargs[n], XtNfromHoriz, commandBox);
   ++n;
   XtSetArg(wargs[n], XtNlabel, "--:--+---");
   ++n;
   /* just to get size right */
   totalDisplay = XtCreateManagedWidget("total", labelWidgetClass,
					topForm, wargs, n);

   /* Create the save and load buttons only when editing */
   if (timestring) {
      addButton(commandBox, "save", NULL, NULL);
      addButton(commandBox, "load", NULL, NULL);
      /* Create a string in the right-hand corner */
      n = 0;
      XtSetArg(wargs[n], XtNlabel, timestring);
      ++n;
      XtSetArg(wargs[n], XtNfromVert, totalDisplay);
      ++n;
      XtSetArg(wargs[n], XtNfromHoriz, commandBox);
      ++n;
      thisCommand = XtCreateManagedWidget("date", labelWidgetClass,
					  topForm, wargs, n);
   } else {
      /* and the pause button only when NOT editing */
      addButton(commandBox, "pause", NULL, NULL);
   }
   /* Create viewport for list */
   n = 0;
   XtSetArg(wargs[n], XtNfromVert, commandBox);
   ++n;
   viewport = XtCreateManagedWidget("viewport", viewportWidgetClass,
				    topForm, wargs, n);
   /* Create two lists inside a form */

   n = 0;
   listForm = XtCreateManagedWidget("form", formWidgetClass, viewport, wargs, n);
   /* The list of times */
   n = 0;
   XtSetArg(wargs[n], XtNlist, timeList);
   ++n;
   timelist_widget = XtCreateManagedWidget("times", listWidgetClass,
					   listForm, wargs, n);
   XtAddCallback(timelist_widget, XtNcallback, notifycallback, NULL);
   /* The list of names */
   n = 0;
   XtSetArg(wargs[n], XtNlist, nameList);
   ++n;
   XtSetArg(wargs[n], XtNfromHoriz, timelist_widget);
   ++n;
   list_widget = XtCreateManagedWidget("names", listWidgetClass,
				       listForm, wargs, n);
   XtAddCallback(list_widget, XtNcallback, notifycallback, NULL);
   setIcon();
   /* The following is only done when the Timex is NOT an editor */
   if (timestring == NULL) {
      XtAddCallback(list_widget, XtNdestroyCallback, savewhendying, NULL);
      /* Start clock ticking */
      XtAppAddTimeOut(appContext, TICKPERIOD, clockTick, (XtPointer) NULL);
      XtAppAddTimeOut(appContext, SAVEPERIOD, saveOnclockTick, NULL);
      /* Set the fatal error handler, so that we can save on being killed */
      XtAppSetErrorHandler(appContext, gaspanddie);
      XSetIOErrorHandler(IOErrorHandler);
      /* Set signal handlers */
      signal(SIGINT, signalhandler);
      signal(SIGHUP, signalhandler);
      signal(SIGTERM, signalhandler);
      signal(SIGQUIT, signalhandler);
      signal(SIGPIPE, signalhandler);
   }
   /* Install action procedures */
   RegisterActions();
   /* Read the previous times for today, if any */
   addprevtimes();

   /* Select the first project by default */
   if (!app_resources.start_paused) {
      XawListHighlight(list_widget, 0);
      XawListHighlight(timelist_widget, 0);
   }
   XtRealizeWidget(toplevel);
   set_icon_name(0);
   purge_working_set();
   /* Tell the old programmer that I have started */
#ifndef DO_NOT_TELL_ABOUT_ME
   sayiamhere(title);
#endif
   redisplayTime(0);		/* Set total-display to current. MUST be
				 * after realize */
   XtAppMainLoop(appContext);
   return 0;			/* will never be reached, but Lint does not
				 * know that */
}
