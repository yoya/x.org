/* $XConsortium: xkbparse.h,v 1.2 94/04/04 15:28:46 rws Exp $ */
/************************************************************
 Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.

 Permission to use, copy, modify, and distribute this
 software and its documentation for any purpose and without
 fee is hereby granted, provided that the above copyright
 notice appear in all copies and that both that copyright
 notice and this permission notice appear in supporting
 documentation, and that the name of Silicon Graphics not be 
 used in advertising or publicity pertaining to distribution 
 of the software without specific prior written permission.
 Silicon Graphics makes no representation about the suitability 
 of this software for any purpose. It is provided "as is"
 without any express or implied warranty.
 
 SILICON GRAPHICS DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS 
 SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY 
 AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL SILICON
 GRAPHICS BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL 
 DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, 
 DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE 
 OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION  WITH
 THE USE OR PERFORMANCE OF THIS SOFTWARE.

 ********************************************************/

#ifndef XKBPARSE_H
#define	XKBPARSE_H 1

#ifndef DEBUG_VAR
#define	DEBUG_VAR	parseDebug
#endif

#include "xkbcomp.h"

extern	char	*scanStr;
extern	int	 scanInt;
extern	int	 lineNum;

static	XkbFile	*rtrnValue;

#ifdef DEBUG
#define	d(str)		fprintf(stderr,"%s\n",str);
#define d1(str,a)	fprintf(stderr,str,a);
#define d2(str,a,b)	fprintf(stderr,str,a,b);
#else
#define	d(str)
#define	d1(str,a)
#define d2(str,a,b)
#endif

#endif /* XKBPARSE_H */
