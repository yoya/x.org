
/* $XConsortium: utility.m,v 5.2 94/04/17 20:46:42 rws Exp $ */

/*****************************************************************

Copyright (c) 1989, 1990, 1991,1990  X Consortium

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

Copyright (c) 1989, 1990, 1991,1990 by Sun Microsystems, Inc.

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


/****************************************************************/
/*  utility.m: general utility functions			*/
/****************************************************************/

/*********** user's function ***************/
/*
 * Integer, Logical, and Boolean Math functions
 */
int 	plus(int,int)		/* 1st arg gets result of oper */
int 	minus(int,int)		/* performed on later args */
int 	multiply(int,int)
int 	divide(int,int)
int 	equal(int,int)
int 	ifval(int,int,int)	/* like a = b ? c : d; in C lang. */
int 	and(int,int)		/* logical */
int 	or(int,int)
int 	not(int)
int 	bitand(int,int)		/* bit-wise */
int 	bitor(int,int)
int 	bitnot(int)

/* floating point functions */
double 	ftod(float)		/* coverted a float to a double */
int 	fequal(float,float)	/* check equality of two floats */
float 	fplus(float,float)	/* 1st arg gets result of oper */
float 	fminus(float,float)	/* performed on later args */
float 	fmultiply(float,float)
float 	fdivide(float,float)

/*
 * Input/Output Functions
 */
!void	printf(char*,VARARG)
void 	beep()

/*********** Unix, C's function ***************/
/* 
 * C library functions and UNIX system calls
 */
void 	sleep(int)

/* memory functions */
char* 	malloc(int)
void	free(char*)  
 
/*string functions */
char*	strcat(char*, char*)
int 	strcmp(char*,char*)
char* 	strcpy(char*,char*)
int 	strlen(char*)

/* env functions */
char*	getenv(char*)
int	getchar()

void    system(char*)
void 	unlink(char*)
