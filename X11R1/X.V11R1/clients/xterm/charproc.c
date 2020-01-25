
/*
 *	$Source: /orpheus/u1/X11/clients/xterm/RCS/charproc.c,v $
 *	$Header: charproc.c,v 1.36 87/09/09 16:40:30 swick Exp $
 */


#include <X11/copyright.h>

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


/* charproc.c */

#include <stdio.h>
#include <sgtty.h>
#include <ctype.h>
#include <errno.h>
#include <setjmp.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Intrinsic.h>
#include <X11/Xatom.h>
#include "ptyx.h"
#include "VTparse.h"
#include "data.h"
#include "error.h"
#ifdef MODEMENU
#include "menu.h"
#endif MODEMENU

extern void exit(), bcopy();

#define	DEFAULT		-1
#define	TEXT_BUF_SIZE	256
#define TRACKTIMESEC	4L
#define TRACKTIMEUSEC	0L

#define	doinput()		(bcnt-- > 0 ? *bptr++ : in_put())

#ifndef lint
static char rcs_id[] = "$Header: charproc.c,v 1.36 87/09/09 16:40:30 swick Exp $";
#endif lint

static long arg;
static int nparam;
static ANSI reply;
static int param[NPARAM];

static unsigned long ctotal;
static unsigned long ntotal;
static jmp_buf vtjmpbuf;

extern XrmNameList nameList;
extern XrmClassList classList;

extern int groundtable[];
extern int csitable[];
extern int dectable[];
extern int eigtable[];
extern int esctable[];
extern int iestable[];
extern int igntable[];
extern int scrtable[];
extern int scstable[];


/* event handlers */
extern XtEventReturnCode HandleKeyPressed();
extern XtEventReturnCode EventDoNothing();
extern XtEventReturnCode HandleEnterWindow();
extern XtEventReturnCode HandleLeaveWindow();
extern XtEventReturnCode HandleFocusChange();
extern XtEventReturnCode VTButtonPressed();
extern XtEventReturnCode VTMouseMoved();
extern XtEventReturnCode VTButtonReleased();


VTparse()
{
	register TScreen *screen = &term.screen;
	register int *parsestate = groundtable;
	register int c;
	register char *cp;
	register int row, col, top, bot, scstype;
	extern int bitset(), bitclr(), finput(), TrackMouse();

	if(setjmp(vtjmpbuf))
		parsestate = groundtable;
	for( ; ; )
		switch(parsestate[c = doinput()]) {
		 case CASE_GROUND_STATE:
			/* exit ignore mode */
			parsestate = groundtable;
			break;

		 case CASE_IGNORE_STATE:
			/* Ies: ignore anything else */
			parsestate = igntable;
			break;

		 case CASE_IGNORE_ESC:
			/* Ign: escape */
			parsestate = iestable;
			break;

		 case CASE_IGNORE:
			/* Ignore character */
			break;

		 case CASE_BELL:
			/* bell */
			Bell();
			break;

		 case CASE_BS:
			/* backspace */
			CursorBack(screen, 1);
			break;

		 case CASE_CR:
			/* carriage return */
			CarriageReturn(screen);
			break;

		 case CASE_ESC:
			/* escape */
			parsestate = esctable;
			break;

		 case CASE_VMOT:
			/*
			 * form feed, line feed, vertical tab
			 */
			Index(screen, 1);
			if (term.flags & LINEFEED)
				CarriageReturn(screen);
			if(screen->display->qlen > 0 ||
			 (ioctl(screen->display->fd, FIONREAD, (char *) &arg), arg) > 0)
				xevents();
			break;

		 case CASE_TAB:
			/* tab */
			screen->cur_col = TabNext(term.tabs, screen->cur_col);
			if (screen->cur_col > screen->max_col)
				screen->cur_col = screen->max_col;
			break;

		 case CASE_SI:
			screen->curgl = 0;
			break;

		 case CASE_SO:
			screen->curgl = 1;
			break;

		 case CASE_SCR_STATE:
			/* enter scr state */
			parsestate = scrtable;
			break;

		 case CASE_SCS0_STATE:
			/* enter scs state 0 */
			scstype = 0;
			parsestate = scstable;
			break;

		 case CASE_SCS1_STATE:
			/* enter scs state 1 */
			scstype = 1;
			parsestate = scstable;
			break;

		 case CASE_SCS2_STATE:
			/* enter scs state 2 */
			scstype = 2;
			parsestate = scstable;
			break;

		 case CASE_SCS3_STATE:
			/* enter scs state 3 */
			scstype = 3;
			parsestate = scstable;
			break;

		 case CASE_ESC_IGNORE:
			/* unknown escape sequence */
			parsestate = eigtable;
			break;

		 case CASE_ESC_DIGIT:
			/* digit in csi or dec mode */
			if((row = param[nparam - 1]) == DEFAULT)
				row = 0;
			param[nparam - 1] = 10 * row + (c - '0');
			break;

		 case CASE_ESC_SEMI:
			/* semicolon in csi or dec mode */
			param[nparam++] = DEFAULT;
			break;

		 case CASE_DEC_STATE:
			/* enter dec mode */
			parsestate = dectable;
			break;

		 case CASE_ICH:
			/* ICH */
			if((c = param[0]) < 1)
				c = 1;
			InsertChar(screen, c);
			parsestate = groundtable;
			break;

		 case CASE_CUU:
			/* CUU */
			if((c = param[0]) < 1)
				c = 1;
			CursorUp(screen, c);
			parsestate = groundtable;
			break;

		 case CASE_CUD:
			/* CUD */
			if((c = param[0]) < 1)
				c = 1;
			CursorDown(screen, c);
			parsestate = groundtable;
			break;

		 case CASE_CUF:
			/* CUF */
			if((c = param[0]) < 1)
				c = 1;
			CursorForward(screen, c);
			parsestate = groundtable;
			break;

		 case CASE_CUB:
			/* CUB */
			if((c = param[0]) < 1)
				c = 1;
			CursorBack(screen, c);
			parsestate = groundtable;
			break;

		 case CASE_CUP:
			/* CUP | HVP */
			if((row = param[0]) < 1)
				row = 1;
			if(nparam < 2 || (col = param[1]) < 1)
				col = 1;
			CursorSet(screen, row-1, col-1, term.flags);
			parsestate = groundtable;
			break;

		 case CASE_ED:
			/* ED */
			switch (param[0]) {
			 case DEFAULT:
			 case 0:
				ClearBelow(screen);
				break;

			 case 1:
				ClearAbove(screen);
				break;

			 case 2:
				ClearScreen(screen);
				break;
			}
			parsestate = groundtable;
			break;

		 case CASE_EL:
			/* EL */
			switch (param[0]) {
			 case DEFAULT:
			 case 0:
				ClearRight(screen);
				break;
			 case 1:
				ClearLeft(screen);
				break;
			 case 2:
				ClearLine(screen);
				break;
			}
			parsestate = groundtable;
			break;

		 case CASE_IL:
			/* IL */
			if((c = param[0]) < 1)
				c = 1;
			InsertLine(screen, c);
			parsestate = groundtable;
			break;

		 case CASE_DL:
			/* DL */
			if((c = param[0]) < 1)
				c = 1;
			DeleteLine(screen, c);
			parsestate = groundtable;
			break;

		 case CASE_DCH:
			/* DCH */
			if((c = param[0]) < 1)
				c = 1;
			DeleteChar(screen, c);
			parsestate = groundtable;
			break;

		 case CASE_TRACK_MOUSE:
		 	/* Track mouse as long as in window and between
			   specified rows */
			TrackMouse(param[0], param[2]-1, param[1]-1,
			 param[3]-1, param[4]-2);
			break;

		 case CASE_DA1:
			/* DA1 */
			if (param[0] <= 0) {	/* less than means DEFAULT */
				reply.a_type   = CSI;
				reply.a_pintro = '?';
				reply.a_nparam = 2;
				reply.a_param[0] = 1;		/* VT102 */
				reply.a_param[1] = 2;		/* VT102 */
				reply.a_inters = 0;
				reply.a_final  = 'c';
				unparseseq(&reply, screen->respond);
			}
			parsestate = groundtable;
			break;

		 case CASE_TBC:
			/* TBC */
			if ((c = param[0]) <= 0) /* less than means default */
				TabClear(term.tabs, screen->cur_col);
			else if (c == 3)
				TabZonk(term.tabs);
			parsestate = groundtable;
			break;

		 case CASE_SET:
			/* SET */
			modes(&term, bitset);
			parsestate = groundtable;
			break;

		 case CASE_RST:
			/* RST */
			modes(&term, bitclr);
			parsestate = groundtable;
			break;

		 case CASE_SGR:
			/* SGR */
			for (c=0; c<nparam; ++c) {
				switch (param[c]) {
				 case DEFAULT:
				 case 0:
					term.flags &= ~(INVERSE|BOLD|UNDERLINE);
					break;
				 case 1:
				 case 5:	/* Blink, really.	*/
					term.flags |= BOLD;
					break;
				 case 4:	/* Underscore		*/
					term.flags |= UNDERLINE;
					break;
				 case 7:
					term.flags |= INVERSE;
				}
			}
			parsestate = groundtable;
			break;

		 case CASE_CPR:
			/* CPR */
			if ((c = param[0]) == 5) {
				reply.a_type = CSI;
				reply.a_pintro = 0;
				reply.a_nparam = 1;
				reply.a_param[0] = 0;
				reply.a_inters = 0;
				reply.a_final  = 'n';
				unparseseq(&reply, screen->respond);
			} else if (c == 6) {
				reply.a_type = CSI;
				reply.a_pintro = 0;
				reply.a_nparam = 2;
				reply.a_param[0] = screen->cur_row+1;
				reply.a_param[1] = screen->cur_col+1;
				reply.a_inters = 0;
				reply.a_final  = 'R';
				unparseseq(&reply, screen->respond);
			}
			parsestate = groundtable;
			break;

		 case CASE_DECSTBM:
			/* DECSTBM */
			if((top = param[0]) < 1)
				top = 1;
			if(nparam < 2 || (bot = param[1]) == DEFAULT
			   || bot > screen->max_row + 1
			   || bot == 0)
				bot = screen->max_row+1;
			if (bot > top) {
				if(screen->scroll_amt)
					FlushScroll(screen);
				screen->top_marg = top-1;
				screen->bot_marg = bot-1;
				CursorSet(screen, 0, 0, term.flags);
			}
			parsestate = groundtable;
			break;

		 case CASE_DECREQTPARM:
			/* DECREQTPARM */
			if ((c = param[0]) == DEFAULT)
				c = 0;
			if (c == 0 || c == 1) {
				reply.a_type = CSI;
				reply.a_pintro = 0;
				reply.a_nparam = 7;
				reply.a_param[0] = c + 2;
				reply.a_param[1] = 1;	/* no parity */
				reply.a_param[2] = 1;	/* eight bits */
				reply.a_param[3] = 112;	/* transmit 9600 baud */
				reply.a_param[4] = 112;	/* receive 9600 baud */
				reply.a_param[5] = 1;	/* clock multiplier ? */
				reply.a_param[6] = 0;	/* STP flags ? */
				reply.a_inters = 0;
				reply.a_final  = 'x';
				unparseseq(&reply, screen->respond);
			}
			parsestate = groundtable;
			break;

		 case CASE_DECSET:
			/* DECSET */
			dpmodes(&term, bitset);
			parsestate = groundtable;
			if(screen->TekEmu)
				return;
			break;

		 case CASE_DECRST:
			/* DECRST */
			dpmodes(&term, bitclr);
			parsestate = groundtable;
			break;

		 case CASE_DECALN:
			/* DECALN */
			if(screen->cursor_state)
				HideCursor();
			for(row = screen->max_row ; row >= 0 ; row--) {
				bzero(screen->buf[2 * row + 1],
				 col = screen->max_col + 1);
				for(cp = screen->buf[2 * row] ; col > 0 ; col--)
					*cp++ = 'E';
			}
			ScrnRefresh(screen, 0, 0, screen->max_row + 1,
			 screen->max_col + 1);
			parsestate = groundtable;
			break;

		 case CASE_GSETS:
			screen->gsets[scstype] = c;
			parsestate = groundtable;
			break;

		 case CASE_DECSC:
			/* DECSC */
			CursorSave(&term, &screen->sc);
			parsestate = groundtable;
			break;

		 case CASE_DECRC:
			/* DECRC */
			CursorRestore(&term, &screen->sc);
			parsestate = groundtable;
			break;

		 case CASE_DECKPAM:
			/* DECKPAM */
			term.keyboard.flags |= KYPD_APL;
			parsestate = groundtable;
			break;

		 case CASE_DECKPNM:
			/* DECKPNM */
			term.keyboard.flags &= ~KYPD_APL;
			parsestate = groundtable;
			break;

		 case CASE_IND:
			/* IND */
			Index(screen, 1);
			if(screen->display->qlen > 0 ||
			 (ioctl(screen->display->fd, FIONREAD, (char *)&arg), arg) > 0)
				xevents();
			parsestate = groundtable;
			break;

		 case CASE_NEL:
			/* NEL */
			Index(screen, 1);
			CarriageReturn(screen);
			if(screen->display->qlen > 0 ||
			 (ioctl(screen->display->fd, FIONREAD, (char *)&arg), arg) > 0)
				xevents();
			parsestate = groundtable;
			break;

		 case CASE_HTS:
			/* HTS */
			TabSet(term.tabs, screen->cur_col);
			parsestate = groundtable;
			break;

		 case CASE_RI:
			/* RI */
			RevIndex(screen, 1);
			parsestate = groundtable;
			break;

		 case CASE_SS2:
			/* SS2 */
			screen->curss = 2;
			parsestate = groundtable;
			break;

		 case CASE_SS3:
			/* SS3 */
			screen->curss = 3;
			parsestate = groundtable;
			break;

		 case CASE_CSI_STATE:
			/* enter csi state */
			nparam = 1;
			param[0] = DEFAULT;
			parsestate = csitable;
			break;

		 case CASE_OSC:
			/* do osc escapes */
			do_osc(finput);
			parsestate = groundtable;
			break;

		 case CASE_RIS:
			/* RIS */
			VTReset(TRUE);
			parsestate = groundtable;
			break;

		 case CASE_LS2:
			/* LS2 */
			screen->curgl = 2;
			parsestate = groundtable;
			break;

		 case CASE_LS3:
			/* LS3 */
			screen->curgl = 3;
			parsestate = groundtable;
			break;

		 case CASE_LS3R:
			/* LS3R */
			screen->curgr = 3;
			parsestate = groundtable;
			break;

		 case CASE_LS2R:
			/* LS2R */
			screen->curgr = 2;
			parsestate = groundtable;
			break;

		 case CASE_LS1R:
			/* LS1R */
			screen->curgr = 1;
			parsestate = groundtable;
			break;

		 case CASE_XTERM_SAVE:
			savemodes(&term);
			parsestate = groundtable;
			break;

		 case CASE_XTERM_RESTORE:
			restoremodes(&term);
			parsestate = groundtable;
			break;

		 case CASE_PRINT:
			/* printable characters */
			top = bcnt > TEXT_BUF_SIZE ? TEXT_BUF_SIZE : bcnt;
			cp = bptr;
			*--bptr = c;
			while(top > 0 && isprint(*cp)) {
				top--;
				bcnt--;
				cp++;
			}
			if(screen->curss) {
				dotext(screen, term.flags,
				 screen->gsets[screen->curss], bptr, bptr + 1);
				screen->curss = 0;
				bptr++;
			}
			if(bptr < cp)
				dotext(screen, term.flags,
				 screen->gsets[screen->curgl], bptr, cp);
			bptr = cp;
			break;
		}
}

finput()
{
	return(doinput());
}

static int select_mask;
static int write_mask;

static char v_buffer[4096];
static char *v_bufstr;
static char *v_bufptr;
static char *v_bufend;
#define	ptymask()	(v_bufptr > v_bufstr ? pty_mask : 0)

v_write(f, d, l)
int f;
char *d;
int l;
{
	int r;
	int c = l;

	if (!v_bufstr) {
		v_bufstr = v_buffer;
		v_bufptr = v_buffer;
		v_bufend = &v_buffer[4096];
	}


	if ((1 << f) != pty_mask)
		return(write(f, d, l));

	if (v_bufptr > v_bufstr) {
		if (l) {
			if (v_bufend > v_bufptr + l) {
				bcopy(d, v_bufptr, l);
				v_bufptr += l;
			} else {
				if (v_bufstr != v_buffer) {
					bcopy(v_bufstr, v_buffer,
					      v_bufptr - v_bufstr);
					v_bufptr -= v_bufstr - v_buffer;
					v_bufstr = v_buffer;
				}
				if (v_bufend > v_bufptr + l) {
					bcopy(d, v_bufptr, l);
					v_bufptr += l;
				} else if (v_bufptr < v_bufend) {
					fprintf(stderr, "Out of buffer space\n");
					c = v_bufend - v_bufptr;
					bcopy(d, v_bufptr, c);
					v_bufptr = v_bufend;
				} else {
					fprintf(stderr, "Out of buffer space\n");
					c = 0;
				}
			}
		}
		if (v_bufptr > v_bufstr) {
			if ((r = write(f, v_bufstr, v_bufptr - v_bufstr)) <= 0)
				return(r);
			if ((v_bufstr += r) >= v_bufptr)
				v_bufstr = v_bufptr = v_buffer;
		}
	} else if (l) {
		if ((r = write(f, d, l)) < 0) {
			if (errno == EWOULDBLOCK)
				r = 0;
			else if (errno == EINTR)
				r = 0;
			else
				return(r);
		}
		if (l - r) {
			if (l - r > v_bufend - v_buffer) {
				fprintf(stderr, "Truncating to %d\n",
						v_bufend - v_buffer);
				l = (v_bufend - v_buffer) + r;
			}
			bcopy(d + r, v_buffer, l - r);
			v_bufstr = v_buffer;
			v_bufptr = v_buffer + (l - r);
		}
	}
	return(c);
}

in_put()
{
	register TScreen *screen = &term.screen;
	register char *cp;
	register int i;
	static struct timeval trackTimeOut;

	select_mask = pty_mask;	/* force initial read */
	for( ; ; ) {
		if((select_mask & pty_mask) && (eventMode == NORMAL)) {
			if(screen->logging)
				FlushLog(screen);
			if((bcnt = read(screen->respond, bptr = buffer,
			 BUF_SIZE)) < 0) {
				if(errno == EIO && am_slave)
					exit(0);
				else if(errno != EWOULDBLOCK)
					Panic(
				 "input: read returned unexpected error (%d)\n",
					 errno);
			} else if(bcnt == 0)
				Panic("input: read returned zero\n", 0);
			else {
				/* strip parity bit */
				for(i = bcnt, cp = bptr ; i > 0 ; i--)
					*cp++ &= CHAR;
				if(screen->scrollWindow && screen->scrollinput &&
				 screen->topline < 0)
					/* Scroll to bottom */
					WindowScroll(screen, 0);
				break;
			}
		}
		if(screen->scroll_amt)
			FlushScroll(screen);
		if(screen->cursor_set && (screen->cursor_col != screen->cur_col
		 || screen->cursor_row != screen->cur_row)) {
			if(screen->cursor_state)
				HideCursor();
			ShowCursor();
		} else if(screen->cursor_set != screen->cursor_state) {
			if(screen->cursor_set)
				ShowCursor();
			else
				HideCursor();
		}
		
	if (waitingForTrackInfo) {
			trackTimeOut.tv_sec = TRACKTIMESEC;
			trackTimeOut.tv_usec = TRACKTIMEUSEC;
			select_mask = pty_mask;
			if ((i = select(max_plus1, &select_mask, (int *)NULL, (int *)NULL,
			 &trackTimeOut)) < 0) {
			 	if (errno != EINTR)
					SysError(ERROR_SELECT);
				continue;
			} else if (i == 0) {
				/* emacs just isn't replying, go on */
				waitingForTrackInfo = 0;
				Bell();
				select_mask = Select_mask;
			}
		} else if (QLength(screen->display))
			select_mask = X_mask;
		else {
			write_mask = ptymask();
			XFlush(screen->display);
			select_mask = Select_mask;
			if (eventMode != NORMAL)
				select_mask = X_mask;
			if(select(max_plus1, &select_mask, &write_mask, 
				(int *)NULL, (struct timeval *) NULL) < 0){
				if (errno != EINTR)
					SysError(ERROR_SELECT);
				continue;
			} 
		}
		if (write_mask & ptymask())
			v_write(screen->respond, 0, 0);	/* flush buffer */
		if(select_mask & X_mask) {
			if (bcnt <= 0) {
				bcnt = 0;
				bptr = buffer;
			}
			xevents();
			if (bcnt > 0)
				break;
		}
	}
	bcnt--;
	return(*bptr++);
}

/*
 * process a string of characters according to the character set indicated
 * by charset.  worry about end of line conditions (wraparound if selected).
 */
dotext(screen, flags, charset, buf, ptr)
register TScreen	*screen;
unsigned	flags;
char		charset;
char	*buf;
char	*ptr;
{
	register char	*s;
	register int	len;
	register int	n;
	register int	next_col;

	switch (charset) {
	case 'A':	/* United Kingdom set			*/
		for (s=buf; s<ptr; ++s)
			if (*s == '#')
				*s = '\036';	/* UK pound sign*/
		break;

	case 'B':	/* ASCII set				*/
		break;

	case '0':	/* special graphics (line drawing)	*/
		for (s=buf; s<ptr; ++s)
			if (*s>=0x5f && *s<=0x7e)
				*s = *s == 0x5f ? 0x7f : *s - 0x5f;
		break;

	default:	/* any character sets we don't recognize*/
		return;
	}

	len = ptr - buf; 
	ptr = buf;
	while (len > 0) {
		n = screen->max_col-screen->cur_col+1;
		if (n <= 1) {
			if (screen->do_wrap && (flags&WRAPAROUND)) {
				Index(screen, 1);
				screen->cur_col = 0;
				screen->do_wrap = 0;
				n = screen->max_col+1;
			} else
				n = 1;
		}
		if (len < n)
			n = len;
		next_col = screen->cur_col + n;
		WriteText(screen, ptr, n, flags);
		/*
		 * the call to WriteText updates screen->cur_col.
		 * If screen->cur_col != next_col, we must have
		 * hit the right margin, so set the do_wrap flag.
		 */
		screen->do_wrap = (screen->cur_col < next_col);
		len -= n;
		ptr += n;
	}
}
 
/*
 * write a string str of length len onto the screen at
 * the current cursor position.  update cursor position.
 */
WriteText(screen, str, len, flags)
register TScreen	*screen;
register char	*str;
register int	len;
unsigned	flags;
{
	register int cx, cy;
	register unsigned fgs = flags;
	GC	currentGC;
 
   if(screen->cur_row - screen->topline <= screen->max_row) {
	/*
	if(screen->cur_row == screen->cursor_row && screen->cur_col <=
	 screen->cursor_col && screen->cursor_col <= screen->cur_col + len - 1)
		screen->cursor_state = OFF;
	 */
	if(screen->cursor_state)
		HideCursor();

	/*
	 *	make sure that the correct GC is current
	 */

	if (fgs & BOLD)
		if (fgs & INVERSE)
			currentGC = screen->reverseboldGC;
		else	currentGC = screen->normalboldGC;
	else  /* not bold */
		if (fgs & INVERSE)
			currentGC = screen->reverseGC;
		else	currentGC = screen->normalGC;

	if (fgs & INSERT)
		InsertChar(screen, len);
      if (!(AddToRefresh(screen))) {
		if(screen->scroll_amt)
			FlushScroll(screen);
	cx = CursorX(screen, screen->cur_col);
	cy = CursorY(screen, screen->cur_row)+screen->fnt_norm->max_bounds.ascent;
 	XDrawImageString(screen->display, TextWindow(screen), currentGC,
			cx, cy, str, len);

	if((fgs & BOLD) && screen->enbolden) 
		if (currentGC == screen->normalGC || screen->reverseGC)
			XDrawString(screen->display, TextWindow(screen),
			      	currentGC,cx + 1, cy, str, len);

	if(fgs & UNDERLINE) 
		XDrawLine(screen->display, TextWindow(screen), currentGC,
			cx, cy+1,
			cx + len * FontWidth(screen), cy+1);
	/*
	 * the following statements compile data to compute the average 
	 * number of characters written on each call to XText.  The data
	 * may be examined via the use of a "hidden" escape sequence.
	 */
	ctotal += len;
	++ntotal;
      }
    }
	ScreenWrite(screen, str, flags, len);
	CursorForward(screen, len);
}
 
/*
 * process ANSI modes set, reset
 */
modes(term, func)
Terminal	*term;
int		(*func)();
{
	register int	i;

	for (i=0; i<nparam; ++i) {
		switch (param[i]) {
		case 4:			/* IRM				*/
			(*func)(&term->flags, INSERT);
			break;

		case 20:		/* LNM				*/
			(*func)(&term->flags, LINEFEED);
			break;
		}
	}
}

/*
 * process DEC private modes set, reset
 */
dpmodes(term, func)
Terminal	*term;
int		(*func)();
{
	register TScreen	*screen	= &term->screen;
	register int	i, j;
	extern int bitset();

	for (i=0; i<nparam; ++i) {
		switch (param[i]) {
		case 1:			/* DECCKM			*/
			(*func)(&term->keyboard.flags, CURSOR_APL);
			break;
		case 3:			/* DECCOLM			*/
			if(screen->c132) {
				ClearScreen(screen);
				CursorSet(screen, 0, 0, term->flags);
				if((j = func == bitset ? 132 : 80) !=
				 ((term->flags & IN132COLUMNS) ? 132 : 80) ||
				 j != screen->max_col + 1) {
					XResizeWindow(
					    screen->display,
					    TextWindow(screen),
					    (unsigned) FontWidth(screen) * j
					        + 2*screen->border
						+ screen->scrollbar,
					    (unsigned) FontHeight(screen)
						* (screen->max_row + 1)
						+ 2 * screen->border);
					XSync(screen->display, FALSE);
					if(QLength(screen->display) > 0)
						xevents();
				}
				(*func)(&term->flags, IN132COLUMNS);
			}
			break;
		case 4:			/* DECSCLM (slow scroll)	*/
			if (func == bitset) {
				screen->jumpscroll = 0;
				if (screen->scroll_amt)
					FlushScroll(screen);
			} else
				screen->jumpscroll = 1;
			(*func)(&term->flags, SMOOTHSCROLL);
			break;
		case 5:			/* DECSCNM			*/
			j = term->flags;
			(*func)(&term->flags, REVERSE_VIDEO);
			if ((term->flags ^ j) & REVERSE_VIDEO)
				ReverseVideo(term);
			break;

		case 6:			/* DECOM			*/
			(*func)(&term->flags, ORIGIN);
			CursorSet(screen, 0, 0, term->flags);
			break;

		case 7:			/* DECAWM			*/
			(*func)(&term->flags, WRAPAROUND);
			break;
		case 8:			/* DECARM			*/
			j = term->flags;
			(*func)(&term->flags, AUTOREPEAT);
			if ((term->flags ^ j) & AUTOREPEAT)
				if(term->flags & AUTOREPEAT)
					XAutoRepeatOn(screen->display);
				else
					XAutoRepeatOff(screen->display);
			break;
		case 9:			/* MIT bogus sequence		*/
			if(func == bitset)
				screen->send_mouse_pos = 1;
			else
				screen->send_mouse_pos = 0;
			break;
		case 38:		/* DECTEK			*/
			if(func == bitset & !(screen->inhibit & I_TEK)) {
				if(screen->logging) {
					FlushLog(screen);
					screen->logstart = Tbuffer;
				}
				screen->TekEmu = TRUE;
			}
			break;
		case 40:		/* 132 column mode		*/
			(*func)(&screen->c132, 1);
			break;
		case 41:		/* curses hack			*/
			(*func)(&screen->curses, 1);
			break;
		case 44:		/* margin bell			*/
			(*func)(&screen->marginbell, 1);
			if(!screen->marginbell)
				screen->bellarmed = -1;
			break;
		case 45:		/* reverse wraparound	*/
			(*func)(&term->flags, REVERSEWRAP);
			break;
		case 46:		/* logging		*/
			if(func == bitset)
				StartLog(screen);
			else
				CloseLog(screen);
			break;
		case 47:		/* alternate buffer		*/
			if(func == bitset)
				ToAlternate(screen);
			else
				FromAlternate(screen);
			break;
		case 1000:		/* xtem bogus sequence		*/
			if(func == bitset)
				screen->send_mouse_pos = 2;
			else
				screen->send_mouse_pos = 0;
			break;
		case 1001:		/* xtem sequence w/hilite tracking */
			if(func == bitset)
				screen->send_mouse_pos = 3;
			else
				screen->send_mouse_pos = 0;
			break;
		}
	}
}

/*
 * process xterm private modes save
 */
savemodes(term)
Terminal *term;
{
	register TScreen	*screen	= &term->screen;
	register int i;

	for (i = 0; i < nparam; i++) {
		switch (param[i]) {
		case 1:			/* DECCKM			*/
			screen->save_modes[0] = term->keyboard.flags &
			 CURSOR_APL;
			break;
		case 3:			/* DECCOLM			*/
			if(screen->c132)
				screen->save_modes[1] = term->flags &
				 IN132COLUMNS;
			break;
		case 4:			/* DECSCLM (slow scroll)	*/
			screen->save_modes[2] = term->flags & SMOOTHSCROLL;
			break;
		case 5:			/* DECSCNM			*/
			screen->save_modes[3] = term->flags & REVERSE_VIDEO;
			break;
		case 6:			/* DECOM			*/
			screen->save_modes[4] = term->flags & ORIGIN;
			break;

		case 7:			/* DECAWM			*/
			screen->save_modes[5] = term->flags & WRAPAROUND;
			break;
		case 8:			/* DECARM			*/
			screen->save_modes[6] = term->flags & AUTOREPEAT;
			break;
		case 9:			/* mouse bogus sequence */
			screen->save_modes[7] = screen->send_mouse_pos;
			break;
		case 40:		/* 132 column mode		*/
			screen->save_modes[8] = screen->c132;
			break;
		case 41:		/* curses hack			*/
			screen->save_modes[9] = screen->curses;
			break;
		case 44:		/* margin bell			*/
			screen->save_modes[12] = screen->marginbell;
			break;
		case 45:		/* reverse wraparound	*/
			screen->save_modes[13] = term->flags & REVERSEWRAP;
			break;
		case 46:		/* logging		*/
			screen->save_modes[14] = screen->logging;
			break;
		case 47:		/* alternate buffer		*/
			screen->save_modes[15] = screen->alternate;
			break;
		case 1000:		/* mouse bogus sequence		*/
		case 1001:
			screen->save_modes[7] = screen->send_mouse_pos;
			break;
		}
	}
}

/*
 * process xterm private modes restore
 */
restoremodes(term)
Terminal *term;
{
	register TScreen	*screen	= &term->screen;
	register int i, j;

	for (i = 0; i < nparam; i++) {
		switch (param[i]) {
		case 1:			/* DECCKM			*/
			term->keyboard.flags &= ~CURSOR_APL;
			term->keyboard.flags |= screen->save_modes[0] &
			 CURSOR_APL;
			break;
		case 3:			/* DECCOLM			*/
			if(screen->c132) {
				ClearScreen(screen);
				CursorSet(screen, 0, 0, term->flags);
				if((j = (screen->save_modes[1] & IN132COLUMNS)
				 ? 132 : 80) != ((term->flags & IN132COLUMNS)
				 ? 132 : 80) || j != screen->max_col + 1) {
					XResizeWindow(
					    screen->display,
					    TextWindow(screen),
					    (unsigned) FontWidth(screen) * j 
						+ 2*screen->border
						+ screen->scrollbar,
					    (unsigned) FontHeight(screen)
						* (screen->max_row + 1)
						+ 2*screen->border);
					XSync(screen->display, FALSE);	/* synchronize */
					if(QLength(screen->display) > 0)
						xevents();
				}
				term->flags &= ~IN132COLUMNS;
				term->flags |= screen->save_modes[1] &
				 IN132COLUMNS;
			}
			break;
		case 4:			/* DECSCLM (slow scroll)	*/
			if (screen->save_modes[2] & SMOOTHSCROLL) {
				screen->jumpscroll = 0;
				if (screen->scroll_amt)
					FlushScroll(screen);
			} else
				screen->jumpscroll = 1;
			term->flags &= ~SMOOTHSCROLL;
			term->flags |= screen->save_modes[2] & SMOOTHSCROLL;
			break;
		case 5:			/* DECSCNM			*/
			if((screen->save_modes[3] ^ term->flags) &
			 REVERSE_VIDEO) {
				term->flags &= ~REVERSE_VIDEO;
				term->flags |= screen->save_modes[3] &
				 REVERSE_VIDEO;
				ReverseVideo(term);
			}
			break;
		case 6:			/* DECOM			*/
			term->flags &= ~ORIGIN;
			term->flags |= screen->save_modes[4] & ORIGIN;
			CursorSet(screen, 0, 0, term->flags);
			break;

		case 7:			/* DECAWM			*/
			term->flags &= ~WRAPAROUND;
			term->flags |= screen->save_modes[5] & WRAPAROUND;
			break;
		case 8:			/* DECARM			*/
			if((screen->save_modes[6] ^ term->flags) & AUTOREPEAT) {
				term->flags &= ~REVERSE_VIDEO;
				term->flags |= screen->save_modes[6] &
				 REVERSE_VIDEO;
				if(term->flags & AUTOREPEAT)
					XAutoRepeatOn(screen->display);
				else
					XAutoRepeatOff(screen->display);
			}
			break;
		case 9:			/* MIT bogus sequence		*/
			screen->send_mouse_pos = screen->save_modes[7];
			break;
		case 40:		/* 132 column mode		*/
			screen->c132 = screen->save_modes[8];
			break;
		case 41:		/* curses hack			*/
			screen->curses = screen->save_modes[9];
			break;
		case 44:		/* margin bell			*/
			if(!(screen->marginbell = screen->save_modes[12]))
				screen->bellarmed = -1;
			break;
		case 45:		/* reverse wraparound	*/
			term->flags &= ~REVERSEWRAP;
			term->flags |= screen->save_modes[13] & REVERSEWRAP;
			break;
		case 46:		/* logging		*/
			if(screen->save_modes[14])
				StartLog(screen);
			else
				CloseLog(screen);
			break;
		case 47:		/* alternate buffer		*/
			if(screen->save_modes[15])
				ToAlternate(screen);
			else
				FromAlternate(screen);
			break;
		case 1000:		/* mouse bogus sequence		*/
		case 1001:
			screen->send_mouse_pos = screen->save_modes[7];
			break;
		}
	}
}

/*
 * set a bit in a word given a pointer to the word and a mask.
 */
bitset(p, mask)
int	*p;
{
	*p |= mask;
}

/*
 * clear a bit in a word given a pointer to the word and a mask.
 */
bitclr(p, mask)
int	*p;
{
	*p &= ~mask;
}

unparseseq(ap, fd)
register ANSI	*ap;
{
	register int	c;
	register int	i;
	register int	inters;

	c = ap->a_type;
	if (c>=0x80 && c<=0x9F) {
		unparseputc(ESC, fd);
		c -= 0x40;
	}
	unparseputc(c, fd);
	c = ap->a_type;
	if (c==ESC || c==DCS || c==CSI || c==OSC || c==PM || c==APC) {
		if (ap->a_pintro != 0)
			unparseputc((char) ap->a_pintro, fd);
		for (i=0; i<ap->a_nparam; ++i) {
			if (i != 0)
				unparseputc(';', fd);
			unparseputn((unsigned int) ap->a_param[i], fd);
		}
		inters = ap->a_inters;
		for (i=3; i>=0; --i) {
			c = (inters >> (8*i)) & 0xff;
			if (c != 0)
				unparseputc(c, fd);
		}
		unparseputc((char) ap->a_final, fd);
	}
}

unparseputn(n, fd)
unsigned int	n;
int fd;
{
	unsigned int	q;

	q = n/10;
	if (q != 0)
		unparseputn(q, fd);
	unparseputc((char) ('0' + (n%10)), fd);
}

unparseputc(c, fd)
char c;
int fd;
{
	char	buf[2];
	register i = 1;
	extern Terminal term;

	if((buf[0] = c) == '\r' && (term.flags & LINEFEED)) {
		buf[1] = '\n';
		i++;
	}
	if (write(fd, buf, i) != i)
		Panic("unparseputc: error writing character\n", 0);
}

ToAlternate(screen)
register TScreen *screen;
{
	extern ScrnBuf Allocate();

	if(screen->alternate)
		return;
	if(!screen->altbuf)
		screen->altbuf = Allocate(screen->max_row + 1, screen->max_col
		 + 1);
	SwitchBufs(screen);
	screen->alternate = TRUE;
}

FromAlternate(screen)
register TScreen *screen;
{
	if(!screen->alternate)
		return;
	screen->alternate = FALSE;
	SwitchBufs(screen);
}

SwitchBufs(screen)
register TScreen *screen;
{
	register int rows, top;
	char *save [2 * MAX_ROWS];

	if(screen->cursor_state)
		HideCursor();
	rows = screen->max_row + 1;
	bcopy((char *)screen->buf, (char *)save, 2 * sizeof(char *) * rows);
	bcopy((char *)screen->altbuf, (char *)screen->buf, 2 * sizeof(char *) *
	 rows);
	bcopy((char *)save, (char *)screen->altbuf, 2 * sizeof(char *) * rows);

	if((top = -screen->topline) <= screen->max_row) {
		if(screen->scroll_amt)
			FlushScroll(screen);
		if(top == 0)
			XClearWindow(screen->display, TextWindow(screen));
		else
			XClearArea(
			    screen->display,
			    TextWindow(screen),
			    (int) screen->border + screen->scrollbar,
			    (int) top * FontHeight(screen) + screen->border,
			    (unsigned) Width(screen),
			    (unsigned) (screen->max_row - top + 1)
				* FontHeight(screen),
			    FALSE);
	}
	ScrnRefresh(screen, 0, 0, rows, screen->max_col + 1);
}

VTRun()
{
	register TScreen *screen = &term.screen;
	register int i;
	
	if(!VWindow(screen) && !VTInit()) {
		if(TWindow(screen)) {
			screen->TekEmu = TRUE;
			return;
		}
		Exit(ERROR_VINIT);
	}
	screen->cursor_state = OFF;
	screen->cursor_set = ON;
	XMapWindow(screen->display, VWindow(screen));
	if(screen->select)
		VTSelect();
	if (L_flag > 0) {
		XWarpPointer (screen->display, None, VWindow(screen),
			    0, 0, 0, 0,
			    FullWidth(screen) >> 1, FullHeight(screen) >>1);
		L_flag = -1;
	}
	bcnt = 0;
	bptr = buffer;
	while(Tpushb > Tpushback) {
		*bptr++ = *--Tpushb;
		bcnt++;
	}
	bcnt += (i = Tbcnt);
	for( ; i > 0 ; i--)
		*bptr++ = *Tbptr++;
	bptr = buffer;
	if(!setjmp(VTend))
		VTparse();
	HideCursor();
	screen->cursor_set = OFF;
	VTUnselect();
}

/*ARGSUSED*/
XtEventReturnCode VTExpose(event, eventdata)
register XExposeEvent *event;
caddr_t eventdata;
{
	register TScreen *screen = &term.screen;

#ifdef DEBUG
	if(debug)
		fputs("Expose\n", stderr);
#endif DEBUG
	if (event->type == Expose) {
		HandleExposure (screen, (XExposeEvent *)event);
		return(XteventHandled);
	}
	if (screen->incopy <= 0) {
		screen->incopy = 1;
		if (screen->scrolls > 0)
			screen->scrolls--;
	}
	if (event->type == GraphicsExpose)
	  if (HandleExposure (screen, (XExposeEvent *)event))
		screen->cursor_state = OFF;
	if ((event->type == NoExpose) || event->count == 0) {
		if (screen->incopy <= 0 && screen->scrolls > 0)
			screen->scrolls--;
		if (screen->scrolls)
			screen->incopy = -1;
		else
			screen->incopy = 0;
	}
	return (XteventHandled);
}



/*ARGSUSED*/
XtEventReturnCode VTConfigure(event, eventdata)
XConfigureEvent *event;
caddr_t eventdata;
{
	register TScreen *screen = &term.screen;

	if (event->type ==ConfigureNotify ) {
		ScreenResize (screen, event->width, event->height, &term.flags);
		return(XteventHandled);
	}	
	if (event->type != UnmapNotify) return(XteventNotHandled);
#ifdef DEBUG
	if(debug)
		fputs("UnmapWindow VT\n", stderr);
#endif DEBUG
	if(screen->Tshow) {
		screen->Tshow = FALSE;
		XUnmapWindow(screen->display,  screen->fullTwin.window );
		SyncUnmap( screen->fullTwin.window, TWINDOWEVENTS );
	}
	reselectwindow(screen);
	return (XteventHandled);
}



VTInit()
{
	unsigned int width, height;
	register TScreen *screen = &term.screen;
	register int i, j;
	XPoint	*vp;
	static short failed;
	int x, y, root_x, root_y, xpos, ypos, pr;
	Window win, root;
	extern char *malloc();
	XGCValues		xgcv;
	GCMask			mask;
	XSizeHints		sizehints;
	XWMHints		wmhints;
	extern int		VTgcFontMask;

	if(failed)
		return(FALSE);
	
	TabReset (term.tabs);

	screen->fnt_norm = screen->fnt_bold = NULL;
	
	/* do the XFont calls */

	if ((screen->fnt_norm = XLoadQueryFont(screen->display, f_n)) == NULL) {
	    fprintf(stderr, "%s: Could not open font %s; using server default\n", 
		    xterm_name, f_n);
	    screen->fnt_norm =
		  XQueryFont(screen->display,
			     DefaultGC(screen->display,
				       DefaultScreen(screen->display)
				       )->gid
			     );
	    VTgcFontMask = 0;
	}

	if (!f_b || !VTgcFontMask
	    || !(screen->fnt_bold = XLoadQueryFont(screen->display, f_b))) {
		screen->fnt_bold = screen->fnt_norm;
		screen->enbolden = TRUE;  /*no bold font */
	}

	/* find the max width and higth of the font */

	screen->fullVwin.f_width = screen->fnt_norm->max_bounds.width;
	screen->fullVwin.f_height = screen->fnt_norm->max_bounds.ascent +
				    screen->fnt_norm->max_bounds.descent;

	/* making cursor -- won't work yet */

	if (XStrCmp(curs_shape, "arrow") == 0) {
		screen->curs = make_arrow(screen->mousecolor,
			screen->background);
	} else {
		screen->curs = make_xterm(screen->mousecolor,
			screen->background);
	}

	i = 2 * screen->border + screen->scrollbar;
	j = 2 * screen->border;


	/* set defaults */
	xpos = 1; ypos = 1; width = 80; height = 24;

	pr = XParseGeometry(geo_metry, &xpos, &ypos, &width, &height);

	screen->max_col = width;
	screen->max_row = height;
	width = width * screen->fullVwin.f_width + i;
	height = height * screen->fullVwin.f_height + j;

	if ((pr & XValue) && (XNegative&pr)) 
	  xpos += DisplayWidth(screen->display, DefaultScreen(screen->display)) 
			- width;
	if ((pr & YValue) && (YNegative&pr))
	  ypos += DisplayHeight(screen->display, DefaultScreen(screen->display)) 
			- height;

	/* set up size hints for window manager */
	sizehints.min_width = 2 * screen->border + screen->scrollbar;
	sizehints.min_height = 2 * screen->border;
	sizehints.width_inc = FontWidth(screen);
	sizehints.height_inc = FontHeight(screen);
	sizehints.flags = PMinSize|PResizeInc;
	sizehints.x = xpos;
	sizehints.y = ypos;
	if ((XValue&pr) && (YValue&pr)) 
	  sizehints.flags |= USPosition;
	else sizehints.flags |= PPosition;
	sizehints.width = width;
	sizehints.height = height;
	if ((WidthValue&pr) && (HeightValue&pr)) 
	  sizehints.flags |= USSize;
	else sizehints.flags |= PSize;

        screen->fullVwin.fullwidth = width;
        screen->fullVwin.fullheight = height;
        screen->fullVwin.width = width - i;
        screen->fullVwin.height = height - j;

	
	if((screen->fullVwin.window = XtCreateWindow(
	    screen->display,
	    DefaultRootWindow(screen->display), 
	    xpos, ypos,
	    width, height,
	    screen->borderwidth, screen->bordercolor,
	    screen->background, NorthWestGravity)) == NULL) {
		fprintf(stderr, "%s: Can't create VT window\n");
		return(FALSE);
	}
	/* crock to make configure notifies get through */
	XtMakeMaster(screen->display, screen->fullVwin.window);

	/* set size hints */
	XSetNormalHints(screen->display, VWindow(screen), &sizehints);
	/* do the GC stuff */

	mask = VTgcFontMask | GCForeground | GCBackground 
	  	| GCGraphicsExposures | GCFunction;

	xgcv.graphics_exposures = TRUE;	/* default */
	xgcv.function = GXcopy;
	xgcv.font = screen->fnt_norm->fid;
	xgcv.foreground = screen->foreground;
	xgcv.background = screen->background;

	screen->normalGC = XtGetGC(
	    screen->display,
	    (XContext)NULL, 
	    DefaultRootWindow(screen->display),
	    mask,
	    &xgcv);

	if (screen->enbolden) { /* there is no bold font */
		xgcv.font = screen->fnt_norm->fid;
		screen->normalboldGC = screen->normalGC;
	} else {
		xgcv.font = screen->fnt_bold->fid;
		screen->normalboldGC = XtGetGC(
		    screen->display,
		    (XContext)NULL,
		    DefaultRootWindow(screen->display),
		    mask,
		    &xgcv);
	}

	xgcv.font = screen->fnt_norm->fid;
	xgcv.foreground = screen->background;
	xgcv.background = screen->foreground;

	screen->reverseGC = XtGetGC(
	    screen->display,
	    (XContext)NULL,
	    DefaultRootWindow(screen->display),
	    mask,
	    &xgcv);

	if (screen->enbolden) /* there is no bold font */
		xgcv.font = screen->fnt_norm->fid;
	else
		xgcv.font = screen->fnt_bold->fid;

	screen->reverseboldGC = XtGetGC(
	    screen->display,
	    (XContext)NULL,
	    DefaultRootWindow(screen->display),
	    mask,
	    &xgcv);

	/* we also need a set of caret (called a cursor here) gc's */

	xgcv.font = screen->fnt_norm->fid;

	if (screen->cursorcolor != screen->foreground) {
		xgcv.foreground = screen->cursorcolor;
		xgcv.background = screen->foreground;
	} else {
		xgcv.foreground = screen->foreground;
		xgcv.background = screen->background;
	}

	screen->cursorGC = XtGetGC(
	    screen->display,
	    (XContext)NULL,
	    DefaultRootWindow(screen->display),
	    mask,
	    &xgcv);

	if (screen->cursorcolor != screen->background) {
		xgcv.foreground = screen->cursorcolor;
		xgcv.background = screen->background;
	} else {
		xgcv.foreground = screen->background;
		xgcv.background = screen->foreground;
	}

	screen->reversecursorGC = XtGetGC(
	    screen->display,
	    (XContext)NULL,
	    DefaultRootWindow(screen->display),
	    mask,
	    &xgcv);


	/* setup toolkit stuff */

	XtSetNameAndClass(screen->display, screen->fullVwin.window, 
	  nameList, classList);
	XtSetEventHandler(screen->display, VWindow(screen), 
	 (XtEventHandler) HandleEnterWindow, EnterWindowMask, (caddr_t)NULL);
	XtSetEventHandler(screen->display, VWindow(screen), 
	 (XtEventHandler) HandleLeaveWindow, LeaveWindowMask, (caddr_t)NULL);
	XtSetEventHandler(screen->display, VWindow(screen), 
	 (XtEventHandler) HandleFocusChange, FocusChangeMask, (caddr_t)NULL);
	XtSetEventHandler(screen->display, VWindow(screen),
	 (XtEventHandler) VTButtonPressed, ButtonPressMask, (caddr_t)NULL);
	XtSetEventHandler(screen->display, VWindow(screen),
	 (XtEventHandler) VTButtonReleased, ButtonReleaseMask, (caddr_t)NULL);
	XtSetEventHandler(screen->display, VWindow(screen), 
	 (XtEventHandler) VTMouseMoved, ButtonMotionMask, (caddr_t)NULL);
	XtSetEventHandler(screen->display, VWindow(screen), 
	 (XtEventHandler) VTExpose, ExposureMask, (caddr_t)NULL);
	XtSetEventHandler(screen->display, VWindow(screen), 
	 (XtEventHandler) VTConfigure, StructureNotifyMask, (caddr_t)NULL);
	XtSetEventHandler(screen->display, VWindow(screen), 
	 (XtEventHandler) HandleKeyPressed, KeyPressMask, (caddr_t)NULL);

	/*
	 * XCreateTerm flushes all events, which might include an EnterWindow
	 * or LeaveWindow.  So if the cursor is not where it is supposed to
	 * be, we set select to the appropriate thing.
	 */
	if(TWindow(screen) && XQueryPointer(
	    screen->display,
	    DefaultRootWindow(screen->display), 
	    &root, &win,
	    &root_x, &root_y,
	    &x, &y,
	    &mask)) {
		if(win == TWindow(screen))
			screen->select |= INWINDOW;
		else
			screen->select &= ~INWINDOW;
	}

	/* Reset variables used by ANSI emulation. */

	screen->gsets[0] = 'B';			/* ASCII_G		*/
	screen->gsets[1] = 'B';
	screen->gsets[2] = 'B';			/* DEC supplemental.	*/
	screen->gsets[3] = 'B';
	screen->curgl = 0;			/* G0 => GL.		*/
	screen->curgr = 2;			/* G2 => GR.		*/
	screen->curss = 0;			/* No single shift.	*/

	wmhints.flags = StateHint | InputHint;
	wmhints.input = True;
	if (iconstartup) wmhints.initial_state = IconicState;
	else wmhints.initial_state = NormalState;
        if(icon_geom) {
	   wmhints.icon_x = 1; wmhints.icon_y = 1; width = 1; height = 1;
	   pr = XParseGeometry(
	       icon_geom,
	       &wmhints.icon_x, &wmhints.icon_y, 
	       &width, &height);
           if((pr & XValue) && (pr & XNegative))
              wmhints.icon_x += 
		 DisplayWidth(screen->display,DefaultScreen(screen->display)) 
		  - width - 2 * screen->borderwidth;
           if((pr & YValue) && (pr & YNegative))
              wmhints.icon_y += 
	         DisplayHeight(screen->display,DefaultScreen(screen->display))
		  - height - 2 * screen->borderwidth;
	   if ((XValue&pr) && (YValue&pr)) 
	    wmhints.flags |= IconPositionHint;
        }
	XSetWMHints(screen->display, VWindow(screen), &wmhints);

	XDefineCursor(screen->display, VWindow(screen), screen->curs );
	XStoreName (screen->display, VWindow(screen), screen->titlename);
	XChangeProperty(
	    screen->display, VWindow(screen),
	    XA_WM_ICON_NAME, XA_STRING,
	    8, PropModeReplace,
	    (unsigned char *)screen->iconname, screen->iconnamelen);

        screen->cur_col = screen->cur_row = 0;
	screen->max_col = Width(screen)  / screen->fullVwin.f_width - 1;
	screen->top_marg = 0;
	screen->bot_marg = screen->max_row = Height(screen) /
				screen->fullVwin.f_height - 1;

	screen->sc.row = screen->sc.col = screen->sc.flags = NULL;

	/* allocate memory for screen buffer */
	screen->buf = screen->allbuf = (ScrnBuf) Allocate (screen->max_row + 1,
					  screen->max_col +1);

	screen->do_wrap = NULL;
	screen->scrolls = screen->incopy = 0;
/*	free((char *)fInfo);	*/
	vp = &VTbox[1];
	(vp++)->x = FontWidth(screen) - 1;
	(vp++)->y = FontHeight(screen) - 1;
	(vp++)->x = -(FontWidth(screen) - 1);
	vp->y = -(FontHeight(screen) - 1);
	screen->box = VTbox;

	screen->savelines = save_lines;
	screen->savedlines = 0;
	if(screen->scrollbar) {
		screen->scrollbar = 0;
		ScrollBarOn(screen, TRUE);
	}
	return(TRUE);
}

/*
 * Shows cursor at new cursor position in screen.
 */
ShowCursor()
{
	register TScreen *screen = &term.screen;
	register int x, y, flags, y1, x1;
	char c;
	GC	currentGC;

	if (eventMode != NORMAL) return;

	if (screen->cur_row - screen->topline > screen->max_row)
		return;
	c = screen->buf[y = 2 * (screen->cursor_row = screen->cur_row)]
	 [x = screen->cursor_col = screen->cur_col];
	flags = screen->buf[y + 1][x];
	if (c == 0)
		c = ' ';

	if(screen->select) {

		if(flags & INVERSE) { /* is reverse video */
			if (flags & BOLD) {
				currentGC = screen->normalboldGC;
			} else {
				currentGC = screen->normalGC;
			}

		} else { /* normal video */
			if (flags & BOLD) {
				currentGC = screen->reverseboldGC;
			} else {
				currentGC = screen->reverseGC;
			}
		}
	} else { /* not selected */
		if(flags & INVERSE) { /* is reverse video */
			currentGC = screen->reverseGC;
		} else { /* normal video */
			currentGC = screen->normalGC;
		}
	    
	}


	x = CursorX (screen, screen->cur_col);
	y = CursorY(screen, screen->cur_row) + 
	  screen->fnt_norm->max_bounds.ascent;
	XDrawImageString(screen->display, TextWindow(screen), currentGC,
		x, y, &c, 1);

	if((flags & BOLD) && screen->enbolden) /* no bold font */
		XDrawString(screen->display, TextWindow(screen), currentGC,
			x + 1, y, &c, 1);
	if(flags & UNDERLINE) 
		XDrawLine(screen->display, TextWindow(screen), currentGC,
			x, y+1, x + FontWidth(screen), y+1);
	if(screen->select) {
 	       y1 = y - screen->fnt_norm->max_bounds.ascent;
	       x1 = x + screen->fnt_norm->max_bounds.lbearing;
               XDrawLine(screen->display, TextWindow(screen), currentGC,
		         x1, y1, x1, y1 + FontHeight(screen)-1);
	} else {
		screen->box->x = x + screen->fnt_norm->max_bounds.lbearing;
		screen->box->y = y - screen->fnt_norm->max_bounds.ascent;
		XDrawLines(screen->display, TextWindow(screen), currentGC,
			screen->box, NBOX, CoordModePrevious);
	}
	screen->cursor_state = ON;
}

/*
 * hide cursor at previous cursor position in screen.
 */
HideCursor()
{
	register TScreen *screen = &term.screen;
	GC	currentGC;
	register int x, y, flags;
	char c;

	if(screen->cursor_row - screen->topline > screen->max_row)
		return;
	c = screen->buf[y = 2 * screen->cursor_row][x = screen->cursor_col];
	flags = screen->buf[y + 1][x];

	if(flags & INVERSE) {
		if(flags & BOLD) {
			currentGC = screen->reverseboldGC;
		} else {
			currentGC = screen->reverseGC;
		}
	} else {
		if(flags & BOLD) {
			currentGC = screen->normalboldGC;
		} else {
			currentGC = screen->normalGC;
		}
	}

	if (c == 0)
		c = ' ';
	x = CursorX (screen, screen->cursor_col);
	y = (((screen->cursor_row - screen->topline) * FontHeight(screen))) +
	 screen->border;
	y = y+screen->fnt_norm->max_bounds.ascent;
	XDrawImageString(screen->display, TextWindow(screen), currentGC,
		x, y, &c, 1);
	if((flags & BOLD) && screen->enbolden)
		XDrawString(screen->display, TextWindow(screen), currentGC,
			x + 1, y, &c, 1);
	if(flags & UNDERLINE) 
		XDrawLine(screen->display, TextWindow(screen), currentGC,
			x, y+1, x + FontWidth(screen), y+1);
	screen->cursor_state = OFF;
}

VTSelect()
{
	register TScreen *screen = &term.screen;

	XSetWindowBorder(screen->display, TextWindow(screen), screen->bordercolor);
}

VTUnselect()
{
	register TScreen *screen = &term.screen;

	XSetWindowBorderPixmap(screen->display, TextWindow(screen), 
	  screen->graybordertile);
}

VTReset(full)
int full;
{
	register TScreen *screen = &term.screen;

	/* reset scrolling region */
	screen->top_marg = 0;
	screen->bot_marg = screen->max_row;
	term.flags &= ~ORIGIN;
	if(full) {
		TabReset (term.tabs);
		term.keyboard.flags = NULL;
		screen->gsets[0] = 'B';
		screen->gsets[1] = 'B';
		screen->gsets[2] = 'B';
		screen->gsets[3] = 'B';
		screen->curgl = 0;
		screen->curgr = 2;
		screen->curss = 0;
		ClearScreen(screen);
		screen->cursor_state = OFF;
		if(!(term.flags & AUTOREPEAT))
			XAutoRepeatOn(screen->display);
		if (term.flags & REVERSE_VIDEO)
			ReverseVideo(&term);

		term.flags = term.initflags;
		if(screen->c132 && (term.flags & IN132COLUMNS)) {
			XResizeWindow(
			    screen->display,
			    TextWindow(screen),
			    (unsigned) 80*FontWidth(screen)
				+ 2 * screen->border + screen->scrollbar,
			    (unsigned) FontHeight(screen)
			        * (screen->max_row + 1) + 2 * screen->border);
			XSync(screen->display, FALSE);	/* synchronize */
			if(QLength(screen->display) > 0)
				xevents();
		}
		CursorSet(screen, 0, 0, term.flags);
	}
	longjmp(vtjmpbuf, 1);	/* force ground state in parser */
}


#ifdef MODEMENU
#define	MMENU_SCROLL		0
#define	MMENU_VIDEO		(MMENU_SCROLL+1)
#define	MMENU_WRAP		(MMENU_VIDEO+1)
#define	MMENU_REVERSEWRAP	(MMENU_WRAP+1)
#define	MMENU_NLM		(MMENU_REVERSEWRAP+1)
#define	MMENU_CURSOR		(MMENU_NLM+1)
#define	MMENU_PAD		(MMENU_CURSOR+1)
#define	MMENU_REPEAT		(MMENU_PAD+1)
#define	MMENU_SCROLLBAR		(MMENU_REPEAT+1)
#define	MMENU_SCROLLKEY		(MMENU_SCROLLBAR+1)
#define	MMENU_SCROLLINPUT	(MMENU_SCROLLKEY+1)
#define	MMENU_C132		(MMENU_SCROLLINPUT+1)
#define	MMENU_CURSES		(MMENU_C132+1)
#define	MMENU_MARGBELL		(MMENU_CURSES+1)
#define	MMENU_TEKWIN		(MMENU_MARGBELL+1)
#define	MMENU_ALTERN		(MMENU_TEKWIN+1)
#define	MMENU_LINE		(MMENU_ALTERN+1)
#define	MMENU_RESET		(MMENU_LINE+1)
#define	MMENU_FULLRESET		(MMENU_RESET+1)
#define	MMENU_TEKMODE		(MMENU_FULLRESET+1)
#define	MMENU_HIDEVT		(MMENU_TEKMODE+1)

static char *vtext[] = {
	"Jump Scroll",
	"Reverse Video",
	"Auto Wraparound",
	"Reverse Wraparound",
	"Auto Linefeed",
	"Application Cursors",
	"Application Pad",
	"Auto Repeat",
	"Scrollbar",
	"Scroll to bottom on key",
	"Scroll to bottom on input",
	"80 <-> 132 Columns",
	"Curses Emulation",
	"Margin Bell",
	"Tek Window Showing",
	"Alternate Screen",
	"-",
	"Soft Reset",
	"Full Reset",
	"Select Tek Mode",
	"Hide VT Window",
	0,
};


static int menutermflags;
static int menukbdflags;
static int t132;
static int taltern;
static int tcurses;
static int tmarginbell;
static int tscrollbar;
static int tscrollkey;
static int tscrollinput;
static int tshow;

Menu *setupmenu(menu)
register Menu **menu;
{
	register TScreen *screen = &term.screen;
	register char **cp;
	register int flags = term.flags;
	register int kflags = term.keyboard.flags;

	if (*menu == NULL) {
		if ((*menu = NewMenu("Modes", re_verse)) == NULL)
			return(NULL);
		for(cp = vtext ; *cp ; cp++)
			AddMenuItem(*menu, *cp);
		if(!(flags & SMOOTHSCROLL))
			CheckItem(*menu, MMENU_SCROLL);
		if(flags & REVERSE_VIDEO)
			CheckItem(*menu, MMENU_VIDEO);
		if(flags & WRAPAROUND)
			CheckItem(*menu, MMENU_WRAP);
		if(flags & REVERSEWRAP)
			CheckItem(*menu, MMENU_REVERSEWRAP);
		if(flags & LINEFEED)
			CheckItem(*menu, MMENU_NLM);
		if(kflags & CURSOR_APL)
			CheckItem(*menu, MMENU_CURSOR);
		if(kflags & KYPD_APL)
			CheckItem(*menu, MMENU_PAD);
		if(flags & AUTOREPEAT)
			CheckItem(*menu, MMENU_REPEAT);
		if(tscrollbar = (screen->scrollbar > 0)) {
			CheckItem(*menu, MMENU_SCROLLBAR);
			if(tscrollkey = screen->scrollkey)
				CheckItem(*menu, MMENU_SCROLLKEY);
			if(tscrollinput = screen->scrollinput)
				CheckItem(*menu, MMENU_SCROLLINPUT);
		} else {
			tscrollkey = FALSE;
			DisableItem(*menu, MMENU_SCROLLKEY);
			tscrollinput = FALSE;
			DisableItem(*menu, MMENU_SCROLLINPUT);
		}
		if(t132 = screen->c132)
			CheckItem(*menu, MMENU_C132);
		if(tcurses = screen->curses)
			CheckItem(*menu, MMENU_CURSES);
		if(tmarginbell = screen->marginbell)
			CheckItem(*menu, MMENU_MARGBELL);
		if(tshow = screen->Tshow)
			CheckItem(*menu, MMENU_TEKWIN);
		else
			DisableItem(*menu, MMENU_HIDEVT);
		DisableItem(*menu, MMENU_ALTERN);
		if(taltern = screen->alternate) {
			CheckItem(*menu, MMENU_ALTERN);
		}
		DisableItem(*menu, MMENU_LINE);
		if(screen->inhibit & I_TEK) {
			DisableItem(*menu, MMENU_TEKWIN);
			DisableItem(*menu, MMENU_TEKMODE);
		}
		menutermflags = flags;
		menukbdflags = kflags;
		return(*menu);
	}
	menutermflags ^= flags;
	menukbdflags ^= kflags;
	if (menutermflags & SMOOTHSCROLL)
		SetItemCheck(*menu, MMENU_SCROLL, !(flags & SMOOTHSCROLL));
	if (menutermflags & REVERSE_VIDEO)
		SetItemCheck(*menu, MMENU_VIDEO, flags & REVERSE_VIDEO);
	if (menutermflags & WRAPAROUND)
		SetItemCheck(*menu, MMENU_WRAP, flags & WRAPAROUND);
	if (menutermflags & REVERSEWRAP)
		SetItemCheck(*menu, MMENU_REVERSEWRAP, flags & REVERSEWRAP);
	if (menutermflags & LINEFEED)
		SetItemCheck(*menu, MMENU_NLM, flags & LINEFEED);
	if (menukbdflags & CURSOR_APL)
		SetItemCheck(*menu, MMENU_CURSOR, kflags & CURSOR_APL);
	if (menukbdflags & KYPD_APL)
		SetItemCheck(*menu, MMENU_PAD, kflags & KYPD_APL);
        if (menutermflags & AUTOREPEAT)
                SetItemCheck(*menu, MMENU_REPEAT, flags & AUTOREPEAT);
	if(tscrollbar != (screen->scrollbar > 0)) {
		SetItemCheck(*menu, MMENU_SCROLLBAR, (tscrollbar =
		 (screen->scrollbar > 0)));
		SetItemDisable(*menu, MMENU_SCROLLKEY, !tscrollbar);
		SetItemCheck(*menu, MMENU_SCROLLKEY,
		 tscrollkey = (tscrollbar && screen->scrollkey));
		SetItemDisable(*menu, MMENU_SCROLLINPUT, !tscrollbar);
		SetItemCheck(*menu, MMENU_SCROLLINPUT,
		 tscrollinput = (tscrollbar && screen->scrollinput));
	} else if (tscrollbar) {
		if (tscrollkey != screen->scrollkey)
			SetItemCheck(*menu, MMENU_SCROLLKEY,
			 tscrollkey = screen->scrollkey);
		if (tscrollinput != screen->scrollinput)
			SetItemCheck(*menu, MMENU_SCROLLINPUT,
			 tscrollinput = screen->scrollinput);
	}
	if(t132 != screen->c132)
		SetItemCheck(*menu, MMENU_C132, (t132 = screen->c132));
	if(tcurses != screen->curses)
		SetItemCheck(*menu, MMENU_CURSES, (tcurses = screen->curses));
	if(tmarginbell != screen->marginbell)
		SetItemCheck(*menu, MMENU_MARGBELL, (tmarginbell =
		screen->marginbell));
	if(tshow != screen->Tshow) {
		SetItemCheck(*menu, MMENU_TEKWIN, (tshow = screen->Tshow));
		SetItemDisable(*menu, MMENU_HIDEVT, !tshow);
	}
	if(taltern != screen->alternate) {
		SetItemCheck(*menu, MMENU_ALTERN, (taltern =
		 screen->alternate));
	}
	menutermflags = flags;
	menukbdflags = kflags;
	return(*menu);
}

domenufunc(item)
int item;
{
	register TScreen *screen = &term.screen;

	switch (item) {
	case MMENU_SCROLL:
		term.flags ^= SMOOTHSCROLL;
		if (term.flags & SMOOTHSCROLL) {
			screen->jumpscroll = FALSE;
			if (screen->scroll_amt)
				FlushScroll(screen);
		} else
			screen->jumpscroll = TRUE;
		break;

	case MMENU_VIDEO:
		term.flags ^= REVERSE_VIDEO;
		ReverseVideo(&term);
		break;

	case MMENU_WRAP:
		term.flags ^= WRAPAROUND;
		break;

	case MMENU_REVERSEWRAP:
		term.flags ^= REVERSEWRAP;
		break;

	case MMENU_NLM:
		term.flags ^= LINEFEED;
		break;

	case MMENU_CURSOR:
		term.keyboard.flags ^= CURSOR_APL;
		break;

	case MMENU_PAD:
		term.keyboard.flags ^= KYPD_APL;
		break;

	case MMENU_REPEAT:
		term.flags ^= AUTOREPEAT;
		if (term.flags & AUTOREPEAT)
			XAutoRepeatOn(screen->display);
		else
			XAutoRepeatOff(screen->display);
		break;

	case MMENU_SCROLLBAR:
		if(screen->scrollbar)
			ScrollBarOff(screen);
		else
			ScrollBarOn(screen, FALSE);
		break;

	case MMENU_SCROLLKEY:
		screen->scrollkey = !screen->scrollkey;
		break;

	case MMENU_SCROLLINPUT:
		screen->scrollinput = !screen->scrollinput;
		break;

		
	case MMENU_C132:
		screen->c132 = !screen->c132;
		break;

	case MMENU_MARGBELL:
		if(!(screen->marginbell = !screen->marginbell))
			screen->bellarmed = -1;
		break;

	case MMENU_CURSES:
		screen->curses = !screen->curses;
		break;

	case MMENU_FULLRESET:
		VTReset(TRUE);
		break;

	case MMENU_RESET:
		VTReset(FALSE);
		break;

	case MMENU_HIDEVT:
		XUnmapWindow(screen->display, VWindow(screen));
		reselectwindow(screen);
		SyncUnmap(VWindow(screen), WINDOWEVENTS);
			/* drop through */
	case MMENU_TEKMODE:
		if(!screen->TekEmu) {
			if(screen->logging) {
				FlushLog(screen);
				screen->logstart = Tbuffer;
			}
			screen->TekEmu = TRUE;
			longjmp(VTend, 1);
		} else
			XRaiseWindow(screen->display, TWindow(screen));
		break;

	case MMENU_TEKWIN:
		if(screen->Tshow = !screen->Tshow) {
			if(TWindow(screen) || TekInit()) {
				XMapWindow(screen->display, TWindow(screen));
				screen->Tshow = TRUE;
			}
		} else {
			screen->Tshow = FALSE;
			XUnmapWindow(screen->display, TWindow(screen));
			SyncUnmap(TWindow(screen), TWINDOWEVENTS);
			if(screen->TekEmu) {
				if(screen->logging) {
					FlushLog(screen);
					screen->logstart = buffer;
				}
				longjmp(Tekend, 1);
			}
		}
		reselectwindow(screen);
		break;
	}
}
#endif MODEMENU

