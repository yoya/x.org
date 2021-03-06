/***********************************************************
		Copyright IBM Corporation 1988

                      All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the name of IBM not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.  

IBM DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
IBM BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.
******************************************************************/
/* 
 * doload.h - environment for dynamic loader
 *
 * Modified for TelmatOS 4.1/S5R4 by Guy Harris, November 1990.
 * This file is not used by doload.c in TelmatOS 4.1/S5R4, rather it declares
 * the external interface to the doload routine for other parts of the class
 * system to use.
 */

/* here is the state during a load */

typedef enum doload_mode_enum {
	Load,			/* .. loading .do file */
	List,			/* .. listing .do or .o file */
	Fix			/* .. converting .o to .do file */
} doload_mode;

extern int doload_trace;

#define doload_extension ".do"

extern void *(*doload() )();
