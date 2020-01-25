#ifndef lint
static char rcsID[] =
	"$Id: xrunclient.c,v 6.1 94/05/18 14:59:15 dd Exp $";
#endif

/*
 * Copyright (c) 1994 David DiGiacomo
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

/*
 * xrunclient: launch X client and wait until it maps a window
 */

#include <X11/Xos.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>

#ifndef WIFSTOPPED
#define	WIFSTOPPED(s)	(((s) & 255) = 127)
#endif
#ifndef	WIFEXITED
#define	WIFEXITED(s)	(((s) & 255) = 0)
#endif
#ifndef	WEXITSTATUS
#define	WEXITSTATUS(s)	(((s) >> 8) & 255)
#endif
#ifndef	WTERMSIG
#define	WTERMSIG(s)	((s) & 127)
#endif

static char *progname;
static char nullstr[] = "";

static void setprogname();
static void usage();
static void error();
static void setdisplay();
static void catch();

extern char *getenv();
extern char *malloc();
extern char **environ;
extern int errno;

main(argc, argv)
	int argc;
	char *argv[];
{
	int i;
	char *displayname = 0;
	int timeout = 15;
	int cmd;
	Display *dpy;
	Window rootwin;
	long event_mask;
	Window qroot, qparent, *qchildren;
	unsigned int nchildren;
	int ochildren;
	int pid;
	XEvent event;

	setprogname(argv[0]);

	for (i = 1; i < argc; i++) {
		if (argv[i][0] == '-')
			switch (argv[i][1]) {
			case 'd':
			case 't':
				if (i + 1 == argc) {
					error("%s: missing argument for %s",
						progname, argv[i], nullstr,
						0, 0);
					usage(1);
				}
				if (argv[i][1] == 'd')
					displayname = argv[++i];
				else {
					timeout = atoi(argv[++i]);
					if (timeout < 0)
						timeout = 0;
				}
				continue;
			case 'h':
				usage(0);
				break;
			case 0:
			case '-':
				i++;
				break;
			default:
				usage(1);
				break;
			}
		break;
	}

	cmd = i;
	
	if (cmd >= argc)
		usage(1);

	if (displayname)
		setdisplay(displayname);
	else
		displayname = getenv("DISPLAY");

	if (!(dpy = XOpenDisplay(displayname)))
		error("%s: cannot open display %s",
			progname, displayname, nullstr, 0, 1);

	rootwin = DefaultRootWindow(dpy);

	event_mask = SubstructureNotifyMask;
	XSelectInput(dpy, rootwin, event_mask);

	/*
	 * wait until number of windows stabilizes
	 */
	for (ochildren = -1, nchildren = 0;
		ochildren != nchildren;
		ochildren = nchildren) {
		if (!XQueryTree(dpy, rootwin, &qroot, &qparent, &qchildren,
			&nchildren))
			error("%s: XQueryTree error",
				progname, nullstr, nullstr, 0, 1);
		if (qchildren)
			XFree(qchildren);
	}

	/*
	 * start client program
	 */
	(void) signal(SIGCHLD, catch);
	if ((pid = fork()) == 0) {
		/* child */
		(void) signal(SIGINT, SIG_IGN);
		(void) signal(SIGQUIT, SIG_IGN);
		(void) signal(SIGTSTP, SIG_IGN);
		(void) execvp(argv[cmd], &argv[cmd]);
		error("%s: cannot execute cmd \"%s\": ",
			progname, argv[cmd], nullstr, 1, 128);
	}

	if (pid < 0)
		error("%s: fork error: ", progname, nullstr, nullstr, 1, 1);

	/*
	 * wait for window creation or timeout
	 */
	(void) signal(SIGALRM, catch);
	(void) alarm(timeout);

	do {
		XNextEvent(dpy, &event);
	} while (event.type != CreateNotify);

	exit(0);
	/* NOTREACHED */
}

static void
setprogname(name)
	char *name;
{
	char c;

	progname = name;

	while (c = *name++)
		if (c == '/')
			progname = name;
}

static void
error(fmt, s1, s2, s3, perr, excode)
	char *fmt;
	char *s1, *s2, *s3;
	int perr;
	int excode;
{
	int save_errno = errno;

	fprintf(stderr, fmt, s1, s2, s3);
	if (perr) {
		fflush(stderr);
		errno = save_errno;
		perror(nullstr);
	}
	else
		fprintf(stderr, "\n");

	if (excode)
		exit(excode);
}

static void
usage(excode)
	int excode;
{
	error(
"usage: %s [-help] [-display display] [-timeout seconds] command arg...",
		progname, nullstr, nullstr, 0, 0);
	exit(excode);
}

static void
setdisplay(display)
	char *display;
{
	int envsize;
	char *p;
	char **oldenv, **newenv;
	char **olddisp = 0, *newdisp;
	static char predisp[] = "DISPLAY=";

	envsize = 2;
	for (oldenv = environ; *oldenv; oldenv++)
		envsize++;

	newenv = (char **) malloc(envsize * sizeof (char *));
	newdisp = malloc(strlen(display) + sizeof predisp);
	if (newenv == 0 || newdisp == 0)
		error("%s: malloc failure", progname, nullstr, nullstr, 0, 1);

	oldenv = environ;
	environ = newenv;
	sprintf(newdisp, "%s%s", predisp, display);

	olddisp = 0;
	for (; p = *oldenv; oldenv++) {
		if (strncmp(p, predisp, sizeof predisp - 1) == 0)
			olddisp = newenv;
		*newenv++ = p;
	}
	if (olddisp == 0)
		olddisp = newenv++;
	*olddisp = newdisp;
	*newenv++ = 0;
}

static void
catch(sig)
	int sig;
{
	int status;
	int excode;

	if (sig == SIGALRM)
		excode = 0;
	else if (wait(&status) < 0)
		error("%s: wait error", progname, nullstr, nullstr, 1, 1);
	/* child is stopped, wait for something else to happen */
	else if (WIFSTOPPED(status))
		return;
	/* child already finished */
	else if (WIFEXITED(status))
		excode = WEXITSTATUS(status);
	/* child caught signal */
	else
		excode = WTERMSIG(status) + 128;

	exit(excode);
}
