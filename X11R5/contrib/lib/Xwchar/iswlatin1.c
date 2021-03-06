/*
 * $XConsortium: iswlatin1.c,v 1.2 91/07/01 11:00:56 xguest Exp $
 */

/*
 * Copyright 1991 by OMRON Corporation
 * Copyright 1991 by the Massachusetts Institute of Technology
 * 
 * Permission to use, copy, modify, distribute, and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and that
 * both that copyright notice and this permission notice appear in
 * supporting documentation, and that the names of OMRON and M.I.T.  not
 * be used in advertising or publicity pertaining to distribution of the
 * software without specific, written prior permission. OMRON and M.I.T. 
 * make no representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 * OMRON AND M.I.T. DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL OMRON OR M.I.T. BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
 * USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 *	Author: Hiroshi Kuribayashi	OMRON Corporation
 *
 */

#include "wcharint.h"

/*
 * true if wc is DIS 10646's latin1, 
 * It's includes C0/C1/DEL(is it OK?) i.e,
 *	code from 0x20202000 to 0x2020207f
 */

#if NeedFunctionPrototypes
 int iswlatin1(register wint_t wc)
#else
 int iswlatin1(wc)
 register wint_t wc;
#endif
{
    return _iswlatin1(wc);
}
