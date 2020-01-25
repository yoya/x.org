/* $XConsortium: HPproto.h,v 3.2 94/04/17 20:53:52 rws Exp $ */

/*
 * Copyright (c) 1992 by Sun Microsystems, Inc.
 * All Rights Reserved
 */

/***********************************************************

Copyright (c) 1989, 1990  X Consortium

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

Copyright 1989, 1990 by Sun Microsystems, Inc. 

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the name of Sun Microsystems,
not be used in advertising or publicity pertaining to distribution of 
the software without specific, written prior permission.  

SUN MICROSYSTEMS DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, 
INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT 
SHALL SUN MICROSYSTEMS BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL 
DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/

/* Definitions for the PEX used by server and c bindings */

/*
 * This packet-construction scheme makes the following assumptions:
 *
 * 1. The compiler is able to generate code which addresses one- and two-byte
 * quantities.  In the worst case, this would be done with bit-fields.  If 
 * bit-fields are used it may be necessary to reorder the request fields in
 * this file, depending on the order in which the machine assigns bit fields
 * to machine words.  There may also be a problem with sign extension, as K+R 
 * specify that bitfields are always unsigned.
 *
 * 2. 2- and 4-byte fields in packet structures must be ordered by hand such 
 * that they are naturally-aligned, so that no compiler will ever insert 
 * padding bytes.
 *
 * 3. All packets are hand-padded to a multiple of 4 bytes, for the same reason.
 */

#ifndef HPPROTO_H
#define HPPROTO_H

/* In the following typedefs, comments appear that say "LISTof Foo( numItems )",
 * "CLIST of Foo()", and "SINGLE Foo()".   These are used when the protocol 
 * specifies that a request or reply contains a variable length list of 
 * (possibly variable types of) objects.
 *
 * A LISTof list is one for which we have already been given the length.
 * The items in the list are of type "Foo". The number of items in the list
 * appears parenthetically after the type.  ("numItems" in our example.)
 * Any other information needed to parse the list is also passed in the
 * parentheses. (E.g., "tableType" in a list of table entries.)
 *
 * A CLISTof list is the same, except that the first 4 bytes of the list
 * indicate the number of items in the list.  The length may need to be
 * byte-swapped.
 *
 * A SINGLE item of an indeterminate length is indicated in the same
 * manner.  (E.g., a "SINGLE TableEntry()".) Any other information
 * needed to parse the item is also passed in the parentheses.
 * (E.g., "itemMask" in a set of pipeline context attributes.)
 *
 * If no information is given in the parentheses, then the size is
 * implicit.
 *
 * Variable length padding is noted with a comment, with the number
 * of bytes of padding required as calculated from the value in
 * the parentheses.  (number of bytes of padding = n?(3-((n-1)%4):0 , where
 * n is the parenthetical value.)
 */

#include "X11/PEX5/HPpex.h"
#include "X11/PEX5/PEXproto.h"

/* The HP "draw to front buffer" escape */

typedef struct {
    Drawable	drawable;
    int		render_to_front_buffer;
} hpEscapeSetRenderingBuffer;

#endif /* HPPROTO_H */
