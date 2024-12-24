/*
 * regtty
 * regtty ttyname1 ttyname2 ttyname3 ...
 */

#include <sys/time.h>

#include <utmp.h>
#include <pwd.h>

#define	SCPYN(a, b)	strncpy(a, b, sizeof(a))
#define SCMPN(a, b)	strncmp(a, b, sizeof(a))

struct	passwd *pw;
struct	utmp utmp;
#define ROOT 0

char	*ttyname();
char	utmpf[] = "/etc/utmp";

main(argc, argv)
	char *argv[];
{
	struct passwd *getpwuid();
	char *ttyn;
	register f, t;
	int i;
	int debug;

	pw = getpwuid(getuid());
	if (pw == 0) exit(1);

	if (argc > 1) {
		for (i = 1; i < argc; i++) {
		    rmut(argv[i]);
		}
		exit(0);
	}
	ttyn = ttyname(0);
	if (ttyn==(char *)0)
		ttyn = "/dev/tty??";
	SCPYN(utmp.ut_name, "");
	SCPYN(utmp.ut_line, rindex(ttyn, '/')+1);
	SCPYN(utmp.ut_host, getenv("VS100"));
	SCPYN(utmp.ut_name, pw->pw_name);
	time(&utmp.ut_time);
	t = ttyslot();
	if (t > 0 && (f = open(utmpf, 1)) >= 0) {
		lseek(f, (long)(t*sizeof(utmp)), 0);
		debug = write(f, (char *)&utmp, sizeof(utmp));
		close(f);
	}
	chown(ttyn, pw->pw_uid, -1);
/*	chmod(ttyn, 0622);  always is 622 */
	exit(0);
}

rmut(line)
	char *line;
{
	register f;
	char device[15];

	f = open(utmpf, 2);
	if (f >= 0) {
		while(read(f, (char *)&utmp, sizeof(utmp)) == sizeof(utmp)) {
			if ((SCMPN(utmp.ut_line, line) != 0) ||
			    (SCMPN(utmp.ut_name, pw->pw_name) != 0))
				continue;
			lseek(f, -(long)sizeof(utmp), 1);
			SCPYN(utmp.ut_name, "");
			SCPYN(utmp.ut_host, "");
			time(&utmp.ut_time);
			write(f, (char *)&utmp, sizeof(utmp));
			SCPYN(device, "/dev/");
			strncpy(device+5, line, sizeof(device)-5);
			chown(device, ROOT, -1);
/*			chmod(device, 0666);  always is 622 */
		}
		close(f);
	}
}


/* end of regtty.c */
