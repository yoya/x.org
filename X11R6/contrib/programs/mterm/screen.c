/*
 *	$Header: /var/home/sm/ws/michael/work/xterm/RCS/screen.c,v 1.2 1994/06/07 00:05:30 michael Exp $ Sony Corporation
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
 *	$XConsortium: screen.c,v 1.33 94/04/02 17:34:36 gildea Exp $
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

/* screen.c */

#include "ptyx.h"
#include "error.h"
#include "data.h"

#include <stdio.h>
#include <signal.h>
#ifdef SVR4
#include <termios.h>
#else
#include <sys/ioctl.h>
#endif

#ifdef att
#include <sys/termio.h>
#include <sys/stream.h>			/* get typedef used in ptem.h */
#include <sys/ptem.h>
#endif

extern Char *calloc(), *malloc(), *realloc();
extern void free();

ScrnBuf Allocate (nrow, ncol, addr)
/*
   allocates memory for a 2-dimensional array of chars and returns a pointer
   thereto
   each line is formed from a pair of char arrays.  The first (even) one is
   the actual character array and the second (odd) one is the attributes.
 */
register int nrow, ncol;
Char **addr;
{
	register ScrnBuf base;
	register Char *tmp;
	register int i;

#ifdef NBUF_TYPE
	if ((base = (ScrnBuf) calloc ((unsigned)(nrow *= NBUF_TYPE), sizeof (char *))) == 0)
#else /* NBUF_TYPE */
	if ((base = (ScrnBuf) calloc ((unsigned)(nrow *= 2), sizeof (char *))) == 0)
#endif /* NBUF_TYPE */
		SysError (ERROR_SCALLOC);

	if ((tmp = calloc ((unsigned) (nrow * ncol), sizeof(char))) == 0)
		SysError (ERROR_SCALLOC2);

	*addr = tmp;
	for (i = 0; i < nrow; i++, tmp += ncol)
		base[i] = tmp;

	return (base);
}

/*
 *  This is called when the screen is resized.
 *  Returns the number of lines the text was moved down (neg for up).
 *  (Return value only necessary with SouthWestGravity.)
 */
static
Reallocate(sbuf, sbufaddr, nrow, ncol, oldrow, oldcol)
    ScrnBuf *sbuf;
    Char **sbufaddr;
    int nrow, ncol, oldrow, oldcol;
{
	register ScrnBuf base;
	register Char *tmp;
	register int i, minrows, mincols;
	Char *oldbuf;
	int move_down = 0, move_up = 0;
#ifdef STATUSLINE
	Char *s0, *s1;
#ifdef MULTIFONT
	Char *s2;
#endif /* MULTIFONT */
#ifdef COLOR
	Char *sc;
#endif /* COLOR */
#endif /* STATUSLINE */
	
	if (sbuf == NULL || *sbuf == NULL)
		return 0;

#ifdef NBUF_TYPE
	oldrow *= NBUF_TYPE;
#ifdef STATUSLINE
	s0 = (*sbuf)[oldrow + CODE_BUF];
	s1 = (*sbuf)[oldrow + ATTR_BUF];
#ifdef MULTIFONT
	s2 = (*sbuf)[oldrow + FNUM_BUF];
#endif /* MULTIFONT */
#ifdef COLOR
	sc = (*sbuf)[oldrow + COLOR_BUF];
#endif /* COLOR */
#endif /* STATUSLINE */
#else /* NBUF_TYPE */
	oldrow *= 2;
#ifdef STATUSLINE
	s0 = (*sbuf)[oldrow + 0];
	s1 = (*sbuf)[oldrow + 1];
#endif /* STATUSLINE */
#endif /* NBUF_TYPE */
	oldbuf = *sbufaddr;

	/*
	 * Special case if oldcol == ncol - straight forward realloc and
	 * update of the additional lines in sbuf
	 */

	/* this is a good idea, but doesn't seem to be implemented.  -gildea */

	/* 
	 * realloc sbuf, the pointers to all the lines.
	 * If the screen shrinks, remove lines off the top of the buffer
	 * if resizeGravity resource says to do so.
	 */
#ifdef NBUF_TYPE
	nrow *= NBUF_TYPE;
#ifdef STATUSLINE
	nrow += NBUF_TYPE;
#endif /* STATUSLINE */
#else /* NBUF_TYPE */
	nrow *= 2;
#ifdef STATUSLINE
	nrow += 2;
#endif /* STATUSLINE */
#endif /* NBUF_TYPE */
	if (nrow < oldrow  &&  term->misc.resizeGravity == SouthWestGravity) {
	    /* Remove lines off the top of the buffer if necessary. */
	    move_up = oldrow-nrow 
#ifdef NBUF_TYPE
		        - NBUF_TYPE*(term->screen.max_row - term->screen.cur_row);
#else /* NBUF_TYPE */
		        - 2*(term->screen.max_row - term->screen.cur_row);
#endif /* NBUF_TYPE */
	    if (move_up < 0)
		move_up = 0;
	    /* Overlapping memmove here! */
	    memmove( *sbuf, *sbuf+move_up, 
		  (oldrow-move_up)*sizeof((*sbuf)[0]) );
	}
	*sbuf = (ScrnBuf) realloc((char *) (*sbuf),
				  (unsigned) (nrow * sizeof(char *)));
	if (*sbuf == 0)
	    SysError(ERROR_RESIZE);
	base = *sbuf;

	/* 
	 *  create the new buffer space and copy old buffer contents there
	 *  line by line.
	 */
	if ((tmp = calloc((unsigned) (nrow * ncol), sizeof(char))) == 0)
		SysError(ERROR_SREALLOC);
	*sbufaddr = tmp;
	minrows = (oldrow < nrow) ? oldrow : nrow;
	mincols = (oldcol < ncol) ? oldcol : ncol;
#ifdef STATUSLINE
#ifdef NBUF_TYPE
	nrow -= NBUF_TYPE;
	oldrow -= NBUF_TYPE;
	minrows -= NBUF_TYPE;
#else /* NBUF_TYPE */
	nrow -= 2;
	oldrow -= 2;
	minrows -= 2;
#endif /* NBUF_TYPE */
#endif /* STATUSLINE */
	if (nrow > oldrow  &&  term->misc.resizeGravity == SouthWestGravity) {
	    /* move data down to bottom of expanded screen */
#ifdef NBUF_TYPE
	    move_down = Min(nrow-oldrow, NBUF_TYPE*term->screen.savedlines);
#else /* NBUF_TYPE */
	    move_down = Min(nrow-oldrow, 2*term->screen.savedlines);
#endif /* NBUF_TYPE */
	    tmp += ncol*move_down;
	}
	for (i = 0; i < minrows; i++, tmp += ncol) {
		memmove( tmp, base[i], mincols);
	}
	/*
	 * update the pointers in sbuf
	 */
	for (i = 0, tmp = *sbufaddr; i < nrow; i++, tmp += ncol)
	    base[i] = tmp;

#ifdef STATUSLINE
	bcopy(s0, tmp, mincols);
	base[i++] = tmp;
	tmp += ncol;
	bcopy(s1, tmp, mincols);
	base[i++] = tmp;
	tmp += ncol;
#ifdef MULTIFONT
	bcopy(s2, tmp, mincols);
	base[i++] = tmp;
	tmp += ncol;
#endif /* MULTIFONT */
#ifdef COLOR
	bcopy(sc, tmp, mincols);
	base[i++] = tmp;
	tmp += ncol;
#endif /* COLOR */
#endif /* STATUSLINE */
        /* Now free the old buffer */
	free(oldbuf);

#ifdef NBUF_TYPE
	return move_down ? move_down/NBUF_TYPE : -move_up/NBUF_TYPE; /* convert to rows */
#else /* NBUF_TYPE */
	return move_down ? move_down/2 : -move_up/2; /* convert to rows */
#endif /* NBUF_TYPE */
}

ScreenWrite (screen, str, flags, length)
/*
   Writes str into buf at row row and column col.  Characters are set to match
   flags.
 */
TScreen *screen;
char *str;
register unsigned flags;
register int length;		/* length of string */
{
	register Char *attrs, *attrs0;
	register int avail  = screen->max_col - screen->cur_col + 1;
	register Char *col;
	register int wrappedbit;
#ifdef MULTIFONT
	register Char *fnum;
	Char fn;
	Boolean max_flag = False;
#endif /* MULTIFONT */
#ifdef COLOR
	register Char *color;
	Char cc;
#endif /* COLOR */

	if (length > avail)
	    length = avail;
	if (length <= 0)
		return;

#ifdef NBUF_TYPE
	avail = NBUF_TYPE * screen->cur_row;
	col = screen->buf[avail + CODE_BUF] + screen->cur_col;
	attrs = attrs0 = screen->buf[avail + ATTR_BUF] + screen->cur_col;
#ifdef MULTIFONT
	fnum = screen->buf[avail + FNUM_BUF] + screen->cur_col;
	fn = (flags & FNUM_MASK) >> 28;
#endif /* MULTIFONT */
#ifdef COLOR
	color = screen->buf[avail + COLOR_BUF] + screen->cur_col;
	cc = (flags & COLOR_MASK) >> 20;
#endif /* COLOR */
#else /* NBUF_TYPE */
	col = screen->buf[avail = 2 * screen->cur_row] + screen->cur_col;
	attrs = attrs0 = screen->buf[avail + 1] + screen->cur_col;
#endif /* NBUF_TYPE */
	wrappedbit = *attrs0&LINEWRAPPED;
	flags &= ATTRIBUTES;
	flags |= CHARDRAWN;
	memmove( col, str, length);
#ifdef NBUF_TYPE
#ifdef MULTIFONT
	if (*attrs & SECOND) {
		col[-1] = 0;
		attrs[-1] = 0;
		fnum[-1] = 0;
		color[-1] = 0;
	}
	col + length;

	if (flags & MBSCS) {
		while(length > 0) {
			*attrs++ = flags & ~SECOND;
			*attrs++ = flags & ~FIRST;
			*fnum++ = fn;
			*fnum++ = fn;
#ifdef COLOR
			*color++ = cc;
			*color++ = cc;
#endif /* COLOR */
			length -= 2;
		}
	} else {
		while(length-- > 0) {
			*attrs++ = flags;
			*fnum++ = fn;
#ifdef COLOR
			*color++ = cc;
#endif /* COLOR */
		}
	}
#else /* MULTIFONT */
	while(length-- > 0) {
		*attrs++ = flags;
		*color++ = cc;
	}
#endif /* MULTIFONT */
#else /* NBUF_TYPE */
	while(length-- > 0)
		*attrs++ = flags;
#endif /* NBUF_TYPE */
	if (wrappedbit)
	    *attrs0 |= LINEWRAPPED;
}

ScrnInsertLine (sb, last, where, n, size)
/*
   Inserts n blank lines at sb + where, treating last as a bottom margin.
   Size is the size of each entry in sb.
   Requires: 0 <= where < where + n <= last
   	     n <= MAX_ROWS
 */
register ScrnBuf sb;
int last;
register int where, n, size;
{
	register int i;
#ifdef NBUF_TYPE
	char *save [NBUF_TYPE * MAX_ROWS];


	/* save n lines at bottom */
	memmove( (char *) save, (char *) &sb [NBUF_TYPE * (last -= n - 1)], 
		NBUF_TYPE * sizeof (char *) * n);
	
	/* clear contents of old rows */
	for (i = NBUF_TYPE * n - 1; i >= 0; i--)
		bzero ((char *) save [i], size);

	/*
	 * WARNING, overlapping copy operation.  Move down lines (pointers).
	 *
	 *   +----|---------|--------+
	 *
	 * is copied in the array to:
	 *
	 *   +--------|---------|----+
	 */
	memmove( (char *) &sb [NBUF_TYPE * (where + n)], (char *) &sb [NBUF_TYPE * where], 
		NBUF_TYPE * sizeof (char *) * (last - where));

	/* reuse storage for new lines at where */
	memmove( (char *) &sb[NBUF_TYPE * where], (char *)save, NBUF_TYPE * sizeof(char *) * n);
#else /* NBUF_TYPE */
	char *save [2 * MAX_ROWS];


	/* save n lines at bottom */
	memmove( (char *) save, (char *) &sb [2 * (last -= n - 1)], 
		2 * sizeof (char *) * n);
	
	/* clear contents of old rows */
	for (i = 2 * n - 1; i >= 0; i--)
		bzero ((char *) save [i], size);

	/*
	 * WARNING, overlapping copy operation.  Move down lines (pointers).
	 *
	 *   +----|---------|--------+
	 *
	 * is copied in the array to:
	 *
	 *   +--------|---------|----+
	 */
	memmove( (char *) &sb [2 * (where + n)], (char *) &sb [2 * where], 
		2 * sizeof (char *) * (last - where));

	/* reuse storage for new lines at where */
	memmove( (char *) &sb[2 * where], (char *)save, 2 * sizeof(char *) * n);
#endif /* NBUF_TYPE */
}


ScrnDeleteLine (sb, last, where, n, size)
/*
   Deletes n lines at sb + where, treating last as a bottom margin.
   Size is the size of each entry in sb.
   Requires 0 <= where < where + n < = last
   	    n <= MAX_ROWS
 */
register ScrnBuf sb;
register int n, last, size;
int where;
{
	register int i;
#ifdef NBUF_TYPE
	char *save [NBUF_TYPE * MAX_ROWS];

	/* save n lines at where */
	memmove( (char *)save, (char *) &sb[NBUF_TYPE * where], NBUF_TYPE * sizeof(char *) * n);

	/* clear contents of old rows */
	for (i = NBUF_TYPE * n - 1 ; i >= 0 ; i--)
		bzero ((char *) save [i], size);

	/* move up lines */
	memmove( (char *) &sb[NBUF_TYPE * where], (char *) &sb[NBUF_TYPE * (where + n)], 
		NBUF_TYPE * sizeof (char *) * ((last -= n - 1) - where));

	/* reuse storage for new bottom lines */
	memmove( (char *) &sb[NBUF_TYPE * last], (char *)save, 
		NBUF_TYPE * sizeof(char *) * n);
#else /* NBUF_TYPE */
	char *save [2 * MAX_ROWS];

	/* save n lines at where */
	memmove( (char *)save, (char *) &sb[2 * where], 2 * sizeof(char *) * n);

	/* clear contents of old rows */
	for (i = 2 * n - 1 ; i >= 0 ; i--)
		bzero ((char *) save [i], size);

	/* move up lines */
	memmove( (char *) &sb[2 * where], (char *) &sb[2 * (where + n)], 
		2 * sizeof (char *) * ((last -= n - 1) - where));

	/* reuse storage for new bottom lines */
	memmove( (char *) &sb[2 * last], (char *)save, 
		2 * sizeof(char *) * n);
#endif /* NBUF_TYPE */
}


ScrnInsertChar (sb, row, col, n, size)
    /*
      Inserts n blanks in sb at row, col.  Size is the size of each row.
      */
    ScrnBuf sb;
    int row, size;
    register int col, n;
{
	register int i, j;
#ifdef NBUF_TYPE
	register Char *ptr = sb [NBUF_TYPE * row + CODE_BUF];
	register Char *attrs = sb [NBUF_TYPE * row + ATTR_BUF];
#ifdef MULTIFONT
	register Char *fnum = sb [NBUF_TYPE * row + FNUM_BUF];
#endif /* MULTIFONT */
#ifdef COLOR
	register Char *color = sb [NBUF_TYPE * row + COLOR_BUF];
#endif /* COLOR */
#else /* NBUF_TYPE */
	register Char *ptr = sb [2 * row];
	register Char *attrs = sb [2 * row + 1];
#endif /* NBUF_TYPE */
	int wrappedbit = attrs[0]&LINEWRAPPED;

#ifdef MULTIFONT
	if (attrs[col] & SECOND) {
		ptr[col - 1] = 0;
		attrs[col - 1] = 0;
		fnum[col - 1] = 0;
#ifdef COLOR
		color[col - 1] = 0;
#endif /* COLOR */
	}
#endif /* MULTIFONT */
	attrs[0] &= ~LINEWRAPPED; /* make sure the bit isn't moved */
	for (i = size - 1; i >= col + n; i--) {
		ptr[i] = ptr[j = i - n];
		attrs[i] = attrs[j];
#ifdef MULTIFONT
		fnum[i] = fnum[j];
#endif /* MULTIFONT */
#ifdef COLOR
		color[i] = color[j];
#endif /* COLOR */
	}

	for (i=col; i<col+n; i++)
	    ptr[i] = ' ';
	for (i=col; i<col+n; i++)
	    attrs[i] = CHARDRAWN;
#ifdef MULTIFONT
	bzero(fnum + col, n);
#endif /* MULTIFONT */
#ifdef COLOR
	bzero(color + col, n);
#endif /* COLOR */

	if (wrappedbit)
	    attrs[0] |= LINEWRAPPED;
}


ScrnDeleteChar (sb, row, col, n, size)
    /*
      Deletes n characters in sb at row, col. Size is the size of each row.
      */
    ScrnBuf sb;
    register int row, size;
    register int n, col;
{
#ifdef NBUF_TYPE
	register Char *ptr = sb[NBUF_TYPE * row + CODE_BUF];
	register Char *attrs = sb[NBUF_TYPE * row + ATTR_BUF];
#ifdef MULTIFONT
	register Char *fnum = sb[NBUF_TYPE * row + FNUM_BUF];
#endif /* MULTIFONT */
#ifdef COLOR
	register Char *color = sb[NBUF_TYPE * row + COLOR_BUF];
#endif /* COLOR */
#else /* NBUF_TYPE */
	register Char *ptr = sb[2 * row];
	register Char *attrs = sb[2 * row + 1];
#endif /* NBUF_TYPE */
	register nbytes = (size - n - col);
	int wrappedbit = attrs[0]&LINEWRAPPED;

#ifdef MULTIFONT
	if (attrs[col] & FIRST) {
		ptr[col + 1] = 0;
		attrs[col + 1] = 0;
		fnum[col + 1] = 0;
#ifdef COLOR
		color[col + 1] = 0;
#endif /* COLOR */
	}
#endif /* MULTIFONT */
	memmove( ptr + col, ptr + col + n, nbytes);
	memmove( attrs + col, attrs + col + n, nbytes);
#ifdef MULTIFONT
	memmove( fnum + col, fnum + col + n, nbytes);
#endif /* MULTIFONT */
#ifdef COLOR
	memmove( color + col, color + col + n, nbytes);
#endif /* COLOR */
	bzero (ptr + size - n, n);
	bzero (attrs + size - n, n);
#ifdef MULTIFONT
	bzero (fnum + size - n, n);
#endif /* MULTIFONT */
#ifdef COLOR
	bzero (color + size - n, n);
#endif /* COLOR */
	if (wrappedbit)
	    attrs[0] |= LINEWRAPPED;
}

#ifdef STATUSLINE
#define Reverse(screen) \
    (((!hilite && (flags & INVERSE) != 0) || \
      (hilite && (flags & INVERSE) == 0)) ^ \
     (dostatus < 0 && screen->reversestatus))
#else /* STATUSLINE */
#define Reverse(screen) \
    ((!hilite && (flags & INVERSE) != 0) || \
     (hilite && (flags & INVERSE) == 0))
#endif /* STATUSLINE */

#ifdef COLOR
#ifdef MULTIFONT
#define GetColor(screen, cc, fg, bg) \
    if (cc & 0xf) { \
	xgc.fg = screen->color[cc & 0xf]; \
	rxgc.bg = screen->color[cc & 0xf]; \
    } else { \
	xgc.fg = screen->foreground; \
	rxgc.bg = screen->foreground; \
    } \
    if (cc & 0xf0) { \
	xgc.bg = screen->color[(cc & 0xf0) >> 4]; \
	rxgc.fg = screen->color[(cc & 0xf0) >> 4]; \
    } else { \
	xgc.bg = term->core.background_pixel; \
	rxgc.fg = term->core.background_pixel; \
    }
#define SetColor(screen) \
    if (gc) { \
	XChangeGC(screen->display, gc, GCForeground|GCBackground, &xgc); \
	XChangeGC(screen->display, rgc, GCForeground|GCBackground, &rxgc); \
    }
#else /* MULTIFONT */
#define GetColor(screen, cc, fg, bg) \
    if (cc & 0xf) { \
	xgc.fg = screen->color[cc & 0xf]; \
    } else { \
	xgc.fg = screen->foreground; \
    } \
    if (cc & 0xf0) { \
	xgc.bg = screen->color[(cc & 0xf0) >> 4]; \
    } else { \
	xgc.bg = term->core.background_pixel; \
    }
#define SetColor(screen) \
    XChangeGC(screen->display, gc, GCForeground|GCBackground, &xgc);
#endif /* MULTIFONT */
#else /* COLOR */
#define GetColor(screen, cc, fg, bg) /**/
#define SetColor(screen, cc, fg, bg) /**/
#endif /* COLOR */

#if __STDC__
#define Concat(a,b)a##b
#else
#define Concat(a,b)a/**/b
#endif
#ifdef MULTIFONT
#define GetGC(screen, n, r) \
    if (flags & BOLD) { \
	gc = screen->Concat(n,boldGC); \
	rgc = screen->Concat(r,boldGC); \
    } else { \
	gc = screen->Concat(n,GC); \
	rgc = screen->Concat(r,GC); \
    }
#define DrawText(screen, x, y, col, lastind) \
    if (gc) { \
	n = col - lastind; \
	if (screen->top_space) { \
	    XFillRectangle(screen->display, TextWindow(screen), \
			    rgc, x, \
			    y - screen->top_space - screen->max_ascent, \
			    n * FontWidth(screen), \
			    screen->top_space); \
	} \
	if (flags & MBSCS) { \
	    XDrawImageString16(screen->display, TextWindow(screen), \
			gc, x, y, (XChar2b *) &chars[lastind], n / 2); \
	    if((flags & BOLD) && screen->enbolden) \
		XDrawString16(screen->display, TextWindow(screen), \
		 gc, x + 1, y, (XChar2b *) &chars[lastind], n); \
	} else { \
	    XDrawImageString(screen->display, TextWindow(screen), \
			gc, x, y, (char *) &chars[lastind], n); \
	    if((flags & BOLD) && screen->enbolden) \
		XDrawString(screen->display, TextWindow(screen), \
		 gc, x + 1, y, (char *) &chars[lastind], n); \
	} \
	if (screen->bottom_space) { \
	    XFillRectangle(screen->display, TextWindow(screen), \
			    rgc, x, y + screen->max_descent, \
			    n * FontWidth(screen), screen->bottom_space); \
	} \
	if (flags & UNDERLINE) { \
	    XDrawLine(screen->display, TextWindow(screen), gc, \
	     x, y+screen->ul_position, x+n*FontWidth(screen), y+screen->ul_position); \
	} \
    }
#else /* MULTIFONT */
#define GetGC(screen, n, r) \
    if (flags & BOLD) { \
	gc = screen->Concat(n,boldGC); \
    } else { \
	gc = screen->Concat(n,GC); \
    }
#endif /* MULTIFONT */

#if defined(COLOR) || defined(STATUSLINE) || defined(MULTIFONT)
#define SetupGC(screen) \
    if (Reverse(screen)) { \
	GetGC(screen, reverse, normal) \
	GetColor(screen, cc, background, foreground) \
    } else { \
	GetGC(screen, normal, reverse) \
	GetColor(screen, cc, foreground, background) \
    } \
    SetColor(screen)
#endif /* COLOR || STATUSLINE || MULTIFONT */

ScrnRefresh (screen, toprow, leftcol, nrows, ncols, force)
/*
   Repaints the area enclosed by the parameters.
   Requires: (toprow, leftcol), (toprow + nrows, leftcol + ncols) are
   	     coordinates of characters in screen;
	     nrows and ncols positive.
 */
register TScreen *screen;
int toprow, leftcol, nrows, ncols;
Boolean force;			/* ... leading/trailing spaces */
{
	int y = toprow * FontHeight(screen) + screen->border +
#ifdef MULTIFONT
		screen->top_space + screen->max_ascent;
#else
		screen->fnt_norm->ascent;
#endif /* MULTIFONT */
	register int row;
	register int topline = screen->topline;
	int maxrow = toprow + nrows - 1;
	int scrollamt = screen->scroll_amt;
	int max = screen->max_row;
#ifdef STATUSLINE
	int dostatus = 0, left, width;
#endif /* STATUSLINE */
#ifdef MULTIFONT
	int fset;
#endif /* MULTIFONT */

#ifdef STATUSLINE
	if (screen->statusheight && maxrow == screen->max_row + 1) {
		dostatus++;
		maxrow--;
	}
#endif /* STATUSLINE */
	if(screen->cursor_col >= leftcol && screen->cursor_col <=
	 (leftcol + ncols - 1) && screen->cursor_row >= toprow + topline &&
	 screen->cursor_row <= maxrow + topline)
		screen->cursor_state = OFF;
#ifdef STATUSLINE
    for (;;) {
#endif /* STATUSLINE */
	for (row = toprow; row <= maxrow; y += FontHeight(screen), row++) {
	   register Char *chars;
	   register Char *attrs;
	   register int col = leftcol;
	   int maxcol = leftcol + ncols - 1;
	   int lastind;
	   int flags;
	   int x, n;
	   GC gc;
	   Boolean hilite;	
#ifdef MULTIFONT
	   register Char *fnum;
	   Char fset;
#endif /* MULTIFONT */
#ifdef COLOR
	   register Char *color;
	   Char cc;
	   XGCValues xgc;
#ifdef MULTIFONT
	   GC rgc;
	   XGCValues rxgc;
#endif /* MULTIFONT */
#endif /* COLOR */

	   if (row < screen->top_marg || row > screen->bot_marg)
		lastind = row;
	   else
		lastind = row - scrollamt;

	   if (lastind < 0 || lastind > max)
	   	continue;

#ifdef NBUF_TYPE
	   chars = screen->buf [NBUF_TYPE * (lastind + topline) + CODE_BUF];
	   attrs = screen->buf [NBUF_TYPE * (lastind + topline) + ATTR_BUF];
#ifdef MULTIFONT
	   fnum = screen->buf [NBUF_TYPE * (lastind + topline) + FNUM_BUF];
#endif /* MULTIFONT */
#ifdef COLOR
	   color = screen->buf [NBUF_TYPE * (lastind + topline) + COLOR_BUF];
#endif /* COLOR */
#else /* NBUF_TYPE */
	   chars = screen->buf [2 * (lastind + topline)];
	   attrs = screen->buf [2 * (lastind + topline) + 1];
#endif /* NBUF_TYPE */

	   if (row < screen->startHRow || row > screen->endHRow ||
	       (row == screen->startHRow && maxcol < screen->startHCol) ||
	       (row == screen->endHRow && col >= screen->endHCol))
	       {
	       /* row does not intersect selection; don't hilite */
	       if (!force) {
		   while (col <= maxcol && (attrs[col] & ~BOLD) == 0 &&
#ifdef COLOR
			  (color[col] == 0) &&
#endif /* COLOR */
#ifdef MULTIFONT
			  (chars[col] & ~140) == 0)
#else
			  (chars[col] & ~040) == 0)
#endif /* MULTIFONT */
		       col++;

		   while (col <= maxcol && (attrs[maxcol] & ~BOLD) == 0 &&
#ifdef COLOR
			  (color[maxcol] == 0) &&
#endif /* COLOR */
#ifdef MULTIFONT
			  (chars[maxcol] & ~140) == 0)
#else /* MULTIFONT */
			  (chars[maxcol] & ~040) == 0)
#endif /* MULTIFONT */
		       maxcol--;
	       }
	       hilite = False;
	   }
	   else {
	       /* row intersects selection; split into pieces of single type */
	       if (row == screen->startHRow && col < screen->startHCol) {
		   ScrnRefresh(screen, row, col, 1, screen->startHCol - col,
			       force);
		   col = screen->startHCol;
	       }
	       if (row == screen->endHRow && maxcol >= screen->endHCol) {
		   ScrnRefresh(screen, row, screen->endHCol, 1,
			       maxcol - screen->endHCol + 1, force);
		   maxcol = screen->endHCol - 1;
	       }
	       /* remaining piece should be hilited */
	       hilite = True;
	   }
#ifdef MULTIFONT
	   if (attrs[maxcol] & FIRST) maxcol++;
#endif /* MULTIFONT */

	   if (col > maxcol) continue;

	   flags = attrs[col];
#ifdef MULTIFONT
	   fset = fnum[col];
#endif /* MULTIFONT */
#ifdef COLOR
	   cc = color[col];
#endif /* COLOR */

#ifdef MULTIFONT
	if (!screen->fnt_norm) load_codeset_font(screen, fset);
#endif /* MULTIFONT */
#ifdef SetupGC
	   SetupGC(screen)
#else /* SetupGC */
	   if ( (!hilite && (flags & INVERSE) != 0) ||
	        (hilite && (flags & INVERSE) == 0) )
	       if (flags & BOLD) gc = screen->reverseboldGC;
	       else gc = screen->reverseGC;
	   else 
	       if (flags & BOLD) gc = screen->normalboldGC;
	       else gc = screen->normalGC;
#endif /* SetupGC */

	   x = CursorX(screen, col);
	   lastind = col;

	   for (; col <= maxcol; col++) {
#ifdef NBUF_TYPE
		if (attrs[col] != flags
#ifdef MULTIFONT
		    || fnum[col] != fset
#endif /* MULTIFONT */
#ifdef COLOR
		    || color[col] != cc
#endif /* COLOR */
		    ) {
#else /* NBUF_TYPE */
		if (attrs[col] != flags) {
#endif /* NBUF_TYPE */
#ifdef DrawText
		   DrawText(screen, x, y, col, lastind)
#else /* DrawText */
		   XDrawImageString(screen->display, TextWindow(screen), 
		        	gc, x, y, (char *) &chars[lastind], n = col - lastind);
		   if((flags & BOLD) && screen->enbolden)
		 	XDrawString(screen->display, TextWindow(screen), 
			 gc, x + 1, y, (char *) &chars[lastind], n);
		   if(flags & UNDERLINE) 
			XDrawLine(screen->display, TextWindow(screen), 
			 gc, x, y+1, x+n*FontWidth(screen), y+1);
#endif /* DrawText */

		   x += (col - lastind) * FontWidth(screen);

		   lastind = col;

		   flags = attrs[col];
#ifdef MULTIFONT
		   fset = fnum[col];
#endif /* MULTIFONT */
#ifdef COLOR
		   cc = color[col];
#endif /* COLOR */

#ifdef MULTIFONT
		   if (!screen->fnt_norm) load_codeset_font(screen, fset);
#endif /* MULTIFONT */
#ifdef SetupGC
		   SetupGC(screen)
#else /* SetupGC */
	   	   if ((!hilite && (flags & INVERSE) != 0) ||
		       (hilite && (flags & INVERSE) == 0) )
	       		if (flags & BOLD) gc = screen->reverseboldGC;
	       		else gc = screen->reverseGC;
	  	    else 
	      		 if (flags & BOLD) gc = screen->normalboldGC;
	      		 else gc = screen->normalGC;
#endif /* SetupGC */
		}

		if(chars[col] == 0)
			chars[col] = ' ';
#ifdef MULTIFONT
		if(attrs[col] & FIRST) {
			col++;
		}
#endif /* MULTIFONT */
	   }


#ifdef MULTIFONT
	   if (!screen->fnt_norm) load_codeset_font(screen, fset);
#endif /* MULTIFONT */
#ifdef SetupGC
	   SetupGC(screen)
#else /* SetupGC */
	   if ( (!hilite && (flags & INVERSE) != 0) ||
	        (hilite && (flags & INVERSE) == 0) )
	       if (flags & BOLD) gc = screen->reverseboldGC;
	       else gc = screen->reverseGC;
	   else 
	       if (flags & BOLD) gc = screen->normalboldGC;
	       else gc = screen->normalGC;
#endif /* SetupGC */
#ifdef DrawText
	   DrawText(screen, x, y, col, lastind)
#else /* DrawText */
	   XDrawImageString(screen->display, TextWindow(screen), gc, 
	         x, y, (char *) &chars[lastind], n = col - lastind);
	   if((flags & BOLD) && screen->enbolden)
		XDrawString(screen->display, TextWindow(screen), gc,
		x + 1, y, (char *) &chars[lastind], n);
	   if(flags & UNDERLINE) 
		XDrawLine(screen->display, TextWindow(screen), gc, 
		 x, y+1, x + n * FontWidth(screen), y+1);
#endif /* DrawText */
	}
#ifdef STATUSLINE
	if (dostatus <= 0)
	    break;
#ifdef MULTIFONT
	fset = 0;
#endif /* MULTIFONT */
	dostatus = -1;
	topline = 0;
	scrollamt = 0;
	toprow = maxrow = max = screen->max_row + 1;
	left = CursorX(screen, leftcol);
	width = ncols * FontWidth(screen);
	if (leftcol == 0) {
	    left = screen->scrollbar;
	    width += screen->border;
	}
	if (leftcol + ncols - 1 >= screen->max_col)
	    width += screen->border;
	y += screen->border + 1;
	XFillRectangle(screen->display, TextWindow(screen),
		screen->reversestatus ?
			screen->normalGC: screen->reverseGC,
		left,
#ifdef MULTIFONT
		y - screen->top_space - screen->max_ascent - 1,
#else /* MULTIFONT */
		y - screen->fnt_norm->ascent - 1,
#endif /* MULTIFONT */
		width, screen->statusheight);
	if (!screen->reversestatus) {
		StatusBox(screen);
	}
    }
#endif /* STATUSLINE */
}

ClearBufRows (screen, first, last)
/*
   Sets the rows first though last of the buffer of screen to spaces.
   Requires first <= last; first, last are rows of screen->buf.
 */
register TScreen *screen;
register int first, last;
{
#ifdef NBUF_TYPE
	first *= NBUF_TYPE;
	last = NBUF_TYPE * last + NBUF_TYPE - 1;
#else /* NBUF_TYPE */
	first *= 2;
	last = 2 * last + 1;
#endif /* NBUF_TYPE */
	while (first <= last)
		bzero (screen->buf [first++], (screen->max_col + 1));
}

/*
  Resizes screen:
  1. If new window would have fractional characters, sets window size so as to
  discard fractional characters and returns -1.
  Minimum screen size is 1 X 1.
  Note that this causes another ExposeWindow event.
  2. Enlarges screen->buf if necessary.  New space is appended to the bottom
  and to the right
  3. Reduces  screen->buf if necessary.  Old space is removed from the bottom
  and from the right
  4. Cursor is positioned as closely to its former position as possible
  5. Sets screen->max_row and screen->max_col to reflect new size
  6. Maintains the inner border (and clears the border on the screen).
  7. Clears origin mode and sets scrolling region to be entire screen.
  8. Returns 0
  */
ScreenResize (screen, width, height, flags)
    register TScreen *screen;
    int width, height;
    unsigned *flags;
{
	int rows, cols;
	int border = 2 * screen->border;
	int move_down_by;
#if defined(sun) && !defined(SVR4)
#ifdef TIOCSSIZE
	struct ttysize ts;
#endif	/* TIOCSSIZE */
#else	/* sun */
#ifdef TIOCSWINSZ
	struct winsize ws;
#endif	/* TIOCSWINSZ */
#endif	/* sun */
	Window tw = TextWindow (screen);

	/* clear the right and bottom internal border because of NorthWest
	   gravity might have left junk on the right and bottom edges */
	XClearArea (screen->display, tw,
#ifdef StatusHeight
		    width - screen->border, StatusHeight(screen),
#else /* StatusHeight */
		    width - screen->border, 0,                /* right edge */
#endif /* StatusHeight */
		    screen->border, height,           /* from top to bottom */
		    False);
	XClearArea (screen->display, tw, 
#ifdef StatusHeight
		    0, height - screen->border + StatusHeight(screen),
#else /* StatusHeight */
		    0, height - screen->border,	                  /* bottom */
#endif /* StatusHeight */
		    width, screen->border,         /* all across the bottom */
		    False);

	/* round so that it is unlikely the screen will change size on  */
	/* small mouse movements.					*/
#ifdef STATUSLINE
	rows = (height + FontHeight(screen) / 2 - border - StatusHeight(screen)) /
#else /* STATUSLINE */
	rows = (height + FontHeight(screen) / 2 - border) /
#endif /* STATUSLINE */
	 FontHeight(screen);
	cols = (width + FontWidth(screen) / 2 - border - screen->scrollbar) /
	 FontWidth(screen);
	if (rows < 1) rows = 1;
	if (cols < 1) cols = 1;
#ifdef StatusHeight
	if (screen->max_row < rows - 1) {
	    XClearArea (screen->display, tw, 
			screen->scrollbar,
			(screen->max_row + 1) * FontHeight(screen) +
			screen->border * 2 + StatusHeight(screen),
			(screen->max_col + 1) * FontWidth(screen) +
			screen->border * 2,
			0,
			False);
	} else if (screen->max_row > rows - 1) {
	    XClearArea (screen->display, tw, 
			screen->scrollbar,
			rows * FontHeight(screen) +
			screen->border * 2 + StatusHeight(screen),
			cols * FontWidth(screen) + screen->border * 2,
			0,
			True);
	} else if (screen->max_col < cols - 1) {
	    XClearArea (screen->display, tw, 
			screen->scrollbar +
			(screen->max_col + 1) * FontWidth(screen) +
			screen->border,
			(screen->max_row + 1) * FontHeight(screen) +
			screen->border * 2 + StatusHeight(screen),
			screen->border,
			0,
			False);
	} else if (screen->max_col > cols - 1) {
	    XClearArea (screen->display, tw, 
			screen->scrollbar +
			cols * FontWidth(screen) + screen->border - 1,
			(screen->max_row + 1) * FontHeight(screen) +
			screen->border * 2 + StatusHeight(screen),
			screen->border + 1,
			0,
			True);
	}
#endif /* StatusHeight */

	/* update buffers if the screen has changed size */
	if (screen->max_row != rows - 1 || screen->max_col != cols - 1) {
		register int savelines = screen->scrollWidget ?
		 screen->savelines : 0;
		int delta_rows = rows - (screen->max_row + 1);
		
		if(screen->cursor_state)
			HideCursor();
		if ( screen->alternate
		     && term->misc.resizeGravity == SouthWestGravity )
		    /* swap buffer pointers back to make all this hair work */
		    SwitchBufPtrs(screen);
		if (screen->altbuf) 
		    (void) Reallocate(&screen->altbuf, (Char **)&screen->abuf_address,
			 rows, cols, screen->max_row + 1, screen->max_col + 1);
		move_down_by = Reallocate(&screen->allbuf,
					  (Char **)&screen->sbuf_address,
					  rows + savelines, cols,
					  screen->max_row + 1 + savelines,
					  screen->max_col + 1);
#ifdef NBUF_TYPE
		screen->buf = &screen->allbuf[NBUF_TYPE * savelines];
#else /* NBUF_TYPE */
		screen->buf = &screen->allbuf[2 * savelines];
#endif /* NBUF_TYPE */

		screen->max_row += delta_rows;
		screen->max_col = cols - 1;

		if (term->misc.resizeGravity == SouthWestGravity) {
		    screen->savedlines -= move_down_by;
		    if (screen->savedlines < 0)
			screen->savedlines = 0;
		    if (screen->savedlines > screen->savelines)
			screen->savedlines = screen->savelines;
		    if (screen->topline < -screen->savedlines)
			screen->topline = -screen->savedlines;
		    screen->cur_row += move_down_by;
		    screen->cursor_row += move_down_by;
		    ScrollSelection(screen, move_down_by);

		    if (screen->alternate)
			SwitchBufPtrs(screen); /* put the pointers back */
		}
	
		/* adjust scrolling region */
		screen->top_marg = 0;
		screen->bot_marg = screen->max_row;
		*flags &= ~ORIGIN;

#ifdef STATUSLINE
		if (screen->instatus)
			screen->cur_row = screen->max_row + 1;
		else
#endif /* STATUSLINE */
		if (screen->cur_row > screen->max_row)
			screen->cur_row = screen->max_row;
		if (screen->cur_col > screen->max_col)
			screen->cur_col = screen->max_col;
	
#ifdef StatusHeight
		screen->fullVwin.height = height - border - StatusHeight(screen);
#else /* StatusHeight */
		screen->fullVwin.height = height - border;
#endif /* StatusHeight */
		screen->fullVwin.width = width - border - screen->scrollbar;

	} else if(FullHeight(screen) == height && FullWidth(screen) == width)
	 	return(0);	/* nothing has changed at all */

	if(screen->scrollWidget)
		ResizeScrollBar(screen->scrollWidget, -1, -1, height);
	
	screen->fullVwin.fullheight = height;
	screen->fullVwin.fullwidth = width;
	ResizeSelection (screen, rows, cols);
#if defined(sun) && !defined(SVR4)
#ifdef TIOCSSIZE
	/* Set tty's idea of window size */
	ts.ts_lines = rows;
	ts.ts_cols = cols;
	ioctl (screen->respond, TIOCSSIZE, &ts);
#ifdef SIGWINCH
	if(screen->pid > 1) {
		int	pgrp;
		
		if (ioctl (screen->respond, TIOCGPGRP, &pgrp) != -1)
			kill_process_group(pgrp, SIGWINCH);
	}
#endif	/* SIGWINCH */
#endif	/* TIOCSSIZE */
#else	/* sun */
#ifdef TIOCSWINSZ
	/* Set tty's idea of window size */
	ws.ws_row = rows;
	ws.ws_col = cols;
	ws.ws_xpixel = width;
	ws.ws_ypixel = height;
	ioctl (screen->respond, TIOCSWINSZ, (char *)&ws);
#ifdef notdef	/* change to SIGWINCH if this doesn't work for you */
	if(screen->pid > 1) {
		int	pgrp;
		
		if (ioctl (screen->respond, TIOCGPGRP, &pgrp) != -1)
		    kill_process_group(pgrp, SIGWINCH);
	}
#endif	/* SIGWINCH */
#endif	/* TIOCSWINSZ */
#endif	/* sun */
#ifdef USE_XIM
	if (screen->ic)
	    SetStatus(screen);
#endif /* USE_XIM */
	return (0);
}

/*
 * Sets the attributes from the row, col, to row, col + length according to
 * mask and value. The bits in the attribute byte specified by the mask are
 * set to the corresponding bits in the value byte. If length would carry us
 * over the end of the line, it stops at the end of the line.
 */
void
ScrnSetAttributes(screen, row, col, mask, value, length)
TScreen *screen;
int row, col;
unsigned mask, value;
register int length;		/* length of string */
{
	register Char *attrs;
	register int avail  = screen->max_col - col + 1;

	if (length > avail)
	    length = avail;
	if (length <= 0)
		return;
#ifdef NBUF_TYPE
	attrs = screen->buf[NBUF_TYPE * row + ATTR_BUF] + col;
#else /* NBUF_TYPE */
	attrs = screen->buf[2 * row + 1] + col;
#endif /* NBUF_TYPE */
	value &= mask;	/* make sure we only change the bits allowed by mask*/
	while(length-- > 0) {
		*attrs &= ~mask;	/* clear the bits */
		*attrs |= value;	/* copy in the new values */
		attrs++;
	}
}

/*
 * Gets the attributes from the row, col, to row, col + length into the
 * supplied array, which is assumed to be big enough.  If length would carry us
 * over the end of the line, it stops at the end of the line. Returns
 * the number of bytes of attributes (<= length)
 */
int
ScrnGetAttributes(screen, row, col, str, length)
TScreen *screen;
int row, col;
Char *str;
register int length;		/* length of string */
{
	register Char *attrs;
	register int avail  = screen->max_col - col + 1;
	int ret;

	if (length > avail)
	    length = avail;
	if (length <= 0)
		return 0;
	ret = length;
#ifdef NBUF_TYPE
	attrs = screen->buf[NBUF_TYPE * row + ATTR_BUF] + col;
#else /* NBUF_TYPE */
	attrs = screen->buf[2 * row + 1] + col;
#endif /* NBUF_TYPE */
	while(length-- > 0) {
		*str++ = *attrs++;
	}
	return ret;
}
Bool
non_blank_line(sb, row, col, len)
ScrnBuf sb;
register int row, col, len;
{
	register int	i;
#ifdef NBUF_TYPE
	register Char *ptr = sb [NBUF_TYPE * row];
#else /* NBUF_TYPE */
	register Char *ptr = sb [2 * row];
#endif /* NBUF_TYPE */

	for (i = col; i < len; i++)	{
		if (ptr[i])
			return True;
	}
	return False;
}
