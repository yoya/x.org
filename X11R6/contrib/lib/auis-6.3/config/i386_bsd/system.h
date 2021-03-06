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

#ifndef	SYSTEM_H
#define	SYSTEM_H


/* Get common definitions */
#include <allsys.h>


#define	OPSYSNAME	"i386_bsd"
#define	SYS_NAME	"i386_bsd"
/*#define vax2            1*/
/*#define	sys_vax_43	1*/
/*#define	VAX_ENV         1*/


/* Here follow the overrides for this system. */
#undef	SY_B43
#define	SY_B43	1 /* This system is most like bsd 4.3 */


#ifndef In_Imake

/* Get major data types (esp. caddr_t) */
#include <sys/types.h>

/* Get open(2) constants */
#include <sys/file.h>

#include <sys/dir.h>
#define DIRENT_TYPE struct direct
#define DIRENT_NAMELEN(d) (strlen((d)->d_name))
#define NEWPGRP() setpgrp(0, getpid())

#undef FILE_HAS_IO
#define FILE_HAS_IO(f) ((f)->_r > 0)

#undef FILE_NEEDS_FLUSH
#define FILE_NEEDS_FLUSH(f) ((f)->_bf._base)

#include <strings.h>

/* Get struct timeval */
#include <sys/time.h>

/* include path for syslog.h */
#include <syslog.h>

/* VMUNIX vs. SY_B4x */
#ifndef VMUNIX
#define	VMUNIX	1
#endif VMUNIX

#define OSI_HAS_SYMLINKS 1
#define osi_readlink(PATH,BUF,SIZE) readlink((PATH),(BUF),(SIZE))

#define osi_ExclusiveLockNoBlock(fid)	flock((fid), LOCK_EX | LOCK_NB)
#define osi_UnLock(fid)			flock((fid), LOCK_UN)
#define osi_O_READLOCK			O_RDONLY
#define osi_F_READLOCK			"r"

#define	osi_vfork()			vfork()

#define	osi_setjmp  _setjmp
#define	osi_longjmp _longjmp

/* Make a time standard. */
struct osi_Times {unsigned long int Secs; unsigned long int USecs;};
/* Set one of the above with a call to osi_GetTimes(&foo) */
#define osi_GetSecs() time((long int *) 0)
extern void osi_SetZone();
extern char *osi_ZoneNames[];
extern long int osi_SecondsWest;
extern int osi_IsEverDaylight;


#define NEED_ANSI_TMPFILES 1
#ifndef IN_ATKOS_LIB
#include <atkos.h>
#endif

/* Put system-specific definitions here */

#define HAS_SYSEXITS 1


/* NetBSD, FreeBSD, 386BSD: off_t lseek() */
#if defined(__NetBSD__) || defined(__386BSD__) || defined(__FreeBSD__)
#if defined(LSEEK_RETURN_TYPE)
#undef LSEEK_RETURN_TYPE
#endif
#define LSEEK_RETURN_TYPE off_t
#endif


/* NetBSD > 0.9: extern const char *const sys_errlist[] */
#if defined(__NetBSD__) && !defined(__386BSD__)
#if defined(SYS_ERRLIST_TYPE)
#undef SYS_ERRLIST_TYPE
#endif
#define SYS_ERRLIST_TYPE extern const char *const
#endif


/* NetBSD > 0.9: missplaced the definition of SGTTY (used by VUI) */
#if defined(__NetBSD__) && !defined(__386BSD__)
#define SGTTY struct sgttyb
#endif


#endif /* !In_Imake */


#define FLEX_ENV 1
#define ANSI_COMPILER 1


/* Now follow the site-specific customizations. */
#include <site.h>

#endif	/* SYSTEM_H */
