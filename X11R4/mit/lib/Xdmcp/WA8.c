/*
 * $XConsortium: WA8.c,v 1.1 89/09/14 17:11:25 keith Exp $
 *
 * Copyright 1989 Massachusetts Institute of Technology
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of M.I.T. not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission.  M.I.T. makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * M.I.T. DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL M.I.T.
 * BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN 
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Author:  Keith Packard, MIT X Consortium
 */

#include    "Xos.h"
#include    "X.h"
#include    "Xmd.h"
#include    "Xdmcp.h"

int
XdmcpWriteARRAY8 (buffer, array)
    XdmcpBufferPtr  buffer;
    ARRAY8Ptr	    array;
{
    int	i;

    if (!XdmcpWriteCARD16 (buffer, array->length))
	return FALSE;
    for (i = 0; i < array->length; i++)
	if (!XdmcpWriteCARD8 (buffer, array->data[i]))
	    return FALSE;
    return TRUE;
}
