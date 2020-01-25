/*
 *	$Header: /var/home/sm/ws/michael/work/xterm/RCS/data.c,v 1.2 1994/06/07 00:05:30 michael Exp $ Sony Corporation
 */
/******************************************************************

Copyright (c) 1992, 1993, 1994  Sony Corporation

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL SONY CORPORATION BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of Sony Corporation shall
not be used in advertising or otherwise to promote the sale, use or ot
her dealings in this Software without prior written authorization from
Sony Corporation.

******************************************************************/
/*
 *	$XConsortium: data.c,v 1.11 93/02/25 17:21:27 gildea Exp $
 */

/*
 * Copyright 1987 by Digital Equipment Corporation, Maynard, Massachusetts.
 *
 *                         All Rights Reserved
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted,
 * provided that the above copyright notice appear in all copies and that
 * both that copyright notice and this permission notice appear in
 * supporting documentation, and that the name of Digital Equipment
 * Corporation not be used in advertising or publicity pertaining to
 * distribution of the software without specific, written prior permission.
 *
 *
 * DIGITAL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
 * ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
 * DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
 * ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
 * ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 */

#include "ptyx.h"		/* gets Xt stuff, too */
#include "data.h"
#include <setjmp.h>

XPoint T_boxlarge[NBOX] = {
	{0, 0},
	{8, 0},
	{0, 14},
	{-8, 0},
	{0, -14},
};
XPoint T_box2[NBOX] = {
	{0, 0},
	{7, 0},
	{0, 12},
	{-7, 0},
	{0, -12},
};
XPoint T_box3[NBOX] = {
	{0, 0},
	{5, 0},
	{0, 12},
	{-5, 0},
	{0, -12},
};
XPoint T_boxsmall[NBOX] = {
	{0, 0},
	{5, 0},
	{0, 9},
	{-5, 0},
	{0, -9},
};
jmp_buf Tekend;
int Tbcnt = 0;
Char *Tbuffer;
Char *Tbptr;
TekLink *TekRefresh;
Char *Tpushb;
Char *Tpushback;
int Ttoggled = 0;
int bcnt = 0;
Char buffer[BUF_SIZE];
Char *bptr = buffer;
jmp_buf VTend;
XPoint VTbox[NBOX] = {
	{0, 0},
	{0, 0},
	{0, 0},
	{0, 0},
	{0, 0},
};
#ifdef MULTIFONT
XPoint VTbox2[NBOX] = {
	{0, 0},
	{0, 0},
	{0, 0},
	{0, 0},
	{0, 0},
};
Char pending = '\0';
#endif /* MULTIFONT */

#ifdef DEBUG
int debug = 0; 		/* true causes error messages to be displayed */
#endif	/* DEBUG */
XtermWidget term;		/* master data structure for client */
char *xterm_name;	/* argv[0] */
int am_slave = 0;	/* set to 1 if running as a slave process */
int max_plus1;
int pty_mask;
int Select_mask;
int X_mask;
char *ptydev;
char *ttydev;
#ifdef ALLOWLOGGING
char log_def_name[] = "XtermLog.XXXXX";
#endif
int T_lastx = -1;
int T_lasty = -1;

int waitingForTrackInfo = 0;
EventMode eventMode = NORMAL;

GC visualBellGC;

int VTgcFontMask = GCFont;
int TEKgcFontMask = GCFont;

TekWidget tekWidget;
