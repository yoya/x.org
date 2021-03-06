/* ********************************************************************** *\
 *         Copyright IBM Corporation 1988,1991 - All Rights Reserved      *
 *        For full copyright information see:'andrew/config/COPYRITE'     *
\* ********************************************************************** */

/*
	$Disclaimer: 
 * Permission to use, copy, modify, and distribute this software and its 
 * documentation for any purpose is hereby granted without fee, 
 * provided that the above copyright notice appear in all copies and that 
 * both that copyright notice, this permission notice, and the following 
 * disclaimer appear in supporting documentation, and that the names of 
 * IBM, Carnegie Mellon University, and other copyright holders, not be 
 * used in advertising or publicity pertaining to distribution of the software 
 * without specific, written prior permission.
 * 
 * IBM, CARNEGIE MELLON UNIVERSITY, AND THE OTHER COPYRIGHT HOLDERS 
 * DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING 
 * ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.  IN NO EVENT 
 * SHALL IBM, CARNEGIE MELLON UNIVERSITY, OR ANY OTHER COPYRIGHT HOLDER 
 * BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY 
 * DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, 
 * WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS 
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE 
 * OF THIS SOFTWARE.
 *  $
*/

#ifndef NORCSID
#define NORCSID
static char rcsid[]="$Header: /afs/cs.cmu.edu/project/atk-dist/auis-6.3/ams/libs/ms/RCS/flush.c,v 2.11 1992/12/15 21:18:15 rr2b R6tape $";
#endif


 

#include <andrewos.h>
#include <ms.h>
#ifdef AFS_ENV
#include <netinet/in.h>
#include <afs/param.h>
#include <sys/ioctl.h>
#include <afs/errors.h>
#include <afs/prs_fs.h>
#include <afs/venus.h>
#endif /* AFS_ENV */

ViceFlush(fname)
char *fname;
{
#ifdef AFS_ENV
    struct ViceIoctl blob;
    if (AMS_ViceIsRunning) {
	blob.out_size = 0;
	blob.in_size = 0;

	if (pioctl(fname, VIOCFLUSHCB, &blob)) {
	    debug(1, ("Callback flush of %s FAILED!\n", fname));
	    if (pioctl(fname, VIOCFLUSH, &blob)) {
		debug(1, ("Both flushes of %s FAILED!\n", fname));
		AMS_RETURN_ERRCODE(errno, EIN_PIOCTL, EVIA_PREFETCHMSG);
	    }
	}
	debug(1, ("ViceFlush of %s succeeded\n", fname));
    }
#endif /* AFS_ENV */
    return(0);
}

ViceFlushPlusParent(fname)
char *fname;
{
    long code = 0;
#ifdef AFS_ENV
    char *s;

    if (AMS_ViceIsRunning) {
    if (ViceFlush(fname)) return(mserrcode);
    s = strrchr(fname, '/');
    if (s) {
	*s = '\0';
	code = ViceFlush(fname);
	*s = '/';
    }
    }
#endif /* AFS_ENV */
    return(code);
}
