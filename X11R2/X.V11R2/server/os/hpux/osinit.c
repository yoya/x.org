/***********************************************************
Copyright 1987 by Digital Equipment Corporation, Maynard, Massachusetts,
and the Massachusetts Institute of Technology, Cambridge, Massachusetts.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the names of Digital or MIT not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.  

DIGITAL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/
/* $Header: osinit.c,v 1.3 88/02/22 10:45:02 rws Exp $ */
#include "os.h"
#include "opaque.h"
#undef NULL
#include <dbm.h>
#undef NULL
#include <stdio.h>

#ifndef ADMPATH
#define ADMPATH "/usr/adm/X%dmsgs"
#endif

int	havergb = 0;
extern char *display;

OsInit()
{
    static Bool been_here = FALSE;
    char fname[32];

    /* hack test to decide where to log errors */

    if (!been_here) {
	if (write (2, fname, 0)) 
	{
	    long t; 
	    char *ctime();
	    fclose(stdin);
	    fclose(stdout);
	    sprintf (fname, ADMPATH, display);
	    freopen (fname, "a+", stderr);
	    setvbuf(stderr, (char *)NULL, _IOLBF, BUFSIZ);
	    time (&t);
	    fprintf (stderr, "start %s", ctime(&t));
	}

	if (getpgrp (0) == 0)
	    setpgrp (0, getpid ());

	been_here = TRUE;
    }

    if(!havergb)
        if(dbminit (rgbPath) == 0)
	    havergb = 1;
        else
	    ErrorF( "Couldn't open RGB_DB '%s'\n", rgbPath );
}
