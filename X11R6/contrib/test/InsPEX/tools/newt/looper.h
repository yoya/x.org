
/* $XConsortium: looper.h,v 5.2 94/04/17 20:46:21 rws Exp $ */

/*****************************************************************

Copyright (c) 1989,1990, 1991  X Consortium

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
X CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of the X Consortium shall not be
used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization from the X Consortium.

Copyright (c) 1989,1990, 1991 by Sun Microsystems, Inc.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the names of Sun Microsystems,
and the X Consortium, not be used in advertising or publicity 
pertaining to distribution of the software without specific, written 
prior permission.  

SUN MICROSYSTEMS DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, 
INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT 
SHALL SUN MICROSYSTEMS BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL 
DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/


#include <stdio.h>
#include <ctype.h>


typedef enum {		/* types of functions handled by looper: */
	FOREACHLOOP,		/* foreach loop */
	FORLOOP,		/* iterative for loop */
	IFSTMNT,		/* if statement with single value condition*/
	PRED,			/* simple predicate or function assignment */
	ASSIGN,			/* looper var assignment (@var=arg style) */
	MACRODEF,		/* macro name and definition */
	SOURCE,			/* file reading command */
	COMMENT,		/* csh/sh-style comment */
} FUNCTYPE;

typedef struct ARG_STRUCT {
	char *name;
	char *cmt;		/* comment field */
	struct ARG_STRUCT *next;
} ARGTYPE;

struct FOREACH_STRUCT {	/* foreach loop structure */
	struct ARG_STRUCT *loopvars;
	struct ARG_STRUCT *values;
	struct FUNC_STRUCT *body;
};

struct FOR_STRUCT {	/* for loop structure */
	struct ARG_STRUCT *loopvar;
	struct ARG_STRUCT *start;
	struct ARG_STRUCT *end;
	struct ARG_STRUCT *incr;
	struct FUNC_STRUCT *body;
};

struct IF_STRUCT {
	struct ARG_STRUCT *cond, *oper, *cond2;
	struct FUNC_STRUCT *thenbody;
	struct FUNC_STRUCT *elsebody;
};

struct PRED_STRUCT {    /* simple predicate structure */
	char *name;
	char *dest;    /* destination var if an assignment */
	struct ARG_STRUCT *args;
};

struct ASSIGN_STRUCT {
	ARGTYPE *var;
	ARGTYPE *val;
};

struct MACRO_STRUCT {
	char *name;
	char *fname;
	ARGTYPE *macvars;
	struct FUNC_STRUCT *body;
	struct FUNC_STRUCT *next_macro, *prev_macro;
};

struct SOURCE_STRUCT {
	char *fname;
	struct FUNC_STRUCT *body;
};

struct COMMENT_STRUCT {
	char *text;
};

typedef struct FUNC_STRUCT {
	FUNCTYPE ftype;
	int lineno;
	struct FUNC_STRUCT *next;
	union {
		struct FOREACH_STRUCT fl;
		struct FOR_STRUCT forl;
		struct IF_STRUCT ifstr;
		struct PRED_STRUCT pred;
		struct ASSIGN_STRUCT assign;
		struct MACRO_STRUCT macro;
		struct SOURCE_STRUCT source;
		struct COMMENT_STRUCT cmt;
    	} fun;
} FUNC;

typedef struct MACROVAR_STRUCT {
	char *shortname, *longname;
	struct MACROVAR_STRUCT *next;
} MACROVARLIST;
extern char* N_filepath; /* path of directories in which to search for files */
extern int N_translate; /* non-zero means in translation mode */
extern int N_2process;  /* two process debugging mode */
extern FUNC *N_funchead; /* pointer to start of function structure */
extern void N_execute();
extern FILE* N_fopen_from_path();
