/*
 * Copyright (C) 1990 Regents of the University of California.
 *
 * Permission to use, copy, modify, distribute, and sell this software and
 * its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and that
 * both that copyright notice and this permission notice appear in
 * supporting documentation, and that the name of the University of
 * California not be used in advertising or publicity pertaining to
 * distribution of the software without specific, written prior
 * permission.  the University of California makes no representations
 * about the suitability of this software for any purpose.  It is provided
 * "as is" without express or implied warranty.
 */

# include <stdio.h>

# include <X11/Intrinsic.h>
# include <X11/StringDefs.h>

# include "debug.h"
# include "cdrom_globs.h"

void
main(argc, argv)
	int		argc;
	char		**argv;
{
	static void	chk_debug();
	Widget		top_form;

	chk_debug(argc, argv);

	if (cdrom_open() == -1)
		exit(1); /* perhaps a bit drastic */

	top_form = top_setup(argc, argv);

	logo_setup(top_form);

	button_setup(top_form);

	top_start();

	XtAppMainLoop(appc);

	exit(0);
}

static void
chk_debug(argc, argv)
	int	argc;
	char	**argv;
{
	int	i;

	/* ugly hack */
	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-debug") == 0) {
			debug = True;
			break;
		}
	}
}

void
usage() {
	(void) fprintf(stderr, "usage: xcdplayer [-debug]\n");

	exit(1);
}
