/***********************************************************
Copyright 1987 by Digital Equipment Corporation, Maynard, Massachusetts,
and the Massachusetts Institute of Technology, Cambridge, Massachusetts.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the names of Digital or MIT not be
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
/* $XConsortium: fonttype.h,v 1.3 88/10/10 16:48:00 rws Exp $ */

#ifndef FONTTYPE_H
#define FONTTYPE_H

#include "font.h"
#include "misc.h"

/*
 * Parameterize the procedures for internalizing a font dependent upon the
 * extension of the file name. This is somewhat OS dependent, but most
 * OSs support this kind of thing.
 */
typedef Mask	(*ReadFontProc)(/* file, ppFont, ppCS, tables, params */);
typedef	void	(*FreeFontProc)(/* pFont */);

#define	NullReadFontProc	((ReadFontProc)NULL)
#define	NullFreeFontProc	((FreeFontProc)NULL)

typedef struct _FontFileReader {
    char *		extension;
    ReadFontProc	loadFont;
    FreeFontProc	freeFont;
    char **		filter;
} FontFileReaderRec, *FontFileReader;

extern FontFileReaderRec fontFileReaders[];

#endif /* FONTTYPE_H */
