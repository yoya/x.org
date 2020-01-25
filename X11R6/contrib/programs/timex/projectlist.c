/* TIMEX - the Time Usage Meter */
/* Projectlist.c */

/*   Handling of the project file: Reading, writing and editing it */
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

/* ------------ SHARED VARIABLES ----------------- */
#include "timex.h"
/* ------------ GLOBAL VARIABLES (to this file) ----------------- */

static Widget   addproject;
static Widget   addshell;
static Widget   addDialog;
static Widget   editproject;
static Widget   editshell;
static Widget   editDialog;
static Widget   messageBox;
static Widget   messageshell;
static Widget   scrollbar;

/* Global variables for edit box */
int             edited_index;
int             edited_time;
String          edited_project;
char            edited_timestring[10];

void            timeshavechanged();	/* Declared later in this file */
void            DeleteProject();
int             testfortime();
int             TestForNoOldProject();
void            MessageBox();

/* Information about the current project list */
int             noOfProjects = 0;
int             lastRealProject = 0;

void 
makeemptyprojectlist()
{
   char            filename[80];
   FILE           *inputfile;

   (void) sprintf(filename, "%.60s/projectlist", timexdirectory);

   inputfile = fopen(filename, "w");
   fprintf(inputfile, "empty\n");
   fclose(inputfile);
}


static void 
addLine(toAll, scan, tobuf)
     String **toAll;
     int *scan;
     String tobuf;
{
   (*toAll)[*scan] = (String) malloc(strlen(tobuf) + 1);
   strcpy((*toAll)[*scan], tobuf);
   ++*scan;
   *toAll = (String *) realloc(*toAll, (*scan + 1) * sizeof(String));
   (*toAll)[*scan] = NULL;
}

/* Add an extra line to an existing nameList
   Used by handling of dead projects. */
static void 
addExtraLine(buf)
     String buf;
{
   addLine(&nameList, &noOfProjects, buf);
   timeUsed = (int *) realloc(timeUsed, noOfProjects * sizeof(int));
   timeUsed[noOfProjects - 1] = 0;
   timeList = (String *) realloc(timeList,
				 (noOfProjects + 1) * sizeof(String));
   timeList[noOfProjects - 1] = (String) malloc(TIMELEN + 1);
   timeList[noOfProjects - 1][0] = 0;
   timeList[noOfProjects] = NULL;
}

void 
ListIsModified()
{
   XawListChange(list_widget, nameList, 0, 0, True);
   XawListChange(timelist_widget, timeList, 0, 0, True);
   timeshavechanged();
}

void 
addDeadProject(buf, mins)
     String buf;
     int mins;
{
   if (noOfProjects == lastRealProject) {
      /* Add visual marker */
      addExtraLine("============================");
      timeUsed[noOfProjects - 1] = 0;
   }
   addExtraLine(buf);
   timeUsed[noOfProjects - 1] = mins;
   ListIsModified();
}

/* Read a projectlist from a file */
String         *
makealist()
{
   char            buffer[80];
   String          tobuf;
   String         *toAll;
   int             scan;
   char            filename[80];
   FILE           *inputfile;


   (void) sprintf(filename, "%.60s/projectlist", timexdirectory);
   if ((inputfile = fopen(filename, "r")) == NULL) {
      makeemptyprojectlist();
      inputfile = fopen(filename, "r");
      if (inputfile == NULL) {
	 char            ermsg[100];

	 (void) sprintf(ermsg,
			"Unable to open or create projectfile %s", filename);
	 perror(ermsg);
	 return NULL;
      }
   }
   scan = 0;
   toAll = (String *) malloc(sizeof(String));
   while ((tobuf = fgets(buffer, 80, inputfile))) {
      buffer[strlen(buffer) - 1] = 0;	/* Cut the newline */
      if (strlen(buffer) > 0) {	/* IGNORE all blank lines */
	 addLine(&toAll, &scan, tobuf);
      }
   }
   if (scan == 0) {		/* Error: Empty project file */
      makeemptyprojectlist();	/* make an empty, 1-project list */
      return makealist();	/* and return with it */
   }
   toAll[scan] = NULL;
   fclose(inputfile);
   lastRealProject = noOfProjects = scan;
   return toAll;
}

/* Put the project list into the global variables */
int 
makethelist()
{
   int             listlen, scan;

   nameList = makealist();
   if (nameList == NULL) {
      return -1;
   }
   for (listlen = 0; nameList[listlen]; ++listlen);
   timeList = (String *) malloc((listlen + 1) * sizeof(String));
   for (scan = 0; scan < listlen; scan++) {
      timeList[scan] = (String) malloc(TIMELEN + 1);
      timeList[scan][0] = 0;
   }
   timeList[scan] = NULL;
   timeUsed = (int *) malloc(listlen * sizeof(int));
   clear();
   return 0;
}

/* Add a new project to the projectlist after the currently highlighted
   project, or first if there is none */
void 
AddNewProject(strchr, newproject)
     int strchr;
     String newproject;
{
   char            filename[80];
   int             j;
   FILE           *outputfile;

   /* NOTE: No check whatsoever for more than 1 project of same name! */
   (void) sprintf(filename, "%.60s/projectlist", timexdirectory);
   outputfile = fopen(filename, "w");
   if (outputfile == NULL) {
      MessageBox(toplevel, "Could not open project file when adding");
      return;
   }
   if (index == -1)
      fprintf(outputfile, "%s\n", newproject);
   for (j = 0; j < lastRealProject; j++) {
      fprintf(outputfile, "%s\n", nameList[j]);
      if (j == index)
	 fprintf(outputfile, "%s\n", newproject);
   }
   fclose(outputfile);
}



/*
 * function to delete a project from the projectlistfile
   Simply saves the current content of the projectnames array
 */

void 
DeleteProject(strchr)
     int strchr;
{
   int             j;
   char            filename[80];
   FILE           *outputfile;
   int             counter;

   (void) sprintf(filename, "%.60s/projectlist", timexdirectory);

   /*
    * MISSING: Code to check that projectlist is not changed before writing
    * it back!!!!!!!!!!!!
    */
   outputfile = fopen(filename, "w");
   counter = 0;
   for (j = 0; j < lastRealProject; j++) {

      if (j != index) {
	 fprintf(outputfile, "%s\n", nameList[j]);
	 ++counter;
      }
   }
   if (counter == 0)
      fprintf(outputfile, "empty\n");

   fclose(outputfile);
}



/* =========== Above this line is list handling. Below is X ============= */

/* Useful functions */
Widget 
addButton(father, name, callback, c_data)
     Widget father;
     String name;
     XtCallbackProc callback;
     XtPointer c_data;
{
   Widget          w;

   w = XtCreateManagedWidget(name,
			     commandWidgetClass, father, NULL, 0);
   if (callback)
      XtAddCallback(w, XtNcallback, callback, c_data);
   return w;
}

void 
Popup(w, topLevel)
     Widget w;
     Widget topLevel;
{
   Position        x;
   Position        y;
   Dimension       width;
   Dimension       height;
   Arg             arg[2];
   int             i;

   /*
    * get the coordinates of the toplevel widget.
    */

   i = 0;
   XtSetArg(arg[i], XtNwidth, &width);
   ++i;
   XtSetArg(arg[i], XtNheight, &height);
   ++i;
   XtGetValues(topLevel, arg, i);

   XtTranslateCoords(topLevel,
		     (Position) width / width,
		     (Position) height / height,
		     &x, &y);

   i = 0;
   XtSetArg(arg[i], XtNx, x);
   ++i;
   XtSetArg(arg[i], XtNy, y);
   ++i;
   XtSetValues(w, arg, i);

   XtPopup(w, XtGrabExclusive);

}

/* ------------- A simple message box ---------------- */

void 
MessageBox(w, string)
     Widget w;
     String string;
{
   int             i;
   Arg             arg[1];

   i = 0;
   XtSetArg(arg[i], XtNlabel, string);
   ++i;
   XtSetValues(messageBox, arg, i);
   Popup(messageshell, w);
}

/*ARGSUSED*/
void 
MessageOK(w, call_data, useless2)
     Widget w;
     XtPointer call_data;
     XtPointer useless2;
{
   XtPopdown((Widget) call_data);
}

void 
initMessageBox()
{
   /* The shell used for messages */
   messageshell = XtCreatePopupShell(
		      "messagepopup", transientShellWidgetClass, commandBox,
				     NULL, 0);
   messageBox = XtCreateManagedWidget(
		    "messagebox", dialogWidgetClass, messageshell, NULL, 0);
   addButton(messageBox, "OK", MessageOK, messageshell);
}

/* ---------- ADDING A PROJECT ------------- */

/*ARGSUSED*/
static void 
Add(widget, event, params, parmcount)
     Widget widget;
     XEvent *event;
     String *params;
     Cardinal *parmcount;
{
   Arg             wargs[1];
   int             n;

   n = 0;
   XtSetArg(wargs[n], XtNvalue, "");
   ++n;
   XtSetValues(addDialog, wargs, n);
   Popup(addshell, toplevel);
}

/* ---- ADD dialog box: Add a project to the projectlist file ---- */


/* Add dialog done button callback */

/*ARGSUSED*/
static void 
AddDone(widget, event, params, parmcount)
     Widget widget;
     XEvent *event;
     String *params;
     Cardinal *parmcount;
{

   String          string;
   int             current;

   save(0, "Adding new project");

   XtPopdown(addshell);
   string = XawDialogGetValueString(addDialog);
   current = currentProject();
   AddNewProject(current, string);

   (void) makethelist();

   XawListChange(list_widget, nameList, 0, 0, True);
   XawListChange(timelist_widget, timeList, 0, 0, True);
   /* Set currency to the new project */
   XawListHighlight(list_widget, current + 1);
   XawListHighlight(timelist_widget, current + 1);

   addprevtimes();
   timeshavechanged();
}

/* Add dialog cancel button callback */

/*ARGSUSED*/
static void 
AddCancel(widget, event, params, parmcount)
     Widget widget;
     XEvent *event;
     String *params;
     Cardinal *parmcount;
{
   XtPopdown(addshell);
}

void 
initAddShell()
{
   Arg             wargs[10];
   int             n;

   /* The shell for the "add project" dialog */
   addshell = XtCreatePopupShell(
			  "addpopup", transientShellWidgetClass, commandBox,
				 NULL, 0);

   n = 0;
   /* This MUST be hardcoded, or else the text box does not appear */
   XtSetArg(wargs[n], XtNvalue, "");
   ++n;
   addDialog = XtCreateManagedWidget(
			   "dialog", dialogWidgetClass, addshell, wargs, n);
   addButton(addDialog, "done", NULL, NULL);
   addButton(addDialog, "cancel", NULL, NULL);
}

/* -------------- EDITING A PROJECT ---------------- */


int 
EditedTime(dialog)
     Widget dialog;
                          

/* Return number of minutes from the edit field */
{
   String          string;
   int             hours, minutes;
   int             count;

   string = XawDialogGetValueString(dialog);
   count = sscanf(string, "%d:%d", &hours, &minutes);
   if (count == 1)
      return hours * 60;
   else if (count == 2)
      return hours * 60 + minutes;
   else
      return 0;
}


void 
EditSetTime(w)
     Widget w;
{
   Arg             arg[1];
   int             n;
   float           pos;

   sprintf(edited_timestring, "%2d:%02d", edited_time / 60,
	   edited_time % 60);
   n = 0;
   XtSetArg(arg[n], XtNvalue, edited_timestring);
   ++n;
   XtSetValues(w, arg, n);
   /* Set scrollbar to percentage of 24 hours */
   pos = (1.0 * edited_time) / (MINSPERDAY + 0.0);
   XawScrollbarSetThumb(scrollbar, pos, -1.0);

}

/*ARGSUSED*/
static void 
Edit(widget, event, params, parmcount)
     Widget widget;
     XEvent *event;
     String *params;
     Cardinal *parmcount;
{
   Arg             arg[2];
   int             n;

   n = 0;
   edited_index = currentProject();
   if (edited_index < 0)
      MessageBox(toplevel, "No highlighted projet");
   else {
      edited_time = timeUsed[edited_index];
      edited_project = nameList[edited_index];
      EditSetTime(editDialog);
      XtSetArg(arg[n], XtNlabel, edited_project);
      ++n;
      XtSetArg(arg[n], XtNvalue, edited_timestring);
      ++n;
      XtSetValues(editDialog, arg, n);
      Popup(editshell, toplevel);
   }
}


/* Edit dialog done button callback */
/*ARGSUSED*/
static void 
EditDone(widget, event, params, parmcount)
     Widget widget;
     XEvent *event;
     String *params;
     Cardinal *parmcount;
{
   XtPopdown(editshell);
   /* Copy data back to the projectlist */
   timeUsed[edited_index] = EditedTime(editDialog);
   timeshavechanged();
}

/* Edit dialog cancel button callback */
/*ARGSUSED*/
static void 
EditCancel(widget, event, params, parmcount)
     Widget widget;
     XEvent *event;
     String *params;
     Cardinal *parmcount;
{
   XtPopdown(editshell);
}

/* Delete current project */
/*ARGSUSED*/
static void 
Delete(widget, event, params, parmcount)
     Widget widget;
     XEvent *event;
     String *params;
     Cardinal *parmcount;
{
   int             cur;

   cur = currentProject();
   if (cur < 0)
      MessageBox(toplevel, "No highlighted projet");
   else {
      save(0, "Deleting a project");
      DeleteProject(cur);
      (void) makethelist();
      ListIsModified();
      addprevtimes();
      timeshavechanged();
   }
}

/*ARGSUSED*/
static void 
EditIncrement(widget, event, params, parmcount)
     Widget widget;
     XEvent *event;
     String *params;
     Cardinal *parmcount;
{
   edited_time = EditedTime(editDialog);
   if (*parmcount > 0)
      edited_time += atoi(params[0]);
   else
      edited_time += incPlus;
   EditSetTime(editDialog);
}

/* ------- Scrollbar handling ------- */

/* Callback function: Set worked-on-this from scrollbar */
/* Called as the JumpProc callback */
/*ARGSUSED*/
void 
ScrollMoved(widget, client_data, call_data)
     Widget widget;
     XtPointer client_data;
     XtPointer call_data;
{
   /* Get the top of the thumb */
   edited_time = (*(float *) call_data) * MINSPERDAY;
   edited_time = edited_time / 5;	/* HACK: Reduce to 5-minute intervals */
   edited_time = edited_time * 5;
   EditSetTime(editDialog);
}

/* Callback function: Respond to left and right button presses */
/* Called as the ScrollProc callback */
/*ARGSUSED*/
void 
ScrollClicked(widget, client_data, call_data)
     Widget widget;
     XtPointer client_data;
     XtPointer call_data;
{
   /* Scroll something proportional */
   int             tojump;
   int             length;
   float           proportion;
   int             addminutes;

   tojump = (int) call_data;

   /*
    * get the current length of the scrollbar. This code DOES NOT WORK.
    */
#ifdef PROBLEM_WITH_GETVALUES_FIXED
   i = 0;
   XtSetArg(arg[i], XtNlength, &length);
   ++i;
   XtGetValues(widget, arg, i);
#else
   length = 240;		/* Read by hand from the app-defaults file */
#endif
   /* Right end moves one-fourth of MINSPERDAY. OK? */
   proportion = tojump * 1.0 / length;
   addminutes = proportion * MINSPERDAY / 4;
   edited_time += addminutes;
   if (edited_time > MINSPERDAY)
      edited_time = MINSPERDAY;
   if (edited_time < 0)
      edited_time = 0;
   edited_time = edited_time / 5;	/* HACK: Reduce to 5-minute intervals */
   edited_time = edited_time * 5;
   EditSetTime(editDialog);
}

static void 
initEditShell()
{
   Widget          doneButton;
   Arg             wargs[10];
   int             n;


   /* The shell for the "add project" dialog */
   editshell = XtCreatePopupShell(
			 "editpopup", transientShellWidgetClass, commandBox,
				  NULL, 0);

   editDialog = XtCreateManagedWidget(
			   "dialog", dialogWidgetClass, editshell, NULL, 0);
   doneButton = addButton(editDialog, "done", NULL, NULL);
   addButton(editDialog, "cancel", NULL, NULL);
   addButton(editDialog, "plus", NULL, NULL);
   addButton(editDialog, "minus", NULL, NULL);
   addButton(editDialog, "delete", NULL, NULL);
   /* Add the scrollbar below the action buttons */
   /* A scrollbar */
   n = 0;
   XtSetArg(wargs[n], XtNfromVert, doneButton);
   ++n;
   scrollbar = XtCreateManagedWidget("scrollbar", scrollbarWidgetClass,
				     editDialog, wargs, n);
   XtAddCallback(scrollbar, XtNjumpProc, ScrollMoved, NULL);
   XtAddCallback(scrollbar, XtNscrollProc, ScrollClicked, NULL);

}

static void 
RegisterActions()
{
   /* Must be static to allow initialization */
   static XtActionsRec actions[] = {
      {"edit", Edit},
      {"add", Add},
      {"addok", AddDone},
      {"addcancel", AddCancel},
      {"edit", Edit},
      {"editok", EditDone},
      {"editcancel", EditCancel},
      {"editincrement", EditIncrement},
      {"delete", Delete},
   };

   XtAppAddActions(appContext, actions, XtNumber(actions));
}



/* Code for initialization of the projectlist editor */
void 
initprojectlisteditor()
{
   /* The "add project" button */
   addproject = addButton(commandBox, "add", NULL, NULL);
   editproject = addButton(commandBox, "edit", NULL, NULL);
   initAddShell();
   initEditShell();
   initMessageBox();
   RegisterActions();
}
