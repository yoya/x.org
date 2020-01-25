/* xcmdmenu - sample X menu / command interface from file data. */

/* Copyright (c) 1993, 1994 Christopher Alexander North-Keys
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
 */

#ifndef lint
static char Version[] = "@(#)xcmdmenu.c 1.6 Copyright (c) 1994 North-Keys";
#endif

/* First compiled 29 January 1993 around 9:00 (and naturally core-dumped)
 * Compilation at 13:25 ran successfully without coredump.
 * Comprehensive rewrite of the X section Thu Jul 12 01:17:48 CDT 1990.
 * Added 1.3 feature Tue Apr 27 1993 (amazing how strange old code looks...)
 * Added 1.4 scrollbars
 * Tue May 31 1994 : renamed from xcmdpanel to xcmdmenu to fit X11R6 prereq`s.
 */

#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xaw/Cardinals.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Paned.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Text.h>
#include <X11/Xaw/Viewport.h>
#include <stdio.h>
#include <string.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>

/* Q-Bools and Functions from stdelf.c */
/* Quasi-Booleans */
#define bool   int
#ifndef TRUE
#define TRUE   (1)
#endif
#ifndef FALSE
#define FALSE  (0)
#endif
#define SUCC   (0)
#define FAIL   (-1)
#ifndef NULL
#define NULL   (0)
#endif

#ifndef XCMDMENU_CONFIG
#define XCMDMENU_CONFIG "."   /* this will, of course, fail */
#endif

/* Functions */
void  Fatal();
long  SwallowFile();

typedef struct _Datum
{
	String  tag;
	String  text;
	struct _Datum *next;
} Datum;

Datum *ConvertFileToData();

typedef struct _FlagsStruct {
    Boolean paned;
	Boolean ghost;
	Boolean debug;
	Boolean oneshot;
} FlagsStruct;

static XtResource resources[] = {      /* XtPointer only to make lint quiet */
    {"paned", "Paned", XtRBoolean, sizeof(Boolean),
     XtOffset(FlagsStruct *, paned), XtRImmediate, (XtPointer)True},
    {"ghost", "Ghost", XtRBoolean, sizeof(Boolean),
     XtOffset(FlagsStruct *, ghost), XtRImmediate, (XtPointer)False},
    {"debug", "Debug", XtRBoolean, sizeof(Boolean),
	 XtOffset(FlagsStruct *, debug), XtRImmediate, (XtPointer)False},
	{"oneshot", "Oneshot", XtRBoolean, sizeof(Boolean),
	 XtOffset(FlagsStruct *, oneshot), XtRImmediate, (XtPointer)False},
};

static XrmOptionDescRec options[] = {
    {"-paned",   "paned",	XrmoptionNoArg,   "True"},
    {"-boxed",   "paned",	XrmoptionNoArg,   "False"},
    {"-ghost",   "ghost",	XrmoptionNoArg,   "True"},
    {"-noghost", "ghost",	XrmoptionNoArg,   "False"},
    {"-debug",   "debug",	XrmoptionNoArg,   "True"},
	{"-oneshot", "oneshot",   XrmoptionNoArg,   "True"},
	{"-persist", "oneshot",   XrmoptionNoArg,   "False"},
};

static Arg space_args[] = {
    {XtNhSpace, (XtArgVal)4},
	{XtNvSpace, (XtArgVal)4},
};

static Arg null_args[] = {
    { NULL, NULL},
};

String fallback_resources[] = {
    "*input:                  True",
    "*showGrip:               False",
	"*exit*label:             Exit",
	"*execute*label:          Execute",
	"*allowHoriz:             false",
	"*allowVert:              true",
    NULL,
};

void   Exit();
void   Execute();
Datum *ConvertFileToData( /*buffer length*/ );
char  *ExtractEscapedString( /*escape, string*/ );
int    FindClosure( /*open, close, escape, start, limit, start_ret, end_ret*/ );
void   ConvertDataToWidgets( /*items, box*/ );
void   Syntax();

FlagsStruct flags;

int
main (argc, argv)
Cardinal argc;
String *argv;
{
	Cardinal i;
	Widget toplevel, viewport, box;
	XtAppContext app_con;
	Arg c_args[2];
	long length;  			/* length of read file */
	String filename = (String)0;
	char *buffer = (char*)0;			/* for file read */
	Datum *items = (Datum*)0;
	
	toplevel = XtAppInitialize(&app_con, "XCmdPanel",
							   options, XtNumber(options),
							   &argc, argv,
							   fallback_resources,
							   NULL,
							   ZERO);

	if (argc == 2)
		filename = argv[1];		/* use commandline configfile if given */

	if( ! filename)
	{
		String filedefault = ".xcmdmenu";
		FILE *tmp = (FILE*)0;
		if(tmp = fopen(filename = filedefault, "r"))	/* else check in cwd */
			fclose(tmp);
		else
		{
			char *home = (char*)0;
			if(home = (char*)getenv("HOME"))
			{
				int filename_len = strlen(home) + strlen(filedefault) + 2;
				if(filename = (char *)calloc(1, filename_len * sizeof(char)))
				{
					strcpy(filename, home);
					strcat(filename, "/");
					strcat(filename, filedefault);		/* fallback to ~ */
					if(tmp = fopen(filename, "r"))
						fclose(tmp);
					else
						filename = (String)0;
				}
				else perror("main - calloc");
			}
		}
	}

	if( ! filename)
	{
		FILE *tmp = (FILE*)0;
		if(tmp = fopen(filename = XCMDMENU_CONFIG, "r"))
			fclose(tmp);
		else
			filename = (String)0;
	}

	if( ! filename || (FAIL == (length = SwallowFile(&buffer, filename))))
	{
		fprintf(stderr, "Error seeking config file; aborting.\n");
		Syntax(argv[0]);
	}
	
    XtGetApplicationResources(toplevel,
							  (XtPointer) &flags,
							  resources, XtNumber(resources),
							  NULL,
							  ZERO );

	viewport = XtCreateManagedWidget(argv[0],
									 viewportWidgetClass,
									 toplevel,
									 space_args,
									 XtNumber(space_args));
	
	box = XtCreateManagedWidget(argv[0],
								flags.paned? panedWidgetClass : boxWidgetClass,
								viewport,
								space_args,
								XtNumber(space_args));
	
	if(NULL == (items = ConvertFileToData(buffer, length)))
	{
		fprintf(stderr,
				"Error in data conversion from file [ %s ], aborting.\n",
				filename);
		Exit();
	}
	
	ConvertDataToWidgets(items, box);

	XtAddCallback(XtCreateManagedWidget("Exit", commandWidgetClass,
										box,
										null_args,
										XtNumber(null_args)),
				  XtNcallback,
				  Exit,
				  NULL);

	XtRealizeWidget(toplevel);
    XtAppMainLoop(app_con);
}

void
Exit(widget, clientData, callData)
Widget widget;
caddr_t clientData, callData;
{
    XtDestroyApplicationContext(XtWidgetToApplicationContext(widget));
	exit(0);
}

void
Execute(widget, clientData, callData)
Widget widget;
String clientData;
XtPointer callData;
{
	if(strlen(clientData)) system(clientData);
	if(flags.oneshot) Exit(widget, clientData, callData);
	if(flags.ghost) XtSetSensitive(widget, 0);
	return;
}

Datum *ConvertFileToData(buffer, length)
String buffer;
long length;
{
	Datum *items, *item;
	char *ind, *lim, StrBeg='(', StrEnd=')', StrEsc='\\';
	short state;
#define S_Tag    (0)
#define S_Text   (1)
	
 	lim = (char *)(&(buffer[length]));
	ind = (char *)(&(buffer[0]));
	if(NULL == (item = items = (Datum *)calloc(1, sizeof(Datum) + 1)))
		Fatal("ConvertFileToData", "calloc");

	for(state = S_Tag ; /*no-exit*/ ; /*internal*/ )
	{
		char *end;
		int  status;
		
		status = FindClosure(StrBeg, StrEnd, StrEsc, ind, lim, &ind, &end);
		switch (status)
		{
		  case FAIL:  return NULL;     break;
		  case FALSE: return items;    break;
		  case TRUE:
		  default:    *end = '\0';     break;
		}
				
		switch (state)
		{
		  case S_Tag:
			item->tag = ExtractEscapedString(StrEsc, ++ind);
			ind = end + 1;
			state = S_Text;
			break;
		  case S_Text:
			item->text = ExtractEscapedString(StrEsc, ++ind);
			item->next = (Datum *)calloc(1, sizeof(Datum));
			if(flags.debug)
				(void)fprintf(stderr, "`%s'  `%s'\n", item->tag,
							  item->text ? item->text : "<NULL>");
			item = item->next;
			ind = end + 1;
			state = S_Tag;
			break;
		  default:
			return NULL;
			break;
		}
	}
}

char *ExtractEscapedString(escape, string)
char escape, *string;
{
	char *new;
	char *n_i, *s_i, *lim;

	if(NULL == (new = (char *)calloc(strlen(string)+1, sizeof(char))))
		Fatal("ExtractEscapedString", "calloc");

	lim = &string[strlen(string)];

	/* Upon hitting the escape char, drop it and forcecopy the next */
	for(n_i = new, s_i = string; s_i <= lim; n_i++, s_i++)
	{
		if(*s_i == escape) s_i++;
		*n_i = *s_i;
	}
	return new;
}

int FindClosure(open, close, escape, start, limit, start_return, end_return)
char open, close, escape, *start, *limit, **start_return, **end_return;
{
	unsigned int level = 0;
	char *index;
	/* escape causes the next character to be skipped, period */

	if(start == NULL)
	{
		(void)fprintf(stderr, "FindClosure:  illegal value.\n");
		return FAIL;
	}
	
	/* NOTE: incoming start/end are COPIES, thus returns are safe to NULL.
	 * Such will be the return values if the start/end points are unfound.
	 */
	*start_return = NULL;
	*end_return = NULL;
	
	for(index=start; index<=limit; index++)
	{
		if(*index == escape)
			if(index >= limit)
				return FAIL;
			else
				index++;
		else
			if(*index == open)
			{
				level++;
				/* be clever and only set on the first instance. */
				if (*start_return == NULL) *start_return = index;
			}
			else
				if(*index == close)
					if(--level == 0)
					{
						*end_return = index;
						return TRUE;
					}
	}
	return FALSE;
}
			

void ConvertDataToWidgets(items, box)
Datum *items;
Widget box;
{
	Datum *item;
	Widget cur;
		
	for(item = items; item->next ; item = item->next)
	{
		if(flags.debug)
		{
			(void)fprintf(stderr,
						  "ConvertDataToWidgets:  tag %s, \ttextlength %d\n",
						  item->tag,
						  strlen(item->text));
		}		
		XtAddCallback(cur = XtCreateManagedWidget(item->tag,
												  commandWidgetClass,
												  box,
												  null_args,
												  XtNumber(null_args)),
					  XtNcallback,
					  Execute,
					  item->text);
		if(!strlen(item->text)) XtSetSensitive(cur, 0);
	}
	return;
}

void Syntax(prog)
char *prog;
{
	fprintf(stderr, "[-ghost]   [-noghost] (default noghost)\n");
	fprintf(stderr, "[-boxed]   [-paned]   (default paned)\n");
	fprintf(stderr, "[-oneshot] [-persist] (default persist)\n");
	fprintf(stderr, "[<config_filename>]   (default is ./.%s)\n", prog);
	fprintf(stderr, "[-debug]\n");
	exit(1);
}
				
/*
 * stdelf.c Copyright (c) 1989 Christoph. Alex. North-Keys
 * These functions may be used, copied, modified, and redistributed freely
 * so long as this notice remains intact.
 */

/*
 * stdelf.c :  Begun 14 mar 89 :  Library of simple routines
 */

#ifndef lint
static char SVersion[]="Code stdelf.c d.14.03.1989 (C. A. North-Keys)";
#endif
	
void
Fatal(loc, s)
char *loc, *s;
{
    fprintf(stderr, "%s: ", loc);
    perror(s);
    exit(-1);
}

long
SwallowFile(addr_buffer, path)
char **addr_buffer, *path;
{
	struct stat statbuf;
	int         in_count;
	int         fd = -1;

	/* Attempt to open desired file */
	if(((fd)=open(path, O_RDONLY))==-1)
	{
		(void)fprintf(stderr, "SwallowFile: %s not opened\n", path);
		return (FAIL);
	}

	if(fstat(fd, &statbuf)==-1)
	{
		(void)fprintf(stderr, "SwallowFile: error on fstat file %s\n", path);
		return (FAIL);
	}
	
	/* Get a buffer to fit */
	if((*addr_buffer=(char *)calloc(1, statbuf.st_size+1))==NULL)
	{
		(void)fprintf(stderr, "SwallowFile: no space for calloc\n");
		return (FAIL);
	}
	
	/* Read in the file */
	if((in_count = read(fd, *addr_buffer, statbuf.st_size)) != statbuf.st_size)
	{
		(void)fprintf(stdout,
					  "SwallowFile: error(?) %d/%d bytes read from %s\n",
					  in_count, statbuf.st_size, path);
		(void)free(*addr_buffer);
		return (FAIL);
	}
	return (in_count);
}

/* end of excerpt from stdelf.c */

