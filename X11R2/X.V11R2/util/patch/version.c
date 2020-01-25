/* $Header: /usr/src/unsup/bin/patch/RCS/version.c,v 2.7 87/02/04 11:06:37 ksb Net $
 *
 * $Log:	version.c,v $
 * Revision 2.7  87/02/04  11:06:37  ksb
 * patched to revision 2.7
 * 
 * Revision 2.0  86/12/02  14:31:15  doc
 * Initial Distributed Revision
 * 
 * Revision 2.0  86/09/17  15:40:11  lwall
 * Baseline for netwide release.
 * 
 */

#include "EXTERN.h"
#include "common.h"
#include "util.h"
#include "INTERN.h"
#include "patchlevel.h"
#include "version.h"

/* Print out the version number and die. */

void
version()
{
    extern char rcsid[];

#ifdef lint
    rcsid[0] = rcsid[0];
#else
    fatal3("%s\nPatch level: %d\n", rcsid, PATCHLEVEL);
#endif
}
