#ifndef lint
static char *rcsid_scores_c = "$Header: scores.c,v 10.1 86/11/30 15:28:38 jg Rel $";
#endif	lint
/* Copyright (c) 1986 	Chris Guthrie */

#include <stdio.h>
#include <sys/types.h>
#include <sys/file.h>
#include <pwd.h>
#include "defs.h"
#include "struct.h"

#define MAXENT 2000

struct stats old;

main()
{
    int fd;
    register int i;
    struct passwd *buf;

    fd = open(SCOREFILE, O_RDONLY, 0777);
    if (fd < 0) {
	perror(SCOREFILE);
	exit(1);
    }
    fprintf(stdout, "Name      rtime   cpu  kills maxkills losses entries conqs coups  torps phasers abomb aship planets gens ratio Name\n");

    for (i = 0; i < MAXENT; i++) {
	if (read(fd, &old, sizeof(struct stats)) == 0)
	    break;
	if (old.st_entries > 0) {
	    buf = getpwuid(i);
	    if (buf <= 0)
		continue;
	    fprintf(stdout, "%-8s %6d %5d %6.2f    %5.2f %6d %7d %5d %5d %6d %7d %5d %5d %7d %4d %5.3f %-8s\n",
		buf->pw_name,
		old.st_time,
		old.st_cpu,
		old.st_kills,
		old.st_maxkills,
		old.st_losses,
		old.st_entries,
		old.st_conqs,
		old.st_coups,
		old.st_torps,
		old.st_phasers,
		old.st_armsbomb,
		old.st_armsship,
		old.st_planets,
		old.st_genocides,
		(old.st_losses ? old.st_kills / old.st_losses : 0.0),
		buf->pw_name);

	}
    }
}
