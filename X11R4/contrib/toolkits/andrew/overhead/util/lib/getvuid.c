/* ********************************************************************** *\
 *         Copyright IBM Corporation 1988,1989 - All Rights Reserved      *
 *        For full copyright information see:'andrew/config/COPYRITE'     *
\* ********************************************************************** */
/* ************************************************************ *\
	getvuid.c
	getvuid() (``get Vice UID'') gets the VUID associated with the primary authentication.
\* ************************************************************ */
/* $Header: getvuid.c,v 1.1 89/12/05 21:17:03 xguest Exp $ */
/* $Source: /xsrc/contrib/toolkits/andrew/overhead/util/lib/RCS/getvuid.c,v $ */

#ifndef lint
static char *rcsid = "$Header: getvuid.c,v 1.1 89/12/05 21:17:03 xguest Exp $";
#endif /* lint */

#include <andyenv.h>
#include <stdio.h>
#include <errno.h>
#include <andrewos.h>	/* types.h */
#include <svcconf.h>
#include <util.h>

extern int errno;

#ifdef WHITEPAGES_ENV
int getvuid()
{/* Return the Vice UID of the current process, or -1 with errno set */
    struct CellAuth *ca;
    int rc;

    CheckServiceConfiguration();
    if (AMS_ViceIsRunning) {
	rc = FindPrimaryCell(&ca);
	if (rc == 0 && ca != NULL) {
	    if (ca->ViceID < 0) FillInCell(ca);
	    if (ca->ViceID >= 0) return ca->ViceID;	/* ViceId in the home cell */
	}
	errno = EINVAL;
	return -1;
    } else {
	return getuid();
    }
}
#endif /* WHITEPAGES_ENV */
