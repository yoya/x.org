/* $XConsortium: resource.h,v 1.2 94/04/17 21:16:51 dpw Exp $ */
/***********************************************************

Copyright (c) 1987, 1989  X Consortium

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


Copyright 1987, 1989 by Digital Equipment Corporation, Maynard, Massachusetts.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the name of Digital not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.  

DIGITAL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/
#ifndef RESOURCE_H
#define RESOURCE_H 1
#include "misc.h"
#ifdef XTHREADS
#include "pixmap.h"     /* DrawablePtr */
#include "window.h"     /* WindowPtr */
#include "gcstruct.h"   /* GCPtr */
#endif /* XTHREADS */

/*****************************************************************
 * STUFF FOR RESOURCES 
 *****************************************************************/

/* classes for Resource routines */

typedef unsigned long RESTYPE;

#define RC_VANILLA	((RESTYPE)0)
#define RC_CACHED	((RESTYPE)1<<31)
#define RC_DRAWABLE	((RESTYPE)1<<30)
/*  Use class RC_NEVERRETAIN for resources that should not be retained
 *  regardless of the close down mode when the client dies.  (A client's
 *  event selections on objects that it doesn't own are good candidates.)
 *  Extensions can use this too!
 */
#define RC_NEVERRETAIN	((RESTYPE)1<<29)
#define RC_LASTPREDEF	RC_NEVERRETAIN
#define RC_ANY		(~(RESTYPE)0)

/* types for Resource routines */

#define RT_WINDOW	((RESTYPE)1|RC_CACHED|RC_DRAWABLE)
#define RT_PIXMAP	((RESTYPE)2|RC_CACHED|RC_DRAWABLE)
#define RT_GC		((RESTYPE)3|RC_CACHED)
#define RT_FONT		((RESTYPE)4)
#define RT_CURSOR	((RESTYPE)5)
#define RT_COLORMAP	((RESTYPE)6)
#define RT_CMAPENTRY	((RESTYPE)7)
#define RT_OTHERCLIENT	((RESTYPE)8|RC_NEVERRETAIN)
#define RT_PASSIVEGRAB	((RESTYPE)9|RC_NEVERRETAIN)
#define RT_LASTPREDEF	((RESTYPE)9)
#define RT_NONE		((RESTYPE)0)

/* bits and fields within a resource id */
#define CLIENTOFFSET 22					/* client field */
#define RESOURCE_ID_MASK	0x3FFFFF		/* low 22 bits */
#define CLIENT_BITS(id) ((id) & 0x1fc00000)		/* hi 7 bits */
#define CLIENT_ID(id) ((int)(CLIENT_BITS(id) >> CLIENTOFFSET))
#define SERVER_BIT		0x20000000		/* use illegal bit */

#ifdef INVALID
#undef INVALID	/* needed on HP/UX */
#endif

/* Invalid resource id */
#define INVALID	(0)

#define BAD_RESOURCE 0xe0000000

/*
 * XXX:SM Moved from resource.c, this needs to be here.  Function prototyping.
 */
#ifdef XTHREADS
typedef enum
{
    reader_writer_lock,
    exclusive_lock,
    no_lock,
    read_lock,
    write_lock
} ResourceLockType;
#endif


typedef int (*DeleteType)(
#if NeedNestedPrototypes
    pointer /*value*/,
    XID /*id*/
#endif
);

#ifdef XTHREADS
extern RESTYPE CreateNewResourceType(
#if NeedFunctionPrototypes
    DeleteType /*deleteFunc*/,
    ResourceLockType /*lockType*/,
    int /*lockOffset*/,
    Bool /*shared*/
#endif
);
#else /*XTHREADS*/
extern RESTYPE CreateNewResourceType(
#if NeedFunctionPrototypes
    DeleteType /*deleteFunc*/
#endif
);
#endif /*XTHREADS*/

extern RESTYPE CreateNewResourceClass(
#if NeedFunctionPrototypes
void
#endif
);

extern Bool InitClientResources(
#if NeedFunctionPrototypes
    ClientPtr /*client*/
#endif
);

extern XID FakeClientID(
#if NeedFunctionPrototypes
    int /*client*/
#endif
);

extern Bool AddResource(
#if NeedFunctionPrototypes
    XID /*id*/,
    RESTYPE /*type*/,
    pointer /*value*/
#endif
);

extern void FreeResource(
#if NeedFunctionPrototypes
    XID /*id*/,
    RESTYPE /*skipDeleteFuncType*/
#endif
);

extern void FreeResourceByType(
#if NeedFunctionPrototypes
    XID /*id*/,
    RESTYPE /*type*/,
    Bool /*skipFree*/
#endif
);

extern Bool ChangeResourceValue(
#if NeedFunctionPrototypes
    XID /*id*/,
    RESTYPE /*rtype*/,
    pointer /*value*/
#endif
);

extern void FreeClientNeverRetainResources(
#if NeedFunctionPrototypes
    ClientPtr /*client*/
#endif
);

extern void FreeClientResources(
#if NeedFunctionPrototypes
    ClientPtr /*client*/
#endif
);

extern void FreeAllResources(
#if NeedFunctionPrototypes
void
#endif
);

extern Bool LegalNewID(
#if NeedFunctionPrototypes
    XID /*id*/,
    ClientPtr /*client*/
#endif
);

extern pointer LookupIDByType(
#if NeedFunctionPrototypes
    XID /*id*/,
    RESTYPE /*rtype*/
#endif
);

extern pointer LookupIDByClass(
#if NeedFunctionPrototypes
    XID /*id*/,
    RESTYPE /*classes*/
#endif
);

extern void GetXIDRange(
#if NeedFunctionPrototypes
    int /*client*/,
    Bool /*server*/,
    XID * /*minp*/,
    XID * /*maxp*/
#endif
);

extern unsigned int GetXIDList(
#if NeedFunctionPrototypes
    ClientPtr /*pClient*/,
    unsigned int /*count*/,
    XID * /*pids*/
#endif
);

#ifdef XTHREADS
extern void UnlockDrawableAndGC(
#if NeedFunctionPrototypes
    DrawablePtr /*pDraw*/,
    GCPtr /**pGC*/,
    XID /*drawID*/,
    XID /*gcID*/ 
#endif
);
extern void UnlockTwoDrawablesAndGC(
#if NeedFunctionPrototypes
    DrawablePtr /*pSrc*/,
    DrawablePtr /*pDst*/,
    GCPtr /**pGC*/,
    XID /*srcID*/,
    XID /*dstID*/,
    XID /*gcID*/ 
#endif
);
extern void UnlockDrawable(
#if NeedFunctionPrototypes
    DrawablePtr /*pDraw*/,
    XID /*drawID*/ 
#endif
);
extern void UnlockGC(
#if NeedFunctionPrototypes
    GCPtr /**pGC*/,
    XID /*gcID*/ 
#endif
);
extern void UnlockWindow(
#if NeedFunctionPrototypes
    WindowPtr /*pWin*/,
    XID /*winID*/ 
#endif
);
#endif /* XTHREADS */


#endif /* RESOURCE_H */

