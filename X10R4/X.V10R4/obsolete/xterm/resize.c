#include <X/mit-copyright.h>

/* Copyright    Massachusetts Institute of Technology    1984	*/

/* resize.c */

#include <stdio.h>
#include <sgtty.h>
#include <strings.h>
#include <sys/ioctl.h>

#ifndef lint
static char *rcsid_resize_c = "$Header: resize.c,v 10.6 86/07/30 15:01:11 wesommer Exp $";
#endif

char *myname;

char *strindex (), *index (), *rindex();

main (argc, argv)
char **argv;
/*
   resets termcap string to reflect current screen size
 */
{
	int rows, cols;
	char *size = "\0337\033[r\033[999;999H\033[6n";
	struct sgttyb sg, sgorig;
	char termcap [1024];
	char newtc [1024];
	char *term = "";
	register char *ptr, *env;
	int csh;
	char *getenv();
#ifdef TIOCSWINSZ
	struct winsize ws;
#else
#ifdef	TIOCSSIZE
	struct ttysize ts;
#endif	TIOCSSIZE
#endif
	tgetent (termcap, "xterm");

	if(ptr = rindex(myname = argv[0], '/'))
		myname = ptr + 1;
	if((env = getenv("TERMCAP")) && *env)
		strcpy(termcap, env);
	else {
		if(!(env = getenv("TERM")) || !*env) {
			term = env = "xterm";
		}
		if(tgetent (termcap, env) <= 0) {
			fprintf(stderr, "%s: Can't get entry \"%s\"\n",
			 myname, env);
			exit(1);
		}
	}

 	ioctl ( 0, TIOCGETP, &sgorig);
	sg = sgorig;
	sg.sg_flags |= RAW;
	ioctl ( 0, TIOCSETP, &sg);

	write ( 0, size, strlen (size));
	scanf ("\033[%d;%dR", &rows, &cols);
	write ( 0, "\0338", 2);

	ioctl ( 0, TIOCSETP, &sgorig);

	/* update termcap string */
	/* first do columns */
	if ((ptr = strindex (termcap, "co#")) == NULL) {
		fprintf(stderr, "%s: No `co#'\n", myname);
		exit (1);
	}
	strncpy (newtc, termcap, ptr - termcap + 3);
	sprintf (newtc + strlen (newtc), "%d", cols);
	ptr = index (ptr, ':');
	strcat (newtc, ptr);

	/* now do lines */
	if ((ptr = strindex (newtc, "li#")) == NULL) {
		fprintf(stderr, "%s: No `li#'\n", myname);
		exit (1);
	}
	strncpy (termcap, newtc, ptr - newtc + 3);
	sprintf (termcap + ((int) ptr - (int) newtc + 3), "%d", rows);
	ptr = index (ptr, ':');
	strcat (termcap, ptr);

 	csh = 0;
 	ptr = env = getenv("SHELL");
 	if (env != NULL) {
		while (*ptr++ != '\0')
 			;
 		if (ptr - env >= 4 &&
		    ptr[-2] == 'h' && ptr[-3] == 's' && ptr[-4] == 'c')
 			csh = 1;
	}

	if (csh) {
		printf("set noglob;\n");
	if (term != NULL && *term != '\0')
			pqstr("setenv TERM ", term, ";\n");
		pqstr("setenv TERMCAP ", termcap, ";\n");
	printf("unset noglob;\n");
	} else {
		printf("export TERM TERMCAP;\n");
		if (term != NULL && *term != '\0')
			pqstr("TERM=", term, ";\n");
		pqstr("TERMCAP=", termcap, ";\n");
	}

#ifdef TIOCGWINSZ
	/* finally, set the tty's window size */
	if (ioctl (0, TIOCGWINSZ, &ws) != -1) {
	    /* we don't have any way of directly finding out
	       the current height & width of the window in pixels.  We try
	       our best by computing the font height and width from the "old"
	       struct winsize values, and multiplying by these ratios...*/
	    if (ws.ws_xpixel != 0 && ws.ws_col != 0)
	        ws.ws_xpixel = cols*(ws.ws_xpixel/ws.ws_col);
	    if (ws.ws_ypixel != 0 && ws.ws_row != 0)
	        ws.ws_ypixel = rows*(ws.ws_ypixel/ws.ws_row);
	    ws.ws_row = rows;
	    ws.ws_col = cols;
	    ioctl (0, TIOCSWINSZ, &ws);
	    }
#else
#ifdef	TIOCSSIZE
	ts.ts_lines = rows;
	ts.ts_cols = cols;
	ioctl(0, TIOCSSIZE, &ts);
#endif	/* TIOCSSIZE */
#endif
	exit(0);
}

char *strindex (s1, s2)
/*
   returns a pointer to the first occurrence of s2 in s1, or NULL if there are
   none.
 */
char *s1, *s2;
{
	char *s3;

	while ((s3 = index (s1, *s2)) != NULL)
	{
		if (strncmp (s3, s2, strlen (s2)) == 0) return (s3);
		s1 = ++s3;
	}
	return (NULL);
}

/* print quoted string */
pqstr(header, s, trailer)
	register char *header;
	register char *s;
	register char *trailer;
{
	if (header != NULL && *header != '\0')
		printf("%s", header);

	putchar('\'');

	for (/* void */; *s != '\0'; s++) {
		if (*s == '\'')
			printf("'\\''");

		putchar(*s);
	}

	putchar('\'');

	if (trailer != NULL && *trailer != '\0')
		printf("%s", trailer);
}
