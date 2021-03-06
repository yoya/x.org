/*
 *	$XConsortium: input.c,v 1.5 88/09/06 17:08:05 jim Exp $
 */

#ifndef lint
static char *rcsid_input_c = "$XConsortium: input.c,v 1.5 88/09/06 17:08:05 jim Exp $";
#endif	/* lint */

#include <X11/copyright.h>

/*
 *	Copyright 1989 OMRON TATEISI ELECTRONICS CO.
 *
 *	Original Source: ./client/kterm/input.c
 *
 *	Modified by OMRON TATEISI Electronics CO., to handle XJp-library.
 *
 *	Author:	Seiji Kuwari	OMRON TATEISI ELECTRONICS CO.
 *				kuwa@frf.omron.junet
 *				uunet!nff.ncl.omron.co.jp!kuwa
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

/* input.c */

#ifndef lint
static char rcs_id[] = "$XConsortium: input.c,v 1.5 88/09/06 17:08:05 jim Exp $";
#endif	/* lint */

#ifdef FACOM
#include <sys/types.h>
#endif

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/DECkeysym.h>
#include <X11/Intrinsic.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include "ptyx.h"

#ifdef	SX9100
extern XtermWidget term;
#ifndef XK_Eisuu_Lock
#define XK_Eisuu_Lock	0xFF2F
#endif
#define	IsKanaKey(keysym) \
	(((unsigned)(keysym) >= 0x47e) && ((unsigned)(keysym) <= 0x4df))
#define	IsKanjiKey(keysym) \
	(((unsigned)(keysym) >= XK_Multi_key && (unsigned)(keysym) <= XK_Eisuu_Lock))
#endif	/* SX9100 */

int MetaMode = 1;	/* prefix with ESC when Meta Key is down */

static XComposeStatus compose_status = {NULL, 0};
static char *kypd_num = " XXXXXXXX\tXXX\rXXXxxxxXXXXXXXXXXXXXXXXXXXXX*+,-.\\0123456789XXX=";
static char *kypd_apl = " ABCDEFGHIJKLMNOPQRSTUVWXYZ??????abcdefghijklmnopqrstuvwxyzXXX";
static char *cur = "DACB";

static int funcvalue(), sunfuncvalue();
extern Boolean sunFunctionKeys;

void
AdjustAfterInput (screen)
register TScreen *screen;
{
	if(screen->scrollkey && screen->topline != 0)
		WindowScroll(screen, 0);
	if(screen->marginbell) {
		int col = screen->max_col - screen->nmarginbell;
		if(screen->bellarmed >= 0) {
			if(screen->bellarmed == screen->cur_row) {
				if(screen->cur_col >= col) {
					if(screen->cur_col == col)
						Bell();
					screen->bellarmed = -1;
				}
			} else
				screen->bellarmed = screen->cur_col <
				 col ? screen->cur_row : -1;
		} else if(screen->cur_col < col)
			screen->bellarmed = screen->cur_row;
	}
}

Input (keyboard, screen, event)
register TKeyboard	*keyboard;
register TScreen		*screen;
register XKeyPressedEvent *event;
{

#define STRBUFSIZE 100

	 char strbuf[STRBUFSIZE];
	register char *string;
	register int key = FALSE;
	int	pty	= screen->respond;
	int	nbytes;
	int 	keycode;
	ANSI	reply;

	nbytes = XLookupString (event, strbuf, STRBUFSIZE, 
		&keycode, &compose_status);

	string = &strbuf[0];
	reply.a_pintro = 0;
	reply.a_final = 0;
	reply.a_nparam = 0;
	reply.a_inters = 0;

	if (IsPFKey(keycode)) {
		reply.a_type = SS3;
		unparseseq(&reply, pty);
		unparseputc((char)(keycode-XK_KP_F1+'P'), pty);
		key = TRUE;
	} else if (IsKeypadKey(keycode)) {
	  	if (keyboard->flags & KYPD_APL)	{
			reply.a_type   = SS3;
			unparseseq(&reply, pty);
			unparseputc(kypd_apl[keycode-XK_KP_Space], pty);
		} else
			unparseputc(kypd_num[keycode-XK_KP_Space], pty);
		key = TRUE;
        } else if (IsCursorKey(keycode) &&
        	keycode != XK_Prior && keycode != XK_Next) {
#ifdef	SX9100
		if (event->state & Mod1Mask) {
			reply.a_type = CSI;
			reply.a_nparam = 1;
			reply.a_param[0] = funcvalue(keycode, event->state);
			reply.a_final = '~';
			if (reply.a_param[0] > 0)
				unparseseq(&reply, pty);
		} else
#endif	/* SX9100 */
       		if (keyboard->flags & CURSOR_APL) {
			reply.a_type = SS3;
			unparseseq(&reply, pty);
			unparseputc(cur[keycode-XK_Left], pty);
		} else {
			reply.a_type = CSI;
			reply.a_final = cur[keycode-XK_Left];
			unparseseq(&reply, pty);
		}
		key = TRUE;
#ifdef	SX9100
	 } else if (IsFunctionKey(keycode) || IsMiscFunctionKey(keycode) ||
	 	keycode == XK_Prior || keycode == XK_Next ||
	 	keycode == DXK_Remove || IsKanjiKey(keycode)) {
#else	/* SX9100 */
	 } else if (IsFunctionKey(keycode) || IsMiscFunctionKey(keycode) ||
	 	keycode == XK_Prior || keycode == XK_Next ||
	 	keycode == DXK_Remove) {
#endif	/* SX9100 */
		reply.a_type = CSI;
		reply.a_nparam = 1;
		if (sunFunctionKeys) {
		    reply.a_param[0] = sunfuncvalue (keycode);
		    reply.a_final = 'z';
		} else {
#ifdef	SX9100
		    reply.a_param[0] = funcvalue(keycode, event->state);
#else	/* SX9100 */
		    reply.a_param[0] = funcvalue (keycode);
#endif	/* SX9100 */
		    reply.a_final = '~';
		}
		if (reply.a_param[0] > 0)
			unparseseq(&reply, pty);
		key = TRUE;
#ifdef	SX9100
	} else if (IsKanaKey(keycode)) {
		if (term->flags & EUC_KANJI) {
			unparseputc(SS2, pty);
		}
		unparseputc((keycode & 0xff), pty);
			/* Becouse XLookupString (V11R3) convert
			   only convert Latin-1 keysyms to string */
		key = TRUE;
#endif	/* SX9100 */
	} else if (nbytes > 0) {
		if(screen->TekGIN) {
			TekEnqMouse(*string++);
			TekGINoff();
			nbytes--;
		}
#ifdef	SX9100
		if (keycode == 0 && *string == NULL)
			return;
#endif	/* SX9100 */
		if ((nbytes == 1) && MetaMode && (event->state & Mod1Mask))
			unparseputc(033, pty);
		while (nbytes-- > 0)
			unparseputc(*string++, pty);
		key = TRUE;
	}
	if(key && !screen->TekEmu)
	        AdjustAfterInput(screen);
#ifdef ENABLE_PRINT
	if (keycode == XK_F2) TekPrint();
#endif
	return;
}

StringInput (screen, string)
register TScreen	*screen;
register char *string;
{
	int	pty	= screen->respond;
	int	nbytes;

	nbytes = strlen(string);
	if(nbytes && screen->TekGIN) {
		TekEnqMouse(*string++);
		TekGINoff();
		nbytes--;
	}
	while (nbytes-- > 0)
		unparseputc(*string++, pty);
	if(!screen->TekEmu)
	        AdjustAfterInput(screen);
}

#ifndef	SX9100
static int funcvalue (keycode)
	int keycode;
{
	switch (keycode) {
		case XK_F1:	return(11);
		case XK_F2:	return(12);
		case XK_F3:	return(13);
		case XK_F4:	return(14);
		case XK_F5:	return(15);
		case XK_F6:	return(17);
		case XK_F7:	return(18);
		case XK_F8:	return(19);
		case XK_F9:	return(20);
		case XK_F10:	return(21);
		case XK_F11:	return(23);
		case XK_F12:	return(24);
		case XK_F13:	return(25);
		case XK_F14:	return(26);
		case XK_F15:	return(28);
		case XK_Help:	return(28);
		case XK_F16:	return(29);
		case XK_Menu:	return(29);
		case XK_F17:	return(31);
		case XK_F18:	return(32);
		case XK_F19:	return(33);
		case XK_F20:	return(34);

		case XK_Find :	return(1);
		case XK_Insert:	return(2);
		case XK_Delete:	return(3);
		case DXK_Remove: return(3);
		case XK_Select:	return(4);
		case XK_Prior:	return(5);
		case XK_Next:	return(6);
		default:	return(-1);
	}
}
#else	/* SX9100 */
static int funcvalue(keycode, state)
int	keycode;
int	state;
{
	if (state & Mod1Mask) {
		switch(keycode) {
			case XK_F11:	return(43);
			case XK_F12:	return(44);
			case XK_F13:	return(45);
			case XK_F14:	return(46);
			case XK_Left:	return(47);
			case XK_Up:	return(48);
			case XK_Right:	return(49);
			case XK_Down:	return(50);
			default:	return(-1);
		}
	} else {
		switch (keycode) {
			case XK_F1:	return(11);
			case XK_F2:	return(12);
			case XK_F3:	return(13);
			case XK_F4:	return(14);
			case XK_F5:	return(15);
			case XK_F6:	return(17);
			case XK_F7:	return(18);
			case XK_F8:	return(19);
			case XK_F9:	return(20);
			case XK_F10:	return(21);
			case XK_F11:	return(23);
			case XK_F12:	return(24);
			case XK_F13:	return(25);
			case XK_F14:	return(26);
			case XK_F15:	return(28);
			case XK_Help:	return(28);
			case XK_F16:	return(29);
			case XK_Menu:	return(29);
			case XK_F17:	return(31);
			case XK_F18:	return(32);
			case XK_F19:	return(33);
			case XK_F20:	return(34);
			case XK_Kanji:	return(35);
			case XK_Execute:	return(36);
	
			case XK_Find :	return(1);
			case XK_Insert:	return(2);
			case XK_Delete:	return(3);
			case XK_Select:	return(4);
			case XK_Prior:	return(5);
			case XK_Next:	return(6);
			default:	return(-1);
		}
	}
}
#endif	/* SX9100 */


static int sunfuncvalue (keycode)
	int keycode;
  {
  	switch (keycode) {
		case XK_F1:	return(224);
		case XK_F2:	return(225);
		case XK_F3:	return(226);
		case XK_F4:	return(227);
		case XK_F5:	return(228);
		case XK_F6:	return(229);
		case XK_F7:	return(230);
		case XK_F8:	return(231);
		case XK_F9:	return(232);
		case XK_F10:	return(233);
		case XK_F11:	return(192);
		case XK_F12:	return(193);
		case XK_F13:	return(194);
		case XK_F14:	return(195);
		case XK_F15:	return(196);
		case XK_Help:	return(196);
		case XK_F16:	return(197);
		case XK_Menu:	return(197);
		case XK_F17:	return(198);
		case XK_F18:	return(199);
		case XK_F19:	return(200);
		case XK_F20:	return(201);

		case XK_R1:	return(208);
		case XK_R2:	return(209);
		case XK_R3:	return(210);
		case XK_R4:	return(211);
		case XK_R5:	return(212);
		case XK_R6:	return(213);
		case XK_R7:	return(214);
		case XK_R8:	return(215);
		case XK_R9:	return(216);
		case XK_R10:	return(217);
		case XK_R11:	return(218);
		case XK_R12:	return(219);
		case XK_R13:	return(220);
		case XK_R14:	return(221);
		case XK_R15:	return(222);
  
		case XK_Find :	return(1);
		case XK_Insert:	return(2);
		case XK_Delete:	return(3);
		case DXK_Remove: return(3);
		case XK_Select:	return(4);
		case XK_Prior:	return(5);
		case XK_Next:	return(6);
		default:	return(-1);
	}
}

