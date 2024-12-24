#ifndef lint
static char *rcsid_startdaemon_c = "$Header: startdaemon.c,v 10.1 86/11/30 15:28:58 jg Rel $";
#endif	lint
/* Copyright (c) 1986 	Chris Guthrie */


#include <stdio.h>
#include "defs.h"
#include "struct.h"

startdaemon()
{
    int i;

    i = vfork();
    if (i == 0) {
	for (i = 3; i < _NFILE; i++)
	    close(i);
	execl(DAEMON, "daemon", 0);
	perror(DAEMON);
	fprintf(stderr, "Couldn't start daemon!!!\n");
	_exit(1);
    }
}
