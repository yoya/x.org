/* ********************************************************************** *\
 *         Copyright IBM Corporation 1988,1989 - All Rights Reserved      *
 *        For full copyright information see:'andrew/config/COPYRITE'     *
\* ********************************************************************** */
/* cptres_search: search for a domain given a query type.
 * (Useful with Bind distributions predating bind 4.8.)
 * Returns the query-buffer in which answers are to be found.
 * Uses the same searching strategy as gethostbyname().
 * Thus, this routine can be used to bind host names or
 * search for MX records for a domain name.
 */

#include <andyenv.h>

#ifdef RESOLVER_ENV
#include <sys/param.h>
#include <netinet/in.h>
#include <ctype.h>
#include <stdio.h>
#include <errno.h>
#include <andrewos.h> /* strings.h */
#include <arpa/nameser.h>
#include <resolv.h>

extern int errno;

static int oneTry(dom, sfx, class, type, answer, anslen)
char *dom, *sfx;		/* domain name and suffix */
int class, type;		/* class and type of query */
char *answer;		/* buffer to put answer */
int anslen;		/* size of answer */
{
    char buf[PACKETSZ], tryname[2*MAXDNAME + 1];
    extern char *strcpy();
    int n;

    if (!(_res.options & RES_INIT))
	if (res_init() == -1)
	    return(-1);
#ifdef DEBUG
    if (_res.options & RES_DEBUG)
	printf("cptres_search()\n");
#endif /* DEBUG */
    /* append to form a new name to lookup */
    strcpy(tryname, dom);
    if (sfx != NULL) {
	strcat(tryname, ".");
	strcat(tryname, sfx);
    }
#ifdef DEBUG
    if (_res.options & RES_DEBUG)
	printf("cptres_search: trying name %s\n", tryname);
#endif /* DEBUG */
    n = res_mkquery(QUERY, tryname, class, type, (char *)NULL, 0, NULL, buf, sizeof(buf));

    if (n <= 0) {
#ifdef DEBUG
	if (_res.options & RES_DEBUG)
	    printf("cptres_search: mkquery failed\n");
#endif /* DEBUG */
	return (n);
    }
    n = res_send(buf, n, answer, anslen);
    if (n < 0) {
#ifdef DEBUG
	if (_res.options & RES_DEBUG)
	    printf("cptres_search: send error\n");
#endif /* DEBUG */
	return (n);
    }
    return(n);
}

static char *myhostalias(name)		/* same as hostalias() in gethostnamadr.c, sigh */
register char *name;
{
    register char *C1, *C2;
    FILE *fp;
    char *file, *getenv(), *strcpy(), *strncpy();
    char buf[BUFSIZ];
    static char abuf[MAXDNAME];

    file = getenv("HOSTALIASES");
    if (file == NULL || (fp = fopen(file, "r")) == NULL)
	return (NULL);
    buf[sizeof(buf) - 1] = '\0';
    while (fgets(buf, sizeof(buf), fp)) {
	for (C1 = buf; *C1 && !isspace(*C1); ++C1);
	if (!*C1 || *C1 == '\n')
	    break;
	if (!strncmp(buf, name, C1 - buf)) {
	    while (isspace(*++C1));
	    if (!*C1)
		break;
	    for (C2 = C1 + 1; *C2 && !isspace(*C2); ++C2);
	    abuf[sizeof(abuf) - 1] = *C2 = '\0';
	    (void)strncpy(abuf, C1, sizeof(abuf) - 1);
	    fclose(fp);
	    return (abuf);
	}
    }
    fclose(fp);
    return (NULL);
}

/* For each of a sequence of queries, form a query, send it, await a reply.
   Always use a QUERY operation, no extra data, no extra resource records.
   Return the size of the response, or -1 for errors. */

cptres_search(name, class, type, answer, anslen)
char *name;		/* domain name */
int class, type;		/* class and type of query */
char *answer;		/* buffer to put answer */
int anslen;		/* size of answer */
{
    register char *cp;
#ifdef RES_DNSRCH
    register char **domain;
#endif /* RES_DNSRCH */
    register HEADER *hp;
    extern char *strcpy();
    int n, NumDots;

    if (!(_res.options & RES_INIT))
	if (res_init() == -1)
	    return(-1);
#ifdef DEBUG
    if (_res.options & RES_DEBUG)
	printf("cptres_search()\n");
#endif /* DEBUG */
    errno = 0;
    for (cp = name, NumDots = 0; *cp; cp++)
	if (*cp == '.')
	    NumDots++;
    if ((NumDots && cp[-1] == '.') || (_res.options & RES_DEFNAMES) == 0) {
	if (cp[-1] == '.')
	    cp[-1] = 0;
	n = oneTry(name, (char *)NULL, class, type, answer, anslen);
	if (cp[-1] == 0)
	    cp[-1] = '.';
	return (n);
    }
    if (NumDots == 0 && (cp = myhostalias(name)))
	return (oneTry(cp, (char *)NULL, class, type, answer, anslen));
    n = -1;
#ifdef RES_DNSRCH
    /* Try all in the domain name search list if we're running a bind recent enough to support it. Otherwise just search the default domain. */
    if (_res.options & RES_DNSRCH)
	for (domain = _res.dnsrch; *domain; domain++) {
	    n = oneTry(name, *domain, class, type, answer, anslen);
	    if (n <= 0) return(n);
	    hp = (HEADER *) answer;
	    if (hp->rcode != NXDOMAIN || ! hp->aa) return (n);
	}
#else /* RES_DNSRCH */
    n = oneTry(name, _res.defdname, class, type, answer, anslen);
    if (n <= 0) return(n);
    hp = (HEADER *) answer;
    if (hp->rcode != NXDOMAIN || ! hp->aa) return (n);
#endif /* RES_DNSRCH */
    if (NumDots == 0) return (n);
    return (oneTry(name, (char *)NULL, class, type, answer, anslen));
}
#endif /* RESOLVER_ENV */
