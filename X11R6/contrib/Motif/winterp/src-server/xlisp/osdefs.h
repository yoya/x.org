/* -*-C-*-
********************************************************************************
*
* File:         osdefs.h
* RCS:          $Header: /users/npm/src/winterp/src-server/xlisp/RCS/osdefs.h,v 2.4 1994/06/06 15:59:09 npm Exp $
* Description:  system specific function declarations -- for 'xlisp' not 'winterp'
* Author:       David Michael Betz. WINTERP portions by Niels Mayer;
*		XLISP-PLUS by Tom Almy with contributions from Johnny
*		Greenblatt, Neal Holtz, Niels Mayer, Blake McBride, Mikael
*		Pettersson, Luke Tierney, Ken Whedbee, Pete Yadlowsky.
* Created:      
* Modified:     Mon Jun  6 03:04:52 1994 (Niels Mayer) npm@indeed
* Language:     C
* Package:      N/A
* Status:       X11r6 contrib release
*
* Copyright (C) 1994, Enterprise Integration Technologies Corp. and Niels Mayer.
* WINTERP 1.15-1.99, Copyright (c) 1993, Niels P. Mayer.
* WINTERP 1.0-1.14, Copyright (c) 1989-1992 Hewlett-Packard Co. and Niels Mayer.
* XLISP-STAT 2.1 Copyright (c) 1990, by Luke Tierney
* XLISP version 2.1, Copyright (c) 1989, by David Betz.
*
* Permission to use, copy, modify, distribute, and sell this software and its
* documentation for any purpose is hereby granted without fee, provided that
* the above copyright notice appear in all copies and that both that
* copyright notice and this permission notice appear in supporting
* documentation, and that the name of Enterprise Integration Technologies,
* Hewlett-Packard Company, Niels Mayer, Luke Tierney and David Betz not be
* used in advertising or publicity pertaining to distribution of the software
* without specific, written prior permission.  Enterprise Integration
* Technologies, Hewlett-Packard Company, Niels Mayer, Luke Tierney and David
* Betz make no representations about the suitability of this software for any
* purpose. It is provided "as is" without express or implied warranty.
*
* ENTERPRISE INTEGRATION TECHNOLOGIES, HEWLETT-PACKARD COMPANY, NIELS MAYER,
* LUKE TIERNEY AND DAVID BETZ DISCLAIM ALL WARRANTIES WITH REGARD TO THIS
* SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS,
* IN NO EVENT SHALL ENTERPRISE INTEGRATION TECHNOLOGIES, HEWLETT-PACKARD
* COMPANY, NIELS MAYER, LUKE TIERNEY NOR DAVID BETZ BE LIABLE FOR ANY SPECIAL,
* INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
* LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
* OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
* PERFORMANCE OF THIS SOFTWARE.
*
********************************************************************************
*/

/*
*------------------------------------------------------------------------------
* See ./winterp/COPYRIGHT for information on contacting the authors.
* Please e-mail comments, modifications, questions, improvements and
* bugfixes to the WINTERP mailing list winterp@netcom.com. Please send 
* mailing list subscribe/unsubscribe notices to winterp-request@netcom.com .
* Post XLISP-specific questions/information to the USENET newsgroup
* comp.lang.lisp.x.
*------------------------------------------------------------------------------
*/

extern LVAL xsystem(V);

#if !(defined(UNIX) || defined(WINTERP))
extern LVAL xgetkey(V);
#endif /* !(defined(UNIX) || defined(WINTERP)) */

#ifdef GRAPHICS
extern LVAL xmode(V), xcolor(V), xmove(V), xdraw(V),
    xmoverel(V), xdrawrel(V);
extern LVAL xcls(V), xcleol(V), xgotoxy(V);
#endif

#if (defined(UNIX) || defined(WINTERP))
extern LVAL
  Prim_POPEN(V), Prim_PCLOSE(V), Prim_SYSTEM(V), /* NPM */
  Prim_FSCANF_FIXNUM(V), Prim_FSCANF_STRING(V),	Prim_FSCANF_FLONUM(V); /* NPM */
#endif /* (defined(UNIX) || defined(WINTERP)) */
