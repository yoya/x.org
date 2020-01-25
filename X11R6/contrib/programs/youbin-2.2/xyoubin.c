/*
 * Program:     $RCSfile: xyoubin.c,v $  $Revision: 4.1 $
 *
 * Purpose:     X Window System application program that becomes a client for
 *              the Internet "youbin" service.
 *
 * Usage:       See global variable usage.
 *
 * Author:      H.Agusa      agusa@nuie.nagoya-u.ac.jp
 *              S.Yamamoto   yamamoto@nuie.nagoya-u.ac.jp
 *              T.Hamaguchi  hamaguti@agusa.nuie.nagoya-u.ac.jp
 *
 * Date:        1993/08/05
 * Modified:    $Date: 1994/05/31 08:44:02 $
 *
 * Copyright:   K.Agusa and S.Yamamoto  1993 - 94
 *
 * The X Consortium, and any party obtaining a copy of these files from
 * the X Consortium, directly or indirectly, is granted, free of charge,
 * a full and unrestricted irrevocable, world-wide, paid up, royalty-free,
 * nonexclusive right and license to deal in this software and documentation
 * files (the "Software"), including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons who receive copies from any such
 * party to do so. This license includes without limitation a license to do
 * the foregoing actions under any patents of the party supplying this
 * software to the X Consortium.
 */

#include <sys/param.h>
#include <sys/time.h>                   /* For wait3(). */
#include <sys/resource.h>               /* For wait3(). */
#include <netinet/in.h>                 /* For struct sockaddr_in. */
#include <signal.h>
#include <stdio.h>

/* #include <X11/Xos.h> */
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>             /* Get standard string definitions. */
#include <X11/Xaw/Command.h>    
#include <X11/Xaw/Label.h>    
#include <X11/Xaw/Cardinals.h>
#include <X11/Xutil.h>

#ifdef USE_XPM
#include <X11/IntrinsicP.h>
#include <X11/CoreP.h>
#define R_PIXMAP    XtRPixmap           /* Use Pixmap. */
#else /* not USE_XPM */
#define R_PIXMAP    XtRBitmap           /* Use Bitmap. */
#endif /* USE_XPM */

#include "youbin.h"

/* 
 * Compatibility of non-blocking wait.
 *
 * USE_POSIX_WAIT:  waitpid() is available.
 * Otherwise:       BSD's wait3() mechanism available.
 */

#ifdef _POSIX_SOURCE
#define USE_POSIX_WAIT
#endif

#if (defined(SYSV) || defined(sun)) && !defined(USE_POSIX_WAIT)
#define USE_POSIX_WAIT
#endif

#if defined(mips) && defined(SYSV)      /* Only MIPS. */
#undef USE_POSIX_WAIT
#define wait3(status, options, rusage)  wait2((status), (options))
#endif

#if defined(USE_POSIX_WAIT)
#include <sys/types.h>                  /* For waitpid(). */
#define WAITPID(pid, status, options)   waitpid((pid), (status), (options))
#else /* !defined(USE_POSIX_WAIT) */
/* Note, the first argument of waitpid() which means pid is is always -1. */
#define WAITPID(pid, status, options)   wait3((status), (options), NULL)
#endif /* !defined(USE_POSIX_WAIT) */

#include <sys/wait.h>

/* 
 * Compatibility of process id.
 */

#if !defined(USE_POSIX_WAIT)
typedef int         pid_t;
#endif /* !defined(USE_POSIX_WAIT) */

#ifndef lint
static char rcsid[] =
    "$Id: xyoubin.c,v 4.1 1994/05/31 08:44:02 yamamoto Exp $";
#endif /* not lint */

static char *usage[] = {
    "NAME",
    "    xyoubin - X window system client for the Internet \"youbin\" service",
    "SYNOPSIS",
    "    xyoubin [-empty pixmap][-flip/-noflip][-full pixmap][-help][-pipe]",
    "            [-server host][-sound file][-volume %][-toolkitoption]",
    "OPTIONS",
    "    -empty pixmap  Pixmap to be shown when no new mail is present.",
    "    -flip/-nofilp  Specifies whether or not the image should be inverted.",
    "    -full pixmap   Pixmap to be shown when new mail has arrived.",
    "    -help          Help: print out the usage.",
    "    -pipe          Executed by other program (don't exec child process).",
    "    -server host   Server host name.",
    "    -sound file    Sound file (SunOS only).",
    "    -volume %      Specifies how loud the bell should be rung.",
    "AUTHORS",
    "    K.Agusa      agusa@nuie.nagoya-u.ac.jp",
    "    S.Yamamoto   yamamoto@nuie.nagoya-u.ac.jp",
    "    T.Hamaguchi  hamaguti@agusa.nuie.nagoya-u.ac.jp",
    "    H.Ariga      ariga@agusa.nuie.nagoya-u.ac.jp",
    NULL
};

/*
 * Constants.
 */

#ifndef BINDIR
#define BINDIR              "."         /* Default path of child program. */
#endif /* not BINDIR */

#define CHILD_PROGRAM_NAME  "youbin"    /* Default child program name. */

/*
 * Resources.
 */

String  fallback_resources[] = { 
    "*emptyPixmap:          flagdown",
    "*fullPixmap:           flagup",
    "*internalHeight:       0",
    "*internalWidth:        0",
    NULL
};

static XrmOptionDescRec     options[] = {
    {"-empty",  "*emptyPixmap",     XrmoptionSepArg,    NULL},
    {"-flip",   "*flip",            XrmoptionNoArg,     "TRUE"},
    {"-full",   "*fullPixmap",      XrmoptionSepArg,    NULL},
    {"-noflip", "*flip",            XrmoptionNoArg,     "FALSE"},
    {"-pipe",   "*pipe",            XrmoptionNoArg,     "TRUE"},
    {"-server", "*server",          XrmoptionSepArg,    NULL},
    {"-sound",  "*sound",           XrmoptionSepArg,    NULL},
    {"-volume", "*volume",          XrmoptionSepArg,    NULL}
};

typedef struct {
    Pixmap          emptyPixmap;
    Boolean         flip;
    Pixmap          fullPixmap;
    Boolean         pipe;
    String          server;
    String          sound;
    int             volume;
} AppData, *AppDataP;

static XtResource       resources[] = {
    {"emptyPixmap", XtCPixmap,      R_PIXMAP,   sizeof(Pixmap),
        XtOffset(AppDataP, emptyPixmap),     XtRPixmap,  NULL},
    {"flip",        "Flip",         XtRBoolean, sizeof(Boolean),
        XtOffset(AppDataP, flip),           XtRString,  "TRUE"},
    {"fullPixmap",  XtCPixmap,      R_PIXMAP,   sizeof(Pixmap),
        XtOffset(AppDataP, fullPixmap),     XtRPixmap,  NULL},
    {"pipe",        "Pipe",         XtRBoolean, sizeof(Boolean),
        XtOffset(AppDataP, pipe),           XtRString,  "FALSE"},
    {"server",      "Server",       XtRString,  sizeof(String),
        XtOffset(AppDataP, server),         XtRString,  NULL},
    {"sound",       "Sound",        XtRString,  sizeof(String),
        XtOffset(AppDataP, sound),          XtRString,  NULL},
    {"volume",      "Volume",       XtRInt,     sizeof(int),
        XtOffset(AppDataP, volume),         XtRString,  "33"}
};

/*
 * Global variables.
 */

char                *prog_name;                     /* This program name. */

AppData             app_data;

Widget              toplevel;

Widget              full;
Widget              empty;
Widget              managed;    /* One of full or empty widget is managed. */ 

char                child_process[MAXPATHLEN + 1];

/* Mode flag. */
int                 debug_mode = OFF;

/*
 * Local functions.
 */

static void         read_from_child();
static void         set_empty();
int                 sound();

static void         sig_cld();

/*
 * Converter from XtRString to XtRPixmap.
 */

#ifdef USE_XPM
void                cvt_str_to_pixmap();

static XtConvertArgRec  xpmScreenConvertArg[] = {
    {XtWidgetBaseOffset,    (XtPointer)XtOffsetOf(CoreRec, core.screen),
        sizeof(Screen *)}
};
#endif /* USE_XPM */

int
main(argc, argv)
int     argc;
char    **argv;
{
    FILE            *pfile;     /* For popen(). */
    char            server_name[MAXHOSTNAMELEN + 1];
    XtAppContext    app_con;
    Pixel           bg, fg;

    if ((prog_name = strrchr(argv[0], '/')) != NULL) {
        prog_name++;
    } else {
        prog_name = argv[0];
    }

    toplevel = XtAppInitialize(&app_con, "XYoubin",
                               options, XtNumber(options),
                               &argc, argv,
                               fallback_resources, NULL, ZERO);
    if (argc != 1) {
        print_usage(usage);
        exit(EXIT_FAILURE);
    }
#ifdef USE_XPM
    /* Add converter from XtRString to XtRPixmap. */
    XtAddConverter(XtRString, XtRPixmap, cvt_str_to_pixmap,
                   xpmScreenConvertArg, XtNumber(xpmScreenConvertArg));
#endif /* USE_XPM */

    XtGetApplicationResources(toplevel, &app_data, resources,
                              XtNumber(resources), NULL, 0);
    if (app_data.server == NULL) {
        gethostname(server_name, sizeof(server_name));
        app_data.server = server_name;
    }

    /* Create empty widget. */
    managed = empty = XtCreateManagedWidget("Empty", labelWidgetClass,
                                            toplevel, NULL, ZERO);
    if (app_data.emptyPixmap) {
        XtVaSetValues(empty, XtNbitmap, app_data.emptyPixmap, NULL);
    }

    /* Create full widget. */
    full = XtVaCreateWidget("Full", commandWidgetClass, toplevel, NULL, ZERO);
    if (app_data.fullPixmap) {
        XtVaSetValues(full, XtNbitmap, app_data.fullPixmap, NULL);
    }

    /* Set callback function. */
    XtAddCallback(full, XtNcallback, set_empty, NULL);

    /* Do flip. */
    if (app_data.flip) {  
        XtVaGetValues(full, XtNbackground, &bg, XtNforeground, &fg, NULL);
        XtVaSetValues(full, XtNbackground, fg, XtNforeground, bg, NULL);
    }
    
    if (!app_data.pipe) {
        signal(SIGCLD, sig_cld);
        sprintf(child_process, "exec %s/%s -s %s",
                BINDIR, CHILD_PROGRAM_NAME, app_data.server);
        if ((pfile = popen(child_process, "r")) == NULL) {
            fprintf(stderr, "%s: Can not execute child process: %s\n",
                    prog_name, child_process);
            perror("popen");
            exit(EXIT_FAILURE);
        }
        XtAppAddInput(app_con, fileno(pfile), (XtPointer)XtInputReadMask,
                      read_from_child, NULL);
    } else {
        XtAppAddInput(app_con, fileno(stdin), (XtPointer)XtInputReadMask,
                      read_from_child, NULL);
    }

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_con);
}

static void
read_from_child(w, fid, id)
Widget      w;
int         *fid;
XtInputId   *id;
{
    char        buff[MESS_LEN + 1];
    int         len;
    char        *cp;

    int         mail_size;
    static int  saved_mail_size = 0;
    
    if ((len = read(*fid, buff, MESS_LEN)) == 0) {
        /* Error occurs in child process. */
        fprintf(stderr, "%s: Child process killed: %s\n",
                prog_name, child_process);
        exit(EXIT_FAILURE);
    } else if (len == -1) {                         /* Unknown error. */
        fprintf(stderr, "%s: Can not read from child process: %s\n",
                prog_name, child_process);
        exit(EXIT_FAILURE);
    }
    buff[len] = '\0' ;                              /* Zero termination. */
    debug("%s: read_from_child(): \"%s\"\n", prog_name, buff);

    mail_size = (int)strtol(buff, &cp, 10);
    if (*cp != ' ') {
        fprintf(stderr, "%s: Invalid message from child process: %s\n",
                prog_name, buff);
        return;
    }
    
    if (mail_size == 0) {                           /* Mail spool is empty. */
        if (managed == full) {
            XtUnmanageChild(full);
            XtManageChild(empty);
            managed = empty;
        }
    } else if (mail_size != saved_mail_size) {      /* Spool changed. */
        if (managed == empty) {
            XtUnmanageChild(empty);
            XtManageChild(full);
            managed = full;
        }
        if (!app_data.sound || sound(app_data.sound)) {
            XBell(XtDisplay(toplevel), app_data.volume);
        }
    }
    saved_mail_size = mail_size;
}

static void
set_empty(widget, client_data, call_data)
Widget     widget;
void       *client_data, *call_data;
{
    if (managed == full) {
        XtUnmanageChild(full);
        XtManageChild(empty);
        managed = empty;
    }
}

static void
sig_cld()
{
    pid_t       pid;

#ifdef SYSV
    int         status;
#else /* SYSV */
    union wait  status;
#endif /* not SYSV */

    pid = WAITPID(-1, &status, WNOHANG);
    fprintf(stderr, "%s: Catch signal CLD\n", prog_name);

    if (0 < pid) {
        exit(EXIT_FAILURE);                 /* Should print child status? */
    }
    signal(SIGCLD, sig_cld);
}
