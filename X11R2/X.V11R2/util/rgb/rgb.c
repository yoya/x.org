/* Copyright 1985, Massachusetts Institute of Technology */

/* reads from standard input lines of the form:
	red green blue name
   where red/green/blue are decimal values, and inserts them in a database.
 */
#ifndef lint
static char *rcsid_rgb_c = "$Header: rgb.c,v 11.4 88/02/13 20:54:13 rws Exp $";
#endif

#include <dbm.h>
#undef NULL
#include <stdio.h>
#include <sys/file.h>
#ifdef SYSV
#include <fcntl.h>
#endif /* SYSV */
#include "rgb.h"
#include "site.h"
#include <ctype.h>

main(argc, argv)
    int argc;
    char **argv;
{
    char *dbname;
    char line[512];
    int red, green, blue;
    RGB rgb;
    datum key, content;
    char name[512];
    int items;
    int lineno;
    int i, n;

    if (argc == 2)
	dbname = argv[1];
    else
	dbname = RGB_DB;
    strcpy (name, dbname);
    strcat (name, ".dir");
    close (open (name, O_WRONLY|O_CREAT, 0666));
    strcpy (name, dbname);
    strcat (name, ".pag");
    close (open (name, O_WRONLY|O_CREAT, 0666));
    if (dbminit (dbname))
	exit (1);
    key.dptr = name;
    content.dptr = (char *) &rgb;
    content.dsize = sizeof (rgb);
    lineno = 0;
    while (fgets (line, sizeof (line), stdin)) {
	lineno++;
	items = sscanf (line, "%d %d %d %[^\n]\n", &red, &green, &blue, name);
	if (items != 4) {
	    fprintf (stderr, "syntax error on line %d\n", lineno);
	    fflush (stderr);
	    continue;
	}
	if (red < 0 || red > 0xff ||
	    green < 0 || green > 0xff ||
	    blue < 0 || blue > 0xff) {
	    fprintf (stderr, "value for %s out of range\n", name);
	    fflush (stderr);
	    continue;
	}
	n = strlen (name);
	for (i = 0; i < n; i++) {
	    if (isupper (name[i]))
		name[i] = tolower (name[i]);
	}
	key.dsize = n;
	rgb.red = red << 8;
	rgb.green = green << 8;
	rgb.blue = blue << 8;
	if (store (key, content)) {
	    fprintf (stderr, "store of %s failed\n", name);
	    fflush (stderr);
	}
    }
}
