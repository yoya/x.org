#ifndef RT_CURS
#define RT_CURS 1

/***********************************************************
		Copyright IBM Corporation 1987

                      All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the name of IBM not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.  

IBM DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
IBM BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/
/* $Header:rtcursor.h 6.0$ */
/* $ACIS:rtcursor.h 6.0$ */
/* $Source: /vice/X11/src/server/ddx/ibm/rt/RCS/rtcursor.h,v $ */

#if !defined(lint) && !defined(LOCORE)  && defined(RCS_HDRS)
static char *rcsidrtcursor = "$Header:rtcursor.h 6.0$";
#endif

extern	Bool	rtRealizeCursor();
extern	Bool	rtUnrealizeCursor();
extern	int	rtDisplayCursor();
extern	int	rtSetCursorPosition();
extern	void	rtCursorLimits();
extern	void	rtPointerNonInterestBox();
extern	void	rtConstrainCursor();

#endif /* RT_CURS */
