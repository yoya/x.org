/* Header file for TIMEX */
/* Declares the variables/routines that are shared between the project list
   editor and the main program */
/* Commonly used include files */
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
/* OS definitions - ought to be avoidable by dilligent use of X,
   but don't see how */
#include <stdio.h>
#include <errno.h>
/* needed on Ultrix v 4.0, must be before sys/stat.h - David Raz */
#include <sys/types.h>
/* Some systems (HP-UX and Ultrix 4.0) have fopen constants in this file */
#include <sys/stat.h>
/* Some systems (eg, NeXT) don't define some Posix constants/macros */
#ifndef S_IRUSR
#define S_IRUSR S_IREAD
#define S_IWUSR S_IWRITE
#define S_IXUSR S_IEXEC
#endif
#ifndef S_ISDIR
#define S_ISDIR(m)      (((m)&S_IFMT) == S_IFDIR)
#endif
/* Needed for catching die signals */
#include <signal.h>
/* If we want getenv prototyped, we want this */
#include <stdlib.h>
/* Needed for MAXPATHLEN */
#include <sys/param.h>
/* Some systems (ie Dolphin 88K) do not define MAXPATHLEN even here */
#ifndef MAXPATHLEN
#define MAXPATHLEN 1024
#endif
/* X11 definitions */
#include <X11/Xos.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
/* Athena Widgets definitions */
#include <X11/Xaw/List.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Viewport.h>
#include <X11/Xaw/Scrollbar.h>
#include <X11/Xaw/Dialog.h>

/* ------------ LOCAL DEFINITIONS ---------------- */
#define LISTSIZE 30
#define TICKSPERSECOND 1000
#define SECONDSPERMINUTE 60
#define SAVEINTERVALMINUTES 10
#define TIMELEN 5		/* maxlen of hh:mm formatted times */
#define TICKPERIOD TICKSPERSECOND * SECONDSPERMINUTE
#define SAVEPERIOD TICKSPERSECOND * SECONDSPERMINUTE * SAVEINTERVALMINUTES
#define MINSPERDAY 60*24

/* All variables are in the main program */
extern String  *nameList;
extern String  *timeList;
extern int     *timeUsed;
extern Widget   toplevel;
extern Widget   list_widget;
extern Widget   timelist_widget;
extern Widget   commandBox;
extern char     timexdirectory[MAXPATHLEN];

extern int      incPlus;
extern int      incMinus;

extern XtAppContext appContext;

/* Cross-referred routines */
/* from timex.c */
extern char    *filenameprefix();
extern void     save();
extern int      currentProject();

/* from projectlist.c */
extern Widget   addButton();
extern int      makethelist();
extern void     initprojectlisteditor();
extern void     addDeadProject();
