/*
 * Copyright (c) 1990, 1991 Stanford University
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of Stanford not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  Stanford makes no representations about
 * the suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * STANFORD DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.
 * IN NO EVENT SHALL STANFORD BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION
 * WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/*
 * Interface to Label, an object derived from Graphic.
 */

#ifndef unidraw_graphic_label_h
#define unidraw_graphic_label_h

#include <Unidraw/Graphic/graphic.h>

class Label : public Graphic {
public:
    Label(const char*, Graphic* = nil);
    virtual ~Label();

    const char* GetOriginal();

    virtual void SetFont(PSFont*);
    virtual PSFont* GetFont();
    virtual PSPattern* GetPattern();            // diallows patterned text
                                                // to work around IV botch
    virtual Graphic* Copy();
protected:
    virtual void getExtent(float&, float&, float&, float&, float&, Graphic*);
    virtual boolean contains(PointObj&, Graphic*);
    virtual boolean intersects(BoxObj&, Graphic*);
    virtual void draw(Canvas*, Graphic*);
protected:
    char* _string;
    PSFont* _font;
};

inline const char* Label::GetOriginal () { return _string; }

#endif
