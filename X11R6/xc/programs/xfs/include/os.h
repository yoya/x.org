/* $XConsortium: os.h,v 1.9 94/09/16 19:00:32 kaleb Exp $ */
/*
Copyright (c) 1987  X Consortium

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
 * Copyright 1990, 1991 Network Computing Devices;
 * Portions Copyright 1987 by Digital Equipment Corporation 
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the names of Network Computing Devices,
 * or Digital not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  Network Computing Devices, or Digital
 * make no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * NETWORK COMPUTING DEVICES, AND DIGITAL DISCLAIM ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS, IN NO EVENT SHALL NETWORK COMPUTING DEVICES, OR DIGITAL BE
 * LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * $NCDId: @(#)os.h,v 4.2 1991/05/10 07:59:16 lemke Exp $
 *
 */

#ifndef	_OS_H_
#define	_OS_H_

#include "FSproto.h"
#include "misc.h"

#ifdef INCLUDE_ALLOCA_H
#include <alloca.h>
#else
extern char *alloca();

#endif

#define	MAX_REQUEST_SIZE	16384

extern unsigned long *FSalloc();
extern unsigned long *FSrealloc();
extern void FSfree();

#define	fsalloc(size)		FSalloc((unsigned long)size)
#define	fsrealloc(ptr, size)	FSrealloc((pointer)ptr, (unsigned long)size)
#define	fsfree(ptr)		FSfree((pointer)ptr)

#ifndef NO_ALLOCA
/*
 * os-dependent definition of local allocation and deallocation
 * If you want something other than FSalloc/FSfree for ALLOCATE/DEALLOCATE
 * LOCAL then you add that in here.
 */
#ifdef __HIGHC__

#ifndef NCR
extern char *alloca();

#if HCVERSION < 21003
#define ALLOCATE_LOCAL(size)    alloca((int)(size))
pragma on(alloca);
#else /* HCVERSION >= 21003 */
#define ALLOCATE_LOCAL(size)    _Alloca((int)(size))
#endif /* HCVERSION < 21003 */
#else /* NCR */
#define ALLOCATE_LOCAL(size)	alloca(size)
#endif /* NCR */

#define DEALLOCATE_LOCAL(ptr)	/* as nothing */

#endif /* __HIGHC__ */

#ifdef __GNUC__
#define alloca __builtin_alloca
#else

/*
 * warning: old mips alloca (pre 2.10) is unusable, new one is builtin
 * Test is easy, the new one is named __builtin_alloca and comes
 * from alloca.h which #defines alloca.
 */
#ifndef NCR
#if defined(vax) || defined(sun) || defined(apollo) || defined(stellar) || defined(alloca)
#define	ALLOCATE_LOCAL(size)		alloca((int)size)
#define	DEALLOCATE_LOCAL(ptr)
#endif
#endif /* NCR */
#endif /* __GNUC__ */

#endif /* NO_ALLOCA */

#ifndef ALLOCATE_LOCAL
#define	ALLOCATE_LOCAL(size)	FSalloc((unsigned long)size)
#define	DEALLOCATE_LOCAL(ptr)	FSfree((pointer)ptr)
#endif				/* ALLOCATE_LOCAL */

int         ReadRequest();

Bool        CloseDownConnection();
void        CreateSockets();
void        FlushAllOuput();
long        GetTimeInMIllis();
void        Error();
void        InitErrors();
void        CloseErrors();
void        NoticeF();
void        ErrorF();
void        FatalError();
void        SetConfigValues();

typedef pointer FID;
typedef struct _FontPathRec *FontPathPtr;

FontPathPtr expand_font_name_pattern();

typedef struct _alt_server *AlternateServerPtr;
typedef struct _auth *AuthPtr;

extern int  ListCatalogues();
extern int  ListAlternateServers();

#endif				/* _OS_H_ */
