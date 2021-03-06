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
static char rcsid[]="$Header: /afs/cs.cmu.edu/project/atk-dist/auis-6.3/overhead/class/machdep/dec_mach/RCS/globals.c,v 1.3 1992/12/15 20:59:09 rr2b R6tape $";
#endif


/* 
 * array of global entry points for dynamic loader
 *
 * Defines a table of entry points and their names
 *
 */

/* macros which actually define entry points */

/*   globalref - direct reference to global symbol x */

#define globalref(x) extern long x;
#define routine(x) extern void x();

	globalref(class_RoutineStruct)
	globalref(class_Error)

	routine(class_NewObject)
	routine(class_Load)
	routine(class_IsLoaded)
	routine(class_Lookup)
	routine(class_IsType)
	routine(class_IsTypeByName)
	routine(class_EnterInfo)
	routine(class_SetClassPath)
	routine(class_PrependClassPath)
	routine(class_GetEText)
	routine(class_GetTextBase)
	routine(class_GetTextLength)


/* common symbols referred to but not defined directly in libc.a */

	globalref(environ)		/* common symbol, defined nowhere */
	globalref(errno)		/* cerror */

/* do not delete the following line - make depends on it */

#include	<globalrefs._h>

#undef globalref
#undef routine

#if 1
#define globalref(x) (long) &x, "x",
#else
#define globalref(x)	.globl x \
                        .data \
                        $$x: \
                         .asciiz	"x" \
                        .text \
                	.word	x, $$x
.text
#endif

#define routine(x) (long) x, "x",

#if 0
	.globl	globals
globals: /* beginning of entry point table */
#else
struct globaltab {
    long entrypoint;	/* entry point value */
    char *entryname;	/* symbolic name */
} globals[] = {
#endif
/* from libclass */
	globalref(class_RoutineStruct)
	globalref(class_Error)

	routine(class_NewObject)
	routine(class_Load)
	routine(class_IsLoaded)
	routine(class_Lookup)
	routine(class_IsType)
	routine(class_IsTypeByName)
	routine(class_EnterInfo)
	routine(class_SetClassPath)
	routine(class_PrependClassPath)
	routine(class_GetEText)
	routine(class_GetTextBase)
	routine(class_GetTextLength)


/* common symbols referred to but not defined directly in libc.a */

	globalref(environ)		/* common symbol, defined nowhere */
	globalref(errno)		/* cerror */

/* do not delete the following line - make depends on it */

#include	<globalrefs._h>

#if 0
	.text

	.globl	globalend
globalend:    .space 0
#else
};

long globalcount = sizeof(globals) / sizeof(struct globaltab);
#endif
