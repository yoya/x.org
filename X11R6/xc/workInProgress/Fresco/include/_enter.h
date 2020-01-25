/* $XConsortium: _enter.h,v 1.3 94/06/03 21:42:38 matt Exp $ */

/*
 * Copyright (c) 1987-91 Stanford University
 * Copyright (c) 1991-93 Silicon Graphics, Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software and 
 * its documentation for any purpose is hereby granted without fee, provided
 * that (i) the above copyright notices and this permission notice appear in
 * all copies of the software and related documentation, and (ii) the names of
 * Stanford and Silicon Graphics may not be used in any advertising or
 * publicity relating to the software without the specific, prior written
 * permission of Stanford and Silicon Graphics.
 * 
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND, 
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  
 *
 * IN NO EVENT SHALL STANFORD OR SILICON GRAPHICS BE LIABLE FOR
 * ANY SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND,
 * OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF 
 * LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE 
 * OF THIS SOFTWARE.
 */

#ifndef xf__scope_h
#define xf__scope_h

#if !defined(UNIXCPP)
#define _lib_fresco(name) Xf##name
#else
#define _lib_fresco(name) Xf/**/name
#endif

#ifndef MAKEDEPEND

#include <X11/Fresco/_undefs.h>
#include <X11/Fresco/_defines.h>

#endif

#include <X11/Fresco/Ox/base.h>

/*
 * Have to repeat this here (nil is also defined by Ox/base.h) because
 * we can't re-included base.h in sequence _enter.h, _leave.h, _enter.h.
 */
#ifndef nil
#define nil 0
#endif

#endif
