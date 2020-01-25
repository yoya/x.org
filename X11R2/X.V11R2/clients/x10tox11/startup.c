/*
 * Copyright 1987 by Tektronix Inc, Beaverton, Oregon.
 *
 *                         All Rights Reserved
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted,
 * provided that the above copyright notice appear in all copies and that
 * both that copyright notice and this permission notice appear in
 * supporting documentation, and that the name of Tektronix not be used
 * in advertising or publicity pertaining to distribution of the software
 * without specific, written prior permission.
 *
 *
 * TEKTRONIX DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
 * ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
 * TEKTRONIX BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
 * ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
 * ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 */
/* $Header: startup.c,v 4.2 88/02/15 09:52:27 jim Exp $ */
#include <sys/types.h>
#include <sys/signal.h>
#include <stdio.h>

#ifdef SYSV
#include <string.h>
#define index(s,c) strchr((s),(c))
#define rindex(s,c) strrchr((s),(c))
#else
#include <strings.h>
#endif

#define	True	1
#define	False	0

u_char	DoSync = False;	/* whether we enforce syncronization */
u_char	GrabServerAllowed = True;/* whether we ignore grabserver requests */
u_char	GrabMouseAllowed = True;/* whether we ignore grabmouse requests */
u_char	ServerGrabbed = False;	/* True if a grab is in progress */
u_char	Bigendian = True;	/* true if this is a big endian machine */
u_char	LockToggleMode = True;	/* true if X10 LockToggleMode is on */
u_char	LockStateOn = False;	/* true if ShiftLock pressed odd # of times */
char	*program;

main(argc, argv)
	int	argc;
	char	**argv;
{
	char	*p;
	int	i, whichbyte, Catch();

	whichbyte = 1;
	if (*(char *) &whichbyte)
		Bigendian = False;

	program = argv[0];

	signal (SIGINT, Catch);
	for(i = 1; i < argc; i++)
	{
		if (strcmp (argv[i], "-display") == 0 || 
		    strcmp (argv[i], "-d") == 0) {
		    if (++i >= argc) Usage ();
		    p = argv[i];
		} else if((p = index(argv[i], ':')) != NULL) {
			/* cared for by the X10 stuff */
		} else if (!SetFlags(argv[i])) {
			fprintf(stderr, "unknown option=\"%s\"\n", argv[i]);
			Usage();
		}
	}

	InitKeycodeMap();

	X10main(argc,argv);
}

Catch(sig)
	int	sig;
{
	if (sig == SIGINT)
		CommandInterp();
	signal (SIGINT, Catch);
}

SetFlags(string)
	char	*string;
{
	int	result = True;

	if (*string == '+' || *string == '-')
		result = SetDebug(string);
	else if (strcmp(string, "Sync") == 0) {
		DoSync = True;
		ChangeSync(True);
	}
	else if (strcmp(string, "NoSync") == 0) {
		DoSync = False;
		ChangeSync(False);
	}
	else if (strcmp(string, "AllowGrabServer") == 0)
		GrabServerAllowed = True;
	else if (strcmp(string, "NoGrabServer") == 0)
		GrabServerAllowed = False;
	else if (strcmp(string, "AllowGrabMouse") == 0)
		GrabMouseAllowed = True;
	else if (strcmp(string, "NoGrabMouse") == 0)
		GrabMouseAllowed = False;
	else if (strcmp(string, "AllowGrab") == 0)
		GrabServerAllowed = GrabMouseAllowed = True;
	else if (strcmp(string, "NoGrab") == 0)
		GrabServerAllowed = GrabMouseAllowed = False;
	else
		result = False;
	return(result);
}

Usage ()
{
	printf("usage: %s ", program);
	printf("[+all] [+/-x10protorequest] [+/-x11eventname]\n");
	printf("\t[+All] [+Events] +[EventsMasks] +[Bitmaps] [-g] [-s] ");
	printf("<display>\n");
	exit(1);
}

#ifndef lint
/*
 * XXX this pulls in the old resource manager's version of
 * XGetErrorText.
 */
static foo()
{
	XGetErrorText();
}
#endif /* lint */

