/* $Author: */

/*
	$Disclaimer: 
 * Permission to use, copy, modify, and distribute this software and its 
 * documentation for any purpose is hereby granted without fee, 
 * provided that the above copyright notice appear in all copies and that 
 * both that copyright notice, this permission notice, and the following 
 * disclaimer appear in supporting documentation, and that the names of 
 * IBM, Carnegie Mellon University, and other copyright holders, not be 
 * used in advertising or publicity pertaining to distribution of the software 
 * without specific, written prior permission.
 * 
 * IBM, CARNEGIE MELLON UNIVERSITY, AND THE OTHER COPYRIGHT HOLDERS 
 * DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING 
 * ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.  IN NO EVENT 
 * SHALL IBM, CARNEGIE MELLON UNIVERSITY, OR ANY OTHER COPYRIGHT HOLDER 
 * BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY 
 * DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, 
 * WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS 
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE 
 * OF THIS SOFTWARE.
 *  $
*/

#ifndef NORCSID
#define NORCSID
static char rcsid[]="$Header: /afs/cs.cmu.edu/project/atk-dist/auis-6.3/contrib/mit/neos/RCS/blank.c,v 1.3 1992/12/15 21:55:51 rr2b R6tape $";
#endif


 

/*
 * blank.c
 *
 * Make a blank rectangle in an lset.
 */

/* ************************************************************
 *  Copyright (C) 1989, 1990, 1991
 *  by the Massachusetts Institute of Technology
 *   For full copyright information see:'mit-copyright.h'     *
 *        This class by Nick Williams, June 1990  
 ************************************************************ */

#include <mit-copyright.h>
#include <class.h>
#include "view.ih"
#include "blank.eh"
#include "graphic.ih"

void
blank__FullUpdate(self, type, left, top, width, height)
register struct blank *self;
enum view_UpdateType type;
long left, top, width, height;
{
    blank_EraseVisualRect(self);
}

void
blank__Update(self)
register struct blank *self;
{
    struct rectangle r;
    blank_GetLogicalBounds(self, &r);
    blank_FullUpdate(self, view_FullRedraw, r.left, r.top, r.width, r.height);
}
