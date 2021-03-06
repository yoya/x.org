
/* $XConsortium: utility.c,v 5.2 94/04/17 20:46:41 rws Exp $ */

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




/*
 * utility.c
 *
 * Some generic interpreter functions, usable with any library 
 * interpreter.  
 *
 * The .m file is utility.m.  
 *
 */

#include "utility.h"

#ifdef NEWT
#include "NewTvararg.h"
#else
#include "varargs.h"
#endif

#define MAXSTRLEN 256
#define MAXLEN 256


/***************** INTEGER MATHEMATICAL FUNCTIONS ****************/

int
plus(x,y)
	int x,y;
{
	return(x+y);
}

int
minus(x,y)
	int x,y;
{
	return(x-y);
}

int
multiply(x,y)
	int x,y;
{
	return(x*y);
}

int
divide(x,y)
	int x,y;
{
	return(x/y);
}

int
equal(x,y)
	int x,y;
{
	return(x==y);
}


/*
 * z = ifval(p, x, y) means:
 * if p is nonzero, then z=x, else z=y.
 *
*/
int
ifval(p,x,y)
	int p, x, y;
{
	return(p? x: y);
}

/*
 * z = and(x,y) sets z to 1 if x and y are nonzero.
 */
int
and(x,y)
	int x,y;
{
	return((x && y) ? 1 : 0); 
}

/*
 * z = or (x,y) sets z to 1 if x or y are nonzero.
 */
int
or(x,y)
	int x,y;
{
	return((x || y) ? 1 : 0); 
}


/*
 * b = not(a) sets b to ~a.
 */
int
not(a)
	int a;
{
	return(a ? 0 : 1);
}


/*
 * z = bitand(x,y) sets z to bitwise-and of x and y
 */
int
bitand(x,y)
	int x,y;
{
	return(x & y);
}

/*
 * z = bitor (x,y) sets z to bitwise-or of x and y
 */
int
bitor(x,y)
	int x,y;
{
	return(x | y);
}

/*
 * b = not(a) sets b to bitwise negation of ~a.
 */
int
bitnot(a)
	int a;
{
	return(~a);
}


/****************** FLOATING POINT MATH FUNCTIONS **********/
double
ftod(x)
	float x;
{
	double y;
	return (y=x);
}

int
fequal(x,y)
	float x,y;
{
	return(feq(x,y));
}

float
fplus(x,y)
	float x,y;
{
	return(x + y);
}

float
fminus(x,y)
	float x,y;
{
	return(x - y);
}

float
fmultiply(x,y)
	float x,y;
{
	return(x * y);
}

float
fdivide(x,y)
	float x,y;
{
	return(x / y);
}

/**** output functions ******/
void
beep()
{
	printf("\7");
	fflush(stdout);
}


void
_printf(fmt,va_alist)
	char *fmt;
	va_dcl
{
	va_list ap;
	char *p;
	char *fp;
	char form[MAXSTRLEN];

	if (strlen(fmt) >= MAXSTRLEN) {
		printf("_prinf: format string exceeded maximum length\n");
		return;
	}
	va_start(ap);
	for(p = fmt; *p; p++) {
		if (*p != '%') {
			putchar(*p);
			continue;
		}
		/* just found %, search for ...d, ...f or ...s */
		for (fp=form,*fp++= *p++;*p;p++,fp++) {
			sprintf(fp,"%c",*p);
			if (*p == 'd') {
				printf(form,va_arg(ap,int));
				fflush(stdout);
				break;
			} else if (*p == 'f') {
				printf(form,va_arg(ap,double));
				fflush(stdout);
				break;
			} else if (*p == 's') {
				printf(form,va_arg(ap,char*));
				fflush(stdout);
				break;
			}
		}
		/* 
		  search for ...d, ...f or ...s failed, 
		  just print out what's accumulated so far
		*/
		if (*p == '\0') {
			printf("%s",form);
			break;
		}
	}
	va_end(ap);
}

