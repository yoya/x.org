/*******************************************************************************

     Copyright (c) 1994    William Pemberton (wfp5p@virginia.edu)

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

*******************************************************************************/



/*
    xbuffy - Bill's version of the multiple mailbox biff

    Author: Bill Pemberton, wfp5p@virginia.edu

    This is a modified version of xmultibiff 2.0 by:

     John Reardon, Midnight Networks, badger@midnight.com, 1993.

 */

/*
  History Log:

    9/21/93:  Merged my changes into Xmultibiff 2.0.  John had included many
              of the changes that made the original Xbuffy, but there were
              many more that made it.  Differences from Xmultibiff 2.0 include:

              - Different parsing of mailheaders.

              - Fewer passes through the mailbox.

              - Automatic backgrounding/nicing.
			
			  - Boxes are now dynamically allocated.  You can watch as many
			  	boxes as memory allows.

              - Smaller
			
			  - Doesn't use multi-box widgets (accomplishes the same
			  	functionality in a different way).
				
			  - audiofile replaced by audiocommand.


*/


#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/time.h>
#include <sys/resource.h>
#ifndef SUN
#include <sys/types.h>
#include <sys/stat.h>
#endif							/* SYSV */
#include "xbuffy.h"
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Command.h>


#include "xbuffy.xbm"

#ifdef WFP_DEBUG
#include "/home/wfp5p/bin/debug_include/malloc.h"
#endif

void CheckBox();
void TimerBreakPopup();
int CountUnixMail();
void ParseMailPath();
void makeBoxTitle();
void initBox();

void ButtonDownHandler();
void ButtonUpHandler();
void BreakPopup();
void ExecuteCommand();
void PopupHeader();
char *EliminatePath();
void UpdateBoxNumber();


struct ApplicationData_s
{
	char *mailBoxes;
#ifdef NNTP
	char *newsBoxes;
#endif							/* NNTP */
	Boolean horiz;
	Boolean shortNames;
	Boolean longNames;
	Boolean origMode;
	Boolean nobeep;
	char *pollTime;
	char *headerTime;
	char *audioCmd;
	char *command;
	char *boxFile;
	char *priority;
	Pixel bg, fg;
};

typedef struct ApplicationData_s ApplicationData_t;


/** globals **/
char versionString[MAX_STRING];
char *programName;
Widget toplevel;
Widget *header;
ApplicationData_t data;
XtAppContext app;
DynObject DynBoxObj;
BoxInfo_t *boxInfo;
int *headerUp;
int nBoxes = 0;
int envPolltime = 0;
int envPriority = 0;
int envHeadertime = 0;
int NNTPinit = 0;
FILE *NNTP_fIn, *NNTP_fOut;


XtResource resources[] = {
	{"mailboxes", "Mailboxes", XtRString, sizeof(String),
	XtOffset(ApplicationData_t *, mailBoxes), XtRString, 0},
	{"nobeep", "Nobeep", XtRBoolean, sizeof(int),
	XtOffset(ApplicationData_t *, nobeep), XtRString, "FALSE"},
	{"horiz", "Horiz", XtRBoolean, sizeof(int),
	XtOffset(ApplicationData_t *, horiz), XtRString, "FALSE"},
	{"command", "Command", XtRString, sizeof(String),
	XtOffset(ApplicationData_t *, command), XtRString, 0},
	{"names", "Names", XtRBoolean, sizeof(int),
	XtOffset(ApplicationData_t *, longNames), XtRString, "FALSE"},
	{"shortnames", "Shortnames", XtRBoolean, sizeof(int),
	XtOffset(ApplicationData_t *, shortNames), XtRString, "FALSE"},
	{"polltime", "Polltime", XtRString, sizeof(String),
	XtOffset(ApplicationData_t *, pollTime), XtRString, "60"},
	{"priority", "Priority", XtRString, sizeof(String),
	XtOffset(ApplicationData_t *, priority), XtRString, "15"},
	{"headertime", "Headertime", XtRString, sizeof(String),
	XtOffset(ApplicationData_t *, headerTime), XtRString, 0},
	{"orig", "Orig", XtRBoolean, sizeof(int),
	XtOffset(ApplicationData_t *, origMode), XtRString, "FALSE"},
	{"audiocmd", "Audiocmd", XtRString, sizeof(String),
	XtOffset(ApplicationData_t *, audioCmd), XtRString, 0},
	{"boxfile", "Boxfile", XtRString, sizeof(String),
	XtOffset(ApplicationData_t *, boxFile), XtRString, 0},
	{XtNforeground, XtCForeground, XtRPixel, sizeof(Pixel),
	XtOffset(ApplicationData_t *, fg), XtRString, XtDefaultForeground},
	{XtNbackground, XtCBackground, XtRPixel, sizeof(Pixel),
	XtOffset(ApplicationData_t *, bg), XtRString, XtDefaultBackground},
#ifdef NNTP
	{"newsboxes", "Newsboxes", XtRString, sizeof(String),
	XtOffset(ApplicationData_t *, newsBoxes), XtRString, 0},
#endif							/* NNTP */

};

XrmOptionDescRec options[] = {
	{"-nobeep", "*nobeep", XrmoptionNoArg, "TRUE"},
	{"-horiz", "*horiz", XrmoptionNoArg, "TRUE"},
	{"-names", "*names", XrmoptionNoArg, "TRUE"},
	{"-shortnames", "*shortnames", XrmoptionNoArg, "TRUE"},
	{"-acmd", "*audiocmd", XrmoptionSepArg, 0},
	{"-poll", "*polltime", XrmoptionSepArg, 0},
	{"-priority", "*priority", XrmoptionSepArg, 0},
	{"-header", "*headertime", XrmoptionSepArg, 0},
	{"-orig", "*orig", XrmoptionNoArg, "TRUE"},
/*	{"-acmd", "*audiocmd", XrmoptionSepArg, 0},				*/
	{"-boxfile", "*boxfile", XrmoptionSepArg, 0},
	{"-command", "*command", XrmoptionSepArg, 0},
};

char *NEWstrdup(s)
	char *s;
{
	/* apparently some OSs choke if you do a strdup with a NULL */

	if (s == NULL)
		return (NULL);

	return ((char *) strdup(s));
}


void CheckBox(i)
	int i;
{
	int num;
	char amt[MAX_STRING];
	Arg args[5];
	int nargs;
	static BoxInfo_t *tempNews = 0;
	BoxInfo_t *currentBox;
	int found;
	static char *mailHeader = NULL;
	int headerSize;
	Boolean beenTouched;
	Boolean isIcon = FALSE;

	currentBox = &boxInfo[i];

#ifdef NNTP
	if (boxInfo[i].type == NNTPBOX)
	{
		num = CountNNTP(currentBox, NULL, &beenTouched);
	}
#endif							/* NNTP */
	if (boxInfo[i].type == MAILBOX)
	{
		num = CountUnixMail(currentBox, NULL, &beenTouched);
	}

	nargs = 0;
	XtSetArg(args[nargs], XtNiconic, &isIcon);


	if ((num > currentBox->n) ||
		((!currentBox->origMode) && ((num != currentBox->n) || (beenTouched))))
	{

		if ((currentBox->headerTime != 0) && (!isIcon) && (num > 0))
		{
			PopupHeader(currentBox->w, currentBox->boxNum, 0, 0);
		}

		if ((!currentBox->nobeep) && (num > 0))
		{
			if (currentBox->audioCmd != NULL)
			{
				system(currentBox->audioCmd);
			}
			else
			{
				XBell(XtDisplay(currentBox->w), 0);
			}
		}
	}

	if (currentBox->n != num)
	{
		currentBox->n = num;
		UpdateBoxNumber(currentBox);
	}


	XtAppAddTimeOut(app, (currentBox->pollTime * 1000), CheckBox, (XtPointer) i);
}



void UpdateBoxNumber(box)
	BoxInfo_t *box;
{
	char amt[MAX_STRING];
	Boolean name, shortName;
	Arg args[5];
	int nargs;

	if (box->boxTitle != NULL)
	{
		sprintf(amt, "%s: %d", box->boxTitle, box->n);
	}
	else
	{
		sprintf(amt, "%d", box->n);
	}

	nargs = 0;

	if (!box->origMode)			/* add color stuff here! */
	{
		if (box->n > 0)
		{
			XtSetArg(args[nargs], XtNbackground, data.fg);
			nargs++;
			XtSetArg(args[nargs], XtNforeground, data.bg);
			nargs++;
		}
		else
		{
			XtSetArg(args[nargs], XtNbackground, data.bg);
			nargs++;
			XtSetArg(args[nargs], XtNforeground, data.fg);
			nargs++;
		}
	}


	XtSetArg(args[nargs], XtNlabel, amt);
	nargs++;
	XtSetValues(box->w, args, nargs);
}


/* event handlers that decides what to do with button clicks */
void ButtonDownHandler(w, i, event, cont)
	Widget w;
	int *i;
	XEvent *event;
	Boolean *cont;
{
	if (event->xbutton.button == 1)
	{
		PopupHeader(w, *i, event, cont);
	}
	/* don't do anything else for other button clicks */
}



void ButtonUpHandler(w, i, event, cont)
	Widget w;
	int *i;
	XEvent *event;
	Boolean *cont;
{
	if (event->xbutton.button == 1)
	{
		BreakPopup(w, *i, event, cont);
	}
	else if (event->xbutton.button == 2)
	{
		ExecuteCommand(w, *i, event, cont);
	}
}



void PopupHeader(w, i, event, cont)
	Widget w;
	int i;
	XEvent *event;
	Boolean *cont;
{
	Arg args[5];
	int nargs;
	Widget tmpCommand;
	Position biff_x, biff_y, root_x, root_y;
	static XtIntervalId timerID;
	static int rootH = 0;
	static int rootW = 0;
	int number;

	static Boolean firstTime = TRUE;
	static DynObject mailHeaders;
	static char *hdrPtr;

	Dimension headerW, headerH;
	BoxInfo_t *currentBox;
	Boolean beenTouched;


	currentBox = &boxInfo[i];

	if (rootH == 0)
	{
		rootH = DisplayHeight(XtDisplay(w), DefaultScreen(XtDisplay(w)));
		rootW = DisplayWidth(XtDisplay(w), DefaultScreen(XtDisplay(w)));
	}


	if ((!firstTime) && (DynSize(mailHeaders) != 0))
	{
		DynDestroy(mailHeaders);
	}

	firstTime = FALSE;
	mailHeaders = DynCreate(sizeof(char), MAX_STRING);

#ifdef DEBUG
	DynDebug(mailHeaders, 1);
	DynParanoid(mailHeaders, 1);
#endif

#ifdef NNTP
	/* check to see if there is any news before proceeding */
	if (boxInfo[i].type == NNTPBOX)
	{
		number = CountNNTP(&boxInfo[i], mailHeaders, &beenTouched);
		DynAdd(mailHeaders, "\0");
		hdrPtr = (char *) DynGet(mailHeaders, 0);
	}
#endif

	/* and mail */
	/* update the number on the box (in case there are new articles) */
	if (currentBox->type == MAILBOX)
	{
		number = CountUnixMail(currentBox, mailHeaders, &beenTouched);


		DynAdd(mailHeaders, "\0");
		hdrPtr = (char *) DynGet(mailHeaders, 0);
	}

	/* if the number is different, update it */
	if ((number != currentBox->n) || (beenTouched))
	{
		currentBox->n = number;

		UpdateBoxNumber(&boxInfo[i]);
	}

	/* if the number is zero, there's no header, so leave */
	if (boxInfo[i].n == 0)
	{
		return;
	}

	/* if its already up, pop it down, because we must update it */
	if (headerUp[i] == TRUE)
	{
		XtRemoveTimeOut(timerID);
		BreakPopup(0, i, 0, 0);
	}

	/* Calculate Relative position -> Root absolute position */
	nargs = 0;
	XtSetArg(args[nargs], XtNwidth, &biff_x);
	nargs++;
	XtSetArg(args[nargs], XtNheight, &biff_y);
	nargs++;
	XtGetValues(w, args, nargs);
	XtTranslateCoords(w, biff_x, biff_y, &root_x, &root_y);


	header[i] = XtCreatePopupShell(currentBox->boxTitle, transientShellWidgetClass,
								   currentBox->w, 0, 0);

	nargs = 0;
	XtSetArg(args[nargs], XtNlabel, hdrPtr);
	nargs++;
	XtSetArg(args[nargs], XtNhighlightThickness, 0);
	nargs++;

	tmpCommand = XtCreateManagedWidget("popup", commandWidgetClass,
									   header[i], args, nargs);

	XtAddCallback(tmpCommand, XtNcallback, BreakPopup, (XtPointer) i);

	if (!XtIsRealized(header[i]))
	{
		XtRealizeWidget(header[i]);
	}

	/* see where we should put this thing so its on the screen */
	/* i.e. make sure we can see it */
	nargs = 0;
	XtSetArg(args[nargs], XtNwidth, &headerW);
	nargs++;
	XtSetArg(args[nargs], XtNheight, &headerH);
	nargs++;
	XtGetValues(header[i], args, nargs);
	if (((int) root_x + (int) headerW) > rootW)
	{
		root_x = (Position) rootW - (Position) headerW;
	}
	if (((int) root_y + (int) headerH) > rootH)
	{
		root_y = (Position) rootH - (Position) headerH;
	}
	nargs = 0;
	XtSetArg(args[nargs], XtNx, root_x);
	nargs++;
	XtSetArg(args[nargs], XtNy, root_y);
	nargs++;
	XtSetValues(header[i], args, nargs);

	XtPopup(header[i], XtGrabNone);

	headerUp[i] = TRUE;
	/* free alloc'ed string */

	/* register a routine to pop it down if it was invoked from a routine */
	if (event == 0)
	{
		timerID = XtAppAddTimeOut(app, (currentBox->headerTime * 1000),
								  TimerBreakPopup, (XtPointer) i);
	}
}

void TimerBreakPopup(i)
	int i;
{
	BreakPopup(0, i, 0, 0);
}

void BreakPopup(w, i, event, cont)
	Widget w;
	int i;
	XEvent *event;
	Boolean *cont;
{
	if (headerUp[i] != TRUE)
	{
		return;
	}
	XtPopdown(header[i]);
	XtDestroyWidget(header[i]);
	headerUp[i] = FALSE;
}


void ExecuteCommand(w, i, event, cont)
	Widget w;
	int i;
	XEvent *event;
	Boolean *cont;
{
	int nargs;
	Arg args[2];
	char *command;
	int pid;
	BoxInfo_t *currentBox;

	currentBox = &boxInfo[i];

	if (currentBox->command != NULL)
	{
		system(currentBox->command);
	}
}


int CountUnixMail(mailBox, headerString, beenTouched)
	BoxInfo_t *mailBox;
	DynObject headerString;
	Boolean *beenTouched;
{
	FILE *fp = 0;
	char buffer[MAX_STRING];
	char From[MAX_STRING], Subject[MAX_STRING];
	register int count = 0;
	int status;
	register Boolean in_header = FALSE;
	struct stat f_stat;

	*beenTouched = FALSE;

	stat(mailBox->box, &f_stat);

	if ((f_stat.st_size != mailBox->st_size) ||
		(f_stat.st_mtime > mailBox->box_mtime))
	{
		mailBox->st_size = f_stat.st_size;
		mailBox->box_mtime = f_stat.st_mtime;
		*beenTouched = TRUE;
	}

	if ((!*beenTouched) && (headerString == NULL))
		return (mailBox->n);

	fp = fopen(mailBox->box, "r");
	if (fp == NULL)
		return 0;

	From[0] = Subject[0] = '\0';


	while (fgets(buffer, MAX_STRING - 1, fp) != 0)
	{
		if ((index(buffer, '\n') == NULL) && (!feof(fp)))
		{
			int c;

			while ((c = getc(fp)) != EOF && c != '\n');	/* keep reading */
		}

		if ((!in_header) && (real_from(buffer)))
		{
			in_header = TRUE;
			status = NEW_MSG;
		}
		else if (in_header)
		{
			if (header_cmp(buffer, "From", NULL))
			{
				strcpy(From, buffer);

			}

			if (header_cmp(buffer, "Subject", NULL))
			{
				strcpy(Subject, buffer);
			}

			if (header_cmp(buffer, "Status", NULL))
			{
				remove_header_keyword(buffer);
				if (*buffer == 'N')
					status = NEW_MSG;
				else
					status = READ_MSG;
			}
			else if (buffer[0] == LINEFEED)
			{
				in_header = FALSE;
				if ((status == NEW_MSG) || (mailBox->origMode))
				{
					count++;
					if (headerString != NULL)
					{
						if (strlen(From) != 0)
							DynInsert(headerString, ((DynHigh(headerString) > 0) ? (DynSize(headerString)) : 0), From, strlen(From));

						if (strlen(Subject) != 0)
							DynInsert(headerString, ((DynHigh(headerString) > 0) ? (DynSize(headerString)) : 0), Subject, strlen(Subject));
					}
				}
				From[0] = Subject[0] = '\0';

			}

		}
	}
	fclose(fp);

	return count;
}


void initBox(box, BoxType, pollTime, headerTime, BoxNameType, command, audioCmd,
			     title, origMode, nobeep)
	char *box;
	BoxType_t BoxType;
	int pollTime;
	int headerTime;
	BoxNameType_t BoxNameType;
	char *command;
	char *audioCmd;
	char *title;
	Boolean origMode;
	Boolean nobeep;

{

	BoxInfo_t tempBox;
	char *ptr;

/* get rid of trailing whitespace in box*/

	ptr = box + strlen(box) - 1;

	while (isspace(*ptr))
		*ptr-- = '\0';




#ifdef DEBUG
	fprintf(stderr, "Init Box = *%s*\n", box);
	fprintf(stderr, "nboxes = %i\n", nBoxes);
	fprintf(stderr, "type = %i\n", BoxType);
	fprintf(stderr, "command	= *%s*\n", command);
	fprintf(stderr, "audio = *%s*\n", audioCmd);
	fprintf(stderr, "boxTitle = *%s*\n", title);
	fprintf(stderr, "pollTime = %i  headerTime = %i\n", pollTime, headerTime);
	fprintf(stderr, "nobeep = %i  origMode = %i \n", nobeep, origMode);
	fprintf(stderr, "nametype = %i\n\n", BoxNameType);
#endif

	tempBox.box = NEWstrdup(box);
	tempBox.type = BoxType;
	tempBox.boxNum = nBoxes;

	if (BoxType = NNTPBOX)
	{
		tempBox.articles = DynCreate(sizeof(Articles_t), 2);
#ifdef DEBUG
		DynDebug(tempBox.articles, 1);
		DynParanoid(tempBox.articles, 1);
#endif

	}
	if ((pollTime <= 0) || (pollTime >= 3600))
		tempBox.pollTime = envPolltime;
	else
		tempBox.pollTime = pollTime;

	if ((tempBox.type == NNTPBOX) && (tempBox.pollTime < 180))
		tempBox.pollTime = 180;

	if ((headerTime < 0) || (headerTime >= 60))
		tempBox.headerTime = envHeadertime;
	else
		tempBox.headerTime = headerTime;

	tempBox.BoxNameType = BoxNameType;

	tempBox.boxTitle = NEWstrdup(title);

	if (tempBox.BoxNameType == UNDEF)
	{
		if (data.shortNames)
			tempBox.BoxNameType = SHORT;
		if (data.longNames)
			tempBox.BoxNameType = LONG;
	}

	makeBoxTitle(&tempBox);

	tempBox.command = NEWstrdup(command);
	tempBox.audioCmd = NEWstrdup(audioCmd);
	tempBox.origMode = origMode;
	tempBox.nobeep = nobeep;

	tempBox.box_mtime = tempBox.st_size = 0;

	DynAdd(DynBoxObj, &tempBox);
	boxInfo = (BoxInfo_t *) DynGet(DynBoxObj, 0);

	nBoxes++;
}

void ParseMailPath()
{
	char *mailPath = 0;
	char *boxes = 0;
	int pos = 0;
	char *str = 0;

	/* get mail path */
	if ((mailPath = getenv("MAILPATH")) != 0)
	{
		boxes = mailPath;
	}
	else if ((mailPath = getenv("MAIL")) != 0)
	{
		boxes = mailPath;
	}
	else if (data.mailBoxes != 0)
	{
		boxes = data.mailBoxes;
	}

	str = (char *) strtok(boxes, ":, ");
	while (str != NULL)
	{
		initBox(NEWstrdup(str), MAILBOX, envPolltime, envHeadertime, UNDEF, data.command,
				data.audioCmd, NULL, data.origMode, data.nobeep);

		str = (char *) strtok(NULL, ":, ");
	}
}


char *EliminatePath(path)
	char *path;
{
	char *file = 0;

	file = (char *) strrchr(path, '/');
	file = (file ? ++file : path);

	return (file);
}

#ifdef NNTP
void ParseNewsPath()
{
	char *newsPath = 0;
	char *boxes = 0;
	int pos = 0;
	char *str = 0;

	/* get nntp path */
	if ((newsPath = getenv("NEWSPATH")) != 0)
	{
		boxes = newsPath;
	}
	else if (data.newsBoxes != 0)
	{
		boxes = data.newsBoxes;
	}

	str = strtok(boxes, ":, ");
	while (str != NULL)
	{
		BoxInfo_t tempBox;

		tempBox.boxNum = nBoxes;
		tempBox.box = NEWstrdup(str);
		boxInfo[nBoxes].type = NNTPBOX;
		boxInfo[nBoxes].n = CountNNTP(&boxInfo[nBoxes], NULL, NULL);
		nBoxes++;
		str = strtok(NULL, ":, ");
	}
}

#endif							/* NNTP */




/* the the icon if it is not already loaded */
void LoadIcon(w)
	Widget w;
{
	Display *display = XtDisplay(w);
	int screen;
	Pixmap icon_pixmap = (Pixmap) 0;
	Arg arg;

	screen = DefaultScreen(display);

	/* User sets iconPixmap resource, converter does the right thing.. */
	XtSetArg(arg, XtNiconPixmap, &icon_pixmap);
	XtGetValues(w, &arg, 1);
	if (icon_pixmap == (Pixmap) 0)
	{
		XtSetArg(arg, XtNiconPixmap,
				 XCreateBitmapFromData(display,
									   RootWindow(display, screen),
									   xbuffy_bits, xbuffy_width,
									   xbuffy_height));
		XtSetValues(w, &arg, 1);
	}
}


void makeBoxTitle(currentBox)
	BoxInfo_t *currentBox;
{
	char line[MAX_STRING];

	line[0] = '\0';

	if (currentBox->type == MAILBOX)
	{
		switch (currentBox->BoxNameType)
		{
		case SHORT:
			strcpy(line, EliminatePath(currentBox->box));
			break;
		case LONG:
			strcpy(line, currentBox->box);
			break;
		case NONE:
		case USR:
			break;
		}

		if (line[0] != '\0')
			currentBox->boxTitle = NEWstrdup(line);
	}
	else
	{
		switch (currentBox->BoxNameType)
		{
		case SHORT:
		case LONG:
			strcpy(line, currentBox->box);
			break;
		case NONE:
		case USR:
			break;
		}

		if (line[0] != '\0')
			currentBox->boxTitle = NEWstrdup(line);
	}


}

/*
void stripAndMove(target, source)
char *target;
char *source;
{
	char *t, *s;

	s = source;
	t = target;

	while (*s != '\0')
	{
		if (!isspace(*s))
			*t++ = *s;
		s++;
	}
	*t = '\0';
}
*/

void Usage()
{
	printf("Usage: %s [toolkit options] [options] <file> ...\n\n", programName);
	printf("Options are:\n");
	printf("  -help           print this message\n");
	printf("  -version        print the version number\n");
	printf("  -poll <secs>    how often to poll the file(s); default: 60\n");
	printf("  -header <secs>  popup header when mail is received\n");
	printf("                   (use '0' for mouse press only)\n");
	printf("  -acmd <command> command for audio instead of <bell>\n");
	printf("  -boxfile <file> filename containing names of mailboxes\n");
	printf("  -horiz          place the boxes horizontally; default: vertical\n");
	printf("  -nobeep         don't ring bell when mail is received\n");
	printf("  -names          display full path of mail files in the boxes\n");
	printf("  -shortnames     display names of mail files in the boxes\n");
	printf("  -orig           original mode - display all messages in the boxes\n");
	printf("  -command <cmd>  system command to execute when middle button is pushed\n");
	printf("  -mail <files>   specify a mailbox(s) to watch\n");
#ifdef NNTP
	printf("  -news <groups> specify a newsgroup(s) to watch\n");
#endif							/* NNTP */
	printf("\n");
	printf("If there are any files specified on the command line, it will\n");
	printf("monitor those mail files, otherwise it will use your MAILPATH\n");
	printf("environment variable.\n");
	printf("\n");
}


void main(argc, argv)
	int argc;
	char *argv[];

{
	FILE *boxes;
	static Boolean mailArgs;
	Widget form;
	Widget command;
	int i;
	char *check;
	char name[MAX_STRING];
	Pixel bg, fg;
	Arg args[5];
	int nargs;
	int pid;

#ifdef DEBUG
/*	gets(name);*/
#endif

	/* initialize program name and version string */
	programName = EliminatePath(argv[0]);
	sprintf(versionString, "%s v%d.%d",
			programName, MAJOR_VERSION, MINOR_VERSION);

	mailArgs = TRUE;

	nBoxes = 0;
	DynBoxObj = DynCreate(sizeof(BoxInfo_t), 1);

#ifdef DEBUG
	DynDebug(DynBoxObj, 1);
	DynParanoid(DynBoxObj, 1);
#endif

	boxInfo = (BoxInfo_t *) DynGet(DynBoxObj, 0);

	nargs = 0;
	XtSetArg(args[nargs], XtNallowShellResize, TRUE);
	nargs++;

	toplevel = XtAppInitialize(&app, X_RESOURCE_CLASS,
							   options, XtNumber(options),
							   &argc, argv, 0, args, nargs);

	XtGetApplicationResources(toplevel, &data, resources, XtNumber(resources),
							  0, 0);

	/* initialize some values */
	if (data.pollTime != NULL)
		envPolltime = atoi(data.pollTime);

	if ((data.pollTime == NULL) && ((check = getenv("MAILCHECK")) != 0))
	{
		if ((envPolltime = atoi(check)) < 0)
		{
			fprintf(stderr, "MAILCHECK has illegal value\n");
		}
	}

	if ((envPolltime <= 0) || (envPolltime >= 3600))
		envPolltime = 60;

	if (data.priority != NULL)
		envPriority = atoi(data.priority);

	if ((envPriority < 0) || (envPriority >= 20))
		envPriority = 15;

	if (data.headerTime != NULL)
		envHeadertime = atoi(data.headerTime);

	if (envHeadertime <= 0)
		envHeadertime = 0;
	if (envHeadertime >= 60)
		envHeadertime = 60;

	argc--;
	++argv;
	while (argc)
	{
		if (strcmp("-help", *argv) == 0)
		{
			Usage();
			exit(0);
		}
		else if (strcmp("-version", *argv) == 0)
		{
			printf("%s\n", versionString);
			exit(0);
		}
		else if (strcmp("-news", *argv) == 0)
		{
#ifndef NNTP
			fprintf(stderr, "program not compiled with -DNNTP ignoring %s\n", *argv);
#else
			mailArgs = FALSE;
#endif							/* !NNTP */
		}
		else if (strcmp("-mail", *argv) == 0)
		{
			mailArgs = TRUE;
		}
		else
		{
			if (*argv[0] == '-')
			{
				fprintf(stderr, "Bad option: %s\n\n", *argv);
				Usage();
				exit(-1);
			}

			if (mailArgs)
			{
				initBox(NEWstrdup(*argv), MAILBOX, envPolltime, envHeadertime,
						UNDEF, data.command, data.audioCmd, NULL, data.origMode, data.nobeep);

			}

#ifdef NNTP
			else
			{
				int dummy;

				initBox(NEWstrdup(*argv), NNTPBOX, envPolltime, envHeadertime,
						UNDEF, data.command, data.audioCmd, NULL, data.origMode, data.nobeep);

			}
#endif							/* NNTP */

		}
		argc--;
		++argv;
	}


	if ((data.boxFile != 0) && (nBoxes == 0))	/* 7! needs work to deal with
												   NNTP & such */
	{
		readBoxfile(data.boxFile);
	}


	if (nBoxes == 0)
	{
		ParseMailPath();
#ifdef NNTP
		ParseNewsPath();
#endif							/* NNTP */
	}

	/* if there are still no boxes, what's the point? */
	if (nBoxes == 0)
	{
		fprintf(stderr, "nothing to watch is specified\n");
		fprintf(stderr, "check $MAILPATH / XBuffy.mailboxes\n");
#ifdef NNTP
		fprintf(stderr, "check $NEWSPATH / XBuffy.newsboxes\n");
#endif							/* NNTP */
		Usage();
		exit(-1);
	}

	/* all nasty config params, are checked, we're ready to go */
	/* load up multibiff icon */
	LoadIcon(toplevel);

	nargs = 0;
	if (data.horiz)
	{
		XtSetArg(args[nargs], XtNorientation, XtorientHorizontal);
		nargs++;
	}

	form = XtCreateManagedWidget("box", boxWidgetClass, toplevel, args, nargs);

	if ((header = (Widget *) malloc(nBoxes * sizeof(Widget))) == 0)
	{
		fprintf(stderr, "Can't malloc header widgets\n");
		exit(-1);
	}
	if ((headerUp = (int *) malloc(nBoxes * sizeof(int))) == 0)
	{
		fprintf(stderr, "Can't malloc header flags\n");
		exit(-1);
	}

	XtRealizeWidget(toplevel);

	for (i = 0; i < nBoxes; i++)
	{
		Boolean dummy;

		headerUp[i] = FALSE;

		if (boxInfo[i].type == MAILBOX)
			boxInfo[i].n = CountUnixMail(&boxInfo[i], NULL, &dummy);

#ifdef NNTP
		if (boxInfo[i].type == NNTPBOX)
			boxInfo[i].n = CountNNTP(&boxInfo[i], NULL, &dummy);
#endif

		sprintf(name, "box%d", i);

		boxInfo[i].w = XtCreateManagedWidget(name, commandWidgetClass, form, args, nargs);


		XtAddEventHandler(boxInfo[i].w, ButtonPressMask, True,
						  ButtonDownHandler, &boxInfo[i].boxNum);
		XtAddEventHandler(boxInfo[i].w, ButtonReleaseMask, True,
						  ButtonUpHandler, &boxInfo[i].boxNum);

		UpdateBoxNumber(&boxInfo[i]);

		CheckBox(i);
	}

#ifdef DEBUG
	fprintf(stderr, "bg = %i, fg = %i\n", data.bg, data.fg);
	for (i = 0; i < nBoxes; i++)
	{
		fprintf(stderr, "box = %s\n", boxInfo[i].box);
		fprintf(stderr, "pollTime = %i\n", boxInfo[i].pollTime);
		fprintf(stderr, "headerTime = %i\n", boxInfo[i].headerTime);
		fprintf(stderr, "origMode = %i\n", boxInfo[i].origMode);
		fprintf(stderr, "nobeep = %i\n", boxInfo[i].nobeep);
		fprintf(stderr, "command = %s\n", boxInfo[i].command);
		fprintf(stderr, "nameType = %i\n", boxInfo[i].BoxNameType);

	}
#endif

/* RealizeWidget was here */

#ifndef DEBUG
	if (setpriority(PRIO_PROCESS, 0, envPriority) == -1)
		perror("Proirity change Failed");

	/* put ourself in the background */
	switch (pid = fork())
	{
	case 0:
		XtAppMainLoop(app);		/* in child do the stuff */
		break;
	case -1:
		perror("Fork failure");
		XtAppMainLoop(app);		/* fork failed - carry on in the parent instead */
		break;
	default:
		exit(0);				/* ok its going we can stop now */
		break;
	}
#else
	XtAppMainLoop(app);
#endif

}
