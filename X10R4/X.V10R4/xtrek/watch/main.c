#include <X/Xlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <signal.h>
#include <setjmp.h>
#include "../defs.h"
#include "../struct.h"
#include "../data.h"

struct itimerval udt;
int redrawall;
extern Window w, mapw, statwin;

main(argc, argv)
int argc;
char **argv;
{
    int redraw();
    int team;
    int pno;
    char buf[80];
    register int i;

    if (argc != 3) {
	fprintf(stderr, "usage: %s monitor.name playerno\n", argv[0]);
	exit(1);
    }
    pno = atoi(argv[2]);

    newwin(argv[1]);
    findslot();
    redrawall = 1;
    XClear(w);
    enter(pno);
    signal(SIGALRM, redraw);
    udt.it_interval.tv_sec = 0;
    udt.it_interval.tv_usec = 200000;
    udt.it_value.tv_sec = 1;
    udt.it_value.tv_usec = 0;
    setitimer(ITIMER_REAL, &udt, 0);
    while (gets(buf) > 0) {
	pno = atoi(buf);
	enter(pno);
    }
}
