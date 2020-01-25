/*
 
Copyright (c) 1990, 1991  X Consortium

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

 *
 * Copyright 1990, 1991 by UniSoft Group Limited.
 * 
 * Permission to use, copy, modify, distribute, and sell this software and
 * its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and that
 * both that copyright notice and this permission notice appear in
 * supporting documentation, and that the name of UniSoft not be
 * used in advertising or publicity pertaining to distribution of the
 * software without specific, written prior permission.  UniSoft
 * makes no representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 * $XConsortium: tpstartup.c,v 1.9 94/04/17 21:01:07 rws Exp $
 */

#include	"xtest.h"
#include	"Xlib.h"
#include	"Xutil.h"
#include	"xtestlib.h"
#include	"pixval.h"

extern	Display	*Dsp;

#ifdef DEBUG
static int 	brkval;
#endif

/*
 * Actions to take at the beginning of a test purpose.
 */
void
tpstartup()
{
#if TEST_ANSI
extern  int     unexp_err(Display *, XErrorEvent *ep);
extern  int     io_err(Display *);
#else
extern  int     unexp_err();
extern  int     io_err();
#endif

	(void) XSetErrorHandler(unexp_err);
	(void) XSetIOErrorHandler(io_err);

	resetdelete();

	if (Dsp)
		XResetScreenSaver(Dsp);
#ifdef DEBUG
	brkval = sbrk(0);
#endif

	/* Do this here in case tpcleanup did not get called */
	freereg();

	/*
	 * Clean up the common display client.
	 */
	if (Dsp)
		XSync(Dsp, True);
}

/*
 * Actions to take at the end of a test purpose.
 */
void
tpcleanup()
{
	/* Free all registered resources */
	freereg();

	/*
	 * Try to ensure that everything that is going to happen for
	 * this test happens now, rather than popping up in the next test.
	 */
	if (Dsp)
		XSync(Dsp, False);
#ifdef DEBUG
	{
	int 	newbrk;
	extern	int 	tet_thistest;

		newbrk = sbrk(0);
		debug(1, "brk value was %d after test %d", newbrk, tet_thistest);
		if (newbrk > brkval)
			printf("brk value increased to %d (+%d) after test %d\n"
				, newbrk, newbrk-brkval, tet_thistest);
	}
#endif
}